
#include <math.h>


#define CLR_TO_RGBA(c) c | 0xFF000000
#define RGB_TO_RGBA(r, g, b) CLR_TO_RGBA(_RGB(r, g, b))
//#define _RGBA(r, g, b, a) _RGB(r, g, b) | (a << 24)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int CreateBitmap_32(CBitmap* hbmp, const CSize& size, void** pBits)
{
  ASSERT(0 < size.w);
  ASSERT(0 != size.h);

  if (pBits != NULL) {
    *pBits = NULL;
  }

  if (size.w <= 0 || size.h == 0) {
    return NULL;
  }

  LPVOID pData = NULL;
  hbmp->CreateBitmap(size.w, size.h, 1, 32, NULL, NULL);

  if (pData != NULL && hbmp != NULL && pBits != NULL) {
    *pBits = pData;
  }

  return 0;
}

int CreateBitmap_32(CBitmap* hbmp, CBitmap* bitmap, COLORREF clrTransparent = -1)
{
  if (bitmap == NULL) {
    ASSERT(FALSE);
    return NULL;
  }

  hbmp->CreateBitmap(bitmap->w, bitmap->h, 1, 32, NULL, NULL);

  {
    DWORD nSizeImage = bitmap->w * bitmap->h;

    if (bitmap->c == 4) {
      memcpy(hbmp->tt.data, bitmap->tt.data, nSizeImage * 4);
    }
    else {
      COLORREF* pBits = (COLORREF*) hbmp->tt.data;

      if (clrTransparent == -1) {
        for (DWORD i = 0; i < nSizeImage; i++) {
          *pBits |= 0xFF000000;
          pBits++;
        }
      }
      else {
        COLORREF clrTrans = _RGB(GetBV(clrTransparent),
            GetGV(clrTransparent),
            GetRV(clrTransparent));

        for (DWORD i = 0; i < nSizeImage; i++) {
          if (*pBits != clrTrans) {
            *pBits |= 0xFF000000;
          }
          else {
            *pBits = (COLORREF) 0;
          }

          pBits++;
        }
      }

    }
  }

  return 0;
}

//****
void MirrorRect(img_t* im, const IRECT* pclip, CRect rect, BOOL bHorz = TRUE)
{
  CRect rectClip = iRectClip(pclip, 0, 0, im->w, im->h);

  if (rect.Height() <= 0 || rect.Width() <= 0) {
    return;
  }

  if (!iRectInclude(rectClip, rect)) {
    return;
  }

  int cx = rect.Width();
  int cy = rect.Height();

  if (im->tt.data == NULL) {
    ASSERT(FALSE);
    return;
  }

  if (bHorz) {
    for (int y = 0; y < cy; y++) {
      COLORREF* pLeft = (COLORREF*)(im->tt.data + (rect.t + y) * im->s + rect.l * im->c);
      COLORREF* pRight = (COLORREF*)(im->tt.data + (rect.t + y) * im->s + (rect.l + cx - 1) * im->c);

      for (; pLeft < pRight; ++pLeft, --pRight) {
        COLORREF t0 = *pLeft;
        COLORREF t1 = *pRight;
        *pLeft = t1;
        *pRight = t0;
      }
    }
  }
  else {
    for (int y = 0; y <= cy / 2; y++) {
      COLORREF* pTop = (COLORREF*)(im->tt.data + (rect.t + y) * im->s + rect.l * im->c);
      COLORREF* pBot = (COLORREF*)(im->tt.data + (rect.t + y + cy - 1) * im->s + (rect.l) * im->c);

      for (int x = 0; x < cx; x++, ++pTop, ++pBot) {
        COLORREF t;
        CV_SWAP(*pTop, *pBot, t);
      }
    }
  }

}

