//****

#ifndef __GLOBAL_DATA_H
#define __GLOBAL_DATA_H

#define BCGCBPRODLLEXPORT
inline BOOL IsBCGPStandardCommand(UINT uiCmd)
{
  return  1;
}

BCGCBPRODLLEXPORT void BCGCBProCleanUp();

//extern BCGCBPRODLLEXPORT CFrameWnd* g_pBCGPTopLevelFrame;

inline BCGCBPRODLLEXPORT void BCGCBProSetTopLevelFrame(CFrameWnd* pFrame)
{
  //g_pBCGPTopLevelFrame = pFrame;
}

inline BCGCBPRODLLEXPORT CFrameWnd* BCGCBProGetTopLevelFrame(const CWnd* pWnd)
{
  ASSERT_VALID(pWnd);
  //return g_pBCGPTopLevelFrame == NULL ? pWnd->GetTopLevelFrame() : g_pBCGPTopLevelFrame;
  return 0;
}

BCGCBPRODLLEXPORT CFrameWnd* BCGPGetParentFrame(const CWnd* pWnd)
{
  return (CFrameWnd*)0;
}


#define BCG_GET_X_LPARAM(lp)    ((int)(short)LOWORD(lp))
#define BCG_GET_Y_LPARAM(lp)    ((int)(short)HIWORD(lp))

#ifndef WS_EX_LAYOUTRTL
#define WS_EX_LAYOUTRTL         0x00400000L // Right to left mirroring
#endif

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED           0x00080000
#endif

#ifndef LWA_COLORKEY
#define LWA_COLORKEY            0x00000001
#endif

#ifndef LWA_ALPHA
#define LWA_ALPHA               0x00000002
#endif

#ifndef TTM_ADJUSTRECT
#define TTM_ADJUSTRECT          (WM_USER + 31)
#endif

#ifndef IS_INTRESOURCE
#define IS_INTRESOURCE(_r) ((((DWORD)(_r)) >> 16) == 0)
#endif

#if _MSC_VER < 1300

//------------------------------
// Windows 64 bit compatibility:
//------------------------------

#ifndef GetClassLongPtr
#define GetClassLongPtr   GetClassLong
#endif

#ifndef SetClassLongPtr
#define SetClassLongPtr   SetClassLong
#endif

#ifndef SetWindowLongPtr
#define SetWindowLongPtr  SetWindowLong
#endif

#ifndef GetWindowLongPtr
#define GetWindowLongPtr  GetWindowLong
#endif

#define DWORD_PTR     DWORD
#define INT_PTR       int
#define UINT_PTR      UINT
#define LONG_PTR      LONG

#ifndef GWLP_WNDPROC
#define GWLP_WNDPROC    GWL_WNDPROC
#endif

#ifndef GCLP_HICON
#define GCLP_HICON      GCL_HICON
#endif

#ifndef GCLP_HICONSM
#define GCLP_HICONSM    GCL_HICONSM
#endif

#ifndef GCLP_HBRBACKGROUND
#define GCLP_HBRBACKGROUND  GCL_HBRBACKGROUND
#endif

#endif // _MSC_VER

#if _MSC_VER >= 1400
#define BCGNcHitTestType  LRESULT
#else
#define BCGNcHitTestType  UINT
#endif

// Windows Vista Notifications:
#ifndef WM_DWMCOMPOSITIONCHANGED
#define WM_DWMCOMPOSITIONCHANGED        0x031E
#endif

#ifndef WM_DWMNCRENDERINGCHANGED
#define WM_DWMNCRENDERINGCHANGED        0x031F
#endif

#ifndef WM_DWMCOLORIZATIONCOLORCHANGED
#define WM_DWMCOLORIZATIONCOLORCHANGED  0x0320
#endif

#ifndef WM_DWMWINDOWMAXIMIZEDCHANGE
#define WM_DWMWINDOWMAXIMIZEDCHANGE     0x0321
#endif

// Smart docking theme:

enum BCGP_SMARTDOCK_THEME {
  BCGP_SDT_DEFAULT = 0,   // Visual Manager-specific theme
  BCGP_SDT_VS2005 = 1,    // VS 2005-style
  BCGP_SDT_VS2008 = 2     // VS 2008-style
};

//---------------------------------------------------
// Uncomment some of these definitions to exlude
// non-required features and reduce the library size:
//---------------------------------------------------

//#define BCGP_EXCLUDE_GRID_CTRL
//#define BCGP_EXCLUDE_GANTT
//#define BCGP_EXCLUDE_PLANNER
//#define BCGP_EXCLUDE_EDIT_CTRL
//#define BCGP_EXCLUDE_PROP_LIST
//#define BCGP_EXCLUDE_POPUP_WINDOW
//#define BCGP_EXCLUDE_SHELL
//#define BCGP_EXCLUDE_TOOLBOX
//#define BCGP_EXCLUDE_HOT_SPOT_IMAGE
//#define BCGP_EXCLUDE_ANIM_CTRL
//#define BCGP_EXCLUDE_TASK_PANE
//#define BCGP_EXCLUDE_RIBBON
//#define BCGP_EXCLUDE_PNG_SUPPORT
//#define BCGP_EXCLUDE_GDI_PLUS // For VS.NET or higher only



#if (!defined _BCGPCALENDAR_STANDALONE) && !(defined _BCGPGRID_STANDALONE) && !(defined _BCGPEDIT_STANDALONE)

// CBCGPAccessibilityData

struct CBCGPAccessibilityData {
  CBCGPAccessibilityData() {
    Clear();
  }

  void Clear() {
    m_nAccRole = 0;
    m_bAccState = 0;
    m_nAccHit = 0;
    m_rectAccLocation = CRect(0, 0, 0, 0);
    m_ptAccHit = CPoint(0, 0);

    m_strAccName.Empty();
    m_strAccValue.Empty();
    m_strDescription.Empty();
    m_strAccKeys.Empty();
    m_strAccHelp.Empty();
    m_strAccName.Empty();
    m_strAccDefAction.Empty();
  }

  CString       m_strAccName;
  CString       m_strAccValue;
  UINT        m_nAccHit;
  CString       m_strDescription;
  int         m_nAccRole;
  UINT        m_bAccState;
  CString       m_strAccKeys;
  CString       m_strAccHelp;
  CString       m_strAccDefAction;
  CRect       m_rectAccLocation;
  CPoint        m_ptAccHit;
};


#ifndef WM_GETOBJECT
#define WM_GETOBJECT      0x003D
#endif

//BCGCBPRODLLEXPORT extern UINT BCGM_ACCGETOBGECT;
//BCGCBPRODLLEXPORT extern UINT BCGM_ACCHITTEST;
//BCGCBPRODLLEXPORT extern UINT BCGM_ACCGETSTATE;

#ifndef OBJID_CLIENT
#define OBJID_CLIENT          0xFFFFFFFC
#endif

