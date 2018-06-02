// ************************************************************************
//  文件名：restore.cpp
//
//  图像复原API函数库：
//
// BlurDIB()               - 图像模糊
// InverseDIB()            - 图像逆滤波
// NoiseBlurDIB()          - 图像模糊加噪
// WienerDIB()             - 图像维纳滤波
// RandomNoiseDIB()        - 图像中加入随机噪声
// SaltNoiseDIB()          - 图像中加入椒盐噪声
// Fourn()                 - n维FFT
// NoRestriction()      - 图像模糊
// InverseFilter()      - 图像逆滤波复原
// NoiseDegeneration()  - 图像模糊加噪
// WinnerFilter()       - 图像维纳滤波
// MotionDegeneration() - 图像运动模糊
// MotionRestore()      - 图像运动模糊复原
//
// *************************************************************************

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "macro.h"
#include "imglib.h"

#define SWAP(a, b) tempr=(a);(a)=(b);(b)=tempr

/*************************************************************************
* 函数名称：
*   BlurDIB()
 *
* 参数:
*   LPBYTE lpDIBBits   - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数）
*   int lHeight      - 源图像高度（象素数）
*   int lLineBytes   - 图像每行的字节数
 *
* 返回值:
*   BOOL               - 平移成功返回TRUE，否则返回FALSE。
 *
* 说明:
*   该函数用来对DIB图像进行模糊操作。
 ************************************************************************/
BOOL BlurDIB(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // 指向源图像的指针
  LPBYTE lpSrc;

  //循环变量
  int i;
  int j;

  //像素值
  BYTE pixel;

  //用于做FFT的数组
  Float* fftSrc, *fftKernel;
  //二维FFT的长度和宽度
  unsigned int nn[ 3 ];
  //图像归一化因子
  Float MaxNum;

  Float dPower = log((Float) lLineBytes) / log(2.0);

  if (dPower != (int) dPower) {
    return FALSE;
  }

  dPower = log((Float) lHeight) / log(2.0);

  if (dPower != (int) dPower) {
    return FALSE;
  }

  fftSrc = MALLOC(Float, lHeight * lLineBytes * 2 + 1);
  fftKernel = MALLOC(Float, lHeight * lLineBytes * 2 + 1);

  nn[ 1 ] = lHeight;
  nn[ 2 ] = lLineBytes;

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      pixel = (BYTE) * lpSrc;

      fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] = (Float) pixel;
      fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ] = 0.0;

      if (i < 5 && j < 5) {
        fftKernel[(2 * lLineBytes) * j + 2 * i + 1 ] = 1 / 25.0;
      }
      else {
        fftKernel[(2 * lLineBytes) * j + 2 * i + 1 ] = 0.0;
      }

      fftKernel[(2 * lLineBytes) * j + 2 * i + 2 ] = 0.0;
    }
  }

  //对源图像进行FFT
  Fourn(fftSrc, nn, 2, 1);
  //对卷积核图像进行FFT
  Fourn(fftKernel, nn, 2, 1);

  //频域相乘
  for (i = 1; i < lHeight * lLineBytes * 2; i += 2) {
    fftSrc[ i ] = fftSrc[ i ] * fftKernel[ i ] - fftSrc[ i + 1 ] * fftKernel[ i + 1 ];
    fftSrc[ i + 1 ] = fftSrc[ i ] * fftKernel[ i + 1 ] + fftSrc[ i + 1 ] * fftKernel[ i ];
  }

  //对结果图像进行反FFT
  Fourn(fftSrc, nn, 2, -1);

  //确定归一化因子
  MaxNum = 0.0;

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] =
          sqrt(fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] * fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] \
              + fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ] * fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ]);

      if (MaxNum < fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ]) {
        MaxNum = fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ];
      }
    }
  }

  //转换为图像
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      *lpSrc = (BYTE)(fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] * 255.0 / MaxNum);
    }
  }

  SAFEFREE(fftSrc);
  SAFEFREE(fftKernel);
  // 返回
  return TRUE;
}