//****
void _FillGradient(img_t* im, const IRECT* pclip, CRect rect,
    COLORREF colorStart, COLORREF colorFinish,
    BOOL bHorz = TRUE,
    int nStartFlatPercentage = 0,
    int nEndFlatPercentage = 0)
{
  if (colorStart == colorFinish) {
    imdraw_rect(im, pclip, rect, colorStart, 0, 0, 0);
    return;
  }

  if (nStartFlatPercentage > 0) {
    ASSERT(nStartFlatPercentage <= 100);

    if (bHorz) {
      CRect rectTop = rect;
      rectTop.b = rectTop.t + rectTop.Height() * nStartFlatPercentage / 100;
      rect.t = rectTop.b;

      imdraw_rect(im, pclip, rectTop, colorFinish, 0, 0, 0);
    }
    else {
      CRect rectLeft = rect;
      rectLeft.r = rectLeft.l +
          rectLeft.Width() * nStartFlatPercentage / 100;
      rect.l = rectLeft.r;

      imdraw_rect(im, pclip, rectLeft, colorStart, 0, 0, 0);
    }
  }

  if (nEndFlatPercentage > 0) {
    ASSERT(nEndFlatPercentage <= 100);

    if (bHorz) {
      CRect rectBottom = rect;
      rectBottom.t = rectBottom.b -         rectBottom.Height() * nEndFlatPercentage / 100;
      rect.b = rectBottom.t;

      imdraw_rect(im, pclip, rectBottom, colorStart, 0, 0, 0);
    }
    else {
      CRect rectRight = rect;
      rectRight.l = rectRight.r -         rectRight.Width() * nEndFlatPercentage / 100;
      rect.r = rectRight.l;

      imdraw_rect(im, pclip, rectRight, colorFinish, 0, 0, 0);
    }
  }



  if (nEndFlatPercentage + nStartFlatPercentage > 100) {
    ASSERT(FALSE);
    return;
  }

  imdraw_rect_gradient(im, pclip, rect, colorStart, colorFinish, bHorz);

  // this will make 2^6 = 64 fountain steps
#if 0
  int nShift = 6;
  int nSteps = 1 << nShift;

  for (int i = 0; i < nSteps; i++) {
    // do a little alpha blending
    BYTE bR = (BYTE)((GetRV(colorStart) * (nSteps - i) + GetRV(colorFinish) * i) >> nShift);
    BYTE bG = (BYTE)((GetGV(colorStart) * (nSteps - i) + GetGV(colorFinish) * i) >> nShift);
    BYTE bB = (BYTE)((GetBV(colorStart) * (nSteps - i) + GetBV(colorFinish) * i) >> nShift);

    CBrush br(_RGB(bR, bG, bB));

    // then paint with the resulting color
    CRect r2 = rect;

    if (bHorz) {
      r2.b = rect.b - ((i * rect.Height()) >> nShift);
      r2.t = rect.b - (((i + 1) * rect.Height()) >> nShift);

      if (r2.Height() > 0) {
        m_dc.FillRect(r2, &br);
      }
    }
    else {
      r2.l = rect.l +
          ((i * rect.Width()) >> nShift);
      r2.r = rect.l +
          (((i + 1) * rect.Width()) >> nShift);

      if (r2.Width() > 0) {
        m_dc.FillRect(r2, &br);
      }
    }
  }

#endif
}
void FillGradient(img_t* im, const IRECT* pclip, CRect rect,
    COLORREF colorStart, COLORREF colorFinish,
    BOOL bHorz = TRUE,
    int nStartFlatPercentage = 0,
    int nEndFlatPercentage = 0)
{
  _FillGradient(im, pclip, rect, colorStart, colorFinish, bHorz,
      nStartFlatPercentage, nEndFlatPercentage);
}
//****
void FillGradient2(img_t* im, const IRECT* pclip, CRect rect, COLORREF colorStart, COLORREF colorFinish,
    int nAngle = 0 /* 0 - 360 */
                  )
{
  imdraw_gradient2(im, pclip, rect, colorStart, colorFinish, nAngle);
}
//****
void Fill4ColorsGradient(img_t* im, const IRECT* pclip, CRect rect,
    COLORREF colorStart1, COLORREF colorFinish1,
    COLORREF colorStart2, COLORREF colorFinish2,
    BOOL bHorz = TRUE,
    int nPercentage = 50 /* 0 - 100 */
                        )
{
  ASSERT(nPercentage >= 0);
  ASSERT(nPercentage <= 100);

  CRect rectFirst = rect;
  CRect rectSecond = rect;

  if (!bHorz) {
    rectFirst.r = rectFirst.l + rectFirst.Width() * nPercentage / 100;
    rectSecond.l = rectFirst.r;
  }
  else {
    rectFirst.b = rectFirst.t + rectFirst.Height() * nPercentage / 100;
    rectSecond.t = rectFirst.b;
  }

  FillGradient(im, pclip, rectFirst, colorStart1, colorFinish1, bHorz);
  FillGradient(im, pclip, rectSecond, colorStart2, colorFinish2, bHorz);
}
// ==================================================================
//
// FUNCTION :  PixelAlpha()
//
// * Description : Shades a color value with a specified percentage
//
// * Author : [Guillaume Nodet]
//
// * Returns : [COLORREF] - The result pixel
//
// * Function parameters :
// [srcPixel] - The source pixel
// [dstPixel] - The destination pixel
// [percent] -  Percentage (amount of shadow)
//
// ==================================================================
COLORREF PixelAlpha(COLORREF srcPixel, COLORREF dstPixel, int percent)
{
  int ipercent = 100 - percent;
  COLORREF clrFinal = _RGB((GetRV(srcPixel) * percent + GetRV(dstPixel) * ipercent) / 100,
      (GetGV(srcPixel) * percent + GetGV(dstPixel) * ipercent) / 100,
      (GetBV(srcPixel) * percent + GetBV(dstPixel) * ipercent) / 100);

  return (clrFinal);
}

