
/*
Copyright (C) <2012> <Kieren 'Razish' McDevitt>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/



#include "common.h"
#include "game/game.h"
#include "renderer/renderer.h"
#include "API/SDL/include/SDL_opengl.h"
#include "API/SDL/include/SDL_keyboard.h"
#include "API/SDL/include/SDL_events.h"
#include "utils/string.h"
#include "utils/hash.h"

#define CARD_SHUFFLE_POOL (208)

#define NUM_WASTE_PILES (2)
#define NUM_FOUNDATION_PILES (4)
#define NUM_TABLEAU_PILES (7)

#define CARD_ACTUAL_WIDTH (200.0f)
#define CARD_ACTUAL_HEIGHT (271.0f)
#define CARD_SCALE ((SCREEN_WIDTH/13.0f)/CARD_ACTUAL_WIDTH)
#define CARD_WIDTH (CARD_ACTUAL_WIDTH*CARD_SCALE)
#define CARD_HEIGHT (CARD_ACTUAL_HEIGHT*CARD_SCALE)

#define WASTE_Y (CARD_HEIGHT*0.25f)
#define WASTE_X( index ) (((SCREEN_WIDTH/NUM_TABLEAU_PILES)-(CARD_WIDTH/2.0f))*(index+1))

#define FOUNDATION_Y (CARD_HEIGHT*0.25f)
#define FOUNDATION_X( index ) (((SCREEN_WIDTH/NUM_TABLEAU_PILES)-(CARD_WIDTH/2.0f))*(index+4)) //( ( ( (SCREEN_WIDTH/2.0f) / NUM_FOUNDATION_PILES ) - (CARD_WIDTH/2.0f) )*(index+1))

#define TABLEAU_Y (CARD_HEIGHT*1.5f)
#define TABLEAU_X( index ) (((SCREEN_WIDTH/NUM_TABLEAU_PILES)-(CARD_WIDTH/2.0f))*(index+1))
#define TABLEAU_Y_OFFSET (24)

static card_t cards[CARD_NUM_CARDS] = { { 0 } };
static cardStack_t *foundation[NUM_FOUNDATION_PILES] = { NULL };
static cardStack_t *tableau[NUM_TABLEAU_PILES] = { NULL };
static cardStack_t *selectedStack = NULL;
static cardStack_t *waste[NUM_WASTE_PILES] = { NULL };

static int faceDownTexture = -1;
static int foundationTexture = -1;

game_t game = { 0 };

stringID_table_t cardNames[CARD_NUM_CARDS] = 
{//They must be in this order! Black magic is used to guess a correct move
	//Black
	ENUM2STRING(CARD_SPADES_ACE),
	ENUM2STRING(CARD_SPADES_2),
	ENUM2STRING(CARD_SPADES_3),
	ENUM2STRING(CARD_SPADES_4),
	ENUM2STRING(CARD_SPADES_5),
	ENUM2STRING(CARD_SPADES_6),
	ENUM2STRING(CARD_SPADES_7),
	ENUM2STRING(CARD_SPADES_8),
	ENUM2STRING(CARD_SPADES_9),
	ENUM2STRING(CARD_SPADES_10),
	ENUM2STRING(CARD_SPADES_JACK),
	ENUM2STRING(CARD_SPADES_QUEEN),
	ENUM2STRING(CARD_SPADES_KING),

	//Red
	ENUM2STRING(CARD_HEARTS_ACE),
	ENUM2STRING(CARD_HEARTS_2),
	ENUM2STRING(CARD_HEARTS_3),
	ENUM2STRING(CARD_HEARTS_4),
	ENUM2STRING(CARD_HEARTS_5),
	ENUM2STRING(CARD_HEARTS_6),
	ENUM2STRING(CARD_HEARTS_7),
	ENUM2STRING(CARD_HEARTS_8),
	ENUM2STRING(CARD_HEARTS_9),
	ENUM2STRING(CARD_HEARTS_10),
	ENUM2STRING(CARD_HEARTS_JACK),
	ENUM2STRING(CARD_HEARTS_QUEEN),
	ENUM2STRING(CARD_HEARTS_KING),

	//Black
	ENUM2STRING(CARD_CLUBS_ACE),
	ENUM2STRING(CARD_CLUBS_2),
	ENUM2STRING(CARD_CLUBS_3),
	ENUM2STRING(CARD_CLUBS_4),
	ENUM2STRING(CARD_CLUBS_5),
	ENUM2STRING(CARD_CLUBS_6),
	ENUM2STRING(CARD_CLUBS_7),
	ENUM2STRING(CARD_CLUBS_8),
	ENUM2STRING(CARD_CLUBS_9),
	ENUM2STRING(CARD_CLUBS_10),
	ENUM2STRING(CARD_CLUBS_JACK),
	ENUM2STRING(CARD_CLUBS_QUEEN),
	ENUM2STRING(CARD_CLUBS_KING),

	//Red
	ENUM2STRING(CARD_DIAMONDS_ACE),
	ENUM2STRING(CARD_DIAMONDS_2),
	ENUM2STRING(CARD_DIAMONDS_3),
	ENUM2STRING(CARD_DIAMONDS_4),
	ENUM2STRING(CARD_DIAMONDS_5),
	ENUM2STRING(CARD_DIAMONDS_6),
	ENUM2STRING(CARD_DIAMONDS_7),
	ENUM2STRING(CARD_DIAMONDS_8),
	ENUM2STRING(CARD_DIAMONDS_9),
	ENUM2STRING(CARD_DIAMONDS_10),
	ENUM2STRING(CARD_DIAMONDS_JACK),
	ENUM2STRING(CARD_DIAMONDS_QUEEN),
	ENUM2STRING(CARD_DIAMONDS_KING),
};

//old is the card you have selected, new is the card you're placing it on
static bool CanPlace( cardStack_t *old, cardStack_t *new, bool foundation )
{
	if ( foundation )
	{//Foundation card, same suit and counting up
		if ( !old->next && new->data->type == old->data->type-1 )
		{//Guaranteed to be the same suit
			return true;
		}
	}
	else
	{//Tableau card, alternate red/black and counting down
		//WARNING: Black magic ahead
		int newIndexBelow = old->data->type-12;
		int newIndexAbove = (old->data->type+14)%CARD_NUM_CARDS;
		if ( newIndexBelow < 0 )
			newIndexBelow += CARD_NUM_CARDS;

		if ( !new->next && (new->data->type == newIndexBelow || new->data->type == newIndexAbove || !old->data->revealed || !new->data->revealed) )
			return true;
	}
	return false;
}

//old is the card you have selected, new is the card you're placing it on
static bool CanPlaceCard( cardStack_t *old, card_t *new, bool foundation )
{
	if ( foundation )
	{//Foundation card, same suit and counting up
		if ( new->type == old->data->type-1 )
		{//Guaranteed to be the same suit
			return true;
		}
	}
	else
	{//Tableau card, alternate red/black and counting down
		//WARNING: Black magic ahead
		int newIndexBelow = old->data->type-12;
		int newIndexAbove = (old->data->type+14)%CARD_NUM_CARDS;
		if ( newIndexBelow < 0 )
			newIndexBelow += CARD_NUM_CARDS;

		if ( new->type == newIndexBelow || new->type == newIndexAbove || !old->data->revealed || !new->revealed )
			return true;
	}
	return false;
}

static bool CanPickUp( cardStack_t *stack )
{
	if ( !stack->data->revealed )
	{
		if ( !stack->next )
			stack->data->revealed = true;
		return false;
	}

	return true;
}

static void EmptyStack( cardStack_t *stack )
{
	if ( !stack )
		return;

	if ( stack->next )
		EmptyStack( stack->next );

	stack->next = NULL;
	stack->data = NULL;
	free( stack );
}

static cardStack_t *CardStackGetTop( cardStack_t *stack )
{
	cardStack_t *current = stack;

	if ( !current )
		return NULL;

	while ( current->next )
		current = current->next;

	return current;
}

//returns NULL if the move was invalid, otherwise returns the top of the stack
static cardStack_t *CardStackPush( cardStack_t **stack, card_t *card, bool foundation )
{
	cardStack_t *current = NULL;

	if ( !*stack )
	{//No cards on this stack yet, allocate the root object
		*stack = (cardStack_t *)malloc( sizeof( cardStack_t ) );
		(*stack)->data = card;
		(*stack)->next = NULL;
		(*stack)->prev = NULL;
		return *stack;
	}

	//Find the top of the stack, and push more on if it's a valid move
	current = CardStackGetTop( *stack );
	if ( CanPlaceCard( current, card, foundation ) )
	{
	//	printf( "Inserting a %s on a %s\n", cardNames[card->type].name, cardNames[current->data->type].name );
		current->next = (cardStack_t *)malloc( sizeof( cardStack_t ) );
		current->next->prev = current;
		current = current->next;
		current->data = card;
		current->next = NULL;
		return current;
	}
	else
	{
		printf( "Invalid move\n" );
	}

	return NULL;
}

void Game_Initialise( void )
{
	int i = 0;
	int cardIndex = 0;

	printf( "\tLoading game\n\t----------------------------\n" );

	printf( "\tInitialising cards..." );
	faceDownTexture = R_LoadTexture( "textures/cards/facedown.png" );
	foundationTexture = R_LoadTexture( "textures/cards/foundation.png" );

	for ( i=0; i<CARD_NUM_CARDS; i++ )
	{//Initialise each card
		cards[i].type = i;
		cards[i].textureID = R_LoadTexture( va( "textures/cards/%s.png", Nx_strlwr( cardNames[i].name+5 ) /* strip CARD_ */ ) );
		if ( !strncmp( cardNames[i].name+5, "spades", 6 ) )		cards[i].suit = SUIT_SPADES;
		if ( !strncmp( cardNames[i].name+5, "hearts", 6 ) )		cards[i].suit = SUIT_HEARTS;
		if ( !strncmp( cardNames[i].name+5, "diamonds", 8 ) )	cards[i].suit = SUIT_DIAMONDS;
		if ( !strncmp( cardNames[i].name+5, "clubs", 5 ) )		cards[i].suit = SUIT_CLUBS;
		//printf( "\tCard %s loaded with textureID %i\n", cardNames[i].name, cards[i].textureID );
	}

	printf( "shuffling..." );
	{
		cardStack_t *cardHT[CARD_SHUFFLE_POOL] = { 0 };
		card_t cardsFake[CARD_NUM_CARDS] = { { 0 } };

		memcpy( &cardsFake[0], &cards[0], sizeof( card_t ) * CARD_NUM_CARDS );
		for ( i=0; i<CARD_NUM_CARDS; i++ )
		{
			unsigned int hash = ( SimpleHash( cardNames[cards[i].type].name, CARD_SHUFFLE_POOL ) ^ rand() ) & (CARD_SHUFFLE_POOL-1);
			cardStack_t *current = cardHT[hash];

			if ( current )
			{//Already taken, insert as child
				while ( current->next )
					current = current->next;
				current->next = (cardStack_t *)malloc( sizeof( cardStack_t ) );
				current = current->next;
				current->data = &cardsFake[i];
				current->next = NULL; //redundant? safe!
			}
			else
			{
				cardHT[hash] = (cardStack_t *)malloc( sizeof( cardStack_t ) );
				current = cardHT[hash];
				current->data = &cardsFake[i];
				current->next = NULL; //redundant? safe!
			}
		}

		for ( i=0; i<CARD_SHUFFLE_POOL; i++ )
		{
			cardStack_t *current = cardHT[i];
			while ( current )
			{
				memcpy( &cards[cardIndex++], current->data, sizeof( card_t ) );
				current = current->next;
			}
		}

		for ( i=0; i<CARD_NUM_CARDS; i++ )
		{
			unsigned int hash = ( SimpleHash( cardNames[cardsFake[i].type].name, CARD_SHUFFLE_POOL ) ^ rand() ) & (CARD_SHUFFLE_POOL-1);
			cardStack_t *current = cardHT[hash];
			cardStack_t *prev = cardHT[hash];

			if ( !current )
				continue; //er?

			while ( stricmp( cardNames[current->data->type].name, cardNames[cardsFake[i].type].name ) )
			{
				if ( current->next )
				{//We didn't get a match on this, but we can try the children
					prev = current;
					current = current->next;
					continue;
				}
				else
					break;
			}

			//If we got here, 'current' is pointing to the right node and 'prev' will be
			//	the previous node (or current if it's the first entry)
			//This means we can safely remove!
			if ( current == prev )
			{//First entry in the root table
				cardHT[hash] = current->next;
				free( current );
			}
			else
			{//At-least the second entry in the list, free and null the prev->next ptr.
				prev->next = current->next;
				free( current );
			}

			//The node was located, freed, linked-list was corrected. Everything worked!
		}
	}
	printf( "ready!\n" );

	//Set the tableau piles
	cardIndex = 0;
	for ( i=0; i<NUM_TABLEAU_PILES; i++ )	CardStackPush( &tableau[i], &cards[cardIndex++], false );
	for ( i=1; i<NUM_TABLEAU_PILES; i++ )	CardStackPush( &tableau[i], &cards[cardIndex++], false );
	for ( i=2; i<NUM_TABLEAU_PILES; i++ )	CardStackPush( &tableau[i], &cards[cardIndex++], false );
	for ( i=3; i<NUM_TABLEAU_PILES; i++ )	CardStackPush( &tableau[i], &cards[cardIndex++], false );
	for ( i=4; i<NUM_TABLEAU_PILES; i++ )	CardStackPush( &tableau[i], &cards[cardIndex++], false );
	for ( i=5; i<NUM_TABLEAU_PILES; i++ )	CardStackPush( &tableau[i], &cards[cardIndex++], false );
	for ( i=6; i<NUM_TABLEAU_PILES; i++ )	CardStackPush( &tableau[i], &cards[cardIndex++], false );
	for ( i=0; i<NUM_TABLEAU_PILES; i++ )	CardStackGetTop( tableau[i] )->data->revealed = true;

	//Populate the waste pile with the remaining cards
	while ( cardIndex < CARD_NUM_CARDS )	CardStackPush( &waste[0], &cards[cardIndex++], false );

	printf( "\t----------------------------\n\n" );
}

