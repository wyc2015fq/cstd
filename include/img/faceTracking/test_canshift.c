
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "chalg.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "window.h"
#include "chalg.h"
#include "faceTracking.h"
#include "cap.h"
#include "chalg.h"
#include "imcolor.h"
#include "filters.h"
#include "utime.h"

  void image_nom( int _h1, int _w, unsigned char * _A, int _al, int _ai, int _val );
  int get_hist( int h, int w, const unsigned char * hsv, int al, int ai, XRECT rt_in, float ss, float * hist );
  int back_proj( int h, int w, const unsigned char * hsv, int al, int ai, unsigned char * pro, int bl, int bi, float * hist );
  
#ifdef __cplusplus
}
#endif

int facebaseid = 0;
const double mindist = 50.;
int hdims = 256, histimg_width = 512, histimg_height = 300;

int test_canshift( int argc, char** argv );
int test_hsv( int argc, char** argv );

int main( int argc, char** argv ) {
  test_canshift( argc, argv );
  return 0;
}

#define IMAGE_NOM(_h, _w, _A, _al, _ai, _val)                                 \
do {                                                                          \
  int _max = 0;                                                               \
  float _v = _val+0.1f;                                                       \
  SKIP_MAX_ELEMENT(_h, _w, _A, _al, _ai, _max);                               \
  _v /= *(_A + _max)+0.1f;                                                    \
  SKIP_DIV_VX2(_h, _w, _A, _al, _ai, _v);                                     \
} while(0)

int test_hsv( int argc, char** argv ) {
#ifdef _IMGIO_HPP_
  int h, w, al, ai;
  float hist[ 256 * 3 ];
  XRECT rt_in = {
#include "\\chsj019\xzh\yiwei.txt"
                };
  float ss = 1. + ( 40. / rt_in.cx );
  unsigned char* hsv = NULL;
  unsigned char* pro = NULL;
  unsigned char* ppp = NULL;
  {
    struct fmtimgreader* reader = new_reader( "\\\\chsj019\\xzh\\1.bmp" );
    h = reader->m_height;
    w = reader->m_width;
    ai = 3;
    al = w * ai;
    if ( ppp ) {
      pfree( ppp );
      pfree( hsv );
    }
    ppp = MALLOC( unsigned char, h * al );
    hsv = MALLOC( unsigned char, h * al );
    imread( reader, ppp, al, 0 );
    icvBGRx2HSV_8u_CnC3R( h, w, ppp, al, ai, hsv, al, 0 );
    image_nom( h, w, hsv, al, ai, 255 );
    image_nom( h, w, ( hsv + 1 ), al, ai, 255 );
    image_nom( h, w, ( hsv + 2 ), al, ai, 255 );
    //IMTRANS(h, w, ppp, al, ai, hsv, al, ai, RGB2HSV);
    imwrite( "\\\\chsj019\\xzh\\1_hsv.bmp", h, w, hsv, al, ai );
  }
  
  get_hist( h, w, hsv, al, ai, rt_in, ss, hist );
  {
    int i;
    FILE* pf = fopen( "\\\\chsj019\\xzh\\1_hist.txt", "w" );
    if ( pf ) {
      for ( i = 0; i < 256*3; ++i ) {
        fprintf( pf, "%6f, \n", hist[ i ] );
      }
      fclose( pf );
    }
  }
  {
    int bl = w, bi = 1;
    pro = MALLOC( unsigned char, h * w );
    back_proj( h, w, hsv, al, ai, pro, bl, bi, hist );
    imwrite( "\\\\chsj019\\xzh\\1_pro.bmp", h, w, pro, bl, bi );
  }
  pfree( pro );
  pfree( hsv );
#endif
  
  return 0;
}

