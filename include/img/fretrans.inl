
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "macro.h"
#include "imglib.h"


/**************************************************************************
*  正交变换API函数库：
*  THREECROSS()   - 将实对称矩阵化作三对角矩阵
*  BSTQ()         - 求三对角对称矩阵的特征值和特征向量
*  FFT_1D()       - 快速一维傅立叶变换
*  IFFT_1D()      - 快速一维傅立叶反变换
*  IFFT_2D()      - 快速二维傅立叶反变换
*  DCT()          - 离散余弦变换
*  IDCT()         - 离散余弦反变换
*  WALSH()        - 沃尔什－哈达玛变换
*  IWALSH()       - 沃尔什－哈达玛反变换
*  Fft2D()       - 图象的二维离散傅立叶快速变换
*  DFT_2D()       - 图象的二维离散傅立叶变换
*  HOTELLING()    - 图象的霍特林变换
*  Dct()          - 图像的离散余弦变换
*  Walsh()        - 图像的沃尔什－哈达玛变换
 *
 *************************************************************************/

// 常数π
#define PI (3.14159265358979323846264338327950288419716939937511)

//////////////////////////////////////////////////////////////////////
//  图像正交变换函数
//////////////////////////////////////////////////////////////////////

/*************************************************************************
 *
 * \函数名称：
 *   FFT_1D()
 *
 * \输入参数:
 *   COMPLEX* pCTData    - 指向时域数据的指针，输入的需要变换的数据
 *   COMPLEX* pCFData    - 指向频域数据的指针，输出的经过变换的数据
 *   nLevel                        －傅立叶变换蝶形算法的级数，2的幂数，
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   一维快速傅立叶变换。
 *
 **************************************************************************/
void FFT_1D(COMPLEX* pCTData, COMPLEX* pCFData, int nLevel)
{
  // 循环控制变量
  int i, j, k, nInter;
  // 某一级的长度
  int nBtFlyLen = 0;
  // 傅立叶变换点数
  int nCount = (int) pow(2, nLevel);

  // 变换系数的角度 ＝2* PI* i / nCount
  Float dAngle;

  // 变换需要的工作空间
  COMPLEX* pCWork1, *pCWork2, *pCW, *pCTmp;

  // 分配内存，存储傅立叶变化需要的系数表
  pCW = MALLOC(COMPLEX, nCount / 2);

  // 计算傅立叶变换的系数
  for (i = 0; i < nCount / 2; i++) {
    dAngle = -2 * PI * i / nCount;
    CplexSet(cos(dAngle), sin(dAngle), pCW[ i ]);
  }

  // 分配工作空间
  pCWork1 = MALLOC(COMPLEX, nCount);
  pCWork2 = MALLOC(COMPLEX, nCount);

  // 初始化，写入数据
  memcpy(pCWork1, pCTData, sizeof(COMPLEX) * nCount);

  nInter = 0;

  // 蝶形算法进行快速傅立叶变换
  for (k = 0; k < nLevel; k++) {
    for (j = 0; j < (int) pow(2, k); j++) {
      //计算长度
      nBtFlyLen = (int) pow(2, (nLevel - k));

      //倒序重排，加权计算
      for (i = 0; i < nBtFlyLen / 2; i++) {
        COMPLEX c;
        nInter = j * nBtFlyLen;
        CplexAdd(pCWork1[ i + nInter ], pCWork1[ i + nInter + nBtFlyLen / 2 ], pCWork2[ i + nInter ]);
        CplexSub(pCWork1[ i + nInter ], pCWork1[ i + nInter + nBtFlyLen / 2 ], c);
        CplexMul(c, pCW[(int)(i * pow(2, k)) ], pCWork2[ i + nInter + nBtFlyLen / 2 ]);
      }
    }

    // 交换 pCWork1和pCWork2的数据
    pCTmp = pCWork1;
    pCWork1 = pCWork2;
    pCWork2 = pCTmp;
  }

  // 重新排序
  for (j = 0; j < nCount; j++) {
    nInter = 0;

    for (i = 0; i < nLevel; i++) {
      if (j & (1 << i)) {
        nInter += 1 << (nLevel - i - 1);
      }
    }

    pCFData[ j ] = pCWork1[ nInter ];
  }

  // 释放内存空间
  SAFEFREE(pCW);
  SAFEFREE(pCWork1);
  SAFEFREE(pCWork2);
}

/*************************************************************************
* \函数名称：
*    IFFT_1D()
 *
* \输入参数:
*   COMPLEX* pCTData    - 指向时域数据的指针，输入的需要反变换的数据
*   COMPLEX* pCFData    - 指向频域数据的指针，输出的经过反变换的数据
*   nLevel                        －傅立叶变换蝶形算法的级数，2的幂数，
 *
* \返回值:
*   无
 *
* \说明:
*   一维快速傅立叶反变换。
 *************************************************************************/
void IFFT_1D(COMPLEX* pCFData, COMPLEX* pCTData, int nLevel)
{

  // 循环控制变量
  int i;

  // 傅立叶反变换点数
  int nCount;
  // 变换需要的工作空间
  COMPLEX* pCWork;

  // 计算傅立叶变换点数
  nCount = (int) pow(2, nLevel);


  // 分配工作空间
  pCWork = MALLOC(COMPLEX, nCount);

  // 将需要反变换的数据写入工作空间pCWork
  memcpy(pCWork, pCFData, sizeof(COMPLEX) * nCount);

  // 为了利用傅立叶正变换, 可以把傅立叶频域的数据取共轭
  // 然后直接利用正变换，输出结果就是傅立叶反变换结果的共轭
  for (i = 0; i < nCount; i++) {
    CplexSet(CREAL(pCWork[ i ]), -CIMAG(pCWork[ i ]), pCWork[ i ]);
  }

  // 调用快速傅立叶变换实现反变换，结果存储在pCTData中
  FFT_1D(pCWork, pCTData, nLevel);

  // 求时域点的共轭，求得最终结果
  // 根据傅立叶变换原理，利用这样的方法求得的结果和实际的时域数据
  // 相差一个系数nCount
  for (i = 0; i < nCount; i++) {
    CplexSet(CREAL(pCTData[ i ]) / nCount, -CIMAG(pCTData[ i ]) / nCount, pCTData[ i ]);
  }

  // 释放内存
  SAFEFREE(pCWork);
}

/*************************************************************************
 *
* \函数名称：
*   Fft2D()
 *
* \输入参数:
*   COMPLEX* pCTData    - 图像数据
*   int   lWidth                - 数据宽度
*   int   lHeight                - 数据高度
*   COMPLEX* pCFData    - 傅立叶变换后的结果
 *
* \返回值:
*   无
 *
* \说明:
*   二维傅立叶变换。
 *
 ************************************************************************
 */
void Fft2D(COMPLEX* pCTData, int lWidth, int lHeight, COMPLEX* pCFData)
{

  // 循环控制变量
  int x;
  int y;

  // 临时变量
  Float dTmpOne, dTmpTwo;

  // 进行傅立叶变换的宽度和高度，（2的整数次幂）
  // 图像的宽度和高度不一定为2的整数次幂
  int nTransWidth, nTransHeight;

  // x，y（行列）方向上的迭代次数
  int nXLev, nYLev;

  // 计算进行傅立叶变换的宽度 （2的整数次幂）
  dTmpOne = log(lWidth) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransWidth = (int) dTmpTwo;

  // 计算进行傅立叶变换的高度 （2的整数次幂）
  dTmpOne = log(lHeight) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransHeight = (int) dTmpTwo;


  // 计算x，y（行列）方向上的迭代次数
  nXLev = (int)(log(nTransWidth) / log(2) + 0.5);
  nYLev = (int)(log(nTransHeight) / log(2) + 0.5);

  for (y = 0; y < nTransHeight; y++) {
    // x方向进行快速傅立叶变换
    FFT_1D(&pCTData[ nTransWidth * y ], &pCFData[ nTransWidth * y ], nXLev);
  }

  // pCFData中目前存储了pCTData经过行变换的结果
  // 为了直接利用FFT_1D，需要把pCFData的二维数据转置，再一次利用FFT_1D进行
  // 傅立叶行变换（实际上相当于对列进行傅立叶变换）
  for (y = 0; y < nTransHeight; y++) {
    for (x = 0; x < nTransWidth; x++) {
      pCTData[ nTransHeight * x + y ] = pCFData[ nTransWidth * y + x ];
    }
  }

  for (x = 0; x < nTransWidth; x++) {
    // 对x方向进行快速傅立叶变换，实际上相当于对原来的图象数据进行列方向的
    // 傅立叶变换
    FFT_1D(&pCTData[ x * nTransHeight ], &pCFData[ x * nTransHeight ], nYLev);
  }

  // pCFData中目前存储了pCTData经过二维傅立叶变换的结果，但是为了方便列方向
  // 的傅立叶变换，对其进行了转置，现在把结果转置回来
  for (y = 0; y < nTransHeight; y++) {
    for (x = 0; x < nTransWidth; x++) {
      pCTData[ nTransWidth * y + x ] = pCFData[ nTransHeight * x + y ];
    }
  }

  memcpy(pCTData, pCFData, sizeof(COMPLEX) * nTransHeight * nTransWidth);
}

/*************************************************************************
 *
* \函数名称：
*   IFFT_2D()
 *
* \输入参数:
*   COMPLEX* pCFData - 频域数据
*   COMPLEX* pCTData - 时域数据
*   int   lWidth    - 图像数据宽度
*   int   lHeight    - 图像数据高度
 *
* \返回值:
*   无
 *
* \说明:
*   二维傅立叶反变换。
 *
 ************************************************************************
 */
void IFFT_2D(COMPLEX* pCFData, COMPLEX* pCTData, int lWidth, int lHeight)
{
  // 循环控制变量
  int x;
  int y;

  // 临时变量
  Float dTmpOne;
  Float dTmpTwo;

  // 进行傅立叶变换的宽度和高度，（2的整数次幂）
  // 图像的宽度和高度不一定为2的整数次幂
  int nTransWidth;
  int nTransHeight;

  // 分配工作需要的内存空间
  COMPLEX* pCWork, *pCTmp;

  // 计算进行傅立叶变换的宽度 （2的整数次幂）
  dTmpOne = log(lWidth) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransWidth = (int) dTmpTwo;

  // 计算进行傅立叶变换的高度 （2的整数次幂）
  dTmpOne = log(lHeight) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransHeight = (int) dTmpTwo;

  pCWork = MALLOC(COMPLEX, nTransWidth * nTransHeight);

  // 为了利用傅立叶正变换, 可以把傅立叶频域的数据取共轭
  // 然后直接利用正变换，输出结果就是傅立叶反变换结果的共轭
  for (y = 0; y < nTransHeight; y++) {
    for (x = 0; x < nTransWidth; x++) {
      pCTmp = &pCFData[ nTransWidth * y + x ];
      CplexSet(CREAL(*pCTmp) , -CIMAG(*pCTmp), pCWork[ nTransWidth * y + x ]);
    }
  }

  // 调用傅立叶正变换
  Fft2D(pCWork, lWidth, lHeight, pCTData);

  // 求时域点的共轭，求得最终结果
  // 根据傅立叶变换原理，利用这样的方法求得的结果和实际的时域数据
  // 相差一个系数
  for (y = 0; y < nTransHeight; y++) {
    for (x = 0; x < nTransWidth; x++) {
      pCTmp = &pCTData[ nTransWidth * y + x ];
      CplexSet(CREAL(*pCTmp) / (nTransWidth * nTransHeight),
          -CIMAG(*pCTmp) / (nTransWidth * nTransHeight),
          pCTData[ nTransWidth * y + x ]);
    }
  }

  SAFEFREE(pCWork);
}

