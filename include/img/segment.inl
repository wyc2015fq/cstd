
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "macro.h"
#include "imglib.h"

/*************************************************************************
* \函数名称：
*   RegionSegFixThreshold()
 *
* \输入参数:
*   int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes     - 指向CDib类的指针，含有原始图象信息
*   int nThreshold     - 区域分割的阈值
 *
* \返回值:
*   无
 *
* \说明:
*   1（逻辑）表示对应象素为前景区域，0表示背景阈值分割的关键问题
*   在于阈值的选取。阈值的选取一般应该视实际的应用而
*   灵活设定。
 **************************************************************************/
void RegionSegFixThreshold(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nThreshold)
{
  int y; //遍历图象的纵坐标
  int x; //遍历图象的横坐标

  for (y = 0; y < lHeight; y++) {
    for (x = 0; x < lWidth; x++) {
      if (*(lpDIBBits + y * lLineBytes + x) < nThreshold) {
        * (lpDIBBits + y * lLineBytes + x) = 0;
      }
    }
  }
}

/*************************************************************************
* \函数名称：
*   RegionSegAdaptive()
 *
* \输入参数:
*   int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes     - 指向CDib类的指针，含有原始图象信息
 *
* \返回值:
*   无
 *
* \说明:
*   1（逻辑）表示对应象素为前景区域，0表示背景
*   阈值分割的关键问题在于阈值的选取。阈值的选取一般应该视实际的应用而
*   灵活设定。本函数中，阈值不是固定的，而是根据图象象素的实际性质而设定的。
*   这个函数把图像分成四个子图象，然后计算每个子图象的均值，根据均值设置阈值
*   阈值只是应用在对应的子图象
 **************************************************************************/
void RegionSegAdaptive(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  int y; //遍历图象的纵坐标
  int x; //遍历图象的横坐标

  // 局部阈值
  int nThd[ 2 ][ 2 ];

  // 子图象的平均值
  int nLocAvg;

  // 对左上图像逐点扫描：
  nLocAvg = 0;

  // y方向
  for (y = 0; y < lHeight / 2; y++) {
    // x方向
    for (x = 0; x < lWidth / 2; x++) {
      nLocAvg += lpDIBBits[ y * lLineBytes + x ];
    }
  }

  // 计算均值
  nLocAvg /= ((lHeight / 2) * (lWidth / 2));

  // 设置阈值为子图象的平均值
  nThd[ 0 ][ 0 ] = nLocAvg;

  // 对左上图像逐点扫描进行分割：
  // y方向
  for (y = 0; y < lHeight / 2; y++) {
    // x方向
    for (x = 0; x < lWidth / 2; x++) {
      if (lpDIBBits[ y * lLineBytes + x ] < nThd[ 0 ][ 0 ]) {
        lpDIBBits[ y * lLineBytes + x ] = 255;
      }
      else {
        lpDIBBits[ y * lLineBytes + x ] = 0;
      }

    }
  }

  // =============================================
  // 对左下图像逐点扫描：
  nLocAvg = 0;

  // y方向
  for (y = lHeight / 2; y < lHeight; y++) {
    // x方向
    for (x = 0; x < lWidth / 2; x++) {
      nLocAvg += lpDIBBits[ y * lLineBytes + x ];
    }
  }

  // 计算均值
  nLocAvg /= ((lHeight - lHeight / 2) * (lWidth / 2));

  // 设置阈值为子图象的平均值
  nThd[ 1 ][ 0 ] = nLocAvg;

  // 对左下图像逐点扫描进行分割：
  // y方向
  for (y = lHeight / 2; y < lHeight; y++) {
    // x方向
    for (x = 0; x < lWidth / 2; x++) {
      if (lpDIBBits[ y * lLineBytes + x ] < nThd[ 1 ][ 0 ]) {
        lpDIBBits[ y * lLineBytes + x ] = 255;
      }
      else {
        lpDIBBits[ y * lLineBytes + x ] = 0;
      }

    }
  }

  // =============================================
  // 对右上图像逐点扫描：
  nLocAvg = 0;

  // y方向
  for (y = 0; y < lHeight / 2; y++) {
    // x方向
    for (x = lWidth / 2; x < lWidth; x++) {
      nLocAvg += lpDIBBits[ y * lLineBytes + x ];
    }
  }

  // 计算均值
  nLocAvg /= ((lHeight / 2) * (lWidth - lWidth / 2));

  // 设置阈值为子图象的平均值
  nThd[ 0 ][ 1 ] = nLocAvg;

  // 对右上图像逐点扫描进行分割：
  // y方向
  for (y = 0; y < lHeight / 2; y++) {
    // x方向
    for (x = lWidth / 2; x < lWidth; x++) {
      if (lpDIBBits[ y * lLineBytes + x ] < nThd[ 0 ][ 1 ]) {
        lpDIBBits[ y * lLineBytes + x ] = 255;
      }
      else {
        lpDIBBits[ y * lLineBytes + x ] = 0;
      }

    }
  }

  // =============================================
  // 对右下图像逐点扫描：
  nLocAvg = 0;

  // y方向
  for (y = lHeight / 2; y < lHeight; y++) {
    // x方向
    for (x = lWidth / 2; x < lWidth; x++) {
      nLocAvg += lpDIBBits[ y * lLineBytes + x ];
    }
  }

  // 计算均值
  nLocAvg /= ((lHeight - lHeight / 2) * (lWidth - lWidth / 2));

  // 设置阈值为子图象的平均值
  nThd[ 1 ][ 1 ] = nLocAvg;

  // 对右下图像逐点扫描进行分割：
  // y方向
  for (y = lHeight / 2; y < lHeight; y++) {
    // x方向
    for (x = lWidth / 2; x < lWidth; x++) {
      if (lpDIBBits[ y * lLineBytes + x ] < nThd[ 1 ][ 1 ]) {
        lpDIBBits[ y * lLineBytes + x ] = 255;
      }
      else {
        lpDIBBits[ y * lLineBytes + x ] = 0;
      }
    }
  }

  // 为了显示方便显示，逻辑1用黑色显示，逻辑0用白色显示
  for (y = 0; y < lHeight; y++) {
    // x方向
    for (x = 0; x < lWidth; x++) {
      lpDIBBits[ y * lLineBytes + x ] = 255 - lpDIBBits[ y * lLineBytes + x ];
    }
  }
}

