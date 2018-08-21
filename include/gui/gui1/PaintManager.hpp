
//#include <zmouse.h>
//#include <tchar.h>
#include <algorithm>

static size_t GetNameHash(const char* pstrName)
{
  size_t i = 0;
  size_t len = strlen(pstrName);
  while ( len-- > 0 ) {
    i = (i << 5) + i + pstrName[len];
  }
  return i;
}


/////////////////////////////////////////////////////////////////////////////////////
//

#define IDB_ICONS16 200
#define IDB_ICONS24 201
#define IDB_ICONS32 202
#define IDB_ICONS50 203


// System settings
typedef struct tagTSystemSettingsUI {
  bool bShowKeyboardCues;
  bool bScrollLists;
} TSystemSettingsUI;

// Various system settings
typedef struct tagTSystemMetricsUI {
  int cxvscroll;
} TSystemMetricsUI;

static const float OneThird = 1.0f / 3;

static void RGBtoHSL(Color ARGB, float* H, float* S, float* L)
{
  const float
  R = (float)(ARGB.GetRedComponent()),
  G = (float)(ARGB.GetGreenComponent()),
  B = (float)(ARGB.GetBlueComponent()),
  nR = (R < 0 ? 0 : (R > 255 ? 255 : R)) / 255,
  nG = (G < 0 ? 0 : (G > 255 ? 255 : G)) / 255,
  nB = (B < 0 ? 0 : (B > 255 ? 255 : B)) / 255,
  m = MIN(MIN(nR, nG), nB),
  M = MAX(MAX(nR, nG), nB);
  *L = (m + M) / 2;
  if (M == m) {
    *H = *S = 0;
  } else {
    const float
    f = (nR == m) ? (nG - nB) : ((nG == m) ? (nB - nR) : (nR - nG)),
    i = (nR == m) ? 3.0f : ((nG == m) ? 5.0f : 1.0f);
    *H = (i - f / (M - m));
    if (*H >= 6) {
      *H -= 6;
    }
    *H *= 60;
    *S = (2 * (*L) <= 1) ? ((M - m) / (M + m)) : ((M - m) / (2 - M - m));
  }
}

static Color HSLtoRGB(float H, float S, float L)
{
  const float
  q = 2 * L < 1 ? L * (1 + S) : (L + S - L * S),
  p = 2 * L - q,
  h = H / 360,
  tr = h + OneThird,
  tg = h,
  tb = h - OneThird,
  ntr = tr < 0 ? tr + 1 : (tr > 1 ? tr - 1 : tr),
  ntg = tg < 0 ? tg + 1 : (tg > 1 ? tg - 1 : tg),
  ntb = tb < 0 ? tb + 1 : (tb > 1 ? tb - 1 : tb),
  B = 255 * (6 * ntr < 1 ? p + (q - p) * 6 * ntr : (2 * ntr < 1 ? q : (3 * ntr < 2 ? p + (q - p) * 6 * (2.0f * OneThird - ntr) : p))),
  G = 255 * (6 * ntg < 1 ? p + (q - p) * 6 * ntg : (2 * ntg < 1 ? q : (3 * ntg < 2 ? p + (q - p) * 6 * (2.0f * OneThird - ntg) : p))),
  R = 255 * (6 * ntb < 1 ? p + (q - p) * 6 * ntb : (2 * ntb < 1 ? q : (3 * ntb < 2 ? p + (q - p) * 6 * (2.0f * OneThird - ntb) : p)));
  //ARGB &= 0xFF000000;
  //ARGB |= RGB((uchar)(R<0 ? 0 : (R>255 ? 255 : R)), (uchar)(G<0 ? 0 : (G>255 ? 255 : G)), (uchar)(B<0 ? 0 : (B>255 ? 255 : B)));
  return Color(R, G, B);
}

/////////////////////////////////////////////////////////////////////////////////////
//


//#include "XUnzip.inl"
#include "utils/stb_image.h"

