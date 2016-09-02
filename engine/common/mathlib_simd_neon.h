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
#include <arm_neon.h>
//#include "arm_neon_neon.h"

#define ALIGN __attribute__((aligned(16)))

FORCEINLINE void SIMD_Load3fv( float32x4_t dst, const vec3_t src )
{
	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[2];
	// w value is rubbish
}

FORCEINLINE void SIMD_Load3f( float32x4_t dst, const vec_t x, const vec_t y, const vec_t z )
{
	dst[0] = x;
	dst[1] = y;
	dst[2] = z;
	// w value is rubbish
}

FORCEINLINE void SIMD_Load2fv( float32x2_t dst, const vec2_t v )
{
	dst[0] = v[0];
	dst[1] = v[1];
}

FORCEINLINE void SIMD_Store2f( vec3_t dst, float32x2_t v )
{
	dst[0] = v[0];
	dst[1] = v[1];
}

FORCEINLINE void SIMD_Store3f( vec3_t dst, float32x4_t v )
{
	/*vec4_t v4;
	vst1q_f32( v4, v );*/
	dst[0] = v[0];
	dst[1] = v[1];
	dst[2] = v[2];
}

FORCEINLINE void SIMD_VectorAdd( const vec3_t x, const vec3_t y, vec3_t dst )
{
	float32x4_t vx, vy;

	SIMD_Load3fv( vx, x );
	SIMD_Load3fv( vy, y );

	SIMD_Store3f( dst, vaddq_f32( vx, vy ) );
}

FORCEINLINE void SIMD_VectorSubtract( const vec3_t x, const vec3_t y, vec3_t dst )
{
	float32x4_t vx, vy;

	SIMD_Load3fv( vx, x );
	SIMD_Load3fv( vy, y );
	SIMD_Store3f( dst, vsubq_f32( vx, vy ) );
}

FORCEINLINE void SIMD_VectorScale( const vec3_t in, vec_t scale, vec3_t out )
{
	float32x4_t vin;

	SIMD_Load3fv( vin, in );

	SIMD_Store3f( out, vmulq_n_f32( vin, scale ) );
}

FORCEINLINE void SIMD_VectorLerp( const vec3_t v1, vec_t lerp, const vec3_t v2, vec3_t dst )
{
	// dst = v1 + lerp * (v2 - v1)

	float32x4_t vv1, vv2, result;

	SIMD_Load3fv( vv1, v1 );
	SIMD_Load3fv( vv2, v2 );

	result = vmlaq_n_f32( vv1, vsubq_f32( vv2, vv1 ), lerp );

	SIMD_Store3f( dst, result );
}

FORCEINLINE void SIMD_VectorAverage( const vec3_t x, const vec3_t y, vec3_t dst )
{
	// dst = (x + y) * 0.5f;

	float32x4_t vx, vy, result;

	SIMD_Load3fv( vx, x );
	SIMD_Load3fv( vy, y );

	result = vmulq_n_f32( vaddq_f32( vx, vy ), 0.5f );

	SIMD_Store3f( dst, result );
}

FORCEINLINE void SIMD_Vector2Subtract( const vec2_t x, const vec2_t y, vec2_t dst )
{
	float32x2_t vx, vy;

	SIMD_Load2fv( vx, x );
	SIMD_Load2fv( vy, y );

	SIMD_Store2f( dst, vsub_f32( vx, vy ) );
}

FORCEINLINE void SIMD_Vector2Average( const vec2_t x, const vec2_t y, vec2_t dst )
{
	float32x2_t vx, vy;

	SIMD_Load2fv( vx, x );
	SIMD_Load2fv( vy, y );

	SIMD_Store2f( dst,  vmul_n_f32( vadd_f32( vx, vy ), 0.5f ) );
}

FORCEINLINE void SIMD_Vector2Add( const vec2_t x, const vec2_t y, vec2_t dst )
{
	float32x2_t vx, vy;

	SIMD_Load2fv( vx, x );
	SIMD_Load2fv( vy, y );

	SIMD_Store2f( dst, vadd_f32( vx, vy ) );
}

FORCEINLINE void SIMD_Vector2Lerp( const vec2_t v1, vec_t lerp, const vec2_t v2, vec2_t dst )
{
	float32x2_t vv1, vv2;

	SIMD_Load2fv( vv1, v1 );
	SIMD_Load2fv( vv2, v2 );

	SIMD_Store2f( dst, vmla_n_f32( vv1, vsub_f32( vv2, vv1 ), lerp ) );
}

FORCEINLINE void SIMD_VectorM( vec_t scale, const vec3_t a, vec3_t dst )
{
	// dst = scale * a

	float32x4_t va;

	SIMD_Load3fv( va, a );
	SIMD_Store3f( dst, vmulq_n_f32( va, scale ) );
}

FORCEINLINE void SIMD_VectorMA( const vec3_t a, vec_t scale, const vec3_t b, vec3_t dst )
{
	// dst = a + scale * b

	float32x4_t va, vb;

	SIMD_Load3fv( va, a );
	SIMD_Load3fv( vb, b );

	SIMD_Store3f( dst, vmlaq_n_f32( va, vb, scale ) );
}

FORCEINLINE void SIMD_VectorMAMAM( const vec_t scale1, const vec3_t a,
							  const vec_t scale2, const vec3_t b,
							  const vec_t scale3, const vec3_t c, vec3_t dst )
{
	// dst = scale1 * a + scale2 * b + scale3 * c
	float32x4_t va, vb, vc, vdst;

	SIMD_Load3fv( va, a );
	SIMD_Load3fv( vb, b );
	SIMD_Load3fv( vc, c );

	vdst = vmlaq_n_f32( vmlaq_n_f32( vmulq_n_f32( va, scale1 ) , vb, scale2 ), vc, scale3 );

	SIMD_Store3f( dst, vdst );
}

FORCEINLINE void SIMD_VectorNegate( const vec3_t in, vec3_t out )
{
	float32x4_t vin;
	SIMD_Load3fv( vin, in );
	SIMD_Store3f( out, vnegq_f32(vin));
}

FORCEINLINE vec_t SIMD_VectorLength( const vec3_t a )
{
	float32x4_t va = { a[0], a[1], a[2], 0 }, v1;
	float32x2_t v2, v3;

	v1 = vmulq_f32( va, va ); // x*x + y*y + z*z + w*w
	v2 = vadd_f32( vget_high_f32( v1 ), vget_low_f32( v1 ) ); // { x + y, z + w }
	v3 = vmul_f32( vrsqrte_f32( vpadd_f32( v2, v2 ) ), v2 ); // x * 1/sqrt(x)

	return vget_lane_f32( v3, 0 );
}
