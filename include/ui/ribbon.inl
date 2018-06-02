typedef struct ribbon_button_t {
  int Style, SizeMode;
  int DropDownArrowDirection;
  BOOL Enabled, Pressed, Selected;
  str_t Text[1];
  IRECT DropDownBounds;
  IRECT ButtonFaceBounds;
  int faceCorners;
  int ddCorners;
} ribbon_button_t;
typedef ribbon_button_t RibbonButton;
typedef struct RibbonItem {
  IRECT Bounds;
} RibbonItem;
typedef struct RibbonItemGroup {
  IRECT Bounds;
  str_t Text[1];
  RibbonItem* items;
  int nitem;
} RibbonItemGroup;
typedef struct ribbon_context_t {
  str_t text[1];
  COLOR GlowColor;
  //Ribbon _owner;
  //RibbonTabCollection _tabs;
} ribbon_context_t;
typedef struct ribbon_tab_t {
  IRECT TabBounds;
  IRECT TabContentBounds;
  ribbon_context_t* Context;
  BOOL Selected;
  str_t Text[1];
} ribbon_tab_t;
typedef struct RibbonPanel {
  IRECT Bounds;
  IRECT ContentBounds;
  IRECT ButtonMoreBounds;
  BOOL Selected;
  BOOL ButtonMoreEnabled;
  BOOL ButtonMorePressed;
  BOOL ButtonMoreSelected;
  BOOL Enabled;
  BOOL ButtonMoreVisible;
  img_t Image[1];
  str_t Text[1];
} RibbonPanel;
typedef ribbon_tab_t RibbonTab;
#define CaptionBarHeight 24
typedef struct ribbon_t {
  int hit, hot;
  int ActualBorderMode;
  str_t CaptionText[1];
  BOOL ForceOrbMenu;
  ISIZE _lastSizeMeasured;
  ribbon_tab_t* Tabs;
  int ntab;
  //RibbonTabCollection _tabs;
  IRECT TabsMargin;
  IRECT TabsPadding;
  //RibbonContextCollection _contexts;
  BOOL _minimized;
  int TabSpacing;
  IRECT TabContentMargin;
  IRECT TabContentPadding;
  IRECT PanelPadding;
  IRECT PanelMargin;
  //RibbonTab ActiveTab;
  int PanelSpacing;
  IRECT ItemMargin;
  IRECT ItemPadding;
  //RibbonTab _lastSelectedTab;
  //RibbonMouseSensor _sensor;
  IRECT DropDownMargin;
  IRECT TabTextMargin;
  IRECT OrbBounds;
  float _tabSum;
  BOOL _updatingSuspended;
  BOOL _orbSelected;
  BOOL _orbPressed;
  BOOL OrbVisible;
  img_t _orbImage[1];
  //RibbonQuickAccessToolbar _quickAcessToolbar;
  BOOL QuickAccessVisible;
  //RibbonOrbDropDown _orbDropDown;
  int _borderMode;
  int _actualBorderMode;
  RibbonButton _CloseButton;
  RibbonButton _MaximizeRestoreButton;
  RibbonButton _MinimizeButton;
  BOOL _CaptionButtonsVisible;
} ribbon_t;
#include "RibbonRenderer.inl"
#include "ribbon_draw.inl"
static int ribbon_init(ribbon_t* s)
{
  //s->_tabs = new RibbonTabCollection(this);
  //s->_contexts = new RibbonContextCollection(this);
  s->TabsMargin = iRECT(12, 24 + 2, 20, 0);
  s->TabTextMargin = iRECT(4, 2, 4, 2);
  s->TabsPadding = iRECT(8, 5, 8, 3);
  s->TabContentMargin = iRECT(1, 0, 1, 2);
  s->PanelPadding = iRECT(3, 3, 3, 3);
  s->PanelMargin = iRECT(3, 2, 3, 15);
  s->PanelSpacing = 3;
  s->ItemPadding = iRECT(1, 0, 1, 0);
  s->ItemMargin = iRECT(4, 2, 4, 2);
  s->TabSpacing = 6;
  s->DropDownMargin = iRECT(2, 2, 2, 2);
  s->OrbVisible = TRUE;
  //s->_orbDropDown = new RibbonOrbDropDown(this);
  //s->_quickAcessToolbar = new RibbonQuickAccessToolbar(this);
  s->QuickAccessVisible = TRUE;
  //s->_MinimizeButton = new RibbonCaptionButton(RibbonCaptionButton.CaptionButton.Minimize);
  //s->_MaximizeRestoreButton = new RibbonCaptionButton(RibbonCaptionButton.CaptionButton.Maximize);
  //s->_CloseButton = new RibbonCaptionButton(RibbonCaptionButton.CaptionButton.Close);
  return 0;
}
static int ribbon_calc_rect(ribbon_t* s, IRECT rc)
{
  BOOL graphicsCreated = FALSE;
  ///X coordinate reminder
  ///Saves the width of the larger tab
  ///Saves the bottom of the tabs
  int i, curLeft, maxWidth = 0, tabsBottom = 0;
  ISIZE ClientSize = iSIZE(RCW(&rc), RCH(&rc));
  s->OrbBounds = s->OrbVisible ? iRECT2(4, 4, 36, 36) : iRECT2(4, 4, 0, 0);
  curLeft = s->TabsMargin.l + RCW(&s->OrbBounds);
  for (i = 0; i < s->ntab; ++i) {
    RibbonTab* tab = s->Tabs + i;
    ISIZE textSize = font_text_size(g_scr->font, tab->Text->s, tab->Text->l, 0);
    tab->TabBounds = iRECT2(curLeft, s->TabsMargin.t, s->TabsPadding.l + textSize.w + s->TabsPadding.r, s->TabsPadding.t + textSize.h + s->TabsPadding.b);
    curLeft = tab->TabBounds.r + s->TabSpacing;
    maxWidth = MAX(RCW(&tab->TabBounds), maxWidth);
    tabsBottom = MAX(tab->TabBounds.b, tabsBottom);
    tab->TabContentBounds = iRECT(s->TabContentMargin.l, tabsBottom + s->TabContentMargin.t, ClientSize.w - 1 - s->TabContentMargin.r, ClientSize.h - 1 - s->TabContentMargin.b);
    //if (tab.Active) { tab.UpdatePanelsRegions(); }
  }
  //SetWindowRgn
#if 0
  while (curLeft > ClientRectangle.Right && maxWidth > 0) {
    curLeft = TabsMargin.Left + OrbBounds.Width;
    maxWidth--;
    foreach(RibbonTab tab in Tabs) {
      if (tab.TabBounds.Width >= maxWidth) {
        tab.SetTabBounds(new Rectangle(curLeft, TabsMargin.Top,
            maxWidth, tab.TabBounds.Height));
      }
      else {
        tab.SetTabBounds(new Rectangle(
            new Point(curLeft, TabsMargin.Top),
            tab.TabBounds.Size));
      }
      curLeft = tab.TabBounds.Right + TabSpacing;
    }
  }
  QuickAcessToolbar.MeasureSize(this, new RibbonElementMeasureSizeEventArgs(g, RibbonElementSizeMode.Compact));
  QuickAcessToolbar.SetBounds(new Rectangle(new Point(OrbBounds.Right + QuickAcessToolbar.Margin.Left, OrbBounds.Top - 2), QuickAcessToolbar.LastMeasuredSize));
  if (CaptionButtonsVisible) {
    ISIZE cbs = new Size(20, 20);
    int cbg = 2;
    CloseButton.SetBounds(new Rectangle(new Point(ClientRectangle.Right - cbs.Width - cbg, cbg), cbs));
    MaximizeRestoreButton.SetBounds(new Rectangle(new Point(CloseButton.Bounds.Left - cbs.Width, cbg), cbs));
    MinimizeButton.SetBounds(new Rectangle(new Point(MaximizeRestoreButton.Bounds.Left - cbs.Width, cbg), cbs));
  }
  if (graphicsCreated) {
    g.Dispose();
  }
  _lastSizeMeasured = Size;
  RenewSensor();
#endif
  return 0;
}
static LRESULT ribbon_proc(ctrl_t* sender, ctrl_t* c, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  ribbon_t* s = (ribbon_t*)c->x;
  IRECT rc = c->rc;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  if (WM_CTRLFREE == uMsg) {
    c->ctrlproc = 0;
    free(s);
    return 0;
  }
  if (WM_CTRLINIT == uMsg) {
    return 0;
  }
  switch (uMsg) {
  case WM_SET:
    if (wParam && lParam) {
      return try_setvar(*(char**)wParam, "CaptionText", s->CaptionText, 's', (va_list*)lParam);
    }
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x = 20;
    mminfo->ptMinTrackSize.y = 20;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_TIMER:
    if (1) {
    }
    break;
  case WM_LBUTTONUP:
    if (s->hit) {
      if (iPtInRect(&rc, x, y)) {
        //xSendCmd(XCMD_CLICK);
        force_redraw(c);
      }
      s->hit = 0;
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      s->hit = 1;
      force_redraw(c);
    }
    break;
  case WM_MOUSEMOVE:
    if (s) {
      int hot = iPtInRect(&rc, x, y);
      if (s->hot != hot) {
        s->hot = hot;
        force_redraw(c);
      }
    }
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      ribbon_draw(s, im, rc);
      c->redraw = 0;
    }
    break;
  }
  return 0;
}

