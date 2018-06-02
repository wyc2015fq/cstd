
#include "fcolor.inl"


/**
 *  1, 4, 8, 24, 32 ==> 16.
@verbatim
    example:
        FCPixelConvertTo16Bit   aCmd;
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/

int FCPixelConvertTo16Bit(int w, int h, const BYTE* pImg, int al, int ai, const BYTE* pOutImg, int bl)
{
  int x, y;
  const BYTE* pRGB;
  WORD* pPixel = (WORD*)pOutImg;

  for (y = 0; y < h; ++y) {
    for (x = 0; x < w; ++x) {
      pRGB = pImg + y * al + x * ai;
      *pPixel++ = (((PCL_R(pRGB) >> 3) << 10) | ((PCL_G(pRGB) >> 3) << 5) | (PCL_B(pRGB) >> 3)); // 24,32 ==> 16
    }
  }

  return 0;
}

/**
 *  1, 4, 8, 16, 24, 32 ==> 8bit gray.
@verbatim
    example:
        FCPixelConvertTo8BitGray   aCmd;
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelConvertTo8BitGray(int w, int h, const BYTE* pImg, int al, int ai, const BYTE* pOutImg, int bl)
{
  int x, y;
  const BYTE* pRGB;
  BYTE* pPixel = (BYTE*)pOutImg;

  for (y = 0; y < h; ++y) {
    for (x = 0; x < w; ++x) {
      pRGB = pImg + y * al + x * ai;
      *pPixel++ = (30 * PCL_R(pRGB) + 59 * PCL_G(pRGB) + 11 * PCL_B(pRGB)) / 100;
    }
  }

  return 0;
}


/**
 *  Fill pattern image (32 bit).
@verbatim
    example:
        FCObjImage           * pMask = new FCObjImage ("c:\\test.jpg"); // mustn't delete
        FCPixelFillPattern   aCmd (pMask, 192, false);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/

int FCPixelFillPattern(int ah, int aw, const BYTE* a, int al, int ai, int bw, int bh, BYTE* b, int bl, int m_bOnlyTexture, int m_nAlpha)
{
  int x, y;

  for (y = 0; y < bh; ++y) {
    for (x = 0; x < bw; ++x) {
      int x1 = x % aw, y1 = y % ah;
      const BYTE* pPat = a + x1 * ai + y1 * al;
      BYTE* pPixel = b + x * ai + y * bl;

      if (m_bOnlyTexture) {
        // calculate texture
        int   n = (PCL_B(pPat) + PCL_G(pPat) + PCL_R(pPat) - 384) * m_nAlpha / 765;
        PCL_B(pPixel) = FClamp0255(PCL_B(pPixel) - n);
        PCL_G(pPixel) = FClamp0255(PCL_G(pPixel) - n);
        PCL_R(pPixel) = FClamp0255(PCL_R(pPixel) - n);
      }
      else {
        CombineAlphaPixel(pPixel, *(COLORREF*)pPixel, pPat, m_nAlpha);
      }

    }
  }

  return 0;
}

/**
 *  Adjust image's hue & saturation (>=24 bit).
@verbatim
    example:
        FCPixelHueSaturation   aCmd (100, 150);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
typedef struct FCPixelHueSaturation_t {
  int   hue_transfer[6][256];
  int   saturation_transfer[6][256];

} FCPixelHueSaturation_t;
/**
 *  Constructor.
 *  nHue & nSaturation : between 0 and 200
 */
int FCPixelHueSaturation_init(FCPixelHueSaturation_t* hs, int nHue, int nSaturation)
{
  int i, hue;
  nHue = FClamp(nHue, 0, 200);
  nHue -= 100;
  nHue = 180 * nHue / 100;
  nSaturation = FClamp(nSaturation, 0, 200);
  nSaturation -= 100;

  for (hue = 0; hue < 6; hue++) {
    for (i = 0; i < 256; i++) {
      int   value = nHue * 255 / 360;

      if ((i + value) < 0) {
        hs->hue_transfer[hue][i] = 255 + (i + value);
      }
      else if ((i + value) > 255) {
        hs->hue_transfer[hue][i] = i + value - 255;
      }
      else {
        hs->hue_transfer[hue][i] = i + value;
      }

      // saturation
      value = nSaturation * 255 / 100;
      value = FClamp(value, -255, 255);
      hs->saturation_transfer[hue][i] = FClamp0255(i * (255 + value) / 255);
    }
  }

  return 0;
}

int FCPixelHueSaturation(FCPixelHueSaturation_t* hs, int ah, int aw, const BYTE* a, int al, int ai, BYTE* b, int bl)
{
  int x, y;

  for (y = 0; y < ah; ++y) {
    for (x = 0; x < aw; ++x) {
      const BYTE* pPixel = a + y * al + x * ai;
      BYTE* pOut = b + y * bl + x * ai;

      COLORREF   cr;
      int   hue = 0;
      double   h, l, s;
      RGBtoHLS(pPixel, &h, &l, &s);
      h *= 255;
      s *= 255;

      if (h < 21) {
        hue = 0;
      }
      else if (h < 64) {
        hue = 1;
      }
      else if (h < 106) {
        hue = 2;
      }
      else if (h < 149) {
        hue = 3;
      }
      else if (h < 192) {
        hue = 4;
      }
      else if (h < 234) {
        hue = 5;
      }
      else {
        hue = 0;
      }

      h = hs->hue_transfer[hue][FClamp0255((int)h)];
      s = hs->saturation_transfer[hue][FClamp0255((int)s)];

      cr = HLStoRGB(h / 255.0, l, s / 255.0);
      CopyPixel(pOut, &cr, 3);
    }
  }

  return 0;
}



/**
 *  Left-Right mirror image (>=8 bit).
@verbatim
    example:
        FCPixelMirror   aCmd;
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelMirror(int ah, int aw, BYTE* a, int al, int ai)
{
  int i, x, y;
  BYTE t;

  for (y = 0; y < ah; ++y) {
    for (x = 0; x < aw / 2; ++x) {
      BYTE* pPixel = a + y * al + x * ai;
      BYTE* pRight = a + y * al + (aw - 1 - x) * ai;

      for (i = 0; i < ai; i++) {
        FSwap(pPixel[i], pRight[i], t);  // bytes of per pixel
      }
    }
  }

  return 0;
}


/**
 *  Top-Bottom flip image (>=8 bit).
@verbatim
    example:
        FCPixelFlip   aCmd;
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelFlip(int ah, int aw, BYTE* a, int al, int ai)
{
  int i, x, y;
  BYTE t;

  for (y = 0; y < ah / 2; ++y) {
    for (x = 0; x < aw; ++x) {
      BYTE* pPixel = a + y * al + x * ai;
      BYTE* pBottom = a + (ah - 1 - y) * al + x * ai;

      for (i = 0; i < ai; i++) {
        FSwap(pPixel[i], pBottom[i], t);  // bytes of per pixel
      }
    }
  }

  return 0;
}

/**
 *  Shift (>=24 bit).
@verbatim
    example:
        FCPixelShift   aCmd(5);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelShift(int ah, int aw, BYTE* a, int al, int ai, int nAmount)
{
  int i, x, y;
  int m_nAmount = (FMax(0, nAmount));
  srand((unsigned int)time(0));

  for (y = 0; y < ah; ++y) {
    for (x = 0; x < aw; ++x) {
      BYTE* pPixel = a + y * al + x * ai;

      int     nSpan = ai;
      int          nShift = rand() % (m_nAmount + 1);
      BYTE     crLeft[4], crRight[4]; // L/R edge pixel color

      // first pixel every line
      if (x || !m_nAmount || !nShift) {
        continue;
      }

      CopyPixel(crLeft, a + y * al, nSpan);
      CopyPixel(crRight, a + y * al + (aw - 1)*ai, nSpan);

      if (rand() % 2) {
        // shift right
        if (aw > nShift) {
          memmove(a + (nShift * ai + y * al), pPixel, (aw - nShift)*nSpan);
        }
        else {
          nShift = aw;
        }

        for (i = 0; i < nShift; i++, pPixel += nSpan) {
          CopyPixel(pPixel, crLeft, nSpan);
        }
      }
      else {
        // shift left
        if (aw > nShift) {
          memmove(pPixel, a + (nShift * ai + y * al), (aw - nShift)*nSpan);
        }
        else {
          nShift = aw;
        }

        pPixel = a + (aw - 1) * ai + y * al;

        for (i = 0; i < nShift; i++, pPixel -= nSpan) {
          CopyPixel(pPixel, crRight, nSpan);
        }
      }
    }
  }

  return 0;
}


/** // 自动对比度
 *  Auto contrast (>=24 bit).
@verbatim
    example:
        FCPixelAutoContrast   aCmd;
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelAutoContrast(int ah, int aw, BYTE* a, int al, int ai)
{
  int b, x, y;
  BYTE     m_byLut[256][3];

  BYTE     byCmin[3] = {255, 255, 255},
      byCmax[3] = {0, 0, 0};

  // Get minimum and maximum values for each channel
  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      BYTE* pPixel = a + y * al + x * ai;

      for (b = 0; b < 3; b++) {
        if (pPixel[b] < byCmin[b]) {
          byCmin[b] = pPixel[b];
        }

        if (pPixel[b] > byCmax[b]) {
          byCmax[b] = pPixel[b];
        }
      }
    }
  }

  // Calculate LUTs with stretched contrast
  for (b = 0; b < 3; b++) {
    const int     nRange = byCmax[b] - byCmin[b];

    if (nRange) {
      for (x = byCmin[b]; x <= byCmax[b]; x++) {
        m_byLut[x][b] = 255 * (x - byCmin[b]) / nRange;
      }
    }
    else {
      m_byLut[byCmin[b]][b] = byCmin[b];
    }
  }

  for (y = 0; y < ah; ++y) {
    for (x = 0; x < aw; ++x) {
      BYTE* pPixel = a + y * al + x * ai;

      for (b = 0; b < 3; b++) {
        pPixel[b] = m_byLut[pPixel[b]][b];
      }
    }
  }

  return 0;
}



/**
 *  Auto color enhance (>=24 bit).
@verbatim
    example:
        FCPixelAutoColorEnhance   aCmd;
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelAutoColorEnhance(int ah, int aw, BYTE* a, int al, int ai)
{
  int x, y;
  double     m_vhi, m_vlo;

  m_vhi = 0.0;
  m_vlo = 1.0;

  // Get minimum and maximum values
  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      BYTE* pPixel = a + y * al + x * ai;
      int     c = 255 - PCL_B(pPixel),
              m = 255 - PCL_G(pPixel),
              y = 255 - PCL_R(pPixel),
              k = c;

      if (m < k) {
        k = m;
      }

      if (y < k) {
        k = y;
      }

      {
        BYTE    byMap[4] = { c - k, m - k, y - k };
        double  h, z, v;
        RGBtoHSV(byMap, &h, &z, &v);

        if (v > m_vhi) {
          m_vhi = v;
        }

        if (v < m_vlo) {
          m_vlo = v;
        }
      }
    }
  }

  for (y = 0; y < ah; ++y) {
    for (x = 0; x < aw; ++x) {
      BYTE* pPixel = a + y * al + x * ai;
      int     c = 255 - PCL_B(pPixel),
              m = 255 - PCL_G(pPixel),
              y = 255 - PCL_R(pPixel),
              k = c;

      if (m < k) {
        k = m;
      }

      if (y < k) {
        k = y;
      }

      {
        BYTE    byMap[4] = { c - k, m - k, y - k };
        double  h, z, v;
        RGBtoHSV(byMap, &h, &z, &v);

        if (m_vhi != m_vlo) {
          v = (v - m_vlo) / (m_vhi - m_vlo);
        }

        *(COLORREF*)byMap = HSVtoRGB(h, z, v);
        c = byMap[0];
        m = byMap[1];
        y = byMap[2];
        c += k;
      }

      if (c > 255) {
        c = 255;
      }

      m += k;

      if (m > 255) {
        m = 255;
      }

      y += k;

      if (y > 255) {
        y = 255;
      }

      PCL_B(pPixel) = 255 - c;
      PCL_G(pPixel) = 255 - m;
      PCL_R(pPixel) = 255 - y;
    }
  }

  return 0;
}

/** 浮雕
 *  Emboss effect (>=24 bit).
@verbatim
    example:
        FCPixelEmboss   aCmd(5);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
/**
*  @param nAngle : [0,360]
*/
int FCPixelEmboss(int ah, int aw, BYTE* a, int al, int ai, int nAngle)
{
  int x, y;
  double   r = AngleToRadian(nAngle), dr = LIB_PI / 4.0;
  int     m_nAngle;
  double  m_Weights[3][3];
  BYTE* b = (BYTE*)memdup(a, ah * al);
  m_nAngle = (nAngle);
  m_Weights[0][0] = cos(r + dr);
  m_Weights[0][1] = cos(r + 2.0 * dr);
  m_Weights[0][2] = cos(r + 3.0 * dr);

  m_Weights[1][0] = cos(r);
  m_Weights[1][1] = 0;
  m_Weights[1][2] = cos(r + 4.0 * dr);

  m_Weights[2][0] = cos(r - dr);
  m_Weights[2][1] = cos(r - 2.0 * dr);
  m_Weights[2][2] = cos(r - 3.0 * dr);


  // Get minimum and maximum values
  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      BYTE* pPixel = a + y * al + x * ai;
      double   sum = 0;
      int dx, dy;
      int x1 = MIN(aw - 2, x), y1 = MIN(ah - 2, y);

      for (dy = 0; dy < 3; dy++) {
        for (dx = 0; dx < 3; dx++) {
          int   n = GetGrayscale(b + (x1 + dx) * ai + (y1 + dy) * al);
          sum += m_Weights[dy][dx] * n;
        }
      }

      PCL_R(pPixel) = PCL_G(pPixel) = PCL_B(pPixel) = FClamp0255((int)sum + 128);
    }
  }

  free(b);
  return 0;
}

