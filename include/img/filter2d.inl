
#include <stdlib.h>
#include <math.h>
#include <string.h>

//#include "xtypes.h"
#define ESP (0.0000001)
#define ISZERO(_X)  (fabs(_X) < ESP)
static int copy_replicate_border(int ah, int aw, const void* src0, int srcstep, int bh, int bw, void* dst0, int dststep, int cn, int top1, int left1)
{
  int i, j, iA = 0, iB = 0;
  const uchar* src = (const uchar*) src0;
  uchar* dst = (uchar*) dst0;
  int left = (left1) * (cn), top = top1;
  int aw1 = (aw) * (cn), ah1 = ah;
  int bw1 = (bw) * (cn), bh1 = bh;

  for (i = 0; i < bh1; i++, iB += (dststep)) {
    memcpy(dst + iB + left, src + iA, (aw1));

    for (j = left - 1; j >= 0; j--) {
      *(dst + iB + j) = *(dst + iB + j + cn);
    }

    for (j = left + aw1; j < (bw1); j++) {
      *(dst + iB + j) = *(dst + iB + j - cn);
    }

    if (i >= top && i < top + ah1 - 1) {
      iA += srcstep;
    }
  }

  return 0;
}

static int filter_f8(int h, int w, const uchar* src, int srcstep, int srccn, double* dst, int dststep, int dstcn, int ny, int nx, const double* kk, int kl)
{
  int i, j, x, y, i2, j2;
  int ny2 = ny / 2, nx2 = nx / 2;
  double t;

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      t = 0;

      for (y = 0; y < ny; ++y) {
        i2 = i - ny2 + y;
        i2 = BOUND(i2, 0, h - 1);

        for (x = 0; x < ny; ++x) {
          j2 = j - nx2 + x;
          j2 = BOUND(j2, 0, w - 1);
          t += kk[y * kl + x] * src[i2 * srcstep + j2];
        }
      }

      dst[j] = t;
    }

    dst += dststep;
  }

  return 0;
}

// 矩形框滤波
// ky[1, ny] 列向量
// kx[nx, 1] 行向量
static int filter_rect(int h, int w, const uchar* src, int srcstep, int srccn, uchar* dst, int dststep, int dstcn, int* buf, int ny, const int* ky, int nx, const int* kx, int shift)
{
  int i, j, k, t, i2;
  int ny2 = ny / 2, nx2 = nx / 2;

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      for (t = 0, k = 0; k < ny; ++k) {
        i2 = i - ny2 + k;
        i2 = BOUND(i2, 0, h - 1);
        t += ky[k] * src[i2 * srcstep + j];
      }

      buf[j] = t >> shift;
    }

    for (j = 0; j < w; ++j) {
      t = 0;

      for (k = 0; k < nx; ++k) {
        i2 = j - nx2 + k;
        i2 = BOUND(i2, 0, w - 1);
        t += kx[k] * buf[i2];
      }

      dst[j] = t >> shift;
    }

    dst += dststep;
  }

  return 0;
}

