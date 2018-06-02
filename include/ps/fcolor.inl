
#define   PCL_B(p)   (((BYTE*)(p))[0])
#define   PCL_G(p)   (((BYTE*)(p))[1])
#define   PCL_R(p)   (((BYTE*)(p))[2])
#define   PCL_A(p)   (((BYTE*)(p))[3])


#ifndef FMin
#define FMin(a,b)  ((a) > (b) ? (b) : (a))
#endif

#ifndef FMax
#define FMax(a,b)  ((a) < (b) ? (b) : (a))
#endif

#ifndef FSwap
#define FSwap(a,b,t)  ((t) = (a), (a) = (b), (b) = (t))
#endif

//#define CopyPixel memcpy
#define FClamp0255 CLAMP255
#define FClamp CLAMP

#define PCL_RGBA(r,g,b,a)          ((COLORREF)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16)|(((DWORD)(BYTE)(a))<<24)))


#define   LIB_PI    3.1415926535897932384626433832795
#define   LIB_2PI   (2.0*LIB_PI)

#define AngleToRadian(nAngle) (LIB_PI * nAngle / 180.0)

// hypotenuse, c2 = a2 + b2
#define FHypot(x, y)  sqrt((x)*(x) + (y)*(y))
#define FSquare(t)   ((t)*(t))

//=============================================================================
// R-G-B-A channel
//=============================================================================
typedef enum IMAGE_CHANNEL {
  CHANNEL_RED   = 1 << 0,// 0x01,
  CHANNEL_GREEN = 1 << 1,// 0x02,
  CHANNEL_BLUE  = 1 << 2,// 0x04,
  CHANNEL_ALPHA = 1 << 3,// 0x08,
  CHANNEL_RGB   = CHANNEL_RED | CHANNEL_GREEN | CHANNEL_BLUE, // 0x07,
  CHANNEL_RGBA  = CHANNEL_RGB | CHANNEL_ALPHA, // 0x0F,
  CHANNEL_GRAY  = 1 << 4,// 0x10
} IMAGE_CHANNEL;

//=============================================================================
// 渐变过渡类型
//=============================================================================
typedef enum REPEAT_MODE {
  REPEAT_NONE = 0,
  REPEAT_SAWTOOTH = 1, // 锯齿波重复
  REPEAT_TRIANGULAR = 2, // 三角波重复
} REPEAT_MODE;

//=============================================================================
// 平面8个方向
//=============================================================================
typedef enum DIRECT_SYS {
  DIRECT_TOP_LEFT,
  DIRECT_TOP,
  DIRECT_TOP_RIGHT,
  DIRECT_LEFT,
  DIRECT_RIGHT,
  DIRECT_BOTTOM_LEFT,
  DIRECT_BOTTOM,
  DIRECT_BOTTOM_RIGHT,
} DIRECT_SYS;
int FRound(double x)
{
  if (x > 0.0) {
    return (int)(x + 0.5);
  }
  else {
    return (int)(x - 0.5);
  }
}

#define RECTWIDTH(rc)  ((rc).right - (rc).left)
#define RECTHEIGHT(rc) ((rc).bottom - (rc).top)

//=============================================================================
// 阴影数据结构
//=============================================================================
typedef struct SHADOWDATA {
  int       nSmooth; // 模糊度
  COLORREF  crShadow; // 颜色/硬度
  int       nAlpha; // 透明度
  int       nOffsetX; // X偏移
  int       nOffsetY; // Y偏移

} SHADOWDATA;

int SHADOWDATA_Init(SHADOWDATA* sd)
{
  sd->nOffsetX = sd->nOffsetY = 5;
  sd->crShadow = PCL_RGBA(75, 75, 75, 0xff);
  sd->nAlpha = 75;
  sd->nSmooth = 5;
  return 0;
}
typedef struct FCPixelGradientLine_t {
  POINT      m_ptStart, m_ptEnd; // coordinate on image
  double     m_fRatX, m_fRatY;
  double     m_fDist;
  COLORREF      m_crStart, m_crEnd;
  REPEAT_MODE  m_nRepeat; // type of repeat
  double     m_fCenX, m_fCenY;
  double     m_fRadiusX, m_fRadiusY;

  double(*CalculateFactor)(struct FCPixelGradientLine_t* gl, int nX, int nY);
} FCPixelGradientLine_t;


