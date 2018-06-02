// 小波变换的函数

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "macro.h"
#include "imglib.h"

#define ASSERT assert

// 小波变换函数原型
// Daubechies紧致正交小波基
// 不同支撑区间长度下的滤波器系数如下
const Float hCoef[ 10 ][ 20 ] = { //
  {
    .707106781187, .707106781187
  },

  { .482962913145, .836516303738, .224143868042, -.129409522551 },

  { .332670552950, .806891509311, .459877502118, -.135011020010, -.085441273882, .035226291882 },

  {
    .230377813309, .714846570553, .630880767930, -.027983769417,
    -.187034811719, .030841381836, .032883011667, -.010597401785
  },

  {
    .160102397974, .603829269797, .724308528438, .138428145901, -.242294887066,
    -.032244869585, .077571493840, -.006241490213, -.012580751999, .003335725285
  },

  {
    .111540743350, .494623890398, .751133908021, .315250351709, -.226264693965,
    -.129766867567, .097501605587, .027522865530, -.031582039318, .000553842201,
    .004777257511, -.001077301085
  },

  {
    .077852054085, .396539319482, .729132090846, .469782287405, -.143906003929,
    -.224036184994, .071309219267, .080612609151, -.038029936935, -.016574541631,
    .012550998556, .000429577973, -.001801640704, .000353713800
  },

  {
    .054415842243, .312871590914, .675630736297, .585354683654, -.015829105256,
    -.284015542962, .000472484574, .128747426620, -.017369301002, -.044088253931,
    .013981027917, .008746094047, -.004870352993, -.000391740373, .000675449406,
    -.000117476784
  },

  {
    .038077947364, .243834674613, .604823123690, .657288078051, .133197385825,
    -.293273783279, -.096840783223, .148540749338, .030725681479, -.067632829061,
    .000250947115, .022361662124, -.004723204758, -.004281503682, .001847646883,
    .000230385764, -.000251963189, .000039347320
  },

  {
    .026670057901, .188176800078, .527201188932, .688459039454, .281172343661,
    -.249846424327, -.195946274377, .127369340336, .093057364604, -.071394147166,
    -.029457536822, .033212674059, .003606553567, -.010733175483, .001395351747,
    .001992405295, -.000685856695, -.000116466855, .000093588670, -.000013264203
  }
};

/*************************************************************************
 *\函数名称：
 *  FloatToChar()
 *
 *\输入参数:
 *  Float  f            - 输入双精度变量
 *
 *\返回值:
 *  Char                 - 返回字符变量
 *
 *\说明:
 *  该函数将输入的双精度变量转换为Char型的变量
 *************************************************************************
 */
char FloatToChar(Float f)
{
  if (f >= 0)
    if (f >= 127.0) {
      return (char) 127;
    }
    else {
      return (char)(f + 0.5);
    }
  else if (f <= -128) {
    return (char) - 128;
  }
  else {
    return -(char)(-f + 0.5);
  }
}

/*************************************************************************
 *\函数名称：
 *  FloatToByte()
 *
 *\输入参数:
 *  Float  f            - 输入双精度变量
 *
 *\返回值:
 *  BYTE            - 返回比特型变量
 *
 *\说明:
 *  该函数将输入的双精度变量转换为BYTE型的变量
 *************************************************************************
 */
BYTE FloatToByte(Float f)
{
  if (f <= 0) {
    return (BYTE) 0;
  }
  else if (f >= 255) {
    return (BYTE) 255;
  }
  else {
    return (BYTE)(f + 0.5);
  }
}

/*************************************************************************
 *
 *\函数名称：
 *  DWTStep_1D()
 *
 *\输入参数:
 *  Float *pDbSrc  - 指向源数据的指针
 *  int nCurLevel  - 当前分界的层数
 *  int nInv    - 是否为DWT，1表示为IDWT，0表示DWT
 *  int nStep   - 当前的计算层数
 *  int nSupp   - 小波基的紧支集的长度
 *
 *\返回值:
 *  BOOL   - 成功则返回TRUE，否则返回FALSE
 *
 *\说明:
 *  该函数用对存放在pDBSrc中的数据进行一层的一维DWT或者IDWT。其中，nInv为表示进行
 *  DWT或者IDWT的标志。nCurLevel为当前需要进行分界的层数。nStep为已经分界的层数
 *  计算后数据仍存放在pDbSrc中
 *
 *************************************************************************
 */