/*************************************************************************
 *
* \函数名称：
*   Roberts()
 *
* \输入参数:
*   int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes  - 指向CDib类的指针，含有原始图象信息
*   Float* pdGrad - 指向梯度数据的指针，含有图像的梯度信息
 *
* \返回值:
*   无
 *
* \说明:
*   Roberts算子
 *
 *************************************************************************
 */
void Roberts(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, Float* pdGrad)
{
  // 遍历图象的纵坐标
  int y;

  // 遍历图象的横坐标
  int x;

  //这两个变量用来表示Roberts算子第一个模板的两个象素值
  int nUpLeft;
  int nDownRight;

  // 这两个变量用来表示Roberts算子第二个模板的两个象素值
  int nUpRight;
  int nDownLeft;

  // 这两个变量用来表示Roberts算子计算的结果
  int nValueOne;
  int nValueTwo;

  // 临时变量
  Float dGrad;

  // 初始化
  for (y = 0; y < lHeight; y++) {
    for (x = 0; x < lWidth; x++) {
      *(pdGrad + y * lWidth + x) = 0;
    }
  }

  // 下面开始利用Roberts算子进行计算，为了保证计算所需要的
  // 的数据位于图像数据的内部，下面的两重循环的条件是
  // y<lHeight-1 而不是y<lHeight，相应的x方向也是x<lWidth-1
  // 而不是x<lWidth

  for (y = 0; y < lHeight - 1; y++)
    for (x = 0; x < lWidth - 1; x++) {
      // Roberts算子第一个模板需要的象素值
      nUpLeft = *(lpDIBBits + y * lLineBytes + x);
      nDownRight = *(lpDIBBits + (y + 1) * lLineBytes + x + 1);
      nDownRight *= -1;

      //Roberts算子的第一个模板计算结果
      nValueOne = nUpLeft + nDownRight;

      // Roberts算子第二个模板需要的象素值
      nUpRight = *(lpDIBBits + y * lLineBytes + x + 1);
      nDownLeft = *(lpDIBBits + (y + 1) * lLineBytes + x);
      nDownLeft *= -1;

      // Roberts算子的第二个模板计算结果
      nValueTwo = nUpRight + nDownLeft;

      // 计算两个偏导数的平方和
      dGrad = nValueOne * nValueOne + nValueTwo * nValueTwo;

      // 开方
      dGrad = pow(dGrad, 0.5);

      // 范数采用欧式距离
      *(pdGrad + y * lWidth + x) = dGrad;
    }
}
/*************************************************************************
 *
* \函数名称：
*   Laplacian1()
 *
* \输入参数:
*   int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes  - 指向CDib类的指针，含有原始图象信息
*   Float* pdGrad - 指向梯度数据的指针，含有图像的梯度信息
 *
* \返回值:
*   无
 *
* \说明:
*   Laplacian算子, 是二阶算子，不想Roberts算子那样需要两个模板计算
*   梯度，Laplacian算子只要一个算子就可以计算梯度。但是因为利用了
*   二阶信息，对噪声比较敏感
 *
 *************************************************************************
 */
