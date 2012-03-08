
/*
Copyright (C) <2012> <Kieren 'Razish' McDevitt>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/



#include "common.h"
#include "utils/math.h"

//
//	Vector Set
//
void	_Vector2Set( vec2_t v, float x, float y )							{	v[0] = x;	v[1] = y;	}
void	_Vector3Set( vec3_t v, float x, float y, float z )					{	v[0] = x;	v[1] = y;	v[2] = z;	}
void	_Vector4Set( vec4_t v, float x, float y, float z, float w )			{	v[0] = x;	v[1] = y;	v[2] = z;	v[3] = w;	}

//
//	Vector Copy
//
void	_Vector2Copy( vec2_t dst, const vec2_t src )						{	dst[0]=src[0];	dst[1]=src[1];	}
void	_Vector3Copy( vec3_t dst, const vec3_t src )						{	dst[0]=src[0];	dst[1]=src[1];	dst[2]=src[2];	}
void	_Vector4Copy( vec4_t dst, const vec4_t src )						{	dst[0]=src[0];	dst[1]=src[1];	dst[2]=src[2];	dst[3]=src[3];	}

//
//	Vector Add
//
void	_Vector2Add( const vec2_t v1, const vec2_t v2, vec2_t out )			{	out[0] = v1[0]+v2[0];	out[1] = v1[1]+v2[1];	}
void	_Vector3Add( const vec3_t v1, const vec3_t v2, vec3_t out )			{	out[0] = v1[0]+v2[0];	out[1] = v1[1]+v2[1];	out[2] = v1[2]+v2[2];	}
void	_Vector4Add( const vec4_t v1, const vec4_t v2, vec4_t out )			{	out[0] = v1[0]+v2[0];	out[1] = v1[1]+v2[1];	out[2] = v1[2]+v2[2];	out[3] = v1[3]+v2[3];	}

//
//	Vector Subtract
//
void	_Vector2Subtract( const vec2_t v1, const vec2_t v2, vec2_t out )	{	out[0] = v1[0]-v2[0];	out[1] = v1[1]-v2[1];	}
void	_Vector3Subtract( const vec3_t v1, const vec3_t v2, vec3_t out )	{	out[0] = v1[0]-v2[0];	out[1] = v1[1]-v2[1];	out[2] = v1[2]-v2[2];	}
void	_Vector4Subtract( const vec4_t v1, const vec4_t v2, vec4_t out )	{	out[0] = v1[0]-v2[0];	out[1] = v1[1]-v2[1];	out[2] = v1[2]-v2[2];	out[3] = v1[3]-v2[3];	}

//
//	Vector Scale
//

//
//	Vector MA
//

//
//	Vector Cross Product
//
float	_Vector2CrossProduct( const vec2_t v1, const vec2_t v2 )			{	return v1[0]*v1[2] - v1[1]*v2[0];	}
void	_Vector3CrossProduct( const vec3_t v1, const vec3_t v2, vec3_t out ){	out[0] = v1[1]*v2[2] - v1[2]*v2[1];	out[1] = v1[2]*v2[0] - v1[0]*v2[2];	out[2] = v1[0]*v2[1] - v1[1]*v2[0];	}

//
//	Vector Normalise
//

void Vector2Normalise( vec2_t vec )
{
	float	length	= 0.0f;
	
	if ( (length = sqrtf( (vec[0]*vec[0]) + (vec[1]*vec[1]) )) )
	{//	Assuming the stack allocation, single division, and multiplications will produce less overhead than
	//	the two divisions..
		#if 1
			float length2 = 1/length;
			vec[0] *= length2;
			vec[1] *= length2;
		#else
			vec[0] /= length;
			vec[1] /= length;
		#endif
	}
}

void Vector3Normalise( vec3_t vec )
{
	float	length	= 0.0f;
	
	if ( (length = sqrtf( (vec[0]*vec[0]) + (vec[1]*vec[1]) + (vec[2]*vec[2]) )) )
	{//	Assuming the stack allocation, single division, and multiplications will produce less overhead than
	//	the three divisions..
		#if 1
			float length2 = 1/length;
			vec[0] *= length2;
			vec[1] *= length2;
			vec[2] *= length2;
		#else
			vec[0] /= length;
			vec[1] /= length;
			vec[2] /= length;
		#endif
	}
}

float Vector2DistanceSquared( const vec2_t v1, const vec2_t v2 )
{
	vec2_t v = { 0.0f };
	Vector2Subtract( v2, v1, v );
	return (v[0]*v[0] + v[1]*v[1]);
}

float Vector3DistanceSquared( const vec3_t v1, const vec3_t v2 )
{
	vec3_t v = { 0.0f };
	Vector3Subtract( v2, v1, v );
	return (v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}