/*************************************************************************
* 函数名称：FastFourierTran(COMPLEX* pTd, COMPLEX* pFd, int power)
* 函数参数:
*   COMPLEX* pTd, 指向时域数组的指针
*   COMPLEX* pFd, 指向频域数组的指针
*   int            power, 2的幂数，即迭代次数
* 函数类型:void
 函数功能:用来实现快速付立叶变换
************************************************************************/
void FastFourierTran(COMPLEX* pTd, COMPLEX* pFd, int power)
{
  int i; //行循环变量
  int j; //列循环变量

  int dotCount; // 付立叶变换点数
  int k; // 循环变量
  int bfsize, p; // 中间变量
  Float angle; // 角度
  COMPLEX* pWn, *temReg1, *temReg2, *temReg;

  dotCount = 1 << power; // 计算付立叶变换点数
  pWn = MALLOC(COMPLEX, sizeof(COMPLEX) * dotCount / 2);     // 分配运算所需存储器
  temReg1 = MALLOC(COMPLEX, sizeof(COMPLEX) * dotCount);
  temReg2 = MALLOC(COMPLEX, sizeof(COMPLEX) * dotCount);

  for (i = 0; i < dotCount / 2; i++) {   // 计算加权系数
    angle = -i * PI * 2 / dotCount;
    CplexSet(cos(angle), sin(angle), pWn[ i ]);
  }

  memcpy(temReg1, pTd, sizeof(COMPLEX) * dotCount);     // 将时域点写入temReg1

  for (k = 0; k < power; k++) {   // 采用蝶形算法进行快速付立叶变换
    for (j = 0; j < 1 << k; j++) {
      bfsize = 1 << (power - k);

      for (i = 0; i < bfsize / 2; i++) {
        COMPLEX c;
        p = j * bfsize;
        CplexAdd(temReg1[ i + p ], temReg1[ i + p + bfsize / 2 ], temReg2[ i + p ]);
        CplexSub(temReg1[ i + p ], temReg1[ i + p + bfsize / 2 ], c);
        CplexMul(c, pWn[ i * (1 << k) ], temReg2[ i + p + bfsize / 2 ]);
      }
    }

    temReg = temReg1;
    temReg1 = temReg2;
    temReg2 = temReg;
  }

  for (j = 0; j < dotCount; j++) { // 重新排序
    p = 0;

    for (i = 0; i < power; i++) {
      if (j & (1 << i)) {
        p += 1 << (power - i - 1);
      }
    }

    pFd[ j ] = temReg1[ p ];
  }

  SAFEFREE(pWn);   // 释放内存
  SAFEFREE(temReg1);
  SAFEFREE(temReg2);
}

/*************************************************************************
 *
* 函数名称：InverFastFourierTran(COMPLEX* pFd, COMPLEX* pTd, int power)
* 函数参数:
*   COMPLEX * pFd，指向频域值的指针
*   COMPLEX* pTd，指向时域值的指针
*   int power    ，2的幂数
 *函数类型:void
 *函数功能:用来实现快速付立叶反变换
 ************************************************************************/
void InverFastFourierTran(COMPLEX* pFd, COMPLEX* pTd, int power)
{
  int dotCount;            // 付立叶变换点数
  int i;                // 循环变量
  COMPLEX* pX;
  dotCount = 1 << power;        // 计算付立叶变换点数
  pX = MALLOC(COMPLEX, sizeof(COMPLEX) * dotCount);     // 分配运算所需存储器
  memcpy(pX, pFd, sizeof(COMPLEX) * dotCount);     // 将频域点写入X

  for (i = 0; i < dotCount; i++) {   // 求共轭
    CplexSet(CREAL(pX[ i ]), -CIMAG(pX[ i ]), pX[ i ]);
  }

  FastFourierTran(pX, pTd, power);   // 调用快速付立叶变换

  for (i = 0; i < dotCount; i++) {   // 求时域点的共轭
    CplexSet(CREAL(pTd[ i ]) / dotCount, -CIMAG(pTd[ i ]) / dotCount, pTd[ i ]);
  }

  SAFEFREE(pX);                  // 释放内存
}

/*************************************************************************
* 函数名称：Fourier(COMPLEX* pTd, int lWidth, int lHeight, COMPLEX* pFd)
* 函数参数:
*   COMPLEX* pTd，指向时域值的指针
*   int   lWidth，图象宽度
*   int   lHeight，图象高度
*   COMPLEX* pFd    ，指向频域值的指针
 *
* 函数类型:void
* 函数功能:二维快速傅立叶变换
 *************************************************************************/
void Fourier1(COMPLEX* pTd, int lWidth, int lHeight, COMPLEX* pFd)
{
  // 循环控制变量
  int i, j;
  // 进行傅立叶变换的宽度和高度，（2的整数次幂）
  // 图象的宽度和高度不一定为2的整数次幂
  int wid = 1;
  int hei = 1;
  int widpor = 0, heiPor = 0; //2的幂数

  while (wid * 2 <= lWidth) {   // 计算进行付立叶变换的宽度和高度（2的整数次方）
    wid *= 2;
    widpor++;
  }

  while (hei * 2 <= lHeight) {
    hei *= 2;
    heiPor++;
  }

  for (i = 0; i < hei; i++) {
    // x方向进行快速傅立叶变换
    FastFourierTran(&pTd[ wid * i ], &pFd[ wid * i ], widpor);
  }

  // pFd中目前存储了pTd经过行变换的结果
  // 为了直接利用FastFourierTran，需要把pFd的二维数据转置，再一次利用FastFourierTran进行
  // 傅立叶行变换（实际上相当于对列进行傅立叶变换）
  for (i = 0; i < hei; i++) {
    for (j = 0; j < wid; j++) {
      pTd[ hei * j + i ] = pFd[ wid * i + j ];
    }
  }

  for (j = 0; j < wid; j++) {
    // 对x方向进行快速傅立叶变换，实际上相当于对原来的图象数据进行列方向的
    // 傅立叶变换
    FastFourierTran(&pTd[ j * hei ], &pFd[ j * hei ], heiPor);
  }

  // pFd中目前存储了pTd经过二维傅立叶变换的结果，但是为了方便列方向
  // 的傅立叶变换，对其进行了转置，现在把结果转置回来
  for (i = 0; i < hei; i++) {
    for (j = 0; j < wid; j++) {
      pTd[ wid * i + j ] = pFd[ hei * j + i ];
    }
  }

  memcpy(pTd, pFd, sizeof(COMPLEX) * hei * wid);
}

/*************************************************************************
* 函数名称：
*   Fourier()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数）
*   int lHeight      - 源图像高度（象素数）
* 返回值:
*   BOOL              - 成功返回TRUE，否则返回FALSE。
* 说明:
*   该函数用来对图像进行付立叶变换。, int lLineBytes
 ************************************************************************/
BOOL Fourier(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc; // 指向源图像的指针
  Float dTemp;
  int i;    // 循环变量
  int j;
  int w;    // 进行付立叶变换的宽度（2的整数次方）
  int h;    // 进行付立叶变换的高度（2的整数次方）
  int wp;
  int hp;
  COMPLEX* TD, *FD;

  // 赋初值
  w = 1;
  h = 1;
  wp = 0;
  hp = 0;

  // 计算进行付立叶变换的宽度和高度（2的整数次方）
  while (w * 2 <= lWidth) {
    w *= 2;
    wp++;
  }

  while (h * 2 <= lHeight) {
    h *= 2;
    hp++;
  }

  TD = MALLOC(COMPLEX, w * h);
  FD = MALLOC(COMPLEX, w * h);

  for (i = 0; i < h; i++) {    // 行
    for (j = 0; j < w; j++) {   // 列
      // 指向DIB第i行，第j个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;

      // 给时域赋值
      CplexSet(*(lpSrc), 0, TD[ j + w * i ]);
    }
  }

  for (i = 0; i < h; i++) {
    // 对y方向进行快速付立叶变换
    FFT(&TD[ w * i ], &FD[ w * i ], wp);
  }

  // 保存变换结果
  for (i = 0; i < h; i++) {
    for (j = 0; j < w; j++) {
      TD[ i + h * j ] = FD[ j + w * i ];
    }
  }

  for (i = 0; i < w; i++) {
    // 对x方向进行快速付立叶变换
    FFT(&TD[ i * h ], &FD[ i * h ], hp);
  }

  for (i = 0; i < h; i++) {    // 行
    for (j = 0; j < w; j++) {   // 列
      // 计算频谱
      dTemp = sqrt(CREAL(FD[ j * h + i ]) * CREAL(FD[ j * h + i ]) +
          CIMAG(FD[ j * h + i ]) * CIMAG(FD[ j * h + i ])) / 100;

      if (dTemp > 255) {
        dTemp = 255;
      }

      // 指向DIB第(i<h/2 ? i+h/2 : i-h/2)行，第(j<w/2 ? j+w/2 : j-w/2)个
      // 象素的指针，此处不直接取i和j，是为了将变换后的原点移到中心
      // lpSrc = (LPBYTE )lpDIBBits + lLineBytes* (lHeight-1-i) + j;
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes *
          (lHeight - 1 - (i < h / 2 ? i + h / 2 : i - h / 2)) + (j < w / 2 ? j + w / 2 : j - w / 2);

      // 更新源图像
      * (lpSrc) = (BYTE)(dTemp);
    }
  }

  SAFEFREE(TD);
  SAFEFREE(FD);
  return TRUE;
}

/*************************************************************************
* 函数名称：InverseFourier(COMPLEX* pFd, COMPLEX* pTd, int lWidth, int lHeight)
* 函数参数:
*   COMPLEX* pTd，指向时域值的指针
*   COMPLEX* pFd    ，指向频域值的指针
*   int   lWidth，图象宽度
*   int   lHeight，图象高度
* 函数类型:void
* 函数功能:二维快速傅立叶反变换
 *************************************************************************/
void InverseFourier(COMPLEX* pFd, COMPLEX* pTd, int lWidth, int lHeight)
{
  // 循环控制变量
  int i, j;
  // 进行傅立叶变换的宽度和高度，（2的整数次幂）
  // 图象的宽度和高度不一定为2的整数次幂
  int wid = 1, hei = 1, widpor = 0, heiPor = 0; //2的幂数

  // 分配工作需要的内存空间
  COMPLEX* pCWork, *pCTmp;

  while (wid * 2 <= lWidth) {   // 计算进行付立叶变换的宽度和高度（2的整数次方）
    wid *= 2;
    widpor++;
  }

  while (hei * 2 <= lHeight) {
    hei *= 2;
    heiPor++;
  }

  pCWork = MALLOC(COMPLEX, sizeof(COMPLEX) * wid * hei);

  // 为了利用傅立叶正变换, 可以把傅立叶频域的数据取共轭
  // 然后直接利用正变换，输出结果就是傅立叶反变换结果的共轭
  for (i = 0; i < hei; i++) {
    for (j = 0; j < wid; j++) {
      pCTmp = &pFd[ wid * i + j ];
      CplexSet(CREAL(*pCTmp), -CIMAG(*pCTmp), pCWork[ wid * i + j ]);
      //CplexSet( pCTmp->CREAL() , -pCTmp->CIMAG() );
    }
  }

  // 调用傅立叶正变换
  Fourier1(pCWork, lWidth, lHeight, pTd);

  // 求时域点的共轭，求得最终结果
  // 根据傅立叶变换原理，利用这样的方法求得的结果和实际的时域数据
  // 相差一个系数
  for (i = 0; i < hei; i++) {
    for (j = 0; j < wid; j++) {
      pCTmp = &pTd[ wid * i + j ];
      CplexSet(CREAL(*pCTmp) / (wid * hei), -CIMAG(*pCTmp) / (wid * hei), pTd[ wid * i + j ]);
    }
  }

  SAFEFREE(pCWork);
}

/*************************************************************************
* 函数名称：DisFCosTran(Float* pTd, Float* pFd, int power)
* 函数参数:
*   Float* pTd, 指向时域值的指针
*   Float* pFd, 指向频域值的指针
*   int   power, 2的幂数
 *
* 函数类型:void
* 函数功能: 用来实现快速离散余弦变换
 ************************************************************************/
