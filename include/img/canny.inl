// 边缘检测

#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "filters.h"
//#include "macro.h"



#define _IABS(a) (((a) ^ ((a) < 0 ? -1 : 0)) - ((a) < 0 ? -1 : 0))

#ifndef MALLOC
#define MALLOC(TYPE, size)  (TYPE*)pmalloc(sizeof(TYPE) * (size_t)(size))
#endif

/*************************************************************************
* \输入参数:
*   double sigma               - 高斯函数的标准差
*   double **pdKernel          - 指向高斯数据数组的指针
*   int *pnWindowSize          - 数据的长度
*
* \返回值:
*   无
*
* \说明:
*   这个函数可以生成一个一维的高斯函数的数字数据，理论上高斯数据的长度应
*   该是无限长的，但是为了计算的简单和速度，实际的高斯数据只能是有限长的
*   pnWindowSize就是数据长度
*
*************************************************************************
*/
void MakeGauss(double sigma, double** pdKernel, int* pnWindowSize)
{
  // 循环控制变量
  int i;

  // 数组的中心点
  int nCenter;

  // 数组的某一点到中心点的距离
  double dDis;

  // 中间变量
  double dValue;
  double dSum;
  dSum = 0;

  // 数组长度，根据概率论的知识，选取[-3*sigma, 3*sigma]以内的数据。
  // 这些数据会覆盖绝大部分的滤波系数
  *pnWindowSize = 1 + 2 * (int) ceil(3 * sigma);

  // 中心
  nCenter = (*pnWindowSize) / 2;

  // 分配内存
  *pdKernel = MALLOC(double, *pnWindowSize);

  for (i = 0; i < (*pnWindowSize); i++) {
    dDis = (double)(i - nCenter);
    dValue = exp((-1. / 2.) * dDis * dDis / (sigma * sigma)) / (sqrt(2 * PI) * sigma);
    (*pdKernel) [ i ] = dValue;
    dSum += dValue;
  }

  // 归一化
  for (i = 0; i < (*pnWindowSize); i++) {
    (*pdKernel) [ i ] /= dSum;
  }
}

/*************************************************************************
 * \输入参数:
 *   unsigned char* pUnchImg      - 指向图象数据的指针
 *   int nWidth           - 图象数据宽度
 *   int nHeight          - 图象数据高度
 *   double dSigma        - 高斯函数的标准差
 *   unsigned char* pUnchSmthdImg - 指向经过平滑之后的图象数据
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   为了抑止噪声，采用高斯滤波对图象进行滤波，滤波先对x方向进行，然后对
 *   y方向进行。
 *
 *************************************************************************
 */
void GaussianSmooth(const unsigned char* pUnchImg, int nWidth, int nHeight,
    double sigma, unsigned char* pUnchSmthdImg)
{
  // 循环控制变量
  int y;
  int x;

  int i;

  // 高斯滤波器的数组长度
  int nWindowSize;

  //  窗口长度的1/2
  int nHalfLen;

  // 一维高斯数据滤波器
  double* pdKernel;

  // 高斯系数与图象数据的点乘
  double dDotMul;

  // 高斯滤波系数的总和
  double dWeightSum;

  // 中间变量
  double* pdTmp;

  // 分配内存
  pdTmp = MALLOC(double, nWidth * nHeight);
  assert(pdTmp);

  // 产生一维高斯数据滤波器
  MakeGauss(sigma, &pdKernel, &nWindowSize);

  // MakeGauss返回窗口的长度，利用此变量计算窗口的半长
  nHalfLen = nWindowSize / 2;

  // x方向进行滤波
  for (y = 0; y < nHeight; y++) {
    for (x = 0; x < nWidth; x++) {
      dDotMul = 0;
      dWeightSum = 0;

      for (i = (-nHalfLen); i <= nHalfLen; i++) {
        // 判断是否在图象内部
        if ((i + x) >= 0 && (i + x) < nWidth) {
          dDotMul += (double) pUnchImg[ y * nWidth + (i + x) ] * pdKernel[ nHalfLen + i ];
          dWeightSum += pdKernel[ nHalfLen + i ];
        }
      }

      pdTmp[ y * nWidth + x ] = dDotMul / dWeightSum;
    }
  }

  // y方向进行滤波
  for (x = 0; x < nWidth; x++) {
    for (y = 0; y < nHeight; y++) {
      dDotMul = 0;
      dWeightSum = 0;

      for (i = (-nHalfLen); i <= nHalfLen; i++) {
        // 判断是否在图象内部
        if ((i + y) >= 0 && (i + y) < nHeight) {
          dDotMul += (double) pdTmp[(y + i) * nWidth + x ] * pdKernel[ nHalfLen + i ];
          dWeightSum += pdKernel[ nHalfLen + i ];
        }
      }

      pUnchSmthdImg[ y * nWidth + x ] = (unsigned char)(int)(dDotMul / dWeightSum);
    }
  }

  // 释放内存
  pfree(pdKernel);
  pdKernel = NULL;

  pfree(pdTmp);
  pdTmp = NULL;
}

