// ************************************************************************
//
// 图像分析与检测API函数库：
//
// Threshold() - 图像阈值分割运算
// AddMinus()  - 图像加减运算
// Hproject() - 图像水平投影
// Vproject() - 图像垂直投影
// Template() - 图像模板匹配运算
//
// ************************************************************************

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "macro.h"
#include "imglib.h"

//////////////////////////////////////////////////////////////////////
// 图像模板变换函数
//////////////////////////////////////////////////////////////////////

/*************************************************************************
* 函数名称：
*   Template()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth          - 源图像宽度（象素数）
*   int lHeight         - 源图像高度（象素数）
*   int  iTempH         - 模板的高度
*   int  iTempW         - 模板的宽度
*   int  iTempMX        - 模板的中心元素X坐标 ( < iTempW - 1)
*   int  iTempMY        - 模板的中心元素Y坐标 ( < iTempH - 1)
*   Float* fpArray      - 指向模板数组的指针
*   Float fCoef         - 模板系数
* 返回值:
*   BOOL                - 成功返回TRUE，否则返回FALSE。
* 说明:
*   该函数用指定的模板（任意大小）来对图像进行操作，参数iTempH指定模板
* 的高度，参数iTempW指定模板的宽度，参数iTempMX和iTempMY指定模板的中心
* 元素坐标，参数fpArray指定模板元素，fCoef指定系数。
 ************************************************************************/
BOOL Template(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    int iTempH, int iTempW, int iTempMX, int iTempMY,
    Float* fpArray, Float fCoef)
{
  LPBYTE lpNewDIBBits;  // 指向复制图像的指针
  LPBYTE lpSrc;    // 指向源图像的指针
  LPBYTE lpDst;    // 指向要复制区域的指针
  int i, j, k, l;  // 循环变量
  Float fResult;   // 计算结果

  // 暂时分配内存，以保存新图像
  lpNewDIBBits = MALLOC(BYTE, lLineBytes * lHeight);

  // 初始化图像为原始图像
  memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);

  for (i = iTempMY; i < lHeight - iTempH + iTempMY + 1; i++) {   // 行(除去边缘几行)
    for (j = iTempMX; j < lWidth - iTempW + iTempMX + 1; j++) {   // 列(除去边缘几列)
      // 指向新DIB第i行，第j个象素的指针
      lpDst = (LPBYTE) lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

      fResult = 0;

      for (k = 0; k < iTempH; k++) {
        for (l = 0; l < iTempW; l++) {
          // 指向DIB第i - iTempMY + k行，第j - iTempMX + l个象素的指针
          lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1
              - i + iTempMY - k) + j - iTempMX + l;

          // 保存象素值
          fResult += (* lpSrc) * fpArray[ k * iTempW + l ];
        }
      }

      fResult *= fCoef;     // 乘上系数
      fResult = (Float) fabs(fResult);     // 取绝对值

      if (fResult > 255) {
        * lpDst = 255;
      }
      else {
        * lpDst = (BYTE)(fResult + 0.5);
      }
    }
  }

  // 复制变换后的图像
  memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

  SAFEFREE(lpNewDIBBits);
  return TRUE;
}

/*************************************************************************
* 函数名称：
*   GetMedianNum()
* 参数:
*   LPBYTE bpArray - 指向要获取中值的数组指针
*   int  iFilterLen   - 数组长度
* 返回值:
*   BYTE      - 返回指定数组的中值。
* 说明:
*   该函数用冒泡法对一维数组进行排序，并返回数组元素的中值。
 ************************************************************************/
BYTE GetMedianNum(LPBYTE bArray, int iFilterLen)
{
  int i, j;   // 循环变量
  BYTE bTemp;

  // 用冒泡法对数组进行排序
  for (j = 0; j < iFilterLen - 1; j ++) {
    for (i = 0; i < iFilterLen - j - 1; i ++) {
      if (bArray[ i ] > bArray[ i + 1 ]) {
        // 互换
        bTemp = bArray[ i ];
        bArray[ i ] = bArray[ i + 1 ];
        bArray[ i + 1 ] = bTemp;
      }
    }
  }

  // 计算中值
  if ((iFilterLen & 1) > 0) {
    // 数组有奇数个元素，返回中间一个元素
    bTemp = bArray[(iFilterLen + 1) / 2 ];
  }
  else {
    // 数组有偶数个元素，返回中间两个元素平均值
    bTemp = (bArray[ iFilterLen / 2 ] + bArray[ iFilterLen / 2 + 1 ]) / 2;
  }

  return bTemp;
}