// 对称矩形框滤波
static int filter_rect_symmetric(int h, int w, const uchar* src, int srcstep, int srccn, uchar* dst, int dststep, int dstcn, int ny, const int* ky, int nx, const int* kx, int shift)
{
  int i, j, k, i2, i3, s2 = 1<<(shift-1);
  int* buf = MALLOC(int, w+nx);

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      int t = ky[0] * src[i * srcstep + j];

      for (k = 1; k < ny; ++k) {
        i2 = i + k, i3 = i - k;
        i2 = BOUND(i2, 0, h - 1);
        i3 = BOUND(i3, 0, h - 1);
        t += ky[k] * (src[i2 * srcstep + j] + src[i3 * srcstep + j]);
      }

      buf[j] = (t + s2)>>shift;
    }

    for (j = 0; j < w; ++j) {
      int t = kx[0] * buf[j];

      for (k = 1; k < nx; ++k) {
        i2 = j + k, i3 = j - k;
        i2 = BOUND(i2, 0, w - 1);
        i3 = BOUND(i3, 0, w - 1);
        t += kx[k] * (buf[i2] + buf[i3]);
      }

      dst[j] = (t + s2)>>shift;
    }

    dst += dststep;
  }

  FREE(buf);
  return 0;
}
static int filter_rect_symmetric_i4(int h, int w, const uchar* src, int srcstep, int srccn, int* dst, int dststep, int dstcn, int* buf, int ny, const int* ky, int nx, const int* kx, int shift)
{
  int i, j, k, t, i2, i3;

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      t = ky[0] * src[i * srcstep + j];

      for (k = 1; k < ny; ++k) {
        i2 = i + k, i3 = i - k;
        i2 = BOUND(i2, 0, h - 1);
        i3 = BOUND(i3, 0, h - 1);
        t += ky[k] * (src[i2 * srcstep + j] + src[i3 * srcstep + j]);
      }

      buf[j] = t;
    }

    for (j = 0; j < w; ++j) {
      t = kx[0] * buf[j];

      for (k = 1; k < nx; ++k) {
        i2 = j + k, i3 = j - k;
        i2 = BOUND(i2, 0, w - 1);
        i3 = BOUND(i3, 0, w - 1);
        t += kx[k] * (buf[i2] + buf[i3]);
      }

      dst[j] = t;
    }

    dst += dststep;
  }

  return 0;
}

static int filter_rect_symmetric_f8(int h, int w, const uchar* src, int srcstep, int srccn, uchar* dst, int dststep, int dstcn, double* buf, int ny, const double* ky, int nx, const double* kx)
{
  int i, j, k, i2, i3;
  double t;

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      t = ky[0] * src[i * srcstep + j];

      for (k = 1; k < ny; ++k) {
        i2 = i + k, i3 = i - k;
        i2 = BOUND(i2, 0, h - 1);
        i3 = BOUND(i3, 0, h - 1);
        t += ky[k] * (src[i2 * srcstep + j] + src[i3 * srcstep + j]);
      }

      buf[j] = t;
    }

    for (j = 0; j < w; ++j) {
      t = kx[0] * buf[j];

      for (k = 1; k < nx; ++k) {
        i2 = j + k, i3 = j - k;
        i2 = BOUND(i2, 0, w - 1);
        i3 = BOUND(i3, 0, w - 1);
        t += kx[k] * (buf[i2] + buf[i3]);
      }

      dst[j] = (int)BOUND(t, 0, 255);
    }

    dst += dststep;
  }

  return 0;
}

// 二值滤波
static int filter_binary(int h, int w, const uchar* src, int srcstep, int srccn, uchar* dst, int dststep, int dstcn, int x1, int y1, int x2, int y2, int is3)
{
  int i, j, t, i1, j1, i2, j2;

  for (i = 0; i < h; ++i) {
    memset(dst + i * dststep, 0, (w + 7) >> 3);
  }

  if (is3) {
    for (i = 0; i < h; ++i) {
      uchar* B1 = dst + i * dststep;

      for (j = 0; j < w; ++j) {
        i1 = i + y1, j1 = j + x1, i2 = i + y2, j2 = j + x2;
        i1 = BOUND(i1, 0, h - 1);
        i2 = BOUND(i2, 0, h - 1);
        j1 = BOUND(j1, 0, w - 1);
        j2 = BOUND(j2, 0, w - 1);
        t = (src[i1 * srcstep + j1] + src[i2 * srcstep + j2]) > (src[i * srcstep + j] << 1);
        //B1[j>>3] |= t<<(j&7);
        B1[j] = t ? 255 : 0;
      }
    }
  }
  else {
    for (i = 0; i < h; ++i) {
      uchar* B1 = dst + i * dststep;

      for (j = 0; j < w; ++j) {
        i1 = i + y1, j1 = j + x1, i2 = i + y2, j2 = j + x2;
        i1 = BOUND(i1, 0, h - 1);
        i2 = BOUND(i2, 0, h - 1);
        j1 = BOUND(j1, 0, w - 1);
        j2 = BOUND(j2, 0, w - 1);
        t = (src[i1 * srcstep + j1] > src[i2 * srcstep + j2]);
        //B1[j>>3] |= t<<(j&7);
        B1[j] = t ? 255 : 0;
      }
    }
  }

  return 0;
}