//****
BOOL DrawGradientRing(img_t* im, const IRECT* pclip, CRect rect,
    COLORREF colorStart, COLORREF colorFinish,
    COLORREF colorBorder,
    int nAngle /* 0 - 360 */,
    int nWidth,
    COLORREF clrFace = (COLORREF) - 1
                     )
{
  int cx = rect.Width();
  int cy = rect.Height();

  if (cx <= 4 || cy <= 4) {
    //--------------------
    // Rectangle too small
    //--------------------
    return FALSE;
  }

  int xOrig = rect.l;
  int yOrig = rect.t;

  const int xCenter = (rect.l + rect.r) / 2;
  const int yCenter = (rect.t + rect.b) / 2;

  const int nSteps = 360;
  const double fDelta = 2. * PI / nSteps;
  const double fStart = PI * nAngle / 180;
  const double fFinish = fStart + 2. * PI;

  double rDelta = (double)(.5 + GetRV(colorFinish) - GetRV(colorStart)) / nSteps * 2;
  double gDelta = (double)(.5 + GetGV(colorFinish) - GetGV(colorStart)) / nSteps * 2;
  double bDelta = (double)(.5 + GetBV(colorFinish) - GetBV(colorStart)) / nSteps * 2;

  COLORREF* pBits = (COLORREF*)(im->tt.data + rect.t * im->s + rect.l * im->c);

  for (int nLevel = 0; nLevel < nWidth; nLevel++) {
    int i = 0;
    const int nRadius = min(rect.Width(), rect.Height()) / 2;
    const int nRectDelta = rect.Width() - rect.Height();

    if (clrFace != (COLORREF) - 1 && nLevel == 0) {
      //---------------
      // Fill interior:
      //---------------
      CBrush brFill(clrFace);

      if (nRectDelta == 0) { // Circle
        imdraw_EllipseR(im, pclip, rect, clrFace, 0, 0);
      }
      else if (nRectDelta > 0) { // Horizontal
        imdraw_Ellipse(im, pclip, rect.l, rect.t, rect.l + rect.Height(), rect.b, clrFace, 0, 0);
        imdraw_Ellipse(im, pclip, rect.r - rect.Height(), rect.t, rect.r, rect.b, clrFace, 0, 0);
        imdraw_rect(im, pclip, iRECT(rect.l + rect.Height() / 2, rect.t, rect.r - rect.Height() / 2, rect.b), clrFace, 0, 0, 0);
      }
      else { // Vertical
        imdraw_Ellipse(im, pclip, rect.l, rect.t, rect.r, rect.t + rect.Width(), clrFace, 0, 0);
        imdraw_Ellipse(im, pclip, rect.l, rect.b - rect.Width(), rect.r, rect.b, clrFace, 0, 0);
        imdraw_rect(im, pclip, iRECT(rect.l, rect.t + rect.Width() / 2, rect.r, rect.b - rect.Width() / 2), clrFace, 0, 0, 0);
      }
    }

    int xPrev = -1;
    int yPrev = -1;

    for (double fAngle = fStart; fAngle < fFinish + fDelta; fAngle += fDelta, i ++) {
      const int nStep = fAngle <= (fFinish + fStart) / 2 ? i : nSteps - i;

      const BYTE bR = (BYTE) max(0, min(255, (.5 + rDelta * nStep + GetRV(colorStart))));
      const BYTE bG = (BYTE) max(0, min(255, (.5 + gDelta * nStep + GetGV(colorStart))));
      const BYTE bB = (BYTE) max(0, min(255, (.5 + bDelta * nStep + GetBV(colorStart))));

      COLORREF color = nLevel == 0 && colorBorder != -1 ?
          colorBorder : _RGB(bR, bG, bB);

      double dx = /*(fAngle >= 0 && fAngle <= PI / 2) || (fAngle >= 3 * PI / 2) ? .5 : -.5*/0;
      double dy = /*(fAngle <= PI) ? .5 : -.5*/0;

      int x = xCenter + (int)(dx + cos(fAngle) * nRadius);
      int y = yCenter + (int)(dy + sin(fAngle) * nRadius);

      if (nRectDelta > 0) {
        if (x > xCenter) {
          x += (int)(.5 * nRectDelta);
        }
        else {
          x -= (int)(.5 * nRectDelta);
        }

        if (xPrev != -1 && (xPrev > xCenter) != (x > xCenter)) {
          for (int x1 = min(x, xPrev); x1 < max(x, xPrev); x1++) {
            imdraw_SetPixel(im, pclip, rect.l + x1, rect.t + y, color);
          }
        }
      }
      else if (nRectDelta < 0) {
        if (y > yCenter) {
          y -= (int)(.5 * nRectDelta);
        }
        else {
          y += (int)(.5 * nRectDelta);
        }

        if (yPrev != -1 && (yPrev > yCenter) != (y > yCenter)) {
          for (int y1 = min(y, yPrev); y1 < max(y, yPrev); y1++) {
            imdraw_SetPixel(im, pclip, rect.l + x, rect.t + y1, color);
          }
        }
      }

      imdraw_SetPixel(im, pclip, rect.l + x, rect.t + y, color);

      xPrev = x;
      yPrev = y;
    }

    rect.DeflateRect(1, 1);
  }

  return TRUE;
}
//****
BOOL DrawShadow(img_t* im, const IRECT* pclip, CRect rect, int nDepth,
    int iMinBrightness = 100,   // Min. brighttness
    int iMaxBrightness = 50,    // Max. beightness
    CBitmap* pBmpSaveBottom = NULL, // Bitmap may be saved in
    CBitmap* pBmpSaveRight = NULL,  // two bitmaps
    COLORREF clrBase = 0, // Base color
    BOOL bRightShadow = TRUE)
{
#if 0
  ASSERT(nDepth >= 0);

  if (nDepth == 0 || rect.IsRectEmpty()) {
    return TRUE;
  }

  int cx = rect.Width();
  int cy = rect.Height();

  const BOOL bIsLeft = !bRightShadow;

  if (pBmpSaveRight != NULL && pBmpSaveRight->GetSafeHandle() != NULL &&
      pBmpSaveBottom != NULL && pBmpSaveBottom->GetSafeHandle() != NULL) {
    //---------------------------------------------------
    // Shadows are already implemented, put them directly
    // to the DC:
    //---------------------------------------------------
    m_dc.DrawState(CPoint(
        bIsLeft ? rect.l - nDepth : rect.r,
        rect.t),
        CSize(nDepth, cy + nDepth),
        pBmpSaveRight, DSS_NORMAL);

    m_dc.DrawState(CPoint(
        bIsLeft ? rect.l - nDepth : rect.l,
        rect.b),
        CSize(cx + nDepth, nDepth),
        pBmpSaveBottom, DSS_NORMAL);
    return TRUE;
  }

  ASSERT(pBmpSaveRight == NULL || pBmpSaveRight->GetSafeHandle() == NULL);
  ASSERT(pBmpSaveBottom == NULL || pBmpSaveBottom->GetSafeHandle() == NULL);

  //--------------------------------------------
  // Copy screen content into the memory bitmap:
  //--------------------------------------------
  CDC dcMem;

  if (!dcMem.CreateCompatibleDC(&m_dc)) {
    ASSERT(FALSE);
    return FALSE;
  }

  //--------------------------------------------
  // Gets the whole menu and changes the shadow.
  //--------------------------------------------
  CBitmap bmpMem;

  if (!bmpMem.CreateCompatibleBitmap(&m_dc, cx + nDepth, cy + nDepth)) {
    ASSERT(FALSE);
    return FALSE;
  }

  CBitmap* pOldBmp = dcMem.SelectObject(&bmpMem);
  ASSERT(pOldBmp != NULL);

  COLORREF* pBits;
  CBitmap* hmbpDib = CreateBitmap_32(CSize(cx + nDepth, cy + nDepth), (LPVOID*)&pBits);

  if (hmbpDib == NULL || pBits == NULL) {
    ASSERT(FALSE);
    return FALSE;
  }

  dcMem.SelectObject(hmbpDib);
  dcMem.BitBlt(0, 0, cx + nDepth, cy + nDepth, &m_dc,
      bIsLeft ? rect.l - nDepth : rect.l,
      rect.t, SRCCOPY);

  BOOL bOldAlgorithm = TRUE;

#ifndef _BCGSUITE_

  if (globalData.bIsOSAlphaBlendingSupport) {
    CBCGPShadowRenderer shadow;

    if (shadow.Create(nDepth, clrBase, 100 - iMinBrightness, 100 - iMaxBrightness, CBCGPToolBarImages::IsRTL())) {
      shadow.DrawFrame(&dcMem, CRect(nDepth, nDepth, cx + nDepth, cy + nDepth));
      bOldAlgorithm = FALSE;
    }
  }

#endif

  if (bOldAlgorithm) {
    //----------------------------------------------------------------------------
    // Process shadowing:
    // For having a very nice shadow effect, its actually hard work. Currently,
    // I'm using a more or less "hardcoded" way to set the shadows (by using a
    // hardcoded algorythm):
    //
    // This algorythm works as follows:
    //
    // It always draws a few lines, from left to bottom, from bottom to right,
    // from right to up, and from up to left). It does this for the specified
    // shadow width and the color settings.
    //-----------------------------------------------------------------------------

    // For speeding up things, iShadowOffset is the
    // value which is needed to multiply for each shadow step
    int iShadowOffset = (iMaxBrightness - iMinBrightness) / nDepth;

    // Loop for drawing the shadow
    // Actually, this was simpler to implement than I thought
    for (int c = 0; c < nDepth; c++) {
      // Draw the shadow from left to bottom
      for (int y = cy; y < cy + (nDepth - c); y++) {
        SetAlphaPixel(pBits, rect, c + nDepth, y,
            iMaxBrightness - ((nDepth  - c) * (iShadowOffset)), nDepth, clrBase, bIsLeft);
      }

      // Draw the shadow from left to right
      for (int x = nDepth + (nDepth - c); x < cx + c; x++) {
        SetAlphaPixel(pBits, rect, x, cy + c,
            iMaxBrightness - ((c) * (iShadowOffset)), nDepth, clrBase, bIsLeft);
      }

      // Draw the shadow from top to bottom
      for (int y1 = nDepth + (nDepth - c); y1 < cy + c + 1; y1++) {
        SetAlphaPixel(pBits, rect, cx + c, y1,
            iMaxBrightness - ((c) * (iShadowOffset)),
            nDepth, clrBase, bIsLeft);
      }

      // Draw the shadow from top to left
      for (int x1 = cx; x1 < cx + (nDepth - c); x1++) {
        SetAlphaPixel(pBits, rect, x1, c + nDepth,
            iMaxBrightness - ((nDepth - c) * (iShadowOffset)),
            nDepth, clrBase, bIsLeft);
      }
    }
  }

  //-----------------------------------------
  // Copy shadowed bitmap back to the screen:
  //-----------------------------------------
  m_dc.BitBlt(bIsLeft ? rect.l - nDepth : rect.l,
      rect.t,
      cx + nDepth, cy + nDepth, &dcMem, 0, 0, SRCCOPY);

  //------------------------------------
  // Save shadows in the memory bitmaps:
  //------------------------------------
  if (pBmpSaveRight != NULL) {
    pBmpSaveRight->CreateCompatibleBitmap(&m_dc, nDepth + 1, cy + nDepth);

    dcMem.SelectObject(pBmpSaveRight);
    dcMem.BitBlt(0, 0, nDepth, cy + nDepth,
        &m_dc, bIsLeft ? 0 : rect.r, rect.t, SRCCOPY);
  }

  if (pBmpSaveBottom != NULL) {
    pBmpSaveBottom->CreateCompatibleBitmap(&m_dc, cx + nDepth, nDepth + 1);

    dcMem.SelectObject(pBmpSaveBottom);
    dcMem.BitBlt(0, 0, cx + nDepth, nDepth, &m_dc,
        bIsLeft ? rect.l - nDepth : rect.l,
        rect.b, SRCCOPY);
  }

  dcMem.SelectObject(pOldBmp);
  DeleteObject(hmbpDib);

#endif

  return TRUE;
}

