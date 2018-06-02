/*************************************************\
common 3-channel 2D-filter of Samsung Project

by: Yuanjie Huang, THU
Modified by: Weiguang Si
date: Mar, 2008

mail:huangyuanjie@gmail.com
\*************************************************/

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)

void RGB2Lab(double R, double G, double B, double* L, double* a, double* b)
{
  // double BLACK = 20;
  // double YELLOW = 70;
  double  X, Y, Z, fX, fY, fZ;
  X = 0.412453 * R + 0.357580 * G + 0.180423 * B;
  Y = 0.212671 * R + 0.715160 * G + 0.072169 * B;
  Z = 0.019334 * R + 0.119193 * G + 0.950227 * B;
  X /= 0.950456;
  Z /= 1.088754;

  if (Y > 0.008856) {
    fY = pow(Y, 1.0 / 3.0);
    *L = 116.0 * fY - 16.0;
  }
  else {
    fY = 7.787 * Y + 16.0 / 116.0;
    *L = 903.3 * Y;
  }

  if (X > 0.008856) {
    fX = pow(X, 1.0 / 3.0);
  }
  else {
    fX = 7.787 * X + 16.0 / 116.0;
  }

  if (Z > 0.008856) {
    fZ = pow(Z, 1.0 / 3.0);
  }
  else {
    fZ = 7.787 * Z + 16.0 / 116.0;
  }

  *a = 500.0 * (fX - fY);
  *b = 200.0 * (fY - fZ);

  /*    if (*L < BLACK)
    {
  *a *= exp((*L - BLACK) / (BLACK / 4));
  *b *= exp((*L - BLACK) / (BLACK / 4));
  *L = BLACK;
    }
    if (*b > YELLOW)
  *b = YELLOW;
  */
}

void Lab2RGB(double L, double a, double b, double* R, double* G, double* B)
{
  double  X, Y, Z, fX, fY, fZ;
  fY = pow((L + 16.0) / 116.0, 3.0);

  if (fY < 0.008856) {
    fY = L / 903.3;
  }

  Y = fY;

  if (fY > 0.008856) {
    fY = pow(fY, 1.0 / 3.0);
  }
  else {
    fY = 7.787 * fY + 16.0 / 116.0;
  }

  fX = a / 500.0 + fY;

  if (fX > 0.206893) {
    X = pow(fX, 3.0);
  }
  else {
    X = (fX - 16.0 / 116.0) / 7.787;
  }

  fZ = fY - b / 200.0;

  if (fZ > 0.206893) {
    Z = pow(fZ, 3.0);
  }
  else {
    Z = (fZ - 16.0 / 116.0) / 7.787;
  }

  X *= 0.950456;
  Z *= 1.088754;
  *R = 3.240479 * X - 1.537150 * Y - 0.498535 * Z;
  *G = -0.969256 * X + 1.875992 * Y + 0.041556 * Z;
  *B = 0.055648 * X - 0.204043 * Y + 1.057311 * Z;

  // *R=(*R<0 ? 0:*R>1? 1:*R);
  // *G=(*R<0 ? 0:*R>1? 1:*G);
  // *B=(*R<0 ? 0:*R>1? 1:*B);
}

// src: source data in ImMat Format;
// des: destination data in ImMat Format;
// flt: filter  in ImMatD Format;
// midLevel: add Level to data
int imFilter(int sHeight, int sWidth, unsigned char* src, int src_step,
    unsigned char* des, int des_step,
    int fHeight, int fWidth, double* flt, int flt_step, int midLevel)
{
  double  dPixR, dPixG, dPixB;
  int     sPixX, sPixY;
  int     x, y, i, j;
  int     fK, index = 0;

  if (fWidth != fHeight || (fWidth % 2) == 0) {
    //Error Here
    return 1;
  }
  else {
    fK = (fWidth - 1) / 2;
  }

  for (x = 0; x < sWidth; x++) {
    for (y = 0; y < sHeight; y++) {
      dPixR = 0;
      dPixG = 0;
      dPixB = 0;

      for (i = 0; i < fWidth; i++) {
        for (j = 0; j < fHeight; j++) {
          sPixX = ((x - fK + i) >= 0) ? (x - fK + i) : x;
          sPixY = ((y - fK + j) >= 0) ? (y - fK + j) : y;
          index = sPixX + sPixY * sWidth;
          dPixR += src[3 * index] * flt[i + fWidth * j];
          dPixG += src[3 * index + 1] * flt[i + fWidth * j];
          dPixB += src[3 * index + 2] * flt[i + fWidth * j];
        }
      }

      dPixR += midLevel;
      dPixG += midLevel;
      dPixB += midLevel;
      index = x + y * sWidth;
      des[3 * index] = (unsigned char)((dPixR >= 0) ? (dPixR <= 255 ? dPixR : 255) : 0);
      des[3 * index + 1] = (unsigned char)((dPixG >= 0) ? (dPixG <= 255 ? dPixG : 255) : 0);
      des[3 * index + 2] = (unsigned char)((dPixB >= 0) ? (dPixB <= 255 ? dPixB : 255) : 0);
    }
  }

  return 0;
}

