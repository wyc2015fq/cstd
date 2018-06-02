#define MOVEDEF int _x0, _y0
#include <math.h>
//#define __TRACE_PAINT_SESSIONS
#define __ExtMfc_CXTEXTMARGIN (-1) // 1
#define __ExtMfc_CYTEXTMARGIN (0) // 1
#define VERIFY(_X) (_X)
int* m_pPaintManager;
BOOL m_bHaveScrollLines;
uint m_uCachedScrollLines;
TCHAR m_strOsVer[256];
OSVERSIONINFO m_osVerData;
// quick BOOLean helpers
BOOL m_bIsWin32s // old Win32s
, m_bIsWin9x // any Windows 9x
, m_bIsWin98 // exactly Windows 98
, m_bIsWin98orLater // Windows 98 or later
, m_bIsWinNT // any NT
, m_bIsWinNT4 // exactly Windows NT 4
, m_bIsWin2000 // exactly Windows 2000
, m_bIsWinXP // exactly Windows XP
, m_bIsWinNT4orLater // Windows NT 4 or later
, m_bIsWin2000orLater // Windows 2000 or later
, m_bIsWinXPorLater // Windows XP or later
;
HINSTANCE m_hInstUserExTheme;
BOOL m_bUxApiInitPassed, m_bUxAppThemed, m_bUxValidColorsExtracted, m_bUxUseIfAvailOnWinXpOrLater;
typedef int CExtPaintManager;
#define __3D_COLOR_DEPTH_R 7
#define __3D_COLOR_DEPTH_G 6
#define __3D_COLOR_DEPTH_B 6
int InitCExtPaintManager()
{
  int i = 0, nX, nY, nZ;
  m_bUseTahomaFont = (TRUE);
  m_DSS_DISABLED_style = DSS_DISABLED;
  /**/
  m_PaletteWideNum = (WORD)(__3D_COLOR_DEPTH_R * __3D_COLOR_DEPTH_G * __3D_COLOR_DEPTH_B);

  for (nX = 0; nX < __3D_COLOR_DEPTH_R; nX++) {
    for (nY = 0; nY < __3D_COLOR_DEPTH_G; nY++) {
      for (nZ = 0; nZ < __3D_COLOR_DEPTH_B; nZ++) {
        m_PaletteWide[i].rgbRed = (BYTE)((nX * 255) / (__3D_COLOR_DEPTH_R - 1));
        m_PaletteWide[i].rgbGreen = (BYTE)((nY * 255) / (__3D_COLOR_DEPTH_G - 1));
        m_PaletteWide[i].rgbBlue = (BYTE)((nZ * 255) / (__3D_COLOR_DEPTH_B - 1));
        m_PaletteWide[i].rgbReserved = 0;
        i++;
      }
    }
  }

  // XP
  m_bEnabledHoverIconShadows = (TRUE), m_bHelperXpStyle8BitBarRect = (TRUE), m_bExpBtnSwapVGlyphs = (FALSE);
  m_pGlyphTbEpBtnH0 = (&g_glyph_btn_expand_bottom), m_pGlyphTbEpBtnH1 = (&g_glyph_btn_expand_right2);
  m_pGlyphTbEpBtnV0 = (&g_glyph_btn_expand_left), m_pGlyphTbEpBtnV1 = (&g_glyph_btn_expand_bottom2);
  m_nIdxClrMlaNormLeft = (- 1), m_nIdxClrMlaNormRight = (- 1), m_nIdxClrMlaRarelyLeft = (- 1);
  m_nIdxClrMlaRarelyRight = (- 1), m_nIdxClrBtnHoverLeft = (- 1), m_nIdxClrBtnHoverRight = (- 1);
  m_nIdxClrBtnPressedLeft = (- 1), m_nIdxClrBtnPressedRight = (- 1);
  m_nIdxClrBtnHovPresLeft = (- 1), m_nIdxClrBtnHovPresRight = (- 1);
  // Office2003
  m_sizeToolBarRgnRounders.w = 4, m_sizeToolBarRgnRounders.h = 4;
  m_sizeMenuExpBtnCircle.w = 16, m_sizeMenuExpBtnCircle.h = 16;
  m_nIdxClrTbFillMargin = (- 1);
  m_bEnabledHoverIconShadows = FALSE;
  m_bHelperXpStyle8BitBarRect = FALSE;
  m_bExpBtnSwapVGlyphs = TRUE;
  m_pGlyphTbEpBtnH0 = &g_glyph_btn_expand_bottom_2003;
  m_pGlyphTbEpBtnH1 = &g_glyph_btn_expand_right2_2003;
  m_pGlyphTbEpBtnV0 = &g_glyph_btn_expand_right_2003;
  m_pGlyphTbEpBtnV1 = &g_glyph_btn_expand_bottom2_2003;
  m_nIdxClrMlaNormLeft = _2003CLR_MLA_NORM_LEFT;
  m_nIdxClrMlaNormRight = _2003CLR_MLA_NORM_RIGHT;
  m_nIdxClrMlaRarelyLeft = _2003CLR_MLA_RARELY_LEFT;
  m_nIdxClrMlaRarelyRight = _2003CLR_MLA_RARELY_RIGHT;
  return 0;
}
HDDC CachedDcGet(Image* hBmp)
{
  static TDrawDC hDC[1];
  HDDCSET_IMAGE(hDC, hBmp);
  return hDC;
}
int stat_CloneBitmap(const Image* hBmpSrc, const RECT* pRcPart, Image* hBmpDst)
{
  int cn = hBmpSrc->s / hBmpSrc->w, w = RCWIDTH(pRcPart), h = RCHEIGHT(pRcPart);
  int aa = newImage(w, h, cn, hBmpDst);
  DWORD* src = (DWORD*)(hBmpSrc->data + pRcPart->top * hBmpSrc->s) + pRcPart->left * cn;
  memcpy2d(hBmpDst->data, hBmpDst->s, src, hBmpSrc->s, MIN(h, hBmpSrc->h - pRcPart->top), MIN(w, hBmpSrc->w - pRcPart->left));
  return hBmpDst;
}
typedef struct _color_pair_t {
  COLORREF clr;
  DWORD count;
} color_pair_t;
typedef struct _color_map_t {
  int count;
  color_pair_t* pair;
} color_map_t;
// returns bitmap's color bit count or zero if error
// key is color, value is count of color entries in bitmap
INT stat_GetBitmapColorMap(Image* hBmpSrc, color_map_t* _map)
{
  LONG i, j, nPixelCount = hBmpSrc->h * hBmpSrc->w;
  COLORREF* pColor = (COLORREF*)(hBmpSrc + 1);
  _map->count = 0;
  _map->pair = MALLOC(color_pair_t, nPixelCount);

  for (i = 0; i < nPixelCount; ++i) {
    COLORREF clrToInsert = pColor[i];

    for (j = 0; j < _map->count; ++j) {
      if (clrToInsert == _map->pair[j].clr) {
        break;
      }
    }

    if (j < _map->count) {
      _map->pair[j].count++;
    }
    else {
      j = _map->count;
      _map->pair[j].clr = clrToInsert;
      _map->pair[j].count = 1;
      _map->count++;
    }
  }

  return 0;
}
// = RT_BITMAP, // default bitmap section
// = NULL // find it automatically
int stat_LoadBitmapResource(__EXT_MFC_SAFE_LPCTSTR sResourceID, __EXT_MFC_SAFE_LPCTSTR sResourceType, HINSTANCE hInst, Image* hBmp)
{
  HGLOBAL hGlobal;
  DWORD dwResourceSize;
  LPBYTE pData;
  HRSRC hRsrc = FindResource(hInst, sResourceID, sResourceType);

  if (hRsrc == NULL) {
    return NULL;
  }

  if (hInst == NULL || hRsrc == NULL) {
    return NULL;
  }

  hGlobal = LoadResource(hInst, hRsrc);

  if (hGlobal == NULL) {
    return NULL;
  }

  pData = (LPBYTE)LockResource(hGlobal);

  if (pData == NULL) {
    FreeResource(hGlobal);
    return NULL;
  } // if( pData == NULL )

  dwResourceSize = SizeofResource(hInst, hRsrc);
  ASSERT(dwResourceSize > 0);
  loadbmpmem(pData, 32, 1, hBmp);
  UnlockResource(hGlobal);
  FreeResource(hGlobal);
  ASSERT(hBmp != NULL);
  return hBmp;
}
int stat_GetMonitorParms(monitor_parms_t* _mp)
{
  VERIFY(GetWindowRect(GetDesktopWindow(), &_mp->m_rcMonitor));
  VERIFY(SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)(&_mp->m_rcWorkArea), 0));
  _mp->m_bPrimaryMonitor = TRUE;
}
int stat_GetMonitorParms1(monitor_parms_t* _mp, POINT _ptScreenFrom)
{
  MONITORINFO _MonitorInfo;
  memset(&_MonitorInfo, 0, sizeof(MONITORINFO));
  _MonitorInfo.cbSize = sizeof(MONITORINFO);

  if (!GetMonitorInfo(MonitorFromPoint(_ptScreenFrom, MONITOR_DEFAULTTONEAREST), &_MonitorInfo)) {
    ASSERT(FALSE);
    stat_GetMonitorParms(_mp);
    return ;
  }

  _mp->m_rcMonitor = _MonitorInfo.rcMonitor;
  _mp->m_rcWorkArea = _MonitorInfo.rcWork;
  _mp->m_bPrimaryMonitor = ((_MonitorInfo.dwFlags & MONITORINFOF_PRIMARY) != 0) ? TRUE : FALSE;
}
int stat_GetMonitorParms2(monitor_parms_t* _mp, const RECT* _rcScreenFrom)
{
  MONITORINFO _MonitorInfo;
  memset(&_MonitorInfo, 0, sizeof(MONITORINFO));
  _MonitorInfo.cbSize = sizeof(MONITORINFO);

  if (!GetMonitorInfo(MonitorFromRect(_rcScreenFrom, MONITOR_DEFAULTTONEAREST), &_MonitorInfo)) {
    ASSERT(FALSE);
    stat_GetMonitorParms(_mp);
    return ;
  }

  _mp->m_rcMonitor = _MonitorInfo.rcMonitor;
  _mp->m_rcWorkArea = _MonitorInfo.rcWork;
  _mp->m_bPrimaryMonitor = ((_MonitorInfo.dwFlags & MONITORINFOF_PRIMARY) != 0) ? TRUE : FALSE;
}
int stat_GetMonitorParms3(monitor_parms_t* _mp, HWND hWndFrom)
{
  RECT rcWnd = {0, 0, 0, 0};
  ASSERT(hWndFrom != NULL && IsWindow(hWndFrom));
  VERIFY(GetWindowRect(hWndFrom, &rcWnd));
  stat_GetMonitorParms2(_mp, &rcWnd);
}
// = TRUE // if FALSE - use all monitor area
// = FALSE
RECT stat_AlignWndRectToMonitor(RECT rcInitial, BOOL bUseWorkArea, BOOL bNoPartialVisibility)
{
  monitor_parms_t _mp;
  RECT rcDesktop;
  stat_GetMonitorParms2(&_mp, &rcInitial);
  rcDesktop = bUseWorkArea ? _mp.m_rcWorkArea : _mp.m_rcMonitor;

  if (rcInitial.left >= rcDesktop.right) {
    OffsetRect(&rcInitial, rcDesktop.right - rcInitial.right, 0);
  }

  if (rcInitial.top >= rcDesktop.bottom) {
    OffsetRect(&rcInitial, 0, rcDesktop.bottom - rcInitial.bottom);
  }

  if (rcInitial.right <= rcDesktop.left) {
    OffsetRect(&rcInitial, rcDesktop.left - rcInitial.left, 0);
  }

  if (rcInitial.bottom <= rcDesktop.top) {
    OffsetRect(&rcInitial, 0, rcDesktop.top - rcInitial.top);
  }

  if (bNoPartialVisibility) {
    if (rcInitial.right > rcDesktop.right) {
      OffsetRect(&rcInitial, rcDesktop.right - rcInitial.right, 0);
    }

    if (rcInitial.bottom > rcDesktop.bottom) {
      OffsetRect(&rcInitial, 0, rcDesktop.bottom - rcInitial.bottom);
    }

    if (rcInitial.left < rcDesktop.left) {
      OffsetRect(&rcInitial, rcDesktop.left - rcInitial.left, 0);
    }

    if (rcInitial.top < rcDesktop.top) {
      OffsetRect(&rcInitial, 0, rcDesktop.top - rcInitial.top);
    }
  } // if( bNoPartialVisibility )

  return rcInitial;
}
COLORREF stat_RGBtoCMY(COLORREF clr)
{
  int nR = GetRV(clr);
  int nG = GetGV(clr);
  int nB = GetBV(clr);
  int nC = 255 - nR;
  int nM = 255 - nG;
  int nY = 255 - nB;
  COLORREF clrCMY = _RGB(nC, nM, nY);
  return clrCMY;
}
COLORREF stat_CMYtoRGB(COLORREF clr)
{
  int nC = GetRV(clr);
  int nM = GetGV(clr);
  int nY = GetBV(clr);
  int nR = 255 - nC;
  int nG = 255 - nM;
  int nB = 255 - nY;
  COLORREF clrRGB = _RGB(nR, nG, nB);
  return clrRGB;
}
// Conversion between the HSL (Hue, Saturation, and Luminosity)
// and RBG color model.
//----------------------------------------------------------------------
// The conversion algorithms presented here come from the book by
// Fundamentals of Interactive Computer Graphics by Foley and van Dam.
// In the example code, HSL values are represented as floating point
// number in the range 0 to 1. RGB tridrants use the Windows convention
// of 0 to 255 of each element.
double stat_HuetoRGB(double m1, double m2, double h)
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
COLORREF stat_HLStoRGB(double H, double L, double S)
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
    r = stat_HuetoRGB(m1, m2, H + 1.0 / 3.0);
    g = stat_HuetoRGB(m1, m2, H);
    b = stat_HuetoRGB(m1, m2, H - 1.0 / 3.0);
  }

  return _RGB((BYTE)(r * 255), (BYTE)(g * 255), (BYTE)(b * 255));
}
int stat_RGBtoHSL(COLORREF rgb, double* H, double* S, double* L)
{
  double delta;
  double r = (double)GetRV(rgb) / 255;
  double g = (double)GetGV(rgb) / 255;
  double b = (double)GetBV(rgb) / 255;
  double cmax = MAX(r, MAX(g, b));
  double cmin = MIN(r, MIN(g, b));
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
BYTE stat_GetRGBFromHue(float rm1, float rm2, float rh)
{
  if (rh > 360.0f) {
    rh -= 360.0f;
  }
  else if (rh < 0.0f) {
    rh += 360.0f;
  }

  if (rh < 60.0f) {
    rm1 = rm1 + (rm2 - rm1) * rh / 60.0f;
  }
  else if (rh < 180.0f) {
    rm1 = rm2;
  }
  else if (rh < 240.0f) {
    rm1 = rm1 + (rm2 - rm1) * (240.0f - rh) / 60.0f;
  }

  return (BYTE)(rm1 * 255);
}
COLORREF stat_GetRGBFromHLSExtend(double H, double L, double S)
{
  WORD R, G, B;

  if (S == 0.0) {
    R = G = B = (unsigned char)(L * 255.0);
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
    R = stat_GetRGBFromHue(rm1, rm2, (float)(H + 120.0f));
    G = stat_GetRGBFromHue(rm1, rm2, (float)(H));
    B = stat_GetRGBFromHue(rm1, rm2, (float)(H - 120.0f));
  }

  return _RGB(R, G, B);
}
COLORREF stat_HLS_Adjust(COLORREF clr, double percentH, // = 0.0
    double percentL, // = 0.0
    double percentS // = 0.0
                        )
{
  double H = 0.0, L = 0.0, S = 0.0;
  stat_RGBtoHSL(clr, &H, &S, &L);

  if (percentH > 0.0) {
    H = H + (1.0 - H) * percentH;
  }
  else if (percentH < 0.0) {
    H = H * (1.0 + percentH);
  }

  if (H < 0.0) {
    H = 0.0;
  }
  else if (H > 1.0) {
    H = 1.0;
  }

  if (percentL > 0.0) {
    L = L + (1.0 - L) * percentL;
  }
  else if (percentL < 0.0) {
    L = L * (1.0 + percentL);
  }

  if (L < 0.0) {
    L = 0.0;
  }
  else if (L > 1.0) {
    L = 1.0;
  }

  if (percentS > 0.0) {
    S = S + (1.0 - S) * percentS;
  }
  else if (percentS < 0.0) {
    S = S * (1.0 + percentS);
  }

  if (S < 0.0) {
    S = 0.0;
  }
  else if (S > 1.0) {
    S = 1.0;
  }

  return stat_HLStoRGB(H, L, S);
}
int stat_GetBPP()
{
  return 32;
}
SIZE iSIZE(int cx, int cy)
{
  SIZE sz;
  sz.w = cx, sz.h = cy;
  return sz;
}
SIZE GetTextSizes(BOOL bBold /*= FALSE*/)
{
  HDC dc = GetMemDC();
  int nTextHeightHorz, nTextHeightVert;
  TEXTMETRIC tm;
  HFONT pOldFont = (HFONT)SelectObject(dc, bBold ? m_FontBold : m_FontNormal);
  ASSERT(pOldFont != NULL);
  GetTextMetrics(dc, &tm);
  nTextHeightHorz = tm.tmHeight + 2;
  SelectObject(dc, m_FontNormal);
  GetTextMetrics(dc, &tm);
  nTextHeightVert = tm.tmHeight + 2;
  SelectObject(dc, pOldFont);
  return iSIZE(nTextHeightHorz, nTextHeightVert);
}
int GetTextHeight(BOOL bHorz)
{
  SIZE _size = GetTextSizes(0);
  return bHorz ? _size.w : _size.h;
}
int PaintGlyph(HDDC dc, int cx, int cy, const glyph_t* glyph, COLORREF* pColorValues)
{
  POINT ptDest = {cx, cy};
  int x, y, x_dest, y_dest;
  const uint* pColorIndex = glyph->m_pColorIndexes;
  RECT rcTestVisiblity = {ptDest.x, ptDest.y, ptDest.x + glyph->m_size.w, ptDest.y = glyph->m_size.h};
  ASSERT(pColorValues != NULL);
  ASSERT(glyph->m_size.w > 0);
  ASSERT(glyph->m_size.h > 0);
  x_dest = ptDest.x + glyph->m_size.w;
  y_dest = ptDest.y + glyph->m_size.h;
  ASSERT(pColorIndex != NULL);

  for (y = ptDest.y; y < y_dest; y++) {
    for (x = ptDest.x; x < x_dest; x++, pColorIndex++) {
      uint nColorIndex = *pColorIndex;
      ASSERT(nColorIndex < glyph->m_nColors);

      if (nColorIndex == 0) {
        continue;
      }

      _SetPixel(dc, x, y, pColorValues[nColorIndex]);
    }
  }
}
int NormalizeRect(RECT* rc)
{
  int t;

  if (rc->left > rc->right) {
    SWAP(rc->left, rc->right, t);
  }

  if (rc->top > rc->bottom) {
    SWAP(rc->top, rc->bottom, t);
  }
}
int PaintGlyphCentered(HDDC dc, const RECT* rect, const glyph_t* glyph, COLORREF* pColorValues)
{
  RECT _rect = (*rect);
  POINT ptDest;
  NormalizeRect(&_rect);

  if (IsRectEmpty(&_rect)) {
    return ;
  }

  ASSERT(glyph->m_size.w > 0);
  ASSERT(glyph->m_size.h > 0);
  ptDest.x = _rect.left + (RCWIDTH(&_rect) - glyph->m_size.w) / 2;
  ptDest.y = _rect.top + (RCHEIGHT(&_rect) - glyph->m_size.h) / 2;
  PaintGlyph(dc, ptDest.x, ptDest.y, glyph, pColorValues);
}
int GetMenuExpandButtonHeight()
{
  int nHeight = GetSystemMetrics(SM_CYMENUCHECK);
  ASSERT(nHeight > 0);
  return nHeight;
}
int GetMenuExpandButtonHeightOffice2003()
{
  int nHeightNormal = GetMenuExpandButtonHeight();
  ASSERT(nHeightNormal > 0);
  return MAX(nHeightNormal, (m_sizeMenuExpBtnCircle.h + 2));
}
// zero terminated string
static int CALLBACK stat_FindFontByNameCallBack(ENUMLOGFONT* pEnumLogFont, NEWTEXTMETRIC* pNewTextMetric, DWORD dwFontType, LPARAM lParam)
{
  __EXT_MFC_SAFE_LPCTSTR sFontToFind = (const TCHAR*)(lParam);
  ASSERT(pEnumLogFont != NULL);
  pNewTextMetric;
  dwFontType;
  ASSERT(sFontToFind != NULL);
  return _tcsicoll(pEnumLogFont->elfLogFont.lfFaceName, sFontToFind);
}
// get any system color based brush
#define GetBrush(nColorIndex) GetColor(nColorIndex)
static HFONT stat_menu_HFONT_from_current_NONCLIENTMETRICS(BOOL bUseTahomaFont)
{
  NONCLIENTMETRICS _ncNfo;
  memset(&_ncNfo, 0, sizeof(NONCLIENTMETRICS));
  _ncNfo.cbSize = sizeof(NONCLIENTMETRICS);

  if (!SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(_ncNfo), &_ncNfo, 0)) {
    return NULL;
  }

  _ncNfo.lfMenuFont.lfCharSet = (BYTE)GetTextCharset(NULL);

  if (bUseTahomaFont) {
    static __EXT_MFC_SAFE_LPCTSTR g_sTahoma = _T("Tahoma");

    if (!EnumFontFamilies(NULL, NULL, (FONTENUMPROC)stat_FindFontByNameCallBack, (LPARAM)((LPCTSTR)(g_sTahoma)))) {
      _tcscpy(_ncNfo.lfMenuFont.lfFaceName, g_sTahoma);
    }
  } // if( bUseTahomaFont )

  return CreateFontIndirect(&_ncNfo.lfMenuFont);
}
COLORREF GetColor(int nColorIndex);
int GetLogFont(HFONT m_hObject, LOGFONT* pLogFont)
{
  ASSERT(m_hObject != NULL);
  return GetObject(m_hObject, sizeof(LOGFONT), pLogFont);
}
// generate DIB for UI light brush (when BPP <= 8 )
int stat_GenLBDIB(COLORREF clrDarker, // = (COLORREF)(-1) // by default (COLORREF)(-1) is changed to _GetSysColor(COLOR_BTNFACE)
    COLORREF clrLighter, // = (COLORREF)(-1) // by default (COLORREF)(-1) is changed to _GetSysColor(COLOR_BTNHIGHLIGHT)
    int nColorMaskHorz, // = 1
    int nColorMaskVert, // = 1
    int nSkipMaskHorz, // = 0
    int nSkipMaskVert, // = 0
    COLORREF clrSkip, // = (COLORREF)(-1L) // // default (COLORREF)(-1) is changed to clrDarker
    Image* bmp
                 )
{
  int nX, nY;
  TDrawDC dc[1];
  COLORREF clr, clrEffectiveDarker, clrEffectiveLighter;
  clrEffectiveDarker = (clrDarker == (COLORREF)(- 1)) ? (_GetSysColor(COLOR_BTNFACE)) : clrDarker;
  clrEffectiveLighter = (clrLighter == (COLORREF)(- 1)) ? (_GetSysColor(COLOR_BTNHIGHLIGHT)) : clrLighter;

  if (clrSkip == (COLORREF)(- 1L)) {
    clrSkip = clrDarker;
  }

  newImage(16, 16, 4, bmp);
  HDDCSET_IMAGE(dc, bmp);

  if (!bmp) {
    ASSERT(FALSE);
    return NULL;
  }

  for (nY = 0; nY < 16; nY++) {
    for (nX = 0; nX < 16; nX++) {
      if ((nX & nSkipMaskHorz) != 0 || (nY & nSkipMaskVert) != 0) {
        _SetPixel(dc, nX, nY, clrSkip);
        continue;
      }

      clr = ((nY & nColorMaskVert) != 0) ? (((nX & nColorMaskHorz) != 0) ? clrEffectiveDarker : clrEffectiveLighter) : (((nX & nColorMaskHorz) != 0) ? clrEffectiveLighter : clrEffectiveDarker);
      _SetPixel(dc, nX, nY, clr);
    }
  }

  return 0;
}
int stat_PaintGradientRect(HDDC dc, const RECT* rcPaintGradient, COLORREF clrLeft, COLORREF clrRight, BOOL bHorz, uint nCountOfSteps)
{
  uint nWidth = RCWIDTH(rcPaintGradient);
  uint nHeight = RCHEIGHT(rcPaintGradient);
  RECT rcItem;
  uint nStep, nBackStep;
  int nBitsPerPixel = stat_GetBPP();

  if (rcPaintGradient->left >= rcPaintGradient->right || rcPaintGradient->top >= rcPaintGradient->bottom) {
    return ;
  }

  ASSERT(nCountOfSteps > 0 && nCountOfSteps <= 256);

  if (bHorz) {
    if (nCountOfSteps > nHeight) {
      nCountOfSteps = nHeight;
    }
  }
  else {
    if (nCountOfSteps > nWidth) {
      nCountOfSteps = nWidth;
    }
  }

  if (nBitsPerPixel <= 8) {
    uint nCalcStepSize = bHorz ? nHeight / nCountOfSteps : nWidth / nCountOfSteps;
    static const uint nLowColorStepMinSize = 4;

    if (nCalcStepSize < nLowColorStepMinSize) {
      nCountOfSteps = bHorz ? nHeight / nLowColorStepMinSize : nWidth / nLowColorStepMinSize;
    }
  } // if( nBitsPerPixel <= 8 )

  rcItem = (*rcPaintGradient);

  for (nStep = 0, nBackStep = nCountOfSteps; nStep < nCountOfSteps; nStep++, nBackStep--) {
    COLORREF clrItem = _RGB((GetRV(clrLeft) * nBackStep + GetRV(clrRight) * nStep) / nCountOfSteps, (GetGV(clrLeft) * nBackStep + GetGV(clrRight) * nStep) / nCountOfSteps, (GetBV(clrLeft) * nBackStep + GetBV(clrRight) * nStep) / nCountOfSteps);

    if (bHorz) {
      rcItem.top = rcPaintGradient->bottom - MulDiv(nStep, nHeight, nCountOfSteps);
      rcItem.top--;

      if (nStep == nCountOfSteps && rcItem.top < rcPaintGradient->top) {
        rcItem.top = rcPaintGradient->top;

        if (rcItem.top >= rcItem.bottom) {
          break;
        }
      }

      ASSERT(RCHEIGHT(&rcItem) >= 1);

      if (nBitsPerPixel <= 8) {
        TFillRect(dc, &rcItem, clrItem);
      } // if( nBitsPerPixel <= 8 )
      else {
        TFillRect(dc, &rcItem, clrItem);
      } // else from if( nBitsPerPixel <= 8 )

      rcItem.bottom = rcItem.top;
    }
    else {
      rcItem.right = MulDiv(nStep, nWidth, nCountOfSteps) + rcPaintGradient->left;
      rcItem.right++;

      if (nStep == nCountOfSteps && rcItem.right > rcPaintGradient->right) {
        rcItem.right = rcPaintGradient->right;

        if (rcItem.right <= rcItem.left) {
          break;
        }
      }

      ASSERT(RCWIDTH(&rcItem) >= 1);

      if (nBitsPerPixel <= 8) {
        TFillRect(dc, &rcItem, clrItem);
      } // if( nBitsPerPixel <= 8 )
      else {
        TFillRect(dc, &rcItem, clrItem);
      } // else from if( nBitsPerPixel <= 8 )

      rcItem.left = rcItem.right;
    }
  }
}
int SyncSysColors()
{
  static __EXT_MFC_SAFE_LPCTSTR sVertFontFaceForNt4 = _T("Arial");
  LOGFONT lf;
  int nBitsPerPixel = stat_GetBPP();
  int i;
  COLORREF clrDefaultExtendColor = _GetSysColor(COLOR_3DFACE);
  // init fonts
  HFONT hDefaultGuiFont = stat_menu_HFONT_from_current_NONCLIENTMETRICS(m_bUseTahomaFont);

  if (hDefaultGuiFont == NULL) {
    hDefaultGuiFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
  }

  if (hDefaultGuiFont == NULL) {
    hDefaultGuiFont = (HFONT)GetStockObject(SYSTEM_FONT);
  }

  ASSERT(hDefaultGuiFont != NULL);

  if (m_FontNormal) {
    DeleteObject(m_FontNormal);
  }

  m_FontNormal = (hDefaultGuiFont);
  VERIFY(GetLogFont(m_FontNormal, &lf));
  lf.lfWeight = 900;

  if (m_FontBold) {
    DeleteObject(m_FontBold);
  }

  VERIFY(m_FontBold = CreateFontIndirect(&lf));
  VERIFY(GetLogFont(m_FontNormal, &lf));
  lf.lfCharSet = SYMBOL_CHARSET;
  lf.lfWeight = 0;
  lf.lfHeight = GetSystemMetrics(SM_CYMENUCHECK) - 1;
  _tcscpy(lf.lfFaceName, _T("Marlett"));

  if (m_FontMarlett) {
    DeleteObject(m_FontMarlett);
  }

  VERIFY(m_FontMarlett = CreateFontIndirect(&lf));
  VERIFY(GetLogFont(m_FontNormal, &lf));
  lf.lfEscapement = __EXT_VERT_FONT_ESCAPEMENT__;

  if (m_FontNormalVert) {
    DeleteObject(m_FontNormalVert);
  }

  if (m_bIsWinNT4 || m_bIsWin9x) {
    _tcscpy(lf.lfFaceName, sVertFontFaceForNt4);
  }

  VERIFY(m_FontNormalVert = CreateFontIndirect(&lf));
  lf.lfEscapement = __EXT_VERT_FONT_ESCAPEMENT_X__;

  if (m_FontNormalVertX) {
    DeleteObject(m_FontNormalVertX);
  }

  if (m_bIsWinNT4 || m_bIsWin9x) {
    _tcscpy(lf.lfFaceName, sVertFontFaceForNt4);
  }

  VERIFY(m_FontNormalVertX = CreateFontIndirect(&lf));
  VERIFY(GetLogFont(m_FontBold, &lf));
  lf.lfEscapement = __EXT_VERT_FONT_ESCAPEMENT__;

  if (m_FontBoldVert) {
    DeleteObject(m_FontBoldVert);
  }

  if (m_bIsWinNT4 || m_bIsWin9x) {
    _tcscpy(lf.lfFaceName, sVertFontFaceForNt4);
  }

  VERIFY(m_FontBoldVert = CreateFontIndirect(&lf));
  lf.lfEscapement = __EXT_VERT_FONT_ESCAPEMENT_X__;

  if (m_FontBoldVertX) {
    DeleteObject(m_FontBoldVertX);
  }

  if (m_bIsWinNT4 || m_bIsWin9x) {
    _tcscpy(lf.lfFaceName, sVertFontFaceForNt4);
  }

  VERIFY(m_FontBoldVertX = CreateFontIndirect(&lf));
  {
    HFONT arrBarCaptFonts[6 * 2] = {
      m_FontNormalBC, m_FontNormal, m_FontNormalVertBC, m_FontNormalVert, m_FontNormalVertXBC, m_FontNormalVertX, m_FontBoldBC, m_FontBold, m_FontBoldVertBC, m_FontBoldVert, m_FontBoldVertXBC, m_FontBoldVertX
    };

    for (i = 0; i < sizeof(arrBarCaptFonts) / sizeof(arrBarCaptFonts[0]); i += 2) {
      HFONT pFontSrc = arrBarCaptFonts[i + 1];
      HFONT pFontDst = arrBarCaptFonts[i];
      ASSERT(pFontSrc != NULL);

      if (pFontDst != NULL) {
        DeleteObject(pFontDst);
      }

      VERIFY(GetLogFont(pFontSrc, &lf));

      if (lf.lfHeight < - 11) {
        lf.lfHeight = - 11;
      }

      VERIFY(pFontDst = CreateFontIndirect(&lf));
    }
  }

  /// init colors
  for (i = 0; i < countof(m_colors); i++) {
    m_colors[i] = (clrDefaultExtendColor);
  }

  //m_colors.clear();
  //m_brushes.clear();
  for (i = __ExtMfc_MIN_SYS_COLOR_VAL; i <= __ExtMfc_MAX_SYS_COLOR_VAL; i++) {
    m_colors[i] = _GetSysColor(i);
  }

  if (nBitsPerPixel > 8) {
    COLORREF clrDarker = GetColor(COLOR_3DFACE);
    COLORREF clrLighter = GetColor(COLOR_3DHILIGHT);
    COLORREF clrMixed = _RGB(GetRV(clrDarker) + ((GetRV(clrLighter) - GetRV(clrDarker)) / 2), GetGV(clrDarker) + ((GetGV(clrLighter) - GetGV(clrDarker)) / 2), GetBV(clrDarker) + ((GetBV(clrLighter) - GetBV(clrDarker)) / 2));
    m_brushLightDefault = (clrMixed);
    clrMixed = _RGB(GetRV(clrDarker) + MulDiv((GetRV(clrLighter) - GetRV(clrDarker)), 3, 5), GetGV(clrDarker) + MulDiv((GetGV(clrLighter) - GetGV(clrDarker)), 3, 5), GetBV(clrDarker) + MulDiv((GetBV(clrLighter) - GetBV(clrDarker)), 3, 5));
    m_brushLighterDefault = (clrMixed);
    clrMixed = _RGB(GetRV(clrDarker) + MulDiv((GetRV(clrLighter) - GetRV(clrDarker)), 4, 5), GetGV(clrDarker) + MulDiv((GetGV(clrLighter) - GetGV(clrDarker)), 4, 5), GetBV(clrDarker) + MulDiv((GetBV(clrLighter) - GetBV(clrDarker)), 4, 5));
    m_brushLightestDefault = (clrMixed);
    clrLighter = clrDarker;
    clrDarker = GetColor(COLOR_3DSHADOW);
    clrMixed = _RGB(GetRV(clrDarker) + ((GetRV(clrLighter) - GetRV(clrDarker)) / 2), GetGV(clrDarker) + ((GetGV(clrLighter) - GetGV(clrDarker)) / 2), GetBV(clrDarker) + ((GetBV(clrLighter) - GetBV(clrDarker)) / 2));
    m_brushDarkDefault = (clrMixed);
    clrMixed = _RGB(GetRV(clrDarker) + MulDiv((GetRV(clrLighter) - GetRV(clrDarker)), 2, 5), GetGV(clrDarker) + MulDiv((GetGV(clrLighter) - GetGV(clrDarker)), 2, 5), GetBV(clrDarker) + MulDiv((GetBV(clrLighter) - GetBV(clrDarker)), 2, 5));
    m_brushDarkerDefault = (clrMixed);
    clrMixed = _RGB(GetRV(clrDarker) + MulDiv((GetRV(clrLighter) - GetRV(clrDarker)), 1, 5), GetGV(clrDarker) + MulDiv((GetGV(clrLighter) - GetGV(clrDarker)), 1, 5), GetBV(clrDarker) + MulDiv((GetBV(clrLighter) - GetBV(clrDarker)), 1, 5));
    m_brushDarkestDefault = (clrMixed);
    clrDarker = _GetSysColor(COLOR_3DFACE);
    clrLighter = _GetSysColor(COLOR_3DHILIGHT);
    clrMixed = _RGB(GetRV(clrDarker) + ((GetRV(clrLighter) - GetRV(clrDarker)) / 2), GetGV(clrDarker) + ((GetGV(clrLighter) - GetGV(clrDarker)) / 2), GetBV(clrDarker) + ((GetBV(clrLighter) - GetBV(clrDarker)) / 2));
    m_brushLightSystem = (clrMixed);
    clrMixed = _RGB(GetRV(clrDarker) + MulDiv((GetRV(clrLighter) - GetRV(clrDarker)), 3, 5), GetGV(clrDarker) + MulDiv((GetGV(clrLighter) - GetGV(clrDarker)), 3, 5), GetBV(clrDarker) + MulDiv((GetBV(clrLighter) - GetBV(clrDarker)), 3, 5));
    m_brushLighterSystem = (clrMixed);
    clrMixed = _RGB(GetRV(clrDarker) + MulDiv((GetRV(clrLighter) - GetRV(clrDarker)), 4, 5), GetGV(clrDarker) + MulDiv((GetGV(clrLighter) - GetGV(clrDarker)), 4, 5), GetBV(clrDarker) + MulDiv((GetBV(clrLighter) - GetBV(clrDarker)), 4, 5));
    m_brushLightestSystem = (clrMixed);
    clrLighter = clrDarker;
    clrDarker = _GetSysColor(COLOR_3DSHADOW);
    clrMixed = _RGB(GetRV(clrDarker) + ((GetRV(clrLighter) - GetRV(clrDarker)) / 2), GetGV(clrDarker) + ((GetGV(clrLighter) - GetGV(clrDarker)) / 2), GetBV(clrDarker) + ((GetBV(clrLighter) - GetBV(clrDarker)) / 2));
    m_brushDarkSystem = (clrMixed);
    clrMixed = _RGB(GetRV(clrDarker) + MulDiv((GetRV(clrLighter) - GetRV(clrDarker)), 2, 5), GetGV(clrDarker) + MulDiv((GetGV(clrLighter) - GetGV(clrDarker)), 2, 5), GetBV(clrDarker) + MulDiv((GetBV(clrLighter) - GetBV(clrDarker)), 2, 5));
    m_brushDarkerSystem = (clrMixed);
    clrMixed = _RGB(GetRV(clrDarker) + MulDiv((GetRV(clrLighter) - GetRV(clrDarker)), 1, 5), GetGV(clrDarker) + MulDiv((GetGV(clrLighter) - GetGV(clrDarker)), 1, 5), GetBV(clrDarker) + MulDiv((GetBV(clrLighter) - GetBV(clrDarker)), 1, 5));
    m_brushDarkestSystem = (clrMixed);
  } // if( nBitsPerPixel > 8 )
}
#define InstallColor(_clr) (_clr)
#define __XP_DSS_MONO DSS_MONO
int InitTranslatedColors()
{
  SyncSysColors();
#define COLORMAPDEF(_a, _b) m_colors[_a] = _GetSysColor(_b)
  COLORMAPDEF(CLR_3DFACE_OUT , COLOR_3DFACE);
  COLORMAPDEF(CLR_3DFACE_IN , COLOR_3DFACE);
  COLORMAPDEF(CLR_3DFACE_DISABLED , COLOR_3DFACE);
  COLORMAPDEF(CLR_3DLIGHT_OUT , COLOR_3DLIGHT);
  COLORMAPDEF(CLR_3DLIGHT_IN , COLOR_3DLIGHT);
  COLORMAPDEF(CLR_3DLIGHT_DISABLED , COLOR_3DLIGHT);
  COLORMAPDEF(CLR_3DHILIGHT_OUT , COLOR_3DHILIGHT);
  COLORMAPDEF(CLR_3DHILIGHT_IN , COLOR_3DHILIGHT);
  COLORMAPDEF(CLR_3DHILIGHT_DISABLED , COLOR_3DHILIGHT);
  COLORMAPDEF(CLR_3DSHADOW_OUT , COLOR_3DSHADOW);
  COLORMAPDEF(CLR_3DSHADOW_IN , COLOR_3DSHADOW);
  COLORMAPDEF(CLR_3DSHADOW_DISABLED , COLOR_3DSHADOW);
  COLORMAPDEF(CLR_3DDKSHADOW_OUT , COLOR_3DDKSHADOW);
  COLORMAPDEF(CLR_3DDKSHADOW_IN , COLOR_3DDKSHADOW);
  COLORMAPDEF(CLR_3DDKSHADOW_DISABLED , COLOR_3DDKSHADOW);
  COLORMAPDEF(CLR_TEXT_OUT , COLOR_BTNTEXT);
  COLORMAPDEF(CLR_TEXT_IN , COLOR_BTNTEXT);
  COLORMAPDEF(CLR_TEXT_DISABLED , COLOR_3DSHADOW);

  if (1 == g_PaintManager) {
    int nBitsPerPixel = stat_GetBPP();
    COLORREF clrSysWindow = _GetSysColor(COLOR_WINDOW);
    COLORREF clrSys3dFace = _GetSysColor(COLOR_3DFACE);
    COLORREF clrSys3dHilight = _GetSysColor(COLOR_3DHILIGHT);
    COLORREF clrSys3dShadow = _GetSysColor(COLOR_3DSHADOW);
    COLORREF clrSys3dDkShadow = _GetSysColor(COLOR_3DDKSHADOW);
    COLORREF clrSysBtnText = _GetSysColor(COLOR_BTNTEXT);
    COLORREF clrSysItemHilight = _GetSysColor(COLOR_HIGHLIGHT);
    COLORREF xpclr_MenuLight = clrSysWindow;
    COLORREF xpclr_ControlBarBk = clrSys3dFace;
    COLORREF xpclr_Highlight = clrSys3dFace;
    COLORREF xpclr_Separator = clrSys3dShadow;
    COLORREF xpclr_PanelBorder = clrSys3dShadow;
    COLORREF xpclr_RarelyUsedMenuLeft = clrSys3dShadow; // clrSys3dFace;
    COLORREF xpclr_HighlightDarked = clrSys3dShadow;
    COLORREF xpclr_HighlightBorder = clrSysBtnText;
    m_DSS_DISABLED_style = __XP_DSS_MONO;

    if (nBitsPerPixel > 8) {
      // xp - menu area light ver (like system COLOR_WINDOW but more brown)
      xpclr_MenuLight = stat_HLS_Adjust(clrSys3dFace, 0.00, 0.70, - 0.50);
      xpclr_MenuLight = _RGB(GetRV(xpclr_MenuLight) + MulDiv(255 - GetRV(xpclr_MenuLight), 40, 100), GetGV(xpclr_MenuLight) + MulDiv(255 - GetGV(xpclr_MenuLight), 20, 100), GetBV(xpclr_MenuLight));
      xpclr_MenuLight = stat_HLS_Adjust(xpclr_MenuLight, 0.00, 0.06, 0.05);
      // xp - rarely used item left side
      xpclr_RarelyUsedMenuLeft = _RGB((999L * (long)(GetRV(clrSys3dFace))) / 1000L, (995L * (long)(GetGV(clrSys3dFace))) / 1000L, (995L * (long)(GetBV(clrSys3dFace))) / 1000L);
      // xp - control bar funny background
      xpclr_ControlBarBk = stat_HLS_Adjust(xpclr_ControlBarBk, 0.00, 0.20, 0.00);
      // xp - blue like light ver
      xpclr_Highlight = stat_HLS_Adjust(clrSysItemHilight, 0.00, 0.75, - 0.75);
      // xp - blue like dark ver
      xpclr_HighlightDarked = stat_HLS_Adjust(xpclr_Highlight, 0.00, - 0.25, 0.10);
      // xp - blue dark selected border
      xpclr_HighlightBorder = stat_HLS_Adjust(xpclr_HighlightDarked, 0.00, - 0.50, 0.00);
      // xp - dark gray separator
      xpclr_Separator = _RGB((857L * (long)(GetRV(clrSys3dFace))) / 1000L, (857L * (long)(GetGV(clrSys3dFace))) / 1000L, (857L * (long)(GetBV(clrSys3dFace))) / 1000L);
      // xp - dark panel border (for floating controlbars)
      xpclr_PanelBorder = _RGB((750L * (long)(GetRV(clrSys3dShadow)) + 250L * (long)(GetRV(clrSys3dDkShadow))) / 1000L, (750L * (long)(GetGV(clrSys3dShadow)) + 250L * (long)(GetGV(clrSys3dDkShadow))) / 1000L, (750L * (long)(GetBV(clrSys3dShadow)) + 250L * (long)(GetBV(clrSys3dDkShadow))) / 1000L);
    } // if( nBitsPerPixel > 8 )

#define COLORMAPDEF2(_a, _b) m_colors[_a] = (_b)
    //idxClrDark = InstallColor(xpclr_PanelBorder);
    COLORMAPDEF2(CLR_3DFACE_OUT , InstallColor(xpclr_ControlBarBk));
    COLORMAPDEF2(CLR_3DFACE_IN , InstallColor(xpclr_Highlight));
    COLORMAPDEF(CLR_3DFACE_DISABLED , COLOR_3DFACE);
    COLORMAPDEF2(CLR_3DLIGHT_OUT , xpclr_PanelBorder);
    COLORMAPDEF2(CLR_3DLIGHT_IN , xpclr_PanelBorder);
    COLORMAPDEF2(CLR_3DLIGHT_DISABLED , xpclr_PanelBorder);
    COLORMAPDEF2(CLR_3DHILIGHT_OUT , xpclr_PanelBorder);
    COLORMAPDEF2(CLR_3DHILIGHT_IN , xpclr_PanelBorder);
    COLORMAPDEF2(CLR_3DHILIGHT_DISABLED , xpclr_PanelBorder);
    COLORMAPDEF2(CLR_3DSHADOW_OUT , xpclr_PanelBorder);
    COLORMAPDEF2(CLR_3DSHADOW_IN , xpclr_PanelBorder);
    COLORMAPDEF2(CLR_3DSHADOW_DISABLED , xpclr_PanelBorder);
    COLORMAPDEF2(CLR_3DDKSHADOW_OUT , xpclr_PanelBorder);
    COLORMAPDEF2(CLR_3DDKSHADOW_IN , xpclr_PanelBorder);
    COLORMAPDEF2(CLR_3DDKSHADOW_DISABLED , xpclr_PanelBorder);
    COLORMAPDEF(CLR_TEXT_OUT , COLOR_BTNTEXT);
    COLORMAPDEF2(CLR_TEXT_IN , InstallColor((nBitsPerPixel > 8) ? _RGB(0, 0, 0) : clrSys3dHilight));
    COLORMAPDEF(CLR_TEXT_DISABLED , COLOR_3DSHADOW);
    COLORMAPDEF(XPCLR_PUSHEDHOVERTEXT , COLOR_HIGHLIGHTTEXT);
    COLORMAPDEF2(XPCLR_3DFACE_DARK , InstallColor(xpclr_ControlBarBk));
    COLORMAPDEF2(XPCLR_3DFACE_NORMAL , InstallColor(xpclr_MenuLight));
    COLORMAPDEF2(XPCLR_SEPARATOR , InstallColor(xpclr_Separator));
    COLORMAPDEF2(XPCLR_HILIGHT , InstallColor(xpclr_HighlightDarked));
    COLORMAPDEF2(XPCLR_HILIGHT_BORDER , InstallColor(xpclr_HighlightBorder));
    COLORMAPDEF2(XPCLR_RARELY_BORDER , InstallColor(xpclr_RarelyUsedMenuLeft));
    COLORMAPDEF(XPCLR_MENU_BORDER , COLOR_3DDKSHADOW);
    COLORMAPDEF2(XPCLR_3DFACE_FLOAT_R , InstallColor(xpclr_MenuLight));
    //m_mapColorTranslate[XPCLR_3DFACE_FLOAT_F , InstallColor(GetColor(CLR_3DFACE_OUT)));
    COLORMAPDEF(XPCLR_3DFACE_FLOAT_F , InstallColor(COLOR_3DFACE));
  }

  return 0;
}
#undef COLORMAPDEF
#undef COLORMAPDEF2
COLORREF GetColor(int nColorIndex)
{
  static int inited = 0;

  if (0 == inited) {
    inited = 1;
    InitTranslatedColors();
  }

  return m_colors[nColorIndex];
}
// for painting system menu glyphs
int InitPAINTMENUITEMDATA(PAINTMENUITEMDATA* m, int* pHelperSrc, const RECT* rcItem, INT nIconAreaWidth, __EXT_MFC_SAFE_LPCTSTR sText, __EXT_MFC_SAFE_LPCTSTR sAccelText, Image* pIcon, BOOL bPopup, BOOL bSelected, BOOL bPushedCheck, BOOL bPushedRadio, BOOL bIndeterminate, BOOL bEnabled, BOOL bRarelyUsed, BOOL bRarelyUsedPrev, BOOL bRarelyUsedNext, uint nHelperSysCmdID)
{
  m->m_pHelperSrc = (pHelperSrc), m->m_lParam = (0L), m->m_rcItem = (*rcItem), m->m_nIconAreaWidth = (nIconAreaWidth);
  m->m_sText = ((sText == NULL) ? _T("") : sText), m->m_sAccelText = ((sAccelText == NULL) ? _T("") : sAccelText);
  m->m_pIcon = (pIcon), m->m_bPopup = (bPopup), m->m_bSelected = (bSelected), m->m_bPushedCheck = (bPushedCheck), m->m_bPushedRadio = (bPushedRadio);
  m->m_bIndeterminate = (bIndeterminate), m->m_bEnabled = (bEnabled), m->m_bRarelyUsed = (bRarelyUsed), m->m_bRarelyUsedPrev = (bRarelyUsedPrev);
  m->m_bRarelyUsedNext = (bRarelyUsedNext), m->m_nHelperSysCmdID = (nHelperSysCmdID), m->m_bForceNoLeftGradient = (FALSE), m->m_bCustomizeListMode = (FALSE);
  m->m_bSeparatedDropDown = (FALSE), SetRect(&m->m_rcInPlaceWnd, 0, 0, 0, 0), m->m_strInPlaceText = (_T(""));
  m->m_bXtraMark = (FALSE), m->m_bXtraChecked = (FALSE), m->m_bHelperPopupDropped = (FALSE);
  return ;
}
#define DeflateRect(_RC, _X, _Y) InflateRect(_RC, -(_X), -(_Y))
int DeflateRect4(RECT* rc, int l, int t, int r, int b)
{
  rc->left += l;
  rc->top += t;
  rc->right -= r;
  rc->bottom -= b;
}
#define GetPushedOffset() iSIZE(1, 1)
int PaintSysCmdGlyph(HDDC dc, uint nHelperSysCmdID, const RECT* rcItem, BOOL bHover, BOOL bPushed, BOOL bEnabled)
{
  e_docking_caption_button_t eGlyph = __DCBT_EMPTY;
  RECT _rcItem = (*rcItem);

  switch (nHelperSysCmdID) {
  case SC_CLOSE:
    eGlyph = __DCBT_CLOSE;
    break;

  case SC_RESTORE:
    eGlyph = __DCBT_WND_RESTORE;
    break;

  case SC_MINIMIZE:
    eGlyph = __DCBT_WND_MINIMIZE;
    break;

  case SC_MAXIMIZE:
    eGlyph = __DCBT_WND_MAXIMIZE;
    break;

  case SC_CONTEXTHELP:
    eGlyph = __DCBT_WND_CONTEXTHELP;
    break;
  } // switch( nHelperSysCmdID )

  if (eGlyph == __DCBT_EMPTY) {
    return ;
  }

  {
    COLORREF ColorValues[2] = {
      _RGB(0, 0, 0), _RGB(0, 0, 0),
    };

    if (bPushed) {
      SIZE sz = GetPushedOffset();
      OffsetRect(&_rcItem, sz.w, sz.h);
    }

    if (bEnabled) {
      ColorValues[1] = GetColor(bHover ? COLOR_HIGHLIGHTTEXT : CLR_TEXT_OUT);
    } // if( bEnabled )
    else {
      ColorValues[1] = GetColor(CLR_3DHILIGHT_OUT);
      OffsetRect(&_rcItem, 1, 1);
      PaintGlyphCentered(dc, &_rcItem, g_DockingCaptionGlyphs[eGlyph], ColorValues);
      OffsetRect(&_rcItem, - 1, - 1);
      ColorValues[1] = GetColor(CLR_3DSHADOW_OUT);
    } // if( bEnabled )

    PaintGlyphCentered(dc, &_rcItem, g_DockingCaptionGlyphs[eGlyph], ColorValues);
  }
}
int InitPAINTCOMBOFRAMEDATA(PAINTCOMBOFRAMEDATA* m, CObject* pHelperSrc, const RECT* rcClient, BOOL bHover, BOOL bPushed, BOOL bEnabled)
{
  m->m_pHelperSrc = (pHelperSrc), m->m_lParam = (0L), m->m_rcClient = (*rcClient), m->m_bHover = (bHover);
  m->m_bPushed = (bPushed), m->m_bEnabled = (bEnabled), m->m_clrForceNABorder = ((COLORREF)(- 1L));
  m->m_clrForceNAContent = ((COLORREF)(- 1L)), m->m_clrForceActiveOuterBorder = ((COLORREF)(- 1L)), m->m_bNoTopOuterBorder = (FALSE);
}
int InitPAINTPUSHBUTTONDATA(PAINTPUSHBUTTONDATA* m, CObject* pHelperSrc, BOOL bHorz, const RECT* rcClient, __EXT_MFC_SAFE_LPCTSTR sText, Image* pIcon, BOOL bFlat, BOOL bHover, BOOL bPushed, BOOL bIndeterminate, BOOL bEnabled, BOOL bDrawBorder, BOOL bDrawFocusRect, BOOL bDefaultPushButton, int eAlign, HFONT hFont, // = NULL
    BOOL bDropDown, // = FALSE
    uint nHelperSysCmdID, // = 0
    BOOL bTransparentBackground // = FALSE
                           )
{
  m->m_pHelperSrc = (pHelperSrc), m->m_lParam = (0L), m->m_bHorz = (bHorz), m->m_rcClient = (*rcClient);
  m->m_sText = ((sText == NULL) ? _T("") : sText), m->m_pIcon = (pIcon), m->m_bFlat = (bFlat);
  m->m_bHover = (bHover), m->m_bPushed = (bPushed), m->m_bIndeterminate = (bIndeterminate);
  m->m_bEnabled = (bEnabled), m->m_bDrawBorder = (bDrawBorder), m->m_bDrawFocusRect = (bDrawFocusRect);
  m->m_bDefaultPushButton = (bDefaultPushButton), m->m_eAlign = (eAlign), m->m_hFont = (hFont);
  m->m_bDropDown = (bDropDown), m->m_nHelperSysCmdID = (nHelperSysCmdID), m->m_bTransparentBackground = (bTransparentBackground);
  m->m_bSeparatedDropDown = (FALSE), m->m_bPushedDropDown = (FALSE), m->m_clrForceTextDisabled = (((COLORREF) - 1L));
  m->m_clrForceTextNormal = (((COLORREF) - 1L)), m->m_clrForceTextHover = (((COLORREF) - 1L)), m->m_clrForceTextPressed = (((COLORREF) - 1L));
  m->m_nDrawTextFlagsH = (DT_SINGLELINE | DT_CENTER | DT_VCENTER), m->m_bForceUseDrawTextApiH = (FALSE);
}
int PaintIcon(HDDC dc, BOOL bHorz, const TCHAR* sTitle, const Image* pIcon, RECT rectItem, RECT rectCaption, BOOL bPushed, BOOL bEnabled, BOOL bHover, int eAlign)
{
  SIZE _sizeIcon;
  RECT btnRect, iconRect;
  bHover;
  rectCaption = rectItem;

  if (pIcon == NULL) {
    return ;
  }

  _sizeIcon = iSIZE(pIcon->w, pIcon->h);
  ASSERT(_sizeIcon.w > 0 && _sizeIcon.h > 0);
  iconRect = rectItem;

  if (bHorz) {
    if ((eAlign & __ALIGN_HORIZ_CENTER) != 0) {
      if (sTitle)
        // Center the icon horizontally
      {
        iconRect.left += ((RCWIDTH(&iconRect) - _sizeIcon.w) / 2);
      }
      else {
        iconRect.left += 3;
        rectCaption.left += _sizeIcon.w + 3;
      }
    } // if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
    else {
      if ((eAlign & __ALIGN_HORIZ_RIGHT) != 0) {
        btnRect = rectCaption;

        if (sTitle)
          // Center the icon horizontally
        {
          iconRect.left += ((RCWIDTH(&iconRect) - _sizeIcon.w) / 2);
        }
        else {
          rectCaption.right = RCWIDTH(&rectCaption) - _sizeIcon.w - 3;
          rectCaption.left = 3;
          iconRect.left = btnRect.right - _sizeIcon.w - 3;
          // Center the icon vertically
          iconRect.top += ((RCHEIGHT(&iconRect) - _sizeIcon.h) / 2);
        }
      } // if( (eAlign&__ALIGN_HORIZ_RIGHT) != 0 )
    } // else from if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )

    if ((eAlign & __ALIGN_VERT) != 0) {
      // Center the icon horizontally
      iconRect.top += ((RCHEIGHT(&iconRect) - _sizeIcon.h) / 2);
    } // if( (eAlign&__ALIGN_VERT) != 0 )
  } // if( bHorz )
  else {
    if ((eAlign & __ALIGN_HORIZ_CENTER) != 0) {
      if (sTitle)
        // Center the icon horizontally
      {
        iconRect.top += ((RCHEIGHT(&iconRect) - _sizeIcon.h) / 2);
      }
      else {
        iconRect.top += 3;
        rectCaption.top += _sizeIcon.h + 3;
      }
    } // if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
    else {
      if ((eAlign & __ALIGN_HORIZ_RIGHT) != 0) {
        btnRect = rectCaption;

        if (sTitle)
          // Center the icon horizontally
        {
          iconRect.top += ((RCHEIGHT(&iconRect) - _sizeIcon.h) / 2);
        }
        else {
          rectCaption.top = RCHEIGHT(&rectCaption) - _sizeIcon.h - 3;
          rectCaption.top = 3;
          iconRect.top = btnRect.top - _sizeIcon.h - 3;
          // Center the icon vertically
          iconRect.left += ((RCWIDTH(&iconRect) - _sizeIcon.w) / 2);
        }
      } // if( (eAlign&__ALIGN_HORIZ_RIGHT) != 0 )
    } // else from if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )

    if ((eAlign & __ALIGN_VERT) != 0) {
      // Center the icon horizontally
      iconRect.left += ((RCWIDTH(&iconRect) - _sizeIcon.w) / 2);
    } // if( (eAlign&__ALIGN_VERT) != 0 )
  } // else from if( bHorz )

  // If button is pressed then press the icon also
  if (bPushed) {
    SIZE sizePushedOffset = GetPushedOffset();
    OffsetRect(&iconRect, sizePushedOffset.w, sizePushedOffset.h);
  }

  //pIcon->Paint(dc, iconRect.TopLeft(), bEnabled ? Image::__DRAW_NORMAL: Image::__DRAW_DISABLED);
  TDrawImage(dc, iconRect.left, iconRect.top, pIcon->w, pIcon->h, pIcon, 0, 0);
}
POINT iPOINT(int x, int y)
{
  POINT pt;
  pt.x = x, pt.y = y;
  return pt;
}
RECT stat_CalcTextDimension(HFONT font, const TCHAR* strText)
{
  HDC dc = GetMemDC();
  RECT rcText = {0, 0, 0, 0};
  ASSERT(dc != NULL);
  ASSERT(font != NULL);
  DrawText(dc, strText, strlen(strText), &rcText, DT_SINGLELINE | DT_LEFT | DT_CALCRECT);
  return rcText;
}
int stat_CalcTextWidth(HFONT font, const TCHAR* strText)
{
  RECT rc = stat_CalcTextDimension(font, strText);
  return RCWIDTH(&rc);
}
int PaintPushButton(HDDC dc, PAINTPUSHBUTTONDATA* _ppbd)
{
  COLORREF bgColor, textColor;
  const TCHAR* sTitle;
  BOOL bPushedDropDown;
  RECT rectClient, rectTotal, rectCaption, rectDropDown;
  MOVEDEF;

  if (IsRectEmpty(&_ppbd->m_rcClient)) {
    return 0;
  }

  if (!_ppbd->m_bEnabled) {
    _ppbd->m_bHover = FALSE;
    //_ppbd->m_bDrawBorder = FALSE;
    _ppbd->m_bDrawFocusRect = FALSE;
    _ppbd->m_bIndeterminate = FALSE;
  }

  if (_ppbd->m_bSeparatedDropDown && (_ppbd->m_bPushed || _ppbd->m_bPushedDropDown)) {
    _ppbd->m_bHover = FALSE;
  }

  if (_ppbd->m_bHover || _ppbd->m_bPushed) {
    bgColor = GetColor(CLR_3DFACE_IN);
  }
  else {
    bgColor = GetColor(CLR_3DFACE_OUT);
  }

  sTitle = (_T(""));

  if (_ppbd->m_sText != NULL) {
    sTitle = _ppbd->m_sText;
  }

  bPushedDropDown = FALSE;

  if ((_ppbd->m_bPushed && !_ppbd->m_bSeparatedDropDown) || _ppbd->m_bPushedDropDown) {
    bPushedDropDown = TRUE;
  }

  rectClient = (_ppbd->m_rcClient);
  rectTotal = (_ppbd->m_rcClient);
  rectCaption = (_ppbd->m_rcClient);
  SetRect(&rectDropDown, 0, 0, 0, 0);

  if (_ppbd->m_bDropDown) {
    rectDropDown = rectClient;

    if (_ppbd->m_bHorz) {
      rectCaption.right -= __DROPDOWN_ARROW_GAP * 2 + g_glyph_btn_expand_bottom.m_size.w;
      rectDropDown.left = rectCaption.right;
    } // if( _ppbd->m_bHorz )
    else {
      rectCaption.bottom -= __DROPDOWN_ARROW_GAP * 2 + g_glyph_btn_expand_right.m_size.h;
      rectDropDown.top = rectCaption.bottom;
    } // if( _ppbd->m_bHorz )

    if (!_ppbd->m_bTransparentBackground) {
      if (_ppbd->m_bEnabled && bPushedDropDown // && _ppbd->m_bPushedDropDown
          && _ppbd->m_bHover
          //&& _ppbd->m_bFlat
         ) {
        TFillRect(dc, &rectDropDown, m_brushLightDefault);
      }
      else {
        TFillRect(dc, &rectDropDown, bgColor);
      }
    } // if( !_ppbd->m_bTransparentBackground )

    if (_ppbd->m_bSeparatedDropDown) {
      rectClient = rectCaption;

      if (bPushedDropDown) {
        if (_ppbd->m_bFlat) {
          if (_ppbd->m_bDrawBorder) {
            TDraw3DRect(dc, &rectDropDown, GetColor(CLR_3DSHADOW_IN), GetColor(CLR_3DHILIGHT_IN));
          }
        } // if( _ppbd->m_bFlat )
        else {
          TDraw3DRect(dc, &rectDropDown, GetColor(CLR_3DSHADOW_IN), GetColor(CLR_3DHILIGHT_IN));
        }
      } // if( bPushedDropDown )
      else {
        if (_ppbd->m_bFlat) {
          if (_ppbd->m_bHover && _ppbd->m_bDrawBorder) {
            TDraw3DRect(dc, &rectDropDown, GetColor(CLR_3DHILIGHT_OUT), GetColor(CLR_3DSHADOW_OUT));
          }
        } // if( _ppbd->m_bFlat )
        else {
          TDraw3DRect(dc, &rectDropDown, GetColor(CLR_3DHILIGHT_OUT), GetColor(CLR_3DSHADOW_OUT));
        }
      } // else from if( bPushedDropDown )
    } // if( _ppbd->m_bSeparatedDropDown )
  } // if( _ppbd->m_bDropDown )

  if (!_ppbd->m_bTransparentBackground) {
    if (_ppbd->m_bEnabled && _ppbd->m_bPushed && _ppbd->m_bHover
        //&& _ppbd->m_bFlat
       ) {
      TFillRect(dc, &rectClient, m_brushLightDefault);
    }
    else {
      TFillRect(dc, &rectClient, bgColor);
    }
  } // if( !_ppbd->m_bTransparentBackground )

  // Draw pressed button
  if (_ppbd->m_bPushed) {
    if (_ppbd->m_bFlat) {
      if (_ppbd->m_bDrawBorder) {
        TDraw3DRect(dc, &rectClient, GetColor(CLR_3DSHADOW_IN), GetColor(CLR_3DHILIGHT_IN));
      }
    }
    else {
      TDraw3DRect(dc, &rectClient, GetColor(CLR_3DSHADOW_IN), GetColor(CLR_3DHILIGHT_IN));
    }
  } // if( bPushed )
  else {
    // ...else draw non pressed button
    if (_ppbd->m_bFlat) {
      if (_ppbd->m_bHover && _ppbd->m_bDrawBorder) {
        TDraw3DRect(dc, &rectClient, GetColor(CLR_3DHILIGHT_OUT), GetColor(CLR_3DSHADOW_OUT));
      }
    } // if( _ppbd->m_bFlat )
    else {
      TDraw3DRect(dc, &rectClient, GetColor(CLR_3DHILIGHT_OUT), GetColor(CLR_3DSHADOW_OUT));

      if (_ppbd->m_bHover) {
        RECT cc = (rectTotal);
        TDraw3DRect(dc, &cc, GetColor(CLR_3DSHADOW_OUT), GetColor(CLR_3DDKSHADOW_OUT));
        DeflateRect(&cc, 1, 1);
        TDraw3DRect(dc, &cc, GetColor(CLR_3DHILIGHT_OUT), GetColor(CLR_3DSHADOW_OUT));
      } // if( _ppbd->m_bHover )
    } // else from if( _ppbd->m_bFlat )
  } // else from if( _ppbd->m_bPushed )

  if (_ppbd->m_bDropDown) {
    RECT rectGlyph = (rectDropDown);
    COLORREF ColorValues[2] = {
      _RGB(0, 0, 0), GetColor(_ppbd->m_bEnabled ? ((_ppbd->m_clrForceTextNormal == ((COLORREF) - 1L)) ? CLR_TEXT_OUT : _ppbd->m_clrForceTextNormal) : ((_ppbd->m_clrForceTextDisabled == ((COLORREF) - 1L)) ? CLR_3DSHADOW_OUT : _ppbd->m_clrForceTextDisabled))
      };

    if (bPushedDropDown) {
      SIZE sz = GetPushedOffset();
      OffsetRect(&rectGlyph, sz.w, sz.h);
    }

    PaintGlyphCentered(dc, &rectGlyph, _ppbd->m_bHorz ? &g_glyph_btn_expand_bottom : &g_glyph_btn_expand_right, ColorValues);
  } // if( _ppbd->m_bDropDown )

  rectClient = rectCaption;

  // Draw the icon
  if (rectClient.bottom > rectClient.top && rectClient.right > rectClient.left) {
    if (_ppbd->m_pIcon != NULL) {
      PaintIcon(dc, _ppbd->m_bHorz, sTitle, _ppbd->m_pIcon, rectClient, rectCaption, _ppbd->m_bPushed, _ppbd->m_bEnabled, _ppbd->m_bHover, _ppbd->m_eAlign);
    } // if( _ppbd->m_pIcon != NULL && (! _ppbd->m_pIcon->IsEmpty()) )
    else {
      PaintSysCmdGlyph(dc, _ppbd->m_nHelperSysCmdID, &rectClient, FALSE, _ppbd->m_bPushed, _ppbd->m_bEnabled);
    } // else from if( _ppbd->m_pIcon != NULL && (! _ppbd->m_pIcon->IsEmpty()) )
  }

  if (rectCaption.bottom > rectCaption.top && rectCaption.right > rectCaption.left) {
    HFONT pOldBtnFont = NULL;
    HFONT pCurrFont = NULL;
    HFONT fontDummy;
    int nTextLength, nAmpIndex;
    TCHAR sBtn[256];
    RECT rcText, rcCenteredCaption;

    // if we have valid area for text & focus rect
    if (!sTitle) {
      if (_ppbd->m_bPushed) {
        SIZE sizePushedOffset = GetPushedOffset();
        OffsetRect(&rectCaption, sizePushedOffset.w, sizePushedOffset.h);
      }

      //dc.SetBkColor(bgColor);
      if (_ppbd->m_bEnabled) {
        if (_ppbd->m_bHover || _ppbd->m_bPushed) {
          if (_ppbd->m_bPushed && _ppbd->m_clrForceTextPressed != ((COLORREF) - 1L)) {
            textColor = (GetColor(_ppbd->m_clrForceTextPressed));
          }
          else if (_ppbd->m_bHover && _ppbd->m_clrForceTextHover != ((COLORREF) - 1L)) {
            textColor = (GetColor(_ppbd->m_clrForceTextHover));
          }
          else {
            textColor = (GetColor(COLOR_3DDKSHADOW));
          }
        } // if( _ppbd->m_bHover || _ppbd->m_bPushed )
        else {
          (_ppbd->m_clrForceTextNormal == ((COLORREF) - 1L)) ? GetColor(COLOR_BTNTEXT) : _ppbd->m_clrForceTextNormal;
        }
      } // if( _ppbd->m_bEnabled )
      else {
        textColor = ((_ppbd->m_clrForceTextDisabled == ((COLORREF) - 1L)) ? GetColor(COLOR_3DFACE) : _ppbd->m_clrForceTextDisabled);
      } // else from if( _ppbd->m_bEnabled )

      /*
      // center text
      RECT centerRect = rectCaption;
      */
      if (_ppbd->m_hFont != NULL) {
        if (_ppbd->m_bHorz) {
          pCurrFont = (_ppbd->m_hFont);
        }
        else {
          LOGFONT lf;
          memset(&lf, 0, sizeof(LOGFONT));
          GetObject(_ppbd->m_hFont, sizeof(LOGFONT), &lf);

          if (lf.lfEscapement == 0) {
            lf.lfEscapement = __EXT_VERT_FONT_ESCAPEMENT__;
            VERIFY(fontDummy = CreateFontIndirect(&lf));
            pCurrFont = fontDummy;
          }
          else {
            // suppose font already prepared
            pCurrFont = (_ppbd->m_hFont);
          }
        }
      } // if( _ppbd->m_hFont != NULL )
      else {
        if (_ppbd->m_bHorz) {
          if (_ppbd->m_bDefaultPushButton) {
            pCurrFont = m_FontBold;
          }
          else {
            pCurrFont = m_FontNormal;
          }
        }
        else {
          if (_ppbd->m_bDefaultPushButton) {
            pCurrFont = m_FontBoldVert;
          }
          else {
            pCurrFont = m_FontNormalVert;
          }
        }
      } // else from if( _ppbd->m_hFont != NULL )

      ASSERT(pCurrFont != NULL);
      TSelectFont(pCurrFont);
      ASSERT(pOldBtnFont != NULL);
      nTextLength = strlen(sTitle);
      nAmpIndex = strchr(sTitle, _T('&')) - sTitle;

      if (nAmpIndex < 0) {
        strcpy(sBtn, sTitle);
      }
      else {
        memcpy(sBtn, sTitle, nAmpIndex);
        strcpy(sBtn + nAmpIndex, sTitle + 1);
        //sBtn = sTitle.Left(nAmpIndex) + sTitle.Right(nTextLength - (nAmpIndex + 1));
      }

      ASSERT(strchr(sBtn, _T('&')) == 0);
      SetRect(&rcText, 0, 0, 0, 0);
      TDrawText(dc, sBtn, &rcText, DT_SINGLELINE | DT_CALCRECT);
      rcCenteredCaption = (rectCaption);
      OffsetRect(&rcCenteredCaption, ((_ppbd->m_nDrawTextFlagsH & DT_CENTER) != 0) ? ((RCWIDTH(&rectCaption) - RCWIDTH(&rcText)) / 2) : 0, ((_ppbd->m_nDrawTextFlagsH & DT_VCENTER) != 0) ? ((RCHEIGHT(&rectCaption) - RCHEIGHT(&rcText)) / 2) : 0);

      if (_ppbd->m_bHorz) {
        if (_ppbd->m_bForceUseDrawTextApiH) {
          TDrawText(dc, sTitle, &rectCaption, _ppbd->m_nDrawTextFlagsH);
        } // if( _ppbd->m_bForceUseDrawTextApiH )
        else {
          //dc.DrawState(rcCenteredCaption.TopLeft(), rcCenteredCaption.Size(), sTitle, (_ppbd->m_bEnabled || (_ppbd->m_clrForceTextDisabled != ((COLORREF) - 1L))) ? DSS_NORMAL : m_DSS_DISABLED_style, TRUE, 0, (COLORREF*)NULL);
        } // else from if( _ppbd->m_bForceUseDrawTextApiH )
      } // if( _ppbd->m_bHorz )
      else {
        int _cyHorzFont, _cyTextMargin, nXOffsPlus;
        LOGFONT lf;
        POINT ptLineFrom = {0, 0}, ptLineTo = {0, 0};
        RECT rc, rcString;
        SIZE ptFix;
        memset(&lf, 0, sizeof(LOGFONT));
        GetLogFont(pCurrFont, &lf);
        _cyHorzFont = abs(lf.lfHeight);
        _cyTextMargin = (RCWIDTH(&rectCaption) - _cyHorzFont) / 2;

        if (nAmpIndex >= 0) {
          char buf[256] = {0};
          int textWidth;
          memcpy(buf, sBtn, nAmpIndex);
          textWidth = stat_CalcTextWidth(_ppbd->m_bDefaultPushButton ? m_FontBold : m_FontNormal, buf);
          ptLineFrom = iPOINT(__ExtMfc_CXTEXTMARGIN + _cyTextMargin, __ExtMfc_CYTEXTMARGIN + textWidth);
          memcpy(buf, sBtn, nAmpIndex + 1);
          textWidth = stat_CalcTextWidth(_ppbd->m_bDefaultPushButton ? m_FontBold : m_FontNormal, buf);
          ptLineTo = iPOINT(__ExtMfc_CXTEXTMARGIN + _cyTextMargin, __ExtMfc_CYTEXTMARGIN + textWidth);
          rc = stat_CalcTextDimension(_ppbd->m_bDefaultPushButton ? m_FontBold : m_FontNormal, sBtn);
          nXOffsPlus = RCWIDTH(&rectCaption) - RCHEIGHT(&rc);
          nXOffsPlus /= 2;
          nXOffsPlus -= 4;
          ptLineFrom.x += nXOffsPlus;
          ptLineTo.x += nXOffsPlus;
        } // if( nAmpIndex >= 0 )

        SetRect(&rcString, rectCaption.right - _cyTextMargin + __ExtMfc_CXTEXTMARGIN, rectCaption.top + __ExtMfc_CYTEXTMARGIN, 0, 0);
        rcString.right = rcString.left + RCWIDTH(&rectCaption);
        rcString.bottom = rcString.top + RCHEIGHT(&rectCaption);
        DeflateRect(&rcString, 2, 2);
#define PTOFFSET(PT, _X, _Y) ((PT).x+=(_X),(PT).y+=(_Y))
#define PTOFFSETPT(PT, PT2) PTOFFSET(PT, (PT2).x, (PT2).y)
        ptFix = iSIZE(-RCWIDTH(&rcString) - 1, 0);
        PTOFFSET(ptLineFrom, rcString.left + ptFix.w, rcString.top + ptFix.h);
        PTOFFSET(ptLineTo, rcString.left + ptFix.w, rcString.top + ptFix.h);

        if (_ppbd->m_bEnabled || (_ppbd->m_clrForceTextDisabled != ((COLORREF) - 1L))) {
          TDrawText(dc, sBtn, &rcString, DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX);

          if (nAmpIndex >= 0) {
            COLORREF pen = _RGB(0, 0, 0);
            TMoveTo(dc, ptLineFrom.x, ptLineFrom.y);
            TLineTo(dc, ptLineTo.x, ptLineTo.y, pen);
          } // if( nAmpIndex >= 0 )
        } // if( _ppbd->m_bEnabled || ( _ppbd->m_clrForceTextDisabled != ((COLORREF)-1L) ) )
        else {
          POINT ptOffsetPre = {1, 1};
          POINT ptOffsetPost = { - 1, - 1};
          COLORREF clrText = GetColor(COLOR_3DSHADOW);
          COLORREF clrShadow = GetColor(COLOR_3DHILIGHT);
          OffsetRect(&rcString, ptOffsetPre.x, ptOffsetPre.y);
          textColor = (clrShadow);
          TDrawText(dc, sBtn, &rcString, DT_SINGLELINE //|DT_CENTER|DT_VCENTER
              | DT_NOCLIP | DT_NOPREFIX); // don't forget DT_NOCLIP
          OffsetRect(&rcString, ptOffsetPost.x, ptOffsetPost.y);
          textColor = (clrText);
          TDrawText(dc, sBtn, &rcString, DT_SINGLELINE //|DT_CENTER|DT_VCENTER
              | DT_NOCLIP | DT_NOPREFIX); // don't forget DT_NOCLIP

          if (nAmpIndex >= 0) {
            COLORREF pen = (clrShadow);
            PTOFFSETPT(ptLineFrom, ptOffsetPre);
            PTOFFSETPT(ptLineTo, ptOffsetPre);
            TMoveTo(dc, ptLineFrom.x, ptLineFrom.y);
            TLineTo(dc, ptLineTo.x, ptLineTo.y, pen);
            pen = (clrText);
            PTOFFSETPT(ptLineFrom, ptOffsetPost);
            PTOFFSETPT(ptLineTo, ptOffsetPost);
            TMoveTo(dc, ptLineFrom.x, ptLineFrom.y);
            TLineTo(dc, ptLineTo.x, ptLineTo.y, pen);
          } // if( nAmpIndex >= 0 )
        } // else from if( _ppbd->m_bEnabled || ( _ppbd->m_clrForceTextDisabled != ((COLORREF)-1L) ) )
      } // else from if( _ppbd->m_bHorz )
    } // if( !sTitle )

    if (_ppbd->m_bDrawFocusRect) {
      RECT focusRect = rectCaption; // rectClient
      DeflateRect(&focusRect, 3, 3);
      TDrawFocusRect(dc, &focusRect);
    } // if( _ppbd->m_bDrawFocusRect )
  } // if we have valid area for text & focus rect

  return 0;
}
#define GetDropDividerMerics() iSIZE(6, 6)
int PaintDropDivider(HDDC dc, const RECT* rcItem, BOOL bBefore, BOOL bHorz)
{
  RECT rc2, rc = (*rcItem);
  //SIZE _sizeDDM = GetDropDividerMerics();
  COLORREF clr = GetColor(COLOR_BTNTEXT);
  ASSERT(dc != NULL);

  if (bHorz) {
    InflateRect(&rc, 0, 1);

    if (bBefore) {
      rc.right = rc.left + 1;
      rc.left--;
    } // if( bBefore )
    else {
      rc.left = rc.right - 1;
      rc.right++;
    } // else from if( bBefore )

    TFillRect(dc, &rc, clr);
    rc2 = (rc);
    rc2.bottom = rc2.top + 1;
    InflateRect(&rc2, 2, 0);
    TFillRect(dc, &rc2, clr);
    DeflateRect(&rc2, 1, 0);
    rc2.bottom++;
    TFillRect(dc, &rc2, clr);
    rc2 = rc;
    rc2.top = rc2.bottom - 1;
    InflateRect(&rc2, 2, 0);
    TFillRect(dc, &rc2, clr);
    DeflateRect(&rc2, 1, 0);
    rc2.top--;
    TFillRect(dc, &rc2, clr);
  } // if( bHorz )
  else {
    InflateRect(&rc, 1, 0);

    if (bBefore) {
      rc.bottom = rc.top + 1;
      rc.top--;
    } // if( bBefore )
    else {
      rc.top = rc.bottom - 1;
      rc.bottom++;
      rc.right--;
    } // else from if( bBefore )

    TFillRect(dc, &rc, clr);
    rc2 = (rc);
    rc2.right = rc2.left + 1;
    InflateRect(&rc2, 0, 2);
    TFillRect(dc, &rc2, clr);
    DeflateRect(&rc2, 0, 1);
    rc2.right++;
    TFillRect(dc, &rc2, clr);
    rc2 = rc;
    rc2.left = rc2.right - 1;
    InflateRect(&rc2, 0, 2);
    TFillRect(dc, &rc2, clr);
    DeflateRect(&rc2, 0, 1);
    rc2.left--;
    TFillRect(dc, &rc2, clr);
  } // else from if( bHorz )
}
int PaintDragSourceRect(HDDC dc, const RECT* rcItem)
{
  COLORREF clr = GetColor(COLOR_BTNTEXT);
  RECT rc = (*rcItem);
  ASSERT(dc != NULL);
  TDraw3DRect(dc, &rc, clr, clr);
  DeflateRect(&rc, 1, 1);
  TDraw3DRect(dc, &rc, clr, clr);
}
int PaintMenuSeparator(HDDC dc, const RECT* rectItem, int nIconAreaWidth, BOOL bRarelyUsed, CObject* pHelperSrc, LPARAM lParam)
{
  RECT rectItem2 = (*rectItem);
  int nFunnyDeflateMetric, y;
  COLORREF clrBtnShadow, clrBtnHilight;
  pHelperSrc;
  lParam;
  nIconAreaWidth;

  if (IsRectEmpty(rectItem)) {
    return ;
  }

  if (bRarelyUsed) {
    InflateRect4(&rectItem2, 3, 0, 4, 0);
    TFillRect(dc, &rectItem2, m_brushLightDefault);
    DeflateRect4(&rectItem2, 3, 0, 4, 0);
  }
  else {
    TFillRect(dc, &rectItem2, GetColor(CLR_3DFACE_OUT));
  }

  nFunnyDeflateMetric = GetSystemMetrics(SM_CXSMICON) / 2;
  DeflateRect(&rectItem2, nFunnyDeflateMetric, 0);
  clrBtnShadow = GetColor(bRarelyUsed ? CLR_3DFACE_OUT : CLR_3DSHADOW_OUT);
  clrBtnHilight = GetColor(CLR_3DHILIGHT_OUT);
  y = (rectItem2.top + rectItem2.bottom) / 2;
  rectItem2.top = y - 1;
  rectItem2.bottom = y + 1;
  TDraw3DRect(dc, &rectItem2, clrBtnShadow, clrBtnHilight);
}
// pLogFont->nHeight is interpreted as PointSize * 10
HFONT CreatePointFontIndirect(const LOGFONT* lpLogFont)
{
  POINT ptOrg = { 0, 0 };
  HDC hDC = GetMemDC();
  // convert nPointSize to logical units based on pDC
  LOGFONT logFont = *lpLogFont;
  POINT pt;
  pt.y = GetDeviceCaps(hDC, LOGPIXELSY) * logFont.lfHeight;
  pt.y /= 720; // 72 points/inch, 10 decipoints/point
  DPtoLP(hDC, &pt, 1);
  DPtoLP(hDC, &ptOrg, 1);
  logFont.lfHeight = -abs(pt.y - ptOrg.y);
  return CreateFontIndirect(&logFont);
}
HFONT CreatePointFont(int nPointSize, LPCTSTR lpszFaceName)
{
  LOGFONT logFont;
  memset(&logFont, 0, sizeof(LOGFONT));
  logFont.lfCharSet = DEFAULT_CHARSET;
  logFont.lfHeight = nPointSize;
  lstrcpyn(logFont.lfFaceName, lpszFaceName, countof(logFont.lfFaceName));
  return CreatePointFontIndirect(&logFont);
}
int PaintComboFrame(HDDC dc, PAINTCOMBOFRAMEDATA* _pcfd)
{
  COLORREF clrTopLeft, clrBottomRight, clrHilightOut, clrFaceOut, clrWindow;
  HFONT font;
  RECT rectClient;
  PAINTPUSHBUTTONDATA _ppbd;
  int ppi = GetDeviceCaps(GetMemDC(), LOGPIXELSX);
  int pointsize = MulDiv(60, 96, ppi); // 6 points at 96 ppi

  if (_pcfd->m_clrForceNAContent == (COLORREF)(- 1L)) {
    clrTopLeft = clrBottomRight = GetColor(CLR_3DFACE_OUT);
  }
  else {
    clrTopLeft = clrBottomRight = _pcfd->m_clrForceNAContent;
  }

  if (_pcfd->m_bPushed || _pcfd->m_bHover) {
    clrTopLeft = GetColor(CLR_3DSHADOW_IN);
    clrBottomRight = GetColor(CLR_3DHILIGHT_IN);
  }

  clrHilightOut = (_pcfd->m_clrForceNABorder == (COLORREF)(- 1L) || _pcfd->m_bPushed || _pcfd->m_bHover) ? GetColor(CLR_3DHILIGHT_OUT) : _pcfd->m_clrForceNABorder;
  clrFaceOut = (_pcfd->m_clrForceNABorder == (COLORREF)(- 1L) || _pcfd->m_bPushed || _pcfd->m_bHover) ? GetColor(CLR_3DFACE_OUT) : _pcfd->m_clrForceNABorder;
  clrWindow = (_pcfd->m_clrForceNABorder == (COLORREF)(- 1L) || _pcfd->m_bPushed || _pcfd->m_bHover) ? GetColor(_pcfd->m_bEnabled ? COLOR_WINDOW : COLOR_3DFACE) : _pcfd->m_clrForceNABorder;
  rectClient = (_pcfd->m_rcClient);
  TDraw3DRect(dc, &rectClient, clrTopLeft, clrBottomRight);
  DeflateRect(&rectClient, 1, 1);
  TDraw3DRect(dc, &rectClient, _pcfd->m_bEnabled ? clrFaceOut : clrHilightOut, _pcfd->m_bEnabled ? clrFaceOut : clrHilightOut);
  rectClient.left = rectClient.right - GetSystemMetrics(SM_CXHTHUMB);
  TDraw3DRect(dc, &rectClient, clrFaceOut, clrFaceOut);
  DeflateRect(&rectClient, 1, 1);
  TDraw3DRect(dc, &rectClient, clrHilightOut, clrHilightOut);
  TFillRect4(dc, rectClient.left - 2, rectClient.top, 3, RCHEIGHT(&rectClient), _pcfd->m_bEnabled ? clrWindow : clrFaceOut);

  if (_pcfd->m_bHover || _pcfd->m_bPushed) {
    TFillRect(dc, &rectClient, clrFaceOut);
  }

  rectClient.left++;
  font = CreatePointFont(pointsize, _T("Marlett"));
  InitPAINTPUSHBUTTONDATA(&_ppbd, 0, TRUE, &rectClient, _T("6"), NULL, TRUE, (_pcfd->m_bHover && (!_pcfd->m_bPushed)) ? TRUE : FALSE, //(bHover||bPushed) ? TRUE : FALSE,
      _pcfd->m_bPushed ? TRUE : FALSE, FALSE, _pcfd->m_bEnabled, TRUE, FALSE, FALSE, __ALIGN_HORIZ_CENTER | __ALIGN_VERT, (HFONT)font, 0, 0, 0);
  PaintPushButton(dc, &_ppbd);

  if (!(_pcfd->m_bHover || _pcfd->m_bPushed)) {
    if (!_pcfd->m_bHover) {
      TDraw3DRect(dc, &rectClient, clrHilightOut, clrHilightOut);
    }
    else {
      TDraw3DRect(dc, &rectClient, clrFaceOut, clrFaceOut);
    }
  }
}
int stat_PaintDotNet3dRect(HDDC dc, int x, int y, int cx, int cy, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
  const int nPixelGap = 1;
  TFillRect4(dc, x + nPixelGap, y, cx - 1 - nPixelGap, 1, clrTopLeft);
  TFillRect4(dc, x, y + nPixelGap, 1, cy - 1 - nPixelGap, clrTopLeft);
  TFillRect4(dc, x + cx, y + nPixelGap, - 1, cy - nPixelGap * 2, clrBottomRight);
  TFillRect4(dc, x + nPixelGap, y + cy, cx - nPixelGap * 2, - 1, clrBottomRight);
}
int stat_PaintDotNet3dRect2(HDDC dc, const RECT* lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
  stat_PaintDotNet3dRect(dc, lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, clrTopLeft, clrBottomRight);
}
int stat_PaintRadioDot(HDDC dc, int x, int y, COLORREF color)
{
  RECT rcDot = {x, y, x + 6, y + 6};
  COLORREF color2 = _RGB(255 - GetRV(color), 255 - GetGV(color), 255 - GetBV(color));
  OffsetRect(&rcDot, 1, 1);
  TEllipse(dc, &rcDot, color2);
  OffsetRect(&rcDot, - 1, - 1);
  TEllipse(dc, &rcDot, color);
}
int stat_PaintCheckMark(HDDC dc, int x, int y, COLORREF color)
{
  COLORREF color2 = _RGB(255 - GetRV(color), 255 - GetGV(color), 255 - GetBV(color));
  _SetPixel(dc, x, y + 2, color);
  _SetPixel(dc, x, y + 3, color);
  _SetPixel(dc, x, y + 4, color2);
  _SetPixel(dc, x + 1, y + 3, color);
  _SetPixel(dc, x + 1, y + 4, color);
  _SetPixel(dc, x + 1, y + 5, color2);
  _SetPixel(dc, x + 2, y + 4, color);
  _SetPixel(dc, x + 2, y + 5, color);
  _SetPixel(dc, x + 2, y + 6, color2);
  _SetPixel(dc, x + 3, y + 3, color);
  _SetPixel(dc, x + 3, y + 4, color);
  _SetPixel(dc, x + 3, y + 5, color2);
  _SetPixel(dc, x + 4, y + 2, color);
  _SetPixel(dc, x + 4, y + 3, color);
  _SetPixel(dc, x + 4, y + 4, color2);
  _SetPixel(dc, x + 5, y + 1, color);
  _SetPixel(dc, x + 5, y + 2, color);
  _SetPixel(dc, x + 5, y + 3, color2);
  _SetPixel(dc, x + 6, y, color);
  _SetPixel(dc, x + 6, y + 1, color);
  _SetPixel(dc, x + 6, y + 2, color2);
}
int stat_PaintTabItemImpl(HDDC dc, RECT* rcTabItemsArea, BOOL bTopLeft, BOOL bHorz, BOOL bSelected, BOOL bCenteredText, BOOL bGroupedMode, BOOL bInGroupActive, BOOL bInvertedVerticalMode, const RECT* rcEntireItem, SIZE sizeTextMeasured, HFONT pFont, __EXT_MFC_SAFE_LPCTSTR sText, Image* pIcon, COLORREF clrText, COLORREF clrTabBk, COLORREF clrTabBorderLT, COLORREF clrTabBorderRB, COLORREF clrTabSeparator)
{
  RECT rcItem = (*rcEntireItem);
  RECT rcItemForIcon;
  RECT rcText;
  TCHAR* sItemText;
  SIZE _sizeIcon = {0, 0};
  SIZE _sizeText;
  BOOL bDrawText;

  if (bGroupedMode) {
    RECT rcTabMargin = (rcItem);
    InflateRect4(&rcTabMargin, 0, 0, bHorz ? 1 : 0, bHorz ? 0 : 1);
    InflateRect(&rcTabMargin, bHorz ? 0 : 2, bHorz ? 2 : 0);
    TFillRect(dc, &rcTabMargin, clrTabBk);
    TDraw3DRect(dc, &rcTabMargin, clrTabBorderLT, clrTabBorderRB);
  } // if( bGroupedMode )
  else {
#define __EXTTAB_SEPARATOR_DX 2
#define __EXTTAB_SEPARATOR_DY 2
#define __EXTTAB_SEPARATOR_GAP_HORZ 2
#define __EXTTAB_SEPARATOR_GAP_VERT 2
#define __EXTTAB_MARGIN_BORDER_HX 2
#define __EXTTAB_MARGIN_BORDER_VX 2
#define __EXTTAB_MARGIN_BORDER_HY 2
#define __EXTTAB_MARGIN_BORDER_VY 2
#define __EXTTAB_MARGIN_ICON2TEXT_X 2
#define __EXTTAB_MARGIN_ICON2TEXT_Y 2

    if (bSelected) {
      RECT rcSetMargins, rcTabSel = (rcItem);
      InflateRect4(&rcTabSel, bHorz ? __EXTTAB_SEPARATOR_DX : 0, bHorz ? 0 : __EXTTAB_SEPARATOR_DY, 0, 0);
      SetRect(&rcSetMargins, ((!bHorz) && (!bTopLeft)) ? 1 : 0, (bHorz && (!bTopLeft)) ? 1 : 0, ((!bHorz) && bTopLeft) ? 1 : 0, (bHorz && bTopLeft) ? 1 : 0);
      InflateRect4(&rcTabSel, rcSetMargins.left, rcSetMargins.top, rcSetMargins.right, rcSetMargins.bottom);
      TDraw3DRect(dc, &rcTabSel, clrTabBorderLT, clrTabBorderRB);

      if (bHorz) {
        _SetPixel(dc, rcTabSel.left, bTopLeft ? (rcTabSel.bottom - 1) : rcTabSel.top, clrTabBk);
      }
      else {
        _SetPixel(dc, bTopLeft ? (rcTabSel.right - 1) : rcTabSel.left, rcTabSel.top, clrTabBk);
      }

      DeflateRect4(&rcTabSel, (rcSetMargins.left == 0) ? 1 : 0, (rcSetMargins.top == 0) ? 1 : 0, (rcSetMargins.right == 0) ? 1 : 0, (rcSetMargins.bottom == 0) ? 1 : 0);
      TFillRect(dc, &rcTabSel, clrTabBk);
    } // if( bSelected )
    else {
      RECT rcSeparator = (rcItem);

      if (bHorz) {
        rcSeparator.left = rcSeparator.right - 1;
        DeflateRect(&rcSeparator, 0, __EXTTAB_SEPARATOR_GAP_HORZ);
      }
      else {
        rcSeparator.top = rcSeparator.bottom - 1;
        DeflateRect(&rcSeparator, __EXTTAB_SEPARATOR_GAP_VERT, 0);
      }

      TFillRect(dc, &rcSeparator, clrTabSeparator);
    } // else from if( bSelected )
  } // else from if( bGroupedMode )

  DeflateRect(&rcItem, bHorz ? __EXTTAB_MARGIN_BORDER_HX : __EXTTAB_MARGIN_BORDER_VX, bHorz ? __EXTTAB_MARGIN_BORDER_HY : __EXTTAB_MARGIN_BORDER_VY);

  if (pIcon != NULL) {
    _sizeIcon = iSIZE(pIcon->w, pIcon->h);
    ASSERT(_sizeIcon.w > 0 && _sizeIcon.h > 0);
  } // if( pIcon != NULL && (!pIcon->IsEmpty()) )

  rcItemForIcon = (rcItem);

  if (pIcon != NULL && _sizeIcon.w > 0 && _sizeIcon.h > 0) {
    // rcItemForIcon DeflateRect(&,
    // bHorz ? 0 : ( rcItem RCWIDTH(&) - _sizeIcon.w),
    // bHorz ? ( rcItem RCHEIGHT(&) - _sizeIcon.h) : 0
    // );
    rcItemForIcon.right = rcItemForIcon.left + _sizeIcon.w;
    rcItemForIcon.bottom = rcItemForIcon.top + _sizeIcon.h;
    OffsetRect(&rcItemForIcon, bHorz ? 0 : ((RCWIDTH(&rcItem) - _sizeIcon.w) / 2), bHorz ? ((RCHEIGHT(&rcItem) - _sizeIcon.h) / 2) : 0);

    if (rcItemForIcon.left < (rcItem.left + 1)) {
      rcItemForIcon.left = (rcItem.left + 1);
    }

    if (rcItemForIcon.right < (rcItem.right - 1)) {
      rcItemForIcon.right = (rcItem.right - 1);
    }

    if (rcItemForIcon.top < (rcItem.top + 1)) {
      rcItemForIcon.top = (rcItem.top + 1);
    }

    if (rcItemForIcon.bottom < (rcItem.bottom - 1)) {
      rcItemForIcon.bottom = (rcItem.bottom - 1);
    }
  }

  sItemText = ((sText == NULL) ? _T("") : sText);
  // IMPORTANT: the rcText calculation fixed by Genka
  SetRect(&rcText, rcItem.left // rcItemForIcon.left
      + (bHorz ? (_sizeIcon.w + ((_sizeIcon.w > 0) ? __EXTTAB_MARGIN_ICON2TEXT_X : 0)) : 0), rcItem.top // rcItemForIcon.left
      + (bHorz ? 0 : (_sizeIcon.h + ((_sizeIcon.h > 0) ? __EXTTAB_MARGIN_ICON2TEXT_Y : 0))), rcItem.right, rcItem.bottom);

  if (!bHorz) {
    int nWidth0 = RCWIDTH(&rcText);
    int nWidth1 = RCWIDTH(&rcItem) + __EXTTAB_MARGIN_ICON2TEXT_X * 2;

    if (nWidth1 > nWidth0) {
      if (bInvertedVerticalMode) {
        rcText.right -= __EXTTAB_MARGIN_ICON2TEXT_X;
        rcText.left = rcText.right - nWidth1;
      } // if( bInvertedVerticalMode )
      else {
        rcText.left += __EXTTAB_MARGIN_ICON2TEXT_X;
        rcText.right = rcText.left + nWidth1;
      } // else from if( bInvertedVerticalMode )
    } // if( nWidth1 > nWidth0 )
  } // if( !bHorz )

  //TFillRect(dc, &rcText, _RGB(0,0,255) );
  _sizeText = iSIZE(RCWIDTH(&rcText), RCHEIGHT(&rcText));
  bDrawText = FALSE;

  if ((bHorz && _sizeText.w >= (MAX(16, _sizeIcon.w))) || ((!bHorz) && _sizeText.h >= (MAX(16, _sizeIcon.h)))) {
    bDrawText = TRUE;
  }

  if (!bDrawText && !(bGroupedMode && !bInGroupActive)) {
    OffsetRect(&rcItemForIcon, bHorz ? (RCWIDTH(&rcItem) - _sizeIcon.w) / 2 : 0, bHorz ? 0 : (RCHEIGHT(&rcItem) - _sizeIcon.h) / 2);
  }

  if (pIcon != NULL) {
    if ((bHorz && RCWIDTH(&rcItemForIcon) >= _sizeIcon.w) || (!bHorz && RCHEIGHT(&rcItemForIcon) >= _sizeIcon.h)) {
      RECT rcTmpText = {0, 0, 0, 0};
      OffsetRect(&rcItemForIcon, - 1, - 1);
      PaintIcon(dc, TRUE, sItemText, pIcon, rcItemForIcon, rcTmpText, FALSE, TRUE, FALSE, 0);
    }
  } // if( pIcon != NULL && (! pIcon->IsEmpty()) )

  if (bDrawText) {
    COLORREF textColor = (clrText);
    // if we have sense to paint text on tab item
    ASSERT(pFont != NULL);
    ASSERT(pFont != NULL);
#define RCCENTERX(pRC) (((pRC)->left+(pRC)->right)/2)
#define RCCENTERY(pRC) (((pRC)->top+(pRC)->bottom)/2)

    if (!bHorz) {
      if (bCenteredText) {
        OffsetRect(&rcText, bInvertedVerticalMode ? sizeTextMeasured.h / 2 : - sizeTextMeasured.h / 2, 0);
        TDrawText(dc, sItemText, &rcText, TA_CENTER);
      } // if( bCenteredText )
      else {
        POINT ptCenter;
        OffsetRect(&rcText, bInvertedVerticalMode ? sizeTextMeasured.h / 2 : - sizeTextMeasured.h / 2, 0);
        ptCenter.x = RCCENTERX(&rcText);

        if (bInvertedVerticalMode) {
          ptCenter.y = rcText.bottom - 4 - (RCHEIGHT(&rcText) - sizeTextMeasured.w);
        }
        else {
          ptCenter.y = rcText.top + 4;
        }

        TDrawText(dc, sItemText, &rcText, 0);
      } // else from if( bCenteredText )
    } // if( !bHorz )
    else {
      uint nFormat = DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;

      if (bCenteredText) {
        nFormat |= DT_CENTER;
      }
      else {
        nFormat |= DT_LEFT;
      }

      //rcText InflateRect(&, 0, 3 );
      TDrawText(dc, sItemText, &rcText, nFormat);
    }
  } // if we have sense to paint text on tab item
}
#define __ETWS_ORIENT_TOP 1
#define __ETWS_ORIENT_BOTTOM 2
#define __ETWS_ORIENT_LEFT 3
#define __ETWS_ORIENT_RIGHT 4
int stat_PaintTabClientAreaImpl(HDDC dc, RECT* rcClient, RECT* rcTabItemsArea, RECT* rcTabNearBorderArea, DWORD dwOrientation, BOOL bGroupedMode, COLORREF clrTabItemsAreaBk, COLORREF clrTabNearBorderAreaBk, COLORREF clrTabNearMargin)
{
  RECT rcTabNearMargin = (*rcTabNearBorderArea); // prepare tab border margin rect

  switch (dwOrientation) {
  case __ETWS_ORIENT_TOP:
    rcTabNearMargin.bottom = rcTabNearMargin.top + 1;
    break;

  case __ETWS_ORIENT_BOTTOM:
    rcTabNearMargin.top = rcTabNearMargin.bottom - 1;
    break;

  case __ETWS_ORIENT_LEFT:
    rcTabNearMargin.right = rcTabNearMargin.left + 1;
    break;

  case __ETWS_ORIENT_RIGHT:
    rcTabNearMargin.left = rcTabNearMargin.right - 1;
    break;
#ifdef _DEBUG

  default:
    ASSERT(FALSE);
    break;
#endif // _DEBUG
  } // switch( dwOrientation )

  // fill all area with tab items background color
  TFillRect(dc, rcClient, clrTabItemsAreaBk);

  if (!bGroupedMode) {
    // fill tab border area
    TFillRect(dc, rcTabNearBorderArea, clrTabNearBorderAreaBk);
    // paint tab border margin
    TFillRect(dc, &rcTabNearMargin, clrTabNearMargin);
  } // if( !bGroupedMode )
}
#define __ETWH_BUTTON_LEFTUP 1
#define __ETWH_BUTTON_RIGHTDOWN 2
#define __ETWH_BUTTON_HELP 3
#define __ETWH_BUTTON_CLOSE 4
int stat_PaintTabButtonImpl(HDDC dc, RECT* rcButton, LONG nHitTest, BOOL bTopLeft, BOOL bHorz, BOOL bEnabled, BOOL bHover, BOOL bPushed, BOOL bGroupedMode, COLORREF clrGlyph, COLORREF clrTL, COLORREF clrBR)
{
  glyph_t* pGlyph = NULL;
  COLORREF ColorValues[2] = {_RGB(0, 0, 0), clrGlyph};

  switch (nHitTest) {
  case __ETWH_BUTTON_LEFTUP:
    pGlyph = g_DockingCaptionGlyphs[bHorz ? (bEnabled ? __DCBT_ARROW_LEFT : __DCBT_HOLLOW_LEFT) : (bEnabled ? __DCBT_ARROW_UP : __DCBT_HOLLOW_UP)];
    ASSERT(pGlyph != NULL);
    break;

  case __ETWH_BUTTON_RIGHTDOWN:
    pGlyph = g_DockingCaptionGlyphs[bHorz ? (bEnabled ? __DCBT_ARROW_RIGHT : __DCBT_HOLLOW_RIGHT) : (bEnabled ? __DCBT_ARROW_DOWN : __DCBT_HOLLOW_DOWN)];
    ASSERT(pGlyph != NULL);
    break;

  case __ETWH_BUTTON_HELP:
    pGlyph = g_DockingCaptionGlyphs[__DCBT_WND_CONTEXTHELP];
    ASSERT(pGlyph != NULL);
    break;

  case __ETWH_BUTTON_CLOSE:
    pGlyph = g_DockingCaptionGlyphs[__DCBT_CLOSE];
    ASSERT(pGlyph != NULL);
    break;
#ifdef _DEBUG

  default:
    ASSERT(FALSE);
    break;
#endif // _DEBUG
  } // switch( nHitTest )

  ASSERT(pGlyph != NULL);
  //TFillRect(dc, &rcButton, _RGB( 255, 128, 128 ) );
  PaintGlyphCentered(dc, rcButton, pGlyph, ColorValues);

  if (bHover || bPushed) {
    TDraw3DRect(dc, rcButton, clrTL, clrBR);
  }
}
int PaintTabItem(HDDC dc, RECT* rcTabItemsArea, BOOL bTopLeft, BOOL bHorz, BOOL bSelected, BOOL bCenteredText, BOOL bGroupedMode, BOOL bInGroupActive, BOOL bInvertedVerticalMode, const RECT* rcEntireItem, SIZE sizeTextMeasured, HFONT pFont, __EXT_MFC_SAFE_LPCTSTR sText, Image* pIcon, CObject* pHelperSrc, LPARAM lParam)
{
  COLORREF clrText = GetColor((bSelected && !bGroupedMode) ? COLOR_BTNTEXT : (bGroupedMode ? COLOR_BTNTEXT : COLOR_3DHILIGHT));
  COLORREF clrTabBk = GetColor(COLOR_BTNFACE);
  COLORREF clrTabBorderLT = GetColor(bGroupedMode ? COLOR_3DSHADOW : COLOR_3DHILIGHT);
  COLORREF clrTabBorderRB = GetColor(bGroupedMode ? COLOR_3DSHADOW : COLOR_3DDKSHADOW);
  COLORREF clrTabSeparator = GetColor((bGroupedMode ? COLOR_3DSHADOW : COLOR_3DLIGHT));
  stat_PaintTabItemImpl(dc, rcTabItemsArea, bTopLeft, bHorz, bSelected, bCenteredText, bGroupedMode, bInGroupActive, bInvertedVerticalMode, rcEntireItem, sizeTextMeasured, pFont, sText, pIcon, clrText, clrTabBk, clrTabBorderLT, clrTabBorderRB, clrTabSeparator);
}
int PaintMenuItem(HDDC dc, PAINTMENUITEMDATA* _pmid)
{
  TCHAR* _sText = (_pmid->m_sText == NULL) ? _T("") : _pmid->m_sText;
  TCHAR* _sAccelText = (_pmid->m_sAccelText == NULL) ? _T("") : _pmid->m_sAccelText;
  COLORREF clr3dFace = GetColor(CLR_3DFACE_OUT);
  COLORREF clrBk = clr3dFace;
  COLORREF clrText = GetColor(CLR_TEXT_OUT);
  RECT rcBk = (_pmid->m_rcItem);
  BOOL bSmallerSelRect;
  RECT rcMeasureText, rcDrawText, rcIcon, rcIconFrame;
  SIZE sizeIcon;
  MOVEDEF;
  rcDrawText;
  rcMeasureText;

  if (IsRectEmpty(&_pmid->m_rcItem)) {
    return ;
  }

  TFillRect(dc, &rcBk, clrBk);

  if (_pmid->m_bSelected) {
    ASSERT(_pmid->m_bEnabled);
    clrBk = GetColor(COLOR_HIGHLIGHT);
    clrText = GetColor(COLOR_HIGHLIGHTTEXT);
  }

  //else
  {
    if (_pmid->m_bRarelyUsed) {
      InflateRect4(&rcBk, 3, 0, 4, 0);
      TFillRect(dc, &rcBk, m_brushLightDefault);

      if (!_pmid->m_bRarelyUsedPrev && 0) {
        COLORREF penShadow = GetColor(COLOR_3DSHADOW);
        TMoveTo(dc, rcBk.left, rcBk.top);
        TLineTo(dc, rcBk.right, rcBk.top, penShadow);
      }

      if (!_pmid->m_bRarelyUsedNext && 0) {
        COLORREF penLight = GetColor(COLOR_3DHIGHLIGHT);
        TMoveTo(dc, rcBk.left, rcBk.bottom - 1);
        TLineTo(dc, rcBk.right, rcBk.bottom - 1, penLight);
      }

      DeflateRect4(&rcBk, 3, 0, 4, 0);
    } // if( _pmid->m_bRarelyUsed )
  }
  bSmallerSelRect = ((_pmid->m_pIcon != NULL && (!_pmid->m_pIcon)) || _pmid->m_bPushedCheck || _pmid->m_bPushedRadio) ? TRUE : FALSE;

  if (bSmallerSelRect) {
    rcBk.left += _pmid->m_nIconAreaWidth;
  }

  if (_pmid->m_bSelected || (!_pmid->m_bRarelyUsed)) {
    TFillRect(dc, &rcBk, clrBk);
  }

  if (!bSmallerSelRect) {
    rcBk.left += _pmid->m_nIconAreaWidth;
  }

  DeflateRect(&rcBk, __EXT_MENU_GAP, __EXT_MENU_GAP);
  DeflateRect(&rcBk, __EXT_MENU_GAP, 0);

  //TSelectFont(m_FontNormal);
  if (_pmid->m_bPopup || 1) {
    RECT rcPopupArrow = (_pmid->m_rcItem);
    DeflateRect(&rcPopupArrow, __EXT_MENU_GAP, __EXT_MENU_GAP);
    rcPopupArrow.left = rcPopupArrow.right - __EXT_MENU_POPUP_ARROW_AREA_DX;

    if (_pmid->m_bCustomizeListMode) {
      COLORREF ColorValues[2] = {
        _RGB(0, 0, 0), GetColor(_pmid->m_bSelected ? COLOR_HIGHLIGHTTEXT : CLR_TEXT_OUT)
      };
      PaintGlyphCentered(dc, &rcPopupArrow, _pmid->m_bSeparatedDropDown ? &g_glyph_customize_dropdown_src : &g_glyph_customize_popup_src, ColorValues);
    } // if( _pmid->m_bCustomizeListMode )
    else {
      TSelectFont(m_FontMarlett);
      TDrawText(dc, _T("8"), &rcPopupArrow, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    } // else from if( _pmid->m_bCustomizeListMode )
  } // if( _pmid->m_bPopup )

  sizeIcon.w = __EXT_MENU_ICON_CX, sizeIcon.h = __EXT_MENU_ICON_CY;

  if (_pmid->m_pIcon != NULL && (!_pmid->m_pIcon)) {
    sizeIcon.w = _pmid->m_pIcon->w, sizeIcon.h = _pmid->m_pIcon->h;

    if (sizeIcon.w <= 18 && sizeIcon.h < sizeIcon.w) {
      sizeIcon.h = sizeIcon.w;
    }
  }

  SetRect(&rcIcon, _pmid->m_rcItem.left, _pmid->m_rcItem.top, _pmid->m_rcItem.left + sizeIcon.w, _pmid->m_rcItem.top + sizeIcon.h);

  if (_pmid->m_bXtraMark) {
    OffsetRect(&rcIcon, (_pmid->m_nIconAreaWidth / 2 - RCWIDTH(&rcIcon)) / 2 + _pmid->m_nIconAreaWidth / 2 - 2, (RCHEIGHT(&_pmid->m_rcItem) - RCHEIGHT(&rcIcon)) / 2);

    if (_pmid->m_bXtraChecked) {
      RECT rcXtraMark;
      int x, y;
      COLORREF clrCheck = GetColor(CLR_TEXT_OUT);
      SetRect(&rcXtraMark, _pmid->m_rcItem.left + 1, _pmid->m_rcItem.top + 1, _pmid->m_rcItem.left + RCHEIGHT(&_pmid->m_rcItem) - 2, _pmid->m_rcItem.bottom - 2);
      TFillRect(dc, &rcXtraMark, m_brushLightDefault);
      x = rcXtraMark.left + RCWIDTH(&rcXtraMark) / 2 - 3;
      y = rcXtraMark.top + RCHEIGHT(&rcXtraMark) / 2 - 3;
      ASSERT(!_pmid->m_bPushedRadio);
      //stat_PaintCheckMark(dc, x, y, clrCheck);
      TDraw3DRect(dc, &rcXtraMark, GetColor(CLR_3DSHADOW_OUT), GetColor(CLR_3DHILIGHT_OUT));
    } // if( _pmid->m_bXtraChecked )
  } // if( _pmid->m_bXtraMark )
  else {
    OffsetRect(&rcIcon, (_pmid->m_nIconAreaWidth - RCWIDTH(&rcIcon)) / 2, (RCHEIGHT(&_pmid->m_rcItem) - RCHEIGHT(&rcIcon)) / 2);
  } // else from if( _pmid->m_bXtraMark )

  rcIconFrame = (rcIcon);
  InflateRect(&rcIconFrame, __EXT_MENU_IMG_RECT_OUT_GAP, __EXT_MENU_IMG_RECT_OUT_GAP);

  if (_pmid->m_bPushedCheck || _pmid->m_bPushedRadio) {
    TFillRect(dc, &rcIconFrame, m_brushLightDefault);
  }

  if (_pmid->m_pIcon != NULL) {
    TDrawImage(dc, rcIcon.left, rcIcon.top, _pmid->m_pIcon->w, _pmid->m_pIcon->h, _pmid->m_pIcon, 0, 0);

    if (_pmid->m_bSelected && _pmid->m_bEnabled && (!(_pmid->m_bPushedCheck || _pmid->m_bPushedRadio))) {
      TDraw3DRect(dc, &rcIconFrame, GetColor(CLR_3DHILIGHT_OUT), GetColor(CLR_3DSHADOW_OUT));
    }
  } // if( _pmid->m_pIcon != NULL && (! _pmid->m_pIcon->IsEmpty()) )
  else {
    PaintSysCmdGlyph(dc, _pmid->m_nHelperSysCmdID, &rcIcon, _pmid->m_bSelected, FALSE, _pmid->m_bEnabled);
  } // else from if( _pmid->m_pIcon != NULL && (! _pmid->m_pIcon->IsEmpty()) )

  if (_pmid->m_bPushedCheck || _pmid->m_bPushedRadio) {
    if (_pmid->m_pIcon == NULL) {
      COLORREF clrCheck = GetColor(CLR_TEXT_OUT);
      int x = rcIcon.left + RCWIDTH(&rcIcon) / 2 - 3;
      int y = rcIcon.top + RCHEIGHT(&rcIcon) / 2 - 3;

      if (_pmid->m_bPushedCheck) {
        ASSERT(!_pmid->m_bPushedRadio);
        stat_PaintCheckMark(dc, x, y, clrCheck);
      }
      else {
        ASSERT(!_pmid->m_bPushedCheck);
        stat_PaintRadioDot(dc, x, y, clrCheck);
      }
    } // if( _pmid->m_pIcon == NULL || _pmid->m_pIcon->IsEmpty() )

    TDraw3DRect(dc, &rcIconFrame, GetColor(CLR_3DSHADOW_OUT), GetColor(CLR_3DHILIGHT_OUT));
  }

  if (!IsRectEmpty(&_pmid->m_rcInPlaceWnd)) {
    RECT rcInPlaceWndOuterBorder = (_pmid->m_rcInPlaceWnd);
    InflateRect4(&rcInPlaceWndOuterBorder, 2, 2, 2, 3);

    if (_pmid->m_bPopup) {
      PAINTCOMBOFRAMEDATA _pcfd;
      INT nDD = (INT)GetSystemMetrics(SM_CXHTHUMB);
      rcInPlaceWndOuterBorder.right += nDD;
      // rcInPlaceWndOuterBorder InflateRect(&, 1, 1 );
      InitPAINTCOMBOFRAMEDATA(&_pcfd, _pmid->m_pHelperSrc, &rcInPlaceWndOuterBorder, _pmid->m_bSelected, // hover
          _pmid->m_bSelected && _pmid->m_bHelperPopupDropped, // pushed
          _pmid->m_bEnabled);
      _pcfd.m_bNoTopOuterBorder = TRUE;
      PaintComboFrame(dc, &_pcfd);
      DeflateRect(&rcInPlaceWndOuterBorder, 1, 1);
      rcInPlaceWndOuterBorder.right -= nDD;
    } // if( _pmid->m_bPopup )
    else {
      COLORREF clrFace = GetColor(COLOR_3DFACE);
      TDraw3DRect(dc, &rcInPlaceWndOuterBorder, clrFace, clrFace);
      DeflateRect(&rcInPlaceWndOuterBorder, 1, 1);

      if (_pmid->m_bSelected) {
        TDraw3DRect(dc, &rcInPlaceWndOuterBorder, GetColor(COLOR_3DSHADOW), GetColor(COLOR_3DHIGHLIGHT));
      }
      else {
        TDraw3DRect(dc, &rcInPlaceWndOuterBorder, clrFace, clrFace);
      }
    } // else from if( _pmid->m_bPopup )

    DeflateRect(&rcInPlaceWndOuterBorder, 1, 1);
    TFillRect(dc, &rcInPlaceWndOuterBorder, GetColor(_pmid->m_bEnabled ? COLOR_WINDOW : COLOR_3DFACE));

    if (!_pmid->m_strInPlaceText) {
      COLORREF textColor = (GetColor(COLOR_BTNTEXT));
      DeflateRect4(&rcInPlaceWndOuterBorder, 5, 2, 1, 2);
      TDrawText2(dc, _pmid->m_strInPlaceText, strlen(_pmid->m_strInPlaceText), &rcInPlaceWndOuterBorder, DT_SINGLELINE | DT_NOPREFIX | DT_LEFT | DT_TOP, textColor);
    } // if( !_pmid->m_strInPlaceText )
  } // if( !_pmid->m_rcInPlaceWnd IsRectEmpty(&) )
}
int PaintMenuExpandButton(HDDC dc, const RECT* rectButton, BOOL bPushed)
{
  RECT rcGlyph = (*rectButton);
  COLORREF ColorValues[] = {
    _RGB(0, 0, 0), GetColor(CLR_TEXT_OUT)
  };

  if (bPushed) {
    TFillRect(dc, &rcGlyph, m_brushLightDefault);
    TDraw3DRect(dc, &rcGlyph, GetColor(CLR_3DHILIGHT_OUT), GetColor(CLR_3DSHADOW_OUT));
  }
  else {
    PAINTMENUITEMDATA _pmid;
    InitPAINTMENUITEMDATA(&_pmid, 0, rectButton, 0, _T(""), _T(""), NULL, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, 0);
    PaintMenuItem(dc, &_pmid);
  }

  PaintGlyphCentered(dc, &rcGlyph, &g_glyph_btn_expand_bottom2, ColorValues);
}
int PaintMenuExpandButtonXP(HDDC dc, const RECT* rectButton, BOOL bPushed)
{
  int nIconAreaWidth = 0;
  RECT rcGlyph = (*rectButton);
  COLORREF ColorValues[] = {
    _RGB(0, 0, 0), GetColor(CLR_TEXT_OUT)
  };

  if (!bPushed) {
    PAINTMENUITEMDATA _pmid;
    RECT rc = (*rectButton);
    rc.top--;
    InitPAINTMENUITEMDATA(&_pmid, 0, &rc, nIconAreaWidth, _T(""), _T(""), NULL, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, 0);
    PaintMenuItem(dc, &_pmid);
  } // if( !bPushed )

  if (bPushed) {
    PAINTPUSHBUTTONDATA _ppbd;
    SIZE sz = GetPushedOffset();
    InitPAINTPUSHBUTTONDATA(&_ppbd, 0, TRUE, rectButton, _T(""), NULL, TRUE, FALSE, bPushed, FALSE, FALSE, TRUE, FALSE, FALSE, __ALIGN_HORIZ_CENTER | __ALIGN_VERT, NULL, FALSE, 0, 0);
    PaintPushButton(dc, &_ppbd);
    OffsetRect(&rcGlyph, sz.w, sz.h);
  }
  else {
    PAINTMENUITEMDATA _pmid;
    InitPAINTMENUITEMDATA(&_pmid, 0, rectButton, nIconAreaWidth, _T(""), _T(""), NULL, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, 0);
    PaintMenuItem(dc, &_pmid);
  }

  PaintGlyphCentered(dc, &rcGlyph, &g_glyph_btn_expand_bottom2, ColorValues);
}
#if 0
int PaintMenuExpandButtonOffice2003(HDDC dc, const RECT* rectButton, BOOL bPushed, BOOL bHover, CObject* pHelperSrc, LPARAM lParam)
{
  POINT ptRgnEffectiveOffset;
  int nIconAreaWidth = 0;
  RECT rcButton = (*rectButton); // (+ 2.23)
  RECT rcGlyph = (rcButton);
  COLORREF _brBaloon = (GetColor(_2003CLR_MENUEXPBALOON));
  ASSERT(m_sizeMenuExpBtnCircle.w > 0);
  ASSERT(m_sizeMenuExpBtnCircle.h > 0);

  if ((RCWIDTH(&rcButton) & 0x01) != 0) {
    // (+ 2.23)
    rcButton.right--; // (+ 2.23)
  }

  ptRgnEffectiveOffset.x = (rcButton.right - rcButton.left - m_sizeMenuExpBtnCircle.w) / 2 + rcButton.left;
  ptRgnEffectiveOffset.y = (rcButton.bottom - rcButton.top - m_sizeMenuExpBtnCircle.h) / 2 + rcButton.top;

  if (m_rgnMenuExpBtnCircle == NULL || m_bmpCtBarGradientHorz == NULL || m_bmpCtBarGradientVert == NULL || m_bmpDockerGradient == NULL || stat_GetBPP() <= 8
      //|| (!rgnEffective.CreateRectRgn(0, 0, 1, 1)) || rgnEffective.CopyRgn(&m_rgnMenuExpBtnCircle) == ERROR || rgnEffective.OffsetRgn(ptRgnEffectiveOffset) == ERROR
     ) {
    PaintMenuExpandButtonXP(dc, rectButton, bPushed);
    return ;
  }

  if (!bPushed) {
    PAINTMENUITEMDATA _pmid;
    RECT rc = (rcButton);
    rc.top--;
    InitPAINTMENUITEMDATA(&_pmid, 0, &rc, nIconAreaWidth, _T(""), _T(""), NULL, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, 0);
    PaintMenuItem(dc, &_pmid);
  }

  if (bPushed) {
    PAINTPUSHBUTTONDATA _ppbd;
    SIZE sz = GetPushedOffset();
    InitPAINTPUSHBUTTONDATA(&_ppbd, 0, TRUE, rectButton, _T(""), NULL, TRUE, FALSE, bPushed, FALSE, FALSE, TRUE, FALSE, FALSE, __ALIGN_HORIZ_CENTER | __ALIGN_VERT, NULL, FALSE, 0, 0);
    PaintPushButton(dc, &_ppbd);
    OffsetRect(&rcGlyph, sz.w, sz.h);
  }
  else {
    PAINTMENUITEMDATA _pmid;
    InitPAINTMENUITEMDATA(&_pmid, 0, rectButton, nIconAreaWidth, _T(""), _T(""), NULL, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, 0);
    PaintMenuItem(dc, &_pmid);
  }

  ASSERT(_brBaloon != NULL);
  FillRgn(dc, rgnEffective, (HBRUSH)_brBaloon);
  COLORREF ColorValues[] = {
    _RGB(0, 0, 0), GetColor(CLR_TEXT_OUT)
  };
  PaintGlyphCentered(dc, &rcGlyph, &g_glyph_btn_expand_bottom2, ColorValues);
}
int PaintToolbarExpandButton(HDDC dc, const RECT* rcButtonArea, BOOL bHorz, // if FALSE - down
    BOOL bBarIsCompletelyVisible, BOOL bEnabled, BOOL bPushed, BOOL bHover, CObject* pHelperSrc, LPARAM lParam, // = 0L
    BOOL bTransparentBackground // = FALSE
                            )
{
  ASSERT(dc != NULL);
  pHelperSrc;
  lParam;
  RECT rect(rcButtonArea);
  rect.NormalizeRect();

  if (!dc.RectVisible(&rect)) {
    return ;
  }

  RECT rectButton(rect);
  // ASSERT( bLeft ); // temporary
  const glyph_t* pGlyph = NULL, *pGlyph2 = NULL;

  if (bHorz) {
    pGlyph = &g_glyph_btn_expand_bottom;

    if (!bBarIsCompletelyVisible) {
      pGlyph2 = &g_glyph_btn_expand_right2;
    }
  }
  else {
    pGlyph = &g_glyph_btn_expand_left;

    if (!bBarIsCompletelyVisible) {
      pGlyph2 = &g_glyph_btn_expand_bottom2;
    }
  }

  ASSERT(pGlyph != NULL);
  PAINTPUSHBUTTONDATA _ppbd(this, TRUE, rect, _T(""), NULL, TRUE, bHover, bPushed, FALSE, bEnabled, TRUE, FALSE, FALSE, __ALIGN_HORIZ_CENTER | __ALIGN_VERT, NULL, FALSE, 0, bTransparentBackground);
  PaintPushButton(dc, _ppbd);
  COLORREF clr = GetColor(bEnabled ? CLR_TEXT_OUT : COLOR_3DHILIGHT);
  COLORREF ColorValues[] = {
    0, clr
  };

  if (bPushed) {
    rect OffsetRect(&, GetPushedOffset());
  }

  RECT rectGlyph(rectButton.TopLeft(), pGlyph->Size());
  RECT rectGlyph2(rectGlyph);
  SIZE sizePushedOffset = GetPushedOffset();

  if (bPushed) {
    rectGlyph OffsetRect(&, sizePushedOffset);
    rectGlyph2 OffsetRect(&, sizePushedOffset);
  }

  if (bHorz) {
    int nGap = (max(sizePushedOffset.h, 1)) * 3;
    rectGlyph OffsetRect(&, (rectButton.Size().w - pGlyph->Size().w) / 2, rectButton.Size().h - pGlyph->Size().h - nGap);

    if (!bBarIsCompletelyVisible) {
      ASSERT(pGlyph2 != NULL);
      rectGlyph2 OffsetRect(&, (rectButton.Size().w - pGlyph2->Size().w) / 2, nGap);
    }
  }
  else {
    int nGap = (max(sizePushedOffset.w, 1)) * 3;
    rectGlyph OffsetRect(&, nGap, (rectButton.Size().h - pGlyph->Size().h) / 2);

    if (!bBarIsCompletelyVisible) {
      ASSERT(pGlyph2 != NULL);
      rectGlyph2 OffsetRect(&, rectButton.Size().w - pGlyph2->Size().w - nGap, (rectButton.Size().h - pGlyph2->Size().h) / 2);
    }
  }

  if (bEnabled) {
    PaintGlyph(dc, rectGlyph.TopLeft(), *pGlyph, ColorValues);

    if (!bBarIsCompletelyVisible) {
      ASSERT(pGlyph2 != NULL);
      PaintGlyph(dc, rectGlyph2.TopLeft(), *pGlyph2, ColorValues);
    }
  } // if( bEnabled )
  else {
    // rect OffsetRect(&, 1,1);
    // PaintGlyphCentered(
    // dc,rect,*pGlyph,ColorValues
    // );
    // ColorValues[1] = GetColor(COLOR_3DSHADOW);
    // rect OffsetRect(&, -1,-1);
    // PaintGlyphCentered(
    // dc,rect,*pGlyph,ColorValues
    // );
    rectGlyph OffsetRect(&, 1, 1);
    PaintGlyph(dc, rectGlyph.TopLeft(), *pGlyph, ColorValues);
    rectGlyph OffsetRect(&, - 1, - 1);
    ColorValues[1] = GetColor(COLOR_3DSHADOW);
    PaintGlyph(dc, rectGlyph.TopLeft(), *pGlyph, ColorValues);
  } // else from if( bEnabled )
}
int CExtPaintManagerXP::PaintToolbarExpandButton(HDDC dc, const RECT* rcButtonArea, BOOL bHorz, // if FALSE - down
    BOOL bBarIsCompletelyVisible, BOOL bEnabled, BOOL bPushed, BOOL bHover, CObject* pHelperSrc, LPARAM lParam, // = 0L
    BOOL bTransparentBackground // = FALSE
                                                )
{
  ASSERT(dc != NULL);
  pHelperSrc;
  lParam;
  RECT rect(rcButtonArea);
  rect.NormalizeRect();

  if (!dc.RectVisible(&rect)) {
    return ;
  }

  RECT rectButton(rect);
  const glyph_t* pGlyph = NULL, *pGlyph2 = NULL;

  if (bHorz) {
    pGlyph = m_pGlyphTbEpBtnH0;
    pGlyph2 = m_pGlyphTbEpBtnH1;
  } // if( bHorz )
  else {
    pGlyph = m_pGlyphTbEpBtnV0;
    pGlyph2 = m_pGlyphTbEpBtnV1;
  } // else from if( bHorz )

  ASSERT(pGlyph != NULL);
  ASSERT(pGlyph2 != NULL);
  PAINTPUSHBUTTONDATA _ppbd(this, TRUE, rect, _T(""), NULL, TRUE, bHover, bPushed, FALSE, bEnabled, TRUE, FALSE, FALSE, __ALIGN_HORIZ_CENTER | __ALIGN_VERT, NULL, FALSE, 0, bTransparentBackground);
  PaintPushButton(dc, _ppbd);
  COLORREF clr = GetColor(bEnabled ? CLR_TEXT_OUT : COLOR_3DHILIGHT);
  COLORREF ColorValues[] = {
    0, clr
  };

  if (bPushed) {
    rect OffsetRect(&, GetPushedOffset());
  }

  RECT rectGlyph(rectButton.TopLeft(), pGlyph->Size());
  RECT rectGlyph2(rectGlyph);
  SIZE sizePushedOffset = GetPushedOffset();

  if (bPushed) {
    rectGlyph OffsetRect(&, sizePushedOffset);
    rectGlyph2 OffsetRect(&, sizePushedOffset);
  } // if( bPushed )

  if (bHorz) {
    int nGap = (max(sizePushedOffset.h, 1)) * 3;
    rectGlyph OffsetRect(&, (rectButton.Size().w - pGlyph->Size().w) / 2, // + 1
        rectButton.Size().h - pGlyph->Size().h - nGap);
    rectGlyph2 OffsetRect(&, (rectButton.Size().w - pGlyph2->Size().w) / 2 + 1, nGap);
  } // if( bHorz )
  else {
    int nGap = (max(sizePushedOffset.w, 1)) * 3;
    rectGlyph OffsetRect(&, nGap, (rectButton.Size().h - pGlyph->Size().h) / 2 // + 1
                        );
    rectGlyph2 OffsetRect(&, rectButton.Size().w - pGlyph2->Size().w - nGap, (rectButton.Size().h - pGlyph2->Size().h) / 2 + 1);

    if (m_bExpBtnSwapVGlyphs) {
      RECT rcTmp(rectGlyph);
      rectGlyph = rectGlyph2;
      rectGlyph2 = rcTmp;
    }
  } // else from if( bHorz )

  if (bEnabled) {
    // if( bHover && (!bPushed) )
    // {
    // COLORREF ColorValuesHover[] =
    // {
    // 0,
    // GetColor(COLOR_3DDKSHADOW)
    // };
    // rectGlyph OffsetRect(&, -1,-1);
    // rectGlyph2 OffsetRect(&, -1,-1);
    // PaintGlyph(
    // dc,rectGlyph.TopLeft(),*pGlyph,ColorValuesHover
    // );
    // if( !bBarIsCompletelyVisible )
    // PaintGlyph(
    // dc,rectGlyph2.TopLeft(),*pGlyph2,ColorValuesHover
    // );
    // rectGlyph OffsetRect(&, 1,1);
    // rectGlyph2 OffsetRect(&, 1,1);
    // }
    PaintGlyph(dc, rectGlyph.TopLeft(), *pGlyph, ColorValues);

    if (!bBarIsCompletelyVisible) {
      PaintGlyph(dc, rectGlyph2.TopLeft(), *pGlyph2, ColorValues);
    }
  } // if( bEnabled )
  else {
    ColorValues[1] = GetColor(COLOR_3DSHADOW);
    PaintGlyph(dc, rectGlyph.TopLeft(), *pGlyph, ColorValues);
  } // else from if( bEnabled )
}
int CExtPaintManagerOffice2003::PaintToolbarExpandButton(HDDC dc, const RECT* rcButtonArea, BOOL bHorz, // if FALSE - down
    BOOL bBarIsCompletelyVisible, BOOL bEnabled, BOOL bPushed, BOOL bHover, CObject* pHelperSrc, LPARAM lParam, // = 0L
    BOOL bTransparentBackground // = FALSE
                                                        )
{
  ASSERT(dc != NULL);
  pHelperSrc;
  lParam;
  RECT rect(rcButtonArea);
  rect.NormalizeRect();

  if (!dc.RectVisible(&rect)) {
    return ;
  }

  CExtToolControlBar* pToolBar = NULL;
  BOOL bStyleOffice2003 = TRUE;

  if (pHelperSrc == NULL || (!bEnabled) || (!pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton))) || (!GetCb2DbTransparentMode(pHelperSrc)) || stat_GetBPP() <= 8) {
    bStyleOffice2003 = FALSE;
  }
  else {
    pToolBar = ((CExtBarContentExpandButton*)pHelperSrc)->GetBar();
    ASSERT_VALID(pToolBar);

    if (pToolBar->m_bPaletteMode || pToolBar->m_pDockSite == NULL || pToolBar->IsFloating() || pToolBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) || pToolBar->IsKindOf(RUNTIME_CLASS(CExtPanelControlBar)) || pToolBar->m_bPresubclassDialogMode || pToolBar->m_pDockSite == NULL) {
      bStyleOffice2003 = FALSE;
    }
  }

  if (!bStyleOffice2003) {
    CExtPaintManagerXP::PaintToolbarExpandButton(dc, rcButtonArea, bHorz, bBarIsCompletelyVisible, bEnabled, bPushed, bHover, pHelperSrc, lParam, bTransparentBackground);
    return ;
  }

  ASSERT_VALID(pToolBar);
  RECT rcToolbarClientArea;
  pToolBar->GetClientRect(&rcToolbarClientArea);
  RECT rectButton(rect);
  RECT rcBaloonExclude(rcToolbarClientArea);

  if (bHorz) {
    rectButton OffsetRect(&, rcToolbarClientArea.right - rectButton.right, 0);
    rectButton InflateRect(&, 1, 0, 0, 0);
    rectButton.top = rcToolbarClientArea.top;
    rectButton.bottom = rcToolbarClientArea.bottom - 1;
    rcBaloonExclude.right = rectButton.left + 1;
    rectButton.left -= m_sizeToolBarRgnRounders.w;
  } // if( bHorz )
  else {
    rectButton OffsetRect(&, 0, rcToolbarClientArea.bottom - rectButton.bottom);
    rectButton InflateRect(&, 0, 1, 0, 0);
    rectButton.left = rcToolbarClientArea.left;
    rectButton.right = rcToolbarClientArea.right - 1;
    rcBaloonExclude.bottom = rectButton.top + 1;
    rectButton.top -= m_sizeToolBarRgnRounders.h;
  } // else from if( bHorz )

  CRgn rgnButton, rgnBE;

  if ((!rgnButton.CreateRoundRectRgn(rcToolbarClientArea.left, rcToolbarClientArea.top, rcToolbarClientArea.right, rcToolbarClientArea.bottom, m_sizeToolBarRgnRounders.w, m_sizeToolBarRgnRounders.h)) || (!rgnBE.CreateRoundRectRgn(rcBaloonExclude.left, rcBaloonExclude.top, rcBaloonExclude.right, rcBaloonExclude.bottom, m_sizeToolBarRgnRounders.w, m_sizeToolBarRgnRounders.h)) || rgnButton.CombineRgn(&rgnButton, &rgnBE, RGN_DIFF) == ERROR) {
    ASSERT(FALSE);
    CExtPaintManagerXP::PaintToolbarExpandButton(dc, rcButtonArea, bHorz, bBarIsCompletelyVisible, bEnabled, bPushed, bHover, pHelperSrc, lParam, bTransparentBackground);
    return ;
  }

  dc.SelectClipRgn(&rgnButton);

  if (bHover || bPushed) {
    RECT rcSolidFill(rectButton);
    rcSolidFill InflateRect(&, 1, 1);
    // TFillRect(dc,
    // &rcSolidFill,
    // GetColor(
    // bPushed
    // ? _2003CLR_EXPBTN_PRESSED
    // : _2003CLR_EXPBTN_HOVER
    // )
    // );
    stat_PaintGradientRect(dc, &rcSolidFill, GetColor(bPushed ? (bHorz ? _2003CLR_EXPBTN_PRESSED_LIGHT : _2003CLR_EXPBTN_PRESSED_DARK) : (bHorz ? _2003CLR_EXPBTN_HOVER_DARK : _2003CLR_EXPBTN_HOVER_LIGHT)), GetColor(bPushed ? (bHorz ? _2003CLR_EXPBTN_PRESSED_DARK : _2003CLR_EXPBTN_PRESSED_LIGHT) : (bHorz ? _2003CLR_EXPBTN_HOVER_LIGHT : _2003CLR_EXPBTN_HOVER_DARK)), bHorz);
  } // if( bHover || bPushed )
  else {
    if (m_bUxApiInitPassed && m_bUxValidColorsExtracted && m_bUxUseIfAvailOnWinXpOrLater) {
      stat_PaintGradientRect(dc, rectButton, GetColor(bHorz ? _2003CLR_EXPBTN_DARK : _2003CLR_EXPBTN_LIGHT), GetColor(bHorz ? _2003CLR_EXPBTN_LIGHT : _2003CLR_EXPBTN_DARK), bHorz);
    }
    else {
      RECT rectButton1(rectButton), rectButton2(rectButton);

      if (bHorz) {
        rectButton1.bottom = rectButton1.top + MulDiv(rectButton1 RCHEIGHT(&), 3, 5);
        rectButton2.top = rectButton1.bottom;
      } // if( bHorz )
      else {
        rectButton1.right = rectButton1.left + MulDiv(rectButton1 RCWIDTH(&), 3, 5);
        rectButton2.left = rectButton1.right;
      } // else from if( bHorz )

      TFillRect(dc, &rectButton2, GetColor(_2003CLR_EXPBTN_DARK));
      stat_PaintGradientRect(dc, rectButton1, GetColor(bHorz ? _2003CLR_EXPBTN_DARK : _2003CLR_EXPBTN_LIGHT), GetColor(bHorz ? _2003CLR_EXPBTN_LIGHT : _2003CLR_EXPBTN_DARK), bHorz);
    }
  } // else from if( bHover || bPushed )

  const glyph_t* pGlyph = NULL, *pGlyph2 = NULL;

  if (bHorz) {
    pGlyph = m_pGlyphTbEpBtnH0;
    pGlyph2 = m_pGlyphTbEpBtnH1;
  } // if( bHorz )
  else {
    pGlyph = m_pGlyphTbEpBtnV0;
    pGlyph2 = m_pGlyphTbEpBtnV1;
  } // else from if( bHorz )

  ASSERT(pGlyph != NULL);
  ASSERT(pGlyph2 != NULL);
  COLORREF ColorValuesLight[] = {
    0, GetColor(_2003CLR_EXPGLYPH_LIGHT)
  };
  COLORREF ColorValuesDark[] = {
    0, GetColor(_2003CLR_EXPGLYPH_DARK)
  };
  RECT rectGlyph(rectButton.TopLeft(), pGlyph->Size());
  RECT rectGlyph2(rectGlyph);
  SIZE sizePushedOffset = GetPushedOffset();

  if (bHorz) {
    int nGap = (max(sizePushedOffset.h, 1)) * 3;
    rectGlyph OffsetRect(&, (rectButton.Size().w - pGlyph->Size().w) / 2 + 1, rectButton.Size().h - pGlyph->Size().h - nGap - 1);
    rectGlyph2 OffsetRect(&, (rectButton.Size().w - pGlyph2->Size().w) / 2 + 1, nGap + 1);
  } // if( bHorz )
  else {
    int nGap = (max(sizePushedOffset.w, 1)) * 3;
    rectGlyph OffsetRect(&, nGap + 1, (rectButton.Size().h - pGlyph->Size().h) / 2 + 1);
    rectGlyph2 OffsetRect(&, rectButton.Size().w - pGlyph2->Size().w - nGap - 1, (rectButton.Size().h - pGlyph2->Size().h) / 2 + 1);
    ASSERT(m_bExpBtnSwapVGlyphs);
    RECT rcTmp(rectGlyph);
    rectGlyph = rectGlyph2;
    rectGlyph2 = rcTmp;
  } // else from if( bHorz )

  rectGlyph OffsetRect(&, 1, 1);
  rectGlyph2 OffsetRect(&, 1, 1);
  PaintGlyph(dc, rectGlyph.TopLeft(), *pGlyph, ColorValuesLight);

  if (!bBarIsCompletelyVisible) {
    PaintGlyph(dc, rectGlyph2.TopLeft(), *pGlyph2, ColorValuesLight);
  }

  rectGlyph OffsetRect(&, - 1, - 1);
  rectGlyph2 OffsetRect(&, - 1, - 1);
  PaintGlyph(dc, rectGlyph.TopLeft(), *pGlyph, ColorValuesDark);

  if (!bBarIsCompletelyVisible) {
    PaintGlyph(dc, rectGlyph2.TopLeft(), *pGlyph2, ColorValuesDark);
  }

  dc.SelectClipRgn(NULL);
}
int stat_ExcludeChildAreas(HDC hDC, HWND hWnd, pfnExcludeChildAreaCallback pCallback, // = NULL
    LPVOID pCallbackCookie // = NULL
                          )
{
  if (hDC == NULL || !IsWindow(hWnd)) {
    return ;
  }

  HWND hWndChild = GetWindow(hWnd, GW_CHILD);

  for (; hWndChild != NULL; hWndChild = GetWindow(hWndChild, GW_HWNDNEXT)) {
    if (!IsWindowVisible(hWndChild)) {
      continue;
    }

    DWORD dwChildStyle = DWORD(GetWindowLong(hWndChild, GWL_STYLE));

    if ((dwChildStyle & WS_CHILD) == 0) {
      continue;
    }

    RECT rc;

    if (!GetWindowRect(hWndChild, &rc)) {
      continue;
    }

    if (!ScreenToClient(hWnd, (LPPOINT)(&rc))) {
      continue;
    }

    if (!ScreenToClient(hWnd, ((LPPOINT)(&rc)) + 1)) {
      continue;
    }

    if (pCallback != NULL) {
      if (pCallback(hDC, hWnd, hWndChild, pCallbackCookie)) {
        continue;
      }
    }

    ExcludeClipRect(hDC, rc.left, rc.top, rc.right, rc.bottom);
  } // for(; hWndChild != NULL; hWndChild = GetWindow(hWndChild,GW_HWNDNEXT))
}
//static volatile BOOL stat_bTrySysTransparentBlt = TRUE;
//static volatile HMODULE stat_hModuleMsimg32 = NULL;
//typedef BOOL (*pMsimg32_TransparentBlt_t)(
// HDC hdcDest, // handle to destination DC
// int nXOriginDest, // x-coord of destination upper-left corner
// int nYOriginDest, // y-coord of destination upper-left corner
// int nWidthDest, // width of destination rectangle
// int hHeightDest, // height of destination rectangle
// HDC hdcSrc, // handle to source DC
// int nXOriginSrc, // x-coord of source upper-left corner
// int nYOriginSrc, // y-coord of source upper-left corner
// int nWidthSrc, // width of source rectangle
// int nHeightSrc, // height of source rectangle
// uint crTransparent // color to make transparent
// );
//static volatile pMsimg32_TransparentBlt_t pMsimg32_TransparentBlt = NULL;
int stat_TransparentBlt(HDC hdcDest, // handle to destination DC
    int nXOriginDest, // x-coord of destination upper-left corner
    int nYOriginDest, // y-coord of destination upper-left corner
    int nWidthDest, // width of destination rectangle
    int hHeightDest, // height of destination rectangle
    HDC hdcSrc, // handle to source DC
    int nXOriginSrc, // x-coord of source upper-left corner
    int nYOriginSrc, // y-coord of source upper-left corner
    int nWidthSrc, // width of source rectangle
    int nHeightSrc, // height of source rectangle
    uint crTransparent // color to make transparent
                       )
{
  ASSERT(hdcDest != NULL && hdcSrc != NULL);
  HDDC* pDcDest = HDDC::FromHandle(hdcDest);
  ASSERT(pDcDest != NULL);
  CExtMemoryDC dcmm(pDcDest), dcmsk(pDcDest, NULL, CExtMemoryDC::MDCOPT_TO_MEMORY | CExtMemoryDC::MDCOPT_FORCE_BLACK);

  if ((nWidthDest != nWidthSrc || hHeightDest != nHeightSrc) && nWidthDest >= 0 && hHeightDest >= 0) {
    VERIFY(::StretchBlt(dcmm, 0, 0, nWidthDest, hHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, SRCCOPY));
  }
  else {
    VERIFY(::BitBlt(dcmm, 0, 0, nWidthSrc, nHeightSrc, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY));
  }

  int nRealWidthDest = (nWidthDest < 0) ? nWidthSrc : nWidthDest;
  int nRealHeightDest = (hHeightDest < 0) ? nHeightSrc : hHeightDest;
  dcmm.SetBkColor(crTransparent);
  VERIFY(dcmsk.BitBlt(0, 0, nRealWidthDest, nRealHeightDest, &dcmm, 0, 0, SRCCOPY));
  dcmm.SetBkColor(RGB(0, 0, 0));
  dcmm.SetTextColor(RGB(255, 255, 255));
  VERIFY(dcmm.BitBlt(0, 0, nRealWidthDest, nRealHeightDest, &dcmsk, 0, 0, SRCAND));
  COLORREF clrBkOld = ::SetBkColor(hdcDest, _RGB(255, 255, 255));
  COLORREF clrTextOld = ::SetTextColor(hdcDest, _RGB(0, 0, 0));
  VERIFY(::BitBlt(hdcDest, nXOriginDest, nYOriginDest, nRealWidthDest, nRealHeightDest, dcmsk, 0, 0, SRCAND));
  VERIFY(::BitBlt(hdcDest, nXOriginDest, nYOriginDest, nRealWidthDest, nRealHeightDest, dcmm, 0, 0, SRCPAINT));
  dcmm.__Flush(FALSE);
  dcmsk.__Flush(FALSE);
  ::SetBkColor(hdcDest, clrBkOld);
  ::SetTextColor(hdcDest, clrTextOld);
}
static const uint stat_pixels_dock_btn_empty[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const uint stat_pixels_dock_btn_close[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const uint stat_pixels_dock_btn_close_dc2k[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const uint stat_pixels_dock_btn_thin_close[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const uint stat_pixels_dock_btn_arrow_up[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const uint stat_pixels_dock_btn_hollow_up[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const uint stat_pixels_dock_btn_arrow_up_l[] = {
  0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const uint stat_pixels_dock_wnd_minimize[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const uint stat_pixels_dock_wnd_restore[] = {
  0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const uint stat_pixels_dock_wnd_maximize[] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};
static const uint stat_pixels_dock_wnd_keep[] = {
  0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const uint stat_pixels_dock_btn_contexthelp[] = {
  0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
};
static const uint stat_pixels_hollow_plus[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const uint stat_pixels_hollow_minus[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const uint stat_pixels_tree_plus[] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};
static const uint stat_pixels_tree_minus[] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};
static const uint stat_pixels_circle_plus[] = {
  0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0,
};
static const uint stat_pixels_circle_minus[] = {
  0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0,
};
static const uint stat_pixels_arrow_pt_up[] = {
  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const uint stat_pixels_arrow_pt_lt[] = {
  1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const uint stat_pixels_arrow_small_up[] = {
  0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1,
};
static const uint stat_pixels_ellipsis[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static glyph_t stat_glyph_dock_empty(9, 9, 2, stat_pixels_dock_btn_empty);
static glyph_t stat_glyph_dock_close(9, 9, 2, stat_pixels_dock_btn_close);
static glyph_t stat_glyph_dock_close_dc2k(9, 9, 2, stat_pixels_dock_btn_close_dc2k);
static glyph_t stat_glyph_dock_xp_thin_close(9, 9, 2, stat_pixels_dock_btn_thin_close);
static glyph_t stat_glyph_dock_arrow_up(9, 9, 2, stat_pixels_dock_btn_arrow_up);
static glyph_t stat_glyph_dock_arrow_down(stat_glyph_dock_arrow_up, 180);
static glyph_t stat_glyph_dock_arrow_left(stat_glyph_dock_arrow_up, 270);
static glyph_t stat_glyph_dock_arrow_right(stat_glyph_dock_arrow_left, 180);
static glyph_t stat_glyph_dock_wnd_minimize(9, 9, 2, stat_pixels_dock_wnd_minimize);
static glyph_t stat_glyph_dock_wnd_restore(9, 9, 2, stat_pixels_dock_wnd_restore);
static glyph_t stat_glyph_dock_wnd_maximize(9, 9, 2, stat_pixels_dock_wnd_maximize);
static glyph_t stat_glyph_dock_wnd_keep(9, 9, 2, stat_pixels_dock_wnd_keep);
static glyph_t stat_glyph_dock_wnd_unkeep(stat_glyph_dock_wnd_keep, 90);
static glyph_t stat_glyph_dock_wnd_contexthelp(9, 9, 2, stat_pixels_dock_btn_contexthelp);
static glyph_t stat_glyph_dock_arrow_up_l(9, 9, 2, stat_pixels_dock_btn_arrow_up_l);
static glyph_t stat_glyph_dock_arrow_down_l(stat_glyph_dock_arrow_up_l, 180);
static glyph_t stat_glyph_dock_arrow_left_l(stat_glyph_dock_arrow_up_l, 270);
static glyph_t stat_glyph_dock_arrow_right_l(stat_glyph_dock_arrow_left_l, 180);
static glyph_t stat_glyph_dock_hollow_up(9, 9, 2, stat_pixels_dock_btn_hollow_up);
static glyph_t stat_glyph_dock_hollow_down(stat_glyph_dock_hollow_up, 180);
static glyph_t stat_glyph_dock_hollow_left(stat_glyph_dock_hollow_up, 270);
static glyph_t stat_glyph_dock_hollow_right(stat_glyph_dock_hollow_left, 180);
static glyph_t stat_glyph_hollow_plus(9, 9, 2, stat_pixels_hollow_plus);
static glyph_t stat_glyph_hollow_minus(9, 9, 2, stat_pixels_hollow_minus);
static glyph_t stat_glyph_tree_plus(9, 9, 2, stat_pixels_tree_plus);
static glyph_t stat_glyph_tree_minus(9, 9, 2, stat_pixels_tree_minus);
static glyph_t stat_glyph_circle_plus(9, 9, 2, stat_pixels_circle_plus);
static glyph_t stat_glyph_circle_minus(9, 9, 2, stat_pixels_circle_minus);
static glyph_t stat_glyph_arrow_pt_up(11, 11, 2, stat_pixels_arrow_pt_up);
static glyph_t stat_glyph_arrow_pt_down(stat_glyph_arrow_pt_up, 180);
static glyph_t stat_glyph_arrow_pt_left(stat_glyph_arrow_pt_up, 270);
static glyph_t stat_glyph_arrow_pt_right(stat_glyph_arrow_pt_left, 180);
static glyph_t stat_glyph_arrow_small_up(5, 3, 2, stat_pixels_arrow_small_up);
static glyph_t stat_glyph_arrow_small_down(stat_glyph_arrow_small_up, 180);
static glyph_t stat_glyph_arrow_small_left(stat_glyph_arrow_small_up, 270);
static glyph_t stat_glyph_arrow_small_right(stat_glyph_arrow_small_left, 180);
static glyph_t stat_glyph_ellipsis(9, 3, 2, stat_pixels_ellipsis);
static glyph_t stat_glyph_arrow_pt_lt(11, 11, 2, stat_pixels_arrow_pt_lt);
static glyph_t stat_glyph_arrow_pt_rt(stat_glyph_arrow_pt_lt, 90);
static glyph_t stat_glyph_arrow_pt_rb(stat_glyph_arrow_pt_rt, 90);
static glyph_t stat_glyph_arrow_pt_lb(stat_glyph_arrow_pt_rb, 90);
/*
static const uint stat_pixels_arrow_sort_up[] =
{
0,0,0,1,1,0,0,0,
0,0,1,3,1,1,0,0,
0,0,3,0,0,1,0,0,
0,1,3,0,0,1,1,0,
0,3,0,0,0,0,1,0,
1,3,0,0,0,0,1,1,
1,2,2,2,2,2,2,2,
};
static const uint stat_pixels_arrow_sort_down[] =
{
2,3,3,3,3,3,3,3,
2,2,0,0,0,0,1,1,
0,2,0,0,0,0,3,0,
0,2,2,0,0,1,1,0,
0,0,2,0,0,3,0,0,
0,0,2,2,1,1,0,0,
0,0,0,2,1,0,0,0,
};
 */
static const uint stat_pixels_arrow_sort_up[] = {
  0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 3, 1, 1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 3, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2,
};
static const uint stat_pixels_arrow_sort_down[] = {
  3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 2, 2, 0, 3, 0, 0, 0, 0, 0, 0, 2, 0, 0, 1, 1, 0, 0, 0, 0, 2, 2, 0, 0, 0, 3, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1, 1, 0, 0, 2, 2, 0, 0, 0, 0, 0, 3, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1, 3, 2, 2, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0,
};
static glyph_t stat_glyph_arrow_sort_up(10, 10, 4, // 8,7,4,
    stat_pixels_arrow_sort_up);
static glyph_t stat_glyph_arrow_sort_down(10, 10, 4, // 8,7,4,
    stat_pixels_arrow_sort_down);
static glyph_t stat_glyph_arrow_sort_left(stat_glyph_arrow_sort_down, 90);
static glyph_t stat_glyph_arrow_sort_right(stat_glyph_arrow_sort_up, 90);
glyph_t* g_DockingCaptionGlyphs[__DCBT_VALUE_MAX + 1] = {
  &stat_glyph_dock_empty, &stat_glyph_dock_close, &stat_glyph_dock_arrow_up, &stat_glyph_dock_arrow_down, &stat_glyph_dock_arrow_left, &stat_glyph_dock_arrow_right, &stat_glyph_dock_wnd_minimize, &stat_glyph_dock_wnd_restore, &stat_glyph_dock_wnd_maximize, &stat_glyph_dock_wnd_contexthelp, &stat_glyph_dock_wnd_keep, &stat_glyph_dock_wnd_unkeep, &stat_glyph_dock_arrow_up_l, &stat_glyph_dock_arrow_down_l, &stat_glyph_dock_arrow_left_l, &stat_glyph_dock_arrow_right_l, &stat_glyph_dock_xp_thin_close, &stat_glyph_dock_hollow_up, &stat_glyph_dock_hollow_down, &stat_glyph_dock_hollow_left, &stat_glyph_dock_hollow_right, &stat_glyph_dock_close_dc2k, &stat_glyph_hollow_plus, &stat_glyph_hollow_minus, &stat_glyph_tree_plus, &stat_glyph_tree_minus, &stat_glyph_circle_plus, &stat_glyph_circle_minus, &stat_glyph_arrow_pt_up, &stat_glyph_arrow_pt_down, &stat_glyph_arrow_pt_left, &stat_glyph_arrow_pt_right, &stat_glyph_arrow_pt_lt, &stat_glyph_arrow_pt_rt, &stat_glyph_arrow_pt_rb, &stat_glyph_arrow_pt_lb, &stat_glyph_arrow_small_up, &stat_glyph_arrow_small_down, &stat_glyph_arrow_small_left, &stat_glyph_arrow_small_right, &stat_glyph_ellipsis, &stat_glyph_arrow_sort_up, &stat_glyph_arrow_sort_down, &stat_glyph_arrow_sort_left, &stat_glyph_arrow_sort_right,
};
PAINTDOCKINGCAPTIONBUTTONDATA::PAINTDOCKINGCAPTIONBUTTONDATA(): m_pHelperSrc(NULL), m_lParam(0L), m_rcClient(0, 0, 0, 0), m_eType(e_docking_caption_button_t::__DCBT_EMPTY), m_bHover(FALSE), m_bPushed(FALSE), m_bEnabled(FALSE), m_bSideBar(FALSE), m_bBarWndActive(FALSE), m_bFloating(FALSE) {}
PAINTDOCKINGCAPTIONBUTTONDATA::PAINTDOCKINGCAPTIONBUTTONDATA(CObject* pHelperSrc, const RECT* rcClient, int eType, // e_docking_caption_button_t
    BOOL bHover, BOOL bPushed, BOOL bEnabled, BOOL bSideBar, BOOL bBarWndActive, BOOL bFloating): m_pHelperSrc(pHelperSrc), m_lParam(0L), m_rcClient(rcClient), m_eType(eType), m_bHover(bHover), m_bPushed(bPushed), m_bEnabled(bEnabled), m_bSideBar(bSideBar), m_bBarWndActive(bBarWndActive), m_bFloating(bFloating) {}
int PaintDockingCaptionButton(HDDC dc, PAINTDOCKINGCAPTIONBUTTONDATA& _pdcbd)
{
  ASSERT(dc != NULL);
  ASSERT(__DCBT_VALUE_MIN <= _pdcbd.m_eType && _pdcbd.m_eType <= __DCBT_VALUE_MAX);

  if (_pdcbd.m_rcClient IsRectEmpty(&)) {
    return ;
  }

  BOOL bHover = _pdcbd.m_bHover;

  if (!_pdcbd.m_bEnabled) {
    bHover = FALSE;
  }

  COLORREF ColorValues[2] = {
    _RGB(0, 0, 0), _RGB(0, 0, 0),
  };
  COLORREF clr3dFace = GetColor(COLOR_3DFACE);
  COLORREF clr3dShadow = GetColor(COLOR_3DSHADOW);
  COLORREF clr3dDkShadow = GetColor(COLOR_3DDKSHADOW);
  COLORREF clr3dHiLight = GetColor(COLOR_3DHILIGHT);

  if (!_pdcbd.m_bEnabled) {
    ColorValues[1] = clr3dShadow;
  }

  RECT rcGlyph(_pdcbd.m_rcClient);
  rcGlyph.NormalizeRect();

  if (rcGlyph RCWIDTH(&) & 1) {
    rcGlyph.left++;
  }

  if (rcGlyph RCHEIGHT(&) & 1) {
    rcGlyph.top++;
  }

  if (_pdcbd.m_eType == __DCBT_CLOSE) {
    _pdcbd.m_eType = __DCBT_CLOSE_DC2K;
  }

  glyph_t* pGlyph = g_DockingCaptionGlyphs[_pdcbd.m_eType];
  ASSERT(pGlyph != NULL);

  if (!_pdcbd.m_bFloating)
    // rcGlyph InflateRect(&, 2, 2 );
    // else
  {
    rcGlyph InflateRect(&, 1, 1);
  }

  rcGlyph InflateRect(&, 1, 1, 0, 0);
  TFillRect(dc, &rcGlyph, clr3dFace);
  RECT rcGlyphShape(rcGlyph);

  if (_pdcbd.m_bPushed) {
    rcGlyphShape OffsetRect(&, 1, 1);
  }

  PaintGlyphCentered(dc, &rcGlyphShape, *pGlyph, ColorValues);

  /*
  if( bEnabled && bPushed )
  {
  TDraw3DRect(dc,&
  &rcGlyph,
  clr3dDkShadow, clr3dFace
  );
  rcGlyph DeflateRect(&, 1, 1 );
  TDraw3DRect(dc,&
  &rcGlyph,
  clr3dShadow, clr3dHiLight
  );
  } // if( bPushed )
  else
  {
  TDraw3DRect(dc,&
  &rcGlyph,
  clr3dFace, clr3dDkShadow
  );
  rcGlyph DeflateRect(&, 1, 1 );
  TDraw3DRect(dc,&
  &rcGlyph,
  clr3dHiLight, clr3dShadow
  );
  } // else if( bHover )
  */
  if (_pdcbd.m_bEnabled && _pdcbd.m_bPushed) {
    TDraw3DRect(dc, &rcGlyph, clr3dShadow, clr3dHiLight);
  } // if( _pdcbd.m_bEnabled && _pdcbd.m_bPushed )
  else if (_pdcbd.m_bEnabled && bHover) {
    TDraw3DRect(dc, &rcGlyph, clr3dFace, clr3dDkShadow);
    rcGlyph DeflateRect(&, 1, 1);
    TDraw3DRect(dc, &rcGlyph, clr3dHiLight, clr3dShadow);
  } // else if( _pdcbd.m_bEnabled && bHover )
  else {
    TDraw3DRect(dc, &rcGlyph, clr3dHiLight, clr3dShadow);
  } // else ...
}
int CExtPaintManagerXP::PaintDockingCaptionButton(HDDC dc, PAINTDOCKINGCAPTIONBUTTONDATA& _pdcbd)
{
  ASSERT(dc != NULL);
  ASSERT(__DCBT_VALUE_MIN <= _pdcbd.m_eType && _pdcbd.m_eType <= __DCBT_VALUE_MAX);

  if (_pdcbd.m_rcClient IsRectEmpty(&)) {
    return ;
  }

  BOOL bHover = _pdcbd.m_bHover;

  if (!_pdcbd.m_bEnabled) {
    bHover = FALSE;
  }

  COLORREF ColorValues[2] = {
    _RGB(0, 0, 0), _RGB(0, 0, 0),
  };

  if (_pdcbd.m_bEnabled) {
    if (!(bHover || _pdcbd.m_bPushed))
      ColorValues[1] = GetColor(_pdcbd.m_bSideBar ? (_pdcbd.m_bBarWndActive ? COLOR_HIGHLIGHTTEXT : COLOR_3DDKSHADOW // COLOR_BTNTEXT
                                                    ) : COLOR_3DHILIGHT);
  } // if( bEnabled )
  else {
    ColorValues[1] = GetColor(COLOR_3DSHADOW // COLOR_3DFACE
                             );
  } // else from if( bEnabled )

  RECT rcGlyph(_pdcbd.m_rcClient);
  rcGlyph.NormalizeRect();

  if (bHover || _pdcbd.m_bPushed) {
    TFillRect(dc, &rcGlyph,
        //clr3dHilight
        GetColor(_pdcbd.m_bPushed ? XPCLR_HILIGHT : CLR_3DFACE_IN));
    RECT rcBtnBorder(rcGlyph);
    rcBtnBorder InflateRect(&, 1, 1);
    COLORREF clrBtnBorder = GetColor(_pdcbd.m_bBarWndActive ? XPCLR_HILIGHT : XPCLR_HILIGHT_BORDER);
    TDraw3DRect(dc, &rcBtnBorder, clrBtnBorder, clrBtnBorder);
  }

  if (rcGlyph RCWIDTH(&) & 1) {
    rcGlyph.left++;
  }

  if (rcGlyph RCHEIGHT(&) & 1) {
    rcGlyph.top++;
  }

  int eType = _pdcbd.m_eType;

  if (_pdcbd.m_bSideBar && !_pdcbd.m_bFloating && eType == __DCBT_CLOSE) {
    eType = __DCBT_XP_THIN_CLOSE;
  }

  glyph_t* pGlyph = g_DockingCaptionGlyphs[eType];
  ASSERT(pGlyph != NULL);
  PaintGlyphCentered(dc, &rcGlyph, *pGlyph, ColorValues);
}
int CExtPaintManagerXP::PaintMenuSeparator(HDDC dc, const RECT* rectItem, int nIconAreaWidth, BOOL bRarelyUsed, CObject* pHelperSrc, LPARAM lParam // = 0L
                                          )
{
  pHelperSrc;
  lParam;
  nIconAreaWidth;

  if (::IsRectEmpty(&rectItem)) {
    return ;
  }

  COLORREF clr3dFace = // GetColor(CLR_3DFACE_OUT);
      GetColor(bRarelyUsed ? XPCLR_RARELY_BORDER : CLR_3DFACE_OUT);
  RECT rc(rectItem), rcBottomFill(rectItem);
  rcBottomFill.right = rcBottomFill.left + nIconAreaWidth;
  TFillRect(dc, rcBottomFill, clr3dFace);
  COLORREF clrGradientLeft = ((COLORREF)(- 1L)), clrGradientRight((COLORREF)(- 1L));
  BOOL b2StepGradient = FALSE;

  if (bRarelyUsed) {
    if (m_nIdxClrMlaRarelyLeft >= 0 && m_nIdxClrMlaRarelyRight >= 0 && stat_GetBPP() > 8) {
      // if paint gradient on rarely used item's left part
      clrGradientLeft = GetColor(m_nIdxClrMlaRarelyLeft);
      clrGradientRight = GetColor(m_nIdxClrMlaRarelyRight);
    } // if paint gradient on rarely used item's left part
  } // if( bRarelyUsed )
  else {
    if ((!(m_bUxApiInitPassed && m_bUxValidColorsExtracted && m_bUxUseIfAvailOnWinXpOrLater))) {
      b2StepGradient = TRUE;
    }

    if (m_nIdxClrMlaNormLeft >= 0 && m_nIdxClrMlaNormRight >= 0 && stat_GetBPP() > 8) {
      // if paint gradient on rarely used item's left part
      clrGradientLeft = GetColor(m_nIdxClrMlaNormLeft);
      clrGradientRight = GetColor(m_nIdxClrMlaNormRight);
    } // if paint gradient on rarely used item's left part
  } // else from if( bRarelyUsed )

  if (clrGradientLeft != ((COLORREF)(- 1L))) {
    ASSERT(clrGradientRight != ((COLORREF)(- 1L)));
    RECT rcGradient(rectItem);
    rcGradient.right = rcGradient.left + nIconAreaWidth;

    if (b2StepGradient) {
      RECT rcGradient1(rcGradient), rcGradient2(rcGradient);
      rcGradient1.right = rcGradient1.left + MulDiv(rcGradient1 RCWIDTH(&), 7, 10);
      rcGradient2.left = rcGradient1.right;
      TFillRect(dc, &rcGradient2, clrGradientRight);
      stat_PaintGradientRect(dc, &rcGradient1, clrGradientLeft, clrGradientRight, FALSE);
    } // if( b2StepGradient )
    else {
      stat_PaintGradientRect(dc, &rcGradient, clrGradientLeft, clrGradientRight, FALSE);
    }
  } // if( clrGradientLeft != ((COLORREF)(-1L)) )

  rc.left += nIconAreaWidth;
  TFillRect(dc, rc, GetColor(XPCLR_3DFACE_NORMAL));
  rc.left += __EXT_MENU_GAP;
  rc.top += rc RCHEIGHT(&) / 2;
  rc.bottom = rc.top + 1;
  TFillRect(dc, rc, clr3dFace);
}
int CExtPaintManagerXP::PaintSysCmdGlyph(HDDC dc, uint nHelperSysCmdID, const RECT* rcItem, BOOL bHover, BOOL bPushed, BOOL bEnabled)
{
  e_docking_caption_button_t eGlyph = __DCBT_EMPTY;

  switch (nHelperSysCmdID) {
  case SC_CLOSE:
    eGlyph = __DCBT_CLOSE;
    break;

  case SC_RESTORE:
    eGlyph = __DCBT_WND_RESTORE;
    break;

  case SC_MINIMIZE:
    eGlyph = __DCBT_WND_MINIMIZE;
    break;

  case SC_MAXIMIZE:
    eGlyph = __DCBT_WND_MAXIMIZE;
    break;

  case SC_CONTEXTHELP:
    eGlyph = __DCBT_WND_CONTEXTHELP;
    break;
  } // switch( nHelperSysCmdID )

  if (eGlyph == __DCBT_EMPTY) {
    return ;
  }

  COLORREF ColorValues[2] = {
    _RGB(0, 0, 0), _RGB(0, 0, 0),
  };
  RECT _rcItem(rcItem);

  if (bPushed) {
    _rcItem OffsetRect(&, GetPushedOffset());
  }

  if (bEnabled) {
    if (m_bEnabledHoverIconShadows && bHover && (!bPushed)) {
      // COLOR_3DDKSHADOW changed to COLOR_3DSHADOW
      // by the Dmitry Yakovlev's advice
      ColorValues[1] = GetColor(COLOR_3DSHADOW // COLOR_3DDKSHADOW
                               );
      PaintGlyphCentered(dc, &_rcItem, *g_DockingCaptionGlyphs[eGlyph], ColorValues);
      // corrected by the Dmitry Yakovlev's advice
      _rcItem OffsetRect(&, - 2, - 2);
    } // if( m_bEnabledHoverIconShadows && bHover && (!bPushed) )

    ColorValues[1] = GetColor(CLR_TEXT_OUT);
  } // if( bEnabled )
  else {
    ColorValues[1] = GetColor(COLOR_3DSHADOW);
  }

  PaintGlyphCentered(dc, &_rcItem, *g_DockingCaptionGlyphs[eGlyph], ColorValues);
}
int CExtPaintManagerXP::PaintMenuItem(HDDC dc, PAINTMENUITEMDATA& _pmid)
{
  if (IsRectEmpty(&_pmid->m_rcItem)) {
    return ;
  }

  CExtSafeString _sText = (_pmid->m_sText == NULL) ? _T("") : _pmid->m_sText;
  CExtSafeString _sAccelText = (_pmid->m_sAccelText == NULL) ? _T("") : _pmid->m_sAccelText;
  COLORREF clr3dFace = // GetColor( CLR_3DFACE_OUT );
      GetColor(_pmid->m_bRarelyUsed ? XPCLR_RARELY_BORDER : CLR_3DFACE_OUT);
  COLORREF clrDkBorder = GetColor(XPCLR_HILIGHT_BORDER);
  COLORREF clrBk = clr3dFace;
  COLORREF clrText = GetColor(CLR_TEXT_OUT);
  COLORREF clrBkHilight = GetColor(XPCLR_HILIGHT);
  COLORREF clrIconBkHilight = _pmid->m_bSelected ? clrBkHilight : GetColor(CLR_3DFACE_IN);
  COLORREF clrRight = GetColor(XPCLR_3DFACE_NORMAL);
  RECT rcBk(_pmid->m_rcItem), rcSelBk(_pmid->m_rcItem);
  {
    // BLOCK: paint left area
    COLORREF clrGradientLeft = ((COLORREF)(- 1L)), clrGradientRight((COLORREF)(- 1L));
    BOOL b2StepGradient = FALSE;

    if (stat_GetBPP() > 8) {
      if (_pmid->m_bRarelyUsed) {
        if (m_nIdxClrMlaRarelyLeft >= 0 && m_nIdxClrMlaRarelyRight >= 0) {
          // if paint gradient on rarely used item's left part
          clrGradientLeft = GetColor(m_nIdxClrMlaRarelyLeft);
          clrGradientRight = GetColor(m_nIdxClrMlaRarelyRight);
        } // if paint gradient on rarely used item's left part
      } // if( _pmid->m_bRarelyUsed )
      else {
        if ((!(m_bUxApiInitPassed && m_bUxValidColorsExtracted && m_bUxUseIfAvailOnWinXpOrLater))) {
          b2StepGradient = TRUE;
        }

        if (m_nIdxClrMlaNormLeft >= 0 && m_nIdxClrMlaNormRight >= 0) {
          // if paint gradient on rarely used item's left part
          clrGradientLeft = GetColor(m_nIdxClrMlaNormLeft);
          clrGradientRight = GetColor(m_nIdxClrMlaNormRight);
        } // if paint gradient on rarely used item's left part
      } // else from if( _pmid->m_bRarelyUsed )
    } // if( stat_GetBPP() > 8 )

    RECT rcGradient(rcBk);
    rcGradient.right = rcGradient.left + _pmid->m_nIconAreaWidth;

    if ((!_pmid->m_bForceNoLeftGradient) && clrGradientLeft != ((COLORREF)(- 1L))) {
      ASSERT(clrGradientRight != ((COLORREF)(- 1L)));

      if (b2StepGradient) {
        RECT rcGradient1(rcGradient), rcGradient2(rcGradient);
        rcGradient1.right = rcGradient1.left + MulDiv(rcGradient1 RCWIDTH(&), 7, 10);
        rcGradient2.left = rcGradient1.right;
        TFillRect(dc, &rcGradient2, clrGradientRight);
        stat_PaintGradientRect(dc, &rcGradient1, clrGradientLeft, clrGradientRight, FALSE);
      } // if( b2StepGradient )
      else {
        stat_PaintGradientRect(dc, &rcGradient, clrGradientLeft, clrGradientRight, FALSE);
      }
    } // if( (!_pmid->m_bForceNoLeftGradient) && clrGradientLeft != ((COLORREF)(-1L)) )
    else {
      TFillRect(dc, &rcGradient, clrBk);
    }

    RECT rcFillRightPart(rcBk);

    if ((!_pmid->m_bForceNoLeftGradient)) {
      rcFillRightPart.left += _pmid->m_nIconAreaWidth;
    }

    TFillRect(dc, &rcFillRightPart, clrRight);
  } // BLOCK: paint left area

  if (_pmid->m_bSelected) {
    ASSERT(_pmid->m_bEnabled);
    rcSelBk.bottom -= __EXT_MENU_VERT_DISTANCE_NORM;
    clrBk = GetColor(CLR_3DFACE_IN);
    TFillRect(dc, &rcSelBk, clrBk);
    TDraw3DRect(dc, &rcSelBk, clrDkBorder, clrDkBorder);
  } // if( _pmid->m_bSelected )

  if ((!_pmid->m_bForceNoLeftGradient)) {
    rcBk.left += _pmid->m_nIconAreaWidth;
  }

  rcBk DeflateRect(&, __EXT_MENU_GAP, __EXT_MENU_IMG_RECT_OUT_GAP - 1);
  COLORREF clrOldText = textColor = (clrText);
  int nOldBkMode = dc.SetBkMode(TRANSPARENT);
  HFONT pOldFont = dc.SelectObject(m_FontNormal);
  ASSERT(pOldFont != NULL);
  RECT rcMeasureText(0, 0, 0, 0);
  TDrawText(dc, _sText, _sText.GetLength(), &rcMeasureText, DT_CALCRECT | DT_SINGLELINE | DT_LEFT);
  RECT rcDrawText(rcBk);
  rcDrawText OffsetRect(&, 0, (rcDrawText RCHEIGHT(&) - rcMeasureText RCHEIGHT(&)) / 2);
  dc.DrawState(rcDrawText.TopLeft(), rcDrawText.Size(), _sText, (_pmid->m_bEnabled ? DSS_NORMAL : m_DSS_DISABLED_style), TRUE, 0, _pmid->m_bEnabled ? (COLORREF*)NULL : COLORREF::FromHandle(GetBrush(COLOR_3DSHADOW)));

  if (!_sAccelText) {
    rcMeasureText.SetRect(0, 0, 0, 0);
    TDrawText(dc, _sAccelText, _sAccelText.GetLength(), &rcMeasureText, DT_CALCRECT | DT_SINGLELINE | DT_LEFT);
    rcDrawText = rcBk;
    rcDrawText OffsetRect(&, 0, (rcDrawText RCHEIGHT(&) - rcMeasureText RCHEIGHT(&)) / 2);
    rcDrawText.left = rcDrawText.right - rcMeasureText RCWIDTH(&) - __EXT_MENU_POPUP_ARROW_AREA_DX;
    rcBk.left = rcDrawText.left;
    dc.DrawState(rcDrawText.TopLeft(), rcDrawText.Size(), _sAccelText, (_pmid->m_bEnabled ? DSS_NORMAL : m_DSS_DISABLED_style), TRUE, 0, _pmid->m_bEnabled ? (COLORREF*)NULL : COLORREF::FromHandle(GetBrush(COLOR_3DSHADOW)));
  } // if( !_sAccelText )

  if (_pmid->m_bPopup) {
    RECT rcPopupArrow(_pmid->m_rcItem);
    rcPopupArrow DeflateRect(&, __EXT_MENU_GAP, __EXT_MENU_GAP);
    rcPopupArrow.left = rcPopupArrow.right - __EXT_MENU_POPUP_ARROW_AREA_DX;

    if (_pmid->m_bCustomizeListMode) {
      COLORREF ColorValues[2] = {
        _RGB(0, 0, 0), GetColor(CLR_TEXT_OUT)
      };
      PaintGlyphCentered(dc, &rcPopupArrow, _pmid->m_bSeparatedDropDown ? &g_glyph_customize_dropdown_src : &g_glyph_customize_popup_src, ColorValues);
    } // if( _pmid->m_bCustomizeListMode )
    else {
      dc.SelectObject(m_FontMarlett);
      TDrawText(dc, _T("8"), // popup rightArrow
          rcPopupArrow, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    } // else from if( _pmid->m_bCustomizeListMode )
  } // if( _pmid->m_bPopup )

  dc.SelectObject(&pOldFont);
  dc.SetBkMode(nOldBkMode);
  textColor = (clrOldText);
  SIZE sizeIcon(__EXT_MENU_ICON_CX, __EXT_MENU_ICON_CY);

  if (_pmid->m_pIcon != NULL && (!_pmid->m_pIcon->IsEmpty())) {
    sizeIcon = _pmid->m_pIcon->GetSize();

    if (sizeIcon.w <= 18 && sizeIcon.h < sizeIcon.w) {
      sizeIcon.h = sizeIcon.w;
    }
  }

  RECT rcIcon(_pmid->m_rcItem.TopLeft(), sizeIcon);

  if (_pmid->m_bXtraMark) {
    rcIcon OffsetRect(&, (_pmid->m_nIconAreaWidth / 2 - RCWIDTH(&rcIcon)) / 2 + _pmid->m_nIconAreaWidth / 2 - 2, (_pmid->m_rcItem RCHEIGHT(&) - RCHEIGHT(&rcIcon)) / 2);

    if (_pmid->m_bXtraChecked) {
      RECT rcXtraMark(_pmid->m_rcItem.left + 1, _pmid->m_rcItem.top + 1, _pmid->m_rcItem.left + _pmid->m_rcItem RCHEIGHT(&) - 2, _pmid->m_rcItem.bottom - 2);
      int nIdxMainGradientLeft = - 1;
      int nIdxMainGradientRight = - 1;

      if (_pmid->m_bSelected) {
        nIdxMainGradientLeft = m_nIdxClrBtnHovPresLeft;
        nIdxMainGradientRight = m_nIdxClrBtnHovPresRight;
      } // if( _pmid->m_bSelected )
      else {
        nIdxMainGradientLeft = m_nIdxClrBtnPressedLeft;
        nIdxMainGradientRight = m_nIdxClrBtnPressedRight;
      } // else if( _pmid->m_bSelected )

      if (nIdxMainGradientLeft >= 0 && nIdxMainGradientRight >= 0) {
        stat_PaintGradientRect(dc, rcXtraMark, GetColor(nIdxMainGradientRight), GetColor(nIdxMainGradientLeft), TRUE);
      } // if( nIdxMainGradientLeft >= 0 && nIdxMainGradientRight >= 0 )
      else {
        TFillRect(dc, rcXtraMark, clrIconBkHilight);
      } // else from if( nIdxMainGradientLeft >= 0 && nIdxMainGradientRight >= 0 )

      COLORREF clrCheck = GetColor(CLR_TEXT_OUT);
      int x = rcXtraMark.left + rcXtraMark RCWIDTH(&) / 2 - 3;
      int y = rcXtraMark.top + rcXtraMark RCHEIGHT(&) / 2 - 3;
      ASSERT(!_pmid->m_bPushedRadio);
      stat_PaintCheckMark(dc, x, y, clrCheck);
      TDraw3DRect(dc, &rcXtraMark, clrDkBorder, clrDkBorder);
    } // if( _pmid->m_bXtraChecked )
  } // if( _pmid->m_bXtraMark )
  else {
    rcIcon OffsetRect(&, (_pmid->m_nIconAreaWidth - RCWIDTH(&rcIcon)) / 2, (_pmid->m_rcItem RCHEIGHT(&) - RCHEIGHT(&rcIcon)) / 2);
  } // else from if( _pmid->m_bXtraMark )

  RECT rcIconFrame(rcIcon);
  rcIconFrame InflateRect(&, __EXT_MENU_IMG_RECT_OUT_GAP, __EXT_MENU_IMG_RECT_OUT_GAP);

  if (_pmid->m_bPushedCheck || _pmid->m_bPushedRadio) {
    int nIdxMainGradientLeft = - 1;
    int nIdxMainGradientRight = - 1;

    if (_pmid->m_bSelected) {
      nIdxMainGradientLeft = m_nIdxClrBtnHovPresLeft;
      nIdxMainGradientRight = m_nIdxClrBtnHovPresRight;
    } // if( _pmid->m_bSelected )
    else {
      nIdxMainGradientLeft = m_nIdxClrBtnPressedLeft;
      nIdxMainGradientRight = m_nIdxClrBtnPressedRight;
    } // else if( _pmid->m_bSelected )

    if (nIdxMainGradientLeft >= 0 && nIdxMainGradientRight >= 0) {
      stat_PaintGradientRect(dc, rcIconFrame, GetColor(nIdxMainGradientRight), GetColor(nIdxMainGradientLeft), TRUE);
    } // if( nIdxMainGradientLeft >= 0 && nIdxMainGradientRight >= 0 )
    else {
      TFillRect(dc, rcIconFrame, clrIconBkHilight);
    } // else from if( nIdxMainGradientLeft >= 0 && nIdxMainGradientRight >= 0 )
  }

  if (_pmid->m_pIcon != NULL && (!_pmid->m_pIcon->IsEmpty())) {
    RECT rcDummy;
    CExtSafeString sTitle(_T(""));
    PaintIcon(dc, TRUE, sTitle, _pmid->m_pIcon, rcIcon, rcDummy, _pmid->m_bPushedCheck || _pmid->m_bPushedRadio, _pmid->m_bEnabled, _pmid->m_bSelected, 0);
  } // if( _pmid->m_pIcon != NULL && (! _pmid->m_pIcon->IsEmpty()) )
  else {
    PaintSysCmdGlyph(dc, _pmid->m_nHelperSysCmdID, rcIcon, _pmid->m_bSelected, FALSE, _pmid->m_bEnabled);
  } // else from if( _pmid->m_pIcon != NULL && (! _pmid->m_pIcon->IsEmpty()) )

  if (_pmid->m_bPushedCheck || _pmid->m_bPushedRadio) {
    if (_pmid->m_pIcon == NULL || _pmid->m_pIcon->IsEmpty()) {
      COLORREF clrCheck = GetColor(CLR_TEXT_OUT);
      int x = rcIcon.left + RCWIDTH(&rcIcon) / 2 - 3;
      int y = rcIcon.top + RCHEIGHT(&rcIcon) / 2 - 3;

      if (_pmid->m_bPushedCheck) {
        ASSERT(!_pmid->m_bPushedRadio);
        stat_PaintCheckMark(dc, x, y, clrCheck);
      }
      else {
        ASSERT(!_pmid->m_bPushedCheck);
        stat_PaintRadioDot(dc, x, y, clrCheck);
      }
    } // if( _pmid->m_pIcon == NULL || _pmid->m_pIcon->IsEmpty() )

    TDraw3DRect(dc, &rcIconFrame, clrDkBorder, clrDkBorder);
  }

  if (!_pmid->m_rcInPlaceWnd IsRectEmpty(&)) {
    RECT rcInPlaceWndOuterBorder(_pmid->m_rcInPlaceWnd);
    rcInPlaceWndOuterBorder InflateRect(&, 1, 2);

    if (_pmid->m_bPopup) {
      INT nDD = (INT)::GetSystemMetrics(SM_CXHTHUMB);
      rcInPlaceWndOuterBorder.right += nDD;
      rcInPlaceWndOuterBorder InflateRect(&, 1, 1);
      PAINTCOMBOFRAMEDATA _pcfd(_pmid->m_pHelperSrc, rcInPlaceWndOuterBorder, _pmid->m_bSelected, // hover
          _pmid->m_bSelected && _pmid->m_bHelperPopupDropped, // pushed
          _pmid->m_bEnabled);
      _pcfd->m_bNoTopOuterBorder = TRUE;
      PaintComboFrame(dc, _pcfd);
      rcInPlaceWndOuterBorder DeflateRect(&, 1, 1);

      if (_pmid->m_bSelected) {
        TDraw3DRect(dc, &rcInPlaceWndOuterBorder, clrDkBorder, clrDkBorder);
      } // if( _pmid->m_bSelected )
      else {
        COLORREF clrBorder = GetColor(CLR_3DFACE_OUT);
        TDraw3DRect(dc, &rcInPlaceWndOuterBorder, clrBorder, clrBorder);
        COLORREF clrWindow = GetColor(COLOR_WINDOW);
        RECT rcBtn(rcInPlaceWndOuterBorder);
        rcBtn.left = rcBtn.right - nDD;
        rcBtn DeflateRect(&, 0, 1, 1, 1);
        TDraw3DRect(dc, &rcBtn, clrWindow, clrWindow);
      } // else from if( _pmid->m_bSelected )

      rcInPlaceWndOuterBorder.right -= nDD;
    } // if( _pmid->m_bPopup )
    else {
      COLORREF clr = _pmid->m_bSelected ? clrDkBorder : GetColor(COLOR_3DLIGHT);
      TDraw3DRect(dc, &rcInPlaceWndOuterBorder, clr, clr);
    } // else from if( _pmid->m_bPopup )

    rcInPlaceWndOuterBorder DeflateRect(&, 1, 1);
    TFillRect(dc, &rcInPlaceWndOuterBorder, GetColor(_pmid->m_bEnabled ? COLOR_WINDOW : COLOR_3DFACE));

    if (!_pmid->m_strInPlaceText) {
      rcInPlaceWndOuterBorder DeflateRect(&, 5, 3, 1, 2);
      COLORREF clrOldText = textColor = (GetColor(COLOR_BTNTEXT));
      int nOldBkMode = dc.SetBkMode(TRANSPARENT);
      HFONT pOldFont = dc.SelectObject(m_FontNormal);
      ASSERT(pOldFont != NULL);
      TDrawText(dc, _pmid->m_strInPlaceText, _pmid->m_strInPlaceText.GetLength(), &rcInPlaceWndOuterBorder, DT_SINGLELINE | DT_NOPREFIX | DT_LEFT | DT_TOP);
      dc.SelectObject(&pOldFont);
      dc.SetBkMode(nOldBkMode);
      textColor = (clrOldText);
    } // if( !_pmid->m_strInPlaceText )
  } // if( !_pmid->m_rcInPlaceWnd IsRectEmpty(&) )
}
int CExtPaintManagerOffice2003::PaintSeparator(HDDC dc, const RECT* rectItem, BOOL bHorz, BOOL bTransparentBk, CObject* pHelperSrc, LPARAM lParam // = 0L
                                              )
{
  if (pHelperSrc == NULL || (!(pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtToolControlBar)) || pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtBarButton)) || pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtStatusControlBar)))) || stat_GetBPP() <= 8) {
    CExtPaintManagerXP::PaintSeparator(dc, rectItem, bHorz, bTransparentBk, pHelperSrc, lParam);
    return ;
  }

  RECT rectItem2(rectItem);

  if (!bTransparentBk) {
    TFillRect(dc, rectItem2, GetColor(CLR_3DFACE_OUT));
  }

  COLORREF clrBtnShadow = GetColor(_2003CLR_SEPARATOR_DARK);
  COLORREF clrBtnHilight = GetColor(_2003CLR_SEPARATOR_LIGHT);

  if (bHorz) {
    int x = (rectItem2.left + rectItem2.right) / 2;
    rectItem2.left = x - 1;
    rectItem2.right = x + 1;

    if (pHelperSrc == NULL || (!pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtStatusControlBar)))) {
      rectItem2 DeflateRect(&, 0, 3, 0, 4);
    }

    TDraw3DRect(dc, &rectItem2, clrBtnShadow, clrBtnHilight);
  } // if( bHorz )
  else {
    int y = (rectItem2.top + rectItem2.bottom) / 2;
    rectItem2.top = y - 1;
    rectItem2.bottom = y + 1;

    if (pHelperSrc == NULL || (!pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtStatusControlBar)))) {
      rectItem2 DeflateRect(&, 4, 0, 5, 0);
    }

    TDraw3DRect(dc, &rectItem2, clrBtnShadow, clrBtnHilight);
  } // else from if( bHorz )
}
int CExtPaintManagerXP::PaintSeparator(HDDC dc, const RECT* rectItem, BOOL bHorz, BOOL bTransparentBk, CObject* pHelperSrc, LPARAM lParam // = 0L
                                      )
{
  pHelperSrc;
  lParam;
  RECT rectItem2(rectItem);

  if (!bTransparentBk) {
    TFillRect(dc, rectItem2, GetColor(CLR_3DFACE_OUT));
  }

  if (bHorz) {
    rectItem2.left = rectItem2.left + rectItem2 RCWIDTH(&) / 2 - 1;
    rectItem2.right = rectItem2.left + 1;
    rectItem2 DeflateRect(&, 0, 1);
  }
  else {
    rectItem2.top = rectItem2.top + rectItem2 RCHEIGHT(&) / 2 - 1;
    rectItem2.bottom = rectItem2.top + 1;
    rectItem2 DeflateRect(&, 1, 0);
  }

  TFillRect(dc, &rectItem2, GetColor(XPCLR_SEPARATOR));
}
int PaintSeparator(HDDC dc, const RECT* rectItem, BOOL bHorz, BOOL bTransparentBk, CObject* pHelperSrc, LPARAM lParam // = 0L
                  )
{
  pHelperSrc;
  lParam;
  RECT rectItem2(rectItem);

  if (!bTransparentBk) {
    TFillRect(dc, rectItem2, GetColor(CLR_3DFACE_OUT));
  }

  COLORREF clrBtnShadow = GetColor(CLR_3DSHADOW_OUT);
  COLORREF clrBtnHilight = GetColor(CLR_3DHILIGHT_OUT);

  if (bHorz) {
    int x = (rectItem2.left + rectItem2.right) / 2;
    rectItem2.left = x - 1;
    rectItem2.right = x + 1;
    rectItem2 DeflateRect(&, 0, 0, 0, 1);
    TDraw3DRect(dc, &rectItem2, clrBtnShadow, clrBtnHilight);
  } // if( bHorz )
  else {
    int y = (rectItem2.top + rectItem2.bottom) / 2;
    rectItem2.top = y - 1;
    rectItem2.bottom = y + 1;
    rectItem2 DeflateRect(&, 0, 0, 1, 0);
    TDraw3DRect(dc, &rectItem2, clrBtnShadow, clrBtnHilight);
  } // else from if( bHorz )
}
PAINTCONTROLFRAMEDATA::PAINTCONTROLFRAMEDATA(): m_pHelperSrc(NULL), m_lParam(0L), m_rcClient(0, 0, 0, 0), m_bFlat(FALSE), m_bHover(FALSE), m_bEnabled(FALSE), m_bFocus(FALSE) {}
PAINTCONTROLFRAMEDATA::PAINTCONTROLFRAMEDATA(CObject* pHelperSrc, const RECT* rcClient, BOOL bFlat, BOOL bHover, BOOL bEnabled, BOOL bFocus): m_pHelperSrc(pHelperSrc), m_lParam(0L), m_rcClient(rcClient), m_bFlat(bFlat), m_bHover(bHover), m_bEnabled(bEnabled), m_bFocus(bFocus) {}
int PaintControlFrame(HDDC dc, PAINTCONTROLFRAMEDATA& _pcfd)
{
  RECT rectItem(_pcfd->m_rcClient);
  COLORREF clrInner = GetColor(COLOR_WINDOW);
  COLORREF clrIn1, clrIn2, clrOut1, clrOut2;
  clrIn1 = clrIn2 = clrOut1 = clrOut2 = GetColor(CLR_3DFACE_OUT);

  if (_pcfd->m_bEnabled) {
    if (_pcfd->m_bHover && _pcfd->m_bFocus) {
      clrIn1 = GetColor(COLOR_3DFACE);
      clrIn2 = GetColor(COLOR_3DDKSHADOW);
      clrOut1 = GetColor(COLOR_3DHILIGHT);
      clrOut2 = GetColor(COLOR_3DSHADOW);
    } // if( _pcfd->m_bHover && _pcfd->m_bFocus )
    else {
      if (_pcfd->m_bHover || _pcfd->m_bFocus) {
        clrIn1 = GetColor(COLOR_3DHILIGHT);
        clrIn2 = GetColor(COLOR_3DSHADOW);
      } // if( _pcfd->m_bHover || _pcfd->m_bFocus )
    } // elsefrom if( _pcfd->m_bHover && _pcfd->m_bFocus )
  } // else from if( _pcfd->m_bEnabled )

  TDraw3DRect(dc, &rectItem, clrInner, clrInner);
  rectItem InflateRect(&, 1, 1);
  TDraw3DRect(dc, &rectItem, clrInner, clrInner);
  rectItem InflateRect(&, 1, 1);
  TDraw3DRect(dc, &rectItem, clrInner, clrInner);
  rectItem InflateRect(&, 1, 1);
  TDraw3DRect(dc, &rectItem, clrIn2, clrIn1);
  rectItem InflateRect(&, 1, 1);
  TDraw3DRect(dc, &rectItem, clrOut2, clrOut1);
}
int CExtPaintManagerXP::PaintControlFrame(HDDC dc, PAINTCONTROLFRAMEDATA& _pcfd)
{
  RECT rectItem(_pcfd->m_rcClient);
  COLORREF clrInner = GetColor(COLOR_WINDOW);
  COLORREF clrIn, clrOut;
  clrIn = clrOut = GetColor(CLR_3DFACE_OUT);

  if (_pcfd->m_bEnabled) {
    if (_pcfd->m_bHover) {
      if (_pcfd->m_bFocus) {
        clrIn = clrOut = GetColor(XPCLR_HILIGHT_BORDER);
      }
      else {
        clrIn = GetColor(XPCLR_HILIGHT_BORDER);
      }
    } // if( _pcfd->m_bHover )
    else {
      if (_pcfd->m_bFocus) {
        clrIn = GetColor(XPCLR_HILIGHT_BORDER);
      }
    } // else from if( _pcfd->m_bHover )
  } // else from if( _pcfd->m_bEnabled )

  TDraw3DRect(dc, &rectItem, clrInner, clrInner);
  rectItem InflateRect(&, 1, 1);
  TDraw3DRect(dc, &rectItem, clrInner, clrInner);
  rectItem InflateRect(&, 1, 1);
  TDraw3DRect(dc, &rectItem, clrInner, clrInner);
  rectItem InflateRect(&, 1, 1);
  TDraw3DRect(dc, &rectItem, clrIn, clrIn);
  rectItem InflateRect(&, 1, 1);
  TDraw3DRect(dc, &rectItem, clrOut, clrOut);
}
int CExtPaintManagerXP::PaintTabItem(HDDC dc, RECT* rcTabItemsArea, BOOL bTopLeft, BOOL bHorz, BOOL bSelected, BOOL bCenteredText, BOOL bGroupedMode, BOOL bInGroupActive, BOOL bInvertedVerticalMode, const RECT* rcEntireItem, SIZE sizeTextMeasured, HFONT pFont, __EXT_MFC_SAFE_LPCTSTR sText, Image* pIcon, CObject* pHelperSrc, LPARAM lParam // = 0L
                                    )
{
  pHelperSrc;
  lParam;
  COLORREF clrText = GetColor((bSelected || bGroupedMode) ? COLOR_BTNTEXT : COLOR_3DSHADOW);
  COLORREF clrTabBk = GetColor(bGroupedMode ? COLOR_3DFACE : XPCLR_3DFACE_DARK);
  COLORREF clrTabBorderLT = GetColor(bGroupedMode ? COLOR_3DSHADOW : COLOR_3DHILIGHT);
  COLORREF clrTabBorderRB = GetColor(bGroupedMode ? COLOR_3DSHADOW : COLOR_3DDKSHADOW);
  COLORREF clrTabSeparator = GetColor(COLOR_3DSHADOW);
  stat_PaintTabItemImpl(dc, rcTabItemsArea, bTopLeft, bHorz, bSelected, bCenteredText, bGroupedMode, bInGroupActive, bInvertedVerticalMode, rcEntireItem, sizeTextMeasured, pFont, sText, pIcon, clrText, clrTabBk, clrTabBorderLT, clrTabBorderRB, clrTabSeparator);
}
int PaintTabClientArea(HDDC dc, RECT* rcClient, RECT* rcTabItemsArea, RECT* rcTabNearBorderArea, DWORD dwOrientation, BOOL bGroupedMode, CObject* pHelperSrc, LPARAM lParam // = 0L
                      )
{
  pHelperSrc;
  lParam;
#if (!defined __EXT_MFC_NO_TAB_CTRL)
  int idxClrTabNearMargin = COLOR_3DDKSHADOW;

  switch (dwOrientation) {
  case __ETWS_ORIENT_TOP:
    idxClrTabNearMargin = COLOR_3DHILIGHT;
    break;

  case __ETWS_ORIENT_BOTTOM:
    break;

  case __ETWS_ORIENT_LEFT:
    idxClrTabNearMargin = COLOR_3DHILIGHT;
    break;

  case __ETWS_ORIENT_RIGHT:
    break;
#ifdef _DEBUG

  default:
    ASSERT(FALSE);
    break;
#endif // _DEBUG
  } // switch( dwOrientation )

  COLORREF clrTabItemsAreaBk = GetColor(bGroupedMode ? COLOR_3DFACE : COLOR_3DSHADOW);
  COLORREF clrTabNearBorderAreaBk = GetColor(COLOR_3DFACE);
  COLORREF clrTabNearMargin = GetColor(idxClrTabNearMargin);
  stat_PaintTabClientAreaImpl(dc, rcClient, rcTabItemsArea, rcTabNearBorderArea, dwOrientation, bGroupedMode, clrTabItemsAreaBk, clrTabNearBorderAreaBk, clrTabNearMargin);
#else // (!defined __EXT_MFC_NO_TAB_CTRL)
  dc;
  rcClient;
  rcTabItemsArea;
  rcTabNearBorderArea;
  dwOrientation;
  bGroupedMode;
#endif // else from(!defined __EXT_MFC_NO_TAB_CTRL)
}
int CExtPaintManagerXP::PaintTabClientArea(HDDC dc, RECT* rcClient, RECT* rcTabItemsArea, RECT* rcTabNearBorderArea, DWORD dwOrientation, BOOL bGroupedMode, CObject* pHelperSrc, LPARAM lParam // = 0L
                                          )
{
  pHelperSrc;
  lParam;
#if (!defined __EXT_MFC_NO_TAB_CTRL)
  int idxClrTabNearMargin = COLOR_3DSHADOW;

  switch (dwOrientation) {
  case __ETWS_ORIENT_TOP:
    idxClrTabNearMargin = COLOR_3DHILIGHT;
    break;

  case __ETWS_ORIENT_BOTTOM:
    break;

  case __ETWS_ORIENT_LEFT:
    idxClrTabNearMargin = COLOR_3DHILIGHT;
    break;

  case __ETWS_ORIENT_RIGHT:
    break;
#ifdef _DEBUG

  default:
    ASSERT(FALSE);
    break;
#endif // _DEBUG
  } // switch( dwOrientation )

  COLORREF clrTabItemsAreaBk = GetColor(XPCLR_3DFACE_NORMAL);
  COLORREF clrTabNearBorderAreaBk = GetColor(XPCLR_3DFACE_DARK);
  COLORREF clrTabNearMargin = GetColor(idxClrTabNearMargin);
  stat_PaintTabClientAreaImpl(dc, rcClient, rcTabItemsArea, rcTabNearBorderArea, dwOrientation, bGroupedMode, clrTabItemsAreaBk, clrTabNearBorderAreaBk, clrTabNearMargin);
#else // (!defined __EXT_MFC_NO_TAB_CTRL)
  dc;
  rcClient;
  rcTabItemsArea;
  rcTabNearBorderArea;
  dwOrientation;
  bGroupedMode;
#endif // else from(!defined __EXT_MFC_NO_TAB_CTRL)
}
int CExtPaintManagerOffice2003::PaintTabClientArea(HDDC dc, RECT* rcClient, RECT* rcTabItemsArea, RECT* rcTabNearBorderArea, DWORD dwOrientation, BOOL bGroupedMode, CObject* pHelperSrc, LPARAM lParam // = 0L
                                                  )
{
  pHelperSrc;
  lParam;
#if (!defined __EXT_MFC_NO_TAB_CTRL)
  BOOL bFrameBackground = FALSE;

  if (pHelperSrc != NULL && pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtTabWnd)) && m_bmpDockerGradient != NULL && stat_GetBPP() > 8) {
    if (
#if (!defined __EXT_MFC_NO_TABMDI_CTRL)
        pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtTabMdiWnd)) ||
#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL)
        ((CExtTabWnd*)pHelperSrc)->m_bReflectParentSizing) {
      bFrameBackground = TRUE;
    }

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
    else if (pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtDynTabWnd)) || pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtDynAutoHideArea))) {
      bFrameBackground = TRUE;
    }

#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
  }

  if (bFrameBackground) {
    RECT rcTabNearMargin(rcTabNearBorderArea); // prepare tab border margin rect

    switch (dwOrientation) {
    case __ETWS_ORIENT_TOP:
      rcTabNearMargin.bottom = rcTabNearMargin.top + 1;
      break;

    case __ETWS_ORIENT_BOTTOM:
      rcTabNearMargin.top = rcTabNearMargin.bottom - 1;
      break;

    case __ETWS_ORIENT_LEFT:
      rcTabNearMargin.right = rcTabNearMargin.left + 1;
      break;

    case __ETWS_ORIENT_RIGHT:
      rcTabNearMargin.left = rcTabNearMargin.right - 1;
      break;
#ifdef _DEBUG

    default:
      ASSERT(FALSE);
      break;
#endif // _DEBUG
    } // switch( dwOrientation )

    CWnd* pFrame = ((CExtTabWnd*)pHelperSrc)->GetParentFrame();

    if (pFrame == NULL) {
      pFrame = ((CExtTabWnd*)pHelperSrc)->GetParent();
    }

    ASSERT_VALID(pFrame);
    RECT rcPaintGradient;
    pFrame->GetClientRect(&rcPaintGradient);
    pFrame->ClientToScreen(&rcPaintGradient);
    ((CExtTabWnd*)pHelperSrc)->ScreenToClient(&rcPaintGradient);
    HDDC dcCompat = CachedDcGet(m_bmpDockerGradient);

    if (dcCompat == NULL) {
      ASSERT(FALSE);
      return ;
    }

    int nOldStretchBltMode = dc.SetStretchBltMode(m_bIsWinNT ? HALFTONE : COLORONCOLOR);
    ::SetBrushOrgEx(dc, 0, 0, NULL);
    dc.StretchBlt(rcPaintGradient->left, rcPaintGradient->top, rcPaintGradient->Width(), rcPaintGradient->Height(), &dcCompat, 0, 0, m_sizeDockerGradient.w, m_sizeDockerGradient.h, SRCCOPY);
    dc.SetStretchBltMode(nOldStretchBltMode);

    if (!bGroupedMode) {
      COLORREF clrTabNearMargin = GetColor(COLOR_3DSHADOW);
      COLORREF clrTabNearBorderAreaBk = GetColor(CLR_3DFACE_OUT);
      // fill tab border area
      TFillRect(dc, &rcTabNearBorderArea, clrTabNearBorderAreaBk);
      // paint tab border margin
      TFillRect(dc, &rcTabNearMargin, clrTabNearMargin);
    } // if( !bGroupedMode )

    return ;
  } // if( bFrameBackground )

  CExtPaintManagerXP::PaintTabClientArea(dc, rcClient, rcTabItemsArea, rcTabNearBorderArea, dwOrientation, bGroupedMode, pHelperSrc, lParam);
#else // (!defined __EXT_MFC_NO_TAB_CTRL)
  dc;
  rcClient;
  rcTabItemsArea;
  rcTabNearBorderArea;
  dwOrientation;
  bGroupedMode;
#endif // else from(!defined __EXT_MFC_NO_TAB_CTRL)
}
int PaintTabButton(HDDC dc, RECT* rcButton, LONG nHitTest, BOOL bTopLeft, BOOL bHorz, BOOL bEnabled, BOOL bHover, BOOL bPushed, BOOL bGroupedMode, CObject* pHelperSrc, LPARAM lParam // = 0L
                  )
{
  pHelperSrc;
  lParam;
  COLORREF clrGlyph = g_PaintManager->GetColor(bGroupedMode ? (bEnabled ? COLOR_BTNTEXT : COLOR_3DSHADOW) : (bEnabled ? COLOR_3DHIGHLIGHT : COLOR_3DFACE));
  COLORREF clrTL = g_PaintManager->GetColor(bPushed ? COLOR_3DDKSHADOW : COLOR_3DHILIGHT);
  COLORREF clrBR = g_PaintManager->GetColor(bPushed ? COLOR_3DHILIGHT : COLOR_3DDKSHADOW);
  stat_PaintTabButtonImpl(dc, rcButton, nHitTest, bTopLeft, bHorz, bEnabled, bHover, bPushed, bGroupedMode, clrGlyph, clrTL, clrBR);
}
int CExtPaintManagerXP::PaintTabButton(HDDC dc, RECT* rcButton, LONG nHitTest, BOOL bTopLeft, BOOL bHorz, BOOL bEnabled, BOOL bHover, BOOL bPushed, BOOL bGroupedMode, CObject* pHelperSrc, LPARAM lParam // = 0L
                                      )
{
  pHelperSrc;
  lParam;
  COLORREF clrGlyph = g_PaintManager->GetColor(bEnabled ? COLOR_BTNTEXT : COLOR_3DSHADOW);
  COLORREF clrTL = g_PaintManager->GetColor(bPushed ? COLOR_3DDKSHADOW : COLOR_3DHILIGHT);
  COLORREF clrBR = g_PaintManager->GetColor(bPushed ? COLOR_3DHILIGHT : COLOR_3DDKSHADOW);
  stat_PaintTabButtonImpl(dc, rcButton, nHitTest, bTopLeft, bHorz, bEnabled, bHover, bPushed, bGroupedMode, clrGlyph, clrTL, clrBR);
}
int PaintTabNcAreaRect(HDDC dc, const RECT* rc, CObject* pHelperSrc, LPARAM lParam // = 0L
                      )
{
  pHelperSrc;
  lParam;
  COLORREF clrTL = GetColor(COLOR_3DSHADOW);
  COLORREF clrBR = GetColor(COLOR_3DHILIGHT);
  TDraw3DRect(dc, &rc, clrTL, clrBR);
}
int CExtPaintManagerXP::PaintTabNcAreaRect(HDDC dc, const RECT* rc, CObject* pHelperSrc, LPARAM lParam // = 0L
                                          )
{
  pHelperSrc;
  lParam;
  COLORREF clrTL = GetColor(COLOR_3DSHADOW);
  COLORREF clrBR = clrTL;
  TDraw3DRect(dc, &rc, clrTL, clrBR);
}
int PaintDockBarClientArea(HDDC dc, const RECT* rcClient, CObject* pHelperSrc, LPARAM lParam // = 0L
                          )
{
  ASSERT(dc != NULL);
  pHelperSrc;
  lParam;

  if (::IsRectEmpty(&rcClient) || (!dc.RectVisible(&rcClient))) {
    return ;
  }

  TFillRect(dc, &rcClient, GetColor(COLOR_3DFACE));
}
int CExtPaintManagerXP::PaintDockBarClientArea(HDDC dc, const RECT* rcClient, CObject* pHelperSrc, LPARAM lParam // = 0L
                                              )
{
  PaintDockBarClientArea(dc, rcClient, pHelperSrc, lParam);
}
BOOL GetCb2DbTransparentMode(CObject* pObjHelperSrc, LPARAM lParam // = 0L
                            )
{
  pObjHelperSrc;
  lParam;
  return FALSE;
}
BOOL CExtPaintManagerXP::GetCb2DbTransparentMode(CObject* pObjHelperSrc, LPARAM lParam // = 0L
                                                )
{
  pObjHelperSrc;
  lParam;
  return FALSE;
}
BOOL CExtPaintManagerOffice2003::GetCb2DbTransparentMode(CObject* pObjHelperSrc, LPARAM lParam // = 0L
                                                        )
{
  pObjHelperSrc;
  lParam;

  if (stat_GetBPP() <= 8) {
    return FALSE;
  }

  return TRUE;
}
BOOL PaintDockerBkgnd(HDDC dc, CWnd* pWnd, LPARAM lParam // = NULL
                     )
{
  ASSERT(dc != NULL);
  ASSERT(pWnd->GetSafeHwnd() != NULL && IsWindow(pWnd->GetSafeHwnd()));
  dc;
  pWnd;
  lParam;
  return FALSE;
}
BOOL CExtPaintManagerXP::PaintDockerBkgnd(HDDC dc, CWnd* pWnd, LPARAM lParam // = NULL
                                         )
{
  ASSERT(dc != NULL);
  ASSERT(pWnd->GetSafeHwnd() != NULL && IsWindow(pWnd->GetSafeHwnd()));
  dc;
  pWnd;
  lParam;
  return FALSE;
}
BOOL CExtPaintManagerOffice2003::PaintDockerBkgnd(HDDC dc, CWnd* pWnd, LPARAM lParam // = NULL
                                                 )
{
  ASSERT(dc != NULL);
  ASSERT(pWnd->GetSafeHwnd() != NULL && IsWindow(pWnd->GetSafeHwnd()));
  lParam;

  if ((!GetCb2DbTransparentMode(pWnd)) || m_bmpDockerGradient == NULL) {
    return FALSE;
  }

  COLORREF clrSolidShade0 = ((COLORREF)(- 1L));
  COLORREF clrSolidShade1 = ((COLORREF)(- 1L));
  BOOL bToolbarShade = FALSE;
  BOOL bToolbarHorz = FALSE;

  for (CWnd* pFrame = pWnd; pFrame != NULL; pFrame = pFrame->GetParent()) {
    if ((pFrame->GetStyle() &WS_CHILD) == 0) {
      if (!pFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd))) {
        return FALSE;
      }

      break;
    }

    if (pFrame != pWnd && pFrame->IsKindOf(RUNTIME_CLASS(CExtToolControlBar))) {
      if (((CExtToolControlBar*)pFrame)->m_pDockSite != NULL) {
        if (!(pFrame->IsKindOf(RUNTIME_CLASS(CExtPanelControlBar)) || pFrame->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)))) {
          if (((CExtToolControlBar*)pFrame)->IsFloating()) {
            clrSolidShade0 = GetColor(_2003CLR_MLA_NORM_RIGHT);
            clrSolidShade1 = GetColor(_2003CLR_SEPARATOR_LIGHT);
          }
          else {
            bToolbarShade = TRUE;
            bToolbarHorz = ((CExtToolControlBar*)pFrame)->IsDockedHorizontally();
          }
        }
      }

      break;
    }
  }

  if (pFrame == NULL) {
    return FALSE;
  }

  RECT rcPaintGradient;
  pFrame->GetClientRect(&rcPaintGradient);
  pFrame->ClientToScreen(&rcPaintGradient);
  pWnd->ScreenToClient(&rcPaintGradient);

  if (::IsRectEmpty(&rcPaintGradient) || (!dc.RectVisible(&rcPaintGradient))) {
    return TRUE;
  }

  if (clrSolidShade0 != ((COLORREF)(- 1L))) {
    if (clrSolidShade1 != ((COLORREF)(- 1L))) {
      stat_PaintGradientRect(dc, rcPaintGradient, clrSolidShade0, clrSolidShade1, TRUE);
    }
    else {
      TFillRect(dc, rcPaintGradient, clrSolidShade0);
    }

    return TRUE;
  } // if( clrSolidShade0 != ((COLORREF)(-1L)) )

  HDDC dcCompat = CachedDcGet(bToolbarShade ? (bToolbarHorz ? m_bmpCtBarGradientHorz : m_bmpCtBarGradientVert) : m_bmpDockerGradient);

  if (dcCompat == NULL) {
    ASSERT(FALSE);
    return FALSE;
  }

  SIZE sizeShade = bToolbarShade ? (bToolbarHorz ? m_sizeCtBarGradientHorz : m_sizeCtBarGradientVert) : m_sizeDockerGradient;
  int nOldStretchBltMode = dc.SetStretchBltMode(m_bIsWinNT ? HALFTONE : COLORONCOLOR);
  ::SetBrushOrgEx(dc, 0, 0, NULL);
  dc.StretchBlt(rcPaintGradient->left, rcPaintGradient->top, rcPaintGradient->Width(), rcPaintGradient->Height(), &dcCompat, 0, 0, sizeShade.w, sizeShade.h, SRCCOPY);
  dc.SetStretchBltMode(nOldStretchBltMode);
  return TRUE;
}
int PaintControlBarClientArea(HDDC dc, const RECT* rcClient, CObject* pHelperSrc, LPARAM lParam // = 0L
                             )
{
  pHelperSrc;
  lParam;

  if (::IsRectEmpty(&rcClient) || (!dc.RectVisible(&rcClient))) {
    return ;
  }

  TFillRect(dc, &rcClient, GetColor(CLR_3DFACE_OUT));
}
int CExtPaintManagerXP::PaintControlBarClientArea(HDDC dc, const RECT* rcClient, CObject* pHelperSrc, LPARAM lParam // = 0L
                                                 )
{
  PaintControlBarClientArea(dc, rcClient, pHelperSrc, lParam);
}
int CExtPaintManagerOffice2003::InitTranslatedColors()
{
  m_nIdxClrBtnHoverLeft = - 1;
  m_nIdxClrBtnHoverRight = - 1;
  m_nIdxClrBtnPressedLeft = - 1;
  m_nIdxClrBtnPressedRight = - 1;
  m_nIdxClrBtnHovPresLeft = - 1;
  m_nIdxClrBtnHovPresRight = - 1;
  m_nIdxClrTbFillMargin = - 1;
  m_bUxApiInitPassed = m_bUxValidColorsExtracted = FALSE;
  InitUserExApi();
  CExtPaintManagerXP::InitTranslatedColors();

  if (m_bmpCtBarGradientHorz != NULL) {
    m_bmpCtBarGradientHorz.DeleteObject();
  }

  if (m_bmpCtBarGradientVert != NULL) {
    m_bmpCtBarGradientVert.DeleteObject();
  }

  if (m_bmpDockerGradient != NULL) {
    m_bmpDockerGradient.DeleteObject();
  }

  if (stat_GetBPP() > 8) {
    static const LONG g_nBarGradientSizeMetric = 256L;
    static const LONG g_nBarGradientPixelCount = 256L * 256L;
    m_sizeCtBarGradientHorz.w = m_sizeCtBarGradientHorz.h = m_sizeCtBarGradientVert.w = m_sizeCtBarGradientVert.h = m_sizeDockerGradient.w = m_sizeDockerGradient.h = g_nBarGradientSizeMetric;
    HDDC dcDesktop(NULL);

    if (m_bUxApiInitPassed && m_bUxValidColorsExtracted && m_bUxUseIfAvailOnWinXpOrLater) {
      // if use WinXP themed colors
      double lfEdgeLightHue, lfEdgeLightSat, lfEdgeLightLum, lfEdgeShadowHue, lfEdgeShadowSat, lfEdgeShadowLum, lfCustFillHue, lfCustFillSat, lfCustFillLum;
      stat_RGBtoHSL(m_clrUxTaskBandEdgeLightColor, &lfEdgeLightHue, &lfEdgeLightSat, &lfEdgeLightLum);
      stat_RGBtoHSL(m_clrUxTaskBandEdgeShadowColor, &lfEdgeShadowHue, &lfEdgeShadowSat, &lfEdgeShadowLum);
      stat_RGBtoHSL(m_clrUxTaskBandFillColorHint, &lfCustFillHue, &lfCustFillSat, &lfCustFillLum);
      COLORREF clrFillHint = stat_HLStoRGB(lfCustFillHue, (lfEdgeLightLum + lfEdgeShadowLum) / 2.0, lfCustFillSat);
      COLORREF clrAccentHint = m_clrUxTaskBandAccentColorHint;
      // re-install XP colors
      // xp - menu area light ver
      COLORREF xpclr_MenuLight = stat_HLS_Adjust(clrFillHint, 0.00, 0.95, - 0.05);
      xpclr_MenuLight = _RGB(GetRV(xpclr_MenuLight) + MulDiv(255 - GetRV(xpclr_MenuLight), 40, 100), GetGV(xpclr_MenuLight) + MulDiv(255 - GetGV(xpclr_MenuLight), 20, 100), GetBV(xpclr_MenuLight));
      xpclr_MenuLight = stat_HLS_Adjust(xpclr_MenuLight, 0.00, 0.06, 0.05);
      // 2003 - float light
      COLORREF xpclr_FloatFace = stat_HLS_Adjust(clrFillHint, 0.00, 0.50, 0.00);
      // xp - rarely used item left side
      COLORREF xpclr_RarelyUsedMenuLeft = _RGB((999L * (long)(GetRV(clrFillHint))) / 1000L, (995L * (long)(GetGV(clrFillHint))) / 1000L, (995L * (long)(GetBV(clrFillHint))) / 1000L);
      // xp - control bar funny background
      COLORREF xpclr_ControlBarBk = stat_HLS_Adjust(clrFillHint, 0.00, 0.20, 0.00);
      // xp - orange like light ver
      COLORREF xpclr_Highlight = stat_HLS_Adjust(clrAccentHint, 0.00, 0.60, 0.45);
      // 2003 - dark orange
      COLORREF clr2003faceIn = stat_HLS_Adjust(clrAccentHint, 0.00, - 0.30, 0.45);
      // xp - blue like dark ver
      COLORREF clrTmp = stat_HLS_Adjust(clrFillHint, 0.00, 0.20, 0.00);
      COLORREF xpclr_HighlightDarked = stat_HLS_Adjust(clrTmp, 0.00, - 0.25, 0.10);
      // xp - dark selected border
      COLORREF xpclr_HighlightBorder = stat_HLS_Adjust(xpclr_HighlightDarked, 0.00, - 0.50, 0.00);
      // xp - dark gray separator
      COLORREF xpclr_Separator = _RGB((857L * (long)(GetRV(clrFillHint))) / 1000L, (857L * (long)(GetGV(clrFillHint))) / 1000L, (857L * (long)(GetBV(clrFillHint))) / 1000L);
      // xp - dark panel border (for floating controlbars)
      COLORREF xpclr_PanelBorder = stat_HLS_Adjust(clrFillHint, 0.00, - 0.25, 0.00);
      m_mapColorTranslate[CLR_3DFACE_OUT] = InstallColor(xpclr_ControlBarBk);
      m_mapColorTranslate[CLR_3DFACE_IN] = InstallColor(xpclr_Highlight);
      m_mapColorTranslate[CLR_3DFACE_DISABLED] = COLOR_3DFACE;
      int idxClrDark = InstallColor(xpclr_PanelBorder);
      m_mapColorTranslate[CLR_3DLIGHT_OUT] = idxClrDark;
      m_mapColorTranslate[CLR_3DLIGHT_IN] = idxClrDark;
      m_mapColorTranslate[CLR_3DLIGHT_DISABLED] = idxClrDark;
      m_mapColorTranslate[CLR_3DHILIGHT_OUT] = idxClrDark;
      m_mapColorTranslate[CLR_3DHILIGHT_IN] = idxClrDark;
      m_mapColorTranslate[CLR_3DHILIGHT_DISABLED] = idxClrDark;
      m_mapColorTranslate[CLR_3DSHADOW_OUT] = idxClrDark;
      m_mapColorTranslate[CLR_3DSHADOW_IN] = idxClrDark;
      m_mapColorTranslate[CLR_3DSHADOW_DISABLED] = idxClrDark;
      m_mapColorTranslate[CLR_3DDKSHADOW_OUT] = idxClrDark;
      m_mapColorTranslate[CLR_3DDKSHADOW_IN] = idxClrDark;
      m_mapColorTranslate[CLR_3DDKSHADOW_DISABLED] = idxClrDark;
      m_mapColorTranslate[CLR_TEXT_OUT] = COLOR_BTNTEXT;
      m_mapColorTranslate[CLR_TEXT_IN] = COLOR_BTNTEXT;
      m_mapColorTranslate[CLR_TEXT_DISABLED] = COLOR_3DSHADOW;
      m_mapColorTranslate[XPCLR_PUSHEDHOVERTEXT] = COLOR_BTNTEXT;
      m_mapColorTranslate[XPCLR_3DFACE_DARK] = InstallColor(xpclr_ControlBarBk);
      m_mapColorTranslate[XPCLR_3DFACE_NORMAL] = InstallColor(xpclr_MenuLight);
      m_mapColorTranslate[XPCLR_SEPARATOR] = InstallColor(xpclr_Separator);
      m_mapColorTranslate[XPCLR_HILIGHT] =
          //InstallColor( xpclr_HighlightDarked );
          InstallColor(clr2003faceIn);
      m_mapColorTranslate[XPCLR_HILIGHT_BORDER] = InstallColor(xpclr_HighlightBorder);
      m_mapColorTranslate[XPCLR_RARELY_BORDER] = InstallColor(xpclr_RarelyUsedMenuLeft);
      m_mapColorTranslate[XPCLR_MENU_BORDER] = InstallColor(stat_HLS_Adjust(clrFillHint, 0.00, - 0.68, 0.00));
      m_mapColorTranslate[XPCLR_3DFACE_FLOAT_F] = InstallColor(xpclr_FloatFace);
      // install new colors
      m_mapColorTranslate[_2003CLR_GRADIENT_LIGHT] = InstallColor(stat_HLS_Adjust(clrFillHint, 0.00, 0.30, 0.00));
      m_mapColorTranslate[_2003CLR_GRADIENT_DARK] = InstallColor(stat_HLS_Adjust(clrFillHint, 0.00, 0.00, 0.10));
      m_mapColorTranslate[_2003CLR_SEPARATOR_LIGHT] = InstallColor(stat_HLS_Adjust(clrFillHint, 0.00, 0.98, 0.00));
      m_mapColorTranslate[_2003CLR_SEPARATOR_DARK] = InstallColor(stat_HLS_Adjust(clrFillHint, 0.00, - 0.45, - 0.00));
      m_mapColorTranslate[_2003CLR_GRIPPER_DOT_DARK] = InstallColor(stat_HLS_Adjust(clrFillHint, 0.00, - 0.60, 0.00));
      m_mapColorTranslate[_2003CLR_GRIPPER_DOT_LIGHT] = InstallColor(stat_HLS_Adjust(clrFillHint, 0.00, 0.95, 0.00));
      m_mapColorTranslate[_2003CLR_EXPBTN_LIGHT] = InstallColor(stat_HLS_Adjust(clrFillHint, 0.00, - 0.10, 0.00));
      m_mapColorTranslate[_2003CLR_EXPBTN_DARK] = InstallColor(stat_HLS_Adjust(clrFillHint, 0.00, - 0.60, 0.00));
      m_mapColorTranslate[_2003CLR_EXPBTN_HOVER_LIGHT] = InstallColor(stat_HLS_Adjust(clrAccentHint, 0.00, 0.85, 0.00));
      m_mapColorTranslate[_2003CLR_EXPBTN_HOVER_DARK] = InstallColor(stat_HLS_Adjust(clrAccentHint, 0.00, - 0.05, 0.00));
      m_mapColorTranslate[_2003CLR_EXPBTN_PRESSED_LIGHT] = InstallColor(stat_HLS_Adjust(clrAccentHint, 0.00, 0.30, 0.00));
      m_mapColorTranslate[_2003CLR_EXPBTN_PRESSED_DARK] = InstallColor(stat_HLS_Adjust(clrAccentHint, 0.00, - 0.30, 0.00));
      m_mapColorTranslate[_2003CLR_EXPGLYPH_LIGHT] = InstallColor(stat_HLS_Adjust(clrFillHint, 0.00, 0.97, 0.00));
      m_mapColorTranslate[_2003CLR_EXPGLYPH_DARK] = InstallColor(stat_HLS_Adjust(clrFillHint, 0.00, - 0.97, 0.00));
      m_mapColorTranslate[_2003CLR_STATUSBAR_ITEM] = COLOR_3DSHADOW;
      // InstallColor(
      // stat_HLS_Adjust(
      // GetColor(XPCLR_3DFACE_NORMAL),
      // 0.0, -0.05, 0.0
      // )
      // );
      m_mapColorTranslate[_2003CLR_MLA_NORM_LEFT] = InstallColor(stat_HLS_Adjust(clrFillHint, 0.00, 0.75, 0.00));
      m_mapColorTranslate[_2003CLR_MLA_NORM_RIGHT] = InstallColor(stat_HLS_Adjust(GetColor(XPCLR_3DFACE_DARK), 0.00, - 0.10, 0.00));
      m_mapColorTranslate[_2003CLR_MLA_RARELY_LEFT] = InstallColor(stat_HLS_Adjust(clrFillHint, 0.00, 0.30, 0.00));
      m_mapColorTranslate[_2003CLR_MLA_RARELY_RIGHT] = InstallColor(stat_HLS_Adjust(clrFillHint, 0.00, - 0.25, 0.00));
      m_mapColorTranslate[_2003CLR_BTN_HOVER_LEFT] = InstallColor(stat_HLS_Adjust(clrAccentHint, 0.00, 0.80, 0.00));
      m_mapColorTranslate[_2003CLR_BTN_HOVER_RIGHT] = InstallColor(stat_HLS_Adjust(clrAccentHint, 0.00, 0.20, 0.00));
      m_mapColorTranslate[_2003CLR_BTN_PRESSED_LEFT] = InstallColor(stat_HLS_Adjust(clrAccentHint, 0.00, 0.40, 0.00));
      m_mapColorTranslate[_2003CLR_BTN_PRESSED_RIGHT] = InstallColor(stat_HLS_Adjust(clrAccentHint, 0.00, - 0.05, 0.00));
      m_mapColorTranslate[_2003CLR_BTN_HP_LEFT] = InstallColor(stat_HLS_Adjust(clrAccentHint, 0.00, - 0.30, 0.75));
      m_mapColorTranslate[_2003CLR_BTN_HP_RIGHT] = InstallColor(stat_HLS_Adjust(clrAccentHint, 0.00, 0.50, 0.00));
      m_nIdxClrBtnHoverLeft = _2003CLR_BTN_HOVER_LEFT;
      m_nIdxClrBtnHoverRight = _2003CLR_BTN_HOVER_RIGHT;
      m_nIdxClrBtnPressedLeft = _2003CLR_BTN_PRESSED_LEFT;
      m_nIdxClrBtnPressedRight = _2003CLR_BTN_PRESSED_RIGHT;
      m_nIdxClrBtnHovPresLeft = _2003CLR_BTN_HP_LEFT;
      m_nIdxClrBtnHovPresRight = _2003CLR_BTN_HP_RIGHT;
      m_nIdxClrTbFillMargin = _2003CLR_EXPBTN_DARK;
      m_mapColorTranslate[_2003CLR_MENUEXPBALOON] = InstallColor(stat_HLS_Adjust(clrFillHint, 0.00, 0.77, 0.00));
      m_colors[COLOR_3DFACE] = clrFillHint;
      m_colors[COLOR_3DLIGHT] = stat_HLS_Adjust(clrFillHint, 0.00, 0.45, 0.10);
      m_colors[COLOR_3DHIGHLIGHT] = stat_HLS_Adjust(clrFillHint, 0.00, 0.75, 0.00);
      m_colors[COLOR_3DSHADOW] = stat_HLS_Adjust(clrFillHint, 0.00, - 0.45, 0.10);
      m_colors[COLOR_3DDKSHADOW] = stat_HLS_Adjust(clrFillHint, 0.00, - 0.75, 0.00);
      // init non-linear gradient bitmaps
      COLORREF clrLeft = stat_HLS_Adjust(GetColor(_2003CLR_GRADIENT_LIGHT), 0.00, 0.50, 0.20);
      COLORREF clrRight = stat_HLS_Adjust(GetColor(_2003CLR_GRADIENT_DARK), 0.00, - 0.30, - 0.20);
      COLORREF clrQuadLeft = _RGB(GetBV(clrLeft), GetGV(clrLeft), GetRV(clrLeft));
      COLORREF clrQuadRight = _RGB(GetBV(clrRight), GetGV(clrRight), GetRV(clrRight));
      double lfColorDist[3] = {
        double(GetRV(clrQuadRight)) - double(GetRV(clrQuadLeft)), double(GetGV(clrQuadRight)) - double(GetGV(clrQuadLeft)), double(GetBV(clrQuadRight)) - double(GetBV(clrQuadLeft)),
      };
      // horizontal bar gradient
      BITMAPINFOHEADER bih;
      memset(&bih, 0, sizeof(BITMAPINFOHEADER));
      bih.biSize = sizeof(BITMAPINFOHEADER);
      bih.biWidth = bih.biHeight = g_nBarGradientSizeMetric;
      bih.biPlanes = 1;
      bih.biBitCount = 32;
      bih.biCompression = BI_RGB;
      bih.biSizeImage = g_nBarGradientPixelCount;
      COLORREF* pSurfaceCB = NULL;
      Image* hDib = ::CreateDIBSection(dcDesktop, (LPBITMAPINFO) &bih, DIB_RGB_COLORS, (int**) &pSurfaceCB, NULL, NULL);
      ASSERT(hDib != NULL && pSurfaceCB != NULL);

      if (hDib != NULL && pSurfaceCB != NULL) {
        m_bmpCtBarGradientHorz.Attach(hDib);

        for (LONG x = 0; x < g_nBarGradientSizeMetric; x++) {
          for (LONG y = 0; y < g_nBarGradientSizeMetric; y++) {
            COLORREF* pClr = pSurfaceCB + g_nBarGradientPixelCount - (y * g_nBarGradientSizeMetric) - g_nBarGradientSizeMetric + x;
            double lfColorOffset[3] = {
              lfColorDist[0], lfColorDist[1], lfColorDist[2],
            };
            double lfQ = (double(g_nBarGradientSizeMetric) - double(y) * 2.0) / double(g_nBarGradientSizeMetric);
            ASSERT((- 1.0) <= lfQ && lfQ <= (1.0));
            double lfQ2 = lfQ * ((lfQ > 0.0) ? lfQ : (- lfQ));
            lfQ = (lfQ * 4.0 + lfQ2) / 5.0;

            for (int i = 0; i < 3; i++) {
              double& lfCV = lfColorOffset[i];
              lfCV /= lfColorDist[i];

              if (lfQ > 0.0) {
                lfCV = lfCV + (1.0 - lfCV) * lfQ;
              }
              else if (lfQ < 0.0) {
                lfCV = lfCV * (1.0 + lfQ);
              }

              if (lfCV < 0.0) {
                lfCV = 0.0;
              }
              else if (lfCV > 1.0) {
                lfCV = 1.0;
              }

              lfCV *= lfColorDist[i];
            } // for( int i = 0; i < 3; i++ )

            *pClr = _RGB(DWORD(double(GetRV(clrQuadRight)) - lfColorOffset[0]), DWORD(double(GetGV(clrQuadRight)) - lfColorOffset[1]), DWORD(double(GetBV(clrQuadRight)) - lfColorOffset[2]));
          }
        } // for( LONG x = 0; x < g_nBarGradientSizeMetric; x++ )
      } // if( hDib != NULL && pSurfaceCB != NULL )

      // vertical bar gradient
      memset(&bih, 0, sizeof(BITMAPINFOHEADER));
      bih.biSize = sizeof(BITMAPINFOHEADER);
      bih.biWidth = bih.biHeight = g_nBarGradientSizeMetric;
      bih.biPlanes = 1;
      bih.biBitCount = 32;
      bih.biCompression = BI_RGB;
      bih.biSizeImage = g_nBarGradientPixelCount;
      pSurfaceCB = NULL;
      hDib = ::CreateDIBSection(dcDesktop, (LPBITMAPINFO) &bih, DIB_RGB_COLORS, (int**) &pSurfaceCB, NULL, NULL);
      ASSERT(hDib != NULL && pSurfaceCB != NULL);

      if (hDib != NULL && pSurfaceCB != NULL) {
        m_bmpCtBarGradientVert.Attach(hDib);

        for (LONG x = 0; x < g_nBarGradientSizeMetric; x++) {
          for (LONG y = 0; y < g_nBarGradientSizeMetric; y++) {
            COLORREF* pClr = pSurfaceCB + g_nBarGradientPixelCount - (y * g_nBarGradientSizeMetric) - g_nBarGradientSizeMetric + x;
            double lfColorOffset[3] = {
              lfColorDist[0], lfColorDist[1], lfColorDist[2],
            };
            double lfQ = (double(g_nBarGradientSizeMetric) - double(x) * 2.0) / double(g_nBarGradientSizeMetric);
            ASSERT((- 1.0) <= lfQ && lfQ <= (1.0));
            double lfQ2 = lfQ * ((lfQ > 0.0) ? lfQ : (- lfQ));
            lfQ = (lfQ * 4.0 + lfQ2) / 5.0;

            for (int i = 0; i < 3; i++) {
              double& lfCV = lfColorOffset[i];
              lfCV /= lfColorDist[i];

              if (lfQ > 0.0) {
                lfCV = lfCV + (1.0 - lfCV) * lfQ;
              }
              else if (lfQ < 0.0) {
                lfCV = lfCV * (1.0 + lfQ);
              }

              if (lfCV < 0.0) {
                lfCV = 0.0;
              }
              else if (lfCV > 1.0) {
                lfCV = 1.0;
              }

              lfCV *= lfColorDist[i];
            } // for( int i = 0; i < 3; i++ )

            *pClr = _RGB(DWORD(double(GetRV(clrQuadRight)) - lfColorOffset[0]), DWORD(double(GetGV(clrQuadRight)) - lfColorOffset[1]), DWORD(double(GetBV(clrQuadRight)) - lfColorOffset[2]));
          }
        } // for( LONG x = 0; x < g_nBarGradientSizeMetric; x++ )
      } // if( hDib != NULL && pSurfaceCB != NULL )
    } // if use WinXP themed colors
    else {
      // if use system colors
      // install new colors
      m_mapColorTranslate[_2003CLR_GRADIENT_LIGHT] = COLOR_WINDOW;
      m_mapColorTranslate[_2003CLR_GRADIENT_DARK] = COLOR_3DFACE;
      m_mapColorTranslate[_2003CLR_SEPARATOR_LIGHT] = COLOR_WINDOW;
      m_mapColorTranslate[_2003CLR_SEPARATOR_DARK] = InstallColor(stat_HLS_Adjust(GetColor(COLOR_3DFACE), 0.0, - 0.20, 0.0));
      m_mapColorTranslate[_2003CLR_GRIPPER_DOT_DARK] = InstallColor(stat_HLS_Adjust(GetColor(COLOR_3DSHADOW), 0.0, 0.20, 0.0));
      m_mapColorTranslate[_2003CLR_GRIPPER_DOT_LIGHT] = COLOR_WINDOW;
      m_mapColorTranslate[_2003CLR_EXPBTN_LIGHT] = InstallColor(stat_HLS_Adjust(GetColor(COLOR_3DSHADOW), 0.0, 0.25, 0.0));
      m_mapColorTranslate[_2003CLR_EXPBTN_DARK] = InstallColor(stat_HLS_Adjust(GetColor(COLOR_3DSHADOW), 0.0, - 0.10, 0.0));
      m_mapColorTranslate[_2003CLR_EXPBTN_HOVER_LIGHT] = m_mapColorTranslate[_2003CLR_EXPBTN_HOVER_DARK] = InstallColor(stat_HLS_Adjust(GetColor(XPCLR_HILIGHT), 0.0, 0.30, 0.0));
      m_mapColorTranslate[_2003CLR_EXPBTN_PRESSED_LIGHT] = m_mapColorTranslate[_2003CLR_EXPBTN_PRESSED_DARK] = InstallColor(stat_HLS_Adjust(GetColor(XPCLR_HILIGHT), 0.0, 0.60, 0.0));
      m_mapColorTranslate[_2003CLR_EXPGLYPH_LIGHT] = COLOR_WINDOW;
      m_mapColorTranslate[_2003CLR_EXPGLYPH_DARK] = COLOR_3DDKSHADOW;
      m_mapColorTranslate[_2003CLR_STATUSBAR_ITEM] = COLOR_3DSHADOW;
      // InstallColor(
      // stat_HLS_Adjust(
      // GetColor(XPCLR_3DFACE_NORMAL),
      // 0.0, -0.05, 0.0
      // )
      // );
      m_mapColorTranslate[_2003CLR_MLA_NORM_LEFT] = COLOR_WINDOW;
      m_mapColorTranslate[_2003CLR_MLA_NORM_RIGHT] = InstallColor(GetColor(XPCLR_3DFACE_DARK));
      m_mapColorTranslate[_2003CLR_MLA_RARELY_LEFT] = InstallColor(GetColor(XPCLR_3DFACE_NORMAL));
      m_mapColorTranslate[_2003CLR_MLA_RARELY_RIGHT] = COLOR_3DFACE;
      m_mapColorTranslate[_2003CLR_MENUEXPBALOON] = COLOR_WINDOW;
      // reinit XP colors
      m_mapColorTranslate[XPCLR_3DFACE_NORMAL] = InstallColor(stat_HLS_Adjust(GetColor(XPCLR_3DFACE_NORMAL), 0.0, 0.30, 0.0));
      // init non-linear gradient bitmaps
      COLORREF clrLeft = GetColor(_2003CLR_GRADIENT_LIGHT);
      COLORREF clrRight = GetColor(_2003CLR_GRADIENT_DARK);
      COLORREF clrQuadLeft = _RGB(GetBV(clrLeft), GetGV(clrLeft), GetRV(clrLeft));
      COLORREF clrQuadRight = _RGB(GetBV(clrRight), GetGV(clrRight), GetRV(clrRight));
      double lfColorDist[3] = {
        double(GetRV(clrQuadRight)) - double(GetRV(clrQuadLeft)), double(GetGV(clrQuadRight)) - double(GetGV(clrQuadLeft)), double(GetBV(clrQuadRight)) - double(GetBV(clrQuadLeft)),
      };
      double lfTmp = ::sqrt(double(g_nBarGradientSizeMetric) * double(g_nBarGradientSizeMetric) * 2.0);
      double lfMaxDist = lfTmp + ::sqrt(lfTmp);
      // horizontal bar gradient
      BITMAPINFOHEADER bih;
      memset(&bih, 0, sizeof(BITMAPINFOHEADER));
      bih.biSize = sizeof(BITMAPINFOHEADER);
      bih.biWidth = bih.biHeight = g_nBarGradientSizeMetric;
      bih.biPlanes = 1;
      bih.biBitCount = 32;
      bih.biCompression = BI_RGB;
      bih.biSizeImage = g_nBarGradientPixelCount;
      COLORREF* pSurfaceCB = NULL;
      Image* hDib = ::CreateDIBSection(dcDesktop, (LPBITMAPINFO) &bih, DIB_RGB_COLORS, (int**) &pSurfaceCB, NULL, NULL);
      ASSERT(hDib != NULL && pSurfaceCB != NULL);

      if (hDib != NULL && pSurfaceCB != NULL) {
        m_bmpCtBarGradientHorz.Attach(hDib);

        for (LONG x = 0; x < g_nBarGradientSizeMetric; x++) {
          for (LONG y = 0; y < g_nBarGradientSizeMetric; y++) {
            COLORREF* pClr = pSurfaceCB + g_nBarGradientPixelCount - (y * g_nBarGradientSizeMetric) - g_nBarGradientSizeMetric + x;
            lfTmp = ::sqrt(double(x) * double(x) + double(y) * double(y));
            double lfPixelDist = lfTmp + ::sqrt(lfTmp);
            ASSERT(lfPixelDist <= lfMaxDist);
            double lfPercent = ::sqrt(1.0 - lfPixelDist / lfMaxDist);
            double lfColorOffset[3] = {
              lfColorDist[0]* lfPercent, lfColorDist[1]* lfPercent, lfColorDist[2]* lfPercent,
            };
            double lfQ = (double(g_nBarGradientSizeMetric) - double(y) * 2.0) / double(g_nBarGradientSizeMetric);
            ASSERT((- 1.0) <= lfQ && lfQ <= (1.0));
            lfQ *= lfQ * lfQ;

            for (int i = 0; i < 3; i++) {
              double& lfCV = lfColorOffset[i];
              lfCV /= lfColorDist[i];

              if (lfQ > 0.0) {
                lfCV = lfCV + (1.0 - lfCV) * lfQ;
              }
              else if (lfQ < 0.0) {
                lfCV = lfCV * (1.0 + lfQ);
              }

              if (lfCV < 0.0) {
                lfCV = 0.0;
              }
              else if (lfCV > 1.0) {
                lfCV = 1.0;
              }

              lfCV *= lfColorDist[i];
            } // for( int i = 0; i < 3; i++ )

            *pClr = _RGB(DWORD(double(GetRV(clrQuadRight)) - lfColorOffset[0]), DWORD(double(GetGV(clrQuadRight)) - lfColorOffset[1]), DWORD(double(GetBV(clrQuadRight)) - lfColorOffset[2]));
          }
        } // for( LONG x = 0; x < g_nBarGradientSizeMetric; x++ )
      } // if( hDib != NULL && pSurfaceCB != NULL )

      // vertical bar gradient
      memset(&bih, 0, sizeof(BITMAPINFOHEADER));
      bih.biSize = sizeof(BITMAPINFOHEADER);
      bih.biWidth = bih.biHeight = g_nBarGradientSizeMetric;
      bih.biPlanes = 1;
      bih.biBitCount = 32;
      bih.biCompression = BI_RGB;
      bih.biSizeImage = g_nBarGradientPixelCount;
      pSurfaceCB = NULL;
      hDib = ::CreateDIBSection(dcDesktop, (LPBITMAPINFO) &bih, DIB_RGB_COLORS, (int**) &pSurfaceCB, NULL, NULL);
      ASSERT(hDib != NULL && pSurfaceCB != NULL);

      if (hDib != NULL && pSurfaceCB != NULL) {
        m_bmpCtBarGradientVert.Attach(hDib);

        for (LONG x = 0; x < g_nBarGradientSizeMetric; x++) {
          for (LONG y = 0; y < g_nBarGradientSizeMetric; y++) {
            COLORREF* pClr = pSurfaceCB + g_nBarGradientPixelCount - (y * g_nBarGradientSizeMetric) - g_nBarGradientSizeMetric + x;
            lfTmp = ::sqrt(double(x) * double(x) + double(y) * double(y));
            double lfPixelDist = lfTmp + ::sqrt(lfTmp);
            ASSERT(lfPixelDist <= lfMaxDist);
            double lfPercent = ::sqrt(1.0 - lfPixelDist / lfMaxDist);
            double lfColorOffset[3] = {
              lfColorDist[0]* lfPercent, lfColorDist[1]* lfPercent, lfColorDist[2]* lfPercent,
            };
            double lfQ = (double(g_nBarGradientSizeMetric) - double(x) * 2.0) / double(g_nBarGradientSizeMetric);
            ASSERT((- 1.0) <= lfQ && lfQ <= (1.0));
            lfQ *= lfQ * lfQ;

            for (int i = 0; i < 3; i++) {
              double& lfCV = lfColorOffset[i];
              lfCV /= lfColorDist[i];

              if (lfQ > 0.0) {
                lfCV = lfCV + (1.0 - lfCV) * lfQ;
              }
              else if (lfQ < 0.0) {
                lfCV = lfCV * (1.0 + lfQ);
              }

              if (lfCV < 0.0) {
                lfCV = 0.0;
              }
              else if (lfCV > 1.0) {
                lfCV = 1.0;
              }

              lfCV *= lfColorDist[i];
            } // for( int i = 0; i < 3; i++ )

            *pClr = _RGB(DWORD(double(GetRV(clrQuadRight)) - lfColorOffset[0]), DWORD(double(GetGV(clrQuadRight)) - lfColorOffset[1]), DWORD(double(GetBV(clrQuadRight)) - lfColorOffset[2]));
          }
        } // for( LONG x = 0; x < g_nBarGradientSizeMetric; x++ )
      } // if( hDib != NULL && pSurfaceCB != NULL )
    } // if use system colors

    // docker gradient
    COLORREF clrLeft = GetColor(_2003CLR_GRADIENT_LIGHT);
    COLORREF clrRight = GetColor(_2003CLR_GRADIENT_DARK);
    COLORREF clrQuadLeft = _RGB(GetBV(clrLeft), GetGV(clrLeft), GetRV(clrLeft));
    COLORREF clrQuadRight = _RGB(GetBV(clrRight), GetGV(clrRight), GetRV(clrRight));
    double lfColorDist[3] = {
      double(GetRV(clrQuadRight)) - double(GetRV(clrQuadLeft)), double(GetGV(clrQuadRight)) - double(GetGV(clrQuadLeft)), double(GetBV(clrQuadRight)) - double(GetBV(clrQuadLeft)),
    };
    BITMAPINFOHEADER bih;
    memset(&bih, 0, sizeof(BITMAPINFOHEADER));
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biWidth = bih.biHeight = g_nBarGradientSizeMetric;
    bih.biPlanes = 1;
    bih.biBitCount = 32;
    bih.biCompression = BI_RGB;
    bih.biSizeImage = g_nBarGradientPixelCount;
    COLORREF* pSurfaceDB = NULL;
    Image* hDib = ::CreateDIBSection(dcDesktop, (LPBITMAPINFO) &bih, DIB_RGB_COLORS, (int**) &pSurfaceDB, NULL, NULL);
    ASSERT(hDib != NULL && pSurfaceDB != NULL);

    if (hDib != NULL && pSurfaceDB != NULL) {
      m_bmpDockerGradient.Attach(hDib);
      double lfMaxDist = ::sqrt(double(g_nBarGradientSizeMetric) * double(g_nBarGradientSizeMetric) * 1.4 + (double(g_nBarGradientSizeMetric) * double(g_nBarGradientSizeMetric)) / (4.0 * 4.0));

      for (LONG x = 0; x < g_nBarGradientSizeMetric; x++) {
        for (LONG y = 0; y < g_nBarGradientSizeMetric; y++) {
          COLORREF* pClr = pSurfaceDB + g_nBarGradientPixelCount - (y * g_nBarGradientSizeMetric) - g_nBarGradientSizeMetric + x;
          double lfPixelDist = ::sqrt(double(x) * double(x) + (double(y) * double(y)) / (8.0 * 8.0));
          ASSERT(lfPixelDist <= lfMaxDist);
          double lfPercent = 1.0 - lfPixelDist / lfMaxDist;
          double lfColorOffset[3] = {
            lfColorDist[0]* lfPercent, lfColorDist[1]* lfPercent, lfColorDist[2]* lfPercent,
          };
          *pClr = _RGB(DWORD(double(GetRV(clrQuadLeft)) + lfColorOffset[0]), DWORD(double(GetGV(clrQuadLeft)) + lfColorOffset[1]), DWORD(double(GetBV(clrQuadLeft)) + lfColorOffset[2]));
        }
      } // for( LONG x = 0; x < g_nBarGradientSizeMetric; x++ )
    } // if( hDib != NULL && pSurfaceCB != NULL )
  } // if( stat_GetBPP() > 8 )
  else {
    m_mapColorTranslate[_2003CLR_STATUSBAR_ITEM] = InstallColor(GetColor(COLOR_3DSHADOW));
  } // else from if( stat_GetBPP() > 8 )
}
int CExtPaintManagerOffice2003::PaintControlBarClientArea(HDDC dc, const RECT* rcClient, CObject* pHelperSrc, LPARAM lParam // = 0L
                                                         )
{
  pHelperSrc;
  lParam;

  if (::IsRectEmpty(&rcClient) || (!dc.RectVisible(&rcClient))) {
    return ;
  }

  CExtToolControlBar* pToolBar = NULL;
  CWnd* pFrame = NULL;

  if (pHelperSrc != NULL) {
    pToolBar = DYNAMIC_DOWNCAST(CExtToolControlBar, pHelperSrc);

    if (pToolBar != NULL) {
      if (!(pToolBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) || pToolBar->IsKindOf(RUNTIME_CLASS(CExtPanelControlBar)))) {
        // not menu bar, not panel bar
        if (pToolBar->m_pDockSite != NULL) {
          if (pToolBar->IsFloating()) {
            pToolBar = NULL;
          }
        } // if( pToolBar->m_pDockSite != NULL )
        else {
          CWnd* pWndOwner = pToolBar->GetOwner();

          if (pWndOwner != NULL) {
            pFrame = DYNAMIC_DOWNCAST(CFrameWnd, pWndOwner);

            if (pFrame == NULL) {
              if ((pWndOwner->GetStyle() &WS_CHILD) != 0) {
                pFrame = pWndOwner->GetParentFrame();
              }
            } // if( pFrame == NULL )

            if (pFrame != NULL) {
              if (pFrame->IsKindOf(RUNTIME_CLASS(CExtMiniDockFrameWnd))) {
                // if ext mini dock frame
              } // if ext mini dock frame
              else {
                CControlBar* pDocker = ((CFrameWnd*)pFrame)->GetControlBar(AFX_IDW_DOCKBAR_TOP);

                if (pDocker == NULL) {
                  pFrame = NULL;
                  pToolBar = NULL;
                }
                else if (!pDocker->IsKindOf(RUNTIME_CLASS(CExtDockOuterBar))) {
                  pFrame = NULL;
                  pToolBar = NULL;
                }
              }
            } // if( pFrame != NULL )
          } // if( pWndOwner != NULL )
        } // else from if( pToolBar->m_pDockSite != NULL )
      } // not menu bar, not panel bar
    } // if( pToolBar != NULL )
  } // if( pHelperSrc != NULL )

  if (pToolBar == NULL || m_bmpCtBarGradientHorz == NULL || m_bmpCtBarGradientVert == NULL || m_bmpDockerGradient == NULL || stat_GetBPP() <= 8) {
    CExtPaintManagerXP::PaintControlBarClientArea(dc, rcClient, pHelperSrc, lParam);
    return ;
  }

  RECT rcPaintGradient;

  if (pFrame == NULL) {
    pFrame = (pToolBar->m_pDockSite == NULL) ? pToolBar->GetParent() : pToolBar->GetParentFrame();
  }

  ASSERT_VALID(pFrame);
  pFrame->GetClientRect(&rcPaintGradient);
  pFrame->ClientToScreen(&rcPaintGradient);
  pToolBar->ScreenToClient(&rcPaintGradient);

  if (::IsRectEmpty(&rcPaintGradient) || (!dc.RectVisible(&rcPaintGradient))) {
    return ;
  }

  HDDC dcCompat = CachedDcGet(m_bmpDockerGradient);

  if (dcCompat == NULL) {
    ASSERT(FALSE);
    return ;
  }

  int nOldStretchBltMode = dc.SetStretchBltMode(m_bIsWinNT ? HALFTONE : COLORONCOLOR);
  ::SetBrushOrgEx(dc, 0, 0, NULL);
  dc.StretchBlt(rcPaintGradient->left, rcPaintGradient->top, rcPaintGradient->Width(), rcPaintGradient->Height(), &dcCompat, 0, 0, m_sizeDockerGradient.w, m_sizeDockerGradient.h, SRCCOPY);
  dc.SetStretchBltMode(nOldStretchBltMode);
  BOOL bHorz = TRUE;

  if (pToolBar->IsDocked()) {
    bHorz = (!pToolBar->IsDockedVertically());
  }

  if (pToolBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) || pToolBar->IsKindOf(RUNTIME_CLASS(CExtPanelControlBar)) || pToolBar->m_pDockSite == NULL) {
    // if dockbar-like background style
    return ;
  } // if dockbar-like background style

  pToolBar->GetWindowRect(&rcPaintGradient);
  pToolBar->ScreenToClient(&rcPaintGradient);
  Image* pBmpOld = dcCompat.SelectObject(bHorz ? (&m_bmpCtBarGradientHorz) : (&m_bmpCtBarGradientVert));
  SIZE sizeShade = bHorz ? m_sizeCtBarGradientHorz : m_sizeCtBarGradientVert;
  nOldStretchBltMode = dc.SetStretchBltMode(m_bIsWinNT ? HALFTONE : COLORONCOLOR);
  ::SetBrushOrgEx(dc, 0, 0, NULL);
  int nIdxClrTbFillMargin = - 1;
  CRgn _rgnBaloonRight, _rgnBaloonLeft, _rgnBaloonSet;

  if (m_sizeToolBarRgnRounders.w > 0 && m_sizeToolBarRgnRounders.h > 0 && pToolBar->IsDocked()) {
    // if baloon style
    nIdxClrTbFillMargin = m_nIdxClrTbFillMargin;

    if (_rgnBaloonRight.CreateRoundRectRgn(0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, m_sizeToolBarRgnRounders.w, m_sizeToolBarRgnRounders.h) && _rgnBaloonLeft.CreateRectRgn(0, 0, bHorz ? (m_sizeToolBarRgnRounders.w + 2) : (rcClient.right - rcClient.left - 1), bHorz ? (rcClient.bottom - rcClient.top - 1) : (m_sizeToolBarRgnRounders.h + 2)) && _rgnBaloonSet.CreateRectRgn(0, 0, 1, 1) && _rgnBaloonSet.CombineRgn(&_rgnBaloonLeft, &_rgnBaloonRight, RGN_OR) != ERROR) {
      dc.SelectClipRgn(&_rgnBaloonSet);
    }

#ifdef _DEBUG
    else {
      ASSERT(FALSE);
    }

#endif // _DEBUG
  } // if baloon style

  dc.StretchBlt(rcPaintGradient->left, rcPaintGradient->top, rcPaintGradient->Width(), rcPaintGradient->Height(), &dcCompat, 0, 0, sizeShade.w, sizeShade.h, SRCCOPY);

  if (nIdxClrTbFillMargin >= 0) {
    COLORREF _pen(PS_SOLID, 1, GetColor(m_nIdxClrTbFillMargin));
    COLORREF* pOldPen = dc.SelectObject(&_pen);
    RECT rcClient;
    pToolBar->GetClientRect(&rcClient);

    if (bHorz) {
      TMoveTo(dc, rcClient.left, rcClient.bottom - 2);
      TLineTo(dc, rcClient.right, rcClient.bottom - 2);
    } // if( bHorz )
    else {
      TMoveTo(dc, rcClient.right - 2, rcClient.top);
      TLineTo(dc, rcClient.right - 2, rcClient.bottom);
    } // else from if( bHorz )

    dc.SelectObject(pOldPen);
  } // if( nIdxClrTbFillMargin >= 0 )

  if (_rgnBaloonSet != NULL) {
    dc.SelectClipRgn(NULL);
  }

  dc.SetStretchBltMode(nOldStretchBltMode);
  dcCompat.SelectObject(pBmpOld);
}
int CExtPaintManagerOffice2003::PaintDockBarClientArea(HDDC dc, const RECT* rcClient, CObject* pHelperSrc, LPARAM lParam // = 0L
                                                      )
{
  ASSERT(dc != NULL);
  pHelperSrc;
  lParam;

  if (::IsRectEmpty(&rcClient) || (!dc.RectVisible(&rcClient))) {
    return ;
  }

  RECT rcPaintGradient(0, 0, 0, 0);
  CExtDockOuterBar* pOuterDocker = NULL;

  if (pHelperSrc != NULL) {
    pOuterDocker = DYNAMIC_DOWNCAST(CExtDockOuterBar, pHelperSrc);

    if (pOuterDocker != NULL) {
      CFrameWnd* pFrame = pOuterDocker->GetParentFrame();
      ASSERT_VALID(pFrame);
      pFrame->GetClientRect(&rcPaintGradient);
      pFrame->ClientToScreen(&rcPaintGradient);
      pOuterDocker->ScreenToClient(&rcPaintGradient);

      if (::IsRectEmpty(&rcPaintGradient) || (!dc.RectVisible(&rcPaintGradient))) {
        return ;
      }
    }
  }

  if (pOuterDocker == NULL || m_bmpDockerGradient == NULL || stat_GetBPP() <= 8) {
    CExtPaintManagerXP::PaintDockBarClientArea(dc, rcClient, pHelperSrc, lParam);
    return ;
  }

  HDDC dcCompat = CachedDcGet(m_bmpDockerGradient);

  if (dcCompat == NULL) {
    ASSERT(FALSE);
    return ;
  }

  int nOldStretchBltMode = dc.SetStretchBltMode(m_bIsWinNT ? HALFTONE : COLORONCOLOR);
  ::SetBrushOrgEx(dc, 0, 0, NULL);
  dc.StretchBlt(rcPaintGradient->left, rcPaintGradient->top, rcPaintGradient->Width(), rcPaintGradient->Height(), &dcCompat, 0, 0, m_sizeDockerGradient.w, m_sizeDockerGradient.h, SRCCOPY);
  dc.SetStretchBltMode(nOldStretchBltMode);
}
PAINTDOCKINGFRAMEDATA::PAINTDOCKINGFRAMEDATA(): m_pHelperSrc(NULL), m_lParam(0L), m_rcWindow(0, 0, 0, 0), m_rcClient(0, 0, 0, 0), m_bFloating(FALSE), m_bExtBar(FALSE), m_bSideBar(FALSE) {}
PAINTDOCKINGFRAMEDATA::PAINTDOCKINGFRAMEDATA(CObject* pHelperSrc, const RECT* rcWindow, const RECT* rcClient, BOOL bFloating, BOOL bExtBar, BOOL bSideBar // = FALSE
                                            ): m_pHelperSrc(pHelperSrc), m_lParam(0L), m_rcWindow(rcWindow), m_rcClient(rcClient), m_bFloating(bFloating), m_bExtBar(bExtBar), m_bSideBar(bSideBar) {}
int PaintDockingFrame(HDDC dc, PAINTDOCKINGFRAMEDATA& _pdfd)
{
  if (_pdfd.m_rcWindow IsRectEmpty(&)) {
    return ;
  }

  dc.ExcludeClipRect(_pdfd.m_rcClient);
  RECT rcWnd(_pdfd.m_rcWindow);
  COLORREF clr3dFace = GetColor(CLR_3DFACE_OUT);
  COLORREF clr3dHilight = GetColor(CLR_3DHILIGHT_OUT);
  COLORREF clr3dShadow = GetColor(CLR_3DSHADOW_OUT);
  COLORREF clr3dDkShadow = GetColor(CLR_3DDKSHADOW_OUT);
  TFillRect(dc, &rcWnd, clr3dFace);

  if (_pdfd.m_bFloating) {
    TDraw3DRect(dc, &rcWnd, clr3dFace, clr3dDkShadow);
    rcWnd DeflateRect(&, 1, 1);
    TDraw3DRect(dc, &rcWnd, clr3dHilight, clr3dShadow);
  }
  else {
    if (!_pdfd.m_bExtBar) {
      TDraw3DRect(dc, &rcWnd, clr3dHilight, clr3dShadow);
    }
  }

  dc.IntersectClipRect(_pdfd.m_rcWindow);
}
int CExtPaintManagerXP::PaintDockingFrame(HDDC dc, PAINTDOCKINGFRAMEDATA& _pdfd)
{
  if (_pdfd.m_rcWindow IsRectEmpty(&)) {
    return ;
  }

  dc.ExcludeClipRect(_pdfd.m_rcClient);
  RECT rcWnd(_pdfd.m_rcWindow);
  COLORREF clr3dFace = GetColor(COLOR_3DFACE);
  COLORREF clrDark = GetColor(COLOR_3DSHADOW);
  COLORREF clrBkOld = dc.GetBkColor();
  COLORREF clrBkNew = GetColor((_pdfd.m_bFloating) ? (_pdfd.m_bExtBar ? XPCLR_3DFACE_FLOAT_R : XPCLR_3DFACE_FLOAT_F) : CLR_3DFACE_OUT);
  TFillRect(dc, &rcWnd, clrBkNew);

  if (_pdfd.m_bFloating) {
    TDraw3DRect(dc, &rcWnd, clrDark, clrDark);
    rcWnd DeflateRect(&, 1, 1);
    TDraw3DRect(dc, &rcWnd, clrDark, clrDark);
    rcWnd DeflateRect(&, 1, 1, 2, 2);
    _SetPixel(dc, rcWnd.left, rcWnd.top, clrDark);
    _SetPixel(dc, rcWnd.left, rcWnd.bottom, clrDark);
    _SetPixel(dc, rcWnd.right, rcWnd.top, clrDark);
    _SetPixel(dc, rcWnd.right, rcWnd.bottom, clrDark);
  } // if( _pdfd.m_bFloating )
  else {
    if (!_pdfd.m_bExtBar) {
      TDraw3DRect(dc, &rcWnd, clr3dFace, clr3dFace);
    }

    if (!_pdfd.m_bSideBar && stat_GetBPP() <= 8) {
      static WORD hatchBits[8] = {
        0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,
      };
      COLORREF br;
      Image* bmp;
      bmp.CreateBitmap(8, 8, 1, 1, hatchBits);
      br.CreatePatternBrush(&bmp);
      LOGBRUSH lbr;
      br.GetLogBrush(&lbr);
      HPEN hPenDesired = ::ExtCreatePen(PS_GEOMETRIC, 1, &lbr, 0, NULL);

      if (hPenDesired == NULL) {
        COLORREF clrDesiredSolidPen = GetColor(COLOR_3DSHADOW);
        hPenDesired = ::CreatePen(PS_SOLID, 1, clrDesiredSolidPen);

        if (hPenDesired == NULL) {
          hPenDesired = (HPEN)GetStockObject(BLACK_PEN);
          ASSERT(hPenDesired != NULL);
        } // if( hPenDesired == NULL )
      } // if( hPenDesired == NULL )

      // COLORREF pen( PS_GEOMETRIC, 1, &lbr );
      COLORREF pen;
      VERIFY(pen.Attach(hPenDesired));
      COLORREF* pOldPen = dc.SelectObject(&pen);

      if (m_bHelperXpStyle8BitBarRect) {
        TMoveTo(dc, rcWnd.left, rcWnd.top);
        TLineTo(dc, rcWnd.right - 1, rcWnd.top);
        TLineTo(dc, rcWnd.right - 1, rcWnd.bottom - 1);
        TLineTo(dc, rcWnd.left, rcWnd.bottom - 1);
        TLineTo(dc, rcWnd.left, rcWnd.top);
      } // if( m_bHelperXpStyle8BitBarRect )
      else {
        static const SIZE g_sizeRO = {
          2, 2
        };
        TMoveTo(dc, rcWnd.left + g_sizeRO.w, rcWnd.top);
        TLineTo(dc, rcWnd.right - 1 - g_sizeRO.w, rcWnd.top);
        TLineTo(dc, rcWnd.right - 1, rcWnd.top + g_sizeRO.h);
        TLineTo(dc, rcWnd.right - 1, rcWnd.bottom - 1 - g_sizeRO.h);
        TLineTo(dc, rcWnd.right - 1 - g_sizeRO.w, rcWnd.bottom - 1);
        TLineTo(dc, rcWnd.left + g_sizeRO.w, rcWnd.bottom - 1);
        TLineTo(dc, rcWnd.left, rcWnd.bottom - 1 - g_sizeRO.h);
        TLineTo(dc, rcWnd.left, rcWnd.top + g_sizeRO.h);
        TLineTo(dc, rcWnd.left + g_sizeRO.w, rcWnd.top);
      } // else from if( m_bHelperXpStyle8BitBarRect )

      dc.SelectObject(pOldPen);
    } // if( !_pdfd.m_bSideBar && stat_GetBPP() < 8 )
  } // else from if( _pdfd.m_bFloating )

  dc.SetBkColor(clrBkOld);
  //dc.IntersectClipRect(rectWindow);
  dc.SelectClipRgn(NULL);
}
int CExtPaintManagerOffice2003::PaintDockingFrame(HDDC dc, PAINTDOCKINGFRAMEDATA& _pdfd)
{
  if (_pdfd.m_rcWindow IsRectEmpty(&)) {
    return ;
  }

  if (_pdfd.m_bFloating
      //|| (!_pdfd.m_bExtBar)
      //|| _pdfd.m_bSideBar
      || _pdfd.m_pHelperSrc == NULL || (!_pdfd.m_pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtControlBar))) || m_bmpCtBarGradientHorz == NULL || m_bmpCtBarGradientVert == NULL || m_bmpDockerGradient == NULL || stat_GetBPP() <= 8) {
    CExtPaintManagerXP::PaintDockingFrame(dc, _pdfd);
    return ;
  }

  CExtControlBar* pControlBar = STATIC_DOWNCAST(CExtControlBar, _pdfd.m_pHelperSrc);
  ASSERT_VALID(pControlBar);
  CWnd* pFrame = NULL;

  if (pControlBar->m_pDockSite == NULL) {
    CExtToolControlBar* pToolBar = DYNAMIC_DOWNCAST(CExtToolControlBar, pControlBar);

    if (pToolBar != NULL) {
      if (!(pToolBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) || pToolBar->IsKindOf(RUNTIME_CLASS(CExtPanelControlBar)))) {
        // not menu bar, not panel bar
        CWnd* pWndOwner = pToolBar->GetOwner();

        if (pWndOwner != NULL) {
          pFrame = DYNAMIC_DOWNCAST(CFrameWnd, pWndOwner);

          if (pFrame == NULL) {
            if ((pWndOwner->GetStyle() &WS_CHILD) != 0) {
              pFrame = pWndOwner->GetParentFrame();
            }
          } // if( pFrame == NULL )

          if (pFrame != NULL) {
            if (pFrame->IsKindOf(RUNTIME_CLASS(CExtMiniDockFrameWnd))) {
              // if ext mini dock frame
            } // if ext mini dock frame
            else {
              CControlBar* pDocker = ((CFrameWnd*)pFrame)->GetControlBar(AFX_IDW_DOCKBAR_TOP);

              if (pDocker == NULL) {
                pFrame = NULL;
              }
              else if (!pDocker->IsKindOf(RUNTIME_CLASS(CExtDockOuterBar))) {
                pFrame = NULL;
              }
            }
          } // if( pFrame != NULL )
        } // if( pWndOwner != NULL )
      } // not menu bar, not panel bar
    } // if( pToolBar != NULL )
  } // if( pControlBar->m_pDockSite == NULL )

  if (pFrame == NULL) {
    pFrame = (pControlBar->m_pDockSite == NULL) ? pControlBar->GetParent() : pControlBar->GetParentFrame();
  }

  ASSERT_VALID(pFrame);
  RECT rcPaintGradient;
  pFrame->GetClientRect(&rcPaintGradient);
  pFrame->ClientToScreen(&rcPaintGradient);
  RECT rcBar;
  pControlBar->GetWindowRect(&rcBar);
  rcPaintGradient->OffsetRect(- rcBar.TopLeft());

  if (::IsRectEmpty(&rcPaintGradient) || (!dc.RectVisible(&rcPaintGradient))) {
    return ;
  }

  HDDC dcCompat = CachedDcGet(m_bmpDockerGradient);

  if (dcCompat == NULL) {
    ASSERT(FALSE);
    return ;
  }

  int nOldStretchBltMode = dc.SetStretchBltMode(m_bIsWinNT ? HALFTONE : COLORONCOLOR);
  ::SetBrushOrgEx(dc, 0, 0, NULL);
  dc.StretchBlt(rcPaintGradient->left, rcPaintGradient->top, rcPaintGradient->Width(), rcPaintGradient->Height(), &dcCompat, 0, 0, m_sizeDockerGradient.w, m_sizeDockerGradient.h, SRCCOPY);
  dc.SetStretchBltMode(nOldStretchBltMode);
}
PAINTGRIPPERDATA::PAINTGRIPPERDATA(): m_pHelperSrc(NULL), m_lParam(0L), m_rcGripper(0, 0, 0, 0), m_rcText(0, 0, 0, 0), m_bActive(FALSE), m_bFloating(FALSE), m_bHorz(FALSE), m_bSideBar(FALSE), m_sCaption(_T("")), m_bHelperNoRect(FALSE), m_bHelperNoFill(FALSE) {}
PAINTGRIPPERDATA::PAINTGRIPPERDATA(CObject* pHelperSrc, const RECT* rcGripper, const RECT* rcText, BOOL bActive, BOOL bFloating, BOOL bHorz, BOOL bSideBar, // = FALSE
    __EXT_MFC_SAFE_LPCTSTR sCaption // = NULL
                                  ): m_pHelperSrc(pHelperSrc), m_lParam(0L), m_rcGripper(rcGripper), m_rcText(rcText), m_bActive(bActive), m_bFloating(bFloating), m_bHorz(bHorz), m_bSideBar(bSideBar), m_sCaption((sCaption == NULL) ? _T("") : sCaption), m_bHelperNoRect(FALSE), m_bHelperNoFill(FALSE) {}
int PaintGripper(HDDC dc, PAINTGRIPPERDATA& _pgd)
{
  RECT rectGripper2(_pgd.m_rcGripper);
  COLORREF clrBtnShadow = GetColor(CLR_3DSHADOW_OUT);
  COLORREF clrBtnHilight = GetColor(CLR_3DHILIGHT_OUT);

  if (_pgd.m_bSideBar || _pgd.m_bFloating) {
    if (!_pgd.m_bFloating) {
      rectGripper2 DeflateRect(&, 1, 1);
    }

#if (defined COLOR_GRADIENTACTIVECAPTION && defined COLOR_GRADIENTACTIVECAPTION)
    ASSERT(COLOR_GRADIENTACTIVECAPTION == 27);
    ASSERT(COLOR_GRADIENTINACTIVECAPTION == 28);
#endif
    INT nTextColorIndex = COLOR_HIGHLIGHTTEXT;

    if (_pgd.m_bSideBar) {
      if (_pgd.m_bFloating || _pgd.m_bActive) {
        nTextColorIndex = COLOR_CAPTIONTEXT;

        if (stat_GetBPP() > 8) {
          stat_PaintGradientRect(dc, rectGripper2, GetColor(COLOR_ACTIVECAPTION), GetColor(27), _pgd.m_bHorz);
        } // if( stat_GetBPP() > 8 )
        else {
          TFillRect(dc, rectGripper2, GetColor(COLOR_ACTIVECAPTION));
        } // else from if( stat_GetBPP() > 8 )
      }
      else {
        nTextColorIndex = COLOR_INACTIVECAPTIONTEXT;

        if (stat_GetBPP() > 8) {
          stat_PaintGradientRect(dc, rectGripper2, GetColor(COLOR_INACTIVECAPTION), GetColor(28), _pgd.m_bHorz);
        } // if( stat_GetBPP() > 8 )
        else {
          TFillRect(dc, rectGripper2, GetColor(COLOR_INACTIVECAPTION));
        } // else from if( stat_GetBPP() > 8 )
      }
    } // if( _pgd.m_bSideBar )
    else {
      TFillRect(dc, rectGripper2, GetColor(COLOR_HIGHLIGHT));
    } // else from if( _pgd.m_bSideBar )

    int nTextLen = 0;

    if (_pgd.m_sCaption != NULL && (nTextLen = _tcslen(_pgd.m_sCaption)) > 0 && !_pgd.m_rcText IsRectEmpty(&) && _pgd.m_rcText.right > _pgd.m_rcText.left && _pgd.m_rcText.bottom > _pgd.m_rcText.top) {
      COLORREF clrText = GetColor(nTextColorIndex /* COLOR_HIGHLIGHTTEXT */
                                 );
      COLORREF clrOldText = textColor = (clrText);
      int nOldBkMode = dc.SetBkMode(TRANSPARENT);
      HFONT pCurrFont = (!_pgd.m_bHorz) ? m_FontBoldBC : m_FontBoldVertXBC;
      HFONT pOldFont = dc.SelectObject(pCurrFont);
      ASSERT(pOldFont != NULL);

      if (!_pgd.m_bHorz) {
        // if text is horizontal
        TDrawText(dc, _pgd.m_sCaption, nTextLen, (LPRECT) &_pgd.m_rcText, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);
      } // if text is horizontal
      else {
        // if text is vertical
        LOGFONT lf;
        memset(&lf, 0, sizeof(LOGFONT));
        pCurrFont->GetLogFont(&lf);
        int _cyHorzFont = abs(lf.lfHeight);
        int _cyTextMargin = (_pgd.m_rcText RCWIDTH(&) - _cyHorzFont) / 2;
        RECT rcString = RECT(POINT(_pgd.m_rcText.left + _cyTextMargin - __ExtMfc_CXTEXTMARGIN, _pgd.m_rcText.bottom - __ExtMfc_CYTEXTMARGIN), _pgd.m_rcText.Size() //m_sizeHorz
                            );
        rcString DeflateRect(&, 2, 2);
        RECT rcExclude;
        rcExclude.SetRect(_pgd.m_rcGripper.left, _pgd.m_rcGripper.top, _pgd.m_rcGripper.right, _pgd.m_rcText.top);
        dc.ExcludeClipRect(&rcExclude);
        TDrawText(dc, _pgd.m_sCaption, nTextLen, rcString, DT_SINGLELINE //|DT_CENTER|DT_VCENTER
            | DT_NOCLIP | DT_NOPREFIX); // don't forget DT_NOCLIP
        //dc.IntersectClipRect( &rectGripper );
        dc.SelectClipRgn(NULL);
      } // if text is vertical

      dc.SelectObject(pOldFont);
      dc.SetBkMode(nOldBkMode);
      textColor = (clrOldText);
    }
  } // if( _pgd.m_bSideBar || _pgd.m_bFloating )
  else {
    if (_pgd.m_bHorz) {
      rectGripper2 DeflateRect(&, 4, 1, 4, 0);
      rectGripper2.right = rectGripper2.left + 3;
      TDraw3DRect(dc, &rectGripper2, clrBtnHilight, clrBtnShadow);
      rectGripper2 OffsetRect(&, 3, 0);
      TDraw3DRect(dc, &rectGripper2, clrBtnHilight, clrBtnShadow);
    } // if( bHorz )
    else {
      rectGripper2 OffsetRect(&, 1, 0);
      rectGripper2 DeflateRect(&, 0, 4, 1, 4);
      rectGripper2.bottom = rectGripper2.top + 3;
      TDraw3DRect(dc, &rectGripper2, clrBtnHilight, clrBtnShadow);
      rectGripper2 OffsetRect(&, 0, 3);
      TDraw3DRect(dc, &rectGripper2, clrBtnHilight, clrBtnShadow);
    } // else from if( bHorz )
  } // else from if( _pgd.m_bSideBar || _pgd.m_bFloating )
}
int CExtPaintManagerXP::PaintGripper(HDDC dc, PAINTGRIPPERDATA& _pgd)
{
  RECT rectGripper2(_pgd.m_rcGripper);

  if (_pgd.m_bSideBar || _pgd.m_bFloating) {
    COLORREF clrGrip = GetColor(
        //COLOR_HIGHLIGHT
        _pgd.m_bSideBar ? (_pgd.m_bActive ? COLOR_HIGHLIGHT : XPCLR_3DFACE_DARK) : COLOR_3DSHADOW);

    if (!_pgd.m_bHelperNoFill) {
      TFillRect(dc, rectGripper2, clrGrip);
    }

    if (_pgd.m_bSideBar && (!_pgd.m_bHelperNoFill) && (!_pgd.m_bFloating) && (!_pgd.m_bActive)) {
      // rect border of docked bar caption like VS-.NET-7.0
      COLORREF clrSideRect = GetColor(COLOR_3DSHADOW);
      stat_PaintDotNet3dRect(dc, rectGripper2, clrSideRect, clrSideRect);
    } // rect border of docked bar caption like VS-.NET-7.0

    int nTextLen = 0;

    if (_pgd.m_sCaption != NULL && (nTextLen = _tcslen(_pgd.m_sCaption)) > 0 && !_pgd.m_rcText IsRectEmpty(&) && _pgd.m_rcText.right > _pgd.m_rcText.left && _pgd.m_rcText.bottom > _pgd.m_rcText.top) {
      COLORREF clrText = GetColor((_pgd.m_bSideBar) ? (_pgd.m_bActive ? COLOR_HIGHLIGHTTEXT : COLOR_BTNTEXT) : COLOR_HIGHLIGHTTEXT);
      COLORREF clrOldText = textColor = (clrText);
      int nOldBkMode = dc.SetBkMode(TRANSPARENT);
      HFONT pCurrFont = (!_pgd.m_bHorz) ? (_pgd.m_bSideBar ? m_FontNormalBC : m_FontBoldBC) : (_pgd.m_bSideBar ? m_FontNormalVertXBC : m_FontBoldVertXBC);
      HFONT pOldFont = dc.SelectObject(pCurrFont);
      ASSERT(pOldFont != NULL);

      if (!_pgd.m_bHorz) {
        // if text is horizontal
        TDrawText(dc, _pgd.m_sCaption, nTextLen, (LPRECT) &_pgd.m_rcText, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);
      } // if text is horizontal
      else {
        // if text is vertical
        LOGFONT lf;
        memset(&lf, 0, sizeof(LOGFONT));
        pCurrFont->GetLogFont(&lf);
        int _cyHorzFont = abs(lf.lfHeight);
        int _cyTextMargin = (_pgd.m_rcText RCWIDTH(&) - _cyHorzFont) / 2;
        RECT rcString = RECT(POINT(_pgd.m_rcText.left + _cyTextMargin - __ExtMfc_CXTEXTMARGIN, _pgd.m_rcText.bottom - __ExtMfc_CYTEXTMARGIN), _pgd.m_rcText.Size() //m_sizeHorz
                            );
        rcString DeflateRect(&, 2, 2);
        RECT rcExclude;
        rcExclude.SetRect(_pgd.m_rcGripper.left, _pgd.m_rcGripper.top, _pgd.m_rcGripper.right, _pgd.m_rcText.top);
        dc.ExcludeClipRect(&rcExclude);
        TDrawText(dc, _pgd.m_sCaption, nTextLen, rcString, DT_SINGLELINE //|DT_CENTER|DT_VCENTER
            | DT_NOCLIP | DT_NOPREFIX); // don't forget DT_NOCLIP
        //dc.IntersectClipRect( &rectGripper );
        dc.SelectClipRgn(NULL);
      } // if text is vertical

      dc.SelectObject(pOldFont);
      dc.SetBkMode(nOldBkMode);
      textColor = (clrOldText);
    }
  } // if( _pgd.m_bSideBar || _pgd.m_bFloating )
  else {
    COLORREF clrGrip = GetColor(CLR_3DFACE_OUT);
    RECT rcGripToClear(_pgd.m_rcGripper);
    rcGripToClear DeflateRect(&, 1, 1);
    TFillRect(dc, rcGripToClear, clrGrip);
    int nBitsPerPixel = stat_GetBPP();
    static WORD horzHatchBits[8] = {
      0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00
    };
    static WORD vertHatchBits[8] = {
      0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA
    };

    if (_pgd.m_bHorz) {
      COLORREF brGripperHorz;
      Image* bmpGripperHorz;

      if (nBitsPerPixel <= 8) {
        bmpGripperHorz.CreateBitmap(8, 8, 1, 1, horzHatchBits);
      }
      else {
        HDDC mdc;
        VERIFY(mdc.CreateCompatibleDC(&dc));
        VERIFY(bmpGripperHorz.CreateCompatibleBitmap(&dc, 8, 8));
        Image* pOldBmp = mdc.SelectObject(&bmpGripperHorz);
        mTFillRect(dc, 0, 0, 8, 8, clrGrip);
        COLORREF clr1 = GetColor(COLOR_3DFACE);
        COLORREF clr2 = GetColor(COLOR_3DSHADOW);
        COLORREF clrGripLines = _RGB((GetRV(clr1) * 2 + GetRV(clr2)) / 3, (GetGV(clr1) * 2 + GetGV(clr2)) / 3, (GetBV(clr1) * 2 + GetBV(clr2)) / 3);
        COLORREF pen(PS_SOLID, 1, mdc.GetNearestColor(clrGripLines));
        COLORREF* pOldPen = mdc.SelectObject(&pen);

        for (INT nPos = 0; nPos < 8; nPos += 2) {
          TMoveTo(mdc, 0, nPos);
          mTLineTo(dc, 8, nPos);
        }

        mdc.SelectObject(pOldPen);
        mdc.SelectObject(pOldBmp);
      } // else from if( nBitsPerPixel < 8 )

      brGripperHorz.CreatePatternBrush(&bmpGripperHorz);
      rectGripper2 DeflateRect(&, 5, 2);
      rectGripper2.right = rectGripper2.left + 5;
      rectGripper2 DeflateRect(&, 1, 0);
      COLORREF clrOldBk = dc.SetBkColor(GetColor(XPCLR_3DFACE_NORMAL));
      COLORREF* pBrushOld = dc.SelectObject(&brGripperHorz);
      dc.PatBlt(rectGripper2.left, rectGripper2.top, rectGripper2 RCWIDTH(&), rectGripper2 RCHEIGHT(&), 0xA000C9);
      dc.SelectObject(pBrushOld);
      dc.SetBkColor(clrOldBk);
    } // if( _pgd.m_bHorz )
    else {
      COLORREF brGripperVert;
      Image* bmpGripperVert;

      if (nBitsPerPixel <= 8) {
        bmpGripperVert.CreateBitmap(8, 8, 1, 1, vertHatchBits);
      }
      else {
        HDDC mdc;
        VERIFY(mdc.CreateCompatibleDC(&dc));
        VERIFY(bmpGripperVert.CreateCompatibleBitmap(&dc, 8, 8));
        Image* pOldBmp = mdc.SelectObject(&bmpGripperVert);
        mTFillRect(dc, 0, 0, 8, 8, clrGrip);
        COLORREF clr1 = GetColor(COLOR_3DFACE);
        COLORREF clr2 = GetColor(COLOR_3DSHADOW);
        COLORREF clrGripLines = _RGB((GetRV(clr1) * 2 + GetRV(clr2)) / 3, (GetGV(clr1) * 2 + GetGV(clr2)) / 3, (GetBV(clr1) * 2 + GetBV(clr2)) / 3);
        COLORREF pen(PS_SOLID, 1, mdc.GetNearestColor(clrGripLines));
        COLORREF* pOldPen = mdc.SelectObject(&pen);

        for (INT nPos = 0; nPos < 8; nPos += 2) {
          TMoveTo(mdc, nPos, 0);
          mTLineTo(dc, nPos, 8);
        }

        mdc.SelectObject(pOldPen);
        mdc.SelectObject(pOldBmp);
      } // else from if( nBitsPerPixel < 8 )

      brGripperVert.CreatePatternBrush(&bmpGripperVert);
      rectGripper2 OffsetRect(&, 1, 0);
      rectGripper2 DeflateRect(&, 2, 5);
      rectGripper2.bottom = rectGripper2.top + 5;
      rectGripper2 DeflateRect(&, 0, 1);
      COLORREF clrOldBk = dc.SetBkColor(GetColor(XPCLR_3DFACE_NORMAL));
      COLORREF* pBrushOld = dc.SelectObject(&brGripperVert);
      dc.PatBlt(rectGripper2.left, rectGripper2.top, rectGripper2 RCWIDTH(&), rectGripper2 RCHEIGHT(&), 0xA000C9);
      dc.SelectObject(pBrushOld);
      dc.SetBkColor(clrOldBk);
    } // else from if( _pgd.m_bHorz )
  } // else from if( _pgd.m_bSideBar || _pgd.m_bFloating )
}
int CExtPaintManagerOffice2003::PaintGripper(HDDC dc, PAINTGRIPPERDATA& _pgd)
{
  BOOL bStyleOffice2003 = TRUE;

  if (_pgd.m_bFloating
      //|| _pgd.m_bSideBar
      || _pgd.m_pHelperSrc == NULL || (!(_pgd.m_pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtControlBar))
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
          || _pgd.m_pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtDynAutoHideSlider))
#endif
                                        )) || m_bmpCtBarGradientHorz == NULL || m_bmpCtBarGradientVert == NULL || m_bmpDockerGradient == NULL || stat_GetBPP() <= 8) {
    bStyleOffice2003 = FALSE;
  }

  CWnd* pBar = NULL;
  BOOL bForceNoBaloon = FALSE;
  BOOL bInactiveResizableBarCapt = FALSE;
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

  if (bStyleOffice2003 && _pgd.m_pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtDynAutoHideSlider))) {
    // auto-hide slider
    pBar = STATIC_DOWNCAST(CWnd, _pgd.m_pHelperSrc);
    bInactiveResizableBarCapt = TRUE;
  } // auto-hide slider
  else
#endif
    if (bStyleOffice2003) {
      // non-dockers
      pBar = STATIC_DOWNCAST(CWnd, _pgd.m_pHelperSrc);
      ASSERT_VALID(pBar);

      if (_pgd.m_pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtPanelControlBar))) {
        // if panel bar
        bForceNoBaloon = TRUE;
      } // if panel bar

      if (_pgd.m_pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtControlBar))) {
        // if control bar
        if (((CExtControlBar*)pBar)->m_pDockSite != NULL) {
          // if docked
          if (((CExtControlBar*)pBar)->IsFloating()) {
            bStyleOffice2003 = FALSE;
          }
        } // if docked
        else {
          // if simple
          bForceNoBaloon = TRUE;
        } // if simple
      } // if control bar
      else {
        // if NOT control bar
        bStyleOffice2003 = FALSE;
      } // if NOT control bar
    } // non-dockers

  if (!bStyleOffice2003) {
    CExtPaintManagerXP::PaintGripper(dc, _pgd);
    return ;
  } // if( !bStyleOffice2003 )

  ASSERT_VALID(pBar);
  RECT rcBarWnd, rcBarClient;
  pBar->GetWindowRect(&rcBarWnd);
  pBar->GetClientRect(&rcBarClient);
  pBar->ClientToScreen(&rcBarClient);

  if (_pgd.m_bSideBar || bInactiveResizableBarCapt) {
    // if resizable bar caption
    _pgd.m_bHelperNoFill = _pgd.m_bHelperNoRect = !_pgd.m_bActive;

    if (_pgd.m_bSideBar) {
      bInactiveResizableBarCapt = !_pgd.m_bActive;
    }

    if (bInactiveResizableBarCapt) {
      RECT rcHelper(rcBarClient);
      rcHelper OffsetRect(&, - rcBarWnd.TopLeft());
      RECT rcGripper(_pgd.m_rcGripper);
      rcGripper.bottom = rcHelper.top + 4;
      stat_PaintGradientRect(dc, rcGripper, GetColor(_2003CLR_GRADIENT_LIGHT), GetColor(_2003CLR_GRADIENT_DARK));
    } // if( bInactiveResizableBarCapt )

    CExtPaintManagerXP::PaintGripper(dc, _pgd);
    return ;
  } // if resizable bar caption

  if ((!pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar))) && (!bForceNoBaloon)) {
    // if toolbar balloon style
    HDDC dcCompat = CachedDcGet(_pgd.m_bHorz ? m_bmpCtBarGradientHorz : m_bmpCtBarGradientVert);

    if (dcCompat == NULL) {
      ASSERT(FALSE);
      return ;
    }

    RECT rcPaintGradient(rcBarWnd);

    if (_pgd.m_bHorz) {
      rcPaintGradient->top = rcBarClient.top;
      rcPaintGradient->bottom = rcBarClient.bottom; // + 1;
      rcPaintGradient->right = rcBarClient.right;
      rcPaintGradient->OffsetRect(- rcPaintGradient->left, - rcPaintGradient->top + rcBarClient.top - rcBarWnd.top);
    } // if( _pgd.m_bHorz )
    else {
      rcPaintGradient->left = rcBarClient.left;
      rcPaintGradient->right = rcBarClient.right; // + 1;
      rcPaintGradient->bottom = rcBarClient.bottom;
      rcPaintGradient->OffsetRect(- rcPaintGradient->left + rcBarClient.left - rcBarWnd.left, - rcPaintGradient->top);
    } // else from if( _pgd.m_bHorz )

    int nIdxClrTbFillMargin = - 1;
    CRgn _rgnBaloonSet;

    if (m_sizeToolBarRgnRounders.w > 0 && m_sizeToolBarRgnRounders.h > 0) {
      // if baloon style
      nIdxClrTbFillMargin = m_nIdxClrTbFillMargin;

      if (_rgnBaloonSet.CreateRoundRectRgn(rcPaintGradient->left, rcPaintGradient->top, rcPaintGradient->Width(), rcPaintGradient->Height(), m_sizeToolBarRgnRounders.w, m_sizeToolBarRgnRounders.h)) {
        dc.SelectClipRgn(&_rgnBaloonSet);
      }

#ifdef _DEBUG
      else {
        ASSERT(FALSE);
      }

#endif // _DEBUG
    } // if baloon style

    SIZE sizeShade = _pgd.m_bHorz ? m_sizeCtBarGradientHorz : m_sizeCtBarGradientVert;
    int nOldStretchBltMode = dc.SetStretchBltMode(m_bIsWinNT ? HALFTONE : COLORONCOLOR);
    ::SetBrushOrgEx(dc, 0, 0, NULL);
    dc.StretchBlt(rcPaintGradient->left, rcPaintGradient->top, rcPaintGradient->Width(), rcPaintGradient->Height(), &dcCompat, 0, 0, sizeShade.w, sizeShade.h, SRCCOPY);

    if (nIdxClrTbFillMargin >= 0) {
      COLORREF _pen(PS_SOLID, 1, GetColor(m_nIdxClrTbFillMargin));
      COLORREF* pOldPen = dc.SelectObject(&_pen);

      if (_pgd.m_bHorz) {
        TMoveTo(dc, rcPaintGradient->left, rcPaintGradient->bottom - 2);
        TLineTo(dc, rcPaintGradient->right, rcPaintGradient->bottom - 2);
      } // if( _pgd.m_bHorz )
      else {
        TMoveTo(dc, rcPaintGradient->right - 2, rcPaintGradient->top);
        TLineTo(dc, rcPaintGradient->right - 2, rcPaintGradient->bottom);
      } // else from if( _pgd.m_bHorz )

      dc.SelectObject(pOldPen);
    } // if( nIdxClrTbFillMargin >= 0 )

    dc.SetStretchBltMode(nOldStretchBltMode);

    if (_rgnBaloonSet != NULL) {
      dc.SelectClipRgn(NULL);
    }
  } // if toolbar balloon style

  COLORREF clrDotShadow = GetColor(_2003CLR_GRIPPER_DOT_LIGHT);
  COLORREF clrDotFace = GetColor(_2003CLR_GRIPPER_DOT_DARK);
  static const SIZE g_sizeGripDot(2, 2);
  static const SIZE g_sizeGripDist(1, 1);
  static const SIZE g_sizeGripShadowOffset(1, 1);
  RECT rcGripHelper(_pgd.m_rcGripper);
  RECT rcBarClientW(rcBarClient);
  rcBarClientW OffsetRect(&, - rcBarWnd.TopLeft());

  if (_pgd.m_bHorz) {
    rcGripHelper.top = rcBarClientW.top;
    rcGripHelper.bottom = rcBarClientW.bottom;
    rcGripHelper OffsetRect(&, 1, 0);
    rcGripHelper.left += (rcGripHelper RCWIDTH(&) - g_sizeGripDot.w) / 2;
    rcGripHelper.right = rcGripHelper.left + g_sizeGripDot.w;
    rcGripHelper DeflateRect(&, 0, g_sizeGripDot.h + g_sizeGripDist.h + g_sizeGripShadowOffset.h);
    int nDotCount = rcGripHelper RCHEIGHT(&) / (g_sizeGripDot.h + g_sizeGripDist.h + g_sizeGripShadowOffset.h);
    rcGripHelper.top += rcGripHelper RCHEIGHT(&) - (g_sizeGripDot.h + g_sizeGripDist.h + g_sizeGripShadowOffset.h) * nDotCount;
    RECT rcDotFace(rcGripHelper);
    rcDotFace.bottom = rcDotFace.top + g_sizeGripDot.h;
    RECT rcDotShadow(rcDotFace);
    rcDotShadow OffsetRect(&, g_sizeGripShadowOffset);

    for (int nDot = 0; nDot < nDotCount; nDot++) {
      TFillRect(dc, &rcDotShadow, clrDotShadow);
      TFillRect(dc, &rcDotFace, clrDotFace);
      rcDotFace OffsetRect(&, 0, g_sizeGripDot.h + g_sizeGripDist.h + g_sizeGripShadowOffset.h);
      rcDotShadow OffsetRect(&, 0, g_sizeGripDot.h + g_sizeGripDist.h + g_sizeGripShadowOffset.h);
    } // for( int nDot = 0; nDot < nDotCount; nDot++ )
  } // if( _pgd.m_bHorz )
  else {
    rcGripHelper.left = rcBarClientW.left;
    rcGripHelper.right = rcBarClientW.right;
    rcGripHelper OffsetRect(&, 0, 1);
    rcGripHelper.top += (rcGripHelper RCHEIGHT(&) - g_sizeGripDot.h) / 2;
    rcGripHelper.bottom = rcGripHelper.top + g_sizeGripDot.h;
    rcGripHelper DeflateRect(&, g_sizeGripDot.w + g_sizeGripDist.w + g_sizeGripShadowOffset.w, 0);
    int nDotCount = rcGripHelper RCWIDTH(&) / (g_sizeGripDot.w + g_sizeGripDist.w + g_sizeGripShadowOffset.w);
    rcGripHelper.left += rcGripHelper RCWIDTH(&) - (g_sizeGripDot.w + g_sizeGripDist.w + g_sizeGripShadowOffset.w) * nDotCount;
    RECT rcDotFace(rcGripHelper);
    rcDotFace.right = rcDotFace.left + g_sizeGripDot.w;
    RECT rcDotShadow(rcDotFace);
    rcDotShadow OffsetRect(&, g_sizeGripShadowOffset);

    for (int nDot = 0; nDot < nDotCount; nDot++) {
      TFillRect(dc, &rcDotShadow, clrDotShadow);
      TFillRect(dc, &rcDotFace, clrDotFace);
      rcDotFace OffsetRect(&, g_sizeGripDot.w + g_sizeGripDist.w + g_sizeGripShadowOffset.w, 0);
      rcDotShadow OffsetRect(&, g_sizeGripDot.w + g_sizeGripDist.w + g_sizeGripShadowOffset.w, 0);
    } // for( int nDot = 0; nDot < nDotCount; nDot++ )
  } // else from if( _pgd.m_bHorz )
}
SIZE CExtPaintManagerXP::GetPushedOffset()
{
  return SIZE(0, 0);
}
SIZE CExtPaintManagerOffice2003::GetPushedOffset()
{
  return SIZE(0, 0);
}
int CExtPaintManagerXP::PaintPushButton(HDDC dc, PAINTPUSHBUTTONDATA& _ppbd)
{
  if (_ppbd->m_rcClient IsRectEmpty(&)) {
    return ;
  }

  if (!_ppbd->m_bEnabled) {
    _ppbd->m_bHover = FALSE;
    //_ppbd->m_bDrawBorder = FALSE;
    _ppbd->m_bDrawFocusRect = FALSE;
    _ppbd->m_bIndeterminate = FALSE;
  }

  COLORREF clrOldText = dc.GetTextColor();
  COLORREF clrOldBk = dc.GetBkColor();
  int nOldBkMode = dc.SetBkMode(TRANSPARENT);
  int nIdxMainGradientLeft = - 1;
  int nIdxMainGradientRight = - 1;
  // Prepare draw... paint button's area with background color
  COLORREF bgColor, bgColorDD;

  if (_ppbd->m_bHover || _ppbd->m_bPushed || _ppbd->m_bPushedDropDown) {
    bgColor = bgColorDD = GetColor(CLR_3DFACE_IN);

    if (_ppbd->m_bPushed || _ppbd->m_bPushedDropDown) {
      ((_ppbd->m_bPushedDropDown) ? bgColorDD : bgColor) = GetColor(_ppbd->m_bHover ? XPCLR_HILIGHT : CLR_3DFACE_IN);
    } // if( _ppbd->m_bPushed || _ppbd->m_bPushedDropDown )

    if ((!_ppbd->m_bHover) && _ppbd->m_bPushed && (!_ppbd->m_bPushedDropDown) && _ppbd->m_bSeparatedDropDown) {
      bgColorDD = GetColor(CLR_3DFACE_OUT);
    }

    if (_ppbd->m_bHover) {
      if (_ppbd->m_bPushed) {
        nIdxMainGradientLeft = m_nIdxClrBtnHovPresLeft;
        nIdxMainGradientRight = m_nIdxClrBtnHovPresRight;
      } // if( _ppbd->m_bPushed )
      else {
        nIdxMainGradientLeft = m_nIdxClrBtnHoverLeft;
        nIdxMainGradientRight = m_nIdxClrBtnHoverRight;
      } // else from if( _ppbd->m_bPushed )
    } // if( _ppbd->m_bHover )
    else if (_ppbd->m_bPushed) {
      nIdxMainGradientLeft = m_nIdxClrBtnPressedLeft;
      nIdxMainGradientRight = m_nIdxClrBtnPressedRight;
    } // else if( _ppbd->m_bPushed )
  } // if( _ppbd->m_bHover || _ppbd->m_bPushed || _ppbd->m_bPushedDropDown )
  else {
    bgColor = bgColorDD = GetColor(_ppbd->m_bFlat ? CLR_3DFACE_OUT : COLOR_3DFACE);
  }

  COLORREF clrDkBorder = GetColor((_ppbd->m_bHover || _ppbd->m_bPushed || _ppbd->m_bPushedDropDown) ? XPCLR_HILIGHT_BORDER : (_ppbd->m_bFlat ? XPCLR_HILIGHT_BORDER : COLOR_3DDKSHADOW));
  BOOL bPushedDropDown = FALSE;

  if ((_ppbd->m_bPushed && !_ppbd->m_bSeparatedDropDown) || _ppbd->m_bPushedDropDown) {
    bPushedDropDown = TRUE;
  }

  RECT rectClient(_ppbd->m_rcClient);
  RECT rectTotal(_ppbd->m_rcClient);
  RECT rectCaption(_ppbd->m_rcClient);
  RECT rectDropDown(0, 0, 0, 0);

  if (_ppbd->m_bDropDown) {
    rectDropDown = rectClient;

    if (_ppbd->m_bHorz) {
      rectCaption.right -= __DROPDOWN_ARROW_GAP * 2 + g_glyph_btn_expand_bottom.Size().w;
      rectDropDown.left = rectCaption.right - 1;
    } // if( _ppbd->m_bHorz )
    else {
      rectCaption.bottom -= __DROPDOWN_ARROW_GAP * 2 + g_glyph_btn_expand_right.Size().h;
      rectDropDown.top = rectCaption.bottom - 1;
    } // if( _ppbd->m_bHorz )

    if (!_ppbd->m_bTransparentBackground) {
      COLORREF br(bgColorDD);
      TFillRect(dc, &rectDropDown, &br);
    } // if( !_ppbd->m_bTransparentBackground )

    if (_ppbd->m_bSeparatedDropDown) {
      rectClient = rectCaption;

      if (bPushedDropDown) {
        if (_ppbd->m_bFlat) {
          if (_ppbd->m_bDrawBorder) {
            TDraw3DRect(dc, &rectDropDown, clrDkBorder, clrDkBorder);
          }
        } // if( _ppbd->m_bFlat )
        else {
          TDraw3DRect(dc, &rectDropDown, clrDkBorder, clrDkBorder);
        }
      } // if( bPushedDropDown )
      else {
        if (_ppbd->m_bFlat) {
          if (_ppbd->m_bHover && _ppbd->m_bDrawBorder) {
            TDraw3DRect(dc, &rectDropDown, clrDkBorder, clrDkBorder);
          }
        } // if( _ppbd->m_bFlat )
        else {
          TDraw3DRect(dc, &rectDropDown, clrDkBorder, clrDkBorder);
        }
      } // else from if( bPushedDropDown )
    } // if( _ppbd->m_bSeparatedDropDown )
  } // if( _ppbd->m_bDropDown )

  if (!_ppbd->m_bTransparentBackground) {
    if (nIdxMainGradientLeft >= 0 && nIdxMainGradientRight >= 0) {
      stat_PaintGradientRect(dc, rectClient, GetColor(_ppbd->m_bHorz ? nIdxMainGradientRight : nIdxMainGradientLeft), GetColor(_ppbd->m_bHorz ? nIdxMainGradientLeft : nIdxMainGradientRight), _ppbd->m_bHorz);
    } // if( nIdxMainGradientLeft >= 0 && nIdxMainGradientRight >= 0 )
    else {
      COLORREF br(bgColor);
      TFillRect(dc, &rectClient, &br);
    } // else from if( nIdxMainGradientLeft >= 0 && nIdxMainGradientRight >= 0 )
  } // if( !_ppbd->m_bTransparentBackground )

  // Draw pressed button
  if (_ppbd->m_bPushed || _ppbd->m_bPushedDropDown) {
    if (_ppbd->m_bFlat) {
      if (_ppbd->m_bDrawBorder) {
        TDraw3DRect(dc, &rectClient, clrDkBorder, clrDkBorder);
      }
    }
    else {
      TDraw3DRect(dc, &rectClient, clrDkBorder, clrDkBorder);
    }
  } // if( bPushed )
  else {
    // ...else draw non pressed button
    if (_ppbd->m_bFlat) {
      if (_ppbd->m_bHover && _ppbd->m_bDrawBorder) {
        TDraw3DRect(dc, &rectClient, clrDkBorder, clrDkBorder);
      }
    } // if( _ppbd->m_bFlat )
    else {
      TDraw3DRect(dc, &rectClient, clrDkBorder, clrDkBorder);

      if (_ppbd->m_bHover) {
        TDraw3DRect(dc, &rectTotal, clrDkBorder, clrDkBorder);
      }
    } // else from if( _ppbd->m_bFlat )
  } // else from if( _ppbd->m_bPushed || _ppbd->m_bPushedDropDown )

  if (_ppbd->m_bDropDown) {
    RECT rectGlyph(rectDropDown);

    if (bPushedDropDown) {
      rectGlyph OffsetRect(&, GetPushedOffset());
    }

    COLORREF ColorValues[2] = {
      _RGB(0, 0, 0), GetColor(
          // _ppbd->m_bEnabled
          // ? (_ppbd->m_bHover || /*_ppbd->m_bPushed ||*/ _ppbd->m_bPushedDropDown)
          // ? ((_ppbd->m_bHover && (/*_ppbd->m_bPushed ||*/ _ppbd->m_bPushedDropDown)) ? COLOR_HIGHLIGHTTEXT : CLR_TEXT_IN )
          // : ( _ppbd->m_bFlat ? CLR_TEXT_OUT : COLOR_BTNTEXT )
          // : CLR_3DSHADOW_OUT
          _ppbd->m_bEnabled ? (_ppbd->m_bHover || _ppbd->m_bPushedDropDown) ? ((_ppbd->m_clrForceTextPressed == ((COLORREF) - 1L)) ? ((_ppbd->m_bHover && _ppbd->m_bPushedDropDown) ? COLOR_HIGHLIGHTTEXT : CLR_TEXT_IN) : _ppbd->m_clrForceTextPressed) : ((_ppbd->m_clrForceTextNormal == ((COLORREF) - 1L)) ? (_ppbd->m_bFlat ? CLR_TEXT_OUT : COLOR_BTNTEXT) : _ppbd->m_clrForceTextNormal) : ((_ppbd->m_clrForceTextDisabled == ((COLORREF) - 1L)) ? CLR_3DSHADOW_OUT : _ppbd->m_clrForceTextDisabled))
      };
    PaintGlyphCentered(dc, &rectGlyph, _ppbd->m_bHorz ? &g_glyph_btn_expand_bottom : &g_glyph_btn_expand_right, ColorValues);
  } // if( _ppbd->m_bDropDown )

  rectClient = rectCaption;
  CExtSafeString sTitle(_T(""));

  if (_ppbd->m_sText != NULL) {
    sTitle = _ppbd->m_sText;
  }

  // Draw the icon
  if (rectClient.bottom > rectClient.top && rectClient.right > rectClient.left) {
    if (_ppbd->m_pIcon != NULL && (!_ppbd->m_pIcon->IsEmpty())) {
      PaintIcon(dc, _ppbd->m_bHorz, sTitle, _ppbd->m_pIcon, rectClient, rectCaption, _ppbd->m_bPushed || _ppbd->m_bPushedDropDown, _ppbd->m_bEnabled, _ppbd->m_bHover, _ppbd->m_eAlign);
    } // if( _ppbd->m_pIcon != NULL && (! _ppbd->m_pIcon->IsEmpty()) )
    else {
      PaintSysCmdGlyph(dc, _ppbd->m_nHelperSysCmdID, rectClient, FALSE, _ppbd->m_bPushed || _ppbd->m_bPushedDropDown, _ppbd->m_bEnabled);
    } // else from if( _ppbd->m_pIcon != NULL && (! _ppbd->m_pIcon->IsEmpty()) )
  }

  if (rectCaption.bottom > rectCaption.top && rectCaption.right > rectCaption.left) {
    // if we have valid area for text & focus rect
    if (!sTitle) {
      if (_ppbd->m_bPushed || _ppbd->m_bPushedDropDown) {
        SIZE sizePushedOffset = GetPushedOffset();
        rectCaption OffsetRect(&, sizePushedOffset);
      }

      dc.SetBkColor(bgColor);
      dc.SetBkMode(TRANSPARENT);

      if (_ppbd->m_bEnabled) {
        int nColorIdx = (_ppbd->m_bHover || _ppbd->m_bPushed) ? ((_ppbd->m_bHover && _ppbd->m_bPushed) ? XPCLR_PUSHEDHOVERTEXT : CLR_TEXT_IN) : (_ppbd->m_bFlat ? CLR_TEXT_OUT : COLOR_BTNTEXT);

        if (nColorIdx == XPCLR_PUSHEDHOVERTEXT && _ppbd->m_clrForceTextPressed != ((COLORREF) - 1L)) {
          textColor = (_ppbd->m_clrForceTextPressed);
        }
        else if (nColorIdx == CLR_TEXT_IN && _ppbd->m_clrForceTextHover != ((COLORREF) - 1L)) {
          textColor = (_ppbd->m_clrForceTextHover);
        }
        else if ((!(_ppbd->m_bHover || _ppbd->m_bPushed)) && _ppbd->m_clrForceTextNormal != ((COLORREF) - 1L)) {
          textColor = (_ppbd->m_clrForceTextNormal);
        }
        else {
          textColor = (GetColor(nColorIdx));
        }
      } // if( _ppbd->m_bEnabled )
      else {
        if (_ppbd->m_clrForceTextDisabled != ((COLORREF) - 1L)) {
          textColor = (_ppbd->m_clrForceTextDisabled);
        }
        else {
          textColor = (GetColor(COLOR_3DSHADOW));
        }
      } // else from if( _ppbd->m_bEnabled )

      // // center text
      // RECT centerRect = rectCaption;
      HFONT pOldBtnFont = NULL;
      HFONT pCurrFont = NULL;
      HFONT fontDummy;

      if (_ppbd->m_hFont != NULL) {
        if (_ppbd->m_bHorz) {
          pCurrFont = HFONT::FromHandle(_ppbd->m_hFont);
        }
        else {
          LOGFONT lf;
          memset(&lf, 0, sizeof(LOGFONT));
          ::GetObject(_ppbd->m_hFont, sizeof(LOGFONT), &lf);

          if (lf.lfEscapement == 0) {
            lf.lfEscapement = __EXT_VERT_FONT_ESCAPEMENT__;
            VERIFY(fontDummy = CreateFontIndirect(&lf));
            pCurrFont = &fontDummy;
          }
          else {
            // suppose font already prepared
            pCurrFont = HFONT::FromHandle(_ppbd->m_hFont);
          }
        }
      } // if( _ppbd->m_hFont != NULL )
      else {
        if (_ppbd->m_bHorz) {
          if (_ppbd->m_bDefaultPushButton) {
            pCurrFont = m_FontBold;
          }
          else {
            pCurrFont = m_FontNormal;
          }
        }
        else {
          if (_ppbd->m_bDefaultPushButton) {
            pCurrFont = m_FontBoldVert;
          }
          else {
            pCurrFont = m_FontNormalVert;
          }
        }
      } // else from if( _ppbd->m_hFont != NULL )

      ASSERT(pCurrFont != NULL);
      pOldBtnFont = dc.SelectObject(pCurrFont);
      ASSERT(pOldBtnFont != NULL);
      int nTextLength = sTitle.GetLength();
      int nAmpIndex = sTitle.Find(_T('&'));
      CExtSafeString sBtn;

      if (nAmpIndex < 0) {
        sBtn = sTitle;
      }
      else {
        sBtn = sTitle.Left(nAmpIndex) + sTitle.Right(nTextLength - (nAmpIndex + 1));
      }

      ASSERT(sBtn.Find(_T('&')) < 0);
      RECT rcText(0, 0, 0, 0);
      TDrawText(dc, sBtn, - 1, rcText, DT_SINGLELINE | DT_CALCRECT);
      RECT rcCenteredCaption(rectCaption);
      rcCenteredCaption OffsetRect(&, (rectCaption RCWIDTH(&) - rcText RCWIDTH(&)) / 2, (rectCaption RCHEIGHT(&) - rcText RCHEIGHT(&)) / 2);

      if (_ppbd->m_bHorz) {
        TDrawText(dc, sTitle, - 1, rectCaption, _ppbd->m_nDrawTextFlagsH);
      }
      else {
        LOGFONT lf;
        memset(&lf, 0, sizeof(LOGFONT));
        pCurrFont->GetLogFont(&lf);
        int _cyHorzFont = abs(lf.lfHeight);
        int _cyTextMargin = (rectCaption RCWIDTH(&) - _cyHorzFont) / 2;
        POINT ptLineFrom(0, 0), ptLineTo(0, 0);

        if (nAmpIndex >= 0) {
          ptLineFrom = POINT(__ExtMfc_CXTEXTMARGIN + _cyTextMargin, __ExtMfc_CYTEXTMARGIN + stat_CalcTextWidth(dc, _ppbd->m_bDefaultPushButton ? m_FontBold : m_FontNormal, sBtn.Left(nAmpIndex)));
          ptLineTo = POINT(__ExtMfc_CXTEXTMARGIN + _cyTextMargin, __ExtMfc_CYTEXTMARGIN + stat_CalcTextWidth(dc, _ppbd->m_bDefaultPushButton ? m_FontBold : m_FontNormal, sBtn.Left(nAmpIndex + 1)));
          int nXOffsPlus = rectCaption RCWIDTH(&) - stat_CalcTextDimension(dc, _ppbd->m_bDefaultPushButton ? m_FontBold : m_FontNormal, sBtn) RCHEIGHT(&);
          nXOffsPlus /= 2;
          nXOffsPlus -= 4;
          ptLineFrom.x += nXOffsPlus;
          ptLineTo.x += nXOffsPlus;
        } // if( nAmpIndex >= 0 )

        RECT rcString = RECT(POINT(rectCaption.right - _cyTextMargin + __ExtMfc_CXTEXTMARGIN, rectCaption.top + __ExtMfc_CYTEXTMARGIN), rectCaption.Size() //m_sizeHorz
                            );
        rcString DeflateRect(&, 2, 2);
        SIZE ptFix(- rcString RCWIDTH(&) - 1, 0);
        ptLineFrom += rcString.TopLeft() + ptFix;
        ptLineTo += rcString.TopLeft() + ptFix;

        if (_ppbd->m_bEnabled) {
          /*VTX*/
          TDrawText(dc, sBtn, sBtn.GetLength(), rcString, DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX);

          if (nAmpIndex >= 0) {
            COLORREF pen(dc.GetTextColor());
            COLORREF* pOldPen = dc.SelectObject(&pen);
            TMoveTo(dc, ptLineFrom);
            TLineTo(dc, ptLineTo);
            dc.SelectObject(pOldPen);
          } // if( nAmpIndex >= 0 )
        } // if( _ppbd->m_bEnabled )
        else {
          // POINT ptOffsetPre(1,1);
          // POINT ptOffsetPost(-1,-1);
          // COLORREF clrText = GetColor(COLOR_3DSHADOW);
          // COLORREF clrShadow = GetColor(COLOR_3DHILIGHT);
          // rcString OffsetRect(&, ptOffsetPre.x,ptOffsetPre.y);
          // textColor = ( clrShadow );
          TDrawText(dc, sBtn, sBtn.GetLength(), rcString, DT_SINGLELINE //|DT_CENTER|DT_VCENTER
              | DT_NOCLIP | DT_NOPREFIX); // don't forget DT_NOCLIP

          // rcString OffsetRect(&, ptOffsetPost.x,ptOffsetPost.y);
          // textColor = ( clrText );
          // TDrawText(dc,
          // sBtn,
          // rcString,
          // DT_SINGLELINE //|DT_CENTER|DT_VCENTER
          // |DT_NOCLIP|DT_NOPREFIX
          // ); // don't forget DT_NOCLIP
          if (nAmpIndex >= 0) {
            COLORREF pen(dc.GetTextColor() // clrShadow
                        );
            COLORREF* pOldPen = dc.SelectObject(&pen);
            // ptLineFrom += ptOffsetPre;
            // ptLineTo += ptOffsetPre;
            TMoveTo(dc, ptLineFrom);
            TLineTo(dc, ptLineTo);
            // pen.DeleteObject();
            // pen = (
            // PS_SOLID,
            // 0,
            // clrText
            // );
            // dc.SelectObject( &pen );
            // ptLineFrom += ptOffsetPost;
            // ptLineTo += ptOffsetPost;
            // TMoveTo(dc, ptLineFrom );
            // TLineTo(dc, ptLineTo );
            dc.SelectObject(pOldPen);
          } // if( nAmpIndex >= 0 )
        } // else from if( _ppbd->m_bEnabled )
      }

      dc.SelectObject(pOldBtnFont);
    } // if( !sTitle )

    if (_ppbd->m_bDrawFocusRect) {
      RECT focusRect = rectCaption; // rectClient
      focusRect DeflateRect(&, 3, 3);
      dc.DrawFocusRect(&focusRect);
    } // if( _ppbd->m_bDrawFocusRect )
  } // if we have valid area for text & focus rect

  dc.SetBkMode(nOldBkMode);
  dc.SetBkColor(clrOldBk);
  textColor = (clrOldText);
}
int CExtPaintManagerXP::PaintIcon(HDDC dc, BOOL bHorz, TCHAR* sTitle, Image* pIcon, const RECT* rectItem, RECT* rectCaption, BOOL bPushed, BOOL bEnabled, BOOL bHover, int eAlign)
{
  rectCaption = rectItem;

  if (pIcon == NULL) {
    return ;
  }

  SIZE _sizeIcon = pIcon->GetSize();
  ASSERT(_sizeIcon.w > 0 && _sizeIcon.h > 0);
  RECT iconRect = rectItem;
  RECT btnRect;

  if (bHorz) {
    if ((eAlign & __ALIGN_HORIZ_CENTER) != 0) {
      if (sTitle)
        // Center the icon horizontally
      {
        iconRect.left += ((iconRect RCWIDTH(&) - _sizeIcon.w) / 2);
      }
      else {
        iconRect.left += 3;
        rectCaption.left += _sizeIcon.w + 3;
      }
    } // if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
    else {
      if ((eAlign & __ALIGN_HORIZ_RIGHT) != 0) {
        btnRect = rectCaption;

        if (sTitle)
          // Center the icon horizontally
        {
          iconRect.left += ((iconRect RCWIDTH(&) - _sizeIcon.w) / 2);
        }
        else {
          rectCaption.right = rectCaption RCWIDTH(&) - _sizeIcon.w - 3;
          rectCaption.left = 3;
          iconRect.left = btnRect.right - _sizeIcon.w - 3;
          // Center the icon vertically
          iconRect.top += ((iconRect RCHEIGHT(&) - _sizeIcon.h) / 2);
        }
      } // if( (eAlign&__ALIGN_HORIZ_RIGHT) != 0 )
    } // else from if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )

    if ((eAlign & __ALIGN_VERT) != 0) {
      // Center the icon horizontally
      iconRect.top += ((iconRect RCHEIGHT(&) - _sizeIcon.h) / 2);
    } // if( (eAlign&__ALIGN_VERT) != 0 )
  } // if( bHorz )
  else {
    if ((eAlign & __ALIGN_HORIZ_CENTER) != 0) {
      if (sTitle)
        // Center the icon horizontally
      {
        iconRect.top += ((iconRect RCHEIGHT(&) - _sizeIcon.h) / 2);
      }
      else {
        iconRect.top += 3;
        rectCaption.top += _sizeIcon.h + 3;
      }
    } // if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
    else {
      if ((eAlign & __ALIGN_HORIZ_RIGHT) != 0) {
        btnRect = rectCaption;

        if (sTitle)
          // Center the icon horizontally
        {
          iconRect.top += ((iconRect RCHEIGHT(&) - _sizeIcon.h) / 2);
        }
        else {
          rectCaption.top = rectCaption RCHEIGHT(&) - _sizeIcon.h - 3;
          rectCaption.top = 3;
          iconRect.top = btnRect.top - _sizeIcon.h - 3;
          // Center the icon vertically
          iconRect.left += ((iconRect RCWIDTH(&) - _sizeIcon.w) / 2);
        }
      } // if( (eAlign&__ALIGN_HORIZ_RIGHT) != 0 )
    } // else from if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )

    if ((eAlign & __ALIGN_VERT) != 0) {
      // Center the icon horizontally
      iconRect.left += ((iconRect RCWIDTH(&) - _sizeIcon.w) / 2);
    } // if( (eAlign&__ALIGN_VERT) != 0 )
  } // else from if( bHorz )

  // If button is pressed then press the icon also
  if (bPushed) {
    SIZE sizePushedOffset = GetPushedOffset();
    iconRect OffsetRect(&, sizePushedOffset.w, sizePushedOffset.h);
  }

  POINT ptTopLeft = iconRect.TopLeft();

  if (m_bEnabledHoverIconShadows) {
    ptTopLeft += SIZE(1, 1);
  }

  if (bEnabled) {
    POINT ptOffs(0, 0);

    if (m_bEnabledHoverIconShadows && bHover && (!bPushed)) {
      COLORREF brBack;

      if (!brBack = (GetColor(COLOR_3DSHADOW))) {
        ASSERT(FALSE);
        return ;
      } // if( !brBack = ( GetColor( COLOR_3DSHADOW ) ) )

      // dc.DrawState(
      // ptTopLeft + ptOffs,
      // iconRect.Size(),
      // pIcon->GetIcon(),
      // __XP_DSS_MONO,
      // &brBack
      // );
      pIcon->Paint(dc, ptTopLeft + ptOffs, Image::__DRAW_MONO, (HBRUSH)brBack);
      ptOffs.x = ptOffs.y = - 2;
    } // if( m_bEnabledHoverIconShadows && bHover && (!bPushed) )

    // dc.DrawState(
    // ptTopLeft + ptOffs,
    // iconRect.Size(),
    // pIcon->GetIcon(),
    // DSS_NORMAL,
    // (COLORREF*)NULL
    // );
    pIcon->Paint(dc, ptTopLeft + ptOffs, Image::__DRAW_NORMAL);
  } // DSS_NORMAL
  else {
    COLORREF brDisabled;

    if (!brDisabled = (GetColor(XPCLR_SEPARATOR))) {
      ASSERT(FALSE);
      return ;
    }

    // dc.DrawState(
    // ptTopLeft,
    // iconRect.Size(),
    // pIcon->GetIcon(),
    // __XP_DSS_MONO,
    // &brDisabled
    // );
    COLORREF clrAlpha = GetIconAlphaColor();

    if (clrAlpha != (COLORREF(- 1L))) {
      if (clrAlpha != pIcon->AlphaColorGet()) {
        pIcon->AlphaColorSet(clrAlpha);
      }

      double lfAlphaAdjustPercent = GetIconAlphaAdjustPercent();

      if (lfAlphaAdjustPercent != pIcon->AlphaAdjustPercentGet()) {
        pIcon->AlphaAdjustPercentSet(lfAlphaAdjustPercent);
      }
    } // if( clrAlpha != (COLORREF(-1L)) )

    pIcon->Paint(dc, iconRect.TopLeft(), (clrAlpha == (COLORREF(- 1L))) ? Image::__DRAW_MONO : Image::__DRAW_ALPHA, (HBRUSH)brDisabled);
  } // DSS_NORMAL
}
int CExtPaintManagerXP::PaintComboFrame(HDDC dc, PAINTCOMBOFRAMEDATA* _pcfd)
{
  RECT rectClient2(_pcfd->m_rcClient);
  int nSavedDC = dc.SaveDC();
  COLORREF clrWindow = (_pcfd->m_clrForceNABorder == (COLORREF)(- 1L) || _pcfd->m_bHover || _pcfd->m_bPushed) ? GetColor(_pcfd->m_bEnabled ? COLOR_WINDOW : COLOR_3DFACE) : _pcfd->m_clrForceNABorder;
  COLORREF clrFaceOut = (_pcfd->m_clrForceNAContent == (COLORREF)(- 1L) || _pcfd->m_bHover || _pcfd->m_bPushed) ? GetColor(CLR_3DFACE_OUT) : _pcfd->m_clrForceNAContent;
  COLORREF clrHilight = (_pcfd->m_clrForceNABorder == (COLORREF)(- 1L) || _pcfd->m_bHover || _pcfd->m_bPushed) ? GetColor(XPCLR_3DFACE_NORMAL) : _pcfd->m_clrForceNABorder;
  COLORREF clrShadow = (_pcfd->m_clrForceNABorder == (COLORREF)(- 1L) || _pcfd->m_bHover || _pcfd->m_bPushed) ? GetColor(CLR_3DFACE_OUT) : _pcfd->m_clrForceNABorder;
  RECT rectClient(_pcfd->m_rcClient);
  COLORREF clrOuterBorder = (_pcfd->m_clrForceActiveOuterBorder != (COLORREF)(- 1L) && (_pcfd->m_bHover || _pcfd->m_bPushed)) ? _pcfd->m_clrForceActiveOuterBorder : clrFaceOut;

  if (!_pcfd->m_bNoTopOuterBorder) {
    TDraw3DRect(dc, &rectClient, clrOuterBorder, clrOuterBorder);
  }

  rectClient DeflateRect(&, 1, 1);
  TDraw3DRect(dc, &rectClient, clrHilight, clrHilight);
  rectClient.left = rectClient.right - ::GetSystemMetrics(SM_CXHTHUMB);
  TDraw3DRect(dc, &rectClient, clrHilight, clrHilight);
  rectClient DeflateRect(&, 1, 1);
  TDraw3DRect(dc, &rectClient, clrHilight, clrHilight);
  TFillRect(dc, rectClient.left - 2, rectClient.top, 3, rectClient RCHEIGHT(&), clrWindow);

  if (_pcfd->m_bHover || _pcfd->m_bPushed) {
    TFillRect(dc, rectClient, clrHilight);
  }

  rectClient InflateRect(&, 1, 1);
  TFillRect(dc, &rectClient, (_pcfd->m_clrForceNAContent == (COLORREF)(- 1L) || _pcfd->m_bHover || _pcfd->m_bPushed) ? clrFaceOut : _pcfd->m_clrForceNAContent);
  HFONT font;
  int ppi = dc.GetDeviceCaps(LOGPIXELSX);
  int pointsize = MulDiv(60, 96, ppi); // 6 points at 96 ppi
  font.CreatePointFont(pointsize, _T("Marlett"));
  PAINTPUSHBUTTONDATA _ppbd(this, TRUE, rectClient, _T("6"), NULL, TRUE, (_pcfd->m_bHover || _pcfd->m_bPushed) ? TRUE : FALSE, _pcfd->m_bPushed ? TRUE : FALSE, FALSE, _pcfd->m_bEnabled, TRUE, FALSE, FALSE, __ALIGN_HORIZ_CENTER | __ALIGN_VERT, (HFONT)font, // = NULL
      FALSE, 0, (_pcfd->m_clrForceNAContent == (COLORREF)(- 1L) || _pcfd->m_bHover || _pcfd->m_bPushed) ? FALSE : TRUE);
  PaintPushButton(dc, _ppbd);

  if (!(_pcfd->m_bHover || _pcfd->m_bPushed)) {
    TDraw3DRect(dc, &rectClient, clrHilight, clrHilight);
  }

  if (_pcfd->m_bHover || _pcfd->m_bPushed) {
    rectClient2 DeflateRect(&, 1, 1);
    HBRUSH hOldBrush = (HBRUSH)::SelectObject(dc, GetStockObject(NULL_BRUSH));
    COLORREF clrOld = textColor = (clrShadow);
    dc.Rectangle(&rectClient2);
    textColor = (clrOld);

    if (hOldBrush) {
      ::SelectObject(dc, hOldBrush);
    }
  }

  dc.RestoreDC(nSavedDC);
}
int CExtPaintManagerOffice2003::PaintComboFrame(HDDC dc, PAINTCOMBOFRAMEDATA* _pcfd)
{
  if (_pcfd->m_pHelperSrc == NULL || (!_pcfd->m_bEnabled) || _pcfd->m_clrForceNABorder != (COLORREF)(- 1L) || _pcfd->m_clrForceNAContent != (COLORREF)(- 1L) || _pcfd->m_clrForceActiveOuterBorder != (COLORREF)(- 1L) || (!(_pcfd->m_pHelperSrc->IsKindOf(RUNTIME_CLASS(CComboBox)) || _pcfd->m_pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtBarButton)))) || (!GetCb2DbTransparentMode(_pcfd->m_pHelperSrc)) || m_bmpCtBarGradientHorz == NULL || m_bmpCtBarGradientVert == NULL || m_bmpDockerGradient == NULL || stat_GetBPP() <= 8) {
    CExtPaintManagerXP::PaintComboFrame(dc, _pcfd);
    return ;
  }

  int nDxHumb = ::GetSystemMetrics(SM_CXHTHUMB);
  RECT rcClient(_pcfd->m_rcClient);
  RECT rcEraser(rcClient);
  rcEraser DeflateRect(&, 1, 1);
  RECT rcHoverFrame(rcEraser);
  RECT rcButton(rcEraser);
  rcButton.left = rcButton.right - nDxHumb;
  RECT rcClientItem(rcEraser);
  rcClientItem DeflateRect(&, 1, 1, nDxHumb - 1 + 3, 1);
  CRgn rgnClip, rgnClientItem;

  if (rgnClip.CreateRectRgnIndirect(&rcClient) && rgnClientItem.CreateRectRgnIndirect(&rcClientItem) && rgnClip.CombineRgn(&rgnClip, &rgnClientItem, RGN_DIFF) != ERROR) {
    dc.SelectClipRgn(&rgnClip);
  }

  CComboBox* pWndCombo = DYNAMIC_DOWNCAST(CComboBox, _pcfd->m_pHelperSrc);

  if (pWndCombo != NULL) {
    ASSERT_VALID(pWndCombo);

    if (!PaintDockerBkgnd(dc, pWndCombo)) {
      if (rgnClip != NULL) {
        dc.SelectClipRgn(NULL);
      }

      CExtPaintManagerXP::PaintComboFrame(dc, _pcfd);
      return ;
    }
  } // if( pWndCombo != NULL )

  int nSavedDC = dc.SaveDC();
  COLORREF clrWindow = GetColor(COLOR_WINDOW);
  TDraw3DRect(dc, &rcEraser, clrWindow, clrWindow);
  rcEraser DeflateRect(&, 0, 1, nDxHumb - 1, 1);
  TDraw3DRect(dc, &rcEraser, clrWindow, clrWindow);
  rcEraser DeflateRect(&, 0, 0, 1, 0);
  TDraw3DRect(dc, &rcEraser, clrWindow, clrWindow);
  rcEraser DeflateRect(&, 0, 0, 1, 0);
  TDraw3DRect(dc, &rcEraser, clrWindow, clrWindow);
  rcEraser DeflateRect(&, 0, 0, 1, 0);
  TDraw3DRect(dc, &rcEraser, clrWindow, clrWindow);

  if (_pcfd->m_bHover || _pcfd->m_bPushed) {
    COLORREF clrBorder = GetColor(XPCLR_HILIGHT_BORDER);
    TDraw3DRect(dc, &rcHoverFrame, clrBorder, clrBorder);
  }

  HFONT font;
  int ppi = dc.GetDeviceCaps(LOGPIXELSX);
  int pointsize = MulDiv(60, 96, ppi); // 6 points at 96 ppi
  font.CreatePointFont(pointsize, _T("Marlett"));
  PAINTPUSHBUTTONDATA _ppbd(this, TRUE, rcButton, _T("6"), NULL, TRUE, (_pcfd->m_bHover || _pcfd->m_bPushed) ? TRUE : FALSE, _pcfd->m_bPushed ? TRUE : FALSE, FALSE, _pcfd->m_bEnabled, TRUE, FALSE, FALSE, __ALIGN_HORIZ_CENTER | __ALIGN_VERT, (HFONT)font, // = NULL
      FALSE, 0, (_pcfd->m_bHover || _pcfd->m_bPushed) ? FALSE : TRUE);
  PaintPushButton(dc, _ppbd);

  if (rgnClip != NULL) {
    dc.SelectClipRgn(NULL);
  }

  dc.RestoreDC(nSavedDC);
}
int GetMenuBorderSize()
{
  return 4;
}
int PaintMenuBorder(HDDC dc, const RECT* rectClient, CObject* pHelperSrc, LPARAM lParam // = 0L
                   )
{
  pHelperSrc;
  lParam;

  if (::IsRectEmpty(&rectClient)) {
    return ;
  }

  RECT rc(rectClient);
  TDraw3DRect(dc, &rc, GetColor(CLR_3DFACE_OUT), GetColor(CLR_3DDKSHADOW_OUT));
  rc DeflateRect(&, 1, 1);
  TDraw3DRect(dc, &rc, GetColor(CLR_3DHILIGHT_OUT), GetColor(CLR_3DSHADOW_OUT));
  COLORREF clrFace = GetColor(CLR_3DFACE_OUT);
  rc DeflateRect(&, 1, 1);
  TDraw3DRect(dc, &rc, clrFace, clrFace);
  rc DeflateRect(&, 1, 1);
  TDraw3DRect(dc, &rc, clrFace, clrFace);
}
int GetMenuShadowSize()
{
  return 4; // 5; // (+ 2.23)
}
int CExtPaintManagerXP::GetMenuShadowSize()
{
  return 4; // 5; // (+ 2.23)
}
int CExtPaintManagerXP::GetMenuBorderSize()
{
  return 2;
}
int CExtPaintManagerXP::PaintMenuBorder(HDDC dc, const RECT* rectClient, CObject* pHelperSrc, LPARAM lParam // = 0L
                                       )
{
  pHelperSrc;
  lParam;

  if (::IsRectEmpty(&rectClient)) {
    return ;
  }

  RECT rc(rectClient);
  COLORREF clr = GetColor(XPCLR_MENU_BORDER);
  TDraw3DRect(dc, &rc, clr, clr);
  COLORREF clrFace = GetColor(XPCLR_3DFACE_NORMAL);
  rc DeflateRect(&, 1, 1);
  TDraw3DRect(dc, &rc, clrFace, clrFace);
  rc.right--;
  rc.bottom--;
  TDraw3DRect(dc, &rc, clrFace, clrFace);
}
BOOL IsMenuMustCombineExcludeArea()
{
  //
  // Implementadion of PaintMenuCombinedArea()
  // is provided, but office 98/2k does not combine its menu area
  // with exclude area. interesting? uncomment and play with it!
  //
  // return TRUE;
  //
  return FALSE;
}
BOOL CExtPaintManagerXP::IsMenuMustCombineExcludeArea()
{
  return TRUE;
}
int PaintMenuCombinedArea(HDDC dc, const RECT* rcArea, const RECT* rcClient, int eCombineAlign, // CExtPopupMenuWnd::e_combine_align_t values
    CObject* pHelperSrc, LPARAM lParam // = 0L
                         )
{
  pHelperSrc;
  lParam;

  if (::IsRectEmpty(&rcArea)) {
    return ;
  }

  dc.SelectClipRgn(NULL);
  ASSERT(eCombineAlign != CExtPopupMenuWnd::__CMBA_NONE);
  int nExpandSize = GetMenuBorderSize() - 1;
  RECT _rcClient(rcClient);
  RECT _rcArea(rcArea);
  _rcArea InflateRect(&, (eCombineAlign == CExtPopupMenuWnd::__CMBA_RIGHT) ? nExpandSize : 0, (eCombineAlign == CExtPopupMenuWnd::__CMBA_BOTTOM) ? nExpandSize : 0, (eCombineAlign == CExtPopupMenuWnd::__CMBA_LEFT) ? nExpandSize : 0, (eCombineAlign == CExtPopupMenuWnd::__CMBA_TOP) ? nExpandSize : 0);
  TFillRect(dc, &_rcArea, GetColor(CLR_3DFACE_OUT));
  // _rcArea = rcArea;
  COLORREF clrMenuBorderLTo = GetColor(CLR_3DFACE_OUT);
  COLORREF clrMenuBorderLTi = GetColor(CLR_3DHILIGHT_OUT);
  COLORREF clrMenuBorderRBo = GetColor(CLR_3DSHADOW_OUT);
  COLORREF clrMenuBorderRBi = GetColor(CLR_3DDKSHADOW_OUT);

  switch (eCombineAlign) {
  case CExtPopupMenuWnd::__CMBA_TOP:
    _rcArea InflateRect(&, 0, 0, 0, nExpandSize);
    _rcClient InflateRect(&, - nExpandSize, 0);
    break;

  case CExtPopupMenuWnd::__CMBA_BOTTOM:
    _rcArea InflateRect(&, 0, nExpandSize, 0, 0);
    _rcClient InflateRect(&, - nExpandSize, 0);
    break;

  case CExtPopupMenuWnd::__CMBA_LEFT:
    _rcArea InflateRect(&, 0, 0, nExpandSize, 0);
    _rcClient InflateRect(&, 0, - nExpandSize);
    break;

  case CExtPopupMenuWnd::__CMBA_RIGHT:
    _rcArea InflateRect(&, nExpandSize, 0, 0, 0);
    _rcClient InflateRect(&, 0, - nExpandSize);
    break;
#ifdef _DEBUG

  default:
    ASSERT(FALSE);
    break;
#endif // _DEBUG
  } // switch( eCombineAlign )

  dc.ExcludeClipRect(&_rcClient);
  int cx = _rcArea RCWIDTH(&);
  int cy = _rcArea RCHEIGHT(&);

  if (eCombineAlign != CExtPopupMenuWnd::__CMBA_BOTTOM) {
    TFillRect(dc, _rcArea.left, _rcArea.top, cx - 1, 1, clrMenuBorderLTo);
  }

  if (eCombineAlign != CExtPopupMenuWnd::__CMBA_RIGHT) {
    TFillRect(dc, _rcArea.left, _rcArea.top, 1, cy - 1, clrMenuBorderLTo);
  }

  if (eCombineAlign != CExtPopupMenuWnd::__CMBA_LEFT) {
    TFillRect(dc, _rcArea.left + cx, _rcArea.top, - 1, cy, clrMenuBorderRBo);
  }

  if (eCombineAlign != CExtPopupMenuWnd::__CMBA_TOP) {
    TFillRect(dc, _rcArea.left, _rcArea.top + cy, cx, - 1, clrMenuBorderRBo);
  }

  _rcArea DeflateRect(&, 1, 1, 1, 1);
  cx = _rcArea RCWIDTH(&);
  cy = _rcArea RCHEIGHT(&);

  if (eCombineAlign != CExtPopupMenuWnd::__CMBA_BOTTOM) {
    TFillRect(dc, _rcArea.left, _rcArea.top, cx - 1, 1, clrMenuBorderLTi);
  }

  if (eCombineAlign != CExtPopupMenuWnd::__CMBA_RIGHT) {
    TFillRect(dc, _rcArea.left, _rcArea.top, 1, cy - 1, clrMenuBorderLTi);
  }

  if (eCombineAlign != CExtPopupMenuWnd::__CMBA_LEFT) {
    TFillRect(dc, _rcArea.left + cx, _rcArea.top, - 1, cy, clrMenuBorderRBi);
  }

  if (eCombineAlign != CExtPopupMenuWnd::__CMBA_TOP) {
    TFillRect(dc, _rcArea.left, _rcArea.top + cy, cx, - 1, clrMenuBorderRBi);
  }

  /*
  dc.ExcludeClipRect( &_rcClient );
  TDraw3DRect(dc,&
  &_rcArea,
  clrMenuBorderLTo,
  clrMenuBorderRBo
  );
  _rcArea DeflateRect(&,1,1,1,1);
  TDraw3DRect(dc,&
  &_rcArea,
  clrMenuBorderLTi,
  clrMenuBorderRBi
  );
  */
  dc.SelectClipRgn(NULL);
}
int CExtPaintManagerXP::PaintMenuCombinedArea(HDDC dc, const RECT* rcArea, const RECT* rcClient, int eCombineAlign, // CExtPopupMenuWnd::e_combine_align_t values
    CObject* pHelperSrc, LPARAM lParam // = 0L
                                             )
{
  pHelperSrc;
  lParam;

  if (::IsRectEmpty(&rcArea)) {
    return ;
  }

  ASSERT(eCombineAlign != CExtPopupMenuWnd::__CMBA_NONE);
  RECT _rcArea(rcArea);
  RECT _rcClient(rcClient);
  TFillRect(dc, &_rcArea, GetColor(XPCLR_3DFACE_DARK));

  switch (eCombineAlign) {
  case CExtPopupMenuWnd::__CMBA_TOP:
  case CExtPopupMenuWnd::__CMBA_BOTTOM:
    _rcClient InflateRect(&, - 1, 0);
    break;

  case CExtPopupMenuWnd::__CMBA_LEFT:
  case CExtPopupMenuWnd::__CMBA_RIGHT:
    _rcClient InflateRect(&, 0, - 1);
    break;
#ifdef _DEBUG

  default:
    ASSERT(FALSE);
    break;
#endif // _DEBUG
  } // switch( eCombineAlign )

  dc.ExcludeClipRect(&_rcClient);
  COLORREF clrMenuBorder = GetColor(XPCLR_MENU_BORDER);
  TDraw3DRect(dc, &_rcArea, clrMenuBorder, clrMenuBorder);
  dc.SelectClipRgn(NULL);
}
int CExtPaintManagerOffice2003::PaintMenuCombinedArea(HDDC dc, const RECT* rcArea, const RECT* rcClient, int eCombineAlign, // CExtPopupMenuWnd::e_combine_align_t values
    CObject* pHelperSrc, LPARAM lParam // = 0L
                                                     )
{
  if (::IsRectEmpty(&rcArea)) {
    return ;
  }

  ASSERT(eCombineAlign != CExtPopupMenuWnd::__CMBA_NONE);
  BOOL bStyleOffice2003 = TRUE;
  CExtPopupMenuWnd* pPopup = NULL;

  if (pHelperSrc == NULL || (!pHelperSrc->IsKindOf(RUNTIME_CLASS(CExtPopupMenuWnd))) || m_bmpCtBarGradientHorz == NULL || m_bmpCtBarGradientVert == NULL || m_bmpDockerGradient == NULL || stat_GetBPP() <= 8) {
    bStyleOffice2003 = FALSE;
  }
  else {
    pPopup = STATIC_DOWNCAST(CExtPopupMenuWnd, pHelperSrc);
    ASSERT_VALID(pPopup);
  }

  if (!bStyleOffice2003) {
    CExtPaintManagerXP::PaintMenuCombinedArea(dc, rcArea, rcClient, eCombineAlign, pHelperSrc, lParam);
    return ;
  }

  ASSERT_VALID(pPopup);
  CObject* pObjCoockie = pPopup->GetCoockieAsObject();
  CExtToolControlBar* pToolBar = NULL;
  int nTbbMenuTrackingBtnIdx = - 1;
  CExtBarButton* pTBB = NULL;

  if (pObjCoockie != NULL && pObjCoockie->IsKindOf(RUNTIME_CLASS(CExtToolControlBar))) {
    // if menu coockie for combined content callback is a CExtToolControlBar
    pToolBar = STATIC_DOWNCAST(CExtToolControlBar, pObjCoockie);
    ASSERT_VALID(pToolBar);

    if (pToolBar->m_bPresubclassDialogMode || pToolBar->m_pDockSite == NULL) {
      CExtPaintManagerXP::PaintMenuCombinedArea(dc, rcArea, rcClient, eCombineAlign, pHelperSrc, lParam);
      return ;
    }

    nTbbMenuTrackingBtnIdx = pToolBar->GetMenuTrackingButton();

    if (nTbbMenuTrackingBtnIdx >= 0) {
      pTBB = pToolBar->GetButton(nTbbMenuTrackingBtnIdx);
      ASSERT_VALID(pTBB);
    } // if( nTbbMenuTrackingBtnIdx >= 0 )
  } // if menu coockie for combined content callback is a CExtToolControlBar

  if (pToolBar != NULL && pTBB != NULL && pTBB->IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton)) && (!pToolBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)))) {
    // if toolbar content expand button
    if ((!(m_bUxApiInitPassed && m_bUxValidColorsExtracted && m_bUxUseIfAvailOnWinXpOrLater))) {
      COLORREF clrFillUpdate = GetColor(_2003CLR_EXPBTN_PRESSED_LIGHT);

      switch (eCombineAlign) {
      case CExtPopupMenuWnd::__CMBA_TOP:
        TFillRect(dc, rcArea.right - m_sizeToolBarRgnRounders.w - 2, rcArea.bottom - m_sizeToolBarRgnRounders.h - 1, m_sizeToolBarRgnRounders.w + 2, m_sizeToolBarRgnRounders.h, clrFillUpdate);
        break;

      case CExtPopupMenuWnd::__CMBA_BOTTOM:
        TFillRect(dc, rcArea.right - m_sizeToolBarRgnRounders.w - 2, rcArea.top + 1, m_sizeToolBarRgnRounders.w + 2, m_sizeToolBarRgnRounders.h, clrFillUpdate);
        TFillRect(dc, rcArea.right - m_sizeToolBarRgnRounders.w, rcArea.bottom - m_sizeToolBarRgnRounders.h - 1, m_sizeToolBarRgnRounders.w, m_sizeToolBarRgnRounders.h, clrFillUpdate);
        break;

      case CExtPopupMenuWnd::__CMBA_LEFT:
        TFillRect(dc, rcArea.right - m_sizeToolBarRgnRounders.w - 2, rcArea.bottom - m_sizeToolBarRgnRounders.h, m_sizeToolBarRgnRounders.w + 1, m_sizeToolBarRgnRounders.h, clrFillUpdate);
        break;

      case CExtPopupMenuWnd::__CMBA_RIGHT:
        TFillRect(dc, rcArea.right - m_sizeToolBarRgnRounders.w, rcArea.bottom - m_sizeToolBarRgnRounders.h - 1, m_sizeToolBarRgnRounders.w, m_sizeToolBarRgnRounders.h, clrFillUpdate);
        break;
#ifdef _DEBUG

      default:
        ASSERT(FALSE);
        break;
#endif // _DEBUG
      } // switch( eCombineAlign )
    }

    return ;
  } // if toolbar content expand button

  RECT _rcArea(rcArea);
  RECT _rcClient(rcClient);

  switch (eCombineAlign) {
  case CExtPopupMenuWnd::__CMBA_TOP:
  case CExtPopupMenuWnd::__CMBA_BOTTOM:
    _rcClient InflateRect(&, - 1, 0);
    break;

  case CExtPopupMenuWnd::__CMBA_LEFT:
  case CExtPopupMenuWnd::__CMBA_RIGHT:
    _rcClient InflateRect(&, 0, - 1);
    break;
#ifdef _DEBUG

  default:
    ASSERT(FALSE);
    break;
#endif // _DEBUG
  } // switch( eCombineAlign )

  BOOL b2003StyleMiddleBorder = TRUE;
  BOOL bNoGradientOverCombineArea = FALSE;

  if (pToolBar != NULL && pTBB != NULL && (pToolBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) || (pTBB->GetIconPtr() != NULL && (!pTBB->GetIconPtr()->IsEmpty())))) {
    b2003StyleMiddleBorder = FALSE;
  }
  else {
    if (pPopup->IsToolBtnCombinedArea()) {
      bNoGradientOverCombineArea = TRUE;
    }
    else {
      b2003StyleMiddleBorder = FALSE;
    }
  }

  BOOL bPreEmptyClipRgn = FALSE;
  COLORREF clrBottomFill = GetColor(bNoGradientOverCombineArea ? XPCLR_3DFACE_DARK : XPCLR_3DFACE_NORMAL);

  if (!b2003StyleMiddleBorder) {
    TFillRect(dc, &_rcArea, clrBottomFill);
    dc.ExcludeClipRect(&_rcClient);
  } // if( !b2003StyleMiddleBorder )
  else {
    if (eCombineAlign == CExtPopupMenuWnd::__CMBA_LEFT) {
      _rcArea InflateRect(&, 0, 0, - 1, 0);
    }

    dc.ExcludeClipRect(&_rcClient);
    TFillRect(dc, &_rcArea, clrBottomFill);
    bPreEmptyClipRgn = TRUE;
  } // else from if( !b2003StyleMiddleBorder )

  if (!bNoGradientOverCombineArea) {
    HDDC dcCompat = CachedDcGet(m_bmpCtBarGradientHorz);

    if (dcCompat != NULL) {
      SIZE sizeShade = m_sizeCtBarGradientHorz;
      int nOldStretchBltMode = dc.SetStretchBltMode(m_bIsWinNT ? HALFTONE : COLORONCOLOR);
      ::SetBrushOrgEx(dc, 0, 0, NULL);
      dc.StretchBlt( // (/2) makes area lighter and more smoth fill
          _rcArea.left, _rcArea.top, _rcArea RCWIDTH(&), _rcArea RCHEIGHT(&), &dcCompat, 0, 0, sizeShade.w / 2, sizeShade.h / 2, SRCCOPY);
      dc.SetStretchBltMode(nOldStretchBltMode);
    } // if( dcCompat != NULL )

#ifdef _DEBUG
    else {
      ASSERT(FALSE);
    } // else from if( dcCompat != NULL )

#endif // _DEBUG
  } // if( !bNoGradientOverCombineArea )

  if (bPreEmptyClipRgn) {
    dc.SelectClipRgn(NULL);
  }

  COLORREF clrMenuBorder = GetColor(XPCLR_MENU_BORDER);
  TDraw3DRect(dc, &_rcArea, clrMenuBorder, clrMenuBorder);
  dc.SelectClipRgn(NULL);
}
PAINTCONTROLBARBORDERSDATA::PAINTCONTROLBARBORDERSDATA(): m_pHelperSrc(NULL), m_lParam(0L), m_eCBBT(e_control_bar_border_type_t::__CB_OUTER_DOCKBAR), m_dwStyle(0L), m_rc(0, 0, 0, 0) {}
PAINTCONTROLBARBORDERSDATA::PAINTCONTROLBARBORDERSDATA(CObject* pHelperSrc, int eCBBT, // e_control_bar_border_type_t
    DWORD dwStyle, const RECT* rc): m_pHelperSrc(pHelperSrc), m_lParam(0L), m_eCBBT(eCBBT), m_dwStyle(dwStyle), m_rc(rc) {}
int PaintControlBarBordersDefImpl(HDDC dc, PAINTCONTROLBARBORDERSDATA& _pcbbd, COLORREF clrHi, COLORREF clrLo)
{
  if (!(_pcbbd.m_dwStyle & CBRS_BORDER_ANY)) {
    return ;
  }

  // prepare for dark lines
  // ASSERT( _pcbbd.m_rc.top == 0 && _pcbbd.m_rc.left == 0 );
  RECT rc1(_pcbbd.m_rc), rc2(_pcbbd.m_rc);
  COLORREF clr = clrLo;
  // afxData.bWin4
  // ? afxData.clrBtnShadow
  // : afxData.clrWindowFrame;
  GetColor(CLR_3DFACE_OUT);

  // draw dark line one pixel back/up
  if (_pcbbd.m_dwStyle & CBRS_BORDER_3D) {
    rc1.right -= CX_BORDER;
    rc1.bottom -= CY_BORDER;
  }

  if (_pcbbd.m_dwStyle & CBRS_BORDER_TOP) {
    rc2.top += afxData.hBorder2;
  }

  if (_pcbbd.m_dwStyle & CBRS_BORDER_BOTTOM) {
    rc2.bottom -= afxData.hBorder2;
  }

  // draw left and top
  if (_pcbbd.m_dwStyle & CBRS_BORDER_LEFT) {
    TFillRect(dc, 0, rc2.top, CX_BORDER, rc2 RCHEIGHT(&), clr);
  }

  if (_pcbbd.m_dwStyle & CBRS_BORDER_TOP) {
    TFillRect(dc, 0, 0, _pcbbd.m_rc.right, CY_BORDER, clr);
  }

  // draw right and bottom
  if (_pcbbd.m_dwStyle & CBRS_BORDER_RIGHT) {
    TFillRect(dc, rc1.right, rc2.top, - CX_BORDER, rc2 RCHEIGHT(&), clr);
  }

  if (_pcbbd.m_dwStyle & CBRS_BORDER_BOTTOM) {
    TFillRect(dc, 0, rc1.bottom, _pcbbd.m_rc.right, - CY_BORDER, clr);
  }

  if (_pcbbd.m_dwStyle & CBRS_BORDER_3D) {
    // prepare for hilite lines
    // clr = afxData.clrBtnHilite;
    clr = clrHi;

    // draw left and top
    if (_pcbbd.m_dwStyle & CBRS_BORDER_LEFT) {
      TFillRect(dc, 1, rc2.top, CX_BORDER, rc2 RCHEIGHT(&), clr);
    }

    if (_pcbbd.m_dwStyle & CBRS_BORDER_TOP) {
      TFillRect(dc, 0, 1, _pcbbd.m_rc.right, CY_BORDER, clr);
    }

    // draw right and bottom
    if (_pcbbd.m_dwStyle & CBRS_BORDER_RIGHT) {
      TFillRect(dc, _pcbbd.m_rc.right, rc2.top, - CX_BORDER, rc2 RCHEIGHT(&), clr);
    }

    if (_pcbbd.m_dwStyle & CBRS_BORDER_BOTTOM) {
      TFillRect(dc, 0, _pcbbd.m_rc.bottom, _pcbbd.m_rc.right, - CY_BORDER, clr);
    }
  }

  // if( _pcbbd.m_dwStyle & CBRS_BORDER_LEFT )
  // _pcbbd.m_rc.left += afxData.wBorder2;
  // if( _pcbbd.m_dwStyle & CBRS_BORDER_TOP )
  // _pcbbd.m_rc.top += afxData.hBorder2;
  // if( _pcbbd.m_dwStyle & CBRS_BORDER_RIGHT )
  // _pcbbd.m_rc.right -= afxData.wBorder2;
  // if( _pcbbd.m_dwStyle & CBRS_BORDER_BOTTOM )
  // _pcbbd.m_rc.bottom -= afxData.hBorder2;
}
int PaintControlBarBorders(HDDC dc, PAINTCONTROLBARBORDERSDATA& _pcbbd)
{
  if (_pcbbd.m_eCBBT == __CB_INNER_STATUSBAR_ITEM) {
    TDraw3DRect(dc, &_pcbbd.m_rc, GetColor(CLR_3DSHADOW_OUT), GetColor(CLR_3DHILIGHT_OUT));
    return ;
  } // if( _pcbbd.m_eCBBT == __CB_INNER_STATUSBAR_ITEM )

  COLORREF clr = GetColor(CLR_3DFACE_OUT);
  PaintControlBarBordersDefImpl(dc, _pcbbd, clr, clr);
}
int CExtPaintManagerXP::PaintControlBarBorders(HDDC dc, PAINTCONTROLBARBORDERSDATA& _pcbbd)
{
  if (_pcbbd.m_eCBBT == __CB_INNER_STATUSBAR_ITEM) {
    COLORREF clr = GetColor(COLOR_3DSHADOW);
    TDraw3DRect(dc, &_pcbbd.m_rc, clr, clr);
    return ;
  } // if( _pcbbd.m_eCBBT == __CB_INNER_STATUSBAR_ITEM )

  COLORREF clr = GetColor(CLR_3DFACE_OUT);
  PaintControlBarBordersDefImpl(dc, _pcbbd, clr, clr);
}
int CExtPaintManagerOffice2003::PaintControlBarBorders(HDDC dc, PAINTCONTROLBARBORDERSDATA& _pcbbd)
{
  if (_pcbbd.m_eCBBT == __CB_INNER_STATUSBAR_ITEM) {
    COLORREF clr = GetColor(_2003CLR_STATUSBAR_ITEM);
    TDraw3DRect(dc, &_pcbbd.m_rc, clr, clr);
    return ;
  } // if( _pcbbd.m_eCBBT == __CB_INNER_STATUSBAR_ITEM )

  CExtPaintManagerXP::PaintControlBarBorders(dc, _pcbbd);
}
int PaintResizableBarChildNcAreaRect(HDDC dc, const RECT* rc, CObject* pHelperSrc, LPARAM lParam // = 0L
                                    )
{
  PaintTabNcAreaRect(dc, rc, pHelperSrc, lParam);
}
int CExtPaintManagerXP::PaintResizableBarChildNcAreaRect(HDDC dc, const RECT* rc, CObject* pHelperSrc, LPARAM lParam // = 0L
                                                        )
{
  PaintResizableBarChildNcAreaRect(dc, rc, pHelperSrc, lParam);
}
HICON stat_GenerateColorIconBox(COLORREF clrIcon)
{
  HDDC dc(NULL);
  ICONINFO iiNewIcon;
  memset(&iiNewIcon, 0, sizeof(ICONINFO));
  iiNewIcon.fIcon = TRUE;
  HDC hIconDC = ::CreateCompatibleDC(dc);
  HDC hMaskDC = ::CreateCompatibleDC(dc);
  ASSERT(hIconDC != NULL && hMaskDC != NULL);
  iiNewIcon.hbmColor = ::CreateCompatibleBitmap(dc, 16, 16);
  ASSERT(iiNewIcon.hbmColor != NULL);
  iiNewIcon.hbmMask = ::CreateCompatibleBitmap(hMaskDC, 16, 16);
  ASSERT(iiNewIcon.hbmMask != NULL);
  HGDIOBJ hOldIconDC = ::SelectObject(hIconDC, iiNewIcon.hbmColor);
  ASSERT(hOldIconDC != NULL);
  HGDIOBJ hOldMaskDC = ::SelectObject(hMaskDC, iiNewIcon.hbmMask);
  ASSERT(hOldMaskDC != NULL);
  COLORREF brColorItem(clrIcon);
  RECT rcColorItem(0, 0, 16, 16);
  ::FillRect(hIconDC, &rcColorItem, (HBRUSH)brColorItem);
  ::FillRect(hMaskDC, &rcColorItem, (HBRUSH)GetStockObject(BLACK_BRUSH));
  ::SelectObject(hIconDC, hOldIconDC);
  ::SelectObject(hMaskDC, hOldMaskDC);
  HICON hIcon = ::CreateIconIndirect(&iiNewIcon);
  ASSERT(hIcon != NULL);
  ::DeleteObject(iiNewIcon.hbmColor);
  ::DeleteObject(iiNewIcon.hbmMask);
  ::DeleteDC(hMaskDC);
  ::DeleteDC(hIconDC);
  return hIcon;
}
HICON stat_GenerateColorIconFrame(COLORREF clrIcon)
{
  HDDC dc(NULL);
  ICONINFO iiNewIcon;
  iiNewIcon.fIcon = TRUE;
  HDC hIconDC = ::CreateCompatibleDC(dc);
  HDC hMaskDC = ::CreateCompatibleDC(dc);
  ASSERT(hIconDC != NULL && hMaskDC != NULL);
  iiNewIcon.hbmColor = ::CreateCompatibleBitmap(dc, 16, 16);
  ASSERT(iiNewIcon.hbmColor != NULL);
  iiNewIcon.hbmMask = ::CreateCompatibleBitmap(hMaskDC, 16, 16);
  ASSERT(iiNewIcon.hbmMask != NULL);
  HGDIOBJ hOldIconDC = ::SelectObject(hIconDC, iiNewIcon.hbmColor);
  ASSERT(hOldIconDC != NULL);
  HGDIOBJ hOldMaskDC = ::SelectObject(hMaskDC, iiNewIcon.hbmMask);
  ASSERT(hOldMaskDC != NULL);
  COLORREF brColorItem(clrIcon);
  RECT rcColorItem(0, 0, 16, 16);
  ::FillRect(hMaskDC, &rcColorItem, (HBRUSH)GetStockObject(WHITE_BRUSH));
  rcColorItem DeflateRect(&, 1, 2);
  ::FillRect(hMaskDC, &rcColorItem, (HBRUSH)GetStockObject(BLACK_BRUSH));
  ::FillRect(hIconDC, &rcColorItem, (HBRUSH)brColorItem);
  rcColorItem DeflateRect(&, 2, 2);
  ::FillRect(hMaskDC, &rcColorItem, (HBRUSH)GetStockObject(WHITE_BRUSH));
  ::FillRect(hIconDC, &rcColorItem, (HBRUSH)GetStockObject(BLACK_BRUSH));
  ::SelectObject(hIconDC, hOldIconDC);
  ::SelectObject(hMaskDC, hOldMaskDC);
  HICON hIcon = ::CreateIconIndirect(&iiNewIcon);
  ASSERT(hIcon != NULL);
  ::DeleteObject(iiNewIcon.hbmColor);
  ::DeleteObject(iiNewIcon.hbmMask);
  ::DeleteDC(hMaskDC);
  ::DeleteDC(hIconDC);
  return hIcon;
}
HICON stat_GenerateColorIconChar(COLORREF clrIcon, HFONT hFontForLetter, __EXT_MFC_SAFE_TCHAR tChr // = _T('A')
                                )
{
  ASSERT(tChr != _T('\0'));
  ASSERT(hFontForLetter != NULL);
  HDDC dc(NULL);
  ICONINFO iiNewIcon;
  iiNewIcon.fIcon = TRUE;
  HDC hIconDC = ::CreateCompatibleDC(dc);
  HDC hMaskDC = ::CreateCompatibleDC(dc);
  ASSERT(hIconDC != NULL && hMaskDC != NULL);
  iiNewIcon.hbmColor = ::CreateCompatibleBitmap(dc, 16, 16);
  ASSERT(iiNewIcon.hbmColor != NULL);
  iiNewIcon.hbmMask = ::CreateCompatibleBitmap(hMaskDC, 16, 16);
  ASSERT(iiNewIcon.hbmMask != NULL);
  HGDIOBJ hOldIconDC = ::SelectObject(hIconDC, iiNewIcon.hbmColor);
  ASSERT(hOldIconDC != NULL);
  HGDIOBJ hOldMaskDC = ::SelectObject(hMaskDC, iiNewIcon.hbmMask);
  ASSERT(hOldMaskDC != NULL);
  RECT rcColorItem(0, 0, 16, 16);
  ::FillRect(hMaskDC, &rcColorItem, (HBRUSH)GetStockObject(WHITE_BRUSH));
  ASSERT(hFontForLetter != NULL);
  HGDIOBJ hOldFontIconDC = ::SelectObject(hIconDC, hFontForLetter);
  HGDIOBJ hOldFontMaskDC = ::SelectObject(hMaskDC, hFontForLetter);
  RECT rcText(rcColorItem);
  rcText.bottom -= 4;
  CExtSafeString sText((TCHAR)tChr, 1);
  ::SetTextColor(hIconDC, _RGB(0, 0, 0));
  ::SetBkMode(hIconDC, TRANSPARENT);
  ::DrawText(hIconDC, sText, sText.GetLength(), &rcText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
  ::SetTextColor(hMaskDC, _RGB(0, 0, 0));
  ::SetBkMode(hMaskDC, TRANSPARENT);
  ::DrawText(hMaskDC, sText, sText.GetLength(), &rcText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
  ::SelectObject(hIconDC, hOldFontIconDC);
  ::SelectObject(hMaskDC, hOldFontMaskDC);
  RECT rcColorBar(rcText);
  rcColorBar OffsetRect(&, 0, rcColorBar RCHEIGHT(&) + 1);
  rcColorBar.bottom = rcColorBar.top + 3;
  rcColorBar DeflateRect(&, 2, 0);
  COLORREF brColor(clrIcon);
  ::FillRect(hIconDC, &rcColorBar, (HBRUSH)brColor);
  ::FillRect(hMaskDC, &rcColorBar, (HBRUSH)GetStockObject(BLACK_BRUSH));
  ::SelectObject(hIconDC, hOldIconDC);
  ::SelectObject(hMaskDC, hOldMaskDC);
  HICON hIcon = ::CreateIconIndirect(&iiNewIcon);
  ASSERT(hIcon != NULL);
  ::DeleteObject(iiNewIcon.hbmColor);
  ::DeleteObject(iiNewIcon.hbmMask);
  ::DeleteDC(hMaskDC);
  ::DeleteDC(hIconDC);
  return hIcon;
}
#if (!defined __EXT_MFC_NO_SHORTCUTLIST_CTRL)
int PaintShortcutListBkgnd(HDDC dc, const RECT* rcClient, CObject* pHelperSrc, LPARAM lParam // = 0L
                          )
{
  ASSERT(dc != NULL);
  pHelperSrc;
  lParam;

  if (rcClient.left >= rcClient.right || rcClient.top >= rcClient.bottom || (!dc.RectVisible(&rcClient))) {
    return ;
  }

  COLORREF clrBk = GetColor(COLOR_3DSHADOW);
  TFillRect(dc, &rcClient, clrBk);
}
int CExtPaintManagerXP::PaintShortcutListBkgnd(HDDC dc, const RECT* rcClient, CObject* pHelperSrc, LPARAM lParam // = 0L
                                              )
{
  ASSERT(dc != NULL);
  pHelperSrc;
  lParam;

  if (rcClient.left >= rcClient.right || rcClient.top >= rcClient.bottom || (!dc.RectVisible(&rcClient))) {
    return ;
  }

  COLORREF clrBk = GetColor(CLR_3DFACE_OUT);
  TFillRect(dc, &rcClient, clrBk);
}
int PaintShortcutListScrollButton(HDDC dc, BOOL bHorzLayout, const RECT* rcButton, BOOL bUpOrLeftBtn, BOOL bHover, BOOL bPressed, CObject* pHelperSrc, LPARAM lParam // = 0L
                                 )
{
  ASSERT(dc != NULL);
  pHelperSrc;
  lParam;

  if (::IsRectEmpty(&rcButton) || (!dc.RectVisible(&rcButton))) {
    return ;
  }

  COLORREF clr3dFace = GetColor(COLOR_3DFACE);
  TFillRect(dc, &rcButton, clr3dFace);
  RECT rcGlyph(rcButton);
  COLORREF clrOuterLT = clr3dFace;
  COLORREF clrOuterRB = clr3dFace;
  COLORREF clrInnerLT = clr3dFace;
  COLORREF clrInnerRB = clr3dFace;

  if (bPressed) {
    rcGlyph OffsetRect(&, 1, 1);
    clrOuterLT = GetColor(COLOR_3DDKSHADOW);
    clrOuterRB = GetColor(COLOR_3DHILIGHT);
    clrInnerLT = GetColor(COLOR_3DSHADOW);
  } // if( bPressed )
  else {
    if (bHover) {
      clrOuterRB = GetColor(COLOR_3DDKSHADOW);
      clrInnerLT = GetColor(COLOR_3DHILIGHT);
      clrInnerRB = GetColor(COLOR_3DSHADOW);
    } // if( bHover )
    else {
      clrOuterLT = GetColor(COLOR_3DHILIGHT);
      clrOuterRB = GetColor(COLOR_3DSHADOW);
    } // else from if( bHover )
  } // else from if( bPressed )

  COLORREF ColorValues[2] = {
    _RGB(0, 0, 0), GetColor(COLOR_BTNTEXT)
  };
  PaintGlyphCentered(dc, &rcGlyph, *g_DockingCaptionGlyphs[bHorzLayout ? (bUpOrLeftBtn ? __DCBT_ARROW_LEFT : __DCBT_ARROW_RIGHT) : (bUpOrLeftBtn ? __DCBT_ARROW_UP : __DCBT_ARROW_DOWN)], ColorValues);
  TDraw3DRect(dc, &rcButton, clrOuterLT, clrOuterRB);
  RECT rcButton2(rcButton);
  rcButton2 DeflateRect(&, 1, 1);
  TDraw3DRect(dc, &rcButton2, clrInnerLT, clrInnerRB);
}
int CExtPaintManagerXP::PaintShortcutListScrollButton(HDDC dc, BOOL bHorzLayout, const RECT* rcButton, BOOL bUpOrLeftBtn, BOOL bHover, BOOL bPressed, CObject* pHelperSrc, LPARAM lParam // = 0L
                                                     )
{
  PaintShortcutListScrollButton(dc, bHorzLayout, rcButton, bUpOrLeftBtn, bHover, bPressed, pHelperSrc, lParam);
}
PAINTSHORTCUTLISTITEMSDATA::PAINTSHORTCUTLISTITEMSDATA(): m_pHelperSrc(NULL), m_lParam(0L), m_dwShortcutListStyle(0L), m_hItemTextFont((HFONT)GetStockObject(DEFAULT_GUI_FONT)), m_pIconBig(NULL), m_pIconSmall(NULL), m_lParamItem(0L), m_rcBoundsAll(0, 0, 0, 0), m_rcBoundsIcon(0, 0, 0, 0), m_rcBoundsText(0, 0, 0, 0), m_rcCtrlClient(0, 0, 0, 0), m_sText(_T("")), m_bHover(FALSE), m_bPressed(FALSE), m_bVisible(FALSE), m_bChecked(FALSE), m_bExpandMode(FALSE), m_bHorzLayout(FALSE), m_bBigIconView(FALSE), m_bEraseQuery(FALSE)
{
  ASSERT(m_hItemTextFont != NULL);
}
PAINTSHORTCUTLISTITEMSDATA::PAINTSHORTCUTLISTITEMSDATA(CObject* pHelperSrc, DWORD dwShortcutListStyle, HFONT hItemTextFont, Image* pIconBig, Image* pIconSmall, LPARAM lParamItem, const RECT* rcBoundsAll, const RECT* rcBoundsIcon, const RECT* rcBoundsText, const RECT* rcCtrlClient, __EXT_MFC_SAFE_LPCTSTR sText, BOOL bHover, BOOL bPressed, BOOL bVisible, BOOL bChecked, BOOL bExpandMode, BOOL bHorzLayout, BOOL bBigIconView, BOOL bEraseQuery): m_pHelperSrc(pHelperSrc), m_lParam(0L), m_dwShortcutListStyle(dwShortcutListStyle), m_hItemTextFont(hItemTextFont), m_pIconBig(pIconBig), m_pIconSmall(pIconSmall), m_lParamItem(lParamItem), m_rcBoundsAll(rcBoundsAll), m_rcBoundsIcon(rcBoundsIcon), m_rcBoundsText(rcBoundsText), m_rcCtrlClient(rcCtrlClient), m_sText((sText == NULL) ? _T("") : sText), m_bHover(bHover), m_bPressed(bPressed), m_bVisible(bVisible), m_bChecked(bChecked), m_bExpandMode(bExpandMode), m_bHorzLayout(bHorzLayout), m_bBigIconView(bBigIconView), m_bEraseQuery(bEraseQuery)
{
  ASSERT(m_hItemTextFont != NULL);
}
int PaintShortcutListItem(HDDC dc, PAINTSHORTCUTLISTITEMSDATA& _pslid)
{
  ASSERT(dc != NULL);

  if (_pslid.m_bEraseQuery) {
    if (!_pslid.m_bExpandMode) {
      return ;
    }

    if (_pslid.m_rcBoundsText.left < _pslid.m_rcBoundsText.right && _pslid.m_rcBoundsText.top < _pslid.m_rcBoundsText.bottom && dc.RectVisible(&_pslid.m_rcBoundsText)) {
      RECT rcExpandErase(_pslid.m_rcBoundsText);
      rcExpandErase InflateRect(&, __EXT_SHORTCUTLISTWND_EXPANDED_TEXT_GAP_X, __EXT_SHORTCUTLISTWND_EXPANDED_TEXT_GAP_Y);
      COLORREF clrDarkBorder = GetColor(COLOR_3DDKSHADOW);
      COLORREF clrInfoBk = GetColor(COLOR_INFOBK);
      TFillRect(dc, &rcExpandErase, clrInfoBk);
      TDraw3DRect(dc, &rcExpandErase, clrDarkBorder, clrDarkBorder);
    }

    return ;
  } // if( _pslid.m_bEraseQuery )

  if (_pslid.m_rcBoundsAll IsRectEmpty(&) || (!dc.RectVisible(&_pslid.m_rcBoundsAll))) {
    return ;
  }

  if ((!_pslid.m_rcBoundsIcon IsRectEmpty(&)) && dc.RectVisible(&_pslid.m_rcBoundsIcon)) {
    Image* pIcon = _pslid.m_bBigIconView ? _pslid.m_pIconBig : _pslid.m_pIconSmall;

    if (pIcon == NULL) {
      pIcon = _pslid.m_pIconBig;

      if (pIcon == NULL) {
        pIcon = _pslid.m_pIconSmall;
      }
    } // if( pIcon == NULL )

    RECT rcUnderIcon(_pslid.m_rcBoundsIcon);
    rcUnderIcon InflateRect(&, __EXT_SHORTCUTLISTWND_ICON_BORDER_GAP_X, __EXT_SHORTCUTLISTWND_ICON_BORDER_GAP_Y);

    if (_pslid.m_bPressed || _pslid.m_bHover) {
      TFillRect(dc, &rcUnderIcon, GetColor(COLOR_3DSHADOW));
    }

    if (pIcon != NULL && (!pIcon->IsEmpty())) {
      ::DrawIconEx(dc, _pslid.m_rcBoundsIcon.left, _pslid.m_rcBoundsIcon.top, pIcon->GetIcon(), _pslid.m_rcBoundsIcon RCWIDTH(&), _pslid.m_rcBoundsIcon RCHEIGHT(&), 0, (HBRUSH)NULL, DI_NORMAL);
    } // if( pIcon != NULL && (! pIcon->IsEmpty()) )

    if (_pslid.m_bPressed || _pslid.m_bHover) {
      COLORREF clrLT = GetColor(_pslid.m_bPressed ? COLOR_3DDKSHADOW : COLOR_3DFACE);
      COLORREF clrRB = GetColor(_pslid.m_bPressed ? COLOR_3DFACE : COLOR_3DDKSHADOW);
      TDraw3DRect(dc, &rcUnderIcon, clrLT, clrRB);
    }
  }

  CExtSafeString sItemText = _pslid.m_sText;

  if ((!_pslid.m_rcBoundsText IsRectEmpty(&)) && dc.RectVisible(&_pslid.m_rcBoundsText) && (!sItemText)) {
    LONG nItemTextLen = sItemText.GetLength();
    RECT rcCaption(_pslid.m_rcBoundsText);
    ASSERT(_pslid.m_hItemTextFont != NULL);
    HFONT pItemFont = HFONT::FromHandle(_pslid.m_hItemTextFont);
    ASSERT(pItemFont != NULL);
    ASSERT(pItemFont != NULL);
    HFONT pOldFont = dc.SelectObject(pItemFont);
    COLORREF clrTextOld = textColor = (GetColor(_pslid.m_bExpandMode ? COLOR_INFOTEXT : COLOR_3DHILIGHT
        //_pslid.m_bEnabled ? COLOR_BTNTEXT : COLOR_3DHILIGHT
                                               ));
    int nOldBkMode = dc.SetBkMode(TRANSPARENT);

    if (_pslid.m_bHorzLayout) {
      rcCaption InflateRect(&, 3, 0);
      SIZE sizeTextMeasured = _pslid.m_rcBoundsText.Size();
      uint nOldTA = dc.SetTextAlign(TA_TOP | TA_BASELINE);
      rcCaption OffsetRect(&, ((_pslid.m_dwShortcutListStyle & __ESLW_INV_VFONT) == 0) ? sizeTextMeasured.w / 2 : - sizeTextMeasured.w / 2 + 2, 0);
      POINT ptCenter = rcCaption.CenterPoint();

      if ((_pslid.m_dwShortcutListStyle & __ESLW_INV_VFONT) == 0) {
        ptCenter.y = rcCaption.bottom - 4 - (rcCaption RCHEIGHT(&) - sizeTextMeasured.h);
      }
      else {
        ptCenter.y = rcCaption.top + 4;
      }

      dc.ExtTextOut(ptCenter.x, ptCenter.y, ETO_CLIPPED, &rcCaption, sItemText, sItemText.GetLength(), NULL);
      dc.SetTextAlign(nOldTA);
    } // if( _pslid.m_bHorzLayout )
    else {
      RECT rcClient(_pslid.m_rcCtrlClient);
      rcClient DeflateRect(&, 1, 0);
      RECT rcText(max(rcCaption.left, rcClient.left), rcCaption.top, min(rcCaption.right, rcClient.right), rcCaption.bottom);

      if (rcText.left < rcText.right) {
        uint nFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
        TDrawText(dc, sItemText, nItemTextLen, &rcText, nFormat);
      } // if( rcText.left < rcText.right )
    } // else from if( _pslid.m_bHorzLayout )

    dc.SetBkMode(nOldBkMode);
    textColor = (clrTextOld);
    dc.SelectObject(pOldFont);
  }
}
int CExtPaintManagerXP::PaintShortcutListItem(HDDC dc, PAINTSHORTCUTLISTITEMSDATA& _pslid)
{
  ASSERT(dc != NULL);

  if (_pslid.m_bEraseQuery) {
    if (!_pslid.m_bExpandMode) {
      return ;
    }

    if (_pslid.m_rcBoundsText.left < _pslid.m_rcBoundsText.right && _pslid.m_rcBoundsText.top < _pslid.m_rcBoundsText.bottom && dc.RectVisible(&_pslid.m_rcBoundsText)) {
      RECT rcExpandErase(_pslid.m_rcBoundsText);
      rcExpandErase InflateRect(&, __EXT_SHORTCUTLISTWND_EXPANDED_TEXT_GAP_X, __EXT_SHORTCUTLISTWND_EXPANDED_TEXT_GAP_Y);
      COLORREF clrDarkBorder = GetColor(COLOR_3DDKSHADOW);
      COLORREF clrInfoBk = GetColor(COLOR_INFOBK);
      TFillRect(dc, &rcExpandErase, clrInfoBk);
      TDraw3DRect(dc, &rcExpandErase, clrDarkBorder, clrDarkBorder);
    }

    return ;
  } // if( _pslid.m_bEraseQuery )

  if (_pslid.m_rcBoundsAll IsRectEmpty(&) || (!dc.RectVisible(&_pslid.m_rcBoundsAll))) {
    return ;
  }

  if ((!_pslid.m_rcBoundsIcon IsRectEmpty(&)) && dc.RectVisible(&_pslid.m_rcBoundsIcon)) {
    Image* pIcon = _pslid.m_bBigIconView ? _pslid.m_pIconBig : _pslid.m_pIconSmall;

    if (pIcon == NULL) {
      pIcon = _pslid.m_pIconBig;

      if (pIcon == NULL) {
        pIcon = _pslid.m_pIconSmall;
      }
    }

    RECT rcUnderIcon(_pslid.m_rcBoundsIcon);
    rcUnderIcon InflateRect(&, __EXT_SHORTCUTLISTWND_ICON_BORDER_GAP_X, __EXT_SHORTCUTLISTWND_ICON_BORDER_GAP_Y);

    // TFillRect(dc,
    // &rcUnderIcon,
    // GetColor(
    // _pslid.m_bPressed
    // ? XPCLR_HILIGHT
    // : ( _pslid.m_bHover
    // ? CLR_3DFACE_IN
    // : CLR_3DFACE_OUT
    // )
    // )
    // );
    if (_pslid.m_bPressed || _pslid.m_bHover) {
      TFillRect(dc, &rcUnderIcon, GetColor(_pslid.m_bPressed ? XPCLR_HILIGHT : CLR_3DFACE_IN));
    }

    if (pIcon != NULL && (!pIcon->IsEmpty())) {
      ::DrawIconEx(dc, _pslid.m_rcBoundsIcon.left, _pslid.m_rcBoundsIcon.top, pIcon->GetIcon(), _pslid.m_rcBoundsIcon RCWIDTH(&), _pslid.m_rcBoundsIcon RCHEIGHT(&), 0, (HBRUSH)NULL, DI_NORMAL);
    } // if( pIcon != NULL && (! pIcon->IsEmpty()) )

    if (_pslid.m_bPressed || _pslid.m_bHover) {
      COLORREF clr = GetColor(XPCLR_HILIGHT_BORDER);
      TDraw3DRect(dc, &rcUnderIcon, clr, clr);
    }
  }

  CExtSafeString sItemText = _pslid.m_sText;

  if ((!_pslid.m_rcBoundsText IsRectEmpty(&)) && dc.RectVisible(&_pslid.m_rcBoundsText) && (!sItemText)) {
    LONG nItemTextLen = sItemText.GetLength();
    RECT rcCaption(_pslid.m_rcBoundsText);
    ASSERT(_pslid.m_hItemTextFont != NULL);
    HFONT pItemFont = HFONT::FromHandle(_pslid.m_hItemTextFont);
    ASSERT(pItemFont != NULL);
    ASSERT(pItemFont != NULL);
    HFONT pOldFont = dc.SelectObject(pItemFont);
    COLORREF clrTextOld = textColor = (GetColor(_pslid.m_bExpandMode ? COLOR_INFOTEXT : COLOR_BTNTEXT));
    int nOldBkMode = dc.SetBkMode(TRANSPARENT);

    if (_pslid.m_bHorzLayout) {
      rcCaption InflateRect(&, 3, 0);
      SIZE sizeTextMeasured = _pslid.m_rcBoundsText.Size();
      uint nOldTA = dc.SetTextAlign(TA_TOP | TA_BASELINE);
      rcCaption OffsetRect(&, ((_pslid.m_dwShortcutListStyle & __ESLW_INV_VFONT) == 0) ? sizeTextMeasured.w / 2 : - sizeTextMeasured.w / 2 + 2, 0);
      POINT ptCenter = rcCaption.CenterPoint();

      if ((_pslid.m_dwShortcutListStyle & __ESLW_INV_VFONT) == 0) {
        ptCenter.y = rcCaption.bottom - 4 - (rcCaption RCHEIGHT(&) - sizeTextMeasured.h);
      }
      else {
        ptCenter.y = rcCaption.top + 4;
      }

      dc.ExtTextOut(ptCenter.x, ptCenter.y, ETO_CLIPPED, &rcCaption, sItemText, sItemText.GetLength(), NULL);
      dc.SetTextAlign(nOldTA);
    } // if( _pslid.m_bHorzLayout )
    else {
      RECT rcClient(_pslid.m_rcCtrlClient);
      rcClient DeflateRect(&, 1, 0);
      RECT rcText(max(rcCaption.left, rcClient.left), rcCaption.top, min(rcCaption.right, rcClient.right), rcCaption.bottom);

      if (rcText.left < rcText.right) {
        uint nFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
        TDrawText(dc, sItemText, nItemTextLen, &rcText, nFormat);
      } // if( rcText.left < rcText.right )
    } // else from if( _pslid.m_bHorzLayout )

    dc.SetBkMode(nOldBkMode);
    textColor = (clrTextOld);
    dc.SelectObject(pOldFont);
  }
}
#endif // (!defined __EXT_MFC_NO_SHORTCUTLIST_CTRL)
COLORREF GetShadowAdjustColor()
{
  return ((COLORREF)(- 1));
}
COLORREF CExtPaintManagerXP::GetShadowAdjustColor()
{
  return ((COLORREF)(- 1));
}
COLORREF CExtPaintManagerOffice2003::GetShadowAdjustColor()
{
  if (m_bUxApiInitPassed && m_bUxValidColorsExtracted && m_bUxUseIfAvailOnWinXpOrLater) {
    return m_clrUxTaskBandFillColorHint;
  }

  return ((COLORREF)(- 1));
}
int FixedBar_AdjustClientRect(CExtControlBar* pBar, RECT* rcPreCalc)
{
  ASSERT_VALID(pBar);
  ASSERT_KINDOF(CExtControlBar, pBar);
  ASSERT(pBar->IsFixedMode());

  if (pBar->IsDockedVertically()) {
    rcPreCalc DeflateRect(&, __EXT_TB_ROW_DISTANCE / 4, __EXT_TB_ROW_DISTANCE / 2);
  }
  else {
    rcPreCalc DeflateRect(&, __EXT_TB_ROW_DISTANCE / 2, __EXT_TB_ROW_DISTANCE / 4);
  }
}
int CExtPaintManagerXP::FixedBar_AdjustClientRect(CExtControlBar* pBar, RECT* rcPreCalc)
{
  FixedBar_AdjustClientRect(pBar, rcPreCalc);
}
int CExtPaintManagerOffice2003::FixedBar_AdjustClientRect(CExtControlBar* pBar, RECT* rcPreCalc)
{
  ASSERT_VALID(pBar);
  ASSERT_KINDOF(CExtControlBar, pBar);
  ASSERT(pBar->IsFixedMode());

  if (stat_GetBPP() <= 8) {
    CExtPaintManagerXP::FixedBar_AdjustClientRect(pBar, rcPreCalc);

    if (pBar->IsDockedVertically()) {
      rcPreCalc InflateRect(&, 0, 2);
    }
    else {
      rcPreCalc InflateRect(&, 2, 0);
    }
  }
}
INT FixedBar_GetRowDistance(CExtToolControlBar* pBar)
{
  ASSERT_VALID(pBar);
  ASSERT_KINDOF(CExtToolControlBar, pBar);
  ASSERT(pBar->IsFixedMode());

  if (pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar))) {
    return 0;
  }

  return __EXT_TB_ROW_DISTANCE;
}
INT CExtPaintManagerXP::FixedBar_GetRowDistance(CExtToolControlBar* pBar)
{
  return FixedBar_GetRowDistance(pBar);
}
INT CExtPaintManagerOffice2003::FixedBar_GetRowDistance(CExtToolControlBar* pBar)
{
  ASSERT_VALID(pBar);
  ASSERT_KINDOF(CExtToolControlBar, pBar);
  ASSERT(pBar->IsFixedMode());

  if (((!pBar->IsFloating())
      // && pBar->OnQueryMultiRowLayout()
      ) || pBar->m_bPaletteMode || stat_GetBPP() <= 8) {
    return CExtPaintManagerXP::FixedBar_GetRowDistance(pBar);
  }

  return 0;
}
BOOL FixedBar_IsPaintRowBkMode(CExtToolControlBar* pBar)
{
  ASSERT_VALID(pBar);
  ASSERT_KINDOF(CExtToolControlBar, pBar);
  ASSERT(pBar->IsFixedMode());
  pBar;
  return FALSE;
}
BOOL CExtPaintManagerXP::FixedBar_IsPaintRowBkMode(CExtToolControlBar* pBar)
{
  return FixedBar_IsPaintRowBkMode(pBar);
}
BOOL CExtPaintManagerOffice2003::FixedBar_IsPaintRowBkMode(CExtToolControlBar* pBar)
{
  ASSERT_VALID(pBar);
  ASSERT_KINDOF(CExtToolControlBar, pBar);
  ASSERT(pBar->IsFixedMode());

  if (pBar->m_pDockSite == NULL || pBar->m_bPresubclassDialogMode || pBar->m_bPaletteMode || pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) || pBar->IsKindOf(RUNTIME_CLASS(CExtPanelControlBar)) || (!pBar->IsFloating()) || stat_GetBPP() <= 8) {
    return FALSE;
  }

  return TRUE;
}
int PaintFloatToolbarRowBk(HDDC dc, CExtToolControlBar* pBar, int nLastReviewBtnIdx, RECT* rcRowBk)
{
  ASSERT(dc != NULL);
  ASSERT_VALID(pBar);
  ASSERT_KINDOF(CExtToolControlBar, pBar);
  ASSERT(nLastReviewBtnIdx >= 0);
  dc;
  pBar;
  nLastReviewBtnIdx;
  rcRowBk;
}
int CExtPaintManagerXP::PaintFloatToolbarRowBk(HDDC dc, CExtToolControlBar* pBar, int nLastReviewBtnIdx, RECT* rcRowBk)
{
  ASSERT(dc != NULL);
  ASSERT_VALID(pBar);
  ASSERT_KINDOF(CExtToolControlBar, pBar);
  ASSERT(nLastReviewBtnIdx >= 0);
  dc;
  pBar;
  nLastReviewBtnIdx;
  rcRowBk;
}
int CExtPaintManagerOffice2003::PaintFloatToolbarRowBk(HDDC dc, CExtToolControlBar* pBar, int nLastReviewBtnIdx, RECT* rcRowBk)
{
  ASSERT(dc != NULL);
  ASSERT_VALID(pBar);
  ASSERT_KINDOF(CExtToolControlBar, pBar);
  ASSERT(nLastReviewBtnIdx >= 0);
  pBar;
  nLastReviewBtnIdx;
  COLORREF clrTop = GetColor(_2003CLR_MLA_NORM_RIGHT);
  COLORREF clrBottom = GetColor(_2003CLR_SEPARATOR_LIGHT);
  stat_PaintGradientRect(dc, rcRowBk, clrTop, clrBottom, TRUE);
}
int OnSysColorChange()
{
  OnPaintSessionComplete(this);
  SyncSysColors();
  InitTranslatedColors();
}
int OnSettingChange(uint uFlags, __EXT_MFC_SAFE_LPCTSTR lpszSection)
{
  uFlags;
  lpszSection;
  OnPaintSessionComplete(this);
  SyncSysColors();
  InitTranslatedColors();
}
int OnDisplayChange(INT nDepthBPP, POINT ptSizes)
{
  nDepthBPP;
  ptSizes;
  OnPaintSessionComplete(this);
  SyncSysColors();
  InitTranslatedColors();
}
int OnThemeChanged(WPARAM wParam, LPARAM lParam)
{
  wParam;
  lParam;
  OnPaintSessionComplete(this);
  SyncSysColors();
  InitTranslatedColors();
}
int OnPaintSessionComplete(CObject* pHelperSrc, LPARAM lParam // = 0L
                          )
{
#ifdef __TRACE_PAINT_SESSIONS
  TRACE(" PAINT_SESSION: Complete\n");
#endif // __TRACE_PAINT_SESSIONS
  pHelperSrc;
  lParam;
  CachedDcFreeAll();
}
int CExtPaintManagerXP::OnPaintSessionComplete(CObject* pHelperSrc, LPARAM lParam // = 0L
                                              )
{
  OnPaintSessionComplete(pHelperSrc, lParam);
}
int CExtPaintManagerOffice2003::OnPaintSessionComplete(CObject* pHelperSrc, LPARAM lParam // = 0L
                                                      )
{
  CExtPaintManagerXP::OnPaintSessionComplete(pHelperSrc, lParam);
}
SIZE GetToolBarRgnRoundersSize()
{
  return SIZE(0, 0);
}
SIZE CExtPaintManagerXP::GetToolBarRgnRoundersSize()
{
  return SIZE(0, 0);
}
SIZE CExtPaintManagerOffice2003::GetToolBarRgnRoundersSize()
{
  return m_sizeToolBarRgnRounders;
}
PAINTTOOLBARTEXTFIELDDATA::PAINTTOOLBARTEXTFIELDDATA(): m_pHelperSrc(NULL), m_lParam(0L), m_nDrawTextFlags(DT_SINGLELINE | DT_LEFT | DT_TOP | DT_NOPREFIX), m_sText(_T("")), m_rcButton(0, 0, 0, 0), m_rcTextField(0, 0, 0, 0), m_bComboField(FALSE), m_bHover(FALSE), m_bPushed(FALSE), m_bEnabled(FALSE) {}
PAINTTOOLBARTEXTFIELDDATA::PAINTTOOLBARTEXTFIELDDATA(CObject* pHelperSrc, __EXT_MFC_SAFE_LPCTSTR sText, const RECT* rcButton, const RECT* rcTextField, BOOL bComboField, BOOL bHover, BOOL bPushed, BOOL bEnabled): m_pHelperSrc(pHelperSrc), m_lParam(0L), m_nDrawTextFlags(DT_SINGLELINE | DT_LEFT | DT_TOP | DT_NOPREFIX), m_sText((sText == NULL) ? _T("") : sText), m_rcButton(rcButton), m_rcTextField(rcTextField), m_bComboField(bComboField), m_bHover(bHover), m_bPushed(bPushed), m_bEnabled(bEnabled) {}
int PaintToolbarTextField(HDDC dc, PAINTTOOLBARTEXTFIELDDATA& _ptbtfd)
{
  ASSERT(dc != NULL);

  if (_ptbtfd.m_bComboField) {
    PAINTCOMBOFRAMEDATA _pcfd(_ptbtfd.m_pHelperSrc, _ptbtfd.m_rcButton, _ptbtfd.m_bHover, _ptbtfd.m_bPushed, _ptbtfd.m_bEnabled);
    _pcfd->m_bNoTopOuterBorder = TRUE;
    PaintComboFrame(dc, _pcfd);
    dc.SelectClipRgn(NULL);
  } // if( _ptbtfd.m_bComboField )
  else {
    if (_ptbtfd.m_bEnabled && (_ptbtfd.m_bHover || _ptbtfd.m_bPushed)) {
      TDraw3DRect(dc, &_ptbtfd.m_rcButton, GetColor(COLOR_3DSHADOW), GetColor(COLOR_3DHIGHLIGHT));
    }
  } // else from if( _ptbtfd.m_bComboField )

  TFillRect(dc, &_ptbtfd.m_rcTextField, GetColor(_ptbtfd.m_bEnabled ? COLOR_WINDOW : COLOR_3DFACE));

  if (!_ptbtfd.m_sText) {
    COLORREF clrTextOld = textColor = (GetColor(COLOR_BTNTEXT));
    HFONT pFontOld = dc.SelectObject(m_FontNormal);
    int nBkModeOld = dc.SetBkMode(TRANSPARENT);
    RECT rcText(_ptbtfd.m_rcTextField);
    rcText.left += 5;
    rcText.top += 2;
    TDrawText(dc, _ptbtfd.m_sText, _ptbtfd.m_sText.GetLength(), &rcText, _ptbtfd.m_nDrawTextFlags);
    dc.SetBkMode(nBkModeOld);
    dc.SelectObject(pFontOld);
    textColor = (clrTextOld);
  } // if( !_ptbtfd.m_sText )
}
int CExtPaintManagerXP::PaintToolbarTextField(HDDC dc, PAINTTOOLBARTEXTFIELDDATA& _ptbtfd)
{
  ASSERT(dc != NULL);

  if (_ptbtfd.m_bComboField) {
    PaintToolbarTextField(dc, _ptbtfd);
    return ;
  }

  if (_ptbtfd.m_bEnabled && (_ptbtfd.m_bHover || _ptbtfd.m_bPushed)) {
    COLORREF clrDkBorder = GetColor(XPCLR_HILIGHT_BORDER);
    RECT rcTextFieldOuter(_ptbtfd.m_rcTextField);
    rcTextFieldOuter InflateRect(&, 1, 1);
    TDraw3DRect(dc, &rcTextFieldOuter, clrDkBorder, clrDkBorder);
  } // if( _ptbtfd.m_bEnabled && (_ptbtfd.m_bHover || _ptbtfd.m_bPushed) )

  TFillRect(dc, &_ptbtfd.m_rcTextField, GetColor(_ptbtfd.m_bEnabled ? COLOR_WINDOW : COLOR_3DFACE));

  if (!_ptbtfd.m_sText) {
    COLORREF clrTextOld = textColor = (GetColor(COLOR_BTNTEXT));
    HFONT pFontOld = dc.SelectObject(m_FontNormal);
    int nBkModeOld = dc.SetBkMode(TRANSPARENT);
    RECT rcText(_ptbtfd.m_rcTextField);
    rcText.left += 5;
    rcText.top += 2;
    TDrawText(dc, _ptbtfd.m_sText, _ptbtfd.m_sText.GetLength(), &rcText, _ptbtfd.m_nDrawTextFlags);
    dc.SetBkMode(nBkModeOld);
    dc.SelectObject(pFontOld);
    textColor = (clrTextOld);
  } // if( !_ptbtfd.m_sText )
}
COLORREF GetIconAlphaColor()const
{
  return (COLORREF(- 1L)); // not alpha icon effect
}
COLORREF CExtPaintManagerXP::GetIconAlphaColor()const
{
  return (COLORREF(- 1L)); // not alpha icon effect
}
COLORREF CExtPaintManagerOffice2003::GetIconAlphaColor()const
{
  COLORREF clrAlpha = (const_cast < CExtPaintManagerOffice2003* >(this))->GetColor(COLOR_3DFACE);
  return clrAlpha;
}
double GetIconAlphaAdjustPercent()const
{
  return 0.0; // no alpha adjustment
}
double CExtPaintManagerXP::GetIconAlphaAdjustPercent()const
{
  return 0.0; // no alpha adjustment
}
double CExtPaintManagerOffice2003::GetIconAlphaAdjustPercent()const
{
  return 0.70; // 70% enlighten
}
int PaintResizingGripper(HDDC dc, const RECT* rcGrip, CObject* pHelperSrc, LPARAM lParam // = 0L
                        )
{
  ASSERT(dc != NULL);
  pHelperSrc;
  lParam;

  if (rcGrip.left > rcGrip.right || rcGrip.top > rcGrip.bottom || (!dc.RectVisible(&rcGrip))) {
    return ;
  }

  HFONT pOldFont = dc.SelectObject(m_FontMarlett);
  ASSERT(pOldFont != NULL);
  COLORREF clrOld = textColor = (GetColor(CLR_TEXT_OUT));
  INT nOldBkMode = dc.SetBkMode(TRANSPARENT);
  TDrawText(dc, _T("o"), 1, (LPRECT) &rcGrip, DT_SINGLELINE | DT_RIGHT | DT_BOTTOM);
  dc.SetBkMode(nOldBkMode);
  textColor = (clrOld);
  dc.SelectObject(pOldFont);
}
int CExtPaintManagerXP::PaintResizingGripper(HDDC dc, const RECT* rcGrip, CObject* pHelperSrc, LPARAM lParam // = 0L
                                            )
{
  ASSERT(dc != NULL);
  PaintResizingGripper(dc, rcGrip, pHelperSrc, lParam);
}
int CExtPaintManagerOffice2003::PaintResizingGripper(HDDC dc, const RECT* rcGrip, CObject* pHelperSrc, LPARAM lParam // = 0L
                                                    )
{
  ASSERT(dc != NULL);
  pHelperSrc;
  lParam;

  if (stat_GetBPP() <= 8) {
    CExtPaintManagerXP::PaintResizingGripper(dc, rcGrip, pHelperSrc, lParam);
    return ;
  }

  COLORREF clrDotShadow = GetColor(_2003CLR_GRIPPER_DOT_LIGHT);
  COLORREF clrDotFace = GetColor(_2003CLR_GRIPPER_DOT_DARK);
  static const SIZE g_sizeGripDot(2, 2);
  static const SIZE g_sizeGripDist(1, 1);
  static const SIZE g_sizeGripShadowOffset(1, 1);
  RECT rcDotFace(POINT(rcGrip.right, rcGrip.bottom), g_sizeGripDot);
  rcDotFace OffsetRect(&, - g_sizeGripDot);
  RECT rcDotShadow(rcDotFace);
  rcDotFace OffsetRect(&, - g_sizeGripShadowOffset);
  RECT rcDotFaceSave(rcDotFace);
  RECT rcDotShadowSave(rcDotShadow);
  int nStepH = - (g_sizeGripDot.w + g_sizeGripDist.w + g_sizeGripShadowOffset.w);
  int nStepV = - (g_sizeGripDot.h + g_sizeGripDist.h + g_sizeGripShadowOffset.h);

  for (int nDot = 0; nDot < 3; nDot++) {
    TFillRect(dc, &rcDotShadow, clrDotShadow);
    TFillRect(dc, &rcDotFace, clrDotFace);
    rcDotFace OffsetRect(&, 0, nStepV);
    rcDotShadow OffsetRect(&, 0, nStepV);
  } // for( int nDot = 0; nDot < nDotCount; nDot++ )

  rcDotFace = rcDotFaceSave;
  rcDotShadow = rcDotShadowSave;
  rcDotFace OffsetRect(&, nStepH, 0);
  rcDotShadow OffsetRect(&, nStepH, 0);

  for (nDot = 0; nDot < 2; nDot++) {
    TFillRect(dc, &rcDotShadow, clrDotShadow);
    TFillRect(dc, &rcDotFace, clrDotFace);
    rcDotFace OffsetRect(&, 0, nStepV);
    rcDotShadow OffsetRect(&, 0, nStepV);
  } // for( nDot = 0; nDot < 2; nDot++ )

  rcDotFace = rcDotFaceSave;
  rcDotShadow = rcDotShadowSave;
  rcDotFace OffsetRect(&, nStepH * 2, 0);
  rcDotShadow OffsetRect(&, nStepH * 2, 0);
  TFillRect(dc, &rcDotShadow, clrDotShadow);
  TFillRect(dc, &rcDotFace, clrDotFace);
}
BOOL InstallPaintManager(CExtPaintManager* pPaintManager)
{
  SyncSysColors();
  InitTranslatedColors();
  return TRUE;
}
BOOL InstallPaintManager(CRuntimeClass* pRtcPaintManager)
{
  if (pRtcPaintManager == NULL) {
    return InitPaintManagerInstance();
  }

  CObject* pObj = pRtcPaintManager->CreateObject();

  if (pObj == NULL) {
    ASSERT(FALSE);
    return FALSE;
  }

  ASSERT_VALID(pObj);
  CExtPaintManager* pPaintManager = DYNAMIC_DOWNCAST(CExtPaintManager, pObj);

  if (pPaintManager == NULL) {
    delete pObj;
    ASSERT(FALSE);
    return FALSE;
  }

  return InstallPaintManager(pPaintManager);
}
uint GetMouseWheelScrollLines()
{
  if (m_bHaveScrollLines) {
    return m_uCachedScrollLines;
  }

  m_bHaveScrollLines = TRUE;
  m_uCachedScrollLines = 3;
#if _MFC_VER < 0x700

  if (!afxData.bWin4) {
    HKEY hKey;

    if (::RegOpenKeyEx(HKEY_CURRENT_USER, _T("Control Panel\\Desktop"), 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS) {
      TCHAR szData[256];
      DWORD dwKeyDataType;
      DWORD dwDataBufSize = _countof(szData);

      if (RegQueryValueEx(hKey, _T("WheelScrollLines"), NULL, &dwKeyDataType, (LPBYTE) &szData, &dwDataBufSize) == ERROR_SUCCESS) {
        m_uCachedScrollLines = _tcstoul(szData, NULL, 10);
      }

      RegCloseKey(hKey);
    }
  }
  else
#endif // _MFC_VER < 0x700
    if (!afxData.bWin95) {
      ::SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &m_uCachedScrollLines, 0);
    }

  return m_uCachedScrollLines;
}
BOOL IsWndUpdateSource(CWnd* pWndNotifySrc)
{
  ASSERT_VALID(pWndNotifySrc);
  ASSERT(pWndNotifySrc->GetSafeHwnd() != NULL && IsWindow(pWndNotifySrc->GetSafeHwnd()));
  ASSERT(m_pPaintManager != NULL);

  if (pWndNotifySrc->IsKindOf(RUNTIME_CLASS(CExtStatusControlBar))) {
    return TRUE;
  }

  CFrameWnd* pFrame = DYNAMIC_DOWNCAST(CFrameWnd, pWndNotifySrc);

  if (pFrame == NULL) {
    pFrame = pWndNotifySrc->GetParentFrame();
  }

  if (pFrame == NULL) {
    return TRUE;
  }

  if (pFrame->IsKindOf(RUNTIME_CLASS(CExtMiniDockFrameWnd))) {
    pFrame = pFrame->GetParentFrame();
  }

  if (pFrame == NULL || pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd))) {
    return TRUE;
  }

  CWnd* pWnd = pFrame->GetControlBar(AFX_IDW_STATUS_BAR);

  if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CExtStatusControlBar))) {
    if (pWnd == pWndNotifySrc) {
      return TRUE;
    }

    return FALSE;
  }

  pWnd = pFrame->GetControlBar(AFX_IDW_DOCKBAR_TOP);

  if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CExtDockOuterBar))) {
    if (pWnd == pWndNotifySrc) {
      return TRUE;
    }

    return FALSE;
  }

  pWnd = pFrame->GetControlBar(AFX_IDW_DOCKBAR_BOTTOM);

  if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CExtDockOuterBar))) {
    if (pWnd == pWndNotifySrc) {
      return TRUE;
    }

    return FALSE;
  }

  pWnd = pFrame->GetControlBar(AFX_IDW_DOCKBAR_LEFT);

  if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CExtDockOuterBar))) {
    if (pWnd == pWndNotifySrc) {
      return TRUE;
    }

    return FALSE;
  }

  pWnd = pFrame->GetControlBar(AFX_IDW_DOCKBAR_RIGHT);

  if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CExtDockOuterBar))) {
    if (pWnd == pWndNotifySrc) {
      return TRUE;
    }

    return FALSE;
  }

#if (!defined __EXT_MFC_NO_TABMDI_CTRL)

  for (pWnd = pFrame->GetWindow(GW_CHILD); pWnd != NULL; pWnd = pWnd->GetWindow(GW_HWNDNEXT)) {
    if (pWnd->IsKindOf(RUNTIME_CLASS(CExtTabMdiWnd))) {
      if (pWnd == pWndNotifySrc) {
        return TRUE;
      }

      return FALSE;
    }
  }

#endif // if (!defined __EXT_MFC_NO_TABMDI_CTRL)
  return TRUE;
}
int OnSysColorChange(CWnd* pWndNotifySrc)
{
  ASSERT_VALID(pWndNotifySrc);
  ASSERT(pWndNotifySrc->GetSafeHwnd() != NULL && IsWindow(pWndNotifySrc->GetSafeHwnd()));
  ASSERT(m_pPaintManager != NULL);

  if (!IsWndUpdateSource(pWndNotifySrc)) {
    return ;
  }

  m_pPaintManager->OnSysColorChange();
}
int OnSettingChange(CWnd* pWndNotifySrc, uint uFlags, __EXT_MFC_SAFE_LPCTSTR lpszSection)
{
  ASSERT_VALID(pWndNotifySrc);
  ASSERT(pWndNotifySrc->GetSafeHwnd() != NULL && IsWindow(pWndNotifySrc->GetSafeHwnd()));
  ASSERT(m_pPaintManager != NULL);

  if (!IsWndUpdateSource(pWndNotifySrc)) {
    return ;
  }

  m_bHaveScrollLines = FALSE;
  m_pPaintManager->OnSettingChange(uFlags, lpszSection);
}
int OnDisplayChange(CWnd* pWndNotifySrc, INT nDepthBPP, POINT ptSizes)
{
  ASSERT_VALID(pWndNotifySrc);
  ASSERT(pWndNotifySrc->GetSafeHwnd() != NULL && IsWindow(pWndNotifySrc->GetSafeHwnd()));
  ASSERT(m_pPaintManager != NULL);

  if (!IsWndUpdateSource(pWndNotifySrc)) {
    return ;
  }

  m_bHaveScrollLines = FALSE;
  m_pPaintManager->OnDisplayChange(nDepthBPP, ptSizes);
}
int OnThemeChanged(CWnd* pWndNotifySrc, WPARAM wParam, LPARAM lParam)
{
  ASSERT_VALID(pWndNotifySrc);
  ASSERT(pWndNotifySrc->GetSafeHwnd() != NULL && IsWindow(pWndNotifySrc->GetSafeHwnd()));
  ASSERT(m_pPaintManager != NULL);

  if (!IsWndUpdateSource(pWndNotifySrc)) {
    return ;
  }

  m_bHaveScrollLines = FALSE;
  m_pPaintManager->OnThemeChanged(wParam, lParam);
}
BOOL InitPaintManagerInstance()
{
  if (m_pPaintManager != NULL) {
    return TRUE;
  }

  return InstallPaintManager((CExtPaintManager*)NULL);
}
InitCExtPaintManagerAutoPtr()
{
  TCHAR* sTmp;
  m_pPaintManager = (NULL), m_bHaveScrollLines = (FALSE), m_uCachedScrollLines = (0);
  m_hInstUserExTheme = (NULL), m_bUxApiInitPassed = (FALSE), m_bUxValidColorsExtracted = (FALSE);
  m_bUxAppThemed = (FALSE), m_bUxUseIfAvailOnWinXpOrLater = (TRUE), strcpy(m_strOsVer, (_T("Unknown")));
  m_bIsWin32s = (FALSE), m_bIsWin9x = (FALSE), m_bIsWin98 = (FALSE), m_bIsWin98orLater = (FALSE);
  m_bIsWinNT = (FALSE), m_bIsWinNT4 = (FALSE), m_bIsWin2000 = (FALSE), m_bIsWinXP == (FALSE);
  m_bIsWinNT4orLater = (FALSE), m_bIsWin2000orLater = (FALSE), m_bIsWinXPorLater = (FALSE);
#ifdef WM_THEMECHANGED
  ASSERT(__ExtMfc_WM_THEMECHANGED == WM_THEMECHANGED);
#endif // WM_THEMECHANGED
  memset((char*) &m_osVerData, 0, sizeof(OSVERSIONINFO));
  m_osVerData.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  sTmp = (_T(""));
  VERIFY(GetVersionEx(&m_osVerData));

  switch (m_osVerData.dwPlatformId) {
  case VER_PLATFORM_WIN32s:
    m_bIsWin32s = TRUE;
    sTmp = _T("Win32s");
    break;

  case VER_PLATFORM_WIN32_WINDOWS: {
    m_bIsWin9x = TRUE;
    m_bIsWin98orLater = (m_osVerData.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) && ((m_osVerData.dwMajorVersion > 4) || ((m_osVerData.dwMajorVersion == 4) && (m_osVerData.dwMinorVersion > 0)));
    sTmp = _T("Windows ");

    if (m_bIsWin98orLater) {
      if ((m_osVerData.dwMajorVersion == 4) && (m_osVerData.dwMinorVersion > 0)) {
        m_bIsWin98 = TRUE;
        sTmp = _T("Windows 98");
      }
      else {
        sTmp = _T("Windows 98 or later");
      }
    }
    else {
      sTmp = _T("Windows 95");
    }
  }
  break;

  case VER_PLATFORM_WIN32_NT:
    m_bIsWinNT = TRUE;

    if (m_osVerData.dwMajorVersion == 5) {
      if (m_osVerData.dwMinorVersion == 0) {
        m_bIsWin2000 = TRUE;
        sTmp = _T("Windows 2000");
      }
      else if (m_osVerData.dwMinorVersion == 1) {
        m_bIsWinXP = TRUE;
        sTmp = _T("Windows XP");
      }
    }
    else if (m_osVerData.dwMajorVersion == 4) {
      m_bIsWinNT4 = TRUE;
      sTmp = _T("Windows NT");
    }
    else {
      sTmp = _T("Windows NT");
    }

    if (m_osVerData.dwMajorVersion >= 4) {
      m_bIsWinNT4orLater = TRUE;
    }

    if (m_osVerData.dwMajorVersion >= 5) {
      m_bIsWin2000orLater = TRUE;

      if (!(m_osVerData.dwMajorVersion == 5 && m_osVerData.dwMinorVersion == 0)) {
        m_bIsWinXPorLater = TRUE;
      }
    }

    break;

  default:
    break;
  } // switch(m_osVerData.dwPlatformId)

  _snprintf(m_strOsVer, 256, _T("%s v.%lu.%lu (build %lu) %s"), sTmp, m_osVerData.dwMajorVersion, m_osVerData.dwMinorVersion, m_osVerData.dwBuildNumber, m_osVerData.szCSDVersion);
  // init uxtheme.dll data
  VERIFY(InitPaintManagerInstance());
}
#endif