/*************************************************************************
 *
* 函数名称：
*   Restore()
 *
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
 *
* 返回值:
*   BOOL               - 平移成功返回TRUE，否则返回FALSE。
 *
* 说明:
*   该函数用来对BlurDIB()生成的DIB图像进行复原操作。
 *
 ************************************************************************/
BOOL Restore(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // 指向源图像的指针
  LPBYTE lpSrc;

  //循环变量
  int i;
  int j;

  //像素值
  BYTE pixel;

  //用于做FFT的数组
  Float* fftSrc, *fftKernel;
  Float a, b, c, d;
  //二维FFT的长度和宽度
  unsigned int nn[ 3 ];
  //图像归一化因子
  Float MaxNum;

  Float dPower = log((Float) lLineBytes) / log(2.0);

  if (dPower != (int) dPower) {
    return FALSE;
  }

  dPower = log((Float) lHeight) / log(2.0);

  if (dPower != (int) dPower) {
    return FALSE;
  }

  fftSrc = MALLOC(Float, lHeight * lLineBytes * 2 + 1);
  fftKernel = MALLOC(Float, lHeight * lLineBytes * 2 + 1);

  nn[ 1 ] = lHeight;
  nn[ 2 ] = lLineBytes;

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      pixel = (BYTE) * lpSrc;

      fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] = (Float) pixel;
      fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ] = 0.0;

      if (i < 5 && j == 0) {
        fftKernel[(2 * lLineBytes) * j + 2 * i + 1 ] = 1 / 5.0;
      }
      else {
        fftKernel[(2 * lLineBytes) * j + 2 * i + 1 ] = 0.0;
      }

      fftKernel[(2 * lLineBytes) * j + 2 * i + 2 ] = 0.0;
    }
  }

  //对源图像进行FFT
  Fourn(fftSrc, nn, 2, 1);
  //对卷积核图像进行FFT
  Fourn(fftKernel, nn, 2, 1);

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      a = fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ];
      b = fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ];
      c = fftKernel[(2 * lLineBytes) * j + 2 * i + 1 ];
      d = fftKernel[(2 * lLineBytes) * j + 2 * i + 2 ];

      if (c * c + d * d > 1e-3) {
        fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] = (a * c + b * d) / (c * c + d * d);
        fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ] = (b * c - a * d) / (c * c + d * d);
      }
    }
  }

  //对结果图像进行反FFT
  Fourn(fftSrc, nn, 2, -1);

  //确定归一化因子
  MaxNum = 0.0;

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] =
          sqrt(fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] * fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] \
              + fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ] * fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ]);

      if (MaxNum < fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ]) {
        MaxNum = fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ];
      }
    }
  }


  //转换为图像
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      *lpSrc = (BYTE)(fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] * 255.0 / MaxNum);
    }
  }

  SAFEFREE(fftSrc);
  SAFEFREE(fftKernel);
  // 返回
  return TRUE;
}

/*************************************************************************
 *
* 函数名称：
*   NoiseBlur()
 *
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数）
*   int lHeight      - 源图像高度（象素数）
 *
* 返回值:
*   BOOL               - 模糊加噪操作成功返回TRUE，否则返回FALSE。
 *
* 说明:
*   该函数用来对DIB图像进行模糊加噪操作。
 *
 ************************************************************************/
