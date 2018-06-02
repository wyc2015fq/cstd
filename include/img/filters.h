
#ifndef _FILTERS_H_
#define _FILTERS_H_

//

#if 1
#define COLOR_FOR(A, _ai, B, _i) for (_i=0; _i<(_ai); ++_i)
#else
#define COLOR_FOR(A, _ai, B, _i)
#endif


// sector numbers
// 1   2   3
//   * * *
// 4 * 5 * 6
//   * * *
// 7   8   9
#define Z1 (-wl)
#define Z2 (-al)
#define Z3 (-wi)
#define Z4 (-ai)
#define Z5 (  0)
#define Z6 ( ai)
#define Z7 ( wi)
#define Z8 ( al)
#define Z9 ( wl)

#define DOT3X3(A, a1, a2, a3, a4, N1, N2, N3, N4, N5, N6, N7, N8, N9)         \
  (*(A+(-a4))*(N1) +                                                          \
      *(A+(-a3))*(N2) +                                                          \
      *(A+(-a2))*(N3) +                                                          \
      *(A+(-a1))*(N4) +                                                          \
      *(A+(  0))*(N5) +                                                          \
      *(A+( a1))*(N6) +                                                          \
      *(A+( a2))*(N7) +                                                          \
      *(A+( a3))*(N8) +                                                          \
      *(A+( a4))*(N9) )

// [ N00, N01, N02 ]      [ in00, in01, in02 ]
// [ N10, N11, N12 ]  .*  [ in10, in11, in12 ]
// [ N20, N21, N22 ]      [ in20, in21, in22 ]
#define TMP_FILTER3X3(A1, al, ai, B1, N1, N2, N3, N4, N5, N6, N7, N8, N9, SH) \
  do {                                                                          \
    int sum;                                                                    \
    int wi=al-ai;                                                               \
    int wl=al+ai;                                                               \
    COLOR_FOR(A, ai, B, i) {                                                    \
      sum = DOT3X3(A1, ai, wi, al, wl, N1, N2, N3, N4, N5, N6, N7, N8, N9)>>SH; \
      sum = BOUND(0, sum, 255);                                                 \
      *(B) = sum;                                                               \
    }                                                                           \
  } while(0)

//腐蚀
#define ERASION3X3_FT(A, al, ai, B, N0, N1, N2, N3, N4, N5, N6, N7, N8)       \
  do {                                                                          \
    const int wi=(al)-(ai), wl=(al)+(ai);                                       \
    int bu[9], maxi=0;                                                          \
    COLOR_FOR(A, ai, B, i) {                                                    \
      bu[0] = *(A-wl+i) - N0;                                                   \
      bu[1] = *(A-al+i) - N1;                                                   \
      bu[2] = *(A-wi+i) - N2;                                                   \
      bu[3] = *(A-ai+i) - N3;                                                   \
      bu[4] = *(A   +i) - N4;                                                   \
      bu[5] = *(A+ai+i) - N5;                                                   \
      bu[6] = *(A+wi+i) - N6;                                                   \
      bu[7] = *(A+al+i) - N7;                                                   \
      bu[8] = *(A+wl+i) - N8;                                                   \
      MIN_ELEMENT(9, bu, maxi);                                                 \
      *(B+i) = bu[maxi];                                                        \
    }                                                                           \
  } while(0)


// 膨胀
#define DILATION3X3_FT(A, al, ai, B, N0, N1, N2, N3, N4, N5, N6, N7, N8)      \
  do {                                                                          \
    const int wi=(al)-(ai), wl=(al)+(ai);                                       \
    int bu[9], maxi=0;                                                          \
    COLOR_FOR(A, ai, B, i) {                                                    \
      bu[0] = *(A-wl+i) + N0;                                                   \
      bu[1] = *(A-al+i) + N1;                                                   \
      bu[2] = *(A-wi+i) + N2;                                                   \
      bu[3] = *(A-ai+i) + N3;                                                   \
      bu[4] = *(A   +i) + N4;                                                   \
      bu[5] = *(A+ai+i) + N5;                                                   \
      bu[6] = *(A+wi+i) + N6;                                                   \
      bu[7] = *(A+al+i) + N7;                                                   \
      bu[8] = *(A+wl+i) + N8;                                                   \
      MAX_ELEMENT(9, bu, maxi);                                                 \
      *(B+i) = bu[maxi];                                                        \
    }                                                                           \
  } while(0)

#define ERASION3X3_FT1(A, al, ai, B)  ERASION3X3_FT(A, al, ai, B, 1, 1, 1, 1, 1, 1, 1, 1, 1)
#define DILATION3X3_FT1(A, al, ai, B)  DILATION3X3_FT(A, al, ai, B, 1, 1, 1, 1, 1, 1, 1, 1, 1)

