
/*
Copyright (C) <2012> <Kieren 'Razish' McDevitt>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/



#include "common.h"

char *va( char *format, ... )
{
	static char string[2][32000]; // in case va is called by nested functions
	static int idx = 0;
	char *buf = string[idx++ & 1];
	va_list	argptr;

	va_start( argptr, format );
	vsprintf( buf, format, argptr );
	va_end( argptr );

	return buf;
}

char *strlwr( char *str )
{
	static char string[2][32000] = { { 0 } };
	static int idx = 0;
	char *buf = string[idx++ & 1];
	int i = 0;

	for ( i=0; *str; str++ )
		buf[i++] = (char)tolower( *str );
	buf[i] = '\0';

	return buf;
}

