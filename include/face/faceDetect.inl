#ifndef _FACEDETECT_INL_
#define _FACEDETECT_INL_

#include "_faceDetect.h"
#include "init.inl"

#undef RECT_VAL2
#undef HAAR_FEAT_VAL
#undef LUT_VAL
#undef RECT_VAL2
// 求 haar 特征
#define RECT_VAL(A, p0, p1, p2, p3) (*(A + (p0)) - *(A + (p1)) - *(A + (p2)) + *(A + (p3)))
#define RECT_VAL2(_A, _a, _b)       (*(_A) - *(_A + (_a)) - *(_A + (_b)) + *(_A + (_a) + (_b)))
#define HAAR_RECT_VAL(A, rect)      (RECT_VAL(A, (rect).p0, (rect).p1, (rect).p2, (rect).p3) * (rect).dd)
#define HAAR_RECT_VAL3(A, rect)     (RECT_VAL(A, (rect).p0, (rect).p0+(rect).w, (rect).p0+(rect).h, (rect).p0+(rect).w+(rect).h) * (rect).dd)
#define HAAR_FEAT_VAL(A, vv, feat)  (vv * (HAAR_RECT_VAL(A, feat->ft0) + HAAR_RECT_VAL(A, feat->ft1)))
#define HAAR_FEAT_VAL3(A, vv, feat) (vv * (HAAR_RECT_VAL3(A, feat->ft0) + HAAR_RECT_VAL3(A, feat->ft1)))
#define HAAR_FEAT_VAL2(A, vv, feat) (vv * ((HAAR_RECT_VAL(A, feat->ft0) + HAAR_RECT_VAL(A, feat->ft1)) >> SHIFT))
#define LUT_BIN_1                   (31)
#define _SHIFT_2                     (_SHIFT * 2)
// LUT adaboost
#define LUT_VAL(x, lut)   ((lut)->val[(int) (((x) - (lut)->minval) * (lut)->binval) & (LUT_BIN_1)])
#define LUT_VAL2(x, lut)  ((lut)->val[(((((x)) - (lut)->minval) * (lut)->binval) >> (SHIFT_2)) & (LUT_BIN_1)]) 

#ifndef HAAR_CN
#define HAAR_CN (2)
#endif // HAAR_CN
#if 1
float invsqrtf( float x ) {
  float xhalf = 0.5f * x;
  suf32_t i;
  i.f = x;
  i.i = 0x5f3759df - (i.i >> 1);// 计算第一个近似根
  x = i.f;
  x = x * (1.5f - xhalf * x * x); // 牛顿迭代法
  return x;
}

int invsqrti( int n ) {
  int next_x, x;
  n = _SHIFT1 * _SHIFT1 / n;
  for ( x = n; x > ( next_x = n / x ); ) {
    x = ( x + next_x ) >> 1;
  }
  
  return x;
}

unsigned int InvSqrtInt( unsigned int d ) {
  unsigned int t, q, r, N;
  d = ( _SHIFT1 * _SHIFT1 ) / d;
  q = 0;
  r = d;
  N = 13;
  do {
    N--;
    t = 2 * q + ( 1 << N );
    if ( ( r >> N ) >= t ) {
      r -= ( t << N );
      q += ( 1 << N );
    }
  } while ( N );
  return q;
}

//#define INVSQRT(x) ( x > 0 ) ? invsqrti(x) : _SHIFT1
#ifdef USE_FIX_FLOAT  // 使用定点数
#define INVSQRT(x)  (x > 0) ? InvSqrtInt(x) : _SHIFT1
#else
#define INVSQRT(x)  (x > 0.f) ? sqrt(1.f / (x)) : 1.f 
//#define INVSQRT(x)  (x > 0.f) ? invsqrtf(x) : 1.f
#endif

//#define INVSQRT(x) ( x > 0 ) ? XFLOOR(invsqrtf(x) * _SHIFT1) : _SHIFT1
#else
#ifdef __PC
#define INVSQRT(x)  (x > 0) ? XFLOOR(sqrt(1. / (x)) * _SHIFT1) : _SHIFT1
#else
#define INVSQRT(x)  (x > 0) ? XFLOOR(sqrtf(1.f / (x)) * _SHIFT1) : _SHIFT1
#endif
#endif

#if 1
//#pragma DATA_SECTION(invvar_table, ".ddr2")
static unsigned short invvar_table[ 2 << 13 ] = {
#include "table/invvar_table.inl"
    };
/*
int invvar_table_inited = 0;
int invvar_table_init( void ) {
  if ( 0 == invvar_table_inited ) {
    int ivar2;
    invvar_table_inited = 1;
    for ( ivar2 = 0; ivar2 < ( 2 << 13 ); ++ivar2 ) {
      invvar_table[ ivar2 ] = INVSQRT( ivar2 );
    }
  }
  return 0;
}
*/

#define INVSQRT2(_X)  invvar_table[(unsigned short) (_X)];
#else

#define INVSQRT2(_X)  INVSQRT(_X)
#endif