// 中值滤波
#define MIDAM3X3_FT(A, al, ai, B)                                             \
  do {                                                                          \
    const int wi=(al)-(ai), wl=(al)+(ai);                                       \
    int bu[9];                                                                  \
    COLOR_FOR(A, ai, B, i) {                                                    \
      bu[0] = *(A-wl+i);                                                        \
      bu[1] = *(A-al+i);                                                        \
      bu[2] = *(A-wi+i);                                                        \
      bu[3] = *(A-ai+i);                                                        \
      bu[4] = *(A   +i);                                                        \
      bu[5] = *(A+ai+i);                                                        \
      bu[6] = *(A+wi+i);                                                        \
      bu[7] = *(A+al+i);                                                        \
      bu[8] = *(A+wl+i);                                                        \
      INSERT_SORT(bu, bu+9, LESS, int);                                         \
      *(B+i) = bu[4];                                                           \
    }                                                                           \
  } while(0)

// -1, -2, -1,
//  0,  0,  0,
//  1,  2,  1,
#define SOBEL_FT(A, al, ai, B)                                                \
  do {                                                                          \
    int wi=(al)-(ai), wl=(al)+(ai);                                             \
    COLOR_FOR(A, ai, B, i) {                                                    \
      *(B+i) = (*(A+wl+i) + *(A+(al)+i) + *(A+(al)+i) + *(A+wi)+i)              \
          - (*(A-wl+i) + *(A-(al)+i) + *(A-(al)+i) + *(A-wi)+i);               \
    }                                                                           \
  } while(0)

#define MEAN_FT(A, al, ai, B) TMP_FILTER3X3(A, al, ai, B,                     \
    (SHIFT1/9), (SHIFT1/9), (SHIFT1/9),                                         \
    (SHIFT1/9), (SHIFT1/9), (SHIFT1/9),                                         \
    (SHIFT1/9), (SHIFT1/9), (SHIFT1/9),                                         \
    SHIFT    )

// 最高位最多乘2^7，所以<<操作最多7次，
// 最后一次按位与即可完成+1或+0的操作(即1*2^0或者0*2^0)。
#define LBP_FT_BASE(A, al, ai, p)                                             \
  do {                                                                          \
    int wl=al+ai, wi=al-ai, _V=*(A);                                            \
    p |= (unsigned char)(*(A-ai) >= _V); p <<= 1;                                       \
    p |= (unsigned char)(*(A+wi) >= _V); p <<= 1;                                       \
    p |= (unsigned char)(*(A+al) >= _V); p <<= 1;                                       \
    p |= (unsigned char)(*(A+wl) >= _V); p <<= 1;                                       \
    p |= (unsigned char)(*(A+ai) >= _V); p <<= 1;                                       \
    p |= (unsigned char)(*(A-wi) >= _V); p <<= 1;                                       \
    p |= (unsigned char)(*(A-al) >= _V); p <<= 1;                                       \
    p |= (unsigned char)(*(A-wl) >= _V);                                                \
  } while(0)

#define LBPNBIN_FT(A, al, ai, B, BIN)                                         \
  do {                                                                          \
    unsigned char p=0;                                                                  \
    LBP_FT_BASE(A, al, ai, p);                                                  \
    *(B) = (p*BIN)>>8;                                                          \
  } while(0)

#define LBP8BIN_FT(A, al, ai, B) LBPNBIN_FT(A, al, ai, B, 8)

#define LBP_FT(A, al, ai, B)                                                  \
  do {                                                                          \
    unsigned char p=0;                                                                  \
    int i = 0;                                                                  \
    LBP_FT_BASE(A, al, ai, p);                                                  \
    COLOR_FOR(A, ai, B, i) {                                                    \
      *(B+i) = p;                                                               \
    }                                                                           \
  } while(0)

#if 0

static int gauss5x5_s1[ 2 ][ 100 ] = {
  {
    7403, 4490, 0, 0, 0, 0, 0, 0, 0, 0,
    6597, 4001, 893, 0, 0, 0, 0, 0, 0, 0,
    6538, 3966, 885, 73, 0, 0, 0, 0, 0, 0,
    6536, 3964, 885, 73, 2, 0, 0, 0, 0, 0,
    6536, 3964, 885, 73, 2, 0, 0, 0, 0, 0,
    6536, 3964, 885, 73, 2, 0, 0, 0, 0, 0,
    6536, 3964, 885, 73, 2, 0, 0, 0, 0, 0,
    6536, 3964, 885, 73, 2, 0, 0, 0, 0, 0,
    6536, 3964, 885, 73, 2, 0, 0, 0, 0, 0,
    6536, 3964, 885, 73, 2, 0, 0, 0, 0, 0,
  },
  {
    5926, 5229, 0, 0, 0, 0, 0, 0, 0, 0,
    4119, 3635, 2498, 0, 0, 0, 0, 0, 0, 0,
    3541, 3125, 2148, 1149, 0, 0, 0, 0, 0, 0,
    3345, 2952, 2029, 1086, 453, 0, 0, 0, 0, 0,
    3286, 2900, 1993, 1067, 445, 144, 0, 0, 0, 0,
    3271, 2887, 1984, 1062, 443, 144, 36, 0, 0, 0,
    3269, 2885, 1983, 1061, 442, 144, 36, 7, 0, 0,
    3268, 2884, 1982, 1061, 442, 144, 36, 7, 1, 0,
    3268, 2884, 1982, 1061, 442, 144, 36, 7, 1, 0,
    3268, 2884, 1982, 1061, 442, 144, 36, 7, 1, 0,
  }
};
#endif