void Laplacian1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, Float* pdGrad)
{
  int y; // 遍历图象的纵坐标
  int x; // 遍历图象的横坐标

  // 临时变量
  Float dGrad;

  // 模板循环控制变量
  int yy;
  int xx;

  //这个变量用来表示Laplacian算子象素值
  int nTmp[ 3 ][ 3 ];

  // 设置模板系数
  static int nWeight[ 3 ][ 3 ];

  // 初始化
  for (y = 0; y < lHeight; y++)
    for (x = 0; x < lWidth; x++) {
      *(pdGrad + y * lWidth + x) = 0;
    }

  nWeight[ 0 ][ 0 ] = -1;
  nWeight[ 0 ][ 1 ] = -1;
  nWeight[ 0 ][ 2 ] = -1;
  nWeight[ 1 ][ 0 ] = -1;
  nWeight[ 1 ][ 1 ] = 8;
  nWeight[ 1 ][ 2 ] = -1;
  nWeight[ 2 ][ 0 ] = -1;
  nWeight[ 2 ][ 1 ] = -1;
  nWeight[ 2 ][ 2 ] = -1;


  // 下面开始利用Laplacian算子进行计算，为了保证计算所需要的
  // 的数据位于图像数据的内部，下面的两重循环的条件是
  // y<lHeight-2 而不是y<lHeight，相应的x方向也是x<lWidth-2
  // 而不是x<lWidth
  for (y = 1; y < lHeight - 2; y++)
    for (x = 1; x < lWidth - 2; x++) {
      dGrad = 0;
      // Laplacian算子需要的各点象素值

      // 模板第一行
      nTmp[ 0 ][ 0 ] = lpDIBBits[(y - 1) * lLineBytes + x - 1 ];
      nTmp[ 0 ][ 1 ] = lpDIBBits[(y - 1) * lLineBytes + x ];
      nTmp[ 0 ][ 2 ] = lpDIBBits[(y - 1) * lLineBytes + x + 1 ];

      // 模板第二行
      nTmp[ 1 ][ 0 ] = lpDIBBits[ y * lLineBytes + x - 1 ];
      nTmp[ 1 ][ 1 ] = lpDIBBits[ y * lLineBytes + x ];
      nTmp[ 1 ][ 2 ] = lpDIBBits[ y * lLineBytes + x + 1 ];

      // 模板第三行
      nTmp[ 2 ][ 0 ] = lpDIBBits[(y + 1) * lLineBytes + x - 1 ];
      nTmp[ 2 ][ 1 ] = lpDIBBits[(y + 1) * lLineBytes + x ];
      nTmp[ 2 ][ 2 ] = lpDIBBits[(y + 1) * lLineBytes + x + 1 ];

      // 计算梯度
      for (yy = 0; yy < 3; yy++)
        for (xx = 0; xx < 3; xx++) {
          dGrad += nTmp[ yy ][ xx ] * nWeight[ yy ][ xx ];
        }

      // 梯度值写入内存
      *(pdGrad + y * lWidth + x) = dGrad;
    }

}

/*************************************************************************
 *
 * \函数名称：
 *   RegionGrow()
 *
 * \输入参数:
 *   int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes     - 指向CDib类的指针，含有原始图象信息
 *   LPBYTE pUnRegion  - 指向区域生长结果的指针
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   pUnRegion指针指向的数据区存储了区域生长的结果，其中1（逻辑）表示
 *  对应象素为生长区域，0表示为非生长区域
 *   区域生长一般包含三个比较重要的问题：
 *  1. 种子点的选取
 *  2. 生长准则
 *  3. 终止条件
 *  可以认为，这三个问题需要具体分析，而且每个问题解决的好坏直接关系到
 *  区域生长的结果。
 *  本函数的种子点选取为图像的中心，生长准则是相邻象素的象素值小于
 *  nThreshold, 终止条件是一直进行到再没有满足生长准则需要的象素时为止
 **************************************************************************/
void RegionGrow(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, LPBYTE pUnRegion, int nThreshold)
{
  static int nDx[] = { -1, 0, 1, 0};
  static int nDy[] = {0, 1, 0, -1};

  // 定义堆栈，存储坐标
  int* pnGrowQueX;
  int* pnGrowQueY;

  // 种子点
  int nSeedX, nSeedY;

  // 定义堆栈的起点和终点
  // 当nStart=nEnd, 表示堆栈中只有一个点
  int nStart;
  int nEnd;

  // 遍历图象的纵坐标
  // int y;

  // 遍历图象的横坐标
  // int x;

  // 当前正在处理的象素
  int nCurrX;
  int nCurrY;

  // 循环控制变量
  int k;

  // 图象的横纵坐标, 用来对当前象素的4邻域进行遍历
  int xx;
  int yy;

  // 初始化
  memset(pUnRegion, 0, sizeof(BYTE) * lWidth * lHeight);


  // 设置种子点为图像的中心
  nSeedX = lWidth / 2;
  nSeedY = lHeight / 2;
  // 分配空间
  pnGrowQueX = MALLOC(int, lWidth * lHeight);
  pnGrowQueY = MALLOC(int, lWidth * lHeight);

  //初始化
  nStart = 0;
  nEnd = 0;

  // 把种子点的坐标压入栈
  pnGrowQueX[ nEnd ] = nSeedX;
  pnGrowQueY[ nEnd ] = nSeedY;

  while (nStart <= nEnd) {
    // 当前种子点的坐标
    nCurrX = pnGrowQueX[ nStart ];
    nCurrY = pnGrowQueY[ nStart ];

    // 对当前点的4邻域进行遍历
    for (k = 0; k < 4; k++) {
      // 4邻域象素的坐标
      xx = nCurrX + nDx[ k ];
      yy = nCurrY + nDy[ k ];

      // 判断象素(xx，yy) 是否在图像内部
      // 判断象素(xx，yy) 是否已经处理过
      // pUnRegion[yy*lWidth+xx]==0 表示还没有处理

      // 生长条件：判断象素(xx，yy)和当前象素(nCurrX, nCurrY) 象素值差的绝对值
      if ((xx < lWidth) && (xx >= 0) && (yy < lHeight) && (yy >= 0)
          && (pUnRegion[ yy * lWidth + xx ] == 0)
          && abs(lpDIBBits[ yy * lLineBytes + xx ] - lpDIBBits[ nCurrY * lLineBytes + nCurrX ]) < nThreshold) {
        // 堆栈的尾部指针后移一位
        nEnd++;

        // 象素(xx，yy) 压入栈
        pnGrowQueX[ nEnd ] = xx;
        pnGrowQueY[ nEnd ] = yy;

        // 把象素(xx，yy)设置成逻辑1（255）
        // 同时也表明该象素处理过
        pUnRegion[ yy * lWidth + xx ] = 255;
      }
    }

    nStart++;
  }

  // 释放内存
  SAFEFREE(pnGrowQueX);
  SAFEFREE(pnGrowQueY);
}

