
#ifndef _PPLIGHT_32F_INL_
#define _PPLIGHT_32F_INL_
// pp预处理
static int pplight_32f( int h, int w, const unsigned char* A, int al, int ai, unsigned char* B, int bl, int bi, void* buf ) {
  typedef double T;
  static const T do_norm = 10.0;
#define POW02(y, x)        (y) = (T)pow(x, 0.2)
#define ABSPOWOR(y, x)     (y) += (T)pow(((x)<0) ? -(x) : (x), 0.1)
#define MINABSPOWOR(y, x)                                            \
do {                                                                 \
  T _X0 = ((x)<0.0) ? -(x) : (x);                                    \
  (y) += (T)pow((double)((10.0<_X0)?(10.0):(_X0)), 0.1);                      \
} while(0)
  
  T ttt;
  int to_gray = 1;
  int hw = h * w;
  T* xx, *yy;
  xx = (T*)buf;
  yy = xx + ( h + 13 ) * ( w + 13 );
  //memset(xx, 0, sizeof(T)*2*hw);
  FILL( 2 * hw, xx, 0.f );
  SKIP_COPY( h, w, A, al, 1, xx, w, 1 );
#if 1
  
  {
    T* xx0, *zz;
    int xxl = ( w + 12 );
    zz = yy + hw;
    VEC_OPT_VV( hw, yy, xx, POW02 );
    COPY_REPLICATE_BORDER( h, w, yy, w, ( h + 12 ), ( w + 12 ), xx, ( w + 12 ), 1, 6, 6 );
    xx0 = xx + 6 * xxl + 6;
    // 两次高斯滤波
    SKIP_FILTER_RC( h, w, xx0, xxl, 1, yy, w, 1, 6, 6, zz, GAUSS2_F13X13, GAUSS2_F13X13 );
    SKIP_FILTER_RC( h, w, xx0, xxl, 1, xx, w, 1, 3, 3, zz, GAUSS_F7X7, GAUSS_F7X7 );
  }
#else
  VEC_OPT_VV( hw, xx, xx, POW02 );
  // 两次高斯滤波
  SKIP_FILTER_RCEX( h, w, xx, w, 1, yy, w, 1, 6, 6, GAUSS2_F13X13, GAUSS2_F13X13, T );
  SKIP_FILTER_RCEX( h, w, xx, w, 1, xx, w, 1, 3, 3, GAUSS_F7X7, GAUSS_F7X7, T );
#endif
  
  VEC_SUB_VV( hw, xx, yy );
#if 1
  
  ttt = 0.0f;
  //ABSPOWOR(ttt, xx[0]);
  //ttt = pow(-xx[0]*(1<<13), 0.1)/pow(1<<13, 0.1);
  VEC_OPT_XV( hw, xx, ttt, ABSPOWOR );
  ttt = ( T ) pow( hw/ttt, do_norm );
  VEC_MUL_VX( hw, xx, ttt );
  ttt = 0.0f;
  VEC_OPT_XV( hw, xx, ttt, MINABSPOWOR );
  ttt = ( T ) pow( hw/ttt, do_norm )/do_norm;
#else
  
  ttt = 0.1f;
#endif
#define TANHXX(_x)        (T)(tanh((_x)*ttt)*do_norm)
  
#define TANHXX_MAP_SIZE (1<<TANHXX_MAP_BIN)
#if 0
  {
    FILE* pf = fopen("./table/tanh_130.txt", "wb");
    int i;
    //ttt=0.1;
    for (i=0; i<TANHXX_MAP_SIZE; ++i) {
      double x = i*1.*(1<<19)/(1<<13)/TANHXX_MAP_SIZE;
      double d = (1<<13)*TANHXX(x);
      //fprintf(pf, "%6d  %6d,\r\n", (int)(x*(1<<13)), (int)d);
      d = cvRound(d);
      fprintf(pf, "%d,\r\n", (int)d);
    }
    fclose(pf);
  }
#endif
  VEC_OPT_VP( hw, xx, TANHXX );
  if ( to_gray ) {
    VEC_LINEARITY( hw, xx, xx, 0.f, 255.f );
  }
  SKIP_COPY_CAST( h, w, xx, w, 1, B, bl, 1, ( unsigned char ) );
  //FREE( xx );
  return 0;
}

#undef TANHXX
#undef POW02
#undef ABSPOWOR
#undef MINABSPOWOR


#endif // _PPLIGHT_32F_INL_