inline int sqr(int value)
{
  return value * value;
};

inline double sqr(double value)
{
  return value * value;
};

inline int sign(int value)
{
  if (value == 0) {
    return 0;
  }
  else {
    if (value > 0) {
      return 1;
    }
  }

  return -1;
}

inline double sign(double value)
{
  if (value == 0.0) {
    return 0.0;
  }
  else {
    if (value > 0.0) {
      return 1.0;
    }
  }

  return -1.0;
}

inline double frac(double value)
{
  return value - floor(value);
}

COLORREF PixelAlpha(COLORREF srcPixel, int percent)
{
  // My formula for calculating the transpareny is as
  // follows (for each single color):
  //
  //                 percent
  // destPixel = sourcePixel * ( ------- )
  //                               100
  //
  // This is not real alpha blending, as it only modifies the brightness,
  // but not the color (a real alpha blending had to mix the source and
  // destination pixels, e.g. mixing green and red makes yellow).
  // For our nice "menu" shadows its good enough.

  COLORREF clrFinal = _RGB(min(255, (GetRV(srcPixel) * percent) / 100),
      min(255, (GetGV(srcPixel) * percent) / 100),
      min(255, (GetBV(srcPixel) * percent) / 100));

  //  TRACE ("%d %d %d\n", GetRV (clrFinal), GetGV (clrFinal), GetBV (clrFinal));
  return (clrFinal);

}