/**
 *  Illusion effect (>=24 bit).
@verbatim
    example:
        FCPixelIllusion   aCmd(3);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelIllusion(int ah, int aw, BYTE* a, int al, int ai, int nAmount)
{
  int i, x, y;
  int     m_nAmount = nAmount;
  double  m_fScale, m_fOffset;
  BYTE* b = (BYTE*)memdup(a, ah * al);
  m_fScale = FHypot(aw, ah) / 2.0;
  m_fOffset = (int)(m_fScale / 2.0);

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      const BYTE*    pPixel2;
      BYTE* pPixel = a + y * al + x * ai;
      const double   fXCen = aw / 2.0,
                     fYCen = ah / 2.0,
                     cx = (x - fXCen) / m_fScale,
                     cy = (y - fYCen) / m_fScale,
                     fTmp = LIB_PI / (double)m_nAmount;

      double   angle = floor(atan2(cy, cx) / 2.0 / fTmp) * 2.0 * fTmp + fTmp;
      double   radius = FHypot(cx, cy);

      int   xx = (int)(x - m_fOffset * cos(angle)),
            yy = (int)(y - m_fOffset * sin(angle));
      xx = FClamp(xx, 0, aw - 1);
      yy = FClamp(yy, 0, ah - 1);

      pPixel2 = b + xx * ai + yy * al;

      for (i = 0; i < ai; i++) {
        pPixel[i] = FClamp0255(pPixel[i] + (int)(radius * (pPixel2[i] - pPixel[i])));
      }
    }
  }

  free(b);
  return 0;
}


/**
 *  Blind effect (>=24 bit).
@verbatim
    example:
        FCPixelBlinds   aCmd (FCPixelBlinds::BLIND_X, 10, 50, PCL_RGBA(0,0,255));
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
typedef enum BLIND_TYPE {
  BLIND_X,
  BLIND_Y,
} BLIND_TYPE;
/**
 *  Constructor.
 *  @param nDirect : can be FCPixelBlinds::BLIND_X or FCPixelBlinds::BLIND_Y
 *  @param nOpacity : percentage of crBlind [1,100]
*/
int FCPixelBlinds(int ah, int aw, BYTE* a, int al, int ai, BLIND_TYPE nDirect, int nWidth, int nOpacity, COLORREF crBlind)
{
  int x, y;
  BLIND_TYPE  m_nDirect;
  int         m_nWidth;
  int         m_nOpacity;
  COLORREF     m_crBlind;
  int     nMod;

  m_nDirect = nDirect;
  m_nWidth = nWidth;
  m_crBlind = crBlind;
  m_nOpacity = FClamp(nOpacity, 1, 100);


  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      double fAlphaAdd;
      BYTE* pPixel = a + y * al + x * ai;

      if (m_nDirect == BLIND_X) { // horizontal direction
        nMod = y % m_nWidth;
      }
      else if (m_nDirect == BLIND_Y) { // vertical direction
        nMod = x % m_nWidth;
      }

      fAlphaAdd = 255.0 * m_nOpacity / 100.0 / (m_nWidth - 1.0);
      AlphaBlendPixel(pPixel, (BYTE*)&m_crBlind, FClamp0255((int)(nMod * fAlphaAdd)));
    }
  }

  return 0;
}

/**
 *  Mosaic effect (32 bit).
@verbatim
    example:
        FCPixelMosaic   aCmd(5);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
/// Constructor.
/// @param nBlock : pixel width of block
int FCPixelMosaic(int ah, int aw, BYTE* a, int al, int ai, int nBlock)
{
  int x, y, i, j, k;

  for (y = 0; y < ah; y += nBlock) {
    for (x = 0; x < aw; x += nBlock) {
      int sum[4] = {0};
      int dx = MIN(aw - x, nBlock);
      int dy = MIN(ah - y, nBlock);
      int n = dx * dy;

      for (i = y; i < y + dy; ++i) {
        for (j = x; j < x + dx; ++j) {
          for (k = 0; k < ai; ++k) {
            sum[k] += a[i * al + j * ai + k];
          }
        }
      }

      for (i = y; i < y + dy; ++i) {
        for (j = x; j < x + dx; ++j) {
          for (k = 0; k < ai; ++k) {
            a[i * al + j * ai + k] = sum[k] / n;
          }
        }
      }
    }
  }

  return 0;
}


/**
 *  Adjust image's RGB value (>=24 bit).
@verbatim
    example:
        FCPixelAdjustRGB   aCmd (-100, 50, 220);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelAdjustRGB(int ah, int aw, BYTE* a, int al, int ai, int nBlock, int nR, int nG, int nB)
{
  int x, y;
  int m_R = (nR), m_G = (nG), m_B = (nB);

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      BYTE* p = a + y * al + x * ai;
      int B = PCL_B(p) + m_B;
      int G = PCL_G(p) + m_G;
      int R = PCL_R(p) + m_R;
      PCL_B(p) = FClamp0255(B);
      PCL_G(p) = FClamp0255(G);
      PCL_R(p) = FClamp0255(R);
    }
  }

  return 0;
}

/**
 *  Color level (>=24 bit).
@verbatim
    example:
        FCPixelColorLevel   aCmd (false, 20, 230);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/

int FCPixelColorLevel(int ah, int aw, BYTE* a, int al, int ai,
    bool bAuto, int nInLow, int nInHigh, IMAGE_CHANNEL nChannel/* = CHANNEL_RGB*/)
{
  int x, y;
  int      m_nInputLow[3] = {0};
  int      m_nInputHigh[3] = {0};
  int      m_nOutputLow, m_nOutputHigh;
  bool     m_bAuto, m_bChannelR, m_bChannelG, m_bChannelB;

  m_bAuto = bAuto;
  m_nInputLow[0] = m_nInputLow[1] = m_nInputLow[2] = FClamp0255(nInLow);
  m_nInputHigh[0] = m_nInputHigh[1] = m_nInputHigh[2] = FClamp0255(nInHigh);
  m_nOutputLow = 0;
  m_nOutputHigh = 255;
  m_bChannelR = (nChannel & CHANNEL_RED) ? true : false;
  m_bChannelG = (nChannel & CHANNEL_GREEN) ? true : false;
  m_bChannelB = (nChannel & CHANNEL_BLUE) ? true : false;

  if (m_bAuto) {
    int     nIndex = 0;
    int hist[256 * 3] = {0};
    FCHistogram_AnalyzeImage(aw, ah, a, al, ai, hist);

    for (nIndex = 0; nIndex < 3; ++nIndex) {
      int* histo = hist + nIndex * 256;
      const int     nCount = ah * aw;

      if (nCount == 0) {
        m_nInputLow[nIndex] = m_nInputHigh[nIndex] = 0;
      }
      else {
        int new_count = 0, i;
        double   percentage, next_percentage;

        m_nInputLow[nIndex] = 0;
        m_nInputHigh[nIndex] = 255;
        // Set the low input

        for (i = 0; i < 255; i++) {
          new_count += histo[i];
          percentage = new_count / (double)nCount;
          next_percentage = (new_count + histo[i + 1]) / (double)nCount;

          if (fabs(percentage - 0.006) < fabs(next_percentage - 0.006)) {
            m_nInputLow[nIndex] = i + 1;
            break;
          }
        }

        // Set the high input
        new_count = 0;

        for (i = 255; i > 0; i--) {
          new_count += histo[i];
          percentage = new_count / (double)nCount;
          next_percentage = (new_count + histo[i - 1]) / (double)nCount;

          if (fabs(percentage - 0.006) < fabs(next_percentage - 0.006)) {
            m_nInputHigh[nIndex] = i - 1;
            break;
          }
        }
      }
    }
  }

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      BYTE* pPixel = a + y * al + x * ai;
      double      fInten;

      if (m_bChannelB) {
        fInten = PCL_B(pPixel) - m_nInputLow[0];

        if (m_nInputHigh[0] != m_nInputLow[0]) {
          fInten /= (double)(m_nInputHigh[0] - m_nInputLow[0]);
        }

        PCL_B(pPixel) = FClamp0255(FRound(fInten * 255.0));
      }

      if (m_bChannelG) {
        fInten = PCL_G(pPixel) - m_nInputLow[1];

        if (m_nInputHigh[1] != m_nInputLow[1]) {
          fInten /= (double)(m_nInputHigh[1] - m_nInputLow[1]);
        }

        PCL_G(pPixel) = FClamp0255(FRound(fInten * 255.0));
      }

      if (m_bChannelR) {
        fInten = PCL_R(pPixel) - m_nInputLow[2];

        if (m_nInputHigh[2] != m_nInputLow[2]) {
          fInten /= (double)(m_nInputHigh[2] - m_nInputLow[2]);
        }

        PCL_R(pPixel) = FClamp0255(FRound(fInten * 255.0));
      }
    }
  }

  return 0;
}

/**
 *  Threshold image (>=24 bit).
@verbatim
    example:
        FCPixelThreshold   aCmd (128);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelThreshold(int ah, int aw, BYTE* a, int al, int ai, int nLevel)
{
  int x, y;

  /// Constructor (nLevel range [0,255]).
  int m_nLevel = (FClamp0255(nLevel));

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      BYTE* pPixel = a + y * al + x * ai;
      int   cr = (GetGrayscale(pPixel) > m_nLevel) ? 255 : 0;
      memset(pPixel, cr, ai);
    }
  }

  return 0;
}


/**
 *  Clockwise rotate 90' (>=8 bit).
@verbatim
    example:
        FCPixelRotate90   aCmd;
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelRotate90(int ah, int aw, BYTE* a, int al, int ai)
{
  int x, y;
  BYTE* b = (BYTE*)memdup(a, ah * al);

  /// Constructor (nLevel range [0,255]).
  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      BYTE* pPixel = a + y * al + x * ai;
      BYTE* pPixelB = b + y * al + (aw - 1 - x) * ai;
      memcpy(pPixel, pPixelB, ai);
    }
  }

  free(b);
  return 0;
}




/**
 *  Halftone (>=24 bit), use Limb Pattern M3 algorithm.
@verbatim
    example:
        FCPixelHalftoneM3   aCmd;
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelHalftoneM3(int ah, int aw, BYTE* a, int al, int ai)
{
  int x, y;
  static BYTE  s_BayerPattern[8][8] = { // 64 level gray
    0, 32, 8, 40, 2, 34, 10, 42,
    48, 16, 56, 24, 50, 18, 58, 26,
    12, 44, 4, 36, 14, 46, 6, 38,
    60, 28, 52, 20, 62, 30, 54, 22,
    3, 35, 11, 43, 1, 33, 9, 41,
    51, 19, 59, 27, 49, 17, 57, 25,
    15, 47, 7, 39, 13, 45, 5, 37,
    63, 31, 55, 23, 61, 29, 53, 21
  };

  /// Constructor (nLevel range [0,255]).
  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      BYTE* pPixel = a + y * al + x * ai;
      BYTE   gr = GetGrayscale(pPixel);
      PCL_R(pPixel) = PCL_G(pPixel) = PCL_B(pPixel) = (((gr >> 2) > s_BayerPattern[y & 7][x & 7]) ? 0xFF : 0);
    }
  }

  return 0;
}

// 每边各扩r个像素
BYTE* ExpandFrame(int ah, int aw, BYTE* a, int al, int ai, int r)
{
  int bw = aw + r * 2;
  int bh = ah + r * 2;
  int bl = bw * ai;
  BYTE* b = MALLOC(BYTE, bw * bh * ai);
  int i, j;

  for (i = 0; i < ah; ++i) {
    memcpy(b + (i + r)*bl + r * ai, a + i * al, aw * ai);

    for (j = 0; j < r; ++j) {
      memcpy(b + (i + r)*bl + j * ai, a + i * al + (r - 1 - j)*ai, ai);
      memcpy(b + (i + r)*bl + (aw + r + j)*ai, a + i * al + (aw - 1 - j)*ai, ai);
    }
  }

  for (i = 0; i < r; ++i) {
    memcpy(b + i * bl, b + (r + r - 1 - i)*bl, bw * ai);
    memcpy(b + (ah + r + i)*bl, b + (ah + r - 1 - i)*bl, bw * ai);
  }

  return b;
}
/**
 *  Oil paint (>=24 bit).
@verbatim
    example:
        FCPixelOilPaint   aCmd (2);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
  */
int FCPixelOilPaint(int ah, int aw, BYTE* A, int al, int ai, int nRadius)
{
  int i, ny, x, y;
  int     m_nRadius; // >= 1
  int     m_nLength; // 2*m_nRadius + 1
  BYTE*   m_ImgGray;
  BYTE* b;
  int bw, bh, bl;
  m_nRadius = FMax(1, nRadius);
  m_nLength = 2 * m_nRadius + 1; // Stat. block

  b = ExpandFrame(aw, ah, A, al, ai, m_nRadius);
  bw = aw + m_nRadius + m_nRadius;
  bh = ah + m_nRadius + m_nRadius;
  bl = bw * ai;
  m_ImgGray = MALLOC(BYTE, bw * bh);

  // calculate block gray
  FCPixelConvertTo8BitGray(bw, bh, b, bl, ai, m_ImgGray, bw);

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      BYTE* pPixel = A + y * al + x * ai;
      BYTE*     pSelPixel = 0;
      int nMaxNum = 0;
      int pHistogram[256];
      memset(pHistogram, 0, sizeof(int) * 256);

      // replace every pixel use most frequency
      for (ny = 0; ny < m_nLength; ny++) {
        for (i = 0; i < m_nLength; i++) {
          BYTE*    pGray = m_ImgGray + (x + i) + (y + ny) * bw;
          ASSERT((x + i) < bw);

          if (++pHistogram[*pGray] > nMaxNum) {
            nMaxNum = pHistogram[*pGray];
            pSelPixel = b + (x + i) * ai + (y + ny) * bl;
          }
        }
      }

      CopyPixel(pPixel, pSelPixel, 3);  // leave alpha channel
    }
  }

  free(m_ImgGray);
  free(b);
  return 0;
}


/**
 *  Color tone (>=24 bit).
@verbatim
    example:
        FCPixelColorTone   aCmd (PCL_RGBA(0,0,255));
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelColorTone(int ah, int aw, BYTE* a, int al, int ai, COLORREF crTone)
{
  int x, y;
  double   m_nHue;
  double   L, S;
  RGBtoHLS(&crTone, &m_nHue, &L, &S);

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      double   H, L, S;
      COLORREF   cr;
      BYTE* pPixel = a + y * al + x * ai;
      int   n = GetGrayscale(pPixel);
      PCL_B(pPixel) = (BYTE)(255 * pow(n / 255.0, 1.2));
      PCL_G(pPixel) = (BYTE)(255 * pow(n / 255.0, 1));
      PCL_R(pPixel) = (BYTE)(255 * pow(n / 255.0, 0.8));

      RGBtoHLS(pPixel, &H, &L, &S);
      cr = HLStoRGB(m_nHue, L, S * 150 / 100);
      CopyPixel(pPixel, &cr, 3);
    }
  }

  return 0;
}

/**
 *  Noisify (>=24 bit).
@verbatim
    example:
        FCPixelNoisify   aCmd (5, false);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
/**
 *  Constructor.
 *  @param nLevel : level (0 <= nLevel <= 100).
*/
int GenGauss(int m_nLevel)
{
  double   d = (rand() + rand() + rand() + rand()) * 5.28596089837e-5 - 3.46410161514;
  return (int)(m_nLevel * d * 127.0 / 100.0);
}
int FCPixelNoisify(int ah, int aw, BYTE* a, int al, int ai, int nLevel, bool bRandom)
{
  int x, y;
  int    m_nLevel;
  bool   m_bRandom;
  m_nLevel = FClamp(nLevel, 0, 100);
  srand((unsigned int)time(0));
  m_bRandom = bRandom;

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      BYTE* pPixel = a + y * al + x * ai;
      int     n1, n2, n3;

      if (m_bRandom) {
        n1 = GenGauss(m_nLevel);
        n2 = GenGauss(m_nLevel);
        n3 = GenGauss(m_nLevel);
      }
      else {
        n1 = n2 = n3 = GenGauss(m_nLevel);
      }

      PCL_B(pPixel) = FClamp0255(PCL_B(pPixel) + n1);
      PCL_G(pPixel) = FClamp0255(PCL_G(pPixel) + n2);
      PCL_R(pPixel) = FClamp0255(PCL_R(pPixel) + n3);
    }
  }

  return 0;
}

