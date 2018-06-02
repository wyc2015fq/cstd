#include "_faceDetect.h"

int lut_detect_2( const void* alg ) {
  detect_info_t * p = ( detect_info_t* ) alg;
  const HAARCASCADE** ca = (const HAARCASCADE**)(p->cas);
  XRECT* B = p->out;
  //const int is_nesting = ( 0 == ( ca->ih[ ca->stage[ 0 ].len ].ft0.w ) );
  iHaarWork ** pWirk = p->pWirk;
  FIX_FLOAT * inter = p->inter;
  FIX_FLOAT * inter1 = p->inter1;
  int height = p->h;
  int width = p->w;
  int stepxy = p->stepxy;
  int mincnt = p->mincnt;
  real ssmin = p->ssmin;
  real ssmax = p->ssmax;
  real ss = p->ss;
  real thd = p->thd;
  int datastep = p->w;
  int markstep = p->w;
  const unsigned char* data = p->tmpimg0;
  const unsigned char* mark = p->tmpmark0;
  int maxoutlen = p->maxoutlen;
  int i, len = 0, cx = ca[0]->w, cy = ca[0]->h, cx0 = ca[0]->w, cy0 = ca[0]->h;
  
  //const int is_nesting = (0==(ca->ih[ca->stage[0].len].ft0.w));
  int stp = ( width + 1 );
  //int all_times = 0, any_times = 0;
  real cx_cy = ( real ) cx0 / cy0;
  setoffset_fun_type setoffset[MAX_CAS_NUM];
  boostpass_fun_type boostpass[MAX_CAS_NUM];
  
  //boostpass_fun_type boostpass_fun;
  
  cvIntegralImage_C1R( height, width, data, datastep, inter, stp, inter1, stp, 0, 0 );
  //memcpy( iHaarBuf, ca->ih, sizeof( iHaarBuf[ 0 ] ) * MIN( HAARBUFLEN, ca->ihlen ) );
  cy = XFLOOR( cy0 * ssmin );
  cx = XFLOOR( cy * cx_cy );
  
  for (i=0; i<p->casnum; ++i) {
#ifdef WIN32
    //p->B = MALLOC( XRECT, maxoutlen );
    if ( CAISHAVEFLOAT( ca[i] ) ) {
      boostpass[i] = ( boostpass_fun_type ) boostpass_nesting_fun;
      setoffset[i] = ( setoffset_fun_type ) setoffset1;
      //printf("ASDFASDF\n");
    } else {
#ifdef USE_INT_FUN2
      boostpass[i] = ( boostpass_fun_type ) boostpass_nesting_int_fun2;
      setoffset[i] = ( setoffset_fun_type ) setoffset2;
#else
      boostpass[i] = ( boostpass_fun_type ) boostpass_nesting_int_fun3;
      setoffset[i] = ( setoffset_fun_type ) setoffset3;
#endif // USE_INT_FUN2
    }
#endif // WIN32
  }
  
  //#define BIGRECT_TO_SMALLRECT1
#ifdef BIGRECT_TO_SMALLRECT1
  
  for
  (
    ;
    ssmin < ssmax && width >= cx && height >= cy;
    ssmin *= ss, cy = XFLOOR( cy0 * ssmin ), cx = XFLOOR( cy * cx_cy )
) {}
  ss = 1.00001f / ss;
  ssmin *= ss, cy =
             XFLOOR( cy0 * ssmin ),
             cx = XFLOOR( cy * cx_cy );
  for
  (
    len = 0;
    len < maxoutlen && ssmin >= ssmin0;
    ssmin *= ss, cy = XFLOOR( cy0 * ssmin ), cx = XFLOOR( cy * cx_cy )
  ) {
#else
  for
  (
    len = 0;
    len < maxoutlen && ssmin < ssmax && width >= cx && height >= cy; ssmin *= ss, cy = XFLOOR
        ( cy0 * ssmin ), cx = XFLOOR( cy * cx_cy )
  ) {
#endif
    int x, y, ranx = width - cx, rany = height - cy, stepxy1;
    real invarea = 1.f / ( cx * cy );
    
    //if ( cx == 177 && cy == 177 ) {
    //  int asdf = 0;
    //}
    for (i=0; i<p->casnum; ++i) {
      setoffset[i]( ca[i]->ihlen, ca[i]->ih, ( iHaarWork * ) (pWirk[i]), stp, 1, ( real ) cy / cy0, invarea * cx0 * cy0 );
    }
    stepxy1 = stepxy > 0 ? stepxy : XFLOOR( ssmin );
    for ( y = 0; len < maxoutlen && y < rany; y += stepxy1 ) {
      for ( x = 0; len < maxoutlen && x < ranx; x += stepxy1 ) {
        //x = 162, y=178;
        if ( !mark || mark[ y * markstep + x ] ) {
          FIX_FLOAT * tmpSamp = inter + y * stp + x;
          FIX_FLOAT * tmpSamp1 = inter1 + y * stp + x;
          int _AL = cx, _AI = cy * stp;
          int ispassed = 0;
          real score, var;
#if 0
          
          var =
            RECT_VAL2( tmpSamp, ( _AL ), ( _AI ) )
            * invarea;
          var =
            RECT_VAL2( tmpSamp1, ( _AL ), ( _AI ) )
            * invarea - var * var;
#else
            
          var = get_invvar( tmpSamp, tmpSamp1, _AL, _AI, invarea );
#endif
          //printf("%f\n", score);
          //if ( x == 122 && y == 148 && cx == 177 && cy == 177 ) {
          //  int asdf = 0;
          //}
          for (i=0; i<p->casnum; ++i) {
            ispassed = boostpass[i]( tmpSamp, ca[i]->ih, ca[i]->stage, ca[i]->stagelen, pWirk[i], var, stp, &score );
            if (!ispassed) {break;}
          }
          if ( ispassed ) {
            MAKE_XRECT( x, y, cx, cy, B[ len ] );
            B[ len ].score = score;
            B[ len ].count = 1;
            
            //if (cx!=cy) {
            //  printf("(%3d, %3d, %3d, %3d) = %f (%d)\n", B[len].x,
            //    B[len].y, B[len].w, B[len].h, B[len].score, B[len].count);
            //}
            ++len;
          }
          
          //if (20==cy0)
          //++any_times;
        }
      }
    }
  }
  
#if 0
  
  if ( B[ 0 ].w != B[ 0 ].h ) {
    int i;
    for ( i = 0; i < len; ++i ) {
      DrawRectangle2( height, width, data, datastep, 1, B[ i ], CC_RGB( 255, 0, 0 ), 1 );
      printf
      (
        "(%3d, %3d, %3d, %3d) = %f (%d)\n", B[ i ].x, B[ i ].y, B[ i ].w, B[ i ].h, B[ i ].score, B[ i ].
        count
      );
    }
    imwrite( "eye.bmp", height, width, data, datastep, 1 );
  }
#endif
  if ( thd > 0.001 && len > mincnt ) {
	  int buflen=0;
    len = xrect_partition( B, B + len, thd, mincnt, 1, p->rect_partition_buf, &buflen );
  }
  
  //if (20==cy0)
  //  printf("%d\n", any_times);
  return len;
}