int ImageLoad(Image* image, const uchar* pData, size_t dwSize, uint32_t mask)
{
  uchar* pImage = NULL;
  int x = 1, y = 1, n;
  pImage = stbi_load_from_memory(pData, dwSize, &x, &y, &n, 4);
  if (!pImage) {
    //::MessageBox(0, _T("½âÎöÍ¼Æ¬Ê§°Ü"), _T("×¥BUG"), MB_OK);
    return 0;
  }
  image->resize(x, y);
  if (0) {
    uchar* pDest = NULL;
    pDest = image->data;
    for (int i = 0; i < x * y; i++) {
      uchar a = pImage[i * 4 + 3];
      pDest[i * 4 + 3] = a;
      if (a < 255) {
        pDest[i * 4] = (uchar)((uint32_t)(pImage[i * 4 + 2]) * a / 255);
        pDest[i * 4 + 1] = (uchar)((uint32_t)(pImage[i * 4 + 1]) * a / 255);
        pDest[i * 4 + 2] = (uchar)((uint32_t)(pImage[i * 4]) * a / 255);
      } else {
        pDest[i * 4] = pImage[i * 4 + 2];
        pDest[i * 4 + 1] = pImage[i * 4 + 1];
        pDest[i * 4 + 2] = pImage[i * 4];
      }
      if (*(uint32_t*)(&pDest[i * 4]) == mask) {
        pDest[i * 4] = (uchar)0;
        pDest[i * 4 + 1] = (uchar)0;
        pDest[i * 4 + 2] = (uchar)0;
        pDest[i * 4 + 3] = (uchar)0;
      }
    }
  } else {
    memcpy(image->data, pImage, x * y * 4);
  }
  stbi_image_free(pImage);
  return 1;
}


/////////////////////////////////////////////////////////////////////////////////////

struct CPaintManagerUI {
#if 0
  TSystemMetricsUI m_SystemMetrics;
  TSystemSettingsUI m_SystemConfig;
  POINT m_ptLastMousePos;
  SIZE m_szMinWindow;
  UINT m_uMsgMouseWheel;
  UINT m_uTimerID;
  bool m_bFirstLayout;
  bool m_bResizeNeeded;
  bool m_bFocusNeeded;
  bool m_bOffscreenPaint;
  bool m_bMouseTracking;
  
  HPEN m_hPens[UICOLOR__LAST] = { 0 };
  HFONT m_hFonts[UIFONT__LAST] = { 0 };
  HBRUSH m_hBrushes[UICOLOR__LAST] = { 0 };
  LOGFONT m_aLogFonts[UIFONT__LAST] = { 0 };
  COLORREF m_clrColors[UICOLOR__LAST][2] = { 0 };
  TEXTMETRIC m_aTextMetrics[UIFONT__LAST] = { 0 };
  HIMAGELIST m_himgIcons16 = NULL;
  HIMAGELIST m_himgIcons24 = NULL;
  HIMAGELIST m_himgIcons32 = NULL;
  HIMAGELIST m_himgIcons50 = NULL;
  