static double get_invvar_0( const FIX_FLOAT* tmpSamp, const FIX_FLOAT* tmpSamp1, int _AL, int _AI, int area ) {
  double var;
  var = RECT_VAL2( tmpSamp, ( _AL ), ( _AI ) ) / area;
  var = RECT_VAL2( tmpSamp1, ( _AL ), ( _AI ) ) / area - var * var;
  
  return ( var > (double)0.f ) ? (double)INVSQRT((uint)var) : (double)1.f;
}

static int get_invvar_1( const FIX_FLOAT* tmpSamp, const FIX_FLOAT* tmpSamp1, int _AL, int _AI, int area2 ) {
  int invvar;
  int a, b, var, a1, b1;
  a1 = RECT_VAL2( tmpSamp, ( _AL ), ( _AI ) );
  b1 = RECT_VAL2( tmpSamp1, ( _AL ), ( _AI ) );
  //if (a>(1<<16)) {
  a = (a1<<5) / area2;//四舍五入
  b = b1 / area2;
  var = b - ((a * a)>>10);
  //} else {
  //  var = (b - (a * a) / area2) / area2;
  //}
  if (var>(2 << 13)) {
    var = (2 << 13)-1;
  }
  //ASSERT(var<(2 << 13));
  invvar = INVSQRT2( var );

  return invvar;
}


#include "lutpass_0.inl"
#include "lutpass_1.inl"

// 计算图像偏移量
static int setoffset1( int len, const fHaar* f, fHaarWork* p, int al, int ai, REAL_TYPE cs, REAL_TYPE iv ) {
  INT ar0, ar1;
  const fHaar * l = f + len;
  for ( ; f != l; ++f, ++p ) {
    int a, b, c, cx, cy;
    if ( f->ft0.w && 0 == f->ft0.h )
      continue;
      
    //SETOFFSET(f->ft0, p->ft0, ar0);
    cx = XROUND( ( f->ft0 ).w * cs );
    cy = XROUND( ( f->ft0 ).h * cs );
    a = XROUND( ( f->ft0 ).y * cs ) * al + XROUND( ( f->ft0 ).x * cs ) * ai;
    b = cx * ai, c = cy * al;
    ar0 = cx * cy;
    ( p->ft0 ).p0 = a;
    ( p->ft0 ).p1 = a + b;
    ( p->ft0 ).p2 = a + c;
    ( p->ft0 ).p3 = a + b + c;
    SETOFFSET( f->ft1, p->ft1, ar1 );
    p->ft1.dd = (float)( f->ft1.d * iv );
    if ( ar0 ) {
      p->ft0.dd = -( p->ft1.dd * ar1 / ar0 );
    }
  }
  
  return 0;
}

// 设置偏移量
#define SETOFFSET2(RECT, OUT, area) \
  { \
    int cx = _SHIFT_MUL((RECT).w, ics); \
    int cy = _SHIFT_MUL((RECT).h, ics); \
    int a = _SHIFT_MUL((RECT).y, ics) * al + _SHIFT_MUL((RECT).x, ics) * ai; \
    int b = cx * ai, c = cy * al; \
    area = cx * cy; \
    (OUT).p0 = a; \
    (OUT).p1 = a + b; \
    (OUT).p2 = a + c; \
    (OUT).p3 = a + b + c; \
  }
static int setoffset2( int len, const fHaar* f, iHaarWork2* p, int al, int ai, REAL_TYPE cs, REAL_TYPE iv0 ) {
  INT ar0, ar1, ics = (int)(cs * _SHIFT1), iv = (int)(iv0 * _SHIFT1);
  const fHaar * l = f + len;
  for ( ; f != l; ++f, ++p ) {
    if ( 0 == f->ft0.w || 0 == f->ft0.h || 0 == f->ft1.w || 0 == f->ft1.h )
      continue;
#if 0
      
    SETOFFSET2( f->ft0, p->ft0, ar0 );
    SETOFFSET2( f->ft1, p->ft1, ar1 );
#else
    
    {
      int a, b, c, cx, cy;
      cx = _SHIFT_MUL( ( f->ft0 ).w, ics );
      cy = _SHIFT_MUL( ( f->ft0 ).h, ics );
      a = _SHIFT_MUL( ( f->ft0 ).y, ics ) * al + _SHIFT_MUL( ( f->ft0 ).x, ics ) * ai;
      b = cx * ai;
      c = cy * al;
      ar0 = cx * cy;
      ( p->ft0 ).p0 = a;
      ( p->ft0 ).p1 = a + b;
      ( p->ft0 ).p2 = a + c;
      ( p->ft0 ).p3 = a + b + c;
    }
    {
      int a, b, c, cx, cy;
      cx = _SHIFT_MUL( ( f->ft1 ).w, ics );
      cy = _SHIFT_MUL( ( f->ft1 ).h, ics );
      a = _SHIFT_MUL( ( f->ft1 ).y, ics ) * al + _SHIFT_MUL( ( f->ft1 ).x, ics ) * ai;
      b = cx * ai;
      c = cy * al;
      ar1 = cx * cy;
      ( p->ft1 ).p0 = a;
      ( p->ft1 ).p1 = a + b;
      ( p->ft1 ).p2 = a + c;
      ( p->ft1 ).p3 = a + b + c;
    }
    
#endif
    p->ft1.dd = ( f->ft1.d * iv );
    if ( ar0 ) {
      p->ft0.dd = -( p->ft1.dd * ar1 / ar0 );
    }
  }
  
  return 0;
}