void DisFCosTran(Float* pTd, Float* pFd, int power)
{
  int i;                 //行循环变量
  int dotCount; // 离散余弦变换点数
  Float dTemp; // 临时变量
  COMPLEX* temReg;
  dotCount = 1 << power; // 计算离散余弦变换点数
  temReg = MALLOC(COMPLEX, sizeof(COMPLEX) * dotCount * 2);     // 分配内存
  memset(temReg, 0, sizeof(COMPLEX) * dotCount * 2);     // 赋为0

  // 将时域点写入数组temReg
  for (i = 0; i < dotCount; i++) {
    CplexSet(pTd[ i ], 0, temReg[ i ]);
  }

  FastFourierTran(temReg, temReg, power + 1);   // 调用快速付立叶变换
  dTemp = 1 / sqrt(dotCount);   // 调整系数
  pFd[ 0 ] = CREAL(temReg[ 0 ]) * dTemp;   // 求pFd[0]
  dTemp *= sqrt(2);

  for (i = 1; i < dotCount; i++) {   // 求pFd[u]
    pFd[ i ] = (CREAL(temReg[ i ]) * cos(i * PI / (dotCount * 2)) + CIMAG(temReg[ i ]) * sin(i * PI / (dotCount * 2))) * dTemp;
  }

  SAFEFREE(temReg);   // 释放内存
}

/*************************************************************************
* 函数名称：CosTran(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
* 函数参数:
*   LPBYTE lpDIBBits, 指向DIB起始像素的指针
*   int lWidth, DIB的宽度
*   int lHeight, int lLineBytes, DIB的高度
*     int lLineBytes, DIB的行字节数，为4的倍数
* 函数类型:BOOL
* 函数功能: 用来对图象进行离散余弦变换
************************************************************************/
BOOL CosTran(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrcUnChr;    //指向像素的指针
  int i;                 //行循环变量
  int j;                 //列循环变量
  int wid = 1, hei = 1; // 进行付立叶变换的宽度和高度，初始化为1
  Float dTemp; // 中间变量
  int widpor = 0, heiPor = 0; //2的幂数
  Float* pTd, *pFd;

  // 计算进行离散余弦变换的宽度和高度（2的整数次方）
  while (wid * 2 <= lWidth) {
    wid *= 2;
    widpor++;
  }

  while (hei * 2 <= lHeight) {
    hei *= 2;
    heiPor++;
  }

  pTd = MALLOC(Float, wid * hei);   // 分配内存
  pFd = MALLOC(Float, wid * hei);

  for (i = 0; i < hei; i++) {   // 行
    for (j = 0; j < wid; j++) {   // 列
      // 指向DIB第i行，第j个像素的指针
      lpSrcUnChr = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
      pTd[ j + i * wid ] = *(lpSrcUnChr);   // 给时域赋值
    }
  }

  for (i = 0; i < hei; i++) {
    DisFCosTran(&pTd[ wid * i ], &pFd[ wid * i ], widpor);   // 对y方向进行离散余弦变换
  }

  for (i = 0; i < hei; i++) {   // 保存计算结果
    for (j = 0; j < wid; j++) {
      pTd[ j * hei + i ] = pFd[ j + wid * i ];
    }
  }

  for (j = 0; j < wid; j++) {
    DisFCosTran(&pTd[ j * hei ], &pFd[ j * hei ], heiPor);   // 对x方向进行离散余弦变换
  }

  for (i = 0; i < hei; i++) {   // 行
    for (j = 0; j < wid; j++) {   // 列
      dTemp = fabs(pFd[ j * hei + i ]);   // 计算频谱

      // 是否超过255, 超过的，直接设置为255
      if (dTemp > 255) {
        dTemp = 255;
      }

      // 指向DIB第y行，第x个像素的指针
      lpSrcUnChr = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
      * (lpSrcUnChr) = (BYTE)(dTemp);        // 更新源图象
    }
  }

  SAFEFREE(pTd);   // 释放内存
  SAFEFREE(pFd);
  return TRUE;
}

/*************************************************************************
* 函数名称：Walshei_Har(Float* pTd, Float* pFd, int power)
* 函数参数:
*   Float* pTd, 指向时域值的指针
*   Float* pFd, 指向频域值的指针
*   int   power, 2的幂数
 *
* 函数类型:void
* 函数功能: 用来实现快速沃尔什-哈达玛变换
************************************************************************/
void Walshei_Har(Float* pTd, Float* pFd, int power)
{
  int dotCount; // 沃尔什-哈达玛变换点数
  int i, j, k; // 循环变量
  int bfsize, p; // 中间变量
  Float* temReg1, *temReg2, *temReg;
  dotCount = 1 << power; // 计算快速沃尔什变换点数
  temReg1 = MALLOC(Float, dotCount);   // 分配运算所需的数组
  temReg2 = MALLOC(Float, dotCount);
  memcpy(temReg1, pTd, sizeof(Float) * dotCount);     // 将时域点写入数组temReg1

  for (k = 0; k < power; k++) {   // 蝶形运算
    for (j = 0; j < 1 << k; j++) {
      bfsize = 1 << (power - k);

      for (i = 0; i < bfsize / 2; i++) {
        p = j * bfsize;
        temReg2[ i + p ] = temReg1[ i + p ] + temReg1[ i + p + bfsize / 2 ];
        temReg2[ i + p + bfsize / 2 ] = temReg1[ i + p ] - temReg1[ i + p + bfsize / 2 ];
      }
    }

    temReg = temReg1; // 互换temReg1和temReg2
    temReg1 = temReg2;
    temReg2 = temReg;
  }

  for (j = 0; j < dotCount; j++) {   // 调整系数
    p = 0;

    for (i = 0; i < power; i++) {
      if (j & (1 << i)) {
        p += 1 << (power - i - 1);
      }
    }

    pFd[ j ] = temReg1[ p ] / dotCount;
  }

  SAFEFREE(temReg1);   // 释放内存
  SAFEFREE(temReg2);
}

/*************************************************************************
* 函数名称：Walsh_HarTran(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
* 函数参数:
*   LPBYTE lpDIBBits, 指向源DIB图象指针
*   int lWidth, 源DIB图象宽度
*   int lHeight, 源DIB图象高度
*   int lLineBytes, 源DIB图象的行字节数，为4的倍数
* 函数类型:BOOL
* 函数功能:用来对图象进行沃尔什-哈达玛变换
 ************************************************************************/
BOOL Walsh_HarTran(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrcUnChr;    //指向像素的指针
  int i;                 //行循环变量
  int j;                 //列循环变量

  int wid = 1, hei = 1; // 进行付立叶变换的宽度和高度，初始化为1
  Float dTemp; // 中间变量
  int widpor = 0, heiPor = 0; //2的幂数
  Float* pTd, *pFd;

  while (wid * 2 <= lWidth) { // 计算进行离散余弦变换的宽度和高度（2的整数次方）
    wid *= 2;
    widpor++;
  }

  while (hei * 2 <= lHeight) {
    hei *= 2;
    heiPor++;
  }

  pTd = MALLOC(Float, wid * hei);   // 分配内存
  pFd = MALLOC(Float, wid * hei);

  for (i = 0; i < hei; i++) {   // 行
    for (j = 0; j < wid; j++) {   // 列
      // 指向DIB第i行，第j个像素的指针
      lpSrcUnChr = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
      pTd[ j + i * wid ] = *(lpSrcUnChr);   // 给时域赋值
    }
  }

  for (i = 0; i < hei; i++) {
    Walshei_Har(pTd + wid * i, pFd + wid * i, widpor);   // 对y方向进行沃尔什-哈达玛变换
  }

  for (i = 0; i < hei; i++) {   // 保存计算结果
    for (j = 0; j < wid; j++) {
      pTd[ j * hei + i ] = pFd[ j + wid * i ];
    }
  }

  for (j = 0; j < wid; j++) {
    Walshei_Har(pTd + j * hei, pFd + j * hei, heiPor);   // 对x方向进行沃尔什-哈达玛变换
  }

  for (i = 0; i < hei; i++) {   // 行
    for (j = 0; j < wid; j++) {   // 列
      dTemp = fabs(pFd[ j * hei + i ] * 1000);   // 计算频谱

      if (dTemp > 255) { // 对于超过255的，直接设置为255
        dTemp = 255;
      }

      // 指向DIB第i行，第j个像素的指针
      lpSrcUnChr = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
      * (lpSrcUnChr) = (BYTE)(dTemp);        // 更新源图象
    }
  }

  SAFEFREE(pTd);   //释放内存
  SAFEFREE(pFd);
  return TRUE;
}

/*************************************************************************
* 函数名称：
*   FFT()
* 参数:
*   COMPLEX* TD - 指向时域数组的指针
*   COMPLEX* FD - 指向频域数组的指针
*   r      －2的幂数，即迭代次数
* 返回值:
*   无。
* 说明:
*   该函数用来实现快速付立叶变换。
 ************************************************************************/
void FFT(COMPLEX* TD, COMPLEX* FD, int r)
{
  int count;    // 付立叶变换点数
  int i, j, k;    // 循环变量
  int bfsize, p;
  Float angle;    // 角度
  COMPLEX* W, *X1, *X2, *X;

  count = 1 << r;    // 计算付立叶变换点数

  // 分配运算所需存储器
  W = MALLOC(COMPLEX, count / 2);
  X1 = MALLOC(COMPLEX, count);
  X2 = MALLOC(COMPLEX, count);

  // 计算加权系数
  for (i = 0; i < count / 2; i++) {
    angle = -i * PI * 2 / count;
    CplexSet(cos(angle), sin(angle), W[ i ]);
  }

  // 将时域点写入X1
  memcpy(X1, TD, sizeof(COMPLEX) * count);

  // 采用蝶形算法进行快速付立叶变换
  for (k = 0; k < r; k++) {
    for (j = 0; j < 1 << k; j++) {
      bfsize = 1 << (r - k);

      for (i = 0; i < bfsize / 2; i++) {
        COMPLEX c;
        p = j * bfsize;
        CplexAdd(X1[ i + p ], X1[ i + p + bfsize / 2 ], X2[ i + p ]);
        CplexSub(X1[ i + p ], X1[ i + p + bfsize / 2 ], c);
        CplexMul(c, W[ i * (1 << k) ], X2[ i + p + bfsize / 2 ]);
      }
    }

    X = X1;
    X1 = X2;
    X2 = X;
  }

  // 重新排序
  for (j = 0; j < count; j++) {
    p = 0;

    for (i = 0; i < r; i++) {
      if (j & (1 << i)) {
        p += 1 << (r - i - 1);
      }
    }

    FD[ j ] = X1[ p ];
  }

  SAFEFREE(W);
  SAFEFREE(X1);
  SAFEFREE(X2);
}

/*************************************************************************
* 函数名称：
*   IFFT()
* 参数:
*   COMPLEX* FD - 指向频域值的指针
*   COMPLEX* TD - 指向时域值的指针
*   r      －2的幂数
* 返回值:
*   无。
* 说明:
*   该函数用来实现快速付立叶反变换。
 ************************************************************************/
void IFFT(COMPLEX* FD, COMPLEX* TD, int r)
{
  int count;     // 付立叶变换点数
  int i;      // 循环变量
  COMPLEX* X;

  count = 1 << r;     // 计算付立叶变换点数
  X = MALLOC(COMPLEX, count);   // 分配运算所需存储器
  memcpy(X, FD, sizeof(COMPLEX) * count);     // 将频域点写入X

  // 求共轭
  for (i = 0; i < count; i++) {
    CplexSet(CREAL(X[ i ]), -CIMAG(X[ i ]), X[ i ]);
  }

  FFT(X, TD, r);       // 调用快速付立叶变换

  // 求时域点的共轭
  for (i = 0; i < count; i++) {
    CplexSet(CREAL(TD[ i ]) / count, -CIMAG(TD[ i ]) / count, TD[ i ]);
  }

  SAFEFREE(X);
}

