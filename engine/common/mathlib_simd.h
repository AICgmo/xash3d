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

// supports NEON only at this moment
#include "arm_neon.h"
#include "stdint.h"

#define XASH_MATHLIB_SIMD

typedef float32x2_t simd2f;
typedef float32x4_t simd4f;

typedef int32x2_t simd2i;
typedef int32x4_t simd4i;

typedef uint32x2_t simd2u;
typedef uint32x4_t simd4u;

#ifdef __GNUC__
#define FORCEINLINE __attribute__((always_inline)) inline
#else
#define FORCEINLINE inline // O_o
#endif

// Loading

FORCEINLINE simd4f SIMD_Load4fv( const float *v ){ return vld1q_f32( v ); }
FORCEINLINE simd2f SIMD_Load2fv( const float *v ){ return vld1_f32( v );  }

FORCEINLINE simd4f SIMD_Load4f( float x, float y, float z, float w )
{
	float32_t a[4] = { x, y, z, w };
	return SIMD_Load4fv( a );
}

FORCEINLINE simd4f SIMD_Load3f( float x, float y, float z )
{
	float32_t a[4] = { x, y, z, 0 };
	return SIMD_Load4fv( a );
}
FORCEINLINE simd4f SIMD_Load3fv( const float *v )
{
	float32_t a[4] = { v[0], v[1], v[2], 0 };
	return SIMD_Load4fv( a );
}

FORCEINLINE simd2f SIMD_Load2f( float x, float y )
{
	float32_t a[4] = { x, y };
	return SIMD_Load2fv( a );
}

FORCEINLINE simd4f SIMD_LoadFill4f( float x )
{
	float32_t a[4] = { x, x, x, x };
	return SIMD_Load4fv( a );
}

FORCEINLINE simd2f SIMD_LoadFill2f( float x )
{
	float32_t a[4] = { x, x };
	return SIMD_Load2fv( a );
}

// Unload

FORCEINLINE void SIMD_Store4fv( float *dst, simd4f src )    { vst1q_f32( dst, src ); }
FORCEINLINE void SIMD_Store2fv( float *dst, simd2f src )    { vst1_f32 ( dst, src ); }
FORCEINLINE void SIMD_Store4iv( int32_t *dst, simd4i src )  { vst1q_s32( dst, src ); }
FORCEINLINE void SIMD_Store2iv( int32_t *dst, simd2i src )  { vst1_s32 ( dst, src ); }
FORCEINLINE void SIMD_Store4uv( uint32_t *dst, simd4u src ) { vst1q_u32( dst, src ); }
FORCEINLINE void SIMD_Store2uv( uint32_t *dst, simd2u src ) { vst1_u32 ( dst, src ); }

// Compare

FORCEINLINE simd4u SIMD_Equal4f( simd4f a, simd4f b ) { return vceqq_f32( a, b ); }
FORCEINLINE simd2u SIMD_Equal2f( simd2f a, simd2f b ) {	return vceq_f32( a, b );  }

// TODO: look for absolute greater than, less than, etc
FORCEINLINE simd4u SIMD_GreaterOrEqual4f( simd4f a, simd4f b ) { return vcgeq_f32( a, b ); }
FORCEINLINE simd2u SIMD_GreaterOrEqual2f( simd2f a, simd2f b ) { return vcge_f32( a, b );  }

FORCEINLINE simd4u SIMD_Greater4f( simd4f a, simd4f b ) { return vcgtq_f32( a, b ); }
FORCEINLINE simd2u SIMD_Greater2f( simd2f a, simd2f b ) { return vcgt_f32( a, b );  }

FORCEINLINE simd4u SIMD_LessOrEqual4f( simd4f a, simd4f b ) { return vcleq_f32( a, b ); }
FORCEINLINE simd2u SIMD_LessOrEqual2f( simd2f a, simd2f b ) { return vcle_f32( a, b );  }

FORCEINLINE simd4u SIMD_Less4f( simd4f a, simd4f b ) { return vcltq_f32( a, b ); }
FORCEINLINE simd2u SIMD_Less2f( simd2f a, simd2f b ) { return vclt_f32( a, b );  }

// Simple math operations
FORCEINLINE simd4f SIMD_Add4f( simd4f a, simd4f b ) { return vaddq_f32( a, b ); }
FORCEINLINE simd2f SIMD_Add2f( simd2f a, simd2f b ) { return vadd_f32( a, b );  }
FORCEINLINE simd4i SIMD_Add4i( simd4i a, simd4i b ) { return vaddq_s32( a, b ); }
FORCEINLINE simd2i SIMD_Add2i( simd2i a, simd2i b ) { return vadd_s32( a, b );  }
FORCEINLINE simd4i SIMD_Add4u( simd4u a, simd4u b ) { return vaddq_u32( a, b ); }
FORCEINLINE simd2i SIMD_Add2u( simd2u a, simd2u b ) { return vadd_u32( a, b );  }