BOOL NoiseBlur(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // 指向源图像的指针
  LPBYTE lpSrc;

  //循环变量
  int i;
  int j;

  //像素值
  BYTE pixel;

  //用于做FFT的数组
  Float* fftSrc, *fftKernel;
  //二维FFT的长度和宽度
  unsigned int nn[ 3 ];
  //图像归一化因子
  Float MaxNum;

  Float dPower = log((Float) lLineBytes) / log(2.0);

  //转换为图像，加噪
  char point;

  if (dPower != (int) dPower) {
    return FALSE;
  }

  dPower = log((Float) lHeight) / log(2.0);

  if (dPower != (int) dPower) {
    return FALSE;
  }

  fftSrc = MALLOC(Float, lHeight * lLineBytes * 2 + 1);
  fftKernel = MALLOC(Float, lHeight * lLineBytes * 2 + 1);

  nn[ 1 ] = lHeight;
  nn[ 2 ] = lLineBytes;

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      pixel = (BYTE) * lpSrc;

      fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] = (Float) pixel;
      fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ] = 0.0;

      if (i < 5 && j == 0) {
        fftKernel[(2 * lLineBytes) * j + 2 * i + 1 ] = 1 / 5.0;
      }
      else {
        fftKernel[(2 * lLineBytes) * j + 2 * i + 1 ] = 0.0;
      }

      fftKernel[(2 * lLineBytes) * j + 2 * i + 2 ] = 0.0;
    }
  }

  //对源图像进行FFT
  Fourn(fftSrc, nn, 2, 1);
  //对卷积核图像进行FFT
  Fourn(fftKernel, nn, 2, 1);

  //频域相乘
  for (i = 1; i < lHeight * lLineBytes * 2; i += 2) {
    fftSrc[ i ] = fftSrc[ i ] * fftKernel[ i ] - fftSrc[ i + 1 ] * fftKernel[ i + 1 ];
    fftSrc[ i + 1 ] = fftSrc[ i ] * fftKernel[ i + 1 ] + fftSrc[ i + 1 ] * fftKernel[ i ];
  }

  //对结果图像进行反FFT
  Fourn(fftSrc, nn, 2, -1);

  //确定归一化因子
  MaxNum = 0.0;

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] =
          sqrt(fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] * fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] \
              + fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ] * fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ]);

      if (MaxNum < fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ]) {
        MaxNum = fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ];
      }
    }
  }

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      if (i + j == ((int)((i + j) / 8)) * 8) {
        point = -16;
      }
      else {
        point = 0;
      }

      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      *lpSrc = (BYTE)(fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] * 255.0 / MaxNum + point);
    }
  }

  SAFEFREE(fftSrc);
  SAFEFREE(fftKernel);
  // 返回
  return TRUE;
}

/*************************************************************************
 *
* 函数名称：
*   Wiener()
 *
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数）
*   int lHeight      - 源图像高度（象素数）
 *
* 返回值:
*   BOOL               - 维纳滤波复原操作成功返回TRUE，否则返回FALSE。
 *
* 说明:
*   该函数用来对DIB图像进行维纳滤波复原操作。
 *
 ************************************************************************/

BOOL Wiener(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // 指向源图像的指针
  LPBYTE lpSrc;

  //循环变量
  int i;
  int j;

  //像素值
  BYTE pixel;

  //用于做FFT的数组
  Float* fftSrc, *fftKernel, *fftNoise;
  Float a, b, c, d, e, f, multi;
  //二维FFT的长度和宽度
  unsigned int nn[ 3 ];
  //图像归一化因子
  Float MaxNum;

  Float dPower = log((Float) lLineBytes) / log(2.0);

  if (dPower != (int) dPower) {
    return FALSE;
  }

  dPower = log((Float) lHeight) / log(2.0);

  if (dPower != (int) dPower) {
    return FALSE;
  }

  fftSrc = MALLOC(Float, lHeight * lLineBytes * 2 + 1);
  fftKernel = MALLOC(Float, lHeight * lLineBytes * 2 + 1);
  fftNoise = MALLOC(Float, lHeight * lLineBytes * 2 + 1);

  nn[ 1 ] = lHeight;
  nn[ 2 ] = lLineBytes;

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      pixel = (BYTE) * lpSrc;

      fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] = (Float) pixel;
      fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ] = 0.0;

      if (i < 5 && j == 0) {
        fftKernel[(2 * lLineBytes) * j + 2 * i + 1 ] = 1 / 5.0;
      }
      else {
        fftKernel[(2 * lLineBytes) * j + 2 * i + 1 ] = 0.0;
      }

      fftKernel[(2 * lLineBytes) * j + 2 * i + 2 ] = 0.0;

      if (i + j == ((int)((i + j) / 8)) * 8) {
        fftNoise [(2 * lLineBytes) * j + 2 * i + 1 ] = -16.0;
      }
      else {
        fftNoise [(2 * lLineBytes) * j + 2 * i + 1 ] = 0.0;
      }

      fftNoise[(2 * lLineBytes) * j + 2 * i + 2 ] = 0.0;

    }
  }

  srand((unsigned) time(NULL));

  //对源图像进行FFT
  Fourn(fftSrc, nn, 2, 1);
  //对卷积核图像进行FFT
  Fourn(fftKernel, nn, 2, 1);
  //对噪声图像进行FFT
  Fourn(fftNoise, nn, 2, 1);

  for (i = 1; i < lHeight * lLineBytes * 2; i += 2) {
    a = fftSrc[ i ];
    b = fftSrc[ i + 1 ];
    c = fftKernel[ i ];
    d = fftKernel[ i + 1 ];
    e = fftNoise[ i ];
    f = fftNoise[ i + 1 ];
    multi = (a * a + b * b) / (a * a + b * b - e * e - f * f);

    if (c * c + d * d > 1e-3) {
      fftSrc[ i ] = (a * c + b * d) / (c * c + d * d) / multi;
      fftSrc[ i + 1 ] = (b * c - a * d) / (c * c + d * d) / multi;
    }
  }

  //对结果图像进行反FFT
  Fourn(fftSrc, nn, 2, -1);

  //确定归一化因子
  MaxNum = 0.0;

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] =
          sqrt(fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] * fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] \
              + fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ] * fftSrc[(2 * lLineBytes) * j + 2 * i + 2 ]);

      if (MaxNum < fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ]) {
        MaxNum = fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ];
      }
    }
  }

  //转换为图像
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      *lpSrc = (BYTE)(fftSrc[(2 * lLineBytes) * j + 2 * i + 1 ] * 255.0 / MaxNum);
    }
  }

  SAFEFREE(fftSrc);
  SAFEFREE(fftKernel);
  SAFEFREE(fftNoise);
  // 返回
  return TRUE;
}