#define SETOFFSET3(RECT, OUT, area) \
  { \
    (OUT).p0 = _SHIFT_MUL((RECT).y, ics) * al + _SHIFT_MUL((RECT).x, ics) * ai; \
    (OUT).w = _SHIFT_MUL((RECT).w, ics); \
    (OUT).h = _SHIFT_MUL((RECT).h, ics) * al; \
    area = (OUT).w * (OUT).h; \
  }

static int setoffset3( int len, const fHaar* f0, fHaarWork* p, int al, int ai, REAL_TYPE cs, REAL_TYPE iv0, int is_int ) {
  INT ar0=0, ar1=0, ics = (int)(cs * _SHIFT1), iv = (int)(iv0 * _SHIFT1);
  iHaarWork3 * p3 = ( iHaarWork3 * ) p;
  if (is_int) {
    const iHaar * f = (const iHaar *)f0;
    const iHaar * l = f + len;
    iv = (int)(iv0);
    //int i;
    //for ( i=0; i< len ;i++, ++f, ++p ) {
    //printf("%d %d %d %d \n", f->ft0.x, f->ft0.y, f->ft0.w, f->ft0.h);
    for ( ; f < l; ++f, ++p3 ) {
      // if ( 0 == f->ft0.w || 0 == f->ft0.h || 0 == f->ft1.w || 0 == f->ft1.h )
      //   continue;
      SETOFFSET3( f->ft0, p3->ft0, ar0 );
      SETOFFSET3( f->ft1, p3->ft1, ar1 );
      p3->ft1.dd = ( f->ft1.d * iv );
      p3->ft0.dd = ( ar0 ) ? ( -( p3->ft1.dd * ar1 / ar0 ) ) : ( -p3->ft1.dd );
      //printf("ft1=%d %d %d %d\n", f->ft1.x, f->ft1.y, f->ft1.w, f->ft1.h);
    }
  } else {
    const fHaar * f = f0;
    const fHaar * l = f + len;
    iv = (int)(iv0);
    //int i;
    //for ( i=0; i< len ;i++, ++f, ++p ) {
    for ( ; f < l; ++f, ++p3 ) {
      // if ( 0 == f->ft0.w || 0 == f->ft0.h || 0 == f->ft1.w || 0 == f->ft1.h )
      //   continue;
      SETOFFSET3( f->ft0, p3->ft0, ar0 );
      SETOFFSET3( f->ft1, p3->ft1, ar1 );
      p3->ft1.dd = ( f->ft1.d * iv );
      p3->ft0.dd = ( ar0 ) ? ( -( p3->ft1.dd * ar1 / ar0 ) ) : ( -p3->ft1.dd );
    }
  }
  
  return 0;
}

#define cast_sqr_macro(x) ((x) * (x))

static int cvIntegralImage_C1R( int size_height, int size_width, const unsigned char* src, int srcstep, FIX_FLOAT* sum, int sumstep, FIX_FLOAT* sqsum, int sqsumstep, FIX_FLOAT* tilted, int tiltedstep ) {
  int x, y;
  FIX_FLOAT s;
  FIX_FLOAT sq;
	UNUSED(tilted);
	UNUSED(tiltedstep);
  //memset( ( size_width + 1 ), 0, ( size_width + 1 ) * sizeof( sum[ 0 ] ) );
  FILL(sumstep, sum, 0);
  sum += sumstep + 1;
  FILL(( size_width + 1 ), sqsum, 0);
  //memset( sqsum, 0, ( size_width + 1 ) * sizeof( sqsum[ 0 ] ) );
  sqsum += sqsumstep + 1;
  for ( y = 0; y < size_height; y++, src += srcstep, sum += sumstep, sqsum += sqsumstep ) {
    sum[ -1 ] = 0;
    sqsum[ -1 ] = 0;
    for ( x = 0, s = 0, sq = 0; x < size_width; x++ ) {
      FIX_FLOAT it = src[ x ];
      FIX_FLOAT t = ( it );
      FIX_FLOAT tq = cast_sqr_macro( it );
      s += t;
      sq += tq;
      t = sum[ x - sumstep ] + s;
      tq = sqsum[ x - sqsumstep ] + sq;
      sum[ x ] = t;
      sqsum[ x ] = tq;
    }
  }
  
  return 0;
}
#undef cast_sqr_macro

#include "lut_detect_1.inl"