void Dft2D(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, Float* pTrRstRpart, Float* pTrRstIpart)
{
  //遍历图象的纵坐标
  int y;

  //遍历图象的横坐标
  int x;

  //频域的横坐标
  int m;

  //频域的纵坐标
  int n;

  int nPxValue;
  Float fCosTable;
  Float fSinTable;
  Float fTmpRstR;
  Float fTmpRstI;

  //初始化结果数据
  for (n = 0; n < lHeight; n++) {
    for (m = 0; m < lWidth; m++) {
      *(pTrRstRpart + n * lWidth + m) = 0;
      *(pTrRstIpart + n * lWidth + m) = 0;
    }
  }

  fCosTable = 0;
  nPxValue = 0;

  for (n = 0; n < lHeight; n++)
    for (m = 0; m < lWidth; m++) {
      fTmpRstR = 0;
      fTmpRstI = 0;

      for (y = 0; y < lHeight; y++)
        for (x = 0; x < lWidth; x++) {
          fCosTable = cos(2 * PI * (((Float) m * x) / lWidth + ((Float) n * y) / lHeight));
          fSinTable = sin(-2 * PI * (((Float) m * x) / lWidth + ((Float) n * y) / lHeight));
          nPxValue = *(lpDIBBits + y * lLineBytes + x);

          fTmpRstR += fCosTable * nPxValue;
          fTmpRstI += fSinTable * nPxValue;
        }

      *(pTrRstRpart + lWidth * n + m) = fTmpRstR;
      *(pTrRstIpart + lWidth * n + m) = fTmpRstI;
    }
}

/*************************************************************************
 *
* \函数名称：
*   Sobel1()
 *
* \输入参数:
*   int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes    - 指向CDib类的指针，含有原始图象信息
*   Float* pdGrad - 指向梯度数据的指针，含有图像的梯度信息
 *
* \返回值:
*   无
 *
* \说明:
*   Sobe算子
 *
*   并行边界分割
 *
 *************************************************************************
 */
void Sobel1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, Float* pdGrad)
{
  // 遍历图象的纵坐标
  int y;

  // 遍历图象的横坐标
  int x;

  // 设置模板系数
  static int nWeight[ 2 ][ 3 ][ 3 ];

  //这个变量用来表示Laplacian算子象素值
  int nTmp[ 3 ][ 3 ];

  // 临时变量
  Float dGrad;
  Float dGradOne;
  Float dGradTwo;

  // 模板循环控制变量
  int yy;
  int xx;


  // 初始化
  for (y = 0; y < lHeight; y++) {
    for (x = 0; x < lWidth; x++) {
      *(pdGrad + y * lWidth + x) = 0;
    }
  }

  nWeight[ 0 ][ 0 ][ 0 ] = -1;
  nWeight[ 0 ][ 0 ][ 1 ] = 0;
  nWeight[ 0 ][ 0 ][ 2 ] = 1;
  nWeight[ 0 ][ 1 ][ 0 ] = -2;
  nWeight[ 0 ][ 1 ][ 1 ] = 0;
  nWeight[ 0 ][ 1 ][ 2 ] = 2;
  nWeight[ 0 ][ 2 ][ 0 ] = -1;
  nWeight[ 0 ][ 2 ][ 1 ] = 0;
  nWeight[ 0 ][ 2 ][ 2 ] = 1;

  nWeight[ 1 ][ 0 ][ 0 ] = 1;
  nWeight[ 1 ][ 0 ][ 1 ] = 2;
  nWeight[ 1 ][ 0 ][ 2 ] = 1;
  nWeight[ 1 ][ 1 ][ 0 ] = 0;
  nWeight[ 1 ][ 1 ][ 1 ] = 0;
  nWeight[ 1 ][ 1 ][ 2 ] = 0;
  nWeight[ 1 ][ 2 ][ 0 ] = -1;
  nWeight[ 1 ][ 2 ][ 1 ] = -2;
  nWeight[ 1 ][ 2 ][ 2 ] = -1;

  // 下面开始利用Prewitt算子进行计算，为了保证计算所需要的
  // 的数据位于图像数据的内部，下面的两重循环的条件是
  // y<lHeight-1 而不是y<lHeight，相应的x方向也是x<lWidth-1
  // 而不是x<lWidth
  for (y = 1; y < lHeight - 1; y++) {
    for (x = 1; x < lWidth - 1; x++) {
      dGrad = 0;
      dGradOne = 0;
      dGradTwo = 0;
      // Laplacian算子需要的各点象素值

      // 模板第一行
      nTmp[ 0 ][ 0 ] = lpDIBBits[(y - 1) * lLineBytes + x - 1 ];
      nTmp[ 0 ][ 1 ] = lpDIBBits[(y - 1) * lLineBytes + x ];
      nTmp[ 0 ][ 2 ] = lpDIBBits[(y - 1) * lLineBytes + x + 1 ];

      // 模板第二行
      nTmp[ 1 ][ 0 ] = lpDIBBits[ y * lLineBytes + x - 1 ];
      nTmp[ 1 ][ 1 ] = lpDIBBits[ y * lLineBytes + x ];
      nTmp[ 1 ][ 2 ] = lpDIBBits[ y * lLineBytes + x + 1 ];

      // 模板第三行
      nTmp[ 2 ][ 0 ] = lpDIBBits[(y + 1) * lLineBytes + x - 1 ];
      nTmp[ 2 ][ 1 ] = lpDIBBits[(y + 1) * lLineBytes + x ];
      nTmp[ 2 ][ 2 ] = lpDIBBits[(y + 1) * lLineBytes + x + 1 ];

      // 计算梯度
      for (yy = 0; yy < 3; yy++)
        for (xx = 0; xx < 3; xx++) {
          dGradOne += nTmp[ yy ][ xx ] * nWeight[ 0 ][ yy ][ xx ];
          dGradTwo += nTmp[ yy ][ xx ] * nWeight[ 1 ][ yy ][ xx ];
        }

      dGrad = dGradOne * dGradOne + dGradTwo * dGradTwo;
      dGrad = sqrt(dGrad);
      // 梯度值写入内存
      *(pdGrad + y * lWidth + x) = dGrad;
    }
  }
}