/*************************************************************************
* 函数名称：
*   MedianFilter()
* 参数:
*   LPBYTE lpDIBBits  - 指向源DIB图像指针
*   int lWidth   - 源图像宽度（象素数）
*   int lHeight   - 源图像高度（象素数）
*   int  iFilterH   - 滤波器的高度
*   int  iFilterW   - 滤波器的宽度
*   int  iFilterMX  - 滤波器的中心元素X坐标
*   int  iFilterMY  - 滤波器的中心元素Y坐标
* 返回值:
*   BOOL     - 成功返回TRUE，否则返回FALSE。
* 说明:
*   该函数对DIB图像进行中值滤波。
 ************************************************************************/
BOOL MedianFilter(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    int iFilterH, int iFilterW, int iFilterMX, int iFilterMY)
{
  LPBYTE lpSrc;   // 指向源图像的指针
  LPBYTE lpDst;   // 指向要复制区域的指针
  LPBYTE lpNewDIBBits; // 指向复制图像的指针
  LPBYTE aValue;  // 指向滤波器数组的指针
  int i, j, k, l;  // 循环变量

  // 暂时分配内存，以保存新图像
  lpNewDIBBits = MALLOC(BYTE, lLineBytes * lHeight);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  // 初始化图像为原始图像
  memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);

  // 暂时分配内存，以保存滤波器数组
  aValue = MALLOC(BYTE, iFilterH * iFilterW);

  if (aValue == NULL) {
    SAFEFREE(lpNewDIBBits);
    return FALSE;
  }

  // 开始中值滤波
  // 行(除去边缘几行)
  for (i = iFilterMY; i < lHeight - iFilterH + iFilterMY + 1; i++) {
    // 列(除去边缘几列)
    for (j = iFilterMX; j < lWidth - iFilterW + iFilterMX + 1; j++) {
      // 指向新DIB第i行，第j个象素的指针
      lpDst = (LPBYTE) lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

      // 读取滤波器数组
      for (k = 0; k < iFilterH; k++) {
        for (l = 0; l < iFilterW; l++) {
          // 指向DIB第i - iFilterMY + k行，第j - iFilterMX + l个象素的指针
          lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1
              - i + iFilterMY - k) + j - iFilterMX + l;

          // 保存象素值
          aValue[ k * iFilterW + l ] = *lpSrc;
        }
      }

      // 获取中值
      * lpDst = GetMedianNum(aValue, iFilterH * iFilterW);
    }
  }

  // 复制变换后的图像
  memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

  SAFEFREE(lpNewDIBBits);
  SAFEFREE(aValue);
  return TRUE;
}

/*************************************************************************
* 函数名称：
*   MedianValue()
* 参数:
*   LPBYTE FiltValue    - 指向要获取中值的数组指针
*   int  iFilterLen             - 数组长度
* 返回值:
*   BYTE                - 返回指定数组的中值。
* 说明:
*   该函数用冒泡法对一维数组进行排序，并返回数组元素的中值。
************************************************************************/
BYTE MedianValue(LPBYTE FiltValue, int mFilterLen)
{
  int i;                                        // 循环变量
  int j;                                        // 循环变量
  BYTE Temp;                                    // 中间变量

  // 用冒泡法对数组进行排序
  for (j = 0; j < mFilterLen - 1; j ++) {
    for (i = 0; i < mFilterLen - j - 1; i ++) {
      if (FiltValue[ i ] > FiltValue[ i + 1 ]) {                     // 前面的值大于后面的
        Temp = FiltValue[ i ];
        FiltValue[ i ] = FiltValue[ i + 1 ];                // 二者的值互换
        FiltValue[ i + 1 ] = Temp;
      }
    }
  }

  if ((mFilterLen & 1) > 0) {                                   // 计算中值
    //数组有奇数个元素，返回中间一个元素
    Temp = FiltValue[(mFilterLen + 1) / 2 ];
  }
  else {
    //数组有偶数个元素，返回中间两个元素平均值
    Temp = (FiltValue[ mFilterLen / 2 ] + FiltValue[ mFilterLen / 2 + 1 ]) / 2;
  }

  return Temp;                                        // 返回中值
}