/**
 *  Splash image (>=24 bit).
@verbatim
    example:
        FCPixelSplash   aCmd (5);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelSplash(int ah, int aw, byte* A, int al, int ai, int nBlock)
{
  int x, y;
  int  m_nBlock;
  /// Constructor.
  /// @param nBlock : splash level (>=3).
  int bl = aw * ai;
  byte* b = memdup2d(A, al, ah, bl);

  m_nBlock = FMax(3, nBlock);
  srand((unsigned int)time(0));

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      const BYTE* pSrc;
      BYTE* pPixel = A + y * al + x * ai;
      int   xx = x - m_nBlock / 2 + (rand() % m_nBlock),
            yy = y - m_nBlock / 2 + (rand() % m_nBlock);
      xx = FClamp(xx, 0, aw - 1);
      yy = FClamp(yy, 0, ah - 1);
      pSrc = b + yy * bl + xx * ai;
      CopyPixel(pPixel, pSrc, ai);
    }
  }

  free(b);
  return 0;
}

/**
 *  Video (>=24 bit).
@verbatim
    example:
        FCPixelVideo   aCmd (FCPixelVideo::VIDEO_STAGGERED);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelVideo(int ah, int aw, BYTE* a, int al, int ai, int nVideoType)
{
  int x, y;
  int   m_VideoType;
  enum VIDEO_TYPE {VIDEO_STAGGERED = 0, VIDEO_TRIPED = 1, VIDEO_3X3 = 2, VIDEO_DOTS = 3};

  static const int   pattern_width[] = {2, 1, 3, 5};
  static const int   pattern_height[] = {6, 3, 3, 15};
  static const int   video_pattern[4][15 * 5/* max pattern size */] = {
    {
      0, 1,
      0, 2,
      1, 2,
      1, 0,
      2, 0,
      2, 1,
    },
    {
      0,
      1,
      2,
    },
    {
      0, 1, 2,
      2, 0, 1,
      1, 2, 0,
    },
    {
      0, 1, 2, 0, 0,
      1, 1, 1, 2, 0,
      0, 1, 2, 2, 2,
      0, 0, 1, 2, 0,
      0, 1, 1, 1, 2,
      2, 0, 1, 2, 2,
      0, 0, 0, 1, 2,
      2, 0, 1, 1, 1,
      2, 2, 0, 1, 2,
      2, 0, 0, 0, 1,
      1, 2, 0, 1, 1,
      2, 2, 2, 0, 1,
      1, 2, 0, 0, 0,
      1, 1, 2, 0, 1,
      1, 2, 2, 2, 0,
    }
  };


  m_VideoType = (nVideoType);
  assert(nVideoType >= VIDEO_STAGGERED && nVideoType <= VIDEO_DOTS);

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      int i;
      BYTE* pPixel = a + y * al + x * ai;
      int   nWidth = pattern_width[m_VideoType],
            nHeight = pattern_height[m_VideoType];

      for (i = 0; i < 3; i++) {
        if (video_pattern[m_VideoType][nWidth * (y % nHeight) + (x % nWidth)] == i) {
          pPixel[i] = FClamp0255(2 * pPixel[i]);
        }
      }
    }
  }

  return 0;
}

/**
 *  Color balance (>=24 bit).
@verbatim
    example:
        FCPixelColorBalance   aCmd (true, TONE_SHADOWS, -30, 20, 30);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
//=============================================================================
// 色调区域（这三个值的顺序一定不能变）
//=============================================================================
typedef enum TONE_REGION {
  /// shadow region of image.
  TONE_SHADOWS = 0,
  /// midtone region of image.
  TONE_MIDTONES = 1,
  /// highlight region of image.
  TONE_HIGHLIGHTS = 2,
} TONE_REGION;
int FCPixelColorBalance(int ah, int aw, BYTE* a, int al, int ai,
    bool bPreLum, TONE_REGION ToneRgn, int cyan_red, int magenta_green, int yellow_blue)
{
  int x, y;
  //BYTE     m_pLookup[3][256];
  BYTE     m_pLookupR[256], m_pLookupG[256], m_pLookupB[256] ;
  bool     m_bPreserveLuminosity;
  {
    int     i;
    double*  cyan_red_transfer[3], * magenta_green_transfer[3], * yellow_blue_transfer[3];
    int cyan_red_rgn[3] = {0, 0, 0}, magenta_green_rgn[3] = {0, 0, 0}, yellow_blue_rgn[3] = {0, 0, 0};
    double  highlights_add[256], midtones_add[256], shadows_add[256],
            highlights_sub[256], midtones_sub[256], shadows_sub[256];
    m_bPreserveLuminosity = bPreLum;

    cyan_red_rgn[ToneRgn] = cyan_red;
    magenta_green_rgn[ToneRgn] = magenta_green;
    yellow_blue_rgn[ToneRgn] = yellow_blue;

    // add for lightening, sub for darkening

    for (i = 0; i < 256; i++) {
      highlights_add[i] = shadows_sub[255 - i] = (1.075 - 1 / (i / 16.0 + 1));
      midtones_add[i] = midtones_sub[i] = 0.667 * (1 - FSquare((i - 127.0) / 127.0));
      shadows_add[i] = highlights_sub[i] = 0.667 * (1 - FSquare((i - 127.0) / 127.0));
    }

    // Set the transfer arrays (for speed)
    cyan_red_transfer[TONE_SHADOWS] = (cyan_red_rgn[TONE_SHADOWS] > 0) ? shadows_add : shadows_sub;
    cyan_red_transfer[TONE_MIDTONES] = (cyan_red_rgn[TONE_MIDTONES] > 0) ? midtones_add : midtones_sub;
    cyan_red_transfer[TONE_HIGHLIGHTS] = (cyan_red_rgn[TONE_HIGHLIGHTS] > 0) ? highlights_add : highlights_sub;
    magenta_green_transfer[TONE_SHADOWS] = (magenta_green_rgn[TONE_SHADOWS] > 0) ? shadows_add : shadows_sub;
    magenta_green_transfer[TONE_MIDTONES] = (magenta_green_rgn[TONE_MIDTONES] > 0) ? midtones_add : midtones_sub;
    magenta_green_transfer[TONE_HIGHLIGHTS] = (magenta_green_rgn[TONE_HIGHLIGHTS] > 0) ? highlights_add : highlights_sub;
    yellow_blue_transfer[TONE_SHADOWS] = (yellow_blue_rgn[TONE_SHADOWS] > 0) ? shadows_add : shadows_sub;
    yellow_blue_transfer[TONE_MIDTONES] = (yellow_blue_rgn[TONE_MIDTONES] > 0) ? midtones_add : midtones_sub;
    yellow_blue_transfer[TONE_HIGHLIGHTS] = (yellow_blue_rgn[TONE_HIGHLIGHTS] > 0) ? highlights_add : highlights_sub;

    for (i = 0; i < 256; i++) {
      int     r_n = i, g_n = i, b_n = i;

      r_n += (int)(cyan_red_rgn[TONE_SHADOWS] * cyan_red_transfer[TONE_SHADOWS][r_n]);
      r_n = FClamp0255(r_n);
      r_n += (int)(cyan_red_rgn[TONE_MIDTONES] * cyan_red_transfer[TONE_MIDTONES][r_n]);
      r_n = FClamp0255(r_n);
      r_n += (int)(cyan_red_rgn[TONE_HIGHLIGHTS] * cyan_red_transfer[TONE_HIGHLIGHTS][r_n]);
      r_n = FClamp0255(r_n);

      g_n += (int)(magenta_green_rgn[TONE_SHADOWS] * magenta_green_transfer[TONE_SHADOWS][g_n]);
      g_n = FClamp0255(g_n);
      g_n += (int)(magenta_green_rgn[TONE_MIDTONES] * magenta_green_transfer[TONE_MIDTONES][g_n]);
      g_n = FClamp0255(g_n);
      g_n += (int)(magenta_green_rgn[TONE_HIGHLIGHTS] * magenta_green_transfer[TONE_HIGHLIGHTS][g_n]);
      g_n = FClamp0255(g_n);

      b_n += (int)(yellow_blue_rgn[TONE_SHADOWS] * yellow_blue_transfer[TONE_SHADOWS][b_n]);
      b_n = FClamp0255(b_n);
      b_n += (int)(yellow_blue_rgn[TONE_MIDTONES] * yellow_blue_transfer[TONE_MIDTONES][b_n]);
      b_n = FClamp0255(b_n);
      b_n += (int)(yellow_blue_rgn[TONE_HIGHLIGHTS] * yellow_blue_transfer[TONE_HIGHLIGHTS][b_n]);
      b_n = FClamp0255(b_n);

      m_pLookupR[i] = r_n;
      m_pLookupG[i] = g_n;
      m_pLookupB[i] = b_n;
    }
  }

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      BYTE* pPixel = a + y * al + x * ai;
      COLORREF     rgb;
      PCL_B(&rgb) = m_pLookupB[PCL_B(pPixel)];
      PCL_G(&rgb) = m_pLookupG[PCL_G(pPixel)];
      PCL_R(&rgb) = m_pLookupR[PCL_R(pPixel)];

      if (m_bPreserveLuminosity) { // preserve luminosity
        double   H, L, S;
        int   cmax, cmin;
        RGBtoHLS(&rgb, &H, &L, &S);

        // calculate L value
        cmax = FMax(PCL_R(pPixel), FMax(PCL_G(pPixel), PCL_B(pPixel))),
        cmin = FMin(PCL_R(pPixel), FMin(PCL_G(pPixel), PCL_B(pPixel)));
        L = (cmax + cmin) / 2.0 / 255.0;

        rgb = HLStoRGB(H, L, S);
      }

      PCL_B(pPixel) = PCL_B(&rgb);
      PCL_G(pPixel) = PCL_G(&rgb);
      PCL_R(pPixel) = PCL_R(&rgb);
    }
  }

  return 0;
}


/**
 *  Fill grid (>=24 bit).
@verbatim
    example:
        FCPixelFillGrid   aCmd (PCL_RGBA(0,255,0), PCL_RGBA(0,0,255), 5);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
 */
/**
 *  Constructor.
 *  set alpha to 0xFF if image color is 32bpp.
 *  @param nPitch : pixel width of grid.
 */
int FCPixelFillGrid(int ah, int aw, BYTE* a, int al, int ai, COLORREF cr1, COLORREF cr2, int nPitch)
{
  int x, y;
  COLORREF   m_cr1, m_cr2;
  int       m_nPitch;
  m_cr1 = (cr1), m_cr2 = (cr2), m_nPitch = (FMax(1, nPitch));

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      BYTE* pPixel = a + y * al + x * ai;
      int     nX = x / m_nPitch, nY = y / m_nPitch;
      CopyPixel(pPixel, ((nX + nY) % 2 == 0) ? &m_cr1 : &m_cr2, 3);
    }
  }

  return 0;
}

/**
 *  Add 3D grid (>=24 bit).
@verbatim
    example:
        FCPixel3DGrid   aCmd (32, 60);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixel3DGrid(int ah, int aw, BYTE* a, int al, int ai, int nSize, int nDepth)
{
  int x, y;
  int     m_nSize, m_nDepth;
  m_nSize = (FMax(1, nSize)), m_nDepth = (nDepth);

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      BYTE* pPixel = a + y * al + x * ai;
      int     nDelta = 0;

      if (((y - 1) % m_nSize == 0) && (x % m_nSize != 0) && ((x + 1) % m_nSize != 0)) {
        nDelta = -m_nDepth;  // top
      }
      else if (((y + 2) % m_nSize == 0) && (x % m_nSize != 0) && ((x + 1) % m_nSize != 0)) {
        nDelta = m_nDepth;  // bottom
      }
      else if (((x - 1) % m_nSize == 0) && (y % m_nSize != 0) && ((y + 1) % m_nSize != 0)) {
        nDelta = m_nDepth;  // left
      }
      else if (((x + 2) % m_nSize == 0) && (y % m_nSize != 0) && ((y + 1) % m_nSize != 0)) {
        nDelta = -m_nDepth;  // right
      }

      PCL_R(pPixel) = FClamp0255(PCL_R(pPixel) + nDelta);
      PCL_G(pPixel) = FClamp0255(PCL_G(pPixel) + nDelta);
      PCL_B(pPixel) = FClamp0255(PCL_B(pPixel) + nDelta);
    }
  }

  return 0;
}

/**
 *  Median filter (>=24 bit).
@verbatim
    example:
        FCPixelMedianFilter   aCmd (3);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
 */

typedef struct BlockElem {
  COLORREF  crOrigin;
  int      nGray;
} BlockElem;
static int __CompareGray(const int* arg1, const int* arg2)
{
  return ((BlockElem*)arg1)->nGray - ((BlockElem*)arg2)->nGray;
}
int FCPixelMedianFilter(int ah, int aw, BYTE* a, int al, int ai, int nSize)
{
  int x, y;
  int         m_nSize = nSize;
  BlockElem*    m_pBlock;
  int     nLeftTop = (m_nSize) / 2;
  BYTE* b;
  BYTE* pGray;
  int bw = aw + nLeftTop + nLeftTop, bh = ah + nLeftTop + nLeftTop, bl = bw * ai;
  // duplicate edge
  b = ExpandFrame(aw, ah, a, al, ai, nLeftTop);
  pGray = MALLOC(BYTE, bw * bh);
  // backup image
  FCPixelConvertTo8BitGray(bw, bh, b, bl, ai, pGray, bw);

  // calculate gray image
  m_nSize = FMax(2, nSize);
  m_pBlock = MALLOC(BlockElem, m_nSize * m_nSize);
  ASSERT(ai <= 4);

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      BYTE* pPixel = a + y * al + x * ai;
      int m, n, i = 0;

      for (m = 0; m < m_nSize; m++) {
        for (n = 0; n < m_nSize; n++) {
          m_pBlock[i].nGray = pGray[(x + n) + (y + m) * bw];
          CopyPixel(&m_pBlock[i].crOrigin, b + (x + n)*ai + (y + m)*bl, ai);
          i++;
        }
      }

      qsort(m_pBlock, i, sizeof(BlockElem), __CompareGray);
      CopyPixel(pPixel, &m_pBlock[i / 2], 3);
    }
  }

  free(m_pBlock);
  free(pGray);
  free(b);
  return 0;
}


