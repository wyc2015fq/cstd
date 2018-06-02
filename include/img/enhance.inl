
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include "macro.h"
//#include "imglib.h"

/*************************************************************************
* 函数名称：
*   GradSharp()
* 参数:
*   uchar* pBits    - 指向源DIB图像指针
*   int w       - 源图像宽度（象素数）
*   int h      - 源图像高度（象素数）
*   uchar  bThre  - 阈值
* 返回值:
*   BOOL               - 成功返回TRUE，否则返回FALSE。
* 说明:
*   该函数用来对图像进行梯度锐化。
 ************************************************************************/
static BOOL GradSharp(int h, int w, uchar* pBits, int lLineBytes, uchar bThre)
{
  uchar* pSrc;  // 指向源图像的指针
  uchar* lpSrc1;
  uchar* lpSrc2;
  int i, j;    // 循环变量
  uchar bTemp;

  for (i = 0; i < h; i++) {   // 每行
    for (j = 0; j < w; j++) {   // 每列
      // 指向DIB第i行，第j个象素的指针
      pSrc = (uchar*) pBits + lLineBytes * (h - 1 - i) + j;
      // 指向DIB第i+1行，第j个象素的指针
      lpSrc1 = (uchar*) pBits + lLineBytes * (h - 2 - i) + j;
      // 指向DIB第i行，第j+1个象素的指针
      lpSrc2 = (uchar*) pBits + lLineBytes * (h - 1 - i)
          + j + 1;

      bTemp = abs((*pSrc) - (*lpSrc1)) + abs((*pSrc) - (*lpSrc2));

      // 判断是否小于阈值
      if (bTemp < 255) {
        // 判断是否大于阈值，对于小于情况，灰度值不变。
        if (bTemp >= bThre) {
          *pSrc = bTemp;
        }
      }
      else {
        *pSrc = 255;
      }
    }
  }

  return TRUE;
}

// 直方图均衡化
static BOOL HistogramEqualize(int h, int w, uchar* pBits, int lLineBytes)
{
  // 临时变量
  int nTemp;

  // 循环变量
  int i, j;

  // 累积直方图，即灰度映射表
  uchar byMap[ 256 ];

  // 直方图
  int nCount[ 256 ] = {0};
  int sumcnt[ 256 ] = {0};

  // 计算各个灰度值的计数，即得到直方图
  for (i = 0; i < h; i ++) {
    uchar* pSrc = pBits + lLineBytes * i;
    for (j = 0; j < w; j ++) {
      // 计数加1
      nCount[ pSrc[j] ] ++;
    }
  }

  sumcnt[0] = nCount[0];

  // 计算累积直方图
  for (i = 0; i < 256; i++) {
    nTemp = sumcnt[i] = sumcnt[i-1] + nCount[ i ];
    // 计算对应的新灰度值
    byMap[ i ] = (uchar)(nTemp * 255 / h / w);
  }

  // 每行
  for (i = 0; i < h; i++) {
    uchar* pSrc = pBits + lLineBytes * i;
    for (j = 0; j < w; j++) {
      // 计算新的灰度值
      pSrc[j] = byMap[ pSrc[j] ];
    }
  }

  // 返回
  return TRUE;
}

/*************************************************************************
 *
* \函数名称：
*   GraySegLinTrans()
 *
* \输入参数:
*   int h, int w, uchar* pBits, int lLineBytes            - 指向CDib类的指针，含有原始图象信息
*   int  nX1            - 分段线性灰度变换第一个拐点的X坐标
*   int  nY1            - 分段线性灰度变换第一个拐点的Y坐标
*   int  nX2            - 分段线性灰度变换第二个拐点的X坐标
*   int  nY2            - 分段线性灰度变换第二个拐点的Y坐标
 *
* \返回值:
*   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
* \说明:
*   该函数用来对图像进行分段线性灰度变换，输入参数中包含了两个拐点的坐标
 *
 *************************************************************************
 */
