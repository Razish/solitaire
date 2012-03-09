
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

common_t common = { 0 };

int main( int argc, char *argv[] )
{
	printf( APP_NAME"\n" );

	common.running = true;
	common.path = argv[0];
	{//Strip the file name + extension to get the current working directory
		int i = strlen( common.path )-1;
		while ( common.path[i] && i > 0 )
		{
			if ( common.path[i] == '/' || common.path[i] == '\\' )
			{
				common.path[i] = '\0';
				break;
			}
			i--;
		}
	}
	printf( "Path is %s\n\n", common.path );

	srand( (unsigned int)time( NULL ) );

	R_Initialise();
	Game_Initialise();
	Input_Initialise();

	while ( common.running )
	{
		Input_Poll();

		if ( common.running )
		{//If the input handler picked up an exit, don't bother with the game
			R_ClearScreen();
			Game_Render();
			SDL_GL_SwapBuffers();
		}
	}

	R_Shutdown();
	Game_Shutdown();

	printf( "Thanks for playing!\n" );

	return 0;
}

