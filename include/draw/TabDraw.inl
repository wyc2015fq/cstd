#include "TabDraw0.inl"
int DrawTab(const TabCtrl* s, HDDC pDC, int hTab)
{
  RECT rc = s->m_vtrtabs[hTab].rc;
  // draw background.
  DrawTabBack(s, pDC, hTab, &rc);
  // draw image and text;
  DrawTabContext(s, pDC, hTab, &rc);
  return 0;
}
BOOL IsTabVisible(const TabCtrl* s, int hTab, BOOL* partially)
{
  RECT rc = s->m_vtrtabs[hTab].rc;
  ASSERT(IsExist(s, hTab) == TRUE);

  if (rc.right <= s->m_rcTabs.left || rc.left >= s->m_rcTabs.right) {
    if (partially != NULL) {
      *partially = FALSE;
    }

    return FALSE;
  }
  else {
    if (partially != NULL) {
      *partially = (rc.left < s->m_rcTabs.left || rc.right > s->m_rcTabs.right);
    }

    return TRUE;
  }
}
int CalcCtrlAreaHeight(const TabCtrl* s)
{
  RECT rcCtrlAreaPadding;
  RECT rcTabPadding;
  SIZE m_szImage, m_szImageDisabled;
  int iTextHeight = FontGetTextSize(_T("H"), 1).h;
  GetIconSize(s->m_ImageList, &m_szImage);
  GetIconSize(s->m_ImageListDisabled, &m_szImageDisabled);
  rcCtrlAreaPadding = GetCtrlAreaPadding(s);
  rcTabPadding = GetTabPadding(s);
  return rcCtrlAreaPadding.top +
      MAX(rcTabPadding.top + MAX(MAX(m_szImage.h, m_szImageDisabled.h), iTextHeight) + rcTabPadding.bottom, s->m_szSysImage.h) +
      rcCtrlAreaPadding.bottom;
}
int CalcTabWidth(const TabCtrl* s, int hTab)
{
  RECT rcTabPadding;
  int imageWidth = 0;
  int textWidth;
  ASSERT(IsExist(s, hTab) == TRUE);

  if (s->m_vtrtabs[hTab].image != -1) {
    SIZE m_szImage, m_szImageDisabled;
    GetIconSize(s->m_ImageList, &m_szImage);
    GetIconSize(s->m_ImageListDisabled, &m_szImageDisabled);
    imageWidth = (s->m_vtrtabs[hTab].disable == FALSE ? m_szImage.w : m_szImageDisabled.w) + s->m_nTabImageTextGap;
  }

  textWidth = FontGetTextSize(s->m_vtrtabs[hTab].text, strlen(s->m_vtrtabs[hTab].text)).w;
  rcTabPadding = GetTabPadding(s);
  return rcTabPadding.left + imageWidth + textWidth + GetTabExtraWidth(s, hTab) + rcTabPadding.right;
}
int GetTabMinWidth(const TabCtrl* s)
{
  switch (s->m_nStyle) {
  case TabCtrlStyle_VS2008_client_base:
  case TabCtrlStyle_VS2008_client_classic:
  case TabCtrlStyle_VS2008_client_blue:
  case TabCtrlStyle_VS2008_client_silver:
  case TabCtrlStyle_VS2008_client_olive: {
    return TabCtrlStyle_base_GetTabMinWidth(s) + (GetSlantWidth(s) - 6);
  }
  }

  return TabCtrlStyle_base_GetTabMinWidth(s);
}
int TabCtrl_CalcTabsWidth(TabCtrl* s)
{
  int i;
  int iMinTabWidth = GetTabMinWidth(s);

  if (s->m_bEqualTabsSize == FALSE) {
    for (i = 0; i < s->m_vtrtabsLen; ++i) {
      int w = CalcTabWidth(s, (i));
      (s->m_vtrtabs + i)->width = MAX(iMinTabWidth, w);
    }
  }
  else {
    int MAXWidth = 0;

    for (i = 0; i < s->m_vtrtabsLen; ++i) {
      int w = CalcTabWidth(s, (i));
      MAXWidth = MAX(MAXWidth, w);
    }

    MAXWidth = MAX(MAXWidth, iMinTabWidth);

    for (i = 0; i < s->m_vtrtabsLen; ++i) {
      (s->m_vtrtabs + i)->width = MAXWidth;
    }
  }

  return 0;
}
int TabCtrl_GetFullTabsWidth(const TabCtrl* s)
{
  int i, width = 0;
  RECT rcTabHorzMargin;

  for (i = 0; i < s->m_vtrtabsLen; ++i) {
    width += (s->m_vtrtabs + i)->width;
  }

  rcTabHorzMargin = GetTabHorzMargin(s);
  return width + s->m_vtrtabsLen * (rcTabHorzMargin.left + rcTabHorzMargin.right);
}
int TabCtrl_RecalcScale(TabCtrl* s, int iVisibleTabsWidth, int iFullTabsWidth)
{
  int i;
  RECT rcTabHorzMargin = GetTabHorzMargin(s);
  BOOL bPartialView = (iFullTabsWidth > iVisibleTabsWidth);
  int pos = s->m_rcTabs.left;

  if (bPartialView == FALSE) {
    for (i = 0; i < s->m_vtrtabsLen; ++i) {
      (s->m_vtrtabs + i)->rc = s->m_rcTabs;
      (s->m_vtrtabs + i)->rc.left = pos;
      (s->m_vtrtabs + i)->rc.right = pos += (rcTabHorzMargin.left + (s->m_vtrtabs + i)->width + rcTabHorzMargin.right);
      DeflateRect4(&(s->m_vtrtabs + i)->rc, rcTabHorzMargin.left, 0, rcTabHorzMargin.right, 0);
    }
  }
  else {
    int iMinTabWidth = GetTabMinWidth(s);
    int totalTabsIndent = s->m_vtrtabsLen * (rcTabHorzMargin.left + rcTabHorzMargin.right);
    int iEqualWidth = MAX(1, (iVisibleTabsWidth - totalTabsIndent) / (int)s->m_vtrtabsLen);

    if (s->m_bEqualTabsSize == TRUE) {
      for (i = 0; i < s->m_vtrtabsLen; ++i) {
        (s->m_vtrtabs + i)->rc = s->m_rcTabs;
        (s->m_vtrtabs + i)->rc.left = pos;
        (s->m_vtrtabs + i)->rc.right = pos += (rcTabHorzMargin.left + MAX(iMinTabWidth, iEqualWidth) + rcTabHorzMargin.right);
        DeflateRect4(&(s->m_vtrtabs + i)->rc, rcTabHorzMargin.left, 0, rcTabHorzMargin.right, 0);
      }
    }
    else {
      int iTail;
      int width;
      int iTotalCorrectWidth = 0;

      for (i = 0; i < s->m_vtrtabsLen; ++i) {
        if ((s->m_vtrtabs + i)->width > iEqualWidth) {
          iTotalCorrectWidth += (s->m_vtrtabs + i)->width - iEqualWidth;
        }
      }

      iTail = iFullTabsWidth - iVisibleTabsWidth;

      for (i = 0; i < s->m_vtrtabsLen; ++i) {
        if (i != s->m_vtrtabsLen - 1) {
          if ((s->m_vtrtabs + i)->width <= iEqualWidth) {
            width = (s->m_vtrtabs + i)->width;
          }
          else {
            width = MAX(iMinTabWidth, (s->m_vtrtabs + i)->width - (int)((double)iTail * ((double)((s->m_vtrtabs + i)->width - iEqualWidth) / (double)iTotalCorrectWidth) + 0.5));
          }
        }
        else {
          width = MAX(iMinTabWidth, (s->m_rcTabs.right - 1) - pos - (rcTabHorzMargin.left + rcTabHorzMargin.right));
        }

        (s->m_vtrtabs + i)->rc = s->m_rcTabs;
        (s->m_vtrtabs + i)->rc.left = pos;
        (s->m_vtrtabs + i)->rc.right = pos += (rcTabHorzMargin.left + width + rcTabHorzMargin.right);
        DeflateRect4(&(s->m_vtrtabs + i)->rc, rcTabHorzMargin.left, 0, rcTabHorzMargin.right, 0);
      }
    }
  }

  return 0;
}
int TabCtrl_RecalcScroll(TabCtrl* s)
{
  RECT rcTabHorzMargin = GetTabHorzMargin(s);
  int i, pos = s->m_rcTabs.left - s->m_iTabsOffset;

  for (i = 0; i < s->m_vtrtabsLen; ++i) {
    (s->m_vtrtabs + i)->rc = s->m_rcTabs;
    (s->m_vtrtabs + i)->rc.left = pos;
    (s->m_vtrtabs + i)->rc.right = pos += (rcTabHorzMargin.left + (s->m_vtrtabs + i)->width + rcTabHorzMargin.right);
    DeflateRect4(&(s->m_vtrtabs + i)->rc, rcTabHorzMargin.left, 0, rcTabHorzMargin.right, 0);
  }

  return 0;
}
int TabCtrl_Recalc(TabCtrl* s, const RECT* prc)
{
  int count;
  BOOL bHideSingleTab;
  int width = s->m_BorderWidth;
  RECT rc = *prc;
  s->m_rcWindows = rc;

  if (width > 0) {
    DeflateRect(&s->m_rcWindows, width, width);
  }

  count = s->m_vtrtabsLen;
  bHideSingleTab = (count == 1 && s->m_bHideSingleTab == TRUE);

  if (count > 0 && bHideSingleTab == FALSE) {
    RECT rcCtrlAreaPadding;
    int hOldCurTab;
    BOOL bShowScrollButtons;
    int iSysImagePosY;
    int iVisibleTabsWidth;
    int iFullTabsWidth;
    s->m_rcCtrlArea = s->m_rcWindows;

    if (s->m_Layout == TAB_LAYOUT_TOP) {
      s->m_rcWindows.top = s->m_rcCtrlArea.bottom = s->m_rcCtrlArea.top + CalcCtrlAreaHeight(s);
      s->m_rcWindows.bottom = MAX(s->m_rcWindows.bottom, s->m_rcWindows.top);
    }
    else {
      s->m_rcWindows.bottom = s->m_rcCtrlArea.top = s->m_rcCtrlArea.bottom - CalcCtrlAreaHeight(s);
      s->m_rcWindows.top = MIN(s->m_rcWindows.top, s->m_rcWindows.bottom);
    }

    if (s->m_hCurTab >= 0 && IsExist(s, s->m_hCurTab) == FALSE) {
      s->m_hCurTab = -1;
    }

    hOldCurTab = s->m_hCurTab;
#define GetFirstEnableTab() 0
#define GetNextEnableTab(x) 0
#define GetPrevEnableTab(x) 0

    if (s->m_hCurTab < 0) {
      s->m_hCurTab = GetFirstEnableTab();
    }
    else if (s->m_vtrtabs[(s->m_hCurTab)].disable == TRUE) {
      int hCurTab = GetNextEnableTab(s->m_hCurTab);

      if (hCurTab < 0) {
        hCurTab = GetPrevEnableTab(s->m_hCurTab);
      }

      s->m_hCurTab = hCurTab;
    }

    rcCtrlAreaPadding = GetCtrlAreaPadding(s);
    s->m_rcTabs = s->m_rcCtrlArea;
    DeflateRectR(&s->m_rcTabs, &rcCtrlAreaPadding);
    TabCtrl_CalcTabsWidth(s);
    bShowScrollButtons = s->m_Behavior == TAB_BEHAVIOR_SCROLL && s->m_bShowScrollButtons == TRUE;
    iSysImagePosY = (s->m_rcTabs.top + s->m_rcTabs.bottom - s->m_szSysImage.h) / 2;

    if (s->m_bShowCloseButton) {
      RECT rcCloseHorzMargin = iRECT(2, 0, 2, 0);
      s->m_rcCloseButton = s->m_rcTabs;
      s->m_rcCloseButton.right -= rcCloseHorzMargin.right;
      s->m_rcCloseButton.left = s->m_rcCloseButton.right - s->m_szSysImage.w;
      s->m_rcTabs.right = s->m_rcCloseButton.left - rcCloseHorzMargin.left;
      s->m_rcCloseButton.top = iSysImagePosY;
      s->m_rcCloseButton.bottom = s->m_rcCloseButton.top + s->m_szSysImage.h;
    }
    else {
      SetRectEmpty(&s->m_rcCloseButton);
    }

    if (s->m_bShowMenuButton) {
      RECT rcMenuHorzMargin = iRECT(8, 0, 0, 0);
      s->m_rcMenuButton = s->m_rcTabs;
      s->m_rcMenuButton.right -= rcMenuHorzMargin.right;
      s->m_rcMenuButton.left = s->m_rcMenuButton.right - s->m_szSysImage.w;
      s->m_rcTabs.right = s->m_rcMenuButton.left - rcMenuHorzMargin.left;
      s->m_rcMenuButton.top = iSysImagePosY;
      s->m_rcMenuButton.bottom = s->m_rcMenuButton.top + s->m_szSysImage.h;
    }
    else {
      SetRectEmpty(&s->m_rcMenuButton);
    }

    if (bShowScrollButtons == TRUE) {
      RECT rcLeftScrollMargin = iRECT(8, 0, 0, 0);
      RECT rcRightScrollMargin = iRECT(0, 0, 0, 0);
      s->m_rcScrollRight = s->m_rcTabs;
      s->m_rcScrollRight.right -= rcRightScrollMargin.right;
      s->m_rcScrollRight.left = s->m_rcScrollRight.right - s->m_szSysImage.w;
      s->m_rcScrollLeft.right = s->m_rcScrollRight.left - rcRightScrollMargin.left - rcLeftScrollMargin.right;
      s->m_rcScrollLeft.left = s->m_rcScrollLeft.right - s->m_szSysImage.w;
      s->m_rcTabs.right = s->m_rcScrollLeft.left - rcLeftScrollMargin.left;
      s->m_rcScrollLeft.top = s->m_rcScrollRight.top = iSysImagePosY;
      s->m_rcScrollLeft.bottom = s->m_rcScrollRight.bottom = s->m_rcScrollLeft.top + s->m_szSysImage.h;
    }
    else {
      SetRectEmpty(&s->m_rcScrollLeft);
      SetRectEmpty(&s->m_rcScrollRight);
    }

    iVisibleTabsWidth = MAX(0, RCW(&s->m_rcTabs) - 1);
    iFullTabsWidth = TabCtrl_GetFullTabsWidth(s);
    s->m_iMaxTabsOffset = MAX(0, iFullTabsWidth - iVisibleTabsWidth);

    if (s->m_iTabsOffset < 0) {
      s->m_iTabsOffset = 0;
    }

    if (s->m_iTabsOffset > s->m_iMaxTabsOffset) {
      s->m_iTabsOffset = s->m_iMaxTabsOffset;
    }

    s->m_bPartialView = (s->m_iMaxTabsOffset > 0);
    s->m_bScrollLeftAllow = (s->m_bPartialView == TRUE && s->m_iTabsOffset > 0);
    s->m_bScrollRightAllow = (s->m_bPartialView == TRUE && s->m_iTabsOffset < s->m_iMaxTabsOffset);

    if (s->m_Behavior == TAB_BEHAVIOR_SCALE) {
      TabCtrl_RecalcScale(s, iVisibleTabsWidth, iFullTabsWidth);
    }
    else {
      TabCtrl_RecalcScroll(s);
    }
  }
  else {
    if (bHideSingleTab == TRUE) {
      int tab = 0;
      SetRectEmpty(&s->m_vtrtabs[tab].rc);
      s->m_vtrtabs[tab].width = 0;

      if (s->m_hCurTab >= 0 && (s->m_hCurTab) != tab) {
        //if(::IsWindow((s->m_hCurTab)->hWnd)!=0)
        // ::ShowWindow((s->m_hCurTab)->hWnd,SW_HIDE);
      }

      s->m_hCurTab = (s->m_vtrtabs[tab].disable == FALSE ? tab : -1);
    }

    SetRectEmpty(&s->m_rcCtrlArea);
    SetRectEmpty(&s->m_rcTabs);
    SetRectEmpty(&s->m_rcCloseButton);
    SetRectEmpty(&s->m_rcMenuButton);
    SetRectEmpty(&s->m_rcScrollLeft);
    SetRectEmpty(&s->m_rcScrollRight);
  }

  return 0;
}
int TabCtrl_OnPaint(HDDC dc, const TabCtrl* s, const RECT* prc)
{
  int i;
  RECT rcWndsAreaPadding;
  RECT rc = *prc;
  //TabCtrl_Recalc(s, prc);

  if (s->m_vtrtabsLen > 0) {
    DrawCtrlAreaBack(s, dc, &s->m_rcCtrlArea);

    if (IsStraightOrder(s) == TRUE) { // left to right.
      for (i = 0; i < s->m_vtrtabsLen; ++i) {
        if ((i) != s->m_hCurTab && IsTabVisible(s, (i), NULL) == TRUE) {
          DrawTab(s, dc, (i));
        }
      }
    }
    else { // right to left.
      for (i = s->m_vtrtabsLen - 1; i >= 0; --i) {
        if ((i) != s->m_hCurTab && IsTabVisible(s, (i), NULL) == TRUE) {
          DrawTab(s, dc, (i));
        }
      }
    }

    if (s->m_hCurTab >= 0) {
      if (s->m_bWatchActivityCtrl == FALSE || s->m_bActive == TRUE) {
        DrawTab(s, dc, s->m_hCurTab);
      }
      else {
        DrawTab(s, dc, s->m_hCurTab);
      }
    }

    if (IsRectEmpty(&s->m_rcScrollLeft) == 0) {
      BOOL bHover = s->m_hHoverTab == m_hButLeftScroll && (s->m_hPushedTab < 0 || s->m_hPushedTab == m_hButLeftScroll);
      DrawButtonScrollLeft(s, dc, &s->m_rcScrollLeft, bHover, s->m_hPushedTab == m_hButLeftScroll, s->m_bScrollLeftAllow);
      bHover = s->m_hHoverTab == m_hButRightScroll && (s->m_hPushedTab < 0 || s->m_hPushedTab == m_hButRightScroll);
      DrawButtonScrollRight(s, dc, &s->m_rcScrollRight, bHover, s->m_hPushedTab == m_hButRightScroll, s->m_bScrollRightAllow);
    }

    if (IsRectEmpty(&s->m_rcMenuButton) == 0) {
      BOOL bHover = s->m_hHoverTab == m_hButMenu && (s->m_hPushedTab < 0 || s->m_hPushedTab == m_hButMenu);
      DrawButtonMenu(s, dc, &s->m_rcMenuButton, bHover, s->m_hPushedTab == m_hButMenu, s->m_bPartialView);
    }

    if (IsRectEmpty(&s->m_rcCloseButton) == 0) {
      BOOL bHover = s->m_hHoverTab == m_hButClose && (s->m_hPushedTab < 0 || s->m_hPushedTab == m_hButClose);
      DrawButtonClose(s, dc, &s->m_rcCloseButton, bHover, s->m_hPushedTab == m_hButClose);
    }
  }

  rcWndsAreaPadding = GetWndsAreaPadding(s);

  if (s->m_vtrtabsLen == 0 || s->m_hCurTab < 0 || IsRectNull(&rcWndsAreaPadding) == 0) {
    DrawWndsAreaBack(s, dc, &s->m_rcWindows);
  }

  if (s->m_BorderWidth > 0) {
    DrawRectRC(dc, &rc, s->m_BorderColor);
  }

  return 0;
}