static int filter_binary_i4(int h, int w, const int* src, int srcstep, int srccn, uchar* dst, int dststep, int dstcn, int x1, int y1, int x2, int y2, int is3)
{
  int i, j, t, i1, j1, i2, j2;

  for (i = 0; i < h; ++i) {
    memset(dst + i * dststep, 0, (w + 7) >> 3);
  }

  if (is3) {
    for (i = 0; i < h; ++i) {
      uchar* B1 = dst + i * dststep;

      for (j = 0; j < w; ++j) {
        i1 = i + y1, j1 = j + x1, i2 = i + y2, j2 = j + x2;
        i1 = BOUND(i1, 0, h - 1);
        i2 = BOUND(i2, 0, h - 1);
        j1 = BOUND(j1, 0, w - 1);
        j2 = BOUND(j2, 0, w - 1);
        t = (src[i1 * srcstep + j1] + src[i2 * srcstep + j2]) > (src[i * srcstep + j] << 1);
        //B1[j>>3] |= t<<(j&7);
        B1[j] = t ? 255 : 0;
      }
    }
  }
  else {
    for (i = 0; i < h; ++i) {
      uchar* B1 = dst + i * dststep;

      for (j = 0; j < w; ++j) {
        i1 = i + y1, j1 = j + x1, i2 = i + y2, j2 = j + x2;
        i1 = BOUND(i1, 0, h - 1);
        i2 = BOUND(i2, 0, h - 1);
        j1 = BOUND(j1, 0, w - 1);
        j2 = BOUND(j2, 0, w - 1);
        t = (src[i1 * srcstep + j1] > src[i2 * srcstep + j2]);
        //B1[j>>3] |= t<<(j&7);
        B1[j] = t ? 255 : 0;
      }
    }
  }

  return 0;
}
static int filter_binary_f8(int h, int w, const double* src, int srcstep, int srccn, uchar* dst, int dststep, int dstcn, int x1, int y1, int x2, int y2, int is3)
{
  int i, j, t, i1, j1, i2, j2;

  for (i = 0; i < h; ++i) {
    memset(dst + i * dststep, 0, (w + 7) >> 3);
  }

  if (is3) {
    for (i = 0; i < h; ++i) {
      uchar* B1 = dst + i * dststep;

      for (j = 0; j < w; ++j) {
        i1 = i + y1, j1 = j + x1, i2 = i + y2, j2 = j + x2;
        i1 = BOUND(i1, 0, h - 1);
        i2 = BOUND(i2, 0, h - 1);
        j1 = BOUND(j1, 0, w - 1);
        j2 = BOUND(j2, 0, w - 1);
        t = (src[i1 * srcstep + j1] + src[i2 * srcstep + j2]) > (src[i * srcstep + j] * 2);
        //B1[j>>3] |= t<<(j&7);
        B1[j] = t ? 255 : 0;
      }
    }
  }
  else {
    for (i = 0; i < h; ++i) {
      uchar* B1 = dst + i * dststep;

      for (j = 0; j < w; ++j) {
        i1 = i + y1, j1 = j + x1, i2 = i + y2, j2 = j + x2;
        i1 = BOUND(i1, 0, h - 1);
        i2 = BOUND(i2, 0, h - 1);
        j1 = BOUND(j1, 0, w - 1);
        j2 = BOUND(j2, 0, w - 1);
        t = (src[i1 * srcstep + j1] > src[i2 * srcstep + j2]);
        //B1[j>>3] |= t<<(j&7);
        B1[j] = t ? 255 : 0;
      }
    }
  }

  return 0;
}
// 二值滤波 bit压缩
static int filter_binary_bit(int h, int w, const uchar* src, int srcstep, int srccn, uchar* dst, int dststep, int dstcn, int x1, int y1, int x2, int y2, int is3)
{
  int i, j, t, i1, j1, i2, j2;

  for (i = 0; i < h; ++i) {
    memset(dst + i * dststep, 0, (w + 7) >> 3);
  }

  if (is3) {
    for (i = 0; i < h; ++i) {
      uchar* B1 = dst + i * dststep;

      for (j = 0; j < w; ++j) {
        i1 = i + y1, j1 = j + x1, i2 = i + y2, j2 = j + x2;
        i1 = BOUND(i1, 0, h - 1);
        i2 = BOUND(i2, 0, h - 1);
        j1 = BOUND(j1, 0, w - 1);
        j2 = BOUND(j2, 0, w - 1);
        t = (src[i1 * srcstep + j1] + src[i2 * srcstep + j2]) > (src[i * srcstep + j] << 1);
        B1[j >> 3] |= t << (j & 7);
      }
    }
  }
  else {
    for (i = 0; i < h; ++i) {
      uchar* B1 = dst + i * dststep;

      for (j = 0; j < w; ++j) {
        i1 = i + y1, j1 = j + x1, i2 = i + y2, j2 = j + x2;
        i1 = BOUND(i1, 0, h - 1);
        i2 = BOUND(i2, 0, h - 1);
        j1 = BOUND(j1, 0, w - 1);
        j2 = BOUND(j2, 0, w - 1);
        t = (src[i1 * srcstep + j1] > src[i2 * srcstep + j2]);
        B1[j >> 3] |= t << (j & 7);
      }
    }
  }

  return 0;
}