#define GEN_FTX3(A, i, x0, x1, x2)   (((x0) * (*(A-  (i)))) + ((x1) * (*(A))) + ((x2) * (*(A+  (i)))) )

#define GAUSS_FTX1(A, i, x0, x1)                                 ((x1) * (*(A-  (i)) + *(A+  (i))) + ((x0) * (*(A))) )
#define GAUSS_FTX2(A, i, x0, x1, x2)                             ((x2) * (*(A-2*(i)) + *(A+2*(i))) + GAUSS_FTX1(A, i, x0, x1))
#define GAUSS_FTX3(A, i, x0, x1, x2, x3)                         ((x3) * (*(A-3*(i)) + *(A+3*(i))) + GAUSS_FTX2(A, i, x0, x1, x2))
#define GAUSS_FTX4(A, i, x0, x1, x2, x3, x4)                     ((x4) * (*(A-4*(i)) + *(A+4*(i))) + GAUSS_FTX3(A, i, x0, x1, x2, x3))
#define GAUSS_FTX5(A, i, x0, x1, x2, x3, x4, x5)                 ((x5) * (*(A-5*(i)) + *(A+5*(i))) + GAUSS_FTX4(A, i, x0, x1, x2, x3, x4))
#define GAUSS_FTX6(A, i, x0, x1, x2, x3, x4, x5, x6)             ((x6) * (*(A-6*(i)) + *(A+6*(i))) + GAUSS_FTX5(A, i, x0, x1, x2, x3, x4, x5))
#define GAUSS_FTX7(A, i, x0, x1, x2, x3, x4, x5, x6, x7)         ((x7) * (*(A-7*(i)) + *(A+7*(i))) + GAUSS_FTX6(A, i, x0, x1, x2, x3, x4, x5, x6))
#define GAUSS_FTX8(A, i, x0, x1, x2, x3, x4, x5, x6, x7, x8)     ((x8) * (*(A-8*(i)) + *(A+8*(i))) + GAUSS_FTX7(A, i, x0, x1, x2, x3, x4, x5, x6, x7))
#define GAUSS_FTX9(A, i, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9) ((x9) * (*(A-9*(i)) + *(A+9*(i))) + GAUSS_FTX8(A, i, x0, x1, x2, x3, x4, x5, x6, x7, x8))

/* 1/16[1    4    6    4    1]       */
/* ...| x0 | x1 | x2 | x3 | x4 |...  */
#define GAUSS3X3_FT(A, ai, B)    *(B) = GAUSS_FTX1(A, ai, 128, 64)>>8
#define GAUSS5X5_FT(A, ai, B)    *(B) = GAUSS_FTX2(A, ai, 6, 4, 1)>>4
#define GAUSS7X7_FT(A, ai, B)    *(B) = GAUSS_FTX3(A, ai, 6538, 3966, 885, 73)>>14
#define GAUSS13X13S2_FT(A, ai, B)  *(B) = GAUSS_FTX6(A, ai, 3271, 2887, 1984, 1062,  443,  144,   36)>>14

#if 0
#define GAUSS_F7X7(A, ai, B)    *(B) = GAUSS_FTX3(A, ai, , 0.399050f, 0.242036f, 0.0540056f, 0.00443305f)
#define GAUSS2_F13X13(A, ai, B)  *(B) = GAUSS_FTX6(A, ai, , 0.199676f, 0.176213f, 0.121109f, 0.0648252f, 0.0270232f, 0.00877314f, 0.00221820f)
#else
#define GAUSS_F7X7(A, i, B)    *(B) = ((0.242036f) * (*(A-  (i)) + *(A+  (i))) + ((0.399050f) * (*(A))) +  \
    (0.0540056f) * (*(A-2*(i)) + *(A+2*(i))) + (0.00443305f) * (*(A-3*(i)) + *(A+3*(i))) )