/*************************************************************************
* 函数名称：
*   Dct1D()
* 参数:
*   Float* f    - 指向时域值的指针
*   Float* F    - 指向频域值的指针
*   r      －2的幂数
* 返回值:
*   无。
* 说明:
*   该函数用来实现快速离散余弦变换。该函数利用2N点的快速付立叶变换
* 来实现离散余弦变换。
 ************************************************************************/
void Dct1D(const Float* f, Float* F, int r)
{
  int count;   // 离散余弦变换点数
  int i;    // 循环变量
  Float dTemp;
  COMPLEX* X;

  count = 1 << r;   // 计算离散余弦变换点数

  X = MALLOC(COMPLEX, count * 2);
  memset(X, 0, sizeof(COMPLEX) * count * 2);     // 赋初值为0

  // 将时域点写入数组X
  for (i = 0; i < count; i++) {
    CplexSet(f[ i ], 0, X[ i ]);
  }

  FFT(X, X, r + 1);      // 调用快速付立叶变换
  dTemp = 1 / sqrt(count);    // 调整系数
  F[ 0 ] = CREAL(X[ 0 ]) * dTemp;   // 求F[0]
  dTemp *= sqrt(2);

  // 求F[u]
  for (i = 1; i < count; i++) {
    F[ i ] = (CREAL(X[ i ]) * cos(i * PI / (count * 2)) + CIMAG(X[ i ]) *
        sin(i * PI / (count * 2))) * dTemp;
  }

  SAFEFREE(X);
}

/*************************************************************************
* 函数名称：
*   iDct1D()
* 参数:
*   Float* F    - 指向频域值的指针
*   Float* f    - 指向时域值的指针
*   r      －2的幂数
* 返回值:
*   无。
* 说明:
*   该函数用来实现快速离散余弦反变换。该函数也利用2N点的快速付立叶变换
* 来实现离散余弦反变换。
 ************************************************************************/
void iDct1D(const Float* F, Float* f, int r)
{
  int count;   // 离散余弦反变换点数
  int i;    // 循环变量
  Float dTemp, d0;
  COMPLEX* X;

  count = 1 << r;   // 计算离散余弦变换点数

  X = MALLOC(COMPLEX, count * 2);
  memset(X, 0, sizeof(COMPLEX) * count * 2);     // 赋初值为0

  // 将频域变换后点写入数组X
  for (i = 0; i < count; i++) {
    CplexSet(F[ i ] * cos(i * PI / (count * 2)), F[ i ] *
        sin(i * PI / (count * 2)), X[ i ]);
  }

  IFFT(X, X, r + 1);    // 调用快速付立叶反变换

  // 调整系数
  dTemp = sqrt(2.0 / count);
  d0 = (sqrt(1.0 / count) - dTemp) * F[ 0 ];

  for (i = 0; i < count; i++) {
    f[ i ] = d0 + CREAL(X[ i ]) * dTemp * 2 * count;
  }

  SAFEFREE(X);
}

/*************************************************************************
* 函数名称：
*   Dct()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数）
*   int lHeight      - 源图像高度（象素数）
* 返回值:
*   BOOL              - 成功返回TRUE，否则返回FALSE。
* 说明:
*   该函数用来对图像进行离散余弦变换。
 ************************************************************************/
BOOL Dct(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc; // 指向源图像的指针
  int i, j;    // 循环变量
  int w;    // 进行付立叶变换的宽度（2的整数次方）
  int h;    // 进行付立叶变换的高度（2的整数次方）
  Float dTemp, *f, *F;
  int wp;
  int hp;

  // 赋初值
  w = 1;
  h = 1;
  wp = 0;
  hp = 0;

  // 计算进行离散余弦变换的宽度和高度（2的整数次方）
  while (w * 2 <= lWidth) {
    w *= 2;
    wp++;
  }

  while (h * 2 <= lHeight) {
    h *= 2;
    hp++;
  }

  f = MALLOC(Float, w * h);
  F = MALLOC(Float, w * h);

  for (i = 0; i < h; i++) {   // 行
    for (j = 0; j < w; j++) {   // 列
      // 指向DIB第i行，第j个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (i) + j;

      // 给时域赋值
      f[ j + i * w ] = *(lpSrc);
    }
  }

  for (i = 0; i < h; i++) {
    // 对y方向进行离散余弦变换
    Dct1D(&f[ w * i ], &F[ w * i ], wp);
  }

  // 保存计算结果
  for (i = 0; i < h; i++) {
    for (j = 0; j < w; j++) {
      f[ j * h + i ] = F[ j + w * i ];
    }
  }

  for (j = 0; j < w; j++) {
    // 对x方向进行离散余弦变换
    Dct1D(&f[ j * h ], &F[ j * h ], hp);
  }

  for (i = 0; i < h; i++) {   // 行
    for (j = 0; j < w; j++) {   // 列
      dTemp = fabs(F[ j * h + i ]);   // 计算频谱

      // 判断是否超过255
      if (dTemp > 255) {
        dTemp = 255;
      }

      // 指向DIB第y行，第x个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (i) + j;

      // 更新源图像
      * (lpSrc) = (BYTE)(dTemp);
    }
  }

  SAFEFREE(f);
  SAFEFREE(F);
  return TRUE;
}

/*************************************************************************
* 函数名称：
*   WALSH()
* 参数:
*   Float* f    - 指向时域值的指针
*   Float* F    - 指向频域值的指针
*   r      －2的幂数
* 返回值:
*   无。
* 说明:
*   该函数用来实现快速沃尔什-哈达玛变换。
 ************************************************************************/
void WALSH(const Float* f, Float* F, int r)
{
  int count;    // 沃尔什-哈达玛变换点数
  int i, j, k;    // 循环变量
  int bfsize, p;
  Float* X1, *X2, *X;

  count = 1 << r;    // 计算快速沃尔什变换点数
  X1 = MALLOC(Float, count);    // 分配运算所需的数组
  X2 = MALLOC(Float, count);    // 分配运算所需的数组

  memcpy(X1, f, sizeof(Float) * count);     // 将时域点写入数组X1

  // 蝶形运算
  for (k = 0; k < r; k++) {
    for (j = 0; j < 1 << k; j++) {
      bfsize = 1 << (r - k);

      for (i = 0; i < bfsize / 2; i++) {
        p = j * bfsize;
        X2[ i + p ] = X1[ i + p ] + X1[ i + p + bfsize / 2 ];
        X2[ i + p + bfsize / 2 ] = X1[ i + p ] - X1[ i + p + bfsize / 2 ];
      }
    }

    // 互换X1和X2
    X = X1;
    X1 = X2;
    X2 = X;
  }

  // 调整系数
  for (j = 0; j < count; j++) {
    p = 0;

    for (i = 0; i < r; i++) {
      if (j & (1 << i)) {
        p += 1 << (r - i - 1);
      }
    }

    F[ j ] = X1[ p ] / count;
  }

  SAFEFREE(X1);
  SAFEFREE(X2);
}

/*************************************************************************
* 函数名称：
*   IWALSH()
* 参数:
*   Float* F    - 指向频域值的指针
*   Float* f    - 指向时域值的指针
*   r      －2的幂数
* 返回值:
*   无。
* 说明:
*   该函数用来实现快速沃尔什-哈达玛反变换。
 ************************************************************************/
void IWALSH(const Float* F, Float* f, int r)
{
  int count;    // 变换点数
  int i;     // 循环变量

  count = 1 << r;    // 计算变换点数
  WALSH(F, f, r);      // 调用快速沃尔什－哈达玛变换进行反变换

  for (i = 0; i < count; i++) {   // 调整系数
    f[ i ] *= count;
  }
}

/*************************************************************************
* 函数名称：
*   Walsh()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int  lWidth      - 源图像宽度（象素数）
*   int  lHeight     - 源图像高度（象素数）
* 返回值:
*   BOOL              - 成功返回TRUE，否则返回FALSE。
* 说明:
*   该函数用来对图像进行沃尔什-哈达玛变换。函数首先对图像每列进行一维
* 沃尔什－哈达玛变换，然后对变换结果的每行进行一维沃尔什－哈达玛变换。
 ************************************************************************/
BOOL Walsh(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc;  // 指向源图像的指针
  int i, j;    // 循环变量
  int w;     // 进行付立叶变换的宽度（2的整数次方）
  int h;     // 进行付立叶变换的高度（2的整数次方）
  Float dTemp, *f, *F;
  int wp;
  int hp;

  // 赋初值
  w = 1;
  h = 1;
  wp = 0;
  hp = 0;

  // 计算进行离散余弦变换的宽度和高度（2的整数次方）
  while (w * 2 <= lWidth) {
    w *= 2;
    wp++;
  }

  while (h * 2 <= lHeight) {
    h *= 2;
    hp++;
  }

  f = MALLOC(Float, w * h);
  F = MALLOC(Float, w * h);

  for (i = 0; i < h; i++) {   // 行
    for (j = 0; j < w; j++) {   // 列
      // 指向DIB第i行，第j个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (i) + j;

      // 给时域赋值
      f[ j + i * w ] = *(lpSrc);
    }
  }

  for (i = 0; i < h; i++) {
    // 对y方向进行沃尔什-哈达玛变换
    WALSH(f + w * i, F + w * i, wp);
  }

  // 保存计算结果
  for (i = 0; i < h; i++) {
    for (j = 0; j < w; j++) {
      f[ j * h + i ] = F[ j + w * i ];
    }
  }

  for (j = 0; j < w; j++) {
    // 对x方向进行沃尔什-哈达玛变换
    WALSH(f + j * h, F + j * h, hp);
  }

  for (i = 0; i < h; i++) {   // 行
    for (j = 0; j < w; j++) {   // 列
      dTemp = fabs(F[ j * h + i ] * 1000);   // 计算频谱

      if (dTemp > 255) {
        dTemp = 255;
      }

      // 指向DIB第i行，第j个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (i) + j;

      // 更新源图像
      * (lpSrc) = (BYTE)(dTemp);
    }
  }

  SAFEFREE(f);
  SAFEFREE(F);

  return TRUE;
}

/*************************************************************************
* 函数名称：
*   Walsh1()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数）
*   int lHeight      - 源图像高度（象素数）
* 返回值:
*   BOOL              - 成功返回TRUE，否则返回FALSE。
* 说明:
*   该函数用来对图像进行沃尔什-哈达玛变换。于上面不同的是，此处是将二维
* 矩阵转换成一个列向量，然后对该列向量进行一次一维沃尔什-哈达玛变换。
 ************************************************************************/
BOOL Walsh1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc;  // 指向源图像的指针
  int i, j;    // 循环变量
  int w;     // 进行付立叶变换的宽度（2的整数次方）
  int h;     // 进行付立叶变换的高度（2的整数次方）
  Float dTemp, *f, *F;
  int wp;
  int hp;

  // 赋初值
  w = 1;
  h = 1;
  wp = 0;
  hp = 0;

  // 计算进行离散余弦变换的宽度和高度（2的整数次方）
  while (w * 2 <= lWidth) {
    w *= 2;
    wp++;
  }

  while (h * 2 <= lHeight) {
    h *= 2;
    hp++;
  }

  f = MALLOC(Float, w * h);
  F = MALLOC(Float, w * h);

  for (i = 0; i < w; i++) {   // 列
    for (j = 0; j < h; j++) {   // 行
      // 指向DIB第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - j) + i;

      // 给时域赋值
      f[ j + i * w ] = *(lpSrc);
    }
  }

  // 调用快速沃尔什－哈达玛变换
  WALSH(f, F, wp + hp);

  for (i = 0; i < w; i++) {   // 列
    for (j = 0; j < h; j++) {   // 行
      dTemp = fabs(F[ i * w + j ] * 1000);   // 计算频谱

      if (dTemp > 255) {
        dTemp = 255;
      }

      // 指向DIB第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - j) + i;

      // 更新源图像
      * (lpSrc) = (BYTE)(dTemp);
    }
  }

  SAFEFREE(f);
  SAFEFREE(F);

  return TRUE;
}