/*************************************************************************
 *
* \函数名称：
*   Prewitt1()
 *
* \输入参数:
*   int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes    - 指向CDib类的指针，含有原始图象信息
*   Float* pdGrad - 指向梯度数据的指针，含有图像的梯度信息
 *
* \返回值:
*   无
 *
* \说明:
*   Prewitt算子
 *
*   并行边界分割
 *
 *************************************************************************
 */
void Prewitt1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, Float* pdGrad)
{
  // 遍历图象的纵坐标
  int y;

  // 遍历图象的横坐标
  int x;

  // 设置模板系数
  static int nWeight[ 2 ][ 3 ][ 3 ];

  //这个变量用来表示Laplacian算子象素值
  int nTmp[ 3 ][ 3 ];

  // 临时变量
  Float dGrad;
  Float dGradOne;
  Float dGradTwo;

  // 模板循环控制变量
  int yy;
  int xx;


  // 初始化
  for (y = 0; y < lHeight; y++) {
    for (x = 0; x < lWidth; x++) {
      *(pdGrad + y * lWidth + x) = 0;
    }
  }

  nWeight[ 0 ][ 0 ][ 0 ] = -1;
  nWeight[ 0 ][ 0 ][ 1 ] = 0;
  nWeight[ 0 ][ 0 ][ 2 ] = 1;
  nWeight[ 0 ][ 1 ][ 0 ] = -1;
  nWeight[ 0 ][ 1 ][ 1 ] = 0;
  nWeight[ 0 ][ 1 ][ 2 ] = 1;
  nWeight[ 0 ][ 2 ][ 0 ] = -1;
  nWeight[ 0 ][ 2 ][ 1 ] = 0;
  nWeight[ 0 ][ 2 ][ 2 ] = 1;

  nWeight[ 1 ][ 0 ][ 0 ] = 1;
  nWeight[ 1 ][ 0 ][ 1 ] = 1;
  nWeight[ 1 ][ 0 ][ 2 ] = 1;
  nWeight[ 1 ][ 1 ][ 0 ] = 0;
  nWeight[ 1 ][ 1 ][ 1 ] = 0;
  nWeight[ 1 ][ 1 ][ 2 ] = 0;
  nWeight[ 1 ][ 2 ][ 0 ] = -1;
  nWeight[ 1 ][ 2 ][ 1 ] = -1;
  nWeight[ 1 ][ 2 ][ 2 ] = -1;

  // 下面开始利用Prewitt算子进行计算，为了保证计算所需要的
  // 的数据位于图像数据的内部，下面的两重循环的条件是
  // y<lHeight-1 而不是y<lHeight，相应的x方向也是x<lWidth-1
  // 而不是x<lWidth
  for (y = 1; y < lHeight - 1; y++)
    for (x = 1; x < lWidth - 1; x++) {
      dGrad = 0;
      dGradOne = 0;
      dGradTwo = 0;
      // Laplacian算子需要的各点象素值

      // 模板第一行
      nTmp[ 0 ][ 0 ] = lpDIBBits[(y - 1) * lLineBytes + x - 1 ];
      nTmp[ 0 ][ 1 ] = lpDIBBits[(y - 1) * lLineBytes + x ];
      nTmp[ 0 ][ 2 ] = lpDIBBits[(y - 1) * lLineBytes + x + 1 ];

      // 模板第二行
      nTmp[ 1 ][ 0 ] = lpDIBBits[ y * lLineBytes + x - 1 ];
      nTmp[ 1 ][ 1 ] = lpDIBBits[ y * lLineBytes + x ];
      nTmp[ 1 ][ 2 ] = lpDIBBits[ y * lLineBytes + x + 1 ];

      // 模板第三行
      nTmp[ 2 ][ 0 ] = lpDIBBits[(y + 1) * lLineBytes + x - 1 ];
      nTmp[ 2 ][ 1 ] = lpDIBBits[(y + 1) * lLineBytes + x ];
      nTmp[ 2 ][ 2 ] = lpDIBBits[(y + 1) * lLineBytes + x + 1 ];

      // 计算梯度
      for (yy = 0; yy < 3; yy++)
        for (xx = 0; xx < 3; xx++) {
          dGradOne += nTmp[ yy ][ xx ] * nWeight[ 0 ][ yy ][ xx ];
          dGradTwo += nTmp[ yy ][ xx ] * nWeight[ 1 ][ yy ][ xx ];
        }

      dGrad = dGradOne * dGradOne + dGradTwo * dGradTwo;
      dGrad = sqrt(dGrad);
      // 梯度值写入内存
      *(pdGrad + y * lWidth + x) = dGrad;
    }
}

