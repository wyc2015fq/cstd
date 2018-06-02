
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "macro.h"
#include "imglib.h"

//////////////////////////////////////////////////////////////////////
// 图像点运算函数
//////////////////////////////////////////////////////////////////////

/*************************************************************************
* \函数名称：
*   GrayLinTrans()
* \输入参数:
*     LPBYTE lpDIBBits        //指向源图像的像素指针
*     int lWidth        //源图像的宽度
*     int lHeight        // 源图像的高度
*   int  oralow            // 原始图像的灰度值集中区域－低值
*   int  orahig            // 原始图像的灰度值集中区域－高值
*   int  newlow            // 变换后图像灰度的动态范围－低值
*   int  newhig            // 变换后图像灰度的动态范围－高值
* \返回值:
*   BOOL               // 成功返回TRUE，否则返回FALSE。
* \说明:
*   该函数用来对图像指定的灰度分布进行灰度变换主要用来实现灰度分布的扩展
*************************************************************************/
BOOL GrayLinTrans(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int oralow, int orahig, int newlow, int newhig)
{
  LPBYTE lpSrc;                        // 指向源图像的指针
  int i, j;                            // 循环变量
  BYTE byMap[ 256 ];                  // 定义灰度映射表

  for (i = 0; i <= oralow; i++) {         //当灰度级小于要增强的灰度级时
    if (newlow > 0) {   //判断新灰度级是否大于0
      byMap[ i ] = newlow;               //赋予新值
    }
    else {
      byMap[ i ] = 0;                   //直接赋值为0
    }
  }

  for (; i <= orahig; i++) {
    // 判断orahig是否等于oralow(防止分母为0)
    if (orahig != oralow) {
      //线性变换
      byMap[ i ] = newlow + (BYTE)((newhig - newlow) * (i - oralow) / (orahig - oralow));
    }
    else {
      byMap[ i ] = newlow;    //直接赋值为newlow
    }
  }

  for (; i < newhig; i++) {
    if (newhig <= 255) {         //判断d是否大于255
      byMap[ i ] = newhig;    //直接赋值为newhig
    }
    else {
      byMap[ i ] = 255;        //直接赋值为255
    }
  }

  for (i = 0; i < lHeight; i++) {   //对图像的每个像素值进行变换
    for (j = 0; j < lWidth; j++) {   //每列
      // 指向DIB第i行，第j个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * (lHeight - 1 - i) + j;
      *lpSrc = byMap[ *lpSrc ];   //用新的灰度替代原有的灰度
    }
  }

  return TRUE;     //返回
}


/*************************************************************************
* 函数名称：
*   LinerTrans()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数）
*   int lHeight      - 源图像高度（象素数）
*   Float fA      - 线性变换的斜率
*   Float fB           - 线性变换的截距
* 返回值:
*   BOOL               - 成功返回TRUE，否则返回FALSE。
* 说明:
*   该函数用来对图像进行灰度的线性变换操作。
 ************************************************************************/
BOOL LinerTrans(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    Float fA, Float fB)
{
  LPBYTE lpSrc;  // 指向源图像的指针
  int i;     // 循环变量
  int j;
  Float fTemp;    // 中间变量

  for (i = 0; i < lHeight; i++) {    // 每行
    for (j = 0; j < lWidth; j++) {    // 每列
      // 指向DIB第i行，第j个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
      fTemp = fA * (*lpSrc) + fB;    // 线性变换

      if (fTemp > 255) {     // 判断是否超出范围
        *lpSrc = 255;
      }
      else if (fTemp < 0) {
        *lpSrc = 0;
      }
      else {
        *lpSrc = (BYTE)(fTemp + 0.5);      // 四舍五入
      }
    }
  }

  return TRUE;
}

/*************************************************************************
 * 函数名称：
 *   Threshold()
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
 * 该函数用于对图像进行阈值分割运算。
 ************************************************************************/