/*************************************************************************
 *
* \函数名称：
*   Fft2D()
 *
* \输入参数:
*   COMPLEX* pCTData - 图像数据
*   int   lWidth    - 数据宽度
*   int   lHeight    - 数据高度
*   COMPLEX* pCFData - 傅立叶变换后的结果
 *
* \返回值:
*   无
 *
* \说明:
*   二维傅立叶变换。
 *
 ************************************************************************
 */
void FFT_2D1(COMPLEX* pCTData, int lWidth, int lHeight, COMPLEX* pCFData)
{
  // 循环控制变量
  int x;
  int y;

  // 临时变量
  Float dTmpOne;
  Float dTmpTwo;

  // 进行傅立叶变换的宽度和高度，（2的整数次幂）
  // 图像的宽度和高度不一定为2的整数次幂
  int nTransWidth;
  int nTransHeight;

  // x，y（行列）方向上的迭代次数
  int nXLev;
  int nYLev;

  // 计算进行傅立叶变换的宽度 （2的整数次幂）
  dTmpOne = log(lWidth) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransWidth = (int) dTmpTwo;

  // 计算进行傅立叶变换的高度 （2的整数次幂）
  dTmpOne = log(lHeight) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransHeight = (int) dTmpTwo;

  // 计算x，y（行列）方向上的迭代次数
  nXLev = (int)(log(nTransWidth) / log(2) + 0.5);
  nYLev = (int)(log(nTransHeight) / log(2) + 0.5);

  for (y = 0; y < nTransHeight; y++) {
    // x方向进行快速傅立叶变换
    FFT_1D(&pCTData[ nTransWidth * y ], &pCFData[ nTransWidth * y ], nXLev);
  }

  // pCFData中目前存储了pCTData经过行变换的结果
  // 为了直接利用FFT_1D，需要把pCFData的二维数据转置，再一次利用FFT_1D进行
  // 傅立叶行变换（实际上相当于对列进行傅立叶变换）
  for (y = 0; y < nTransHeight; y++) {
    for (x = 0; x < nTransWidth; x++) {
      pCTData[ nTransHeight * x + y ] = pCFData[ nTransWidth * y + x ];
    }
  }

  for (x = 0; x < nTransWidth; x++) {
    // 对x方向进行快速傅立叶变换，实际上相当于对原来的图象数据进行列方向的
    // 傅立叶变换
    FFT_1D(&pCTData[ x * nTransHeight ], &pCFData[ x * nTransHeight ], nYLev);
  }

  // pCFData中目前存储了pCTData经过二维傅立叶变换的结果，但是为了方便列方向
  // 的傅立叶变换，对其进行了转置，现在把结果转置回来
  for (y = 0; y < nTransHeight; y++) {
    for (x = 0; x < nTransWidth; x++) {
      pCTData[ nTransWidth * y + x ] = pCFData[ nTransHeight * x + y ];
    }
  }

  memcpy(pCTData, pCFData, sizeof(COMPLEX) * nTransHeight * nTransWidth);
}

/*************************************************************************
 * 函数名称：
 *   ThreeCross()
 *
 * 参数:
 *   Float  *Matrix     - 用来存放矩阵A
 *   int    Rank        - 矩阵A的阶数
 *   Float  *QMatrix    - 返回householder变换的矩阵Q
 *   Float  *MainCross  - 对称三角阵中的主对角元素
 *   Float  *HypoCross  - 对称三角阵中的次对角元素
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用householder变换将n阶实对称矩阵化为对称三角阵
 *
 ************************************************************************/
BOOL ThreeCross(Float* Matrix, int Rank, Float* QMatrix, Float* MainCross, Float* HypoCross)
{
  //   循环变量
  int i, j, k, u;
  Float h, f, g, h2;

  // 对矩阵QMatrix赋值
  for (i = 0; i <= Rank - 1; i++)
    for (j = 0; j <= Rank - 1; j++) {
      u = i * Rank + j;
      QMatrix[ u ] = Matrix[ u ];
    }

  for (i = Rank - 1; i >= 1; i--) {
    h = 0.0;

    if (i > 1)
      for (k = 0; k <= i - 1; k++) {
        u = i * Rank + k;
        h = h + QMatrix[ u ] * QMatrix[ u ];
      }

    // 如果一行全部为零
    if (h + 1.0 == 1.0) {
      HypoCross[ i ] = 0.0;

      if (i == 1) {
        HypoCross[ i ] = QMatrix[ i * Rank + i - 1 ];
      }

      MainCross[ i ] = 0.0;
    }

    // 否则进行householder变换，求正交矩阵的值
    else {
      // 求次对角元素的值
      HypoCross[ i ] = sqrt(h);

      // 判断i行i-1列元素是不是大于零
      u = i * Rank + i - 1;

      if (QMatrix[ u ] > 0.0) {
        HypoCross[ i ] = -HypoCross[ i ];
      }

      h = h - QMatrix[ u ] * HypoCross[ i ];
      QMatrix[ u ] = QMatrix[ u ] - HypoCross[ i ];
      f = 0.0;

      // householder变换
      for (j = 0; j <= i - 1; j++) {
        QMatrix[ j * Rank + i ] = QMatrix[ i * Rank + j ] / h;
        g = 0.0;

        for (k = 0; k <= j; k++) {
          g = g + QMatrix[ j * Rank + k ] * QMatrix[ i * Rank + k ];
        }

        if (j + 1 <= i - 1)
          for (k = j + 1; k <= i - 1; k++) {
            g = g + QMatrix[ k * Rank + j ] * QMatrix[ i * Rank + k ];
          }

        HypoCross[ j ] = g / h;
        f = f + g * QMatrix[ j * Rank + i ];
      }

      h2 = f / (h + h);

      // 求正交矩阵的值
      for (j = 0; j <= i - 1; j++) {
        f = QMatrix[ i * Rank + j ];
        g = HypoCross[ j ] - h2 * f;
        HypoCross[ j ] = g;

        for (k = 0; k <= j; k++) {
          u = j * Rank + k;
          QMatrix[ u ] = QMatrix[ u ] - f * HypoCross[ k ] - g * QMatrix[ i * Rank + k ];
        }
      }

      MainCross[ i ] = h;
    }
  }

  // 赋零值
  for (i = 0; i <= Rank - 2; i++) {
    HypoCross[ i ] = HypoCross[ i + 1 ];
  }

  HypoCross[ Rank - 1 ] = 0.0;
  MainCross[ 0 ] = 0.0;

  for (i = 0; i <= Rank - 1; i++) {
    // 主对角元素的计算
    if ((MainCross[ i ] != 0.0) && (i - 1 >= 0))
      for (j = 0; j <= i - 1; j++) {
        g = 0.0;

        for (k = 0; k <= i - 1; k++) {
          g = g + QMatrix[ i * Rank + k ] * QMatrix[ k * Rank + j ];
        }

        for (k = 0; k <= i - 1; k++) {
          u = k * Rank + j;
          QMatrix[ u ] = QMatrix[ u ] - g * QMatrix[ k * Rank + i ];
        }
      }

    // 将主对角线的元素进行存储，以便返回
    u = i * Rank + i;
    MainCross[ i ] = QMatrix[ u ];
    QMatrix[ u ] = 1.0;

    // 将三对角外所有的元素赋零值
    if (i - 1 >= 0)
      for (j = 0; j <= i - 1; j++) {
        QMatrix[ i * Rank + j ] = 0.0;
        QMatrix[ j * Rank + i ] = 0.0;
      }
  }

  // 返回值
  return (TRUE);
}


/*************************************************************************
* 函数名称：
*   BSTQ()
 *
* 参数:
*   int    Rank        - 矩阵A的阶数
*   Float  *MainCross  - 对称三角阵中的主对角元素，返回时存放A的特征值
*   Float  *HypoCross  - 对称三角阵中的次对角元素
*   Float  *Matrix     - 返回对称矩阵A的特征向量
*   Float Eps          - 控制精度
*   int MaxT            - 最大迭代次数
 *
* 返回值:
*   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
* 说明:
*   该函数用变形QR方法计算实对称三角矩阵的全部特征值以及相应的特征向量
 ************************************************************************/
BOOL BSTQ(int Rank, Float* MainCross, Float* HypoCross,
    Float* Matrix, Float Eps, int MaxT)
{
  // 变量的定义
  int i, j, k, m, it, u, v;
  Float d, f, h, g, p, r, e, s;

  // 赋零值
  HypoCross[ Rank - 1 ] = 0.0;
  d = 0.0;
  f = 0.0;

  for (j = 0; j <= Rank - 1; j++) {
    //  迭代精度的控制
    it = 0;
    h = Eps * (fabs(MainCross[ j ]) + fabs(HypoCross[ j ]));

    if (h > d) {
      d = h;
    }

    m = j;

    while ((m <= Rank - 1) && (fabs(HypoCross[ m ]) > d)) {
      m = m + 1;
    }

    if (m != j) {
      // 进行迭代，求得矩阵A的特征值和特征向量
      do {
        // 超过迭代次数，返回迭代失败
        if (it == MaxT) {
          return (FALSE);
        }

        it = it + 1;
        g = MainCross[ j ];
        p = (MainCross[ j + 1 ] - g) / (2.0 * HypoCross[ j ]);
        r = sqrt(p * p + 1.0);

        // 如果p大于0
        if (p >= 0.0) {
          MainCross[ j ] = HypoCross[ j ] / (p + r);
        }
        else {
          MainCross[ j ] = HypoCross[ j ] / (p - r);
        }

        h = g - MainCross[ j ];

        //  计算主对角线的元素
        for (i = j + 1; i <= Rank - 1; i++) {
          MainCross[ i ] = MainCross[ i ] - h;
        }

        // 赋值
        f = f + h;
        p = MainCross[ m ];
        e = 1.0;
        s = 0.0;

        for (i = m - 1; i >= j; i--) {
          g = e * HypoCross[ i ];
          h = e * p;

          //  主对角线元素的绝对值是否大于次对角线元素的
          if (fabs(p) >= fabs(HypoCross[ i ])) {
            e = HypoCross[ i ] / p;
            r = sqrt(e * e + 1.0);
            HypoCross[ i + 1 ] = s * p * r;
            s = e / r;
            e = 1.0 / r;
          }
          else {
            e = p / HypoCross[ i ];
            r = sqrt(e * e + 1.0);
            HypoCross[ i + 1 ] = s * HypoCross[ i ] * r;
            s = 1.0 / r;
            e = e / r;
          }

          p = e * MainCross[ i ] - s * g;
          MainCross[ i + 1 ] = h + s * (e * g + s * MainCross[ i ]);

          // 重新存储特征向量
          for (k = 0; k <= Rank - 1; k++) {
            u = k * Rank + i + 1;
            v = u - 1;
            h = Matrix[ u ];
            Matrix[ u ] = s * Matrix[ v ] + e * h;
            Matrix[ v ] = e * Matrix[ v ] - s * h;
          }

        }

        // 将主对角线和次对角线元素重新赋值
        HypoCross[ j ] = s * p;
        MainCross[ j ] = e * p;

      }
      while (fabs(HypoCross[ j ]) > d);
    }

    MainCross[ j ] = MainCross[ j ] + f;
  }

  // 返回A的特征值
  for (i = 0; i <= Rank - 1; i++) {
    k = i;
    p = MainCross[ i ];

    // 将A特征值赋给p
    if (i + 1 <= Rank - 1) {
      j = i + 1;

      while ((j <= Rank - 1) && (MainCross[ j ] <= p)) {
        k = j;
        p = MainCross[ j ];
        j = j + 1;
      }
    }

    // 存储A的特征值和特征向量
    if (k != i) {
      MainCross[ k ] = MainCross[ i ];
      MainCross[ i ] = p;

      for (j = 0; j <= Rank - 1; j++) {
        u = j * Rank + i;
        v = j * Rank + k;
        p = Matrix[ u ];
        Matrix[ u ] = Matrix[ v ];
        Matrix[ v ] = p;
      }
    }
  }

  // 返回值
  return (TRUE);
}