/*************************************************************************
* \输入参数:
*   unsigned char* pSrc      - 经过高斯滤波后的图象
*   int nWidht       - 图象宽度
*   int nHeight      - 图象高度
*   int *pnMag       - 梯度幅度
*   int *pnGradX     - x方向的方向导数
*   int *pnGradY     - y方向的方向导数
*
* \返回值:
*   无
*
* \说明:
*   这个函数计算方向倒数梯度幅度，采用的微分算子是(-1 0 1) 和 (-1 0 1)'(转置)
*   计算的时候对边界象素采用了特殊处理
*
*************************************************************************
*/
void DirGradMagnitude(const unsigned char* pSrc, int nWidth, int nHeight, int* pnMag, int* pnGradX, int* pnGradY)
{
  // 循环控制变量
  int x, y;

  for (y = 0; y < nHeight; y++) {
    int pos = y * nWidth;
    const unsigned char* _pUnch = pSrc + pos;
    const unsigned char* _pUnchPrev = pSrc + MAX(0, y - 1) * nWidth;
    const unsigned char* _pUnchNext = pSrc + MIN(nHeight - 1, y + 1) * nWidth;
    int* _pnGradX = pnGradX + pos;
    int* _pnGradY = pnGradY + pos;
    int* _pnMag = pnMag + pos;

    for (x = 0; x < nWidth; x++) {
      int nGradX, nGradY;
      int xPrev = MAX(0, x - 1);
      int xNext = MIN(nWidth - 1, x + 1);
      // 计算x和y方向的方向导数，在边界出进行了处理，防止要访问的象素出界
#if 0

      nGradX = (int)(_pUnch[ xNext ] - _pUnch[ xPrev ]);
      nGradY = (int)(_pUnchNext[ x ] - _pUnchPrev[ x ]);
      _pnGradX[ x ] = nGradX;
      _pnGradY[ x ] = nGradY;
      _pnMag[ x ] = (int)(sqrt((double)(nGradX * nGradX + nGradY * nGradY)) + 0.5);
#else

      nGradX = (int)(_pUnch[ xNext ] - _pUnch[ xPrev ]);
      nGradX += nGradX + (_pUnchNext[ xNext ] - _pUnchNext[ xPrev ]) + (_pUnchPrev[ xNext ] - _pUnchPrev[ xPrev ]);
      nGradX >>= 2;
      nGradY = (int)(_pUnchNext[ x ] - _pUnchPrev[ x ]);
      nGradY += nGradY + (_pUnchNext[ xNext ] - _pUnchPrev[ xNext ]) + (_pUnchNext[ xPrev ] - _pUnchPrev[ xPrev ]);
      nGradY >>= 2;
      _pnGradX[ x ] = nGradX;
      _pnGradY[ x ] = nGradY;
      _pnMag[ x ] = (nGradX + nGradY);
#endif

    }
  }
}

/*************************************************************************
 * \输入参数:
 *   int *pnMag         - 梯度图
 *   int *pnGradX       - x方向的方向导数
 *   int *pnGradY       - y方向的方向导数
 *   int nWidth         - 图象数据宽度
 *   int nHeight        - 图象数据高度
 *   unsigned char* pUnchRst    - 经过NonmaxSuppress处理后的结果
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   抑止梯度图中非局部极值点的象素。
 *
 *************************************************************************
 */