#ifndef OBJID_WINDOW
#define OBJID_WINDOW          0x00000000
#endif

#ifndef CHILDID_SELF
#define CHILDID_SELF          0
#endif

#ifndef EVENT_SYSTEM_MENUEND
#define EVENT_SYSTEM_MENUEND            0x0005
#endif

#ifndef EVENT_SYSTEM_MENUPOPUPSTART
#define EVENT_SYSTEM_MENUPOPUPSTART     0x0006
#endif

#ifndef EVENT_SYSTEM_MENUPOPUPEND
#define EVENT_SYSTEM_MENUPOPUPEND       0x0007
#endif

#ifndef EVENT_OBJECT_FOCUS
#define EVENT_OBJECT_FOCUS              0x8005
#endif

#ifndef EVENT_SYSTEM_MENUSTART
#define EVENT_SYSTEM_MENUSTART      0x0004
#endif

#ifndef STATE_SYSTEM_HASPOPUP
#define STATE_SYSTEM_HASPOPUP         0x40000000
#endif

#ifndef ROLE_SYSTEM_SPLITBUTTON
#define ROLE_SYSTEM_SPLITBUTTON         0x3e
#endif

#ifndef STATE_SYSTEM_NORMAL
#define STATE_SYSTEM_NORMAL       0x0000
#endif


/////////////////////////////////////////////////////////////////////////////
// Auxiliary System/Screen metrics

typedef enum BCGP_DOCK_TYPE {
  BCGP_DT_UNDEFINED = 0,    // inherit from application
  BCGP_DT_IMMEDIATE = 1,    // control bar torn off immediately and follows the mouse
  BCGP_DT_STANDARD  = 2,    // user drags a frame
  BCGP_DT_SMART = 0x80    // smart docking style
};

// autohide sliding modes
static const UINT BCGP_AHSM_MOVE    = 1;
static const UINT BCGP_AHSM_STRETCH   = 2;

#define BCGP_AUTOHIDE_LEFT    0x0001
#define BCGP_AUTOHIDE_RIGHT   0x0002
#define BCGP_AUTOHIDE_TOP   0x0004
#define BCGP_AUTOHIDE_BOTTOM  0x0008

typedef BOOL (__stdcall* NOTIFYWINEVENT)(DWORD event, HWND hwnd, LONG idObject, LONG idChild);
#endif

#ifndef _UXTHEME_H_

// From uxtheme.h:
typedef HANDLE HTHEME;          // handle to a section of theme data for class
#endif // THEMEAPI

typedef HANDLE BCGPHPAINTBUFFER;  // handle to a buffered paint context

typedef BOOL (__stdcall* SETLAYEATTRIB)(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
typedef BOOL (__stdcall* UPDATELAYEREDWINDOW)(HWND hwnd, CDC* hdcDst, IPOINT* pptDst, ISIZE* psize,
    CDC* hdcSrc, IPOINT* pptSrc, COLORREF crKey,
    BLENDFUNCTION* pblend, DWORD dwFlags);
typedef HRESULT(__stdcall* BCGP_DRAWTHEMEPARENTBACKGROUND)(HWND hWnd, CDC* hdc, const IRECT* pRec);


// BCGP_BP_BUFFERFORMAT
typedef enum _BCGP_BP_BUFFERFORMAT {
  BCGP_BPBF_COMPATIBLEBITMAP,    // Compatible bitmap
  BCGP_BPBF_DIB,                 // Device-independent bitmap
  BCGP_BPBF_TOPDOWNDIB,          // Top-down device-independent bitmap
  BCGP_BPBF_TOPDOWNMONODIB       // Top-down monochrome device-independent bitmap
} BCGP_BP_BUFFERFORMAT;

#define BCGP_BP_BUFFERFORMATBPBF_COMPOSITED BCGP_BP_BUFFERFORMATBPBF_DIB

// BCGP_BP_PAINTPARAMS
typedef struct _BCGP_BP_PAINTPARAMS {
  DWORD                       cbSize;
  DWORD                       dwFlags; // BPPF_ flags
  const IRECT*                 prcExclude;
  const BLENDFUNCTION*        pBlendFunction;
} BCGP_BP_PAINTPARAMS;

typedef BCGPHPAINTBUFFER(__stdcall* BCGP_BEGINBUFFEREDPAINT)(CDC* hdcTarget, const IRECT* rcTarget,
    BCGP_BP_BUFFERFORMAT dwFormat,
    BCGP_BP_PAINTPARAMS* pPaintParams,
    CDC* *phdc);

typedef HRESULT(__stdcall* BCGP_BUFFEREDPAINTSETALPHA)(BCGPHPAINTBUFFER hBufferedPaint, const IRECT* prc, BYTE alpha);


typedef HRESULT(__stdcall* BCGP_ENDBUFFEREDPAINT)(BCGPHPAINTBUFFER hBufferedPaint, BOOL fUpdateTarget);

typedef struct _BCGPMARGINS {
  int cxLeftWidth;
  int cxRightWidth;
  int cyTopHeight;
  int cyBottomHeight;
} BCGPMARGINS;

typedef HRESULT(__stdcall* BCGP_DWMEXTENDFRAMEINTOCLIENTAREA)(HWND hWnd, const BCGPMARGINS* pMargins);
typedef HRESULT(__stdcall* BCGP_DWMDEFWINDOWPROC)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* plResult);
typedef HRESULT(__stdcall* BCGP_DWMISCOMPOSITIONENABLED)(BOOL* pfEnabled);

typedef int (WINAPI* BCGPDTT_CALLBACK_PROC)
(
    CDC* hdc,
    LPWSTR pszText,
    int cchText,
    IRECT* prc,
    UINT dwFlags,
    LPARAM lParam);

typedef struct _BCGPDTTOPTS {
  DWORD dwSize;
  DWORD dwFlags;
  COLORREF crText;
  COLORREF crBorder;
  COLORREF crShadow;
  int iTextShadowType;
  IPOINT ptShadowOffset;
  int iBorderSize;
  int iFontPropId;
  int iColorPropId;
  int iStateId;
  BOOL fApplyOverlay;
  int iGlowSize;
  BCGPDTT_CALLBACK_PROC pfnDrawTextCallback;
  LPARAM lParam;
} BCGPDTTOPTS;

typedef HRESULT(__stdcall* BCGP_DRAWTHEMETEXTEX)(HTHEME hTheme, CDC* hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwFlags, IRECT* pRect, const BCGPDTTOPTS* pOptions);
typedef HRESULT(__stdcall* BCGP_DRAWTHEMEICON)(HTHEME hTheme, CDC* hdc, int iPartId, int iStateId, const IRECT* pRect, HIMAGELIST himl, int iImageIndex);

struct BCGPGLOBAL_DATA {
  sys_t sys[1];
  friend class CBCGPMemDC;

  int m_nBitsPerPixel;
  BOOL  m_bIsDrawOnGlass;
  BOOL  m_bUseSystemFont; // Use system font for menu/toolbar/ribbons

