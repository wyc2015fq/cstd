
#include <stdio.h>
#include "chalg.h"
#include "faceTracking.h"
#include "cap.h"
#include "chalg.h"
#include "imcolor.h"
#include "mathfuncs.h"
#include "macro.h"
#include "macro.h"
#include "xtypes.h"
#include "window.h"
#include "tables.h"
#include "utime.h"

#define HISTSIZE  (256*3)

int cvCamShift2( int height, int width, const unsigned char* img, int step, int cn, IRECT windowIn,
                 int max_iter, int eps, CvConnectedComp* _comp, CvBox2D* box );

//#define _DBG_CAMTRACK

#ifdef _DBG_CAMTRACK
#define BEGIN_DBG if (1)
#else
#define BEGIN_DBG while (0)
#endif

#define MALLOC2D(TYPE, h, w, outptr)                                          \
do {                                                                          \
  int sz0 = (h)*sizeof(TYPE*);                                                \
  int sz1 = (h)*(w);                                                          \
  TYPE** ptr = (TYPE**)pmalloc(sz0+sz1*sizeof(TYPE));                          \
  TYPE* ptr1 = (TYPE*)(((char*)ptr) + sz0);                                   \
  TYPE* ptr2 = ptr1 + sz1;                                                    \
  outptr = ptr;                                                               \
  for (; ptr1!=ptr2; ptr1+=w, ++ptr) {                                        \
    *ptr = ptr1;                                                              \
  }                                                                           \
} while(0)

int rect_partition( PXRECT B0, PXRECT B, REAL_TYPE thd, int mincnt, int sort, void* buf, int* pbuflen );

PTRACKER new_tracker( size_t maxcount ) {
  PTRACKER tk = MALLOC( TRACKER, 1 );
  if ( tk ) {
    MALLOC2D( real, maxcount, HISTSIZE, tk->phist );
    tk->ptrack = MALLOC( XRECT, maxcount );
    tk->count = 0;
    tk->maxcount = maxcount;
    tk->baseid = 0;
    tk->zoom = 1.5f;
    tk->mindist = 1700.f;
    tk->minarea = 1.f;
    tk->ssmin = 2.f;
    tk->ss = 1.1f;
    tk->stepxy = -1;
    tk->thd = 0.8f;
    tk->mincnt = 2;
  }
  return tk;
}

void free_tracker( PTRACKER tk ) {
  pfree( tk->phist );
  pfree( tk->ptrack );
  pfree( tk );
}

int track_xrect( int h, int w, unsigned char* img, int al, int ai, const unsigned char* pro, int bl, int bi, const void* cas,
                 XRECT rt, XRECT* out_rt, real ss, real minarea, int* update ) {
  XRECT tmp[ 100 ] = {0};
  IRECT crt = {rt.x, rt.y, rt.cx, rt.cy};
  CvScalar colors = {255, 255, 255};
  int n, pos = 0;
  int mincx = ( int ) ( rt.cx );
  int maxcx = ( int ) ( rt.cx * ss );
  int zz = maxcx - rt.cx;
  if ( out_rt ) {
    *out_rt = rt;
    n = 0;
#if 0

    if ( cas ) {
      *update = 1;
      XRECT_ZOOM( rt, zz );
      XRECT_BOUND( rt, h, w );
      pos = rt.y * al + rt.x * ai;
      {
        unsigned char* gray = MALLOC( unsigned char, rt.cy * rt.cx );
        IM2IM( h, w, img + pos, al, ai, gray, w, 1 );
        n = lut_detect( cas, rt.cy, rt.cx, gray, rt.cx, 0, 0,
                        2.f, 100.f, 1.1f, -1, 0.8f, 2, face, 100 );
        SAFEFREE( gray );
      }
      tmp[ 0 ].x += rt.x;
      tmp[ 0 ].y += rt.y;
      tmp[ 0 ].count = -tmp[ 0 ].count;
    }
#endif
    if ( !n ) {
      real areaavg;
      CvConnectedComp comp = {0};
      CvBox2D box;
      int hh = h < ( rt.y + rt.cy + 5 ) ? h : ( rt.y + rt.cy + 5 );
      cvCamShift2( hh, w, pro, bl, bi, crt, 20, 1, &comp, &box );
      XRECT_BOUND( comp.rect, h, w );
      tmp[ 0 ].cx = crt.cx;
      tmp[ 0 ].cy = tmp[ 0 ].cx;
      tmp[ 0 ].x = ( int ) box.center.x - tmp[ 0 ].cx / 2;
      tmp[ 0 ].y = ( int ) box.center.y - tmp[ 0 ].cx / 2;
      //XRECT_COPY(comp.rect, tmp[0]);
      areaavg = ( ( real ) comp.area / ( crt.cx * crt.cy ) );
      areaavg /= 100.f;
      n = ( areaavg > minarea ) && ( areaavg < 10.f );
      printf( " track %f ", areaavg );
      colors = colors_[ 0 ];
      *update = 0;
      //n = 1;
    }
    if ( n ) {
      tmp[ 0 ].id = ( *out_rt ).id;
      *out_rt = tmp[ 0 ];
      BEGIN_DBG
      {
        XRECT are = *out_rt;
        XRECT_ZOOM( are, 10 );
        DrawRectangle2( h, w, img, al, ai, are, colors, 3 );
      }
    }
  }
  return n;
}