typedef struct FCPixelBilinearDistord_t {
  double* m_ColIndex;

  int aw, ah;
  double  m_fPhase; // [0..2n]
  int     m_nAmplitude;
  int     m_nWavelength;

  /// @param fWhirl : [-2n,2n]
  /// @param fPinch : [-1.0,1.0]
  double  m_fWhirl; // radian of whirl
  double  m_fPinch; // [-1.0, 1.0]
  //    double  m_fRadiusScale; // [0.0, 2.0]

  int     m_nDepth; // >=1

  double   m_fRefraction; // >= 1.0
  bool     m_bKeepBk;

  POINT   m_ptNewPos[4]; // LT, RT, RB, LB
  int     m_nNewWidth;
  int     m_nNewHeight;

  // @param nAngle : 0 --> 360
  double   m_fAngle, m_fInvAngle;


  int      m_nSwing, m_nFreq;
  double   m_fDelta;
  double*   m_ShiftDown;

  bool    m_bSinType;


  bool (*calc_undistorted_coord)(struct FCPixelBilinearDistord_t* gl, int x, int y, double* un_x, double* un_y);
} FCPixelBilinearDistord_t;


//-----------------------------------------------------------------------------
int FCHistogram_AnalyzeImage(int aw, int ah, const BYTE* a, int al, int ai, int* his)
{
  int i, x, y;

  for (i = 0; i < 256 * ai; i++) {
    his[i] = 0;
  }

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      const BYTE* p = a + y * al + x * ai;

      for (i = 0; i < ai; ++i) {
        his[i * 256 + p[i]]++;
      }
    }
  }

  return 0;
}
/**
 *  Color helper class.
 *  all function has no param check.
 */

/**
 *  AlphaBlend two pixel, put result into pDest.
 */
static void AlphaBlendPixel(BYTE* pDest, const BYTE* pSrc, BYTE nAlphaSrc)
{
  if (nAlphaSrc == 0xFF) {
    *(WORD*)pDest = *(WORD*)pSrc;
    ((BYTE*)pDest)[2] = ((BYTE*)pSrc)[2];
    return;
  }

  if (nAlphaSrc == 0) {
    return;
  }

  // needn't bound in [0,0xFF], i have checked :-)
  pDest[0] = (pSrc[0] - pDest[0]) * nAlphaSrc / 0xFF + pDest[0];
  pDest[1] = (pSrc[1] - pDest[1]) * nAlphaSrc / 0xFF + pDest[1];
  pDest[2] = (pSrc[2] - pDest[2]) * nAlphaSrc / 0xFF + pDest[2];
}

/**
 *  Calculate pixel's grayscale value.
 *  @param prgb : point a 24bpp or 32bpp pixel address.
 */
static BYTE GetGrayscale(const void* prgb)
{
  return (30 * PCL_R(prgb) + 59 * PCL_G(prgb) + 11 * PCL_B(prgb)) / 100;
}

/**
 *  Rapid pixel copy.
 *  @param nBytes : can be 1,2,3,4
 */
static void CopyPixel(void* pDest, const void* pSrc, int nBytes)
{
  if (nBytes == 4) {
    *(DWORD*)pDest = *(DWORD*)pSrc;
  }
  else if (nBytes == 3) {
    *(WORD*)pDest = *(WORD*)pSrc;
    ((BYTE*)pDest)[2] = ((BYTE*)pSrc)[2];
  }
  else if (nBytes == 1) {
    *(BYTE*)pDest = *(BYTE*)pSrc;
  }
  else if (nBytes == 2) {
    *(WORD*)pDest = *(WORD*)pSrc;
  }
  else {
    assert(false);
    memcpy(pDest, pSrc, nBytes);
  }
}


static bool __IsRGBEqual(const void* p1, const void* p2)
{
  return (PCL_B(p1) == PCL_B(p2)) && (PCL_G(p1) == PCL_G(p2)) && (PCL_R(p1) == PCL_R(p2));
}
static COLORREF __DoubleRGB_to_RGB(const double r, const double g, const double b)
{
  return PCL_RGBA(FClamp0255((int)(r * 255)), FClamp0255((int)(g * 255)), FClamp0255((int)(b * 255)), 0xff);
}



// implement