  // solid brushes with convenient gray colors and system colors
  COLOR hbrBtnHilite, hbrBtnShadow;

  COLOR hbrWindow;

  // color values of system colors used for CToolBar
  COLORREF clrBtnFace, clrBtnShadow, clrBtnHilite;
  COLORREF clrBtnText, clrWindowFrame;
  COLORREF clrBtnDkShadow, clrBtnLight;
  COLORREF clrGrayedText;
  COLORREF clrHilite;
  COLORREF clrTextHilite;
  COLORREF clrHotText;
  COLORREF clrHotLinkText;

  COLORREF clrBarWindow;
  COLORREF clrBarFace;
  COLORREF clrBarShadow, clrBarHilite;
  COLORREF clrBarDkShadow, clrBarLight;
  COLORREF clrBarText;

  COLORREF clrWindow;
  COLORREF clrWindowText;

  COLORREF clrCaptionText;

  COLORREF clrMenuText;

  COLORREF clrActiveCaption;
  COLORREF clrInactiveCaption;

  COLORREF clrActiveCaptionGradient;
  COLORREF clrInactiveCaptionGradient;

  COLORREF clrInactiveCaptionText;

  COLORREF clrActiveBorder;
  COLORREF clrInactiveBorder;

  COLOR brBtnFace;
  COLOR brHilite;
  COLOR brLight;
  COLOR brBlack;
  COLOR brActiveCaption;
  COLOR brInactiveCaption;
  COLOR brWindow;

  COLOR brBarFace;

  COLOR penHilite;
  COLOR penBarFace;
  COLOR penBarShadow;

  // Library cursors:
  CBitmap*  m_hcurStretch;
  CBitmap*  m_hcurStretchVert;
  CBitmap*  m_hcurHand;
  CBitmap*  m_hcurSizeAll;
  CBitmap*  m_hcurMoveTab;
  CBitmap*  m_hcurNoMoveTab;
  CBitmap*  m_hcurSelectRow;

  CBitmap*  GetHandCursor();

  CBitmap*  m_hiconTool;
  CBitmap*  m_hiconLink;
  CBitmap*  m_hiconColors;

  // Shell icon sizes:
  CSize       m_sizeSmallIcon;

  // Toolbar and menu fonts:
  CFont*        fontRegular;
  CFont*        fontTooltip;
  CFont*        fontBold;
  CFont*        fontDefaultGUIBold;
  CFont*        fontUnderline;
  CFont*        fontDefaultGUIUnderline;
  CFont*        fontVert;
  CFont*        fontVertCaption;
  CFont*        fontSmall;
  CFont*        fontMarlett;  // Standard Windows menu symbols

  BOOL        bDisableAero;

  BOOL        bIsRemoteSession;
  BOOL        bIsOSAlphaBlendingSupport;

  int         m_nDragFrameThiknessFloat;
  int         m_nDragFrameThiknessDock;

  int         m_nAutoHideToolBarSpacing;
  int         m_nAutoHideToolBarMargin;

  int         m_nCoveredMainWndClientAreaPercent;

  int         m_nMaxToolTipWidth;

  BOOL        m_bIsBlackHighContrast;
  BOOL        m_bIsWhiteHighContrast;

  BOOL        m_bUseBuiltIn32BitIcons;
  BOOL        m_bUseVisualManagerInBuiltInDialogs;

  CRect       m_rectVirtual;

  BOOL        m_bMenuAnimation;
  BOOL        m_bMenuFadeEffect;

  int         m_nShellAutohideBars;

  BOOL        m_bIsRTL;

  BOOL        m_bEnableAccessibility;
  BOOL        m_bInSettingsChange;

  BOOL        m_bUnderlineKeyboardShortcuts;
  BOOL        m_bSysUnderlineKeyboardShortcuts;

  // Implementation
  BCGPGLOBAL_DATA();
  ~BCGPGLOBAL_DATA();

  void UpdateSysColors();
  void UpdateFonts();
  void OnSettingChange();

  CBitmap* LoadCursor(LPCTSTR lpCursorName) {
    return (CBitmap*)::LoadCursor(sys->hInstance, lpCursorName);
  }
  BOOL SetMenuFont(LPLOGFONT lpLogFont, BOOL bHorz);

  int GetTextHeight(BOOL bHorz = TRUE) {
    return bHorz ? m_nTextHeightHorz : m_nTextHeightVert;
  }

  int GetTextWidth(BOOL bHorz = TRUE) {
    return bHorz ? m_nTextWidthHorz : m_nTextWidthVert;
  }

  int GetTextMargins(BOOL bHorz = TRUE) {
    return bHorz ? m_nTextMarginsHorz : m_nTextMarginsVert;
  }

  double GetRibbonImageScale() {
    return m_bIsRibbonImageScale ? m_dblRibbonImageScale : 1.;
  }

  void EnableRibbonImageScale(BOOL bEnable = TRUE) {
    m_bIsRibbonImageScale = bEnable;
  }

  BOOL IsRibbonImageScaleEnabled() {
    return m_bIsRibbonImageScale;
  }

  BOOL IsWinXPDrawParentBackground() const {
    return m_pfDrawThemeBackground != NULL;
  }

  BOOL DrawParentBackground(CDC* pDC, IRECT* lpRect = NULL);
  void CleanUp();

  COLORREF GetColor(int nColor);