static inline int AdjustChannel1(int nValue, double nPercent)
{
  int nNewValue = (int)(.5 + nPercent * nValue);

  if (nValue == 0 && nPercent > 1.) {
    nNewValue = (int)(.5 + (nPercent - 1.) * 255);
  }

  return min(nNewValue, 255);
}

COLORREF PixelAlpha(COLORREF srcPixel, double percentR, double percentG, double percentB)
{
  COLORREF clrFinal = _RGB(AdjustChannel(GetRV(srcPixel), percentR),
      AdjustChannel(GetGV(srcPixel), percentG),
      AdjustChannel(GetBV(srcPixel), percentB));

  return (clrFinal);

}

inline void SetAlphaPixel(COLORREF* pBits,
    CRect rect, int x, int y,
    int percent, int m_iShadowSize,
    COLORREF clrBase = 0,
    BOOL bIsRight = TRUE)
{
  // Our direct bitmap access swapped the y coordinate...
  y = (rect.Height() + m_iShadowSize) - y;

  COLORREF* pColor = (COLORREF*)(bIsRight ?
      (pBits + (rect.Width() + m_iShadowSize) * (y + 1) - x) :
      (pBits + (rect.Width() + m_iShadowSize) * y + x));

  *pColor = PixelAlpha(*pColor, percent);

  if (clrBase == 0) {
    return;
  }

  *pColor = _RGB(min(255, (3 * GetRV(*pColor) + GetBV(clrBase)) / 4),
      min(255, (3 * GetGV(*pColor) + GetGV(clrBase)) / 4),
      min(255, (3 * GetBV(*pColor) + GetRV(clrBase)) / 4));
}

