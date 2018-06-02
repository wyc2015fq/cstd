
#include <stdlib.h>
#include <string.h>

int imMedian(int height, int width, unsigned char* src, unsigned char* des, int num);

int imFilter(int sHeight, int sWidth, unsigned char* src, int src_step, unsigned char* des, int des_step,
    int fHeight, int fWidth, double* flt, int flt_step, int midLevel);

#define FILTER2D_DO(_H, _W, _A, _AL, _AI, _B, _BL, _BI, _DO) do { int _IA, _J;  for (_IA=0; _I<_H*_AL; _I+=, } while(0)

#define BEGIN_FILTER2D(h, w, src, sl, dst, dl, cn, y, x, cy, cx)  \
  do { \
    const unsigned char* srcex = NULL; \
    srcex = memcpy2dex(NULL, (w+cx-1)*cn, h+cy-1, w+cx-1, src, sl, h, w, cn, y, x); \
    sl = (w+cx-1)*cn; \
    src = srcex+y*sl+x*cn;

#define END_FILTER2D() \
  pfree((void*)srcex); \
  } while(0)

int imWaterColor(int src_height, int src_width, unsigned char* src, int src_step, int N)
{
  int   num = 2 * N + 1;
  double h[9] = {
    0, -1, 0,
    -1, 5, -1,
    0, -1, 0
  };
  unsigned char* Temp = (unsigned char*) pmalloc(sizeof(unsigned char) * 3 * src_height * src_width);

  if (Temp == NULL) {
    return 0;
  }

  imMedian(src_height, src_width, src, Temp, num);
  imFilter(src_height, src_width, Temp, src_width, src, src_width, 3, 3, h, 3, 0);
  pfree(Temp);
  return 1;
}



int* makeidx(int* idx, int y, int x, int cy, int cx, int al, int ai)
{
  int i, j, k = 0;

  if (NULL == idx) {
    idx = (int*)pmalloc(cy * cx * sizeof(int));
  }

  for (i = -y; i < cy - y; ++i) {
    for (j = -x; j < cx - x; ++j) {
      idx[k++] = i * al + j * ai;
    }
  }

  return idx;
}

void* memcpy2dex(void* dst, int dl, int dh, int dw,
    const void* src, int sl, int sh, int sw,
    int cn, int y, int x);

unsigned char* immedian(int h, int w, const unsigned char* src, int sl,
    unsigned char* dst, int dl, int cn, int y, int x, int cy, int cx)
{
  const unsigned char* srcex = NULL;
  int* idx = NULL;
  int i, j, k;
  int K = cy * cx;
  int K2 = K / 2;
  int hist[256];
  unsigned char* buf = (unsigned char*)pmalloc(K);
  srcex = memcpy2dex(NULL, (w + cx - 1) * cn, h + cy - 1, w + cx - 1, src, sl, h, w, cn, y, x);
  sl = (w + cx - 1) * cn;
  src = srcex + y * sl + x * cn;
  idx = makeidx(NULL, y, x, cy, cx, sl, cn);

  for (i = 0; i < h; ++i, dst += dl, src += sl) {
    for (j = 0; j < w * cn; ++j) {
#if 1
      int t;
      memset(hist, 0, 256 * sizeof(hist[0]));

      for (k = 0; k < K; ++k) {
        hist[src[j + idx[k]]]++;
      }

      for (t = 0, k = 0; (t < K / 2) && k < 256; ++k) {
        t += hist[k];
      }

      dst[j] = k - 1;
#else

      int m = 0;

      for (m = 0; m < K; ++m) {
        buf[m] = src[j + idx[m]];
      }

      for (k = 0; k < K2 + 1; ++k) {
        for (m = k + 1; m < K; ++m) {
          if (buf[k] < buf[m]) {
            unsigned char t;
            t = buf[k], buf[k] = buf[m], buf[m] = t;
          }
        }
      }

      dst[j] = buf[K2];
#endif

    }
  }

  pfree(buf);
  pfree(idx);
  pfree((void*)srcex);
  return 0;
}

unsigned char* immax(int h, int w, const unsigned char* src, int sl,
    unsigned char* dst, int dl, int cn, int y, int x, int cy, int cx)
{
  const unsigned char* srcex = NULL;
  int* idx = NULL;
  int i, j, k, t;
  int K = cy * cx;
  int hist[256];
  unsigned char* buf = (unsigned char*)pmalloc(K);
  srcex = memcpy2dex(NULL, (w + cx - 1) * cn, h + cy - 1, w + cx - 1, src, sl, h, w, cn, y, x);
  sl = (w + cx - 1) * cn;
  src = srcex + y * sl + x * cn;
  idx = makeidx(NULL, y, x, cy, cx, sl, cn);

  for (i = 0; i < h; ++i, dst += dl, src += sl) {
    for (j = 0; j < w * cn; ++j) {
      int kmax = 0;
      memset(hist, 0, 256 * sizeof(hist[0]));

      for (k = 0; k < K; ++k) {
        hist[src[j + idx[k]]]++;
      }

      for (t = 0, k = 0; k < 256; ++k) {
        if (hist[k] > t) {
          t = hist[k];
          kmax = k;
        }
      }

      dst[j] = kmax;
    }
  }

  pfree(buf);
  pfree(idx);
  pfree((void*)srcex);
  return dst;
}

unsigned char* imwatercolor(int h, int w, const unsigned char* src, int sl, unsigned char* dst, int dl, int cn, int N)
{
  int i, j;
  immedian(h, w, src, sl, dst, dl, cn, N, N, N * 2 + 1, N * 2 + 1);
  src = dst;
  sl = dl;
  BEGIN_FILTER2D(h, w, src, sl, dst, dl, cn, 1, 1, 3, 3);

  for (i = 0; i < h; ++i, dst += dl, src += sl) {
    for (j = 0; j < w * cn; ++j) {
      int a, b;
      a = src[j];
      a += a << 2;
      b = src[j - sl] + src[j - cn] + src[j + cn] + src[j + sl];
      a -= b;
      dst[j] = ((a > 0) ? (a < 255 ? a : 255) : 0);
    }
  }

  END_FILTER2D();
  return dst;
}