int CmpPixel(const void* a, const void* b)
{
  unsigned char* ma, *mb;
  ma = (unsigned char*) a;
  mb = (unsigned char*) b;

  if (*ma < *mb) {
    return -1;
  }

  if (*ma > *mb) {
    return 1;
  }

  return 0;
}

void PixelMedian(unsigned char* lpDst, unsigned char* lpSrc, int width, int num)
{
  int i, j, temp;
  unsigned char* R, *G, *B;
  unsigned char* lpTemp1, *lpTemp2;
  temp = num * num;
  R = (unsigned char*) pmalloc(sizeof(unsigned char) * temp);
  G = (unsigned char*) pmalloc(sizeof(unsigned char) * temp);
  B = (unsigned char*) pmalloc(sizeof(unsigned char) * temp);
  temp = num / 2;
  lpTemp1 = lpSrc - temp * width * 3 - 3 * temp;

  for (i = 0; i < num; i++) {
    lpTemp2 = lpTemp1;

    for (j = 0; j < num; j++) {
      temp = i * num + j;
      R[temp] = *lpTemp2++;
      G[temp] = *lpTemp2++;
      B[temp] = *lpTemp2++;
    }

    lpTemp1 += 3 * width;
  }

  temp = num * num;
  qsort(R, temp, sizeof(unsigned char), CmpPixel);
  qsort(G, temp, sizeof(unsigned char), CmpPixel);
  qsort(B, temp, sizeof(unsigned char), CmpPixel);
  temp /= 2;
  *lpDst = R[temp];
  *(lpDst + 1) = G[temp];
  *(lpDst + 2) = B[temp];
  pfree(R);
  pfree(G);
  pfree(B);
}

// src: source data in ImMat Format;
// des: destination data in ImMat Format;
// num: the length of the filter;
int imMedian(int height, int width, unsigned char* src, unsigned char* des, int num)
{
  int i, j, temp;
  unsigned char* lpSrcTemp = src;
  unsigned char* lpDstTemp = des;
  unsigned char* lpSrcPixel = NULL;
  unsigned char* lpDstPixel = NULL;

  if (num % 2 != 1 || num <= 2) {
    return 0;
  }

  if (width < num || height < num) {
    return 0;
  }

  temp = num / 2;

  //copy the top num/2 rows
  memcpy(lpDstTemp, lpSrcTemp, sizeof(unsigned char) * 3 * temp * width);
  lpSrcTemp += 3 * temp * width;
  lpDstTemp += 3 * temp * width;

  for (i = 0; i < height - num + 1; i++) {
    lpSrcPixel = lpSrcTemp;
    lpDstPixel = lpDstTemp;

    for (j = 0; j < temp; j++) {
      memcpy(lpDstPixel, lpSrcPixel, sizeof(unsigned char) * 3);
      lpSrcPixel += 3;
      lpDstPixel += 3;
    }

    for (j = 0; j < width - num + 1; j++) {
      PixelMedian(lpDstPixel, lpSrcPixel, width, num);
      lpSrcPixel += 3;
      lpDstPixel += 3;
    }

    for (j = 0; j < temp; j++) {
      memcpy(lpDstPixel, lpSrcPixel, sizeof(unsigned char) * 3);
      lpSrcPixel += 3;
      lpDstPixel += 3;
    }

    lpSrcTemp += 3 * width;
    lpDstTemp += 3 * width;
  }

  memcpy(lpDstTemp, lpSrcTemp, sizeof(unsigned char) * 3 * temp * width);
  return 1;
}

#define FOREACH_DO(_N, _I, _DO)  for (_I=0; _I<(_N); ++_I) {_DO;}

void* memcpy2d(void* dst, int dl, const void* src, int sl, int h, int w)
{
  char* d = (char*)(dst);
  const char* s = (const char*)(src);

  if (dl == sl && dl == w) {
    return memcpy(dst, src, h * w);
  }

  for (; h--; s += sl, d += dl) {
    memcpy(d, s, w);
  }

  return dst;
}

void* memrep(void* dst, int cn, int n)
{
  char* d = (char*)(dst);
  int i;

  if (cn < 0) {
    for (i = 0; i < n; ++i) {
      d[i] = d[i + cn];
    }
  }
  else {
    for (i = n; i-- > 0;) {
      d[i] = d[i + cn];
    }
  }

  return dst;
}