//----------------------------------------------------------------------
// Conversion between the HSL (Hue, Saturation, and Luminosity)
// and RBG color model.
//----------------------------------------------------------------------
// The conversion algorithms presented here come from the book by
// Fundamentals of Interactive Computer Graphics by Foley and van Dam.
// In the example code, HSL values are represented as floating point
// number in the range 0 to 1. _RGB tridrants use the Windows convention
// of 0 to 255 of each element.
//----------------------------------------------------------------------

double HuetoRGB1(double m1, double m2, double h)
{
  if (h < 0) {
    h += 1.0;
  }

  if (h > 1) {
    h -= 1.0;
  }

  if (6.0 * h < 1) {
    return (m1 + (m2 - m1) * h * 6.0);
  }

  if (2.0 * h < 1) {
    return m2;
  }

  if (3.0 * h < 2.0) {
    return (m1 + (m2 - m1) * ((2.0 / 3.0) - h) * 6.0);
  }

  return m1;
}

BYTE HueToRGB1(float rm1, float rm2, float rh)
{
  if (rh > 360.0f) {
    rh -= 360.0f;
  }
  else if (rh < 0.0f) {
    rh += 360.0f;
  }

  if (rh <  60.0f) {
    rm1 = rm1 + (rm2 - rm1) * rh / 60.0f;
  }
  else if (rh < 180.0f) {
    rm1 = rm2;
  }
  else if (rh < 240.0f) {
    rm1 = rm1 + (rm2 - rm1) * (240.0f - rh) / 60.0f;
  }

  return static_cast<BYTE>(rm1 * 255);
}

COLORREF HLStoRGB_ONE1(double H, double L, double S)
{
  double r, g, b;
  double m1, m2;

  if (S == 0) {
    r = g = b = L;
  }
  else {
    if (L <= 0.5) {
      m2 = L * (1.0 + S);
    }
    else {
      m2 = L + S - L * S;
    }

    m1 = 2.0 * L - m2;
    r = HuetoRGB(m1, m2, H + 1.0 / 3.0);
    g = HuetoRGB(m1, m2, H);
    b = HuetoRGB(m1, m2, H - 1.0 / 3.0);
  }

  return _RGB((BYTE)(r * 255), (BYTE)(g * 255), (BYTE)(b * 255));
}

COLORREF HLStoRGB_TWO1(double H, double L, double S)
{
  WORD R, G, B; // _RGB component values

  if (S == 0.0) {
    R = G = B = unsigned char(L * 255.0);
  }
  else {
    float rm1, rm2;

    if (L <= 0.5f) {
      rm2 = (float)(L + L * S);
    }
    else {
      rm2 = (float)(L + S - L * S);
    }

    rm1 = (float)(2.0f * L - rm2);

    R = HueToRGB(rm1, rm2, (float)(H + 120.0f));
    G = HueToRGB(rm1, rm2, (float)(H));
    B = HueToRGB(rm1, rm2, (float)(H - 120.0f));
  }

  return _RGB(R, G, B);
}

void RGBtoHSL(COLORREF rgb, double* H, double* S, double* L)
{
  double delta;
  double r = (double)GetRV(rgb) / 255;
  double g = (double)GetGV(rgb) / 255;
  double b = (double)GetBV(rgb) / 255;
  double cmax = max(r, max(g, b));
  double cmin = min(r, min(g, b));
  *L = (cmax + cmin) / 2.0;

  if (cmax == cmin) {
    *S = 0;
    *H = 0; // it's really undefined
  }
  else {
    if (*L < 0.5) {
      *S = (cmax - cmin) / (cmax + cmin);
    }
    else {
      *S = (cmax - cmin) / (2.0 - cmax - cmin);
    }

    delta = cmax - cmin;

    if (r == cmax) {
      *H = (g - b) / delta;
    }
    else if (g == cmax) {
      *H = 2.0 + (b - r) / delta;
    }
    else {
      *H = 4.0 + (r - g) / delta;
    }

    *H /= 6.0;

    if (*H < 0.0) {
      *H += 1;
    }
  }
}

