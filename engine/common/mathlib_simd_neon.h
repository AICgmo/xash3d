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
#error "Include mathlib_simd.h instead of me!"
#endif

// supports NEON only at this moment
#include "port.h"
#include "mathlib.h"
#if 0
#include <arm_neon.h>
#else
#include "arm_neon_neon.h"
#endif

#define ALIGN __attribute__((aligned(16)))

#define VEC3TO4( x ) x[0], x[1], x[2], 0
#define SIMD_Load3f( x ) vld1q_f32( x )
#define SIMD_Load2f( x ) vld1_f32( x )

#define SIMD_Store2f( dst, src ) vst1_f32( dst, src )
/*#define SIMD_Store3f( dst, src ) vst1q_f32( dst, src )
*/

typedef union
{
	float32x4_t r;
	float s[4];
} float32x4_u_t;

FORCEINLINE void SIMD_Store3f( vec3_t dst, float32x4_t src )
{
#if 1
	vst1q_f32( dst, src ); // if properly aligned
#else
	vst1q_lane_f32( dst, src, 0 );
	vst1q_lane_f32( ++dst, src, 1 );
	vst1q_lane_f32( ++dst, src, 2 );
#endif
}

FORCEINLINE void SIMD_VectorScale( const vec3_t in, vec_t scale, vec3_t out )
{
	float32x4_t vin = SIMD_Load3f(in);
	SIMD_Store3f( out, vmulq_n_f32( vin, scale ) );
}

FORCEINLINE void SIMD_VectorLerp( const vec3_t v1, vec_t lerp, const vec3_t v2, vec3_t dst )
{
	// dst = v1 + lerp * (v2 - v1)

	float32x4_t vv1 = SIMD_Load3f( v1 ), vv2 = SIMD_Load3f( v2 );

	float32x4_t	result = vmlaq_n_f32( vv1, vsubq_f32( vv2, vv1 ), lerp );

	SIMD_Store3f( dst, result );
}

FORCEINLINE void SIMD_VectorM( vec_t scale, const vec3_t a, vec3_t dst )
{
	// dst = scale * a

	float32x4_t va = SIMD_Load3f( a );
	SIMD_Store3f( dst, vmulq_n_f32( va, scale ) );
}

FORCEINLINE void SIMD_VectorMA( const vec3_t a, vec_t scale, const vec3_t b, vec3_t dst )
{
	// dst = a + scale * b

	float32x4_t va = SIMD_Load3f( a ), vb = SIMD_Load3f( b );
	SIMD_Store3f( dst, vmlaq_n_f32( va, vb, scale ) );
}

FORCEINLINE void SIMD_VectorMAMAM( const vec_t scale1, const vec3_t a,
							  const vec_t scale2, const vec3_t b,
							  const vec_t scale3, const vec3_t c, vec3_t dst )
{
	// dst = scale1 * a + scale2 * b + scale3 * c
	float32x4_t va = SIMD_Load3f( a ), vb = SIMD_Load3f( b ), vc = SIMD_Load3f( c );

	float32x4_t vdst = vmlaq_n_f32( vmlaq_n_f32( vmulq_n_f32( va, scale1 ) , vb, scale2 ), vc, scale3 );

	SIMD_Store3f( dst, vdst );
}

FORCEINLINE float32x2_t SIMD_RSqrt_1Iter2f( const float32x2_t v, float32x2_t est )
{
	float32x2_t vest = vmul_f32( v, est );
	return vmul_f32( est, vrsqrts_f32( vest, est ) );
}

FORCEINLINE float32x2_t SIMD_RSqrt1_2f( float32x2_t v )
{
	float32x2_t est = vrsqrte_f32( v );
	return SIMD_RSqrt_1Iter2f( v, est );
}

FORCEINLINE float32x2_t SIMD_RSqrt2_2f( float32x2_t v )
{
	float32x2_t est = vrsqrte_f32( v );
	return SIMD_RSqrt_1Iter2f( v,
		SIMD_RSqrt_1Iter2f( v, est ) );
}

FORCEINLINE float32x2_t SIMD_RSqrt3_2f( float32x2_t v )
{
	float32x2_t est = vrsqrte_f32( v );
	return SIMD_RSqrt_1Iter2f( v,
		SIMD_RSqrt_1Iter2f( v,
		SIMD_RSqrt_1Iter2f( v, est ) ) );
}

FORCEINLINE vec_t SIMD_VectorLength( const vec3_t a )
{
	float32x4_t va = { a[0], a[1], a[2], 0 };
	float32x2_t v2;

	va = vmulq_f32( va, va ); // x*x + y*y + z*z + 0*0
	v2 = vadd_f32( vget_high_f32( va ), vget_low_f32( va ) ); // { x + z, y + w }
	v2 = vpadd_f32( v2, v2 ); // { x + y, x + y }
	v2 = vmul_f32( v2, SIMD_RSqrt1_2f( v2 ) );

	return vget_lane_f32( v2, 0 );
}

FORCEINLINE void SIMD_VectorNormalize2( const vec3_t a, vec3_t out )
{
	float32x4_t va = { a[0], a[1], a[2], 0 };

	float32x4_t v1 = vmulq_f32( va, va ); // x*x + y*y + z*z + w*w
	float32x2_t v2 = vadd_f32( vget_high_f32( v1 ), vget_low_f32( v1 ) ); // { x + y, z + w }
	float32x2_t v3 = SIMD_RSqrt1_2f( vpadd_f32( v2, v2 ) );
	float32x4_t v4 = vcombine_f32( v3, v3 ); // convert to 128 bit vector
	float32x4_t v5 = vmulq_f32( va, v4 );

	SIMD_Store3f( out, v5 );
}