/*************************************************************************
 *
* \函数名称：
*   IDFT_2D()
 *
* \输入参数:
*   int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes    - 指向CDib类的指针，含有图像数据
*   Float* pTrRstRpart  - 指向傅立叶系数实部的指针
*   Float* pTrRstIpart  - 指向傅立叶系数虚部的指针
 *
* \返回值:
*   无
 *
* \说明:
*   二维傅立叶反变换。
 *
 *************************************************************************
 */

void IDFT_2D(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, Float* pTrRstRpart, Float* pTrRstIpart)
{
  //遍历图象的纵坐标
  int y;

  //遍历图象的横坐标
  int x;

  //频域的横坐标
  int m;

  //频域的纵坐标
  int n;

  Float fCosTable, fSinTable;
  Float fTmpPxValue, fRpartValue, fIpartValue;

  fCosTable = 0;
  fSinTable = 0;
  fTmpPxValue = 0;
  fRpartValue = 0;
  fIpartValue = 0;

  for (y = 0; y < lHeight; y++)
    for (x = 0; x < lWidth; x++) {
      fTmpPxValue = 0;

      for (n = 0; n < lHeight; n++)
        for (m = 0; m < lWidth; m++) {
          fCosTable = cos(2 * PI * (((Float) m * x) / lWidth + ((Float) n * y) / lHeight));
          fSinTable = sin(2 * PI * (((Float) m * x) / lWidth + ((Float) n * y) / lHeight));
          fRpartValue = *(pTrRstRpart + n * lHeight + m);
          fIpartValue = *(pTrRstIpart + n * lHeight + m);

          fTmpPxValue += fCosTable * fRpartValue - fSinTable * fIpartValue;
        }

      fTmpPxValue = fTmpPxValue / (lHeight * lWidth);
      *(lpDIBBits + lLineBytes * y + x) = (BYTE) fTmpPxValue;
    }
}

/*************************************************************************
* 函数名称：
*   DIBOHOTELLING()
 *
* 参数:
*   CDib  *pDib        - 指向CDib类的指针
 *
* 返回值:
*   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
* 说明:
*   该函数用霍特林变换来对图像进行旋转。
 ************************************************************************/
BOOL Hotelling(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // 规定迭代的计算精度
  Float Eps = 0.000001;

  // 指向源图像的指针
  LPBYTE lpSrc;

  // 循环变量
  int i;
  int j;

  // 经过变换后图象最大可能范围
  int lMaxRange;

  //  物体坐标的均值
  int AverEx;
  int AverEy;

  //  物体总的象素数
  int ToaCount;

  // 坐标值的协方差矩阵
  Float Matr4C[ 2 ][ 2 ];

  // 存放协方差矩阵的特征向量
  Float QMatrix[ 2 ][ 2 ];

  //  三对角阵的主对角和次对角线元素
  Float MainCross[ 2 ];
  Float HypoCross[ 2 ];

  // 中间变量
  Float dTemp, *F;
  int lTempI;
  int lTempJ;

  // 估计图象经过旋转后可能最大的宽度和高度
  if (lWidth > lHeight) {
    lMaxRange = lWidth;
  }
  else {
    lMaxRange = lHeight;
  }

  // 赋初值
  AverEx = 0;
  AverEy = 0;
  ToaCount = 0;
  Matr4C[ 0 ][ 0 ] = Matr4C[ 0 ][ 1 ] = Matr4C[ 1 ][ 0 ] = Matr4C[ 1 ][ 1 ] = 0.0;

  // 分配内存
  F = MALLOC(Float, lWidth * lHeight);


  for (i = 0; i < lHeight; i++) {   // 行
    for (j = 0; j < lWidth; j++) {   // 列
      // 给旋转后坐标轴的每个点赋零值（灰度值255对应显示白色）
      F[ i * lWidth + j ] = 255;

      // 指向位图i行j列象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * i + j;

      // 值小于255（非背景色白色）的象素认为物体的一部分
      // 并将其坐标值x和y看作二维随机矢量
      if ((*lpSrc) < 255) {
        // 属于物体象素的Y坐标和X坐标累计值
        AverEx = AverEx + i;
        AverEy = AverEy + j;

        // 物体总的象素数加一
        ToaCount++;

        // 随机矢量协方差矩阵的累计值
        Matr4C[ 0 ][ 0 ] = Matr4C[ 0 ][ 0 ] + i * i;
        Matr4C[ 0 ][ 1 ] = Matr4C[ 0 ][ 1 ] + i * j;
        Matr4C[ 1 ][ 0 ] = Matr4C[ 1 ][ 0 ] + j * i;
        Matr4C[ 1 ][ 1 ] = Matr4C[ 1 ][ 1 ] + j * j;
      }
    }
  }


  // 计算随机矢量的均值
  AverEx = AverEx / ToaCount;
  AverEy = AverEy / ToaCount;

  //  计算随机矢量的协方差矩阵
  Matr4C[ 0 ][ 0 ] = Matr4C[ 0 ][ 0 ] / ToaCount - AverEx * AverEx;
  Matr4C[ 0 ][ 1 ] = Matr4C[ 0 ][ 1 ] / ToaCount - AverEx * AverEy;
  Matr4C[ 1 ][ 0 ] = Matr4C[ 1 ][ 0 ] / ToaCount - AverEx * AverEy;
  Matr4C[ 1 ][ 1 ] = Matr4C[ 1 ][ 1 ] / ToaCount - AverEy * AverEy;

  // 将协方差矩阵化作三对角对称阵
  ThreeCross(*Matr4C, 2, *QMatrix, MainCross, HypoCross);

  // 求协方差矩阵的特征值和特征矢向量
  BSTQ(2, MainCross, HypoCross, *Matr4C, Eps, 50);

  // 将特征列向量转化称特征列向量
  dTemp = Matr4C[ 0 ][ 1 ];
  Matr4C[ 0 ][ 1 ] = Matr4C[ 1 ][ 0 ];
  Matr4C[ 1 ][ 0 ] = dTemp;

  // 对特征列向量进行归一化
  for (i = 0; i <= 1; i++) {
    dTemp = pow(Matr4C[ i ][ 0 ], 2) + pow(Matr4C[ i ][ 1 ], 2);
    dTemp = sqrt(dTemp);
    Matr4C[ i ][ 0 ] = Matr4C[ i ][ 0 ] / dTemp;
    Matr4C[ i ][ 1 ] = Matr4C[ i ][ 1 ] / dTemp;
  }

  // 查找经霍特林变换后的坐标点在原坐标系中的坐标
  for (i = -lMaxRange + 1; i < lMaxRange; i++) {
    for (j = -lMaxRange + 1; j < lMaxRange; j++) {
      //  将新坐标值映射到旧的坐标系
      int Cx = (int)(i * Matr4C[ 0 ][ 0 ] - j * Matr4C[ 0 ][ 1 ]) + AverEx;
      int Cy = (int)(-i * Matr4C[ 1 ][ 0 ] + j * Matr4C[ 1 ][ 1 ]) + AverEy;

      //  映射值是否属于源图象
      if (Cx >= 0 && Cx < lHeight && Cy >= 0 && Cy < lWidth) {
        lpSrc = (LPBYTE) lpDIBBits + lLineBytes * Cx + Cy;

        // 映射值是否属于原来的物体
        if (*(lpSrc) < 255) {
          //  将新坐标系原点平移到中心，以便显示
          lTempI = (int)(lHeight / 2) + j;
          lTempJ = (int)(lWidth / 2) + i;

          // 看如果能够进行显示，赋值给数组，进行存储
          if (lTempI >= 0 && lTempI < lHeight && lTempJ >= 0 && lTempJ < lWidth) {
            F[ lTempJ + (lTempI) * lWidth ] = *(lpSrc);
          }
        }
      }
    }
  }

  // 行
  for (i = 0; i < lMaxRange; i++) {
    // 列
    for (j = 0; j < lMaxRange; j++) {
      // 霍特林变换后的象素值
      dTemp = F[ i * lMaxRange + j ];

      // 指向位图i行j列象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;

      // 更新源图像
      * (lpSrc) = (BYTE)(dTemp);
    }
  }

  // 释放内存
  SAFEFREE(F);

  // 返回
  return TRUE;
}

/*************************************************************************
* 函数名称：InverseFilter (int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
* 函数参数:
*   LPBYTE lpDIBBits, 指向DIB起始像素的指针
*   int lWidth, DIB的宽度
*   int lHeight, int lLineBytes, DIB的高度
*     int lLineBytes, DIB的行字节数，为4的倍数
* 函数类型:BOOL
* 函数功能: 用来对图象进行逆滤波复原
************************************************************************/
BOOL InverseFilter(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrcUnChr;    //指向像素的指针
  int i, j; //循环变量
  int wid = 1; // 实际进行付立叶变换的宽度和高度
  int hei = 1;
  int widpor = 0;
  int heiPor = 0;
  Float tempre, tempim, a, b, c, d; //临时变量
  COMPLEX* pTdSrc, *pTh; //用来存储源图象和变换核的时域数据
  COMPLEX* pFdSrc, *pFh; //用来存储源图象和变换核的频域数据
  Float MaxNum; //图象归一化因子

  // 保证离散傅立叶变换的宽度和高度为2的整数次方
  while (wid * 2 <= lLineBytes) {
    wid = wid * 2;
    widpor++;
  }

  while (hei * 2 <= lHeight) {
    hei = hei * 2;
    heiPor++;
  }

  //输入退化图象的长和宽必须为2的整数倍
  if (wid != (int) lLineBytes) {
    return FALSE;
  }

  if (hei != (int) lHeight) {
    return FALSE;
  }

  pTdSrc = MALLOC(COMPLEX, sizeof(COMPLEX) * lHeight * lLineBytes);     // 为时域和频域的数组分配空间
  pTh = MALLOC(COMPLEX, sizeof(COMPLEX) * lHeight * lLineBytes);
  pFdSrc = MALLOC(COMPLEX, sizeof(COMPLEX) * lHeight * lLineBytes);
  pFh = MALLOC(COMPLEX, sizeof(COMPLEX) * lHeight * lLineBytes);

  // 将退化图象数据存入时域数组
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      lpSrcUnChr = (LPBYTE) lpDIBBits + lLineBytes * j + i;   // 指向退化图象倒数第j行，第i个像素的指针

      CplexSet((Float) * lpSrcUnChr, 0.0, pTdSrc[ lLineBytes * j + i ]);
      CplexSet(0.0, 0.0, pFdSrc[ lLineBytes * j + i ]);

      if (i < 5 && j < 5) {
        CplexSet(0.04, 0.0, pTh[ lLineBytes * j + i ]);
      }
      else {
        CplexSet(0.0, 0.0, pTh[ lLineBytes * j + i ]);
      }

      CplexSet(0.0, 0.0, pFh[ lLineBytes * j + i ]);
    }
  }

  Fourier1(pTdSrc, lLineBytes, lHeight, pFdSrc);   //对退化图象进行FFT
  Fourier1(pTh, lLineBytes, lHeight, pFh);   //对变换核图象进行FFT

  for (i = 0; i < lHeight * lLineBytes; i++) { //频域相除
    a = CREAL(pFdSrc[ i ]);
    b = CIMAG(pFdSrc[ i ]);
    c = CREAL(pFh[ i ]);
    d = CIMAG(pFh[ i ]);

    //如果频域值太小，不予考虑
    if (c * c + d * d > 1e-3) {
      tempre = (a * c + b * d) / (c * c + d * d);
      tempim = (b * c - a * d) / (c * c + d * d);
    }

    CplexSet(tempre, tempim, pFdSrc[ i ]);

  }

  InverseFourier(pFdSrc, pTdSrc, lLineBytes, lHeight);   //对复原图象进行反FFT
  MaxNum = 300; //确定归一化因子

  for (j = 0; j < lHeight; j++) { //转换为复原图象
    for (i = 0; i < lLineBytes; i++) {
      lpSrcUnChr = (LPBYTE) lpDIBBits + lLineBytes * j + i;   // 指向复原图象倒数第j行，第i个像素的指针
      *lpSrcUnChr = (BYTE)(CREAL(pTdSrc[(lLineBytes) * j + i ]) * 255.0 / MaxNum);
    }
  }

  SAFEFREE(pTdSrc);   //释放存储空间
  SAFEFREE(pTh);
  SAFEFREE(pFdSrc);
  SAFEFREE(pFh);
  return TRUE; // 返回
}