void NonmaxSuppress(const int* pnMag, const int* pnGradX, const int* pnGradY, int nWidth,
    int nHeight, unsigned char* pUnchRst)
{
  // 循环控制变量
  int x, y;
  int nPos;

  // x方向梯度分量
  int gx, gy;

  // 临时变量
  int g1, g2, g3, g4;
  double weight, dTmp1, dTmp2, dTmp;

  for (y = 1; y < nHeight - 1; y++) {
    for (x = 1; x < nWidth - 1; x++) {
      nPos = y * nWidth + x;

      // 如果当前象素的梯度幅度为0，则不是边界点
      if (pnMag[ nPos ] == 0) {
        pUnchRst[ nPos ] = 1;
      }
      else {
        int nPos1 = nPos - nWidth;
        int nPos2 = nPos + nWidth;
        // 当前象素的梯度幅度
        dTmp = pnMag[ nPos ];

        // x，y方向导数
        gx = pnGradX[ nPos ];
        gy = pnGradY[ nPos ];

        // 如果方向导数y分量比x分量大，说明导数的方向更加“趋向”于y分量。
        if (abs(gy) > abs(gx)) {
          // 计算插值的比例
          weight = fabs((double) gx) / fabs((double) gy);

          g2 = pnMag[ nPos1 ];
          g4 = pnMag[ nPos2 ];

          // 如果x，y两个方向的方向导数的符号相同
          // C是当前象素，与g1-g4的位置关系为：
          // g1 g2
          //   C
          //   g4 g3
          if (gx * gy > 0) {
            g1 = pnMag[ nPos1 - 1 ];
            g3 = pnMag[ nPos2 + 1 ];
          }

          // 如果x，y两个方向的方向导数的符号相反
          // C是当前象素，与g1-g4的位置关系为：
          //    g2 g1
          //   C
          // g3 g4
          else {
            g1 = pnMag[ nPos1 + 1 ];
            g3 = pnMag[ nPos2 - 1 ];
          }
        }

        // 如果方向导数x分量比y分量大，说明导数的方向更加“趋向”于x分量
        // 这个判断语句包含了x分量和y分量相等的情况
        else {
          // 计算插值的比例
          weight = fabs((double) gy) / fabs((double) gx);

          g2 = pnMag[ nPos + 1 ];
          g4 = pnMag[ nPos - 1 ];

          // 如果x，y两个方向的方向导数的符号相同
          // C是当前象素，与g1-g4的位置关系为：
          // g3
          // g4 C g2
          //       g1
          if (gx * gy > 0) {
            g1 = pnMag[ nPos2 + 1 ];
            g3 = pnMag[ nPos1 - 1 ];
          }
          // 如果x，y两个方向的方向导数的符号相反
          // C是当前象素，与g1-g4的位置关系为：
          //      g1
          // g4 C g2
          //  g3
          else {
            g1 = pnMag[ nPos1 + 1 ];
            g3 = pnMag[ nPos2 - 1 ];
          }
        }

        // 下面利用g1-g4对梯度进行插值
        {
          dTmp1 = weight * g1 + (1 - weight) * g2;
          dTmp2 = weight * g3 + (1 - weight) * g4;

          // 当前象素的梯度是局部的最大值
          // 该点可能是个边界点
          if (dTmp >= dTmp1 && dTmp >= dTmp2) {
            pUnchRst[ nPos ] = 0;
          }
          else {
            // 不可能是边界点
            pUnchRst[ nPos ] = 1;
          }
        }
      } //else
    } // for

  }
}

/*************************************************************************
 * \输入参数:
 *   int *pnMag               - 梯度幅度图
 *  int nWidth               - 图象数据宽度
 *  int nHeight              - 图象数据高度
 *   int *pnThdHigh           - 高阈值
 *   int *pnThdLow            - 低阈值
 *  double dRatioLow         - 低阈值和高阈值之间的比例
 *  double dRatioHigh        - 高阈值占图象象素总数的比例
 *   unsigned char* pUnchEdge - 经过non-maximum处理后的数据
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   经过non-maximum处理后的数据pUnchEdge，统计pnMag的直方图，确定阈值。
 *   本函数中只是统计pUnchEdge中可能为边界点的那些象素。然后利用直方图，
 *   根据dRatioHigh设置高阈值，存储到pnThdHigh。利用dRationLow和高阈值，
 *   设置低阈值，存储到*pnThdLow。dRatioHigh是一种比例：表明梯度小于
 *   *pnThdHigh的象素数目占象素总数目的比例。dRationLow表明*pnThdHigh
 *   和*pnThdLow的比例，这个比例在canny算法的原文里，作者给出了一个区间。
 *
 *************************************************************************
 */