#include "LensFlare.h"


#include "TileReflection.h"

/// Image convolute (>= 24 bit)
/**
 *  Set convolute kernel.
 *  @param nElements : array from top-left of matrix.
 *  @param iBlockLen : width of matrix.
 */
int FCPixelConvolute(int ah, int aw, BYTE* a, int al, int ai, const int* nElements, int iBlockLen, int iDivisor, int nOffset)
{
  int i, x, y;
  int*      m_pElement = 0;
  int     m_iBlock = 0, m_iDivisor = 1, m_nOffset = 0;
  int   nLeftTop;
  BYTE* b;
  int bw, bh, bl;

  if (!nElements || (iBlockLen < 1)) {
    assert(false);
    return 0;
  }

  m_pElement = MALLOC(int, FSquare(iBlockLen));

  for (i = 0; i < FSquare(iBlockLen); i++) {
    m_pElement[i] = nElements[i];
  }

  m_iBlock = iBlockLen;
  m_iDivisor = FMax(1, iDivisor);
  m_nOffset = nOffset;

  nLeftTop = m_iBlock / 2;
  bw = aw + nLeftTop * 2, bh = ah + nLeftTop * 2, bl = bw * ai;

  // duplicate edge, easier to processs
  b = ExpandFrame(aw, ah, a, al, ai, nLeftTop);

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      BYTE* pPixel = a + y * al + x * ai;
      // calculate the sum of sub-block
      int  ix, iy, nSumR = 0, nSumG = 0, nSumB = 0, i = 0;

      for (iy = 0; iy < m_iBlock; iy++) {
        for (ix = 0; ix < m_iBlock; ix++, i++) {
          BYTE*    pOld = b + (x + ix) * ai + (y + iy) * bl;
          nSumB += PCL_B(pOld) * m_pElement[i];
          nSumG += PCL_G(pOld) * m_pElement[i];
          nSumR += PCL_R(pOld) * m_pElement[i];
        }
      }

      // set pixel
      PCL_B(pPixel) = FClamp0255(m_nOffset + nSumB / m_iDivisor);
      PCL_G(pPixel) = FClamp0255(m_nOffset + nSumG / m_iDivisor);
      PCL_R(pPixel) = FClamp0255(m_nOffset + nSumR / m_iDivisor);
    }
  }

  if (m_pElement) {
    free(m_pElement);
  }

  return 0;
}


/**
 *  Detect edges (>=24 bit).
@verbatim
    example:
        FCPixelDetectEdges   aCmd(3);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelDetectEdges(int ah, int aw, BYTE* a, int al, int ai, int nRadius)
{
  /// Constructor (nRadius >= 1).
  int   i, nBlock = 2 * FMax(1, nRadius) + 1,
           nDivisor = nRadius,
           nOffset = 0,
           nWidth = nBlock * nBlock;

  int*   pKernel = MALLOC(int, nWidth);

  for (i = 0; i < nWidth; i++) {
    pKernel[i] = -1;
  }

  pKernel[nWidth / 2] = nWidth - 1;
  FCPixelConvolute(aw, ah, a, al, ai, pKernel, nBlock, nDivisor, nOffset);
  return 0;
}

/**
 *  Sharp (laplacian template) (>=24 bit).
@verbatim
    example:
        FCPixelSharp   aCmd(3);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelSharp(int ah, int aw, BYTE* a, int al, int ai, int nStep)
{
  /// Constructor (nStep >= 1).
  int   arKernel[] = { -1, -1, -1, -1, 8 + nStep, -1, -1, -1, -1},
      nBlock = 3,
      nDivisor = FMax(1, nStep),
      nOffset = 0;
  FCPixelConvolute(aw, ah, a, al, ai, arKernel, nBlock, nDivisor, nOffset);
  return 0;
}



// the function <fmod> is extreme slow :-(, so we just do it.
// the function == fmod (x, 1.0)
#define FDoubleMod1(x)   ((x) - (int)(x))


/**
 *  Constructor.
 *  @param ptStart : start coordinate on image.
 *  @param ptEnd : end coordinate on image.
 *  @param nRepeat : REPEAT_NONE, REPEAT_SAWTOOTH, REPEAT_TRIANGULAR
*/

/// Base class of gradient fill (>=24 bit)
int FCPixelGradientBase(int ah, int aw, BYTE* a, int al, int ai, FCPixelGradientLine_t* gl)
{
  int x, y;

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      BYTE* pPixel = a + y * al + x * ai;
      double     fFac;
      fFac = gl->CalculateFactor(gl, x, y);

      switch (gl->m_nRepeat) {
      case REPEAT_NONE :
        fFac = FClamp(fFac, 0.0, 1.0);
        break;

      case REPEAT_SAWTOOTH :
        if (fFac < 0.0) {
          fFac = 1.0 - FDoubleMod1(-fFac);
        }
        else {
          fFac = FDoubleMod1(fFac);
        }

        break;

      case REPEAT_TRIANGULAR :
        if (fFac < 0.0) {
          fFac = -fFac;
        }

        if (((int)fFac) & 1) {
          fFac = 1.0 - FDoubleMod1(fFac);
        }
        else {
          fFac = FDoubleMod1(fFac);
        }

        break;
      }

      PCL_B(pPixel) = (BYTE)(PCL_B(&gl->m_crStart) + (PCL_B(&gl->m_crEnd) - PCL_B(&gl->m_crStart)) * fFac);
      PCL_G(pPixel) = (BYTE)(PCL_G(&gl->m_crStart) + (PCL_G(&gl->m_crEnd) - PCL_G(&gl->m_crStart)) * fFac);
      PCL_R(pPixel) = (BYTE)(PCL_R(&gl->m_crStart) + (PCL_R(&gl->m_crEnd) - PCL_R(&gl->m_crStart)) * fFac);
    }
  }

  return 0;
}

/**
 *  Gradient fill linear (>=24 bit).
@verbatim
    example:
        POINT     ptStart={0,0}, ptEnd={100,100};
        FCPixelGradientLine  aCmd (ptStart, ptEnd, PCL_RGBA(0,0,0), PCL_RGBA(0,0,255), REPEAT_NONE);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
 */

double FCPixelGradientLine_CalculateFactor(FCPixelGradientLine_t* gl, int nX, int nY)
{
  double   rat = gl->m_fRatX * (nX - gl->m_ptStart.x) + gl->m_fRatY * (nY - gl->m_ptStart.y);
  rat = rat / gl->m_fDist;
  return (rat < 0.0) ? 0.0 : rat;
}

/**
 *  Gradient fill bilinear (>=24 bit).
@verbatim
    example:
        POINT     ptStart={0,0}, ptEnd={100,100};
        FCPixelGradientBiLine  aCmd (ptStart, ptEnd, PCL_RGBA(0,0,0), PCL_RGBA(0,0,255), REPEAT_NONE);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
double FCPixelGradientBiLine_CalculateFactor(FCPixelGradientLine_t* gl, int nX, int nY)
{
  double   rat = gl->m_fRatX * (nX - gl->m_ptStart.x) + gl->m_fRatY * (nY - gl->m_ptStart.y);
  rat = rat / gl->m_fDist;
  return fabs(rat);
}

/**
 *  Gradient fill symmetric conical (>=24 bit).
@verbatim
    example:
        POINT     ptStart={0,0}, ptEnd={100,100};
        FCPixelGradientConicalSym  aCmd (ptStart, ptEnd, PCL_RGBA(0,0,0), PCL_RGBA(0,0,255), REPEAT_NONE);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
double FCPixelGradientConicalSym_CalculateFactor(FCPixelGradientLine_t* gl, int nX, int nY)
{
  double   rat;
  double   dx = nX - gl->m_ptStart.x, dy = nY - gl->m_ptStart.y;

  if ((dx != 0) || (dy != 0)) {
    double    dr = FHypot(dx, dy);
    rat = gl->m_fRatX * dx / dr + gl->m_fRatY * dy / dr;
    rat = FClamp(rat, -1.0, 1.0);
    rat = acos(rat) / LIB_PI;
    rat = FClamp(rat, 0.0, 1.0);
  }
  else {
    rat = 0.5;
  }

  return rat;
}

/**
 *  Gradient fill Anti-symmetric conical (>=24 bit).
@verbatim
    example:
        POINT     ptStart={0,0}, ptEnd={100,100};
        FCPixelGradientConicalSym  aCmd (ptStart, ptEnd, PCL_RGBA(0,0,0), PCL_RGBA(0,0,255), REPEAT_NONE);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
double FCPixelGradientConicalASym_CalculateFactor(FCPixelGradientLine_t* gl, int nX, int nY)
{
  double   rat;
  double   dx = nX - gl->m_ptStart.x, dy = nY - gl->m_ptStart.y;

  if ((dx != 0) || (dy != 0)) {
    double   ang0, ang1, ang;
    ang0 = atan2(gl->m_fRatX, gl->m_fRatY) + LIB_PI;
    ang1 = atan2(dx, dy) + LIB_PI;
    ang = ang1 - ang0;

    if (ang < 0.0) {
      ang += LIB_2PI;
    }

    rat = ang / LIB_2PI;
    rat = FClamp(rat, 0.0, 1.0);
  }
  else {
    rat = 0.5;
  }

  return rat;
}

int FCPixelGradientLine(int ah, int aw, BYTE* a, int al, int ai, POINT ptStart, POINT ptEnd, COLORREF crStart, COLORREF crEnd, REPEAT_MODE nRepeat)
{
  FCPixelGradientLine_t gl[1] = {0};
  gl->m_crStart = (crStart), gl->m_crEnd = (crEnd), gl->m_nRepeat = (nRepeat);
  gl->m_ptStart = ptStart;
  gl->m_ptEnd = ptEnd;
  gl->m_fDist = FHypot(gl->m_ptStart.x - gl->m_ptEnd.x, gl->m_ptStart.y - gl->m_ptEnd.y);
  gl->m_fRatX = (gl->m_ptEnd.x - gl->m_ptStart.x) / gl->m_fDist;
  gl->m_fRatY = (gl->m_ptEnd.y - gl->m_ptStart.y) / gl->m_fDist;
  gl->CalculateFactor = FCPixelGradientLine_CalculateFactor;
  FCPixelGradientBase(aw, ah, a, al, ai, gl);
  return 0;
}

int FCPixelGradientBiLine(int ah, int aw, BYTE* a, int al, int ai, POINT ptStart, POINT ptEnd, COLORREF crStart, COLORREF crEnd, REPEAT_MODE nRepeat)
{
  FCPixelGradientLine_t gl[1] = {0};
  gl->m_crStart = (crStart), gl->m_crEnd = (crEnd), gl->m_nRepeat = (nRepeat);
  gl->m_ptStart = ptStart;
  gl->m_ptEnd = ptEnd;
  gl->m_fDist = FHypot(gl->m_ptStart.x - gl->m_ptEnd.x, gl->m_ptStart.y - gl->m_ptEnd.y);
  gl->m_fRatX = (gl->m_ptEnd.x - gl->m_ptStart.x) / gl->m_fDist;
  gl->m_fRatY = (gl->m_ptEnd.y - gl->m_ptStart.y) / gl->m_fDist;
  gl->CalculateFactor = FCPixelGradientBiLine_CalculateFactor;
  FCPixelGradientBase(aw, ah, a, al, ai, gl);
  return 0;
}

int FCPixelGradientConicalASym(int ah, int aw, BYTE* a, int al, int ai, POINT ptStart, POINT ptEnd, COLORREF crStart, COLORREF crEnd, REPEAT_MODE nRepeat)
{
  FCPixelGradientLine_t gl[1] = {0};
  gl->m_crStart = (crStart), gl->m_crEnd = (crEnd), gl->m_nRepeat = (nRepeat);
  gl->m_ptStart = ptStart;
  gl->m_ptEnd = ptEnd;
  gl->m_fDist = FHypot(gl->m_ptStart.x - gl->m_ptEnd.x, gl->m_ptStart.y - gl->m_ptEnd.y);
  gl->m_fRatX = (gl->m_ptEnd.x - gl->m_ptStart.x) / gl->m_fDist;
  gl->m_fRatY = (gl->m_ptEnd.y - gl->m_ptStart.y) / gl->m_fDist;
  gl->CalculateFactor = FCPixelGradientConicalASym_CalculateFactor;
  FCPixelGradientBase(aw, ah, a, al, ai, gl);
  return 0;
}

/**
 *  Gradient fill rect (>=24 bit).
@verbatim
    example:
        RECT     rc = {0, 0, 100, 100};
        FCPixelGradientRect  aCmd (rc, PCL_RGBA(0,0,0), PCL_RGBA(0,0,255), REPEAT_NONE);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
 */

/**
 *  Constructor.
 *  @param rcRect : rect on image.
 *  @param nRepeat : REPEAT_NONE, REPEAT_SAWTOOTH, REPEAT_TRIANGULAR
*/
double FCPixelGradientRect_CalculateFactor(FCPixelGradientLine_t* gl, int nX, int nY)
{
  double   ratX = fabs((nX - gl->m_fCenX) / gl->m_fRadiusX),
           ratY = fabs((nY - gl->m_fCenY) / gl->m_fRadiusY);
  return FMax(ratX, ratY);
}


/**
 *  Gradient fill radial (>=24 bit).
@verbatim
    example:
        RECT     rc = {0, 0, 100, 100};
        FCPixelGradientRadial  aCmd (rc, PCL_RGBA(0,0,0), PCL_RGBA(0,0,255), REPEAT_NONE);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
/**
 *  Constructor.
 *  @param rcEllipse : rect on image.
 *  @param nRepeat : REPEAT_NONE, REPEAT_SAWTOOTH, REPEAT_TRIANGULAR
*/
double FCPixelGradientRadial_CalculateFactor(FCPixelGradientLine_t* gl, int nX, int nY)
{
  double   rat = FHypot((nX - gl->m_fCenX) / gl->m_fRadiusX, (nY - gl->m_fCenY) / gl->m_fRadiusY);
  return rat;
}