int test_canshift( int argc, char** argv ) {
  capcam * cap = 0;
  unsigned char* histimg;
  unsigned char* rgb = 0;
  unsigned char* gray = 0;
  unsigned char* gray2 = 0;
  int isspeye[ 5 ] = {0};
  char buf[ 256 ];
  int zoom = 0;
  int minarea = 5;
  int low = 30, high = 50;
  int isdrawrect = 0;
  CvFont font;
  cvInitFont( &font, CC_FONT_HERSHEY_SIMPLEX, 1.0f, 1.0f, 0, 3, 0 );
  cap = cam_start( 0 );
  cvNamedWindow( "cam", 1 );
  cvCreateTrackbar( "zoom", "cam", &zoom, 1000, 0 );
  cvCreateTrackbar( "Æ½¾ùÃÜ¶È", "cam", &minarea, 100, 0 );
  cvCreateTrackbar( "low", "cam", &low, 300, 0 );
  cvCreateTrackbar( "high", "cam", &high, 300, 0 );
  histimg = MALLOC( unsigned char, ( histimg_width * histimg_height * 3 ) );
  if ( cap ) {
    PTRACKER tk = new_tracker( 100 );
    int i, fram = 0;
    char ch = 0;
    int outnumber;
    PXRECT out = tk->ptrack;
    for ( ; 'q' != ch && 'Q' != ch && cam_GrabFrame( cap ); ++fram ) {
      unsigned char* img = cam_begin( cap );
      int nn = 5;
      int dodetect = ( fram % nn ) == 3;
      utime_start( s );
      int h = cam_height( cap ), w = cam_width( cap ), al = cam_step( cap ), ai = cam_channel( cap );
      if ( !gray ) {
        gray = MALLOC( unsigned char, h * w );
        gray2 = MALLOC( unsigned char, h * w );
      }
      if ( 3 != ai ) {
        //printf("channle mast be 3!");
        //break;
        if ( !rgb ) {
          rgb = MALLOC( unsigned char, h * w * 3 );
        }
        //utime_restart(s);
        icvYUV4222BGR_8u_C2C3R( h, w, img, al, rgb, w * 3, 3 );
        //printf("%f\n", utime_elapsed(s));
        img = rgb;
        al = w * 3;
        ai = 3;
      }
      {
        IM2IM( h, w, img, al, ai, gray, w, 1 );
        //utime_restart(s);
        //cvSobel(h, w, gray, w, 1, gray2, w, 1, 0, 1, unsigned char );
        //cvCanny(h, w, gray, w, 1, gray2, w, 1, low, high);
        //printf("%f\n", utime_elapsed(s));
      }
#if 1
      tk->minarea = minarea / 100.f;
      _snprintf( buf, 256, "pic\\%05d.bmp", fram );
      
      utime_restart( s );
      camtrack( h, w, img, al, ai, 0, 0, tk, CPM_GetCascade( VIS_FACE_CASCADE ), 1 );
      printf( "%f\n", utime_elapsed( s ) );
      outnumber = tk->count;
      for ( i = 0; i < outnumber; ++i ) {
        char buf[ 20 ];
        int colidx = out[ i ].id % ( sizeof( colors_ ) / sizeof( CvScalar ) );
        CvPoint pt1 = {out[ i ].x, out[ i ].y};
        _snprintf( buf, 20, "%d", out[ i ].id );
        if ( isdrawrect ) {
          DrawRectangle2( h, w, img, al, ai, out[ i ], colors_[ colidx ], 3 );
          printf( "asdfasdf\n" );
        }
        cvPutText( h, w, img, al, ai, buf, pt1, &font, colors_[ colidx ] );
      }
      //printf("%f\t ", utime_elapsed(s));
#else
      //utime_restart(s);
      {
        outnumber = CPM_FaceDetect( CPM_GetCascade( VIS_FACE_CASCADE ), h, w, gray, w, 0, 0,
                                    0.5, 2.f, 100.f, 1.25f, -1, 0.8f, 2, out, 1000 );
        printf( "%d %f\n", outnumber, utime_elapsed( s ) );
      }
#endif
      
#if 0
      
      if ( 0 ) {
        // int x = w/2, y=h/2;
        int outi;
        for ( outi = 0; outi < outnumber; ++outi ) {
          XRECT eyes[ 100 ];
          int eyes_cnt = 0, i;
          eyes_cnt = CPM_EyesDetect( 0, CPM_GetCascade( VIS_EYE_CASCADE ), CPM_GetCascade( VIS_EYE_CASCADE ),
                                     img, al, ai, 0, 0, 5, out + outi, eyes, eyes + 100 ) - eyes;
          for ( i = 1; i < 3; ++i ) {
            int xx = eyes[ i ].x + eyes[ i ].cx / 2;
            int yy = eyes[ i ].y + eyes[ i ].cy / 2;
            if ( xx && isspeye[ i ] ) {
              if ( isdrawrect )
                DrawRectangle2( h, w, img, al, ai, eyes[ i ], colors_[ 0 ], 3 );
              Spherize_trans( h, w, ( unsigned char* ) img, al, ai, xx, yy, eyes[ i ].cx, 0.5 );
            }
          }
          //ArchimedeSpiral ar( w,h,x,y );
          //Mosaic mo( w,h,x,y,200 );
          //Spherize sp( w,h,x,y );
          //UnSpherize us( w,h,x,y );
          //sp.trans( w/2,h/2,( unsigned char* )img, al,ai,( unsigned char* )img,al,ai );
          if ( isspeye[ 3 ] ) {
            int xx = out[ outi ].x + out[ outi ].cx / 2;
            int yy = out[ outi ].y + out[ outi ].cy / 2;
            Spherize_trans( h, w, ( unsigned char* ) img, al, ai, xx, yy, out[ outi ].cx, 0.5 );
          }
        }
      }
#endif
      cvShowImage( "cam", h, w, img, al, ai );
      //cvShowImage("pro", h, w, gray2, w, 1);
      //SKIP_COPY(h, w, hsv, al, ai, pro, w, 1);
      //cvShowImage("pro", h, w, pro, w, 1);
      //cvShowImage(", """, histimg_height, histimg_width, histimg, histimg_width*3, 3);
      ch = cvWaitKey( 10 );
      if ( 'd' == ch || 'D' == ch )
        isdrawrect ^= 1;
      if ( '1' == ch )
        isspeye[ 1 ] ^= 1;
      if ( '2' == ch )
        isspeye[ 2 ] ^= 1;
      if ( '3' == ch )
        isspeye[ 3 ] ^= 1;
    }
    free_tracker( tk );
    if ( gray ) {
      pfree( gray );
      pfree( gray2 );
    }
    if ( rgb ) {
      pfree( rgb );
    }
    pfree( histimg );
  }
  return 0;
}