BOOL DWTStep_1D(Float* pDbSrc, int nCurLevel, int nInv, int nStep, int nSupp)
{
  int i, j, Index1, Index2;
  Float* ptemp, s1, s2, s = sqrt(2);
  // 获得小波基的指针
  Float* h = (Float*) hCoef[ nSupp - 1 ];

  // 计算当前层数的长度
  int CurN = 1 << nCurLevel;

  if (nInv) {
    CurN <<= 1;
  }

  // 确认当前层数有效
  ASSERT(nCurLevel >= 0);

  // 确认所选择的小波基和当前层数的长度有效
  if (nSupp < 1 || nSupp > 10 || CurN < 2 * nSupp) {
    return FALSE;
  }

  // 分配临时内存用于存放结果
  ptemp = MALLOC(Float, CurN);

  if (!ptemp) {
    return FALSE;
  }


  // 判断是进行DWT还是IDWT
  if (!nInv) {   // DWT
    Index1 = 0;
    Index2 = 2 * nSupp - 1;

    // 进行卷积，其中s1为低频部分，s2为高频部分的结果
    for (i = 0; i < CurN / 2; i++) {
      Float t = -1;
      s1 = s2 = 0;

      for (j = 0; j < 2 * nSupp; j++, t = -t) {
        s1 += h[ j ] * pDbSrc[(Index1 & CurN - 1) * nStep ];
        s2 += t * h[ j ] * pDbSrc[(Index2 & CurN - 1) * nStep ];

        Index1++;
        Index2--;
      }

      // 将结果存放在临时内存中
      ptemp[ i ] = s1 / s;
      ptemp[ i + CurN / 2 ] = s2 / s;

      Index1 -= 2 * nSupp;
      Index2 += 2 * nSupp;
      Index1 += 2;
      Index2 += 2;
    }
  }

  // 否则进行IDWT
  else { // IDWT
    Index1 = CurN / 2;
    Index2 = CurN / 2 - nSupp + 1;

    // 进行卷积，其中其中s1为低频部分，s2为高频部分的结果
    for (i = 0; i < CurN / 2; i++) {
      int Index3 = 0;
      s1 = s2 = 0;

      for (j = 0; j < nSupp; j++) {
        double sss = pDbSrc[(Index1 & CurN / 2 - 1) * nStep ];
        s1 += h[ Index3 ] * pDbSrc[(Index1 & CurN / 2 - 1) * nStep ]
            + h[ Index3 + 1 ] * pDbSrc[((Index2 + CurN / 2) & CurN / 2 - 1) * nStep ];
        s2 += h[ Index3 + 1 ] * pDbSrc[(Index1 & CurN / 2 - 1) * nStep ]
            - h[ Index3 ] * pDbSrc[((Index2 + CurN / 2) & CurN / 2 - 1) * nStep ];

        Index3 += 2;
        Index1--;
        Index2++;
      }

      // 将结果存入临时内存
      ptemp[ 2 * i ] = s1 * s;
      ptemp[ 2 * i + 1 ] = s2 * s;

      Index1 += nSupp;
      Index2 -= nSupp;
      Index1++;
      Index2++;
    }
  }

  // 将结果存入源图象中
  for (i = 0; i < CurN; i++) {
    pDbSrc[ i * nStep ] = ptemp[ i ];
  }

  // 释放临时内存，并返回
  SAFEFREE(ptemp);
  return TRUE;
}

