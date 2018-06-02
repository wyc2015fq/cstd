
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ml.h"
#include "macro.h"
#include "xtypes.h"
#include "rand.h"
#include "utime.h"
#include "chalg.h"
#include "Clustering.h"


int test_rand() {
  CvRNG rng_state = cvRNG( 0xffffffff );
  real a[ 100 ];
  int b[ 100 ];
  int t0 = ( int ) ceil( 0 );
  int t1 = ( int ) floor( 100 ) - t0;
  cvRandArr( 1, 100, a, 100, 0, 100, rng_state, CV_RAND_UNI, real );
  cvRandArr( 1, 100, a, 100, 0, 1, rng_state, CV_RAND_NORMAL, real );
  _RAND_BITS( 1, 100, b, 100, 0, 100, rng_state, int );
  return 0;
}

real point_distor( const void* a, const void* b, const void* userdata ) {
  real * ra = ( real* ) a;
  real* rb = ( real* ) b;
  real dx = ra[ 0 ] - rb[ 0 ];
  real dy = ra[ 1 ] - rb[ 1 ];
  return dx*dx + dy*dy;
}

int test_KNEAREST() {
  PKNEAREST knn = VECTOR_NEW( 0, 2 * sizeof( real ) + sizeof( int ) );
  CvRNG rng_state = cvRNG( -1 );
  int i, j, k, n = 50;
  real vv0[ 100 ], vv1[ 100 ];
  int base = 50;
  real c0 = 2.f * base, c1 = 3.f * base, ck = 0.5f * base;
  int img_height = 5 * base, img_width = 5 * base, img_cn = 3;
  int img_step = img_width * img_cn;
  unsigned char* img = MALLOC( unsigned char, img_height * img_step );
  MEMSET( img, 0, img_height * img_step );
  RANDARR_1D( 100, vv0, c0, ck, rng_state, CV_RAND_NORMAL, real );
  for ( i = 0; i < n; ++i ) {
    real* pt = VECTOR_PUSH_BACK( knn, 1, vv0 + 2 * i, 2 * sizeof( real ), 2 * sizeof( real ) );
    int* id = ( int* ) ( pt + 2 );
    *id = 0;
  }
  RANDARR_1D( 100, vv1, c1, ck, rng_state, CV_RAND_NORMAL, real );
  for ( i = 0; i < n; ++i ) {
    real* pt = VECTOR_PUSH_BACK( knn, 1, vv1 + 2 * i, 2 * sizeof( real ), 2 * sizeof( real ) );
    int* id = ( int* ) ( pt + 2 );
    *id = 1;
  }
  
  {
    NEAREST nearests[ 10 ];
    CvScalar col;
    int len = 2;
    for ( i = 0; i < img_height; i++ ) {
      for ( j = 0; j < img_width; j++ ) {
        real sample[ 2 ] = {( real ) i, ( real ) j};
        int score[ 2 ] = {0};
        find_nearest( knn, sample, nearests, 10, point_distor, &len );
        //find_nearest(knn, sample, nearests, 10, chi_square, &len);
        
        for ( k = 0; k < 10; k++ ) {
          int id = *( int* ) ( nearests[ k ].data + 2 ) == 1;
          score[ id ] += 1;
        }
        //printf("%3d  %3d\n", score[0], score[1]);
        col = score[ 0 ] > score[ 1 ] ? CV_RGB( 0, 180, 0 ) : CV_RGB( 0, 180, 0 );
        //(accuracy > 5 ? CV_RGB(180,0,0) : CV_RGB(180,120,0)) :
        //(accuracy > 5 ? CV_RGB(0,180,0) : CV_RGB(120,120,0));
        cvSet2D( img + i * img_step + j * img_cn, img_cn, col, unsigned char );
        cvSet2D( img + i * img_step + j * img_cn, img_cn, colors_[ score[ 0 ] ], unsigned char );
      }
    }
  }
  
  {
    // display the original training samples
    for ( i = 0; i < 50; i++ ) {
      CvPoint pt;
      pt.x = cvRound( vv0[ i * 2 ] );
      pt.y = cvRound( vv0[ i * 2 + 1 ] );
      DrawCircle( img_height, img_width, img, img_step, img_cn, pt, 2, CV_RGB( 255, 0, 0 ), -1, 0, 0 );
      pt.x = cvRound( vv1[ i * 2 ] );
      pt.y = cvRound( vv1[ i * 2 + 1 ] );
      DrawCircle( img_height, img_width, img, img_step, img_cn, pt, 2, CV_RGB( 0, 255, 0 ), -1, 0, 0 );
    }
    cvNamedWindow( "classifier result", 1 );
    cvShowImage( "classifier result", img_height, img_width, img, img_step, img_cn );
    cvWaitKey( 0 );
  }
  VECTOR_DEL( knn );
  return 0;
}

int test_Clustering() {
  DumpClusterData( stdout, 0, 0, 0, 0 );
  return 0;
}

int main( int argc, char** argv ) {
  test_KNEAREST();
  return 0;
}