/*************************************************************************
 *
* \函数名称：
*   EdgeTrack()
 *
* \输入参数:
*   int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes     - 指向CDib类的指针，含有原始图象信息
*   LPBYTE pUnEdgeTrack - 指向边界跟踪结果的指针
 *
* \返回值:
*   无
 *
* \说明:
*   pUnEdgeTrack指针指向的数据区存储了边界跟踪的结果，其中1（逻辑）表示
*  对应象素为边界点，0表示为非边界点
 *
*   串行边界分割
 *
 *************************************************************************
 */
void EdgeTrack(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, LPBYTE pUnEdgeTrack)
{
  static int nDx[ 8 ] = { -1, -1, -1, 0, 0, 1, 1, 1};
  static int nDy[ 8 ] = { -1, 0, 1, -1, 1, -1, 0, 1};

  // 遍历图象的纵坐标
  int y;

  // 遍历图象的横坐标
  int x;

  // 指向梯度数据的指针
  Float* pdGrad;

  // 定义当前象素梯度值
  Float dCurrGrad = 0;

  // 定义最大梯度值
  Float dMaxGrad;

  // 最大梯度值对应的象素点坐标
  int nPx;
  int nPy;

  // 循环变量，遍历当前象素的8邻域
  int i;

  int yy;
  int xx;

  int nDetX;
  int nDetY;

  // 按照图像的大小开辟内存空间，存储梯度计算的结果
  pdGrad = MALLOC(Float, lHeight * lWidth);

  // 调用Roberts算子求梯度
  Roberts(lHeight, lWidth, lpDIBBits, lLineBytes, pdGrad);

  // 设置初值
  dMaxGrad = 0;

  nPx = 0;
  nPy = 0;

  // 求梯度最大值所在的象素点坐标
  for (y = 0; y < lHeight; y++) {
    for (x = 0; x < lWidth; x++) {
      dCurrGrad = pdGrad[ y * lWidth + x ];

      if (dMaxGrad < dCurrGrad) {
        dMaxGrad = dCurrGrad;
        nPx = x;
        nPy = y;
      }
    }
  }

  // 初始化
  memset(pUnEdgeTrack, 0, sizeof(BYTE) * lWidth * lHeight);
  dCurrGrad = pdGrad[ nPy * lWidth + nPx ];

  // 从（nPx，nPy）点开始进行边界跟踪
  pUnEdgeTrack[ nPy * lWidth + nPx ] = 255;

  while (dCurrGrad > 10) {
    // 设置当前点为边界点
    pUnEdgeTrack[ nPy * lWidth + nPx ] = 255;

    dMaxGrad = 0;

    for (i = 0; i < 8; i++) {
      nDetX = nDx[ i ];
      nDetY = nDy[ i ];
      y = nPy + nDetY;
      x = nPx + nDetX;

      // 判断是否在图像内部
      if (x >= 0 && x < lWidth && y >= 0 && y < lHeight) {
        if ((pdGrad[ y * lWidth + x ] > dMaxGrad) && (pUnEdgeTrack[ y * lWidth + x ] == 0)) {
          dMaxGrad = pdGrad[ y * lWidth + x ];
          yy = y;
          xx = x;
        }
      }
    }

    // 下一个边界点的梯度，横纵坐标
    dCurrGrad = dMaxGrad;
    nPy = yy;
    nPx = xx;
  }

  //释放内存
  SAFEFREE(pdGrad);

}

/*************************************************************************
* \函数名称：
*   GradMagnitude()
 *
* \输入参数:
*   int *pnGradX                         - x方向的方向导数
*   int *pnGradY                         - y方向的方向导数
*   int nWidht              - 图象宽度
*   int lHeight                - 图象高度
*   int *pnMag                           - 梯度幅度
 *
* \返回值:
*   无
 *
* \说明:
*   这个函数利用方向倒数计算梯度幅度，方向倒数是DirGrad函数计算的结果
*
 **************************************************************************/