/*************************************************************************
 *
* 函数名称：
*   Project()
 *
* 参数:
*   LPBYTE lpDIBBits        - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
*     int  direction        - 投影方向
* 返回值:
*   BOOL               - 运算成功返回TRUE，否则返回FALSE。
* 说明:
* 该函数用于对两幅图像进行水平、垂直投影运算。
* 要求目标图像为只有0和255两个灰度值的灰度图像，否则强行转换。
 ************************************************************************/

BOOL Project(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int direction)
{
  // 临时存放图像数据的指针
  LPBYTE lpImage;
  // 指向源图像的指针
  LPBYTE lpSrc;
  // 指向要复制区域的指针
  LPBYTE lpDst;
  //循环变量
  int i;
  int j;
  int k;
  //图像中每行内的黑点个数
  int BlackNumber;
  //像素值
  BYTE pixeltemp;
  // 分配内存
  lpImage = (LPBYTE) MALLOC(BYTE, lWidth * lHeight);

  // 判断是否内存分配失败
  if (lpImage == NULL) {
    // 分配内存失败
    return FALSE;
  }

  // 将目标图像所有像素初始化为白点
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

  if (direction == 0) {   //水平方向投影
    for (j = 0; j < lHeight; j++) {
      BlackNumber = 0;    //黑点数目置0

      for (i = 0; i < lWidth; i++) {
        // 指向源图像倒数第j行，第i个象素的指针
        lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
        //取得当前指针处的像素值，注意要转换为BYTE型
        pixeltemp = (BYTE) * lpSrc;

        if (pixeltemp == 0) { //如果发现黑点
          BlackNumber++; //黑点数目加1
        }
      }

      for (k = 0; k < BlackNumber; k++) {
        // 指向目标图像倒数第j行，第i个象素的指针
        lpDst = (LPBYTE) lpImage + lWidth * j + k;
        *lpDst = (BYTE) 0;   //置为黑点
      }
    }
  }
  else if (direction == 1) {   //垂直方向投影
    for (i = 0; i < lWidth; i++) {
      BlackNumber = 0;    //黑点数目置0

      for (j = 0; j < lHeight; j++) {
        // 指向源图像倒数第j行，第i个象素的指针
        lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
        //取得当前指针处的像素值，注意要转换为BYTE型
        pixeltemp = (BYTE) * lpSrc;

        if (pixeltemp == 0) { //如果发现黑点
          BlackNumber++; //黑点数目加1
        }
      }

      for (k = 0; k < BlackNumber; k++) {
        // 指向目标图像倒数第j行，第i个象素的指针
        lpDst = (LPBYTE) lpImage + lWidth * k + i;
        *lpDst = (BYTE) 0;   //置为黑点
      }
    }
  }

  // 复制区域生长后的图像
  memcpy(lpDIBBits, lpImage, lWidth * lHeight);
  // 释放内存
  SAFEFREE(lpImage);
  // 返回
  return TRUE;
}

/*************************************************************************
* 函数名称：
*   Subtract()
* 参数:
*   LPBYTE lpDIBBits1   - 指向源DIB图像指针
*   LPBYTE lpDIBBits2   - 指向背景DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
 *
* 返回值:
*   BOOL               - 运算成功返回TRUE，否则返回FALSE。
* 说明:
* 该函数用于对两幅图像进行差影运算。
* 要求目标图像为255个灰度值的灰度图像。
 ************************************************************************/