void Game_Render( void )
{
	int i=0;
	float x = 0.0f;
	float y = 0.0f;
	cardStack_t *current = NULL;

	//Waste piles
	R_ResetColor();

	if ( game.wasteView )
	{
		current = waste[0];
		if ( current )
		{
			x = WASTE_X( 0 );
			y = WASTE_Y;

			while ( current )
			{
				if ( current == selectedStack )
					R_SetColor( 1.0f, 0.878f, 0.5f, 1.0f );
				R_DrawRect( current->data->textureID, x, y, CARD_WIDTH, CARD_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f );
				y += TABLEAU_Y_OFFSET;
				current = current->next;
			}
		}
		else
			R_DrawRect( foundationTexture, WASTE_X( 0 ), WASTE_Y, CARD_WIDTH, CARD_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f );

		current = waste[1];
		if ( current )
		{
			x = WASTE_X( 1 );
			y = WASTE_Y;

			while ( current )
			{
				if ( current == selectedStack )
					R_SetColor( 1.0f, 0.878f, 0.5f, 1.0f );
				R_DrawRect( current->data->textureID, x, y, CARD_WIDTH, CARD_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f );
				y += TABLEAU_Y_OFFSET;
				current = current->next;
			}
		}
		else
			R_DrawRect( foundationTexture, WASTE_X( 1 ), WASTE_Y, CARD_WIDTH, CARD_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f );

		return;
	}

	R_DrawRect( waste[0] ? faceDownTexture : foundationTexture, WASTE_X( 0 ), WASTE_Y, CARD_WIDTH, CARD_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f );
	current = CardStackGetTop( waste[1] );
	if ( current && current == selectedStack )
		R_SetColor( 1.0f, 0.878f, 0.5f, 1.0f );
	R_DrawRect( current ? current->data->textureID : foundationTexture, WASTE_X( 1 ), WASTE_Y, CARD_WIDTH, CARD_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f );

	//Tableau piles
	for ( i=0; i<NUM_TABLEAU_PILES; i++ )
	{
		current = tableau[i];
		x = TABLEAU_X( i );
		y = TABLEAU_Y;

		R_ResetColor();
		R_DrawRect( foundationTexture, x, y, CARD_WIDTH, CARD_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f );

		while ( current )
		{
			if ( current == selectedStack )
				R_SetColor( 1.0f, 0.878f, 0.5f, 1.0f );
			R_DrawRect( (current->data->revealed || game.showAllCards) ? current->data->textureID : faceDownTexture, x, y, CARD_WIDTH, CARD_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f );
			y += TABLEAU_Y_OFFSET;
			current = current->next;
		}
	}

	//Foundation piles
	for ( i=0; i<NUM_FOUNDATION_PILES; i++ )
	{
		current = CardStackGetTop( foundation[i] );

		R_ResetColor();
		if ( selectedStack && selectedStack == current )
			R_SetColor( 1.0f, 0.878f, 0.5f, 1.0f );

		R_DrawRect( current ? current->data->textureID : foundationTexture, FOUNDATION_X( i ), FOUNDATION_Y, CARD_WIDTH, CARD_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f );
	}
}