void EstimateThreshold(const int* pnMag, int nWidth, int nHeight, int* pnThdHigh, int* pnThdLow,
    const unsigned char* pUnchEdge, double dRatioHigh, double dRationLow)
{
  // 循环控制变量
  int x, y, k;

  // 该数组的大小和梯度值的范围有关，如果采用本程序的算法，那么梯度的范围不会超过pow(2,10)
  int nHist[ 1024 ];

  // 可能的边界数目
  int nEdgeNb;

  // 最大梯度值
  int nMaxMag;

  int nHighCount;

  nMaxMag = 0;

  // 初始化
  for (k = 0; k < 1024; k++) {
    nHist[ k ] = 0;
  }

  // 统计直方图，然后利用直方图计算阈值
  for (y = 0; y < nHeight; y++) {
    for (x = 0; x < nWidth; x++) {
      // 只是统计那些可能是边界点，并且还没有处理过的象素
      if (!pUnchEdge[ y * nWidth + x ]) {
        nHist[ pnMag[ y * nWidth + x ] ] ++;
      }
    }
  }

  nEdgeNb = nHist[ 0 ];
  nMaxMag = 0;

  // 统计经过“非最大值抑止(non-maximum suppression)”后有多少象素
  for (k = 1; k < 1024; k++) {
    if (nHist[ k ] != 0) {
      // 最大梯度值
      nMaxMag = k;
    }

    // 梯度为0的点是不可能为边界点的
    // 经过non-maximum suppression后有多少象素
    nEdgeNb += nHist[ k ];
  }

  // 梯度比高阈值*pnThdHigh小的象素点总数目
  nHighCount = (int)(dRatioHigh * nEdgeNb + 0.5);

  k = 1;
  nEdgeNb = nHist[ 1 ];

  // 计算高阈值
  while ((k < (nMaxMag - 1)) && (nEdgeNb < nHighCount)) {
    k++;
    nEdgeNb += nHist[ k ];
  }

  // 设置高阈值
  *pnThdHigh = k;

  // 设置低阈值
  *pnThdLow = (int)((*pnThdHigh) * dRationLow + 0.5);
}

/*************************************************************************
 * \输入参数:
 *   int    x         - 跟踪起点的x坐标
 *   int    y         - 跟踪起点的y坐标
 *   int nLowThd       - 判断一个点是否为边界点的低阈值
 *   unsigned char* pUnchEdge - 记录边界点的缓冲区
 *   int nWidth               - 图象数据宽度
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   递归调用
 *   从(x,y)坐标出发，进行边界点的跟踪，跟踪只考虑pUnchEdge中没有处理并且
 *   可能是边界点的那些象素(=0),象素值为1表明该点不可能是边界点，象素值
 *   为255表明该点已经被设置为边界点，不必再考虑
 *
 *************************************************************************
 */
void TraceEdge(unsigned char* edges, int step)
{
  // 对8邻域象素进行查询
  if (*edges) {
    return ;
  }

  *edges = 2;
  TraceEdge(edges + 1, step);
  TraceEdge(edges - 1, step);
  TraceEdge(edges + step + 1, step);
  TraceEdge(edges + step , step);
  TraceEdge(edges + step - 1, step);
  TraceEdge(edges - step - 1, step);
  TraceEdge(edges - step , step);
  TraceEdge(edges - step + 1, step);
}

/*************************************************************************
 * \输入参数:
 *   int *pnMag              - 梯度幅度图
 *  int nWidth               - 图象数据宽度
 *  int nHeight              - 图象数据高度
 *  double dRatioLow         - 低阈值和高阈值之间的比例
 *  double dRatioHigh        - 高阈值占图象象素总数的比例
 *   unsigned char* pUnchEdge        - 记录边界点的缓冲区
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   本函数实现类似“磁滞现象”的一个功能，也就是，先调用EstimateThreshold
 *   函数对经过non-maximum处理后的数据pUnchSpr估计一个高阈值，然后判断
 *   pUnchSpr中可能的边界象素(=0)的梯度是不是大于高阈值nThdHigh，如果比
 *   该阈值大，该点将作为一个边界的起点，调用TraceEdge函数，把对应该边界
 *   的所有象素找出来。最后，当整个搜索完毕时，如果还有象素没有被标志成
 *   边界点，那么就一定不是边界点。
 *
 *************************************************************************
 */