int FCPixelGradientRect(int ah, int aw, BYTE* a, int al, int ai, RECT rcRect, COLORREF crStart, COLORREF crEnd, REPEAT_MODE nRepeat)
{
  FCPixelGradientLine_t gl[1] = {0};
  gl->m_crStart = (crStart), gl->m_crEnd = (crEnd), gl->m_nRepeat = (nRepeat);
  assert(!IsRectEmpty(&rcRect));
  gl->m_fCenX = (rcRect.left + rcRect.right) / 2.0;
  gl->m_fCenY = (rcRect.top + rcRect.bottom) / 2.0;
  gl->m_fRadiusX = RECTWIDTH(rcRect) / 2.0;
  gl->m_fRadiusY = RECTHEIGHT(rcRect) / 2.0;
  gl->CalculateFactor = FCPixelGradientRect_CalculateFactor;
  FCPixelGradientBase(aw, ah, a, al, ai, gl);
  return 0;
}

int FCPixelGradientRadial(int ah, int aw, BYTE* a, int al, int ai, RECT rcRect, COLORREF crStart, COLORREF crEnd, REPEAT_MODE nRepeat)
{
  FCPixelGradientLine_t gl[1] = {0};
  gl->m_crStart = (crStart), gl->m_crEnd = (crEnd), gl->m_nRepeat = (nRepeat);
  assert(!IsRectEmpty(&rcRect));
  gl->m_fCenX = (rcRect.left + rcRect.right) / 2.0;
  gl->m_fCenY = (rcRect.top + rcRect.bottom) / 2.0;
  gl->m_fRadiusX = RECTWIDTH(rcRect) / 2.0;
  gl->m_fRadiusY = RECTHEIGHT(rcRect) / 2.0;
  gl->CalculateFactor = FCPixelGradientRadial_CalculateFactor;
  FCPixelGradientBase(aw, ah, a, al, ai, gl);
  return 0;
}

#define IsInside(x, y)  ((x>=0) && (x<aw) && (y>=0) && (y<ah))

/// Bilinear distord (>=24 bit).
/// if derived class override OnEnterProcess, it must call OnEnterProcess of base.
int FCPixelBilinearDistord(int ah, int aw, BYTE* a, int al, int ai, FCPixelBilinearDistord_t* gl)
{
  int x, y;
  static COLORREF   s_crNull = PCL_RGBA(0xFF, 0xFF, 0xFF, 0);
  BYTE* b = (BYTE*)memdup(a, ah * al);

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      BYTE* pPixel = a + y * al + x * ai;
      double   un_x, un_y;

      if (!gl->calc_undistorted_coord(gl, x, y, &un_x, &un_y)) {
        continue;
      }

      // this is ugly
      un_x = FClamp(un_x, (double) - 0x7FFFFF00, (double)0x7FFFFF00);
      un_y = FClamp(un_y, (double) - 0x7FFFFF00, (double)0x7FFFFF00);

      {
        int nSrcX = (int)((un_x < 0) ? (un_x - 1) : un_x),
            nSrcY = (int)((un_y < 0) ? (un_y - 1) : un_y),
            nSrcX_1 = nSrcX + 1,
            nSrcY_1 = nSrcY + 1;
        const BYTE*    pcrPixel[4] = {
          IsInside(nSrcX, nSrcY) ? (b + nSrcX * ai + al * nSrcY) : (BYTE*)& s_crNull,
          IsInside(nSrcX_1, nSrcY) ? (b + nSrcX_1 * ai + al * nSrcY) : (BYTE*)& s_crNull,
          IsInside(nSrcX, nSrcY_1) ? (b + nSrcX * ai + al * nSrcY_1) : (BYTE*)& s_crNull,
          IsInside(nSrcX_1, nSrcY_1) ? (b + nSrcX_1 * ai + al * nSrcY_1) : (BYTE*)& s_crNull,
        };

        COLORREF   cr = Get_Bilinear_Pixel(un_x - nSrcX, un_y - nSrcY, ai == 4, pcrPixel);
        CopyPixel(pPixel, &cr, ai);
      }
    }
  }

  free(b);
  return 0;
}

/**
 *  Cylinder (>=24 bit).
@verbatim
    example:
        FCPixelCylinder  aCmd;
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
bool FCPixelCylinder_calc_undistorted_coord(FCPixelBilinearDistord_t* gl, int x, int y, double* un_x, double* un_y)
{
  *un_x = gl->m_ColIndex[x];
  *un_y = y;
  return true;
}

int FCPixelCylinder(int ah, int aw, BYTE* a, int al, int ai)
{
  int x;
  FCPixelBilinearDistord_t gl[1] = {0};
  gl->m_ColIndex = MALLOC(double, aw);

  // position on origin image.
  for (x = 0; x < aw; x++) {
    double   R = aw / 2.0;
    gl->m_ColIndex[x] = (2 * R * acos((R - x) / R) / LIB_PI);
  }

  gl->calc_undistorted_coord = FCPixelCylinder_calc_undistorted_coord;
  FCPixelBilinearDistord(aw, ah, a, al, ai, gl);
  return 0;
}

/**
 *  Wave (>=24 bit).
@verbatim
    example:
        FCPixelWave  aCmd (25, 30, 0);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
bool FCPixelWave_calc_undistorted_coord(FCPixelBilinearDistord_t* gl, int x, int y, double* un_x, double* un_y)
{
  const int  nImgWidth = gl->aw,
             nImgHeight = gl->ah;
  double     fScaleX = 1.0, fScaleY = 1.0;

  if (nImgWidth < nImgHeight) {
    fScaleX = nImgHeight / (double)nImgWidth;
  }
  else if (nImgWidth > nImgHeight) {
    fScaleY = nImgWidth / (double)nImgHeight;
  }

  // Distances to center, scaled
  {
    double   fCenX = gl->aw / 2.0,
             fCenY = gl->ah / 2.0,
             dx = (x - fCenX) * fScaleX,
             dy = (y - fCenY) * fScaleY,
             amnt = gl->m_nAmplitude * sin(LIB_2PI * FHypot(dx, dy) / (double)gl->m_nWavelength + gl->m_fPhase);
    *un_x = (amnt + dx) / fScaleX + fCenX;
    *un_y = (amnt + dy) / fScaleY + fCenY;
    *un_x = FClamp(*un_x, 0.0, gl->aw - 1.0);
    *un_y = FClamp(*un_y, 0.0, gl->ah - 1.0);
  }
  return true;
}
int FCPixelWave(int ah, int aw, BYTE* a, int al, int ai, int nWavelength, int nAmplitude, double fPhase)
{
  FCPixelBilinearDistord_t gl[1] = {0};
  gl->aw = aw, gl->ah = ah;
  gl->m_nWavelength = (2 * FMax(1, nWavelength));
  gl->m_nAmplitude = (FMax(1, nAmplitude)), gl->m_fPhase = (fPhase);
  gl->calc_undistorted_coord = FCPixelWave_calc_undistorted_coord;
  FCPixelBilinearDistord(aw, ah, a, al, ai, gl);
  return 0;
}


/**
 *  Whirl & Pinch (>=24 bit).
@verbatim
    example:
        FCPixelWhirlPinch  aCmd (1.5, 0.5);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
bool FCPixelWhirlPinch_calc_undistorted_coord(FCPixelBilinearDistord_t* gl, int x, int y, double* un_x, double* un_y)
{

  int      nImgWidth = gl->aw,
           nImgHeight = gl->ah;
  double   fScaleX = 1.0, fScaleY = 1.0;

  if (nImgWidth < nImgHeight) {
    fScaleX = nImgHeight / (double)nImgWidth;
  }
  else if (nImgWidth > nImgHeight) {
    fScaleY = nImgWidth / (double)nImgHeight;
  }

  // Distances to center, scaled
  {
    double   fCenX = gl->aw / 2.0,
             fCenY = gl->ah / 2.0,
             fRadius = FMax(fCenX, fCenY),
             dx = (x - fCenX) * fScaleX,
             dy = (y - fCenY) * fScaleY;
    double   d = dx * dx + dy * dy; // Distance^2 to center of *circle* (scaled ellipse)
    double   fSqrtD = sqrt(d);

    // If we are inside circle, then distort, else, just return the same position
    bool     bInside = (fSqrtD < fRadius);

    // exclude center point
    if (fSqrtD < FLT_EPSILON) {
      bInside = false;
    }

    if (bInside) {
      double   fAng, sina, cosa;
      //        double  fDist = sqrt (d / m_fRadiusScale) / m_fRadius;
      double  fDist = fSqrtD / fRadius;

      // Pinch
      double  fFactor = pow(sin(LIB_PI / 2.0 * fDist), -gl->m_fPinch);
      dx *= fFactor;
      dy *= fFactor;

      // Whirl
      fAng = gl->m_fWhirl * FSquare(1.0 - fDist);
      sina = sin(fAng), cosa = cos(fAng);
      *un_x = (cosa * dx - sina * dy) / fScaleX + fCenX;
      *un_y = (sina * dx + cosa * dy) / fScaleY + fCenY;
    }
    else {
      *un_x = x;
      *un_y = y;
    }

    return bInside;
  }
}

int FCPixelWhirlPinch(int ah, int aw, BYTE* a, int al, int ai, double fWhirl, double fPinch)
{
  FCPixelBilinearDistord_t gl[1] = {0};
  gl->aw = aw, gl->ah = ah;
  gl->m_fWhirl = FClamp(fWhirl, -LIB_2PI, LIB_2PI);
  gl->m_fPinch = FClamp(fPinch, -1.0, 1.0);
  gl->calc_undistorted_coord = FCPixelWhirlPinch_calc_undistorted_coord;
  FCPixelBilinearDistord(aw, ah, a, al, ai, gl);
  return 0;
}

/**
 *  Fractal trace (>=24 bit).
@verbatim
    example:
        FCPixelFractalTrace  aCmd (2);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int mandelbrot(int m_nDepth, double x, double y, double* u, double* v)
{
  int     iter = 0;
  double  xx = x, yy = y;
  double  x2 = xx * xx,
          y2 = yy * yy;

  while (iter++ < m_nDepth) {
    double   tmp = x2 - y2 + x;
    yy = 2 * xx * yy + y;
    xx = tmp;
    x2 = xx * xx;
    y2 = yy * yy;
  }

  *u = xx;
  *v = yy;
  return 0;
}
bool FCPixelFractalTrace_calc_undistorted_coord(FCPixelBilinearDistord_t* gl, int x, int y, double* un_x, double* un_y)
{
  double   fImgWidth = gl->aw,
           fImgHeight = gl->ah,
           fScaleX = 1.5 / fImgWidth,
           fScaleY = 2.0 / fImgHeight,
           cy = -1.0 + y * fScaleY,
           cx = -1.0 + x * fScaleX,
           px, py;
  mandelbrot(gl->m_nDepth, cx, cy, &px, &py);
  *un_x = (px + 1.0) / fScaleX;
  *un_y = (py + 1.0) / fScaleY;

  if (!(0 <= *un_x && *un_x < fImgWidth && 0 <= *un_y && *un_y < fImgHeight)) {
    *un_x = fmod(*un_x, fImgWidth);
    *un_y = fmod(*un_y, fImgHeight);

    if (*un_x < 0.0) {
      *un_x += fImgWidth;
    }

    if (*un_y < 0.0) {
      *un_y += fImgHeight;
    }
  }

  return true;
}

int FCPixelFractalTrace(int ah, int aw, BYTE* a, int al, int ai, int nDepth)
{
  FCPixelBilinearDistord_t gl[1] = {0};
  gl->aw = aw, gl->ah = ah;
  gl->m_nDepth = (FMax(1, nDepth));
  gl->calc_undistorted_coord = FCPixelFractalTrace_calc_undistorted_coord;
  FCPixelBilinearDistord(aw, ah, a, al, ai, gl);
  return 0;
}


/**
 *  Lens (>=24 bit).
@verbatim
    example:
        FCPixelLens  aCmd (1.5);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
 */

bool FCPixelLens_calc_undistorted_coord(FCPixelBilinearDistord_t* gl, int x, int y, double* un_x, double* un_y)
{
  if (gl->m_bKeepBk) {
    *un_x = x;
    *un_y = y;
  }

  {
    double   fCenX = gl->aw / 2.0,
             fCenY = gl->ah / 2.0,
             asqr = fCenX * fCenX,
             bsqr = fCenY * fCenY,
             csqr = FSquare(FMin(fCenX, fCenY)),
             dy = fCenY - y,
             ysqr = FSquare(dy),
             dx = x - fCenX,
             xsqr = FSquare(dx);

    if (ysqr < (bsqr - (bsqr * xsqr) / asqr)) {
      double  xx, yy, nyangle;
      double  fTmp = sqrt((1 - xsqr / asqr - ysqr / bsqr) * csqr);
      double  fTmpsqr = FSquare(fTmp);

      double  nxangle = acos(dx / sqrt(xsqr + fTmpsqr));
      double  theta2 = asin(sin(LIB_PI / 2.0 - nxangle) / gl->m_fRefraction);
      theta2 = LIB_PI / 2.0 - nxangle - theta2;
      xx = dx - tan(theta2) * fTmp;

      nyangle = acos(dy / sqrt(ysqr + fTmpsqr));
      theta2 = asin(sin(LIB_PI / 2.0 - nyangle) / gl->m_fRefraction);
      theta2 = LIB_PI / 2.0 - nyangle - theta2;
      yy = dy - tan(theta2) * fTmp;

      *un_x = xx + fCenX;
      *un_y = fCenY - yy;
    }

  }

  return true;
}
int FCPixelLens(int ah, int aw, BYTE* a, int al, int ai, double fRefraction, bool bKeepBk)
{
  FCPixelBilinearDistord_t gl[1] = {0};
  gl->aw = aw, gl->ah = ah;
  gl->m_fRefraction = (FMax(1.0, fRefraction)), gl->m_bKeepBk = (bKeepBk);
  gl->calc_undistorted_coord = FCPixelLens_calc_undistorted_coord;
  FCPixelBilinearDistord(aw, ah, a, al, ai, gl);
  return 0;
}

