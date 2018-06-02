
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "macro.h"
#include "imglib.h"

#define print_err(A, B)  printf(A)

//////////////////////////////////////////////////////////////////////
// 图像边缘与轮廓运算函数
//////////////////////////////////////////////////////////////////////

/*************************************************************************
* 函数名称：
*   Robert()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
* 返回值:
*   BOOL               - 边缘检测成功返回TRUE，否则返回FALSE。
* 说明:
* 该函数用Robert边缘检测算子对图像进行边缘检测运算。
* 要求目标图像为灰度图像。
 ************************************************************************/
BOOL Robert(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc;   // 指向源图像的指针
  LPBYTE lpDst;   // 指向缓存图像的指针
  LPBYTE lpNewDIBBits; // 指向缓存DIB图像的指针
  int i, j;    //循环变量
  Float result;   //像素值
  BYTE pixel[ 4 ];

  // 暂时分配内存，以保存新图像
  lpNewDIBBits = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  // 初始化新分配的内存，设定初始值为255
  lpDst = lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  //使用水平方向的结构元素进行腐蚀
  for (j = lHeight - 1; j > 0; j--) {
    for (i = 0; i < lWidth - 1; i++) {
      //由于使用2×2的模板，为防止越界，所以不处理最下边和最右边的两列像素

      // 指向源图像第j行，第i个象素的指针
      lpSrc = lpDIBBits + lWidth * j + i;
      // 指向目标图像第j行，第i个象素的指针
      lpDst = lpNewDIBBits + lWidth * j + i;

      //取得当前指针处2*2区域的像素值，注意要转换为BYTE型
      pixel[ 0 ] = (BYTE) * lpSrc;
      pixel[ 1 ] = (BYTE) * (lpSrc + 1);
      pixel[ 2 ] = (BYTE) * (lpSrc - lWidth);
      pixel[ 3 ] = (BYTE) * (lpSrc - lWidth + 1);

      //计算目标图像中的当前点
      result = sqrt((pixel[ 0 ] - pixel[ 3 ]) * (pixel[ 0 ] - pixel[ 3 ]) + \
          (pixel[ 1 ] - pixel[ 2 ]) * (pixel[ 1 ] - pixel[ 2 ]));
      *lpDst = (BYTE) result;
    }
  }

  // 复制腐蚀后的图像
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  SAFEFREE(lpNewDIBBits);

  return TRUE;
}

/*************************************************************************
* 函数名称：
*   Sobel()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
* 返回值:
*   BOOL               - 边缘检测成功返回TRUE，否则返回FALSE。
* 说明:
* 该函数用Sobel边缘检测算子对图像进行边缘检测运算。
* 要求目标图像为灰度图像。
 ************************************************************************/
BOOL Sobel(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpDst1;    // 指向缓存图像的指针
  LPBYTE lpDst2;
  LPBYTE lpNewDIBBits1;  // 指向缓存DIB图像的指针
  LPBYTE lpNewDIBBits2;

  int i, j;      // 循环变量
  int iTempH;    // 模板高度
  int iTempW;    // 模板宽度
  Float fTempC;  // 模板系数
  int iTempMX;   // 模板中心元素X坐标
  int iTempMY;   // 模板中心元素Y坐标
  Float aTemplate[ 9 ];   // 模板数组

  // 暂时分配内存，以保存新图像
  lpNewDIBBits1 = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits1 == NULL) {
    return FALSE;
  }

  // 暂时分配内存，以保存新图像
  lpNewDIBBits2 = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits2 == NULL) {
    return FALSE;
  }

  // 拷贝源图像到缓存图像中
  lpDst1 = lpNewDIBBits1;
  memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
  lpDst2 = lpNewDIBBits2;
  memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

  // 设置Sobel模板参数
  iTempW = 3;
  iTempH = 3;
  fTempC = 1.0;
  iTempMX = 1;
  iTempMY = 1;
  aTemplate[ 0 ] = -1.0;
  aTemplate[ 1 ] = -2.0;
  aTemplate[ 2 ] = -1.0;
  aTemplate[ 3 ] = 0.0;
  aTemplate[ 4 ] = 0.0;
  aTemplate[ 5 ] = 0.0;
  aTemplate[ 6 ] = 1.0;
  aTemplate[ 7 ] = 2.0;
  aTemplate[ 8 ] = 1.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits1, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  // 设置Sobel模板参数
  aTemplate[ 0 ] = -1.0;
  aTemplate[ 1 ] = 0.0;
  aTemplate[ 2 ] = 1.0;
  aTemplate[ 3 ] = -2.0;
  aTemplate[ 4 ] = 0.0;
  aTemplate[ 5 ] = 2.0;
  aTemplate[ 6 ] = -1.0;
  aTemplate[ 7 ] = 0.0;
  aTemplate[ 8 ] = 1.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits2, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  //求两幅缓存图像的最大值
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth - 1; i++) {
      // 指向缓存图像1倒数第j行，第i个象素的指针
      lpDst1 = lpNewDIBBits1 + lWidth * j + i;
      // 指向缓存图像2倒数第j行，第i个象素的指针
      lpDst2 = lpNewDIBBits2 + lWidth * j + i;

      if (*lpDst2 > *lpDst1) {
        *lpDst1 = *lpDst2;
      }
    }
  }

  // 复制经过模板运算后的图像到源图像
  memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);

  SAFEFREE(lpNewDIBBits1);
  SAFEFREE(lpNewDIBBits2);

  return TRUE;
}

/*************************************************************************
* 函数名称：
*   Prewitt()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
* 返回值:
*   BOOL               - 边缘检测成功返回TRUE，否则返回FALSE。
* 说明:
* 该函数用Prewitt边缘检测算子对图像进行边缘检测运算。
* 要求目标图像为灰度图像。
 ************************************************************************/
BOOL Prewitt(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpDst1;    // 指向缓存图像的指针
  LPBYTE lpDst2;
  LPBYTE lpNewDIBBits1;  // 指向缓存DIB图像的指针
  LPBYTE lpNewDIBBits2;

  int i, j;     // 循环变量
  int iTempH;    // 模板高度
  int iTempW;    // 模板宽度
  Float fTempC;    // 模板系数
  int iTempMX;   // 模板中心元素X坐标
  int iTempMY;   // 模板中心元素Y坐标
  Float aTemplate[ 9 ];   // 模板数组

  // 暂时分配内存，以保存新图像
  lpNewDIBBits1 = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits1 == NULL) {
    return FALSE;
  }

  // 暂时分配内存，以保存新图像
  lpNewDIBBits2 = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits2 == NULL) {
    return FALSE;
  }

  // 拷贝源图像到缓存图像中
  lpDst1 = lpNewDIBBits1;
  memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
  lpDst2 = lpNewDIBBits2;
  memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);


  // 设置Prewitt模板参数
  iTempW = 3;
  iTempH = 3;
  fTempC = 1.0;
  iTempMX = 1;
  iTempMY = 1;
  aTemplate[ 0 ] = -1.0;
  aTemplate[ 1 ] = -1.0;
  aTemplate[ 2 ] = -1.0;
  aTemplate[ 3 ] = 0.0;
  aTemplate[ 4 ] = 0.0;
  aTemplate[ 5 ] = 0.0;
  aTemplate[ 6 ] = 1.0;
  aTemplate[ 7 ] = 1.0;
  aTemplate[ 8 ] = 1.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits1, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  // 设置Prewitt模板参数
  aTemplate[ 0 ] = 1.0;
  aTemplate[ 1 ] = 0.0;
  aTemplate[ 2 ] = -1.0;
  aTemplate[ 3 ] = 1.0;
  aTemplate[ 4 ] = 0.0;
  aTemplate[ 5 ] = -1.0;
  aTemplate[ 6 ] = 1.0;
  aTemplate[ 7 ] = 0.0;
  aTemplate[ 8 ] = -1.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits2, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  //求两幅缓存图像的最大值
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth - 1; i++) {
      // 指向缓存图像1倒数第j行，第i个象素的指针
      lpDst1 = lpNewDIBBits1 + lWidth * j + i;
      // 指向缓存图像2倒数第j行，第i个象素的指针
      lpDst2 = lpNewDIBBits2 + lWidth * j + i;

      if (*lpDst2 > *lpDst1) {
        *lpDst1 = *lpDst2;
      }
    }
  }

  // 复制经过模板运算后的图像到源图像
  memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);

  SAFEFREE(lpNewDIBBits1);
  SAFEFREE(lpNewDIBBits2);

  return TRUE;
}

/*************************************************************************
* 函数名称：
*   Kirsch()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
* 返回值:
*   BOOL               - 边缘检测成功返回TRUE，否则返回FALSE。
* 说明:
* 该函数用kirsch边缘检测算子对图像进行边缘检测运算。
* 要求目标图像为灰度图像。
 ************************************************************************/