/*************************************************************************
* 函数名称：WinnerFilter(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
* 函数参数:
*   LPBYTE lpDIBBits, 指向DIB起始像素的指针
*   int lWidth, DIB的宽度
*   int lHeight, int lLineBytes, DIB的高度
*     int lLineBytes, DIB的行字节数，为4的倍数
* 函数类型:BOOL
* 函数功能: 用来对图象进行维纳滤波复原
************************************************************************/
BOOL WinnerFilter(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrcUnChr;    //指向像素的指针
  int i; //循环变量
  int j;
  Float temp, tempre, tempim,  //临时变量
        a, b, c, d, tempnorm;
  int wid = 1; // 实际进行付立叶变换的宽度和高度
  int hei = 1;

  int widpor = 0;
  int heiPor = 0;
  COMPLEX* pTdSrc, *pTh; //用来存储源图象和变换核的时域数据
  COMPLEX* pFdSrc, *pFh; //用来存储源图象和变换核的频域数据
  Float* pCFFilter;

  // 保证离散傅立叶变换的宽度和高度为2的整数次方
  while (wid * 2 <= lLineBytes) {
    wid = wid * 2;
    widpor++;
  }

  while (hei * 2 <= lHeight) {
    hei = hei * 2;
    heiPor++;
  }

  //输入退化图象的长和宽必须为2的整数倍
  if (wid != (int) lLineBytes) {
    return FALSE;
  }

  if (hei != (int) lHeight) {
    return FALSE;
  }

  pTdSrc = MALLOC(COMPLEX, sizeof(COMPLEX) * lHeight * lLineBytes);     // 为时域和频域的数组分配空间
  pTh = MALLOC(COMPLEX, sizeof(COMPLEX) * lHeight * lLineBytes);
  pFdSrc = MALLOC(COMPLEX, sizeof(COMPLEX) * lHeight * lLineBytes);
  pFh = MALLOC(COMPLEX, sizeof(COMPLEX) * lHeight * lLineBytes);

  pCFFilter = MALLOC(Float, lHeight * lLineBytes);   // 滤波器加权系数

  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lLineBytes; i++) {
      lpSrcUnChr = (LPBYTE) lpDIBBits + lLineBytes * j + i;   // 指向退化图象倒数第j行，第i个像素的指针
      // 将像素值存储到时域数组中
      CplexSet((Float) * lpSrcUnChr, 0.0, pTdSrc[ lLineBytes * j + i ]);
      // 频域赋零值
      CplexSet(0.0, 0.0, pFdSrc[ lLineBytes * j + i ]);

      if (i < 5 && j < 5) {   // 退化系统时域及维纳滤波加权系数赋值
        CplexSet(0.04, 0.0, pTh[ lLineBytes * j + i ]);
        pCFFilter[ lLineBytes * j + i ] = 0.5;
      }
      else {
        CplexSet(0.0, 0.0, pTh[ lLineBytes * j + i ]);
        pCFFilter[ lLineBytes * j + i ] = 0.05;
      }

      // 频域赋零值
      CplexSet(0.0, 0.0, pFh[ lLineBytes * j + i ]);
    }
  }

  Fourier1(pTdSrc, lLineBytes, lHeight, pFdSrc);   //对退化图象进行FFT
  Fourier1(pTh, lLineBytes, lHeight, pFh);         //对变换核图象进行FFT

  for (i = 0; i < lHeight * lLineBytes; i++) {  // 计算M
    a = CREAL(pFdSrc[ i ]);   // 赋值
    b = CIMAG(pFdSrc[ i ]);
    c = CREAL(pFh[ i ]);
    d = CIMAG(pFh[ i ]);
    tempnorm = c * c + d * d; // 求模
    temp = (tempnorm) / (tempnorm + pCFFilter[ i ]);     // |H(u, v)|*|H(u, v)|/(|H(u, v)|*|H(u, v)|+a)
    {
      tempre = (a * c + b * d) / (c * c + d * d);
      tempim = (b * c - a * d) / (c * c + d * d);
      // 求得f(u, v)
      CplexSet(temp * tempre, temp * tempim, pFdSrc[ i ]);
    }
  }

  InverseFourier(pFdSrc, pTdSrc, lLineBytes, lHeight);   //对复原图象进行反FFT

  for (j = 0; j < lHeight; j++) { //转换为复原图象
    for (i = 0; i < lLineBytes; i++) {
      // 指向复原图象倒数第j行，第i个像素的指针
      lpSrcUnChr = (LPBYTE) lpDIBBits + lLineBytes * j + i;
      a = CREAL(pTdSrc[(lLineBytes) * j + i ]);
      b = CIMAG(pTdSrc[(lLineBytes) * j + i ]);
      tempnorm = a * a + b * b;
      tempnorm = sqrt(tempnorm) + 40;

      if (tempnorm > 255) {
        tempnorm = 255.0;
      }

      if (tempnorm < 0) {
        tempnorm = 0;
      }

      *lpSrcUnChr = (BYTE)(tempnorm);
    }
  }

  SAFEFREE(pTdSrc);   //释放存储空间
  SAFEFREE(pTh);
  SAFEFREE(pFdSrc);
  SAFEFREE(pFh);
  SAFEFREE(pCFFilter);
  return TRUE; // 返回
}

/*************************************************************************
* \函数名称：
*   LowPassFilter()
* \输入参数:
*   LPBYTE lpDIBBits     - 指向需要滤波的图像像素指针
*   int lWidth      - 数据宽度
*   int lHeight      - 数据高度
*   int nRadius1      - 理想低通滤波的横向滤波半径
*  int nRadius2      - 理想低通滤波的纵向滤波半径
* \返回值:
*    BOOL       // 成功返回TRUE，否则返回FALSE。
* \说明:
*  lpDIBBits 是指向需要滤波的图像像素指针。
*  经过低通滤波的数据仍然存储在lpDIBBits 当中。
**************************************************************************/
BOOL LowPassFilter(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nRadius1, int nRadius2)
{
  LPBYTE lpSrc;       // 指向源图像的指针
  int x, y;          // 循环控制变量
  Float dTmpOne;        //存放临时变量
  Float dTmpTwo;        //存放临时变量
  int nTransWidth;        // 傅立叶变换的宽度（2的整数次幂）
  int nTransHeight;        // 傅立叶变换的高度（2的整数次幂）
  Float unchValue;        // 存贮图像各像素灰度的临时变量
  Float dReal;         // 傅立叶变换的实部
  Float dImag;         // 傅立叶变换的虚部
  COMPLEX* pCTData;      // 指向时域数据的指针
  COMPLEX* pCFData;      // 指向频域数据的指针
  // 计算进行傅立叶变换的点数－横向 （2的整数次幂）
  dTmpOne = log(lWidth) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransWidth = (int) dTmpTwo;
  // 计算进行傅立叶变换的点数－纵向 （2的整数次幂）
  dTmpOne = log(lHeight) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransHeight = (int) dTmpTwo;

  // 低通滤波的半径不能超过频域的最大半径
  if (nRadius1 > nTransWidth || nRadius2 > nTransHeight) {
    return (FALSE);          // 返回FALSE
  }

  pCTData = MALLOC(COMPLEX, nTransWidth * nTransHeight);   // 分配内存
  pCFData = MALLOC(COMPLEX, nTransWidth * nTransHeight);   // 分配内存

  // 图像数据的宽和高不一定是2的整数次幂，所以pCTData有一部分数据需要补0
  for (y = 0; y < nTransHeight; y++) {
    for (x = 0; x < nTransWidth; x++) {
      CplexSet(0, 0, pCTData[ y * nTransWidth + x ]);    // 补零
    }
  }

  //把图像数据传给pCTData
  for (y = 0; y < lHeight; y++) {
    for (x = 0; x < lWidth; x++) {
      // 指向DIB第y行，第x个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * (lHeight - 1 - y) + x;
      unchValue = *lpSrc;
      CplexSet(unchValue, 0, pCTData[ y * nTransWidth + x ]);
    }
  }

  Fft2D(pCTData, lWidth, lHeight, pCFData);       // 傅立叶正变换

  for (x = 0; x < nTransWidth; x++) {       //开始实施理想的低通滤波
    for (y = nRadius2; y < nTransHeight - nRadius2; y++) {
      // 把纵向所有大于nRadius2的高频分量设置为0
      CplexSet(0, 0, pCFData[ x * nTransHeight + y ]);
    }
  }

  for (x = nRadius1; x < nTransWidth - nRadius1; x++) {
    for (y = 0; y < nTransHeight; y++) {
      // 把横向所有大于nRadius1的高频分量设置为0
      CplexSet(0, 0, pCFData[ x * nTransHeight + y ]);
    }
  }

  IFFT_2D(pCFData, pCTData, lHeight, lWidth);       // 经过低通滤波的图象进行反变换

  for (y = 0; y < lHeight; y++) {         // 反变换的数据传给lpDIBBits
    for (x = 0; x < lWidth; x++) {
      //需要考虑信号的正负问题以及实际所用的图象数据是灰度值还是原始数据
      dReal = CREAL(pCTData[ y * nTransWidth + x ]);     // 实部
      dImag = CIMAG(pCTData[ y * nTransWidth + x ]);    // 虚部
      unchValue = dReal;
      // 指向DIB第y行，第x个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * (lHeight - 1 - y) + x;
      *lpSrc = (BYTE) unchValue;
    }
  }

  SAFEFREE(pCTData);            // 释放内存
  SAFEFREE(pCFData);            // 释放内存
  pCTData = NULL;
  pCFData = NULL;
  return (TRUE);            //返回结果
}


/*************************************************************************
* \函数名称：
*   ButterWorthLowPass()
* \输入参数:
*  LPBYTE lpDIBBits     - 指向需要滤波的图像像素指针
*   int lWidth      - 数据宽度
*   int lHeight      - 数据高度
*   int nRadius      - ButterWorth低通滤波的"半功率"点
* \返回值:
*    BOOL       // 成功返回TRUE，否则返回FALSE。
* \说明:
*  pDIBBits 是指向需要滤波的图像像素指针。
*  经过ButterWorth低通滤波的数据仍然存储在lpDIBBits 当中。
 **************************************************************************/
