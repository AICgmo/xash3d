#ifdef __arm__

#include "common.h"
#include "mathlib.h"
#include "mathlib_simd_neon.h"

void SIMD_CrossProduct(const vec3_t x, const vec3_t y, vec3_t dst)
{

}

void SIMD_Matrix3x4_VectorTransform( cmatrix3x4 in, const float vv[3], float out[3] )
{
	float32x4_t vec = { vv[0], vv[1], vv[2], 1.0f };
	float32x4_t mat[3] =
	{
		vmulq_f32( vld1q_f32( in[0] ), vec ),
		vmulq_f32( vld1q_f32( in[1] ), vec ),
		vmulq_f32( vld1q_f32( in[2] ), vec )
	};
	float32x2_t v[3] =
	{
		vadd_f32( vget_high_f32( mat[0] ), vget_low_f32( mat[0] ) ),
		vadd_f32( vget_high_f32( mat[1] ), vget_low_f32( mat[1] ) ),
		vadd_f32( vget_high_f32( mat[2] ), vget_low_f32( mat[2] ) )
	};

	out[0] = vget_lane_f32(vpadd_f32( v[0], v[0] ), 0);
	out[1] = vget_lane_f32(vpadd_f32( v[1], v[1] ), 0);
	out[2] = vget_lane_f32(vpadd_f32( v[2], v[2] ), 0);
}

void SIMD_Matrix4x4_VectorITransform( cmatrix4x4 in, const float vv[3], float out[3] )
{
	float32x4_t vec = { vv[0], vv[1], vv[2], 0.0f }; // ignore w
	float32x4x4_t mat = vld4q_f32( in );
	float32x4_u_t dir = vsetq_lane_f32( 0, vsubq_f32( vec, mat.val[3] ), 3 );

	float32x4_t v1 =
		vmlaq_n_f32(
			vmlaq_n_f32(
				vmulq_n_f32(
					mat.val[2],
					dir.s[2]
				),
				mat.val[1],
				dir.s[1]
			),
			mat.val[0],
			dir.s[0]
		);
	float32x2_t v2 = vadd_f32( vget_high_f32( v1 ), vget_low_f32( v1 ) );
	float32x2_t v3 = vpadd_f32(v2, v2);
	SIMD_Store3f( out, v3 );
}

void SIMD_Matrix3x4_VectorRotate( cmatrix3x4 in, const float vv[3], float out[3] )
{
	float32x4_t vec = { vv[0], vv[1], vv[2], 0.0f };
	float32x4_t mat[3] =
	{
		vmulq_f32( vld1q_f32( in[0] ), vec ),
		vmulq_f32( vld1q_f32( in[1] ), vec ),
		vmulq_f32( vld1q_f32( in[2] ), vec )
	};
	float32x2_t v[3] =
	{
		vadd_f32( vget_high_f32( mat[0] ), vget_low_f32( mat[0] ) ),
		vadd_f32( vget_high_f32( mat[1] ), vget_low_f32( mat[1] ) ),
		vadd_f32( vget_high_f32( mat[2] ), vget_low_f32( mat[2] ) )
	};

	out[0] = vget_lane_f32(vpadd_f32( v[0], v[0] ), 0);
	out[1] = vget_lane_f32(vpadd_f32( v[1], v[1] ), 0);
	out[2] = vget_lane_f32(vpadd_f32( v[2], v[2] ), 0);
}

#endif // __arm__
