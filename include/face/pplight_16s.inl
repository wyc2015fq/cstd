
#define TANHXX_MAP_BIN (11)

static const short pow02_13_table[] = {
#include "./table/pow02_13.txt"
};
static const short pow01_13_table[] = {
#include "./table/pow01_13.txt"
};
static const short tanh_13_table[] = {
#include "./table/tanh_13.txt"
};
static const int tanh_130_table[] = {
#include "./table/tanh_130.txt"
};
#define PPMAP_SIZE 1024
static const unsigned ppmap_table[PPMAP_SIZE] = {
#include "./table/ppmap.txt"
};
// pp预处理
static int pplight_16s( int h, int w, const unsigned char* A, int al, int ai, unsigned char* B, int bl, int bi, void* buf ) {
  typedef int T;
  //static const T do_norm = 10;
#define POW02(y, x)        (y) = (T)pow02_13_table[(unsigned char)x]
  //int to_gray = 1;
  int hw = h * w;
  T* xx, *yy;
  int _I, _AMIN = 0, _AMAX = 0;
  int ttt;
  xx = (int*)buf;//( T, ( h + 13 ) * ( w + 13 ) + hw + ( h + 13 ) + 1111 );
  yy = xx + ( h + 13 ) * ( w + 13 );
  //memset(xx, 0, sizeof(T)*2*hw);
  // fill array with xx
  FILL( 2 * hw, xx, 0 );
  // 拷贝A 到 xx中去
  SKIP_COPY( h, w, A, al, 1, xx, w, 1 );
  {
    T* xx0, *zz;
    int xxl = ( w + 12 );
    zz = yy + hw;
    // 对每个元素计算 POW02
    VEC_OPT_VV( hw, yy, xx, POW02 );
    COPY_REPLICATE_BORDER( h, w, yy, w, ( h + 12 ), ( w + 12 ), xx, ( w + 12 ), 1, 6, 6 );
    xx0 = xx + 6 * xxl + 6;
    // 两次高斯滤波
    SKIP_FILTER_RC( h, w, xx0, xxl, 1, yy, w, 1, 6, 6, zz, GAUSS2_I13X13, GAUSS2_I13X13 );
    SKIP_FILTER_RC( h, w, xx0, xxl, 1, xx, w, 1, 3, 3, zz, GAUSS_I7X7, GAUSS_I7X7 );
    VEC_SUB_VV( hw, xx, yy );
  }
#if 1
#define ABSPOWOR_TABLE(y, x)     (y) += (T)((x>0) ? pow01_13_table[(x)>4095?4095:(x)] : pow01_13_table[(-x)>4095?4095:(-x)])
#define MINABSPOWOR_TABLE(y, x)     (y) += (T)((x>0) ? pow01_13_table[(x)>4095?4095:(x)] : pow01_13_table[(-x)>4095?4095:(-x)])
  ttt = 0;
  // 对xx每个元素计算 ABSPOWOR_TABLE
  VEC_OPT_XV( hw, xx, ttt, ABSPOWOR_TABLE );
#define do_norm 10
  // 计算 y=x^10
#define POW10(_X, _Y) { double _z = (_X); _Y=(int)(_z*_z*_z*_z*_z*_z*_z*_z*_z*_z); }
  // ttt = ( T ) pow( hw*1.*_SHIFT1/ttt, do_norm );
  POW10(hw*1.*_SHIFT1/ttt, ttt);
  {
    int i;
    for (i=0; i<hw; ++i) {
      xx[i] *= ttt;
    }
  }
  //VEC_MUL_VX( hw, xx, ttt );
  ttt = 0;
  //VEC_OPT_XV( hw, xx, ttt, MINABSPOWOR );
#endif
#define clip(_x, _z) ((_x)<(_z)?(_x):((_z)-1))
#if 0
#define TANHXX_TABLE(x)        (x>0) ? (T)(tanh_13_table[(unsigned char)x]) : (T)(-tanh_13_table[(unsigned char)(-x)])
#else
#define TANHXX_TABLE(x)        (x>=0) ? (T)(tanh_130_table[clip(x, (1<<19))>>(19-TANHXX_MAP_BIN)]) : (T)(-tanh_130_table[clip((-x), (1<<19))>>(19-TANHXX_MAP_BIN)])
  
//#define TANHXX_TABLE(_x)        (T)((1<<13)*tanh((_x*1./(1<<13))*0.103)*do_norm)
  //对每个元素计算 TANHXX_TABLE
  VEC_OPT_VP( hw, xx, TANHXX_TABLE );
#endif
#undef TANHXX
#undef clip
#if 1
//#include "make_ppmap.inl"
  // 线性映射 => [0-255]
  {
    MIN_ELEMENT( hw, xx, _AMIN );
    MAX_ELEMENT( hw, xx, _AMAX );
    if ( _AMIN != _AMAX ) {
      int vmin = xx[ _AMIN ];
      int vmax = xx[ _AMAX ];
      int __A = ( 256 << _SHIFT ) / ( vmax - vmin );
      int __B = -__A * vmin;
      for ( _I = 0; _I < hw; ++_I ) {
        *( xx + _I ) = ( __A * ( *( xx + _I ) ) + __B ) >> _SHIFT;
      }
    }
  }
  SKIP_COPY_CAST( h, w, xx, w, 1, B, bl, 1, ( unsigned char ) );
#else
  {
    MIN_ELEMENT( hw, xx, _AMIN );
    MAX_ELEMENT( hw, xx, _AMAX );
    if ( _AMIN != _AMAX ) {
      int vmin = xx[ _AMIN ];
      int vmax = xx[ _AMAX ];
      int __A = ( PPMAP_SIZE << _SHIFT ) / ( vmax - vmin );
      int __B = -__A * vmin;
      for ( _I = 0; _I < hw; ++_I ) {
        *( xx + _I ) = ( __A * ( *( xx + _I ) ) + __B ) >> _SHIFT;
      }
    }
  }
  SKIP_MAP( h, w, xx, w, 1, B, bl, 1, ppmap_table );
#endif
  //FREE( xx );
  return 0;
}

#undef POW02
#undef ABSPOWOR
#undef MINABSPOWOR
