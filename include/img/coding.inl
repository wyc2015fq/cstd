
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "macro.h"
#include "imglib.h"

// FOURBYTES就是用来计算离4最近的整倍数
#define FOURBYTES(bits)    (((bits) + 31) / 32* 4)

#define FWRITE(file, _PTR, _N) fwrite( _PTR, _N, 1, file)
#define FWRITEHUGE(file, _PTR, _N) fwrite( _PTR, _N, 1, file)
#define FREAD(file, _PTR, _N) fread( _PTR, _N, 1, file)
#define FREADHUGE(file, _PTR, _N) fread( _PTR, _N, 1, file)

/**************************************************************************
 *  文件名：ImageCoding.cpp
 *
 *  正交变换API函数库：
 *
 *  BITPLANE()            - 图像位平面分解
 *  LimbPatternBayer()       - 用BAYER表抖动显示图象
 *  DitherFloydSteinberg()   - 用Floyd－Steinberg算法抖动生成图象
 *
 *************************************************************************/

/*************************************************************************
 *
* 函数名称：
*   BITPLANE()
 *
* 参数:
*   int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes        - 指向CDib类的指针
*   FILE* file        - 要读取的文件
 *
* 返回值:
*   BOOL               - 成功返回TRUE
 *
* 说明:
*   该函数将制定的图象进行位平面分解
 *
 *************************************************************************/
BOOL BitPlane(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, BYTE bBitNum)
{
  LPBYTE lpSrc; // 指向源图像的指针
  int i, j; // 循环变量
  BYTE bTemp, bA; // 中间变量

  // 二进制第i位对应的十进制值
  BYTE bCount;

  // 如果输入的数不符合要求，不进行分解
  if (bBitNum < 1 || bBitNum > 8) {
    return FALSE;
  }

  bCount = (BYTE)(1 << (bBitNum - 1));

  for (i = 0; i < lHeight; i++) {
    for (j = 0; j < lWidth; j++) {
      // 指向位图i行j列的指针
      lpSrc = (LPBYTE) lpDIBBits + j + lLineBytes * (lHeight - 1 - i);

      // 对位图按灰度码进行分解
      if (bBitNum == 8) {
        // 如果求的是位图8，直接取二值的第8位
        bTemp = (BYTE)((*(lpSrc) & bCount) / bCount);
        bTemp = (BYTE)(bTemp * 255);
      }
      else {
        // 否则进行异或，求得灰度码
        bTemp = (BYTE)((*(lpSrc) & bCount) / bCount);
        bA = (BYTE)(bCount * 2);

        // 第i＋1位图的象素值
        bA = (BYTE)((*(lpSrc) & bA) / bA);

        // 异或
        bTemp = (BYTE)((bTemp ^ bA) * 255);
      }

      // 更新源图象
      *(lpSrc) = bTemp;
    }
  }

  // 返回值
  return TRUE;
}

/*************************************************************************
 * 函数名称：
 *   LimbPatternBayer()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数利用BAYER表抖动显示图象。
 ************************************************************************/
BOOL LimbPatternBayer(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // Bayer表的定义
  BYTE BayerPattern[ 8 ][ 8 ] = { //
    0, 32, 8, 40, 2, 34, 10, 42,
    48, 16, 56, 24, 50, 18, 58, 26,
    12, 44, 4, 36, 14, 46, 6, 38,
    60, 28, 52, 20, 62, 30, 54, 22,
    3, 35, 11, 43, 1, 33, 9, 41,
    51, 19, 59, 27, 49, 17, 57, 25,
    15, 47, 7, 39, 13, 45, 5, 37,
    63, 31, 55, 23, 61, 29, 53, 21
  };

  // 指向源图像的指针
  LPBYTE lpSrc;

  // 循环变量
  int i, j;

  // 象素的值
  int nPixelValue;

  // 将图象二值化，利用BAYER表抖动显示图象
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      nPixelValue = (*lpSrc);

      nPixelValue = nPixelValue;

      // 右移两位后做比较
      if ((nPixelValue >> 2) > BayerPattern[ j & 7 ][ i & 7 ])
        //打白点
      {
        *(lpSrc) = (BYTE) 255;
      }

      else
        //打黑点
      {
        *(lpSrc) = (BYTE) 0;
      }
    }
  }

  return TRUE;
}

/*************************************************************************
 * 函数名称：
 *   DitherFloydSteinberg()
 *
 * 参数:
 *   CDib  *pDib       - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来用Floyd－Steinberg算法抖动生成图象。
 ************************************************************************/
BOOL DitherFloydSteinberg(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // 指向源图像的指针
  LPBYTE lpSrc;

  // 循环变量
  int i, j;

  // 误差传播系数
  Float temp, error;

  // 象素值
  int nPixelValue;

  // 将图象二值化，并用Floyd－Steinberg算法抖动生成图象
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      nPixelValue = *lpSrc;

      //128是中值
      if (nPixelValue > 128) {
        //打白点
        *lpSrc = 255;

        //计算误差
        error = (Float)(nPixelValue - 255.0);
      }
      else {
        //打黑点
        *lpSrc = 0;

        //计算误差
        error = (Float) nPixelValue;
      }

      // 如果不是边界
      if (i < lLineBytes - 1) {
        //向右传播
        temp = (Float) * (lpSrc + 1);

        temp = temp + error * (1.5 / 8.0);

        if (temp > 255.0) {
          temp = 255.0;
        }

        *(lpSrc + 1) = (BYTE) temp;

      }

      // 如果不是边界
      if (j < lHeight - 1) {
        // 向下传播
        temp = (Float) * (lpSrc + lLineBytes);

        temp = temp + error * (1.5 / 8.0);

        *(lpSrc + lLineBytes) = (BYTE) temp;

        if (i < lLineBytes - 1) {
          // 向右下传播
          temp = (Float) * (lpSrc + lLineBytes + 1);

          temp = temp + error * (2.0 / 16.0);

          *(lpSrc + lLineBytes + 1) = (BYTE) temp;
        }
      }

    }

  }

  return TRUE;
}
