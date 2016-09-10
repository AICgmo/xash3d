#define XASH_USE_SIMD
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#define COMMON_H

typedef float		vec_t;
typedef vec_t		vec2_t[2];
typedef vec_t		vec3_t[3];
typedef vec_t		vec4_t[4];
typedef vec_t		quat_t[4];
typedef vec_t		matrix3x4[3][4];
typedef vec_t		matrix4x4[4][4];
typedef short word;
typedef int qboolean;
#define true 1
#define false 0

#include "engine/common/mathlib.h"
// !!!
#include "engine/common/mathlib.c" // !!!
#include "engine/common/matrixlib.c" // !!!
#include "engine/common/mathlib_simd_neon.c" // !!!
// !!!

// Config
#define BENCHMARK_SIZE 220000 
#define ERROR_EPSILON 0.01f

// Routines
#define DECLARE_DEFAULT_VARIABLES \
	int i; \
	struct timespec end, start; \
	unsigned long long time1, time2;
#define START clock_gettime( CLOCK_MONOTONIC, &start )
#define END( time ) \
	clock_gettime( CLOCK_MONOTONIC, &end ); \
	time = Com_TimeDiff( start, end ); \
	memset( &start, 0, sizeof( struct timespec ) );\
	memset( &end, 0, sizeof( struct timespec ) );
#define REPORT_TIME_DIFF( name ) \
	printf( "-- SIMD_%s:   %lld ms\n-- %s:        %lld ms\n", \
		name, time1, name, time2 )

float Com_RandomFloat( float flLow, float flHigh )
{
	return flLow + ( (double)rand() ) / ( (double)(RAND_MAX/(flHigh-flLow)) );
}

unsigned long long Com_TimeDiff( struct timespec s, struct timespec e )
{
	return ( e.tv_sec - s.tv_sec ) * 1000000 + ( e.tv_nsec - s.tv_nsec ) / 1000;
}

void Test_FillRandomVector( vec_t *v, size_t s )
{
	int i;
	for( i = 0; i < s; i++ ) { v[i] = Com_RandomFloat( -30, 30 ); }
	printf("-- Test vector is filled with random data\n");
}

void Test_CheckErrors( vec_t *first, vec_t *second, size_t s, vec_t eps )
{
	size_t i, errors = 0;
	float maxError = 0.0f;
	for( i = 0; i < s; i++ )
	{
		float error = fabs(first[i] - second[i]);
		if( maxError < error ) maxError = error;
		if( error >= eps )
		{
			printf("%i %f %f\n", i, first[i], second[i]);
			errors++;
		}
	}
	printf( "-- Errors: %llu/%llu. Max Error: %f\n", (long long)errors, (long long)s, maxError );
}

/*
void Test_CheckVectorAdd( vec3_t *vTestOrig, vec3_t *vTestSIMD, vec3_t *vTestClassic )
{
	int i;
	struct timespec end, start;	

	START;
	for( i = 0; i < BENCHMARK_SIZE-1; i++ )
	{
		SIMD_VectorAdd(vTestOrig[i], vTestOrig[i+1], vTestSIMD[i]);
	}
	END;
	REPORT_TIME_DIFF( "SIMD_VectorAdd" );
	
	START;
	for( i = 0; i < BENCHMARK_SIZE-1; i++ )
	{
		VectorAdd( vTestOrig[i], vTestOrig[i+1], vTestClassic[i] );
	}
	END;
	REPORT_TIME_DIFF( "VectorAdd" );
}

void Test_CheckVectorSubtract( vec3_t *vTestOrig, vec3_t *vTestSIMD, vec3_t *vTestClassic )
{
	int i;
	struct timespec end, start;	
	START;
	for( i = 0; i < BENCHMARK_SIZE-1; i++ )
	{
		SIMD_VectorSubtract(vTestOrig[i], vTestOrig[i+1], vTestSIMD[i]);
	}
	END;
	REPORT_TIME_DIFF( "SIMD_VectorSubtract" );
	
	START;
	for( i = 0; i < BENCHMARK_SIZE-1; i++ )
	{
		VectorSubtract( vTestOrig[i], vTestOrig[i+1], vTestClassic[i] );
	}
	END;
	REPORT_TIME_DIFF( "VectorSubtract" );
}*/

void Test_VectorScale( vec3_t *vTestOrig, vec3_t *vTestSIMD, vec3_t *vTestClassic )
{
	DECLARE_DEFAULT_VARIABLES;
	START;
	for( i = 0; i < BENCHMARK_SIZE; i++ )
	{
		SIMD_VectorScale(vTestOrig[i], vTestOrig[i][0], vTestSIMD[i]);
	}
	END(time1);
	
	START;
	for( i = 0; i < BENCHMARK_SIZE; i++ )
	{
		VectorScale( vTestOrig[i], vTestOrig[i][0], vTestClassic[i] );
	}
	END(time2);
	
	REPORT_TIME_DIFF( "VectorScale" );
}

