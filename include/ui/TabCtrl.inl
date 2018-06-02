// TabCtrl.
#define TIMER_ID_LEFT_SCROLL_CLICK 1
#define TIMER_ID_LEFT_SCROLLING 2
#define TIMER_ID_RIGHT_SCROLL_CLICK 3
#define TIMER_ID_RIGHT_SCROLLING 4
enum TAB_LAYOUT { TAB_LAYOUT_TOP, TAB_LAYOUT_BOTTOM};
enum TAB_BEHAVIOR { TAB_BEHAVIOR_SCALE, TAB_BEHAVIOR_SCROLL };
#include "draw.h"
#define bzero1(x) bzero(x, sizeof(*(x)))
#define TabHit_Null -1
#define TabHit_ButClose -11
#define TabHit_ButMenu -12
#define TabHit_ButLeftScroll -13
#define TabHit_ButRightScroll -14
enum IMAGE {// id of images of system buttons (close, menu, left scroll, right scroll).
  TabCtrl_IMAGE_CLOSE,
  TabCtrl_IMAGE_MENU, TabCtrl_IMAGE_MENU_PARTIAL,
  TabCtrl_IMAGE_ARROW_LEFT, TabCtrl_IMAGE_ARROW_LEFT_DISABLE,
  TabCtrl_IMAGE_ARROW_RIGHT, TabCtrl_IMAGE_ARROW_RIGHT_DISABLE
};
enum {
  TB_CLOSEBUTTONCLICKED, //(IRECT const* pRect, int x, int y);// ptScr - in screen space.
  TB_MENUBUTTONCLICKED, //(IRECT const* pRect, int x, int y);// ptScr - in screen space.
  TB_TABSELECTED, //(int hTab);
  TB_LBUTTONDOWN, //(int hTab, int x, int y);// ptScr - in screen space.
  TB_LBUTTONDBLCLK, //(int hTab, int x, int y);// ptScr - in screen space.
  TB_RBUTTONDOWN, //(int hTab, int x, int y);// ptScr - in screen space, hTab can be NULL.
  TB_RBUTTONUP, //(int hTab, int x, int y);// ptScr - in screen space, hTab can be NULL.
  TB_STARTDRAG, //(int hTab, int x, int y);// ptScr - in screen space.
  TB_DRAG, //(int hTab, int x, int y, BOOL /*outside*/);// ptScr - in screen space, outside==TRUE - dragging out of tabs area.
  TB_FINISHDRAG//( int hTab, BOOL /*cancel*/);// cancel==FALSE - dragging was finished using left button up.
};
//WM_NOTIFY
typedef struct Tab {
  ctrl_t* c;
  int image;
  char* text;
  char* tooltipText;
  BOOL disable;
  __int64 data;
  IRECT rc;
  int width;
} Tab;
typedef struct TabCtrl {
  ctrl_t c[1];
  // TabCtrlDraw
  int (*DrawBorder)(struct TabCtrl* pCtrl, img_t* im, const IRECT* pclip, IRECT const* pRect);
  int (*DrawCtrlAreaBack)(struct TabCtrl* pCtrl, img_t* im, const IRECT* pclip, IRECT const* pRect);
  int (*DrawTab)(struct TabCtrl* pCtrl, img_t* im, const IRECT* pclip, int hTab);
  int (*DrawButton)(struct TabCtrl* pCtrl, img_t* im, const IRECT* pclip, IRECT const* pRect, int type, BOOL bHover, BOOL bPushed);
  int (*DrawWndsAreaBack)(struct TabCtrl* pCtrl, img_t* im, const IRECT* pclip, IRECT const* pRect);
  int (*DrawTabBack)(struct TabCtrl* pCtrl, img_t* im, const IRECT* pclip, int hTab, IRECT const* pRect);
  int (*DrawTabContext)(struct TabCtrl* pCtrl, img_t* im, const IRECT* pclip, int hTab, IRECT const* pRect);
  int (*DrawTabImage)(struct TabCtrl* pCtrl, img_t* im, const IRECT* pclip, int hTab, IRECT const* pRect);
  int (*DrawTabText)(struct TabCtrl* pCtrl, img_t* im, const IRECT* pclip, int hTab, IRECT const* pRect);
  int (*DrawButtonFrame)(struct TabCtrl* pCtrl, img_t* im, const IRECT* pclip, IRECT const* pRect, BOOL bHover, BOOL bPushed);
  COLOR m_clrBorderColor;
  COLOR m_clrTabBorderColor;
  COLOR m_clrCtrlAreaBackColor;
  COLOR m_clrWndsAreaBackColor;
  COLOR m_clrTabSelectedBackColor;
  //COLOR m_clrTabTextColor;
  //COLOR m_clrTabDisableTextColor;
  //COLOR m_clrButtonMarkerColor;
  //COLOR m_clrPushedButtonMarkerColor;
  //COLOR m_clrHoverButtonMarkerColor;
  COLOR m_clrChildWndBackColor;
  COLOR m_clrEmptyWndsAreaBackColor;
  COLOR(*GetTabTextColor)(struct TabCtrl* pCtrl, int hTab);
  COLOR(*GetButtonMarkerColor)(struct TabCtrl* pCtrl, BOOL bHover, BOOL bPushed);
  COLOR(*GetTabBorderColor)(struct TabCtrl* pCtrl, BOOL active, BOOL hover, BOOL disable);
  COLOR(*GetTabOutlineColor)(struct TabCtrl* pCtrl, BOOL active, BOOL hover, BOOL disable, BOOL left);
  COLOR(*GetTabGradientLightColor)(struct TabCtrl* pCtrl, BOOL active, BOOL hover, BOOL disable);
  COLOR(*GetTabGradientDarkColor)(struct TabCtrl* pCtrl, BOOL active, BOOL hover, BOOL disable);
  int (*GetSlantWidth)(struct TabCtrl* pCtrl);
  int (*GetTabOutline)(struct TabCtrl* pCtrl, int hTab, IRECT const* rect, BOOL top, IPOINT pts[8]/*out*/, IRECT* rcFill/*out*/);
  int (*GetBorderWidth)(struct TabCtrl* pCtrl);
  IRECT(*GetCtrlAreaPadding)(struct TabCtrl* pCtrl);
  IRECT(*GetWndsAreaPadding)(struct TabCtrl* pCtrl);
  IRECT(*GetTabHorzMargin)(struct TabCtrl* pCtrl); // uses only IRECT left and IRECT right.
  IRECT(*GetTabPadding)(struct TabCtrl* pCtrl);
  int (*GetTabImageTextGap)(struct TabCtrl* pCtrl); // IMAGE<- gap ->TEXT.
  int (*GetTabExtraWidth)(struct TabCtrl* pCtrl, int hTab); // additional width of tab.
  int (*GetTabMinWidth)(struct TabCtrl* pCtrl); // minimal width of tab.
  IRECT(*GetLeftScrollButtonHorzMargin)(struct TabCtrl* pCtrl); // uses only IRECT left and IRECT right.
  IRECT(*GetRightScrollButtonHorzMargin)(struct TabCtrl* pCtrl); // uses only IRECT left and IRECT right.
  IRECT(*GetMenuButtonHorzMargin)(struct TabCtrl* pCtrl); // uses only IRECT left and IRECT right.
  IRECT(*GetCloseButtonHorzMargin)(struct TabCtrl* pCtrl); // uses only IRECT left and IRECT right.
  int (*CreateToolTip)(struct TabCtrl* pCtrl);
  int (*HitTestItem)(struct TabCtrl* pCtrl, int hTab, BOOL top, int x, int y);
  int m_Layout;
  int m_Behavior;
  char* m_ImageList;
  char* m_ImageListDisabled;
  ISIZE m_szImage, m_szImageDisabled;
  char* m_SysImageList;
  ISIZE m_szSysImage;
  HCURSOR m_hCursor;
  char* m_Font;
  char* m_FontSelect;
  char* m_sToolTip;
  char* m_sToolTipButClose;
  char* m_sToolTipButMenu;
  char* m_sToolTipButLeftScroll;
  char* m_sToolTipButRightScroll;
  BOOL m_bIsStraightOrder;
  BOOL m_bShowBorder;
  BOOL m_bEqualTabsSize;
  BOOL m_bRemoveTabEnable;
  BOOL m_bHideSingleTab;
  BOOL m_bShowCloseButton;
  BOOL m_bShowMenuButton;
  BOOL m_bShowScrollButtons;
  BOOL m_bToolTipEnable;
  int m_iScrollingStep;
  //TabCtrlDraw* m_pDrawManager;
  int m_hCurTab;
  int m_hHoverArea;
  int m_hPushedArea;
  int m_iTabsOffset, m_iMaxTabsOffset;
  BOOL m_bPartialView, m_bScrollLeftAllow, m_bScrollRightAllow;
  BOOL m_bTabDrag;
  IPOINT m_ptStartTabDrag;
  BOOL m_bWatchActivityCtrl;
  BOOL m_bActive;
  IRECT m_rcCtrlArea, m_rcTabs, m_rcWindows;
  IRECT m_rcCloseButton, m_rcMenuButton, m_rcScrollLeft, m_rcScrollRight;
  const char** m_pToolTip;
  Tab* m_vtrtabs;
  int m_count;
  BOOL* m_pLifeStatus;
  BOOL m_bCanShowCloseButton;
  BOOL m_bCanShowMenuButton;
  BOOL m_bCanShowScrollButtons;
} TabCtrl;
static int TabCtrl_Invalidate(TabCtrl* s, BOOL bb)
{
  ctrl_t* c = s->c;
  //HWND hwnd = c->listener->hwnd;
  force_redraw(c);
  return 0;
}
static int TabCtrl_set(TabCtrl* s)
{
  s->m_bIsStraightOrder = TRUE;
  s->m_bCanShowCloseButton = TRUE;
  s->m_bCanShowMenuButton = TRUE;
  s->m_bCanShowScrollButtons = TRUE;
  s->m_vtrtabs = NULL;
  s->m_count = 0;
  s->m_hCurTab = TabHit_Null;
  s->m_hHoverArea = TabHit_Null;
  s->m_hPushedArea = TabHit_Null;
  s->m_iTabsOffset = 0;
  s->m_bPartialView = s->m_bScrollLeftAllow = s->m_bScrollRightAllow = FALSE;
  s->m_bTabDrag = FALSE;
  s->m_bWatchActivityCtrl = FALSE;
  s->m_bActive = FALSE;
  s->m_pToolTip = NULL;
  s->m_Layout = TAB_LAYOUT_TOP;
  s->m_Behavior = TAB_BEHAVIOR_SCALE;
  s->m_bShowBorder = TRUE;
  s->m_bEqualTabsSize = FALSE;
  s->m_bRemoveTabEnable = FALSE;
  s->m_bHideSingleTab = FALSE;
  s->m_bShowCloseButton = TRUE;
  s->m_bShowMenuButton = TRUE;
  s->m_bShowScrollButtons = TRUE;
  s->m_bToolTipEnable = TRUE;
  s->m_iScrollingStep = 15;
  s->m_ImageList = s->m_ImageListDisabled = NULL;
  s->m_SysImageList = NULL;
  s->m_szSysImage.w = s->m_szSysImage.h = 0;
  s->m_szImage = s->m_szImageDisabled = s->m_szSysImage;
  s->m_Font = s->m_FontSelect = NULL;
  s->m_hCursor = NULL;
  s->m_hCursor = NULL;
  s->m_pLifeStatus = NULL;
  return 0;
}
static int TabCtrl_StepLeft(TabCtrl* s)
{
  s->m_iTabsOffset -= s->m_iScrollingStep;
  return 0;
}
static int TabCtrl_StepRight(TabCtrl* s)
{
  s->m_iTabsOffset += s->m_iScrollingStep;
  return 0;
}
static BOOL TabCtrl_IsSystemButton(TabCtrl* s, int hTab)
{
  return hTab == TabHit_ButClose || hTab == TabHit_ButMenu || hTab == TabHit_ButLeftScroll || hTab == TabHit_ButRightScroll;
}
static int TabCtrl_StopScrolling(TabCtrl* s)
{
  if (s->m_hPushedArea != TabHit_Null && TabCtrl_IsSystemButton(s, s->m_hPushedArea) == TRUE) {
    if (s->m_hPushedArea == TabHit_ButLeftScroll) {
      //KillTimer(TIMER_ID_LEFT_SCROLL_CLICK);
      //KillTimer(TIMER_ID_LEFT_SCROLLING);
    }
    else if (s->m_hPushedArea == TabHit_ButRightScroll) {
      //KillTimer(TIMER_ID_RIGHT_SCROLL_CLICK);
      //KillTimer(TIMER_ID_RIGHT_SCROLLING);
    }
    s->m_hPushedArea = TabHit_Null;
  }
  return 0;
}
typedef struct NOTIFYMSG {
  ctrl_t* c;
  void* obj;
  int id;
  int code;
  int x, y;
  void* p;
} NOTIFYMSG;
static int TabCtrl_Notify(TabCtrl* s, int code, int id, int x, int y, void* p)
{
  NOTIFYMSG msg[1] = {0};
  msg->c = s->c;
  msg->obj = s;
  msg->code = code;
  msg->id = id;
  msg->x = x;
  msg->y = y;
  msg->p = p;
  sendmsg(s->c, s->c->listener, WM_NOTIFY, (WPARAM)0, (LPARAM)msg);
  return 0;
}
static int TabCtrl_StopDragging(TabCtrl* s, BOOL cancel)
{
  if (s->m_hPushedArea != TabHit_Null && TabCtrl_IsSystemButton(s, s->m_hPushedArea) == FALSE) {
    int hPushedArea = s->m_hPushedArea;
    s->m_hPushedArea = TabHit_Null;
    if (s->m_bTabDrag == TRUE) {
      s->m_bTabDrag = FALSE;
      TabCtrl_Notify(s, TB_FINISHDRAG, hPushedArea, 0, 0, 0);
    }
  }
  return 0;
}
static int TabCtrl_DeleteAll(TabCtrl* s)
{
  TabCtrl_StopScrolling(s);
  TabCtrl_StopDragging(s, TRUE);
  s->m_hHoverArea = s->m_hPushedArea = s->m_hCurTab = TabHit_Null;
  s->m_iTabsOffset = 0;
  s->m_bPartialView = s->m_bScrollLeftAllow = s->m_bScrollRightAllow = FALSE;
  return 0;
}
static int TabCtrl_OnDestroy(TabCtrl* s)
{
  TabCtrl_DeleteAll(s);
  if (s->m_hCursor != NULL) {
    DestroyCursor(s->m_hCursor);
  }
  if (s->m_pLifeStatus != NULL) {
    *s->m_pLifeStatus = FALSE;
  }
  return 0;
}
static int TabCtrl_OnActive(TabCtrl* s, BOOL active) // ActivityHook INotify.
{
  if (active != s->m_bActive) {
    s->m_bActive = active;
    TabCtrl_Invalidate(s, FALSE);
  }
  return 0;
}
static int TabCtrl_GetFirstEnableTab(TabCtrl* s)
{
  int i;
  for (i = 0; i < s->m_count; ++i) {
    if (s->m_vtrtabs[i].disable == FALSE) {
      return i;
    }
  }
  return TabHit_Null;
}
static int TabCtrl_Insert(TabCtrl* s, int before, ctrl_t* c, TCHAR* text, int image)
{
  Tab* tab = NULL;
  if (s->m_count > 0) {
    if (before < 0) {
      before = (before + s->m_count + 1) % s->m_count;
    }
    else {
      before = (before) % s->m_count;
    }
  }
  else {
    before = 0;
  }
  tab = s->m_vtrtabs + before;
  MEMMOVE(tab + 1, tab, s->m_count - before);
  tab->c = c;
  tab->image = image;
  tab->text = text;
  tab->disable = FALSE;
  tab->data = 0;
  ++s->m_count;
  if (s->m_hCurTab == TabHit_Null) {
    s->m_hCurTab = TabCtrl_GetFirstEnableTab(s);
  }
  return before;
}
static int TabCtrl_Add(TabCtrl* s, ctrl_t* c, TCHAR* text, int image)
{
  return TabCtrl_Insert(s, -1, c, text, image);
}
static BOOL TabCtrl_IsExist(TabCtrl* s, int hTab)
{
  return 0 <= hTab && hTab < s->m_count;
}
static int TabCtrl_GetNextEnableTab(TabCtrl* s, int hTab)
{
  int i;
  for (i = 0 + (hTab) + 1; i < s->m_count; ++i) {
    if (s->m_vtrtabs[i].disable == FALSE) {
      return i;
    }
  }
  return TabHit_Null;
}
static int TabCtrl_GetPrevEnableTab(TabCtrl* s, int hTab)
{
  int i;
  for (i = hTab; i > 0;) {
    if (s->m_vtrtabs[--i].disable == FALSE) {
      return i;
    }
  }
  return TabHit_Null;
}
static int TabCtrl_Delete(TabCtrl* s, int hTab)
{
  ASSERT(TabCtrl_IsExist(s, hTab) == TRUE);
  if (hTab == s->m_hPushedArea) {
    TabCtrl_StopScrolling(s);
    TabCtrl_StopDragging(s, TRUE);
  }
  if (hTab == s->m_hHoverArea) {
    s->m_hHoverArea = TabHit_Null;
  }
  if (hTab < s->m_count) {
    MEMMOVE(s->m_vtrtabs + hTab, s->m_vtrtabs + hTab + 1, s->m_count - hTab - 1);
    if (s->m_hCurTab == hTab) {
      s->m_hCurTab = TabCtrl_GetNextEnableTab(s, hTab);
      if (s->m_hCurTab == TabHit_Null) {
        s->m_hCurTab = TabCtrl_GetPrevEnableTab(s, hTab);
      }
    }
    --s->m_count;
  }
  if (0 == s->m_count) {
    s->m_iTabsOffset = 0;
    s->m_bPartialView = s->m_bScrollLeftAllow = s->m_bScrollRightAllow = FALSE;
  }
  return 0;
}
static BOOL TabCtrl_SetImageLists(TabCtrl* s, char* pImageList, char* pImageListDisabled)
{
  img_t* im1;
  BOOL res = TRUE;
  ASSERT(pImageList == NULL || pImageList != NULL);
  ASSERT(pImageListDisabled == NULL || pImageListDisabled != NULL);
  s->m_ImageList = pImageList;
  s->m_ImageListDisabled = pImageListDisabled;
  im1 = sys_find_img(s->c->sys, pImageList);
  if (im1 != NULL) {
    s->m_szImage = iSIZE(im1->w, im1->h);
  }
  else {
    s->m_szImage = iSIZE(0, 0);
  }
  im1 = sys_find_img(s->c->sys, pImageListDisabled);
  if (im1 != NULL) {
    s->m_szImageDisabled = iSIZE(im1->w, im1->h);
  }
  else {
    s->m_szImageDisabled = iSIZE(0, 0);
  }
  return res;
}
static BOOL TabCtrl_SetSystemImageList(TabCtrl* s, char* pImageList)
{
  img_t* im1;
  ASSERT(pImageList == NULL || pImageList != NULL);
  s->m_SysImageList = pImageList;
  im1 = sys_find_img(s->c->sys, pImageList);
  if (im1 != NULL) {
    s->m_szSysImage = iSIZE(im1->w, im1->h);
  }
  else {
    s->m_szSysImage = iSIZE(0, 0);
  }
  return TRUE;
}
static ISIZE TabCtrl_GetSystemImageSize(TabCtrl* s)
{
  return s->m_szSysImage;
}
static BOOL TabCtrl_SetCursor(TabCtrl* s, HCURSOR hCursor)
{
  if (s->m_hCursor != NULL) {
    DestroyCursor(s->m_hCursor);
    s->m_hCursor = NULL;
  }
  s->m_hCursor = NULL;
  if (hCursor != NULL) {
    s->m_hCursor = (HCURSOR)CopyImage(hCursor, IMAGE_CURSOR, 0, 0, 0);
    if (s->m_hCursor == NULL) {
      return FALSE;
    }
  }
  return TRUE;
}
static int TabCtrl_CalcCtrlAreaHeight(TabCtrl* s)
{
  int iTextHeight = font_text_size(s->c->fo, ("H"), 1, 0).h;
  int iCtrlAreaHeight;
  IRECT rcCtrlAreaPadding = s->GetCtrlAreaPadding(s);
  IRECT rcTabPadding = s->GetTabPadding(s);
  iCtrlAreaHeight = rcCtrlAreaPadding.t +
      max(rcTabPadding.t + max(max(s->m_szImage.h, s->m_szImageDisabled.h), iTextHeight) + rcTabPadding.b, s->m_szSysImage.h) +
      rcCtrlAreaPadding.b;
  return iCtrlAreaHeight;
}
static int TabCtrl_CalcTabWidth(TabCtrl* s, int hTab)
{
  int textWidth;
  int imageWidth = 0;
  int tabWidth = 0;
  ASSERT(TabCtrl_IsExist(s, hTab) == TRUE);
  if (s->m_vtrtabs[hTab].image != -1 &&
      ((s->m_vtrtabs[hTab].disable == FALSE && s->m_ImageList != NULL && s->m_ImageList != NULL) ||
          (s->m_vtrtabs[hTab].disable == TRUE && s->m_ImageListDisabled != NULL && s->m_ImageListDisabled != NULL))) {
    imageWidth = (s->m_vtrtabs[hTab].disable == FALSE ? s->m_szImage.w : s->m_szImageDisabled.w) + s->GetTabImageTextGap(s);
  }
  {
    IRECT rcTabPadding = s->GetTabPadding(s);
    textWidth = font_text_size(s->c->fo, s->m_vtrtabs[hTab].text, -1, 0).w;
    tabWidth = rcTabPadding.l + imageWidth + textWidth + s->GetTabExtraWidth(s, hTab) + rcTabPadding.r;
  }
  return tabWidth;
}
static int TabCtrl_CalcTabsWidth(TabCtrl* s)
{
  int i;
  int iMinTabWidth = s->GetTabMinWidth(s);
  if (!s->m_bEqualTabsSize) {
    for (i = 0; i < s->m_count; ++i) {
      s->m_vtrtabs[i].width = max(iMinTabWidth, TabCtrl_CalcTabWidth(s, i));
    }
  }
  else {
    int maxWidth = 0;
    for (i = 0; i < s->m_count; ++i) {
      maxWidth = max(maxWidth, TabCtrl_CalcTabWidth(s, i));
    }
    maxWidth = max(maxWidth, iMinTabWidth);
    for (i = 0; i < s->m_count; ++i) {
      s->m_vtrtabs[i].width = maxWidth;
    }
  }
  return 0;
}
static int TabCtrl_GetFullTabsWidth(TabCtrl* s)
{
  int i, width = 0;
  IRECT rcTabHorzMargin;
  for (i = 0; i < s->m_count; ++i) {
    width += s->m_vtrtabs[i].width;
  }
  rcTabHorzMargin = s->GetTabHorzMargin(s);
  return width + s->m_count * (rcTabHorzMargin.l + rcTabHorzMargin.r);
}
static int TabCtrl_RecalcScale(TabCtrl* s, int iVisibleTabsWidth, int iFullTabsWidth)
{
  IRECT rcTabHorzMargin = s->GetTabHorzMargin(s);
  BOOL bPartialView = (iFullTabsWidth > iVisibleTabsWidth);
  int pos = s->m_rcTabs.l;
  int i;
  if (bPartialView == FALSE)
    for (i = 0; i < s->m_count; ++i) {
      s->m_vtrtabs[i].rc = s->m_rcTabs;
      s->m_vtrtabs[i].rc.l = pos;
      s->m_vtrtabs[i].rc.r = pos += (rcTabHorzMargin.l + s->m_vtrtabs[i].width + rcTabHorzMargin.r);
      iRectDeflate(&s->m_vtrtabs[i].rc, rcTabHorzMargin.l, 0, rcTabHorzMargin.r, 0);
    }
  else {
    int iMinTabWidth = s->GetTabMinWidth(s);
    int totalTabsIndent = s->m_count * (rcTabHorzMargin.l + rcTabHorzMargin.r);
    int iEqualWidth = max(1, (iVisibleTabsWidth - totalTabsIndent) / (int)s->m_count);
    if (s->m_bEqualTabsSize == TRUE)
      for (i = 0; i < s->m_count; ++i) {
        s->m_vtrtabs[i].rc = s->m_rcTabs;
        s->m_vtrtabs[i].rc.l = pos;
        s->m_vtrtabs[i].rc.r = pos += (rcTabHorzMargin.l + max(iMinTabWidth, iEqualWidth) + rcTabHorzMargin.r);
        iRectDeflate(&s->m_vtrtabs[i].rc, rcTabHorzMargin.l, 0, rcTabHorzMargin.r, 0);
      }
    else {
      int width, iTail, iTotalCorrectWidth = 0;
      for (i = 0; i < s->m_count; ++i)
        if (s->m_vtrtabs[i].width > iEqualWidth) {
          iTotalCorrectWidth += s->m_vtrtabs[i].width - iEqualWidth;
        }
      iTail = iFullTabsWidth - iVisibleTabsWidth;
      for (i = 0; i < s->m_count; ++i) {
        if (i < s->m_count - 1) {
          if (s->m_vtrtabs[i].width <= iEqualWidth) {
            width = s->m_vtrtabs[i].width;
          }
          else {
            width = max(iMinTabWidth, s->m_vtrtabs[i].width - (int)((double)iTail * ((double)(s->m_vtrtabs[i].width - iEqualWidth) / (double)iTotalCorrectWidth) + 0.5));
          }
        }
        else {
          width = max(iMinTabWidth, (s->m_rcTabs.r - 1) - pos - (rcTabHorzMargin.l + rcTabHorzMargin.r));
        }
        s->m_vtrtabs[i].rc = s->m_rcTabs;
        s->m_vtrtabs[i].rc.l = pos;
        s->m_vtrtabs[i].rc.r = pos += (rcTabHorzMargin.l + width + rcTabHorzMargin.r);
        iRectDeflate(&s->m_vtrtabs[i].rc, rcTabHorzMargin.l, 0, rcTabHorzMargin.r, 0);
      }
    }
  }
  return 0;
}
static int TabCtrl_RecalcScroll(TabCtrl* s)
{
  IRECT rcTabHorzMargin = s->GetTabHorzMargin(s);
  int i, pos = s->m_rcTabs.l - s->m_iTabsOffset;
  for (i = 0; i < s->m_count; ++i) {
    s->m_vtrtabs[i].rc = s->m_rcTabs;
    s->m_vtrtabs[i].rc.l = pos;
    s->m_vtrtabs[i].rc.r = pos += (rcTabHorzMargin.l + s->m_vtrtabs[i].width + rcTabHorzMargin.r);
    iRectDeflate(&s->m_vtrtabs[i].rc, rcTabHorzMargin.l, 0, rcTabHorzMargin.r, 0);
  }
  return 0;
}
// Recalculate control.
static int TabCtrl_Recalc(TabCtrl* s, BOOL redraw)
{
  int count, hOldCurTab, iSysImagePosY;
  int iVisibleTabsWidth, iFullTabsWidth;
  IRECT rcCtrlAreaPadding;
  BOOL bHideSingleTab, bShowCloseButton, bShowMenuButton, bShowScrollButtons;
  s->m_rcWindows = s->c->rc;
  if (s->m_bShowBorder == TRUE) {
    int width = s->GetBorderWidth(s);
    RCDEFLATE(&s->m_rcWindows, width, width);
  }
  count = s->m_count;
  bHideSingleTab = (count == 1 && s->m_bHideSingleTab == TRUE);
  if (count > 0 && bHideSingleTab == FALSE) {
    s->m_rcCtrlArea = s->m_rcWindows;
    if (s->m_Layout == TAB_LAYOUT_TOP) {
      s->m_rcWindows.t = s->m_rcCtrlArea.b = s->m_rcCtrlArea.t + TabCtrl_CalcCtrlAreaHeight(s);
      s->m_rcWindows.b = max(s->m_rcWindows.b, s->m_rcWindows.t);
    }
    else {
      s->m_rcWindows.b = s->m_rcCtrlArea.t = s->m_rcCtrlArea.b - TabCtrl_CalcCtrlAreaHeight(s);
      s->m_rcWindows.t = min(s->m_rcWindows.t, s->m_rcWindows.b);
    }
    if (s->m_hCurTab != TabHit_Null && TabCtrl_IsExist(s, s->m_hCurTab) == FALSE) {
      s->m_hCurTab = TabHit_Null;
    }
    hOldCurTab = s->m_hCurTab;
    if (s->m_hCurTab == TabHit_Null) {
      s->m_hCurTab = TabCtrl_GetFirstEnableTab(s);
    }
    else if (s->m_vtrtabs[s->m_hCurTab].disable) {
      int hCurTab = TabCtrl_GetNextEnableTab(s, s->m_hCurTab);
      if (hCurTab == TabHit_Null) {
        hCurTab = TabCtrl_GetPrevEnableTab(s, s->m_hCurTab);
      }
      s->m_hCurTab = hCurTab;
    }
    if (s->m_hCurTab != TabHit_Null) {
      IRECT rcWindows = (s->m_rcWindows);
      IRECT rc1 = s->GetWndsAreaPadding(s);
      iRectDeflateR(&rcWindows, &rc1);
      if (s->m_vtrtabs[s->m_hCurTab].c) {
        s->m_vtrtabs[s->m_hCurTab].c->rc = rcWindows;
      }
    }
    if (hOldCurTab != s->m_hCurTab) {
    }
    rcCtrlAreaPadding = s->GetCtrlAreaPadding(s);
    s->m_rcTabs = s->m_rcCtrlArea;
    iRectDeflateR(&s->m_rcTabs, &rcCtrlAreaPadding);
    TabCtrl_CalcTabsWidth(s);
    bShowCloseButton = (s->m_bShowCloseButton == TRUE &&
        s->m_SysImageList != NULL && s->m_SysImageList != NULL &&
        s->m_bCanShowCloseButton);
    bShowMenuButton = (s->m_bShowMenuButton == TRUE &&
        s->m_SysImageList != NULL && s->m_SysImageList != NULL &&
        s->m_bCanShowMenuButton);
    bShowScrollButtons = (s->m_Behavior == TAB_BEHAVIOR_SCROLL && s->m_bShowScrollButtons == TRUE &&
        s->m_SysImageList != NULL && s->m_SysImageList != NULL &&
        s->m_bCanShowScrollButtons);
    iSysImagePosY = (s->m_rcTabs.t + s->m_rcTabs.b - s->m_szSysImage.h) / 2;
    if (bShowCloseButton == TRUE) {
      IRECT rcCloseHorzMargin = s->GetCloseButtonHorzMargin(s);
      s->m_rcCloseButton = s->m_rcTabs;
      s->m_rcCloseButton.r -= rcCloseHorzMargin.r;
      s->m_rcCloseButton.l = s->m_rcCloseButton.r - s->m_szSysImage.w;
      s->m_rcTabs.r = s->m_rcCloseButton.l - rcCloseHorzMargin.l;
      s->m_rcCloseButton.t = iSysImagePosY;
      s->m_rcCloseButton.b = s->m_rcCloseButton.t + s->m_szSysImage.h;
    }
    else {
      bzero1(&s->m_rcCloseButton);
    }
    if (bShowMenuButton == TRUE) {
      IRECT rcMenuHorzMargin = s->GetMenuButtonHorzMargin(s);
      s->m_rcMenuButton = s->m_rcTabs;
      s->m_rcMenuButton.r -= rcMenuHorzMargin.r;
      s->m_rcMenuButton.l = s->m_rcMenuButton.r - s->m_szSysImage.w;
      s->m_rcTabs.r = s->m_rcMenuButton.l - rcMenuHorzMargin.l;
      s->m_rcMenuButton.t = iSysImagePosY;
      s->m_rcMenuButton.b = s->m_rcMenuButton.t + s->m_szSysImage.h;
    }
    else {
      bzero1(&s->m_rcMenuButton);
    }
    if (bShowScrollButtons == TRUE) {
      IRECT rcLeftScrollMargin = s->GetLeftScrollButtonHorzMargin(s);
      IRECT rcRightScrollMargin = s->GetRightScrollButtonHorzMargin(s);
      s->m_rcScrollRight = s->m_rcTabs;
      s->m_rcScrollRight.r -= rcRightScrollMargin.r;
      s->m_rcScrollRight.l = s->m_rcScrollRight.r - s->m_szSysImage.w;
      s->m_rcScrollLeft.r = s->m_rcScrollRight.l - rcRightScrollMargin.l - rcLeftScrollMargin.r;
      s->m_rcScrollLeft.l = s->m_rcScrollLeft.r - s->m_szSysImage.w;
      s->m_rcTabs.r = s->m_rcScrollLeft.l - rcLeftScrollMargin.l;
      s->m_rcScrollLeft.t = s->m_rcScrollRight.t = iSysImagePosY;
      s->m_rcScrollLeft.b = s->m_rcScrollRight.b = s->m_rcScrollLeft.t + s->m_szSysImage.h;
    }
    else {
      bzero1(&s->m_rcScrollLeft);
      bzero1(&s->m_rcScrollRight);
    }
    iVisibleTabsWidth = max(0, RCW(&s->m_rcTabs) - 1);
    iFullTabsWidth = TabCtrl_GetFullTabsWidth(s);
    s->m_iMaxTabsOffset = max(0, iFullTabsWidth - iVisibleTabsWidth);
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
      Tab* tab = s->m_vtrtabs;
      bzero1(&tab->rc);
      tab->width = 0;
      if (0 <= s->m_hCurTab && s->m_hCurTab < s->m_count && (s->m_hCurTab) != 0) {
        //ShowWindow(s->m_vtrtabs[s->m_hCurTab].hWnd, SW_HIDE);
      }
      s->m_hCurTab = (tab->disable == FALSE ? 0 : TabHit_Null);
      if (0 <= s->m_hCurTab && s->m_hCurTab < s->m_count) {
        IRECT rcWindows = (s->m_rcWindows);
        IRECT rcWndsAreaPadding = s->GetWndsAreaPadding(s);
        iRectDeflateR(&rcWindows, &rcWndsAreaPadding);
        s->m_vtrtabs[s->m_hCurTab].c->rc = rcWindows;
      }
    }
    bzero1(&s->m_rcCtrlArea);
    bzero1(&s->m_rcTabs);
    bzero1(&s->m_rcCloseButton);
    bzero1(&s->m_rcMenuButton);
    bzero1(&s->m_rcScrollLeft);
    bzero1(&s->m_rcScrollRight);
  }
  return 0;
}
static int TabCtrl_Update(TabCtrl* s, BOOL redraw)
{
  TabCtrl_Recalc(s, redraw);
  if (redraw == TRUE) {
    TabCtrl_Invalidate(s, FALSE);
  }
  return 0;
}
static int TabCtrl_OnSize(TabCtrl* s)
{
  TabCtrl_Recalc(s, TRUE);
  return 0;
}
static int TabCtrl_TtnNeedText(TabCtrl* s, NMTTDISPINFO* pTT)
{
  s->m_sToolTip = ("");
  if (s->m_hHoverArea != TabHit_Null)
    if (s->m_hHoverArea == TabHit_ButClose) {
      s->m_sToolTip = s->m_sToolTipButClose;
    }
    else if (s->m_hHoverArea == TabHit_ButMenu) {
      s->m_sToolTip = s->m_sToolTipButMenu;
    }
    else if (s->m_hHoverArea == TabHit_ButLeftScroll) {
      s->m_sToolTip = s->m_sToolTipButLeftScroll;
    }
    else if (s->m_hHoverArea == TabHit_ButRightScroll) {
      s->m_sToolTip = s->m_sToolTipButRightScroll;
    }
    else {
      Tab* tab = s->m_vtrtabs + s->m_hHoverArea;
      IRECT rc = (tab->rc);
      rc.l = max(rc.l, s->m_rcTabs.l);
      rc.r = min(rc.r, s->m_rcTabs.r);
      if (iRectIsNull(&rc) == FALSE) {
        if (STRisempty(tab->tooltipText) == FALSE) {
          s->m_sToolTip = tab->tooltipText;
        }
        else if (RCW(&rc) < tab->width) {// partial view of tab.
          s->m_sToolTip = tab->text;
        }
      }
    }
  pTT->lpszText = (TCHAR*)(TCHAR*)s->m_sToolTip;
  return 0;
}
static BOOL TabCtrl_OnNotify(TabCtrl* s, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
  NMTTDISPINFO* info = (NMTTDISPINFO*)lParam;
  ctrl_t* c = s->c;
  if (info->hdr.code == TTN_NEEDTEXT) {
    TabCtrl_TtnNeedText(s, info);
  }
  else {
    *pResult = 0;
    sendmsg(c, c->listener, WM_NOTIFY, wParam, lParam);
  }
  return TRUE;
}
static BOOL TabCtrl_IsTabVisible(TabCtrl* s, int hTab, BOOL* partially/*out*/)
{
  IRECT rc;
  ASSERT(TabCtrl_IsExist(s, hTab) == TRUE);
  rc = s->m_vtrtabs[hTab].rc;
  if (rc.r <= s->m_rcTabs.l || rc.l >= s->m_rcTabs.r) {
    if (partially != NULL) {
      *partially = FALSE;
    }
    return FALSE;
  }
  else {
    if (partially != NULL) {
      *partially = (rc.l < s->m_rcTabs.l || rc.r > s->m_rcTabs.r);
    }
    return TRUE;
  }
}
static int TabCtrl_OnPaint(TabCtrl* s, img_t* im)
{
  int i;
  IRECT pclip[1];
  IRECT rcWndsAreaPadding;
  pclip[0] = s->c->rc;
  if (s->m_count > 0) {
    s->DrawCtrlAreaBack(s, im, pclip, &s->m_rcCtrlArea);
    pclip[0] = iRECT(s->m_rcTabs.l, s->m_rcTabs.t, max(s->m_rcTabs.l, s->m_rcTabs.r), s->m_rcTabs.b);
    if (s->m_bIsStraightOrder) {// left to right.
      for (i = 0; i < s->m_count; ++i) {
        if (i != s->m_hCurTab && TabCtrl_IsTabVisible(s, i, NULL) == TRUE) {
          s->DrawTab(s, im, pclip, i);
        }
      }
    }
    else {// right to left.
      for (i = s->m_count - 1; i >= 0; --i) {
        if (i != s->m_hCurTab && TabCtrl_IsTabVisible(s, i, NULL) == TRUE) {
          s->DrawTab(s, im, pclip, i);
        }
      }
    }
    if (s->m_hCurTab != TabHit_Null) {
      if (s->m_bWatchActivityCtrl == FALSE || s->m_bActive == TRUE) {
        s->DrawTab(s, im, pclip, s->m_hCurTab);
      }
      else {
        s->DrawTab(s, im, pclip, s->m_hCurTab);
      }
    }
    if (iRectIsNull(&s->m_rcScrollLeft) == 0) {
      BOOL bHover = s->m_hHoverArea == TabHit_ButLeftScroll && (s->m_hPushedArea == TabHit_Null || s->m_hPushedArea == TabHit_ButLeftScroll);
      int iFrame;
      iFrame = (s->m_bScrollLeftAllow == FALSE ? TabCtrl_IMAGE_ARROW_LEFT_DISABLE : TabCtrl_IMAGE_ARROW_LEFT);
      s->DrawButton(s, im, pclip, &s->m_rcScrollLeft, iFrame, bHover, s->m_hPushedArea == TabHit_ButLeftScroll);
      bHover = s->m_hHoverArea == TabHit_ButRightScroll && (s->m_hPushedArea == TabHit_Null || s->m_hPushedArea == TabHit_ButRightScroll);
      iFrame = (s->m_bScrollRightAllow == FALSE ? TabCtrl_IMAGE_ARROW_RIGHT_DISABLE : TabCtrl_IMAGE_ARROW_RIGHT);
      s->DrawButton(s, im, pclip, &s->m_rcScrollRight, iFrame, bHover, s->m_hPushedArea == TabHit_ButRightScroll);
    }
    if (iRectIsNull(&s->m_rcMenuButton) == 0) {
      BOOL bHover = s->m_hHoverArea == TabHit_ButMenu && (s->m_hPushedArea == TabHit_Null || s->m_hPushedArea == TabHit_ButMenu);
      int iFrame = (s->m_bPartialView == FALSE ? TabCtrl_IMAGE_MENU : TabCtrl_IMAGE_MENU_PARTIAL);
      s->DrawButton(s, im, pclip, &s->m_rcMenuButton, iFrame, bHover, s->m_hPushedArea == TabHit_ButMenu);
    }
    if (iRectIsNull(&s->m_rcCloseButton) == 0) {
      BOOL bHover = s->m_hHoverArea == TabHit_ButClose && (s->m_hPushedArea == TabHit_Null || s->m_hPushedArea == TabHit_ButClose);
      s->DrawButton(s, im, pclip, &s->m_rcCloseButton, TabCtrl_IMAGE_CLOSE, bHover, s->m_hPushedArea == TabHit_ButClose);
    }
  }
  rcWndsAreaPadding = s->GetWndsAreaPadding(s);
  if (s->m_count <= 0 || s->m_hCurTab < 0 || iRectIsNull(&rcWndsAreaPadding) == 0) {
    s->DrawWndsAreaBack(s, im, pclip, &s->m_rcWindows);
  }
  if (s->m_bShowBorder == TRUE) {
    if (s->GetBorderWidth(s) > 0) {
      IRECT rc = s->c->rc;
      s->DrawBorder(s, im, pclip, &rc);
    }
  }
  return 0;
}
static int TabCtrl_EnsureVisible(TabCtrl* s, int hTab)
{
  if (s->m_Behavior == TAB_BEHAVIOR_SCROLL) {
    int i = (hTab);
    if (s->m_vtrtabs[i].rc.l < s->m_rcTabs.l) {
      s->m_iTabsOffset -= s->m_rcTabs.l - s->m_vtrtabs[i].rc.l;
    }
    else if (s->m_vtrtabs[i].rc.r > s->m_rcTabs.r) {
      s->m_iTabsOffset += s->m_vtrtabs[i].rc.r - s->m_rcTabs.r;
    }
  }
  return 0;
}
static int TabCtrl_ScrollToBegin(TabCtrl* s)
{
  if (s->m_Behavior == TAB_BEHAVIOR_SCROLL) {
    s->m_iTabsOffset = 0;
  }
  return 0;
}
static int TabCtrl_ScrollToEnd(TabCtrl* s)
{
  if (s->m_Behavior == TAB_BEHAVIOR_SCROLL) {
    s->m_iTabsOffset = s->m_iMaxTabsOffset;
  }
  return 0;
}
static int TabCtrl_HitTest1(TabCtrl* s, int x, int y) // get tab in the given point.
{
  if (iPtInRect(&s->m_rcTabs, x, y)) {
    int i = 0;
    if (iPtInRect(&s->m_rcCloseButton, x, y)) {
      return TabHit_ButClose;
    }
    else if (iPtInRect(&s->m_rcMenuButton, x, y)) {
      return TabHit_ButMenu;
    }
    else if (iPtInRect(&s->m_rcScrollLeft, x, y)) {
      return TabHit_ButLeftScroll;
    }
    else if (iPtInRect(&s->m_rcScrollRight, x, y)) {
      return TabHit_ButRightScroll;
    }
    for (i = 0; i < s->m_count; ++i) {
      if (s->HitTestItem(s, i, (s->m_Layout == TAB_LAYOUT_TOP), x, y)) {
        return i;
      }
    }
  }
  return TabHit_Null;
}
static int TabCtrl_StartDragging(TabCtrl* s, int x, int y)
{
  s->m_bTabDrag = TRUE;
  TabCtrl_Notify(s, TB_STARTDRAG, s->m_hPushedArea, x, y, 0);
  return 0;
}
static int TabCtrl_CancelDragging(TabCtrl* s)
{
  TabCtrl_StopDragging(s, TRUE);
  return 0;
}
static int TabCtrl_OnMouseMove(TabCtrl* s, UINT nFlags, int x, int y)
{
  int i, hit = TabCtrl_HitTest1(s, x, y);
  if (hit != s->m_hHoverArea) {
    s->m_hHoverArea = hit;
    TabCtrl_Invalidate(s, 1);
  }
  if (s->m_bRemoveTabEnable == TRUE && s->m_hPushedArea >= 0) {
    if (s->m_bTabDrag == FALSE) {
      IRECT rc1 = iRECT2(s->m_ptStartTabDrag.x - 3, s->m_ptStartTabDrag.y - 3, 6, 6);
      if (iPtInRect(&rc1, x, y) == 0) {
        TabCtrl_StartDragging(s, x, y);
      }
    }
    else {
      if (iPtInRect(&s->m_rcCtrlArea, x, y) == 0) {
        TabCtrl_Notify(s, TB_DRAG, s->m_hPushedArea, x, y, 0);
      }
      else {
        if (s->m_hHoverArea >= 0 && s->m_hHoverArea < s->m_count && s->m_hPushedArea != s->m_hHoverArea) {
          if ((s->m_hHoverArea) < (s->m_hPushedArea)) {
            IRECT rc = s->m_vtrtabs[s->m_hHoverArea].rc;
            rc.r = rc.l + RCW(&s->m_vtrtabs[s->m_hPushedArea].rc);
            if (iPtInRect(&rc, x, y)) {
              Tab tab = s->m_vtrtabs[s->m_hPushedArea];
              for (i = s->m_hPushedArea; i > s->m_hHoverArea; --i) {
                s->m_vtrtabs[i] = s->m_vtrtabs[i - 1];
              }
              s->m_vtrtabs[s->m_hHoverArea] = tab;
              s->m_hPushedArea = s->m_hHoverArea;
              TabCtrl_Recalc(s, TRUE);
              TabCtrl_Invalidate(s, FALSE);
            }
          }
          else {
            IRECT rc = s->m_vtrtabs[s->m_hHoverArea].rc;
            rc.l = rc.r - RCW(&s->m_vtrtabs[s->m_hPushedArea].rc);
            if (iPtInRect(&rc, x, y)) {
              Tab tab = s->m_vtrtabs[s->m_hPushedArea];
              for (i = s->m_hPushedArea; i < s->m_hHoverArea; ++i) {
                s->m_vtrtabs[i] = s->m_vtrtabs[i + 1];
              }
              s->m_vtrtabs[s->m_hHoverArea] = tab;
              s->m_hPushedArea = s->m_hHoverArea;
              TabCtrl_Recalc(s, TRUE);
              TabCtrl_Invalidate(s, FALSE);
            }
          }
        }
      }
    }
  }
  return 0;
}
static LRESULT TabCtrl_OnMouseLeave(TabCtrl* s, WPARAM wp, LPARAM lp)
{
  if (s->m_hHoverArea != TabHit_Null) {
    s->m_hHoverArea = TabHit_Null;
    TabCtrl_Invalidate(s, FALSE);
  }
  return 0;
}
static int TabCtrl_LButtonDown(TabCtrl* s, int x, int y)
{
  if (s->m_hHoverArea != TabHit_Null) {
    if (!TabCtrl_IsSystemButton(s, s->m_hHoverArea)) {// s is tab.
      BOOL bSelect = FALSE;
      if (!s->m_vtrtabs[s->m_hHoverArea].disable) {
        if (s->m_bRemoveTabEnable == TRUE) {
          s->m_ptStartTabDrag = iPOINT(x, y);
          s->m_hPushedArea = s->m_hHoverArea;
        }
        bSelect = (s->m_hCurTab != s->m_hHoverArea);
      }
      TabCtrl_Notify(s, TB_LBUTTONDOWN, s->m_hHoverArea, x, y, 0);
      if (bSelect == TRUE) {
        s->m_hCurTab = (s->m_hHoverArea);
        TabCtrl_Recalc(s, TRUE);
        TabCtrl_Notify(s, TB_TABSELECTED, s->m_hHoverArea, x, y, 0);
      }
    }
    else {
      s->m_hPushedArea = s->m_hHoverArea;
      if (s->m_hPushedArea == TabHit_ButLeftScroll) {
        TabCtrl_StepLeft(s);
        //SetTimer(TIMER_ID_LEFT_SCROLL_CLICK, 300, NULL);
        TabCtrl_Recalc(s, TRUE);
      }
      else if (s->m_hPushedArea == TabHit_ButRightScroll) {
        TabCtrl_StepRight(s);
        //SetTimer(TIMER_ID_RIGHT_SCROLL_CLICK, 300, NULL);
        TabCtrl_Recalc(s, TRUE);
      }
    }
  }
  TabCtrl_Invalidate(s, FALSE);
  return 0;
}
static int TabCtrl_OnLButtonDown(TabCtrl* s, UINT nFlags, int x, int y)
{
  TabCtrl_LButtonDown(s, x, y);
  return 0;
}
static int TabCtrl_OnLButtonDblClk(TabCtrl* s, UINT nFlags, int x, int y)
{
  TabCtrl_LButtonDown(s, x, y);
  if (s->m_hHoverArea != TabHit_Null && TabCtrl_IsSystemButton(s, s->m_hHoverArea) == FALSE) {
    TabCtrl_Notify(s, TB_LBUTTONDBLCLK, s->m_hHoverArea, x, y, 0);
  }
  return 0;
}
static int TabCtrl_OnLButtonUp(TabCtrl* s, UINT nFlags, int x, int y)
{
  if (s->m_hPushedArea != TabHit_Null) {
    BOOL close = (s->m_hPushedArea == TabHit_ButClose && iPtInRect(&s->m_rcCloseButton, x, y) != 0);
    BOOL menu = (s->m_hPushedArea == TabHit_ButMenu && iPtInRect(&s->m_rcMenuButton, x, y) != 0);
    TabCtrl_StopScrolling(s);
    TabCtrl_Invalidate(s, FALSE);
    TabCtrl_StopDragging(s, FALSE);
    if (1) {
      BOOL alive = TRUE;
      if (close == TRUE) {
        s->m_pLifeStatus = &alive;
        s->m_hPushedArea = TabHit_ButClose;
        TabCtrl_Notify(s, TB_CLOSEBUTTONCLICKED, s->m_hPushedArea, x, y, &s->m_rcCloseButton);
        if (alive == TRUE) {
          s->m_hPushedArea = TabHit_Null;
          TabCtrl_Invalidate(s, FALSE);
          s->m_pLifeStatus = NULL;
        }
      }
      else if (menu == TRUE) {
        s->m_pLifeStatus = &alive;
        s->m_hPushedArea = TabHit_ButMenu;
        TabCtrl_Notify(s, TB_MENUBUTTONCLICKED, s->m_hPushedArea, x, y, &s->m_rcMenuButton);
        if (alive == TRUE) {
          s->m_hPushedArea = TabHit_Null;
          TabCtrl_Invalidate(s, FALSE);
          s->m_pLifeStatus = NULL;
        }
      }
    }
  }
  return 0;
}
static int TabCtrl_OnRButtonDown(TabCtrl* s, UINT nFlags, int x, int y)
{
  if (s->m_hHoverArea == TabHit_Null || TabCtrl_IsSystemButton(s, s->m_hHoverArea) == FALSE) {
    TabCtrl_Notify(s, TB_RBUTTONDOWN, s->m_hHoverArea, x, y, 0);
  }
  //CWnd OnRButtonDown(nFlags, x, y);
  return 0;
}
static int TabCtrl_OnRButtonUp(TabCtrl* s, UINT nFlags, int x, int y)
{
  //CWnd OnRButtonUp(nFlags, x, y);
  if (s->m_hHoverArea == TabHit_Null || TabCtrl_IsSystemButton(s, s->m_hHoverArea) == FALSE) {
    TabCtrl_Notify(s, TB_RBUTTONUP, s->m_hHoverArea, x, y, 0);
  }
  return 0;
}
static int TabCtrl_OnKeyDown(TabCtrl* s, UINT keyCode, UINT msgFlag)
{
  if (keyCode == VK_ESCAPE) {
    if (s->m_bTabDrag) {
      TabCtrl_CancelDragging(s);
    }
  }
  return 0;
}
static int TabCtrl_OnTimer(TabCtrl* s, UINT nIDEvent)
{
  switch (nIDEvent) {
  case TIMER_ID_LEFT_SCROLL_CLICK:
    //KillTimer(TIMER_ID_LEFT_SCROLL_CLICK);
    if (s->m_hPushedArea == TabHit_ButLeftScroll) {
      // SetTimer(TIMER_ID_LEFT_SCROLLING, 20, NULL);
    }
    break;
  case TIMER_ID_LEFT_SCROLLING:
    if (s->m_hHoverArea == TabHit_ButLeftScroll) {
      TabCtrl_StepLeft(s);
      TabCtrl_Update(s, 0);
    }
    break;
  case TIMER_ID_RIGHT_SCROLL_CLICK:
    //KillTimer(TIMER_ID_RIGHT_SCROLL_CLICK);
    if (s->m_hPushedArea == TabHit_ButRightScroll) {
      // SetTimer(TIMER_ID_RIGHT_SCROLLING, 20, NULL);
    }
    break;
  case TIMER_ID_RIGHT_SCROLLING:
    if (s->m_hHoverArea == TabHit_ButRightScroll) {
      TabCtrl_StepRight(s);
      TabCtrl_Update(s, 0);
    }
    break;
  }
  //CWnd OnTimer(nIDEvent);
  return 0;
}
static LRESULT TabCtrl_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  TabCtrl* s = (TabCtrl*)c->x;
  IRECT rc = c->rc;
  int hit0 = TabHit_Null, hit1 = TabHit_Null;
  int x = LOWORD(lParam), y = HIWORD(lParam);
  if (WM_CTRLFREE == uMsg) {
    c->ctrlproc = 0;
    return 0;
  }
  if (WM_CTRLINIT == uMsg) {
    TabCtrl_set(s);
    return 0;
  }
  //sub = s->lay0->c;
  switch (uMsg) {
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x += 120;
    mminfo->ptMinTrackSize.y += 120;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_SIZE:
    if (1) {
      //multipane_calcrect(s, rc, 0, x, y, 0, 0);
      TabCtrl_OnSize(s);
      force_redraw(c);
      //sub->rc = c->rc;
      //sub->redraw = 1;
      //sub->ctrlproc(c, sub, hwnd, uMsg, wParam, lParam);
    }
    break;
  case WM_COMMAND:
    if (s) {
    }
    break;
  case WM_LBUTTONUP:
    if (s) {
      TabCtrl_OnLButtonUp(s, 0, x, y);
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
      TabCtrl_OnLButtonDown(s, 0, x, y);
    }
    break;
  case WM_MOUSEMOVE:
    if (iPtInRect(&rc, x, y)) {
      TabCtrl_OnMouseMove(s, 0, x, y);
    }
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      TabCtrl_OnPaint(s, im);
    }
    c->redraw = 0;
    break;
  }
  return 0;
}
// TabCtrlStyle_base.
static int TabCtrlStyle_base_DrawBorder(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect)
{
  COLOR clr = s->m_clrBorderColor;
  imdraw_DrawRectR(im, pclip, *pRect, 0, clr);
  return 0;
}
static int TabCtrlStyle_base_DrawCtrlAreaBack(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect)
{
  IRECT rc = (*pRect);
  if (s->m_Layout == TAB_LAYOUT_TOP) {
    // draw background for tabs area.
    rc.b -= s->GetCtrlAreaPadding(s).b;
    imdraw_FillSolidRect(im, pclip, &rc, s->m_clrCtrlAreaBackColor);
    // draw separator.
    imdraw_DrawLine(im, pclip, pRect->l, rc.b - 1, pRect->r, rc.b - 1, s->m_clrTabBorderColor);
    // draw background for windows area.
    rc.t = rc.b;
    rc.b = pRect->b;
    if (iRectIsNull(&rc) == 0) {
      imdraw_FillSolidRect(im, pclip, &rc, s->m_clrChildWndBackColor);
    }
  }
  else {
    // draw background for windows area.
    rc.b = rc.t + s->GetCtrlAreaPadding(s).t;
    if (iRectIsNull(&rc) == 0) {
      imdraw_FillSolidRect(im, pclip, &rc, s->m_clrChildWndBackColor);
    }
    // draw separator.
    imdraw_DrawLine(im, pclip, pRect->l, rc.b, pRect->r, rc.b, s->m_clrTabBorderColor);
    // draw background for tabs area.
    rc.t = rc.b + 1;
    rc.b = pRect->b;
    imdraw_FillSolidRect(im, pclip, &rc, s->m_clrCtrlAreaBackColor);
  }
  return 0;
}
static int TabCtrlStyle_base_DrawWndsAreaBack(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect)
{
  if (s->m_count == 0 || s->m_hCurTab == TabHit_Null) {
    imdraw_FillSolidRect(im, pclip, pRect, s->m_clrEmptyWndsAreaBackColor);
  }
  else {
    imdraw_FillSolidRect(im, pclip, pRect, s->m_clrTabSelectedBackColor);
  }
  return 0;
}
static int TabCtrlStyle_base_DrawTab(TabCtrl* s, img_t* im, const IRECT* pclip, int hTab)
{
  IRECT rc;
  rc = s->m_vtrtabs[hTab].rc;
  // draw background.
  s->DrawTabBack(s, im, pclip, hTab, &rc);
  // draw image and text;
  s->DrawTabContext(s, im, pclip, hTab, &rc);
  return 0;
}
// Draw image and text.
static int TabCtrlStyle_base_DrawTabContext(TabCtrl* s, img_t* im, const IRECT* pclip, int hTab, IRECT const* pRect)
{
  IRECT rcTabPadding = s->GetTabPadding(s);
  IRECT rc = *(pRect);
  BOOL disable = s->m_vtrtabs[hTab].disable;
  char* text = s->m_vtrtabs[hTab].text;
  int textWidth = font_text_size(s->c->fo, text, -1, 0).w;
  char* pImageList;
  iRectDeflateR(&rc, &rcTabPadding);
  pImageList = disable ? s->m_ImageListDisabled : s->m_ImageList;
  // draw image.
  if (s->m_vtrtabs[hTab].image > -1 && pImageList != NULL && pImageList != NULL) {
    ISIZE szImage = disable ? s->m_szImageDisabled : s->m_szImage;
    int iContentWidth = szImage.w + s->GetTabImageTextGap(s) + textWidth;
    rc.l += (iContentWidth < RCW(&rc) ? (RCW(&rc) - iContentWidth) / 2 : 0);
    s->DrawTabImage(s, im, pclip, hTab, &rc);
    rc.l += szImage.w + s->GetTabImageTextGap(s);
  }
  else {
    rc.l += (textWidth < RCW(&rc) ? (RCW(&rc) - textWidth) / 2 : 0);
  }
  // draw text.
  if (iRectIsNull(&rc) == FALSE) {
    s->DrawTabText(s, im, pclip, hTab, &rc);
  }
  return 0;
}
static int TabCtrlStyle_base_DrawTabBack(TabCtrl* s, img_t* im, const IRECT* pclip, int hTab, IRECT const* pRect)
{
  COLOR clrBorder = s->m_clrTabBorderColor;
  COLOR clrBack = s->m_clrTabSelectedBackColor;
  IRECT rcTabPadding;
  int topMargin;
  int bottomMargin;
  if (s->m_hCurTab != hTab) {
    if (s->m_hHoverArea == hTab && s->m_hPushedArea == TabHit_Null && !s->m_vtrtabs[hTab].disable) {// highlight tab.
      IRECT rc = *(pRect);
      if (s->m_Layout == TAB_LAYOUT_TOP) {
        iRectDeflate(&rc, 2, 1, 1, 1);
        imdraw_DrawHalfRoundFrame(im, pclip, &rc, SIDE_TOP, 1, clrBorder, clrBack);
        iRectDeflate(&rc, 1, RCH(&rc) / 2, 1, 0);
        imdraw_DrawGradient(im, pclip, &rc, FALSE, clrBack, clrBorder);
      }
      else {
        iRectDeflate(&rc, 2, 1, 1, 1);
        imdraw_DrawHalfRoundFrame(im, pclip, &rc, SIDE_BOTTOM, 1, clrBorder, clrBack);
      }
    }
    rcTabPadding = s->GetTabPadding(s);
    topMargin = rcTabPadding.t + 1;
    bottomMargin = rcTabPadding.b + 1;
    if (s->m_Layout == TAB_LAYOUT_TOP) {
      bottomMargin += 1;/*separator*/
    }
    else {
      topMargin += 1;/*separator*/
    }
    if (hTab == 0) {// this is first tab.
      imdraw_DrawLine(im, pclip, pRect->l, pRect->t + topMargin, pRect->l, pRect->b - bottomMargin, clrBorder);
    }
    imdraw_DrawLine(im, pclip, pRect->r, pRect->t + topMargin, pRect->r, pRect->b - bottomMargin, clrBorder);
  }
  else {
    int side = (s->m_Layout == TAB_LAYOUT_TOP ? SIDE_TOP : SIDE_BOTTOM);
    imdraw_DrawHalfRoundFrame(im, pclip, pRect, side, 2, clrBorder, clrBack);
  }
  return 0;
}
static int TabCtrlStyle_base_DrawTabImage(TabCtrl* s, img_t* im, const IRECT* pclip, int hTab, IRECT const* pRect)
{
  char* pImageList;
  ISIZE szImage;
  img_t* im1;
  if (s->m_vtrtabs[hTab].disable == FALSE) {
    pImageList = s->m_ImageList;
    szImage = s->m_szImage;
  }
  else {
    pImageList = s->m_ImageListDisabled;
    szImage = s->m_szImageDisabled;
  }
  if (im1 = sys_find_img(s->c->sys, pImageList)) {
    IRECT rc = iRECT2(pRect->l, (pRect->t + pRect->b - szImage.h + 1) / 2, im1->w, im1->h);
    imdraw_imagelist(im, pclip, rc, im1, s->m_vtrtabs[hTab].image, 0, 0, 0);
  }
  return 0;
}
static int TabCtrlStyle_base_DrawTabText(TabCtrl* s, img_t* im, const IRECT* pclip, int hTab, IRECT const* pRect)
{
  char* text = s->m_vtrtabs[hTab].text;
  COLOR clr = s->GetTabTextColor(s, hTab);
  imdraw_text(im, pclip, *pRect, text, -1, s->c->fo, clr, 0, DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_END_ELLIPSIS);
  return 0;
}
static int TabCtrlStyle_base_DrawButtonFrame(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect, BOOL bHover, BOOL bPushed)
{
  if (bHover != bPushed) {
    imdraw_DrawRectR(im, pclip, *pRect, PixelAlpha_100(_GetSysColor(CLR_HIGHLIGHT), _GetSysColor(CLR_WINDOW), 30), _GetSysColor(CLR_HIGHLIGHT));// it is approximate color (VisualStudio uses some another way).
  }
  else if (bHover == TRUE && bPushed == TRUE) {
    imdraw_DrawRectR(im, pclip, *pRect, PixelAlpha_100(_GetSysColor(CLR_HIGHLIGHT), _GetSysColor(CLR_WINDOW), 22), _GetSysColor(CLR_HIGHLIGHT));// it is approximate color (VisualStudio uses some another way).
  }
  return 0;
}
static int TabCtrlStyle_base_DrawButton(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect, int iFrame, BOOL bHover, BOOL bPushed)
{
  img_t* pImageList;
  s->DrawButtonFrame(s, im, pclip, pRect, bHover, bPushed);
  if (pImageList = sys_find_img(s->c->sys, s->m_SysImageList)) {
    COLOR clrButtonMarkerColor = s->GetButtonMarkerColor(s, bHover, bPushed);
    imdraw_DrawMarker(im, pclip, pRect, pImageList, iFrame, clrButtonMarkerColor);
  }
  return 0;
}
static COLOR TabCtrlStyle_base_GetTabTextColor(TabCtrl* s, int hTab)
{
  return (s->m_vtrtabs[hTab].disable ? _GetSysColor(CLR_GRAYTEXT) : _GetSysColor(CLR_BTNTEXT));
}
static COLOR TabCtrlStyle_base_GetButtonMarkerColor(TabCtrl* s, BOOL bHover, BOOL bPushed)
{
  return (s->m_hCurTab != TabHit_Null ? _GetSysColor(CLR_BTNTEXT) : _GetSysColor(CLR_GRAYTEXT));
}
// ITabCtrlRecalc.
//char* m_ToolTipCtrl;
static int TabCtrlStyle_base_GetBorderWidth(TabCtrl* s)
{
  return 1;
}
static IRECT TabCtrlStyle_base_GetCtrlAreaPadding(TabCtrl* s)
{
  return (s->m_Layout == TAB_LAYOUT_TOP ? iRECT(3, 1, 3, 2/*indent*/) : iRECT(3, 2/*indent*/, 3, 1));
}
static IRECT TabCtrlStyle_base_GetWndsAreaPadding(TabCtrl* s)
{
  return iRECT(0, 0, 0, 0);
}
static IRECT TabCtrlStyle_base_GetTabHorzMargin(TabCtrl* s)
{
  return iRECT(0, 0, 0, 0);
}
static IRECT TabCtrlStyle_base_GetTabPadding(TabCtrl* s)
{
  return iRECT(5, 2 + 1/*border*/, 5, 2 + 1/*border*/);
}
static int TabCtrlStyle_base_GetTabImageTextGap(TabCtrl* s)
{
  return 3;
}
static int TabCtrlStyle_base_GetTabExtraWidth(TabCtrl* s, int hTab)
{
  return 0;
}
static int TabCtrlStyle_base_GetTabMinWidth(TabCtrl* s)
{
  IRECT rcTabPadding = s->GetTabPadding(s);
  return rcTabPadding.l + max(s->m_szImage.w, s->m_szImageDisabled.w) + rcTabPadding.r;
}
static IRECT TabCtrlStyle_base_GetCloseButtonHorzMargin(TabCtrl* s)
{
  return iRECT(2, 0, 2, 0);
}
static IRECT TabCtrlStyle_base_GetMenuButtonHorzMargin(TabCtrl* s)
{
  return iRECT(8, 0, 0, 0);
}
static IRECT TabCtrlStyle_base_GetLeftScrollButtonHorzMargin(TabCtrl* s)
{
  return iRECT(8, 0, 0, 0);
}
static IRECT TabCtrlStyle_base_GetRightScrollButtonHorzMargin(TabCtrl* s)
{
  return iRECT(0, 0, 0, 0);
}
static int TabCtrlStyle_base_CreateToolTip(TabCtrl* s)
{
  return 0;
}
static int TabCtrlStyle_base_HitTestItem(TabCtrl* s, int hTab, BOOL top, int x, int y)
{
  return iPtInRect(&s->m_vtrtabs[hTab].rc, x, y);
}
static int TabCtrlStyle_base_set(TabCtrl* s)
{
  CLASSFUN_INIT(s, TabCtrlStyle_base, DrawBorder);
  CLASSFUN_INIT(s, TabCtrlStyle_base, DrawCtrlAreaBack);
  CLASSFUN_INIT(s, TabCtrlStyle_base, DrawTab);
  CLASSFUN_INIT(s, TabCtrlStyle_base, DrawButton);
  CLASSFUN_INIT(s, TabCtrlStyle_base, DrawWndsAreaBack);
  CLASSFUN_INIT(s, TabCtrlStyle_base, DrawTabBack);
  CLASSFUN_INIT(s, TabCtrlStyle_base, DrawTabContext);
  CLASSFUN_INIT(s, TabCtrlStyle_base, DrawTabImage);
  CLASSFUN_INIT(s, TabCtrlStyle_base, DrawTabText);
  CLASSFUN_INIT(s, TabCtrlStyle_base, DrawButtonFrame);
  CLASSFUN_INIT(s, TabCtrlStyle_base, GetTabTextColor);
  CLASSFUN_INIT(s, TabCtrlStyle_base, GetButtonMarkerColor);
  CLASSFUN_INIT(s, TabCtrlStyle_base, HitTestItem);
  s->m_clrBorderColor = _GetSysColor(CLR_BTNSHADOW);
  s->m_clrTabBorderColor = _GetSysColor(CLR_BTNSHADOW);
  s->m_clrCtrlAreaBackColor = _GetSysColor(CLR_BTNFACE);
  s->m_clrWndsAreaBackColor = _GetSysColor(CLR_BTNFACE);
  s->m_clrTabSelectedBackColor = _GetSysColor(CLR_WINDOW);
  s->m_clrChildWndBackColor = _GetSysColor(CLR_WINDOW);
  s->m_clrEmptyWndsAreaBackColor = _GetSysColor(CLR_APPWORKSPACE);
  CLASSFUN_INIT(s, TabCtrlStyle_base, GetBorderWidth);
  CLASSFUN_INIT(s, TabCtrlStyle_base, GetCtrlAreaPadding);
  CLASSFUN_INIT(s, TabCtrlStyle_base, GetWndsAreaPadding);
  CLASSFUN_INIT(s, TabCtrlStyle_base, GetTabHorzMargin);
  CLASSFUN_INIT(s, TabCtrlStyle_base, GetTabPadding);
  CLASSFUN_INIT(s, TabCtrlStyle_base, GetTabImageTextGap);
  CLASSFUN_INIT(s, TabCtrlStyle_base, GetTabExtraWidth);
  CLASSFUN_INIT(s, TabCtrlStyle_base, GetTabMinWidth);
  CLASSFUN_INIT(s, TabCtrlStyle_base, GetLeftScrollButtonHorzMargin);
  CLASSFUN_INIT(s, TabCtrlStyle_base, GetRightScrollButtonHorzMargin);
  CLASSFUN_INIT(s, TabCtrlStyle_base, GetMenuButtonHorzMargin);
  CLASSFUN_INIT(s, TabCtrlStyle_base, GetCloseButtonHorzMargin);
  CLASSFUN_INIT(s, TabCtrlStyle_base, CreateToolTip);
  return 0;
}
// TabCtrlStyle_VS2003_base.
// ITabCtrlStyle.
static int TabCtrlStyle_VS2003_base_DrawButton(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect, int iFrame, BOOL bHover, BOOL bPushed)
{
  img_t* pImageList;
  IRECT rc = *(pRect);
  s->DrawButtonFrame(s, im, pclip, pRect, bHover, bPushed);
  if (bHover == TRUE && bPushed == TRUE) {
    iRectOff2(&rc, 1, 1);
  }
  if (pImageList = sys_find_img(s->c->sys, s->m_SysImageList)) {
    COLOR clrButtonMarkerColor = s->GetButtonMarkerColor(s, bHover, bPushed);
    imdraw_DrawMarker(im, pclip, &rc, pImageList, iFrame, clrButtonMarkerColor);
  }
  return 0;
}
static int TabCtrlStyle_VS2003_base_DrawButtonFrame(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect, BOOL bHover, BOOL bPushed)
{
  if (bHover == TRUE && bPushed == FALSE) {
    imdraw_border_syscolor(im, pclip, *pRect, BDS_LOW, BDM_ALL);
    //im->DrawEdge((IRECT*)pRect, BDR_RAISEDOUTER, BF_RECT);
  }
  else if (bHover == TRUE && bPushed == TRUE) {
    imdraw_border_syscolor(im, pclip, *pRect, BDS_SUNKEN, BDM_ALL);
    //im->DrawEdge((IRECT*)pRect, BDR_SUNKENINNER, BF_RECT);
  }
  return 0;
}
static COLOR TabCtrlStyle_VS2003_base_GetTabTextColor(TabCtrl* s, int hTab)
{
  if (s->m_hCurTab == hTab)
    if (s->m_bWatchActivityCtrl == FALSE || s->m_bActive == TRUE) {
      return _GetSysColor(CLR_BTNTEXT);
    }
    else {
      return PixelAlpha_100(_GetSysColor(CLR_BTNSHADOW), _RGB(0, 0, 0), 55);
    }
  return (s->m_vtrtabs[hTab].disable == FALSE ?
      PixelAlpha_100(_GetSysColor(CLR_BTNSHADOW), _RGB(0, 0, 0), 55) :// it is approximate color (VS2003 uses some another way).
      TabCtrlStyle_base_GetTabTextColor(s, hTab));
}
static COLOR TabCtrlStyle_VS2003_base_GetButtonMarkerColor(TabCtrl* s, BOOL bHover, BOOL bPushed)
{
  return (s->m_hCurTab != TabHit_Null ? PixelAlpha_100(_GetSysColor(CLR_BTNSHADOW), _RGB(0, 0, 0), 55) : TabCtrlStyle_base_GetButtonMarkerColor(s, bHover, bPushed));
}
static int TabCtrlStyle_VS2003_base_set(TabCtrl* s)
{
  TabCtrlStyle_base_set(s);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2003_base, DrawButton);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2003_base, DrawButtonFrame);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2003_base, GetTabTextColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2003_base, GetButtonMarkerColor);
  s->m_clrCtrlAreaBackColor = PixelAlpha_100(_GetSysColor(CLR_BTNFACE), _GetSysColor(CLR_BTNHIGHLIGHT), 45);// it is approximate color (VS2003 uses some another way).
  return 0;
}
// TabCtrlStyle_VS2003_client.
// ITabCtrlRecalc.
static IRECT TabCtrlStyle_VS2003_client_GetCtrlAreaPadding(TabCtrl* s)
{
  return (s->m_Layout == TAB_LAYOUT_TOP ? iRECT(5, 2, 3, 0) : iRECT(5, 0, 3, 2));
}
static IRECT TabCtrlStyle_VS2003_client_GetWndsAreaPadding(TabCtrl* s)
{
  BOOL bHideSingleTab = (s->m_count == 1 && s->m_bHideSingleTab == TRUE);
  if (bHideSingleTab == FALSE) {
    return (s->m_Layout == TAB_LAYOUT_TOP ? iRECT(2, 3, 2, 2) : iRECT(2, 2, 2, 3));
  }
  else {
    return iRECT(2, 2, 2, 2);
  }
}
static IRECT TabCtrlStyle_VS2003_client_GetTabPadding(TabCtrl* s)
{
  return (s->m_Layout == TAB_LAYOUT_TOP ?
      iRECT(6, 1/*border*/ + 2, 6, 2 + 1/*line*/) : iRECT(6, 1/*line*/ + 2, 6, 2 + 1/*border*/));
}
static int TabCtrlStyle_VS2003_client_DrawCtrlAreaBack(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect)
{
  imdraw_FillSolidRect(im, pclip, pRect, s->m_clrCtrlAreaBackColor);
  if (s->m_Layout == TAB_LAYOUT_TOP) {
    imdraw_DrawLine(im, pclip, pRect->l, pRect->b - 1, pRect->r, pRect->b - 1, _GetSysColor(CLR_BTNHIGHLIGHT));
  }
  else {
    imdraw_DrawLine(im, pclip, pRect->l, pRect->t, pRect->r, pRect->t, _GetSysColor(CLR_BTNHIGHLIGHT));
  }
  imdraw_DrawLine(im, pclip, pRect->l, pRect->t, pRect->l, pRect->b, s->m_clrWndsAreaBackColor);
  imdraw_DrawLine(im, pclip, pRect->r - 1, pRect->t, pRect->r - 1, pRect->b, s->m_clrWndsAreaBackColor);
  return 0;
}
static int TabCtrlStyle_VS2003_client_DrawTabBack(TabCtrl* s, img_t* im, const IRECT* pclip, int hTab, IRECT const* pRect)
{
  BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
  BOOL select = (s->m_hCurTab == hTab);
  IRECT rc = *(pRect);
  if (select == TRUE) {
    COLOR penBorder = s->m_clrTabBorderColor;
    if (top == TRUE) {
      imdraw_FillSolidRect(im, pclip, &rc, s->m_clrWndsAreaBackColor);// fill background.
      // draw left-top border.
      imdraw_DrawLine(im, pclip, rc.l, rc.b - 1, rc.l, rc.t, penBorder);
      imdraw_DrawLine(im, pclip, rc.l, rc.t, rc.r, rc.t, penBorder);
      // draw right border.
      imdraw_DrawLine(im, pclip, rc.r - 1, rc.t + 1, rc.r - 1, rc.b, _GetSysColor(CLR_BTNTEXT));
    }
    else {
      imdraw_FillSolidRect(im, pclip, &rc, s->m_clrWndsAreaBackColor);// fill background.
      // draw left-bottom border.
      imdraw_DrawLine(im, pclip, rc.l, rc.t, rc.l, rc.b - 1, penBorder);
      imdraw_DrawLine(im, pclip, rc.l, rc.b - 1, rc.r, rc.b - 1, penBorder);
      // draw right border.
      imdraw_DrawLine(im, pclip, rc.r - 1, rc.t, rc.r - 1, rc.b - 1, _GetSysColor(CLR_BTNTEXT));
    }
  }
  else {
    IRECT rcTabPadding = s->GetTabPadding(s);
    iRectDeflate(&rc, 0, rcTabPadding.t - 1, 0, rcTabPadding.b - 1);
    imdraw_DrawLine(im, pclip, rc.r, rc.t, rc.r, rc.b, _GetSysColor(CLR_BTNSHADOW));// draw right separator.
  }
  return 0;
}
static int TabCtrlStyle_VS2003_client_DrawWndsAreaBack(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect)
{
  if (s->m_count == 0 || s->m_hCurTab == TabHit_Null) {
    TabCtrlStyle_base_DrawWndsAreaBack(s, im, pclip, pRect);
  }
  else {
    IRECT rc = *(pRect);
    IRECT rcWndsAreaPadding = s->GetWndsAreaPadding(s);
    imdraw_FillSolidRect(im, pclip, &rc, s->m_clrWndsAreaBackColor);
    iRectDeflateR(&rc, &rcWndsAreaPadding);
    iRectInflate2(&rc, 1, 1);
    if (iRectIsNull(&rc) == 0) {
      COLOR color = s->m_clrBorderColor;
      imdraw_DrawRectR(im, pclip, rc, 0, color);// rect around child window.
    }
  }
  return 0;
}
static int TabCtrlStyle_VS2003_client_set(TabCtrl* s)
{
  TabCtrlStyle_VS2003_base_set(s);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2003_client, GetCtrlAreaPadding);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2003_client, GetWndsAreaPadding);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2003_client, GetTabPadding);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2003_client, DrawCtrlAreaBack);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2003_client, DrawTabBack);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2003_client, DrawWndsAreaBack);
  s->m_clrTabBorderColor = _GetSysColor(CLR_BTNHIGHLIGHT);
  return 0;
}
// TabCtrlStyle_VS2003_client_custom1.
// TabCtrlStyle_base.
static int TabCtrlStyle_VS2003_client_custom1_set(TabCtrl* s)
{
  TabCtrlStyle_VS2003_client_set(s);
  CLASSFUN_INIT(s, TabCtrlStyle_base, DrawButton);
  CLASSFUN_INIT(s, TabCtrlStyle_base, DrawButtonFrame);
  return 0;
}
// TabCtrlStyle_VS2003_bars.
// ITabCtrlRecalc.
static int TabCtrlStyle_VS2003_bars_GetBorderWidth(TabCtrl* s)
{
  return 0;
}
static IRECT TabCtrlStyle_VS2003_bars_GetCtrlAreaPadding(TabCtrl* s)
{
  return (s->m_Layout == TAB_LAYOUT_TOP ? iRECT(5, 2, 3, 2) : iRECT(5, 2, 3, 2));
}
static IRECT TabCtrlStyle_VS2003_bars_GetTabPadding(TabCtrl* s)
{
  return (s->m_Layout == TAB_LAYOUT_TOP ?
      iRECT(6, 1/*border*/ + 1, 6, 1 + 1/*line*/) : iRECT(6, 1/*line*/ + 1, 6, 1 + 1/*border*/));
}
static int TabCtrlStyle_VS2003_bars_DrawCtrlAreaBack(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect)
{
  IRECT rc = *(pRect);
  COLOR clrWndsAreaBack = s->m_clrWndsAreaBackColor;
  if (s->m_Layout == TAB_LAYOUT_TOP) {
    imdraw_DrawLine(im, pclip, rc.l + 1, rc.t, rc.r - 1, rc.t, clrWndsAreaBack);
    ++rc.t;
    rc.b -= 3;
    imdraw_FillSolidRect(im, pclip, &rc, s->m_clrCtrlAreaBackColor);
    rc.t = rc.b;
    rc.b = pRect->b;
    imdraw_DrawLine(im, pclip, rc.l + 1, rc.t, rc.r - 1, rc.t, _GetSysColor(CLR_BTNTEXT));
    ++rc.t;
    imdraw_FillSolidRect(im, pclip, &rc, clrWndsAreaBack);
  }
  else {
    rc.t += 3;
    --rc.b;
    imdraw_DrawLine(im, pclip, rc.l + 1, rc.b, rc.r - 1, rc.b, clrWndsAreaBack);
    imdraw_FillSolidRect(im, pclip, &rc, s->m_clrCtrlAreaBackColor);
    --rc.t;
    imdraw_DrawLine(im, pclip, rc.l + 1, rc.t, rc.r - 1, rc.t, _GetSysColor(CLR_BTNTEXT));
    rc.b = rc.t;
    rc.t = pRect->t;
    imdraw_FillSolidRect(im, pclip, &rc, clrWndsAreaBack);
  }
  imdraw_DrawLine(im, pclip, pRect->l, pRect->t, pRect->l, pRect->b, clrWndsAreaBack);
  imdraw_DrawLine(im, pclip, pRect->r - 1, pRect->t, pRect->r - 1, pRect->b, clrWndsAreaBack);
  return 0;
}
static int TabCtrlStyle_VS2003_bars_DrawTabBack(TabCtrl* s, img_t* im, const IRECT* pclip, int hTab, IRECT const* pRect)
{
  BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
  BOOL select = (s->m_hCurTab == hTab);
  IRECT rc = *(pRect);
  if (select == TRUE) {
    COLOR penBorder = (s->m_clrTabBorderColor);
    if (top == TRUE) {
      iRectDeflate(&rc, 1, 1, 1, 0);
      imdraw_FillSolidRect(im, pclip, &rc, s->m_clrWndsAreaBackColor);// fill background.
      // draw left-top border.
      iRectInflate(&rc, 1, 1, 1, 0);
      imdraw_DrawLine(im, pclip, rc.l + 1, rc.t, rc.r - 1, rc.t, penBorder);
      imdraw_DrawLine(im, pclip, rc.r - 1, rc.t, rc.r - 1, rc.b, penBorder);
      // draw right border.
      imdraw_DrawLine(im, pclip, rc.l, rc.t, rc.l, rc.b - 1, _GetSysColor(CLR_BTNHIGHLIGHT));
    }
    else {
      iRectDeflate(&rc, 1, 0, 1, 1);
      imdraw_FillSolidRect(im, pclip, &rc, s->m_clrWndsAreaBackColor);// fill background.
      // draw left-bottom border.
      iRectInflate(&rc, 1, 0, 1, 1);
      imdraw_DrawLine(im, pclip, rc.l + 1, rc.b - 1, rc.r - 1, rc.b - 1, penBorder);
      imdraw_DrawLine(im, pclip, rc.r - 1, rc.b - 1, rc.r - 1, rc.t - 1, penBorder);
      // draw right border.
      imdraw_DrawLine(im, pclip, rc.l, rc.t + 1, rc.l, rc.b, _GetSysColor(CLR_BTNHIGHLIGHT));
    }
  }
  else {
    IRECT rcTabPadding = s->GetTabPadding(s);
    if (top == TRUE) {
      iRectDeflate(&rc, 0, rcTabPadding.t, 0, rcTabPadding.b + 1);
    }
    else {
      iRectDeflate(&rc, 0, rcTabPadding.t + 1, 0, rcTabPadding.b);
    }
    imdraw_DrawLine(im, pclip, rc.r, rc.t, rc.r, rc.b, _GetSysColor(CLR_BTNSHADOW));// draw right separator.
  }
  return 0;
}
static int TabCtrlStyle_VS2003_bars_set(TabCtrl* s)
{
  TabCtrlStyle_VS2003_base_set(s);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2003_bars, GetBorderWidth);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2003_bars, GetCtrlAreaPadding);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2003_bars, GetTabPadding);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2003_bars, DrawCtrlAreaBack);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2003_bars, DrawTabBack);
  s->m_clrTabBorderColor = _GetSysColor(CLR_BTNTEXT);
  return 0;
}
// TabCtrlStyle_VS2003_bars_custom1.
static IRECT TabCtrlStyle_VS2003_bars_custom1_GetCtrlAreaPadding(TabCtrl* s)
{
  return (s->m_Layout == TAB_LAYOUT_TOP ? iRECT(4, 1, 2, 0) : iRECT(4, 0, 2, 1));
}
static IRECT TabCtrlStyle_VS2003_bars_custom1_GetWndsAreaPadding(TabCtrl* s)
{
  BOOL bHideSingleTab = (s->m_count == 1 && s->m_bHideSingleTab == TRUE);
  if (bHideSingleTab == FALSE) {
    return (s->m_Layout == TAB_LAYOUT_TOP ? iRECT(1, 0, 1, 1) : iRECT(1, 1, 1, 0));
  }
  else {
    return iRECT(1, 1, 1, 1);
  }
}
static int TabCtrlStyle_VS2003_bars_custom1_DrawCtrlAreaBack(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect)
{
  IRECT rc = *(pRect);
  COLOR clrBorder = s->m_clrTabBorderColor;
  COLOR clrCtrlAreaBack = s->m_clrCtrlAreaBackColor;
  if (s->m_Layout == TAB_LAYOUT_TOP) {
    --rc.b;
    imdraw_FillSolidRect(im, pclip, &rc, clrCtrlAreaBack);
    imdraw_DrawLine(im, pclip, rc.l, rc.b, rc.r, rc.b, clrBorder);
  }
  else {
    imdraw_DrawLine(im, pclip, rc.l, rc.t, rc.r, rc.t, clrBorder);
    ++rc.t;
    imdraw_FillSolidRect(im, pclip, &rc, clrCtrlAreaBack);
  }
  return 0;
}
static int TabCtrlStyle_VS2003_bars_custom1_DrawWndsAreaBack(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect)
{
  int count = s->m_count;
  if (count == 0 || s->m_hCurTab == TabHit_Null) {
    TabCtrlStyle_base_DrawWndsAreaBack(s, im, pclip, pRect);
  }
  else {
    COLOR clrBorder = s->m_clrTabBorderColor;
    BOOL bHideSingleTab = (count == 1 && s->m_bHideSingleTab == TRUE);
    if (bHideSingleTab == TRUE) {
      imdraw_DrawRectR(im, pclip, *pRect, 0, clrBorder);
    }
    else {
      BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
      COLOR penBorder = (clrBorder);
      if (top == TRUE) {
        imdraw_DrawLine(im, pclip, pRect->l, pRect->t, pRect->l, pRect->b - 1, penBorder);
        imdraw_DrawLine(im, pclip, pRect->l, pRect->b - 1, pRect->r - 1, pRect->b - 1, penBorder);
        imdraw_DrawLine(im, pclip, pRect->r - 1, pRect->b - 1, pRect->r - 1, pRect->t - 1, penBorder);
      }
      else {
        imdraw_DrawLine(im, pclip, pRect->l, pRect->b - 1, pRect->l, pRect->t, penBorder);
        imdraw_DrawLine(im, pclip, pRect->l, pRect->t, pRect->r - 1, pRect->t, penBorder);
        imdraw_DrawLine(im, pclip, pRect->r - 1, pRect->t, pRect->r - 1, pRect->b, penBorder);
      }
    }
  }
  return 0;
}
static int TabCtrlStyle_VS2003_bars_custom1_set(TabCtrl* s)
{
  TabCtrlStyle_VS2003_bars_set(s);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2003_bars_custom1, GetCtrlAreaPadding);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2003_bars_custom1, GetWndsAreaPadding);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2003_bars_custom1, DrawCtrlAreaBack);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2003_bars_custom1, DrawWndsAreaBack);
  CLASSFUN_INIT(s, TabCtrlStyle_base, DrawButton);
  CLASSFUN_INIT(s, TabCtrlStyle_base, DrawButtonFrame);
  s->m_clrTabBorderColor = _GetSysColor(CLR_BTNSHADOW);
  return 0;
}
// TabCtrlStyle_VS2008_client_base.
static int TabCtrlStyle_VS2008_client_base_GetBorderWidth(TabCtrl* s)
{
  return 0;
}
static IRECT TabCtrlStyle_VS2008_client_base_GetCtrlAreaPadding(TabCtrl* s)
{
  return iRECT(0, 0, 3, 0);
}
static IRECT TabCtrlStyle_VS2008_client_base_GetWndsAreaPadding(TabCtrl* s)
{
  BOOL bHideSingleTab = (s->m_count == 1 && s->m_bHideSingleTab == TRUE);
  if (bHideSingleTab == FALSE) {
    return (s->m_Layout == TAB_LAYOUT_TOP ? iRECT(5, 3, 5, 5) : iRECT(5, 5, 5, 3));
  }
  else {
    return iRECT(5, 5, 5, 5);
  }
}
static IRECT TabCtrlStyle_VS2008_client_base_GetTabPadding(TabCtrl* s)
{
  return (s->m_Layout == TAB_LAYOUT_TOP ? iRECT(6, 1/*border*/ + 2, 6, 3 + 2/*indent*/) : iRECT(6, 2/*indent*/ + 2, 6, 3 + 1/*border*/));
}
static int TabCtrlStyle_VS2008_client_base_GetTabExtraWidth(TabCtrl* s, int hTab)
{
  if (hTab != 0) {
    return 0;// it isn't first tab.
  }
  return s->GetSlantWidth(s) - 6;
}
static int TabCtrlStyle_VS2008_client_base_GetTabMinWidth(TabCtrl* s)
{
  return TabCtrlStyle_base_GetTabMinWidth(s) + (s->GetSlantWidth(s) - 6);
}
static COLOR TabCtrlStyle_VS2008_client_base_GetTabBorderColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return 0;
}
static COLOR TabCtrlStyle_VS2008_client_base_GetTabOutlineColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable, BOOL left)
{
  return 0;
}
static COLOR TabCtrlStyle_VS2008_client_base_GetTabGradientLightColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return 0;
}
static COLOR TabCtrlStyle_VS2008_client_base_GetTabGradientDarkColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return 0;
}
static int TabCtrlStyle_VS2008_client_base_DrawCtrlAreaBack(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect)
{
  BOOL allDisabled = (s->m_hCurTab == TabHit_Null);// all tabs are disabled.
  COLOR penBorder = (s->GetTabBorderColor(s, TRUE, 0, allDisabled));
  COLOR penOutline = (s->GetTabOutlineColor(s, TRUE, FALSE, allDisabled, TRUE));
  imdraw_FillSolidRect(im, pclip, pRect, s->m_clrCtrlAreaBackColor);
  if (s->m_Layout == TAB_LAYOUT_TOP) {
    if (allDisabled == FALSE) {
      // draw border
      imdraw_DrawLine(im, pclip, pRect->l + 1, pRect->b - 1, pRect->l + 2, pRect->b - 2, penBorder);
      imdraw_DrawLine(im, pclip, pRect->l + 2, pRect->b - 2, pRect->r - 3, pRect->b - 2, penBorder);
      imdraw_DrawLine(im, pclip, pRect->r - 3, pRect->b - 2, pRect->r - 1, pRect->b, penBorder);
      // draw outline.
      imdraw_DrawLine(im, pclip, pRect->l + 2, pRect->b - 1, pRect->r - 2, pRect->b - 1, penOutline);
    }
    else {
      // draw border.
      imdraw_DrawLine(im, pclip, pRect->l, pRect->b - 2, pRect->r, pRect->b - 2, penBorder);
      // draw outline.
      imdraw_DrawLine(im, pclip, pRect->l, pRect->b - 1, pRect->r, pRect->b - 1, penOutline);
    }
  }
  else {
    if (allDisabled == FALSE) {
      // draw border.
      imdraw_DrawLine(im, pclip, pRect->l + 1, pRect->t, pRect->l + 2, pRect->t + 1, penBorder);
      imdraw_DrawLine(im, pclip, pRect->l + 2, pRect->t + 1, pRect->r - 3, pRect->t + 1, penBorder);
      imdraw_DrawLine(im, pclip, pRect->r - 3, pRect->t + 1, pRect->r - 1, pRect->t - 1, penBorder);
      // draw outline.
      imdraw_DrawLine(im, pclip, pRect->l + 2, pRect->t, pRect->r - 2, pRect->t, penOutline);
    }
    else {
      // draw border.
      imdraw_DrawLine(im, pclip, pRect->l, pRect->t + 1, pRect->r, pRect->t + 1, penBorder);
      // draw outline.
      imdraw_DrawLine(im, pclip, pRect->l, pRect->t, pRect->r, pRect->t, penOutline);
    }
  }
  return 0;
}
static int TabCtrlStyle_VS2008_client_base_DrawTabBack(TabCtrl* s, img_t* im, const IRECT* pclip, int hTab, IRECT const* pRect)
{
  BOOL selected = (s->m_hCurTab == hTab);
  BOOL hover = (s->m_hHoverArea == hTab) && (s->m_hPushedArea == TabHit_Null);
  BOOL disabled = s->m_vtrtabs[hTab].disable;
  BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
  COLOR clrBorder = s->GetTabBorderColor(s, selected, 0, disabled);
  COLOR clrBackLight = s->GetTabGradientLightColor(s, selected, hover, disabled);
  COLOR clrBackDark = s->GetTabGradientDarkColor(s, selected, hover, disabled);
  COLOR clrOutlineLeft = s->GetTabOutlineColor(s, selected, hover, disabled, TRUE);
  COLOR clrOutlineRight = s->GetTabOutlineColor(s, selected, hover, disabled, FALSE);
  COLOR penBorder = (clrBorder);
  COLOR penOutlineLeft = (clrOutlineLeft);
  COLOR penOutlineRight = (clrOutlineRight);
  IPOINT pts[8];
  IRECT rcFill;
  int len = 8;
  s->GetTabOutline(s, hTab, pRect, top, pts, &rcFill);
  // draw back.
  if (top == TRUE) {
    imdrawaa_poly_gradient_hv_i(im, &rcFill, pts, &len, 1, rcFill, 1, clrBackLight, clrBackDark, 0);
    //imdraw_DrawGradient(im, pclip, &rcFill, FALSE, clrBackLight, clrBackDark);
  }
  else {
    imdrawaa_poly_gradient_hv_i(im, &rcFill, pts, &len, 1, rcFill, 1, clrBackDark, clrBackLight, 0);
    //imdraw_DrawGradient(im, pclip, &rcFill, FALSE, clrBackDark, clrBackLight);
  }
  // left outline.
  imdraw_DrawLine(im, pclip, pts[1].x + 1, pts[1].y, pts[2].x + 1, pts[2].y, penOutlineLeft);
  imdraw_DrawLine(im, pclip, pts[2].x + 1, pts[2].y, pts[3].x + 1, pts[3].y, penOutlineLeft);
  // right outline.
  imdraw_DrawLine(im, pclip, pts[4].x - 1, pts[4].y, pts[5].x - 1, pts[5].y, penOutlineRight);
  imdraw_DrawLine(im, pclip, pts[5].x - 1, pts[5].y, pts[6].x - 1, pts[6].y, penOutlineRight);
  // draw border.
  imdraw_DrawLine(im, pclip, pts[1].x, pts[1].y, pts[2].x, pts[2].y, penBorder);
  imdraw_DrawLine(im, pclip, pts[2].x, pts[2].y, pts[3].x, pts[3].y, penBorder);
  imdraw_DrawLine(im, pclip, pts[3].x, pts[3].y, pts[4].x, pts[4].y, penBorder);
  imdraw_DrawLine(im, pclip, pts[4].x, pts[4].y, pts[5].x, pts[5].y, penBorder);
  imdraw_DrawLine(im, pclip, pts[5].x, pts[5].y, pts[6].x, pts[6].y, penBorder);
  return 0;
}
static int TabCtrlStyle_VS2008_client_base_DrawTabContext(TabCtrl* s, img_t* im, const IRECT* pclip, int hTab, IRECT const* pRect)
{
  IRECT rc = *(pRect);
  if (s->m_hCurTab != hTab) {
    if (s->m_Layout == TAB_LAYOUT_TOP) {
      rc.t += 2;
    }
    else {
      rc.b -= 2;
    }
  }
  rc.l += s->GetTabExtraWidth(s, hTab);
  TabCtrlStyle_base_DrawTabContext(s, im, pclip, hTab, &rc);
  return 0;
}
static int TabCtrlStyle_VS2008_client_base_DrawWndsAreaBack(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect)
{
  int count = s->m_count;
  if (count == 0 || s->m_hCurTab == TabHit_Null) {
    TabCtrlStyle_base_DrawWndsAreaBack(s, im, pclip, pRect);
  }
  else {
    BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
    BOOL bHideSingleTab = (count == 1 && s->m_bHideSingleTab == TRUE);
    COLOR clrBorder = s->GetTabBorderColor(s, TRUE, 0, FALSE);
    COLOR clrOutline = s->GetTabOutlineColor(s, TRUE, FALSE, FALSE, TRUE);
    COLOR clrBack = s->GetTabGradientDarkColor(s, TRUE, FALSE, FALSE);
    COLOR clrBackOut = s->m_clrWndsAreaBackColor;
    COLOR penBorder = (clrBorder);
    COLOR penOutline = (clrOutline);
    COLOR penBack = (clrBack);
    IRECT rc = *(pRect);
    if (bHideSingleTab == TRUE) {
      // outside border.
      imdraw_DrawBeveledRect(im, pclip, &rc, 2, penBorder);
      // left-top corner.
      imdraw_SetPixel(im, pclip, rc.l, rc.t, clrBackOut);
      imdraw_SetPixel(im, pclip, rc.l + 1, rc.t, clrBackOut);
      imdraw_SetPixel(im, pclip, rc.l, rc.t + 1, clrBackOut);
      // right-top corner.
      imdraw_SetPixel(im, pclip, rc.r - 1, rc.t, clrBackOut);
      imdraw_SetPixel(im, pclip, rc.r - 2, rc.t, clrBackOut);
      imdraw_SetPixel(im, pclip, rc.r - 1, rc.t + 1, clrBackOut);
      // left-bottom corner.
      imdraw_SetPixel(im, pclip, rc.l, rc.b - 1, clrBackOut);
      imdraw_SetPixel(im, pclip, rc.l + 1, rc.b - 1, clrBackOut);
      imdraw_SetPixel(im, pclip, rc.l, rc.b - 2, clrBackOut);
      // right-bottom corner.
      imdraw_SetPixel(im, pclip, rc.r - 1, rc.b - 1, clrBackOut);
      imdraw_SetPixel(im, pclip, rc.r - 2, rc.b - 1, clrBackOut);
      imdraw_SetPixel(im, pclip, rc.r - 1, rc.b - 2, clrBackOut);
      // inside border.
      iRectDeflate2(&rc, 4, 4);
      if (iRectIsNull(&rc) == 0) {
        imdraw_DrawRectR(im, pclip, rc, 0, penBorder);
      }
    }
    else if (top == TRUE) {
      // outside border.
      imdraw_DrawLine(im, pclip, rc.l, rc.t, rc.l, rc.b - 3, penBorder);
      imdraw_DrawLine(im, pclip, rc.l, rc.b - 3, rc.l + 2, rc.b - 1, penBorder);
      imdraw_DrawLine(im, pclip, rc.l + 2, rc.b - 1, rc.r - 3, rc.b - 1, penBorder);
      imdraw_DrawLine(im, pclip, rc.r - 3, rc.b - 1, rc.r - 1, rc.b - 3, penBorder);
      imdraw_DrawLine(im, pclip, rc.r - 1, rc.b - 3, rc.r - 1, rc.t - 1, penBorder);
      // left-bottom corner.
      imdraw_SetPixel(im, pclip, rc.l, rc.b - 1, clrBackOut);
      imdraw_SetPixel(im, pclip, rc.l + 1, rc.b - 1, clrBackOut);
      imdraw_SetPixel(im, pclip, rc.l, rc.b - 2, clrBackOut);
      // right-bottom corner.
      imdraw_SetPixel(im, pclip, rc.r - 1, rc.b - 1, clrBackOut);
      imdraw_SetPixel(im, pclip, rc.r - 2, rc.b - 1, clrBackOut);
      imdraw_SetPixel(im, pclip, rc.r - 1, rc.b - 2, clrBackOut);
      // inside border.
      iRectDeflate(&rc, 4, 2, 4, 4);
      if (iRectIsNull(&rc) == 0) {
        imdraw_DrawRectR(im, pclip, rc, 0, penBorder);
      }
    }
    else {
      // outside border.
      imdraw_DrawLine(im, pclip, rc.l, rc.b - 1, rc.l, rc.t + 2, penBorder);
      imdraw_DrawLine(im, pclip, rc.l, rc.t + 2, rc.l + 2, rc.t, penBorder);
      imdraw_DrawLine(im, pclip, rc.l + 2, rc.t, rc.r - 3, rc.t, penBorder);
      imdraw_DrawLine(im, pclip, rc.r - 3, rc.t, rc.r - 1, rc.t + 2, penBorder);
      imdraw_DrawLine(im, pclip, rc.r - 1, rc.t + 2, rc.r - 1, rc.b, penBorder);
      // left-bottom corner.
      imdraw_SetPixel(im, pclip, rc.l, rc.t, clrBackOut);
      imdraw_SetPixel(im, pclip, rc.l + 1, rc.t, clrBackOut);
      imdraw_SetPixel(im, pclip, rc.l, rc.t + 1, clrBackOut);
      // right-bottom corner.
      imdraw_SetPixel(im, pclip, rc.r - 1, rc.t, clrBackOut);
      imdraw_SetPixel(im, pclip, rc.r - 2, rc.t, clrBackOut);
      imdraw_SetPixel(im, pclip, rc.r - 1, rc.t + 1, clrBackOut);
      // inside border.
      iRectDeflate(&rc, 4, 4, 4, 2);
      if (iRectIsNull(&rc) == 0) {
        imdraw_DrawRectR(im, pclip, rc, 0, penBorder);
      }
    }
    // draw back.
    iRectInflate2(&rc, 1, 1);
    if (iRectIsNull(&rc) == 0) {
      imdraw_DrawRectR(im, pclip, rc, 0, penBack);
    }
    iRectInflate2(&rc, 1, 1);
    if (iRectIsNull(&rc) == 0) {
      imdraw_DrawRectR(im, pclip, rc, 0, penBack);
    }
    // draw outline;
    if (bHideSingleTab == TRUE) {
      iRectInflate2(&rc, 1, 1);
      if (iRectIsNull(&rc) == 0) {
        imdraw_DrawLine(im, pclip, rc.l, rc.t + 1, rc.l, rc.b - 1, penOutline);
        imdraw_DrawLine(im, pclip, rc.l, rc.b - 1, rc.l + 1, rc.b - 1, penOutline);
        imdraw_DrawLine(im, pclip, rc.l + 1, rc.b - 1, rc.r - 1, rc.b - 1, penOutline);
        imdraw_DrawLine(im, pclip, rc.r - 1, rc.b - 2, rc.r - 1, rc.t, penOutline);
        imdraw_DrawLine(im, pclip, rc.r - 2, rc.t, rc.l, rc.t, penOutline);
      }
    }
    else if (top == TRUE) {
      iRectInflate(&rc, 1, 0, 1, 1);
      if (iRectIsNull(&rc) == 0) {
        imdraw_DrawLine(im, pclip, rc.l, rc.t, rc.l, rc.b - 1, penOutline);
        imdraw_DrawLine(im, pclip, rc.l + 1, rc.b - 1, rc.r - 1, rc.b - 1, penOutline);
        imdraw_DrawLine(im, pclip, rc.r - 1, rc.b - 2, rc.r - 1, rc.t - 1, penOutline);
      }
    }
    else {
      iRectInflate(&rc, 1, 1, 1, 0);
      if (iRectIsNull(&rc) == 0) {
        imdraw_DrawLine(im, pclip, rc.l, rc.b - 1, rc.l, rc.t, penOutline);
        imdraw_DrawLine(im, pclip, rc.l + 1, rc.t, rc.r - 1, rc.t, penOutline);
        imdraw_DrawLine(im, pclip, rc.r - 1, rc.t + 1, rc.r - 1, rc.b, penOutline);
      }
    }
  }
  return 0;
}
static int TabCtrlStyle_VS2008_client_base_HitTestItem(TabCtrl* s, int hTab, BOOL top, int x, int y)
{
  IRECT rc;
  IPOINT pts[8];
  rc = s->m_vtrtabs[hTab].rc;
  s->GetTabOutline(s, hTab, &rc, top, pts, NULL);
  return iPtInPolygon(pts, sizeof(pts) / sizeof(IPOINT), x, y);
}
static int TabCtrlStyle_VS2008_client_base_GetTabOutline(TabCtrl* s, int hTab, IRECT const* rect, BOOL top, IPOINT pts[8]/*out*/, IRECT* rcFill/*out*/)
{
  BOOL first = ((hTab) == 0);
  BOOL selected = (s->m_hCurTab == hTab);
  int iSlantWidth = s->GetSlantWidth(s);
  IRECT rc = *(rect);
  if (selected == FALSE) {
    rc.t += 2;
    rc.b -= 2;
    iSlantWidth -= 4;
  }
  if (first == FALSE) {
    rc.l -= (iSlantWidth - 6);
  }
  else if (selected == FALSE) {
    rc.l += 4;
  }
  if (top == TRUE) {
    if (selected == FALSE) {
      pts[0].x = rc.l;
      pts[0].y = rc.b;
      pts[1].x = rc.l + 1;
      pts[1].y = rc.b - 1;
    }
    else {
      pts[0].x = rc.l + 2;
      pts[0].y = rc.b;
      pts[1].x = rc.l + 2;
      pts[1].y = rc.b - 2;
    }
    pts[2].x = rc.l + iSlantWidth - 2;
    pts[2].y = rc.t + 2;
    pts[3].x = rc.l + iSlantWidth + 3;
    pts[3].y = rc.t;
    pts[4].x = rc.r - 3;
    pts[4].y = rc.t;
    pts[5].x = rc.r - 1;
    pts[5].y = rc.t + 2;
    if (selected == FALSE) {
      pts[6].x = rc.r - 1;
      pts[6].y = rc.b;
    }
    else {
      pts[6].x = rc.r - 1;
      pts[6].y = rc.b - 1;
    }
    pts[7].x = rc.r - 1;
    pts[7].y = rc.b;
  }
  else {
    if (selected == FALSE) {
      pts[0].x = rc.l;
      pts[0].y = rc.t;
      pts[1].x = rc.l + 1;
      pts[1].y = rc.t;
    }
    else {
      pts[0].x = rc.l + 2;
      pts[0].y = rc.t;
      pts[1].x = rc.l + 2;
      pts[1].y = rc.t + 1;
    }
    pts[2].x = rc.l + iSlantWidth - 2;
    pts[2].y = rc.b - 3;
    pts[3].x = rc.l + iSlantWidth + 3;
    pts[3].y = rc.b - 1;
    pts[4].x = rc.r - 3;
    pts[4].y = rc.b - 1;
    pts[5].x = rc.r - 1;
    pts[5].y = rc.b - 3;
    if (selected == FALSE) {
      pts[6].x = rc.r - 1;
      pts[6].y = rc.t - 1;
    }
    else {
      pts[6].x = rc.r - 1;
      pts[6].y = rc.t;
    }
    pts[7].x = rc.r - 1;
    pts[7].y = rc.t;
  }
  if (rcFill != NULL) {
    *rcFill = rc;
    if (selected == FALSE && first == FALSE) {
      rcFill->l = rect->l;
    }
  }
  return 0;
}
static int TabCtrlStyle_VS2008_client_base_GetSlantWidth(TabCtrl* s)
{
  IRECT rc = s->m_rcTabs;
  return RCH(&rc);
}
static int TabCtrlStyle_VS2008_client_base_set(TabCtrl* s)
{
  TabCtrlStyle_base_set(s);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_base, GetBorderWidth);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_base, GetCtrlAreaPadding);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_base, GetWndsAreaPadding);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_base, GetTabPadding);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_base, GetTabExtraWidth);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_base, GetTabMinWidth);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_base, GetTabBorderColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_base, GetTabOutlineColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_base, GetTabGradientLightColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_base, GetTabGradientDarkColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_base, DrawCtrlAreaBack);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_base, DrawTabBack);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_base, DrawTabContext);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_base, DrawWndsAreaBack);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_base, HitTestItem);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_base, GetTabOutline);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_base, GetSlantWidth);
  s->m_bIsStraightOrder = FALSE;
  return 0;
}
// TabCtrlStyle_VS2008_client_classic.
static COLOR TabCtrlStyle_VS2008_client_classic_GetTabBorderColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return _GetSysColor(CLR_BTNSHADOW);
}
static COLOR TabCtrlStyle_VS2008_client_classic_GetTabOutlineColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable, BOOL left)
{
  if (active == TRUE) {
    return _GetSysColor(CLR_WINDOW);
  }
  if (hover == TRUE && disable == FALSE) {
    return _GetSysColor(CLR_WINDOW);
  }
  if (disable == TRUE) {
    return _GetSysColor(CLR_BTNFACE);
  }
  return _GetSysColor(CLR_BTNFACE);
}
static COLOR TabCtrlStyle_VS2008_client_classic_GetTabGradientLightColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  if (active == TRUE) {
    return _GetSysColor(CLR_BTNHIGHLIGHT);
  }
  if (hover == TRUE && disable == FALSE) {
    return _GetSysColor(CLR_BTNHIGHLIGHT);
  }
  return _GetSysColor(CLR_BTNFACE);
}
static COLOR TabCtrlStyle_VS2008_client_classic_GetTabGradientDarkColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  if (active == TRUE) {
    return _GetSysColor(CLR_BTNFACE);
  }
  if (hover == TRUE && disable == FALSE) {
    return _GetSysColor(CLR_BTNHIGHLIGHT);
  }
  return _GetSysColor(CLR_BTNFACE);
}
static int TabCtrlStyle_VS2008_client_classic_set(TabCtrl* s)
{
  TabCtrlStyle_VS2008_client_base_set(s);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_classic, GetTabBorderColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_classic, GetTabOutlineColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_classic, GetTabGradientLightColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_classic, GetTabGradientDarkColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_classic, GetTabBorderColor);
  return 0;
}
// TabCtrlStyle_VS2008_client_blue.
static COLOR TabCtrlStyle_VS2008_client_blue_GetTabBorderColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  if (active == TRUE) {
    return _RGB(105, 161, 191);
  }
  if (disable == TRUE) {
    return _RGB(145, 150, 162);
  }
  return _RGB(145, 150, 162);
}
static COLOR TabCtrlStyle_VS2008_client_blue_GetTabOutlineColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable, BOOL left)
{
  if (active == TRUE) {
    return _RGB(255, 255, 255);
  }
  if (hover == TRUE && disable == FALSE) {
    return _RGB(255, 255, 255);
  }
  if (disable == TRUE) {
    return _RGB(140, 171, 204);
  }
  return (left == TRUE ? _RGB(242, 250, 255) : _RGB(140, 171, 204));
}
static COLOR TabCtrlStyle_VS2008_client_blue_GetTabGradientLightColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  if (active == TRUE) {
    return _RGB(252, 253, 254);
  }
  if (hover == TRUE && disable == FALSE) {
    return _RGB(247, 252, 254);
  }
  if (disable == TRUE) {
    return _RGB(207, 223, 237);
  }
  return _RGB(236, 245, 252);
}
static COLOR TabCtrlStyle_VS2008_client_blue_GetTabGradientDarkColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  if (active == TRUE) {
    return _RGB(210, 230, 250);
  }
  if (hover == TRUE && disable == FALSE) {
    return _RGB(129, 208, 241);
  }
  if (disable == TRUE) {
    return _RGB(207, 223, 237);
  }
  return _RGB(152, 180, 210);
}
static int TabCtrlStyle_VS2008_client_blue_set(TabCtrl* s)
{
  TabCtrlStyle_VS2008_client_base_set(s);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_blue, GetTabBorderColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_blue, GetTabOutlineColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_blue, GetTabGradientLightColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_blue, GetTabGradientDarkColor);
  return 0;
}
// TabCtrlStyle_VS2008_client_silver.
static COLOR TabCtrlStyle_VS2008_client_silver_GetTabBorderColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  if (active == TRUE) {
    return _RGB(147, 145, 176);
  }
  if (disable == TRUE) {
    return _RGB(157, 157, 161);
  }
  return _RGB(157, 157, 161);
}
static COLOR TabCtrlStyle_VS2008_client_silver_GetTabOutlineColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable, BOOL left)
{
  if (active == TRUE) {
    return _RGB(255, 255, 255);
  }
  if (hover == TRUE && disable == FALSE) {
    return _RGB(255, 255, 255);
  }
  if (disable == TRUE) {
    return _RGB(172, 171, 196);
  }
  return (left == TRUE ? _RGB(255, 255, 255) : _RGB(172, 171, 196));
}
static COLOR TabCtrlStyle_VS2008_client_silver_GetTabGradientLightColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  if (active == TRUE) {
    return _RGB(247, 247, 253);
  }
  if (hover == TRUE && disable == FALSE) {
    return _RGB(247, 247, 253);
  }
  if (disable == TRUE) {
    return _RGB(203, 205, 217);
  }
  return _RGB(234, 235, 240);
}
static COLOR TabCtrlStyle_VS2008_client_silver_GetTabGradientDarkColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  if (active == TRUE) {
    return _RGB(225, 226, 236);
  }
  if (hover == TRUE && disable == FALSE) {
    return _RGB(182, 185, 201);
  }
  if (disable == TRUE) {
    return _RGB(203, 205, 217);
  }
  return _RGB(172, 171, 196);
}
static int TabCtrlStyle_VS2008_client_silver_DrawButtonFrame(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect, BOOL bHover, BOOL bPushed)
{
  if (bHover != bPushed) {
    imdraw_DrawRectR(im, pclip, *pRect, _RGB(255, 227, 173), _RGB(74, 73, 107));
  }
  else if (bHover == TRUE && bPushed == TRUE) {
    imdraw_DrawRectR(im, pclip, *pRect, _RGB(255, 182, 115), _RGB(74, 73, 107));
  }
  return 0;
}
static int TabCtrlStyle_VS2008_client_silver_set(TabCtrl* s)
{
  TabCtrlStyle_VS2008_client_base_set(s);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_silver, GetTabBorderColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_silver, GetTabOutlineColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_silver, GetTabGradientLightColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_silver, GetTabGradientDarkColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_silver, DrawButtonFrame);
  return 0;
}
// TabCtrlStyle_VS2008_client_olive.
static COLOR TabCtrlStyle_VS2008_client_olive_GetTabBorderColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  if (active == TRUE) {
    return _RGB(147, 160, 112);
  }
  if (disable == TRUE) {
    return _RGB(172, 168, 153);
  }
  return _RGB(172, 168, 153);
}
static COLOR TabCtrlStyle_VS2008_client_olive_GetTabOutlineColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable, BOOL left)
{
  if (active == TRUE) {
    return _RGB(255, 255, 255);
  }
  if (hover == TRUE && disable == FALSE) {
    return _RGB(255, 255, 255);
  }
  if (disable == TRUE) {
    return _RGB(165, 179, 133);
  }
  return (left == TRUE ? _RGB(255, 255, 255) : _RGB(165, 179, 133));
}
static COLOR TabCtrlStyle_VS2008_client_olive_GetTabGradientLightColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  if (active == TRUE) {
    return _RGB(250, 251, 247);
  }
  if (hover == TRUE && disable == FALSE) {
    return _RGB(245, 247, 240);
  }
  if (disable == TRUE) {
    return _RGB(208, 217, 181);
  }
  return _RGB(241, 244, 233);
}
static COLOR TabCtrlStyle_VS2008_client_olive_GetTabGradientDarkColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  if (active == TRUE) {
    return _RGB(173, 190, 126);
  }
  if (hover == TRUE && disable == FALSE) {
    return _RGB(197, 210, 165);
  }
  if (disable == TRUE) {
    return _RGB(208, 217, 181);
  }
  return _RGB(165, 179, 133);
}
static int TabCtrlStyle_VS2008_client_olive_DrawButtonFrame(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect, BOOL bHover, BOOL bPushed)
{
  if (bHover != bPushed) {
    imdraw_DrawRectR(im, pclip, *pRect, _RGB(255, 227, 173), _RGB(118, 128, 95));
  }
  else if (bHover == TRUE && bPushed == TRUE) {
    imdraw_DrawRectR(im, pclip, *pRect, _RGB(255, 182, 115), _RGB(118, 128, 95));
  }
  return 0;
}
static int TabCtrlStyle_VS2008_client_olive_set(TabCtrl* s)
{
  TabCtrlStyle_VS2008_client_base_set(s);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_olive, GetTabBorderColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_olive, GetTabOutlineColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_olive, GetTabGradientLightColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_olive, GetTabGradientDarkColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_client_olive, DrawButtonFrame);
  return 0;
}
// TabCtrlStyle_VS2008_bars_base.
// ITabCtrlStyle.
static int TabCtrlStyle_VS2008_bars_base_GetBorderWidth(TabCtrl* s)
{
  return 0;
}
static IRECT TabCtrlStyle_VS2008_bars_base_GetCtrlAreaPadding(TabCtrl* s)
{
  return (s->m_Layout == TAB_LAYOUT_TOP ? iRECT(0, 1, 0, 2) : iRECT(0, 2, 0, 1));
}
static IRECT TabCtrlStyle_VS2008_bars_base_GetTabPadding(TabCtrl* s)
{
  return (s->m_Layout == TAB_LAYOUT_TOP ?
      iRECT(1/*border*/ + 2, 1 + 1/*border*/ + 2/*indent*/, 3, 1/*border*/ + 1) :
      iRECT(1/*border*/ + 2, 1/*border*/ + 1, 3, 1 + 1/*border*/ + 2/*indent*/));
}
static COLOR TabCtrlStyle_VS2008_bars_base_GetTabBorderColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return 0;
}
static COLOR TabCtrlStyle_VS2008_bars_base_GetTabGradientLightColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return 0;
}
static COLOR TabCtrlStyle_VS2008_bars_base_GetTabGradientDarkColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return 0;
}
static int TabCtrlStyle_VS2008_bars_base_DrawCtrlAreaBack(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect)
{
  COLOR clrBorder;
  imdraw_FillSolidRect(im, pclip, pRect, s->m_clrCtrlAreaBackColor);
  clrBorder = s->GetTabBorderColor(s, 0, FALSE, 0);
  if (s->m_Layout == TAB_LAYOUT_TOP) {
    imdraw_DrawLine(im, pclip, pRect->l, pRect->b - 3, pRect->r, pRect->b - 3, clrBorder);
  }
  else {
    imdraw_DrawLine(im, pclip, pRect->l, pRect->t + 2, pRect->r, pRect->t + 2, clrBorder);
  }
  return 0;
}
static int TabCtrlStyle_VS2008_bars_base_DrawTabBack(TabCtrl* s, img_t* im, const IRECT* pclip, int hTab, IRECT const* pRect)
{
  BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
  BOOL select = (s->m_hCurTab == hTab);
  int count = s->m_count;
  COLOR clrBorder = s->GetTabBorderColor(s, FALSE, FALSE, FALSE);
  COLOR penBorder = (clrBorder);
  if (select == TRUE) {
    BOOL firstTab = (0 == hTab);
    COLOR clrBackSelected = s->m_clrTabSelectedBackColor;
    // draw back.
    imdraw_FillSolidRect(im, pclip, pRect, clrBackSelected);// fill background.
    // draw tab border.
    if (top == TRUE) {
      imdraw_DrawLine(im, pclip, pRect->l, pRect->b - (firstTab == TRUE ? 1 : 2), pRect->l, pRect->t, penBorder);
      imdraw_DrawLine(im, pclip, pRect->l, pRect->t, pRect->r - 1, pRect->t, penBorder);
      imdraw_DrawLine(im, pclip, pRect->r - 1, pRect->t, pRect->r - 1, pRect->b - 1, penBorder);
    }
    else {
      imdraw_DrawLine(im, pclip, pRect->l, pRect->t + (firstTab == TRUE ? 0 : 1), pRect->l, pRect->b - 1, penBorder);
      imdraw_DrawLine(im, pclip, pRect->l, pRect->b - 1, pRect->r - 1, pRect->b - 1, penBorder);
      imdraw_DrawLine(im, pclip, pRect->r - 1, pRect->b - 1, pRect->r - 1, pRect->t, penBorder);
    }
  }
  else {// tab isn't selected.
    BOOL hover = (s->m_hHoverArea == hTab) && (s->m_hPushedArea == TabHit_Null);
    BOOL disabled = s->m_vtrtabs[hTab].disable;
    COLOR clrBorderHover = s->GetTabBorderColor(s, FALSE, TRUE, FALSE);
    COLOR clrBackLight = s->GetTabGradientLightColor(s, FALSE, hover, disabled);
    COLOR clrBackDark = s->GetTabGradientDarkColor(s, FALSE, hover, disabled);
    int tabSel = s->m_hCurTab;
    int cmpRes = (tabSel >= 0 ? CV_CMP(hTab, tabSel) : -1);
    IRECT rc = *(pRect);
    if (top == TRUE) {
      iRectDeflate(&rc, 0, 2, 0, 1);
      // draw back.
      imdraw_DrawGradient(im, pclip, &rc, FALSE, clrBackLight, clrBackDark);
      if (cmpRes < 0) {// before selected tab or there isn't selected tab.
        // draw tab border.
        if (hover == TRUE && disabled == FALSE) {
          imdraw_DrawLine(im, pclip, rc.l, rc.b - 1, rc.l, rc.t, clrBorderHover);
          imdraw_DrawLine(im, pclip, rc.l, rc.t, rc.r, rc.t, clrBorderHover);
        }
        else {
          imdraw_DrawLine(im, pclip, rc.l, rc.b - 1, rc.l, rc.t, penBorder);
          imdraw_DrawLine(im, pclip, rc.l, rc.t, rc.r, rc.t, penBorder);
        }
        // draw outline.
        imdraw_DrawLine(im, pclip, rc.l + 1, rc.t + 1, rc.l + 1, rc.b, clrBackLight);
      }
      else {// after selected tab.
        // draw tab border.
        if (hover == TRUE && disabled == FALSE) {
          imdraw_DrawLine(im, pclip, rc.l, rc.t, rc.r - 1, rc.t, clrBorderHover);
          imdraw_DrawLine(im, pclip, rc.r - 1, rc.t, rc.r - 1, rc.b, clrBorderHover);
        }
        else {
          imdraw_DrawLine(im, pclip, rc.l, rc.t, rc.r - 1, rc.t, penBorder);
          imdraw_DrawLine(im, pclip, rc.r - 1, rc.t, rc.r - 1, rc.b, penBorder);
        }
        // draw outline.
        imdraw_DrawLine(im, pclip, rc.r - 2, rc.t + 1, rc.r - 2, rc.b, clrBackLight);
      }
    }
    else {// bottom.
      iRectDeflate(&rc, 0, 1, 0, 2);
      // draw back.
      imdraw_DrawGradient(im, pclip, &rc, FALSE, clrBackDark, clrBackLight);
      if (cmpRes < 0) {// before selected tab or there isn't selected tab.
        // draw tab border.
        if (hover == TRUE && disabled == FALSE) {
          imdraw_DrawLine(im, pclip, rc.l, rc.t, rc.l, rc.b - 1, clrBorderHover);
          imdraw_DrawLine(im, pclip, rc.l, rc.b - 1, rc.r, rc.b - 1, clrBorderHover);
        }
        else {
          imdraw_DrawLine(im, pclip, rc.l, rc.t, rc.l, rc.b - 1, penBorder);
          imdraw_DrawLine(im, pclip, rc.l, rc.b - 1, rc.r, rc.b - 1, penBorder);
        }
        // draw outline.
        imdraw_DrawLine(im, pclip, rc.l + 1, rc.t, rc.l + 1, rc.b - 1, clrBackLight);
      }
      else {// after selected tab.
        // draw tab border.
        if (hover == TRUE && disabled == FALSE) {
          imdraw_DrawLine(im, pclip, rc.l, rc.b - 1, rc.r - 1, rc.b - 1, clrBorderHover);
          imdraw_DrawLine(im, pclip, rc.r - 1, rc.b - 1, rc.r - 1, rc.t - 1, clrBorderHover);
        }
        else {
          imdraw_DrawLine(im, pclip, rc.l, rc.b - 1, rc.r - 1, rc.b - 1, penBorder);
          imdraw_DrawLine(im, pclip, rc.r - 1, rc.b - 1, rc.r - 1, rc.t - 1, penBorder);
        }
        // draw outline.
        imdraw_DrawLine(im, pclip, rc.r - 2, rc.t, rc.r - 2, rc.b - 1, clrBackLight);
      }
    }
    // draw closing line.
    if (cmpRes < 0) {// before selected tab or there isn't selected tab.
      BOOL lastTab = ((count - 1) == hTab);
      if (lastTab == TRUE) {
        imdraw_DrawLine(im, pclip, rc.r - 1, rc.t, rc.r - 1, rc.b, penBorder);
      }
    }
  }
  return 0;
}
static int TabCtrlStyle_VS2008_bars_base_DrawTabContext(TabCtrl* s, img_t* im, const IRECT* pclip, int hTab, IRECT const* pRect)
{
  IRECT rc = *(pRect);
  if (s->m_hCurTab == hTab) {
    if (s->m_Layout == TAB_LAYOUT_TOP) {
      rc.t -= 2;
    }
    else {
      rc.b += 2;
    }
  }
  TabCtrlStyle_base_DrawTabContext(s, im, pclip, hTab, &rc);
  return 0;
}
static COLOR TabCtrlStyle_VS2008_bars_base_GetTabTextColor(TabCtrl* s, int hTab)
{
  if (s->m_vtrtabs[hTab].disable == TRUE) {// disabled.
    return TabCtrlStyle_base_GetTabTextColor(s, hTab);
  }
  else if (s->m_hCurTab == hTab) {// selected.
    return _GetSysColor(CLR_BTNTEXT);
  }
  else {
    return PixelAlpha_100(_GetSysColor(CLR_BTNSHADOW), _RGB(0, 0, 0), 55);
  }
  return 0;
}
static int TabCtrlStyle_VS2008_bars_base_set(TabCtrl* s)
{
  TabCtrlStyle_base_set(s);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_base, GetBorderWidth);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_base, GetCtrlAreaPadding);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_base, GetTabPadding);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_base, GetTabBorderColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_base, GetTabGradientLightColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_base, GetTabGradientDarkColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_base, DrawCtrlAreaBack);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_base, DrawTabBack);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_base, DrawTabContext);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_base, GetTabTextColor);
  return 0;
}
// TabCtrlStyle_VS2008_bars_classic.
// TabCtrlStyle_VS2008_bars_base.
static COLOR TabCtrlStyle_VS2008_bars_classic_GetTabBorderColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return (hover == TRUE ? _GetSysColor(CLR_3DDKSHADOW) : _GetSysColor(CLR_BTNSHADOW));
}
static COLOR TabCtrlStyle_VS2008_bars_classic_GetTabGradientLightColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return _GetSysColor(CLR_WINDOW);
}
static COLOR TabCtrlStyle_VS2008_bars_classic_GetTabGradientDarkColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return ((hover == TRUE && disable == FALSE) ?
      PixelAlpha_100(_GetSysColor(CLR_BTNSHADOW), _RGB(255, 255, 255), 75) :
      PixelAlpha_100(_GetSysColor(CLR_BTNSHADOW), _RGB(255, 255, 255), 55));
}
static int TabCtrlStyle_VS2008_bars_classic_set(TabCtrl* s)
{
  TabCtrlStyle_VS2008_bars_base_set(s);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_classic, GetTabBorderColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_classic, GetTabGradientLightColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_classic, GetTabGradientDarkColor);
  return 0;
}
// TabCtrlStyle_VS2008_bars_blue.
// TabCtrlStyle_VS2008_bars_base.
static COLOR TabCtrlStyle_VS2008_bars_blue_GetTabBorderColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return (hover == TRUE ? _RGB(60, 127, 177) : _RGB(137, 140, 149));
}
static COLOR TabCtrlStyle_VS2008_bars_blue_GetTabGradientLightColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return ((hover == TRUE && disable == FALSE) ? _RGB(250, 253, 254) : _RGB(252, 252, 252));
}
static COLOR TabCtrlStyle_VS2008_bars_blue_GetTabGradientDarkColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return ((hover == TRUE && disable == FALSE) ? _RGB(167, 217, 245) : _RGB(207, 207, 207));
}
static int TabCtrlStyle_VS2008_bars_blue_set(TabCtrl* s)
{
  TabCtrlStyle_VS2008_bars_base_set(s);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_blue, GetTabBorderColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_blue, GetTabGradientLightColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_blue, GetTabGradientDarkColor);
  return 0;
}
// TabCtrlStyle_VS2008_bars_silver.
// TabCtrlStyle_VS2008_bars_base.
static COLOR TabCtrlStyle_VS2008_bars_silver_GetTabBorderColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return (hover == TRUE ? _RGB(119, 119, 146) : _RGB(137, 140, 149));
}
static COLOR TabCtrlStyle_VS2008_bars_silver_GetTabGradientLightColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return ((hover == TRUE && disable == FALSE) ? _RGB(247, 247, 253) : _RGB(252, 252, 252));
}
static COLOR TabCtrlStyle_VS2008_bars_silver_GetTabGradientDarkColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return ((hover == TRUE && disable == FALSE) ? _RGB(180, 179, 202) : _RGB(207, 207, 207));
}
static int TabCtrlStyle_VS2008_bars_silver_DrawButtonFrame(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect, BOOL bHover, BOOL bPushed)
{
  if (bHover != bPushed) {
    imdraw_DrawRectR(im, pclip, *pRect, _RGB(255, 227, 173), _RGB(74, 73, 107));
  }
  else if (bHover == TRUE && bPushed == TRUE) {
    imdraw_DrawRectR(im, pclip, *pRect, _RGB(255, 182, 115), _RGB(74, 73, 107));
  }
  return 0;
}
static int TabCtrlStyle_VS2008_bars_silver_set(TabCtrl* s)
{
  TabCtrlStyle_VS2008_bars_base_set(s);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_silver, GetTabBorderColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_silver, GetTabGradientLightColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_silver, GetTabGradientDarkColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_silver, DrawButtonFrame);
  return 0;
}
// TabCtrlStyle_VS2008_bars_olive.
// TabCtrlStyle_VS2008_bars_base.
static COLOR TabCtrlStyle_VS2008_bars_olive_GetTabBorderColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return (hover == TRUE ? _RGB(143, 158, 116) : _RGB(137, 140, 149));
}
static COLOR TabCtrlStyle_VS2008_bars_olive_GetTabGradientLightColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return ((hover == TRUE && disable == FALSE) ? _RGB(250, 251, 247) : _RGB(252, 252, 252));
}
static COLOR TabCtrlStyle_VS2008_bars_olive_GetTabGradientDarkColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return ((hover == TRUE && disable == FALSE) ? _RGB(182, 198, 141) : _RGB(207, 207, 207));
}
static int TabCtrlStyle_VS2008_bars_olive_DrawButtonFrame(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect, BOOL bHover, BOOL bPushed)
{
  if (bHover != bPushed) {
    imdraw_DrawRectR(im, pclip, *pRect, _RGB(255, 227, 173), _RGB(118, 128, 95));
  }
  else if (bHover == TRUE && bPushed == TRUE) {
    imdraw_DrawRectR(im, pclip, *pRect, _RGB(255, 182, 115), _RGB(118, 128, 95));
  }
  return 0;
}
static int TabCtrlStyle_VS2008_bars_olive_set(TabCtrl* s)
{
  TabCtrlStyle_VS2008_bars_base_set(s);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_olive, GetTabBorderColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_olive, GetTabGradientLightColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_olive, GetTabGradientDarkColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_olive, DrawButtonFrame);
  return 0;
}
// TabCtrlStyle_VS2008_bars_custom1_base.
// ITabCtrlRecalc.
static IRECT TabCtrlStyle_VS2008_bars_custom1_base_GetCtrlAreaPadding(TabCtrl* s)
{
  return (s->m_Layout == TAB_LAYOUT_TOP ? iRECT(0, 1, 0, 0) : iRECT(0, 0, 0, 1));
}
static IRECT TabCtrlStyle_VS2008_bars_custom1_base_GetWndsAreaPadding(TabCtrl* s)
{
  BOOL bHideSingleTab = (s->m_count == 1 && s->m_bHideSingleTab == TRUE);
  if (bHideSingleTab == FALSE) {
    return (s->m_Layout == TAB_LAYOUT_TOP ? iRECT(1, 0, 1, 1) : iRECT(1, 1, 1, 0));
  }
  else {
    return iRECT(1, 1, 1, 1);
  }
}
static int TabCtrlStyle_VS2008_bars_custom1_base_DrawCtrlAreaBack(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect)
{
  COLOR clrBorder = s->GetTabBorderColor(s, FALSE, FALSE, FALSE);
  imdraw_FillSolidRect(im, pclip, pRect, s->m_clrCtrlAreaBackColor);
  if (s->m_Layout == TAB_LAYOUT_TOP) {
    imdraw_DrawLine(im, pclip, pRect->l, pRect->b - 1, pRect->r, pRect->b - 1, clrBorder);
  }
  else {
    imdraw_DrawLine(im, pclip, pRect->l, pRect->t, pRect->r, pRect->t, clrBorder);
  }
  return 0;
}
static int TabCtrlStyle_VS2008_bars_custom1_base_DrawWndsAreaBack(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect)
{
  int count = s->m_count;
  if (count == 0 || s->m_hCurTab == TabHit_Null) {
    TabCtrlStyle_base_DrawWndsAreaBack(s, im, pclip, pRect);
  }
  else {
    COLOR clrBorder = s->GetTabBorderColor(s, FALSE, FALSE, FALSE);
    BOOL bHideSingleTab = (count == 1 && s->m_bHideSingleTab == TRUE);
    if (bHideSingleTab == TRUE) {
      imdraw_DrawRectR(im, pclip, *pRect, 0, clrBorder);
    }
    else {
      BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
      COLOR penBorder = (clrBorder);
      if (top == TRUE) {
        imdraw_DrawLine(im, pclip, pRect->l, pRect->t, pRect->l, pRect->b - 1, penBorder);
        imdraw_DrawLine(im, pclip, pRect->l, pRect->b - 1, pRect->r - 1, pRect->b - 1, penBorder);
        imdraw_DrawLine(im, pclip, pRect->r - 1, pRect->b - 1, pRect->r - 1, pRect->t - 1, penBorder);
      }
      else {
        imdraw_DrawLine(im, pclip, pRect->l, pRect->b - 1, pRect->l, pRect->t, penBorder);
        imdraw_DrawLine(im, pclip, pRect->l, pRect->t, pRect->r - 1, pRect->t, penBorder);
        imdraw_DrawLine(im, pclip, pRect->r - 1, pRect->t, pRect->r - 1, pRect->b, penBorder);
      }
    }
  }
  return 0;
}
static int TabCtrlStyle_VS2008_bars_custom1_base_set(TabCtrl* s)
{
  TabCtrlStyle_VS2008_bars_base_set(s);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_custom1_base, GetCtrlAreaPadding);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_custom1_base, GetWndsAreaPadding);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_custom1_base, DrawCtrlAreaBack);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_custom1_base, DrawWndsAreaBack);
  return 0;
}
// TabCtrlStyle_VS2008_bars_classic_custom1.
// TabCtrlStyle_VS2008_bars_base.
static COLOR TabCtrlStyle_VS2008_bars_classic_custom1_GetTabBorderColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return (hover == TRUE ? _GetSysColor(CLR_3DDKSHADOW) : _GetSysColor(CLR_BTNSHADOW));
}
static COLOR TabCtrlStyle_VS2008_bars_classic_custom1_GetTabGradientLightColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return _GetSysColor(CLR_WINDOW);
}
static COLOR TabCtrlStyle_VS2008_bars_classic_custom1_GetTabGradientDarkColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return ((hover == TRUE && disable == FALSE) ?
      PixelAlpha_100(_GetSysColor(CLR_BTNSHADOW), _RGB(255, 255, 255), 75) :
      PixelAlpha_100(_GetSysColor(CLR_BTNSHADOW), _RGB(255, 255, 255), 55));
}
static int TabCtrlStyle_VS2008_bars_classic_custom1_set(TabCtrl* s)
{
  TabCtrlStyle_VS2008_bars_custom1_base_set(s);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_classic_custom1, GetTabBorderColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_classic_custom1, GetTabGradientLightColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_classic_custom1, GetTabGradientDarkColor);
  return 0;
}
// TabCtrlStyle_VS2008_bars_blue.
// TabCtrlStyle_VS2008_bars_base.
static COLOR TabCtrlStyle_VS2008_bars_blue_custom1_GetTabBorderColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return (hover == TRUE ? _RGB(60, 127, 177) : _RGB(137, 140, 149));
}
static COLOR TabCtrlStyle_VS2008_bars_blue_custom1_GetTabGradientLightColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return ((hover == TRUE && disable == FALSE) ? _RGB(250, 253, 254) : _RGB(252, 252, 252));
}
static COLOR TabCtrlStyle_VS2008_bars_blue_custom1_GetTabGradientDarkColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return ((hover == TRUE && disable == FALSE) ? _RGB(167, 217, 245) : _RGB(207, 207, 207));
}
static int TabCtrlStyle_VS2008_bars_blue_custom1_set(TabCtrl* s)
{
  TabCtrlStyle_VS2008_bars_custom1_base_set(s);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_blue_custom1, GetTabBorderColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_blue_custom1, GetTabGradientLightColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_blue_custom1, GetTabGradientDarkColor);
  return 0;
}
// TabCtrlStyle_VS2008_bars_silver.
// TabCtrlStyle_VS2008_bars_base.
static COLOR TabCtrlStyle_VS2008_bars_silver_custom1_GetTabBorderColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return (hover == TRUE ? _RGB(119, 119, 146) : _RGB(137, 140, 149));
}
static COLOR TabCtrlStyle_VS2008_bars_silver_custom1_GetTabGradientLightColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return ((hover == TRUE && disable == FALSE) ? _RGB(247, 247, 253) : _RGB(252, 252, 252));
}
static COLOR TabCtrlStyle_VS2008_bars_silver_custom1_GetTabGradientDarkColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return ((hover == TRUE && disable == FALSE) ? _RGB(180, 179, 202) : _RGB(207, 207, 207));
}
static int TabCtrlStyle_VS2008_bars_silver_custom1_DrawButtonFrame(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect, BOOL bHover, BOOL bPushed)
{
  if (bHover != bPushed) {
    imdraw_DrawRectR(im, pclip, *pRect, _RGB(255, 227, 173), _RGB(74, 73, 107));
  }
  else if (bHover == TRUE && bPushed == TRUE) {
    imdraw_DrawRectR(im, pclip, *pRect, _RGB(255, 182, 115), _RGB(74, 73, 107));
  }
  return 0;
}
static int TabCtrlStyle_VS2008_bars_silver_custom1_set(TabCtrl* s)
{
  TabCtrlStyle_VS2008_bars_custom1_base_set(s);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_silver_custom1, GetTabBorderColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_silver_custom1, GetTabGradientLightColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_silver_custom1, GetTabGradientDarkColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_silver_custom1, DrawButtonFrame);
  return 0;
}
// TabCtrlStyle_VS2008_bars_olive.
// TabCtrlStyle_VS2008_bars_base.
static COLOR TabCtrlStyle_VS2008_bars_olive_custom1_GetTabBorderColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return (hover == TRUE ? _RGB(143, 158, 116) : _RGB(137, 140, 149));
}
static COLOR TabCtrlStyle_VS2008_bars_olive_custom1_GetTabGradientLightColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return ((hover == TRUE && disable == FALSE) ? _RGB(250, 251, 247) : _RGB(252, 252, 252));
}
static COLOR TabCtrlStyle_VS2008_bars_olive_custom1_GetTabGradientDarkColor(TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  return ((hover == TRUE && disable == FALSE) ? _RGB(182, 198, 141) : _RGB(207, 207, 207));
}
static int TabCtrlStyle_VS2008_bars_olive_custom1_DrawButtonFrame(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect, BOOL bHover, BOOL bPushed)
{
  if (bHover != bPushed) {
    imdraw_DrawRectR(im, pclip, *pRect, _RGB(255, 227, 173), _RGB(118, 128, 95));
  }
  else if (bHover == TRUE && bPushed == TRUE) {
    imdraw_DrawRectR(im, pclip, *pRect, _RGB(255, 182, 115), _RGB(118, 128, 95));
  }
  return 0;
}
static int TabCtrlStyle_VS2008_bars_olive_custom1_set(TabCtrl* s)
{
  TabCtrlStyle_VS2008_bars_custom1_base_set(s);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_olive_custom1, GetTabBorderColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_olive_custom1, GetTabGradientLightColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_olive_custom1, GetTabGradientDarkColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2008_bars_olive_custom1, DrawButtonFrame);
  return 0;
}
// TabCtrlStyle_VS2010_client.
// ITabCtrlStyle.
static int TabCtrlStyle_VS2010_client_GetBorderWidth(TabCtrl* s)
{
  return 0;
}
static IRECT TabCtrlStyle_VS2010_client_GetCtrlAreaPadding(TabCtrl* s)
{
  return iRECT(0, 0, 3, 0);
}
static IRECT TabCtrlStyle_VS2010_client_GetWndsAreaPadding(TabCtrl* s)
{
  return iRECT(0, 4, 0, 4);
}
static IRECT TabCtrlStyle_VS2010_client_GetTabPadding(TabCtrl* s)
{
  return (s->m_Layout == TAB_LAYOUT_TOP ?
      iRECT(5, 1/*border*/ + 3, 5, 3) : iRECT(5, 3, 5, 3 + 1/*border*/));
}
static int TabCtrlStyle_VS2010_client_DrawCtrlAreaBack(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect)
{
  return imdraw_FillSolidRect(im, pclip, pRect, s->m_clrCtrlAreaBackColor);
}
static int TabCtrlStyle_VS2010_client_DrawTabBack_Impl(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect, BOOL top, BOOL active, BOOL select, BOOL hover)
{
  IRECT rc = *(pRect);
  if (top == TRUE) {
    if (select == TRUE) {
      if (active == TRUE) {
        rc.t = (rc.t + rc.b) / 2;
        imdraw_FillSolidRect(im, pclip, &rc, _RGB(255, 232, 166));// bottom path.
        rc.b = rc.t;
        rc.t = pRect->t;
        imdraw_DrawGradient(im, pclip, &rc, FALSE, _RGB(255, 252, 242), _RGB(255, 243, 207));// top path.
        // left corner.
        imdraw_SetPixel(im, pclip, rc.l, rc.t, _RGB(94, 109, 133));
        imdraw_SetPixel(im, pclip, rc.l + 1, rc.t, _RGB(195, 198, 199));
        imdraw_SetPixel(im, pclip, rc.l, rc.t + 1, _RGB(195, 197, 197));
        // right corner.
        imdraw_SetPixel(im, pclip, rc.r - 1, rc.t, _RGB(94, 109, 133));
        imdraw_SetPixel(im, pclip, rc.r - 2, rc.t, _RGB(195, 198, 199));
        imdraw_SetPixel(im, pclip, rc.r - 1, rc.t + 1, _RGB(195, 197, 197));
      }
      else {
        rc.t = (rc.t + rc.b) / 2;
        imdraw_FillSolidRect(im, pclip, &rc, _RGB(206, 212, 223));// bottom path.
        rc.b = rc.t;
        rc.t = pRect->t;
        imdraw_DrawGradient(im, pclip, &rc, FALSE, _RGB(251, 252, 252), _RGB(215, 220, 228));// top path.
        // left corner.
        imdraw_SetPixel(im, pclip, rc.l, rc.t, _RGB(93, 109, 135));
        imdraw_SetPixel(im, pclip, rc.l + 1, rc.t, _RGB(192, 198, 206));
        imdraw_SetPixel(im, pclip, rc.l, rc.t + 1, _RGB(190, 196, 204));
        // right corner.
        imdraw_SetPixel(im, pclip, rc.r - 1, rc.t, _RGB(93, 109, 135));
        imdraw_SetPixel(im, pclip, rc.r - 2, rc.t, _RGB(192, 198, 206));
        imdraw_SetPixel(im, pclip, rc.r - 1, rc.t + 1, _RGB(190, 196, 204));
      }
    }
    else if (hover == TRUE) {
      // draw border.
      COLOR pen = (_RGB(155, 167, 183));
      imdraw_DrawLine(im, pclip, rc.l, rc.b, rc.l, rc.t, pen);
      imdraw_DrawLine(im, pclip, rc.l, rc.t, rc.r - 1, rc.t, pen);
      imdraw_DrawLine(im, pclip, rc.r - 1, rc.t, rc.r - 1, rc.b, pen);
      // draw back.
      iRectDeflate(&rc, 1, 1, 1, 0);
      imdraw_DrawGradient(im, pclip, &rc, FALSE, _RGB(111, 119, 118), _RGB(79, 95, 116));
      iRectInflate(&rc, 1, 1, 1, 0);
      // left corner.
      imdraw_SetPixel(im, pclip, rc.l, rc.t, _RGB(50, 65, 93));
      imdraw_SetPixel(im, pclip, rc.l + 1, rc.t, _RGB(120, 135, 157));
      imdraw_SetPixel(im, pclip, rc.l, rc.t + 1, _RGB(117, 130, 150));
      imdraw_SetPixel(im, pclip, rc.l + 1, rc.t + 1, _RGB(115, 126, 139));
      // right corner.
      imdraw_SetPixel(im, pclip, rc.r - 1, rc.t, _RGB(50, 65, 93));
      imdraw_SetPixel(im, pclip, rc.r - 2, rc.t, _RGB(120, 135, 157));
      imdraw_SetPixel(im, pclip, rc.r - 1, rc.t + 1, _RGB(117, 130, 150));
      imdraw_SetPixel(im, pclip, rc.r - 2, rc.t + 1, _RGB(115, 126, 139));
    }
  }
  else {// bottom.
    if (select == TRUE) {
      if (active == TRUE) {
        rc.b = (rc.t + rc.b) / 2;
        imdraw_FillSolidRect(im, pclip, &rc, _RGB(255, 232, 166));// top path.
        rc.t = rc.b;
        rc.b = pRect->b;
        imdraw_DrawGradient(im, pclip, &rc, FALSE, _RGB(255, 243, 207), _RGB(255, 252, 242));// bottom path.
        // left corner.
        imdraw_SetPixel(im, pclip, rc.l, rc.b - 1, _RGB(94, 109, 133));
        imdraw_SetPixel(im, pclip, rc.l + 1, rc.b - 1, _RGB(195, 198, 199));
        imdraw_SetPixel(im, pclip, rc.l, rc.b - 2, _RGB(195, 197, 197));
        // right corner.
        imdraw_SetPixel(im, pclip, rc.r - 1, rc.b - 1, _RGB(94, 109, 133));
        imdraw_SetPixel(im, pclip, rc.r - 2, rc.b - 1, _RGB(195, 198, 199));
        imdraw_SetPixel(im, pclip, rc.r - 1, rc.b - 2, _RGB(195, 197, 197));
      }
      else {
        rc.b = (rc.t + rc.b) / 2;
        imdraw_FillSolidRect(im, pclip, &rc, _RGB(206, 212, 223));// bottom path.
        rc.t = rc.b;
        rc.b = pRect->b;
        imdraw_DrawGradient(im, pclip, &rc, FALSE, _RGB(215, 220, 228), _RGB(251, 252, 252));// top path.
        // left corner.
        imdraw_SetPixel(im, pclip, rc.l, rc.b - 1, _RGB(93, 109, 135));
        imdraw_SetPixel(im, pclip, rc.l + 1, rc.b - 1, _RGB(192, 198, 206));
        imdraw_SetPixel(im, pclip, rc.l, rc.b - 2, _RGB(190, 196, 204));
        // right corner.
        imdraw_SetPixel(im, pclip, rc.r - 1, rc.b - 1, _RGB(93, 109, 135));
        imdraw_SetPixel(im, pclip, rc.r - 2, rc.b - 1, _RGB(192, 198, 206));
        imdraw_SetPixel(im, pclip, rc.r - 1, rc.b - 2, _RGB(190, 196, 204));
      }
    }
    else if (hover == TRUE) {
      // draw border.
      COLOR pen = _RGB(155, 167, 183);
      imdraw_DrawLine(im, pclip, rc.l, rc.t, rc.l, rc.b - 1, pen);
      imdraw_DrawLine(im, pclip, rc.l, rc.b - 1, rc.r - 1, rc.b - 1, pen);
      imdraw_DrawLine(im, pclip, rc.r - 1, rc.b - 1, rc.r - 1, rc.t, pen);
      // draw back.
      iRectDeflate(&rc, 1, 0, 1, 1);
      imdraw_DrawGradient(im, pclip, &rc, FALSE, _RGB(79, 95, 116), _RGB(111, 119, 118));
      iRectInflate(&rc, 1, 0, 1, 1);
      // left corner.
      imdraw_SetPixel(im, pclip, rc.l, rc.b - 1, _RGB(50, 65, 93));
      imdraw_SetPixel(im, pclip, rc.l + 1, rc.b - 1, _RGB(120, 135, 157));
      imdraw_SetPixel(im, pclip, rc.l, rc.b - 2, _RGB(117, 130, 150));
      imdraw_SetPixel(im, pclip, rc.l + 1, rc.b - 2, _RGB(115, 126, 139));
      // right corner.
      imdraw_SetPixel(im, pclip, rc.r - 1, rc.b - 1, _RGB(50, 65, 93));
      imdraw_SetPixel(im, pclip, rc.r - 2, rc.b - 1, _RGB(120, 135, 157));
      imdraw_SetPixel(im, pclip, rc.r - 1, rc.b - 2, _RGB(117, 130, 150));
      imdraw_SetPixel(im, pclip, rc.r - 2, rc.b - 2, _RGB(115, 126, 139));
    }
  }
  return 0;
}
static int TabCtrlStyle_VS2010_client_DrawTabBack(TabCtrl* s, img_t* im, const IRECT* pclip, int hTab, IRECT const* pRect)
{
  if (s->m_vtrtabs[hTab].disable == FALSE) {
    BOOL active = s->m_bActive;
    BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
    BOOL select = (s->m_hCurTab == hTab);
    BOOL hover = (s->m_hHoverArea == hTab);
    TabCtrlStyle_VS2010_client_DrawTabBack_Impl(s, im, pclip, pRect, top, active, select, hover);
  }
  return 0;
}
static int TabCtrlStyle_VS2010_client_DrawWndsAreaBack_Impl(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect, BOOL top, BOOL active)
{
  IRECT rc;
  int firstTab = (s->m_count > 0 ? 0 : -1);
  BOOL selectFirstTab = (s->m_hCurTab == firstTab);
  BOOL scaleMode = (s->m_Behavior == TAB_BEHAVIOR_SCALE);
  COLOR clr = (active == TRUE ? _RGB(255, 232, 166) : _RGB(206, 212, 223));
  // draw top.
  rc = iRECT(pRect->l, pRect->t, pRect->r, pRect->t + 4);
  imdraw_FillSolidRect(im, pclip, &rc, clr);
  if (top == FALSE || scaleMode == TRUE) {
    // left corner.
    if (top == FALSE || selectFirstTab == FALSE || (s->m_count == 1 && s->m_bHideSingleTab == TRUE))
      if (top == TRUE && s->m_hHoverArea == firstTab) {// tab is highlight.
        if (active == TRUE) {
          imdraw_SetPixel(im, pclip, rc.l, rc.t, _RGB(175, 180, 180));
          imdraw_SetPixel(im, pclip, rc.l + 1, rc.t, _RGB(205, 194, 152));
          imdraw_SetPixel(im, pclip, rc.l, rc.t + 1, _RGB(227, 214, 171));
        }
        else {
          imdraw_SetPixel(im, pclip, rc.l, rc.t, _RGB(165, 176, 191));
          imdraw_SetPixel(im, pclip, rc.l + 1, rc.t, _RGB(170, 179, 193));
          imdraw_SetPixel(im, pclip, rc.l, rc.t + 1, _RGB(192, 199, 212));
        }
      }
      else// tab isn't highlight.
        if (active == TRUE) {
          imdraw_SetPixel(im, pclip, rc.l, rc.t, _RGB(86, 95, 105));
          imdraw_SetPixel(im, pclip, rc.l + 1, rc.t, _RGB(198, 188, 149));
          imdraw_SetPixel(im, pclip, rc.l, rc.t + 1, _RGB(195, 184, 144));
        }
        else {
          imdraw_SetPixel(im, pclip, rc.l, rc.t, _RGB(76, 91, 116));
          imdraw_SetPixel(im, pclip, rc.l + 1, rc.t, _RGB(163, 173, 190));
          imdraw_SetPixel(im, pclip, rc.l, rc.t + 1, _RGB(160, 169, 185));
        }
    // right corner.
    if (active == TRUE) {
      imdraw_SetPixel(im, pclip, rc.r - 1, rc.t, _RGB(100, 106, 109));
      imdraw_SetPixel(im, pclip, rc.r - 2, rc.t, _RGB(205, 192, 148));
      imdraw_SetPixel(im, pclip, rc.r - 1, rc.t + 1, _RGB(195, 184, 144));
    }
    else {
      imdraw_SetPixel(im, pclip, rc.r - 1, rc.t, _RGB(87, 100, 124));
      imdraw_SetPixel(im, pclip, rc.r - 2, rc.t, _RGB(168, 176, 192));
      imdraw_SetPixel(im, pclip, rc.r - 1, rc.t + 1, _RGB(160, 169, 185));
    }
  }
  // draw bottom.
  rc = iRECT(pRect->l, pRect->b - 4, pRect->r, pRect->b);
  imdraw_FillSolidRect(im, pclip, &rc, clr);
  if (top == TRUE || scaleMode == TRUE) {
    // left corner.
    if (top == TRUE || selectFirstTab == FALSE || (s->m_count == 1 && s->m_bHideSingleTab == TRUE))
      if (top == FALSE && s->m_hHoverArea == firstTab) {// tab is highlight.
        if (active == TRUE) {
          imdraw_SetPixel(im, pclip, rc.l, rc.b - 1, _RGB(175, 180, 180));
          imdraw_SetPixel(im, pclip, rc.l + 1, rc.b - 1, _RGB(205, 194, 152));
          imdraw_SetPixel(im, pclip, rc.l, rc.b - 2, _RGB(227, 214, 171));
        }
        else {
          imdraw_SetPixel(im, pclip, rc.l, rc.b - 1, _RGB(165, 176, 191));
          imdraw_SetPixel(im, pclip, rc.l + 1, rc.b - 1, _RGB(170, 179, 193));
          imdraw_SetPixel(im, pclip, rc.l, rc.b - 2, _RGB(192, 199, 212));
        }
      }
      else// tab isn't highlight.
        if (active == TRUE) {
          imdraw_SetPixel(im, pclip, rc.l, rc.b - 1, _RGB(86, 95, 105));
          imdraw_SetPixel(im, pclip, rc.l + 1, rc.b - 1, _RGB(198, 188, 149));
          imdraw_SetPixel(im, pclip, rc.l, rc.b - 2, _RGB(195, 184, 144));
        }
        else {
          imdraw_SetPixel(im, pclip, rc.l, rc.b - 1, _RGB(76, 91, 116));
          imdraw_SetPixel(im, pclip, rc.l + 1, rc.b - 1, _RGB(163, 173, 190));
          imdraw_SetPixel(im, pclip, rc.l, rc.b - 2, _RGB(160, 169, 185));
        }
    // right corner.
    if (active == TRUE) {
      imdraw_SetPixel(im, pclip, rc.r - 1, rc.b - 1, _RGB(100, 106, 109));
      imdraw_SetPixel(im, pclip, rc.r - 2, rc.b - 1, _RGB(205, 192, 148));
      imdraw_SetPixel(im, pclip, rc.r - 1, rc.b - 2, _RGB(195, 184, 144));
    }
    else {
      imdraw_SetPixel(im, pclip, rc.r - 1, rc.b - 1, _RGB(87, 100, 124));
      imdraw_SetPixel(im, pclip, rc.r - 2, rc.b - 1, _RGB(168, 176, 192));
      imdraw_SetPixel(im, pclip, rc.r - 1, rc.b - 2, _RGB(160, 169, 185));
    }
  }
  return 0;
}
static int TabCtrlStyle_VS2010_client_DrawWndsAreaBack(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect)
{
  if (s->m_count == 0 || s->m_hCurTab == TabHit_Null) {
    TabCtrlStyle_base_DrawWndsAreaBack(s, im, pclip, pRect);
  }
  else {
    BOOL active = s->m_bActive;
    BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
    TabCtrlStyle_VS2010_client_DrawWndsAreaBack_Impl(s, im, pclip, pRect, top, active);
  }
  return 0;
}
static int TabCtrlStyle_VS2010_client_DrawButtonFrame(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect, BOOL bHover, BOOL bPushed)
{
  if (bHover != bPushed || (bHover == TRUE && bPushed == TRUE)) {
    IRECT rc = *(pRect);
    imdraw_DrawRectR(im, pclip, rc, 0, _RGB(229, 195, 101));
    iRectDeflate2(&rc, 1, 1);
    if (bHover == TRUE && bPushed == TRUE) {
      imdraw_FillSolidRect(im, pclip, &rc, _RGB(255, 232, 166));
    }
    else {
      imdraw_FillSolidRect(im, pclip, &rc, _RGB(255, 252, 244));
    }
  }
  return 0;
}
static COLOR TabCtrlStyle_VS2010_client_GetTabTextColor(TabCtrl* s, int hTab)
{
  return (s->m_hCurTab == hTab ? _RGB(13, 0, 5) : _RGB(248, 255, 255));
}
static COLOR TabCtrlStyle_VS2010_client_GetButtonMarkerColor(TabCtrl* s, BOOL bHover, BOOL bPushed)
{
  return ((bHover == TRUE || bPushed == TRUE) ? _RGB(0, 0, 0) : _RGB(206, 212, 221));
}
static int TabCtrlStyle_VS2010_client_set(TabCtrl* s)
{
  TabCtrlStyle_base_set(s);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2010_client, GetBorderWidth);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2010_client, GetCtrlAreaPadding);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2010_client, GetWndsAreaPadding);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2010_client, GetTabPadding);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2010_client, DrawCtrlAreaBack);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2010_client, DrawTabBack);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2010_client, DrawWndsAreaBack);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2010_client, DrawWndsAreaBack);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2010_client, DrawButtonFrame);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2010_client, GetTabTextColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2010_client, GetButtonMarkerColor);
  s->m_clrCtrlAreaBackColor = _RGB(46, 64, 94);
  s->m_clrEmptyWndsAreaBackColor = _RGB(46, 64, 94);
  return 0;
}
// TabCtrlStyle_VS2010_client_custom1.
// TabCtrlStyle_VS2010_client.
static int TabCtrlStyle_VS2010_client_custom1_DrawTabBack(TabCtrl* s, img_t* im, const IRECT* pclip, int hTab, IRECT const* pRect)
{
  if (s->m_vtrtabs[hTab].disable == FALSE) {
    BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
    BOOL select = (s->m_hCurTab == hTab);
    BOOL hover = (s->m_hHoverArea == hTab);
    TabCtrlStyle_VS2010_client_DrawTabBack_Impl(s, im, pclip, pRect, top, TRUE, select, hover);
  }
  return 0;
}
static int TabCtrlStyle_VS2010_client_custom1_DrawWndsAreaBack(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect)
{
  if (s->m_count == 0 || s->m_hCurTab == TabHit_Null) {
    TabCtrlStyle_base_DrawWndsAreaBack(s, im, pclip, pRect);
  }
  else {
    BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
    TabCtrlStyle_VS2010_client_DrawWndsAreaBack_Impl(s, im, pclip, pRect, top, TRUE);
  }
  return 0;
}
static int TabCtrlStyle_VS2010_client_custom1_set(TabCtrl* s)
{
  TabCtrlStyle_VS2010_client_set(s);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2010_client_custom1, DrawTabBack);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2010_client_custom1, DrawWndsAreaBack);
  return 0;
}
// TabCtrlStyle_VS2010_client_custom2.
// TabCtrlStyle_VS2010_client.
static int TabCtrlStyle_VS2010_client_custom2_DrawTabBack(TabCtrl* s, img_t* im, const IRECT* pclip, int hTab, IRECT const* pRect)
{
  if (s->m_vtrtabs[hTab].disable == FALSE) {
    BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
    BOOL select = (s->m_hCurTab == hTab);
    BOOL hover = (s->m_hHoverArea == hTab);
    TabCtrlStyle_VS2010_client_DrawTabBack_Impl(s, im, pclip, pRect, top, FALSE, select, hover);
  }
  return 0;
}
static int TabCtrlStyle_VS2010_client_custom2_DrawWndsAreaBack(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect)
{
  if (s->m_count == 0 || s->m_hCurTab == TabHit_Null) {
    TabCtrlStyle_base_DrawWndsAreaBack(s, im, pclip, pRect);
  }
  else {
    BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
    TabCtrlStyle_VS2010_client_DrawWndsAreaBack_Impl(s, im, pclip, pRect, top, FALSE);
  }
  return 0;
}
static int TabCtrlStyle_VS2010_client_custom2_set(TabCtrl* s)
{
  TabCtrlStyle_VS2010_client_set(s);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2010_client_custom2, DrawTabBack);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2010_client_custom2, DrawWndsAreaBack);
  return 0;
}
// TabCtrlStyle_VS2010_bars.
// ITabCtrlStyle.
static int TabCtrlStyle_VS2010_bars_GetBorderWidth(TabCtrl* s)
{
  return 0;
}
static IRECT TabCtrlStyle_VS2010_bars_GetCtrlAreaPadding(TabCtrl* s)
{
  return iRECT(0, 0, 3, 0);
}
static IRECT TabCtrlStyle_VS2010_bars_GetTabPadding(TabCtrl* s)
{
  return (s->m_Layout == TAB_LAYOUT_TOP ?
      iRECT(5, 1/*border*/ + 1, 5, 2) : iRECT(5, 2, 5, 1 + 1/*border*/));
}
static int TabCtrlStyle_VS2010_bars_DrawCtrlAreaBack(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect)
{
  imdraw_FillSolidRect(im, pclip, pRect, s->m_clrCtrlAreaBackColor);
  return 0;
}
static int TabCtrlStyle_VS2010_bars_DrawTabBack(TabCtrl* s, img_t* im, const IRECT* pclip, int hTab, IRECT const* pRect)
{
  if (s->m_vtrtabs[hTab].disable == FALSE) {
    BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
    BOOL select = (s->m_hCurTab == hTab);
    BOOL hover = (s->m_hHoverArea == hTab);
    IRECT rc = *(pRect);
    if (top == TRUE) {
      if (select == TRUE) {
        imdraw_FillSolidRect(im, pclip, &rc, _RGB(255, 255, 255));// back.
        // left corner.
        imdraw_SetPixel(im, pclip, rc.l, rc.t, _RGB(103, 116, 138));
        imdraw_SetPixel(im, pclip, rc.l + 1, rc.t, _RGB(221, 224, 228));
        imdraw_SetPixel(im, pclip, rc.l, rc.t + 1, _RGB(197, 202, 210));
        // right corner.
        imdraw_SetPixel(im, pclip, rc.r - 1, rc.t, _RGB(103, 116, 138));
        imdraw_SetPixel(im, pclip, rc.r - 2, rc.t, _RGB(221, 224, 228));
        imdraw_SetPixel(im, pclip, rc.r - 1, rc.t + 1, _RGB(197, 202, 210));
      }
      else if (hover == TRUE) {
        // draw border.
        COLOR pen = (_RGB(155, 167, 183));
        IRECT rc1 = rc;
        iRectOffset(&rc1, 0, 0, -1, -1);
        imdraw_rect(im, pclip, rc1, 0, pen, 1, BDM_ALL);
        // draw back.
        iRectDeflate2(&rc, 1, 1);
        imdraw_DrawGradient(im, pclip, &rc, FALSE, _RGB(76, 92, 116), _RGB(111, 119, 118));
        iRectInflate2(&rc, 1, 1);
        // left corner.
        imdraw_SetPixel(im, pclip, rc.l, rc.t, _RGB(50, 65, 93));
        imdraw_SetPixel(im, pclip, rc.l + 1, rc.t, _RGB(120, 135, 157));
        imdraw_SetPixel(im, pclip, rc.l, rc.t + 1, _RGB(117, 130, 150));
        imdraw_SetPixel(im, pclip, rc.l + 1, rc.t + 1, _RGB(115, 126, 139));
        // right corner.
        imdraw_SetPixel(im, pclip, rc.r - 1, rc.t, _RGB(50, 65, 93));
        imdraw_SetPixel(im, pclip, rc.r - 2, rc.t, _RGB(120, 135, 157));
        imdraw_SetPixel(im, pclip, rc.r - 1, rc.t + 1, _RGB(117, 130, 150));
        imdraw_SetPixel(im, pclip, rc.r - 2, rc.t + 1, _RGB(115, 126, 139));
      }
    }
    else {// bottom.
      if (select == TRUE) {
        imdraw_FillSolidRect(im, pclip, &rc, _RGB(255, 255, 255));// back.
        // left corner.
        imdraw_SetPixel(im, pclip, rc.l, rc.b - 1, _RGB(103, 116, 138));
        imdraw_SetPixel(im, pclip, rc.l + 1, rc.b - 1, _RGB(221, 224, 228));
        imdraw_SetPixel(im, pclip, rc.l, rc.b - 2, _RGB(197, 202, 210));
        // right corner.
        imdraw_SetPixel(im, pclip, rc.r - 1, rc.b - 1, _RGB(103, 116, 138));
        imdraw_SetPixel(im, pclip, rc.r - 2, rc.b - 1, _RGB(221, 224, 228));
        imdraw_SetPixel(im, pclip, rc.r - 1, rc.b - 2, _RGB(197, 202, 210));
      }
      else if (hover == TRUE) {
        // draw border.
        COLOR pen = (_RGB(155, 167, 183));
        IRECT rc1 = rc;
        iRectOffset(&rc1, 0, 0, -1, -1);
        imdraw_rect(im, pclip, rc1, 0, pen, 1, BDM_ALL);
        // draw back.
        iRectDeflate2(&rc, 1, 1);
        imdraw_DrawGradient(im, pclip, &rc, FALSE, _RGB(111, 119, 118), _RGB(76, 92, 116));
        iRectInflate2(&rc, 1, 1);
        // left corner.
        imdraw_SetPixel(im, pclip, rc.l, rc.b - 1, _RGB(50, 65, 93));
        imdraw_SetPixel(im, pclip, rc.l + 1, rc.b - 1, _RGB(120, 135, 157));
        imdraw_SetPixel(im, pclip, rc.l, rc.b - 2, _RGB(117, 130, 150));
        imdraw_SetPixel(im, pclip, rc.l + 1, rc.b - 2, _RGB(115, 126, 139));
        // right corner.
        imdraw_SetPixel(im, pclip, rc.r - 1, rc.b - 1, _RGB(50, 65, 93));
        imdraw_SetPixel(im, pclip, rc.r - 2, rc.b - 1, _RGB(120, 135, 157));
        imdraw_SetPixel(im, pclip, rc.r - 1, rc.b - 2, _RGB(117, 130, 150));
        imdraw_SetPixel(im, pclip, rc.r - 2, rc.b - 2, _RGB(115, 126, 139));
      }
    }
  }
  return 0;
}
static int TabCtrlStyle_VS2010_bars_DrawButtonFrame(TabCtrl* s, img_t* im, const IRECT* pclip, IRECT const* pRect, BOOL bHover, BOOL bPushed)
{
  if (bHover != bPushed || (bHover == TRUE && bPushed == TRUE)) {
    IRECT rc = *(pRect);
    imdraw_DrawRectR(im, pclip, rc, 0, _RGB(229, 195, 101));
    iRectDeflate2(&rc, 1, 1);
    if (bHover == TRUE && bPushed == TRUE) {
      imdraw_FillSolidRect(im, pclip, &rc, _RGB(255, 232, 166));
    }
    else {
      imdraw_FillSolidRect(im, pclip, &rc, _RGB(255, 252, 244));
    }
  }
  return 0;
}
static COLOR TabCtrlStyle_VS2010_bars_GetTabTextColor(TabCtrl* s, int hTab)
{
  return (s->m_hCurTab == hTab ? _RGB(13, 0, 5) : _RGB(248, 255, 255));
}
static COLOR TabCtrlStyle_VS2010_bars_GetButtonMarkerColor(TabCtrl* s, BOOL bHover, BOOL bPushed)
{
  return ((bHover == TRUE || bPushed == TRUE) ? _RGB(0, 0, 0) : _RGB(206, 212, 221));
}
static int TabCtrlStyle_VS2010_bars_set(TabCtrl* s)
{
  TabCtrlStyle_base_set(s);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2010_bars, GetBorderWidth);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2010_bars, GetCtrlAreaPadding);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2010_bars, GetTabPadding);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2010_bars, DrawCtrlAreaBack);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2010_bars, DrawTabBack);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2010_bars, DrawButtonFrame);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2010_bars, GetTabTextColor);
  CLASSFUN_INIT(s, TabCtrlStyle_VS2010_bars, GetButtonMarkerColor);
  s->m_clrCtrlAreaBackColor = _RGB(46, 64, 94);
  s->m_clrEmptyWndsAreaBackColor = _RGB(46, 64, 94);
  return 0;
}