#if 0
#define OPINRANGE(A)  INRANGE(*(A), 0, 180) && INRANGE(*(A+1), 30, 256) && INRANGE(*(A+2), 10, 256)
#else
#define OPINRANGE(A)  1
#endif

int get_hist( int h, int w, const unsigned char* hsv, int al, int ai, XRECT rt_in, real ss, real* hist ) {
  int hist_in[ 256 * 3 ];
  int hist_ou[ 256 * 3 ];
  XRECT rt_ou = rt_in;
  int zz, i, pos_in, pos_ou, thd;
  thd = rt_in.cx * rt_in.cy / 150;
  zz = ( int ) ( rt_in.cx * ( ss - 1. ) );
  //zz = 50;
  XRECT_ZOOM( rt_ou, zz );
  rt_ou.y = rt_in.y;
  rt_ou.cy = rt_in.cy;
  XRECT_BOUND( rt_ou, h, w );
  pos_in = rt_in.y * al + rt_in.x * ai;
  pos_ou = rt_ou.y * al + rt_ou.x * ai;
  // IMTRANS(h, w, img, al, ai, mask, w, 1, OPINRANGE);
  memset( hist_in, 0, 256 * 3 * sizeof( int ) );
  memset( hist_ou, 0, 256 * 3 * sizeof( int ) );
  for ( i = 0; i < ai; ++i ) {
    int* hist_in_i = hist_in + i * 256;
    int* hist_ou_i = hist_ou + i * 256;
#if 1

    SKIP_HIST( rt_in.cy, rt_in.cx, ( hsv + pos_in + i ), al, ai, hist_in_i );
    SKIP_HIST( rt_ou.cy, rt_ou.cx, ( hsv + pos_ou + i ), al, ai, hist_ou_i );
#else

    SKIP_HIST_IF( rt_in.cy, rt_in.cx, ( hsv + pos_in + i ), al, ai, hist_in_i, OPINRANGE );
    SKIP_HIST_IF( rt_ou.cy, rt_ou.cx, ( hsv + pos_ou + i ), al, ai, hist_ou_i, OPINRANGE );
#endif

  }
  zz /= 10;
#if 0

  {
    int i;
    FILE* pf = fopen( "\\\\chsj019\\xzh\\1_hist_in.txt", "w" );
    if ( pf ) {
      for ( i = 0; i < 256*3; ++i ) {
        fprintf( pf, "%6d, \n", hist_in[ i ] );
      }
      fclose( pf );
    }
  }
#endif
  for ( i = 0; i < 256*3; ++i ) {
    if ( hist_ou[ i ] > thd ) {
      real aa = ( real ) ( hist_ou[ i ] - hist_in[ i ] );
      aa = ( hist_in[ i ] ) / ( aa + 0.0000001f );
      aa = aa > 100.f ? 100.f : aa;
      hist[ i ] = aa < 1. ? 0 : aa;
    } else {
      hist[ i ] = 0.f;
    }
  }
  return 0;
}