static void* detect_fix(void* p0, void* buf, int bufsize) {
  lut_detect_param_t* p = (lut_detect_param_t*)p0;
  //int buflen;
  //buflen = detect_memsize( pa );
  //if (NULL!=membuf && buflen < bufsize)
  {
    int i, h, w;
    XRECT r = p->roi;
#if 0
    p->sc    = SHIFT_TOFLOAT(p->isc    );
    p->ssmin = SHIFT_TOFLOAT(p->issmin );
    p->ssmax = SHIFT_TOFLOAT(p->issmax );
    p->ss    = SHIFT_TOFLOAT(p->iss    );
    p->thd   = SHIFT_TOFLOAT(p->ithd   );
#endif
    if (r.w) {
      p->height = r.h;
      p->width = r.w;
    }
    p->h = h = XFLOOR(p->height*p->sc);
    p->w = w = XFLOOR(p->width*p->sc);
    BUFUSEBEGIN(buf, p->buflen);
    detect_set_cas(p, p->cas, p->casnum);
    for (i=0; i<p->casnum; ++i) {
      HAARCASCADE * ca = ( HAARCASCADE* )(p->cas[i]);
      p->pWirk[i] = BUFMALLOC( fHaarWork, ca->ihlen );
    }
    p->tmpimg0 = BUFMALLOC( unsigned char, w * h );
    p->tmpmask0 = BUFMALLOC( unsigned char, w * h );
    p->tmpimg = BUFMALLOC( unsigned char, w * h );
    p->tmpmask = BUFMALLOC( unsigned char, w * h );
    p->inter = BUFMALLOC( FIX_FLOAT, ( h + 1 ) * ( w + 1 ) );
    p->inter1 = BUFMALLOC( FIX_FLOAT, ( h + 1 ) * ( w + 1 ) );
    p->rect_partition_buf = BUFMALLOC( unsigned char, SEQPARTITIONSIZE(sizeof(XRECT), p->maxoutlen) );
    BUFUSEEND();
  }
  return p;
}

int detect_process(void* p0) {
  int ret;
  detect_info_t* p = (struct lut_detect_param_t *)p0;
  p = (struct lut_detect_param_t *)detect_fix(p, p->buf, p->buflen);
  if (NULL==p) {
    return 0;
  }
  //const HAARCASCADE* cas = p->cas;
  //ss = p->ss, ssmin = p->ssmin, ssmax = p->ssmax;
  //STS_set(&sts0, CLK_gethtime());
  //STS_delta(&sts0, CLK_gethtime());
#if 0
  if ( p->fastmode && 0 ) {
    double ss1, sss = 1.9, ssmin1;
    sss /= ssmin;
    //ssmax = MIN( ssmax, ( p->height / ( ( PHAARCASCADE ) cas ) ->h ) );
#if 0
    
    ss1 = pow( ss, 0.5 * ssmax / ssmin );
#else
    
    for ( ss1 = 1.f, ssmin1 = ssmin; ssmin1 < ssmax; ss1 += sss, ssmin1 *= ss1 ) {}
    
#endif
    
    ss1 = MAX( ss1, ss );
    for ( ssmin1 = ssmin; ssmin1 < ( ssmax / ss1 ); ssmin1 *= ss1 ) {}
    
    for ( ret = 0; !ret && ssmin1 >= ( ssmin - 0.00001 ); ssmax = ssmin1, ssmin1 /= ss1 ) {
      detect_info_t tmpalg = *p;
      //assert( ( 1.f / ssmin1 ) <= ( 1.1f / ssmin ) && "内存错误!" );
      tmpalg.sc = 1.f / ssmin1;
      tmpalg.ssmin = 1.f;
      tmpalg.ssmax = ssmax / ssmin1;
      //printf("%f  %f  %f  \n", ssmin, ssmin1, ssmax2);
      ret += lut_detect( &tmpalg );
    }
  } else 
#endif
  {
    //return 0;
  ret = lut_detect( p );
  }
  
  p->outlen = ret;
  return 1;
}

int detect_process2(lut_detect_param_t* p, int bufsize, XRECT* out, 
                    const unsigned char* data, const unsigned char* mask, 
                    const void* cas0, const void* cas1,
                    const void* cas2, const void* cas3) {
  int ret=0;
  //if (((int*)cas0)[0]==32) return 0;
  p->data = data;
  p->mask = mask;
  p->out = out;
  p->cas[0] = cas0;
  p->cas[1] = cas1;
  p->cas[2] = cas2;
  p->cas[3] = cas3;
  detect_set_cas(p, p->cas, 4);
  //if (((const HAARCASCADE*)(p->cas[0]))->w==32) return 0;
  detect_memsize(p);
  if (bufsize >= p->buflen) {
    p->buflen = bufsize;
    ret = detect_process(p);
  }
  return ret;
}

#define GET_EYE_RECT(_FACE, _DEYE, _LEYE, _REYE, _W, _DY, _CY, _DX, _CX) \
  ((_DEYE)->x = MAX(0, (_FACE)->x-(_DX)), \
  (_DEYE)->y  = (_FACE)->y+(_DY), \
  (_DEYE)->w = MIN(_W, (_FACE)->w+(_DX)+(_DX)), \
  (_DEYE)->h = (_CY), \
  (_LEYE)->x  = (_DEYE)->x, \
  (_LEYE)->y  = (_DEYE)->y, \
  (_LEYE)->w = (_CX), \
  (_LEYE)->h = (_DEYE)->h, \
  (_REYE)->x  = (_DEYE)->x+(_DEYE)->w-(_CX), \
  (_REYE)->y  = (_DEYE)->y, \
  (_REYE)->w = (_CX), \
  (_REYE)->h = (_DEYE)->h)
  