BOOL Fourn(Float* data, unsigned int nn[], int ndim, int isign)
{
  int idim;
  unsigned int i1, i2, i3, i2rev, i3rev, ip1, ip2, ip3, ifp1, ifp2;
  unsigned int ibit, k1, k2, n, nprev, nrem, ntot;
  Float tempi, tempr;
  Float theta, wi, wpi, wpr, wr, wtemp;

  for (ntot = 1, idim = 1; idim <= ndim; idim++) {
    ntot *= nn[ idim ];
  }

  nprev = 1;

  for (idim = ndim; idim >= 1; idim--) {
    n = nn[ idim ];
    nrem = ntot / (n * nprev);
    ip1 = nprev << 1;
    ip2 = ip1 * n;
    ip3 = ip2 * nrem;
    i2rev = 1;

    for (i2 = 1; i2 <= ip2; i2 += ip1) {
      if (i2 < i2rev) {
        for (i1 = i2; i1 <= i2 + ip1 - 2; i1 += 2) {
          for (i3 = i1; i3 <= ip3; i3 += ip2) {
            i3rev = i2rev + i3 - i2;
            SWAP(data[ i3 ], data[ i3rev ]);
            SWAP(data[ i3 + 1 ], data[ i3rev + 1 ]);
          }
        }
      }

      ibit = ip2 >> 1;

      while (ibit >= ip1 && i2rev > ibit) {
        i2rev -= ibit;
        ibit >>= 1;
      }

      i2rev += ibit;
    }

    ifp1 = ip1;

    while (ifp1 < ip2) {
      ifp2 = ifp1 << 1;
      theta = isign * 6.28318530717959 / (ifp2 / ip1);
      wtemp = sin(0.5 * theta);
      wpr = -2.0 * wtemp * wtemp;
      wpi = sin(theta);
      wr = 1.0;
      wi = 0.0;

      for (i3 = 1; i3 <= ifp1; i3 += ip1) {
        for (i1 = i3; i1 <= i3 + ip1 - 2; i1 += 2) {
          for (i2 = i1; i2 <= ip3; i2 += ifp2) {
            k1 = i2;
            k2 = k1 + ifp1;
            tempr = wr * data[ k2 ] - wi * data[ k2 + 1 ];
            tempi = wr * data[ k2 + 1 ] + wi * data[ k2 ];
            data[ k2 ] = data[ k1 ] - tempr;
            data[ k2 + 1 ] = data[ k1 + 1 ] - tempi;
            data[ k1 ] += tempr;
            data[ k1 + 1 ] += tempi;
          }
        }

        wr = (wtemp = wr) * wpr - wi * wpi + wr;
        wi = wi * wpr + wtemp * wpi + wi;
      }

      ifp1 = ifp2;
    }

    nprev *= n;
  }

  return TRUE;
}