BOOL Threshold(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc;       // 指向源图像的指针
  LPBYTE lpDst;       // 指向缓存图像的指针
  LPBYTE lpNewDIBBits; // 指向缓存DIB图像的指针
  int i, j; //循环变量
  BYTE pixel; //像素值
  int lHistogram[ 256 ]; //直方图数组
  int iIterationTimes; //迭代次数

  //阈值，最大灰度值与最小灰度值，两个区域的平均灰度值
  BYTE iThreshold, iNewThreshold, iMaxGrayValue, iMinGrayValue, iMean1GrayValue, iMean2GrayValue;

  //用于计算区域灰度平均值的中间变量
  int lP1, lP2, lS1, lS2;

  // 暂时分配内存，以保存新图像
  lpNewDIBBits = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits == NULL) {
    // 分配内存失败
    return FALSE;
  }

  // 初始化新分配的内存，设定初始值为255
  lpDst = (LPBYTE) lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  for (i = 0; i < 256; i++) {
    lHistogram[ i ] = 0;
  }

  //获得直方图
  iMaxGrayValue = 0;
  iMinGrayValue = 255;

  for (i = 0; i < lWidth; i++) {
    for (j = 0; j < lHeight; j++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      pixel = (BYTE) * lpSrc;

      lHistogram[ pixel ] ++;

      //修改最大，最小灰度值
      if (iMinGrayValue > pixel) {
        iMinGrayValue = pixel;
      }

      if (iMaxGrayValue < pixel) {
        iMaxGrayValue = pixel;
      }
    }
  }

  //迭代求最佳阈值
  iNewThreshold = (iMinGrayValue + iMaxGrayValue) / 2;
  iThreshold = 0;

  for (iIterationTimes = 0; iThreshold != iNewThreshold && iIterationTimes < 100; iIterationTimes ++) {
    iThreshold = iNewThreshold;
    lP1 = 0;
    lP2 = 0;
    lS1 = 0;
    lS2 = 0;

    //求两个区域的灰度平均值
    for (i = iMinGrayValue; i < iThreshold; i++) {
      lP1 += lHistogram[ i ] * i;
      lS1 += lHistogram[ i ];
    }

    iMean1GrayValue = (BYTE)(lP1 / lS1);

    for (i = iThreshold + 1; i < iMaxGrayValue; i++) {
      lP2 += lHistogram[ i ] * i;
      lS2 += lHistogram[ i ];
    }

    iMean2GrayValue = (BYTE)(lP2 / lS2);
    iNewThreshold = (iMean1GrayValue + iMean2GrayValue) / 2;
  }

  //根据阈值将图像二值化
  for (i = 0; i < lWidth; i++) {
    for (j = 0; j < lHeight; j++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      // 指向目标图像倒数第j行，第i个象素的指针
      lpDst = (LPBYTE) lpNewDIBBits + lLineBytes * j + i;

      pixel = (BYTE) * lpSrc;

      if (pixel <= iThreshold) {
        *lpDst = (BYTE) 0;
      }
      else {
        *lpDst = (BYTE) 255;
      }
    }
  }

  // 复制图像
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  // 释放内存
  SAFEFREE(lpNewDIBBits);

  // 返回
  return TRUE;
}

/*************************************************************************
 * 函数名称：
 *   ThresholdTrans()
 * 参数:
 *   LPBYTE lpDIBBits    - 指向源DIB图像指针
 *   int lWidth       - 源图像宽度（象素数）
 *   int lHeight      - 源图像高度（象素数）
 *   BYTE  bThre     - 阈值
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 * 说明:
 *   该函数用来对图像进行阈值变换。对于灰度值小于阈值的象素直接设置
 * 灰度值为0；灰度值大于阈值的象素直接设置为255。
 ************************************************************************/
BOOL ThresholdTrans(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, BYTE bThre)
{
  LPBYTE lpSrc;    // 指向源图像的指针
  int i;       // 循环变量
  int j;

  for (i = 0; i < lHeight; i++) {   // 每行
    for (j = 0; j < lWidth; j++) {   // 每列
      // 指向DIB第i行，第j个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;

      if ((*lpSrc) < bThre) {     // 判断是否小于阈值
        *lpSrc = 0;
      }
      else {
        *lpSrc = 255;
      }
    }
  }

  return TRUE;
}