void Hysteresis(int* pnMag, int nWidth, int nHeight, double dRatioLow,
    double dRatioHigh, unsigned char* pUnchEdge)
{
  // 循环控制变量
  int x, y, nPos;

  int nThdHigh, nThdLow;
  // 估计TraceEdge需要的低阈值，以及Hysteresis函数使用的高阈值
  EstimateThreshold(pnMag, nWidth, nHeight, &nThdHigh,
      &nThdLow, pUnchEdge, dRatioHigh, dRatioLow);

  // 这个循环用来寻找大于nThdHigh的点，这些点被用来当作边界点，然后用
  // TraceEdge函数来跟踪该点对应的边界
  // 设置图象边缘部分为不可能的边界点
  memset(pUnchEdge, 1, nWidth);
  memset(pUnchEdge + nWidth * (nHeight - 1), 1, nWidth);

  for (y = 1; y < nHeight - 1; y++) {
    unsigned char* _pUnchEdge = pUnchEdge + y * nWidth;
    int* _pnMag = pnMag + y * nWidth;
    _pUnchEdge[ 0 ] = _pUnchEdge[ nWidth - 1 ] = 1;

    for (x = 1; x < nWidth - 1; x++) {
      nPos = y * nWidth + x;

      if (_pnMag[ x ] < nThdLow) {
        _pUnchEdge[ x ] = 1;
      }
    }
  }

  for (y = 1; y < nHeight - 1; y++) {
    unsigned char* _pUnchEdge = pUnchEdge + y * nWidth;
    int* _pnMag = pnMag + y * nWidth;

    for (x = 1; x < nWidth - 1; x++) {
      // 如果该象素是可能的边界点，并且梯度大于高阈值，该象素作为
      // 一个边界的起点
      if ((!_pUnchEdge[ x ]) && (_pnMag[ x ] >= nThdHigh)) {
        // 设置该点为边界点
        TraceEdge(_pUnchEdge + x, nWidth);
      }
    }
  }

  // 那些还没有被设置为边界点的象素已经不可能成为边界点
  for (y = 0; y < nHeight; y++) {
    unsigned char* _pUnchEdge = pUnchEdge + y * nWidth;

    for (x = 0; x < nWidth; x++) {
      // 1设置为非边界点, 2设置为边界点
      _pUnchEdge[ x ] = (unsigned char) - (_pUnchEdge[ x ] >> 1);
    }
  }
}

/*************************************************************************
* \输入参数:
*   unsigned char* pUnchImage- 图象数据
*  int nWidth               - 图象数据宽度
*  int nHeight              - 图象数据高度
*  double sigma             - 高斯滤波的标准方差
*  double dRatioLow         - 低阈值和高阈值之间的比例 0.4
*  double dRatioHigh        - 高阈值占图象象素总数的比例 0.79
*  unsigned char* pUnchEdge - canny算子计算后的分割图
*
* \返回值:
*   无
*
* \说明:
*   canny分割算子，计算的结果保存在pUnchEdge中，逻辑1(255)表示该点为
*   边界点，逻辑0(0)表示该点为非边界点。该函数的参数sigma，dRatioLow
*   dRatioHigh，是需要指定的。这些参数会影响分割后边界点数目的多少
*************************************************************************
*/
void Canny(int nHeight, int nWidth, const unsigned char* pUnchImage, unsigned char* pUnchEdge,
    double sigma, double dRatioLow, double dRatioHigh)
{
  // 经过高斯滤波后的图象数据
  unsigned char* pUnchSmooth;

  // 指向x方向导数的指针
  int* pnGradX;

  // 指向y方向导数的指针
  int* pnGradY;

  // 梯度的幅度
  int* pnGradMag;
  int bufsize = nWidth * nHeight;

  pUnchSmooth = MALLOC(unsigned char, bufsize);
  pnGradMag = MALLOC(int, 3 * bufsize);
  pnGradX = pnGradMag + bufsize;
  pnGradY = pnGradMag + 2 * bufsize;

  // 对原图象进行滤波
  GaussianSmooth(pUnchImage, nWidth, nHeight, sigma, pUnchSmooth);

  // 计算方向导数，梯度的幅度
  DirGradMagnitude(pUnchImage, nWidth, nHeight, pnGradMag, pnGradX, pnGradY);

  // 应用 non-maximum 抑制
  NonmaxSuppress(pnGradMag, pnGradX, pnGradY, nWidth, nHeight, pUnchEdge);

  // 应用Hysteresis，找到所有的边界
  Hysteresis(pnGradMag, nWidth, nHeight, dRatioLow, dRatioHigh, pUnchEdge);

  // 释放内存
  pfree(pnGradMag);
  pnGradMag = NULL;
  pfree(pUnchSmooth);
  pUnchSmooth = NULL;
}