/*************************************************************************
 *
 *\函数名称：
 *  DWTStep_2D()
 *
 *\输入参数:
 *  Float *pDbSrc  - 指向源数据的指针
 *  int nCurWLevel  - X方向上当前分解的层数
 *  int nCurHLevel  - Y方向上当前分解的层数
 *  int nMaxWLevel  - X方向上最大可分解的层数
 *  int nMaxHLevel  - Y方向上最大可分解的层数
 *  int nInv   - 是否为DWT，1表示为IDWT，0表示DWT
 *  int nStep   - 当前的计算层数
 *  int nSupp   - 小波基的紧支集的长度
 *
 *\返回值:
 *  BOOL   - 成功则返回TRUE，否则返回FALSE
 *
 *\说明:
 *  该函数用对存放在pDBSrc中的数据进行一层的二维DWT或者IDWT。
 *  计算后数据仍存放在pDbSrc中
 *
 *************************************************************************
 */
BOOL DWTStep_2D(Float* pDbSrc, int nCurWLevel, int nCurHLevel,
    int nMaxWLevel, int nMaxHLevel, int nInv, int nStep, int nSupp)
{
  // 计算图象的长度和宽度（2次幂对齐）
  int i, W = 1 << nMaxWLevel, H = 1 << nMaxHLevel;

  // 计算当前分解的图象的长度和宽度
  int CurW = 1 << nCurWLevel, CurH = 1 << nCurHLevel;

  // 判断是进行DWT还是IDWT
  if (!nInv) {   // 对行进行一维DWT
    for (i = 0; i < CurH; i++)
      if (!DWTStep_1D(pDbSrc + (int) i * W * nStep, nCurWLevel, nInv, nStep, nSupp)) {
        return FALSE;
      }

    // 对列进行一维DWT
    for (i = 0; i < CurW; i++)
      if (!DWTStep_1D(pDbSrc + i * nStep, nCurHLevel, nInv, W * nStep, nSupp)) {
        return FALSE;
      }
  }

  // 否则进行IDWT
  else {
    // 计算当前变换的图象的长度和宽度
    CurW <<= 1;
    CurH <<= 1;

    // 对列进行IDWT
    for (i = 0; i < CurW; i++)
      if (!DWTStep_1D(pDbSrc + i * nStep, nCurHLevel, nInv, W * nStep, nSupp)) {
        return FALSE;
      }

    // 对行进行IDWT
    for (i = 0; i < CurH; i++)
      if (!DWTStep_1D(pDbSrc + (int) i * W * nStep, nCurWLevel, nInv, nStep, nSupp)) {
        return FALSE;
      }
  }

  // 返回
  return TRUE;
}

/*************************************************************************
 *\函数名称：
 *  DWT_1D()
 *
 *\输入参数:
 *  Float *pDbSrc  - 指向源数据的指针
 *  int nMaxLevel  - 最大可分解的层数
 *  int nDWTSteps  - 需要分界的层数
 *  int nInv   - 是否为DWT，1表示为IDWT，0表示DWT
 *  int nStep   - 当前的计算层数
 *  int nSupp   - 小波基的紧支集的长度
 *
 *\返回值:
 *  BOOL   - 成功则返回TRUE，否则返回FALSE
 *
 *\说明:
 *  该函数用对存放在pDBSrc中的数据进行一维DWT或者IDWT。其中，nInv为表示进行
 *  DWT或者IDWT的标志。nStep为当前已经分界的层数。计算后数据仍存放在pDbSrc中
 **************************************************************************/
BOOL DWT_1D(Float* pDbSrc, int nMaxLevel,
    int nDWTSteps, int nInv, int nStep, int nSupp)
{
  // 计算最小可分界的层数
  int MinLevel = nMaxLevel - nDWTSteps;

  // 判断是否为DWT
  if (!nInv) {   // DWT
    int n = nMaxLevel;

    while (n > MinLevel)

      // 调用DWTStep_1D进行第n层的DWT
      if (!DWTStep_1D(pDbSrc, n--, nInv, nStep, nSupp)) {
        return FALSE;
      }
  }

  // nInv为1则进行IDWT
  else { // IDWT
    int n = MinLevel;

    while (n < nMaxLevel)

      // 调用DWTStep_1D进行第n层的IDWT
      if (!DWTStep_1D(pDbSrc, n++, nInv, nStep, nSupp)) {
        return FALSE;
      }
  }

  return TRUE;
}

