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
float Com_RandomFloat( float flLow, float flHigh )
{
	float r3 = flLow + ( (double)rand() ) / ( (double)(RAND_MAX/(flHigh-flLow)) );
}

unsigned long long Com_TimeDiff( struct timespec s, struct timespec e )
{
	return ( e.tv_sec - s.tv_sec ) * 1000000 + ( e.tv_nsec - s.tv_nsec ) / 1000;
}

#define BENCHMARK_SIZE 65536 
#define ERROR_EPSILON 0.00001f
#define START clock_gettime( CLOCK_MONOTONIC, &start )
#define END clock_gettime( CLOCK_MONOTONIC, &end )
#define REPORT_TIME_DIFF(name) printf("-- %s: %lld ms\n", name, Com_TimeDiff( start, end ) )

void Test_FillRandomVector( vec_t *v, size_t s )
{
	int i;
	for( i = 0; i < s; i++ )
	{
		v[i] = Com_RandomFloat( -30, 30 );
	}
	printf("-- Test vector is filled with random data\n");
}

void Test_CheckErrors( vec_t *first, vec_t *end, size_t s )
{
	size_t i, errors = 0;
	for( i = 0; i < s; i++ )
	{
		if( fabs(first[i] - end[i]) >= ERROR_EPSILON )
			errors++;
	}
	printf( "-- Errors: %llu/%llu\n", (long long)errors, (long long)s );
}

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
}

void Test_CheckVectorScale( vec3_t *vTestOrig, vec3_t *vTestSIMD, vec3_t *vTestClassic )
{
	int i;
	struct timespec end, start;	
	START;
	for( i = 0; i < BENCHMARK_SIZE-1; i++ )
	{
		SIMD_VectorScale(vTestOrig[i], vTestOrig[i][0], vTestSIMD[i]);
	}
	END;
	REPORT_TIME_DIFF( "SIMD_VectorScale" );
	
	START;
	for( i = 0; i < BENCHMARK_SIZE-1; i++ )
	{
		VectorScale( vTestOrig[i], vTestOrig[i][0], vTestClassic[i] );
	}
	END;
	REPORT_TIME_DIFF( "VectorScale" );
}

void Test_CheckVectorMAMAM( vec3_t *vTestOrig, vec3_t *vTestSIMD, vec3_t *vTestClassic )
{
	int i;
	struct timespec end, start;	
	START;
	for( i = 0; i < BENCHMARK_SIZE-1; i++ )
	{
		SIMD_VectorMAMAM(vTestOrig[i][0], vTestOrig[i], 
						vTestOrig[i][1], vTestOrig[i],
						vTestOrig[i][2], vTestOrig[i], vTestSIMD[i]);
	}
	END;
	REPORT_TIME_DIFF( "SIMD_VectorMAMAM" );
	
	START;
	for( i = 0; i < BENCHMARK_SIZE-1; i++ )
	{
		VectorMAMAM(vTestOrig[i][0], vTestOrig[i], 
						vTestOrig[i][1], vTestOrig[i],
						vTestOrig[i][2], vTestOrig[i], vTestClassic[i]);
	}
	END;
	REPORT_TIME_DIFF( "VectorMAMAM" );
}

void Test_CheckVectorLength( vec3_t *vTestOrig, vec3_t *vTestSIMD, vec3_t *vTestClassic )
{
	int i;
	struct timespec end, start;	
	vec_t *testSIMD = (vec_t*)vTestSIMD, *testClassic = (vec_t*)vTestClassic;
	START;
	for( i = 0; i < BENCHMARK_SIZE; i++ )
	{
		testSIMD[i] = SIMD_VectorLength( vTestOrig[i] );
	}
	END;
	REPORT_TIME_DIFF( "SIMD_VectorLength" );
	
	START;
	for( i = 0; i < BENCHMARK_SIZE-1; i++ )
	{
		testClassic[i] = VectorLength( vTestOrig[i] );
	}
	END;
	REPORT_TIME_DIFF( "VectorLength" );
}

int main() 
{
	vec3_t vTestOrig[BENCHMARK_SIZE], vTestClassic[BENCHMARK_SIZE], vTestSIMD[BENCHMARK_SIZE];
	//matrix3x4 mTest3x4[BENCHMARK_SIZE];
	//matrix4x4 mTest4x4[BENCHMARK_SIZE];
	
	srand(time(NULL));
	
	Test_FillRandomVector( (float*)vTestOrig, 3 * BENCHMARK_SIZE );
	
	Test_CheckVectorAdd( vTestOrig, vTestSIMD, vTestClassic );
	Test_CheckErrors( (float*)vTestSIMD, (float*)vTestClassic, 3 * ( BENCHMARK_SIZE - 1 ) );
	
	Test_CheckVectorSubtract( vTestOrig, vTestSIMD, vTestClassic );
	Test_CheckErrors( (float*)vTestSIMD, (float*)vTestClassic, 3 * ( BENCHMARK_SIZE - 1 ) );
	
	Test_CheckVectorScale( vTestOrig, vTestSIMD, vTestClassic );
	Test_CheckErrors( (float*)vTestSIMD, (float*)vTestClassic, 3 * ( BENCHMARK_SIZE - 1 ) );
	
	Test_CheckVectorMAMAM( vTestOrig, vTestSIMD, vTestClassic );
	Test_CheckErrors( (float*)vTestSIMD, (float*)vTestClassic, 3 * ( BENCHMARK_SIZE - 1 ) );
	
	Test_CheckVectorLength( vTestOrig, vTestSIMD, vTestClassic );
	Test_CheckErrors( (float*)vTestSIMD, (float*)vTestClassic, BENCHMARK_SIZE );
}