//static unsigned int NIRDoubleEYE[] = {
//#include "cas/NIRDoubleEYE.inl"
//                                      };
static int detect_eyes(int height, int width, const unsigned char* data, int datastep, void const* deyecas[], void const* leyecas0[], void const* reyecas0[],
                const XRECT* face, XRECT* deye, XRECT* leye, XRECT* reye, unsigned char* buf, int buflen, XRECT* out, int maxoutlen, int max_width, int is_trans, int is_flip, int is_flop) {
  float sc, mincx, kk=180.f;
  int ret, cy, dy, dx, cx, t;
  lut_detect_param_t pp = {0};
  XRECT face2=*face, ddeye, dleye, dreye;
  void const** leyecas=leyecas0;
  void const** reyecas=reyecas0;
  if (is_trans) {
    reyecas=leyecas0;
    leyecas=reyecas0;
  }
  //if (is_trans) { void const** t; CC_SWAP(leyecas, reyecas, t); }
  //deyecas[0] = leyecas[3];
  //leyecas[3]=0;
  if (deyecas[0]) {
    dx = (int)(face->w * ( 0.1 ));    // 水平偏移
    cx = (int)(face->w * ( 0.4 ))+dx; // 宽
    dy = (int)(face->h * ( 0.1 ));  // 垂直偏移
    cy = (int)(face->h * ( 0.3 ));    // 高
  } else {
    dx = (int)(face->w * ( 0.0 ));    // 水平偏移
    cx = (int)(face->w * ( 0.5 ))+dx; // 宽
    dy = (int)(face->h * ( 0.15 ));   // 垂直偏移
    cy = (int)(face->h * ( 0.3 ));    // 高
  }
  mincx = face->w / kk;
  if (is_trans) {
    RECT_TRANS(face2, t);
  }
  GET_EYE_RECT(&face2, &ddeye, &dleye, &dreye, max_width, dy, cy, dx, cx);
  if (is_trans) {
    RECT_TRANS(ddeye, t);
    RECT_TRANS(dleye, t);
    RECT_TRANS(dreye, t);
    CC_SWAP(dleye, dreye, face2);
  }
  sc = 1.f / mincx;
  pp.buf = (unsigned char *)buf;
  //pp=*p;
  if (deye) *deye = ddeye;
#if 1
  if (deyecas[0]) {
    // left
    //imwrite( "./leye.bmp", deye->h, deye->w/2, pos, srcstep, 1 );
    detect_init(&pp, 0, 0, deye->h, deye->w, data, datastep,
      0, 0, sc, 4.f*kk/180.f, 20.f*kk/180.f, 1.05f, 1, 0.8f, 0, 0, out, maxoutlen, is_trans, is_flip, is_flop );
    pp.buf = (unsigned char *)buf;
    ret = 1;
    if (ret) {
      //NIRDoubleEYE
      pp.roi = ddeye;
      pp.is_flop = 0;
      pp.outlen=0;
      RECT_SET(pp.out[0], 0, 0, 0, 0);
      ret = detect_process2(&pp, buflen, out, data, 0, deyecas[0], deyecas[1], deyecas[2], deyecas[3]);
      //ASSERT(ret && "buf空间太少了！");
      if (pp.outlen && deye) {
        *deye = pp.out[0];
        if (1) {
          ddeye = pp.out[0];
          if (is_trans) {
            ddeye.y = face->y;
            ddeye.h = face->h;
            dreye = dleye = ddeye;
            dleye.h/=2;
            dreye.h/=2;
            dreye.y+=dleye.h;
            CC_SWAP(dleye, dreye, face2);
          } else {
            ddeye.x = face->x;
            ddeye.w = face->w;
            dreye = dleye = ddeye;
            dleye.w/=2;
            dreye.w/=2;
            dreye.x+=dleye.w;
          }
        }
#if 0
        if (0) {
          if (is_trans) {
            dleye.x=dleye.x=deye->x;
            //dleye.w=dleye.w=deye->w;
          } else {
            dleye.y=dleye.y=deye->y;
            //dleye.h=dleye.h=deye->h;
          }
        }
#endif
      } else {
        deye->x=deye->y=deye->w=deye->h=0;
        return 0;
      }
      ret = ret && pp.outlen && pp.out[0].w && pp.out[0].h;
    }
  }
#endif
  detect_init(&pp, leyecas, 4, dleye.h, dleye.w, data, datastep,
    0, 0, sc, 1.4f*kk/180.f, 2.f*kk/180.f, 1.05f, 1, 0.8f, 5, 0, out, maxoutlen, is_trans, is_flip, is_flop );
  pp.buf = (unsigned char *)buf;
  pp.buflen = buflen;
  ret = 1;
  if (ret) {
    pp.outlen=0;
    RECT_SET(pp.out[0], 0, 0, 0, 0);
    pp.roi = dleye;
    pp.is_flop=0;
    //imwriterect(".\\leyeroi.bmp", data, datastep, 1, dleye.x, dleye.y, dleye.w, dleye.h);
    ret = detect_process2(&pp, buflen, out, data, 0, leyecas[0], leyecas[1], leyecas[2], leyecas[3]);
    ASSERT(ret && "buf空间太少了！");
    if (pp.outlen && leye) {
      *leye = pp.out[0];
    }
    ret = ret && pp.outlen && pp.out[0].w && pp.out[0].h;
#if 0
    {
      int i=0;
      for (i=0; i<pp.outlen; ++i) {
        DrawRectangle2(pp.width, pp.height, pp.data, pp.datastep, 1, pp.out[i], CC_RGB(255, 0, 0), 2);
      }
      cvShowImage("asdf", pp.width, pp.height, pp.data, pp.datastep, 1);
      cvWaitKey(-1);
    }
#endif
  }
  // right
  //imwrite( "./reye.bmp", deye->h, deye->w/2, pos, srcstep, 1 );
  if (ret) {
    pp.outlen=0;
    RECT_SET(pp.out[0], 0, 0, 0, 0);
    pp.roi = dreye;
    pp.is_flop=0;
    ret = detect_process2(&pp, buflen, out, data, 0, reyecas[0], reyecas[1], reyecas[2], reyecas[3]);
    ASSERT(ret && "buf空间太少了！");
    if (pp.outlen && reye) {
      *reye = pp.out[0];
    }
    ret = ret && pp.outlen && pp.out[0].w && pp.out[0].h;
  }
  if (deyecas[0] && leye->w && reye->w) {
    deye->x = leye->x-5-(leye->x&15);
    deye->w = reye->x+reye->w-leye->x+10+(leye->y&15);
    deye->h = deye->w/4;
  }
  return ret;
}