/*************************************************************************
 *
* 函数名称：
*   RandomNoise()
 *
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
 *
* 返回值:
*   BOOL               - 模糊操作成功返回TRUE，否则返回FALSE。
 *
* 说明:
*   该函数用来对DIB图像进行模糊操作。
 *
 ************************************************************************/

BOOL RandomNoise(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // 指向源图像的指针
  LPBYTE lpSrc;

  //循环变量
  int i;
  int j;

  //像素值
  BYTE pixel;

  //噪声
  BYTE NoisePoint;

  //生成伪随机种子
  srand((unsigned) time(NULL));

  //在图像中加噪
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      NoisePoint = rand() / 1024;

      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      //取得像素值
      pixel = (BYTE) * lpSrc;

      *lpSrc = (BYTE)(pixel * 224 / 256 + NoisePoint);
    }
  }

  // 返回
  return TRUE;
}

/*************************************************************************
 *
* 函数名称：
*   SaltNoise()
 *
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
 *
* 返回值:
*   BOOL               - 模糊操作成功返回TRUE，否则返回FALSE。
 *
* 说明:
*   该函数用来对DIB图像进行模糊操作。
 *
 ************************************************************************/

BOOL SaltNoise(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // 指向源图像的指针
  LPBYTE lpSrc;

  //循环变量
  int i;
  int j;

  //生成伪随机种子
  srand((unsigned) time(NULL));

  //在图像中加噪
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      if (rand() > 31500) {
        // 指向源图像倒数第j行，第i个象素的指针
        lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

        //图像中当前点置为黑
        *lpSrc = 0;
      }
    }
  }

  // 返回
  return TRUE;
}

/*************************************************************************
 *
* 函数名称：
*   NoRestriction()
 *
* 参数:
*   CDib  *pDib       - 指向CDib类的指针
 *
* 返回值:
*   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
* 说明:
*   该函数用来对DIB图像进行模糊操作。
 *
 ************************************************************************/
// int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes
BOOL NoRestriction(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // 指向源图像的指针
  LPBYTE lpSrc;

  //循环变量
  int i;
  int j;

  //临时变量
  Float temp;

  // 实际进行付立叶变换的宽度和高度
  int lW = 1;
  int lH = 1;

  int wp = 0;
  int hp = 0;

  //用来存储源图象和变换核的时域数据
  COMPLEX* pCTSrc, *pCTH;

  //用来存储源图象和变换核的频域数据
  COMPLEX* pCFSrc, *pCFH;

  //图像归一化因子
  Float MaxNum;

  // 保证离散傅立叶变换的宽度和高度为2的整数次方
  while (lW * 2 <= lLineBytes) {
    lW = lW * 2;
    wp++;
  }

  while (lH * 2 <= lHeight) {
    lH = lH * 2;
    hp++;
  }

  //输入图象的长和宽必须为2的整数倍
  if (lW != (int) lLineBytes) {
    return FALSE;
  }

  if (lH != (int) lHeight) {
    return FALSE;
  }

  // 为时域和频域的数组分配空间
  pCTSrc = MALLOC(COMPLEX, lHeight * lLineBytes);
  pCTH = MALLOC(COMPLEX, lHeight * lLineBytes);

  pCFSrc = MALLOC(COMPLEX, lHeight * lLineBytes);
  pCFH = MALLOC(COMPLEX, lHeight * lLineBytes);

  // 将数据存入时域数组
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      CplexSet((Float) * lpSrc , 0, pCTSrc[ lLineBytes * j + i ]);
      CplexSet(0.0 , 0.0, pCFSrc[ lLineBytes * j + i ]);

      if (i < 5 && j < 5) {
        CplexSet(0.04 , 0.0, pCTH[ lLineBytes * j + i ]);
      }
      else {
        CplexSet(0.0 , 0.0, pCTH[ lLineBytes * j + i ]);
      }

      CplexSet(0.0 , 0.0, pCFH[ lLineBytes * j + i ]);
    }
  }

  //对源图像进行FFT
  Fft2D(pCTSrc, lLineBytes, lHeight, pCFSrc);

  //对变换核图像进行FFT
  Fft2D(pCTH, lLineBytes, lHeight, pCFH);

  //频域相乘
  for (i = 0; i < lHeight * lLineBytes; i++) {
    CplexMul(pCFSrc[ i ], pCFH[ i ], pCFSrc[ i ]);
  }

  //对结果图像进行反FFT
  IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

  //确定归一化因子
  MaxNum = 0.0;

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      temp = sqrt(CREAL(pCTSrc[ lLineBytes * j + i ]) * CREAL(pCTSrc[ lLineBytes * j + i ]) +
          CIMAG(pCTSrc[ lLineBytes * j + i ]) * CIMAG(pCTSrc[ lLineBytes * j + i ]));

      //选择归一化因子
      if (MaxNum < temp) {
        MaxNum = temp;
      }
    }
  }

  //转换为图像
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;
      *lpSrc = (BYTE)(CREAL(pCTSrc[(lLineBytes) * j + i ]) * 255.0 / MaxNum);
    }
  }

  //释放存储空间
  SAFEFREE(pCTSrc);
  SAFEFREE(pCTH);

  SAFEFREE(pCFSrc);
  SAFEFREE(pCFH);

  // 返回
  return TRUE;
}