void RGBtoHSV(COLORREF rgb, double* H, double* S, double* V)
// Algoritm by A. R. Smith
{
  double r = (double) GetRV(rgb) / 255;
  double g = (double) GetGV(rgb) / 255;
  double b = (double) GetBV(rgb) / 255;

  double dblMin = min(r, min(g, b));
  double dblMax = max(r, max(g, b));

  *V = dblMax;        // v
  double delta = dblMax - dblMin;

  if (dblMax != 0) {
    *S = delta / dblMax;  // s
  }
  else {
    // r = g = b = 0    // s = 0, v is undefined
    *S = 0;
    *H = -1;
    return;
  }

  if (delta == 0.) {
    *H = 1;
  }
  else {
    if (r == dblMax) {
      *H = (g - b) / delta;  // between yellow & magenta
    }
    else if (g == dblMax) {
      *H = 2 + (b - r) / delta;  // between cyan & yellow
    }
    else {
      *H = 4 + (r - g) / delta;  // between magenta & cyan
    }
  }

  *H *= 60;       // degrees

  if (*H < 0) {
    *H += 360;
  }
}

// Algoritm by A. R. Smith
COLORREF HSVtoRGB1(double h, double s, double v)
{
  int i;
  double f, p, q, t;
  double r, g, b;

  if (s == 0) {
    // achromatic (grey)
    r = g = b = v;
  }
  else {
    h /= 60;      // sector 0 to 5
    i = (int) floor(h);
    f = h - i;      // factorial part of h
    p = v * (1 - s);
    q = v * (1 - s * f);
    t = v * (1 - s * (1 - f));

    switch (i) {
    case 0:
      r = v;
      g = t;
      b = p;
      break;

    case 1:
      r = q;
      g = v;
      b = p;
      break;

    case 2:
      r = p;
      g = v;
      b = t;
      break;

    case 3:
      r = p;
      g = q;
      b = v;
      break;

    case 4:
      r = t;
      g = p;
      b = v;
      break;

    default:    // case 5:
      r = v;
      g = p;
      b = q;
      break;
    }
  }

  return _RGB(
      (int)(.5 + r * 255),
      (int)(.5 + g * 255),
      (int)(.5 + b * 255));
}

COLORREF SmartMixColors(COLORREF color1, COLORREF color2,
    double dblLumRatio = 1., int k1 = 1, int k2 = 1)
{
  ASSERT(k1 >= 0);
  ASSERT(k2 >= 0);

  if (k1 + k2 == 0) {
    ASSERT(FALSE);
    return _RGB(0, 0, 0);
  }

  COLORREF color = _RGB(
      (GetRV(color1) * k1 + GetRV(color2) * k2) / (k1 + k2),
      (GetGV(color1) * k1 + GetGV(color2) * k2) / (k1 + k2),
      (GetBV(color1) * k1 + GetBV(color2) * k2) / (k1 + k2));

  double h1, s1, v1;
  RGBtoHSV(color, &h1, &s1, &v1);

  double h2, s2, v2;
  RGBtoHSV(color2, &h2, &s2, &v2);

  v1 = v2;
  s1 = (s1 *  k1 + s2 *  k2) / (k1 + k2);

  color = HSVtoRGB(h1, s1, v1);

  if (dblLumRatio != 1.) {
    double H, S, L;
    RGBtoHSL(color, &H, &S, &L);

    color = HLStoRGB_ONE(H, min(1., L * dblLumRatio), S);
  }

  return color;
}

COLORREF MixColors(COLORREF clr1, COLORREF clr2, double dblRatio)
{
  ASSERT(dblRatio > 0.0f && dblRatio <= 1.0f);

  return _RGB(
      GetRV(clr1) + dblRatio * (GetRV(clr2) - GetRV(clr1)),
      GetGV(clr1) + dblRatio * (GetGV(clr2) - GetGV(clr1)),
      GetBV(clr1) + dblRatio * (GetBV(clr2) - GetBV(clr1))
      );
}