static bool PointInElementBounds( int mousePos[2], float x, float y, float w, float h )
{//TODO: Redo for heirarchical positioning.
	float elemPosX	= ((x) / (float)SCREEN_WIDTH) * (float)WINDOW_WIDTH;
	float elemPosY	= ((y) / (float)SCREEN_HEIGHT) * (float)WINDOW_HEIGHT;
	float elemSizeX	= (w / (float)SCREEN_WIDTH) * (float)WINDOW_WIDTH;
	float elemSizeY	= (h / (float)SCREEN_HEIGHT) * (float)WINDOW_HEIGHT;

	return !!(	mousePos[0] > elemPosX && mousePos[0] < elemPosX + elemSizeX &&
				mousePos[1] > elemPosY && mousePos[1] < elemPosY + elemSizeY );
}

void Game_HandleEvents( int mousePos[], byte mouseState )
{
	cardStack_t *card = NULL;
	int i = 0, j = 0;
	static int lastState = 0;
	float x = 0.0f, y = 0.0f;

	if ( mouseState != lastState && (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) )
	{
		// TABLEAUS
		for ( i=0; i<NUM_TABLEAU_PILES; i++ )
		{//Begin tableau collisions
			x = TABLEAU_X( i );
			y = TABLEAU_Y;

			card = tableau[i];

			if ( !card )
			{//Must be a tableau/foundation root
				if ( selectedStack && PointInElementBounds( mousePos, x, y, CARD_WIDTH, CARD_HEIGHT ) &&
					(selectedStack->data->type == CARD_SPADES_KING || selectedStack->data->type == CARD_HEARTS_KING || selectedStack->data->type == CARD_CLUBS_KING || selectedStack->data->type == CARD_DIAMONDS_KING) )
				{//Clicking on this slot, only place cards (kings) down.
					tableau[i] = selectedStack;
					if ( selectedStack->prev )
					{//The card we're moving was part of another stack
						selectedStack->prev->next = NULL;
						selectedStack->prev->data->revealed = true;
						selectedStack->prev = NULL;
					}
					else
					{//The card we're moving was also a tableau/foundation/waste root
						for ( j=0; j<NUM_TABLEAU_PILES; j++ )
						{
							if ( tableau[j] == selectedStack && j != i )
							{
								tableau[j] = NULL;
								selectedStack->prev = NULL;
								break;
							}
						}
						for ( j=0; j<NUM_FOUNDATION_PILES; j++ )
						{
							if ( foundation[j] == selectedStack )
							{
								foundation[j] = NULL;
								selectedStack->prev = NULL;
								break;
							}
						}
						if ( waste[1] == selectedStack )
						{
							waste[1] = NULL;
							selectedStack->prev = NULL;
							break;
						}
					}
//found1:
					selectedStack = NULL;
					lastState = mouseState;
					return;	
				}

				//There are no cards on this tableau pile, and we didn't place anything down.
				//	Skip to the next tableau pile, no need to try iterating through an empty stack
				continue;
			}

			//If we got here, this tableau pile is not empty, iterate backwards from the top-most element and check for a collision
			while ( card && card->next )
			{//Iterate up the stack to get the coordinates
				y += TABLEAU_Y_OFFSET;
				card = card->next;
			}
			while ( card )
			{//And back down, checking coordinates
				if ( PointInElementBounds( mousePos, x, y, CARD_WIDTH, CARD_HEIGHT ) )
				{
					if ( !selectedStack )
					{
						if ( CanPickUp( card ) )
						{
							selectedStack = card;
						}
					}
					else
					{//Already had a selection, place it and clear the selection
						if ( CanPlace( selectedStack, card, false ) )
						{//Guaranteed to be the top of the stack
							card->next = selectedStack;
							if ( selectedStack->prev )
							{
								selectedStack->prev->next = NULL;
								selectedStack->prev->data->revealed = true;
								selectedStack->prev = card;
							}
							else
							{
								for ( i=0; i<NUM_TABLEAU_PILES; i++ )
								{
									if ( tableau[i] == selectedStack )
									{
										tableau[i] = NULL;
										selectedStack->prev = card;
										goto found2;
									}
								}
								for ( i=0; i<NUM_FOUNDATION_PILES; i++ )
								{
									if ( foundation[i] == selectedStack )
									{
										foundation[i] = NULL;
										selectedStack->prev = card;
										goto found2;
									}
								}
								if ( waste[1] == selectedStack )
								{
									waste[1] = NULL;
									selectedStack->prev = card;
									goto found2;
								}
							}
found2:
							selectedStack = NULL;
						}
					}
					lastState = mouseState;
					return;
				}
				y -= TABLEAU_Y_OFFSET;
				card = card->prev;
			}
		}//End tableau collisions

		// FOUNDATIONS
		for ( i=0; i<NUM_FOUNDATION_PILES; i++ )
		{//Look for foundation collisions
			x = FOUNDATION_X( i );
			y = FOUNDATION_Y;

			card = foundation[i];

			if ( !card )
			{//Check for collisions on foundation roots
				if ( selectedStack && PointInElementBounds( mousePos, x, y, CARD_WIDTH, CARD_HEIGHT ) &&
					(selectedStack->data->type == CARD_SPADES_ACE || selectedStack->data->type == CARD_HEARTS_ACE || selectedStack->data->type == CARD_CLUBS_ACE || selectedStack->data->type == CARD_DIAMONDS_ACE) )
				{
					foundation[i] = selectedStack;
					if ( selectedStack->prev )
					{//The card we're moving was part of another stack
						selectedStack->prev->next = NULL;
						selectedStack->prev->data->revealed = true;
						selectedStack->prev = NULL;
					}
					else
					{//The card we're moving was also a tableau/foundation/waste root
						for ( j=0; j<NUM_TABLEAU_PILES; j++ )
						{
							if ( tableau[j] == selectedStack )
							{
								tableau[j] = NULL;
								selectedStack->prev = NULL;
								goto found3;
							}
						}
						for ( j=0; j<NUM_FOUNDATION_PILES; j++ )
						{
							if ( foundation[j] == selectedStack && j != i )
							{
								foundation[j] = NULL;
								selectedStack->prev = NULL;
								goto found3;
							}
						}
						if ( waste[1] == selectedStack )
						{
							waste[1] = NULL;
							selectedStack->prev = NULL;
							goto found3;
						}
					}
found3:
					selectedStack = NULL;
					lastState = mouseState;
					return;
				}
				continue;
			}

			else
			{//Check for collisions on the top foundation card
				card = CardStackGetTop( card );
				if ( PointInElementBounds( mousePos, x, y, CARD_WIDTH, CARD_HEIGHT ) )
				{
					if ( !selectedStack )
					{
						if ( CanPickUp( card ) )
						{
							selectedStack = card;
						}
					}
					else
					{//Already had a selection, place it and clear the selection
						if ( CanPlace( selectedStack, card, true ) )
						{//Guaranteed to be the top of the stack
							card->next = selectedStack;
							if ( selectedStack->prev )
							{
								selectedStack->prev->next = NULL;
								selectedStack->prev->data->revealed = true;
								selectedStack->prev = card;
							}
							else
							{
								for ( i=0; i<NUM_TABLEAU_PILES; i++ )
								{
									if ( tableau[i] == selectedStack )
									{
										tableau[i] = NULL;
										selectedStack->prev = card;
										goto found4;
									}
								}
								for ( i=0; i<NUM_FOUNDATION_PILES; i++ )
								{
									if ( foundation[i] == selectedStack )
									{
										foundation[i] = NULL;
										selectedStack->prev = card;
										goto found4;
									}
								}
								if ( waste[1] == selectedStack )
								{
									waste[1] = NULL;
									selectedStack->prev = card;
									goto found4;
								}
							}
found4:
							selectedStack = NULL;
						}
					}
					lastState = mouseState;
					return;
				}
			}
		}//End foundation collisions

		//WASTES
		if ( PointInElementBounds( mousePos, WASTE_X( 0 ), WASTE_Y, CARD_WIDTH, CARD_HEIGHT ) )
		{
			card = CardStackGetTop( waste[0] );

			if ( card )
			{//Card exists, pop it to the next waste pile
				cardStack_t *dest = CardStackGetTop( waste[1] );
				if ( card->prev )
					card->prev->next = NULL;
				else
					waste[0] = NULL;
				card->prev = dest;
				if ( dest )
					dest->next = card;
				else
					waste[1] = card;
				card->data->revealed = true;
			}
			else
			{//No card there, switch the piles
				//FIXME: for some reason the top of the stack's prev is NULL. what gives ;_;
				cardStack_t *src = NULL;//CardStackGetTop( waste[1] );
				while ( (src = CardStackGetTop( waste[1] )) )
				{
					cardStack_t *dest = CardStackGetTop( waste[0] );
					if ( src->prev )
						src->prev->next = NULL;
					else
						waste[1] = NULL;
					src->prev = dest;
					if ( dest )
						dest->next = src;
					else
						waste[0] = src;
					src->data->revealed = false;
				//	printf( "\tDerped! src->prev == %x\n", src->prev );
				//	src = src->prev;
				}
				//printf( "\tNo card on waste[0]\n" );
			}
		}

		if ( PointInElementBounds( mousePos, WASTE_X( 1 ), WASTE_Y, CARD_WIDTH, CARD_HEIGHT ) )
		{
			selectedStack = CardStackGetTop( waste[1] );
			lastState = mouseState;
			return;
		}

#if 0
		if ( PointInElementBounds( mousePos, WASTE_X( 0 ), WASTE_Y, CARD_WIDTH, CARD_HEIGHT ) )
		{
			card = CardStackGetTop( waste[0] );

			if ( card )
			{//Card exists, pop it to the next waste pile
				cardStack_t *dest = CardStackGetTop( waste[1] );
				if ( card->prev )
					card->prev->next = NULL;
				card->prev = dest;
				if ( dest )
					dest->next = card;
				else
					waste[1] = card;
			}
			else
			{//No card there, switch the piles
			}
		}
#endif

		selectedStack = NULL;
	}
	lastState = mouseState;
}

void Game_Shutdown( void )
{
	int i = 0;
	printf( "\tUnloading game\n\t----------------------------\n" );

	for ( i=0; i<NUM_TABLEAU_PILES; i++ )
	{
		EmptyStack( tableau[i] );
		tableau[i] = NULL;
	}

	for ( i=0; i<NUM_FOUNDATION_PILES; i++ )
	{
		EmptyStack( foundation[i] );
		foundation[i] = NULL;
	}

	for ( i=0; i<NUM_WASTE_PILES; i++ )
	{
		EmptyStack( waste[i] );
		waste[i] = NULL;
	}

	memset( cards, 0, sizeof( cards ) );
	selectedStack = NULL;

	printf( "\t----------------------------\n\n" );
}