bool FCPixelSkew_calc_undistorted_coord(FCPixelBilinearDistord_t* gl, int x, int y, double* un_x, double* un_y)
{
  if (gl->m_ptNewPos[0].x != gl->m_ptNewPos[3].x) {
    // x axis slope
    int     nDelta = gl->m_ptNewPos[0].x - gl->m_ptNewPos[3].x;
    *un_x = x - ((nDelta > 0) ? (gl->m_nNewHeight - y) : y) * abs(nDelta) / (double)gl->m_nNewHeight;
    *un_y = y * gl->ah / (double)gl->m_nNewHeight;
  }
  else if (gl->m_ptNewPos[0].y != gl->m_ptNewPos[1].y) {
    // y axis slope
    int     nDelta = gl->m_ptNewPos[0].y - gl->m_ptNewPos[1].y;
    *un_x = x * gl->aw / (double)gl->m_nNewWidth;
    *un_y = y - ((nDelta > 0) ? (gl->m_nNewWidth - x) : x) * abs(nDelta) / (double)gl->m_nNewWidth;
  }
  else {
    *un_x = x;
    *un_y = y;
  }

  return true;
}
/**
 *  Skew transform (>=24 bit).
*/
int FCPixelSkew(int ah, int aw, BYTE* a, int al, int ai, POINT ptNewPos[4])
{
  FCPixelBilinearDistord_t gl[1] = {0};
  gl->aw = aw, gl->ah = ah;
  memcpy(gl->m_ptNewPos, ptNewPos, sizeof(POINT) * 4);
  gl->calc_undistorted_coord = FCPixelSkew_calc_undistorted_coord;
  gl->m_nNewWidth = FMax(abs(gl->m_ptNewPos[0].x - gl->m_ptNewPos[2].x), abs(gl->m_ptNewPos[1].x - gl->m_ptNewPos[3].x));
  gl->m_nNewHeight = FMax(abs(gl->m_ptNewPos[0].y - gl->m_ptNewPos[2].y), abs(gl->m_ptNewPos[1].y - gl->m_ptNewPos[3].y));
  FCPixelBilinearDistord(aw, ah, a, al, ai, gl);
  return 0;
}

/**
 *  Perspective transform (>=24 bit).
*/
bool FCPixelPerspective_calc_undistorted_coord(FCPixelBilinearDistord_t* gl, int x, int y, double* un_x, double* un_y)
{
  if (gl->m_ptNewPos[0].y != gl->m_ptNewPos[1].y) {
    // y axis perspective
    int     nDelta = abs(gl->m_ptNewPos[0].y - gl->m_ptNewPos[3].y) - abs(gl->m_ptNewPos[1].y - gl->m_ptNewPos[2].y);
    double  fOffset = fabs(nDelta * ((nDelta > 0) ? x : (gl->m_nNewWidth - x)) / (2.0 * gl->m_nNewWidth));
    *un_y = gl->ah * (y - fOffset) / (gl->m_nNewHeight - 2.0 * fOffset);
    *un_x = gl->aw * x / (double)gl->m_nNewWidth;
  }
  else if (gl->m_ptNewPos[0].x != gl->m_ptNewPos[3].x) {
    // x axis perspective
    int     nDelta = abs(gl->m_ptNewPos[0].x - gl->m_ptNewPos[1].x) - abs(gl->m_ptNewPos[2].x - gl->m_ptNewPos[3].x);
    double  fOffset = fabs(nDelta * ((nDelta > 0) ? y : (gl->m_nNewHeight - y)) / (2.0 * gl->m_nNewHeight));
    *un_x = gl->aw * (x - fOffset) / (gl->m_nNewWidth - 2.0 * fOffset);
    *un_y = gl->ah * y / (double)gl->m_nNewHeight;
  }
  else {
    *un_x = x;
    *un_y = y;
  }

  return true;
}
int FCPixelPerspective(int ah, int aw, BYTE* a, int al, int ai, POINT ptNewPos[4])
{
  FCPixelBilinearDistord_t gl[1] = {0};
  gl->aw = aw, gl->ah = ah;
  memcpy(gl->m_ptNewPos, ptNewPos, sizeof(POINT) * 4);
  gl->calc_undistorted_coord = FCPixelPerspective_calc_undistorted_coord;
  gl->m_nNewWidth = FMax(abs(gl->m_ptNewPos[0].x - gl->m_ptNewPos[1].x), abs(gl->m_ptNewPos[2].x - gl->m_ptNewPos[3].x));
  gl->m_nNewHeight = FMax(abs(gl->m_ptNewPos[0].y - gl->m_ptNewPos[3].y), abs(gl->m_ptNewPos[1].y - gl->m_ptNewPos[2].y));
  FCPixelBilinearDistord(aw, ah, a, al, ai, gl);
  return 0;
}

/**
 *  Rotate image (>=24 bit).
@verbatim
    example:
        FCPixelRotate   aCmd (45);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
static int RotatePoint(double* x, double* y, double center_x, double center_y, double fAngle)
{
  double   dx = *x - center_x, dy = -*y + center_y,
           cost = cos(fAngle), sint = sin(fAngle);
  *x = center_x + (dx * cost + dy * sint);
  *y = center_y - (dy * cost - dx * sint);
  return 0;
}
bool FCPixelRotate_calc_undistorted_coord(FCPixelBilinearDistord_t* gl, int x, int y, double* un_x, double* un_y)
{
  double   cen_x = (gl->m_nNewWidth - 1) / 2.0, cen_y = (gl->m_nNewHeight - 1) / 2.0;

  *un_x = x;
  *un_y = y;
  RotatePoint(un_x, un_y, cen_x, cen_y, gl->m_fInvAngle);

  *un_x -= (gl->m_nNewWidth - gl->aw) / 2.0;
  *un_y -= (gl->m_nNewHeight - gl->ah) / 2.0;
  return true;
}

/**
 *  Constructor.
 *  @param nAngle : 0 --> 360
 */
int FCPixelRotate(int ah, int aw, BYTE* a, int al, int ai, int nAngle)
{
  FCPixelBilinearDistord_t gl[1] = {0};
  // calculate new width & height
  double   cen_x = aw / 2.0, cen_y = ah / 2.0,
           x1 = 0, y1 = 0,
           x2 = aw, y2 = 0,
           x3 = 0, y3 = ah,
           x4 = aw, y4 = ah;

  RotatePoint(&x1, &y1, cen_x, cen_y, gl->m_fAngle);
  RotatePoint(&x2, &y2, cen_x, cen_y, gl->m_fAngle);
  RotatePoint(&x3, &y3, cen_x, cen_y, gl->m_fAngle);
  RotatePoint(&x4, &y4, cen_x, cen_y, gl->m_fAngle);

  {
    double  L = FMin(x1, FMin(x2, FMin(x3, x4))),
            T = FMin(y1, FMin(y2, FMin(y3, y4))),
            R = FMax(x1, FMax(x2, FMax(x3, x4))),
            B = FMax(y1, FMax(y2, FMax(y3, y4)));

    gl->m_nNewWidth = (int)ceil(R - L);
    gl->m_nNewHeight = (int)ceil(B - T);

    gl->aw = aw, gl->ah = ah;

    while (nAngle < 0) {
      nAngle += 360;
    }

    nAngle %= 360;
    gl->m_fAngle = AngleToRadian(nAngle);
    gl->m_fInvAngle = AngleToRadian(360 - nAngle);
  }
  gl->aw = aw, gl->ah = ah;
  gl->calc_undistorted_coord = FCPixelRotate_calc_undistorted_coord;
  FCPixelBilinearDistord(aw, ah, a, al, ai, gl);
  return 0;
}

/**
 *  Ribbon (>=24 bit).
@verbatim
    example:
        FCPixelRibbon   aCmd (80, 30);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
/// Constructor.
/// @param nSwing : [0..100],  percentage
/// @param nFrequency : >=0,  a pi every 10
bool FCPixelRibbon_calc_undistorted_coord(FCPixelBilinearDistord_t* gl, int x, int y, double* un_x, double* un_y)
{
  *un_x = x;
  *un_y = y + gl->m_fDelta - gl->m_ShiftDown[x];
  return true;
}
int FCPixelRibbon(int ah, int aw, BYTE* a, int al, int ai, int nSwing, int nFrequency)
{
  double   fAngleSpan;
  int i, x;
  FCPixelBilinearDistord_t gl[1] = {0};
  gl->m_nSwing = FClamp(nSwing, 0, 100);
  gl->m_nFreq = FMax(nFrequency, 0);

  // clear image
  //memset(pImg->GetMemStart(), 0, pImg->GetPitch()*ah);

  gl->m_fDelta = gl->m_nSwing * ah * 75.0 / 100 / 100; // upper, max 75%

  for (i = 0; i < (int)gl->m_fDelta; i++) {
    //memset(a+i*al, 0, al);
  }

  fAngleSpan = gl->m_nFreq * LIB_PI / 10.0 / aw;

  gl->m_ShiftDown = MALLOC(double, aw);

  for (x = 0; x < aw; x++) {
    double   d = (1 - cos(x * fAngleSpan)) * gl->m_fDelta / 2.0;
    gl->m_ShiftDown[x] = (d);
  }

  gl->aw = aw, gl->ah = ah;
  gl->calc_undistorted_coord = FCPixelRibbon_calc_undistorted_coord;
  FCPixelBilinearDistord(aw, ah, a, al, ai, gl);
  free(gl->m_ShiftDown);
  return 0;
}

/**
 *  Ripple (>=24 bit).
@verbatim
    example:
        FCPixelRipple   aCmd (10, 30);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
bool FCPixelRipple_calc_undistorted_coord(FCPixelBilinearDistord_t* gl, int x, int y, double* un_x, double* un_y)
{
  double  tt, w = gl->aw;
  int nPos = y;

  if (gl->m_bSinType) {
    tt = gl->m_nAmplitude * sin(nPos * LIB_2PI / (double)gl->m_nWavelength);
  }
  else {
    tt = floor(gl->m_nAmplitude * (fabs((((nPos % gl->m_nWavelength) / (double)gl->m_nWavelength) * 4) - 2) - 1));
  }

  *un_x = fmod(x + w + tt, w);
  *un_x = FClamp(*un_x, 0.0, w - 1);
  *un_y = y;
  return true;
}
int FCPixelRipple(int ah, int aw, BYTE* a, int al, int ai, int nWavelength, int nAmplitude, bool bSinType)
{
  FCPixelBilinearDistord_t gl[1] = {0};
  gl->m_nWavelength = FMax(1, nWavelength);
  gl->m_nAmplitude = FMax(1, nAmplitude);
  gl->m_bSinType = bSinType;
  gl->aw = aw, gl->ah = ah;
  gl->calc_undistorted_coord = FCPixelRipple_calc_undistorted_coord;
  FCPixelBilinearDistord(aw, ah, a, al, ai, gl);
  return 0;
}

/// LUT(look up table) routine (>=24 bit)
int FCPixelLUTRoutine(int ah, int aw, BYTE* a, int al, int ai, int* m_LUT)
{
  int x, y;
  int     m_bChannelR, m_bChannelG, m_bChannelB;
  IMAGE_CHANNEL nChannel = CHANNEL_RGBA;
  m_bChannelR = nChannel & CHANNEL_RED;
  m_bChannelG = nChannel & CHANNEL_GREEN;
  m_bChannelB = nChannel & CHANNEL_BLUE;


  for (y = 0; y < ah; ++y) {
    for (x = 0; x < aw; ++x) {
      BYTE* pPixel = a + y * al + x * ai;

      if (m_bChannelB) {
        PCL_B(pPixel) = m_LUT[PCL_B(pPixel)];
      }

      if (m_bChannelG) {
        PCL_G(pPixel) = m_LUT[PCL_G(pPixel)];
      }

      if (m_bChannelR) {
        PCL_R(pPixel) = m_LUT[PCL_R(pPixel)];
      }
    }
  }

  return 0;
}
/**
 *  Adjust brightness (>=24 bit).
@verbatim
    example:
        FCPixelBrightness   aCmd (150);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
 */
int FCPixelBrightness(int ah, int aw, BYTE* a, int al, int ai, int nLevel)
{
  int i, m_LUT[256];
  int   m_nLevel;
  m_nLevel = FClamp(nLevel, 0, 200);
  m_nLevel -= 100;

  for (i = 0; i < 256; i++) {
    int nLUTIndex = i;

    if (m_nLevel <= 0) {
      m_LUT[i] =  FClamp0255(nLUTIndex + nLUTIndex * m_nLevel / 200);
    }
    else {
      m_LUT[i] =  FClamp0255(nLUTIndex + (255 - nLUTIndex) * m_nLevel / 200);
    }
  }

  FCPixelLUTRoutine(aw, ah, a, al, ai, m_LUT);
  return 0;
}

/**
 *  Adjust contrast (>=24 bit).
@verbatim
    example:
        FCPixelContrast   aCmd (150);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelContrast(int ah, int aw, BYTE* a, int al, int ai, int nLevel)
{
  int i, m_LUT[256];
  int   m_nLevel;
  m_nLevel = FClamp(nLevel, 0, 200);
  m_nLevel -= 100;

  for (i = 0; i < 256; i++) {
    int nLUTIndex = i;
    double   contrast = (double)m_nLevel / 100.0,
             value = (double)nLUTIndex / 255.0,
             n = (value > 0.5) ? (1.0 - value) : value;

    if (n < 0) {
      n = 0;
    }

    if (contrast < 0) {
      n = 0.5 * pow(2.0 * n, 1.0 + contrast);
    }
    else {
      double   power = (m_nLevel == 100) ? 127 : (1.0 / (1.0 - contrast));
      n = 0.5 * pow(2.0 * n, power);
    }

    value = (value > 0.5) ? (1.0 - n) : n;
    m_LUT[i] = FClamp0255((int)(value * 255));
  }

  FCPixelLUTRoutine(aw, ah, a, al, ai, m_LUT);
  return 0;
}

/**
 *  Adjust gamma (>=24 bit).
@verbatim
    example:
        FCPixelGamma   aCmd (0.5);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
 */
int FCPixelGamma(int ah, int aw, BYTE* a, int al, int ai, double fGamma)
{
  int i, m_LUT[256];
  double     m_fInvGamma;
  fGamma = FMax(0.0, fGamma);
  m_fInvGamma = 1.0 / fGamma;

  for (i = 0; i < 256; i++) {
    int nLUTIndex = i;
    double   fMax = pow(255.0, m_fInvGamma) / 255.0;
    m_LUT[i] = FClamp0255(FRound(pow((double)nLUTIndex, m_fInvGamma) / fMax));
  }

  FCPixelLUTRoutine(aw, ah, a, al, ai, m_LUT);
  return 0;
}

