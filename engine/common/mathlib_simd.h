/*
mathlib_simd.h - SIMD mathlib
Copyright (C) 2016 a1batross

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/
#pragma once
#ifndef MATHLIB_SIMD_H
#define MATHLIB_SIMD_H

#include "port.h"
#include "common.h"
#include "stdint.h"

// will be implemented by SIMD backend
// also additional typecheck
vec_t SIMD_DotProduct( const vec3_t x, const vec3_t y );
vec_t SIMD_DotProductAbs( const vec3_t x, const vec3_t y );
vec_t SIMD_DotProductFabs( const vec3_t x, const vec3_t y );
void SIMD_CrossProduct( const vec3_t x, const vec3_t y, vec3_t dst );
FORCEINLINE void SIMD_VectorScale( const vec3_t in, vec_t scale, vec3_t out );
FORCEINLINE void SIMD_VectorLerp( const vec3_t v1, vec_t lerp, const vec3_t v2, vec3_t dst );
FORCEINLINE qboolean SIMD_VectorCompare( const vec3_t v1, const vec3_t v2 );

vec_t SIMD_VectorMax( const vec3_t a );
vec_t SIMD_VectorAvg( const vec3_t a );
vec_t SIMD_VectorLength( const vec3_t a );
vec_t SIMD_VectorLength2( const vec3_t a ); // just dot product, sqrt-less
vec_t SIMD_VectorDistance( const vec3_t a, const vec3_t b );
vec_t SIMD_VectorDistance2( const vec3_t a, const vec3_t b ); // sqrt-less vector distance
FORCEINLINE void SIMD_VectorNormalize( vec3_t a );
FORCEINLINE void SIMD_VectorNormalize2( const vec3_t a, vec3_t dst );
FORCEINLINE void SIMD_VectorNormalizeFast( vec3_t a );
FORCEINLINE float SIMD_VectorNormalizeLength( vec3_t a );
FORCEINLINE float SIMD_VectorNormalizeLength2( const vec3_t v, vec3_t out );
FORCEINLINE void SIMD_VectorNegate( const vec3_t in, vec3_t out );
FORCEINLINE void SIMD_VectorM( vec_t scale, const vec3_t a, vec3_t dst );
FORCEINLINE void SIMD_VectorMA( const vec3_t a, vec_t scale, const vec3_t b, vec3_t dst );
FORCEINLINE void SIMD_VectorMAMAM( const vec_t scale1, const vec3_t a,
                  const vec_t scale2, const vec3_t b,
                  const vec_t scale3, const vec3_t c, vec3_t dst );
FORCEINLINE int SIMD_VectorIsNull( const vec3_t v );
FORCEINLINE qboolean SIMD_BoundsIntersect( const vec3_t mins1, const vec3_t maxs1, const vec3_t mins2, const vec3_t maxs2 );
FORCEINLINE qboolean SIMD_BoundsAndSphereIntersect( const vec3_t mins, const vec3_t maxs, const vec3_t origin, float radius );

#if defined(__ARM_NEON__) || defined(__NEON__) || defined(XASH_NEON_TEST)
    #include "mathlib_simd_neon.h"
/*#elif defined(__SSE__) || defined(_M_IX86_FP)
	#if defined(__SSE2__) || defined(_M_IX86_FP)
		#define USE_SSE2
	#endif
	#include "mathlib_simd_sse.h"*/
#endif

#endif // MATHLIB_SIMD_H
