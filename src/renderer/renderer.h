
/*
Copyright (C) <2012> <Kieren 'Razish' McDevitt>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/



#pragma once

typedef struct renderer_s
{
	bool			in2D;
	vec4_t			currentColor;
	const char		*extensionsString;
	int				maxTextureUnits;

//	fontHandle_t	*fonts; //malloc'ed (NOTE: These are *HANDLES* not fontData_t's!)
} renderer_t;
extern renderer_t renderer;

typedef struct nexTexture_s
{
    char texturePath[64];
    unsigned int internalId;
} nexTexture_t;
nexTexture_t *R_GetTextureForHandle( int handle );
void R_DestroyTextures( void );


//Public functions
void R_Initialise( void );
void R_ClearScreen( void );
void R_Shutdown( void );

void R_SetupViewport( int x, int y, int width, int height );
int R_LoadTexture( const char *texturePath );

void R_SetColor( float r, float g, float b, float a );
void R_ResetColor( void );

void R_DrawQuad( float x, float y, float width, float height );
void R_DrawRect( const int textureHandle, float x, float y, float width, float height, float s0, float t0, float s1, float t1 );