/*************************************************************************
 *
 *\函数名称：
 *  DWT_2D()
 *
 *\输入参数:
 *  Float *pDbSrc  - 指向源数据的指针
 *  int nMaxWLevel  - X方向最大可分解的层数
 *  int nMaxHLevel  - Y方向最大可分解的层数
 *  int nDWTSteps  - 需要分界的层数
 *  int nInv   - 是否为DWT，1表示为IDWT，0表示DWT
 *  int nStep   - 当前的计算层数
 *  int nSupp   - 小波基的紧支集的长度
 *
 *\返回值:
 *  BOOL   - 成功则返回TRUE，否则返回FALSE
 *
 *\说明:
 *  该函数用对存放在pDBSrc中的二维数据进行二维DWT或者IDWT。其中，nDWTSteps表示
 *  需要分解的层数，nInv为表示进行DWT或者IDWT的标志。nStep为当前已经分界的层数
 *  计算后数据仍存放在pDbSrc中
 *
 *************************************************************************
 */
BOOL DWT_2D(Float* pDbSrc, int nMaxWLevel, int nMaxHLevel,
    int nDWTSteps, int nInv, int nStep, int nSupp)
{
  // 计算X，Y方向上最小的分界层数
  int MinWLevel = nMaxWLevel - nDWTSteps;
  int MinHLevel = nMaxHLevel - nDWTSteps;

  // 判断是进行DWT，还是IDWT
  if (!nInv) {   // DWT
    int n = nMaxWLevel, m = nMaxHLevel;

    // 调用DWTStep_2D进行分解，分解的层数为nDWTSteps
    while (n > MinWLevel)
      if (!DWTStep_2D(pDbSrc, n--, m--, nMaxWLevel, nMaxHLevel, nInv, nStep, nSupp)) {
        return FALSE;
      }
  }

  // 否则进行IDWT
  else { // IDWT
    int n = MinWLevel, m = MinHLevel;

    // 调用DWTStep_2D进行IDWT，进行恢复的层数为nDWTSteps
    while (n < nMaxWLevel)
      if (!DWTStep_2D(pDbSrc, n++, m++, nMaxWLevel, nMaxHLevel, nInv, nStep, nSupp)) {
        return FALSE;
      }
  }

  // 返回
  return TRUE;
}

/*************************************************************************
 * \函数名称：  Log2()
 * \输入参数:
 *   int n            - 输入整型变量
 *
 * \返回值:
 *  int           - 返回输入参数的对数
 *
 * \说明:
 *    该函数求取输入参数的以2为底的对数，并转换为整型输出。
 *************************************************************************
 */
int Log2(int n)
{
  int rsl = 0;

  while (n >>= 1) {
    rsl++;
  }

  return rsl;
}

BOOL DIBDWTStep(LPBYTE lpDIBBits, Float* m_pDbImage, int lWidth, int lHeight, int nInv, int m_nDWTCurDepth, int m_nSupp)
{
  // 循环变量
  int i, j;

  // 获取变换的最大层数
  int nMaxWLevel = Log2(lWidth);
  int nMaxHLevel = Log2(lHeight);
  int nMaxLevel, lfw, lfh;
  // 临时变量
  Float* pDbTemp;
  BYTE* pBits;

  if (lWidth == 1 << nMaxWLevel && lHeight == 1 << nMaxHLevel) {
    nMaxLevel = MIN(nMaxWLevel, nMaxHLevel);
  }

  // 如果小波变换的存储内存还没有分配，则分配此内存
  if (!m_pDbImage) {
    m_pDbImage = MALLOC(Float, lWidth * lHeight);

    if (!m_pDbImage) {
      return FALSE;
    }

    // 将图象数据放入m_pDbImage中
    for (j = 0; j < lHeight; j++) {
      pDbTemp = m_pDbImage + j * lWidth;
      pBits = (LPBYTE) lpDIBBits + (lHeight - 1 - j) * lWidth;

      for (i = 0; i < lWidth; i++) {
        pDbTemp[ i ] = pBits[ i ];
      }
    }
  }

  // 进行小波变换（或反变换）
  if (!DWTStep_2D(m_pDbImage, nMaxWLevel - m_nDWTCurDepth, nMaxHLevel - m_nDWTCurDepth,
      nMaxWLevel, nMaxHLevel, nInv, 1, m_nSupp)) {
    return FALSE;
  }

  // 如果是反变换，则当前层数减1
  if (nInv) {
    m_nDWTCurDepth --;
  }
  // 否则加1
  else {
    m_nDWTCurDepth ++;
  }

  // 然后，将数据拷贝回原CDib中，并进行相应的数据转换
  lfw = lWidth >> m_nDWTCurDepth;
  lfh = lHeight >> m_nDWTCurDepth;

  for (j = 0; j < lHeight; j++) {
    pDbTemp = m_pDbImage + j * lWidth;
    pBits = (LPBYTE) lpDIBBits + (lHeight - 1 - j) * lWidth;

    for (i = 0; i < lWidth; i++) {
      if (j < lfh && i < lfw) {
        pBits[ i ] = FloatToByte(pDbTemp[ i ]);
      }
      else {
        pBits[ i ] = (BYTE)(FloatToChar(pDbTemp[ i ]) ^ 0x80);
      }
    }
  }

  // 返回
  return TRUE;
}