//unsigned char facebuf[ DETECT_MEMSIZE( 1000, 500, 500 ) ];
// 
// 函数功能：用输入的分类器检测灰度图像上人脸的位置
// 参数说明：
// 输入：
// cas - 分类器，由 CPM_GetCascade 获取
// [h, w, A, al] - 灰度图片[高, 宽, 左上角指针, 步进]
// 参数sc,ssmin,ssmax,ss,step控制检测框移动方式。
// sc            - 图像的缩放比例 如果给0.25，图像先缩小到原来的0.25倍再检测，如果给的值<0.00001则启动自动缩放策略。
// 假设分类器中定义的原始检测框大小为[20×20](象素)，检测函数会从[(20*ssmin)×(20*ssmin)]的大小开始(在图像上，先由左到右，再由上到下)搜索人脸，每次搜完整幅图像再将检测框放大ss倍重新开始搜索，直到[(20*ssmax)×(20*ssmax)]的大小为止。
// ssmin         - 检测框的最小倍数[0~INF]，其值越大待搜索的检测框的最小框越大，检测速度越快，其值越小待搜索的检测框的最小框越小，检测速度越慢（特别是其值小于1的时候）。
// ssmax         - 检测框的最大倍数[0~INF]，其值越大待搜索的检测框的最大框越大，对速度影响不大(建议值100)
// ss            - 检测框每次放大的倍数, 每次 ssmin*=ss;(建议值1.05～1.5)，其值越大检测速度越快，检测结果越少。
// step          - 检测框横向或纵向平移的步长，如果设为1或2，每次将检测框移动1或2个象素，如果设为0，则每次移动k个象素(k=当前检测框大小/原始检测框大小)。
// 参数mincnt,thd用来合并和筛选合并检测结果。
// mincnt        - 最小检测框合并数。如果最终结果的检测框是由5个原始检测框按覆盖率法则合并而成的，那么他的合并数就是n，本函数只返回合并数大于mincnt的结果(mincnt 一般设为5)，其值越大误检率越低，检测率越低。
// thd           - 如果两个检测框覆盖率大于thd，那么这两个检测框将会被合并成一个(建议值0.1 ~ 0.6)，直到结果中任意两个检测框的覆盖率都不大于thd为止。覆盖率指两个检测框的公共部分面积占两个检测框面积之和的百分比。
// 参数mask,maskstep为图像掩码，本函数只会搜索左上角象素对应的掩码值非0的检测框。
// mask          - 图像掩码只对mask[y*mask_step+x]的点(x, y)检测，mask为0无掩码
// maskstep      - 掩码步进
// 输出:
// [B, B_len]    - 外部分配的矩形框数组指针，会按 XRECT::count 值降序排列
// 函数返回：如果B==NULL返回需要的B_len大小，否则返回检测结果数目
// 使用说明：

int CPM_FaceDetect( const void* cas, int height, int width, const unsigned char* data, int datastep,
                   const unsigned char* mask, int maskstep, double sc, double ssmin, double ssmax, double ss, int stepxy,
                   double thd, int mincnt, XRECT* B, int B_len ) {
  //int fastmode = sc < 0.001; //快速模式
  lut_detect_param_t p={0};
  if (sc<0.01) {
    sc = 1./ssmin;
    ssmin = 1.;
  }
  detect_init(&p, &cas, 1, height, width, data, datastep, mask, maskstep, sc, ssmin, ssmax, ss, stepxy, thd, mincnt, 0, B, B_len, 0, 0, 0);
  p.buf = MALLOC(uchar, p.buflen);
  if (NULL!=p.buf) {
    detect_process(&p);
    FREE(p.buf);
  }
  return p.outlen;
}
typedef struct FaceDetectParams {
  double ssmin;
  double ssmax;
  double ss;
  int stepxy;
  double thd;
  int mincnt;
}FaceDetectParams;

