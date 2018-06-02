typedef enum {
  UIFONT__FIRST = 0,
  UIFONT_NORMAL,
  UIFONT_BOLD,
  UIFONT_CAPTION,
  UIFONT_MENU,
  UIFONT_LINK,
  UIFONT_TITLE,
  UIFONT_HEADLINE,
  UIFONT_SUBSCRIPT,
  UIFONT_MARLETT,
  UIFONT__LAST,
} UITYPE_FONT;
typedef enum {
  UICOLOR__FIRST = 0,
  UICOLOR_WINDOW_BACKGROUND,
  UICOLOR_WINDOW_TEXT,
  UICOLOR_DIALOG_BACKGROUND,
  UICOLOR_DIALOG_TEXT_NORMAL,
  UICOLOR_DIALOG_TEXT_DARK,
  UICOLOR_MENU_BACKGROUND,
  UICOLOR_MENU_TEXT_NORMAL,
  UICOLOR_MENU_TEXT_HOVER,
  UICOLOR_MENU_TEXT_SELECTED,
  UICOLOR_TEXTCOLOR_NORMAL,
  UICOLOR_TEXTCOLOR_HOVER,
  UICOLOR_TAB_BACKGROUND_NORMAL,
  UICOLOR_TAB_BACKGROUND_SELECTED,
  UICOLOR_TAB_FOLDER_NORMAL,
  UICOLOR_TAB_FOLDER_SELECTED,
  UICOLOR_TAB_BORDER,
  UICOLOR_TAB_TEXT_NORMAL,
  UICOLOR_TAB_TEXT_SELECTED,
  UICOLOR_TAB_TEXT_DISABLED,
  UICOLOR_NAVIGATOR_BACKGROUND,
  UICOLOR_NAVIGATOR_BUTTON_HOVER,
  UICOLOR_NAVIGATOR_BUTTON_PUSHED,
  UICOLOR_NAVIGATOR_BUTTON_SELECTED,
  UICOLOR_NAVIGATOR_BORDER_NORMAL,
  UICOLOR_NAVIGATOR_BORDER_SELECTED,
  UICOLOR_NAVIGATOR_TEXT_NORMAL,
  UICOLOR_NAVIGATOR_TEXT_SELECTED,
  UICOLOR_NAVIGATOR_TEXT_PUSHED,
  UICOLOR_BUTTON_BACKGROUND_NORMAL,
  UICOLOR_BUTTON_BACKGROUND_DISABLED,
  UICOLOR_BUTTON_BACKGROUND_PUSHED,
  UICOLOR_BUTTON_TEXT_NORMAL,
  UICOLOR_BUTTON_TEXT_PUSHED,
  UICOLOR_BUTTON_TEXT_DISABLED,
  UICOLOR_BUTTON_BORDER_LIGHT,
  UICOLOR_BUTTON_BORDER_DARK,
  UICOLOR_BUTTON_BORDER_DISABLED,
  UICOLOR_BUTTON_BORDER_FOCUS,
  UICOLOR_CONTROL_BACKGROUND_NORMAL,
  UICOLOR_CONTROL_BACKGROUND_SELECTED,
  UICOLOR_CONTROL_BACKGROUND_DISABLED,
  UICOLOR_CONTROL_BACKGROUND_READONLY,
  UICOLOR_CONTROL_BACKGROUND_HOVER,
  UICOLOR_CONTROL_BACKGROUND_SORTED,
  UICOLOR_CONTROL_BACKGROUND_EXPANDED,
  UICOLOR_CONTROL_BORDER_NORMAL,
  UICOLOR_CONTROL_BORDER_SELECTED,
  UICOLOR_CONTROL_BORDER_DISABLED,
  UICOLOR_CONTROL_TEXT_NORMAL,
  UICOLOR_CONTROL_TEXT_SELECTED,
  UICOLOR_CONTROL_TEXT_DISABLED,
  UICOLOR_CONTROL_TEXT_READONLY,
  UICOLOR_TOOL_BACKGROUND_NORMAL,
  UICOLOR_TOOL_BACKGROUND_DISABLED,
  UICOLOR_TOOL_BACKGROUND_HOVER,
  UICOLOR_TOOL_BACKGROUND_PUSHED,
  UICOLOR_TOOL_BORDER_NORMAL,
  UICOLOR_TOOL_BORDER_DISABLED,
  UICOLOR_TOOL_BORDER_HOVER,
  UICOLOR_TOOL_BORDER_PUSHED,
  UICOLOR_EDIT_BACKGROUND_NORMAL,
  UICOLOR_EDIT_BACKGROUND_HOVER,
  UICOLOR_EDIT_BACKGROUND_DISABLED,
  UICOLOR_EDIT_BACKGROUND_READONLY,
  UICOLOR_EDIT_TEXT_NORMAL,
  UICOLOR_EDIT_TEXT_DISABLED,
  UICOLOR_EDIT_TEXT_READONLY,
  UICOLOR_TITLE_BACKGROUND,
  UICOLOR_TITLE_TEXT,
  UICOLOR_TITLE_BORDER_LIGHT,
  UICOLOR_TITLE_BORDER_DARK,
  UICOLOR_HEADER_BACKGROUND,
  UICOLOR_HEADER_BORDER,
  UICOLOR_HEADER_SEPARATOR,
  UICOLOR_HEADER_TEXT,
  UICOLOR_TASK_BACKGROUND,
  UICOLOR_TASK_CAPTION,
  UICOLOR_TASK_BORDER,
  UICOLOR_TASK_TEXT,
  UICOLOR_LINK_TEXT_HOVER,
  UICOLOR_LINK_TEXT_NORMAL,
  UICOLOR_STANDARD_BLACK,
  UICOLOR_STANDARD_YELLOW,
  UICOLOR_STANDARD_RED,
  UICOLOR_STANDARD_GREY,
  UICOLOR_STANDARD_LIGHTGREY,
  UICOLOR_STANDARD_WHITE,
  UICOLOR__LAST,
  UICOLOR__INVALID,
} UITYPE_COLOR;
typedef struct _FontInfo {
  TCHAR szName[32];
  HFONT hFont;
} FontInfo;
HFONT m_hFonts[UIFONT__LAST] = { 0 };
LOGFONT m_aLogFonts[UIFONT__LAST] = { 0 };
COLORREF m_clrColors[UICOLOR__LAST][2] = { 0 };
TEXTMETRIC m_aTextMetrics[UIFONT__LAST] = { 0 };
HIMAGELIST m_himgIcons16 = NULL;
HIMAGELIST m_himgIcons24 = NULL;
HIMAGELIST m_himgIcons32 = NULL;
HIMAGELIST m_himgIcons50 = NULL;
POINT GetMousePos()
{
  POINT point;
  GetCursorPos(&point);
  return point;
}
#define SelectThemeFontToMemDC(idFont) SelectObject(GetMemDC(), GetThemeFont(idFont))
int g_nScrollSize = 20;
int BlueRenderEngineInit()
{
  static int inited = 0;
  if (!inited) {
    // Fill in default font information
    LOGFONT lf = { 0 };
    LOGFONT lfBold;
    LOGFONT lfSubscript;
    LOGFONT lfLink;
    LOGFONT lfMarlett;
    inited = 1;
    GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
    _tcscpy(lf.lfFaceName, _T("宋体"));
    // TODO: Handle "large fonts" or other font sizes when
    // screen resolution changes!!!
    lf.lfHeight = -16;
    m_aLogFonts[UIFONT_NORMAL] = lf;
    m_aLogFonts[UIFONT_CAPTION] = lf;
    lfBold = lf;
    lfBold.lfWeight += FW_BOLD;
    m_aLogFonts[UIFONT_BOLD] = lfBold;
    lfBold.lfHeight -= 2;
    m_aLogFonts[UIFONT_TITLE] = lfBold;
    lfBold.lfHeight -= 4;
    m_aLogFonts[UIFONT_HEADLINE] = lfBold;
    lfSubscript = lf;
    lfSubscript.lfHeight -= 4;
    m_aLogFonts[UIFONT_SUBSCRIPT] = lfSubscript;
    lfLink = lf;
    lfLink.lfUnderline = TRUE;
    m_aLogFonts[UIFONT_LINK] = lfLink;
    lfMarlett = lf;
    lfMarlett.lfQuality = DEFAULT_QUALITY;
    lfMarlett.lfCharSet = DEFAULT_CHARSET;
    lfMarlett.lfOutPrecision = OUT_TT_ONLY_PRECIS;
    lfMarlett.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    lfMarlett.lfPitchAndFamily = DEFAULT_PITCH | FF_DECORATIVE;
    _tcscpy(lfMarlett.lfFaceName, _T("Marlett"));
    m_aLogFonts[UIFONT_MARLETT] = lfMarlett;
    //chk=CreateFont( ( int ) ( ( double ) lf.lfHeight * 1.25 ), 0, 0, 0, FW_NORMAL, 0, 0, 0,
    // DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DECORATIVE, "Marlett" );
    //m_TxtFont = CreateFont( 15, 0, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS,
    // CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, "宋体" );
    //SelectThemeFontToMemDC(UIFONT_NORMAL);
    // Fill the color table
    m_clrColors[UICOLOR_WINDOW_BACKGROUND][0] = _RGB(239, 239, 235);
    m_clrColors[UICOLOR_DIALOG_BACKGROUND][0] = _RGB(238, 238, 238);
    m_clrColors[UICOLOR_DIALOG_TEXT_NORMAL][0] = _RGB(0, 0, 0);
    m_clrColors[UICOLOR_DIALOG_TEXT_DARK][0] = _RGB(96, 96, 80);
    m_clrColors[UICOLOR_TITLE_BACKGROUND][0] = _RGB(114, 136, 172);
    m_clrColors[UICOLOR_TITLE_TEXT][0] = _RGB(255, 255, 255);
    m_clrColors[UICOLOR_TITLE_BORDER_LIGHT][0] = _RGB(171, 192, 231);
    m_clrColors[UICOLOR_TITLE_BORDER_DARK][0] = _RGB(0, 55, 122);
    m_clrColors[UICOLOR_BUTTON_BACKGROUND_NORMAL][0] = _RGB(250, 250, 252);
    m_clrColors[UICOLOR_BUTTON_BACKGROUND_NORMAL][1] = _RGB(215, 215, 227);
    m_clrColors[UICOLOR_BUTTON_BACKGROUND_DISABLED][0] = _RGB(248, 248, 248);
    m_clrColors[UICOLOR_BUTTON_BACKGROUND_DISABLED][1] = _RGB(214, 214, 214);
    m_clrColors[UICOLOR_BUTTON_BACKGROUND_PUSHED][0] = _RGB(215, 215, 227);
    m_clrColors[UICOLOR_BUTTON_BACKGROUND_PUSHED][1] = _RGB(250, 250, 252);
    m_clrColors[UICOLOR_BUTTON_TEXT_NORMAL][0] = _RGB(0, 0, 0);
    m_clrColors[UICOLOR_BUTTON_TEXT_PUSHED][0] = _RGB(0, 0, 20);
    m_clrColors[UICOLOR_BUTTON_TEXT_DISABLED][0] = _RGB(204, 204, 204);
    m_clrColors[UICOLOR_BUTTON_BORDER_LIGHT][0] = _RGB(123, 158, 189);
    m_clrColors[UICOLOR_BUTTON_BORDER_DARK][0] = _RGB(123, 158, 189);
    m_clrColors[UICOLOR_BUTTON_BORDER_DISABLED][0] = _RGB(204, 204, 204);
    m_clrColors[UICOLOR_BUTTON_BORDER_FOCUS][0] = _RGB(140, 140, 140);
    m_clrColors[UICOLOR_TOOL_BACKGROUND_NORMAL][0] = _RGB(114, 136, 172);
    m_clrColors[UICOLOR_TOOL_BACKGROUND_DISABLED][0] = _RGB(100, 121, 156);
    m_clrColors[UICOLOR_TOOL_BACKGROUND_HOVER][0] = _RGB(100, 121, 156);
    m_clrColors[UICOLOR_TOOL_BACKGROUND_PUSHED][0] = _RGB(80, 101, 136);
    m_clrColors[UICOLOR_TOOL_BORDER_NORMAL][0] = _RGB(0, 55, 122);
    m_clrColors[UICOLOR_TOOL_BORDER_DISABLED][0] = _RGB(0, 55, 122);
    m_clrColors[UICOLOR_TOOL_BORDER_HOVER][0] = _RGB(0, 55, 122);
    m_clrColors[UICOLOR_TOOL_BORDER_PUSHED][0] = _RGB(0, 55, 122);
    m_clrColors[UICOLOR_EDIT_BACKGROUND_DISABLED][0] = _RGB(255, 251, 255);
    m_clrColors[UICOLOR_EDIT_BACKGROUND_READONLY][0] = _RGB(255, 251, 255);
    m_clrColors[UICOLOR_EDIT_BACKGROUND_NORMAL][0] = _RGB(255, 255, 255);
    m_clrColors[UICOLOR_EDIT_BACKGROUND_HOVER][0] = _RGB(255, 251, 255);
    m_clrColors[UICOLOR_EDIT_TEXT_NORMAL][0] = _RGB(0, 0, 0);
    m_clrColors[UICOLOR_EDIT_TEXT_DISABLED][0] = _RGB(167, 166, 170);
    m_clrColors[UICOLOR_EDIT_TEXT_READONLY][0] = _RGB(167, 166, 170);
    m_clrColors[UICOLOR_NAVIGATOR_BACKGROUND][0] = _RGB(229, 217, 213);
    m_clrColors[UICOLOR_NAVIGATOR_BACKGROUND][1] = _RGB(201, 199, 187);
    m_clrColors[UICOLOR_NAVIGATOR_TEXT_NORMAL][0] = _RGB(102, 102, 102);
    m_clrColors[UICOLOR_NAVIGATOR_TEXT_SELECTED][0] = _RGB(0, 0, 0);
    m_clrColors[UICOLOR_NAVIGATOR_TEXT_PUSHED][0] = _RGB(0, 0, 0);
    m_clrColors[UICOLOR_NAVIGATOR_BORDER_NORMAL][0] = _RGB(131, 133, 116);
    m_clrColors[UICOLOR_NAVIGATOR_BORDER_SELECTED][0] = _RGB(159, 160, 144);
    m_clrColors[UICOLOR_NAVIGATOR_BUTTON_HOVER][0] = _RGB(200, 200, 200);
    m_clrColors[UICOLOR_NAVIGATOR_BUTTON_PUSHED][0] = _RGB(184, 184, 183);
    m_clrColors[UICOLOR_NAVIGATOR_BUTTON_SELECTED][0] = _RGB(238, 238, 238);
    m_clrColors[UICOLOR_TAB_BACKGROUND_NORMAL][0] = _RGB(255, 251, 255);
    m_clrColors[UICOLOR_TAB_FOLDER_NORMAL][0] = _RGB(255, 251, 255);
    m_clrColors[UICOLOR_TAB_FOLDER_NORMAL][1] = _RGB(233, 231, 215);
    m_clrColors[UICOLOR_TAB_FOLDER_SELECTED][0] = _RGB(255, 251, 255);
    m_clrColors[UICOLOR_TAB_BORDER][0] = _RGB(148, 166, 181);
    m_clrColors[UICOLOR_TAB_TEXT_NORMAL][0] = _RGB(0, 0, 0);
    m_clrColors[UICOLOR_TAB_TEXT_SELECTED][0] = _RGB(0, 0, 0);
    m_clrColors[UICOLOR_TAB_TEXT_DISABLED][0] = _RGB(0, 0, 0);
    m_clrColors[UICOLOR_HEADER_BACKGROUND][0] = _RGB(233, 231, 215);
    m_clrColors[UICOLOR_HEADER_BACKGROUND][1] = _RGB(150, 150, 147);
    m_clrColors[UICOLOR_HEADER_BORDER][0] = _RGB(218, 219, 201);
    m_clrColors[UICOLOR_HEADER_SEPARATOR][0] = _RGB(197, 193, 177);
    m_clrColors[UICOLOR_HEADER_TEXT][0] = _RGB(0, 0, 0);
    m_clrColors[UICOLOR_TASK_BACKGROUND][0] = _RGB(230, 243, 255);
    m_clrColors[UICOLOR_TASK_BACKGROUND][1] = _RGB(255, 255, 255);
    m_clrColors[UICOLOR_TASK_BORDER][0] = _RGB(140, 158, 198);
    m_clrColors[UICOLOR_TASK_CAPTION][0] = _RGB(140, 158, 198);
    m_clrColors[UICOLOR_TASK_TEXT][0] = _RGB(65, 65, 110);
    m_clrColors[UICOLOR_LINK_TEXT_NORMAL][0] = _RGB(0, 0, 255);
    m_clrColors[UICOLOR_LINK_TEXT_HOVER][0] = _RGB(0, 0, 100);
    m_clrColors[UICOLOR_CONTROL_BACKGROUND_NORMAL][0] = _RGB(255, 255, 255);
    m_clrColors[UICOLOR_CONTROL_BACKGROUND_SELECTED][0] = _RGB(173, 195, 231);
    m_clrColors[UICOLOR_CONTROL_BACKGROUND_READONLY][0] = _RGB(255, 255, 255);
    m_clrColors[UICOLOR_CONTROL_BACKGROUND_DISABLED][0] = _RGB(255, 255, 255);
    m_clrColors[UICOLOR_CONTROL_BACKGROUND_HOVER][0] = _RGB(233, 245, 255);
    m_clrColors[UICOLOR_CONTROL_BACKGROUND_SORTED][0] = _RGB(242, 242, 246);
    m_clrColors[UICOLOR_CONTROL_BACKGROUND_EXPANDED][0] = _RGB(255, 255, 255);
    m_clrColors[UICOLOR_CONTROL_BACKGROUND_EXPANDED][1] = _RGB(236, 242, 255);
    m_clrColors[UICOLOR_CONTROL_BORDER_NORMAL][0] = _RGB(123, 158, 189);
    m_clrColors[UICOLOR_CONTROL_BORDER_SELECTED][0] = _RGB(123, 158, 189);
    m_clrColors[UICOLOR_CONTROL_BORDER_DISABLED][0] = _RGB(204, 204, 204);
    m_clrColors[UICOLOR_CONTROL_TEXT_NORMAL][0] = _RGB(0, 0, 0);
    m_clrColors[UICOLOR_CONTROL_TEXT_SELECTED][0] = _RGB(0, 0, 0);
    m_clrColors[UICOLOR_CONTROL_TEXT_DISABLED][0] = _RGB(204, 204, 204);
    m_clrColors[UICOLOR_STANDARD_BLACK][0] = _RGB(0, 0, 0);
    m_clrColors[UICOLOR_STANDARD_YELLOW][0] = _RGB(255, 255, 204);
    m_clrColors[UICOLOR_STANDARD_RED][0] = _RGB(255, 204, 204);
    m_clrColors[UICOLOR_STANDARD_GREY][0] = _RGB(145, 146, 119);
    m_clrColors[UICOLOR_STANDARD_LIGHTGREY][0] = _RGB(195, 196, 179);
    m_clrColors[UICOLOR_STANDARD_WHITE][0] = _RGB(255, 255, 255);
    // Boot Windows Common Controls (for the ToolTip control)
    InitCommonControls();
    // We need the image library for effects. It is however optional in Windows so
    // we'll also need to provide a gracefull fallback.
    //LoadLibrary("msimg32.dll");
  }
  return 0;
}
#define GetThemeBrush GetThemeColor
#define GetThemePen GetThemeColor
COLORREF GetThemeColor(int Index)
{
  BlueRenderEngineInit();
  if (Index <= UICOLOR__FIRST || Index >= UICOLOR__LAST) {
    return _RGB(0, 0, 0);
  }
  return m_clrColors[Index][0];
}
HFONT GetThemeFont(int Index)
{
  if (Index <= UIFONT__FIRST || Index >= UIFONT__LAST) {
    return NULL;
  }
  if (m_hFonts[Index] == NULL) {
    m_hFonts[Index] = CreateFontIndirect(&m_aLogFonts[Index]);
  }
  return m_hFonts[Index];
}
const TEXTMETRIC* GetThemeFontInfo(int Index)
{
  if (Index <= UIFONT__FIRST || Index >= UIFONT__LAST) {
    return m_aTextMetrics;
  }
  if (m_aTextMetrics[Index].tmHeight == 0) {
    HDC hMemDC = GetMemDC();
    HFONT hOldFont = (HFONT) SelectObject(GetMemDC(), GetThemeFont(Index));
    GetTextMetrics(hMemDC, &m_aTextMetrics[Index]);
    SelectObject(hMemDC, hOldFont);
  }
  return m_aTextMetrics + Index;
}
BOOL GetThemeColorPair(int Index, COLORREF* clr1, COLORREF* clr2)
{
  if (Index <= UICOLOR__FIRST || Index >= UICOLOR__LAST) {
    return FALSE;
  }
  *clr1 = m_clrColors[Index][0];
  *clr2 = m_clrColors[Index][1];
  return TRUE;
}
#define DoFillRect(h, r, c) draw_fillrectR(h, &r, GetThemePen(c))
int DoPaintFrame(HDDC hDC, RECT rcItem, int Light, int Dark, int Background, UINT uStyle)
{
  int _x0, _y0;
  COLORREF _clr;
  if (Background != UICOLOR__INVALID) {
    DoFillRect(hDC, rcItem, Background);
  }
  if ((uStyle & UIFRAME_ROUND) != 0) {
    _clr = GetThemePen(Light);
    TMoveTo(hDC, rcItem.left, rcItem.bottom - 2);
    _LineTo(hDC, rcItem.left, rcItem.top + 1);
    _LineTo(hDC, rcItem.left + 1, rcItem.top);
    _LineTo(hDC, rcItem.right - 2, rcItem.top);
    _clr = GetThemePen(Dark);
    _LineTo(hDC, rcItem.right - 1, rcItem.top + 1);
    _LineTo(hDC, rcItem.right - 1, rcItem.bottom - 2);
    _LineTo(hDC, rcItem.right - 2, rcItem.bottom - 1);
    _LineTo(hDC, rcItem.left, rcItem.bottom - 1);
  }
  if ((uStyle & UIFRAME_FOCUS) != 0) {
    _clr = GetThemeColor(UICOLOR_BUTTON_BORDER_FOCUS);
    TMoveTo(hDC, rcItem.left, rcItem.bottom - 1);
    _LineTo(hDC, rcItem.left, rcItem.top);
    _LineTo(hDC, rcItem.right - 1, rcItem.top);
    _LineTo(hDC, rcItem.right - 1, rcItem.bottom - 1);
    _LineTo(hDC, rcItem.left, rcItem.bottom - 1);
  }
  else {
    _clr = (GetThemePen(Light));
    TMoveTo(hDC, rcItem.left, rcItem.bottom - 1);
    _LineTo(hDC, rcItem.left, rcItem.top);
    _LineTo(hDC, rcItem.right - 1, rcItem.top);
    _clr = (GetThemePen(Dark));
    _LineTo(hDC, rcItem.right - 1, rcItem.bottom - 1);
    _LineTo(hDC, rcItem.left, rcItem.bottom - 1);
  }
  return 0;
}
int DoPaintPanel(HDDC hDC, RECT rcItem)
{
  DoPaintFrame(hDC, rcItem, UICOLOR_TITLE_BORDER_LIGHT, UICOLOR_TITLE_BORDER_DARK, UICOLOR_TITLE_BACKGROUND, 0);
  return 0;
}
int clip_point(int n, POINT* pt, int cx, int cy)
{
  int i;
  for (i = 0; i < n; ++i) {
    pt[i].x = BOUND(pt[i].x, 0, cx);
    pt[i].y = BOUND(pt[i].y, 0, cy);
  }
  return 0;
}
// Paint image
int DoPaintBitmap(HDDC hDC, img_t* im, RECT rc)
{
  int i, j, w, h, al;
  DWORD* ptr;
  clip_point(2, (POINT*)&rc, hDC->w, hDC->h);
  ptr = &_Pixel(hDC, rc.left, rc.top);
  w = RCW(&rc), h = RCH(&rc), al = hDC->bw >> 2;
  if (4 == im->c) {
    for (i = 0; i < h; ++i) {
      unsigned char* src = im->data + im->s * i;
      DWORD* dst = ptr + al * i;
      for (j = 0; j < w; ++j, src += 2) {
        DWORD clr = _RGB(src[0], src[1], src[2]);
        dst[j] = RGBBLEN(dst[j], clr, src[3]);
      }
    }
  }
  else if (3 == im->c) {
    for (i = 0; i < h; ++i) {
      unsigned char* src = im->data + im->s * i;
      DWORD* dst = ptr + al * i;
      for (j = 0; j < w; ++j, src += 3) {
        dst[j] = _RGB(src[0], src[1], src[2]);
      }
    }
  }
  else if (2 == im->c) {
    for (i = 0; i < h; ++i) {
      unsigned char* src = im->data + im->s * i;
      DWORD* dst = ptr + al * i;
      for (j = 0; j < w; ++j, src += 2) {
        DWORD clr = _RGB(src[0], src[0], src[0]);
        dst[j] = RGBBLEN(dst[j], clr, src[1]);
      }
    }
  }
  else if (1 == im->c) {
    for (i = 0; i < h; ++i) {
      unsigned char* src = im->data + im->s * i;
      DWORD* dst = ptr + al * i;
      for (j = 0; j < w; ++j, src += 1) {
        dst[j] = _RGB(src[0], src[0], src[0]);
      }
    }
  }
  else {
    ASSERT(0);
  }
  return 0;
}
int DoPaintImageList(HDDC hDC, const img_t* im, int i, int x, int y)
{
  RECT rc = iRECT(x, y, x + im->w, y + im->h);
  img_t im2[1];
  ASSERT(i < im->f);
  *im2 = *im, im2->f = 1, im2->data = im->data + i * im->s * im->h;
  DoPaintBitmap(hDC, im2, rc);
  return 0;
}
int DoPaintAlphaBitmap(HDDC hDC, img_t* im, RECT rc, BYTE iAlpha)
{
  int i, j, w, h, al, _a;
  DWORD* ptr, clr;
  clip_point(2, (POINT*)&rc, hDC->w, hDC->h);
  ptr = &_Pixel(hDC, rc.left, rc.top);
  w = RCW(&rc), h = RCH(&rc), al = hDC->bw >> 2;
  if (4 == im->c) {
    for (i = 0; i < h; ++i) {
      unsigned char* src = im->data + im->s * i;
      DWORD* dst = ptr + al * i;
      for (j = 0; j < w; ++j, src += 2) {
        clr = _RGB(src[0], src[1], src[2]);
        _a = (src[3] * iAlpha) / 255;
        dst[j] = RGBBLEN(dst[j], clr, _a);
      }
    }
  }
  else if (3 == im->c) {
    for (i = 0; i < h; ++i) {
      unsigned char* src = im->data + im->s * i;
      DWORD* dst = ptr + al * i;
      for (j = 0; j < w; ++j, src += 3) {
        clr = _RGB(src[0], src[1], src[2]);
        dst[j] = RGBBLEN(dst[j], clr, iAlpha);
      }
    }
  }
  else if (2 == im->c) {
    for (i = 0; i < h; ++i) {
      unsigned char* src = im->data + im->s * i;
      DWORD* dst = ptr + al * i;
      for (j = 0; j < w; ++j, src += 2) {
        clr = _RGB(src[0], src[0], src[0]);
        _a = (src[1] * iAlpha) / 255;
        dst[j] = RGBBLEN(dst[j], clr, _a);
      }
    }
  }
  else if (1 == im->c) {
    for (i = 0; i < h; ++i) {
      unsigned char* src = im->data + im->s * i;
      DWORD* dst = ptr + al * i;
      for (j = 0; j < w; ++j, src += 1) {
        clr = _RGB(src[0], src[0], src[0]);
        dst[j] = RGBBLEN(dst[j], clr, iAlpha);
      }
    }
  }
  else {
    ASSERT(0);
  }
  return 0;
}
// The string formatter supports a kind of "mini-html" that consists of various short tags:
//
// Link: <a>text</a>
// Change font: <f x> where x = font id
// Bold: <b>text</b>
// Indent: <x i> where i = indent in pixels
// Paragraph: <p>
// Horizontal line: <h>
// Icon: <i x y> where x = icon id and (optional) y = size (16/32/50)
// <i x> where x = icon resource name
// Color: <c #xxxxxx> where x = RGB in hex
// <c x> where x = color id
//
// In addition the standard string resource formats apply:
//
// %{n} where n = resource-string-id
int DoPaintPrettyText(HDDC hDC, RECT* prc, LPCTSTR pstrText, int iTextColor, int iBackColor,
    RECT* prcLinks, int* pnLinkRects, UINT uStyle, UINT uState)
{
  BOOL bDraw;
  RECT rcClip = { 0 };
  int i;
  RECT rc = *prc;
  DWORD _clr;
  int _x0, _y0;
  if (IsRectEmpty(&rc)) {
    return 0;
  }
  bDraw = (uStyle & DT_CALCRECT) == 0;
  //CStdString sText = pstrText;
  //sText.ProcessResourceTokens();
  //pstrText = sText;
  SelectThemeFontToMemDC(UIFONT_NORMAL);
  _clr = (GetThemeColor(iTextColor));
  // If the drawstyle includes an alignment, we'll need to first determine the text-size so
  // we can draw it at the correct position...
  if ((uStyle & DT_SINGLELINE) != 0 && (uStyle & DT_VCENTER) != 0 && (uStyle & DT_CALCRECT) == 0) {
    RECT rcText = { 0, 0, 9999, 100 };
    int nLinks = 0;
    DoPaintPrettyText(hDC, &rcText, pstrText, iTextColor, iBackColor, NULL, &nLinks, uStyle | DT_CALCRECT, uState);
    rc.top = rc.top + ((rc.bottom - rc.top) / 2) - ((rcText.bottom - rcText.top) / 2);
    rc.bottom = rc.top + (rcText.bottom - rcText.top);
  }
  if ((uStyle & DT_SINGLELINE) != 0 && (uStyle & DT_CENTER) != 0 && (uStyle & DT_CALCRECT) == 0) {
    RECT rcText = { 0, 0, 9999, 100 };
    int nLinks = 0;
    DoPaintPrettyText(hDC, &rcText, pstrText, iTextColor, iBackColor, NULL, &nLinks, uStyle | DT_CALCRECT, uState);
    OffsetRect(&rc, (rc.right - rc.left) / 2 - (rcText.right - rcText.left) / 2, 0);
  }
  if ((uStyle & DT_SINGLELINE) != 0 && (uStyle & DT_RIGHT) != 0 && (uStyle & DT_CALCRECT) == 0) {
    RECT rcText = { 0, 0, 9999, 100 };
    int nLinks = 0;
    DoPaintPrettyText(hDC, &rcText, pstrText, iTextColor, iBackColor, NULL, &nLinks, uStyle | DT_CALCRECT, uState);
    rc.left = rc.right - (rcText.right - rcText.left);
  }
  // Paint backhground
  if (iBackColor != UICOLOR__INVALID) {
    DoFillRect(hDC, rc, iBackColor);
  }
  // Determine if we're hovering over a link, because we would like to
  // indicate it by coloring the link text.
  // BUG: This assumes that the prcLink has already been filled once with
  // link coordinates! That is usually not the case at first repaint. We'll clear
  // the remanining entries at exit.
  {
    TEXTMETRIC tm = *GetThemeFontInfo(UIFONT_NORMAL);
    POINT pt = { rc.left, rc.top };
    SIZE sz = { RCW(&rc), RCH(&rc) };
    int iLineIndent = 0;
    int iLinkIndex = 0;
    int cyLine = tm.tmHeight + tm.tmExternalLeading;
    int cyMinHeight = 0;
    POINT ptLinkStart = { 0 };
    BOOL bInLink = FALSE;
    UINT uFormat = DT_VCENTER;
    while (*pstrText != '\0') {
      if (pt.x >= rc.right || *pstrText == '\n') {
        // A new link was detected/requested. We'll adjust the line height
        // for the next line and expand the link hitbox (if any)
        if (bInLink && iLinkIndex < *pnLinkRects) {
          SetRect(&prcLinks[iLinkIndex++], ptLinkStart.x, ptLinkStart.y, pt.x, pt.y + tm.tmHeight);
        }
        if ((uStyle & DT_SINGLELINE) != 0) {
          break;
        }
        if (*pstrText == '\n') {
          pstrText++;
        }
        pt.x = rc.left + iLineIndent;
        pt.y += cyLine - tm.tmDescent;
        ptLinkStart = pt;
        cyLine = tm.tmHeight + tm.tmExternalLeading;
        if (pt.x >= rc.right) {
          break;
        }
        while (*pstrText == ' ') {
          pstrText++;
        }
      }
      else if (*pstrText == '<'
          && (pstrText[1] >= 'a' && pstrText[1] <= 'z')
          && (pstrText[2] == ' ' || pstrText[2] == '>')) {
        pstrText++;
        switch (*pstrText++) {
        case 'a': { // Link
          _clr = (GetThemeColor(uState & UIS_HOT ? UICOLOR_LINK_TEXT_HOVER : UICOLOR_LINK_TEXT_NORMAL));
          SelectThemeFontToMemDC((UIFONT_LINK));
          tm = *GetThemeFontInfo(UIFONT_LINK);
          cyLine = MAX(cyLine, tm.tmHeight + tm.tmExternalLeading);
          ptLinkStart = pt;
          bInLink = TRUE;
        }
        break;
        case 'f': { // Font
          UITYPE_FONT iFont = (UITYPE_FONT) _tcstol(pstrText, (LPTSTR*)(&pstrText), 10);
          SelectThemeFontToMemDC(iFont);
          tm = *GetThemeFontInfo(iFont);
          cyLine = MAX(cyLine, tm.tmHeight + tm.tmExternalLeading);
        }
        break;
        case 'b': { // Bold text
          SelectThemeFontToMemDC((UIFONT_BOLD));
          tm = *GetThemeFontInfo(UIFONT_BOLD);
          cyLine = MAX(cyLine, tm.tmHeight + tm.tmExternalLeading);
        }
        break;
        case 'x': { // Indent
          iLineIndent = (int) _tcstol(pstrText, (LPTSTR*)(&pstrText), 10);
          if (pt.x < rc.left + iLineIndent) {
            pt.x = rc.left + iLineIndent;
          }
        }
        break;
        case 'p': { // Paragraph
          pt.x = rc.right;
          cyLine = MAX(cyLine, tm.tmHeight + tm.tmExternalLeading) + 5;
          iLineIndent = 0;
          SelectThemeFontToMemDC((UIFONT_NORMAL));
          _clr = (GetThemeColor(iTextColor));
          tm = *GetThemeFontInfo(UIFONT_NORMAL);
        }
        break;
        case 'h': { // Horizontal line
          _clr = (GetThemePen(UICOLOR_STANDARD_GREY));
          if (bDraw) {
            POINT ptTemp = { 0 };
            TMoveTo(hDC, pt.x, pt.y + 5);
            _LineTo(hDC, rc.right - iLineIndent, pt.y + 5);
          }
          cyLine = 12;
        }
        break;
        case 'i': { // Icon
#if 0
          int iSize = 16;
          if (*pstrText == ' ') {
            pstrText++;
          }
          if (isdigit(*pstrText)) {
            int iIndex = (int) _tcstol(pstrText, (LPTSTR*)(&pstrText), 10);
            iSize = MAX(16, _ttoi(pstrText));
            if (bDraw) {
              img_t hIcon[1] = {0};
              GetThemeIcon(iIndex, iSize, hIcon);
              ASSERT(hIcon != NULL);
              DrawIconEx(hDC, pt.x, pt.y, hIcon, iSize, iSize, 0, NULL, DI_NORMAL);
              DestroyIcon(hIcon);
            }
          }
          else {
            ICONINFO ii = { 0 };
            BITMAP bi = { 0 };
            img_t hIcon;
            hIcon = GetImage(pstrText);
            ASSERT(hIcon != NULL);
            GetIconInfo(hIcon, &ii);
            GetObject(ii.hbmColor, sizeof(BITMAP), &bi);
            iSize = bi.bmWidth;
            if (bDraw) {
              DrawIconEx(hDC, pt.x, pt.y, hIcon, iSize, iSize, 0, NULL, DI_NORMAL);
            }
            DestroyIcon(hIcon);
          }
          // A special feature with an icon at the left edge is that it also sets
          // the paragraph indent.
          if (pt.x == rc.left) {
            iLineIndent = iSize + (iSize / 8);
          }
          else {
            cyLine = MAX(iSize, cyLine);
          }
          pt.x += iSize + (iSize / 8);
          cyMinHeight = pt.y + iSize;
#endif
        }
        break;
        case 'c': { // Color
          if (*pstrText == ' ') {
            pstrText++;
          }
          if (*pstrText == '#') {
            COLORREF clrColor;
            pstrText++;
            clrColor = _tcstol(pstrText, (LPTSTR*)(&pstrText), 16);
            clrColor = _RGB(GetBValue(clrColor), GetGValue(clrColor), GetRValue(clrColor));
            _clr = (clrColor);
          }
          else {
            UITYPE_COLOR Color = (UITYPE_COLOR) _tcstol(pstrText, (LPTSTR*)(&pstrText), 10);
            _clr = (GetThemeColor(Color));
          }
        }
        break;
        }
        while (*pstrText != '\0' && *pstrText != '>') {
          pstrText++;
        }
        pstrText++;
      }
      else if (*pstrText == '<' && pstrText[1] == '/') {
        pstrText += 2;
        switch (*pstrText++) {
        case 'a':
          if (iLinkIndex < *pnLinkRects) {
            SetRect(&prcLinks[iLinkIndex++], ptLinkStart.x, ptLinkStart.y, pt.x, pt.y + tm.tmHeight + tm.tmExternalLeading);
          }
          _clr = (GetThemeColor(iTextColor));
          SelectThemeFontToMemDC((UIFONT_NORMAL));
          tm = *GetThemeFontInfo(UIFONT_NORMAL);
          bInLink = FALSE;
          break;
        case 'f':
        case 'b':
          // TODO: Use a context stack instead
          SelectThemeFontToMemDC((UIFONT_NORMAL));
          tm = *GetThemeFontInfo(UIFONT_NORMAL);
          break;
        case 'c':
          _clr = (GetThemeColor(iTextColor));
          break;
        }
        while (*pstrText != '\0' && *pstrText != '>') {
          pstrText++;
        }
        pstrText++;
      }
      else if (*pstrText == '&') {
        if ((uStyle & DT_NOPREFIX) == 0) {
          if (bDraw) {
            draw_text(hDC, _T("_"), 1, pt.x, pt.y, sz.w, sz.h, uFormat, _clr);
          }
        }
        else {
          SIZE szChar = { 0 };
          GetTextExtentPoint32(GetMemDC(), _T("&"), 1, &szChar);
          if (bDraw) {
            draw_text(hDC, _T("&"), 1, pt.x, pt.y, sz.w, sz.h, uFormat, _clr);
          }
          pt.x += szChar.w;
        }
        pstrText++;
      }
      else if (*pstrText == ' ') {
        SIZE szSpace = { 0 };
        GetTextExtentPoint32(GetMemDC(), _T(" "), 1, &szSpace);
        // Still need to paint the space because the font might have
        // underline formatting.
        if (bDraw) {
          draw_text(hDC, _T(" "), 1, pt.x, pt.y, sz.w, sz.h, uFormat, _clr);
        }
        pt.x += szSpace.w;
        pstrText++;
      }
      else {
        POINT ptPos = pt;
        int cchChars = 0;
        int cchLastGoodWord = 0;
        LPCTSTR p = pstrText;
        SIZE szText = { 0 };
        if (*p == '<') {
          p++, cchChars++;
        }
        while (*p != '\0' && *p != '<' && *p != '\n' && *p != '&') {
          // This part makes sure that we're word-wrapping if needed or providing support
          // for DT_END_ELLIPSIS. Unfortunately the GetTextExtentPoint32() call is pretty
          // slow when repeated so often.
          // TODO: Rewrite and use GetTextExtentExPoint() instead!
          cchChars++;
          szText.w = cchChars * tm.tmMaxCharWidth;
          if (pt.x + szText.w >= rc.right) {
            GetTextExtentPoint32(GetMemDC(), pstrText, cchChars, &szText);
          }
          if (pt.x + szText.w >= rc.right) {
            if ((uStyle & DT_WORDBREAK) != 0 && cchLastGoodWord > 0) {
              cchChars = cchLastGoodWord;
              pt.x = rc.right;
            }
            if ((uStyle & DT_END_ELLIPSIS) != 0 && cchChars > 2) {
              cchChars -= 2;
              pt.x = rc.right;
            }
            break;
          }
          if (*p == ' ') {
            cchLastGoodWord = cchChars;
          }
          //p = CharNext(p);
          ++p;
        }
        if (cchChars > 0) {
          GetTextExtentPoint32(GetMemDC(), pstrText, cchChars, &szText);
          if (bDraw) {
            draw_text(hDC, pstrText, cchChars, ptPos.x, ptPos.y, sz.w, sz.h, uFormat, _clr);
            if (pt.x == rc.right && (uStyle & DT_END_ELLIPSIS) != 0) {
              draw_text(hDC, _T("..."), 3, rc.right - 10, ptPos.y, sz.w, sz.h, uFormat, _clr);
            }
          }
          pt.x += szText.w;
          pstrText += cchChars;
        }
      }
      ASSERT(iLinkIndex <= *pnLinkRects);
    }
    // Clear remaining link rects and return number of used rects
    for (i = iLinkIndex; i < *pnLinkRects; i++) {
      ZeroMemory(prcLinks + i, sizeof(RECT));
    }
    *pnLinkRects = iLinkIndex;
    // Return size of text when requested
    if ((uStyle & DT_CALCRECT) != 0) {
      rc.bottom = MAX(cyMinHeight, pt.y + cyLine);
      if (rc.right >= 9999) {
        if (_tcslen(pstrText) > 0) {
          pt.x += 3;
        }
        rc.right = pt.x;
      }
    }
  }
  *prc = rc;
  return 0;
}
int DoPaintArcCaption(HDDC hDC, RECT rc, LPCTSTR pstrText, UINT uStyle)
{
  RECT rcText, rcButton1, rcButton2, rcButton3, rc1;
  DWORD _clr;
  int d = 5;
  _clr = (GetThemePen(UICOLOR_TASK_CAPTION));
  rc1 = rc, rc1.bottom += 15;
  InflateRect(&rc1, -d, -d);
  draw_roundrectR(hDC, &rc1, d, d, _clr, 1);
  rcText = iRECT(rc.left, rc.top + 3, rc.right, rc.bottom);
  if ((uStyle & UIARC_GRIPPER) != 0) {
    rcButton1 = iRECT(rc.left + 10, rc.top + 4, rc.left + 14, rc.top + 7);
    DoPaintFrame(hDC, rcButton1, UICOLOR_TITLE_BORDER_LIGHT, UICOLOR_TITLE_BORDER_DARK, UICOLOR__INVALID, 0);
    rcButton2 = iRECT(rc.left + 6, rc.top + 8, rc.left + 10, rc.top + 11);
    DoPaintFrame(hDC, rcButton2, UICOLOR_TITLE_BORDER_LIGHT, UICOLOR_TITLE_BORDER_DARK, UICOLOR__INVALID, 0);
    rcButton3 = iRECT(rc.left + 10, rc.top + 12, rc.left + 14, rc.top + 15);
    DoPaintFrame(hDC, rcButton3, UICOLOR_TITLE_BORDER_LIGHT, UICOLOR_TITLE_BORDER_DARK, UICOLOR__INVALID, 0);
    rcText.left += 12;
  }
  rcText.left += 8;
  if (pstrText != NULL && _tcslen(pstrText) > 0) {
    int nLinks = 0;
    DoPaintPrettyText(hDC, &rcText, pstrText, UICOLOR_TITLE_TEXT, UICOLOR__INVALID, NULL, &nLinks, DT_SINGLELINE, 0);
  }
  return 0;
}
int DoPaintEditBox(HDDC hDC, RECT rcItem, LPCTSTR pstrText, UINT uState, UINT uDrawStyle, BOOL bPaintFrameOnly)
{
  RECT rcEdit;
  COLORREF clrText;
  COLORREF _clr;
  if ((uState & UIS_DISABLED) != 0) {
    DoPaintFrame(hDC, rcItem, UICOLOR_CONTROL_BORDER_DISABLED, UICOLOR_CONTROL_BORDER_DISABLED, UICOLOR_EDIT_BACKGROUND_DISABLED, 0);
  }
  else if ((uState & UIS_READONLY) != 0) {
    DoPaintFrame(hDC, rcItem, UICOLOR_CONTROL_BORDER_DISABLED, UICOLOR_CONTROL_BORDER_DISABLED, UICOLOR_EDIT_BACKGROUND_READONLY, 0);
  }
  else {
    DoPaintFrame(hDC, rcItem, UICOLOR_CONTROL_BORDER_NORMAL, UICOLOR_CONTROL_BORDER_NORMAL, UICOLOR_EDIT_BACKGROUND_NORMAL, 0);
  }
  if (bPaintFrameOnly) {
    return 0;
  }
  // We should also draw the actual text
  clrText = GetThemeColor(UICOLOR_EDIT_TEXT_NORMAL);
  if ((uState & UIS_READONLY) != 0) {
    clrText = GetThemeColor(UICOLOR_EDIT_TEXT_READONLY);
  }
  if ((uState & UIS_DISABLED) != 0) {
    clrText = GetThemeColor(UICOLOR_EDIT_TEXT_DISABLED);
  }
  _clr = (clrText);
  SelectThemeFontToMemDC((UIFONT_NORMAL));
  rcEdit = rcItem;
  InflateRect(&rcEdit, -3, -2);
  draw_textR(hDC, pstrText, -1, &rcEdit, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_EDITCONTROL | uDrawStyle, _clr);
  return 0;
}
int DoPaintOptionBox(HDDC hDC, RECT rcItem, LPCTSTR pstrText, UINT uState, UINT uStyle)
{
  RECT rcText, rcButton;
  BOOL bSelected;
  int iIcon;
  int nLinks;
  //img_t hIcon;
  UITYPE_COLOR iTextColor;
  ASSERT(GetObjectType(hDC) == OBJ_DC || GetObjectType(hDC) == OBJ_MEMDC);
  // Determine placement of elements
  rcText = rcItem;
  rcButton = rcItem;
  if ((uStyle & DT_RIGHT) != 0) {
    rcText.right -= 18;
    rcButton.left = rcButton.right - 18;
  }
  else {
    rcText.left += 18;
    rcButton.right = rcButton.left + 18;
  }
  bSelected = (uState & UIS_CHECKED) != 0;
  iIcon = bSelected ? 8 : 9;
  if ((uState & UIS_PUSHED) != 0) {
    iIcon = 10;
  }
  if ((uState & UIS_DISABLED) != 0) {
    iIcon = bSelected ? 10 : 11;
  }
#if 0
  hIcon = GetThemeIcon(iIcon, 16);
  DrawIconEx(hDC, rcButton.left, rcButton.top, hIcon, 16, 16, 0, NULL, DI_NORMAL);
  DestroyIcon(hIcon);
#endif
  // Paint text
  iTextColor = ((uState & UIS_DISABLED) != 0) ? UICOLOR_EDIT_TEXT_DISABLED : UICOLOR_EDIT_TEXT_NORMAL;
  nLinks = 0;
  DoPaintPrettyText(hDC, &rcText, pstrText, iTextColor, UICOLOR__INVALID, NULL, &nLinks, DT_SINGLELINE, uState);
  // Paint focus rectangle
  if (((uState & UIS_FOCUSED) != 0)) {
    RECT rcFocus = { 0, 0, 9999, 9999 };
    int nLinks = 0;
    DoPaintPrettyText(hDC, &rcFocus, pstrText, iTextColor, UICOLOR__INVALID, NULL, &nLinks, DT_SINGLELINE | DT_CALCRECT, uState);
    rcText.right = rcText.left + (rcFocus.right - rcFocus.left);
    rcText.bottom = rcText.top + (rcFocus.bottom - rcFocus.top);
    InflateRect(&rcText, 2, 0);
    DoPaintFrame(hDC, rcText, UICOLOR_STANDARD_BLACK, UICOLOR_STANDARD_BLACK, UICOLOR__INVALID, UIFRAME_FOCUS);
  }
  return 0;
}
int ProcessResourceTokens(LPCTSTR pstrText)
{
  // Replace string-tokens: %{nnn} where nnn is a resource string identifier
  char* iPos = strchr(pstrText, '%');
  while (iPos) {
    if (iPos[1] == '{') {
      char* iEndPos = iPos + 2;
      while (isdigit(*iEndPos)) {
        iEndPos++;
      }
      if (*iEndPos == '}') {
        //CStdString sTemp = CStdString::RES((UINT)_ttoi(m_pstr + iPos + 2));
        //Replace(Mid(iPos, iEndPos - iPos + 1), sTemp);
      }
    }
    iPos = strchr(iPos + 1, '%');
  }
  return 0;
}
int DoPaintTabFolder(HDDC hDC, RECT* prcItem, LPCTSTR pstrText, UINT uState)
{
  LPCTSTR sText = pstrText;
  int cchText = _tcslen(pstrText);
  SIZE szText = { 0 };
  RECT rcTab = { 0 };
  DWORD _clr;
  int _x0, _y0;
  SelectThemeFontToMemDC((UIFONT_NORMAL));
  ProcessResourceTokens(sText);
  GetTextExtentPoint32(GetMemDC(), sText, cchText, &szText);
  if ((uState & UIS_PUSHED) != 0) {
    RECT rcText;
    RECT rcTop;
    int nLinks = 0;
    SetRect(&rcTab, prcItem->left, prcItem->top + 1, prcItem->left + szText.w + 14, prcItem->bottom);
    DoFillRect(hDC, rcTab, UICOLOR_TAB_BACKGROUND_NORMAL);
    _clr = (GetThemePen(UICOLOR_TAB_BORDER));
    TMoveTo(hDC, rcTab.left, rcTab.bottom);
    _LineTo(hDC, rcTab.left, rcTab.top + 2);
    _LineTo(hDC, rcTab.left + 1, rcTab.top + 1);
    _LineTo(hDC, rcTab.right - 1, rcTab.top + 1);
    _LineTo(hDC, rcTab.right, rcTab.top + 2);
    _LineTo(hDC, rcTab.right, rcTab.bottom);
    _clr = (GetThemePen(UICOLOR_TAB_BACKGROUND_NORMAL));
    _LineTo(hDC, rcTab.left, rcTab.bottom);
    rcText = iRECT(rcTab.left, rcTab.top + 1, rcTab.right, rcTab.bottom);
    DoPaintPrettyText(hDC, &rcText, pstrText, UICOLOR_TAB_TEXT_SELECTED, UICOLOR__INVALID, NULL, &nLinks, DT_SINGLELINE | DT_CENTER | DT_VCENTER, uState);
    rcTop = iRECT(rcTab.left + 1, rcTab.top, rcTab.right - 1, rcTab.top + 3);
    DoPaintGradient(hDC, rcTop, _RGB(222, 142, 41), _RGB(255, 199, 25), TRUE);
  }
  else if ((uState & UIS_DISABLED) != 0) {
  }
  else {
    COLORREF clrFolder1, clrFolder2;
    RECT rcText;
    int nLinks = 0;
    SetRect(&rcTab, prcItem->left, prcItem->top + 3, prcItem->left + szText.w + 12, prcItem->bottom);
    GetThemeColorPair(UICOLOR_TAB_FOLDER_NORMAL, &clrFolder1, &clrFolder2);
    DoPaintGradient(hDC, rcTab, clrFolder1, clrFolder2, TRUE);
    _clr = (GetThemePen(UICOLOR_TAB_BORDER));
    TMoveTo(hDC, rcTab.left, rcTab.bottom);
    _LineTo(hDC, rcTab.left, rcTab.top);
    _LineTo(hDC, rcTab.right, rcTab.top);
    _LineTo(hDC, rcTab.right, rcTab.bottom);
    rcText = iRECT(rcTab.left, rcTab.top + 3, rcTab.right, rcTab.bottom);
    DoPaintPrettyText(hDC, &rcText, pstrText, UICOLOR_TAB_TEXT_NORMAL, UICOLOR__INVALID, NULL, &nLinks, DT_SINGLELINE | DT_CENTER | DT_VCENTER, uState);
  }
  *prcItem = rcTab;
  return 0;
}
int DoPaintToolbarButton(HDDC hDC, RECT rc, LPCTSTR pstrText, SIZE szPadding, UINT uState)
{
  RECT rcText;
  int nLinks = 0;
  ASSERT(GetObjectType(hDC) == OBJ_DC || GetObjectType(hDC) == OBJ_MEMDC);
  if ((uState & UIS_PUSHED) != 0) {
    DoPaintFrame(hDC, rc, UICOLOR_TOOL_BORDER_PUSHED, UICOLOR_TOOL_BORDER_PUSHED, UICOLOR_TOOL_BACKGROUND_PUSHED, 0);
    rc.top += 2;
    rc.left++;
  }
  else if ((uState & UIS_HOT) != 0) {
    DoPaintFrame(hDC, rc, UICOLOR_TOOL_BORDER_HOVER, UICOLOR_TOOL_BORDER_HOVER, UICOLOR_TOOL_BACKGROUND_HOVER, 0);
  }
  else if ((uState & UIS_DISABLED) != 0) {
    // TODO
  }
  rcText = rc;
  InflateRect(&rcText, -szPadding.w, -szPadding.h);
  DoPaintPrettyText(hDC, &rcText, pstrText, UICOLOR_TITLE_TEXT, UICOLOR__INVALID, NULL, &nLinks, DT_SINGLELINE | DT_LEFT | DT_VCENTER, uState);
  return 0;
}
int DoPaintQuickText(HDDC hDC, RECT rc, LPCSTR pstrText, UITYPE_COLOR iTextColor, UITYPE_FONT iFont, UINT uStyle)
{
  DWORD _clr;
  _clr = (GetThemeColor(iTextColor));
  SelectThemeFontToMemDC(iFont);
  draw_textR(hDC, pstrText, -1, &rc, DT_VCENTER | DT_SINGLELINE, _clr);
  return 0;
}
typedef enum SIDE { SIDE_LEFT, SIDE_TOP, SIDE_RIGHT, SIDE_BOTTOM } SIDE;
static int DrawFrameC(HDDC hDC, const POINT* pPoints, int iCount, COLORREF clrLine)
{
  int i;
  COLORREF _clr = (clrLine);
  int _x0, _y0;
  //
  TMoveTo(hDC, pPoints[0].x, pPoints[0].y);
  for (i = 1; i < iCount; ++i) {
    _LineTo(hDC, pPoints[i].x, pPoints[i].y);
  }
  _SetPixel(hDC, pPoints[iCount - 1].x, pPoints[iCount - 1].y, clrLine); // draw last pixel.
  return 0;
}
static int DrawFrame(HDDC hDC, const POINT* pPoints, int iCount, COLORREF clrBorder, COLORREF clrBack)
{
  DPOINT pt[100];
  int i;
  for (i = 0; i < iCount; ++i) {
    pt[i] = dPOINT(pPoints[i].x, pPoints[i].y);
  }
  drawaa_fillpoly_solid(hDC, pt, &iCount, 1, clrBack);
  DrawFrameC(hDC, pPoints, iCount, clrBorder);
  return 0;
}
static int DrawHalfRoundFrame(HDDC hDC, const RECT* pRect, SIDE side, int radius, COLORREF clrBorder, COLORREF clrBack)
{
  POINT pts[6];
  switch (side) {
  case SIDE_LEFT:
    pts[0].x = pRect->right - 1;
    pts[0].y = pRect->top;
    pts[1].x = pRect->left + radius;
    pts[1].y = pRect->top;
    pts[2].x = pRect->left;
    pts[2].y = pRect->top + radius;
    pts[3].x = pRect->left;
    pts[3].y = pRect->bottom - 1 - radius;
    pts[4].x = pRect->left + radius;
    pts[4].y = pRect->bottom - 1;
    pts[5].x = pRect->right - 1;
    pts[5].y = pRect->bottom - 1;
    break;
  case SIDE_TOP:
    pts[0].x = pRect->left;
    pts[0].y = pRect->bottom - 1;
    pts[1].x = pRect->left;
    pts[1].y = pRect->top + radius;
    pts[2].x = pRect->left + radius;
    pts[2].y = pRect->top;
    pts[3].x = pRect->right - 1 - radius;
    pts[3].y = pRect->top;
    pts[4].x = pRect->right - 1;
    pts[4].y = pRect->top + radius;
    pts[5].x = pRect->right - 1;
    pts[5].y = pRect->bottom - 1;
    break;
  case SIDE_RIGHT:
    pts[0].x = pRect->left;
    pts[0].y = pRect->top;
    pts[1].x = pRect->right - 1 - radius;
    pts[1].y = pRect->top;
    pts[2].x = pRect->right - 1;
    pts[2].y = pRect->top + radius;
    pts[3].x = pRect->right - 1;
    pts[3].y = pRect->bottom - 1 - radius;
    pts[4].x = pRect->right - 1 - radius;
    pts[4].y = pRect->bottom - 1;
    pts[5].x = pRect->left;
    pts[5].y = pRect->bottom - 1;
    break;
  case SIDE_BOTTOM:
    pts[0].x = pRect->left;
    pts[0].y = pRect->top;
    pts[1].x = pRect->left;
    pts[1].y = pRect->bottom - 1 - radius;
    pts[2].x = pRect->left + radius;
    pts[2].y = pRect->bottom - 1;
    pts[3].x = pRect->right - 1 - radius;
    pts[3].y = pRect->bottom - 1;
    pts[4].x = pRect->right - 1;
    pts[4].y = pRect->bottom - 1 - radius;
    pts[5].x = pRect->right - 1;
    pts[5].y = pRect->top;
    break;
  }
  DrawFrame(hDC, pts, sizeof(pts) / sizeof(POINT), clrBorder, clrBack);
  return 0;
}
static int DrawRectC(HDDC hDC, int x1, int y1, int x2, int y2, COLORREF clrLine)
{
  COLORREF _clr = (clrLine);
  int _x0, _y0;
  TMoveTo(hDC, x1, y1);
  _LineTo(hDC, x1, y2);
  _LineTo(hDC, x2, y2);
  _LineTo(hDC, x2, y1);
  _LineTo(hDC, x1, y1);
  return 0;
}
static int DrawRectRC(HDDC hDC, const RECT* pRect, COLORREF clrLine)
{
  DrawRectC(hDC, pRect->left, pRect->top, pRect->right - 1, pRect->bottom - 1, clrLine);
  return 0;
}
static int DrawRectR(HDDC hDC, const RECT* pRect)
{
  COLORREF _clr = rgb(0, 0, 0);
  int _x0, _y0;
  TMoveTo(hDC, pRect->left, pRect->top);
  _LineTo(hDC, pRect->left, pRect->bottom - 1);
  _LineTo(hDC, pRect->right - 1, pRect->bottom - 1);
  _LineTo(hDC, pRect->right - 1, pRect->top);
  _LineTo(hDC, pRect->left, pRect->top);
  return 0;
}
static int DrawBeveledRect(HDDC hDC, const RECT* pRect, int bevel)
{
  COLORREF _clr = rgb(0, 0, 0);
  int _x0, _y0;
  TMoveTo(hDC, pRect->left, pRect->top + bevel);
  _LineTo(hDC, pRect->left, pRect->bottom - bevel - 1);
  _LineTo(hDC, pRect->left + bevel, pRect->bottom - 1);
  _LineTo(hDC, pRect->right - bevel - 1, pRect->bottom - 1);
  _LineTo(hDC, pRect->right - 1, pRect->bottom - bevel - 1);
  _LineTo(hDC, pRect->right - 1, pRect->top + bevel);
  _LineTo(hDC, pRect->right - bevel - 1, pRect->top);
  _LineTo(hDC, pRect->left + bevel, pRect->top);
  _LineTo(hDC, pRect->left, pRect->top + bevel);
  return 0;
}
static COLORREF PixelAlpha1(COLORREF src, COLORREF dst, int percent)
{
  int ipercent = 100 - percent;
  return _RGB(
      (GetRV(src) * percent + GetRV(dst) * ipercent) / 100,
      (GetGV(src) * percent + GetGV(dst) * ipercent) / 100,
      (GetBV(src) * percent + GetBV(dst) * ipercent) / 100
      );
}
static int FillSolidRectRC(HDDC hDC, const RECT* rc, COLORREF color)
{
  draw_fillrectR(hDC, rc, color);
  return 0;
}
static BOOL DrawMarker(HDDC hDC, const RECT* pRect, img_t* hbmpMask, COLORREF color)
{
  DoPaintBitmap(hDC, hbmpMask, *pRect);
  return 0;
}
int imgetframe(const img_t* im, int i, img_t* frame)
{
  if (i < im->f) {
    imsetsize(frame, im->h, im->w, im->c, 1);
    //memcpy2d(frame->data, frame->s, im->data + i*im->s*im->h, im->s, im->h, im->w);
    memcpy(frame->data, im->data + i * im->s * im->h, im->s * im->h);
  }
  return 0;
}
int imgetframeref(const img_t* im, int i, img_t* frame)
{
  if (i < im->f) {
    frame->data = im->data + i * im->s * im->h;
    frame->h = im->h, frame->w = im->w;
    frame->c = im->c, frame->s = im->s, frame->f = 1;
  }
  return 0;
}
static int DrawMarkerI(HDDC hDC, const RECT* pRect, const img_t* pImageList, int iImage, COLORREF color)
{
  BOOL res = FALSE;
  img_t hIcon[1] = {0};
  imgetframeref(pImageList, iImage, hIcon);
  // try to draw using mask.
  res = DrawMarker(hDC, pRect, hIcon, color);
  return 0;
}
// 创建背景的函数
int CEditWnd_MakeBKGround(HDDC hDC, COLORREF clrBegin, COLORREF clrEnd, RECT rect)
{
  int x1 = 0, y1 = 0;
  int x2 = 0, y2 = 0;
  int r1 = GetRValue(clrBegin), g1 = GetGValue(clrBegin), b1 = GetBValue(clrBegin);
  int r2 = GetRValue(clrEnd), g2 = GetGValue(clrEnd), b2 = GetBValue(clrEnd);
  int _x0, _y0;
  COLORREF _clr;
  while (x1 < RCW(&rect) && y1 < RCH(&rect)) {
    int r, g, b, i;
    if (y1 < RCH(&rect) - 1) {
      y1++;
    }
    else {
      x1++;
    }
    if (x2 < RCW(&rect) - 1) {
      x2++;
    }
    else {
      y2++;
    }
    i = x1 + y1;
    r = r1 + (i * (r2 - r1) / (RCW(&rect) + RCH(&rect)));
    g = g1 + (i * (g2 - g1) / (RCW(&rect) + RCH(&rect)));
    b = b1 + (i * (b2 - b1) / (RCW(&rect) + RCH(&rect)));
    _clr = _RGB(r, g, b);
    TMoveTo(hDC, rect.left + x1, rect.top + y1);
    _LineTo(hDC, rect.left + x2, rect.top + y2);
  }
  return 0;
}
int drawaa_edit_border(HDDC hDC, RECT rect)
{
  int n, r = 2;
  DPOINT pt[100];
  n = vcgen_roundrectR(pt, &rect, r);
  // 设置缺省背景
  //CEditWnd_MakeBKGround(BKdc, _RGB(148, 182, 225), _RGB(46, 74, 137), pMsg->rect);
  drawaa_fillpoly_gradient(hDC, pt, &n, 1, rect.left, rect.top, rect.left, rect.bottom, _RGB(255, 255, 225), _RGB(255, 255, 255));
  //drawaa_fillpoly_gradient(hDC, pt, &n, 1, rect.left, rect.top, rect.left, rect.bottom, _RGB(148, 182, 225), _RGB(46, 74, 137));
  drawaa_strokepoly(hDC, pt, n, 1, _RGB(135, 155, 200), 1); // 绘制窗体的边框
  return 0;
}
int CNavigatorPanelUI_DoPaint(HDDC hDC, RECT rcPaint)
{
  COLORREF clrFirst, clrSecond;
  GetThemeColorPair(UICOLOR_NAVIGATOR_BACKGROUND, &clrFirst, &clrSecond);
  DoPaintGradient(hDC, rcPaint, clrFirst, clrSecond, FALSE);
  return 0;
}
int CNavigatorButtonUI_DrawItem(HDDC hDC, RECT rcItem, LPCTSTR m_sText, UINT uState, UINT uStyle)
{
  UITYPE_COLOR iTextColor;
  RECT rcButton = (rcItem);
  RECT rcText;
  int nLinks = 0;
  int m_bSelected = uState & UIS_SELECTED;
  DWORD _clr;
  int _x0, _y0;
  if ((uState & UIS_PUSHED) != 0) {
    DoFillRect(hDC, rcButton, UICOLOR_NAVIGATOR_BUTTON_PUSHED);
  }
  else if (m_bSelected) {
    DoFillRect(hDC, rcButton, UICOLOR_NAVIGATOR_BUTTON_SELECTED);
  }
  else if ((uState & UIS_PUSHED) != 0) {
    DoFillRect(hDC, rcButton, UICOLOR_NAVIGATOR_BUTTON_HOVER);
  }
  _clr = (GetThemePen(m_bSelected ? UICOLOR_NAVIGATOR_BORDER_SELECTED : UICOLOR_NAVIGATOR_BORDER_NORMAL));
  TMoveTo(hDC, rcButton.left, rcButton.top);
  _LineTo(hDC, rcButton.left, rcButton.bottom);
  _LineTo(hDC, rcButton.right, rcButton.bottom);
  iTextColor = UICOLOR_NAVIGATOR_TEXT_NORMAL;
  if ((uState & UIS_PUSHED) != 0) {
    iTextColor = UICOLOR_NAVIGATOR_TEXT_PUSHED;
  }
  else if (m_bSelected) {
    iTextColor = UICOLOR_NAVIGATOR_TEXT_SELECTED;
  }
  rcText = rcButton;
  OffsetRect(&rcText, 0, 1);
  InflateRect(&rcText, -8, 0);
  if ((uState & UIS_PUSHED) != 0) {
    rcText.left++;
    rcText.top += 2;
  }
  DoPaintPrettyText(hDC, &rcText, m_sText, iTextColor, UICOLOR__INVALID, NULL, &nLinks, DT_SINGLELINE | DT_VCENTER, uState);
  return 0;
}
int COptionUI_DoPaint(HDDC hDC, RECT m_rcItem, LPCTSTR m_sText, UINT uState, UINT uStyle)
{
  // Draw button
  DoPaintOptionBox(hDC, m_rcItem, m_sText, uState, uStyle);
  return 0;
}
int CSingleLinePickUI_DoPaint(HDDC hDC, RECT m_rcItem, LPCTSTR m_sText, UINT uState, UINT uStyle)
{
  RECT rcPadding = { 0 };
  RECT m_rcLinks[8];
  int m_nLinks;
  int cy = m_rcItem.bottom - m_rcItem.top;
  RECT rcText, m_rcButton;
  UITYPE_COLOR iTextColor = UICOLOR_EDIT_TEXT_NORMAL;
  UITYPE_COLOR iBorderColor = UICOLOR_CONTROL_BORDER_NORMAL;
  UITYPE_COLOR iBackColor = UICOLOR_CONTROL_BACKGROUND_NORMAL;
  SetRect(&m_rcButton, m_rcItem.right - cy, m_rcItem.top, m_rcItem.right, m_rcItem.bottom);
  SetRect(&rcText, m_rcItem.left, m_rcItem.top, m_rcButton.left - 4, m_rcItem.bottom);
  if (UIS_FOCUSED & uState) {
    iTextColor = UICOLOR_EDIT_TEXT_NORMAL;
    iBorderColor = UICOLOR_CONTROL_BORDER_NORMAL;
    iBackColor = UICOLOR_CONTROL_BACKGROUND_HOVER;
  }
  if (UIS_DISABLED & uState) {
    iTextColor = UICOLOR_EDIT_TEXT_DISABLED;
    iBorderColor = UICOLOR_CONTROL_BORDER_DISABLED;
    iBackColor = UICOLOR__INVALID;
  }
  DoPaintFrame(hDC, rcText, iBorderColor, iBorderColor, iBackColor, 0);
  InflateRect(&rcText, -4, -2);
  m_nLinks = lengthof(m_rcLinks);
  DoPaintPrettyText(hDC, &rcText, m_sText, iTextColor, UICOLOR__INVALID, m_rcLinks, &m_nLinks, DT_SINGLELINE, uState);
  //DoPaintButton(hDC, m_rcButton, _T("<i 4>"), rcPadding, uState, 0);
  return 0;
}
int CCanvasUI_DoPaint(HDDC hDC, RECT m_rcItem, img_t* m_hBitmap, int m_iOrientation)
{
  COLORREF m_clrBack;
  BITMAP m_BitmapInfo = {0};
  int iclrBack;
  // Fill background
  RECT rcFill = { 0 };
  iclrBack = UICOLOR_WINDOW_BACKGROUND;
  iclrBack = UICOLOR_CONTROL_BACKGROUND_NORMAL;
  iclrBack = UICOLOR_STANDARD_WHITE;
  iclrBack = UICOLOR_DIALOG_BACKGROUND;
  m_clrBack = GetThemeColor(iclrBack);
  if (IntersectRect(&rcFill, &m_rcItem, &m_rcItem)) {
    DoFillRect(hDC, rcFill, m_clrBack);
  }
  // Paint watermark bitmap
  if (m_hBitmap != NULL) {
    RECT rcBitmap = { 0 };
    RECT rcTemp = { 0 };
    switch (m_iOrientation) {
    case HTTOPRIGHT:
      SetRect(&rcBitmap, m_rcItem.right - m_BitmapInfo.bmWidth, m_rcItem.top, m_rcItem.right, m_rcItem.top + m_BitmapInfo.bmHeight);
      break;
    case HTBOTTOMRIGHT:
      SetRect(&rcBitmap, m_rcItem.right - m_BitmapInfo.bmWidth, m_rcItem.bottom - m_BitmapInfo.bmHeight, m_rcItem.right, m_rcItem.bottom);
      break;
    default:
      SetRect(&rcBitmap, m_rcItem.right - m_BitmapInfo.bmWidth, m_rcItem.bottom - m_BitmapInfo.bmHeight, m_rcItem.right, m_rcItem.bottom);
      break;
    }
    if (IntersectRect(&rcTemp, &m_rcItem, &rcBitmap)) {
      DoPaintBitmap(hDC, m_hBitmap, rcBitmap);
    }
  }
  return 0;
}
int CTitleShadowUI_DoPaint(HDDC hDC, RECT m_rcItem)
{
  COLORREF clrBack1 = GetThemeColor(UICOLOR_TITLE_BACKGROUND);
  COLORREF clrBack2 = GetThemeColor(UICOLOR_DIALOG_BACKGROUND);
  RECT rcTop = { m_rcItem.left, m_rcItem.top, m_rcItem.right, m_rcItem.top + 4 };
  DoPaintGradient(hDC, rcTop, clrBack1, clrBack2, TRUE);
  return 0;
}
int CListHeaderShadowUI_DoPaint(HDDC hDC, RECT m_rcItem)
{
  COLORREF clrBack1, clrBack2;
  RECT rcTop;
  GetThemeColorPair(UICOLOR_HEADER_BACKGROUND, &clrBack1, &clrBack2);
  SetRect(&rcTop, m_rcItem.left + 1, m_rcItem.top, m_rcItem.right - 1, m_rcItem.top + 4);
  DoPaintGradient(hDC, rcTop, clrBack2, clrBack1, TRUE);
  return 0;
}
int DoPaintLine(HDDC hDC, RECT rcItem, UITYPE_COLOR Color)
{
  int _x0, _y0;
  DWORD _clr = (GetThemePen(Color));
  TMoveTo(hDC, rcItem.left, rcItem.top);
  _LineTo(hDC, rcItem.right, rcItem.bottom);
  return 0;
}
int CFadedLineUI_DoPaint(HDDC hDC, RECT m_rcItem)
{
  RECT rc1, rc2;
  COLORREF clrLine = GetThemeColor(UICOLOR_NAVIGATOR_BORDER_NORMAL);
  COLORREF clrDialog = GetThemeColor(UICOLOR_DIALOG_BACKGROUND);
  SetRect(&rc1, m_rcItem.left, m_rcItem.top + 3, m_rcItem.right - 120, m_rcItem.top + 3);
  DoPaintLine(hDC, rc1, UICOLOR_NAVIGATOR_BORDER_NORMAL);
  SetRect(&rc2, m_rcItem.right - 120, m_rcItem.top + 3, m_rcItem.right - 40, m_rcItem.top + 4);
  DoPaintGradient(hDC, rc2, clrLine, clrDialog, FALSE);
  return 0;
}
int CSeparatorLineUI_DoPaint(HDDC hDC, RECT m_rcItem)
{
  RECT rc1, rc2;
  SetRect(&rc1, m_rcItem.left, m_rcItem.top + 3, m_rcItem.right - 1, m_rcItem.top + 3);
  DoPaintLine(hDC, rc1, UICOLOR_EDIT_TEXT_DISABLED);
  SetRect(&rc2, m_rcItem.left, m_rcItem.top + 4, m_rcItem.right, m_rcItem.top + 4);
  DoPaintLine(hDC, rc1, UICOLOR_EDIT_TEXT_DISABLED);
  return 0;
}
int CListHeaderUI_DoPaint(HDDC hDC, RECT m_rcItem)
{
  // Draw background
  COLORREF clrBack1, clrBack2;
  RECT rcBottom;
  GetThemeColorPair(UICOLOR_HEADER_BACKGROUND, &clrBack1, &clrBack2);
  DoPaintFrame(hDC, m_rcItem, UICOLOR_HEADER_BORDER, UICOLOR_HEADER_BORDER, UICOLOR_HEADER_BACKGROUND, 0);
  SetRect(&rcBottom, m_rcItem.left + 1, m_rcItem.bottom - 3, m_rcItem.right - 1, m_rcItem.bottom);
  DoPaintGradient(hDC, rcBottom, clrBack1, clrBack2, TRUE);
  return 0;
}
RECT GetThumbRect(RECT rc)
{
  RECT rc2;
  SetRect(&rc2, rc.right - 4, rc.top, rc.right, rc.bottom - 3);
  return rc2;
}
int CListHeaderItemUI_DoPaint(HDDC hDC, RECT m_rcItem, LPCTSTR m_sText)
{
  // Paint text (with some indent)
  int nLinks = 0;
  RECT rc1, rc2, rcThumb, rcMessage = m_rcItem;
  POINT ptTemp = { 0 };
  rcMessage.left += 6;
  rcMessage.bottom -= 1;
  DoPaintPrettyText(hDC, &rcMessage, m_sText, UICOLOR_HEADER_TEXT, UICOLOR__INVALID, NULL, &nLinks, DT_SINGLELINE | DT_VCENTER, 0);
  // Draw gripper
  rcThumb = GetThumbRect(m_rcItem);
  SetRect(&rc1, rcThumb.left + 2, rcThumb.top + 4, rcThumb.left + 2, rcThumb.bottom - 1);
  DoPaintLine(hDC, rc1, UICOLOR_HEADER_SEPARATOR);
  SetRect(&rc2, rcThumb.left + 3, rcThumb.top + 4, rcThumb.left + 3, rcThumb.bottom - 1);
  DoPaintLine(hDC, rc2, UICOLOR_STANDARD_WHITE);
  return 0;
}
int CListFooterUI_DoPaint(HDDC hDC, RECT m_rcItem)
{
  COLORREF clrBack1, clrBack2;
  RECT rcTop, rcBottom;
  GetThemeColorPair(UICOLOR_HEADER_BACKGROUND, &clrBack1, &clrBack2);
  DoPaintFrame(hDC, m_rcItem, UICOLOR_HEADER_BORDER, UICOLOR_HEADER_BORDER, UICOLOR_HEADER_BACKGROUND, 0);
  SetRect(&rcTop, m_rcItem.left + 1, m_rcItem.top, m_rcItem.right - 1, m_rcItem.top + 1);
  DoPaintGradient(hDC, rcTop, clrBack2, clrBack1, TRUE);
  SetRect(&rcBottom, m_rcItem.left + 1, m_rcItem.bottom - 3, m_rcItem.right - 1, m_rcItem.bottom);
  DoPaintGradient(hDC, rcBottom, clrBack1, clrBack2, TRUE);
  // Paint items as well...
  return 0;
}
int CListLabelElementUI_DrawItem(HDDC hDC, RECT rcItem, LPCTSTR m_sText,
    UINT uDrawStyle, UINT m_uButtonState, UINT m_uTextStyle, BOOL m_bSelected)
{
  RECT rcText;
  int nLinks = 0;
  UITYPE_COLOR iTextColor = UICOLOR_CONTROL_TEXT_NORMAL;
  UITYPE_COLOR iBackColor = UICOLOR__INVALID;
  if ((m_uButtonState & UIS_HOT) != 0) {
    iTextColor = UICOLOR_CONTROL_TEXT_NORMAL;
    iBackColor = UICOLOR_CONTROL_BACKGROUND_HOVER;
  }
  if (m_bSelected) {
    iTextColor = UICOLOR_CONTROL_TEXT_SELECTED;
    iBackColor = UICOLOR_CONTROL_BACKGROUND_SELECTED;
  }
#define UIDRAWSTYLE_INPLACE 0x00001
#define UIDRAWSTYLE_FOCUS 0x00002
  if ((uDrawStyle & UIDRAWSTYLE_INPLACE) != 0) {
    iTextColor = UICOLOR_CONTROL_TEXT_NORMAL;
    iBackColor = UICOLOR_CONTROL_BACKGROUND_NORMAL;
  }
  if ((uDrawStyle & UIDRAWSTYLE_FOCUS) != 0) {
    iTextColor = UICOLOR_CONTROL_TEXT_NORMAL;
    iBackColor = UICOLOR_CONTROL_BACKGROUND_HOVER;
  }
  if (UIS_DISABLED & m_uButtonState) {
    iTextColor = UICOLOR_CONTROL_TEXT_DISABLED;
    iBackColor = UICOLOR_CONTROL_BACKGROUND_DISABLED;
  }
  // Paint background (because we're vertically centering the text area
  // so it cannot paint the entire item rectangle)
  if (iBackColor != UICOLOR__INVALID) {
    DoFillRect(hDC, rcItem, iBackColor);
  }
  // Paint text
  rcText = rcItem;
  InflateRect(&rcText, -4, 0);
  DoPaintPrettyText(hDC, &rcText, m_sText, iTextColor, UICOLOR__INVALID, NULL, &nLinks, DT_SINGLELINE | m_uTextStyle, m_uButtonState);
  return 0;
}
#if 0
enum { FADE_TIMERID = 10 };
enum { FADE_DELAY = 500UL };
int CTaskPanelUI_DoPaint(HDDC hDC, RECT m_rcItem, const char* m_sText, RECT m_rcFade, img_t* m_hFadeBitmap, DWORD m_dwFadeTick)
{
  int cyFont;
  RECT rcArc;
  RECT rcClient;
  COLORREF clrFirst, clrSecond;
  // Handling gracefull fading of panel
  if (m_hFadeBitmap != NULL) {
    DWORD dwTimeDiff = timeGetTime() - m_dwFadeTick;
    BYTE iAlpha = (BYTE) CLAMP(255 - (long)(dwTimeDiff / (FADE_DELAY / 255.0)), 0, 255);
    DoPaintAlphaBitmap(hDC, m_hFadeBitmap, m_rcFade, iAlpha);
  }
  // A tiny panel (see explaination in EstimateSize()) is invisible
  if (m_rcItem.right - m_rcItem.left < 2) {
    return 0;
  }
  // Paint caption
  cyFont = GetThemeFontInfo(UIFONT_NORMAL)->tmHeight;
  rcArc = iRECT(m_rcItem.left, m_rcItem.top, m_rcItem.right, m_rcItem.top + cyFont + 6);
  DoPaintArcCaption(hDC, rcArc, m_sText, UIARC_GRIPPER);
  // Paint background
  rcClient = iRECT(m_rcItem.left, rcArc.bottom, m_rcItem.right, m_rcItem.bottom);
  GetThemeColorPair(UICOLOR_TASK_BACKGROUND, &clrFirst, &clrSecond);
  DoPaintGradient(hDC, rcClient, clrFirst, clrSecond, FALSE, 128);
  DoPaintFrame(hDC, rcClient, UICOLOR_TASK_CAPTION, UICOLOR_TASK_CAPTION, UICOLOR__INVALID, 0);
  // Paint elements
  return 0;
}
int CSearchTitlePanelUI_DoPaint(HDDC hDC, RECT m_rcItem)
{
  RECT rcFrame = { m_rcItem.left, m_rcItem.top + 34, m_rcItem.right, m_rcItem.bottom };
  RECT rcArc = { m_rcItem.left, m_rcItem.top + 14, m_rcItem.right, m_rcItem.top + 34 };
  RECT rcTemp = { 0 };
  DoPaintFrame(hDC, rcFrame, UICOLOR_HEADER_SEPARATOR, UICOLOR_HEADER_SEPARATOR, UICOLOR_HEADER_BACKGROUND, 0);
  if (IntersectRect(&rcTemp, &m_rcItem, &rcArc)) {
    DoPaintArcCaption(hDC, rcArc, _T(""), 0);
  }
  if (m_iIconIndex >= 0) {
    HICON hIcon = GetThemeIcon(m_iIconIndex, 50);
    DrawIconEx(hDC, m_rcItem.left + 5, m_rcItem.top, hIcon, 50, 50, 0, NULL, DI_NORMAL);
    DestroyIcon(hIcon);
  }
  return 0;
}
int CTextPanelUI_DoPaint(HDDC hDC, RECT m_rcItem)
{
  RECT rcText = m_rcItem;
  m_nLinks = lengthof(m_rcLinks);
  DoPaintPrettyText(hDC, rcText, m_sText, m_TextColor, m_BackColor, m_rcLinks, m_nLinks, m_uTextStyle);
  return 0;
}
int CWarningPanelUI_DoPaint(HDDC hDC, RECT m_rcItem)
{
  RECT rcSign = m_rcItem;
  rcSign.bottom -= 8;
  DoPaintFrame(hDC, rcSign, UICOLOR_STANDARD_GREY, UICOLOR_STANDARD_GREY, m_BackColor, 0);
  RECT rcText = rcSign;
  InflateRect(&rcText, -6, -4);
  m_nLinks = lengthof(m_rcLinks);
  DoPaintPrettyText(hDC, rcText, m_sText, UICOLOR_BUTTON_TEXT_NORMAL, UICOLOR__INVALID, m_rcLinks, m_nLinks, DT_WORDBREAK);
  return 0;
}
int CTabFolderUI_DoPaint(HDDC hDC, RECT m_rcItem)
{
  {
    CRenderClip clip;
    GenerateClip(hDC, m_rcItem, clip);
    // Fill client area background
    RECT rcFill = { 0 };
    IntersectRect(&rcFill, &m_rcItem, &m_rcClient);
    DoFillRect(hDC, rcFill, UICOLOR_TAB_BACKGROUND_NORMAL);
    // Frame around client area
    DoPaintRectangle(hDC, m_rcClient, UICOLOR_TAB_BORDER, UICOLOR__INVALID);
    // Paint tab strip
    RECT rcTabs = m_rcItem;
    rcTabs.left += m_rcInset.left;
    rcTabs.top += m_rcInset.top;
    rcTabs.right -= m_rcInset.right;
    rcTabs.bottom = m_rcClient.top;
    RECT rcTemp = { 0 };
    if (IntersectRect(&rcTemp, &m_rcItem, &rcTabs)) {
      int iPosX = 1;
      m_aTabAreas.Empty();
      for (int i = 0; i < GetCount(); i++) {
        const CControlUI* pPage = GetItem(i);
        const CStdString& strText = pPage->GetText();
        RECT rcTab = { rcTabs.left + iPosX, rcTabs.top, rcTabs.right, m_rcClient.top };
        UINT uState = 0;
        if (UIS_FOCUSED & uState) {
          uState |= UIS_FOCUSED;
        }
        if (UIS_DISABLED & uState) {
          uState |= UIS_DISABLED;
        }
        if (m_iCurSel == i) {
          uState = UIS_PUSHED;
        }
        DoPaintTabFolder(hDC, rcTab, strText, uState);
        iPosX += (rcTab.right - rcTab.left) + 2;
        m_aTabAreas.Add(&rcTab);
      }
    }
  }
  if (m_pCurPage != NULL) {
    m_pCurPage->DoPaint(hDC, m_rcItem);
  }
  return 0;
}
int CStatusbarUI_DoPaint(HDDC hDC, RECT m_rcItem)
{
  DoPaintPanel(hDC, m_rcItem);
  RECT rcMessage = m_rcItem;
  InflateRect(&rcMessage, -8, -2);
  int nLinks = 0;
  DoPaintPrettyText(hDC, rcMessage, m_sText, UICOLOR_TITLE_TEXT, UICOLOR__INVALID, NULL, nLinks, DT_SINGLELINE | DT_VCENTER);
  return 0;
}
int CToolbarTitlePanelUI_DoPaint(HDDC hDC, RECT m_rcItem)
{
  DoPaintPanel(hDC, m_rcItem);
  RECT rcText = m_rcItem;
  InflateRect(&rcText, -m_iPadding, -m_iPadding);
  int nLinks = 0;
  DoPaintPrettyText(hDC, rcText, m_sText, UICOLOR_TITLE_TEXT, UICOLOR__INVALID, NULL, nLinks, DT_WORDBREAK | DT_NOPREFIX);
  return 0;
}
int CToolbarUI_DoPaint(HDDC hDC, RECT m_rcItem)
{
  DoPaintPanel(hDC, m_rcItem);
  return 0;
}
int CToolButtonUI_DoPaint(HDDC hDC, RECT m_rcItem)
{
  UINT uState = 0;
  if (UIS_FOCUSED & uState) {
    uState |= UIS_FOCUSED;
  }
  if (UIS_DISABLED & uState) {
    uState |= UIS_DISABLED;
  }
  DoPaintToolbarButton(hDC, m_rcItem, m_sText, m_szPadding, m_uButtonState | uState);
  return 0;
}
int CToolSeparatorUI_DoPaint(HDDC hDC, RECT m_rcItem)
{
  RECT rc1 = { m_rcItem.left + 1, m_rcItem.top + 2, m_rcItem.left + 1, m_rcItem.bottom - 3 };
  DoPaintLine(hDC, rc1, UICOLOR_TITLE_BORDER_DARK);
  RECT rc2 = { m_rcItem.left + 2, m_rcItem.top + 2, m_rcItem.left + 2, m_rcItem.bottom - 3 };
  DoPaintLine(hDC, rc2, UICOLOR_TITLE_BORDER_LIGHT);
  return 0;
}
int CToolGripperUI_DoPaint(HDDC hDC, RECT m_rcItem)
{
  int i;
  RECT rcLine = { m_rcItem.left + 5, m_rcItem.top + 6, m_rcItem.left + 5 + 3, m_rcItem.top + 6 };
  for (i = m_rcItem.top + 6; i <= m_rcItem.bottom - 6; i += 2) {
    DoPaintLine(hDC, rcLine, UICOLOR_TITLE_BORDER_DARK);
    OffsetRect(&rcLine, 0, 2);
  }
  return 0;
}
int CDropDownUI_DoPaint(HDDC hDC, RECT m_rcItem, LPCTSTR m_sText, UINT uState, UINT uStyle)
{
  // Paint the nice frame
  int cy = m_rcItem.bottom - m_rcItem.top;
  RECT rcText, m_rcButton;
  RECT rcPadding = { 0 };
  SetRect(&m_rcButton, m_rcItem.right - cy, m_rcItem.top, m_rcItem.right, m_rcItem.bottom);
  SetRect(&rcText, m_rcItem.left, m_rcItem.top, m_rcButton.left + 1, m_rcItem.bottom);
  if (UIS_DISABLED & uState) {
    DoPaintFrame(hDC, rcText, UICOLOR_CONTROL_BORDER_DISABLED, UICOLOR__INVALID, UICOLOR__INVALID, 0);
  }
  else {
    DoPaintFrame(hDC, rcText, UICOLOR_CONTROL_BORDER_NORMAL, UICOLOR_CONTROL_BORDER_NORMAL, UICOLOR__INVALID, 0);
  }
  // Paint dropdown edit box
  InflateRect(&rcText, -1, -1);
  if (m_iCurSel >= 0) {
    CControlUI* pControl = static_cast<CControlUI*>(m_items[m_iCurSel]);
    IListItemUI* pElement = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
    if (pElement != NULL) {
      // Render item with specific draw-style
      pElement->DrawItem(hDC, rcText, UIDRAWSTYLE_INPLACE | (m_bFocused ? UIDRAWSTYLE_FOCUS : 0));
    }
    else {
      // Allow non-listitems to render as well.
      RECT rcOldPos = pControl->GetPos();
      pControl->SetPos(rcText);
      pControl->DoPaint(hDC, rcText);
      pControl->SetPos(rcOldPos);
    }
  }
  else {
    DoFillRect(hDC, rcText, UICOLOR_CONTROL_BACKGROUND_NORMAL);
  }
  // Paint dropdown button
  DoPaintButton(hDC, m_rcButton, _T("<i 6>"), rcPadding, m_uButtonState, 0);
  return 0;
}
int CListTextElementUI_DrawItem(HDDC hDC, RECT rcItem, UINT uStyle)
{
  ASSERT(m_pOwner);
  if (m_pOwner == NULL) {
    return;
  }
  const TListInfoUI* pInfo = m_pOwner->GetListInfo();
  UITYPE_COLOR iTextColor = pInfo->Text;
  UITYPE_COLOR iBackColor = pInfo->Background;
  if ((m_uButtonState & UIS_HOT) != 0) {
    iBackColor = pInfo->HotBackground;
    iTextColor = pInfo->HotText;
  }
  if (IsSelected()) {
    iTextColor = pInfo->SelText;
    iBackColor = pInfo->SelBackground;
  }
  if (UIS_DISABLED & uState) {
    iTextColor = UICOLOR_CONTROL_TEXT_DISABLED;
    iBackColor = UICOLOR_CONTROL_BACKGROUND_DISABLED;
  }
  if (iBackColor != UICOLOR__INVALID) {
    DoFillRect(hDC, m_rcItem, iBackColor);
  }
  IListCallbackUI* pCallback = m_pOwner->GetTextCallback();
  ASSERT(pCallback);
  if (pCallback == NULL) {
    return;
  }
  m_nLinks = 0;
  int nLinks = lengthof(m_rcLinks);
  for (int i = 0; i < pInfo->nColumns; i++) {
    // Paint text
    RECT rcItem = { pInfo->rcColumn[i].left, m_rcItem.top, pInfo->rcColumn[i].right, m_rcItem.bottom - 1 };
    LPCTSTR pstrText = pCallback->GetItemText(this, m_iIndex, i);
    InflateRect(&rcItem, -4, 0);
    DoPaintPrettyText(hDC, rcItem, pstrText, iTextColor, UICOLOR__INVALID, m_rcLinks, nLinks, DT_SINGLELINE | m_uTextStyle);
    if (nLinks > 0) {
      m_nLinks = nLinks, nLinks = 0;
    }
    else {
      nLinks = lengthof(m_rcLinks);
    }
  }
  RECT rcLine = { m_rcItem.left, m_rcItem.bottom - 1, m_rcItem.right, m_rcItem.bottom - 1 };
  DoPaintLine(hDC, rcLine, UICOLOR_DIALOG_BACKGROUND);
  return 0;
}
int CListExpandElementUI_DrawItem(HDDC hDC, const RECT& rcItem, UINT uStyle)
{
  ASSERT(m_pOwner);
  if (m_pOwner == NULL) {
    return;
  }
  const TListInfoUI* pInfo = m_pOwner->GetListInfo();
  UITYPE_COLOR iTextColor = pInfo->Text;
  UITYPE_COLOR iBackColor = pInfo->Background;
  if ((m_uButtonState & UIS_HOT) != 0) {
    iBackColor = pInfo->HotBackground;
    iTextColor = pInfo->HotText;
  }
  if (IsSelected()) {
    iTextColor = pInfo->SelText;
    iBackColor = pInfo->SelBackground;
  }
  if (UIS_DISABLED & uState) {
    iTextColor = UICOLOR_CONTROL_TEXT_DISABLED;
    iBackColor = UICOLOR_CONTROL_BACKGROUND_DISABLED;
  }
  if (iBackColor != UICOLOR__INVALID) {
    RECT rcItem = { m_rcItem.left, m_rcItem.top, m_rcItem.right, m_rcItem.top + m_cyItem };
    DoFillRect(hDC, rcItem, iBackColor);
  }
  IListCallbackUI* pCallback = m_pOwner->GetTextCallback();
  ASSERT(pCallback);
  if (pCallback == NULL) {
    return;
  }
  m_nLinks = 0;
  CStdString sColText;
  int nLinks = lengthof(m_rcLinks);
  for (int i = 0; i < pInfo->nColumns; i++) {
    // Paint text
    RECT rcItem = { pInfo->rcColumn[i].left, m_rcItem.top, pInfo->rcColumn[i].right, m_rcItem.top + m_cyItem };
    LPCTSTR pstrText = pCallback->GetItemText(this, m_iIndex, i);
    // If list control is expandable then we'll automaticially draw
    // the expander-box at the first column.
    if (i == 0 && pInfo->bExpandable) {
      sColText = (m_bExpanded ? _T("<i 14> ") : _T("<i 13> "));
      sColText += pstrText;
      pstrText = sColText;
    }
    InflateRect(&rcItem, -4, 0);
    DoPaintPrettyText(hDC, rcItem, pstrText, iTextColor, UICOLOR__INVALID, m_rcLinks, nLinks, DT_SINGLELINE | m_uTextStyle);
    if (nLinks > 0) {
      m_nLinks = nLinks, nLinks = 0;
    }
    else {
      nLinks = lengthof(m_rcLinks);
    }
  }
  RECT rcLine = { m_rcItem.left, m_rcItem.bottom - 1, m_rcItem.right, m_rcItem.bottom - 1 };
  DoPaintLine(hDC, rcLine, UICOLOR_DIALOG_BACKGROUND);
  return 0;
}
int CListExpandElementUI_DoPaint(HDDC hDC, RECT m_rcItem)
{
  CListTextElementUI_DoPaint(hDC, m_rcItem);
  // Paint the expanded items?
  if (m_bExpanded && m_pContainer != NULL) {
    // Paint gradient box for the items
    RECT rcFrame = m_pContainer->GetPos();
    COLORREF clr1, clr2;
    GetThemeColorPair(UICOLOR_CONTROL_BACKGROUND_EXPANDED, clr1, clr2);
    DoPaintGradient(hDC, rcFrame, clr1, clr2, TRUE, 64);
    DoPaintRectangle(hDC, rcFrame, UICOLOR_HEADER_BORDER, UICOLOR__INVALID);
    RECT rcLine = { m_rcItem.left, rcFrame.top, m_rcItem.right, rcFrame.top };
    DoPaintLine(hDC, rcLine, UICOLOR_STANDARD_BLACK);
    // We'll draw the items then...
  }
  return 0;
}
#endif