/*************************************************************************
* 函数名称：
*   WindowTrans()
* 参数:
*   LPBYTE lpDIBBits   - 指向源DIB图像指针
*   int lWidth         - 源图像宽度（象素数）
*   int lHeight        - 源图像高度（象素数）
*   BYTE  bLow         - 窗口下限
*   BYTE  bUp          - 窗口上限
* 返回值:
*   BOOL               - 成功返回TRUE，否则返回FALSE。
* 说明:
*   该函数用来对图像进行窗口变换。只有在窗口范围内的灰度保持不变，
* 小于下限的象素直接设置灰度值为0；大于上限的象素直接设置灰度值为255。
 ************************************************************************/
BOOL WindowTrans(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    BYTE bLow, BYTE bUp)
{
  LPBYTE lpSrc;    // 指向源图像的指针
  int i;       // 循环变量
  int j;

  for (i = 0; i < lHeight; i++) {   // 每行
    for (j = 0; j < lWidth; j++) {   // 每列
      // 指向DIB第i行，第j个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;

      if ((*lpSrc) < bLow) {     // 判断是否超出范围
        *lpSrc = 0;
      }
      else if ((*lpSrc) > bUp) {
        *lpSrc = 255;
      }
    }
  }

  return TRUE;
}

/*************************************************************************
* 函数名称：
*   GrayStretch()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数）
*   int lHeight      - 源图像高度（象素数）
*   BYTE bX1   - 灰度拉伸第一个点的X坐标
*   BYTE bY1   - 灰度拉伸第一个点的Y坐标
*   BYTE bX2   - 灰度拉伸第二个点的X坐标
*   BYTE bY2   - 灰度拉伸第二个点的Y坐标
* 返回值:
*   BOOL               - 成功返回TRUE，否则返回FALSE。
* 说明:
*   该函数用来对图像进行灰度拉伸。
 ************************************************************************/
BOOL GrayStretch(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    BYTE bX1, BYTE bY1, BYTE bX2, BYTE bY2)
{
  LPBYTE lpSrc;    // 指向源图像的指针
  int i;       // 循环变量
  int j;
  BYTE bMap[ 256 ];     // 灰度映射表

  // 计算灰度映射表
  for (i = 0; i <= bX1; i++) {
    if (bX1 > 0) {      // 判断bX1是否大于0（防止分母为0）
      bMap[ i ] = (BYTE) bY1 * i / bX1;
    }
    else {
      bMap[ i ] = 0;
    }
  }

  for (; i <= bX2; i++) {
    if (bX2 != bX1) {      // 判断bX1是否等于bX2（防止分母为0）
      bMap[ i ] = bY1 + (BYTE)((bY2 - bY1) * (i - bX1) / (bX2 - bX1));
    }
    else {
      bMap[ i ] = bY1;
    }
  }

  for (; i < 256; i++) {
    if (bX2 != 255) {      // 判断bX2是否等于255（防止分母为0）
      bMap[ i ] = bY2 + (BYTE)((255 - bY2) * (i - bX2) / (255 - bX2));
    }
    else {
      bMap[ i ] = 255;
    }
  }

  for (i = 0; i < lHeight; i++) {   // 每行
    for (j = 0; j < lWidth; j++) {   // 每列
      // 指向DIB第i行，第j个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
      // 计算新的灰度值
      *lpSrc = bMap[ *lpSrc ];
    }
  }

  return TRUE;
}

/*************************************************************************
* 函数名称：
*   InteEqualize()
* 参数:
*   LPBYTE lpDIBBits   - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数）
*   int lHeight      - 源图像高度（象素数）
*   int lLineBytes   - 图像每行的字节数
* 返回值:
*   BOOL               - 成功返回TRUE，否则返回FALSE。
* 说明:
*   该函数用来对图像进行直方图均衡。
 ************************************************************************/