FaceDetectParams* FaceDetectParamsDefault(FaceDetectParams* s) {
  s->ssmin = 1.;
  s->ssmax = 100.;
  s->ss = 1.1;
  s->stepxy = 1;
  s->thd = 0.81;
  s->mincnt = 5;
  return s;
}

static int cvFaceDetect( const void* cas, const img_t* im, const img_t* mask, double scale, const FaceDetectParams* params, XRECT* B, int B_len ) {
  img_t gry[1] = {0};
  FaceDetectParams _params[1] = {0};
  int ret = 0;
  const unsigned char* mask_ptr = NULL;
  int mask_step = 0;
  ASSERT(im->c==3 ||im->c==2 ||im->c==4 ||im->c==1 );
  if (im->c!=1) {
    //im2gry(im, gry);    im = gry;
  }
  if (NULL==params) {
    params = FaceDetectParamsDefault(_params);
  }
  if (mask) {
    if (!CC_ARE_SIZES_EQ(im, mask)) {
      CC_ERROR(CC_StsBadSize, "");
    }
    mask_ptr = mask->tt.data;
    mask_step = mask->step;
  }
  ret = CPM_FaceDetect(cas, im->h, im->w, im->tt.data, im->s, mask_ptr, mask_step,
    scale, params->ssmin, params->ssmax, params->ss, params->stepxy, params->thd, params->mincnt, B, B_len);
  imfree(gry);
  return ret;
}

static int face_detect( buf_t* bf, const void* cas, int height, int width, const unsigned char* data, int datastep,
                const unsigned char* mask, int maskstep, double sc, double ssmin, double ssmax, double ss, int stepxy,
                double thd, int mincnt, XRECT* B, int B_len, int is_trans ) {
  //int fastmode = sc < 0.001; //快速模式
  lut_detect_param_t p={0};
  if (sc<0.01) {
    sc = 1./ssmin;
    ssmin = 1.;
  }
  detect_init(&p, &cas, 1, height, width, data, datastep, mask, maskstep, sc, ssmin, ssmax, ss, stepxy, thd, mincnt, 0, B, B_len, is_trans, 0, 0);
  BFMALLOC(bf, p.buf, p.buflen);
  if (NULL!=p.buf) {
    detect_process(&p);
    BFFREE(bf, p.buf);
  }
  return p.outlen;
}

int cas_ihlen(const void* cas[], int casnum);

// 函数功能：本函数用输入的分类器检测人脸灰度图像上眼睛的位置
// 参数说明：
// [A, al]     - 图片[左上角指针, 步进]
// face        - 人脸框的位置
// deye        - 双眼分类器，红外情况由 CPM_GetCascade (NIR_DEYE_CASCADE) 获取，可见光传 NULL
// leye        - 左眼分类器，红外情况由 CPM_GetCascade (NIR_LEYE_GLASS_CASCADE) 获取，可见光由 CPM_GetCascade (VIS_EYE_CASCADE) 获取
// reye        - 右眼分类器，红外情况由 CPM_GetCascade (NIR_REYE_GLASS_CASCADE) 获取，可见光由 CPM_GetCascade (VIS_EYE_CASCADE) 获取
// lalleye     - 左眼粗检分类器，红外情况由 CPM_GetCascade (NIR_EYE_CASCADE) 获取，可见光传 NULL
// ralleye     - 右眼粗检分类器，红外情况由 CPM_GetCascade (NIR_EYE_CASCADE) 获取，可见光传 NULL
// leye_without_glass  - 左眼不带眼镜分类器，红外情况由 CPM_GetCascade (NIR_EYE_WITHOUT_GLASS_CASCADE) 获取，可见光传 NULL
// reye_without_glass  - 右眼不带眼镜分类器，红外情况由 CPM_GetCascade (NIR_EYE_WITHOUT_GLASS_CASCADE) 获取，可见光传 NULL
// mincnt      - 检测框聚类数，即结果的 XRECT::count 值必须大于总结果 百分之 mincnt 一般设为5
// [B, B_end]  - 输出 buffer 必须大于10
// 函数返回：检测结果数目, 3或者0, 3表示检测到左眼和右眼, 0表示左眼或右眼中至少一个没有检测到。
//          如果检测到 B[0] 存放双眼结果，B[1] 存放左眼结果，B[2] 存放右眼结果。
// 使用说明：分类器给的越多检测结果越多误差越大，反之检测结果越少误差越小
static int CPM_EyesDetect( const void* deye, const void* leye, const void* reye,
                    const void* lalleye, const void* ralleye, const void* leye_without_glass,
                    const void* reye_without_glass, const unsigned char* A, int al, int mincnt,
                    const XRECT* face, XRECT* B, int B_len ) {
  XRECT out[50]={0};
  int ret = 0, maxoutlen = 50, buflen = 0, ihlen;
  unsigned char* buf = 0;
  const void* deyecas[4], * leyecas[4], * reyecas[4];
  deyecas[0] = deye, deyecas[1] = 0, deyecas[2] = 0, deyecas[3] = 0;
  leyecas[0] = leye, leyecas[1] = lalleye, leyecas[2] = leye_without_glass, leyecas[3] = 0;
  reyecas[0] = reye, reyecas[1] = ralleye, reyecas[2] = reye_without_glass, reyecas[3] = 0;
  ihlen = MAX(cas_ihlen(leyecas, 4), cas_ihlen(reyecas, 4));
  buflen = DETECT_MEMSIZE(ihlen, MAX(128,face->h), MAX(128,face->w), maxoutlen );
  buf = (unsigned char*)pmalloc(buflen);
  if (buf) {
    ret = detect_eyes(0, 0, A, al, deyecas, leyecas, reyecas, (face), (B), (B+1), (B+2), buf, buflen, out, maxoutlen, al, 0, 0, 0);
    pfree(buf);
    if (ret) {
      ret = 3;
    }
  }
  return ret;
}