/*************************************************************************
 *
* 函数名称：
*   NoiseDegeneration()
 *
* 参数:
*   CDib  *pDib       - 指向CDib类的指针
 *
* 返回值:
*   BOOL               - 模糊加噪操作成功返回TRUE，否则返回FALSE。
 *
* 说明:
*   该函数用来对DIB图像进行模糊加噪操作。
 *
 ************************************************************************/

BOOL NoiseDegeneration(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // 指向源图像的指针
  LPBYTE lpSrc;

  //循环变量
  int i;
  int j;

  //转换为图像，加噪
  BYTE NoisePoint;

  //临时变量
  Float temp;

  //图像归一化因子
  Float MaxNum;

  // 实际进行付立叶变换的宽度和高度
  int lW = 1;
  int lH = 1;

  int wp = 0;
  int hp = 0;

  //用来存储源图象和变换核的时域数据
  COMPLEX* pCTSrc, *pCTH;

  //用来存储源图象和变换核的频域数据
  COMPLEX* pCFSrc, *pCFH;

  // 保证离散傅立叶变换的宽度和高度为2的整数次方
  while (lW * 2 <= lLineBytes) {
    lW = lW * 2;
    wp++;
  }

  while (lH * 2 <= lHeight) {
    lH = lH * 2;
    hp++;
  }

  // 为时域和频域的数组分配空间
  pCTSrc = MALLOC(COMPLEX, lHeight * lLineBytes);
  pCTH = MALLOC(COMPLEX, lHeight * lLineBytes);

  pCFSrc = MALLOC(COMPLEX, lHeight * lLineBytes);
  pCFH = MALLOC(COMPLEX, lHeight * lLineBytes);

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      // 将象素值存储到时域数组中
      CplexSet((Float) * lpSrc , 0, pCTSrc[ lLineBytes * j + i ]);

      // 频域赋零值
      CplexSet(0.0 , 0.0, pCFSrc[ lLineBytes * j + i ]);

      // 用来对图象做退化的系统
      if (i < 5 && j < 5) {
        CplexSet(0.04 , 0.0, pCTH[ lLineBytes * j + i ]);
      }
      else {
        CplexSet(0.0 , 0.0, pCTH[ lLineBytes * j + i ]);
      }

      // 频域赋零值
      CplexSet(0.0 , 0.0, pCFH[ lLineBytes * j + i ]);
    }
  }

  //对源图像进行FFT
  Fft2D(pCTSrc, lLineBytes, lHeight, pCFSrc);

  //对变换核图像进行FFT
  Fft2D(pCTH, lLineBytes, lHeight, pCFH);

  //频域相乘
  for (i = 0; i < lHeight * lLineBytes; i++) {
    CplexMul(pCFSrc[ i ], pCFH[ i ], pCFSrc[ i ]);
  }

  //对结果图像进行反FFT
  IFFT_2D(pCFSrc, pCTSrc, lLineBytes, lHeight);

  //确定归一化因子
  MaxNum = 0.0;

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      temp = sqrt(CREAL(pCTSrc[ lLineBytes * j + i ]) * CREAL(pCTSrc[ lLineBytes * j + i ]) +
          CIMAG(pCTSrc[ lLineBytes * j + i ]) * CIMAG(pCTSrc[ lLineBytes * j + i ]));

      //选择归一化因子
      if (MaxNum < temp) {
        MaxNum = temp;
      }
    }
  }

  //生成伪随机数种子
  srand((unsigned) time(NULL));

  //转换为图像，并加入伪随机噪声
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      // 产生的噪声
      NoisePoint = rand() / 2048 - 8;

      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      // 时域加噪，存储象素值
      *lpSrc = (BYTE)(CREAL(pCTSrc[(lLineBytes) * j + i ]) * 255.0 / MaxNum + NoisePoint);

      //如果象素值过大，直接赋值255
      if (*lpSrc > 255) {
        * lpSrc = 255;
      }
    }
  }

  //释放存储空间
  SAFEFREE(pCTSrc);
  SAFEFREE(pCTH);

  SAFEFREE(pCFSrc);
  SAFEFREE(pCFH);

  // 返回
  return TRUE;
}