int back_proj( int h, int w, const unsigned char* hsv, int al, int ai, unsigned char* pro, int bl, int bi, real* hist ) {
  int a1, a0, b1, b0;
  real* hist1 = hist + 1 * 256;
  real* hist2 = hist + 2 * 256;
  for ( a1 = 0, b1 = 0; a1 < h*al; a1 += al, b1 += bl ) {
    for ( a0 = 0, b0 = 0; a0 < w*ai; a0 += ai, b0 += bi ) {
      int idx = a0 + a1;
      //if (OPINRANGE((hsv+idx))) {
      real zz = hist[ *( hsv + idx ) ];
      zz *= hist1[ *( hsv + idx + 1 ) ];
      zz *= hist2[ *( hsv + idx + 2 ) ];
      // zz = BOUND(0, zz, 255);
      *( pro + b1 + b0 ) = ( unsigned char ) ( zz > 255 ? 255 : zz );
      //} else { *(pro+b1+b0) = 0; }
    }
  }
  return 0;
}

int merg_face_to_out( int outnumber, PXRECT out, int facenumber, PXRECT face, int baseid, real mindist ) {
  int i, j;
  int fn = facenumber;
  for ( i = 0; i < outnumber; ++i ) {
    real mindis = mindist + 1.f;
    int jj = -1;
    for ( j = 0; j < facenumber; ++j ) {
      int iseq = 0;
#define IS_RECT_EQUAL_3(r1, r2, OUT)  IS_RECT_EQUAL_COVER_RETE(r1, r2, 0.1, OUT)

      IS_RECT_EQUAL_3( out[ i ], face[ j ], iseq );
#undef IS_RECT_EQUAL_3

      if ( iseq ) {
        jj = j;
        break;
      }
    }
    if ( jj >= 0 ) {
      //out[i].id = jj;
      face[ j ].x = out[ i ].x;
      face[ j ].cx = out[ i ].cx;
      face[ j ].y = out[ i ].y;
      face[ j ].cy = out[ i ].cy;
      //face[j].count = -face[j].count;
    } else {
      out[ i ].id = baseid;
      face[ fn ] = out[ i ];
      face[ fn ].count = -face[ fn ].count;
      ++fn;
      ++baseid;
    }
  }
  return fn;
}

void image_nom( int _h1, int _w, unsigned char* _A, int _al, int _ai, int _val ) {
  int _max = 0;
  int _v = _val << 14;
  SKIP_MAX_ELEMENT( _h1, _w, _A, _al, _ai, _max );
  _v /= *( _A + _max ) + 1;
#define _SHIFT_MUL(_X, _Y) ((_X) = ((_X) * (_Y))>>14)

  SKIP_OPT_VX2( _h1, _w, _A, _al, _ai, _v, _SHIFT_MUL );
#undef _SHIFT_MUL
}