/*************************************************************************
 *
 * \函数名称：
 *    ImageDWT()
 *
 * \输入参数:
 *    CDib *pDibSrc  - 指向源数据的指针
 *    int nMaxWLevel  - X方向上最大可分解的层数
 *    int nMaxHLevel  - Y方向上最大可分解的层数
 *    int nDWTSteps  - 需要进行变换的层数
 *    int nInv   - 是否为DWT，1表示为IDWT，0表示DWT
 *    int nStep   - 当前的计算层数
 *    int nSupp   - 小波基的紧支集的长度
 *
 * \返回值:
 *    BOOL   - 成功则返回TRUE，否则返回FALSE
 *
 * \说明:
 *    该函数用对存放在pDBSrc中的数据进行一层的二维DWT或者IDWT。
 *    计算后数据仍存放在pDbSrc中
 *
 **************************************************************************/
BOOL ImageDWT(LPBYTE lpImage, int nMaxWLevel, int nMaxHLevel,
    int nDWTSteps, int nInv, int nStep, int nSupp)
{
  // 获得X，Y方向上的最大象素数（2次幂对齐）
  int W = 1 << nMaxWLevel, H = 1 << nMaxHLevel;

  // 获得X，Y方向上变换时最小的象素数
  int minW = W >> nDWTSteps, minH = H >> nDWTSteps;

  int i, j, index;

  // 分配临时内存存放结果
  Float* pDbTemp;

  // 判断变换的层数以及当前层数是否有效
  if (nDWTSteps > nMaxWLevel || nDWTSteps > nMaxHLevel || nStep <= 0) {
    return FALSE;
  }

  pDbTemp = MALLOC(Float, W * H);

  if (!pDbTemp) {
    return FALSE;
  }

  // 判断是进行DWT还是IDWT，然后将数据存放到临时内存中，需要注意的是，需要进行采样
  if (!nInv)    // DWT
    for (index = 0; index < W * H; index++) {
      pDbTemp[ index ] = lpImage[ index * nStep ];
    }
  else { // IDWT
    index = 0;

    for (i = 0; i < minH; i++) {
      for (j = 0; j < minW; j++, index++) {
        pDbTemp[ index ] = lpImage[ index * nStep ];
      }

      for (; j < W; j++, index++) {
        pDbTemp[ index ] = (char) lpImage[ index * nStep ];
      }
    }

    for (; index < W * H; index++) {
      pDbTemp[ index ] = (char) lpImage[ index * nStep ];
    }
  }

  // 调用DWT_2D进行小波变换
  if (!DWT_2D(pDbTemp, nMaxWLevel, nMaxHLevel, nDWTSteps, nInv, nStep, nSupp)) {
    SAFEFREE(pDbTemp);
    return FALSE;
  }

  // 将数据存入原始的内存中，需要注意的是，存储时需要进行类型转换
  if (!nInv) {   // DWT
    index = 0;

    for (i = 0; i < minH; i++) {
      for (j = 0; j < minW; j++, index++) {
        lpImage[ index * nStep ] = FloatToByte(pDbTemp[ index ]);
      }

      for (; j < W; j++, index++) {
        lpImage[ index * nStep ] = (BYTE) FloatToChar(pDbTemp[ index ]);
      }

      // lpImage[index*nStep] = (BYTE)FloatToByte(pDbTemp[index]);
    }

    for (; index < W * H; index++) {
      lpImage[ index * nStep ] = (BYTE) FloatToChar(pDbTemp[ index ]);
    }

    //lpImage[index*nStep] = (BYTE)FloatToByte(pDbTemp[index]);
  }
  else    // IDWT
    for (index = 0; index < W * H; index++) {
      lpImage[ index * nStep ] = FloatToByte(pDbTemp[ index ]);
    }

  // 释放内存
  SAFEFREE(pDbTemp);

  // 返回
  return TRUE;
}