/**
 *  Negate image(>=24 bit).
@verbatim
    example:
        FCPixelInvert   aCmd;
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelInvert(int ah, int aw, BYTE* a, int al, int ai)
{
  int i, m_LUT[256];

  for (i = 0; i < 256; i++) {
    int nLUTIndex = i;
    m_LUT[i] = (255 - nLUTIndex);
  }

  FCPixelLUTRoutine(aw, ah, a, al, ai, m_LUT);
  return 0;
}


/**
 *  Solarize image(>=24 bit).
@verbatim
    example:
        FCPixelSolarize   aCmd (128);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelSolarize(int ah, int aw, BYTE* a, int al, int ai, int nThreshold)
{
  int i, m_LUT[256];
  int     m_nThreshold = (FClamp0255(nThreshold));

  for (i = 0; i < 256; i++) {
    int nLUTIndex = i;
    m_LUT[i] = (nLUTIndex >= m_nThreshold) ? (255 - nLUTIndex) : nLUTIndex;
  }

  FCPixelLUTRoutine(aw, ah, a, al, ai, m_LUT);
  return 0;
}

/**
 *  Posterize image(>=24 bit).
@verbatim
    example:
        FCPixelPosterize   aCmd (2);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelPosterize(int ah, int aw, BYTE* a, int al, int ai, int nLevel)
{
  int i, m_LUT[256];
  int   m_nLevel;
  m_nLevel = (FMax(2, nLevel));

  for (i = 0; i < 256; i++) {
    int nLUTIndex = i;
    double   d = 255.0 / (m_nLevel - 1.0);
    m_LUT[i] =  FClamp0255(FRound(d * FRound(nLUTIndex / d)));
  }

  FCPixelLUTRoutine(aw, ah, a, al, ai, m_LUT);
  return 0;
}

/**
*  Count image's number of color (>=24 bit).
@verbatim
    example:
        FCPixelColorsCount   aCmd;
        img.SinglePixelProcessProc (aCmd);
        aCmd.GetColorsNumber();
@endverbatim
*/
/**
*  Find a color unused in image (>=24 bit).
@verbatim
    example:
        FCPixelGetKeyColor   aCmd;
        img.SinglePixelProcessProc (aCmd);
        aCmd.IsFind();
        aCmd.GetKeyColor();
@endverbatim
*/
int FCPixelColorsCount(int ah, int aw, BYTE* a, int al, int ai, int nLevel, COLORREF* m_crKey)
{
  int x, y;
  int m_nCount = (0);
  BYTE* m_pMap;
  int     iMaxColor = 1 << 24;
  m_pMap = MALLOC(BYTE, iMaxColor + 1);
  memset(m_pMap, 0, iMaxColor + 1);

  for (y = 0; y < ah; ++y) {
    for (x = 0; x < aw; ++x) {
      BYTE* pPixel = a + y * al + x * ai;
      DWORD     i = 0;
      CopyPixel(&i, pPixel, 3);

      if (m_pMap[i] == 0) {
        m_pMap[i] = 1;
        m_nCount++;
      }
    }
  }

  if (m_crKey) {
    int i;

    for (i = 0; i <= 0xFFFFFF; i++) {
      if (m_pMap[i] == 0) {
        *(DWORD*)&m_crKey = i;
        break;
      }
    }

  }

  free(m_pMap);
  return m_nCount;
}


/**
 *  Save a ASCII text file (>=24 bit).
@verbatim
    example:
        FCPixelExportAscII   aCmd ("c:\\PhoXo.txt");
        img.SinglePixelProcessProc (aCmd);
@endverbatim
 */
int FCPixelExportAscII(int ah, int aw, BYTE* a, int al, int ai, const char* szFileName)
{
  char     m_chIndex[95];
  int      m_nGray[95];
  FILE*      m_pFile;
  int i, j;

  char   ch[95] = {
    ' ',
    '`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\\',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '|',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?'
  };
  int   gr[95] = {
    0,
    7, 22, 28, 31, 31, 27, 32, 22, 38, 32, 40, 6, 12, 20, 38, 32, 26, 20, 24, 40,
    29, 24, 28, 38, 32, 32, 26, 22, 34, 24, 44, 33, 32, 32, 24, 16, 6, 22, 26, 22,
    26, 34, 29, 35, 10, 6, 20, 14, 22, 47, 42, 34, 40, 10, 35, 21, 22, 22, 16, 14,
    26, 40, 39, 29, 38, 22, 28, 36, 22, 36, 30, 22, 22, 36, 26, 36, 25, 34, 38, 24,
    36, 22, 12, 12, 26, 30, 30, 34, 39, 42, 41, 18, 18, 22
  };

  // Bubble Sort
  for (i = 0; i < 94; i++) {
    for (j = i + 1; j < 95; j++) {
      if (gr[i] > gr[j]) {
        BYTE t;
        FSwap(ch[i], ch[j], t);
        FSwap(gr[i], gr[j], t);
      }
    }
  }

  memcpy(m_chIndex, ch, 95 * sizeof(char));
  memcpy(m_nGray, gr, 95 * sizeof(int));

  m_pFile = fopen(szFileName, "wb");
  assert(m_pFile);

  {
    const int     nTransWidth = aw / 8,
                  nTransHeight = ah / 16;
    int x, y;
    // most of image is brightness

    for (y = 0; y < nTransHeight; y++) {
      char    tchar;

      for (x = 0; x < nTransWidth; x++) {
        int t, k, h, nGray = 0;

        for (k = 0; k < 16; k++) {
          for (h = 0; h < 8; h++) {
            BYTE*    pGray = a + (8 * x + h) * ai + al * (y * 16 + k);
            nGray += 255 - GetGrayscale(pGray);
          }
        }

        nGray /= 16 * 8;
        nGray = m_nGray[94] * nGray / 255;
        t = 0;

        while (m_nGray[t + 1] < nGray) {
          t++;
        }

        fwrite(&m_chIndex[t], 1, sizeof(char), m_pFile);
      }

      tchar = (char)0x0D;
      fwrite(&tchar, 1, sizeof(char), m_pFile);
      tchar = (char)0x0A;
      fwrite(&tchar, 1, sizeof(char), m_pFile);
    }
  }
  fclose(m_pFile);
  return 0;
}

#include "Blur_Box.h"


