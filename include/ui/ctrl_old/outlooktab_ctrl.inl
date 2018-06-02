

enum OUTLOOKTABCTRL_LAYOUT { // from top to bottom.
  OUTLOOKTABCTRL_LAYOUT_1,  // controls|splitter|tabs|buttons - default.
  OUTLOOKTABCTRL_LAYOUT_2,  // controls|splitter|buttons|tabs.
  OUTLOOKTABCTRL_LAYOUT_3,  // buttons|tabs|splitter|controls.
  OUTLOOKTABCTRL_LAYOUT_4   // tabs|buttons|splitter|controls.
};

enum OUTLOOKTABCTRL_BUTTONSALIGN {
  OUTLOOKTABCTRL_BUTTONSALIGN_RIGHT,  // rise from right to left - default.
  OUTLOOKTABCTRL_BUTTONSALIGN_LEFT    // rise from left to right.
};

enum OUTLOOKTABCTRL_CAPTIONALIGN {
  OUTLOOKTABCTRL_CAPTIONALIGN_TOP,    // default.
  OUTLOOKTABCTRL_CAPTIONALIGN_BOTTOM
};

typedef struct OutlookTabCtrlItem {
  int imageBig, imageSmall;
  char text[256];
  DWORD data;

  RECT rc;
  BOOL button;
  BOOL visible;
  BOOL disabled;
} OutlookTabCtrlItem;

typedef struct ItemState {
  BOOL bSelected;
  BOOL bHighlighted;
  BOOL bPushed;
} ItemState;


typedef struct outlooktab_ctrl {
  uictrl base;
  int m_hTopVisibleTab;
  int m_hBottomVisibleTab;
  int m_hCurItem;
  int m_hHighlightedItem;
  int m_hPushedItem; // area (splitter,button menu,tab or button) selected during WM_LBUTTONDOWN, but till WM_LBUTTONUP.

  OutlookTabCtrlItem m_vtritems[256];
  int m_vtritemsLen;

#define HNDLtoPTR(i)  s->m_vtritems[i]

  int m_Layout;
  int m_ButtonsAlign;
  int m_CaptionAlign;
  RECT m_rcCaption, m_rcWindows;
  RECT m_rcSplitter, m_rcButtonMenu;
  RECT m_rcTabs, m_rcButtons;

  BOOL m_bShowCaption;
  BOOL m_bActiveSplitter;
  BOOL m_bShowMenuButton;
  BOOL m_bHideEmptyButtonsBar;

  UINT m_iTimerId;

  POINT m_ptSplitterDrag;
  int m_iSplitterDragStartCountTabs;

  img_t m_ImageListBig[1];
  img_t m_ImageListDisabledBig[1];
  img_t m_ImageListSmall[1];
  img_t m_ImageListDisabledSmall[1];

  int m_BorderWidth;
  int m_CaptionTextHorzMargin;
  int m_TabTextHorzMargin;
  int m_TabHeight;
  int m_Style;


  COLORREF m_BackgroundColor;
  COLORREF m_clrHighlight;
  COLORREF m_clrHighlightDark;
  COLORREF m_brButtonMenu;

  COLORREF m_BorderColor;
  COLORREF m_CaptionColor;
  COLORREF m_CaptionTextColor;
  COLORREF m_SeparationLineColor;
  COLORREF m_IconShadowColor;
  COLORREF m_MenuButtonImageColor;
  COLORREF m_TabTextColor[2];
  COLORREF m_DisabledTabTextColor;
} outlooktab_ctrl;


#define HIT_SPLITTER   (1000) // handle splitter area (like item).
#define HIT_MENUBUTTON (2000) // handle menu button area (like item).


#define BTNMENUICON_COLORMASK   _RGB(255,0,255)

enum CUSTOMSTYLE {
  CUSTOMSTYLE_NONE,
  CUSTOMSTYLE_1,
  CUSTOMSTYLE_2
};