BOOL InteEqualize(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc;    // 指向源图像的指针
  int lTemp;      // 临时变量
  int i;       // 循环变量
  int j;
  BYTE bMap[ 256 ];     // 灰度映射表
  int lCount[ 256 ];    // 灰度映射表

  for (i = 0; i < 256; i ++) {   // 重置计数为0
    lCount[ i ] = 0;
  }

  // 计算各个灰度值的计数
  for (i = 0; i < lHeight; i ++) {
    for (j = 0; j < lWidth; j ++) {
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * i + j;
      lCount[ *(lpSrc) ] ++;
    }
  }

  // 计算灰度映射表
  for (i = 0; i < 256; i++) {
    lTemp = 0;

    for (j = 0; j <= i; j++) {
      lTemp += lCount[ j ];
    }

    // 计算对应的新灰度值
    bMap[ i ] = (BYTE)(lTemp * 255 / lHeight / lWidth);
  }

  for (i = 0; i < lHeight; i++) {    // 每行
    for (j = 0; j < lWidth; j++) {    // 每列
      // 指向DIB第i行，第j个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
      // 计算新的灰度值
      *lpSrc = bMap[ *lpSrc ];
    }
  }

  return TRUE;
}

/*************************************************************************
* 函数名称：LineTranslation(LPBYTE lpSrcStartBits, int lWidth, int lHeight, int lLineBytes, Float fSlope, Float fIntercept, int lLineBytes)
* 函数参数:
*   LPBYTE lpSrcStartBits，指向源DIB图象起始像素的指针
*   int lWidth，DIB图象宽度
*   int lHeight，DIB图象高度
*   Float fSlope，斜率
*   Float fIntercept，截距
*   intlLineBytes，DIB图象的行字节数，为4的倍数
* 函数类型:BOOL
* 函数功能:该函数用来对图象进行灰度的线性变换操作
 ************************************************************************/
BOOL LineTranslation(LPBYTE lpSrcStartBits, int lWidth, int lHeight, int lLineBytes, Float fSlope, Float fIntercept)
{
  int i;                 //行循环变量
  int j;                 //列循环变量
  Float varFloat;        //浮点参数变量
  LPBYTE lpSrcUnChr;    //指向像素的指针

  for (i = 0; i < lHeight; i++) { // 行
    for (j = 0; j < lWidth; j++) { // 列
      lpSrcUnChr = (LPBYTE) lpSrcStartBits + lLineBytes * (lHeight - 1 - i) + j;     // 指向DIB第i行，第j个像素的指针
      varFloat = fSlope * (*lpSrcUnChr) + fIntercept;   // 线性变换

      if (varFloat > 255) { // 超出范围直接赋值为255
        *lpSrcUnChr = 255;
      }
      else if (varFloat < 0) {
        *lpSrcUnChr = 0; // 直接赋值为0
      }
      else {
        *lpSrcUnChr = (BYTE)(varFloat + 0.5);      // 近似
      }
    }
  }

  return TRUE;
}

/*************************************************************************
* 函数名称：LogTranslation(LPBYTE lpSrcStartBits, int lWidth, int lHeight, int lLineBytes)
* 函数参数:
     LPBYTE lpSrcStartBits, 指向DIB起始像素的指针
     int   lWidth，DIB图象的宽度
     int   lHeight，DIB图象的高度
     int   lLineBytes，DIB图象的行字节数，为4的倍数
* 函数类型:BOOL
* 函数功能:该函数用来对图象进行对数变换
 ************************************************************************/

BOOL LogTranslation(LPBYTE lpSrcStartBits, int lWidth, int lHeight, int lLineBytes)
{
  int i;                 //行循环变量
  int j;                 //列循环变量
  LPBYTE lpSrcUnChr;    //指向像素的指针

  for (i = 0; i < lHeight; i++) { // 行
    for (j = 0; j < lWidth; j++) { // 列
      lpSrcUnChr = (LPBYTE) lpSrcStartBits + lLineBytes * (lHeight - 1 - i) + j;     // 指向DIB第i行，第j个像素的指针
      *lpSrcUnChr = 50 * (BYTE) log((*lpSrcUnChr + 1));
    }
  }

  return TRUE;
}