int TabCtrl_setStyle_base(TabCtrl* s)
{
  s->m_BorderWidth = 0;
  s->m_CtrlAreaBackColor = _GetSysColor(COLOR_BTNFACE);
  s->m_EmptyWndsAreaBackColor = _GetSysColor(COLOR_APPWORKSPACE);
  s->m_TabBorderColor = _GetSysColor(COLOR_BTNSHADOW);
  s->m_WndsAreaBackColor = _GetSysColor(COLOR_BTNFACE);
  s->m_ChildWndBackColor = _GetSysColor(COLOR_WINDOW);
  s->m_BorderColor = _GetSysColor(COLOR_BTNSHADOW);
  s->m_TabSelectedBackColor = _GetSysColor(COLOR_WINDOW);
  s->m_nTabImageTextGap = 3;
  return 0;
}
int TabCtrl_setStyle_VS2003_base(TabCtrl* s)
{
  TabCtrl_setStyle_base(s);
  s->m_CtrlAreaBackColor = PixelAlpha1(_GetSysColor(COLOR_BTNFACE), _GetSysColor(COLOR_BTNHIGHLIGHT), 45);
  return 0;
}
int TabCtrl_setStyle_VS2010_client(TabCtrl* s)
{
  TabCtrl_setStyle_base(s);
  s->m_CtrlAreaBackColor = _RGB(46, 64, 94);
  return 0;
}
int TabCtrl_setStyle_VS2010_client_custom1(TabCtrl* s)
{
  TabCtrl_setStyle_VS2010_client(s);
  return 0;
}
int TabCtrl_setStyle_VS2010_client_custom2(TabCtrl* s)
{
  TabCtrl_setStyle_VS2010_client(s);
  return 0;
}
int TabCtrl_setStyle_VS2010_bars(TabCtrl* s)
{
  TabCtrl_setStyle_base(s);
  s->m_CtrlAreaBackColor = _RGB(46, 64, 94);
  return 0;
}
int TabCtrl_setStyle_VS2003_client(TabCtrl* s)
{
  TabCtrl_setStyle_VS2003_base(s);
  s->m_BorderColor = _GetSysColor(COLOR_BTNHIGHLIGHT);
  return 0;
}
int TabCtrl_setStyle_VS2003_client_custom1(TabCtrl* s)
{
  TabCtrl_setStyle_VS2003_client(s);
  return 0;
}
int TabCtrl_setStyle_VS2003_bars(TabCtrl* s)
{
  TabCtrl_setStyle_VS2003_base(s);
  s->m_BorderColor = _GetSysColor(COLOR_BTNTEXT);
  return 0;
}
int TabCtrl_setStyle_VS2003_bars_custom1(TabCtrl* s)
{
  TabCtrl_setStyle_VS2003_bars(s);
  s->m_BorderColor = _GetSysColor(COLOR_BTNSHADOW);
  return 0;
}
int TabCtrl_setStyle_VS2008_bars_base(TabCtrl* s)
{
  TabCtrl_setStyle_base(s);
  return 0;
}
int TabCtrl_setStyle_VS2008_bars_custom1_base(TabCtrl* s)
{
  TabCtrl_setStyle_VS2008_bars_base(s);
  return 0;
}
int TabCtrl_setStyle_VS2008_bars_olive_custom1(TabCtrl* s)
{
  TabCtrl_setStyle_VS2008_bars_custom1_base(s);
  SetTabBorderColor2(s, _RGB(143, 158, 116), _RGB(137, 140, 149));
  return 0;
}
int TabCtrl_setStyle_VS2008_bars_olive(TabCtrl* s)
{
  SetTabBorderColor2(s, _RGB(143, 158, 116), _RGB(137, 140, 149));
  return 0;
}
int TabCtrl_setStyle_VS2008_bars_silver_custom1(TabCtrl* s)
{
  SetTabBorderColor2(s, _RGB(119, 119, 146), _RGB(137, 140, 149));
  return 0;
}
int TabCtrl_setStyle_VS2008_bars_silver(TabCtrl* s)
{
  SetTabBorderColor2(s, _RGB(119, 119, 146), _RGB(137, 140, 149));
  return 0;
}
int TabCtrl_setStyle_VS2008_bars_blue_custom1(TabCtrl* s)
{
  SetTabBorderColor2(s, _RGB(60, 127, 177), _RGB(137, 140, 149));
  return 0;
}
int TabCtrl_setStyle_VS2008_bars_blue(TabCtrl* s)
{
  SetTabBorderColor2(s, _RGB(60, 127, 177), _RGB(137, 140, 149));
  return 0;
}
int TabCtrl_setStyle_VS2008_bars_classic_custom1(TabCtrl* s)
{
  SetTabBorderColor2(s, _GetSysColor(COLOR_3DDKSHADOW), _GetSysColor(COLOR_BTNSHADOW));
  return 0;
}
int TabCtrl_setStyle_VS2008_bars_classic(TabCtrl* s)
{
  SetTabBorderColor2(s, _GetSysColor(COLOR_3DDKSHADOW), _GetSysColor(COLOR_BTNSHADOW));
  return 0;
}
int TabCtrl_setStyle_VS2008_client_olive(TabCtrl* s)
{
  return 0;
}
int TabCtrl_setStyle_VS2008_client_silver(TabCtrl* s)
{
  return 0;
}
int TabCtrl_setStyle_VS2008_client_blue(TabCtrl* s)
{
  return 0;
}
int TabCtrl_setStyle_VS2008_client_classic(TabCtrl* s)
{
  return 0;
}
int TabCtrl_setStyle_VS2008_client_base(TabCtrl* s)
{
  return 0;
}
int TabCtrl_setStyle(TabCtrl* s, int nStyle)
{
  s->m_nStyle = nStyle;
  TabCtrl_setStyle_base(s);

  switch (s->m_nStyle) {
#define SETSTYLEDEF(aa) case TabCtrlStyle_##aa: return TabCtrl_setStyle_##aa(s);
#include "TabCtrlStyle.inl"
#undef SETSTYLEDEF
  }

  return 0;
}
int TabCtrl_set(TabCtrl* s)
{
  static int inited = 0;
  int i, n = 5;

  if (0 == inited) {
    inited = 1;
    s->m_vtrtabsLen = n;

    for (i = 0; i < n; ++i) {
      _snprintf(s->m_vtrtabs[i].text, 256, "ÎÄ±¾ %d", i);
      s->m_vtrtabs[i].image = -1;
    }

    s->m_hCurTab = 1;
    s->m_hHoverTab = -1;
    s->m_hPushedTab = -1;
    s->m_bShowCloseButton = 1;
    s->m_bShowMenuButton = 1;
    s->m_nStyle = TabCtrlStyle_VS2010_client;
    s->m_nStyle = TabCtrlStyle_VS2008_client_olive;
    s->m_nStyle = TabCtrlStyle_VS2010_bars;
    TabCtrl_setStyle(s, s->m_nStyle);
  }

  return 0;
}