static int filter_binary_bit_i4(int h, int w, const int* src, int srcstep, int srccn, uchar* dst, int dststep, int dstcn, int x1, int y1, int x2, int y2, int is3)
{
  int i, j, t, i1, j1, i2, j2;

  for (i = 0; i < h; ++i) {
    memset(dst + i * dststep, 0, (w + 7) >> 3);
  }

  if (is3) {
    y1 -= 2, y2 += 2;

    for (i = 0; i < h; ++i) {
      uchar* B1 = dst + i * dststep;

      for (j = 0; j < w; ++j) {
        i1 = i + y1, j1 = j + x1, i2 = i + y2, j2 = j + x2;
        i1 = BOUND(i1, 0, h - 1);
        i2 = BOUND(i2, 0, h - 1);
        j1 = BOUND(j1, 0, w - 1);
        j2 = BOUND(j2, 0, w - 1);
        t = (src[i1 * srcstep + j1] + src[i2 * srcstep + j2]) > (src[i * srcstep + j] << 1);
        B1[j >> 3] |= t << (j & 7);
      }
    }
  }
  else {
    for (i = 0; i < h; ++i) {
      uchar* B1 = dst + i * dststep;

      for (j = 0; j < w; ++j) {
        i1 = i + y1, j1 = j + x1, i2 = i + y2, j2 = j + x2;
        i1 = BOUND(i1, 0, h - 1);
        i2 = BOUND(i2, 0, h - 1);
        j1 = BOUND(j1, 0, w - 1);
        j2 = BOUND(j2, 0, w - 1);
        t = (src[i1 * srcstep + j1] > src[i2 * srcstep + j2]);
        B1[j >> 3] |= t << (j & 7);
      }
    }
  }

  return 0;
}