void DrawAlpha(CDC* pDstDC, const CRect& rectDst, CDC* pSrcDC, const CRect& rectSrc, BYTE nOpacity = 255)
{
#if 0
  BLENDFUNCTION pixelblend = { AC_SRC_OVER, 0,
                nOpacity, 1 /*AC_SRC_ALPHA*/
                             };

  imdraw_image_corner()
#ifndef _BCGSUITE_

  if (CBCGPToolBarImages::m_pfAlphaBlend != NULL && !globalData.bIsWindows9x) {
    (*CBCGPToolBarImages::m_pfAlphaBlend)(pDstDC->m_hDC,
        rectDst.l, rectDst.t,
        rectDst.Width(), rectDst.Height(),
        pSrcDC->m_hDC, rectSrc.l, rectSrc.t,
        rectSrc.Width(), rectSrc.Height(),
        pixelblend);
  }
  else {
    if (rectSrc.Width() != rectDst.Width() ||
        rectSrc.Height() != rectDst.Height()) {
      pDstDC->StretchBlt(rectDst.l, rectDst.t,
          rectDst.Width(), rectDst.Height(),
          pSrcDC, rectSrc.l, rectSrc.t,
          rectSrc.Width(), rectSrc.Height(), SRCCOPY);
    }
    else {
      pDstDC->BitBlt(rectDst.l, rectDst.t, rectDst.Width(), rectDst.Height(),
          pSrcDC, rectSrc.l, rectSrc.t, SRCCOPY);
    }
  }

#else
  pDstDC->AlphaBlend(rectDst.l, rectDst.t,
      rectDst.Width(), rectDst.Height(),
      pSrcDC, rectSrc.l, rectSrc.t,
      rectSrc.Width(), rectSrc.Height(),
      pixelblend);
#endif
#endif
}
//****
void FillAlpha(const CRect& rect, BYTE bValue = 255)
{
#if 0
  const int cx = rect.Width();
  const int cy = rect.Height();

  COLORREF* pBits = NULL;
  CBitmap* hmbpDib = CreateBitmap_32(CSize(cx, cy), (LPVOID*)&pBits);

  if (hmbpDib == NULL || pBits == NULL) {
    ASSERT(FALSE);
    return;
  }

  CDC dcMem;
  dcMem.CreateCompatibleDC(&m_dc);

  CBitmap* hbmpOld = (CBitmap*) dcMem.SelectObject(hmbpDib);

  dcMem.BitBlt(0, 0, cx, cy, &m_dc, rect.l, rect.t, SRCCOPY);

  const DWORD dwMask = (bValue << 24) & 0xFF000000;

  for (int i = 0; i < cx * cy; i++) {
    *pBits |= dwMask;
    pBits++;
  }

  m_dc.BitBlt(rect.l, rect.t, cx, cy, &dcMem, 0, 0, SRCCOPY);

  dcMem.SelectObject(hbmpOld);
  DeleteObject(hmbpDib);
#endif
}

//****
void DrawRotated(CRect rect, CDC& dcSrc, BOOL bClockWise)
{
#if 0
  const int cx = rect.Width();
  const int cy = rect.Height();

  if (cx <= 0 || cy <= 0) {
    return;
  }

  for (int y = 0; y <= cy; y++) {
    for (int x = 0; x <= cx; x++) {
      int xSrc = y;
      int ySrc = bClockWise ? (cx - x - 1) : x;

      m_dc.SetPixel(rect.l + x, rect.t + y, dcSrc.GetPixel(xSrc, ySrc));
    }
  }

#endif
}

void CBCGPMenuImages_Draw(CDC* pDC, int id, const CRect& rectImage, int state = ImageBlack, const CSize& sizeImageDest = CSize(0, 0))
{
  COLOR clr = _RGB(0, 0, 0);
#define MENUIMAGES_COLORDEF(a, b)  if (a==state) {clr = b;}
  MENUIMAGES_COLORDEF(ImageBlack, _RGB(0, 0, 0));
  MENUIMAGES_COLORDEF(ImageGray, _RGB(128, 128, 128));
  MENUIMAGES_COLORDEF(ImageDkGray,  _RGB(72, 72, 72));
  MENUIMAGES_COLORDEF(ImageLtGray, _RGB(192, 192, 192));
  MENUIMAGES_COLORDEF(ImageWhite, _RGB(255, 255, 255));
  MENUIMAGES_COLORDEF(ImageBlack2, _RGB(0, 0, 0));

  imdraw_menuimg(pDC->im, &rectImage, rectImage, id, clr);
  return ;
}
void CBCGPMenuImages_Draw(CDC* pDC, int id, const CPoint& ptImage, int state = ImageBlack, const CSize& sizeImage = CSize(0, 0))
{
  IRECT rectImage = iRECT2(ptImage.x, ptImage.y, 9, 9);
  CBCGPMenuImages_Draw(pDC, id, rectImage, state, sizeImage);
  return ;
}
BOOL DrawIconEx(CDC* pDC, int xLeft, int yTop, img_t* hIcon, IN int cxWidth, IN int cyWidth,
    UINT istepIfAniCur, CBrush* hbrFlickerFreeDraw, UINT diFlags)
{
  IRECT rc = iRECT2(xLeft, yTop, cxWidth, cyWidth);
  imdraw_image_corner(pDC->im, &rc, rc, hIcon, 0, 0, 0);
  return TRUE;
}