static BOOL GraySegLinTrans(int h, int w, uchar* pBits, int lLineBytes, int nX1, int nY1, int nX2, int nY2)
{

  // 指向源图像的指针
  uchar* pSrc;

  // 循环变量
  int i, j;

  // 灰度映射表
  uchar byMap[ 256 ];

  // 图像每行的字节数
  //int   lLineBytes;

  // 计算图像每行的字节数
  //lLineBytes = WIDTHBYTES(w* 8);

  // 计算灰度映射表
  for (i = 0; i <= nX1; i++) {
    // 判断nX1是否大于0（防止分母为0）
    if (nX1 > 0) {
      // 线性变换
      byMap[ i ] = (uchar) nY1 * i / nX1;
    }
    else {
      // 直接赋值为0
      byMap[ i ] = 0;
    }
  }

  for (; i <= nX2; i++) {
    // 判断nX1是否等于nX2（防止分母为0）
    if (nX2 != nX1) {
      // 线性变换
      byMap[ i ] = nY1 + (uchar)((nY2 - nY1) * (i - nX1) / (nX2 - nX1));
    }
    else {
      // 直接赋值为nY1
      byMap[ i ] = nY1;
    }
  }

  for (; i < 256; i++) {
    // 判断nX2是否等于255（防止分母为0）
    if (nX2 != 255) {
      // 线性变换
      byMap[ i ] = nY2 + (uchar)((255 - nY2) * (i - nX2) / (255 - nX2));
    }
    else {
      // 直接赋值为255
      byMap[ i ] = 255;
    }
  }

  // 对图象的象素值进行变换
  // 每行
  for (i = 0; i < h; i++) {
    // 每列
    for (j = 0; j < w; j++) {
      // 指向DIB第i行，第j个象素的指针 (i*lLineBytes+j)
      pSrc = (uchar*) pBits + (i * lLineBytes + j);

      // 计算新的灰度值
      *pSrc = byMap[ *pSrc ];
    }
  }

  // 返回
  return TRUE;
}

/*************************************************************************
 *
* \函数名称：
*   GeneralTemplate()
 *
* \输入参数:
*   CDib* pDib    - 指向CDib类的指针，含有原始图象信息
*   int nTempWidth    - 模板的宽度
*   int nTempHeight        - 模板的高度
*   int nTempCenX        - 模板中心的X坐标（相对于模板）
*   int nTempCenY        - 模板中心的Y坐标（相对于模板）
*   float* pdbTemp        - 模板数组的指针
*   float* dbCoef        - 模板的系数
 *
* \返回值:
*   BOOL            - 成功则返回TRUE，否则返回FALSE
 *
* \说明:
*   该函数用指定的模板对pDib指向的图象进行模板操作。模板的定义了宽度，高度，
*    中心坐标和系数，模板的数据存放在pdbTemp中。对图象进行模板操作后，仍
*    然存放在pDib指向的CDib对象中。需要注意的是，该函数只能处理8位的图象，
*   否则，指向的数据将出错。
 *
 *************************************************************************
 */
static BOOL GeneralTemplate(int h, int w, uchar* pBits, int lLineBytes, int nTempWidth, int nTempHeight,
    int nTempCenX, int nTempCenY,
    float* pdbTemp, float dbCoef)
{
  // 临时存放图像数据的指针
  uchar* lpImage;

  // 循环变量
  int i, j, k, l;

  // 指向源图像的指针
  uchar* pSrc;

  // 指向要复制区域的指针
  uchar* lpDst;

  // 计算结果
  float dbResult;

  // 获得图象数据存储的尺寸
  int nSizeImage;
  nSizeImage = h * lLineBytes;

  // 给临时存放数据分配内存
  lpImage = (uchar*) MALLOC(uchar, nSizeImage);

  // 判断是否内存分配失败
  if (lpImage == NULL) {
    // 分配内存失败
    return FALSE;
  }

  // 将原始图像的数据拷贝到临时存放内存中
  memcpy(lpImage, pBits, nSizeImage);



  // 进行模板操作
  // 行(除去边缘几行)
  for (i = nTempCenY; i < h - nTempHeight + nTempCenY + 1; i++) {
    // 列(除去边缘几列)
    for (j = nTempCenX; j < w - nTempWidth + nTempCenX + 1; j++) {
      // 指向新DIB第i行，第j个象素的指针
      lpDst = (uchar*) lpImage + (i * lLineBytes + j);

      dbResult = 0;

      // 计算
      for (k = 0; k < nTempHeight; k++) {
        for (l = 0; l < nTempWidth; l++) {
#define GetPixelOffset(i, j) (lLineBytes*(i) + j)
          // 指向DIB第i - nTempCenY + k行，第j - nTempCenX + l个象素的指针
          pSrc = (uchar*) pBits + GetPixelOffset(i - nTempCenY + k, j - nTempCenX + l);

          // 保存象素值
          dbResult += (* pSrc) * pdbTemp[ k * nTempWidth + l ];
        }
      }

      // 乘上系数
      dbResult *= dbCoef;

      // 取绝对值
      dbResult = (float) fabs(dbResult);

      // 判断是否超过255
      if (dbResult > 255) {
        // 直接赋值为255
        * lpDst = 255;
      }
      else {
        // 赋值
        * lpDst = (uchar)(dbResult + 0.5);
      }

    }
  }

  // 复制变换后的图像
  memcpy(pBits, lpImage, nSizeImage);

  // 释放内存
  FREE(lpImage);

  // 返回
  return TRUE;
}