void Test_VectorLerp( vec3_t *vTestOrig, vec3_t *vTestSIMD, vec3_t *vTestClassic )
{
	DECLARE_DEFAULT_VARIABLES;
	START;
	for( i = 0; i < BENCHMARK_SIZE - 1; i++ )
	{
		SIMD_VectorLerp(vTestOrig[i], vTestOrig[i][0], vTestOrig[i+1], vTestSIMD[i]);
	}
	END(time1);
	
	START;
	for( i = 0; i < BENCHMARK_SIZE - 1; i++ )
	{
		VectorLerp(vTestOrig[i], vTestOrig[i][0], vTestOrig[i+1], vTestClassic[i]);
	}
	END(time2);
	REPORT_TIME_DIFF( "VectorLerp" );
}

void Test_VectorMA( vec3_t *vTestOrig, vec3_t *vTestSIMD, vec3_t *vTestClassic )
{
	DECLARE_DEFAULT_VARIABLES;
	START;
	for( i = 0; i < BENCHMARK_SIZE - 1; i++ )
	{
		SIMD_VectorMA(vTestOrig[i], vTestOrig[i][0], vTestOrig[i+1], vTestSIMD[i]);
	}
	END(time1);
	
	START;
	for( i = 0; i < BENCHMARK_SIZE - 1; i++ )
	{
		VectorMA(vTestOrig[i], vTestOrig[i][0], vTestOrig[i+1], vTestClassic[i]);
	}
	END(time2);
	REPORT_TIME_DIFF( "VectorMA" );
}

void Test_VectorMAMAM( vec3_t *vTestOrig, vec3_t *vTestSIMD, vec3_t *vTestClassic )
{
	DECLARE_DEFAULT_VARIABLES;
	START;
	for( i = 0; i < BENCHMARK_SIZE; i++ )
	{
		SIMD_VectorMAMAM(vTestOrig[i][0], vTestOrig[i], 
						vTestOrig[i][1], vTestOrig[i],
						vTestOrig[i][2], vTestOrig[i], vTestSIMD[i]);
	}
	END(time1);
	
	START;
	for( i = 0; i < BENCHMARK_SIZE; i++ )
	{
		VectorMAMAM(vTestOrig[i][0], vTestOrig[i], 
						vTestOrig[i][1], vTestOrig[i],
						vTestOrig[i][2], vTestOrig[i], vTestClassic[i]);
	}
	END(time2);
	REPORT_TIME_DIFF( "VectorMAMAM" );
}

void Test_VectorNormalize( vec3_t *vTestOrig, vec3_t *vTestSIMD, vec3_t *vTestClassic )
{
	DECLARE_DEFAULT_VARIABLES;
	START;
	for( i = 0; i < BENCHMARK_SIZE; i++ )
	{
		SIMD_VectorNormalize2(vTestOrig[i], vTestSIMD[i]);
	}
	END(time1);
	
	START;
	for( i = 0; i < BENCHMARK_SIZE; i++ )
	{
		VectorNormalize2( vTestOrig[i], vTestClassic[i] );
	}
	END(time2);
	REPORT_TIME_DIFF( "VectorNormalize2" );
}

void Test_VectorLength( vec3_t *vTestOrig, vec3_t *vTestSIMD, vec3_t *vTestClassic )
{
	DECLARE_DEFAULT_VARIABLES;
	vec_t *testSIMD = (vec_t*)vTestSIMD, *testClassic = (vec_t*)vTestClassic;
	START;
	for( i = 0; i < BENCHMARK_SIZE; i++ )
	{
		testSIMD[i] = SIMD_VectorLength( vTestOrig[i] );
	}
	END(time1);
	
	START;
	for( i = 0; i < BENCHMARK_SIZE; i++ )
	{
		testClassic[i] = VectorLength( vTestOrig[i] );
	}
	END(time2);
	REPORT_TIME_DIFF( "VectorLength" );
}

void Test_Matrix3x4_VectorTransform( vec3_t *vTestOrig, vec3_t *vTestSIMD, vec3_t *vTestClassic )
{
	DECLARE_DEFAULT_VARIABLES;
	START;
	for( i = 0; i < BENCHMARK_SIZE - 4; i++ )
	{
		SIMD_Matrix3x4_VectorTransform( (vec4_t*)&vTestOrig[i], vTestOrig[i], vTestSIMD[i] );
	}
	END(time1);
	
	START;
	for( i = 0; i < BENCHMARK_SIZE - 4; i++ )
	{
		Matrix3x4_VectorTransform( (vec4_t*)&vTestOrig[i], vTestOrig[i], vTestClassic[i] );
	}
	END(time2);
	REPORT_TIME_DIFF( "Matrix3x4_VectorTransform" );
}