BOOL Kirsch(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpDst1;   // 指向缓存图像的指针
  LPBYTE lpDst2;
  LPBYTE lpNewDIBBits1; // 指向缓存DIB图像的指针
  LPBYTE lpNewDIBBits2;

  int i, j;    // 循环变量
  int iTempH;   // 模板高度
  int iTempW;   // 模板宽度
  Float fTempC;   // 模板系数
  int iTempMX;  // 模板中心元素X坐标
  int iTempMY;  // 模板中心元素Y坐标
  Float aTemplate[ 9 ];  // 模板数组

  // 暂时分配内存，以保存新图像
  lpNewDIBBits1 = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits1 == NULL) {
    return FALSE;
  }

  // 暂时分配内存，以保存新图像
  lpNewDIBBits2 = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits2 == NULL) {
    return FALSE;
  }

  // 拷贝源图像到缓存图像中
  lpDst1 = lpNewDIBBits1;
  memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
  lpDst2 = lpNewDIBBits2;
  memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

  // 设置Kirsch模板1参数
  iTempW = 3;
  iTempH = 3;
  fTempC = 1.0;
  iTempMX = 1;
  iTempMY = 1;
  aTemplate[ 0 ] = 5.0;
  aTemplate[ 1 ] = 5.0;
  aTemplate[ 2 ] = 5.0;
  aTemplate[ 3 ] = -3.0;
  aTemplate[ 4 ] = 0.0;
  aTemplate[ 5 ] = -3.0;
  aTemplate[ 6 ] = -3.0;
  aTemplate[ 7 ] = -3.0;
  aTemplate[ 8 ] = -3.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits1, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  // 设置Kirsch模板2参数
  aTemplate[ 0 ] = -3.0;
  aTemplate[ 1 ] = 5.0;
  aTemplate[ 2 ] = 5.0;
  aTemplate[ 3 ] = -3.0;
  aTemplate[ 4 ] = 0.0;
  aTemplate[ 5 ] = 5.0;
  aTemplate[ 6 ] = -3.0;
  aTemplate[ 7 ] = -3.0;
  aTemplate[ 8 ] = -3.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits2, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  //求两幅缓存图像的最大值
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth - 1; i++) {
      // 指向缓存图像1倒数第j行，第i个象素的指针
      lpDst1 = lpNewDIBBits1 + lWidth * j + i;
      // 指向缓存图像2倒数第j行，第i个象素的指针
      lpDst2 = lpNewDIBBits2 + lWidth * j + i;

      if (*lpDst2 > *lpDst1) {
        *lpDst1 = *lpDst2;
      }
    }
  }

  // 拷贝源图像到缓存图像中
  memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

  // 设置Kirsch模板3参数
  aTemplate[ 0 ] = -3.0;
  aTemplate[ 1 ] = -3.0;
  aTemplate[ 2 ] = 5.0;
  aTemplate[ 3 ] = -3.0;
  aTemplate[ 4 ] = 0.0;
  aTemplate[ 5 ] = 5.0;
  aTemplate[ 6 ] = -3.0;
  aTemplate[ 7 ] = -3.0;
  aTemplate[ 8 ] = 5.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits2, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  //求两幅缓存图像的最大值
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth - 1; i++) {
      // 指向缓存图像1倒数第j行，第i个象素的指针
      lpDst1 = lpNewDIBBits1 + lWidth * j + i;
      // 指向缓存图像2倒数第j行，第i个象素的指针
      lpDst2 = lpNewDIBBits2 + lWidth * j + i;

      if (*lpDst2 > *lpDst1) {
        *lpDst1 = *lpDst2;
      }
    }
  }

  // 拷贝源图像到缓存图像中
  memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

  // 设置Kirsch模板4参数
  aTemplate[ 0 ] = -3.0;
  aTemplate[ 1 ] = -3.0;
  aTemplate[ 2 ] = -3.0;
  aTemplate[ 3 ] = -3.0;
  aTemplate[ 4 ] = 0.0;
  aTemplate[ 5 ] = 5.0;
  aTemplate[ 6 ] = -3.0;
  aTemplate[ 7 ] = 5.0;
  aTemplate[ 8 ] = 5.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits2, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  //求两幅缓存图像的最大值
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth - 1; i++) {
      // 指向缓存图像1倒数第j行，第i个象素的指针
      lpDst1 = lpNewDIBBits1 + lWidth * j + i;
      // 指向缓存图像2倒数第j行，第i个象素的指针
      lpDst2 = lpNewDIBBits2 + lWidth * j + i;

      if (*lpDst2 > *lpDst1) {
        *lpDst1 = *lpDst2;
      }
    }
  }

  // 拷贝源图像到缓存图像中
  memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

  // 设置Kirsch模板5参数
  aTemplate[ 0 ] = -3.0;
  aTemplate[ 1 ] = -3.0;
  aTemplate[ 2 ] = -3.0;
  aTemplate[ 3 ] = -3.0;
  aTemplate[ 4 ] = 0.0;
  aTemplate[ 5 ] = -3.0;
  aTemplate[ 6 ] = 5.0;
  aTemplate[ 7 ] = 5.0;
  aTemplate[ 8 ] = 5.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits2, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  // 拷贝源图像到缓存图像中
  memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

  //求两幅缓存图像的最大值
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth - 1; i++) {
      // 指向缓存图像1倒数第j行，第i个象素的指针
      lpDst1 = lpNewDIBBits1 + lWidth * j + i;
      // 指向缓存图像2倒数第j行，第i个象素的指针
      lpDst2 = lpNewDIBBits2 + lWidth * j + i;

      if (*lpDst2 > *lpDst1) {
        *lpDst1 = *lpDst2;
      }
    }
  }

  // 拷贝源图像到缓存图像中
  memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

  // 设置Kirsch模板6参数
  aTemplate[ 0 ] = -3.0;
  aTemplate[ 1 ] = -3.0;
  aTemplate[ 2 ] = -3.0;
  aTemplate[ 3 ] = 5.0;
  aTemplate[ 4 ] = 0.0;
  aTemplate[ 5 ] = -3.0;
  aTemplate[ 6 ] = 5.0;
  aTemplate[ 7 ] = 5.0;
  aTemplate[ 8 ] = -3.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits2, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  //求两幅缓存图像的最大值
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth - 1; i++) {
      // 指向缓存图像1倒数第j行，第i个象素的指针
      lpDst1 = lpNewDIBBits1 + lWidth * j + i;
      // 指向缓存图像2倒数第j行，第i个象素的指针
      lpDst2 = lpNewDIBBits2 + lWidth * j + i;

      if (*lpDst2 > *lpDst1) {
        *lpDst1 = *lpDst2;
      }
    }
  }

  // 拷贝源图像到缓存图像中
  memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

  // 设置Kirsch模板7参数
  aTemplate[ 0 ] = 5.0;
  aTemplate[ 1 ] = -3.0;
  aTemplate[ 2 ] = -3.0;
  aTemplate[ 3 ] = 5.0;
  aTemplate[ 4 ] = 0.0;
  aTemplate[ 5 ] = -3.0;
  aTemplate[ 6 ] = 5.0;
  aTemplate[ 7 ] = -3.0;
  aTemplate[ 8 ] = -3.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits2, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  //求两幅缓存图像的最大值
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth - 1; i++) {
      // 指向缓存图像1倒数第j行，第i个象素的指针
      lpDst1 = lpNewDIBBits1 + lWidth * j + i;
      // 指向缓存图像2倒数第j行，第i个象素的指针
      lpDst2 = lpNewDIBBits2 + lWidth * j + i;

      if (*lpDst2 > *lpDst1) {
        *lpDst1 = *lpDst2;
      }
    }
  }

  // 拷贝源图像到缓存图像中
  memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

  // 设置Kirsch模板8参数
  aTemplate[ 0 ] = 5.0;
  aTemplate[ 1 ] = 5.0;
  aTemplate[ 2 ] = -3.0;
  aTemplate[ 3 ] = 5.0;
  aTemplate[ 4 ] = 0.0;
  aTemplate[ 5 ] = -3.0;
  aTemplate[ 6 ] = -3.0;
  aTemplate[ 7 ] = -3.0;
  aTemplate[ 8 ] = -3.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits2, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  //求两幅缓存图像的最大值
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth - 1; i++) {
      // 指向缓存图像1倒数第j行，第i个象素的指针
      lpDst1 = lpNewDIBBits1 + lWidth * j + i;
      // 指向缓存图像2倒数第j行，第i个象素的指针
      lpDst2 = lpNewDIBBits2 + lWidth * j + i;

      if (*lpDst2 > *lpDst1) {
        *lpDst1 = *lpDst2;
      }
    }
  }

  // 复制经过模板运算后的图像到源图像
  memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);

  SAFEFREE(lpNewDIBBits1);
  SAFEFREE(lpNewDIBBits2);

  return TRUE;
}

/*************************************************************************
* 函数名称：
*  Gauss()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
* 返回值:
*   BOOL               - 边缘检测成功返回TRUE，否则返回FALSE。
* 说明:
* 该函数用高斯拉普拉斯边缘检测算子对图像进行边缘检测运算。
* 要求目标图像为灰度图像。
 ************************************************************************/
BOOL Gauss(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpDst1;    // 指向缓存图像的指针
  LPBYTE lpDst2;
  LPBYTE lpNewDIBBits1;  // 指向缓存DIB图像的指针
  LPBYTE lpNewDIBBits2;

  int iTempH;    // 模板高度
  int iTempW;    // 模板宽度
  Float fTempC;    // 模板系数
  int iTempMX;   // 模板中心元素X坐标
  int iTempMY;   // 模板中心元素Y坐标
  Float aTemplate[ 25 ];  //模板数组

  // 暂时分配内存，以保存新图像
  lpNewDIBBits1 = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits1 == NULL) {
    return FALSE;
  }

  // 暂时分配内存，以保存新图像
  lpNewDIBBits2 = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits2 == NULL) {
    return FALSE;
  }

  // 拷贝源图像到缓存图像中
  lpDst1 = lpNewDIBBits1;
  memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
  lpDst2 = lpNewDIBBits2;
  memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

  // 设置Gauss模板参数
  iTempW = 5;
  iTempH = 5;
  fTempC = 1.0;
  iTempMX = 3;
  iTempMY = 3;
  aTemplate[ 0 ] = -2.0;
  aTemplate[ 1 ] = -4.0;
  aTemplate[ 2 ] = -4.0;
  aTemplate[ 3 ] = -4.0;
  aTemplate[ 4 ] = -2.0;
  aTemplate[ 5 ] = -4.0;
  aTemplate[ 6 ] = 0.0;
  aTemplate[ 7 ] = 8.0;
  aTemplate[ 8 ] = 0.0;
  aTemplate[ 9 ] = -4.0;
  aTemplate[ 10 ] = -4.0;
  aTemplate[ 11 ] = 8.0;
  aTemplate[ 12 ] = 24.0;
  aTemplate[ 13 ] = 8.0;
  aTemplate[ 14 ] = -4.0;
  aTemplate[ 15 ] = -4.0;
  aTemplate[ 16 ] = 0.0;
  aTemplate[ 17 ] = 8.0;
  aTemplate[ 18 ] = 0.0;
  aTemplate[ 19 ] = -4.0;
  aTemplate[ 20 ] = -2.0;
  aTemplate[ 21 ] = -4.0;
  aTemplate[ 22 ] = -4.0;
  aTemplate[ 23 ] = -4.0;
  aTemplate[ 24 ] = -2.0;

  if (!Template(lHeight, lWidth, lpNewDIBBits1, lLineBytes,
      iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC)) {
    return FALSE;
  }

  // 复制经过模板运算后的图像到源图像
  memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);

  SAFEFREE(lpNewDIBBits1);
  SAFEFREE(lpNewDIBBits2);

  return TRUE;
}

