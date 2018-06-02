#include "draw.h"
typedef BOOL(__stdcall* NOTIFYWINEVENT)(DWORD event, HWND hwnd, LONG idObject, LONG idChild);
#ifndef SPI_GETMENUANIMATION
#define SPI_GETMENUANIMATION 0x1002
#endif
#ifndef SPI_GETMENUFADE
#define SPI_GETMENUFADE 0x1012
#endif
#ifndef SPI_GETMENUUNDERLINES
#define SPI_GETKEYBOARDCUES 0x100A
#define SPI_GETMENUUNDERLINES SPI_GETKEYBOARDCUES
#endif
static const char* strOfficeFontName = ("Tahoma");
static const char* strOffice2007FontName = ("Segoe UI");
static const char* strDefaultFontName = ("MS Sans Serif");
static const char* strVertFontName = ("Arial");
static const char* strMarlettFontName = ("Marlett");
int GetTextHeight(uidraw_t* s, BOOL bHorz DEFAULT(TRUE))
{
  return bHorz ? s->m_nTextHeightHorz : s->m_nTextHeightVert;
}
int GetTextWidth(uidraw_t* s, BOOL bHorz DEFAULT(TRUE))
{
  return bHorz ? s->m_nTextWidthHorz : s->m_nTextWidthVert;
}
int GetTextMargins(uidraw_t* s, BOOL bHorz DEFAULT(TRUE))
{
  return bHorz ? s->m_nTextMarginsHorz : s->m_nTextMarginsVert;
}
double GetRibbonImageScale(uidraw_t* s)
{
  return s->m_bIsRibbonImageScale ? s->m_dblRibbonImageScale : 1.;
}
int EnableRibbonImageScale(uidraw_t* s, BOOL bEnable DEFAULT(TRUE))
{
  s->m_bIsRibbonImageScale = bEnable;
  return 0;
}
BOOL IsRibbonImageScaleEnabled(uidraw_t* s)
{
  return s->m_bIsRibbonImageScale;
}
BOOL Is32BitIcons(uidraw_t* s)
{
  return s->m_bUseBuiltIn32BitIcons && !s->m_bIsBlackHighContrast && !s->m_bIsWhiteHighContrast;
}
BOOL IsHighContastMode(uidraw_t* s)
{
  return s->m_bIsWhiteHighContrast || s->m_bIsBlackHighContrast;
}
#define IMAGE_MARGIN 4
#define TEXT_MARGIN 3
#define MENU_IMAGE_MARGIN 2
// MFC Control bar compatibility
#define CX_BORDER 1
#define CY_BORDER 1
#define CX_GRIPPER 3
#define CY_GRIPPER 3
#define CX_BORDER_GRIPPER 2
#define CY_BORDER_GRIPPER 2
BOOL PreMultiplyAlpha(img_t* im, BOOL bAutoCheckPremlt)
{
  int i, j;
  if (im->c < 4) {
    return FALSE;
  }
  for (i = 0; i < im->h; ++i) {
    uchar* pix = im->tt.data + i * im->s;
    for (j = 0; j < im->h; ++j, pix += im->c) {
      int alpha = pix[3];
      pix[0] = (uchar)(pix[0] * alpha / 255);
      pix[1] = (uchar)(pix[1] * alpha / 255);
      pix[2] = (uchar)(pix[2] * alpha / 255);
    }
  }
  return TRUE;
}
int UpdateSysColors(uidraw_t* s)
{
  s->m_bIsBlackHighContrast = (_GetSysColor(CLR_3DLIGHT) == _RGB(255, 255, 255)) && (_GetSysColor(CLR_3DFACE) == _RGB(0, 0, 0));
  s->m_bIsWhiteHighContrast = _GetSysColor(CLR_3DDKSHADOW) == _RGB(0, 0, 0) && _GetSysColor(CLR_3DFACE) == _RGB(255, 255, 255);
  s->clrBarFace = s->clrBtnFace = _GetSysColor(CLR_BTNFACE);
  s->clrBarShadow = s->clrBtnShadow = _GetSysColor(CLR_BTNSHADOW);
  s->clrBarDkShadow = s->clrBtnDkShadow = _GetSysColor(CLR_3DDKSHADOW);
  s->clrBarLight = s->clrBtnLight = _GetSysColor(CLR_3DLIGHT);
  s->clrBarHilite = s->clrBtnHilite = _GetSysColor(CLR_BTNHIGHLIGHT);
  s->clrBarText = s->clrBtnText = _GetSysColor(CLR_BTNTEXT);
  s->clrGrayedText = _GetSysColor(CLR_GRAYTEXT);
  s->clrWindowFrame = _GetSysColor(CLR_WINDOWFRAME);
  s->clrHilite = _GetSysColor(CLR_HIGHLIGHT);
  s->clrTextHilite = _GetSysColor(CLR_HIGHLIGHTTEXT);
  s->clrBarWindow = s->clrWindow = _GetSysColor(CLR_WINDOW);
  s->clrWindowText = _GetSysColor(CLR_WINDOWTEXT);
  s->clrCaptionText = _GetSysColor(CLR_CAPTIONTEXT);
  s->clrMenuText = _GetSysColor(CLR_MENUTEXT);
  s->clrActiveCaption = _GetSysColor(CLR_ACTIVECAPTION);
  s->clrInactiveCaption = _GetSysColor(CLR_INACTIVECAPTION);
  s->clrActiveCaptionGradient = s->clrActiveCaption;
  s->clrInactiveCaptionGradient = s->clrInactiveCaption;
  s->clrActiveBorder = _GetSysColor(CLR_ACTIVEBORDER);
  s->clrInactiveBorder = _GetSysColor(CLR_INACTIVEBORDER);
  s->clrInactiveCaptionText = _GetSysColor(CLR_INACTIVECAPTIONTEXT);
  if (s->m_bIsBlackHighContrast) {
    s->clrHotText = s->clrWindowText;
    s->clrHotLinkText = s->clrWindowText;
  }
  else {
    if (1) {
      s->clrHotText = GetSysColor(COLOR_HOTLIGHT);
      s->clrHotLinkText = _RGB(0, 0, 255); // Light blue
    }
    else {
      s->clrHotText = _RGB(0, 0, 255); // Light blue
      s->clrHotLinkText = _RGB(255, 0, 255); // Violet
    }
  }
  s->hbrBtnShadow = _GetSysColor(COLOR_BTNSHADOW);
  s->hbrBtnHilite = _GetSysColor(COLOR_BTNHIGHLIGHT);
  s->hbrWindow = _GetSysColor(COLOR_WINDOW);
  s->clrBlack = (s->clrBtnDkShadow);
  s->clrLight = _RGB(
      GetRV(s->clrBtnFace) + ((GetRV(s->clrBtnHilite) - GetRV(s->clrBtnFace)) / 2),
      GetGV(s->clrBtnFace) + ((GetGV(s->clrBtnHilite) - GetGV(s->clrBtnFace)) / 2),
      GetBV(s->clrBtnFace) + ((GetBV(s->clrBtnHilite) - GetBV(s->clrBtnFace)) / 2)
      );
  return 0;
}
int UpdateTextMetrics(uidraw_t* s)
{
  font_t* fo;
  fo = s->fontRegular;
  s->m_nTextMarginsHorz = fo->h < 15 ? 2 : 5;
  s->m_nTextHeightHorz = fo->h + s->m_nTextMarginsHorz;
  s->m_nTextWidthHorz = fo->w + s->m_nTextMarginsHorz;
  fo = s->fontVert;
  s->m_nTextMarginsVert = fo->h < 15 ? 2 : 5;
  s->m_nTextHeightVert = fo->h + s->m_nTextMarginsVert;
  s->m_nTextWidthVert = fo->w + s->m_nTextMarginsVert;
  return 0;
}
int UpdateFonts(uidraw_t* s, sys_t* sys)
{
  const char* strRegular = "ËÎÌו";
  int size = 12;
  s->m_dblRibbonImageScale = 1;//dc.GetDeviceCaps(LOGPIXELSX) / 96.0f;
  if (s->m_dblRibbonImageScale > 1. && s->m_dblRibbonImageScale < 1.1) {
    s->m_dblRibbonImageScale = 1.;
  }
  s->fontRegular = sys_font_load_win32_impl(sys, strRegular, size, 0);
  s->fontSmall = sys_font_load_win32_impl(sys, strRegular, size, 0);
  s->fontTooltip = sys_font_load_win32_impl(sys, strRegular, size, 0);
  s->fontUnderline = sys_font_load_win32_impl(sys, strRegular, size, FONT_UNDERLINE);
  s->fontBold = sys_font_load_win32_impl(sys, strRegular, size, FONT_BOLD);
  s->fontMarlett = sys_font_load_win32_impl(sys, strMarlettFontName, size, 0);
  s->fontVert = sys_font_load_win32_impl(sys, strRegular, size, 0);
  s->fontVertCaption = sys_font_load_win32_impl(sys, strRegular, size, 0);
  s->fontDefault = sys_font_load_win32_impl(sys, strRegular, size, 0);
  s->fontDefaultGUIUnderline = sys_font_load_win32_impl(sys, strRegular, size, FONT_UNDERLINE);
  s->fontDefaultGUIBold = sys_font_load_win32_impl(sys, strRegular, size, FONT_BOLD);
  UpdateTextMetrics(s);
  return 0;
}
int OnSettingChange(uidraw_t* s)
{
  s->m_bInSettingsChange = TRUE;
  s->m_sizeSmallIcon.w = GetSystemMetrics(SM_CXSMICON);
  s->m_sizeSmallIcon.h = GetSystemMetrics(SM_CYSMICON);
  iRectSetEmpty(&s->m_rectVirtual);
  // Get system menu animation type:
  s->m_bMenuAnimation = FALSE;
  s->m_bMenuFadeEffect = FALSE;
  s->m_nShellAutohideBars = 0;
  s->m_bSysUnderlineKeyboardShortcuts = TRUE;
  s->m_bUnderlineKeyboardShortcuts = s->m_bSysUnderlineKeyboardShortcuts;
  s->m_bInSettingsChange = FALSE;
  return 0;
}
BOOL CreateDitherBitmap(img_t* hbm, COLOR clr0, COLOR clr1)
{
  int i;
  long patGray[8];
  COLOR bmiColors[2];
  bmiColors[0] = clr0;
  bmiColors[1] = clr1;//_GetSysColor(CLR_BTNHIGHLIGHT);
  // initialize the brushes
  for (i = 0; i < 8; i++) {
    patGray[i] = (i & 1) ? 0xAAAA5555L : 0x5555AAAAL;
  }
  return imsetbitmap(hbm, 8, 8, 1, 1, patGray, bmiColors);
}
BOOL DrawParentBackground(uidraw_t* s, img_t* im, IRECT* rectClip)
{
  return 0;
}
COLOR GetColor(uidraw_t* s, int nColor)
{
  switch (nColor) {
  case COLOR_BTNFACE:
    return s->clrBtnFace;
  case COLOR_BTNSHADOW:
    return s->clrBtnShadow;
  case COLOR_3DDKSHADOW:
    return s->clrBtnDkShadow;
  case COLOR_3DLIGHT:
    return s->clrBtnLight;
  case COLOR_BTNHIGHLIGHT:
    return s->clrBtnHilite;
  case COLOR_BTNTEXT:
    return s->clrBtnText;
  case COLOR_GRAYTEXT:
    return s->clrGrayedText;
  case COLOR_WINDOWFRAME:
    return s->clrWindowFrame;
  case COLOR_HIGHLIGHT:
    return s->clrHilite;
  case COLOR_HIGHLIGHTTEXT:
    return s->clrTextHilite;
  case COLOR_WINDOW:
    return s->clrWindow;
  case COLOR_WINDOWTEXT:
    return s->clrWindowText;
  case COLOR_CAPTIONTEXT:
    return s->clrCaptionText;
  case COLOR_MENUTEXT:
    return s->clrMenuText;
  case COLOR_ACTIVECAPTION:
    return s->clrActiveCaption;
  case COLOR_INACTIVECAPTION:
    return s->clrInactiveCaption;
  case COLOR_ACTIVEBORDER:
    return s->clrActiveBorder;
  case COLOR_INACTIVEBORDER:
    return s->clrInactiveBorder;
  case COLOR_INACTIVECAPTIONTEXT:
    return s->clrInactiveCaptionText;
  }
  return GetSysColor(nColor);
}
#define AfxGetInstanceHandle() sys->hInstance
#if 0
BOOL uidraw_t::ExcludeTag(str_t* strBuffer, const char* lpszTag, str_t* strTag, BOOL bIsCharsList /* = FALSE*/)
{
  const int iBufLen = strlen(strBuffer);
  CString strTagStart = ('<');
  strTagStart += lpszTag;
  strTagStart += ('>');
  const int iTagStartLen = strTagStart.GetLength();
  int iStart = -1;
  int iIndexStart = strBuffer.Find(strTagStart);
  if (iIndexStart < 0) {
    return FALSE;
  }
  iStart = iIndexStart + iTagStartLen;
  CString strTagEnd = ("</");
  strTagEnd += lpszTag;
  strTagEnd += ('>');
  const int iTagEndLen = strTagEnd.GetLength();
  int iIndexEnd = -1;
  int nBalanse = 1;
  for (int i = iStart; i < iBufLen - iTagEndLen + 1; i ++) {
    if (strBuffer [i] != '<') {
      continue;
    }
    if (i < iBufLen - iTagStartLen && _tcsncmp(strBuffer.Mid(i), strTagStart, iTagStartLen) == 0) {
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
    if (strTag.GetLength() > 1 && strTag [0] == ('\"')) {
      strTag = strTag.Mid(1, strTag.GetLength() - 2);
    }
    strTag.Replace(("\\t"), ("\t"));
    strTag.Replace(("\\n"), ("\n"));
    strTag.Replace(("\\r"), ("\r"));
    strTag.Replace(("\\b"), ("\b"));
    strTag.Replace(("LT"), ("<"));
    strTag.Replace(("GT"), (">"));
    strTag.Replace(("AMP"), ("&"));
  }
  return TRUE;
}
#endif
BOOL IsWinXPThemeSupported()
{
  return FALSE;
}
BOOL IsHighlightWholeMenuItem()
{
  return FALSE;
}
int OnFillMenuImageRect(uidraw_t* s, img_t* im, IRECT rect, int state)
{
  s->OnFillButtonInterior(s, im, rect, state);
  return 0;
}
int OnDrawMenuImageRectBorder(uidraw_t* s, img_t* im, IRECT rect, int state)
{
  s->OnDrawButtonBorder(s, im, rect, state);
  return 0;
}
BOOL IsOwnerDrawMenuCheck(uidraw_t* s)
{
  return FALSE;
}
COLOR GetToolbarDisabledColor(uidraw_t* s)
{
  return(COLOR) - 1;
}
BOOL IsToolbarRoundShape(uidraw_t* s, int* pToolBar)
{
  return FALSE;
}
int GetDockingBarCaptionExtraHeight(uidraw_t* s)
{
  return 0;
}
BOOL IsHighlightOneNoteTabs(uidraw_t* s)
{
  return FALSE;
}
BOOL AlwaysHighlight3DTabs(uidraw_t* s)
{
  return FALSE;
}
COLOR GetTabTextColor(uidraw_t* s, const int* pTabWnd, int iTab, BOOL bIsActive)
{
  return(COLOR) - 1;
}
int GetTabHorzMargin(uidraw_t* s, const int* pTabWnd)
{
  return 0;
}
int GetMDITabsBordersSize(uidraw_t* s)
{
  return -1; /* Default */
}
BOOL IsMDITabsTopEdge(uidraw_t* s)
{
  return TRUE;
}
int GetDockingTabsBordersSize(uidraw_t* s)
{
  return -1; /* Default */
}
BOOL IsDockingTabHasBorder(uidraw_t* s)
{
  return TRUE;
}
int GetTabButtonState(uidraw_t* s, int* pTab, int* pButton)
{
  return -1;
}
ISIZE GetButtonExtraBorder(uidraw_t* s)
{
  return iSIZE(0, 0);
}
ISIZE GetCaptionButtonExtraBorder(uidraw_t* s)
{
  return iSIZE(0, 0);
}
// Obsolete
COLOR OnDrawPropSheetListItem(uidraw_t* s, img_t* im, int* p, IRECT rc, BOOL b1, BOOL b2)
{
  return(COLOR)0;
}
COLOR GetPropListDesciptionTextColor(uidraw_t* s, int* pPropList)
{
  return(COLOR) - 1;
}
// Auto-hide buttons:
BOOL HasOverlappedAutoHideButtons(uidraw_t* s)
{
  return FALSE;
}
// Windows XP drawing methods:
BOOL DrawComboDropButtonWinXP(uidraw_t* s, img_t* im, IRECT rect, BOOL bDisabled, BOOL bIsDropped, BOOL bIsHighlighted)
{
  return FALSE;
}
BOOL DrawComboBorderWinXP(uidraw_t* s, img_t* im, IRECT rect, BOOL bDisabled, BOOL bIsDropped, BOOL bIsHighlighted)
{
  return FALSE;
}
int GetSmartDockingTheme(uidraw_t* s)
{
  return BCGP_SDT_VS2005;
}
// Day planner:
DWORD GetPlannerDrawFlags(uidraw_t* s)
{
  return 0;
}
int GetPlannerRowExtraHeight(uidraw_t* s)
{
  return 0;
}
COLOR GetPlannerWorkColor(uidraw_t* s)
{
  return s->m_clrPlannerWork;
}
// Window apperance:
BOOL IsOwnerDrawCaption(uidraw_t* s)
{
  return FALSE;
}
int GetRibbonQATChevronOffset(uidraw_t* s)
{
  return 0;
}
int GetRibbonQATRightMargin(uidraw_t* s)
{
  return 0;
}
COLOR VisualManager_GetToolbarDisabledTextColor(uidraw_t* s)
{
  return s->clrGrayedText;
}
int VisualManager_OnHighlightMenuItem(uidraw_t* s, img_t* im, IRECT rect, COLOR clrText)
{
  imdraw_FillRect(im, 0, &rect, s->clrHilite);
  return 0;
}
COLOR GetRibbonQATTextColor(uidraw_t* s, BOOL bDisabled DEFAULT(FALSE))
{
  return bDisabled ? s->GetToolbarDisabledTextColor(s) : (COLOR) - 1;
}
int GetRibbonPopupBorderSize(uidraw_t* s, const int* pPopup)
{
  return s->m_nMenuBorderSize;
}
BOOL IsLayeredRibbonKeyTip(uidraw_t* s)
{
  return FALSE;
}
IPOINT GetRibbonMainImageOffset(uidraw_t* s)
{
  return s->m_ptRibbonMainImageOffset;
}
// Scrollbar
BOOL IsOwnerDrawScrollBar(uidraw_t* s)
{
  return FALSE;
}
int OnScrollBarDrawThumb(uidraw_t* s, img_t* im, int* pScrollBar, IRECT rect,
    BOOL bHorz, BOOL bHighlighted, BOOL bPressed, BOOL bDisabled)
{
  ASSERT(FALSE);
  return 0;
}
int OnScrollBarDrawButton(uidraw_t* s, img_t* im, int* pScrollBar, IRECT rect,
    BOOL bHorz, BOOL bHighlighted, BOOL bPressed, BOOL bFirst, BOOL bDisabled)
{
  ASSERT(FALSE);
  return 0;
}
int OnScrollBarFillBackground(uidraw_t* s, img_t* im, int* pScrollBar, IRECT rect,
    BOOL bHorz, BOOL bHighlighted, BOOL bPressed, BOOL bFirst, BOOL bDisabled)
{
  ASSERT(FALSE);
  return 0;
}
// Push button:
BOOL OnDrawPushButton(uidraw_t* s, img_t* im, IRECT rect, int* pButton, COLOR clrText)
{
  return FALSE;
}
int SetMenuShadowDepth(uidraw_t* s, int nDepth) // Pixels
{
  s->m_nMenuShadowDepth = nDepth;
  return 0;
}
int GetMenuShadowDepth(uidraw_t* s)
{
  return s->m_nMenuShadowDepth;
}
BOOL IsLook2000(uidraw_t* s)
{
  return s->m_bLook2000;
}
int SetLook2000(uidraw_t* s, BOOL bLook2000 DEFAULT(TRUE))
{
  s->m_bLook2000 = bLook2000;
  return 0;
}
BOOL IsMenuFlatLook(uidraw_t* s)
{
  return s->m_bMenuFlatLook;
}
int SetMenuFlatLook(uidraw_t* s, BOOL bMenuFlatLook DEFAULT(TRUE))
{
  s->m_bMenuFlatLook = bMenuFlatLook;
  return 0;
}
BOOL IsAutoDestroy(uidraw_t* s)
{
  return s->m_bAutoDestroy;
}
int SetShadowHighlightedImage(uidraw_t* s, BOOL bShadow DEFAULT(TRUE))
{
  s->m_bShadowHighlightedImage = bShadow;
  return 0;
}
BOOL IsShadowHighlightedImage(uidraw_t* s)
{
  return s->m_bShadowHighlightedImage;
}
int EnableToolbarButtonFill(uidraw_t* s, BOOL bEnable DEFAULT(TRUE))
{
  s->m_bEnableToolbarButtonFill = bEnable;
  return 0;
}
BOOL IsToolbarButtonFillEnabled(uidraw_t* s)
{
  return s->m_bEnableToolbarButtonFill;
}
BOOL IsEmbossDisabledImage(uidraw_t* s)
{
  return s->m_bEmbossDisabledImage;
}
int SetEmbossDisabledImage(uidraw_t* s, BOOL bEmboss DEFAULT(TRUE))
{
  s->m_bEmbossDisabledImage = bEmboss;
  return 0;
}
BOOL IsFadeInactiveImage(uidraw_t* s)
{
  return s->m_bFadeInactiveImage;
}
int SetFadeInactiveImage(uidraw_t* s, BOOL bFade DEFAULT(TRUE))
{
  s->m_bFadeInactiveImage = bFade;
  return 0;
}
int GetMenuImageMargin(uidraw_t* s)
{
  return 2;
}
COLOR GetMenuShadowBaseColor(uidraw_t* s)
{
  return s->m_clrMenuShadowBase;
}
IRECT GetMenuImageFrameOffset(uidraw_t* s)
{
  return iRECT(2, 1, 0, 2);
}
int GetPopupMenuGap(uidraw_t* s)
{
  return 1;
}
// Allows choose "Look 2000" in the customization dialog
BOOL IsLook2000Allowed(uidraw_t* s)
{
  return TRUE;
}
// TasksPane:
int GetTasksPaneVertMargin(uidraw_t* s)
{
  return s->m_nVertMargin;
}
int GetTasksPaneHorzMargin(uidraw_t* s)
{
  return s->m_nHorzMargin;
}
int GetTasksPaneGroupVertOffset(uidraw_t* s)
{
  return s->m_nGroupVertOffset;
}
int GetTasksPaneGroupCaptionHeight(uidraw_t* s)
{
  return s->m_nGroupCaptionHeight;
}
int GetTasksPaneGroupCaptionHorzOffset(uidraw_t* s)
{
  return s->m_nGroupCaptionHorzOffset;
}
int GetTasksPaneGroupCaptionVertOffset(uidraw_t* s)
{
  return s->m_nGroupCaptionVertOffset;
}
int GetTasksPaneTaskHorzOffset(uidraw_t* s)
{
  return s->m_nTasksHorzOffset;
}
int GetTasksPaneIconHorzOffset(uidraw_t* s)
{
  return s->m_nTasksIconHorzOffset;
}
int GetTasksPaneIconVertOffset(uidraw_t* s)
{
  return s->m_nTasksIconVertOffset;
}
int GetToolBarCustomizeButtonMargin(uidraw_t* s)
{
  return 2;
}
BOOL IsOffsetPressedButton(uidraw_t* s)
{
  return TRUE;
}
BOOL IsOfficeXPStyleMenus(uidraw_t* s)
{
  return s->m_bOfficeXPStyleMenus;
}
BOOL GetPopupMenuBorderSize(uidraw_t* s)
{
  return s->m_nMenuBorderSize;
}
BOOL IsFrameMenuCheckedItems(uidraw_t* s)
{
  return s->m_bFrameMenuCheckedItems;
}
BOOL IsEmbossDisabledBtnText(uidraw_t* s)
{
  return TRUE;
}
int VisualManager_OnUpdateSystemColors(uidraw_t* s)
{
  s->m_clrPlannerWork = _RGB(255, 255, 0);
  s->m_clrPalennerLine = _RGB(128, 128, 128);
  s->m_clrReportGroupText = s->clrHilite;
  return 0;
}
#define ASSERT_VALID(x)
int VisualManager_OnDrawBarGripper(uidraw_t* s, img_t* im, IRECT rectGripper, BOOL bHorz)
{
  const COLOR clrBtnFace = s->clrBtnFace;
  const COLOR clrHilite = s->clrBarHilite;
  const COLOR clrShadow = s->clrBarShadow;
  const BOOL bSingleGripper = s->m_bLook2000;
  const int iGripperSize = 3;
  const int iGripperOffset = bSingleGripper ? 0 : 1;
  const int iLinesNum = bSingleGripper ? 1 : 2;
  int i;
  if (bHorz) {
    //-----------------
    // Gripper at left:
    //-----------------
    iRectDeflate2(&rectGripper, 0, bSingleGripper ? 3 : 2);
    // ET: Center the grippers
    rectGripper.l = iGripperOffset + RCX(&rectGripper) / 2 - (iLinesNum * iGripperSize + (iLinesNum - 1) * iGripperOffset) / 2;
    rectGripper.r = rectGripper.l + iGripperSize;
    for (i = 0; i < iLinesNum; i ++) {
      imdraw_Draw3dRect(im, 0, &rectGripper, clrHilite, clrShadow, clrBtnFace);
      // ET: not used for NewFlat look
      if (! bSingleGripper) {
        //-----------------------------------
        // To look same as MS Office Gripper!
        //-----------------------------------
        imdraw_SetPixel(im, 0, rectGripper.l, rectGripper.b - 1, clrHilite);
      }
      iRectOffset2(&rectGripper, iGripperSize + 1, 0);
    }
  }
  else {
    //----------------
    // Gripper at top:
    //----------------
    rectGripper.t += iGripperOffset;
    iRectDeflate2(&rectGripper, bSingleGripper ? 3 : 2, 0);
    // ET: Center the grippers
    rectGripper.t = iGripperOffset + RCY(&rectGripper) / 2 -
        (iLinesNum * iGripperSize + (iLinesNum - 1)) / 2;
    rectGripper.b = rectGripper.t + iGripperSize;
    for (i = 0; i < iLinesNum; i ++) {
      imdraw_Draw3dRect(im, 0, &rectGripper, clrHilite, clrShadow, s->clrBtnFace);
      // ET: not used for NewFlat look
      if (! bSingleGripper) {
        //-----------------------------------
        // To look same as MS Office Gripper!
        //-----------------------------------
        imdraw_SetPixel(im, 0, rectGripper.r - 1, rectGripper.t, clrHilite);
      }
      iRectOffset2(&rectGripper, 0, iGripperSize + 1);
    }
  }
  return 0;
}
int OutlookBarPane_OnEraseWorkArea(uidraw_t* s, img_t* im, IRECT rectWorkArea, int* pBar)
{
#if 0
  CBitmap* s->m_bmpBack = pBar->GetBackBitmap();
  if (s->m_bmpBack->GetCount() == 0) {
    COLOR s->m_clrBackColor = pBar->GetBackColor();
    imdraw_FillRect(im, 0, rectWorkArea, s->m_clrBackColor);
  }
  else {
    ASSERT(s->m_bmpBack->GetCount() == 1);
    CSize sizeBack = s->m_bmpBack->GetImageSize();
    for (int x = rectWorkArea.l; x < rectWorkArea.r; x += sizeBack.w) {
      for (int y = rectWorkArea.t; y < rectWorkArea.b; y += sizeBack.h) {
        DrawImage(im, s->m_bmpBack, x, y, 0);
      }
    }
  }
#endif
  return 0;
}
int VisualManager_FillRebarPane(uidraw_t* s, img_t* im, int* pBar, IRECT rectClient)
{
  ASSERT_VALID(pBar);
  imdraw_FillRect(im, 0, &rectClient, s->clrBarFace);
  return 0;
}
int VisualManager_OnFillBarBackground(uidraw_t* s, img_t* im, int* pBar, IRECT rectClient, IRECT rectClip, BOOL bNCArea)
{
#if 0
  int cls = pBar->GetClass();
  ASSERT_VALID(pBar);
  if (pBar->IsOnGlass()) {
    imdraw_FillSolidRect(im, 0, rectClient, _RGB(0, 0, 0));
    return 0;
  }
  switch (cls) {
  case CLASS_REBAR :
    FillRebarPane(im, pBar, rectClient);
    return 0;
  case CLASS_OUTLOOKBAR :
    OutlookBarPane_OnEraseWorkArea(im, rectClient, pBar);
    return 0;
  case CLASS_CAPTIONBAR :
    if (pBar->IsMessageBarMode()) {
      imdraw_FillRect(im, 0, rectClip, s->clrBarFace);
    }
    else {
      COLOR s->m_clrBarBackground = pBar->GetBackColor();
      imdraw_FillSolidRect(im, 0, rectClip, s->m_clrBarBackground == -1 ?
          s->clrBarShadow : s->m_clrBarBackground);
    }
    return 0;
  case CLASS_POPUPMENUBAR:
    if (pBar->IsDropDownListMode()) {
      imdraw_FillRect(im, 0, rectClip, s->clrWindow);
      return 0;
    }
  default:
    // By default, control bar background is filled by
    // the system 3d background color
    imdraw_FillRect(im, 0, rectClip.IsRectEmpty() ? rectClient : rectClip,
        pBar->IsDialogControl() ?
        s->clrBtnFace : s->clrBarFace);
    break;
  }
#endif
  return 0;
}
#if 0
int VisualManager_OnDrawBarBorder(uidraw_t* s, img_t* im, int* pBar, IRECT& rect)
{
  ASSERT_VALID(pBar);
  if (pBar->IsFloating()) {
    return 0;
  }
  DWORD dwStyle = pBar->GetBarStyle();
  if (!(dwStyle & CBRS_BORDER_ANY)) {
    return 0;
  }
  COLOR clrBckOld = im->GetBkColor(); // FillSolidRect changes it
  const COLOR clrHilite = pBar->IsOnGlass() ? _RGB(0, 0, 0) : pBar->IsDialogControl() ?
      s->clrBtnHilite : s->clrBarHilite;
  const COLOR clrShadow = pBar->IsOnGlass() ? _RGB(0, 0, 0) : pBar->IsDialogControl() ?
      s->clrBtnShadow : s->clrBarShadow;
  COLOR clr = pBar->IsOnGlass() ? _RGB(0, 0, 0) : clrHilite;
  if (dwStyle & CBRS_BORDER_LEFT) {
    im->FillSolidRect(0, 0, 1, rect.Height() - 1, clr);
  }
  if (dwStyle & CBRS_BORDER_TOP) {
    im->FillSolidRect(0, 0, rect.Width() - 1 , 1, clr);
  }
  if (dwStyle & CBRS_BORDER_RIGHT)
    im->FillSolidRect(rect.r, 0/*RGL~:1*/, -1,
        rect.Height()/*RGL-: - 1*/, clrShadow);
  if (dwStyle & CBRS_BORDER_BOTTOM) {
    im->FillSolidRect(0, rect.b, rect.Width() - 1, -1, clrShadow);
  }
  // if undockable toolbar at top of frame, apply special formatting to mesh
  // properly with frame menu
  if (!pBar->CanFloat()) {
    im->FillSolidRect(0, 0, rect.Width(), 1, clrShadow);
    im->FillSolidRect(0, 1, rect.Width(), 1, clrHilite);
  }
  if (dwStyle & CBRS_BORDER_LEFT) {
    ++rect.l;
  }
  if (dwStyle & CBRS_BORDER_TOP) {
    ++rect.t;
  }
  if (dwStyle & CBRS_BORDER_RIGHT) {
    --rect.r;
  }
  if (dwStyle & CBRS_BORDER_BOTTOM) {
    --rect.b;
  }
  // Restore Bk color:
  im->SetBkColor(clrBckOld);
}
int VisualManager_OnDrawMenuBorder(uidraw_t* s, img_t* im, PopupMenu* /*pMenu*/, IRECT rect)
{
  imdraw_Draw3dRect(im, 0, &rect, s->clrBarLight, s->clrBarDkShadow, s->clrBtnFace);
  iRectDeflate(&rect, 1, 1);
  imdraw_Draw3dRect(im, 0, &rect, s->clrBarHilite, s->clrBarShadow, s->clrBtnFace);
}
int VisualManager_OnDrawMenuShadow(img_t* pPaintDC, const IRECT& rectClient, const IRECT& /*rectExclude*/,
    int nDepth, int iMinBrightness, int iMaxBrightness,
    CBitmap* pBmpSaveBottom, CBitmap* pBmpSaveRight,
    BOOL bRTL)
{
  ASSERT_VALID(pPaintDC);
  ASSERT_VALID(pBmpSaveBottom);
  ASSERT_VALID(pBmpSaveRight);
  //------------------------------------------------------
  // Simple draw the shadow, ignore rectExclude parameter:
  //------------------------------------------------------
  DrawShadow(pPaintDC->im, &rectClient, rectClient, nDepth, iMinBrightness, iMaxBrightness, pBmpSaveBottom, pBmpSaveRight, 0, !bRTL);
}
#endif
int VisualManager_OnFillButtonInterior(uidraw_t* s, img_t* im, IRECT rect, int state)
{
  //int cls = pButton->GetClass();
  if (state == ButtonsIsHighlighted) {
    HighlightRect(im, 0, rect, -1, 0, 0, 0);
  }
  return 0;
}
COLOR VisualManager_GetToolbarHighlightColor(uidraw_t* s)
{
  return s->clrBarFace;
}
#if 0
COLOR VisualManager_GetHighlightedMenuItemTextColor(int* pButton)
{
  int s->m_nStyle = pButton->GetStyle();
  if (s->m_nStyle & TBBS_DISABLED) {
    return s->clrGrayedText;
  }
  return s->clrTextHilite;
}
int VisualManager_OnHighlightRarelyUsedMenuItems(uidraw_t* s, img_t* im, IRECT rectRarelyUsed)
{
  HighlightRect(im, 0, rectRarelyUsed);
  imdraw_Draw3dRect(im, 0, &rectRarelyUsed, s->clrBarShadow, s->clrBarHilite, s->clrBtnFace);
}
int VisualManager_OnDrawMenuCheck(uidraw_t* s, img_t* im, int* pButton,
    IRECT rectCheck, BOOL /*bHighlight*/, BOOL bIsRadio)
{
  int iImage = bIsRadio ? IdRadio : IdCheck;
  int s->m_nStyle = pButton->GetStyle();
  imdraw_menuimg(im, 0, rectCheck, iImage, (s->m_nStyle & TBBS_DISABLED) ? ImageGray : ImageBlack);
}
int VisualManager_OnDrawMenuItemButton(uidraw_t* s, img_t* im, int* /*pButton*/,
    IRECT rectButton, BOOL bHighlight, BOOL /*bDisabled*/)
{
  IRECT rect = rectButton;
  rect.r = rect.l + 1;
  rect.l--;
  iRectDeflate(&rect, 0, bHighlight ? 1 : 4);
  imdraw_Draw3dRect(im, 0, &rect, s->clrBarShadow, s->clrBarHilite, s->clrBtnFace);
}
#endif
int VisualManager_OnDrawButtonBorder(uidraw_t* s, img_t* im, IRECT rect, int state)
{
  BOOL bIsOutlookButton = FALSE;
  BOOL bIsDrawOnGlass = FALSE;
  COLOR clrDark = bIsOutlookButton ? s->clrBarDkShadow : s->clrBarShadow;
  if (bIsDrawOnGlass) {
    if (state == ButtonsIsPressed || state == ButtonsIsHighlighted) {
      imdraw_DrawRectR(im, &rect, rect, 0, clrDark);
    }
    return 0;
  }
  switch (state) {
  case ButtonsIsPressed:
    imdraw_Draw3dRect(im, 0, &rect, clrDark, s->clrBarHilite, s->clrBtnFace);
    return 0;
  case ButtonsIsHighlighted:
    imdraw_Draw3dRect(im, 0, &rect, s->clrBarHilite, clrDark, s->clrBtnFace);
    return 0;
  }
  return 0;
}
#if 0
int VisualManager_OnDrawButtonSeparator(uidraw_t* s, img_t* im, int* pButton, IRECT rect, int state, BOOL bHorz)
{
  if (!s->m_bMenuFlatLook || !pButton->IsDroppedDown()) {
    OnDrawButtonBorder(im, pButton, rect, state);
  }
}
int VisualManager_OnDrawSeparator(uidraw_t* s, img_t* im, int* pBar,
    IRECT rect, BOOL bHorz)
{
  ASSERT_VALID(pBar);
  IRECT rectSeparator = rect;
  if (bHorz) {
    rectSeparator.l += rectSeparator.Width() / 2 - 1;
    rectSeparator.r = rectSeparator.l + 2;
  }
  else {
    rectSeparator.t += rectSeparator.Height() / 2 - 1;
    rectSeparator.b = rectSeparator.t + 2;
  }
  const COLOR clrHilite = pBar->IsDialogControl() ?
      s->clrBtnHilite : s->clrBarHilite;
  const COLOR clrShadow = pBar->IsDialogControl() ?
      s->clrBtnShadow : s->clrBarShadow;
  if (s->m_bIsDrawOnGlass) {
    int x1, x2;
    int y1, y2;
    if (bHorz) {
      x1 = x2 = (rect.l + rect.r) / 2;
      y1 = rect.t;
      y2 = rect.b - 1;
    }
    else {
      y1 = y2 = (rect.t + rect.b) / 2;
      x1 = rect.l;
      x2 = rect.r;
    }
    imdraw_DrawLine(im, 0, x1, y1, x2, y2, clrShadow);
  }
  else {
    imdraw_Draw3dRect(im, 0, &rectSeparator, clrShadow, clrHilite, s->clrBtnFace);
  }
}
COLOR VisualManager_OnDrawMenuLabel(uidraw_t* s, img_t* im, IRECT rect)
{
  imdraw_FillRect(im, 0, rect, s->clrBtnFace);
  IRECT rectSeparator = rect;
  rectSeparator.t = rectSeparator.b - 2;
  imdraw_Draw3dRect(im, 0, &rectSeparator, s->clrBtnShadow, s->clrBtnHilite, s->clrBtnFace);
  return s->clrBtnText;
}
COLOR VisualManager_OnDrawControlBarCaption(uidraw_t* s, img_t* im, DockingControlBar* /*pBar*/,
    BOOL bActive, IRECT rectCaption, IRECT /*rectButtons*/)
{
  COLOR clr = (bActive ? s->clrActiveCaption : s->clrInactiveCaption);
  imdraw_FillRect(im, 0, rectCaption, clr);
  // get the text color
  return bActive ? s->clrCaptionText : s->clrInactiveCaptionText;
}
int VisualManager_OnDrawCaptionButton(
    img_t* im, CaptionButton* pButton, BOOL bActive,
    BOOL bHorz, BOOL bMaximized, BOOL bDisabled,
    int nImageID /*= -1*/)
{
  IRECT rc = pButton->GetRect();
  int id = -1;
  if (nImageID != -1) {
    id = nImageID;
  }
  else {
    id = pButton->GetIconID(bHorz, bMaximized);
  }
  IRECT rectImage = rc;
  if (pButton->IsPushed() && (pButton->IsFocused() || pButton->IsDroppedDown())) {
    rectImage iRectOffset(&, 1, 1);
  }
  IMAGE_STATE imageState;
  COLOR s->m_clrForeground = pButton->GetForegroundColor();
  if (bDisabled) {
    imageState = ImageGray;
  }
  else if (s->m_clrForeground == 0) {
    imageState = bActive ? ImageWhite : ImageBlack;
  }
  else {
    if (GetRV(s->m_clrForeground) > 192 &&
        GetGV(s->m_clrForeground) > 192 &&
        GetBV(s->m_clrForeground) > 192) {
      imageState = ImageWhite;
    }
    else {
      imageState = ImageBlack;
    }
  }
  MenuImages_Draw(im, id, rectImage, imageState);
  if (!bDisabled) {
    if (pButton->IsPushed() && (pButton->IsFocused() || pButton->IsDroppedDown())) {
      imdraw_Draw3dRect(im, 0, rc, s->clrBarDkShadow, s->clrBarLight);
      rc iRectDeflate(&, 1, 1);
      imdraw_Draw3dRect(im, 0, rc, s->clrBarDkShadow, s->clrBarHilite);
    }
    else if (!s->m_bLook2000) {
      imdraw_Draw3dRect(im, 0, rc, s->clrBarLight, s->clrBarDkShadow);
      rc iRectDeflate(&, 1, 1);
      imdraw_Draw3dRect(im, 0, rc, s->clrBarHilite, s->clrBarShadow);
    }
    else if (pButton->IsFocused() || pButton->IsPushed() || pButton->IsDroppedDown()) {
      imdraw_Draw3dRect(im, 0, rc, s->clrBarHilite, s->clrBarShadow);
    }
  }
}
int VisualManager_OnEraseTabsArea(uidraw_t* s, img_t* im, IRECT rect,
    const BaseTabWnd* /*pTabWnd*/)
{
  imdraw_FillRect(im, 0, rect, s->clrBarFace);
}
int VisualManager_OnDrawTab(uidraw_t* s, img_t* im, IRECT rectTab,
    int iTab, BOOL bIsActive, const BaseTabWnd* pTabWnd)
{
  ASSERT_VALID(pTabWnd);
  COLOR clrTab = pTabWnd->GetTabBkColor(iTab);
  IRECT rectClip;
  im->GetClipBox(rectClip);
  if (pTabWnd->IsFlatTab()) {
    //----------------
    // Draw tab edges:
    //----------------
#define FLAT_POINTS_NUM 4
    IPOINT pts [FLAT_POINTS_NUM];
    const int nHalfHeight = pTabWnd->GetTabsHeight() / 2;
    if (pTabWnd->GetLocation() == BaseTabWnd::LOCATION_BOTTOM) {
      rectTab.b --;
      pts [0].x = rectTab.l;
      pts [0].y = rectTab.t;
      pts [1].x = rectTab.l + nHalfHeight;
      pts [1].y = rectTab.b;
      pts [2].x = rectTab.r - nHalfHeight;
      pts [2].y = rectTab.b;
      pts [3].x = rectTab.r;
      pts [3].y = rectTab.t;
    }
    else {
      rectTab.t ++;
      pts [0].x = rectTab.l + nHalfHeight;
      pts [0].y = rectTab.t;
      pts [1].x = rectTab.l;
      pts [1].y = rectTab.b;
      pts [2].x = rectTab.r;
      pts [2].y = rectTab.b;
      pts [3].x = rectTab.r - nHalfHeight;
      pts [3].y = rectTab.t;
      rectTab.l += 2;
    }
    im->Polygon(pts, FLAT_POINTS_NUM, clrTab);
  }
  else if (pTabWnd->IsLeftRightRounded()) {
    CList<IPOINT> pts;
    POSITION posLeft = pts.AddHead(CPoint(rectTab.l, rectTab.t));
    posLeft = pts.InsertAfter(posLeft, CPoint(rectTab.l, rectTab.t + 2));
    POSITION posRight = pts.AddTail(CPoint(rectTab.r, rectTab.t));
    posRight = pts.InsertBefore(posRight, CPoint(rectTab.r, rectTab.t + 2));
    int xLeft = rectTab.l + 1;
    int xRight = rectTab.r - 1;
    int y = 0;
    for (y = rectTab.t + 2; y < rectTab.b - 4; y += 2) {
      posLeft = pts.InsertAfter(posLeft, CPoint(xLeft, y));
      posLeft = pts.InsertAfter(posLeft, CPoint(xLeft, y + 2));
      posRight = pts.InsertBefore(posRight, CPoint(xRight, y));
      posRight = pts.InsertBefore(posRight, CPoint(xRight, y + 2));
      xLeft++;
      xRight--;
    }
    if (pTabWnd->GetLocation() == BaseTabWnd::LOCATION_TOP) {
      xLeft--;
      xRight++;
    }
    const int nTabLeft = xLeft - 1;
    const int nTabRight = xRight + 1;
    for (; y < rectTab.b - 1; y++) {
      posLeft = pts.InsertAfter(posLeft, CPoint(xLeft, y));
      posLeft = pts.InsertAfter(posLeft, CPoint(xLeft + 1, y + 1));
      posRight = pts.InsertBefore(posRight, CPoint(xRight, y));
      posRight = pts.InsertBefore(posRight, CPoint(xRight - 1, y + 1));
      if (y == rectTab.b - 2) {
        posLeft = pts.InsertAfter(posLeft, CPoint(xLeft + 1, y + 1));
        posLeft = pts.InsertAfter(posLeft, CPoint(xLeft + 3, y + 1));
        posRight = pts.InsertBefore(posRight, CPoint(xRight, y + 1));
        posRight = pts.InsertBefore(posRight, CPoint(xRight - 2, y + 1));
      }
      xLeft++;
      xRight--;
    }
    posLeft = pts.InsertAfter(posLeft, CPoint(xLeft + 2, rectTab.b));
    posRight = pts.InsertBefore(posRight, CPoint(xRight - 2, rectTab.b));
    IPOINT* points = new IPOINT [pts.GetCount()];
    int i = 0;
    for (POSITION pos = pts.GetHeadPosition(); pos != NULL; i++) {
      points [i] = pts.GetNext(pos);
      if (pTabWnd->GetLocation() == BaseTabWnd::LOCATION_TOP) {
        points [i].y = rectTab.b - (points [i].y - rectTab.t);
      }
    }
    COLOR clr = (clrTab == 0 ? s->clrBtnFace : clrTab);
    OnFillTab(im, rectTab, clr, iTab, bIsActive, pTabWnd, points, (int) pts.GetCount());
    COLOR pen = (s->clrBarShadow);
    COLOR pOLdPen = im->SelectObject(pen);
    for (i = 0; i < pts.GetCount(); i++) {
      if ((i % 2) != 0) {
        int x1 = points [i - 1].x;
        int y1 = points [i - 1].y;
        int x2 = points [i].x;
        int y2 = points [i].y;
        if (x1 > rectTab.CenterPoint().x && x2 > rectTab.CenterPoint().x) {
          x1--;
          x2--;
        }
        if (y2 >= y1) {
          im->MoveTo(x1, y1);
          im->LineTo(x2, y2);
        }
        else {
          im->MoveTo(x2, y2);
          im->LineTo(x1, y1);
        }
      }
    }
    delete [] points;
    im->SelectObject(pOLdPen);
    rectTab.l = nTabLeft;
    rectTab.r = nTabRight;
  }
  else { // 3D Tab
    IRECT rectClip;
    pTabWnd->GetTabsRect(rectClip);
    BOOL bIsCutted = FALSE;
    const BOOL bIsOneNote = pTabWnd->IsOneNoteStyle() || pTabWnd->IsVS2005Style();
    const int nExtra = bIsOneNote ?
        ((iTab == 0 || bIsActive || pTabWnd->IsVS2005Style()) ?
            0 : rectTab.Height()) : 0;
    if (rectTab.l + nExtra + 10 > rectClip.r ||
        rectTab.r - 10 <= rectClip.l) {
      return 0;
    }
    const int iVertOffset = 2;
    const int iHorzOffset = 2;
    const BOOL bIs2005 = pTabWnd->IsVS2005Style();
#define POINTS_NUM 8
    IPOINT pts [POINTS_NUM];
    if (!bIsActive || bIsOneNote || clrTab != 0 || s->m_bAlwaysFillTab) {
      if (clrTab != 0 || bIsOneNote || s->m_bAlwaysFillTab) {
        COLOR clr = (clrTab == 0 ? s->clrBtnFace : clrTab);
        IPOINT* pt = NULL;
        int npt = 0;
        IRECT rectFill = rectTab;
        if (bIsOneNote) {
          IRECT rectFill = rectTab;
          const int nHeight = rectFill.Height();
          pts [0].x = rectFill.l;
          pts [0].y = rectFill.b;
          pts [1].x = rectFill.l;
          pts [1].y = rectFill.b;
          pts [2].x = rectFill.l + 2;
          pts [2].y = rectFill.b;
          pts [3].x = rectFill.l + nHeight;
          pts [3].y = rectFill.t + 2;
          pts [4].x = rectFill.l + nHeight + 4;
          pts [4].y = rectFill.t;
          pts [5].x = rectFill.r - 2;
          pts [5].y = rectFill.t;
          pts [6].x = rectFill.r;
          pts [6].y = rectFill.t + 2;
          pts [7].x = rectFill.r;
          pts [7].y = rectFill.b;
          for (int i = 0; i < POINTS_NUM; i++) {
            if (pts [i].x > rectClip.r) {
              pts [i].x = rectClip.r;
              bIsCutted = TRUE;
            }
            if (pTabWnd->GetLocation() == BaseTabWnd::LOCATION_BOTTOM) {
              pts [i].y = rectFill.b - pts [i].y + rectFill.t - 1;
            }
          }
          pt = pts, npt = POINTS_NUM;
        }
        else {
          rectFill iRectDeflate(&, 1, 0);
          if (pTabWnd->GetLocation() == BaseTabWnd::LOCATION_BOTTOM) {
            rectFill.b--;
          }
          else {
            rectFill.t++;
          }
          rectFill.r = min(rectFill.r, rectClip.r);
        }
        OnFillTab(im, rectFill, clr, iTab, bIsActive, pTabWnd, pt, npt);
        if (bIsOneNote) {
          IRECT rectLeft;
          pTabWnd->GetClientRect(rectLeft);
          rectLeft.r = rectClip.l - 1;
          im->ExcludeClipRect(rectLeft);
          if (iTab > 0 && !bIsActive && iTab != pTabWnd->GetFirstVisibleTabNum()) {
            IRECT rectLeftTab = rectClip;
            rectLeftTab.r = rectFill.l + rectFill.Height() - 10;
            const int nVertOffset = bIs2005 ? 2 : 1;
            if (pTabWnd->GetLocation() == BaseTabWnd::LOCATION_BOTTOM) {
              rectLeftTab.t -= nVertOffset;
            }
            else {
              rectLeftTab.b += nVertOffset;
            }
            im->ExcludeClipRect(rectLeftTab);
          }
          im->Polyline(pts, POINTS_NUM);
          if (bIsCutted) {
            im->MoveTo(rectClip.r, rectTab.t);
            im->LineTo(rectClip.r, rectTab.b);
          }
          IRECT rectRight = rectClip;
          rectRight.l = rectFill.r;
          im->ExcludeClipRect(rectRight);
        }
      }
    }
    COLOR penLight = (s->clrBarHilite);
    COLOR penShadow = (s->clrBarShadow);
    COLOR penDark = (s->clrBarDkShadow);
    COLOR pOldPen;
    if (bIsOneNote) {
      pOldPen = im->SelectObject(penLight);
      ASSERT(pOldPen != NULL);
      if (pTabWnd->GetLocation() == BaseTabWnd::LOCATION_BOTTOM) {
        if (!bIsCutted) {
          int yTop = bIsActive ? pts [7].y - 1 : pts [7].y;
          im->MoveTo(pts [6].x - 1, pts [6].y);
          im->LineTo(pts [7].x - 1, yTop);
        }
      }
      else {
        im->MoveTo(pts [2].x + 1, pts [2].y);
        im->LineTo(pts [3].x + 1, pts [3].y);
        im->MoveTo(pts [3].x + 1, pts [3].y);
        im->LineTo(pts [3].x + 2, pts [3].y);
        im->MoveTo(pts [3].x + 2, pts [3].y);
        im->LineTo(pts [3].x + 3, pts [3].y);
        im->MoveTo(pts [4].x - 1, pts [4].y + 1);
        im->LineTo(pts [5].x + 1, pts [5].y + 1);
        if (!bIsActive && !bIsCutted && s->m_b3DTabWideBorder) {
          im->SelectObject(penShadow);
          im->MoveTo(pts [6].x - 2, pts [6].y - 1);
          im->LineTo(pts [6].x - 1, pts [6].y - 1);
        }
        im->MoveTo(pts [6].x - 1, pts [6].y);
        im->LineTo(pts [7].x - 1, pts [7].y);
      }
    }
    else {
      if (rectTab.r > rectClip.r) {
        IRECT rectTabClip = rectTab;
        rectTabClip.r = rectClip.r;
        im->SetClipBox(&rectTabClip);
      }
      if (pTabWnd->GetLocation() == BaseTabWnd::LOCATION_BOTTOM) {
        pOldPen = im->SelectObject(penLight);
        ASSERT(pOldPen != NULL);
        if (!s->m_b3DTabWideBorder) {
          im->SelectObject(penShadow);
        }
        im->MoveTo(rectTab.l, rectTab.t);
        im->LineTo(rectTab.l, rectTab.b - iVertOffset);
        if (s->m_b3DTabWideBorder) {
          im->SelectObject(penDark);
        }
        im->LineTo(rectTab.l + iHorzOffset, rectTab.b);
        im->LineTo(rectTab.r - iHorzOffset, rectTab.b);
        im->LineTo(rectTab.r, rectTab.b - iVertOffset);
        im->LineTo(rectTab.r, rectTab.t - 1);
        im->SelectObject(penShadow);
        if (s->m_b3DTabWideBorder) {
          im->MoveTo(rectTab.l + iHorzOffset + 1, rectTab.b - 1);
          im->LineTo(rectTab.r - iHorzOffset, rectTab.b - 1);
          im->LineTo(rectTab.r - 1, rectTab.b - iVertOffset);
          im->LineTo(rectTab.r - 1, rectTab.t - 1);
        }
      }
      else {
        pOldPen = im->SelectObject(s->m_b3DTabWideBorder ? penDark : penShadow);
        ASSERT(pOldPen != NULL);
        im->MoveTo(rectTab.r, bIsActive ? rectTab.b : rectTab.b - 1);
        im->LineTo(rectTab.r, rectTab.t + iVertOffset);
        im->LineTo(rectTab.r - iHorzOffset, rectTab.t);
        if (s->m_b3DTabWideBorder) {
          im->SelectObject(penLight);
        }
        im->LineTo(rectTab.l + iHorzOffset, rectTab.t);
        im->LineTo(rectTab.l, rectTab.t + iVertOffset);
        im->LineTo(rectTab.l, rectTab.b);
        if (s->m_b3DTabWideBorder) {
          im->SelectObject(penShadow);
          im->MoveTo(rectTab.r - 1, bIsActive ? rectTab.b : rectTab.b - 1);
          im->LineTo(rectTab.r - 1, rectTab.t + iVertOffset - 1);
        }
      }
    }
    if (bIsActive) {
      const int iBarHeight = 1;
      const int y = (pTabWnd->GetLocation() == BaseTabWnd::LOCATION_BOTTOM) ?
          (rectTab.t - iBarHeight - 1) : (rectTab.b);
      IRECT rectFill(CPoint(rectTab.l, y),
          CSize(rectTab.Width(), iBarHeight + 1));
      COLOR clrActiveTab = pTabWnd->GetTabBkColor(iTab);
      if (bIsOneNote) {
        if (bIs2005) {
          rectFill.l += 3;
        }
        else {
          rectFill iRectOffset(&, 1, 0);
          rectFill.l++;
        }
        if (clrActiveTab == 0) {
          clrActiveTab = s->clrWindow;
        }
      }
      if (clrActiveTab != 0) {
        imdraw_FillRect(im, 0, rectFill, clrActiveTab);
      }
      else {
        imdraw_FillRect(im, 0, rectFill, s->clrBarFace);
      }
    }
    im->SelectObject(pOldPen);
    if (bIsOneNote) {
      const int nLeftMargin = pTabWnd->IsVS2005Style() && bIsActive ?
          rectTab.Height() * 3 / 4 : rectTab.Height();
      const int nRightMargin = pTabWnd->IsVS2005Style() && bIsActive ?
          BaseTabWnd::TAB_IMAGE_MARGIN * 3 / 4 : BaseTabWnd::TAB_IMAGE_MARGIN;
      rectTab.l += nLeftMargin;
      rectTab.r -= nRightMargin;
      if (pTabWnd->IsVS2005Style() && bIsActive && pTabWnd->HasImage(iTab)) {
        rectTab iRectOffset(&, BaseTabWnd::TAB_IMAGE_MARGIN, 0);
      }
    }
    im->SelectClipRgn(NULL);
  }
  COLOR clrText = pTabWnd->GetTabTextColor(iTab);
  COLOR cltTextOld = 0;
  if (!bIsActive && clrText != 0) {
    cltTextOld = im->SetTextColor(clrText);
  }
  if (pTabWnd->IsOneNoteStyle() || pTabWnd->IsVS2005Style()) {
    IRECT rectClip;
    pTabWnd->GetTabsRect(rectClip);
    rectTab.r = min(rectTab.r, rectClip.r - 2);
  }
  im->SetClipBox(&rectClip);
  OnDrawTabContent(im, rectTab, iTab, bIsActive, pTabWnd, 0);
  if (cltTextOld != 0) {
    im->SetTextColor(cltTextOld);
  }
  im->SelectClipRgn(NULL);
}
int VisualManager_OnFillTab(uidraw_t* s, img_t* im, IRECT rectFill, COLOR pbrFill, int iTab, BOOL bIsActive, const BaseTabWnd* pTabWnd, IPOINT* pt, int npt)
{
  ASSERT_VALID(pbrFill);
  ASSERT_VALID(pTabWnd);
  if (bIsActive && !IsHighContastMode(s) &&
      (pTabWnd->IsOneNoteStyle() || pTabWnd->IsVS2005Style() || pTabWnd->IsLeftRightRounded()) &&
      pTabWnd->GetTabBkColor(iTab) == 0) {
    pbrFill = s->clrWindow;
  }
  if (pt) {
    imdraw_poly(im, &rectFill, pt, npt, 1, pbrFill, 0, 0);
  }
  else {
    imdraw_FillRect(im, 0, rectFill, pbrFill);
  }
}
BOOL VisualManager_OnEraseTabsFrame(uidraw_t* s, img_t* im, IRECT rect,
    const BaseTabWnd* pTabWnd)
{
  ASSERT_VALID(pTabWnd);
  COLOR clrActiveTab = pTabWnd->GetTabBkColor(pTabWnd->GetActiveTab());
  if (clrActiveTab == 0) {
    return FALSE;
  }
  im->FillSolidRect(rect, clrActiveTab);
  return TRUE;
}
int VisualManager_OnDrawTabContent(uidraw_t* s, img_t* im, IRECT rectTab,
    int iTab, BOOL bIsActive, const BaseTabWnd* pTabWnd,
    COLOR clrText)
{
  ASSERT_VALID(pTabWnd);
  if (pTabWnd->IsActiveTabCloseButton() && bIsActive) {
    IRECT rectClose = pTabWnd->GetTabCloseButton();
    rectTab.r = rectClose.l;
    OnDrawTabCloseButton(im, rectClose, pTabWnd,
        pTabWnd->IsTabCloseButtonHighlighted(),
        pTabWnd->IsTabCloseButtonPressed(),
        FALSE /* Disabled */);
  }
  CString strText;
  pTabWnd->GetTabLabel(iTab, strText);
  if (pTabWnd->IsFlatTab()) {
    //---------------
    // Draw tab text:
    //---------------
    UINT nFormat = DT_SINGLELINE | DT_CENTER | DT_VCENTER;
    if (pTabWnd->IsDrawNoPrefix()) {
      nFormat |= DT_NOPREFIX;
    }
    im->DrawText(strText, rectTab, nFormat);
  }
  else {
    CSize sizeImage = pTabWnd->GetImageSize();
    UINT uiIcon = pTabWnd->GetTabIcon(iTab);
    CBitmap* hIcon = pTabWnd->GetTabHicon(iTab);
    if (uiIcon == (UINT) - 1 && hIcon == NULL) {
      sizeImage.w = 0;
    }
    if (sizeImage.w + 2 * BaseTabWnd::TAB_IMAGE_MARGIN <= rectTab.Width()) {
      if (hIcon != NULL) {
        //---------------------
        // Draw the tab's icon:
        //---------------------
        IRECT rectImage = rectTab;
        rectImage.t += (rectTab.Height() - sizeImage.h) / 2;
        rectImage.b = rectImage.t + sizeImage.h;
        rectImage.l += IMAGE_MARGIN;
        rectImage.r = rectImage.l + sizeImage.w;
        DrawIconEx(im, rectImage.l, rectImage.t, hIcon,
            0, 0, 0, NULL, DI_NORMAL);
      }
      else {
        const CImageList* pImageList = pTabWnd->GetImageList();
        if (pImageList != NULL && uiIcon != (UINT) - 1) {
          //----------------------
          // Draw the tab's image:
          //----------------------
          IRECT rectImage = rectTab;
          rectImage.t += (rectTab.Height() - sizeImage.h) / 2;
          rectImage.b = rectImage.t + sizeImage.h;
          rectImage.l += IMAGE_MARGIN;
          rectImage.r = rectImage.l + sizeImage.w;
          ASSERT_VALID(pImageList);
          CImageList_Draw(im, pImageList, uiIcon, rectImage.TopLeft(), ILD_TRANSPARENT);
        }
      }
      //------------------------------
      // Finally, draw the tab's text:
      //------------------------------
      IRECT rcText = rectTab;
      rcText.l += sizeImage.w + 2 * TEXT_MARGIN;
      if (rcText.Width() < sizeImage.w * 2 &&
          !pTabWnd->IsLeftRightRounded()) {
        rcText.r -= TEXT_MARGIN;
      }
      if (clrText == 0) {
        clrText = GetTabTextColor(pTabWnd, iTab, bIsActive);
      }
      if (clrText != 0) {
        im->SetTextColor(clrText);
      }
      CString strText;
      pTabWnd->GetTabLabel(iTab, strText);
      UINT nFormat = DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;
      if (pTabWnd->IsDrawNoPrefix()) {
        nFormat |= DT_NOPREFIX;
      }
      if (pTabWnd->IsOneNoteStyle() || pTabWnd->IsVS2005Style()) {
        nFormat |= DT_CENTER;
      }
      else {
        nFormat |= DT_LEFT;
      }
      im->DrawText(strText, rcText, nFormat);
    }
  }
}
int VisualManager_OnDrawTabCloseButton(uidraw_t* s, img_t* im, IRECT rect,
    const BaseTabWnd* /*pTabWnd*/,
    BOOL bIsHighlighted,
    BOOL bIsPressed,
    BOOL /*bIsDisabled*/)
{
  if (bIsHighlighted) {
    imdraw_FillRect(im, 0, rect, s->clrBarFace);
  }
  MenuImages_Draw(im, IdClose, rect, ImageBlack);
  if (bIsHighlighted) {
    if (bIsPressed) {
      imdraw_Draw3dRect(im, 0, &rect, s->clrBarDkShadow, s->clrBarHilite);
    }
    else {
      imdraw_Draw3dRect(im, 0, &rect, s->clrBarHilite, s->clrBarDkShadow);
    }
  }
}
int VisualManager_OnEraseTabsButton(uidraw_t* s, img_t* im, IRECT rect,
    ButtonBase* /*pButton*/,
    BaseTabWnd* /*pWndTab*/)
{
  imdraw_FillRect(im, 0, rect, s->clrBarFace);
}
int VisualManager_OnDrawTabsButtonBorder(uidraw_t* s, img_t* im, IRECT& rect,
    ButtonBase* pButton, UINT uiState,
    BaseTabWnd* /*pWndTab*/)
{
  if (pButton->IsPressed() || (uiState & ODS_SELECTED)) {
    imdraw_Draw3dRect(im, 0, &rect, s->clrBarDkShadow, s->clrBarHilite);
    rect.l += 2;
    rect.t += 2;
  }
  else {
    imdraw_Draw3dRect(im, 0, &rect, s->clrBarHilite, s->clrBarDkShadow);
  }
  iRectDeflate(&rect, 2, 2);
}
int VisualManager_OnDrawTabResizeBar(uidraw_t* s, img_t* im, BaseTabWnd* /*pWndTab*/,
    BOOL bIsVert, IRECT rect,
    COLOR pbrFace, COLOR pPen)
{
  ASSERT_VALID(pbrFace);
  ASSERT_VALID(pPen);
  imdraw_FillRect(im, 0, rect, pbrFace);
  COLOR pOldPen = im->SelectObject(pPen);
  ASSERT_VALID(pOldPen);
  if (bIsVert) {
    im->MoveTo(rect.l, rect.t);
    im->LineTo(rect.l, rect.b);
  }
  else {
    im->MoveTo(rect.l, rect.t);
    im->LineTo(rect.r, rect.t);
  }
  im->SelectObject(pOldPen);
}
COLOR VisualManager_OnFillCommandsListBackground(uidraw_t* s, img_t* im, IRECT rect, BOOL bIsSelected)
{
  ASSERT_VALID(this);
  if (bIsSelected) {
    const int nFrameSize = 1;
    COLOR clr = s->clrHilite;
    imdraw_rect(im, &rect, rect, clr, (~clr) | _RGB(0, 0, 0), 1, BDs->m_ALL);
#if 0
    imdraw_FillRect(im, 0, rect, s->clrHilite);
    iRectDeflate(&rect, 1, 1);
    rect.r--;
    rect.b--;
    im->PatBlt(rect.l, rect.t + nFrameSize, nFrameSize, rect.Height(), PATINVERT);
    im->PatBlt(rect.l, rect.t, rect.Width(), nFrameSize, PATINVERT);
    im->PatBlt(rect.r, rect.t, nFrameSize, rect.Height(), PATINVERT);
    im->PatBlt(rect.l + nFrameSize, rect.b, rect.Width(), nFrameSize, PATINVERT);
#endif
    return s->clrTextHilite;
  }
  imdraw_FillRect(im, 0, rect, s->clrBarFace);
  return s->clrBarText;
}
int VisualManager_OnDrawMenuArrowOnCustomizeList(uidraw_t* s, img_t* im,
    IRECT rectCommand, BOOL bSelected)
{
  IRECT rectTriangle = rectCommand;
  rectTriangle.l = rectTriangle.r - MENUIMAGES_W;
  MenuImages_Draw(im, IdArowRightLarge, rectTriangle,
      bSelected ? ImageWhite : ImageBlack);
  IRECT rectLine = rectCommand;
  rectLine.r = rectTriangle.l - 1;
  rectLine.l = rectLine.r - 2;
  rectLine iRectDeflate(&, 0, 2);
  imdraw_Draw3dRect(im, 0, &rectLine, s->clrBtnShadow, s->clrBtnHilite);
}
int VisualManager_OnDrawTearOffCaption(uidraw_t* s, img_t* im, IRECT rect, BOOL bIsActive)
{
  const int iBorderSize = 2;
  imdraw_FillRect(im, 0, rect, s->clrBarFace);
  iRectDeflate(&rect, iBorderSize, 1);
  im->FillSolidRect(rect,
      bIsActive ?
      s->clrActiveCaption :
      s->clrInactiveCaption);
}
int VisualManager_OnDrawMenuResizeBar(uidraw_t* s, img_t* im, IRECT rect, int /*nResizeFlags*/)
{
  im->FillSolidRect(rect, s->clrInactiveCaption);
}
int VisualManager_OnDrawMenuScrollButton(uidraw_t* s, img_t* im, IRECT rect, BOOL bIsScrollDown,
    BOOL bIsHighlited, BOOL /*bIsPressed*/,
    BOOL /*bIsDisabled*/)
{
  IRECT rectFill = rect;
  rectFill.t -= 2;
  imdraw_FillRect(im, 0, rectFill, s->clrBarFace);
  MenuImages_Draw(im, bIsScrollDown ? IdArowDown : IdArowUp, rect);
  if (bIsHighlited) {
    imdraw_Draw3dRect(im, 0, &rect,
        s->clrBarHilite,
        s->clrBarShadow);
  }
}
int VisualManager_OnDrawMenuSystemButton(uidraw_t* s, img_t* im, IRECT rect,
    UINT uiSystemCommand,
    UINT nStyle, BOOL /*bHighlight*/)
{
  UINT uiState = 0;
  switch (uiSystemCommand) {
  case SC_CLOSE:
    uiState |= DFCS_CAPTIONCLOSE;
    break;
  case SC_MINIMIZE:
    uiState |= DFCS_CAPTIONMIN;
    break;
  case SC_RESTORE:
    uiState |= DFCS_CAPTIONRESTORE;
    break;
  default:
    return 0;
  }
  if (nStyle & TBBS_PRESSED) {
    uiState |= DFCS_PUSHED;
  }
  if (nStyle & TBBS_DISABLED) { // Jan Vasina: Add support for disabled buttons
    uiState |= DFCS_INACTIVE;
  }
  im->DrawFrameControl(rect, DFC_CAPTION, uiState);
}
int VisualManager_OnDrawComboDropButton(uidraw_t* s, img_t* im, IRECT rect,
    BOOL bDisabled,
    BOOL bIsDropped,
    BOOL bIsHighlighted,
    ToolbarComboBoxButton* /*pButton*/)
{
  ASSERT_VALID(this);
  COLOR clrText = im->GetTextColor();
  if (s->m_bIsDrawOnGlass) {
    imdraw_DrawRectR(im, 0, rect, s->clrBarFace, s->clrBarHilite);
    if (bIsDropped) {
      rect iRectOffset(&, 1, 1);
      imdraw_DrawRectR(im, 0, rect, 0, s->clrBarShadow);
    }
    else if (bIsHighlighted) {
      imdraw_DrawRectR(im, 0, rect, 0, s->clrBarShadow);
    }
  }
  else {
    imdraw_FillRect(im, 0, rect, s->clrBarFace);
    imdraw_Draw3dRect(im, 0, &rect, s->clrBarHilite, s->clrBarHilite);
    if (bIsDropped) {
      rect iRectOffset(&, 1, 1);
      imdraw_Draw3dRect(im, 0, &rect, s->clrBarShadow, s->clrBarHilite);
    }
    else if (bIsHighlighted) {
      imdraw_Draw3dRect(im, 0, &rect, s->clrBarHilite, s->clrBarShadow);
    }
  }
  MenuImages_Draw(im, IdArowDown, rect,
      bDisabled ? ImageGray : ImageBlack);
  im->SetTextColor(clrText);
}
int VisualManager_OnDrawComboBorder(uidraw_t* s, img_t* im, IRECT rect,
    BOOL /*bDisabled*/,
    BOOL bIsDropped,
    BOOL bIsHighlighted,
    ToolbarComboBoxButton* /*pButton*/)
{
  if (bIsHighlighted || bIsDropped) {
    if (s->m_bMenuFlatLook) {
      iRectDeflate(&rect, 1, 1);
    }
    if (s->m_bIsDrawOnGlass) {
      imdraw_DrawRectR(im, 0, rect, 0, s->clrBarDkShadow);
    }
    else {
      if (s->m_bMenuFlatLook) {
        imdraw_Draw3dRect(im, 0, &rect, s->clrBarDkShadow, s->clrBarDkShadow);
      }
      else {
        imdraw_Draw3dRect(im, 0, &rect, s->clrBarShadow, s->clrBarHilite);
      }
    }
  }
}
int VisualManager_OnDrawStatusBarPaneBorder(uidraw_t* s, img_t* im, StatusBar* /*pBar*/,
    IRECT rectPane, UINT /*uiID*/, UINT nStyle)
{
  ASSERT_VALID(this);
  if (!(nStyle & SBPS_NOBORDERS)) {
    // draw the borders
    COLOR clrHilite;
    COLOR clrShadow;
    if (nStyle & SBPS_POPOUT) {
      // reverse colors
      clrHilite = s->clrBarShadow;
      clrShadow = s->clrBarHilite;
    }
    else {
      // normal colors
      clrHilite = s->clrBarHilite;
      clrShadow = s->clrBarShadow;
    }
    imdraw_Draw3dRect(im, 0, &rectPane, clrShadow, clrHilite);
  }
}
COLOR VisualManager_OnFillMiniFrameCaption(uidraw_t* s, img_t* im,
    IRECT rectCaption,
    MiniFrameWnd* pFrameWnd, BOOL bActive)
{
  ASSERT_VALID(pFrameWnd);
  COLOR clr = (bActive ? s->clrActiveCaption : s->clrInactiveCaption);
  imdraw_FillRect(im, 0, rectCaption, clr);
  // get the text color
  return bActive ? s->clrCaptionText : s->clrInactiveCaptionText;
}
int VisualManager_OnDrawMiniFrameBorder(
    img_t* im, MiniFrameWnd* pFrameWnd,
    IRECT rectBorder, IRECT rectBorderSize)
{
  ASSERT_VALID(pFrameWnd);
  int preDockState = pFrameWnd->GetPreDockState();
  if (preDockState == BCGP_PDS_DOCK_REGULAR) {
    // draw outer edge;
    imdraw_Draw3dRect(im, 0, &rectBorder, _RGB(127, 0, 0), s->clrBarDkShadow);
    rectBorder iRectDeflate(&, 1, 1);
    imdraw_Draw3dRect(im, 0, &rectBorder, s->clrBarHilite, _RGB(127, 0, 0));
  }
  else if (preDockState == BCGP_PDS_DOCK_TO_TAB) {
    // draw outer edge;
    imdraw_Draw3dRect(im, 0, &rectBorder, _RGB(0, 0, 127), s->clrBarDkShadow);
    rectBorder iRectDeflate(&, 1, 1);
    imdraw_Draw3dRect(im, 0, &rectBorder, s->clrBarHilite, _RGB(0, 0, 127));
  }
  else {
    // draw outer edge;
    imdraw_Draw3dRect(im, 0, &rectBorder, s->clrBarFace, s->clrBarDkShadow);
    rectBorder iRectDeflate(&, 1, 1);
    imdraw_Draw3dRect(im, 0, &rectBorder, s->clrBarHilite, s->clrBarShadow);
  }
  // draw the inner egde
  rectBorder iRectDeflate(&, rectBorderSize.r - 2, rectBorderSize.t - 2);
  imdraw_Draw3dRect(im, 0, &rectBorder, s->clrBarFace, s->clrBarFace);
  rectBorder.InflateRect(1, 1);
  imdraw_Draw3dRect(im, 0, &rectBorder, s->clrBarFace, s->clrBarFace);
}
int VisualManager_OnDrawFloatingToolbarBorder(
    img_t* im, BaseToolBar* /*pToolBar*/,
    IRECT rectBorder, IRECT /*rectBorderSize*/)
{
  imdraw_Draw3dRect(im, 0, &rectBorder, s->clrBarFace, s->clrBarDkShadow);
  rectBorder iRectDeflate(&, 1, 1);
  imdraw_Draw3dRect(im, 0, &rectBorder, s->clrBarHilite, s->clrBarShadow);
  rectBorder iRectDeflate(&, 1, 1);
  imdraw_Draw3dRect(im, 0, &rectBorder, s->clrBarFace, s->clrBarFace);
}
COLOR GetHotTextColor()
{
  return s->clrBtnText;
}
COLOR VisualManager_GetToolbarButtonTextColor(int* pButton,
    int state)
{
  BOOL bDisabled = !pButton->IsEnabled();
  if (pButton->GetClass() == CLASS_OutlookButton) {
    if (IsHighContastMode(s)) {
      return bDisabled ? s->clrGrayedText : s->clrWindowText;
    }
    return bDisabled ? s->clrBtnFace : s->clrWindow;
  }
  return(bDisabled ? s->clrGrayedText :
      (state == ButtonsIsHighlighted) ?
      GetHotTextColor() : s->clrBarText);
}
BOOL VisualManager_IsToolBarButtonDefaultBackground(int* pButton,
    int /*state*/)
{
  return !(pButton->GetStyle() & (TBBS_CHECKED | TBBS_INDETERMINATE));
}
int VisualManager_OnFillOutlookPageButton(uidraw_t* s, img_t* im,
    const IRECT& rect,
    BOOL /*bIsHighlighted*/, BOOL /*bIsPressed*/,
    COLOR& clrText)
{
  imdraw_FillRect(im, 0, rect, s->clrBarFace);
  clrText = s->clrBarText;
}
int VisualManager_OnDrawOutlookPageButtonBorder(
    img_t* im, IRECT& rectBtn, BOOL bIsHighlighted, BOOL bIsPressed)
{
  if (bIsHighlighted && bIsPressed) {
    imdraw_Draw3dRect(im, 0, &rectBtn, s->clrBarDkShadow, s->clrBarFace);
    rectBtn iRectDeflate(&, 1, 1);
    imdraw_Draw3dRect(im, 0, &rectBtn, s->clrBarShadow, s->clrBarHilite);
  }
  else {
    if (bIsHighlighted || bIsPressed) {
      imdraw_Draw3dRect(im, 0, &rectBtn, s->clrBarFace, s->clrBarDkShadow);
      rectBtn iRectDeflate(&, 1, 1);
    }
    imdraw_Draw3dRect(im, 0, &rectBtn, s->clrBarHilite, s->clrBarShadow);
  }
  rectBtn iRectDeflate(&, 1, 1);
}
COLOR VisualManager_GetCaptionBarTextColor(CaptionBar* pBar)
{
  ASSERT_VALID(pBar);
  return pBar->IsMessageBarMode() ? _GetSysColor(CLR_INFOTEXT) : s->clrWindow;
}
int VisualManager_OnDrawCaptionBarBorder(uidraw_t* s, img_t* im,
    CaptionBar* /*pBar*/, IRECT rect, COLOR clrBarBorder, BOOL bFlatBorder)
{
  if (clrBarBorder == (COLOR) - 1) {
    imdraw_FillRect(im, 0, rect, s->clrBarFace);
  }
  else {
    imdraw_FillRect(im, 0, rect, clrBarBorder);
  }
  if (!bFlatBorder) {
    imdraw_Draw3dRect(im, 0, &rect, s->clrBarHilite, s->clrBarShadow);
  }
}
int VisualManager_OnDrawCaptionBarInfoArea(uidraw_t* s, img_t* im, CaptionBar* /*pBar*/, IRECT rect)
{
  imdraw_FillRect(im, 0, rect, GetSysColorBrush(CLR_INFOBK));
  imdraw_Draw3dRect(im, 0, &rect, s->clrBarShadow, s->clrBarHilite);
  iRectDeflate(&rect, 1, 1);
  imdraw_Draw3dRect(im, 0, &rect, s->clrBarHilite, s->clrBarShadow);
}
COLOR VisualManager_OnFillCaptionBarButton(uidraw_t* s, img_t* im, CaptionBar* pBar,
    IRECT rect, BOOL bIsPressed, BOOL bIsHighlighted,
    BOOL bIsDisabled, BOOL bHasDropDownArrow,
    BOOL bIsSysButton)
{
  UNREFERENCED_PARAMETER(bIsPressed);
  UNREFERENCED_PARAMETER(bIsHighlighted);
  UNREFERENCED_PARAMETER(bIsDisabled);
  UNREFERENCED_PARAMETER(bHasDropDownArrow);
  UNREFERENCED_PARAMETER(bIsSysButton);
  ASSERT_VALID(pBar);
  if (!pBar->IsMessageBarMode()) {
    return 0;
  }
  imdraw_FillRect(im, 0, rect, s->clrBarFace);
  return bIsDisabled ? s->clrGrayedText : s->clrBarText;
}
int VisualManager_OnDrawCaptionBarButtonBorder(uidraw_t* s, img_t* im, CaptionBar* pBar,
    IRECT rect, BOOL bIsPressed, BOOL bIsHighlighted,
    BOOL bIsDisabled, BOOL bHasDropDownArrow,
    BOOL bIsSysButton)
{
  UNREFERENCED_PARAMETER(bIsDisabled);
  UNREFERENCED_PARAMETER(bHasDropDownArrow);
  UNREFERENCED_PARAMETER(bIsSysButton);
  ASSERT_VALID(pBar);
  if (bIsPressed) {
    imdraw_Draw3dRect(im, 0, &rect, s->clrBarDkShadow, s->clrBarHilite);
  }
  else if (bIsHighlighted || pBar->IsMessageBarMode()) {
    imdraw_Draw3dRect(im, 0, &rect, s->clrBarHilite, s->clrBarDkShadow);
  }
}
int VisualManager_OnDrawStatusBarProgress(uidraw_t* s, img_t* im, StatusBar* /*pStatusBar*/,
    IRECT rectProgress, int nProgressTotal, int nProgressCurr,
    COLOR clrBar, COLOR clrProgressBarDest, COLOR clrProgressText,
    BOOL bProgressText)
{
  if (nProgressTotal == 0) {
    return 0;
  }
  IRECT rectComplete = rectProgress;
  rectComplete.r = rectComplete.l +
      nProgressCurr * rectComplete.Width() / nProgressTotal;
  if (clrProgressBarDest == 0) {
    // one-color bar
    COLOR clr = (clrBar);
    imdraw_FillRect(im, 0, rectComplete, clr);
  }
  else {
    // gradient bar:
    FillGradient(im, 0, rectComplete, clrBar, clrProgressBarDest, FALSE);
  }
  if (bProgressText) {
    char strText[256];
    _snprintf(strText, 256, ("%d%%"), nProgressCurr * 100 / nProgressTotal);
    COLOR clrText = im->SetTextColor(s->clrBarText);
    im->DrawText(strText, rectProgress, DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
    im->SetClipBox(&rectComplete);
    im->SetTextColor(clrProgressText == 0 ?
        s->clrTextHilite : clrProgressText);
    im->DrawText(strText, rectProgress, DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
    im->SetTextColor(clrText);
  }
}
int VisualManager_OnFillHeaderCtrlBackground(HeaderCtrl* pCtrl,
    img_t* im, IRECT rect)
{
  imdraw_FillRect(im, 0, &rect, pCtrl->IsDialogControl() ? s->clrBtnFace : s->clrBarFace);
}
int VisualManager_OnDrawHeaderCtrlBorder(HeaderCtrl* pCtrl, img_t* im,
    IRECT& rect, BOOL bIsPressed, BOOL /*bIsHighlighted*/)
{
  if (bIsPressed) {
    if (pCtrl->IsDialogControl()) {
      imdraw_Draw3dRect(im, 0, &rect, s->clrBtnShadow, s->clrBtnShadow);
    }
    else {
      imdraw_Draw3dRect(im, 0, &rect, s->clrBarShadow, s->clrBarShadow);
    }
    rect.l++;
    rect.t++;
  }
  else {
    if (pCtrl->IsDialogControl()) {
      imdraw_Draw3dRect(im, 0, &rect, s->clrBtnHilite, s->clrBtnShadow);
    }
    else {
      imdraw_Draw3dRect(im, 0, &rect, s->clrBarHilite, s->clrBarShadow);
    }
  }
}
int VisualManager_OnDrawHeaderCtrlSortArrow(HeaderCtrl* pCtrl,
    img_t* im,
    IRECT& rectArrow, BOOL bIsUp)
{
  DoDrawHeaderSortArrow(im, rectArrow, bIsUp, pCtrl != NULL && pCtrl->IsDialogControl());
}
int VisualManager_OnDrawStatusBarSizeBox(uidraw_t* s, img_t* im, StatusBar* /*pStatBar*/,
    IRECT rectSizeBox)
{
  CFont* pOldFont = im->SelectObject(s->fontMarlett);
  ASSERT(pOldFont != NULL);
  const CString strSizeBox(("o")); // Char of the sizing box in "Marlett" font
  UINT nTextAlign = im->SetTextAlign(TA_RIGHT | TA_BOTTOM);
  COLOR clrText = im->SetTextColor(s->clrBarShadow);
  im->ExtTextOut(rectSizeBox.r, rectSizeBox.b, ETO_CLIPPED, &rectSizeBox, strSizeBox, NULL);
  im->SelectObject(pOldFont);
  im->SetTextColor(clrText);
  im->SetTextAlign(nTextAlign);
}
int VisualManager_OnDrawEditBorder(uidraw_t* s, img_t* im, IRECT rect,
    BOOL /*bDisabled*/,
    BOOL bIsHighlighted,
    ToolbarEditBoxButton* /*pButton*/)
{
  if (bIsHighlighted) {
    im->DrawEdge(rect, EDGE_SUNKEN, BF_RECT);
  }
}
#ifndef BCGP_EXCLUDE_TASK_PANE
int VisualManager_OnFillTasksPaneBackground(uidraw_t* s, img_t* im, IRECT rectWorkArea)
{
  imdraw_FillRect(im, 0, rectWorkArea, s->clrWindow);
}
int VisualManager_OnDrawTasksGroupCaption(uidraw_t* s, img_t* im, TasksGroup* pGroup,
    BOOL bIsHighlighted, BOOL bIsSelected, BOOL bCanCollapse)
{
  ASSERT(pGroup != NULL);
  BOOL bIsToolBox = FALSE;
  IRECT rectGroup = pGroup->GetRect();
  if (bIsToolBox) {
    IRECT rectFill = rectGroup;
    rectFill iRectDeflate(&, 1, 0, 1, 1);
    imdraw_FillRect(im, 0, rectFill, s->clrBarShadow);
    if (bCanCollapse) {
      //--------------------
      // Draw expanding box:
      //--------------------
      int nBoxSize = 9;
      int nBoxOffset = 6;
      if (s->GetRibbonImageScale() != 1.) {
        nBoxSize = (int)(.5 + nBoxSize * s->GetRibbonImageScale());
      }
      IRECT rectButton = rectFill;
      rectButton.l += nBoxOffset;
      rectButton.r = rectButton.l + nBoxSize;
      rectButton.t = RCY(&rectButton) / 2 - nBoxSize / 2;
      rectButton.b = rectButton.t + nBoxSize;
      imdraw_FillRect(im, 0, rectButton, s->clrBarFace);
      OnDrawExpandingBox(im, rectButton, !pGroup->IsCollapsed(),
          s->clrBarText);
      rectGroup.l = rectButton.r + nBoxOffset;
      bCanCollapse = FALSE;
    }
  }
  else {
    // ---------------------------------
    // Draw caption background(Windows)
    // ---------------------------------
    COLOR clrBckOld = im->GetBkColor();
    im->FillSolidRect(rectGroup,
        (pGroup->IsSpecial() ? s->clrHilite : s->clrBarFace));
    im->SetBkColor(clrBckOld);
  }
  // ---------------------------
  // Draw an icon if it presents
  // ---------------------------
  img_t* s->m_hIcon = pGroup->GetIcon();
  BOOL bShowIcon = (s->m_hIcon != NULL
      && s->m_hIcon->w < rectGroup.Width() - rectGroup.Height());
  if (bShowIcon) {
    OnDrawTasksGroupIcon(im, pGroup, 5, bIsHighlighted, bIsSelected, bCanCollapse);
  }
  // -----------------------
  // Draw group caption text
  // -----------------------
  CFont* pFontOld = im->SelectObject(s->fontBold);
  COLOR clrTextOld = im->GetTextColor();
  if (bIsToolBox) {
    im->SetTextColor(s->clrWindow);
  }
  else {
    if (bCanCollapse && bIsHighlighted) {
      clrTextOld = im->SetTextColor(pGroup->GetTextHotColor() == 0 ?
          (pGroup->IsSpecial() ? s->clrWindow : s->clrWindowText) :
            pGroup->GetTextHotColor());
    }
    else {
      clrTextOld = im->SetTextColor(pGroup->GetTextColor() == 0 ?
          (pGroup->IsSpecial() ? s->clrWindow : s->clrWindowText) :
            pGroup->GetTextColor());
    }
  }
  int nBkModeOld = im->SetBkMode(TRANSPARENT);
  int nTaskPaneHOffset = pGroup->GetGroupCaptionHorzOffset();
  int nTaskPaneVOffset = pGroup->GetGroupCaptionVertOffset();
  int nCaptionHOffset = (nTaskPaneHOffset != -1 ? nTaskPaneHOffset : s->m_nGroupCaptionHorzOffset);
  IRECT rectText = rectGroup;
  rectText.l += (bShowIcon ? s->m_hIcon->w + 5 : nCaptionHOffset);
  rectText.t += (nTaskPaneVOffset != -1 ? nTaskPaneVOffset : s->m_nGroupCaptionVertOffset);
  rectText.r = max(rectText.l,
      rectText.r - (bCanCollapse ? rectGroup.Height() : nCaptionHOffset));
  im->DrawText(pGroup->GetName(), rectText, DT_SINGLELINE | DT_VCENTER);
  im->SetBkMode(nBkModeOld);
  im->SelectObject(pFontOld);
  im->SetTextColor(clrTextOld);
  CString s->m_strName = pGroup->GetStrName();
  // -------------------------
  // Draw group caption button
  // -------------------------
  if (bCanCollapse && !s->m_strName.IsEmpty()) {
    CSize sizeButton = CSize(MENUIMAGES_W, MENUIMAGES_H);
    IRECT rectButton = rectGroup;
    rectButton.l = max(rectButton.l,
        rectButton.r - (rectButton.Height() + 1) / 2 - (sizeButton.w + 1) / 2);
    rectButton.t = max(rectButton.t,
        rectButton.b - (rectButton.Height() + 1) / 2 - (sizeButton.h + 1) / 2);
    rectButton.r = rectButton.l + sizeButton.w;
    rectButton.b = rectButton.t + sizeButton.h;
    if (rectButton.r <= rectGroup.r && rectButton.b <= rectGroup.b) {
      if (bIsHighlighted) {
        // Draw button frame
        imdraw_Draw3dRect(im, 0, &rectButton, s->clrWindow, s->clrBarShadow, s->clrBarFace);
      }
      if (pGroup->IsSpecial()) {
        if (!pGroup->IsCollapsed()) {
          MenuImages_Draw(im, IdArowUp, rectButton.TopLeft());
        }
        else {
          MenuImages_Draw(im, IdArowDown, rectButton.TopLeft());
        }
      }
      else {
        if (!pGroup->IsCollapsed()) {
          MenuImages_Draw(im, IdArowUp, rectButton.TopLeft());
        }
        else {
          MenuImages_Draw(im, IdArowDown, rectButton.TopLeft());
        }
      }
    }
  }
}
int VisualManager_OnDrawTasksGroupIcon(uidraw_t* s, img_t* im, TasksGroup* pGroup,
    int nIconHOffset, BOOL /*bIsHighlighted*/, BOOL /*bIsSelected*/, BOOL /*bCanCollapse*/)
{
  ASSERT_VALID(pGroup);
  img_t* s->m_hIcon = pGroup->GetIcon();
  if (s->m_hIcon == NULL) {
    return 0;
  }
  int nTaskPaneVOffset = pGroup->GetGroupCaptionVertOffset();
  IRECT rectImage = pGroup->GetRect();
  rectImage.t += (nTaskPaneVOffset != -1 ? nTaskPaneVOffset : s->m_nGroupCaptionVertOffset);
  rectImage.r = rectImage.l + s->m_hIcon->w + nIconHOffset;
  int x = max(0, (rectImage.Width() - s->m_hIcon->w) / 2);
  int y = max(0, (rectImage.Height() - s->m_hIcon->h) / 2);
  ::DrawIconEx(im,
      rectImage.l + x, rectImage.b - y - s->m_hIcon->h,
      s->m_hIcon, s->m_hIcon->w, s->m_hIcon->h,
      0, NULL, DI_NORMAL);
}
int VisualManager_OnFillTasksGroupInterior(uidraw_t* s, img_t* im, IRECT rect, BOOL /*bSpecial*/)
{
}
int VisualManager_OnDrawTasksGroupAreaBorder(uidraw_t* s, img_t* im, IRECT rect, BOOL bSpecial,
    BOOL bNoTitle)
{
  // Draw caption background:
  COLOR pPenOld = (COLOR) im->SelectObject(bSpecial ? s->clrHilite : s->clrBarFace);
  im->MoveTo(rect.l, rect.t);
  im->LineTo(rect.l, rect.b - 1);
  im->LineTo(rect.r - 1, rect.b - 1);
  im->LineTo(rect.r - 1, rect.t);
  if (bNoTitle) {
    im->LineTo(rect.l, rect.t);
  }
  else {
    im->LineTo(rect.r - 1, rect.t - 1);
  }
  im->SelectObject(pPenOld);
}
int VisualManager_OnDrawTask(uidraw_t* s, img_t* im, Task* pTask, CImageList* pIcons,
    BOOL bIsHighlighted, BOOL /*bIsSelected*/)
{
  ASSERT_VALID(pIcons);
  ASSERT(pTask != NULL);
  IRECT rectText = pTask->GetRect();
  if (pTask->IsSeparator()) {
    COLOR pPenOld = im->SelectObject(s->clrBarFace);
    im->MoveTo(rectText.l, RCY(&rectText) / 2);
    im->LineTo(rectText.r, RCY(&rectText) / 2);
    im->SelectObject(pPenOld);
    return 0;
  }
  // ---------
  // Draw icon
  // ---------
  CSize sizeIcon(0, 0);
  ImageList_GetIconSize(pIcons, (int*) &sizeIcon.w, (int*) &sizeIcon.h);
  int s->m_nIcon = pTask->GetIconIndex();
  if (s->m_nIcon >= 0 && sizeIcon.w > 0) {
    CImageList_Draw(im, pIcons, s->m_nIcon, rectText.TopLeft(), ILD_TRANSPARENT);
  }
  int nTaskPaneOffset = pTask->GetTasksIconHorzOffset();
  rectText.l += sizeIcon.w + (nTaskPaneOffset != -1 ? nTaskPaneOffset : s->m_nTasksIconHorzOffset);
  // ---------
  // Draw text
  // ---------
  BOOL bIsLabel = (pTask->GetCommandID() == 0);
  CFont* pFontOld = NULL;
  COLOR clrTextOld = im->GetTextColor();
  if (bIsLabel) {
    pFontOld = im->SelectObject(
        pTask->IsBold() ? s->fontBold : s->fontRegular);
    im->SetTextColor(pTask->GetTextColor() == 0 ?
        s->clrWindowText : pTask->GetTextColor());
  }
  else if (!pTask->IsEnabled()) {
    im->SetTextColor(s->clrGrayedText);
    pFontOld = im->SelectObject(s->fontRegular);
  }
  else if (bIsHighlighted) {
    pFontOld = im->SelectObject(s->fontUnderline);
    im->SetTextColor(pTask->GetTextHotColor() == 0 ?
        s->clrWindowText : pTask->GetTextHotColor());
  }
  else {
    pFontOld = im->SelectObject(s->fontRegular);
    im->SetTextColor(pTask->GetTextColor() == 0 ?
        s->clrWindowText : pTask->GetTextColor());
  }
  int nBkModeOld = im->SetBkMode(TRANSPARENT);
  TasksPane* pTaskPane = pTask->GetTaskPane();
  ASSERT_VALID(pTaskPane);
  BOOL bMultiline = bIsLabel ?
      pTaskPane->IsWrapLabelsEnabled() : pTaskPane->IsWrapTasksEnabled();
  if (bMultiline) {
    im->DrawText(pTask->GetName(), rectText, DT_WORDBREAK);
  }
  else {
    CString strText = pTask->GetName();
    strText.Remove(('\n'));
    strText.Remove(('\r'));
    im->DrawText(strText, rectText, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
  }
  im->SetBkMode(nBkModeOld);
  im->SelectObject(pFontOld);
  im->SetTextColor(clrTextOld);
}
int VisualManager_OnDrawScrollButtons(uidraw_t* s, img_t* im, const IRECT& rect, const int nBorderSize,
    int iImage, BOOL bHilited)
{
  IRECT rectImage(CPoint(0, 0), MenuImages_Size());
  IRECT rectFill = rect;
  rectFill.t -= nBorderSize;
  imdraw_FillRect(im, 0, rectFill, s->clrBarFace);
  if (bHilited) {
    HighlightRect(im, 0, rect);
    imdraw_Draw3dRect(im, 0, &rect,
        s->clrBarHilite,
        s->clrBarDkShadow);
  }
  MenuImages_Draw(im, (IMAGES_IDS) iImage, rect);
}
int VisualManager_OnDrawToolBoxFrame(uidraw_t* s, img_t* im, const IRECT& rect)
{
  imdraw_Draw3dRect(im, 0, &rect, s->clrBarFace, s->clrBarFace);
}
#endif // BCGP_EXCLUDE_TASK_PANE
int VisualManager_OnDrawSlider(uidraw_t* s, img_t* im, Slider* pSlider, IRECT rect, BOOL bAutoHideMode)
{
  ASSERT_VALID(pSlider);
  IRECT rectScreen = s->s->m_rectVirtual;
  pSlider->ScreenToClient(&rectScreen);
  IRECT rectFill = rect;
  rectFill.l = min(rectFill.l, rectScreen.l);
  OnFillBarBackground(im, pSlider, rectFill, rect);
  if (bAutoHideMode) {
    // draw outer edge;
    DWORD dwAlgn = pSlider->GetCurrentAlignment();
    IRECT rectBorder = rect;
    COLOR clrBorder = s->clrBarDkShadow;
    if (dwAlgn & CBRS_ALIGN_LEFT) {
      rectBorder.l = rectBorder.r;
    }
    else if (dwAlgn & CBRS_ALIGN_RIGHT) {
      rectBorder.r = rectBorder.l;
      clrBorder = s->clrBarHilite;
    }
    else if (dwAlgn & CBRS_ALIGN_TOP) {
      rectBorder.t = rectBorder.b;
    }
    else if (dwAlgn & CBRS_ALIGN_BOTTOM) {
      rectBorder.b = rectBorder.t;
      clrBorder = s->clrBarHilite;
    }
    else {
      ASSERT(FALSE);
      return 0;
    }
    imdraw_Draw3dRect(im, 0, &rectBorder, clrBorder, clrBorder);
  }
}
int VisualManager_OnDrawSplitterBorder(uidraw_t* s, img_t* im, SplitterWnd* /*pSplitterWnd*/, IRECT rect)
{
  imdraw_Draw3dRect(im, 0, &rect, s->clrBarShadow, s->clrBarHilite);
  rect.InflateRect(-CX_BORDER, -CY_BORDER);
  imdraw_Draw3dRect(im, 0, &rect, s->clrBarFace, s->clrBarFace);
}
int VisualManager_OnDrawSplitterBox(uidraw_t* s, img_t* im, SplitterWnd* /*pSplitterWnd*/, IRECT& rect)
{
  imdraw_Draw3dRect(im, 0, &rect, s->clrBarFace, s->clrBarShadow);
}
int VisualManager_OnFillSplitterBackground(uidraw_t* s, img_t* im, SplitterWnd* /*pSplitterWnd*/, IRECT rect)
{
  im->FillSolidRect(rect, s->clrBarFace);
}
int VisualManager_OnDrawCheckBox(uidraw_t* s, img_t* im, IRECT rect,
    BOOL bHighlighted,
    BOOL bChecked,
    BOOL bEnabled)
{
  OnDrawCheckBoxEx(im, rect, bChecked ? 1 : 0, bHighlighted, FALSE, bEnabled);
}
int VisualManager_OnDrawCheckBoxEx(uidraw_t* s, img_t* im, IRECT rect,
    int nState,
    BOOL bHighlighted,
    BOOL /*bPressed*/,
    BOOL bEnabled)
{
  if (s->m_bIsDrawOnGlass) {
    iRectDeflate(&rect, 1, 1);
    imdraw_DrawRectR(im, 0, rect,
        bEnabled ? s->clrWindow : s->clrBarFace,
        s->clrBarShadow);
    if (nState == 1) {
      MenuImages_Draw(im, IdCheck, rect,
          ImageBlack);
    }
    return 0;
  }
  if (bHighlighted) {
    im->DrawFocusRect(rect);
  }
  iRectDeflate(&rect, 1, 1);
  im->FillSolidRect(&rect, bEnabled ? s->clrWindow :
      s->clrBarFace);
  imdraw_Draw3dRect(im, 0, &rect,
      s->clrBarDkShadow,
      s->clrBarHilite);
  iRectDeflate(&rect, 1, 1);
  imdraw_Draw3dRect(im, 0, &rect,
      s->clrBarShadow,
      s->clrBarLight);
  if (nState == 1) {
    MenuImages_Draw(im, IdCheck, rect,
        ImageBlack);
  }
  else if (nState == 2) {
    iRectDeflate(&rect, 1, 1);
    WORD HatchBits [8] = { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 };
    CBitmap bmp;
    bmp.CreateBitmap(8, 8, 1, 1, HatchBits);
    //CBrush br;
    //br.CreatePatternBrush(&bmp);
    imdraw_image_pattern(im, 0, rect, &bmp, 0);
  }
}
int VisualManager_OnDrawRadioButton(uidraw_t* s, img_t* im, IRECT rect,
    BOOL bOn,
    BOOL bHighlighted,
    BOOL /*bPressed*/,
    BOOL bEnabled)
{
  iRectDeflate(&rect, 1, 1);
  imdraw_EllipseR(im, 0, rect,
      bEnabled ? s->clrBarHilite : s->clrBarFace,
      bHighlighted && bEnabled ? s->clrBarDkShadow : s->clrBarShadow, 1);
  if (bOn) {
    iRectDeflate(&rect, rect.Width() / 3, rect.Width() / 3);
    imdraw_EllipseR(im, 0, rect,
        bHighlighted && bEnabled ? s->clrBarDkShadow : s->clrBarShadow,
        0, 0);
  }
}
int VisualManager_OnDrawSpinButtons(uidraw_t* s, img_t* im, IRECT rectSpin,
    int nState, BOOL bOrientation, SpinButtonCtrl* /*pSpinCtrl*/)
{
  ASSERT_VALID(this);
  rectSpin iRectDeflate(&, 1, 1);
  IRECT rect[2];
  rect[0] = rect[1] = rectSpin;
  if (!bOrientation) {
    rect[0] iRectDeflate(&, 0, 0, 0, rect[0].Height() / 2);
    rect[1].t = rect[0].b + 1;
  }
  else {
    rect[0] iRectDeflate(&, 0, 0, rect[0].Width() / 2, 0);
    rect[1].l = rect[0].r + 1;
  }
  if (s->m_bIsDrawOnGlass) {
    imdraw_DrawRectR(im, 0, rectSpin, s->clrBarFace, s->clrBarHilite);
  }
  else {
    imdraw_FillRect(im, 0, rectSpin, s->clrBarFace);
    imdraw_Draw3dRect(im, 0, &rectSpin, s->clrBarHilite, s->clrBarHilite);
  }
  IMAGES_IDS id[2][2] = {{IdArowUp, IdArowDown}, {IdArowLeft, IdArowRight}};
  int idxPressed = (nState & (SPIN_PRESSEDUP | SPIN_PRESSEDDOWN)) - 1;
  BOOL bDisabled = nState & SPIN_DISABLED;
  for (int i = 0; i < 2; i ++) {
    if (s->m_bIsDrawOnGlass) {
      if (idxPressed == i) {
        imdraw_DrawRectR(im, 0, rect[i], 0, s->clrBarShadow);
      }
      else {
        imdraw_DrawRectR(im, 0, rect[i], 0, s->clrBarHilite);
      }
    }
    else {
      if (idxPressed == i) {
        imdraw_Draw3dRect(im, 0, &rect[i], s->clrBarShadow, s->clrBarHilite);
      }
      else {
        imdraw_Draw3dRect(im, 0, &rect[i], s->clrBarHilite, s->clrBarShadow);
      }
    }
    MenuImages_Draw(im, id[bOrientation ? 1 : 0][i], rect[i],
        bDisabled ? ImageGray : ImageBlack);
  }
}
int VisualManager_OnDrawExpandingBox(uidraw_t* s, img_t* im, IRECT rect, BOOL bIsOpened,
    COLOR colorBox)
{
  imdraw_Draw3dRect(im, 0, &rect, colorBox, colorBox);
  iRectDeflate(&rect, 2, 2);
  COLOR penLine = (s->clrBarText);
  COLOR pOldPen = im->SelectObject(penLine);
  CPoint ptCenter = rect.CenterPoint();
  im->MoveTo(rect.l, ptCenter.y);
  im->LineTo(rect.r, ptCenter.y);
  if (!bIsOpened) {
    im->MoveTo(ptCenter.x, rect.t);
    im->LineTo(ptCenter.x, rect.b);
  }
  im->SelectObject(pOldPen);
}
COLOR VisualManager_OnFillCalendarBarNavigationRow(uidraw_t* s, img_t* im, IRECT rect)
{
  COLOR clr = (s->clrInactiveCaption);
  imdraw_FillRect(im, 0, rect, clr);
  imdraw_Draw3dRect(im, 0, &rect, s->clrInactiveBorder, s->clrInactiveBorder);
  return s->clrInactiveCaptionText;
}
int VisualManager_GetCalendarColors(const Calendar* /*pCalendar*/,
    CalendarColors& colors)
{
  colors.clrCaption = s->clrBtnFace;
  colors.clrCaptionText = s->clrBtnText;
  colors.clrSelected = s->clrBtnFace;
  colors.clrSelectedText = s->clrBtnText;
  colors.clrTodayBorder = _RGB(187, 85, 3);
}
#if 0
int VisualManager_OnDrawControlBorder(CWnd* pWndCtrl)
{
  ASSERT_VALID(pWndCtrl);
  IRECT rect;
  pWndCtrl->GetWindowRect(rect);
  rect.b -= rect.t;
  rect.r -= rect.l;
  rect.l = rect.t = 0;
  OnDrawControlBorder(&dc, rect, pWndCtrl, s->m_bIsDrawOnGlass);
}
#endif
int VisualManager_OnDrawControlBorder(uidraw_t* s, img_t* im, IRECT rect, BaseControl* pWndCtrl, BOOL bDrawOnGlass)
{
  if (bDrawOnGlass) {
    imdraw_DrawRectR(im, 0, rect, 0, s->clrBarShadow);
    iRectDeflate(&rect, 1, 1);
    imdraw_DrawRectR(im, 0, rect, 0, s->clrWindow);
  }
  else {
    if ((pWndCtrl->GetStyle() & WS_POPUP)) {
      imdraw_Draw3dRect(im, 0, &rect, s->clrBarShadow, s->clrBarShadow);
    }
    else {
      imdraw_Draw3dRect(im, 0, &rect, s->clrBarDkShadow, s->clrBarHilite);
    }
    iRectDeflate(&rect, 1, 1);
    imdraw_Draw3dRect(im, 0, &rect, s->clrWindow, s->clrWindow);
  }
}
int VisualManager_OnDrawShowAllMenuItems(uidraw_t* s, img_t* im, IRECT rect,
    int /*state*/)
{
  MenuImages_Draw(im, IdArowShowAll, rect);
}
int VisualManager_GetShowAllMenuItemsHeight(uidraw_t* s, img_t* im, const CSize& /*sizeDefault*/)
{
  return MenuImages_Size().h + 2 * TEXT_MARGIN;
}
int VisualManager_GetTabFrameColors(const BaseTabWnd* pTabWnd,
    COLOR& clrDark,
    COLOR& clrBlack,
    COLOR& clrHighlight,
    COLOR& clrFace,
    COLOR& clrDarkShadow,
    COLOR& clrLight,
    COLOR& pbrFace,
    COLOR& pbrBlack)
{
  ASSERT_VALID(pTabWnd);
  COLOR clrActiveTab = pTabWnd->GetTabBkColor(pTabWnd->GetActiveTab());
  if (pTabWnd->IsOneNoteStyle() && clrActiveTab != 0) {
    clrFace = clrActiveTab;
  }
  else if (pTabWnd->IsDialogControl()) {
    clrFace = s->clrBtnFace;
  }
  else {
    clrFace = s->clrBarFace;
  }
  if (pTabWnd->IsDialogControl() && !pTabWnd->IsVisualManagerStyle()) {
    clrDark = s->clrBtnShadow;
    clrBlack = s->clrBtnText;
    clrHighlight = pTabWnd->IsVS2005Style() ? s->clrBtnShadow : s->clrBtnHilite;
    clrDarkShadow = s->clrBtnDkShadow;
    clrLight = s->clrBtnLight;
    pbrFace = s->clrBtnFace;
  }
  else {
    clrDark = s->clrBarShadow;
    clrBlack = s->clrBarText;
    clrHighlight = pTabWnd->IsVS2005Style() ? s->clrBarShadow : s->clrBarHilite;
    clrDarkShadow = s->clrBarDkShadow;
    clrLight = s->clrBarLight;
    pbrFace = s->clrBarFace;
  }
  pbrBlack = s->clrBlack;
}
int VisualManager_OnFillAutoHideButtonBackground(uidraw_t* s, img_t* im, IRECT rect, AutoHideButton* /*pButton*/)
{
  imdraw_FillRect(im, 0, rect, s->clrBarFace);
}
int VisualManager_OnDrawAutoHideButtonBorder(uidraw_t* s, img_t* im, IRECT rectBounds, IRECT rectBorderSize, AutoHideButton* /*pButton*/)
{
  COLOR clr = s->clrBarShadow;
  COLOR clrText = im->GetTextColor();
  if (rectBorderSize.l > 0) {
    im->FillSolidRect(rectBounds.l, rectBounds.t,
        rectBounds.l + rectBorderSize.l,
        rectBounds.b, clr);
  }
  if (rectBorderSize.t > 0) {
    im->FillSolidRect(rectBounds.l, rectBounds.t,
        rectBounds.r,
        rectBounds.t + rectBorderSize.t, clr);
  }
  if (rectBorderSize.r > 0) {
    im->FillSolidRect(rectBounds.r - rectBorderSize.r, rectBounds.t,
        rectBounds.r,
        rectBounds.b, clr);
  }
  if (rectBorderSize.b > 0) {
    im->FillSolidRect(rectBounds.l, rectBounds.b - rectBorderSize.b,
        rectBounds.r,
        rectBounds.b, clr);
  }
  im->SetTextColor(clrText);
}
COLOR VisualManager_GetAutoHideButtonTextColor(AutoHideButton* /*pButton*/)
{
  return s->clrBarText;
}
int VisualManager_OnDrawOutlookBarSplitter(uidraw_t* s, img_t* im, IRECT rectSplitter)
{
  imdraw_FillRect(im, 0, rectSplitter, s->clrBarFace);
  imdraw_Draw3dRect(im, 0, &rectSplitter, s->clrBarHilite, s->clrBarShadow);
}
int VisualManager_OnFillOutlookBarCaption(uidraw_t* s, img_t* im, IRECT rectCaption, COLOR clrText)
{
  im->FillSolidRect(rectCaption, s->clrBarShadow);
  clrText = s->clrBarHilite;
}
#endif
BOOL VisualManager_OnDrawCalculatorButton(uidraw_t* s, img_t* im, IRECT rect, int state, const char* strText)
{
  IRECT rcTxt = rect;
  switch (state) {
  case ButtonsIsPressed:
    imdraw_Draw3dRect(im, &rect, &rect, s->clrBarShadow, s->clrBarHilite, s->clrLight);
    iRectOffset2(&rcTxt, 1, 1);
    break;
  case ButtonsIsHighlighted:
    imdraw_Draw3dRect(im, &rect, &rect, s->clrBarHilite, s->clrBarShadow, s->clrLight);
    break;
  default:
    imdraw_Draw3dRect(im, &rect, &rect, s->clrBarHilite, s->clrBarShadow, s->clrBtnFace);
  }
  imdraw_text(im, &rect, rcTxt, strText, -1, s->fontDefault, s->clrWindowText, 0, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
  return TRUE;
}
BOOL VisualManager_OnDrawCalculatorDisplay(uidraw_t* s, img_t* im, IRECT rect, const char* strText)
{
  imdraw_Draw3dRect(im, 0, &rect, s->clrBarShadow, s->clrBarHilite, s->clrWindow);
  return TRUE;
}
#if 0
BOOL VisualManager_OnDrawBrowseButton(uidraw_t* s, img_t* im, IRECT rect,
    Edit* /*pEdit*/,
    int state,
    COLOR& /*clrText*/)
{
  imdraw_FillRect(im, 0, &rect, s->clrBtnFace);
  IRECT rectFrame = rect;
  rectFrame.InflateRect(0, 1, 1, 1);
  imdraw_Draw3dRect(im, 0, &rectFrame, s->clrBtnDkShadow, s->clrBtnDkShadow);
  rectFrame iRectDeflate(&, 1, 1);
  im->DrawEdge(rectFrame, state == ButtonsIsPressed ? BDR_SUNKENINNER : BDR_RAISEDINNER, BF_RECT);
  return TRUE;
}
int VisualManager_OnDrawAppBarBorder(uidraw_t* s, img_t* im, AppBarWnd* /*pAppBarWnd*/,
    IRECT rectBorder, IRECT rectBorderSize)
{
  ASSERT(im);
  imdraw_rect_border(im, 0, rectBorder, 0, s->clrBtnFace, rectBorderSize);
  rectBorderSize iRectDeflate(&, 2, 2);
  rectBorder iRectDeflate(&, 2, 2);
  imdraw_rect_border(im, 0, rectBorder, 0, s->clrLight, rectBorderSize);
}
int VisualManager_OnDrawAppBarCaption(uidraw_t* s, img_t* im, AppBarWnd* /*pAppBarWnd*/,
    IRECT rectCaption, CString strCaption)
{
  imdraw_FillRect(im, 0, rectCaption, s->clrBarFace);
  // Paint caption text:
  int nOldMode = im->SetBkMode(TRANSPARENT);
  COLOR clrOldText = im->SetTextColor(s->clrBarText);
  CFont* pOldFont = im->SelectObject(s->fontBold);
  ASSERT_VALID(pOldFont);
  IRECT rectText = rectCaption;
  rectText iRectDeflate(&, 2, 0);
  im->DrawText(strCaption, rectText, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
  im->SelectObject(pOldFont);
  im->SetBkMode(nOldMode);
  im->SetTextColor(clrOldText);
}
int VisualManager_GetSmartDockingBaseMarkerColors(
    COLOR& clrBaseGroupBackground,
    COLOR& clrBaseGroupBorder)
{
  clrBaseGroupBackground = s->clrBarFace;
  clrBaseGroupBorder = s->clrBarShadow;
}
COLOR VisualManager_GetSmartDockingMarkerToneColor()
{
  return s->clrActiveCaption;
}
#ifndef BCGP_EXCLUDE_TOOLBOX
BOOL VisualManager_OnEraseToolBoxButton(uidraw_t* s, img_t* im, IRECT rect,
    ToolBoxButton* pButton)
{
  if (pButton->GetCheck()) {
    iRectDeflate(&rect, 1, 1);
    FillDitheredRect(im, rect);
  }
  return TRUE;
}
BOOL VisualManager_OnDrawToolBoxButtonBorder(uidraw_t* s, img_t* im, IRECT& rect,
    ToolBoxButton* pButton, UINT /*uiState*/)
{
  if (pButton->GetCheck()) {
    imdraw_Draw3dRect(im, 0, &rect, s->clrBarShadow, s->clrBarHilite);
  }
  else if (pButton->IsHighlighted()) {
    imdraw_Draw3dRect(im, 0, &rect, s->clrBarHilite, s->clrBarShadow);
  }
  return TRUE;
}
COLOR VisualManager_GetToolBoxButtonTextColor(ToolBoxButton* /*pButton*/)
{
  return s->clrWindowText;
}
#endif // BCGP_EXCLUDE_TOOLBOX
#ifndef BCGP_EXCLUDE_POPUP_WINDOW
int VisualManager_OnFillPopupWindowBackground(uidraw_t* s, img_t* im, IRECT rect)
{
  imdraw_FillRect(im, 0, rect, s->clrBarFace);
}
int VisualManager_OnDrawPopupWindowBorder(uidraw_t* s, img_t* im, IRECT rect)
{
  imdraw_Draw3dRect(im, 0, &rect, s->clrBarLight, s->clrBarDkShadow);
  iRectDeflate(&rect, 1, 1);
  imdraw_Draw3dRect(im, 0, &rect, s->clrBarHilite, s->clrBarShadow);
}
COLOR VisualManager_OnDrawPopupWindowCaption(uidraw_t* s, img_t* im, IRECT rectCaption, PopupWindow* /*pPopupWnd*/)
{
  BOOL bActive = TRUE; // TODO
  COLOR clr = (bActive ? s->clrActiveCaption : s->clrInactiveCaption);
  imdraw_FillRect(im, 0, rectCaption, clr);
  // get the text color
  return bActive ? s->clrCaptionText : s->clrInactiveCaptionText;
}
int VisualManager_OnErasePopupWindowButton(uidraw_t* s, img_t* im, IRECT rect, PopupWndButton* pButton)
{
  if (pButton->IsCaptionButton()) {
    imdraw_FillRect(im, 0, rect, s->clrBtnFace);
    return 0;
  }
#if 0
  IRECT rectParent;
  pButton->GetParent()->GetClientRect(rectParent);
  pButton->GetParent()->MapWindowPoints(pButton, rectParent);
  OnFillPopupWindowBackground(im, rectParent);
#endif
}
int VisualManager_OnDrawPopupWindowButtonBorder(uidraw_t* s, img_t* im, IRECT rect, PopupWndButton* pButton)
{
  if (pButton->IsPressed()) {
    imdraw_Draw3dRect(im, 0, &rect, s->clrBarDkShadow, s->clrBarLight);
    iRectDeflate(&rect, 1, 1);
    imdraw_Draw3dRect(im, 0, &rect, s->clrBarShadow, s->clrBarHilite);
  }
  else {
    imdraw_Draw3dRect(im, 0, &rect, s->clrBarLight, s->clrBarDkShadow);
    iRectDeflate(&rect, 1, 1);
    imdraw_Draw3dRect(im, 0, &rect, s->clrBarHilite, s->clrBarShadow);
  }
}
#endif // BCGP_EXCLUDE_POPUP_WINDOW
#ifndef BCGP_EXCLUDE_PLANNER
COLOR VisualManager_OnFillPlannerCaption(uidraw_t* s, img_t* im,
    PlannerView* pView, IRECT rect, BOOL bIsToday, BOOL bIsSelected,
    BOOL bMonth, BOOL bNoBorder/* = FALSE*/, BOOL /*bHorz = TRUE*/)
{
  COLOR clrText = s->clrBtnText;
  //const BOOL bMonth = DYNAMIC_DOWNCAST(PlannerViewMonth, pView) != NULL;
  if (bMonth && !bIsToday && !bIsSelected) {
    return clrText;
  }
  iRectDeflate(&rect, 1, 1);
  if (bIsToday) {
    imdraw_FillRect(im, 0, rect, s->clrBtnFace);
    FillDitheredRect(im, rect);
  }
  else if (bIsSelected) {
    imdraw_FillRect(im, 0, rect, s->clrHilite);
    clrText = s->clrTextHilite;
  }
  else {
    imdraw_FillRect(im, 0, rect, s->clrBtnFace);
  }
  if (bIsToday || !bNoBorder) {
    COLOR clr = (bIsToday ? s->clrBarShadow : s->clrBtnShadow);
    COLOR pOldPen = im->SelectObject(clr);
    im->MoveTo(rect.l, rect.b);
    im->LineTo(rect.r, rect.b);
    im->SelectObject(pOldPen);
  }
  return clrText;
}
int VisualManager_OnDrawPlannerCaptionText(uidraw_t* s, img_t* im,
    PlannerView* /*pView*/, IRECT rect, const CString& strText,
    COLOR clrText, int nAlign, BOOL bHighlight)
{
  const int nTextMargin = 2;
  if (bHighlight) {
    const int nTextLen = min(rect.Width(),
        im->GetTextExtent(strText).w + 2 * nTextMargin);
    IRECT rectHighlight = rect;
    rectHighlight iRectDeflate(&, 1, 1);
    switch (nAlign) {
    case DT_LEFT:
      rectHighlight.r = rectHighlight.l + nTextLen;
      break;
    case DT_RIGHT:
      rectHighlight.l = rectHighlight.r - nTextLen;
      break;
    case DT_CENTER:
      rectHighlight.l = rectHighlight.CenterPoint().x - nTextLen / 2;
      rectHighlight.r = rectHighlight.l + nTextLen;
      break;
    }
    imdraw_FillRect(im, 0, rectHighlight, s->clrHilite);
    clrText = s->clrTextHilite;
  }
  iRectDeflate(&rect, nTextMargin, 0);
  COLOR clrOld = im->SetTextColor(clrText);
  im->DrawText(strText, rect, DT_SINGLELINE | DT_VCENTER | nAlign);
  im->SetTextColor(clrOld);
}
int VisualManager_GetPlannerAppointmentColors(PlannerView* pView,
    BOOL bSelected, BOOL bSimple, DWORD dwDrawFlags,
    COLOR& clrBack1, COLOR& clrBack2,
    COLOR& clrFrame1, COLOR& clrFrame2, COLOR& clrText)
{
  int clr = pView->GetClass();
  ASSERT_VALID(pView);
  const BOOL bIsGradientFill =
      dwDrawFlags & BCGP_PLANNER_DRAW_APP_GRADIENT_FILL &&
      s->s->m_nBitsPerPixel > 8 &&
      !(s->s->m_bIsBlackHighContrast || s->s->m_bIsWhiteHighContrast);
  const BOOL bIsOverrideSelection = dwDrawFlags & BCGP_PLANNER_DRAW_APP_OVERRIDE_SELECTION;
  const BOOL bSelect = bSelected && !bIsOverrideSelection;
  const BOOL bDayView = clr == CLASS_PlannerViewDay;
  if (bSelect) {
    if (bDayView) {
      if (!bSimple) {
        clrBack1 = s->clrBtnFace;
        clrText = s->clrBtnText;
      }
    }
    else {
      clrBack1 = s->clrHilite;
      clrText = s->clrTextHilite;
    }
  }
  BOOL bDefault = FALSE;
  if (clrBack1 == CLR_DEFAULT) {
    if (bIsGradientFill) {
      clrBack1 = pView->GetPlanner()->GetBackgroundColor();
      if (clrBack1 == CLR_DEFAULT) {
        bDefault = TRUE;
        clrBack1 = s->m_clrPlannerWork;
      }
    }
    else {
      clrBack1 = (bDayView || !bSimple)
          ? s->clrWindow
          : CLR_DEFAULT;
    }
  }
  clrBack2 = clrBack1;
  if (clrText == CLR_DEFAULT) {
    clrText = s->clrWindowText;
  }
  clrFrame1 = s->clrWindowFrame;
  clrFrame2 = clrFrame1;
  if (bIsGradientFill && clrBack1 != CLR_DEFAULT) {
    if (!bSelect) {
      if (bDefault) {
        clrBack2 = _RGB(255, 255, 255);
      }
      else {
        clrBack2 = PixelAlpha(clrBack1, _RGB(255, 255, 255), 25);
      }
    }
    clrFrame1 = PixelAlpha(clrBack1, _RGB(0, 0, 0), 75);
    if (!bSelected) {
      clrFrame2 = clrFrame1;
    }
  }
}
COLOR VisualManager_GetPlannerAppointmentTimeColor(PlannerView* /*pView*/,
    BOOL /*bSelected*/, BOOL /*bSimple*/, DWORD /*dwDrawFlags*/)
{
  return CLR_DEFAULT;
}
COLOR VisualManager_GetPlannerHourLineColor(PlannerView* /*pView*/,
    BOOL /*bWorkingHours*/, BOOL /*bHour*/)
{
  return s->m_clrPalennerLine;
}
COLOR VisualManager_GetPlannerSelectionColor(PlannerView* /*pView*/)
{
  return s->clrHilite;
}
COLOR VisualManager_GetPlannerSeparatorColor(PlannerView* /*pView*/)
{
  return s->clrWindowFrame;
}
int VisualManager_OnFillPlanner(uidraw_t* s, img_t* im, PlannerView* pView,
    IRECT rect, BOOL bWorkingArea)
{
  ASSERT_VALID(pView);
  COLOR colorFill = IsHighContastMode(s) ?
      CLR_DEFAULT : pView->GetPlanner()->GetBackgroundColor();
  BOOL bIsDefaultColor = colorFill == CLR_DEFAULT;
  if (bIsDefaultColor) {
    colorFill = s->m_clrPlannerWork; // Use default color
  }
  switch (pView->GetPlanner()->GetType()) {
  case BCGP_PLANNER_TYPE_DAY:
  case BCGP_PLANNER_TYPE_WORK_WEEK: {
    COLOR clrTextOld = im->SetTextColor(colorFill);
    COLOR clrBkOld = im->SetBkColor(bWorkingArea ?
        _RGB(255, 255, 255) : _RGB(128, 128, 128));
    //imdraw_FillRect(im, 0, rect, s->m_clrPlanner);
    {
      WORD HatchBits [8] = { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 };
      CBitmap bmp;
      bmp.CreateBitmap(8, 8, 1, 1, HatchBits);
      imdraw_image_pattern(im, 0, rect, &bmp, 0);
    }
    im->SetTextColor(clrTextOld);
    im->SetBkColor(clrBkOld);
  }
  break;
  default:
    if (bIsDefaultColor || !bWorkingArea) {
      imdraw_FillRect(im, 0, rect, bWorkingArea ?
          s->clrWindow : s->clrBtnFace);
    }
    else {
      COLOR clr = (colorFill);
      imdraw_FillRect(im, 0, rect, clr);
    }
  }
}
COLOR VisualManager_OnFillPlannerTimeBar(uidraw_t* s, img_t* im, PlannerView* /*pView*/,
    IRECT rect, COLOR& clrLine)
{
  imdraw_FillRect(im, 0, rect, s->clrBtnFace);
  clrLine = s->clrBtnShadow;
  return s->clrBtnText;
}
int VisualManager_OnDrawPlannerTimeLine(uidraw_t* s, img_t* im, PlannerView* /*pView*/, IRECT rect)
{
  imdraw_FillRect(im, 0, rect, s->clrActiveCaption);
  COLOR pOldPen = im->SelectObject(s->clrBarShadow);
  im->MoveTo(rect.l, rect.b);
  im->LineTo(rect.r, rect.b);
  im->SelectObject(pOldPen);
}
int VisualManager_OnFillPlannerWeekBar(uidraw_t* s, img_t* im, PlannerView* /*pView*/, IRECT rect)
{
  imdraw_FillRect(im, 0, rect, s->clrBtnFace);
}
int VisualManager_OnDrawPlannerHeader(uidraw_t* s, img_t* im,
    PlannerView* /*pView*/, IRECT rect)
{
  imdraw_FillRect(im, 0, rect, s->clrBtnFace);
  imdraw_Draw3dRect(im, 0, &rect, s->clrBtnHilite, s->clrBtnShadow);
}
int VisualManager_OnDrawPlannerHeaderPane(uidraw_t* s, img_t* im,
    PlannerView* /*pView*/, IRECT rect)
{
  imdraw_Draw3dRect(im, 0, &rect, s->clrBtnShadow, s->clrBtnHilite);
}
int VisualManager_OnFillPlannerHeaderAllDay(uidraw_t* s, img_t* im,
    PlannerView* /*pView*/, IRECT rect)
{
  COLOR clr = (s->clrBtnShadow);
  imdraw_FillRect(im, 0, rect, clr);
}
int VisualManager_OnDrawPlannerHeaderAllDayItem(uidraw_t* s, img_t* im,
    PlannerView* /*pView*/, IRECT rect, BOOL /*bIsToday*/, BOOL bIsSelected)
{
  if (bIsSelected) {
    imdraw_FillRect(im, 0, rect, s->clrWindow);
  }
}
int VisualManager_PreparePlannerBackItem(BOOL /*bIsToday*/, BOOL /*bIsSelected*/)
{
  s->m_bPlannerBackItemToday = FALSE;
  s->m_bPlannerBackItemSelected = FALSE;
}
int VisualManager_PreparePlannerCaptionBackItem(BOOL bIsHeader)
{
  s->m_bPlannerCaptionBackItemHeader = bIsHeader;
}
#endif // BCGP_EXCLUDE_PLANNER
int VisualManager_DoDrawHeaderSortArrow(uidraw_t* s, img_t* im, IRECT rectArrow, BOOL bIsUp, BOOL /*bDlgCtrl*/)
{
  MenuImages_Draw(im,
      bIsUp ? IdArowUpLarge : IdArowDownLarge,
      rectArrow);
}
#ifndef BCGP_EXCLUDE_GRID_CTRL
int VisualManager_OnDrawGridSortArrow(GridCtrl* /*pCtrl*/, img_t* im,
    IRECT rectArrow, BOOL bIsUp)
{
  DoDrawHeaderSortArrow(im, rectArrow, bIsUp, TRUE/*!pCtrl->IsControlBarColors()*/);
}
COLOR VisualManager_OnFillGridGroupByBoxBackground(uidraw_t* s, img_t* im, IRECT rect)
{
  COLOR clr = (s->clrBtnShadow);
  imdraw_FillRect(im, 0, rect, clr);
  return s->clrBarText;
}
COLOR VisualManager_OnFillGridGroupByBoxTitleBackground(uidraw_t* s, img_t* im, IRECT rect)
{
  imdraw_FillRect(im, 0, rect, s->clrBtnFace);
  return s->clrBtnShadow;
}
COLOR VisualManager_GetGridGroupByBoxLineColor() const
{
  return s->clrBarText;
}
int VisualManager_OnDrawGridGroupByBoxItemBorder(GridCtrl* /*pCtrl*/,
    img_t* im, IRECT rect)
{
  imdraw_FillRect(im, 0, rect, s->clrBtnFace);
  imdraw_Draw3dRect(im, 0, &rect, s->clrBarWindow, s->clrBtnText);
  iRectDeflate(&rect, 0, 0, 1, 1);
  imdraw_Draw3dRect(im, 0, &rect, s->clrBarWindow, s->clrBtnShadow);
}
COLOR VisualManager_GetGridLeftOffsetColor(GridCtrl* pCtrl)
{
  ASSERT_VALID(pCtrl);
  COLOR clrGray = 0;
  if (s->s->m_nBitsPerPixel <= 8) {
    clrGray = pCtrl->IsControlBarColors() ?
        s->clrBarShadow : s->clrBtnShadow;
  }
  else {
    clrGray = PixelAlpha(
        pCtrl->IsControlBarColors() ? s->clrBarFace : s->clrBtnFace, 94);
  }
  return clrGray;
}
COLOR VisualManager_GetGridItemSortedColor(GridCtrl* pCtrl)
{
  ASSERT_VALID(pCtrl);
  COLOR clrSortedColumn = 0;
  if (s->s->m_nBitsPerPixel <= 8) {
    clrSortedColumn = pCtrl->GetBkColor();
  }
  else {
    clrSortedColumn = PixelAlpha(
        pCtrl->GetBkColor(), .97, .97, .97);
  }
  return clrSortedColumn;
}
int VisualManager_OnFillGridGroupBackground(GridCtrl* /*pCtrl*/, img_t* im,
    IRECT rectFill)
{
  imdraw_FillRect(im, 0, rectFill, s->clrWindow);
}
int VisualManager_OnDrawGridGroupUnderline(GridCtrl* pCtrl, img_t* im,
    IRECT rectFill)
{
  ASSERT_VALID(pCtrl);
  COLOR clrOld = im->GetBkColor();
  im->FillSolidRect(rectFill,
      pCtrl->IsControlBarColors() ? s->clrBarShadow : s->clrBtnShadow);
  im->SetBkColor(clrOld);
}
COLOR VisualManager_OnFillGridRowBackground(GridCtrl* pCtrl,
    img_t* im, IRECT rectFill, BOOL bSelected)
{
  ASSERT_VALID(pCtrl);
  // Fill area:
  if (!pCtrl->IsFocused()) {
    imdraw_FillRect(im, 0, rectFill,
        pCtrl->IsControlBarColors() ? s->clrBarFace : s->clrBtnFace);
  }
  else {
    imdraw_FillRect(im, 0, rectFill, s->clrHilite);
  }
  // Return text color:
  if (!pCtrl->IsHighlightGroups() && bSelected) {
    return(!pCtrl->IsFocused()) ? s->clrHotText : s->clrTextHilite;
  }
  return pCtrl->IsHighlightGroups() ?
      (pCtrl->IsControlBarColors() ? s->clrBarShadow : s->clrBtnShadow) :
        s->clrWindowText;
}
COLOR VisualManager_OnFillGridItem(GridCtrl* pCtrl,
    img_t* im, IRECT rectFill,
    BOOL bSelected, BOOL bActiveItem, BOOL bSortedColumn)
{
  ASSERT_VALID(pCtrl);
  // Fill area:
  if (bSelected && !bActiveItem) {
    if (!pCtrl->IsFocused()) {
      imdraw_FillRect(im, 0, rectFill,
          pCtrl->IsControlBarColors() ? s->clrBarFace : s->clrBtnFace);
      return s->clrBtnText;
    }
    else {
      imdraw_FillRect(im, 0, rectFill, s->clrHilite);
      return s->clrTextHilite;
    }
  }
  else {
    if (bActiveItem) {
      imdraw_FillRect(im, 0, rectFill, s->clrWindow);
    }
    else if (bSortedColumn) {
      COLOR clr = (pCtrl->GetSortedColor());
      imdraw_FillRect(im, 0, rectFill, clr);
    }
    else {
      // no painting
    }
  }
  return 0;
}
int VisualManager_OnDrawGridHeaderMenuButton(GridCtrl* /*pCtrl*/, img_t* im, IRECT rect,
    BOOL bHighlighted, BOOL bPressed, BOOL bDisabled)
{
  iRectDeflate(&rect, 1, 1);
  if (bHighlighted) {
    imdraw_Draw3dRect(im, 0, &rect, s->clrBtnHilite, s->clrBtnDkShadow);
  }
}
int VisualManager_OnDrawGridSelectionBorder(GridCtrl* /*pCtrl*/, img_t* im, IRECT rect)
{
  imdraw_Draw3dRect(im, 0, &rect, s->clrBtnDkShadow, s->clrBtnDkShadow);
  iRectDeflate(&rect, 1, 1);
  imdraw_Draw3dRect(im, 0, &rect, s->clrBtnDkShadow, s->clrBtnDkShadow);
}
BOOL VisualManager_OnSetGridColorTheme(GridCtrl* /*pCtrl*/, BCGP_GRID_COLOR_DATA& /*theme*/)
{
  return TRUE;
}
COLOR VisualManager_OnFillReportCtrlRowBackground(GridCtrl* pCtrl,
    img_t* im, IRECT rectFill,
    BOOL bSelected, BOOL bGroup)
{
  ASSERT_VALID(pCtrl);
  // Fill area:
  COLOR clrText = 0;
  if (bSelected) {
    if (!pCtrl->IsFocused()) {
      imdraw_FillRect(im, 0, rectFill,
          pCtrl->IsControlBarColors() ? s->clrBarFace : s->clrBtnFace);
      clrText = s->m_clrReportGroupText;
    }
    else {
      imdraw_FillRect(im, 0, rectFill, s->clrHilite);
      clrText = s->clrTextHilite;
    }
  }
  else {
    if (bGroup) {
      // no painting
      clrText = s->m_clrReportGroupText;
    }
  }
  // Return text color:
  return clrText;
}
COLOR VisualManager_GetReportCtrlGroupBackgoundColor()
{
  return s->clrBtnLight;
}
int VisualManager_OnFillGridBackground(uidraw_t* s, img_t* im, IRECT rect)
{
  imdraw_FillRect(im, 0, rect, s->clrWindow);
}
int VisualManager_OnDrawGridExpandingBox(uidraw_t* s, img_t* im, IRECT rect, BOOL bIsOpened, COLOR colorBox)
{
  OnDrawExpandingBox(im, rect, bIsOpened, colorBox);
}
int VisualManager_OnFillGridHeaderBackground(GridCtrl* /*pCtrl*/, img_t* im, IRECT rect)
{
  imdraw_FillRect(im, 0, rect, s->clrBtnFace);
}
BOOL VisualManager_OnDrawGridHeaderItemBorder(GridCtrl* /*pCtrl*/, img_t* im, IRECT rect, BOOL /*bPressed*/)
{
  imdraw_Draw3dRect(im, 0, &rect, s->clrBtnHilite, s->clrBtnDkShadow);
  return TRUE;
}
int VisualManager_OnFillGridRowHeaderBackground(GridCtrl* /*pCtrl*/, img_t* im, IRECT rect)
{
  imdraw_FillRect(im, 0, rect, s->clrBtnFace);
}
BOOL VisualManager_OnDrawGridRowHeaderItemBorder(GridCtrl* /*pCtrl*/, img_t* im, IRECT rect, BOOL /*bPressed*/)
{
  imdraw_Draw3dRect(im, 0, &rect, s->clrBtnHilite, s->clrBtnDkShadow);
  return TRUE;
}
int VisualManager_OnFillGridSelectAllAreaBackground(GridCtrl* /*pCtrl*/, img_t* im, IRECT rect, BOOL /*bPressed*/)
{
  imdraw_FillRect(im, 0, rect, s->clrBtnFace);
}
BOOL VisualManager_OnDrawGridSelectAllAreaBorder(GridCtrl* /*pCtrl*/, img_t* im, IRECT rect, BOOL /*bPressed*/)
{
  imdraw_Draw3dRect(im, 0, &rect, s->clrBtnHilite, s->clrBtnDkShadow);
  return TRUE;
}
#endif // BCGP_EXCLUDE_GRID_CTRL
#if !defined(BCGP_EXCLUDE_GRID_CTRL) && !defined(BCGP_EXCLUDE_GANTT)
int GanttChart_PrepareColorScheme(COLOR clrMain, BCGP_GANTT_CHART_COLORS& colors) // static
{
  double H, S, L;
  RGBtoHSL(clrMain, &H, &S, &L);
  double L1 = max(L, 0.85);
  clrMain = HLStoRGB_ONE(H, L1, S);
  colors.clrRowBackground = clrMain;
  colors.clrRowDayOff = HLStoRGB_ONE(H, L1 - 0.05, S);
  colors.clrBarFill = HLStoRGB_ONE(H, L1 - 0.50, S);
  colors.clrBarComplete = HLStoRGB_ONE(H, L1 - 0.20, S);
  colors.clrConnectorLines = MixColors(clrMain, _RGB(0, 0, 0), 0.85f);
  colors.clrGridLine0 = MixColors(clrMain, _RGB(192, 192, 192), 0.75f);
  colors.clrGridLine1 = MixColors(clrMain, _RGB(64, 64, 64), 0.75f);
  colors.clrShadows = MixColors(clrMain, _RGB(32, 32, 32), 0.8f);
}
int VisualManager_GetGanttColors(const GanttChart* /*pChart*/, BCGP_GANTT_CHART_COLORS& colors, COLOR clrBack) const
{
  if (clrBack == CLR_DEFAULT) {
    clrBack = s->clrWindow;
  }
  GanttChart_PrepareColorScheme(clrBack, colors);
  colors.clrBackground = s->clrWindow;
  colors.clrShadows = s->m_clrMenuShadowBase;
  colors.clrRowBackground = colors.clrBackground;
  colors.clrGridLine0 = s->clrBarShadow;
  colors.clrGridLine1 = s->clrBarLight;
  colors.clrSelection = s->clrHilite;
  colors.clrSelectionBorder = s->clrHilite;
  colors.clrBarFill = _RGB(0, 0, 255);
  colors.clrBarComplete = _RGB(0, 255, 0);
}
int VisualManager_DrawGanttChartBackground(const GanttChart*, img_t& dc, const IRECT& rectChart, COLOR clrFill)
{
  dc.FillSolidRect(rectChart, clrFill);
}
int VisualManager_DrawGanttItemBackgroundCell(const GanttChart*, img_t& dc, const IRECT& /*rectItem*/, const IRECT& rectClip, const BCGP_GANTT_CHART_COLORS& colors, BOOL bDayOff)
{
  dc.FillSolidRect(rectClip, (bDayOff) ? colors.clrRowDayOff : colors.clrRowBackground);
}
int VisualManager_DrawGanttHeaderCell(const GanttChart*, img_t& dc, const BCGP_GANTT_CHART_HEADER_CELL_INFO& cellInfo, BOOL /*bHilite*/)
{
  dc.FillSolidRect(cellInfo.rectCell, s->clrBarFace);
  dc.Draw3dRect(cellInfo.rectCell, s->clrBtnHilite, s->clrBtnDkShadow);
}
int VisualManager_DrawGanttHeaderText(const GanttChart*, img_t& dc, const BCGP_GANTT_CHART_HEADER_CELL_INFO& cellInfo, const CString& sCellText, BOOL bHilite) const
{
  IRECT rcCell = cellInfo.rectCell;
  rcCell iRectDeflate(&, 1, 2, 2, 2); // text padding
  IRECT rcVisible;
  rcVisible.IntersectRect(&cellInfo.rectClip, rcCell);
  if (rcVisible.IsRectEmpty()) {
    return 0;
  }
  CFont* pOldFont = dc.SelectObject(s->fontRegular);
  dc.SetBkMode(TRANSPARENT);
  dc.SetTextColor(GetGanttHeaderTextColor(bHilite));
  ASSERT(cellInfo.pHeaderInfo != NULL);
  DWORD dwFlags = DT_VCENTER | DT_NOPREFIX | cellInfo.pHeaderInfo->dwAlignment;
  dc.DrawText(sCellText, rcCell, dwFlags);
  dc.SelectObject(pOldFont);
}
COLOR VisualManager_GetGanttHeaderTextColor(BOOL bHilite) const
{
  return bHilite ? s->clrHotText : s->clrWindowText;
}
int VisualManager_FillGanttBar(const GanttItem* /*pItem*/, img_t& dc, const IRECT& rectFill, COLOR color, double /*dGlowLine*/)
{
  dc.FillSolidRect(rectFill, color);
}
#endif // !defined(BCGP_EXCLUDE_GRID_CTRL) && !defined(BCGP_EXCLUDE_GANTT)
#ifndef BCGP_EXCLUDE_PROP_LIST
COLOR VisualManager_GetPropListGroupColor(PropList* pPropList)
{
  ASSERT_VALID(pPropList);
  return pPropList->DrawControlBarColors() ?
      s->clrBarFace : s->clrBtnFace;
}
COLOR VisualManager_GetPropListGroupTextColor(PropList* pPropList)
{
  ASSERT_VALID(pPropList);
  return pPropList->DrawControlBarColors() ?
      s->clrBarDkShadow : s->clrBtnDkShadow;
}
COLOR VisualManager_GetPropListCommandTextColor(PropList* /*pPropList*/)
{
  return s->clrHotLinkText;
}
#endif // BCGP_EXCLUDE_PROP_LIST
COLOR VisualManager_GetMenuItemTextColor(
    int* /*pButton*/, BOOL bHighlighted, BOOL bDisabled)
{
  if (bHighlighted) {
    return bDisabled ? s->clrBtnFace : s->clrTextHilite;
  }
  return bDisabled ? s->clrGrayedText : s->clrWindowText;
}
int VisualManager_GetMenuDownArrowState(int* /*pButton*/, BOOL /*bHightlight*/, BOOL bPressed, BOOL bDisabled)
{
  return(int) ImageBlack;
}
COLOR VisualManager_GetStatusBarPaneTextColor(StatusBar* /*pStatusBar*/,
    CBCGStatusBarPaneInfo* pPane)
{
  ASSERT(pPane != NULL);
  return(pPane->GetStyle() & SBPS_DISABLED) ? s->clrGrayedText :
      pPane->GetTextColor() == 0 ? s->clrBtnText : pPane->GetTextColor();
}
#ifndef BCGP_EXCLUDE_RIBBON
int VisualManager_OnDrawRibbonCaption(uidraw_t* s, img_t* im, RibbonBar* pBar,
    IRECT rect, IRECT rectText)
{
  ASSERT_VALID(pBar);
  const BOOL bGlass = pBar->IsTransparentCaption();
  const DWORD dwStyleEx = pBar->GetExStyle();
  const BOOL bIsRTL = (dwStyleEx & WS_EX_LAYOUTRTL) == WS_EX_LAYOUTRTL;
  BOOL bTextCenter = TRUE;
  IRECT rectQAT = pBar->GetQuickAccessToolbarLocation();
  BOOL bHide = (pBar->GetHideFlags() & BCGPRIBBONBAR_HIDE_ALL) != 0;
  BOOL bExtra = !bHide && pBar->IsQuickAccessToolbarOnTop() &&
      rectQAT.l < rectQAT.r && !pBar->IsQATEmpty();
  if ((bHide && !bExtra) || pBar->IsScenicLook()) {
    CBitmap* hIcon = pBar->GetWndIcon();
    if (hIcon != NULL) {
      CSize szIcon(::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
      long x = rect.l + 2;
      long y = rect.t + max(0, (rect.Height() - szIcon.h) / 2);
      if (bGlass) {
        s->DrawIconOnGlass(0, im, hIcon, IRECT(x, y, x + szIcon.w, y + szIcon.h));
      }
      else {
        ::DrawIconEx(im, x, y, hIcon, szIcon.w, szIcon.h,
            0, NULL, DI_NORMAL);
      }
      if (rectText.l < (x + szIcon.w + 4)) {
        rectText.l = x + szIcon.w + 4;
      }
    }
    bTextCenter = TRUE;
  }
  CFont* pOldFont = im->SelectObject(s->fontBold);
  ASSERT(pOldFont != NULL);
  int nOldMode = im->SetBkMode(TRANSPARENT);
  CString strCaption;
  pBar->GetWindowText(strCaption);
  DWORD dwTextStyle = DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER |
      (bIsRTL ? DT_RTLREADING | DT_RIGHT : 0);
  COLOR clrText = _RGB(0, 0, 0);
  int widthFull = rectText.Width();
  int width = im->GetTextExtent(strCaption).w;
  if (bTextCenter && width < widthFull) {
    rectText.l += (widthFull - width) / 2;
  }
  rectText.r = min(rectText.l + width, rectText.r);
  if (rectText.r > rectText.l) {
    if (bGlass) {
      DrawTextOnGlass(im, strCaption, rectText, dwTextStyle, 10);
    }
    else {
      COLOR clrOldText = im->SetTextColor(clrText);
      im->DrawText(strCaption, rectText, dwTextStyle);
      im->SetTextColor(clrOldText);
    }
  }
  im->SetBkMode(nOldMode);
  im->SelectObject(pOldFont);
}
int VisualManager_OnDrawRibbonCaptionButton(
    img_t* im, RibbonCaptionButton* pButton)
{
  OnFillRibbonButton(im, pButton);
  IMAGES_IDS imageID;
  switch (pButton->GetID()) {
  case SC_CLOSE:
    imageID = IdClose;
    break;
  case SC_MINIMIZE:
    imageID = IdMinimize;
    break;
  case SC_MAXIMIZE:
    imageID = IdMaximize;
    break;
  case SC_RESTORE:
    imageID = IdRestore;
    break;
  default:
    return 0;
  }
  MenuImages_Draw(im, imageID, pButton->GetRect(),
      pButton->IsDisabled() ?
      ImageGray : ImageBlack);
  OnDrawRibbonButtonBorder(im, pButton);
}
COLOR VisualManager_OnDrawRibbonButtonsGroup(
    img_t* im, RibbonButtonsGroup* /*pGroup*/,
    IRECT /*rectGroup*/)
{
  return 0;
}
int VisualManager_OnDrawDefaultRibbonImage(
    img_t* im, IRECT rectImage,
    BOOL bIsDisabled,
    BOOL /*bIsPressed*/,
    BOOL /*bIsHighlighted*/)
{
  IRECT rectBullet(rectImage.CenterPoint(), CSize(1, 1));
  rectBullet.InflateRect(5, 5);
  if (s->s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    COLOR clr = (bIsDisabled ? s->clrGrayedText : _RGB(0, 127, 0));
    im->Ellipse(rectBullet, clr);
  }
  else {
    imdraw_EllipseR(im, 0, rectBullet,
        bIsDisabled ? s->clrGrayedText : _RGB(160, 208, 128),
        bIsDisabled ? s->clrBtnShadow : _RGB(71, 117, 44), 1);
  }
}
int VisualManager_OnDrawRibbonMainButton(
    img_t* im,
    RibbonButton* pButton)
{
  const BOOL bIsHighlighted = pButton->IsHighlighted() || pButton->IsFocused();
  const BOOL bIsPressed = pButton->IsPressed() || pButton->IsDroppedDown();
  IRECT rect = pButton->GetRect();
  iRectDeflate(&rect, 2, 2);
  imdraw_EllipseR(im, 0, rect,
      bIsPressed ? s->clrBarLight : s->clrBarFace,
      bIsHighlighted ? s->clrBarDkShadow : s->clrBarShadow, 1);
}
COLOR VisualManager_OnDrawRibbonTabsFrame(
    img_t* im,
    RibbonBar* /*pWndRibbonBar*/,
    IRECT rectTab)
{
  COLOR pen = (s->clrBarShadow);
  COLOR pOldPen = im->SelectObject(pen);
  ASSERT(pOldPen != NULL);
  im->MoveTo(rectTab.l, rectTab.t);
  im->LineTo(rectTab.r, rectTab.t);
  im->SelectObject(pOldPen);
  return 0;
}
int VisualManager_OnDrawRibbonCategory(
    img_t* im,
    RibbonCategory* pCategory,
    IRECT rectCategory)
{
  ASSERT_VALID(pCategory);
  const int nShadowSize = 2;
  rectCategory.r -= nShadowSize;
  rectCategory.b -= nShadowSize;
  imdraw_FillRect(im, 0, rectCategory, s->clrBarFace);
  IRECT rectActiveTab = pCategory->GetTabRect();
  COLOR pen = (s->clrBarShadow);
  COLOR pOldPen = im->SelectObject(pen);
  ASSERT(pOldPen != NULL);
  im->MoveTo(rectCategory.l, rectCategory.t);
  im->LineTo(rectActiveTab.l + 1, rectCategory.t);
  im->MoveTo(rectActiveTab.r - 2, rectCategory.t);
  im->LineTo(rectCategory.r, rectCategory.t);
  im->LineTo(rectCategory.r, rectCategory.b);
  im->LineTo(rectCategory.l, rectCategory.b);
  im->LineTo(rectCategory.l, rectCategory.t);
  im->SelectObject(pOldPen);
  DrawShadow(im, 0, rectCategory, nShadowSize, 100, 75, NULL, NULL,
      s->m_clrMenuShadowBase);
}
int VisualManager_OnDrawRibbonCategoryScroll(
    img_t* im,
    RibbonCategoryScroll* pScroll)
{
  ASSERT_VALID(pScroll);
  IRECT rect = pScroll->GetRect();
  rect.b--;
  imdraw_FillRect(im, 0, rect, s->clrBarFace);
  if (pScroll->IsHighlighted()) {
    HighlightRect(im, 0, rect);
  }
  BOOL bIsLeft = pScroll->IsLeftScroll();
  if (s->s->m_bIsRTL) {
    bIsLeft = !bIsLeft;
  }
  MenuImages_Draw(im,
      bIsLeft ? IdArowLeftLarge : IdArowRightLarge,
      rect);
  imdraw_Draw3dRect(im, 0, &rect, s->clrBarShadow, s->clrBarShadow);
}
COLOR VisualManager_OnDrawRibbonCategoryTab(
    img_t* im,
    RibbonTab* pTab,
    BOOL bIsActive)
{
  ASSERT_VALID(pTab);
  RibbonCategory* pCategory = pTab->GetParentCategory();
  ASSERT_VALID(pCategory);
  RibbonBar* pBar = pCategory->GetParentRibbonBar();
  ASSERT_VALID(pBar);
  bIsActive = bIsActive &&
      ((pBar->GetHideFlags() & BCGPRIBBONBAR_HIDE_ELEMENTS) == 0 || pTab->GetDroppedDown() != NULL);
  const BOOL bIsFocused = pTab->IsFocused() && (pBar->GetHideFlags() & BCGPRIBBONBAR_HIDE_ELEMENTS);
  const BOOL bIsHighlighted = (pTab->IsHighlighted() || bIsFocused) && !pTab->IsDroppedDown();
  COLOR pen = (s->clrBarShadow);
  COLOR pOldPen = im->SelectObject(pen);
  ASSERT(pOldPen != NULL);
  IRECT rectTab = pTab->GetRect();
  rectTab.t += 3;
  const int nTrancateRatio =
      pTab->GetParentCategory()->GetParentRibbonBar()->GetTabTrancateRatio();
  if (nTrancateRatio > 0) {
    const int nPercent = max(10, 100 - nTrancateRatio / 2);
    COLOR color = PixelAlpha(s->clrBarFace, nPercent);
    COLOR pOldPen = im->SelectObject(color);
    im->MoveTo(rectTab.r - 1, rectTab.t);
    im->LineTo(rectTab.r - 1, rectTab.b);
    im->SelectObject(pOldPen);
  }
  if (!bIsActive && !bIsHighlighted) {
    return s->clrBarText;
  }
  rectTab.r -= 2;
#define POINTS_NUM 8
  IPOINT pts [POINTS_NUM];
  pts [0] = CPoint(rectTab.l, rectTab.b);
  pts [1] = CPoint(rectTab.l + 1, rectTab.b - 1);
  pts [2] = CPoint(rectTab.l + 1, rectTab.t + 2);
  pts [3] = CPoint(rectTab.l + 3, rectTab.t);
  pts [4] = CPoint(rectTab.r - 3, rectTab.t);
  pts [5] = CPoint(rectTab.r - 1, rectTab.t + 2);
  pts [6] = CPoint(rectTab.r - 1, rectTab.b - 1);
  pts [7] = CPoint(rectTab.r, rectTab.b);
  if (bIsActive) {
    COLOR clrFill = pTab->IsSelected() ? s->clrBarHilite :
        RibbonCategoryColorToRGB(pTab->GetParentCategory()->GetTabColor());
    if (clrFill == 0) {
      clrFill = bIsHighlighted ? s->clrWindow : s->clrBarFace;
    }
    imdraw_poly(im, &rectTab, pts, POINTS_NUM, 1, clrFill, 0, 0);
  }
  im->Polyline(pts, POINTS_NUM);
  im->SelectObject(pOldPen);
  return s->clrBarText;
}
COLOR VisualManager_OnDrawRibbonPanel(
    img_t* im,
    RibbonPanel* pPanel,
    IRECT rectPanel,
    IRECT /*rectCaption*/)
{
  ASSERT_VALID(pPanel);
  COLOR clrText = s->clrBarText;
  if (pPanel->IsCollapsed() && pPanel->GetDefaultButton()->IsFocused()) {
    imdraw_FillRect(im, 0, rectPanel, s->clrHilite);
    clrText = s->clrTextHilite;
  }
  else if (pPanel->IsHighlighted()) {
    HighlightRect(im, 0, rectPanel);
  }
  imdraw_Draw3dRect(im, 0, &rectPanel, s->clrBarHilite, s->clrBarHilite);
  rectPanel iRectOffset(&, -1, -1);
  imdraw_Draw3dRect(im, 0, &rectPanel, s->clrBarShadow, s->clrBarShadow);
  return clrText;
}
int VisualManager_OnDrawRibbonPanelCaption(
    img_t* im,
    RibbonPanel* pPanel,
    IRECT rectCaption)
{
  ASSERT_VALID(pPanel);
  rectCaption iRectDeflate(&, 1, 1);
  rectCaption.r -= 2;
  COLOR clrText = OnFillRibbonPanelCaption(im, pPanel, rectCaption);
  COLOR clrTextOld = im->SetTextColor(clrText);
  CString str = pPanel->GetName();
  if (pPanel->GetLaunchButton()->GetID() > 0) {
    rectCaption.r = pPanel->GetLaunchButton()->GetRect().l;
  }
  im->DrawText(str, rectCaption,
      DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX);
  im->SetTextColor(clrTextOld);
}
COLOR VisualManager_OnFillRibbonPanelCaption(uidraw_t* s, img_t* im, RibbonPanel* pPanel, IRECT rectCaption)
{
  ASSERT_VALID(pPanel);
  COLOR clr = (pPanel->IsHighlighted() ?
      s->clrActiveCaption : s->clrInactiveCaption);
  imdraw_FillRect(im, 0, rectCaption, clr);
  return pPanel->IsHighlighted() ? s->clrCaptionText : s->clrInactiveCaptionText;
}
int VisualManager_OnDrawRibbonLaunchButton(
    img_t* im,
    RibbonLaunchButton* pButton,
    RibbonPanel* pPanel)
{
  ASSERT_VALID(pPanel);
  OnFillRibbonButton(im, pButton);
  COLOR clrText = pPanel->IsHighlighted() ?
      s->clrCaptionText : s->clrInactiveCaptionText;
  IMAGE_STATE imageState = ImageBlack;
  if (pButton->IsDisabled()) {
    imageState = ImageGray;
  }
  else if (!pButton->IsHighlighted()) {
    if (GetRV(clrText) > 192 &&
        GetGV(clrText) > 192 &&
        GetBV(clrText) > 192) {
      imageState = ImageWhite;
    }
    else {
      imageState = ImageBlack;
    }
  }
  MenuImages_Draw(im, IdLaunchArrow,
      pButton->GetRect(), imageState);
  OnDrawRibbonButtonBorder(im, pButton);
}
int VisualManager_OnDrawRibbonDefaultPaneButton(
    img_t* im,
    RibbonButton* pButton)
{
  if (pButton->IsQATMode()) {
    OnFillRibbonButton(im, pButton);
    OnDrawRibbonDefaultPaneButtonContext(im, pButton);
    OnDrawRibbonButtonBorder(im, pButton);
  }
  else {
    OnDrawRibbonDefaultPaneButtonContext(im, pButton);
  }
}
int VisualManager_OnDrawRibbonDefaultPaneButtonContext(
    img_t* im,
    RibbonButton* pButton)
{
  IRECT rectMenuArrow = pButton->GetRect();;
  if (pButton->IsQATMode()) {
    pButton->DrawImage(im, RibbonImageSmall, pButton->GetRect());
    return 0;
  }
  IRECT rectImage = pButton->GetRect();
  rectImage.t += 10;
  rectImage.b = rectImage.t + pButton->GetImageSize(RibbonImageSmall).h;
  pButton->DrawImage(im, RibbonImageSmall, rectImage);
  // Draw text:
  pButton->DrawBottomText(im, FALSE);
}
int VisualManager_OnDrawRibbonDefaultPaneButtonIndicator(
    img_t* im,
    RibbonButton* /*pButton*/,
    IRECT rect,
    BOOL /*bIsSelected*/,
    BOOL bHighlighted)
{
  rect.l = rect.r - rect.Height();
  iRectDeflate(&rect, 1, 1);
  imdraw_FillRect(im, 0, rect, s->clrBarFace);
  imdraw_Draw3dRect(im, 0, &rect, s->clrBarShadow, s->clrBarShadow);
  IRECT rectWhite = rect;
  rectWhite iRectOffset(&, 0, 1);
  MenuImages_Draw(im, IdArowDown, rectWhite,
      ImageWhite);
  MenuImages_Draw(im, IdArowDown, rect,
      ImageBlack);
}
COLOR VisualManager_OnFillRibbonButton(
    img_t* im,
    RibbonButton* pButton)
{
  int cls = pButton->GetClass();
  if (CLASS_RibbonEdit == cls) {
    COLOR clrBorder = s->clrBarShadow;
    IRECT rectCommand = pButton->GetCommandRect();
    IRECT rect = pButton->GetRect();
    rect.l = rectCommand.l;
    if (s->m_bIsDrawOnGlass) {
      imdraw_DrawRectR(im, 0, rect, s->clrWindow, clrBorder);
    }
    else {
      if (pButton->IsDroppedDown() || pButton->IsHighlighted()) {
        imdraw_FillRect(im, 0, rectCommand, s->clrWindow);
      }
      else {
        HighlightRect(im, 0, rectCommand);
      }
      imdraw_Draw3dRect(im, 0, &rect, clrBorder, clrBorder);
    }
    return 0;
  }
  if (pButton->IsMenuMode() && !pButton->IsPaletteIcon()) {
    if (pButton->IsHighlighted()) {
      imdraw_FillRect(im, 0, &pButton->GetRect(), s->clrHilite);
      return s->clrTextHilite;
    }
  }
  else {
    if (pButton->IsChecked() && !pButton->IsHighlighted()) {
      if (s->m_bIsDrawOnGlass) {
        imdraw_DrawRectR(im, 0, pButton->GetRect(), s->clrWindow, 0);
      }
      else {
        FillDitheredRect(im, pButton->GetRect());
      }
    }
  }
  return 0;
}
COLOR VisualManager_OnFillRibbonMainPanelButton(
    img_t* im,
    RibbonButton* pButton)
{
  return OnFillRibbonButton(im, pButton);
}
int VisualManager_OnDrawRibbonMainPanelButtonBorder(
    img_t* im, RibbonButton* pButton)
{
  OnDrawRibbonButtonBorder(im, pButton);
}
int VisualManager_OnFillRibbonEdit(
    img_t* im,
    RibbonEditCtrl* /*pEdit*/,
    IRECT rect,
    BOOL bIsHighlighted,
    BOOL /*bIsPaneHighlighted*/,
    BOOL bIsDisabled,
    COLOR& /*clrText*/, COLOR& /*clrSelBackground*/, COLOR& /*clrSelText*/)
{
  if (bIsHighlighted && !bIsDisabled) {
    if (s->m_bIsDrawOnGlass) {
      imdraw_DrawRectR(im, 0, rect, s->clrWindow, 0);
    }
    else {
      imdraw_FillRect(im, 0, rect, s->clrWindow);
    }
  }
  else {
    if (s->m_bIsDrawOnGlass) {
      imdraw_DrawRectR(im, 0, rect, s->clrBarFace, 0);
    }
    else {
      imdraw_FillRect(im, 0, rect, s->clrBarFace);
      HighlightRect(im, 0, rect);
    }
  }
}
COLOR VisualManager_GetRibbonEditBackgroundColor(
    RibbonEditCtrl* /*pEdit*/,
    BOOL bIsHighlighted,
    BOOL /*bIsPaneHighlighted*/,
    BOOL bIsDisabled)
{
  return(bIsHighlighted && !bIsDisabled) ? s->clrWindow : s->clrBarFace;
}
int VisualManager_OnDrawRibbonButtonBorder(
    img_t* im,
    RibbonButton* pButton)
{
  if (CLASS_RibbonEdit == pButton->GetClass()) {
    return 0;
  }
  IRECT rect = pButton->GetRect();
  if (pButton->IsMenuMode() &&
      pButton->IsChecked() && !pButton->IsHighlighted()) {
    return 0;
  }
  if (pButton->IsHighlighted() || pButton->IsChecked() ||
      pButton->IsDroppedDown() || pButton->IsFocused()) {
    if (s->m_bIsDrawOnGlass) {
      imdraw_DrawRectR(im, 0, rect, 0, s->clrBarShadow);
    }
    else {
      if (pButton->IsPressed() || pButton->IsChecked() ||
          pButton->IsDroppedDown()) {
        imdraw_Draw3dRect(im, 0, &rect, s->clrBarShadow, s->clrBarHilite);
      }
      else {
        imdraw_Draw3dRect(im, 0, &rect, s->clrBarHilite, s->clrBarShadow);
      }
    }
    IRECT rectMenu = pButton->GetMenuRect();
    if (!rectMenu.IsRectEmpty()) {
      if (s->m_bIsDrawOnGlass) {
        if (pButton->IsMenuOnBottom()) {
          imdraw_DrawLine(im, 0, rectMenu.l, rectMenu.t, rectMenu.r, rectMenu.t, s->clrBarShadow);
        }
        else {
          imdraw_DrawLine(im, 0, rectMenu.l, rectMenu.t, rectMenu.l, rectMenu.b, s->clrBarShadow);
        }
      }
      else {
        COLOR pOldPen = im->SelectObject(s->clrBarShadow);
        ASSERT(pOldPen != NULL);
        if (pButton->IsMenuOnBottom()) {
          im->MoveTo(rectMenu.l, rectMenu.t);
          im->LineTo(rectMenu.r, rectMenu.t);
        }
        else {
          im->MoveTo(rectMenu.l, rectMenu.t);
          im->LineTo(rectMenu.l, rectMenu.b);
        }
        im->SelectObject(pOldPen);
      }
    }
  }
}
int VisualManager_OnDrawRibbonMenuCheckFrame(
    img_t* im,
    RibbonButton* /*pButton*/,
    IRECT rect)
{
  imdraw_FillRect(im, 0, rect, s->clrBtnFace);
  imdraw_Draw3dRect(im, 0, &rect, s->clrBtnShadow,
      s->clrBtnHilite);
}
int VisualManager_OnDrawRibbonMainPanelFrame(
    img_t* im,
    RibbonMainPanel* /*pPanel*/,
    IRECT rect)
{
  imdraw_Draw3dRect(im, 0, &rect, s->clrBarShadow, s->clrBarShadow);
  rect.InflateRect(1, 1);
  imdraw_Draw3dRect(im, 0, &rect, s->clrBarHilite, s->clrBarHilite);
}
int VisualManager_OnFillRibbonMenuFrame(
    img_t* im,
    RibbonMainPanel* /*pPanel*/,
    IRECT rect)
{
  imdraw_FillRect(im, 0, rect, s->clrWindow);
}
int VisualManager_OnDrawRibbonRecentFilesFrame(
    img_t* im,
    RibbonMainPanel* /*pPanel*/,
    IRECT rect)
{
  imdraw_FillRect(im, 0, rect, s->clrBtnFace);
  IRECT rectSeparator = rect;
  rectSeparator.r = rectSeparator.l + 2;
  imdraw_Draw3dRect(im, 0, &rectSeparator, s->clrBtnShadow,
      s->clrBtnHilite);
}
int VisualManager_OnDrawRibbonLabel(
    img_t* im,
    RibbonLabel* /*pLabel*/,
    IRECT rect)
{
}
int VisualManager_OnDrawRibbonPaletteButton(
    img_t* im,
    RibbonPaletteIcon* pButton)
{
  OnFillRibbonButton(im, pButton);
  OnDrawRibbonButtonBorder(im, pButton);
}
int VisualManager_OnDrawRibbonPaletteBorder(
    img_t* im,
    RibbonPaletteButton* /*pButton*/,
    IRECT rectBorder)
{
  imdraw_Draw3dRect(im, 0, &rectBorder, s->clrBarShadow, s->clrBarShadow);
}
COLOR VisualManager_RibbonCategoryColorToRGB(BCGPRibbonCategoryColor color)
{
  if (s->s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    switch (color) {
    case BCGPCategoryColor_Red:
      return _RGB(255, 0, 0);
    case BCGPCategoryColor_Orange:
      return _RGB(255, 128, 0);
    case BCGPCategoryColor_Yellow:
      return _RGB(255, 255, 0);
    case BCGPCategoryColor_Green:
      return _RGB(0, 255, 0);
    case BCGPCategoryColor_Blue:
      return _RGB(0, 0, 255);
    case BCGPCategoryColor_Indigo:
      return _RGB(0, 0, 128);
    case BCGPCategoryColor_Violet:
      return _RGB(255, 0, 255);
    }
    return 0;
  }
  switch (color) {
  case BCGPCategoryColor_Red:
    return _RGB(255, 160, 160);
  case BCGPCategoryColor_Orange:
    return _RGB(239, 189, 55);
  case BCGPCategoryColor_Yellow:
    return _RGB(253, 229, 27);
  case BCGPCategoryColor_Green:
    return _RGB(113, 190, 89);
  case BCGPCategoryColor_Blue:
    return _RGB(128, 181, 196);
  case BCGPCategoryColor_Indigo:
    return _RGB(114, 163, 224);
  case BCGPCategoryColor_Violet:
    return _RGB(214, 178, 209);
  }
  return 0;
}
COLOR VisualManager_OnDrawRibbonCategoryCaption(
    img_t* im,
    RibbonContextCaption* pContextCaption)
{
  ASSERT_VALID(pContextCaption);
  COLOR clrFill = RibbonCategoryColorToRGB(pContextCaption->GetColor());
  IRECT rect = pContextCaption->GetRect();
  if (clrFill != 0) {
    if (s->m_bIsDrawOnGlass) {
      iRectDeflate(&rect, 0, 1);
      imdraw_DrawRectR(im, 0, rect, clrFill, 0);
    }
    else {
      COLOR clr = (clrFill);
      imdraw_FillRect(im, 0, rect, clr);
    }
  }
  return s->clrBarText;
}
COLOR VisualManager_OnDrawRibbonStatusBarPane(uidraw_t* s, img_t* im, RibbonStatusBar* /*pBar*/,
    RibbonStatusBarPane* pPane)
{
  ASSERT_VALID(pPane);
  IRECT rect = pPane->GetRect();
  if (pPane->IsHighlighted()) {
    IRECT rectButton = rect;
    rectButton iRectDeflate(&, 1, 1);
    imdraw_Draw3dRect(im, 0, &rectButton,
        pPane->IsPressed() ? s->clrBarShadow : s->clrBarHilite,
        pPane->IsPressed() ? s->clrBarHilite : s->clrBarShadow);
  }
  return 0;
}
int VisualManager_GetRibbonSliderColors(RibbonSlider* /*pSlider*/,
    BOOL /*bIsHighlighted*/,
    BOOL /*bIsPressed*/,
    BOOL /*bIsDisabled*/,
    COLOR& /*clrLine*/,
    COLOR& /*clrFill*/)
{
  /* clrLine = bIsDisabled ? s->clrBarShadow : s->clrBarDkShadow;
  clrFill = bIsPressed && bIsHighlighted ?
  s->clrBarShadow :
  bIsHighlighted ? s->clrBarHilite : s->clrBarFace;*/
}
int VisualManager_OnDrawRibbonSliderZoomButton(
    img_t* im, RibbonSlider* /*pSlider*/,
    IRECT rect, BOOL bIsZoomOut,
    BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled)
{
  COLOR clrLine = bIsDisabled ? s->clrBarShadow : s->clrBarDkShadow;
  COLOR clrFill = bIsPressed && bIsHighlighted ?
      s->clrBarShadow :
      bIsHighlighted ? s->clrBarHilite : s->clrBarFace;
  CPoint ptCenter = rect.CenterPoint();
  IRECT rectCircle(CPoint(ptCenter.x - 7, ptCenter.y - 7), CSize(15, 15));
  imdraw_EllipseR(im, 0, rectCircle, clrFill, clrLine, 1);
  // Draw +/- sign:
  IRECT rectSign(CPoint(ptCenter.x - 3, ptCenter.y - 3), CSize(7, 7));
  if (s->m_bIsDrawOnGlass) {
    imdraw_DrawLine(im, 0, rectSign.l, ptCenter.y, rectSign.r, ptCenter.y, clrLine);
    if (!bIsZoomOut) {
      imdraw_DrawLine(im, 0, ptCenter.x, rectSign.t, ptCenter.x, rectSign.b, clrLine);
    }
  }
  else {
    COLOR pOldPen = im->SelectObject(clrLine);
    im->MoveTo(rectSign.l, ptCenter.y);
    im->LineTo(rectSign.r, ptCenter.y);
    if (!bIsZoomOut) {
      im->MoveTo(ptCenter.x, rectSign.t);
      im->LineTo(ptCenter.x, rectSign.b);
    }
    im->SelectObject(pOldPen);
  }
}
int VisualManager_OnDrawRibbonSliderChannel(
    img_t* im, RibbonSlider* pSlider, IRECT rect)
{
  BOOL bIsVert = FALSE;
  if (pSlider != NULL) {
    ASSERT_VALID(pSlider);
    bIsVert = pSlider->IsVert();
  }
  OnDrawSliderChannel(im, NULL, bIsVert, rect, s->m_bIsDrawOnGlass);
}
int VisualManager_OnDrawRibbonSliderThumb(
    img_t* im, RibbonSlider* pSlider,
    IRECT rect, BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled)
{
  BOOL bIsVert = FALSE;
  BOOL bLeftTop = FALSE;
  BOOL bRightBottom = FALSE;
  if (pSlider != NULL) {
    ASSERT_VALID(pSlider);
    bIsVert = pSlider->IsVert();
    bLeftTop = pSlider->IsThumbLeftTop();
    bRightBottom = pSlider->IsThumbRightBottom();
  }
  OnDrawSliderThumb(im, NULL, rect, bIsHighlighted, bIsPressed, bIsDisabled,
      bIsVert, bLeftTop, bRightBottom, s->m_bIsDrawOnGlass);
}
int VisualManager_OnDrawRibbonProgressBar(uidraw_t* s, img_t* im,
    RibbonProgressBar* /*pProgress*/,
    IRECT rectProgress, IRECT rectChunk,
    BOOL /*bInfiniteMode*/)
{
  if (s->m_bIsDrawOnGlass) {
    if (!rectChunk.IsRectEmpty()) {
      imdraw_DrawRectR(im, 0, rectChunk, s->clrHilite, 0);
    }
    imdraw_DrawRectR(im, 0, rectProgress, 0, s->clrBarShadow);
  }
  else {
    if (!rectChunk.IsRectEmpty()) {
      imdraw_FillRect(im, 0, rectChunk, s->clrHilite);
    }
    imdraw_Draw3dRect(im, 0, &rectProgress, s->clrBarShadow, s->clrBarHilite);
  }
}
int VisualManager_OnFillRibbonQATPopup(
    img_t* im, RibbonPanelMenuBar* /*pMenuBar*/, IRECT rect)
{
  imdraw_FillRect(im, 0, rect, s->clrBarFace);
}
int VisualManager_OnDrawRibbonQATSeparator(uidraw_t* s, img_t* im,
    RibbonSeparator* /*pSeparator*/, IRECT rect)
{
  if (s->m_bIsDrawOnGlass) {
    imdraw_DrawRectR(im, 0, rect, 0, s->clrBtnShadow);
  }
  else {
    imdraw_Draw3dRect(im, 0, &rect, s->clrBarShadow, s->clrBarHilite);
  }
}
int VisualManager_OnDrawRibbonKeyTip(uidraw_t* s, img_t* im, BaseRibbonElement* pElement,
    IRECT rect, CString str)
{
  ASSERT_VALID(pElement);
  COLOR clrText = _GetSysColor(CLR_INFOTEXT);
  COLOR clrBorder = clrText;
  {
    ::imdraw_FillRect(im, 0, rect, GetSysColorBrush(CLR_INFOBK));
  }
  str.MakeUpper();
  COLOR clrTextOld = im->SetTextColor(pElement->IsDisabled() ? s->clrGrayedText : clrText);
  im->DrawText(str, rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
  im->SetTextColor(clrTextOld);
  imdraw_Draw3dRect(im, 0, &rect, clrBorder, clrBorder);
}
int VisualManager_OnDrawRibbonRadioButtonOnList(uidraw_t* s, img_t* im, RibbonRadioButton* /*pRadioButton*/,
    IRECT rect, BOOL /*bIsSelected*/, BOOL bHighlighted)
{
  rect iRectOffset(&, 1, 1);
  MenuImages_Draw(im, IdRadio, rect, ImageWhite);
  rect iRectOffset(&, -1, -1);
  MenuImages_Draw(im, IdRadio, rect, ImageBlack);
}
int VisualManager_OnDrawRibbonCheckBoxOnList(uidraw_t* s, img_t* im, RibbonCheckBox* /*pCheckBox*/,
    IRECT rect, BOOL /*bIsSelected*/, BOOL bHighlighted)
{
  rect iRectOffset(&, 1, 1);
  MenuImages_Draw(im, IdCheck, rect, ImageWhite);
  rect iRectOffset(&, -1, -1);
  MenuImages_Draw(im, IdCheck, rect, ImageBlack);
}
COLOR VisualManager_GetRibbonHyperlinkTextColor(RibbonHyperlink* pHyperLink)
{
  ASSERT_VALID(pHyperLink);
  if (pHyperLink->IsDisabled()) {
    return GetToolbarDisabledTextColor();
  }
  return pHyperLink->IsHighlighted() ? s->clrHotLinkText : s->clrHotText;
}
COLOR VisualManager_GetRibbonStatusBarTextColor(RibbonStatusBar* /*pStatusBar*/)
{
  return s->clrBarText;
}
int VisualManager_OnDrawRibbonColorPaletteBox(uidraw_t* s, img_t* im, RibbonColorButton* /*pColorButton*/,
    RibbonPaletteIcon* /*pIcon*/,
    COLOR color, IRECT rect, BOOL bDrawTopEdge, BOOL bDrawBottomEdge,
    BOOL bIsHighlighted, BOOL bIsChecked, BOOL /*bIsDisabled*/)
{
  IRECT rectFill = rect;
  rectFill iRectDeflate(&, 1, 0);
  if (bIsHighlighted || bIsChecked) {
    FillDitheredRect(im, rect);
    rectFill iRectDeflate(&, 1, 2);
  }
  if (color != 0) {
    COLOR clr = (color);
    imdraw_FillRect(im, 0, rectFill, clr);
  }
  COLOR clrBorder = s->clrBtnShadow;
  if (bDrawTopEdge && bDrawBottomEdge) {
    imdraw_Draw3dRect(im, 0, &rect, clrBorder, clrBorder);
  }
  else {
    COLOR pOldPen = im->SelectObject(clrBorder);
    ASSERT(pOldPen != NULL);
    im->MoveTo(rect.l, rect.t);
    im->LineTo(rect.l, rect.b);
    im->MoveTo(rect.r - 1, rect.t);
    im->LineTo(rect.r - 1, rect.b);
    if (bDrawTopEdge) {
      im->MoveTo(rect.l, rect.t);
      im->LineTo(rect.r, rect.t);
    }
    if (bDrawBottomEdge) {
      im->MoveTo(rect.l, rect.b - 1);
      im->LineTo(rect.r, rect.b - 1);
    }
    im->SelectObject(pOldPen);
  }
  if (bIsHighlighted) {
    imdraw_Draw3dRect(im, 0, &rect, s->clrBarHilite, s->clrBarShadow);
  }
  else if (bIsChecked) {
    imdraw_Draw3dRect(im, 0, &rect, s->clrBarShadow, s->clrBarHilite);
  }
}
#endif // BCGP_EXCLUDE_RIBBON
BOOL VisualManager_OnSetWindowRegion(CWnd* pWnd, CSize sizeWindow)
{
#if 0
#ifdef BCGP_EXCLUDE_RIBBON
  UNREFERENCED_PARAMETER(pWnd);
  UNREFERENCED_PARAMETER(sizeWindow);
  return FALSE;
#else
  if (s->DwmIsCompositionEnabled()) {
    return FALSE;
  }
  ASSERT_VALID(pWnd);
  RibbonBar* pRibbonBar = NULL;
  if (pWnd->IsKindOf(RUNTIME_CLASS(FrameWnd))) {
    pRibbonBar = ((FrameWnd*) pWnd)->GetRibbonBar();
  }
  else if (pWnd->IsKindOf(RUNTIME_CLASS(MDIFrameWnd))) {
    pRibbonBar = ((MDIFrameWnd*) pWnd)->GetRibbonBar();
  }
  if (pRibbonBar == NULL ||
      !pRibbonBar->IsWindowVisible() ||
      !pRibbonBar->IsReplaceFrameCaption()) {
    return FALSE;
  }
  const int nLeftRadius = 11;
  const int nRightRadius = 11;
  CRgn rgnWnd;
  rgnWnd.CreateRectRgn(0, 0, sizeWindow.w, sizeWindow.h);
  CRgn rgnTemp;
  rgnTemp.CreateRectRgn(0, 0, nLeftRadius / 2, nLeftRadius / 2);
  rgnWnd.CombineRgn(&rgnTemp, &rgnWnd, RGN_XOR);
  rgnTemp.DeleteObject();
  rgnTemp.CreateEllipticRgn(0, 0, nLeftRadius, nLeftRadius);
  rgnWnd.CombineRgn(&rgnTemp, &rgnWnd, RGN_OR);
  rgnTemp.DeleteObject();
  rgnTemp.CreateRectRgn(sizeWindow.w - nRightRadius / 2, 0, sizeWindow.w, nRightRadius / 2);
  rgnWnd.CombineRgn(&rgnTemp, &rgnWnd, RGN_XOR);
  rgnTemp.DeleteObject();
  rgnTemp.CreateEllipticRgn(sizeWindow.w - nRightRadius + 1, 0, sizeWindow.w + 1, nRightRadius);
  rgnWnd.CombineRgn(&rgnTemp, &rgnWnd, RGN_OR);
  pWnd->SetWindowRgn((HRGN)rgnWnd.Detach(), TRUE);
#endif
#endif
  return TRUE;
}
BOOL VisualManager_OnNcPaint(CWnd* /*pWnd*/, const CObList& /*lstSysButtons*/, IRECT /*rectRedraw*/)
{
  return FALSE;
}
int VisualManager_OnActivateApp(CWnd* /*pWnd*/, BOOL /*bActive*/)
{
}
BOOL VisualManager_OnNcActivate(CWnd* /*pWnd*/, BOOL /*bActive*/)
{
  return FALSE;
}
CSize VisualManager_GetNcBtnSize(BOOL /*bSmall*/) const
{
  return CSize(0, 0);
}
BOOL VisualManager_OnUpdateNcButtons(CWnd* /*pWnd*/, const CObList& /*lstSysButtons*/, IRECT /*rectCaption*/)
{
  return FALSE;
}
BOOL VisualManager_OnEraseMDIClientArea(uidraw_t* s, img_t* im, IRECT /*rectClient*/)
{
  return FALSE;
}
BOOL VisualManager_GetToolTipParams(ToolTipParams& params,
    UINT /*nType*/ /*=(UINT)(-1)*/)
{
  ToolTipParams dummy;
  params = dummy;
  return TRUE;
}
int VisualManager_OnFillToolTip(uidraw_t* s, img_t* im, ToolTipCtrl* /*pToolTip*/, IRECT rect,
    COLOR& clrText, COLOR& clrLine)
{
  {
    ::imdraw_FillRect(im, 0, rect, GetSysColorBrush(CLR_INFOBK));
  }
}
BOOL VisualManager_DrawTextOnGlass(uidraw_t* s, img_t* im, CString strText, IRECT rect,
    DWORD dwFlags, int nGlowSize, COLOR clrText)
{
  //---- bits used in dwFlags of DTTOPTS ----
#define BCGP_DTT_TEXTCOLOR (1UL << 0) // crText has been specified
#define BCGP_DTT_BORDERCOLOR (1UL << 1) // crBorder has been specified
#define BCGP_DTT_SHADOWCOLOR (1UL << 2) // crShadow has been specified
#define BCGP_DTT_SHADOWTYPE (1UL << 3) // iTextShadowType has been specified
#define BCGP_DTT_SHADOWOFFSET (1UL << 4) // ptShadowOffset has been specified
#define BCGP_DTT_BORDERSIZE (1UL << 5) // iBorderSize has been specified
#define BCGP_DTT_FONTPROP (1UL << 6) // iFontPropId has been specified
#define BCGP_DTT_COLORPROP (1UL << 7) // iColorPropId has been specified
#define BCGP_DTT_STATEID (1UL << 8) // IStateId has been specified
#define BCGP_DTT_CALCRECT (1UL << 9) // Use pRect as and in/out parameter
#define BCGP_DTT_APPLYOVERLAY (1UL << 10) // fApplyOverlay has been specified
#define BCGP_DTT_GLOWSIZE (1UL << 11) // iGlowSize has been specified
#define BCGP_DTT_CALLBACK (1UL << 12) // pfnDrawTextCallback has been specified
#define BCGP_DTT_COMPOSITED (1UL << 13) // Draws text with antialiased alpha(needs a DIB section)
  im->DrawText(strText, rect, dwFlags);
  return TRUE;
}
COLOR VisualManager_OnFillListBoxItem(uidraw_t* s, img_t* im, ListBox* /*pListBox*/, int /*nItem*/, IRECT rect, BOOL bIsHighlihted, BOOL bIsSelected)
{
  COLOR clrText = 0;
  if (bIsSelected) {
    imdraw_FillRect(im, 0, rect, s->clrHilite);
    clrText = s->clrTextHilite;
  }
  if (bIsHighlihted) {
    im->DrawFocusRect(rect);
  }
  return clrText;
}
int VisualManager_OnDrawGroup(uidraw_t* s, img_t* im, Group* pGroup, IRECT rect, const CString& strName)
{
  ASSERT_VALID(pGroup);
  CSize sizeText = im->GetTextExtent(strName);
  IRECT rectFrame = rect;
  rectFrame.t += sizeText.h / 2;
  if (sizeText == CSize(0, 0)) {
    rectFrame.t += im->GetTextExtent(("A")).h / 2;
  }
  int xMargin = sizeText.h / 2;
  IRECT rectText = rect;
  rectText.l += xMargin;
  rectText.r = rectText.l + sizeText.w + xMargin;
  rectText.b = rectText.t + sizeText.h;
  if (!strName.IsEmpty()) {
    im->ExcludeClipRect(rectText);
  }
  {
    imdraw_DrawRectR(im, 0, rectFrame, 0, s->clrBarDkShadow);
  }
  im->SelectClipRgn(NULL);
  if (strName.IsEmpty()) {
    return 0;
  }
  DWORD dwTextStyle = DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_NOCLIP;
  if (pGroup->IsOnGlass()) {
    DrawTextOnGlass(im, strName, rectText, dwTextStyle, 10, s->clrBarText);
  }
  else {
    CString strCaption = strName;
    im->DrawText(strCaption, rectText, dwTextStyle);
  }
}
BOOL VisualManager_OnFillDialog(uidraw_t* s, img_t* im, IRECT rect)
{
  imdraw_FillRect(im, 0, rect, GetDlgBackBrush());
  return TRUE;
}
COLOR VisualManager_GetDlgBackBrush()
{
  return s->clrBarFace;
}
int VisualManager_OnDrawSliderChannel(uidraw_t* s, img_t* im, SliderCtrl* /*pSlider*/, BOOL bVert, IRECT rect, BOOL bDrawOnGlass)
{
  if (bVert) {
    if (rect.Width() < 3) {
      rect.r++;
    }
  }
  else {
    if (rect.Height() < 3) {
      rect.b++;
    }
  }
  if (bDrawOnGlass) {
    imdraw_DrawRectR(im, 0, rect, 0, s->clrBarShadow);
  }
  else {
    imdraw_Draw3dRect(im, 0, &rect, s->clrBarShadow, s->clrBarHilite);
  }
}
int VisualManager_OnDrawSliderThumb(uidraw_t* s, img_t* im, SliderCtrl* /*pSlider*/,
    IRECT rect, BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled,
    BOOL bVert, BOOL bLeftTop, BOOL bRightBottom, BOOL /*bDrawOnGlass*/)
{
  COLOR clrLine = bIsDisabled ? s->clrBarShadow : s->clrBarDkShadow;
  COLOR clrFill = !bIsDisabled && (bIsHighlighted || bIsPressed) ?
      s->clrBarHilite : s->clrBarFace;
  if (bVert) {
    iRectDeflate(&rect, 2, 1);
    rect.l = rect.CenterPoint().x - rect.Height();
    rect.r = rect.l + 2 * rect.Height();
  }
  else {
    iRectDeflate(&rect, 1, 2);
    rect.t = RCY(&rect) / 2 - rect.Width();
    rect.b = rect.t + 2 * rect.Width();
  }
  imdraw_DrawRectR(im, 0, rect, clrFill, clrLine);
}
COLOR VisualManager_GetURLLinkColor(URLLinkButton* /*pButton*/, BOOL bHover)
{
  return bHover ? s->clrHotLinkText : s->clrHotText;
}
BOOL VisualManager_OnFillParentBarBackground(CWnd* pWnd, img_t* im, IRECT* rectClip)
{
#if 0
  ASSERT_VALID(pWnd);
  CWnd* pParent = pWnd->GetParent();
  ASSERT_VALID(pParent);
  int* pParentBar = DYNAMIC_DOWNCAST(int, pParent);
  if (pParentBar == NULL) {
    return FALSE;
  }
  DialogBar* pDialogBar = DYNAMIC_DOWNCAST(DialogBar, pParentBar);
  if (pDialogBar != NULL && pDialogBar->IsVisualManagerStyle()) {
    return FALSE;
  }
  CRgn rgn;
  if (rectClip != NULL) {
    rgn.CreateRectRgnIndirect(rectClip);
    im->SelectClipRgn(&rgn);
  }
  CPoint pt(0, 0);
  pWnd->MapWindowPoints(pParent, &pt, 1);
  pt = im->OffsetWindowOrg(pt.x, pt.y);
  IRECT rectBar;
  pParentBar->GetClientRect(rectBar);
  OnFillBarBackground(im, pParentBar, rectBar, rectBar);
  im->SetWindowOrg(pt.x, pt.y);
  im->SelectClipRgn(NULL);
#endif
  return TRUE;
}
#undef POINTS_NUM
#endif
// Initialization code
int uidraw_init(uidraw_t* s, sys_t* sys)
{
  sys_init(sys);
  s->sys = sys;
  sys->drawer = s;
  s->m_nBitsPerPixel = 32;
  s->bIsOSAlphaBlendingSupport = TRUE;
  s->bIsRemoteSession = FALSE;
  s->bDisableAero = FALSE;
  s->m_bIsRibbonImageScale = TRUE;
  //---------------------------------------------------------
  // Cached system values(updated in CWnd::OnSysColorChange)
  //---------------------------------------------------------
  s->hbrBtnShadow = 0;
  s->hbrBtnHilite = 0;
  s->hbrWindow = 0;
  UpdateSysColors(s);
  s->m_hcurStretch = NULL;
  s->m_hcurStretchVert = NULL;
  s->m_hcurHand = NULL;
  s->m_hcurSizeAll = NULL;
  s->m_hiconTool = NULL;
  s->m_hiconLink = NULL;
  s->m_hiconColors = NULL;
  s->m_hcurMoveTab = NULL;
  s->m_hcurNoMoveTab = NULL;
  s->m_hcurSelectRow = NULL;
  s->m_bUseSystemFont = FALSE;
  s->m_bInSettingsChange = FALSE;
  UpdateFonts(s, sys);
  OnSettingChange(s);
  s->m_bIsRTL = FALSE;
  //------------------
  // Small icon sizes:
  //------------------
  s->m_nDragFrameThiknessFloat = 4; // pixels
  s->m_nDragFrameThiknessDock = 3; // pixels
  s->m_nAutoHideToolBarSpacing = 14; // pixels
  s->m_nAutoHideToolBarMargin = 4; // pixels
  s->m_nCoveredMainWndClientAreaPercent = 50; // percents
  s->m_nMaxToolTipWidth = -1;
  s->m_bIsBlackHighContrast = FALSE;
  s->m_bIsWhiteHighContrast = FALSE;
  s->m_bUseBuiltIn32BitIcons = TRUE;
  s->m_bUseVisualManagerInBuiltInDialogs = FALSE;
  return 0;
}
COLOR VisualManager_GetWindowColor(uidraw_t* s)
{
  return s->clrWindow;
}
int VisualManager_OnFillHighlightedArea(img_t* im, IRECT rect, COLOR pBrush)
{
  imdraw_FillRect(im, 0, &rect, pBrush);
  return 0;
}
int VisualManager_init(uidraw_t* s, sys_t* sys)
{
  uidraw_init(s, sys);
#define XXX(type, name, args) s->name = VisualManager_##name;
  UIGLOBAL_FUN_DEF(XXX);
#undef XXX
  s->m_bAutoDestroy = FALSE;
  s->m_bLook2000 = FALSE;
  s->m_bMenuFlatLook = FALSE;
  s->m_nMenuShadowDepth = 6;
  s->m_bShadowHighlightedImage = FALSE;
  s->m_bEmbossDisabledImage = TRUE;
  s->m_bFadeInactiveImage = FALSE;
  s->m_bEnableToolbarButtonFill = TRUE;
  s->m_nVertMargin = 12;
  s->m_nHorzMargin = 12;
  s->m_nGroupVertOffset = 15;
  s->m_nGroupCaptionHeight = 25;
  s->m_nGroupCaptionHorzOffset = 13;
  s->m_nGroupCaptionVertOffset = 7;
  s->m_nTasksHorzOffset = 12;
  s->m_nTasksIconHorzOffset = 5;
  s->m_nTasksIconVertOffset = 4;
  s->m_bActiveCaptions = TRUE;
  s->m_bOfficeXPStyleMenus = FALSE;
  s->m_nMenuBorderSize = 2;
  s->m_b3DTabWideBorder = TRUE;
  s->m_bAlwaysFillTab = FALSE;
  s->m_bFrameMenuCheckedItems = FALSE;
  s->m_clrMenuShadowBase = 0; // Used in derived classes
  UpdateSysColors(s);
  s->m_bPlannerBackItemToday = FALSE;
  s->m_bPlannerBackItemSelected = FALSE;
  s->m_bPlannerCaptionBackItemHeader = FALSE;
  s->m_ptRibbonMainImageOffset = iPOINT(0, 0);
  s->OnUpdateSystemColors(s);
  return 0;
}
// BCGPVisualManagerXP.cpp: implementation of the VisualManagerXP class.
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int VisualManagerXP_OnUpdateSystemColors(uidraw_t* s)
{
  COLOR clrTabBack;
  COLOR clrFloatToolBarBorder;
  COLOR clrHL, clrMix;
  double H, S, L, S1, L1;
  VisualManager_OnUpdateSystemColors(s);
  if (s->m_nBitsPerPixel > 8 && !IsHighContastMode(s)) {
    COLOR clrWindow = s->GetWindowColor(s);
    COLOR clrFace = s->clrBarFace;
    s->m_clrMenuLight = _RGB(
        (219 * GetRV(clrWindow) + 36 * GetRV(clrFace)) / 255,
        (219 * GetGV(clrWindow) + 36 * GetGV(clrFace)) / 255,
        (219 * GetBV(clrWindow) + 36 * GetBV(clrFace)) / 255);
    RGBtoHSL(clrFace, &H, &S, &L);
    if (S < 0.1) {
      L1 = min(1., L + (1. - L) * .5);
      S1 = S == 0 ? 0 : min(1., S + .1);
    }
    else {
      L1 = min(1., 0.5 * L + 0.5);
      S1 = min(1., S * 2);
    }
    clrTabBack = HLStoRGB_ONE(H, L1, S1);
    s->m_clrBarBkgnd = _RGB(
        (40 * GetRV(clrWindow) + 215 * GetRV(clrFace)) / 255,
        (40 * GetGV(clrWindow) + 215 * GetGV(clrFace)) / 255,
        (40 * GetBV(clrWindow) + 215 * GetBV(clrFace)) / 255);
    s->m_clrMenuRarelyUsed = PixelAlpha2(s->m_clrBarBkgnd, 94);
    s->m_clrInactiveTabText = PixelAlpha2(clrFace, 55);
    clrHL = s->clrHilite;
    RGBtoHSL(clrHL, &H, &S, &L);
    clrMix = _RGB(
        (77 * GetRV(clrHL) + 178 * GetRV(s->m_clrMenuLight)) / 255,
        (77 * GetGV(clrHL) + 178 * GetGV(s->m_clrMenuLight)) / 255,
        (77 * GetBV(clrHL) + 178 * GetBV(s->m_clrMenuLight)) / 255);
    if (L > .8) { // The highlight color is very light
      s->m_clrHighlight = PixelAlpha2(clrMix, 91);
      s->m_clrHighlightDn = PixelAlpha2(clrMix, 98);
      s->m_clrMenuItemBorder = PixelAlpha2(s->clrHilite, 84);
    }
    else {
      s->m_clrHighlight = PixelAlpha2(clrMix, 102);
      s->m_clrHighlightDn = PixelAlpha2(s->m_clrHighlight, 87);
      s->m_clrMenuItemBorder = s->clrHilite;
    }
    s->m_clrHighlightChecked = PixelAlpha2(_RGB(
        (GetRV(clrHL) + 5 * GetRV(s->m_clrMenuLight)) / 6,
        (GetGV(clrHL) + 5 * GetGV(s->m_clrMenuLight)) / 6,
        (GetBV(clrHL) + 5 * GetBV(s->m_clrMenuLight)) / 6),
        100);
    s->m_clrSeparator = PixelAlpha4(s->clrBarFace, .86, .86, .86);
    s->m_clrPaneBorder = s->clrBarShadow;
    s->m_clrMenuBorder = PixelAlpha2(clrFace, 55);
    clrFloatToolBarBorder = PixelAlpha4(s->clrBarShadow, .85, .85, .85);
    s->m_clrGripper = PixelAlpha2(s->clrBarShadow, 110);
  }
  else {
    s->m_clrMenuLight = s->clrWindow;
    s->m_clrBarBkgnd = s->clrBtnFace;
    if (s->m_bIsBlackHighContrast) {
      s->m_clrHighlightChecked = s->m_clrHighlightDn = s->m_clrHighlight = s->clrHilite;
      s->m_clrMenuRarelyUsed = s->clrBtnFace;
    }
    else {
      s->m_clrHighlightDn = s->m_clrHighlight = s->clrBtnFace;
      s->m_clrHighlightChecked = s->clrWindow;
      s->m_clrMenuRarelyUsed = s->clrBarLight;
    }
    clrTabBack = s->clrBtnFace;
    s->m_clrInactiveTabText = s->clrBtnDkShadow;
    s->m_clrSeparator = s->clrBtnShadow;
    s->m_clrGripper = s->clrBtnShadow;
    s->m_clrPaneBorder = s->clrBtnShadow;
    s->m_clrMenuBorder = s->clrBtnDkShadow;
    clrFloatToolBarBorder = s->clrBtnShadow;
    s->m_clrMenuItemBorder = IsHighContastMode(s) ?
        s->clrBtnDkShadow : s->clrHilite;
  }
  s->m_clrPressedButtonBorder = 0; // Used in derived classes
  s->m_clrTaskPaneHotText = s->clrHotText;
  return 0;
}
#if 0
int VisualManagerXP_OnDrawBarGripper(img_t* im, IRECT rectGripper, BOOL bHorz,
    BaseControlBar* pBar)
{
  if (pBar != NULL && pBar->IsDialogControl()) {
    VisualManager::OnDrawBarGripper(im, rectGripper, bHorz, pBar);
    return 0;
  }
  if (s->m_clrGripperHorz.GetSafeHandle() == NULL) {
    CreateGripperBrush();
  }
  BOOL bSideBar = pBar != NULL && pBar->IsKindOf(RUNTIME_CLASS(DockingControlBar));
  BOOL bMenuBar = pBar != NULL && pBar->IsKindOf(RUNTIME_CLASS(MenuBar));
  IRECT rectFill = rectGripper;
  if (!bSideBar) {
    if (bHorz) {
      int xCenter = rectFill.CenterPoint().x;
      rectFill.l = xCenter - 1;
      rectFill.r = xCenter + 2;
      rectFill.DeflateRect(0, 5);
    }
    else {
      int yCenter = rectFill.CenterPoint().y;
      rectFill.t = yCenter - 1;
      rectFill.b = yCenter + 2;
      rectFill.DeflateRect(5, 0);
    }
  }
  else {
    if (bHorz) {
      rectFill.DeflateRect(4, 0);
    }
    else {
      rectFill.DeflateRect(4, 0);
    }
    bHorz = !bHorz;
  }
  COLOR clrTextOld = im->SetTextColor(s->m_clrGripper);
  COLOR clrBkOld = im->SetBkColor(bSideBar || bMenuBar ?
      s->clrBarFace : s->m_clrBarBkgnd);
  imdraw_FillRect(im, 0, rectFill, bHorz ? s->m_clrGripperHorz : s->m_clrGripperVert);
  if (bSideBar) {
    //------------------
    // Draw bar caption:
    //------------------
    int nOldBkMode = im->SetBkMode(OPAQUE);
    im->SetTextColor(s->clrBarText);
    const CFont& font = MenuBar::GetMenuFont(bHorz);
    CFont* pOldFont = im->SelectObject((CFont*) &font);
    CString strCaption;
    pBar->GetWindowText(strCaption);
    strCaption = (" ") + strCaption + (" ");
    IRECT rectText = rectGripper;
    UINT uiTextFormat = 0;
    TEXTMETRIC tm;
    im->GetTextMetrics(&tm);
    CPoint ptTextOffset(0, 0);
    if (bHorz) {
      ptTextOffset.y = (rectGripper.Height() - tm.tmHeight - 1) / 2;
    }
    else {
      ptTextOffset.x = (rectGripper.Width() - tm.tmHeight + 1) / 2;
    }
    if (bHorz) {
      rectText.t += ptTextOffset.y;
      im->DrawText(strCaption, &rectText, uiTextFormat);
    }
    else {
      rectText.l = rectText.r - ptTextOffset.x;
      rectText.t = rectGripper.t + ptTextOffset.y;
      rectText.b = rectGripper.t + 3 * ptTextOffset.y;
      uiTextFormat |= DT_NOCLIP;
      im->DrawText(strCaption, &rectText, uiTextFormat);
    }
    im->SelectObject(pOldFont);
    im->SetBkMode(nOldBkMode);
  }
  im->SetTextColor(clrTextOld);
  im->SetBkColor(clrBkOld);
}
int VisualManagerXP_OnDrawMenuBorder(img_t* im, PopupMenu* pMenu, IRECT rect)
{
  ASSERT_VALID(pMenu);
  const BOOL bRTL = pMenu->GetExStyle() & WS_EX_LAYOUTRTL;
  imdraw_Draw3dRect(im, 0, rect, s->m_clrMenuBorder, s->m_clrMenuBorder);
  rect.DeflateRect(1, 1);
  imdraw_Draw3dRect(im, 0, rect, s->m_clrMenuLight, s->m_clrMenuLight);
  IRECT rectLeft(1, 1, 2, rect.b - 1);
  imdraw_FillRect(im, 0, rectLeft, s->m_clrBarBkgnd);
  //------------------------------------------------
  // Quick Customize Office XP like draw popup menu
  //------------------------------------------------
  PopupMenu* pParentPopup = pMenu->GetParentPopupMenu();
  if (pParentPopup != NULL) {
    if (pParentPopup->IsQuickCustomize() && !bRTL) {
      ToolbarMenuButton* pParentBtn = pMenu->GetParentButton();
      if ((pParentBtn != NULL) && (pParentBtn->IsQuickMode())) {
        PopupMenu* pParent = (PopupMenu*)pMenu->GetParentPopupMenu();
        IRECT rcParent;
        pParent->GetWindowRect(rcParent);
        IRECT rcCurrent;
        pMenu->GetWindowRect(rcCurrent);
        ToolbarMenuButton* pBtn = pMenu->GetMenuItem(0);
        IRECT rcButton = pBtn->Rect();
        IRECT rectBorder;
        rectBorder.SetRectEmpty();
        if (rcParent.l > rcCurrent.l) {
          if (rcParent.t <= rcCurrent.t) {
            rectBorder.SetRect(
                rect.r - 1,
                rect.t,
                rect.r + 1,
                rcButton.b);
          }
          else {
            // up
            rectBorder.SetRect(
                rect.r - 1,
                rect.b - rcButton.Height(),
                rect.r + 1,
                rect.b);
          }
        }
        else {
          if (rcParent.t <= rcCurrent.t) {
            rectBorder.SetRect(
                rect.l - 1,
                rect.t,
                rect.l + 1,
                rcButton.b);
          }
          else {
            // up
            rectBorder.SetRect(
                rect.l - 1,
                rect.b - rcButton.Height(),
                rect.l + 1,
                rect.b);
          }
        }
        if (!rectBorder.IsRectEmpty()) {
          imdraw_FillRect(im, 0, rectBorder, s->m_clrBarBkgnd);
        }
      }
    }
  }
  if (!ToolBar::IsCustomizeMode()) {
    //-------------------------------------
    // "Connect" menu to the parent button:
    //-------------------------------------
    ToolbarMenuButton* pParentMenuBtn = pMenu->GetParentButton();
    if (s->m_bConnectMenuToParent &&
        pParentMenuBtn != NULL && pMenu->GetParentPopupMenu() == NULL &&
        pParentMenuBtn->IsBorder()) {
      IRECT rectConnect;
      rectConnect.SetRectEmpty();
      IRECT rectParent = pParentMenuBtn->Rect();
      CWnd* pWnd = pParentMenuBtn->GetParentWnd();
      pWnd->ClientToScreen(rectParent);
      pMenu->ScreenToClient(&rectParent);
      switch (pMenu->GetDropDirection()) {
      case PopupMenu::DROP_DIRECTION_BOTTOM:
        rectConnect = IRECT(rectParent.l + 1, rect.t - 1, rectParent.r - 1, rect.t);
        if (rectConnect.Width() > rect.Width() + 2) {
          return 0;
        }
        break;
      case PopupMenu::DROP_DIRECTION_TOP:
        rectConnect = IRECT(rectParent.l + 1, rect.b, rectParent.r - 1, rect.b + 1);
        if (rectConnect.Width() > rect.Width() + 2) {
          return 0;
        }
        break;
      case PopupMenu::DROP_DIRECTION_RIGHT:
        rectConnect = IRECT(rect.l - 1, rectParent.t + 1, rect.l, rectParent.b - 1);
        if (rectConnect.Height() > rect.Height() + 2) {
          return 0;
        }
        break;
      case PopupMenu::DROP_DIRECTION_LEFT:
        rectConnect = IRECT(rect.r, rectParent.t + 1, rect.r + 1, rectParent.b - 1);
        if (rectConnect.Height() > rect.Height() + 2) {
          return 0;
        }
        break;
      }
      IRECT rectBorder = rect;
      rectBorder.InflateRect(1, 1);
      rectConnect.IntersectRect(&rectConnect, &rectBorder);
      rectParent.InflateRect(1, 1);
      rectConnect.IntersectRect(&rectConnect, &rectParent);
      imdraw_FillRect(im, 0, rectConnect, s->m_clrBarBkgnd);
    }
  }
}
int VisualManagerXP_OnDrawMenuShadow(img_t* pPaintDC, const IRECT& rectClient, const IRECT& rectExclude,
    int nDepth, int iMinBrightness, int iMaxBrightness,
    CBitmap* pBmpSaveBottom, CBitmap* pBmpSaveRight, BOOL bRTL)
{
  ASSERT_VALID(pPaintDC);
  ASSERT_VALID(pBmpSaveBottom);
  ASSERT_VALID(pBmpSaveRight);
  if (rectExclude.IsRectNull()) {
    //------------------------
    // Simple draw the shadow:
    //------------------------
    DrawManager dm(*pPaintDC);
    dm.DrawShadow(rectClient, nDepth, iMinBrightness, iMaxBrightness,
        pBmpSaveBottom, pBmpSaveRight, s->m_clrMenuShadowBase,
        !bRTL);
  }
  else {
    //--------------------------------------------
    // Copy screen content into the memory bitmap:
    //--------------------------------------------
    CDC dcMem;
    if (!dcMem.CreateCompatibleDC(pPaintDC)) {
      ASSERT(FALSE);
      return 0;
    }
    //--------------------------------------------
    // Gets the whole menu and changes the shadow.
    //--------------------------------------------
    IRECT rectBmp(0, 0, rectClient.Width(), rectClient.Height());
    int cx = rectBmp.Width() + nDepth;
    int cy = rectBmp.Height() + nDepth;
    CBitmap bmpMem;
    if (!bmpMem.CreateCompatibleBitmap(pPaintDC, cx, cy)) {
      ASSERT(FALSE);
      return 0;
    }
    CBitmap* pOldBmp = dcMem.SelectObject(&bmpMem);
    ASSERT(pOldBmp != NULL);
    dcMem.BitBlt(0, 0, cx, cy, pPaintDC, rectClient.l, rectClient.t, SRCCOPY);
    //-----------------
    // Draw the shadow:
    //-----------------
    DrawManager dm(dcMem);
    dm.DrawShadow(rectBmp, nDepth, iMinBrightness, iMaxBrightness,
        pBmpSaveBottom, pBmpSaveRight, s->m_clrMenuShadowBase,
        !bRTL);
    //------------------------------------------
    // Do not cover rectExclude with the shadow:
    //------------------------------------------
    dcMem.BitBlt(rectExclude.l - rectClient.l, rectExclude.t - rectClient.t,
        rectExclude.Width(), rectExclude.Height(),
        pPaintDC, rectExclude.l, rectExclude.t, SRCCOPY);
    //-----------------------------------------
    // Copy shadowed bitmap back to the screen:
    //-----------------------------------------
    pPaintDC->BitBlt(rectClient.l, rectClient.t, cx, cy, &dcMem, 0, 0, SRCCOPY);
    dcMem.SelectObject(pOldBmp);
  }
}
int VisualManagerXP_OnDrawBarBorder(img_t* im, BaseControlBar* pBar, IRECT& rect)
{
  ASSERT_VALID(pBar);
  if (pBar->IsDialogControl()) {
    VisualManager::OnDrawBarBorder(im, pBar, rect);
    return 0;
  }
  DWORD dwBarStyle = pBar->GetBarStyle();
  if (!(dwBarStyle & CBRS_BORDER_ANY)) {
    return 0;
  }
  COLOR clrBckOld = im->GetBkColor(); // FillSolidRect changes it
  COLOR clrFill = pBar->IsOnGlass() ? _RGB(0, 0, 0) : s->clrBarFace;
  if (dwBarStyle & CBRS_BORDER_LEFT) {
    im->FillSolidRect(0, 0, 1, rect.Height() - 1, clrFill);
  }
  if (dwBarStyle & CBRS_BORDER_TOP) {
    im->FillSolidRect(0, 0, rect.Width() - 1 , 1, clrFill);
  }
  if (dwBarStyle & CBRS_BORDER_RIGHT)
    im->FillSolidRect(rect.r, 0/*RGL~:1*/, -1,
        rect.Height()/*RGL-: - 1*/, clrFill);
  if (dwBarStyle & CBRS_BORDER_BOTTOM) {
    im->FillSolidRect(0, rect.b, rect.Width() - 1, -1, clrFill);
  }
  if (dwBarStyle & CBRS_BORDER_LEFT) {
    ++rect.l;
  }
  if (dwBarStyle & CBRS_BORDER_TOP) {
    ++rect.t;
  }
  if (dwBarStyle & CBRS_BORDER_RIGHT) {
    --rect.r;
  }
  if (dwBarStyle & CBRS_BORDER_BOTTOM) {
    --rect.b;
  }
  // Restore Bk color:
  im->SetBkColor(clrBckOld);
}
int VisualManagerXP_OnFillBarBackground(img_t* im, BaseControlBar* pBar,
    IRECT rectClient, IRECT rectClip,
    BOOL /*bNCArea*/)
{
  ASSERT_VALID(pBar);
  if (pBar->IsOnGlass()) {
    im->FillSolidRect(rectClient, _RGB(0, 0, 0));
    return 0;
  }
  if (DYNAMIC_DOWNCAST(CReBar, pBar) != NULL ||
      DYNAMIC_DOWNCAST(CReBar, pBar->GetParent())) {
    FillRebarPane(im, pBar, rectClient);
    return 0;
  }
  if (rectClip.IsRectEmpty()) {
    rectClip = rectClient;
  }
  CRuntimeClass* pBarClass = pBar->GetRuntimeClass();
  if (pBarClass == NULL || pBarClass->IsDerivedFrom(RUNTIME_CLASS(MenuBar))) {
    VisualManager::OnFillBarBackground(im, pBar, rectClient, rectClip);
    return 0;
  }
  if (pBarClass->IsDerivedFrom(RUNTIME_CLASS(OutlookBarPane))) {
    OutlookBarPane* pOlBar = DYNAMIC_DOWNCAST(OutlookBarPane, pBar);
    ASSERT_VALID(pOlBar);
    if (pOlBar->IsBackgroundTexture()) {
      VisualManager::OnFillBarBackground(im, pBar, rectClient, rectClip);
      return 0;
    }
  }
  if (pBarClass->IsDerivedFrom(RUNTIME_CLASS(ColorBar))) {
    if (pBar->IsDialogControl()) {
      VisualManager::OnFillBarBackground(im, pBar, rectClient, rectClip);
    }
    else {
      imdraw_FillRect(im, 0, rectClip,
          ((ColorBar*) pBar)->IsTearOff() ?
          s->m_clrBarBkgnd : s->m_clrMenuLight);
    }
    return 0;
  }
  if (pBarClass->IsDerivedFrom(RUNTIME_CLASS(Calculator))) {
    if (pBar->IsDialogControl()) {
      VisualManager::OnFillBarBackground(im, pBar, rectClient, rectClip);
    }
    else {
      imdraw_FillRect(im, 0, rectClip, s->m_clrMenuLight);
    }
    return 0;
  }
  if (pBarClass->IsDerivedFrom(RUNTIME_CLASS(CalendarBar))) {
    imdraw_FillRect(im, 0, rectClip, s->m_clrMenuLight);
    IRECT rectImages = rectClient;
    rectImages.r = rectImages.l + ToolBar::GetMenuImageSize().w +
        2 * GetMenuImageMargin() + 1;
    return 0;
  }
  if (pBarClass->IsDerivedFrom(RUNTIME_CLASS(PopupMenuBar))) {
    imdraw_FillRect(im, 0, rectClip, s->m_clrMenuLight);
    PopupMenuBar* pMenuBar = DYNAMIC_DOWNCAST(PopupMenuBar, pBar);
    if (!pMenuBar->s->m_bDisableSideBarInXPMode) {
      IRECT rectImages = rectClient;
      rectImages.r = rectImages.l + pMenuBar->GetGutterWidth();
      rectImages.DeflateRect(0, 1);
      imdraw_FillRect(im, 0, rectImages, s->m_clrBarBkgnd);
    }
    return 0;
  }
  if (pBarClass->IsDerivedFrom(RUNTIME_CLASS(ToolBar))) {
    if (pBar->IsDialogControl()) {
      VisualManager::OnFillBarBackground(im, pBar, rectClient, rectClip);
    }
    else {
      imdraw_FillRect(im, 0, rectClip, s->m_clrBarBkgnd);
    }
    return 0;
  }
  if (pBarClass->IsDerivedFrom(RUNTIME_CLASS(AutoHideDockBar))) {
    imdraw_FillRect(im, 0, rectClip, s->m_clrTabBack);
    return 0;
  }
  VisualManager::OnFillBarBackground(im, pBar, rectClient, rectClip);
}
int VisualManagerXP_OnDrawSeparator(img_t* im, BaseControlBar* pBar,
    IRECT rect, BOOL bHorz)
{
  ASSERT_VALID(pBar);
  if (pBar->IsDialogControl()) {
    VisualManager::OnDrawSeparator(im, pBar, rect, bHorz);
    return 0;
  }
  IRECT rectSeparator = rect;
  int x1, x2;
  int y1, y2;
  if (bHorz) {
    x1 = x2 = (rect.l + rect.r) / 2;
    y1 = rect.t;
    y2 = rect.b - 1;
  }
  else {
    y1 = y2 = (rect.t + rect.b) / 2;
    x1 = rect.l;
    x2 = rect.r;
    BOOL bIsRibbon = FALSE;
#ifndef BCGP_EXCLUDE_RIBBON
    bIsRibbon = pBar->IsKindOf(RUNTIME_CLASS(RibbonPanelMenuBar));
    if (bIsRibbon && ((RibbonPanelMenuBar*) pBar)->IsDefaultMenuLook()) {
      bIsRibbon = FALSE;
    }
#endif
    if (pBar->IsKindOf(RUNTIME_CLASS(PopupMenuBar)) &&
        !bIsRibbon &&
        !pBar->IsKindOf(RUNTIME_CLASS(ColorBar)) &&
        !pBar->IsKindOf(RUNTIME_CLASS(Calculator)) &&
        !pBar->IsKindOf(RUNTIME_CLASS(CalendarBar))) {
      x1 = rect.l + ToolBar::GetMenuImageSize().w +
          GetMenuImageMargin() + 1;
      IRECT rectBar;
      pBar->GetClientRect(rectBar);
      if (rectBar.r - x2 < 50) { // Last item in row
        x2 = rectBar.r;
      }
      if (((PopupMenuBar*) pBar)->s->m_bDisableSideBarInXPMode) {
        x1 = 0;
      }
      //---------------------------------
      // Maybe Quick Customize separator
      //---------------------------------
      if (pBar->IsKindOf(RUNTIME_CLASS(PopupMenuBar))) {
        CWnd* pWnd = pBar->GetParent();
        if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(PopupMenu))) {
          PopupMenu* pMenu = (PopupMenu*)pWnd;
          if (pMenu->IsCustomizePane()) {
            x1 = rect.l + 2 * ToolBar::GetMenuImageSize().w +
                3 * GetMenuImageMargin() + 2;
          }
        }
      }
    }
  }
  if (ToolBarImages::s->m_bIsDrawOnGlass) {
    DrawManager dm(*im);
    dm.DrawLine(x1, y1, x2, y2, s->m_clrSeparator);
  }
  else {
    CPen* pOldPen = im->SelectObject(&s->m_penSeparator);
    ASSERT(pOldPen != NULL);
    im->MoveTo(x1, y1);
    im->LineTo(x2, y2);
    im->SelectObject(pOldPen);
  }
}
int VisualManagerXP_OnDrawButtonBorder(img_t* im,
    ToolbarButton* pButton, IRECT rect, BCGBUTTON_STATE state)
{
  if (state != ButtonsIsPressed && state != ButtonsIsHighlighted) {
    ASSERT((pButton->s->m_nStyle & TBBS_CHECKED) == 0);
    return 0;
  }
  COLOR clrBorder = s->m_clrMenuItemBorder;
  ToolbarMenuButton* pMenuButton =
      DYNAMIC_DOWNCAST(ToolbarMenuButton, pButton);
  BOOL bIsMenuButton = pMenuButton != NULL;
  BOOL bIsPopupMenu = bIsMenuButton &&
      pMenuButton->GetParentWnd() != NULL &&
      pMenuButton->GetParentWnd()->IsKindOf(RUNTIME_CLASS(PopupMenuBar));
  BOOL bIsPressedBorder = !bIsPopupMenu;
  if (bIsMenuButton && !bIsPopupMenu &&
      pMenuButton->IsDroppedDown()) {
    bIsPressedBorder = FALSE;
    PopupMenu* pPopupMenu = pMenuButton->GetPopupMenu();
    if (pPopupMenu != NULL &&
        (pPopupMenu->IsWindowVisible() || pPopupMenu->IsShown())) {
      clrBorder = s->m_clrMenuBorder;
      ExtendMenuButton(pMenuButton, rect);
      BOOL bRTL = pPopupMenu->GetExStyle() & WS_EX_LAYOUTRTL;
      if (s->m_bShdowDroppedDownMenuButton && !bRTL &&
          MenuBar::IsMenuShadows() &&
          !ToolBar::IsCustomizeMode() &&
          s->m_nBitsPerPixel > 8 &&
          !IsHighContastMode(s) &&
          !pPopupMenu->IsRightAlign()) {
        DrawManager dm(*im);
        dm.DrawShadow(rect, s->m_nMenuShadowDepth, 100, 75, NULL, NULL,
            s->m_clrMenuShadowBase);
      }
    }
  }
  const BOOL bIsChecked = (pButton->s->m_nStyle & TBBS_CHECKED);
  switch (state) {
  case ButtonsIsPressed:
    if (bIsPressedBorder && s->m_clrPressedButtonBorder != (COLOR) - 1 &&
        !bIsChecked &&
        rect.Width() > 5 && rect.Height() > 5) {
      clrBorder = s->m_clrPressedButtonBorder;
    }
  case ButtonsIsHighlighted:
    if (bIsPopupMenu && bIsChecked) {
      if (pButton->s->m_nStyle & TBBS_MARKED) {
        clrBorder = s->m_clrPressedButtonBorder;
      }
      rect.b ++;
    }
    if (ToolBarImages::s->m_bIsDrawOnGlass) {
      DrawManager dm(*im);
      dm.DrawRect(rect, (COLOR) - 1, clrBorder);
    }
    else {
      imdraw_Draw3dRect(im, 0, rect, clrBorder, clrBorder);
    }
  }
}
int VisualManagerXP_OnFillButtonInterior(img_t* im,
    ToolbarButton* pButton, IRECT rect, VisualManager::BCGBUTTON_STATE state)
{
  if (state != ButtonsIsPressed && state != ButtonsIsHighlighted) {
    return 0;
  }
  if (ToolBar::IsCustomizeMode() &&
      !ToolBar::IsAltCustomizeMode() && !pButton->IsLocked()) {
    return 0;
  }
  ToolbarMenuButton* pMenuButton =
      DYNAMIC_DOWNCAST(ToolbarMenuButton, pButton);
  BOOL bIsMenuButton = pMenuButton != NULL;
  BOOL bIsPopupMenu = bIsMenuButton &&
      pMenuButton->GetParentWnd() != NULL &&
      pMenuButton->GetParentWnd()->IsKindOf(RUNTIME_CLASS(PopupMenuBar));
  if (!bIsPopupMenu && !s->m_bEnableToolbarButtonFill) {
    return 0;
  }
  COLOR pBrush = ((pButton->s->m_nStyle & TBBS_PRESSED) && !bIsPopupMenu) ?
      s->m_clrHighlightDn : s->m_clrHighlight;
  if (bIsMenuButton && !bIsPopupMenu && pMenuButton->IsDroppedDown()) {
    ExtendMenuButton(pMenuButton, rect);
    pBrush = s->m_clrBarBkgnd;
  }
  if (pButton->s->m_nStyle & TBBS_CHECKED) {
    pBrush = (state == ButtonsIsHighlighted) ?
        s->m_clrHighlightDn : s->m_clrHighlightChecked;
  }
  if (bIsMenuButton && (pButton->s->m_nStyle & TBBS_DISABLED)) {
    pBrush = s->m_clrMenuLight;
  }
  switch (state) {
  case ButtonsIsPressed:
  case ButtonsIsHighlighted:
    if ((pButton->s->m_nStyle & TBBS_CHECKED) == 0) {
      rect.DeflateRect(1, 1);
    }
    OnFillHighlightedArea(im, rect, pBrush, pButton);
  }
}
int VisualManagerXP_OnHighlightMenuItem(img_t* im, ToolbarMenuButton* pButton,
    IRECT rect, COLOR& clrText)
{
  COLOR pBrush = (pButton->s->m_nStyle & TBBS_DISABLED) ?
      s->m_clrMenuLight : s->m_clrHighlight;
  rect.DeflateRect(1, 0);
  OnFillHighlightedArea(im, rect, pBrush, pButton);
  imdraw_Draw3dRect(im, 0, rect, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
  clrText = GetHighlightedMenuItemTextColor(pButton);
}
COLOR VisualManagerXP_GetHighlightedMenuItemTextColor(ToolbarMenuButton* pButton)
{
  if (pButton->s->m_nStyle & TBBS_DISABLED) {
    return s->clrGrayedText;
  }
  if (GetRV(s->m_clrHighlight) > 128 &&
      GetGV(s->m_clrHighlight) > 128 &&
      GetBV(s->m_clrHighlight) > 128) {
    return _RGB(0, 0, 0);
  }
  else {
    return _RGB(255, 255, 255);
  }
}
int VisualManagerXP_OnHighlightQuickCustomizeMenuButton(img_t* im,
    ToolbarMenuButton* /*pButton*/, IRECT rect)
{
  imdraw_FillRect(im, 0, rect, s->m_clrBarBkgnd);
  imdraw_Draw3dRect(im, 0, rect, s->m_clrMenuBorder, s->m_clrMenuBorder);
}
int VisualManagerXP_OnHighlightRarelyUsedMenuItems(img_t* im, IRECT rectRarelyUsed)
{
  rectRarelyUsed.l --;
  rectRarelyUsed.r = rectRarelyUsed.l + ToolBar::GetMenuImageSize().w +
      2 * GetMenuImageMargin() + 2;
  imdraw_FillRect(im, 0, rectRarelyUsed, s->m_clrMenuRarelyUsed);
}
int VisualManagerXP_OnDrawTab(img_t* im, IRECT rectTab,
    int iTab, BOOL bIsActive, const BaseTabWnd* pTabWnd)
{
#define TEXT_MARGIN 4
#define IMAGE_MARGIN 4
  ASSERT_VALID(pTabWnd);
  if (pTabWnd->IsFlatTab() || pTabWnd->IsOneNoteStyle() ||
      pTabWnd->IsColored() || pTabWnd->IsVS2005Style() ||
      pTabWnd->IsLeftRightRounded()) {
    VisualManager::OnDrawTab(im, rectTab, iTab, bIsActive, pTabWnd);
    return 0;
  }
  COLOR clrDark;
  COLOR clrBlack;
  COLOR clrHighlight;
  COLOR clrFace;
  COLOR clrDarkShadow;
  COLOR clrLight;
  COLOR pbrFace = NULL;
  COLOR pbrBlack = NULL;
  GetTabFrameColors(
      pTabWnd, clrDark, clrBlack, clrHighlight, clrFace, clrDarkShadow, clrLight,
      pbrFace, pbrBlack);
  CPen penGray(PS_SOLID, 1, clrDark);
  CPen penDkGray(PS_SOLID, 1, clrBlack);
  CPen penHiLight(PS_SOLID, 1, clrHighlight);
  CPen* pOldPen = im->SelectObject(&penGray);
  ASSERT(pOldPen != NULL);
  if (iTab != pTabWnd->GetActiveTab() - 1) {
    if (iTab < pTabWnd->GetVisibleTabsNum() - 1 || s->m_bDrawLastTabLine) {
      im->MoveTo(rectTab.r, rectTab.t + 3);
      im->LineTo(rectTab.r, rectTab.b - 3);
    }
  }
  if (bIsActive) {
    if (pTabWnd->GetLocation() == BaseTabWnd::LOCATION_BOTTOM) {
      IRECT rectFace = rectTab;
      rectFace.t--;
      OnFillTab(im, rectFace, pbrFace, iTab, bIsActive, pTabWnd);
      im->SelectObject(&penDkGray);
      im->MoveTo(rectTab.r, rectTab.t);
      im->LineTo(rectTab.r, rectTab.b);
      im->LineTo(rectTab.l, rectTab.b);
      im->SelectObject(&penHiLight);
      im->LineTo(rectTab.l, rectTab.t - 2);
    }
    else {
      CPen penLight(PS_SOLID, 1, s->m_clrMenuLight);
      IRECT rectFace = rectTab;
      rectFace.b++;
      rectFace.l++;
      OnFillTab(im, rectFace, pbrFace, iTab, bIsActive, pTabWnd);
      im->SelectObject(&penDkGray);
      im->MoveTo(rectTab.r, rectTab.b);
      im->LineTo(rectTab.r, rectTab.t);
      im->SelectObject(&penHiLight);
      im->LineTo(rectTab.r, rectTab.t);
      im->LineTo(rectTab.l, rectTab.t);
      im->LineTo(rectTab.l, rectTab.b);
    }
  }
  im->SelectObject(pOldPen);
  COLOR clrText;
  if (pTabWnd->IsDialogControl()) {
    clrText = s->clrBtnText;
  }
  else {
    clrText = bIsActive ? s->clrBarText : s->m_clrInactiveTabText;
  }
  OnDrawTabContent(im, rectTab, iTab, bIsActive, pTabWnd, clrText);
}
int VisualManagerXP_OnFillTab(img_t* im, IRECT rectFill, COLOR pbrFill,
    int iTab, BOOL bIsActive,
    const BaseTabWnd* pTabWnd)
{
  ASSERT_VALID(pbrFill);
  ASSERT_VALID(pTabWnd);
  if (pTabWnd->GetTabBkColor(iTab) != (COLOR) - 1 && !bIsActive) {
    CBrush br(pTabWnd->GetTabBkColor(iTab));
    imdraw_FillRect(im, 0, rectFill, &br);
    return 0;
  }
  if (pTabWnd->IsOneNoteStyle() || pTabWnd->IsVS2005Style() ||
      pTabWnd->IsLeftRightRounded()) {
    VisualManager::OnFillTab(im, rectFill, pbrFill,
        iTab, bIsActive, pTabWnd);
  }
  else if (bIsActive) {
    imdraw_FillRect(im, 0, rectFill, pbrFill);
  }
}
int VisualManagerXP_OnEraseTabsArea(img_t* im, IRECT rect,
    const BaseTabWnd* pTabWnd)
{
  ASSERT_VALID(pTabWnd);
  if (pTabWnd->IsFlatTab()) {
    VisualManager::OnEraseTabsArea(im, rect, pTabWnd);
    return 0;
  }
  if (pTabWnd->IsDialogControl()) {
    if (pTabWnd->IsVisualManagerStyle()) {
      OnFillDialog(im, pTabWnd->GetParent(), rect);
    }
    else {
      imdraw_FillRect(im, 0, rect, &s->clrBtnFace);
    }
    return 0;
  }
  imdraw_FillRect(im, 0, rect, s->m_clrTabBack);
}
COLOR VisualManagerXP_OnDrawControlBarCaption(img_t* im, DockingControlBar* /*pBar*/,
    BOOL bActive, IRECT rectCaption, IRECT /*rectButtons*/)
{
  CPen pen(PS_SOLID, 1,
      bActive ? s->clrBarLight : s->clrBarShadow);
  CPen* pOldPen = im->SelectObject(&pen);
  COLOR pOldBrush = (COLOR) im->SelectObject(
      bActive ? &s->clrActiveCaption : &s->clrBarFace);
  if (bActive) {
    rectCaption.InflateRect(1, 1);
  }
  im->RoundRect(rectCaption, CPoint(2, 2));
  im->SelectObject(pOldBrush);
  im->SelectObject(pOldPen);
  // get the text color
  COLOR clrCptnText = bActive ?
      s->clrCaptionText :
      s->clrBarText;
  return clrCptnText;
}
int VisualManagerXP_OnDrawCaptionButton(img_t* im, CaptionButton* pButton,
    BOOL bActive,
    BOOL bHorz, BOOL bMaximized, BOOL bDisabled,
    int nImageID /*= -1*/)
{
  ASSERT(pButton != NULL);
  IRECT rc = pButton->GetRect();
  if (pButton->s->m_bPushed && (pButton->s->m_bFocused || pButton->s->m_bDroppedDown) && !bDisabled) {
    OnFillHighlightedArea(im, rc, s->m_clrHighlightDn, NULL);
    bActive = TRUE;
  }
  else if (pButton->s->m_bPushed || pButton->s->m_bFocused || pButton->s->m_bDroppedDown) {
    if (!bDisabled) {
      OnFillHighlightedArea(im, rc, s->m_clrHighlight, NULL);
    }
    bActive = FALSE;
  }
  MenuImages::IMAGES_IDS id = (MenuImages::IMAGES_IDS) - 1;
  if (nImageID != -1) {
    id = (MenuImages::IMAGES_IDS)nImageID;
  }
  else {
    id = pButton->GetIconID(bHorz, bMaximized);
  }
  if (id != (MenuImages::IMAGES_IDS) - 1) {
    CSize sizeImage = MenuImages::Size();
    CPoint ptImage(rc.l + (rc.Width() - sizeImage.w) / 2,
        rc.t + (rc.Height() - sizeImage.h) / 2);
    OnDrawCaptionButtonIcon(im, pButton, id, bActive, bDisabled, ptImage);
  }
  if ((pButton->s->m_bPushed || pButton->s->m_bFocused || pButton->s->m_bDroppedDown) && !bDisabled) {
    COLOR clrDark = s->clrBarDkShadow;
    imdraw_Draw3dRect(im, 0, rc, clrDark, clrDark);
  }
}
int VisualManagerXP_OnDrawCaptionButtonIcon(img_t* im,
    CaptionButton* pButton,
    MenuImages::IMAGES_IDS id,
    BOOL bActive, BOOL bDisabled,
    CPoint ptImage)
{
  MenuImages::IMAGE_STATE imageState;
  if (bDisabled) {
    imageState = MenuImages::ImageGray;
  }
  else if (pButton->s->m_bFocused || pButton->s->m_bPushed) {
    COLOR clrBack = pButton->s->m_bPushed ? s->m_clrHighlightDn : s->m_clrHighlight;
    if (GetRV(clrBack) <= 192 &&
        GetGV(clrBack) <= 192 &&
        GetBV(clrBack) <= 192) {
      imageState = MenuImages::ImageWhite;
    }
    else {
      imageState = MenuImages::ImageBlack;
    }
  }
  else if (pButton->s->m_clrForeground == (COLOR) - 1) {
    imageState = bActive ? MenuImages::ImageWhite : MenuImages::ImageBlack;
  }
  else {
    if (GetRV(pButton->s->m_clrForeground) > 192 &&
        GetGV(pButton->s->m_clrForeground) > 192 &&
        GetBV(pButton->s->m_clrForeground) > 192) {
      imageState = MenuImages::ImageWhite;
    }
    else {
      imageState = MenuImages::ImageBlack;
    }
  }
  MenuImages::Draw(im, id, ptImage, imageState);
}
COLOR VisualManagerXP_OnFillCommandsListBackground(img_t* im, IRECT rect, BOOL bIsSelected)
{
  ASSERT_VALID(this);
  COLOR clrText = s->clrBarText;
  int iImageWidth = ToolBar::GetMenuImageSize().w + GetMenuImageMargin();
  if (bIsSelected) {
    if (s->m_bEnableToolbarButtonFill) {
      rect.l = 0;
    }
    OnFillHighlightedArea(im, rect, s->m_clrHighlight, NULL);
    imdraw_Draw3dRect(im, 0, rect, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
    // Now, we should define a menu text color...
    if (GetRV(s->m_clrHighlight) > 128 &&
        GetGV(s->m_clrHighlight) > 128 &&
        GetBV(s->m_clrHighlight) > 128) {
      clrText = _RGB(0, 0, 0);
    }
    else {
      clrText = _RGB(255, 255, 255);
    }
  }
  else {
    imdraw_FillRect(im, 0, rect, s->m_clrMenuLight);
    IRECT rectImages = rect;
    rectImages.r = rectImages.l + iImageWidth + MENU_IMAGE_MARGIN;
    imdraw_FillRect(im, 0, rectImages, s->m_clrBarBkgnd);
    clrText = s->clrBarText;
  }
  return clrText;
}
int VisualManagerXP_OnDrawMenuArrowOnCustomizeList(img_t* im,
    IRECT rectCommand, BOOL /*bSelected*/)
{
  IRECT rectTriangle = rectCommand;
  rectTriangle.l = rectTriangle.r - MenuImages::Size().w;
  MenuImages::IMAGE_STATE state = MenuImages::ImageBlack;
  COLOR clrPen = _RGB(0, 0, 0);
  if (GetRV(s->m_clrHighlight) < 128 ||
      GetGV(s->m_clrHighlight) < 128 ||
      GetBV(s->m_clrHighlight) < 128) {
    state = MenuImages::ImageWhite;
    clrPen = _RGB(255, 255, 255);
  }
  MenuImages::Draw(im, MenuImages::IdArowRightLarge, rectTriangle, state);
  CPen penLine(PS_SOLID, 1, clrPen);
  CPen* pOldPen = im->SelectObject(&penLine);
  ASSERT(pOldPen != NULL);
  im->MoveTo(rectTriangle.l - 1, rectCommand.t + 2);
  im->LineTo(rectTriangle.l - 1, rectCommand.b - 2);
  im->SelectObject(pOldPen);
}
int VisualManagerXP_OnDrawTearOffCaption(img_t* im, IRECT rect, BOOL bIsActive)
{
  const int iBorderSize = 1;
  imdraw_FillRect(im, 0, rect, s->m_clrMenuLight);
  rect.DeflateRect(iBorderSize, iBorderSize);
  OnFillHighlightedArea(im, rect, bIsActive ? s->m_clrHighlight : s->m_clrBarBkgnd,
      NULL);
  // Draw gripper:
  int nGripperWidth = max(20, ToolBar::GetMenuImageSize().w * 2);
  IRECT rectGripper = rect;
  rectGripper.DeflateRect((rectGripper.Width() - nGripperWidth) / 2, 1);
  if (s->m_clrGripperHorz.GetSafeHandle() == NULL) {
    CreateGripperBrush();
  }
  COLOR clrTextOld = im->SetTextColor(bIsActive ?
      s->clrBarDkShadow : s->clrBarShadow);
  COLOR clrBkOld = im->SetBkColor(
      bIsActive ? s->m_clrHighlight : s->m_clrBarBkgnd);
  if (bIsActive) {
    rectGripper.DeflateRect(0, 1);
  }
  imdraw_FillRect(im, 0, rectGripper, s->m_clrGripperHorz);
  im->SetTextColor(clrTextOld);
  im->SetBkColor(clrBkOld);
  if (bIsActive) {
    imdraw_Draw3dRect(im, 0, rect, s->clrBarDkShadow, s->clrBarDkShadow);
  }
}
int VisualManagerXP_OnDrawMenuResizeBar(img_t* im, IRECT rect, int nResizeFlags)
{
  const int iBorderSize = 1;
  imdraw_FillRect(im, 0, rect, s->m_clrMenuLight);
  rect.DeflateRect(iBorderSize, iBorderSize);
  OnFillHighlightedArea(im, rect, s->m_clrBarBkgnd, NULL);
  IRECT rectGripper = rect;
  if (nResizeFlags == (int) PopupMenu::MENU_RESIZE_BOTTOs->m_RIGHT ||
      nResizeFlags == (int) PopupMenu::MENU_RESIZE_TOP_RIGHT) {
    rectGripper.l = rectGripper.r - rectGripper.Height();
  }
  else {
    rectGripper.l = rectGripper.CenterPoint().x - rectGripper.Height() / 2;
    rectGripper.r = rectGripper.l + rectGripper.Height();
  }
  rectGripper.DeflateRect(2, 2);
  if (s->m_clrGripperHorz.GetSafeHandle() == NULL) {
    CreateGripperBrush();
  }
  COLOR clrTextOld = im->SetTextColor(s->clrBarShadow);
  COLOR clrBkOld = im->SetBkColor(s->m_clrBarBkgnd);
  imdraw_FillRect(im, 0, rectGripper, s->m_clrGripperHorz);
  im->SetTextColor(clrTextOld);
  im->SetBkColor(clrBkOld);
}
int VisualManagerXP_OnDrawMenuScrollButton(img_t* im, IRECT rect, BOOL bIsScrollDown,
    BOOL bIsHighlited, BOOL /*bIsPressed*/,
    BOOL /*bIsDisabled*/)
{
  rect.t --;
  imdraw_FillRect(im, 0, rect, &s->clrBarFace);
  MenuImages::Draw(im, bIsScrollDown ? MenuImages::IdArowDown : MenuImages::IdArowUp, rect);
  if (bIsHighlited) {
    CPen pen(PS_SOLID, 1, s->clrBarShadow);
    CPen* pOldPen = im->SelectObject(&pen);
    COLOR pOldBrush = (COLOR) im->SelectStockObject(NULL_BRUSH);
    rect.DeflateRect(1, 1);
    im->RoundRect(rect, CPoint(2, 2));
    im->SelectObject(pOldBrush);
    im->SelectObject(pOldPen);
  }
}
int VisualManagerXP_CreateGripperBrush()
{
  ASSERT(s->m_clrGripperHorz.GetSafeHandle() == NULL);
  ASSERT(s->m_clrGripperVert.GetSafeHandle() == NULL);
  WORD horzHatchBits [8] = { 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00 };
  CBitmap bmpGripperHorz;
  bmpGripperHorz.CreateBitmap(8, 8, 1, 1, horzHatchBits);
  s->m_clrGripperHorz.CreatePatternBrush(&bmpGripperHorz);
  WORD vertHatchBits[8] = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };
  CBitmap bmpGripperVert;
  bmpGripperVert.CreateBitmap(8, 8, 1, 1, vertHatchBits);
  s->m_clrGripperVert.CreatePatternBrush(&bmpGripperVert);
}
int VisualManagerXP_ExtendMenuButton(ToolbarMenuButton* pMenuButton,
    IRECT& rect)
{
  ASSERT_VALID(pMenuButton);
  PopupMenu* pPopupMenu = pMenuButton->GetPopupMenu();
  if (pPopupMenu == NULL || pPopupMenu->GetSafeHwnd() == NULL) {
    return 0;
  }
  IRECT rectMenu;
  pPopupMenu->GetWindowRect(rectMenu);
  if (DYNAMIC_DOWNCAST(CCustomizeButton, pMenuButton) != NULL) {
    BaseControlBar* pParentBar = DYNAMIC_DOWNCAST(
        BaseControlBar, pMenuButton->GetParentWnd());
    if (pParentBar != NULL) {
      IRECT rectScreen = rect;
      pParentBar->ClientToScreen(&rectScreen);
      if (pParentBar->IsHorizontal()) {
        rectScreen.t = rectMenu.t;
        rectScreen.b = rectMenu.b;
      }
      else {
        rectScreen.l = rectMenu.l;
        rectScreen.r = rectMenu.r;
        rectScreen.b++;
      }
      IRECT rectInter;
      if (!rectInter.IntersectRect(rectScreen, rectMenu)) {
        return 0;
      }
    }
  }
  int nGrow = 4;
  switch (pPopupMenu->GetDropDirection()) {
  case PopupMenu::DROP_DIRECTION_BOTTOM:
    if (rectMenu.Width() < rect.Width()) {
      nGrow = 1;
    }
    rect.b += nGrow;
    break;
  case PopupMenu::DROP_DIRECTION_TOP:
    if (rectMenu.Width() < rect.Width()) {
      nGrow = 1;
    }
    rect.t -= nGrow;
    break;
  case PopupMenu::DROP_DIRECTION_RIGHT:
    if (rectMenu.Height() < rect.Height()) {
      nGrow = 1;
    }
    rect.r += nGrow;
    break;
  case PopupMenu::DROP_DIRECTION_LEFT:
    if (rectMenu.Height() < rect.Height()) {
      nGrow = 1;
    }
    rect.l -= nGrow;
    break;
  }
}
int VisualManagerXP_OnDrawMenuSystemButton(img_t* im, IRECT rect, UINT uiSystemCommand,
    UINT nStyle, BOOL bHighlight)
{
  BOOL bIsDisabled = (nStyle & TBBS_DISABLED);
  BOOL bIsPressed = (nStyle & TBBS_PRESSED);
  MenuImages::IMAGES_IDS imageID;
  switch (uiSystemCommand) {
  case SC_CLOSE:
    imageID = MenuImages::IdClose;
    break;
  case SC_MINIMIZE:
    imageID = MenuImages::IdMinimize;
    break;
  case SC_RESTORE:
    imageID = MenuImages::IdRestore;
    break;
  default:
    return 0;
  }
  if (bHighlight && !bIsDisabled) {
    OnFillHighlightedArea(im, rect,
        bIsPressed ? s->m_clrHighlightDn : s->m_clrHighlight, NULL);
    COLOR clrBorder = s->m_clrMenuItemBorder;
    imdraw_Draw3dRect(im, 0, rect, clrBorder, clrBorder);
  }
  MenuImages::Draw(im, imageID, rect,
      bIsDisabled ? MenuImages::ImageGray : bHighlight ?
      MenuImages::ImageWhite : MenuImages::ImageBlack);
}
int VisualManagerXP_OnDrawStatusBarPaneBorder(img_t* im, StatusBar* /*pBar*/,
    IRECT rectPane, UINT /*uiID*/, UINT nStyle)
{
  if (!(nStyle & SBPS_NOBORDERS)) {
    if (nStyle & SBPS_POPOUT) {
      DrawManager dm(*im);
      dm.HighlightRect(rectPane);
    }
    // Draw pane border:
    imdraw_Draw3dRect(im, 0, rectPane, s->m_clrPaneBorder, s->m_clrPaneBorder);
  }
}
int VisualManagerXP_OnDrawComboDropButton(img_t* im, IRECT rect,
    BOOL bDisabled,
    BOOL bIsDropped,
    BOOL bIsHighlighted,
    ToolbarComboBoxButton* /*pButton*/)
{
  ASSERT_VALID(this);
  COLOR clrText = im->GetTextColor();
  if (bIsDropped || bIsHighlighted) {
    OnFillHighlightedArea(im, rect,
        bIsDropped ? s->m_clrHighlightDn : s->m_clrHighlight,
        NULL);
    if (ToolBarImages::s->m_bIsDrawOnGlass) {
      DrawManager dm(*im);
      dm.DrawLine(rect.l, rect.t, rect.l, rect.b, s->m_clrMenuItemBorder);
    }
    else {
      CPen* pOldPen = im->SelectObject(&s->m_penMenuItemBorder);
      ASSERT(pOldPen != NULL);
      im->MoveTo(rect.l, rect.t);
      im->LineTo(rect.l, rect.b);
      im->SelectObject(pOldPen);
    }
  }
  else {
    imdraw_FillRect(im, 0, rect, &s->clrBarFace);
    if (ToolBarImages::s->m_bIsDrawOnGlass) {
      DrawManager dm(*im);
      dm.DrawRect(rect, (COLOR) - 1, s->clrWindow);
    }
    else {
      imdraw_Draw3dRect(im, 0, rect, s->clrBarWindow, s->clrBarWindow);
    }
  }
  MenuImages::Draw(im, MenuImages::IdArowDown, rect,
      bDisabled ? MenuImages::ImageGray : (bIsDropped && bIsHighlighted) ?
      MenuImages::ImageWhite : MenuImages::ImageBlack);
  im->SetTextColor(clrText);
}
int VisualManagerXP_OnDrawComboBorder(img_t* im, IRECT rect,
    BOOL /*bDisabled*/,
    BOOL bIsDropped,
    BOOL bIsHighlighted,
    ToolbarComboBoxButton* /*pButton*/)
{
  if (bIsHighlighted || bIsDropped) {
    rect.DeflateRect(1, 1);
    if (ToolBarImages::s->m_bIsDrawOnGlass) {
      DrawManager dm(*im);
      dm.DrawRect(rect, (COLOR) - 1, s->m_clrMenuItemBorder);
    }
    else {
      imdraw_Draw3dRect(im, 0, &rect, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
    }
  }
}
int VisualManagerXP_OnDrawTabCloseButton(img_t* im, IRECT rect,
    const BaseTabWnd* /*pTabWnd*/,
    BOOL bIsHighlighted,
    BOOL bIsPressed,
    BOOL /*bIsDisabled*/)
{
  if (bIsHighlighted) {
    OnFillHighlightedArea(im, rect, bIsPressed ? s->m_clrHighlightDn : s->m_clrHighlight, NULL);
  }
  MenuImages::Draw(im, MenuImages::IdClose, rect, MenuImages::ImageBlack);
  if (bIsHighlighted) {
    imdraw_Draw3dRect(im, 0, rect, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
  }
}
int VisualManagerXP_OnEraseTabsButton(img_t* im, IRECT rect,
    Button* pButton,
    BaseTabWnd* pWndTab)
{
  ASSERT_VALID(pWndTab);
  if (pWndTab->IsFlatTab()) {
    COLOR pBrush = pButton->IsPressed() ?
        s->m_clrHighlightDn : pButton->IsHighlighted() ? s->m_clrHighlight : &s->clrBarFace;
    imdraw_FillRect(im, 0, rect, pBrush);
    OnFillHighlightedArea(im, rect, pBrush, NULL);
  }
  else if (pWndTab->IsDialogControl()) {
    if (pWndTab->IsVisualManagerStyle()) {
      OnFillDialog(im, pWndTab->GetParent(), rect);
    }
    else {
      imdraw_FillRect(im, 0, rect, &s->clrBtnFace);
    }
  }
  else {
    imdraw_FillRect(im, 0, rect, s->m_clrTabBack);
  }
}
int VisualManagerXP_OnDrawTabsButtonBorder(img_t* im, IRECT& rect,
    Button* pButton, UINT /*uiState*/,
    BaseTabWnd* pWndTab)
{
  ASSERT_VALID(pWndTab);
  if (pWndTab->IsFlatTab()) {
    if (pButton->IsPushed() || pButton->IsHighlighted()) {
      COLOR clrDark = s->clrBarDkShadow;
      imdraw_Draw3dRect(im, 0, rect, clrDark, clrDark);
    }
  }
  else {
    if (pButton->IsPushed() || pButton->IsHighlighted()) {
      if (pButton->IsPressed()) {
        imdraw_Draw3dRect(im, 0, rect, s->clrBarDkShadow, s->m_clrGripper);
      }
      else {
        imdraw_Draw3dRect(im, 0, rect, s->m_clrGripper, s->clrBarDkShadow);
      }
    }
  }
}
COLOR VisualManagerXP_OnFillMiniFrameCaption(img_t* im,
    IRECT rectCaption,
    MiniFrameWnd* pFrameWnd,
    BOOL bActive)
{
  ASSERT_VALID(pFrameWnd);
  BOOL bIsToolBar = FALSE;
#ifndef BCGP_EXCLUDE_TASK_PANE
  BOOL bIsTasksPane = pFrameWnd->IsKindOf(RUNTIME_CLASS(TaskPaneMiniFrameWnd));
#else
  BOOL bIsTasksPane = FALSE;
#endif
  if (DYNAMIC_DOWNCAST(BaseToolBar, pFrameWnd->GetControlBar()) != NULL) {
    bActive = FALSE;
    bIsToolBar = TRUE;
  }
  if (bIsToolBar) {
    imdraw_FillRect(im, 0, rectCaption, s->m_clrFloatToolBarBorder);
    return s->clrCaptionText;
  }
  else if (bIsTasksPane) {
    imdraw_FillRect(im, 0, rectCaption, &s->clrBarFace);
    return s->clrBarText;
  }
  imdraw_FillRect(im, 0, rectCaption,
      bActive ? &s->clrActiveCaption : &s->clrInactiveCaption);
  // get the text color
  return s->clrCaptionText;
}
int VisualManagerXP_OnDrawMiniFrameBorder(
    img_t* im, MiniFrameWnd* pFrameWnd,
    IRECT rectBorder, IRECT rectBorderSize)
{
  ASSERT_VALID(pFrameWnd);
#ifndef BCGP_EXCLUDE_TASK_PANE
  BOOL bIsTasksPane = pFrameWnd->IsKindOf(RUNTIME_CLASS(TaskPaneMiniFrameWnd));
#else
  BOOL bIsTasksPane = FALSE;
#endif
  if (bIsTasksPane) {
    COLOR pOldBrush = im->SelectObject(s->m_clrFloatToolBarBorder);
    ASSERT(pOldBrush != NULL);
    im->PatBlt(rectBorder.l, rectBorder.t, rectBorderSize.l, rectBorder.Height(), PATCOPY);
    im->PatBlt(rectBorder.l, rectBorder.t, rectBorder.Width(), rectBorderSize.t, PATCOPY);
    im->PatBlt(rectBorder.r - rectBorderSize.r, rectBorder.t, rectBorderSize.r, rectBorder.Height(), PATCOPY);
    im->PatBlt(rectBorder.l, rectBorder.b - rectBorderSize.b, rectBorder.Width(), rectBorderSize.b, PATCOPY);
    rectBorderSize.DeflateRect(2, 2);
    rectBorder.DeflateRect(2, 2);
    im->SelectObject(bIsTasksPane ? &s->clrLight : &s->clrBarFace);
    im->PatBlt(rectBorder.l, rectBorder.t + 1, rectBorderSize.l, rectBorder.Height() - 2, PATCOPY);
    im->PatBlt(rectBorder.l + 1, rectBorder.t, rectBorder.Width() - 2, rectBorderSize.t, PATCOPY);
    im->PatBlt(rectBorder.r - rectBorderSize.r, rectBorder.t + 1, rectBorderSize.r, rectBorder.Height() - 2, PATCOPY);
    im->PatBlt(rectBorder.l + 1, rectBorder.b - rectBorderSize.b, rectBorder.Width() - 2, rectBorderSize.b, PATCOPY);
    im->SelectObject(pOldBrush);
  }
  else {
    VisualManager::OnDrawMiniFrameBorder(im, pFrameWnd, rectBorder, rectBorderSize);
  }
}
int VisualManagerXP_OnDrawFloatingToolbarBorder(
    img_t* im, BaseToolBar* /*pToolBar*/,
    IRECT rectBorder, IRECT rectBorderSize)
{
  COLOR pOldBrush = im->SelectObject(s->m_clrFloatToolBarBorder);
  ASSERT(pOldBrush != NULL);
  im->PatBlt(rectBorder.l, rectBorder.t, rectBorderSize.l, rectBorder.Height(), PATCOPY);
  im->PatBlt(rectBorder.l, rectBorder.t, rectBorder.Width(), rectBorderSize.t, PATCOPY);
  im->PatBlt(rectBorder.r - rectBorderSize.r, rectBorder.t, rectBorderSize.r, rectBorder.Height(), PATCOPY);
  im->PatBlt(rectBorder.l, rectBorder.b - rectBorderSize.b, rectBorder.Width(), rectBorderSize.b, PATCOPY);
  rectBorderSize.DeflateRect(2, 2);
  rectBorder.DeflateRect(2, 2);
  im->SelectObject(&s->clrBarFace);
  im->PatBlt(rectBorder.l, rectBorder.t + 1, rectBorderSize.l, rectBorder.Height() - 2, PATCOPY);
  im->PatBlt(rectBorder.l + 1, rectBorder.t, rectBorder.Width() - 2, rectBorderSize.t, PATCOPY);
  im->PatBlt(rectBorder.r - rectBorderSize.r, rectBorder.t + 1, rectBorderSize.r, rectBorder.Height() - 2, PATCOPY);
  im->PatBlt(rectBorder.l + 1, rectBorder.b - rectBorderSize.b, rectBorder.Width() - 2, rectBorderSize.b, PATCOPY);
  im->SelectObject(pOldBrush);
}
COLOR VisualManagerXP_GetToolbarButtonTextColor(ToolbarButton* pButton,
    VisualManager::BCGBUTTON_STATE state)
{
  if (!IsHighContastMode(s)) {
    BOOL bDisabled = (ToolBar::IsCustomizeMode() && !pButton->IsEditable()) ||
        (!ToolBar::IsCustomizeMode() && (pButton->s->m_nStyle & TBBS_DISABLED));
    if (pButton->IsKindOf(RUNTIME_CLASS(OutlookButton))) {
      if (bDisabled) {
        return s->clrGrayedText;
      }
      return IsHighContastMode(s) ?
          s->clrWindowText : s->clrBarText;
    }
    if (state == ButtonsIsHighlighted &&
        (pButton->s->m_nStyle & (TBBS_PRESSED | TBBS_CHECKED))) {
      return s->clrTextHilite;
    }
  }
  return VisualManager::GetToolbarButtonTextColor(pButton, state);
}
BOOL VisualManagerXP_IsToolBarButtonDefaultBackground(ToolbarButton* pButton,
    VisualManager::BCGBUTTON_STATE state)
{
  BOOL bIsChecked = pButton->s->m_nStyle & (TBBS_CHECKED | TBBS_INDETERMINATE);
  return !bIsChecked && state != ButtonsIsHighlighted && state != ButtonsIsPressed;
}
int VisualManagerXP_OnDrawEditBorder(img_t* im, IRECT rect,
    BOOL bDisabled,
    BOOL bIsHighlighted,
    ToolbarEditBoxButton* pButton)
{
  if (!ToolbarEditBoxButton::IsFlatMode()) {
    VisualManager::OnDrawEditBorder(im, rect, bDisabled, bIsHighlighted, pButton);
    return 0;
  }
  if (bIsHighlighted) {
    imdraw_Draw3dRect(im, 0, &rect, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
  }
}
#ifndef BCGP_EXCLUDE_TASK_PANE
int VisualManagerXP_OnDrawTasksGroupCaption(img_t* im, TasksGroup* pGroup,
    BOOL bIsHighlighted, BOOL bIsSelected, BOOL bCanCollapse)
{
  ASSERT(pGroup != NULL);
  ASSERT_VALID(pGroup->s->m_pPage);
#ifndef BCGP_EXCLUDE_TOOLBOX
  BOOL bIsToolBox = pGroup->s->m_pPage->s->m_pTaskPane != NULL &&
      (pGroup->s->m_pPage->s->m_pTaskPane->IsKindOf(RUNTIME_CLASS(ToolBoxEx)));
#else
  BOOL bIsToolBox = FALSE;
#endif
  IRECT rectGroup = pGroup->s->m_rect;
  if (bIsToolBox) {
    IRECT rectFill = rectGroup;
    rectFill.DeflateRect(1, 0, 1, 1);
    CBrush brFill(IsHighContastMode(s) ?
        s->clrWindow : s->clrBarShadow);
    imdraw_FillRect(im, 0, rectFill, &brFill);
    if (bCanCollapse) {
      //--------------------
      // Draw expanding box:
      //--------------------
      int nBoxSize = 9;
      int nBoxOffset = 6;
      if (s->GetRibbonImageScale() != 1.) {
        nBoxSize = (int)(.5 + nBoxSize * s->GetRibbonImageScale());
      }
      IRECT rectButton = rectFill;
      rectButton.l += nBoxOffset;
      rectButton.r = rectButton.l + nBoxSize;
      rectButton.t = rectButton.CenterPoint().y - nBoxSize / 2;
      rectButton.b = rectButton.t + nBoxSize;
      imdraw_FillRect(im, 0, rectButton, &s->clrBarFace);
      OnDrawExpandingBox(im, rectButton, !pGroup->s->m_bIsCollapsed,
          s->clrBarText);
      rectGroup.l = rectButton.r + nBoxOffset;
      bCanCollapse = FALSE;
    }
  }
  // ------------------------------
  // Draw group caption(Office XP)
  // ------------------------------
  // ---------------------------
  // Draw an icon if it presents
  // ---------------------------
  BOOL bShowIcon = (pGroup->s->m_hIcon != NULL
      && pGroup->s->m_sizeIcon.w < rectGroup.Width() - rectGroup.Height());
  if (bShowIcon) {
    OnDrawTasksGroupIcon(im, pGroup, 5, bIsHighlighted, bIsSelected, bCanCollapse);
  }
  // -----------------------
  // Draw group caption text
  // -----------------------
  CFont* pFontOld = im->SelectObject(s->fontBold);
  COLOR clrTextOld = im->GetTextColor();
  if (bIsToolBox && !IsHighContastMode(s)) {
    im->SetTextColor(s->clrWindow);
  }
  else {
    if (bCanCollapse && bIsHighlighted) {
      clrTextOld = im->SetTextColor(pGroup->s->m_clrTextHot == (COLOR) - 1 ?
          s->clrWindowText : pGroup->s->m_clrTextHot);
    }
    else {
      clrTextOld = im->SetTextColor(pGroup->s->m_clrText == (COLOR) - 1 ?
          s->clrWindowText : pGroup->s->m_clrText);
    }
  }
  int nBkModeOld = im->SetBkMode(TRANSPARENT);
  int nTaskPaneHOffset = pGroup->s->m_pPage->s->m_pTaskPane->GetGroupCaptionHorzOffset();
  int nTaskPaneVOffset = pGroup->s->m_pPage->s->m_pTaskPane->GetGroupCaptionVertOffset();
  int nCaptionHOffset = (nTaskPaneHOffset != -1 ? nTaskPaneHOffset : s->m_nGroupCaptionHorzOffset);
  IRECT rectText = rectGroup;
  rectText.l += (bShowIcon ? pGroup->s->m_sizeIcon.w + 5 : nCaptionHOffset);
  rectText.t += (nTaskPaneVOffset != -1 ? nTaskPaneVOffset : s->m_nGroupCaptionVertOffset);
  rectText.r = max(rectText.l,
      rectText.r - (bCanCollapse ? rectGroup.Height() : nCaptionHOffset));
  im->DrawText(pGroup->s->m_strName, rectText, DT_SINGLELINE | DT_VCENTER);
  im->SetBkMode(nBkModeOld);
  im->SelectObject(pFontOld);
  im->SetTextColor(clrTextOld);
  // -------------------------
  // Draw group caption button
  // -------------------------
  if (bCanCollapse && !pGroup->s->m_strName.IsEmpty()) {
    CSize sizeButton = MenuImages::Size();
    IRECT rectButton = rectGroup;
    rectButton.l = max(rectButton.l, rectButton.r - sizeButton.w);
    rectButton.t = max(rectButton.t, rectButton.b - sizeButton.h);
    if (rectButton.Width() >= sizeButton.w && rectButton.Height() >= sizeButton.h) {
      if (bIsHighlighted) {
        // Draw button frame
        CPen* pPenOld = (CPen*) im->SelectObject(&s->penHilite);
        COLOR pBrushOld = (COLOR) im->SelectObject(s->m_clrHighlight);
        COLOR clrBckOld = im->GetBkColor();
        im->Rectangle(&rectButton);
        im->SetBkColor(clrBckOld);
        im->SelectObject(pPenOld);
        im->SelectObject(pBrushOld);
      }
      if (!pGroup->s->m_bIsCollapsed) {
        MenuImages::Draw(im, MenuImages::IdArowUp, rectButton.TopLeft());
      }
      else {
        MenuImages::Draw(im, MenuImages::IdArowDown, rectButton.TopLeft());
      }
    }
  }
}
int VisualManagerXP_OnFillTasksGroupInterior(img_t* im, IRECT rect, BOOL /*bSpecial*/)
{
  // Draw underline
  CPen* pPenOld = (CPen*) im->SelectObject(&s->penBarShadow);
  im->MoveTo(rect.l, rect.t);
  im->LineTo(rect.r, rect.t);
  im->SelectObject(pPenOld);
}
int VisualManagerXP_OnDrawTasksGroupAreaBorder(img_t* /*im*/, IRECT /*rect*/,
    BOOL /*bSpecial*/, BOOL /*bNoTitle*/)
{
}
int VisualManagerXP_OnDrawTask(img_t* im, Task* pTask, CImageList* pIcons, BOOL bIsHighlighted, BOOL /*bIsSelected*/)
{
  ASSERT_VALID(pIcons);
  ASSERT(pTask != NULL);
  IRECT rectText = pTask->s->m_rect;
  if (pTask->s->m_bIsSeparator) {
    CPen* pPenOld = (CPen*) im->SelectObject(&s->penBarShadow);
    im->MoveTo(rectText.l, rectText.CenterPoint().y);
    im->LineTo(rectText.r, rectText.CenterPoint().y);
    im->SelectObject(pPenOld);
    return 0;
  }
  // ---------
  // Draw icon
  // ---------
  CSize sizeIcon(0, 0);
  ::ImageList_GetIconSize(pIcons->m_hImageList, (int*) &sizeIcon.w, (int*) &sizeIcon.h);
  if (pTask->s->m_nIcon >= 0 && sizeIcon.w > 0) {
    pIcons->Draw(im, pTask->s->m_nIcon, rectText.TopLeft(), ILD_TRANSPARENT);
  }
  int nTaskPaneOffset = pTask->s->m_pGroup->s->m_pPage->s->m_pTaskPane->GetTasksIconHorzOffset();
  rectText.l += sizeIcon.w + (nTaskPaneOffset != -1 ? nTaskPaneOffset : s->m_nTasksIconHorzOffset);
  // ---------
  // Draw text
  // ---------
  BOOL bIsLabel = (pTask->s->m_uiCommandID == 0);
  CFont* pFontOld = NULL;
  COLOR clrTextOld = im->GetTextColor();
  if (bIsLabel) {
    pFontOld = im->SelectObject(
        pTask->s->m_bIsBold ? &s->fontBold : &s->fontRegular);
    im->SetTextColor(pTask->s->m_clrText == (COLOR) - 1 ?
        s->clrWindowText : pTask->s->m_clrText);
  }
  else if (!pTask->s->m_bEnabled) {
    im->SetTextColor(s->clrGrayedText);
    pFontOld = im->SelectObject(s->fontRegular);
  }
  else if (bIsHighlighted) {
    im->SetTextColor(pTask->s->m_clrTextHot == (COLOR) - 1 ?
        s->m_clrTaskPaneHotText : pTask->s->m_clrTextHot);
    pFontOld = im->SelectObject(s->fontUnderline);
  }
  else {
    im->SetTextColor(pTask->s->m_clrText == (COLOR) - 1 ?
        s->clrWindowText : pTask->s->m_clrText);
    pFontOld = im->SelectObject(s->fontRegular);
  }
  int nBkModeOld = im->SetBkMode(TRANSPARENT);
  TasksPane* pTaskPane = pTask->s->m_pGroup->s->m_pPage->s->m_pTaskPane;
  ASSERT_VALID(pTaskPane);
  BOOL bMultiline = bIsLabel ?
      pTaskPane->IsWrapLabelsEnabled() : pTaskPane->IsWrapTasksEnabled();
  if (bMultiline) {
    im->DrawText(pTask->s->m_strName, rectText, DT_WORDBREAK);
  }
  else {
    CString strText = pTask->s->m_strName;
    strText.Remove(('\n'));
    strText.Remove(('\r'));
    im->DrawText(strText, rectText, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
  }
  im->SetBkMode(nBkModeOld);
  im->SelectObject(pFontOld);
  im->SetTextColor(clrTextOld);
}
int VisualManagerXP_OnDrawScrollButtons(img_t* im, const IRECT& rect, const int nBorderSize,
    int iImage, BOOL bHilited)
{
  IRECT rectFill = rect;
  rectFill.t -= nBorderSize;
  imdraw_FillRect(im, 0, rectFill, &s->clrWindow);
  if (bHilited) {
    imdraw_FillRect(im, 0, rect, s->m_clrHighlight);
    imdraw_Draw3dRect(im, 0, rect, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
  }
  MenuImages::Draw(im, (MenuImages::IMAGES_IDS) iImage, rect);
}
#endif // BCGP_EXCLUDE_TASK_PANE
int VisualManagerXP_OnDrawSpinButtons(img_t* im, IRECT rectSpin,
    int nState, BOOL bOrientation, SpinButtonCtrl* pSpinCtrl)
{
  ASSERT_VALID(this);
  IRECT rect [2];
  rect[0] = rect[1] = rectSpin;
  if (!bOrientation) {
    rect[0].DeflateRect(0, 0, 0, rect[0].Height() / 2);
    rect[1].t = rect[0].b ;
  }
  else {
    rect[0].DeflateRect(0, 0, rect[0].Width() / 2, 0);
    rect[1].l = rect[0].r ;
  }
  MenuImages::IMAGES_IDS id[2][2] = {{MenuImages::IdArowUp, MenuImages::IdArowDown}, {MenuImages::IdArowLeft, MenuImages::IdArowRight}};
  int idxPressed = (nState & (SPIN_PRESSEDUP | SPIN_PRESSEDDOWN)) - 1;
  int idxHighlighted = -1;
  if (nState & SPIN_HIGHLIGHTEDUP) {
    idxHighlighted = 0;
  }
  else if (nState & SPIN_HIGHLIGHTEDDOWN) {
    idxHighlighted = 1;
  }
  BOOL bDisabled = nState & SPIN_DISABLED;
  for (int i = 0; i < 2; i ++) {
    if (idxPressed == i || idxHighlighted == i) {
      OnFillHighlightedArea(im, rect [i],
          (idxPressed == i) ? s->m_clrHighlightDn : s->m_clrHighlight, NULL);
    }
    else {
      OnFillSpinButton(im, pSpinCtrl, rect[i], bDisabled);
    }
    MenuImages::Draw(im, id[bOrientation ? 1 : 0][i], rect[i],
        bDisabled ? MenuImages::ImageGray : MenuImages::ImageBlack);
  }
  if (idxHighlighted >= 0) {
    IRECT rectHot = rect [idxHighlighted];
    if (ToolBarImages::s->m_bIsDrawOnGlass) {
      DrawManager dm(*im);
      dm.DrawRect(rectHot, (COLOR) - 1, s->m_clrMenuItemBorder);
    }
    else {
      imdraw_Draw3dRect(im, 0, rectHot, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
    }
  }
}
int VisualManagerXP_OnFillSpinButton(img_t* im, SpinButtonCtrl* /*pSpinCtrl*/, IRECT rect, BOOL bDisabled)
{
  if (ToolBarImages::s->m_bIsDrawOnGlass) {
    DrawManager dm(*im);
    dm.DrawRect(rect,
        bDisabled ? s->clrBtnFace : s->clrBarFace,
        s->clrBarHilite);
  }
  else {
    imdraw_FillRect(im, 0, rect, bDisabled ? &s->clrBtnFace : &s->clrBarFace);
    imdraw_Draw3dRect(im, 0, rect, s->clrBarHilite, s->clrBarHilite);
  }
}
int VisualManagerXP_OnDrawSplitterBorder(img_t* im, SplitterWnd* /*pSplitterWnd*/, IRECT rect)
{
  imdraw_Draw3dRect(im, 0, rect, s->clrBarShadow, s->clrBarShadow);
  rect.InflateRect(-CX_BORDER, -CY_BORDER);
  imdraw_Draw3dRect(im, 0, rect, s->clrBarFace, s->clrBarFace);
}
int VisualManagerXP_OnDrawSplitterBox(img_t* im, SplitterWnd* /*pSplitterWnd*/, IRECT& rect)
{
  imdraw_Draw3dRect(im, 0, rect, s->clrBarFace, s->clrBarFace);
}
#endif
BOOL VisualManagerXP_OnDrawCalculatorButton(uidraw_t* s, img_t* im, IRECT rect, int state, const char* strText)
{
  IRECT rcTxt = rect;
  COLOR clrText = s->clrWindowText;
  switch (state) {
  case ButtonsIsPressed:
    clrText = s->clrWindow;
    imdraw_Draw3dRect(im, 0, &rect, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder, s->m_clrHighlightDn);
    iRectOffset2(&rcTxt, 1, 1);
    break;
  case ButtonsIsHighlighted:
    imdraw_Draw3dRect(im, 0, &rect, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder, s->m_clrHighlight);
    break;
  default:
    imdraw_Draw3dRect(im, &rect, &rect, s->clrBarHilite, s->clrBarShadow, s->clrBtnFace);
    break;
  }
  imdraw_text(im, &rect, rcTxt, strText, -1, s->fontDefault, clrText, 0, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
  return TRUE;
}
BOOL VisualManagerXP_OnDrawCalculatorDisplay(uidraw_t* s, img_t* im, IRECT rect, const char* strText)
{
  imdraw_Draw3dRect(im, 0, &rect, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder, s->clrWindow);
  return TRUE;
}
#if 0
BOOL VisualManagerXP_OnDrawBrowseButton(img_t* im, IRECT rect,
    Edit* /*pEdit*/, VisualManager::BCGBUTTON_STATE state, COLOR& /*clrText*/)
{
  IRECT rectFrame = rect;
  rectFrame.InflateRect(0, 1, 1, 1);
  switch (state) {
  case ButtonsIsPressed:
    imdraw_FillRect(im, 0, rect, s->m_clrHighlightDn);
    imdraw_Draw3dRect(im, 0, &rectFrame, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
    im->SetTextColor(s->clrWindow);
    break;
  case ButtonsIsHighlighted:
    imdraw_FillRect(im, 0, rect, s->m_clrHighlight);
    imdraw_Draw3dRect(im, 0, &rectFrame, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
    break;
  default:
    imdraw_FillRect(im, 0, rect, &s->clrBtnFace);
    imdraw_Draw3dRect(im, 0, rect, s->clrBarHilite, s->clrBarHilite);
    break;
  }
  return TRUE;
}
COLOR VisualManagerXP_GetAutoHideButtonTextColor(AutoHideButton* /*pButton*/)
{
  return s->clrBtnDkShadow;
}
int VisualManagerXP_OnDrawAppBarBorder(
    img_t* im, AppBarWnd* /*pAppBarWnd*/,
    IRECT rectBorder, IRECT rectBorderSize)
{
  COLOR pOldBrush = im->SelectObject(s->m_clrFloatToolBarBorder);
  ASSERT(pOldBrush != NULL);
  im->PatBlt(rectBorder.l, rectBorder.t, rectBorderSize.l, rectBorder.Height(), PATCOPY);
  im->PatBlt(rectBorder.l, rectBorder.t, rectBorder.Width(), rectBorderSize.t, PATCOPY);
  im->PatBlt(rectBorder.r - rectBorderSize.r, rectBorder.t, rectBorderSize.r, rectBorder.Height(), PATCOPY);
  im->PatBlt(rectBorder.l, rectBorder.b - rectBorderSize.b, rectBorder.Width(), rectBorderSize.b, PATCOPY);
  rectBorderSize.DeflateRect(2, 2);
  rectBorder.DeflateRect(2, 2);
  im->SelectObject(&s->clrLight);
  im->PatBlt(rectBorder.l, rectBorder.t + 1, rectBorderSize.l, rectBorder.Height() - 2, PATCOPY);
  im->PatBlt(rectBorder.l + 1, rectBorder.t, rectBorder.Width() - 2, rectBorderSize.t, PATCOPY);
  im->PatBlt(rectBorder.r - rectBorderSize.r, rectBorder.t + 1, rectBorderSize.r, rectBorder.Height() - 2, PATCOPY);
  im->PatBlt(rectBorder.l + 1, rectBorder.b - rectBorderSize.b, rectBorder.Width() - 2, rectBorderSize.b, PATCOPY);
  im->SelectObject(pOldBrush);
}
int VisualManagerXP_GetSmartDockingBaseMarkerColors(
    COLOR& clrBaseGroupBackground,
    COLOR& clrBaseGroupBorder)
{
  clrBaseGroupBackground = s->m_clrBarBkgnd;
  clrBaseGroupBorder = s->m_clrMenuBorder;
}
int VisualManagerXP_OnDrawButtonSeparator(img_t* im,
    ToolbarButton* /*pButton*/, IRECT rect, VisualManager::BCGBUTTON_STATE /*state*/,
    BOOL bHorz)
{
  CPen* pOldPen = im->SelectObject(&s->m_penMenuItemBorder);
  ASSERT(pOldPen != NULL);
  if (bHorz) {
    im->MoveTo(rect.l, rect.t);
    im->LineTo(rect.l, rect.b);
  }
  else {
    im->MoveTo(rect.l, rect.t);
    im->LineTo(rect.r, rect.t);
  }
  im->SelectObject(pOldPen);
}
#ifndef BCGP_EXCLUDE_POPUP_WINDOW
int VisualManagerXP_OnDrawPopupWindowBorder(img_t* im, IRECT rect)
{
  imdraw_Draw3dRect(im, 0, rect, s->m_clrMenuBorder, s->m_clrMenuBorder);
  rect.DeflateRect(1, 1);
  imdraw_Draw3dRect(im, 0, rect, s->m_clrMenuLight, s->m_clrMenuLight);
}
COLOR VisualManagerXP_OnDrawPopupWindowCaption(img_t* im, IRECT rectCaption, PopupWindow* /*pPopupWnd*/)
{
  imdraw_FillRect(im, 0, rectCaption, s->m_clrHighlight);
  // get the text color
  return s->clrBarText;
}
int VisualManagerXP_OnErasePopupWindowButton(img_t* im, IRECT rc, PopupWndButton* pButton)
{
  if (pButton->IsPressed()) {
    CBrush br(s->m_clrHighlightDn);
    imdraw_FillRect(im, 0, &rc, &br);
    return 0;
  }
  else if (pButton->IsHighlighted() || pButton->IsPushed()) {
    CBrush br(s->m_clrHighlight);
    imdraw_FillRect(im, 0, &rc, &br);
    return 0;
  }
  IRECT rectParent;
  pButton->GetParent()->GetClientRect(rectParent);
  pButton->GetParent()->MapWindowPoints(pButton, rectParent);
  OnFillPopupWindowBackground(im, rectParent);
}
int VisualManagerXP_OnDrawPopupWindowButtonBorder(img_t* im, IRECT rc, PopupWndButton* pButton)
{
  if (pButton->IsHighlighted() || pButton->IsPushed() ||
      pButton->IsCaptionButton()) {
    imdraw_Draw3dRect(im, 0, rc, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
  }
}
int VisualManagerXP_OnFillPopupWindowBackground(img_t* im, IRECT rect)
{
  imdraw_FillRect(im, 0, rect, s->m_clrMenuLight);
}
#endif // BCGP_EXCLUDE_POPUP_WINDOW
#ifndef BCGP_EXCLUDE_PLANNER
COLOR VisualManagerXP_OnFillPlannerCaption(img_t* im, PlannerView* pView,
    IRECT rect, BOOL bIsToday, BOOL bIsSelected, BOOL bNoBorder/* = FALSE*/, BOOL bHorz/* = TRUE*/)
{
  if (bIsToday) {
    rect.DeflateRect(1, 1);
    imdraw_FillRect(im, 0, rect, s->m_clrHighlight);
    CPen pen(PS_SOLID, 1, s->m_clrMenuItemBorder);
    CPen* pOldPen = im->SelectObject(&pen);
    im->MoveTo(rect.l, rect.b);
    im->LineTo(rect.r, rect.b);
    im->SelectObject(pOldPen);
    return s->clrBtnText;
  }
  return VisualManager::OnFillPlannerCaption(im, pView, rect, bIsToday,
      bIsSelected, bNoBorder, bHorz);
}
int VisualManagerXP_OnDrawPlannerTimeLine(img_t* im, PlannerView* /*pView*/, IRECT rect)
{
  imdraw_FillRect(im, 0, rect, s->m_clrHighlight);
  CPen* pOldPen = im->SelectObject(&s->m_penSeparator);
  im->MoveTo(rect.l, rect.b);
  im->LineTo(rect.r, rect.b);
  im->SelectObject(pOldPen);
}
#endif // BCGP_EXCLUDE_PLANNER
int VisualManagerXP_GetCalendarColors(const Calendar* pCalendar,
    CalendarColors& colors)
{
  VisualManager::GetCalendarColors(pCalendar, colors);
  colors.clrSelected = s->m_clrHighlight;
}
#ifndef BCGP_EXCLUDE_PROP_LIST
COLOR VisualManagerXP_GetPropListGroupColor(PropList* pPropList)
{
  ASSERT_VALID(pPropList);
  if (s->m_nBitsPerPixel <= 8) {
    return VisualManager::GetPropListGroupColor(pPropList);
  }
  return PixelAlpha2(
      pPropList->DrawControlBarColors() ?
      s->clrBarFace : s->clrBtnFace, 94);
}
COLOR VisualManagerXP_GetPropListGroupTextColor(PropList* pPropList)
{
  ASSERT_VALID(pPropList);
  return pPropList->DrawControlBarColors() ?
      s->clrBarShadow : s->clrBtnShadow;
}
#endif // BCGP_EXCLUDE_PROP_LIST
#ifndef BCGP_EXCLUDE_RIBBON
COLOR VisualManagerXP_OnFillRibbonButton(
    img_t* im,
    RibbonButton* pButton)
{
  if (pButton->IsDefaultPanelButton() && !pButton->IsQATMode()) {
    return(COLOR) - 1;
  }
  IRECT rect = pButton->GetRect();
  const BOOL bIsMenuMode = pButton->IsMenuMode() && !pButton->IsPaletteIcon();
  const BOOL bIsHighlighted =
      ((pButton->IsHighlighted() || pButton->IsDroppedDown()) &&
          !pButton->IsDisabled()) || pButton->IsFocused();
  if (pButton->IsKindOf(RUNTIME_CLASS(RibbonEdit))) {
    COLOR clrBorder = s->clrBarShadow;
    IRECT rectCommand = pButton->GetCommandRect();
    if (pButton->GetLocationInGroup() != BaseRibbonElement::RibbonElementNotInGroup) {
      rectCommand.r++;
    }
    if (ToolBarImages::s->m_bIsDrawOnGlass) {
      DrawManager dm(*im);
      if (bIsHighlighted) {
        dm.DrawRect(rect, s->clrWindow, clrBorder);
      }
      else {
        dm.DrawRect(rect, s->clrBarFace, clrBorder);
      }
    }
    else {
      if (bIsHighlighted) {
        imdraw_FillRect(im, 0, rectCommand, &s->clrWindow);
      }
      else {
        imdraw_FillRect(im, 0, rectCommand, &s->clrBarFace);
        DrawManager dm(*im);
        dm.HighlightRect(rectCommand);
      }
      imdraw_Draw3dRect(im, 0, rect, clrBorder, clrBorder);
    }
    return(COLOR) - 1;
  }
  if (!pButton->IsChecked() && !bIsHighlighted) {
    return(COLOR) - 1;
  }
  if (pButton->IsChecked() && bIsMenuMode && !bIsHighlighted) {
    return(COLOR) - 1;
  }
  IRECT rectMenu = pButton->GetMenuRect();
  if (pButton->GetLocationInGroup() != BaseRibbonElement::RibbonElementNotInGroup) {
    rect.DeflateRect(1, 1);
  }
  IRECT rectCommand(0, 0, 0, 0);
  if (!rectMenu.IsRectEmpty()) {
    rectCommand = pButton->GetCommandRect();
    if (pButton->GetLocationInGroup() != BaseRibbonElement::RibbonElementNotInGroup) {
      rectMenu.DeflateRect(0, 1, 1, 1);
      rectCommand.DeflateRect(1, 1, 0, 1);
    }
  }
  if (!rectMenu.IsRectEmpty() && bIsHighlighted) {
    if (pButton->IsCommandAreaHighlighted()) {
      OnFillHighlightedArea(im, rectCommand,
          (pButton->IsPressed() || pButton->IsDroppedDown()) && !bIsMenuMode ?
          s->m_clrHighlightDn : s->m_clrHighlight, NULL);
    }
    else {
      OnFillHighlightedArea(im, rectCommand,
          s->m_clrHighlight, NULL);
      DrawManager dm(*im);
      dm.HighlightRect(rectCommand);
    }
    if (pButton->IsMenuAreaHighlighted()) {
      OnFillHighlightedArea(im, rectMenu,
          (pButton->IsPressed() || pButton->IsDroppedDown()) && !bIsMenuMode ?
          s->m_clrHighlightDn : s->m_clrHighlight, NULL);
    }
    else {
      OnFillHighlightedArea(im, rectMenu,
          s->m_clrHighlight, NULL);
      DrawManager dm(*im);
      dm.HighlightRect(rectMenu);
    }
  }
  else {
    COLOR pBrush =
        (pButton->IsPressed() || pButton->IsDroppedDown()) && !bIsMenuMode ?
        s->m_clrHighlightDn : s->m_clrHighlight;
    IRECT rectFill = rect;
    if (pButton->IsChecked() && !bIsMenuMode) {
      pBrush = bIsHighlighted ?
          s->m_clrHighlightDn : s->m_clrHighlightChecked;
      if (!bIsHighlighted && !rectCommand.IsRectEmpty()) {
        rectFill = rectCommand;
      }
    }
    OnFillHighlightedArea(im, rectFill, pBrush, NULL);
  }
  return(COLOR) - 1;
}
int VisualManagerXP_OnDrawRibbonCategoryScroll(
    img_t* im,
    RibbonCategoryScroll* pScroll)
{
  ASSERT_VALID(pScroll);
  IRECT rect = pScroll->GetRect();
  rect.b--;
  OnFillHighlightedArea(im, rect,
      pScroll->IsHighlighted() ? s->m_clrHighlight : &s->clrBarFace, NULL);
  BOOL bIsLeft = pScroll->IsLeftScroll();
  if (s->m_bIsRTL) {
    bIsLeft = !bIsLeft;
  }
  MenuImages::Draw(im,
      bIsLeft ? MenuImages::IdArowLeftLarge : MenuImages::IdArowRightLarge,
      rect);
  imdraw_Draw3dRect(im, 0, rect, s->clrBarShadow, s->clrBarShadow);
}
int VisualManagerXP_OnDrawRibbonButtonBorder(
    img_t* im,
    RibbonButton* pButton)
{
  if (pButton->IsDefaultPanelButton() && !pButton->IsQATMode()) {
    return 0;
  }
  if (pButton->IsKindOf(RUNTIME_CLASS(RibbonEdit))) {
    return 0;
  }
  const BOOL bIsMenuMode = pButton->IsMenuMode();
  const BOOL bIsHighlighted =
      ((pButton->IsHighlighted() || pButton->IsDroppedDown()) &&
          !pButton->IsDisabled()) || pButton->IsFocused();
  if (pButton->IsChecked() && bIsMenuMode && !bIsHighlighted) {
    return 0;
  }
  IRECT rect = pButton->GetRect();
  IRECT rectMenu = pButton->GetMenuRect();
  if (pButton->GetLocationInGroup() != BaseRibbonElement::RibbonElementNotInGroup) {
    rect.DeflateRect(1, 1);
  }
  if ((bIsHighlighted || pButton->IsChecked()) &&
      (!pButton->IsDisabled() || pButton->IsFocused() || pButton->IsChecked())) {
    COLOR clrLine =
        ((pButton->IsPressed() || pButton->IsDroppedDown()) && !bIsMenuMode) ?
        s->m_clrPressedButtonBorder : s->m_clrMenuItemBorder;
    if (ToolBarImages::s->m_bIsDrawOnGlass) {
      DrawManager dm(*im);
      dm.DrawRect(rect, (COLOR) - 1, s->m_clrMenuItemBorder);
    }
    else {
      imdraw_Draw3dRect(im, 0, rect, clrLine, clrLine);
    }
    if (!rectMenu.IsRectEmpty()) {
      if (pButton->GetLocationInGroup() != BaseRibbonElement::RibbonElementNotInGroup) {
        rectMenu.DeflateRect(0, 1, 1, 1);
      }
      if (ToolBarImages::s->m_bIsDrawOnGlass) {
        DrawManager dm(*im);
        if (pButton->IsMenuOnBottom()) {
          dm.DrawLine(rectMenu.l, rectMenu.t, rectMenu.r, rectMenu.t, s->m_clrMenuItemBorder);
        }
        else {
          dm.DrawLine(rectMenu.l, rectMenu.t, rectMenu.l, rectMenu.b - 1, s->m_clrMenuItemBorder);
        }
      }
      else {
        CPen* pOldPen = im->SelectObject(&s->m_penMenuItemBorder);
        ASSERT(pOldPen != NULL);
        if (pButton->IsMenuOnBottom()) {
          im->MoveTo(rectMenu.l, rectMenu.t);
          im->LineTo(rectMenu.r, rectMenu.t);
        }
        else {
          im->MoveTo(rectMenu.l, rectMenu.t);
          im->LineTo(rectMenu.l, rectMenu.b);
        }
        im->SelectObject(pOldPen);
      }
    }
  }
}
int VisualManagerXP_OnDrawRibbonMenuCheckFrame(
    img_t* im,
    RibbonButton* /*pButton*/,
    IRECT rect)
{
  OnFillHighlightedArea(im, rect, s->m_clrHighlight, NULL);
  imdraw_Draw3dRect(im, 0, rect, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
}
int VisualManagerXP_OnFillRibbonMenuFrame(
    img_t* im,
    RibbonMainPanel* /*pPanel*/,
    IRECT rect)
{
  imdraw_FillRect(im, 0, rect, s->m_clrMenuLight);
}
int VisualManagerXP_OnDrawRibbonRecentFilesFrame(
    img_t* im,
    RibbonMainPanel* /*pPanel*/,
    IRECT rect)
{
  imdraw_FillRect(im, 0, rect, s->m_clrBarBkgnd);
  IRECT rectSeparator = rect;
  rectSeparator.r = rectSeparator.l + 2;
  imdraw_Draw3dRect(im, 0, rectSeparator, s->clrBarShadow,
      s->clrBarHilite);
}
COLOR VisualManagerXP_OnDrawRibbonStatusBarPane(img_t* im, RibbonStatusBar* /*pBar*/,
    RibbonStatusBarPane* pPane)
{
  ASSERT_VALID(pPane);
  IRECT rect = pPane->GetRect();
  if (pPane->IsHighlighted()) {
    IRECT rectButton = rect;
    rectButton.DeflateRect(1, 1);
    OnFillHighlightedArea(im, rectButton,
        pPane->IsPressed() ?
        s->m_clrHighlightDn : s->m_clrHighlight, NULL);
    imdraw_Draw3dRect(im, 0, rectButton, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
  }
  IRECT rectSeparator = rect;
  rectSeparator.DeflateRect(0, 2);
  rectSeparator.l = rectSeparator.r - 1;
  imdraw_Draw3dRect(im, 0, rectSeparator, s->clrBarShadow,
      s->clrBarShadow);
  if (IsHighContastMode(s)) {
    return s->clrBarText;
  }
  return(COLOR) - 1;
}
int VisualManagerXP_GetRibbonSliderColors(RibbonSlider* /*pSlider*/,
    BOOL bIsHighlighted,
    BOOL bIsPressed,
    BOOL bIsDisabled,
    COLOR& clrLine,
    COLOR& clrFill)
{
  clrLine = bIsDisabled ? s->clrBarShadow :
      (bIsPressed || bIsHighlighted) ? s->m_clrMenuItemBorder : s->clrBarDkShadow;
  clrFill = bIsPressed && bIsHighlighted ?
      s->m_clrHighlightDn :
      bIsHighlighted ? s->m_clrHighlight : s->clrBarFace;
}
int VisualManagerXP_OnDrawRibbonQATSeparator(img_t* im,
    RibbonSeparator* /*pSeparator*/, IRECT rect)
{
  int x = rect.CenterPoint().x;
  int y1 = rect.t;
  int y2 = rect.b - 1;
  if (ToolBarImages::s->m_bIsDrawOnGlass) {
    DrawManager dm(*im);
    dm.DrawLine(x, y1, x, y2, s->m_clrSeparator);
  }
  else {
    CPen* pOldPen = im->SelectObject(&s->m_penSeparator);
    ASSERT(pOldPen != NULL);
    im->MoveTo(x, y1);
    im->LineTo(x, y2);
    im->SelectObject(pOldPen);
  }
}
int VisualManagerXP_OnDrawRibbonColorPaletteBox(img_t* im, RibbonColorButton* /*pColorButton*/,
    RibbonPaletteIcon* /*pIcon*/,
    COLOR color, IRECT rect, BOOL bDrawTopEdge, BOOL bDrawBottomEdge,
    BOOL bIsHighlighted, BOOL bIsChecked, BOOL /*bIsDisabled*/)
{
  IRECT rectFill = rect;
  rectFill.DeflateRect(1, 0);
  if (bIsHighlighted || bIsChecked) {
    OnFillHighlightedArea(im, rect, s->m_clrHighlight, NULL);
    rectFill.DeflateRect(1, 2);
  }
  if (color != (COLOR) - 1) {
    CBrush br(color);
    imdraw_FillRect(im, 0, rectFill, &br);
  }
  COLOR clrBorder = _RGB(197, 197, 197);
  if (bDrawTopEdge && bDrawBottomEdge) {
    imdraw_Draw3dRect(im, 0, rect, clrBorder, clrBorder);
  }
  else {
    CPen penBorder(PS_SOLID, 1, clrBorder);
    CPen* pOldPen = im->SelectObject(&penBorder);
    ASSERT(pOldPen != NULL);
    im->MoveTo(rect.l, rect.t);
    im->LineTo(rect.l, rect.b);
    im->MoveTo(rect.r - 1, rect.t);
    im->LineTo(rect.r - 1, rect.b);
    if (bDrawTopEdge) {
      im->MoveTo(rect.l, rect.t);
      im->LineTo(rect.r, rect.t);
    }
    if (bDrawBottomEdge) {
      im->MoveTo(rect.l, rect.b - 1);
      im->LineTo(rect.r, rect.b - 1);
    }
    im->SelectObject(pOldPen);
  }
  if (bIsHighlighted || bIsChecked) {
    clrBorder = bIsChecked ? s->m_clrPressedButtonBorder : s->m_clrMenuItemBorder;
    imdraw_Draw3dRect(im, 0, rect, clrBorder, clrBorder);
  }
}
COLOR VisualManagerXP_OnDrawRibbonPanel(
    img_t* im,
    RibbonPanel* pPanel,
    IRECT rectPanel,
    IRECT /*rectCaption*/)
{
  ASSERT_VALID(pPanel);
  COLOR clrText = s->clrBarText;
  if (pPanel->IsCollapsed() && pPanel->GetDefaultButton().IsFocused()) {
    OnFillHighlightedArea(im, rectPanel,
        pPanel->GetDefaultButton().IsDroppedDown() ? s->m_clrHighlightDn : s->m_clrHighlight, NULL);
    if (GetRV(s->m_clrHighlight) > 128 &&
        GetGV(s->m_clrHighlight) > 128 &&
        GetBV(s->m_clrHighlight) > 128) {
      clrText = _RGB(0, 0, 0);
    }
    else {
      clrText = _RGB(255, 255, 255);
    }
  }
  else if (pPanel->IsHighlighted()) {
    DrawManager dm(*im);
    dm.HighlightRect(rectPanel);
  }
  imdraw_Draw3dRect(im, 0, rectPanel, s->clrBarHilite, s->clrBarHilite);
  rectPanel.OffsetRect(-1, -1);
  imdraw_Draw3dRect(im, 0, rectPanel, s->clrBarShadow, s->clrBarShadow);
  return clrText;
}
#endif // BCGP_EXCLUDE_RIBBON
COLOR VisualManagerXP_OnFillListBoxItem(img_t* im, ListBox* /*pListBox*/, int /*nItem*/, IRECT rect, BOOL bIsHighlihted, BOOL bIsSelected)
{
  COLOR pBrush = NULL;
  if (bIsSelected) {
    pBrush = !bIsHighlihted ? s->m_clrHighlightChecked : s->m_clrHighlightDn;
  }
  else if (bIsHighlihted) {
    pBrush = s->m_clrHighlight;
  }
  OnFillHighlightedArea(im, rect, pBrush, NULL);
  imdraw_Draw3dRect(im, 0, rect, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
  return s->clrBtnText;
}
COLOR VisualManagerXP_OnDrawMenuLabel(img_t* im, IRECT rect)
{
  imdraw_FillRect(im, 0, rect, s->m_clrBarBkgnd);
  IRECT rectSeparator = rect;
  rectSeparator.t = rectSeparator.b - 2;
  imdraw_Draw3dRect(im, 0, rectSeparator, s->clrBarShadow,
      s->clrBarHilite);
  return s->clrBarText;
}
COLOR VisualManagerXP_OnFillCaptionBarButton(img_t* im, CaptionBar* pBar,
    IRECT rect, BOOL bIsPressed, BOOL bIsHighlighted,
    BOOL bIsDisabled, BOOL bHasDropDownArrow,
    BOOL bIsSysButton)
{
  ASSERT_VALID(pBar);
  if (!pBar->IsMessageBarMode()) {
    return VisualManager::OnFillCaptionBarButton(im, pBar,
        rect, bIsPressed, bIsHighlighted,
        bIsDisabled, bHasDropDownArrow, bIsSysButton);
  }
  if (bIsDisabled) {
    return(COLOR) - 1;
  }
  COLOR clrText = s->clrBarText;
  if (bIsHighlighted) {
    OnFillHighlightedArea(im, rect, s->m_clrHighlight, NULL);
    if (GetRV(s->m_clrHighlight) > 128 &&
        GetGV(s->m_clrHighlight) > 128 &&
        GetBV(s->m_clrHighlight) > 128) {
      clrText = _RGB(0, 0, 0);
    }
    else {
      clrText = _RGB(255, 255, 255);
    }
  }
  else if (!bIsSysButton) {
    imdraw_FillRect(im, 0, rect, s->m_clrMenuLight);
  }
  return clrText;
}
int VisualManagerXP_OnDrawCaptionBarButtonBorder(img_t* im, CaptionBar* pBar,
    IRECT rect, BOOL bIsPressed, BOOL bIsHighlighted,
    BOOL bIsDisabled, BOOL bHasDropDownArrow,
    BOOL bIsSysButton)
{
  ASSERT_VALID(pBar);
  if (!pBar->IsMessageBarMode()) {
    VisualManager::OnDrawCaptionBarButtonBorder(im, pBar,
        rect, bIsPressed, bIsHighlighted,
        bIsDisabled, bHasDropDownArrow, bIsSysButton);
    return 0;
  }
  if (bIsHighlighted) {
    if (bIsSysButton && bIsPressed && s->m_clrPressedButtonBorder != (COLOR) - 1) {
      imdraw_Draw3dRect(im, 0, rect, s->m_clrPressedButtonBorder, s->m_clrPressedButtonBorder);
    }
    else {
      imdraw_Draw3dRect(im, 0, rect, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
    }
  }
  else if (!bIsSysButton) {
    imdraw_Draw3dRect(im, 0, rect, s->clrBarDkShadow, s->clrBarDkShadow);
  }
}
int VisualManagerXP_OnDrawCaptionBarInfoArea(img_t* im, CaptionBar* /*pBar*/, IRECT rect)
{
  ::FillRect(im, rect, GetSysColorBrush(COLOR_INFOBK));
  imdraw_Draw3dRect(im, 0, rect, s->clrBarShadow, s->clrBarShadow);
}
BOOL VisualManagerXP_OnDrawPushButton(img_t* im, IRECT rect, Button* pButton, COLOR& clrText)
{
  DrawManager dm(*im);
  COLOR clrFace = s->clrBarFace;
  DrawOnGlass dog(pButton->s->m_bOnGlass);
  if (!pButton->IsWindowEnabled()) {
    if (pButton->s->m_bOnGlass) {
      dm.DrawRect(rect, clrFace, s->clrBarShadow);
    }
    else {
      imdraw_FillRect(im, 0, rect, &s->clrBarFace);
      imdraw_Draw3dRect(im, 0, rect, s->clrBarShadow, s->clrBarShadow);
    }
  }
  else if (pButton->IsPressed() || pButton->GetCheck()) {
    OnFillHighlightedArea(im, rect, s->m_clrHighlightDn, NULL);
    if (pButton->s->m_bOnGlass) {
      dm.DrawRect(rect, (COLOR) - 1, s->m_clrMenuItemBorder);
    }
    else {
      imdraw_Draw3dRect(im, 0, rect, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
    }
    clrFace = s->m_clrHighlightDn;
  }
  else if (pButton->IsHighlighted() || CWnd::GetFocus() == pButton) {
    OnFillHighlightedArea(im, rect, s->m_clrHighlight, NULL);
    if (pButton->s->m_bOnGlass) {
      dm.DrawRect(rect, (COLOR) - 1, s->m_clrMenuItemBorder);
    }
    else {
      imdraw_Draw3dRect(im, 0, rect, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
    }
    clrFace = s->m_clrHighlight;
  }
  else {
    if (pButton->s->m_bOnGlass) {
      dm.DrawRect(rect, clrFace, s->clrBarDkShadow);
    }
    else {
      imdraw_FillRect(im, 0, rect, &s->clrBarFace);
      imdraw_Draw3dRect(im, 0, rect, s->clrBarDkShadow, s->clrBarDkShadow);
    }
  }
  if (GetRV(clrFace) <= 128 ||
      GetGV(clrFace) <= 128 ||
      GetBV(clrFace) <= 128) {
    clrText = _RGB(255, 255, 255);
  }
  return TRUE;
}
#ifndef BCGP_EXCLUDE_GRID_CTRL
int VisualManagerXP_OnDrawGridHeaderMenuButton(GridCtrl* /*pCtrl*/, img_t* im, IRECT rect,
    BOOL bHighlighted, BOOL /*bPressed*/, BOOL /*bDisabled*/)
{
  rect.DeflateRect(1, 1);
  if (bHighlighted) {
    OnFillHighlightedArea(im, rect, s->m_clrHighlight, NULL);
    imdraw_Draw3dRect(im, 0, rect, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
  }
}
BOOL VisualManagerXP_OnSetGridColorTheme(GridCtrl* pCtrl, BCGP_GRID_COLOR_DATA& theme)
{
  theme.m_EvenColors.m_clrBackground = s->m_clrBarBkgnd;
  theme.m_EvenColors.m_clrText = s->clrBarText;
  theme.m_OddColors.m_clrBackground = s->m_clrMenuLight;
  theme.m_OddColors.m_clrText = s->clrBarText;
  theme.m_SelColors.s->m_clrBackground = s->m_clrHighlight;
  theme.m_SelColors.s->m_clrBorder = s->m_clrMenuItemBorder;
  if (GetRV(s->m_clrHighlight) > 128 &&
      GetGV(s->m_clrHighlight) > 128 &&
      GetBV(s->m_clrHighlight) > 128) {
    theme.m_SelColors.s->m_clrText = _RGB(0, 0, 0);
  }
  else {
    theme.m_SelColors.s->m_clrText = _RGB(255, 255, 255);
  }
  theme.m_GroupColors.s->m_clrBackground = GetGridLeftOffsetColor(pCtrl);
  theme.m_GroupColors.s->m_clrText = s->clrBarText;
  return TRUE;
}
#endif
#endif
int VisualManagerXP_init(uidraw_t* s, sys_t* sys)
{
  VisualManager_init(s, sys);
#define INITFUN(name) s->name = VisualManagerXP_##name
  INITFUN(OnUpdateSystemColors);
  INITFUN(OnDrawCalculatorButton);
  INITFUN(OnDrawCalculatorDisplay);
#undef INITFUN
  s->m_bConnectMenuToParent = TRUE;
  s->m_nVertMargin = 4;
  s->m_nHorzMargin = 4;
  s->m_nGroupVertOffset = 4;
  s->m_nGroupCaptionHeight = 0;
  s->m_nGroupCaptionHorzOffset = 0;
  s->m_nGroupCaptionVertOffset = 0;
  s->m_nTasksHorzOffset = 12;
  s->m_nTasksIconHorzOffset = 5;
  s->m_nTasksIconVertOffset = 4;
  s->m_bActiveCaptions = FALSE;
  s->m_bMenuFlatLook = TRUE;
  s->m_bShadowHighlightedImage = TRUE;
  s->m_bEmbossDisabledImage = FALSE;
  s->m_bFadeInactiveImage = TRUE;
  s->m_bLook2000 = TRUE;
  s->m_nMenuShadowDepth = 4;
  s->m_nMenuBorderSize = 1;
  s->m_bShdowDroppedDownMenuButton = TRUE;
  s->m_bOfficeXPStyleMenus = TRUE;
  s->m_bDrawLastTabLine = TRUE;
  UpdateSysColors(s);
  s->OnUpdateSystemColors(s);
  return 0;
}
#if 0
//
// BCGWinXPVisualManager.cpp: implementation of the WinXPVisualManager class.
//
//////////////////////////////////////////////////////////////////////
//#include "multimon.h"
#include "BCGPWinXPVisualManager.h"
#include "BCGPPopupMenuBar.h"
#include "BCGPButton.h"
#include "BCGPDrawManager.h"
#include "BCGPBaseTabWnd.h"
#include "BCGPDockingControlBar.h"
#include "BCGPTasksPane.h"
#include "BCGPToolbarMenuButton.h"
#include "BCGPOutlookButton.h"
#include "BCGPStatusBar.h"
#include "BCGPCaptionBar.h"
#include "BCGPToolBox.h"
#include "BCGPPopupWindow.h"
#include "BCGPCalendarBar.h"
#include "BCGPDockBar.h"
#include "BCGPRibbonStatusBar.h"
#include "BCGPRibbonPanelMenu.h"
#include "BCGPToolbarEditBoxButton.h"
#include "bcgpstyle.h"
#include "BCGPGanttChart.h"
#include "BCGPColorBar.h"
#include "BCGPSpinButtonCtrl.h"
IMPLEMENT_DYNCREATE(WinXPVisualManager, VisualManagerXP)
BOOL WinXPVisualManager::s->m_b3DTabsXPTheme = FALSE;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
WinXPVisualManager::WinXPVisualManager(BOOL bIsTemporary) :
  VisualManagerXP(bIsTemporary)
{
  s->m_bShadowHighlightedImage = FALSE;
  s->m_bOfficeStyleMenus = FALSE;
  s->m_nVertMargin = 12;
  s->m_nHorzMargin = 12;
  s->m_nGroupVertOffset = 15;
  s->m_nGroupCaptionHeight = 25;
  s->m_nGroupCaptionHorzOffset = 13;
  s->m_nGroupCaptionVertOffset = 7;
  s->m_nTasksHorzOffset = 12;
  s->m_nTasksIconHorzOffset = 5;
  s->m_nTasksIconVertOffset = 4;
  s->m_bActiveCaptions = TRUE;
  s->UpdateSysColors();
  OnUpdateSystemColors();
}
WinXPVisualManager::~WinXPVisualManager()
{
}
BOOL WinXPVisualManager::IsWinXPThemeAvailible()
{
  WinXPVisualManager* pWinXPManager =
      DYNAMIC_DOWNCAST(WinXPVisualManager, s->m_pVisManager);
  if (pWinXPManager != NULL) {
    return pWinXPManager->s->m_hThemeWindow != NULL;
  }
  // Create a temporary manager and check it:
  WinXPVisualManager winXPManager(TRUE /* Temporary */);
  return winXPManager.s->m_hThemeWindow != NULL;
}
int WinXPVisualManager::SetOfficeStyleMenus(BOOL bOn)
{
  s->m_bOfficeStyleMenus = bOn;
}
int WinXPVisualManager::OnUpdateSystemColors()
{
  VisualManagerXP::OnUpdateSystemColors();
  s->m_bShadowHighlightedImage = TRUE;
  WinXPThemeManager::UpdateSystemColors();
  if (s->m_hThemeWindow != NULL) {
    s->m_bShadowHighlightedImage = FALSE;
  }
  s->m_clrCalendarCaption = (COLOR) - 1;
  s->m_clrCalendarCaptionText = (COLOR) - 1;
  if (s->m_pfGetThemeColor != NULL) {
    if (s->m_hThemeToolBar != NULL) {
      (*s->m_pfGetThemeColor)(s->m_hThemeToolBar, TP_BUTTON, 0, TMT_TEXTCOLOR, &s->clrBarText);
      (*s->m_pfGetThemeColor)(s->m_hThemeToolBar, TP_BUTTON, 0, TMT_FILLCOLOR, &s->clrBarFace);
      (*s->m_pfGetThemeColor)(s->m_hThemeToolBar, TP_BUTTON, 0, TMT_EDGELIGHTCOLOR, &s->clrBarLight);
      (*s->m_pfGetThemeColor)(s->m_hThemeToolBar, TP_BUTTON, 0, TMT_EDGEHIGHLIGHTCOLOR, &s->clrBarHilite);
      (*s->m_pfGetThemeColor)(s->m_hThemeToolBar, TP_BUTTON, 0, TMT_EDGESHADOWCOLOR, &s->clrBarShadow);
      (*s->m_pfGetThemeColor)(s->m_hThemeToolBar, TP_BUTTON, 0, TMT_EDGEDKSHADOWCOLOR, &s->clrBarDkShadow);
    }
    if (s->m_hThemeButton != NULL) {
      (*s->m_pfGetThemeColor)(s->m_hThemeButton, BP_PUSHBUTTON, 0, TMT_TEXTCOLOR, &s->clrBtnText);
      (*s->m_pfGetThemeColor)(s->m_hThemeButton, BP_PUSHBUTTON, 0, TMT_FILLCOLOR, &s->clrBtnFace);
      (*s->m_pfGetThemeColor)(s->m_hThemeButton, BP_PUSHBUTTON, 0, TMT_EDGELIGHTCOLOR, &s->clrBtnLight);
      (*s->m_pfGetThemeColor)(s->m_hThemeButton, BP_PUSHBUTTON, 0, TMT_EDGEHIGHLIGHTCOLOR, &s->clrBtnHilite);
      (*s->m_pfGetThemeColor)(s->m_hThemeButton, BP_PUSHBUTTON, 0, TMT_EDGESHADOWCOLOR, &s->clrBtnShadow);
      (*s->m_pfGetThemeColor)(s->m_hThemeButton, BP_PUSHBUTTON, 0, TMT_EDGEDKSHADOWCOLOR, &s->clrBtnDkShadow);
    }
    if (s->m_hThemeExplorerBar != NULL) {
      (*s->m_pfGetThemeColor)(s->m_hThemeExplorerBar, EBP_NORMALGROUPBACKGROUND, 0, TMT_FILLCOLOR, s->m_clrCalendarCaption);
      (*s->m_pfGetThemeColor)(s->m_hThemeExplorerBar, EBP_NORMALGROUPBACKGROUND, 0, TMT_TEXTCOLOR, s->m_clrCalendarCaptionText);
    }
  }
}
int WinXPVisualManager::OnFillBarBackground(img_t* im, BaseControlBar* pBar,
    IRECT rectClient, IRECT rectClip,
    BOOL bNCArea)
{
  ASSERT_VALID(pBar);
  if (pBar->IsOnGlass()) {
    im->FillSolidRect(rectClient, _RGB(0, 0, 0));
    return 0;
  }
  if (pBar->IsKindOf(RUNTIME_CLASS(StatusBar)) &&
      s->m_hThemeStatusBar != NULL) {
    (*s->m_pfDrawThemeBackground)(s->m_hThemeStatusBar,
        im,
        0, 0, &rectClient, 0);
    return 0;
  }
  if (s->m_pfDrawThemeBackground == NULL ||
      s->m_hThemeRebar == NULL ||
      pBar->IsDialogControl() ||
      pBar->IsKindOf(RUNTIME_CLASS(CaptionBar)) ||
      pBar->IsKindOf(RUNTIME_CLASS(ColorBar))) {
    VisualManagerXP::OnFillBarBackground(im, pBar, rectClient, rectClip, bNCArea);
    return 0;
  }
  if (pBar->IsKindOf(RUNTIME_CLASS(PopupMenuBar))) {
    if (s->m_bOfficeStyleMenus) {
      VisualManagerXP::OnFillBarBackground(im, pBar, rectClient, rectClip, bNCArea);
    }
    else {
      OnFillPopupMenuBackground(im, rectClient);
      PopupMenuBar* pMenuBar = DYNAMIC_DOWNCAST(PopupMenuBar, pBar);
      if (s->m_hThemeMenu != NULL && !pMenuBar->s->m_bDisableSideBarInXPMode) {
        IRECT rectGutter = rectClient;
        rectGutter.r = rectGutter.l + pMenuBar->GetGutterWidth() + 2;
        rectGutter.DeflateRect(0, 1);
        (*s->m_pfDrawThemeBackground)(s->m_hThemeMenu, im,
            MENU_POPUPGUTTER, 0, &rectGutter, 0);
      }
    }
    return 0;
  }
  FillRebarPane(im, pBar, rectClient);
}
int WinXPVisualManager::OnFillPopupMenuBackground(img_t* im, IRECT rect)
{
  ::FillRect(im, rect, GetSysColorBrush(COLOR_MENU));
}
int WinXPVisualManager::OnDrawBarBorder(img_t* im, BaseControlBar* pBar, IRECT& rect)
{
  VisualManager::OnDrawBarBorder(im, pBar, rect);
}
int WinXPVisualManager::OnFillButtonInterior(img_t* im,
    ToolbarButton* pButton, IRECT rect,
    VisualManager::BCGBUTTON_STATE state)
{
  if (s->m_hThemeToolBar == NULL) {
    VisualManagerXP::OnFillButtonInterior(im, pButton, rect, state);
    return 0;
  }
  BOOL bIsMenuBar = FALSE;
  BOOL bIsPopupMenu = FALSE;
  ToolbarMenuButton* pMenuButton =
      DYNAMIC_DOWNCAST(ToolbarMenuButton, pButton);
  if (pMenuButton != NULL) {
    bIsMenuBar = pMenuButton->GetParentWnd() != NULL &&
        pMenuButton->GetParentWnd()->IsKindOf(RUNTIME_CLASS(MenuBar));
    bIsPopupMenu = pMenuButton->GetParentWnd() != NULL &&
        pMenuButton->GetParentWnd()->IsKindOf(RUNTIME_CLASS(PopupMenuBar));
  }
  if (s->m_bOfficeStyleMenus && (bIsPopupMenu || bIsMenuBar)) {
    VisualManagerXP::OnFillButtonInterior(im, pButton, rect, state);
    return 0;
  }
  if (bIsPopupMenu && state != ButtonsIsHighlighted &&
      state != ButtonsIsPressed) {
    return 0;
  }
  int nState = TS_NORMAL;
  if (pButton->s->m_nStyle & TBBS_DISABLED) {
    nState = TS_DISABLED;
  }
  else if ((pButton->s->m_nStyle & TBBS_PRESSED) && state == ButtonsIsHighlighted) {
    nState = TS_PRESSED;
  }
  else if (pButton->s->m_nStyle & TBBS_CHECKED) {
    nState = (state == ButtonsIsHighlighted) ? TS_HOTCHECKED : TS_CHECKED;
  }
  else if (state == ButtonsIsHighlighted) {
    nState = TS_HOT;
    if (pMenuButton != NULL && pMenuButton->IsDroppedDown()) {
      nState = TS_PRESSED;
      rect.b--;
    }
  }
  (*s->m_pfDrawThemeBackground)(s->m_hThemeToolBar, im, TP_BUTTON, nState, &rect, 0);
}
COLOR WinXPVisualManager::GetToolbarButtonTextColor(ToolbarButton* pButton,
    VisualManager::BCGBUTTON_STATE state)
{
  if (s->m_hThemeToolBar == NULL ||
      pButton->IsKindOf(RUNTIME_CLASS(OutlookButton))) {
    return VisualManagerXP::GetToolbarButtonTextColor(pButton, state);
  }
  return VisualManager::GetToolbarButtonTextColor(pButton, state);
}
int WinXPVisualManager::OnHighlightMenuItem(img_t* im, ToolbarMenuButton* pButton,
    IRECT rect, COLOR& clrText)
{
  if (s->m_hThemeWindow == NULL || s->m_bOfficeStyleMenus) {
    VisualManagerXP::OnHighlightMenuItem(im, pButton, rect, clrText);
    return 0;
  }
  if (s->m_hThemeMenu == NULL) {
    VisualManager::OnHighlightMenuItem(im, pButton, rect, clrText);
    return 0;
  }
  (*s->m_pfDrawThemeBackground)(s->m_hThemeMenu, im,
      MENU_POPUPITEM, MPI_HOT, &rect, 0);
  (*s->m_pfGetThemeColor)(s->m_hThemeMenu, MENU_POPUPITEM, MPI_HOT, TMT_TEXTCOLOR, &clrText);
}
COLOR WinXPVisualManager::GetHighlightedMenuItemTextColor(ToolbarMenuButton* pButton)
{
  if (s->m_hThemeMenu != NULL && !s->m_bOfficeStyleMenus) {
    COLOR clrText = 0;
    (*s->m_pfGetThemeColor)(s->m_hThemeMenu, MENU_POPUPITEM, 0, TMT_TEXTCOLOR, &clrText);
    return clrText;
  }
  if (s->m_hThemeWindow == NULL || s->m_bOfficeStyleMenus) {
    return VisualManagerXP::GetHighlightedMenuItemTextColor(pButton);
  }
  else {
    return VisualManager::GetHighlightedMenuItemTextColor(pButton);
  }
}
int WinXPVisualManager::OnHighlightRarelyUsedMenuItems(img_t* im, IRECT rectRarelyUsed)
{
  if (s->m_hThemeWindow == NULL || s->m_bOfficeStyleMenus) {
    VisualManagerXP::OnHighlightRarelyUsedMenuItems(im, rectRarelyUsed);
    return 0;
  }
  DrawManager dm(*im);
  rectRarelyUsed.l --;
  rectRarelyUsed.r = rectRarelyUsed.l + ToolBar::GetMenuImageSize().w +
      2 * GetMenuImageMargin() + 2;
  dm.HighlightRect(rectRarelyUsed, 94);
}
int WinXPVisualManager::OnFillMenuImageRect(img_t* im,
    ToolbarButton* pButton, IRECT rect, VisualManager::BCGBUTTON_STATE state)
{
  if (s->m_hThemeMenu == NULL || s->m_bOfficeStyleMenus) {
    VisualManagerXP::OnFillMenuImageRect(im, pButton, rect, state);
    return 0;
  }
  if (pButton->s->m_nStyle & TBBS_CHECKED) {
    (*s->m_pfDrawThemeBackground)(s->m_hThemeMenu, im,
        /*MENU_POPUPCHECKBACKGROUND*/12,
        2 /* MCB_NORMAL */, &rect, 0);
  }
}
int WinXPVisualManager::OnDrawButtonBorder(img_t* im,
    ToolbarButton* pButton, IRECT rect, BCGBUTTON_STATE state)
{
  if (s->m_hThemeToolBar == NULL) {
    VisualManagerXP::OnDrawButtonBorder(im, pButton, rect, state);
    return 0;
  }
  if (s->m_bOfficeStyleMenus) {
    ToolbarMenuButton* pMenuButton =
        DYNAMIC_DOWNCAST(ToolbarMenuButton, pButton);
    if (pMenuButton != NULL) {
      BOOL bIsMenuBar = pMenuButton->GetParentWnd() != NULL &&
          pMenuButton->GetParentWnd()->IsKindOf(RUNTIME_CLASS(MenuBar));
      BOOL bIsPopupMenu = pMenuButton->GetParentWnd() != NULL &&
          pMenuButton->GetParentWnd()->IsKindOf(RUNTIME_CLASS(PopupMenuBar));
      if (bIsPopupMenu || bIsMenuBar) {
        VisualManagerXP::OnDrawButtonBorder(im, pButton, rect, state);
        return 0;
      }
    }
  }
}
int WinXPVisualManager::OnDrawButtonSeparator(img_t* im,
    ToolbarButton* pButton, IRECT rect, VisualManager::BCGBUTTON_STATE state,
    BOOL bHorz)
{
  if (s->m_hThemeToolBar == NULL) {
    VisualManagerXP::OnDrawButtonSeparator(im, pButton, rect, state, bHorz);
    return 0;
  }
  rect.InflateRect(2, 2);
  (*s->m_pfDrawThemeBackground)(s->m_hThemeToolBar, im,
      bHorz ? TP_SEPARATOR : TP_SEPARATORVERT,
      0, &rect, 0);
}
int WinXPVisualManager::OnDrawSeparator(img_t* im, BaseControlBar* pBar,
    IRECT rect, BOOL bHorz)
{
  ASSERT_VALID(pBar);
  if (s->m_hThemeToolBar == NULL ||
      pBar->IsDialogControl()) {
    VisualManagerXP::OnDrawSeparator(im, pBar, rect, bHorz);
    return 0;
  }
  PopupMenuBar* pMenuBar = DYNAMIC_DOWNCAST(PopupMenuBar, pBar);
  if (pMenuBar != NULL) {
    if (s->m_bOfficeStyleMenus) {
      VisualManagerXP::OnDrawSeparator(im, pBar, rect, bHorz);
      return 0;
    }
    if (s->m_hThemeMenu != NULL) {
#ifndef BCGP_EXCLUDE_RIBBON
      RibbonPanelMenuBar* pRibbonMenuBar = DYNAMIC_DOWNCAST(RibbonPanelMenuBar, pBar);
      if (pRibbonMenuBar != NULL && pRibbonMenuBar->IsCategoryPopup()) {
        if (bHorz && rect.Width() < 2) {
          rect.r++;
        }
        (*s->m_pfDrawThemeBackground)(s->m_hThemeToolBar, im,
            bHorz ? TP_SEPARATOR : TP_SEPARATORVERT,
            0, &rect, 0);
        return 0;
      }
      if (pRibbonMenuBar == NULL || pRibbonMenuBar->IsMainPanel())
#endif
      {
        IRECT rectClient;
        pBar->GetClientRect(rectClient);
        rect.l = rectClient.l + pMenuBar->GetGutterWidth();
        rect.r = pMenuBar->IsRibbonPanel() ? rect.r : rectClient.r;
        rect.t = rect.CenterPoint().y - 3;
        rect.b = rect.CenterPoint().y + 3;
      }
      (*s->m_pfDrawThemeBackground)(s->m_hThemeMenu, im,
          /*MENU_POPUPSEPARATOR */15,
          0, &rect, 0);
      return 0;
    }
  }
#ifndef BCGP_EXCLUDE_RIBBON
  if (pBar->IsKindOf(RUNTIME_CLASS(RibbonStatusBar))) {
    IRECT rectClient;
    pBar->GetClientRect(rectClient);
    rect.t = rectClient.t;
    rect.b = rectClient.b;
  }
#endif
  if (bHorz && rect.Width() < 2) {
    rect.r++;
  }
  (*s->m_pfDrawThemeBackground)(s->m_hThemeToolBar, im,
      bHorz ? TP_SEPARATOR : TP_SEPARATORVERT,
      0, &rect, 0);
}
int WinXPVisualManager::OnDrawCaptionButton(img_t* im, CaptionButton* pButton,
    BOOL bActive, BOOL bHorz, BOOL bMaximized, BOOL bDisabled,
    int nImageID /*= -1*/)
{
  if (s->m_hThemeToolBar == NULL) {
    VisualManagerXP::OnDrawCaptionButton(im, pButton, bActive, bHorz, bMaximized, bDisabled, nImageID);
    return 0;
  }
  ASSERT(pButton != NULL);
  BOOL bPushed = pButton->s->m_bPushed;
  BOOL bFocused = pButton->s->m_bFocused || pButton->s->m_bDroppedDown;
  int nState = TS_NORMAL;
  if (bDisabled) {
    nState = TS_DISABLED;
  }
  else if (bPushed && bFocused) {
    nState = TS_PRESSED;
  }
  else if (bFocused) {
    nState = TS_HOT;
  }
  IRECT rect = pButton->GetRect();
  if (pButton->IsMiniFrameButton()) {
    rect.b -= 3;
    rect.r -= 2;
  }
  int nPart = 0;
  HTHEME hTheme = NULL;
  if (nImageID == -1) {
    switch (pButton->s->m_nHit) {
    case HTCLOSE:
    case HTCLOSE_BCG:
      if (pButton->IsMiniFrameButton() || s->bIsWindowsVista) {
        nPart = WP_SMALLCLOSEBUTTON;
        hTheme = s->m_hThemeWindow;
        if (!pButton->IsMiniFrameButton()) {
          rect.DeflateRect(1, 2);
        }
      }
      else {
        nPart = EBP_HEADERCLOSE;
        hTheme = s->m_hThemeExplorerBar;
      }
      break;
    case HTMAXBUTTON:
      if (!s->bIsWindowsVista) {
        nPart = EBP_HEADERPIN;
        hTheme = s->m_hThemeExplorerBar;
        if (!bMaximized) {
          nState += 3;
        }
      }
      break;
    }
  }
  if (nPart == 0 || hTheme == NULL) {
    if (!pButton->IsMiniFrameButton() && !s->bIsWindowsVista) {
      rect.DeflateRect(1, 2);
    }
    (*s->m_pfDrawThemeBackground)(s->m_hThemeToolBar, im, TP_BUTTON, nState, &rect, 0);
    MenuImages::IMAGES_IDS id = (MenuImages::IMAGES_IDS) - 1;
    if (nImageID != -1) {
      id = (MenuImages::IMAGES_IDS)nImageID;
    }
    else {
      id = pButton->GetIconID(bHorz, bMaximized);
    }
    if (id != (MenuImages::IMAGES_IDS) - 1) {
      CSize sizeImage = MenuImages::Size();
      CPoint ptImage(rect.l + (rect.Width() - sizeImage.w) / 2,
          rect.t + (rect.Height() - sizeImage.h) / 2);
      OnDrawCaptionButtonIcon(im, pButton, id, bActive, bDisabled, ptImage);
    }
  }
  else {
    (*s->m_pfDrawThemeBackground)(hTheme, im, nPart, nState, &rect, 0);
  }
}
int WinXPVisualManager::OnDrawCaptionButtonIcon(img_t* im,
    CaptionButton* pButton,
    MenuImages::IMAGES_IDS id,
    BOOL bActive, BOOL bDisabled,
    CPoint ptImage)
{
  BOOL bFocused = pButton->s->m_bFocused || pButton->s->m_bDroppedDown;
  if (s->m_hThemeButton == NULL || !bFocused) {
    VisualManagerXP::OnDrawCaptionButtonIcon(im, pButton, id, bActive,
        bDisabled, ptImage);
    return 0;
  }
  MenuImages::Draw(im, id, ptImage, MenuImages::ImageBlack);
}
COLOR WinXPVisualManager::OnFillCommandsListBackground(img_t* im, IRECT rect, BOOL bIsSelected)
{
  if (s->m_hThemeWindow == NULL || s->m_bOfficeStyleMenus) {
    return VisualManagerXP::OnFillCommandsListBackground(im, rect, bIsSelected);
  }
  ::FillRect(im, rect, GetSysColorBrush(COLOR_MENU));
  if (bIsSelected) {
    imdraw_FillRect(im, 0, rect, &s->clrHilite);
    imdraw_Draw3dRect(im, 0, rect, s->clrMenuText, s->clrMenuText);
    return s->clrTextHilite;
  }
  return s->clrMenuText;
}
int WinXPVisualManager::OnDrawTearOffCaption(img_t* im, IRECT rect, BOOL bIsActive)
{
  VisualManagerXP::OnDrawTearOffCaption(im, rect, bIsActive);
}
int WinXPVisualManager::OnDrawMenuSystemButton(img_t* im, IRECT rect,
    UINT uiSystemCommand,
    UINT nStyle, BOOL bHighlight)
{
  if (s->m_hThemeWindow == NULL) {
    VisualManagerXP::OnDrawMenuSystemButton(im, rect, uiSystemCommand, nStyle, bHighlight);
    return 0;
  }
  int nPart;
  switch (uiSystemCommand) {
  case SC_CLOSE:
    nPart = WP_MDICLOSEBUTTON;
    break;
  case SC_RESTORE:
    nPart = WP_MDIRESTOREBUTTON;
    break;
  case SC_MINIMIZE:
    nPart = WP_MDIMINBUTTON;
    break;
  default:
    return 0;
  }
  BOOL bIsDisabled = (nStyle & TBBS_DISABLED);
  BOOL bIsPressed = (nStyle & TBBS_PRESSED);
  int nState = CBS_NORMAL;
  if (bIsDisabled) {
    nState = CBS_DISABLED;
  }
  else if (bIsPressed && bHighlight) {
    nState = CBS_PUSHED;
  }
  else if (bHighlight) {
    nState = CBS_HOT;
  }
  (*s->m_pfDrawThemeBackground)(s->m_hThemeWindow, im, nPart,
      nState, &rect, 0);
}
int WinXPVisualManager::OnDrawStatusBarPaneBorder(img_t* im, StatusBar* pBar,
    IRECT rectPane, UINT uiID, UINT nStyle)
{
  if (s->m_hThemeStatusBar == NULL) {
    VisualManagerXP::OnDrawStatusBarPaneBorder(im, pBar, rectPane, uiID, nStyle);
    return 0;
  }
  if (!(nStyle & SBPS_NOBORDERS)) {
    (*s->m_pfDrawThemeBackground)(s->m_hThemeStatusBar, im, SP_PANE,
        0, &rectPane, 0);
  }
}
int WinXPVisualManager::OnDrawStatusBarSizeBox(img_t* im, StatusBar* pStatBar,
    IRECT rectSizeBox)
{
  if (s->m_hThemeScrollBar == NULL) {
    VisualManagerXP::OnDrawStatusBarSizeBox(im, pStatBar, rectSizeBox);
    return 0;
  }
  (*s->m_pfDrawThemeBackground)(s->m_hThemeScrollBar, im, SBP_SIZEBOX,
      SZB_RIGHTALIGN, &rectSizeBox, 0);
}
int WinXPVisualManager::OnDrawMenuBorder(img_t* im, PopupMenu* pMenu, IRECT rect)
{
  if (s->m_hThemeWindow == NULL || s->m_bOfficeStyleMenus) {
    ASSERT_VALID(pMenu);
    BOOL bConnectMenuToParent = s->m_bConnectMenuToParent;
    s->m_bConnectMenuToParent = FALSE;
    if (s->m_hThemeWindow == NULL) {
      s->m_bConnectMenuToParent = TRUE;
    }
    else if (!ToolBar::IsCustomizeMode()) {
      ToolbarMenuButton* pMenuButton = pMenu->GetParentButton();
      if (pMenuButton != NULL) {
        BOOL bIsMenuBar = pMenuButton->GetParentWnd() != NULL &&
            pMenuButton->GetParentWnd()->IsKindOf(RUNTIME_CLASS(MenuBar));
        if (bIsMenuBar) {
          s->m_bConnectMenuToParent = TRUE;
        }
      }
    }
    VisualManagerXP::OnDrawMenuBorder(im, pMenu, rect);
    s->m_bConnectMenuToParent = bConnectMenuToParent;
  }
  else {
    VisualManager::OnDrawMenuBorder(im, pMenu, rect);
  }
}
int WinXPVisualManager::OnDrawComboDropButton(img_t* im, IRECT rect,
    BOOL bDisabled,
    BOOL bIsDropped,
    BOOL bIsHighlighted,
    ToolbarComboBoxButton* pButton)
{
  if (s->m_hThemeComboBox == NULL) {
    VisualManagerXP::OnDrawComboDropButton(im, rect, bDisabled, bIsDropped, bIsHighlighted, pButton);
    return 0;
  }
  int nState = bDisabled ? CBXS_DISABLED : bIsDropped ? CBXS_PRESSED : bIsHighlighted ? CBXS_HOT : CBXS_NORMAL;
  (*s->m_pfDrawThemeBackground)(s->m_hThemeComboBox, im, CP_DROPDOWNBUTTON,
      nState, &rect, 0);
}
int WinXPVisualManager::OnDrawComboBorder(img_t* im, IRECT rect,
    BOOL bDisabled,
    BOOL bIsDropped,
    BOOL bIsHighlighted,
    ToolbarComboBoxButton* pButton)
{
  if (s->m_hThemeWindow == NULL) {
    VisualManagerXP::OnDrawComboBorder(im, rect, bDisabled, bIsDropped, bIsHighlighted, pButton);
    return 0;
  }
  if (bIsHighlighted || bIsDropped) {
    rect.DeflateRect(1, 1);
    if (ToolBarImages::s->m_bIsDrawOnGlass) {
      DrawManager dm(*im);
      dm.DrawRect(rect, (COLOR) - 1, s->clrHilite);
    }
    else {
      imdraw_Draw3dRect(im, 0, &rect, s->clrHilite, s->clrHilite);
    }
  }
}
int WinXPVisualManager::OnEraseTabsButton(img_t* im, IRECT rect,
    Button* pButton,
    BaseTabWnd* pWndTab)
{
  if (!s->m_b3DTabsXPTheme || s->m_hThemeTab == NULL || pWndTab->IsFlatTab() ||
      pWndTab->IsOneNoteStyle() || pWndTab->IsVS2005Style()) {
    VisualManagerXP::OnEraseTabsButton(im, rect, pButton, pWndTab);
    return 0;
  }
  CRgn rgn;
  rgn.CreateRectRgnIndirect(rect);
  im->SelectClipRgn(&rgn);
  IRECT rectTabs;
  pWndTab->GetClientRect(&rectTabs);
  IRECT rectTabArea;
  pWndTab->GetTabsRect(rectTabArea);
  if (pWndTab->GetLocation() == BaseTabWnd::LOCATION_BOTTOM) {
    rectTabs.t = rectTabArea.t;
    rectTabs.b += 2;
  }
  else {
    rectTabs.b = rectTabArea.b;
  }
  pWndTab->MapWindowPoints(pButton, rectTabs);
  OnEraseTabsArea(im, rectTabs, pWndTab);
  im->SelectClipRgn(NULL);
}
int WinXPVisualManager::OnDrawTabsButtonBorder(img_t* im, IRECT& rect,
    Button* pButton, UINT uiState,
    BaseTabWnd* pWndTab)
{
  if (s->m_hThemeToolBar == NULL) {
    VisualManagerXP::OnDrawTabsButtonBorder(im, rect, pButton, uiState, pWndTab);
    return 0;
  }
  int nState = TS_NORMAL;
  if (!pButton->IsWindowEnabled()) {
    nState = TS_DISABLED;
  }
  else if (pButton->IsPressed() || pButton->GetCheck()) {
    nState = TS_PRESSED;
  }
  else if (pButton->IsHighlighted()) {
    nState = TS_HOT;
  }
  s->DrawParentBackground(pButton, im, rect);
  (*s->m_pfDrawThemeBackground)(s->m_hThemeToolBar, im, TP_BUTTON, nState, &rect, 0);
}
COLOR WinXPVisualManager::OnFillMiniFrameCaption(
    img_t* im, IRECT rectCaption,
    MiniFrameWnd* pFrameWnd,
    BOOL bActive)
{
  if (s->m_hThemeWindow == NULL) {
    return VisualManager::OnFillMiniFrameCaption(im, rectCaption, pFrameWnd, bActive);
  }
  return s->clrCaptionText;
}
int WinXPVisualManager::OnDrawMiniFrameBorder(
    img_t* im, MiniFrameWnd* pFrameWnd,
    IRECT rectBorder, IRECT rectBorderSize)
{
  if (s->m_hThemeWindow == NULL) {
    VisualManager::OnDrawMiniFrameBorder(im, pFrameWnd, rectBorder, rectBorderSize);
    return 0;
  }
  (*s->m_pfDrawThemeBackground)(s->m_hThemeWindow, im,
      WP_SMALLCAPTION,
      0, &rectBorder, 0);
  imdraw_Draw3dRect(im, 0, rectBorder, s->clrBarFace, s->clrBarDkShadow);
  rectBorder.DeflateRect(1, 1);
  imdraw_Draw3dRect(im, 0, rectBorder, s->clrBarHilite, s->clrBarShadow);
}
int WinXPVisualManager::OnDrawFloatingToolbarBorder(
    img_t* im, BaseToolBar* pToolBar,
    IRECT rectBorder, IRECT rectBorderSize)
{
  if (s->m_hThemeWindow == NULL) {
    VisualManager::OnDrawFloatingToolbarBorder(im, pToolBar,
        rectBorder, rectBorderSize);
    return 0;
  }
  (*s->m_pfDrawThemeBackground)(s->m_hThemeWindow, im,
      WP_SMALLCAPTION,
      0, &rectBorder, 0);
  imdraw_Draw3dRect(im, 0, rectBorder, s->clrBarFace, s->clrBarDkShadow);
  rectBorder.DeflateRect(1, 1);
  imdraw_Draw3dRect(im, 0, rectBorder, s->clrBarHilite, s->clrBarShadow);
}
int WinXPVisualManager::OnFillOutlookPageButton(img_t* im, const IRECT& rectClient,
    BOOL bIsHighlighted, BOOL bIsPressed,
    COLOR& clrText)
{
  if (s->m_hThemeButton == NULL) {
    VisualManagerXP::OnFillOutlookPageButton(im, rectClient, bIsHighlighted, bIsPressed, clrText);
    return 0;
  }
  int nState = PBS_NORMAL;
  if (bIsHighlighted) {
    nState = PBS_PRESSED;
  }
  else if (bIsPressed) {
    nState = PBS_HOT;
  }
  IRECT rect = rectClient;
  rect.InflateRect(1, 1);
  (*s->m_pfDrawThemeBackground)(s->m_hThemeButton, im, BP_PUSHBUTTON,
      nState, &rect, 0);
}
int WinXPVisualManager::OnDrawOutlookPageButtonBorder(img_t* im,
    IRECT& rectBtn, BOOL bIsHighlighted, BOOL bIsPressed)
{
  if (s->m_hThemeButton == NULL) {
    VisualManagerXP::OnDrawOutlookPageButtonBorder(im, rectBtn, bIsHighlighted, bIsPressed);
  }
}
int WinXPVisualManager::OnDrawStatusBarProgress(img_t* im, StatusBar* pStatusBar,
    IRECT rectProgress, int nProgressTotal, int nProgressCurr,
    COLOR clrBar, COLOR clrProgressBarDest, COLOR clrProgressText,
    BOOL bProgressText)
{
  if (!DrawStatusBarProgress(im, pStatusBar,
      rectProgress, nProgressTotal, nProgressCurr,
      clrBar, clrProgressBarDest, clrProgressText, bProgressText)) {
    VisualManagerXP::OnDrawStatusBarProgress(im, pStatusBar,
        rectProgress, nProgressTotal, nProgressCurr,
        clrBar, clrProgressBarDest, clrProgressText, bProgressText);
  }
}
CSize WinXPVisualManager::GetButtonExtraBorder() const
{
  if (s->m_hThemeWindow == NULL) {
    return VisualManagerXP::GetButtonExtraBorder();
  }
  return CSize(2, 2);
}
CSize WinXPVisualManager::GetCaptionButtonExtraBorder() const
{
  if (s->m_hThemeWindow == NULL) {
    return VisualManagerXP::GetCaptionButtonExtraBorder();
  }
  return CSize(1, 1);
}
int WinXPVisualManager::OnDrawHeaderCtrlBorder(HeaderCtrl* pCtrl, img_t* im,
    IRECT& rect, BOOL bIsPressed, BOOL bIsHighlighted)
{
  if (s->m_hThemeHeader == NULL) {
    VisualManagerXP::OnDrawHeaderCtrlBorder(pCtrl, im, rect, bIsPressed, bIsHighlighted);
    return 0;
  }
  int nState = HIS_NORMAL;
  if (bIsPressed) {
    nState = HIS_PRESSED;
  }
  else if (bIsHighlighted) {
    nState = HIS_HOT;
  }
  (*s->m_pfDrawThemeBackground)(s->m_hThemeHeader, im,
      HP_HEADERITEM, nState, &rect, 0);
}
int WinXPVisualManager::OnDrawHeaderCtrlSortArrow(HeaderCtrl* pCtrl,
    img_t* im,
    IRECT& rect, BOOL bIsUp)
{
  if (s->m_hThemeHeader == NULL) {
    VisualManagerXP::OnDrawHeaderCtrlSortArrow(pCtrl, im, rect, bIsUp);
    return 0;
  }
  /* TODO
  int nState = bIsUp ? HSAS_SORTEDUP : HSAS_SORTEDDOWN;
  (*s->m_pfDrawThemeBackground)(s->m_hThemeHeader, im,
  HP_HEADERSORTARROW, nState, &rect, 0);
  */
#define POINTS_NUM 3
  IPOINT pts [POINTS_NUM];
  if (bIsUp) {
    pts [0].x = rect.l;
    pts [0].y = rect.b;
    pts [1].x = rect.CenterPoint().x;
    pts [1].y = rect.t;
    pts [2].x = rect.r;
    pts [2].y = rect.b;
  }
  else {
    pts [0].x = rect.l;
    pts [0].y = rect.t;
    pts [1].x = rect.CenterPoint().x;
    pts [1].y = rect.b;
    pts [2].x = rect.r;
    pts [2].y = rect.t;
  }
  CBrush br(s->clrBtnShadow);
  COLOR pOldBrush = im->SelectObject(&br);
  CPen* pOldPen = (CPen*) im->SelectStockObject(NULL_PEN);
  im->Polygon(pts, POINTS_NUM);
  im->SelectObject(pOldBrush);
  im->SelectObject(pOldPen);
}
int WinXPVisualManager::OnDrawEditBorder(img_t* im, IRECT rect,
    BOOL bDisabled,
    BOOL bIsHighlighted,
    ToolbarEditBoxButton* pButton)
{
  if (s->m_hThemeComboBox == NULL) {
    VisualManagerXP::OnDrawEditBorder(im, rect, bDisabled, bIsHighlighted, pButton);
    return 0;
  }
  if (!pButton->IsFlatMode()) {
    COLOR clrBorder = (COLOR) - 1;
    if ((*s->m_pfGetThemeColor)(s->m_hThemeComboBox, 5, 0, TMT_BORDERCOLOR, &clrBorder) != S_OK) {
      VisualManagerXP::OnDrawEditBorder(im, rect, bDisabled, bIsHighlighted, pButton);
      return 0;
    }
    if (ToolBarImages::s->m_bIsDrawOnGlass) {
      DrawManager dm(*im);
      dm.DrawRect(rect, (COLOR) - 1, clrBorder);
    }
    else {
      imdraw_Draw3dRect(im, 0, &rect, clrBorder, clrBorder);
    }
    return 0;
  }
  if (bIsHighlighted) {
    if (ToolBarImages::s->m_bIsDrawOnGlass) {
      DrawManager dm(*im);
      dm.DrawRect(rect, (COLOR) - 1, s->clrBtnHilite);
    }
    else {
      imdraw_Draw3dRect(im, 0, &rect, s->clrBtnHilite, s->clrBtnHilite);
    }
  }
}
#ifndef BCGP_EXCLUDE_TASK_PANE
int WinXPVisualManager::OnFillTasksPaneBackground(img_t* im, IRECT rectWorkArea)
{
  if (s->m_hThemeExplorerBar == NULL) {
    VisualManagerXP::OnFillTasksPaneBackground(im, rectWorkArea);
    return 0;
  }
  (*s->m_pfDrawThemeBackground)(s->m_hThemeExplorerBar, im, EBP_HEADERBACKGROUND,
      0, &rectWorkArea, 0);
}
int WinXPVisualManager::OnDrawTasksGroupCaption(img_t* im, TasksGroup* pGroup,
    BOOL bIsHighlighted, BOOL bIsSelected, BOOL bCanCollapse)
{
  ASSERT(pGroup != NULL);
  ASSERT_VALID(pGroup->s->m_pPage);
#ifndef BCGP_EXCLUDE_TOOLBOX
  BOOL bIsToolBox = pGroup->s->m_pPage->s->m_pTaskPane != NULL &&
      (pGroup->s->m_pPage->s->m_pTaskPane->IsKindOf(RUNTIME_CLASS(ToolBoxEx)));
#else
  BOOL bIsToolBox = FALSE;
#endif
  if (s->m_hThemeExplorerBar == NULL || bIsToolBox) {
    VisualManagerXP::OnDrawTasksGroupCaption(im, pGroup, bIsHighlighted, bIsSelected, bCanCollapse);
    return 0;
  }
  if (pGroup->s->m_strName.IsEmpty()) {
    return 0;
  }
  // -------------------------------
  // Draw group caption(Windows XP)
  // -------------------------------
  if (pGroup->s->m_bIsSpecial) {
    (*s->m_pfDrawThemeBackground)(s->m_hThemeExplorerBar, im, EBP_SPECIALGROUPHEAD,
        0, &pGroup->s->m_rect, 0);
  }
  else {
    (*s->m_pfDrawThemeBackground)(s->m_hThemeExplorerBar, im, EBP_NORMALGROUPHEAD,
        0, &pGroup->s->m_rect, 0);
  }
  // ---------------------------
  // Draw an icon if it presents
  // ---------------------------
  BOOL bShowIcon = (pGroup->s->m_hIcon != NULL
      && pGroup->s->m_sizeIcon.w < pGroup->s->m_rect.Width() - pGroup->s->m_rect.Height());
  if (bShowIcon) {
    OnDrawTasksGroupIcon(im, pGroup, 5, bIsHighlighted, bIsSelected, bCanCollapse);
  }
  // -----------------------
  // Draw group caption text
  // -----------------------
  CFont* pFontOld = im->SelectObject(s->fontBold);
  COLOR clrTextOld = im->GetTextColor();
  if (bCanCollapse && bIsHighlighted) {
    if (pGroup->s->m_bIsSpecial) {
      im->SetTextColor(pGroup->s->m_clrTextHot == (COLOR) - 1 ?
          s->clrWindow : pGroup->s->m_clrTextHot);
    }
    else {
      im->SetTextColor(pGroup->s->m_clrTextHot == (COLOR) - 1 ?
          s->m_clrTaskPaneHotText : pGroup->s->m_clrTextHot);
    }
  }
  else {
    if (pGroup->s->m_bIsSpecial) {
      im->SetTextColor(pGroup->s->m_clrText == (COLOR) - 1 ?
          s->clrWindow : pGroup->s->m_clrText);
    }
    else {
      im->SetTextColor(pGroup->s->m_clrText == (COLOR) - 1 ?
          s->m_clrTaskPaneHotText : pGroup->s->m_clrText);
    }
  }
  int nBkModeOld = im->SetBkMode(TRANSPARENT);
  int nTaskPaneHOffset = pGroup->s->m_pPage->s->m_pTaskPane->GetGroupCaptionHorzOffset();
  int nTaskPaneVOffset = pGroup->s->m_pPage->s->m_pTaskPane->GetGroupCaptionVertOffset();
  int nCaptionHOffset = (nTaskPaneHOffset != -1 ? nTaskPaneHOffset : s->m_nGroupCaptionHorzOffset);
  IRECT rectText = pGroup->s->m_rect;
  rectText.l += (bShowIcon ? pGroup->s->m_sizeIcon.w + 5 : nCaptionHOffset);
  rectText.t += (nTaskPaneVOffset != -1 ? nTaskPaneVOffset : s->m_nGroupCaptionVertOffset);
  rectText.r = max(rectText.l,
      rectText.r - (bCanCollapse ? pGroup->s->m_rect.Height() : nCaptionHOffset));
  im->DrawText(pGroup->s->m_strName, rectText, DT_SINGLELINE | DT_END_ELLIPSIS);
  im->SetBkMode(nBkModeOld);
  im->SelectObject(pFontOld);
  im->SetTextColor(clrTextOld);
  // -------------------------
  // Draw group caption button
  // -------------------------
  if (bCanCollapse) {
    IRECT rectButton = pGroup->s->m_rect;
    rectButton.l = max(rectButton.l, rectButton.r - rectButton.Height());
    if (pGroup->s->m_bIsSpecial) {
      if (!pGroup->s->m_bIsCollapsed) {
        if (bIsHighlighted) {
          (*s->m_pfDrawThemeBackground)(s->m_hThemeExplorerBar, im,
              EBP_SPECIALGROUPCOLLAPSE, EBSGC_HOT, &rectButton, 0);
        }
        else {
          (*s->m_pfDrawThemeBackground)(s->m_hThemeExplorerBar, im,
              EBP_SPECIALGROUPCOLLAPSE, EBSGC_NORMAL, &rectButton, 0);
        }
      }
      else {
        if (bIsHighlighted) {
          (*s->m_pfDrawThemeBackground)(s->m_hThemeExplorerBar, im,
              EBP_SPECIALGROUPEXPAND, EBSGE_HOT, &rectButton, 0);
        }
        else {
          (*s->m_pfDrawThemeBackground)(s->m_hThemeExplorerBar, im,
              EBP_SPECIALGROUPEXPAND, EBSGE_NORMAL, &rectButton, 0);
        }
      }
    }
    else {
      if (!pGroup->s->m_bIsCollapsed) {
        if (bIsHighlighted) {
          (*s->m_pfDrawThemeBackground)(s->m_hThemeExplorerBar, im,
              EBP_NORMALGROUPCOLLAPSE, EBNGC_HOT, &rectButton, 0);
        }
        else {
          (*s->m_pfDrawThemeBackground)(s->m_hThemeExplorerBar, im,
              EBP_NORMALGROUPCOLLAPSE, EBNGC_NORMAL, &rectButton, 0);
        }
      }
      else {
        if (bIsHighlighted) {
          (*s->m_pfDrawThemeBackground)(s->m_hThemeExplorerBar, im,
              EBP_NORMALGROUPEXPAND, EBNGE_HOT, &rectButton, 0);
        }
        else {
          (*s->m_pfDrawThemeBackground)(s->m_hThemeExplorerBar, im,
              EBP_NORMALGROUPEXPAND, EBNGE_NORMAL, &rectButton, 0);
        }
      }
    }
  }
}
int WinXPVisualManager::OnFillTasksGroupInterior(img_t* im, IRECT rect, BOOL bSpecial)
{
  if (s->m_hThemeExplorerBar == NULL) {
    VisualManagerXP::OnFillTasksGroupInterior(im, rect);
    return 0;
  }
  if (!bSpecial) {
    (*s->m_pfDrawThemeBackground)(s->m_hThemeExplorerBar, im,
        EBP_NORMALGROUPBACKGROUND, 0, &rect, 0);
  }
  else {
    (*s->m_pfDrawThemeBackground)(s->m_hThemeExplorerBar, im,
        EBP_SPECIALGROUPBACKGROUND, 0, &rect, 0);
  }
}
int WinXPVisualManager::OnDrawTasksGroupAreaBorder(img_t* im, IRECT rect, BOOL bSpecial,
    BOOL bNoTitle)
{
  if (s->m_hThemeExplorerBar == NULL) {
    VisualManagerXP::OnDrawTasksGroupAreaBorder(im, rect, bSpecial, bNoTitle);
    return 0;
  }
  // Draw underline
  if (bNoTitle) {
    IRECT rectDraw = rect;
    rectDraw.b = rectDraw.t + 1;
    if (bSpecial) {
      (*s->m_pfDrawThemeBackground)(s->m_hThemeExplorerBar, im, EBP_SPECIALGROUPHEAD,
          0, &rectDraw, 0);
    }
    else {
      (*s->m_pfDrawThemeBackground)(s->m_hThemeExplorerBar, im, EBP_NORMALGROUPHEAD,
          0, &rectDraw, 0);
    }
  }
  return 0;
}
int WinXPVisualManager::OnDrawTask(img_t* im, Task* pTask, CImageList* pIcons,
    BOOL bIsHighlighted, BOOL bIsSelected)
{
  ASSERT_VALID(pIcons);
  ASSERT(pTask != NULL);
  if (s->m_hThemeExplorerBar == NULL) {
    VisualManagerXP::OnDrawTask(im, pTask, pIcons, bIsHighlighted, bIsSelected);
    return 0;
  }
  if (pTask->s->m_bIsSeparator) {
    // --------------
    // Draw separator
    // --------------
    IRECT rectDraw = pTask->s->m_rect;
    rectDraw.t = pTask->s->m_rect.CenterPoint().y;
    rectDraw.b = rectDraw.t + 1;
    // draw same as group caption
    (*s->m_pfDrawThemeBackground)(s->m_hThemeExplorerBar, im, EBP_NORMALGROUPHEAD,
        0, &rectDraw, 0);
    return 0;
  }
  // ---------
  // Draw icon
  // ---------
  IRECT rectText = pTask->s->m_rect;
  CSize sizeIcon(0, 0);
  ::ImageList_GetIconSize(pIcons->m_hImageList, (int*) &sizeIcon.w, (int*) &sizeIcon.h);
  if (pTask->s->m_nIcon >= 0 && sizeIcon.w > 0) {
    pIcons->Draw(im, pTask->s->m_nIcon, rectText.TopLeft(), ILD_TRANSPARENT);
  }
  int nTaskPaneOffset = pTask->s->m_pGroup->s->m_pPage->s->m_pTaskPane->GetTasksIconHorzOffset();
  rectText.l += sizeIcon.w + (nTaskPaneOffset != -1 ? nTaskPaneOffset : s->m_nTasksIconHorzOffset);
  // ---------
  // Draw text
  // ---------
  BOOL bIsLabel = (pTask->s->m_uiCommandID == 0);
  CFont* pFontOld = NULL;
  COLOR clrTextOld = im->GetTextColor();
  if (bIsLabel) {
    pFontOld = im->SelectObject(
        pTask->s->m_bIsBold ? &s->fontBold : &s->fontRegular);
    im->SetTextColor(pTask->s->m_clrText == (COLOR) - 1 ?
        s->clrWindowText : pTask->s->m_clrText);
  }
  else if (!pTask->s->m_bEnabled) {
    im->SetTextColor(s->clrGrayedText);
    pFontOld = im->SelectObject(s->fontRegular);
  }
  else if (bIsHighlighted) {
    pFontOld = im->SelectObject(s->fontUnderline);
    im->SetTextColor(pTask->s->m_clrTextHot == (COLOR) - 1 ?
        s->m_clrTaskPaneHotText : pTask->s->m_clrTextHot);
  }
  else {
    pFontOld = im->SelectObject(s->fontRegular);
    im->SetTextColor(pTask->s->m_clrText == (COLOR) - 1 ?
        s->m_clrTaskPaneHotText : pTask->s->m_clrText);
  }
  int nBkModeOld = im->SetBkMode(TRANSPARENT);
  TasksPane* pTaskPane = pTask->s->m_pGroup->s->m_pPage->s->m_pTaskPane;
  ASSERT_VALID(pTaskPane);
  BOOL bMultiline = bIsLabel ?
      pTaskPane->IsWrapLabelsEnabled() : pTaskPane->IsWrapTasksEnabled();
  if (bMultiline) {
    im->DrawText(pTask->s->m_strName, rectText, DT_WORDBREAK);
  }
  else {
    CString strText = pTask->s->m_strName;
    strText.Remove(('\n'));
    strText.Remove(('\r'));
    im->DrawText(strText, rectText, DT_SINGLELINE | DT_VCENTER);
  }
  im->SetBkMode(nBkModeOld);
  im->SelectObject(pFontOld);
  im->SetTextColor(clrTextOld);
}
int WinXPVisualManager::OnDrawScrollButtons(img_t* im, const IRECT& rect, const int nBorderSize,
    int iImage, BOOL bHilited)
{
  IRECT rectImage(CPoint(0, 0), MenuImages::Size());
  IRECT rectFill = rect;
  rectFill.t -= nBorderSize;
  imdraw_FillRect(im, 0, rectFill, &s->clrBarFace);
  if (bHilited) {
    DrawManager dm(*im);
    dm.HighlightRect(rect);
    imdraw_Draw3dRect(im, 0, rect,
        s->clrBarHilite,
        s->clrBarDkShadow);
  }
  MenuImages::Draw(im, (MenuImages::IMAGES_IDS) iImage, rect);
}
#endif // BCGP_EXCLUDE_TASK_PANE
int WinXPVisualManager::OnDrawExpandingBox(img_t* im, IRECT rect, BOOL bIsOpened, COLOR colorBox)
{
  if (s->m_hThemeTree == NULL) {
    VisualManagerXP::OnDrawExpandingBox(im, rect, bIsOpened, colorBox);
    return 0;
  }
  (*s->m_pfDrawThemeBackground)(s->m_hThemeTree, im, TVP_GLYPH,
      bIsOpened ? GLPS_OPENED : GLPS_CLOSED, &rect, 0);
}
int WinXPVisualManager::OnDrawCheckBoxEx(CDC* im, IRECT rect,
    int nState,
    BOOL bHighlighted,
    BOOL bPressed,
    BOOL bEnabled)
{
  if (!DrawCheckBox(im, rect, bHighlighted, nState, bEnabled, bPressed)) {
    VisualManagerXP::OnDrawCheckBoxEx(im, rect, nState, bHighlighted, bPressed, bEnabled);
  }
}
int WinXPVisualManager::OnDrawRadioButton(CDC* im, IRECT rect,
    BOOL bOn,
    BOOL bHighlighted,
    BOOL bPressed,
    BOOL bEnabled)
{
  if (!DrawRadioButton(im, rect, bHighlighted, bOn, bEnabled, bPressed)) {
    VisualManagerXP::OnDrawRadioButton(im, rect, bOn, bHighlighted, bPressed, bEnabled);
  }
}
int WinXPVisualManager::OnDrawControlBorder(img_t* im, IRECT rect, CWnd* pWndCtrl, BOOL bDrawOnGlass)
{
  if (s->m_hThemeComboBox == NULL) {
    VisualManagerXP::OnDrawControlBorder(im, rect, pWndCtrl, bDrawOnGlass);
    return 0;
  }
  COLOR clrBorder = (COLOR) - 1;
  if ((*s->m_pfGetThemeColor)(s->m_hThemeComboBox, 5, 0, TMT_BORDERCOLOR, &clrBorder) != S_OK) {
    VisualManagerXP::OnDrawControlBorder(im, rect, pWndCtrl, bDrawOnGlass);
    return 0;
  }
  if (bDrawOnGlass) {
    DrawManager dm(*im);
    dm.DrawRect(rect, (COLOR) - 1, clrBorder);
    rect.DeflateRect(1, 1);
    dm.DrawRect(rect, (COLOR) - 1, s->clrWindow);
  }
  else {
    imdraw_Draw3dRect(im, 0, &rect, clrBorder, clrBorder);
    rect.DeflateRect(1, 1);
    imdraw_Draw3dRect(im, 0, rect, s->clrWindow, s->clrWindow);
  }
}
#ifndef BCGP_EXCLUDE_GRID_CTRL
int WinXPVisualManager::OnFillGridSelectAllAreaBackground(GridCtrl* pCtrl, img_t* im, IRECT rect, BOOL /*bPressed*/)
{
  OnFillGridHeaderBackground(pCtrl, im, rect);
}
BOOL WinXPVisualManager::OnDrawGridSelectAllAreaBorder(GridCtrl* pCtrl, img_t* im, IRECT rect, BOOL bPressed)
{
  return OnDrawGridHeaderItemBorder(pCtrl, im, rect, bPressed);
}
#endif // BCGP_EXCLUDE_GRID_CTRL
#if !defined(BCGP_EXCLUDE_GRID_CTRL) && !defined(BCGP_EXCLUDE_GANTT)
int WinXPVisualManager::DrawGanttHeaderCell(const GanttChart* pChart, CDC& dc, const BCGP_GANTT_CHART_HEADER_CELL_INFO& cellInfo, BOOL bHilite)
{
  if (s->m_hThemeHeader == NULL) {
    VisualManagerXP::DrawGanttHeaderCell(pChart, dc, cellInfo, bHilite);
    return 0;
  }
  (*s->m_pfDrawThemeBackground)(s->m_hThemeHeader, dc.GetSafeHdc(), HP_HEADERITEM, HIS_NORMAL, cellInfo.rectCell, 0);
}
int WinXPVisualManager::FillGanttBar(const GanttItem* pItem, CDC& dc, const IRECT& rectFill, COLOR color, double dGlowLine)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManager::FillGanttBar(pItem, dc, rectFill, color, dGlowLine);
    return 0;
  }
  dGlowLine = min(1.0, dGlowLine);
  dGlowLine = max(0.0, dGlowLine);
  IRECT rectPart = rectFill;
  int h = rectFill.Height();
  rectPart.b = (LONG)(rectFill.t + dGlowLine * h);
  DrawManager dm(dc);
  dm.FillGradient(rectPart, DrawManager::MixColors(color, _RGB(255, 255, 255), 0.3f), DrawManager::MixColors(color, _RGB(255, 255, 255), 0.15f), TRUE);
  rectPart.t = rectPart.b;
  rectPart.b = rectFill.b;
  dm.FillGradient(rectPart, DrawManager::MixColors(color, _RGB(0, 0, 0), 0.2f), color, TRUE);
}
#endif // !defined(BCGP_EXCLUDE_GRID_CTRL) && !defined(BCGP_EXCLUDE_GANTT)
int WinXPVisualManager::GetCalendarColors(const Calendar* pCalendar,
    CalendarColors& colors)
{
  VisualManagerXP::GetCalendarColors(pCalendar, colors);
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s) ||
      s->m_clrCalendarCaption == (COLOR) - 1 ||
      s->m_clrCalendarCaptionText == (COLOR) - 1) {
    return 0;
  }
  colors.clrCaption = s->m_clrCalendarCaption;
  colors.clrCaptionText = s->m_clrCalendarCaptionText;
}
COLOR WinXPVisualManager::OnDrawControlBarCaption(img_t* im, DockingControlBar* pBar,
    BOOL bActive, IRECT rectCaption, IRECT rectButtons)
{
  if (s->m_hThemeExplorerBar == NULL || s->m_pfGetThemeColor == NULL) {
    return VisualManagerXP::OnDrawControlBarCaption(im, pBar,
        bActive, rectCaption, rectButtons);
  }
  if (s->bIsWindowsVista) {
    DrawManager dm(*im);
    dm.FillGradient(rectCaption,
        bActive ? s->clrActiveCaptionGradient : s->clrInactiveCaptionGradient,
        bActive ? s->clrActiveCaption : s->clrInactiveCaption,
        TRUE);
    return bActive ? s->clrCaptionText : s->clrInactiveCaptionText;
  }
  (*s->m_pfDrawThemeBackground)(s->m_hThemeExplorerBar, im,
      EBP_HEADERBACKGROUND,
      0, &rectCaption, 0);
  COLOR clrText;
  if ((*s->m_pfGetThemeColor)(s->m_hThemeExplorerBar, EBP_HEADERBACKGROUND, 0, TMT_TEXTCOLOR, &clrText) != S_OK) {
    clrText = s->clrInactiveCaptionText;
  }
  if (bActive) {
    DrawManager dm(*im);
    int nPercentage = 110;
    if (GetRV(clrText) > 128 &&
        GetGV(clrText) > 128 &&
        GetBV(clrText) > 128) {
      nPercentage = 80;
    }
    dm.HighlightRect(rectCaption, nPercentage);
  }
  return clrText;
}
#ifndef BCGP_EXCLUDE_TOOLBOX
BOOL WinXPVisualManager::OnEraseToolBoxButton(img_t* im, IRECT rect,
    ToolBoxButton* pButton)
{
  if (s->m_hThemeToolBar == NULL) {
    return VisualManagerXP::OnEraseToolBoxButton(im, rect, pButton);
  }
  if (pButton->IsHighlighted() || pButton->GetCheck()) {
    int nState = TS_NORMAL;
    if (pButton->GetCheck()) {
      nState = (pButton->IsHighlighted()) ? TS_HOTCHECKED : TS_CHECKED;
    }
    else if (pButton->IsHighlighted()) {
      nState = TS_HOT;
    }
    (*s->m_pfDrawThemeBackground)(s->m_hThemeToolBar, im, TP_BUTTON, nState, &rect, 0);
  }
  return TRUE;
}
BOOL WinXPVisualManager::OnDrawToolBoxButtonBorder(img_t* im, IRECT& rect,
    ToolBoxButton* pButton, UINT uiState)
{
  if (s->m_hThemeToolBar == NULL) {
    return VisualManagerXP::OnDrawToolBoxButtonBorder(im, rect, pButton, uiState);
  }
  return TRUE;
}
#endif
BOOL WinXPVisualManager::OnDrawPushButton(img_t* im, IRECT rect, Button* pButton, COLOR& clrText)
{
  clrText = s->clrBtnText;
  return DrawPushButton(im, rect, pButton, 0);
}
#ifndef BCGP_EXCLUDE_RIBBON
int WinXPVisualManager::OnDrawRibbonProgressBar(img_t* im,
    RibbonProgressBar* pProgress,
    IRECT rectProgress, IRECT rectChunk,
    BOOL bInfiniteMode)
{
#define PP_BAR 1
#define PP_CHUNK 3
  if (s->m_hThemeProgress != NULL) {
    (*s->m_pfDrawThemeBackground)(s->m_hThemeProgress, im,
        PP_BAR, 0, &rectProgress, 0);
    if (!rectChunk.IsRectEmpty()) {
      rectChunk.DeflateRect(2, 2);
      (*s->m_pfDrawThemeBackground)(s->m_hThemeProgress, im,
          PP_CHUNK, 0, &rectChunk, 0);
    }
  }
  else {
    VisualManagerXP::OnDrawRibbonProgressBar(im, pProgress,
        rectProgress, rectChunk, bInfiniteMode);
  }
}
#endif
//
// BCGPVisualManagerScenic.cpp: implementation of the VisualManagerScenic class.
//
//////////////////////////////////////////////////////////////////////
//#include "BCGCBPro.h"
#include "bcgglobals.h"
#include "BCGCBProVer.h"
#include "BCGPVisualManagerScenic.h"
#include "BCGPDrawManager.h"
#include "BCGPTagManager.h"
#include "BCGPToolbarMenuButton.h"
#include "BCGPGlobalUtils.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPFrameWnd.h"
#include "CustomizeButton.h"
#include "BCGPToolbarComboBoxButton.h"
#ifndef BCGP_EXCLUDE_RIBBON
#include "BCGPRibbonBar.h"
#include "BCGPRibbonPanel.h"
#include "BCGPRibbonCategory.h"
#include "BCGPRibbonButton.h"
#include "BCGPRibbonQuickAccessToolbar.h"
#include "BCGPRibbonComboBox.h"
#include "BCGPRibbonMainPanel.h"
#include "BCGPRibbonPanelMenu.h"
#include "BCGPRibbonLabel.h"
#include "BCGPRibbonPaletteButton.h"
#include "BCGPRibbonStatusBar.h"
#include "BCGPRibbonStatusBarPane.h"
#include "BCGPRibbonProgressBar.h"
#include "BCGPRibbonHyperlink.h"
#include "BCGPRibbonSlider.h"
#endif
IMPLEMENT_DYNCREATE(VisualManagerScenic, WinXPVisualManager)
#define RT_BCG_STYLE_XML ("STYLE_XML")
CString VisualManagerScenic::s->m_strStylePrefix;
HINSTANCE VisualManagerScenic::s->m_hinstRes = NULL;
BOOL VisualManagerScenic::s->m_bAutoFreeRes = FALSE;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
VisualManagerScenic::VisualManagerScenic()
  : s->m_bLoaded(FALSE)
{
}
VisualManagerScenic::~VisualManagerScenic()
{
}
CString VisualManagerScenic::MakeResourceID(LPCTSTR lpszID)
{
  CString strResID(lpszID);
  ASSERT(!strResID.IsEmpty());
  if (!s->m_strStylePrefix.IsEmpty()) {
    strResID = s->m_strStylePrefix + strResID;
  }
  return strResID;
}
CString VisualManagerScenic::GetStyleResourceID()
{
  CString strResID(("IDX_STYLE"));
#if !defined _AFXDLL || defined _BCGCBPRO_STATIC_
  strResID = ("SCENIC_") + strResID;
#endif
  return strResID;
};
int VisualManagerScenic::SetResourceHandle(HINSTANCE hinstRes)
{
  s->m_bAutoFreeRes = FALSE;
  if (s->m_hinstRes != hinstRes) {
    s->m_hinstRes = hinstRes;
    if (VisualManager::GetInstance()->IsKindOf(RUNTIME_CLASS(VisualManagerScenic))) {
      VisualManager::GetInstance()->OnUpdateSystemColors();
    }
  }
}
BOOL VisualManagerScenic::SetStyle(LPCTSTR lpszPath)
{
  if (s->m_hinstRes > (HINSTANCE) 32) {
    return TRUE;
  }
#if defined _AFXDLL && !defined _BCGCBPRO_STATIC_
  CString strVer;
  strVer.Format(("%d%d"), _BCGCBPRO_VERSION_MAJOR, _BCGCBPRO_VERSION_MINOR);
  CString strStyleDLLName = ("BCGPStyleScenic") + strVer + (".dll");
  CString strStyleDLLPath;
  if (lpszPath != NULL && _tcslen(lpszPath) > 0) {
    strStyleDLLPath = lpszPath;
    if (strStyleDLLPath [strStyleDLLPath.GetLength() - 1] != ('\\')) {
      strStyleDLLPath += ('\\');
    }
    strStyleDLLPath += strStyleDLLName;
  }
  else {
    strStyleDLLPath = strStyleDLLName;
  }
  CleanStyle();
  HINSTANCE hinstRes = LoadLibrary(strStyleDLLPath);
  if (hinstRes <= (HINSTANCE) 32) {
    TRACE(("Cannot load Style DLL: %s\r\n"), strStyleDLLPath);
    ASSERT(FALSE);
    return FALSE;
  }
  SetResourceHandle(hinstRes);
  s->m_bAutoFreeRes = TRUE;
#else
  UNREFERENCED_PARAMETER(lpszPath);
  CString strStyle(GetStyleResourceID());
  HINSTANCE hinstRes = AfxFindResourceHandle(strStyle, RT_BCG_STYLE_XML);
  if (::FindResource(hinstRes, strStyle, RT_BCG_STYLE_XML) == NULL) {
    TRACE(("Cannot load Style: %s\r\n"), strStyle);
    ASSERT(FALSE);
    return FALSE;
  }
  CleanStyle();
  SetResourceHandle(hinstRes);
#endif
  return TRUE;
}
int VisualManagerScenic::CleanStyle()
{
  if (s->m_bAutoFreeRes && s->m_hinstRes > (HINSTANCE) 32) {
    ::FreeLibrary(s->m_hinstRes);
  }
  s->m_hinstRes = NULL;
  s->m_strStylePrefix.Empty();
}
int VisualManagerScenic::DrawSeparator(img_t* im, const IRECT& rect, CPen& pen1, CPen& pen2, BOOL bHorz)
{
  IRECT rect1(rect);
  IRECT rect2;
  if (bHorz) {
    rect1.t += rect.Height() / 2 - 1;
    rect1.b = rect1.t;
    rect2 = rect1;
    rect2.OffsetRect(0, 1);
  }
  else {
    rect1.l += rect.Width() / 2 - 1;
    rect1.r = rect1.l;
    rect2 = rect1;
    rect2.OffsetRect(1, 0);
  }
  if (ToolBarImages::s->m_bIsDrawOnGlass) {
    DrawManager dm(*im);
    LOGPEN logpen;
    pen1.GetLogPen(&logpen);
    dm.DrawLine(rect1.l, rect1.t, rect1.r, rect1.b, logpen.lopnColor);
    pen2.GetLogPen(&logpen);
    dm.DrawLine(rect2.l, rect2.t, rect2.r, rect2.b, logpen.lopnColor);
  }
  else {
    CPen* pOldPen = im->SelectObject(&pen1);
    im->MoveTo(rect1.TopLeft());
    im->LineTo(rect1.BottomRight());
    im->SelectObject(&pen2);
    im->MoveTo(rect2.TopLeft());
    im->LineTo(rect2.BottomRight());
    im->SelectObject(pOldPen);
  }
}
int VisualManagerScenic::DrawNcBtn(img_t* im, const IRECT& rect, UINT nButton,
    BCGBUTTON_STATE state, BOOL bSmall,
    BOOL bActive, BOOL bMDI/* = FALSE*/)
{
  if (s->m_hThemeWindow == NULL) {
    UINT nState = 0;
    switch (nButton) {
    case SC_CLOSE:
      nState = DFCS_CAPTIONCLOSE;
      break;
    case SC_MINIMIZE:
      nState = DFCS_CAPTIONMIN;
      break;
    case SC_MAXIMIZE:
      nState = DFCS_CAPTIONMAX;
      break;
    case SC_RESTORE:
      nState = DFCS_CAPTIONRESTORE;
      break;
    case SC_CONTEXTHELP:
      nState = DFCS_CAPTIONHELP;
      break;
    default:
      return 0;
    }
    if (!bActive) {
      nState |= DFCS_INACTIVE;
    }
    if (state != ButtonsIsRegular) {
      nState |= state == ButtonsIsHighlighted ? 0x1000/*DFCS_HOT*/ : DFCS_PUSHED;
    }
    IRECT rt(rect);
    im->DrawFrameControl(rt, DFC_CAPTION, nState);
    return 0;
  }
  int nPart = 0;
  int nState = 0;
  if (nButton == SC_CLOSE) {
    if (bMDI) {
      nPart = /*WP_MDICLOSEBUTTON*/20;
    }
    else {
      nPart = bSmall ? /*WP_SMALLCLOSEBUTTON*/19 : /*WP_CLOSEBUTTON*/18;
    }
    nState = bActive ? /*CBS_NORMAL*/1 : 5;
    if (state != ButtonsIsRegular) {
      nState = state == ButtonsIsHighlighted ? /*CBS_HOT*/2 : /*CBS_PUSHED*/3;
    }
  }
  else if (nButton == SC_MINIMIZE) {
    if (bMDI) {
      nPart = /*WP_MDIMINBUTTON*/16;
    }
    else if (!bSmall) {
      nPart = /*WP_MINBUTTON*/15;
    }
    nState = bActive ? /*MINBS_NORMAL*/1 : 5;
    if (state != ButtonsIsRegular) {
      nState = state == ButtonsIsHighlighted ? /*MINBS_HOT*/2 : /*MINBS_PUSHED*/3;
    }
  }
  else if (nButton == SC_MAXIMIZE) {
    if (!bMDI && !bSmall) {
      nPart = /*WP_MAXBUTTON*/17;
    }
    nState = bActive ? /*MAXBS_NORMAL*/1 : 5;
    if (state != ButtonsIsRegular) {
      nState = state == ButtonsIsHighlighted ? /*MAXBS_HOT*/2 : /*MAXBS_PUSHED*/3;
    }
  }
  else if (nButton == SC_RESTORE) {
    if (bMDI) {
      nPart = /*WP_MDIRESTOREBUTTON*/22;
    }
    else {
      nPart = /*WP_RESTOREBUTTON*/21;
    }
    nState = bActive ? /*RBS_NORMAL*/1 : 5;
    if (state != ButtonsIsRegular) {
      nState = state == ButtonsIsHighlighted ? /*RBS_HOT*/2 : /*RBS_PUSHED*/3;
    }
  }
  else if (nButton == SC_CONTEXTHELP) {
    if (bMDI) {
      nPart = /*WP_MDIHELPBUTTON*/24;
    }
    else if (!bSmall) {
      nPart = /*WP_HELPBUTTON*/23;
    }
    nState = bActive ? /*HBS_NORMAL*/1 : /*HBS_DISABLED*/4;
    if (state != ButtonsIsRegular) {
      nState = state == ButtonsIsHighlighted ? /*HBS_HOT*/2 : /*HBS_PUSHED*/3;
    }
  }
  if (nPart == 0) {
    return 0;
  }
  (*s->m_pfDrawThemeBackground)(s->m_hThemeWindow, im, nPart, nState, &rect, 0);
}
int VisualManagerScenic::DrawNcText(img_t* im, IRECT& rect,
    const CString& strTitle,
    BOOL bActive, BOOL bIsRTL,
    BOOL bTextCenter,
    BOOL bGlass/* = FALSE*/, int nGlassGlowSize/* = 0*/,
    COLOR clrGlassText/* =(COLOR)-1*/)
{
  if (strTitle.IsEmpty() || rect.r <= rect.l) {
    return 0;
  }
  int nOldMode = im->SetBkMode(TRANSPARENT);
  COLOR clrOldText = im->GetTextColor();
  DWORD dwTextStyle = DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER |
      (bIsRTL ? DT_RTLREADING : 0);
  COLOR clrText = bActive
      ? s->clrCaptionText
      : s->clrInactiveCaptionText;
  int widthFull = rect.Width();
  int width = im->GetTextExtent(strTitle).w;
  if (bTextCenter && width < widthFull) {
    rect.l += (widthFull - width) / 2;
  }
  rect.r = min(rect.l + width, rect.r);
  if (rect.r > rect.l) {
    if (bGlass) {
      DrawTextOnGlass(im, strTitle, rect, dwTextStyle, nGlassGlowSize, clrGlassText);
    }
    else {
      im->SetTextColor(clrText);
      im->DrawText(strTitle, rect, dwTextStyle);
    }
  }
  im->SetBkMode(nOldMode);
  im->SetTextColor(clrOldText);
}
BOOL VisualManagerScenic::IsOwnerDrawMenuCheck()
{
  return CanDrawImage() ? FALSE : WinXPVisualManager::IsOwnerDrawMenuCheck();
}
BOOL VisualManagerScenic::IsHighlightWholeMenuItem()
{
  return CanDrawImage() ? TRUE : WinXPVisualManager::IsHighlightWholeMenuItem();
}
int VisualManagerScenic::CleanUp()
{
  s->m_AppCaptionFont.DeleteObject();
  s->m_penSeparatorDark.DeleteObject();
  s->m_penSeparatorLight.DeleteObject();
  s->m_clrRibbonBarBkgnd.DeleteObject();
  s->m_ctrlMenuItemBack.CleanUp();
  s->m_MenuItemMarkerC.Clear();
  s->m_MenuItemMarkerR.Clear();
  s->m_ctrlMenuHighlighted[0].CleanUp();
  s->m_ctrlMenuHighlighted[1].CleanUp();
  s->m_ctrlRibbonComboBoxBtn.CleanUp();
  s->m_ctrlRibbonCaptionQA.CleanUp();
  s->m_ctrlRibbonCategoryBack.CleanUp();
  s->m_ctrlRibbonCategoryTab.CleanUp();
  s->m_ctrlRibbonCategoryTabSep.CleanUp();
  s->m_ctrlRibbonPanelBackSep.CleanUp();
  s->m_ctrlRibbonMainPanel.CleanUp();
  s->m_ctrlRibbonBtnMainPanel.CleanUp();
  s->m_ctrlRibbonBtnGroup_S.CleanUp();
  s->m_ctrlRibbonBtnGroup_F.CleanUp();
  s->m_ctrlRibbonBtnGroup_M.CleanUp();
  s->m_ctrlRibbonBtnGroup_L.CleanUp();
  s->m_ctrlRibbonBtnGroupMenu_F[0].CleanUp();
  s->m_ctrlRibbonBtnGroupMenu_F[1].CleanUp();
  s->m_ctrlRibbonBtnGroupMenu_M[0].CleanUp();
  s->m_ctrlRibbonBtnGroupMenu_M[1].CleanUp();
  s->m_ctrlRibbonBtnGroupMenu_L[0].CleanUp();
  s->m_ctrlRibbonBtnGroupMenu_L[1].CleanUp();
  s->m_ctrlRibbonBtn[0].CleanUp();
  s->m_ctrlRibbonBtn[1].CleanUp();
  s->m_ctrlRibbonBtnMenuH[0].CleanUp();
  s->m_ctrlRibbonBtnMenuH[1].CleanUp();
  s->m_ctrlRibbonBtnMenuV[0].CleanUp();
  s->m_ctrlRibbonBtnMenuV[1].CleanUp();
  s->m_ctrlRibbonBtnLaunch.CleanUp();
  s->m_RibbonBtnLaunchIcon.Clear();
  s->m_ctrlRibbonBtnMain.CleanUp();
  s->m_ctrlRibbonSliderBtnPlus.CleanUp();
  s->m_ctrlRibbonSliderBtnMinus.CleanUp();
  s->m_RibbonBtnDefaultImage.Clear();
  s->m_ctrlRibbonBtnDefault.CleanUp();
  s->m_ctrlRibbonBtnDefaultIcon.CleanUp();
  s->m_ctrlRibbonBtnDefaultQAT.CleanUp();
  s->m_ctrlRibbonBtnStatusPane.CleanUp();
  s->m_ctrlRibbonBtnPalette[0].CleanUp();
  s->m_ctrlRibbonBtnPalette[1].CleanUp();
  s->m_ctrlRibbonBtnPalette[2].CleanUp();
  s->m_ctrlRibbonBorder_QAT.CleanUp();
  s->m_cacheRibbonCategoryBack.Clear();
  s->m_cacheRibbonBtnGroup_S.Clear();
  s->m_cacheRibbonBtnGroup_F.Clear();
  s->m_cacheRibbonBtnGroup_M.Clear();
  s->m_cacheRibbonBtnGroup_L.Clear();
  s->m_cacheRibbonBtnGroupMenu_F[0].Clear();
  s->m_cacheRibbonBtnGroupMenu_M[0].Clear();
  s->m_cacheRibbonBtnGroupMenu_L[0].Clear();
  s->m_cacheRibbonBtnGroupMenu_F[1].Clear();
  s->m_cacheRibbonBtnGroupMenu_M[1].Clear();
  s->m_cacheRibbonBtnGroupMenu_L[1].Clear();
  s->m_cacheRibbonBtnDefault.Clear();
  s->m_ActivateFlag.RemoveAll();
  s->m_bLoaded = FALSE;
}
int VisualManagerScenic::OnUpdateSystemColors()
{
  HINSTANCE hinstResOld = NULL;
  if (s->m_hinstRes == NULL) {
    SetStyle();
    return 0;
  }
  if (s->m_hinstRes != NULL) {
    hinstResOld = AfxGetResourceHandle();
    AfxSetResourceHandle(s->m_hinstRes);
  }
  CleanUp();
  WinXPVisualManager::OnUpdateSystemColors();
  TagManager tm;
  if (!tm.LoadFromResource(GetStyleResourceID(), RT_BCG_STYLE_XML)) {
#if !defined _AFXDLL
    TRACE(("\r\nImportant: to enable the Scenic look in static link,\r\n"));
    TRACE(("include afxribbon.rc from the RC file in your project.\r\n\r\n"));
    ASSERT(FALSE);
#endif
    if (hinstResOld != NULL) {
      AfxSetResourceHandle(hinstResOld);
    }
    return 0;
  }
  {
    CString strStyle;
    tm.ExcludeTag(("STYLE"), strStyle);
    tm.SetBuffer(strStyle);
  }
  CString strItem;
  if (!tm.IsEmpty()) {
    int nVersion = 0;
    if (tm.ExcludeTag(("VERSION"), strItem)) {
      TagManager tmItem(strItem);
      tmItem.ReadInt(("NUMBER"), nVersion);
      int nType = 20;
      if (nVersion == 2007) {
        tmItem.ReadInt(("TYPE"), nType);
        s->m_bLoaded = TRUE;
      }
      if (s->m_bLoaded) {
        if (tmItem.ExcludeTag(("ID_PREFIX"), strItem)) {
          strItem.TrimLeft();
          strItem.TrimRight();
          s->m_strStylePrefix = strItem;
        }
      }
    }
  }
  if (!s->m_bLoaded) {
    if (hinstResOld != NULL) {
      ::AfxSetResourceHandle(hinstResOld);
    }
    return 0;
  }
  // menu
  if (tm.ExcludeTag(("MENU"), strItem)) {
    TagManager tmItem(strItem);
    tmItem.ReadInt(("BorderSize"), s->m_nMenuBorderSize);
    tmItem.ReadControlRenderer(("ItemBack"), s->m_ctrlMenuItemBack, MakeResourceID(("IDB_MENU_ITEs->m_BACK")));
    tmItem.ReadToolBarImages(("ItemCheck"), s->m_MenuItemMarkerC, MakeResourceID(("IDB_MENU_ITEs->m_MARKER_C")));
    tmItem.ReadToolBarImages(("ItemRadio"), s->m_MenuItemMarkerR, MakeResourceID(("IDB_MENU_ITEs->m_MARKER_R")));
    tmItem.ReadControlRenderer(("Highlighted"), s->m_ctrlMenuHighlighted[0], MakeResourceID(("IDB_MENU_BTN")));
    tmItem.ReadControlRenderer(("HighlightedDisabled"), s->m_ctrlMenuHighlighted[1], MakeResourceID(("IDB_MENU_BTN_DISABLED")));
  }
  s->m_clrMenuRarelyUsed.DeleteObject();
  s->m_clrMenuRarelyUsed.CreateSolidBrush(s->m_clrMenuRarelyUsed);
  s->m_clrRibbonEdit = s->clrWindow;
  s->m_clrRibbonEditHighlighted = s->clrWindow;
  s->m_clrRibbonEditPressed = s->m_clrRibbonEditHighlighted;
  s->m_clrRibbonEditDisabled = s->clrBtnFace;
  s->m_clrRibbonEditBorder = s->clrWindow;
  s->m_clrRibbonEditBorderDisabled = s->clrBtnShadow;
  s->m_clrRibbonEditBorderHighlighted = s->m_clrMenuItemBorder;
  s->m_clrRibbonEditBorderPressed = s->m_clrRibbonEditBorderHighlighted;
  s->m_clrRibbonEditSelection = s->clrHilite;
  s->m_clrRibbonBarBkgnd = s->clrBarFace;
  s->m_clrRibbonBarGradientLight = s->clrBarLight;
  s->m_clrRibbonBarGradientDark = s->m_clrRibbonBarGradientLight;
  // bars
  if (tm.ExcludeTag(("BARS"), strItem)) {
    TagManager tmItem(strItem);
    CString strBar;
    if (tmItem.ExcludeTag(("RIBBONBAR"), strBar)) {
      TagManager tmBar(strBar);
      tmBar.ReadColor(("Bkgnd"), s->m_clrRibbonBarBkgnd);
      tmBar.ReadColor(("GradientLight"), s->m_clrRibbonBarGradientLight);
      s->m_clrRibbonBarGradientDark = s->m_clrRibbonBarGradientLight;
      tmBar.ReadColor(("GradientDark"), s->m_clrRibbonBarGradientDark);
    }
  }
  s->m_clrRibbonBarBkgnd.DeleteObject();
  s->m_clrRibbonBarBkgnd.CreateSolidBrush(s->m_clrRibbonBarBkgnd);
  if (tm.ExcludeTag(("RIBBON"), strItem)) {
    TagManager tmItem(strItem);
    CString str;
    if (tmItem.ExcludeTag(("CATEGORY"), str)) {
      TagManager tmCategory(str);
      tmCategory.ReadControlRenderer(("BACK"), s->m_ctrlRibbonCategoryBack, MakeResourceID(("IDB_RIBBON_CATEGORY_BACK")));
      CString strTab;
      if (tmCategory.ExcludeTag(("TAB"), strTab)) {
        TagManager tmTab(strTab);
        tmTab.ReadControlRenderer(("BUTTON"), s->m_ctrlRibbonCategoryTab, MakeResourceID(("IDB_RIBBON_CATEGORY_TAB")));
      }
      tmCategory.ReadControlRenderer(("TAB_SEPARATOR"), s->m_ctrlRibbonCategoryTabSep, MakeResourceID(("IDB_RIBBON_CATEGORY_TAB_SEP")));
      // tmCategory.ReadControlRenderer(("BUTTON_PAGE_L"), s->m_ctrlRibbonCategoryBtnPage[0], MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_PAGE_L")));
      // tmCategory.ReadControlRenderer(("BUTTON_PAGE_R"), s->m_ctrlRibbonCategoryBtnPage[1], MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_PAGE_R")));
    }
    if (tmItem.ExcludeTag(("PANEL"), str)) {
      TagManager tmPanel(str);
      {
        CString strBack;
        if (tmPanel.ExcludeTag(("BACK"), strBack)) {
          TagManager tmBack(strBack);
          tmBack.ReadControlRenderer(("SEPARATOR"), s->m_ctrlRibbonPanelBackSep, MakeResourceID(("IDB_RIBBON_PANEL_BACK_SEP")));
        }
      }
      {
        CString strCaption;
        if (tmPanel.ExcludeTag(("CAPTION"), strCaption)) {
          TagManager tmCaption(strCaption);
          tmCaption.ReadControlRenderer(("LAUNCH_BTN"), s->m_ctrlRibbonBtnLaunch, MakeResourceID(("IDB_RIBBON_BTN_LAUNCH")));
          tmCaption.ReadToolBarImages(("LAUNCH_ICON"), s->m_RibbonBtnLaunchIcon, MakeResourceID(("IDB_RIBBON_BTN_LAUNCH_ICON")));
          tmCaption.ReadColor(("TextNormal"), s->m_clrRibbonPanelCaptionText);
          tmCaption.ReadColor(("TextHighlighted"), s->m_clrRibbonPanelCaptionTextHighlighted);
        }
      }
      {
        CString strButtons;
        if (tmPanel.ExcludeTag(("BUTTONS"), strButtons)) {
          TagManager tmButtons(strButtons);
          tmButtons.ReadControlRenderer(("BUTTON_GROUP_F"), s->m_ctrlRibbonBtnGroup_F, MakeResourceID(("IDB_RIBBON_BTN_GROUP_F")));
          tmButtons.ReadControlRenderer(("BUTTON_GROUP_M"), s->m_ctrlRibbonBtnGroup_M, MakeResourceID(("IDB_RIBBON_BTN_GROUP_M")));
          tmButtons.ReadControlRenderer(("BUTTON_GROUP_L"), s->m_ctrlRibbonBtnGroup_L, MakeResourceID(("IDB_RIBBON_BTN_GROUP_L")));
          tmButtons.ReadControlRenderer(("BUTTON_GROUP_S"), s->m_ctrlRibbonBtnGroup_S, MakeResourceID(("IDB_RIBBON_BTN_GROUP_S")));
          tmButtons.ReadControlRenderer(("BUTTON_GROUPMENU_F_C"), s->m_ctrlRibbonBtnGroupMenu_F[0], MakeResourceID(("IDB_RIBBON_BTN_GROUPMENU_F_C")));
          tmButtons.ReadControlRenderer(("BUTTON_GROUPMENU_F_M"), s->m_ctrlRibbonBtnGroupMenu_F[1], MakeResourceID(("IDB_RIBBON_BTN_GROUPMENU_F_M")));
          tmButtons.ReadControlRenderer(("BUTTON_GROUPMENU_s->m_C"), s->m_ctrlRibbonBtnGroupMenu_M[0], MakeResourceID(("IDB_RIBBON_BTN_GROUPMENU_s->m_C")));
          tmButtons.ReadControlRenderer(("BUTTON_GROUPMENU_s->m_M"), s->m_ctrlRibbonBtnGroupMenu_M[1], MakeResourceID(("IDB_RIBBON_BTN_GROUPMENU_s->m_M")));
          tmButtons.ReadControlRenderer(("BUTTON_GROUPMENU_L_C"), s->m_ctrlRibbonBtnGroupMenu_L[0], MakeResourceID(("IDB_RIBBON_BTN_GROUPMENU_L_C")));
          tmButtons.ReadControlRenderer(("BUTTON_GROUPMENU_L_M"), s->m_ctrlRibbonBtnGroupMenu_L[1], MakeResourceID(("IDB_RIBBON_BTN_GROUPMENU_L_M")));
          tmButtons.ReadControlRenderer(("BUTTON_NORMAL_S"), s->m_ctrlRibbonBtn[0], MakeResourceID(("IDB_RIBBON_BTN_NORMAL_S")));
          tmButtons.ReadControlRenderer(("BUTTON_NORMAL_B"), s->m_ctrlRibbonBtn[1], MakeResourceID(("IDB_RIBBON_BTN_NORMAL_B")));
          tmButtons.ReadControlRenderer(("BUTTON_DEFAULT"), s->m_ctrlRibbonBtnDefault, MakeResourceID(("IDB_RIBBON_BTN_DEFAULT")));
          tmButtons.ReadControlRenderer(("BUTTON_DEFAULT_ICON"), s->m_ctrlRibbonBtnDefaultIcon, MakeResourceID(("IDB_RIBBON_BTN_DEFAULT_ICON")));
          tmButtons.ReadToolBarImages(("BUTTON_DEFAULT_IMAGE"), s->m_RibbonBtnDefaultImage, MakeResourceID(("IDB_RIBBON_BTN_DEFAULT_IMAGE")));
          tmButtons.ReadControlRenderer(("BUTTON_DEFAULT_QAT"), s->m_ctrlRibbonBtnDefaultQAT, MakeResourceID(("IDB_RIBBON_BTN_DEFAULT_QAT")));
          tmButtons.ReadControlRenderer(("BUTTON_MENU_H_C"), s->m_ctrlRibbonBtnMenuH[0], MakeResourceID(("IDB_RIBBON_BTN_MENU_H_C")));
          tmButtons.ReadControlRenderer(("BUTTON_MENU_H_M"), s->m_ctrlRibbonBtnMenuH[1], MakeResourceID(("IDB_RIBBON_BTN_MENU_H_M")));
          tmButtons.ReadControlRenderer(("BUTTON_MENU_V_C"), s->m_ctrlRibbonBtnMenuV[0], MakeResourceID(("IDB_RIBBON_BTN_MENU_V_C")));
          tmButtons.ReadControlRenderer(("BUTTON_MENU_V_M"), s->m_ctrlRibbonBtnMenuV[1], MakeResourceID(("IDB_RIBBON_BTN_MENU_V_M")));
          tmButtons.ReadControlRenderer(("BUTTON_PNL_T"), s->m_ctrlRibbonBtnPalette[0], MakeResourceID(("IDB_RIBBON_BTN_PALETTE_T")));
          tmButtons.ReadControlRenderer(("BUTTON_PNL_M"), s->m_ctrlRibbonBtnPalette[1], MakeResourceID(("IDB_RIBBON_BTN_PALETTE_M")));
          tmButtons.ReadControlRenderer(("BUTTON_PNL_B"), s->m_ctrlRibbonBtnPalette[2], MakeResourceID(("IDB_RIBBON_BTN_PALETTE_B")));
        }
      }
      {
        CString strEdit;
        if (tmPanel.ExcludeTag(("EDIT"), strEdit)) {
          TagManager tmEdit(strEdit);
          tmEdit.ReadColor(("Normal"), s->m_clrRibbonEdit);
          tmEdit.ReadColor(("Highlighted"), s->m_clrRibbonEditHighlighted);
          tmEdit.ReadColor(("Disabled"), s->m_clrRibbonEditDisabled);
          tmEdit.ReadColor(("Pressed"), s->m_clrRibbonEditPressed);
          tmEdit.ReadColor(("BorderNormal"), s->m_clrRibbonEditBorder);
          tmEdit.ReadColor(("BorderHighlighted"), s->m_clrRibbonEditBorderHighlighted);
          tmEdit.ReadColor(("BorderDisabled"), s->m_clrRibbonEditBorderDisabled);
          tmEdit.ReadColor(("BorderPressed"), s->m_clrRibbonEditBorderPressed);
          tmEdit.ReadColor(("Selection"), s->m_clrRibbonEditSelection);
          CString strButton;
          if (tmEdit.ExcludeTag(("BUTTON"), strButton)) {
            TagManager tmButton(strButton);
            tmButton.ReadControlRenderer(("IMAGE"), s->m_ctrlRibbonComboBoxBtn, MakeResourceID(("IDB_COMBOBOX_BTN")));
          }
        }
      }
    }
    tmItem.ReadControlRenderer(("MAIN_BUTTON"), s->m_ctrlRibbonBtnMain, MakeResourceID(("IDB_RIBBON_BTN_MAIN")));
    if (tmItem.ExcludeTag(("MAIN"), str)) {
      TagManager tmMain(str);
      tmMain.ReadControlRenderer(("BACK"), s->m_ctrlRibbonMainPanel, MakeResourceID(("IDB_RIBBON_PANEL_MAIN")));
      tmMain.ReadControlRenderer(("BUTTON"), s->m_ctrlRibbonBtnMainPanel, MakeResourceID(("IDB_RIBBON_BTN_PANEL_MAIN")));
    }
    if (tmItem.ExcludeTag(("CAPTION"), str)) {
      TagManager tmCaption(str);
      tmCaption.ReadControlRenderer(("QA"), s->m_ctrlRibbonCaptionQA, MakeResourceID(("IDB_RIBBON_CAPTION_QA")));
    }
    if (tmItem.ExcludeTag(("STATUS"), str)) {
      TagManager tmStatus(str);
      tmStatus.ReadControlRenderer(("PANE_BUTTON"), s->m_ctrlRibbonBtnStatusPane, MakeResourceID(("IDB_RIBBON_BTN_STATUS_PANE")));
      CString strSlider;
      if (tmStatus.ExcludeTag(("SLIDER"), strSlider)) {
        TagManager tmSlider(strSlider);
        tmSlider.ReadControlRenderer(("PLUS"), s->m_ctrlRibbonSliderBtnPlus, MakeResourceID(("IDB_RIBBON_SLIDER_BTN_PLUS")));
        tmSlider.ReadControlRenderer(("MINUS"), s->m_ctrlRibbonSliderBtnMinus, MakeResourceID(("IDB_RIBBON_SLIDER_BTN_MINUS")));
      }
    }
    if (tmItem.ExcludeTag(("BORDERS"), str)) {
      TagManager tmBorders(str);
      tmBorders.ReadControlRenderer(("QAT"), s->m_ctrlRibbonBorder_QAT, MakeResourceID(("IDB_RIBBON_BORDER_QAT")));
      //tmBorders.ReadControlRenderer(("FLOATY"), s->m_ctrlRibbonBorder_Floaty, MakeResourceID(("IDB_OFFICE2007_RIBBON_BORDER_FLOATY")));
    }
  }
  if (hinstResOld != NULL) {
    AfxSetResourceHandle(hinstResOld);
  }
  NONCLIENTMETRICS ncm;
  s->GetNonClientMetrics(ncm);
  s->m_AppCaptionFont.CreateFontIndirect(&ncm.lfCaptionFont);
  s->m_penSeparatorDark.CreatePen(PS_SOLID, 0, _RGB(198, 212, 227));
  s->m_penSeparatorLight.CreatePen(PS_SOLID, 0, _RGB(237, 243, 248));
}
int VisualManagerScenic::OnDrawMenuBorder(img_t* im, PopupMenu* pMenu, IRECT rect)
{
  if (!CanDrawImage()) {
    WinXPVisualManager::OnDrawMenuBorder(im, pMenu, rect);
    return 0;
  }
#ifndef BCGP_EXCLUDE_RIBBON
  if (pMenu != NULL) {
    RibbonPanelMenuBar* pRibbonMenuBar =
        DYNAMIC_DOWNCAST(RibbonPanelMenuBar, pMenu->GetMenuBar());
    if (pRibbonMenuBar != NULL) {
      ASSERT_VALID(pRibbonMenuBar);
      if (pRibbonMenuBar->IsMainPanel()) {
        if (s->m_ctrlRibbonMainPanel.IsValid()) {
          s->m_ctrlRibbonMainPanel.DrawFrame(im, rect);
        }
        return 0;
      }
      if (!pRibbonMenuBar->IsMenuMode()) {
        if (pRibbonMenuBar->IsQATPopup() &&
            s->m_ctrlRibbonBorder_QAT.IsValid()) {
          s->m_ctrlRibbonBorder_QAT.DrawFrame(im, rect);
          return 0;
        }
        else if (pRibbonMenuBar->IsCategoryPopup()) {
          return 0;
        }
        else if (pRibbonMenuBar->IsFloaty() /*&&
 s->m_ctrlRibbonBorder_Floaty.IsValid()*/) {
          /*
          s->m_ctrlRibbonBorder_Floaty.DrawFrame(im, rect);
          return 0;
          */
        }
        else {
          if (pRibbonMenuBar->GetPanel() != NULL) {
            return 0;
          }
        }
      }
    }
  }
#endif
  WinXPVisualManager::OnDrawMenuBorder(im, pMenu, rect);
}
int VisualManagerScenic::OnDrawMenuCheck(img_t* im, ToolbarMenuButton* pButton,
    IRECT rect, BOOL bHighlight, BOOL bIsRadio)
{
  ToolBarImages& img = bIsRadio ? s->m_MenuItemMarkerR : s->m_MenuItemMarkerC;
  if (!CanDrawImage() || img.GetCount() == 0) {
    WinXPVisualManager::OnDrawMenuCheck(im, pButton, rect, bHighlight, bIsRadio);
    return 0;
  }
  CSize size(img.GetImageSize());
  IRECT rectImage(0, 0, size.w, size.h);
  if ((pButton->s->m_nStyle & TBBS_DISABLED) == TBBS_DISABLED) {
    rectImage.OffsetRect(0, size.h);
  }
  if (s->m_bIsRTL) {
    img.Mirror();
  }
  img.DrawEx(im, rect, 0, ToolBarImages::ImageAlignHorzCenter, ImageAlignVertCenter, rectImage);
  if (s->m_bIsRTL) {
    img.Mirror();
  }
}
int VisualManagerScenic::OnHighlightMenuItem(CDC* im, ToolbarMenuButton* pButton,
    IRECT rect, COLOR& clrText)
{
  if (!CanDrawImage()) {
    WinXPVisualManager::OnHighlightMenuItem(im, pButton, rect, clrText);
    return 0;
  }
  clrText = s->clrMenuText;
  s->m_ctrlMenuHighlighted[(pButton->s->m_nStyle & TBBS_DISABLED) == TBBS_DISABLED ? 1 : 0].Draw(im, rect);
}
#ifndef BCGP_EXCLUDE_RIBBON
int VisualManagerScenic::OnDrawRibbonCaption(img_t* im, RibbonBar* pBar,
    IRECT rectCaption, IRECT rectText)
{
  ASSERT_VALID(pBar);
  if (!CanDrawImage()) {
    WinXPVisualManager::OnDrawRibbonCaption(im, pBar, rectCaption, rectText);
    return 0;
  }
  CWnd* pWnd = pBar->GetParent();
  ASSERT_VALID(pWnd);
  //const DWORD dwStyle = pWnd->GetStyle();
  const DWORD dwStyleEx = pWnd->GetExStyle();
  const BOOL bIsRTL = (dwStyleEx & WS_EX_LAYOUTRTL) == WS_EX_LAYOUTRTL;
  const BOOL bActive = IsWindowActive(pWnd);
  const BOOL bGlass = pBar->IsTransparentCaption();
  {
    CSize szSysBorder(GetSystemBorders(TRUE));
    if (!bGlass) {
      IRECT rectCaption1(rectCaption);
      rectCaption1.InflateRect(szSysBorder.w, szSysBorder.h, szSysBorder.w, 0);
      if (s->m_hThemeWindow != NULL) {
        (*s->m_pfDrawThemeBackground)(s->m_hThemeWindow, im,
            /*WP_CAPTION*/1, bActive ? /*CS_ACTIVE*/1 : /*CS_INACTIVE*/2, &rectCaption1, 0);
      }
      else {
        DrawManager dm(*im);
        dm.FillGradient(rectCaption1,
            bActive ? s->clrActiveCaption : s->clrInactiveCaption,
            bActive ? s->clrActiveCaptionGradient : s->clrInactiveCaptionGradient, FALSE);
      }
    }
    IRECT rectQAT = pBar->GetQuickAccessToolbarLocation();
    if (rectQAT.l > rectQAT.r) {
      rectText.l = rectQAT.l + 1;
    }
    BOOL bHide = (pBar->GetHideFlags() & BCGPRIBBONBAR_HIDE_ALL) != 0;
    BOOL bExtra = !bHide && pBar->IsQuickAccessToolbarOnTop() &&
        rectQAT.l < rectQAT.r && !pBar->IsQATEmpty();
    BOOL bDrawIcon = (bHide && !bExtra) || pBar->IsScenicLook();
    if (bExtra) {
      IRECT rectQAFrame(rectQAT);
      rectQAFrame.r = rectText.l - 6;
      rectQAFrame.InflateRect(1, 1, 1, 1);
      const ControlRendererParams& params = s->m_ctrlRibbonCaptionQA.GetParams();
      if (rectQAFrame.Height() < params.s->m_rectImage.Height()) {
        rectQAFrame.t = rectQAFrame.b - params.s->m_rectImage.Height();
      }
      /*
      if(bGlass)
      {
      const int dxFrame = GetSystemMetrics(SM_CXSIZEFRAME) / 2;
      const int nTop = s->GetRibbonImageScale() != 1. ? -2 : 1;
      rectQAFrame.DeflateRect(1, nTop, dxFrame, 0);
      }
      */
      s->m_ctrlRibbonCaptionQA.Draw(im, rectQAFrame, bActive ? 0 : 1);
    }
    if (bDrawIcon) {
      BOOL bDestroyIcon = FALSE;
      CBitmap* hIcon = globalUtils.GetWndIcon(pWnd, &bDestroyIcon);
      if (hIcon != NULL) {
        CSize szIcon(::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
        long x = rectCaption.l + 2;
        long y = rectCaption.t + max(0, (rectCaption.Height() - szIcon.h) / 2);
        if (bGlass) {
          s->DrawIconOnGlass(s->m_hThemeWindow, im, hIcon, IRECT(x, y, x + szIcon.w, y + szIcon.h));
        }
        else {
          ::DrawIconEx(im, x, y, hIcon, szIcon.w, szIcon.h,
              0, NULL, DI_NORMAL);
        }
        if (rectText.l < (x + szIcon.w + 4)) {
          rectText.l = x + szIcon.w + 4;
        }
        if (bDestroyIcon) {
          ::DestroyIcon(hIcon);
        }
      }
    }
  }
  CString strText;
  pWnd->GetWindowText(strText);
  CFont* pOldFont = (CFont*)im->SelectObject(&s->m_AppCaptionFont);
  ASSERT(pOldFont != NULL);
  DrawNcText(im, rectText, strText, bActive,
      bIsRTL, FALSE, bGlass, pWnd->IsZoomed() ? 0 : 10,
      pWnd->IsZoomed() ? _RGB(255, 255, 255) : (COLOR) - 1);
  im->SelectObject(pOldFont);
}
int VisualManagerScenic::OnDrawRibbonCaptionButton(img_t* im, RibbonCaptionButton* pButton)
{
  const BOOL bHighlighted = pButton->IsHighlighted() || pButton->IsFocused();
  const BOOL bPressed = pButton->IsPressed();
  BCGBUTTON_STATE state = ButtonsIsRegular;
  if (bPressed) {
    if (bHighlighted) {
      state = ButtonsIsPressed;
    }
  }
  else if (bHighlighted) {
    state = ButtonsIsHighlighted;
  }
  const BOOL bMDI = pButton->IsMDIChildButton();
  BOOL bActive = TRUE;
  if (!bMDI) {
    RibbonBar* pBar = pButton->GetParentRibbonBar();
    if (pBar->GetSafeHwnd() != NULL) {
      CWnd* pWnd = pBar->GetParent();
      ASSERT_VALID(pWnd);
      bActive = IsWindowActive(pWnd);
    }
  }
  IRECT rect(pButton->GetRect());
  rect.DeflateRect(1, 1);
  DrawNcBtn(im, rect, pButton->GetID(), state, FALSE, bActive, bMDI);
}
COLOR VisualManagerScenic::OnDrawRibbonPanel(
    img_t* im,
    RibbonPanel* pPanel,
    IRECT rectPanel,
    IRECT rectCaption)
{
  if (!CanDrawImage()) {
    return WinXPVisualManager::OnDrawRibbonPanel(im, pPanel, rectPanel, rectCaption);
  }
  ASSERT_VALID(pPanel);
  if (pPanel->IsKindOf(RUNTIME_CLASS(RibbonMainPanel))) {
    const int nBorderSize = GetPopupMenuBorderSize();
    rectPanel.InflateRect(nBorderSize, nBorderSize);
    s->m_ctrlRibbonMainPanel.Draw(im, rectPanel);
  }
  else {
    if (!pPanel->IsMenuMode() && !pPanel->IsCollapsed()) {
      int nWidth = s->m_ctrlRibbonPanelBackSep.GetParams().s->m_rectImage.Width();
      int nWidth2 = nWidth / 2;
      rectPanel.l = rectPanel.r - nWidth2;
      rectPanel.r += (nWidth - nWidth2);
      rectPanel.DeflateRect(0, 4);
      s->m_ctrlRibbonPanelBackSep.Draw(im, rectPanel);
    }
  }
  return s->clrBarText;
}
COLOR VisualManagerScenic::OnFillRibbonPanelCaption(
    img_t* /*im*/,
    RibbonPanel* /*pPanel*/,
    IRECT /*rectCaption*/)
{
  return s->m_clrRibbonPanelCaptionText;
}
int VisualManagerScenic::OnDrawRibbonCategory(
    img_t* im,
    RibbonCategory* pCategory,
    IRECT rectCategory)
{
  ASSERT_VALID(pCategory);
  if (!CanDrawImage() || pCategory->IsOnDialogBar()) {
    WinXPVisualManager::OnDrawRibbonCategory(im, pCategory, rectCategory);
    return 0;
  }
  ControlRenderer* pRenderer = &s->m_ctrlRibbonCategoryBack;
  BitmapCache* pCache = &s->m_cacheRibbonCategoryBack;
  /*
  BaseRibbonElement* pParentButton = pCategory->GetParentButton();
  if(pCategory->GetTabColor() != BCGPCategoryColor_None &&
  (pParentButton == NULL || !pParentButton->IsQATMode()))
  {
  XRibbonContextCategory& context =
  s->m_ctrlRibbonContextCategory[pCategory->GetTabColor() - 1];
  pRenderer = &context.s->m_ctrlBack;
  pCache = &context.s->m_cacheBack;
  }
  */
  const ControlRendererParams& params = pRenderer->GetParams();
  RibbonPanelMenuBar* pMenuBar = pCategory->GetParentMenuBar();
  if (pMenuBar != NULL) {
    if (pMenuBar->GetCategory() != NULL) {
      if (rectCategory.l < 0 || rectCategory.t < 0) {
        DrawManager dm(*im);
        dm.FillGradient(rectCategory, s->m_clrRibbonBarGradientDark, s->m_clrRibbonBarGradientLight, TRUE);
        return 0;
      }
    }
  }
  int nCacheIndex = -1;
  if (pCache != NULL) {
    CSize size(params.s->m_rectImage.Width(), rectCategory.Height());
    nCacheIndex = pCache->FindIndex(size);
    if (nCacheIndex == -1) {
      nCacheIndex = pCache->CacheY(size.h, *pRenderer);
    }
  }
  if (nCacheIndex != -1) {
    pCache->Get(nCacheIndex)->DrawY(im, rectCategory,
        CSize(params.s->m_rectInter.l, params.s->m_rectImage.r - params.s->m_rectInter.r));
  }
  else {
    pRenderer->Draw(im, rectCategory);
  }
}
COLOR VisualManagerScenic::OnDrawRibbonCategoryTab(
    img_t* im,
    RibbonTab* pTab,
    BOOL bIsActive)
{
  ASSERT_VALID(pTab);
  if (!CanDrawImage()) {
    return WinXPVisualManager::OnDrawRibbonCategoryTab(im,
        pTab, bIsActive);
  }
  RibbonCategory* pCategory = pTab->GetParentCategory();
  ASSERT_VALID(pCategory);
  RibbonBar* pBar = pCategory->GetParentRibbonBar();
  ASSERT_VALID(pBar);
  bIsActive = bIsActive &&
      ((pBar->GetHideFlags() & BCGPRIBBONBAR_HIDE_ELEMENTS) == 0 || pTab->GetDroppedDown() != NULL);
  const BOOL bPressed = pTab->IsPressed();
  const BOOL bIsFocused = pTab->IsFocused() && (pBar->GetHideFlags() & BCGPRIBBONBAR_HIDE_ELEMENTS);
  const BOOL bIsHighlight = (pTab->IsHighlighted() || bIsFocused) && !pTab->IsDroppedDown();
  IRECT rectTab(pTab->GetRect());
  rectTab.b++;
  int ratio = 0;
  if (s->m_ctrlRibbonCategoryTabSep.IsValid()) {
    ratio = pBar->GetTabTrancateRatio();
  }
  if (ratio > 0) {
    rectTab.l++;
  }
  int nImage = bIsActive ? 3 : 0;
  if (bPressed) {
    if (bIsHighlight) {
      nImage = bIsActive ? 2 : 1;
    }
  }
  if (bIsHighlight) {
    nImage += 1;
  }
  ControlRenderer* pRenderer = &s->m_ctrlRibbonCategoryTab;
  /*
  COLOR clrText = s->m_clrRibbonCategoryText;
  COLOR clrTextHighlighted = s->m_clrRibbonCategoryTextHighlighted;
  if(pCategory->GetTabColor() != BCGPCategoryColor_None || pTab->IsSelected())
  {
  XRibbonContextCategory& context =
  s->m_ctrlRibbonContextCategory [
  (pTab->IsSelected() || nImage == 4)
  ? BCGPCategoryColor_Orange - 1
  : pCategory->GetTabColor() - 1];
  pRenderer = &context.s->m_ctrlTab;
  clrText = context.s->m_clrText;
  clrTextHighlighted = context.s->m_clrTextHighlighted;
  }
  */
  pRenderer->Draw(im, rectTab, nImage);
  if (ratio > 0) {
    IRECT rectSep(rectTab);
    rectSep.l = rectSep.r;
    rectSep.r++;
    rectSep.b--;
    s->m_ctrlRibbonCategoryTabSep.Draw(im, rectSep, 0, (BYTE)min(ratio * 255 / 100, 255));
  }
  return s->clrBarText;
}
COLOR VisualManagerScenic::OnDrawRibbonTabsFrame(
    img_t* im,
    RibbonBar* pWndRibbonBar,
    IRECT rectTab)
{
  UNREFERENCED_PARAMETER(im);
  UNREFERENCED_PARAMETER(pWndRibbonBar);
  UNREFERENCED_PARAMETER(rectTab);
  return(COLOR) - 1;
}
int VisualManagerScenic::OnFillMenuImageRect(img_t* im,
    ToolbarButton* pButton, IRECT rect, VisualManager::BCGBUTTON_STATE state)
{
  if (!CanDrawImage()) {
    WinXPVisualManager::OnFillMenuImageRect(im, pButton, rect, state);
    return 0;
  }
  OnFillButtonInterior(im, pButton, rect, state);
}
int VisualManagerScenic::OnFillButtonInterior(img_t* im,
    ToolbarButton* pButton, IRECT rect, VisualManager::BCGBUTTON_STATE state)
{
  if (!CanDrawImage()) {
    WinXPVisualManager::OnFillButtonInterior(im, pButton, rect, state);
    return 0;
  }
  CCustomizeButton* pCustButton = DYNAMIC_DOWNCAST(CCustomizeButton, pButton);
  if (pCustButton == NULL) {
    if (ToolBar::IsCustomizeMode() &&
        !ToolBar::IsAltCustomizeMode() && !pButton->IsLocked()) {
      return 0;
    }
    ControlRenderer* pRenderer = NULL;
    int index = 0;
    BOOL bDisabled = (pButton->s->m_nStyle & TBBS_DISABLED) == TBBS_DISABLED;
    //BOOL bPressed =(pButton->s->m_nStyle & TBBS_PRESSED ) == TBBS_PRESSED;
    BOOL bChecked = (pButton->s->m_nStyle & TBBS_CHECKED) == TBBS_CHECKED;
    BaseControlBar* pBar = DYNAMIC_DOWNCAST(BaseControlBar, pButton->GetParentWnd());
    ToolbarMenuButton* pMenuButton =
        DYNAMIC_DOWNCAST(ToolbarMenuButton, pButton);
    if (pMenuButton != NULL && pBar != NULL) {
      if (!pBar->IsKindOf(RUNTIME_CLASS(MenuBar)) &&
          pBar->IsKindOf(RUNTIME_CLASS(PopupMenuBar))) {
        if (bChecked) {
          pRenderer = &s->m_ctrlMenuItemBack;
          if (bDisabled) {
            index = 1;
          }
          rect.InflateRect(0, 0, 0, 1);
        }
        else if (state == ButtonsIsPressed || state == ButtonsIsHighlighted) {
          pRenderer = &s->m_ctrlMenuHighlighted[bDisabled ? 1 : 0];
        }
        else {
          return 0;
        }
      }
    }
    if (pRenderer != NULL) {
      pRenderer->Draw(im, rect, index);
      return 0;
    }
  }
  WinXPVisualManager::OnFillButtonInterior(im, pButton, rect, state);
}
int VisualManagerScenic::OnDrawRibbonMenuCheckFrame(
    img_t* im,
    RibbonButton* pButton,
    IRECT rect)
{
  if (!CanDrawImage()) {
    WinXPVisualManager::OnDrawRibbonMenuCheckFrame(im, pButton, rect);
    return 0;
  }
  s->m_ctrlMenuItemBack.Draw(im, rect);
}
int VisualManagerScenic::OnFillRibbonQATPopup(
    img_t* im, RibbonPanelMenuBar* pMenuBar, IRECT rect)
{
  if (!CanDrawImage() || !s->m_ctrlRibbonBorder_QAT.IsValid()) {
    WinXPVisualManager::OnFillRibbonQATPopup(im, pMenuBar, rect);
    return 0;
  }
  s->m_ctrlRibbonBorder_QAT.FillInterior(im, rect);
}
int VisualManagerScenic::GetRibbonPopupBorderSize(const RibbonPanelMenu* pPopup) const
{
  if (!CanDrawImage()) {
    return WinXPVisualManager::GetRibbonPopupBorderSize(pPopup);
  }
  if (pPopup != NULL) {
    ASSERT_VALID(pPopup);
    RibbonPanelMenuBar* pRibbonMenuBar =
        DYNAMIC_DOWNCAST(RibbonPanelMenuBar,
            (const_cast<RibbonPanelMenu*>(pPopup))->GetMenuBar());
    if (pRibbonMenuBar != NULL) {
      if (pRibbonMenuBar->IsMainPanel()) {
        return s->m_ctrlRibbonMainPanel.GetParams().s->m_rectSides.l;
      }
      if (!pRibbonMenuBar->IsMenuMode()) {
        if (pRibbonMenuBar->IsQATPopup()) {
          if (s->m_ctrlRibbonBorder_QAT.IsValid()) {
            return s->m_ctrlRibbonBorder_QAT.GetParams().s->m_rectSides.l;
          }
        }
        else if (pRibbonMenuBar->IsCategoryPopup()) {
          return 0;
        }
        else if (pRibbonMenuBar->IsFloaty()) {
          /*
          if(s->m_ctrlRibbonBorder_Floaty.IsValid())
          {
          return s->m_ctrlRibbonBorder_Floaty.GetParams().s->m_rectSides.l;
          }
          */
        }
        else {
          if (pRibbonMenuBar->GetPanel() != NULL) {
            return 0;
          }
          // standard size
        }
      }
    }
  }
  return(int)GetPopupMenuBorderSize();
}
#endif
int VisualManagerScenic::OnDrawButtonBorder(img_t* im,
    ToolbarButton* pButton, IRECT rect, VisualManager::BCGBUTTON_STATE state)
{
  ToolbarMenuButton* pMenuButton = DYNAMIC_DOWNCAST(ToolbarMenuButton, pButton);
  if (CanDrawImage() && pMenuButton != NULL &&
      pMenuButton->GetParentWnd() != NULL &&
      pMenuButton->GetParentWnd()->IsKindOf(RUNTIME_CLASS(PopupMenuBar))) {
    return 0;
  }
  WinXPVisualManager::OnDrawButtonBorder(im, pButton, rect, state);
}
int VisualManagerScenic::OnFillBarBackground(img_t* im, BaseControlBar* pBar,
    IRECT rectClient, IRECT rectClip,
    BOOL bNCArea)
{
  ASSERT_VALID(pBar);
  if (!CanDrawImage()) {
    WinXPVisualManager::OnFillBarBackground(im, pBar, rectClient, rectClip, bNCArea);
    return 0;
  }
  if (pBar->IsOnGlass()) {
    im->FillSolidRect(rectClient, _RGB(0, 0, 0));
    return 0;
  }
  if (pBar->IsKindOf(RUNTIME_CLASS(RibbonStatusBar))) {
    if (s->m_hThemeWindow != NULL) {
      (*s->m_pfDrawThemeBackground)(s->m_hThemeStatusBar, im,
          0, 0, &rectClient, 0);
      return 0;
    }
  }
  else if (pBar->IsKindOf(RUNTIME_CLASS(RibbonBar))) {
    DrawManager dm(*im);
    dm.FillGradient(rectClient, s->m_clrRibbonBarGradientDark, s->m_clrRibbonBarGradientLight, TRUE);
    return 0;
  }
  WinXPVisualManager::OnFillBarBackground(im, pBar, rectClient, rectClip, bNCArea);
}
int VisualManagerScenic::OnDrawStatusBarSizeBox(img_t* im, StatusBar* pStatBar,
    IRECT rectSizeBox)
{
  if (!CanDrawImage() || s->m_hThemeStatusBar == NULL) {
    WinXPVisualManager::OnDrawStatusBarSizeBox(im, pStatBar, rectSizeBox);
    return 0;
  }
  (*s->m_pfDrawThemeBackground)(s->m_hThemeStatusBar, im,
      /*SP_GRIPPER*/3, 0, &rectSizeBox, 0);
}
#ifndef BCGP_EXCLUDE_RIBBON
int VisualManagerScenic::OnDrawRibbonPaletteButton(
    img_t* im,
    RibbonPaletteIcon* pButton)
{
  if (!CanDrawImage()) {
    WinXPVisualManager::OnDrawRibbonPaletteButton(im, pButton);
    return 0;
  }
  int index = 0;
  if (pButton->IsDisabled()) {
    index = 3;
  }
  else {
    if (pButton->IsPressed()) {
      if (pButton->IsHighlighted()) {
        index = 2;
      }
    }
    else if (pButton->IsHighlighted() || pButton->IsFocused()) {
      index = 1;
    }
  }
  int nBtn = 1;
  if (pButton->IsLast()) {
    nBtn = 2;
  }
  else if (pButton->IsFirst()) {
    nBtn = 0;
  }
  s->m_ctrlRibbonBtnPalette[nBtn].Draw(im, pButton->GetRect(), index);
}
COLOR VisualManagerScenic::OnDrawRibbonStatusBarPane(img_t* im, RibbonStatusBar* pBar,
    RibbonStatusBarPane* pPane)
{
  ASSERT_VALID(pBar);
  ASSERT_VALID(pPane);
  if (!CanDrawImage()) {
    return WinXPVisualManager::OnDrawRibbonStatusBarPane(im, pBar, pPane);
  }
  IRECT rectPane = pPane->GetRect();
  const BOOL bHighlighted = pPane->IsHighlighted() || pPane->IsFocused();
  const BOOL bChecked = pPane->IsChecked();
  if (bHighlighted || bChecked) {
    IRECT rectButton = rectPane;
    rectButton.DeflateRect(1, 1);
    int index = 0;
    if (pPane->IsPressed()) {
      if (bHighlighted) {
        index = 1;
      }
    }
    else if (bChecked) {
      if (bHighlighted) {
        index = 0;
      }
      else {
        index = 1;
      }
    }
    s->m_ctrlRibbonBtnStatusPane.Draw(im, rectButton, index);
  }
  return(COLOR) - 1;
}
int VisualManagerScenic::OnDrawRibbonSliderZoomButton(
    img_t* im, RibbonSlider* pSlider,
    IRECT rect, BOOL bIsZoomOut,
    BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled)
{
  if (!CanDrawImage()) {
    WinXPVisualManager::OnDrawRibbonSliderZoomButton(im, pSlider, rect,
        bIsZoomOut, bIsHighlighted, bIsPressed, bIsDisabled);
    return 0;
  }
  ControlRenderer* pRenderer = bIsZoomOut
      ? &s->m_ctrlRibbonSliderBtnMinus
      : &s->m_ctrlRibbonSliderBtnPlus;
  int index = 0;
  if (bIsDisabled) {
    index = 3;
  }
  else {
    if (bIsPressed) {
      if (bIsHighlighted) {
        index = 2;
      }
    }
    else if (bIsHighlighted) {
      index = 1;
    }
  }
  pRenderer->FillInterior(im, rect,
      s->GetRibbonImageScale() != 1. ? ToolBarImages::ImageAlignHorzStretch : ToolBarImages::ImageAlignHorzCenter,
      s->GetRibbonImageScale() != 1. ? ImageAlignVertStretch : ImageAlignVertCenter,
      index);
}
int VisualManagerScenic::OnDrawRibbonProgressBar(img_t* im,
    RibbonProgressBar* pProgress,
    IRECT rectProgress, IRECT rectChunk,
    BOOL bInfiniteMode)
{
  if (!CanDrawImage() || s->m_hThemeProgress == NULL) {
    WinXPVisualManager::OnDrawRibbonProgressBar(im, pProgress, rectProgress,
        rectChunk, bInfiniteMode);
    return 0;
  }
  (*s->m_pfDrawThemeBackground)(s->m_hThemeProgress, im,
      /*PP_BAR*/1, 0, &rectProgress, 0);
  if (!bInfiniteMode) {
    if (!rectChunk.IsRectEmpty() || pProgress->GetPos() != pProgress->GetRangeMin()) {
      (*s->m_pfDrawThemeBackground)(s->m_hThemeProgress, im,
          /*PP_CHUNK*/3, 0, &rectChunk, 0);
    }
  }
  else {
    // (*s->m_pfDrawThemeBackground)(s->m_hThemeProgress, im,
    // /*PP_FILL*/5, 0, &rectProgress, 0);
    CRgn rgn;
    rgn.CreateRectRgnIndirect(rectProgress);
    im->SelectClipRgn(&rgn);
    double index = (pProgress->GetPos() - pProgress->GetRangeMin()) * 2.0 /
        double(pProgress->GetRangeMax() - pProgress->GetRangeMin());
    rectProgress.OffsetRect((int)(rectProgress.Width() * (index - 1.0)), 0);
    (*s->m_pfDrawThemeBackground)(s->m_hThemeProgress, im,
        /*PP_MOVEOVERLAY*/8, 0, &rectProgress, 0);
    im->SelectClipRgn(NULL);
  }
}
int VisualManagerScenic::OnDrawDefaultRibbonImage(img_t* im, IRECT rectImage,
    BOOL bIsDisabled/* = FALSE*/,
    BOOL bIsPressed/* = FALSE*/,
    BOOL bIsHighlighted/* = FALSE*/)
{
  if (!CanDrawImage()) {
    WinXPVisualManager::OnDrawDefaultRibbonImage(im, rectImage, bIsDisabled,
        bIsPressed, bIsHighlighted);
    return 0;
  }
  s->m_RibbonBtnDefaultImage.DrawEx(im, rectImage, bIsDisabled ? 1 : 0,
      ToolBarImages::ImageAlignHorzCenter, ImageAlignVertCenter);
}
int VisualManagerScenic::OnDrawRibbonMainButton(
    img_t* im,
    RibbonButton* pButton)
{
  if (!CanDrawImage()) {
    WinXPVisualManager::OnDrawRibbonMainButton(im, pButton);
    return 0;
  }
  BOOL bIsHighlighted = pButton->IsHighlighted() || pButton->IsFocused();
  BOOL bIsPressed = pButton->IsPressed() || pButton->IsDroppedDown();
  if (pButton->IsDroppedDown()) {
    bIsPressed = TRUE;
    bIsHighlighted = TRUE;
  }
  IRECT rect = pButton->GetRect();
  int index = 0;
  if (bIsPressed) {
    if (bIsHighlighted) {
      index = 2;
    }
  }
  else if (bIsHighlighted) {
    index = 1;
  }
  s->m_ctrlRibbonBtnMain.Draw(im, pButton->GetRect(), index);
}
int VisualManagerScenic::OnDrawCheckBoxEx(CDC* im, IRECT rect,
    int nState,
    BOOL bHighlighted,
    BOOL bPressed,
    BOOL bEnabled)
{
  if (s->m_hThemeButton == NULL) {
    VisualManager::OnDrawCheckBoxEx(im, rect, nState, bHighlighted, bPressed, bEnabled);
    return 0;
  }
  DrawCheckBox(im, rect, bHighlighted, nState, bEnabled, bPressed);
}
int VisualManagerScenic::OnDrawRadioButton(CDC* im, IRECT rect,
    BOOL bOn,
    BOOL bHighlighted,
    BOOL bPressed,
    BOOL bEnabled)
{
  if (s->m_hThemeButton == NULL) {
    VisualManager::OnDrawRadioButton(im, rect, bOn, bHighlighted, bPressed, bEnabled);
    return 0;
  }
  DrawRadioButton(im, rect, bHighlighted, bOn, bEnabled, bPressed);
}
int VisualManagerScenic::OnDrawRibbonButtonBorder(
    img_t* im,
    RibbonButton* pButton)
{
  if (!CanDrawImage()) {
    WinXPVisualManager::OnDrawRibbonButtonBorder(im, pButton);
    return 0;
  }
  if (pButton->IsKindOf(RUNTIME_CLASS(RibbonEdit))) {
    IRECT rect(pButton->GetRect());
    COLOR colorBorder = s->m_clrRibbonEditBorder;
    if (pButton->IsDisabled()) {
      colorBorder = s->m_clrRibbonEditBorderDisabled;
    }
    else if (pButton->IsHighlighted() || pButton->IsDroppedDown() || pButton->IsFocused()) {
      colorBorder = pButton->IsDroppedDown()
          ? s->m_clrRibbonEditBorderPressed
          : s->m_clrRibbonEditBorderHighlighted;
    }
    rect.l = pButton->GetCommandRect().l;
    if (ToolBarImages::s->m_bIsDrawOnGlass) {
      DrawManager dm(*im);
      dm.DrawRect(rect, (COLOR) - 1, colorBorder);
    }
    else {
      imdraw_Draw3dRect(im, 0, rect, colorBorder, colorBorder);
    }
  }
}
int VisualManagerScenic::OnDrawRibbonLaunchButton(
    img_t* im,
    RibbonLaunchButton* pButton,
    RibbonPanel* pPanel)
{
  ASSERT_VALID(pPanel);
  if (!CanDrawImage()) {
    WinXPVisualManager::OnDrawRibbonLaunchButton(im, pButton, pPanel);
    return 0;
  }
  IRECT rect(pButton->GetRect());
  rect.r--;
  rect.b--;
  BOOL bHighlighted = pButton->IsHighlighted() || pButton->IsFocused();
  int index = 0;
  if (s->m_RibbonBtnLaunchIcon.GetCount() > 3) {
    if (pButton->IsDisabled()) {
      index = 3;
    }
    else if (pButton->IsPressed()) {
      if (bHighlighted) {
        index = 2;
      }
    }
    else if (bHighlighted) {
      index = 1;
    }
  }
  else {
    if (!pButton->IsDisabled()) {
      if (pButton->IsPressed()) {
        if (bHighlighted) {
          index = 2;
        }
      }
      else if (bHighlighted) {
        index = 1;
      }
    }
  }
  if (s->m_ctrlRibbonBtnLaunch.IsValid()) {
    s->m_ctrlRibbonBtnLaunch.Draw(im, rect, index);
  }
  if (s->m_RibbonBtnLaunchIcon.IsValid()) {
    const double dblImageScale = s->GetRibbonImageScale();
    if (dblImageScale == 1.) {
      s->m_RibbonBtnLaunchIcon.DrawEx(im, rect, index,
          ToolBarImages::ImageAlignHorzCenter, ImageAlignVertCenter);
    }
    else {
      CSize sizeImage = s->m_RibbonBtnLaunchIcon.GetImageSize();
      sizeImage.w = (int)(.5 + dblImageScale * sizeImage.w);
      sizeImage.h = (int)(.5 + dblImageScale * sizeImage.h);
      rect.l = rect.CenterPoint().x - sizeImage.w / 2;
      rect.r = rect.l + sizeImage.w;
      rect.t = rect.CenterPoint().y - sizeImage.h / 2;
      rect.b = rect.t + sizeImage.h;
      s->m_RibbonBtnLaunchIcon.DrawEx(im, rect, index,
          ToolBarImages::ImageAlignHorzStretch,
          ImageAlignVertStretch);
    }
  }
}
COLOR VisualManagerScenic::OnFillRibbonButton(img_t* im, RibbonButton* pButton)
{
  if (!CanDrawImage()) {
    return WinXPVisualManager::OnFillRibbonButton(im, pButton);
  }
  const BOOL bIsMenuMode = pButton->IsMenuMode();
  IRECT rect(pButton->GetRect());
  ControlRenderer* pRenderer = NULL;
  BitmapCache* pCache = NULL;
  int index = 0;
  BOOL bDisabled = pButton->IsDisabled();
  BOOL bWasDisabled = bDisabled;
  BOOL bFocused = pButton->IsFocused();
  BOOL bDroppedDown = pButton->IsDroppedDown();
  BOOL bPressed = pButton->IsPressed() && !bIsMenuMode;
  BOOL bChecked = pButton->IsChecked();
  BOOL bHighlighted = pButton->IsHighlighted() || bFocused;
  BOOL bDefaultPanelButton = pButton->IsDefaultPanelButton() && !pButton->IsQATMode();
  if (bFocused) {
    bDisabled = FALSE;
  }
  if (pButton->IsDroppedDown() && !bIsMenuMode) {
    bChecked = TRUE;
    bPressed = FALSE;
    bHighlighted = FALSE;
  }
  BaseRibbonElement::RibbonElementLocation location =
      pButton->GetLocationInGroup();
  if (pButton->IsKindOf(RUNTIME_CLASS(RibbonEdit))) {
    COLOR color1 = s->m_clrRibbonEdit;
    if (bDisabled) {
      color1 = s->m_clrRibbonEditDisabled;
    }
    else if (bChecked || bHighlighted) {
      color1 = s->m_clrRibbonEditHighlighted;
    }
    COLOR color2 = color1;
    rect.l = pButton->GetCommandRect().l;
    {
      DrawManager dm(*im);
      dm.FillGradient(rect, color1, color2, TRUE);
    }
    return(COLOR) - 1;
  }
  if (bChecked && bIsMenuMode && !pButton->IsPaletteIcon()) {
    bChecked = FALSE;
  }
  if (location != BaseRibbonElement::RibbonElementNotInGroup &&
      pButton->IsShowGroupBorder()) {
    if (!pButton->GetMenuRect().IsRectEmpty()) {
      IRECT rectC = pButton->GetCommandRect();
      IRECT rectM = pButton->GetMenuRect();
      ControlRenderer* pRendererC = NULL;
      ControlRenderer* pRendererM = NULL;
      BitmapCache* pCacheC = NULL;
      BitmapCache* pCacheM = NULL;
      if (location == BaseRibbonElement::RibbonElementSingleInGroup) {
        pRendererC = &s->m_ctrlRibbonBtnGroupMenu_F[0];
        pRendererM = &s->m_ctrlRibbonBtnGroupMenu_L[1];
        pCacheC = &s->m_cacheRibbonBtnGroupMenu_F[0];
        pCacheM = &s->m_cacheRibbonBtnGroupMenu_L[1];
      }
      else if (location == BaseRibbonElement::RibbonElementFirstInGroup) {
        pRendererC = &s->m_ctrlRibbonBtnGroupMenu_F[0];
        pRendererM = &s->m_ctrlRibbonBtnGroupMenu_F[1];
        pCacheC = &s->m_cacheRibbonBtnGroupMenu_F[0];
        pCacheM = &s->m_cacheRibbonBtnGroupMenu_F[1];
      }
      else if (location == BaseRibbonElement::RibbonElementLastInGroup) {
        pRendererC = &s->m_ctrlRibbonBtnGroupMenu_L[0];
        pRendererM = &s->m_ctrlRibbonBtnGroupMenu_L[1];
        pCacheC = &s->m_cacheRibbonBtnGroupMenu_L[0];
        pCacheM = &s->m_cacheRibbonBtnGroupMenu_L[1];
      }
      else {
        pRendererC = &s->m_ctrlRibbonBtnGroupMenu_M[0];
        pRendererM = &s->m_ctrlRibbonBtnGroupMenu_M[1];
        pCacheC = &s->m_cacheRibbonBtnGroupMenu_M[0];
        pCacheM = &s->m_cacheRibbonBtnGroupMenu_M[1];
      }
      int indexC = 0;
      int indexM = 0;
      BOOL bHighlightedC = pButton->IsCommandAreaHighlighted();
      BOOL bHighlightedM = pButton->IsMenuAreaHighlighted();
      if (bChecked) {
        indexC = 3;
        if (bHighlighted) {
          indexM = 5;
        }
      }
      if (bDisabled) {
        if (bChecked) {
          indexC = 5;
          indexM = 4;
        }
      }
      else {
        if (pButton->IsDroppedDown() && !bIsMenuMode) {
          indexC = pButton->IsChecked() ? 3 : 6;
          indexM = 3;
        }
        else {
          if (bFocused) {
            indexC = 6;
            indexM = 5;
          }
          if (bHighlightedC || bHighlightedM) {
            if (bChecked) {
              indexC = bHighlightedC ? 4 : 3;
            }
            else {
              indexC = bHighlightedC ? 1 : 6;
            }
            indexM = bHighlightedM ? 1 : 5;
          }
          if (bPressed) {
            if (bHighlightedC) {
              indexC = 2;
            }
          }
        }
      }
      if (indexC != -1 && indexM != -1) {
        int nCacheIndex = -1;
        if (pCacheC != NULL) {
          CSize size(rectC.Size());
          nCacheIndex = pCacheC->FindIndex(size);
          if (nCacheIndex == -1) {
            nCacheIndex = pCacheC->Cache(size, *pRendererC);
          }
        }
        if (nCacheIndex != -1) {
          pCacheC->Get(nCacheIndex)->Draw(im, rectC, indexC);
        }
        else {
          pRendererC->Draw(im, rectC, indexC);
        }
        nCacheIndex = -1;
        if (pCacheM != NULL) {
          CSize size(rectM.Size());
          nCacheIndex = pCacheM->FindIndex(size);
          if (nCacheIndex == -1) {
            nCacheIndex = pCacheM->Cache(size, *pRendererM);
          }
        }
        if (nCacheIndex != -1) {
          pCacheM->Get(nCacheIndex)->Draw(im, rectM, indexM);
        }
        else {
          pRendererM->Draw(im, rectM, indexM);
        }
      }
      return(COLOR) - 1;
    }
    else {
      if (location == BaseRibbonElement::RibbonElementSingleInGroup) {
        pRenderer = &s->m_ctrlRibbonBtnGroup_S;
        pCache = &s->m_cacheRibbonBtnGroup_S;
      }
      else if (location == BaseRibbonElement::RibbonElementFirstInGroup) {
        pRenderer = &s->m_ctrlRibbonBtnGroup_F;
        pCache = &s->m_cacheRibbonBtnGroup_F;
      }
      else if (location == BaseRibbonElement::RibbonElementLastInGroup) {
        pRenderer = &s->m_ctrlRibbonBtnGroup_L;
        pCache = &s->m_cacheRibbonBtnGroup_L;
      }
      else {
        pRenderer = &s->m_ctrlRibbonBtnGroup_M;
        pCache = &s->m_cacheRibbonBtnGroup_M;
      }
      if (bChecked) {
        index = 3;
      }
      if (bDisabled && !bFocused) {
        index = 0;
      }
      else {
        if (bPressed) {
          if (bHighlighted) {
            index = 2;
          }
        }
        else if (bHighlighted) {
          index++;
        }
      }
    }
  }
  else if (bDefaultPanelButton) {
    if (bPressed) {
      if (bHighlighted) {
        index = 2;
      }
    }
    else if (bHighlighted) {
      index = 1;
    }
    else if (bChecked) {
      index = 2;
    }
    if (bFocused && !bDroppedDown && s->m_ctrlRibbonBtnDefault.GetImageCount() > 3) {
      index = 3;
    }
    if (index != -1) {
      pRenderer = &s->m_ctrlRibbonBtnDefault;
      BitmapCache* pCache = &s->m_cacheRibbonBtnDefault;
      RibbonCategory* pCategory = pButton->GetParentCategory();
      if (pCategory != NULL) {
        ASSERT_VALID(pCategory);
        /*
        if(pCategory->GetTabColor() != BCGPCategoryColor_None)
        {
        XRibbonContextCategory& context =
        s->m_ctrlRibbonContextCategory[pCategory->GetTabColor() - 1];
        pRenderer = &context.s->m_ctrlBtnDefault;
        pCache = &context.s->m_cacheBtnDefault;
        }
        */
      }
      const ControlRendererParams& params = pRenderer->GetParams();
      int nCacheIndex = -1;
      if (pCache != NULL) {
        CSize size(params.s->m_rectImage.Width(), rect.Height());
        nCacheIndex = pCache->FindIndex(size);
        if (nCacheIndex == -1) {
          nCacheIndex = pCache->CacheY(size.h, *pRenderer);
        }
      }
      if (nCacheIndex != -1) {
        pCache->Get(nCacheIndex)->DrawY(im, rect,
            CSize(params.s->m_rectInter.l, params.s->m_rectImage.r - params.s->m_rectInter.r),
            index);
        return s->clrBtnText;
      }
    }
  }
  else if ((!bDisabled && (bPressed || bChecked || bHighlighted)) ||
      (bDisabled && bFocused)) {
    if (!pButton->GetMenuRect().IsRectEmpty()/* &&
 (pButton->IsHighlighted() || bChecked)*/) {
      IRECT rectC = pButton->GetCommandRect();
      IRECT rectM = pButton->GetMenuRect();
      ControlRenderer* pRendererC = pButton->IsMenuOnBottom()
          ? &s->m_ctrlRibbonBtnMenuV[0]
          : &s->m_ctrlRibbonBtnMenuH[0];
      ControlRenderer* pRendererM = pButton->IsMenuOnBottom()
          ? &s->m_ctrlRibbonBtnMenuV[1]
          : &s->m_ctrlRibbonBtnMenuH[1];
      int indexC = -1;
      int indexM = -1;
      BOOL bDropped = pButton->IsDroppedDown();
      BOOL bHighlightedC = pButton->IsCommandAreaHighlighted();
      BOOL bHighlightedM = pButton->IsMenuAreaHighlighted();
      if (bDisabled) {
        if (bHighlightedC || bHighlightedM) {
          indexC = 4;
          indexM = 4;
          if (bHighlightedM) {
            indexM = 0;
            if (bDropped && !bIsMenuMode) {
              indexC = 5;
              indexM = 2;
            }
            else if (bPressed) {
              indexM = 1;
            }
          }
        }
      }
      else {
        if (bDropped && !bIsMenuMode) {
          indexC = 5;
          indexM = 2;
        }
        else {
          if (bFocused) {
            indexC = 5;
            indexM = 4;
          }
          if (bChecked) {
            indexC = 2;
            indexM = 2;
          }
          if (bHighlightedC || bHighlightedM) {
            indexM = 4;
            if (bPressed) {
              if (bHighlightedC) {
                indexC = 1;
              }
              else if (bHighlightedM) {
                indexC = bChecked ? 3 : 5;
              }
            }
            else {
              indexC = bChecked ? 3 : 0;
              if (bHighlightedM) {
                indexC = bChecked ? 3 : 5;
                indexM = 0;
              }
            }
          }
        }
      }
      if (indexC != -1) {
        pRendererC->Draw(im, rectC, indexC);
      }
      if (indexM != -1) {
        pRendererM->Draw(im, rectM, indexM);
      }
      return(COLOR) - 1;
    }
    else {
      index = -1;
      pRenderer = &s->m_ctrlRibbonBtn[0];
      if (rect.Height() > pRenderer->GetParams().s->m_rectImage.Height() * 1.5 &&
          s->m_ctrlRibbonBtn[1].IsValid()) {
        pRenderer = &s->m_ctrlRibbonBtn[1];
      }
      if (bDisabled && bFocused) {
        if (pRenderer->GetImageCount() > 4) {
          index = 4;
        }
        else {
          index = 0;
        }
      }
      if (!bDisabled) {
        if (bChecked) {
          index = 2;
        }
        if (bPressed) {
          if (bHighlighted) {
            index = 1;
          }
        }
        else if (bHighlighted) {
          index++;
        }
      }
    }
  }
  COLOR clrText = bWasDisabled
      ? s->clrGrayedText
      : COLOR(-1);
  if (pRenderer != NULL) {
    if (index != -1) {
      int nCacheIndex = -1;
      if (pCache != NULL) {
        CSize size(rect.Size());
        nCacheIndex = pCache->FindIndex(size);
        if (nCacheIndex == -1) {
          nCacheIndex = pCache->Cache(size, *pRenderer);
        }
      }
      if (nCacheIndex != -1) {
        pCache->Get(nCacheIndex)->Draw(im, rect, index);
      }
      else {
        pRenderer->Draw(im, rect, index);
      }
      if (!bWasDisabled) {
        clrText = s->clrBtnText;
      }
    }
  }
  return clrText;
}
int VisualManagerScenic::OnDrawRibbonDefaultPaneButton(
    img_t* im,
    RibbonButton* pButton)
{
  if (!CanDrawImage()) {
    WinXPVisualManager::OnDrawRibbonDefaultPaneButton(im, pButton);
    return 0;
  }
  OnFillRibbonButton(im, pButton);
  BOOL bIsQATMode = pButton->IsQATMode();
  IRECT rectFrame(pButton->GetRect());
  if (!bIsQATMode) {
    if (s->m_ctrlRibbonBtnDefaultIcon.IsValid()) {
      const CSize sizeImage = pButton->GetImageSize(RibbonButton::RibbonImageSmall);
      const int nMarginX = 11;
      const int nMarginY = 10;
      rectFrame.t += nMarginY / 2;
      rectFrame.b = rectFrame.t + sizeImage.h + 2 * nMarginY;
      rectFrame.t -= 2;
      rectFrame.l = rectFrame.CenterPoint().x - sizeImage.w / 2 - nMarginX;
      rectFrame.r = rectFrame.l + sizeImage.w + 2 * nMarginX;
      s->m_ctrlRibbonBtnDefaultIcon.Draw(im, rectFrame);
    }
  }
  else {
    if (s->m_ctrlRibbonBtnDefaultQAT.IsValid()) {
      int index = 0;
      if (pButton->IsDroppedDown()) {
        index = 2;
      }
      else if (pButton->IsPressed()) {
        if (pButton->IsHighlighted()) {
          index = 2;
        }
      }
      else if (pButton->IsHighlighted() || pButton->IsFocused()) {
        index = 1;
      }
      s->m_ctrlRibbonBtnDefaultQAT.Draw(im, rectFrame, index);
    }
  }
  OnDrawRibbonDefaultPaneButtonContext(im, pButton);
}
int VisualManagerScenic::OnDrawRibbonRecentFilesFrame(
    img_t* im,
    RibbonMainPanel* pPanel,
    IRECT rect)
{
  if (!CanDrawImage()) {
    WinXPVisualManager::OnDrawRibbonRecentFilesFrame(im,
        pPanel, rect);
    return 0;
  }
  rect.r += 2; //TODO
  imdraw_FillRect(im, 0, rect, s->m_clrRibbonBarBkgnd);
  IRECT rectSeparator = rect;
  rectSeparator.r = rectSeparator.l + 2;
  imdraw_Draw3dRect(im, 0, rectSeparator, s->clrBtnShadow,
      s->clrBtnHilite);
}
#endif
BOOL VisualManagerScenic::IsWindowActive(CWnd* pWnd) const
{
  BOOL bActive = FALSE;
  HWND hWnd = pWnd->GetSafeHwnd();
  if (hWnd != NULL) {
    if (!s->m_ActivateFlag.Lookup(pWnd->GetSafeHwnd(), bActive)) {
      //ASSERT(FALSE);
      bActive = TRUE;
    }
  }
  return bActive;
}
BOOL VisualManagerScenic::OnNcPaint(CWnd* pWnd, const CObList& lstSysButtons, IRECT rectRedraw)
{
  UNREFERENCED_PARAMETER(lstSysButtons);
#ifdef BCGP_EXCLUDE_RIBBON
  UNREFERENCED_PARAMETER(pWnd);
  UNREFERENCED_PARAMETER(rectRedraw);
#else
  if (s->DwmIsCompositionEnabled()) {
    return FALSE;
  }
  if (pWnd->GetSafeHwnd() == NULL || s->m_hThemeWindow == NULL) {
    return FALSE;
  }
  ASSERT_VALID(pWnd);
  RibbonBar* pBar = GetRibbonBar(pWnd);
  BOOL bRibbonCaption = pBar != NULL && pBar->IsWindowVisible() && pBar->IsReplaceFrameCaption();
  if (!bRibbonCaption) {
    return FALSE;
  }
  else {
    const DWORD dwStyle = pWnd->GetStyle();
    BOOL bMaximized = (dwStyle & WS_MAXIMIZE) == WS_MAXIMIZE;
    if (bMaximized) {
      return TRUE;
    }
  }
  CWindowDC dc(pWnd);
  if (dc.GetSafeHdc() != NULL) {
    CRgn rgn;
    if (!rectRedraw.IsRectEmpty()) {
      rgn.CreateRectRgnIndirect(rectRedraw);
      dc.SelectClipRgn(&rgn);
    }
    IRECT rtWindow;
    pWnd->GetWindowRect(rtWindow);
    pWnd->ScreenToClient(rtWindow);
    IRECT rtClient;
    pWnd->GetClientRect(rtClient);
    rtClient.OffsetRect(-rtWindow.TopLeft());
    dc.ExcludeClipRect(rtClient);
    rtWindow.OffsetRect(-rtWindow.TopLeft());
    BOOL bActive = IsWindowActive(pWnd);
    // Modify bActive(if currently TRUE) for owner-drawn MDI child windows: draw child
    // frame active only if window is active MDI child and the MDI frame window is active.
    if (bActive && IsOwnerDrawCaption() && pWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd))) {
      CMDIFrameWnd* pParent = ((CMDIChildWnd*)pWnd)->GetMDIFrame();
      if (pParent) {
        CMDIChildWnd* pActiveChild = pParent->MDIGetActive(NULL);
        if (pActiveChild) {
          bActive = ((pActiveChild->GetSafeHwnd() == pWnd->GetSafeHwnd()) && IsWindowActive(pParent));
        }
      }
    }
    IRECT rectCaption(rtWindow);
    CSize szSysBorders(GetSystemBorders(bRibbonCaption));
    rectCaption.b = rectCaption.t + szSysBorders.h + pBar->GetCaptionHeight();
    (*s->m_pfDrawThemeBackground)(s->m_hThemeWindow, dc.GetSafeHdc(),
        /*WP_CAPTION*/1, bActive ? /*CS_ACTIVE*/1 : /*CS_INACTIVE*/2, &rectCaption, 0);
    rtWindow.t = rectCaption.b;
    dc.ExcludeClipRect(rectCaption);
    int framestate = bActive ? /*FS_ACTIVE*/1 : /*FS_INACTIVE*/2;
    IRECT rectPart(rtWindow);
    rectPart.t = rectPart.b - szSysBorders.h;
    (*s->m_pfDrawThemeBackground)(s->m_hThemeWindow, dc.GetSafeHdc(),
        /*WP_FRAMEBOTTOM*/9, framestate, &rectPart, 0);
    rectPart.b = rectPart.t;
    rectPart.t = rtWindow.t;
    rectPart.r = rectPart.l + szSysBorders.w;
    (*s->m_pfDrawThemeBackground)(s->m_hThemeWindow, dc.GetSafeHdc(),
        /*WP_FRAMELEFT*/7, framestate, &rectPart, 0);
    rectPart.r = rtWindow.r;
    rectPart.l = rectPart.r - szSysBorders.w;
    (*s->m_pfDrawThemeBackground)(s->m_hThemeWindow, dc.GetSafeHdc(),
        /*WP_FRAMERIGHT*/8, framestate, &rectPart, 0);
    dc.SelectClipRgn(NULL);
    return TRUE;
  }
#endif
  return FALSE;
}
BOOL VisualManagerScenic::OnNcActivate(CWnd* pWnd, BOOL bActive)
{
  ASSERT_VALID(pWnd);
  if (pWnd->GetSafeHwnd() == NULL) {
    return FALSE;
  }
  if (s->DwmIsCompositionEnabled()) {
    return FALSE;
  }
  // stay active if WF_STAYACTIVE bit is on
  if (pWnd->s->m_nFlags & WF_STAYACTIVE) {
    bActive = TRUE;
  }
  // but do not stay active if the window is disabled
  if (!pWnd->IsWindowEnabled()) {
    bActive = FALSE;
  }
  BOOL bIsMDIFrame = FALSE;
  BOOL bWasActive = FALSE;
  // If the active state of an owner-draw MDI frame window changes, we need to
  // invalidate the MDI client area so the MDI child window captions are redrawn.
  if (IsOwnerDrawCaption()) {
    bIsMDIFrame = pWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd));
    bWasActive = IsWindowActive(pWnd);
  }
  s->m_ActivateFlag[pWnd->GetSafeHwnd()] = bActive;
  pWnd->SendMessage(Ws->m_NCPAINT, 0, 0);
  if (IsOwnerDrawCaption()) {
    if (bIsMDIFrame && (bWasActive != bActive)) {
      ::RedrawWindow(((CMDIFrameWnd*)pWnd)->s->m_hWndMDIClient, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
    }
  }
  return TRUE;
}
CSize VisualManagerScenic::GetSystemBorders(BOOL bRibbonPresent) const
{
  CSize size(::GetSystemMetrics(SM_CYSIZEFRAME), ::GetSystemMetrics(SM_CXSIZEFRAME));
  if (bRibbonPresent) {
    size.w--;
    size.h--;
  }
  return size;
}
int VisualManagerScenic::OnDrawComboDropButton(img_t* im, IRECT rect,
    BOOL bDisabled,
    BOOL bIsDropped,
    BOOL bIsHighlighted,
    ToolbarComboBoxButton* pButton)
{
  if (!CanDrawImage() || !pButton->IsRibbonButton()) {
    WinXPVisualManager::OnDrawComboDropButton(im, rect, bDisabled, bIsDropped, bIsHighlighted, pButton);
    return 0;
  }
  rect.InflateRect(0, 1, 1, 1);
  int nIndex = 0;
  if (bDisabled) {
    nIndex = 3;
  }
  else {
    if (bIsDropped) {
      nIndex = 2;
    }
    else if (bIsHighlighted) {
      nIndex = 1;
    }
  }
  s->m_ctrlRibbonComboBoxBtn.Draw(im, rect, nIndex);
  rect.DeflateRect(0, 1, 1, 1);
  rect.b -= 2;
  MenuImages::Draw(im, MenuImages::IdArowDown, rect,
      bDisabled
      ? MenuImages::ImageGray
      : MenuImages::ImageBlack);
}
#ifndef BCGP_EXCLUDE_RIBBON
RibbonBar* VisualManagerScenic::GetRibbonBar(CWnd* pWnd) const
{
  RibbonBar* pBar = NULL;
  if (pWnd == NULL) {
    pWnd = AfxGetMainWnd();
  }
  if (pWnd->GetSafeHwnd() == NULL) {
    return NULL;
  }
  if (pWnd->IsKindOf(RUNTIME_CLASS(FrameWnd))) {
    pBar = ((FrameWnd*) pWnd)->GetRibbonBar();
  }
  else if (pWnd->IsKindOf(RUNTIME_CLASS(MDIFrameWnd))) {
    pBar = ((MDIFrameWnd*) pWnd)->GetRibbonBar();
  }
  return pBar;
}
BOOL VisualManagerScenic::IsRibbonPresent(CWnd* pWnd) const
{
  RibbonBar* pBar = GetRibbonBar(pWnd);
  return pBar != NULL && pBar->IsWindowVisible();
}
COLOR VisualManagerScenic::GetRibbonEditBackgroundColor(
    RibbonEditCtrl* pEdit,
    BOOL bIsHighlighted,
    BOOL bIsPaneHighlighted,
    BOOL bIsDisabled)
{
  if (!CanDrawImage()) {
    return WinXPVisualManager::GetRibbonEditBackgroundColor(
        pEdit, bIsHighlighted, bIsPaneHighlighted, bIsDisabled);
  }
  COLOR color = s->m_clrRibbonEdit;
  if (bIsDisabled) {
    color = s->m_clrRibbonEditDisabled;
  }
  else {
    if (bIsHighlighted) {
      color = s->m_clrRibbonEditHighlighted;
    }
  }
  return color;
}
int VisualManagerScenic::OnDrawRibbonPaletteBorder(
    img_t* im,
    RibbonPaletteButton* pButton,
    IRECT rectBorder)
{
  if (!CanDrawImage()) {
    WinXPVisualManager::OnDrawRibbonPaletteBorder(im, pButton, rectBorder);
    return 0;
  }
  rectBorder.r -= 5;
  imdraw_Draw3dRect(im, 0, rectBorder, s->m_clrRibbonEditBorder, s->m_clrRibbonEditBorder);
}
#endif
COLOR VisualManagerScenic::OnDrawMenuLabel(img_t* im, IRECT rect)
{
  if (!CanDrawImage()) {
    return WinXPVisualManager::OnDrawMenuLabel(im, rect);
  }
  imdraw_FillRect(im, 0, rect, s->m_clrRibbonBarBkgnd);
  IRECT rectSeparator = rect;
  rectSeparator.t = rectSeparator.b - 2;
  DrawSeparator(im, rectSeparator, s->m_penSeparatorDark, s->m_penSeparatorLight, TRUE);
  return s->clrBarText;
}
int VisualManagerScenic::OnFillPopupMenuBackground(img_t* im, IRECT rect)
{
  if (!CanDrawImage()) {
    WinXPVisualManager::OnFillPopupMenuBackground(im, rect);
    return 0;
  }
  CBrush br(_RGB(255, 255, 255));
  imdraw_FillRect(im, 0, rect, &br);
}
#endif
// BCGPVisualManager2003.cpp: implementation of the VisualManager2003 class.
//////////////////////////////////////////////////////////////////////
static COLOR CalculateHourLineColor2003(COLOR clrBase, BOOL bWorkingHours, BOOL bHour)
{
  int nAlpha = bHour ? 85 : 95;
  return PixelAlpha2(clrBase, nAlpha);
}
static COLOR CalculateWorkingColor2003(COLOR clrBase)
{
  return clrBase;
}
static COLOR CalculateNonWorkingColor2003(uidraw_t* s, COLOR clrBase)
{
  double H, S, L;
  if (clrBase == _RGB(255, 255, 255)) {
    return s->clrBtnFace;
  }
  RGBtoHSL(clrBase, &H, &S, &L);
  return HLStoRGB_ONE(S == 1 ? H * .84 : H, S == 1 ? L * .95 : L * .84, S);
}
#if 0
int VisualManager2003_DrawCustomizeButton(img_t* im, IRECT rect, BOOL bIsHorz,
    VisualManager::BCGBUTTON_STATE state,
    BOOL bIsCustomize, BOOL bIsMoreButtons)
{
  COLOR clrDark = state == ButtonsIsRegular ?
      s->m_clrCustomizeButtonGradientDark : s->m_clrHighlightGradientDark;
  COLOR clrLight = state == ButtonsIsRegular ?
      s->m_clrCustomizeButtonGradientLight : s->m_clrHighlightGradientLight;
#define PTS_NUM 6
  IPOINT pts [PTS_NUM];
  if (bIsHorz) {
    pts [0] = CPoint(rect.l, rect.t);
    pts [1] = CPoint(rect.l + 2, rect.t + 1);
    pts [2] = CPoint(rect.l + 3, rect.b - 3);
    pts [3] = CPoint(rect.l, rect.b);
    pts [4] = CPoint(rect.r, rect.b);
    pts [5] = CPoint(rect.r, rect.t);
  }
  else {
    pts [0] = CPoint(rect.l, rect.t);
    pts [1] = CPoint(rect.l + 3, rect.t + 2);
    pts [2] = CPoint(rect.r - 3, rect.t + 3);
    pts [3] = CPoint(rect.r, rect.t);
    pts [4] = CPoint(rect.r, rect.b);
    pts [5] = CPoint(rect.l, rect.b);
  }
  CRgn rgnClip;
  rgnClip.CreatePolygonRgn(pts, PTS_NUM, WINDING);
  im->SelectClipRgn(&rgnClip);
  DrawManager dm(*im);
  dm.FillGradient(rect, clrDark, clrLight, bIsHorz);
  //---------------------
  // Draw button content:
  //---------------------
  const int nEllipse = 2;
  if (bIsHorz) {
    rect.DeflateRect(0, nEllipse);
    rect.l += nEllipse;
  }
  else {
    rect.DeflateRect(nEllipse, 0);
    rect.t += nEllipse;
  }
  const int nMargin = GetToolBarCustomizeButtonMargin();
  CSize sizeImage = MenuImages::Size();
  if (ToolBar::IsLargeIcons()) {
    sizeImage.w *= 2;
    sizeImage.h *= 2;
  }
  if (bIsCustomize) {
    //-----------------
    // Draw menu image:
    //-----------------
    IRECT rectMenu = rect;
    if (bIsHorz) {
      rectMenu.t = rectMenu.b - sizeImage.h - 2 * nMargin;
    }
    else {
      rectMenu.t++;
      rectMenu.l = rectMenu.r - sizeImage.w - 2 * nMargin;
    }
    rectMenu.DeflateRect(
        (rectMenu.Width() - sizeImage.w) / 2,
        (rectMenu.Height() - sizeImage.h) / 2);
    rectMenu.OffsetRect(1, 1);
    MenuImages::IMAGES_IDS id = bIsHorz ?
        MenuImages::IdCustomizeArowDown : MenuImages::IdCustomizeArowLeft;
    MenuImages::Draw(im, id, rectMenu.TopLeft(),
        MenuImages::ImageWhite, sizeImage);
    rectMenu.OffsetRect(-1, -1);
    MenuImages::Draw(im, id, rectMenu.TopLeft(), MenuImages::ImageBlack, sizeImage);
  }
  if (bIsMoreButtons) {
    //-------------------
    // Draw "more" image:
    //-------------------
    IRECT rectMore = rect;
    if (bIsHorz) {
      rectMore.b = rectMore.t + sizeImage.h + 2 * nMargin;
    }
    else {
      rectMore.r = rectMore.l + sizeImage.w + 2 * nMargin;
      rectMore.t++;
    }
    rectMore.DeflateRect(
        (rectMore.Width() - sizeImage.w) / 2,
        (rectMore.Height() - sizeImage.h) / 2);
    MenuImages::IMAGES_IDS id =
        bIsHorz ?
        MenuImages::IdCustomizeMoreButtonsHorz :
        MenuImages::IdCustomizeMoreButtonsVert;
    rectMore.OffsetRect(1, 1);
    MenuImages::Draw(im, id, rectMore.TopLeft(), MenuImages::ImageWhite, sizeImage);
    rectMore.OffsetRect(-1, -1);
    MenuImages::Draw(im, id, rectMore.TopLeft(), MenuImages::ImageBlack, sizeImage);
  }
  im->SelectClipRgn(NULL);
}
BOOL VisualManager2003_IsToolbarRoundShape(ToolBar* pToolBar)
{
  ASSERT_VALID(pToolBar);
  return !pToolBar->IsKindOf(RUNTIME_CLASS(MenuBar));
}
int VisualManager2003_OnFillBarBackground(img_t* im, BaseControlBar* pBar,
    IRECT rectClient, IRECT rectClip,
    BOOL bNCArea)
{
  ASSERT_VALID(pBar);
  if (pBar->IsOnGlass()) {
    im->FillSolidRect(rectClient, _RGB(0, 0, 0));
    return 0;
  }
  if (DYNAMIC_DOWNCAST(ReBar, pBar) != NULL ||
      DYNAMIC_DOWNCAST(ReBar, pBar->GetParent())) {
    FillRebarPane(im, pBar, rectClient);
    return 0;
  }
  CRuntimeClass* pBarClass = pBar->GetRuntimeClass();
  if (s->m_nBitsPerPixel <= 8 ||
      IsHighContastMode(s) ||
      pBar->IsDialogControl() ||
      pBarClass->IsDerivedFrom(RUNTIME_CLASS(ColorBar)) ||
      pBarClass->IsDerivedFrom(RUNTIME_CLASS(Calculator)) ||
      pBarClass->IsDerivedFrom(RUNTIME_CLASS(CalendarBar))) {
    VisualManagerXP::OnFillBarBackground(im, pBar, rectClient, rectClip);
    return 0;
  }
  if (pBar->IsKindOf(RUNTIME_CLASS(StatusBar))) {
    if (s->m_bStatusBarOfficeXPLook && s->m_hThemeStatusBar != NULL) {
      (*s->m_pfDrawThemeBackground)(s->m_hThemeStatusBar,
          im,
          0, 0, &rectClient, 0);
      return 0;
    }
  }
#ifndef BCGP_EXCLUDE_RIBBON
  if (pBar->IsKindOf(RUNTIME_CLASS(RibbonStatusBar))) {
    if (s->m_hThemeStatusBar != NULL) {
      (*s->m_pfDrawThemeBackground)(s->m_hThemeStatusBar,
          im,
          0, 0, &rectClient, 0);
      return 0;
    }
  }
#endif // BCGP_EXCLUDE_RIBBON
  if (rectClip.IsRectEmpty()) {
    rectClip = rectClient;
  }
  DrawManager dm(*im);
  if (pBar->IsKindOf(RUNTIME_CLASS(CaptionBar))) {
    CaptionBar* pCaptionBar = (CaptionBar*) pBar;
    if (pCaptionBar->IsMessageBarMode()) {
      dm.FillGradient(rectClient,
          s->m_clrBarGradientDark, s->m_clrBarGradientLight, FALSE);
    }
    else {
      dm.FillGradient(rectClient,
          s->m_clrCaptionBarGradientDark, s->m_clrCaptionBarGradientLight, TRUE);
    }
    return 0;
  }
  if (pBar->IsKindOf(RUNTIME_CLASS(PopupMenuBar))) {
    imdraw_FillRect(im, 0, rectClip, s->m_clrMenuLight);
    PopupMenuBar* pMenuBar = DYNAMIC_DOWNCAST(PopupMenuBar, pBar);
    if (!pMenuBar->s->m_bDisableSideBarInXPMode) {
      IRECT rectImages = rectClient;
      rectImages.r = rectImages.l + pMenuBar->GetGutterWidth();
      rectImages.DeflateRect(0, 1);
      dm.FillGradient(rectImages, s->m_clrToolBarGradientLight, s->m_clrToolBarGradientDark, FALSE,
          35);
    }
    return 0;
  }
  BOOL bIsHorz = (pBar->GetBarStyle() & CBRS_ORIENT_HORZ);
  BOOL bIsToolBar = pBar->IsKindOf(RUNTIME_CLASS(ToolBar)) &&
      !pBar->IsKindOf(RUNTIME_CLASS(MenuBar));
#ifndef BCGP_EXCLUDE_TOOLBOX
  BOOL bIsToolBox = pBar->IsKindOf(RUNTIME_CLASS(ToolBoxPage)) ||
      pBar->IsKindOf(RUNTIME_CLASS(ToolBox)) ||
      pBar->IsKindOf(RUNTIME_CLASS(ToolBoxEx));
#else
  BOOL bIsToolBox = FALSE;
#endif
  COLOR clr1 = bIsHorz ? s->m_clrToolBarGradientDark : s->m_clrToolBarGradientVertLight;
  COLOR clr2 = bIsHorz ? s->m_clrToolBarGradientLight : s->m_clrToolBarGradientVertDark;
  if (bIsToolBox) {
    clr1 = s->m_clrBarGradientLight;
    clr2 = s->m_clrBarGradientDark;
    bIsHorz = FALSE;
  }
  if (!bIsToolBar && !bIsToolBox) {
    bIsHorz = FALSE;
    clr1 = s->m_clrBarGradientDark;
    clr2 = s->m_clrBarGradientLight;
    rectClient.r = rectClient.l + s->m_rectVirtual.Width() + 10;
  }
  const int nStartFlatPercentage = bIsToolBar ? 25 : 0;
  BOOL bRoundedCorners = FALSE;
  if (pBar->IsKindOf(RUNTIME_CLASS(DropDownToolBar))) {
    bNCArea = FALSE;
  }
  ToolBar* pToolBar = DYNAMIC_DOWNCAST(ToolBar, pBar);
  if (bNCArea && pToolBar != NULL && pToolBar->IsDocked() &&
      pToolBar->GetParentDockBar() != NULL &&
      !pToolBar->IsKindOf(RUNTIME_CLASS(MenuBar))) {
    bRoundedCorners = TRUE;
    BaseControlBar* pParentBar = DYNAMIC_DOWNCAST(BaseControlBar,
        pBar->GetParent());
    if (pParentBar != NULL) {
      CPoint pt(0, 0);
      pBar->MapWindowPoints(pParentBar, &pt, 1);
      pt = im->OffsetWindowOrg(pt.x, pt.y);
      IRECT rectParent;
      pParentBar->GetClientRect(rectParent);
      OnFillBarBackground(im, pParentBar, rectParent, rectParent);
      im->SetWindowOrg(pt.x, pt.y);
    }
    IRECT rectFill = rectClient;
    rectFill.DeflateRect(1, 0);
    dm.FillGradient(rectFill, clr1, clr2, bIsHorz, nStartFlatPercentage);
    IRECT rectLeft = rectClient;
    rectLeft.t ++;
    rectLeft.r = rectLeft.l + 1;
    dm.FillGradient(rectLeft, clr1, clr2, bIsHorz);
    IRECT rectRight = rectClient;
    rectLeft.t ++;
    rectRight.l = rectRight.r - 1;
    dm.FillGradient(rectRight, clr1, clr2, bIsHorz);
  }
  else {
    IRECT rectFill = rectClient;
    if (bIsToolBar && pBar->IsDocked() && pToolBar->GetParentDockBar() != NULL) {
      ASSERT_VALID(pToolBar);
      rectFill.l -= pToolBar->s->m_cxLeftBorder;
      rectFill.r += pToolBar->s->m_cxRightBorder;
      rectFill.t -= pToolBar->s->m_cyTopBorder;
      rectFill.b += pToolBar->s->m_cyBottomBorder;
    }
    if (pBar->GetCurrentAlignment() == CBRS_ALIGN_RIGHT &&
        clr1 != clr2) {
      if (DYNAMIC_DOWNCAST(DockBar, pBar) != NULL ||
          DYNAMIC_DOWNCAST(AutoHideToolBar, pBar) != NULL) {
        CFrameWnd* pMainFrame = BCGCBProGetTopLevelFrame(pBar);
        if (pMainFrame->GetSafeHwnd() != NULL) {
          IRECT rectMain;
          pMainFrame->GetClientRect(rectMain);
          pMainFrame->MapWindowPoints(pBar, &rectMain);
          rectFill.t = rectMain.t;
          rectFill.l = rectMain.l;
          rectFill.r = rectFill.l + s->m_rectVirtual.Width() + 10;
        }
      }
    }
    dm.FillGradient(rectFill, clr1, clr2, bIsHorz, nStartFlatPercentage);
  }
  if (bNCArea) {
    CCustomizeButton* pCustomizeButton = NULL;
    IRECT rectCustomizeBtn;
    rectCustomizeBtn.SetRectEmpty();
    if (pToolBar != NULL && pToolBar->GetCount() > 0) {
      pCustomizeButton = DYNAMIC_DOWNCAST(CCustomizeButton,
          pToolBar->GetButton(pToolBar->GetCount() - 1));
      if (pCustomizeButton != NULL) {
        rectCustomizeBtn = pCustomizeButton->Rect();
      }
    }
    if (bRoundedCorners) {
      //------------------------
      // Draw bottom/right edge:
      //------------------------
      CPen* pOldPen = im->SelectObject(&s->m_penBottomLine);
      ASSERT(pOldPen != NULL);
      if (bIsHorz) {
        im->MoveTo(rectClient.l + 2, rectClient.b - 1);
        im->LineTo(rectClient.r - rectCustomizeBtn.Width(), rectClient.b - 1);
      }
      else {
        im->MoveTo(rectClient.r - 1, rectClient.t + 2);
        im->LineTo(rectClient.r - 1, rectClient.b - 2 - rectCustomizeBtn.Height());
      }
      im->SelectObject(pOldPen);
    }
    if (pToolBar != NULL && pToolBar->GetCount() > 0) {
      if (pCustomizeButton != NULL && !rectCustomizeBtn.IsRectEmpty() &&
          pCustomizeButton->IsPipeStyle()) {
        BOOL bIsRTL = pBar->GetExStyle() & WS_EX_LAYOUTRTL;
        //----------------------------------------
        // Special drawing for "Customize" button:
        //----------------------------------------
        IRECT rectWindow;
        pBar->GetWindowRect(rectWindow);
        pBar->ClientToScreen(&rectCustomizeBtn);
        IRECT rectButton = rectClient;
        if (pToolBar->IsHorizontal()) {
          if (bIsRTL) {
            int nButtonWidth = rectCustomizeBtn.Width();
            nButtonWidth -=
                rectWindow.l - rectCustomizeBtn.l;
            rectButton.l = rectButton.r - nButtonWidth;
          }
          else {
            rectButton.l = rectButton.r - rectCustomizeBtn.Width() -
                rectWindow.r + rectCustomizeBtn.r;
          }
          pCustomizeButton->SetExtraSize(
              0,
              rectWindow.b - rectCustomizeBtn.b);
        }
        else {
          rectButton.t = rectButton.b - rectCustomizeBtn.Height() -
              rectWindow.b + rectCustomizeBtn.b;
          pCustomizeButton->SetExtraSize(
              rectWindow.r - rectCustomizeBtn.r,
              0);
        }
        BCGBUTTON_STATE state = ButtonsIsRegular;
        if (pToolBar->IsButtonHighlighted(pToolBar->GetCount() - 1) ||
            pCustomizeButton->IsDroppedDown()) {
          state = ButtonsIsHighlighted;
        }
        else if (pCustomizeButton->s->m_nStyle & (TBBS_PRESSED | TBBS_CHECKED)) {
          //-----------------------
          // Pressed in or checked:
          //-----------------------
          state = ButtonsIsPressed;
        }
        DrawCustomizeButton(im, rectButton,
            pToolBar->IsHorizontal(), state,
            (int) pCustomizeButton->GetCustomizeCmdId() > 0,
            !pCustomizeButton->GetInvisibleButtons().IsEmpty());
      }
    }
  }
}
int VisualManager2003_OnDrawBarBorder(img_t* im, BaseControlBar* pBar, IRECT& rect)
{
  ASSERT_VALID(pBar);
  if (pBar->IsDialogControl() || s->m_nBitsPerPixel <= 8 ||
      IsHighContastMode(s)) {
    VisualManagerXP::OnDrawBarBorder(im, pBar, rect);
  }
}
int VisualManager2003_OnDrawBarGripper(img_t* im, IRECT rectGripper, BOOL bHorz,
    BaseControlBar* pBar)
{
  if (pBar != NULL && pBar->IsDialogControl() ||
      s->m_nBitsPerPixel <= 8) {
    VisualManagerXP::OnDrawBarGripper(im, rectGripper, bHorz, pBar);
    return 0;
  }
  const int nBoxSize = 4;
  if (bHorz) {
    rectGripper.l = rectGripper.r - nBoxSize;
  }
  else {
    rectGripper.t = rectGripper.b - nBoxSize;
  }
  ToolBar* pToolBar = DYNAMIC_DOWNCAST(ToolBar, pBar);
  if (pToolBar != NULL) {
    if (bHorz) {
      const int nHeight = ToolBar::IsLargeIcons() ?
          pToolBar->GetRowHeight() : pToolBar->GetButtonSize().h;
      const int nDelta = max(0, (nHeight - pToolBar->GetImageSize().h) / 2);
      rectGripper.DeflateRect(0, nDelta);
    }
    else {
      const int nWidth = ToolBar::IsLargeIcons() ?
          pToolBar->GetColumnWidth() : pToolBar->GetButtonSize().w;
      const int nDelta = max(0, (nWidth - pToolBar->GetImageSize().w) / 2);
      rectGripper.DeflateRect(nDelta, 0);
    }
  }
  const int nBoxesNumber = bHorz ?
      (rectGripper.Height() - nBoxSize) / nBoxSize :
      (rectGripper.Width() - nBoxSize) / nBoxSize;
  int nOffset = bHorz ?
      (rectGripper.Height() - nBoxesNumber * nBoxSize) / 2 :
      (rectGripper.Width() - nBoxesNumber * nBoxSize) / 2;
  for (int nBox = 0; nBox < nBoxesNumber; nBox++) {
    int x = bHorz ?
        rectGripper.l :
        rectGripper.l + nOffset;
    int y = bHorz ?
        rectGripper.t + nOffset :
        rectGripper.t;
    im->FillSolidRect(x + 1, y + 1, nBoxSize / 2, nBoxSize / 2,
        s->clrBtnHilite);
    im->FillSolidRect(x, y, nBoxSize / 2, nBoxSize / 2,
        s->m_clrGripper);
    nOffset += nBoxSize;
  }
}
int VisualManager2003_OnDrawComboBorder(img_t* im, IRECT rect,
    BOOL bDisabled,
    BOOL bIsDropped,
    BOOL bIsHighlighted,
    ToolbarComboBoxButton* pButton)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::OnDrawComboBorder(im, rect,
        bDisabled,
        bIsDropped,
        bIsHighlighted,
        pButton);
    return 0;
  }
  if (bIsHighlighted || bIsDropped || bDisabled) {
    rect.DeflateRect(1, 1);
    COLOR colorBorder = bDisabled ? s->clrBtnShadow : s->m_clrMenuItemBorder;
    if (ToolBarImages::s->m_bIsDrawOnGlass) {
      DrawManager dm(*im);
      dm.DrawRect(rect, (COLOR) - 1, colorBorder);
    }
    else {
      imdraw_Draw3dRect(im, 0, &rect, colorBorder, colorBorder);
    }
  }
}
int VisualManager2003_OnFillOutlookPageButton(img_t* im, const IRECT& rect,
    BOOL bIsHighlighted, BOOL bIsPressed,
    COLOR& clrText)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::OnFillOutlookPageButton(im, rect,
        bIsHighlighted, bIsPressed,
        clrText);
    return 0;
  }
  COLOR clr1 = s->m_clrBarGradientDark;
  COLOR clr2 = s->m_clrBarGradientLight;
  if (bIsPressed) {
    if (bIsHighlighted) {
      clr1 = s->m_clrHighlightDnGradientDark;
      clr2 = s->m_clrHighlightDnGradientLight;
    }
    else {
      clr1 = s->m_clrHighlightDnGradientLight;
      clr2 = s->m_clrHighlightDnGradientDark;
    }
  }
  else if (bIsHighlighted) {
    clr1 = s->m_clrHighlightGradientDark;
    clr2 = s->m_clrHighlightGradientLight;
  }
  DrawManager dm(*im);
  dm.FillGradient(rect, clr1, clr2, TRUE);
  clrText = s->clrBtnText;
}
int VisualManager2003_OnDrawOutlookPageButtonBorder(img_t* im,
    IRECT& rectBtn, BOOL bIsHighlighted, BOOL bIsPressed)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::OnDrawOutlookPageButtonBorder(im,
        rectBtn, bIsHighlighted, bIsPressed);
    return 0;
  }
  imdraw_Draw3dRect(im, 0, rectBtn, s->clrBtnHilite, s->m_clrGripper);
}
int VisualManager2003_OnFillButtonInterior(img_t* im,
    ToolbarButton* pButton, IRECT rect, VisualManager::BCGBUTTON_STATE state)
{
  CCustomizeButton* pCustButton = DYNAMIC_DOWNCAST(CCustomizeButton, pButton);
  if (pCustButton == NULL || !pCustButton->IsPipeStyle() || s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::OnFillButtonInterior(im, pButton, rect, state);
    return 0;
  }
  ToolBar* pToolBar = pCustButton->GetParentToolbar();
  if (pToolBar != NULL) {
    ASSERT_VALID(pToolBar);
    IRECT rectToolbar;
    pToolBar->GetWindowRect(rectToolbar);
    pToolBar->ScreenToClient(rectToolbar);
    if (pToolBar->IsHorizontal()) {
      rect.r = rectToolbar.r;
    }
    else {
      rect.b = rectToolbar.b;
    }
  }
  CSize sizeExtra = pCustButton->GetExtraSize();
  rect.InflateRect(sizeExtra);
  DrawCustomizeButton(im, rect, pToolBar->IsHorizontal(), state,
      (int) pCustButton->GetCustomizeCmdId() > 0,
      !pCustButton->GetInvisibleButtons().IsEmpty());
  pCustButton->SetDefaultDraw(FALSE);
}
int VisualManager2003_OnDrawButtonBorder(img_t* im,
    ToolbarButton* pButton, IRECT rect, BCGBUTTON_STATE state)
{
  CCustomizeButton* pCustButton = DYNAMIC_DOWNCAST(CCustomizeButton, pButton);
  if (pCustButton == NULL || !pCustButton->IsPipeStyle() || s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::OnDrawButtonBorder(im, pButton, rect, state);
  }
  // Do nothing - the border is already painted in OnFillButtonInterior
}
int VisualManager2003_OnDrawSeparator(img_t* im, BaseControlBar* pBar,
    IRECT rect, BOOL bHorz)
{
  ASSERT_VALID(pBar);
  if (pBar->IsDialogControl() ||
      pBar->IsKindOf(RUNTIME_CLASS(PopupMenuBar)) ||
      s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::OnDrawSeparator(im, pBar, rect, bHorz);
    return 0;
  }
#ifndef BCGP_EXCLUDE_RIBBON
  if (pBar->IsKindOf(RUNTIME_CLASS(RibbonStatusBar))) {
    if (s->m_hThemeStatusBar == NULL) {
      VisualManagerXP::OnDrawSeparator(im, pBar, rect, bHorz);
      return 0;
    }
    rect.InflateRect(1, 5);
    (*s->m_pfDrawThemeBackground)(s->m_hThemeStatusBar, im, 1 /*SP_PANE*/,
        0, &rect, 0);
    return 0;
  }
#endif
  ToolBar* pToolBar = DYNAMIC_DOWNCAST(ToolBar, pBar);
  if (pToolBar == NULL) {
    VisualManagerXP::OnDrawSeparator(im, pBar, rect, bHorz);
    return 0;
  }
  CPen* pOldPen = im->SelectObject(&s->m_penSeparator);
  ASSERT(pOldPen != NULL);
  if (bHorz) {
    const int nDelta = max(0, (pToolBar->GetButtonSize().h - pToolBar->GetImageSize().h) / 2);
    rect.DeflateRect(0, nDelta);
    int x = rect.l += rect.Width() / 2 - 1;
    if (ToolBarImages::s->m_bIsDrawOnGlass) {
      DrawManager dm(*im);
      dm.DrawLine(x, rect.t, x, rect.b - 1, s->m_clrSeparator);
      dm.DrawLine(x + 1, rect.t + 1, x + 1, rect.b, s->clrBarHilite);
    }
    else {
      im->MoveTo(x, rect.t);
      im->LineTo(x, rect.b - 1);
      im->SelectObject(&s->m_penSeparatorLight);
      im->MoveTo(x + 1, rect.t + 1);
      im->LineTo(x + 1, rect.b);
    }
  }
  else {
    const int nDelta = max(0, (pToolBar->GetButtonSize().w - pToolBar->GetImageSize().w) / 2);
    rect.DeflateRect(nDelta, 0);
    int y = rect.t += rect.Height() / 2 - 1;
    if (ToolBarImages::s->m_bIsDrawOnGlass) {
      DrawManager dm(*im);
      dm.DrawLine(rect.l, y, rect.r - 1, y, s->m_clrSeparator);
      dm.DrawLine(rect.l + 1, y + 1, rect.r, y + 1, s->clrBarHilite);
    }
    else {
      im->MoveTo(rect.l, y);
      im->LineTo(rect.r - 1, y);
      im->SelectObject(&s->m_penSeparatorLight);
      im->MoveTo(rect.l + 1, y + 1);
      im->LineTo(rect.r, y + 1);
    }
  }
  im->SelectObject(pOldPen);
}
int VisualManager2003_OnUpdateSystemColors()
{
  WinXPThemeManager::UpdateSystemColors();
  BOOL bIsAppThemed = s->m_bUseGlobalTheme || (s->m_pfGetWindowTheme != NULL &&
      (*s->m_pfGetWindowTheme)(AfxGetMainWnd()->GetSafeHwnd()) != NULL);
  s->m_WinXPTheme = bIsAppThemed ? GetStandardWinXPTheme() : WinXpTheme_None;
  if (!s->m_bDefaultWinXPColors && s->m_WinXPTheme != WinXpTheme_None) {
    s->m_WinXPTheme = WinXpTheme_NonStandard;
  }
  s->m_bIsStandardWinXPTheme =
      s->m_WinXPTheme == WinXpTheme_Blue ||
      s->m_WinXPTheme == WinXpTheme_Olive ||
      s->m_WinXPTheme == WinXpTheme_Silver;
  //----------------------
  // Modify global colors:
  //----------------------
  ModifyGlobalColors();
  VisualManagerXP::OnUpdateSystemColors();
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    s->m_clrTaskPaneGradientDark = s->clrWindow;
    s->m_clrTaskPaneGradientLight = s->clrWindow;
    s->m_clrTaskPaneGroupCaptionDark = s->clrBarFace;
    s->m_clrTaskPaneGroupCaptionLight = s->clrBarFace;
    s->m_clrTaskPaneGroupCaptionSpecDark = s->clrBarFace;
    s->m_clrTaskPaneGroupCaptionSpecLight = s->clrBarFace;
    s->m_clrTaskPaneGroupAreaLight = s->clrWindow;
    s->m_clrTaskPaneGroupAreaDark = s->clrWindow;
    s->m_clrTaskPaneGroupAreaSpecLight = s->clrWindow;
    s->m_clrTaskPaneGroupAreaSpecDark = s->clrWindow;
    s->m_clrTaskPaneGroupBorder = s->clrBtnShadow;
    s->m_clrBarGradientLight = s->m_clrToolBarGradientLight = s->clrBarLight;
    s->m_penTaskPaneGroupBorder.DeleteObject();
    s->m_penTaskPaneGroupBorder.CreatePen(PS_SOLID, 1, s->m_clrTaskPaneGroupBorder);
    s->m_penGridExpandBoxLight.DeleteObject();
    s->m_penGridExpandBoxLight.CreatePen(PS_SOLID, 1, s->clrBtnLight);
    s->m_penGridExpandBoxDark.DeleteObject();
    s->m_penGridExpandBoxDark.CreatePen(PS_SOLID, 1, s->clrBtnShadow);
    s->m_clrToolbarDisabled = s->clrBtnHilite;
    return 0;
  }
  //--------------------------------------------------
  // Calculate control bars bakground gradient colors:
  //--------------------------------------------------
  COLOR clrBase = GetBaseThemeColor();
  if (s->m_WinXPTheme == WinXpTheme_Olive) {
    s->m_clrToolBarGradientDark = PixelAlpha2(
        clrBase, 120);
    s->m_clrBarGradientDark = DrawManager::SmartMixColors(
        clrBase,
        GetThemeColor(s->m_hThemeWindow, COLOR_3DFACE),
        .87, 1, 3);
    s->m_clrToolBarGradientLight = DrawManager::SmartMixColors(
        clrBase,
        GetThemeColor(s->m_hThemeWindow, COLOR_WINDOW),
        1., 2, 1);
    s->m_clrBarGradientLight = DrawManager::SmartMixColors(
        clrBase,
        GetThemeColor(s->m_hThemeWindow, COLOR_WINDOW),
        1.03);
  }
  else if (s->m_WinXPTheme == WinXpTheme_Silver) {
    s->m_clrToolBarGradientDark = DrawManager::SmartMixColors(
        clrBase,
        GetThemeColor(s->m_hThemeWindow, COLOR_3DFACE),
        0.75, 2);
    s->m_clrBarGradientDark = PixelAlpha2(
        clrBase, 120);
    s->m_clrToolBarGradientLight = DrawManager::SmartMixColors(
        clrBase,
        GetThemeColor(s->m_hThemeWindow, COLOR_3DHIGHLIGHT),
        .98);
    s->m_clrBarGradientLight = DrawManager::SmartMixColors(
        clrBase,
        GetThemeColor(s->m_hThemeWindow, COLOR_WINDOW),
        1.03);
  }
  else if (s->m_WinXPTheme == WinXpTheme_Blue) {
    s->m_clrToolBarGradientDark = DrawManager::SmartMixColors(
        clrBase,
        GetThemeColor(s->m_hThemeWindow, COLOR_3DFACE),
        0.93, 2);
    s->m_clrBarGradientDark = DrawManager::SmartMixColors(
        clrBase,
        GetThemeColor(s->m_hThemeWindow, COLOR_3DLIGHT),
        .99, 2, 1);
    s->m_clrToolBarGradientLight = DrawManager::SmartMixColors(
        clrBase,
        GetThemeColor(s->m_hThemeWindow, COLOR_WINDOW),
        1.03);
    s->m_clrBarGradientLight = s->m_clrToolBarGradientLight;
  }
  else {
    s->m_clrToolBarGradientDark = DrawManager::SmartMixColors(
        clrBase,
        GetThemeColor(s->m_hThemeWindow, COLOR_3DFACE),
        0.93, 2);
    s->m_clrBarGradientDark = DrawManager::SmartMixColors(
        clrBase,
        GetThemeColor(s->m_hThemeWindow, COLOR_3DLIGHT),
        .99, 2, 1);
    s->m_clrToolBarGradientLight = DrawManager::SmartMixColors(
        clrBase,
        GetThemeColor(s->m_hThemeWindow, COLOR_WINDOW),
        1., 1, 4);
    s->m_clrBarGradientLight = s->m_clrToolBarGradientLight;
  }
  s->m_clrToolBarGradientVertLight = s->m_clrToolBarGradientLight;
  s->m_clrToolBarGradientVertDark = PixelAlpha2(
      s->m_clrToolBarGradientDark, 98);
  COLOR clrSeparatorDark;
  COLOR clrPlannerTodayLine;
  //-------------------------------------
  // Calculate highlight gradient colors:
  //-------------------------------------
  if (s->m_bIsStandardWinXPTheme) {
    ASSERT(s->m_pfGetThemeColor != NULL);
    COLOR clr1, clr2, clr3;
    if (s->m_WinXPTheme == WinXpTheme_Blue && s->bIsWindowsVista) {
      clr1 = _RGB(250, 196, 88);
      clr2 = _RGB(250, 196, 88);
      clr3 = _RGB(228, 93, 61);
    }
    else {
      (*s->m_pfGetThemeColor)(s->m_hThemeButton, BP_PUSHBUTTON, 0, TMT_ACCENTCOLORHINT, &clr1);
      (*s->m_pfGetThemeColor)(s->m_hThemeButton, BP_RADIOBUTTON, 0, TMT_ACCENTCOLORHINT, &clr2);
      (*s->m_pfGetThemeColor)(s->m_hThemeWindow, WP_CLOSEBUTTON, 0, TMT_FILLCOLORHINT, &clr3);
    }
    s->m_clrHighlightMenuItem = DrawManager::SmartMixColors(
        clr1,
        clr2,
        1.3, 1, 1);
    s->m_clrHighlightGradientLight = DrawManager::SmartMixColors(
        clr1,
        clr3,
        1.55, 2, 1);
    s->m_clrHighlightGradientDark = DrawManager::SmartMixColors(
        clr1,
        clr2,
        1.03, 2, 1);
    s->m_clrHighlightDnGradientLight = DrawManager::SmartMixColors(
        clr1,
        clr3,
        1.03, 1, 2);
    s->m_clrFloatToolBarBorder.DeleteObject();
    COLOR clrCustom;
    (*s->m_pfGetThemeColor)(s->m_hThemeButton, BP_RADIOBUTTON, 0, TMT_BORDERCOLORHINT, &clrCustom);
    COLOR clrToolbarBorder = DrawManager::SmartMixColors(
        clrCustom,
        clrBase,
        0.84, 1, 4);
    s->m_clrFloatToolBarBorder.CreateSolidBrush(clrToolbarBorder);
    if (s->m_WinXPTheme == WinXpTheme_Blue || s->m_WinXPTheme == WinXpTheme_Silver) {
      s->m_clrCustomizeButtonGradientDark = s->bIsWindowsVista ? _RGB(7, 57, 142) :
          PixelAlpha2(s->clrActiveCaption, 60);
      const double k = (s->m_WinXPTheme == WinXpTheme_Blue) ? 1.61 : 1;
      s->m_clrCustomizeButtonGradientLight = DrawManager::SmartMixColors(
          s->m_clrCustomizeButtonGradientDark,
          clrBase, k, 3, 1);
      (*s->m_pfGetThemeColor)(s->m_hThemeButton, BP_PUSHBUTTON, PBS_DEFAULTED, TMT_ACCENTCOLORHINT, &clrCustom);
    }
    else {
      s->m_clrCustomizeButtonGradientDark = DrawManager::SmartMixColors(
          clrCustom,
          clrBase,
          0.63, 1, 3);
      (*s->m_pfGetThemeColor)(s->m_hThemeButton, BP_PUSHBUTTON, PBS_DEFAULTED, TMT_ACCENTCOLORHINT, &clrCustom);
      s->m_clrCustomizeButtonGradientLight = DrawManager::SmartMixColors(
          clrCustom,
          clrBase,
          1.2, 1, 3);
    }
    s->clrBarDkShadow = s->m_clrCustomizeButtonGradientDark;
    if (s->m_WinXPTheme != WinXpTheme_Silver) {
      s->clrBarShadow = DrawManager::SmartMixColors(
          clrCustom,
          clrBase,
          1.4, 1, 3);
    }
    s->m_clrToolBarBottomLine = s->m_WinXPTheme == WinXpTheme_Silver ?
        PixelAlpha2(s->m_clrToolBarGradientDark, 80) :
        PixelAlpha2(s->m_clrToolBarGradientDark, 50);
    s->m_colorToolBarCornerTop = PixelAlpha2(s->m_clrToolBarGradientLight, 92);
    s->m_colorToolBarCornerBottom = PixelAlpha2(s->m_clrToolBarGradientDark, 97);
    s->m_clrGripper =
        PixelAlpha2(s->m_clrToolBarGradientVertDark, 40);
    clrSeparatorDark =
        PixelAlpha2(s->m_clrToolBarGradientVertDark, 81);
    s->m_clrMenuItemBorder = s->m_clrGripper;
    s->m_clrMenuBorder =
        PixelAlpha2(clrBase, 80);
    s->m_clrCaptionBarGradientDark = s->m_clrCustomizeButtonGradientDark;
    s->m_clrCaptionBarGradientLight = s->m_clrCustomizeButtonGradientLight;
    s->m_clrPlannerTodayFill = _RGB(249, 203, 95);
    clrPlannerTodayLine = _RGB(187, 85, 3);
    s->m_clrMenuLight = PixelAlpha2(
        s->clrWindow, .96, .96, .96);
    s->m_clrMenuLight.DeleteObject();
    s->m_clrMenuLight.CreateSolidBrush(s->m_clrMenuLight);
  }
  else {
    s->m_clrHighlightMenuItem = (COLOR) - 1;
    s->m_clrHighlightGradientLight = s->m_clrHighlight;
    s->m_clrHighlightGradientDark = s->m_clrHighlightDn;
    s->m_clrHighlightDnGradientLight =
        PixelAlpha2(s->m_clrHighlightGradientLight, 120);
    s->m_clrCustomizeButtonGradientDark = s->clrBarShadow;
    s->m_clrCustomizeButtonGradientLight = DrawManager::SmartMixColors(
        s->m_clrCustomizeButtonGradientDark,
        s->clrBarFace, 1, 1, 1);
    s->m_clrToolBarBottomLine = PixelAlpha2(s->m_clrToolBarGradientDark, 75);
    s->m_colorToolBarCornerTop = s->clrBarLight;
    s->m_colorToolBarCornerBottom = s->m_clrToolBarGradientDark;
    s->m_clrGripper = s->clrBarShadow;
    clrSeparatorDark = s->clrBarShadow;
    s->m_clrCaptionBarGradientLight = s->clrBarShadow;
    s->m_clrCaptionBarGradientDark = s->clrBarDkShadow;
    s->m_clrPlannerTodayFill = s->m_clrHighlight;
    clrPlannerTodayLine = s->m_clrMenuItemBorder;
  }
  s->m_clrHighlightDnGradientDark = s->m_clrHighlightGradientDark;
  s->m_clrHighlightCheckedGradientLight = s->m_clrHighlightDnGradientDark;
  s->m_clrHighlightCheckedGradientDark =
      PixelAlpha2(s->m_clrHighlightDnGradientLight, 120);
  s->m_clrTabBack.DeleteObject();
  s->m_clrTabBack.CreateSolidBrush(s->m_clrToolBarGradientLight);
  s->m_penSeparatorLight.DeleteObject();
  s->m_penSeparatorLight.CreatePen(PS_SOLID, 1, s->clrBarHilite);
  s->m_clrTearOffCaption.DeleteObject();
  s->m_clrTearOffCaption.CreateSolidBrush(s->clrBarFace);
  s->m_clrFace.DeleteObject();
  s->m_clrFace.CreateSolidBrush(s->m_clrToolBarGradientLight);
  s->m_penSeparator.DeleteObject();
  s->m_penSeparator.CreatePen(PS_SOLID, 1, clrSeparatorDark);
  s->m_clrMenuShadowBase = s->clrBarFace;
  s->m_clrToolbarDisabled = DrawManager::SmartMixColors(
      s->m_clrToolBarGradientDark, s->m_clrToolBarGradientLight, .92);
  s->m_penBottomLine.DeleteObject();
  s->m_penBottomLine.CreatePen(PS_SOLID, 1, s->m_clrToolBarBottomLine);
  // --------------------------
  // Calculate TaskPane colors:
  // --------------------------
  if (s->m_bIsStandardWinXPTheme && s->m_hThemeExplorerBar != NULL) {
    ASSERT(s->m_pfGetThemeColor != NULL);
    (*s->m_pfGetThemeColor)(s->m_hThemeExplorerBar, 0, 0, TMT_GRADIENTCOLOR1, s->m_clrTaskPaneGradientLight);
    (*s->m_pfGetThemeColor)(s->m_hThemeExplorerBar, 0, 0, TMT_GRADIENTCOLOR2, s->m_clrTaskPaneGradientDark);
    (*s->m_pfGetThemeColor)(s->m_hThemeExplorerBar, EBP_NORMALGROUPBACKGROUND, 0, TMT_FILLCOLOR, s->m_clrTaskPaneGroupCaptionDark);
    (*s->m_pfGetThemeColor)(s->m_hThemeExplorerBar, EBP_SPECIALGROUPHEAD, 0, TMT_FILLCOLOR, s->m_clrTaskPaneGroupCaptionSpecDark);
    s->m_clrTaskPaneGroupCaptionSpecLight = s->m_clrTaskPaneGroupCaptionDark;
    (*s->m_pfGetThemeColor)(s->m_hThemeExplorerBar, EBP_NORMALGROUPBACKGROUND, 0, TMT_FILLCOLOR, s->m_clrTaskPaneGroupAreaLight);
    s->m_clrTaskPaneGroupAreaDark = s->m_clrTaskPaneGroupAreaLight;
    (*s->m_pfGetThemeColor)(s->m_hThemeExplorerBar, EBP_SPECIALGROUPBACKGROUND, 0, TMT_FILLCOLORHINT, s->m_clrTaskPaneGroupAreaSpecLight);
    s->m_clrTaskPaneGroupAreaSpecDark = s->m_clrTaskPaneGroupAreaSpecLight;
    (*s->m_pfGetThemeColor)(s->m_hThemeExplorerBar, EBP_NORMALGROUPBACKGROUND, 0, TMT_BORDERCOLOR, s->m_clrTaskPaneGroupBorder);
    s->m_clrTaskPaneGroupCaptionLight = s->m_clrTaskPaneGroupBorder;
    if (s->m_WinXPTheme == WinXpTheme_Blue && s->bIsWindowsVista) {
      s->m_clrTaskPaneGradientDark = s->m_clrBarGradientDark;
      s->m_clrTaskPaneGradientLight = s->m_clrBarGradientLight;
      s->m_clrTaskPaneGroupCaptionSpecDark = s->m_clrToolBarGradientDark;
      s->m_clrTaskPaneGroupCaptionSpecLight = s->m_clrToolBarGradientLight;
    }
  }
  else {
    s->m_clrTaskPaneGradientDark = s->m_clrBarGradientDark;
    s->m_clrTaskPaneGradientLight = s->m_clrToolBarGradientLight;
    s->m_clrTaskPaneGroupCaptionDark = s->m_clrBarGradientDark;
    s->m_clrTaskPaneGroupCaptionLight = s->m_clrToolBarGradientLight;
    COLOR clrLight =
        PixelAlpha2(s->clrBarShadow, 125);
    s->m_clrTaskPaneGroupCaptionSpecDark = DrawManager::SmartMixColors(
        s->m_clrCustomizeButtonGradientDark,
        clrLight);
    s->m_clrTaskPaneGroupCaptionSpecLight = s->m_clrCustomizeButtonGradientLight;
    s->m_clrTaskPaneGroupAreaLight = s->m_clrToolBarGradientLight;
    s->m_clrTaskPaneGroupAreaDark = s->m_clrToolBarGradientLight;
    s->m_clrTaskPaneGroupAreaSpecLight = s->m_clrToolBarGradientLight;
    s->m_clrTaskPaneGroupAreaSpecDark = s->m_clrToolBarGradientLight;
    s->m_clrTaskPaneGroupBorder = s->m_clrToolBarGradientLight;
  }
  s->m_penTaskPaneGroupBorder.DeleteObject();
  s->m_penTaskPaneGroupBorder.CreatePen(PS_SOLID, 1, s->m_clrTaskPaneGroupBorder);
  s->m_clrTaskPaneHotText = s->clrHilite;
  //--------------------------------------
  // Calculate grid/report control colors:
  //--------------------------------------
  s->m_penGridExpandBoxLight.DeleteObject();
  s->m_penGridExpandBoxLight.CreatePen(PS_SOLID, 1,
      PixelAlpha2(s->m_clrToolBarBottomLine, 210));
  s->m_penGridExpandBoxDark.DeleteObject();
  s->m_penGridExpandBoxDark.CreatePen(PS_SOLID, 1,
      PixelAlpha2(s->m_clrToolBarBottomLine, 75));
  //--------------------------
  // Calculate planner colors:
  //--------------------------
  s->m_clrPlannerWork = _RGB(255, 255, 213);
  s->m_penPlannerTodayLine.DeleteObject();
  s->m_penPlannerTodayLine.CreatePen(PS_SOLID, 1, clrPlannerTodayLine);
}
int VisualManager2003_OnFillHighlightedArea(img_t* im, IRECT rect,
    COLOR pBrush, ToolbarButton* pButton)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::OnFillHighlightedArea(im, rect, pBrush, pButton);
    return 0;
  }
  BOOL bIsHorz = TRUE;
  BOOL bIsPopupMenu = FALSE;
  COLOR clr1 = (COLOR) - 1;
  COLOR clr2 = (COLOR) - 1;
  if (pButton != NULL) {
    bIsHorz = pButton->IsHorizontal();
    ToolbarMenuButton* pMenuButton =
        DYNAMIC_DOWNCAST(ToolbarMenuButton, pButton);
    bIsPopupMenu = pMenuButton != NULL &&
        pMenuButton->GetParentWnd() != NULL &&
        pMenuButton->GetParentWnd()->IsKindOf(RUNTIME_CLASS(PopupMenuBar));
    if (bIsPopupMenu && pBrush == s->m_clrHighlight) {
      if (s->m_clrHighlightMenuItem != (COLOR) - 1) {
        CBrush br(s->m_clrHighlightMenuItem);
        imdraw_FillRect(im, 0, &rect, &br);
        return 0;
      }
    }
    if (pMenuButton != NULL &&
        !bIsPopupMenu &&
        pMenuButton->IsDroppedDown()) {
      clr1 = PixelAlpha2(
          s->m_clrToolBarGradientDark,
          s->m_bIsStandardWinXPTheme ? 101 : 120);
      clr2 = PixelAlpha2(
          s->m_clrToolBarGradientLight, 110);
    }
  }
  if (pBrush == s->m_clrHighlight && s->m_bIsStandardWinXPTheme) {
    clr1 = s->m_clrHighlightGradientDark;
    clr2 = bIsPopupMenu ? clr1 : s->m_clrHighlightGradientLight;
  }
  else if (pBrush == s->m_clrHighlightDn && s->m_bIsStandardWinXPTheme) {
    clr1 = bIsPopupMenu ? s->m_clrHighlightDnGradientLight : s->m_clrHighlightDnGradientDark;
    clr2 = s->m_clrHighlightDnGradientLight;
  }
  else if (pBrush == s->m_clrHighlightChecked && s->m_bIsStandardWinXPTheme) {
    clr1 = bIsPopupMenu ? s->m_clrHighlightCheckedGradientLight : s->m_clrHighlightCheckedGradientDark;
    clr2 = s->m_clrHighlightCheckedGradientLight;
  }
  if (clr1 == (COLOR) - 1 || clr2 == (COLOR) - 1) {
    VisualManagerXP::OnFillHighlightedArea(im, rect, pBrush, pButton);
    return 0;
  }
  DrawManager dm(*im);
  dm.FillGradient(rect, clr1, clr2, bIsHorz);
}
int VisualManager2003_OnDrawShowAllMenuItems(img_t* im, IRECT rect,
    VisualManager::BCGBUTTON_STATE state)
{
  if (s->m_nBitsPerPixel > 8 && !IsHighContastMode(s)) {
    const int nRadius = 8;
    rect = IRECT(rect.CenterPoint() - CSize(nRadius - 1, nRadius - 1), CSize(nRadius * 2, nRadius * 2));
    DrawManager dm(*im);
    dm.DrawGradientRing(rect, s->m_clrToolBarGradientDark, s->m_clrMenuLight,
        (COLOR) - 1,
        45, nRadius);
  }
  VisualManager::OnDrawShowAllMenuItems(im, rect, state);
}
int VisualManager2003_GetShowAllMenuItemsHeight(img_t* im, const CSize& sizeDefault)
{
  int nHeight = VisualManager::GetShowAllMenuItemsHeight(im, sizeDefault);
  return nHeight + 4;
}
int VisualManager2003_OnDrawCaptionBarBorder(img_t* im,
    CaptionBar* pBar, IRECT rect, COLOR clrBarBorder, BOOL bFlatBorder)
{
  if (clrBarBorder == (COLOR) - 1) {
    imdraw_FillRect(im, 0, rect,
        (pBar != NULL && pBar->IsDialogControl()) ? &s->clrBtnFace : &s->clrBarFace);
  }
  else {
    CBrush brBorder(clrBarBorder);
    imdraw_FillRect(im, 0, rect, &brBorder);
  }
  if (!bFlatBorder) {
    imdraw_Draw3dRect(im, 0, rect, s->m_clrBarGradientLight, s->m_clrToolBarBottomLine);
  }
}
int VisualManager2003_OnDrawComboDropButton(img_t* im, IRECT rect,
    BOOL bDisabled,
    BOOL bIsDropped,
    BOOL bIsHighlighted,
    ToolbarComboBoxButton* pButton)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::OnDrawComboDropButton(im, rect,
        bDisabled, bIsDropped,
        bIsHighlighted, pButton);
    return 0;
  }
  ASSERT_VALID(this);
  const BOOL bIsCtrl = pButton != NULL && pButton->IsCtrlButton();
  if (!bDisabled) {
    if (bIsDropped || bIsHighlighted) {
      OnFillHighlightedArea(im, rect,
          bIsDropped ? s->m_clrHighlightDn : s->m_clrHighlight,
          NULL);
      if (!bIsCtrl) {
        if (ToolBarImages::s->m_bIsDrawOnGlass) {
          DrawManager dm(*im);
          dm.DrawLine(rect.l, rect.t, rect.l, rect.b, s->m_clrMenuItemBorder);
        }
        else {
          CPen pen(PS_SOLID, 1, s->m_clrMenuItemBorder);
          CPen* pOldPen = im->SelectObject(&pen);
          ASSERT(pOldPen != NULL);
          im->MoveTo(rect.l, rect.t);
          im->LineTo(rect.l, rect.b);
          im->SelectObject(pOldPen);
        }
      }
    }
    else {
      DrawManager dm(*im);
      dm.FillGradient(rect, s->m_clrToolBarGradientDark, s->m_clrToolBarGradientLight, TRUE);
      if (ToolBarImages::s->m_bIsDrawOnGlass) {
        dm.DrawRect(rect, (COLOR) - 1, s->clrWindow);
      }
      else {
        imdraw_Draw3dRect(im, 0, rect, s->clrWindow, s->clrWindow);
      }
    }
  }
  MenuImages::Draw(im, MenuImages::IdArowDown, rect,
      bDisabled ? MenuImages::ImageGray : (bIsDropped && bIsHighlighted) ?
      MenuImages::ImageWhite : MenuImages::ImageBlack);
}
int VisualManager2003_OnDrawTearOffCaption(img_t* im, IRECT rect, BOOL bIsActive)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::OnDrawTearOffCaption(im, rect, bIsActive);
    return 0;
  }
  const int iBorderSize = 1;
  imdraw_FillRect(im, 0, rect, s->m_clrMenuLight);
  rect.DeflateRect(iBorderSize, 1);
  if (bIsActive) {
    OnFillHighlightedArea(im, rect, bIsActive ? s->m_clrHighlight : s->m_clrBarBkgnd,
        NULL);
  }
  else {
    imdraw_FillRect(im, 0, rect, s->m_clrTearOffCaption);
  }
  // Draw gripper:
  OnDrawBarGripper(im, rect, FALSE, NULL);
  if (bIsActive) {
    imdraw_Draw3dRect(im, 0, rect, s->m_clrMenuBorder, s->m_clrMenuBorder);
  }
}
int VisualManager2003_OnDrawMenuBorder(img_t* im,
    PopupMenu* pMenu, IRECT rect)
{
  BOOL bConnectMenuToParent = s->m_bConnectMenuToParent;
  if (DYNAMIC_DOWNCAST(CCustomizeButton, pMenu->GetParentButton()) != NULL) {
    s->m_bConnectMenuToParent = FALSE;
  }
  VisualManagerXP::OnDrawMenuBorder(im, pMenu, rect);
  s->m_bConnectMenuToParent = bConnectMenuToParent;
}
COLOR VisualManager2003_GetThemeColor(HTHEME hTheme, int nIndex) const
{
  if (hTheme != NULL && s->m_pfGetThemeSysColor != NULL) {
    return(*s->m_pfGetThemeSysColor)(hTheme, nIndex);
  }
  return GetSysColor(nIndex);
}
int VisualManager2003_OnEraseTabsArea(img_t* im, IRECT rect,
    const BaseTabWnd* pTabWnd)
{
  ASSERT_VALID(pTabWnd);
  if (pTabWnd->IsDialogControl()) {
    if (pTabWnd->IsVisualManagerStyle()) {
      OnFillDialog(im, pTabWnd->GetParent(), rect);
    }
    else {
      imdraw_FillRect(im, 0, rect, &s->clrBtnFace);
    }
    return 0;
  }
  if (pTabWnd->IsFlatTab() || s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::OnEraseTabsArea(im, rect, pTabWnd);
    return 0;
  }
  DrawManager dm(*im);
  COLOR clr1 = s->m_clrToolBarGradientDark;
  COLOR clr2 = s->m_clrToolBarGradientLight;
  if (pTabWnd->GetLocation() == BaseTabWnd::LOCATION_BOTTOM) {
    dm.FillGradient(rect, clr1, clr2, TRUE);
  }
  else {
    dm.FillGradient(rect, clr2, clr1, TRUE);
  }
}
int VisualManager2003_OnDrawTab(img_t* im, IRECT rectTab,
    int iTab, BOOL bIsActive, const BaseTabWnd* pTabWnd)
{
  ASSERT_VALID(pTabWnd);
  if (!pTabWnd->IsOneNoteStyle() || s->m_nBitsPerPixel <= 8 ||
      IsHighContastMode(s) || pTabWnd->IsLeftRightRounded()) {
    VisualManagerXP::OnDrawTab(im, rectTab, iTab, bIsActive, pTabWnd);
    return 0;
  }
  IRECT rectClip;
  pTabWnd->GetTabsRect(rectClip);
  const int nExtra = (iTab == 0 || bIsActive) ? 0 : rectTab.Height();
  if (rectTab.l + nExtra + 10 > rectClip.r ||
      rectTab.r - 10 <= rectClip.l) {
    return 0;
  }
  const BOOL bIsHighlight = iTab == pTabWnd->GetHighlightedTab();
  COLOR clrTab = pTabWnd->GetTabBkColor(iTab);
  if (clrTab == (COLOR) - 1 && bIsActive) {
    clrTab = s->clrWindow;
  }
  if (pTabWnd->GetLocation() == BaseTabWnd::LOCATION_BOTTOM) {
    rectTab.OffsetRect(0, -1);
  }
  IRECT rectFill = rectTab;
#define POINTS_NUM 5
  IPOINT pts [POINTS_NUM];
  const int nHeight = rectFill.Height();
  pts [0].x = rectFill.l;
  pts [0].y = rectFill.b;
  pts [1].x = rectFill.l + nHeight;
  pts [1].y = rectFill.t;
  pts [2].x = rectFill.r - 2;
  pts [2].y = rectFill.t;
  pts [3].x = rectFill.r;
  pts [3].y = rectFill.t + 2;
  pts [4].x = rectFill.r;
  pts [4].y = rectFill.b;
  BOOL bIsCutted = FALSE;
  for (int i = 0; i < POINTS_NUM; i++) {
    if (pts [i].x > rectClip.r) {
      pts [i].x = rectClip.r;
      bIsCutted = TRUE;
    }
    if (pTabWnd->GetLocation() == BaseTabWnd::LOCATION_BOTTOM) {
      pts [i].y = rectFill.b - pts [i].y + rectFill.t;
    }
  }
  CRgn rgn;
  rgn.CreatePolygonRgn(pts, POINTS_NUM, WINDING);
  im->SelectClipRgn(&rgn);
  IRECT rectLeft;
  pTabWnd->GetClientRect(rectLeft);
  rectLeft.r = rectClip.l - 1;
  im->ExcludeClipRect(rectLeft);
  DrawManager dm(*im);
  COLOR clrFill = bIsHighlight ? s->m_clrHighlightMenuItem : clrTab;
  COLOR clr2;
  if (clrFill != (COLOR) - 1) {
    clr2 = PixelAlpha2(clrFill, 1.22, 1.22, 1.22);
  }
  else {
    clrFill = s->m_clrToolBarGradientDark;
    clr2 = s->m_clrToolBarGradientLight;
  }
  if (pTabWnd->GetLocation() == TabWnd::LOCATION_BOTTOM) {
    rectFill.t++;
  }
  IRECT rectTop = rectFill;
  rectTop.b = rectTop.CenterPoint().y - 1;
  CBrush brTop(clr2);
  imdraw_FillRect(im, 0, rectTop, &brTop);
  IRECT rectMiddle = rectFill;
  rectMiddle.t = rectTop.b;
  rectMiddle.b = rectMiddle.t + 3;
  dm.FillGradient(rectMiddle, clrFill, clr2);
  IRECT rectBottom = rectFill;
  rectBottom.t = rectMiddle.b;
  CBrush brBottom(clrFill);
  imdraw_FillRect(im, 0, rectBottom, &brBottom);
  im->SelectClipRgn(NULL);
  im->ExcludeClipRect(rectLeft);
  if (iTab > 0 && !bIsActive && iTab != pTabWnd->GetFirstVisibleTabNum()) {
    IRECT rectLeftTab = rectClip;
    rectLeftTab.r = rectFill.l + rectFill.Height() - 10;
    if (pTabWnd->GetLocation() == TabWnd::LOCATION_BOTTOM) {
      rectLeftTab.t -= 2;
    }
    else {
      rectLeftTab.b++;
    }
    im->ExcludeClipRect(rectLeftTab);
  }
  CPen penGray(PS_SOLID, 1, s->clrBarDkShadow);
  CPen penShadow(PS_SOLID, 1, s->clrBarShadow);
  CPen* pOldPen = im->SelectObject(&penGray);
  COLOR pOldBrush = (COLOR) im->SelectStockObject(NULL_BRUSH);
  im->Polyline(pts, POINTS_NUM);
  if (bIsCutted) {
    im->MoveTo(rectClip.r, rectTab.t);
    im->LineTo(rectClip.r, rectTab.b);
  }
  IRECT rectRight = rectClip;
  rectRight.l = rectFill.r;
  im->ExcludeClipRect(rectRight);
  CPen penLight(PS_SOLID, 1, s->clrBarHilite);
  im->SelectObject(&penLight);
  if (pTabWnd->GetLocation() == BaseTabWnd::LOCATION_BOTTOM) {
  }
  else {
    im->MoveTo(rectFill.l + 1, rectFill.b);
    im->LineTo(rectFill.l + nHeight, rectFill.t + 1);
    im->LineTo(rectFill.r - 1, rectFill.t + 1);
  }
  im->SelectObject(pOldPen);
  im->SelectObject(pOldBrush);
  if (bIsActive) {
    const int iBarHeight = 1;
    const int y = (pTabWnd->GetLocation() == BaseTabWnd::LOCATION_BOTTOM) ?
        (rectTab.t - iBarHeight) : (rectTab.b);
    IRECT rectFill(CPoint(rectTab.l + 2, y),
        CSize(rectTab.Width() - 1, iBarHeight));
    if (pTabWnd->GetLocation() == BaseTabWnd::LOCATION_BOTTOM) {
      rectFill.OffsetRect(-1, 1);
    }
    rectFill.r = min(rectFill.r, rectClip.r);
    CBrush br(clrTab);
    imdraw_FillRect(im, 0, rectFill, &br);
  }
  if (pTabWnd->GetLocation() == BaseTabWnd::LOCATION_BOTTOM) {
    rectTab.l += rectTab.Height() + BaseTabWnd::TAB_IMAGE_MARGIN;
  }
  else {
    rectTab.l += rectTab.Height();
    rectTab.r -= BaseTabWnd::TAB_IMAGE_MARGIN;
  }
  COLOR clrText = pTabWnd->GetTabTextColor(iTab);
  COLOR cltTextOld = (COLOR) - 1;
  if (!bIsActive && clrText != (COLOR) - 1) {
    cltTextOld = im->SetTextColor(clrText);
  }
  rectTab.r = min(rectTab.r, rectClip.r - 2);
  OnDrawTabContent(im, rectTab, iTab, bIsActive, pTabWnd, (COLOR) - 1);
  if (cltTextOld != (COLOR) - 1) {
    im->SetTextColor(cltTextOld);
  }
  im->SelectClipRgn(NULL);
}
int VisualManager2003_OnFillTab(img_t* im, IRECT rectFill, COLOR pbrFill,
    int iTab, BOOL bIsActive,
    const BaseTabWnd* pTabWnd)
{
  ASSERT_VALID(pTabWnd);
  if (pTabWnd->IsFlatTab() || s->m_nBitsPerPixel <= 8 ||
      IsHighContastMode(s) || pTabWnd->IsDialogControl()) {
    VisualManagerXP::OnFillTab(im, rectFill, pbrFill,
        iTab, bIsActive, pTabWnd);
    return 0;
  }
  COLOR clr1 = PixelAlpha2(s->m_clrBarGradientDark, 105);
  if (pTabWnd->GetTabBkColor(iTab) != (COLOR) - 1) {
    clr1 = pTabWnd->GetTabBkColor(iTab);
    if (clr1 == s->clrWindow && bIsActive) {
      imdraw_FillRect(im, 0, rectFill, &s->clrWindow);
      return 0;
    }
  }
  else {
    if (s->m_bAlwaysFillTab) {
      if (bIsActive) {
        imdraw_FillRect(im, 0, rectFill, &s->clrWindow);
        return 0;
      }
    }
    else {
      if (pTabWnd->IsVS2005Style() || pTabWnd->IsLeftRightRounded()) {
        if (bIsActive) {
          imdraw_FillRect(im, 0, rectFill, &s->clrWindow);
          return 0;
        }
      }
      else if (!bIsActive) {
        return 0;
      }
    }
  }
  COLOR clr2 = PixelAlpha2(clr1, 120);
  DrawManager dm(*im);
  if (pTabWnd->GetLocation() == TabWnd::LOCATION_TOP) {
    dm.FillGradient(rectFill, clr1, clr2, TRUE);
  }
  else {
    dm.FillGradient(rectFill, clr2, clr1, TRUE);
  }
}
BOOL VisualManager2003_OnEraseTabsFrame(img_t* im, IRECT rect, const BaseTabWnd* pTabWnd)
{
  ASSERT_VALID(pTabWnd);
  if (pTabWnd->IsFlatTab() || s->m_nBitsPerPixel <= 8 ||
      IsHighContastMode(s) || pTabWnd->IsDialogControl()) {
    return VisualManagerXP::OnEraseTabsFrame(im, rect, pTabWnd);
  }
  COLOR clrActiveTab = pTabWnd->GetTabBkColor(pTabWnd->GetActiveTab());
  if (clrActiveTab == (COLOR) - 1 &&
      (pTabWnd->IsOneNoteStyle() || pTabWnd->IsVS2005Style())) {
    imdraw_FillRect(im, 0, rect, &s->clrWindow);
    return TRUE;
  }
  DrawManager dm(*im);
  COLOR clr1 = s->m_clrBarGradientDark;
  if (clrActiveTab != (COLOR) - 1) {
    clr1 = clrActiveTab;
  }
  COLOR clr2 = PixelAlpha2(clr1, 130);
  if (pTabWnd->GetLocation() == TabWnd::LOCATION_BOTTOM) {
    COLOR clr = clr1;
    clr1 = clr2;
    clr2 = clr;
  }
  dm.FillGradient2(rect, clr1, clr2, 45);
  return TRUE;
}
int VisualManager2003_OnEraseTabsButton(img_t* im, IRECT rect,
    Button* pButton,
    BaseTabWnd* pBaseTab)
{
  ASSERT_VALID(pBaseTab);
  TabWnd* pWndTab = DYNAMIC_DOWNCAST(TabWnd, pBaseTab);
  if (pWndTab == NULL || pBaseTab->IsFlatTab() ||
      s->m_nBitsPerPixel <= 8 || IsHighContastMode(s) ||
      pBaseTab->IsDialogControl()) {
    if (pBaseTab->IsDialogControl() && pBaseTab->IsVisualManagerStyle()) {
      OnFillDialog(im, pBaseTab->GetParent(), rect);
    }
    else {
      VisualManagerXP::OnEraseTabsButton(im, rect, pButton, pBaseTab);
    }
    return 0;
  }
  if ((pBaseTab->IsOneNoteStyle() || pBaseTab->IsVS2005Style()) &&
      (pButton->IsPressed() || pButton->IsHighlighted())) {
    DrawManager dm(*im);
    if (pButton->IsPressed()) {
      dm.FillGradient(rect, s->m_clrHighlightDnGradientDark, s->m_clrHighlightDnGradientLight);
    }
    else {
      dm.FillGradient(rect, s->m_clrHighlightGradientDark, s->m_clrHighlightGradientLight);
    }
    return 0;
  }
  CRgn rgn;
  rgn.CreateRectRgnIndirect(rect);
  im->SelectClipRgn(&rgn);
  IRECT rectTabs;
  pWndTab->GetClientRect(&rectTabs);
  IRECT rectTabArea;
  pWndTab->GetTabsRect(rectTabArea);
  if (pWndTab->GetLocation() == BaseTabWnd::LOCATION_BOTTOM) {
    rectTabs.t = rectTabArea.t;
  }
  else {
    rectTabs.b = rectTabArea.b;
  }
  pWndTab->MapWindowPoints(pButton, rectTabs);
  OnEraseTabsArea(im, rectTabs, pWndTab);
  im->SelectClipRgn(NULL);
}
int VisualManager2003_OnDrawTabsButtonBorder(img_t* im, IRECT& rect,
    Button* pButton, UINT /*uiState*/,
    BaseTabWnd* /*pWndTab*/)
{
  if (pButton->IsPushed() || pButton->IsHighlighted()) {
    imdraw_Draw3dRect(im, 0, rect, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
  }
}
int VisualManager2003_ModifyGlobalColors()
{
  if (s->m_nBitsPerPixel <= 8 || !s->m_bIsStandardWinXPTheme || IsHighContastMode(s)) {
    //----------------------------------------------
    // Theme color may differ from the system color:
    //----------------------------------------------
    s->clrBarFace = GetThemeColor(s->m_hThemeButton, COLOR_3DFACE);
    s->clrBarShadow = GetThemeColor(s->m_hThemeButton, COLOR_3DSHADOW);
    s->clrBarHilite = GetThemeColor(s->m_hThemeButton, COLOR_3DHIGHLIGHT);
    s->clrBarDkShadow = GetThemeColor(s->m_hThemeButton, COLOR_3DDKSHADOW);
    s->clrBarLight = GetThemeColor(s->m_hThemeButton, COLOR_3DLIGHT);
  }
  else {
    COLOR clrBase = GetBaseThemeColor();
    if (s->m_WinXPTheme == WinXpTheme_Olive) {
      COLOR clrToolBarGradientDark = PixelAlpha2(
          clrBase, 120);
      COLOR clrToolBarGradientLight = DrawManager::SmartMixColors(
          clrBase,
          GetThemeColor(s->m_hThemeWindow, COLOR_WINDOW),
          1., 2, 1);
      s->clrBarFace = DrawManager::SmartMixColors(
          clrToolBarGradientDark,
          clrToolBarGradientLight, 1., 2, 1);
    }
    else if (s->m_WinXPTheme == WinXpTheme_Silver) {
      COLOR clrToolBarGradientDark = DrawManager::SmartMixColors(
          clrBase,
          GetThemeColor(s->m_hThemeWindow, COLOR_3DFACE),
          0.75, 2);
      COLOR clrToolBarGradientLight = DrawManager::SmartMixColors(
          clrBase,
          GetThemeColor(s->m_hThemeWindow, COLOR_WINDOW),
          1.03);
      s->clrBarFace = PixelAlpha2(DrawManager::SmartMixColors(
          clrToolBarGradientDark,
          clrToolBarGradientLight), 95);
    }
    else {
      s->clrBarFace = DrawManager::SmartMixColors(
          GetThemeColor(s->m_hThemeWindow, /*COLOR_HIGHLIGHT*/29),
          GetThemeColor(s->m_hThemeWindow, COLOR_WINDOW));
    }
    s->clrBarShadow = PixelAlpha2(
        s->clrBarFace, 70);
    s->clrBarHilite = PixelAlpha2(
        s->clrBarFace, 130);
    s->clrBarDkShadow = PixelAlpha2(
        s->clrBarFace, 50);
    s->clrBarLight = PixelAlpha2(
        s->clrBarFace, 110);
  }
  s->clrBarFace.DeleteObject();
  s->clrBarFace.CreateSolidBrush(s->clrBarFace);
}
int VisualManager2003_SetUseGlobalTheme(BOOL bUseGlobalTheme/* = TRUE*/)
{
  s->m_bUseGlobalTheme = bUseGlobalTheme;
  VisualManager::GetInstance()->OnUpdateSystemColors();
  VisualManager::GetInstance()->RedrawAll();
}
int VisualManager2003_SetStatusBarOfficeXPLook(BOOL bStatusBarOfficeXPLook/* = TRUE*/)
{
  s->m_bStatusBarOfficeXPLook = bStatusBarOfficeXPLook;
  VisualManager::GetInstance()->RedrawAll();
}
int VisualManager2003_SetDefaultWinXPColors(BOOL bDefaultWinXPColors/* = TRUE*/)
{
  s->m_bDefaultWinXPColors = bDefaultWinXPColors;
  VisualManager::GetInstance()->OnUpdateSystemColors();
  VisualManager::GetInstance()->RedrawAll();
}
int VisualManager2003_GetTabFrameColors(const BaseTabWnd* pTabWnd,
    COLOR& clrDark,
    COLOR& clrBlack,
    COLOR& clrHighlight,
    COLOR& clrFace,
    COLOR& clrDarkShadow,
    COLOR& clrLight,
    COLOR& pbrFace,
    COLOR& pbrBlack)
{
  ASSERT_VALID(pTabWnd);
  VisualManagerXP::GetTabFrameColors(pTabWnd,
      clrDark, clrBlack,
      clrHighlight, clrFace,
      clrDarkShadow, clrLight,
      pbrFace, pbrBlack);
  if (pTabWnd->IsOneNoteStyle() || s->m_nBitsPerPixel <= 8 ||
      IsHighContastMode(s) || pTabWnd->IsDialogControl() ||
      !s->m_bIsStandardWinXPTheme) {
    return 0;
  }
  COLOR clrActiveTab = pTabWnd->GetTabBkColor(pTabWnd->GetActiveTab());
  if (clrActiveTab == (COLOR) - 1) {
    clrFace = s->clrWindow;
  }
  clrDark = s->clrBarShadow;
  clrBlack = s->clrBarDkShadow;
  clrHighlight = pTabWnd->IsVS2005Style() ? s->clrBarShadow : s->clrBarLight;
  clrDarkShadow = s->clrBarShadow;
  clrLight = s->clrBarFace;
}
#ifndef BCGP_EXCLUDE_TASK_PANE
int VisualManager2003_OnFillTasksPaneBackground(img_t* im, IRECT rectWorkArea)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::OnFillTasksPaneBackground(im, rectWorkArea);
    return 0;
  }
  DrawManager dm(*im);
  dm.FillGradient(rectWorkArea, s->m_clrTaskPaneGradientDark, s->m_clrTaskPaneGradientLight, TRUE);
}
int VisualManager2003_OnDrawTasksGroupCaption(
    img_t* im, TasksGroup* pGroup,
    BOOL bIsHighlighted /*= FALSE*/, BOOL bIsSelected /*= FALSE*/,
    BOOL bCanCollapse /*= FALSE*/)
{
  ASSERT_VALID(pGroup);
  ASSERT_VALID(pGroup->s->m_pPage);
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::OnDrawTasksGroupCaption(
        im, pGroup,
        bIsHighlighted, bIsSelected, bCanCollapse);
    return 0;
  }
#ifndef BCGP_EXCLUDE_TOOLBOX
  BOOL bIsToolBox = pGroup->s->m_pPage->s->m_pTaskPane != NULL &&
      (pGroup->s->m_pPage->s->m_pTaskPane->IsKindOf(RUNTIME_CLASS(ToolBoxEx)));
#else
  BOOL bIsToolBox = FALSE;
#endif
  IRECT rectGroup = pGroup->s->m_rect;
  // -----------------------
  // Draw caption background
  // -----------------------
  if (bIsToolBox) {
    IRECT rectFill = rectGroup;
    rectFill.DeflateRect(1, 1);
    rectFill.b--;
    COLOR clrGrdaient1 = PixelAlpha2(
        s->m_clrToolBarGradientDark, 105);
    COLOR clrGrdaient2 = PixelAlpha2(
        s->m_clrToolBarGradientDark, 120);
    DrawManager dm(*im);
    dm.FillGradient(rectFill, clrGrdaient1, clrGrdaient2, TRUE);
    CBrush brFillBottom(PixelAlpha2(s->m_clrToolBarGradientDark, 120));
    IRECT rectFillBottom = rectGroup;
    rectFillBottom.DeflateRect(1, 0);
    rectFillBottom.t = rectFillBottom.b - 1;
    imdraw_FillRect(im, 0, rectFillBottom, &brFillBottom);
    if (bCanCollapse) {
      //--------------------
      // Draw expanding box:
      //--------------------
      int nBoxSize = 9;
      int nBoxOffset = 6;
      if (s->GetRibbonImageScale() != 1.) {
        nBoxSize = (int)(.5 + nBoxSize * s->GetRibbonImageScale());
      }
      IRECT rectButton = rectFill;
      rectButton.l += nBoxOffset;
      rectButton.r = rectButton.l + nBoxSize;
      rectButton.t = rectButton.CenterPoint().y - nBoxSize / 2;
      rectButton.b = rectButton.t + nBoxSize;
      OnDrawExpandingBox(im, rectButton, !pGroup->s->m_bIsCollapsed,
          s->clrBarText);
      rectGroup.l = rectButton.r + nBoxOffset;
      bCanCollapse = FALSE;
    }
  }
  else {
    IPOINT pts [7];
    const int nLeft = pGroup->s->m_rect.l;
    const int nTop = pGroup->s->m_rect.t;
    pts [0].x = nLeft;
    pts [0].y = pGroup->s->m_rect.b;
    pts [1].x = nLeft;
    pts [1].y = nTop + 4;
    pts [2].x = nLeft + 1;
    pts [2].y = nTop + 2;
    pts [3].x = nLeft + 2;
    pts [3].y = nTop + 1;
    pts [4].x = nLeft + 4;
    pts [4].y = nTop;
    pts [5].x = pGroup->s->m_rect.r;
    pts [5].y = nTop;
    pts [6].x = pGroup->s->m_rect.r;
    pts [6].y = pGroup->s->m_rect.b;
    CRgn rgn;
    rgn.CreatePolygonRgn(pts, 7, WINDING);
    im->SelectClipRgn(&rgn);
    DrawManager dm(*im);
    if (pGroup->s->m_bIsSpecial) {
      dm.FillGradient(pGroup->s->m_rect, s->m_clrTaskPaneGroupCaptionSpecDark,
          s->m_clrTaskPaneGroupCaptionSpecLight, FALSE);
    }
    else {
      dm.FillGradient(pGroup->s->m_rect, s->m_clrTaskPaneGroupCaptionLight,
          s->m_clrTaskPaneGroupCaptionDark, FALSE);
    }
    im->SelectClipRgn(NULL);
  }
  // ---------------------------
  // Draw an icon if it presents
  // ---------------------------
  BOOL bShowIcon = (pGroup->s->m_hIcon != NULL
      && pGroup->s->m_sizeIcon.w < rectGroup.Width() - rectGroup.Height());
  if (bShowIcon) {
    OnDrawTasksGroupIcon(im, pGroup, 5, bIsHighlighted, bIsSelected, bCanCollapse);
  }
  // -----------------------
  // Draw group caption text
  // -----------------------
  CFont* pFontOld = im->SelectObject(s->fontBold);
  COLOR clrTextOld = im->GetTextColor();
  if (bIsToolBox) {
    im->SetTextColor(s->clrBarText);
  }
  else {
    if (bCanCollapse && bIsHighlighted) {
      im->SetTextColor(pGroup->s->m_clrTextHot == (COLOR) - 1 ?
          (pGroup->s->m_bIsSpecial ? s->m_clrTaskPaneGroupBorder : s->m_clrTaskPaneHotText) :
            pGroup->s->m_clrTextHot);
    }
    else {
      im->SetTextColor(pGroup->s->m_clrText == (COLOR) - 1 ?
          (pGroup->s->m_bIsSpecial ? s->m_clrTaskPaneGroupBorder : s->m_clrTaskPaneHotText) :
            pGroup->s->m_clrText);
    }
  }
  int nBkModeOld = im->SetBkMode(TRANSPARENT);
  int nTaskPaneHOffset = pGroup->s->m_pPage->s->m_pTaskPane->GetGroupCaptionHorzOffset();
  int nTaskPaneVOffset = pGroup->s->m_pPage->s->m_pTaskPane->GetGroupCaptionVertOffset();
  int nCaptionHOffset = (nTaskPaneHOffset != -1 ? nTaskPaneHOffset : s->m_nGroupCaptionHorzOffset);
  IRECT rectText = rectGroup;
  rectText.l += (bShowIcon ? pGroup->s->m_sizeIcon.w + 5 : nCaptionHOffset);
  rectText.t += (nTaskPaneVOffset != -1 ? nTaskPaneVOffset : s->m_nGroupCaptionVertOffset);
  rectText.r = max(rectText.l,
      rectText.r - (bCanCollapse ? rectGroup.Height() : nCaptionHOffset));
  im->DrawText(pGroup->s->m_strName, rectText, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
  im->SetBkMode(nBkModeOld);
  im->SelectObject(pFontOld);
  im->SetTextColor(clrTextOld);
  // -------------------------
  // Draw group caption button
  // -------------------------
  if (bCanCollapse && !pGroup->s->m_strName.IsEmpty()) {
    CSize sizeButton = MenuImages::Size();
    IRECT rectButton = rectGroup;
    rectButton.l = max(rectButton.l,
        rectButton.r - (rectButton.Height() + 1) / 2 - (sizeButton.w + 1) / 2);
    rectButton.t = max(rectButton.t,
        rectButton.b - (rectButton.Height() + 1) / 2 - (sizeButton.h + 1) / 2);
    rectButton.r = rectButton.l + sizeButton.w;
    rectButton.b = rectButton.t + sizeButton.h;
    if (rectButton.r <= rectGroup.r && rectButton.b <= rectGroup.b) {
      if (bIsHighlighted) {
        // Draw button frame
        COLOR pBrushOld = (COLOR) im->SelectObject(&s->clrBarFace);
        COLOR clrBckOld = im->GetBkColor();
        imdraw_Draw3dRect(im, 0, &rectButton, s->clrWindow, s->clrBarShadow);
        im->SetBkColor(clrBckOld);
        im->SelectObject(pBrushOld);
      }
      if (!pGroup->s->m_bIsCollapsed) {
        MenuImages::Draw(im, MenuImages::IdArowUp, rectButton.TopLeft());
      }
      else {
        MenuImages::Draw(im, MenuImages::IdArowDown, rectButton.TopLeft());
      }
    }
  }
}
int VisualManager2003_OnFillTasksGroupInterior(
    img_t* im, IRECT rect, BOOL bSpecial /*= FALSE*/)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::OnFillTasksGroupInterior(im, rect, bSpecial);
    return 0;
  }
  DrawManager dm(*im);
  if (bSpecial) {
    dm.FillGradient(rect, s->m_clrTaskPaneGroupCaptionSpecDark,
        s->m_clrTaskPaneGroupCaptionSpecLight, TRUE);
  }
  else {
    dm.FillGradient(rect, s->m_clrTaskPaneGroupAreaDark,
        s->m_clrTaskPaneGroupAreaLight, TRUE);
  }
}
int VisualManager2003_OnDrawTasksGroupAreaBorder(
    img_t* im, IRECT rect, BOOL /*bSpecial = FALSE*/,
    BOOL /*bNoTitle = FALSE*/)
{
  // Draw underline
  CPen* pPenOld = (CPen*) im->SelectObject(&s->m_penTaskPaneGroupBorder);
  rect.r -= 1;
  rect.b -= 1;
  im->MoveTo(rect.l, rect.t);
  im->LineTo(rect.r, rect.t);
  im->LineTo(rect.r, rect.b);
  im->LineTo(rect.l, rect.b);
  im->LineTo(rect.l, rect.t);
  im->SelectObject(pPenOld);
}
int VisualManager2003_OnDrawTask(img_t* im, Task* pTask, CImageList* pIcons,
    BOOL bIsHighlighted /*= FALSE*/, BOOL bIsSelected /*= FALSE*/)
{
  ASSERT_VALID(pTask);
  if (pTask->s->m_bIsSeparator) {
    IRECT rectText = pTask->s->m_rect;
    CPen* pPenOld = (CPen*) im->SelectObject(&s->m_penSeparator);
    im->MoveTo(rectText.l, rectText.CenterPoint().y);
    im->LineTo(rectText.r, rectText.CenterPoint().y);
    im->SelectObject(pPenOld);
    return 0;
  }
  VisualManagerXP::OnDrawTask(im, pTask, pIcons, bIsHighlighted, bIsSelected);
}
int VisualManager2003_OnDrawScrollButtons(img_t* im, const IRECT& rect, const int nBorderSize,
    int iImage, BOOL bHilited)
{
  IRECT rectImage(CPoint(0, 0), MenuImages::Size());
  IRECT rectFill = rect;
  rectFill.t -= nBorderSize;
  imdraw_FillRect(im, 0, rectFill, &s->clrBarFace);
  if (bHilited) {
    CBrush br(s->m_nBitsPerPixel <= 8 || IsHighContastMode(s) ?
        s->clrWindow : s->m_clrHighlightMenuItem == (COLOR) - 1 ?
        s->m_clrHighlight : s->m_clrHighlightMenuItem);
    imdraw_FillRect(im, 0, rect, &br);
    imdraw_Draw3dRect(im, 0, rect, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
  }
  else {
    imdraw_Draw3dRect(im, 0, rect, s->clrBarShadow, s->clrBarShadow);
  }
  MenuImages::Draw(im, (MenuImages::IMAGES_IDS) iImage, rect);
}
#endif // BCGP_EXCLUDE_TASK_PANE
COLOR VisualManager2003_OnFillCommandsListBackground(img_t* im, IRECT rect, BOOL bIsSelected)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    return VisualManagerXP::OnFillCommandsListBackground(im, rect, bIsSelected);
  }
  ASSERT_VALID(this);
  COLOR clrText = s->clrBarText;
  int iImageWidth = ToolBar::GetMenuImageSize().w + GetMenuImageMargin();
  if (bIsSelected) {
    rect.l = 0;
    COLOR color = s->m_clrHighlightMenuItem == (COLOR) - 1 ?
        s->m_clrHighlight : s->m_clrHighlightMenuItem;
    CBrush br(color);
    imdraw_FillRect(im, 0, &rect, &br);
    imdraw_Draw3dRect(im, 0, rect, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
    // Now, we should define a menu text color...
    if (GetRV(color) > 128 &&
        GetGV(color) > 128 &&
        GetBV(color) > 128) {
      clrText = _RGB(0, 0, 0);
    }
    else {
      clrText = _RGB(255, 255, 255);
    }
  }
  else {
    imdraw_FillRect(im, 0, rect, s->m_clrMenuLight);
    IRECT rectImages = rect;
    rectImages.r = rectImages.l + iImageWidth + MENU_IMAGE_MARGIN;
    DrawManager dm(*im);
    dm.FillGradient(rectImages, s->m_clrToolBarGradientLight, s->m_clrToolBarGradientDark, FALSE);
    clrText = s->clrBarText;
  }
  return clrText;
}
int VisualManager2003_OnDrawStatusBarProgress(img_t* im, StatusBar* pStatusBar,
    IRECT rectProgress, int nProgressTotal, int nProgressCurr,
    COLOR clrBar, COLOR clrProgressBarDest, COLOR clrProgressText,
    BOOL bProgressText)
{
  if (!DrawStatusBarProgress(im, pStatusBar,
      rectProgress, nProgressTotal, nProgressCurr,
      clrBar, clrProgressBarDest, clrProgressText, bProgressText)) {
    VisualManagerXP::OnDrawStatusBarProgress(im, pStatusBar,
        rectProgress, nProgressTotal, nProgressCurr,
        clrBar, clrProgressBarDest, clrProgressText, bProgressText);
  }
}
int VisualManager2003_OnDrawStatusBarPaneBorder(img_t* im, StatusBar* pBar,
    IRECT rectPane, UINT uiID, UINT nStyle)
{
  if (!s->m_bStatusBarOfficeXPLook || s->m_hThemeStatusBar == NULL) {
    VisualManagerXP::OnDrawStatusBarPaneBorder(im, pBar,
        rectPane, uiID, nStyle);
  }
  if (s->m_hThemeStatusBar != NULL &&
      !(nStyle & SBPS_NOBORDERS)) {
    (*s->m_pfDrawThemeBackground)(s->m_hThemeStatusBar, im, 1 /*SP_PANE*/,
        0, &rectPane, 0);
  }
}
int VisualManager2003_OnFillHeaderCtrlBackground(HeaderCtrl* pCtrl,
    img_t* im, IRECT rect)
{
  VisualManagerXP::OnFillHeaderCtrlBackground(pCtrl, im, rect);
}
COLOR VisualManager2003_OnDrawControlBarCaption(img_t* im, DockingControlBar* pBar,
    BOOL bActive, IRECT rectCaption, IRECT rectButtons)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    return VisualManagerXP::OnDrawControlBarCaption(im, pBar,
        bActive, rectCaption, rectButtons);
  }
  DrawManager dm(*im);
  if (!bActive) {
    dm.FillGradient(rectCaption,
        s->m_clrToolBarGradientDark,
        s->m_clrToolBarGradientLight, TRUE);
  }
  else {
    dm.FillGradient(rectCaption,
        s->m_clrHighlightGradientDark,
        s->m_clrHighlightGradientLight,
        TRUE);
  }
  return s->clrBarText;
}
int VisualManager2003_OnFillAutoHideButtonBackground(img_t* im, IRECT rect, AutoHideButton* pButton)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::OnFillAutoHideButtonBackground(im, rect, pButton);
    return 0;
  }
  DrawManager dm(*im);
  if (pButton->IsActive()) {
    dm.FillGradient(rect,
        s->m_clrHighlightGradientLight, s->m_clrHighlightGradientDark,
        pButton->IsHorizontal());
  }
  else {
    dm.FillGradient(rect,
        s->m_clrBarGradientLight, s->m_clrBarGradientDark,
        pButton->IsHorizontal());
  }
}
int VisualManager2003_OnDrawAutoHideButtonBorder(img_t* im, IRECT rectBounds, IRECT rectBorderSize, AutoHideButton* pButton)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::OnDrawAutoHideButtonBorder(im, rectBounds, rectBorderSize, pButton);
    return 0;
  }
  COLOR clr = s->clrBarShadow;
  COLOR clrText = im->GetTextColor();
  if (rectBorderSize.l > 0) {
    im->FillSolidRect(rectBounds.l, rectBounds.t,
        rectBounds.l + rectBorderSize.l,
        rectBounds.b, clr);
  }
  if (rectBorderSize.t > 0) {
    im->FillSolidRect(rectBounds.l, rectBounds.t,
        rectBounds.r,
        rectBounds.t + rectBorderSize.t, clr);
  }
  if (rectBorderSize.r > 0) {
    im->FillSolidRect(rectBounds.r - rectBorderSize.r, rectBounds.t,
        rectBounds.r,
        rectBounds.b, clr);
  }
  if (rectBorderSize.b > 0) {
    im->FillSolidRect(rectBounds.l, rectBounds.b - rectBorderSize.b,
        rectBounds.r,
        rectBounds.b, clr);
  }
  im->SetTextColor(clrText);
}
int VisualManager2003_OnDrawOutlookBarSplitter(img_t* im, IRECT rectSplitter)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::OnDrawOutlookBarSplitter(im, rectSplitter);
    return 0;
  }
  DrawManager dm(*im);
  dm.FillGradient(rectSplitter,
      s->m_clrCaptionBarGradientDark,
      s->m_clrCaptionBarGradientLight,
      TRUE);
  const int nBoxesNumber = 10;
  const int nBoxSize = rectSplitter.Height() - 3;
  int x = rectSplitter.CenterPoint().x - nBoxSize * nBoxesNumber / 2;
  int y = rectSplitter.t + 2;
  for (int nBox = 0; nBox < nBoxesNumber; nBox++) {
    im->FillSolidRect(x + 1, y + 1, nBoxSize / 2, nBoxSize / 2,
        s->clrBtnHilite);
    im->FillSolidRect(x, y, nBoxSize / 2, nBoxSize / 2,
        s->m_clrGripper);
    x += nBoxSize;
  }
}
int VisualManager2003_OnFillOutlookBarCaption(img_t* im, IRECT rectCaption, COLOR& clrText)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::OnFillOutlookBarCaption(im, rectCaption, clrText);
    return 0;
  }
  DrawManager dm(*im);
  dm.FillGradient(rectCaption,
      s->m_clrCaptionBarGradientDark, s->m_clrCaptionBarGradientLight, TRUE);
  clrText = s->clrBarHilite;
}
#endif
BOOL VisualManager2003_OnDrawCalculatorButton(uidraw_t* s, img_t* im, IRECT rect, int state, const char* strText)
{
  IRECT rcTxt = rect;
  COLOR clrText = s->clrWindowText;
  switch (state) {
  case ButtonsIsPressed:
    s->OnFillHighlightedArea(im, rect, s->m_clrHighlightDn);
    imdraw_Draw3dRect(im, 0, &rect, s->m_clrToolBarGradientDark, s->m_clrToolBarGradientDark, 0);
    break;
  case ButtonsIsHighlighted:
    s->OnFillHighlightedArea(im, rect, s->m_clrHighlight);
    imdraw_Draw3dRect(im, 0, &rect, s->m_clrToolBarGradientDark, s->m_clrToolBarGradientDark, 0);
    break;
  default:
    if (1) {
      imdraw_FillGradient(im, 0, rect, s->clrBtnFace, s->clrBtnHilite, 0, 0, 0);
      imdraw_Draw3dRect(im, 0, &rect, s->clrBtnShadow, s->clrBtnShadow, 0);
    }
    else {
      imdraw_FillGradient(im, 0, rect, s->m_clrToolBarGradientDark, s->m_clrToolBarGradientLight, TRUE, 0, 0);
      imdraw_Draw3dRect(im, 0, &rect, s->m_clrToolBarGradientDark, s->m_clrToolBarGradientDark, 0);
    }
    break;
  }
  imdraw_text(im, &rect, rcTxt, strText, -1, s->fontDefault, clrText, 0, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
  return TRUE;
}
BOOL VisualManager2003_OnDrawCalculatorDisplay(uidraw_t* s, img_t* im, IRECT rect, const char* strText)
{
  imdraw_Draw3dRect(im, 0, &rect, s->m_clrToolBarGradientDark, s->m_clrToolBarGradientDark, s->clrWindow);
  return TRUE;
}
#if 0
BOOL VisualManager2003_OnDrawBrowseButton(img_t* im, IRECT rect,
    Edit* /*pEdit*/, VisualManager::BCGBUTTON_STATE state, COLOR& /*clrText*/)
{
  IRECT rectFrame = rect;
  rectFrame.InflateRect(0, 1, 1, 1);
  switch (state) {
  case ButtonsIsPressed:
    OnFillHighlightedArea(im, rect, s->m_clrHighlightDn, NULL);
    imdraw_Draw3dRect(im, 0, &rectFrame, s->m_clrToolBarGradientDark, s->m_clrToolBarGradientDark);
    break;
  case ButtonsIsHighlighted:
    OnFillHighlightedArea(im, rect, s->m_clrHighlight, NULL);
    imdraw_Draw3dRect(im, 0, &rectFrame, s->m_clrToolBarGradientDark, s->m_clrToolBarGradientDark);
    break;
  default: {
    DrawManager dm(*im);
    dm.FillGradient(rect, s->clrBtnFace, s->clrBtnHilite);
    imdraw_Draw3dRect(im, 0, rect, s->clrBarHilite, s->clrBarHilite);
  }
  break;
  }
  return TRUE;
}
COLOR VisualManager2003_GetWindowColor() const
{
  return GetThemeColor(s->m_hThemeWindow, COLOR_WINDOW);
}
int VisualManager2003_OnHighlightRarelyUsedMenuItems(img_t* im, IRECT rectRarelyUsed)
{
  rectRarelyUsed.l --;
  rectRarelyUsed.r = rectRarelyUsed.l + ToolBar::GetMenuImageSize().w +
      2 * GetMenuImageMargin() + 2;
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::OnHighlightRarelyUsedMenuItems(im, rectRarelyUsed);
    return 0;
  }
  DrawManager dm(*im);
  dm.FillGradient(rectRarelyUsed, s->m_clrMenuRarelyUsed, s->m_clrToolBarGradientDark, FALSE);
}
int VisualManager2003_OnDrawControlBorder(img_t* im, IRECT rect, CWnd* pWndCtrl, BOOL bDrawOnGlass)
{
  if (s->m_hThemeComboBox == NULL) {
    VisualManagerXP::OnDrawControlBorder(im, rect, pWndCtrl, bDrawOnGlass);
    return 0;
  }
  COLOR clrBorder = (COLOR) - 1;
  if ((*s->m_pfGetThemeColor)(s->m_hThemeComboBox, CP_READONLY, 0, TMT_BORDERCOLOR, &clrBorder) != S_OK) {
    VisualManagerXP::OnDrawControlBorder(im, rect, pWndCtrl, bDrawOnGlass);
    return 0;
  }
  if (bDrawOnGlass) {
    DrawManager dm(*im);
    dm.DrawRect(rect, (COLOR) - 1, clrBorder);
    rect.DeflateRect(1, 1);
    dm.DrawRect(rect, (COLOR) - 1, s->clrWindow);
  }
  else {
    imdraw_Draw3dRect(im, 0, &rect, clrBorder, clrBorder);
    rect.DeflateRect(1, 1);
    imdraw_Draw3dRect(im, 0, rect, s->clrWindow, s->clrWindow);
  }
}
int VisualManager2003_OnDrawExpandingBox(img_t* im, IRECT rect, BOOL bIsOpened, COLOR colorBox)
{
  if (s->m_hThemeTree == NULL) {
    VisualManagerXP::OnDrawExpandingBox(im, rect, bIsOpened, colorBox);
    return 0;
  }
  (*s->m_pfDrawThemeBackground)(s->m_hThemeTree, im, TVP_GLYPH,
      bIsOpened ? GLPS_OPENED : GLPS_CLOSED, &rect, 0);
}
int VisualManager2003_GetSmartDockingBaseMarkerColors(
    COLOR& clrBaseGroupBackground,
    COLOR& clrBaseGroupBorder)
{
  if (s->m_nBitsPerPixel > 8 && !IsHighContastMode(s)) {
    clrBaseGroupBackground = _RGB(228, 228, 228);
    clrBaseGroupBorder = _RGB(181, 181, 181);
  }
  else {
    clrBaseGroupBackground = s->clrBarFace;
    clrBaseGroupBorder = s->clrBarShadow;
  }
}
COLOR VisualManager2003_GetSmartDockingMarkerToneColor()
{
  if (s->m_nBitsPerPixel > 8 && !IsHighContastMode(s)) {
    WinXpTheme theme = GetStandardWinXPTheme();
    switch (theme) {
    case WinXpTheme_Blue:
      return _RGB(61, 123, 241);
    case WinXpTheme_Olive:
      return _RGB(190, 146, 109);
    case WinXpTheme_Silver:
      return _RGB(134, 130, 169);
    }
  }
  return VisualManagerXP::GetSmartDockingMarkerToneColor();
}
int VisualManager2003_OnDrawStatusBarSizeBox(img_t* im, StatusBar* pStatBar,
    IRECT rectSizeBox)
{
  if (s->m_hThemeScrollBar == NULL) {
    VisualManagerXP::OnDrawStatusBarSizeBox(im, pStatBar, rectSizeBox);
    return 0;
  }
  (*s->m_pfDrawThemeBackground)(s->m_hThemeScrollBar, im, SBP_SIZEBOX,
      SZB_RIGHTALIGN, &rectSizeBox, 0);
}
COLOR VisualManager2003_GetBaseThemeColor()
{
  return s->m_bIsStandardWinXPTheme && s->m_hThemeWindow != NULL ?
      GetThemeColor(s->m_hThemeWindow, 29) :
      s->clrBarFace;
}
#ifndef BCGP_EXCLUDE_TOOLBOX
BOOL VisualManager2003_OnEraseToolBoxButton(img_t* im, IRECT rect,
    ToolBoxButton* pButton)
{
  if (pButton->IsHighlighted() || pButton->GetCheck()) {
    OnFillHighlightedArea(im, rect,
        pButton->GetCheck() ? s->m_clrHighlightChecked : s->m_clrHighlight, NULL);
  }
  return TRUE;
}
BOOL VisualManager2003_OnDrawToolBoxButtonBorder(img_t* im, IRECT& rect,
    ToolBoxButton* pButton, UINT /*uiState*/)
{
  if (pButton->IsHighlighted() || pButton->GetCheck()) {
    imdraw_Draw3dRect(im, 0, &rect, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
  }
  return TRUE;
}
#endif // BCGP_EXCLUDE_TOOLBOX
int VisualManager2003_OnHighlightQuickCustomizeMenuButton(img_t* im,
    ToolbarMenuButton* /*pButton*/, IRECT rect)
{
  if (IsHighContastMode(s)) {
    imdraw_FillRect(im, 0, rect, s->m_clrBarBkgnd);
  }
  else {
    CBrush br(s->m_clrToolBarGradientLight);
    imdraw_FillRect(im, 0, rect, &br);
  }
  imdraw_Draw3dRect(im, 0, rect, s->m_clrMenuBorder, s->m_clrMenuBorder);
}
int VisualManager2003_OnDrawHeaderCtrlBorder(HeaderCtrl* pCtrl, img_t* im,
    IRECT& rect, BOOL bIsPressed, BOOL bIsHighlighted)
{
  if (s->m_hThemeHeader == NULL) {
    VisualManagerXP::OnDrawHeaderCtrlBorder(pCtrl, im, rect, bIsPressed, bIsHighlighted);
    return 0;
  }
  int nState = HIS_NORMAL;
  if (bIsPressed) {
    nState = HIS_PRESSED;
  }
  else if (bIsHighlighted) {
    nState = HIS_HOT;
  }
  (*s->m_pfDrawThemeBackground)(s->m_hThemeHeader, im,
      HP_HEADERITEM, nState, &rect, 0);
}
#ifndef BCGP_EXCLUDE_POPUP_WINDOW
int VisualManager2003_OnFillPopupWindowBackground(img_t* im, IRECT rect)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::OnFillPopupWindowBackground(im, rect);
    return 0;
  }
  DrawManager dm(*im);
  dm.FillGradient(rect, s->m_clrBarGradientDark, s->m_clrBarGradientLight);
}
int VisualManager2003_OnDrawPopupWindowBorder(img_t* im, IRECT rect)
{
  imdraw_Draw3dRect(im, 0, rect, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
}
COLOR VisualManager2003_OnDrawPopupWindowCaption(img_t* im, IRECT rectCaption, PopupWindow* pPopupWnd)
{
  if (s->m_nBitsPerPixel <= 8 ||
      IsHighContastMode(s)) {
    return VisualManagerXP::OnDrawPopupWindowCaption(im, rectCaption, pPopupWnd);
  }
  DrawManager dm(*im);
  dm.FillGradient(rectCaption,
      s->m_clrCaptionBarGradientDark, s->m_clrCaptionBarGradientLight, TRUE);
  if (pPopupWnd->HasSmallCaption()) {
    IRECT rectGripper = rectCaption;
    int xCenter = rectGripper.CenterPoint().x;
    int yCenter = rectGripper.CenterPoint().y;
    rectGripper.l = xCenter - 20;
    rectGripper.r = xCenter + 20;
    rectGripper.t = yCenter - 4;
    rectGripper.b = yCenter + 2;
    OnDrawBarGripper(im, rectGripper, FALSE, NULL);
  }
  // get the text color
  return s->clrBarHilite;
}
int VisualManager2003_OnErasePopupWindowButton(img_t* im, IRECT rc, PopupWndButton* pButton)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::OnErasePopupWindowButton(im, rc, pButton);
    return 0;
  }
  if (pButton->IsPressed()) {
    COLOR color = s->m_clrHighlightDnGradientLight == (COLOR) - 1 ?
        s->m_clrHighlightDn : s->m_clrHighlightDnGradientLight;
    CBrush br(color);
    imdraw_FillRect(im, 0, &rc, &br);
    return 0;
  }
  else if (pButton->IsHighlighted() || pButton->IsPushed()) {
    COLOR color = s->m_clrHighlightMenuItem == (COLOR) - 1 ?
        s->m_clrHighlight : s->m_clrHighlightMenuItem;
    CBrush br(color);
    imdraw_FillRect(im, 0, &rc, &br);
    return 0;
  }
  IRECT rectParent;
  pButton->GetParent()->GetClientRect(rectParent);
  pButton->GetParent()->MapWindowPoints(pButton, rectParent);
  OnFillPopupWindowBackground(im, rectParent);
}
int VisualManager2003_OnDrawPopupWindowButtonBorder(img_t* im, IRECT rc, PopupWndButton* pButton)
{
  if (pButton->IsHighlighted() || pButton->IsPushed() ||
      pButton->IsCaptionButton()) {
    imdraw_Draw3dRect(im, 0, rc, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
  }
}
#endif // BCGP_EXCLUDE_POPUP_WINDOW
#ifndef BCGP_EXCLUDE_PLANNER
COLOR VisualManager2003_OnFillPlannerCaption(img_t* im, PlannerView* pView,
    IRECT rect, BOOL bIsToday, BOOL bIsSelected, BOOL bNoBorder/* = FALSE*/, BOOL bHorz /*= TRUE*/)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s) ||
      !bIsToday) {
    return VisualManagerXP::OnFillPlannerCaption(im, pView,
        rect, bIsToday, bIsSelected, bNoBorder, bHorz);
  }
  DrawManager dm(*im);
  rect.DeflateRect(1, 1);
  dm.FillGradient(rect,
      s->m_clrPlannerTodayFill, s->clrBtnFace, bHorz);
  if (bIsToday) {
    CPen* pOldPen = im->SelectObject(&s->m_penPlannerTodayLine);
    im->MoveTo(rect.l, rect.b);
    im->LineTo(rect.r, rect.b);
    im->SelectObject(pOldPen);
  }
  return s->clrBtnText;
}
COLOR VisualManager2003_GetPlannerViewBackgroundColor(PlannerView* pView)
{
  ASSERT_VALID(pView);
  COLOR colorFill = pView->GetPlanner()->GetBackgroundColor();
  if (colorFill == CLR_DEFAULT) {
    colorFill = s->m_clrPlannerWork; // Use default color
  }
  return colorFill;
}
COLOR VisualManager2003_GetPlannerHourLineColor(PlannerView* pView,
    BOOL bWorkingHours, BOOL bHour)
{
  ASSERT_VALID(pView);
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    return VisualManagerXP::GetPlannerHourLineColor(pView,
        bWorkingHours, bHour);
  }
  COLOR colorFill = CLR_DEFAULT;
  if (bWorkingHours) {
    colorFill = GetPlannerViewNonWorkingColor(pView);
  }
  else {
    colorFill = GetPlannerViewWorkingColor(pView);
  }
  if (colorFill == CLR_DEFAULT) {
    colorFill = GetPlannerViewBackgroundColor(pView);
  }
  return CalculateHourLineColor2003(colorFill, bWorkingHours, bHour);
}
COLOR VisualManager2003_GetPlannerNonWorkingColor(COLOR clrWorking)
{
  return CalculateNonWorkingColor2003(clrWorking);
}
COLOR VisualManager2003_GetPlannerViewWorkingColor(PlannerView* pView)
{
  return CalculateWorkingColor2003(GetPlannerViewBackgroundColor(pView));
}
COLOR VisualManager2003_GetPlannerViewNonWorkingColor(PlannerView* pView)
{
  return GetPlannerNonWorkingColor(GetPlannerViewWorkingColor(pView));
}
int VisualManager2003_OnDrawPlannerTimeLine(img_t* im, PlannerView* pView, IRECT rect)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::OnDrawPlannerTimeLine(im, pView, rect);
    return 0;
  }
  DrawManager dm(*im);
  dm.FillGradient(rect, s->m_clrPlannerTodayFill, s->clrBtnFace, TRUE);
  CPen* pOldPen = im->SelectObject(&s->m_penPlannerTodayLine);
  im->MoveTo(rect.l, rect.b);
  im->LineTo(rect.r, rect.b);
  im->SelectObject(pOldPen);
}
int VisualManager2003_OnFillPlanner(img_t* im, PlannerView* pView,
    IRECT rect, BOOL bWorkingArea)
{
  ASSERT_VALID(pView);
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::OnFillPlanner(im, pView, rect, bWorkingArea);
    return 0;
  }
  COLOR colorFill = CLR_DEFAULT;
  if (bWorkingArea) {
    colorFill = GetPlannerViewWorkingColor(pView);
  }
  else {
    colorFill = GetPlannerViewNonWorkingColor(pView);
  }
  if (colorFill == CLR_DEFAULT) {
    colorFill = GetPlannerViewBackgroundColor(pView);
  }
  CBrush br(colorFill);
  imdraw_FillRect(im, 0, rect, &br);
}
#endif // BCGP_EXCLUDE_PLANNER
#ifndef BCGP_EXCLUDE_GRID_CTRL
COLOR VisualManager2003_GetReportCtrlGroupBackgoundColor()
{
  return s->m_clrHighlightMenuItem;
}
int VisualManager2003_OnDrawGridSelectionBorder(GridCtrl* /*pCtrl*/, img_t* im, IRECT rect)
{
  imdraw_Draw3dRect(im, 0, rect, s->m_clrHighlightGradientDark, s->m_clrHighlightGradientDark);
  rect.DeflateRect(1, 1);
  imdraw_Draw3dRect(im, 0, rect, s->m_clrHighlightGradientDark, s->m_clrHighlightGradientDark);
}
int VisualManager2003_OnDrawGridExpandingBox(img_t* im, IRECT rect, BOOL bIsOpened, COLOR colorBox)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::OnDrawGridExpandingBox(im, rect, bIsOpened, colorBox);
    return 0;
  }
  if ((rect.Width() % 2) != 0) {
    rect.r++;
  }
  if ((rect.Height() % 2) != 0) {
    rect.b++;
  }
  rect.DeflateRect(1, 1, 0, 0);
  DrawManager dm(*im);
  dm.FillGradient2(rect, _RGB(255, 255, 255), s->m_clrToolBarGradientDark, 45);
  rect.InflateRect(1, 1, 0, 0);
  CPen* pOldPen = im->SelectObject(&s->m_penGridExpandBoxLight);
  ASSERT_VALID(pOldPen);
  im->MoveTo(rect.l + 1, rect.t);
  im->LineTo(rect.r, rect.t);
  im->MoveTo(rect.l, rect.t + 1);
  im->LineTo(rect.l, rect.b);
  im->SelectObject(&s->m_penGridExpandBoxDark);
  im->MoveTo(rect.l + 1, rect.b);
  im->LineTo(rect.r, rect.b);
  im->MoveTo(rect.r, rect.t + 1);
  im->LineTo(rect.r, rect.b);
  const int dx = rect.Width() / 2 - 3;
  const int dy = rect.Height() / 2 - 3;
  CPoint ptCenter = rect.CenterPoint();
  im->MoveTo(ptCenter.x - dx, ptCenter.y);
  im->LineTo(ptCenter.x + dx + 1, ptCenter.y);
  if (!bIsOpened) {
    im->MoveTo(ptCenter.x, ptCenter.y - dy);
    im->LineTo(ptCenter.x, ptCenter.y + dy + 1);
  }
  im->SelectObject(pOldPen);
}
int VisualManager2003_OnFillGridHeaderBackground(GridCtrl* pCtrl, img_t* im, IRECT rect)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s) ||
      s->m_hThemeHeader == NULL) {
    VisualManagerXP::OnFillGridHeaderBackground(pCtrl, im, rect);
    return 0;
  }
  imdraw_FillRect(im, 0, rect, &s->clrBtnFace);
}
BOOL VisualManager2003_OnDrawGridHeaderItemBorder(GridCtrl* pCtrl, img_t* im, IRECT rect, BOOL bPressed)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s) ||
      s->m_hThemeHeader == NULL) {
    return VisualManagerXP::OnDrawGridHeaderItemBorder(pCtrl, im, rect, bPressed);
  }
  (*s->m_pfDrawThemeBackground)(s->m_hThemeHeader, im,
      HP_HEADERITEM, HIS_NORMAL, &rect, 0);
  return TRUE;
}
int VisualManager2003_OnFillGridRowHeaderBackground(GridCtrl* /*pCtrl*/, img_t* im, IRECT rect)
{
  imdraw_FillRect(im, 0, rect, &s->clrBtnFace);
}
BOOL VisualManager2003_OnDrawGridRowHeaderItemBorder(GridCtrl* /*pCtrl*/, img_t* im, IRECT rect, BOOL /*bPressed*/)
{
  CPen* pOldPen = im->SelectObject(&s->penBarShadow);
  im->MoveTo(rect.l, rect.t);
  im->LineTo(rect.l, rect.b);
  im->MoveTo(rect.r - 1, rect.t);
  im->LineTo(rect.r - 1, rect.b);
  im->MoveTo(rect.l, rect.t);
  im->LineTo(rect.r - 1, rect.t);
  im->SelectObject(pOldPen);
  return TRUE;
}
int VisualManager2003_OnFillGridSelectAllAreaBackground(GridCtrl* pCtrl, img_t* im, IRECT rect, BOOL /*bPressed*/)
{
  OnFillGridHeaderBackground(pCtrl, im, rect);
}
BOOL VisualManager2003_OnDrawGridSelectAllAreaBorder(GridCtrl* pCtrl, img_t* im, IRECT rect, BOOL bPressed)
{
  return OnDrawGridHeaderItemBorder(pCtrl, im, rect, bPressed);
}
BOOL VisualManager2003_OnSetGridColorTheme(GridCtrl* pCtrl, BCGP_GRID_COLOR_DATA& theme)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s) ||
      s->m_hThemeHeader == NULL) {
    return VisualManagerXP::OnSetGridColorTheme(pCtrl, theme);
  }
  theme.m_clrHorzLine = s->m_clrToolBarGradientDark;
  theme.m_clrVertLine = s->m_clrToolBarBottomLine;
  theme.m_EvenColors.m_clrBackground = s->m_clrToolBarGradientLight;
  theme.m_EvenColors.m_clrText = s->clrBarText;
  theme.m_OddColors.m_clrBackground = s->m_clrToolBarGradientDark;
  theme.m_OddColors.m_clrText = s->clrBarText;
  theme.m_SelColors.s->m_clrBackground = s->m_clrHighlightMenuItem;
  if (GetRV(s->m_clrHighlightMenuItem) > 128 &&
      GetGV(s->m_clrHighlightMenuItem) > 128 &&
      GetBV(s->m_clrHighlightMenuItem) > 128) {
    theme.m_SelColors.s->m_clrText = _RGB(0, 0, 0);
  }
  else {
    theme.m_SelColors.s->m_clrText = _RGB(255, 255, 255);
  }
  theme.m_GroupColors.s->m_clrBackground = s->m_clrToolBarGradientDark;
  theme.m_GroupColors.s->m_clrGradient = s->m_clrToolBarGradientLight;
  theme.m_GroupColors.s->m_clrText = s->clrBarText;
  theme.m_GroupSelColors.s->m_clrBackground = s->m_clrHighlightGradientDark;
  theme.m_GroupSelColors.s->m_clrGradient = s->m_clrHighlightGradientLight;
  theme.m_GroupSelColors.s->m_clrText = s->clrBarText;
  theme.m_LeftOffsetColors.s->m_clrBackground = s->m_clrToolBarGradientDark;
  theme.m_LeftOffsetColors.s->m_clrBorder = theme.m_clrHorzLine;
  return TRUE;
}
#endif // BCGP_EXCLUDE_GRID_CTRL
#if !defined(BCGP_EXCLUDE_GRID_CTRL) && !defined(BCGP_EXCLUDE_GANTT)
int VisualManager2003_GetGanttColors(const GanttChart* pChart, BCGP_GANTT_CHART_COLORS& colors, COLOR clrBack) const
{
  VisualManagerXP::GetGanttColors(pChart, colors, clrBack);
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    return 0;
  }
  colors.clrGridLine0 = CalculateHourLineColor2003(colors.clrRowDayOff, TRUE, TRUE);
  colors.clrGridLine1 = CalculateHourLineColor2003(colors.clrRowDayOff, TRUE, FALSE);
  colors.clrSelection = s->clrHilite;
  colors.clrSelectionBorder = s->clrHilite;
  colors.clrConnectorLines = s->clrWindowFrame;
}
int VisualManager2003_DrawGanttHeaderCell(const GanttChart* pChart, CDC& dc, const BCGP_GANTT_CHART_HEADER_CELL_INFO& cellInfo, BOOL bHilite)
{
  if (s->m_hThemeHeader == NULL) {
    VisualManagerXP::DrawGanttHeaderCell(pChart, dc, cellInfo, bHilite);
    return 0;
  }
  (*s->m_pfDrawThemeBackground)(s->m_hThemeHeader, dc.GetSafeHdc(), HP_HEADERITEM, HIS_NORMAL, cellInfo.rectCell, 0);
}
int VisualManager2003_FillGanttBar(const GanttItem* pItem, CDC& dc, const IRECT& rectFill, COLOR color, double dGlowLine)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::FillGanttBar(pItem, dc, rectFill, color, dGlowLine);
    return 0;
  }
  dGlowLine = min(1.0, dGlowLine);
  dGlowLine = max(0.0, dGlowLine);
  IRECT rectPart = rectFill;
  int h = rectFill.Height();
  rectPart.b = (LONG)(rectFill.t + dGlowLine * h);
  DrawManager dm(dc);
  dm.FillGradient(rectPart, DrawManager::MixColors(color, _RGB(255, 255, 255), 0.3f), DrawManager::MixColors(color, _RGB(255, 255, 255), 0.15f), TRUE);
  rectPart.t = rectPart.b;
  rectPart.b = rectFill.b;
  dm.FillGradient(rectPart, DrawManager::MixColors(color, _RGB(0, 0, 0), 0.2f), color, TRUE);
}
#endif // !defined(BCGP_EXCLUDE_GRID_CTRL) && !defined(BCGP_EXCLUDE_GANTT)
int VisualManager2003_OnDrawCheckBoxEx(CDC* im, IRECT rect,
    int nState,
    BOOL bHighlighted,
    BOOL bPressed,
    BOOL bEnabled)
{
  if (!DrawCheckBox(im, rect, bHighlighted, nState, bEnabled, bPressed)) {
    VisualManagerXP::OnDrawCheckBoxEx(im, rect, nState, bHighlighted, bPressed, bEnabled);
  }
}
int VisualManager2003_OnDrawRadioButton(CDC* im, IRECT rect,
    BOOL bOn,
    BOOL bHighlighted,
    BOOL bPressed,
    BOOL bEnabled)
{
  if (!DrawRadioButton(im, rect, bHighlighted, bOn, bEnabled, bPressed)) {
    VisualManagerXP::OnDrawRadioButton(im, rect, bOn, bHighlighted, bPressed, bEnabled);
  }
}
int VisualManager2003_GetCalendarColors(const Calendar* pCalendar,
    CalendarColors& colors)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::GetCalendarColors(pCalendar, colors);
    return 0;
  }
  colors.clrCaption = s->m_clrBarGradientDark;
  colors.clrCaptionText = s->clrBarText;
  if (s->m_bIsStandardWinXPTheme) {
    colors.clrSelected = _RGB(251, 230, 148);
  }
  else {
    if (s->m_clrHighlightMenuItem != (COLOR) - 1) {
      colors.clrSelected = s->m_clrHighlightMenuItem;
    }
    else {
      colors.clrSelected = s->m_clrHighlight;
    }
  }
  colors.clrSelectedText = s->clrBarText;
  colors.clrTodayBorder = _RGB(187, 85, 3);
}
#ifndef BCGP_EXCLUDE_PROP_LIST
COLOR VisualManager2003_GetPropListGroupColor(PropList* pPropList)
{
  return VisualManager::GetPropListGroupColor(pPropList);
}
COLOR VisualManager2003_GetPropListGroupTextColor(PropList* pPropList)
{
  return VisualManager::GetPropListGroupTextColor(pPropList);
}
#endif
#ifndef BCGP_EXCLUDE_RIBBON
COLOR VisualManager2003_OnDrawRibbonCategoryTab(
    img_t* im,
    RibbonTab* pTab,
    BOOL bIsActive)
{
  if (s->m_nBitsPerPixel <= 8 ||
      IsHighContastMode(s)) {
    return VisualManagerXP::OnDrawRibbonCategoryTab(
        im, pTab, bIsActive);
  }
  ASSERT_VALID(pTab);
  RibbonCategory* pCategory = pTab->GetParentCategory();
  ASSERT_VALID(pCategory);
  RibbonBar* pBar = pCategory->GetParentRibbonBar();
  ASSERT_VALID(pBar);
  bIsActive = bIsActive &&
      ((pBar->GetHideFlags() & BCGPRIBBONBAR_HIDE_ELEMENTS) == 0 || pTab->GetDroppedDown() != NULL);
  const BOOL bIsFocused = pTab->IsFocused() && (pBar->GetHideFlags() & BCGPRIBBONBAR_HIDE_ELEMENTS);
  const BOOL bIsHighlighted = (pTab->IsHighlighted() || bIsFocused) && !pTab->IsDroppedDown();
  IRECT rectTab = pTab->GetRect();
  rectTab.t += 3;
  const int nTrancateRatio = pBar->GetTabTrancateRatio();
  if (nTrancateRatio > 0) {
    IRECT rectRight = rectTab;
    rectRight.l = rectRight.r - 1;
    const int nPercent = max(10, 100 - nTrancateRatio / 2);
    COLOR color1 = PixelAlpha2(
        s->clrBarShadow, nPercent);
    COLOR color2 = PixelAlpha2(
        color1, 120);
    DrawManager dm(*im);
    dm.FillGradient(rectRight, color1, color2, TRUE);
  }
  if (!bIsActive && !bIsHighlighted) {
    return s->clrBarText;
  }
  rectTab.r -= 2;
  CPen pen(PS_SOLID, 1, s->clrBarShadow);
  CPen* pOldPen = im->SelectObject(&pen);
  ASSERT(pOldPen != NULL);
#define RIBBONTAB_POINTS_NUM 8
  IPOINT pts [RIBBONTAB_POINTS_NUM];
  pts [0] = CPoint(rectTab.l, rectTab.b);
  pts [1] = CPoint(rectTab.l + 1, rectTab.b - 1);
  pts [2] = CPoint(rectTab.l + 1, rectTab.t + 2);
  pts [3] = CPoint(rectTab.l + 3, rectTab.t);
  pts [4] = CPoint(rectTab.r - 3, rectTab.t);
  pts [5] = CPoint(rectTab.r - 1, rectTab.t + 2);
  pts [6] = CPoint(rectTab.r - 1, rectTab.b - 1);
  pts [7] = CPoint(rectTab.r, rectTab.b);
  CRgn rgnClip;
  rgnClip.CreatePolygonRgn(pts, RIBBONTAB_POINTS_NUM, WINDING);
  im->SelectClipRgn(&rgnClip);
  DrawManager dm(*im);
  const BOOL bIsSelected = pTab->IsSelected();
  COLOR clrFill = bIsSelected ? s->m_clrHighlightGradientDark : RibbonCategoryColorToRGB(pCategory->GetTabColor());
  COLOR clr1 = s->clrBarFace;
  COLOR clr2 = (clrFill == (COLOR) - 1) ?
      PixelAlpha2(clr1, 120) : clrFill;
  if (bIsHighlighted) {
    if (bIsActive) {
      clr2 = s->m_clrHighlightGradientLight;
    }
    else {
      if (clrFill == (COLOR) - 1) {
        clr1 = s->m_clrHighlightGradientDark;
        clr2 = s->m_clrHighlightGradientLight;
      }
      else {
        clr1 = clrFill;
        clr2 = PixelAlpha2(clr1, 120);
      }
    }
  }
  dm.FillGradient(rectTab, clr1, clr2, TRUE);
  im->SelectClipRgn(NULL);
  im->Polyline(pts, RIBBONTAB_POINTS_NUM);
  if (bIsHighlighted && bIsActive && !bIsSelected) {
    //---------------------
    // Draw internal frame:
    //---------------------
    const CPoint ptCenter = rectTab.CenterPoint();
    for (int i = 0; i < RIBBONTAB_POINTS_NUM; i++) {
      if (pts [i].x < ptCenter.x) {
        pts [i].x++;
      }
      else {
        pts [i].x--;
      }
      if (pts [i].y < ptCenter.y) {
        pts [i].y++;
      }
      else {
        pts [i].y--;
      }
    }
    CPen penInternal(PS_SOLID, 1, s->m_clrHighlightGradientDark);
    im->SelectObject(&penInternal);
    im->Polyline(pts, RIBBONTAB_POINTS_NUM);
    im->SelectObject(pOldPen);
  }
  im->SelectObject(pOldPen);
  return s->clrBarText;
}
COLOR VisualManager2003_OnDrawRibbonButtonsGroup(
    img_t* im, RibbonButtonsGroup* pGroup,
    IRECT rect)
{
  ASSERT_VALID(pGroup);
  if (DYNAMIC_DOWNCAST(RibbonQuickAccessToolbar, pGroup) != NULL ||
      pGroup->GetCount() == 0) {
    return(COLOR) - 1;
  }
  BaseRibbonElement* pButton = pGroup->GetButton(0);
  if (!pButton->IsShowGroupBorder()) {
    return(COLOR) - 1;
  }
  const int dx = 2;
  const int dy = 2;
  CPen pen(PS_SOLID, 1, s->m_clrToolBarGradientDark);
  CPen* pOldPen = im->SelectObject(&pen);
  ASSERT(pOldPen != NULL);
  COLOR pOldBrush = (COLOR) im->SelectStockObject(NULL_BRUSH);
  ASSERT(pOldBrush != NULL);
  rect.DeflateRect(1, 1);
  im->RoundRect(rect, CPoint(dx, dy));
  im->SelectObject(pOldPen);
  im->SelectObject(pOldBrush);
  return(COLOR) - 1;
}
COLOR VisualManager2003_OnDrawRibbonCategoryCaption(
    img_t* im,
    RibbonContextCaption* pContextCaption)
{
  ASSERT_VALID(pContextCaption);
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    return VisualManagerXP::OnDrawRibbonCategoryCaption(
        im, pContextCaption);
  }
  COLOR clrFill = RibbonCategoryColorToRGB(pContextCaption->GetColor());
  IRECT rect = pContextCaption->GetRect();
  if (clrFill != (COLOR) - 1) {
    if (ToolBarImages::s->m_bIsDrawOnGlass) {
      rect.DeflateRect(0, 1);
    }
    DrawManager dm(*im);
    dm.FillGradient(rect, clrFill, s->clrBarFace, TRUE);
  }
  return s->clrBarText;
}
int VisualManager2003_OnDrawRibbonSliderZoomButton(
    img_t* im, RibbonSlider* pSlider,
    IRECT rect, BOOL bIsZoomOut,
    BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerXP::OnDrawRibbonSliderZoomButton(
        im, pSlider, rect, bIsZoomOut, bIsHighlighted, bIsPressed, bIsDisabled);
    return 0;
  }
  COLOR clrLine =
      (bIsPressed || bIsHighlighted) ? s->clrBarDkShadow : s->clrBtnDkShadow;
  CPoint ptCenter = rect.CenterPoint();
  IRECT rectCircle(CPoint(ptCenter.x - 7, ptCenter.y - 7), CSize(15, 15));
  DrawManager dm(*im);
  COLOR clrFill = (COLOR) - 1;
  if (bIsPressed || bIsHighlighted) {
    clrFill = bIsPressed ? s->m_clrHighlightDnGradientLight : s->m_clrHighlightDnGradientDark;
  }
  dm.DrawEllipse(rectCircle, clrFill, clrLine);
  // Draw +/- sign:
  IRECT rectSign(CPoint(ptCenter.x - 3, ptCenter.y - 3), CSize(7, 7));
  if (ToolBarImages::s->m_bIsDrawOnGlass) {
    dm.DrawLine(rectSign.l, ptCenter.y, rectSign.r, ptCenter.y, clrLine);
    if (!bIsZoomOut) {
      dm.DrawLine(ptCenter.x, rectSign.t, ptCenter.x, rectSign.b, clrLine);
    }
  }
  else {
    CPen penLine(PS_SOLID, 1, clrLine);
    CPen* pOldPen = im->SelectObject(&penLine);
    im->MoveTo(rectSign.l, ptCenter.y);
    im->LineTo(rectSign.r, ptCenter.y);
    if (!bIsZoomOut) {
      im->MoveTo(ptCenter.x, rectSign.t);
      im->LineTo(ptCenter.x, rectSign.b);
    }
    im->SelectObject(pOldPen);
  }
}
COLOR VisualManager2003_OnDrawRibbonStatusBarPane(img_t* im, RibbonStatusBar* pBar,
    RibbonStatusBarPane* pPane)
{
  ASSERT_VALID(pPane);
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s) ||
      s->m_hThemeStatusBar == NULL) {
    return VisualManagerXP::OnDrawRibbonStatusBarPane(
        im, pBar, pPane);
  }
  IRECT rect = pPane->GetRect();
  if (pPane->IsHighlighted()) {
    IRECT rectButton = rect;
    rectButton.DeflateRect(1, 1);
    OnFillHighlightedArea(im, rectButton,
        pPane->IsPressed() ?
        s->m_clrHighlightDn : s->m_clrHighlight, NULL);
    imdraw_Draw3dRect(im, 0, rectButton, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
  }
  return(COLOR) - 1;
}
int VisualManager2003_OnDrawRibbonProgressBar(img_t* im,
    RibbonProgressBar* pProgress,
    IRECT rectProgress, IRECT rectChunk,
    BOOL bInfiniteMode)
{
#define PP_BAR 1
#define PP_CHUNK 3
  if (s->m_hThemeProgress != NULL) {
    (*s->m_pfDrawThemeBackground)(s->m_hThemeProgress, im,
        PP_BAR, 0, &rectProgress, 0);
    if (!rectChunk.IsRectEmpty()) {
      rectChunk.DeflateRect(2, 2);
      (*s->m_pfDrawThemeBackground)(s->m_hThemeProgress, im,
          PP_CHUNK, 0, &rectChunk, 0);
    }
  }
  else {
    VisualManagerXP::OnDrawRibbonProgressBar(im, pProgress,
        rectProgress, rectChunk, bInfiniteMode);
  }
}
int VisualManager2003_OnDrawRibbonQATSeparator(img_t* im,
    RibbonSeparator* /*pSeparator*/, IRECT rect)
{
  int x = rect.CenterPoint().x;
  if (ToolBarImages::s->m_bIsDrawOnGlass) {
    DrawManager dm(*im);
    dm.DrawLine(x, rect.t, x, rect.b - 1, s->clrBarDkShadow);
    dm.DrawLine(x + 1, rect.t + 1, x + 1, rect.b, s->clrBarLight);
  }
  else {
    CPen* pOldPen = im->SelectObject(&s->m_penSeparator);
    ASSERT(pOldPen != NULL);
    im->MoveTo(x, rect.t);
    im->LineTo(x, rect.b - 1);
    im->SelectObject(&s->m_penSeparatorLight);
    im->MoveTo(x + 1, rect.t + 1);
    im->LineTo(x + 1, rect.b);
    im->SelectObject(pOldPen);
  }
}
COLOR VisualManager2003_OnFillRibbonPanelCaption(img_t* im, RibbonPanel* pPanel, IRECT rectCaption)
{
  ASSERT_VALID(pPanel);
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    return VisualManagerXP::OnFillRibbonPanelCaption(im, pPanel, rectCaption);
  }
  CBrush br(!pPanel->IsHighlighted() ?
      s->m_clrBarGradientDark :
      PixelAlpha2(s->m_clrBarGradientDark, 108));
  imdraw_FillRect(im, 0, rectCaption, &br);
  return s->clrBarText;
}
#endif // BCGP_EXCLUDE_RIBBON
BOOL VisualManager2003_GetToolTipParams(ToolTipParams& params,
    UINT /*nType*/ /*=(UINT)(-1)*/)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    return VisualManagerXP::GetToolTipParams(params);
  }
  params.s->m_bBoldLabel = TRUE;
  params.s->m_bDrawDescription = TRUE;
  params.s->m_bDrawIcon = TRUE;
  params.s->m_bRoundedCorners = TRUE;
  params.s->m_bDrawSeparator = FALSE;
  params.s->m_clrFill = s->clrBarHilite;
  params.s->m_clrFillGradient = s->clrBarFace;
  params.s->m_clrText = s->clrBarText;
  params.s->m_clrBorder = s->clrBarShadow;
  return TRUE;
}
#undef POINTS_NUM
#endif
int VisualManager2003_init(uidraw_t* s, sys_t* sys)
{
  VisualManagerXP_init(s, sys);
#define INITFUN(name) s->name = VisualManager2003_##name
  INITFUN(OnDrawCalculatorButton);
  INITFUN(OnDrawCalculatorDisplay);
#undef INITFUN
  s->m_bShadowHighlightedImage = FALSE;
  s->m_bFadeInactiveImage = FALSE;
  s->m_nMenuShadowDepth = 3;
  s->m_nVertMargin = 8;
  s->m_nHorzMargin = 8;
  s->m_nGroupVertOffset = 8;
  s->m_nGroupCaptionHeight = 18;
  s->m_nGroupCaptionHorzOffset = 3;
  s->m_nGroupCaptionVertOffset = 3;
  s->m_nTasksHorzOffset = 8;
  s->m_nTasksIconHorzOffset = 5;
  s->m_nTasksIconVertOffset = 4;
  s->m_bActiveCaptions = TRUE;
  s->m_bUseGlobalTheme = TRUE;
  s->m_bStatusBarOfficeXPLook = TRUE;
  s->m_bDefaultWinXPColors = TRUE;
  s->OnUpdateSystemColors(s);
  return 0;
}
#if 0
//
// BCGPVisualManagerVS2005.cpp: implementation of the VisualManagerVS2005 class.
//
//////////////////////////////////////////////////////////////////////
//#include "multimon.h"
//#include "BCGCBPro.h"
#include "bcgglobals.h"
#include "BCGPDrawManager.h"
#include "BCGPCaptionButton.h"
#include "BCGPTabWnd.h"
#include "BCGPVisualManagerVS2005.h"
#include "BCGPAutoHideButton.h"
#include "BCGPToolBar.h"
#include "BCGPToolbarMenuButton.h"
#include "BCGPStatusBar.h"
#include "BCGPDockManager.h"
#include "BCGPTabbedControlBar.h"
#include "BCGPPropList.h"
BOOL VisualManagerVS2005::s->m_bRoundedAutohideButtons = FALSE;
IMPLEMENT_DYNCREATE(VisualManagerVS2005, VisualManager2003)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
VisualManagerVS2005::VisualManagerVS2005()
{
  s->m_bAlwaysFillTab = TRUE;
  s->m_b3DTabWideBorder = FALSE;
  s->m_bShdowDroppedDownMenuButton = TRUE;
  s->m_bDrawLastTabLine = FALSE;
  s->m_colorActiveTabBorder = (COLOR) - 1;
  s->m_bFrameMenuCheckedItems = TRUE;
  DockManager::EnableDockBarMenu();
  DockManager::SetDockMode(BCGP_DT_SMART);
  AutoHideButton::s->m_bOverlappingTabs = FALSE;
}
VisualManagerVS2005::~VisualManagerVS2005()
{
}
int VisualManagerVS2005::OnUpdateSystemColors()
{
  BOOL bDefaultWinXPColors = s->m_bDefaultWinXPColors;
  s->m_clrPressedButtonBorder = (COLOR) - 1;
  s->m_CurrAppTheme = GetStandardWinXPTheme();
  if (s->m_CurrAppTheme != WinXpTheme_Silver) {
    s->m_bDefaultWinXPColors = FALSE;
  }
  VisualManager2003_OnUpdateSystemColors();
  if (!bDefaultWinXPColors) {
    return 0;
  }
  COLOR clrMenuButtonDroppedDown = s->m_clrBarBkgnd;
  COLOR clrMenuItemCheckedHighlight = s->m_clrHighlightDn;
  if (s->m_hThemeComboBox == NULL ||
      s->m_pfGetThemeColor == NULL ||
      (*s->m_pfGetThemeColor)(s->m_hThemeComboBox, 5, 0, 3801, &s->m_colorActiveTabBorder) != S_OK) {
    s->m_colorActiveTabBorder = (COLOR) - 1;
  }
  if (s->m_nBitsPerPixel > 8 && !IsHighContastMode(s)) {
    s->m_clrCustomizeButtonGradientLight = DrawManager::SmartMixColors(
        s->m_clrCustomizeButtonGradientDark,
        s->clrBarFace, 1.5, 1, 1);
    if (s->m_CurrAppTheme == WinXpTheme_Blue ||
        s->m_CurrAppTheme == WinXpTheme_Olive) {
      s->m_clrToolBarGradientDark = PixelAlpha2(
          s->m_clrToolBarGradientDark, 83);
      s->m_clrToolBarGradientLight = DrawManager::SmartMixColors(
          GetBaseThemeColor(),
          GetThemeColor(s->m_hThemeWindow, COLOR_WINDOW),
          1., 3, 2);
    }
    else if (!s->m_bIsStandardWinXPTheme) {
      s->m_clrToolBarGradientLight = DrawManager::SmartMixColors(
          s->m_clrToolBarGradientLight,
          s->clrBarHilite,
          1.05, 1, 1);
    }
    if (s->m_CurrAppTheme == WinXpTheme_Blue) {
      s->m_clrCustomizeButtonGradientDark = PixelAlpha2(
          s->m_clrCustomizeButtonGradientDark, 90);
      s->m_clrCustomizeButtonGradientLight = PixelAlpha2(
          s->m_clrCustomizeButtonGradientLight, 115);
      s->m_clrToolBarBottomLine = PixelAlpha2(
          s->m_clrToolBarBottomLine, 85);
    }
    else if (s->m_CurrAppTheme == WinXpTheme_Olive) {
      s->m_clrToolBarBottomLine = PixelAlpha2(
          s->m_clrToolBarBottomLine, 110);
      s->m_clrCustomizeButtonGradientDark = s->m_clrToolBarBottomLine;
      s->m_clrCustomizeButtonGradientLight = PixelAlpha2(
          s->m_clrCustomizeButtonGradientLight, 120);
      s->m_clrHighlightDn = s->clrHilite;
      s->m_clrHighlight = PixelAlpha2(
          s->m_clrHighlightDn, 124);
      s->m_clrHighlightChecked = PixelAlpha2(
          GetThemeColor(s->m_hThemeWindow, 27 /*COLOR_GRADIENTACTIVECAPTION*/), 98);
      s->m_clrHighlight.DeleteObject();
      s->m_clrHighlightDn.DeleteObject();
      s->m_clrHighlight.CreateSolidBrush(s->m_clrHighlight);
      s->m_clrHighlightDn.CreateSolidBrush(s->m_clrHighlightDn);
      s->m_clrHighlightChecked.DeleteObject();
      s->m_clrHighlightChecked.CreateSolidBrush(s->m_clrHighlightChecked);
      s->m_clrHighlightGradientDark = s->m_clrHighlightChecked;
      s->m_clrHighlightGradientLight = PixelAlpha2(
          s->m_clrHighlightGradientDark, 120);
    }
    else if (s->m_CurrAppTheme != WinXpTheme_Silver) {
      s->m_clrToolBarBottomLine = s->m_clrToolBarGradientDark;
    }
    clrMenuButtonDroppedDown = PixelAlpha2(
        s->m_clrBarBkgnd, 107);
    clrMenuItemCheckedHighlight = GetThemeColor(s->m_hThemeWindow, COLOR_HIGHLIGHT);
    if (s->m_CurrAppTheme == WinXpTheme_Blue ||
        s->m_CurrAppTheme == WinXpTheme_Olive) {
      s->m_clrBarGradientLight = PixelAlpha2(
          s->m_clrToolBarGradientLight, 95);
      s->m_clrBarGradientDark = PixelAlpha2(
          s->m_clrBarGradientDark, 97);
    }
    s->m_clrToolbarDisabled = DrawManager::SmartMixColors(
        s->m_clrToolBarGradientDark, s->m_clrToolBarGradientLight,
        .92, 1, 2);
    s->m_clrPressedButtonBorder = DrawManager::SmartMixColors(
        s->m_clrMenuItemBorder,
        s->clrBarDkShadow,
        .8, 1, 2);
  }
  s->m_clrMenuButtonDroppedDown.DeleteObject();
  s->m_clrMenuButtonDroppedDown.CreateSolidBrush(clrMenuButtonDroppedDown);
  s->m_clrMenuItemCheckedHighlight.DeleteObject();
  s->m_clrMenuItemCheckedHighlight.CreateSolidBrush(clrMenuItemCheckedHighlight);
  s->m_penActiveTabBorder.DeleteObject();
  if (s->m_colorActiveTabBorder != (COLOR) - 1) {
    s->m_penActiveTabBorder.CreatePen(PS_SOLID, 1, s->m_colorActiveTabBorder);
  }
  s->m_bDefaultWinXPColors = bDefaultWinXPColors;
  s->m_clrInactiveTabText = s->clrBtnDkShadow;
  if (s->m_nBitsPerPixel > 8 && !IsHighContastMode(s)) {
    s->m_penSeparator.DeleteObject();
    COLOR clrSeparator = PixelAlpha2(
        s->clrBarFace, 84);
    s->m_penSeparator.CreatePen(PS_SOLID, 1, clrSeparator);
  }
}
COLOR VisualManagerVS2005::OnDrawControlBarCaption(img_t* im, DockingControlBar* pBar,
    BOOL bActive, IRECT rectCaption, IRECT rectButtons)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    return VisualManagerXP::OnDrawControlBarCaption(im, pBar,
        bActive, rectCaption, rectButtons);
  }
  rectCaption.b++;
  COLOR clrFill;
  if (!bActive) {
    if (s->m_CurrAppTheme == WinXpTheme_Blue ||
        s->m_CurrAppTheme == WinXpTheme_Olive ||
        s->m_CurrAppTheme == WinXpTheme_Silver) {
      clrFill = PixelAlpha2(s->m_clrBarGradientDark, 87);
      CBrush brFill(clrFill);
      imdraw_FillRect(im, 0, rectCaption, &brFill);
      imdraw_Draw3dRect(im, 0, rectCaption, s->clrBarShadow, s->clrBarShadow);
    }
    else {
      CBrush brFill(s->clrInactiveCaption);
      imdraw_FillRect(im, 0, rectCaption, &brFill);
      return s->clrInactiveCaptionText;
    }
  }
  else {
    if (s->m_CurrAppTheme == WinXpTheme_Blue ||
        s->m_CurrAppTheme == WinXpTheme_Olive ||
        s->m_CurrAppTheme == WinXpTheme_Silver) {
      COLOR clrLight =
          PixelAlpha2(s->clrHilite, 130);
      DrawManager dm(*im);
      dm.FillGradient(rectCaption, s->clrHilite, clrLight, TRUE);
      return s->clrTextHilite;
    }
    else {
      imdraw_FillRect(im, 0, rectCaption, &s->clrActiveCaption);
      return s->clrCaptionText;
    }
  }
  if (GetRV(clrFill) <= 192 &&
      GetGV(clrFill) <= 192 &&
      GetBV(clrFill) <= 192) {
    return _RGB(255, 255, 255);
  }
  else {
    return _RGB(0, 0, 0);
  }
}
int VisualManagerVS2005::OnDrawCaptionButton(img_t* im, CaptionButton* pButton,
    BOOL bActive, BOOL bHorz, BOOL bMaximized, BOOL bDisabled,
    int nImageID /*= -1*/)
{
  if (bActive || pButton->IsMiniFrameButton()) {
    VisualManagerXP::OnDrawCaptionButton(im, pButton, bActive, bHorz, bMaximized, bDisabled, nImageID);
    return 0;
  }
  IRECT rc = pButton->GetRect();
  const BOOL bHighlight =
      (pButton->s->m_bPushed || pButton->s->m_bFocused || pButton->s->m_bDroppedDown) && !bDisabled;
  if (bHighlight) {
    imdraw_FillRect(im, 0, rc, &s->clrBarFace);
  }
  MenuImages::IMAGES_IDS id = (MenuImages::IMAGES_IDS) - 1;
  if (nImageID != -1) {
    id = (MenuImages::IMAGES_IDS)nImageID;
  }
  else {
    id = pButton->GetIconID(bHorz, bMaximized);
  }
  if (id != (MenuImages::IMAGES_IDS) - 1) {
    CSize sizeImage = MenuImages::Size();
    CPoint ptImage(rc.l + (rc.Width() - sizeImage.w) / 2,
        rc.t + (rc.Height() - sizeImage.h) / 2);
    OnDrawCaptionButtonIcon(im, pButton, id, bActive, bDisabled, ptImage);
  }
  if (bHighlight) {
    imdraw_Draw3dRect(im, 0, rc, s->clrBarDkShadow, s->clrBarDkShadow);
  }
}
int VisualManagerVS2005::OnEraseTabsArea(img_t* im, IRECT rect,
    const BaseTabWnd* pTabWnd)
{
  ASSERT_VALID(pTabWnd);
  if (pTabWnd->IsFlatTab() || s->m_nBitsPerPixel <= 8 ||
      IsHighContastMode(s)) {
    VisualManagerXP::OnEraseTabsArea(im, rect, pTabWnd);
    return 0;
  }
  if (pTabWnd->IsOneNoteStyle() || pTabWnd->IsVS2005Style()) {
    if (pTabWnd->IsDialogControl()) {
      if (pTabWnd->IsVisualManagerStyle()) {
        OnFillDialog(im, pTabWnd->GetParent(), rect);
      }
      else {
        imdraw_FillRect(im, 0, rect, &s->clrBtnFace);
      }
    }
    else {
      imdraw_FillRect(im, 0, rect, &s->clrBarFace);
    }
  }
  else {
    BaseControlBar* pParentBar = DYNAMIC_DOWNCAST(BaseControlBar,
        pTabWnd->GetParent());
    if (pParentBar == NULL) {
      imdraw_FillRect(im, 0, rect, &s->clrBtnFace);
    }
    else {
      IRECT rectScreen = s->m_rectVirtual;
      pTabWnd->ScreenToClient(&rectScreen);
      IRECT rectFill = rect;
      rectFill.l = min(rectFill.l, rectScreen.l);
      OnFillBarBackground(im, pParentBar, rectFill, rect);
    }
  }
}
int VisualManagerVS2005::OnDrawTab(img_t* im, IRECT rectTab,
    int iTab, BOOL bIsActive, const BaseTabWnd* pTabWnd)
{
  ASSERT_VALID(pTabWnd);
  if (pTabWnd->IsFlatTab() || pTabWnd->IsOneNoteStyle() ||
      pTabWnd->IsVS2005Style()) {
    CPen* pOldPen = NULL;
    if (bIsActive && pTabWnd->IsVS2005Style() &&
        s->m_penActiveTabBorder.IsValid()) {
      pOldPen = im->SelectObject(&s->m_penActiveTabBorder);
    }
    VisualManager2003_OnDrawTab(im, rectTab, iTab, bIsActive, pTabWnd);
    if (pOldPen != NULL) {
      im->SelectObject(pOldPen);
    }
    return 0;
  }
  COLOR clrTab = pTabWnd->GetTabBkColor(iTab);
  COLOR clrTextOld = (COLOR) - 1;
  if (bIsActive && clrTab == (COLOR) - 1) {
    clrTextOld = im->SetTextColor(s->clrWindowText);
    ((BaseTabWnd*)pTabWnd)->SetTabBkColor(iTab, s->clrWindow);
  }
  VisualManagerXP::OnDrawTab(im, rectTab, iTab, bIsActive, pTabWnd);
  ((BaseTabWnd*)pTabWnd)->SetTabBkColor(iTab, clrTab);
  if (clrTextOld != (COLOR) - 1) {
    im->SetTextColor(clrTextOld);
  }
}
int VisualManagerVS2005::CreateAutoHideButtonRegion(IRECT rect,
    DWORD dwAlignment, LPPOINT& points)
{
  switch (dwAlignment & CBRS_ALIGN_ANY) {
  case CBRS_ALIGN_LEFT:
    rect.r--;
    break;
  case CBRS_ALIGN_TOP:
    rect.b--;
    break;
  }
  IRECT rectOrign = rect;
  DWORD dwAlignmentOrign = dwAlignment;
  if ((dwAlignment & CBRS_ALIGN_ANY) == CBRS_ALIGN_LEFT ||
      (dwAlignment & CBRS_ALIGN_ANY) == CBRS_ALIGN_RIGHT) {
    rect = IRECT(0, 0, rectOrign.Height(), rectOrign.Width());
    dwAlignment = (dwAlignment == CBRS_ALIGN_LEFT) ? CBRS_ALIGN_TOP : CBRS_ALIGN_BOTTOM;
  }
  CList<IPOINT, IPOINT> pts;
  if (!s->m_bRoundedAutohideButtons) {
    rect.r--;
    pts.AddHead(CPoint(rect.l, rect.t));
    pts.AddHead(CPoint(rect.l, rect.b - 2));
    pts.AddHead(CPoint(rect.l + 2, rect.b));
    pts.AddHead(CPoint(rect.r - 2, rect.b));
    pts.AddHead(CPoint(rect.r, rect.b - 2));
    pts.AddHead(CPoint(rect.r, rect.t));
  }
  else {
    POSITION posLeft = pts.AddHead(CPoint(rect.l, rect.t));
    posLeft = pts.InsertAfter(posLeft, CPoint(rect.l, rect.t + 2));
    POSITION posRight = pts.AddTail(CPoint(rect.r, rect.t));
    posRight = pts.InsertBefore(posRight, CPoint(rect.r, rect.t + 2));
    int xLeft = rect.l + 1;
    int xRight = rect.r - 1;
    int y = 0;
    BOOL bIsHorz =
        (dwAlignmentOrign & CBRS_ALIGN_ANY) == CBRS_ALIGN_LEFT ||
        (dwAlignmentOrign & CBRS_ALIGN_ANY) == CBRS_ALIGN_RIGHT;
    for (y = rect.t + 2; y < rect.b - 4; y += 2) {
      posLeft = pts.InsertAfter(posLeft, CPoint(xLeft, y));
      posLeft = pts.InsertAfter(posLeft, CPoint(xLeft, y + 2));
      posRight = pts.InsertBefore(posRight, CPoint(xRight, y));
      posRight = pts.InsertBefore(posRight, CPoint(xRight, y + 2));
      xLeft++;
      xRight--;
    }
    if ((dwAlignmentOrign & CBRS_ALIGN_ANY) == CBRS_ALIGN_BOTTOM && !bIsHorz) {
      xLeft--;
      xRight++;
    }
    if (bIsHorz) {
      xRight++;
    }
    for (; y < rect.b - 1; y++) {
      posLeft = pts.InsertAfter(posLeft, CPoint(xLeft, y));
      posLeft = pts.InsertAfter(posLeft, CPoint(xLeft + 1, y + 1));
      posRight = pts.InsertBefore(posRight, CPoint(xRight, y));
      posRight = pts.InsertBefore(posRight, CPoint(xRight - 1, y + 1));
      if (y == rect.b - 2) {
        posLeft = pts.InsertAfter(posLeft, CPoint(xLeft + 1, y + 1));
        posLeft = pts.InsertAfter(posLeft, CPoint(xLeft + 3, y + 1));
        posRight = pts.InsertBefore(posRight, CPoint(xRight, y + 1));
        posRight = pts.InsertBefore(posRight, CPoint(xRight - 2, y + 1));
      }
      xLeft++;
      xRight--;
    }
    posLeft = pts.InsertAfter(posLeft, CPoint(xLeft + 2, rect.b));
    posRight = pts.InsertBefore(posRight, CPoint(xRight - 2, rect.b));
  }
  points = new IPOINT [pts.GetCount()];
  int i = 0;
  for (POSITION pos = pts.GetHeadPosition(); pos != NULL; i++) {
    points [i] = pts.GetNext(pos);
    switch (dwAlignmentOrign & CBRS_ALIGN_ANY) {
    case CBRS_ALIGN_BOTTOM:
      points [i].y = rect.b - (points [i].y - rect.t);
      break;
    case CBRS_ALIGN_RIGHT: {
      int x = rectOrign.r - points [i].y;
      int y = rectOrign.t + points [i].x;
      points [i] = CPoint(x, y);
    }
    break;
    case CBRS_ALIGN_LEFT: {
      int x = rectOrign.l + points [i].y;
      int y = rectOrign.t + points [i].x;
      points [i] = CPoint(x, y);
    }
    break;
    }
  }
  return(int) pts.GetCount();
}
int VisualManagerVS2005::OnFillAutoHideButtonBackground(img_t* im, IRECT rect, AutoHideButton* pButton)
{
  if (!s->m_bRoundedAutohideButtons) {
    return 0;
  }
  LPPOINT points;
  int nPoints = CreateAutoHideButtonRegion(rect, pButton->GetAlignment(), points);
  CRgn rgnClip;
  rgnClip.CreatePolygonRgn(points, nPoints, WINDING);
  im->SelectClipRgn(&rgnClip);
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManager2003_OnFillAutoHideButtonBackground(im, rect, pButton);
  }
  else {
    BOOL bIsHorz =
        ((pButton->GetAlignment() & CBRS_ALIGN_ANY) == CBRS_ALIGN_LEFT ||
            (pButton->GetAlignment() & CBRS_ALIGN_ANY) == CBRS_ALIGN_RIGHT);
    DrawManager dm(*im);
    dm.FillGradient(rect,
        s->m_clrBarGradientDark, s->m_clrBarGradientLight, !bIsHorz);
  }
  im->SelectClipRgn(NULL);
  delete [] points;
}
int VisualManagerVS2005::OnDrawAutoHideButtonBorder(img_t* im, IRECT rect, IRECT /*rectBorderSize*/, AutoHideButton* pButton)
{
  CPen pen(PS_SOLID, 1, s->clrBarShadow);
  CPen* pOldPen = im->SelectObject(&pen);
  ASSERT(pOldPen != NULL);
  LPPOINT points;
  int nPoints = CreateAutoHideButtonRegion(rect, pButton->GetAlignment(), points);
  if (!s->m_bRoundedAutohideButtons) {
    im->Polyline(points, nPoints);
  }
  else {
    BOOL bIsHorz
    ((pButton->GetAlignment() & CBRS_ALIGN_ANY) == CBRS_ALIGN_LEFT ||
        (pButton->GetAlignment() & CBRS_ALIGN_ANY) == CBRS_ALIGN_RIGHT);
    for (int i = 0; i < nPoints; i++) {
      if ((i % 2) != 0) {
        int x1 = points [i - 1].x;
        int y1 = points [i - 1].y;
        int x2 = points [i].x;
        int y2 = points [i].y;
        if (bIsHorz) {
          if (y1 > rect.CenterPoint().y && y2 > rect.CenterPoint().y) {
            y1--;
            y2--;
          }
        }
        else {
          if (x1 > rect.CenterPoint().x && x2 > rect.CenterPoint().x) {
            x1--;
            x2--;
          }
        }
        if (y2 >= y1) {
          im->MoveTo(x1, y1);
          im->LineTo(x2, y2);
        }
        else {
          im->MoveTo(x2, y2);
          im->LineTo(x1, y1);
        }
      }
    }
  }
  im->SelectObject(pOldPen);
  delete [] points;
}
int VisualManagerVS2005::GetTabFrameColors(const BaseTabWnd* pTabWnd,
    COLOR& clrDark,
    COLOR& clrBlack,
    COLOR& clrHighlight,
    COLOR& clrFace,
    COLOR& clrDarkShadow,
    COLOR& clrLight,
    COLOR& pbrFace,
    COLOR& pbrBlack)
{
  ASSERT_VALID(pTabWnd);
  VisualManager2003_GetTabFrameColors(pTabWnd,
      clrDark, clrBlack,
      clrHighlight, clrFace,
      clrDarkShadow, clrLight,
      pbrFace, pbrBlack);
  if (pTabWnd->IsVS2005Style() && s->m_colorActiveTabBorder != (COLOR) - 1) {
    clrHighlight = s->m_colorActiveTabBorder;
  }
  clrBlack = clrDarkShadow;
}
int VisualManagerVS2005::OnDrawSeparator(img_t* im, BaseControlBar* pBar,
    IRECT rect, BOOL bHorz)
{
  ToolBar* pToolBar = DYNAMIC_DOWNCAST(ToolBar, pBar);
  if (pToolBar != NULL) {
    ASSERT_VALID(pToolBar);
    if (bHorz) {
      const int nDelta = max(0, (pToolBar->GetButtonSize().h - pToolBar->GetImageSize().h) / 2);
      rect.t += nDelta;
    }
    else {
      const int nDelta = max(0, (pToolBar->GetButtonSize().w - pToolBar->GetImageSize().w) / 2);
      rect.l += nDelta;
    }
  }
  VisualManagerXP::OnDrawSeparator(im, pBar, rect, bHorz);
}
int VisualManagerVS2005::OnFillHighlightedArea(img_t* im, IRECT rect,
    COLOR pBrush, ToolbarButton* pButton)
{
  if (pButton != NULL &&
      (s->m_CurrAppTheme == WinXpTheme_Blue || s->m_CurrAppTheme == WinXpTheme_Olive)) {
    ToolbarMenuButton* pMenuButton =
        DYNAMIC_DOWNCAST(ToolbarMenuButton, pButton);
    BOOL bIsPopupMenu = pMenuButton != NULL &&
        pMenuButton->GetParentWnd() != NULL &&
        pMenuButton->GetParentWnd()->IsKindOf(RUNTIME_CLASS(PopupMenuBar));
    if (bIsPopupMenu &&
        (pButton->s->m_nStyle & TBBS_CHECKED) &&
        pBrush == s->m_clrHighlightDn) {
      imdraw_FillRect(im, 0, rect, s->m_clrMenuItemCheckedHighlight);
      return 0;
    }
    if (pMenuButton != NULL && !bIsPopupMenu && pMenuButton->IsDroppedDown()) {
      imdraw_FillRect(im, 0, rect, s->m_clrMenuButtonDroppedDown);
      return 0;
    }
  }
  VisualManager2003_OnFillHighlightedArea(im, rect, pBrush, pButton);
}
int VisualManagerVS2005::GetDockingTabsBordersSize()
{
  return
      TabbedControlBar::s->m_StyleTabWnd == TabWnd::STYLE_3D_ROUNDED ?
      0 : 3;
}
#ifndef BCGP_EXCLUDE_PROP_LIST
COLOR VisualManagerVS2005::GetPropListGroupColor(PropList* pPropList)
{
  ASSERT_VALID(pPropList);
  if (s->m_bDefaultWinXPColors) {
    return VisualManager2003_GetPropListGroupColor(pPropList);
  }
  return pPropList->DrawControlBarColors() ?
      s->clrBarLight : s->clrBtnLight;
}
#endif // BCGP_EXCLUDE_PROP_LIST
COLOR VisualManagerVS2005::OnFillMiniFrameCaption(img_t* im,
    IRECT rectCaption,
    MiniFrameWnd* pFrameWnd,
    BOOL bActive)
{
  ASSERT_VALID(pFrameWnd);
  if (DYNAMIC_DOWNCAST(BaseToolBar, pFrameWnd->GetControlBar()) == NULL) {
    return VisualManager2003_OnFillMiniFrameCaption(im,
        rectCaption, pFrameWnd, bActive);
  }
  ::FillRect(im, rectCaption, GetSysColorBrush(COLOR_3DSHADOW));
  return s->clrCaptionText;
}
#ifndef BCGP_EXCLUDE_TASK_PANE
int VisualManagerVS2005::OnDrawToolBoxFrame(img_t* im, const IRECT& rect)
{
  imdraw_Draw3dRect(im, 0, rect, s->clrBarShadow, s->clrBarShadow);
}
#endif // BCGP_EXCLUDE_TASK_PANE
//
// BCGPVisualManagerVS2008.cpp: implementation of the VisualManagerVS2008 class.
//
//////////////////////////////////////////////////////////////////////
#include "BCGPVisualManagerVS2008.h"
#include "BCGPToolbarMenuButton.h"
#include "BCGPDrawManager.h"
#include "BCGPTabWnd.h"
#include "BCGPAutoHideButton.h"
#include "BCGPColorBar.h"
#include "BCGPCalculator.h"
#include "BCGPCalendarBar.h"
#include "bcgpstyle.h"
#define WINXPBLUE_GRADIENT_LIGHT _RGB(239, 243, 250)
#define WINXPBLUE_GRADIENT_DARK _RGB(193, 210, 238)
#define WINXPBLUE_MENUITEs->m_BORDER _RGB(152, 181, 226)
#define WINXPBLUE_MENU_GUTTER _RGB(241, 241, 241)
#define WINXPBLUE_MENU_GUTTER_DARK _RGB(225, 225, 225)
IMPLEMENT_DYNCREATE(VisualManagerVS2008, VisualManagerVS2005)
VisualManagerVS2008::VisualManagerVS2008()
{
  s->m_bConnectMenuToParent = FALSE;
  s->m_bShdowDroppedDownMenuButton = FALSE;
  s->m_bOSColors = FALSE;
  OnUpdateSystemColors();
}
VisualManagerVS2008::~VisualManagerVS2008()
{
}
int VisualManagerVS2008::OnFillBarBackground(img_t* im, BaseControlBar* pBar,
    IRECT rectClient, IRECT rectClip,
    BOOL bNCArea)
{
  ASSERT_VALID(pBar);
  if (pBar->IsOnGlass()) {
    im->FillSolidRect(rectClient, _RGB(0, 0, 0));
    return 0;
  }
  if (s->m_nBitsPerPixel <= 8 ||
      IsHighContastMode(s) ||
      !pBar->IsKindOf(RUNTIME_CLASS(PopupMenuBar)) ||
      pBar->IsKindOf(RUNTIME_CLASS(ColorBar)) ||
      pBar->IsKindOf(RUNTIME_CLASS(Calculator)) ||
      pBar->IsKindOf(RUNTIME_CLASS(CalendarBar)) ||
      GetStandardWinXPTheme() != WinXpTheme_Blue) {
    VisualManagerVS2005::OnFillBarBackground(im, pBar, rectClient, rectClip, bNCArea);
    return 0;
  }
  if (rectClip.IsRectEmpty()) {
    rectClip = rectClient;
  }
  imdraw_FillRect(im, 0, rectClip, s->m_clrMenuLight);
  PopupMenuBar* pMenuBar = DYNAMIC_DOWNCAST(PopupMenuBar, pBar);
  ASSERT_VALID(pMenuBar);
  if (!pMenuBar->s->m_bDisableSideBarInXPMode) {
    IRECT rectImages = rectClient;
    rectImages.r = rectImages.l + pMenuBar->GetGutterWidth();
    rectImages.DeflateRect(0, 1);
    CBrush br(WINXPBLUE_MENU_GUTTER);
    imdraw_FillRect(im, 0, rectImages, &br);
    CPen* pOldPen = im->SelectObject(&s->m_penSeparator);
    ASSERT(pOldPen != NULL);
    im->MoveTo(rectImages.r, rectImages.t);
    im->LineTo(rectImages.r, rectImages.b);
    im->SelectObject(pOldPen);
  }
}
int VisualManagerVS2008::OnHighlightRarelyUsedMenuItems(img_t* im, IRECT rectRarelyUsed)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s) ||
      GetStandardWinXPTheme() != WinXpTheme_Blue) {
    VisualManagerVS2005::OnHighlightRarelyUsedMenuItems(im, rectRarelyUsed);
    return 0;
  }
  rectRarelyUsed.l --;
  rectRarelyUsed.r = rectRarelyUsed.l + ToolBar::GetMenuImageSize().w +
      2 * GetMenuImageMargin() + 2;
  CBrush br(WINXPBLUE_MENU_GUTTER_DARK);
  imdraw_FillRect(im, 0, rectRarelyUsed, &br);
}
int VisualManagerVS2008::OnDrawButtonBorder(img_t* im,
    ToolbarButton* pButton, IRECT rect, BCGBUTTON_STATE state)
{
  ToolbarMenuButton* pMenuButton =
      DYNAMIC_DOWNCAST(ToolbarMenuButton, pButton);
  BOOL bIsMenuBarButton = pMenuButton != NULL &&
      pMenuButton->GetParentWnd() != NULL &&
      pMenuButton->GetParentWnd()->IsKindOf(RUNTIME_CLASS(MenuBar));
  if (bIsMenuBarButton) {
    rect.b -= 2;
  }
  if (!bIsMenuBarButton || !pMenuButton->IsDroppedDown()) {
    VisualManagerVS2005::OnDrawButtonBorder(im, pButton, rect, state);
    return 0;
  }
  imdraw_Draw3dRect(im, 0, rect, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
}
int VisualManagerVS2008::OnFillButtonInterior(img_t* im,
    ToolbarButton* pButton, IRECT rect, VisualManager::BCGBUTTON_STATE state)
{
  ToolbarMenuButton* pMenuButton =
      DYNAMIC_DOWNCAST(ToolbarMenuButton, pButton);
  BOOL bIsMenuBarButton = pMenuButton != NULL &&
      pMenuButton->GetParentWnd() != NULL &&
      pMenuButton->GetParentWnd()->IsKindOf(RUNTIME_CLASS(MenuBar));
  if (bIsMenuBarButton) {
    rect.b -= 2;
  }
  if (!bIsMenuBarButton || !pMenuButton->IsDroppedDown()) {
    VisualManagerVS2005::OnFillButtonInterior(im, pButton, rect, state);
    return 0;
  }
  if (!s->m_bOSColors) {
    OnFillHighlightedArea(im, rect, s->m_clrBarBkgnd, pButton);
    return 0;
  }
  COLOR clr1 = PixelAlpha2(s->m_clrHighlight, 85);
  COLOR clr2 = _RGB(255, 255, 255);
  DrawManager dm(*im);
  dm.FillGradient(rect, clr1, clr2, TRUE);
}
int VisualManagerVS2008::OnHighlightMenuItem(img_t* im, ToolbarMenuButton* pButton,
    IRECT rect, COLOR& clrText)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    VisualManagerVS2005::OnHighlightMenuItem(im, pButton, rect, clrText);
    return 0;
  }
  const int nRoundSize = 3;
  const BOOL bIsWinXPBlue = (GetStandardWinXPTheme() == WinXpTheme_Blue);
  COLOR clr1 = bIsWinXPBlue ? WINXPBLUE_GRADIENT_DARK : s->m_clrHighlightGradientDark;
  COLOR clr2 = bIsWinXPBlue ? WINXPBLUE_GRADIENT_LIGHT : s->m_clrHighlightGradientLight;
  COLOR clrBorder = bIsWinXPBlue ? WINXPBLUE_MENUITEs->m_BORDER : s->m_clrHighlightGradientDark;
  if (s->m_bOSColors) {
    clr1 = s->m_clrHighlight;
    clr2 = _RGB(255, 255, 255);
    clrBorder = s->m_clrHighlightDn;
  }
  rect.DeflateRect(2, 0);
  CRgn rgn;
  rgn.CreateRoundRectRgn(rect.l, rect.t, rect.r, rect.b, nRoundSize, nRoundSize);
  im->SelectClipRgn(&rgn);
  DrawManager dm(*im);
  dm.FillGradient(rect, clr1, clr2, TRUE);
  im->SelectClipRgn(NULL);
  CPen pen(PS_SOLID, 1, clrBorder);
  CPen* pOldPen = im->SelectObject(&pen);
  COLOR pOldBrush = (COLOR) im->SelectStockObject(NULL_BRUSH);
  im->RoundRect(rect.l, rect.t, rect.r, rect.b, nRoundSize + 2, nRoundSize + 2);
  im->SelectObject(pOldPen);
  im->SelectObject(pOldBrush);
  clrText = GetHighlightedMenuItemTextColor(pButton);
}
int VisualManagerVS2008::OnEraseTabsArea(img_t* im, IRECT rect,
    const BaseTabWnd* pTabWnd)
{
  ASSERT_VALID(pTabWnd);
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s) ||
      !pTabWnd->IsVS2005Style() ||
      pTabWnd->IsDialogControl()) {
    VisualManagerVS2005::OnEraseTabsArea(im, rect, pTabWnd);
    return 0;
  }
  CFrameWnd* pMainFrame = BCGCBProGetTopLevelFrame(pTabWnd);
  if (pMainFrame->GetSafeHwnd() != NULL) {
    IRECT rectMain;
    pMainFrame->GetClientRect(rectMain);
    pMainFrame->MapWindowPoints((TabWnd*)pTabWnd, &rectMain);
    rect.t = rectMain.t;
    rect.l = rectMain.l;
    rect.r = rect.l + s->m_rectVirtual.Width() + 10;
  }
  DrawManager dm(*im);
  dm.FillGradient(rect, s->m_clrBarGradientDark, s->m_clrBarGradientLight, FALSE, 0);
}
int VisualManagerVS2008::OnEraseTabsButton(img_t* im, IRECT rect,
    Button* pButton,
    BaseTabWnd* pBaseTab)
{
  ASSERT_VALID(pBaseTab);
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s) ||
      !pBaseTab->IsVS2005Style() ||
      pBaseTab->IsDialogControl() ||
      pButton->IsPressed() || pButton->IsHighlighted()) {
    VisualManagerVS2005::OnEraseTabsButton(im, rect, pButton, pBaseTab);
    return 0;
  }
  CRgn rgn;
  rgn.CreateRectRgnIndirect(rect);
  im->SelectClipRgn(&rgn);
  CFrameWnd* pMainFrame = BCGCBProGetTopLevelFrame(pButton);
  if (pMainFrame->GetSafeHwnd() != NULL) {
    IRECT rectMain;
    pMainFrame->GetClientRect(rectMain);
    pMainFrame->MapWindowPoints(pButton, &rectMain);
    rect.t = rectMain.t;
    rect.l = rectMain.l;
    rect.r = rect.l + s->m_rectVirtual.Width() + 10;
  }
  DrawManager dm(*im);
  dm.FillGradient(rect, s->m_clrBarGradientDark, s->m_clrBarGradientLight, FALSE, 0);
  im->SelectClipRgn(NULL);
}
BOOL VisualManagerVS2008::OnEraseTabsFrame(img_t* im, IRECT rect, const BaseTabWnd* pTabWnd)
{
  ASSERT_VALID(pTabWnd);
  if (!pTabWnd->IsVS2005Style() || s->m_nBitsPerPixel <= 8 ||
      IsHighContastMode(s)) {
    return VisualManagerVS2005::OnEraseTabsFrame(im, rect, pTabWnd);
  }
  COLOR clrActiveTab = pTabWnd->GetTabBkColor(pTabWnd->GetActiveTab());
  if (clrActiveTab == (COLOR) - 1) {
    clrActiveTab = s->m_clrHighlight;
  }
  CBrush brFill(clrActiveTab);
  imdraw_FillRect(im, 0, rect, &brFill);
  return TRUE;
}
int VisualManagerVS2008::OnDrawTab(img_t* im, IRECT rectTab,
    int iTab, BOOL bIsActive, const BaseTabWnd* pTabWnd)
{
  ASSERT_VALID(pTabWnd);
  const COLOR clrTab = pTabWnd->GetTabBkColor(iTab);
  const BOOL bIsHighlight = (iTab == pTabWnd->GetHighlightedTab());
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s) ||
      pTabWnd->IsDialogControl() ||
      pTabWnd->IsFlatTab() ||
      clrTab != (COLOR) - 1) {
    VisualManagerVS2005::OnDrawTab(im, rectTab, iTab, bIsActive, pTabWnd);
    return 0;
  }
  if ((bIsActive || bIsHighlight || s->m_bOSColors) && pTabWnd->IsVS2005Style()) {
    ((BaseTabWnd*)pTabWnd)->SetTabBkColor(iTab,
        bIsActive ? s->m_clrHighlight :
        bIsHighlight ? s->m_clrHighlightDnGradientDark : GetThemeColor(s->m_hThemeButton, 2));
    VisualManagerVS2005::OnDrawTab(im, rectTab, iTab, bIsActive, pTabWnd);
    ((BaseTabWnd*)pTabWnd)->SetTabBkColor(iTab, clrTab);
    return 0;
  }
  if (s->m_hThemeTab == NULL ||
      pTabWnd->IsOneNoteStyle() || pTabWnd->IsVS2005Style() ||
      pTabWnd->IsLeftRightRounded()) {
    IRECT rectClip = rectTab;
    rectClip.b -= 2;
    CRgn rgn;
    rgn.CreateRectRgnIndirect(&rectClip);
    im->SelectClipRgn(&rgn);
    VisualManagerVS2005::OnDrawTab(im, rectTab, iTab, bIsActive, pTabWnd);
    im->SelectClipRgn(NULL);
    return 0;
  }
  int nState = TIS_NORMAL;
  if (bIsActive) {
    nState = TIS_SELECTED;
  }
  else if (iTab == pTabWnd->GetHighlightedTab()) {
    nState = TIS_HOT;
  }
  rectTab.r += 2;
  if (!bIsActive) {
    rectTab.b--;
  }
  if (rectTab.Width() > 25) { // DrawThemeBackground will draw < 25 width tab bad
    (*s->m_pfDrawThemeBackground)(s->m_hThemeTab, im, TABP_TABITEM, nState, &rectTab, 0);
    if (pTabWnd->GetLocation() == BaseTabWnd::LOCATION_BOTTOM) {
      DrawManager dm(*im);
      dm.MirrorRect(rectTab, FALSE);
    }
  }
  COLOR clrTabText = s->clrWindowText;
  if (!bIsActive) {
    clrTabText = s->clrBtnDkShadow;
  }
  else if (s->m_pfGetThemeColor != NULL) {
    (*s->m_pfGetThemeColor)(s->m_hThemeTab, TABP_TABITEM, nState, TMT_TEXTCOLOR, &clrTabText);
  }
  COLOR cltTextOld = im->SetTextColor(clrTabText);
  rectTab.r -= 2;
  OnDrawTabContent(im, rectTab, iTab, bIsActive, pTabWnd, (COLOR) - 1);
  im->SetTextColor(cltTextOld);
}
int VisualManagerVS2008::OnDrawAutoHideButtonBorder(img_t* im, IRECT rect, IRECT rectBorderSize, AutoHideButton* pButton)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s) ||
      s->m_hThemeTab == NULL) {
    VisualManagerVS2005::OnDrawAutoHideButtonBorder(im, rect, rectBorderSize, pButton);
    return 0;
  }
  const int nState = pButton->IsHighlighted() ? TIS_HOT : TIS_NORMAL;
  const DWORD dwAlign = (pButton->GetAlignment()) & CBRS_ALIGN_ANY;
  DrawManager dm(*im);
  switch (dwAlign) {
  case CBRS_ALIGN_LEFT:
  case CBRS_ALIGN_RIGHT: {
    IRECT rectTab(0, 0, rect.Height(), rect.Width());
    CDC dcMem;
    dcMem.CreateCompatibleDC(im);
    CBitmap bmpMem;
    bmpMem.CreateCompatibleBitmap(im, rectTab.Width(), rectTab.Height());
    CBitmap* pBmpOld = (CBitmap*) dcMem.SelectObject(&bmpMem);
    (*s->m_pfDrawThemeBackground)(s->m_hThemeTab, dcMem.GetSafeHdc(), TABP_TABITEM, nState, &rectTab, 0);
    dm.DrawRotated(rect, dcMem, dwAlign == CBRS_ALIGN_LEFT);
    dcMem.SelectObject(pBmpOld);
  }
  break;
  case CBRS_ALIGN_TOP:
  case CBRS_ALIGN_BOTTOM:
    (*s->m_pfDrawThemeBackground)(s->m_hThemeTab, im, TABP_TABITEM, nState, &rect, 0);
    if (dwAlign == CBRS_ALIGN_TOP) {
      dm.MirrorRect(rect, FALSE);
      break;
    }
  }
}
COLOR VisualManagerVS2008::OnDrawControlBarCaption(img_t* im, DockingControlBar* pBar,
    BOOL bActive, IRECT rectCaption, IRECT rectButtons)
{
  if (!s->m_bOSColors) {
    return VisualManagerVS2005::OnDrawControlBarCaption(im, pBar,
        bActive, rectCaption, rectButtons);
  }
  rectCaption.b++;
  DrawManager dm(*im);
  dm.FillGradient(rectCaption,
      bActive ? s->clrActiveCaptionGradient : s->clrInactiveCaptionGradient,
      bActive ? s->clrActiveCaption : s->clrInactiveCaption,
      TRUE);
  return bActive ? s->clrCaptionText : s->clrInactiveCaptionText;
}
int VisualManagerVS2008::OnUpdateSystemColors()
{
  s->m_bOSColors = s->bIsWindowsVista &&
      s->m_hThemeExplorerBar != NULL &&
      s->m_nBitsPerPixel > 8 && !IsHighContastMode(s);
  VisualManagerVS2005::OnUpdateSystemColors();
  if (!s->m_bOSColors || s->m_pfGetThemeColor == NULL) {
    return 0;
  }
  (*s->m_pfGetThemeColor)(s->m_hThemeExplorerBar, 0, 0, TMT_EDGEHIGHLIGHTCOLOR, s->m_clrToolBarGradientLight);
  (*s->m_pfGetThemeColor)(s->m_hThemeExplorerBar, 0, 0, TMT_GRADIENTCOLOR2, s->m_clrToolBarGradientDark);
  s->m_clrBarGradientDark = DrawManager::SmartMixColors(s->m_clrToolBarGradientDark, s->m_clrToolBarGradientLight,
      1., 2, 1);
  s->m_clrBarGradientLight = s->m_clrToolBarGradientLight;
  s->m_clrToolBarGradientVertLight = s->m_clrToolBarGradientLight;
  s->m_clrToolBarGradientVertDark = PixelAlpha2(
      s->m_clrToolBarGradientDark, 98);
  //-------------------------------------
  // Calculate highlight gradient colors:
  //-------------------------------------
  s->m_clrCustomizeButtonGradientLight = s->m_clrToolBarGradientDark;
  s->m_clrCustomizeButtonGradientDark = s->m_clrBarGradientDark;
  s->m_clrToolBarBottomLine = PixelAlpha2(s->m_clrToolBarGradientDark, 85);
  s->m_colorToolBarCornerBottom = s->m_clrToolBarGradientDark;
  s->m_clrTabBack.DeleteObject();
  s->m_clrTabBack.CreateSolidBrush(s->m_clrToolBarGradientLight);
  s->m_clrFace.DeleteObject();
  s->m_clrFace.CreateSolidBrush(s->m_clrToolBarGradientLight);
  s->m_clrToolbarDisabled = DrawManager::SmartMixColors(
      s->m_clrToolBarGradientDark, s->m_clrToolBarGradientLight, .92);
  s->m_penBottomLine.DeleteObject();
  s->m_penBottomLine.CreatePen(PS_SOLID, 1, s->m_clrToolBarBottomLine);
  //--------------------------------------
  // Calculate grid/report control colors:
  //--------------------------------------
  s->m_penGridExpandBoxLight.DeleteObject();
  s->m_penGridExpandBoxLight.CreatePen(PS_SOLID, 1,
      PixelAlpha2(s->m_clrToolBarBottomLine, 210));
  s->m_penGridExpandBoxDark.DeleteObject();
  s->m_penGridExpandBoxDark.CreatePen(PS_SOLID, 1,
      PixelAlpha2(s->m_clrToolBarBottomLine, 75));
}
COLOR VisualManagerVS2008::OnFillCommandsListBackground(img_t* im, IRECT rect, BOOL bIsSelected)
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s)) {
    return VisualManagerVS2005::OnFillCommandsListBackground(im, rect, bIsSelected);
  }
  ASSERT_VALID(this);
  rect.l = 0;
  const BOOL bIsWinXPBlue = (GetStandardWinXPTheme() == WinXpTheme_Blue) || s->m_bOSColors;
  if (bIsSelected) {
    COLOR clr1 = bIsWinXPBlue ? WINXPBLUE_GRADIENT_DARK : s->m_clrHighlightGradientDark;
    COLOR clr2 = bIsWinXPBlue ? WINXPBLUE_GRADIENT_LIGHT : s->m_clrHighlightGradientLight;
    COLOR clrBorder = bIsWinXPBlue ? WINXPBLUE_MENUITEs->m_BORDER : s->m_clrHighlightGradientDark;
    if (s->m_bOSColors) {
      clr1 = s->m_clrHighlight;
      clr2 = _RGB(255, 255, 255);
      clrBorder = s->m_clrHighlightDn;
    }
    DrawManager dm(*im);
    dm.FillGradient(rect, clr1, clr2, TRUE);
    imdraw_Draw3dRect(im, 0, rect, clrBorder, clrBorder);
    ToolbarMenuButton dummy;
    return GetHighlightedMenuItemTextColor(&dummy);
  }
  else {
    imdraw_FillRect(im, 0, rect, s->m_clrMenuLight);
    int iImageWidth = ToolBar::GetMenuImageSize().w + GetMenuImageMargin();
    IRECT rectImages = rect;
    rectImages.r = rectImages.l + iImageWidth + MENU_IMAGE_MARGIN;
    if (bIsWinXPBlue) {
      CBrush br(WINXPBLUE_MENU_GUTTER);
      imdraw_FillRect(im, 0, rectImages, &br);
      CPen* pOldPen = im->SelectObject(&s->m_penSeparator);
      ASSERT(pOldPen != NULL);
      im->MoveTo(rectImages.r, rectImages.t);
      im->LineTo(rectImages.r, rectImages.b);
      im->SelectObject(pOldPen);
    }
    else {
      DrawManager dm(*im);
      dm.FillGradient(rectImages, s->m_clrToolBarGradientLight, s->m_clrToolBarGradientDark, FALSE);
    }
    return s->clrBarText;
  }
}
int VisualManagerVS2008::GetTabFrameColors(const BaseTabWnd* pTabWnd,
    COLOR& clrDark,
    COLOR& clrBlack,
    COLOR& clrHighlight,
    COLOR& clrFace,
    COLOR& clrDarkShadow,
    COLOR& clrLight,
    COLOR& pbrFace,
    COLOR& pbrBlack)
{
  ASSERT_VALID(pTabWnd);
  VisualManagerVS2005::GetTabFrameColors(pTabWnd,
      clrDark, clrBlack,
      clrHighlight, clrFace,
      clrDarkShadow, clrLight,
      pbrFace, pbrBlack);
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s) ||
      pTabWnd->IsFlatTab()) {
    return 0;
  }
  clrBlack = s->clrBarShadow;
}
int VisualManagerVS2008::OnDrawTabResizeBar(img_t* im, BaseTabWnd* pWndTab,
    BOOL bIsVert, IRECT rect,
    COLOR pbrFace, CPen* pPen)
{
  ASSERT_VALID(pbrFace);
  ASSERT_VALID(pPen);
  ASSERT_VALID(pWndTab);
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s) || pWndTab->IsFlatTab()) {
    VisualManagerVS2005::OnDrawTabResizeBar(im, pWndTab, bIsVert, rect, pbrFace, pPen);
    return 0;
  }
  CRgn rgn;
  rgn.CreateRectRgnIndirect(rect);
  im->SelectClipRgn(&rgn);
  CFrameWnd* pMainFrame = BCGCBProGetTopLevelFrame(pWndTab);
  if (pMainFrame->GetSafeHwnd() != NULL) {
    IRECT rectMain;
    pMainFrame->GetClientRect(rectMain);
    pMainFrame->MapWindowPoints(pWndTab, &rectMain);
    rect.t = rectMain.t;
    rect.l = rectMain.l;
    rect.r = rect.l + s->m_rectVirtual.Width() + 10;
  }
  DrawManager dm(*im);
  dm.FillGradient(rect, s->m_clrBarGradientDark, s->m_clrBarGradientLight, FALSE, 0);
  im->SelectClipRgn(NULL);
}
BCGP_SMARTDOCK_THEME VisualManagerVS2008::GetSmartDockingTheme()
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s) || !s->IsWindowsLayerSupportAvailable() ||
      !s->bIsWindowsVista) {
    return VisualManagerVS2005::GetSmartDockingTheme();
  }
  return BCGP_SDT_VS2008;
}
//
// BCGPVisualManager2007.cpp: implementation of the VisualManager2007 class.
//
//////////////////////////////////////////////////////////////////////
//#include "BCGCBPro.h"
#include "BCGPGlobalUtils.h"
#include "BCGPVisualManager2007.h"
#include "BCGPToolBar.h"
#include "BCGPDrawManager.h"
#include "BCGPPopupMenuBar.h"
#include "BCGPMenuBar.h"
#include "bcgglobals.h"
#include "BCGPToolbarMenuButton.h"
#include "CustomizeButton.h"
#include "MenuImages.h"
#include "BCGPCaptionBar.h"
#include "BCGPBaseTabWnd.h"
#include "BCGPColorBar.h"
#include "BCGPCalculator.h"
#include "BCGPCalendarBar.h"
#include "BCGPTabWnd.h"
#include "BCGPTasksPane.h"
#include "BCGPStatusBar.h"
#include "BCGPAutoHideButton.h"
#include "BCGPHeaderCtrl.h"
#include "BCGPReBar.h"
#include "BCGPToolBox.h"
#include "BCGPPopupWindow.h"
#include "BCGPCalendarBar.h"
#include "BCGPDropDown.h"
#include "BCGPTagManager.h"
#include "BCGPFrameWnd.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPDockingControlBar.h"
#include "BCGPOutlookWnd.h"
#include "BCGPGridCtrl.h"
#include "BCGPToolbarComboBoxButton.h"
#include "BCGCBProVer.h"
#include "BCGPGroup.h"
#include "BCGPDialog.h"
#include "BCGPPropertySheet.h"
#ifndef BCGP_EXCLUDE_RIBBON
#include "BCGPRibbonBar.h"
#include "BCGPRibbonPanel.h"
#include "BCGPRibbonCategory.h"
#include "BCGPRibbonButton.h"
#include "BCGPRibbonQuickAccessToolbar.h"
#include "BCGPRibbonComboBox.h"
#include "BCGPRibbonMainPanel.h"
#include "BCGPRibbonPanelMenu.h"
#include "BCGPRibbonLabel.h"
#include "BCGPRibbonPaletteButton.h"
#include "BCGPRibbonStatusBar.h"
#include "BCGPRibbonStatusBarPane.h"
#include "BCGPRibbonProgressBar.h"
#include "BCGPRibbonHyperlink.h"
#include "BCGPRibbonSlider.h"
#endif
#ifndef BCGP_EXCLUDE_PLANNER
#include "BCGPPlannerViewDay.h"
#include "BCGPPlannerViewMonth.h"
#endif
#include "BCGPGanttChart.h"
#define RT_BCG_STYLE_XML ("STYLE_XML")
const VisualManager2007_Style c_StyleDefault =
    VisualManager2007_VS2007_LunaBlue;
VisualManager2007_Style VisualManager2007_s->m_Style = c_StyleDefault;
CString VisualManager2007_s->m_strStylePrefix;
HINSTANCE VisualManager2007_s->m_hinstRes = NULL;
BOOL VisualManager2007_s->m_bAutoFreeRes = FALSE;
COLOR VisualManager2007_s->m_clrBase = (COLOR) - 1;
COLOR VisualManager2007_s->m_clrTarget = (COLOR) - 1;
static COLOR CalculateHourLineColor2007(COLOR clrBase, BOOL bWorkingHours, BOOL bHour)
{
  double H, S, V;
  DrawManager::RGBtoHSV(clrBase, &H, &S, &V);
  if (bHour) {
    S = S * 0.77;
    V = min(V * 1.03, 1.0);
  }
  else {
    if (bWorkingHours) {
      S = S * 0.2;
      V = min(V * 1.14, 1.0);
    }
    else {
      S = S * 0.34;
      V = min(V * 1.12, 1.0);
    }
  }
  return DrawManager::HSVtoRGB(H, S, V);
}
static COLOR CalculateWorkingColor2007(COLOR /*clrBase*/)
{
  return _RGB(255, 255, 255);
}
static COLOR CalculateNonWorkingColor2007(COLOR clrBase, BOOL bLight)
{
  if (bLight) {
    return CalculateHourLineColor2007(clrBase, TRUE, FALSE);
  }
  return CalculateHourLineColor2007(clrBase, FALSE, TRUE);
}
static COLOR CalculateSelectionColor(COLOR clrBase)
{
  double H, S, V;
  DrawManager::RGBtoHSV(clrBase, &H, &S, &V);
  return DrawManager::HSVtoRGB(H, min(S * 1.88, 1.0), V * 0.56);
}
static COLOR CalculateSeparatorColor(COLOR clrBase)
{
  double H, S, V;
  DrawManager::RGBtoHSV(clrBase, &H, &S, &V);
  return DrawManager::HSVtoRGB(H, min(S * 1.6, 1.0), V * 0.85);
}
BitmapCache::CBitmapCacheItem::CBitmapCacheItem()
{
  s->m_bMirror = FALSE;
}
BitmapCache::CBitmapCacheItem::~CBitmapCacheItem()
{
}
int BitmapCache::CBitmapCacheItem::AddImage(CBitmap* hBmp)
{
  s->m_Images.AddImage(hBmp, TRUE);
}
int BitmapCache::CBitmapCacheItem::Cache(const CSize& size, ControlRenderer& renderer)
{
  s->m_Images.Clear();
  const int nCount = renderer.GetImageCount();
  if (nCount > 0) {
    s->m_Images.SetImageSize(size);
    s->m_Images.SetTransparentColor((COLOR) - 1);
    for (int i = 0; i < nCount; i++) {
      BITMAPINFO bi;
      memset(&bi, 0, sizeof(BITMAPINFO));
      // Fill in the BITMAPINFOHEADER
      bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      bi.bmiHeader.biWidth = size.w;
      bi.bmiHeader.biHeight = size.h;
      bi.bmiHeader.biPlanes = 1;
      bi.bmiHeader.biBitCount = 32;
      bi.bmiHeader.biCompression = BI_RGB;
      bi.bmiHeader.biSizeImage = size.h * size.w * 4;
      LPBYTE pBits = NULL;
      CBitmap* hDib = ::CreateDIBSection(
          NULL, &bi, DIB_RGB_COLORS, (int**)&pBits,
          NULL, NULL);
      if (hDib == NULL || pBits == NULL) {
        ASSERT(FALSE);
        break;
      }
      CDC dc;
      dc.CreateCompatibleDC(NULL);
      CBitmap* hOldDib = (CBitmap*)::SelectObject(dc.GetSafeHdc(), hDib);
      s->m_bMirror = renderer.IsMirror();
      if (s->m_bMirror) {
        renderer.Mirror();
      }
      renderer.Draw(&dc, IRECT(0, 0, size.w, size.h), i);
      if (s->m_bMirror) {
        renderer.Mirror();
      }
      ::SelectObject(dc.GetSafeHdc(), hOldDib);
      AddImage(hDib);
      ::DeleteObject(hDib);
    }
  }
}
int BitmapCache::CBitmapCacheItem::Draw(img_t* im, IRECT rect, int iImageIndex/* = 0*/,
    BYTE alphaSrc/* = 255*/)
{
  s->m_Images.DrawEx(im, IRECT(rect.TopLeft(), s->m_Images.GetImageSize()),
      iImageIndex, ToolBarImages::ImageAlignHorzLeft,
      ImageAlignVertTop, IRECT(0, 0, 0, 0), alphaSrc);
}
int BitmapCache::CBitmapCacheItem::DrawY(img_t* im, IRECT rect, CSize sides,
    int iImageIndex/* = 0*/, BYTE alphaSrc/* = 255*/)
{
  IRECT rectImage(CPoint(0, 0), s->m_Images.GetImageSize());
  ASSERT(rect.Height() == rectImage.Height());
  if (sides.w > 0) {
    IRECT rt(rectImage);
    if (s->m_bMirror) {
      rt.l = rectImage.r - sides.w;
      rectImage.r = rt.l;
    }
    else {
      rt.r = rt.l + sides.w;
      rectImage.l = rt.r;
    }
    s->m_Images.DrawEx(im, rect, iImageIndex, ToolBarImages::ImageAlignHorzLeft,
        ImageAlignVertTop, rt, alphaSrc);
  }
  if (sides.h > 0) {
    IRECT rt(rectImage);
    if (s->m_bMirror) {
      rt.r = rectImage.l + sides.h;
      rectImage.l = rt.r;
    }
    else {
      rt.l = rectImage.r - sides.h;
      rectImage.r = rt.l;
    }
    s->m_Images.DrawEx(im, rect, iImageIndex, ToolBarImages::ImageAlignHorzRight,
        ImageAlignVertTop, rt, alphaSrc);
  }
  if (rectImage.Width() > 0) {
    rect.DeflateRect(sides.w, 0, sides.h, 0);
    s->m_Images.DrawEx(im, rect, iImageIndex, ToolBarImages::ImageAlignHorzStretch,
        ImageAlignVertTop, rectImage, alphaSrc);
  }
}
BitmapCache::BitmapCache()
{
}
BitmapCache::~BitmapCache()
{
  Clear();
}
int BitmapCache::Clear()
{
  for (int i = 0; i < s->m_Cache.GetSize(); i++) {
    if (s->m_Cache[i] != NULL) {
      delete s->m_Cache[i];
    }
  }
  s->m_Cache.RemoveAll();
  s->m_Sizes.RemoveAll();
}
int BitmapCache::Cache(const CSize& size, ControlRenderer& renderer)
{
  if (FindIndex(size) != -1) {
    ASSERT(FALSE);
    return -1;
  }
  CBitmapCacheItem* pItem = new CBitmapCacheItem;
  pItem->Cache(size, renderer);
  int nCache = (int) s->m_Cache.Add(pItem);
  int nSize = (int) s->m_Sizes.Add(size);
  ASSERT(nCache == nSize);
  return nCache;
}
int BitmapCache::CacheY(int height, ControlRenderer& renderer)
{
  CSize size(renderer.GetParams().s->m_rectImage.Width(), height);
  return Cache(size, renderer);
}
BOOL BitmapCache::IsCached(const CSize& size) const
{
  return FindIndex(size) != -1;
}
int BitmapCache::FindIndex(const CSize& size) const
{
  int nRes = -1;
  for (int i = 0; i < s->m_Sizes.GetSize(); i++) {
    if (size == s->m_Sizes[i]) {
      nRes = i;
      break;
    }
  }
  return nRes;
}
BitmapCache::CBitmapCacheItem* BitmapCache::Get(const CSize& size)
{
  int nIndex = FindIndex(size);
  if (nIndex != -1) {
    return s->m_Cache[nIndex];
  }
  return NULL;
}
BitmapCache::CBitmapCacheItem* BitmapCache::Get(int nIndex)
{
  if (0 <= nIndex && nIndex < s->m_Cache.GetSize()) {
    return s->m_Cache[nIndex];
  }
  return NULL;
}
IMPLEMENT_DYNCREATE(VisualManager2007, VisualManager2003)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
VisualManager2007_VisualManager2007()
  : s->m_bNcTextCenter(FALSE)
  , s->m_bLoaded(FALSE)
  , s->m_bPlannerBlack(FALSE)
{
  s->m_szNcBtnSize[0] = CSize(0, 0);
  s->m_szNcBtnSize[1] = CSize(0, 0);
  s->m_ptRibbonMainImageOffset = CPoint(0, -1);
}
VisualManager2007_~VisualManager2007()
{
  MenuImages::SetColor(MenuImages::ImageBlack, (COLOR) - 1);
}
CString VisualManager2007_MakeResourceID(LPCTSTR lpszID)
{
  CString strResID(lpszID);
  ASSERT(!strResID.IsEmpty());
  if (!s->m_strStylePrefix.IsEmpty()) {
    strResID = s->m_strStylePrefix + strResID;
  }
  return strResID;
}
CString VisualManager2007_GetStyleResourceID(Style style)
{
  CString strResID(("IDX_OFFICE2007_STYLE"));
#if !defined _AFXDLL || defined _BCGCBPRO_STATIC_
  CString strStylePrefix;
  switch (style) {
  case VS2007_LunaBlue:
    strStylePrefix = ("BLUE_");
    break;
  case VS2007_ObsidianBlack:
    strStylePrefix = ("BLACK_");
    break;
  case VS2007_Aqua:
    strStylePrefix = ("AQUA_");
    break;
  case VS2007_Silver:
    strStylePrefix = ("SILVER_");
    break;
  default:
    ASSERT(FALSE);
  }
  strResID = strStylePrefix + strResID;
#else
  UNREFERENCED_PARAMETER(style);
#endif
  return strResID;
};
BOOL VisualManager2007_SetStyle(Style style, LPCTSTR lpszPath)
{
  if (s->m_Style == style && s->m_hinstRes > (HINSTANCE) 32) {
    return TRUE;
  }
#if defined _AFXDLL && !defined _BCGCBPRO_STATIC_
  CString strTheme;
  switch (style) {
  case VS2007_LunaBlue:
    strTheme = ("Luna");
    break;
  case VS2007_ObsidianBlack:
    strTheme = ("Obsidian");
    break;
  case VS2007_Aqua:
    strTheme = ("Aqua");
    break;
  case VS2007_Silver:
    strTheme = ("Silver");
    break;
  default:
    ASSERT(FALSE);
    return FALSE;
  }
  CString strVer;
  strVer.Format(("%d%d"), _BCGCBPRO_VERSION_MAJOR, _BCGCBPRO_VERSION_MINOR);
  CString strStyleDLLName = ("BCGPStyle2007") + strTheme + strVer + (".dll");
  CString strStyleDLLPath;
  if (lpszPath != NULL && _tcslen(lpszPath) > 0) {
    strStyleDLLPath = lpszPath;
    if (strStyleDLLPath [strStyleDLLPath.GetLength() - 1] != ('\\')) {
      strStyleDLLPath += ('\\');
    }
    strStyleDLLPath += strStyleDLLName;
  }
  else {
    strStyleDLLPath = strStyleDLLName;
  }
  Style oldStyle = s->m_Style;
  CleanStyle();
  HINSTANCE hinstRes = LoadLibrary(strStyleDLLPath);
  if (hinstRes <= (HINSTANCE) 32) {
    s->m_Style = oldStyle;
    TRACE(("Cannot load Style DLL: %s\r\n"), strStyleDLLPath);
    ASSERT(FALSE);
    return FALSE;
  }
  s->m_Style = style;
  SetResourceHandle(hinstRes);
  s->m_bAutoFreeRes = TRUE;
#else
  UNREFERENCED_PARAMETER(lpszPath);
  CString strStyle(GetStyleResourceID(style));
  HINSTANCE hinstRes = AfxFindResourceHandle(strStyle, RT_BCG_STYLE_XML);
  if (::FindResource(hinstRes, strStyle, RT_BCG_STYLE_XML) == NULL) {
    TRACE(("Cannot load Style: %s\r\n"), strStyle);
    ASSERT(FALSE);
    return FALSE;
  }
  CleanStyle();
  s->m_Style = style;
  SetResourceHandle(hinstRes);
#endif
  return TRUE;
}
VisualManager2007_Style VisualManager2007_GetStyle()
{
  return s->m_Style;
}
int VisualManager2007_SetResourceHandle(HINSTANCE hinstRes)
{
  s->m_bAutoFreeRes = FALSE;
  if (s->m_hinstRes != hinstRes) {
    s->m_hinstRes = hinstRes;
    if (VisualManager::GetInstance()->IsKindOf(
        RUNTIME_CLASS(VisualManager2007))) {
      VisualManager::GetInstance()->OnUpdateSystemColors();
    }
  }
}
int VisualManager2007_CleanStyle()
{
  if (s->m_bAutoFreeRes && s->m_hinstRes > (HINSTANCE) 32) {
    ::FreeLibrary(s->m_hinstRes);
  }
  s->m_hinstRes = NULL;
  s->m_Style = c_StyleDefault;
  s->m_strStylePrefix.Empty();
}
int VisualManager2007_SetCustomColor(COLOR clrTarget)
{
  if (clrTarget == (COLOR) - 1) {
    s->m_clrBase = (COLOR) - 1;
  }
  else {
    switch (s->m_Style) {
    case VS2007_LunaBlue:
      s->m_clrBase = _RGB(206, 221, 238); // Only 'LunaBlue' style is supported
      break;
    default:
      ASSERT(FALSE);
      clrTarget = s->m_clrBase = (COLOR) - 1;
    }
  }
  if (s->m_clrTarget == clrTarget) {
    return 0;
  }
  s->m_clrTarget = clrTarget;
  if (VisualManager::GetInstance()->IsKindOf(
      RUNTIME_CLASS(VisualManager2007))) {
    VisualManager::GetInstance()->OnUpdateSystemColors();
    MenuImages::CleanUp();
  }
}
int VisualManager2007_CleanUp()
{
  s->m_clrEditBorder = (COLOR)(-1);
  s->m_clrEditBorderDisabled = (COLOR)(-1);
  s->m_clrEditBorderHighlighted = (COLOR)(-1);
  s->m_clrEditSelection = (COLOR)(-1);
  s->m_clrComboBorder = (COLOR)(-1);
  s->m_clrComboBorderDisabled = (COLOR)(-1);
  s->m_clrComboBorderPressed = (COLOR)(-1);
  s->m_clrComboBorderHighlighted = (COLOR)(-1);
  s->m_clrComboBtnStart = (COLOR)(-1);
  s->m_clrComboBtnFinish = (COLOR)(-1);
  s->m_clrComboBtnBorder = (COLOR)(-1);
  s->m_clrComboBtnDisabledStart = (COLOR)(-1);
  s->m_clrComboBtnDisabledFinish = (COLOR)(-1);
  s->m_clrComboBtnBorderDisabled = (COLOR)(-1);
  s->m_clrComboBtnPressedStart = (COLOR)(-1);
  s->m_clrComboBtnPressedFinish = (COLOR)(-1);
  s->m_clrComboBtnBorderPressed = (COLOR)(-1);
  s->m_clrComboBtnHighlightedStart = (COLOR)(-1);
  s->m_clrComboBtnHighlightedFinish = (COLOR)(-1);
  s->m_clrComboBtnBorderHighlighted = (COLOR)(-1);
  s->m_clrComboSelection = (COLOR)(-1);
  s->m_ctrlComboBoxBtn.CleanUp();
  s->m_ToolBarGripper.Clear();
  s->m_ToolBarTear.Clear();
  s->m_ctrlToolBarBorder.CleanUp();
  s->m_ctrlStatusBarBack.CleanUp();
  s->m_ctrlStatusBarBack_Ext.CleanUp();
  s->m_StatusBarPaneBorder.Clear();
  s->m_StatusBarSizeBox.Clear();
  s->m_SysBtnBack[0].CleanUp();
  s->m_SysBtnBack[1].CleanUp();
  s->m_SysBtnClose[0].Clear();
  s->m_SysBtnClose[1].Clear();
  s->m_SysBtnRestore[0].Clear();
  s->m_SysBtnRestore[1].Clear();
  s->m_SysBtnMaximize[0].Clear();
  s->m_SysBtnMaximize[1].Clear();
  s->m_SysBtnMinimize[0].Clear();
  s->m_SysBtnMinimize[1].Clear();
  s->m_SysBtnHelp[0].Clear();
  s->m_SysBtnHelp[1].Clear();
  s->m_clrMainClientArea.DeleteObject();
  s->m_AppCaptionFont.DeleteObject();
  s->m_penSeparator2.DeleteObject();
  s->m_clrGroupBackground.DeleteObject();
  s->m_clrGroupText = (COLOR) - 1;
  s->m_penSeparatorDark.DeleteObject();
  s->m_ctrlMainBorder.CleanUp();
  s->m_ctrlMDIChildBorder.CleanUp();
  s->m_ctrlDialogBorder.CleanUp();
  s->m_ctrlMainBorderCaption.CleanUp();
  s->m_ctrlPopupBorder.CleanUp();
  s->m_ctrlPopupResizeBar.CleanUp();
  s->m_PopupResizeBar_HV.Clear();
  s->m_PopupResizeBar_HVT.Clear();
  s->m_PopupResizeBar_V.Clear();
  s->m_ctrlMenuBarBtn.CleanUp();
  s->m_ctrlMenuItemBack.CleanUp();
  s->m_MenuItemMarkerC.Clear();
  s->m_MenuItemMarkerR.Clear();
  s->m_ctrlMenuItemShowAll.CleanUp();
  s->m_ctrlMenuHighlighted[0].CleanUp();
  s->m_ctrlMenuHighlighted[1].CleanUp();
  s->m_ctrlMenuButtonBorder.CleanUp();
  s->m_ctrlMenuScrollBtn[0].CleanUp();
  s->m_ctrlMenuScrollBtn[1].CleanUp();
  s->m_ctrlToolBarBtn.CleanUp();
#ifndef BCGP_EXCLUDE_TASK_PANE
  s->m_ctrlTaskScrollBtn.CleanUp();
#endif
  int i, j;
  for (i = 0; i < 2; i++) {
    for (j = 0; j < 2; j++) {
      s->m_ctrlScrollBar_Back[i][j].CleanUp();
      s->m_ctrlScrollBar_Item[i][j].CleanUp();
      s->m_ctrlScrollBar_ThumbBack[i][j].CleanUp();
      s->m_ctrlScrollBar_ThumbIcon[i][j].CleanUp();
    }
  }
  s->m_ctrlSliderThumb[0].CleanUp();
  s->m_ctrlSliderThumb[1].CleanUp();
  s->m_ctrlSliderThumb[2].CleanUp();
  s->m_ctrlSliderThumb[3].CleanUp();
  s->m_ctrlSliderThumb[4].CleanUp();
  s->m_ctrlSliderThumb[5].CleanUp();
  s->m_ctrlTab3D[0].CleanUp();
  s->m_ctrlTab3D[1].CleanUp();
  s->m_ctrlTabFlat[0].CleanUp();
  s->m_ctrlTabFlat[1].CleanUp();
  s->m_clrTabTextActive = CLR_DEFAULT;
  s->m_clrTabTextInactive = CLR_DEFAULT;
  s->m_clrTab3DFace = CLR_DEFAULT;
  s->m_clrTab3DBlack = CLR_DEFAULT;
  s->m_clrTab3DDark = CLR_DEFAULT;
  s->m_clrTab3DDarkShadow = CLR_DEFAULT;
  s->m_clrTab3DLight = CLR_DEFAULT;
  s->m_clrTab3DHighlight = CLR_DEFAULT;
  s->m_clrTabFlatFace = CLR_DEFAULT;
  s->m_clrTabFlatBlack = CLR_DEFAULT;
  s->m_clrTabFlatDark = CLR_DEFAULT;
  s->m_clrTabFlatDarkShadow = CLR_DEFAULT;
  s->m_clrTabFlatLight = CLR_DEFAULT;
  s->m_clrTabFlatHighlight = CLR_DEFAULT;
  s->m_penTabFlatInner[0].DeleteObject();
  s->m_penTabFlatInner[1].DeleteObject();
  s->m_penTabFlatOuter[0].DeleteObject();
  s->m_penTabFlatOuter[1].DeleteObject();
  s->m_ctrlOutlookWndBar.CleanUp();
  s->m_ctrlOutlookWndPageBtn.CleanUp();
  s->m_ctrlRibbonCaptionQA.CleanUp();
  s->m_ctrlRibbonCaptionQA_Glass.CleanUp();
  s->m_ctrlRibbonCategoryBack.CleanUp();
  s->m_ctrlRibbonCategoryTab.CleanUp();
  s->m_ctrlRibbonCategoryTabSep.CleanUp();
  s->m_ctrlRibbonCategoryBtnPage[0].CleanUp();
  s->m_ctrlRibbonCategoryBtnPage[1].CleanUp();
  s->m_ctrlRibbonPanelBack_T.CleanUp();
  s->m_ctrlRibbonPanelBack_B.CleanUp();
  s->m_RibbonPanelSeparator.Clear();
  s->m_ctrlRibbonPanelQAT.CleanUp();
  s->m_ctrlRibbonMainPanel.CleanUp();
  s->m_ctrlRibbonMainPanelBorder.CleanUp();
  s->m_ctrlRibbonBtnMainPanel.CleanUp();
  s->m_ctrlRibbonBtnGroup_S.CleanUp();
  s->m_ctrlRibbonBtnGroup_F.CleanUp();
  s->m_ctrlRibbonBtnGroup_M.CleanUp();
  s->m_ctrlRibbonBtnGroup_L.CleanUp();
  s->m_ctrlRibbonBtnGroupMenu_F[0].CleanUp();
  s->m_ctrlRibbonBtnGroupMenu_F[1].CleanUp();
  s->m_ctrlRibbonBtnGroupMenu_M[0].CleanUp();
  s->m_ctrlRibbonBtnGroupMenu_M[1].CleanUp();
  s->m_ctrlRibbonBtnGroupMenu_L[0].CleanUp();
  s->m_ctrlRibbonBtnGroupMenu_L[1].CleanUp();
  s->m_ctrlRibbonBtn[0].CleanUp();
  s->m_ctrlRibbonBtn[1].CleanUp();
  s->m_ctrlRibbonBtnMenuH[0].CleanUp();
  s->m_ctrlRibbonBtnMenuH[1].CleanUp();
  s->m_ctrlRibbonBtnMenuV[0].CleanUp();
  s->m_ctrlRibbonBtnMenuV[1].CleanUp();
  s->m_ctrlRibbonBtnLaunch.CleanUp();
  s->m_RibbonBtnLaunchIcon.Clear();
  s->m_RibbonBtnMain.CleanUp();
  s->m_ctrlRibbonBtnDefault.CleanUp();
  s->m_ctrlRibbonBtnDefaultIcon.CleanUp();
  s->m_RibbonBtnDefaultImage.Clear();
  s->m_ctrlRibbonBtnDefaultQATIcon.CleanUp();
  s->m_ctrlRibbonBtnDefaultQAT.CleanUp();
  s->m_ctrlRibbonBtnCheck.CleanUp();
  s->m_ctrlRibbonBtnRadio.CleanUp();
  s->m_ctrlRibbonBtnPush.CleanUp();
  s->m_ctrlRibbonBtnGroup.CleanUp();
  s->m_ctrlRibbonBtnPalette[0].CleanUp();
  s->m_ctrlRibbonBtnPalette[1].CleanUp();
  s->m_ctrlRibbonBtnPalette[2].CleanUp();
  s->m_ctrlRibbonBtnStatusPane.CleanUp();
  s->m_ctrlRibbonSliderThumb.CleanUp();
  s->m_ctrlRibbonSliderThumbA[0].CleanUp();
  s->m_ctrlRibbonSliderThumbA[1].CleanUp();
  s->m_ctrlRibbonSliderThumbA[2].CleanUp();
  s->m_ctrlRibbonSliderThumbA[3].CleanUp();
  s->m_ctrlRibbonSliderThumbA[4].CleanUp();
  s->m_ctrlRibbonSliderBtnPlus.CleanUp();
  s->m_ctrlRibbonSliderBtnMinus.CleanUp();
  s->m_ctrlRibbonProgressBack.CleanUp();
  s->m_ctrlRibbonProgressNormal.CleanUp();
  s->m_ctrlRibbonProgressNormalExt.CleanUp();
  s->m_ctrlRibbonProgressInfinity.CleanUp();
  s->m_ctrlRibbonBorder_QAT.CleanUp();
  s->m_ctrlRibbonBorder_Floaty.CleanUp();
  s->m_ctrlRibbonKeyTip.CleanUp();
  s->m_clrRibbonKeyTipTextNormal = (COLOR)(-1);
  s->m_clrRibbonKeyTipTextDisabled = (COLOR)(-1);
  s->m_ctrlRibbonComboBoxBtn.CleanUp();
  s->m_cacheRibbonCategoryBack.Clear();
  s->m_cacheRibbonPanelBack_T.Clear();
  s->m_cacheRibbonPanelBack_B.Clear();
  s->m_cacheRibbonBtnDefault.Clear();
  s->m_cacheRibbonBtnGroup_S.Clear();
  s->m_cacheRibbonBtnGroup_F.Clear();
  s->m_cacheRibbonBtnGroup_M.Clear();
  s->m_cacheRibbonBtnGroup_L.Clear();
  s->m_cacheRibbonBtnGroupMenu_F[0].Clear();
  s->m_cacheRibbonBtnGroupMenu_M[0].Clear();
  s->m_cacheRibbonBtnGroupMenu_L[0].Clear();
  s->m_cacheRibbonBtnGroupMenu_F[1].Clear();
  s->m_cacheRibbonBtnGroupMenu_M[1].Clear();
  s->m_cacheRibbonBtnGroupMenu_L[1].Clear();
  s->m_ctrlRibbonContextPanelBack_T.CleanUp();
  s->m_ctrlRibbonContextPanelBack_B.CleanUp();
  s->m_cacheRibbonContextPanelBack_T.Clear();
  s->m_cacheRibbonContextPanelBack_B.Clear();
  s->m_ctrlRibbonContextSeparator.CleanUp();
  for (i = 0; i < BCGPRibbonCategoryColorCount; i++) {
    s->m_ctrlRibbonContextCategory[i].CleanUp();
  }
  s->m_clrPlannerTodayCaption[0] = _RGB(247, 208, 112);
  s->m_clrPlannerTodayCaption[1] = _RGB(251, 230, 148);
  s->m_clrPlannerTodayCaption[2] = _RGB(239, 155, 30);
  s->m_clrPlannerTodayCaption[3] = _RGB(250, 224, 139);
  s->m_clrPlannerTodayBorder = _RGB(238, 147, 17);
  s->m_clrPlannerNcArea = s->clrBtnFace;
  s->m_clrPlannerNcLine = s->clrBtnShadow;
  s->m_clrPlannerNcText = s->clrBtnText;
  s->m_clrCaptionBarText = s->clrWindow;
  s->m_penGridSeparator.DeleteObject();
  s->m_penGridSeparator.CreatePen(PS_SOLID, 1, s->clrBarShadow);
  s->m_clrGridLeftOffset = s->clrBarFace;
  s->m_bToolTipParams = FALSE;
  ToolTipParams dummy;
  s->m_ToolTipParams = dummy;
  s->m_ActivateFlag.RemoveAll();
  s->m_bPlannerBlack = FALSE;
  s->m_clrRibbonHyperlinkInactive = (COLOR) - 1;
  s->m_clrRibbonHyperlinkActive = (COLOR) - 1;
  s->m_clrRibbonStatusbarHyperlinkInactive = (COLOR) - 1;
  s->m_clrRibbonStatusbarHyperlinkActive = (COLOR) - 1;
  s->m_clrDlgBackground = (COLOR) - 1;
  s->m_clrDlgBackground.DeleteObject();
#ifndef BCGP_EXCLUDE_GRID_CTRL
  GridColors gridcolors;
  s->m_GridColors = gridcolors;
#endif
  s->m_bLoaded = FALSE;
}
int VisualManager2007_OnUpdateSystemColors()
{
  CleanUp();
  VisualManager2003_OnUpdateSystemColors();
  if (s->bIsWindows9x) {
    return 0;
  }
  if (!s->bIsOSAlphaBlendingSupport ||
      IsHighContastMode(s) ||
      s->m_nBitsPerPixel <= 8) {
    return 0;
  }
  s->m_nMenuBorderSize = 1;
  HINSTANCE hinstResOld = NULL;
  if (s->m_hinstRes == NULL) {
    SetStyle(c_StyleDefault);
  }
  if (s->m_hinstRes != NULL) {
    hinstResOld = AfxGetResourceHandle();
    AfxSetResourceHandle(s->m_hinstRes);
  }
  TagManager tm;
  TagManager::SetBaseColor(s->m_clrBase, s->m_clrTarget);
  if (!tm.LoadFromResource(GetStyleResourceID(s->m_Style), RT_BCG_STYLE_XML)) {
#if !defined _AFXDLL || defined _BCGCBPRO_STATIC_
    TRACE(("\r\nImportant: to enable Office 2007 look in static link, you need:\r\n"));
    TRACE(("1. Open \"Resource Includes\" dialog and add resource files:\r\n"));
    TRACE(("<BCGCBPro-Path>\\styles\\BCGPStyle2007Luna.rc\r\n"));
    TRACE(("<BCGCBPro-Path>\\styles\\BCGPStyle2007Obsidian.rc\r\n"));
    TRACE(("<BCGCBPro-Path>\\styles\\BCGPStyle2007Silver.rc\r\n"));
    TRACE(("<BCGCBPro-Path>\\styles\\BCGPStyle2007Aqua.rc\r\n"));
    TRACE(("2. Add path to this folder to \"Additional Resource Include Directories\"\r\n"));
    TRACE(("<BCGCBPro-Path>\\styles\r\n\r\n"));
    ASSERT(FALSE);
#endif
    if (hinstResOld != NULL) {
      AfxSetResourceHandle(hinstResOld);
    }
    TagManager::SetBaseColor((COLOR) - 1, (COLOR) - 1);
    return 0;
  }
  {
    CString strStyle;
    tm.ExcludeTag(("STYLE"), strStyle);
    tm.SetBuffer(strStyle);
  }
  CString strItem;
  s->m_nType = 20;
  if (!tm.IsEmpty()) {
    int nVersion = 0;
    if (tm.ExcludeTag(("VERSION"), strItem)) {
      TagManager tmItem(strItem);
      tmItem.ReadInt(("NUMBER"), nVersion);
      if (nVersion == 2007) {
        tmItem.ReadInt(("TYPE"), s->m_nType);
        if (s->m_nType < 10) {
          s->m_nType *= 10;
        }
        s->m_bLoaded = TRUE;
      }
      if (s->m_bLoaded) {
        if (tmItem.ExcludeTag(("ID_PREFIX"), strItem)) {
          strItem.TrimLeft();
          strItem.TrimRight();
          s->m_strStylePrefix = strItem;
        }
      }
    }
  }
  if (!s->m_bLoaded) {
    if (hinstResOld != NULL) {
      ::AfxSetResourceHandle(hinstResOld);
    }
    TagManager::SetBaseColor((COLOR) - 1, (COLOR) - 1);
    return 0;
  }
  // globals
  if (tm.ExcludeTag(("GLOBALS"), strItem)) {
    TagManager tmItem(strItem);
    tmItem.ReadColor(("BarText"), s->clrBarText);
    if (tmItem.ReadColor(("BarFace"), s->clrBarFace)) {
      s->clrBarFace.DeleteObject();
      s->clrBarFace.CreateSolidBrush(s->clrBarFace);
      s->m_clrMenuShadowBase = s->clrBarFace;
    }
    if (tmItem.ReadColor(("ActiveCaption"), s->clrActiveCaption)) {
      s->clrInactiveCaption = s->clrActiveCaption;
      s->clrActiveCaption.DeleteObject();
      s->clrActiveCaption.CreateSolidBrush(s->clrActiveCaption);
    }
    if (tmItem.ReadColor(("CaptionText"), s->clrCaptionText)) {
      s->clrInactiveCaptionText = s->clrCaptionText;
    }
    tmItem.ReadColor(("InactiveCaption"), s->clrInactiveCaption);
    s->clrInactiveCaption.DeleteObject();
    s->clrInactiveCaption.CreateSolidBrush(s->clrInactiveCaption);
    tmItem.ReadColor(("InactiveCaptionText"), s->clrInactiveCaptionText);
    tmItem.ReadColor(("BarShadow"), s->clrBarShadow);
    tmItem.ReadColor(("BarDkShadow"), s->clrBarDkShadow);
    tmItem.ReadColor(("BarLight"), s->clrBarLight);
    COLOR clrFloatToolBarBorder;
    tmItem.ReadColor(("FloatToolBarBorder"), clrFloatToolBarBorder);
    s->m_clrFloatToolBarBorder.DeleteObject();
    s->m_clrFloatToolBarBorder.CreateSolidBrush(clrFloatToolBarBorder);
    tmItem.ReadColor(("HighlightGradientDark"), s->m_clrHighlightGradientDark);
    tmItem.ReadColor(("HighlightGradientLight"), s->m_clrHighlightGradientLight);
    s->m_clrHighlightDnGradientDark = s->m_clrHighlightGradientLight;
    s->m_clrHighlightDnGradientLight = s->m_clrHighlightGradientDark;
    tmItem.ReadColor(("HighlightDnGradientDark"), s->m_clrHighlightDnGradientDark);
    tmItem.ReadColor(("HighlightDnGradientLight"), s->m_clrHighlightDnGradientLight);
    s->m_clrHighlightCheckedGradientDark = s->m_clrHighlightDnGradientLight;
    s->m_clrHighlightCheckedGradientLight = s->m_clrHighlightDnGradientDark;
    tmItem.ReadColor(("HighlightCheckedGradientDark"), s->m_clrHighlightCheckedGradientDark);
    tmItem.ReadColor(("HighlightCheckedGradientLight"), s->m_clrHighlightCheckedGradientLight);
    tmItem.ReadColor(("PressedButtonBorder"), s->m_clrPressedButtonBorder);
    COLOR clrHB = s->clrHilite;
    COLOR clrHT = s->clrTextHilite;
    if (tmItem.ReadColor(("Highlight"), clrHB) &&
        tmItem.ReadColor(("HighlightText"), clrHT)) {
      s->clrHilite = clrHB;
      s->clrHilite.DeleteObject();
      s->clrHilite.CreateSolidBrush(clrHB);
      s->clrTextHilite = clrHT;
    }
    tmItem.ReadColor(("MenuShadowColor"), s->m_clrMenuShadowBase);
    // dialog background
    s->m_clrDlgBackground = s->clrBarLight;
    tmItem.ReadColor(("DlgBackColor"), s->m_clrDlgBackground);
    s->m_clrDlgBackground.DeleteObject();
    s->m_clrDlgBackground.CreateSolidBrush(s->m_clrDlgBackground);
    // ToolTipParams
    s->m_bToolTipParams = tmItem.ReadToolTipParams(("TOOLTIP"), s->m_ToolTipParams);
  }
  // mainwnd
  if (tm.ExcludeTag(("MAINWND"), strItem)) {
    TagManager tmItem(strItem);
    // caption
    CString strCaption;
    if (tmItem.ExcludeTag(("CAPTION"), strCaption)) {
      TagManager tmCaption(strCaption);
      NONCLIENTMETRICS ncm;
      if (s->GetNonClientMetrics(ncm)) {
        tmCaption.ReadFont(("FONT"), ncm.lfCaptionFont);
        s->m_AppCaptionFont.DeleteObject();
        s->m_AppCaptionFont.CreateFontIndirect(&ncm.lfCaptionFont);
      }
      tmCaption.ReadColor(("ActiveStart"), s->m_clrAppCaptionActiveStart);
      tmCaption.ReadColor(("ActiveFinish"), s->m_clrAppCaptionActiveFinish);
      tmCaption.ReadColor(("InactiveStart"), s->m_clrAppCaptionInactiveStart);
      tmCaption.ReadColor(("InactiveFinish"), s->m_clrAppCaptionInactiveFinish);
      tmCaption.ReadColor(("ActiveText"), s->m_clrAppCaptionActiveText);
      tmCaption.ReadColor(("InactiveText"), s->m_clrAppCaptionInactiveText);
      tmCaption.ReadColor(("ActiveTitleText"), s->m_clrAppCaptionActiveTitleText);
      tmCaption.ReadColor(("InactiveTitleText"), s->m_clrAppCaptionInactiveTitleText);
      tmCaption.ReadBool(("TextCenter"), s->m_bNcTextCenter);
      tmCaption.ReadControlRenderer(("BORDER"), s->m_ctrlMainBorderCaption, MakeResourceID(("IDB_OFFICE2007_MAINBORDER_CAPTION")));
      s->m_szNcBtnSize[0] = CSize(::GetSystemMetrics(SM_CXSIZE),
          ::GetSystemMetrics(SM_CYSIZE));
      s->m_szNcBtnSize[1] = CSize(::GetSystemMetrics(SM_CXSMSIZE),
          ::GetSystemMetrics(SM_CYSMSIZE));
      // buttons
      CString strButtons;
      if (tmCaption.ExcludeTag(("BUTTONS"), strButtons)) {
        TagManager tmButtons(strButtons);
        for (int i = 0; i < 2; i++) {
          CString str;
          CString suffix;
          if (i == 1) {
            suffix = ("_S");
          }
          if (tmButtons.ExcludeTag(i == 0 ? ("NORMAL") : ("SMALL"), str)) {
            TagManager tmBtn(str);
            tmBtn.ReadSize(("ConstSize"), s->m_szNcBtnSize[i]);
            CSize sizeIcon(0, 0);
            if (tmBtn.ReadSize(("IconSize"), sizeIcon)) {
              s->m_SysBtnClose[i].Clear();
              s->m_SysBtnClose[i].SetPreMultiplyAutoCheck(TRUE);
              s->m_SysBtnClose[i].SetImageSize(sizeIcon);
              s->m_SysBtnClose[i].LoadStr(MakeResourceID(("IDB_OFFICE2007_SYS_BTN_CLOSE") + suffix));
              s->m_SysBtnRestore[i].Clear();
              s->m_SysBtnRestore[i].SetPreMultiplyAutoCheck(TRUE);
              s->m_SysBtnRestore[i].SetImageSize(sizeIcon);
              s->m_SysBtnRestore[i].LoadStr(MakeResourceID(("IDB_OFFICE2007_SYS_BTN_RESTORE") + suffix));
              s->m_SysBtnMaximize[i].Clear();
              s->m_SysBtnMaximize[i].SetPreMultiplyAutoCheck(TRUE);
              s->m_SysBtnMaximize[i].SetImageSize(sizeIcon);
              s->m_SysBtnMaximize[i].LoadStr(MakeResourceID(("IDB_OFFICE2007_SYS_BTN_MAXIMIZE") + suffix));
              s->m_SysBtnMinimize[i].Clear();
              s->m_SysBtnMinimize[i].SetPreMultiplyAutoCheck(TRUE);
              s->m_SysBtnMinimize[i].SetImageSize(sizeIcon);
              s->m_SysBtnMinimize[i].LoadStr(MakeResourceID(("IDB_OFFICE2007_SYS_BTN_MINIMIZE") + suffix));
              s->m_SysBtnHelp[i].Clear();
              s->m_SysBtnHelp[i].SetPreMultiplyAutoCheck(TRUE);
              s->m_SysBtnHelp[i].SetImageSize(sizeIcon);
              s->m_SysBtnHelp[i].LoadStr(MakeResourceID(("IDB_OFFICE2007_SYS_BTN_HELP") + suffix));
            }
            TagManager::ParseControlRenderer(tmBtn.GetBuffer(),
                s->m_SysBtnBack[i], MakeResourceID(("IDB_OFFICE2007_SYS_BTN_BACK")));
          }
        }
      }
    }
    // border
    tmItem.ReadControlRenderer(("BORDER"), s->m_ctrlMainBorder, MakeResourceID(("IDB_OFFICE2007_MAINBORDER")));
    tmItem.ReadControlRenderer(("BORDER_MDICHILD"), s->m_ctrlMDIChildBorder, MakeResourceID(("IDB_OFFICE2007_MDICHILDBORDER")));
    tmItem.ReadControlRenderer(("BORDER_DIALOG"), s->m_ctrlDialogBorder, MakeResourceID(("IDB_OFFICE2007_DIALOGBORDER")));
    if (tmItem.ReadColor(("MainClientArea"), s->m_clrMainClientArea)) {
      s->m_clrMainClientArea.DeleteObject();
      s->m_clrMainClientArea.CreateSolidBrush(s->m_clrMainClientArea);
    }
  }
  // menu
  if (tm.ExcludeTag(("MENU"), strItem)) {
    TagManager tmItem(strItem);
    if (tmItem.ReadColor(("Light"), s->m_clrMenuLight)) {
      s->m_clrMenuLight.DeleteObject();
      s->m_clrMenuLight.CreateSolidBrush(s->m_clrMenuLight);
    }
    s->m_clrMenuRarelyUsed = CLR_DEFAULT;
    tmItem.ReadColor(("Rarely"), s->m_clrMenuRarelyUsed);
    tmItem.ReadColor(("Border"), s->m_clrMenuBorder);
    if (tmItem.ReadColor(("Separator1"), s->m_clrSeparator1)) {
      s->m_penSeparator.DeleteObject();
      s->m_penSeparator.CreatePen(PS_SOLID, 1, s->m_clrSeparator1);
    }
    if (tmItem.ReadColor(("Separator2"), s->m_clrSeparator2)) {
      s->m_penSeparator2.DeleteObject();
      s->m_penSeparator2.CreatePen(PS_SOLID, 1, s->m_clrSeparator2);
    }
    COLOR clrGroupBack = (COLOR) - 1;
    if (tmItem.ReadColor(("GroupBackground"), clrGroupBack)) {
      s->m_clrGroupBackground.DeleteObject();
      s->m_clrGroupBackground.CreateSolidBrush(clrGroupBack);
    }
    tmItem.ReadColor(("GroupText"), s->m_clrGroupText);
    if (tmItem.ReadColor(("ItemBorder"), s->m_clrMenuItemBorder)) {
      s->m_penMenuItemBorder.DeleteObject();
      s->m_penMenuItemBorder.CreatePen(PS_SOLID, 1, s->m_clrMenuItemBorder);
    }
    tmItem.ReadInt(("BorderSize"), s->m_nMenuBorderSize);
    tmItem.ReadControlRenderer(("ItemBack"), s->m_ctrlMenuItemBack, MakeResourceID(("IDB_OFFICE2007_MENU_ITEs->m_BACK")));
    tmItem.ReadToolBarImages(("ItemCheck"), s->m_MenuItemMarkerC, MakeResourceID(("IDB_OFFICE2007_MENU_ITEs->m_MARKER_C")));
    tmItem.ReadToolBarImages(("ItemRadio"), s->m_MenuItemMarkerR, MakeResourceID(("IDB_OFFICE2007_MENU_ITEs->m_MARKER_R")));
    tmItem.ReadControlRenderer(("ItemShowAll"), s->m_ctrlMenuItemShowAll, MakeResourceID(("IDB_OFFICE2007_MENU_ITEs->m_SHOWALL")));
    tmItem.ReadControlRenderer(("Highlighted"), s->m_ctrlMenuHighlighted[0], MakeResourceID(("IDB_OFFICE2007_MENU_BTN")));
    tmItem.ReadControlRenderer(("HighlightedDisabled"), s->m_ctrlMenuHighlighted[1], MakeResourceID(("IDB_OFFICE2007_MENU_BTN_DISABLED")));
    tmItem.ReadControlRenderer(("ButtonBorder"), s->m_ctrlMenuButtonBorder, MakeResourceID(("IDB_OFFICE2007_MENU_BTN_VERT_SEPARATOR")));
    tmItem.ReadControlRenderer(("ScrollBtn_T"), s->m_ctrlMenuScrollBtn[0], MakeResourceID(("IDB_OFFICE2007_MENU_BTN_SCROLL_T")));
    tmItem.ReadControlRenderer(("ScrollBtn_B"), s->m_ctrlMenuScrollBtn[1], MakeResourceID(("IDB_OFFICE2007_MENU_BTN_SCROLL_B")));
    tmItem.ReadColor(("TextNormal"), s->m_clrMenuText);
    tmItem.ReadColor(("TextHighlighted"), s->m_clrMenuTextHighlighted);
    tmItem.ReadColor(("TextDisabled"), s->m_clrMenuTextDisabled);
    COLOR clrImages = s->m_clrMenuText;
    CString strColors;
    if (tmItem.ExcludeTag(("COLORS"), strColors)) {
      TagManager tmColors(strColors);
      tmColors.ReadColor(("Black"), clrImages);
      MenuImages::SetColor(MenuImages::ImageBlack, clrImages);
      tmColors.ReadColor(("Black2"), clrImages);
      MenuImages::SetColor(MenuImages::ImageBlack2, clrImages);
      struct XColors {
        MenuImages::IMAGE_STATE state;
        LPCTSTR name;
      };
      XColors colors[4] = {
        {MenuImages::ImageGray, ("Gray")},
        {MenuImages::ImageLtGray, ("LtGray")},
        {MenuImages::ImageWhite, ("White")},
        {MenuImages::ImageDkGray, ("DkGray")}
      };
      for (int ic = 0; ic < 4; ic++) {
        if (tmColors.ReadColor(colors[ic].name, clrImages)) {
          MenuImages::SetColor(colors[ic].state, clrImages);
        }
      }
    }
    else {
      tmItem.ReadColor(("ImagesColor"), clrImages);
      MenuImages::SetColor(MenuImages::ImageBlack, clrImages);
      MenuImages::SetColor(MenuImages::ImageBlack2, clrImages);
    }
    // TODO:
    //MenuImages::SetColor(MenuImages::ImageBlack2, _RGB(21, 66, 139));
  }
  // bars
  if (tm.ExcludeTag(("BARS"), strItem)) {
    TagManager tmItem(strItem);
    CString strBar;
    if (tmItem.ExcludeTag(("DEFAULT"), strBar)) {
      TagManager tmBar(strBar);
      if (tmBar.ReadColor(("Bkgnd"), s->m_clrBarBkgnd)) {
        s->m_clrBarBkgnd.DeleteObject();
        s->m_clrBarBkgnd.CreateSolidBrush(s->m_clrBarBkgnd);
      }
      tmBar.ReadColor(("GradientLight"), s->m_clrBarGradientLight);
      s->m_clrBarGradientDark = s->m_clrBarGradientLight;
      tmBar.ReadColor(("GradientDark"), s->m_clrBarGradientDark);
    }
    if (tmItem.ExcludeTag(("TOOLBAR"), strBar)) {
      TagManager tmBar(strBar);
      s->m_clrToolBarGradientLight = s->m_clrBarGradientLight;
      s->m_clrToolBarGradientDark = s->m_clrBarGradientDark;
      s->m_clrToolbarDisabled = DrawManager::SmartMixColors(
          s->m_clrToolBarGradientDark, s->m_clrToolBarGradientLight);
      tmBar.ReadColor(("GradientLight"), s->m_clrToolBarGradientLight);
      tmBar.ReadColor(("GradientDark"), s->m_clrToolBarGradientDark);
      s->m_clrToolBarGradientVertLight = s->m_clrToolBarGradientLight;
      s->m_clrToolBarGradientVertDark = s->m_clrToolBarGradientDark;
      tmBar.ReadColor(("GradientVertLight"), s->m_clrToolBarGradientVertLight);
      tmBar.ReadColor(("GradientVertDark"), s->m_clrToolBarGradientVertDark);
      tmBar.ReadColor(("CustomizeButtonGradientLight"), s->m_clrCustomizeButtonGradientLight);
      tmBar.ReadColor(("CustomizeButtonGradientDark"), s->m_clrCustomizeButtonGradientDark);
      tmBar.ReadToolBarImages(("GRIPPER"), s->m_ToolBarGripper, MakeResourceID(("IDB_OFFICE2007_GRIPPER")));
      tmBar.ReadToolBarImages(("TEAR"), s->m_ToolBarTear, MakeResourceID(("IDB_OFFICE2007_TEAR")));
      tmBar.ReadControlRenderer(("BUTTON"), s->m_ctrlToolBarBtn, MakeResourceID(("IDB_OFFICE2007_TOOLBAR_BTN")));
      tmBar.ReadControlRenderer(("BORDER"), s->m_ctrlToolBarBorder, MakeResourceID(("IDB_OFFICE2007_TOOLBAR_BORDER")));
      s->m_clrToolBarBtnText = s->clrBarText;
      s->m_clrToolBarBtnTextHighlighted = s->m_clrToolBarBtnText;
      tmBar.ReadColor(("TextNormal"), s->m_clrToolBarBtnText);
      tmBar.ReadColor(("TextHighlighted"), s->m_clrToolBarBtnTextHighlighted);
      tmBar.ReadColor(("TextDisabled"), s->m_clrToolBarBtnTextDisabled);
      if (tmBar.ReadColor(("BottomLineColor"), s->m_clrToolBarBottomLine)) {
        s->m_penBottomLine.DeleteObject();
        s->m_penBottomLine.CreatePen(PS_SOLID, 1, s->m_clrToolBarBottomLine);
      }
      s->m_penSeparatorDark.DeleteObject();
      s->m_penSeparatorDark.CreatePen(PS_SOLID, 1,
          PixelAlpha2(s->m_clrToolBarBottomLine, _RGB(255, 255, 255), 95));
      s->m_penSeparatorLight.DeleteObject();
      s->m_penSeparatorLight.CreatePen(PS_SOLID, 1, _RGB(255, 255, 255));
    }
    if (tmItem.ExcludeTag(("MENUBAR"), strBar)) {
      TagManager tmBar(strBar);
      s->m_clrMenuBarGradientLight = s->m_clrToolBarGradientLight;
      s->m_clrMenuBarGradientDark = s->m_clrToolBarGradientDark;
      tmBar.ReadColor(("GradientLight"), s->m_clrMenuBarGradientLight);
      tmBar.ReadColor(("GradientDark"), s->m_clrMenuBarGradientDark);
      s->m_clrMenuBarGradientVertLight = s->m_clrMenuBarGradientLight;
      s->m_clrMenuBarGradientVertDark = s->m_clrMenuBarGradientDark;
      tmBar.ReadColor(("GradientVertLight"), s->m_clrMenuBarGradientVertLight);
      tmBar.ReadColor(("GradientVertDark"), s->m_clrMenuBarGradientVertDark);
      s->m_clrMenuBarBtnText = s->m_clrToolBarBtnText;
      s->m_clrMenuBarBtnTextHighlighted = s->m_clrToolBarBtnTextHighlighted;
      s->m_clrMenuBarBtnTextDisabled = s->m_clrToolBarBtnTextDisabled;
      tmBar.ReadColor(("TextNormal"), s->m_clrMenuBarBtnText);
      tmBar.ReadColor(("TextHighlighted"), s->m_clrMenuBarBtnTextHighlighted);
      tmBar.ReadColor(("TextDisabled"), s->m_clrMenuBarBtnTextDisabled);
      tmBar.ReadControlRenderer(("BUTTON"), s->m_ctrlMenuBarBtn, MakeResourceID(("IDB_OFFICE2007_MENUBAR_BTN")));
    }
    if (tmItem.ExcludeTag(("POPUPBAR"), strBar)) {
      TagManager tmBar(strBar);
      tmBar.ReadControlRenderer(("BORDER"), s->m_ctrlPopupBorder, MakeResourceID(("IDB_OFFICE2007_POPUPMENU_BORDER")));
      CString strResize;
      if (tmBar.ExcludeTag(("RESIZEBAR"), strResize)) {
        TagManager tmResize(strResize);
        tmResize.ReadControlRenderer(("BACK"), s->m_ctrlPopupResizeBar, MakeResourceID(("IDB_OFFICE2007_POPUPMENU_RESIZEBAR")));
        tmResize.ReadToolBarImages(("ICON_HV"), s->m_PopupResizeBar_HV, MakeResourceID(("IDB_OFFICE2007_POPUPMENU_RESIZEBAR_ICON_HV")));
        tmResize.ReadToolBarImages(("ICON_HVT"), s->m_PopupResizeBar_HVT, MakeResourceID(("IDB_OFFICE2007_POPUPMENU_RESIZEBAR_ICON_HVT")));
        tmResize.ReadToolBarImages(("ICON_V"), s->m_PopupResizeBar_V, MakeResourceID(("IDB_OFFICE2007_POPUPMENU_RESIZEBAR_ICON_V")));
      }
    }
    if (tmItem.ExcludeTag(("STATUSBAR"), strBar)) {
      TagManager tmBar(strBar);
      tmBar.ReadControlRenderer(("BACK"), s->m_ctrlStatusBarBack, MakeResourceID(("IDB_OFFICE2007_STATUSBAR_BACK")));
      tmBar.ReadControlRenderer(("BACK_EXT"), s->m_ctrlStatusBarBack_Ext, MakeResourceID(("IDB_OFFICE2007_STATUSBAR_BACK_EXT")));
      tmBar.ReadToolBarImages(("PANEBORDER"), s->m_StatusBarPaneBorder, MakeResourceID(("IDB_OFFICE2007_STATUSBAR_PANEBORDER")));
      tmBar.ReadToolBarImages(("SIZEBOX"), s->m_StatusBarSizeBox, MakeResourceID(("IDB_OFFICE2007_STATUSBAR_SIZEBOX")));
      s->m_clrStatusBarText = s->m_clrMenuBarBtnText;
      s->m_clrStatusBarTextDisabled = s->m_clrMenuBarBtnTextDisabled;
      s->m_clrExtenedStatusBarTextDisabled = s->m_clrMenuBarBtnTextDisabled;
      tmBar.ReadColor(("TextNormal"), s->m_clrStatusBarText);
      tmBar.ReadColor(("TextDisabled"), s->m_clrStatusBarTextDisabled);
      tmBar.ReadColor(("TextExtendedDisabled"), s->m_clrExtenedStatusBarTextDisabled);
    }
    if (tmItem.ExcludeTag(("CAPTIONBAR"), strBar)) {
      TagManager tmBar(strBar);
      tmBar.ReadColor(("GradientLight"), s->m_clrCaptionBarGradientLight);
      tmBar.ReadColor(("GradientDark"), s->m_clrCaptionBarGradientDark);
      tmBar.ReadColor(("TextNormal"), s->m_clrCaptionBarText);
    }
    if (tmItem.ExcludeTag(("SCROLLBAR"), strBar)) {
      TagManager tmBar(strBar);
      LPCTSTR szSBName [] = {("HORZ"), ("VERT")};
      CString strSB;
      for (int i = 0; i < 2; i++) {
        if (tmBar.ExcludeTag(szSBName[i], strSB)) {
          TagManager tmSB(strSB);
          CString strName(("IDB_OFFICE2007_STATUSBAR_"));
          strName += szSBName[i];
          tmSB.ReadControlRenderer(("BACK_1"), s->m_ctrlScrollBar_Back[i][0], MakeResourceID(strName + ("_BACK_1")));
          tmSB.ReadControlRenderer(("ITEs->m_1"), s->m_ctrlScrollBar_Item[i][0], MakeResourceID(strName + ("_ITEs->m_1")));
          tmSB.ReadControlRenderer(("THUMB_BACK_1"), s->m_ctrlScrollBar_ThumbBack[i][0], MakeResourceID(strName + ("_THUMB_BACK_1")));
          tmSB.ReadControlRenderer(("THUMB_ICON_1"), s->m_ctrlScrollBar_ThumbIcon[i][0], MakeResourceID(strName + ("_THUMB_ICON_1")));
          tmSB.ReadControlRenderer(("BACK_2"), s->m_ctrlScrollBar_Back[i][1], MakeResourceID(strName + ("_BACK_2")));
          tmSB.ReadControlRenderer(("ITEs->m_2"), s->m_ctrlScrollBar_Item[i][1], MakeResourceID(strName + ("_ITEs->m_2")));
          tmSB.ReadControlRenderer(("THUMB_BACK_2"), s->m_ctrlScrollBar_ThumbBack[i][1], MakeResourceID(strName + ("_THUMB_BACK_2")));
          tmSB.ReadControlRenderer(("THUMB_ICON_2"), s->m_ctrlScrollBar_ThumbIcon[i][1], MakeResourceID(strName + ("_THUMB_ICON_2")));
        }
      }
    }
  }
  if (s->m_clrMenuRarelyUsed == CLR_DEFAULT) {
    s->m_clrMenuRarelyUsed = s->m_clrBarBkgnd;
  }
  s->m_clrMenuRarelyUsed.DeleteObject();
  s->m_clrMenuRarelyUsed.CreateSolidBrush(s->m_clrMenuRarelyUsed);
  s->m_clrEditBorder = s->clrWindow;
  s->m_clrEditBorderDisabled = s->clrBtnShadow;
  s->m_clrEditBorderHighlighted = s->m_clrMenuItemBorder;
  s->m_clrEditSelection = s->clrHilite;
  // edit
  if (tm.ExcludeTag(("EDIT"), strItem)) {
    TagManager tmItem(strItem);
    tmItem.ReadColor(("BorderNormal"), s->m_clrEditBorder);
    tmItem.ReadColor(("BorderHighlighted"), s->m_clrEditBorderHighlighted);
    tmItem.ReadColor(("BorderDisabled"), s->m_clrEditBorderDisabled);
    tmItem.ReadColor(("Selection"), s->m_clrEditSelection);
  }
  s->m_clrComboBorder = s->clrWindow;
  s->m_clrComboBorderDisabled = s->clrBtnShadow;
  s->m_clrComboBorderHighlighted = s->m_clrMenuItemBorder;
  s->m_clrComboBorderPressed = s->m_clrComboBorderHighlighted;
  s->m_clrComboBtnBorder = s->m_clrComboBorder;
  s->m_clrComboBtnBorderHighlighted = s->m_clrComboBorderHighlighted;
  s->m_clrComboBtnBorderPressed = s->m_clrComboBorderHighlighted;
  s->m_clrComboSelection = s->clrHilite;
  s->m_clrComboBtnStart = s->m_clrToolBarGradientDark;
  s->m_clrComboBtnFinish = s->m_clrToolBarGradientLight;
  s->m_clrComboBtnDisabledStart = s->clrBtnFace;
  s->m_clrComboBtnDisabledFinish = s->m_clrComboBtnDisabledStart;
  s->m_clrComboBtnHighlightedStart = s->m_clrHighlightGradientDark;
  s->m_clrComboBtnHighlightedFinish = s->m_clrHighlightGradientLight;
  s->m_clrComboBtnPressedStart = s->m_clrHighlightDnGradientDark;
  s->m_clrComboBtnPressedFinish = s->m_clrHighlightDnGradientLight;
  // combobox
  if (tm.ExcludeTag(("COMBO"), strItem)) {
    TagManager tmItem(strItem);
    tmItem.ReadColor(("BorderNormal"), s->m_clrComboBorder);
    tmItem.ReadColor(("BorderHighlighted"), s->m_clrComboBorderHighlighted);
    tmItem.ReadColor(("BorderDisabled"), s->m_clrComboBorderDisabled);
    s->m_clrComboBorderPressed = s->m_clrComboBorderHighlighted;
    tmItem.ReadColor(("BorderPressed"), s->m_clrComboBorderPressed);
    tmItem.ReadColor(("Selection"), s->m_clrComboSelection);
    CString strButton;
    if (tmItem.ExcludeTag(("BUTTON"), strButton)) {
      TagManager tmButton(strButton);
      tmButton.ReadColor(("GradientStartNormal"), s->m_clrComboBtnStart);
      tmButton.ReadColor(("GradientFinishNormal"), s->m_clrComboBtnFinish);
      tmButton.ReadColor(("BtnBorderNormal"), s->m_clrComboBtnBorder);
      if (!tmButton.ReadControlRenderer(("IMAGE"), s->m_ctrlComboBoxBtn, MakeResourceID(("IDB_OFFICE2007_COMBOBOX_BTN")))) {
        tmButton.ReadColor(("GradientStartHighlighted"), s->m_clrComboBtnHighlightedStart);
        tmButton.ReadColor(("GradientFinishHighlighted"), s->m_clrComboBtnHighlightedFinish);
        tmButton.ReadColor(("GradientStartDisabled"), s->m_clrComboBtnDisabledStart);
        tmButton.ReadColor(("GradientFinishDisabled"), s->m_clrComboBtnDisabledFinish);
        tmButton.ReadColor(("GradientStartPressed"), s->m_clrComboBtnPressedStart);
        tmButton.ReadColor(("GradientFinishPressed"), s->m_clrComboBtnPressedFinish);
        tmButton.ReadColor(("BtnBorderHighlighted"), s->m_clrComboBtnBorderHighlighted);
        tmButton.ReadColor(("BtnBorderDisabled"), s->m_clrComboBtnBorderDisabled);
        s->m_clrComboBtnBorderPressed = s->m_clrComboBtnBorderHighlighted;
        tmButton.ReadColor(("BtnBorderPressed"), s->m_clrComboBtnBorderPressed);
      }
    }
  }
  s->m_clrRibbonEditBorder = s->m_clrEditBorder;
  s->m_clrRibbonEditBorderDisabled = s->m_clrEditBorderDisabled;
  s->m_clrRibbonEditBorderHighlighted = s->m_clrEditBorderHighlighted;
  s->m_clrRibbonEditBorderPressed = s->m_clrRibbonEditBorderHighlighted;
  s->m_clrRibbonEditSelection = s->m_clrEditSelection;
  s->m_clrRibbonComboBtnBorder = s->m_clrComboBtnBorder;
  s->m_clrRibbonComboBtnBorderHighlighted = s->m_clrComboBtnBorderHighlighted;
  s->m_clrRibbonComboBtnBorderPressed = s->m_clrComboBtnBorderPressed;
  s->m_clrRibbonComboBtnStart = s->m_clrComboBtnStart;
  s->m_clrRibbonComboBtnFinish = s->m_clrComboBtnFinish;
  s->m_clrRibbonComboBtnDisabledStart = s->m_clrComboBtnDisabledStart;
  s->m_clrRibbonComboBtnDisabledFinish = s->m_clrComboBtnDisabledFinish;
  s->m_clrRibbonComboBtnHighlightedStart = s->m_clrComboBtnHighlightedStart;
  s->m_clrRibbonComboBtnHighlightedFinish = s->m_clrComboBtnHighlightedFinish;
  s->m_clrRibbonComboBtnPressedStart = s->m_clrComboBtnPressedStart;
  s->m_clrRibbonComboBtnPressedFinish = s->m_clrComboBtnPressedFinish;
  // task pane
#ifndef BCGP_EXCLUDE_TASK_PANE
  s->m_clrTaskPaneGradientDark = s->m_clrBarGradientLight;
  s->m_clrTaskPaneGradientLight = s->m_clrTaskPaneGradientDark;
  if (tm.ExcludeTag(("TASK"), strItem)) {
    TagManager tmItem(strItem);
    tmItem.ReadColor(("GradientDark"), s->m_clrTaskPaneGradientDark);
    tmItem.ReadColor(("GradientLight"), s->m_clrTaskPaneGradientLight);
    CString strGroup;
    if (tmItem.ExcludeTag(("GROUP"), strGroup)) {
      TagManager tmGroup(strGroup);
      CString strState;
      if (tmGroup.ExcludeTag(("NORMAL"), strState)) {
        TagManager tmState(strState);
        CString str;
        if (tmState.ExcludeTag(("CAPTION"), str)) {
          TagManager tmCaption(str);
          tmCaption.ReadColor(("DarkNormal"), s->m_clrTaskPaneGroupCaptionDark);
          tmCaption.ReadColor(("LightNormal"), s->m_clrTaskPaneGroupCaptionLight);
          tmCaption.ReadColor(("DarkHighlighted"), s->m_clrTaskPaneGroupCaptionHighDark);
          tmCaption.ReadColor(("LightHighlighted"), s->m_clrTaskPaneGroupCaptionHighLight);
          tmCaption.ReadColor(("TextNormal"), s->m_clrTaskPaneGroupCaptionText);
          tmCaption.ReadColor(("TextHighlighted"), s->m_clrTaskPaneGroupCaptionTextHigh);
        }
        if (tmState.ExcludeTag(("AREA"), str)) {
          TagManager tmArea(str);
          tmArea.ReadColor(("DarkNormal"), s->m_clrTaskPaneGroupAreaDark);
          tmArea.ReadColor(("LightNormal"), s->m_clrTaskPaneGroupAreaLight);
        }
      }
      if (tmGroup.ExcludeTag(("SPECIAL"), strState)) {
        TagManager tmState(strState);
        CString str;
        if (tmState.ExcludeTag(("CAPTION"), str)) {
          TagManager tmCaption(str);
          tmCaption.ReadColor(("DarkNormal"), s->m_clrTaskPaneGroupCaptionSpecDark);
          tmCaption.ReadColor(("LightNormal"), s->m_clrTaskPaneGroupCaptionSpecLight);
          tmCaption.ReadColor(("DarkHighlighted"), s->m_clrTaskPaneGroupCaptionHighSpecDark);
          tmCaption.ReadColor(("LightHighlighted"), s->m_clrTaskPaneGroupCaptionHighSpecLight);
          tmCaption.ReadColor(("TextNormal"), s->m_clrTaskPaneGroupCaptionTextSpec);
          tmCaption.ReadColor(("TextHighlighted"), s->m_clrTaskPaneGroupCaptionTextHighSpec);
        }
        if (tmState.ExcludeTag(("AREA"), str)) {
          TagManager tmArea(str);
          tmArea.ReadColor(("DarkNormal"), s->m_clrTaskPaneGroupAreaSpecDark);
          tmArea.ReadColor(("LightNormal"), s->m_clrTaskPaneGroupAreaSpecLight);
        }
      }
      if (tmGroup.ReadColor(("BORDER"), s->m_clrTaskPaneGroupBorder)) {
        s->m_penTaskPaneGroupBorder.DeleteObject();
        s->m_penTaskPaneGroupBorder.CreatePen(PS_SOLID, 1, s->m_clrTaskPaneGroupBorder);
      }
    }
    tmItem.ReadControlRenderer(("SCROLL_BUTTON"), s->m_ctrlTaskScrollBtn, MakeResourceID(("IDB_OFFICE2007_TASKPANE_SCROLL_BTN")));
  }
#endif
  if (tm.ExcludeTag(("TABS"), strItem)) {
    TagManager tmItem(strItem);
    tmItem.ReadColor(("TextColorActive"), s->m_clrTabTextActive);
    tmItem.ReadColor(("TextColorInactive"), s->m_clrTabTextInactive);
    CString strTab;
    if (tmItem.ExcludeTag(("3D"), strTab)) {
      TagManager tmTab(strTab);
      CString strBtn;
      if (tmTab.ExcludeTag(("BUTTON"), strBtn)) {
        ControlRendererParams params(MakeResourceID(("IDB_OFFICE2007_TAB_3D")), IRECT(0, 0, 0, 0), IRECT(0, 0, 0, 0));
        if (TagManager::ParseControlRendererParams(strBtn, params)) {
          s->m_ctrlTab3D[0].Create(params);
          s->m_ctrlTab3D[1].Create(params, TRUE);
        }
      }
      tmTab.ReadColor(("Face"), s->m_clrTab3DFace);
      tmTab.ReadColor(("Black"), s->m_clrTab3DBlack);
      tmTab.ReadColor(("Dark"), s->m_clrTab3DDark);
      tmTab.ReadColor(("DarkShadow"), s->m_clrTab3DDarkShadow);
      tmTab.ReadColor(("Light"), s->m_clrTab3DLight);
      tmTab.ReadColor(("Highlight"), s->m_clrTab3DHighlight);
    }
    if (tmItem.ExcludeTag(("FLAT"), strTab)) {
      TagManager tmTab(strTab);
      CString strBtn;
      if (tmTab.ExcludeTag(("BUTTON"), strBtn)) {
        ControlRendererParams params(MakeResourceID(("IDB_OFFICE2007_TAB_FLAT")), IRECT(0, 0, 0, 0), IRECT(0, 0, 0, 0));
        if (TagManager::ParseControlRendererParams(strBtn, params)) {
          s->m_ctrlTabFlat[0].Create(params);
          s->m_ctrlTabFlat[1].Create(params, TRUE);
        }
      }
      tmTab.ReadColor(("Face"), s->m_clrTabFlatFace);
      tmTab.ReadColor(("Black"), s->m_clrTabFlatBlack);
      tmTab.ReadColor(("Dark"), s->m_clrTabFlatDark);
      tmTab.ReadColor(("DarkShadow"), s->m_clrTabFlatDarkShadow);
      tmTab.ReadColor(("Light"), s->m_clrTabFlatLight);
      tmTab.ReadColor(("Highlight"), s->m_clrTabFlatHighlight);
      COLOR clr;
      if (tmTab.ReadColor(("BorderInnerNormal"), clr)) {
        s->m_penTabFlatInner[0].DeleteObject();
        s->m_penTabFlatInner[0].CreatePen(PS_SOLID, 1, clr);
      }
      if (tmTab.ReadColor(("BorderInnerActive"), clr)) {
        s->m_penTabFlatInner[1].DeleteObject();
        s->m_penTabFlatInner[1].CreatePen(PS_SOLID, 1, clr);
      }
      if (tmTab.ReadColor(("BorderOuterNormal"), clr)) {
        s->m_penTabFlatOuter[0].DeleteObject();
        s->m_penTabFlatOuter[0].CreatePen(PS_SOLID, 1, clr);
      }
      if (tmTab.ReadColor(("BorderOuterActive"), clr)) {
        s->m_penTabFlatOuter[1].DeleteObject();
        s->m_penTabFlatOuter[1].CreatePen(PS_SOLID, 1, clr);
      }
    }
  }
  if (tm.ExcludeTag(("HEADER"), strItem)) {
    TagManager tmItem(strItem);
    tmItem.ReadColor(("NormalStart"), s->m_clrHeaderNormalStart);
    tmItem.ReadColor(("NormalFinish"), s->m_clrHeaderNormalFinish);
    tmItem.ReadColor(("NormalBorder"), s->m_clrHeaderNormalBorder);
    tmItem.ReadColor(("HighlightedStart"), s->m_clrHeaderHighlightedStart);
    tmItem.ReadColor(("HighlightedFinish"), s->m_clrHeaderHighlightedFinish);
    tmItem.ReadColor(("HighlightedBorder"), s->m_clrHeaderHighlightedBorder);
    tmItem.ReadColor(("PressedStart"), s->m_clrHeaderPressedStart);
    tmItem.ReadColor(("PressedFinish"), s->m_clrHeaderPressedFinish);
    tmItem.ReadColor(("PressedBorder"), s->m_clrHeaderPressedBorder);
  }
  s->m_clrRibbonCategoryText = s->m_clrMenuBarBtnText;
  s->m_clrRibbonCategoryTextHighlighted = s->m_clrMenuBarBtnTextHighlighted;
  s->m_clrRibbonCategoryTextDisabled = s->m_clrMenuBarBtnTextDisabled;
  s->m_clrRibbonPanelText = s->m_clrToolBarBtnText;
  s->m_clrRibbonPanelTextHighlighted = s->m_clrToolBarBtnTextHighlighted;
  s->m_clrRibbonPanelCaptionText = s->m_clrRibbonPanelText;
  s->m_clrRibbonPanelCaptionTextHighlighted = s->m_clrRibbonPanelTextHighlighted;
  s->m_clrRibbonEdit = s->clrBarLight;
  s->m_clrRibbonEditHighlighted = s->clrWindow;
  s->m_clrRibbonEditPressed = s->m_clrRibbonEditHighlighted;
  s->m_clrRibbonEditDisabled = s->clrBtnFace;
  if (tm.ExcludeTag(("RIBBON"), strItem)) {
    TagManager tmItem(strItem);
    CString str;
    if (tmItem.ExcludeTag(("CATEGORY"), str)) {
      TagManager tmCategory(str);
      tmCategory.ReadControlRenderer(("BACK"), s->m_ctrlRibbonCategoryBack, MakeResourceID(("IDB_OFFICE2007_RIBBON_CATEGORY_BACK")));
      CString strTab;
      if (tmCategory.ExcludeTag(("TAB"), strTab)) {
        TagManager tmTab(strTab);
        tmTab.ReadControlRenderer(("BUTTON"), s->m_ctrlRibbonCategoryTab, MakeResourceID(("IDB_OFFICE2007_RIBBON_CATEGORY_TAB")));
        tmTab.ReadColor(("TextNormal"), s->m_clrRibbonCategoryText);
        tmTab.ReadColor(("TextHighlighted"), s->m_clrRibbonCategoryTextHighlighted);
        tmTab.ReadColor(("TextDisabled"), s->m_clrRibbonCategoryTextDisabled);
      }
      tmCategory.ReadControlRenderer(("TAB_SEPARATOR"), s->m_ctrlRibbonCategoryTabSep, MakeResourceID(("IDB_OFFICE2007_RIBBON_CATEGORY_TAB_SEP")));
      tmCategory.ReadControlRenderer(("BUTTON_PAGE_L"), s->m_ctrlRibbonCategoryBtnPage[0], MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_PAGE_L")));
      tmCategory.ReadControlRenderer(("BUTTON_PAGE_R"), s->m_ctrlRibbonCategoryBtnPage[1], MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_PAGE_R")));
    }
    if (tmItem.ExcludeTag(("PANEL"), str)) {
      TagManager tmPanel(str);
      {
        CString strBack;
        if (tmPanel.ExcludeTag(("BACK"), strBack)) {
          TagManager tmBack(strBack);
          tmBack.ReadControlRenderer(("TOP"), s->m_ctrlRibbonPanelBack_T, MakeResourceID(("IDB_OFFICE2007_RIBBON_PANEL_BACK_T")));
          tmBack.ReadControlRenderer(("BOTTOM"), s->m_ctrlRibbonPanelBack_B, MakeResourceID(("IDB_OFFICE2007_RIBBON_PANEL_BACK_B")));
        }
      }
      {
        CString strCaption;
        if (tmPanel.ExcludeTag(("CAPTION"), strCaption)) {
          TagManager tmCaption(strCaption);
          tmCaption.ReadControlRenderer(("LAUNCH_BTN"), s->m_ctrlRibbonBtnLaunch, MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_LAUNCH")));
          tmCaption.ReadToolBarImages(("LAUNCH_ICON"), s->m_RibbonBtnLaunchIcon, MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_LAUNCH_ICON")));
          tmCaption.ReadColor(("TextNormal"), s->m_clrRibbonPanelCaptionText);
          tmCaption.ReadColor(("TextHighlighted"), s->m_clrRibbonPanelCaptionTextHighlighted);
          s->m_RibbonBtnLaunchIcon.SmoothResize(s->GetRibbonImageScale());
        }
      }
      tmPanel.ReadToolBarImages(("SEPARATOR"), s->m_RibbonPanelSeparator, MakeResourceID(("IDB_OFFICE2007_RIBBON_PANEL_SEPARATOR")));
      tmPanel.ReadControlRenderer(("QAT"), s->m_ctrlRibbonPanelQAT, MakeResourceID(("IDB_OFFICE2007_RIBBON_PANEL_QAT")));
      {
        CString strButtons;
        if (tmPanel.ExcludeTag(("BUTTONS"), strButtons)) {
          TagManager tmButtons(strButtons);
          tmButtons.ReadControlRenderer(("BUTTON_GROUP_F"), s->m_ctrlRibbonBtnGroup_F, MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_GROUP_F")));
          tmButtons.ReadControlRenderer(("BUTTON_GROUP_M"), s->m_ctrlRibbonBtnGroup_M, MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_GROUP_M")));
          tmButtons.ReadControlRenderer(("BUTTON_GROUP_L"), s->m_ctrlRibbonBtnGroup_L, MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_GROUP_L")));
          tmButtons.ReadControlRenderer(("BUTTON_GROUP_S"), s->m_ctrlRibbonBtnGroup_S, MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_GROUP_S")));
          tmButtons.ReadControlRenderer(("BUTTON_GROUPMENU_F_C"), s->m_ctrlRibbonBtnGroupMenu_F[0], MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_GROUPMENU_F_C")));
          tmButtons.ReadControlRenderer(("BUTTON_GROUPMENU_F_M"), s->m_ctrlRibbonBtnGroupMenu_F[1], MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_GROUPMENU_F_M")));
          tmButtons.ReadControlRenderer(("BUTTON_GROUPMENU_s->m_C"), s->m_ctrlRibbonBtnGroupMenu_M[0], MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_GROUPMENU_s->m_C")));
          tmButtons.ReadControlRenderer(("BUTTON_GROUPMENU_s->m_M"), s->m_ctrlRibbonBtnGroupMenu_M[1], MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_GROUPMENU_s->m_M")));
          tmButtons.ReadControlRenderer(("BUTTON_GROUPMENU_L_C"), s->m_ctrlRibbonBtnGroupMenu_L[0], MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_GROUPMENU_L_C")));
          tmButtons.ReadControlRenderer(("BUTTON_GROUPMENU_L_M"), s->m_ctrlRibbonBtnGroupMenu_L[1], MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_GROUPMENU_L_M")));
          tmButtons.ReadControlRenderer(("BUTTON_NORMAL_S"), s->m_ctrlRibbonBtn[0], MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_NORMAL_S")));
          tmButtons.ReadControlRenderer(("BUTTON_NORMAL_B"), s->m_ctrlRibbonBtn[1], MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_NORMAL_B")));
          tmButtons.ReadControlRenderer(("BUTTON_DEFAULT"), s->m_ctrlRibbonBtnDefault, MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_DEFAULT")));
          tmButtons.ReadControlRenderer(("BUTTON_DEFAULT_ICON"), s->m_ctrlRibbonBtnDefaultIcon, MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_DEFAULT_ICON")));
          tmButtons.ReadToolBarImages(("BUTTON_DEFAULT_IMAGE"), s->m_RibbonBtnDefaultImage, MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_DEFAULT_IMAGE")));
          tmButtons.ReadControlRenderer(("BUTTON_DEFAULT_QAT"), s->m_ctrlRibbonBtnDefaultQAT, MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_DEFAULT_QAT")));
          if (!s->m_ctrlRibbonBtnDefaultQAT.IsValid()) {
            tmButtons.ReadControlRenderer(("BUTTON_DEFAULT_QAT_ICON"), s->m_ctrlRibbonBtnDefaultQATIcon, MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_DEFAULT_QAT_ICON")));
          }
          tmButtons.ReadControlRenderer(("BUTTON_MENU_H_C"), s->m_ctrlRibbonBtnMenuH[0], MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_MENU_H_C")));
          tmButtons.ReadControlRenderer(("BUTTON_MENU_H_M"), s->m_ctrlRibbonBtnMenuH[1], MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_MENU_H_M")));
          tmButtons.ReadControlRenderer(("BUTTON_MENU_V_C"), s->m_ctrlRibbonBtnMenuV[0], MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_MENU_V_C")));
          tmButtons.ReadControlRenderer(("BUTTON_MENU_V_M"), s->m_ctrlRibbonBtnMenuV[1], MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_MENU_V_M")));
          tmButtons.ReadControlRenderer(("BUTTON_CHECK"), s->m_ctrlRibbonBtnCheck, MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_CHECK")));
          tmButtons.ReadControlRenderer(("BUTTON_RADIO"), s->m_ctrlRibbonBtnRadio, MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_RADIO")));
          tmButtons.ReadControlRenderer(("BUTTON_PUSH"), s->m_ctrlRibbonBtnPush, MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_PUSH")));
          tmButtons.ReadControlRenderer(("BUTTON_GROUP"), s->m_ctrlRibbonBtnGroup, MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_GROUP")));
          s->m_ctrlRibbonBtnCheck.SmoothResize(s->GetRibbonImageScale());
          s->m_ctrlRibbonBtnRadio.SmoothResize(s->GetRibbonImageScale());
          tmButtons.ReadControlRenderer(("BUTTON_PNL_T"), s->m_ctrlRibbonBtnPalette[0], MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_PALETTE_T")));
          tmButtons.ReadControlRenderer(("BUTTON_PNL_M"), s->m_ctrlRibbonBtnPalette[1], MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_PALETTE_M")));
          tmButtons.ReadControlRenderer(("BUTTON_PNL_B"), s->m_ctrlRibbonBtnPalette[2], MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_PALETTE_B")));
        }
      }
      {
        CString strEdit;
        if (tmPanel.ExcludeTag(("EDIT"), strEdit)) {
          TagManager tmEdit(strEdit);
          tmEdit.ReadColor(("Normal"), s->m_clrRibbonEdit);
          tmEdit.ReadColor(("Highlighted"), s->m_clrRibbonEditHighlighted);
          tmEdit.ReadColor(("Disabled"), s->m_clrRibbonEditDisabled);
          tmEdit.ReadColor(("Pressed"), s->m_clrRibbonEditPressed);
          tmEdit.ReadColor(("BorderNormal"), s->m_clrRibbonEditBorder);
          tmEdit.ReadColor(("BorderHighlighted"), s->m_clrRibbonEditBorderHighlighted);
          tmEdit.ReadColor(("BorderDisabled"), s->m_clrRibbonEditBorderDisabled);
          tmEdit.ReadColor(("BorderPressed"), s->m_clrRibbonEditBorderPressed);
          tmEdit.ReadColor(("Selection"), s->m_clrRibbonEditSelection);
          CString strButton;
          if (tmEdit.ExcludeTag(("BUTTON"), strButton)) {
            TagManager tmButton(strButton);
            tmButton.ReadColor(("GradientStartNormal"), s->m_clrRibbonComboBtnStart);
            tmButton.ReadColor(("GradientFinishNormal"), s->m_clrRibbonComboBtnFinish);
            tmButton.ReadColor(("BtnBorderNormal"), s->m_clrRibbonComboBtnBorder);
            if (!tmButton.ReadControlRenderer(("IMAGE"), s->m_ctrlRibbonComboBoxBtn, MakeResourceID(("IDB_OFFICE2007_COMBOBOX_BTN")))) {
              tmButton.ReadColor(("GradientStartHighlighted"), s->m_clrRibbonComboBtnHighlightedStart);
              tmButton.ReadColor(("GradientFinishHighlighted"), s->m_clrRibbonComboBtnHighlightedFinish);
              tmButton.ReadColor(("GradientStartDisabled"), s->m_clrRibbonComboBtnDisabledStart);
              tmButton.ReadColor(("GradientFinishDisabled"), s->m_clrRibbonComboBtnDisabledFinish);
              tmButton.ReadColor(("GradientStartPressed"), s->m_clrRibbonComboBtnPressedStart);
              tmButton.ReadColor(("GradientFinishPressed"), s->m_clrRibbonComboBtnPressedFinish);
              tmButton.ReadColor(("BtnBorderHighlighted"), s->m_clrRibbonComboBtnBorderHighlighted);
              tmButton.ReadColor(("BtnBorderDisabled"), s->m_clrRibbonComboBtnBorderDisabled);
              s->m_clrRibbonComboBtnBorderPressed = s->m_clrRibbonComboBtnBorderHighlighted;
              tmButton.ReadColor(("BtnBorderPressed"), s->m_clrRibbonComboBtnBorderPressed);
            }
          }
        }
      }
      tmPanel.ReadColor(("TextNormal"), s->m_clrRibbonPanelText);
      tmPanel.ReadColor(("TextHighlighted"), s->m_clrRibbonPanelTextHighlighted);
    }
    if (tmItem.ExcludeTag(("CONTEXT"), str)) {
      TagManager tmContext(str);
      CString strCategory;
      if (tmContext.ExcludeTag(("CATEGORY"), strCategory)) {
        TagManager tmCategory(strCategory);
        ControlRendererParams prBack;
        ControlRendererParams prCaption;
        ControlRendererParams prTab;
        ControlRendererParams prDefault;
        COLOR clrText = s->m_clrRibbonCategoryText;
        COLOR clrTextHighlighted = s->m_clrRibbonCategoryTextHighlighted;
        COLOR clrCaptionText = clrText;
        tmCategory.ReadControlRendererParams(("BACK"), prBack);
        CString strTab;
        if (tmCategory.ExcludeTag(("TAB"), strTab)) {
          TagManager tmTab(strTab);
          tmTab.ReadControlRendererParams(("BUTTON"), prTab);
          tmTab.ReadColor(("TextNormal"), clrText);
          tmTab.ReadColor(("TextHighlighted"), clrTextHighlighted);
        }
        CString strCaption;
        if (tmCategory.ExcludeTag(("CAPTION"), strCaption)) {
          TagManager tmCaption(strCaption);
          tmCaption.ReadControlRendererParams(("BACK"), prCaption);
          tmCaption.ReadColor(("TextNormal"), clrCaptionText);
        }
        tmCategory.ReadControlRendererParams(("BUTTON_DEFAULT"), prDefault);
        CString strID[BCGPRibbonCategoryColorCount] = {
          MakeResourceID(("IDB_OFFICE2007_RIBBON_CONTEXT_R_")),
          MakeResourceID(("IDB_OFFICE2007_RIBBON_CONTEXT_O_")),
          MakeResourceID(("IDB_OFFICE2007_RIBBON_CONTEXT_Y_")),
          MakeResourceID(("IDB_OFFICE2007_RIBBON_CONTEXT_G_")),
          MakeResourceID(("IDB_OFFICE2007_RIBBON_CONTEXT_B_")),
          MakeResourceID(("IDB_OFFICE2007_RIBBON_CONTEXT_I_")),
          MakeResourceID(("IDB_OFFICE2007_RIBBON_CONTEXT_V_"))
        };
        for (int i = 0; i < BCGPRibbonCategoryColorCount; i++) {
          XRibbonContextCategory& cat = s->m_ctrlRibbonContextCategory[i];
          prDefault.s->m_strBmpResID = strID[i] + ("BTN_DEFAULT");
          prTab.s->m_strBmpResID = strID[i] + ("CATEGORY_TAB");
          prCaption.s->m_strBmpResID = strID[i] + ("CATEGORY_CAPTION");
          prBack.s->m_strBmpResID = strID[i] + ("CATEGORY_BACK");
          cat.s->m_ctrlBtnDefault.Create(prDefault);
          cat.s->m_ctrlCaption.Create(prCaption);
          cat.s->m_ctrlTab.Create(prTab);
          cat.s->m_ctrlBack.Create(prBack);
          cat.s->m_clrText = clrText;
          cat.s->m_clrTextHighlighted = clrTextHighlighted;
          cat.s->m_clrCaptionText = clrCaptionText;
        }
      }
      CString strPanel;
      if (tmContext.ExcludeTag(("PANEL"), strPanel)) {
        TagManager tmPanel(strPanel);
        CString strBack;
        if (tmPanel.ExcludeTag(("BACK"), strBack)) {
          TagManager tmBack(strBack);
          tmBack.ReadControlRenderer(("TOP"), s->m_ctrlRibbonContextPanelBack_T, MakeResourceID(("IDB_OFFICE2007_RIBBON_CONTEXT_PANEL_BACK_T")));
          tmBack.ReadControlRenderer(("BOTTOM"), s->m_ctrlRibbonContextPanelBack_B, MakeResourceID(("IDB_OFFICE2007_RIBBON_CONTEXT_PANEL_BACK_B")));
        }
        CString strCaption;
        if (tmPanel.ExcludeTag(("CAPTION"), strCaption)) {
          TagManager tmCaption(strCaption);
          tmCaption.ReadColor(("TextNormal"), s->m_clrRibbonContextPanelCaptionText);
          tmCaption.ReadColor(("TextHighlighted"), s->m_clrRibbonContextPanelCaptionTextHighlighted);
        }
        tmPanel.ReadColor(("TextNormal"), s->m_clrRibbonContextPanelText);
        tmPanel.ReadColor(("TextHighlighted"), s->m_clrRibbonContextPanelTextHighlighted);
      }
      tmContext.ReadControlRenderer(("SEPARATOR"), s->m_ctrlRibbonContextSeparator, MakeResourceID(("IDB_OFFICE2007_RIBBON_CONTEXT_SEPARATOR")));
    }
    tmItem.ReadControlRenderer(("MAIN_BUTTON"), s->m_RibbonBtnMain, MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_MAIN")));
    if (s->m_RibbonBtnMain.IsValid()) {
      s->m_RibbonBtnMain.SmoothResize(s->GetRibbonImageScale());
    }
    if (tmItem.ExcludeTag(("MAIN"), str)) {
      TagManager tmMain(str);
      tmMain.ReadControlRenderer(("BACK"), s->m_ctrlRibbonMainPanel, MakeResourceID(("IDB_OFFICE2007_RIBBON_PANEL_MAIN")));
      tmMain.ReadControlRenderer(("BORDER"), s->m_ctrlRibbonMainPanelBorder, MakeResourceID(("IDB_OFFICE2007_RIBBON_PANEL_MAIN_BORDER")));
      tmMain.ReadControlRenderer(("BUTTON"), s->m_ctrlRibbonBtnMainPanel, MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_PANEL_MAIN")));
    }
    if (tmItem.ExcludeTag(("CAPTION"), str)) {
      TagManager tmCaption(str);
      tmCaption.ReadControlRenderer(("QA"), s->m_ctrlRibbonCaptionQA, MakeResourceID(("IDB_OFFICE2007_RIBBON_CAPTION_QA")));
      tmCaption.ReadControlRenderer(("QA_GLASS"), s->m_ctrlRibbonCaptionQA_Glass, MakeResourceID(("IDB_OFFICE2007_RIBBON_CAPTION_QA_GLASS")));
    }
    if (tmItem.ExcludeTag(("STATUS"), str)) {
      TagManager tmStatus(str);
      tmStatus.ReadControlRenderer(("PANE_BUTTON"), s->m_ctrlRibbonBtnStatusPane, MakeResourceID(("IDB_OFFICE2007_RIBBON_BTN_STATUS_PANE")));
      CString strSlider;
      if (tmStatus.ExcludeTag(("SLIDER"), strSlider)) {
        TagManager tmSlider(strSlider);
        tmSlider.ReadControlRenderer(("THUMB"), s->m_ctrlRibbonSliderThumb, MakeResourceID(("IDB_OFFICE2007_RIBBON_SLIDER_THUMB")));
        tmSlider.ReadControlRenderer(("THUMB_H"), s->m_ctrlRibbonSliderThumbA[0], MakeResourceID(("IDB_OFFICE2007_RIBBON_SLIDER_THUMB_H")));
        tmSlider.ReadControlRenderer(("THUMB_T"), s->m_ctrlRibbonSliderThumbA[1], MakeResourceID(("IDB_OFFICE2007_RIBBON_SLIDER_THUMB_T")));
        tmSlider.ReadControlRenderer(("THUMB_R"), s->m_ctrlRibbonSliderThumbA[2], MakeResourceID(("IDB_OFFICE2007_RIBBON_SLIDER_THUMB_R")));
        tmSlider.ReadControlRenderer(("THUMB_V"), s->m_ctrlRibbonSliderThumbA[3], MakeResourceID(("IDB_OFFICE2007_RIBBON_SLIDER_THUMB_V")));
        tmSlider.ReadControlRenderer(("THUMB_L"), s->m_ctrlRibbonSliderThumbA[4], MakeResourceID(("IDB_OFFICE2007_RIBBON_SLIDER_THUMB_L")));
        tmSlider.ReadControlRenderer(("PLUS"), s->m_ctrlRibbonSliderBtnPlus, MakeResourceID(("IDB_OFFICE2007_RIBBON_SLIDER_BTN_PLUS")));
        tmSlider.ReadControlRenderer(("MINUS"), s->m_ctrlRibbonSliderBtnMinus, MakeResourceID(("IDB_OFFICE2007_RIBBON_SLIDER_BTN_MINUS")));
        s->m_ctrlRibbonSliderThumb.SmoothResize(s->GetRibbonImageScale());
        s->m_ctrlRibbonSliderThumbA[0].SmoothResize(s->GetRibbonImageScale());
        s->m_ctrlRibbonSliderThumbA[1].SmoothResize(s->GetRibbonImageScale());
        s->m_ctrlRibbonSliderThumbA[2].SmoothResize(s->GetRibbonImageScale());
        s->m_ctrlRibbonSliderThumbA[3].SmoothResize(s->GetRibbonImageScale());
        s->m_ctrlRibbonSliderThumbA[4].SmoothResize(s->GetRibbonImageScale());
        s->m_ctrlRibbonSliderBtnMinus.SmoothResize(s->GetRibbonImageScale());
        s->m_ctrlRibbonSliderBtnPlus.SmoothResize(s->GetRibbonImageScale());
      }
      CString strProgress;
      if (tmStatus.ExcludeTag(("PROGRESS"), strProgress)) {
        TagManager tmProgress(strProgress);
        tmProgress.ReadControlRenderer(("BACK"), s->m_ctrlRibbonProgressBack, MakeResourceID(("IDB_OFFICE2007_RIBBON_PROGRESS_BACK")));
        tmProgress.ReadControlRenderer(("NORMAL"), s->m_ctrlRibbonProgressNormal, MakeResourceID(("IDB_OFFICE2007_RIBBON_PROGRESS_NORMAL")));
        tmProgress.ReadControlRenderer(("NORMAL_EXT"), s->m_ctrlRibbonProgressNormalExt, MakeResourceID(("IDB_OFFICE2007_RIBBON_PROGRESS_NORMAL_EXT")));
        tmProgress.ReadControlRenderer(("INFINITY"), s->m_ctrlRibbonProgressInfinity, MakeResourceID(("IDB_OFFICE2007_RIBBON_PROGRESS_INFINITY")));
      }
    }
    if (tmItem.ExcludeTag(("BORDERS"), str)) {
      TagManager tmBorders(str);
      tmBorders.ReadControlRenderer(("QAT"), s->m_ctrlRibbonBorder_QAT, MakeResourceID(("IDB_OFFICE2007_RIBBON_BORDER_QAT")));
      tmBorders.ReadControlRenderer(("FLOATY"), s->m_ctrlRibbonBorder_Floaty, MakeResourceID(("IDB_OFFICE2007_RIBBON_BORDER_FLOATY")));
    }
    if (tmItem.ExcludeTag(("KEYTIP"), str)) {
      TagManager tmKeyTip(str);
      tmKeyTip.ReadControlRenderer(("BACK"), s->m_ctrlRibbonKeyTip, MakeResourceID(("IDB_OFFICE2007_RIBBON_KEYTIP_BACK")));
      tmKeyTip.ReadColor(("TextNormal"), s->m_clrRibbonKeyTipTextNormal);
      BOOL bSystem = FALSE;
      if (s->m_clrRibbonKeyTipTextNormal == (COLOR)(-1)) {
        if (s->m_bToolTipParams && s->m_ToolTipParams.s->m_clrText != (COLOR)(-1)) {
          s->m_clrRibbonKeyTipTextNormal = s->m_ToolTipParams.s->m_clrText;
        }
        else {
          bSystem = TRUE;
          s->m_clrRibbonKeyTipTextNormal = _GetSysColor(CLR_INFOTEXT);
        }
      }
      tmKeyTip.ReadColor(("TextDisabled"), s->m_clrRibbonKeyTipTextDisabled);
      if (s->m_clrRibbonKeyTipTextDisabled == (COLOR)(-1)) {
        if (bSystem) {
          s->m_clrRibbonKeyTipTextDisabled = s->clrGrayedText;
        }
        else {
          s->m_clrRibbonKeyTipTextDisabled = PixelAlpha2(
              s->m_clrRibbonKeyTipTextNormal, s->clrWindow, 50);
        }
      }
    }
    if (tmItem.ExcludeTag(("HYPERLINK"), str)) {
      TagManager tmHyperlink(str);
      tmHyperlink.ReadColor(("Inactive"), s->m_clrRibbonHyperlinkInactive);
      tmHyperlink.ReadColor(("Active"), s->m_clrRibbonHyperlinkActive);
      tmHyperlink.ReadColor(("StatusbarInactive"), s->m_clrRibbonStatusbarHyperlinkInactive);
      tmHyperlink.ReadColor(("StatusbarActive"), s->m_clrRibbonStatusbarHyperlinkActive);
    }
  }
  if (tm.ExcludeTag(("PLANNER"), strItem)) {
    TagManager tmItem(strItem);
    if (tmItem.ReadColor(("WorkColor"), s->m_clrPlannerWork)) {
      s->m_clrPlanner.DeleteObject();
      s->m_clrPlanner.CreateSolidBrush(s->m_clrPlannerWork);
    }
    tmItem.ReadColor(("NcAreaColor"), s->m_clrPlannerNcArea);
    tmItem.ReadColor(("NcLineColor"), s->m_clrPlannerNcLine);
    tmItem.ReadColor(("NcTextColor"), s->m_clrPlannerNcText);
    CString str;
    if (tmItem.ExcludeTag(("TODAY"), str)) {
      TagManager tmToday(str);
      tmToday.ReadColor(("Caption1Start") , s->m_clrPlannerTodayCaption[0]);
      tmToday.ReadColor(("Caption1Finish"), s->m_clrPlannerTodayCaption[1]);
      tmToday.ReadColor(("Caption2Start") , s->m_clrPlannerTodayCaption[2]);
      tmToday.ReadColor(("Caption2Finish"), s->m_clrPlannerTodayCaption[3]);
      tmToday.ReadColor(("BorderColor") , s->m_clrPlannerTodayBorder);
    }
  }
  s->m_clrGridLeftOffset = s->clrBarFace;
  s->m_clrGridGroupLine = s->clrBarShadow;
  s->m_clrGridHeaderNormalStart = s->m_clrHeaderNormalStart;
  s->m_clrGridHeaderNormalFinish = s->m_clrHeaderNormalFinish;
  s->m_clrGridHeaderNormalBorder = s->m_clrHeaderNormalBorder;
  s->m_clrGridHeaderPressedStart = s->m_clrHeaderPressedStart;
  s->m_clrGridHeaderPressedFinish = s->m_clrHeaderPressedFinish;
  s->m_clrGridHeaderPressedBorder = s->m_clrHeaderPressedBorder;
  s->m_clrGridHeaderAllNormalBackStart = DrawManager::MixColors(s->m_clrGridHeaderNormalStart, s->m_clrGridHeaderNormalBorder, 0.50);
  s->m_clrGridHeaderAllNormalBackFinish = s->m_clrGridHeaderAllNormalBackStart;
  s->m_clrGridHeaderAllPressedBackStart = s->m_clrHeaderPressedBorder;
  s->m_clrGridHeaderAllPressedBackFinish = s->m_clrGridHeaderAllPressedBackStart;
  s->m_clrGridHeaderAllNormalBorderHighlighted = s->m_clrGridHeaderNormalStart;
  s->m_clrGridHeaderAllNormalBorderShadow = s->m_clrGridHeaderNormalFinish;
  s->m_clrGridHeaderAllPressedBorderHighlighted = s->m_clrGridHeaderAllNormalBorderHighlighted;
  s->m_clrGridHeaderAllPressedBorderShadow = s->m_clrGridHeaderAllPressedBorderShadow;
  s->m_clrGridHeaderAllNormalSignStart = s->m_clrGridHeaderNormalStart;
  s->m_clrGridHeaderAllNormalSignFinish = s->m_clrGridHeaderNormalFinish;
  s->m_clrGridHeaderAllPressedSignStart = s->m_clrGridHeaderAllNormalSignStart;
  s->m_clrGridHeaderAllPressedSignFinish = s->m_clrGridHeaderAllNormalSignFinish;
  if (tm.ExcludeTag(("GRID"), strItem)) {
    TagManager tmItem(strItem);
    CString strHeader;
    if (tmItem.ExcludeTag(("HEADER"), strHeader)) {
      TagManager tmHeader(strHeader);
      tmHeader.ReadColor(("NormalStart"), s->m_clrGridHeaderNormalStart);
      tmHeader.ReadColor(("NormalFinish"), s->m_clrGridHeaderNormalFinish);
      tmHeader.ReadColor(("NormalBorder"), s->m_clrGridHeaderNormalBorder);
      s->m_clrGridHeaderPressedStart = s->m_clrGridHeaderNormalFinish;
      s->m_clrGridHeaderPressedFinish = s->m_clrGridHeaderNormalStart;
      s->m_clrGridHeaderPressedBorder = s->m_clrGridHeaderNormalBorder;
      tmHeader.ReadColor(("PressedStart"), s->m_clrGridHeaderPressedStart);
      tmHeader.ReadColor(("PressedFinish"), s->m_clrGridHeaderPressedFinish);
      tmHeader.ReadColor(("PressedBorder"), s->m_clrGridHeaderPressedBorder);
      s->m_clrGridHeaderAllNormalBackStart = DrawManager::MixColors(s->m_clrGridHeaderNormalStart, s->m_clrGridHeaderNormalBorder, 0.50);
      s->m_clrGridHeaderAllNormalBackFinish = s->m_clrGridHeaderAllNormalBackStart;
      s->m_clrGridHeaderAllPressedBackStart = s->m_clrHeaderPressedBorder;
      s->m_clrGridHeaderAllPressedBackFinish = s->m_clrGridHeaderAllPressedBackStart;
      s->m_clrGridHeaderAllNormalBorderHighlighted = s->m_clrGridHeaderNormalStart;
      s->m_clrGridHeaderAllNormalBorderShadow = s->m_clrGridHeaderNormalFinish;
      s->m_clrGridHeaderAllPressedBorderHighlighted = s->m_clrGridHeaderAllNormalBorderHighlighted;
      s->m_clrGridHeaderAllPressedBorderShadow = s->m_clrGridHeaderAllPressedBorderShadow;
      s->m_clrGridHeaderAllNormalSignStart = s->m_clrGridHeaderNormalStart;
      s->m_clrGridHeaderAllNormalSignFinish = s->m_clrGridHeaderNormalFinish;
      s->m_clrGridHeaderAllPressedSignStart = s->m_clrGridHeaderAllNormalSignStart;
      s->m_clrGridHeaderAllPressedSignFinish = s->m_clrGridHeaderAllNormalSignFinish;
      CString strAll;
      if (tmHeader.ExcludeTag(("ALL_AREA"), strAll)) {
        TagManager tmAll(strAll);
        tmAll.ReadColor(("NormalBackStart"), s->m_clrGridHeaderAllNormalBackStart);
        s->m_clrGridHeaderAllNormalBackFinish = s->m_clrGridHeaderAllNormalBackStart;
        tmAll.ReadColor(("NormalBackFinish"), s->m_clrGridHeaderAllNormalBackFinish);
        s->m_clrGridHeaderAllPressedBackStart = s->m_clrGridHeaderAllNormalBackStart;
        s->m_clrGridHeaderAllPressedBackFinish = s->m_clrGridHeaderAllNormalBackFinish;
        tmAll.ReadColor(("PressedBackStart"), s->m_clrGridHeaderAllPressedBackStart);
        s->m_clrGridHeaderAllPressedBackFinish = s->m_clrGridHeaderAllPressedBackStart;
        tmAll.ReadColor(("PressedBackFinish"), s->m_clrGridHeaderAllPressedBackFinish);
        tmAll.ReadColor(("NormalBorderHighlighted"), s->m_clrGridHeaderAllNormalBorderHighlighted);
        s->m_clrGridHeaderAllNormalBorderShadow = s->m_clrGridHeaderAllNormalBorderHighlighted;
        tmAll.ReadColor(("NormalBorderShadow"), s->m_clrGridHeaderAllNormalBorderShadow);
        s->m_clrGridHeaderAllPressedBorderHighlighted = s->m_clrGridHeaderAllNormalBorderHighlighted;
        s->m_clrGridHeaderAllPressedBorderShadow = s->m_clrGridHeaderAllNormalBorderShadow;
        tmAll.ReadColor(("PressedBorderHighlighted"), s->m_clrGridHeaderAllPressedBorderHighlighted);
        s->m_clrGridHeaderAllPressedBorderShadow = s->m_clrGridHeaderAllPressedBorderHighlighted;
        tmAll.ReadColor(("PressedBorderShadow"), s->m_clrGridHeaderAllPressedBorderShadow);
        if (tmAll.ReadColor(("NormalSignStart"), s->m_clrGridHeaderAllNormalSignStart)) {
          s->m_clrGridHeaderAllNormalSignFinish = s->m_clrGridHeaderAllNormalSignStart;
        }
        tmAll.ReadColor(("NormalSignFinish"), s->m_clrGridHeaderAllNormalSignFinish);
        s->m_clrGridHeaderAllPressedSignStart = s->m_clrGridHeaderAllNormalSignStart;
        s->m_clrGridHeaderAllPressedSignFinish = s->m_clrGridHeaderAllNormalSignFinish;
        if (tmAll.ReadColor(("PressedSignStart"), s->m_clrGridHeaderAllPressedSignStart)) {
          s->m_clrGridHeaderAllPressedSignFinish = s->m_clrGridHeaderAllPressedSignStart;
        }
        tmAll.ReadColor(("PressedSignFinish"), s->m_clrGridHeaderAllPressedSignFinish);
      }
      COLOR clr;
      if (tmHeader.ReadColor(("Separator"), clr)) {
        s->m_penGridSeparator.DeleteObject();
        s->m_penGridSeparator.CreatePen(PS_SOLID, 1, clr);
      }
    }
    COLOR clr = s->clrBtnLight;
    if (tmItem.ReadColor(("ExpandBoxLight"), clr)) {
      s->m_penGridExpandBoxLight.DeleteObject();
      s->m_penGridExpandBoxLight.CreatePen(PS_SOLID, 1, clr);
    }
    clr = s->clrBtnShadow;
    if (tmItem.ReadColor(("ExpandBoxDark"), clr)) {
      s->m_penGridExpandBoxDark.DeleteObject();
      s->m_penGridExpandBoxDark.CreatePen(PS_SOLID, 1, clr);
    }
    tmItem.ReadColor(("LeftOffset"), s->m_clrGridLeftOffset);
    tmItem.ReadColor(("GroupLine"), s->m_clrGridGroupLine);
    s->m_clrGridGroupSubLine = s->m_clrGridGroupLine;
    tmItem.ReadColor(("GroupSubLine"), s->m_clrGridGroupSubLine);
#ifndef BCGP_EXCLUDE_GRID_CTRL
    if (!tmItem.ReadGridColors(("COLORS"), s->m_GridColors)) {
      s->m_GridColors.s->m_clrHorzLine = s->m_clrToolBarGradientDark;
      s->m_GridColors.s->m_clrVertLine = s->m_clrToolBarBottomLine;
      s->m_GridColors.s->m_EvenColors.m_clrBackground = s->m_clrToolBarGradientLight;
      s->m_GridColors.s->m_EvenColors.m_clrText = s->m_clrToolBarBtnText;
      s->m_GridColors.s->m_OddColors.m_clrBackground = s->m_clrToolBarGradientDark;
      s->m_GridColors.s->m_OddColors.m_clrText = s->m_clrToolBarBtnText;
      s->m_GridColors.s->m_SelColors.s->m_clrBackground = s->m_clrRibbonComboBtnHighlightedFinish;
      s->m_GridColors.s->m_GroupColors.s->m_clrBackground = s->m_clrToolBarGradientDark;
      s->m_GridColors.s->m_GroupColors.s->m_clrGradient = s->m_clrToolBarGradientLight;
      s->m_GridColors.s->m_GroupColors.s->m_clrText = s->m_clrToolBarBtnText;
      s->m_GridColors.s->m_GroupSelColors.s->m_clrBackground = s->m_clrRibbonComboBtnHighlightedStart;
      s->m_GridColors.s->m_GroupSelColors.s->m_clrGradient = s->m_clrRibbonComboBtnHighlightedFinish;
      s->m_GridColors.s->m_HeaderSelColors.s->m_clrBackground = s->m_clrRibbonComboBtnHighlightedStart;
      s->m_GridColors.s->m_HeaderSelColors.s->m_clrGradient = s->m_clrRibbonComboBtnHighlightedFinish;
      s->m_GridColors.s->m_HeaderSelColors.s->m_clrBorder = s->m_clrRibbonComboBtnBorderHighlighted;
      s->m_GridColors.s->m_LeftOffsetColors.s->m_clrBackground = s->m_clrToolBarGradientDark;
      s->m_GridColors.s->m_LeftOffsetColors.s->m_clrBorder = s->m_GridColors.s->m_clrHorzLine;
    }
#endif
  }
  s->m_clrOutlookCaptionTextNormal = s->m_clrCaptionBarText;
  s->m_clrOutlookPageTextNormal = s->m_clrOutlookCaptionTextNormal;
  s->m_clrOutlookPageTextHighlighted = s->m_clrOutlookPageTextNormal;
  s->m_clrOutlookPageTextPressed = s->m_clrOutlookPageTextNormal;
  if (tm.ExcludeTag(("OUTLOOK"), strItem)) {
    TagManager tmItem(strItem);
    CString str;
    if (tmItem.ExcludeTag(("CAPTION"), str)) {
      TagManager tmCaption(str);
      tmCaption.ReadColor(("TextNormal"), s->m_clrOutlookCaptionTextNormal);
    }
    if (tmItem.ExcludeTag(("PAGEBUTTON"), str)) {
      TagManager tmPage(str);
      tmPage.ReadControlRenderer(("BACK"), s->m_ctrlOutlookWndPageBtn, MakeResourceID(("IDB_OFFICE2007_OUTLOOK_BTN_PAGE")));
      tmPage.ReadColor(("TextNormal"), s->m_clrOutlookPageTextNormal);
      tmPage.ReadColor(("TextHighlighted"), s->m_clrOutlookPageTextHighlighted);
      tmPage.ReadColor(("TextPressed"), s->m_clrOutlookPageTextPressed);
    }
    if (tmItem.ExcludeTag(("BAR"), str)) {
      TagManager tmBar(str);
      tmBar.ReadControlRenderer(("BACK"), s->m_ctrlOutlookWndBar, MakeResourceID(("IDB_OFFICE2007_OUTLOOK_BAR_BACK")));
    }
  }
  // Popup Window:
  s->m_clrPopupGradientLight = s->m_clrBarGradientLight;
  s->m_clrPopupGradientDark = s->m_clrBarGradientDark;
  if (tm.ExcludeTag(("POPUP"), strItem)) {
    TagManager tmItem(strItem);
    tmItem.ReadColor(("GradientFillLight"), s->m_clrPopupGradientLight);
    tmItem.ReadColor(("GradientFillDark"), s->m_clrPopupGradientDark);
  }
  if (tm.ExcludeTag(("SLIDER"), strItem)) {
    TagManager tmItem(strItem);
    tmItem.ReadControlRenderer(("THUMB_B"), s->m_ctrlSliderThumb[0], MakeResourceID(("IDB_OFFICE2007_SLIDER_THUMB_B")));
    tmItem.ReadControlRenderer(("THUMB_H"), s->m_ctrlSliderThumb[1], MakeResourceID(("IDB_OFFICE2007_SLIDER_THUMB_H")));
    tmItem.ReadControlRenderer(("THUMB_T"), s->m_ctrlSliderThumb[2], MakeResourceID(("IDB_OFFICE2007_SLIDER_THUMB_T")));
    tmItem.ReadControlRenderer(("THUMB_R"), s->m_ctrlSliderThumb[3], MakeResourceID(("IDB_OFFICE2007_SLIDER_THUMB_R")));
    tmItem.ReadControlRenderer(("THUMB_V"), s->m_ctrlSliderThumb[4], MakeResourceID(("IDB_OFFICE2007_SLIDER_THUMB_V")));
    tmItem.ReadControlRenderer(("THUMB_L"), s->m_ctrlSliderThumb[5], MakeResourceID(("IDB_OFFICE2007_SLIDER_THUMB_L")));
  }
  if (hinstResOld != NULL) {
    AfxSetResourceHandle(hinstResOld);
  }
  TagManager::SetBaseColor((COLOR) - 1, (COLOR) - 1);
}
BOOL VisualManager2007_IsWindowActive(CWnd* pWnd) const
{
  BOOL bActive = FALSE;
  HWND hWnd = pWnd->GetSafeHwnd();
  if (hWnd != NULL) {
    if (!s->m_ActivateFlag.Lookup(pWnd->GetSafeHwnd(), bActive)) {
      //ASSERT(FALSE);
      bActive = TRUE;
    }
  }
  return bActive;
}
BOOL VisualManager2007_OnNcActivate(CWnd* pWnd, BOOL bActive)
{
  ASSERT_VALID(pWnd);
  if (pWnd->GetSafeHwnd() == NULL) {
    return FALSE;
  }
  if (s->DwmIsCompositionEnabled()) {
    return FALSE;
  }
  // stay active if WF_STAYACTIVE bit is on
  if (pWnd->s->m_nFlags & WF_STAYACTIVE) {
    bActive = TRUE;
  }
  // but do not stay active if the window is disabled
  if (!pWnd->IsWindowEnabled()) {
    bActive = FALSE;
  }
  s->m_ActivateFlag[pWnd->GetSafeHwnd()] = bActive;
  pWnd->SendMessage(Ws->m_NCPAINT, 0, 0);
  return TRUE;
}
int VisualManager2007_DrawNcBtn(img_t* im, const IRECT& rect, UINT nButton,
    BCGBUTTON_STATE state, BOOL bSmall,
    BOOL bActive, BOOL bMDI/* = FALSE*/)
{
  ToolBarImages* pImage = NULL;
  int nIndex = bSmall ? 1 : 0;
  if (nButton == SC_CLOSE) {
    pImage = &s->m_SysBtnClose[nIndex];
  }
  else if (nButton == SC_MINIMIZE) {
    pImage = &s->m_SysBtnMinimize[nIndex];
  }
  else if (nButton == SC_MAXIMIZE) {
    pImage = &s->m_SysBtnMaximize[nIndex];
  }
  else if (nButton == SC_RESTORE) {
    pImage = &s->m_SysBtnRestore[nIndex];
  }
  else if (nButton == SC_CONTEXTHELP) {
    pImage = &s->m_SysBtnHelp[nIndex];
  }
  ImageAlignHorz horz = s->GetRibbonImageScale() != 1. ? ToolBarImages::ImageAlignHorzStretch : ToolBarImages::ImageAlignHorzCenter;
  ImageAlignVert vert = s->GetRibbonImageScale() != 1. ? ImageAlignVertStretch : ImageAlignVertCenter;
  IRECT rtBtnImage(CPoint(0, 0), pImage->GetImageSize());
  if (!bActive) {
    rtBtnImage.OffsetRect(0, pImage->GetImageSize().h * 3);
  }
  else {
    if (state != ButtonsIsRegular) {
      if (!IsBeta() && bMDI) {
        s->m_ctrlRibbonBtn[0].Draw(im, rect, state == ButtonsIsHighlighted ? 0 : 1);
      }
      else {
        s->m_SysBtnBack[nIndex].Draw(im, rect, state == ButtonsIsHighlighted ? 0 : 1);
      }
      rtBtnImage.OffsetRect(0, pImage->GetImageSize().h *
          (state == ButtonsIsHighlighted ? 1 : 2));
    }
  }
  if (pImage != NULL) {
    pImage->DrawEx(im, rect, 0, horz, vert, rtBtnImage);
  }
}
int VisualManager2007_DrawNcText(img_t* im, IRECT& rect,
    const CString& strTitle,
    const CString& strDocument,
    BOOL bPrefix, BOOL bActive, BOOL bIsRTL,
    BOOL bTextCenter,
    BOOL bGlass/* = FALSE*/, int nGlassGlowSize/* = 0*/,
    COLOR clrGlassText/* =(COLOR)-1*/)
{
  if ((strTitle.IsEmpty() && strDocument.IsEmpty()) ||
      rect.r <= rect.l) {
    return 0;
  }
  int nOldMode = im->SetBkMode(TRANSPARENT);
  COLOR clrOldText = im->GetTextColor();
  DWORD dwTextStyle = DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER |
      (bIsRTL ? DT_RTLREADING : 0);
  if (strDocument.IsEmpty()) {
    COLOR clrText = bActive
        ? s->m_clrAppCaptionActiveTitleText
        : s->m_clrAppCaptionInactiveTitleText;
    int widthFull = rect.Width();
    int width = im->GetTextExtent(strTitle).w;
    if (bTextCenter && width < widthFull) {
      rect.l += (widthFull - width) / 2;
    }
    rect.r = min(rect.l + width, rect.r);
    if (rect.r > rect.l) {
      if (bGlass) {
        DrawTextOnGlass(im, strTitle, rect, dwTextStyle, nGlassGlowSize, clrGlassText);
      }
      else {
        im->SetTextColor(clrText);
        im->DrawText(strTitle, rect, dwTextStyle);
      }
    }
  }
  else {
    const CString& str1 = bPrefix ? strDocument : strTitle;
    const CString& str2 = bPrefix ? strTitle : strDocument;
    COLOR clrText1 = bActive
        ? s->m_clrAppCaptionActiveText
        : s->m_clrAppCaptionInactiveText;
    COLOR clrText2 = bActive
        ? s->m_clrAppCaptionActiveTitleText
        : s->m_clrAppCaptionInactiveTitleText;
    if (!bPrefix) {
      COLOR clr = clrText1;
      clrText1 = clrText2;
      clrText2 = clr;
    }
    int widthFull = rect.Width();
    CSize sz1 = im->GetTextExtent(str1);
    CSize sz2 = im->GetTextExtent(str2);
    int width = sz1.w + sz2.w;
    int left = rect.l;
    if (bTextCenter && width < widthFull) {
      rect.l += (widthFull - width) / 2;
    }
    rect.r = min(rect.l + width, rect.r);
    if (bIsRTL) {
      if (width <= rect.Width()) {
        rect.l += sz2.w;
      }
      else {
        if (sz1.w < rect.Width()) {
          rect.l += max(0, sz2.w + (rect.Width() - width));
        }
      }
    }
    if (bGlass) {
      DrawTextOnGlass(im, str1, rect, dwTextStyle, nGlassGlowSize, clrGlassText);
    }
    else {
      im->SetTextColor(clrText1);
      im->DrawText(str1, rect, dwTextStyle);
    }
    if (bIsRTL) {
      if (width <= (rect.r - left)) {
        rect.r = rect.l;
        rect.l = rect.r - sz2.w;
      }
      else {
        rect.l = left;
        rect.r -= sz1.w;
      }
    }
    else {
      rect.l += sz1.w;
    }
    if (rect.r > rect.l) {
      if (bGlass) {
        DrawTextOnGlass(im, str2, rect, dwTextStyle, nGlassGlowSize, clrGlassText);
      }
      else {
        im->SetTextColor(clrText2);
        im->DrawText(str2, rect, dwTextStyle);
      }
    }
  }
  im->SetBkMode(nOldMode);
  im->SetTextColor(clrOldText);
}
int VisualManager2007_DrawNcCaption(img_t* im, IRECT rectCaption,
    DWORD dwStyle, DWORD dwStyleEx,
    const CString& strTitle, const CString& strDocument,
    CBitmap* hIcon, BOOL bPrefix, BOOL bActive,
    BOOL bTextCenter,
    const CObList& lstSysButtons)
{
  const BOOL bIsRTL = (dwStyleEx & WS_EX_LAYOUTRTL) == WS_EX_LAYOUTRTL;
  const BOOL bIsSmallCaption = (dwStyleEx & WS_EX_TOOLWINDOW) != 0;
  const int nSysCaptionHeight = bIsSmallCaption ? ::GetSystemMetrics(SM_CYSMCAPTION) : ::GetSystemMetrics(SM_CYCAPTION);
  CSize szSysBorder(GetSystemBorders(FALSE));
  CDC memDC;
  memDC.CreateCompatibleDC(im);
  CBitmap memBmp;
  memBmp.CreateCompatibleBitmap(im, rectCaption.Width(), rectCaption.Height());
  CBitmap* pBmpOld = memDC.SelectObject(&memBmp);
  memDC.BitBlt(0, 0, rectCaption.Width(), rectCaption.Height(), im, 0, 0, SRCCOPY);
  BOOL bMaximized = (dwStyle & WS_MAXIMIZE) == WS_MAXIMIZE;
  {
    if (IsBeta()) {
      COLOR clr1 = bActive
          ? s->m_clrAppCaptionActiveStart
          : s->m_clrAppCaptionInactiveStart;
      COLOR clr2 = bActive
          ? s->m_clrAppCaptionActiveFinish
          : s->m_clrAppCaptionInactiveFinish;
      IRECT rectCaption1(rectCaption);
      IRECT rectBorder(s->m_ctrlMainBorderCaption.GetParams().s->m_rectSides);
      rectCaption1.DeflateRect(rectBorder.l, rectBorder.t,
          rectBorder.r, rectBorder.b);
      {
        DrawManager dm(memDC);
        dm.Fill4ColorsGradient(rectCaption1, clr1, clr2, clr2, clr1, FALSE);
      }
      s->m_ctrlMainBorderCaption.DrawFrame(&memDC, rectCaption, bActive ? 0 : 1);
    }
    else {
      IRECT rectBorderCaption(rectCaption);
      if (bMaximized) {
        rectBorderCaption.OffsetRect(-rectBorderCaption.TopLeft());
        rectBorderCaption.b -= szSysBorder.h;
      }
      s->m_ctrlMainBorderCaption.Draw(&memDC, rectBorderCaption, bActive ? 0 : 1);
    }
  }
  IRECT rect(rectCaption);
  rect.DeflateRect(szSysBorder.w, szSysBorder.h, szSysBorder.w, 0);
  rect.t = rect.b - nSysCaptionHeight - 1;
  // Draw icon:
  if (hIcon != NULL && !bIsSmallCaption) {
    CSize szIcon(::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
    long x = rect.l + (bMaximized ? szSysBorder.w : 0) + 2;
    long y = rect.t + max(0, (nSysCaptionHeight - szIcon.h) / 2);
    ::DrawIconEx(memDC.GetSafeHdc(), x, y, hIcon, szIcon.w, szIcon.h,
        0, NULL, DI_NORMAL);
    rect.l = x + szIcon.w + (bMaximized ? szSysBorder.w : 4);
  }
  // Draw system buttons:
  int xButtonsRight = rect.r;
  for (POSITION pos = lstSysButtons.GetHeadPosition(); pos != NULL;) {
    FrameCaptionButton* pButton = (FrameCaptionButton*)
        lstSysButtons.GetNext(pos);
    BCGBUTTON_STATE state = ButtonsIsRegular;
    if (pButton->s->m_bPushed && pButton->s->m_bFocused) {
      state = ButtonsIsPressed;
    }
    else if (pButton->s->m_bFocused) {
      state = ButtonsIsHighlighted;
    }
    UINT uiHit = pButton->GetHit();
    UINT nButton = 0;
    switch (uiHit) {
    case HTCLOSE_BCG:
      nButton = SC_CLOSE;
      break;
    case HTMAXBUTTON_BCG:
      nButton =
          (dwStyle & WS_MAXIMIZE) == WS_MAXIMIZE ? SC_RESTORE : SC_MAXIMIZE;
      break;
    case HTMINBUTTON_BCG:
      nButton =
          (dwStyle & WS_MINIMIZE) == WS_MINIMIZE ? SC_RESTORE : SC_MINIMIZE;
      break;
    case HTHELPBUTTON_BCG:
      nButton = SC_CONTEXTHELP;
      break;
    }
    IRECT rectBtn(pButton->GetRect());
    if (bMaximized) {
      rectBtn.OffsetRect(szSysBorder.w, szSysBorder.h);
    }
    DrawNcBtn(&memDC, rectBtn, nButton, state, FALSE, bActive, FALSE);
    xButtonsRight = min(xButtonsRight, pButton->GetRect().l);
  }
  // Draw text:
  if ((!strTitle.IsEmpty() || !strDocument.IsEmpty()) &&
      rect.l < rect.r) {
    CFont* pOldFont = (CFont*)memDC.SelectObject(&s->m_AppCaptionFont);
    IRECT rectText = rect;
    rectText.r = xButtonsRight - 1;
    DrawNcText(&memDC, rectText, strTitle, strDocument, bPrefix, bActive, bIsRTL, bTextCenter);
    memDC.SelectObject(pOldFont);
  }
  im->BitBlt(rectCaption.l, rectCaption.t, rectCaption.Width(), rectCaption.Height(),
      &memDC, 0, 0, SRCCOPY);
  memDC.SelectObject(pBmpOld);
}
#ifndef BCGP_EXCLUDE_RIBBON
RibbonBar* VisualManager2007_GetRibbonBar(CWnd* pWnd) const
{
  RibbonBar* pBar = NULL;
  if (pWnd == NULL) {
    pWnd = AfxGetMainWnd();
  }
  if (pWnd->GetSafeHwnd() == NULL) {
    return NULL;
  }
  if (pWnd->IsKindOf(RUNTIME_CLASS(FrameWnd))) {
    pBar = ((FrameWnd*) pWnd)->GetRibbonBar();
  }
  else if (pWnd->IsKindOf(RUNTIME_CLASS(MDIFrameWnd))) {
    pBar = ((MDIFrameWnd*) pWnd)->GetRibbonBar();
  }
  return pBar;
}
BOOL VisualManager2007_IsRibbonPresent(CWnd* pWnd) const
{
  RibbonBar* pBar = GetRibbonBar(pWnd);
  return pBar != NULL && pBar->IsWindowVisible();
}
#endif
BOOL VisualManager2007_OnNcPaint(CWnd* pWnd, const CObList& lstSysButtons, IRECT rectRedraw)
{
  if (!CanDrawImage()) {
    return VisualManager2003_OnNcPaint(pWnd, lstSysButtons, rectRedraw);
  }
  ASSERT_VALID(pWnd);
  if (pWnd->GetSafeHwnd() == NULL) {
    return FALSE;
  }
  CWindowDC dc(pWnd);
  if (dc.GetSafeHdc() != NULL) {
    CRgn rgn;
    if (!rectRedraw.IsRectEmpty()) {
      rgn.CreateRectRgnIndirect(rectRedraw);
      dc.SelectClipRgn(&rgn);
    }
#ifndef BCGP_EXCLUDE_RIBBON
    RibbonBar* pBar = GetRibbonBar(pWnd);
    BOOL bRibbonCaption = pBar != NULL &&
        pBar->IsWindowVisible() &&
        pBar->IsReplaceFrameCaption();
#else
    BOOL bRibbonCaption = FALSE;
#endif
    IRECT rtWindow;
    pWnd->GetWindowRect(rtWindow);
    pWnd->ScreenToClient(rtWindow);
    IRECT rtClient;
    pWnd->GetClientRect(rtClient);
    rtClient.OffsetRect(-rtWindow.TopLeft());
    dc.ExcludeClipRect(rtClient);
    rtWindow.OffsetRect(-rtWindow.TopLeft());
    BOOL bActive = IsWindowActive(pWnd);
    IRECT rectCaption(rtWindow);
    CSize szSysBorder(GetSystemBorders(bRibbonCaption));
    BOOL bDialog = pWnd->IsKindOf(RUNTIME_CLASS(Dialog)) || pWnd->IsKindOf(RUNTIME_CLASS(PropertySheet));
    if (bDialog && (pWnd->GetStyle() & WS_THICKFRAME) == 0) {
      szSysBorder.h = ::GetSystemMetrics(SM_CYFIXEDFRAME);
    }
    rectCaption.b = rectCaption.t + szSysBorder.h;
    const DWORD dwStyle = pWnd->GetStyle();
    BOOL bMaximized = (dwStyle & WS_MAXIMIZE) == WS_MAXIMIZE;
    if (!bRibbonCaption) {
      const DWORD dwStyleEx = pWnd->GetExStyle();
      const BOOL bIsSmallCaption = (dwStyleEx & WS_EX_TOOLWINDOW) != 0;
      const int nSysCaptionHeight = bIsSmallCaption ? ::GetSystemMetrics(SM_CYSMCAPTION) : ::GetSystemMetrics(SM_CYCAPTION);
      rectCaption.b += nSysCaptionHeight;
      BOOL bDestroyIcon = FALSE;
      CBitmap* hIcon = globalUtils.GetWndIcon(pWnd, &bDestroyIcon);
      CString strText;
      pWnd->GetWindowText(strText);
      CString strTitle(strText);
      CString strDocument;
      BOOL bPrefix = FALSE;
      if ((dwStyle & FWS_ADDTOTITLE) == FWS_ADDTOTITLE) {
        bPrefix = (dwStyle & FWS_PREFIXTITLE) == FWS_PREFIXTITLE;
        CFrameWnd* pFrameWnd = DYNAMIC_DOWNCAST(CFrameWnd, pWnd);
        if (pFrameWnd != NULL) {
          strTitle = pFrameWnd->GetTitle();
          if (!strTitle.IsEmpty()) {
            if (strText.GetLength() >= strTitle.GetLength()) {
              if (bPrefix) {
                int pos = strText.Find(strTitle, strText.GetLength() - strTitle.GetLength());
                if (pos != -1) {
                  strTitle = strText.Right(strTitle.GetLength() + 3);
                  strDocument = strText.Left(strText.GetLength() - strTitle.GetLength());
                }
              }
              else {
                int pos = strText.Find(strTitle);
                if (pos != -1) {
                  strTitle = strText.Left(strTitle.GetLength() + 3);
                  strDocument = strText.Right(strText.GetLength() - strTitle.GetLength());
                }
              }
            }
          }
          else {
            strDocument = strText;
          }
        }
      }
      if (bMaximized) {
        rectCaption.InflateRect(szSysBorder.w, szSysBorder.h, szSysBorder.w, 0);
      }
      DrawNcCaption(&dc, rectCaption, dwStyle, dwStyleEx,
          strTitle, strDocument, hIcon, bPrefix, bActive, s->m_bNcTextCenter,
          lstSysButtons);
      if (bDestroyIcon) {
        ::DestroyIcon(hIcon);
      }
      if (bMaximized) {
        return TRUE;
      }
    }
#ifndef BCGP_EXCLUDE_RIBBON
    else {
      if (bMaximized) {
        return TRUE;
      }
      rectCaption.b += pBar->GetCaptionHeight();
      if (IsBeta()) {
        IRECT rectBorder(s->m_ctrlMainBorderCaption.GetParams().s->m_rectSides);
        COLOR clr1 = bActive
            ? s->m_clrAppCaptionActiveStart
            : s->m_clrAppCaptionInactiveStart;
        COLOR clr2 = bActive
            ? s->m_clrAppCaptionActiveFinish
            : s->m_clrAppCaptionInactiveFinish;
        IRECT rectCaption2(rectCaption);
        rectCaption2.DeflateRect(rectBorder.l, rectBorder.t,
            rectBorder.r, rectBorder.b);
        {
          DrawManager dm(dc);
          dm.Fill4ColorsGradient(rectCaption2, clr1, clr2, clr2, clr1, FALSE);
        }
        s->m_ctrlMainBorderCaption.DrawFrame(&dc, rectCaption, bActive ? 0 : 1);
      }
      else {
        s->m_ctrlMainBorderCaption.Draw(&dc, rectCaption, bActive ? 0 : 1);
      }
    }
#endif // BCGP_EXCLUDE_RIBBON
    rtWindow.t = rectCaption.b;
    dc.ExcludeClipRect(rectCaption);
    if (pWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)) ||
        (bDialog && !s->m_ctrlDialogBorder.IsValid())) {
      if (bDialog) {
        IRECT rtDialog(rtWindow);
        rtDialog.DeflateRect(1, 0, 1, 1);
        dc.FillRect(rtDialog, &GetDlgBackBrush(pWnd));
        dc.ExcludeClipRect(rtDialog);
      }
      s->m_ctrlMDIChildBorder.DrawFrame(&dc, rtWindow, bActive ? 0 : 1);
    }
    else if (bDialog) {
      s->m_ctrlDialogBorder.DrawFrame(&dc, rtWindow, bActive ? 0 : 1);
    }
    else {
      s->m_ctrlMainBorder.DrawFrame(&dc, rtWindow, bActive ? 0 : 1);
    }
    if (bDialog) {
      dc.SelectClipRgn(NULL);
      return TRUE;
    }
    //-------------------------------
    // Find status bar extended area:
    //-------------------------------
    IRECT rectExt(0, 0, 0, 0);
    BOOL bExtended = FALSE;
    BOOL bBottomFrame = FALSE;
    BOOL bIsStatusBar = FALSE;
    CWnd* pStatusBar = pWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR, TRUE);
    if (pStatusBar->GetSafeHwnd() != NULL && pStatusBar->IsWindowVisible()) {
      StatusBar* pClassicStatusBar = DYNAMIC_DOWNCAST(
          StatusBar, pStatusBar);
      if (pClassicStatusBar != NULL) {
        bExtended = pClassicStatusBar->GetExtendedArea(rectExt);
        bIsStatusBar = TRUE;
      }
#ifndef BCGP_EXCLUDE_RIBBON
      else {
        RibbonStatusBar* pRibbonStatusBar = DYNAMIC_DOWNCAST(
            RibbonStatusBar, pStatusBar);
        if (pRibbonStatusBar != NULL) {
          bExtended = pRibbonStatusBar->GetExtendedArea(rectExt);
          bBottomFrame = pRibbonStatusBar->IsBottomFrame();
          bIsStatusBar = TRUE;
        }
      }
#endif // BCGP_EXCLUDE_RIBBON
    }
    if (bIsStatusBar) {
      IRECT rectStatus;
      pStatusBar->GetClientRect(rectStatus);
      int nHeight = rectStatus.Height();
      rectStatus.b = rtWindow.b;
      rectStatus.t = rectStatus.b - nHeight - (bBottomFrame ? -1 : szSysBorder.h);
      rectStatus.l = rtWindow.l;
      rectStatus.r = rtWindow.r;
      if (bExtended) {
        rectExt.l = rectStatus.r - rectExt.Width() - szSysBorder.w;
        rectExt.t = rectStatus.t;
        rectExt.b = rectStatus.b;
        rectExt.r = rtWindow.r;
      }
      s->m_ctrlStatusBarBack.Draw(&dc, rectStatus, bActive ? 0 : 1);
      if (bExtended) {
        rectExt.l -= s->m_ctrlStatusBarBack_Ext.GetParams().s->m_rectCorners.l;
        s->m_ctrlStatusBarBack_Ext.Draw(&dc, rectExt, bActive ? 0 : 1);
      }
    }
    dc.SelectClipRgn(NULL);
    return TRUE;
  }
  return VisualManager2003_OnNcPaint(pWnd, lstSysButtons, rectRedraw);
}
BOOL VisualManager2007_OnSetWindowRegion(CWnd* pWnd, CSize sizeWindow)
{
  ASSERT_VALID(pWnd);
  if (pWnd->GetSafeHwnd() == NULL) {
    return FALSE;
  }
  if (!CanDrawImage()) {
    return FALSE;
  }
  if (s->DwmIsCompositionEnabled()) {
    return FALSE;
  }
  CSize sz(0, 0);
  BOOL bMainWnd = FALSE;
  if (DYNAMIC_DOWNCAST(PopupMenu, pWnd) != NULL) {
    sz = CSize(3, 3);
  }
#ifndef BCGP_EXCLUDE_RIBBON
  else if (DYNAMIC_DOWNCAST(RibbonBar, pWnd) != NULL) {
    return FALSE;
  }
#endif
  else {
    if (pWnd->IsZoomed()) {
      pWnd->SetWindowRgn(NULL, TRUE);
      return TRUE;
    }
    sz = CSize(9, 9);
    bMainWnd = TRUE;
  }
  if (sz != CSize(0, 0)) {
    CRgn rgn;
    BOOL bCreated = FALSE;
    bCreated = rgn.CreateRoundRectRgn(0, 0, sizeWindow.w + 1, sizeWindow.h + 1, sz.w, sz.h);
    if (bCreated) {
      if (pWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)) ||
          pWnd->IsKindOf(RUNTIME_CLASS(Dialog)) ||
          pWnd->IsKindOf(RUNTIME_CLASS(PropertySheet))) {
        CRgn rgnWinodw;
        rgnWinodw.CreateRectRgn(0, sz.h, sizeWindow.w, sizeWindow.h);
        rgn.CombineRgn(&rgn, &rgnWinodw, RGN_OR);
      }
      pWnd->SetWindowRgn((HRGN)rgn.Detach(), TRUE);
      return TRUE;
    }
  }
  return FALSE;
}
CSize VisualManager2007_GetNcBtnSize(BOOL bSmall) const
{
  return s->m_szNcBtnSize[bSmall ? 1 : 0];
}
int VisualManager2007_DrawSeparator(img_t* im, const IRECT& rect, BOOL bHorz)
{
  DrawSeparator(im, rect, s->m_penSeparator, s->m_penSeparator2, bHorz);
}
int VisualManager2007_DrawSeparator(img_t* im, const IRECT& rect, CPen& pen1, CPen& pen2, BOOL bHorz)
{
  IRECT rect1(rect);
  IRECT rect2;
  if (bHorz) {
    rect1.t += rect.Height() / 2 - 1;
    rect1.b = rect1.t;
    rect2 = rect1;
    rect2.OffsetRect(0, 1);
  }
  else {
    rect1.l += rect.Width() / 2 - 1;
    rect1.r = rect1.l;
    rect2 = rect1;
    rect2.OffsetRect(1, 0);
  }
  if (ToolBarImages::s->m_bIsDrawOnGlass) {
    DrawManager dm(*im);
    LOGPEN logpen;
    pen1.GetLogPen(&logpen);
    dm.DrawLine(rect1.l, rect1.t, rect1.r, rect1.b, logpen.lopnColor);
    pen2.GetLogPen(&logpen);
    dm.DrawLine(rect2.l, rect2.t, rect2.r, rect2.b, logpen.lopnColor);
  }
  else {
    CPen* pOldPen = im->SelectObject(&pen1);
    im->MoveTo(rect1.TopLeft());
    im->LineTo(rect1.BottomRight());
    im->SelectObject(&pen2);
    im->MoveTo(rect2.TopLeft());
    im->LineTo(rect2.BottomRight());
    im->SelectObject(pOldPen);
  }
}
COLOR VisualManager2007_GetCaptionBarTextColor(CaptionBar* pBar)
{
  if (!CanDrawImage()) {
    return VisualManager2003_GetCaptionBarTextColor(pBar);
  }
  return s->m_clrCaptionBarText;
}
int VisualManager2007_OnDrawCaptionBarInfoArea(img_t* im, CaptionBar* pBar, IRECT rect)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawCaptionBarInfoArea(im, pBar, rect);
    return 0;
  }
  DrawManager dm(*im);
  dm.FillGradient(rect, s->clrBarFace, _RGB(255, 255, 255));
  imdraw_Draw3dRect(im, 0, rect, s->clrBarDkShadow, s->clrBarDkShadow);
}
int VisualManager2007_OnFillOutlookPageButton(img_t* im,
    const IRECT& rect,
    BOOL bIsHighlighted, BOOL bIsPressed,
    COLOR& clrText)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnFillOutlookPageButton(im, rect, bIsHighlighted, bIsPressed, clrText);
    return 0;
  }
  IRECT rt(rect);
  if (s->m_ctrlOutlookWndPageBtn.IsValid()) {
    int index = 0;
    if (bIsPressed) {
      index = 2;
      if (bIsHighlighted) {
        index = 3;
      }
      clrText = s->m_clrOutlookPageTextPressed;
    }
    else if (bIsHighlighted) {
      index = 1;
      clrText = s->m_clrOutlookPageTextHighlighted;
    }
    s->m_ctrlOutlookWndPageBtn.Draw(im, rt, index);
  }
  else {
    COLOR clr1 = s->m_clrBarGradientDark;
    COLOR clr2 = s->m_clrBarGradientLight;
    if (bIsPressed) {
      if (bIsHighlighted) {
        clr1 = s->m_clrHighlightDnGradientDark;
        clr2 = s->m_clrHighlightDnGradientLight;
      }
      else {
        clr1 = s->m_clrHighlightCheckedGradientLight;
        clr2 = s->m_clrHighlightCheckedGradientDark;
      }
    }
    else if (bIsHighlighted) {
      clr1 = s->m_clrHighlightGradientDark;
      clr2 = s->m_clrHighlightGradientLight;
    }
    DrawManager dm(*im);
    dm.FillGradient(rect, clr1, clr2, TRUE);
  }
  clrText = s->m_clrOutlookPageTextNormal;
  if (bIsPressed) {
    clrText = s->m_clrOutlookPageTextPressed;
  }
  else if (bIsHighlighted) {
    clrText = s->m_clrOutlookPageTextHighlighted;
  }
}
int VisualManager2007_OnDrawOutlookPageButtonBorder(
    img_t* im, IRECT& rectBtn, BOOL bIsHighlighted, BOOL bIsPressed)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawOutlookPageButtonBorder(im, rectBtn, bIsHighlighted, bIsPressed);
    return 0;
  }
  imdraw_Draw3dRect(im, 0, rectBtn, s->clrBtnHilite, s->m_clrToolBarBottomLine);
}
int VisualManager2007_OnDrawOutlookBarSplitter(img_t* im, IRECT rectSplitter)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawOutlookBarSplitter(im, rectSplitter);
    return 0;
  }
  DrawManager dm(*im);
  dm.FillGradient(rectSplitter,
      s->m_clrCaptionBarGradientDark,
      s->m_clrCaptionBarGradientLight,
      TRUE);
  rectSplitter.OffsetRect(0, 1);
  s->m_ToolBarTear.DrawEx(im, rectSplitter, 0, ToolBarImages::ImageAlignHorzCenter, ImageAlignVertCenter);
  rectSplitter.OffsetRect(0, -1);
  CPen* pOldPen = im->SelectObject(&s->m_penBottomLine);
  im->MoveTo(rectSplitter.l, rectSplitter.t);
  im->LineTo(rectSplitter.r, rectSplitter.t);
  im->MoveTo(rectSplitter.l, rectSplitter.b - 1);
  im->LineTo(rectSplitter.r, rectSplitter.b - 1);
  im->SelectObject(pOldPen);
}
int VisualManager2007_OnFillOutlookBarCaption(img_t* im, IRECT rectCaption, COLOR& clrText)
{
  VisualManager2003_OnFillOutlookBarCaption(im, rectCaption, clrText);
  if (CanDrawImage()) {
    clrText = s->m_clrOutlookCaptionTextNormal;
  }
}
int VisualManager2007_OnFillBarBackground(img_t* im, BaseControlBar* pBar,
    IRECT rectClient, IRECT rectClip,
    BOOL bNCArea/* = FALSE*/)
{
  ASSERT_VALID(pBar);
  if (pBar->IsOnGlass()) {
    im->FillSolidRect(rectClient, _RGB(0, 0, 0));
    return 0;
  }
  CRuntimeClass* pBarClass = pBar->GetRuntimeClass();
  if (!CanDrawImage() ||
      pBar->IsDialogControl() ||
      pBarClass->IsDerivedFrom(RUNTIME_CLASS(ColorBar)) ||
      pBarClass->IsDerivedFrom(RUNTIME_CLASS(Calculator)) ||
      pBarClass->IsDerivedFrom(RUNTIME_CLASS(CalendarBar))) {
    VisualManager2003_OnFillBarBackground(im, pBar, rectClient, rectClip, bNCArea);
    return 0;
  }
  if (pBar->IsKindOf(RUNTIME_CLASS(MenuBar))) {
    BOOL bIsHorz = (pBar->GetBarStyle() & CBRS_ORIENT_HORZ);
    COLOR clr1 = bIsHorz ? s->m_clrMenuBarGradientDark : s->m_clrMenuBarGradientVertLight;
    COLOR clr2 = bIsHorz ? s->m_clrMenuBarGradientLight : s->m_clrMenuBarGradientVertDark;
    DrawManager dm(*im);
    dm.Fill4ColorsGradient(rectClient, clr1, clr2, clr2, clr1, !bIsHorz);
    return 0;
  }
  else if (pBar->IsKindOf(RUNTIME_CLASS(PopupMenuBar))) {
    imdraw_FillRect(im, 0, rectClip, s->m_clrMenuLight);
    PopupMenuBar* pMenuBar = DYNAMIC_DOWNCAST(PopupMenuBar, pBar);
    if (!pMenuBar->s->m_bDisableSideBarInXPMode) {
      IRECT rectImages = rectClient;
      rectImages.r = rectImages.l + pMenuBar->GetGutterWidth();
      rectImages.DeflateRect(0, 1);
      imdraw_FillRect(im, 0, rectImages, s->m_clrBarBkgnd);
      rectImages.l = rectImages.r;
      rectImages.r += 2;
      DrawSeparator(im, rectImages, FALSE);
    }
    return 0;
  }
  else if (pBar->IsKindOf(RUNTIME_CLASS(StatusBar))) {
    CSize szSysBorder(GetSystemBorders(TRUE));
    IRECT rect(rectClient);
    IRECT rectExt(0, 0, 0, 0);
    BOOL bExtended = ((StatusBar*)pBar)->GetExtendedArea(rectExt);
    //IRECT rectBorders = s->m_ctrlStatusBarBack.GetParams().s->m_rectSides;
    if (bExtended) {
      rect.r = rectExt.l;
    }
    CWnd* pWnd = ((StatusBar*)pBar)->GetParent();
    ASSERT_VALID(pWnd);
    BOOL bActive = IsWindowActive(pWnd);
    rect.InflateRect(szSysBorder.w, 0, szSysBorder.w, szSysBorder.h);
    s->m_ctrlStatusBarBack.Draw(im, rect, bActive ? 0 : 1);
    if (bExtended) {
      rectExt.InflateRect(0, 0, szSysBorder.w, szSysBorder.h);
      rectExt.l -= s->m_ctrlStatusBarBack_Ext.GetParams().s->m_rectCorners.l;
      s->m_ctrlStatusBarBack_Ext.Draw(im, rectExt, bActive ? 0 : 1);
    }
    return 0;
  }
#ifndef BCGP_EXCLUDE_RIBBON
  else if (pBar->IsKindOf(RUNTIME_CLASS(RibbonStatusBar))) {
    RibbonStatusBar* pRibbonStatusBar = DYNAMIC_DOWNCAST(
        RibbonStatusBar, pBar);
    CSize szSysBorder(GetSystemBorders(TRUE));
    IRECT rect(rectClient);
    IRECT rectExt(0, 0, 0, 0);
    BOOL bExtended = pRibbonStatusBar->GetExtendedArea(rectExt);
    BOOL bBottomFrame = pRibbonStatusBar->IsBottomFrame();
    //IRECT rectBorders = s->m_ctrlStatusBarBack.GetParams().s->m_rectSides;
    if (bExtended) {
      rect.r = rectExt.l;
    }
    CWnd* pWnd = pBar->GetParent();
    ASSERT_VALID(pWnd);
    BOOL bActive = IsWindowActive(pWnd);
    rect.InflateRect(szSysBorder.w, 0, szSysBorder.w, bBottomFrame ? -1 : szSysBorder.h);
    s->m_ctrlStatusBarBack.Draw(im, rect, bActive ? 0 : 1);
    if (bExtended) {
      rectExt.InflateRect(0, 0, szSysBorder.w, bBottomFrame ? -1 : szSysBorder.h);
      rectExt.l -= s->m_ctrlStatusBarBack_Ext.GetParams().s->m_rectCorners.l;
      s->m_ctrlStatusBarBack_Ext.Draw(im, rectExt, bActive ? 0 : 1);
    }
    return 0;
  }
#endif
  else if (pBar->IsKindOf(RUNTIME_CLASS(OutlookBarToolBar))) {
    if (s->m_ctrlOutlookWndBar.IsValid()) {
      s->m_ctrlOutlookWndBar.Draw(im, rectClient);
    }
    else {
      DrawManager dm(*im);
      dm.FillGradient(rectClient, s->m_clrToolBarGradientDark,
          s->m_clrToolBarGradientLight,
          TRUE);
    }
    return 0;
  }
#ifndef BCGP_EXCLUDE_TOOLBOX
  BOOL bIsToolBox = pBar->IsKindOf(RUNTIME_CLASS(ToolBoxPage)) ||
      pBar->IsKindOf(RUNTIME_CLASS(ToolBox)) ||
      pBar->IsKindOf(RUNTIME_CLASS(ToolBoxEx));
  if (bIsToolBox) {
    DrawManager dm(*im);
    dm.FillGradient(rectClient, s->m_clrToolBarGradientLight,
        s->m_clrToolBarGradientDark,
        FALSE);
    return 0;
  }
#endif
  VisualManager2003_OnFillBarBackground(im, pBar, rectClient, rectClip, bNCArea);
}
int VisualManager2007_OnFillHighlightedArea(img_t* im, IRECT rect,
    COLOR pBrush, ToolbarButton* pButton)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnFillHighlightedArea(im, rect, pBrush, pButton);
    return 0;
  }
  BOOL bIsHorz = TRUE;
  // BOOL bIsPopupMenu = FALSE;
  COLOR clr1 = (COLOR) - 1;
  COLOR clr2 = (COLOR) - 1;
  if (pButton != NULL) {
    bIsHorz = pButton->IsHorizontal();
    ToolbarMenuButton* pCustButton =
        DYNAMIC_DOWNCAST(CCustomizeButton, pButton);
    if (pCustButton != NULL) {
      if (pButton->IsDroppedDown()) {
        clr1 = s->m_clrHighlightDnGradientDark;
        clr2 = s->m_clrHighlightDnGradientLight;
      }
    }
  }
  if (pBrush == s->m_clrHighlight) {
    clr1 = s->m_clrHighlightGradientDark;
    clr2 = s->m_clrHighlightGradientLight;//bIsPopupMenu ? clr1 : s->m_clrHighlightGradientLight;
  }
  else if (pBrush == s->m_clrHighlightDn) {
    clr1 = s->m_clrHighlightDnGradientDark;//bIsPopupMenu ? s->m_clrHighlightDnGradientLight : s->m_clrHighlightDnGradientDark;
    clr2 = s->m_clrHighlightDnGradientLight;
  }
  else if (pBrush == s->m_clrHighlightChecked) {
    clr1 = s->m_clrHighlightCheckedGradientDark;//bIsPopupMenu ? s->m_clrHighlightCheckedGradientLight : s->m_clrHighlightCheckedGradientDark;
    clr2 = s->m_clrHighlightCheckedGradientLight;
  }
  if (clr1 == (COLOR) - 1 || clr2 == (COLOR) - 1) {
    VisualManager2003_OnFillHighlightedArea(im, rect, pBrush, pButton);
    return 0;
  }
  DrawManager dm(*im);
  dm.FillGradient(rect, clr1, clr2, bIsHorz);
}
int VisualManager2007_OnDrawMenuBorder(img_t* im, PopupMenu* pMenu, IRECT rect)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawMenuBorder(im, pMenu, rect);
    return 0;
  }
#ifndef BCGP_EXCLUDE_RIBBON
  if (pMenu != NULL) {
    RibbonPanelMenuBar* pRibbonMenuBar =
        DYNAMIC_DOWNCAST(RibbonPanelMenuBar, pMenu->GetMenuBar());
    if (pRibbonMenuBar != NULL) {
      ASSERT_VALID(pRibbonMenuBar);
      if (pRibbonMenuBar->IsMainPanel()) {
        if (s->m_ctrlRibbonMainPanel.IsValid()) {
          s->m_ctrlRibbonMainPanel.DrawFrame(im, rect);
        }
        else {
          s->m_ctrlPopupBorder.DrawFrame(im, rect);
        }
        return 0;
      }
      if (!pRibbonMenuBar->IsMenuMode()) {
        if (pRibbonMenuBar->IsQATPopup() &&
            s->m_ctrlRibbonBorder_QAT.IsValid()) {
          s->m_ctrlRibbonBorder_QAT.DrawFrame(im, rect);
          return 0;
        }
        else if (pRibbonMenuBar->IsCategoryPopup()) {
          if (IsBeta1()) {
            s->m_ctrlRibbonCategoryBack.DrawFrame(im, rect);
          }
          return 0;
        }
        else if (pRibbonMenuBar->IsFloaty() &&
            s->m_ctrlRibbonBorder_Floaty.IsValid()) {
          s->m_ctrlRibbonBorder_Floaty.DrawFrame(im, rect);
          return 0;
        }
        else {
          if (pRibbonMenuBar->GetPanel() != NULL) {
            if (IsBeta1()) {
              s->m_ctrlRibbonCategoryBack.DrawFrame(im, rect);
            }
            return 0;
          }
          // draw standard
        }
      }
    }
  }
#endif
  BaseControlBar* pTopLevelBar = NULL;
  for (PopupMenu* pParentMenu = pMenu;
      pParentMenu != NULL; pParentMenu = pParentMenu->GetParentPopupMenu()) {
    ToolbarMenuButton* pParentButton = pParentMenu->GetParentButton();
    if (pParentButton == NULL) {
      break;
    }
    pTopLevelBar =
        DYNAMIC_DOWNCAST(BaseControlBar, pParentButton->GetParentWnd());
  }
  if (pTopLevelBar == NULL || pTopLevelBar->IsKindOf(RUNTIME_CLASS(PopupMenuBar))) {
    s->m_ctrlPopupBorder.DrawFrame(im, rect);
  }
  else {
    VisualManager2003_OnDrawMenuBorder(im, pMenu, rect);
  }
}
int VisualManager2007_OnDrawBarGripper(img_t* im, IRECT rectGripper, BOOL bHorz, BaseControlBar* pBar)
{
  if (!CanDrawImage() ||
      (pBar != NULL && pBar->IsDialogControl()) ||
      s->m_ToolBarGripper.GetCount() == 0) {
    VisualManager2003_OnDrawBarGripper(im, rectGripper, bHorz, pBar);
    return 0;
  }
  CSize szBox(s->m_ToolBarGripper.GetImageSize());
  if (szBox != CSize(0, 0)) {
    if (bHorz) {
      rectGripper.l = rectGripper.r - szBox.w;
    }
    else {
      rectGripper.t = rectGripper.b - szBox.h;
    }
    ToolBar* pToolBar = DYNAMIC_DOWNCAST(ToolBar, pBar);
    if (pToolBar != NULL) {
      if (bHorz) {
        const int nHeight = ToolBar::IsLargeIcons() ?
            pToolBar->GetRowHeight() : pToolBar->GetButtonSize().h;
        const int nDelta = max(0, (nHeight - pToolBar->GetImageSize().h) / 2);
        rectGripper.DeflateRect(0, nDelta);
      }
      else {
        const int nWidth = ToolBar::IsLargeIcons() ?
            pToolBar->GetColumnWidth() : pToolBar->GetButtonSize().w;
        const int nDelta = max(0, (nWidth - pToolBar->GetImageSize().w) / 2);
        rectGripper.DeflateRect(nDelta, 0);
      }
    }
    const int nBoxesNumber = bHorz ?
        (rectGripper.Height() - szBox.h) / szBox.h :
        (rectGripper.Width() - szBox.w) / szBox.w;
    int nOffset = bHorz ?
        (rectGripper.Height() - nBoxesNumber * szBox.h) / 2 :
        (rectGripper.Width() - nBoxesNumber * szBox.w) / 2;
    for (int nBox = 0; nBox < nBoxesNumber; nBox++) {
      int x = bHorz ?
          rectGripper.l :
          rectGripper.l + nOffset;
      int y = bHorz ?
          rectGripper.t + nOffset :
          rectGripper.t;
      s->m_ToolBarGripper.DrawEx(im, IRECT(CPoint(x, y), szBox), 0);
      nOffset += bHorz ? szBox.h : szBox.w;
    }
  }
}
int VisualManager2007_OnDrawSeparator(img_t* im, BaseControlBar* pBar, IRECT rect, BOOL bHorz)
{
  if (!CanDrawImage() || pBar == NULL || pBar->IsDialogControl()) {
    VisualManager2003_OnDrawSeparator(im, pBar, rect, bHorz);
    return 0;
  }
  ASSERT_VALID(pBar);
  IRECT rectSeparator(rect);
#ifndef BCGP_EXCLUDE_RIBBON
  if (pBar->IsKindOf(RUNTIME_CLASS(RibbonStatusBar))) {
    RibbonStatusBar* pRibbonStatusBar = DYNAMIC_DOWNCAST(
        RibbonStatusBar, pBar);
    rect.InflateRect(1, 5, 1, pRibbonStatusBar->IsBottomFrame() ? 2 : 5);
    s->m_StatusBarPaneBorder.DrawEx(im, rect, 0, ToolBarImages::ImageAlignHorzRight,
        ImageAlignVertStretch);
    return 0;
  }
  if (pBar->IsKindOf(RUNTIME_CLASS(RibbonBar)) ||
      (bHorz && pBar->IsKindOf(RUNTIME_CLASS(RibbonPanelMenuBar)))) {
    if (rect.Width() < s->m_RibbonPanelSeparator.GetImageSize().w) {
      rect.l = rect.r - s->m_RibbonPanelSeparator.GetImageSize().w;
    }
    s->m_RibbonPanelSeparator.DrawEx(im, rect, 0, ToolBarImages::ImageAlignHorzCenter,
        ImageAlignVertCenter);
    return 0;
  }
#endif
  BOOL bPopupMenu = FALSE;
  if (!bHorz) {
    BOOL bIsRibbon = FALSE;
#ifndef BCGP_EXCLUDE_RIBBON
    bIsRibbon = pBar->IsKindOf(RUNTIME_CLASS(RibbonPanelMenuBar));
    if (bIsRibbon && ((RibbonPanelMenuBar*) pBar)->IsDefaultMenuLook()) {
      bIsRibbon = FALSE;
    }
#endif
    bPopupMenu = pBar->IsKindOf(RUNTIME_CLASS(PopupMenuBar));
    if (bPopupMenu &&
        !bIsRibbon &&
        !pBar->IsKindOf(RUNTIME_CLASS(ColorBar)) &&
        !pBar->IsKindOf(RUNTIME_CLASS(Calculator)) &&
        !pBar->IsKindOf(RUNTIME_CLASS(CalendarBar))) {
      rectSeparator.l = rect.l + ToolBar::GetMenuImageSize().w +
          GetMenuImageMargin() + 1;
      IRECT rectBar;
      pBar->GetClientRect(rectBar);
      if (rectBar.r - rectSeparator.r < 50) { // Last item in row
        rectSeparator.r = rectBar.r;
      }
      if (((PopupMenuBar*) pBar)->s->m_bDisableSideBarInXPMode) {
        rectSeparator.l = 0;
      }
      //---------------------------------
      // Maybe Quick Customize separator
      //---------------------------------
      if (bPopupMenu) {
        CWnd* pWnd = pBar->GetParent();
        if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(PopupMenu))) {
          PopupMenu* pMenu = (PopupMenu*)pWnd;
          if (pMenu->IsCustomizePane()) {
            rectSeparator.l = rect.l + 2 * ToolBar::GetMenuImageSize().w +
                3 * GetMenuImageMargin() + 2;
          }
        }
      }
    }
  }
  if (bPopupMenu) {
    DrawSeparator(im, rectSeparator, !bHorz);
  }
  else {
    if (bHorz) {
      int nHeight = rectSeparator.Height() / 5;
      rectSeparator.t += nHeight;
      rectSeparator.b -= nHeight;
    }
    else {
      int nWidth = rectSeparator.Width() / 5;
      rectSeparator.l += nWidth;
      rectSeparator.r -= nWidth;
    }
    DrawSeparator(im, rectSeparator, s->m_penSeparatorDark, s->m_penSeparatorLight, !bHorz);
  }
}
COLOR VisualManager2007_OnDrawControlBarCaption(img_t* im, DockingControlBar* pBar,
    BOOL bActive, IRECT rectCaption, IRECT rectButtons)
{
  if (!CanDrawImage() || pBar == NULL || pBar->IsDialogControl()) {
    return VisualManager2003_OnDrawControlBarCaption(im, pBar, bActive, rectCaption, rectButtons);
  }
  CPen pen(PS_SOLID, 1, s->clrBarFace);
  CPen* pOldPen = im->SelectObject(&pen);
  rectCaption.b += 2;
  im->MoveTo(rectCaption.l, rectCaption.b);
  im->LineTo(rectCaption.l, rectCaption.t);
  im->MoveTo(rectCaption.l + 1, rectCaption.t);
  im->LineTo(rectCaption.r - 1, rectCaption.t);
  im->MoveTo(rectCaption.r - 1, rectCaption.t + 1);
  im->LineTo(rectCaption.r - 1, rectCaption.b);
  im->SelectObject(pOldPen);
  rectCaption.DeflateRect(1, 1, 1, 0);
  imdraw_FillRect(im, 0, rectCaption, bActive ? &s->clrActiveCaption : &s->clrInactiveCaption);
  return bActive ? s->clrCaptionText : s->clrInactiveCaptionText;
}
int VisualManager2007_OnDrawStatusBarPaneBorder(img_t* im, StatusBar* pBar,
    IRECT rectPane, UINT uiID, UINT nStyle)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawStatusBarPaneBorder(im, pBar, rectPane,
        uiID, nStyle);
    return 0;
  }
  BOOL bExtended = pBar->GetDrawExtendedArea();
  if (!bExtended || ((nStyle & SBPS_STRETCH) == 0 && bExtended)) {
    rectPane.OffsetRect(1, 0);
    s->m_StatusBarPaneBorder.DrawEx(im, rectPane, 0, ToolBarImages::ImageAlignHorzRight,
        ImageAlignVertStretch);
  }
}
int VisualManager2007_OnDrawStatusBarSizeBox(img_t* im, StatusBar* pStatBar,
    IRECT rectSizeBox)
{
  if (!CanDrawImage() ||
      s->m_StatusBarSizeBox.GetCount() == 0) {
    VisualManager2003_OnDrawStatusBarSizeBox(im, pStatBar, rectSizeBox);
    return 0;
  }
  s->m_StatusBarSizeBox.DrawEx(im, rectSizeBox, 0, ToolBarImages::ImageAlignHorzRight, ImageAlignVertBottom);
}
int VisualManager2007_OnDrawComboDropButton(img_t* im, IRECT rect,
    BOOL bDisabled,
    BOOL bIsDropped,
    BOOL bIsHighlighted,
    ToolbarComboBoxButton* pButton)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawComboDropButton(im, rect,
        bDisabled, bIsDropped,
        bIsHighlighted, pButton);
    return 0;
  }
  BOOL bRibbon = pButton != NULL && pButton->IsRibbonButton();
  BOOL bActive = bIsHighlighted || bIsDropped;
  ControlRenderer* pRenderer = bRibbon
      ? &s->m_ctrlRibbonComboBoxBtn
      : &s->m_ctrlComboBoxBtn;
  if (!pRenderer->IsValid()) {
    COLOR color1 = bRibbon
        ? s->m_clrRibbonComboBtnStart
        : s->m_clrComboBtnStart;
    COLOR color2 = bRibbon
        ? s->m_clrRibbonComboBtnFinish
        : s->m_clrComboBtnFinish;
    COLOR colorBorder = bRibbon
        ? s->m_clrRibbonComboBtnBorder
        : s->m_clrComboBtnBorder;
    if (bDisabled) {
      color1 = bRibbon
          ? s->m_clrRibbonComboBtnDisabledStart
          : s->m_clrComboBtnDisabledStart;
      color2 = bRibbon
          ? s->m_clrRibbonComboBtnDisabledFinish
          : s->m_clrComboBtnDisabledFinish;
      colorBorder = bRibbon
          ? s->m_clrRibbonComboBtnBorderDisabled
          : s->m_clrComboBtnBorderDisabled;
    }
    else if (bActive) {
      if (bIsDropped) {
        color1 = bRibbon
            ? s->m_clrRibbonComboBtnPressedStart
            : s->m_clrComboBtnPressedStart;
        color2 = bRibbon
            ? s->m_clrRibbonComboBtnPressedFinish
            : s->m_clrComboBtnPressedFinish;
        colorBorder = bRibbon
            ? s->m_clrRibbonComboBtnBorderPressed
            : s->m_clrComboBtnBorderPressed;
      }
      else {
        color1 = bRibbon
            ? s->m_clrRibbonComboBtnHighlightedStart
            : s->m_clrComboBtnHighlightedStart;
        color2 = bRibbon
            ? s->m_clrRibbonComboBtnHighlightedFinish
            : s->m_clrComboBtnHighlightedFinish;
        colorBorder = bRibbon
            ? s->m_clrRibbonComboBtnBorderHighlighted
            : s->m_clrComboBtnBorderHighlighted;
      }
    }
    if (bRibbon ||
        !bDisabled ||
        (bDisabled && colorBorder != (COLOR)(-1))) {
      if (!bDisabled) {
        rect.InflateRect(0, 1, 1, 1);
      }
      if (ToolBarImages::s->m_bIsDrawOnGlass) {
        DrawManager dm(*im);
        dm.DrawRect(rect, (COLOR) - 1, colorBorder);
      }
      else {
        imdraw_Draw3dRect(im, 0, rect, colorBorder, colorBorder);
      }
      if (!bDisabled) {
        rect.DeflateRect(0, 1, 1, 1);
      }
    }
    if (bDisabled) {
      rect.DeflateRect(0, 1, 1, 1);
    }
    else if (bActive) {
      rect.DeflateRect(1, 0, 0, 0);
    }
    DrawManager dm(*im);
    dm.FillGradient(rect, color1, color2, TRUE);
    if (bDisabled) {
      rect.InflateRect(0, 1, 1, 1);
    }
    else if (bActive) {
      rect.InflateRect(1, 0, 0, 0);
    }
  }
  else {
    rect.InflateRect(0, 1, 1, 1);
    int nIndex = 0;
    if (bDisabled) {
      nIndex = 3;
    }
    else {
      if (bIsDropped) {
        nIndex = 2;
      }
      else if (bIsHighlighted) {
        nIndex = 1;
      }
    }
    pRenderer->Draw(im, rect, nIndex);
    rect.DeflateRect(0, 1, 1, 1);
  }
  rect.b -= 2;
  MenuImages::Draw(im, MenuImages::IdArowDown, rect,
      bDisabled
      ? MenuImages::ImageGray
      : MenuImages::ImageBlack);
}
int VisualManager2007_OnDrawComboBorder(img_t* im, IRECT rect,
    BOOL bDisabled,
    BOOL bIsDropped,
    BOOL bIsHighlighted,
    ToolbarComboBoxButton* pButton)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawComboBorder(im, rect,
        bDisabled,
        bIsDropped,
        bIsHighlighted,
        pButton);
    return 0;
  }
  rect.DeflateRect(1, 1);
  COLOR colorBorder = s->m_clrComboBorder;
  if (bDisabled) {
    colorBorder = s->m_clrComboBorderDisabled;
  }
  else if (bIsHighlighted || bIsDropped) {
    colorBorder = bIsDropped
        ? s->m_clrComboBorderPressed
        : s->m_clrComboBorderHighlighted;
  }
  if (ToolBarImages::s->m_bIsDrawOnGlass) {
    DrawManager dm(*im);
    dm.DrawRect(rect, (COLOR) - 1, colorBorder);
  }
  else {
    imdraw_Draw3dRect(im, 0, &rect, colorBorder, colorBorder);
  }
}
int VisualManager2007_OnDrawEditBorder(img_t* im, IRECT rect,
    BOOL bDisabled,
    BOOL bIsHighlighted,
    ToolbarEditBoxButton* pButton)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawEditBorder(im, rect,
        bDisabled,
        bIsHighlighted,
        pButton);
    return 0;
  }
  rect.DeflateRect(1, 1);
  COLOR colorBorder = s->m_clrEditBorder;
  if (bDisabled) {
    colorBorder = s->m_clrEditBorderDisabled;
  }
  else if (bIsHighlighted) {
    colorBorder = s->m_clrEditBorderHighlighted;
  }
  if (ToolBarImages::s->m_bIsDrawOnGlass) {
    DrawManager dm(*im);
    dm.DrawRect(rect, (COLOR) - 1, colorBorder);
  }
  else {
    imdraw_Draw3dRect(im, 0, &rect, colorBorder, colorBorder);
  }
}
int VisualManager2007_OnDrawTearOffCaption(img_t* im, IRECT rect, BOOL bIsActive)
{
  if (!CanDrawImage() ||
      s->m_ToolBarTear.GetCount() == 0) {
    VisualManager2003_OnDrawTearOffCaption(im, rect, bIsActive);
    return 0;
  }
  imdraw_FillRect(im, 0, rect, s->m_clrBarBkgnd);
  if (bIsActive) {
    s->m_ctrlMenuHighlighted[0].Draw(im, rect);
  }
  s->m_ToolBarTear.DrawEx(im, rect, 0, ToolBarImages::ImageAlignHorzCenter, ImageAlignVertCenter);
}
int VisualManager2007_OnDrawMenuResizeBar(img_t* im, IRECT rect, int nResizeFlags)
{
  ToolBarImages& images =
      (nResizeFlags == (int) PopupMenu::MENU_RESIZE_BOTTOs->m_RIGHT) ?
      s->m_PopupResizeBar_HV :
      (nResizeFlags == (int) PopupMenu::MENU_RESIZE_TOP_RIGHT) ?
      s->m_PopupResizeBar_HVT :
      s->m_PopupResizeBar_V; // TODO - vertical resize
  if (!CanDrawImage() ||
      !s->m_ctrlPopupResizeBar.IsValid() ||
      !images.IsValid()) {
    VisualManager2003_OnDrawMenuResizeBar(im, rect, nResizeFlags);
    return 0;
  }
  s->m_ctrlPopupResizeBar.Draw(im, rect);
  if (nResizeFlags == (int) PopupMenu::MENU_RESIZE_BOTTOs->m_RIGHT ||
      nResizeFlags == (int) PopupMenu::MENU_RESIZE_TOP_RIGHT) {
    images.DrawEx(im, rect, 0, ToolBarImages::ImageAlignHorzRight,
        nResizeFlags == (int) PopupMenu::MENU_RESIZE_TOP_RIGHT ?
        ImageAlignVertTop :
        ImageAlignVertBottom);
  }
  else {
    images.DrawEx(im, rect, 0, ToolBarImages::ImageAlignHorzCenter,
        ImageAlignVertCenter);
  }
}
int VisualManager2007_OnDrawMenuScrollButton(img_t* im, IRECT rect, BOOL bIsScrollDown,
    BOOL bIsHighlited, BOOL bIsPressed,
    BOOL bIsDisabled)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawMenuScrollButton(im, rect, bIsScrollDown, bIsHighlited, bIsPressed, bIsDisabled);
    return 0;
  }
  ControlRenderer* pRenderer = &s->m_ctrlMenuScrollBtn[0];
  if (bIsScrollDown && s->m_ctrlMenuScrollBtn[1].IsValid()) {
    pRenderer = &s->m_ctrlMenuScrollBtn[1];
  }
  rect.t --;
  pRenderer->Draw(im, rect, bIsHighlited ? 1 : 0);
  MenuImages::Draw(im, bIsScrollDown ? MenuImages::IdArowDown : MenuImages::IdArowUp, rect);
}
int VisualManager2007_OnDrawMenuSystemButton(img_t* im, IRECT rect,
    UINT uiSystemCommand,
    UINT nStyle, BOOL bHighlight)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawMenuSystemButton(im, rect,
        uiSystemCommand, nStyle, bHighlight);
    return 0;
  }
  ToolBarImages* pImage = NULL;
  switch (uiSystemCommand) {
  case SC_CLOSE:
    pImage = &s->m_SysBtnClose[0];
    break;
  case SC_MINIMIZE:
    pImage = &s->m_SysBtnMinimize[0];
    break;
  case SC_RESTORE:
    pImage = &s->m_SysBtnRestore[0];
    break;
  default:
    return 0;
  }
  BOOL bDisabled = (nStyle & TBBS_DISABLED);
  BOOL bPressed = (nStyle & TBBS_PRESSED);
  IRECT rtBtnImage(CPoint(0, 0), pImage->GetImageSize());
  int nImage = 0;
  if (bDisabled) {
    nImage = 3;
  }
  else if (bPressed || bHighlight) {
    int index = -1;
    if (bPressed) {
      if (bHighlight) {
        index = 1;
      }
    }
    else if (bHighlight) {
      index = 0;
    }
    if (index != -1) {
      s->m_ctrlRibbonBtn[0].Draw(im, rect, index);
    }
  }
  rtBtnImage.OffsetRect(0, pImage->GetImageSize().h * nImage);
  pImage->DrawEx(im, rect, 0,
      ToolBarImages::ImageAlignHorzCenter, ImageAlignVertCenter,
      rtBtnImage);
}
int VisualManager2007_OnFillButtonInterior(img_t* im,
    ToolbarButton* pButton, IRECT rect, VisualManager::BCGBUTTON_STATE state)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnFillButtonInterior(im, pButton, rect, state);
    return 0;
  }
  CCustomizeButton* pCustButton = DYNAMIC_DOWNCAST(CCustomizeButton, pButton);
  if (pCustButton == NULL) {
    if (ToolBar::IsCustomizeMode() &&
        !ToolBar::IsAltCustomizeMode() && !pButton->IsLocked()) {
      return 0;
    }
    ControlRenderer* pRenderer = NULL;
    int index = 0;
    BOOL bDisabled = (pButton->s->m_nStyle & TBBS_DISABLED) == TBBS_DISABLED;
    BOOL bPressed = (pButton->s->m_nStyle & TBBS_PRESSED) == TBBS_PRESSED;
    BOOL bChecked = (pButton->s->m_nStyle & TBBS_CHECKED) == TBBS_CHECKED;
    BOOL bHandled = FALSE;
    BaseControlBar* pBar = DYNAMIC_DOWNCAST(BaseControlBar, pButton->GetParentWnd());
    ToolbarMenuButton* pMenuButton =
        DYNAMIC_DOWNCAST(ToolbarMenuButton, pButton);
    if (pMenuButton != NULL && pBar != NULL) {
      if (pBar->IsKindOf(RUNTIME_CLASS(MenuBar))) {
        if (state == ButtonsIsPressed || state == ButtonsIsHighlighted) {
          if (pMenuButton->IsDroppedDown()) {
            ExtendMenuButton(pMenuButton, rect);
            index = 1;
          }
          pRenderer = &s->m_ctrlMenuBarBtn;
          bHandled = TRUE;
        }
        else {
          return 0;
        }
        bHandled = TRUE;
      }
      else if (pBar->IsKindOf(RUNTIME_CLASS(PopupMenuBar))) {
        if (bChecked) {
          pRenderer = &s->m_ctrlMenuItemBack;
          if (bDisabled) {
            index = 1;
          }
          rect.InflateRect(0, 0, 0, 1);
          bHandled = TRUE;
        }
        else if (state == ButtonsIsPressed || state == ButtonsIsHighlighted) {
          pRenderer = &s->m_ctrlMenuHighlighted[bDisabled ? 1 : 0];
          bHandled = TRUE;
        }
        else {
          return 0;
        }
      }
      else if (pBar->IsKindOf(RUNTIME_CLASS(ToolBar))) {
        if (pMenuButton->IsDroppedDown()) {
          ExtendMenuButton(pMenuButton, rect);
        }
      }
    }
    else if (pBar != NULL && pBar->IsKindOf(RUNTIME_CLASS(ColorBar))) {
      if (bChecked) {
        pRenderer = &s->m_ctrlMenuItemBack;
        if (bDisabled) {
          index = 1;
        }
      }
      if (!bDisabled) {
        if (state == ButtonsIsHighlighted) {
          pRenderer = &s->m_ctrlMenuHighlighted[0];
          index = 0;
        }
      }
      bHandled = TRUE;
    }
    else if (pBar != NULL && pBar->IsKindOf(RUNTIME_CLASS(OutlookBarToolBar))) {
      bHandled = TRUE;
    }
    if (!bHandled) {
      index = -1;
      if (bChecked) {
        if (bDisabled) {
          index = 0;
        }
        else if (state == ButtonsIsPressed || state == ButtonsIsHighlighted) {
          index = 3;
        }
      }
      if (!bDisabled) {
        if (bPressed) {
          index = 2;
        }
        else if (state == ButtonsIsHighlighted) {
          if (index == -1) {
            index = 0;
          }
          index++;
        }
      }
      if (index == -1) {
        return 0;
      }
      pRenderer = &s->m_ctrlToolBarBtn;
    }
    if (pRenderer != NULL) {
      pRenderer->Draw(im, rect, index);
      return 0;
    }
  }
  VisualManager2003_OnFillButtonInterior(im, pButton, rect, state);
}
int VisualManager2007_OnDrawButtonBorder(img_t* im,
    ToolbarButton* pButton, IRECT rect, VisualManager::BCGBUTTON_STATE state)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawButtonBorder(im, pButton, rect, state);
    return 0;
  }
  //------------------------------------------------
  // Draw shadow under the dropped-down menu button:
  //------------------------------------------------
  if (state != ButtonsIsPressed && state != ButtonsIsHighlighted) {
    return 0;
  }
  if (!s->m_bShdowDroppedDownMenuButton ||
      !MenuBar::IsMenuShadows() ||
      ToolBar::IsCustomizeMode()) {
    return 0;
  }
  ToolbarMenuButton* pMenuButton = DYNAMIC_DOWNCAST(ToolbarMenuButton, pButton);
  if (pMenuButton == NULL || !pMenuButton->IsDroppedDown()) {
    return 0;
  }
  BOOL bIsPopupMenu =
      pMenuButton->GetParentWnd() != NULL &&
      pMenuButton->GetParentWnd()->IsKindOf(RUNTIME_CLASS(PopupMenuBar));
  if (bIsPopupMenu) {
    return 0;
  }
  PopupMenu* pPopupMenu = pMenuButton->GetPopupMenu();
  if (pPopupMenu != NULL &&
      (pPopupMenu->IsWindowVisible() || pPopupMenu->IsShown()) &&
      !pPopupMenu->IsRightAlign() &&
      !(pPopupMenu->GetExStyle() & WS_EX_LAYOUTRTL)) {
    ExtendMenuButton(pMenuButton, rect);
    DrawManager dm(*im);
    dm.DrawShadow(rect, s->m_nMenuShadowDepth, 100, 75, NULL, NULL,
        s->m_clrMenuShadowBase);
  }
}
int VisualManager2007_OnDrawButtonSeparator(img_t* im,
    ToolbarButton* pButton, IRECT rect, VisualManager::BCGBUTTON_STATE state,
    BOOL bHorz)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawButtonSeparator(im, pButton, rect, state, bHorz);
    return 0;
  }
  CPen* pPen = &s->m_penMenuItemBorder;
  CPen* pOldPen = im->SelectObject(pPen);
  ASSERT(pOldPen != NULL);
  if (bHorz) {
    im->MoveTo(rect.l, rect.t + 2);
    im->LineTo(rect.l, rect.b - 2);
  }
  else {
    im->MoveTo(rect.l + 2, rect.t);
    im->LineTo(rect.r - 2, rect.t);
  }
  im->SelectObject(pOldPen);
}
int VisualManager2007_OnHighlightMenuItem(CDC* im, ToolbarMenuButton* pButton,
    IRECT rect, COLOR& clrText)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnHighlightMenuItem(im, pButton, rect, clrText);
    return 0;
  }
  s->m_ctrlMenuHighlighted[(pButton->s->m_nStyle & TBBS_DISABLED) == TBBS_DISABLED ? 1 : 0].Draw(im, rect);
}
int VisualManager2007_OnHighlightRarelyUsedMenuItems(img_t* im, IRECT rectRarelyUsed)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnHighlightRarelyUsedMenuItems(im, rectRarelyUsed);
  }
  rectRarelyUsed.l --;
  rectRarelyUsed.r = rectRarelyUsed.l + ToolBar::GetMenuImageSize().w +
      2 * GetMenuImageMargin() + 2;
  imdraw_FillRect(im, 0, rectRarelyUsed, s->m_clrMenuRarelyUsed);
}
int VisualManager2007_OnDrawMenuCheck(img_t* im, ToolbarMenuButton* pButton,
    IRECT rect, BOOL bHighlight, BOOL bIsRadio)
{
  ToolBarImages& img = bIsRadio ? s->m_MenuItemMarkerR : s->m_MenuItemMarkerC;
  if (!CanDrawImage() || img.GetCount() == 0) {
    VisualManager2003_OnDrawMenuCheck(im, pButton, rect, bHighlight, bIsRadio);
    return 0;
  }
  CSize size(img.GetImageSize());
  IRECT rectImage(0, 0, size.w, size.h);
  if ((pButton->s->m_nStyle & TBBS_DISABLED) == TBBS_DISABLED) {
    rectImage.OffsetRect(0, size.h);
  }
  if (s->m_bIsRTL) {
    img.Mirror();
  }
  img.DrawEx(im, rect, 0, ToolBarImages::ImageAlignHorzCenter, ImageAlignVertCenter, rectImage);
  if (s->m_bIsRTL) {
    img.Mirror();
  }
}
int VisualManager2007_OnDrawMenuItemButton(img_t* im, ToolbarMenuButton* pButton,
    IRECT rectButton, BOOL bHighlight, BOOL bDisabled)
{
  if (!CanDrawImage() || !s->m_ctrlMenuButtonBorder.IsValid()) {
    VisualManager2003_OnDrawMenuItemButton(im, pButton,
        rectButton, bHighlight, bDisabled);
    return 0;
  }
  IRECT rect = rectButton;
  rect.r = rect.l + 1;
  rect.l--;
  rect.DeflateRect(0, 1);
  if (bHighlight) {
    s->m_ctrlMenuButtonBorder.Draw(im, rect);
  }
  else {
    CBrush br(s->clrBtnShadow);
    rect.DeflateRect(0, 3);
    rect.r--;
    imdraw_FillRect(im, 0, rect, &br);
  }
}
int VisualManager2007_OnDrawShowAllMenuItems(img_t* im, IRECT rect,
    VisualManager::BCGBUTTON_STATE state)
{
  if (!CanDrawImage() || !s->m_ctrlMenuItemShowAll.IsValid()) {
    VisualManager2003_OnDrawShowAllMenuItems(im, rect, state);
    return 0;
  }
  s->m_ctrlMenuItemShowAll.FillInterior(im, rect, ToolBarImages::ImageAlignHorzCenter,
      ImageAlignVertCenter, state == ButtonsIsHighlighted ? 1 : 0);
}
int VisualManager2007_GetShowAllMenuItemsHeight(img_t* im, const CSize& sizeDefault)
{
  return(CanDrawImage() && s->m_ctrlMenuItemShowAll.IsValid())
      ? s->m_ctrlMenuItemShowAll.GetParams().s->m_rectImage.Size().h + 2 * TEXT_MARGIN
      : VisualManager2003_GetShowAllMenuItemsHeight(im, sizeDefault);
}
COLOR VisualManager2007_OnFillMiniFrameCaption(img_t* im,
    IRECT rectCaption,
    MiniFrameWnd* pFrameWnd, BOOL bActive)
{
  if (!CanDrawImage()) {
    return VisualManager2003_OnFillMiniFrameCaption(im,
        rectCaption, pFrameWnd, bActive);
  }
  ASSERT_VALID(pFrameWnd);
  BOOL bIsToolBar = FALSE;
  if (DYNAMIC_DOWNCAST(BaseToolBar, pFrameWnd->GetControlBar()) != NULL) {
    bActive = FALSE;
    bIsToolBar = TRUE;
  }
  imdraw_FillRect(im, 0, rectCaption,
      bActive ? &s->clrActiveCaption : &s->clrInactiveCaption);
  // get the text color
  return bActive ? s->clrCaptionText : s->clrInactiveCaptionText;
}
int VisualManager2007_OnDrawMiniFrameBorder(
    img_t* im, MiniFrameWnd* pFrameWnd,
    IRECT rectBorder, IRECT rectBorderSize)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawMiniFrameBorder(
        im, pFrameWnd,
        rectBorder, rectBorderSize);
    return 0;
  }
  ASSERT_VALID(pFrameWnd);
#ifndef BCGP_EXCLUDE_TASK_PANE
  BOOL bIsTasksPane = pFrameWnd->IsKindOf(RUNTIME_CLASS(TaskPaneMiniFrameWnd));
#else
  BOOL bIsTasksPane = FALSE;
#endif
  if (bIsTasksPane) {
    COLOR pOldBrush = im->SelectObject(s->m_clrFloatToolBarBorder);
    ASSERT(pOldBrush != NULL);
    im->PatBlt(rectBorder.l, rectBorder.t, rectBorderSize.l, rectBorder.Height(), PATCOPY);
    im->PatBlt(rectBorder.l, rectBorder.t, rectBorder.Width(), rectBorderSize.t, PATCOPY);
    im->PatBlt(rectBorder.r - rectBorderSize.r, rectBorder.t, rectBorderSize.r, rectBorder.Height(), PATCOPY);
    im->PatBlt(rectBorder.l, rectBorder.b - rectBorderSize.b, rectBorder.Width(), rectBorderSize.b, PATCOPY);
    rectBorderSize.DeflateRect(2, 2);
    rectBorder.DeflateRect(2, 2);
    im->SelectObject(&s->clrBarFace);
    im->PatBlt(rectBorder.l, rectBorder.t + 1, rectBorderSize.l, rectBorder.Height() - 1, PATCOPY);
    im->PatBlt(rectBorder.l + 1, rectBorder.t, rectBorder.Width() - 2, rectBorderSize.t, PATCOPY);
    im->PatBlt(rectBorder.r - rectBorderSize.r, rectBorder.t + 1, rectBorderSize.r, rectBorder.Height() - 1, PATCOPY);
    im->PatBlt(rectBorder.l + 1, rectBorder.b - rectBorderSize.b, rectBorder.Width() - 2, rectBorderSize.b, PATCOPY);
    im->SelectObject(pOldBrush);
  }
  else {
    VisualManager2003_OnDrawMiniFrameBorder(im, pFrameWnd, rectBorder, rectBorderSize);
  }
}
int VisualManager2007_OnDrawFloatingToolbarBorder(
    img_t* im, BaseToolBar* pToolBar,
    IRECT rectBorder, IRECT rectBorderSize)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawFloatingToolbarBorder(im, pToolBar,
        rectBorder, rectBorderSize);
    return 0;
  }
  COLOR pOldBrush = im->SelectObject(s->m_clrFloatToolBarBorder);
  ASSERT(pOldBrush != NULL);
  im->PatBlt(rectBorder.l, rectBorder.t, rectBorderSize.l, rectBorder.Height(), PATCOPY);
  im->PatBlt(rectBorder.l, rectBorder.t, rectBorder.Width(), rectBorderSize.t, PATCOPY);
  im->PatBlt(rectBorder.r - rectBorderSize.r, rectBorder.t, rectBorderSize.r, rectBorder.Height(), PATCOPY);
  im->PatBlt(rectBorder.l, rectBorder.b - rectBorderSize.b, rectBorder.Width(), rectBorderSize.b, PATCOPY);
  rectBorderSize.DeflateRect(2, 2);
  rectBorder.DeflateRect(2, 2);
  im->SelectObject(&s->clrBarFace);
  im->PatBlt(rectBorder.l, rectBorder.t + 1, rectBorderSize.l, rectBorder.Height() - 1, PATCOPY);
  im->PatBlt(rectBorder.l + 1, rectBorder.t, rectBorder.Width() - 2, rectBorderSize.t, PATCOPY);
  im->PatBlt(rectBorder.r - rectBorderSize.r, rectBorder.t + 1, rectBorderSize.r, rectBorder.Height() - 1, PATCOPY);
  im->PatBlt(rectBorder.l + 1, rectBorder.b - rectBorderSize.b, rectBorder.Width() - 2, rectBorderSize.b, PATCOPY);
  im->SelectObject(pOldBrush);
}
BOOL VisualManager2007_IsOwnerDrawMenuCheck()
{
  return CanDrawImage() ? FALSE : VisualManager2003_IsOwnerDrawMenuCheck();
}
BOOL VisualManager2007_IsHighlightWholeMenuItem()
{
  return CanDrawImage() ? TRUE : VisualManager2003_IsHighlightWholeMenuItem();
}
COLOR VisualManager2007_GetStatusBarPaneTextColor(StatusBar* pStatusBar,
    CBCGStatusBarPaneInfo* pPane)
{
  if (!CanDrawImage()) {
    return VisualManager2003_GetStatusBarPaneTextColor(pStatusBar, pPane);
  }
  ASSERT(pPane != NULL);
  return(pPane->nStyle & SBPS_DISABLED) ? s->m_clrStatusBarTextDisabled :
      pPane->clrText == (COLOR) - 1 ? s->m_clrStatusBarText : pPane->clrText;
}
COLOR VisualManager2007_GetToolbarButtonTextColor(
    ToolbarButton* pButton, VisualManager::BCGBUTTON_STATE state)
{
  if (!CanDrawImage()) {
    return VisualManager2003_GetToolbarButtonTextColor(pButton, state);
  }
  BOOL bDisabled = (ToolBar::IsCustomizeMode() && !pButton->IsEditable()) ||
      (!ToolBar::IsCustomizeMode() && (pButton->s->m_nStyle & TBBS_DISABLED));
  if (pButton->GetParentWnd() != NULL &&
      pButton->GetParentWnd()->IsKindOf(RUNTIME_CLASS(MenuBar))) {
    if (ToolBar::IsCustomizeMode()) {
      return s->m_clrMenuBarBtnText;
    }
    return bDisabled
        ? s->m_clrMenuBarBtnTextDisabled
        : ((state == ButtonsIsHighlighted || state == ButtonsIsPressed ||
            pButton->IsDroppedDown())
            ? s->m_clrMenuBarBtnTextHighlighted
            : s->m_clrMenuBarBtnText);
  }
  return bDisabled
      ? s->m_clrToolBarBtnTextDisabled
      : ((state == ButtonsIsHighlighted || state == ButtonsIsPressed ||
          pButton->IsDroppedDown())
          ? s->m_clrToolBarBtnTextHighlighted
          : s->m_clrToolBarBtnText);
}
COLOR VisualManager2007_GetMenuItemTextColor(
    ToolbarMenuButton* pButton, BOOL bHighlighted, BOOL bDisabled)
{
  if (!CanDrawImage()) {
    return VisualManager2003_GetMenuItemTextColor(pButton,
        bHighlighted, bDisabled);
  }
  return bDisabled ? s->m_clrMenuTextDisabled : s->m_clrMenuText;
}
COLOR VisualManager2007_GetHighlightedMenuItemTextColor(ToolbarMenuButton* pButton)
{
  if (!CanDrawImage()) {
    return VisualManager2003_GetHighlightedMenuItemTextColor(pButton);
  }
  return s->m_clrMenuTextHighlighted;
}
int VisualManager2007_GetTabFrameColors(const BaseTabWnd* pTabWnd,
    COLOR& clrDark,
    COLOR& clrBlack,
    COLOR& clrHighlight,
    COLOR& clrFace,
    COLOR& clrDarkShadow,
    COLOR& clrLight,
    COLOR& pbrFace,
    COLOR& pbrBlack)
{
  if (!CanDrawImage()) {
    VisualManager2003_GetTabFrameColors(pTabWnd,
        clrDark, clrBlack, clrHighlight, clrFace,
        clrDarkShadow, clrLight, pbrFace, pbrBlack);
    return 0;
  }
  ASSERT_VALID(pTabWnd);
  VisualManager2003_GetTabFrameColors(pTabWnd,
      clrDark,
      clrBlack,
      clrHighlight,
      clrFace,
      clrDarkShadow,
      clrLight,
      pbrFace,
      pbrBlack);
  if (!pTabWnd->IsDialogControl()) {
    if (pTabWnd->IsFlatTab()) {
      if (s->m_clrTabFlatFace != CLR_DEFAULT) {
        clrFace = s->m_clrTabFlatFace;
      }
      if (s->m_clrTabFlatBlack != CLR_DEFAULT) {
        clrBlack = s->m_clrTabFlatBlack;
      }
      if (s->m_clrTabFlatDark != CLR_DEFAULT) {
        clrDark = s->m_clrTabFlatDark;
      }
      if (s->m_clrTabFlatDarkShadow != CLR_DEFAULT) {
        clrDarkShadow = s->m_clrTabFlatDarkShadow;
      }
      if (s->m_clrTabFlatLight != CLR_DEFAULT) {
        clrLight = s->m_clrTabFlatLight;
      }
      if (s->m_clrTabFlatHighlight != CLR_DEFAULT) {
        clrHighlight = s->m_clrTabFlatHighlight;
      }
    }
    else {
      if (s->m_clrTab3DFace != CLR_DEFAULT) {
        clrFace = s->m_clrTab3DFace;
      }
      if (s->m_clrTab3DBlack != CLR_DEFAULT) {
        clrBlack = s->m_clrTab3DBlack;
      }
      if (s->m_clrTab3DDark != CLR_DEFAULT) {
        clrDark = s->m_clrTab3DDark;
      }
      if (s->m_clrTab3DDarkShadow != CLR_DEFAULT) {
        clrDarkShadow = s->m_clrTab3DDarkShadow;
      }
      if (s->m_clrTab3DLight != CLR_DEFAULT) {
        clrLight = s->m_clrTab3DLight;
      }
      if (s->m_clrTab3DHighlight != CLR_DEFAULT) {
        clrHighlight = s->m_clrTab3DHighlight;
      }
    }
  }
}
int VisualManager2007_OnEraseTabsArea(img_t* im, IRECT rect,
    const BaseTabWnd* pTabWnd)
{
  ASSERT_VALID(pTabWnd);
  if (!CanDrawImage() || pTabWnd->IsDialogControl()) {
    VisualManager2003_OnEraseTabsArea(im, rect, pTabWnd);
    return 0;
  }
  if (pTabWnd->IsOneNoteStyle() ||
      pTabWnd->IsColored() ||
      pTabWnd->IsVS2005Style() ||
      pTabWnd->IsLeftRightRounded()) {
    VisualManager2003_OnEraseTabsArea(im, rect, pTabWnd);
    return 0;
  }
  const BOOL bBottom = pTabWnd->GetLocation() == TabWnd::LOCATION_BOTTOM;
  if (pTabWnd->IsFlatTab()) {
    s->m_ctrlTabFlat[bBottom ? 1 : 0].Draw(im, rect);
  }
  else {
    DrawManager dm(*im);
    COLOR clr1 = s->m_clrBarGradientDark;
    COLOR clr2 = s->m_clrBarGradientLight;
    if (bBottom) {
      dm.FillGradient(rect, clr1, clr2, TRUE);
    }
    else {
      dm.FillGradient(rect, clr2, clr1, TRUE);
    }
  }
}
int VisualManager2007_OnDrawTab(img_t* im, IRECT rectTab,
    int iTab, BOOL bIsActive, const BaseTabWnd* pTabWnd)
{
  ASSERT_VALID(pTabWnd);
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawTab(im, rectTab, iTab, bIsActive, pTabWnd);
    return 0;
  }
  if (pTabWnd->IsOneNoteStyle() ||
      pTabWnd->IsColored() ||
      pTabWnd->IsVS2005Style() ||
      pTabWnd->IsLeftRightRounded()) {
    VisualManager2003_OnDrawTab(im, rectTab, iTab, bIsActive, pTabWnd);
    return 0;
  }
  const BOOL bBottom = pTabWnd->GetLocation() == TabWnd::LOCATION_BOTTOM;
  const BOOL bIsHighlight = iTab == pTabWnd->GetHighlightedTab();
  COLOR clrText = pTabWnd->GetTabTextColor(iTab);
  if (pTabWnd->IsFlatTab()) {
    int nImage = (bIsActive || bIsHighlight) ? 2 : 1;
    CRgn rgn;
    IPOINT pts[4];
    if (bBottom) {
      rectTab.b++;
      pts[0].x = rectTab.l;
      pts[0].y = rectTab.b + 1;
      pts[1].x = rectTab.l;
      pts[1].y = rectTab.t;
      pts[2].x = rectTab.r + 1;
      pts[2].y = rectTab.t;
      pts[3].x = rectTab.r - rectTab.Height() + 1;
      pts[3].y = rectTab.b + 1;
      rectTab.t++;
    }
    else {
      pts[0].x = rectTab.l;
      pts[0].y = rectTab.b + 1;
      pts[1].x = rectTab.l;
      pts[1].y = rectTab.t;
      pts[2].x = rectTab.r - rectTab.Height() + 1;
      pts[2].y = rectTab.t;
      pts[3].x = rectTab.r + 1;
      pts[3].y = rectTab.b + 1;
    }
    rgn.CreatePolygonRgn(pts, 4, WINDING);
    int isave = im->SaveDC();
    im->SelectClipRgn(&rgn, RGN_AND);
    s->m_ctrlTabFlat[bBottom ? 1 : 0].Draw(im, rectTab, nImage);
    CPen* pOldPen = im->SelectObject(&s->m_penTabFlatOuter[bIsActive ? 1 : 0]);
    if (bBottom) {
      im->MoveTo(pts[2].x, pts[2].y);
      im->LineTo(pts[3].x, pts[3].y - 1);
    }
    else {
      im->MoveTo(pts[2].x - 1, pts[2].y);
      im->LineTo(pts[3].x - 1, pts[3].y - 1);
    }
    im->SelectObject(&s->m_penTabFlatInner[bIsActive ? 1 : 0]);
    if (bBottom) {
      im->MoveTo(pts[2].x - 2, pts[2].y + 1);
      im->LineTo(pts[3].x, pts[3].y - 2);
    }
    else {
      im->MoveTo(pts[2].x - 1, pts[2].y + 1);
      im->LineTo(pts[3].x - 2, pts[3].y - 1);
    }
    im->SelectObject(pOldPen);
    im->SelectClipRgn(NULL);
    clrText = s->clrBarText;
    im->RestoreDC(isave);
  }
  else {
    if (clrText == (COLOR) - 1) {
      clrText = bIsActive
          ? s->m_clrMenuBarBtnTextHighlighted
          : s->m_clrMenuBarBtnText;
    }
    int nImage = bIsActive ? 3 : 0;
    if (bIsHighlight) {
      nImage += 1;
    }
    s->m_ctrlTab3D[bBottom ? 1 : 0].Draw(im, rectTab, nImage);
    if (pTabWnd->IsDialogControl()) {
      clrText = s->clrBtnText;
    }
  }
  OnDrawTabContent(im, rectTab, iTab, bIsActive, pTabWnd, clrText);
}
int VisualManager2007_OnFillTab(img_t* im, IRECT rectFill, COLOR pbrFill,
    int iTab, BOOL bIsActive,
    const BaseTabWnd* pTabWnd)
{
  ASSERT_VALID(pTabWnd);
  if (!CanDrawImage() || pTabWnd->IsDialogControl()) {
    VisualManager2003_OnFillTab(im, rectFill, pbrFill,
        iTab, bIsActive, pTabWnd);
    return 0;
  }
  if (pTabWnd->IsFlatTab() ||
      pTabWnd->IsOneNoteStyle() ||
      pTabWnd->IsColored() ||
      pTabWnd->IsVS2005Style() ||
      pTabWnd->IsLeftRightRounded()) {
    VisualManager2003_OnFillTab(im, rectFill, pbrFill,
        iTab, bIsActive, pTabWnd);
    return 0;
  }
  const BOOL bBottom = pTabWnd->GetLocation() == TabWnd::LOCATION_BOTTOM;
  const BOOL bIsHighlight = iTab == pTabWnd->GetHighlightedTab();
  BOOL bIsBeta = IsBeta();
  if (!bIsBeta || (bIsActive || bIsHighlight)) {
    int nImage = bIsActive
        ? (bIsBeta ? 2 : 3)
          : (bIsBeta ? -1 : 0);
    if (bIsHighlight) {
      nImage += 1;
    }
    s->m_ctrlTab3D[bBottom ? 1 : 0].Draw(im, rectFill, nImage);
  }
}
COLOR VisualManager2007_GetTabTextColor(const BaseTabWnd* pTabWnd, int iTab, BOOL bIsActive)
{
  if (!CanDrawImage() || pTabWnd->IsDialogControl()) {
    return VisualManager2003_GetTabTextColor(pTabWnd, iTab, bIsActive);
  }
  ASSERT_VALID(pTabWnd);
  if (pTabWnd->IsOneNoteStyle() || pTabWnd->GetTabBkColor(iTab) != (COLOR) - 1) {
    return VisualManager2003_GetTabTextColor(pTabWnd, iTab, bIsActive);
  }
  return bIsActive ? s->m_clrTabTextActive : s->m_clrTabTextInactive;
}
int VisualManager2007_GetTabHorzMargin(const BaseTabWnd* pTabWnd)
{
  if (!CanDrawImage()) {
    return VisualManager2003_GetTabHorzMargin(pTabWnd);
  }
  ControlRenderer* pRenderer = pTabWnd->IsFlatTab()
      ? &s->m_ctrlTabFlat[0]
      : &s->m_ctrlTab3D[0];
  if (pTabWnd->IsOneNoteStyle() ||
      pTabWnd->IsColored() ||
      pTabWnd->IsVS2005Style() ||
      pTabWnd->IsLeftRightRounded() ||
      !pRenderer->IsValid()) {
    return VisualManager2003_GetTabHorzMargin(pTabWnd);
  }
  return pRenderer->GetParams().s->m_rectSides.r / 2;
}
BOOL VisualManager2007_OnEraseTabsFrame(img_t* im, IRECT rect, const BaseTabWnd* pTabWnd)
{
  ASSERT_VALID(pTabWnd);
  if (!CanDrawImage() || pTabWnd->IsDialogControl()) {
    return VisualManager2003_OnEraseTabsFrame(im, rect, pTabWnd);
  }
  if (pTabWnd->IsOneNoteStyle() ||
      pTabWnd->IsColored() ||
      pTabWnd->IsVS2005Style() ||
      pTabWnd->IsLeftRightRounded()) {
    return VisualManager2003_OnEraseTabsFrame(im, rect, pTabWnd);
  }
  if (pTabWnd->IsFlatTab()) {
    imdraw_FillRect(im, 0, rect, &s->clrWindow);
    if (pTabWnd->GetLocation() != TabWnd::LOCATION_BOTTOM) {
      CPen pen(PS_SOLID, 1, s->m_clrTabFlatBlack);
      CPen* pOldPen = im->SelectObject(&pen);
      im->MoveTo(rect.l, rect.t + pTabWnd->GetTabsHeight() + 1);
      im->LineTo(rect.r, rect.t + pTabWnd->GetTabsHeight() + 1);
      im->SelectObject(pOldPen);
    }
    return TRUE;
  }
  return FALSE;
}
int VisualManager2007_OnEraseTabsButton(img_t* im, IRECT rect,
    Button* pButton,
    BaseTabWnd* pBaseTab)
{
  ASSERT_VALID(pBaseTab);
  TabWnd* pWndTab = DYNAMIC_DOWNCAST(TabWnd, pBaseTab);
  if (!CanDrawImage() ||
      pWndTab == NULL ||
      pBaseTab->IsDialogControl()) {
    VisualManager2003_OnEraseTabsButton(im, rect, pButton, pBaseTab);
    return 0;
  }
  if (pBaseTab->IsFlatTab() ||
      pBaseTab->IsOneNoteStyle() ||
      pBaseTab->IsColored() ||
      pBaseTab->IsVS2005Style() ||
      pBaseTab->IsLeftRightRounded() ||
      (!pButton->IsPressed() && !pButton->IsHighlighted())) {
    VisualManager2003_OnEraseTabsButton(im, rect, pButton, pBaseTab);
    return 0;
  }
  CRgn rgn;
  rgn.CreateRectRgnIndirect(rect);
  im->SelectClipRgn(&rgn);
  IRECT rectTabs;
  pWndTab->GetClientRect(&rectTabs);
  IRECT rectTabArea;
  pWndTab->GetTabsRect(rectTabArea);
  if (pWndTab->GetLocation() == BaseTabWnd::LOCATION_BOTTOM) {
    rectTabs.t = rectTabArea.t;
  }
  else {
    rectTabs.b = rectTabArea.b;
  }
  pWndTab->MapWindowPoints(pButton, rectTabs);
  OnEraseTabsArea(im, rectTabs, pWndTab);
  im->SelectClipRgn(NULL);
  int index = pButton->IsPressed() ? 2 : 1;
  s->m_ctrlToolBarBtn.Draw(im, rect, index);
}
int VisualManager2007_OnDrawTabsButtonBorder(img_t* im, IRECT& rect,
    Button* pButton, UINT uiState,
    BaseTabWnd* pWndTab)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawTabsButtonBorder(im, rect,
        pButton, uiState,
        pWndTab);
  }
}
#ifndef BCGP_EXCLUDE_TASK_PANE
int VisualManager2007_OnDrawTasksGroupCaption(
    img_t* im, TasksGroup* pGroup,
    BOOL bIsHighlighted /*= FALSE*/, BOOL bIsSelected /*= FALSE*/,
    BOOL bCanCollapse /*= FALSE*/)
{
  ASSERT_VALID(pGroup);
  ASSERT_VALID(pGroup->s->m_pPage);
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawTasksGroupCaption(
        im, pGroup,
        bIsHighlighted, bIsSelected, bCanCollapse);
    return 0;
  }
#ifndef BCGP_EXCLUDE_TOOLBOX
  BOOL bIsToolBox = pGroup->s->m_pPage->s->m_pTaskPane != NULL &&
      (pGroup->s->m_pPage->s->m_pTaskPane->IsKindOf(RUNTIME_CLASS(ToolBoxEx)));
#else
  BOOL bIsToolBox = FALSE;
#endif
  IRECT rectGroup = pGroup->s->m_rect;
  // -----------------------
  // Draw caption background
  // -----------------------
  if (bIsToolBox) {
    IRECT rectFill = rectGroup;
    rectFill.DeflateRect(1, 1);
    rectFill.b--;
    COLOR clrGrdaient1 = PixelAlpha2(
        s->m_clrToolBarGradientDark, 105);
    COLOR clrGrdaient2 = PixelAlpha2(
        s->m_clrToolBarGradientDark, 120);
    DrawManager dm(*im);
    dm.FillGradient(rectFill, clrGrdaient1, clrGrdaient2, TRUE);
    CBrush brFillBottom(PixelAlpha2(s->m_clrToolBarGradientDark, 120));
    IRECT rectFillBottom = rectGroup;
    rectFillBottom.DeflateRect(1, 0);
    rectFillBottom.t = rectFillBottom.b - 1;
    imdraw_FillRect(im, 0, rectFillBottom, &brFillBottom);
    if (bCanCollapse) {
      //--------------------
      // Draw expanding box:
      //--------------------
      int nBoxSize = 9;
      int nBoxOffset = 6;
      if (s->GetRibbonImageScale() != 1.) {
        nBoxSize = (int)(.5 + nBoxSize * s->GetRibbonImageScale());
      }
      IRECT rectButton = rectFill;
      rectButton.l += nBoxOffset;
      rectButton.r = rectButton.l + nBoxSize;
      rectButton.t = rectButton.CenterPoint().y - nBoxSize / 2;
      rectButton.b = rectButton.t + nBoxSize;
      OnDrawExpandingBox(im, rectButton, !pGroup->s->m_bIsCollapsed,
          s->clrBarText);
      rectGroup.l = rectButton.r + nBoxOffset;
      bCanCollapse = FALSE;
    }
  }
  else {
    DrawManager dm(*im);
    if (pGroup->s->m_bIsSpecial) {
      if (IsBeta()) {
        dm.FillGradient(pGroup->s->m_rect,
            bIsHighlighted ? s->m_clrTaskPaneGroupCaptionHighSpecLight : s->m_clrTaskPaneGroupCaptionSpecLight,
            bIsHighlighted ? s->m_clrTaskPaneGroupCaptionHighSpecDark : s->m_clrTaskPaneGroupCaptionSpecDark,
            TRUE);
      }
      else {
        dm.Fill4ColorsGradient(pGroup->s->m_rect,
            bIsHighlighted ? s->m_clrTaskPaneGroupCaptionHighSpecDark : s->m_clrTaskPaneGroupCaptionSpecDark,
            bIsHighlighted ? s->m_clrTaskPaneGroupCaptionHighSpecLight : s->m_clrTaskPaneGroupCaptionSpecLight,
            bIsHighlighted ? s->m_clrTaskPaneGroupCaptionHighSpecLight : s->m_clrTaskPaneGroupCaptionSpecLight,
            bIsHighlighted ? s->m_clrTaskPaneGroupCaptionHighSpecDark : s->m_clrTaskPaneGroupCaptionSpecDark,
            FALSE);
      }
    }
    else {
      if (IsBeta()) {
        dm.FillGradient(pGroup->s->m_rect,
            bIsHighlighted ? s->m_clrTaskPaneGroupCaptionHighLight : s->m_clrTaskPaneGroupCaptionLight,
            bIsHighlighted ? s->m_clrTaskPaneGroupCaptionHighDark : s->m_clrTaskPaneGroupCaptionDark,
            TRUE);
      }
      else {
        dm.Fill4ColorsGradient(pGroup->s->m_rect,
            bIsHighlighted ? s->m_clrTaskPaneGroupCaptionHighDark : s->m_clrTaskPaneGroupCaptionDark,
            bIsHighlighted ? s->m_clrTaskPaneGroupCaptionHighLight : s->m_clrTaskPaneGroupCaptionLight,
            bIsHighlighted ? s->m_clrTaskPaneGroupCaptionHighLight : s->m_clrTaskPaneGroupCaptionLight,
            bIsHighlighted ? s->m_clrTaskPaneGroupCaptionHighDark : s->m_clrTaskPaneGroupCaptionDark,
            FALSE);
      }
    }
  }
  //-------------
  // Draw border:
  //-------------
  IRECT rectBorder = pGroup->s->m_rect;
  rectBorder.b++;
  imdraw_Draw3dRect(im, 0, rectBorder, s->m_clrTaskPaneGroupBorder, s->m_clrTaskPaneGroupBorder);
  // ---------------------------
  // Draw an icon if it presents
  // ---------------------------
  BOOL bShowIcon = (pGroup->s->m_hIcon != NULL
      && pGroup->s->m_sizeIcon.w < rectGroup.Width() - rectGroup.Height());
  if (bShowIcon) {
    OnDrawTasksGroupIcon(im, pGroup, 5, bIsHighlighted, bIsSelected, bCanCollapse);
  }
  // -----------------------
  // Draw group caption text
  // -----------------------
  CFont* pFontOld = im->SelectObject(s->fontBold);
  COLOR clrTextOld = im->GetTextColor();
  if (bIsToolBox) {
    im->SetTextColor(s->clrBarText);
  }
  else {
    if (bCanCollapse && bIsHighlighted) {
      im->SetTextColor(pGroup->s->m_clrTextHot == (COLOR) - 1 ?
          (pGroup->s->m_bIsSpecial ? s->m_clrTaskPaneGroupCaptionTextHighSpec : s->m_clrTaskPaneGroupCaptionTextHigh) :
            pGroup->s->m_clrTextHot);
    }
    else {
      im->SetTextColor(pGroup->s->m_clrText == (COLOR) - 1 ?
          (pGroup->s->m_bIsSpecial ? s->m_clrTaskPaneGroupCaptionTextSpec : s->m_clrTaskPaneGroupCaptionText) :
            pGroup->s->m_clrText);
    }
  }
  int nBkModeOld = im->SetBkMode(TRANSPARENT);
  int nTaskPaneHOffset = pGroup->s->m_pPage->s->m_pTaskPane->GetGroupCaptionHorzOffset();
  int nTaskPaneVOffset = pGroup->s->m_pPage->s->m_pTaskPane->GetGroupCaptionVertOffset();
  int nCaptionHOffset = (nTaskPaneHOffset != -1 ? nTaskPaneHOffset : s->m_nGroupCaptionHorzOffset);
  IRECT rectText = rectGroup;
  rectText.l += (bShowIcon ? pGroup->s->m_sizeIcon.w + 5 : nCaptionHOffset) + 5;
  rectText.t += (nTaskPaneVOffset != -1 ? nTaskPaneVOffset : s->m_nGroupCaptionVertOffset);
  rectText.r = max(rectText.l,
      rectText.r - (bCanCollapse ? rectGroup.Height() : nCaptionHOffset));
  im->DrawText(pGroup->s->m_strName, rectText, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
  im->SetBkMode(nBkModeOld);
  im->SelectObject(pFontOld);
  im->SetTextColor(clrTextOld);
  // -------------------------
  // Draw group caption button
  // -------------------------
  if (bCanCollapse && !pGroup->s->m_strName.IsEmpty()) {
    CSize sizeButton = MenuImages::Size();
    IRECT rectButton = rectGroup;
    rectButton.l = max(rectButton.l,
        rectButton.r - (rectButton.Height() + 1) / 2 - (sizeButton.w + 1) / 2);
    rectButton.t = max(rectButton.t,
        rectButton.b - (rectButton.Height() + 1) / 2 - (sizeButton.h + 1) / 2);
    rectButton.r = rectButton.l + sizeButton.w;
    rectButton.b = rectButton.t + sizeButton.h;
    if (rectButton.r <= rectGroup.r && rectButton.b <= rectGroup.b) {
      if (bIsHighlighted) {
        // Draw button frame
        COLOR pBrushOld = (COLOR) im->SelectObject(&s->clrBarFace);
        COLOR clrBckOld = im->GetBkColor();
        imdraw_Draw3dRect(im, 0, &rectButton, s->clrWindow, s->clrBarShadow);
        im->SetBkColor(clrBckOld);
        im->SelectObject(pBrushOld);
      }
      MenuImages::Draw(im,
          pGroup->s->m_bIsCollapsed
          ? MenuImages::IdArowDown
          : MenuImages::IdArowUp,
          rectButton.TopLeft(),
          MenuImages::ImageBlack);
    }
  }
}
int VisualManager2007_OnDrawTask(img_t* im, Task* pTask, CImageList* pIcons,
    BOOL bIsHighlighted /*= FALSE*/, BOOL bIsSelected /*= FALSE*/)
{
  ASSERT_VALID(pTask);
  if (CanDrawImage() && pTask->s->m_bIsSeparator) {
    IRECT rectFill = pTask->s->m_rect;
    rectFill.t = rectFill.CenterPoint().y;
    rectFill.b = rectFill.t + 1;
    DrawManager dm(*im);
    dm.Fill4ColorsGradient(rectFill, s->m_clrTaskPaneGroupAreaLight,
        s->m_clrTaskPaneGroupBorder, s->m_clrTaskPaneGroupBorder, s->m_clrTaskPaneGroupAreaLight, FALSE);
    return 0;
  }
  VisualManager2003_OnDrawTask(im, pTask, pIcons, bIsHighlighted,
      bIsSelected);
}
int VisualManager2007_OnDrawScrollButtons(img_t* im, const IRECT& rect, const int nBorderSize,
    int iImage, BOOL bHilited)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawScrollButtons(im, rect, nBorderSize, iImage, bHilited);
    return 0;
  }
  IRECT rt(rect);
  rt.t--;
  s->m_ctrlTaskScrollBtn.Draw(im, rt, bHilited ? 1 : 0);
  MenuImages::Draw(im, (MenuImages::IMAGES_IDS) iImage, rect);
}
#endif // BCGP_EXCLUDE_TASK_PANE
int VisualManager2007_OnDrawHeaderCtrlBorder(HeaderCtrl* pCtrl, img_t* im,
    IRECT& rect, BOOL bIsPressed, BOOL bIsHighlighted)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawHeaderCtrlBorder(pCtrl, im, rect, bIsPressed, bIsHighlighted);
    return 0;
  }
  COLOR clrStart = s->m_clrHeaderNormalStart;
  COLOR clrFinish = s->m_clrHeaderNormalFinish;
  COLOR clrBorder = s->m_clrHeaderNormalBorder;
  if (bIsPressed) {
    clrStart = s->m_clrHeaderPressedStart;
    clrFinish = s->m_clrHeaderPressedFinish;
    clrBorder = s->m_clrHeaderPressedBorder;
  }
  else if (bIsHighlighted) {
    clrStart = s->m_clrHeaderHighlightedStart;
    clrFinish = s->m_clrHeaderHighlightedFinish;
    clrBorder = s->m_clrHeaderHighlightedBorder;
  }
  {
    DrawManager dm(*im);
    dm.FillGradient(rect, clrFinish, clrStart);
  }
  CPen pen(PS_SOLID, 0, clrBorder);
  CPen* pOldPen = im->SelectObject(&pen);
  if (bIsPressed || bIsHighlighted) {
    im->MoveTo(rect.r - 1, rect.t);
    im->LineTo(rect.r - 1, rect.b - 1);
    im->LineTo(rect.l, rect.b - 1);
    im->LineTo(rect.l, rect.t - 1);
  }
  else {
    im->MoveTo(rect.r - 1, rect.t);
    im->LineTo(rect.r - 1, rect.b - 1);
    im->LineTo(rect.l - 1, rect.b - 1);
  }
  im->SelectObject(pOldPen);
}
#ifndef BCGP_EXCLUDE_GRID_CTRL
int VisualManager2007_OnFillGridHeaderBackground(GridCtrl* pCtrl, img_t* im, IRECT rect)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnFillGridHeaderBackground(pCtrl, im, rect);
    return 0;
  }
  imdraw_FillRect(im, 0, rect, &s->clrBarFace);
}
BOOL VisualManager2007_OnDrawGridHeaderItemBorder(GridCtrl* pCtrl, img_t* im, IRECT rect, BOOL bPressed)
{
  if (!CanDrawImage()) {
    return VisualManager2003_OnDrawGridHeaderItemBorder(pCtrl, im, rect, bPressed);
  }
  COLOR clrStart = bPressed ? s->m_clrGridHeaderPressedStart : s->m_clrGridHeaderNormalStart;
  COLOR clrFinish = bPressed ? s->m_clrGridHeaderPressedFinish : s->m_clrGridHeaderNormalFinish;
  COLOR clrBorder = bPressed ? s->m_clrGridHeaderPressedBorder : s->m_clrGridHeaderNormalBorder;
  {
    DrawManager dm(*im);
    dm.FillGradient(rect, clrFinish, clrStart);
  }
  CPen* pOldPen = im->SelectObject(&s->m_penGridSeparator);
  int nHeight = rect.Height() / 5;
  im->MoveTo(rect.r - 1, rect.t + nHeight);
  im->LineTo(rect.r - 1, rect.b - nHeight);
  im->SelectObject(pOldPen);
  {
    CPen pen(PS_SOLID, 1, clrBorder);
    pOldPen = im->SelectObject(&pen);
    im->MoveTo(rect.l, rect.t);
    im->LineTo(rect.r, rect.t);
    im->MoveTo(rect.l, rect.b - 1);
    im->LineTo(rect.r, rect.b - 1);
    im->SelectObject(pOldPen);
  }
  return FALSE;
}
int VisualManager2007_OnFillGridRowHeaderBackground(GridCtrl* pCtrl, img_t* im, IRECT rect)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnFillGridRowHeaderBackground(pCtrl, im, rect);
    return 0;
  }
  IRECT rectFill = rect;
  rectFill.t += 1;
  imdraw_FillRect(im, 0, rectFill, &s->clrBarFace);
}
BOOL VisualManager2007_OnDrawGridRowHeaderItemBorder(GridCtrl* pCtrl, img_t* im, IRECT rect, BOOL bPressed)
{
  if (!CanDrawImage()) {
    return VisualManager2003_OnDrawGridRowHeaderItemBorder(pCtrl, im, rect, bPressed);
  }
  IRECT rectInnerBorders(1, 1, 1, 0);
  IRECT rectOuterBorders(0, 0, 0, 1);
  COLOR clrStart = bPressed ? s->m_clrGridHeaderPressedFinish : s->m_clrGridHeaderNormalFinish;
  COLOR clrFinish = bPressed ? s->m_clrGridHeaderPressedStart : s->m_clrGridHeaderNormalStart;
  COLOR clrBorder = bPressed ? s->m_clrGridHeaderPressedBorder : s->m_clrGridHeaderNormalBorder;
  {
    IRECT rectFill = rect;
    rectFill.t += rectInnerBorders.t;
    DrawManager dm(*im);
    dm.FillGradient(rectFill, clrFinish, clrStart, FALSE);
  }
  CPen* pOldPen = im->SelectObject(&s->m_penGridSeparator);
  im->MoveTo(rect.l, rect.b - 1 + rectOuterBorders.b);
  im->LineTo(rect.r, rect.b - 1 + rectOuterBorders.b);
  im->SelectObject(pOldPen);
  {
    CPen pen(PS_SOLID, 1, clrBorder);
    pOldPen = im->SelectObject(&pen);
    im->MoveTo(rect.l, rect.t);
    im->LineTo(rect.l, rect.b + rectOuterBorders.b);
    im->MoveTo(rect.r - 1, rect.t);
    im->LineTo(rect.r - 1, rect.b + rectOuterBorders.b);
    im->SelectObject(pOldPen);
  }
  return FALSE;
}
int VisualManager2007_OnFillGridSelectAllAreaBackground(GridCtrl* pCtrl, img_t* im, IRECT rect, BOOL bPressed)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnFillGridSelectAllAreaBackground(pCtrl, im, rect, bPressed);
    return 0;
  }
  {
    DrawManager dm(*im);
    dm.FillGradient
    (
        rect,
        bPressed ? s->m_clrGridHeaderAllPressedBackFinish : s->m_clrGridHeaderAllNormalBackFinish,
        bPressed ? s->m_clrGridHeaderAllPressedBackStart : s->m_clrGridHeaderAllNormalBackStart
    );
  }
  rect.DeflateRect(5, 5);
  int nMin = min(rect.Width(), rect.Height());
  rect.l = rect.r - nMin;
  rect.t = rect.b - nMin;
  IPOINT ptRgn [] = {
    {rect.r, rect.t},
    {rect.r, rect.b},
    {rect.l, rect.b}
  };
  CRgn rgn;
  rgn.CreatePolygonRgn(ptRgn, 3, WINDING);
  im->SelectClipRgn(&rgn, RGN_COPY);
  {
    DrawManager dm(*im);
    dm.FillGradient
    (
        rect,
        bPressed ? s->m_clrGridHeaderAllPressedSignFinish : s->m_clrGridHeaderAllNormalSignFinish,
        bPressed ? s->m_clrGridHeaderAllPressedSignStart : s->m_clrGridHeaderAllNormalSignStart
    );
  }
  im->SelectClipRgn(NULL, RGN_COPY);
}
BOOL VisualManager2007_OnDrawGridSelectAllAreaBorder(GridCtrl* pCtrl, img_t* im, IRECT rect, BOOL bPressed)
{
  if (!CanDrawImage()) {
    return VisualManager2003_OnDrawGridSelectAllAreaBorder(pCtrl, im, rect, bPressed);
  }
  COLOR clrBorder = bPressed ? s->m_clrGridHeaderPressedBorder : s->m_clrGridHeaderNormalBorder;
  imdraw_Draw3dRect(im, 0, rect, clrBorder, clrBorder);
  rect.DeflateRect(1, 1);
  im->Draw3dRect
  (
      rect,
      bPressed ? s->m_clrGridHeaderAllPressedBorderHighlighted : s->m_clrGridHeaderAllNormalBorderHighlighted,
      bPressed ? s->m_clrGridHeaderAllPressedBorderShadow : s->m_clrGridHeaderAllNormalBorderShadow
  );
  return FALSE;
}
COLOR VisualManager2007_OnFillGridGroupByBoxBackground(img_t* im, IRECT rect)
{
  if (!CanDrawImage()) {
    return VisualManager2003_OnFillGridGroupByBoxBackground(im, rect);
  }
  imdraw_FillRect(im, 0, rect, &s->clrBarFace);
  return s->clrBarText;
}
COLOR VisualManager2007_OnFillGridGroupByBoxTitleBackground(img_t* im, IRECT rect)
{
  if (!CanDrawImage()) {
    return VisualManager2003_OnFillGridGroupByBoxTitleBackground(im, rect);
  }
  return s->clrBarText;
}
int VisualManager2007_OnDrawGridGroupByBoxItemBorder(GridCtrl* pCtrl,
    img_t* im, IRECT rect)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawGridGroupByBoxItemBorder(pCtrl,
        im, rect);
    return 0;
  }
  COLOR clrStart = s->m_clrGridHeaderNormalStart;
  COLOR clrFinish = s->m_clrGridHeaderNormalFinish;
  COLOR clrBorder = s->m_clrGridHeaderNormalBorder;
  {
    DrawManager dm(*im);
    dm.FillGradient(rect, clrFinish, clrStart);
  }
  imdraw_Draw3dRect(im, 0, rect, clrBorder, clrBorder);
}
COLOR VisualManager2007_GetGridLeftOffsetColor(GridCtrl* pCtrl)
{
  if (!CanDrawImage()) {
    return VisualManager2003_GetGridLeftOffsetColor(pCtrl);
  }
  return s->m_clrGridLeftOffset;
}
COLOR VisualManager2007_OnFillGridRowBackground(GridCtrl* pCtrl,
    img_t* im, IRECT rectFill, BOOL bSelected)
{
  COLOR clr = VisualManager2003_OnFillGridRowBackground(pCtrl,
      im, rectFill, bSelected);
  if (CanDrawImage()) {
    clr = pCtrl->IsHighlightGroups()
        ? (pCtrl->IsControlBarColors()
            ? s->clrBarText
            : s->clrBtnShadow)
          : s->clrWindowText;
  }
  return clr;
}
BOOL VisualManager2007_OnSetGridColorTheme(GridCtrl* pCtrl, BCGP_GRID_COLOR_DATA& theme)
{
  if (!CanDrawImage()) {
    return VisualManager2003_OnSetGridColorTheme(pCtrl, theme);
  }
  theme = s->m_GridColors;
  return TRUE;
}
#endif // BCGP_EXCLUDE_GRID_CTRL
#if !defined(BCGP_EXCLUDE_GRID_CTRL) && !defined(BCGP_EXCLUDE_GANTT)
int VisualManager2007_GetGanttColors(const GanttChart* pChart, BCGP_GANTT_CHART_COLORS& colors, COLOR clrBack) const
{
  if (!CanDrawImage()) {
    VisualManager2003_GetGanttColors(pChart, colors, clrBack);
    return 0;
  }
  BOOL bIsDefault = FALSE;
  if (clrBack == CLR_DEFAULT) {
    clrBack = s->m_clrPlannerWork;
    bIsDefault = TRUE;
  }
  GanttChart::PrepareColorScheme(clrBack, colors);
  colors.clrBackground = s->clrWindow;
  colors.clrShadows = s->m_clrMenuShadowBase;
  colors.clrRowBackground = CalculateWorkingColor2007(clrBack);
  colors.clrRowDayOff = CalculateNonWorkingColor2007(clrBack, TRUE);
  colors.clrGridLine0 = CalculateSeparatorColor(clrBack);
  if (bIsDefault) {
    colors.clrGridLine1 = s->clrBarFace;
  }
  else {
    colors.clrGridLine1 = CalculateHourLineColor2007(clrBack, TRUE, TRUE);
  }
  colors.clrConnectorLines = CalculateSelectionColor(clrBack);
  colors.clrSelection = colors.clrGridLine0;
  colors.clrSelectionBorder = colors.clrConnectorLines;
  double H, S, L;
  DrawManager::RGBtoHSL(_RGB(0, 0, 255), &H, &S, &L);
  colors.clrBarFill = DrawManager::HLStoRGB_ONE(H, max(L, 0.85) - 0.30, S);
  DrawManager::RGBtoHSL(_RGB(0, 255, 0), &H, &S, &L);
  colors.clrBarComplete = DrawManager::HLStoRGB_ONE(H, max(L, 0.85) - 0.30, S);
}
int VisualManager2007_DrawGanttHeaderCell(const GanttChart* pChart, CDC& dc, const BCGP_GANTT_CHART_HEADER_CELL_INFO& cellInfo, BOOL bHilite)
{
  if (!CanDrawImage()) {
    VisualManager2003_DrawGanttHeaderCell(pChart, dc, cellInfo, bHilite);
    return 0;
  }
  // Use the same as in grid control
  BOOL bPressed = FALSE; // reserved
  COLOR clrStart = bPressed ? s->m_clrGridHeaderPressedStart : s->m_clrGridHeaderNormalStart;
  COLOR clrFinish = bPressed ? s->m_clrGridHeaderPressedFinish : s->m_clrGridHeaderNormalFinish;
  COLOR clrBorder = bPressed ? s->m_clrGridHeaderPressedBorder : s->m_clrGridHeaderNormalBorder;
  IRECT rect = cellInfo.rectCell;
  // Fill the header
  {
    DrawManager dm(dc);
    dm.FillGradient(rect, clrFinish, clrStart);
  }
  // Drawing header separator
  {
    CPen* pOldPen = dc.SelectObject(&s->m_penGridSeparator);
    int nHeight = rect.Height() / 5;
    dc.MoveTo(rect.r - 1, rect.t + nHeight);
    dc.LineTo(rect.r - 1, rect.b - nHeight);
    dc.SelectObject(pOldPen);
  }
  // Drawing header borders
  {
    PenSelector pen(dc, clrBorder);
    if (cellInfo.pHeaderInfo != NULL && cellInfo.pHeaderInfo->bAboveChart) {
      dc.MoveTo(rect.l, rect.b - 1);
      dc.LineTo(rect.r, rect.b - 1);
    }
    else {
      dc.MoveTo(rect.l, rect.t);
      dc.LineTo(rect.r, rect.t);
    }
  }
}
COLOR VisualManager2007_GetGanttHeaderTextColor(BOOL bHilite) const
{
  if (!CanDrawImage()) {
    return VisualManager2003_GetGanttHeaderTextColor(bHilite);
  }
  return bHilite ? s->clrHotText : s->clrBtnText;
}
#endif // !defined(BCGP_EXCLUDE_GRID_CTRL) && !defined(BCGP_EXCLUDE_GANTT)
BOOL VisualManager2007_DrawCheckBox(CDC* im, IRECT rect,
    BOOL bHighlighted,
    int nState,
    BOOL bEnabled,
    BOOL bPressed)
{
  if (!CanDrawImage()) {
    return VisualManager2003_DrawCheckBox(im, rect, bHighlighted, nState, bEnabled, bPressed);
  }
  int index = nState * 4;
  if (!bEnabled) {
    index += 3;
  }
  else if (bPressed) {
    if (bHighlighted) {
      index += 2;
    }
  }
  else if (bHighlighted) {
    index += 1;
  }
  if (s->m_bIsRTL) {
    s->m_ctrlRibbonBtnCheck.Mirror();
  }
  s->m_ctrlRibbonBtnCheck.FillInterior(im, rect,
      ToolBarImages::ImageAlignHorzCenter,
      ImageAlignVertCenter,
      index);
  if (s->m_bIsRTL) {
    s->m_ctrlRibbonBtnCheck.Mirror();
  }
  return TRUE;
}
BOOL VisualManager2007_DrawRadioButton(CDC* im, IRECT rect,
    BOOL bHighlighted,
    BOOL bChecked,
    BOOL bEnabled,
    BOOL bPressed)
{
  if (!CanDrawImage()) {
    return VisualManager2003_DrawRadioButton(im, rect, bHighlighted, bChecked, bEnabled, bPressed);
  }
  int index = bChecked ? 4 : 0;
  if (!bEnabled) {
    index += 3;
  }
  else if (bPressed) {
    if (bHighlighted) {
      index += 2;
    }
  }
  else if (bHighlighted) {
    index += 1;
  }
  if (s->m_bIsRTL) {
    s->m_ctrlRibbonBtnRadio.Mirror();
  }
  s->m_ctrlRibbonBtnRadio.FillInterior(im, rect,
      ToolBarImages::ImageAlignHorzCenter,
      ImageAlignVertCenter,
      index);
  if (s->m_bIsRTL) {
    s->m_ctrlRibbonBtnRadio.Mirror();
  }
  return TRUE;
}
#ifndef BCGP_EXCLUDE_RIBBON
int VisualManager2007_OnDrawRibbonCaption(img_t* im, RibbonBar* pBar,
    IRECT rectCaption, IRECT rectText)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawRibbonCaption(im, pBar, rectCaption, rectText);
    return 0;
  }
  ASSERT_VALID(pBar);
  CWnd* pWnd = pBar->GetParent();
  ASSERT_VALID(pWnd);
  const DWORD dwStyle = pWnd->GetStyle();
  const DWORD dwStyleEx = pWnd->GetExStyle();
  const BOOL bIsRTL = (dwStyleEx & WS_EX_LAYOUTRTL) == WS_EX_LAYOUTRTL;
  const BOOL bActive = IsWindowActive(pWnd);
  const BOOL bGlass = pBar->IsTransparentCaption();
  {
    CSize szSysBorder(GetSystemBorders(TRUE));
    IRECT rectCaption1(rectCaption);
    IRECT rectBorder(s->m_ctrlMainBorderCaption.GetParams().s->m_rectSides);
    IRECT rectQAT = pBar->GetQuickAccessToolbarLocation();
    if (rectQAT.l > rectQAT.r) {
      rectText.l = rectQAT.l + 1;
    }
    rectCaption1.InflateRect(szSysBorder.w, szSysBorder.h, szSysBorder.w, 0);
    BOOL bHide = (pBar->GetHideFlags() & BCGPRIBBONBAR_HIDE_ALL) != 0;
    BOOL bExtra = !bHide && pBar->IsQuickAccessToolbarOnTop() &&
        rectQAT.l < rectQAT.r &&
        (!pBar->IsQATEmpty() || IsBeta1());
    if (!bGlass) {
      if (IsBeta()) {
        COLOR clr1 = bActive
            ? s->m_clrAppCaptionActiveStart
            : s->m_clrAppCaptionInactiveStart;
        COLOR clr2 = bActive
            ? s->m_clrAppCaptionActiveFinish
            : s->m_clrAppCaptionInactiveFinish;
        IRECT rectCaption2(rectCaption1);
        rectCaption2.DeflateRect(rectBorder.l, rectBorder.t,
            rectBorder.r, rectBorder.b);
        DrawManager dm(*im);
        dm.Fill4ColorsGradient(rectCaption2, clr1, clr2, clr2, clr1, FALSE);
        s->m_ctrlMainBorderCaption.DrawFrame(im, rectCaption1, bActive ? 0 : 1);
      }
      else {
        s->m_ctrlMainBorderCaption.Draw(im, rectCaption1, bActive ? 0 : 1);
      }
    }
    BOOL bDrawIcon = (bHide && !bExtra) || pBar->IsScenicLook();
    if (bExtra) {
      ControlRenderer* pCaptionQA = bGlass
          ? &s->m_ctrlRibbonCaptionQA_Glass
          : &s->m_ctrlRibbonCaptionQA;
      if (pCaptionQA->IsValid()) {
        IRECT rectQAFrame(rectQAT);
        rectQAFrame.r = pBar->GetQATCommandsLocation().r;
        rectQAFrame.InflateRect(0, 1, 1, 1);
        if (!pBar->IsScenicLook()) {
          const ControlRendererParams& params = pCaptionQA->GetParams();
          rectQAFrame.l -= params.s->m_rectCorners.l - 2;
          rectQAFrame.r += GetRibbonQATRightMargin();
          if (rectQAFrame.Height() < params.s->m_rectImage.Height()) {
            rectQAFrame.t = rectQAFrame.b - params.s->m_rectImage.Height();
          }
          if (bGlass) {
            const int dxFrame = GetSystemMetrics(SM_CXSIZEFRAME) / 2;
            const int nTop = s->GetRibbonImageScale() != 1. ? -2 : 1;
            rectQAFrame.DeflateRect(1, nTop, dxFrame, 0);
          }
          pCaptionQA->Draw(im, rectQAFrame, bActive ? 0 : 1);
        }
        else {
          rectQAFrame.DeflateRect(0, 4, 0, 6);
          IRECT rectSep(rectQAFrame);
          rectSep.l -= 2;
          rectSep.r = rectSep.l + 2;
          DrawSeparator(im, rectSep, s->m_penSeparatorDark, s->m_penSeparatorLight, FALSE);
          rectSep.l = rectText.l - 6;
          rectSep.r = rectSep.l + 2;
          DrawSeparator(im, rectSep, s->m_penSeparatorDark, s->m_penSeparatorLight, FALSE);
        }
      }
    }
    if (bDrawIcon) {
      BOOL bDestroyIcon = FALSE;
      CBitmap* hIcon = globalUtils.GetWndIcon(pWnd, &bDestroyIcon);
      if (hIcon != NULL) {
        CSize szIcon(::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
        long x = rectCaption.l + 2;
        long y = rectCaption.t + max(0, (rectCaption.Height() - szIcon.h) / 2);
        if (bGlass) {
          s->DrawIconOnGlass(s->m_hThemeWindow, im, hIcon, IRECT(x, y, x + szIcon.w, y + szIcon.h));
        }
        else {
          ::DrawIconEx(im, x, y, hIcon, szIcon.w, szIcon.h,
              0, NULL, DI_NORMAL);
        }
        if (rectText.l < (x + szIcon.w + 4)) {
          rectText.l = x + szIcon.w + 4;
        }
        if (bDestroyIcon) {
          ::DestroyIcon(hIcon);
        }
      }
    }
  }
  CString strText;
  pWnd->GetWindowText(strText);
  CFont* pOldFont = (CFont*)im->SelectObject(&s->m_AppCaptionFont);
  ASSERT(pOldFont != NULL);
  CString strTitle(strText);
  CString strDocument;
  BOOL bPrefix = FALSE;
  if ((dwStyle & FWS_ADDTOTITLE) == FWS_ADDTOTITLE) {
    bPrefix = (dwStyle & FWS_PREFIXTITLE) == FWS_PREFIXTITLE;
    CFrameWnd* pFrameWnd = DYNAMIC_DOWNCAST(CFrameWnd, pWnd);
    if (pFrameWnd != NULL) {
      strTitle = pFrameWnd->GetTitle();
      if (!strTitle.IsEmpty()) {
        if (strText.GetLength() >= strTitle.GetLength()) {
          if (bPrefix) {
            int pos = strText.Find(strTitle, strText.GetLength() - strTitle.GetLength());
            if (pos != -1) {
              strTitle = strText.Right(strTitle.GetLength() + 3);
              strDocument = strText.Left(strText.GetLength() - strTitle.GetLength());
            }
          }
          else {
            int pos = strText.Find(strTitle);
            if (pos != -1) {
              strTitle = strText.Left(strTitle.GetLength() + 3);
              strDocument = strText.Right(strText.GetLength() - strTitle.GetLength());
            }
          }
        }
      }
      else {
        strDocument = strText;
      }
    }
  }
  DrawNcText(im, rectText, strTitle, strDocument, bPrefix, bActive,
      bIsRTL, s->m_bNcTextCenter && !pBar->IsScenicLook(), bGlass, pWnd->IsZoomed() ? 0 : 10,
      pWnd->IsZoomed() ? _RGB(255, 255, 255) : (COLOR) - 1);
  im->SelectObject(pOldFont);
}
int VisualManager2007_GetRibbonQATRightMargin()
{
  if (!CanDrawImage() ||
      !s->m_ctrlRibbonCaptionQA.IsValid()) {
    return VisualManager2003_GetRibbonQATRightMargin();
  }
  return s->m_ctrlRibbonCaptionQA.GetParams().s->m_rectSides.r;
}
int VisualManager2007_OnDrawRibbonCaptionButton(
    img_t* im, RibbonCaptionButton* pButton)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawRibbonCaptionButton(im, pButton);
    return 0;
  }
  const BOOL bHighlighted = pButton->IsHighlighted() || pButton->IsFocused();
  const BOOL bPressed = pButton->IsPressed();
  BCGBUTTON_STATE state = ButtonsIsRegular;
  if (bPressed) {
    if (bHighlighted) {
      state = ButtonsIsPressed;
    }
  }
  else if (bHighlighted) {
    state = ButtonsIsHighlighted;
  }
  const BOOL bMDI = pButton->IsMDIChildButton();
  BOOL bActive = TRUE;
  if (!bMDI) {
    RibbonBar* pBar = pButton->GetParentRibbonBar();
    if (pBar->GetSafeHwnd() != NULL) {
      CWnd* pWnd = pBar->GetParent();
      ASSERT_VALID(pWnd);
      bActive = IsWindowActive(pWnd);
    }
  }
  DrawNcBtn(im, pButton->GetRect(), pButton->GetID(), state, FALSE, bActive, pButton->IsMDIChildButton());
}
COLOR VisualManager2007_OnDrawRibbonButtonsGroup(
    img_t* im, RibbonButtonsGroup* pGroup,
    IRECT rectGroup)
{
  if (!CanDrawImage()) {
    return VisualManager2003_OnDrawRibbonButtonsGroup(im, pGroup, rectGroup);
  }
  ASSERT_VALID(pGroup);
  if (pGroup->IsKindOf(RUNTIME_CLASS(RibbonQuickAccessToolbar)) &&
      s->m_ctrlRibbonPanelQAT.IsValid()) {
    RibbonBar* pBar = pGroup->GetParentRibbonBar();
    if (pBar != NULL &&
        (pBar->GetHideFlags() & BCGPRIBBONBAR_HIDE_ALL) == 0 &&
        !pBar->IsQuickAccessToolbarOnTop()) {
      s->m_ctrlRibbonPanelQAT.Draw(im, rectGroup);
    }
  }
  return(COLOR) - 1;
}
int VisualManager2007_OnDrawDefaultRibbonImage(img_t* im, IRECT rectImage,
    BOOL bIsDisabled/* = FALSE*/,
    BOOL bIsPressed/* = FALSE*/,
    BOOL bIsHighlighted/* = FALSE*/)
{
  if (!CanDrawImage() || !s->m_RibbonBtnDefaultImage.IsValid()) {
    VisualManager2003_OnDrawDefaultRibbonImage(im, rectImage,
        bIsDisabled, bIsPressed, bIsHighlighted);
    return 0;
  }
  s->m_RibbonBtnDefaultImage.DrawEx(im, rectImage, bIsDisabled ? 1 : 0,
      ToolBarImages::ImageAlignHorzCenter, ImageAlignVertCenter);
}
int VisualManager2007_OnDrawRibbonMainButton(
    img_t* im,
    RibbonButton* pButton)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawRibbonMainButton(im, pButton);
    return 0;
  }
  BOOL bHighlighted = pButton->IsHighlighted() || pButton->IsFocused();
  BOOL bPressed = pButton->IsPressed();
  if (pButton->IsDroppedDown()) {
    bPressed = TRUE;
    bHighlighted = TRUE;
  }
  IRECT rect = pButton->GetRect();
  int index = 0;
  if (bPressed) {
    if (bHighlighted) {
      index = 2;
    }
  }
  else if (bHighlighted) {
    index = 1;
  }
  if (!pButton->GetParentRibbonBar()->IsScenicLook()) {
    IRECT rectImage(s->m_RibbonBtnMain.GetParams().s->m_rectImage);
    ImageAlignHorz horz = ToolBarImages::ImageAlignHorzStretch;
    ImageAlignVert vert = ImageAlignVertStretch;
    if (rect.Width() >= rectImage.Width() &&
        rect.Height() >= rectImage.Height() &&
        (s->GetRibbonImageScale() == 1. || s->m_RibbonBtnMain.IsScaled())) {
      horz = ToolBarImages::ImageAlignHorzCenter;
      vert = ImageAlignVertCenter;
    }
    rect.OffsetRect(1, -1);
    s->m_RibbonBtnMain.FillInterior(im, rect, horz, vert, index);
  }
  else {
    s->m_ctrlRibbonBtnGroup_S.Draw(im, rect, index);
  }
}
COLOR VisualManager2007_OnDrawRibbonTabsFrame(
    img_t* im,
    RibbonBar* pWndRibbonBar,
    IRECT rectTab)
{
  if (!CanDrawImage()) {
    return VisualManager2003_OnDrawRibbonTabsFrame(im,
        pWndRibbonBar, rectTab);
  }
  return s->m_clrRibbonCategoryText;
}
int VisualManager2007_OnDrawRibbonCategory(
    img_t* im,
    RibbonCategory* pCategory,
    IRECT rectCategory)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawRibbonCategory(im, pCategory, rectCategory);
    return 0;
  }
  if (pCategory->IsOnDialogBar()) {
    DrawManager dm(*im);
    dm.FillGradient(rectCategory, s->m_clrToolBarGradientDark, s->m_clrToolBarGradientLight, TRUE);
    return 0;
  }
  ControlRenderer* pRenderer = &s->m_ctrlRibbonCategoryBack;
  BitmapCache* pCache = &s->m_cacheRibbonCategoryBack;
  BaseRibbonElement* pParentButton = pCategory->GetParentButton();
  if (pCategory->GetTabColor() != BCGPCategoryColor_None &&
      (pParentButton == NULL || !pParentButton->IsQATMode())) {
    XRibbonContextCategory& context =
        s->m_ctrlRibbonContextCategory[pCategory->GetTabColor() - 1];
    pRenderer = &context.s->m_ctrlBack;
    pCache = &context.s->m_cacheBack;
  }
  const ControlRendererParams& params = pRenderer->GetParams();
  RibbonPanelMenuBar* pMenuBar = pCategory->GetParentMenuBar();
  if (pMenuBar != NULL) {
    if (pMenuBar->GetCategory() != NULL) {
      if (rectCategory.l < 0 || rectCategory.t < 0) {
        DrawManager dm(*im);
        dm.FillGradient(rectCategory, s->m_clrBarGradientDark, s->m_clrBarGradientLight, TRUE);
        return 0;
      }
    }
    else if (pMenuBar->GetPanel() != NULL) {
      if (IsBeta()) {
        pRenderer->FillInterior(im, rectCategory);
        return 0;
      }
    }
  }
  int nCacheIndex = -1;
  if (pCache != NULL) {
    CSize size(params.s->m_rectImage.Width(), rectCategory.Height());
    nCacheIndex = pCache->FindIndex(size);
    if (nCacheIndex == -1) {
      nCacheIndex = pCache->CacheY(size.h, *pRenderer);
    }
  }
  if (nCacheIndex != -1) {
    pCache->Get(nCacheIndex)->DrawY(im, rectCategory,
        CSize(params.s->m_rectInter.l, params.s->m_rectImage.r - params.s->m_rectInter.r));
  }
  else {
    pRenderer->Draw(im, rectCategory);
  }
}
int VisualManager2007_OnDrawRibbonCategoryScroll(
    img_t* im,
    RibbonCategoryScroll* pScroll)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawRibbonCategoryScroll(im, pScroll);
    return 0;
  }
  ASSERT_VALID(pScroll);
  IRECT rect = pScroll->GetRect();
  ControlRenderer* pRenderer =
      &s->m_ctrlRibbonCategoryBtnPage[pScroll->IsLeftScroll() ? 0 : 1];
  int index = 0;
  if (pScroll->IsPressed()) {
    index = 1;
    if (pScroll->IsHighlighted()) {
      index = 2;
    }
  }
  else if (pScroll->IsHighlighted()) {
    index = 1;
  }
  pRenderer->Draw(im, rect, index);
  BOOL bIsLeft = pScroll->IsLeftScroll();
  if (s->m_bIsRTL) {
    bIsLeft = !bIsLeft;
  }
  MenuImages::Draw(im,
      bIsLeft ? MenuImages::IdArowLeftLarge : MenuImages::IdArowRightLarge,
      rect);
}
COLOR VisualManager2007_OnDrawRibbonCategoryTab(
    img_t* im,
    RibbonTab* pTab,
    BOOL bIsActive)
{
  if (!CanDrawImage()) {
    return VisualManager2003_OnDrawRibbonCategoryTab(im,
        pTab, bIsActive);
  }
  ASSERT_VALID(pTab);
  RibbonCategory* pCategory = pTab->GetParentCategory();
  ASSERT_VALID(pCategory);
  RibbonBar* pBar = pCategory->GetParentRibbonBar();
  ASSERT_VALID(pBar);
  bIsActive = bIsActive &&
      ((pBar->GetHideFlags() & BCGPRIBBONBAR_HIDE_ELEMENTS) == 0 || pTab->GetDroppedDown() != NULL);
  const BOOL bPressed = pTab->IsPressed();
  const BOOL bIsFocused = pTab->IsFocused() && (pBar->GetHideFlags() & BCGPRIBBONBAR_HIDE_ELEMENTS);
  const BOOL bIsHighlight = (pTab->IsHighlighted() || bIsFocused) && !pTab->IsDroppedDown();
  IRECT rectTab(pTab->GetRect());
  rectTab.b++;
  int ratio = 0;
  if (!IsBeta() && s->m_ctrlRibbonCategoryTabSep.IsValid()) {
    ratio = pBar->GetTabTrancateRatio();
  }
  if (ratio > 0) {
    rectTab.l++;
  }
  int nImage = bIsActive ? 3 : 0;
  if (bPressed) {
    if (bIsHighlight) {
      nImage = bIsActive ? 2 : 1;
    }
  }
  if (bIsHighlight) {
    nImage += 1;
  }
  ControlRenderer* pRenderer = &s->m_ctrlRibbonCategoryTab;
  COLOR clrText = s->m_clrRibbonCategoryText;
  COLOR clrTextHighlighted = s->m_clrRibbonCategoryTextHighlighted;
  if (pCategory->GetTabColor() != BCGPCategoryColor_None || pTab->IsSelected()) {
    XRibbonContextCategory& context =
        s->m_ctrlRibbonContextCategory [
            (pTab->IsSelected() || nImage == 4)
            ? BCGPCategoryColor_Orange - 1
            : pCategory->GetTabColor() - 1];
    pRenderer = &context.s->m_ctrlTab;
    clrText = context.s->m_clrText;
    clrTextHighlighted = context.s->m_clrTextHighlighted;
  }
  pRenderer->Draw(im, rectTab, nImage);
  if (ratio > 0) {
    IRECT rectSep(rectTab);
    rectSep.l = rectSep.r;
    rectSep.r++;
    rectSep.b--;
    s->m_ctrlRibbonCategoryTabSep.Draw(im, rectSep, 0, (BYTE)min(ratio * 255 / 100, 255));
  }
  return bIsActive
      ? clrTextHighlighted
      : clrText;
}
COLOR VisualManager2007_OnDrawRibbonPanel(
    img_t* im,
    RibbonPanel* pPanel,
    IRECT rectPanel,
    IRECT rectCaption)
{
  if (!CanDrawImage()) {
    return VisualManager2003_OnDrawRibbonPanel(im,
        pPanel, rectPanel, rectCaption);
  }
  ASSERT_VALID(pPanel);
  COLOR clrText = s->m_clrRibbonPanelText;
  if (pPanel->IsKindOf(RUNTIME_CLASS(RibbonMainPanel))) {
    const int nBorderSize = GetPopupMenuBorderSize();
    rectPanel.InflateRect(nBorderSize, nBorderSize);
    s->m_ctrlRibbonMainPanel.Draw(im, rectPanel);
  }
  else {
    BOOL bHighlighted = pPanel->IsHighlighted();
    if (bHighlighted) {
      clrText = s->m_clrRibbonPanelTextHighlighted;
    }
    if (!pPanel->IsScenicLook()) {
      ControlRenderer* pRendererB = &s->m_ctrlRibbonPanelBack_B;
      ControlRenderer* pRendererT = &s->m_ctrlRibbonPanelBack_T;
      BitmapCache* pCacheB = &s->m_cacheRibbonPanelBack_B;
      BitmapCache* pCacheT = &s->m_cacheRibbonPanelBack_T;
      RibbonCategory* pCategory = pPanel->GetParentCategory();
      ASSERT_VALID(pCategory);
      BaseRibbonElement* pParentButton = pPanel->GetParentButton();
      if (pCategory->GetTabColor() != BCGPCategoryColor_None &&
          (pParentButton == NULL || !pParentButton->IsQATMode())) {
        pRendererB = &s->m_ctrlRibbonContextPanelBack_B;
        pRendererT = &s->m_ctrlRibbonContextPanelBack_T;
        pCacheB = &s->m_cacheRibbonContextPanelBack_B;
        pCacheT = &s->m_cacheRibbonContextPanelBack_T;
        clrText = bHighlighted
            ? s->m_clrRibbonContextPanelTextHighlighted
            : s->m_clrRibbonContextPanelText;
      }
      if (!pPanel->IsCollapsed()) {
        IRECT rect(rectPanel);
        BOOL bDrawCaption = rectCaption.Height() > 0 && pRendererT->IsValid();
        if (bDrawCaption) {
          BOOL bBottomEnabled = pRendererB->IsValid();
          if (bBottomEnabled) {
            rect.b -= rectCaption.Height() == 0
                ? pRendererB->GetParams().s->m_rectImage.Height()
                : rectCaption.Height();
          }
          {
            const ControlRendererParams& params = pRendererT->GetParams();
            int nCacheIndex = -1;
            if (pCacheT != NULL) {
              CSize size(params.s->m_rectImage.Width(), rect.Height());
              nCacheIndex = pCacheT->FindIndex(size);
              if (nCacheIndex == -1) {
                nCacheIndex = pCacheT->CacheY(size.h, *pRendererT);
              }
            }
            if (nCacheIndex != -1) {
              pCacheT->Get(nCacheIndex)->DrawY(im, rect,
                  CSize(params.s->m_rectInter.l, params.s->m_rectImage.r - params.s->m_rectInter.r),
                  bHighlighted ? 1 : 0);
            }
            else {
              pRendererT->Draw(im, rect, bHighlighted ? 1 : 0);
            }
          }
          if (bBottomEnabled) {
            rect.t = rect.b;
            rect.b = rectPanel.b;
            const ControlRendererParams& params = pRendererB->GetParams();
            int nCacheIndex = -1;
            if (pCacheB != NULL) {
              CSize size(params.s->m_rectImage.Width(), rect.Height());
              nCacheIndex = pCacheB->FindIndex(size);
              if (nCacheIndex == -1) {
                nCacheIndex = pCacheB->CacheY(size.h, *pRendererB);
              }
            }
            if (nCacheIndex != -1) {
              pCacheB->Get(nCacheIndex)->DrawY(im, rect,
                  CSize(params.s->m_rectInter.l, params.s->m_rectImage.r - params.s->m_rectInter.r),
                  bHighlighted ? 1 : 0);
            }
            else {
              pRendererB->Draw(im, rect, bHighlighted ? 1 : 0);
            }
          }
        }
      }
    }
    else if (!pPanel->IsMenuMode() && !pPanel->IsCollapsed()) {
      rectPanel.l = rectPanel.r;
      rectPanel.r += 2;
      rectPanel.DeflateRect(0, 4);
      DrawSeparator(im, rectPanel, s->m_penSeparatorDark, s->m_penSeparatorLight, FALSE);
    }
  }
  return clrText;
}
int VisualManager2007_OnDrawRibbonPanelCaption(
    img_t* im,
    RibbonPanel* pPanel,
    IRECT rectCaption)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawRibbonPanelCaption(im,
        pPanel, rectCaption);
    return 0;
  }
  if (pPanel->IsKindOf(RUNTIME_CLASS(RibbonMainPanel))) {
    return 0;
  }
  CString str = pPanel->GetName();
  if (!str.IsEmpty()) {
    if (pPanel->GetLaunchButton().GetID() > 0) {
      rectCaption.r = pPanel->GetLaunchButton().GetRect().l;
      rectCaption.DeflateRect(1, 1);
      rectCaption.OffsetRect(-1, -1);
    }
    else {
      rectCaption.DeflateRect(1, 1);
      if ((rectCaption.Width() % 2) == 0) {
        rectCaption.r--;
      }
      rectCaption.OffsetRect(0, -1);
    }
    COLOR clrTextOld = im->SetTextColor(pPanel->IsHighlighted() ?
        s->m_clrRibbonPanelCaptionTextHighlighted : s->m_clrRibbonPanelCaptionText);
    im->DrawText(str, rectCaption,
        DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX);
    im->SetTextColor(clrTextOld);
  }
}
int VisualManager2007_OnDrawRibbonLaunchButton(
    img_t* im,
    RibbonLaunchButton* pButton,
    RibbonPanel* pPanel)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawRibbonLaunchButton(im,
        pButton, pPanel);
    return 0;
  }
  ASSERT_VALID(pPanel);
  IRECT rect(pButton->GetRect());
  if (!IsBeta()) {
    rect.r--;
    rect.b--;
  }
  BOOL bHighlighted = pButton->IsHighlighted() || pButton->IsFocused();
  int index = 0;
  if (s->m_RibbonBtnLaunchIcon.GetCount() > 3) {
    if (pButton->IsDisabled()) {
      index = 3;
    }
    else if (pButton->IsPressed()) {
      if (bHighlighted) {
        index = 2;
      }
    }
    else if (bHighlighted) {
      index = 1;
    }
  }
  else {
    if (!pButton->IsDisabled()) {
      if (pButton->IsPressed()) {
        if (bHighlighted) {
          index = 2;
        }
      }
      else if (bHighlighted) {
        index = 1;
      }
    }
  }
  if (s->m_ctrlRibbonBtnLaunch.IsValid()) {
    s->m_ctrlRibbonBtnLaunch.Draw(im, rect, index);
  }
  if (s->m_RibbonBtnLaunchIcon.IsValid()) {
    s->m_RibbonBtnLaunchIcon.DrawEx(im, rect, index,
        ToolBarImages::ImageAlignHorzCenter, ImageAlignVertCenter);
  }
}
COLOR VisualManager2007_OnFillRibbonButton(
    img_t* im,
    RibbonButton* pButton)
{
  if (!CanDrawImage()) {
    return VisualManager2003_OnFillRibbonButton(im, pButton);
  }
  const BOOL bIsMenuMode = pButton->IsMenuMode();
  IRECT rect(pButton->GetRect());
  ControlRenderer* pRenderer = NULL;
  BitmapCache* pCache = NULL;
  int index = 0;
  BOOL bDisabled = pButton->IsDisabled();
  BOOL bWasDisabled = bDisabled;
  BOOL bFocused = pButton->IsFocused();
  BOOL bDroppedDown = pButton->IsDroppedDown();
  BOOL bPressed = pButton->IsPressed() && !bIsMenuMode;
  BOOL bChecked = pButton->IsChecked();
  BOOL bHighlighted = pButton->IsHighlighted() || bFocused;
  BOOL bDefaultPanelButton = pButton->IsDefaultPanelButton() && !pButton->IsQATMode();
  if (bFocused) {
    bDisabled = FALSE;
  }
  if (pButton->IsDroppedDown() && !bIsMenuMode) {
    bChecked = TRUE;
    bPressed = FALSE;
    bHighlighted = FALSE;
  }
  BaseRibbonElement::RibbonElementLocation location =
      pButton->GetLocationInGroup();
  if (pButton->IsKindOf(RUNTIME_CLASS(RibbonEdit))) {
    COLOR color1 = s->m_clrRibbonEdit;
    if (bDisabled) {
      color1 = s->m_clrRibbonEditDisabled;
    }
    else if (bChecked || bHighlighted) {
      color1 = s->m_clrRibbonEditHighlighted;
    }
    COLOR color2 = color1;
    rect.l = pButton->GetCommandRect().l;
    {
      DrawManager dm(*im);
      dm.FillGradient(rect, color1, color2, TRUE);
    }
    return(COLOR) - 1;
  }
  if (bChecked && bIsMenuMode && !pButton->IsPaletteIcon()) {
    bChecked = FALSE;
  }
  if (location != BaseRibbonElement::RibbonElementNotInGroup &&
      pButton->IsShowGroupBorder()) {
    if (!pButton->GetMenuRect().IsRectEmpty()) {
      IRECT rectC = pButton->GetCommandRect();
      IRECT rectM = pButton->GetMenuRect();
      ControlRenderer* pRendererC = NULL;
      ControlRenderer* pRendererM = NULL;
      BitmapCache* pCacheC = NULL;
      BitmapCache* pCacheM = NULL;
      if (location == BaseRibbonElement::RibbonElementSingleInGroup) {
        pRendererC = &s->m_ctrlRibbonBtnGroupMenu_F[0];
        pRendererM = &s->m_ctrlRibbonBtnGroupMenu_L[1];
        pCacheC = &s->m_cacheRibbonBtnGroupMenu_F[0];
        pCacheM = &s->m_cacheRibbonBtnGroupMenu_L[1];
      }
      else if (location == BaseRibbonElement::RibbonElementFirstInGroup) {
        pRendererC = &s->m_ctrlRibbonBtnGroupMenu_F[0];
        pRendererM = &s->m_ctrlRibbonBtnGroupMenu_F[1];
        pCacheC = &s->m_cacheRibbonBtnGroupMenu_F[0];
        pCacheM = &s->m_cacheRibbonBtnGroupMenu_F[1];
      }
      else if (location == BaseRibbonElement::RibbonElementLastInGroup) {
        pRendererC = &s->m_ctrlRibbonBtnGroupMenu_L[0];
        pRendererM = &s->m_ctrlRibbonBtnGroupMenu_L[1];
        pCacheC = &s->m_cacheRibbonBtnGroupMenu_L[0];
        pCacheM = &s->m_cacheRibbonBtnGroupMenu_L[1];
      }
      else {
        pRendererC = &s->m_ctrlRibbonBtnGroupMenu_M[0];
        pRendererM = &s->m_ctrlRibbonBtnGroupMenu_M[1];
        pCacheC = &s->m_cacheRibbonBtnGroupMenu_M[0];
        pCacheM = &s->m_cacheRibbonBtnGroupMenu_M[1];
      }
      int indexC = 0;
      int indexM = 0;
      BOOL bHighlightedC = pButton->IsCommandAreaHighlighted();
      BOOL bHighlightedM = pButton->IsMenuAreaHighlighted();
      if (IsBeta()) {
        if (bChecked) {
          indexC = 3;
        }
        if (bDisabled) {
          indexC = 0;
        }
        else {
          if (pButton->IsDroppedDown() && !bIsMenuMode) {
            indexC = pButton->IsChecked() ? 3 : 0;
            indexM = 3;
          }
          else {
            if (bPressed) {
              if (bHighlightedC) {
                indexM = 1;
                indexC = 2;
              }
            }
            else if (bHighlighted) {
              indexC++;
              indexM = 1;
            }
          }
        }
      }
      else {
        if (bChecked) {
          indexC = 3;
          if (bHighlighted) {
            indexM = 5;
          }
        }
        if (bDisabled) {
          if (bChecked) {
            indexC = 5;
            indexM = 4;
          }
        }
        else {
          if (pButton->IsDroppedDown() && !bIsMenuMode) {
            indexC = pButton->IsChecked() ? 3 : 6;
            indexM = 3;
          }
          else {
            if (bFocused) {
              indexC = 6;
              indexM = 5;
            }
            if (bHighlightedC || bHighlightedM) {
              if (bChecked) {
                indexC = bHighlightedC ? 4 : 3;
              }
              else {
                indexC = bHighlightedC ? 1 : 6;
              }
              indexM = bHighlightedM ? 1 : 5;
            }
            if (bPressed) {
              if (bHighlightedC) {
                indexC = 2;
              }
            }
          }
        }
      }
      if (indexC != -1 && indexM != -1) {
        int nCacheIndex = -1;
        if (pCacheC != NULL) {
          CSize size(rectC.Size());
          nCacheIndex = pCacheC->FindIndex(size);
          if (nCacheIndex == -1) {
            nCacheIndex = pCacheC->Cache(size, *pRendererC);
          }
        }
        if (nCacheIndex != -1) {
          pCacheC->Get(nCacheIndex)->Draw(im, rectC, indexC);
        }
        else {
          pRendererC->Draw(im, rectC, indexC);
        }
        nCacheIndex = -1;
        if (pCacheM != NULL) {
          CSize size(rectM.Size());
          nCacheIndex = pCacheM->FindIndex(size);
          if (nCacheIndex == -1) {
            nCacheIndex = pCacheM->Cache(size, *pRendererM);
          }
        }
        if (nCacheIndex != -1) {
          pCacheM->Get(nCacheIndex)->Draw(im, rectM, indexM);
        }
        else {
          pRendererM->Draw(im, rectM, indexM);
        }
      }
      return(COLOR) - 1;
    }
    else {
      if (location == BaseRibbonElement::RibbonElementSingleInGroup) {
        pRenderer = &s->m_ctrlRibbonBtnGroup_S;
        pCache = &s->m_cacheRibbonBtnGroup_S;
      }
      else if (location == BaseRibbonElement::RibbonElementFirstInGroup) {
        pRenderer = &s->m_ctrlRibbonBtnGroup_F;
        pCache = &s->m_cacheRibbonBtnGroup_F;
      }
      else if (location == BaseRibbonElement::RibbonElementLastInGroup) {
        pRenderer = &s->m_ctrlRibbonBtnGroup_L;
        pCache = &s->m_cacheRibbonBtnGroup_L;
      }
      else {
        pRenderer = &s->m_ctrlRibbonBtnGroup_M;
        pCache = &s->m_cacheRibbonBtnGroup_M;
      }
      if (bChecked) {
        index = 3;
      }
      if (bDisabled && !bFocused) {
        index = 0;
      }
      else {
        if (bPressed) {
          if (bHighlighted) {
            index = 2;
          }
        }
        else if (bHighlighted) {
          index++;
        }
      }
    }
  }
  else if (bDefaultPanelButton) {
    if (bPressed) {
      if (bHighlighted) {
        index = 2;
      }
    }
    else if (bHighlighted) {
      index = 1;
    }
    else if (bChecked) {
      index = 2;
    }
    if (bFocused && !bDroppedDown && s->m_ctrlRibbonBtnDefault.GetImageCount() > 3) {
      index = 3;
    }
    if (index != -1) {
      pRenderer = &s->m_ctrlRibbonBtnDefault;
      BitmapCache* pCache = &s->m_cacheRibbonBtnDefault;
      RibbonCategory* pCategory = pButton->GetParentCategory();
      if (pCategory != NULL) {
        ASSERT_VALID(pCategory);
        if (pCategory->GetTabColor() != BCGPCategoryColor_None) {
          XRibbonContextCategory& context =
              s->m_ctrlRibbonContextCategory[pCategory->GetTabColor() - 1];
          pRenderer = &context.s->m_ctrlBtnDefault;
          pCache = &context.s->m_cacheBtnDefault;
        }
      }
      const ControlRendererParams& params = pRenderer->GetParams();
      int nCacheIndex = -1;
      if (pCache != NULL) {
        CSize size(params.s->m_rectImage.Width(), rect.Height());
        nCacheIndex = pCache->FindIndex(size);
        if (nCacheIndex == -1) {
          nCacheIndex = pCache->CacheY(size.h, *pRenderer);
        }
      }
      if (nCacheIndex != -1) {
        pCache->Get(nCacheIndex)->DrawY(im, rect,
            CSize(params.s->m_rectInter.l, params.s->m_rectImage.r - params.s->m_rectInter.r),
            index);
        return s->m_clrToolBarBtnTextHighlighted;
      }
    }
  }
  else if ((!bDisabled && (bPressed || bChecked || bHighlighted)) ||
      (bDisabled && bFocused)) {
    if (!pButton->GetMenuRect().IsRectEmpty()/* &&
 (pButton->IsHighlighted() || bChecked)*/) {
      IRECT rectC = pButton->GetCommandRect();
      IRECT rectM = pButton->GetMenuRect();
      ControlRenderer* pRendererC = pButton->IsMenuOnBottom()
          ? &s->m_ctrlRibbonBtnMenuV[0]
          : &s->m_ctrlRibbonBtnMenuH[0];
      ControlRenderer* pRendererM = pButton->IsMenuOnBottom()
          ? &s->m_ctrlRibbonBtnMenuV[1]
          : &s->m_ctrlRibbonBtnMenuH[1];
      int indexC = -1;
      int indexM = -1;
      BOOL bDropped = pButton->IsDroppedDown();
      BOOL bHighlightedC = pButton->IsCommandAreaHighlighted();
      BOOL bHighlightedM = pButton->IsMenuAreaHighlighted();
      if (IsBeta()) {
        if (bChecked) {
          indexC = 2;
          indexM = 2;
        }
        if (bDisabled) {
          if (bChecked) {
            // TODO
          }
        }
        else {
          if (bDropped && !bIsMenuMode) {
            indexC = bChecked ? 2 : 4;
            indexM = 2;
          }
          else {
            if (bPressed) {
              if (bHighlighted) {
                if (bHighlightedC) {
                  indexC = 1;
                }
                else {
                  indexC = bChecked ? indexC : 0;
                }
                indexM = bChecked ? indexM : 0;
              }
            }
            else if (bHighlighted) {
              indexC++;
              indexM++;
            }
          }
        }
      }
      else {
        if (bDisabled) {
          if (bHighlightedC || bHighlightedM) {
            indexC = 4;
            indexM = 4;
            if (bHighlightedM) {
              indexM = 0;
              if (bDropped && !bIsMenuMode) {
                indexC = 5;
                indexM = 2;
              }
              else if (bPressed) {
                indexM = 1;
              }
            }
          }
        }
        else {
          if (bDropped && !bIsMenuMode) {
            indexC = 5;
            indexM = 2;
          }
          else {
            if (bFocused) {
              indexC = 5;
              indexM = 4;
            }
            if (bChecked) {
              indexC = 2;
              indexM = 2;
            }
            if (bHighlightedC || bHighlightedM) {
              indexM = 4;
              if (bPressed) {
                if (bHighlightedC) {
                  indexC = 1;
                }
                else if (bHighlightedM) {
                  indexC = bChecked ? 3 : 5;
                }
              }
              else {
                indexC = bChecked ? 3 : 0;
                if (bHighlightedM) {
                  indexC = bChecked ? 3 : 5;
                  indexM = 0;
                }
              }
            }
          }
        }
      }
      if (indexC != -1) {
        pRendererC->Draw(im, rectC, indexC);
      }
      if (indexM != -1) {
        pRendererM->Draw(im, rectM, indexM);
      }
      return(COLOR) - 1;
    }
    else {
      index = -1;
      pRenderer = &s->m_ctrlRibbonBtn[0];
      if (rect.Height() > pRenderer->GetParams().s->m_rectImage.Height() * 1.5 &&
          s->m_ctrlRibbonBtn[1].IsValid()) {
        pRenderer = &s->m_ctrlRibbonBtn[1];
      }
      if (bDisabled && bFocused) {
        if (pRenderer->GetImageCount() > 4) {
          index = 4;
        }
        else {
          index = 0;
        }
      }
      if (!bDisabled) {
        if (bChecked) {
          index = 2;
        }
        if (bPressed) {
          if (bHighlighted) {
            index = 1;
          }
        }
        else if (bHighlighted) {
          index++;
        }
      }
    }
  }
  COLOR clrText = bWasDisabled
      ? s->m_clrToolBarBtnTextDisabled
      : COLOR(-1);
  if (pRenderer != NULL) {
    if (index != -1) {
      int nCacheIndex = -1;
      if (pCache != NULL) {
        CSize size(rect.Size());
        nCacheIndex = pCache->FindIndex(size);
        if (nCacheIndex == -1) {
          nCacheIndex = pCache->Cache(size, *pRenderer);
        }
      }
      if (nCacheIndex != -1) {
        pCache->Get(nCacheIndex)->Draw(im, rect, index);
      }
      else {
        pRenderer->Draw(im, rect, index);
      }
      if (!bWasDisabled) {
        clrText = s->m_clrToolBarBtnTextHighlighted;
      }
    }
  }
  return clrText;
}
int VisualManager2007_OnDrawRibbonButtonBorder(
    img_t* im,
    RibbonButton* pButton)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawRibbonButtonBorder(im, pButton);
  }
  if (pButton->IsKindOf(RUNTIME_CLASS(RibbonEdit))) {
    IRECT rect(pButton->GetRect());
    COLOR colorBorder = s->m_clrRibbonEditBorder;
    if (pButton->IsDisabled()) {
      colorBorder = s->m_clrRibbonEditBorderDisabled;
    }
    else if (pButton->IsHighlighted() || pButton->IsDroppedDown() || pButton->IsFocused()) {
      colorBorder = pButton->IsDroppedDown()
          ? s->m_clrRibbonEditBorderPressed
          : s->m_clrRibbonEditBorderHighlighted;
    }
    rect.l = pButton->GetCommandRect().l;
    if (ToolBarImages::s->m_bIsDrawOnGlass) {
      DrawManager dm(*im);
      dm.DrawRect(rect, (COLOR) - 1, colorBorder);
    }
    else {
      imdraw_Draw3dRect(im, 0, rect, colorBorder, colorBorder);
    }
  }
}
int VisualManager2007_OnDrawRibbonMenuCheckFrame(
    img_t* im,
    RibbonButton* pButton,
    IRECT rect)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawRibbonMenuCheckFrame(im, pButton, rect);
    return 0;
  }
  s->m_ctrlMenuItemBack.Draw(im, rect);
}
int VisualManager2007_OnDrawRibbonDefaultPaneButton(
    img_t* im,
    RibbonButton* pButton)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawRibbonDefaultPaneButton(im, pButton);
    return 0;
  }
  OnFillRibbonButton(im, pButton);
  BOOL bIsQATMode = pButton->IsQATMode();
  IRECT rectFrame(pButton->GetRect());
  if (!bIsQATMode) {
    if (s->m_ctrlRibbonBtnDefaultIcon.IsValid()) {
      const CSize sizeImage = pButton->GetImageSize(RibbonButton::RibbonImageSmall);
      const int nMarginX = 11;
      const int nMarginY = 10;
      rectFrame.t += nMarginY / 2;
      rectFrame.b = rectFrame.t + sizeImage.h + 2 * nMarginY;
      rectFrame.t -= 2;
      rectFrame.l = rectFrame.CenterPoint().x - sizeImage.w / 2 - nMarginX;
      rectFrame.r = rectFrame.l + sizeImage.w + 2 * nMarginX;
      s->m_ctrlRibbonBtnDefaultIcon.Draw(im, rectFrame);
    }
  }
  else {
    if (s->m_ctrlRibbonBtnDefaultQAT.IsValid()) {
      int index = 0;
      if (pButton->IsDroppedDown()) {
        index = 2;
      }
      else if (pButton->IsPressed()) {
        if (pButton->IsHighlighted()) {
          index = 2;
        }
      }
      else if (pButton->IsHighlighted() || pButton->IsFocused()) {
        index = 1;
      }
      s->m_ctrlRibbonBtnDefaultQAT.Draw(im, rectFrame, index);
    }
    else if (s->m_ctrlRibbonBtnDefaultQATIcon.IsValid()) {
      int nHeight = s->m_ctrlRibbonBtnDefaultQATIcon.GetParams().s->m_rectImage.Height();
      if (rectFrame.Height() < nHeight) {
        nHeight = rectFrame.Height() / 2;
      }
      rectFrame.DeflateRect(1, 0);
      rectFrame.t = rectFrame.b - nHeight;
      s->m_ctrlRibbonBtnDefaultQATIcon.Draw(im, rectFrame);
    }
  }
  OnDrawRibbonDefaultPaneButtonContext(im, pButton);
}
int VisualManager2007_OnDrawRibbonDefaultPaneButtonIndicator(
    img_t* im,
    RibbonButton* pButton,
    IRECT rect,
    BOOL bIsSelected,
    BOOL bHighlighted)
{
  if (!CanDrawImage() || !s->m_ctrlRibbonBtnDefaultIcon.IsValid()) {
    VisualManager2003_OnDrawRibbonDefaultPaneButtonIndicator(
        im, pButton, rect, bIsSelected, bHighlighted);
    return 0;
  }
  rect.l = rect.r - s->m_ctrlRibbonBtnDefaultIcon.GetParams().s->m_rectImage.Width();
  s->m_ctrlRibbonBtnDefaultIcon.Draw(im, rect);
  IRECT rectWhite = rect;
  rectWhite.OffsetRect(0, 1);
  MenuImages::Draw(im, MenuImages::IdArowDown, rectWhite,
      MenuImages::ImageWhite);
  MenuImages::Draw(im, MenuImages::IdArowDown, rect,
      MenuImages::ImageBlack);
}
int VisualManager2007_OnFillRibbonEdit(
    img_t* im,
    RibbonEditCtrl* pEdit,
    IRECT rect,
    BOOL bIsHighlighted,
    BOOL bIsPaneHighlighted,
    BOOL bIsDisabled,
    COLOR& clrText, COLOR& clrSelBackground, COLOR& clrSelText)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnFillRibbonEdit(im,
        pEdit, rect, bIsHighlighted, bIsPaneHighlighted, bIsDisabled,
        clrText, clrSelBackground, clrSelText);
    return 0;
  }
  COLOR color = s->m_clrRibbonEdit;
  if (bIsDisabled) {
    color = s->m_clrRibbonEditDisabled;
  }
  else {
    if (bIsHighlighted) {
      color = s->m_clrRibbonEditHighlighted;
    }
  }
  DrawManager dm(*im);
  dm.DrawRect(rect, color, (COLOR) - 1);
  if (bIsDisabled) {
    clrText = s->clrGrayedText;
  }
  else {
    clrText = s->m_clrMenuText; // TODO
    clrSelText = s->m_clrMenuText; // TODO
    clrSelBackground = s->m_clrRibbonEditSelection;
  }
}
COLOR VisualManager2007_GetRibbonEditBackgroundColor(
    RibbonEditCtrl* pEdit,
    BOOL bIsHighlighted,
    BOOL bIsPaneHighlighted,
    BOOL bIsDisabled)
{
  if (!CanDrawImage()) {
    return VisualManager2003_GetRibbonEditBackgroundColor(
        pEdit, bIsHighlighted, bIsPaneHighlighted, bIsDisabled);
  }
  COLOR color = s->m_clrRibbonEdit;
  if (bIsDisabled) {
    color = s->m_clrRibbonEditDisabled;
  }
  else {
    if (bIsHighlighted) {
      color = s->m_clrRibbonEditHighlighted;
    }
  }
  return color;
}
int VisualManager2007_OnDrawRibbonMainPanelFrame(
    img_t* im,
    RibbonMainPanel* pPanel,
    IRECT rect)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawRibbonMainPanelFrame(im,
        pPanel, rect);
    return 0;
  }
  if (!IsBeta()) {
    rect.r += 2; //TODO
    s->m_ctrlRibbonMainPanelBorder.DrawFrame(im, rect);
  }
}
int VisualManager2007_OnFillRibbonMenuFrame(
    img_t* im,
    RibbonMainPanel* pPanel,
    IRECT rect)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnFillRibbonMenuFrame(im,
        pPanel, rect);
    return 0;
  }
  imdraw_FillRect(im, 0, rect, s->m_clrMenuLight);
}
int VisualManager2007_OnDrawRibbonRecentFilesFrame(
    img_t* im,
    RibbonMainPanel* pPanel,
    IRECT rect)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawRibbonRecentFilesFrame(im,
        pPanel, rect);
    return 0;
  }
  rect.r += 2; //TODO
  imdraw_FillRect(im, 0, rect, s->m_clrBarBkgnd);
  IRECT rectSeparator = rect;
  rectSeparator.r = rectSeparator.l + 2;
  DrawSeparator(im, rectSeparator, FALSE);
}
COLOR VisualManager2007_OnFillRibbonMainPanelButton(
    img_t* im,
    RibbonButton* pButton)
{
  if (!CanDrawImage()) {
    return VisualManager2003_OnFillRibbonMainPanelButton(im, pButton);
  }
  BOOL bHighlighted = pButton->IsHighlighted();
  COLOR clrText = bHighlighted
      ? s->m_clrMenuTextHighlighted
      : pButton->IsDisabled()
      ? s->m_clrMenuTextDisabled
      : s->m_clrMenuText;
  const int index = bHighlighted ? 1 : 0;
  s->m_ctrlRibbonBtnMainPanel.Draw(im, pButton->GetRect(), index);
  return clrText;
}
int VisualManager2007_OnDrawRibbonMainPanelButtonBorder(
    img_t* im, RibbonButton* pButton)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawRibbonMainPanelButtonBorder(im, pButton);
    return 0;
  }
}
int VisualManager2007_OnDrawRibbonPaletteButton(
    img_t* im,
    RibbonPaletteIcon* pButton)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawRibbonPaletteButton(im, pButton);
    return 0;
  }
  int index = 0;
  if (pButton->IsDisabled()) {
    index = 3;
  }
  else {
    if (pButton->IsPressed()) {
      if (pButton->IsHighlighted()) {
        index = 2;
      }
    }
    else if (pButton->IsHighlighted() || pButton->IsFocused()) {
      index = 1;
    }
  }
  int nBtn = 1;
  if (pButton->IsLast()) {
    nBtn = 2;
  }
  else if (pButton->IsFirst()) {
    nBtn = 0;
  }
  s->m_ctrlRibbonBtnPalette[nBtn].Draw(im, pButton->GetRect(), index);
}
int VisualManager2007_OnDrawRibbonPaletteBorder(
    img_t* im,
    RibbonPaletteButton* pButton,
    IRECT rectBorder)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawRibbonPaletteBorder(im, pButton, rectBorder);
    return 0;
  }
  rectBorder.r -= 5;
  imdraw_Draw3dRect(im, 0, rectBorder, s->clrBarShadow, s->clrBarShadow);
}
COLOR VisualManager2007_OnDrawRibbonCategoryCaption(
    img_t* im,
    RibbonContextCaption* pContextCaption)
{
  if (!CanDrawImage() || pContextCaption->GetColor() == BCGPCategoryColor_None) {
    return VisualManager2003_OnDrawRibbonCategoryCaption(im, pContextCaption);
  }
  XRibbonContextCategory& context =
      s->m_ctrlRibbonContextCategory[pContextCaption->GetColor() - 1];
  IRECT rect(pContextCaption->GetRect());
  context.s->m_ctrlCaption.Draw(im, rect);
  int xTabRight = pContextCaption->GetRightTabX();
  if (xTabRight > 0) {
    IRECT rectTab(pContextCaption->GetParentRibbonBar()->GetActiveCategory()->GetTabRect());
    rect.t = rectTab.t;
    rect.b = rectTab.b;
    rect.r = xTabRight;
    s->m_ctrlRibbonContextSeparator.DrawFrame(im, rect);
  }
  return context.s->m_clrCaptionText;
}
COLOR VisualManager2007_OnDrawRibbonStatusBarPane(img_t* im, RibbonStatusBar* pBar,
    RibbonStatusBarPane* pPane)
{
  if (!CanDrawImage()) {
    return VisualManager2003_OnDrawRibbonStatusBarPane(im, pBar, pPane);
  }
  ASSERT_VALID(pBar);
  ASSERT_VALID(pPane);
  IRECT rectPane = pPane->GetRect();
  const BOOL bHighlighted = pPane->IsHighlighted() || pPane->IsFocused();
  const BOOL bChecked = pPane->IsChecked();
  const BOOL bExtended = pPane->IsExtended();
  if (bHighlighted || bChecked) {
    IRECT rectButton = rectPane;
    rectButton.DeflateRect(1, 1);
    int index = 0;
    if (pPane->IsPressed()) {
      if (bHighlighted) {
        index = 1;
      }
    }
    else if (bChecked) {
      if (bHighlighted) {
        index = 0;
      }
      else {
        index = 1;
      }
    }
    s->m_ctrlRibbonBtnStatusPane.Draw(im, rectButton, index);
  }
  if (pPane->IsDisabled()) {
    return bExtended ? s->m_clrExtenedStatusBarTextDisabled : s->m_clrStatusBarTextDisabled;
  }
  return bHighlighted ?
      s->m_clrToolBarBtnTextHighlighted :
      s->m_clrStatusBarText;
}
int VisualManager2007_OnDrawRibbonSliderZoomButton(
    img_t* im, RibbonSlider* pSlider,
    IRECT rect, BOOL bIsZoomOut,
    BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawRibbonSliderZoomButton(
        im, pSlider, rect, bIsZoomOut, bIsHighlighted, bIsPressed, bIsDisabled);
    return 0;
  }
  ControlRenderer* pRenderer = bIsZoomOut
      ? &s->m_ctrlRibbonSliderBtnMinus
      : &s->m_ctrlRibbonSliderBtnPlus;
  int index = 0;
  if (bIsDisabled) {
    index = 3;
  }
  else {
    if (bIsPressed) {
      if (bIsHighlighted) {
        index = 2;
      }
    }
    else if (bIsHighlighted) {
      index = 1;
    }
  }
  pRenderer->FillInterior(im, rect,
      ToolBarImages::ImageAlignHorzCenter,
      ImageAlignVertCenter,
      index);
}
int VisualManager2007_OnDrawRibbonSliderChannel(
    img_t* im, RibbonSlider* pSlider,
    IRECT rect)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawRibbonSliderChannel(
        im, pSlider, rect);
    return 0;
  }
  DrawSeparator(im, rect, s->m_penSeparatorDark, s->m_penSeparator2, !pSlider->IsVert());
  if (pSlider->IsVert()) {
    rect.t += rect.Height() / 2 - 1;
    rect.b = rect.t + 2;
    rect.InflateRect(2, 0);
  }
  else {
    rect.l += rect.Width() / 2 - 1;
    rect.r = rect.l + 2;
    rect.InflateRect(0, 2);
  }
  DrawSeparator(im, rect, s->m_penSeparatorDark, s->m_penSeparator2, pSlider->IsVert());
}
int VisualManager2007_OnDrawRibbonSliderThumb(
    img_t* im, RibbonSlider* pSlider,
    IRECT rect, BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawRibbonSliderThumb(
        im, pSlider, rect, bIsHighlighted, bIsPressed, bIsDisabled);
    return 0;
  }
  BOOL bIsVert = FALSE;
  BOOL bLeftTop = FALSE;
  BOOL bRightBottom = FALSE;
  if (pSlider != NULL) {
    ASSERT_VALID(pSlider);
    bIsVert = pSlider->IsVert();
    bLeftTop = pSlider->IsThumbLeftTop();
    bRightBottom = pSlider->IsThumbRightBottom();
  }
  int indexRen = 0;
  if (bLeftTop && bRightBottom) {
    indexRen = 1;
  }
  else if (bLeftTop) {
    indexRen = 2;
  }
  if (bIsVert) {
    indexRen += 3;
  }
  ControlRenderer* pRenderer = NULL;
  if (indexRen == 0) {
    pRenderer = &s->m_ctrlRibbonSliderThumb;
  }
  else {
    pRenderer = &s->m_ctrlRibbonSliderThumbA[indexRen - 1];
  }
  if (pRenderer != NULL) {
    int index = 0;
    if (bIsDisabled) {
      index = 3;
    }
    else {
      if (bIsPressed) {
        index = 2;
      }
      else if (bIsHighlighted) {
        index = 1;
      }
    }
    pRenderer->FillInterior(im, rect,
        ToolBarImages::ImageAlignHorzCenter,
        ImageAlignVertCenter,
        index);
  }
}
int VisualManager2007_OnDrawRibbonProgressBar(img_t* im,
    RibbonProgressBar* pProgress,
    IRECT rectProgress, IRECT rectChunk,
    BOOL bInfiniteMode)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawRibbonProgressBar(im, pProgress,
        rectProgress, rectChunk, bInfiniteMode);
    return 0;
  }
  ASSERT_VALID(pProgress);
  s->m_ctrlRibbonProgressBack.Draw(im, rectProgress);
  IRECT rectInternal(rectProgress);
  IRECT rectBorders(s->m_ctrlRibbonProgressBack.GetParams().s->m_rectCorners);
  rectInternal.DeflateRect(rectBorders.l, rectBorders.t, rectBorders.r, rectBorders.b);
  if (!bInfiniteMode) {
    // normal
    rectChunk.IntersectRect(rectChunk, rectInternal);
    if (!rectChunk.IsRectEmpty() || pProgress->GetPos() != pProgress->GetRangeMin()) {
      CRgn rgn;
      rgn.CreateRectRgnIndirect(rectInternal);
      im->SelectClipRgn(&rgn);
      if (!rectChunk.IsRectEmpty()) {
        rectChunk.l = rectChunk.r - rectInternal.Width();
        s->m_ctrlRibbonProgressNormal.Draw(im, rectChunk);
      }
      else {
        rectChunk = rectInternal;
        rectChunk.r = rectInternal.l;
      }
      if (rectChunk.r != rectInternal.r) {
        rectChunk.l = rectChunk.r;
        rectChunk.r += s->m_ctrlRibbonProgressNormalExt.GetParams().s->m_rectImage.Width();
        s->m_ctrlRibbonProgressNormalExt.Draw(im, rectChunk);
      }
      im->SelectClipRgn(NULL);
    }
  }
  else if (pProgress->GetPos() != pProgress->GetRangeMin()) {
    int index = (pProgress->GetPos() - pProgress->GetRangeMin()) %
        s->m_ctrlRibbonProgressInfinity.GetImageCount();
    s->m_ctrlRibbonProgressInfinity.Draw(im, rectInternal, index);
  }
}
int VisualManager2007_OnFillRibbonQATPopup(
    img_t* im, RibbonPanelMenuBar* pMenuBar, IRECT rect)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnFillRibbonQATPopup(im, pMenuBar, rect);
    return 0;
  }
  if (s->m_ctrlRibbonBorder_QAT.IsValid()) {
    s->m_ctrlRibbonBorder_QAT.FillInterior(im, rect);
  }
  else {
    DrawManager dm(*im);
    dm.FillGradient(rect, s->m_clrBarGradientDark, s->m_clrBarGradientLight, TRUE);
  }
}
int VisualManager2007_GetRibbonPopupBorderSize(const RibbonPanelMenu* pPopup) const
{
  if (!CanDrawImage()) {
    return VisualManager2003_GetRibbonPopupBorderSize(pPopup);
  }
  if (pPopup != NULL) {
    ASSERT_VALID(pPopup);
    RibbonPanelMenuBar* pRibbonMenuBar =
        DYNAMIC_DOWNCAST(RibbonPanelMenuBar,
            (const_cast<RibbonPanelMenu*>(pPopup))->GetMenuBar());
    if (pRibbonMenuBar != NULL) {
      if (pRibbonMenuBar->IsMainPanel()) {
        return(int)GetPopupMenuBorderSize();
      }
      if (!pRibbonMenuBar->IsMenuMode()) {
        if (pRibbonMenuBar->IsQATPopup()) {
          if (s->m_ctrlRibbonBorder_QAT.IsValid()) {
            return s->m_ctrlRibbonBorder_QAT.GetParams().s->m_rectSides.l;
          }
        }
        else if (pRibbonMenuBar->IsCategoryPopup()) {
          return 0;
        }
        else if (pRibbonMenuBar->IsFloaty()) {
          if (s->m_ctrlRibbonBorder_Floaty.IsValid()) {
            return s->m_ctrlRibbonBorder_Floaty.GetParams().s->m_rectSides.l;
          }
        }
        else {
          if (pRibbonMenuBar->GetPanel() != NULL) {
            if (!IsBeta1()) {
              return 0;
            }
          }
          // standard size
        }
      }
    }
  }
  return(int)GetPopupMenuBorderSize();
}
int VisualManager2007_OnDrawRibbonKeyTip(img_t* im,
    BaseRibbonElement* pElement,
    IRECT rect, CString str)
{
  if (!CanDrawImage() ||
      !s->m_ctrlRibbonKeyTip.IsValid()) {
    VisualManager2003_OnDrawRibbonKeyTip(im, pElement, rect, str);
    return 0;
  }
  ASSERT_VALID(pElement);
  BOOL bDisabled = pElement->IsDisabled();
  s->m_ctrlRibbonKeyTip.Draw(im, rect, 0);
  str.MakeUpper();
  COLOR clrTextOld = im->SetTextColor(
      bDisabled ? s->m_clrRibbonKeyTipTextDisabled : s->m_clrRibbonKeyTipTextNormal);
  im->DrawText(str, rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
  im->SetTextColor(clrTextOld);
}
int VisualManager2007_OnDrawRibbonCheckBoxOnList(img_t* im, RibbonCheckBox* pCheckBox,
    IRECT rect, BOOL bIsSelected, BOOL bHighlighted)
{
  ToolBarImages& img = s->m_MenuItemMarkerC;
  if (!CanDrawImage() || img.GetCount() == 0) {
    VisualManager2003_OnDrawRibbonCheckBoxOnList(im, pCheckBox, rect, bIsSelected, bHighlighted);
    return 0;
  }
  if (s->GetRibbonImageScale() != 1.0) {
    rect.DeflateRect(5, 5);
    img.DrawEx(im, rect, 0, ToolBarImages::ImageAlignHorzStretch, ImageAlignVertStretch);
  }
  else {
    img.DrawEx(im, rect, 0, ToolBarImages::ImageAlignHorzCenter, ImageAlignVertCenter);
  }
}
int VisualManager2007_OnDrawRibbonRadioButtonOnList(img_t* im, RibbonRadioButton* pRadioButton,
    IRECT rect, BOOL bIsSelected, BOOL bHighlighted)
{
  ToolBarImages& img = s->m_MenuItemMarkerR;
  if (!CanDrawImage() || img.GetCount() == 0) {
    VisualManager2003_OnDrawRibbonRadioButtonOnList(im, pRadioButton, rect, bIsSelected, bHighlighted);
    return 0;
  }
  if (s->GetRibbonImageScale() != 1.0) {
    rect.DeflateRect(5, 5);
    img.DrawEx(im, rect, 0, ToolBarImages::ImageAlignHorzStretch, ImageAlignVertStretch);
  }
  else {
    img.DrawEx(im, rect, 0, ToolBarImages::ImageAlignHorzCenter, ImageAlignVertCenter);
  }
}
COLOR VisualManager2007_GetRibbonHyperlinkTextColor(RibbonHyperlink* pHyperLink)
{
  ASSERT_VALID(pHyperLink);
  if (!CanDrawImage() || pHyperLink->IsDisabled()) {
    return VisualManager2003_GetRibbonHyperlinkTextColor(pHyperLink);
  }
  COLOR clrText = pHyperLink->IsHighlighted() ?
      s->m_clrRibbonHyperlinkActive : s->m_clrRibbonHyperlinkInactive;
  if (s->m_clrRibbonStatusbarHyperlinkActive != (COLOR) - 1 &&
      s->m_clrRibbonStatusbarHyperlinkInactive != (COLOR) - 1) {
    RibbonStatusBar* pParentStatusBar = DYNAMIC_DOWNCAST(
        RibbonStatusBar, pHyperLink->GetParentRibbonBar());
    if (pParentStatusBar != NULL) {
      ASSERT_VALID(pParentStatusBar);
      if (!pParentStatusBar->IsExtendedElement(pHyperLink)) {
        clrText = pHyperLink->IsHighlighted() ?
            s->m_clrRibbonStatusbarHyperlinkActive : s->m_clrRibbonStatusbarHyperlinkInactive;
      }
    }
  }
  if (clrText == (COLOR) - 1) {
    return VisualManager2003_GetRibbonHyperlinkTextColor(pHyperLink);
  }
  return clrText;
}
COLOR VisualManager2007_GetRibbonStatusBarTextColor(RibbonStatusBar* pStatusBar)
{
  if (!CanDrawImage()) {
    return VisualManager2003_GetRibbonStatusBarTextColor(pStatusBar);
  }
  return s->m_clrStatusBarText;
}
#endif // BCGP_EXCLUDE_RIBBON
#ifndef BCGP_EXCLUDE_PLANNER
int VisualManager2007_OnFillPlanner(img_t* im, PlannerView* pView,
    IRECT rect, BOOL bWorkingArea)
{
  ASSERT_VALID(pView);
  if (!CanDrawImage()) {
    VisualManager2003_OnFillPlanner(im, pView, rect, bWorkingArea);
    return 0;
  }
  if (s->m_bPlannerBackItemSelected) {
    CBrush br(GetPlannerHourLineColor(pView, TRUE, FALSE));
    imdraw_FillRect(im, 0, rect, &br);
  }
  else {
    VisualManager2003_OnFillPlanner(im, pView, rect, bWorkingArea);
  }
  if (s->m_bPlannerBackItemToday && DYNAMIC_DOWNCAST(PlannerViewDay, pView) == NULL) {
    rect.r--;
    imdraw_Draw3dRect(im, 0, rect, s->m_clrPlannerTodayBorder, s->m_clrPlannerTodayBorder);
    rect.l--;
    rect.r++;
    rect.b++;
    imdraw_Draw3dRect(im, 0, rect, s->m_clrPlannerTodayBorder, s->m_clrPlannerTodayBorder);
  }
}
COLOR VisualManager2007_OnFillPlannerCaption(img_t* im,
    PlannerView* pView, IRECT rect, BOOL bIsToday, BOOL bIsSelected,
    BOOL bNoBorder/* = FALSE*/, BOOL bHorz /*= TRUE*/)
{
  if (!CanDrawImage()) {
    return VisualManager2003_OnFillPlannerCaption(im,
        pView, rect, bIsToday, bIsSelected, bNoBorder, bHorz);
  }
  const BOOL bMonth = DYNAMIC_DOWNCAST(PlannerViewMonth, pView) != NULL;
  if (bMonth && s->m_bPlannerCaptionBackItemHeader) {
    return s->m_clrPlannerNcText;
  }
  BOOL bDay = FALSE;
  if (!bMonth) {
    bDay = pView->IsKindOf(RUNTIME_CLASS(PlannerViewDay));
    if (bDay) {
      if (!bIsToday) {
        rect.DeflateRect(1, 1);
      }
    }
  }
  else {
    if (!bIsToday) {
      rect.b--;
    }
  }
  COLOR clrText = _RGB(0, 0, 0);
  COLOR clrBorder = CLR_DEFAULT;
  if (bIsToday) {
    IRECT rectHalf(rect);
    DrawManager dm(*im);
    if (bHorz) {
      rectHalf.b = rectHalf.t + rectHalf.Height() / 2;
      dm.Fill4ColorsGradient(rectHalf,
          s->m_clrPlannerTodayCaption[0], s->m_clrPlannerTodayCaption[1],
          s->m_clrPlannerTodayCaption[1], s->m_clrPlannerTodayCaption[0], FALSE);
      rectHalf.t = rectHalf.b;
      rectHalf.b = rect.b;
      dm.Fill4ColorsGradient(rectHalf,
          s->m_clrPlannerTodayCaption[2], s->m_clrPlannerTodayCaption[3],
          s->m_clrPlannerTodayCaption[3], s->m_clrPlannerTodayCaption[2], FALSE);
    }
    else {
      rectHalf.r = rectHalf.l + rectHalf.Width() / 2;
      dm.Fill4ColorsGradient(rectHalf,
          s->m_clrPlannerTodayCaption[1], s->m_clrPlannerTodayCaption[0],
          s->m_clrPlannerTodayCaption[0], s->m_clrPlannerTodayCaption[1], TRUE);
      rectHalf.l = rectHalf.r;
      rectHalf.r = rect.r;
      dm.Fill4ColorsGradient(rectHalf,
          s->m_clrPlannerTodayCaption[3], s->m_clrPlannerTodayCaption[2],
          s->m_clrPlannerTodayCaption[2], s->m_clrPlannerTodayCaption[3], TRUE);
    }
    clrBorder = s->m_clrPlannerTodayBorder;
  }
  else {
    COLOR clrBack = GetPlannerViewBackgroundColor(pView);
    double H, S, V;
    DrawManager::RGBtoHSV(GetPlannerViewBackgroundColor(pView), &H, &S, &V);
    DrawManager dm(*im);
    if (bHorz) {
      dm.Fill4ColorsGradient(rect,
          DrawManager::HSVtoRGB(H, S * 0.40, min(V * 1.09, 1.0)),
          DrawManager::HSVtoRGB(H, S * 0.20, min(V * 1.12, 1.0)),
          DrawManager::HSVtoRGB(H, S * 0.37, min(V * 1.10, 1.0)),
          DrawManager::HSVtoRGB(H, S * 0.48, min(V * 1.08, 1.0)),
          TRUE);
    }
    else {
      dm.Fill4ColorsGradient(rect,
          DrawManager::HSVtoRGB(H, S * 0.20, min(V * 1.12, 1.0)),
          DrawManager::HSVtoRGB(H, S * 0.40, min(V * 1.09, 1.0)),
          DrawManager::HSVtoRGB(H, S * 0.48, min(V * 1.08, 1.0)),
          DrawManager::HSVtoRGB(H, S * 0.37, min(V * 1.10, 1.0)),
          FALSE);
    }
    if (!bDay) {
      clrBorder = clrBack;
    }
  }
  if (clrBorder != CLR_DEFAULT && !bNoBorder) {
    if (!bDay) {
      rect.InflateRect(1, 0);
    }
    imdraw_Draw3dRect(im, 0, rect, clrBorder, clrBorder);
  }
  return clrText;
}
int VisualManager2007_OnDrawPlannerCaptionText(img_t* im,
    PlannerView* pView, IRECT rect, const CString& strText,
    COLOR clrText, int nAlign, BOOL bHighlight)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawPlannerCaptionText(im,
        pView, rect, strText, clrText, nAlign, bHighlight);
    return 0;
  }
  const int nTextMargin = 2;
  rect.DeflateRect(nTextMargin, 0);
  COLOR clrOld = im->SetTextColor(clrText);
  im->DrawText(strText, rect, DT_SINGLELINE | DT_VCENTER | nAlign);
  im->SetTextColor(clrOld);
}
COLOR VisualManager2007_GetPlannerAppointmentTimeColor(PlannerView* pView,
    BOOL bSelected, BOOL bSimple, DWORD dwDrawFlags)
{
  if (!CanDrawImage() ||
      (bSelected && (dwDrawFlags & BCGP_PLANNER_DRAW_APP_OVERRIDE_SELECTION) == 0)) {
    return VisualManager2003_GetPlannerAppointmentTimeColor(pView,
        bSelected, bSimple, dwDrawFlags);
  }
  double H, S, V;
  DrawManager::RGBtoHSV(GetPlannerViewBackgroundColor(pView), &H, &S, &V);
  return DrawManager::HSVtoRGB(H, min(S * 1.88, 1.0), V * 0.5);
}
// in the future versions use base function
COLOR VisualManager2007_GetPlannerViewBackgroundColor(PlannerView* pView)
{
  if (!CanDrawImage()) {
    return VisualManager2003_GetPlannerViewBackgroundColor(pView);
  }
  ASSERT_VALID(pView);
  PlannerManagerCtrl* pCtrl = pView->GetPlanner();
  ASSERT_VALID(pCtrl);
  return pCtrl->GetBackgroundColor() == CLR_DEFAULT
      ? s->m_clrPlannerWork
      : pCtrl->GetBackgroundColor();
}
COLOR VisualManager2007_GetPlannerHourLineColor(PlannerView* pView,
    BOOL bWorkingHours, BOOL bHour)
{
  if (!CanDrawImage()) {
    return VisualManager2003_GetPlannerHourLineColor(pView,
        bWorkingHours, bHour);
  }
  ASSERT_VALID(pView);
  return CalculateHourLineColor2007(GetPlannerViewBackgroundColor(pView), bWorkingHours, bHour);
}
COLOR VisualManager2007_GetPlannerViewWorkingColor(PlannerView* pView)
{
  if (!CanDrawImage()) {
    return VisualManager2003_GetPlannerViewWorkingColor(pView);
  }
  ASSERT_VALID(pView);
  return CalculateWorkingColor2007(GetPlannerViewBackgroundColor(pView));
}
COLOR VisualManager2007_GetPlannerViewNonWorkingColor(PlannerView* pView)
{
  if (!CanDrawImage()) {
    return VisualManager2003_GetPlannerViewNonWorkingColor(pView);
  }
  ASSERT_VALID(pView);
  return CalculateNonWorkingColor2007(GetPlannerViewBackgroundColor(pView),
      pView->IsKindOf(RUNTIME_CLASS(PlannerViewDay)));
}
COLOR VisualManager2007_GetPlannerSelectionColor(PlannerView* pView)
{
  if (!CanDrawImage()) {
    return VisualManager2003_GetPlannerSelectionColor(pView);
  }
  ASSERT_VALID(pView);
  return CalculateSelectionColor(GetPlannerViewBackgroundColor(pView));
}
COLOR VisualManager2007_GetPlannerSeparatorColor(PlannerView* pView)
{
  if (!CanDrawImage()) {
    return VisualManager2003_GetPlannerSeparatorColor(pView);
  }
  ASSERT_VALID(pView);
  return CalculateSeparatorColor(GetPlannerViewBackgroundColor(pView));
}
COLOR VisualManager2007_OnFillPlannerTimeBar(img_t* im,
    PlannerView* pView, IRECT rect, COLOR& clrLine)
{
  if (!CanDrawImage()) {
    return VisualManager2003_OnFillPlannerTimeBar(im, pView, rect, clrLine);
  }
  CBrush br(s->m_clrPlannerNcArea);
  imdraw_FillRect(im, 0, rect, &br);
  clrLine = s->m_clrPlannerNcLine;
  return s->m_clrPlannerNcText;
}
int VisualManager2007_OnFillPlannerWeekBar(img_t* im,
    PlannerView* pView, IRECT rect)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnFillPlannerWeekBar(im, pView, rect);
    return 0;
  }
  CBrush br(s->m_clrPlannerNcArea);
  imdraw_FillRect(im, 0, rect, &br);
}
int VisualManager2007_OnDrawPlannerHeader(img_t* im,
    PlannerView* pView, IRECT rect)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawPlannerHeader(im, pView, rect);
    return 0;
  }
  COLOR clr = GetPlannerViewBackgroundColor(pView);
  if (DYNAMIC_DOWNCAST(PlannerViewMonth, pView) != NULL) {
    clr = s->m_clrPlannerNcArea;
  }
  CBrush br(clr);
  imdraw_FillRect(im, 0, rect, &br);
}
int VisualManager2007_OnDrawPlannerHeaderPane(img_t* im,
    PlannerView* pView, IRECT rect)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawPlannerHeaderPane(im, pView, rect);
    return 0;
  }
  if (DYNAMIC_DOWNCAST(PlannerViewMonth, pView) != NULL) {
    imdraw_Draw3dRect(im, 0, rect.r - 1, rect.t - 2, 1, rect.Height() + 4,
        s->m_clrPlannerNcText, s->m_clrPlannerNcText);
  }
}
int VisualManager2007_OnFillPlannerHeaderAllDay(img_t* im,
    PlannerView* pView, IRECT rect)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnFillPlannerHeaderAllDay(im, pView, rect);
    return 0;
  }
  CBrush br(GetPlannerHourLineColor(pView, FALSE, TRUE));
  imdraw_FillRect(im, 0, rect, &br);
}
int VisualManager2007_OnDrawPlannerHeaderAllDayItem(img_t* im,
    PlannerView* pView, IRECT rect, BOOL bIsToday, BOOL bIsSelected)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawPlannerHeaderAllDayItem(im, pView, rect,
        bIsToday, bIsSelected);
    return 0;
  }
  rect.l++;
  if (bIsSelected) {
    CBrush br(GetPlannerSelectionColor(pView));
    imdraw_FillRect(im, 0, rect, &br);
  }
  if (bIsToday) {
    rect.t--;
    rect.r--;
    imdraw_Draw3dRect(im, 0, rect, s->m_clrPlannerTodayBorder, s->m_clrPlannerTodayBorder);
    rect.l--;
    rect.r++;
    rect.b++;
    imdraw_Draw3dRect(im, 0, rect, s->m_clrPlannerTodayBorder, s->m_clrPlannerTodayBorder);
  }
}
int VisualManager2007_PreparePlannerBackItem(BOOL bIsToday, BOOL bIsSelected)
{
  s->m_bPlannerBackItemToday = bIsToday;
  s->m_bPlannerBackItemSelected = bIsSelected;
}
DWORD VisualManager2007_GetPlannerDrawFlags() const
{
  if (!CanDrawImage()) {
    return VisualManager2003_GetPlannerDrawFlags();
  }
  return BCGP_PLANNER_DRAW_APP_GRADIENT_FILL |
      BCGP_PLANNER_DRAW_APP_ROUNDED_CORNERS |
      BCGP_PLANNER_DRAW_APP_OVERRIDE_SELECTION |
      BCGP_PLANNER_DRAW_APP_NO_MULTIDAY_CLOCKS |
      BCGP_PLANNER_DRAW_APP_DURATION_SHAPE |
      BCGP_PLANNER_DRAW_VIEW_NO_DURATION |
      BCGP_PLANNER_DRAW_VIEW_WEEK_BAR |
      BCGP_PLANNER_DRAW_VIEW_CAPTION_DAY_BOLD |
      BCGP_PLANNER_DRAW_VIEW_CAPTION_DAY_COMPACT;
}
#endif // BCGP_EXCLUDE_PLANNER
CSize VisualManager2007_GetSystemBorders(BOOL bRibbonPresent) const
{
  CSize size(::GetSystemMetrics(SM_CYSIZEFRAME),
      ::GetSystemMetrics(SM_CXSIZEFRAME));
  if (bRibbonPresent) {
    size.w--;
    size.h--;
  }
  return size;
}
BOOL VisualManager2007_OnEraseMDIClientArea(img_t* im, IRECT rectClient)
{
  if (!CanDrawImage() ||
      s->m_clrMainClientArea.GetSafeHandle() == NULL) {
    return VisualManager2003_OnEraseMDIClientArea(im, rectClient);
  }
  imdraw_FillRect(im, 0, rectClient, s->m_clrMainClientArea);
  return TRUE;
}
BOOL VisualManager2007_GetToolTipParams(ToolTipParams& params,
    UINT /*nType*/ /*=(UINT)(-1)*/)
{
  if (!CanDrawImage() ||
      !s->m_bToolTipParams) {
    return VisualManager2003_GetToolTipParams(params);
  }
  params = s->m_ToolTipParams;
  return TRUE;
}
int VisualManager2007_OnScrollBarDrawThumb(img_t* im, ScrollBar* pScrollBar, IRECT rect,
    BOOL bHorz, BOOL bHighlighted, BOOL bPressed, BOOL bDisabled)
{
  if (!CanDrawImage()) {
    return 0;
  }
  int nScroll = bHorz ? 0 : 1;
  BOOL bIsFrame = pScrollBar->GetVisualStyle() == ScrollBar::BCGP_SBSTYLE_VISUAL_MANAGER_FRAME;
  int nIndex = bIsFrame ? 0 : 1;
  s->m_ctrlScrollBar_Back[nScroll][nIndex].Draw(im, rect, 0);
  if (!bDisabled) {
    if (!IsBeta()) {
      if (bHorz) {
        rect.DeflateRect(0, 1);
      }
      else {
        rect.DeflateRect(1, 0);
      }
    }
    s->m_ctrlScrollBar_ThumbBack[nScroll][nIndex].Draw(im, rect, bPressed ? 2 : bHighlighted ? 1 : 0);
    if (rect.Width() - 4 > s->m_ctrlScrollBar_ThumbIcon[nScroll][nIndex].GetParams().s->m_rectImage.Width() &&
        rect.Height() - 4 > s->m_ctrlScrollBar_ThumbIcon[nScroll][nIndex].GetParams().s->m_rectImage.Height()) {
      rect.OffsetRect(1, 1);
      s->m_ctrlScrollBar_ThumbIcon[nScroll][nIndex].FillInterior(im, rect,
          ToolBarImages::ImageAlignHorzCenter, ImageAlignVertCenter,
          bPressed ? 2 : bHighlighted ? 1 : 0);
    }
  }
}
int VisualManager2007_OnScrollBarDrawButton(img_t* im, ScrollBar* pScrollBar, IRECT rect,
    BOOL bHorz, BOOL bHighlighted, BOOL bPressed, BOOL bFirst, BOOL bDisabled)
{
  if (!CanDrawImage()) {
    return 0;
  }
  int nScroll = bHorz ? 0 : 1;
  BOOL bIsFrame = pScrollBar->GetVisualStyle() == ScrollBar::BCGP_SBSTYLE_VISUAL_MANAGER_FRAME;
  int nIndex = bIsFrame ? 0 : 1;
  s->m_ctrlScrollBar_Back[nScroll][nIndex].Draw(im, rect, 0);
  if (!bDisabled) {
    if (!IsBeta()) {
      if (bHorz) {
        rect.DeflateRect(0, 1);
      }
      else {
        rect.DeflateRect(1, 0);
      }
    }
    s->m_ctrlScrollBar_Item[nScroll][nIndex].Draw(im, rect, bPressed ? 3 : bHighlighted ? 2 : pScrollBar->IsActive() ? 1 : 0);
  }
  MenuImages::IMAGES_IDS ids;
  if (bHorz) {
    ids = bFirst ? MenuImages::IdArowLeftLarge : MenuImages::IdArowRightLarge;
  }
  else {
    ids = bFirst ? MenuImages::IdArowUpLarge : MenuImages::IdArowDownLarge;
  }
  MenuImages::IMAGE_STATE state = bDisabled ? MenuImages::ImageGray : MenuImages::ImageBlack2;
  if (!pScrollBar->IsActive() && s->m_Style == VS2007_ObsidianBlack && bIsFrame) {
    state = MenuImages::ImageLtGray;
  }
  MenuImages::Draw(im, ids, rect, state);
}
int VisualManager2007_OnScrollBarFillBackground(img_t* im, ScrollBar* pScrollBar, IRECT rect,
    BOOL bHorz, BOOL /*bHighlighted*/, BOOL bPressed, BOOL /*bFirst*/, BOOL /*bDisabled*/)
{
  if (!CanDrawImage()) {
    return 0;
  }
  int nScroll = bHorz ? 0 : 1;
  BOOL bIsFrame = pScrollBar->GetVisualStyle() == ScrollBar::BCGP_SBSTYLE_VISUAL_MANAGER_FRAME;
  int nIndex = bIsFrame ? 0 : 1;
  s->m_ctrlScrollBar_Back[nScroll][nIndex].Draw(im, rect, bPressed ? 1 : 0);
}
int VisualManager2007_GetCalendarColors(const Calendar* pCalendar,
    CalendarColors& colors)
{
  VisualManager2003_GetCalendarColors(pCalendar, colors);
  if (!CanDrawImage()) {
    return 0;
  }
  colors.clrCaption = s->m_clrCaptionBarGradientDark;
  colors.clrCaptionText = s->m_clrCaptionBarText;
  colors.clrSelected = s->m_clrHighlightGradientDark;
  colors.clrSelectedText = s->m_clrOutlookPageTextHighlighted;
}
#ifndef BCGP_EXCLUDE_POPUP_WINDOW
int VisualManager2007_OnFillPopupWindowBackground(img_t* im, IRECT rect)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnFillPopupWindowBackground(im, rect);
    return 0;
  }
  DrawManager dm(*im);
  dm.FillGradient(rect, s->m_clrPopupGradientDark, s->m_clrPopupGradientLight);
}
COLOR VisualManager2007_OnDrawPopupWindowCaption(img_t* im, IRECT rectCaption, PopupWindow* pPopupWnd)
{
  COLOR clrText = VisualManager2003_OnDrawPopupWindowCaption(im, rectCaption, pPopupWnd);
  if (CanDrawImage()) {
    clrText = s->m_clrOutlookCaptionTextNormal;
  }
  return clrText;
}
#endif
COLOR VisualManager2007_OnFillListBoxItem(img_t* im, ListBox* pListBox, int nItem, IRECT rect, BOOL bIsHighlihted, BOOL bIsSelected)
{
  if (!CanDrawImage() || !s->m_ctrlRibbonBtn[0].IsValid()) {
    return VisualManager2003_OnFillListBoxItem(im, pListBox, nItem, rect, bIsHighlihted, bIsSelected);
  }
  rect.DeflateRect(2, 1);
  int nIndex = 0;
  if (bIsSelected) {
    nIndex = bIsHighlihted ? 1 : 2;
  }
  s->m_ctrlRibbonBtn [0].Draw(im, rect, nIndex);
  return s->m_clrToolBarBtnTextHighlighted;
}
COLOR VisualManager2007_OnDrawMenuLabel(img_t* im, IRECT rect)
{
  imdraw_FillRect(im, 0, rect,
      s->m_clrGroupBackground.IsValid() ? s->m_clrGroupBackground : s->m_clrBarBkgnd);
  IRECT rectSeparator = rect;
  rectSeparator.t = rectSeparator.b - 2;
  DrawSeparator(im, rectSeparator, TRUE);
  return s->m_clrGroupText != (COLOR) - 1 ? s->m_clrGroupText : s->m_clrMenuText;
}
COLOR VisualManager2007_OnFillCaptionBarButton(img_t* im, CaptionBar* pBar,
    IRECT rect, BOOL bIsPressed, BOOL bIsHighlighted,
    BOOL bIsDisabled, BOOL bHasDropDownArrow,
    BOOL bIsSysButton)
{
  COLOR clrText = VisualManager2003_OnFillCaptionBarButton(im, pBar,
      rect, bIsPressed, bIsHighlighted,
      bIsDisabled, bHasDropDownArrow, bIsSysButton);
  ASSERT_VALID(pBar);
  if (CanDrawImage() &&
      pBar->IsMessageBarMode() && bIsSysButton && !bIsHighlighted) {
    clrText = s->m_clrMenuBarBtnText;
  }
  return clrText;
}
BOOL VisualManager2007_OnDrawPushButton(img_t* im, IRECT rect, Button* pButton, COLOR& clrText)
{
  if (!CanDrawImage()) {
    return VisualManager2003_OnDrawPushButton(im, rect, pButton, clrText);
  }
  int index = 0;
  BOOL bDisabled = !pButton->IsWindowEnabled();
  BOOL bFocused = pButton->GetSafeHwnd() == ::GetFocus();
  BOOL bDefault = pButton->IsDefaultButton();
  BOOL bPressed = pButton->IsPressed();
  BOOL bChecked = pButton->IsChecked();
  BOOL bHighlighted = pButton->IsHighlighted();
  bHighlighted |= bFocused;
  if (bDisabled) {
    index = 6;
  }
  else {
    if (bChecked) {
      index = 3;
    }
    else {
      if (bDefault && !bHighlighted) {
        index = 5;
      }
    }
    if (bPressed) {
      if (bHighlighted) {
        index = 2;
      }
    }
    else if (bHighlighted) {
      index++;
    }
  }
  s->DrawParentBackground(pButton, im);
  s->m_ctrlRibbonBtnPush.Draw(im, rect, index);
  if (bDisabled) {
    clrText = s->m_clrToolBarBtnTextDisabled;
  }
  else if (bHighlighted) {
    clrText = s->m_clrToolBarBtnTextHighlighted;
  }
  return TRUE;
}
int VisualManager2007_OnDrawGroup(img_t* im, Group* pGroup, IRECT rect, const CString& strName)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawGroup(im, pGroup, rect, strName);
    return 0;
  }
  ASSERT_VALID(pGroup);
  CSize sizeText = im->GetTextExtent(strName);
  IRECT rectFrame = rect;
  rectFrame.t += sizeText.h / 2;
  if (sizeText == CSize(0, 0)) {
    rectFrame.t += im->GetTextExtent(("A")).h / 2;
  }
  int xMargin = sizeText.h / 2;
  IRECT rectText = rect;
  rectText.l += xMargin;
  rectText.r = rectText.l + sizeText.w + xMargin;
  rectText.b = rectText.t + sizeText.h;
  if (!strName.IsEmpty()) {
    im->ExcludeClipRect(rectText);
  }
  s->m_ctrlRibbonBtnGroup.DrawFrame(im, rectFrame);
  im->SelectClipRgn(NULL);
  if (strName.IsEmpty()) {
    return 0;
  }
  DWORD dwTextStyle = DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_NOCLIP;
  if (pGroup->s->m_bOnGlass) {
    DrawTextOnGlass(im, strName, rectText, dwTextStyle, 10, s->clrBarText);
  }
  else {
    CString strCaption = strName;
    im->DrawText(strCaption, rectText, dwTextStyle);
  }
}
BOOL VisualManager2007_OnFillDialog(img_t* im, CWnd* pDlg, IRECT rect)
{
  if (!CanDrawImage() || s->m_clrDlgBackground.GetSafeHandle() == NULL) {
    return VisualManager2003_OnFillDialog(im, pDlg, rect);
  }
  imdraw_FillRect(im, 0, rect, &GetDlgBackBrush(pDlg));
  return TRUE;
}
int VisualManager2007_OnDrawSliderChannel(img_t* im, SliderCtrl* pSlider, BOOL bVert, IRECT rect, BOOL bDrawOnGlass)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawSliderChannel(im, pSlider, bVert, rect, bDrawOnGlass);
    return 0;
  }
  DrawSeparator(im, rect, s->m_penSeparatorDark, s->m_penSeparator2, !bVert);
}
int VisualManager2007_OnDrawSliderThumb(img_t* im, SliderCtrl* pSlider,
    IRECT rect, BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled,
    BOOL bVert, BOOL bLeftTop, BOOL bRightBottom,
    BOOL bDrawOnGlass)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawSliderThumb(
        im, pSlider, rect, bIsHighlighted, bIsPressed, bIsDisabled,
        bVert, bLeftTop, bRightBottom, bDrawOnGlass);
    return 0;
  }
  int indexRen = 0;
  if (bLeftTop && bRightBottom) {
    indexRen = 1;
  }
  else if (bLeftTop) {
    indexRen = 2;
  }
  if (bVert) {
    indexRen += 3;
  }
  ControlRenderer* pRenderer = &s->m_ctrlSliderThumb[indexRen];
  if (pRenderer != NULL) {
    int index = 0;
    if (bIsDisabled) {
      index = 3;
    }
    else {
      if (bIsPressed) {
        index = 2;
      }
      else if (bIsHighlighted) {
        index = 1;
      }
    }
    pRenderer->Draw(im, rect, index);
  }
}
BCGP_SMARTDOCK_THEME VisualManager2007_GetSmartDockingTheme()
{
  if (s->m_nBitsPerPixel <= 8 || IsHighContastMode(s) || !s->IsWindowsLayerSupportAvailable() ||
      !s->bIsWindowsVista) {
    return VisualManager2003_GetSmartDockingTheme();
  }
  return BCGP_SDT_VS2008;
}
//
// BCGPVisualManagerCarbon.cpp: implementation of the VisualManagerCarbon class.
//
//////////////////////////////////////////////////////////////////////
//#include "BCGCBPro.h"
#include "BCGPGlobalUtils.h"
#include "BCGPVisualManagerCarbon.h"
#include "BCGPToolBar.h"
#include "BCGPDrawManager.h"
#include "BCGPPopupMenuBar.h"
#include "BCGPMenuBar.h"
#include "bcgglobals.h"
#include "BCGPToolbarMenuButton.h"
#include "CustomizeButton.h"
#include "MenuImages.h"
#include "BCGPCaptionBar.h"
#include "BCGPBaseTabWnd.h"
#include "BCGPColorBar.h"
#include "BCGPCalculator.h"
#include "BCGPCalendarBar.h"
#include "BCGPTabWnd.h"
#include "BCGPTasksPane.h"
#include "BCGPStatusBar.h"
#include "BCGPAutoHideButton.h"
#include "BCGPHeaderCtrl.h"
#include "BCGPReBar.h"
#include "BCGPToolBox.h"
#include "BCGPPopupWindow.h"
#include "BCGPCalendarBar.h"
#include "BCGPDropDown.h"
#include "BCGPTagManager.h"
#include "BCGPFrameWnd.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPDockingControlBar.h"
#include "BCGPOutlookWnd.h"
#include "BCGPGridCtrl.h"
#include "BCGPToolbarComboBoxButton.h"
#include "BCGPToolbarEditBoxButton.h"
#include "BCGCBProVer.h"
#include "BCGPGroup.h"
#include "BCGPDialog.h"
#include "BCGPPropertySheet.h"
#include "BCGPOutlookButton.h"
#include "BCGPShowAllButton.h"
#ifndef BCGP_EXCLUDE_RIBBON
#include "BCGPRibbonBar.h"
#include "BCGPRibbonPanel.h"
#include "BCGPRibbonCategory.h"
#include "BCGPRibbonButton.h"
#include "BCGPRibbonQuickAccessToolbar.h"
#include "BCGPRibbonComboBox.h"
#include "BCGPRibbonMainPanel.h"
#include "BCGPRibbonPanelMenu.h"
#include "BCGPRibbonLabel.h"
#include "BCGPRibbonPaletteButton.h"
#include "BCGPRibbonStatusBar.h"
#include "BCGPRibbonStatusBarPane.h"
#include "BCGPRibbonProgressBar.h"
#include "BCGPRibbonHyperlink.h"
#include "BCGPRibbonSlider.h"
#endif
#ifndef BCGP_EXCLUDE_PLANNER
#include "BCGPPlannerViewDay.h"
#include "BCGPPlannerViewMonth.h"
#endif
#include "BCGPGanttChart.h"
#include "BCGPPropList.h"
#include "BCGPURLLinkButton.h"
#define UPDATE_COLOR(clr, dblHue, dblSaturation) \
  { \
    double H, L, S; \
    DrawManager::RGBtoHSL(clr, &H, &S, &L); \
    H = H * 360.0 + dblHue; \
    H = H -((int)(H / 360)) * 360; \
    S = max(0.0, min(S + dblSaturation, 1.0)); \
    clr = DrawManager::HLStoRGB_TWO(H, L, S); \
  } \
#define RT_BCG_STYLE_XML ("STYLE_XML")
IMPLEMENT_DYNCREATE(VisualManagerCarbon, VisualManager2007)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CString VisualManagerCarbon::GetStyleResourceID()
{
  CString strResID(("IDX_STYLE"));
#if !defined _AFXDLL || defined _BCGCBPRO_STATIC_
  strResID = ("CARBON_") + strResID;
#endif
  return strResID;
};
BOOL VisualManagerCarbon::SetStyle(LPCTSTR lpszPath)
{
#if defined _AFXDLL && !defined _BCGCBPRO_STATIC_
  CString strVer;
  strVer.Format(("%d%d"), _BCGCBPRO_VERSION_MAJOR, _BCGCBPRO_VERSION_MINOR);
  CString strStyleDLLName = ("BCGPStyleCarbon") + strVer + (".dll");
  CString strStyleDLLPath;
  if (lpszPath != NULL && _tcslen(lpszPath) > 0) {
    strStyleDLLPath = lpszPath;
    if (strStyleDLLPath [strStyleDLLPath.GetLength() - 1] != ('\\')) {
      strStyleDLLPath += ('\\');
    }
    strStyleDLLPath += strStyleDLLName;
  }
  else {
    strStyleDLLPath = strStyleDLLName;
  }
  HINSTANCE hinstRes = LoadLibrary(strStyleDLLPath);
  if (hinstRes <= (HINSTANCE) 32) {
    TRACE(("Cannot load Style DLL: %s\r\n"), strStyleDLLPath);
    ASSERT(FALSE);
    return FALSE;
  }
  SetResourceHandle(hinstRes);
  s->m_bAutoFreeRes = TRUE;
#else
  UNREFERENCED_PARAMETER(lpszPath);
  CString strStyle(GetStyleResourceID());
  HINSTANCE hinstRes = AfxFindResourceHandle(strStyle, RT_BCG_STYLE_XML);
  if (::FindResource(hinstRes, strStyle, RT_BCG_STYLE_XML) == NULL) {
    TRACE(("Cannot load Style: %s\r\n"), strStyle);
    ASSERT(FALSE);
    return FALSE;
  }
  SetResourceHandle(hinstRes);
#endif
  return TRUE;
}
/*
int VisualManagerCarbon::SetResourceHandle(HINSTANCE hinstRes)
{
s->m_bAutoFreeRes = FALSE;
if(s->m_hinstRes != hinstRes)
{
s->m_hinstRes = hinstRes;
if(VisualManager::GetInstance()->IsKindOf(
RUNTIME_CLASS(VisualManagerCarbon)))
{
VisualManager::GetInstance()->OnUpdateSystemColors();
}
}
}
*/
VisualManagerCarbon::VisualManagerCarbon()
{
  s->m_bUpdateColors = FALSE;
  s->m_dblHue = 0.0;
  s->m_dblSaturation = 0.0;
  s->m_arLinkImages.Add(XLinkImages(&s->m_ctrlBaseMainBorderCaption.GetImages(), &s->m_ctrlMainBorderCaption.GetImages()));
  s->m_arLinkImages.Add(XLinkImages(&s->m_ctrlBaseMainBorderL.GetImages(), &s->m_ctrlMainBorderL.GetImages()));
  s->m_arLinkImages.Add(XLinkImages(&s->m_ctrlBaseMainBorderR.GetImages(), &s->m_ctrlMainBorderR.GetImages()));
  s->m_arLinkImages.Add(XLinkImages(&s->m_ctrlBaseMainBorderTB.GetImages(), &s->m_ctrlMainBorderTB.GetImages()));
  s->m_arLinkImages.Add(XLinkImages(&s->m_ctrlBaseMainBorder.GetImages(), &s->m_ctrlMainBorder.GetImages()));
  s->m_arLinkImages.Add(XLinkImages(&s->m_ctrlBaseMDIChildBorder.GetImages(), &s->m_ctrlMDIChildBorder.GetImages()));
  s->m_arLinkImages.Add(XLinkImages(&s->m_ctrlBaseMiniBorderCaption.GetImages(), &s->m_ctrlMiniBorderCaption.GetImages()));
  s->m_arLinkImages.Add(XLinkImages(&s->m_ctrlBaseMiniBorderTB.GetImages(), &s->m_ctrlMiniBorderTB.GetImages()));
  s->m_arLinkImages.Add(XLinkImages(&s->m_ctrlBaseMiniBorder.GetImages(), &s->m_ctrlMiniBorder.GetImages()));
  s->m_arLinkImages.Add(XLinkImages(&s->m_ctrlBaseDialogBorder.GetImages(), &s->m_ctrlDialogBorder.GetImages()));
  for (int i = 0; i < 2; i++) {
    s->m_arLinkImages.Add(XLinkImages(&s->m_ctrlBaseSysBtnBack[i].GetImages(), &s->m_SysBtnBack[i].GetImages()));
    s->m_arLinkImages.Add(XLinkImages(&s->m_ctrlBaseSysBtnBackC[i].GetImages(), &s->m_SysBtnBackC[i].GetImages()));
  }
  s->m_arLinkImages.Add(XLinkImages(&s->m_ctrlBaseStatusBarBack.GetImages(), &s->m_ctrlStatusBarBack.GetImages()));
  s->m_arLinkImages.Add(XLinkImages(&s->m_ctrlBaseStatusBarBack_Ext.GetImages(), &s->m_ctrlStatusBarBack_Ext.GetImages()));
  s->m_arLinkImages.Add(XLinkImages(&s->m_ctrlBaseMenuBarBtn.GetImages(), &s->m_ctrlMenuBarBtn.GetImages()));
  s->m_arLinkImages.Add(XLinkImages(&s->m_ctrlBasePopupBorder.GetImages(), &s->m_ctrlPopupBorder.GetImages()));
  s->m_arLinkImages.Add(XLinkImages(&s->m_BaseToolBarGripper, &s->m_ToolBarGripper));
  s->m_arLinkImages.Add(XLinkImages(&s->m_BaseToolBarTear, &s->m_ToolBarTear));
  s->m_arLinkImages.Add(XLinkImages(&s->m_BaseStatusBarPaneBorder, &s->m_StatusBarPaneBorder));
  s->m_arLinkImages.Add(XLinkImages(&s->m_BaseStatusBarSizeBox, &s->m_StatusBarSizeBox));
  s->m_arLinkImages.Add(XLinkImages(&s->m_ctrlBaseToolBarBtn.GetImages(), &s->m_ctrlToolBarBtn.GetImages()));
  s->m_arLinkImages.Add(XLinkImages(&s->m_ctrlBaseMenuItemBack.GetImages(), &s->m_ctrlMenuItemBack.GetImages()));
  s->m_arLinkImages.Add(XLinkImages(&s->m_BaseMenuItemMarkerC, &s->m_MenuItemMarkerC));
  s->m_arLinkImages.Add(XLinkImages(&s->m_BaseMenuItemMarkerR, &s->m_MenuItemMarkerR));
  s->m_arLinkImages.Add(XLinkImages(&s->m_ctrlBaseRibbonBtnPush.GetImages(), &s->m_ctrlRibbonBtnPush.GetImages()));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseBarFace, &s->clrBarFace));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseActiveCaption, &s->clrActiveCaption));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseInactiveCaption, &s->clrInactiveCaption));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseBarShadow, &s->clrBarShadow));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseBarDkShadow, &s->clrBarDkShadow));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseBarLight, &s->clrBarLight));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseFloatToolBarBorder, s->m_clrFloatToolBarBorder));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseHighlightGradientDark, s->m_clrHighlightGradientDark));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseHighlightGradientLight, s->m_clrHighlightGradientLight));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseHighlightDnGradientDark, s->m_clrHighlightDnGradientDark));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseHighlightDnGradientLight, s->m_clrHighlightDnGradientLight));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseHighlightCheckedGradientDark, s->m_clrHighlightCheckedGradientDark));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseHighlightCheckedGradientLight, s->m_clrHighlightCheckedGradientLight));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBasePressedButtonBorder, s->m_clrPressedButtonBorder));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseDlgBackground, s->m_clrDlgBackground));
  s->m_arLinkColors.Add(XLinkColors(&s->m_BaseToolTipParams.s->m_clrFill, &s->m_ToolTipParams.s->m_clrFill));
  s->m_arLinkColors.Add(XLinkColors(&s->m_BaseToolTipParams.s->m_clrFillGradient, &s->m_ToolTipParams.s->m_clrFillGradient));
  s->m_arLinkColors.Add(XLinkColors(&s->m_BaseToolTipParams.s->m_clrBorder, &s->m_ToolTipParams.s->m_clrBorder));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseMainClientArea, s->m_clrMainClientArea));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseMenuLight, s->m_clrMenuLight));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseMenuBorder, s->m_clrMenuBorder));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseMenuRarelyUsed, s->m_clrMenuRarelyUsed));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseMenuItemBorder, s->m_clrMenuItemBorder));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseMenuGutterLight, s->m_clrMenuGutterLight));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseMenuGutterDark, s->m_clrMenuGutterDark));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseSeparator1, s->m_clrSeparator1));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseSeparator2, s->m_clrSeparator2));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseSeparatorLight, s->m_clrSeparatorLight));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseSeparatorDark, s->m_clrSeparatorDark));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseBarBkgnd, s->m_clrBarBkgnd));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseBarGradientLight, s->m_clrBarGradientLight));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseBarGradientDark, s->m_clrBarGradientDark));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseToolBarGradientLight, s->m_clrToolBarGradientLight));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseToolBarGradientDark, s->m_clrToolBarGradientDark));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseToolbarDisabled, s->m_clrToolbarDisabled));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseToolBarGradientVertLight, s->m_clrToolBarGradientVertLight));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseToolBarGradientVertDark, s->m_clrToolBarGradientVertDark));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseCustomizeButtonGradientLight, s->m_clrCustomizeButtonGradientLight));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseCustomizeButtonGradientDark, s->m_clrCustomizeButtonGradientDark));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseToolBarBottomLine, s->m_clrToolBarBottomLine));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseSeparatorLight, s->m_clrSeparatorLight));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseSeparatorDark, s->m_clrSeparatorDark));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseMenuBarGradientLight, s->m_clrMenuBarGradientLight));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseMenuBarGradientDark, s->m_clrMenuBarGradientDark));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseMenuBarGradientVertLight, s->m_clrMenuBarGradientVertLight));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseMenuBarGradientVertDark, s->m_clrMenuBarGradientVertDark));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseCaptionBarGradientLight, s->m_clrCaptionBarGradientLight));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseCaptionBarGradientDark, s->m_clrCaptionBarGradientDark));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseTab3DFace, s->m_clrTab3DFace));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseTab3DBlack, s->m_clrTab3DBlack));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseTab3DDark, s->m_clrTab3DDark));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseTab3DDarkShadow, s->m_clrTab3DDarkShadow));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseTab3DLight, s->m_clrTab3DLight));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseTab3DHighlight, s->m_clrTab3DHighlight));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseTabFlatFace, s->m_clrTabFlatFace));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseTabFlatBlack, s->m_clrTabFlatBlack));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseTabFlatDark, s->m_clrTabFlatDark));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseTabFlatDarkShadow, s->m_clrTabFlatDarkShadow));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseTabFlatLight, s->m_clrTabFlatLight));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBaseTabFlatHighlight, s->m_clrTabFlatHighlight));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBasePopupGradientLight, s->m_clrPopupGradientLight));
  s->m_arLinkColors.Add(XLinkColors(s->m_clrBasePopupGradientDark, s->m_clrPopupGradientDark));
}
VisualManagerCarbon::~VisualManagerCarbon()
{
}
int VisualManagerCarbon::CleanUp()
{
  if (s->m_bUpdateColors) {
    return 0;
  }
  VisualManager2007::CleanUp();
  int i = 0;
  s->m_ctrlBaseMainBorderCaption.CleanUp();
  s->m_ctrlBaseMainBorderL.CleanUp();
  s->m_ctrlBaseMainBorderR.CleanUp();
  s->m_ctrlBaseMainBorderTB.CleanUp();
  s->m_ctrlBaseMainBorder.CleanUp();
  s->m_ctrlBaseMDIChildBorder.CleanUp();
  s->m_ctrlBaseMiniBorderCaption.CleanUp();
  s->m_ctrlBaseMiniBorderTB.CleanUp();
  s->m_ctrlBaseMiniBorder.CleanUp();
  s->m_ctrlBaseDialogBorder.CleanUp();
  for (i = 0; i < 2; i++) {
    s->m_ctrlBaseSysBtnBack[i].CleanUp();
    s->m_ctrlBaseSysBtnBackC[i].CleanUp();
  }
  s->m_ctrlBaseStatusBarBack.CleanUp();
  s->m_ctrlBaseStatusBarBack_Ext.CleanUp();
  s->m_ctrlBaseMenuBarBtn.CleanUp();
  s->m_ctrlBasePopupBorder.CleanUp();
  s->m_BaseToolBarGripper.Clear();
  s->m_BaseToolBarTear.Clear();
  s->m_BaseStatusBarPaneBorder.Clear();
  s->m_BaseStatusBarSizeBox.Clear();
  s->m_ctrlBaseToolBarBtn.CleanUp();
  s->m_ctrlBaseMenuItemBack.CleanUp();
  s->m_BaseMenuItemMarkerC.Clear();
  s->m_BaseMenuItemMarkerR.Clear();
  s->m_clrBaseBarFace = (COLOR) - 1;
  s->m_clrBaseActiveCaption = (COLOR) - 1;
  s->m_clrBaseInactiveCaption = (COLOR) - 1;
  s->m_clrBaseFloatToolBarBorder = (COLOR) - 1;
  s->m_clrBaseDlgBackground = (COLOR) - 1;
  s->m_clrBaseMainClientArea = (COLOR) - 1;
  s->m_clrBaseBarBkgnd = (COLOR) - 1;
  s->m_clrBaseBarGradientLight = (COLOR) - 1;
  s->m_clrBaseBarGradientDark = (COLOR) - 1;
  s->m_clrBaseToolBarGradientLight = (COLOR) - 1;
  s->m_clrBaseToolBarGradientDark = (COLOR) - 1;
  s->m_clrBaseToolbarDisabled = (COLOR) - 1;
  s->m_clrBaseToolBarGradientVertLight = (COLOR) - 1;
  s->m_clrBaseToolBarGradientVertDark = (COLOR) - 1;
  s->m_clrBaseCustomizeButtonGradientLight = (COLOR) - 1;
  s->m_clrBaseCustomizeButtonGradientDark = (COLOR) - 1;
  s->m_clrBaseToolBarBottomLine = (COLOR) - 1;
  s->m_clrBaseSeparatorLight = (COLOR) - 1;
  s->m_clrBaseSeparatorDark = (COLOR) - 1;
  s->m_clrBaseMenuBarGradientLight = (COLOR) - 1;
  s->m_clrBaseMenuBarGradientDark = (COLOR) - 1;
  s->m_clrBaseMenuBarGradientVertLight = (COLOR) - 1;
  s->m_clrBaseMenuBarGradientVertDark = (COLOR) - 1;
  s->m_clrBaseTab3DFace = CLR_DEFAULT;
  s->m_clrBaseTab3DBlack = CLR_DEFAULT;
  s->m_clrBaseTab3DDark = CLR_DEFAULT;
  s->m_clrBaseTab3DDarkShadow = CLR_DEFAULT;
  s->m_clrBaseTab3DLight = CLR_DEFAULT;
  s->m_clrBaseTab3DHighlight = CLR_DEFAULT;
  s->m_clrBaseTabFlatFace = CLR_DEFAULT;
  s->m_clrBaseTabFlatBlack = CLR_DEFAULT;
  s->m_clrBaseTabFlatDark = CLR_DEFAULT;
  s->m_clrBaseTabFlatDarkShadow = CLR_DEFAULT;
  s->m_clrBaseTabFlatLight = CLR_DEFAULT;
  s->m_clrBaseTabFlatHighlight = CLR_DEFAULT;
  s->m_clrLinkText = CLR_DEFAULT;
  s->m_clrLinkHotText = CLR_DEFAULT;
  s->m_clrActionText = CLR_DEFAULT;
  ToolTipParams dummy;
  s->m_BaseToolTipParams = dummy;
  for (i = 0; i < 2; i++) {
    s->m_SysBtnBackC[i].CleanUp();
    s->m_SysBtnBackH[i].CleanUp();
    s->m_SysBtnBackCH[i].CleanUp();
  }
  s->m_ctrlMiniSysBtn.CleanUp();
  s->m_clrBaseMenuGutterLight = CLR_DEFAULT;
  s->m_clrBaseMenuGutterDark = CLR_DEFAULT;
  s->m_bLoaded = FALSE;
}
int VisualManagerCarbon::UpdateColors(double h, double s)
{
  if (s->m_dblHue != h || s->m_dblSaturation != s) {
    s->m_dblHue = h;
    s->m_dblSaturation = s;
    UpdateLinked();
  }
}
int VisualManagerCarbon::UpdateLinked()
{
  if (s->m_arLinkImages.GetSize() == 0 &&
      s->m_arLinkColors.GetSize() == 0) {
    return 0;
  }
  int i = 0;
  for (i = 0; i < s->m_arLinkImages.GetSize(); i++) {
    CBitmap* bmpSrc = s->m_arLinkImages[i].bmpSrc->GetImageWell();
    CBitmap* bmpDst = s->m_arLinkImages[i].bmpDst->GetImageWell();
    if (bmpSrc == NULL) {
      continue;
    }
    if (bmpDst == NULL) {
      s->m_arLinkImages[i].bmpSrc->CopyTo(*s->m_arLinkImages[i].bmpDst);
      bmpDst = s->m_arLinkImages[i].bmpDst->GetImageWell();
      if (bmpDst == NULL) {
        continue;
      }
    }
    DIBSECTION dibSrc;
    ZeroMemory(&dibSrc, sizeof(DIBSECTION));
    if (::GetObject(bmpSrc, sizeof(DIBSECTION), &dibSrc) != sizeof(DIBSECTION) ||
        dibSrc.dsBm.bmBits == NULL) {
      continue;
    }
    DIBSECTION dibDst;
    ZeroMemory(&dibDst, sizeof(DIBSECTION));
    if (::GetObject(bmpDst, sizeof(DIBSECTION), &dibDst) != sizeof(DIBSECTION) ||
        dibDst.dsBm.bmBits == NULL) {
      continue;
    }
    if (dibSrc.dsBm.bmWidth != dibDst.dsBm.bmWidth ||
        dibSrc.dsBm.bmHeight != dibDst.dsBm.bmHeight ||
        dibSrc.dsBm.bmBitsPixel != dibDst.dsBm.bmBitsPixel) {
      continue;
    }
    const int width = dibSrc.dsBm.bmWidth;
    const int height = dibSrc.dsBm.bmHeight;
    const int pitch = dibSrc.dsBm.bmWidthBytes;
    LPBYTE pBitsSrc = (LPBYTE)dibSrc.dsBm.bmBits;
    LPBYTE pBitsDst = (LPBYTE)dibDst.dsBm.bmBits;
    for (int y = 0; y < height; y++) {
      LPBYTE pRowSrc = pBitsSrc;
      LPBYTE pRowDst = pBitsDst;
      for (int x = 0; x < width; x++) {
        if (pRowSrc[2] != pRowSrc[1] || pRowSrc[1] != pRowSrc[0]) {
          COLOR clr = _RGB(pRowSrc[2], pRowSrc[1], pRowSrc[0]);
          UPDATE_COLOR(clr, s->m_dblHue, s->m_dblSaturation);
          *pRowDst++ = GetBV(clr);
          *pRowDst++ = GetGV(clr);
          *pRowDst++ = GetRV(clr);
          *pRowDst++;
        }
        else {
          memcpy(pRowDst, pRowSrc, 4);
          pRowDst += 4;
        }
        pRowSrc += 4;
      }
      pBitsSrc += pitch;
      pBitsDst += pitch;
    }
  }
  for (i = 0; i < s->m_arLinkColors.GetSize(); i++) {
    *s->m_arLinkColors[i].clrDst = *s->m_arLinkColors[i].clrSrc;
    if (*s->m_arLinkColors[i].clrSrc != (COLOR) - 1) {
      UPDATE_COLOR(*s->m_arLinkColors[i].clrDst, s->m_dblHue, s->m_dblSaturation);
    }
  }
  if (s->clrBarFace != (COLOR) - 1) {
    s->clrBarFace.DeleteObject();
    s->clrBarFace.CreateSolidBrush(s->clrBarFace);
  }
  if (s->clrActiveCaption != (COLOR) - 1) {
    s->clrActiveCaption.DeleteObject();
    s->clrActiveCaption.CreateSolidBrush(s->clrActiveCaption);
  }
  if (s->clrInactiveCaption != (COLOR) - 1) {
    s->clrInactiveCaption.DeleteObject();
    s->clrInactiveCaption.CreateSolidBrush(s->clrInactiveCaption);
  }
  if (s->m_clrFloatToolBarBorder != (COLOR) - 1) {
    s->m_clrFloatToolBarBorder.DeleteObject();
    s->m_clrFloatToolBarBorder.CreateSolidBrush(s->m_clrFloatToolBarBorder);
  }
  if (s->m_clrDlgBackground != (COLOR) - 1) {
    s->m_clrDlgBackground.DeleteObject();
    s->m_clrDlgBackground.CreateSolidBrush(s->m_clrDlgBackground);
  }
  if (s->m_clrMainClientArea != (COLOR) - 1) {
    s->m_clrMainClientArea.DeleteObject();
    s->m_clrMainClientArea.CreateSolidBrush(s->m_clrMainClientArea);
  }
  if (s->m_clrMenuLight != (COLOR) - 1) {
    s->m_clrMenuLight.DeleteObject();
    s->m_clrMenuLight.CreateSolidBrush(s->m_clrMenuLight);
  }
  if (s->m_clrMenuRarelyUsed != (COLOR) - 1) {
    s->m_clrMenuRarelyUsed.DeleteObject();
    s->m_clrMenuRarelyUsed.CreateSolidBrush(s->m_clrMenuRarelyUsed);
  }
  if (s->m_clrSeparator1 != (COLOR) - 1) {
    s->m_penSeparator.DeleteObject();
    s->m_penSeparator.CreatePen(PS_SOLID, 1, s->m_clrSeparator1);
  }
  if (s->m_clrSeparator2 != (COLOR) - 1) {
    s->m_penSeparator2.DeleteObject();
    s->m_penSeparator2.CreatePen(PS_SOLID, 1, s->m_clrSeparator2);
  }
  if (s->m_clrMenuItemBorder != (COLOR) - 1) {
    s->m_penMenuItemBorder.DeleteObject();
    s->m_penMenuItemBorder.CreatePen(PS_SOLID, 1, s->m_clrMenuItemBorder);
  }
  if (s->m_clrBarBkgnd != (COLOR) - 1) {
    s->m_clrBarBkgnd.DeleteObject();
    s->m_clrBarBkgnd.CreateSolidBrush(s->m_clrBarBkgnd);
  }
  if (s->m_clrToolBarBottomLine != (COLOR) - 1) {
    s->m_penBottomLine.DeleteObject();
    s->m_penBottomLine.CreatePen(PS_SOLID, 1, s->m_clrToolBarBottomLine);
  }
  if (s->m_clrSeparatorDark != (COLOR) - 1) {
    s->m_penSeparatorDark.DeleteObject();
    s->m_penSeparatorDark.CreatePen(PS_SOLID, 1, s->m_clrSeparatorDark);
  }
  if (s->m_clrSeparatorLight != (COLOR) - 1) {
    s->m_penSeparatorLight.DeleteObject();
    s->m_penSeparatorLight.CreatePen(PS_SOLID, 1, s->m_clrSeparatorLight);
  }
  s->m_clrEditBorder = s->m_clrPressedButtonBorder;
  s->m_clrEditBorderDisabled = s->clrBtnShadow;
  s->m_clrEditBorderHighlighted = s->m_clrEditBorder;
  s->m_clrEditSelection = s->clrHilite;
  s->m_clrComboBorder = s->m_clrEditBorder;
  s->m_clrComboBorderDisabled = s->m_clrEditBorderDisabled;
  s->m_clrComboBorderHighlighted = s->m_clrEditBorderHighlighted;
  s->m_clrComboBorderPressed = s->m_clrComboBorderHighlighted;
  s->m_clrComboBtnBorder = s->m_clrComboBorder;
  s->m_clrComboBtnBorderDisabled = s->clrBtnHilite;
  s->m_clrComboBtnBorderHighlighted = s->m_clrComboBorderHighlighted;
  s->m_clrComboBtnBorderPressed = s->m_clrComboBorderPressed;
  s->m_clrComboSelection = s->m_clrEditSelection;
  s->m_clrComboBtnStart = s->m_clrHighlightGradientDark;
  s->m_clrComboBtnFinish = s->m_clrHighlightGradientLight;
  s->m_clrComboBtnDisabledStart = s->clrBtnFace;
  s->m_clrComboBtnDisabledFinish = s->m_clrComboBtnDisabledStart;
  s->m_clrComboBtnHighlightedStart = s->m_clrHighlightCheckedGradientDark;
  s->m_clrComboBtnHighlightedFinish = s->m_clrHighlightCheckedGradientLight;
  s->m_clrComboBtnPressedStart = s->m_clrHighlightDnGradientDark;
  s->m_clrComboBtnPressedFinish = s->m_clrHighlightDnGradientLight;
  s->m_clrTaskPaneGroupBorder = s->m_clrToolBarBottomLine;
}
int VisualManagerCarbon::OnUpdateSystemColors()
{
  if (s->m_bUpdateColors || VisualManager::GetInstance() != this) {
    return 0;
  }
  if (s->bIsWindows9x) {
    VisualManager2007::OnUpdateSystemColors();
    return 0;
  }
  if (!s->bIsOSAlphaBlendingSupport ||
      IsHighContastMode(s) ||
      s->m_nBitsPerPixel <= 8) {
    VisualManager2007::OnUpdateSystemColors();
    return 0;
  }
  s->m_bUpdateColors = TRUE;
  VisualManager2007::Style style = VisualManager2007::s->m_Style;
  HINSTANCE hinstRes = VisualManager2007::s->m_hinstRes;
  CString strStylePrefix = VisualManager2007::s->m_strStylePrefix;
  BOOL bAutoFreeRes = VisualManager2007::s->m_bAutoFreeRes;
  s->m_bAutoFreeRes = FALSE;
  if (!VisualManager2007::SetStyle(VisualManager2007::VS2007_ObsidianBlack)) {
    return 0;
  }
  VisualManager2007::OnUpdateSystemColors();
  HINSTANCE hinstResOld = NULL;
  if (!SetStyle()) {
    return 0;
  }
  if (s->m_hinstRes != NULL) {
    hinstResOld = AfxGetResourceHandle();
    AfxSetResourceHandle(s->m_hinstRes);
  }
  TagManager tm;
  TagManager::SetBaseColor((COLOR) - 1, (COLOR) - 1);
  if (!tm.LoadFromResource(GetStyleResourceID(), RT_BCG_STYLE_XML)) {
#if !defined _AFXDLL || defined _BCGCBPRO_STATIC_
    TRACE(("\r\nImportant: to enable Carbon look in static link, you need:\r\n"));
    TRACE(("1. Open \"Resource Includes\" dialog and add resource files:\r\n"));
    TRACE(("<BCGCBPro-Path>\\styles\\BCGPStyleCarbon.rc\r\n"));
    TRACE(("2. Add path to this folder to \"Additional Resource Include Directories\"\r\n"));
    TRACE(("<BCGCBPro-Path>\\styles\r\n\r\n"));
    ASSERT(FALSE);
#endif
    if (hinstResOld != NULL) {
      AfxSetResourceHandle(hinstResOld);
    }
    return 0;
  }
  {
    CString strStyle;
    tm.ExcludeTag(("STYLE"), strStyle);
    tm.SetBuffer(strStyle);
  }
  CString strItem;
  s->m_nType = 20;
  if (!tm.IsEmpty()) {
    int nVersion = 0;
    if (tm.ExcludeTag(("VERSION"), strItem)) {
      TagManager tmItem(strItem);
      tmItem.ReadInt(("NUMBER"), nVersion);
      if (nVersion == 2007) {
        tmItem.ReadInt(("TYPE"), s->m_nType);
        if (s->m_nType < 10) {
          s->m_nType *= 10;
        }
        s->m_bLoaded = TRUE;
      }
      if (s->m_bLoaded) {
        if (tmItem.ExcludeTag(("ID_PREFIX"), strItem)) {
          strItem.TrimLeft();
          strItem.TrimRight();
          s->m_strStylePrefix = strItem;
        }
      }
    }
  }
  if (!s->m_bLoaded) {
    if (hinstResOld != NULL) {
      ::AfxSetResourceHandle(hinstResOld);
    }
    TagManager::SetBaseColor((COLOR) - 1, (COLOR) - 1);
    return 0;
  }
  s->m_szNcBtnSize[0] = CSize(::GetSystemMetrics(SM_CXSIZE),
      ::GetSystemMetrics(SM_CYSIZE));
  s->m_szNcBtnSize[1] = CSize(::GetSystemMetrics(SM_CXSMSIZE),
      ::GetSystemMetrics(SM_CYSMSIZE));
  // globals
  if (tm.ExcludeTag(("GLOBALS"), strItem)) {
    TagManager tmItem(strItem);
    tmItem.ReadColor(("BarText"), s->clrBarText);
    tmItem.ReadColor(("BarFace"), s->m_clrBaseBarFace);
    tmItem.ReadColor(("ActiveCaption"), s->m_clrBaseActiveCaption);
    s->m_clrBaseInactiveCaption = s->m_clrBaseActiveCaption;
    if (tmItem.ReadColor(("CaptionText"), s->clrCaptionText)) {
      s->clrInactiveCaptionText = s->clrCaptionText;
    }
    tmItem.ReadColor(("InactiveCaption"), s->m_clrBaseInactiveCaption);
    tmItem.ReadColor(("InactiveCaptionText"), s->clrInactiveCaptionText);
    tmItem.ReadColor(("BarShadow"), s->m_clrBaseBarShadow);
    tmItem.ReadColor(("BarDkShadow"), s->m_clrBaseBarDkShadow);
    tmItem.ReadColor(("BarLight"), s->m_clrBaseBarLight);
    tmItem.ReadColor(("FloatToolBarBorder"), s->m_clrBaseFloatToolBarBorder);
    tmItem.ReadColor(("HighlightGradientDark"), s->m_clrBaseHighlightGradientDark);
    tmItem.ReadColor(("HighlightGradientLight"), s->m_clrBaseHighlightGradientLight);
    s->m_clrBaseHighlightDnGradientDark = s->m_clrBaseHighlightGradientLight;
    s->m_clrBaseHighlightDnGradientLight = s->m_clrBaseHighlightGradientDark;
    tmItem.ReadColor(("HighlightDnGradientDark"), s->m_clrBaseHighlightDnGradientDark);
    tmItem.ReadColor(("HighlightDnGradientLight"), s->m_clrBaseHighlightDnGradientLight);
    s->m_clrBaseHighlightCheckedGradientDark = s->m_clrBaseHighlightDnGradientLight;
    s->m_clrBaseHighlightCheckedGradientLight = s->m_clrBaseHighlightDnGradientDark;
    tmItem.ReadColor(("HighlightCheckedGradientDark"), s->m_clrBaseHighlightCheckedGradientDark);
    tmItem.ReadColor(("HighlightCheckedGradientLight"), s->m_clrBaseHighlightCheckedGradientLight);
    tmItem.ReadColor(("PressedButtonBorder"), s->m_clrBasePressedButtonBorder);
    COLOR clrHB = s->clrHilite;
    COLOR clrHT = s->clrTextHilite;
    if (tmItem.ReadColor(("Highlight"), clrHB) &&
        tmItem.ReadColor(("HighlightText"), clrHT)) {
      s->clrHilite = clrHB;
      s->clrHilite.DeleteObject();
      s->clrHilite.CreateSolidBrush(clrHB);
      s->clrTextHilite = clrHT;
    }
    tmItem.ReadColor(("LinkText"), s->m_clrLinkText);
    tmItem.ReadColor(("LinkHotText"), s->m_clrLinkHotText);
    tmItem.ReadColor(("ActionText"), s->m_clrActionText);
    tmItem.ReadColor(("MenuShadowColor"), s->m_clrMenuShadowBase);
    // dialog background
    s->m_clrBaseDlgBackground = s->m_clrBaseBarLight;
    tmItem.ReadColor(("DlgBackColor"), s->m_clrBaseDlgBackground);
    // ToolTipParams
    s->m_bToolTipParams = tmItem.ReadToolTipParams(("TOOLTIP"), s->m_BaseToolTipParams);
    s->m_ToolTipParams = s->m_BaseToolTipParams;
  }
  // mainwnd
  if (tm.ExcludeTag(("MAINWND"), strItem)) {
    TagManager tmItem(strItem);
    // caption
    CString strCaption;
    if (tmItem.ExcludeTag(("CAPTION"), strCaption)) {
      TagManager tmCaption(strCaption);
      NONCLIENTMETRICS ncm;
      if (s->GetNonClientMetrics(ncm)) {
        tmCaption.ReadFont(("FONT"), ncm.lfCaptionFont);
        s->m_AppCaptionFont.DeleteObject();
        s->m_AppCaptionFont.CreateFontIndirect(&ncm.lfCaptionFont);
      }
      tmCaption.ReadColor(("ActiveText"), s->m_clrAppCaptionActiveText);
      tmCaption.ReadColor(("InactiveText"), s->m_clrAppCaptionInactiveText);
      tmCaption.ReadColor(("ActiveTitleText"), s->m_clrAppCaptionActiveTitleText);
      tmCaption.ReadColor(("InactiveTitleText"), s->m_clrAppCaptionInactiveTitleText);
      tmCaption.ReadBool(("TextCenter"), s->m_bNcTextCenter);
      tmCaption.ReadControlRenderer(("BORDER"), s->m_ctrlBaseMainBorderCaption, MakeResourceID(("IDB_MAINBORDER_CAPTION")));
      s->m_ctrlBaseMainBorderCaption.CopyTo(s->m_ctrlMainBorderCaption);
      // buttons
      CString strButtons;
      if (tmCaption.ExcludeTag(("BUTTONS"), strButtons)) {
        TagManager tmButtons(strButtons);
        LPCTSTR szTags [4] = {
          ("NORMAL"),
          ("NORMAL_SMALL"),
          ("CLOSE"),
          ("CLOSE_SMALL")
        };
        for (int i = 0; i < 2; i++) {
          CString str;
          CString suffix;
          if (i == 1) {
            suffix = ("_S");
          }
          if (tmButtons.ExcludeTag(szTags[i], str)) {
            TagManager tmBtn(str);
            CSize sizeIcon(0, 0);
            if (tmBtn.ReadSize(("IconSize"), sizeIcon)) {
              s->m_SysBtnRestore[i].Clear();
              s->m_SysBtnRestore[i].SetPreMultiplyAutoCheck(TRUE);
              s->m_SysBtnRestore[i].SetImageSize(sizeIcon);
              s->m_SysBtnRestore[i].LoadStr(MakeResourceID(("IDB_SYS_BTN_RESTORE") + suffix));
              s->m_SysBtnMaximize[i].Clear();
              s->m_SysBtnMaximize[i].SetPreMultiplyAutoCheck(TRUE);
              s->m_SysBtnMaximize[i].SetImageSize(sizeIcon);
              s->m_SysBtnMaximize[i].LoadStr(MakeResourceID(("IDB_SYS_BTN_MAXIMIZE") + suffix));
              s->m_SysBtnMinimize[i].Clear();
              s->m_SysBtnMinimize[i].SetPreMultiplyAutoCheck(TRUE);
              s->m_SysBtnMinimize[i].SetImageSize(sizeIcon);
              s->m_SysBtnMinimize[i].LoadStr(MakeResourceID(("IDB_SYS_BTN_MINIMIZE") + suffix));
              s->m_SysBtnHelp[i].Clear();
              s->m_SysBtnHelp[i].SetPreMultiplyAutoCheck(TRUE);
              s->m_SysBtnHelp[i].SetImageSize(sizeIcon);
              s->m_SysBtnHelp[i].LoadStr(MakeResourceID(("IDB_SYS_BTN_HELP") + suffix));
            }
            TagManager::ParseControlRenderer(tmBtn.GetBuffer(),
                s->m_ctrlBaseSysBtnBack[i], MakeResourceID(("IDB_SYS_BTN_BACK")));
            s->m_ctrlBaseSysBtnBack[i].CopyTo(s->m_SysBtnBack[i]);
            TagManager::ParseControlRenderer(tmBtn.GetBuffer(),
                s->m_SysBtnBackH[i], MakeResourceID(("IDB_SYS_BTN_BACK_H")));
          }
          if (tmButtons.ExcludeTag(szTags[i + 2], str)) {
            TagManager tmBtn(str);
            CSize sizeIcon(0, 0);
            if (tmBtn.ReadSize(("IconSize"), sizeIcon)) {
              s->m_SysBtnClose[i].Clear();
              s->m_SysBtnClose[i].SetPreMultiplyAutoCheck(TRUE);
              s->m_SysBtnClose[i].SetImageSize(sizeIcon);
              s->m_SysBtnClose[i].LoadStr(MakeResourceID(("IDB_SYS_BTN_CLOSE") + suffix));
            }
            TagManager::ParseControlRenderer(tmBtn.GetBuffer(),
                s->m_ctrlBaseSysBtnBackC[i], MakeResourceID(("IDB_SYS_BTN_BACK_C") + suffix));
            s->m_ctrlBaseSysBtnBackC[i].CopyTo(s->m_SysBtnBackC[i]);
            TagManager::ParseControlRenderer(tmBtn.GetBuffer(),
                s->m_SysBtnBackCH[i], MakeResourceID(("IDB_SYS_BTN_BACK_C_H") + suffix));
          }
        }
      }
    }
    // border
    tmItem.ReadControlRenderer(("BORDER"), s->m_ctrlBaseMainBorder, MakeResourceID(("IDB_MAINBORDER")));
    s->m_ctrlBaseMainBorder.CopyTo(s->m_ctrlMainBorder);
    tmItem.ReadControlRenderer(("BORDER_L"), s->m_ctrlBaseMainBorderL, MakeResourceID(("IDB_MAINBORDER_L")));
    s->m_ctrlBaseMainBorderL.CopyTo(s->m_ctrlMainBorderL);
    tmItem.ReadControlRenderer(("BORDER_R"), s->m_ctrlBaseMainBorderR, MakeResourceID(("IDB_MAINBORDER_R")));
    s->m_ctrlBaseMainBorderR.CopyTo(s->m_ctrlMainBorderR);
    tmItem.ReadControlRenderer(("BORDER_TB"), s->m_ctrlBaseMainBorderTB, MakeResourceID(("IDB_MAINBORDER_TB")));
    s->m_ctrlBaseMainBorderTB.CopyTo(s->m_ctrlMainBorderTB);
    tmItem.ReadControlRenderer(("BORDER_MDICHILD"), s->m_ctrlBaseMDIChildBorder, MakeResourceID(("IDB_MDICHILDBORDER")));
    s->m_ctrlBaseMDIChildBorder.CopyTo(s->m_ctrlMDIChildBorder);
    if (tmItem.ReadColor(("MainClientArea"), s->m_clrMainClientArea)) {
      s->m_clrMainClientArea.DeleteObject();
      s->m_clrMainClientArea.CreateSolidBrush(s->m_clrMainClientArea);
    }
  }
  // miniwnd
  if (tm.ExcludeTag(("MINIWND"), strItem)) {
    TagManager tmItem(strItem);
    // caption
    CString strCaption;
    if (tmItem.ExcludeTag(("CAPTION"), strCaption)) {
      TagManager tmCaption(strCaption);
      tmCaption.ReadControlRenderer(("BORDER"), s->m_ctrlBaseMiniBorderCaption, MakeResourceID(("IDB_MINIBORDER_CAPTION")));
      s->m_ctrlBaseMiniBorderCaption.CopyTo(s->m_ctrlMiniBorderCaption);
      tmCaption.ReadControlRenderer(("BUTTONS"), s->m_ctrlMiniSysBtn, MakeResourceID(("IDB_MINI_SYS_BTN")));
    }
    // border
    tmItem.ReadControlRenderer(("BORDER"), s->m_ctrlBaseMiniBorder, MakeResourceID(("IDB_MINIBORDER")));
    s->m_ctrlBaseMiniBorder.CopyTo(s->m_ctrlMiniBorder);
    tmItem.ReadControlRenderer(("BORDER_TB"), s->m_ctrlBaseMiniBorderTB, MakeResourceID(("IDB_MINIBORDER_TB")));
    s->m_ctrlBaseMiniBorderTB.CopyTo(s->m_ctrlMiniBorderTB);
  }
  // menu
  if (tm.ExcludeTag(("MENU"), strItem)) {
    TagManager tmItem(strItem);
    tmItem.ReadColor(("Light"), s->m_clrBaseMenuLight);
    tmItem.ReadColor(("GutterLight"), s->m_clrBaseMenuGutterLight);
    tmItem.ReadColor(("GutterDark"), s->m_clrBaseMenuGutterDark);
    s->m_clrMenuRarelyUsed = CLR_DEFAULT;
    tmItem.ReadColor(("Rarely"), s->m_clrBaseMenuRarelyUsed);
    tmItem.ReadColor(("Border"), s->m_clrBaseMenuBorder);
    tmItem.ReadColor(("Separator1"), s->m_clrBaseSeparator1);
    tmItem.ReadColor(("Separator2"), s->m_clrBaseSeparator2);
    COLOR clrGroupBack = (COLOR) - 1;
    if (tmItem.ReadColor(("GroupBackground"), clrGroupBack)) {
      s->m_clrGroupBackground.DeleteObject();
      s->m_clrGroupBackground.CreateSolidBrush(clrGroupBack);
    }
    tmItem.ReadColor(("GroupText"), s->m_clrGroupText);
    tmItem.ReadColor(("ItemBorder"), s->m_clrBaseMenuItemBorder);
    tmItem.ReadControlRenderer(("ItemBack"), s->m_ctrlBaseMenuItemBack, MakeResourceID(("IDB_MENU_ITEs->m_BACK")));
    s->m_ctrlBaseMenuItemBack.CopyTo(s->m_ctrlMenuItemBack);
    tmItem.ReadToolBarImages(("ItemCheck"), s->m_BaseMenuItemMarkerC, MakeResourceID(("IDB_MENU_ITEs->m_MARKER_C")));
    s->m_BaseMenuItemMarkerC.CopyTo(s->m_MenuItemMarkerC);
    tmItem.ReadToolBarImages(("ItemRadio"), s->m_BaseMenuItemMarkerR, MakeResourceID(("IDB_MENU_ITEs->m_MARKER_R")));
    s->m_BaseMenuItemMarkerR.CopyTo(s->m_MenuItemMarkerR);
    tmItem.ReadColor(("TextNormal"), s->m_clrMenuText);
    tmItem.ReadColor(("TextHighlighted"), s->m_clrMenuTextHighlighted);
    tmItem.ReadColor(("TextDisabled"), s->m_clrMenuTextDisabled);
    COLOR clrImages = s->m_clrMenuText;
    CString strColors;
    if (tmItem.ExcludeTag(("COLORS"), strColors)) {
      TagManager tmColors(strColors);
      tmColors.ReadColor(("Black"), clrImages);
      MenuImages::SetColor(MenuImages::ImageBlack, clrImages);
      tmColors.ReadColor(("Black2"), clrImages);
      MenuImages::SetColor(MenuImages::ImageBlack2, clrImages);
      struct XColors {
        MenuImages::IMAGE_STATE state;
        LPCTSTR name;
      };
      XColors colors[4] = {
        {MenuImages::ImageGray, ("Gray")},
        {MenuImages::ImageLtGray, ("LtGray")},
        {MenuImages::ImageWhite, ("White")},
        {MenuImages::ImageDkGray, ("DkGray")}
      };
      for (int ic = 0; ic < 4; ic++) {
        if (tmColors.ReadColor(colors[ic].name, clrImages)) {
          MenuImages::SetColor(colors[ic].state, clrImages);
        }
      }
    }
    else {
      tmItem.ReadColor(("ImagesColor"), clrImages);
      MenuImages::SetColor(MenuImages::ImageBlack, clrImages);
      MenuImages::SetColor(MenuImages::ImageBlack2, clrImages);
    }
    // TODO:
    //MenuImages::SetColor(MenuImages::ImageBlack2, _RGB(21, 66, 139));
  }
  // bars
  if (tm.ExcludeTag(("BARS"), strItem)) {
    TagManager tmItem(strItem);
    CString strBar;
    if (tmItem.ExcludeTag(("DEFAULT"), strBar)) {
      TagManager tmBar(strBar);
      tmBar.ReadColor(("Bkgnd"), s->m_clrBaseBarBkgnd);
      tmBar.ReadColor(("GradientLight"), s->m_clrBaseBarGradientLight);
      s->m_clrBaseBarGradientDark = s->m_clrBaseBarGradientLight;
      tmBar.ReadColor(("GradientDark"), s->m_clrBaseBarGradientDark);
    }
    if (tmItem.ExcludeTag(("TOOLBAR"), strBar)) {
      TagManager tmBar(strBar);
      s->m_clrBaseToolBarGradientLight = s->m_clrBaseBarGradientLight;
      s->m_clrBaseToolBarGradientDark = s->m_clrBaseBarGradientDark;
      s->m_clrBaseToolbarDisabled = DrawManager::SmartMixColors(
          s->m_clrBaseToolBarGradientDark, s->m_clrBaseToolBarGradientLight);
      tmBar.ReadColor(("GradientLight"), s->m_clrBaseToolBarGradientLight);
      tmBar.ReadColor(("GradientDark"), s->m_clrBaseToolBarGradientDark);
      s->m_clrBaseToolBarGradientVertLight = s->m_clrBaseToolBarGradientLight;
      s->m_clrBaseToolBarGradientVertDark = s->m_clrBaseToolBarGradientDark;
      tmBar.ReadColor(("GradientVertLight"), s->m_clrBaseToolBarGradientVertLight);
      tmBar.ReadColor(("GradientVertDark"), s->m_clrBaseToolBarGradientVertDark);
      tmBar.ReadColor(("CustomizeButtonGradientLight"), s->m_clrBaseCustomizeButtonGradientLight);
      tmBar.ReadColor(("CustomizeButtonGradientDark"), s->m_clrBaseCustomizeButtonGradientDark);
      tmBar.ReadToolBarImages(("GRIPPER"), s->m_BaseToolBarGripper, MakeResourceID(("IDB_GRIPPER")));
      s->m_BaseToolBarGripper.CopyTo(s->m_ToolBarGripper);
      tmBar.ReadToolBarImages(("TEAR"), s->m_BaseToolBarTear, MakeResourceID(("IDB_TEAR")));
      s->m_BaseToolBarTear.CopyTo(s->m_ToolBarTear);
      tmBar.ReadControlRenderer(("BUTTON"), s->m_ctrlBaseToolBarBtn, MakeResourceID(("IDB_TOOLBAR_BTN")));
      s->m_ctrlBaseToolBarBtn.CopyTo(s->m_ctrlToolBarBtn);
      s->m_clrToolBarBtnText = s->clrBarText;
      s->m_clrToolBarBtnTextHighlighted = s->m_clrToolBarBtnText;
      tmBar.ReadColor(("TextNormal"), s->m_clrToolBarBtnText);
      tmBar.ReadColor(("TextHighlighted"), s->m_clrToolBarBtnTextHighlighted);
      tmBar.ReadColor(("TextDisabled"), s->m_clrToolBarBtnTextDisabled);
      tmBar.ReadColor(("BottomLineColor"), s->m_clrBaseToolBarBottomLine);
      if (!tmBar.ReadColor(("SeparatorDark"), s->m_clrBaseSeparatorDark)) {
        s->m_clrBaseSeparatorDark =
            PixelAlpha2(s->m_clrBaseToolBarBottomLine, _RGB(255, 255, 255), 95);
      }
      if (!tmBar.ReadColor(("SeparatorLight"), s->m_clrBaseSeparatorLight)) {
        s->m_clrBaseSeparatorLight = _RGB(255, 255, 255);
      }
    }
    if (tmItem.ExcludeTag(("MENUBAR"), strBar)) {
      TagManager tmBar(strBar);
      s->m_clrBaseMenuBarGradientLight = s->m_clrBaseToolBarGradientLight;
      s->m_clrBaseMenuBarGradientDark = s->m_clrBaseToolBarGradientDark;
      tmBar.ReadColor(("GradientLight"), s->m_clrBaseMenuBarGradientLight);
      tmBar.ReadColor(("GradientDark"), s->m_clrBaseMenuBarGradientDark);
      s->m_clrBaseMenuBarGradientVertLight = s->m_clrBaseMenuBarGradientLight;
      s->m_clrBaseMenuBarGradientVertDark = s->m_clrBaseMenuBarGradientDark;
      tmBar.ReadColor(("GradientVertLight"), s->m_clrBaseMenuBarGradientVertLight);
      tmBar.ReadColor(("GradientVertDark"), s->m_clrBaseMenuBarGradientVertDark);
      s->m_clrMenuBarBtnText = s->m_clrToolBarBtnText;
      s->m_clrMenuBarBtnTextHighlighted = s->m_clrToolBarBtnTextHighlighted;
      s->m_clrMenuBarBtnTextDisabled = s->m_clrToolBarBtnTextDisabled;
      tmBar.ReadColor(("TextNormal"), s->m_clrMenuBarBtnText);
      tmBar.ReadColor(("TextHighlighted"), s->m_clrMenuBarBtnTextHighlighted);
      tmBar.ReadColor(("TextDisabled"), s->m_clrMenuBarBtnTextDisabled);
      tmBar.ReadControlRenderer(("BUTTON"), s->m_ctrlBaseMenuBarBtn, MakeResourceID(("IDB_MENUBAR_BTN")));
      s->m_ctrlBaseMenuBarBtn.CopyTo(s->m_ctrlMenuBarBtn);
    }
    if (tmItem.ExcludeTag(("POPUPBAR"), strBar)) {
      TagManager tmBar(strBar);
      tmBar.ReadControlRenderer(("BORDER"), s->m_ctrlBasePopupBorder, MakeResourceID(("IDB_POPUPMENU_BORDER")));
      s->m_ctrlBasePopupBorder.CopyTo(s->m_ctrlPopupBorder);
    }
    if (tmItem.ExcludeTag(("STATUSBAR"), strBar)) {
      TagManager tmBar(strBar);
      tmBar.ReadControlRenderer(("BACK"), s->m_ctrlBaseStatusBarBack, MakeResourceID(("IDB_STATUSBAR_BACK")));
      s->m_ctrlBaseStatusBarBack.CopyTo(s->m_ctrlStatusBarBack);
      tmBar.ReadControlRenderer(("BACK_EXT"), s->m_ctrlBaseStatusBarBack_Ext, MakeResourceID(("IDB_STATUSBAR_BACK_EXT")));
      s->m_ctrlBaseStatusBarBack_Ext.CopyTo(s->m_ctrlStatusBarBack_Ext);
      tmBar.ReadToolBarImages(("PANEBORDER"), s->m_BaseStatusBarPaneBorder, MakeResourceID(("IDB_STATUSBAR_PANEBORDER")));
      s->m_BaseStatusBarPaneBorder.CopyTo(s->m_StatusBarPaneBorder);
      tmBar.ReadToolBarImages(("SIZEBOX"), s->m_BaseStatusBarSizeBox, MakeResourceID(("IDB_STATUSBAR_SIZEBOX")));
      s->m_BaseStatusBarSizeBox.CopyTo(s->m_StatusBarSizeBox);
      s->m_clrStatusBarText = s->m_clrMenuBarBtnText;
      s->m_clrStatusBarTextDisabled = s->m_clrMenuBarBtnTextDisabled;
      s->m_clrExtenedStatusBarTextDisabled = s->m_clrMenuBarBtnTextDisabled;
      tmBar.ReadColor(("TextNormal"), s->m_clrStatusBarText);
      tmBar.ReadColor(("TextDisabled"), s->m_clrStatusBarTextDisabled);
      tmBar.ReadColor(("TextExtendedDisabled"), s->m_clrExtenedStatusBarTextDisabled);
    }
    s->m_clrBaseCaptionBarGradientLight = s->m_clrBaseToolBarGradientLight;
    s->m_clrBaseCaptionBarGradientDark = s->m_clrBaseToolBarGradientDark;
    if (tmItem.ExcludeTag(("CAPTIONBAR"), strBar)) {
      TagManager tmBar(strBar);
      tmBar.ReadColor(("GradientLight"), s->m_clrBaseCaptionBarGradientLight);
      tmBar.ReadColor(("GradientDark"), s->m_clrBaseCaptionBarGradientDark);
      tmBar.ReadColor(("TextNormal"), s->m_clrCaptionBarText);
    }
  }
  if (tm.ExcludeTag(("TABS"), strItem)) {
    TagManager tmItem(strItem);
    tmItem.ReadColor(("TextColorActive"), s->m_clrTabTextActive);
    tmItem.ReadColor(("TextColorInactive"), s->m_clrTabTextInactive);
    CString strTab;
    if (tmItem.ExcludeTag(("3D"), strTab)) {
      TagManager tmTab(strTab);
      tmTab.ReadColor(("Face"), s->m_clrBaseTab3DFace);
      tmTab.ReadColor(("Black"), s->m_clrBaseTab3DBlack);
      tmTab.ReadColor(("Dark"), s->m_clrBaseTab3DDark);
      tmTab.ReadColor(("DarkShadow"), s->m_clrBaseTab3DDarkShadow);
      tmTab.ReadColor(("Light"), s->m_clrBaseTab3DLight);
      tmTab.ReadColor(("Highlight"), s->m_clrBaseTab3DHighlight);
    }
    if (tmItem.ExcludeTag(("FLAT"), strTab)) {
      TagManager tmTab(strTab);
      tmTab.ReadColor(("Face"), s->m_clrBaseTabFlatFace);
      tmTab.ReadColor(("Black"), s->m_clrBaseTabFlatBlack);
      tmTab.ReadColor(("Dark"), s->m_clrBaseTabFlatDark);
      tmTab.ReadColor(("DarkShadow"), s->m_clrBaseTabFlatDarkShadow);
      tmTab.ReadColor(("Light"), s->m_clrBaseTabFlatLight);
      tmTab.ReadColor(("Highlight"), s->m_clrBaseTabFlatHighlight);
    }
  }
  if (tm.ExcludeTag(("RIBBON"), strItem)) {
    TagManager tmItem(strItem);
    CString str;
    if (tmItem.ExcludeTag(("PANEL"), str)) {
      TagManager tmPanel(str);
      CString strButtons;
      if (tmPanel.ExcludeTag(("BUTTONS"), strButtons)) {
        TagManager tmButtons(strButtons);
        tmButtons.ReadControlRenderer(("BUTTON_PUSH"), s->m_ctrlBaseRibbonBtnPush, MakeResourceID(("IDB_RIBBON_BTN_PUSH")));
        s->m_ctrlBaseRibbonBtnPush.CopyTo(s->m_ctrlRibbonBtnPush);
      }
    }
  }
  s->m_clrOutlookCaptionTextNormal = s->m_clrCaptionBarText;
  s->m_clrOutlookPageTextNormal = s->m_clrOutlookCaptionTextNormal;
  s->m_clrOutlookPageTextHighlighted = s->m_clrOutlookPageTextNormal;
  s->m_clrOutlookPageTextPressed = s->m_clrOutlookPageTextNormal;
  s->m_ctrlOutlookWndBar.CleanUp();
  s->m_ctrlOutlookWndPageBtn.CleanUp();
  if (tm.ExcludeTag(("OUTLOOK"), strItem)) {
    TagManager tmItem(strItem);
    CString str;
    if (tmItem.ExcludeTag(("CAPTION"), str)) {
      TagManager tmCaption(str);
      tmCaption.ReadColor(("TextNormal"), s->m_clrOutlookCaptionTextNormal);
    }
    if (tmItem.ExcludeTag(("PAGEBUTTON"), str)) {
      TagManager tmPage(str);
      tmPage.ReadColor(("TextNormal"), s->m_clrOutlookPageTextNormal);
      tmPage.ReadColor(("TextHighlighted"), s->m_clrOutlookPageTextHighlighted);
      tmPage.ReadColor(("TextPressed"), s->m_clrOutlookPageTextPressed);
    }
  }
  // Popup Window:
  s->m_clrBasePopupGradientLight = s->m_clrBaseBarGradientLight;
  s->m_clrBasePopupGradientDark = s->m_clrBaseBarGradientDark;
  if (tm.ExcludeTag(("POPUP"), strItem)) {
    TagManager tmItem(strItem);
    tmItem.ReadColor(("GradientFillLight"), s->m_clrBasePopupGradientLight);
    tmItem.ReadColor(("GradientFillDark"), s->m_clrBasePopupGradientDark);
  }
  if (s->m_clrBaseMenuGutterLight == CLR_DEFAULT) {
    s->m_clrBaseMenuGutterLight = s->m_clrBaseToolBarGradientLight;
  }
  if (s->m_clrBaseMenuGutterDark == CLR_DEFAULT) {
    s->m_clrBaseMenuGutterDark = s->m_clrBaseToolBarGradientLight;
  }
  if (hinstResOld != NULL) {
    AfxSetResourceHandle(hinstResOld);
  }
  CleanStyle();
  VisualManager2007::s->m_hinstRes = hinstRes;
  VisualManager2007::s->m_strStylePrefix = strStylePrefix;
  VisualManager2007::s->m_bAutoFreeRes = bAutoFreeRes;
  VisualManager2007::SetStyle(style);
  s->m_clrTabBack.DeleteObject();
  s->m_clrTabBack.CreateSolidBrush(s->clrBarFace);
  s->m_clrInactiveTabText = s->m_clrTabTextInactive;
  TagManager::SetBaseColor((COLOR) - 1, (COLOR) - 1);
  UpdateLinked();
  s->m_bUpdateColors = FALSE;
}
BOOL VisualManagerCarbon::OnNcActivate(CWnd* pWnd, BOOL bActive)
{
  ASSERT_VALID(pWnd);
  if (pWnd->GetSafeHwnd() == NULL) {
    return FALSE;
  }
  if (s->DwmIsCompositionEnabled()) {
    return FALSE;
  }
  // stay active if WF_STAYACTIVE bit is on
  if (pWnd->s->m_nFlags & WF_STAYACTIVE) {
    bActive = TRUE;
  }
  // but do not stay active if the window is disabled
  if (!pWnd->IsWindowEnabled()) {
    bActive = FALSE;
  }
  s->m_ActivateFlag[pWnd->GetSafeHwnd()] = bActive;
  pWnd->SendMessage(Ws->m_NCPAINT, 0, 0);
  return TRUE;
}
BOOL VisualManagerCarbon::OnSetWindowRegion(CWnd* pWnd, CSize sizeWindow)
{
  ASSERT_VALID(pWnd);
  if (pWnd->GetSafeHwnd() == NULL) {
    return FALSE;
  }
  if (!CanDrawImage()) {
    return FALSE;
  }
  if (s->DwmIsCompositionEnabled()) {
    return FALSE;
  }
  CSize sz(0, 0);
  BOOL bMainWnd = FALSE;
#ifndef BCGP_EXCLUDE_RIBBON
  if (DYNAMIC_DOWNCAST(RibbonBar, pWnd) != NULL) {
    return FALSE;
  }
  else
#endif
  {
    if (pWnd->IsZoomed()) {
      pWnd->SetWindowRgn(NULL, TRUE);
      return TRUE;
    }
    sz = CSize(9, 9);
    bMainWnd = TRUE;
  }
  if (sz != CSize(0, 0)) {
    CRgn rgn;
    BOOL bCreated = FALSE;
    bCreated = rgn.CreateRoundRectRgn(0, 0, sizeWindow.w + 1, sizeWindow.h + 1, sz.w, sz.h);
    if (bCreated) {
      if (pWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)) ||
          pWnd->IsKindOf(RUNTIME_CLASS(Dialog)) ||
          pWnd->IsKindOf(RUNTIME_CLASS(PropertySheet))) {
        CRgn rgnWinodw;
        rgnWinodw.CreateRectRgn(0, sz.h, sizeWindow.w, sizeWindow.h);
        rgn.CombineRgn(&rgn, &rgnWinodw, RGN_OR);
      }
      pWnd->SetWindowRgn((HRGN)rgn.Detach(), TRUE);
      return TRUE;
    }
  }
  return FALSE;
}
int VisualManagerCarbon::DrawNcBtn(img_t* im, const IRECT& rect, UINT nButton,
    BCGBUTTON_STATE state, BOOL bSmall,
    BOOL bActive, BOOL /*bMDI = FALSE*/)
{
  ControlRenderer* pBack = NULL;
  ControlRenderer* pBackH = NULL;
  ToolBarImages* pImage = NULL;
  int nIndex = bSmall ? 1 : 0;
  if (nButton == SC_CLOSE) {
    pBack = &s->m_SysBtnBackC[nIndex];
    pBackH = &s->m_SysBtnBackCH[nIndex];
    pImage = &s->m_SysBtnClose[nIndex];
  }
  else {
    pBack = &s->m_SysBtnBack[nIndex];
    pBackH = &s->m_SysBtnBackH[nIndex];
    if (nButton == SC_MINIMIZE) {
      pImage = &s->m_SysBtnMinimize[nIndex];
    }
    else if (nButton == SC_MAXIMIZE) {
      pImage = &s->m_SysBtnMaximize[nIndex];
    }
    else if (nButton == SC_RESTORE) {
      pImage = &s->m_SysBtnRestore[nIndex];
    }
    else if (nButton == SC_CONTEXTHELP) {
      pImage = &s->m_SysBtnHelp[nIndex];
    }
  }
  ImageAlignHorz horz = s->GetRibbonImageScale() != 1. ? ToolBarImages::ImageAlignHorzStretch : ToolBarImages::ImageAlignHorzCenter;
  ImageAlignVert vert = s->GetRibbonImageScale() != 1. ? ImageAlignVertStretch : ImageAlignVertCenter;
  BYTE alphaSrc = 255;
  pBack->Draw(im, rect, 0);
  if (state != ButtonsIsRegular) {
    pBackH->Draw(im, rect, state == ButtonsIsHighlighted ? 0 : 1);
  }
  else if (!bActive) {
    alphaSrc = 189;
  }
  if (pImage != NULL) {
    pImage->DrawEx(im, rect, 0, horz, vert, IRECT(0, 0, 0, 0), alphaSrc);
  }
}
int VisualManagerCarbon::DrawNcCaption(img_t* im, IRECT rectCaption,
    DWORD dwStyle, DWORD dwStyleEx,
    const CString& strTitle, const CString& strDocument,
    CBitmap* hIcon, BOOL bPrefix, BOOL bActive,
    BOOL bTextCenter,
    const CObList& lstSysButtons)
{
  const BOOL bIsRTL = (dwStyleEx & WS_EX_LAYOUTRTL) == WS_EX_LAYOUTRTL;
  const BOOL bIsSmallCaption = (dwStyleEx & WS_EX_TOOLWINDOW) != 0;
  const int nSysCaptionHeight = bIsSmallCaption ? ::GetSystemMetrics(SM_CYSMCAPTION) : ::GetSystemMetrics(SM_CYCAPTION);
  CSize szSysBorder(GetSystemBorders(FALSE));
  CDC memDC;
  memDC.CreateCompatibleDC(im);
  CBitmap memBmp;
  memBmp.CreateCompatibleBitmap(im, rectCaption.Width(), rectCaption.Height());
  CBitmap* pBmpOld = memDC.SelectObject(&memBmp);
  memDC.BitBlt(0, 0, rectCaption.Width(), rectCaption.Height(), im, 0, 0, SRCCOPY);
  BOOL bMaximized = (dwStyle & WS_MAXIMIZE) == WS_MAXIMIZE;
  int indexBorder = 0;
  IRECT rectBorderCaption(rectCaption);
  if (bMaximized) {
    rectBorderCaption.OffsetRect(-rectBorderCaption.TopLeft());
    rectBorderCaption.b -= szSysBorder.h;
  }
  s->m_ctrlMainBorderCaption.Draw(&memDC, rectBorderCaption, indexBorder);
  IRECT rectCaptionB(rectCaption);
  rectCaptionB.DeflateRect((bMaximized ? 0 : szSysBorder.w) - 2, 0);
  rectCaptionB.t = rectCaptionB.b - s->m_ctrlMainBorderTB.GetParams().s->m_rectImage.Height();
  s->m_ctrlMainBorderTB.Draw(&memDC, rectCaptionB, indexBorder);
  IRECT rect(rectCaption);
  rect.DeflateRect(szSysBorder.w, szSysBorder.h, szSysBorder.w, 0);
  rect.t = rect.b - nSysCaptionHeight - 1;
  // Draw icon:
  if (hIcon != NULL && !bIsSmallCaption) {
    CSize szIcon(::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
    long x = rect.l + (bMaximized ? szSysBorder.w : 0) + 2;
    long y = rect.t + max(0, (nSysCaptionHeight - szIcon.h) / 2);
    ::DrawIconEx(memDC.GetSafeHdc(), x, y, hIcon, szIcon.w, szIcon.h,
        0, NULL, DI_NORMAL);
    rect.l = x + szIcon.w + (bMaximized ? szSysBorder.w : 4);
  }
  else {
    int xOffset = (bMaximized ? szSysBorder.w : 0) + 2;
    rect.l += xOffset;
  }
  // Draw system buttons:
  int xButtonsRight = rect.r;
  for (POSITION pos = lstSysButtons.GetHeadPosition(); pos != NULL;) {
    FrameCaptionButton* pButton = (FrameCaptionButton*)
        lstSysButtons.GetNext(pos);
    BCGBUTTON_STATE state = ButtonsIsRegular;
    if (pButton->s->m_bPushed && pButton->s->m_bFocused) {
      state = ButtonsIsPressed;
    }
    else if (pButton->s->m_bFocused) {
      state = ButtonsIsHighlighted;
    }
    UINT uiHit = pButton->GetHit();
    UINT nButton = 0;
    switch (uiHit) {
    case HTCLOSE_BCG:
      nButton = SC_CLOSE;
      break;
    case HTMAXBUTTON_BCG:
      nButton =
          (dwStyle & WS_MAXIMIZE) == WS_MAXIMIZE ? SC_RESTORE : SC_MAXIMIZE;
      break;
    case HTMINBUTTON_BCG:
      nButton =
          (dwStyle & WS_MINIMIZE) == WS_MINIMIZE ? SC_RESTORE : SC_MINIMIZE;
      break;
    case HTHELPBUTTON_BCG:
      nButton = SC_CONTEXTHELP;
      break;
    }
    IRECT rectBtn(pButton->GetRect());
    if (bMaximized) {
      rectBtn.OffsetRect(szSysBorder.w, szSysBorder.h);
    }
    DrawNcBtn(&memDC, rectBtn, nButton, state, FALSE, bActive, FALSE);
    xButtonsRight = min(xButtonsRight, pButton->GetRect().l);
  }
  // Draw text:
  if ((!strTitle.IsEmpty() || !strDocument.IsEmpty()) &&
      rect.l < rect.r) {
    CFont* pOldFont = (CFont*)memDC.SelectObject(&s->m_AppCaptionFont);
    IRECT rectText = rect;
    rectText.r = xButtonsRight - 1;
    DrawNcText(&memDC, rectText, strTitle, strDocument, bPrefix, bActive, bIsRTL, bTextCenter);
    memDC.SelectObject(pOldFont);
  }
  im->BitBlt(rectCaption.l, rectCaption.t, rectCaption.Width(), rectCaption.Height(),
      &memDC, 0, 0, SRCCOPY);
  memDC.SelectObject(pBmpOld);
}
BOOL VisualManagerCarbon::OnNcPaint(CWnd* pWnd, const CObList& lstSysButtons, IRECT rectRedraw)
{
  if (!CanDrawImage()) {
    return VisualManager2003_OnNcPaint(pWnd, lstSysButtons, rectRedraw);
  }
  ASSERT_VALID(pWnd);
  if (pWnd->GetSafeHwnd() == NULL) {
    return FALSE;
  }
  CWindowDC dc(pWnd);
  if (dc.GetSafeHdc() != NULL) {
    CRgn rgn;
    if (!rectRedraw.IsRectEmpty()) {
      rgn.CreateRectRgnIndirect(rectRedraw);
      dc.SelectClipRgn(&rgn);
    }
#ifndef BCGP_EXCLUDE_RIBBON
    RibbonBar* pBar = GetRibbonBar(pWnd);
    BOOL bRibbonCaption = pBar != NULL &&
        pBar->IsWindowVisible() &&
        pBar->IsReplaceFrameCaption();
#else
    BOOL bRibbonCaption = FALSE;
#endif
    IRECT rtWindow;
    pWnd->GetWindowRect(rtWindow);
    pWnd->ScreenToClient(rtWindow);
    IRECT rtClient;
    pWnd->GetClientRect(rtClient);
    rtClient.OffsetRect(-rtWindow.TopLeft());
    dc.ExcludeClipRect(rtClient);
    rtWindow.OffsetRect(-rtWindow.TopLeft());
    BOOL bActive = IsWindowActive(pWnd);
    IRECT rectCaption(rtWindow);
    CSize szSysBorder(GetSystemBorders(bRibbonCaption));
    BOOL bDialog = pWnd->IsKindOf(RUNTIME_CLASS(Dialog)) || pWnd->IsKindOf(RUNTIME_CLASS(PropertySheet));
    if (bDialog && (pWnd->GetStyle() & WS_THICKFRAME) == 0) {
      szSysBorder.h = ::GetSystemMetrics(SM_CYFIXEDFRAME);
    }
    rectCaption.b = rectCaption.t + szSysBorder.h;
    const DWORD dwStyle = pWnd->GetStyle();
    BOOL bMaximized = (dwStyle & WS_MAXIMIZE) == WS_MAXIMIZE;
    if (!bRibbonCaption) {
      const DWORD dwStyleEx = pWnd->GetExStyle();
      const BOOL bIsSmallCaption = (dwStyleEx & WS_EX_TOOLWINDOW) != 0;
      const int nSysCaptionHeight = bIsSmallCaption ? ::GetSystemMetrics(SM_CYSMCAPTION) : ::GetSystemMetrics(SM_CYCAPTION);
      rectCaption.b += nSysCaptionHeight;
      BOOL bDestroyIcon = FALSE;
      CBitmap* hIcon = globalUtils.GetWndIcon(pWnd, &bDestroyIcon);
      CString strText;
      pWnd->GetWindowText(strText);
      CString strTitle(strText);
      CString strDocument;
      BOOL bPrefix = FALSE;
      if ((dwStyle & FWS_ADDTOTITLE) == FWS_ADDTOTITLE) {
        bPrefix = (dwStyle & FWS_PREFIXTITLE) == FWS_PREFIXTITLE;
        CFrameWnd* pFrameWnd = DYNAMIC_DOWNCAST(CFrameWnd, pWnd);
        if (pFrameWnd != NULL) {
          strTitle = pFrameWnd->GetTitle();
          if (!strTitle.IsEmpty()) {
            if (strText.GetLength() >= strTitle.GetLength()) {
              if (bPrefix) {
                int pos = strText.Find(strTitle, strText.GetLength() - strTitle.GetLength());
                if (pos != -1) {
                  strTitle = strText.Right(strTitle.GetLength() + 3);
                  strDocument = strText.Left(strText.GetLength() - strTitle.GetLength());
                }
              }
              else {
                int pos = strText.Find(strTitle);
                if (pos != -1) {
                  strTitle = strText.Left(strTitle.GetLength() + 3);
                  strDocument = strText.Right(strText.GetLength() - strTitle.GetLength());
                }
              }
            }
          }
          else {
            strDocument = strText;
          }
        }
      }
      if (bMaximized) {
        rectCaption.InflateRect(szSysBorder.w, szSysBorder.h, szSysBorder.w, 0);
      }
      DrawNcCaption(&dc, rectCaption, dwStyle, dwStyleEx,
          strTitle, strDocument, hIcon, bPrefix, bActive, s->m_bNcTextCenter,
          lstSysButtons);
      if (bDestroyIcon) {
        ::DestroyIcon(hIcon);
      }
      if (bMaximized) {
        return TRUE;
      }
    }
#ifndef BCGP_EXCLUDE_RIBBON
    else {
      if (bMaximized) {
        return TRUE;
      }
      rectCaption.b += pBar->GetCaptionHeight();
      s->m_ctrlMainBorderCaption.Draw(&dc, rectCaption, bActive ? 0 : 1);
    }
#endif // BCGP_EXCLUDE_RIBBON
    rtWindow.t = rectCaption.b;
    dc.ExcludeClipRect(rectCaption);
    int indexBorder = 0;
    //-------------------------------
    // Find status bar extended area:
    //-------------------------------
    IRECT rectExt(0, 0, 0, 0);
    BOOL bExtended = FALSE;
    BOOL bIsStatusBar = FALSE;
    CWnd* pStatusBar = pWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR, TRUE);
    if (pStatusBar->GetSafeHwnd() != NULL && pStatusBar->IsWindowVisible()) {
      StatusBar* pClassicStatusBar = DYNAMIC_DOWNCAST(
          StatusBar, pStatusBar);
      if (pClassicStatusBar != NULL) {
        bExtended = pClassicStatusBar->GetExtendedArea(rectExt);
        bIsStatusBar = TRUE;
      }
    }
    IRECT rectStatus(0, 0, 0, szSysBorder.h);
    if (bIsStatusBar) {
      pStatusBar->GetClientRect(rectStatus);
      int nHeight = rectStatus.Height() + szSysBorder.h;
      rectStatus.b = rtWindow.b;
      rectStatus.t = rectStatus.b - nHeight;
      rectStatus.l = rtWindow.l;
      rectStatus.r = rtWindow.r;
      if (bExtended) {
        rectExt.l = rectStatus.r - rectExt.Width() - szSysBorder.w;
        rectExt.t = rectStatus.t;
        rectExt.b = rectStatus.b;
        rectExt.r = rtWindow.r;
      }
      s->m_ctrlStatusBarBack.Draw(&dc, rectStatus, indexBorder);
      if (bExtended) {
        rectExt.l -= s->m_ctrlStatusBarBack_Ext.GetParams().s->m_rectCorners.l;
        s->m_ctrlStatusBarBack_Ext.Draw(&dc, rectExt, indexBorder);
      }
    }
    IRECT rectPart(rtWindow);
    rectPart.b -= rectStatus.Height();
    rectPart.r = rectPart.l + szSysBorder.w;
    s->m_ctrlMainBorderL.Draw(&dc, rectPart, indexBorder);
    rectPart.r = rtWindow.r;
    rectPart.l = rectPart.r - szSysBorder.w;
    s->m_ctrlMainBorderR.Draw(&dc, rectPart, indexBorder);
    BOOL bBottomLine = TRUE;
    if (!bIsStatusBar) {
      IRECT rectBottom(rtWindow);
      rectBottom.t = rectBottom.b - szSysBorder.h;
      if (pWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)) ||
          (bDialog && !s->m_ctrlDialogBorder.IsValid())) {
        if (bDialog) {
          IRECT rtDialog(rtWindow);
          rtDialog.DeflateRect(1, 0, 1, 1);
          dc.FillRect(rtDialog, &GetDlgBackBrush(pWnd));
          dc.ExcludeClipRect(rtDialog);
          bBottomLine = FALSE;
        }
        s->m_ctrlMDIChildBorder.DrawFrame(&dc, rectBottom, indexBorder);
      }
      else if (bDialog) {
        s->m_ctrlDialogBorder.DrawFrame(&dc, rectBottom, indexBorder);
      }
      else {
        s->m_ctrlMainBorder.DrawFrame(&dc, rectBottom, indexBorder);
      }
    }
    if (bBottomLine) {
      IRECT rectBottom(rtWindow);
      rectBottom.t = rectBottom.b - rectStatus.Height();
      rectBottom.DeflateRect((bMaximized ? 0 : szSysBorder.w) - 2, 0);
      rectBottom.b = rectBottom.t + s->m_ctrlMainBorderTB.GetParams().s->m_rectImage.Height();
      s->m_ctrlMainBorderTB.Draw(&dc, rectBottom, indexBorder + s->m_ctrlMainBorderTB.GetImageCount() / 2);
    }
    dc.SelectClipRgn(NULL);
    return TRUE;
  }
  return VisualManager2003_OnNcPaint(pWnd, lstSysButtons, rectRedraw);
}
BOOL VisualManagerCarbon::OnUpdateNcButtons(CWnd* pWnd, const CObList& lstSysButtons, IRECT rectCaption)
{
  if (!CanDrawImage()) {
    return VisualManager2007::OnUpdateNcButtons(pWnd, lstSysButtons, rectCaption);
  }
  ASSERT_VALID(pWnd);
  if (pWnd->GetSafeHwnd() == NULL) {
    return FALSE;
  }
#ifndef BCGP_EXCLUDE_RIBBON
  RibbonBar* pBar = GetRibbonBar(pWnd);
  BOOL bRibbonCaption = pBar != NULL &&
      pBar->IsWindowVisible() &&
      pBar->IsReplaceFrameCaption();
#else
  BOOL bRibbonCaption = FALSE;
#endif
  CSize szSysBorder(GetSystemBorders(bRibbonCaption));
  BOOL bDialog = pWnd->IsKindOf(RUNTIME_CLASS(Dialog)) || pWnd->IsKindOf(RUNTIME_CLASS(PropertySheet));
  if (bDialog && (pWnd->GetStyle() & WS_THICKFRAME) == 0) {
    szSysBorder.h = ::GetSystemMetrics(SM_CYFIXEDFRAME);
  }
  int x = rectCaption.r + 2;
  rectCaption.t -= szSysBorder.h;
  int index = 0; // normal, small
  for (POSITION pos = lstSysButtons.GetHeadPosition(); pos != NULL;) {
    CSize sizeButton(0, 0);
    FrameCaptionButton* pButton = (FrameCaptionButton*)
        lstSysButtons.GetNext(pos);
    if (pButton->GetHit() == HTCLOSE_BCG) {
      sizeButton = s->m_SysBtnBackC[index].GetParams().s->m_rectImage.Size();
    }
    else {
      sizeButton = s->m_SysBtnBack[index].GetParams().s->m_rectImage.Size();
    }
    x -= sizeButton.w;
    pButton->SetRect(IRECT(CPoint(x, rectCaption.t), sizeButton));
  }
  return TRUE;
}
int VisualManagerCarbon::OnFillBarBackground(img_t* im, BaseControlBar* pBar,
    IRECT rectClient, IRECT rectClip,
    BOOL bNCArea/* = FALSE*/)
{
  ASSERT_VALID(pBar);
  if (pBar->IsOnGlass()) {
    im->FillSolidRect(rectClient, _RGB(0, 0, 0));
    return 0;
  }
  CRuntimeClass* pBarClass = pBar->GetRuntimeClass();
  if (!CanDrawImage() ||
      pBar->IsDialogControl() ||
      pBarClass->IsDerivedFrom(RUNTIME_CLASS(ColorBar)) ||
      pBarClass->IsDerivedFrom(RUNTIME_CLASS(Calculator)) ||
      pBarClass->IsDerivedFrom(RUNTIME_CLASS(CalendarBar))) {
    VisualManager2007::OnFillBarBackground(im, pBar, rectClient, rectClip, bNCArea);
    return 0;
  }
  if (pBar->IsKindOf(RUNTIME_CLASS(PopupMenuBar))) {
    imdraw_FillRect(im, 0, rectClip, s->m_clrMenuLight);
    PopupMenuBar* pMenuBar = DYNAMIC_DOWNCAST(PopupMenuBar, pBar);
    if (!pMenuBar->s->m_bDisableSideBarInXPMode) {
      IRECT rectImages = rectClient;
      rectImages.r = rectImages.l + pMenuBar->GetGutterWidth();
      DrawManager dm(*im);
      dm.FillGradient(rectImages, s->m_clrMenuGutterLight, s->m_clrMenuGutterDark, FALSE,
          35);
      rectImages.l = rectImages.r;
      rectImages.r += 2;
      DrawSeparator(im, rectImages, FALSE);
    }
    return 0;
  }
  else if (pBar->IsKindOf(RUNTIME_CLASS(StatusBar))) {
    CSize szSysBorder(GetSystemBorders(FALSE));
    IRECT rect(rectClient);
    IRECT rectExt(0, 0, 0, 0);
    BOOL bExtended = ((StatusBar*)pBar)->GetExtendedArea(rectExt);
    if (bExtended) {
      rect.r = rectExt.l;
    }
    CWnd* pWnd = ((StatusBar*)pBar)->GetParent();
    ASSERT_VALID(pWnd);
    rect.InflateRect(szSysBorder.w, 0, szSysBorder.w, szSysBorder.h);
    s->m_ctrlStatusBarBack.Draw(im, rect, 0);
    if (bExtended) {
      rectExt.InflateRect(0, 0, szSysBorder.w, szSysBorder.h);
      rectExt.l -= s->m_ctrlStatusBarBack_Ext.GetParams().s->m_rectCorners.l;
      s->m_ctrlStatusBarBack_Ext.Draw(im, rectExt, 0);
    }
    rect = rectClient;
    rect.InflateRect(2, 0);
    rect.b = rect.t + s->m_ctrlMainBorderTB.GetParams().s->m_rectImage.Height();
    s->m_ctrlMainBorderTB.Draw(im, rect, 1);
    return 0;
  }
  VisualManager2007::OnFillBarBackground(im, pBar, rectClient, rectClip, bNCArea);
}
COLOR VisualManagerCarbon::OnFillMiniFrameCaption(img_t* im,
    IRECT rectCaption,
    MiniFrameWnd* pFrameWnd, BOOL bActive)
{
  if (!CanDrawImage()) {
    return VisualManager2003_OnFillMiniFrameCaption(im,
        rectCaption, pFrameWnd, bActive);
  }
  IRECT rectCaptionB(rectCaption);
  IRECT rectBorderSize(0, 0, 0, 0);
  if (pFrameWnd != NULL) {
    pFrameWnd->CalcBorderSize(rectBorderSize);
  }
  else {
    rectBorderSize.l = ::GetSystemMetrics(SM_CYSIZEFRAME);
    rectBorderSize.r = rectBorderSize.l;
    rectBorderSize.t = ::GetSystemMetrics(SM_CXSIZEFRAME);
    rectBorderSize.b = rectBorderSize.t;
  }
  rectCaption.InflateRect(rectBorderSize.l, rectBorderSize.t, rectBorderSize.r, 0);
  s->m_ctrlMiniBorderCaption.Draw(im, rectCaption);
  rectCaptionB.InflateRect(2, 0);
  rectCaptionB.t = rectCaptionB.b - s->m_ctrlMiniBorderTB.GetParams().s->m_rectImage.Height();
  s->m_ctrlMiniBorderTB.Draw(im, rectCaptionB, 0);
  //rectCaption
  // get the text color
  return bActive
      ? s->m_clrAppCaptionActiveTitleText
      : s->m_clrAppCaptionInactiveTitleText;
}
int VisualManagerCarbon::OnDrawMiniFrameBorder(
    img_t* im, MiniFrameWnd* pFrameWnd,
    IRECT rectBorder, IRECT rectBorderSize)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawMiniFrameBorder(im,
        pFrameWnd, rectBorder, rectBorderSize);
    return 0;
  }
  rectBorder.t += rectBorderSize.t + ::GetSystemMetrics(SM_CYSMCAPTION);
  IRECT rtPart(rectBorder);
  rtPart.r = rtPart.l + rectBorderSize.l;
  rtPart.b -= rectBorderSize.b;
  s->m_ctrlMainBorderL.Draw(im, rtPart);
  rtPart = rectBorder;
  rtPart.l = rtPart.r - rectBorderSize.r;
  rtPart.b -= rectBorderSize.b;
  s->m_ctrlMainBorderR.Draw(im, rtPart);
  rectBorder.t = rtPart.b;
  s->m_ctrlMiniBorder.Draw(im, rectBorder);
  rectBorder.DeflateRect(::GetSystemMetrics(SM_CXSIZEFRAME), 0);
  rectBorder.InflateRect(2, 0);
  rectBorder.b = rectBorder.t + s->m_ctrlMiniBorderTB.GetParams().s->m_rectImage.Height();
  s->m_ctrlMiniBorderTB.Draw(im, rectBorder, 1);
}
int VisualManagerCarbon::OnDrawFloatingToolbarBorder(
    img_t* im, BaseToolBar* pToolBar,
    IRECT rectBorder, IRECT rectBorderSize)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawFloatingToolbarBorder(im,
        pToolBar, rectBorder, rectBorderSize);
    return 0;
  }
  OnDrawMiniFrameBorder(im, NULL, rectBorder, rectBorderSize);
}
int VisualManagerCarbon::OnDrawMenuBorder(img_t* im, PopupMenu* pMenu, IRECT rect)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawMenuBorder(im, pMenu, rect);
    return 0;
  }
#ifndef BCGP_EXCLUDE_RIBBON
  if (pMenu != NULL) {
    RibbonPanelMenuBar* pRibbonMenuBar =
        DYNAMIC_DOWNCAST(RibbonPanelMenuBar, pMenu->GetMenuBar());
    if (pRibbonMenuBar != NULL) {
      VisualManager2007::OnDrawMenuBorder(im, pMenu, rect);
      return 0;
    }
  }
#endif
  s->m_ctrlPopupBorder.DrawFrame(im, rect);
}
int VisualManagerCarbon::OnDrawCaptionButton(
    img_t* im, CaptionButton* pButton, BOOL bActive,
    BOOL bHorz, BOOL bMaximized, BOOL bDisabled,
    int nImageID/* = -1*/)
{
  if (!CanDrawImage() || bDisabled) {
    VisualManager2007::OnDrawCaptionButton(im, pButton,
        bActive, bHorz, bMaximized, bDisabled, nImageID);
    return 0;
  }
  MenuImages::IMAGES_IDS id = (MenuImages::IMAGES_IDS) - 1;
  if (nImageID != -1) {
    id = (MenuImages::IMAGES_IDS)nImageID;
  }
  else if (pButton != NULL) {
    id = pButton->GetIconID(bHorz, bMaximized);
  }
  IRECT rc = pButton->GetRect();
  rc.DeflateRect(0, 1);
  BOOL bPushed = pButton->s->m_bPushed;
  BOOL bFocused = pButton->s->m_bFocused || pButton->s->m_bDroppedDown;
  int nIndex = 0;
  if (bPushed && bFocused) {
    nIndex = 2;
  }
  else if (bFocused) {
    nIndex = 1;
  }
  if (id == MenuImages::IdClose) {
    nIndex += 3;
  }
  s->m_ctrlMiniSysBtn.Draw(im, rc, nIndex);
  if (id != -1) {
    MenuImages::IMAGE_STATE imageState = MenuImages::ImageWhite;
    if (!bActive && !bPushed && !bFocused) {
      imageState = MenuImages::ImageGray;
    }
    MenuImages::Draw(im, id, rc, imageState);
  }
}
int VisualManagerCarbon::GetTabFrameColors(const BaseTabWnd* pTabWnd,
    COLOR& clrDark,
    COLOR& clrBlack,
    COLOR& clrHighlight,
    COLOR& clrFace,
    COLOR& clrDarkShadow,
    COLOR& clrLight,
    COLOR& pbrFace,
    COLOR& pbrBlack)
{
  VisualManager2007::GetTabFrameColors(pTabWnd,
      clrDark, clrBlack,
      clrHighlight, clrFace,
      clrDarkShadow, clrLight,
      pbrFace, pbrBlack);
  pbrFace = s->m_clrTabBack;
  pbrBlack = s->m_clrTabBack;
  /*
  clrFace = s->m_clrTabFlatFace;
  clrBlack = s->m_clrTabFlatBlack;
  clrDark = s->m_clrTabFlatDark;
  clrDarkShadow = s->m_clrTabFlatDarkShadow;
  clrLight = s->m_clrTabFlatLight;
  clrHighlight = s->m_clrTabFlatHighlight;
  */
}
int VisualManagerCarbon::OnEraseTabsArea(img_t* im, IRECT rect,
    const BaseTabWnd* pTabWnd)
{
  VisualManager2003_OnEraseTabsArea(im, rect, pTabWnd);
}
int VisualManagerCarbon::OnDrawTab(img_t* im, IRECT rectTab,
    int iTab, BOOL bIsActive, const BaseTabWnd* pTabWnd)
{
  VisualManager2003_OnDrawTab(im, rectTab, iTab, bIsActive, pTabWnd);
}
int VisualManagerCarbon::OnFillTab(img_t* im, IRECT rectFill, COLOR pbrFill,
    int iTab, BOOL bIsActive,
    const BaseTabWnd* pTabWnd)
{
  ASSERT_VALID(pTabWnd);
  VisualManager2003_OnFillTab(im, rectFill, pbrFill,
      iTab, bIsActive, pTabWnd);
}
COLOR VisualManagerCarbon::GetTabTextColor(const BaseTabWnd* pTabWnd, int iTab, BOOL bIsActive)
{
  if (!CanDrawImage()) {
    return VisualManager2003_GetTabTextColor(pTabWnd, iTab, bIsActive);
  }
  ASSERT_VALID(pTabWnd);
  if (!bIsActive && (pTabWnd->IsOneNoteStyle() || pTabWnd->IsVS2005Style()) && pTabWnd->GetTabBkColor(iTab) == (COLOR) - 1) {
    return s->m_clrTabTextInactive;
  }
  return VisualManager2003_GetTabTextColor(pTabWnd, iTab, bIsActive);
}
int VisualManagerCarbon::GetTabHorzMargin(const BaseTabWnd* pTabWnd)
{
  return VisualManager2003_GetTabHorzMargin(pTabWnd);
}
BOOL VisualManagerCarbon::OnEraseTabsFrame(img_t* im, IRECT rect, const BaseTabWnd* pTabWnd)
{
  return VisualManager2003_OnEraseTabsFrame(im, rect, pTabWnd);
}
int VisualManagerCarbon::OnEraseTabsButton(img_t* im, IRECT rect,
    Button* pButton,
    BaseTabWnd* pBaseTab)
{
  VisualManager2003_OnEraseTabsButton(im, rect, pButton, pBaseTab);
}
int VisualManagerCarbon::OnDrawTabsButtonBorder(img_t* im, IRECT& rect,
    Button* pButton, UINT uiState,
    BaseTabWnd* pWndTab)
{
  VisualManager2003_OnDrawTabsButtonBorder(im, rect,
      pButton, uiState,
      pWndTab);
}
int VisualManagerCarbon::OnDrawSlider(img_t* im, Slider* pSlider, IRECT rect, BOOL bAutoHideMode)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawSlider(im, pSlider, rect, bAutoHideMode);
    return 0;
  }
  ASSERT_VALID(pSlider);
  imdraw_FillRect(im, 0, rect, &s->clrBarFace);
}
int VisualManagerCarbon::OnHighlightMenuItem(CDC* im, ToolbarMenuButton* pButton,
    IRECT rect, COLOR& clrText)
{
  VisualManager2003_OnHighlightMenuItem(im, pButton, rect, clrText);
}
int VisualManagerCarbon::OnDrawOutlookPageButtonBorder(
    img_t* im, IRECT& rectBtn, BOOL bIsHighlighted, BOOL bIsPressed)
{
  if (!CanDrawImage()) {
    VisualManager2003_OnDrawOutlookPageButtonBorder(im, rectBtn, bIsHighlighted, bIsPressed);
    return 0;
  }
  if (bIsHighlighted || (bIsPressed && bIsHighlighted)) {
    imdraw_Draw3dRect(im, 0, rectBtn, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
  }
  else {
    imdraw_Draw3dRect(im, 0, rectBtn, s->m_clrToolBarGradientLight, s->m_clrToolBarBottomLine);
  }
}
#ifndef BCGP_EXCLUDE_PROP_LIST
COLOR VisualManagerCarbon::GetPropListGroupTextColor(PropList* pPropList)
{
  ASSERT_VALID(pPropList);
  if (!CanDrawImage() || !pPropList->DrawControlBarColors()) {
    return VisualManager2003_GetPropListGroupTextColor(pPropList);
  }
  return s->clrBarText;
}
COLOR VisualManagerCarbon::GetPropListDesciptionTextColor(PropList* pPropList)
{
  ASSERT_VALID(pPropList);
  if (!CanDrawImage() || !pPropList->DrawControlBarColors()) {
    return VisualManager2003_GetPropListDesciptionTextColor(pPropList);
  }
  return s->clrBarText;
}
COLOR VisualManagerCarbon::GetPropListCommandTextColor(PropList* pPropList)
{
  ASSERT_VALID(pPropList);
  if (!CanDrawImage() || !pPropList->DrawControlBarColors()) {
    return VisualManager2003_GetPropListCommandTextColor(pPropList);
  }
  return s->m_clrActionText == CLR_DEFAULT ? s->clrHotText : s->m_clrActionText;
}
#endif // BCGP_EXCLUDE_PROP_LIST
#ifndef BCGP_EXCLUDE_TOOLBOX
COLOR VisualManagerCarbon::GetToolBoxButtonTextColor(ToolBoxButton* pButton)
{
  if (!CanDrawImage()) {
    return VisualManager2003_GetToolBoxButtonTextColor(pButton);
  }
  return pButton->s->m_bIsHighlighted || pButton->s->m_bIsChecked ? s->m_clrToolBarBtnTextHighlighted : s->m_clrToolBarBtnText;
}
#endif // BCGP_EXCLUDE_TOOLBOX
COLOR VisualManagerCarbon::GetURLLinkColor(URLLinkButton* pButton, BOOL bHover)
{
  if (!CanDrawImage() || !pButton->s->m_bVisualManagerStyle || pButton->s->m_bOnGlass) {
    return VisualManager2003_GetURLLinkColor(pButton, bHover);
  }
  return bHover
      ? s->m_clrLinkHotText == CLR_DEFAULT
      ? s->clrHotLinkText
      : s->m_clrLinkHotText
    : s->m_clrLinkText == CLR_DEFAULT
      ? s->clrHotText
      : s->m_clrLinkText;
}
COLOR VisualManagerCarbon::GetToolbarButtonTextColor(ToolbarButton* pButton,
    VisualManager::BCGBUTTON_STATE state)
{
  if (CanDrawImage()) {
    BOOL bDisabled = (ToolBar::IsCustomizeMode() && !pButton->IsEditable()) ||
        (!ToolBar::IsCustomizeMode() && (pButton->s->m_nStyle & TBBS_DISABLED));
    if (pButton->IsKindOf(RUNTIME_CLASS(OutlookButton)) != NULL ||
        pButton->IsKindOf(RUNTIME_CLASS(ToolbarComboBoxButton)) != NULL ||
        pButton->IsKindOf(RUNTIME_CLASS(ToolbarEditBoxButton)) != NULL) {
      return bDisabled ? s->m_clrToolBarBtnTextDisabled : s->m_clrToolBarBtnText;
    }
  }
  return VisualManager2007::GetToolbarButtonTextColor(pButton, state);
}
#ifndef BCGP_EXCLUDE_GRID_CTRL
COLOR VisualManagerCarbon::OnFillGridItem(GridCtrl* pCtrl,
    img_t* im, IRECT rectFill,
    BOOL bSelected, BOOL bActiveItem, BOOL bSortedColumn)
{
  if (!CanDrawImage()) {
    return VisualManager2003_OnFillGridItem(pCtrl,
        im, rectFill, bSelected, bActiveItem, bSortedColumn);
  }
  ASSERT_VALID(pCtrl);
  // Fill area:
  if (bSelected && !bActiveItem) {
    if (!pCtrl->IsFocused()) {
      imdraw_FillRect(im, 0, rectFill, &s->clrBtnFace);
      return s->clrBtnText;
    }
    else {
      imdraw_FillRect(im, 0, rectFill, &s->clrHilite);
      return s->clrTextHilite;
    }
  }
  else {
    if (bActiveItem) {
      imdraw_FillRect(im, 0, rectFill, &s->clrWindow);
    }
    else if (bSortedColumn) {
      CBrush br(pCtrl->GetSortedColor());
      imdraw_FillRect(im, 0, rectFill, &br);
    }
    else {
      // no painting
    }
  }
  return(COLOR) - 1;
}
COLOR VisualManagerCarbon::OnFillGridGroupByBoxBackground(img_t* im, IRECT rect)
{
  COLOR clr = VisualManager2007::OnFillGridGroupByBoxBackground(im, rect);
  if (CanDrawImage()) {
    clr = s->clrWindowText;
  }
  return clr;
}
COLOR VisualManagerCarbon::OnFillReportCtrlRowBackground(GridCtrl* pCtrl,
    img_t* im, IRECT rectFill,
    BOOL bSelected, BOOL bGroup)
{
  if (!CanDrawImage()) {
    return VisualManager2003_OnFillReportCtrlRowBackground(pCtrl,
        im, rectFill, bSelected, bGroup);
  }
  ASSERT_VALID(pCtrl);
  // Fill area:
  COLOR clrText = (COLOR) - 1;
  if (bSelected) {
    if (!pCtrl->IsFocused()) {
      imdraw_FillRect(im, 0, rectFill, &s->clrBtnFace);
      clrText = s->m_clrReportGroupText;
    }
    else {
      imdraw_FillRect(im, 0, rectFill, &s->clrHilite);
      clrText = s->clrTextHilite;
    }
  }
  else {
    if (bGroup) {
      // no painting
      clrText = s->m_clrReportGroupText;
    }
  }
  // Return text color:
  return clrText;
}
#endif // BCGP_EXCLUDE_GRID_CTRL
int VisualManagerCarbon::OnFillSpinButton(img_t* im, SpinButtonCtrl* pSpinCtrl, IRECT rect, BOOL bDisabled)
{
  if (ToolBarImages::s->m_bIsDrawOnGlass || !CanDrawImage()) {
    VisualManager2003_OnFillSpinButton(im, pSpinCtrl, rect, bDisabled);
  }
  else {
    CBrush br(bDisabled ? s->clrBtnFace : s->m_clrComboBtnStart);
    imdraw_FillRect(im, 0, rect, &br);
    imdraw_Draw3dRect(im, 0, rect, s->clrBarHilite, s->clrBarHilite);
  }
}
COLOR VisualManagerCarbon::OnFillCommandsListBackground(img_t* im, IRECT rect, BOOL bIsSelected)
{
  if (!CanDrawImage()) {
    return VisualManager2003_OnFillCommandsListBackground(im, rect, bIsSelected);
  }
  ASSERT_VALID(this);
  rect.l = 0;
  if (bIsSelected) {
    COLOR clr1 = s->m_clrHighlightGradientDark;
    COLOR clr2 = s->m_clrHighlightGradientLight;
    COLOR clrBorder = s->m_clrPressedButtonBorder;
    DrawManager dm(*im);
    dm.FillGradient(rect, clr1, clr2, TRUE);
    imdraw_Draw3dRect(im, 0, rect, clrBorder, clrBorder);
    ToolbarMenuButton dummy;
    return GetHighlightedMenuItemTextColor(&dummy);
  }
  else {
    imdraw_FillRect(im, 0, rect, &s->clrWindow);
    int iImageWidth = ToolBar::GetMenuImageSize().w + GetMenuImageMargin();
    IRECT rectImages = rect;
    rectImages.r = rectImages.l + iImageWidth + MENU_IMAGE_MARGIN;
    DrawManager dm(*im);
    dm.FillGradient(rectImages, s->m_clrToolBarGradientLight, s->m_clrToolBarGradientDark, FALSE);
    return s->clrWindowText;
  }
}
int VisualManagerCarbon::GetMenuDownArrowState(ToolbarMenuButton* pButton, BOOL bHightlight, BOOL bPressed, BOOL bDisabled)
{
  if (!CanDrawImage()) {
    return VisualManager2003_GetMenuDownArrowState(pButton, bHightlight, bPressed, bDisabled);
  }
  return(int)((bHightlight || bPressed) ? MenuImages::ImageBlack : MenuImages::ImageWhite);
}
int VisualManagerCarbon::GetTabButtonState(TabWnd* pTab, CBCGTabButton* pButton)
{
  if (!CanDrawImage() || pButton->IsHighlighted() || pButton->IsPressed()) {
    return VisualManager2003_GetTabButtonState(pTab, pButton);
  }
  return(int) MenuImages::ImageWhite;
}
int VisualManagerCarbon::OnDrawShowAllMenuItems(img_t* im, IRECT rect,
    VisualManager::BCGBUTTON_STATE state)
{
  if (!CanDrawImage()) {
    VisualManager2007::OnDrawShowAllMenuItems(im, rect, state);
    return 0;
  }
  if (s->m_nBitsPerPixel > 8 && !IsHighContastMode(s)) {
    const int nRadius = 8;
    rect = IRECT(rect.CenterPoint() - CSize(nRadius - 1, nRadius - 1), CSize(nRadius * 2, nRadius * 2));
    DrawManager dm(*im);
    dm.DrawGradientRing(rect, s->m_clrHighlightDnGradientLight, s->m_clrHighlightGradientLight,
        (COLOR) - 1,
        45, nRadius);
  }
  VisualManager::OnDrawShowAllMenuItems(im, rect, state);
}
int VisualManagerCarbon::OnFillButtonInterior(img_t* im,
    ToolbarButton* pButton, IRECT rect, VisualManager::BCGBUTTON_STATE state)
{
  if (CanDrawImage()) {
    ControlRenderer* pRenderer = NULL;
    int index = 0;
    BOOL bDisabled = (pButton->s->m_nStyle & TBBS_DISABLED) == TBBS_DISABLED;
    BOOL bPressed = (pButton->s->m_nStyle & TBBS_PRESSED) == TBBS_PRESSED;
    BOOL bChecked = (pButton->s->m_nStyle & TBBS_CHECKED) == TBBS_CHECKED;
    BaseControlBar* pBar = DYNAMIC_DOWNCAST(BaseControlBar, pButton->GetParentWnd());
    ToolbarMenuButton* pMenuButton =
        DYNAMIC_DOWNCAST(ToolbarMenuButton, pButton);
    if (pMenuButton != NULL && pBar != NULL) {
      if (pBar->IsKindOf(RUNTIME_CLASS(PopupMenuBar)) &&
          DYNAMIC_DOWNCAST(ShowAllButton, pButton) != NULL) {
        if (state == ButtonsIsPressed || state == ButtonsIsHighlighted) {
          COLOR clrText;
          VisualManager2003_OnHighlightMenuItem(im, pMenuButton, rect, clrText);
        }
        return 0;
      }
    }
    else if (pBar != NULL && pBar->IsKindOf(RUNTIME_CLASS(ColorBar))) {
      index = -1;
      if (bChecked) {
        if (bDisabled) {
          index = 0;
        }
        else if (state == ButtonsIsPressed || state == ButtonsIsHighlighted) {
          index = 3;
        }
      }
      if (!bDisabled) {
        if (bPressed) {
          index = 2;
        }
        else if (state == ButtonsIsHighlighted) {
          if (index == -1) {
            index = 0;
          }
          index++;
        }
      }
      if (index == -1) {
        return 0;
      }
      pRenderer = &s->m_ctrlToolBarBtn;
    }
    if (pRenderer != NULL) {
      if (pRenderer != NULL) {
        pRenderer->Draw(im, rect, index);
      }
      return 0;
    }
  }
  VisualManager2007::OnFillButtonInterior(im, pButton, rect, state);
}
int VisualManagerCarbon::OnDrawTearOffCaption(img_t* im, IRECT rect, BOOL bIsActive)
{
  if (!CanDrawImage()) {
    VisualManager2007::OnDrawTearOffCaption(im, rect, bIsActive);
    return 0;
  }
  COLOR clrDark = s->m_clrToolBarGradientDark;
  COLOR clrLight = s->m_clrToolBarGradientLight;
  if (bIsActive) {
    clrDark = s->m_clrHighlightGradientDark;
    clrLight = s->m_clrHighlightGradientLight;
  }
  DrawManager dm(*im);
  dm.FillGradient(rect,
      clrDark,
      clrLight,
      TRUE);
  if (bIsActive) {
    imdraw_Draw3dRect(im, 0, rect, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
  }
  s->m_ToolBarTear.DrawEx(im, rect, 0, ToolBarImages::ImageAlignHorzCenter, ImageAlignVertCenter);
}
int VisualManagerCarbon::OnDrawMenuScrollButton(img_t* im, IRECT rect, BOOL bIsScrollDown,
    BOOL bIsHighlited, BOOL bIsPressed,
    BOOL bIsDisabled)
{
  if (!CanDrawImage()) {
    VisualManager2007::OnDrawMenuScrollButton(im, rect, bIsScrollDown, bIsHighlited, bIsPressed, bIsDisabled);
    return 0;
  }
  COLOR clrDark = s->m_clrHighlightGradientDark;
  COLOR clrLight = s->m_clrHighlightGradientLight;
  if (bIsHighlited) {
    clrDark = s->m_clrHighlightCheckedGradientDark;
    clrLight = s->m_clrHighlightCheckedGradientLight;
  }
  DrawManager dm(*im);
  if (!bIsScrollDown) {
    rect.t--;
    dm.FillGradient(rect,
        clrDark,
        clrLight,
        TRUE);
  }
  else {
    dm.FillGradient(rect,
        clrLight,
        clrDark,
        TRUE);
  }
  if (bIsHighlited) {
    imdraw_Draw3dRect(im, 0, rect, s->m_clrMenuItemBorder, s->m_clrMenuItemBorder);
  }
  MenuImages::Draw(im, bIsScrollDown ? MenuImages::IdArowDown : MenuImages::IdArowUp, rect);
}
int VisualManagerCarbon::OnDrawMenuSystemButton(img_t* im, IRECT rect,
    UINT uiSystemCommand,
    UINT nStyle, BOOL bHighlight)
{
  if (!CanDrawImage()) {
    VisualManager2007::OnDrawMenuSystemButton(im, rect,
        uiSystemCommand, nStyle, bHighlight);
    return 0;
  }
  MenuImages::IMAGES_IDS imageID = (MenuImages::IMAGES_IDS) - 1;
  switch (uiSystemCommand) {
  case SC_CLOSE:
    imageID = MenuImages::IdClose;
    break;
  case SC_MINIMIZE:
    imageID = MenuImages::IdMinimize;
    break;
  case SC_RESTORE:
    imageID = MenuImages::IdRestore;
    break;
  }
  if (imageID == -1) {
    return 0;
  }
  BOOL bDisabled = (nStyle & TBBS_DISABLED);
  BOOL bPressed = (nStyle & TBBS_PRESSED);
  if (!bDisabled) {
    int nIndex = 0;
    if (bPressed && bHighlight) {
      nIndex = 2;
    }
    else if (bHighlight) {
      nIndex = 1;
    }
    if (imageID == MenuImages::IdClose) {
      nIndex += 3;
    }
    s->m_ctrlMiniSysBtn.Draw(im, rect, nIndex);
  }
  MenuImages::IMAGE_STATE imageState = MenuImages::ImageWhite;
  if (bDisabled) {
    imageState = MenuImages::ImageDkGray;
  }
  else if (!bPressed && !bHighlight) {
    imageState = MenuImages::ImageGray;
  }
  MenuImages::Draw(im, imageID, rect, imageState);
}
#ifndef BCGP_EXCLUDE_TASK_PANE
int VisualManagerCarbon::OnDrawScrollButtons(img_t* im, const IRECT& rect, const int nBorderSize,
    int iImage, BOOL bHilited)
{
  if (!CanDrawImage()) {
    VisualManager2007::OnDrawScrollButtons(im, rect, nBorderSize, iImage, bHilited);
    return 0;
  }
  IRECT rt(rect);
  rt.t--;
  COLOR clrDark = s->m_clrHighlightGradientDark;
  COLOR clrLight = s->m_clrHighlightGradientLight;
  if (bHilited) {
    clrDark = s->m_clrHighlightCheckedGradientDark;
    clrLight = s->m_clrHighlightCheckedGradientLight;
  }
  DrawManager dm(*im);
  dm.FillGradient(rt,
      clrDark,
      clrLight,
      TRUE);
  if (bHilited) {
    imdraw_Draw3dRect(im, 0, rt, s->m_clrPressedButtonBorder, s->m_clrPressedButtonBorder);
  }
  MenuImages::Draw(im, (MenuImages::IMAGES_IDS) iImage, rect);
}
#endif
#endif