int OutlookTabCtrl_setStyle(outlooktab_ctrl* s, int newStyle)
{
  s->m_Style = newStyle;

  if (s->m_Style == CUSTOMSTYLE_1) {
    s->m_BorderColor =  _RGB(77, 115, 61);
    s->m_CaptionColor = _GetSysColor(COLOR_BTNSHADOW);
    s->m_CaptionTextColor = _RGB(255, 255, 255);
    s->m_SeparationLineColor = _RGB(77, 115, 61);
    s->m_IconShadowColor = _RGB(77, 115, 61);
    s->m_MenuButtonImageColor = _RGB(77, 115, 61);
    s->m_TabTextColor[0] = _GetSysColor(COLOR_WINDOWTEXT);
    s->m_TabTextColor[1] = _GetSysColor(COLOR_HIGHLIGHTTEXT);
    s->m_DisabledTabTextColor = _RGB(128, 128, 128);
    s->m_BackgroundColor = _GetSysColor(COLOR_BTNFACE);
  }
  else if (s->m_Style == CUSTOMSTYLE_2) {
    s->m_BorderColor =  _RGB(29, 112, 224);
    s->m_CaptionColor = _RGB(121, 80, 72);
    s->m_CaptionTextColor = _RGB(208, 183, 179);
    s->m_SeparationLineColor = _RGB(29, 112, 224);
    s->m_IconShadowColor = _RGB(29, 112, 224);
    s->m_MenuButtonImageColor =  _RGB(72, 36, 0);
    s->m_TabTextColor[0] = _GetSysColor(COLOR_WINDOWTEXT);
    s->m_TabTextColor[1] = _GetSysColor(COLOR_HIGHLIGHTTEXT);
    s->m_DisabledTabTextColor = _RGB(15, 58, 117);
    s->m_BackgroundColor = _GetSysColor(COLOR_BTNFACE);
  }
  else {
    s->m_BorderColor  = _GetSysColor(COLOR_BTNSHADOW);
    s->m_CaptionColor = _GetSysColor(COLOR_BTNSHADOW);
    s->m_CaptionTextColor = _GetSysColor(COLOR_CAPTIONTEXT);
    s->m_SeparationLineColor = _GetSysColor(COLOR_BTNSHADOW);
    s->m_IconShadowColor = _GetSysColor(COLOR_BTNSHADOW);
    s->m_MenuButtonImageColor = _GetSysColor(COLOR_WINDOWTEXT);
    s->m_TabTextColor[0] = _GetSysColor(COLOR_WINDOWTEXT);
    s->m_TabTextColor[1] = _GetSysColor(COLOR_HIGHLIGHTTEXT);
    s->m_DisabledTabTextColor = _GetSysColor(COLOR_GRAYTEXT);
    s->m_BackgroundColor = _GetSysColor(COLOR_BTNFACE);
  }

  return 0;
}