BOOL Subtract(LPBYTE lpDIBBits1, LPBYTE lpDIBBits2, int lWidth, int lHeight)
{
  //循环变量
  int i, j;
  // 临时存放图像数据的指针
  LPBYTE lpImage;
  // 指向源图像的指针
  LPBYTE lpSrc1;
  // 指向背景图像的指针
  LPBYTE lpSrc2;
  // 指向要复制区域的指针
  LPBYTE lpDst;
  //像素值
  BYTE pixeltemp1;
  BYTE pixeltemp2;
  // 分配内存
  lpImage = (LPBYTE) MALLOC(BYTE, lWidth * lHeight);

  // 判断是否内存分配失败
  if (lpImage == NULL) {
    // 分配内存失败
    return FALSE;
  }

  // 将目标图像所有像素初始化为白点
  lpDst = (LPBYTE) lpImage;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc1 = (LPBYTE) lpDIBBits1 + lWidth * j + i;
      // 指向背景图像倒数第j行，第i个象素的指针
      lpSrc2 = (LPBYTE) lpDIBBits2 + lWidth * j + i;
      // 指向缓冲图像倒数第j行，第i个象素的指针
      lpDst = (LPBYTE) lpImage + lWidth * j + i;
      //取得源图像当前指针处的像素值，注意要转换为BYTE型
      pixeltemp1 = (BYTE) * lpSrc1;
      //取得背景图像当前指针处的像素值，注意要转换为BYTE型
      pixeltemp2 = (BYTE) * lpSrc2;

      //进行差影运算
      if (pixeltemp1 - pixeltemp2 < 0) {
        *lpDst = 0;
      }
      else {
        *lpDst = pixeltemp1 - pixeltemp2;
      }
    }
  }

  // 复制区域生长后的图像
  memcpy(lpDIBBits1, lpImage, lWidth * lHeight);
  // 释放内存
  SAFEFREE(lpImage);
  // 返回
  return TRUE;
}


/*************************************************************************
* 函数名称：
*   TemplateMatch()
* 参数:
*   LPBYTE lpDIBBits1    - 指向源DIB图像指针
*   LPBYTE lpDIBBits2    - 指向模板DIB图像指针
*   int lWidth    - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight   - 源图像高度（象素数）
*   int TemplateWidth       - 模板图像宽度（象素数，必须是4的倍数）
*   int TemplateHeight      - 模板图像高度（象素数）
* 返回值:
*   BOOL               - 运算成功返回TRUE，否则返回FALSE。
* 说明:
* 该函数用于对图像进行模板匹配运算。
* 要求目标图像为255个灰度值的灰度图像。
 ************************************************************************/
