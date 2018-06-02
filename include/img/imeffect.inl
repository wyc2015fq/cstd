
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "macro.h"
#include "filters.h"

// 玻璃飞溅
// radius - 玻璃飞溅半径
void glass_splash(int h, int w, const unsigned char* A, int al, int ai,
    unsigned char* B, int bl, int bi, int cx, int cy)
{
  int h2 = h + cy * 2 + 1, w2 = w + cx * 2 + 1;
  int al2 = (w2) * ai;
  unsigned char* data = (unsigned char*) pmalloc(h2 * al2);
  unsigned char* data2;
  COPY_REPLICATE_BORDER(h, w, A, al, h2, w2, data, al2, ai, cy, cx);
  data2 = data + al2 * cy + cx * ai;
#define GLASS_SPLASH_FT(A, al, ai, B) GLASS_SPLASH(A, al, ai, B, cx, cy)

  SKIP_FILTER(h, w, data2, al2, ai, B, bl, bi, GLASS_SPLASH_FT);
#undef GLASS_SPLASH_FT

  pfree(data);
}


void skip_mosaic2(int h, int w, const unsigned char* A, int al, int ai,
    unsigned char* B, int bl, int bi)
{
  int i, j, k;
  int al2 = al << 1;
  int bl2 = bl << 1;
  int ai2 = ai << 1;
  int bi2 = bi << 1;
  int avg;
  const unsigned char* As;
  unsigned char* Bs;

  for (i = 2; i <= h; i += 2, A += al2, B += bl2) {
    for (j = 2, As = A, Bs = B; j <= w; j += 2, As += ai2, Bs += bi2) {
      for (k = 0; k < bi; ++k) {
        avg = (As[ k ] + As[ k + ai ] + As[ k + al ] + As[ k + al + ai ]) >> 2;
        Bs[ k ] = Bs[ k + bi ] = Bs[ k + bl ] = Bs[ k + bl + bi ] = avg;
      }
    }

    if (j != w + 2) {
      for (k = 0; k < bi; ++k) {
        avg = (As[ k ] + As[ k + al ]) >> 1;
        Bs[ k ] = Bs[ k + bl ] = avg;
      }
    }
  }

  if (i != h + 2) {
    for (j = 2, As = A, Bs = B; j <= w; j += 2, As += ai2, Bs += bi2) {
      for (k = 0; k < bi; ++k) {
        avg = (As[ k ] + As[ k + ai ]) >> 1;
        Bs[ k ] = Bs[ k + bi ] = avg;
      }
    }

    if (j != w + 2) {
      for (k = 0; k < bi; ++k) {
        Bs[ k ] = As[ k ];
      }
    }
  }
}

