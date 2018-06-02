
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <windows.h>

#include "macro.h"
#include "tchar_x.h"
#include "xtypes.h"
#include "rand.h"
#include "distance.h"
#include "utime.h"
#include "..\\geometry\\Eigen.h"

#include "matmul.h"

real* new_vector( int n, int inittype, real initval, CvRNG* rng_state ) {
  real * a = MALLOC( real, n );
  if ( CV_RAND_UNI == inittype || CV_RAND_NORMAL == inittype ) {
    CvRNG rng = cvRNG( 0xffffffff );
    if ( rng_state ) {
      rng = *rng_state;
    }
    cvRandArr( 1, n, a, n, 0, initval, rng, inittype, real );
    if ( rng_state ) {
      *rng_state = rng;
    }
  } else {
    FILL( n, a, initval );
  }
  return a;
}

int test_cvGEMM() {
  enum {dim = 500};
  CvRNG rng_state = cvRNG( 0xffffffff );
  real* a = new_vector( dim * dim, CV_RAND_NORMAL, 1, 0 );
  real* b = new_vector( dim * dim, CV_RAND_NORMAL, 1, 0 );
  real* c = MALLOC( real, dim * dim );
  real* d = MALLOC( real, dim * dim );
  FILL( dim * dim, c, 0.f );
  FILL( dim * dim, d, 0.f );
  {
    utime_start( _start_time );
    cvGEMM( dim, dim, dim, a, dim, b, dim, 1, 0, 0, 0, c, dim, 0 );
    printf( "utime=%f\n", utime_elapsed( _start_time ) );
  }
  {
    utime_start( _start_time );
    MAT_ABC( dim, dim, dim, a, dim, b, dim, d, dim );
    printf( "utime=%f\n", utime_elapsed( _start_time ) );
  }
  {
    real sum = 0.f;
    int _N = dim * dim;
    EUCLID_DISTOR2( _N, c, d, sum, real );
    sum = ( real ) sqrt( sum / _N );
    printf( "sum=%f\n", sum );
  }
  SAFEFREE( a );
  SAFEFREE( b );
  SAFEFREE( c );
  SAFEFREE( d );
  return 0;
}

void
cvSVD( int arows, int acols, real* aarr, int astep,
       real* warr, int wstep, int u_cols, real* uarr, int ustep,
       real* varr, int vstep, int flags );
       
int test_cvSVD() {
#if 0
  enum {arows = 5, acols = 3, dim = 5};
  real X0[ arows * acols ] = {
                               -0.6313f, 0.3792f, -1.0181f,
                               -2.3252f, 0.9442f, -0.1821f,
                               -1.2316f, -2.1204f, 1.5210f,
                               1.0556f, -0.6447f, -0.0384f,
                               -0.1132f, -0.7043f, 1.2274f, };
#else
                               
  enum {arows = 3, acols = 5, dim = 5};
  real X[ arows * acols ] = {
                              0.3899f, -0.5596f, 0.7812f, -0.2656f, 0.9863f,
                              0.0880f, 0.4437f, 0.5690f, -1.1878f, -0.5186f,
                              -0.6355f, -0.9499f, -0.8217f, -2.2023f, 0.3274f, };
#endif
                               
  real U[ dim * dim ] = {0}, S[ dim * dim ] = {0}, V[ dim * dim ] = {0};
  cvSVD( arows, acols, X, acols, S, dim, dim, U, dim, V, dim, CV_SVD_V_T + CV_SVD_U_T );
  FPRINT2D( "test_cvSVD_U.txt", "%13.6g, ", "", dim, dim, U, dim, 1, );
  FPRINT2D( "test_cvSVD_S.txt", "%13.6g, ", "", dim, dim, S, dim, 1, );
  FPRINT2D( "test_cvSVD_V.txt", "%13.6g, ", "", dim, dim, V, dim, 1, );
  return 0;
}

int test_cvSVD2() {
  int h = 250, w = 250;
  int i, al = w;
  real *A = MALLOC( real, h * w );
  real *V = MALLOC( real, h * w );
  real *E = MALLOC( real, h * w );
  for ( i = 0; i < h*w; ++i ) {
    A[ i ] = ( real ) i;
  }
  {
    utime_start( _start_time );
    // JACOBI_EIGENS(_N, _A, _V, _E, real)
    // JACOBI_EIGENS(w, A, V, E, real);
    cvSVD( h, w, A, al, A, w, w, 0, 0, 0, 0, 0 );
    printf( "utime=%f\n", utime_elapsed( _start_time ) );
  }
  SAFEFREE( A );
  SAFEFREE( V );
  SAFEFREE( E );
  return 0;
}

void trans( int M, int N, real* A ) {
  real t;
  int i, s, d, cnt;
  if ( 0 && M == N ) {
    int j;
    for ( i = 0; i < M; ++i ) {
      for ( j = i + 1; j < M; ++j ) {
        CV_SWAP( *( A + i * M + j ), *( A + j * M + i ), t );
      }
    }
    return ;
  }
  for ( i = 0; i < M*N; ++i ) {
    for ( d = i, cnt = 0; i != ( d = ( d % M ) * N + ( d / M ) ); ++cnt ) {
      if ( i > d ) {
        cnt = 0;
        break;
      }
    }
    if ( cnt ) {
      for ( d = i, s = i, t = A[ d ]; 1; ) {
        s = ( d % M ) * N + ( d / M );
        if ( i == s ) {
          A[ d ] = t;
          break;
        }
        A[ d ] = A[ s ];
        d = s;
      }
    }
  }
}

void trans1( int M, int N ) {
  int i, s, d, cnt;
  for ( i = 0; i < M*N; ++i ) {
    for ( d = i, cnt = 1; i != ( d = ( d % M ) * N + ( d / M ) ); ++cnt ) {
      if ( i > d ) {
        cnt = 0;
        break;
      }
    }
    if ( cnt ) {
      printf( "(%2d) %2d ", cnt, i );
      for ( d = i; i != ( s = ( d % M ) * N + ( d / M ) );d = s ) {
        printf( "%2d ", s );
      }
      printf( "\n" );
    }
  }
}

int test_trans() {
  int i, j;
  enum {M = 1000, N = 500};
  real* b = MALLOC( real, M * N );
  for ( i = 0; i < M*N; ++i ) {
    b[ i ] = ( real ) i;
  }
  if ( N < 20 )
    PRINT2D(stdout, "%3g, ", M, N, b, N, 1 );
  trans1( 7, 11 );
  {
    utime_start( _start_time );
    trans( M, N, b );
    printf( "utime=%f\n", utime_elapsed( _start_time ) );
  }
  if ( M < 20 )
    PRINT2D(stdout, "%3g, ", N, M, b, M, 1 );
  for ( i = 0; i < N; ++i ) {
    for ( j = 0; j < M; ++j ) {
      if ( b[ i * M + j ] != ( real ) ( j * N + i ) ) {
        //printf("error: (%d %d)=%g", i, j, b[i*N+j]);
      }
    }
  }
  SAFEFREE( b );
  return 0;
}

int main( int argc, char** argv ) {
  test_cvGEMM();
#if 0
  test_cvSVD2();
  
  test_cvSVD();
  test_trans();
#endif
  
  
  return 0;
}
