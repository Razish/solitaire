
/*
Copyright (C) <2012> <Kieren 'Razish' McDevitt>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/



#pragma once

#ifdef __linux__
	#define stricmp strcasecmp 
	#define strnicmp strncasecmp 
	#include <unistd.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include <ctype.h>
#include <time.h>

#define APP_NAME "solitaire!"

//We're going to draw HUD elements to a virtual screen of SCREEN_WIDTH/SCREEN_HEIGHT, and reposition accordingly.
//	Also going to take widescreen into account -_-..?!!?
//	Perhaps let them specify the /center/ of each element? (Despite, you know, there not actually being a center for a 64x32 image?)
//	Meh, relative positioning of top-left corner by virtual screen size
#define WINDOW_WIDTH				(1000)	//	Window width
#define WINDOW_HEIGHT				(750)	//	Window height
#define SCREEN_WIDTH				(1000)	//	Virtual screen width (For menu coordinates)
#define SCREEN_HEIGHT				(750)	//	Virtual screen height (For menu coordinates)

// Macros
#define TABLESIZE( table, var ) const int var = sizeof( table ) / sizeof( table[0] )
#define ENUM2STRING( arg ) { #arg, arg }

typedef struct stringID_table_s
{
	char	*name;
	int		id;
} stringID_table_t;

// Typedefs
#ifndef __cplusplus
	typedef enum bool_e { false=0, true } bool_t;
	typedef unsigned char bool;
#endif
typedef unsigned char byte;
typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];

// Common data
typedef struct common_s
{
	bool running;
	bool appActive;
	char *path;
} common_t;
extern common_t common;

