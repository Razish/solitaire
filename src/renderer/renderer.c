
/*
Copyright (C) <2012> <Kieren 'Razish' McDevitt>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/



#include "common.h"
#include "renderer/renderer.h"
//#include "renderer/font.h"
#include "utils/math.h"
#include "utils/string.h"
#include "API/SDL/include/SDL.h"
#include "API/SDL/include/SDL_opengl.h"

renderer_t renderer = { 0 };

void R_SetupViewport( int x, int y, int width, int height )
{
	glViewport( x, y, width, height );
}

void R_ClearScreen( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void R_SetColor( float r, float g, float b, float a )
{
	renderer.currentColor[0] = r; renderer.currentColor[1] = g; renderer.currentColor[2] = b; renderer.currentColor[3] = a;
}

void R_ResetColor( void )
{
	renderer.currentColor[0] = renderer.currentColor[1] = renderer.currentColor[2] = renderer.currentColor[3] = 1.0f;
}

static void R_Use2D( void )
{
	if ( renderer.in2D )
		return;

	renderer.in2D = true;

	glDisable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glEnable( GL_TEXTURE_2D );
}

void R_DrawRect( const int textureHandle, float x, float y, float width, float height, float s0, float t0, float s1, float t1 )
{// Actually draws 2 triangles, but it's still the same...!
	vec2_t vertices[4];
	vec2_t texcoords[4];
	vec4_t colors[4];
	static const unsigned short indices[6] = { 0, 1, 2, 1, 2, 3 };
	nexTexture_t *texture = NULL;

	if ( !(texture = R_GetTextureForHandle( textureHandle )) )
		return;

	R_Use2D();

	glBindTexture( GL_TEXTURE_2D, texture->internalId );

	// Top-left
	vertices[0][0]	= x;
	vertices[0][1]	= y;
	texcoords[0][0]	= s0;
	texcoords[0][1]	= t0;
	Vector4Copy( colors[0], renderer.currentColor );

	// Top-right
	vertices[1][0]	= x + width;
	vertices[1][1]	= y;
	texcoords[1][0]	= s1;
	texcoords[1][1]	= t0;
	Vector4Copy( colors[1], renderer.currentColor );

	// Bottom-left
	vertices[2][0]	= x;
	vertices[2][1]	= y + height;
	texcoords[2][0]	= s0;
	texcoords[2][1]	= t1;
	Vector4Copy( colors[2], renderer.currentColor );

	// Bottom-right
	vertices[3][0]	= x + width;
	vertices[3][1]	= y + height;
	texcoords[3][0]	= s1;
	texcoords[3][1]	= t1;
	Vector4Copy( colors[3], renderer.currentColor );

	#ifndef R_USE_IMMEDIATE_MODE
		glEnableClientState( GL_COLOR_ARRAY );
		glVertexPointer( 2, GL_FLOAT, 0, (const GLvoid *)&vertices[0] );
		glTexCoordPointer( 2, GL_FLOAT, 0, (const GLvoid *)&texcoords[0] );
		glColorPointer( 4, GL_FLOAT, 0, (const GLvoid *)&colors[0] );

		glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const GLvoid *)&indices[0] );
		glDisableClientState( GL_COLOR_ARRAY );
	#else
		{
			int i=0;
			TABLESIZE( indices, numIndices );

			glBegin( GL_TRIANGLES );
			for ( i=0; i<numIndices; i++ )
			{
				glTexCoord2fv( &texcoords[indices[i]][0] );
				glColor4fv( &colors[indices[i]][0] );
				glVertex2fv( &vertices[indices[i]][0] );
			}
			glEnd();
		}
	#endif
}

void R_DrawQuad( float x, float y, float width, float height )
{
	glBegin( GL_QUADS );
		glTexCoord2f( 0.0f, 0.0f ); glVertex2f( x,			y+height );
		glTexCoord2f( 0.0f, 1.0f ); glVertex2f( x,			y );
		glTexCoord2f( 1.0f, 1.0f ); glVertex2f( x+width,	y );
		glTexCoord2f( 1.0f, 0.0f ); glVertex2f( x+width,	y+height );
	glEnd();
}

void R_DrawQuadExplicit( float x, float y, float width, float height, float s, float t, float u, float v )
{//TODO: s,t,u,v
	glBegin( GL_QUADS );
		glTexCoord2f( 0.0f, 0.0f ); glVertex2f( x,			y+height );
		glTexCoord2f( 0.0f, 1.0f ); glVertex2f( x,			y );
		glTexCoord2f( 1.0f, 1.0f ); glVertex2f( x+width,	y );
		glTexCoord2f( 1.0f, 0.0f ); glVertex2f( x+width,	y+height );
	glEnd();
}

#define R_DrawFullscreenQuad() R_DrawQuad( 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT )

#if 0
#ifndef GL_SELECTMACRO
	void GL_SelectTexture( int texUnit )
	{
		switch ( texUnit )
		{
		case 0:
			glActiveTextureARB( GL_TEXTURE0_ARB );	glClientActiveTextureARB( GL_TEXTURE0_ARB );
			break;
		case 1:
			glActiveTextureARB( GL_TEXTURE1_ARB );	glClientActiveTextureARB( GL_TEXTURE1_ARB );
			break;
		case 2:
			glActiveTextureARB( GL_TEXTURE2_ARB );	glClientActiveTextureARB( GL_TEXTURE2_ARB );
			break;
		case 3:
			glActiveTextureARB( GL_TEXTURE3_ARB );	glClientActiveTextureARB( GL_TEXTURE3_ARB );
			break;
		default:
			printf( "GL_SelectTexture: Invalid texture unit %d\n", texUnit );
			common.running = false;
			break;
		}
		return;
	}
#endif //GL_SELECTMACRO
#endif //0

static bool SetupSDL( void )
{
	if ( SDL_Init( SDL_INIT_VIDEO ) == -1 )
	{
		printf( "\tFailed to initialise SDL system.\n" );
		return false;
	}

	//Set the window title
	SDL_WM_SetIcon( SDL_LoadBMP( va( "%s/textures/icons/icon.bmp", common.path ) ), NULL );
	SDL_WM_SetCaption( APP_NAME, NULL );

	//Full 32 bit colour
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );	SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

	//16-bit depth buffer
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );

	//V-sync + Double buffering (Or not!)
//	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 0 );

	//FSAA
//	SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 4 );
//	SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 16 );

	if ( !SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT, 0, SDL_OPENGL ) )
	{
		printf( "\tFailed to create video context.\n" );
		return false;
	}

	return true;
}

static void SetupGL( void )
{
	glClearColor( 0.1f, 0.14f, 0.1f, 0.0f );
	//glClearColor( 0.15f, 0.35f, 0.15f, 0.0f );

	//Alpha blending
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	// Enable back-face culling
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glFrontFace( GL_CCW );

	//Rendering primitives
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	renderer.extensionsString = /*GLeeGetExtStrGL();*/(char *)glGetString( GL_EXTENSIONS );

//	Setup3D();
	R_Use2D();

	return;
}

void R_Initialise( void )
{
	printf( "\tLoading renderer\n\t----------------------------\n" );

	R_ResetColor();

	//Set up SDL/OpenGL
	if ( !SetupSDL() )
	{
		common.running = false;
		printf( "\t----------------------------\n\n" );
		return;
	}

	//Configure OpenGL
	SetupGL();

	//TODO: Load the font library
//	R_SetupFontLibrary();

	printf( "\t----------------------------\n\n" );
	return;
}

void R_Shutdown( void )
{//Shutdown
    GLenum err;
	printf( "\tUnloading renderer\n\t----------------------------\n" );

	//TODO: Clean up fonts
//	free( renderer.fonts );

	R_DestroyTextures();

	while ( (err = glGetError()) != GL_NO_ERROR )
	    printf( "GL Error: 0x%x\n", err );

	SDL_Quit();

	printf( "\t----------------------------\n\n" );
	return;
}