int OutlookTabCtrl_set(outlooktab_ctrl* s)
{
  s->m_hCurItem = -1;
  s->m_hHighlightedItem = -1;
  s->m_hPushedItem = -1;
  s->m_hTopVisibleTab = s->m_hBottomVisibleTab = -1;

  s->m_Layout = OUTLOOKTABCTRL_LAYOUT_1;
  s->m_ButtonsAlign = OUTLOOKTABCTRL_BUTTONSALIGN_RIGHT;
  s->m_CaptionAlign = OUTLOOKTABCTRL_CAPTIONALIGN_TOP;
  s->m_bShowCaption = true;
  s->m_bActiveSplitter = true;
  s->m_bShowMenuButton = true;
  s->m_bHideEmptyButtonsBar = false;
  s->m_iTimerId = 0;
  s->m_BorderWidth = 1;
  s->m_CaptionTextHorzMargin = 7;
  s->m_TabTextHorzMargin = 5;

  {
    // From BCGControlBar Library, file BCGPVisualManagerXP.cpp, struct CBCGPVisualManagerXP.
    COLORREF clrWindow = _GetSysColor(COLOR_WINDOW);
    COLORREF clrFace = _GetSysColor(COLOR_BTNFACE);

    COLORREF clrMenuLight = RGBBLEN(clrWindow, clrFace, 36);
    COLORREF clrHL = _GetSysColor(COLOR_HIGHLIGHT);

    COLORREF clrMix = RGBBLEN(clrHL, clrMenuLight, 178);
    s->m_clrHighlight = PixelAlpha2(clrMix, 102);
    s->m_clrHighlightDark = PixelAlpha2(s->m_clrHighlight, 87);
  }

  OutlookTabCtrl_setStyle(s, CUSTOMSTYLE_1);


  return 0;
}
void GetItemState(const outlooktab_ctrl* s, int hItem, ItemState* state/*out*/)
{
  state->bSelected = s->m_hCurItem == hItem;
  state->bHighlighted = s->m_hHighlightedItem == hItem;
  state->bPushed = s->m_hPushedItem == hItem;
}
void AssignHighlightStateOfItem(const outlooktab_ctrl* s, ItemState const* state, BOOL* bSelectLight/*out*/, BOOL* bSelectDark/*out*/)
{
  BOOL bPushed = s->m_hPushedItem != -1;

  if (bSelectDark != NULL) {
    *bSelectDark = state->bHighlighted == TRUE && ((bPushed == FALSE && state->bSelected == TRUE) || (bPushed == TRUE && state->bPushed == TRUE));
  }

  if (bSelectLight != NULL) {
    *bSelectLight = (bPushed == FALSE && ((state->bSelected == TRUE && state->bHighlighted == FALSE) || (state->bSelected == FALSE && state->bHighlighted == TRUE))) ||
        (bPushed == TRUE && state->bSelected == TRUE);
  }
}
int GetCountVisibleButtons(const outlooktab_ctrl* s)
{
  int i;
  int countbuts = 0;

  for (i = 0; i != s->m_vtritemsLen; ++i) {
    if (s->m_vtritems[i].visible == FALSE) {
      continue;
    }

    if (s->m_vtritems[i].button == FALSE) {
      continue;
    }

    countbuts++;
  }

  return countbuts;
}
int GetCountVisibleTabs(const outlooktab_ctrl* s)
{
  int i;
  int counttabs = 0;

  for (i = 0; i != s->m_vtritemsLen; ++i) {
    if (s->m_vtritems[i].visible == FALSE) {
      continue;
    }

    if (s->m_vtritems[i].button == TRUE) {
      break;
    }

    counttabs++;
  }

  return counttabs;
}
// Correct order items and assign top and bottom visible tabs.
int PrepareRecalc(outlooktab_ctrl* s)
{
  int i;
  s->m_hTopVisibleTab = s->m_hBottomVisibleTab = -1;

  for (i = 0; i < s->m_vtritemsLen; ++i) {
    if (s->m_vtritems[i].visible == TRUE) {
      if (s->m_vtritems[i].button == FALSE) { // this is tab.
        if (s->m_hTopVisibleTab == -1) {
          s->m_hTopVisibleTab = i;
        }

        s->m_hBottomVisibleTab = i;
      }
      else { // this is button.
        break;
      }
    }
  }

  return 0;
}
BOOL IsVisibleButtonsBar(const outlooktab_ctrl* s)
{
  return s->m_bShowMenuButton == TRUE || s->m_bHideEmptyButtonsBar == FALSE || GetCountVisibleButtons(s) > 0;
}
int GetTopVisibleAndEnableItem(const outlooktab_ctrl* s)
{
  int i;

  for (i = 0; i != s->m_vtritemsLen; ++i) {
    if (s->m_vtritems[i].visible == TRUE && s->m_vtritems[i].disabled == FALSE) {
      return i;
    }
  }

  return -1;
}
// Recalculate control.
int Recalc(outlooktab_ctrl* s, RECT rcClient)
{
  int i, ret = PrepareRecalc(s);
  int iTextHeight = FontGetTextSize("H", 1).h;
  int counttabs = GetCountVisibleTabs(s);
  int countbuttons = GetCountVisibleButtons(s);
  int iButtonWidth;
  BOOL bVisibleButtonsBar = IsVisibleButtonsBar(s);
  int iAllTabsHeight;
  int iAllButtonsWidth = 0;
  int iCountFoundTabs = 0;
  int iCountFoundButtons = 0;
  int iSplitterHeight = 7 - (s->m_bActiveSplitter == TRUE ? 0 : 2);
  s->m_TabHeight = (3 + MAX(MAX(m_szImageBig.h, m_szImageDisabledBig.h), iTextHeight) + 3);
  iButtonWidth = (3 + MAX(s->m_ImageListSmall->w, s->m_ImageListDisabledSmall->w) + 3);
  iAllTabsHeight = (counttabs * s->m_TabHeight + (counttabs/*pixels for borders*/ - (bVisibleButtonsBar == TRUE ? 0 : 1)));

  if (bVisibleButtonsBar) {
    iAllButtonsWidth = MAX(s->m_TabHeight, 3 + MAX(s->m_ImageListSmall->h, s->m_ImageListDisabledSmall->h) + 3) - 6;
  }

  s->m_rcCaption = rcClient;
  s->m_rcSplitter = s->m_rcTabs = s->m_rcButtons = s->m_rcButtonMenu = s->m_rcWindows = s->m_rcCaption;

  if (s->m_bShowCaption == TRUE) {
    int nCaptionHeight = 3 + iTextHeight + 3;

    if (s->m_CaptionAlign == OUTLOOKTABCTRL_CAPTIONALIGN_TOP) {
      s->m_rcSplitter.top = s->m_rcTabs.top = s->m_rcButtons.top = s->m_rcButtonMenu.top = s->m_rcWindows.top =
          s->m_rcCaption.bottom = s->m_rcCaption.top + nCaptionHeight;
    }
    else { // OUTLOOKTABCTRL_CAPTIONALIGN_BOTTOM.
      s->m_rcSplitter.bottom = s->m_rcTabs.bottom = s->m_rcButtons.bottom = s->m_rcButtonMenu.bottom =
          s->m_rcWindows.bottom = s->m_rcCaption.top = s->m_rcCaption.bottom - nCaptionHeight;
    }
  }

  s->m_rcButtons.right = s->m_rcButtonMenu.left = s->m_rcButtonMenu.right - (s->m_bShowMenuButton == TRUE ? iButtonWidth : 0);

  switch (s->m_Layout) {
  case OUTLOOKTABCTRL_LAYOUT_1:
    s->m_rcTabs.bottom = s->m_rcButtons.top = s->m_rcButtonMenu.top = s->m_rcButtons.bottom - iAllButtonsWidth;
    s->m_rcSplitter.bottom = s->m_rcTabs.top = s->m_rcTabs.bottom - iAllTabsHeight;
    s->m_rcWindows.bottom = s->m_rcSplitter.top = s->m_rcSplitter.bottom - iSplitterHeight;
    break;

  case OUTLOOKTABCTRL_LAYOUT_2:
    s->m_rcButtons.bottom = s->m_rcTabs.top = s->m_rcButtonMenu.bottom = s->m_rcTabs.bottom - iAllTabsHeight;
    s->m_rcSplitter.bottom = s->m_rcButtons.top = s->m_rcButtonMenu.top = s->m_rcButtons.bottom - iAllButtonsWidth;
    s->m_rcWindows.bottom = s->m_rcSplitter.top = s->m_rcSplitter.bottom - iSplitterHeight;
    break;

  case OUTLOOKTABCTRL_LAYOUT_3:
    s->m_rcButtons.bottom = s->m_rcTabs.top = s->m_rcButtonMenu.bottom = s->m_rcButtons.top + iAllButtonsWidth;
    s->m_rcSplitter.top = s->m_rcTabs.bottom = s->m_rcTabs.top + iAllTabsHeight;
    s->m_rcWindows.top = s->m_rcSplitter.bottom = s->m_rcSplitter.top + iSplitterHeight;
    break;

  case OUTLOOKTABCTRL_LAYOUT_4:
    s->m_rcButtons.top = s->m_rcTabs.bottom = s->m_rcButtonMenu.top = s->m_rcTabs.top + iAllTabsHeight;
    s->m_rcSplitter.top = s->m_rcButtons.bottom = s->m_rcButtonMenu.bottom = s->m_rcButtons.top + iAllButtonsWidth;
    s->m_rcWindows.top = s->m_rcSplitter.bottom = s->m_rcSplitter.top + iSplitterHeight;
    break;
  }

  iCountFoundTabs = 0;
  iCountFoundButtons = 0;

  for (i = 0; i != s->m_vtritemsLen; ++i) {
    OutlookTabCtrlItem* item = s->m_vtritems + i;

    if (item->visible == FALSE) {
      continue;
    }

    if (item->button == FALSE) { // this is tab.
      item->rc = s->m_rcTabs;

      if (s->m_Layout == OUTLOOKTABCTRL_LAYOUT_1 || s->m_Layout == OUTLOOKTABCTRL_LAYOUT_4) {
        item->rc.top = s->m_rcTabs.top + (s->m_TabHeight + 1/*separator*/) * iCountFoundTabs;
        item->rc.bottom = MIN(item->rc.top + (s->m_TabHeight + 1/*separator*/), s->m_rcTabs.bottom);
      }
      else // OUTLOOKTABCTRL_LAYOUT_2 or OUTLOOKTABCTRL_LAYOUT_3.
        if (bVisibleButtonsBar == TRUE) {
          item->rc.top = s->m_rcTabs.top + (s->m_TabHeight + 1/*separator*/) * iCountFoundTabs;
          item->rc.bottom = MIN(item->rc.top + (s->m_TabHeight + 1/*separator*/), s->m_rcTabs.bottom);
        }
        else {
          item->rc.top = s->m_rcTabs.top + (i != s->m_hTopVisibleTab ? ((s->m_TabHeight + 1/*separator*/) * iCountFoundTabs - 1) : 0);
          item->rc.bottom = item->rc.top + (s->m_TabHeight + (i != s->m_hTopVisibleTab ? 1/*separator*/ : 0));
        }

      iCountFoundTabs ++;
    }
    else { // this is button.
      item->rc = s->m_rcButtons;

      if (s->m_ButtonsAlign == OUTLOOKTABCTRL_BUTTONSALIGN_RIGHT) {
        int needWidth = iButtonWidth * countbuttons; // width for all buttons.

        if (RCW(&s->m_rcButtons) > needWidth) {
          item->rc.left = s->m_rcButtons.right - needWidth;
        }
      }

      item->rc.left += iCountFoundButtons * iButtonWidth;
      item->rc.right = item->rc.left + iButtonWidth;
      iCountFoundButtons ++;
    }
  }

  if (s->m_hCurItem == -1 || s->m_vtritems[s->m_hCurItem].visible == FALSE || s->m_vtritems[s->m_hCurItem].disabled == TRUE) {
    s->m_hCurItem = GetTopVisibleAndEnableItem(s);
  }

  return 0;
}
BOOL CanPushItem(const outlooktab_ctrl* s)
{
  return GetCountVisibleTabs(s) > 0;
}
void PushItem(outlooktab_ctrl* s)
{
  int ri;
  ASSERT(s->m_hPushedItem == -1);

  for (ri = s->m_vtritemsLen - 1; ri >= 0; --ri) {
    if (s->m_vtritems[ri].visible == FALSE) {
      s->m_vtritems[ri].button = TRUE;
      continue;
    }

    if (s->m_vtritems[ri].button == FALSE) {
      s->m_vtritems[ri].button = TRUE;
      break;
    }
  }
}
void PopItem(outlooktab_ctrl* s)
{
  int i;
  ASSERT(s->m_hPushedItem == -1);

  for (i = 0; i != s->m_vtritemsLen; ++i) {
    if (s->m_vtritems[i].visible == FALSE) {
      s->m_vtritems[i].button = FALSE;
      continue;
    }

    if (s->m_vtritems[i].button == TRUE) {
      s->m_vtritems[i].button = FALSE;
      break;
    }
  }
}
int GetCountVisibleItems(const outlooktab_ctrl* s)
{
  return GetCountVisibleTabs(s) + GetCountVisibleButtons(s);
}
void SetCountItemsInTabState(outlooktab_ctrl* s, int count)
{
  int i;
  ASSERT(count >= 0 && count <= (int)GetCountVisibleItems(s));

  for (i = 0; i != s->m_vtritemsLen; ++i) {
    if (s->m_vtritems[i].visible == FALSE) {
      continue;
    }

    s->m_vtritems[i].button = (count > 0 ? FALSE : TRUE);
    count--;
  }
}
#define IsDisabled(hItem) (ASSERT(hItem>=0), s->m_vtritems[hItem].disabled)
#define IsTabState(hItem) (ASSERT(hItem>=0), s->m_vtritems[hItem].button==FALSE)
#define IsButtonState(hItem) (ASSERT(hItem>=0), s->m_vtritems[hItem].button)
int HitTest(const outlooktab_ctrl* s, POINT point)
{
  int i;

  for (i = 0; i != s->m_vtritemsLen; ++i) {
    if (s->m_vtritems[i].visible == FALSE) {
      continue;
    }

    if (PtInRect(&s->m_vtritems[i].rc, point) != 0) {
      return i;
    }
  }

  return -1;
}
int HitTestEx(const outlooktab_ctrl* s, POINT point)
{
  int item = HitTest(s, point);

  if (item != -1) {
    if (IsDisabled(item) == TRUE) {
      return -1;
    }

    return item;
  }

  if (PtInRect(&s->m_rcSplitter, point) != 0) {
    return HIT_SPLITTER;
  }

  if (PtInRect(&s->m_rcButtonMenu, point) != 0) {
    return HIT_MENUBUTTON;
  }

  return -1;
}
void DrawBackground(const outlooktab_ctrl* s, HDDC hDC, const ItemState* state, RECT const* pRect)
{
  BOOL bSelectLight = FALSE, bSelectDark = FALSE;
  AssignHighlightStateOfItem(s, state, &bSelectLight, &bSelectDark);

  if (s->m_Style != CUSTOMSTYLE_NONE) {
    if (s->m_Style == CUSTOMSTYLE_1) {
      if (bSelectLight == TRUE) {
        DrawGradient(hDC, pRect, 0, _RGB(248, 230, 182), _RGB(241, 189, 111));
      }
      else if (bSelectDark == TRUE) {
        DrawGradient(hDC, pRect, 0, _RGB(247, 227, 171), _RGB(225, 142, 24));
      }
      else {
        DrawGradient(hDC, pRect, 0, _RGB(255, 255, 255), _RGB(213, 209, 201));
      }
    }
    else { // CUSTOMSTYLE_2.
      if (bSelectLight == TRUE) {
        DrawGradient(hDC, pRect, 0, _RGB(207, 225, 251), _RGB(116, 163, 228));
      }
      else if (bSelectDark == TRUE) {
        DrawGradient(hDC, pRect, 0, _RGB(167, 199, 248), _RGB(97, 151, 224));
      }
      else {
        DrawGradient(hDC, pRect, 0, _RGB(255, 255, 255), _RGB(213, 209, 201));
      }
    }
  }
  else {
    if (bSelectLight == TRUE) {
      draw_fillrectR(hDC, pRect, s->m_clrHighlight);
    }
    else if (bSelectDark == TRUE) {
      draw_fillrectR(hDC, pRect, s->m_clrHighlightDark);
    }
    else {
      draw_fillrectR(hDC, pRect, s->m_BackgroundColor);
    }
  }
}
void OutlookTabCtrl_DrawTab(const outlooktab_ctrl* s, HDDC hDC, int hItem, BOOL bDrawSeparator)
{
  int _x0, _y0;
  COLORREF _clr;
  ItemState state;
  BOOL bDisabled = IsDisabled(hItem);
  const OutlookTabCtrlItem* tab = s->m_vtritems + hItem;
  RECT rctext;
  RECT rc = (tab->rc);
  GetItemState(s, hItem, &state);

  // draw separator (border).
  if (bDrawSeparator == TRUE) {
    _clr = (s->m_SeparationLineColor);

    if (s->m_Layout == OUTLOOKTABCTRL_LAYOUT_1 || s->m_Layout == OUTLOOKTABCTRL_LAYOUT_4) {
      rc.bottom--;
      TMoveTo(hDC, rc.left, rc.bottom);
      _LineTo(hDC, rc.right, rc.bottom);
    }
    else { // OUTLOOKTABCTRL_LAYOUT_2 or OUTLOOKTABCTRL_LAYOUT_3.
      TMoveTo(hDC, rc.left, rc.top);
      _LineTo(hDC, rc.right, rc.top);
      rc.top++;
    }
  }

  // fill background.
  DrawBackground(s, hDC, &state, &rc);
  rctext = (rc);
  rctext.right -= 2;

  // draw icon.
  if (tab->imageBig != -1 && ((bDisabled == FALSE && s->m_ImageListBig->data != NULL) ||
      (bDisabled == TRUE && s->m_ImageListDisabledBig->data != NULL))) {
    const img_t* pImageList = 0;
    RECT rc1 = rc;
    rc1.left = rc.left + s->m_TabTextHorzMargin;

    if (bDisabled == FALSE) {
      pImageList = s->m_ImageListBig;
    }
    else {
      pImageList = s->m_ImageListDisabledBig;
    }

    RectAlign(rc1, pImageList->w, pImageList->h, DT_VCENTER, &rc1);

    if (s->m_hPushedItem == -1 && state.bHighlighted == TRUE && state.bSelected == FALSE) {
      draw_image_monoR(hDC, &rc1, pImageList, tab->imageBig, s->m_IconShadowColor);
      RCOFFSET(&rc1, -2, -2, -2, -2);
    }

    draw_imageR(hDC, &rc1, pImageList, tab->imageBig, 0, 0);
    rctext.left += pImageList->w + 2 * s->m_TabTextHorzMargin;
  }
  else {
    rctext.left += s->m_TabTextHorzMargin;
  }

  if (IsDisabled(hItem)) {
    _clr = s->m_DisabledTabTextColor;
  }
  else {
    BOOL bSelectDark;
    AssignHighlightStateOfItem(s, &state, NULL, &bSelectDark);
    _clr = s->m_TabTextColor[bSelectDark == TRUE];
  }

  // draw text.
  draw_textR(hDC, tab->text, -1, &rctext, DT_VCENTER | DT_LEFT, _clr);
}
void OutlookTabCtrl_DrawButton(const outlooktab_ctrl* s, HDDC hDC, int hItem)
{
  ItemState state;
  const OutlookTabCtrlItem* tab = s->m_vtritems + hItem;
  GetItemState(s, hItem, &state);
  // fill background.
  DrawBackground(s, hDC, &state, &tab->rc);

  // draw icon.
  if (tab->imageSmall != -1 && (s->m_ImageListSmall->data != NULL || s->m_ImageListDisabledSmall->data != NULL)) {
    RECT rc1 = tab->rc;
    const img_t* pImageList = 0;

    if (IsDisabled(hItem) == TRUE && s->m_ImageListDisabledSmall->data != NULL) {
      pImageList = s->m_ImageListDisabledSmall;
    }
    else {
      pImageList = s->m_ImageListSmall;
    }

    RectAlign(tab->rc, pImageList->w, pImageList->h, DT_VCENTER | DT_CENTER, &rc1);

    if (s->m_hPushedItem == -1 && state.bHighlighted == TRUE && state.bSelected == FALSE) {
      draw_image_monoR(hDC, &rc1, pImageList, tab->imageSmall, s->m_IconShadowColor);
      RCOFFSET(&rc1, -2, -2, -2, -2);
    }

    draw_imageR(hDC, &rc1, pImageList, tab->imageSmall, 0, 0);
  }
}
int OutlookTabCtrlBase_OnPaint(HDDC hDC, const outlooktab_ctrl* s, const RECT* prc)
{
  DWORD _clr;
  int _x0, _y0;
  //RECT rc = *prc;
  //Recalc(s, rc);

  if (s->m_bShowCaption == TRUE) {
    const RECT* pRect = &s->m_rcCaption;
    // DrawCaption
    int item = s->m_hCurItem;
    draw_fillrectR(hDC, pRect, s->m_CaptionColor);

    if (item != -1) {
      const char* text = s->m_vtritems[item].text;
      RECT rc = (*pRect);
      rc.left += s->m_CaptionTextHorzMargin;
      draw_textR(hDC, text, -1, &rc, DT_VCENTER | DT_LEFT, s->m_CaptionTextColor);
    }
  }

  // DrawSplitter
  {
    const RECT* pRect = &s->m_rcSplitter;
    draw_fillrectR(hDC, pRect, _GetSysColor(COLOR_BTNFACE));

    if (s->m_bActiveSplitter == TRUE) {
      COLORREF clrTopDot, clrBottomDot;
      RECT rc = (*pRect);
      int count;

      if (s->m_Style == CUSTOMSTYLE_1) {
        DrawGradient(hDC, pRect, 0, _RGB(127, 127, 127), _RGB(71, 71, 71));

        if (s->m_bActiveSplitter == TRUE) {
          count = 7, clrTopDot = _RGB(10, 10, 10), clrBottomDot = _RGB(230, 230, 230);
        }
      }
      else if (s->m_Style == CUSTOMSTYLE_2) {
        DrawGradient(hDC, pRect, 0, _RGB(145, 96, 87), _RGB(121, 80, 72));

        if (s->m_bActiveSplitter == TRUE) {
          count = 7, clrTopDot = _RGB(51, 33, 30), clrBottomDot = _RGB(208, 183, 179);
        }
      }
      else {
        count = 10;
        clrTopDot = _GetSysColor(COLOR_3DDKSHADOW);
        clrBottomDot = _GetSysColor(COLOR_BTNHIGHLIGHT);
        rc.top ++;
        rc.right ++;
        draw_3drect_inR(hDC, &rc, _GetSysColor(COLOR_BTNHIGHLIGHT), _GetSysColor(COLOR_BTNFACE));
        rc.top --;
      }

      //DrawSplitterDots
      {
        // From BCGControlBar Library, file BCGPVisualManager2003.cpp, struct CBCGPVisualManager2003.
        const int nBoxesNumber = count;
        const int nBoxSize = RCH(pRect) - 3;
        int x = CenterPoint(pRect).x - nBoxSize * nBoxesNumber / 2;
        int y = pRect->top + 2;
        int nBox;

        for (nBox = 0; nBox < nBoxesNumber; nBox++) {
          RECT rc;
          SetRect(&rc, x + 1, y + 1, x + 1 + nBoxSize / 2, y + 1 + nBoxSize / 2);
          draw_fillrectR(hDC, &rc, clrBottomDot);
          SetRect(&rc, x, y, x + nBoxSize / 2, y + nBoxSize / 2);
          draw_fillrectR(hDC, &rc, clrTopDot);
          x += nBoxSize;
        }
      }
    }

    if (1) {
      _clr = (s->m_SeparationLineColor);
      TMoveTo(hDC, pRect->left, pRect->top);
      _LineTo(hDC, pRect->right, pRect->top);
      TMoveTo(hDC, pRect->left, pRect->bottom - 1);
      _LineTo(hDC, pRect->right, pRect->bottom - 1);
    }
  }

  if (s->m_bShowMenuButton == TRUE) {
    // DrawButtonMenu
    int x, y;
    const RECT* pRect = &s->m_rcButtonMenu;
    ItemState state;
    enum { BTNMENUICON_WIDTH = 8, BTNMENUICON_HEIGHT = 13};
    GetItemState(s, HIT_MENUBUTTON, &state);
    DrawBackground(s, hDC, &state, pRect);
    x = pRect->left + (RCW(pRect) - BTNMENUICON_WIDTH) / 2;
    y = pRect->top + (RCH(pRect) - BTNMENUICON_HEIGHT) / 2;

    if (1) {
      uchar wImageBits[BTNMENUICON_HEIGHT] = {0x33, 0x99, 0xCC, 0x99, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC1, 0xE3, 0xF7};
      uchar mapA[2] = {255, 0};
      draw_bit(hDC, x, y, BTNMENUICON_WIDTH, BTNMENUICON_HEIGHT, wImageBits, 1, 1, _RGB(0, 0, 0), mapA, 0);
    }
  }

  {
    BOOL visibleButtonsBar = IsVisibleButtonsBar(s);

    if (visibleButtonsBar == TRUE) {
      ItemState state = {FALSE, FALSE, FALSE};
      DrawBackground(s, hDC, &state, &s->m_rcButtons);
    }

    if (s->m_vtritemsLen > 0) {
      int i;

      for (i = 0; i != s->m_vtritemsLen; ++i) {
        if (s->m_vtritems[i].visible == FALSE) {
          continue;
        }

        if (s->m_vtritems[i].button == FALSE) { // this is tab.
          BOOL bDrawSeparator = visibleButtonsBar == TRUE ||
              ((s->m_Layout == OUTLOOKTABCTRL_LAYOUT_1 || s->m_Layout == OUTLOOKTABCTRL_LAYOUT_4) && i != s->m_hBottomVisibleTab) ||
              ((s->m_Layout == OUTLOOKTABCTRL_LAYOUT_2 || s->m_Layout == OUTLOOKTABCTRL_LAYOUT_3) && i != s->m_hTopVisibleTab);
          OutlookTabCtrl_DrawTab(s, hDC, i, bDrawSeparator);
        }
        else { // this is button.
          OutlookTabCtrl_DrawButton(s, hDC, i);
        }
      }
    }
  }

  if (s->m_hCurItem == -1) {
    draw_fillrectR(hDC, &s->m_rcWindows, _GetSysColor(COLOR_WINDOW));
  }

  if (s->m_BorderWidth > 0) {
    draw_3drect_inR(hDC, prc, s->m_BorderColor, s->m_BorderColor);
  }

  return 0;
}
BOOL OnSetCursor(outlooktab_ctrl* s, UINT nHitTest, UINT message)
{
  return 0;
}
int OutlookTabCtrl_event(outlooktab_ctrl* s, const event* e)
{
  int x = e->x, y = e->y;
  POINT pt = iPOINT(x, y);

  switch (e->msg) {
  case WM_LBUTTONDBLCLK: {
    int nHitTest = HitTestEx(s, pt);

    if (s->m_bActiveSplitter == TRUE && nHitTest == HIT_SPLITTER) {
      if (GetCountVisibleButtons(s) > 0) { // there are buttons.
        SetCountItemsInTabState(s, GetCountVisibleItems(s));
      }
      else { // there are not buttons.
        SetCountItemsInTabState(s, 0);
      }
    }
  }
  break;

  case WM_LBUTTONDOWN: {
    int hPushedArea = HitTestEx(s, pt);
    int nHitTest = hPushedArea;

    if (hPushedArea != -1) {
      s->m_hPushedItem = hPushedArea;
    }

    if (s->m_bActiveSplitter == TRUE && nHitTest == HIT_SPLITTER) {
      s->m_ptSplitterDrag = pt;
      s->m_iSplitterDragStartCountTabs = GetCountVisibleTabs(s);
      s->m_hPushedItem = HIT_SPLITTER;
      SetCursor(LoadCursor(NULL, IDC_SIZENS));
    }

    force_redraw(e);
  }
  break;

  case WM_LBUTTONUP: {
#define ShowMenu(x)

    if (s->m_hPushedItem != -1) {
      int hArea = HitTestEx(s, pt);
      BOOL changedSelect = FALSE;

      if (hArea == s->m_hPushedItem) {
        changedSelect = (hArea != HIT_SPLITTER && (hArea == HIT_MENUBUTTON || s->m_hCurItem != hArea)); // select new item.
      }

      s->m_hPushedItem = -1;

      if (changedSelect == TRUE) {
        if (hArea == HIT_MENUBUTTON) {
          ShowMenu(&s->m_rcButtonMenu);
        }
        else {
          s->m_hCurItem = (hArea);
        }
      }

      force_redraw(e);
    }
  }
  break;

  case WM_MOUSEMOVE: {
    if (s->m_hPushedItem == HIT_SPLITTER) {
      float delta = (float)(pt.y - s->m_ptSplitterDrag.y) / (float)(s->m_TabHeight + 1/*border*/);
      int idelta = (int)(delta >= 0 ? delta + 0.5f : delta - 0.5f);
      int needCountTabs = s->m_iSplitterDragStartCountTabs +
          ((s->m_Layout == OUTLOOKTABCTRL_LAYOUT_1 || s->m_Layout == OUTLOOKTABCTRL_LAYOUT_2) ? -idelta : idelta);

      if (needCountTabs < 0) {
        needCountTabs = 0;
      }

      if (needCountTabs > GetCountVisibleItems(s)) {
        needCountTabs = GetCountVisibleItems(s);
      }

      if (needCountTabs != GetCountVisibleTabs(s)) {
        SetCountItemsInTabState(s, needCountTabs);
      }

      force_redraw(e);
    }
    else {
      int hit = HitTestEx(s, pt);

      if (hit != -1) {
        BOOL changeHover = s->m_hHighlightedItem == -1;

        if (hit != s->m_hHighlightedItem) {
          s->m_hHighlightedItem = hit;
          force_redraw(e);
        }
      }

      if (hit == HIT_SPLITTER && s->m_bActiveSplitter == TRUE) {
        SetCursor(LoadCursor(NULL, IDC_SIZENS));
        return TRUE;
      }
    }
  }
  break;
  }

  return 0;
}