/*************************************************************************
* 函数名称：
*   Hough()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
*   int lLineBytes   - 图像每行的字节数
* 返回值:
*   BOOL               - 运算成功返回TRUE，否则返回FALSE。
* 说明:
* 该函数用于对检测图像中的平行直线。如果图像中有两条平行的直线，
* 则将这两条平行直线提取出来。
* 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/
BOOL Hough(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc;    // 指向源图像的指针
  LPBYTE lpDst;    // 指向缓存图像的指针
  LPBYTE lpTrans;   // 指向变换域的指针
  LPBYTE lpNewDIBBits;  // 指向缓存DIB图像的指针
  LPBYTE lpTransArea;  // 指向变换域的指针

  int iMaxDist;   // 变换域的尺寸
  int iMaxAngleNumber;
  int iDist;    // 变换域的坐标
  int iAngleNumber;
  int i, j;    // 循环变量
  BYTE pixel;  // 像素值
  struct {
    int Value;
    int Dist;
    int AngleNumber;
  }
  MaxValue1, MaxValue2; // 存储变换域中的两个最大值

  // 暂时分配内存，以保存新图像
  lpNewDIBBits = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  // 初始化新分配的内存，设定初始值为255
  lpDst = lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  //计算变换域的尺寸 最大距离
  iMaxDist = (int) sqrt(lWidth * lWidth + lHeight * lHeight);
  //角度从0－180，每格2度
  iMaxAngleNumber = 90;

  //为变换域分配内存
  lpTransArea = MALLOC(BYTE, lWidth * lHeight * sizeof(int));

  if (lpTransArea == NULL) {
    return FALSE;
  }

  // 初始化新分配的内存，设定初始值为0
  lpTrans = lpTransArea;
  memset(lpTrans, 0, lWidth * lHeight * sizeof(int));

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = lpDIBBits + lLineBytes * j + i;

      //取得当前指针处的像素值，注意要转换为BYTE型
      pixel = (BYTE) * lpSrc;

      //目标图像中含有0和255外的其它灰度值
      if (pixel != 255 && *lpSrc != 0) {
        return FALSE;
      }

      //如果是黑点，则在变换域的对应各点上加1
      if (pixel == 0) {
        //注意步长是2度
        for (iAngleNumber = 0; iAngleNumber < iMaxAngleNumber; iAngleNumber++) {
          iDist = (int) fabs(i * cos(iAngleNumber * 2 * PI / 180.0) + \
              j * sin(iAngleNumber * 2 * PI / 180.0));

          //变换域的对应点上加1
          *(lpTransArea + iDist * iMaxAngleNumber + iAngleNumber) = \
              *(lpTransArea + iDist * iMaxAngleNumber + iAngleNumber) + 1;
        }
      }
    }
  }

  //找到变换域中的两个最大值点
  MaxValue1.Value = 0;
  MaxValue2.Value = 0;

  //找到第一个最大值点
  for (iDist = 0; iDist < iMaxDist; iDist++) {
    for (iAngleNumber = 0; iAngleNumber < iMaxAngleNumber; iAngleNumber++) {
      if ((int) * (lpTransArea + iDist * iMaxAngleNumber + iAngleNumber) > MaxValue1.Value) {
        MaxValue1.Value = (int) * (lpTransArea + iDist * iMaxAngleNumber
            + iAngleNumber);
        MaxValue1.Dist = iDist;
        MaxValue1.AngleNumber = iAngleNumber;
      }
    }
  }

  //将第一个最大值点附近清零
  for (iDist = -9; iDist < 10; iDist++) {
    for (iAngleNumber = -1; iAngleNumber < 2; iAngleNumber++) {
      if (iDist + MaxValue1.Dist >= 0 && iDist + MaxValue1.Dist < iMaxDist \
          && iAngleNumber + MaxValue1.AngleNumber >= 0 && iAngleNumber
          + MaxValue1.AngleNumber <= iMaxAngleNumber) {
        *(lpTransArea + (iDist + MaxValue1.Dist) * iMaxAngleNumber + \
            (iAngleNumber + MaxValue1.AngleNumber)) = 0;
      }
    }
  }

  //找到第二个最大值点
  for (iDist = 0; iDist < iMaxDist; iDist++) {
    for (iAngleNumber = 0; iAngleNumber < iMaxAngleNumber; iAngleNumber++) {
      if ((int) * (lpTransArea + iDist * iMaxAngleNumber + iAngleNumber) > MaxValue2.Value) {
        MaxValue2.Value = (int) * (lpTransArea + iDist * iMaxAngleNumber
            + iAngleNumber);
        MaxValue2.Dist = iDist;
        MaxValue2.AngleNumber = iAngleNumber;
      }
    }
  }

  //判断两直线是否平行
  if (abs(MaxValue1.AngleNumber - MaxValue2.AngleNumber) <= 2) {
    //两直线平行，在缓存图像中重绘这两条直线
    for (j = 0; j < lHeight; j++) {
      for (i = 0; i < lWidth; i++) {
        // 指向缓存图像倒数第j行，第i个象素的指针
        lpDst = lpNewDIBBits + lLineBytes * j + i;

        //如果该点在某一条平行直线上，则在缓存图像上将该点赋为黑

        //在第一条直线上
        iDist = (int) fabs(i * cos(MaxValue1.AngleNumber * 2 * PI / 180.0) + \
            j * sin(MaxValue1.AngleNumber * 2 * PI / 180.0));

        if (iDist == MaxValue1.Dist) {
          *lpDst = (BYTE) 0;
        }

        //在第二条直线上
        iDist = (int) fabs(i * cos(MaxValue2.AngleNumber * 2 * PI / 180.0) + \
            j * sin(MaxValue2.AngleNumber * 2 * PI / 180.0));

        if (iDist == MaxValue2.Dist) {
          *lpDst = (BYTE) 0;
        }
      }
    }
  }

  // 复制腐蚀后的图像
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  SAFEFREE(lpNewDIBBits);
  SAFEFREE(lpTransArea);

  return TRUE;
}

/*************************************************************************
* 函数名称：
*   Fill2()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
* 返回值:
*   BOOL               - 种子填充成功返回TRUE，否则返回FALSE。
* 说明:
* 该函数用于对图像进行种子填充运算。
* 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/
BOOL Fill2(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc;   // 指向源图像的指针
  int i;     // 循环变量
  BYTE pixel; // 像素值
  int xl, xr;    // 左右边界像素位置
  BOOL bFilll, bFillr;  // 是否已填充至边界
  IPOINT Seeds[ 10 ];   // 种子堆栈及指针
  int StackPoint;
  int iCurrentPixelx, iCurrentPixely; // 当前像素位置
  int iBufferPixelx, iBufferPixely;

  Seeds[ 1 ].y = lHeight / 2;  // 初始化种子
  Seeds[ 1 ].x = lWidth / 2;
  StackPoint = 1;

  while (StackPoint != 0) {
    //取出种子
    iCurrentPixelx = Seeds[ StackPoint ].x;
    iCurrentPixely = Seeds[ StackPoint ].y;
    StackPoint--;
    //  if(Seed2.Height== 75)
    //  {
    //   return TRUE;
    //   i++;
    //  }
    bFilll = TRUE;
    bFillr = TRUE;
    //填充种子所在的行，保存种子像素的位置
    iBufferPixelx = iCurrentPixelx;
    iBufferPixely = iCurrentPixely;

    //先向左填充
    while (bFilll) {
      lpSrc = lpDIBBits + lWidth * iCurrentPixely + iCurrentPixelx;
      //取得当前指针处的像素值，注意要转换为BYTE型
      pixel = (BYTE) * lpSrc;

      //目标图像中含有0和255外的其它灰度值
      if (pixel != 255 && pixel != 0) {
        return FALSE;
      }

      //遇到边界
      if (pixel == 0) {
        bFilll = FALSE;
        xl = iCurrentPixelx + 1;
      }
      else {
        *lpSrc = (BYTE) 0;
        iCurrentPixelx--;

        //防止越界
        if (iCurrentPixelx < 0) {
          bFilll = FALSE;
          iCurrentPixelx = 0;
          xl = 0;
        }
      }
    }

    //再向右填充，取回种子像素的位置
    iCurrentPixelx = iBufferPixelx + 1;

    if (iCurrentPixelx > lWidth) {
      bFillr = FALSE;
      iCurrentPixelx = lWidth;
      xr = lWidth;
    }

    iCurrentPixely = iBufferPixely;

    while (bFillr) {
      lpSrc = lpDIBBits + lWidth * iCurrentPixely + iCurrentPixelx;
      //取得当前指针处的像素值，注意要转换为BYTE型
      pixel = (BYTE) * lpSrc;

      //目标图像中含有0和255外的其它灰度值
      if (pixel != 255 && pixel != 0) {
        return FALSE;
      }

      //遇到边界
      if (pixel == 0) {
        bFillr = FALSE;
        xr = iCurrentPixelx - 1;
      }
      else {
        *lpSrc = (BYTE) 0;
        iCurrentPixelx++;

        //防止越界
        if (iCurrentPixelx > lWidth) {
          bFillr = FALSE;
          iCurrentPixelx = lWidth;
          xr = lWidth;
        }
      }
    }

    //上、下两条扫描线是否全为边界象素或已填充过
    //先看上面的扫描线
    iCurrentPixely--;

    if (iCurrentPixely < 0) {
      iCurrentPixely = 0;
    }

    for (i = xr; i >= xl; i--) {
      lpSrc = lpDIBBits + lWidth * iCurrentPixely + i;
      //取得当前指针处的像素值，注意要转换为BYTE型
      pixel = (BYTE) * lpSrc;

      //有未填充的像素，将新的种子压入堆栈
      if (pixel == 255) {
        StackPoint++;
        Seeds[ StackPoint ].y = iCurrentPixely;
        Seeds[ StackPoint ].x = i;
        break;
      }
    }

    //再看下面的扫描线
    iCurrentPixely += 2;

    if (iCurrentPixely > lHeight) {
      iCurrentPixely = lHeight;
    }

    for (i = xr; i >= xl; i--) {
      lpSrc = lpDIBBits + lWidth * iCurrentPixely + i;
      //取得当前指针处的像素值，注意要转换为BYTE型
      pixel = (BYTE) * lpSrc;

      //有未填充的像素，将新的种子压入堆栈
      if (pixel == 255) {
        StackPoint++;
        Seeds[ StackPoint ].y = iCurrentPixely;
        Seeds[ StackPoint ].x = i;
        break;
      }
    }
  }

  return TRUE;
}

/*************************************************************************
* 函数名称：
*   Fill()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
* 返回值:
*   BOOL               - 种子填充成功返回TRUE，否则返回FALSE。
* 说明:
* 该函数用于对图像进行种子填充运算。
* 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/
BOOL Fill(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc;    // 指向源图像的指针
  BYTE pixel;  // 像素值
  IPOINT* Seeds;    // 种子堆栈及指针
  int StackPoint;
  int iCurrentPixelx, iCurrentPixely; // 当前像素位置

  Seeds = MALLOC(IPOINT, lWidth * lHeight);   // 初始化种子
  Seeds[ 1 ].y = lHeight / 2;
  Seeds[ 1 ].x = lWidth / 2;
  StackPoint = 1;

  while (StackPoint != 0) {
    //取出种子
    iCurrentPixelx = Seeds[ StackPoint ].x;
    iCurrentPixely = Seeds[ StackPoint ].y;
    StackPoint--;

    lpSrc = lpDIBBits + lWidth * iCurrentPixely + iCurrentPixelx;
    //取得当前指针处的像素值，注意要转换为BYTE型
    pixel = (BYTE) * lpSrc;

    //目标图像中含有0和255外的其它灰度值
    if (pixel != 255 && pixel != 0) {
      return FALSE;
    }

    //将当前点涂黑
    *lpSrc = (BYTE) 0;

    //判断左面的点，如果为白，则压入堆栈，注意防止越界
    if (iCurrentPixelx > 0) {
      lpSrc = lpDIBBits + lWidth * iCurrentPixely + iCurrentPixelx - 1;
      //取得当前指针处的像素值，注意要转换为BYTE型
      pixel = (BYTE) * lpSrc;

      if (pixel == 255) {
        StackPoint++;
        Seeds[ StackPoint ].y = iCurrentPixely;
        Seeds[ StackPoint ].x = iCurrentPixelx - 1;
      }

      //目标图像中含有0和255外的其它灰度值
      if (pixel != 255 && pixel != 0) {
        return FALSE;
      }
    }

    //判断上面的点，如果为白，则压入堆栈，注意防止越界
    if (iCurrentPixely < lHeight - 1) {
      lpSrc = lpDIBBits + lWidth * (iCurrentPixely + 1) + iCurrentPixelx;
      //取得当前指针处的像素值，注意要转换为BYTE型
      pixel = (BYTE) * lpSrc;

      if (pixel == 255) {
        StackPoint++;
        Seeds[ StackPoint ].y = iCurrentPixely + 1;
        Seeds[ StackPoint ].x = iCurrentPixelx;
      }

      //目标图像中含有0和255外的其它灰度值
      if (pixel != 255 && pixel != 0) {
        return FALSE;
      }
    }

    //判断右面的点，如果为白，则压入堆栈，注意防止越界
    if (iCurrentPixelx < lWidth - 1) {
      lpSrc = lpDIBBits + lWidth * iCurrentPixely + iCurrentPixelx + 1;
      //取得当前指针处的像素值，注意要转换为BYTE型
      pixel = (BYTE) * lpSrc;

      if (pixel == 255) {
        StackPoint++;
        Seeds[ StackPoint ].y = iCurrentPixely;
        Seeds[ StackPoint ].x = iCurrentPixelx + 1;
      }

      //目标图像中含有0和255外的其它灰度值
      if (pixel != 255 && pixel != 0) {
        return FALSE;
      }
    }

    //判断下面的点，如果为白，则压入堆栈，注意防止越界
    if (iCurrentPixely > 0) {
      lpSrc = lpDIBBits + lWidth * (iCurrentPixely - 1) + iCurrentPixelx;
      //取得当前指针处的像素值，注意要转换为BYTE型
      pixel = (BYTE) * lpSrc;

      if (pixel == 255) {
        StackPoint++;
        Seeds[ StackPoint ].y = iCurrentPixely - 1;
        Seeds[ StackPoint ].x = iCurrentPixelx;
      }

      //目标图像中含有0和255外的其它灰度值
      if (pixel != 255 && pixel != 0) {
        return FALSE;
      }
    }
  }

  SAFEFREE(Seeds);

  return TRUE;
}

/*************************************************************************
* 函数名称：
*   Contour()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
* 返回值:
*   BOOL               - 运算成功返回TRUE，否则返回FALSE。
* 说明:
* 该函数用于对图像进行轮廓提取运算。
* 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/
BOOL Contour(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc;    // 指向源图像的指针
  LPBYTE lpDst;    // 指向缓存图像的指针
  LPBYTE lpNewDIBBits;  // 指向缓存DIB图像的指针
  int i, j;     //循环变量
  BYTE n, e, s, w, ne, se, nw, sw;
  BYTE pixel;  //像素值

  // 暂时分配内存，以保存新图像
  lpNewDIBBits = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  // 初始化新分配的内存，设定初始值为255
  lpDst = lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  for (j = 1; j < lHeight - 1; j++) {
    for (i = 1; i < lWidth - 1; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = lpDIBBits + lWidth * j + i;
      // 指向目标图像倒数第j行，第i个象素的指针
      lpDst = lpNewDIBBits + lWidth * j + i;
      //取得当前指针处的像素值，注意要转换为BYTE型
      pixel = (BYTE) * lpSrc;

      //目标图像中含有0和255外的其它灰度值
      //   if(pixel != 255 && pixel != 0)
      //    return FALSE;
      if (pixel == 0) {
        *lpDst = (BYTE) 0;
        nw = (BYTE) * (lpSrc + lWidth - 1);
        n = (BYTE) * (lpSrc + lWidth);
        ne = (BYTE) * (lpSrc + lWidth + 1);
        w = (BYTE) * (lpSrc - 1);
        e = (BYTE) * (lpSrc + 1);
        sw = (BYTE) * (lpSrc - lWidth - 1);
        s = (BYTE) * (lpSrc - lWidth);
        se = (BYTE) * (lpSrc - lWidth + 1);

        //如果相邻的八个点都是黑点
        if (nw + n + ne + w + e + sw + s + se == 0) {
          *lpDst = (BYTE) 255;
        }
      }
    }
  }

  // 复制腐蚀后的图像
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  SAFEFREE(lpNewDIBBits);

  return TRUE;
}

/*************************************************************************
* 函数名称：
*   Trace()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
*   int lLineBytes   - 图像每行的字节数
* 返回值:
*   BOOL               - 运算成功返回TRUE，否则返回FALSE。
* 说明:
* 该函数用于对图像进行轮廓跟踪运算。
* 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/
BOOL Trace(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc;    // 指向源图像的指针
  LPBYTE lpDst;    // 指向缓存图像的指针
  LPBYTE lpNewDIBBits;  // 指向缓存DIB图像的指针
  int i, j;     // 循环变量
  BYTE pixel;  // 像素值
  BOOL bFindStartPoint;  // 是否找到起始点及回到起始点
  BOOL bFindPoint;   // 是否扫描到一个边界点
  IPOINT StartPoint, CurrentPoint; // 起始边界点与当前边界点

  //八个方向和起始扫描方向
  int Direction[ 8 ][ 2 ] = {{ -1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, { -1, -1}, { -1, 0}};
  int BeginDirect;

  // 暂时分配内存，以保存新图像
  lpNewDIBBits = MALLOC(BYTE, lLineBytes * lHeight);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  // 初始化新分配的内存，设定初始值为255
  lpDst = lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lLineBytes * lHeight);

  //先找到最左上方的边界点
  bFindStartPoint = FALSE;

  for (j = 0; j < lHeight && !bFindStartPoint; j++) {
    for (i = 0; i < lWidth && !bFindStartPoint; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = lpDIBBits + lLineBytes * j + i;
      //取得当前指针处的像素值，注意要转换为BYTE型
      pixel = (BYTE) * lpSrc;

      if (pixel == 0) {
        bFindStartPoint = TRUE;
        StartPoint.y = j;
        StartPoint.x = i;

        // 指向目标图像倒数第j行，第i个象素的指针
        lpDst = lpNewDIBBits + lLineBytes * j + i;
        *lpDst = (BYTE) 0;
      }
    }
  }

  //由于起始点是在左下方，故起始扫描沿左上方向
  BeginDirect = 0;
  //跟踪边界
  bFindStartPoint = FALSE;
  //从初始点开始扫描
  CurrentPoint.y = StartPoint.y;
  CurrentPoint.x = StartPoint.x;

  while (!bFindStartPoint) {
    bFindPoint = FALSE;

    while (!bFindPoint) {
      //沿扫描方向查看一个像素
      lpSrc = lpDIBBits + lLineBytes * (CurrentPoint.y + Direction[ BeginDirect ][ 1 ]) +
          (CurrentPoint.x + Direction[ BeginDirect ][ 0 ]);
      pixel = (BYTE) * lpSrc;

      if (pixel == 0) {
        bFindPoint = TRUE;
        CurrentPoint.y = CurrentPoint.y + Direction[ BeginDirect ][ 1 ];
        CurrentPoint.x = CurrentPoint.x + Direction[ BeginDirect ][ 0 ];

        if (CurrentPoint.y == StartPoint.y && CurrentPoint.x
            == StartPoint.x) {
          bFindStartPoint = TRUE;
        }

        lpDst = lpNewDIBBits + lLineBytes * CurrentPoint.y
            + CurrentPoint.x;
        *lpDst = (BYTE) 0;
        //扫描的方向逆时针旋转两格
        BeginDirect--;

        if (BeginDirect == -1) {
          BeginDirect = 7;
        }

        BeginDirect--;

        if (BeginDirect == -1) {
          BeginDirect = 7;
        }
      }
      else {
        //扫描方向顺时针旋转一格
        BeginDirect++;

        if (BeginDirect == 8) {
          BeginDirect = 0;
        }
      }
    }
  }

  // 复制腐蚀后的图像
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  SAFEFREE(lpNewDIBBits);

  return TRUE;
}


/*************************************************************************
* 函数名称：
*   EdgeDetection()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
*     int mode            - 算子类型
* 返回值:
*   BOOL               - 边缘检测成功返回TRUE，否则返回FALSE。
 *
* 说明:
* 该函数用Sobel、Isotropic Sobel、LOG边缘检测算子对图像进行边缘检测运算。
* 要求目标图像为灰度图像。
 ************************************************************************/
