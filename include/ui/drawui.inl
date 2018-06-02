#include <math.h>
// »­¶à½ÇÐÇÐÇ ½ÇÊý=n*m
int drawaa_star(HDDC hDC, DPOINT ptc, double* rad, int n, int m, COLORREF clr)
{
  int i, mn = n * m;
  int lens[1];
  //DPOINT pt[10];
  DPOINT* pt = (DPOINT*)malloc(mn * sizeof(DPOINT));
  lens[0] = m * n;
  for (i = 0; i < mn; ++i) {
    double r = rad[i % n], ang = i * PI * 2 / mn;
    pt[i] = dPOINT(ptc.x + r * sin(ang), ptc.y + r * cos(ang));
  }
  drawaa_fillpoly_solid(hDC, pt, lens, 1, clr);
  free(pt);
  return 0;
}
int MirrorRect(HDDC hDC, RECT rect, BOOL bHorz DEFULT(TRUE))
{
  int x, y, h, w, al = hDC->bw >> 2;
  RECT rc = rect;
  COLORREF t;
  COLORREF* ptr;
  ClipRect(hDC, &rc);
  h = RCH(&rc), w = RCW(&rc);
  ptr = &_Pixel(hDC, rc.left, rc.top);
  if (bHorz) {
    for (y = 0; y < h; ++y, ptr += al) {
      for (x = 0; x < w / 2; ++x) {
        CV_SWAP(ptr[x], ptr[w - 1 - x], t);
      }
    }
  }
  else {
    COLORREF* ptr2;
    ptr2 = &_Pixel(hDC, rc.left, rc.bottom - 1);
    for (y = 0; y < h / 2; ++y, ptr += al, ptr2 -= al) {
      for (x = 0; x < w; ++x) {
        CV_SWAP(ptr[x], ptr2[x], t);
      }
    }
  }
  return 0;
}
#define HighlightRect1(hDC, rectCommand) HighlightRect(hDC, rectCommand, -1, 0, 0, 0)
BOOL HighlightRect(HDDC hDC, RECT rect, int nPercentage DEFULT(-1), COLORREF clrTransparent DEFULT(0),
    int nTolerance DEFULT(0), COLORREF clrBlend DEFULT(0))
{
  int x, y;
  RECT rc = rect;
  ClipRect(hDC, &rc);
  if (nPercentage == 100) {
    // Nothing to do
    return TRUE;
  }
  if (clrBlend != 0 && nPercentage > 100) {
    return FALSE;
  }
  ClipRect(hDC, &rect);
  for (y = rc.top; y < rc.bottom; ++y) {
    for (x = rc.left; x < rc.right; ++x) {
      COLORREF* pBits = &_Pixel(hDC, x, y);
      COLORREF color = (COLORREF) * pBits;
      BOOL bIgnore = FALSE;
      if (nTolerance > 0) {
        bIgnore = (abs(GetRV(color) - GetRV(clrTransparent)) < nTolerance &&
            abs(GetGV(color) - GetGV(clrTransparent)) < nTolerance &&
            abs(GetBV(color) - GetBV(clrTransparent)) < nTolerance);
      }
      else {
        bIgnore = color == clrTransparent;
      }
      if (!bIgnore) {
        if (nPercentage == -1) {
          *pBits = RGB_TO_RGBA(
              MIN(255, (2 * GetRV(color) + 255) / 3),
              MIN(255, (2 * GetGV(color) + 255) / 3),
              MIN(255, (2 * GetBV(color) + 255) / 3));
        }
        else {
          if (clrBlend == 0) {
            *pBits = PixelAlpha(color, (.01 * nPercentage), (.01 * nPercentage), (.01 * nPercentage));
          }
          else {
            long R = GetRV(color);
            long G = GetGV(color);
            long B = GetBV(color);
            *pBits = RGB_TO_RGBA(
                MIN(255, R + MulDiv(GetRV(clrBlend) - R, nPercentage, 100)),
                MIN(255, G + MulDiv(GetGV(clrBlend) - G, nPercentage, 100)),
                MIN(255, B + MulDiv(GetBV(clrBlend) - B, nPercentage, 100))
                );
          }
        }
      }
    }
  }
  return TRUE;
}
BOOL GrayRect(HDDC hDC, RECT rect, int nPercentage, COLORREF clrTransparent, COLORREF clrDisabled)
{
  int x, y;
  RECT rc = rect;
  ClipRect(hDC, &rc);
  for (y = rc.top; y < rc.bottom; ++y) {
    for (x = rc.left; x < rc.right; ++x) {
      COLORREF* pBits = &_Pixel(hDC, x, y);
      COLORREF color = (COLORREF) * pBits;
      if (color != clrTransparent) {
        double H, S, L;
        RGBtoHSL(color, &H, &S, &L);
        color = HLStoRGB_ONE(H, L, 0);
        if (nPercentage == -1) {
          *pBits = RGB_TO_RGBA(
              MIN(255, GetRV(color) + ((GetRV(clrDisabled) - GetRV(color)) / 2)),
              MIN(255, GetGV(color) + ((GetGV(clrDisabled) - GetGV(color)) / 2)),
              MIN(255, GetBV(color) + ((GetBV(clrDisabled) - GetBV(color)) / 2)));
        }
        else {
          *pBits = CLR_TO_RGBA(PixelAlpha(color, .01 * nPercentage, .01 * nPercentage, .01 * nPercentage));
        }
      }
    }
  }
  return TRUE;
}
BOOL DrawGradientRing(HDDC hDC, RECT rect, COLORREF colorStart, COLORREF colorFinish,
    COLORREF colorBorder, int nAngle DEFULT(0 - 360), int nWidth, COLORREF clrFace DEFULT(-1))
{
  int nLevel;
  const int xCenter = (rect.left + rect.right) / 2;
  const int yCenter = (rect.top + rect.bottom) / 2;
  const int nSteps = 360;
  const double fDelta = 2. * PI / nSteps;
  const double fStart = PI * nAngle / 180;
  const double fFinish = fStart + 2. * PI;
  double rDelta = (double)(.5 + GetRV(colorFinish) - GetRV(colorStart)) / nSteps * 2;
  double gDelta = (double)(.5 + GetGV(colorFinish) - GetGV(colorStart)) / nSteps * 2;
  double bDelta = (double)(.5 + GetBV(colorFinish) - GetBV(colorStart)) / nSteps * 2;
  int xPrev = -1;
  int yPrev = -1;
  double fAngle;
  for (nLevel = 0; nLevel < nWidth; nLevel++) {
    int i = 0;
    const int nRadius = MIN(RCW(&rect), RCH(&rect)) / 2;
    const int nRectDelta = RCW(&rect) - RCH(&rect);
    if (clrFace != 0 && nLevel == 0) {
      //---------------
      // Fill interior:
      //---------------
      COLORREF brFill = (clrFace);
      if (nRectDelta == 0) { // Circle
        draw_ellipseR(hDC, &rect, brFill, 1);
      }
      else if (nRectDelta > 0) { // Horizontal
        draw_ellipse(hDC, rect.left, rect.top, rect.left + RCH(&rect), rect.bottom, brFill, 1);
        draw_ellipse(hDC, rect.right - RCH(&rect), rect.top, rect.right, rect.bottom, brFill, 1);
        draw_fillrect(hDC, rect.left + RCH(&rect) / 2, rect.top, rect.right - RCH(&rect) / 2, rect.bottom, brFill);
      }
      else { // Vertical
        draw_ellipse(hDC, rect.left, rect.top, rect.right, rect.top + RCW(&rect), brFill, 1);
        draw_ellipse(hDC, rect.left, rect.bottom - RCW(&rect), rect.right, rect.bottom, brFill, 1);
        draw_fillrect(hDC, rect.left, rect.top + RCW(&rect) / 2, rect.right, rect.bottom - RCW(&rect) / 2, brFill);
      }
    }
    for (fAngle = fStart; fAngle < fFinish + fDelta; fAngle += fDelta, i ++) {
      const int nStep = fAngle <= (fFinish + fStart) / 2 ? i : nSteps - i;
      const BYTE bR = (BYTE) MAX(0, MIN(255, (.5 + rDelta * nStep + GetRV(colorStart))));
      const BYTE bG = (BYTE) MAX(0, MIN(255, (.5 + gDelta * nStep + GetGV(colorStart))));
      const BYTE bB = (BYTE) MAX(0, MIN(255, (.5 + bDelta * nStep + GetBV(colorStart))));
      COLORREF color = nLevel == 0 && colorBorder != -1 ?
          colorBorder : _RGB(bR, bG, bB);
      double dx = /*(fAngle >= 0 && fAngle <= PI / 2) || (fAngle >= 3 * PI / 2) ?
 .5 : -.5*/0;
      double dy = /*(fAngle <= PI) ? .5 : -.5*/0;
      int x = xCenter + (int)(dx + cos(fAngle) * nRadius);
      int y = yCenter + (int)(dy + sin(fAngle) * nRadius);
      int x1, y1;
      if (nRectDelta > 0) {
        if (x > xCenter) {
          x += (int)(.5 * nRectDelta);
        }
        else {
          x -= (int)(.5 * nRectDelta);
        }
        if (xPrev != -1 && (xPrev > xCenter) != (x > xCenter)) {
          for (x1 = MIN(x, xPrev); x1 < MAX(x, xPrev); x1++) {
            _SetPixel(hDC, x1, y, color);
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
          for (y1 = MIN(y, yPrev); y1 < MAX(y, yPrev); y1++) {
            _SetPixel(hDC, x, y1, color);
          }
        }
      }
      _SetPixel(hDC, x, y, color);
      xPrev = x;
      yPrev = y;
    }
    DeflateRect(&rect, 1, 1);
  }
  return TRUE;
}
BOOL DrawShadow(HDDC hDC, RECT rect, int nDepth, int iMinBrightness DEFULT(100), int iMaxBrightness DEFULT(50), COLORREF clrBase DEFULT(0), BOOL bRightShadow DEFULT(TRUE))
{
  int cx, cy, x, y, x1, y1, c, iShadowOffset, al = (hDC->bw >> 2);
  DWORD* pBits;
  RECT rc = rect;
  BOOL bIsLeft = !bRightShadow;
  NormalizeRect(&rc);
  ClipRect(hDC, &rc);
  pBits = &_Pixel(hDC, rc.left, rc.top);
  cx = RCW(&rc);
  cy = RCH(&rc);
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
  iShadowOffset = (iMaxBrightness - iMinBrightness) / nDepth;
  // Loop for drawing the shadow
  // Actually, this was simpler to implement than I thought
  for (c = 0; c < nDepth; c++) {
    // Draw the shadow from left to bottom
    for (y = cy; y < cy + (nDepth - c); y++) {
      SetAlphaPixel(pBits, al, c + nDepth, y,
          iMaxBrightness - ((nDepth - c) * (iShadowOffset)), nDepth, clrBase, bIsLeft);
    }
    // Draw the shadow from left to right
    for (x = nDepth + (nDepth - c); x < cx + c; x++) {
      SetAlphaPixel(pBits, al, x, cy + c,
          iMaxBrightness - ((c) * (iShadowOffset)), nDepth, clrBase, bIsLeft);
    }
    // Draw the shadow from top to bottom
    for (y1 = nDepth + (nDepth - c); y1 < cy + c + 1; y1++) {
      SetAlphaPixel(pBits, al, cx + c, y1,
          iMaxBrightness - ((c) * (iShadowOffset)),
          nDepth, clrBase, bIsLeft);
    }
    // Draw the shadow from top to left
    for (x1 = cx; x1 < cx + (nDepth - c); x1++) {
      SetAlphaPixel(pBits, al, x1, c + nDepth,
          iMaxBrightness - ((nDepth - c) * (iShadowOffset)),
          nDepth, clrBase, bIsLeft);
    }
  }
  return 0;
}
enum FlatStyle {
  BUTTONSTYLE_3D,
  BUTTONSTYLE_FLAT,
  BUTTONSTYLE_SEMIFLAT,
  BUTTONSTYLE_NOBORDERS
};
#define imsize(im) iSIZE((im)->w, (im)->h)
#define m_szImageBig imsize(s->m_ImageListBig)
#define m_szImageDisabledBig imsize(s->m_ImageListDisabledBig)
int DrawGradient(HDDC hDC, const RECT* pRect, BOOL horz, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
  DoPaintGradient(hDC, *pRect, clrTopLeft, clrBottomRight, !horz);
  return 0;
}
// for custom drawing
typedef struct tagDrawState {
  img_t* hbmMono;
  img_t* hbmMonoOld;
  img_t* hbmOldGlyphs;
}
CBCGPDrawState;
#if 0
typedef struct CBCGPToolBarImages {
  CSize m_sizeImage; // size of glyph
  CSize m_sizeImageOriginal;
  CSize m_sizeImageDest; // destination size glyph
  img_t* m_hbmImageWell; // glyphs only
  img_t* m_hbmImageLight; // "faded" version (hi-color only)
  img_t* m_hbmImageShadow; // "shadow" version
  BOOL m_bUserImagesList; // is user-defined images list?
  CString m_strUDLPath; // user-defined images path
  BOOL m_bModified; // is image modified?
  int m_iCount; // image counter
  CList<UINT, UINT> m_lstOrigResIds; // original resource ids
  CList<HINSTANCE, HINSTANCE> m_lstOrigResInstances; // original resource instances
  CMap<UINT, UINT, int, int> m_mapOrigResOffsets;// original resource offsets
  BOOL m_bStretch; // stretch images
  CDC m_dcMem; // DC for the drawing in memory
  CBitmap m_bmpMem; // bitmap for the drawing in memory
  CBitmap* m_pBmpOriginal;
  COLORREF m_clrTransparent; // Transparent color
  COLORREF m_clrTransparentOriginal;
  BOOL m_bReadOnly; // Loaded from read-only file
  BOOL m_bIsTemporary; // Temporary copy of another image
  COLORREF m_clrImageShadow; // Color of the shadow
  BOOL m_bFadeInactive; // Inactive image will be drawn with the "fading" effect
  int m_nBitsPerPixel; // Bitmap color depth
  static HINSTANCE m_hinstMSIMGDLL; // MSIMG.DLL instance
  static ALPHABLEND m_pfAlphaBlend; // Pointer to AlphaBlend method from MSIMG.DLL
  static TRANSPARENTBLT m_pfTransparentBlt;// Pointer to TransparentBlt method
  static BYTE m_nDisabledImageAlpha; // Used in the alpha-blending only
  static BYTE m_nFadedImageAlpha; // Used in the alpha-blending only
  BOOL m_bIsGray; // Image is grayed
  int m_nGrayPercentage; // Grayed image brightness
  static BOOL m_bIsRTL; // RTL languages support
  int m_nLightPercentage; // Light image percentage value
  BOOL m_bMapTo3DColors; // Map image colors to system
  BOOL m_bAlwaysLight; // Always use light image in all color resolutions
  CRect m_rectLastDraw; // Last drawn image location
  CRect m_rectSubImage; // The part of drawn image
  BOOL m_bAutoCheckPremlt; // Auto-check for 32 bpp images
  BOOL m_bCreateMonoDC; // Create mono DC in CBCGPDrawState
  double m_dblScale;
  static CCriticalSection g_cs; // For multi-theard applications
} CBCGPToolBarImages;
#include "Stdafx.h"
#include <math.h>
#include "BCGGlobals.h"
#include "BCGPToolBarImages.h"
#include "BCGPToolBar.h"
#include "BCGPLocalResource.h"
#include "bcgprores.h"
#include "BCGPVisualManager.h"
#include "BCGPDrawManager.h"
#include "BCGPPngImage.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
static BOOL WriteDIB(LPCTSTR szFile, HANDLE hDIB);
static HANDLE DDBToDIB(img_t* bitmap, DWORD dwCompression);
BOOL CBCGPToolBarImages::m_bDisableTrueColorAlpha = TRUE;
CCriticalSection CBCGPToolBarImages::g_cs;
BOOL CBCGPToolBarImages::m_bMultiThreaded = FALSE;
BOOL CBCGPToolBarImages::m_bIsDrawOnGlass = FALSE;
HINSTANCE CBCGPToolBarImages::m_hinstMSIMGDLL = NULL;
ALPHABLEND CBCGPToolBarImages::m_pfAlphaBlend = NULL;
TRANSPARENTBLT CBCGPToolBarImages::m_pfTransparentBlt = NULL;
BYTE CBCGPToolBarImages::m_nDisabledImageAlpha = 127;
BYTE CBCGPToolBarImages::m_nFadedImageAlpha = 150;
BOOL CBCGPToolBarImages::m_bIsRTL = FALSE;
double CBCGPToolBarImages::m_dblColorTolerance = 0.2;
// globals for fast drawing (shared globals)
static HDC hDCGlyphs = NULL;
static HDC hDCMono = NULL;
/*
 DIBs use RGBQUAD format:
 0xbb 0xgg 0xrr 0x00
 Reasonably efficient code to convert a COLORREF into an
 RGBQUAD is byte-order-dependent, so we need different
 code depending on the byte order we're targeting.
*/
#define RGB_TO_RGBQUAD(r,g,b) (_RGB(b,g,r))
#define CLR_TO_RGBQUAD(clr) (_RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr)))
#define RGBQUAD_TO_CLR(clr) (_RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr)))
// Raster Ops
#define ROP_DSPDxax 0x00E20746L
#define ROP_PSDPxax 0x00B8074AL
// Internal images:
#define BCGImage_Light 0
#define BCGImage_Shadow 1
static double clamp(double value, double low, double high)
{
  return value < low ? low : (value > high ? high : value);
}
static double clamp_to_byte(double value)
{
  return clamp(value, 0.0, 255.0);
}
struct CBCGPZoomKernel {
public:
  typedef double XFilterProc(double dValue);
  typedef XFilterProc* XLPFilterProc;
  struct XKernel {
    long pixel;
    double weight;
  };
  struct XKernelList {
    DWORD count;
    XKernel* stat;
  };
  enum XZoomType {
    e_ZoomTypeFirst = 0,
    e_ZoomTypeStretch = e_ZoomTypeFirst,
    e_ZoomTypeFitImage = 1,
    e_ZoomTypeFitWidth = 2,
    e_ZoomTypeFitHeight = 3,
    e_ZoomTypeLast = e_ZoomTypeFitHeight
  };
  enum XFilterType {
    e_FilterTypeFirst = 0,
    e_FilterTypeBox = e_FilterTypeFirst,
    e_FilterTypeBilinear = 1,
    e_FilterTypeBicubic = 2,
    e_FilterTypeBell = 3,
    e_FilterTypeBSpline = 4,
    e_FilterTypeLanczos3 = 5,
    e_FilterTypeMitchell = 6,
    e_FilterTypeLast = e_FilterTypeMitchell
  };
public:
  CBCGPZoomKernel();
  virtual ~CBCGPZoomKernel();
  void Create
  (
      long sizeSrc,
      long sizeDst,
      long originSrc,
      long widthSrc,
      XFilterType ft
  );
  void Create
  (
      long sizeSrc,
      long sizeDst,
      XFilterType ft
  );
  void Empty();
  XKernelList&
  operator [](long index);
  const XKernelList&
  operator [](long index) const;
  static double FilterWidth(XFilterType ft);
  static XLPFilterProc FilterProc(XFilterType ft);
  static double Filter(XFilterType ft, double value);
  static void CorrectZoomSize(const CSize& sizeSrc, CSize& sizeDst,
      XZoomType zt);
private:
  DWORD m_Size;
  XKernelList*
  m_List;
};
void CBCGPZoomKernel::CorrectZoomSize(const CSize& sizeSrc, CSize& sizeDst, XZoomType zt)
{
  double ZoomX = (double)sizeDst.w / (double)sizeSrc.w;
  double ZoomY = (double)sizeDst.h / (double)sizeSrc.h;
  if (zt != e_ZoomTypeStretch) {
    switch (zt) {
    case e_ZoomTypeFitWidth:
      ZoomY = ZoomX;
      break;
    case e_ZoomTypeFitHeight:
      ZoomX = ZoomY;
      break;
    case e_ZoomTypeFitImage:
      ZoomX = min(ZoomX, ZoomY);
      ZoomY = ZoomX;
      break;
    }
    sizeDst.w = (long)(sizeSrc.w * ZoomX);
    sizeDst.h = (long)(sizeSrc.h * ZoomY);
  }
}
double SinC(double dValue)
{
  if (dValue != 0.0) {
    dValue *= M_PI;
    return sin(dValue) / dValue;
  }
  return 1.0;
}
double
Filter_Box(double dValue)
{
  if ((dValue > -0.5) && (dValue <= 0.5)) {
    return 1.0;
  }
  return 0.0;
}
// Bilinear filter
// a.k.a. "Linear" or "Triangle" filter
double
Filter_Bilinear(double dValue)
{
  if (dValue < 0.0) {
    dValue = -dValue;
  }
  if (dValue < 1.0) {
    return 1.0 - dValue;
  }
  return 0.0;
}
double
Filter_Bicubic(double dValue)
{
  // f(t) = 2|t|^3 - 3|t|^2 + 1, -1 <= t <= 1
  if (dValue < 0.0) {
    dValue = -dValue;
  }
  if (dValue < 1.0) {
    return (2.0 * dValue - 3.0) * dValue * dValue + 1.0;
  }
  return 0.0;
}
// Bell filter
double
Filter_Bell(double dValue)
{
  if (dValue < 0.0) {
    dValue = -dValue;
  }
  if (dValue < 0.5) {
    return 0.75 - dValue * dValue;
  }
  else {
    if (dValue < 1.5) {
      dValue = dValue - 1.5;
      return 0.5 * dValue * dValue;
    }
  }
  return 0.0;
}
double
Filter_BSpline(double dValue)
{
  if (dValue < 0.0) {
    dValue = -dValue;
  }
  if (dValue < 1.0) {
    double tt = dValue * dValue;
    return 0.5 * tt * dValue - tt + 2.0 / 3.0;
  }
  else {
    if (dValue < 2.0) {
      dValue = 2.0 - dValue;
      return dValue * dValue * dValue / 6.0;
    }
  }
  return 0.0;
}
double
Filter_Lanczos3(double dValue)
{
  if (dValue < 0.0) {
    dValue = -dValue;
  }
  if (dValue < 3.0) {
    return SinC(dValue) * SinC(dValue / 3.0);
  }
  return 0.0;
}
double
Filter_Mitchell(double dValue)
{
  static double B = 1.0 / 3.0;
  static double C = B;
  if (dValue < 0.0) {
    dValue = -dValue;
  }
  const double tt = dValue * dValue;
  if (dValue < 1.0) {
    return ((12.0 - 9.0 * B - 6.0 * C) * (dValue * tt) +
        (-18.0 + 12.0 * B + 6.0 * C) * tt +
        (6.0 - 2.0 * B)) / 6.0;
  }
  else {
    if (dValue < 2.0) {
      return ((-1.0 * B - 6.0 * C) * (dValue * tt) +
          (6.0 * B + 30.0 * C) * tt +
          (-12.0 * B - 48.0 * C) * dValue +
          (8.0 * B + 24.0 * C)) / 6.0;
    }
  }
  return 0.0;
}
struct CBCGPImageResizeFilter {
  CBCGPZoomKernel::XLPFilterProc Proc;
  double Width;
};
static CBCGPImageResizeFilter Filters[7] = {
  { &Filter_Box , 0.5},
  { &Filter_Bilinear, 1.0},
  { &Filter_Bicubic , 1.0},
  { &Filter_Bell , 1.5},
  { &Filter_BSpline , 2.0},
  { &Filter_Lanczos3, 3.0},
  { &Filter_Mitchell, 2.0}
};
CBCGPZoomKernel::XLPFilterProc
CBCGPZoomKernel::FilterProc(CBCGPZoomKernel::XFilterType ft)
{
  return Filters[ft].Proc;
}
double CBCGPZoomKernel::FilterWidth(CBCGPZoomKernel::XFilterType ft)
{
  return Filters[ft].Width;
}
double CBCGPZoomKernel::Filter(CBCGPZoomKernel::XFilterType ft, double value)
{
  return Filters[ft].Proc(value);
}
CBCGPZoomKernel::CBCGPZoomKernel()
  : m_Size(0)
  , m_List(NULL)
{
}
CBCGPZoomKernel::~CBCGPZoomKernel()
{
  Empty();
}
void CBCGPZoomKernel::Create(long sizeSrc, long sizeDst, long originSrc, long widthSrc, XFilterType ft)
{
  if (sizeSrc <= 0) {
    ASSERT(FALSE);
    return;
  }
  if (sizeDst <= 0) {
    ASSERT(FALSE);
    return;
  }
  Empty();
  m_Size = sizeDst;
  const double dScale = (double)(m_Size) / (double)(sizeSrc);
  const XLPFilterProc lpFilterProc = Filters[ft].Proc;
  const double dFilterWidth = Filters[ft].Width;
  m_List = new XKernelList[m_Size];
  double width = dFilterWidth;
  double scale = 1.0;
  double correction = -0.25;
  if (dScale < 1.0) {
    width /= dScale;
    scale = dScale;
    correction = -correction;
  }
  for (DWORD i = 0; i < m_Size; i++) {
    double center = i / dScale;
    long left = (long)floor(center - width);
    long right = (long)ceil(center + width);
    const long c_Count = right - left + 1;
    m_List[i].count = 0;
    if (c_Count == 0) {
      continue;
    }
    m_List[i].stat = new XKernel[c_Count];
    BOOL bCross = FALSE;
    DWORD index = 0;
    double weightSum = 0.0;
    XKernel* pStat = m_List[i].stat;
    BOOL bFirst = TRUE;
    for (long j = left; j <= right; j++) {
      double weight = lpFilterProc((center - (double)j + correction) * scale) * scale;
      if (weight == 0.0) {
        if (!bFirst) {
          break;
        }
        continue;
      }
      bFirst = FALSE;
      long pixel = j + originSrc;
      if (pixel < 0) {
        pixel = -pixel;
        bCross = TRUE;
      }
      else if (pixel >= widthSrc) {
        pixel = 2 * widthSrc - pixel - 1;
        bCross = TRUE;
      }
      BOOL bFound = FALSE;
      if (bCross) {
        for (DWORD k = 0; k < index; k++) {
          if (pStat[k].pixel == pixel) {
            pStat[k].weight += weight;
            bFound = TRUE;
            break;
          }
        }
      }
      if (!bFound) {
        pStat[index].pixel = pixel;
        pStat[index].weight = weight;
        index++;
        m_List[i].count = index;
      }
      weightSum += weight;
    }
    if (weightSum != 0.0) {
      for (DWORD j = 0; j <= m_List[i].count; j++) {
        m_List[i].stat[j].weight /= weightSum;
      }
    }
  }
}
void CBCGPZoomKernel::Create(long sizeSrc, long sizeDst, XFilterType ft)
{
  Create(sizeSrc, sizeDst, 0, sizeSrc, ft);
}
void CBCGPZoomKernel::Empty()
{
  if (m_List != NULL) {
    for (DWORD i = 0; i < m_Size; i++) {
      if (m_List[i].count > 0) {
        delete [] m_List[i].stat;
      }
    }
    delete [] m_List;
    m_List = NULL;
    m_Size = 0;
  }
}
CBCGPZoomKernel::XKernelList&
CBCGPZoomKernel::operator [](long index)
{
  return m_List[index];
}
const CBCGPZoomKernel::XKernelList&
CBCGPZoomKernel::operator [](long index)
{
  return m_List[index];
}
struct CBCGPScanliner {
public:
  CBCGPScanliner() {
    empty();
  }
  CBCGPScanliner
  (
      LPBYTE data,
      const CSize& size,
      size_t height = 0,
      size_t pitch = 0,
      BYTE cn = 4,
      BOOL invert = FALSE
  ) {
    attach(data, size, height, pitch, cn, invert);
  }
  CBCGPScanliner
  (
      LPBYTE data,
      const CRect& rect,
      size_t height = 0,
      size_t pitch = 0,
      BYTE cn = 4,
      BOOL invert = FALSE
  ) {
    attach(data, rect, height, pitch, cn, invert);
  }
  void attach
  (
      LPBYTE data,
      const CSize& size,
      size_t height = 0,
      size_t pitch = 0,
      BYTE cn = 4,
      BOOL invert = FALSE
  ) {
    attach(data, CRect(CPoint(0, 0), size), height, pitch, cn, invert);
  }
  void attach
  (
      LPBYTE data,
      const CRect& rect,
      size_t height = 0,
      size_t pitch = 0,
      BYTE cn = 4,
      BOOL invert = FALSE
  ) {
    empty();
    ASSERT(data != NULL);
    CPoint point(rect.TopLeft());
    CSize size(rect.Size());
    if (pitch == 0) {
      pitch = size.w;
    }
    if (height == 0) {
      height = point.y + size.h;
    }
    ASSERT((size_t)size.w <= pitch);
    ASSERT((size_t)(point.y + size.h) <= height);
    m_rows = size.h;
    m_cols = size.w * cn;
    m_pitch = (DWORD)pitch;
    m_offset = (long)m_pitch;
    if (invert) {
      m_offset = -m_offset;
    }
    m_channels = cn;
    m_height = (DWORD)height;
    m_start_row = point.y;
    m_start_col = point.x;
    m_line_begin = _begin(data);
    m_line_end = _end(data);
    m_line = m_line_begin;
  }
  LPBYTE begin() {
    m_line = m_line_begin;
    return m_line;
  }
  LPBYTE end() {
    m_line = m_line_end;
    return m_line;
  }
  LPBYTE operator[](long index) {
    return m_line_begin + m_offset * index;
  }
  const LPBYTE operator[](long index) {
    return m_line_begin + m_offset * index;
  }
  LPBYTE get() {
    return m_line;
  }
  const LPBYTE get() {
    return m_line;
  }
  DWORD pitch() {
    return m_pitch;
  }
  DWORD rows() {
    return m_rows;
  }
  DWORD cols() {
    return m_cols;
  }
  const CBCGPScanliner&
  operator += (DWORD line) {
    m_line += m_offset * line;
    return *this;
  }
  const CBCGPScanliner&
  operator -= (DWORD line) {
    m_line -= m_offset * line;
    return *this;
  }
  const CBCGPScanliner&
  operator ++ () {
    m_line += m_offset;
    return *this;
  }
  const CBCGPScanliner&
  operator ++ (int) {
    m_line += m_offset;
    return *this;
  }
  const CBCGPScanliner&
  operator -- () {
    m_line -= m_offset;
    return *this;
  }
  const CBCGPScanliner&
  operator -- (int) {
    m_line += m_offset;
    return *this;
  }
protected:
  void empty() {
    m_line = NULL;
    m_pitch = 0;
    m_start_row = 0;
    m_start_col = 0;
    m_rows = 0;
    m_cols = 0;
    m_offset = 0;
    m_height = 0;
    m_line_begin = NULL;
    m_line_end = NULL;
  }
  LPBYTE _begin(LPBYTE data) {
    LPBYTE line = data;
    if (m_offset > 0) {
      line += m_start_row * m_pitch;
    }
    else {
      line += (m_height - m_start_row - 1) * m_pitch;
    }
    if (m_start_col != 0) {
      line += m_start_col * m_channels;
    }
    return line;
  }
  LPBYTE _end(LPBYTE data) {
    LPBYTE line = data;
    if (m_offset > 0) {
      line += (m_start_row + m_rows - 1) * m_pitch;
    }
    else {
      line += (m_height - m_start_row - m_rows) * m_pitch;
    }
    if (m_start_col != 0) {
      line += m_start_col * m_channels;
    }
    return line;
  }
private:
  LPBYTE m_line;
  LPBYTE m_line_begin;
  LPBYTE m_line_end;
  DWORD m_pitch;
  DWORD m_start_row;
  DWORD m_start_col;
  DWORD m_rows;
  DWORD m_cols;
  long m_offset;
  BYTE m_channels;
  DWORD m_height;
};
struct CBCGPScanlinerBitmap: public CBCGPScanliner {
public:
  CBCGPScanlinerBitmap() {
    empty();
  }
  void attach(img_t* bitmap, const CPoint& ptBegin = CPoint(0, 0)) {
    if (bitmap == NULL) {
      ASSERT(FALSE);
      return;
    }
    BITMAP bmp;
    if (::GetObject(bitmap, sizeof(BITMAP), &bmp) == 0 ||
        bmp.bmBits == 0 || bmp.bmBitsPixel < 24) {
      ASSERT(FALSE);
      return;
    }
    CSize size(bmp.bmWidth, abs(bmp.bmHeight));
    CRect rect(CPoint(0, 0), size);
    rect.IntersectRect(CRect(ptBegin, size), rect);
    int cn = bmp.bmBitsPixel / 8;
    int pitch = cn * size.w;
    if (pitch % 4) {
      pitch += 4 - (pitch % 4);
    }
    CBCGPScanliner::attach((LPBYTE) bmp.bmBits,
        size, size.h, pitch, (BYTE) cn, FALSE);
  }
};
/////////////////////////////////////////////////////////////////////////////
// Init / Term
void CBCGPToolBarImages::CleanUp()
{
  if (hDCMono != NULL) {
    ::DeleteDC(hDCMono);
    hDCMono = NULL;
  }
  if (hDCGlyphs != NULL) {
    ::DeleteDC(hDCGlyphs);
    hDCGlyphs = NULL;
  }
  if (m_hinstMSIMGDLL != NULL) {
    ::FreeLibrary(m_hinstMSIMGDLL);
    m_hinstMSIMGDLL = NULL;
  }
  m_pfAlphaBlend = NULL;
  m_pfTransparentBlt = NULL;
}
// a special struct that will cleanup automatically
struct _AFX_TOOLBAR_TERM {
  ~_AFX_TOOLBAR_TERM() {
    CBCGPToolBarImages::CleanUp();
  }
};
static const _AFX_TOOLBAR_TERM toolbarTerm;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBCGPToolBarImages::CBCGPToolBarImages()
{
  m_bModified = FALSE;
  m_bReadOnly = FALSE;
  m_bIsTemporary = FALSE;
  m_iCount = 0;
  m_bIsGray = FALSE;
  m_nGrayPercentage = 0;
  m_hbmImageWell = NULL;
  m_hbmImageLight = NULL;
  m_hbmImageShadow = NULL;
  m_bUserImagesList = FALSE;
  // initialize the toolbar drawing engine
  static BOOL bInitialized;
  if (!bInitialized) {
    hDCGlyphs = CreateCompatibleDC(NULL);
    // Mono DC and Bitmap for disabled image
    hDCMono = ::CreateCompatibleDC(NULL);
    if (hDCGlyphs == NULL || hDCMono == NULL) {
      AfxThrowResourceException();
    }
    OSVERSIONINFO osvi;
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    ::GetVersionEx(&osvi);
    BOOL bIsWindowsNT4 = ((osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) &&
        (osvi.dwMajorVersion < 5));
    BOOL bIsOSAlphaBlendingSupport = (osvi.dwMajorVersion > 4) ||
        ((osvi.dwMajorVersion == 4) && (osvi.dwMinorVersion > 0));
    if (bIsWindowsNT4) {
      bIsOSAlphaBlendingSupport = FALSE;
    }
    if (bIsOSAlphaBlendingSupport) {
      if ((m_hinstMSIMGDLL = LoadLibrary(_T("msimg32.dll"))) != NULL) {
        m_pfAlphaBlend =
            (ALPHABLEND)::GetProcAddress(m_hinstMSIMGDLL, "AlphaBlend");
        if (!globalData.bIsWindows9x) {
          m_pfTransparentBlt =
              (TRANSPARENTBLT)::GetProcAddress(m_hinstMSIMGDLL, "TransparentBlt");
        }
      }
    }
    bInitialized = TRUE;
  }
  m_clrTransparentOriginal = m_clrTransparent = (COLORREF) - 1;
  // UISG standard sizes
  m_sizeImage = CSize(16, 15);
  m_sizeImageOriginal = CSize(0, 0);
  m_sizeImageDest = CSize(0, 0);
  m_rectLastDraw = CRect(0, 0, 0, 0);
  m_rectSubImage = CRect(0, 0, 0, 0);
  m_bStretch = FALSE;
  m_pBmpOriginal = NULL;
  m_bFadeInactive = FALSE;
  m_nBitsPerPixel = 0;
  m_nLightPercentage = 130;
  m_bAlwaysLight = FALSE;
  m_bMapTo3DColors = TRUE;
  m_bAutoCheckPremlt = FALSE;
  m_bCreateMonoDC = TRUE;
  m_dblScale = 1.0;
  OnSysColorChange();
}
CBCGPToolBarImages::~CBCGPToolBarImages()
{
  ASSERT(m_dcMem.GetSafeHdc() == NULL);
  ASSERT(m_bmpMem.GetSafeHandle() == NULL);
  ASSERT(m_pBmpOriginal == NULL);
  if (!m_bIsTemporary) {
    AfxDeleteObject((HGDIOBJ*)&m_hbmImageWell);
    AfxDeleteObject((HGDIOBJ*)&m_hbmImageLight);
    AfxDeleteObject((HGDIOBJ*)&m_hbmImageShadow);
  }
}
void CBCGPToolBarImages::UpdateCount()
{
  if (m_hbmImageWell == NULL) {
    m_iCount = 0;
    return;
  }
  BITMAP bmp;
  if (::GetObject(m_hbmImageWell, sizeof(BITMAP), &bmp) == 0) {
    m_iCount = 0;
    return;
  }
  int iWidth = bmp.bmWidth;
  m_iCount = iWidth / m_sizeImage.w;
}
COLORREF CBCGPToolBarImages::MapToSysColor(COLORREF color, BOOL bUseRGBQUAD)
{
  struct COLORMAP {
    // use DWORD instead of RGBQUAD so we can compare two RGBQUADs easily
    DWORD rgbqFrom;
    int iSysColorTo;
  };
  static const COLORMAP sysColorMap[] = {
    // mapping from color in DIB to system color
    { RGB_TO_RGBQUAD(0x00, 0x00, 0x00), COLOR_BTNTEXT }, // black
    { RGB_TO_RGBQUAD(0x80, 0x80, 0x80), COLOR_BTNSHADOW }, // dark grey
    { RGB_TO_RGBQUAD(0xC0, 0xC0, 0xC0), COLOR_BTNFACE }, // bright grey
    { RGB_TO_RGBQUAD(0xFF, 0xFF, 0xFF), COLOR_BTNHIGHLIGHT } // white
  };
  const int nMaps = 4;
  // look for matching RGBQUAD color in original
  for (int i = 0; i < nMaps; i++) {
    if (color == sysColorMap[i].rgbqFrom) {
      return bUseRGBQUAD ?
          CLR_TO_RGBQUAD(globalData.GetColor(sysColorMap[i].iSysColorTo)) :
          globalData.GetColor(sysColorMap[i].iSysColorTo);
    }
  }
  return color;
}
COLORREF CBCGPToolBarImages::MapToSysColorAlpha(COLORREF color)
{
  BYTE r = GetRValue(color);
  BYTE g = GetGValue(color);
  BYTE b = GetBValue(color);
  const int nDelta = 10;
  if (abs(r - b) > nDelta || abs(r - g) > nDelta || abs(b - g) > nDelta) {
    return color;
  }
  return CBCGPDrawManager::PixelAlpha(globalData.clrBarFace,
      1. + ((double) r - 192) / 255,
      1. + ((double) g - 192) / 255,
      1. + ((double) b - 192) / 255);
}
COLORREF CBCGPToolBarImages::MapFromSysColor(COLORREF color, BOOL bUseRGBQUAD)
{
  struct COLORMAP {
    // use DWORD instead of RGBQUAD so we can compare two RGBQUADs easily
    DWORD rgbTo;
    int iSysColorFrom;
  };
  static const COLORMAP sysColorMap[] = {
    // mapping from color in DIB to system color
    { _RGB(0x00, 0x00, 0x00), COLOR_BTNTEXT }, // black
    { _RGB(0x80, 0x80, 0x80), COLOR_BTNSHADOW }, // dark grey
    { _RGB(0xC0, 0xC0, 0xC0), COLOR_BTNFACE }, // bright grey
    { _RGB(0xFF, 0xFF, 0xFF), COLOR_BTNHIGHLIGHT } // white
  };
  const int nMaps = 4;
  // look for matching RGBQUAD color in original
  for (int i = 0; i < nMaps; i++) {
    COLORREF clrSystem = globalData.GetColor(sysColorMap[i].iSysColorFrom);
    if (bUseRGBQUAD) {
      if (color == CLR_TO_RGBQUAD(clrSystem)) {
        return CLR_TO_RGBQUAD(sysColorMap[i].rgbTo);
      }
    }
    else {
      if (color == clrSystem) {
        return sysColorMap[i].rgbTo;
      }
    }
  }
  return color;
}
BOOL CBCGPToolBarImages::CopyImageToClipboard(int iImage)
{
  CBCGPLocalResource locaRes;
  try {
    CWindowDC dc(NULL);
    // Create a bitmap copy:
    CDC memDCDest;
    memDCDest.CreateCompatibleDC(NULL);
    CBitmap bitmapCopy;
    if (!bitmapCopy.CreateCompatibleBitmap(&dc, m_sizeImage.w, m_sizeImage.h)) {
      AfxMessageBox(IDP_BCGBARRES_CANT_COPY_BITMAP);
      return FALSE;
    }
    CBitmap* pOldBitmapDest = memDCDest.SelectObject(&bitmapCopy);
    memDCDest.FillRect(CRect(0, 0, m_sizeImage.w, m_sizeImage.h),
        &globalData.clrBtnFace);
    CBCGPDrawState ds;
    PrepareDrawImage(ds, FALSE);
    Draw(&memDCDest, 0, 0, iImage);
    EndDrawImage(ds);
    memDCDest.SelectObject(pOldBitmapDest);
    if (!AfxGetMainWnd()->OpenClipboard()) {
      AfxMessageBox(IDP_BCGBARRES_CANT_COPY_BITMAP);
      return FALSE;
    }
    if (!::EmptyClipboard()) {
      AfxMessageBox(IDP_BCGBARRES_CANT_COPY_BITMAP);
      ::CloseClipboard();
      return FALSE;
    }
    HANDLE hclipData = ::SetClipboardData(CF_BITMAP, bitmapCopy.Detach());
    if (hclipData == NULL) {
      AfxMessageBox(IDP_BCGBARRES_CANT_COPY_BITMAP);
      TRACE(_T("CBCGToolBar::OnBcgbarresCopyImage error. Error code = %x\n"), GetLastError());
    }
    ::CloseClipboard();
    return TRUE;
  }
  catch (...) {
    AfxMessageBox(IDP_BCGBARRES_INTERLAL_ERROR);
  }
  return FALSE;
}
BOOL CBCGPToolBarImages::MapBmpTo3dColors(img_t*& hBmp,
    BOOL bUseRGBQUAD/* = TRUE*/,
    COLORREF clrSrc/* = (COLORREF)-1*/,
    COLORREF clrDest/* = (COLORREF)-1*/)
{
  if (hBmp == NULL) {
    return FALSE;
  }
  if (clrSrc != (COLORREF) - 1 && clrDest == (COLORREF) - 1) {
    ASSERT(FALSE);
    return FALSE;
  }
  // Create memory source DC and select an original bitmap:
  CDC memDCSrc;
  memDCSrc.CreateCompatibleDC(NULL);
  img_t* hOldBitmapSrc = NULL;
  int iBitmapWidth;
  int iBitmapHeight;
  // Get original bitmap attrbutes:
  BITMAP bmp;
  if (::GetObject(hBmp, sizeof(BITMAP), &bmp) == 0) {
    return FALSE;
  }
  hOldBitmapSrc = (img_t*) memDCSrc.SelectObject(hBmp);
  if (hOldBitmapSrc == NULL) {
    return FALSE;
  }
  iBitmapWidth = bmp.bmWidth;
  iBitmapHeight = bmp.bmHeight;
  // Create a new bitmap compatibel with the source memory DC:
  // (original bitmap SHOULD BE ALREADY SELECTED!):
  img_t* hNewBitmap = (img_t*) ::CreateCompatibleBitmap(memDCSrc,
      iBitmapWidth,
      iBitmapHeight);
  if (hNewBitmap == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    return FALSE;
  }
  // Create memory destination DC:
  CDC memDCDst;
  memDCDst.CreateCompatibleDC(&memDCSrc);
  img_t* hOldBitmapDst = (img_t*) memDCDst.SelectObject(hNewBitmap);
  if (hOldBitmapDst == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    ::DeleteObject(hNewBitmap);
    return FALSE;
  }
  // Copy original bitmap to new:
  memDCDst.BitBlt(0, 0, iBitmapWidth, iBitmapHeight,
      &memDCSrc, 0, 0, SRCCOPY);
  // Change a specific colors to system:
  for (int x = 0; x < iBitmapWidth; x ++) {
    for (int y = 0; y < iBitmapHeight; y ++) {
      COLORREF clrOrig = ::GetPixel(memDCDst, x, y);
      if (clrSrc != (COLORREF) - 1) {
        if (clrOrig == clrSrc) {
          ::SetPixel(memDCDst, x, y, clrDest);
        }
      }
      else {
        COLORREF clrNew = bmp.bmBitsPixel == 24 && !m_bDisableTrueColorAlpha ?
            MapToSysColorAlpha(clrOrig) :
            MapToSysColor(clrOrig, bUseRGBQUAD);
        if (clrOrig != clrNew) {
          ::SetPixel(memDCDst, x, y, clrNew);
        }
      }
    }
  }
  memDCDst.SelectObject(hOldBitmapDst);
  memDCSrc.SelectObject(hOldBitmapSrc);
  ::DeleteObject(hBmp);
  hBmp = hNewBitmap;
  return TRUE;
}
BOOL CBCGPToolBarImages::MapTo3dColors(BOOL bUseRGBQUAD/* = TRUE*/,
    COLORREF clrSrc/* = (COLORREF)-1*/,
    COLORREF clrDest/* = (COLORREF)-1*/)
{
  return MapBmpTo3dColors(m_hbmImageWell, bUseRGBQUAD, clrSrc, clrDest);
}
BOOL CBCGPToolBarImages::UpdateInternalImage(int nIndex)
{
  img_t*& hbmpInternal = (nIndex == BCGImage_Light) ?
      m_hbmImageLight : m_hbmImageShadow;
  if (nIndex == BCGImage_Light) {
    if ((m_nBitsPerPixel > 4 && !m_bAlwaysLight) || m_nBitsPerPixel == 0) {
      // Down't fade 256+ or unknown bitmaps
      return FALSE;
    }
  }
  AfxDeleteObject((HGDIOBJ*)&hbmpInternal);
  hbmpInternal = NULL;
  if (m_hbmImageWell == NULL) {
    return TRUE;
  }
  OSVERSIONINFO osvi;
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  ::GetVersionEx(&osvi);
  if (globalData.m_nBitsPerPixel <= 8 ||
      osvi.dwPlatformId != VER_PLATFORM_WIN32_NT) {
    return TRUE;
  }
  // Create memory source DC and select an original bitmap:
  CDC memDCSrc;
  memDCSrc.CreateCompatibleDC(NULL);
  BITMAP bmp;
  if (::GetObject(m_hbmImageWell, sizeof(BITMAP), &bmp) == 0) {
    return FALSE;
  }
  int iBitmapWidth = bmp.bmWidth;
  int iBitmapHeight = bmp.bmHeight;
  img_t* hOldBitmapSrc = (img_t*) memDCSrc.SelectObject(m_hbmImageWell);
  if (hOldBitmapSrc == NULL) {
    return FALSE;
  }
  // Create memory destination DC and select a new bitmap:
  CDC memDCDst;
  memDCDst.CreateCompatibleDC(&memDCSrc);
  BITMAPINFO bi;
  // Fill in the BITMAPINFOHEADER
  bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bi.bmiHeader.biWidth = iBitmapWidth;
  bi.bmiHeader.biHeight = iBitmapHeight;
  bi.bmiHeader.biPlanes = 1;
  bi.bmiHeader.biBitCount = 32;
  bi.bmiHeader.biCompression = BI_RGB;
  bi.bmiHeader.biSizeImage = iBitmapWidth * iBitmapHeight;
  bi.bmiHeader.biXPelsPerMeter = 0;
  bi.bmiHeader.biYPelsPerMeter = 0;
  bi.bmiHeader.biClrUsed = 0;
  bi.bmiHeader.biClrImportant = 0;
  COLORREF* pBits = NULL;
  hbmpInternal = CreateDIBSection(
      memDCDst.m_hDC, &bi, DIB_RGB_COLORS, (void**)&pBits,
      NULL, NULL);
  if (hbmpInternal == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    return FALSE;
  }
  img_t* hOldBitmapDst = (img_t*) memDCDst.SelectObject(hbmpInternal);
  if (hOldBitmapDst == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    ::DeleteObject(hbmpInternal);
    hbmpInternal = NULL;
    return FALSE;
  }
  // Copy original bitmap to new:
  COLORREF clrTransparent = m_nBitsPerPixel == 32 && m_pfAlphaBlend != NULL ?
      (COLORREF) - 1 : m_clrTransparent;
  memDCDst.BitBlt(0, 0, iBitmapWidth, iBitmapHeight,
      &memDCSrc, 0, 0, SRCCOPY);
  if (nIndex == BCGImage_Light) {
    CBCGPDrawManager dm(memDCDst);
    dm.HighlightRect(CRect(0, 0, iBitmapWidth, iBitmapHeight),
        m_nLightPercentage,
        clrTransparent == -1 ? globalData.clrBtnFace : clrTransparent);
  }
  else {
    COLORREF clrTr =
        clrTransparent == -1 ? globalData.clrBtnFace : clrTransparent;
    COLORREF clrHL = CBCGPVisualManager::GetInstance()->GetToolbarHighlightColor();
    COLORREF clrShadow =
        globalData.m_nBitsPerPixel <= 8 ?
        globalData.clrBtnShadow :
        CBCGPDrawManager::PixelAlpha(clrHL, 67);
    for (int x = 0; x < iBitmapWidth; x++) {
      for (int y = 0; y < iBitmapHeight; y++) {
        COLORREF clr = memDCDst.GetPixel(x, y);
        if (clr != clrTr) {
          memDCDst.SetPixel(x, y, clrShadow);
        }
      }
    }
  }
  memDCDst.SelectObject(hOldBitmapDst);
  memDCSrc.SelectObject(hOldBitmapSrc);
  return TRUE;
}
BOOL CBCGPToolBarImages::PreMultiplyAlpha(img_t* hbmp, BOOL bAutoCheckPremlt)
{
  DIBSECTION ds;
  if (::GetObject(hbmp, sizeof(DIBSECTION), &ds) == 0) {
    ASSERT(FALSE);
    return FALSE;
  }
  if (ds.dsBm.bmBitsPixel != 32) {
    return FALSE;
  }
  int i = 0;
  RGBQUAD* pBits = (RGBQUAD*) ds.dsBm.bmBits;
  const int length = ds.dsBm.bmWidth * ds.dsBm.bmHeight;
  if (bAutoCheckPremlt) {
    BOOL bPremultiply = FALSE;
    RGBQUAD* pBit = pBits;
    for (i = 0; i < length; i++) {
      if (pBit->rgbRed > pBit->rgbReserved ||
          pBit->rgbGreen > pBit->rgbReserved ||
          pBit->rgbBlue > pBit->rgbReserved) {
        bPremultiply = TRUE;
        break;
      }
      pBit++;
    }
    if (!bPremultiply) {
      return TRUE;
    }
  }
  // Premultiply the R,G and B values with the Alpha channel values:
  RGBQUAD* pBit = pBits;
  for (i = 0; i < length; i++) {
    pBit->rgbRed = (BYTE)(pBit->rgbRed * pBit->rgbReserved / 255);
    pBit->rgbGreen = (BYTE)(pBit->rgbGreen * pBit->rgbReserved / 255);
    pBit->rgbBlue = (BYTE)(pBit->rgbBlue * pBit->rgbReserved / 255);
    pBit++;
  }
  return TRUE;
}
BOOL CBCGPToolBarImages::PreMultiplyAlpha(img_t* hbmp)
{
  return PreMultiplyAlpha(hbmp, m_bAutoCheckPremlt);
}
BOOL CBCGPToolBarImages::CreateFromImageList(const CImageList& imageList)
{
  ASSERT(imageList.GetSafeHandle() != NULL);
  ASSERT(imageList.GetImageCount() > 0);
  Clear();
  IMAGEINFO info;
  imageList.GetImageInfo(0, &info);
  CRect rectImage = info.rcImage;
  m_sizeImage = rectImage.Size();
  for (int i = 0; i < imageList.GetImageCount(); i++) {
    HICON hIcon = ((CImageList&) imageList).ExtractIcon(i);
    ASSERT(hIcon != NULL);
    AddIcon(hIcon);
    ::DestroyIcon(hIcon);
  }
  return TRUE;
}
BOOL CBCGPToolBarImages::Is32BitTransparencySupported()
{
  return globalData.bIsOSAlphaBlendingSupport;
}
BOOL CBCGPToolBarImages::GrayImages(int nGrayPercentage)
{
  m_bIsGray = TRUE;
  m_nGrayPercentage = nGrayPercentage;
  if (m_hbmImageWell == NULL) {
    return TRUE;
  }
  OSVERSIONINFO osvi;
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  ::GetVersionEx(&osvi);
  if (globalData.m_nBitsPerPixel <= 8 ||
      osvi.dwPlatformId != VER_PLATFORM_WIN32_NT) {
    return TRUE;
  }
  // Create memory source DC and select an original bitmap:
  CDC memDCSrc;
  memDCSrc.CreateCompatibleDC(NULL);
  BITMAP bmp;
  if (::GetObject(m_hbmImageWell, sizeof(BITMAP), &bmp) == 0) {
    return FALSE;
  }
  int iBitmapWidth = bmp.bmWidth;
  int iBitmapHeight = bmp.bmHeight;
  img_t* hOldBitmapSrc = (img_t*) memDCSrc.SelectObject(m_hbmImageWell);
  if (hOldBitmapSrc == NULL) {
    return FALSE;
  }
  // Create memory destination DC and select a new bitmap:
  CDC memDCDst;
  memDCDst.CreateCompatibleDC(&memDCSrc);
  BITMAPINFO bi;
  // Fill in the BITMAPINFOHEADER
  bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bi.bmiHeader.biWidth = iBitmapWidth;
  bi.bmiHeader.biHeight = iBitmapHeight;
  bi.bmiHeader.biPlanes = 1;
  bi.bmiHeader.biBitCount = 32;
  bi.bmiHeader.biCompression = BI_RGB;
  bi.bmiHeader.biSizeImage = iBitmapWidth * iBitmapHeight;
  bi.bmiHeader.biXPelsPerMeter = 0;
  bi.bmiHeader.biYPelsPerMeter = 0;
  bi.bmiHeader.biClrUsed = 0;
  bi.bmiHeader.biClrImportant = 0;
  COLORREF* pBits = NULL;
  img_t* hNewBitmap = CreateDIBSection(
      memDCDst.m_hDC, &bi, DIB_RGB_COLORS, (void**)&pBits,
      NULL, NULL);
  if (hNewBitmap == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    return FALSE;
  }
  img_t* hOldBitmapDst = (img_t*) memDCDst.SelectObject(hNewBitmap);
  if (hOldBitmapDst == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    ::DeleteObject(hNewBitmap);
    hNewBitmap = NULL;
    return FALSE;
  }
  // Copy original bitmap to new:
  memDCDst.BitBlt(0, 0, iBitmapWidth, iBitmapHeight,
      &memDCSrc, 0, 0, SRCCOPY);
  int nPercentage = m_nGrayPercentage <= 0 ? 130 : m_nGrayPercentage;
  if (m_nBitsPerPixel == 32 && m_pfAlphaBlend != NULL) {
    DIBSECTION ds;
    if (::GetObject(hNewBitmap, sizeof(DIBSECTION), &ds) == 0) {
      ASSERT(FALSE);
      return FALSE;
    }
    if (ds.dsBm.bmBitsPixel != 32) {
      ASSERT(FALSE);
      return FALSE;
    }
    RGBQUAD* pBits32 = (RGBQUAD*) ds.dsBm.bmBits;
    // Premultiply the R,G and B values with the Alpha channel values:
    for (int i = 0; i < ds.dsBm.bmWidth * ds.dsBm.bmHeight; i++) {
      RGBQUAD* pBit = pBits32 + i;
      double H, S, L;
      CBCGPDrawManager::RGBtoHSL(_RGB(pBit->rgbRed, pBit->rgbGreen, pBit->rgbBlue), &H, &S, &L);
      COLORREF color = CBCGPDrawManager::PixelAlpha(
          CBCGPDrawManager::HLStoRGB_ONE(H, L, 0),
          .01 * nPercentage, .01 * nPercentage, .01 * nPercentage);
      pBit->rgbRed = (BYTE)(GetRValue(color) * pBit->rgbReserved / 255);
      pBit->rgbGreen = (BYTE)(GetGValue(color) * pBit->rgbReserved / 255);
      pBit->rgbBlue = (BYTE)(GetBValue(color) * pBit->rgbReserved / 255);
    }
  }
  else {
    CBCGPDrawManager dm(memDCDst);
    dm.GrayRect(CRect(0, 0, iBitmapWidth, iBitmapHeight),
        nPercentage,
        m_clrTransparent == -1 ? globalData.clrBtnFace : m_clrTransparent);
  }
  memDCDst.SelectObject(hOldBitmapDst);
  memDCSrc.SelectObject(hOldBitmapSrc);
  ::DeleteObject(m_hbmImageWell);
  m_hbmImageWell = hNewBitmap;
  return TRUE;
}
BOOL CBCGPToolBarImages::MirrorBitmap(img_t*& hbmp, int cxImage)
{
  if (hbmp == NULL) {
    return TRUE;
  }
  BITMAP bmp;
  if (::GetObject(hbmp, sizeof(BITMAP), &bmp) == 0) {
    return FALSE;
  }
  const int cx = bmp.bmWidth;
  const int cy = bmp.bmHeight;
  const int iCount = cx / cxImage;
  if (bmp.bmBitsPixel == 32) {
    DIBSECTION ds;
    if (::GetObject(hbmp, sizeof(DIBSECTION), &ds) == 0) {
      ASSERT(FALSE);
      return FALSE;
    }
    if (ds.dsBm.bmBitsPixel != 32) {
      ASSERT(FALSE);
      return FALSE;
    }
    DWORD* pBits = (DWORD*) ds.dsBm.bmBits;
    for (int iImage = 0; iImage < iCount; iImage++) {
      for (int y = 0; y < cy; y++) {
        DWORD* pRow1 = pBits + cx * y + iImage * cxImage;
        DWORD* pRow2 = pRow1 + cxImage - 1;
        for (int x = 0; x < cxImage / 2; x++) {
          DWORD color = *pRow1;
          *pRow1 = *pRow2;
          *pRow2 = color;
          pRow1++;
          pRow2--;
        }
      }
    }
    return TRUE;
  }
  CDC memDC;
  memDC.CreateCompatibleDC(NULL);
  img_t* hOldBitmap = (img_t*) memDC.SelectObject(hbmp);
  if (hOldBitmap == NULL) {
    return FALSE;
  }
  for (int iImage = 0; iImage < iCount; iImage++) {
    for (int y = 0; y < cy; y++) {
      int x1 = iImage * cxImage;
      int x2 = x1 + cxImage - 1;
      for (int x = 0; x < cxImage / 2; x++) {
        COLORREF color = memDC.GetPixel(x1, y);
        memDC.SetPixel(x1, y, memDC.GetPixel(x2, y));
        memDC.SetPixel(x2, y, color);
        x1++;
        x2--;
      }
    }
  }
  memDC.SelectObject(hOldBitmap);
  return TRUE;
}
BOOL CBCGPToolBarImages::MirrorBitmapVert(img_t*& hbmp, int cyImage)
{
  if (hbmp == NULL) {
    return TRUE;
  }
  BITMAP bmp;
  if (::GetObject(hbmp, sizeof(BITMAP), &bmp) == 0) {
    return FALSE;
  }
  const int cx = bmp.bmWidth;
  const int cy = bmp.bmHeight;
  const int iCount = cy / cyImage;
  if (bmp.bmBitsPixel >= 16) {
    DIBSECTION ds;
    if (::GetObject(hbmp, sizeof(DIBSECTION), &ds) == 0) {
      ASSERT(FALSE);
      return FALSE;
    }
    if (ds.dsBm.bmBitsPixel != bmp.bmBitsPixel) {
      ASSERT(FALSE);
      return FALSE;
    }
    LPBYTE pBits = (LPBYTE)ds.dsBm.bmBits;
    DWORD pitch = cx * ds.dsBm.bmBitsPixel / 8;
    if (pitch % 4) {
      pitch = (DWORD)(pitch / 4) * 4 + 4;
    }
    LPBYTE pRowTemp = new BYTE[pitch];
    for (int iImage = 0; iImage < iCount; iImage++) {
      LPBYTE pRowBits1 = pBits + iImage * cyImage * pitch;
      LPBYTE pRowBits2 = pRowBits1 + (cyImage - 1) * pitch;
      for (int y = 0; y < cyImage / 2; y++) {
        memcpy(pRowTemp, pRowBits1, pitch);
        memcpy(pRowBits1, pRowBits2, pitch);
        memcpy(pRowBits2, pRowTemp, pitch);
        pRowBits1 += pitch;
        pRowBits2 -= pitch;
      }
    }
    delete [] pRowTemp;
    return TRUE;
  }
  CDC memDC;
  memDC.CreateCompatibleDC(NULL);
  img_t* hOldBitmap = (img_t*) memDC.SelectObject(hbmp);
  if (hOldBitmap == NULL) {
    return FALSE;
  }
  for (int iImage = 0; iImage < iCount; iImage++) {
    int y1 = iImage * cyImage;
    int y2 = y1 + cyImage - 1;
    for (int y = 0; y < cyImage / 2; y++) {
      for (int x = 0; x < cx; x++) {
        COLORREF color = memDC.GetPixel(x, y1);
        memDC.SetPixel(x, y1, memDC.GetPixel(x, y2));
        memDC.SetPixel(x, y2, color);
      }
      y1++;
      y2--;
    }
  }
  memDC.SelectObject(hOldBitmap);
  return TRUE;
}
BOOL CBCGPToolBarImages::MirrorVert()
{
  if (!MirrorBitmapVert(m_hbmImageWell, m_sizeImage.h)) {
    return FALSE;
  }
  if (m_hbmImageLight != NULL) {
    MirrorBitmapVert(m_hbmImageLight, m_sizeImage.h);
  }
  if (m_hbmImageShadow != NULL) {
    MirrorBitmapVert(m_hbmImageShadow, m_sizeImage.h);
  }
  return TRUE;
}
void CBCGPToolBarImages::EnableRTL(BOOL bIsRTL/* = TRUE*/)
{
  m_bIsRTL = bIsRTL;
}
void CBCGPToolBarImages::AddaptColors(COLORREF clrBase, COLORREF clrTone)
{
  double dSrcH, dSrcS, dSrcL;
  CBCGPDrawManager::RGBtoHSL(clrBase, &dSrcH, &dSrcS, &dSrcL);
  double dDestH, dDestS, dDestL;
  CBCGPDrawManager::RGBtoHSL(clrTone, &dDestH, &dDestS, &dDestL);
  double DH = dDestH - dSrcH;
  double DL = dDestL - dSrcL;
  double DS = dDestS - dSrcS;
  if (m_nBitsPerPixel >= 24) {
    DIBSECTION ds;
    if (::GetObject(m_hbmImageWell, sizeof(DIBSECTION), &ds) == 0) {
      ASSERT(FALSE);
      return;
    }
    if (ds.dsBm.bmBitsPixel < 24) {
      ASSERT(FALSE);
      return;
    }
    if (ds.dsBm.bmBitsPixel == 24) {
      COLORREF clrTransparent =
          m_clrTransparent == (COLORREF) - 1 ? globalData.clrBtnFace : m_clrTransparent;
      DWORD dwPitch = ((ds.dsBm.bmWidth * 24 + 31) & ~31) / 8;
      LPBYTE pBits = (LPBYTE)ds.dsBm.bmBits;
      for (int y = 0; y < ds.dsBm.bmHeight; y++) {
        LPBYTE pBitsRow = pBits;
        for (int x = 0; x < ds.dsBm.bmWidth; x++) {
          COLORREF clrOrig = _RGB(pBitsRow[2], pBitsRow[1], pBitsRow[0]);
          if (clrOrig == clrTransparent) {
            pBitsRow += 3;
            continue;
          }
          double H, S, L;
          CBCGPDrawManager::RGBtoHSL(clrOrig, &H, &S, &L);
          if (fabs(dSrcH - H) < m_dblColorTolerance) {
            double HNew = max(0., min(3.59, H + DH));
            double SNew = max(0., min(1.00, S + DS));
            double LNew = max(0., min(1.00, L + DL));
            COLORREF color = CBCGPDrawManager::HLStoRGB_ONE(
                HNew, dDestH > 0.5 ? L : LNew, SNew);
            pBitsRow[2] = (BYTE)(GetRValue(color));
            pBitsRow[1] = (BYTE)(GetGValue(color));
            pBitsRow[0] = (BYTE)(GetBValue(color));
          }
          pBitsRow += 3;
        }
        pBits += dwPitch;
      }
    }
    else {
      RGBQUAD* pBits = (RGBQUAD*) ds.dsBm.bmBits;
      for (int i = 0; i < ds.dsBm.bmWidth * ds.dsBm.bmHeight; i++) {
        RGBQUAD* pBit = pBits + i;
        if (pBit->rgbReserved == 0) {
          continue;
        }
        double a = (double)pBit->rgbReserved / 255.0;
        double aR = 255.0 / (double)pBit->rgbReserved;
        COLORREF clrOrig = _RGB(pBit->rgbRed * aR, pBit->rgbGreen * aR, pBit->rgbBlue * aR);
        double H, S, L;
        CBCGPDrawManager::RGBtoHSL(clrOrig, &H, &S, &L);
        if (fabs(dSrcH - H) < m_dblColorTolerance) {
          double HNew = max(0., min(3.59, H + DH));
          double SNew = max(0., min(1.00, S + DS));
          double LNew = max(0., min(1.00, L + DL));
          COLORREF color = CBCGPDrawManager::HLStoRGB_ONE(
              HNew, dDestH > 0.5 ? L : LNew, SNew);
          pBit->rgbRed = (BYTE)(GetRValue(color) * a);
          pBit->rgbGreen = (BYTE)(GetGValue(color) * a);
          pBit->rgbBlue = (BYTE)(GetBValue(color) * a);
        }
      }
    }
    return;
  }
  // Create memory source DC and select an original bitmap:
  CDC memDCSrc;
  memDCSrc.CreateCompatibleDC(NULL);
  img_t* hOldBitmapSrc = NULL;
  int iBitmapWidth;
  int iBitmapHeight;
  // Get original bitmap attrbutes:
  BITMAP bmp;
  if (::GetObject(m_hbmImageWell, sizeof(BITMAP), &bmp) == 0) {
    return;
  }
  hOldBitmapSrc = (img_t*) memDCSrc.SelectObject(m_hbmImageWell);
  if (hOldBitmapSrc == NULL) {
    return;
  }
  iBitmapWidth = bmp.bmWidth;
  iBitmapHeight = bmp.bmHeight;
  // Create a new bitmap compatibel with the source memory DC:
  // (original bitmap SHOULD BE ALREADY SELECTED!):
  img_t* hNewBitmap = (img_t*) ::CreateCompatibleBitmap(memDCSrc,
      iBitmapWidth,
      iBitmapHeight);
  if (hNewBitmap == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    return;
  }
  // Create memory destination DC:
  CDC memDCDst;
  memDCDst.CreateCompatibleDC(&memDCSrc);
  img_t* hOldBitmapDst = (img_t*) memDCDst.SelectObject(hNewBitmap);
  if (hOldBitmapDst == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    ::DeleteObject(hNewBitmap);
    return;
  }
  // Copy original bitmap to new:
  memDCDst.BitBlt(0, 0, iBitmapWidth, iBitmapHeight,
      &memDCSrc, 0, 0, SRCCOPY);
  COLORREF clrTransparent =
      m_clrTransparent == (COLORREF) - 1 ? globalData.clrBtnFace : m_clrTransparent;
  for (int x = 0; x < iBitmapWidth; x ++) {
    for (int y = 0; y < iBitmapHeight; y ++) {
      COLORREF clrOrig = ::GetPixel(memDCDst, x, y);
      if (clrOrig == clrTransparent) {
        continue;
      }
      double H, L, S;
      CBCGPDrawManager::RGBtoHSL(clrOrig, &H, &S, &L);
      if (fabs(dSrcH - H) < m_dblColorTolerance) {
        double HNew = max(0., min(3.59, H + DH));
        double SNew = max(0., min(1.00, S + DS));
        double LNew = max(0., min(1.00, L + DL));
        COLORREF clrNew = CBCGPDrawManager::HLStoRGB_ONE(
            HNew, dDestH > 0.5 ? L : LNew, SNew);
        if (clrOrig != clrNew) {
          ::SetPixel(memDCDst, x, y, clrNew);
        }
      }
    }
  }
  memDCDst.SelectObject(hOldBitmapDst);
  memDCSrc.SelectObject(hOldBitmapSrc);
  ::DeleteObject(m_hbmImageWell);
  m_hbmImageWell = hNewBitmap;
  AfxDeleteObject((HGDIOBJ*)&m_hbmImageLight);
  m_hbmImageLight = NULL;
  AfxDeleteObject((HGDIOBJ*)&m_hbmImageShadow);
  m_hbmImageShadow = NULL;
}
#define RGB_MID(r, g, b) (BYTE)((r + g + b) / 3)
#define RGB_MID_COLOR(clr) RGB_MID(GetRValue (clr), GetGValue (clr), GetBValue (clr))
#define RGB_LUM(r, g, b) (BYTE)(((DWORD)r * 299 + (DWORD)g * 587 + (DWORD)b * 114) / 1000)
#define RGB_LUM_COLOR(clr) RGB_LUM(GetRValue (clr), GetGValue (clr), GetBValue (clr))
void CBCGPToolBarImages::AddaptColors(COLORREF clrBase, COLORREF clrTone, double dOpacity/* = 1.0*/)
{
  double dH, dS, dL;
  CBCGPDrawManager::RGBtoHSL(clrTone, &dH, &dS, &dL);
  BOOL bUseBase = clrBase != COLORREF(-1) && clrBase != CLR_DEFAULT;
  double dBaseH = 0.0, dBaseS, dBaseL;
  if (bUseBase) {
    CBCGPDrawManager::RGBtoHSL(clrBase, &dBaseH, &dBaseS, &dBaseL);
  }
  DWORD dwLum = RGB_LUM_COLOR(clrTone);
  if (m_nBitsPerPixel >= 24) {
    DIBSECTION ds;
    if (::GetObject(m_hbmImageWell, sizeof(DIBSECTION), &ds) == 0) {
      ASSERT(FALSE);
      return;
    }
    if (ds.dsBm.bmBitsPixel < 24) {
      ASSERT(FALSE);
      return;
    }
    if (ds.dsBm.bmBitsPixel == 24) {
      COLORREF clrTransparent =
          m_clrTransparent == (COLORREF) - 1 ? globalData.clrBtnFace : m_clrTransparent;
      DWORD dwPitch = ((ds.dsBm.bmWidth * 24 + 31) & ~31) / 8;
      LPBYTE pBits = (LPBYTE)ds.dsBm.bmBits;
      for (int y = 0; y < ds.dsBm.bmHeight; y++) {
        LPBYTE pBitsRow = pBits;
        for (int x = 0; x < ds.dsBm.bmWidth; x++) {
          COLORREF clrOrig = _RGB(pBitsRow[2], pBitsRow[1], pBitsRow[0]);
          if (clrOrig == clrTransparent) {
            pBitsRow += 3;
            continue;
          }
          if (bUseBase) {
            double H, L, S;
            CBCGPDrawManager::RGBtoHSL(clrOrig, &H, &S, &L);
            if (fabs(dBaseH - H) > m_dblColorTolerance) {
              pBitsRow += 3;
              continue;
            }
          }
          DWORD dwLumPix = RGB_LUM(pBitsRow[2], pBitsRow[1], pBitsRow[0]);
          if (dwLumPix <= 128) {
            dwLumPix = dwLum * dwLumPix / 128;
          }
          else {
            dwLumPix = (255 - dwLum) * (dwLumPix - 128) / 128 + dwLum;
          }
          COLORREF color = CBCGPDrawManager::HLStoRGB_ONE(
              dH, dwLumPix / 255.0, dS * dOpacity);
          pBitsRow[2] = (BYTE)(GetRValue(color));
          pBitsRow[1] = (BYTE)(GetGValue(color));
          pBitsRow[0] = (BYTE)(GetBValue(color));
          pBitsRow += 3;
        }
        pBits += dwPitch;
      }
    }
    else {
      RGBQUAD* pBits = (RGBQUAD*) ds.dsBm.bmBits;
      for (int i = 0; i < ds.dsBm.bmWidth * ds.dsBm.bmHeight; i++) {
        RGBQUAD* pBit = pBits + i;
        if (pBit->rgbReserved == 0) {
          continue;
        }
        if (bUseBase) {
          COLORREF clrOrig = _RGB(pBit->rgbRed, pBit->rgbGreen, pBit->rgbBlue);
          double H, L, S;
          CBCGPDrawManager::RGBtoHSL(clrOrig, &H, &S, &L);
          if (fabs(dBaseH - H) > m_dblColorTolerance) {
            continue;
          }
        }
        double a = (double)pBit->rgbReserved / 255.0;
        double aR = 255.0 / (double)pBit->rgbReserved;
        DWORD dwLumPix = RGB_LUM(pBit->rgbRed * aR, pBit->rgbGreen * aR, pBit->rgbBlue * aR);
        if (dwLumPix <= 128) {
          dwLumPix = dwLum * dwLumPix / 128;
        }
        else {
          dwLumPix = (255 - dwLum) * (dwLumPix - 128) / 128 + dwLum;
        }
        COLORREF color = CBCGPDrawManager::HLStoRGB_ONE(
            dH, dwLumPix / 255.0, dS * dOpacity);
        pBit->rgbRed = (BYTE)(GetRValue(color) * a);
        pBit->rgbGreen = (BYTE)(GetGValue(color) * a);
        pBit->rgbBlue = (BYTE)(GetBValue(color) * a);
      }
    }
    return;
  }
  // Create memory source DC and select an original bitmap:
  CDC memDCSrc;
  memDCSrc.CreateCompatibleDC(NULL);
  img_t* hOldBitmapSrc = NULL;
  int iBitmapWidth;
  int iBitmapHeight;
  // Get original bitmap attrbutes:
  BITMAP bmp;
  if (::GetObject(m_hbmImageWell, sizeof(BITMAP), &bmp) == 0) {
    return;
  }
  hOldBitmapSrc = (img_t*) memDCSrc.SelectObject(m_hbmImageWell);
  if (hOldBitmapSrc == NULL) {
    return;
  }
  iBitmapWidth = bmp.bmWidth;
  iBitmapHeight = bmp.bmHeight;
  // Create a new bitmap compatibel with the source memory DC:
  // (original bitmap SHOULD BE ALREADY SELECTED!):
  img_t* hNewBitmap = (img_t*) ::CreateCompatibleBitmap(memDCSrc,
      iBitmapWidth,
      iBitmapHeight);
  if (hNewBitmap == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    return;
  }
  // Create memory destination DC:
  CDC memDCDst;
  memDCDst.CreateCompatibleDC(&memDCSrc);
  img_t* hOldBitmapDst = (img_t*) memDCDst.SelectObject(hNewBitmap);
  if (hOldBitmapDst == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    ::DeleteObject(hNewBitmap);
    return;
  }
  // Copy original bitmap to new:
  memDCDst.BitBlt(0, 0, iBitmapWidth, iBitmapHeight,
      &memDCSrc, 0, 0, SRCCOPY);
  COLORREF clrTransparent =
      m_clrTransparent == (COLORREF) - 1 ? globalData.clrBtnFace : m_clrTransparent;
  for (int x = 0; x < iBitmapWidth; x ++) {
    for (int y = 0; y < iBitmapHeight; y ++) {
      COLORREF clrOrig = ::GetPixel(memDCDst, x, y);
      if (clrOrig == clrTransparent) {
        continue;
      }
      if (bUseBase) {
        double H, L, S;
        CBCGPDrawManager::RGBtoHSL(clrOrig, &H, &S, &L);
        if (fabs(dBaseH - H) > m_dblColorTolerance) {
          continue;
        }
      }
      DWORD dwLumPix = RGB_LUM_COLOR(clrOrig);
      if (dwLumPix <= 128) {
        dwLumPix = dwLum * dwLumPix / 128;
      }
      else {
        dwLumPix = (255 - dwLum) * (dwLumPix - 128) / 128 + dwLum;
      }
      COLORREF color = CBCGPDrawManager::HLStoRGB_ONE(
          dH, dwLumPix / 255.0, dS * dOpacity);
      if (clrOrig != color) {
        ::SetPixel(memDCDst, x, y, color);
      }
    }
  }
  memDCDst.SelectObject(hOldBitmapDst);
  memDCSrc.SelectObject(hOldBitmapSrc);
  ::DeleteObject(m_hbmImageWell);
  m_hbmImageWell = hNewBitmap;
  AfxDeleteObject((HGDIOBJ*)&m_hbmImageLight);
  m_hbmImageLight = NULL;
  AfxDeleteObject((HGDIOBJ*)&m_hbmImageShadow);
  m_hbmImageShadow = NULL;
}
HRGN CBCGPToolBarImages::CreateRegionFromImage(img_t* hbmp, COLORREF clrTransparent)
{
  if (hbmp == NULL) {
    ASSERT(FALSE);
    return NULL;
  }
  BITMAP bmp;
  if (::GetObject(hbmp, sizeof(BITMAP), &bmp) == 0) {
    ASSERT(FALSE);
    return NULL;
  }
  CDC dc;
  dc.CreateCompatibleDC(NULL);
  img_t* hbmpOld = (img_t*) dc.SelectObject(hbmp);
  int iBitmapWidth = bmp.bmWidth;
  int iBitmapHeight = bmp.bmHeight;
  CRgn rgnAll;
  rgnAll.CreateRectRgn(0, 0, iBitmapWidth, iBitmapHeight);
  for (int y = 0; y < iBitmapHeight; y++) {
    for (int x = 0; x < iBitmapWidth; x++) {
      COLORREF color = dc.GetPixel(x, y);
      if (color == clrTransparent) {
        CRgn rgnPoint;
        rgnPoint.CreateRectRgn(x, y, x + 1, y + 1);
        rgnAll.CombineRgn(&rgnAll, &rgnPoint, RGN_DIFF);
      }
    }
  }
  dc.SelectObject(hbmpOld);
  return (HRGN) rgnAll.Detach();
}
void CBCGPToolBarImages::SetSingleImage()
{
  if (m_hbmImageWell == NULL) {
    return;
  }
  BITMAP bmp;
  if (::GetObject(m_hbmImageWell, sizeof(BITMAP), &bmp) == 0) {
    ASSERT(FALSE);
    return;
  }
  m_sizeImage.w = bmp.bmWidth;
  m_sizeImage.h = bmp.bmHeight;
  m_iCount = 1;
  UpdateInternalImage(BCGImage_Light);
  UpdateInternalImage(BCGImage_Shadow);
}
img_t* CBCGPToolBarImages::Copy(img_t* hbmpSrc)
{
  if (hbmpSrc == NULL) {
    ASSERT(FALSE);
    return NULL;
  }
  if (!globalData.bIsWindowsVista) {
    return (img_t*) ::CopyImage(hbmpSrc, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
  }
  // Windows Vista has some problems in ::CopyImage method,
  // copy bitmap not using this method:
  CDC memDCSrc;
  memDCSrc.CreateCompatibleDC(NULL);
  img_t* hOldBitmapSrc = (img_t*) memDCSrc.SelectObject(hbmpSrc);
  if (hOldBitmapSrc == NULL) {
    return NULL;
  }
  BITMAP bmp;
  ::GetObject(hbmpSrc, sizeof(BITMAP), &bmp);
  // Create a new bitmap compatibel with the source memory DC:
  img_t* hNewBitmap = (img_t*) ::CreateCompatibleBitmap(memDCSrc,
      bmp.bmWidth,
      bmp.bmHeight);
  if (hNewBitmap == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    return NULL;
  }
  // Create memory destination DC and select a new bitmap:
  CDC memDCDst;
  memDCDst.CreateCompatibleDC(&memDCSrc);
  img_t* hOldBitmapDst = (img_t*) memDCDst.SelectObject(hNewBitmap);
  if (hOldBitmapDst == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    ::DeleteObject(hNewBitmap);
    return NULL;
  }
  // Copy original bitmap to new:
  memDCDst.BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, &memDCSrc, 0, 0, SRCCOPY);
  memDCDst.SelectObject(hOldBitmapDst);
  memDCSrc.SelectObject(hOldBitmapSrc);
  return hNewBitmap;
}
BOOL CBCGPToolBarImages::SmoothResize(double dblImageScale)
{
  if (m_hbmImageWell == NULL) {
    return FALSE;
  }
  if (m_nBitsPerPixel < 24) {
    return FALSE;
  }
  if (dblImageScale == 0.0) {
    ASSERT(FALSE);
    return FALSE;
  }
  if (dblImageScale == 1.0) {
    return TRUE;
  }
  CSize sizeNew((int)(.5 + m_sizeImage.w * dblImageScale), (int)(.5 + m_sizeImage.h * dblImageScale));
  if (sizeNew == m_sizeImage ||
      m_sizeImage.w <= 0 || m_sizeImage.h <= 0 ||
      sizeNew.w <= 0 || sizeNew.h <= 0) {
    return TRUE;
  }
  int nImageCount = GetCount();
  if (nImageCount == 0) {
    return TRUE;
  }
  CSize sizeIW(0, 0);
  {
    BITMAP bmp;
    if (::GetObject(GetImageWell(), sizeof(BITMAP), &bmp) == 0) {
      ASSERT(FALSE);
      return FALSE;
    }
    sizeIW.w = bmp.bmWidth;
    sizeIW.h = bmp.bmHeight;
  }
  m_dblScale *= dblImageScale;
  CPoint offSrc(m_sizeImage.w, 0);
  CPoint offDst(0, 0);
  if (nImageCount == 1) {
    if (sizeIW.h > m_sizeImage.h) {
      nImageCount = sizeIW.h / m_sizeImage.h;
      offSrc = CPoint(0, m_sizeImage.h);
    }
  }
  img_t* hBmpSrc = CBCGPDrawManager::CreateBitmap_32(m_hbmImageWell, m_clrTransparent);
  if (hBmpSrc == NULL) {
    ASSERT(FALSE);
    return FALSE;
  }
  CSize sizeNewIW(sizeNew);
  if (offSrc.x > 0) {
    sizeNewIW.w *= nImageCount;
    offDst.x = sizeNew.w;
  }
  else {
    sizeNewIW.h *= nImageCount;
    offDst.y = sizeNew.h;
  }
  img_t* hBmpDst = CBCGPDrawManager::CreateBitmap_32(sizeNewIW, NULL);
  if (hBmpDst == NULL) {
    ASSERT(FALSE);
    ::DeleteObject(hBmpSrc);
    return FALSE;
  }
  CBCGPZoomKernel::XFilterType ft = CBCGPZoomKernel::e_FilterTypeMitchell;
  CBCGPScanlinerBitmap ms;
  ms.attach(hBmpSrc);
  CBCGPScanlinerBitmap md;
  md.attach(hBmpDst);
  DWORD channel = ms.m_channels;
  CBCGPZoomKernel KernelX;
  KernelX.Create(m_sizeImage.w, sizeNew.w, 0, m_sizeImage.w, ft);
  CBCGPZoomKernel KernelY;
  KernelY.Create(m_sizeImage.h, sizeNew.h, 0, m_sizeImage.h, ft);
  double* values = new double[channel];
  double* values2 = new double[channel];
  CPoint offSrcSum(0, 0);
  CPoint offDstSum(0, 0);
  for (int index = 0; index < nImageCount; index++) {
    const DWORD val_size = sizeof(double) * channel;
    const DWORD offsetDstX = offDstSum.x * channel;
    for (DWORD dy = 0; dy < (DWORD)sizeNew.h; dy++) {
      const CBCGPZoomKernel::XKernelList& listY = KernelY[dy];
      LPBYTE pRowDst = md[dy + offDstSum.y] + offsetDstX;
      for (DWORD dx = 0; dx < (DWORD)sizeNew.w; dx++) {
        const CBCGPZoomKernel::XKernelList& listX = KernelX[dx];
        memset(values, 0, val_size);
        for (DWORD sy = 0; sy < listY.count; sy++) {
          const CBCGPZoomKernel::XKernel& statY = listY.stat[sy];
          const LPBYTE pRowSrc = ms[statY.pixel + offSrcSum.y];
          double weight = statY.weight;
          memset(values2, 0, val_size);
          for (DWORD sx = 0; sx < listX.count; sx++) {
            const CBCGPZoomKernel::XKernel& statX = listX.stat[sx];
            LPBYTE pRowSrc2 = pRowSrc + (statX.pixel + offSrcSum.x) * channel;
            double weight2 = statX.weight;
            for (DWORD c = 0; c < channel; c++) {
              values2[c] += (double)(*pRowSrc2) * weight2;
              pRowSrc2++;
            }
          }
          for (DWORD c = 0; c < channel; c++) {
            values[c] += values2[c] * weight;
          }
        }
        if (channel == 4) {
          values[0] = min(values[0], values[3]);
          values[1] = min(values[1], values[3]);
          values[2] = min(values[2], values[3]);
        }
        for (DWORD c = 0; c < channel; c++) {
          *pRowDst = (BYTE)clamp_to_byte(values[c]);
          pRowDst++;
        }
      }
    }
    offSrcSum.x += offSrc.x;
    offSrcSum.y += offSrc.y;
    offDstSum.x += offDst.x;
    offDstSum.y += offDst.y;
  }
  delete [] values;
  delete [] values2;
  ::DeleteObject(hBmpSrc);
  int nOldCount = m_iCount;
  if (m_sizeImageOriginal == CSize(0, 0)) {
    m_sizeImageOriginal = m_sizeImage;
  }
  SetImageSize(sizeNew);
  m_clrTransparentOriginal = m_clrTransparent;
  m_clrTransparent = (COLORREF) - 1;
  m_hbmImageWell = hBmpDst;
  m_iCount = nOldCount;
  m_nBitsPerPixel = 32;
  UpdateInternalImage(BCGImage_Light);
  UpdateInternalImage(BCGImage_Shadow);
  return IsValid();
}
#endif

