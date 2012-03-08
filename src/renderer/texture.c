
/*
Copyright (C) <2012> <Kieren 'Razish' McDevitt>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/



#include "common.h"
#include "renderer/renderer.h"
#include "utils/string.h"
#include "API/SDL/include/SDL_opengl.h"
#include "API/SDL/include/SDL_image.h"

#define MAX_TEXTURES (64)

static nexTexture_t loadedTextures[MAX_TEXTURES];
static unsigned int numLoadedTextures;

void R_DestroyTextures( void )
{
	unsigned int i;

	for ( i=0; i<numLoadedTextures; i++ )
		glDeleteTextures( 1, &loadedTextures[i].internalId );
}

nexTexture_t *R_GetTextureForHandle( int handle )
{
	unsigned int idx = (handle < 0 || handle >= numLoadedTextures) ? 0 : handle; //default to the fallback texture for out-of-range indices
	return &loadedTextures[idx];
}

static int R_FindTexture( const char *texturePath )
{
	unsigned int i;
	for ( i=0; i<numLoadedTextures; i++ )
		if ( !strcmp( texturePath, loadedTextures[i].texturePath ) )
			return (int)i;

	return -1;
}

int R_LoadTexture( const char *texturePath )
{
	SDL_Surface		*textureSurface;
	nexTexture_t	*currentTexture;
	int				currentTextureId;
	GLint			format;
	GLint			internalFormat;
	char			*path = va( "%s/%s", common.path, texturePath );

	if ( numLoadedTextures >= MAX_TEXTURES )
	{
		printf( "\tFailed to load texture %s (MAX_TEXTURES reached - %i)", path, MAX_TEXTURES );
		common.running = false;
		return -1;
	}

	if ( (currentTextureId = R_FindTexture( texturePath )) != -1 )
		return currentTextureId;

	if ( !(textureSurface = IMG_Load( path )) )
	{
		printf( "\tFailed to load texture %s (IMG_Load failed)\n", path );
		common.running = false;
		return -1;
	}

	currentTextureId	= numLoadedTextures;
	currentTexture		= &loadedTextures[currentTextureId];
	strcpy( currentTexture->texturePath, texturePath );

	glGenTextures( 1, &currentTexture->internalId );
	glBindTexture( GL_TEXTURE_2D, currentTexture->internalId );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ); //GL_REPEAT
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ); //GL_REPEAT
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); //GL_NEAREST
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); //GL_NEAREST

	format			= GL_RGB;
	internalFormat	= GL_RGB8;
	if ( textureSurface->format->BytesPerPixel == 4 )
	{
		format			= GL_RGBA;
		internalFormat	= GL_RGBA8;
	}

	// Do we need mipmaps?
	glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, textureSurface->w, textureSurface->h, 0, format, GL_UNSIGNED_BYTE, (const GLvoid *)textureSurface->pixels );

	SDL_FreeSurface( textureSurface );

	#ifdef DEBUG_RENDERER
		printf( "\tLoaded texture '%s'\n", texturePath );
	#endif
	numLoadedTextures++;
	return currentTextureId;
}