// 构造高斯核
static int make_guass_kernel(double delta, double* output, int n)
{
  int i, size = (int)(delta * 10);

  if (output) {
    double ss = output[0] = 1;

    for (i = 1; i < n && output[i - 1] > 0.001; ++i) {
      output[i] = exp(-i * i / (2 * delta * delta));
      ss += output[i] * 2;
    }

    size = MIN(n, i);

    for (i = 0; i < size; ++i) {
      output[i] /= ss;
    }
  }

  return size;
}

static int make_guass_kernel_2d(double delta, double* output, int n)
{
  double kk0[100] = {0};
  double kk[100] = {0};
  double d, ss;
  int i, j, nn, nk = make_guass_kernel(delta, kk0, n);
  nn = nk * 2 - 1;

  for (i = nn - 1; i >= 0; --i) {
    kk[i] = kk0[abs(i - nk + 1)];
  }

  ss = 0;

  for (i = 0; i < nn; ++i) {
    for (j = 0; j < nn; ++j) {
      output[i * nn + j] = d = kk[i] * kk[j];
      ss += d;
    }
  }

  for (i = 0; i < nn * nn; ++i) {
    output[i] /= ss;
  }

  return nn;
}
static int make_dist_guass_kernel_2d(double delta, double* output, int n, int x, int y)
{
  double kk0[100] = {0};
  double kk[100] = {0};
  int i, j, nn, nk = make_guass_kernel(delta, kk0, n);
  double ki1, kj1, ki2, kj2;
  nk = n;
  nn = nk * 2 - 1;

  for (i = nn - 1; i >= 0; --i) {
    kk[i] = kk0[abs(i - nk + 1)];
  }

  for (i = 0; i < nn; ++i) {
    ki1 = (i - y) < 0 ? 0 : kk[i - y];
    ki2 = (i + y) >= nn ? 0 : kk[i + y];

    for (j = 0; j < nn; ++j) {
      kj1 = (j - x) < 0 ? 0 : kk[j - x];
      kj2 = (j + x) >= nn ? 0 : kk[j + x];
      output[i * nn + j] = ki2 * kj2 - ki1 * kj1;
      //printf("%+.5f,", output[i*nn+j]);
    }

    //printf("\n");
  }

  //printf("\n");
  return nn;
}

static int make_guass_kernel_i(double delta, int* output, int shift)
{
  double d_out[100] = {0};
  int size = make_guass_kernel(delta, d_out, 100);
  int i, ss = 0, one = 1 << shift;

  for (i = 1; i < size; ++i) {
    output[i] = (int)(one * d_out[i] + 0.5);
    ss += output[i] * 2;

    if (0 == output[i]) {
      size = i;
      break;
    }
  }

  output[0] = one - ss;
  return size;
}

static int filter_guass(int h, int w, const uchar* src, int srcstep, int srccn, uchar* dst, int dststep, int dstcn, double delta)
{
  int ik[100] = {0}, shift = 8;
  int nk = make_guass_kernel_i(delta, ik, shift);
  filter_rect_symmetric(h, w, src, srcstep, srccn, dst, dststep, dstcn, nk, ik, nk, ik, shift);
  return 0;
}



// col - [h]
// row - [w]
static const double* decompose_col_row(int h, int w, const double* src, int srcstep)
{
  int i, j;
  const double* col = src;

  for (i = 0; i < w && ISZERO(col[ i ]); ++i, ++col) {}

  for (i = 1; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      if ((src[ i * srcstep + j ] * col[ 0 ]) != (src[ j ] * col[ i * srcstep ])) {
        return 0;
      }
    }
  }

  return col;
}

#define _DECOMPOSE4