/*************************************************************************
 *
* 函数名称：
*   MotionDegeneration()
 *
* 参数:
*   CDib  *pDib       - 指向CDib类的指针
 *
* 返回值:
*   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
* 说明:
*   该函数用来对DIB图像模拟由匀速直线运动造成的模糊
 *
 ************************************************************************/
BOOL MotionDegeneration(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // 指向源图像的指针
  LPBYTE lpSrc;

  //循环变量
  int iColumn;
  int jRow;

  //临时变量
  int temp, m;

  // 临时变量
  Float p, q;

  int nTotTime, nTotLen, nTime;

  // 摄像机的暴光系数
  Float B = 0.1;

  //用来存储源图象和变换核的时域数据
  int* nImageDegener;

  //总的运动时间10s
  nTotTime = 10;

  // 总的运动距离10个象素点
  nTotLen = 10;

  // 为时域和频域的数组分配空间
  nImageDegener = MALLOC(int, lHeight * lLineBytes);

  // 将数据存入时域数组
  for (jRow = 0; jRow < lHeight; jRow++) {
    for (iColumn = 0; iColumn < lLineBytes; iColumn++) {
      temp = 0;

      // 指向源图像倒数第jRow行，第iColumn个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * jRow + iColumn;

      // 象素点的象素值积累
      for (nTime = 0; nTime < nTotTime; nTime++) {
        p = (Float) iColumn - (Float)(nTotLen) * nTime / nTotTime;

        if (p > 0) {
          q = p - floor((Float) p);

          if (q >= 0.5) {
            m = (int) ceil((Float) p);
          }
          else {
            m = (int) floor((Float) p);
          }

          // 累加
          lpSrc = (LPBYTE) lpDIBBits + lLineBytes * jRow + m;
          temp = temp + *lpSrc;
        }
      }

      // 乘以摄像机的暴光系数
      temp = (int)(B * temp);

      temp = (int) ceil((Float) temp);

      // 使得temp的取值符合取值范围
      if (temp < 0) {
        temp = 0;
      }

      if (temp > 255) {
        temp = 255;
      }

      nImageDegener[ lLineBytes * jRow + iColumn ] = temp;
    }
  }

  //转换为图像
  for (jRow = 0; jRow < lHeight; jRow++) {
    for (iColumn = 0; iColumn < lLineBytes; iColumn++) {
      // 指向源图像倒数第jRow行，第iColumn个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * jRow + iColumn;

      *lpSrc = nImageDegener[ lLineBytes * jRow + iColumn ];
    }
  }

  //释放存储空间
  SAFEFREE(nImageDegener);

  // 返回
  return TRUE;
}