#define GAUSS2_F13X13(A, i, B)  *(B) = ((0.176213f) * (*(A-  (i)) + *(A+  (i))) + ((0.199676f) * (*(A))) + \
    (0.121109f) * (*(A-2*(i)) + *(A+2*(i))) +  (0.0648252f) * (*(A-3*(i)) + *(A+3*(i))) + \
    (0.0270232f) * (*(A-4*(i)) + *(A+4*(i))) + (0.00877314f) * (*(A-5*(i)) + *(A+5*(i))) + \
    (0.00221820f) * (*(A-6*(i)) + *(A+6*(i))))
#endif

#define GAUSS_I7X7(A, i, B)    *(B) = (int)((1983) * (*(A-  (i)) + *(A+  (i))) + ((3269) * (*(A))) +  \
    (442) * (*(A-2*(i)) + *(A+2*(i))) + (36) * (*(A-3*(i)) + *(A+3*(i))) )>>13

#define GAUSS2_I13X13(A, i, B)  *(B) = (int)((1444) * (*(A-  (i)) + *(A+  (i))) + ((1636) * (*(A))) + \
    (992) * (*(A-2*(i)) + *(A+2*(i))) +  (531) * (*(A-3*(i)) + *(A+3*(i))) + \
    (221) * (*(A-4*(i)) + *(A+4*(i))) + (72) * (*(A-5*(i)) + *(A+5*(i))) + \
    (18) * (*(A-6*(i)) + *(A+6*(i))))>>13

// 玻璃飞溅效果
// [cx, cy] 为小方块边长, 在[2..100]
#define GLASS_SPLASH(A, al, ai, B, CX, CY)                                    \
  do {                                                                          \
    int kk, k=0, x, y, _i;                                                      \
    int area = CX*CY;                                                           \
    if (area) {                                                                 \
      kk = rand() % area;                                                       \
      x = kk%CX-CX/2;                                                           \
      y = kk/CX-CY/2;                                                           \
      k = x*ai+y*al;                                                            \
    }                                                                           \
    COLOR_FOR(A, ai, B, _i) {                                                   \
      *(B+_i) = *(A+k+_i);                                                      \
    }                                                                           \
  } while(0)

#define DILATE_3x3(A, al, ai, B, N00, N01, N02, N10, N11, N12, N20, N21, N22) \
  do {                                                                          \
    int _i, wl=al+ai, wi=al-ai;                                                 \
    COLOR_FOR(A, ai, B, _i) {                                                   \
      *B = MAX9(A[-wl]+N00, A[-al]+N01, A[-wi]+N02, A[-ai]+N10,                 \
          A[0]+N11, A[ai]+N12, A[wi]+N20, A[al]+N21, A[wl]+N22);                  \
    }                                                                           \
  } while(0)

#define ERODE_3x3(A, al, ai, B, N00, N01, N02, N10, N11, N12, N20, N21, N22)  \
  do {                                                                          \
    int _i, wl=al+ai, wi=al-ai;                                                 \
    COLOR_FOR(A, ai, B, _i) {                                                   \
      *B = MIN9(A[-wl]-N00, A[-al]-N01, A[-wi]-N02, A[-ai]-N10,                 \
          A[0]-N11, A[ai]-N12, A[wi]-N20, A[al]-N21, A[wl]-N22);                  \
    }                                                                           \
  } while(0)

#define DILATE_RECT_3x3(A, al, ai, B) DILATE_3x3(A, al, ai, B,1,1,1,1,1,1,1,1,1)
#define ERODE_RECT_3x3(A, al, ai, B) ERODE_3x3(A, al, ai, B,1,1,1,1,1,1,1,1,1)

#define DILATE_CROSS_3x3(A, al, ai, B)                                        \
  do {                                                                          \
    int _i;                                                                     \
    COLOR_FOR(A, ai, B, _i) {                                                   \
      *B = MAX5(A[-al]+1, A[-ai]+1, A[0]+1, A[ai]+1, A[al]+1);                  \
    }                                                                           \
  } while(0)

#define ERODE_CROSS_3x3(A, al, ai, B)                                         \
  do {                                                                          \
    int _i;                                                                     \
    COLOR_FOR(A, ai, B, _i) {                                                   \
      *B = MIN5(A[-al]-1, A[-ai]-1, A[0]-1, A[ai]-1, A[al]-1);                  \
    }                                                                           \
  } while(0)


#ifdef __cplusplus
extern "C" {
#endif


  int Spherize_trans(int height, int width, unsigned char* src, int src_step, int src_cn,
      int x, int y, int R2, double m_Rate);


  int UnSpherize_trans(int height, int width, unsigned char* src, int src_step, int src_cn,
      int x, int y, int nRadius, double m_dRate);

#ifdef __cplusplus
}
#endif






#endif // _FILTERS_H_