FORCEINLINE simd4f SIMD_Sub4f( simd4f a, simd4f b ) { return vsubq_f32( a, b ); }
FORCEINLINE simd2f SIMD_Sub2f( simd2f a, simd2f b ) { return vsub_f32( a, b );  }
FORCEINLINE simd4i SIMD_Sub4i( simd4i a, simd4i b ) { return vsubq_s32( a, b ); }
FORCEINLINE simd2i SIMD_Sub2i( simd2i a, simd2i b ) { return vsub_s32( a, b );  }
FORCEINLINE simd4i SIMD_Sub4u( simd4u a, simd4u b ) { return vsubq_u32( a, b ); }
FORCEINLINE simd2i SIMD_Sub2u( simd2u a, simd2u b ) { return vsub_u32( a, b );  }

FORCEINLINE simd4f SIMD_Mul4f( simd4f a, simd4f b ) { return vmulq_f32( a, b ); }
FORCEINLINE simd2f SIMD_Mul2f( simd2f a, simd2f b ) { return vmul_f32( a, b );  }
FORCEINLINE simd4i SIMD_Mul4i( simd4i a, simd4i b ) { return vmulq_s32( a, b ); }
FORCEINLINE simd2i SIMD_Mul2i( simd2i a, simd2i b ) { return vmul_s32( a, b );  }
FORCEINLINE simd4i SIMD_Mul4u( simd4u a, simd4u b ) { return vmulq_u32( a, b ); }
FORCEINLINE simd2i SIMD_Mul2u( simd2u a, simd2u b ) { return vmul_u32( a, b );  }

FORCEINLINE simd4f SIMD_Reciprocal4f( simd4f a ) { return vrecpeq_f32( a ); }
FORCEINLINE simd2f SIMD_Reciprocal2f( simd2f a ) { return vrecpe_f32( a );  }
FORCEINLINE simd4i SIMD_Reciprocal4u( simd4u a ) { return vrecpeq_u32( a ); }
FORCEINLINE simd2i SIMD_Reciprocal2u( simd2u a ) { return vrecpe_u32( a );  }

FORCEINLINE simd4f SIMD_RSqrt4f( simd4f a ) { return vrsqrteq_f32( a ); }
FORCEINLINE simd2f SIMD_RSqrt2f( simd2f a ) { return vrsqrte_f32( a );  }
FORCEINLINE simd4i SIMD_RSqrt4u( simd4u a ) { return vrsqrteq_u32( a ); }
FORCEINLINE simd2i SIMD_RSqrt2u( simd2u a ) { return vrsqrte_u32( a );  }

// TODO: Divide1
/*
FORCEINLINE simd4f SIMD_Div4f( simd4f a, simd4f b ) { return vmulq_f32( a, b ); }
FORCEINLINE simd2f SIMD_Div2f( simd2f a, simd2f b ) { return vmul_f32( a, b );  }
FORCEINLINE simd4i SIMD_Div4i( simd4i a, simd4i b ) { return vmulq_s32( a, b ); }
FORCEINLINE simd2i SIMD_Div2i( simd2i a, simd2i b ) { return vmul_s32( a, b );  }
FORCEINLINE simd4i SIMD_Div4u( simd4u a, simd4u b ) { return vmulq_u32( a, b ); }
FORCEINLINE simd2i SIMD_Div2u( simd2u a, simd2u b ) { return vmul_u32( a, b );  }
*/

// Get low/high values

FORCEINLINE simd2f SIMD_GetHigh4f( simd4f a ) { return vget_high_f32( a ); }
FORCEINLINE simd2f SIMD_GetLow4f( simd4f a ) { return vget_low_f32( a ); }

// Combine

FORCEINLINE simd4f SIMD_Combine2f( simd2f a, simd2f b ) { return vcombine_f32( a, b ); }

// Lane values

FORCEINLINE float SIMD_GetX4f( simd4f a ) { return vgetq_lane_f32( a, 0 ); }
FORCEINLINE float SIMD_GetY4f( simd4f a ) { return vgetq_lane_f32( a, 1 ); }
FORCEINLINE float SIMD_GetZ4f( simd4f a ) { return vgetq_lane_f32( a, 2 ); }
FORCEINLINE float SIMD_GetW4f( simd4f a ) { return vgetq_lane_f32( a, 3 ); }
FORCEINLINE float SIMD_GetX2f( simd2f a ) { return vget_lane_f32( a, 0 ); }
FORCEINLINE float SIMD_GetY2f( simd2f a ) { return vget_lane_f32( a, 1 ); }