  CPaintManagerUI::CPaintManagerUI() {
    if ( m_hFonts[1] == NULL ) {
      // Fill in default font information
      LOGFONT lf = { 0 };
      ::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
      _tcscpy(lf.lfFaceName, _T("Tahoma"));
      // TODO: Handle "large fonts" or other font sizes when
      //       screen resolution changes!!!
      lf.lfHeight = -12;
      m_aLogFonts[UIFONT_NORMAL] = lf;
      m_aLogFonts[UIFONT_CAPTION] = lf;
      LOGFONT lfBold = lf;
      lfBold.lfWeight += FW_BOLD;
      m_aLogFonts[UIFONT_BOLD] = lfBold;
      lfBold.lfHeight -= 2;
      m_aLogFonts[UIFONT_TITLE] = lfBold;
      lfBold.lfHeight -= 4;
      m_aLogFonts[UIFONT_HEADLINE] = lfBold;
      LOGFONT lfSubscript = lf;
      lfSubscript.lfHeight -= 4;
      m_aLogFonts[UIFONT_SUBSCRIPT] = lfSubscript;
      LOGFONT lfLink = lf;
      lfLink.lfUnderline = TRUE;
      m_aLogFonts[UIFONT_LINK] = lfLink;
      // Fill the color table
      m_clrColors[UICOLOR_WINDOW_BACKGROUND][0]            = RGB(239, 239, 235);
      m_clrColors[UICOLOR_DIALOG_BACKGROUND][0]            = RGB(238, 238, 238);
      m_clrColors[UICOLOR_DIALOG_TEXT_NORMAL][0]           = RGB(0, 0, 0);
      m_clrColors[UICOLOR_DIALOG_TEXT_DARK][0]             = RGB(96, 96, 80);
      m_clrColors[UICOLOR_TITLE_BACKGROUND][0]             = RGB(114, 136, 172);
      m_clrColors[UICOLOR_TITLE_TEXT][0]                   = RGB(255, 255, 255);
      m_clrColors[UICOLOR_TITLE_BORDER_LIGHT][0]           = RGB(171, 192, 231);
      m_clrColors[UICOLOR_TITLE_BORDER_DARK][0]            = RGB(0, 55, 122);
      m_clrColors[UICOLOR_BUTTON_BACKGROUND_NORMAL][0]     = RGB(250, 250, 252);
      m_clrColors[UICOLOR_BUTTON_BACKGROUND_NORMAL][1]    = RGB(215, 215, 227);
      m_clrColors[UICOLOR_BUTTON_BACKGROUND_DISABLED][0]   = RGB(248, 248, 248);
      m_clrColors[UICOLOR_BUTTON_BACKGROUND_DISABLED][1]  = RGB(214, 214, 214);
      m_clrColors[UICOLOR_BUTTON_BACKGROUND_PUSHED][0]     = RGB(215, 215, 227);
      m_clrColors[UICOLOR_BUTTON_BACKGROUND_PUSHED][1]    = RGB(250, 250, 252);
      m_clrColors[UICOLOR_BUTTON_TEXT_NORMAL][0]           = RGB(0, 0, 0);
      m_clrColors[UICOLOR_BUTTON_TEXT_PUSHED][0]           = RGB(0, 0, 20);
      m_clrColors[UICOLOR_BUTTON_TEXT_DISABLED][0]         = RGB(204, 204, 204);
      m_clrColors[UICOLOR_BUTTON_BORDER_LIGHT][0]          = RGB(123, 158, 189);
      m_clrColors[UICOLOR_BUTTON_BORDER_DARK][0]           = RGB(123, 158, 189);
      m_clrColors[UICOLOR_BUTTON_BORDER_DISABLED][0]       = RGB(204, 204, 204);
      m_clrColors[UICOLOR_BUTTON_BORDER_FOCUS][0]          = RGB(140, 140, 140);
      m_clrColors[UICOLOR_TOOL_BACKGROUND_NORMAL][0]       = RGB(114, 136, 172);
      m_clrColors[UICOLOR_TOOL_BACKGROUND_DISABLED][0]     = RGB(100, 121, 156);
      m_clrColors[UICOLOR_TOOL_BACKGROUND_HOVER][0]        = RGB(100, 121, 156);
      m_clrColors[UICOLOR_TOOL_BACKGROUND_PUSHED][0]       = RGB(80, 101, 136);
      m_clrColors[UICOLOR_TOOL_BORDER_NORMAL][0]           = RGB(0, 55, 122);
      m_clrColors[UICOLOR_TOOL_BORDER_DISABLED][0]         = RGB(0, 55, 122);
      m_clrColors[UICOLOR_TOOL_BORDER_HOVER][0]            = RGB(0, 55, 122);
      m_clrColors[UICOLOR_TOOL_BORDER_PUSHED][0]           = RGB(0, 55, 122);
      m_clrColors[UICOLOR_EDIT_BACKGROUND_DISABLED][0]     = RGB(255, 251, 255);
      m_clrColors[UICOLOR_EDIT_BACKGROUND_READONLY][0]     = RGB(255, 251, 255);
      m_clrColors[UICOLOR_EDIT_BACKGROUND_NORMAL][0]       = RGB(255, 255, 255);
      m_clrColors[UICOLOR_EDIT_BACKGROUND_HOVER][0]        = RGB(255, 251, 255);
      m_clrColors[UICOLOR_EDIT_TEXT_NORMAL][0]             = RGB(0, 0, 0);
      m_clrColors[UICOLOR_EDIT_TEXT_DISABLED][0]           = RGB(167, 166, 170);
      m_clrColors[UICOLOR_EDIT_TEXT_READONLY][0]           = RGB(167, 166, 170);
      m_clrColors[UICOLOR_NAVIGATOR_BACKGROUND][0]         = RGB(229, 217, 213);
      m_clrColors[UICOLOR_NAVIGATOR_BACKGROUND][1]        = RGB(201, 199, 187);
      m_clrColors[UICOLOR_NAVIGATOR_TEXT_NORMAL][0]        = RGB(102, 102, 102);
      m_clrColors[UICOLOR_NAVIGATOR_TEXT_SELECTED][0]      = RGB(0, 0, 0);
      m_clrColors[UICOLOR_NAVIGATOR_TEXT_PUSHED][0]        = RGB(0, 0, 0);
      m_clrColors[UICOLOR_NAVIGATOR_BORDER_NORMAL][0]      = RGB(131, 133, 116);
      m_clrColors[UICOLOR_NAVIGATOR_BORDER_SELECTED][0]    = RGB(159, 160, 144);
      m_clrColors[UICOLOR_NAVIGATOR_BUTTON_HOVER][0]       = RGB(200, 200, 200);
      m_clrColors[UICOLOR_NAVIGATOR_BUTTON_PUSHED][0]      = RGB(184, 184, 183);
      m_clrColors[UICOLOR_NAVIGATOR_BUTTON_SELECTED][0]    = RGB(238, 238, 238);
      m_clrColors[UICOLOR_TAB_BACKGROUND_NORMAL][0]        = RGB(255, 251, 255);
      m_clrColors[UICOLOR_TAB_FOLDER_NORMAL][0]            = RGB(255, 251, 255);
      m_clrColors[UICOLOR_TAB_FOLDER_NORMAL][1]           = RGB(233, 231, 215);
      m_clrColors[UICOLOR_TAB_FOLDER_SELECTED][0]          = RGB(255, 251, 255);
      m_clrColors[UICOLOR_TAB_BORDER][0]                   = RGB(148, 166, 181);
      m_clrColors[UICOLOR_TAB_TEXT_NORMAL][0]              = RGB(0, 0, 0);
      m_clrColors[UICOLOR_TAB_TEXT_SELECTED][0]            = RGB(0, 0, 0);
      m_clrColors[UICOLOR_TAB_TEXT_DISABLED][0]            = RGB(0, 0, 0);
      m_clrColors[UICOLOR_HEADER_BACKGROUND][0]            = RGB(233, 231, 215);
      m_clrColors[UICOLOR_HEADER_BACKGROUND][1]           = RGB(150, 150, 147);
      m_clrColors[UICOLOR_HEADER_BORDER][0]                = RGB(218, 219, 201);
      m_clrColors[UICOLOR_HEADER_SEPARATOR][0]             = RGB(197, 193, 177);
      m_clrColors[UICOLOR_HEADER_TEXT][0]                  = RGB(0, 0, 0);
      m_clrColors[UICOLOR_TASK_BACKGROUND][0]              = RGB(230, 243, 255);
      m_clrColors[UICOLOR_TASK_BACKGROUND][1]             = RGB(255, 255, 255);
      m_clrColors[UICOLOR_TASK_BORDER][0]                  = RGB(140, 158, 198);
      m_clrColors[UICOLOR_TASK_CAPTION][0]                 = RGB(140, 158, 198);
      m_clrColors[UICOLOR_TASK_TEXT][0]                    = RGB(65, 65, 110);
      m_clrColors[UICOLOR_LINK_TEXT_NORMAL][0]             = RGB(0, 0, 255);
      m_clrColors[UICOLOR_LINK_TEXT_HOVER][0]              = RGB(0, 0, 100);
      m_clrColors[UICOLOR_CONTROL_BACKGROUND_NORMAL][0]    = RGB(255, 255, 255);
      m_clrColors[UICOLOR_CONTROL_BACKGROUND_SELECTED][0]  = RGB(173, 195, 231);
      m_clrColors[UICOLOR_CONTROL_BACKGROUND_READONLY][0]  = RGB(255, 255, 255);
      m_clrColors[UICOLOR_CONTROL_BACKGROUND_DISABLED][0]  = RGB(255, 255, 255);
      m_clrColors[UICOLOR_CONTROL_BACKGROUND_HOVER][0]     = RGB(233, 245, 255);
      m_clrColors[UICOLOR_CONTROL_BACKGROUND_SORTED][0]    = RGB(242, 242, 246);
      m_clrColors[UICOLOR_CONTROL_BACKGROUND_EXPANDED][0]  = RGB(255, 255, 255);
      m_clrColors[UICOLOR_CONTROL_BACKGROUND_EXPANDED][1] = RGB(236, 242, 255);
      m_clrColors[UICOLOR_CONTROL_BORDER_NORMAL][0]        = RGB(123, 158, 189);
      m_clrColors[UICOLOR_CONTROL_BORDER_SELECTED][0]      = RGB(123, 158, 189);
      m_clrColors[UICOLOR_CONTROL_BORDER_DISABLED][0]      = RGB(204, 204, 204);
      m_clrColors[UICOLOR_CONTROL_TEXT_NORMAL][0]          = RGB(0, 0, 0);
      m_clrColors[UICOLOR_CONTROL_TEXT_SELECTED][0]        = RGB(0, 0, 0);
      m_clrColors[UICOLOR_CONTROL_TEXT_DISABLED][0]        = RGB(204, 204, 204);
      m_clrColors[UICOLOR_STANDARD_BLACK][0]               = RGB(0, 0, 0);
      m_clrColors[UICOLOR_STANDARD_YELLOW][0]              = RGB(255, 255, 204);
      m_clrColors[UICOLOR_STANDARD_RED][0]                 = RGB(255, 204, 204);
      m_clrColors[UICOLOR_STANDARD_GREY][0]                = RGB(145, 146, 119);
      m_clrColors[UICOLOR_STANDARD_LIGHTGREY][0]           = RGB(195, 196, 179);
      m_clrColors[UICOLOR_STANDARD_WHITE][0]               = RGB(255, 255, 255);
      // Boot Windows Common Controls (for the ToolTip control)
      ::InitCommonControls();
      // We need the image library for effects. It is however optional in Windows so
      // we'll also need to provide a gracefull fallback.
      ::LoadLibrary("msimg32.dll");
    }
    m_szMinWindow.cx = 140;
    m_szMinWindow.cy = 200;
    m_ptLastMousePos.x = m_ptLastMousePos.y = -1;
    m_uMsgMouseWheel = ::RegisterWindowMessage(MSH_MOUSEWHEEL);
    // System Config
    m_SystemConfig.bShowKeyboardCues = false;
    m_SystemConfig.bScrollLists = false;
    // System Metrics
    m_SystemMetrics.cxvscroll = (INT) ::GetSystemMetrics(SM_CXVSCROLL);
  }