void Test_Matrix3x4_VectorITransform( vec3_t *vTestOrig, vec3_t *vTestSIMD, vec3_t *vTestClassic )
{
	DECLARE_DEFAULT_VARIABLES;
	START;
	for( i = 0; i < BENCHMARK_SIZE - 4; i++ )
	{
		SIMD_Matrix3x4_VectorITransform( (vec4_t*)&vTestOrig[i], vTestOrig[i], vTestSIMD[i] );
	}
	END(time1);
	
	START;
	for( i = 0; i < BENCHMARK_SIZE - 4; i++ )
	{
		Matrix3x4_VectorITransform( (vec4_t*)&vTestOrig[i], vTestOrig[i], vTestClassic[i] );
	}
	END(time2);
	REPORT_TIME_DIFF( "Matrix3x4_VectorITransform" );
}

void Test_Matrix3x4_VectorRotate( vec3_t *vTestOrig, vec3_t *vTestSIMD, vec3_t *vTestClassic )
{
	DECLARE_DEFAULT_VARIABLES;
	START;
	for( i = 0; i < BENCHMARK_SIZE - 4; i++ )
	{
		SIMD_Matrix3x4_VectorRotate( (vec4_t*)&vTestOrig[i], vTestOrig[i], vTestSIMD[i] );
	}
	END(time1);
	
	START;
	for( i = 0; i < BENCHMARK_SIZE - 4; i++ )
	{
		Matrix3x4_VectorRotate( (vec4_t*)&vTestOrig[i], vTestOrig[i], vTestClassic[i] );
	}
	END(time2);
	REPORT_TIME_DIFF( "Matrix3x4_VectorRotate" );
}

int main() 
{
	vec3_t vTestOrig[BENCHMARK_SIZE], vTestClassic[BENCHMARK_SIZE], vTestSIMD[BENCHMARK_SIZE];
	//matrix3x4 mTest3x4[BENCHMARK_SIZE];
	//matrix4x4 mTest4x4[BENCHMARK_SIZE];
	
	srand(time(NULL));
	
	Test_FillRandomVector( (float*)vTestOrig, 3 * BENCHMARK_SIZE );
	
	/*Test_CheckVectorAdd( vTestOrig, vTestSIMD, vTestClassic );
	Test_CheckErrors( (float*)vTestSIMD, (float*)vTestClassic, 3 * ( BENCHMARK_SIZE - 1 ) );
	
	Test_CheckVectorSubtract( vTestOrig, vTestSIMD, vTestClassic );
	Test_CheckErrors( (float*)vTestSIMD, (float*)vTestClassic, 3 * ( BENCHMARK_SIZE - 1 ) );
	
	*/Test_VectorScale( vTestOrig, vTestSIMD, vTestClassic );
	Test_CheckErrors( (float*)vTestSIMD, (float*)vTestClassic, 3 * ( BENCHMARK_SIZE ), ERROR_EPSILON );
	
	Test_VectorLerp( vTestOrig, vTestSIMD, vTestClassic );
	Test_CheckErrors( (float*)vTestSIMD, (float*)vTestClassic, 3 * ( BENCHMARK_SIZE - 1 ), ERROR_EPSILON );
		
	Test_VectorMA( vTestOrig, vTestSIMD, vTestClassic );
	Test_CheckErrors( (float*)vTestSIMD, (float*)vTestClassic, 3 * ( BENCHMARK_SIZE - 1 ), ERROR_EPSILON );
	
	Test_VectorMAMAM( vTestOrig, vTestSIMD, vTestClassic );
	Test_CheckErrors( (float*)vTestSIMD, (float*)vTestClassic, 3 * ( BENCHMARK_SIZE ), ERROR_EPSILON );
	
	Test_VectorNormalize( vTestOrig, vTestSIMD, vTestClassic );
	Test_CheckErrors( (float*)vTestSIMD, (float*)vTestClassic, 3 * ( BENCHMARK_SIZE ), 0.0001 );
	
	Test_VectorLength( vTestOrig, vTestSIMD, vTestClassic );
	Test_CheckErrors( (float*)vTestSIMD, (float*)vTestClassic, BENCHMARK_SIZE, ERROR_EPSILON );
	
	Test_Matrix3x4_VectorTransform( vTestOrig, vTestSIMD, vTestClassic );
	Test_CheckErrors( (float*)vTestSIMD, (float*)vTestClassic, BENCHMARK_SIZE - 4, ERROR_EPSILON );
	
	Test_Matrix3x4_VectorITransform( vTestOrig, vTestSIMD, vTestClassic );
	Test_CheckErrors( (float*)vTestSIMD, (float*)vTestClassic, BENCHMARK_SIZE - 4, ERROR_EPSILON );
	
	Test_Matrix3x4_VectorRotate( vTestOrig, vTestSIMD, vTestClassic );
	Test_CheckErrors( (float*)vTestSIMD, (float*)vTestClassic, BENCHMARK_SIZE - 4, ERROR_EPSILON );
}