BOOL ButterWorthLowPass1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nRadius)
{
  LPBYTE lpSrc;       // 指向源图像的指针
  int y;          // 循环控制变量
  int x;          // 循环控制变量
  Float dTmpOne;        //存放临时变量
  Float dTmpTwo;        //存放临时变量
  Float H;          // ButterWorth 滤波系数
  int nTransWidth;        // 傅立叶变换的宽度（2的整数次幂）
  int nTransHeight;        // 傅立叶变换的高度（2的整数次幂）
  Float dReal;         // 傅立叶变换的实部
  Float dImag;         // 傅立叶变换的虚部
  Float unchValue;        // 存贮图像各像素灰度的临时变量
  COMPLEX* pCTData;      // 指向时域数据的指针
  COMPLEX* pCFData;      // 指向频域数据的指针
  // 计算进行傅立叶变换的点数－横向 （2的整数次幂）
  dTmpOne = log(lWidth) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransWidth = (int) dTmpTwo;
  // 计算进行傅立叶变换的点数－纵向 （2的整数次幂）
  dTmpOne = log(lHeight) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransHeight = (int) dTmpTwo;
  pCTData = MALLOC(COMPLEX, nTransWidth * nTransHeight);   // 分配内存
  pCFData = MALLOC(COMPLEX, nTransWidth * nTransHeight);   // 分配内存

  // 图像数据的宽和高不一定是2的整数次幂，所以pCTData有一部分数据需要补0
  for (y = 0; y < nTransHeight; y++) {
    for (x = 0; x < nTransWidth; x++) {
      CplexSet(0, 0, pCTData[ y * nTransWidth + x ]);   // 补零
    }
  }

  for (y = 0; y < lHeight; y++) {   // 把图像数据传给pCTData
    for (x = 0; x < lWidth; x++) {
      // 指向DIB第y行，第x个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * (lHeight - 1 - y) + x;
      unchValue = *lpSrc;
      CplexSet(unchValue, 0, pCTData[ y * nTransWidth + x ]);
    }
  }

  Fft2D(pCTData, lWidth, lHeight, pCFData);     // 傅立叶正变换

  for (y = 0; y < nTransHeight; y++) {   // 开始实施ButterWorth低通滤波
    for (x = 0; x < nTransWidth; x++) {
      H = (Float)(y * y + x * x);
      H = H / (nRadius * nRadius);
      H = 1 / (1 + H);         // 求H值
      CplexSet(CREAL(pCFData[ y * nTransWidth + x ]) * H,
          CIMAG(pCFData[ y * nTransWidth + x ]) * H, pCFData[ y * nTransWidth + x ]);
    }
  }

  // 经过ButterWorth低通滤波的图象进行反变换
  IFFT_2D(pCFData, pCTData, lWidth, lHeight);

  for (y = 0; y < lHeight; y++) {        // 反变换的数据传给lpDIBBits
    for (x = 0; x < lWidth; x++) {
      dReal = CREAL(pCTData[ y * nTransWidth + x ]);
      dImag = CIMAG(pCTData[ y * nTransWidth + x ]);
      unchValue = MAX(0, MIN(255, sqrt(dReal * dReal + dImag * dImag)));
      // 指向DIB第y行，第x个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * (lHeight - 1 - y) + x;
      *lpSrc = (BYTE) unchValue;
    }
  }

  SAFEFREE(pCTData);           // 释放内存
  SAFEFREE(pCFData);           // 释放内存
  return (TRUE);           //返回结果
}

/*************************************************************************
* \函数名称：
*   HighPassFilter()
* \输入参数:
*   LPBYTE lpDIBBits     - 指向需要滤波的图像像素指针
*   int lWidth      - 数据宽度
*   int lHeight      - 数据高度
*   int nRadius1      - 理想高通滤波的横向滤波半径
*  int nRadius2      - 理想高通滤波的纵向滤波半径
* \返回值:
*    BOOL       // 成功返回TRUE，否则返回FALSE。
* \说明:
*  lpDIBBits 是指向需要滤波的图像像素指针。
*  经过高通滤波的数据仍然存储在lpDIBBits 当中。
**************************************************************************/
BOOL HighPassFilter(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nRadius1, int nRadius2)
{
  LPBYTE lpSrc;       // 指向源图像的指针
  int y;          // 循环控制变量
  int x;          // 循环控制变量
  Float dTmpOne;        //存放临时变量
  Float dTmpTwo;        //存放临时变量
  int nTransWidth;        // 傅立叶变换的宽度（2的整数次幂）
  int nTransHeight;        // 傅立叶变换的高度（2的整数次幂）
  Float unchValue;        // 存贮图像各像素灰度的临时变量
  COMPLEX* pCTData;      // 指向时域数据的指针
  COMPLEX* pCFData;      // 指向频域数据的指针
  Float dReal;         // 傅立叶变换的实部
  Float dImag;         // 傅立叶变换的虚部
  // 计算进行傅立叶变换的点数－横向 （2的整数次幂）
  dTmpOne = log(lWidth) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransWidth = (int) dTmpTwo;
  // 计算进行傅立叶变换的点数－纵向 （2的整数次幂）
  dTmpOne = log(lHeight) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransHeight = (int) dTmpTwo;

  // 滤波的半径不能超过频域的最大半径
  if (nRadius1 > nTransWidth - 1 || nRadius2 > nTransHeight - 1) {
    return (FALSE);          // 返回FALSE
  }

  pCTData = MALLOC(COMPLEX, nTransWidth * nTransHeight);   // 分配内存
  pCFData = MALLOC(COMPLEX, nTransWidth * nTransHeight);   // 分配内存

  // 图像数据的宽和高不一定是2的整数次幂，所以pCTData有一部分数据需要补0
  for (y = 0; y < nTransHeight; y++) {
    for (x = 0; x < nTransWidth; x++) {
      CplexSet(0, 0, pCTData[ y * nTransWidth + x ]);   // 补零
    }
  }

  for (y = 0; y < lHeight; y++) {   // 把图像数据传给pCTData
    for (x = 0; x < lWidth; x++) {
      // 指向DIB第y行，第x个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * (lHeight - 1 - y) + x;
      unchValue = *lpSrc;
      CplexSet(unchValue, 0, pCTData[ y * nTransWidth + x ]);
    }
  }

  Fft2D(pCTData, lWidth, lHeight, pCFData);      // 傅立叶正变换

  // 下面开始滤波，把所有小于nRadius的低频分量设置为0
  // 采用的范数不是欧式距离，而是无穷大范数
  //  || (u, v)-(0, 0) || = max(|u|, |v|)
  for (x = 0; x < nTransWidth; x++) {   // 开始实施理想的高通滤波
    for (y = 0; y < nRadius2; y++) {
      //把纵向所有小于nRadius2的低频分量设置为0
      CplexSet(0, 0, pCFData[ x * nTransHeight + y ]);
    }

    for (y = nTransHeight - 1 - nRadius2; y < nTransHeight; y++) {
      CplexSet(0, 0, pCFData[ x * nTransHeight + y ]);
    }
  }

  for (x = 0; x < nRadius1; x++) {
    //把横向所有小于nRadius1的低频分量设置为0
    for (y = 0; y < nTransHeight; y++) {
      CplexSet(0, 0, pCFData[ x * nTransHeight + y ]);
    }
  }

  for (x = nTransWidth - nRadius1; x < nTransWidth; x++) {
    for (y = 0; y < nTransHeight; y++) {
      CplexSet(0, 0, pCFData[ x * nTransHeight + y ]);
    }
  }

  IFFT_2D(pCFData, pCTData, lHeight, lWidth);     // 经过高通滤波的图象进行反变换

  // 反变换的数据传给lpDIBBits
  for (y = 0; y < lHeight; y++) {
    for (x = 0; x < lWidth; x++) {
      // 因为高通滤波器会把低频成分去掉，所以图象看起来会比较暗，为了解决这个问题
      // 在经过增强后的图象加上一个常数，使图象变得亮起来。严格来讲这种处理相当于
      // 增加了一些低频分量（在频率(0, 0)）。不过如果不加一个常数，高通效果在观看
      // 上就不是很方便。
      // 需要考虑信号的正负问题以及实际所用的图象数据是灰度值还是原始数据
      dReal = CREAL(pCTData[ y * nTransWidth + x ]);
      dImag = CIMAG(pCTData[ y * nTransWidth + x ]);
      unchValue = dReal;
      // 指向DIB第y行，第x个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * (lHeight - 1 - y) + x;
      *lpSrc = (BYTE) unchValue;
    }
  }

  SAFEFREE(pCTData);            // 释放内存
  SAFEFREE(pCFData);            // 释放内存
  return (TRUE);            // 返回结果
}

/*************************************************************************
* \函数名称：
*   ButterWorthHighPass()
* \输入参数:
*  LPBYTE lpDIBBits     - 指向需要滤波的图像像素指针
*   int lWidth       - 数据宽度
*   int lHeight      - 数据高度
*   int nRadius            - ButterWorth高通滤波的"半功率"点
* \返回值:
*    BOOL       // 成功返回TRUE，否则返回FALSE。
* \说明:
*   lpImage 是指向需要增强的数据指针。注意，这个指针指向的数据区不能是
*     CDib指向的数据区， 因为CDib指向的数据区的每一行是DWORD对齐的。
*   经过ButterWorth高通滤波的数据存储在lpImage当中。
 **************************************************************************/
BOOL ButterWorthHighPass1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nRadius)
{
  LPBYTE lpSrc;       // 指向源图像的指针
  int x, y;           // 循环控制变量
  Float dTmpOne, dTmpTwo; //存放临时变量
  Float H;                // ButterWorth 滤波系数
  int nTransWidth;         // 傅立叶变换的宽度（2的整数次幂）
  int nTransHeight;        // 傅立叶变换的高度（2的整数次幂）
  Float dReal;            // 傅立叶变换的实部
  Float dImag;            // 傅立叶变换的虚部
  Float unchValue;        // 存贮图像各像素灰度的临时变量
  COMPLEX* pCTData;       // 指向时域数据的指针
  COMPLEX* pCFData;       // 指向频域数据的指针
  // 计算进行傅立叶变换的点数－横向（2的整数次幂）
  dTmpOne = log(lWidth) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransWidth = (int) dTmpTwo;
  // 计算进行傅立叶变换的点数－纵向 （2的整数次幂）
  dTmpOne = log(lHeight) / log(2);
  dTmpTwo = ceil(dTmpOne);
  dTmpTwo = pow(2, dTmpTwo);
  nTransHeight = (int) dTmpTwo;
  pCTData = MALLOC(COMPLEX, nTransWidth * nTransHeight);      // 分配内存
  pCFData = MALLOC(COMPLEX, nTransWidth * nTransHeight);      // 分配内存

  // 图像数据的宽和高不一定是2的整数次幂，所以pCTData有一部分数据需要补0
  for (y = 0; y < nTransHeight; y++) {
    for (x = 0; x < nTransWidth; x++) {
      CplexSet(0, 0, pCTData[ y * nTransWidth + x ]);      // 补零
    }
  }

  for (y = 0; y < lHeight; y++) {                         // 把图像数据传给pCTData
    for (x = 0; x < lWidth; x++) {
      // 指向DIB第y行，第x个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * (lHeight - 1 - y) + x;
      unchValue = *lpSrc;
      CplexSet(unchValue, 0, pCTData[ y * nTransWidth + x ]);
    }
  }

  Fft2D(pCTData, lWidth, lHeight, pCFData);          // 傅立叶正变换

  for (y = 0; y < nTransHeight; y++) {                     // 下面开始实施ButterWorth高通滤波
    for (x = 0; x < nTransWidth; x++) {
      H = (Float)(y * y + x * x);
      H = (nRadius * nRadius) / H;
      H = 1 / (1 + H);                          // 求H值
      CplexSet(H * (CREAL(pCFData[ y * nTransWidth + x ])),
          H * (CIMAG(pCFData[ y * nTransWidth + x ])),
          pCFData[ y * nTransWidth + x ]);
    }
  }

  // 经过ButterWorth高通滤波的图象进行反变换
  IFFT_2D(pCFData, pCTData, lWidth, lHeight);

  // 反变换的数据传给lpDIBBits
  for (y = 0; y < lHeight; y++) {
    for (x = 0; x < lWidth; x++) {
      dReal = CREAL(pCTData[ y * nTransWidth + x ]);
      dImag = CIMAG(pCTData[ y * nTransWidth + x ]);
      unchValue = MAX(0, MIN(255, sqrt(dReal * dReal + dImag * dImag) + 100));
      // 指向DIB第y行，第x个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * (lHeight - 1 - y) + x;
      *lpSrc = (BYTE) unchValue;
    }
  }

  SAFEFREE(pCTData);                                      // 释放内存
  SAFEFREE(pCFData);                                      // 释放内存
  return (TRUE);                                      // 返回结果
}

