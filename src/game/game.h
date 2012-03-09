
/*
Copyright (C) <2012> <Kieren 'Razish' McDevitt>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/



#pragma once

typedef enum cardType_e {//They must be in this order! Black magic is used to guess a correct move
	//Black
	CARD_SPADES_ACE=0,
	CARD_SPADES_2,
	CARD_SPADES_3,
	CARD_SPADES_4,
	CARD_SPADES_5,
	CARD_SPADES_6,
	CARD_SPADES_7,
	CARD_SPADES_8,
	CARD_SPADES_9,
	CARD_SPADES_10,
	CARD_SPADES_JACK,
	CARD_SPADES_QUEEN,
	CARD_SPADES_KING,
	
	//Red
	CARD_HEARTS_ACE,
	CARD_HEARTS_2,
	CARD_HEARTS_3,
	CARD_HEARTS_4,
	CARD_HEARTS_5,
	CARD_HEARTS_6,
	CARD_HEARTS_7,
	CARD_HEARTS_8,
	CARD_HEARTS_9,
	CARD_HEARTS_10,
	CARD_HEARTS_JACK,
	CARD_HEARTS_QUEEN,
	CARD_HEARTS_KING,

	//Black
	CARD_CLUBS_ACE,
	CARD_CLUBS_2,
	CARD_CLUBS_3,
	CARD_CLUBS_4,
	CARD_CLUBS_5,
	CARD_CLUBS_6,
	CARD_CLUBS_7,
	CARD_CLUBS_8,
	CARD_CLUBS_9,
	CARD_CLUBS_10,
	CARD_CLUBS_JACK,
	CARD_CLUBS_QUEEN,
	CARD_CLUBS_KING,

	//Red
	CARD_DIAMONDS_ACE,
	CARD_DIAMONDS_2,
	CARD_DIAMONDS_3,
	CARD_DIAMONDS_4,
	CARD_DIAMONDS_5,
	CARD_DIAMONDS_6,
	CARD_DIAMONDS_7,
	CARD_DIAMONDS_8,
	CARD_DIAMONDS_9,
	CARD_DIAMONDS_10,
	CARD_DIAMONDS_JACK,
	CARD_DIAMONDS_QUEEN,
	CARD_DIAMONDS_KING,

	CARD_NUM_CARDS
} cardType_t;

typedef enum cardSuit_e {
	SUIT_SPADES=0,
	SUIT_HEARTS,
	SUIT_DIAMONDS,
	SUIT_CLUBS,
	
	SUIT_NUM_SUITS
} cardSuit_t;

typedef struct card_s {
	cardType_t type;
	cardSuit_t suit;
	bool revealed;
	int textureID;
} card_t;

typedef struct cardStack_s {
	card_t *data;
	struct cardStack_s *next;
	struct cardStack_s *prev;
} cardStack_t;

typedef struct game_s {
	bool showAllCards;
	bool wasteView;
} game_t;
extern game_t game;

void Game_Initialise( void );
void Game_Render( void );
void Game_HandleEvents( int mousePos[], byte mouseState );
void Game_Shutdown( void );