void GradMagnitude(int* pnGradX, int* pnGradY, int lWidth, int lHeight, int* pnMag)
{

  // 循环控制变量
  int y;
  int x;

  // 中间变量
  Float dSqtOne;
  Float dSqtTwo;

  for (y = 0; y < lHeight; y++) {
    for (x = 0; x < lWidth; x++) {
      dSqtOne = pnGradX[ y * lWidth + x ] * pnGradX[ y * lWidth + x ];
      dSqtTwo = pnGradY[ y * lWidth + x ] * pnGradY[ y * lWidth + x ];
      pnMag[ y * lWidth + x ] = (int)(sqrt(dSqtOne + dSqtTwo) + 0.5);
    }
  }
}

/*************************************************************************
* 函数名称：
*   RegionDivideThreshold()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
*   int threshold        - 分割门限
* 返回值:
*   BOOL               - 运算成功返回TRUE，否则返回FALSE。
* 说明:
* 该函数用于对图像进行固定阈值分割。
 ************************************************************************/
BOOL RegionDivideThreshold(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int threshold)
{
  int i;  //循环变量
  int j;    //循环变量
  // 指向源图像的指针
  LPBYTE lpSrc;
  //像素值
  BYTE pixeltemp;

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {

      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
      //取得当前指针处的像素值
      pixeltemp = (BYTE) * lpSrc;

      //目标图像中灰度值小于门限则设置为黑点
      if (pixeltemp <= threshold) {
        *lpSrc = (BYTE) 0;
      }
      else {    //否则设置为白点
        *lpSrc = (BYTE) 255;
      }
    }
  }

  return TRUE;
}

/*************************************************************************
* \函数名称：
*   RegionDivideAdaptive()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
* 返回值:
*   BOOL               - 运算成功返回TRUE，否则返回FALSE。
* 说明:
* 该函数用于对图像进行自适应阈值分割。把图像分成四个子图象，然后计算每个子图象的均值，
*  根据均值设置阈值，阈值只是应用在对应的子图像
 *************************************************************************
 */
BOOL RegionDivideAdaptive(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  int i;  //循环变量
  int j;    //循环变量
  // 指向源图像的指针
  LPBYTE lpSrc;
  //像素值
  BYTE pixeltemp;
  // 子图像灰度的平均值
  int nAvg;
  nAvg = 0; //初始化为0

  // 对左下图像逐点扫描：
  for (j = 0; j < lHeight / 2; j++) {
    for (i = 0; i < lWidth / 2; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
      //取得当前指针处的像素值
      pixeltemp = (BYTE) * lpSrc;
      //灰度的累计
      nAvg = (int) pixeltemp + nAvg;
    }
  }

  // 计算均值
  nAvg = nAvg / ((lHeight / 2) * (lWidth / 2));

  // 对左下图像逐点扫描：
  for (j = 0; j < lHeight / 2; j++) {
    for (i = 0; i < lWidth / 2; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
      //取得当前指针处的像素值
      pixeltemp = (BYTE) * lpSrc;

      //目标图像中灰度值小于门限则设置为黑点
      if (pixeltemp <= nAvg) {
        *lpSrc = (BYTE) 0;
      }
      else {    //否则设置为白点
        *lpSrc = (BYTE) 255;
      }
    }
  }

  nAvg = 0;   //初始化为0

  // 对左上图像逐点扫描：
  for (j = lHeight / 2; j < lHeight; j++) {
    for (i = 0; i < lWidth / 2; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
      //取得当前指针处的像素值
      pixeltemp = (BYTE) * lpSrc;
      //灰度的累计
      nAvg = (int) pixeltemp + nAvg;
    }
  }

  // 计算均值
  nAvg = nAvg / ((lHeight / 2) * (lWidth / 2));

  // 对左上图像逐点扫描：
  for (j = lHeight / 2; j < lHeight; j++) {
    for (i = 0; i < lWidth / 2; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
      //取得当前指针处的像素值
      pixeltemp = (BYTE) * lpSrc;

      //目标图像中灰度值小于门限则设置为黑点
      if (pixeltemp <= nAvg) {
        *lpSrc = (BYTE) 0;
      }
      else {    //否则设置为白点
        *lpSrc = (BYTE) 255;
      }
    }
  }

  nAvg = 0;   //初始化为0

  // 对右上图像逐点扫描：
  for (j = lHeight / 2; j < lHeight; j++) {
    for (i = lWidth / 2; i < lWidth; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
      //取得当前指针处的像素值
      pixeltemp = (BYTE) * lpSrc;
      //灰度的累计
      nAvg = (int) pixeltemp + nAvg;
    }
  }

  // 计算均值
  nAvg = nAvg / ((lHeight / 2) * (lWidth / 2));

  // 对右上图像逐点扫描：
  for (j = lHeight / 2; j < lHeight; j++) {
    for (i = lWidth / 2; i < lWidth; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
      //取得当前指针处的像素值
      pixeltemp = (BYTE) * lpSrc;

      //目标图像中灰度值小于门限则设置为黑点
      if (pixeltemp <= nAvg) {
        *lpSrc = (BYTE) 0;
      }
      else {    //否则设置为白点
        *lpSrc = (BYTE) 255;
      }
    }
  }

  nAvg = 0;   //初始化为0

  // 对右下图像逐点扫描：
  for (j = 0; j < lHeight / 2; j++) {
    for (i = lWidth / 2; i < lWidth; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
      //取得当前指针处的像素值
      pixeltemp = (BYTE) * lpSrc;
      //灰度的累计
      nAvg = (int) pixeltemp + nAvg;
    }
  }

  // 计算均值
  nAvg = nAvg / ((lHeight / 2) * (lWidth / 2));

  // 对右下图像逐点扫描：
  for (j = 0; j < lHeight / 2; j++) {
    for (i = lWidth / 2; i < lWidth; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
      //取得当前指针处的像素值
      pixeltemp = (BYTE) * lpSrc;

      //目标图像中灰度值小于门限则设置为黑点
      if (pixeltemp <= nAvg) {
        *lpSrc = (BYTE) 0;
      }
      else {    //否则设置为白点
        *lpSrc = (BYTE) 255;
      }
    }
  }

  return TRUE;
}