/*************************************************************************
 *
* 函数名称：
*   MedianFilter()
 *
* \输入参数:
*   CDib* pDib        - 指向CDib类的指针，含有原始图象信息
*   int nTempWidth        - 模板的宽度
*   int nTempHeight        - 模板的高度
*   int nTempCenX        - 模板中心的X坐标（相对于模板）
*   int nTempCenY        - 模板中心的Y坐标（相对于模板）
 *
* \返回值:
*   BOOL            - 成功则返回TRUE，否则返回FALSE
 *
* 说明:
*   该函数对指定的DIB图像进行中值滤波。
 *
 ************************************************************************/
static BOOL MedianFilter1(int h, int w, uchar* pBits, int lLineBytes, int nTempWidth, int nTempHeight,
    int nTempCenX, int nTempCenY)
{
  // 临时存放图像数据的指针
  uchar* lpImage;

  // 循环变量
  int i, j, k, l;

  // 指向源图像的指针
  uchar* pSrc;

  // 指向要复制区域的指针
  //uchar* lpDst;

  // 获得图象数据存储的尺寸
  int nSizeImage = h * lLineBytes;

  // 指向滤波器数组的指针
  uchar* pUnchFltValue;

  // 给临时存放数据分配内存
  lpImage = (uchar*) MALLOC(uchar, nSizeImage);

  // 判断是否内存分配失败
  if (lpImage == NULL) {
    // 返回
    return FALSE;
  }

  // 将原始图像的数据拷贝到临时存放内存中
  memcpy(lpImage, pBits, nSizeImage);

  // 暂时分配内存，以保存滤波器数组
  pUnchFltValue = MALLOC(uchar, nTempHeight * nTempWidth);

  // 判断是否内存分配失败
  if (pUnchFltValue == NULL) {
    // 释放已分配内存
    FREE(lpImage);
    // 返回
    return FALSE;
  }

  // 开始中值滤波
  // 行(除去边缘几行)
  for (i = nTempCenY; i < h - nTempHeight + nTempCenY + 1; i++) {
    // 列(除去边缘几列)
    for (j = nTempCenX; j < w - nTempWidth + nTempCenX + 1; j++) {
      // 指向新DIB第i行，第j个象素的指针
      //lpDst = (uchar*) lpImage + (i * lLineBytes + j);
      //lpDst = (uchar* )lpImage + w* (h - 1 - i) + j;

      // 读取滤波器数组
      for (k = 0; k < nTempHeight; k++) {
        for (l = 0; l < nTempWidth; l++) {
          // 指向DIB第i - nTempCenY + k行，第j - nTempCenX + l个象素的指针
          pSrc = (uchar*) pBits + GetPixelOffset(i - nTempCenY + k, j - nTempCenX + l);
          //pSrc = (uchar* )pBits + w* (h - 1 - i + nTempCenY - k) + j - nTempCenX + l;

          // 保存象素值
          pUnchFltValue[ k * nTempWidth + l ] = *pSrc;
        }
      }

      // 获取中值
      //* lpDst = GetMedianValue(pUnchFltValue, nTempHeight* nTempWidth);
    }
  }

  // 复制变换后的图像
  memcpy(pBits, lpImage, nSizeImage);

  // 释放内存
  FREE(lpImage);
  FREE(pUnchFltValue);

  // 返回
  return TRUE;
}

/*************************************************************************
 *
* 函数名称：
*   GetMedianValue()
 *
* 参数:
*   uchar* pUnchFltValue    - 指向要获取中值的数组指针
*   int  iFilterLen            - 数组长度
 *
* 返回值:
*   uchar            - 返回指定数组的中值。
 *
* 说明:
*   该函数用冒泡法对一维数组进行排序，并返回数组元素的中值。
 *
 ************************************************************************/