void CombineAlphaPixel(void* pDest, COLORREF cr1, const void* cr2, BYTE nAlpha2)
{
  if (PCL_A(&cr1) || nAlpha2) {
    if (nAlpha2 == 0) {
      *(COLORREF*)pDest = cr1;
      return;
    }

    if ((PCL_A(&cr1) == 0) || (nAlpha2 == 0xFF)) {
      CopyPixel(pDest, cr2, 3);
      PCL_A(pDest) = nAlpha2;
      return;
    }

    // needn't bound in [0,0xFF], i have checked :-)
    {
      int   nTmp1 = 0xFF * PCL_A(&cr1), nTmp2 = 0xFF * nAlpha2,
            nTmp12 = PCL_A(&cr1) * nAlpha2,
            nTemp = nTmp1 + nTmp2 - nTmp12;
      PCL_B(pDest) = (nTmp2 * PCL_B(cr2) + (nTmp1 - nTmp12) * PCL_B(&cr1)) / nTemp;
      PCL_G(pDest) = (nTmp2 * PCL_G(cr2) + (nTmp1 - nTmp12) * PCL_G(&cr1)) / nTemp;
      PCL_R(pDest) = (nTmp2 * PCL_R(cr2) + (nTmp1 - nTmp12) * PCL_R(&cr1)) / nTemp;
      PCL_A(pDest) = nTemp / 0xFF;
    }

  }
  else {
    PCL_B(pDest) = PCL_G(pDest) = PCL_R(pDest) = 0xFF;
    PCL_A(pDest) = 0;
  }
}
//-----------------------------------------------------------------------------
void RGBtoHLS(const void* prgb, double* H, double* L, double* S)
{
  double r, g, b, cmax, cmin, delta;
  const int   n_cmax = FMax(PCL_R(prgb), FMax(PCL_G(prgb), PCL_B(prgb))),
              n_cmin = FMin(PCL_R(prgb), FMin(PCL_G(prgb), PCL_B(prgb)));

  *L = (n_cmax + n_cmin) / 2.0 / 255.0;

  if (n_cmax == n_cmin) {
    *S = *H = 0.0;
    return;
  }

  r = PCL_R(prgb) / 255.0,
  g = PCL_G(prgb) / 255.0,
  b = PCL_B(prgb) / 255.0,
  cmax = n_cmax / 255.0,
  cmin = n_cmin / 255.0,
  delta = cmax - cmin;

  if (*L < 0.5) {
    *S = delta / (cmax + cmin);
  }
  else {
    *S = delta / (2.0 - cmax - cmin);
  }

  if (PCL_R(prgb) == n_cmax) {
    *H = (g - b) / delta;
  }
  else if (PCL_G(prgb) == n_cmax) {
    *H = 2.0 + (b - r) / delta;
  }
  else {
    *H = 4.0 + (r - g) / delta;
  }

  *H /= 6.0;

  if (*H < 0.0) {
    *H += 1.0;
  }
}
//-----------------------------------------------------------------------------
double __HLS_Value(double m1, double m2, double h)
{
  if (h > 6.0) {
    h -= 6.0;
  }
  else if (h < 0.0) {
    h += 6.0;
  }

  if (h < 1.0) {
    return m1 + (m2 - m1) * h;
  }
  else if (h < 3.0) {
    return m2;
  }
  else if (h < 4.0) {
    return m1 + (m2 - m1) * (4.0 - h);
  }

  return m1;
}
COLORREF HLStoRGB(double H, double L, double S)
{
  double     m1, m2;

  if (S < FLT_EPSILON) { // == 0
    return __DoubleRGB_to_RGB(L, L, L);
  }

  if (L < 0.5) {
    m2 = L * (1.0 + S);
  }
  else {
    m2 = L + S - L * S;
  }

  m1 = 2.0 * L - m2;

  return __DoubleRGB_to_RGB(__HLS_Value(m1, m2, H * 6.0 + 2.0),
      __HLS_Value(m1, m2, H * 6.0),
      __HLS_Value(m1, m2, H * 6.0 - 2.0));
}
//-----------------------------------------------------------------------------
void RGBtoHSV(const void* prgb, double* H, double* S, double* V)
{
  const int      n_cmax = FMax(PCL_R(prgb), FMax(PCL_G(prgb), PCL_B(prgb))),
                 n_cmin = FMin(PCL_R(prgb), FMin(PCL_G(prgb), PCL_B(prgb)));
  const double   r = PCL_R(prgb) / 255.0,
                 g = PCL_G(prgb) / 255.0,
                 b = PCL_B(prgb) / 255.0,
                 delta = (n_cmax - n_cmin) / 255.0;
  *V = n_cmax / 255.0;

  if (n_cmax == n_cmin) {
    *S = 0.0;
    *H = 0.0;
    return;
  }

  *S = (n_cmax - n_cmin) / (double)n_cmax;

  if (PCL_R(prgb) == n_cmax) {
    *H = (g - b) / delta;
  }
  else if (PCL_G(prgb) == n_cmax) {
    *H = 2.0 + (b - r) / delta;
  }
  else if (PCL_B(prgb) == n_cmax) {
    *H = 4.0 + (r - g) / delta;
  }

  *H /= 6.0;

  if (*H < 0.0) {
    *H += 1.0;
  }
  else if (*H > 1.0) {
    *H -= 1.0;
  }
}
//-----------------------------------------------------------------------------
COLORREF HSVtoRGB(double h, double s, double v)
{
  double f, p, q, t;

  if (s < FLT_EPSILON) { // == 0
    return __DoubleRGB_to_RGB(v, v, v);
  }

  if (h == 1.0) {
    h = 0.0;
  }

  h *= 6.0;

  f = h - (int)h,
  p = v * (1.0 - s),
  q = v * (1.0 - s * f),
  t = v * (1.0 - s * (1.0 - f));

  switch ((int)h) {
  case 0 :
    return __DoubleRGB_to_RGB(v, t, p);

  case 1 :
    return __DoubleRGB_to_RGB(q, v, p);

  case 2 :
    return __DoubleRGB_to_RGB(p, v, t);

  case 3 :
    return __DoubleRGB_to_RGB(p, q, v);

  case 4 :
    return __DoubleRGB_to_RGB(t, p, v);

  case 5 :
    return __DoubleRGB_to_RGB(v, p, q);
  }

  return __DoubleRGB_to_RGB(0, 0, 0);
}
//-----------------------------------------------------------------------------
// Computes bilinear interpolation of four pixels.
// The pixels in 'crPixel' in the following order: [0,0], [1,0], [0,1], [1,1].
// !!! the pointer must 24bit or 32bit color
COLORREF Get_Bilinear_Pixel(double x, double y, bool bHasAlpha, const BYTE* crPixel[4])
{
  int i;
  const BYTE*     pPixel0 = crPixel[0], * pPixel1 = crPixel[1],
                  * pPixel2 = crPixel[2], * pPixel3 = crPixel[3];
  COLORREF       crRet = 0xFFFFFFFF;

  if (bHasAlpha && (*(DWORD*)pPixel0 == *(DWORD*)pPixel1) &&
      (*(DWORD*)pPixel0 == *(DWORD*)pPixel2) &&
      (*(DWORD*)pPixel0 == *(DWORD*)pPixel3)) {
    return *(COLORREF*)pPixel0;
  }

  if (!bHasAlpha && __IsRGBEqual(pPixel0, pPixel1) &&
      __IsRGBEqual(pPixel0, pPixel2) &&
      __IsRGBEqual(pPixel0, pPixel3)) {
    CopyPixel(&crRet, pPixel0, 3);
    return crRet;
  }

  // test x-[0,1] y-[0,1]
  assert((x > -FLT_EPSILON) && (x < 1.0) && (y > -FLT_EPSILON) && (y < 1.0));

  // x && y is zero
  if ((x < FLT_EPSILON) && (y < FLT_EPSILON)) {
    CopyPixel(&crRet, pPixel0, bHasAlpha ? 4 : 3);
    return crRet;
  }

  if (!bHasAlpha || ((PCL_A(pPixel0) & PCL_A(pPixel1) & PCL_A(pPixel2) & PCL_A(pPixel3)) == 0xFF)) {
    // if 24bit color
    for (i = 0; i < 3; i++) {
      const double   m0 = pPixel0[i] + x * (pPixel1[i] - pPixel0[i]),
                     m1 = pPixel2[i] + x * (pPixel3[i] - pPixel2[i]),
                     my = m0 + y * (m1 - m0);
      ((BYTE*)&crRet)[i] = (int)my; // needn't bound
    }
  }
  else {
    // under is 32bit color with alpha
    int       nAlpha[4] = {pPixel0[3], pPixel1[3], pPixel2[3], pPixel3[3]};
    // calc dest alpha value
    double    m0 = nAlpha[0] + x * (nAlpha[1] - nAlpha[0]),
              m1 = nAlpha[2] + x * (nAlpha[3] - nAlpha[2]),
              my = m0 + y * (m1 - m0);
    PCL_A(&crRet) = (int)my;

    if (PCL_A(&crRet)) {// has alpha
      for (i = 0; i < 3; i++) {
        int   nSum0 = nAlpha[0] * pPixel0[i],
              nSum2 = nAlpha[2] * pPixel2[i];

        m0 = nSum0 + x * (pPixel1[i] * nAlpha[1] - nSum0);
        m1 = nSum2 + x * (pPixel3[i] * nAlpha[3] - nSum2);
        my = m0 + y * (m1 - m0);
        ((BYTE*)&crRet)[i] = (int)(my) / PCL_A(&crRet);
      }
    }
  }

  return crRet;
}