// 计算用于规一化的仿射变换参数
CC_INLINE MATRIX3X2 get_std( IPOINT s1, IPOINT e1, IPOINT s2, IPOINT e2 ) {
  MATRIX3X2 m;
  double c, s;
  double dd = POINT_DIST( s1, e1 ) / ( POINT_DIST( s2, e2 ) );
  ANGLE_SIN_COS( e1.x - s1.x, e1.y - s1.y, e2.x - s2.x, e2.y - s2.y, s, c );
  c *= dd;
  s *= dd;
  MATRIX3X2_GET_STD_TOINT( m, c, s, s1, s2 );
  return m;
}
//  函数功能：人脸规一化
//  参数说明：
//  [ah, aw, A, al, ai] - 输入图片
//  [bh, bw, B, bl, bi] - 输出图片
//  leye, reye - 左右眼中心位置
//  lx - 左眼x坐标除以bw 一般取 0.25
//  ly - 左眼y坐标除以bh 一般取 0.25
//  rx - 右眼x坐标除以bw 一般取 0.75
//  ry - 右眼y坐标除以bh 一般取 0.25
//  函数返回：无意义
//  使用说明：
int CPM_FaceStd( int ah, int aw, const unsigned char* A, int al, int ai,
                int bh, int bw, unsigned char* B, int bl, int bi,
                IPOINT leye, IPOINT reye,
                double lx, double ly, double rx, double ry ) {
  IPOINT s2, e2;
  MATRIX3X2 m;
  s2.x = ( int ) ( bw * lx );
  s2.y = ( int ) ( bh * ly );
  e2.x = ( int ) ( bw * rx );
  e2.y = ( int ) ( bh * ry );
  m = get_std( leye, reye, s2, e2 );
  IMAFFINE( ah, aw, A, al, ai, bh, bw, B, bl, bi, m, INTER_BILINEARITY );
  return 0;
}

int CPM_FaceStd2( int ah, int aw, const unsigned char* A, int al, int ai,
                int bh, int bw, unsigned char* B, int bl, int bi,
                IPOINT leye, IPOINT reye,
                double lx, double ly, double rx, double ry ) {
  IPOINT s2, e2;
  MATRIX3X2 m;
  s2.x = ( int ) ( bw * lx );
  s2.y = ( int ) ( bh * ly );
  e2.x = ( int ) ( bw * rx );
  e2.y = ( int ) ( bh * ry );
  m = get_std( leye, reye, s2, e2 );
  IMAFFINE2( ah, aw, A, al, ai, bh, bw, B, bl, bi, m, INTER_BILINEARITY );
  return 0;
}

#if 0
// 用method指定的方法判断矩形是否在矩形列表中（如果距离小于thd就认为两个矩形相等）
// 如果在就返回编号，否则返回-1
int is_rect_in_rectlist( const XRECT* rect, const XRECT* rectlist, int listlen, REAL_TYPE thd, int method ) {
  int i;
  for ( i = 0; i < listlen; ++i ) {
    int cover = 0;
    IS_RECT_EQUAL_COVER_RETE( *rect, rectlist[ i ], thd, cover );
    if ( cover ) {
      return i;
    }
  }
  return -1;
}

static iRectW getEyesRect( int imgWidth, const iRectW face_rect, double h1, double h2, double w1 ) {
  iRectW ir;
  ir.x = face_rect.x - ( int ) ( 0.5 + 0.5 * ( w1 - 1.0 ) * ( face_rect.w ) );
  if ( ir.x < 0 ) {
    ir.x = 0;
  }
  
  ir.y = face_rect.y + XROUND( h1 * ( face_rect.h ) );
  ir.w = XROUND( w1 * ( face_rect.w ) );
  if ( ( ir.x + ir.w ) >= imgWidth ) {
    ir.x = imgWidth - 1 - ir.w;
  }
  
  ir.h = XROUND( h2 * ( face_rect.h ) );
  return ir;
}

static void* memdup( const void* p, size_t n ) {
  void * o = pmalloc( n );
  if ( NULL != o ) {
    memcpy( o, p, n );
  }
  return o;
}

#endif

#endif // _FACEDETECT_INL_