BOOL TemplateMatch(int lHeight, int lWidth, LPBYTE lpDIBBits1,
    int TemplateHeight, int TemplateWidth, LPBYTE lpDIBBits2)
{
  // 临时存放图像数据的指针
  LPBYTE lpImage;
  // 指向源图像的指针
  LPBYTE lpSrc1;
  // 指向模板图像的指针
  LPBYTE lpSrc2;
  // 指向要复制区域的指针
  LPBYTE lpDst;
  //像素值
  BYTE pixeltemp1; //源图像像素
  BYTE pixeltemp2; //模板图像像素
  //循环变量
  int i, j, m, n;
  int cofT; //模板像素自相关值
  int cofS; //源像素自相关值
  int cofTS; //模板、源图像像素灰度互相关值
  Float R; //测度R
  Float Rmax; //最大测度
  IPOINT matchpoint; //匹配点
  // 分配内存
  lpImage = (LPBYTE) MALLOC(BYTE, lWidth * lHeight);

  // 判断是否内存分配失败
  if (lpImage == NULL) {
    // 分配内存失败
    return FALSE;
  }

  // 将目标图像所有像素初始化为灰白点
  lpDst = (LPBYTE) lpImage;
  memset(lpDst, (BYTE) 205, lWidth * lHeight);
  //计算cofT
  cofT = 0;

  for (j = 0; j < TemplateHeight; j++) {
    for (i = 0; i < TemplateWidth; i++) {
      // 指向模板图像倒数第j行，第i个象素的指针
      lpSrc2 = (LPBYTE) lpDIBBits2 + lWidth * j + i;
      //取得模板图像当前指针处的像素值，注意要转换为BYTE型
      pixeltemp2 = (BYTE) * lpSrc2;
      //进行自相关运算
      cofT = cofT + pixeltemp2 * pixeltemp2;
    }
  }

  //找到图像中最大相似性的出现位置
  Rmax = 0.0;

  for (j = 0; j < lHeight - TemplateHeight + 1; j++) {
    for (i = 0; i < lWidth - TemplateWidth + 1; i++) {
      cofS = 0; //初始化为0
      cofTS = 0; //初始化为0

      for (n = 0; n < TemplateHeight; n++) {
        for (m = 0; m < TemplateWidth; m++) {
          // 指向源图像倒数第j+n行，第i+m个象素的指针
          lpSrc1 = (LPBYTE) lpDIBBits1 + lWidth * (j + n) + (i + m);
          // 指向模板图像倒数第n行，第m个象素的指针
          lpSrc2 = (LPBYTE) lpDIBBits2 + TemplateWidth * n + m;
          //获取
          pixeltemp1 = (BYTE) * lpSrc1;
          pixeltemp2 = (BYTE) * lpSrc2;

          cofS = (int)(cofS + (Float) pixeltemp1 * pixeltemp1);
          cofTS = (int)(cofTS + (Float) pixeltemp1 * pixeltemp2);
        }
      }

      //计算相似性
      R = cofTS / (sqrt(cofS) * sqrt(cofT));

      //与最大相似性比较
      if (R > Rmax) {
        Rmax = R;
        matchpoint.x = i;
        matchpoint.y = j;
      }
    }
  }


  //将源图像中与模板图像匹配的像素区域复制到目标图像
  for (n = 0; n < TemplateHeight; n++) {
    for (m = 0; m < TemplateWidth; m++) {
      lpSrc1 = (LPBYTE) lpDIBBits1 + lWidth * (n + matchpoint.y) + (m + matchpoint.x);
      lpDst = (LPBYTE) lpImage + lWidth * (n + matchpoint.y) + (m + matchpoint.x);
      *lpDst = *lpSrc1;
    }
  }

  // 复制图像
  memcpy(lpDIBBits1, lpImage, lWidth * lHeight);
  // 释放内存
  SAFEFREE(lpImage);
  // 返回
  return TRUE;
}

/*************************************************************************
 *
* \函数名称：
*   TemplateMatch()
 *
* \输入参数:
*   CDib* pDibSrc  - 指向CDib类的指针，含有待匹配图象信息
*   CDib* pDibTemplate - 指向CDib类的指针，含有模板图象信息
*   int nImageHeight   - 图象的高度
*   int nImageWidth    - 图象的宽度
*   int nTemplateHeight   - 模板图象的高度
*   int nTemplateWidth    - 模板图象的宽度
*   LPBYTE lpSrc    - 指向源图像的指针
*   LPBYTE lpDst    - 指向源图像的指针
 *
* \返回值:
*   BOOL   - 成功则返回TRUE，否则返回FALSE
 *
* \说明:
*   该函数将对图象进行模板匹配操作。需要注意的是，此程序只处理256灰度级的
 *图象。
 *
 *************************************************************************
 */