static uchar GetMedianValue(uchar* pUnchFltValue, int iFilterLen)
{
  // 循环变量
  int i;
  int j;

  // 中间变量
  uchar bTemp;

  // 用冒泡法对数组进行排序
  for (j = 0; j < iFilterLen - 1; j ++) {
    for (i = 0; i < iFilterLen - j - 1; i ++) {
      if (pUnchFltValue[ i ] > pUnchFltValue[ i + 1 ]) {
        // 互换
        bTemp = pUnchFltValue[ i ];
        pUnchFltValue[ i ] = pUnchFltValue[ i + 1 ];
        pUnchFltValue[ i + 1 ] = bTemp;
      }
    }
  }

  // 计算中值
  if ((iFilterLen & 1) > 0) {
    // 数组有奇数个元素，返回中间一个元素
    bTemp = pUnchFltValue[(iFilterLen + 1) / 2 ];
  }
  else {
    // 数组有偶数个元素，返回中间两个元素平均值
    bTemp = (pUnchFltValue[ iFilterLen / 2 ] + pUnchFltValue[ iFilterLen / 2 + 1 ]) / 2;
  }

  // 返回中值
  return bTemp;
}

/*************************************************************************
* \函数名称：
*   LinearSharpen()
 *
* \输入参数:
*   uchar* lpImage        - 指向图象数据得指针
*   int w            - 图象数据宽度
*   int h        - 图象数据高度
 *
* \返回值:
*   无
 *
* \说明:
*   线性锐化图象增强
*   本函数采用拉普拉斯算子对图象进行线性锐化
*   在原来图象上加上拉普拉斯算子锐化的信息
 **************************************************************************/
static void LinearSharpen(int h, int w, uchar* pBits, int lLineBytes)
{
  int y; // 遍历图象的纵坐标
  int x; // 遍历图象的横坐标
  uchar* lpImage = pBits;
  float* pdGrad;

  // 临时变量
  float dGrad;

  // 模板循环控制变量
  int yy;
  int xx;

  //这个变量用来表示Laplacian算子象素值
  int nTmp[ 3 ][ 3 ];

  // 设置模板系数
  static int nWeight[ 3 ][ 3 ];

  pdGrad = MALLOC(float, w * h);

  // 初始化为0
  memset(pdGrad, 0, w * h * sizeof(float));

  nWeight[ 0 ][ 0 ] = -1;
  nWeight[ 0 ][ 1 ] = -1;
  nWeight[ 0 ][ 2 ] = -1;
  nWeight[ 1 ][ 0 ] = -1;
  nWeight[ 1 ][ 1 ] = 8;
  nWeight[ 1 ][ 2 ] = -1;
  nWeight[ 2 ][ 0 ] = -1;
  nWeight[ 2 ][ 1 ] = -1;
  nWeight[ 2 ][ 2 ] = -1;

  for (y = 1; y < h - 1; y++)
    for (x = 1; x < w - 1; x++) {
      dGrad = 0;
      // Laplacian算子需要的各点象素值

      // 模板第一行
      nTmp[ 0 ][ 0 ] = lpImage[(y - 1) * lLineBytes + x - 1 ];
      nTmp[ 0 ][ 1 ] = lpImage[(y - 1) * lLineBytes + x ];
      nTmp[ 0 ][ 2 ] = lpImage[(y - 1) * lLineBytes + x + 1 ];

      // 模板第二行
      nTmp[ 1 ][ 0 ] = lpImage[ y * lLineBytes + x - 1 ];
      nTmp[ 1 ][ 1 ] = lpImage[ y * lLineBytes + x ];
      nTmp[ 1 ][ 2 ] = lpImage[ y * lLineBytes + x + 1 ];

      // 模板第三行
      nTmp[ 2 ][ 0 ] = lpImage[(y + 1) * lLineBytes + x - 1 ];
      nTmp[ 2 ][ 1 ] = lpImage[(y + 1) * lLineBytes + x ];
      nTmp[ 2 ][ 2 ] = lpImage[(y + 1) * lLineBytes + x + 1 ];

      // 计算梯度
      for (yy = 0; yy < 3; yy++)
        for (xx = 0; xx < 3; xx++) {
          dGrad += nTmp[ yy ][ xx ] * nWeight[ yy ][ xx ];
        }

      // 梯度值写入内存
      *(pdGrad + y * lLineBytes + x) = dGrad;
    }

  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      lpImage[ y * lLineBytes + x ] = (uchar) MAX(0, MIN(255, (lpImage[ y * lLineBytes + x ] + (int) pdGrad[ y * lLineBytes + x ])));
    }
  }

  FREE(pdGrad);
}