/**
 *  Blur zoom (>=24 bit).
@verbatim
    example:
        FCPixelBlur_Zoom   aCmd (15);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelBlur_Zoom(int ah, int aw, BYTE* a, int al, int ai, int nLength)
{
  int m_nLength = (FMax(0, nLength));
  int x, y;
  BYTE* b = memdup(a, ah * al);

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      // Stat.
      int     nSumB = 0, nSumG = 0, nSumR = 0, nSumA = 0, i = 0;
      BYTE*    pWrite;

      for (i = 0; i < m_nLength; i++) {
        BYTE* p;
        int    nA;
        int     nCenX = aw / 2, nCenY = ah / 2,
                xx = (int)(nCenX + (x - nCenX) * (1.0 + 0.02 * i)),
                yy = (int)(nCenY + (y - nCenY) * (1.0 + 0.02 * i));

        if (!IsInside(xx, yy)) {
          break;
        }

        p = b + xx * ai + al * yy;
        nA = (ai == 4) ? PCL_A(p) : 0xFF;
        nSumA += nA;
        nSumB += nA * PCL_B(p);
        nSumG += nA * PCL_G(p);
        nSumR += nA * PCL_R(p);
      }

      // set pixel
      pWrite = a + x * ai + al * y;

      if (nSumA) {
        PCL_B(pWrite) = nSumB / nSumA;
        PCL_G(pWrite) = nSumG / nSumA;
        PCL_R(pWrite) = nSumR / nSumA;
      }

      if ((ai == 4) && i) {
        PCL_A(pWrite) = nSumA / i;
      }
    }

  }

  free(b);
  return 0;
}

/**
 *  Blur radial (>=24 bit).
@verbatim
    example:
        FCPixelBlur_Radial   aCmd (30);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelBlur_Radial(int ah, int aw, BYTE* a, int al, int ai, int nAngle)
{
  int x, y, i;
  int     m_nAngle = (abs(nAngle) % 360); // [0, 360]
  BYTE* b = memdup(a, ah * al);
  int   nCenX = aw / 2, nCenY = ah / 2,
        R = (int)FHypot(FMax(nCenX, aw - nCenX), FMax(nCenY, ah - nCenY)),
        n = (int)(4 * AngleToRadian(m_nAngle) * sqrt((double)R) + 2);

  double*   ct = MALLOC(double, n),
            *st = MALLOC(double, n);
  double   theta = (double)AngleToRadian(m_nAngle) / ((double)(n - 1)),
           offset = (double)(theta * (n - 1) / 2.0);

  for (i = 0; i < n; i++) {
    ct[i] = cos(theta * i - offset);
    st[i] = sin(theta * i - offset);
  }

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      BYTE*    pWrite;
      BYTE*    p;
      int    nA;
      int     nSumB = 0, nSumG = 0, nSumR = 0, nSumA = 0,
              nCount = 0;
      int   xr = x - nCenX, yr = y - nCenY,
            r = (int)sqrt((double)FSquare(xr) + (double)FSquare(yr)),
            nStep;

      if (r == 0) {
        nStep = 1;
      }
      else {
        nStep = R / r;

        if (nStep == 0) {
          nStep = 1;
        }
        else if (nStep > n - 1) {
          nStep = n - 1;
        }
      }

      // Stat.
      nSumB = 0, nSumG = 0, nSumR = 0, nSumA = 0,
      nCount = 0;

      for (i = 0; i < n; i += nStep) {
        int     xx = (int)(nCenX + xr * ct[i] - yr * st[i]),
                yy = (int)(nCenY + xr * st[i] + yr * ct[i]);

        if (!IsInside(xx, yy)) {
          continue;
        }

        nCount++;

        p = b + xx * ai + al * yy;
        nA = (ai == 4) ? PCL_A(p) : 0xFF;
        nSumA += nA;
        nSumB += nA * PCL_B(p);
        nSumG += nA * PCL_G(p);
        nSumR += nA * PCL_R(p);
      }

      // set pixel
      pWrite = a + (x * ai + al * y);

      if (nSumA) {
        PCL_B(pWrite) = nSumB / nSumA;
        PCL_G(pWrite) = nSumG / nSumA;
        PCL_R(pWrite) = nSumR / nSumA;
      }

      if ((ai == 4) && nCount) {
        PCL_A(pWrite) = nSumA / nCount;
      }
    }

  }

  free(b);
  free(st);
  free(ct);
  return 0;
};

/**
 *  Blur motion (>=24 bit).
@verbatim
    example:
        FCPixelBlur_Motion   aCmd (15, DIRECT_LEFT);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelBlur_Motion(int ah, int aw, BYTE* a, int al, int ai, int nStep, DIRECT_SYS Direct)
{
  DIRECT_SYS  m_Direct;
  int         m_nStep; // (>= 2)
  int y;
  m_nStep = (nStep), m_Direct = (Direct);

  if (aw < 5) {
    return 0;
  }

  m_nStep = FClamp(m_nStep, 2, (int)aw - 2);

  for (y = 0; y < ah; y++) {
    int     B = 0, G = 0, R = 0, A = 0, i = 0;
    int       nCurrX = 0,
              nSpanX = 1;
    COLORREF   rgb; // pixel at edge

    switch (m_Direct) {
    case DIRECT_LEFT :
      CopyPixel(&rgb, a + (aw - 1)*ai + al * (y), ai);
      nCurrX = 0;
      nSpanX = 1;
      break;

    case DIRECT_RIGHT :
      CopyPixel(&rgb, a + (0)*ai + al * (y), ai);
      nCurrX = aw - 1;
      nSpanX = -1;
      break;
    }

    // first block
    B = 0, G = 0, R = 0, A = 0, i = 0;

    for (i = 0; i < m_nStep; i++) {
      BYTE*    p = a + (nCurrX + i * nSpanX) * ai + al * (y);
      B += PCL_B(p);
      G += PCL_G(p);
      R += PCL_R(p);
      A += PCL_A(p);
    }

    // move block
    for (i = 0; i < aw - 2; i++, nCurrX += nSpanX) { // leave 2 pixel edge
      int     newB = FClamp0255(B / m_nStep),  // don't set pixel current
              newG = FClamp0255(G / m_nStep),
              newR = FClamp0255(R / m_nStep),
              newA = FClamp0255(A / m_nStep);
      BYTE*     p = a + (nCurrX) * ai + al * (y);

      // step
      if (i >= aw - m_nStep) { // edge
        B = B - PCL_B(p) + PCL_B(&rgb);
        G = G - PCL_G(p) + PCL_G(&rgb);
        R = R - PCL_R(p) + PCL_R(&rgb);

        if (ai == 4) {
          A = A - PCL_A(p) + PCL_A(&rgb);
        }
      }
      else {
        BYTE*    pA = a + (nCurrX + nSpanX * m_nStep) * ai + al * (y);
        B = B - PCL_B(p) + PCL_B(pA);
        G = G - PCL_G(p) + PCL_G(pA);
        R = R - PCL_R(p) + PCL_R(pA);

        if (ai == 4) {
          A = A - PCL_A(p) + PCL_A(pA);
        }
      }

      PCL_B(p) = newB;
      PCL_G(p) = newG;
      PCL_R(p) = newR;

      if (ai == 4) {
        PCL_A(p) = newA;
      }
    }

  }

  return 0;
}

#include "Blur_IIRGauss.h"

#include "SoftGlow.h"

/**
 *  Add inner bevel frame (>=24 bit).
@verbatim
    example:
        FCPixelInnerBevel   aCmd (20, 10);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
// the temporary object adjust brightness
BYTE* ToRGBA(int ah, int aw, const BYTE* a, int al, int ai)
{
  int x, y;
  BYTE* b = MALLOC(BYTE, ah * aw * 4);
  int bi = 4, bl = aw * bi;

  if (3 == ai) {
    for (y = 0; y < ah; y++) {
      for (x = 0; x < aw; x++) {
        const BYTE* a1 = a + (x) * ai + al * (y);
        BYTE* b1 = b + (x) * bi + bl * (y);
        b1[0] = a1[0], b1[1] = a1[1], b1[2] = a1[2], b1[3] = 0xff;
      }
    }
  }
  else if (1 == ai) {
    for (y = 0; y < ah; y++) {
      for (x = 0; x < aw; x++) {
        const BYTE* a1 = a + (x) * ai + al * (y);
        BYTE* b1 = b + (x) * bi + bl * (y);
        b1[0] = a1[0], b1[1] = a1[0], b1[2] = a1[0], b1[3] = 0xff;
      }
    }
  }

  return b;
}
int FromRGBA(int ah, int aw, const BYTE* a, int al, int ai, BYTE* b, int bl, int bi)
{
  int x, y;
  ASSERT(ai == 4);

  if (3 == bi) {
    for (y = 0; y < ah; y++) {
      for (x = 0; x < aw; x++) {
        const BYTE* a1 = a + (x) * ai + al * (y);
        BYTE* b1 = b + (x) * bi + bl * (y);
        b1[0] = a1[0], b1[1] = a1[1], b1[2] = a1[2];
      }
    }
  }
  else if (1 == bi) {
    for (y = 0; y < ah; y++) {
      for (x = 0; x < aw; x++) {
        const BYTE* a1 = a + (x) * ai + al * (y);
        BYTE* b1 = b + (x) * bi + bl * (y);
        b1[0] = (a1[0] + a1[1] + a1[1] + a1[2]) >> 2;
      }
    }
  }

  return 0;
}
int __FCPixelFillInnerBevel(int ah, int aw, BYTE* a, int al, int ai)
{
  int x, y;
  ASSERT(4 == ai);

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      BYTE* pPixel = a + (x) * ai + al * (y);
      BYTE* p = a + (x) * ai + al * (y);
      PCL_B(pPixel) = FClamp0255(PCL_B(pPixel) * PCL_A(p) / 100);
      PCL_G(pPixel) = FClamp0255(PCL_G(pPixel) * PCL_A(p) / 100);
      PCL_R(pPixel) = FClamp0255(PCL_R(pPixel) * PCL_A(p) / 100);
    }
  }

  return 0;
}

int FCPixelInnerBevel(int ah, int aw, BYTE* a, int al, int ai, int nSize, int nSmooth)
{
  int x, y;
  int m_nSize = FMax(1, nSize);
  int m_nSmooth = nSmooth;
  // image brightness
  const int    nLeft = 160, nTop = 160, nRight = 40, nBottom = 40;
  BYTE* b = ToRGBA(aw, ah, a, al, ai);
  int bl = aw * 4, bi = 4;

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      BYTE*    p = b + (x) * bi + bl * (y);

      if ((x < m_nSize) && (y < ah - x) && (y > x)) {
        PCL_A(p) = nLeft;
      }
      else if ((y < m_nSize) && (x < aw - y) && (x > y)) {
        PCL_A(p) = nTop;
      }
      else if ((x > aw - m_nSize) && (y > aw - x) && (y < ah + x - aw)) {
        PCL_A(p) = nRight;
      }
      else if (y > ah - m_nSize) {
        PCL_A(p) = nBottom;
      }
      else {
        PCL_A(p) = 100;
      }
    }
  }

  FCPixelBlur_Box(aw, ah, b, bl, bi, m_nSmooth);
  __FCPixelFillInnerBevel(aw, ah, b, bl, bi);
  FromRGBA(aw, ah, b, bl, bi, a, al, ai);
  return 0;
}


/**
 *  Smooth edge (32 bit).
@verbatim
    example:
        FCPixelSmoothEdge   aCmd (15);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
BYTE* FCPixelCreateHalo(int nRadius, int crCenter, int crEdge, int* pWidth)
{
  int x, y;
  double   m_fRadius;
  int      m_crCenter, m_crEdge, nWidth;
  POINT    m_ptCenter;
  int aw, ah, al, ai = 1;
  BYTE* a;

  m_crCenter = (crCenter), m_crEdge = (crEdge);

  m_fRadius = FMax(1, nRadius);
  m_fRadius += 1.0;
  m_ptCenter.x = (int)m_fRadius;
  m_ptCenter.y = (int)m_fRadius;

  nWidth = (int)(2 * m_fRadius + 1);
  al = ah = aw = nWidth;
  *pWidth = nWidth;
  a = MALLOC(BYTE, ah * aw);

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      BYTE* pPixel = a + (x) * ai + al * (y);
      double   fDist = FHypot(x - m_ptCenter.x, y - m_ptCenter.y);

      if (fDist <= m_fRadius) {
        *pPixel = FRound(m_crCenter + (m_crEdge - m_crCenter) * fDist / m_fRadius);
      }
      else {
        *pPixel = m_crEdge;
      }
    }
  }

  return a;
}
static int RecordHaloPoint(int ah, int aw, BYTE* a, int al, int ai, POINT* listHalo)
{
  // expand edge to search easily
  int x, y, i, sw, sh, sl, listHaloLen;
  BYTE* imgAlpha = a;
  BYTE* imgSearch;
  BYTE* imgRecord;
  POINT nDirect[4] = {{0, -1}, {0, 1}, { -1, 0}, {1, 0}}; // up-down-left-right
  ASSERT(ai == 1);
  imgSearch = ExpandFrame(aw, ah, imgAlpha, al, ai, 1);
  sw = aw + 2, sh = ah + 2, sl = sw * ai;

  // record halo point
  imgRecord = MALLOC(BYTE, sw * sh);
  memset(imgRecord, 0, sw * sh);

  for (y = 1; y < sh - 1; y++) {
    for (x = 1; x < sw - 1; x++) {
      BYTE*    p = imgSearch + (x) * ai + sl * (y);

      for (i = 0; i < 4; i++) {
        POINT   pt;
        // direction
        int    nDX = x + nDirect[i].x, nDY = y + nDirect[i].y;
        BYTE*    pTmp = imgSearch + (nDX) * ai + sl * (nDY);

        if (*pTmp == *p) {
          continue;
        }

        // draw halo at smaller alpha

        if (*pTmp < *p) {
          pt.x = nDX;
          pt.y = nDY;
        }
        else {
          pt.x = x;
          pt.y = y;
        }

        imgRecord[pt.x * ai + sl * pt.y] = 0xFF;
      }
    }
  }

  // halo point
  listHaloLen = 0;

  for (y = 0; y < sh; y++) {
    for (x = 0; x < sw; x++) {
      if (imgRecord[x * ai + al * y] == 0xFF) {
        POINT pt = {x - 1, y - 1}; // remember -1
        listHalo[listHaloLen++] = (pt);
      }
    }
  }

  free(imgSearch);
  free(imgRecord);
  return listHaloLen;
}


int DrawHalo(int dw, int dh, BYTE* imgDest, int dl, int ah, int aw, BYTE* imgAlpha, int al, int hw, BYTE* imgHalo, POINT ptCenter)
{
  int x, y;
  int    nLT = (hw - 1) / 2;
  int ai = 1;
  RECT   rcHalo = {ptCenter.x - nLT, ptCenter.y - nLT, ptCenter.x + nLT + 1, ptCenter.y + nLT + 1},
         rcAlpha = {0, 0, aw, ah}, rcDest;
  int    nCenter = 0;

  if (IntersectRect(&rcDest, &rcHalo, &rcAlpha) == 0) {
    return 0;
  }

  if (IsInside(ptCenter.x, ptCenter.y)) {
    nCenter = *imgAlpha + (ptCenter.x) * ai + al * (ptCenter.y);
  }

  for (y = rcDest.top; y < rcDest.bottom; y++) {
    for (x = rcDest.left; x < rcDest.right; x++) {
      BYTE*    pDest = imgDest + (x) * ai + dl * (y),
               * pAlpha = imgAlpha + (x) * ai + al * (y),
                 * pHalo = imgHalo + (x - rcHalo.left) + hw * (y - rcHalo.top);
      // calculate percentage
      int    nNew = FMax(nCenter, *pAlpha * *pHalo / 0xFF);

      if (nNew < *pDest) {
        *pDest = nNew;
      }
    }
  }

  return 0;
}

int CopyChannel(int ah, int aw, const BYTE* a, int al, int ai, int aj, BYTE* b, int bl, int bi, int bj)
{
  int x, y;

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      b[bj + (x) * bi + bl * (y)] = a[aj + (x) * ai + al * (y)];
    }
  }

  return 0;
}

int FCPixelSmoothEdge(int ah, int aw, BYTE* a, int al, int ai, int iBlock)
{
  int i, listHaloLen, hw;
  int     m_iBlock; // >=1
  POINT* listHalo;
  BYTE* imgAlpha;
  BYTE* imgHalo;
  BYTE* imgDest;

  m_iBlock = (FMax(1, iBlock));

  // get alpha channel
  imgAlpha = MALLOC(BYTE, aw * ah);
  CopyChannel(aw, ah, a, al, ai, 3, imgAlpha, aw, 1, 0);

  // get draw halo point
  listHalo = MALLOC(POINT, aw * ah);
  listHaloLen = RecordHaloPoint(aw, ah, imgAlpha, aw, 1, listHalo);

  if (listHaloLen == 0) {
    return 0;
  }

  // create halo image
  imgHalo = FCPixelCreateHalo(m_iBlock, 0, 0xFF, &hw);

  // make alpha image
  imgDest = memdup(imgAlpha, aw * ah);

  for (i = 0; i < listHaloLen; i++) {
    DrawHalo(aw, ah, imgDest, aw, aw, ah, imgAlpha, aw, hw, imgHalo, listHalo[i]);
  }

  CopyChannel(aw, ah, imgDest, aw, 1, 0, a, al, ai, 3);
  //pImg->AppendAlphaChannel(imgDest);
  free(imgHalo);
  free(imgDest);
  free(imgAlpha);
  free(listHalo);
  return 0;
}

/// Calculate optimized image's rect.
int FCPixelGetOptimizedRect(int ah, int aw, const BYTE* a, int al, int ai)
{
  int x, y;
  RECT     m_rcOptimized;
  bool     m_bFirst;

  memset(&m_rcOptimized, 0, sizeof(m_rcOptimized));
  m_bFirst = true;

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      const BYTE* pPixel = a + x * ai + y * al;

      if (PCL_A(pPixel)) {
        if (m_bFirst) {
          m_rcOptimized.left = x;
          m_rcOptimized.right = x + 1;
          m_rcOptimized.top = y;
          m_rcOptimized.bottom = y + 1;
          m_bFirst = false;
        }
        else {
          if (x < m_rcOptimized.left) {
            m_rcOptimized.left = x;
          }

          if (x + 1 > m_rcOptimized.right) {
            m_rcOptimized.right = x + 1;
          }

          if (y < m_rcOptimized.top) {
            m_rcOptimized.top = y;
          }

          if (y + 1 > m_rcOptimized.bottom) {
            m_rcOptimized.bottom = y + 1;
          }
        }
      }
    }
  }

  return 0;
}


#include "FCObjImage.inl"

#if 0



/**
 *  Add shadow (32 bit).
@verbatim
    example:
    SHADOWDATA    ShData;
        ShData.crShadow = crWhite();
        ShData.nAlpha = 75;
        ShData.nSmooth = 10;
        ShData.nOffsetX = 5;
        ShData.nOffsetY = 5;
        FCPixelAddShadow   aCmd (ShData);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelAddShadow(int ah, int aw, const BYTE* a, int al, int ai, SHADOWDATA ShData)
{
  SHADOWDATA   m_ShadowData;

  m_ShadowData = ShData;
  m_ShadowData.nSmooth = FMax(2, (int)m_ShadowData.nSmooth);
  m_ShadowData.nAlpha = FClamp((int)m_ShadowData.nAlpha, 1, 100);


  {
    // backup image
    const FCObjImage   imgOld(*pImg);

    // calculate new image size
    RECT    rcImg = {0, 0, aw, ah},
            rcShadowOffset = rcImg;
    ::OffsetRect(&rcShadowOffset, m_ShadowData.nOffsetX, m_ShadowData.nOffsetY);
    RECT     rcShadow = rcShadowOffset;
    ::InflateRect(&rcShadow, m_ShadowData.nSmooth, m_ShadowData.nSmooth);
    RECT     rcResult;
    ::UnionRect(&rcResult, &rcImg, &rcShadow);

    // create shadow background and box-blur it
    pImg->Create(RECTWIDTH(rcResult), RECTHEIGHT(rcResult), 32);
    int     nStartX = rcShadowOffset.left - rcResult.left,
            nStartY = rcShadowOffset.top - rcResult.top;

    for (int y = 0; y < imgOld.Height(); y++) {
      for (int x = 0; x < imgOld.Width(); x++) {
        COLORREF   cr = m_ShadowData.crShadow;
        PCL_A(&cr) = PCL_A(imgOld.GetBits(x, y)) * m_ShadowData.nAlpha / 100;
        *(COLORREF*)pImg->GetBits(nStartX + x, nStartY + y) = cr;
      }
    }

    // box-blur alpha-channel
    FCPixelBlur_Box   cmdSmooth(m_ShadowData.nSmooth, false);
    pImg->SinglePixelProcessProc(cmdSmooth, pProgress);

    // combine origin image
    pImg->CombineImage(imgOld, rcImg.left - rcResult.left, rcImg.top - rcResult.top);

    // adjust new img's position
    pImg->SetGraphObjPos(imgOld.GetGraphObjPos().x - rcImg.left + rcResult.left,
        imgOld.GetGraphObjPos().y - rcImg.top + rcResult.top);
  }

};


/**
 *  Frame image with pattern (>=24 bit).
@verbatim
    example:
        FCObjImage   img ("border.jpg");
        FCPixelPatternFrame   aCmd (img);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
 */
class FCPixelPatternFrame : public FCPixelWholeImageBase
{
public:
  /**
   *  Constructor.
   */
  FCPixelPatternFrame(FCObjImage imgTop) {
    imgTop.ConvertTo32Bit();
    imgTop.SetAlphaChannelValue(0xFF);

    m_imgLeft = m_imgTop = imgTop;
    FCPixelRotate270   aCmd;
    m_imgLeft.SinglePixelProcessProc(aCmd);
  }
private:
  virtual int ProcessWholeImage(FCObjImage* pImg, FCObjProgress* pProgress) {
    if (!m_imgTop.IsValidImage()) {
      return;
    }

    int   nBorder = m_imgTop.Height(),
          w = aw,
          h = ah,
          nSpan = ai, t;

    if ((aw < 2 * nBorder) || (ah < 2 * nBorder)) {
      return;
    }

    for (int i = 0; i < nBorder; i++, w -= 2, h -= 2) {
      // top & bottom
      for (t = 0; t < w; t++) {
        int     x = i + t;
        BYTE*     pS = m_imgTop.GetBits(x % m_imgTop.Width(), i);
        CopyPixel(pImg->GetBits(x, i), pS, nSpan);
        CopyPixel(pImg->GetBits(x, ah - 1 - i), pS, nSpan);
      }

      // left & right
      for (t = 0; t < h; t++) {
        int     y = i + t;
        BYTE*     pS = m_imgLeft.GetBits(i, y % m_imgLeft.Height());
        CopyPixel(pImg->GetBits(i, y), pS, nSpan);
        CopyPixel(pImg->GetBits(aw - 1 - i, y), pS, nSpan);
      }
    }
  }

  FCObjImage   m_imgTop;
  FCObjImage   m_imgLeft;
};


/**
 *  Fill a gradient frame (>=24 bit).
@verbatim
    example:
        FCPixelFillGradientFrame   aCmd (crBlack(), crWhite(), 5);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
 */
class FCPixelFillGradientFrame : public FCPixelPatternFrame
{
public:
  /**
   *  Constructor.
   *  @param nWidth : pixel width of border.
   */
  FCPixelFillGradientFrame(COLORREF crOut, COLORREF crIn, int nWidth) : FCPixelPatternFrame(MakeGradientBorder(crOut, crIn, nWidth)) {}
private:
  static FCObjImage MakeGradientBorder(COLORREF crOut, COLORREF crIn, int nWidth) {
    FCObjImage   img(1, nWidth, 32);
    const POINT  ptStart = {0, 0},
                 ptEnd = {0, img.Height() - 1};
    FCPixelGradientLine   aCmd(ptStart, ptEnd, crOut, crIn);
    img.SinglePixelProcessProc(aCmd);
    return img;
  }
};


// inline Implement


#endif
