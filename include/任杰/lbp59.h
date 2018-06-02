
#if 1
#define COLOR_FOR(A, _ai, B, _i) for (_i=0; _i<(_ai); ++_i)
#else
#define COLOR_FOR(A, _ai, B, _i)
#endif

// 最高位最多乘2^7，所以<<操作最多7次，
// 最后一次按位与即可完成+1或+0的操作(即1*2^0或者0*2^0)。
#define LBP_FT_BASE(A, al, ai, p)                                             \
do {                                                                          \
  int wl=al+ai, wi=al-ai, _V=*(A);                                            \
  p |= (uchar)(*(A-ai) >= _V); p <<= 1;                                       \
  p |= (uchar)(*(A+wi) >= _V); p <<= 1;                                       \
  p |= (uchar)(*(A+al) >= _V); p <<= 1;                                       \
  p |= (uchar)(*(A+wl) >= _V); p <<= 1;                                       \
  p |= (uchar)(*(A+ai) >= _V); p <<= 1;                                       \
  p |= (uchar)(*(A-wi) >= _V); p <<= 1;                                       \
  p |= (uchar)(*(A-al) >= _V); p <<= 1;                                       \
  p |= (uchar)(*(A-wl) >= _V);                                                \
} while(0)

#define LBPNBIN_FT(A, al, ai, B, BIN)                                         \
do {                                                                          \
  uchar p=0;                                                                  \
  LBP_FT_BASE(A, al, ai, p);                                                  \
  *(B) = (p*BIN)>>8;                                                          \
} while(0)

#define LBP8BIN_FT(A, al, ai, B) LBPNBIN_FT(A, al, ai, B, 8)

#define LBP_FT(A, al, ai, B)                                                  \
do {                                                                          \
  uchar p=0;                                                                  \
  int i = 0;                                                                  \
  LBP_FT_BASE(A, al, ai, p);                                                  \
  COLOR_FOR(A, ai, B, i) {                                                    \
    *(B+i) = p;                                                               \
  }                                                                           \
} while(0)

#define LBP59_FT(src, srcstep, cn, B)  do { LBP_FT(src, srcstep, cn, B); } while(0)
