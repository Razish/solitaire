
/*
Copyright (C) <2012> <Kieren 'Razish' McDevitt>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/



#pragma once

//Mmm, maybe should standardise this
#ifndef M_PI
	#define M_PI							3.14159265358979323846f	// matches value in gcc v2 math.h
#endif

#define DEG2RAD( a )						( ((a)*M_PI) / 180.0f )

//Min/Max
#ifndef max
	#define max( a, b )						( ((a)>(b)) ? (a):(b) )
#endif
#ifndef min
	#define min( a, b )						( ((a)<(b)) ? (a):(b) )
#endif

//Vector Set
void	_Vector2Set( vec2_t v, float x, float y );
#define	 Vector2Set( v, x, y ) ( (v)[0]=(x), (v)[1]=(y) )
void	_Vector3Set( vec2_t v, float x, float y, float z );
#define	 Vector3Set( v, x, y, z ) ( (v)[0]=(x), (v)[1]=(y), (v)[2]=(z) )
void	_Vector4Set( vec2_t v, float x, float y, float z, float w );
#define  Vector4Set( v, x, y, z, w ) ( (v)[0]=(x), (v)[1]=(y), (v)[2]=(z), (v)[3]=(w) )

//Vector Copy
void	_Vector2Copy( vec2_t dst, const vec2_t src );
#define  Vector2Copy( dst, src ) ( (dst)[0]=(src)[0], (dst)[1]=(src)[1] )
void	_Vector3Copy( vec3_t dst, const vec3_t src );
#define  Vector3Copy( dst, src ) ( (dst)[0]=(src)[0], (dst)[1]=(src)[1], (dst)[2]=(src)[2] )
void	_Vector4Copy( vec4_t dst, const vec4_t src );
#define  Vector4Copy( dst, src ) ( (dst)[0]=(src)[0], (dst)[1]=(src)[1], (dst)[2]=(src)[2], (dst)[3]=(src)[3] )

//Vector Add
void	_Vector2Add( const vec2_t v1, const vec2_t v2, vec2_t out );
#define	 Vector2Add( v1, v2, out ) ( (out)[0]=(v1)[0]+(v2)[0], (out)[1]=(v1)[1]+(v2)[1] )
void	_Vector2Add( const vec3_t v1, const vec3_t v2, vec3_t out );
#define	 Vector3Add( v1, v2, out ) ( (out)[0]=(v1)[0]+(v2)[0], (out)[1]=(v1)[1]+(v2)[1], (out)[2]=(v1)[2]+(v2)[2] )
void	_Vector2Add( const vec4_t v1, const vec4_t v2, vec4_t out );
#define	 Vector4Add( v1, v2, out ) ( (out)[0]=(v1)[0]+(v2)[0], (out)[1]=(v1)[1]+(v2)[1], (out)[2]=(v1)[2]+(v2)[2], (out)[3]=(v1)[3]+(v2)[3] )

//Vector Subtract
void	_Vector2Subtract( const vec2_t v1, const vec2_t v2, vec2_t out );
#define	 Vector2Subtract( v1, v2, out ) ( (out)[0]=(v1)[0]-(v2)[0], (out)[1]=(v1)[1]-(v2)[1] )
void	_Vector3Subtract( const vec3_t v1, const vec3_t v2, vec3_t out );
#define	 Vector3Subtract( v1, v2, out ) ( (out)[0]=(v1)[0]-(v2)[0], (out)[1]=(v1)[1]-(v2)[1], (out)[2]=(v1)[2]-(v2)[2] )
void	_Vector4Subtract( const vec4_t v1, const vec4_t v2, vec4_t out );
#define	 Vector4Subtract( v1, v2, out ) ( (out)[0]=(v1)[0]-(v2)[0], (out)[1]=(v1)[1]-(v2)[1], (out)[2]=(v1)[2]-(v2)[2], (out)[3]=(v1)[3]-(v2)[3] )

//Vector Scale
void	_Vector2Scale( const vec2_t src, float scale, vec2_t dst );
#define  Vector2Scale( src, scale, dst ) ( (dst)[0]=(src)[0]*(scale), (dst)[1]=(src)[1]*(scale) )
void	_Vector3Scale( const vec3_t src, float scale, vec3_t dst );
#define  Vector3Scale( src, scale, dst ) ( (dst)[0]=(src)[0]*(scale), (dst)[1]=(src)[1]*(scale), (dst)[2]=(src)[2]*(scale) )
void	_Vector4Scale( const vec4_t src, float scale, vec4_t dst );
#define  Vector4Scale( src, scale, dst ) ( (dst)[0]=(src)[0]*(scale), (dst)[1]=(src)[1]*(scale), (dst)[2]=(src)[2]*(scale), (dst)[3]=(src)[3]*(scale) )

//Vector MA - Linear interpolation - (x + y) * s
#define  Vector2MA( v1, scale, v2, dst ) ( (dst)[0]=(v1)[0]+(v2)[0]*(scale), (dst)[1]=(v1)[1]+(v2)[1]*(scale) )
#define  Vector3MA( v1, scale, v2, dst ) ( (dst)[0]=(v1)[0]+(v2)[0]*(scale), (dst)[1]=(v1)[1]+(v2)[1]*(scale), (dst)[1]=(v1)[1]+(v2)[1]*(scale) )
#define  Vector4MA( v1, scale, v2, dst ) ( (dst)[0]=(v1)[0]+(v2)[0]*(scale), (dst)[1]=(v1)[1]+(v2)[1]*(scale), (dst)[1]=(v1)[1]+(v2)[1]*(scale), (dst)[1]=(v1)[1]+(v2)[1]*(scale) )

//Vector Cross-Product
float	_Vector2CrossProduct( const vec2_t v1, const vec2_t v2 );
#define	 Vector2CrossProduct( v1, v2 ) ( v1[0]*v1[2] - v1[1]*v2[0] )
void	_Vector3CrossProduct( const vec3_t v1, const vec3_t v2, vec3_t out );
#define	 Vector3CrossProduct( x, y, out ) ( (out)[0]=(x)[1]*(y)[2]-(x)[2]*(y)[1], (out)[1]=(x)[2]*(y)[0]-(x)[0]*(y)[2], (out)[2]=(x)[0]*(y)[1]-(x)[1]*(y)[0] )

//Vector Normalise
void	 Vector2Normalise( vec2_t vec );
void	 Vector3Normalise( vec3_t vec );

float	 Vector2DistanceSquared( const vec2_t v1, const vec2_t v2 );
float	 Vector3DistanceSquared( const vec3_t v1, const vec3_t v2 );

#define DotProduct( x, y )	( (x)[0]*(y)[0] + (x)[1]*(y)[1] + (x)[2]*(y)[2] )

//Matrix constructors
#define EMPTY_MATRIX4		{	0.0f, 0.0f, 0.0f, 0.0f, \
								0.0f, 0.0f, 0.0f, 0.0f, \
								0.0f, 0.0f, 0.0f, 0.0f, \
								0.0f, 0.0f, 0.0f, 0.0f }

#define IDENTITY_MATRIX4	{	1.0f, 0.0f, 0.0f, 0.0f, \
								0.0f, 1.0f, 0.0f, 0.0f, \
								0.0f, 0.0f, 1.0f, 0.0f, \
								0.0f, 0.0f, 0.0f, 1.0f }