// int lHeight, int lWidth 图象的长度和宽度
BOOL DWTStep(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nInv)
{
  // 循环变量
  int i, j;
  // 临时变量
  double* m_pDbImage = NULL, * pDbTemp;
  BYTE* pBits;
  int lfw, lfh;
  int m_nSupp;
  int m_nDWTCurDepth;
  int lDbLineBytes;

  // 获取变换的最大层数
  int nMaxWLevel = Log2(lWidth);
  int nMaxHLevel = Log2(lHeight);
  int nMaxLevel;

  if (lWidth == 1 << nMaxWLevel && lHeight == 1 << nMaxHLevel) {
    nMaxLevel = MIN(nMaxWLevel, nMaxHLevel);
  }

  // 设置当前层数
  m_nDWTCurDepth = 0;

  // 设置小波基紧支集长度
  m_nSupp = 1;

  // 如果小波变换的存储内存还没有分配，则分配此内存
  //if(!m_pDbImage)
  {
    if (nInv) {
      lDbLineBytes = 2 * lWidth;
      m_pDbImage = MALLOC(double, 4 * lWidth * lHeight);
    }
    else {
      lDbLineBytes = lWidth;
      m_pDbImage = MALLOC(double, lWidth * lHeight);
    }

    if (!m_pDbImage) {
      return FALSE;
    }

    // 将图象数据放入m_pDbImage中
    for (j = 0; j < lHeight; j++) {
      pDbTemp = m_pDbImage + j * lDbLineBytes;
      pBits = lpDIBBits + (lHeight - 1 - j) * lLineBytes;

      for (i = 0; i < lWidth; i++) {
        pDbTemp[ i ] = pBits[ i ];
      }
    }
  }

  // 进行小波变换（或反变换）
  if (!DWTStep_2D(m_pDbImage, nMaxWLevel - m_nDWTCurDepth, nMaxHLevel - m_nDWTCurDepth,
      nMaxWLevel, nMaxHLevel, nInv, 1, m_nSupp)) {
    return FALSE;
  }

  // 如果是反变换，则当前层数减1
  if (nInv) {
    m_nDWTCurDepth --;
  }
  // 否则加1
  else {
    m_nDWTCurDepth ++;
  }

  // 然后，将数据拷贝回原CDib中，并进行相应的数据转换
  lfw = lWidth >> m_nDWTCurDepth;
  lfh = lHeight >> m_nDWTCurDepth;

  for (j = 0; j < lHeight; j++) {
    pDbTemp = m_pDbImage + j * lDbLineBytes;
    pBits = lpDIBBits + (lHeight - 1 - j) * lLineBytes;

    for (i = 0; i < lWidth; i++) {
      if (j < lfh && i < lfw) {
        pBits[ i ] = FloatToByte(pDbTemp[ i ]);
      }
      else {
        pBits[ i ] = (BYTE)(FloatToChar(pDbTemp[ i ]) ^ 0x80);
      }
    }
  }

  SAFEFREE(m_pDbImage);
  // 返回
  return TRUE;
}