BOOL EdgeDetection(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int mode)
{
  int nTempWidth;        //模板的宽度
  int nTempHeight;     // 模板的高度
  int nTempCenX;        //模板中心的X坐标（相对于模板）
  int nTempCenY;        //模板中心的Y坐标（相对于模板）
  Float* Tempdata;    //模板数组的指针
  Float TempCoef;    //模板系数
  //循环变量
  int i;
  int j;

  if (mode == 0) {         //Sobel算子
    // 指向缓存图像的指针
    LPBYTE lpDst1;
    LPBYTE lpDst2;
    // 指向存贮图像的指针
    LPBYTE lpImage1;
    LPBYTE lpImage2;

    // 暂时分配内存，以保存新图像
    lpImage1 = (LPBYTE) MALLOC(char, lWidth * lHeight);
    lpImage2 = (LPBYTE) MALLOC(char, lWidth * lHeight);

    // 判断是否内存分配失败
    if (lpImage1 == NULL) {
      // 分配内存失败
      return FALSE;
    }

    // 判断是否内存分配失败
    if (lpImage2 == NULL) {
      // 分配内存失败
      return FALSE;
    }

    // 初始化图像为原始图像
    memcpy(lpImage1, lpDIBBits, lWidth * lHeight);
    memcpy(lpImage2, lpDIBBits, lWidth * lHeight);
    lpDst1 = (LPBYTE) lpImage1;
    lpDst2 = (LPBYTE) lpImage2;
    //Sobel算子水平检测模板参数
    nTempWidth = 3;
    nTempHeight = 3;
    nTempCenX = 1;
    nTempCenY = 1;
    TempCoef = 1;
    Tempdata = MALLOC(Float, 9);   //Sobel算子模板为3X3
    //水平向模板参数
    Tempdata[ 0 ] = -1;
    Tempdata[ 1 ] = -2;
    Tempdata[ 2 ] = -1;
    Tempdata[ 3 ] = 0;
    Tempdata[ 4 ] = 0;
    Tempdata[ 5 ] = 0;
    Tempdata[ 6 ] = 1;
    Tempdata[ 7 ] = 2;
    Tempdata[ 8 ] = 1;

    if (!Template(lHeight, lWidth, lpImage1, lLineBytes, nTempWidth, nTempHeight,
        nTempCenX, nTempCenY, Tempdata, TempCoef)) {
      return FALSE;
    }

    //垂直方向模板参数
    Tempdata[ 0 ] = -1;
    Tempdata[ 1 ] = 0;
    Tempdata[ 2 ] = 1;
    Tempdata[ 3 ] = -2;
    Tempdata[ 4 ] = 0;
    Tempdata[ 5 ] = 2;
    Tempdata[ 6 ] = -1;
    Tempdata[ 7 ] = 0;
    Tempdata[ 8 ] = 1;

    if (!Template(lHeight, lWidth, lpImage2, lLineBytes, nTempWidth, nTempHeight,
        nTempCenX, nTempCenY, Tempdata, TempCoef)) {
      return FALSE;
    }

    SAFEFREE(Tempdata);

    //求两幅缓存图像的最大值
    for (j = 0; j < lHeight; j++) {
      for (i = 0; i < lWidth; i++) {

        // 指向缓存图像1倒数第j行，第i个象素的指针
        lpDst1 = (LPBYTE) lpImage1 + lWidth * j + i;

        // 指向缓存图像2倒数第j行，第i个象素的指针
        lpDst2 = (LPBYTE) lpImage2 + lWidth * j + i;

        if (*lpDst2 > *lpDst1) {
          *lpDst1 = *lpDst2;
        }

      }
    }

    //复制经过模板运算后的图像到源图像
    memcpy(lpDIBBits, lpImage1, lWidth * lHeight);
    return TRUE;
  }

  if (mode == 1) {   //Isotropic Sobel算子
    // 指向缓存图像的指针
    LPBYTE lpDst1;
    LPBYTE lpDst2;
    // 指向存贮图像的指针
    LPBYTE lpImage1;
    LPBYTE lpImage2;

    // 暂时分配内存，以保存新图像
    lpImage1 = (LPBYTE) MALLOC(char, lWidth * lHeight);
    lpImage2 = (LPBYTE) MALLOC(char, lWidth * lHeight);

    // 判断是否内存分配失败
    if (lpImage1 == NULL) {
      // 分配内存失败
      return FALSE;
    }

    // 判断是否内存分配失败
    if (lpImage2 == NULL) {
      // 分配内存失败
      return FALSE;
    }

    // 初始化图像为原始图像
    memcpy(lpImage1, lpDIBBits, lWidth * lHeight);
    memcpy(lpImage2, lpDIBBits, lWidth * lHeight);
    lpDst1 = (LPBYTE) lpImage1;
    lpDst2 = (LPBYTE) lpImage2;
    //Sobel算子水平检测模板参数
    nTempWidth = 3;
    nTempHeight = 3;
    nTempCenX = 1;
    nTempCenY = 1;
    TempCoef = 1;
    Tempdata = MALLOC(Float, 9);   //Sobel算子模板为3X3
    //水平向模板参数
    Tempdata[ 0 ] = -1;
    Tempdata[ 1 ] = -sqrt(2);
    Tempdata[ 2 ] = -1;
    Tempdata[ 3 ] = 0;
    Tempdata[ 4 ] = 0;
    Tempdata[ 5 ] = 0;
    Tempdata[ 6 ] = 1;
    Tempdata[ 7 ] = sqrt(2);
    Tempdata[ 8 ] = 1;

    if (!Template(lHeight, lWidth, lpImage1, lLineBytes, nTempWidth, nTempHeight,
        nTempCenX, nTempCenY, Tempdata, TempCoef)) {
      return FALSE;
    }

    //垂直方向模板参数
    Tempdata[ 0 ] = -1;
    Tempdata[ 1 ] = 0;
    Tempdata[ 2 ] = 1;
    Tempdata[ 3 ] = -sqrt(2);
    Tempdata[ 4 ] = 0;
    Tempdata[ 5 ] = sqrt(2);
    Tempdata[ 6 ] = -1;
    Tempdata[ 7 ] = 0;
    Tempdata[ 8 ] = 1;

    if (!Template(lHeight, lWidth, lpImage2, lLineBytes, nTempWidth, nTempHeight,
        nTempCenX, nTempCenY, Tempdata, TempCoef)) {
      return FALSE;
    }

    SAFEFREE(Tempdata);

    //求两幅缓存图像的最大值
    for (j = 0; j < lHeight; j++) {
      for (i = 0; i < lWidth; i++) {

        // 指向缓存图像1倒数第j行，第i个象素的指针
        lpDst1 = (LPBYTE) lpImage1 + lWidth * j + i;

        // 指向缓存图像2倒数第j行，第i个象素的指针
        lpDst2 = (LPBYTE) lpImage2 + lWidth * j + i;

        if (*lpDst2 > *lpDst1) {
          *lpDst1 = *lpDst2;
        }

      }
    }

    //复制经过模板运算后的图像到源图像
    memcpy(lpDIBBits, lpImage1, lWidth * lHeight);
    return TRUE;
  }

  if (mode == 2) {         //LOG算子
    nTempWidth = 5;
    nTempHeight = 5;
    nTempCenX = 2;
    nTempCenY = 2;
    TempCoef = 1;
    Tempdata = MALLOC(Float, 25);
    Tempdata[ 0 ] = -2;
    Tempdata[ 1 ] = -4;
    Tempdata[ 2 ] = -4;
    Tempdata[ 3 ] = -4;
    Tempdata[ 4 ] = -2;
    Tempdata[ 5 ] = -4;
    Tempdata[ 6 ] = 0;
    Tempdata[ 7 ] = 8;
    Tempdata[ 8 ] = 0;
    Tempdata[ 9 ] = -4;
    Tempdata[ 10 ] = -4;
    Tempdata[ 11 ] = 8;
    Tempdata[ 12 ] = 24;
    Tempdata[ 13 ] = 8;
    Tempdata[ 14 ] = -4;
    Tempdata[ 15 ] = -4;
    Tempdata[ 16 ] = 0;
    Tempdata[ 17 ] = 8;
    Tempdata[ 18 ] = 0;
    Tempdata[ 19 ] = -4;
    Tempdata[ 20 ] = -2;
    Tempdata[ 21 ] = -4;
    Tempdata[ 22 ] = -4;
    Tempdata[ 23 ] = -4;
    Tempdata[ 24 ] = -2;

    if (!Template(lHeight, lWidth, lpDIBBits, lLineBytes, nTempWidth, nTempHeight,
        nTempCenX, nTempCenY, Tempdata, TempCoef)) {
      return FALSE;
    }

    SAFEFREE(Tempdata);
    return TRUE;
  }

  return TRUE;
}