int camtrack( int h, int w, unsigned char* img, int al, int ai,
              unsigned char* mask, int mask_step, PTRACKER tk,
              const void* cas, int detectface ) {
  unsigned char * hsv = MALLOC( unsigned char, h * al );
  unsigned char* pro = MALLOC( unsigned char, h * w );
  unsigned char* himg = MALLOC( unsigned char, 512 * 300 * 3 );
  int i, j;
  int facenumber = tk->count;
  int maxcount = tk->maxcount;
  int baseid = tk->baseid;
  PXRECT face = tk->ptrack;
  real** hist = tk->phist;
  real mindist = tk->mindist;
  real minarea = tk->minarea;
  int updatehist = 0;
  utime_start( s );
  utime_restart( s );
  icvBGRx2HSV_8u_CnC3R( h, w, img, al, ai, hsv, al, 0 );
  image_nom( h, w, hsv, al, ai, 255 );
  image_nom( h, w, ( hsv + 1 ), al, ai, 255 );
  image_nom( h, w, ( hsv + 2 ), al, ai, 255 );
  //printf("%f\t ", utime_elapsed(s));

  BEGIN_DBG
  {
    static int NamedWindow = 0;
    if ( !NamedWindow ) {
    NamedWindow = 1;
    cvNamedWindow( "hsv", 1 );
      cvNamedWindow( "hist", 1 );
      cvNamedWindow( "pro", 1 );
    }
  }

  for ( i = 0, j = 0; i < facenumber; ++i ) {
    int ok;
    real ss = 2.5f;
    int update = 0;
    memset( pro, 0, h * w );
    {
      XRECT are = face[ i ];
      are.x -= 50;
      are.cx += 100;
      are.y -= 20;
      are.cy += 40;
      XRECT_BOUND( are, h, w );
      back_proj( are.cy, are.cx, hsv + are.y * al + are.x * ai, al, ai,
                 pro + are.y * w + are.x, w, 1, hist[ i ] );
    }
    ok = track_xrect( h, w, img, al, ai, pro, w, 1,
                      cas, face[ i ], face + j, ss, minarea, &update );
    if ( ok ) {
      XRECT_BOUND( face[ j ], h, w );
      if ( update ) {
        XRECT are = face[ j ];
        //int kk = (int)(0.2*are.cx);
        //XRECT_ZOOM(are, -kk);
        get_hist( h, w, hsv, al, ai, are, tk->zoom, hist[ j ] );
      }
      ++j;
    }
  }
  facenumber = tk->count = j;
  if ( facenumber ) {
    //back_proj(h, w, hsv, al, ai, pro, w, 1, hist[0]);
  }
  if ( detectface ) {
    PXRECT out = face + facenumber;
    PXRECT out_end = face + maxcount;
    int outnumber;
    {
      unsigned char* gray = MALLOC( unsigned char, w * h );
      IM2IM( h, w, img, al, ai, gray, w, 1 );
      outnumber = lut_detect( cas, h, w, gray, w, mask, mask_step,
                              tk->ssmin, 100.f, tk->ss, tk->stepxy,
                              tk->thd, tk->mincnt, out, out_end - out );
      SAFEFREE( gray );
    }
    BEGIN_DBG
    {
      for ( i = 0; i < outnumber; ++i ) {
      XRECT are = out[ i ];
        XRECT_ZOOM( are, 10 );
        DrawRectangle2( h, w, img, al, ai, are, CC_RGB( 255, 255, 255 ), 3 );
      }
    }
    if ( outnumber ) {
      int fn = merg_face_to_out( outnumber, out, facenumber, face, baseid, mindist );
      tk->count = fn;
      tk->baseid = face[ fn - 1 ].id + 1;
      updatehist = 1;
    }
  }
  if ( updatehist ) {
    for ( i = 0; i < ( int ) tk->count; ++i ) {
      if ( face[ i ].count < 0 ) {
        face[ i ].count = -face[ i ].count;
        //printf(" u%d ", i);
        XRECT_BOUND( face[ i ], h, w );
        get_hist( h, w, hsv, al, ai, face[ i ], tk->zoom, hist[ i ] );
      }
    }
  }
  BEGIN_DBG
  {
    if ( tk->count ) {
    int ihist[ 256 * 3 ];
      VEC_COPY( 256 * 3, hist[ 0 ], ihist );
      DrawHistHsv( 300, 512, himg, 512 * 3, 3, ihist, 256 );
      cvShowImage( "hist", 300, 512, himg, 512 * 3, 3 );
    }
    cvShowImage( "hsv", h, w, hsv, al, ai );
    cvShowImage( "pro", h, w, pro, w, 1 );
  }
  pfree( hsv );
  pfree( pro );
  pfree( himg );

  {
    //PXRECT B = face + tk->count;
    //#define IS_RECT_EQUAL_3(r1, r2, OUT)  IS_RECT_EQUAL_COVER_RETE(r1, r2, 0.3, OUT)
    //RECT_PARTITION(PXRECT, face, B, IS_RECT_EQUAL_3, 0, XRECT, XRECT_ADD, XRECT_DIV, B);
    //#undef IS_RECT_EQUAL_3
    //B = rect_partition(face, B, 0.3, 0, 0);
    //tk->count = B - face;
  }
  return 0;
}