  BOOL SetLayeredAttrib(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
  BOOL UpdateLayeredWindow(HWND hwnd, CDC* hdcDst, IPOINT* pptDst, ISIZE* psize, CDC* hdcSrc,
      IPOINT* pptSrc, COLORREF crKey, BLENDFUNCTION* pblend, DWORD dwFlags);
  BOOL IsWindowsLayerSupportAvailable() const {
    return m_pfSetLayeredWindowAttributes != NULL;
  }

  BOOL Is32BitIcons() const {
    return m_bUseBuiltIn32BitIcons && !m_bIsBlackHighContrast && !m_bIsWhiteHighContrast;
  }

  BOOL IsHighContastMode() const {
    return m_bIsWhiteHighContrast || m_bIsBlackHighContrast;
  }

#if (!defined _BCGPCALENDAR_STANDALONE) && !(defined _BCGPGRID_STANDALONE) && !(defined _BCGPEDIT_STANDALONE)
  BOOL IsAccessibilitySupport() const {
    return m_bEnableAccessibility;
  }

  void EnableAccessibilitySupport(BOOL bEnable = TRUE);
  BOOL NotifyWinEvent(DWORD event, HWND hwnd, LONG idObject, LONG idChild);
#endif

  CString RegisterWindowClass(LPCTSTR lpszClassNamePrefix);
  BOOL ExcludeTag(CString& strBuffer, LPCTSTR lpszTag, CString& strTag, BOOL bIsCharsList = FALSE);

  BOOL DwmExtendFrameIntoClientArea(HWND hWnd, BCGPMARGINS* pMargins);
  LRESULT DwmDefWindowProc(HWND hWnd, UINT message, WPARAM wp, LPARAM lp);
  BOOL DwmIsCompositionEnabled();

  BOOL DrawIconOnGlass(HTHEME hTheme, CDC* pDC, CBitmap* hIcon, CRect rect);

  BOOL Resume();

  BOOL GetNonClientMetrics(NONCLIENTMETRICS& ncm);
  UINT GetRebarBandInfoSize();
  DWORD GetComCtlVersion();

  BOOL SetDPIAware();

protected:

  void UpdateTextMetrics();
  BOOL CreateDitherBitmap(CBitmap* hbm);

  int m_nTextHeightHorz;
  int m_nTextHeightVert;

  int m_nTextMarginsHorz;
  int m_nTextMarginsVert;

  int m_nTextWidthHorz;
  int m_nTextWidthVert;

  double  m_dblRibbonImageScale;
  BOOL  m_bIsRibbonImageScale;

  //HINSTANCE m_hinstUXThemeDLL;
  HINSTANCE m_hinstUser32;
  HINSTANCE m_hinstDwmapiDLL;

  SETLAYEATTRIB m_pfSetLayeredWindowAttributes;
  UPDATELAYEREDWINDOW m_pfUpdateLayeredWindow;

#if (!defined _BCGPCALENDAR_STANDALONE) && !(defined _BCGPGRID_STANDALONE) && !(defined _BCGPEDIT_STANDALONE)
  NOTIFYWINEVENT      m_pfNotifyWinEvent;
#endif

  BCGP_DRAWTHEMEPARENTBACKGROUND    m_pfDrawThemeBackground;
  BCGP_DRAWTHEMETEXTEX        m_pfDrawThemeTextEx;
  BCGP_DRAWTHEMEICON          m_pfDrawThemeIcon;
  BCGP_BEGINBUFFEREDPAINT       m_pfBeginBufferedPaint;
  BCGP_BUFFEREDPAINTSETALPHA      m_pfBufferedPaintSetAlpha;
  BCGP_ENDBUFFEREDPAINT       m_pfEndBufferedPaint;
  BCGP_DWMEXTENDFRAMEINTOCLIENTAREA m_pfDwmExtendFrameIntoClientArea;
  BCGP_DWMDEFWINDOWPROC       m_pfDwmDefWindowProc;
  BCGP_DWMISCOMPOSITIONENABLED    m_pfDwmIsCompositionEnabled;

  DWORD               m_dwComCtlVersion;
};

#define IMAGE_MARGIN  4
#define TEXT_MARGIN 3
#define MENU_IMAGE_MARGIN 2

// MFC Control bar compatibility
#define CX_BORDER   1
#define CY_BORDER   1

#define CX_GRIPPER  3
#define CY_GRIPPER  3
#define CX_BORDER_GRIPPER 2
#define CY_BORDER_GRIPPER 2
//****
BOOL PreMultiplyAlpha(img_t* hbmp, BOOL bAutoCheckPremlt)
{
  if (hbmp->c != 4) {
    return FALSE;
  }

  int i = 0;

  RGBQUAD* pBits = (RGBQUAD*)hbmp->tt.data;
  const int length = hbmp->w * hbmp->h;

  if (bAutoCheckPremlt) {
    BOOL bPremultiply = FALSE;

    RGBQUAD* pBit = pBits;

    for (i = 0; i < length; i++) {
      if (pBit->rgbRed   > pBit->rgbReserved ||
          pBit->rgbGreen > pBit->rgbReserved ||
          pBit->rgbBlue  > pBit->rgbReserved) {
        bPremultiply = TRUE;
        break;
      }

      pBit++;
    }

    if (!bPremultiply) {
      return TRUE;
    }
  }

  //----------------------------------------------------------------
  // Premultiply the R,G and B values with the Alpha channel values:
  //----------------------------------------------------------------
  RGBQUAD* pBit = pBits;

  for (i = 0; i < length; i++) {
    pBit->rgbRed   = (BYTE)(pBit->rgbRed   * pBit->rgbReserved / 255);
    pBit->rgbGreen = (BYTE)(pBit->rgbGreen * pBit->rgbReserved / 255);
    pBit->rgbBlue  = (BYTE)(pBit->rgbBlue  * pBit->rgbReserved / 255);
    pBit++;
  }

  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////



#ifndef SPI_GETMENUANIMATION
#define SPI_GETMENUANIMATION  0x1002
#endif

#ifndef SPI_GETMENUFADE
#define SPI_GETMENUFADE     0x1012
#endif

#ifndef SPI_GETMENUUNDERLINES
#define SPI_GETKEYBOARDCUES   0x100A
#define SPI_GETMENUUNDERLINES SPI_GETKEYBOARDCUES
#endif

static const CString strOfficeFontName    = _T("Tahoma");
static const CString strOffice2007FontName  = _T("Segoe UI");
static const CString strDefaultFontName   = _T("MS Sans Serif");
static const CString strVertFontName    = _T("Arial");
static const CString strMarlettFontName   = _T("Marlett");

static int CALLBACK FontFamalyProcFonts(const LOGFONT FAR* lplf,
    const TEXTMETRIC FAR* /*lptm*/,
    ULONG /*ulFontType*/,
    LPARAM lParam)
{
  ASSERT(lplf != NULL);
  ASSERT(lParam != NULL);

  CString strFont = lplf->lfFaceName;
  return strFont.CollateNoCase((LPCTSTR) lParam) == 0 ? 0 : 1;
}
/////////////////////////////////////////////////////////////////////////////
// Cached system metrics, etc

//BCGPGLOBAL_DATA globalData;

// Initialization code
BCGPGLOBAL_DATA::BCGPGLOBAL_DATA()
{
  sys_init(sys);
  m_nBitsPerPixel = 32;
  bIsOSAlphaBlendingSupport = TRUE;

  bIsRemoteSession = FALSE;

  bDisableAero = FALSE;

  m_bIsRibbonImageScale = TRUE;

  //---------------------------------------------------------
  // Cached system values (updated in CWnd::OnSysColorChange)
  //---------------------------------------------------------
  hbrBtnShadow = NULL;
  hbrBtnHilite = NULL;
  hbrWindow = NULL;

  UpdateSysColors();

  {
    m_pfDrawThemeBackground = NULL;
    m_pfDrawThemeTextEx = NULL;
    m_pfDrawThemeIcon = NULL;
    m_pfBeginBufferedPaint = NULL;
    m_pfBufferedPaintSetAlpha = NULL;
    m_pfEndBufferedPaint = NULL;
  }

  m_hinstUser32 = LoadLibrary(_T("USER32.DLL"));

  if (!bIsOSAlphaBlendingSupport) {
    m_pfSetLayeredWindowAttributes = NULL;
    m_pfUpdateLayeredWindow = NULL;
  }
  else {
    if (m_hinstUser32 == NULL) {
      ASSERT(FALSE);
    }
    else {
      m_pfSetLayeredWindowAttributes =
          (SETLAYEATTRIB)::GetProcAddress(
              m_hinstUser32, "SetLayeredWindowAttributes");
      m_pfUpdateLayeredWindow =
          (UPDATELAYEREDWINDOW)::GetProcAddress(
              m_hinstUser32, "UpdateLayeredWindow");
    }
  }

  m_hinstDwmapiDLL = LoadLibrary(_T("dwmapi.dll"));

  if (m_hinstDwmapiDLL != NULL) {
    m_pfDwmExtendFrameIntoClientArea =
        (BCGP_DWMEXTENDFRAMEINTOCLIENTAREA)::GetProcAddress(m_hinstDwmapiDLL, "DwmExtendFrameIntoClientArea");

    m_pfDwmDefWindowProc =
        (BCGP_DWMDEFWINDOWPROC) ::GetProcAddress(m_hinstDwmapiDLL, "DwmDefWindowProc");

    m_pfDwmIsCompositionEnabled =
        (BCGP_DWMISCOMPOSITIONENABLED)::GetProcAddress(m_hinstDwmapiDLL, "DwmIsCompositionEnabled");
  }
  else {
    m_pfDwmExtendFrameIntoClientArea = NULL;
    m_pfDwmDefWindowProc = NULL;
    m_pfDwmIsCompositionEnabled = NULL;
  }

  m_hcurStretch = NULL;
  m_hcurStretchVert = NULL;
  m_hcurHand = NULL;
  m_hcurSizeAll = NULL;
  m_hiconTool = NULL;
  m_hiconLink = NULL;
  m_hiconColors = NULL;
  m_hcurMoveTab = NULL;
  m_hcurNoMoveTab = NULL;
  m_hcurSelectRow = NULL;

  m_bUseSystemFont = FALSE;
  m_bInSettingsChange = FALSE;

  UpdateFonts();
  OnSettingChange();

  m_bIsRTL = FALSE;

  //------------------
  // Small icon sizes:
  //------------------
  m_nDragFrameThiknessFloat = 4;  // pixels
  m_nDragFrameThiknessDock = 3;   // pixels

  m_nAutoHideToolBarSpacing = 14; // pixels
  m_nAutoHideToolBarMargin  = 4;  // pixels

  m_nCoveredMainWndClientAreaPercent = 50; // percents

  m_nMaxToolTipWidth = -1;
  m_bIsBlackHighContrast = FALSE;
  m_bIsWhiteHighContrast = FALSE;

  m_bUseBuiltIn32BitIcons = TRUE;
  m_bUseVisualManagerInBuiltInDialogs = FALSE;

#if (!defined _BCGPCALENDAR_STANDALONE) && !(defined _BCGPGRID_STANDALONE) && !(defined _BCGPEDIT_STANDALONE)
  m_bEnableAccessibility = FALSE;
  m_pfNotifyWinEvent = NULL;
  EnableAccessibilitySupport();
#endif

  m_dwComCtlVersion = (DWORD) - 1;
}
//****
BCGPGLOBAL_DATA::~BCGPGLOBAL_DATA()
{
  CleanUp();
}
//****
void BCGPGLOBAL_DATA::UpdateFonts()
{
  m_dblRibbonImageScale = 1;//dc.GetDeviceCaps(LOGPIXELSX) / 96.0f;

  if (m_dblRibbonImageScale > 1. && m_dblRibbonImageScale < 1.1) {
    m_dblRibbonImageScale = 1.;
  }

  //-------------------------------------
  //  Check if we should use system font:
  //-------------------------------------
  const char* strRegular = "ËÎÌו";
  int size = 12;
  fontRegular = (CFont*)sys_font_load_win32_impl(sys, strRegular, size, 0);

  //-------------------
  // Create small font:
  //-------------------
  fontSmall = (CFont*)sys_font_load_win32_impl(sys, strRegular, size, 0);

  //---------------------
  // Create tooltip font:
  //---------------------
  fontTooltip = (CFont*)sys_font_load_win32_impl(sys, strRegular, size, 0);

  //-------------------------
  // Create "underline" font:
  //-------------------------
  fontUnderline = (CFont*)sys_font_load_win32_impl(sys, strRegular, size, FONT_UNDERLINE);

  //------------------
  // Create bold font:
  //------------------
  fontBold = (CFont*)sys_font_load_win32_impl(sys, strRegular, size, FONT_BOLD);

  //---------------------
  // Create Marlett font:
  //---------------------
  fontMarlett = (CFont*)sys_font_load_win32_impl(sys, strMarlettFontName, size, 0);

  //----------------------
  // Create vertical font:
  //----------------------
  fontVert = (CFont*)sys_font_load_win32_impl(sys, strRegular, size, 0);
  fontVertCaption = (CFont*)sys_font_load_win32_impl(sys, strRegular, size, 0);

  //----------------------------------------
  // Create dialog underline and bold fonts:
  //----------------------------------------
  fontDefaultGUIUnderline = (CFont*)sys_font_load_win32_impl(sys, strRegular, size, FONT_UNDERLINE);
  fontDefaultGUIBold = (CFont*)sys_font_load_win32_impl(sys, strRegular, size, FONT_BOLD);

  UpdateTextMetrics();

}
//****
static BOOL CALLBACK InfoEnumProc(HMONITOR hMonitor, HDC /*hdcMonitor*/, RECT* /*lprcMonitor*/, LPARAM dwData)
{
  CRect* pRect = (CRect*) dwData;

  MONITORINFO mi;
  mi.cbSize = sizeof(MONITORINFO);

  if (GetMonitorInfo(hMonitor, &mi)) {
    CRect rectMon = *(IRECT*)&mi.rcWork;

    pRect->l = min(pRect->l, rectMon.l);
    pRect->r = max(pRect->r, rectMon.r);
    pRect->t = min(pRect->t, rectMon.t);
    pRect->b = max(pRect->b, rectMon.b);
  }

  return TRUE;
}
//****
void BCGPGLOBAL_DATA::OnSettingChange()
{
  m_bInSettingsChange = TRUE;

  m_sizeSmallIcon.w = ::GetSystemMetrics(SM_CXSMICON);
  m_sizeSmallIcon.h = ::GetSystemMetrics(SM_CYSMICON);

  m_rectVirtual.SetRectEmpty();

  if (!EnumDisplayMonitors(NULL, NULL, InfoEnumProc, (LPARAM) &m_rectVirtual)) {
    ::SystemParametersInfo(SPI_GETWORKAREA, 0, &m_rectVirtual, 0);
  }

  //--------------------------------
  // Get system menu animation type:
  //--------------------------------
  m_bMenuAnimation = FALSE;
  m_bMenuFadeEffect = FALSE;

  m_nShellAutohideBars = 0;

  m_bSysUnderlineKeyboardShortcuts = TRUE;

  m_bUnderlineKeyboardShortcuts = m_bSysUnderlineKeyboardShortcuts;

  m_bInSettingsChange = FALSE;
}
//****
void BCGPGLOBAL_DATA::UpdateSysColors()
{
  m_bIsBlackHighContrast =
      _GetSysColor(CLR_3DLIGHT) == _RGB(255, 255, 255) &&
      _GetSysColor(CLR_3DFACE) == _RGB(0, 0, 0);

  m_bIsWhiteHighContrast =
      _GetSysColor(CLR_3DDKSHADOW) == _RGB(0, 0, 0) &&
      _GetSysColor(CLR_3DFACE) == _RGB(255, 255, 255);

  clrBarFace = clrBtnFace = _GetSysColor(CLR_BTNFACE);
  clrBarShadow = clrBtnShadow = _GetSysColor(CLR_BTNSHADOW);
  clrBarDkShadow = clrBtnDkShadow = _GetSysColor(CLR_3DDKSHADOW);
  clrBarLight = clrBtnLight = _GetSysColor(CLR_3DLIGHT);
  clrBarHilite = clrBtnHilite = _GetSysColor(CLR_BTNHIGHLIGHT);
  clrBarText = clrBtnText = _GetSysColor(CLR_BTNTEXT);
  clrGrayedText = _GetSysColor(CLR_GRAYTEXT);
  clrWindowFrame = _GetSysColor(CLR_WINDOWFRAME);

  clrHilite = _GetSysColor(CLR_HIGHLIGHT);
  clrTextHilite = _GetSysColor(CLR_HIGHLIGHTTEXT);

  clrBarWindow = clrWindow = _GetSysColor(CLR_WINDOW);
  clrWindowText = _GetSysColor(CLR_WINDOWTEXT);

  clrCaptionText = _GetSysColor(CLR_CAPTIONTEXT);
  clrMenuText = _GetSysColor(CLR_MENUTEXT);

  clrActiveCaption = _GetSysColor(CLR_ACTIVECAPTION);
  clrInactiveCaption = _GetSysColor(CLR_INACTIVECAPTION);

  clrActiveCaptionGradient = clrActiveCaption;
  clrInactiveCaptionGradient = clrInactiveCaption;

  clrActiveBorder = _GetSysColor(CLR_ACTIVEBORDER);
  clrInactiveBorder = _GetSysColor(CLR_INACTIVEBORDER);

  clrInactiveCaptionText = _GetSysColor(CLR_INACTIVECAPTIONTEXT);

  if (m_bIsBlackHighContrast) {
    clrHotText = clrWindowText;
    clrHotLinkText = clrWindowText;
  }
  else {
    if (1) {
      clrHotText = GetSysColor(COLOR_HOTLIGHT);
      clrHotLinkText = _RGB(0, 0, 255);   // Light blue
    }
    else {
      clrHotText = _RGB(0, 0, 255);     // Light blue
      clrHotLinkText = _RGB(255, 0, 255);   // Violet
    }
  }

  hbrBtnShadow = GetSysColorBrush(COLOR_BTNSHADOW);
  ASSERT(hbrBtnShadow != NULL);

  hbrBtnHilite = GetSysColorBrush(COLOR_BTNHIGHLIGHT);
  ASSERT(hbrBtnHilite != NULL);

  hbrWindow = GetSysColorBrush(COLOR_WINDOW);
  ASSERT(hbrWindow != NULL);

  brBtnFace = (clrBtnFace);

  brBarFace = (clrBarFace);

  brActiveCaption = (clrActiveCaption);

  brInactiveCaption = (clrInactiveCaption);

  brHilite = (clrHilite);

  brBlack = (clrBtnDkShadow);

  brWindow = (clrWindow);

  penHilite = (clrHilite);

  penBarFace = (clrBarFace);

  penBarShadow = (clrBarShadow);

  COLORREF clrLight = _RGB(
      GetRV(clrBtnFace) + ((GetRV(clrBtnHilite) - GetRV(clrBtnFace)) / 2),
      GetGV(clrBtnFace) + ((GetGV(clrBtnHilite) - GetGV(clrBtnFace)) / 2),
      GetBV(clrBtnFace) + ((GetBV(clrBtnHilite) - GetBV(clrBtnFace)) / 2)
      );

  brLight = (clrLight);

}

//****
void BCGPGLOBAL_DATA::UpdateTextMetrics()
{
  CFont* fo;
  fo = fontRegular;

  m_nTextMarginsHorz = fo->h < 15 ? 2 : 5;
  m_nTextHeightHorz = fo->h + m_nTextMarginsHorz;
  m_nTextWidthHorz = fo->w + m_nTextMarginsHorz;

  fo = fontVert;

  m_nTextMarginsVert = fo->h < 15 ? 2 : 5;
  m_nTextHeightVert = fo->h + m_nTextMarginsVert;
  m_nTextWidthVert = fo->w + m_nTextMarginsVert;

}
//****
BOOL BCGPGLOBAL_DATA::CreateDitherBitmap(CBitmap* hbm)
{
  int i;
  long patGray[8];
  COLORREF bmiColors[2];
  bmiColors[0] = clrBtnFace;
  bmiColors[1] = _GetSysColor(CLR_BTNHIGHLIGHT);

  // initialize the brushes
  for (i = 0; i < 8; i++) {
    patGray[i] = (i & 1) ? 0xAAAA5555L : 0x5555AAAAL;
  }

  return hbm->CreateBitmap(8, 8, 1, 1, patGray, bmiColors);
}
//****
void BCGPGLOBAL_DATA::CleanUp()
{
#if (!defined _BCGPCALENDAR_STANDALONE) && !(defined _BCGPGRID_STANDALONE) && !(defined _BCGPEDIT_STANDALONE)
  m_pfSetLayeredWindowAttributes = NULL;
  m_pfUpdateLayeredWindow = NULL;
  m_pfNotifyWinEvent = NULL;
#endif
}
//****
BOOL BCGPGLOBAL_DATA::DrawParentBackground(CDC* pDC, IRECT* rectClip)
{
  return 0;
}

COLORREF BCGPGLOBAL_DATA::GetColor(int nColor)
{
  switch (nColor) {
  case COLOR_BTNFACE:
    return clrBtnFace;

  case COLOR_BTNSHADOW:
    return clrBtnShadow;

  case COLOR_3DDKSHADOW:
    return clrBtnDkShadow;

  case COLOR_3DLIGHT:
    return clrBtnLight;

  case COLOR_BTNHIGHLIGHT:
    return clrBtnHilite;

  case COLOR_BTNTEXT:
    return clrBtnText;

  case COLOR_GRAYTEXT:
    return clrGrayedText;

  case COLOR_WINDOWFRAME:
    return clrWindowFrame;

  case COLOR_HIGHLIGHT:
    return clrHilite;

  case COLOR_HIGHLIGHTTEXT:
    return clrTextHilite;

  case COLOR_WINDOW:
    return clrWindow;

  case COLOR_WINDOWTEXT:
    return clrWindowText;

  case COLOR_CAPTIONTEXT:
    return clrCaptionText;

  case COLOR_MENUTEXT:
    return clrMenuText;

  case COLOR_ACTIVECAPTION:
    return clrActiveCaption;

  case COLOR_INACTIVECAPTION:
    return clrInactiveCaption;

  case COLOR_ACTIVEBORDER:
    return clrActiveBorder;

  case COLOR_INACTIVEBORDER:
    return clrInactiveBorder;

  case COLOR_INACTIVECAPTIONTEXT:
    return clrInactiveCaptionText;
  }

  return GetSysColor(nColor);
}
//****
BOOL BCGPGLOBAL_DATA::SetLayeredAttrib(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags)
{
  if (m_pfSetLayeredWindowAttributes == NULL) {
    return FALSE;
  }

  return (*m_pfSetLayeredWindowAttributes)(hwnd, crKey, bAlpha, dwFlags);
}

BOOL BCGPGLOBAL_DATA::UpdateLayeredWindow(HWND hwnd, CDC* hdcDst, IPOINT* pptDst, ISIZE* psize, CDC* hdcSrc,
    IPOINT* pptSrc, COLORREF crKey, BLENDFUNCTION* pblend, DWORD dwFlags)
{
  if (m_pfUpdateLayeredWindow == NULL) {
    return FALSE;
  }

  return (*m_pfUpdateLayeredWindow)(hwnd, hdcDst, pptDst, psize, hdcSrc, pptSrc, crKey, pblend, dwFlags);
}

#if (!defined _BCGPCALENDAR_STANDALONE) && !(defined _BCGPGRID_STANDALONE) && !(defined _BCGPEDIT_STANDALONE)

void BCGPGLOBAL_DATA::EnableAccessibilitySupport(BOOL bEnable/* = TRUE*/)
{
  OSVERSIONINFO osvi;
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  ::GetVersionEx(&osvi);

  if ((osvi.dwPlatformId != VER_PLATFORM_WIN32_NT) || (osvi.dwMajorVersion < 5)) {
    return;
  }

  if (bEnable) {
    if (m_hinstUser32 == NULL) {
      m_pfNotifyWinEvent = NULL;
      ASSERT(FALSE);
      return;
    }

    m_pfNotifyWinEvent = (NOTIFYWINEVENT)
        GetProcAddress(m_hinstUser32, "NotifyWinEvent");
    ASSERT(m_pfNotifyWinEvent != NULL);
  }

  m_bEnableAccessibility = bEnable;
}
//****
BOOL BCGPGLOBAL_DATA::NotifyWinEvent(DWORD event, HWND hwnd, LONG idObject, LONG idChild)
{
  if (m_pfNotifyWinEvent == NULL) {
    return FALSE;
  }

  return (*m_pfNotifyWinEvent)(event, hwnd, idObject, idChild);
}

#endif

#define AfxGetInstanceHandle() sys->hInstance

CString BCGPGLOBAL_DATA::RegisterWindowClass(LPCTSTR lpszClassNamePrefix)
{
  ASSERT(lpszClassNamePrefix != NULL);

  //-----------------------------
  // Register a new window class:
  //-----------------------------
  HINSTANCE hInst = AfxGetInstanceHandle();
  UINT uiClassStyle = CS_DBLCLKS;
  CBitmap* hCursor = LoadCursor(IDC_ARROW);
  COLOR hbrBackground = COLOR_BTNFACE;

  char strClassName[256];
  _snprintf(strClassName, 256, _T("%s:%x:%x:%x:%x"), lpszClassNamePrefix, (UINT_PTR)hInst, uiClassStyle, (UINT_PTR)hCursor, (UINT_PTR)hbrBackground);
  return strClassName;
}
//****
BOOL BCGPGLOBAL_DATA::ExcludeTag(CString& strBuffer,
    LPCTSTR lpszTag,
    CString& strTag, BOOL bIsCharsList /* = FALSE*/)
{
  const int iBufLen = strBuffer.GetLength();

  CString strTagStart = _T('<');
  strTagStart += lpszTag;
  strTagStart += _T('>');

  const int iTagStartLen = strTagStart.GetLength();

  int iStart = -1;

  int iIndexStart = strBuffer.Find(strTagStart);

  if (iIndexStart < 0) {
    return FALSE;
  }

  iStart = iIndexStart + iTagStartLen;

  CString strTagEnd = _T("</");
  strTagEnd += lpszTag;
  strTagEnd += _T('>');

  const int iTagEndLen = strTagEnd.GetLength();

  int iIndexEnd =  -1;
  int nBalanse = 1;

  for (int i = iStart; i < iBufLen - iTagEndLen + 1; i ++) {
    if (strBuffer [i] != '<') {
      continue;
    }

    if (i < iBufLen - iTagStartLen &&
        _tcsncmp(strBuffer.Mid(i), strTagStart, iTagStartLen) == 0) {
      i += iTagStartLen - 1;
      nBalanse ++;
      continue;
    }

    if (_tcsncmp(strBuffer.Mid(i), strTagEnd, iTagEndLen) == 0) {
      nBalanse --;

      if (nBalanse == 0) {
        iIndexEnd = i;
        break;
      }

      i += iTagEndLen - 1;
    }
  }

  if (iIndexEnd == -1 || iStart > iIndexEnd) {
    return FALSE;
  }

  strTag = strBuffer.Mid(iStart, iIndexEnd - iStart);
  strTag.TrimLeft();
  strTag.TrimRight();

  strBuffer.Delete(iIndexStart, iIndexEnd + iTagEndLen - iIndexStart);

  if (bIsCharsList) {
    if (strTag.GetLength() > 1 && strTag [0] == _T('\"')) {
      strTag = strTag.Mid(1, strTag.GetLength() - 2);
    }

    strTag.Replace(_T("\\t"), _T("\t"));
    strTag.Replace(_T("\\n"), _T("\n"));
    strTag.Replace(_T("\\r"), _T("\r"));
    strTag.Replace(_T("\\b"), _T("\b"));
    strTag.Replace(_T("LT"), _T("<"));
    strTag.Replace(_T("GT"), _T(">"));
    strTag.Replace(_T("AMP"), _T("&"));
  }

  return TRUE;
}
//****
BOOL BCGPGLOBAL_DATA::DwmExtendFrameIntoClientArea(HWND hWnd, BCGPMARGINS* pMargins)
{
  if (m_pfDwmExtendFrameIntoClientArea == NULL) {
    return FALSE;
  }

  HRESULT hres = (*m_pfDwmExtendFrameIntoClientArea)(hWnd, pMargins);
  return hres == S_OK;
}
//****
LRESULT BCGPGLOBAL_DATA::DwmDefWindowProc(HWND hWnd, UINT message, WPARAM wp, LPARAM lp)
{
  if (m_pfDwmDefWindowProc == NULL) {
    return (LRESULT) - 1;
  }

  LRESULT lres = 0;
  (*m_pfDwmDefWindowProc)(hWnd, message, wp, lp, &lres);

  return lres;
}
//****
BOOL BCGPGLOBAL_DATA::DwmIsCompositionEnabled()
{
  if (m_pfDwmIsCompositionEnabled == NULL || bDisableAero) {
    return FALSE;
  }

  BOOL bEnabled = FALSE;

  (*m_pfDwmIsCompositionEnabled)(&bEnabled);
  return bEnabled;
}

//****
BOOL BCGPGLOBAL_DATA::DrawIconOnGlass(HTHEME hTheme, CDC* pDC, CBitmap* hIcon, CRect rect)
{
  ASSERT_VALID(pDC);

  pDC->DrawState(rect.TopLeft(), rect.Size(), hIcon, DSS_NORMAL, (CBrush*)NULL);
  return FALSE;
}
//****
CBitmap*  BCGPGLOBAL_DATA::GetHandCursor()
{
  if (m_hcurHand == NULL) {
    {
      m_hcurHand = LoadCursor(IDC_HAND);
    }
  }

  return m_hcurHand;
}
//****
BOOL BCGPGLOBAL_DATA::Resume()
{
  m_pfDrawThemeBackground = NULL;
  m_pfDrawThemeTextEx = NULL;
  m_pfBeginBufferedPaint = NULL;
  m_pfBufferedPaintSetAlpha = NULL;
  m_pfEndBufferedPaint = NULL;


  m_hinstUser32 = LoadLibrary(_T("USER32.DLL"));

  if (!bIsOSAlphaBlendingSupport) {
    m_pfSetLayeredWindowAttributes = NULL;
    m_pfUpdateLayeredWindow = NULL;
  }
  else {
    if (m_hinstUser32 == NULL) {
      ASSERT(FALSE);
      return FALSE;
    }
    else {
      m_pfSetLayeredWindowAttributes =
          (SETLAYEATTRIB)::GetProcAddress(
              m_hinstUser32, "SetLayeredWindowAttributes");
      m_pfUpdateLayeredWindow =
          (UPDATELAYEREDWINDOW)::GetProcAddress(
              m_hinstUser32, "UpdateLayeredWindow");
    }
  }

  if (m_hinstDwmapiDLL != NULL) {
    m_hinstDwmapiDLL = LoadLibrary(_T("dwmapi.dll"));

    m_pfDwmExtendFrameIntoClientArea =
        (BCGP_DWMEXTENDFRAMEINTOCLIENTAREA)::GetProcAddress(m_hinstDwmapiDLL, "DwmExtendFrameIntoClientArea");

    m_pfDwmDefWindowProc =
        (BCGP_DWMDEFWINDOWPROC) ::GetProcAddress(m_hinstDwmapiDLL, "DwmDefWindowProc");

    m_pfDwmIsCompositionEnabled =
        (BCGP_DWMISCOMPOSITIONENABLED)::GetProcAddress(m_hinstDwmapiDLL, "DwmIsCompositionEnabled");
  }

#if (!defined _BCGPCALENDAR_STANDALONE) && !(defined _BCGPGRID_STANDALONE) && !(defined _BCGPEDIT_STANDALONE)

  if (m_bEnableAccessibility) {
    m_pfNotifyWinEvent = NULL;
    EnableAccessibilitySupport();
  }

#endif
  return TRUE;
}

struct BCGP_DLLVERSIONINFO {
  DWORD cbSize;
  DWORD dwMajorVersion;                   // Major version
  DWORD dwMinorVersion;                   // Minor version
  DWORD dwBuildNumber;                    // Build number
  DWORD dwPlatformID;                     // DLLVER_PLATFORM_*
};

typedef HRESULT(CALLBACK* BCGP_DLLGETVERSIONPROC)(BCGP_DLLVERSIONINFO*);

DWORD BCGPGLOBAL_DATA::GetComCtlVersion()
{
  if (m_dwComCtlVersion != -1) {
    return m_dwComCtlVersion;
  }

  return m_dwComCtlVersion;
}

BOOL BCGPGLOBAL_DATA::GetNonClientMetrics(NONCLIENTMETRICS& ncm)
{
  struct BCGP_OLDNONCLIENTMETRICS {
    UINT    cbSize;
    int     iBorderWidth;
    int     iScrollWidth;
    int     iScrollHeight;
    int     iCaptionWidth;
    int     iCaptionHeight;
    LOGFONT lfCaptionFont;
    int     iSmCaptionWidth;
    int     iSmCaptionHeight;
    LOGFONT lfSmCaptionFont;
    int     iMenuWidth;
    int     iMenuHeight;
    LOGFONT lfMenuFont;
    LOGFONT lfStatusFont;
    LOGFONT lfMessageFont;
  };

  const UINT cbProperSize = (GetComCtlVersion() < MAKELONG(1, 6))
      ? sizeof(BCGP_OLDNONCLIENTMETRICS) : sizeof(NONCLIENTMETRICS);

  ZeroMemory(&ncm, sizeof(NONCLIENTMETRICS));
  ncm.cbSize = cbProperSize;

  return ::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, cbProperSize, &ncm, 0);
}

UINT BCGPGLOBAL_DATA::GetRebarBandInfoSize()
{
  struct BCGP_OLDREBARBANDINFO {
    UINT cbSize;
    UINT fMask;
    UINT fStyle;
    COLORREF clrFore;
    COLORREF clrBack;
    LPTSTR lpText;
    UINT cch;
    int iImage;
    HWND hwndChild;
    UINT cxMinChild;
    UINT cyMinChild;
    UINT cx;
    CBitmap* hbmBack;
    UINT wID;
#if (_WIN32_IE >= 0x0400)
    UINT cyChild;
    UINT cyMaxChild;
    UINT cyIntegral;
    UINT cxIdeal;
    LPARAM lParam;
    UINT cxHeader;
#endif
  };

  return (GetComCtlVersion() < MAKELONG(1, 6))
      ? sizeof(BCGP_OLDREBARBANDINFO) : sizeof(REBARBANDINFO);
}

BOOL BCGPGLOBAL_DATA::SetDPIAware()
{
  if (m_hinstUser32 == NULL) {
    ASSERT(FALSE);
    return FALSE;
  }

  typedef BOOL (__stdcall * BCGPSETPROCESSDPIAWARE)();

  BCGPSETPROCESSDPIAWARE pSetDPIAware =
      (BCGPSETPROCESSDPIAWARE)::GetProcAddress(m_hinstUser32, "SetProcessDPIAware");

  if (pSetDPIAware == NULL) {
    return FALSE;
  }

  BOOL bRes = (*pSetDPIAware)();

  UpdateSysColors();
  UpdateFonts();
  OnSettingChange();

  return bRes;
}



#endif // __GLOBAL_DATA_H