/*************************************************************************
* 函数名称：
*   Outline()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
* 返回值:
*   BOOL               - 运算成功返回TRUE，否则返回FALSE。
 *
* 说明:
* 该函数用于对图像进行轮廓提取运算。
* 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/

BOOL Outline(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // 指向源图像的指针
  LPBYTE lpSrc;
  // 指向要复制区域的指针
  LPBYTE lpDst;
  // 指向复制图像的指针
  LPBYTE lpImage;
  // 循环变量
  int i;
  int j;
  //存放处理像素八邻域的像素灰度
  BYTE n, e, s, w, ne, se, nw, sw;
  //像素值
  BYTE pixeltemp;
  // 暂时分配内存，以保存新图像
  lpImage = (LPBYTE) MALLOC(BYTE, lWidth * lHeight);

  // 判断是否内存分配失败
  if (lpImage == NULL) {
    // 分配内存失败
    return FALSE;
  }

  // 初始化新分配的内存，设定初始值为255
  lpDst = (LPBYTE) lpImage;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  //如果图像不是二值图像，转化为二值图像
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {

      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;

      //取得当前指针处的像素值，注意要转换为BYTE型
      pixeltemp = (BYTE) * lpSrc;

      //目标图像中灰度值大于120的设置为255，否则设置为0
      if (pixeltemp >= 120) {
        *lpSrc = (BYTE) 255;
      }
      else {
        *lpSrc = (BYTE) 0;
      }
    }
  }

  for (j = 1; j < lHeight - 1; j++) {
    for (i = 1; i < lWidth - 1; i++) {
      //为防止越界，宽度，高度上下左右各留出一行
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;

      // 指向目标图像倒数第j行，第i个象素的指针
      lpDst = (LPBYTE) lpImage + lWidth * j + i;

      //是黑点才做处理
      if (*(lpSrc) == 0) {
        //目标图像相应的像素也相应的为黑点
        *(lpDst) = 0;
        w = (BYTE) * (lpSrc - 1);      //左邻点
        e = (BYTE) * (lpSrc + 1);      //右邻点
        nw = (BYTE) * (lpSrc + lWidth - 1);     //左上邻点
        n = (BYTE) * (lpSrc + lWidth);     //上邻点
        ne = (BYTE) * (lpSrc + lWidth + 1);     //右上邻点
        sw = (BYTE) * (lpSrc - lWidth - 1);     //左下邻点
        s = (BYTE) * (lpSrc - lWidth);     //下邻点
        se = (BYTE) * (lpSrc - lWidth + 1);     //右下邻点

        //如果相邻的八个点都是黑点
        if (nw + n + ne + w + e + sw + s + se == 0) {
          //结果图中该黑点删除
          *(lpDst) = (BYTE) 255;
        }
      }
    }
  }

  // 复制轮廓提取后的图像
  memcpy(lpDIBBits, lpImage, lWidth * lHeight);
  // 释放内存
  SAFEFREE(lpImage);
  // 返回
  return TRUE;
}

/*************************************************************************
* 函数名称：
*   ContourTrace()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
* 返回值:
*   BOOL               - 运算成功返回TRUE，否则返回FALSE。
* 说明:
* 该函数用于对图像进行轮廓跟踪运算。
* 要求目标图像为只有0和255两个灰度值的灰度图像，否则自动转化。
 ************************************************************************/