  CPaintManagerUI::~CPaintManagerUI() {
  }

  HINSTANCE CPaintManagerUI::GetResourceInstance() {
    return m_hInstance;
  }

  HINSTANCE CPaintManagerUI::GetLanguageInstance() {
    return m_hLangInst;
  }

  void CPaintManagerUI::SetResourceInstance(HINSTANCE hInst) {
    m_hInstance = hInst;
    if ( m_hLangInst == NULL ) {
      m_hLangInst = hInst;
    }
  }

  void CPaintManagerUI::SetLanguageInstance(HINSTANCE hInst) {
    m_hLangInst = hInst;
  }

  HWND CPaintManagerUI::GetPaintWindow() const {
    return m_hWndPaint;
  }

  HDC CPaintManagerUI::GetPaintDC() const {
    return m_hDcPaint;
  }

  POINT CPaintManagerUI::GetMousePos() const {
    return m_ptLastMousePos;
  }

  SIZE CPaintManagerUI::GetClientSize() const {
    RECT rcClient = { 0 };
    ::GetClientRect(m_hWndPaint, &rcClient);
    return CSize(rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
  }

  void CPaintManagerUI::SetMinMaxInfo(int cx, int cy) {
    ASSERT(cx >= 0 && cy >= 0);
    m_szMinWindow.cx = cx;
    m_szMinWindow.cy = cy;
  }

  TSystemSettingsUI CPaintManagerUI::GetSystemSettings() const {
    return m_SystemConfig;
  }

  void CPaintManagerUI::SetSystemSettings(const TSystemSettingsUI Config) {
    m_SystemConfig = Config;
  }

  TSystemMetricsUI CPaintManagerUI::GetSystemMetrics() const {
    return m_SystemMetrics;
  }

  HFONT CPaintManagerUI::GetThemeFont(UITYPE_FONT Index) const {
    if ( Index <= UIFONT__FIRST || Index >= UIFONT__LAST ) {
      return NULL;
    }
    if ( m_hFonts[Index] == NULL ) {
      m_hFonts[Index] = ::CreateFontIndirect(&m_aLogFonts[Index]);
    }
    return m_hFonts[Index];
  }

  HICON CPaintManagerUI::GetThemeIcon(int iIndex, int cxySize) const {
    if ( m_himgIcons16 == NULL ) {
      m_himgIcons16 = ImageList_LoadImage(m_hInstance, MAKEINTRESOURCE(IDB_ICONS16), 16, 0, RGB(255, 0, 255), IMAGE_BITMAP, LR_CREATEDIBSECTION);
      m_himgIcons24 = ImageList_LoadImage(m_hInstance, MAKEINTRESOURCE(IDB_ICONS16), 16, 0, RGB(255, 0, 255), IMAGE_BITMAP, LR_CREATEDIBSECTION);
      m_himgIcons32 = ImageList_LoadImage(m_hInstance, MAKEINTRESOURCE(IDB_ICONS16), 16, 0, RGB(255, 0, 255), IMAGE_BITMAP, LR_CREATEDIBSECTION);
      m_himgIcons50 = ImageList_LoadImage(m_hInstance, MAKEINTRESOURCE(IDB_ICONS50), 50, 0, RGB(255, 0, 255), IMAGE_BITMAP, LR_CREATEDIBSECTION);
    }
    if ( cxySize == 16 ) {
      return ImageList_GetIcon(m_himgIcons16, iIndex, ILD_NORMAL);
    } else if ( cxySize == 24 ) {
      return ImageList_GetIcon(m_himgIcons24, iIndex, ILD_NORMAL);
    } else if ( cxySize == 32 ) {
      return ImageList_GetIcon(m_himgIcons32, iIndex, ILD_NORMAL);
    } else if ( cxySize == 50 ) {
      return ImageList_GetIcon(m_himgIcons50, iIndex, ILD_NORMAL);
    }
    return NULL;
  }

  HPEN CPaintManagerUI::GetThemePen(UITYPE_COLOR Index) const {
    if ( Index <= UICOLOR__FIRST || Index >= UICOLOR__LAST ) {
      return NULL;
    }
    if ( m_hPens[Index] == NULL ) {
      m_hPens[Index] = ::CreatePen(PS_SOLID, 1, m_clrColors[Index][0]);
    }
    return m_hPens[Index];
  }

  HBRUSH CPaintManagerUI::GetThemeBrush(UITYPE_COLOR Index) const {
    if ( Index <= UICOLOR__FIRST || Index >= UICOLOR__LAST ) {
      return NULL;
    }
    if ( m_hBrushes[Index] == NULL ) {
      m_hBrushes[Index] = ::CreateSolidBrush(m_clrColors[Index][0]);
    }
    return m_hBrushes[Index];
  }

  const TEXTMETRIC & CPaintManagerUI::GetThemeFontInfo(UITYPE_FONT Index) const {
    if ( Index <= UIFONT__FIRST || Index >= UIFONT__LAST ) {
      return m_aTextMetrics[0];
    }
    if ( m_aTextMetrics[Index].tmHeight == 0 ) {
      HFONT hOldFont = (HFONT) ::SelectObject(m_hDcPaint, GetThemeFont(Index));
      ::GetTextMetrics(m_hDcPaint, &m_aTextMetrics[Index]);
      ::SelectObject(m_hDcPaint, hOldFont);
    }
    return m_aTextMetrics[Index];
  }

  COLORREF CPaintManagerUI::GetThemeColor(UITYPE_COLOR Index) const {
    if ( Index <= UICOLOR__FIRST || Index >= UICOLOR__LAST ) {
      return RGB(0, 0, 0);
    }
    return m_clrColors[Index][0];
  }

  bool CPaintManagerUI::GetThemeColorPair(UITYPE_COLOR Index, COLORREF & clr1, COLORREF & clr2) const {
    if ( Index <= UICOLOR__FIRST || Index >= UICOLOR__LAST ) {
      return false;
    }
    clr1 = m_clrColors[Index][0];
    clr2 = m_clrColors[Index][1];
    return true;
  }

#endif

};