BOOL TemplateMatch1(int nImageHeight, int nImageWidth, LPBYTE lpSrc1, int sizeSaveImage_cx,
    int nTemplateHeight, int nTemplateWidth, LPBYTE lpTemplateSrc1, int sizeSaveTemplate_cx)
{
  // 指向缓存图像的指针
  LPBYTE lpSrc, lpDst, lpDst1, lpTemplateSrc;

  //循环变量
  int i, j, m, n;

  //中间结果
  Float dSigmaST, dSigmaS, dSigmaT;

  //相似性测度
  Float R;

  //最大相似性测度
  Float dbMaxR;

  //最大相似性出现位置
  int nMaxWidth;
  int nMaxHeight;

  //像素值
  BYTE unchPixel;
  BYTE unchTemplatePixel;

  // 暂时分配内存，以保存新图像
  lpDst1 = MALLOC(BYTE, nImageHeight * sizeSaveImage_cx);

  // 初始化新分配的内存，设定初始值为255
  memset(lpDst1, (BYTE) 255, nImageHeight * sizeSaveImage_cx);

  //计算dSigmaT
  dSigmaT = 0;

  for (n = 0; n < nTemplateHeight; n++) {
    for (m = 0; m < nTemplateWidth; m++) {
      // 指向模板图像倒数第j行，第i个象素的指针
      lpTemplateSrc = (LPBYTE) lpTemplateSrc1 + sizeSaveTemplate_cx * n + m;
      unchTemplatePixel = (BYTE) * lpTemplateSrc;
      dSigmaT += (Float) unchTemplatePixel * unchTemplatePixel;
    }
  }

  //找到图像中最大相似性的出现位置
  dbMaxR = 0.0;

  for (j = 0; j < nImageHeight - nTemplateHeight + 1; j++) {
    for (i = 0; i < nImageWidth - nTemplateWidth + 1; i++) {
      dSigmaST = 0;
      dSigmaS = 0;

      for (n = 0; n < nTemplateHeight; n++) {
        for (m = 0; m < nTemplateWidth; m++) {
          // 指向源图像倒数第j+n行，第i+m个象素的指针
          lpSrc = (LPBYTE) lpSrc1 + sizeSaveImage_cx * (j + n) + (i + m);

          // 指向模板图像倒数第n行，第m个象素的指针
          lpTemplateSrc = (LPBYTE) lpTemplateSrc1 + sizeSaveTemplate_cx * n + m;

          unchPixel = (BYTE) * lpSrc;
          unchTemplatePixel = (BYTE) * lpTemplateSrc;

          dSigmaS += (Float) unchPixel * unchPixel;
          dSigmaST += (Float) unchPixel * unchTemplatePixel;
        }
      }

      //计算相似性
      R = dSigmaST / (sqrt(dSigmaS) * sqrt(dSigmaT));

      //与最大相似性比较
      if (R > dbMaxR) {
        dbMaxR = R;
        nMaxWidth = i;
        nMaxHeight = j;
      }
    }
  }

  //将最大相似性出现区域部分复制到目标图像
  for (n = 0; n < nTemplateHeight; n++) {
    for (m = 0; m < nTemplateWidth; m++) {
      lpTemplateSrc = (LPBYTE) lpTemplateSrc1 + sizeSaveTemplate_cx * n + m;
      lpDst = (LPBYTE) lpDst1 + sizeSaveImage_cx * (n + nMaxHeight) + (m + nMaxWidth);
      *lpDst = *lpTemplateSrc;
    }
  }

  // 复制图像
  memcpy(lpSrc1, lpDst1, sizeSaveImage_cx * nImageHeight);

  // 释放内存
  SAFEFREE(lpDst1);

  // 返回
  return TRUE;
}

/*************************************************************************
 *
* 函数名称：
*   AddMinus()
 *
* 参数:
*   LPBYTE lpDIBBitsBK  - 指向背景DIB图像指针
*   LPBYTE lpDIBBits   - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数）
*   int lHeight      - 源图像高度（象素数）
*   int lLineBytes   - 图像每行的字节数
*  BOOL  bAddMinus - 为TRUE时执行加运算，否则执行减运算。
 *
* 返回值:
*   BOOL               - 运算成功返回TRUE，否则返回FALSE。
 *
* 说明:
* 该函数用于对两幅图像进行加减运算。
*
* 要求目标图像为255个灰度值的灰度图像。
 ************************************************************************/