static int filter2d(int h, int w, const double* src, int srcstep, int srccn, double* dst, int dststep,
    int kh, int kw, const double* K, int kl, int kx, int ky, int border_type) {
  double* A_buf = 0, *B_buf = 0;
  double* ky_ptr = 0, *kx_ptr = 0;
  int* kx_ptx = 0, *kx_pty = 0;
  int* ky_ptx = 0, *ky_pty = 0;
  int i, j, k, ky_cnt = 0, kx_cnt = 0;
  const double** A_ptr = 0;
  const double* Ky = 0;

  if (1 == border_type) {
    int h2 = h + kh - 1;
    int w2 = w + kw - 1;
    A_buf = (double*) pmalloc(sizeof(double) * h2 * w2 * srccn);
    copy_replicate_border(h, w, src, srcstep * sizeof(double), h2, w2, A_buf, srccn * w2 * sizeof(double), srccn * sizeof(double), ky, kx);
    //COPY(h2*w2, A2, uA);
    //imwrite("c.bmp", h2, w2, uA, w2, 1);
    srcstep = srccn * w2;
    src = A_buf + ky * srcstep + kx * srccn;
  }

  A_ptr = (const double**) pmalloc((sizeof(double*) + sizeof(double) + sizeof(int) * 2) * kh * kw);
  ky_ptr = (double*)(A_ptr + kh * kw);
  ky_ptx = (int*)(ky_ptr + kh * kw);
  ky_pty = (int*)(ky_ptx + kh * kw);

  Ky = decompose_col_row(kh, kw, K, kl);
  Ky = 0;

  if (Ky) {
    int w2 = w + kw - 1;
    B_buf = (double*) pmalloc(sizeof(double) * w2 * srccn);
    kx_ptr = ky_ptr + kh;
    kx_ptx = ky_ptx + kh;
    kx_pty = ky_pty + kh;

    for (i = 0; i < kh; ++i) {
      double kval = *(Ky + i * kl) / Ky[ 0 ];

      if (!ISZERO(kval)) {
        ky_ptr[ ky_cnt ] = kval;
        ky_pty[ ky_cnt ] = i - ky;
        ky_ptx[ ky_cnt ] = 0;
        ++ky_cnt;
      }
    }

    for (i = 0; i < kw; ++i) {
      double kval = *(K + i);

      if (!ISZERO(kval)) {
        kx_ptr[ kx_cnt ] = kval;
        kx_pty[ kx_cnt ] = 0;
        kx_ptx[ kx_cnt ] = i - kx;
        ++kx_cnt;
      }
    }
  }
  else {
    for (i = 0; i < kh; ++i) {
      for (j = 0; j < kw; ++j) {
        double kval = *(K + i * kl + j);

        if (!ISZERO(kval)) {
          ky_ptr[ ky_cnt ] = kval;
          ky_pty[ ky_cnt ] = i - ky;
          ky_ptx[ ky_cnt ] = j - kx;
          ++ky_cnt;
        }
      }
    }
  }

  w *= srccn;

  for (i = 0; i < h; ++i, dst += dststep, src += srcstep) {
    double* B2 = dst;

    for (k = 0; k < ky_cnt; ++k) {
      A_ptr[ k ] = src + (ky_pty[ k ] * srcstep + ky_ptx[ k ] * srccn);
    }

    if (kx_ptr) {
      B2 = B_buf + kx;
    }

    j = 0;
#ifdef _DECOMPOSE4

    for (; j <= w - 4; j += 4) {
      double s0 = 0.f, s1 = 0.f, s2 = 0.f, s3 = 0.f;

      for (k = 0; k < ky_cnt; ++k) {
        const double* aptr = A_ptr[ k ] + j;
        double f = ky_ptr[ k ];
        s0 += f * aptr[ 0 ];
        s1 += f * aptr[ 1 ];
        s2 += f * aptr[ 2 ];
        s3 += f * aptr[ 3 ];
      }

      B2[ j + 0 ] = s0;
      B2[ j + 1 ] = s1;
      B2[ j + 2 ] = s2;
      B2[ j + 3 ] = s3;
    }

#endif

    for (; j < w; ++j) {
      double s0 = 0.f;

      for (k = 0; k < ky_cnt; ++k) {
        const double* aptr = A_ptr[ k ] + j;
        double f = ky_ptr[ k ];
        s0 += f * aptr[ 0 ];
      }

      B2[ j ] = s0;
    }

    if (kx_ptr) {
      if (1 == border_type) {
        for (j = (kx - 1) * srccn; j >= 0; --j) {
          B_buf[ j ] = B_buf[ j + srccn ];
        }

        for (j = w; j < w + (kw - kx - 1) * srccn; ++j) {
          B_buf[ j ] = B_buf[ j - srccn ];
        }
      }

      for (k = 0; k < kx_cnt; ++k) {
        A_ptr[ k ] = src + (kx_ptx[ k ] * srccn);
      }

      j = 0;
#ifdef _DECOMPOSE4

      for (; j <= w - 4; j += 4) {
        double s0 = 0.f, s1 = 0.f, s2 = 0.f, s3 = 0.f;

        for (k = 0; k < kx_cnt; ++k) {
          const double* aptr = A_ptr[ k ] + j;
          double f = kx_ptr[ k ];
          s0 += f * aptr[ 0 ];
          s1 += f * aptr[ 1 ];
          s2 += f * aptr[ 2 ];
          s3 += f * aptr[ 3 ];
        }

        dst[ j + 0 ] = s0;
        dst[ j + 1 ] = s1;
        dst[ j + 2 ] = s2;
        dst[ j + 3 ] = s3;
      }

#endif

      for (; j < w; ++j) {
        double s0 = 0.f;

        for (k = 0; k < kx_cnt; ++k) {
          const double* aptr = A_ptr[ k ] + j;
          double f = kx_ptr[ k ];
          s0 += f * aptr[ 0 ];
        }

        dst[ j ] = s0;
      }
    }
  }

  pfree((void*) A_ptr);

  if (A_buf) {
    pfree((void*) A_buf);
  }

  if (B_buf) {
    pfree((void*) B_buf);
  }

  return 0;
}

