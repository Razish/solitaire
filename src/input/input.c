
/*
Copyright (C) <2012> <Kieren 'Razish' McDevitt>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/



#include "common.h"
#include "input/input.h"
#include "renderer/renderer.h"
#include "game/game.h"

static SDL_Event inputEvent;		//	SDL event handler
static bool keyState[SDLK_LAST];	//	Mark which keys are being pressed

#define CURRENTKEY ( inputEvent.key.keysym.sym )

void Input_Poll( void )
{
	static int mousePos[2] = { 0, 0 };
	byte mouseState = SDL_GetMouseState( &mousePos[0], &mousePos[1] );

	//Is the app active?
	common.appActive = (SDL_GetAppState() == (SDL_APPMOUSEFOCUS|SDL_APPINPUTFOCUS|SDL_APPACTIVE) );

	while ( SDL_PollEvent( &inputEvent ) )
	{
		switch ( inputEvent.type )
		{
		case SDL_QUIT:
			common.running = false;
			break;

		case SDL_KEYDOWN:
			keyState[CURRENTKEY] = true;

			//Handle once-off button presses here
			if ( inputEvent.key.keysym.sym == SDLK_ESCAPE || inputEvent.key.keysym.sym == SDLK_q )
				common.running = false;

			else if ( inputEvent.key.keysym.sym == SDLK_f )
				game.showAllCards = !game.showAllCards;

			else if ( inputEvent.key.keysym.sym == SDLK_g )
				game.wasteView = !game.wasteView;

			else if ( inputEvent.key.keysym.sym == SDLK_r )
			{
				Game_Shutdown();
				Game_Initialise();
			}

			break;

		case SDL_KEYUP:
			keyState[CURRENTKEY] = false;
			break;

		case SDL_VIDEORESIZE:
			R_SetupViewport( 0, 0, inputEvent.resize.w, inputEvent.resize.h );
			break;
		}
	}

	Game_HandleEvents( mousePos, mouseState );
	
	//Handle constant events such as movement here

	return;
}

bool Input_IsKeyDown( SDLKey key )
{
	return keyState[key];
}

void Input_Initialise( void )
{
	printf( "\tLoading input handler...\n\t----------------------------\n" );

	//Resize window
	printf( "\tResizing window (%ix%i)\n", WINDOW_WIDTH, WINDOW_HEIGHT );
	inputEvent.type		= SDL_VIDEORESIZE;
	inputEvent.resize.w	= WINDOW_WIDTH;
	inputEvent.resize.h	= WINDOW_HEIGHT;
	SDL_PushEvent( &inputEvent );

	printf( "\t----------------------------\n\n" );
	return;
}