// 马赛克效果。
// cx, cy:：小正方形边长
// 适用于 ：24,32 bit
void skip_mosaic(int h, int w, const unsigned char* A, int al, int ai,
    unsigned char* B, int bl, int bi, int cx, int cy)
{
  int i, j, k, iy, ix, al1, bl1, ai1, bi1, cnt, cnt1;
  const unsigned char* As;
  unsigned char* Bs;
  int avg;

  if (cx * cy < 2) {
    SKIP_COPY(h, w, A, al, ai, B, bl, bi);
    return ;
  }

  if (2 == cx && 2 == cy) {
    skip_mosaic2(h, w, A, al, ai, B, bl, bi);
    return ;
  }

  assert(ai >= bi);
  iy = h % cy;
  ix = w % cx;
  al1 = al * cy;
  bl1 = bl * cy;
  ai1 = ai * cx;
  bi1 = bi * cx;
  cnt = cx * cy;
  cnt1 = cy * ix;

  for (i = cy; i <= h; i += cy, A += al1, B += bl1) {
    for (j = cx, As = A, Bs = B; j <= w; j += cx, As += ai1, Bs += bi1) {
      for (k = 0; k < bi; ++k) {
        int sum = 0;
        SKIP_SUM(cy, cx, As + k, al, ai, sum);
        avg = sum / cnt;
        SKIP_FILL(cy, cx, Bs + k, bl, bi, avg);
      }
    }

    if (cnt1) {
      for (k = 0; k < bi; ++k) {
        int sum = 0;
        SKIP_SUM(cy, ix, As + k, al, ai, sum);
        avg = sum / cnt1;
        SKIP_FILL(cy, ix, Bs + k, bl, bi, avg);
      }
    }
  }

  cnt1 = iy * cx;

  if (cnt1) {
    for (j = cx, As = A, Bs = B; j <= w; j += cx, As += ai1, Bs += bi1) {
      for (k = 0; k < bi; ++k) {
        int sum = 0;
        SKIP_SUM(iy, cx, As + k, al, ai, sum);
        avg = sum / cnt1;
        SKIP_FILL(iy, cx, Bs + k, bl, bi, avg);
      }
    }

    cnt1 = iy * ix;

    if (cnt1) {
      for (k = 0; k < bi; ++k) {
        int sum = 0;
        SKIP_SUM(iy, ix, As + k, al, ai, sum);
        avg = sum / cnt1;
        SKIP_FILL(iy, ix, Bs + k, bl, bi, avg);
      }
    }
  }

  return ;
}

//风化效果。（使用平均算法）
//nStep ：风化程度，必须 >=1 。
//nDirection ：DIRECT_RIGHT
//　　　　　　 DIRECT_LEFT
//bSmooth ：柔和风化。
//内部调用WindySoftLine进行行运算。
//适用于 ：24,32 bit
void skip_windy(int h, int w, const unsigned char* A, int al, int ai,
    unsigned char* B, int bl, int bi, int nStep0, int bSmooth)
{
  int y, k;
  const unsigned char* As;
  unsigned char* Bs;
  int nDirection = nStep0;
  nStep0 = nStep0 > 0 ? nStep0 : -nStep0;
  ++nStep0;

  if (nStep0 > w) {
    nStep0 = w - 1;
  }

  for (y = 0; y < h; ++y, A += al, B += bl) {
    for (k = 0; k < bi; ++k) {
      int nStep, iDiv, nSpan, rgb, pix, iBlk, m, iXmax, x;
      nStep = bSmooth ? nStep0 : 1 + (rand() % nStep0);

      if (nStep < 1) {
        nStep = 1;
      }

      iDiv = nStep + 1; // 平均象素个数
      nSpan = bi; // 每象素字节数3, 4
      pix = 0; // 边缘象素值

      if (nDirection > 0) {
        rgb = A[(w - 1) * ai ];
        Bs = B + k;
        As = A + k;
      }
      else {
        rgb = A[ 0 ];
        Bs = B + k + (w - 1) * bi;
        As = A + k + (w - 1) * ai;
        nSpan = -nSpan;
      }

      iBlk = iDiv * nSpan; // 块长度

      for (m = 0; m < iDiv; ++m) {
        pix += As[ m * nSpan ];
      }

      Bs[ 0 ] = pix / iDiv;

      //    移动方阵
      iXmax = w - nStep - 1;

      for (x = 0; x < w - 2; x++) {   // "-2"两边象素不用处理
        if (x >= iXmax) {   // 处理右边缘象素
          pix += -Bs[ 0 ] + rgb;
        }
        else {
          pix += -Bs[ 0 ] + As[ iBlk ];
        }

        Bs += nSpan;
        As += nSpan;
        Bs[ 0 ] = MAX(0, MIN(0xFF, pix / iDiv));
      }
    }
  }

  return ;
}