BOOL AddMinus(LPBYTE lpDIBBits, LPBYTE lpDIBBitsBK, int lWidth, int lHeight, int lLineBytes, BOOL bAddMinus)
{

  // 指向源图像的指针
  LPBYTE lpSrc, lpSrcBK;

  // 指向缓存图像的指针
  LPBYTE lpDst;

  // 指向缓存DIB图像的指针
  LPBYTE lpNewDIBBits;

  //循环变量
  int i;
  int j;

  //像素值
  BYTE pixel, pixelBK;

  // 暂时分配内存，以保存新图像
  lpNewDIBBits = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits == NULL) {
    // 分配内存失败
    return FALSE;
  }

  // 初始化新分配的内存，设定初始值为255
  lpDst = (LPBYTE) lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;
      lpSrcBK = (LPBYTE) lpDIBBitsBK + lLineBytes * j + i;

      // 指向目标图像倒数第j行，第i个象素的指针
      lpDst = (LPBYTE) lpNewDIBBits + lLineBytes * j + i;

      pixel = (BYTE) * lpSrc;
      pixelBK = (BYTE) * lpSrcBK;

      if (bAddMinus) {
        * lpDst = pixel + pixelBK > 255 ? 255 : pixel + pixelBK;
      }
      else {
        *lpDst = pixel - pixelBK < 0 ? 0 : pixel - pixelBK;
      }


    }
  }


  // 复制腐蚀后的图像
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  // 释放内存
  SAFEFREE(lpNewDIBBits);

  // 返回
  return TRUE;
}

/*************************************************************************
 *
* 函数名称：
*   Hproject()
 *
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数）
*   int lHeight      - 源图像高度（象素数）
 *
* 返回值:
*   BOOL               - 运算成功返回TRUE，否则返回FALSE。
 *
* 说明:
* 该函数用于对两幅图像进行水平投影运算。
*
* 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/

BOOL Hproject(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // 指向源图像的指针
  LPBYTE lpSrc;

  // 指向缓存图像的指针
  LPBYTE lpDst;

  // 指向缓存DIB图像的指针
  LPBYTE lpNewDIBBits;

  //循环变量
  int i;
  int j;

  //图像中每行内的黑点个数
  int lBlackNumber;

  //像素值
  BYTE pixel;

  // 暂时分配内存，以保存新图像
  lpNewDIBBits = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits == NULL) {
    // 分配内存失败
    return FALSE;
  }

  // 初始化新分配的内存，设定初始值为255
  lpDst = (LPBYTE) lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  for (j = 0; j < lHeight; j++) {
    lBlackNumber = 0;

    for (i = 0; i < lWidth; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      pixel = (BYTE) * lpSrc;

      if (pixel != 255 && pixel != 0) {
        return FALSE;
      }

      if (pixel == 0) {
        lBlackNumber++;
      }
    }

    for (i = 0; i < lBlackNumber; i++) {
      // 指向目标图像倒数第j行，第i个象素的指针
      lpDst = (LPBYTE) lpNewDIBBits + lLineBytes * j + i;

      *lpDst = (BYTE) 0;
    }
  }


  // 复制投影图像
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  // 释放内存
  SAFEFREE(lpNewDIBBits);

  // 返回
  return TRUE;
}

/*************************************************************************
 *
* 函数名称：
*   Vproject()
 *
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数）
*   int lHeight      - 源图像高度（象素数）
 *
* 返回值:
*   BOOL               - 运算成功返回TRUE，否则返回FALSE。
 *
* 说明:
* 该函数用于对两幅图像进行垂直投影运算。
*
* 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/
BOOL Vproject(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // 指向源图像的指针
  LPBYTE lpSrc;

  // 指向缓存图像的指针
  LPBYTE lpDst;

  // 指向缓存DIB图像的指针
  LPBYTE lpNewDIBBits;

  // 循环变量
  int i, j;

  // 图像中每行内的黑点个数
  int lBlackNumber;

  // 像素值
  BYTE pixel;

  // 暂时分配内存，以保存新图像
  lpNewDIBBits = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits == NULL) {
    // 分配内存失败
    return FALSE;
  }

  // 初始化新分配的内存，设定初始值为255
  lpDst = (LPBYTE) lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  for (i = 0; i < lWidth; i++) {
    lBlackNumber = 0;

    for (j = 0; j < lHeight; j++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      pixel = (BYTE) * lpSrc;

      if (pixel != 255 && pixel != 0) {
        return FALSE;
      }

      if (pixel == 0) {
        lBlackNumber++;
      }
    }

    for (j = 0; j < lBlackNumber; j++) {
      // 指向目标图像倒数第j行，第i个象素的指针
      lpDst = (LPBYTE) lpNewDIBBits + lLineBytes * j + i;

      *lpDst = (BYTE) 0;
    }
  }


  // 复制投影图像
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  // 释放内存
  SAFEFREE(lpNewDIBBits);

  // 返回
  return TRUE;
}

/*************************************************************************
 *
* 函数名称：
*   TemplateMatch()
 *
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   LPBYTE lpDIBBitsBK  - 指向背景DIB图像指针
*   int lWidth       - 源图像宽度（象素数）
*   int lHeight      - 源图像高度（象素数）
*   int lTemplateWidth       - 模板图像宽度（象素数）
*   int lTemplateHeight      - 模板图像高度（象素数）
 *
* 返回值:
*   BOOL               - 运算成功返回TRUE，否则返回FALSE。
 *
* 说明:
* 该函数用于对图像进行模板匹配运算。
*
* 要求目标图像为255个灰度值的灰度图像。
 ************************************************************************/