static int filter2d_old(int h, int w, const double* src, int srcstep, int srccn, double* dst, int dststep,
    int kh, int kw, const double* K, int kl, int kx, int ky, int border_type)
{
  double* A_buf = 0;
  int i, j;

  if (1 == border_type) {
    int h2 = h + kh - 1;
    int w2 = w + kw - 1;
    A_buf = (double*) pmalloc(sizeof(double) * h2 * w2 * srccn);
    copy_replicate_border(h, w, src, srcstep * sizeof(double), h2, w2, A_buf, srccn * w2 * sizeof(double), srccn * sizeof(double), ky, kx);
    //COPY(h2*w2, A2, uA);
    //imwrite("c.bmp", h2, w2, uA, w2, 1);
    srcstep = srccn * w2;
    src = A_buf;
  }

  for (i = 0; i < h; ++i, dst += dststep, src += srcstep) {
    for (j = 0; j < w; ++j) {
      int x, y;
      double s0 = 0.f;
      const double* Ai = src + j;

      for (y = 0; y < kh; ++y) {
        int pos = y * kl;

        for (x = 0; x < kw; ++x) {
          int posx = pos + x;
          s0 += K[ posx ] * Ai[ posx ];
        }
      }

      dst[ j ] = s0;
    }
  }

  if (A_buf) {
    pfree((void*) A_buf);
  }

  return 0;
}

static int copy_to_8u(int h, int w, const double* src, int srcstep, unsigned char* dst, int dststep)
{
  int i, j;
  double minA, maxA, binA;
  minA = maxA = *src;

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      double f = src[ i * srcstep + j ];

      if (f < minA) {
        minA = f;
      }

      if (f > maxA) {
        maxA = f;
      }
    }
  }

  binA = 255.f / (maxA - minA);

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      double f = src[ i * srcstep + j ];
      dst[ i * dststep + j ] = (unsigned char)(int)((f - minA) * binA);
    }
  }

  return 0;
}