/*************************************************************************
 *
* 函数名称：
*   MotionRestore()
 *
* 参数:
*   CDib  *pDib       - 指向CDib类的指针
 *
* 返回值:
*   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
* 说明:
*   该函数用来对拟由匀速直线运动造成的模糊图象进行复原
 *
 ************************************************************************/
BOOL MotionRestore(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // 指向源图像的指针
  LPBYTE lpSrc;

  //循环变量
  int iColumn;
  int jRow;

  int i, n, m;

  //临时变量
  int temp1, temp2,
      totalq, q1, q2, z;

  Float p, q;

  // 常量A赋值
  int A = 80;

  //常量B赋值
  int B = 10;

  //总的移动距离
  int nTotLen = 10;

  // 图象宽度包含多少个ntotlen
  int K = lLineBytes / nTotLen;

  int error[ 10 ];

  //用来存储源图象时域数据
  int* nImageDegener;

  // 为时域数组分配空间
  nImageDegener = MALLOC(int, lHeight * lLineBytes);

  // 将象素存入数组中
  for (jRow = 0; jRow < lHeight; jRow++) {
    for (iColumn = 0; iColumn < lLineBytes; iColumn++) {
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * jRow + iColumn;

      nImageDegener[ lLineBytes * jRow + iColumn ] = (*lpSrc);
    }
  }

  for (jRow = 0; jRow < lHeight; jRow++) {
    // 计算error[i]
    for (i = 0; i < 10; i++) {

      error[ i ] = 0;

      for (n = 0; n < K; n++)
        for (m = 0; m <= n; m++) {
          // 象素是否为一行的开始处
          if (i == 0 && m == 0) {
            temp1 = temp2 = 0;
          }

          // 进行差分运算
          else {
            lpSrc = (LPBYTE) lpDIBBits + lLineBytes * jRow + m * 10 + i;
            temp1 = *lpSrc;

            lpSrc = (LPBYTE) lpDIBBits + lLineBytes * jRow + m * 10 + i - 1;
            temp2 = *lpSrc;
          }

          error[ i ] = error[ i ] + temp1 - temp2;

        }

      error[ i ] = B * error[ i ] / K;
    }

    for (iColumn = 0; iColumn < lLineBytes; iColumn++) {
      // 计算m，以及z
      m = iColumn / nTotLen;
      z = iColumn - m * nTotLen;

      // 初始化
      totalq = 0;
      q = 0;

      for (n = 0; n <= m; n++) {
        q1 = iColumn - nTotLen * n;

        if (q1 == 0) {
          q = 0;
        }

        // 进行差分运算
        else {
          q2 = q1 - 1;

          lpSrc = (LPBYTE) lpDIBBits + lLineBytes * jRow + q1;
          temp1 = *lpSrc;

          lpSrc = (LPBYTE) lpDIBBits + lLineBytes * jRow + q2;
          temp2 = *lpSrc;

          q = (temp1 - temp2) * B;
        }

        totalq += (int) q;
      }

      p = error[ z ];

      // 得到f(x, y)的值
      temp1 = (int)(totalq + A - p);

      // 使得象素的取值符合取值范围
      if (temp1 < 0) {
        temp1 = 0;
      }

      if (temp1 > 255) {
        temp1 = 255;
      }

      nImageDegener[ lLineBytes * jRow + iColumn ] = temp1;
    }
  }

  //转换为图像
  for (jRow = 0; jRow < lHeight; jRow++) {
    for (iColumn = 0; iColumn < lLineBytes; iColumn++) {
      // 指向源图像倒数第jRow行，第iColumn个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * jRow + iColumn;

      // 存储象素值
      *lpSrc = nImageDegener[ lLineBytes * jRow + iColumn ];
    }
  }

  //释放存储空间
  SAFEFREE(nImageDegener);

  // 返回
  return TRUE;
}

#undef SWAP