BOOL TemplateMatchDIB(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    int lTemplateHeight, int lTemplateWidth,
    LPBYTE lpTemplateDIBBits, int lTemplateLineBytes)
{
  // 指向源图像的指针
  LPBYTE lpSrc, lpTemplateSrc;

  // 指向缓存图像的指针
  LPBYTE lpDst;

  // 指向缓存DIB图像的指针
  LPBYTE lpNewDIBBits;

  //循环变量
  int i, j, m, n;

  //中间结果
  Float dSigmaST, dSigmaS, dSigmaT;

  //相似性测度
  Float R;

  //最大相似性测度
  Float MaxR;

  //最大相似性出现位置
  int lMaxWidth;
  int lMaxHeight;

  //像素值
  BYTE pixel;
  BYTE templatepixel;

  // 暂时分配内存，以保存新图像
  lpNewDIBBits = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits == NULL) {
    // 分配内存失败
    return FALSE;
  }

  // 初始化新分配的内存，设定初始值为255
  lpDst = (LPBYTE) lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  //计算dSigmaT
  dSigmaT = 0;

  for (n = 0; n < lTemplateHeight; n++) {
    for (m = 0; m < lTemplateWidth; m++) {
      // 指向模板图像倒数第j行，第i个象素的指针
      lpTemplateSrc = (LPBYTE) lpTemplateDIBBits + lTemplateLineBytes * n + m;
      templatepixel = (BYTE) * lpTemplateSrc;
      dSigmaT += (Float) templatepixel * templatepixel;
    }
  }

  //找到图像中最大相似性的出现位置
  MaxR = 0.0;

  for (j = 0; j < lHeight - lTemplateHeight + 1; j++) {
    for (i = 0; i < lWidth - lTemplateWidth + 1; i++) {
      dSigmaST = 0;
      dSigmaS = 0;

      for (n = 0; n < lTemplateHeight; n++) {
        for (m = 0; m < lTemplateWidth; m++) {
          // 指向源图像倒数第j+n行，第i+m个象素的指针
          lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (j + n) + (i + m);

          // 指向模板图像倒数第n行，第m个象素的指针
          lpTemplateSrc = (LPBYTE) lpTemplateDIBBits + lTemplateLineBytes * n + m;

          pixel = (BYTE) * lpSrc;
          templatepixel = (BYTE) * lpTemplateSrc;

          dSigmaS += (Float) pixel * pixel;
          dSigmaST += (Float) pixel * templatepixel;
        }
      }

      //计算相似性
      R = dSigmaST / (sqrt(dSigmaS) * sqrt(dSigmaT));

      //与最大相似性比较
      if (R > MaxR) {
        MaxR = R;
        lMaxWidth = i;
        lMaxHeight = j;
      }
    }
  }

  //将最大相似性出现区域部分复制到目标图像
  for (n = 0; n < lTemplateHeight; n++) {
    for (m = 0; m < lTemplateWidth; m++) {
      lpTemplateSrc = (LPBYTE) lpTemplateDIBBits + lTemplateLineBytes * n + m;
      lpDst = (LPBYTE) lpNewDIBBits + lLineBytes * (n + lMaxHeight) + (m + lMaxWidth);
      *lpDst = *lpTemplateSrc;
    }
  }

  // 复制图像
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  // 释放内存
  SAFEFREE(lpNewDIBBits);

  // 返回
  return TRUE;
}