void* memcpy2dex(void* dst, int dl, int dh, int dw,
    const void* src, int sl, int sh, int sw,
    int cn, int y, int x)
{
  char* d = (char*)(dst);
  const char* s = (const char*)(src);
  int i, j;

  if (NULL == d) {
    dst = d = (char*)pmalloc(dh * dl);
  }

  dw *= cn;
  sw *= cn;
  x *= cn;

  for (i = 0; i < dh; ++i, d += dl) {
    char* dx = d + x;

    if (i < y || i >= (y + sh - 1)) {
      memcpy(dx, s, sw);
    }
    else {
      memcpy(dx, s, sw);
      s += sl;
    }

    //memrep(d, cn, x);
    for (j = x; j-- > 0;) {
      d[j] = d[j + cn];
    }

    //memrep(d+x+sw, -cn, dw-sw-x);
    for (j = x + sw; j < dw - cn; ++j) {
      d[j] = d[j - cn];
    }
  }

  return dst;
}

void* malloccpy(const void* src, int n, int newn)
{
  void* dst;
  assert(n < newn);
  dst = pmalloc(newn);
  memcpy(dst, src, n);
  return dst;
}

void* malloccpy2d(const void* src, int sl, int h, int w, int dl)
{
  void* dst;
  assert(w < dl && w < sl);
  dst = pmalloc(h * dl);
  memcpy2d(dst, dl, src, sl, h, w);
  return dst;
}

void* memset2d(void* dst, int dl, int c, int h, int w)
{
  char* d = (char*)(dst);

  if (dl == w) {
    return memset(dst, c, h * w);
  }

  for (; h--; d += dl) {
    memset(d, c, w);
  }

  return dst;
}

// src: source data in ImMat Format;
// des: destination data in ImMat Format;
// w:   bilateral filter half-width
// sigma_d: spatial-domain standard deviation
// sigma_r: intensity-domain standard deviation
int imBiFilter(int src_height, int src_width, double* src,
    double* des, int w, double sigma_d, double sigma_r)
{
  int     i, j;
  int* src_idx;
  int X_height = 2 * w + 1;
  int X_width = 2 * w + 1;
  int Y_height = 2 * w + 1;
  int Y_width = 2 * w + 1;
  int _N = X_height * X_width;
  double* G, *GSub, idd, irr;
  sigma_r = sigma_r * 100;
  idd = -1. / (2. * sigma_d * sigma_d);
  irr = -1. / (2. * sigma_r * sigma_r);
  G = (double*) pmalloc(sizeof(double) * _N);
  GSub = (double*) pmalloc(sizeof(double) * _N);
  src_idx = (int*) pmalloc(sizeof(int) * _N);

  if (G == NULL) {
    return 0;
  }

  for (i = 0; i < 2 * w + 1; i++) {
    for (j = 0; j < 2 * w + 1; j++) {
      int index = i * (2 * w + 1) + j;
      double X = j - w;
      double Y = i - w;
      G[index] = exp(idd * (X * X + Y * Y));
    }
  }

  for (i = 0; i < src_height; i++) {
    for (j = 0; j < src_width; j++) {
      int r, c, k, m, src_index;
      int iMin = max(i - w, 0);
      int iMax = min(i + w, src_height - 1);
      int jMin = max(j - w, 0);
      int jMax = min(j + w, src_width - 1);
      int I1_height = iMax - iMin + 1;
      int I1_width = jMax - jMin + 1;
      int _M = I1_height * I1_width;
      double  dL, da, db;

      for (r = 0; r < I1_height; r++) {
        for (c = 0; c < I1_width; c++) {
          int index = r * I1_width + c;
          int G_index = (r + iMin - i + w) * X_width + (c + jMin - j + w);
          src_index = ((r + iMin) * src_width + c + jMin) * 3;
          src_idx[index] = src_index;
          GSub[index] = G[G_index];
        }
      }

      src_index = (i * src_width + j) * 3;
      {
        double cL = src[src_index], ca = src[src_index + 1], cb = src[src_index + 2];
        double F, norm_F = 0., sum_I1 = 0., sum_I2 = 0., sum_I3 = 0.;
        FOREACH_DO(_M, m, k = src_idx[m]; dL = src[k] - cL; da = src[k + 1] - ca; db = src[k + 2] - cb;
            F = GSub[m] * exp(irr * (dL * dL + da * da + db * db));
            norm_F += F; sum_I1 += src[k] * F; sum_I2 += src[k + 1] * F; sum_I3 += src[k + 2] * F;);
        des[src_index] = sum_I1 / norm_F;
        des[src_index + 1] = sum_I2 / norm_F;
        des[src_index + 2] = sum_I3 / norm_F;
      }
    }
  }

  pfree(GSub);
  pfree(G);
  pfree(src_idx);
  return 1;
}