/*************************************************************************
* 函数名称：GrayEqualize(uchar* lpSrcStartBits, int w, int h, int lLineBytes, int   lLineBytes)
* 函数参数:
     uchar* lpSrcStartBits, 指向DIB起始像素的指针
     int   w，DIB图象的宽度
     int   h，DIB图象的高度
     int   lLineBytes，DIB图象的行字节数，为4的倍数
* 函数类型:BOOL
* 函数功能:该函数用来进行直方图均衡
 ************************************************************************/
static BOOL GrayEqualize(uchar* lpSrcStartBits, int w, int h, int lLineBytes)
{
  int i;                 //行循环变量
  int j;                 //列循环变量
  uchar* lpSrcUnChr;    //指向像素的指针

  uchar bGrayMap[ 256 ]; // 灰度映射
  int lGrayNum[ 256 ]; // 灰度映射

  for (i = 0; i < 256; i ++) { // 置0
    lGrayNum[ i ] = 0;
  }

  for (i = 0; i < h; i ++) { // 各灰度值计数
    for (j = 0; j < w; j ++) {
      lpSrcUnChr = (uchar*) lpSrcStartBits + lLineBytes * i + j;
      lGrayNum[ *(lpSrcUnChr) ] ++;   // 加1
    }
  }

  for (i = 0; i < 256; i++) { // 计算灰度映射表
    int varLong;        //临时变量
    varLong = 0; // 初始为0

    for (j = 0; j <= i; j++) {
      varLong += lGrayNum[ j ];
    }

    bGrayMap[ i ] = (uchar)(varLong * 255 / h / w);      // 计算对应的新灰度值
  }

  for (i = 0; i < h; i++) { // 行
    for (j = 0; j < w; j++) { // 列
      // 指向DIB第i行，第j个像素的指针(uchar* )
      lpSrcUnChr = (uchar*) lpSrcStartBits + lLineBytes * (h - 1 - i) + j;
      *lpSrcUnChr = bGrayMap[ *lpSrcUnChr ]; // 计算新的灰度值
    }
  }

  return TRUE;
}
/*************************************************************************
* \函数名称：
*   GrayHistEnhance()
* \输入参数:
*     uchar* pBits        //指向源图像的像素指针
*     int w        //源图像的宽度
*     int h        //源图像的高度
* \返回值:
*   BOOL                   //成功则返回TRUE，否则返回FALSE
* \说明:
*   该函数对指定的图像进行直方图均衡化处理
*************************************************************************/
static BOOL GrayHistEnhance(int h, int w, uchar* pBits, int lLineBytes)
{
  uchar* pSrc;                                    //指向源图像的指针
  int nTemp;                                            //临时变量
  int j;                                                    //循环变量
  int i;                                                    //循环变量
  uchar byMap[ 256 ];                                    //累积直方图，即灰度映射表
  int nCount[ 256 ];                                        //直方图

  for (i = 0; i < 256; i ++) {                                     //重置计数为0
    nCount[ i ] = 0;                                        //清零
  }

  for (i = 0; i < h; i++) {                                 //每行
    for (j = 0; j < w; j++) {                             //每列
      // 指向DIB第i行，第j个象素的指针
      pSrc = (uchar*) pBits + w * (h - 1 - i) + j;
      nCount[ *(pSrc) ] ++;                                  //计数加1
    }
  }

  for (i = 0; i < 256; i++) {                                         //计算累积直方图
    nTemp = 0;                                        //初始为0

    for (j = 0; j <= i; j++) {
      nTemp += nCount[ j ];
    }

    byMap[ i ] = (uchar)(nTemp * 255 / h / w);          //计算对应的新灰度值
    //说明：* 255实际上就是将新的灰度值也转换为0～255的范围。
  }

  for (i = 0; i < h; i++) {
    for (j = 0; j < w; j++) {                             //每列
      //指向DIB第i行，第j个象素的指针
      pSrc = (uchar*) pBits + w * (h - 1 - i) + j;
      *pSrc = byMap[ *pSrc ];                            //计算新的灰度值
    }
  }

  return TRUE;                                            //返回
}

