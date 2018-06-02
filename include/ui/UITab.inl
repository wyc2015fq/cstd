typedef enum TAB_LAYOUT {
  TAB_LAYOUT_TOP,
  TAB_LAYOUT_BOTTOM
} TAB_LAYOUT;
typedef enum TAB_BEHAVIOR {
  TAB_BEHAVIOR_SCALE,
  TAB_BEHAVIOR_SCROLL
} TAB_BEHAVIOR;
enum IMAGE { // id of images of system buttons (close, menu, left scroll, right scroll).
  IMAGE_CLOSE,
  IMAGE_MENU, IMAGE_MENU_PARTIAL,
  IMAGE_ARROW_LEFT, IMAGE_ARROW_LEFT_DISABLE,
  IMAGE_ARROW_RIGHT, IMAGE_ARROW_RIGHT_DISABLE
};
enum TabCtrlStyle {
#define SETSTYLEDEF(aa) TabCtrlStyle_##aa,
#include "TabCtrlStyle.inl"
#undef SETSTYLEDEF
};
typedef struct Tab {
  int image;
  BOOL disable;
  RECT rc;
  int width;
  TCHAR text[256];
} Tab;
typedef Tab* HTAB;
#define SYSTEMBUTTON (10000)
#define m_hButClose (SYSTEMBUTTON+1)
#define m_hButMenu (SYSTEMBUTTON+2)
#define m_hButLeftScroll (SYSTEMBUTTON+3)
#define m_hButRightScroll (SYSTEMBUTTON+4)
typedef struct _TabCtrl {
  uictrl base;
  RECT m_rcWindows;
  RECT m_rcTabs;
  RECT m_rcCtrlArea;
  RECT m_rcCloseButton, m_rcMenuButton;
  RECT m_rcScrollLeft, m_rcScrollRight;
  BOOL m_bActive;
  BOOL m_bWatchActivityCtrl;
  BOOL m_bHideSingleTab;
  BOOL m_bScrollLeftAllow;
  BOOL m_bScrollRightAllow;
  BOOL m_bPartialView;
  BOOL m_bEqualTabsSize;
  BOOL m_bShowCloseButton;
  BOOL m_bShowMenuButton;
  BOOL m_bShowScrollButtons;
  int m_nStyle;
  int m_Layout;
  int m_Behavior;
  int m_BorderWidth;
  int m_iTabsOffset, m_iMaxTabsOffset;
  SIZE m_szSysImage;
  img_t m_SysImageList[1];
  img_t m_ImageList[1];
  img_t m_ImageListDisabled[1];
  //HFONT m_Font, m_FontSelect;
  Tab m_vtrtabs[256];
  int m_vtrtabsLen;
  int m_hCurTab, m_hHoverTab, m_hPushedTab;
  int m_nTabImageTextGap;
  COLORREF m_CtrlAreaBackColor;
  COLORREF m_TabSelectedBackColor;
  COLORREF m_EmptyWndsAreaBackColor;
  COLORREF m_TabBorderColor;
  COLORREF m_BorderColor;
  COLORREF m_WndsAreaBackColor;
  COLORREF m_ChildWndBackColor;
  COLORREF m_TabBorderColor2[4];
} TabCtrl;
#include "TabDraw.inl"
int TabCtrl_event(TabCtrl* s, const event* e)
{
  UIEVT(e);
  switch (e->msg) {
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (PtInRect(prc, e->pt)) {
      int hit = TabCtrl_HitTest0(s, e->pt);
      if (hit >= 0 && hit != s->m_hCurTab) {
        s->m_hCurTab = hit;
        force_redraw(e);
      }
    }
    break;
  case WM_LBUTTONUP: {
  }
  break;
  case WM_MOUSEMOVE:
    if (PtInRect(prc, e->pt)) {
      int hit = TabCtrl_HitTest0(s, e->pt);
      if (hit != s->m_hHoverTab) {
        s->m_hHoverTab = hit;
        force_redraw(e);
      }
    }
    else {
      int hit = -1;
      if (hit >= 0 && hit != s->m_hHoverTab) {
        s->m_hHoverTab = hit;
        force_redraw(e);
      }
    }
    break;
  }
  return 0;
}