/*************************************************************************
* \函数名称：
*   RegionGrow()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
*     int nThreshold            - 区域生长门限
*     int seedx                - 区域生长种子的横坐标
*     int seedy                - 区域生长种子的纵坐标
 *     int mode                - 区域生长种子的选取类型
* 返回值:
*   BOOL               - 运算成功返回TRUE，否则返回FALSE。
* \说明:
*   本函数的处理结果是将生长后的区域表示为黑色像素，未生长区域表示为白色像素。
*   区域生长一般包含三个比较重要的问题：
*        1. 种子点的选取
*        2. 生长准则
*        3. 终止条件
*     可以认为，这三个问题需要具体分析，而且每个问题解决的好坏直接关系到
*     区域生长的结果。
 *************************************************************************/
BOOL RegionGrow1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nThreshold, int seedx, int seedy, int mode)
{
  //设置用于处理四临域的数组
  static int Dx[] = { -1, 0, 1, 0};    //X向
  static int Dy[] = {0, 1, 0, -1};    //y向
  // 临时存放图像数据的指针
  LPBYTE lpTmpBits;
  // 指向源图像的指针
  LPBYTE lpSrc;
  LPBYTE lpSrc1;
  // 指向要复制区域的指针
  LPBYTE lpDst;

  // 种子点
  IPOINT seed;

  // 定义堆栈，分别存贮X Y坐标
  int* GrowX;
  int* GrowY;

  // 定义堆栈的起点和终点
  // 当Start=End, 表示堆栈中只有一个点
  int Start;
  int End;

  // 当前正在处理的象素
  IPOINT Current;
  // 循环控制变量
  int k;
  // 图象的横纵坐标, 用来对当前象素的4邻域进行遍历
  int xx;
  int yy;

  // 分配内存
  lpTmpBits = MALLOC(BYTE, lWidth * lHeight);

  // 判断是否内存分配失败
  if (lpTmpBits == NULL) {
    // 分配内存失败
    return FALSE;
  }

  // 将目标图像所有像素初始化为白点
  lpDst = (LPBYTE) lpDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  if (mode == 0) {
    // 如果选择种子点为图像的中心
    // 设置种子点为图像的中心
    seed.x = lWidth / 2;
    seed.y = lHeight / 2;
  }
  else {   //自己设置种子点
    // 设置种子点为输入点
    seed.x = seedx;
    seed.y = seedy;
  }

  // 分配空间，最大为图像的像素数目
  GrowX = MALLOC(int, lWidth * lHeight);
  GrowY = MALLOC(int, lWidth * lHeight);

  //初始化
  Start = 0;
  End = 0;
  // 把种子点的坐标压入栈
  GrowX[ End ] = seed.x;
  GrowY[ End ] = seed.y;

  while (Start <= End) {
    // 当前种子点的坐标
    Current.x = GrowX[ Start ];
    Current.y = GrowY[ Start ];

    // 对当前点的4邻域进行遍历
    for (k = 0; k < 4; k++) {
      // 4邻域象素的坐标
      xx = Current.x + Dx[ k ];
      yy = Current.y + Dy[ k ];

      // 判断象素(xx，yy) 是否在图像内部
      // 判断象素(xx，yy) 是否已经处理过
      // *lpDst==255 表示还没有处理
      // 指向源图像(xx，yy)象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * (lHeight - yy) + xx;
      lpSrc1 = (LPBYTE) lpDIBBits + lWidth * (lHeight - Current.y) + Current.x;
      // 指向目标图像第j行，第i个象素的指针
      lpDst = (LPBYTE) lpTmpBits + lWidth * (lHeight - yy) + xx;

      // 生长条件：判断象素(xx，yy)和当前象素(nCurrX, nCurrY) 象素值差的绝对值
      if ((xx < lWidth) && (xx >= 0) && (yy < lHeight) && (yy >= 0) && (*lpDst == 255)
          && abs(*lpSrc - *lpSrc1) < nThreshold) {
        // 堆栈的尾部指针后移一位
        End++;
        // 象素(xx，yy) 压入栈
        GrowX[ End ] = xx;
        GrowY[ End ] = yy;
        // 目标图像为黑点，同时也表明该象素处理过
        *lpDst = 0;
      }
    }

    Start++;
  }

  // 复制区域生长后的图像
  memcpy(lpDIBBits, lpTmpBits, lWidth * lHeight);
  // 释放内存
  SAFEFREE(lpDIBBits);
  SAFEFREE(GrowX);
  SAFEFREE(GrowY);
  // 返回
  return TRUE;
}