// 带状效果。
// iSwing ：振幅[0..100]
// iFrequency ：频率 >=0
// 适用于 ：24,32 bit
void skip_stripe(int h, int w, const unsigned char* A, int al, int ai,
    unsigned char* B, int bl, int bi, int iSwing, int iFrequency)
{
  int i;
  int wi = w * bi;
  double ang = 3.14 * iSwing / h;

  for (i = 0; i < h; ++i, A += al, B += bl) {
    int k = (int)(cos(ang * i) * iFrequency), j;
    k = k >= w ? (k % w) : k;
    k = k < 0 ? (k + w) : k;

    for (j = 0; j < bi; ++j) {
      SHIFT_COPY(w, k, (A + j), ai, (B + j), bi);
    }
  }
}

// 柱面效果
void skip_column(int h, int w, const unsigned char* A, int al, int ai,
    unsigned char* B, int bl, int bi)
{
  int i;
  int wi = w * ai;
  double R = (w % 2 == 0) ? (w - 1) / 2 : w / 2;
  int dwNewMid = (w % 2 == 0) ? (w - 1) / 2 : w / 2;
  const unsigned char* As;
  unsigned char* A0 = (unsigned char*) A;
  unsigned char* B0 = (unsigned char*) B;

  if (A == B) {
    A0 = MALLOC(unsigned char, h * al);
    MEMCPY(A0, A, h * al);
  }

  for (i = 0; i < h; ++i, B0 += bl) {
    int dwIndex = dwNewMid - (int)(R * asin((R - i) / R));
    dwIndex = dwIndex >= h ? (dwIndex % h) : dwIndex;
    dwIndex = dwIndex < 0 ? (dwIndex + h) : dwIndex;
    As = A0 + (dwIndex * al);
    COPY(wi, As, B0);
  }

  if (A == B) {
    pfree(A0);
  }

  return ;
}

typedef struct tagSPHERE {
  int x, y;
  int radius;
  double rate;
}
SPHERE, PSPHERE;

#include "macro.h"
#include "macro.h"
#include "imresize.h"

#define BOUND_NUL(x, min, max)

#define SPHERE_TR(co, _J, _I, _X, _Y)                                         \
  do {                                                                          \
    double rate2 = (co).rate>0.?(co).rate : -(co).rate;                         \
    if (rate2>0.01) {                                                           \
      double zoom = 1.;                                                         \
      int _dx = _J-(co).x;                                                      \
      int _dy = _I-(co).y;                                                      \
      int src_radius2 = _dx*_dx + _dy*_dy;                                      \
      int radius2 = (co).radius*(co).radius;                                    \
      double zoom2;                                                             \
      if (src_radius2<radius2) {                                                \
        if ((co).rate<0) {                                                      \
          zoom2 = (radius2/ ((src_radius2-radius2)*rate2+radius2));             \
        } else {                                                                \
          zoom2 = (((src_radius2-radius2)*rate2+radius2)/ radius2);             \
        }                                                                       \
        zoom = sqrt(zoom2);                                                     \
        _X = SHIFT_TOINT((co).x + (int)(_dx*zoom));                             \
        _Y = SHIFT_TOINT((co).y + (int)(_dy*zoom));                             \
        break ;                                                                 \
      }                                                                         \
    }                                                                           \
    _X = SHIFT_TOINT(_J);                                                       \
    _Y = SHIFT_TOINT(_I);                                                       \
  } while(0)

void sphere(int h, int w, const unsigned char* A, int al, int ai,
    unsigned char* B, int bl, int bi, int x, int y, int radius, double rate)
{
  SPHERE aa = { x, y, radius, (rate) };
  unsigned char* A0 = (unsigned char*) A;
  unsigned char* B0 = (unsigned char*) B;

  if (A == B) {
    A0 = MALLOC(unsigned char, h * al);
    MEMCPY(A0, A, h * al);
  }

  __IMAFFINE(h, w, A0, al, ai, h, w, B0, bl, bi, SPHERE_TR, aa, INTER_BILINEARITY, BOUND_NUL);

  if (A == B) {
    pfree(A0);
  }
}