BOOL ContourTrace(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{

  // 指向源图像的指针
  LPBYTE lpSrc;
  // 指向要复制区域的指针
  LPBYTE lpDst;
  // 指向复制图像的指针
  LPBYTE lpImage;
  // 循环变量
  int i;
  int j;
  //像素值
  BYTE pixeltemp;
  //是否找到起始点及回到起始点
  BOOL IsStartPoint;
  //是否扫描到一个边界点
  BOOL Pointfind;
  //起始边界点与当前边界点
  IPOINT StartPoint;
  IPOINT CurrentPoint;
  //八个方向和起始扫描方向
  int Direction[ 8 ][ 2 ] = {{ -1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, { -1, -1}, { -1, 0}};
  int BeginDirect;

  // 暂时分配内存，以保存新图像
  lpImage = (LPBYTE) MALLOC(BYTE, lWidth * lHeight);

  // 判断是否内存分配失败
  if (lpImage == NULL) {
    // 分配内存失败
    return FALSE;
  }

  // 初始化新分配的内存，设定初始值为255
  lpDst = (LPBYTE) lpImage;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  //如果图像不是二值图像，转化为二值图像
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {

      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;

      //取得当前指针处的像素值，注意要转换为BYTE型
      pixeltemp = (BYTE) * lpSrc;

      //目标图像中灰度值大于120的设置为255，否则设置为0
      if (pixeltemp >= 120) {
        *lpSrc = (BYTE) 255;
      }
      else {
        *lpSrc = (BYTE) 0;
      }
    }
  }

  //先找到图像最左下方的边界点，注意是左下方
  IsStartPoint = FALSE;

  for (j = 1; j < lHeight && !IsStartPoint; j++) {
    for (i = 1; i < lWidth && !IsStartPoint; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;

      //取得当前指针处的像素值
      pixeltemp = (BYTE) * lpSrc;

      //如果为黑点，则为起始点
      if (pixeltemp == 0) {
        IsStartPoint = TRUE;
        StartPoint.y = j;        //起始点的垂直位置
        StartPoint.x = i;        //起始点的水平位置

        // 指向目标图像倒数第j行，第i个象素的指针
        lpDst = (LPBYTE) lpImage + lWidth * j + i;
        *lpDst = (BYTE) 0;          //目标点设置为黑点
      }
    }
  }

  //由于起始点是在左下方，故起始扫描沿左上方向
  BeginDirect = 0;
  //跟踪边界
  IsStartPoint = FALSE;
  //从初始点开始扫描
  CurrentPoint.y = StartPoint.y;
  CurrentPoint.x = StartPoint.x;

  while (!IsStartPoint) {
    Pointfind = FALSE;

    while (!Pointfind) {
      //沿扫描方向查看下一个像素
      lpSrc = (LPBYTE) lpDIBBits + lWidth * (CurrentPoint.y + Direction[ BeginDirect ][ 1 ])
          + (CurrentPoint.x + Direction[ BeginDirect ][ 0 ]);
      //存贮该点的像素值
      pixeltemp = (BYTE) * lpSrc;

      //如果该点为黑点
      if (pixeltemp == 0) {
        //找到了点，设置为真
        Pointfind = TRUE;
        //记录当前点的位置
        CurrentPoint.y = CurrentPoint.y + Direction[ BeginDirect ][ 1 ];
        CurrentPoint.x = CurrentPoint.x + Direction[ BeginDirect ][ 0 ];

        //如果当前点就是起始点
        if (CurrentPoint.y == StartPoint.y && CurrentPoint.x == StartPoint.x) {
          //起始点变量设置为真，退出循环
          IsStartPoint = TRUE;
        }

        //记录当前点，设置为黑点
        lpDst = (LPBYTE) lpImage + lWidth * CurrentPoint.y + CurrentPoint.x;
        *lpDst = (BYTE) 0;
        //扫描的方向逆时针旋转两格
        BeginDirect--; //选择一格

        if (BeginDirect == -1) {   //如果方向为－1表明是方向7
          BeginDirect = 7;
        }

        BeginDirect--;

        if (BeginDirect == -1) {     //如果方向为－1表明是方向7
          BeginDirect = 7;
        }
      }
      //扫描方向的下一个点为白点，那么我们需要改变扫描方向，继续寻找
      else {
        //扫描方向顺时针旋转一格
        BeginDirect++;

        if (BeginDirect == 8) {
          BeginDirect = 0;    //如果方向为8表明是方向0
        }

      }

    }
  }

  // 复制轮廓跟踪后的图像
  memcpy(lpDIBBits, lpImage, lWidth * lHeight);
  // 释放内存
  SAFEFREE(lpImage);
  // 返回
  return TRUE;
}
//强调一点，本程序只是轮廓跟踪的简单演示，首先搜索图像的左下方，找到的第一个黑点
//就设置为起始点，以此点为基础进行边界扫描，因此只能跟踪一个物体轮廓，当有多个物体
//轮廓需要跟踪时，需要将已经跟踪过的物体去除，继续上述过程。



#define TEMPLATE_SMOOTH_BOX    1
#define TEMPLATE_SMOOTH_GAUSS  2
#define TEMPLATE_SHARPEN_LAPLACIAN 3
#define TEMPLATE_SOBEL_H 4
#define TEMPLATE_SOBEL_V 5
#define TEMPLATE_ISOSOBEL_H 6
#define TEMPLATE_ISOSOBEL_V 7

//template array
static const Float Template_Smooth_Box[ 9 ] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const Float Template_Smooth_Gauss[ 9 ] = {
  1.0f, 2.0f, 1.0f, 2.0f, 4.0f, 2.0f, 1.0f, 2.0f, 1.0f
};
static const Float Template_Sharpen_Laplacian[ 9 ] = {
  -1.0f, -1.0f, -1.0f, -1.0f, 9.0f, -1.0f, -1.0f, -1.0f, -1.0f
};
static const Float Template_HSobel[ 9 ] = {
  -1.0f, 0.0f, 1.0f, -2.0f, 0.0f, 2.0f, -1.0f, 0.0f, 1.0f
};
static const Float Template_VSobel[ 9 ] = {
  -1.0f, -2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 1.0f
};
static const Float Template_HIsoSobel[ 9 ] = {
  -1.0f, 0.0f, 1.0f, -1.4142f, 0.0f, 1.4142f, -1.0f, 0.0f, 1.0f
};
static const Float Template_VIsoSobel[ 9 ] = {
  -1.0f, -1.4142f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.4142f, 1.0f
};
static const Float Template_Log[ 25 ] = { //
  -2.0f, -4.0f, -4.0f, -4.0f, -2.0f,
  -4.0f, 0.0f, 8.0f, 0.0f, -4.0f,
  -4.0f, 8.0f, 24.0f, 8.0f, -4.0f,
  -4.0f, 0.0f, 8.0f, 0.0f, -4.0f,
  -2.0f, -4.0f, -4.0f, -4.0f, -2.0f
};
////////////////////////////////////////////////////////////////
BOOL TemplateOperation(LPBYTE lpImgData, int lWidth, int lHeight, int lLineBytes,
    LPBYTE lpTempImgData, int TemplateType)
{
  DWORD BufSize = lHeight * lLineBytes;
  LPBYTE lpPtr;
  LPBYTE lpTempPtr;
  int x, y;
  Float coef;
  Float CoefArray[ 9 ];
  Float TempNum;

  switch (TemplateType) {
  case TEMPLATE_SMOOTH_BOX:
    coef = (Float)(1.0 / 9.0);
    memcpy(CoefArray, Template_Smooth_Box, 9 * sizeof(Float));
    break;

  case TEMPLATE_SMOOTH_GAUSS:
    coef = (Float)(1.0 / 16.0);
    memcpy(CoefArray, Template_Smooth_Gauss, 9 * sizeof(Float));
    break;

  case TEMPLATE_SHARPEN_LAPLACIAN:
    coef = (Float) 1.0;
    memcpy(CoefArray, Template_Sharpen_Laplacian, 9 * sizeof(Float));
    break;

  case TEMPLATE_SOBEL_H:
    coef = (Float) 1.0;
    memcpy(CoefArray, Template_HSobel, 9 * sizeof(Float));
    break;

  case TEMPLATE_SOBEL_V:
    coef = (Float) 1.0;
    memcpy(CoefArray, Template_VSobel, 9 * sizeof(Float));
    break;

  case TEMPLATE_ISOSOBEL_H:
    coef = (Float) 1.0;
    memcpy(CoefArray, Template_HIsoSobel, 9 * sizeof(Float));
    break;

  case TEMPLATE_ISOSOBEL_V:
    coef = (Float) 1.0;
    memcpy(CoefArray, Template_VIsoSobel, 9 * sizeof(Float));
    break;
  }

  lpPtr = (LPBYTE) lpImgData;
  lpTempPtr = (LPBYTE) lpTempImgData;

  memcpy(lpTempPtr, lpPtr, BufSize);

  for (y = 1; y < lHeight - 1; y++) {
    for (x = 1; x < lWidth - 1; x++) {
      lpPtr = (LPBYTE) lpImgData + (BufSize - lLineBytes - y * lLineBytes) + x;
      lpTempPtr = (LPBYTE) lpTempImgData + (BufSize - lLineBytes - y * lLineBytes) + x;
      TempNum = (Float)((BYTE) * (lpPtr + lLineBytes - 1)) * CoefArray[ 0 ];
      TempNum += (Float)((BYTE) * (lpPtr + lLineBytes)) * CoefArray[ 1 ];
      TempNum += (Float)((BYTE) * (lpPtr + lLineBytes + 1)) * CoefArray[ 2 ];
      TempNum += (Float)((BYTE) * (lpPtr - 1)) * CoefArray[ 3 ];
      TempNum += (Float)((BYTE) * lpPtr) * CoefArray[ 4 ];
      TempNum += (Float)((BYTE) * (lpPtr + 1)) * CoefArray[ 5 ];
      TempNum += (Float)((BYTE) * (lpPtr - lLineBytes - 1)) * CoefArray[ 6 ];
      TempNum += (Float)((BYTE) * (lpPtr - lLineBytes)) * CoefArray[ 7 ];
      TempNum += (Float)((BYTE) * (lpPtr - lLineBytes + 1)) * CoefArray[ 8 ];
      TempNum *= coef;

      if (TempNum > 255.0) {
        * lpTempPtr = (BYTE) 255;
      }
      else if (TempNum < 0.0) {
        * lpTempPtr = (BYTE) fabs(TempNum);
      }
      else {
        *lpTempPtr = (BYTE) TempNum;
      }
    }
  }

  return TRUE;
}

////////////////////////////////////////////////////////////////
BOOL Outline1(LPBYTE lpImgData, int lWidth, int lHeight, int lLineBytes, LPBYTE lpTempImgData)
{
  DWORD BufSize = lHeight * lLineBytes;
  LPBYTE lpPtr;
  LPBYTE lpTempPtr;
  int x, y, num;
  int nw, n, ne, w, e, sw, s, se;

  memcpy(lpTempImgData, lpImgData, BufSize);

  for (y = 1; y < lHeight - 1; y++) {
    lpPtr = (LPBYTE) lpImgData + (BufSize - lLineBytes - y * lLineBytes);
    lpTempPtr = (LPBYTE) lpTempImgData + (BufSize - lLineBytes - y * lLineBytes);

    for (x = 1; x < lWidth - 1; x++) {
      if (*(lpPtr + x) == 0) {
        nw = (BYTE) * (lpPtr + x + lLineBytes - 1);
        n = (BYTE) * (lpPtr + x + lLineBytes);
        ne = (BYTE) * (lpPtr + x + lLineBytes + 1);
        w = (BYTE) * (lpPtr + x - 1);
        e = (BYTE) * (lpPtr + x + 1);
        sw = (BYTE) * (lpPtr + x - lLineBytes - 1);
        s = (BYTE) * (lpPtr + x - lLineBytes);
        se = (BYTE) * (lpPtr + x - lLineBytes + 1);
        num = nw + n + ne + w + e + sw + s + se;

        if (num == 0) {
          * (lpTempPtr + x) = (BYTE) 255;
        }
      }
    }
  }

  return TRUE;
}

////////////////////////////////////////////////////////////////
BOOL Hough1(LPBYTE lpImgData, int lWidth, int lHeight, int lLineBytes)
{
  typedef struct {
    int topx;
    int topy;
    int botx;
    int boty;
  }
  MYLINE;
  DWORD BufSize = lHeight * lLineBytes;
  int x, y, i, maxd;
  int k, Dist, Alpha, *lpDistAlpha;
  LPBYTE lpPtr;
  MYLINE* lpMyLine, *TempLine, MaxdLine;
  //static LOGPEN rlp={PS_SOLID, 1, 1, RGB(255, 0, 0)};

  Dist = (int)(sqrt((Float) lWidth * lWidth + (Float) lHeight * lHeight) + 0.5);
  Alpha = 180 / 2; //0 degree to 178 degree , step is 2 degrees

  if ((lpDistAlpha = MALLOC(int, Dist * Alpha)) == NULL) {
    print_err("Error alloc memory!", "Error Message");
    return FALSE;
  }

  if ((lpMyLine = MALLOC(MYLINE, Dist * Alpha)) == NULL) {
    SAFEFREE(lpDistAlpha);
    return FALSE;
  }

  for (i = 0; i < (int) Dist * Alpha; i++) {
    TempLine = (MYLINE*)(lpMyLine + i);
    (*TempLine).boty = 32767;
  }

  for (y = 0; y < lHeight; y++) {
    lpPtr = (LPBYTE) lpImgData + (BufSize - lLineBytes - y * lLineBytes);

    for (x = 0; x < lWidth; x++) {
      if (*(lpPtr++) == 0) {
        for (k = 0; k < 180; k += 2) {
          i = (int) fabs((x * cos(k * PI / 180.0) + y * sin(k * PI / 180.0)));
          *(lpDistAlpha + i * Alpha + k / 2) = *(lpDistAlpha + i * Alpha + k / 2) + 1;
          TempLine = (MYLINE*)(lpMyLine + i * Alpha + k / 2);

          if (y > (*TempLine).topy) {
            (*TempLine).topx = x;
            (*TempLine).topy = y;
          }

          if (y < (*TempLine).boty) {
            (*TempLine).botx = x;
            (*TempLine).boty = y;
          }
        }
      }
    }
  }

  maxd = 0;

  for (i = 0; i < (int) Dist * Alpha; i++) {
    TempLine = (MYLINE*)(lpMyLine + i);
    k = *(lpDistAlpha + i);

    if (k > maxd) {
      maxd = k;
      MaxdLine.topx = (*TempLine).topx;
      MaxdLine.topy = (*TempLine).topy;
      MaxdLine.botx = (*TempLine).botx;
      MaxdLine.boty = (*TempLine).boty;
    }
  }

  SAFEFREE(lpDistAlpha);
  SAFEFREE(lpMyLine);
  return TRUE;
}
////////////////////////////////////////////////////////////////
BOOL LapOfGauss(LPBYTE lpImgData, int lWidth, int lHeight, int lLineBytes, LPBYTE lpTempImgData)
{
  LPBYTE lpPtr;
  LPBYTE lpTempPtr;
  int x, y, BufSize = lHeight * lLineBytes;
  Float coef;
  Float TempNum;

  coef = (Float)(1.0);

  lpPtr = (LPBYTE) lpImgData;
  lpTempPtr = (LPBYTE) lpTempImgData;

  memcpy(lpTempPtr, lpPtr, BufSize);

  for (y = 2; y < lHeight - 2; y++) {
    for (x = 2; x < lWidth - 2; x++) {
      lpPtr = (LPBYTE) lpImgData + (BufSize - lLineBytes - y * lLineBytes) + x;
      lpTempPtr = (LPBYTE) lpTempImgData + (BufSize - lLineBytes - y * lLineBytes) + x;
      TempNum = (Float)((BYTE) * (lpPtr + 2 * lLineBytes - 2)) * Template_Log[ 0 ];
      TempNum += (Float)((BYTE) * (lpPtr + 2 * lLineBytes - 1)) * Template_Log[ 1 ];
      TempNum += (Float)((BYTE) * (lpPtr + 2 * lLineBytes)) * Template_Log[ 2 ];
      TempNum += (Float)((BYTE) * (lpPtr + 2 * lLineBytes + 1)) * Template_Log[ 3 ];
      TempNum += (Float)((BYTE) * (lpPtr + 2 * lLineBytes + 2)) * Template_Log[ 4 ];

      TempNum += (Float)((BYTE) * (lpPtr + lLineBytes - 2)) * Template_Log[ 5 ];
      TempNum += (Float)((BYTE) * (lpPtr + lLineBytes - 1)) * Template_Log[ 6 ];
      TempNum += (Float)((BYTE) * (lpPtr + lLineBytes)) * Template_Log[ 7 ];
      TempNum += (Float)((BYTE) * (lpPtr + lLineBytes + 1)) * Template_Log[ 8 ];
      TempNum += (Float)((BYTE) * (lpPtr + lLineBytes + 2)) * Template_Log[ 9 ];

      TempNum += (Float)((BYTE) * (lpPtr - 2)) * Template_Log[ 10 ];
      TempNum += (Float)((BYTE) * (lpPtr - 1)) * Template_Log[ 11 ];
      TempNum += (Float)((BYTE) * (lpPtr)) * Template_Log[ 12 ];
      TempNum += (Float)((BYTE) * (lpPtr + 1)) * Template_Log[ 13 ];
      TempNum += (Float)((BYTE) * (lpPtr + 2)) * Template_Log[ 14 ];

      TempNum += (Float)((BYTE) * (lpPtr - lLineBytes - 2)) * Template_Log[ 15 ];
      TempNum += (Float)((BYTE) * (lpPtr - lLineBytes - 1)) * Template_Log[ 16 ];
      TempNum += (Float)((BYTE) * (lpPtr - lLineBytes)) * Template_Log[ 17 ];
      TempNum += (Float)((BYTE) * (lpPtr - lLineBytes + 1)) * Template_Log[ 18 ];
      TempNum += (Float)((BYTE) * (lpPtr - lLineBytes + 2)) * Template_Log[ 19 ];

      TempNum += (Float)((BYTE) * (lpPtr - 2 * lLineBytes - 2)) * Template_Log[ 20 ];
      TempNum += (Float)((BYTE) * (lpPtr - 2 * lLineBytes - 1)) * Template_Log[ 21 ];
      TempNum += (Float)((BYTE) * (lpPtr - 2 * lLineBytes)) * Template_Log[ 22 ];
      TempNum += (Float)((BYTE) * (lpPtr - 2 * lLineBytes + 1)) * Template_Log[ 23 ];
      TempNum += (Float)((BYTE) * (lpPtr - 2 * lLineBytes + 2)) * Template_Log[ 24 ];

      TempNum *= coef;

      if (TempNum > 255.0) {
        * lpTempPtr = (BYTE) 255;
      }
      else if (TempNum < 0.0) {
        * lpTempPtr = (BYTE) fabs(TempNum);
      }
      else {
        *lpTempPtr = (BYTE) TempNum;
      }
    }
  }

  return TRUE;
}

////////////////////////////////////////////////////////////////
BOOL IsContourP(int lLineBytes, LPBYTE lpPtr)
{
  int num, n, w, e, s;

  n = (BYTE) * (lpPtr + lLineBytes);
  w = (BYTE) * (lpPtr - 1);
  e = (BYTE) * (lpPtr + 1);
  s = (BYTE) * (lpPtr - lLineBytes);
  num = n + w + e + s;

  if (num == 0) {
    return FALSE;
  }

  return TRUE;
}

////////////////////////////////////////////////////////////////
BOOL ContourTemp(LPBYTE lpImgData, int lWidth, int lHeight, int lLineBytes, LPBYTE lpTempImgData)
{
  LPBYTE lpPtr;
  LPBYTE lpTempPtr;
  int x, y, BufSize = lHeight * lLineBytes;
  IPOINT StartP, CurP;
  BOOL found;
  int i;
  int direct[ 8 ][ 2 ] = {{1, 0}, {1, -1}, {0, -1}, { -1, -1}, { -1, 0}, { -1, 1}, {0, 1}, {1, 1}};

  memset(lpTempImgData, (BYTE) 255, BufSize);
  //memcpy(lpTempImgData, lpImgData, OffBits);

  found = FALSE;

  for (y = 0; y < lHeight && !found; y++) {
    lpPtr = (LPBYTE) lpImgData + (BufSize - lLineBytes - y * lLineBytes);

    for (x = 0; x < lWidth && !found; x++)
      if (*(lpPtr++) == 0) {
        found = TRUE;
      }
  }

  if (found) {
    StartP.x = x - 1;
    StartP.y = y - 1;
    lpTempPtr = (LPBYTE) lpTempImgData + (BufSize - lLineBytes - StartP.y * lLineBytes) + StartP.x;
    *lpTempPtr = (BYTE) 0;
    CurP.x = StartP.x + 1;
    CurP.y = StartP.y;
    lpPtr = (LPBYTE) lpImgData + (BufSize - lLineBytes - CurP.y * lLineBytes) + CurP.x;

    if (*lpPtr != 0) {
      CurP.x = StartP.x + 1;
      CurP.y = StartP.y + 1;
      lpPtr = (LPBYTE) lpImgData + (BufSize - lLineBytes - CurP.y * lLineBytes) + CurP.x;

      if (*lpPtr != 0) {
        CurP.x = StartP.x;
        CurP.y = StartP.y + 1;
      }
      else {
        CurP.x = StartP.x - 1;
        CurP.y = StartP.y + 1;
      }
    }

    while (!((CurP.x == StartP.x) && (CurP.y == StartP.y))) {
      lpTempPtr = (LPBYTE) lpTempImgData + (BufSize - lLineBytes - CurP.y * lLineBytes) + CurP.x;
      *lpTempPtr = (BYTE) 0;

      for (i = 0; i < 8; i++) {
        x = CurP.x + direct[ i ][ 0 ];
        y = CurP.y + direct[ i ][ 1 ];
        lpTempPtr = (LPBYTE) lpTempImgData + (BufSize - lLineBytes - y * lLineBytes) + x;
        lpPtr = (LPBYTE) lpImgData + (BufSize - lLineBytes - y * lLineBytes) + x;

        if (((*lpPtr == 0) && (*lpTempPtr != 0)) || ((x == StartP.x) && (y == StartP.y)))
          if (IsContourP(lLineBytes, lpPtr)) {
            CurP.x = x;
            CurP.y = y;
            break;
          }
      }
    }
  }

  return TRUE;
}

//owner defined stack
typedef struct MYSTACK {
  int ElementsNum;
  int ptr;
  IPOINT* lpMyStack;
}
MYSTACK;

////////////////////////////////////////////////////////////////
BOOL InitStack(MYSTACK SeedFillStack, int StackLen)
{
  SeedFillStack.ElementsNum = StackLen;

  if ((SeedFillStack.lpMyStack = MALLOC(IPOINT, SeedFillStack.ElementsNum)) == NULL) {
    print_err("Error alloc memory!", "ErrorMessage");
    return FALSE;
  }

  memset(SeedFillStack.lpMyStack, 0, SeedFillStack.ElementsNum * sizeof(IPOINT));
  SeedFillStack.ptr = 0;
  return TRUE;
}
////////////////////////////////////////////////////////////////
void DeInitStack(MYSTACK SeedFillStack)
{
  SAFEFREE(SeedFillStack.lpMyStack);
  SeedFillStack.ElementsNum = 0;
  SeedFillStack.ptr = 0;
}
////////////////////////////////////////////////////////////////
BOOL MyPush(MYSTACK SeedFillStack, IPOINT p)
{
  IPOINT* TempPtr;

  if (SeedFillStack.ptr >= SeedFillStack.ElementsNum) {
    return FALSE;
  }

  TempPtr = (IPOINT*)(SeedFillStack.lpMyStack + SeedFillStack.ptr++);
  (*TempPtr).x = p.x;
  (*TempPtr).y = p.y;
  return TRUE;
}
////////////////////////////////////////////////////////////////
IPOINT MyPop(MYSTACK SeedFillStack)
{
  IPOINT InvalidP;
  InvalidP.x = -1;
  InvalidP.y = -1;

  if (SeedFillStack.ptr <= 0) {
    return InvalidP;
  }

  SeedFillStack.ptr--;
  return *(SeedFillStack.lpMyStack + SeedFillStack.ptr);
}
////////////////////////////////////////////////////////////////
#define IsStackEmpty(SeedFillStack) (((SeedFillStack).ptr==0) ? TRUE : FALSE)

////////////////////////////////////////////////////////////////
BOOL SeedFill(LPBYTE lpImgData, int lWidth, int lHeight, int lLineBytes, LPBYTE lpTempImgData, IPOINT SeedPoint)
{
  DWORD BufSize = lHeight * lLineBytes;
  IPOINT CurP, NeighborP;
  LPBYTE lpTempPtr, lpTempPtr1;
  MYSTACK SeedFillStack = {0};

  //copy image data
  memcpy(lpTempImgData, lpImgData, BufSize);

  if (!InitStack(SeedFillStack, (int) lHeight * lWidth)) {
    return FALSE;
  }

  lpTempPtr = (LPBYTE) lpTempImgData + (BufSize - lLineBytes - SeedPoint.y * lLineBytes) + SeedPoint.x;

  if (*lpTempPtr == 0) {
    DeInitStack(SeedFillStack);
    return FALSE;
  }

  MyPush(SeedFillStack, SeedPoint);

  while (!IsStackEmpty(SeedFillStack)) {
    CurP = MyPop(SeedFillStack);
    lpTempPtr = (LPBYTE) lpTempImgData + (BufSize - lLineBytes - CurP.y * lLineBytes) + CurP.x;
    *lpTempPtr = (BYTE) 0;

    //left neighbour
    if (CurP.x > 0) {
      NeighborP.x = CurP.x - 1;
      NeighborP.y = CurP.y;
      lpTempPtr1 = lpTempPtr - 1;

      if (*lpTempPtr1 != 0) {
        MyPush(SeedFillStack, NeighborP);
      }
    }

    //up neighbour
    if (CurP.y > 0) {
      NeighborP.x = CurP.x;
      NeighborP.y = CurP.y - 1;
      lpTempPtr1 = lpTempPtr + lLineBytes;

      if (*lpTempPtr1 != 0) {
        MyPush(SeedFillStack, NeighborP);
      }
    }

    //right neighbour
    if (CurP.x < lWidth - 1) {
      NeighborP.x = CurP.x + 1;
      NeighborP.y = CurP.y;
      lpTempPtr1 = lpTempPtr + 1;

      if (*lpTempPtr1 != 0) {
        MyPush(SeedFillStack, NeighborP);
      }
    }

    //down neighbour
    if (CurP.y < lHeight - 1) {
      NeighborP.x = CurP.x;
      NeighborP.y = CurP.y + 1;
      lpTempPtr1 = lpTempPtr - lLineBytes;

      if (*lpTempPtr1 != 0) {
        MyPush(SeedFillStack, NeighborP);
      }
    }
  }

  DeInitStack(SeedFillStack);

  return TRUE;
}
