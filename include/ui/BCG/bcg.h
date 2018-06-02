
#define _AFX_NO_OCC_SUPPORT
#include "bcgres.h"

#include "date.h"
#define BCGCBPRODLLEXPORT
#define afx_msg
#define AFXAPI
#define AFX_CDECL
#define DECLARE_SERIAL(x)
#define DECLARE_DYNAMIC(x)
#define DECLARE_DYNCREATE(x)
#define IMPLEMENT_DYNCREATE(a, b)
#define DECLARE_MESSAGE_MAP()  static LRESULT proc(ctrl_t* sender, ctrl_t* c, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#define RUNTIME_CLASS(x)  NULL
#define ASSERT_VALID(x)  ((x)!=NULL)
#define AfxFindResourceHandle(a, b)  (NULL)
#define GetSysColorBrush(x)  (x)
#define ImageList_GetIconSize(im, px, py)   (*(px) = (im)->w, *(py) = (im)->h)

#define HI_BIT(dwTarget, wBit) \
  do { \
    if (dwTarget != 0) \
      for (wBit = 32; (dwTarget & (0x00000001 << (wBit-1))) == 0; wBit--);\
    else \
      wBit = 0; \
  } while (0)

//#include "windowsx.h"

enum {
  BCGM_MSG__ = WM_USER + 100,
  BCGM_UPDATETOOLTIPS,
  BCGM_APPBAR_CALLBACK,
  BCGM_ON_RENAME_TAB,
  BCGM_CHANGE_ACTIVE_TAB,
  BCGM_ON_MOVE_TAB,
  BCGM_CHANGING_ACTIVE_TAB,
  BCGM_ON_GET_TAB_TOOLTIP,
  BCGM_CALENDAR_ON_SELCHANGED,
  BCGM_GETDOCUMENTCOLORS,
  BCGM_ONSETCONTROLAERO,
  BCGM_ONSETCONTROLVMMODE,
  BCGM_ON_PRESS_CLOSE_BUTTON,
  BCGM_ON_EDITCHANGE,
  WM_FINDREPLACE,
  BCGM_POSTSETPREVIEWFRAME,
  BCGM_ONAFTERUPDATECAPTION,
  BCGM_GANTT_CHART_CLICKITEM,
  BCGM_GANTT_CHART_CLICKHEADER,
  BCGM_GANTT_CHART_CLICKCHART,
  BCGM_GANTT_CHART_DBLCLICKITEM,
  BCGM_GANTT_CHART_DBLCLICKHEADER,
  BCGM_GANTT_CHART_DBLCLICKCHART,
  BCGM_GANTT_CHART_ITEM_MOVING,
  BCGM_GANTT_CHART_SCALE_CHANGING,
  BCGM_GANTT_CHART_SCALE_CHANGED,
  BCGM_GANTT_CONTROL_READ_ITEM_DATA_FROM_GRID,
  BCGM_GANTT_CONTROL_WRITE_ITEM_DATA_TO_GRID,
  BCGM_GANTT_CONTROL_CREATE_CHART,
  BCGM_GANTT_CONTROL_CREATE_GRID,
  BCGM_GANTT_STORAGE_CHANGED,
  BCGM_GANTT_CONNECTION_ADDED,
  BCGM_GANTT_CONNECTION_REMOVED,
  BCGM_GRID_ITEM_CHANGED,
  BCGM_GRID_SEL_CHANGED,
  BCGM_GRID_ITEM_DBLCLICK,
  BCGM_GRID_ON_HIDE_COLUMNCHOOSER,
  BCGM_ON_MOVETOTABGROUP,
  BCGPM_CHECKEMPTYMINIFRAME,
  BCGP_PLANNER_ADD_APPOINTMENT,
  BCGP_PLANNER_BEFORE_UPDATE_APPOINTMENT,
  BCGP_PLANNER_UPDATE_APPOINTMENT,
  BCGP_PLANNER_REMOVE_APPOINTMENT,
  BCGP_PLANNER_REMOVE_ALL_APPOINTMENTS,
  BCGP_PLANNER_BEFORE_SELECT_APPOINTMENT,
  BCGP_PLANNER_SELECT_APPOINTMENT,
  BCGP_PLANNER_TYPE_CHANGED,
  BCGP_PLANNER_DATE_CHANGED,
  BCGP_PLANNER_RESOURCEID_CHANGED,
  BCGP_PLANNER_LBUTTONDBLCLK,
  BCGP_PLANNER_KEYDOWN,
  BCGP_PLANNER_ICONUPDOWN_CLICK,
  BCGP_PLANNER_DAYCAPTION_CLICK,
  BCGP_PLANNER_WEEKCAPTION_CLICK,
  BCGP_PLANNER_DROP_APPOINTMENTS,
  BCGP_PLANNER_DRAG_APPOINTMENTS,
  BCGP_PLANNER_BEGIN_CHANGE_OPERATION,
  BCGP_PLANNER_END_CHANGE_OPERATION,
  BCGM_ON_CLOSEPOPUPWINDOW,
  BCGM_PROPERTY_CHANGED,
  BCGM_PROPERTY_COMMAND_CLICKED,
  BCGM_POSTRECALCLAYOUT,
  BCGM_ON_CHANGE_RIBBON_CATEGORY,
  BCGM_ON_RIBBON_CUSTOMIZE,
  BCGM_ON_HIGHLIGHT_RIBBON_LIST_ITEM,
  BCGM_ON_BEFORE_SHOW_RIBBON_ITEM_MENU,
  BCGM_ON_BEFORE_SHOW_PALETTE_CONTEXTMENU,
  BCGM_ON_DRAW_SCROLLBAR,
  BCGPM_CHANGE_CURRENT_FOLDER,
  BCGPM_ON_AFTER_SHELL_COMMAND,
  BCGM_ON_HSCROLL,
  BCGM_GETDRAGBOUNDS,
  BCGM_ON_DRAGCOMPLETE,
  BCGM_ON_TABGROUPMOUSEMOVE,
  BCGM_ON_CANCELTABMOVE,
  BCGM_ON_MOVETABCOMPLETE,
  BCGM_TOOLBARMENU,
  BCGM_CUSTOMIZETOOLBAR,
  BCGM_CREATETOOLBAR,
  BCGM_DELETETOOLBAR,
  BCGM_CUSTOMIZEHELP,
  BCGM_RESETTOOLBAR,
  BCGM_RESETMENU,
  BCGM_SHOWREGULARMENU,
  BCGM_RESETCONTEXTMENU,
  BCGM_RESETKEYBOARD,
  BCGM_RESETRPROMPT,
  BCGM_CHANGEVISUALMANAGER,
  WM_BCGWINDOW_HELP,
  DRAGLISTMSGSTRINGW,
};

// #include "windowsx.h"
#define CN_COMMAND              0               // void ()
#define CN_UPDATE_COMMAND_UI    ((UINT)(-1))    // void (CCmdUI*)
#define CN_EVENT                ((UINT)(-2))    // OLE event
#define CN_OLECOMMAND           ((UINT)(-3))    // OLE document command
#define CN_OLE_UNREGISTER       ((UINT)(-4))    // OLE unregister

#define BEGIN_MESSAGE_MAP(cls, base_cls)  LRESULT cls::proc(ctrl_t* sender, ctrl_t* c, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { cls* s = (cls*)(c->x);

#define MESSAGE_MAP_DEFAULT(cls, base_proc) cls() {memset(this, 0, sizeof(cls));} static LRESULT proc(ctrl_t* sender, ctrl_t* c, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return base_proc(sender, c, hwnd, uMsg, wParam, lParam); }
#define END_MESSAGE_MAP()                {} return 0;}
#define ON_MESSAGE(msg, fn)              if ( msg==uMsg) { s->fn(wParam, lParam); } else
#define ON_REGISTERED_MESSAGE(msg, fn)   if ( msg==uMsg) { s->fn(wParam, lParam); } else
#define ON_WM_ERASEBKGND()        if (WM_ERASEBKGND==uMsg) {CDC dc[1]; s->OnEraseBkgnd(dc); } else
#define ON_WM_CANCELMODE()        if (WM_CANCELMODE==uMsg) { s->OnCancelMode(); } else
#define ON_WM_MOUSEMOVE()         if (WM_MOUSEMOVE==uMsg) { s->OnMouseMove(wParam, CPoint(lParam)); } else
#define ON_WM_LBUTTONDOWN()       if (WM_LBUTTONDOWN==uMsg) { s->OnLButtonDown(wParam, CPoint(lParam)); } else
#define ON_WM_LBUTTONUP()         if (WM_LBUTTONUP==uMsg) { s->OnLButtonUp(wParam, CPoint(lParam)); } else
#define ON_WM_LBUTTONDBLCLK()     if (WM_LBUTTONDBLCLK==uMsg) { s->OnLButtonDblClk(wParam, CPoint(lParam)); } else
#define ON_WM_SETCURSOR()         if (WM_SETCURSOR==uMsg) { s->OnSetCursor(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)); } else
#define ON_WM_ENABLE()            if (WM_ENABLE==uMsg) { s->OnEnable((BOOL)(wParam)); } else
#define ON_WM_SIZE()              if (WM_SIZE==uMsg) { s->OnSize((UINT)(wParam), (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam)); } else
#define ON_WM_KILLFOCUS()         if (WM_KILLFOCUS==uMsg) { s->OnKillFocus(0); } else
#define ON_WM_TIMER()             if (WM_TIMER==uMsg) { s->OnTimer((UINT)(wParam)); } else
#define ON_WM_DESTROY()           if (WM_DESTROY==uMsg) { s->OnDestroy(); } else
#define ON_WM_GETDLGCODE()        if (WM_GETDLGCODE==uMsg) { s->OnGetDlgCode(); } else
#define ON_WM_CTRLINIT()          if (WM_CTRLINIT==uMsg) { s->OnInit(); } else
#define ON_WM_GETMINMAXINFO()     if (WM_GETMINMAXINFO==uMsg) { s->OnGetMinMaxInfo((MINMAXINFO*)lParam); } else
#define ON_WM_PAINT()             if (WM_PAINT==uMsg && c->redraw) { CDC dc[1]; dc->SelectObject((CBitmap*)wParam); s->OnPaint(dc); c->redraw = 0;} else
#define ON_WM_CONTEXTMENU()       if (WM_CONTEXTMENU==uMsg) { s->OnContextMenu(0, CPoint(lParam)); } else
#define ON_CONTROL(codeNotify, id, memberFxn)  if (WM_COMMAND==uMsg && id == LOWORD(wParam) && codeNotify==HIWORD(wParam)) { s->memberFxn(); } else

#define ON_COMMAND(id, memberFxn)  ON_CONTROL(CN_COMMAND, id, memberFxn)

// Static control notification codes
#define ON_STN_CLICKED(id, memberFxn) ON_CONTROL(STN_CLICKED, id, memberFxn)
#define ON_STN_DBLCLK(id, memberFxn) ON_CONTROL(STN_DBLCLK, id, memberFxn)
#define ON_STN_ENABLE(id, memberFxn) ON_CONTROL(STN_ENABLE, id, memberFxn)
#define ON_STN_DISABLE(id, memberFxn) ON_CONTROL(STN_DISABLE, id, memberFxn)


// Edit Control Notification Codes
#define ON_EN_SETFOCUS(id, memberFxn) ON_CONTROL(EN_SETFOCUS, id, memberFxn)
#define ON_EN_KILLFOCUS(id, memberFxn) ON_CONTROL(EN_KILLFOCUS, id, memberFxn)
#define ON_EN_CHANGE(id, memberFxn) ON_CONTROL(EN_CHANGE, id, memberFxn)
#define ON_EN_UPDATE(id, memberFxn) ON_CONTROL(EN_UPDATE, id, memberFxn)
#define ON_EN_ERRSPACE(id, memberFxn) ON_CONTROL(EN_ERRSPACE, id, memberFxn)
#define ON_EN_MAXTEXT(id, memberFxn) ON_CONTROL(EN_MAXTEXT, id, memberFxn)
#define ON_EN_HSCROLL(id, memberFxn) ON_CONTROL(EN_HSCROLL, id, memberFxn)
#define ON_EN_VSCROLL(id, memberFxn) ON_CONTROL(EN_VSCROLL, id, memberFxn)

// User Button Notification Codes
#define ON_BN_CLICKED(id, memberFxn) ON_CONTROL(BN_CLICKED, id, memberFxn)
#define ON_BN_DOUBLECLICKED(id, memberFxn) ON_CONTROL(BN_DOUBLECLICKED, id, memberFxn)
#define ON_BN_SETFOCUS(id, memberFxn) ON_CONTROL(BN_SETFOCUS, id, memberFxn)
#define ON_BN_KILLFOCUS(id, memberFxn) ON_CONTROL(BN_KILLFOCUS, id, memberFxn)

// old BS_USERBUTTON button notifications - obsolete in Win31
#define ON_BN_PAINT(id, memberFxn) ON_CONTROL(BN_PAINT, id, memberFxn)
#define ON_BN_HILITE(id, memberFxn) ON_CONTROL(BN_HILITE, id, memberFxn)
#define ON_BN_UNHILITE(id, memberFxn) ON_CONTROL(BN_UNHILITE, id, memberFxn)
#define ON_BN_DISABLE(id, memberFxn) ON_CONTROL(BN_DISABLE, id, memberFxn)

// Listbox Notification Codes
#define ON_LBN_ERRSPACE(id, memberFxn) ON_CONTROL(LBN_ERRSPACE, id, memberFxn)
#define ON_LBN_SELCHANGE(id, memberFxn) ON_CONTROL(LBN_SELCHANGE, id, memberFxn)
#define ON_LBN_DBLCLK(id, memberFxn) ON_CONTROL(LBN_DBLCLK, id, memberFxn)
#define ON_LBN_SELCANCEL(id, memberFxn) ON_CONTROL(LBN_SELCANCEL, id, memberFxn)
#define ON_LBN_SETFOCUS(id, memberFxn) ON_CONTROL(LBN_SETFOCUS, id, memberFxn)
#define ON_LBN_KILLFOCUS(id, memberFxn) ON_CONTROL(LBN_KILLFOCUS, id, memberFxn)

// Check Listbox Notification codes
#define CLBN_CHKCHANGE (40)
#define ON_CLBN_CHKCHANGE(id, memberFxn) ON_CONTROL(CLBN_CHKCHANGE, id, memberFxn)

// Combo Box Notification Codes
#define ON_CBN_ERRSPACE(id, memberFxn) ON_CONTROL(CBN_ERRSPACE, id, memberFxn)
#define ON_CBN_SELCHANGE(id, memberFxn) ON_CONTROL(CBN_SELCHANGE, id, memberFxn)
#define ON_CBN_DBLCLK(id, memberFxn) ON_CONTROL(CBN_DBLCLK, id, memberFxn)
#define ON_CBN_SETFOCUS(id, memberFxn) ON_CONTROL(CBN_SETFOCUS, id, memberFxn)
#define ON_CBN_KILLFOCUS(id, memberFxn) ON_CONTROL(CBN_KILLFOCUS, id, memberFxn)
#define ON_CBN_EDITCHANGE(id, memberFxn) ON_CONTROL(CBN_EDITCHANGE, id, memberFxn)
#define ON_CBN_EDITUPDATE(id, memberFxn) ON_CONTROL(CBN_EDITUPDATE, id, memberFxn)
#define ON_CBN_DROPDOWN(id, memberFxn) ON_CONTROL(CBN_DROPDOWN, id, memberFxn)
#define ON_CBN_CLOSEUP(id, memberFxn)  ON_CONTROL(CBN_CLOSEUP, id, memberFxn)
#define ON_CBN_SELENDOK(id, memberFxn)  ON_CONTROL(CBN_SELENDOK, id, memberFxn)
#define ON_CBN_SELENDCANCEL(id, memberFxn)  ON_CONTROL(CBN_SELENDCANCEL, id, memberFxn)


enum ImageAlignHorz {
  ImageAlignHorzLeft,
  ImageAlignHorzCenter,
  ImageAlignHorzRight,
  ImageAlignHorzStretch
};

enum ImageAlignVert {
  ImageAlignVertTop,
  ImageAlignVertCenter,
  ImageAlignVertBottom,
  ImageAlignVertStretch
};

// Styles for toolbar buttons
#define TBBS_BUTTON     MAKELONG(TBSTYLE_BUTTON, 0) // this entry is button
#define TBBS_SEPARATOR  MAKELONG(TBSTYLE_SEP, 0)    // this entry is a separator
#define TBBS_CHECKBOX   MAKELONG(TBSTYLE_CHECK, 0)  // this is an auto check button
#define TBBS_GROUP      MAKELONG(TBSTYLE_GROUP, 0)  // marks the start of a group
#define TBBS_CHECKGROUP (TBBS_GROUP|TBBS_CHECKBOX)  // normal use of TBBS_GROUP
#define TBBS_DROPDOWN   MAKELONG(TBSTYLE_DROPDOWN, 0) // drop down style
#define TBBS_AUTOSIZE   MAKELONG(TBSTYLE_AUTOSIZE, 0) // autocalc button width
#define TBBS_NOPREFIX   MAKELONG(TBSTYLE_NOPREFIX, 0) // no accel prefix for this button

// styles for display states
#define TBBS_CHECKED    MAKELONG(0, TBSTATE_CHECKED)    // button is checked/down
#define TBBS_PRESSED    MAKELONG(0, TBSTATE_PRESSED)    // button is being depressed
#define TBBS_DISABLED   MAKELONG(0, TBSTATE_ENABLED)    // button is disabled
#define TBBS_INDETERMINATE  MAKELONG(0, TBSTATE_INDETERMINATE)  // third state
#define TBBS_HIDDEN     MAKELONG(0, TBSTATE_HIDDEN) // button is hidden
#define TBBS_WRAPPED    MAKELONG(0, TBSTATE_WRAP)   // button is wrapped at this point
#define TBBS_ELLIPSES   MAKELONG(0, TBSTATE_ELIPSES)
#define TBBS_MARKED     MAKELONG(0, TBSTATE_MARKED)

// Styles for status bar panes
#define SBPS_NORMAL     0x0000
#define SBPS_NOBORDERS  SBT_NOBORDERS
#define SBPS_POPOUT     SBT_POPOUT
#define SBPS_OWNERDRAW  SBT_OWNERDRAW
#define SBPS_DISABLED   0x04000000
#define SBPS_STRETCH    0x08000000  // stretch to fill status bar

#define BCGP_CAPTION_BTN_PIN          0x1
#define BCGP_CAPTION_BTN_CLOSE        0x2
#define BCGP_CAPTION_BTN_MENU         0x4
#define BCGP_CAPTION_BTN_MAXIMIZE     0x8
#define BCGP_CAPTION_BTN_CUSTOMIZE    0x10

typedef enum BCGP_PREDOCK_STATE {
  BCGP_PDS_NOTHING,
  BCGP_PDS_DOCK_REGULAR,
  BCGP_PDS_DOCK_TO_TAB
};

enum BCGBUTTON_STATE {
  ButtonsIsRegular,
  ButtonsIsPressed,
  ButtonsIsHighlighted,
};

#define BCGP_PLANNER_RULE_DAILY   1
#define BCGP_PLANNER_RULE_WEEKLY  2
#define BCGP_PLANNER_RULE_MONTHLY 3
#define BCGP_PLANNER_RULE_YEARLY  4

#define BCGP_PLANNER_DRAW_APP_GRADIENT_FILL     0x0001
#define BCGP_PLANNER_DRAW_APP_ROUNDED_CORNERS   0x0002
#define BCGP_PLANNER_DRAW_APP_OVERRIDE_SELECTION  0x0004
#define BCGP_PLANNER_DRAW_APP_NO_MULTIDAY_CLOCKS  0x0008
#define BCGP_PLANNER_DRAW_APP_DURATION_SHAPE    0x0010
#define BCGP_PLANNER_DRAW_APP_NO_DURATION     0x0020
#define BCGP_PLANNER_DRAW_APP_NO_IMAGES       0x0040
#define BCGP_PLANNER_DRAW_APP_NO_RECURRENCE_IMAGE 0x0080
#define BCGP_PLANNER_DRAW_VIEW_NO_DURATION      0x0100
#define BCGP_PLANNER_DRAW_VIEW_WEEK_BAR         0x0200
#define BCGP_PLANNER_DRAW_VIEW_CAPTION_DAY_BOLD   0x0400
#define BCGP_PLANNER_DRAW_VIEW_CAPTION_DAY_COMPACT  0x0800

enum {
  BCGP_PLANNER_ID_SCROLL = 1,
  BCGP_PLANNER_ID_INPLACE = 2,
};

enum BCGP_PLANNER_TYPE {
  BCGP_PLANNER_TYPE_FIRST     = 0,
  BCGP_PLANNER_TYPE_DAY       = BCGP_PLANNER_TYPE_FIRST,
  BCGP_PLANNER_TYPE_WORK_WEEK = 1,
  BCGP_PLANNER_TYPE_WEEK      = 2,
  BCGP_PLANNER_TYPE_MONTH     = 3,
  BCGP_PLANNER_TYPE_MULTI     = 4,
  BCGP_PLANNER_TYPE_LAST      = BCGP_PLANNER_TYPE_MULTI
};

enum BCGP_PLANNER_CHANGE_OPERATION {
  BCGP_PLANNER_CHANGE_OPERATION_FIRST = 0,
  BCGP_PLANNER_CHANGE_OPERATION_NONE  = BCGP_PLANNER_CHANGE_OPERATION_FIRST,
  BCGP_PLANNER_CHANGE_OPERATION_CUT   = 1,
  BCGP_PLANNER_CHANGE_OPERATION_PASTE = 2,
  BCGP_PLANNER_CHANGE_OPERATION_DRAG  = 3,
  BCGP_PLANNER_CHANGE_OPERATION_DROP  = 4,
  BCGP_PLANNER_CHANGE_OPERATION_LAST  = BCGP_PLANNER_CHANGE_OPERATION_DROP
};

#define BCGPRIBBONBAR_HIDE_ELEMENTS 0x0001
#define BCGPRIBBONBAR_HIDE_ALL    0x0002

enum BCGPRibbonCategoryColor {
  BCGPCategoryColor_None,
  BCGPCategoryColor_Red,
  BCGPCategoryColor_Orange,
  BCGPCategoryColor_Yellow,
  BCGPCategoryColor_Green,
  BCGPCategoryColor_Blue,
  BCGPCategoryColor_Indigo,
  BCGPCategoryColor_Violet
};

enum RibbonElementLocation {
  RibbonElementNotInGroup,
  RibbonElementSingleInGroup,
  RibbonElementFirstInGroup,
  RibbonElementLastInGroup,
  RibbonElementMiddleInGroup,
};

enum RibbonImageType {
  RibbonImageLarge,
  RibbonImageSmall
};


struct CObject {
  int x;
};
struct CPrintPreviewState {
  int x;
};
typedef DWORD DROPEFFECT;
struct COleDataObject {
  COLOR x;
};
struct COleDataSource {
  COLOR x;
};
struct COleDropSource {
  COLOR x;
};
struct COleDropTarget {
  COLOR x;
};
struct CFile {
  int x;
};
struct CArchive {
  int x;
};
struct CDumpContext {
  int x;
};

struct CRuntimeClass {
  int x;
  CObject* CreateObject() {
    return 0;
  }
};
struct CPoint : public IPOINT {
  CPoint() {
    x = 0;
    y = 0;
  }
  CPoint(LPARAM lParam) {
    x = LOWORD(lParam), y = HIWORD(lParam);
  }
  CPoint(int w1, int h1) {
    x = w1;
    y = h1;
  }
  CPoint(IPOINT sz) {
    x = sz.x;
    y = sz.y;
  }
};
static int iSizeAdd(ISIZE* sz, int x, int y)
{
  sz->w += x;
  sz->h += y;
  return 0;
}
struct CSize : public ISIZE {
  CSize() {
    w = 0;
    h = 0;
  }
  CSize(int w1, int h1) {
    w = w1;
    h = h1;
  }
  CSize(ISIZE sz) {
    w = sz.w;
    h = sz.h;
  }
  CSize& operator += (const CSize& o) {
    w += o.w, h += o.h;
    return *this;
  }
  CSize(DWORD dwSize) {
    w = LOWORD(dwSize), h = HIWORD(dwSize);
  }

  // Operations
  BOOL operator==(ISIZE size) const {
    return w == size.w && h == size.h;
  }
  BOOL operator!=(ISIZE size) const;
  void operator+=(ISIZE size);
  void operator-=(ISIZE size);

  // Operators returning CSize values
  CSize operator+(ISIZE size) const;
  CSize operator-(ISIZE size) const;
  CSize operator-() const;

  // Operators returning CPoint values
  CPoint operator+(POINT point) const;
  CPoint operator-(POINT point) const;
};
struct CRect : public IRECT {
  CRect() {
    RCSET(this, 0, 0, 0, 0);
  }
  CRect(int l1, int t1, int r1, int b1) {
    RCSET(this, l1, t1, r1, b1);
  }
  // copy constructor
  CRect(const IRECT& srcRect) {
    *(IRECT*)this = srcRect;
  }
  // from a pointer to another rect
  CRect(const IRECT* lpSrcRect);
  // from a point and size
  CRect(IPOINT point, ISIZE size) {
    l = point.x, t = point.y;
    r = l + size.w, b = t + size.h;
  }
  // from two points
  CRect(IPOINT topLeft, IPOINT bottomRight);

  // Attributes (in addition to IRECT members)

  // retrieves the width
  int Width() const {
    return r - l;
  }
  // returns the height
  int Height() const {
    return b - t;
  }
  // returns the size
  CSize Size() const {
    return CSize(r - l, b - t);
  }
  // reference to the top-left point
  CPoint& TopLeft() {
    return *(CPoint*)&l;
  }
  // reference to the bottom-right point
  CPoint& BottomRight() {
    return *(CPoint*)&r;
  }
  // const reference to the top-left point
  const CPoint& TopLeft() const {
    return *(CPoint*)&l;
  }
  // const reference to the bottom-right point
  const CPoint& BottomRight() const {
    return *(CPoint*)&r;
  }
  // the geometric center point of the rectangle
  CPoint CenterPoint() const {
    return CPoint((l + r) >> 1, (t + b) >> 1);
  }
  // swap the left and right
  void SwapLeftRight();
  static void SwapLeftRight(IRECT* lpRect);

  // convert between CRect and IRECT*/const IRECT* (no need for &)
  operator IRECT* () {
    return this;
  }
  operator const IRECT* () const {
    return this;
  }

  // returns TRUE if rectangle has no area
  BOOL IsRectEmpty() const {
    return iRectIsEmpty(*this);
  }
  // returns TRUE if rectangle is at (0,0) and has no area
  BOOL IsRectNull() const {
    return l == 0 && t == 0 && r == 0 && b == 0;
  }
  // returns TRUE if point is within rectangle
  BOOL PtInRect(IPOINT point) const {
    return iPtInRect(this, point.x, point.y);
  }

  // Operations

  // set rectangle from left, top, right, and bottom
  void SetRect(int x1, int y1, int x2, int y2) {
    l = x1, t = y1, r = x2, b = y2;
    return ;
  }
  void SetRect(IPOINT topLeft, IPOINT bottomRight) {
    return ;
  }
  // empty the rectangle
  void SetRectEmpty() {
    l = t = r = b = 0;
  }
  // copy from another rectangle
  void CopyRect(const IRECT* lpSrcRect);
  // TRUE if exactly the same as another rectangle
  BOOL EqualRect(const IRECT* lpRect) const {
    const IRECT* pThis = this;
    return 0 == memcmp(pThis, lpRect, sizeof(IRECT));
  }

  // inflate rectangle's width and height without
  // moving its top or left
  void InflateRect(int x, int y) {
    InflateRect(x, y, x, y);
  }
  void InflateRect(ISIZE size);
  void InflateRect(const IRECT* lpRect) {
    InflateRect(lpRect->l, lpRect->t, lpRect->r, lpRect->b);
  }
  void InflateRect(int l, int t, int r, int b) {
    iRectInflate(this, l, t, r, b);
  }
  // deflate the rectangle's width and height without
  // moving its top or left
  void DeflateRect(int x, int y) {
    iRectDeflate(this, x, y, x, y);
  }
  void DeflateRect(ISIZE size);
  void DeflateRect(const IRECT* lpRect);
  void DeflateRect(int l, int t, int r, int b) {
    iRectDeflate(this, l, t, r, b);
  }

  // translate the rectangle by moving its top and left
  void OffsetRect(int x, int y) {
    iRectOffset(this, x, y, x, y);
  }
  void OffsetRect(ISIZE size);
  void OffsetRect(IPOINT point);
  void NormalizeRect();

  // set this rectangle to intersection of two others
  BOOL IntersectRect(const IRECT* lpRect1, const IRECT* lpRect2) {
    *this = iRectInter(*lpRect1, *lpRect2);
    return 0;
  }

  // set this rectangle to bounding union of two others
  BOOL UnionRect(const IRECT* lpRect1, const IRECT* lpRect2) {
    *this = iRectUnion(*lpRect1, *lpRect2);
    return 0;
  }

  // set this rectangle to minimum of two others
  BOOL SubtractRect(const IRECT* lpRectSrc1, const IRECT* lpRectSrc2);

  // Additional Operations
  void operator=(const IRECT& srcRect) {
    IRECT& me = *this;
    me = srcRect;
  }
  BOOL operator==(const IRECT& rect) const;
  BOOL operator!=(const IRECT& rect) const;
  void operator+=(IPOINT point);
  void operator+=(ISIZE size);
  void operator+=(const IRECT* lpRect);
  void operator-=(IPOINT point);
  void operator-=(ISIZE size);
  void operator-=(const IRECT* lpRect);
  void operator&=(const IRECT& rect);
  void operator|=(const IRECT& rect);

  // Operators returning CRect values
  CRect operator+(IPOINT point) const;
  CRect operator-(IPOINT point) const;
  CRect operator+(const IRECT* lpRect) const;
  CRect operator+(ISIZE size) const;
  CRect operator-(ISIZE size) const;
  CRect operator-(const IRECT* lpRect) const;
  CRect operator&(const IRECT& rect2) const;
  CRect operator|(const IRECT& rect2) const;
  CRect MulDiv(int nMultiplier, int nDivisor) const;
};
int iRectCmp(const IRECT* rc1, const IRECT* rc2)
{
  return memcmp(rc1, rc2, sizeof(IRECT));
}

template<typename TYPE>
struct CArray : public CObject {
public:
  typedef TYPE ARG_TYPE;
  // Construction
  CArray();

  // Attributes
  int GetSize() const;
  int GetUpperBound() const;
  void SetSize(int nNewSize, int nGrowBy = -1);

  // Operations
  // Clean up
  void FreeExtra();
  void RemoveAll();

  // Accessing elements
  TYPE GetAt(int nIndex) const;
  void SetAt(int nIndex, ARG_TYPE newElement);
  TYPE& ElementAt(int nIndex);

  // Direct Access to the element data (may return NULL)
  const TYPE* GetData() const;
  TYPE* GetData();

  // Potentially growing the array
  void SetAtGrow(int nIndex, ARG_TYPE newElement);
  int Add(ARG_TYPE newElement);
  int Append(const CArray& src);
  void Copy(const CArray& src);

  // overloaded operator helpers
  TYPE operator[](int nIndex) const;
  TYPE& operator[](int nIndex);

  // Operations that move elements around
  void InsertAt(int nIndex, ARG_TYPE newElement, int nCount = 1);
  void RemoveAt(int nIndex, int nCount = 1);
  void InsertAt(int nStartIndex, CArray* pNewArray);

  // Implementation
protected:
  TYPE* m_pData;   // the actual array of data
  int m_nSize;     // # of elements (upperBound - 1)
  int m_nMaxSize;  // max allocated
  int m_nGrowBy;   // grow amount

public:
  ~CArray();
  void Serialize(CArchive&);
};
struct CBitmap : public img_t {
  CBitmap() {
    img_t* im = this;
    memset(im, 0, sizeof(img_t));
  }
  BOOL IsValid() const {
    return tt.data != NULL;
  }
  BOOL LoadBitmap(LPCTSTR lpszResourceName);
  BOOL LoadBitmap(UINT nIDResource);
  BOOL LoadOEMBitmap(UINT nIDBitmap); // for OBM_/OCR_/OIC_
  BOOL LoadMappedBitmap(UINT nIDBitmap, UINT nFlags = 0,
      LPCOLORMAP lpColorMap = NULL, int nMapSize = 0);

  //LONG      biWidth; / 图像的宽度，单位是像素
  //LONG      biHeight; / 图像的高度，单位是像素
  //WORD      biPlanes; / 位平面数，必须是1，不用考虑
  //WORD      biBitCount;/ 指定颜色位数，1为二值，4为16色，8为256色，16、24、32为真彩色
  //DWORD     biCompression; / 指定是否压缩，有效的值为BI_RGB，BI_RLE8，BI_RLE4,BI_BITFIELDS
  BOOL CreateBitmap(int nWidth, int nHeight, UINT nPlanes, UINT nBitcount, const void* lpBits, const void* bmiColors = NULL) {
    int cn = (nBitcount > 8 || palette_is_color(1 << nBitcount, (uchar*)bmiColors)) ? 4 : 1;
    imsetsize(this, nWidth, nHeight, cn, 1);
    imsetbitmap(h, w, tt.data, s, c, (const uchar*)lpBits, -1, nBitcount, (const uchar*)bmiColors);
    return 1;
  }
  BOOL CreateBitmapIndirect(LPBITMAP lpBitmap);
  BOOL CreateCompatibleBitmap(void* pDC, int nWidth, int nHeight);
  BOOL CreateDiscardableBitmap(void* pDC, int nWidth, int nHeight);

  // Attributes
  int GetBitmap(BITMAP* pBitMap);

  // Operations
  DWORD SetBitmapBits(DWORD dwCount, const void* lpBits);
  DWORD GetBitmapBits(DWORD dwCount, LPVOID lpBits) const;
  CSize SetBitmapDimension(int nWidth, int nHeight);
  CSize GetBitmapDimension() const;
  CSize GetImageSize() const {
    return CSize(w, h);
  }
  int GetCount() const {
    return f;
  }
  int Clear() {
    imfree(this);
    return 0;
  }

  typedef CBitmap CImageList;
  BOOL Create(int cx, int cy, UINT nFlags, int nInitial, int nGrow);
  BOOL Create(UINT nBitmapID, int cx, int nGrow, COLORREF crMask);
  BOOL Create(LPCTSTR lpszBitmapID, int cx, int nGrow, COLORREF crMask);
  BOOL Create(CImageList& imagelist1, int nImage1, CImageList& imagelist2,
      int nImage2, int dx, int dy);
  BOOL Create(CImageList* pImageList);

};
typedef CBitmap CImageList;


struct CPalette {
public:
  COLOR m_Palette[256];
  int m_nCount;
  // Constructors
  CPalette();
  BOOL CreatePalette(COLOR* pPalette);

  // Attributes
  int GetEntryCount();
  UINT GetPaletteEntries(UINT nStartIndex, UINT nNumEntries,
      LPPALETTEENTRY lpPaletteColors) const;
  UINT SetPaletteEntries(UINT nStartIndex, UINT nNumEntries,
      LPPALETTEENTRY lpPaletteColors);

  // Operations
  void AnimatePalette(UINT nStartIndex, UINT nNumEntries,
      LPPALETTEENTRY lpPaletteColors);
  UINT GetNearestPaletteIndex(COLORREF crColor) const;
  BOOL ResizePalette(UINT nNumEntries);

};
struct CPen {
  COLOR m_crColor;
  double m_nWidth;
  int m_nPenStyle;
  CPen() {}
  CPen(int nPenStyle, int nWidth, COLORREF crColor);
  CPen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush,
      int nStyleCount = 0, const DWORD* lpStyle = NULL);
  BOOL CreatePen(int nPenStyle, int nWidth, COLORREF crColor) {
    m_nPenStyle = nPenStyle;
    m_nWidth = nWidth;
    m_crColor = crColor;
    return TRUE;
  }
  BOOL CreatePen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush,
      int nStyleCount = 0, const DWORD* lpStyle = NULL);
  BOOL CreatePenIndirect(LPLOGPEN lpLogPen);

  // Attributes
  operator HPEN() const;
  int GetLogPen(LOGPEN* pLogPen);
  int GetExtLogPen(EXTLOGPEN* pLogPen);
};
struct CRgn {
  // Constructors
#if 0

  CRgn();
  BOOL CreateRectRgn(int x1, int y1, int x2, int y2);
  BOOL CreateRectRgnIndirect(const IRECT* lpRect);
  BOOL CreateEllipticRgn(int x1, int y1, int x2, int y2);
  BOOL CreateEllipticRgnIndirect(const IRECT* lpRect);
  BOOL CreatePolygonRgn(IPOINT* lpPoints, int nCount, int nMode);
  BOOL CreatePolyPolygonRgn(IPOINT* lpPoints, LPINT lpPolyCounts,
      int nCount, int nPolyFillMode);
  BOOL CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3);
  BOOL CreateFromData(const XFORM* lpXForm, int nCount, const RGNDATA* pRgnData);

  // Operations
  void SetRectRgn(int x1, int y1, int x2, int y2);
  void SetRectRgn(const IRECT* lpRect);
  int CombineRgn(CRgn* pRgn1, CRgn* pRgn2, int nCombineMode);
  int CopyRgn(CRgn* pRgnSrc);
  BOOL EqualRgn(CRgn* pRgn) const;
  int OffsetRgn(int x, int y);
  int OffsetRgn(IPOINT point);
  int GetRgnBox(IRECT* lpRect) const;
  BOOL PtInRegion(int x, int y) const;
  BOOL PtInRegion(IPOINT point) const;
  BOOL RectInRegion(const IRECT* lpRect) const;
  int GetRegionData(LPRGNDATA lpRgnData, int nCount) const;

  // Implementation
  ~CRgn();
#endif
};
struct CBrush {
  COLOR m_crColor;
  BOOL m_Valid;
  CBitmap m_Pattern[1];
  int m_nIndex;
  int m_nType;
  enum { BRT_SOLID, BRT_HATCH, BRT_PATTERN,  };
  CBrush() {
    m_crColor = 0;
    m_Valid = 0;
  }
  ~CBrush() {
    imfree(m_Pattern);
  }
  CBrush(COLORREF crColor) {             // CreateSolidBrush
    CreateSolidBrush(crColor);
  }
  CBrush(int nIndex, COLORREF crColor) { // CreateHatchBrush
    CreateHatchBrush(nIndex, crColor);
  }
  CBrush(CBitmap* pBitmap) {          // CreatePatternBrush
    CreatePatternBrush(pBitmap);
  }
  BOOL IsValid() const {
    return m_Valid;
  }

  BOOL CreateSolidBrush(COLORREF crColor) {
    m_crColor = crColor;
    m_nType = BRT_SOLID;
    return TRUE;
  }
  BOOL CreateHatchBrush(int nIndex, COLORREF crColor) {
    m_nIndex = nIndex;
    m_crColor = crColor;
    m_nType = BRT_HATCH;
    return TRUE;
  }
  BOOL CreatePatternBrush(CBitmap* pBitmap) {
    imclone2(m_Pattern, pBitmap);
    m_nType = BRT_PATTERN;
    return TRUE;
  }

  // Attributes
  int GetLogBrush(LOGBRUSH* pLogBrush);
};
struct CString : public str_t {
  // constructs empty CString
  CString() {
    s = NULL, l = 0;
  }
  // copy constructor
  CString(const CString& stringSrc) {
    s = NULL, l = 0;
    str_setstr(this, stringSrc.s, stringSrc.l);
  }
  // from a single character
  CString(TCHAR ch, int nRepeat = 1) {
    s = NULL, l = 0;
    str_setsize(this, nRepeat);
    memset(s, ch, nRepeat);
  }
  // from an ANSI string (converts to TCHAR)
  CString(LPCSTR lpsz) {
    int len = strlen(lpsz);
    s = NULL, l = 0;
    str_setstr(this, lpsz, len);
  }
  // from a UNICODE string (converts to TCHAR)
  //CString(LPCWSTR lpsz);
  // subset of characters from an ANSI string (converts to TCHAR)
  CString(LPCSTR lpch, int nLength) {
    s = NULL, l = 0;
    str_setstr(this, lpch, nLength);
  }
  // subset of characters from a UNICODE string (converts to TCHAR)
  //CString(LPCWSTR lpch, int nLength);
  // from unsigned characters
  //CString(const unsigned char* psz);

  // Attributes & Operations

  // get data length
  int GetLength() const {
    return l;
  }
  // TRUE if zero length
  BOOL IsEmpty() const {
    return l > 0;
  }
  // clear contents to empty
  void Empty();

  // return single character at zero-based index
  TCHAR GetAt(int nIndex) const;
  // return single character at zero-based index
  TCHAR operator[](int nIndex) const {
    return s[nIndex];
  }
  // set a single character at zero-based index
  void SetAt(int nIndex, TCHAR ch);
  // return pointer to const string
  operator LPCTSTR() const {
    return s;
  }

  // overloaded assignment

  // ref-counted copy from another CString
  const CString& operator=(const CString& stringSrc) {
    str_setstr(this, stringSrc.s, stringSrc.l);
    return *this;
  }
  // set string content to single character
  const CString& operator=(TCHAR ch) {
    str_setstr(this, &ch, 1);
    return *this;
  }
#ifdef _UNICODE
  const CString& operator=(char ch);
#endif
  // copy string content from ANSI string (converts to TCHAR)
  const CString& operator=(LPCSTR lpsz) {
    str_setstr(this, lpsz, strlen(lpsz));
    return *this;
  }
  // copy string content from UNICODE string (converts to TCHAR)
  //const CString& operator=(LPCWSTR lpsz);
  // copy string content from unsigned chars
  //const CString& operator=(const unsigned char* psz);

  // string concatenation

  // concatenate from another CString
  const CString& operator+=(const CString& string);

  // concatenate a single character
  const CString& operator+=(TCHAR ch) {
    str_catc(this, ch);
    return *this;
  }
#ifdef _UNICODE
  // concatenate an ANSI character after converting it to TCHAR
  const CString& operator+=(char ch);
#endif
  // concatenate a UNICODE character after converting it to TCHAR
  const CString& operator+=(LPCTSTR lpsz) {
    str_cat(this, STR1(lpsz));
    return *this;
  }

  friend CString AFXAPI operator+(const CString& string1,
      const CString& string2);
  friend CString AFXAPI operator+(const CString& string, TCHAR ch);
  friend CString AFXAPI operator+(TCHAR ch, const CString& string);
#ifdef _UNICODE
  friend CString AFXAPI operator+(const CString& string, char ch);
  friend CString AFXAPI operator+(char ch, const CString& string);
#endif
  friend CString AFXAPI operator+(const CString& string, LPCTSTR lpsz);
  friend CString AFXAPI operator+(LPCTSTR lpsz, const CString& string);

  // string comparison

  // straight character comparison
  int Compare(LPCTSTR lpsz) const;
  // compare ignoring case
  int CompareNoCase(LPCTSTR lpsz) const;
  // NLS aware comparison, case sensitive
  int Collate(LPCTSTR lpsz) const;
  // NLS aware comparison, case insensitive
  int CollateNoCase(LPCTSTR lpsz) const;

  // simple sub-string extraction

  // return nCount characters starting at zero-based nFirst
  CString Mid(int nFirst, int nCount) const {
    nFirst = BOUND(nFirst, 0, l);
    nCount = BOUND(nCount, 0, l - nFirst);
    return CString(s + nFirst, nCount);
  }
  // return all characters starting at zero-based nFirst
  CString Mid(int nFirst) const {
    nFirst = BOUND(nFirst, 0, l);
    return CString(s + nFirst, l - nFirst);
  }
  // return first nCount characters in string
  CString Left(int nCount) const {
    nCount = BOUND(nCount, 0, l);
    return CString(s, nCount);
  }
  // return nCount characters from end of string
  CString Right(int nCount) const {
    nCount = BOUND(nCount, 0, l);
    return CString(s + l - nCount, nCount);
  }

  //  characters from beginning that are also in passed string
  CString SpanIncluding(LPCTSTR lpszCharSet) const;
  // characters from beginning that are not also in passed string
  CString SpanExcluding(LPCTSTR lpszCharSet) const;

  // upper/lower/reverse conversion

  // NLS aware conversion to uppercase
  void MakeUpper() {
    str_toupper(this);
  }
  // NLS aware conversion to lowercase
  void MakeLower();
  // reverse string right-to-left
  void MakeReverse();

  // trimming whitespace (either side)

  // remove whitespace starting from right edge
  void TrimRight() {
    str_trimright(this);
  }
  // remove whitespace starting from left side
  void TrimLeft() {
    str_trimleft(this);
  }

  // trimming anything (either side)

  // remove continuous occurrences of chTarget starting from right
  void TrimRight(TCHAR chTarget);
  // remove continuous occcurrences of characters in passed string,
  // starting from right
  void TrimRight(LPCTSTR lpszTargets);
  // remove continuous occurrences of chTarget starting from left
  void TrimLeft(TCHAR chTarget);
  // remove continuous occcurrences of characters in
  // passed string, starting from left
  void TrimLeft(LPCTSTR lpszTargets);

  // advanced manipulation

  // replace occurrences of chOld with chNew
  int Replace(TCHAR chOld, TCHAR chNew) {
    return str_replace_chr(this, chOld, chNew);
  }
  // replace occurrences of substring lpszOld with lpszNew;
  // empty lpszNew removes instances of lpszOld
  int Replace(LPCTSTR lpszOld, LPCTSTR lpszNew) {
    return str_replace_str(this, STR1(lpszOld), STR1(lpszNew));
  }
  // remove occurrences of chRemove
  int Remove(TCHAR chRemove) {
    return str_remove_chr(this, chRemove);
  }
  // insert character at zero-based index; concatenates
  // if index is past end of string
  int Insert(int nIndex, TCHAR ch);
  // insert substring at zero-based index; concatenates
  // if index is past end of string
  int Insert(int nIndex, LPCTSTR pstr);
  // delete nCount characters starting at zero-based index
  int Delete(int nIndex, int nCount = 1) {
    return str_del(this, nIndex, nCount);
  }

  // searching

  // find character starting at left, -1 if not found
  int Find(TCHAR ch) const {
    return str_find_chr(*this, ch);
  }
  // find character starting at right
  int ReverseFind(TCHAR ch) const;
  // find character starting at zero-based index and going right
  int Find(TCHAR ch, int nStart) const;
  // find first instance of any character in passed string
  int FindOneOf(LPCTSTR lpszCharSet) const;
  // find first instance of substring
  int Find(LPCTSTR lpszSub) const {
    return Find(lpszSub, 0);
  }
  // find first instance of substring starting at zero-based index
  int Find(LPCTSTR lpszSub, int nStart) const {
    str_t s1 = STR2(s + nStart, l - nStart);
    str_t s2 = STR1(lpszSub);
    int pos = str_find_str(s1, s2);
    return pos >= 0 ? pos + nStart : pos;
  }

  // simple formatting

  // printf-like formatting using passed string
  //void AFX_CDECL Format(LPCTSTR lpszFormat, ...);
  // printf-like formatting using referenced string resource
  //void AFX_CDECL Format(UINT nFormatID, ...);
  // printf-like formatting using variable arguments parameter
  void FormatV(LPCTSTR lpszFormat, va_list argList);

  // formatting for localization (uses FormatMessage API)

  // format using FormatMessage API on passed string
  void AFX_CDECL FormatMessage(LPCTSTR lpszFormat, ...);
  // format using FormatMessage API on referenced string resource
  void AFX_CDECL FormatMessage(UINT nFormatID, ...);

  // input and output
#ifdef _DEBUG
  friend CDumpContext& AFXAPI operator<<(CDumpContext& dc,
      const CString& string);
#endif
  friend CArchive& AFXAPI operator<<(CArchive& ar, const CString& string);
  friend CArchive& AFXAPI operator>>(CArchive& ar, CString& string);

  // load from string resource
  BOOL LoadString(UINT nID);

#ifndef _UNICODE
  // ANSI <-> OEM support (convert string in place)

  // convert string from ANSI to OEM in-place
  void AnsiToOem();
  // convert string from OEM to ANSI in-place
  void OemToAnsi();
#endif

#ifndef _AFX_NO_BSTR_SUPPORT
  // OLE BSTR support (use for OLE automation)
  // return a BSTR initialized with this CString's data
  BSTR AllocSysString() const;
  // reallocates the passed BSTR, copies content of this CString to it
  BSTR SetSysString(BSTR* pbstr) const;
#endif

  // Access to string implementation buffer as "C" character array

  // get pointer to modifiable buffer at least as long as nMinBufLength
  LPTSTR GetBuffer(int nMinBufLength);
  // release buffer, setting length to nNewLength (or to first nul if -1)
  void ReleaseBuffer(int nNewLength = -1);
  // get pointer to modifiable buffer exactly as long as nNewLength
  LPTSTR GetBufferSetLength(int nNewLength);
  // release memory allocated to but unused by string
  void FreeExtra();

  // Use LockBuffer/UnlockBuffer to turn refcounting off

  // turn refcounting back on
  LPTSTR LockBuffer();
  // turn refcounting off
  void UnlockBuffer();

  // Implementation
  ~CString() {
    str_free(this);
  }
  int GetAllocLength() const;

  // implementation helpers
  void Init();
  void AllocCopy(CString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const;
  void AllocBuffer(int nLen);
  void AssignCopy(int nSrcLen, LPCTSTR lpszSrcData);
  void ConcatCopy(int nSrc1Len, LPCTSTR lpszSrc1Data, int nSrc2Len, LPCTSTR lpszSrc2Data);
  void ConcatInPlace(int nSrcLen, LPCTSTR lpszSrcData);
  void CopyBeforeWrite();
  void AllocBeforeWrite(int nLen);
  void Release();
};

struct CStringList : public strv_t {
  // Construction
  CStringList();

  // Attributes (head and tail)
  // count of elements
  int GetCount() const {
    return n;
  }
  BOOL IsEmpty() const {
    return 0 == n;
  }

  // peek at head or tail
  CString& GetHead();
  CString GetHead() const;
  CString& GetTail();
  CString GetTail() const;

  // Operations
  // get head or tail (and remove it) - don't call on empty list!
  CString RemoveHead();
  CString RemoveTail();

  // add before head or after tail
  int AddHead(LPCTSTR newElement);
  int AddTail(LPCTSTR newElement);

  int AddHead(const CString& newElement);
  int AddTail(const CString& newElement);


  // add another list of elements before head or after tail
  void AddHead(CStringList* pNewList);
  void AddTail(CStringList* pNewList);

  // remove all elements
  void RemoveAll();

  // getting/modifying an element at a given position
  CString& GetAt(int position);
  CString GetAt(int position) const;
  void SetAt(int pos, LPCTSTR newElement);

  void SetAt(int pos, const CString& newElement);

  void RemoveAt(int position);

  // inserting before or after a given position
  int InsertBefore(int position, LPCTSTR newElement);
  int InsertAfter(int position, LPCTSTR newElement);

  int InsertBefore(int position, const CString& newElement);
  int InsertAfter(int position, const CString& newElement);


  // helper functions (note: O(n) speed)
  int Find(LPCTSTR searchValue, int startAfter = NULL) const;
  // defaults to starting at the HEAD
  // return NULL if not found
  int FindIndex(int nIndex) const;
  // get the 'nIndex'th element (may return NULL)

public:
  ~CStringList();

  void Serialize(CArchive&);
};

struct CBCGPLocalResource {
};

struct CFont : public font_t {
  BOOL IsValid() const {
    return user != NULL;
  }
};
ISIZE GetTextExtent(font_t* fo, const char* s)
{
  return font_text_size(fo, s, -1, 0);
}
struct CDC {
  CBitmap* im;
  CFont* fo;
  CPen* pen;
  CBrush* brush;
  CRgn* rgn;
  IRECT m_pclip[1];
  COLOR m_crText;
  COLOR m_crPen;
  COLOR m_crBack;
  int _x, _y;
  CDC() {
    memset(this, 0, sizeof(CDC));
    m_crText = rgb(0, 0, 0);
    m_crBack = 0;
    *m_pclip = iRECT(0, 0, 0, 0);
    _x = 0, _y = 0;
  }
  int GetClipBox(IRECT* pclip) {
    *pclip = *m_pclip;
    return 0;
  }
  int SetClipBox(const IRECT* pclip) {
    *m_pclip = *pclip;
    return 0;
  }
  COLORREF SetTextColor(COLORREF crColor) {
    return m_crText = crColor;
  }
  COLORREF SetBkColor(COLORREF crBkColor) {
    return m_crBack = crBkColor;
  }
  COLORREF GetBkColor() const {
    return m_crBack;
  }
  COLORREF GetTextColor() const {
    return m_crText;
  }
  CFont* SelectObject(CFont* fo_new) {
    CFont* fo_old = fo;
    fo = fo_new;
    return fo_old;
  }
  CFont* SelectObject(font_t* fo_new) {
    return SelectObject((CFont*)fo_new);
  }
  CPen* SelectObject(CPen* pen_new) {
    CPen* pen_old = pen;
    pen = pen_new;
    return pen_old;
  }
  CBrush* SelectObject(CBrush* brush_new) {
    CBrush* brush_old = brush;
    brush = brush_new;
    return brush_old;
  }
  CBitmap* SelectObject(CBitmap* im_new) {
    CBitmap* im_old = im;
    im = im_new;
    return im_old;
  }
  CRgn* SelectObject(CRgn* rgn_new) {
    CRgn* rgn_old = rgn;
    rgn = rgn_new;
    return rgn_old;
  }
  COLOR SelectObject(COLOR crPen) {
    COLOR clr_old = m_crPen;
    m_crPen = crPen;
    return clr_old;
  }
  CRgn* SelectClipRgn(CRgn* rgn_new) {
    return SelectObject(rgn_new);
  }
  CFont* SelectStockObject(int fo_index) {
    return SelectObject((CFont*)0);
  }

  // Clipping Functions
  int GetClipBox(IRECT* lpRect) const;
  BOOL PtVisible(int x, int y) const;
  BOOL PtVisible(POINT point) const;
  BOOL RectVisible(const IRECT* lpRect) const;
  int ExcludeClipRect(int x1, int y1, int x2, int y2) {
    return 0;
  }
  int ExcludeClipRect(const IRECT* lpRect) {
    return 0;
  }
  int IntersectClipRect(int x1, int y1, int x2, int y2);
  int IntersectClipRect(IRECT* lpRect);
  int OffsetClipRgn(int x, int y);
  int OffsetClipRgn(ISIZE size);
  int SelectClipRgn(CRgn* pRgn, int nMode);

  // Line-Output Functions
  CPoint GetCurrentPosition() const;
  CPoint MoveTo(int x, int y) {
    _x = x, _y = y;
    return CPoint(x, y);
  }
  CPoint MoveTo(IPOINT point) {
    return MoveTo(point.x, point.y);
  }
  BOOL LineTo(int x, int y) {
    imdraw_DrawLine(im, 0, _x, _y, x, y, m_crPen);
    return TRUE;
  }
  BOOL LineTo(IPOINT point) {
    return LineTo(point.x, point.y);
  }
  BOOL Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
  BOOL Arc(const IRECT* lpRect, IPOINT ptStart, IPOINT ptEnd);
  BOOL Polyline(IPOINT* lpPoints, int nCount) {
    imdraw_polyline(im, 0, lpPoints, &nCount, 1, m_crPen, 1);
    return TRUE;
  }

  BOOL AngleArc(int x, int y, int nRadius, float fStartAngle, float fSweepAngle);
  BOOL ArcTo(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
  BOOL ArcTo(const IRECT* lpRect, IPOINT ptStart, IPOINT ptEnd);
  int GetArcDirection() const;
  int SetArcDirection(int nArcDirection);

  BOOL PolyDraw(const IPOINT* lpPoints, const BYTE* lpTypes, int nCount);
  BOOL PolylineTo(const IPOINT* lpPoints, int nCount);
  BOOL PolyPolyline(const IPOINT* lpPoints,
      const DWORD* lpPolyPoints, int nCount);

  BOOL PolyBezier(const IPOINT* lpPoints, int nCount);
  BOOL PolyBezierTo(const IPOINT* lpPoints, int nCount);

  // Simple Drawing Functions
  void FillRect(const IRECT* lpRect, CBrush* pBrush);
  void FrameRect(const IRECT* lpRect, CBrush* pBrush);
  void InvertRect(const IRECT* lpRect);
  BOOL DrawIcon(int x, int y, CBitmap* hIcon);
  BOOL DrawIcon(IPOINT point, CBitmap* hIcon);

  BOOL DrawState(CPoint pt, CSize size, CBitmap* pBitmap, UINT nFlags, CBrush* pBrush = NULL) {
    //imdraw_image;
    return 0;
  }
  //BOOL DrawState(CPoint pt, CSize size, LPCTSTR lpszText, UINT nFlags, BOOL bPrefixText = TRUE, int nTextLen = 0, CBrush* pBrush = NULL);
  BOOL DrawState(CPoint pt, CSize size, DRAWSTATEPROC lpDrawProc, LPARAM lData, UINT nFlags, CBrush* pBrush = NULL);


  // Ellipse and Polygon Functions
  BOOL Chord(int x1, int y1, int x2, int y2, int x3, int y3,    int x4, int y4);
  BOOL Chord(const IRECT* lpRect, IPOINT ptStart, IPOINT ptEnd);
  void DrawFocusRect(const IRECT* lpRect) {
    imdraw_focus_rect(im, 0, *lpRect, rgb(0, 0, 0));
    return ;
  }
  BOOL Ellipse(int x1, int y1, int x2, int y2);
  BOOL Ellipse(const IRECT* lpRect, COLOR crFill) {
    //imdraw_ellipse1(im, );
    return TRUE;
  }
  BOOL Pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
  BOOL Pie(const IRECT* lpRect, IPOINT ptStart, IPOINT ptEnd);
  BOOL Polygon(IPOINT* lpPoints, int nCount, COLOR crFill) {
    imdraw_poly(im, 0, lpPoints, nCount, 1, crFill, 0, 0);
    return TRUE;
  }
  BOOL PolyPolygon(IPOINT* lpPoints, LPINT lpPolyCounts, int nCount);
  BOOL Rectangle(int x1, int y1, int x2, int y2);
  BOOL Rectangle(const IRECT* lpRect);
  BOOL RoundRect(int x1, int y1, int x2, int y2, int x3, int y3);
  BOOL RoundRect(const IRECT* lpRect, IPOINT point);

  // Bitmap Functions
  BOOL PatBlt(int x, int y, int nWidth, int nHeight, DWORD dwRop);
  //BOOL BitBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC,   int xSrc, int ySrc, DWORD dwRop);
  BOOL StretchBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC,   int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop);
  COLORREF GetPixel(int x, int y) const {
    return imdraw_GetPixel(im, 0, x, y);
  }
  COLORREF GetPixel(IPOINT point) const {
    return imdraw_GetPixel(im, 0, point.x, point.y);
  }
  COLORREF SetPixel(int x, int y, COLORREF crColor) {
    imdraw_SetPixel(im, 0, x, y, crColor);
    return 0;
  }
  COLORREF SetPixel(IPOINT point, COLORREF crColor) {
    imdraw_SetPixel(im, 0, point.x, point.y, crColor);
    return 0;
  }
  BOOL FloodFill(int x, int y, COLORREF crColor);
  BOOL ExtFloodFill(int x, int y, COLORREF crColor, UINT nFillType);
  BOOL MaskBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC,    int xSrc, int ySrc, CBitmap& maskBitmap, int xMask, int yMask,    DWORD dwRop);
  BOOL PlgBlt(IPOINT* lpPoint, CDC* pSrcDC, int xSrc, int ySrc,   int nWidth, int nHeight, CBitmap& maskBitmap, int xMask, int yMask);
  BOOL SetPixelV(int x, int y, COLORREF crColor);
  BOOL SetPixelV(IPOINT point, COLORREF crColor);

  // Text Functions
  BOOL TextOut(int x, int y, LPCTSTR lpszString, int nCount);
  BOOL TextOut(int x, int y, const CString& str);
  UINT GetTextFlag() {
    UINT flag = DT_VCENTER;

    if (TA_RIGHT & m_nFlags) {
      flag |= DT_RIGHT;
    }

    if (TA_CENTER & m_nFlags) {
      flag |= DT_CENTER;
    }

    if (TA_LEFT & m_nFlags) {
      flag |= DT_LEFT;
    }

    return flag;
  }
  BOOL ExtTextOut(int x, int y, UINT nOptions, const IRECT* lpRect, LPCTSTR lpszString, UINT nCount, LPINT lpDxWidths) {
    imdraw_text(im, m_pclip, *lpRect, lpszString, nCount, fo, m_crText, 0, GetTextFlag());
    return TRUE;
  }
  UINT m_nFlags;
  BOOL ExtTextOut(int x, int y, UINT nOptions, const IRECT* lpRect, const CString& str, LPINT lpDxWidths) {
    imdraw_text(im, m_pclip, *lpRect, str, str.GetLength(), fo, m_crText, 0, GetTextFlag());
    return TRUE;
  }
  CSize TabbedTextOut(int x, int y, LPCTSTR lpszString, int nCount,       int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin);
  CSize TabbedTextOut(int x, int y, const CString& str,       int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin);
  int DrawText(LPCTSTR lpszString, int nCount, IRECT* lpRect, UINT nFormat) {
    imdraw_text(im, lpRect, *lpRect, lpszString, nCount, fo, m_crText, m_crBack, nFormat);
    return 0;
  }
  int DrawText(const CString& str, IRECT* lpRect, UINT nFormat) {
    return DrawText(str.s, str.l, lpRect, nFormat);
  }
  CSize GetTextExtent(LPCTSTR lpszString, int nCount) const {
    return font_text_size(fo, lpszString, nCount, 0);
  }
  CSize GetTextExtent(const CString& str) const {
    return GetTextExtent(str, str.GetLength());
  }
  CSize GetOutputTextExtent(LPCTSTR lpszString, int nCount) const;
  CSize GetOutputTextExtent(const CString& str) const;
  CSize GetTabbedTextExtent(LPCTSTR lpszString, int nCount,   int nTabPositions, LPINT lpnTabStopPositions) const;
  CSize GetTabbedTextExtent(const CString& str,   int nTabPositions, LPINT lpnTabStopPositions) const;
  CSize GetOutputTabbedTextExtent(LPCTSTR lpszString, int nCount,   int nTabPositions, LPINT lpnTabStopPositions) const;
  CSize GetOutputTabbedTextExtent(const CString& str,   int nTabPositions, LPINT lpnTabStopPositions) const;
  BOOL GrayString(CBrush* pBrush,   BOOL (CALLBACK* lpfnOutput)(CDC*, LPARAM, int), LPARAM lpData,    int nCount, int x, int y, int nWidth, int nHeight);
  UINT GetTextAlign() const;
  UINT SetTextAlign(UINT nFlags) {
    return m_nFlags = nFlags;
  }
  int GetTextFace(int nCount, LPTSTR lpszFacename) const;
  int GetTextFace(CString& rString) const;
  BOOL GetTextMetrics(LPTEXTMETRIC lpMetrics) const;
  BOOL GetOutputTextMetrics(LPTEXTMETRIC lpMetrics) const;
  int SetTextJustification(int nBreakExtra, int nBreakCount);
  int GetTextCharacterExtra() const;
  int SetTextCharacterExtra(int nCharExtra);

  // Advanced Drawing
  BOOL DrawEdge(IRECT* lpRect, UINT nEdge, UINT nFlags) {
    return imdraw_edge(im, lpRect, nEdge, nFlags);
  }
  BOOL DrawFrameControl(IRECT* lpRect, UINT nType, UINT nState) {
    return TRUE;
  }

  // Scrolling Functions
  BOOL ScrollDC(int dx, int dy, const IRECT* lpRectScroll, const IRECT* lpRectClip,   CRgn* pRgnUpdate, IRECT* lpRectUpdate);

  // Font Functions
  BOOL GetCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const;
  BOOL GetOutputCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const;
  DWORD SetMapperFlags(DWORD dwFlag);
  CSize GetAspectRatioFilter() const;

  BOOL GetCharABCWidths(UINT nFirstChar, UINT nLastChar, LPABC lpabc) const;
  DWORD GetFontData(DWORD dwTable, DWORD dwOffset, LPVOID lpData, DWORD cbData) const;
  int GetKerningPairs(int nPairs, LPKERNINGPAIR lpkrnpair) const;
  UINT GetOutlineTextMetrics(UINT cbData, LPOUTLINETEXTMETRIC lpotm) const;
  DWORD GetGlyphOutline(UINT nChar, UINT nFormat, LPGLYPHMETRICS lpgm,    DWORD cbBuffer, LPVOID lpBuffer, const MAT2* lpmat2) const;

  BOOL GetCharABCWidths(UINT nFirstChar, UINT nLastChar,    LPABCFLOAT lpABCF) const;
  BOOL GetCharWidth(UINT nFirstChar, UINT nLastChar,    float* lpFloatBuffer) const;

  // Printer/Device Escape Functions
  int Escape(int nEscape, int nCount,   LPCSTR lpszInData, LPVOID lpOutData);
  int Escape(int nEscape, int nInputSize, LPCSTR lpszInputData,   int nOutputSize, LPSTR lpszOutputData);
  int DrawEscape(int nEscape, int nInputSize, LPCSTR lpszInputData);

  // Escape helpers
  int StartDoc(LPCTSTR lpszDocName);  // old Win3.0 version
  int StartDoc(LPDOCINFO lpDocInfo);
  int StartPage();
  int EndPage();
  int SetAbortProc(BOOL (CALLBACK* lpfn)(CDC*, int));
  int AbortDoc();
  int EndDoc();

  // Path Functions
  BOOL AbortPath();
  BOOL BeginPath();
  BOOL CloseFigure();
  BOOL EndPath();
  BOOL FillPath();
  BOOL FlattenPath();
  BOOL StrokeAndFillPath();
  BOOL StrokePath();
  BOOL WidenPath();
  float GetMiterLimit() const;
  BOOL SetMiterLimit(float fMiterLimit);
  int GetPath(IPOINT* lpPoints, LPBYTE lpTypes, int nCount) const;
  BOOL SelectClipPath(int nMode);

  // Misc Helper Functions
  static CBrush* PASCAL GetHalftoneBrush();
  void DrawDragRect(const IRECT* lpRect, ISIZE size,    const IRECT* lpRectLast, ISIZE sizeLast,    CBrush* pBrush = NULL, CBrush* pBrushLast = NULL);
  void FillSolidRect(const IRECT* lpRect, COLORREF clr) {
    imdraw_FillSolidRect(im, 0, lpRect, clr);
  }
  void FillSolidRect(int x, int y, int cx, int cy, COLORREF clr) {
    IRECT rc = iRECT2(x, y, cx, cy);
    FillSolidRect(&rc, clr);
  }
  void Draw3dRect(const IRECT* lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight, COLORREF clrFace = 0) {
    COLOR crLT = clrTopLeft;
    COLOR crRB = clrBottomRight;

    if (!clrFace) {
      clrFace = this->m_crBack;
    }

    imdraw_border(im, lpRect, *lpRect, clrFace, 1, &crLT, 1, &crLT, 1, &crRB, 1, &crRB);
  }
  void Draw3dRect(int x, int y, int cx, int cy, COLORREF clrTopLeft, COLORREF clrBottomRight, COLORREF clrFace = 0) {
    IRECT rc = iRECT2(x, y, cx, cy);
    Draw3dRect(&rc, clrTopLeft, clrBottomRight, clrFace);
  }

  int SetBkMode(int nBkMode) {
    return 0;
  }
  BOOL AlphaBlend(CDC*, int, int, int, int, CDC*, int, int, int, int, BLENDFUNCTION);
  BOOL TransparentBlt(CDC* hdcDest, int xDest, int yDest, int nDestWidth, int nDestHeight,
      CDC* hdcSrc, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight,
      UINT clrTransparent);

  //BOOL Draw(img_t* im, int x, int y,int iImageIndex,BOOL bHilite = FALSE, BOOL bDisabled = FALSE,BOOL bIndeterminate = FALSE,BOOL bShadow = FALSE,BOOL bInactive = FALSE,BYTE alphaSrc = 255) {
  //  return ;
  // }

};
struct CCriticalSection : public CRITICAL_SECTION {
  CCriticalSection() {
    InitializeCriticalSection(this);
  }
  // Operations
  BOOL Unlock() {
    LeaveCriticalSection(this);
    return TRUE;
  }
  BOOL Lock() {
    EnterCriticalSection(this);
    return TRUE;
  }

  ~CCriticalSection() {
    DeleteCriticalSection(this);
  }
};
typedef int POSITION;
template <typename TYPE>
struct CList {
  TYPE* ele;
  int n;
  CList() {
    ele = 0;
    n = 0;
  }
  // Attributes (head and tail)
  // count of elements
  int GetCount() const {
    return n;
  }
  BOOL IsEmpty() const {
    return n > 0;
  }

  // peek at head or tail
  TYPE& GetHead() {
    return *ele;
  }
  TYPE& GetTail() {
    return ele[n - 1];
  }

  // Operations
  // get head or tail (and remove it) - don't call on empty list !
  TYPE RemoveHead() {
    TYPE t = *ele;
    --n;
    memcpy(ele, ele + 1, n * sizeof(TYPE));
    return t;
  }
  TYPE RemoveTail() {
    --n;
    return ele[n];
  }

  // add before head or after tail
  int AddHead(const TYPE& newElement) {
    REALLOC(TYPE, ele, n + 1);
    mem_insert(&ele, &n, 0, &newElement, 1, sizeof(TYPE));
    return 0;
  }
  int AddTail(const TYPE& newElement) {
    mem_push_back(&ele, &n, &newElement, 1, sizeof(TYPE));
    return n - 1;
  }

  // add another list of elements before head or after tail
  void AddHead(CList* pNewList);
  void AddTail(CList* pNewList);

  // remove all elements
  void RemoveAll();

  // iteration
  int GetHeadPosition() const {
    return 0;
  }
  int GetTailPosition() const {
    return n - 1;
  }
  TYPE& GetNext(int& rPosition) {
    return ele[rPosition++];
  } // return *Position++
  TYPE GetNext(int& rPosition) const { // return *Position++
    return ele[rPosition++];
  }
  TYPE& GetPrev(int& rPosition) { // return *Position--
    return ele[rPosition--];
  }
  TYPE GetPrev(int& rPosition) const { // return *Position--
    return ele[rPosition--];
  }
  // getting/modifying an element at a given position
  TYPE& GetAt(int position) {
    return ele[rPosition];
  }
  TYPE GetAt(int position) const {
    return ele[rPosition];
  }
  void SetAt(int pos, const TYPE& newElement);
  void RemoveAt(int position);

  // inserting before or after a given position
  int InsertBefore(int position, const TYPE& newElement) {
    mem_insert(&ele, &n, position, &newElement, 1, sizeof(TYPE));
    return position;
  }
  int InsertAfter(int position, const TYPE& newElement) {
    mem_insert(&ele, &n, position + 1, &newElement, 1, sizeof(TYPE));
    return position;
  }

  // helper functions (note: O(n) speed)
  int Find(const TYPE& searchValue, int startAfter = 0) const {
    int i;

    for (i = startAfter; i < n; ++i) {
      if (ele[i] == searchValue) {
        return i;
      }
    }

    return -1;
  }
  // defaults to starting at the HEAD, return NULL if not found
  int FindIndex(int nIndex) const;
  // get the 'nIndex'th element (may return NULL)
};
typedef CList<CObject*> CObList;

template <typename KEY, typename VALUE>
struct CMap {
  typedef struct {
    KEY key;
    VALUE value;
  } elem_type;
  elem_type* ele;
  int n;
  // Attributes
  // number of elements
  int GetCount() const;
  BOOL IsEmpty() const {
    return n > 0;
  }

  // Lookup
  BOOL Lookup(const KEY& key, VALUE& value) const {
    int i;

    for (i = 0; i < n; ++i) {
      if (ele[i].key == key) {
        value = ele[i].value;
        return 1;
      }
    }

    return 0;
  }

  // Operations
  // Lookup and add if not there
  VALUE& operator[](const KEY& key);

  // add a new (key, value) pair
  void SetAt(const KEY& key, const VALUE& newValue);

  // removing existing (key, ?) pair
  BOOL RemoveKey(const KEY& key);
  void RemoveAll();

  // iterating all (key, value) pairs
  int GetStartPosition() const;
  void GetNextAssoc(int& rNextPosition, KEY& rKey, VALUE& rValue) const;

  // advanced features for derived classes
  UINT GetHashTableSize() const;
  void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);

};
struct CToolTipCtrl {
  int x;
};
struct CCreateContext {
  int x;
};
#if 0
struct CWnd {
  ctrl_t c[1];
  HWND m_hWnd;

  HWND GetSafeHwnd() const {
    return c->listener->hwnd;
  }
  DWORD GetStyle() const {
    return c->type;
  }
  DWORD GetExStyle() const {
    return 0;
  }
  BOOL ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);
  BOOL ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);

  HWND GetOwner() const {
    return c->listener->hwnd;
  }
  void SetOwner(CWnd* pOwnerWnd);

  // Constructors and other creation
  CWnd() {
    memset(c, 0, sizeof(ctrl_t));
  }

  static CWnd* PASCAL FromHandle(HWND hWnd);
  static CWnd* PASCAL FromHandlePermanent(HWND hWnd);
  static void PASCAL DeleteTempMap();
  BOOL Attach(HWND hWndNew);
  HWND Detach();

  // subclassing/unsubclassing functions
  void PreSubclassWindow();
  //BOOL SubclassWindow(HWND hWnd);
  BOOL SubclassDlgItem(UINT nID, CWnd* pParent);
  HWND UnsubclassWindow();

  // handling of RT_DLGINIT resource (extension to RT_DIALOG)
  BOOL ExecuteDlgInit(LPCTSTR lpszResourceName);
  BOOL ExecuteDlgInit(LPVOID lpResource);

  // Window Text Functions
  void SetWindowText(LPCTSTR lpszString);
  int GetWindowText(LPTSTR lpszStringBuf, int nMaxCount) const;
  void GetWindowText(CString& rString) const;
  int GetWindowTextLength() const;
  void SetFont(CFont* pFont, BOOL bRedraw = TRUE);
  CFont* GetFont() const;

  // CMenu Functions - non-Child windows only
  //CMenu* GetMenu() const;
  //BOOL SetMenu(CMenu* pMenu);
  //void DrawMenuBar();
  //CMenu* GetSystemMenu(BOOL bRevert) const;
  //BOOL HiliteMenuItem(CMenu* pMenu, UINT nIDHiliteItem, UINT nHilite);

  // Window Size and Position Functions
  BOOL IsIconic() const;
  BOOL IsZoomed() const;
  void MoveWindow(int x, int y, int nWidth, int nHeight,
      BOOL bRepaint = TRUE);
  void MoveWindow(const IRECT* lpRect, BOOL bRepaint = TRUE);
  int SetWindowRgn(HRGN hRgn, BOOL bRedraw);
  int GetWindowRgn(HRGN hRgn) const;

  BOOL SetWindowPos(const CWnd* pWndInsertAfter, int x, int y,
      int cx, int cy, UINT nFlags);
  UINT ArrangeIconicWindows();
  void BringWindowToTop();
  void GetWindowRect(IRECT* lpRect) const;
  void GetClientRect(IRECT* lpRect) const;

  BOOL GetWindowPlacement(WINDOWPLACEMENT* lpwndpl) const;
  BOOL SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl);

  // Coordinate Mapping Functions
  void ClientToScreen(IPOINT* lpPoint) const;
  void ClientToScreen(IRECT* lpRect) const;
  void ScreenToClient(IPOINT* lpPoint) const;
  void ScreenToClient(IRECT* lpRect) const;
  void MapWindowPoints(CWnd* pwndTo, IPOINT* lpPoint, UINT nCount) const;
  void MapWindowPoints(CWnd* pwndTo, IRECT* lpRect) const;

  // Update/Painting Functions
  CDC* BeginPaint(LPPAINTSTRUCT lpPaint);
  void EndPaint(LPPAINTSTRUCT lpPaint);
  CDC* GetDC();
  CDC* GetWindowDC();
  int ReleaseDC(CDC* pDC);
  void Print(CDC* pDC, DWORD dwFlags) const;
  void PrintClient(CDC* pDC, DWORD dwFlags) const;

  void UpdateWindow() {
    return ;
  }
  void SetRedraw(BOOL bRedraw = TRUE);
  BOOL GetUpdateRect(IRECT* lpRect, BOOL bErase = FALSE);
  int GetUpdateRgn(CRgn* pRgn, BOOL bErase = FALSE);
  void Invalidate(BOOL bErase = TRUE) {
    ::InvalidateRect(m_hWnd, 0, bErase);
    return ;
  }
  void InvalidateRect(const IRECT* lpRect, BOOL bErase = TRUE) {
    ::InvalidateRect(m_hWnd, 0, bErase);
    return ;
  }
  void InvalidateRgn(CRgn* pRgn, BOOL bErase = TRUE);
  void ValidateRect(const IRECT* lpRect);
  void ValidateRgn(CRgn* pRgn);
  BOOL ShowWindow(int nCmdShow);
  BOOL IsWindowVisible() const;
  void ShowOwnedPopups(BOOL bShow = TRUE);

  CDC* GetDCEx(CRgn* prgnClip, DWORD flags);
  BOOL LockWindowUpdate();    // for backward compatibility
  void UnlockWindowUpdate();
  BOOL RedrawWindow(const IRECT* lpRectUpdate = NULL, CRgn* prgnUpdate = NULL, UINT flags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE) {
    HWND hwnd = c->listener->hwnd;
    (c)->redraw = 1, ::InvalidateRect(hwnd, 0, 0);
    return 0;
  }
  BOOL EnableScrollBar(int nSBFlags, UINT nArrowFlags = ESB_ENABLE_BOTH);

  // Timer Functions
  UINT SetTimer(UINT nIDEvent, UINT nElapse,
      void (CALLBACK* lpfnTimer)(HWND, UINT, UINT, DWORD)) {
    return ::SetTimer(m_hWnd, nIDEvent, nElapse, lpfnTimer);
  }

  BOOL KillTimer(int nIDEvent) {
    return ::KillTimer(m_hWnd, nIDEvent);
  }

  // ToolTip Functions
  BOOL EnableToolTips(BOOL bEnable = TRUE);
  BOOL EnableTrackingToolTips(BOOL bEnable = TRUE);
  static void PASCAL CancelToolTips(BOOL bKeys = FALSE);
  void FilterToolTipMessage(MSG* pMsg);

  // for command hit testing (used for automatic tooltips)
  int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

  // Window State Functions
  BOOL IsWindowEnabled(c) const {
    return ((c->type & WS_DISABLED) == 0);
  }
  BOOL EnableWindow(BOOL bEnable = TRUE);

  // the active window applies only to top-level (frame windows)
  static CWnd* PASCAL GetActiveWindow();
  CWnd* SetActiveWindow();

  // the foreground window applies only to top-level windows (frame windows)
  BOOL SetForegroundWindow();
  static CWnd* PASCAL GetForegroundWindow();

  // capture and focus apply to all windows
  static CWnd* PASCAL GetCapture();
  HWND SetCapture() {
    return ::SetCapture(m_hWnd);
  }
  static CWnd* PASCAL GetFocus() {
    return 0;
  }
  CWnd* SetFocus();

  static CWnd* PASCAL GetDesktopWindow();

  // Obsolete and non-portable APIs - not recommended for new code
  void CloseWindow();
  BOOL OpenIcon();

  // Dialog-Box Item Functions
  // (NOTE: Dialog-Box Items/Controls are not necessarily in dialog boxes!)
  void CheckDlgButton(int nIDButton, UINT nCheck);
  void CheckRadioButton(int nIDFirstButton, int nIDLastButton,
      int nIDCheckButton);
  int GetCheckedRadioButton(int nIDFirstButton, int nIDLastButton);
  int DlgDirList(LPTSTR lpPathSpec, int nIDListBox,
      int nIDStaticPath, UINT nFileType);
  int DlgDirListComboBox(LPTSTR lpPathSpec, int nIDComboBox,
      int nIDStaticPath, UINT nFileType);
  BOOL DlgDirSelect(LPTSTR lpString, int nIDListBox);
  BOOL DlgDirSelectComboBox(LPTSTR lpString, int nIDComboBox);

  UINT GetDlgItemInt(int nID, BOOL* lpTrans = NULL,
      BOOL bSigned = TRUE) const;
  int GetDlgItemText(int nID, LPTSTR lpStr, int nMaxCount) const;
  int GetDlgItemText(int nID, CString& rString) const;
  CWnd* GetNextDlgGroupItem(CWnd* pWndCtl, BOOL bPrevious = FALSE) const;

  CWnd* GetNextDlgTabItem(CWnd* pWndCtl, BOOL bPrevious = FALSE) const;
  UINT IsDlgButtonChecked(int nIDButton) const;
  LRESULT SendDlgItemMessage(int nID, UINT message,
      WPARAM wParam = 0, LPARAM lParam = 0);
  void SetDlgItemInt(int nID, UINT nValue, BOOL bSigned = TRUE);
  void SetDlgItemText(int nID, LPCTSTR lpszString);

  // Scrolling Functions
  int GetScrollPos(int nBar) const;
  void GetScrollRange(int nBar, LPINT lpMinPos, LPINT lpMaxPos) const;
  void ScrollWindow(int xAmount, int yAmount,
      const IRECT* lpRect = NULL,
      const IRECT* lpClipRect = NULL);
  int SetScrollPos(int nBar, int nPos, BOOL bRedraw = TRUE);
  void SetScrollRange(int nBar, int nMinPos, int nMaxPos,
      BOOL bRedraw = TRUE);
  void ShowScrollBar(UINT nBar, BOOL bShow = TRUE);
  void EnableScrollBarCtrl(int nBar, BOOL bEnable = TRUE);
  // return sibling scrollbar control (or NULL if none)

  int ScrollWindowEx(int dx, int dy,
      const IRECT* lpRectScroll, const IRECT* lpRectClip,
      CRgn* prgnUpdate, IRECT* lpRectUpdate, UINT flags);
  BOOL SetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo,
      BOOL bRedraw = TRUE);
  BOOL GetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, UINT nMask = SIF_ALL);
  int GetScrollLimit(int nBar);

  // Window Access Functions
  CWnd* ChildWindowFromPoint(IPOINT point) const;
  CWnd* ChildWindowFromPoint(IPOINT point, UINT nFlags) const;
  static CWnd* PASCAL FindWindow(LPCTSTR lpszClassName, LPCTSTR lpszWindowName);
  //CWnd* GetNextWindow(UINT nFlag = GW_HWNDNEXT) const;
  CWnd* GetTopWindow() const;

  CWnd* GetWindow(UINT nCmd) const;
  CWnd* GetLastActivePopup() const;

  BOOL IsChild(const CWnd* pWnd) const;
  CWnd* GetParent() const;
  CWnd* SetParent(CWnd* pWndNewParent);
  static CWnd* PASCAL WindowFromPoint(IPOINT point);

  // Alert Functions
  BOOL FlashWindow(BOOL bInvert);
  int MessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = NULL,
      UINT nType = MB_OK);

  // Clipboard Functions
  BOOL ChangeClipboardChain(HWND hWndNext);
  HWND SetClipboardViewer();
  BOOL OpenClipboard();
  static CWnd* PASCAL GetClipboardOwner();
  static CWnd* PASCAL GetClipboardViewer();
  static CWnd* PASCAL GetOpenClipboardWindow();

  // Caret Functions
  void CreateCaret(CBitmap* pBitmap);
  void CreateSolidCaret(int nWidth, int nHeight);
  void CreateGrayCaret(int nWidth, int nHeight);
  static CPoint PASCAL GetCaretPos();
  static void PASCAL SetCaretPos(IPOINT point);
  void HideCaret();
  void ShowCaret();

  // Shell Interaction Functions
  void DragAcceptFiles(BOOL bAccept = TRUE);

  // Icon Functions
  CBitmap* SetIcon(CBitmap* hIcon, BOOL bBigIcon);
  CBitmap* GetIcon(BOOL bBigIcon) const;

  // Context Help Functions
  BOOL SetWindowContextHelpId(DWORD dwContextHelpId);
  DWORD GetWindowContextHelpId() const;

  // Dialog Data support
public:
  BOOL UpdateData(BOOL bSaveAndValidate = TRUE);
  // data wnd must be same type as this

  // Help Command Handlers
  afx_msg void OnHelp();          // F1 (uses current context)
  afx_msg void OnHelpIndex();     // ID_HELP_INDEX
  afx_msg void OnHelpFinder();    // ID_HELP_FINDER, ID_DEFAULT_HELP
  afx_msg void OnHelpUsing();     // ID_HELP_USING

  BOOL IsWindowVisible() {
    return (c->type & WS_VISIBLE) != 0;
  }
  img_t* GetIcon(BOOL bBigIcon) {
    return 0;
  }
  img_t* GetBarIcon(BOOL bBigIcon) {
    return CWnd::GetIcon(bBigIcon);
  }

  BOOL IsBarVisible() const {
    return ((c->type & WS_VISIBLE) != 0);
  }
  BOOL GetWindowRect(IRECT& rc) const {
    rc = c->rc;
    return 0;
  }
  BOOL GetClientRect(IRECT& rc) const {
    rc = c->rc;
    return 0;
  }
  //CWnd* GetOwner() const {    return 0;  }
  int IsFrameWnd() const {
    return 0;
  }
  int SendMessage(UINT Msg, WPARAM wParam, LPARAM lParam) {
    return c->ctrlproc(c, c->listener, c->hwnd, Msg, wParam, lParam);
  }
  int GetDlgCtrlID() {
    return (int)c;
  }
  BOOL ModifyStyle(int a, int b) {
    return 0;
  }
};
#endif

typedef ctrl_t CWnd;
typedef CWnd CFrameWnd;
typedef CWnd CMiniFrameWnd;
struct CMultiDocTemplate {};
BOOL IsWindowEnabled(ctrl_t* c)
{
  return ((c->style & WS_DISABLED) == 0);
}
#undef SendMessage
#define SendMessage(uMsg, wParam, lParam)  SendMessage1(c, uMsg, wParam, lParam)
BOOL SendMessage1(ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (c->listener && c->listener->ctrlproc) {
    return c->listener->ctrlproc(c, c->listener, c->listener->hwnd, uMsg, wParam, lParam);
  }

  return FALSE;
}
BOOL EnableWindow(ctrl_t* c, BOOL bEnable)
{
  SETBIT(c->style, WS_DISABLED, !bEnable);
  return TRUE;
}
BOOL SetWindowText(ctrl_t* c, const char* lpString)
{
  c->text = lpString;
  return TRUE;
}
#define UpdateWindow(c)  Invalidate(c)
#define RedrawWindow(c)  Invalidate(c)
BOOL Invalidate(ctrl_t* c)
{
  c->redraw = 1;

  if (c->listener) {
    //c->listener->redraw = 1;
    InvalidateRect(get_hwnd(c), 0, 0);
  }

  return 1;
}
struct AFX_CMDHANDLERINFO {
};

struct CUIntArray {
  uint x;
};

struct CGroupBox : public groupbox_t {
  MESSAGE_MAP_DEFAULT(CGroupBox, groupbox_proc);
};

struct CStatic : public lable_t {
  MESSAGE_MAP_DEFAULT(CStatic, lable_proc);
};

struct CButton : public button_t {
  // Constructors
public:
  MESSAGE_MAP_DEFAULT(CButton, button_proc);

  BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle,
      const IRECT& rect, CWnd* pParentWnd, UINT nID);

  // Attributes
  UINT GetButtonStyle() const;
  void SetButtonStyle(UINT nStyle, BOOL bRedraw = TRUE);

#if 0
  CBitmap* SetIcon(CBitmap* hIcon);
  CBitmap* GetIcon() const;
  CBitmap* SetBitmap(CBitmap* hBitmap);
  CBitmap* GetBitmap() const;
  CBitmap* SetCursor(CBitmap* hCursor);
  CBitmap* GetCursor();
#endif
};
struct CRadioBox : public rbox_t {
  DWORD m_nFlatStyle;
  img_t* m_Image;
  img_t* m_ImageHot;
  img_t* m_ImageDisabled;
  img_t* m_ImageChecked;
  img_t* m_ImageCheckedHot;
  img_t* m_ImageCheckedDisabled;
  
  MESSAGE_MAP_DEFAULT(CRadioBox, rbox_proc);
  UINT GetState() const;
  void SetState(BOOL bHighlight);
  int GetCount() const;
  int GetCurSel() const;
  int SetCurSel(int nSelect);
  void SetImage(img_t* hBitmap, img_t* hBitmapHot = NULL, img_t* hBitmapDisabled = NULL) {
    m_Image = hBitmap;
    m_ImageHot = hBitmapHot;
    m_ImageDisabled = hBitmapDisabled;
  }
  void SetCheckedImage(img_t* hBitmap, img_t* hBitmapHot = NULL, img_t* hBitmapDisabled = NULL) {
    m_ImageChecked = hBitmap;
    m_ImageCheckedHot = hBitmapHot;
    m_ImageCheckedDisabled = hBitmapDisabled;
  }
};
struct CCheckBox : public cbox_t {
  DWORD m_nFlatStyle;
  img_t* m_Image;
  img_t* m_ImageHot;
  img_t* m_ImageDisabled;
  img_t* m_ImageChecked;
  img_t* m_ImageCheckedHot;
  img_t* m_ImageCheckedDisabled;
  
  MESSAGE_MAP_DEFAULT(CCheckBox, cbox_proc);
  UINT GetState() const;
  void SetState(BOOL bHighlight);
  int GetCheck() const {
    return hit;
  }
  int SetCheck(int nCheck) {
    return hit = nCheck;
  }
  void SetImage(img_t* hBitmap, img_t* hBitmapHot = NULL, img_t* hBitmapDisabled = NULL) {
    m_Image = hBitmap;
    m_ImageHot = hBitmapHot;
    m_ImageDisabled = hBitmapDisabled;
  }
  void SetCheckedImage(img_t* hBitmap, img_t* hBitmapHot = NULL, img_t* hBitmapDisabled = NULL) {
    m_ImageChecked = hBitmap;
    m_ImageCheckedHot = hBitmapHot;
    m_ImageCheckedDisabled = hBitmapDisabled;
  }
};

struct CComboBox : public combobox_t {
  // Constructors
public:
  MESSAGE_MAP_DEFAULT(CComboBox, combobox_proc);
  BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

  // Attributes
  // for entire combo box
  int GetCount() const {
    return combobox_getcount(this);
  }
  int GetCurSel() const {
    return combobox_getcursel(this);
  }
  int SetCurSel(int nSelect) {
    return combobox_setcursel(this, nSelect);
  }
  LCID GetLocale() const;
  LCID SetLocale(LCID nNewLocale);
  // Win4
  int GetTopIndex() const;
  int SetTopIndex(int nIndex);
  int InitStorage(int nItems, UINT nBytes);
  void SetHorizontalExtent(UINT nExtent);
  UINT GetHorizontalExtent() const;
  int SetDroppedWidth(UINT nWidth);
  int GetDroppedWidth() const;

  // for edit control
  DWORD GetEditSel() const;
  BOOL LimitText(int nMaxChars);
  BOOL SetEditSel(int nStartChar, int nEndChar);

  // for combobox item
  DWORD GetItemData(int nIndex) const;
  int SetItemData(int nIndex, DWORD dwItemData);
  void* GetItemDataPtr(int nIndex) const;
  int SetItemDataPtr(int nIndex, void* pData);
  int GetLBText(int nIndex, LPTSTR lpszText) const;
  void GetLBText(int nIndex, CString& rString) const;
  int GetLBTextLen(int nIndex) const;

  int SetItemHeight(int nIndex, UINT cyItemHeight);
  int GetItemHeight(int nIndex) const;
  int FindStringExact(int nIndexStart, LPCTSTR lpszFind) const;
  int SetExtendedUI(BOOL bExtended = TRUE);
  BOOL GetExtendedUI() const;
  void GetDroppedControlRect(IRECT* lprect) const;
  BOOL GetDroppedState() const;

  // Operations
  // for drop-down combo boxes
  void ShowDropDown(BOOL bShowIt = TRUE);

  // manipulating listbox items
  int AddString(LPCTSTR lpszString);
  int DeleteString(UINT nIndex);
  int InsertString(int nIndex, LPCTSTR lpszString);
  void ResetContent();
  int Dir(UINT attr, LPCTSTR lpszWildCard);

  // selection helpers
  int FindString(int nStartAfter, LPCTSTR lpszString) const;
  int SelectString(int nStartAfter, LPCTSTR lpszString);

  // Clipboard operations
  void Clear();
  void Copy();
  void Cut();
  void Paste();

  // Overridables (must override draw, measure and compare for owner draw)
  void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
  void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
  int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
  void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);

};

struct CMenu {
  menuctrl_t menu0[4];
public:

  // Constructors
  CMenu() {memset(menu0, 0, sizeof(menu0));}
  ~CMenu() {}

  BOOL CreateMenu();
  BOOL CreatePopupMenu();
  BOOL LoadMenu(int n, menuitem_t* items, sys_t* sys) {
    menuctrl_set(menu0, menu0->c->listener, sys, n, items, 0);
    return TRUE;
  }
  BOOL LoadMenuIndirect(const void* lpMenuTemplate);
  BOOL DestroyMenu();

  // Attributes
  HMENU m_hMenu;          // must be first data member
  HMENU GetSafeHmenu() const;
  operator HMENU() const;

  static CMenu* PASCAL FromHandle(HMENU hMenu);
  static void PASCAL DeleteTempMap();
  BOOL Attach(HMENU hMenu);
  HMENU Detach();

  // CMenu Operations
  BOOL DeleteMenu(UINT nPosition, UINT nFlags);
  BOOL TrackPopupMenu(UINT nFlags, int x, int y,
      CWnd* pWnd, LPCRECT lpRect = 0);
  BOOL operator==(const CMenu& menu) const;
  BOOL operator!=(const CMenu& menu) const;

  // CMenuItem Operations
  BOOL AppendMenu(UINT nFlags, UINT nIDNewItem = 0,
      LPCTSTR lpszNewItem = NULL);
  BOOL AppendMenu(UINT nFlags, UINT nIDNewItem, const CBitmap* pBmp);
  UINT CheckMenuItem(UINT nIDCheckItem, UINT nCheck);
  UINT EnableMenuItem(UINT nIDEnableItem, UINT nEnable);
  UINT GetMenuItemCount() const;
  UINT GetMenuItemID(int nPos) const;
  UINT GetMenuState(UINT nID, UINT nFlags) const;
  int GetMenuString(UINT nIDItem, LPTSTR lpString, int nMaxCount,
      UINT nFlags) const;
  int GetMenuString(UINT nIDItem, CString& rString, UINT nFlags) const;
  BOOL GetMenuItemInfo(UINT nIDItem, LPMENUITEMINFO lpMenuItemInfo,
      BOOL fByPos = FALSE);
  CMenu* GetSubMenu(int nPos) const;
  BOOL InsertMenu(UINT nPosition, UINT nFlags, UINT nIDNewItem = 0,
      LPCTSTR lpszNewItem = NULL);
  BOOL InsertMenu(UINT nPosition, UINT nFlags, UINT nIDNewItem,
      const CBitmap* pBmp);
  BOOL ModifyMenu(UINT nPosition, UINT nFlags, UINT nIDNewItem = 0,
      LPCTSTR lpszNewItem = NULL);
  BOOL ModifyMenu(UINT nPosition, UINT nFlags, UINT nIDNewItem,
      const CBitmap* pBmp);
  BOOL RemoveMenu(UINT nPosition, UINT nFlags);
  BOOL SetMenuItemBitmaps(UINT nPosition, UINT nFlags,
      const CBitmap* pBmpUnchecked, const CBitmap* pBmpChecked);
  BOOL CheckMenuRadioItem(UINT nIDFirst, UINT nIDLast, UINT nIDItem, UINT nFlags);
  BOOL SetDefaultItem(UINT uItem, BOOL fByPos = FALSE);
  UINT GetDefaultItem(UINT gmdiFlags, BOOL fByPos = FALSE);

  // Context Help Functions
  BOOL SetMenuContextHelpId(DWORD dwContextHelpId);
  DWORD GetMenuContextHelpId() const;

  // Overridables (must override draw and measure for owner-draw menu items)
  void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
  void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
};

struct CEdit : public editctrl_t {
public:
  MESSAGE_MAP_DEFAULT(CEdit, editctrl_proc);
  BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

  CString GetText() const;
  // Attributes
  BOOL CanUndo() const;
  int GetLineCount() const;
  BOOL GetModify() const;
  void SetModify(BOOL bModified = TRUE);
  void GetRect(IRECT* lpRect) const;
  DWORD GetSel() const;
  void GetSel(int& nStartChar, int& nEndChar) const;
  HLOCAL GetHandle() const;
  void SetHandle(HLOCAL hBuffer);
#if (WINVER >= 0x400)
  void SetMargins(UINT nLeft, UINT nRight);
  DWORD GetMargins() const;
  void SetLimitText(UINT nMax);
  UINT GetLimitText() const;
  CPoint PosFromChar(UINT nChar) const;
  int CharFromPos(CPoint pt) const;
#endif

  // NOTE: first word in lpszBuffer must contain the size of the buffer!
  int GetLine(int nIndex, LPTSTR lpszBuffer) const;
  int GetLine(int nIndex, LPTSTR lpszBuffer, int nMaxLength) const;

  // Operations
  void EmptyUndoBuffer();
  BOOL FmtLines(BOOL bAddEOL);

  void LimitText(int nChars = 0);
  int LineFromChar(int nIndex = -1) const;
  int LineIndex(int nLine = -1) const;
  int LineLength(int nLine = -1) const;
  void LineScroll(int nLines, int nChars = 0);
  void ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo = FALSE);
  void SetPasswordChar(TCHAR ch);
  void SetRect(LPCRECT lpRect);
  void SetRectNP(LPCRECT lpRect);
  void SetSel(DWORD dwSelection, BOOL bNoScroll = FALSE);
  void SetSel(int nStartChar, int nEndChar, BOOL bNoScroll = FALSE);
  BOOL SetTabStops(int nTabStops, LPINT rgTabStops);
  void SetTabStops();
  BOOL SetTabStops(const int& cxEachStop);    // takes an 'int'

  // Clipboard operations
  BOOL Undo();
  void Clear();
  void Copy();
  void Cut();
  void Paste();

  BOOL SetReadOnly(BOOL bReadOnly = TRUE);
  int GetFirstVisibleLine() const;
  TCHAR GetPasswordChar() const;
};

struct CScrollBar : public scrollbar_t {
public:
  CScrollBar();
  ~CScrollBar();
  BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

  // Attributes
  int GetScrollPos() const;
  int SetScrollPos(int nPos, BOOL bRedraw = TRUE);
  void GetScrollRange(LPINT lpMinPos, LPINT lpMaxPos) const;
  void SetScrollRange(int nMinPos, int nMaxPos, BOOL bRedraw = TRUE);
  void ShowScrollBar(BOOL bShow = TRUE);

  BOOL EnableScrollBar(UINT nArrowFlags = ESB_ENABLE_BOTH);

  BOOL SetScrollInfo(LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE);
  BOOL GetScrollInfo(LPSCROLLINFO lpScrollInfo, UINT nMask = SIF_ALL);
  int GetScrollLimit();
};
struct CSpinButtonCtrl {
  // Implementation
public:
  ~CSpinButtonCtrl();
  // Constructors
public:
  CSpinButtonCtrl();
  BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

  // Attributes
  BOOL SetAccel(int nAccel, UDACCEL* pAccel);
  UINT GetAccel(int nAccel, UDACCEL* pAccel) const;
  int SetBase(int nBase);
  UINT GetBase() const;
  CWnd* SetBuddy(CWnd* pWndBuddy);
  CWnd* GetBuddy() const;
  int SetPos(int nPos);
  int GetPos() const;
  void SetRange(int nLower, int nUpper);
  void SetRange32(int nLower, int nUpper);
  DWORD GetRange() const;
  void GetRange(int& lower, int& upper) const;
  void GetRange32(int& lower, int& upper) const;

};
#define AFX_ABBREV_FILENAME_LEN 30

struct CRecentFileList {
  // Constructors
public:
  CRecentFileList(UINT nStart, LPCTSTR lpszSection,
      LPCTSTR lpszEntryFormat, int nSize,
      int nMaxDispLen = AFX_ABBREV_FILENAME_LEN);

  // Attributes
  int GetSize() const;
  CString& operator[](int nIndex);

  // Operations
  void Remove(int nIndex);
  void Add(LPCTSTR lpszPathName);
  BOOL GetDisplayName(CString& strName, int nIndex,
      LPCTSTR lpszCurDir, int nCurDir, BOOL bAtLeastName = TRUE) const;
  void UpdateMenu();
  void ReadList();    // reads from registry or ini file
  void WriteList();   // writes to registry or ini file

  // Implementation
  ~CRecentFileList();

  int m_nSize;                // contents of the MRU list
  CString* m_arrNames;
  CString m_strSectionName;   // for saving
  CString m_strEntryFormat;
  UINT m_nStart;              // for displaying
  int m_nMaxDisplayLength;
  CString m_strOriginal;      // original menu item contents
};

struct COleDateTime {
  enum DateTimeStatus {
    valid = 0,
    invalid = 1,    // Invalid date (out of range, etc.)
    null = 2,       // Literally has no value
  };
  DATE m_dt;
  DateTimeStatus m_status;
#define _AFXDISP_INLINE inline
  _AFXDISP_INLINE COleDateTime::COleDateTime() {
    m_dt = 0;
    SetStatus(valid);
  }
  _AFXDISP_INLINE COleDateTime::COleDateTime(const COleDateTime& dateSrc) {
    m_dt = dateSrc.m_dt;
    m_status = dateSrc.m_status;
  }
  _AFXDISP_INLINE COleDateTime::COleDateTime(const VARIANT& varSrc) {
    *this = varSrc;
  }
  _AFXDISP_INLINE COleDateTime::COleDateTime(DATE dtSrc) {
    m_dt = dtSrc;
    SetStatus(valid);
  }
  _AFXDISP_INLINE COleDateTime::COleDateTime(time_t timeSrc) {
    *this = timeSrc;
  }
  _AFXDISP_INLINE COleDateTime::COleDateTime(const SYSTEMTIME& systimeSrc) {
    *this = systimeSrc;
  }
  _AFXDISP_INLINE COleDateTime::COleDateTime(const FILETIME& filetimeSrc) {
    *this = filetimeSrc;
  }
  _AFXDISP_INLINE COleDateTime::COleDateTime(int nYear, int nMonth, int nDay,
      int nHour, int nMin, int nSec) {
    SetDateTime(nYear, nMonth, nDay, nHour, nMin, nSec);
  }
  _AFXDISP_INLINE COleDateTime::COleDateTime(WORD wDosDate, WORD wDosTime) {
    m_status = DosDateTimeToVariantTime(wDosDate, wDosTime, &m_dt) ?
        valid : invalid;
  }
  _AFXDISP_INLINE const COleDateTime& COleDateTime::operator=(const COleDateTime& dateSrc) {
    m_dt = dateSrc.m_dt;
    m_status = dateSrc.m_status;
    return *this;
  }
  _AFXDISP_INLINE COleDateTime::DateTimeStatus COleDateTime::GetStatus() const {
    return m_status;
  }
  _AFXDISP_INLINE void COleDateTime::SetStatus(DateTimeStatus status) {
    m_status = status;
  }
  _AFXDISP_INLINE BOOL COleDateTime::operator==(const COleDateTime& date) const {
    return (m_status == date.m_status && m_dt == date.m_dt);
  }
  _AFXDISP_INLINE BOOL COleDateTime::operator!=(const COleDateTime& date) const {
    return (m_status != date.m_status || m_dt != date.m_dt);
  }

  _AFXDISP_INLINE int COleDateTime::SetDate(int nYear, int nMonth, int nDay) {
    return SetDateTime(nYear, nMonth, nDay, 0, 0, 0);
  }
  _AFXDISP_INLINE int COleDateTime::SetTime(int nHour, int nMin, int nSec)
  // Set date to zero date - 12/30/1899
  {
    return SetDateTime(1899, 12, 30, nHour, nMin, nSec);
  }


  COleDateTime PASCAL COleDateTime::GetCurrentTime() {
    return COleDateTime(::time(NULL));
  }

  BOOL COleDateTime::GetAsSystemTime(SYSTEMTIME& sysTime) const {
    BOOL bRetVal = FALSE;

    if (GetStatus() == valid) {
      struct tm tmTemp;

      if (_AfxTmFromOleDate(m_dt, &tmTemp)) {
        sysTime.wYear = (WORD) tmTemp.tm_year;
        sysTime.wMonth = (WORD) tmTemp.tm_mon;
        sysTime.wDayOfWeek = (WORD)(tmTemp.tm_wday - 1);
        sysTime.wDay = (WORD) tmTemp.tm_mday;
        sysTime.wHour = (WORD) tmTemp.tm_hour;
        sysTime.wMinute = (WORD) tmTemp.tm_min;
        sysTime.wSecond = (WORD) tmTemp.tm_sec;
        sysTime.wMilliseconds = 0;

        bRetVal = TRUE;
      }
    }

    return bRetVal;
  }
#define AFX_OLE_DATETIME_ERROR -1
  int COleDateTime::GetYear() const {
    struct tm tmTemp;

    if (GetStatus() == valid && _AfxTmFromOleDate(m_dt, &tmTemp)) {
      return tmTemp.tm_year;
    }
    else {
      return AFX_OLE_DATETIME_ERROR;
    }
  }

  int COleDateTime::GetMonth() const {
    struct tm tmTemp;

    if (GetStatus() == valid && _AfxTmFromOleDate(m_dt, &tmTemp)) {
      return tmTemp.tm_mon;
    }
    else {
      return AFX_OLE_DATETIME_ERROR;
    }
  }

  int COleDateTime::GetDay() const {
    struct tm tmTemp;

    if (GetStatus() == valid && _AfxTmFromOleDate(m_dt, &tmTemp)) {
      return tmTemp.tm_mday;
    }
    else {
      return AFX_OLE_DATETIME_ERROR;
    }
  }

  int COleDateTime::GetHour() const {
    struct tm tmTemp;

    if (GetStatus() == valid && _AfxTmFromOleDate(m_dt, &tmTemp)) {
      return tmTemp.tm_hour;
    }
    else {
      return AFX_OLE_DATETIME_ERROR;
    }
  }

  int COleDateTime::GetMinute() const {
    struct tm tmTemp;

    if (GetStatus() == valid && _AfxTmFromOleDate(m_dt, &tmTemp)) {
      return tmTemp.tm_min;
    }
    else {
      return AFX_OLE_DATETIME_ERROR;
    }
  }

  int COleDateTime::GetSecond() const {
    struct tm tmTemp;

    if (GetStatus() == valid && _AfxTmFromOleDate(m_dt, &tmTemp)) {
      return tmTemp.tm_sec;
    }
    else {
      return AFX_OLE_DATETIME_ERROR;
    }
  }

  int COleDateTime::GetDayOfWeek() const {
    struct tm tmTemp;

    if (GetStatus() == valid && _AfxTmFromOleDate(m_dt, &tmTemp)) {
      return tmTemp.tm_wday;
    }
    else {
      return AFX_OLE_DATETIME_ERROR;
    }
  }

  int COleDateTime::GetDayOfYear() const {
    struct tm tmTemp;

    if (GetStatus() == valid && _AfxTmFromOleDate(m_dt, &tmTemp)) {
      return tmTemp.tm_yday;
    }
    else {
      return AFX_OLE_DATETIME_ERROR;
    }
  }

  const COleDateTime& COleDateTime::operator=(DATE dtSrc) {
    m_dt = dtSrc;
    SetStatus(valid);

    return *this;
  }

  const COleDateTime& COleDateTime::operator=(const time_t& timeSrc) {
    // Convert time_t to struct tm
    tm* ptm = localtime(&timeSrc);

    if (ptm != NULL) {
      m_status = _AfxOleDateFromTm((WORD)(ptm->tm_year + 1900),
          (WORD)(ptm->tm_mon + 1), (WORD)ptm->tm_mday,
          (WORD)ptm->tm_hour, (WORD)ptm->tm_min,
          (WORD)ptm->tm_sec, &m_dt) ? valid : invalid;
    }
    else {
      // Local time must have failed (timsSrc before 1/1/70 12am)
      SetStatus(invalid);
      ASSERT(FALSE);
    }

    return *this;
  }

  const COleDateTime& COleDateTime::operator=(const SYSTEMTIME& systimeSrc) {
    m_status = _AfxOleDateFromTm(systimeSrc.wYear, systimeSrc.wMonth,
        systimeSrc.wDay, systimeSrc.wHour, systimeSrc.wMinute,
        systimeSrc.wSecond, &m_dt) ? valid : invalid;

    return *this;
  }

  const COleDateTime& COleDateTime::operator=(const FILETIME& filetimeSrc) {
    // Assume UTC FILETIME, so convert to LOCALTIME
    FILETIME filetimeLocal;

    if (!FileTimeToLocalFileTime(&filetimeSrc, &filetimeLocal)) {
#ifdef _DEBUG
      DWORD dwError = GetLastError();
      TRACE("\nFileTimeToLocalFileTime failed. Error = %lu.\n\t", dwError);
#endif // _DEBUG
      m_status = invalid;
    }
    else {
      // Take advantage of SYSTEMTIME -> FILETIME conversion
      SYSTEMTIME systime;
      m_status = FileTimeToSystemTime(&filetimeLocal, &systime) ?
          valid : invalid;

      // At this point systime should always be valid, but...
      if (GetStatus() == valid) {
        m_status = _AfxOleDateFromTm(systime.wYear, systime.wMonth,
            systime.wDay, systime.wHour, systime.wMinute,
            systime.wSecond, &m_dt) ? valid : invalid;
      }
    }

    return *this;
  }

  BOOL COleDateTime::operator<(const COleDateTime& date) const {
    ASSERT(GetStatus() == valid);
    ASSERT(date.GetStatus() == valid);

    // Handle negative dates
    return _AfxDoubleFromDate(m_dt) < _AfxDoubleFromDate(date.m_dt);
  }

  BOOL COleDateTime::operator>(const COleDateTime& date) const {
    ASSERT(GetStatus() == valid);
    ASSERT(date.GetStatus() == valid);

    // Handle negative dates
    return _AfxDoubleFromDate(m_dt) > _AfxDoubleFromDate(date.m_dt);
  }

  BOOL COleDateTime::operator<=(const COleDateTime& date) const {
    ASSERT(GetStatus() == valid);
    ASSERT(date.GetStatus() == valid);

    // Handle negative dates
    return _AfxDoubleFromDate(m_dt) <= _AfxDoubleFromDate(date.m_dt);
  }

  BOOL COleDateTime::operator>=(const COleDateTime& date) const {
    ASSERT(GetStatus() == valid);
    ASSERT(date.GetStatus() == valid);

    // Handle negative dates
    return _AfxDoubleFromDate(m_dt) >= _AfxDoubleFromDate(date.m_dt);
  }

  int COleDateTime::SetDateTime(int nYear, int nMonth, int nDay,
      int nHour, int nMin, int nSec) {
    return m_status = _AfxOleDateFromTm((WORD)nYear, (WORD)nMonth,
        (WORD)nDay, (WORD)nHour, (WORD)nMin, (WORD)nSec, &m_dt) ?
        valid : invalid;
  }

  CString COleDateTime::Format(LPCTSTR pFormat) const {
    CString strDate;
    struct tm tmTemp;

    // If null, return empty string
    if (GetStatus() == null) {
      return strDate;
    }

    // If invalid, return DateTime resource string
    if (GetStatus() == invalid || !_AfxTmFromOleDate(m_dt, &tmTemp)) {
      VERIFY(strDate.LoadString(AFX_IDS_INVALID_DATETIME));
      return strDate;
    }

    // Convert tm from afx internal format to standard format
    _AfxTmConvertToStandardFormat(&tmTemp);

    // Fill in the buffer, disregard return value as it's not necessary
    LPTSTR lpszTemp = strDate.GetBufferSetLength(MAX_TIME_BUFFER_SIZE);
    _tcsftime(lpszTemp, strDate.GetLength(), pFormat, &tmTemp);
    strDate.ReleaseBuffer();

    return strDate;
  }

  CString COleDateTime::Format(UINT nFormatID) const {
    CString strFormat;
    VERIFY(strFormat.LoadString(nFormatID) != 0);
    return Format(strFormat);
  }

  void COleDateTime::CheckRange() {
    if (m_dt > MAX_DATE || m_dt < MIN_DATE) { // about year 100 to about 9999
      SetStatus(invalid);
    }
  }

};


#define AFX_OLE_DATETIME_HALFSECOND (1.0 / (2.0 * (60.0 * 60.0 * 24.0)))

struct COleDateTimeSpan {
  enum DateTimeSpanStatus {
    valid = 0,
    invalid = 1,    // Invalid span (out of range, etc.)
    null = 2,       // Literally has no value
  };

  double m_span;
  DateTimeSpanStatus m_status;

#define MAX_DAYS_IN_SPAN    3615897L

  // COleDateTimeSpan
  _AFXDISP_INLINE COleDateTimeSpan::COleDateTimeSpan() {
    m_span = 0;
    SetStatus(valid);
  }
  _AFXDISP_INLINE COleDateTimeSpan::COleDateTimeSpan(double dblSpanSrc) {
    m_span = dblSpanSrc;
    SetStatus(valid);
  }
  _AFXDISP_INLINE COleDateTimeSpan::COleDateTimeSpan(
      const COleDateTimeSpan& dateSpanSrc) {
    m_span = dateSpanSrc.m_span;
    m_status = dateSpanSrc.m_status;
  }
  _AFXDISP_INLINE COleDateTimeSpan::COleDateTimeSpan(
      long lDays, int nHours, int nMins, int nSecs) {
    SetDateTimeSpan(lDays, nHours, nMins, nSecs);
  }
  _AFXDISP_INLINE COleDateTimeSpan::DateTimeSpanStatus COleDateTimeSpan::GetStatus() const {
    return m_status;
  }
  _AFXDISP_INLINE void COleDateTimeSpan::SetStatus(DateTimeSpanStatus status) {
    m_status = status;
  }
  _AFXDISP_INLINE double COleDateTimeSpan::GetTotalDays() const {
    ASSERT(GetStatus() == valid);
    return m_span;
  }
  _AFXDISP_INLINE double COleDateTimeSpan::GetTotalHours() const {
    ASSERT(GetStatus() == valid);
    long lReturns = (long)(m_span * 24 + AFX_OLE_DATETIME_HALFSECOND);
    return lReturns;
  }
  _AFXDISP_INLINE double COleDateTimeSpan::GetTotalMinutes() const {
    ASSERT(GetStatus() == valid);
    long lReturns = (long)(m_span * 24 * 60 + AFX_OLE_DATETIME_HALFSECOND);
    return lReturns;
  }
  _AFXDISP_INLINE double COleDateTimeSpan::GetTotalSeconds() const {
    ASSERT(GetStatus() == valid);
    long lReturns = (long)(m_span * 24 * 60 * 60 + AFX_OLE_DATETIME_HALFSECOND);
    return lReturns;
  }

  _AFXDISP_INLINE long COleDateTimeSpan::GetDays() const {
    ASSERT(GetStatus() == valid);
    return (long)m_span;
  }
  _AFXDISP_INLINE BOOL COleDateTimeSpan::operator==(
      const COleDateTimeSpan& dateSpan) const {
    return (m_status == dateSpan.m_status &&
        m_span == dateSpan.m_span);
  }
  _AFXDISP_INLINE BOOL COleDateTimeSpan::operator!=(
      const COleDateTimeSpan& dateSpan) const {
    return (m_status != dateSpan.m_status ||
        m_span != dateSpan.m_span);
  }
  _AFXDISP_INLINE BOOL COleDateTimeSpan::operator<(
      const COleDateTimeSpan& dateSpan) const {
    ASSERT(GetStatus() == valid);
    ASSERT(dateSpan.GetStatus() == valid);
    return m_span < dateSpan.m_span;
  }
  _AFXDISP_INLINE BOOL COleDateTimeSpan::operator>(
      const COleDateTimeSpan& dateSpan) const {
    ASSERT(GetStatus() == valid);
    ASSERT(dateSpan.GetStatus() == valid);
    return m_span > dateSpan.m_span;
  }
  _AFXDISP_INLINE BOOL COleDateTimeSpan::operator<=(
      const COleDateTimeSpan& dateSpan) const {
    ASSERT(GetStatus() == valid);
    ASSERT(dateSpan.GetStatus() == valid);
    return m_span <= dateSpan.m_span;
  }
  _AFXDISP_INLINE BOOL COleDateTimeSpan::operator>=(
      const COleDateTimeSpan& dateSpan) const {
    ASSERT(GetStatus() == valid);
    ASSERT(dateSpan.GetStatus() == valid);
    return m_span >= dateSpan.m_span;
  }
  _AFXDISP_INLINE const COleDateTimeSpan& COleDateTimeSpan::operator+=(
      const COleDateTimeSpan dateSpan) {
    *this = *this + dateSpan;
    return *this;
  }
  _AFXDISP_INLINE const COleDateTimeSpan& COleDateTimeSpan::operator-=(
      const COleDateTimeSpan dateSpan) {
    *this = *this - dateSpan;
    return *this;
  }
  _AFXDISP_INLINE COleDateTimeSpan COleDateTimeSpan::operator-() const {
    return -this->m_span;
  }

  /////////////////////////////////////////////////////////////////////////////
  // COleDateTimeSpan struct
  long COleDateTimeSpan::GetHours() const {
    ASSERT(GetStatus() == valid);

    double dblTemp;

    // Truncate days and scale up
    dblTemp = modf(m_span, &dblTemp);

    long lReturns = (long)((dblTemp + AFX_OLE_DATETIME_HALFSECOND) * 24);

    if (lReturns >= 24) {
      lReturns -= 24;
    }

    return lReturns;
  }

  long COleDateTimeSpan::GetMinutes() const {
    ASSERT(GetStatus() == valid);

    double dblTemp;

    // Truncate hours and scale up
    dblTemp = modf(m_span * 24, &dblTemp);

    long lReturns = (long)((dblTemp + AFX_OLE_DATETIME_HALFSECOND) * 60);

    if (lReturns >= 60) {
      lReturns -= 60;
    }

    return lReturns;
  }

  long COleDateTimeSpan::GetSeconds() const {
    ASSERT(GetStatus() == valid);

    double dblTemp;

    // Truncate minutes and scale up
    dblTemp = modf(m_span * 24 * 60, &dblTemp);

    long lReturns = (long)((dblTemp + AFX_OLE_DATETIME_HALFSECOND) * 60);

    if (lReturns >= 60) {
      lReturns -= 60;
    }

    return lReturns;
  }

  const COleDateTimeSpan& COleDateTimeSpan::operator=(double dblSpanSrc) {
    m_span = dblSpanSrc;
    SetStatus(valid);
    return *this;
  }

  const COleDateTimeSpan& COleDateTimeSpan::operator=(const COleDateTimeSpan& dateSpanSrc) {
    m_span = dateSpanSrc.m_span;
    m_status = dateSpanSrc.m_status;
    return *this;
  }

  COleDateTimeSpan COleDateTimeSpan::operator+(const COleDateTimeSpan& dateSpan) const {
    COleDateTimeSpan dateSpanTemp;

    // If either operand Null, result Null
    if (GetStatus() == null || dateSpan.GetStatus() == null) {
      dateSpanTemp.SetStatus(null);
      return dateSpanTemp;
    }

    // If either operand Invalid, result Invalid
    if (GetStatus() == invalid || dateSpan.GetStatus() == invalid) {
      dateSpanTemp.SetStatus(invalid);
      return dateSpanTemp;
    }

    // Add spans and validate within legal range
    dateSpanTemp.m_span = m_span + dateSpan.m_span;
    dateSpanTemp.CheckRange();

    return dateSpanTemp;
  }

  COleDateTimeSpan COleDateTimeSpan::operator-(const COleDateTimeSpan& dateSpan) const {
    COleDateTimeSpan dateSpanTemp;

    // If either operand Null, result Null
    if (GetStatus() == null || dateSpan.GetStatus() == null) {
      dateSpanTemp.SetStatus(null);
      return dateSpanTemp;
    }

    // If either operand Invalid, result Invalid
    if (GetStatus() == invalid || dateSpan.GetStatus() == invalid) {
      dateSpanTemp.SetStatus(invalid);
      return dateSpanTemp;
    }

    // Subtract spans and validate within legal range
    dateSpanTemp.m_span = m_span - dateSpan.m_span;
    dateSpanTemp.CheckRange();

    return dateSpanTemp;
  }

  void COleDateTimeSpan::SetDateTimeSpan(
      long lDays, int nHours, int nMins, int nSecs) {
    // Set date span by breaking into fractional days (all input ranges valid)
    m_span = lDays + ((double)nHours) / 24 + ((double)nMins) / (24 * 60) +
        ((double)nSecs) / (24 * 60 * 60);

    SetStatus(valid);
  }

  CString COleDateTimeSpan::Format(LPCTSTR pFormat) const {
    CString strSpan;
    struct tm tmTemp;

    // If null, return empty string
    if (GetStatus() == null) {
      return strSpan;
    }

    // If invalid, return DateTimeSpan resource string
    if (GetStatus() == invalid || !_AfxTmFromOleDate(m_span, &tmTemp)) {
      VERIFY(strSpan.LoadString(AFX_IDS_INVALID_DATETIMESPAN));
      return strSpan;
    }

    // Convert tm from afx internal format to standard format
    _AfxTmConvertToStandardFormat(&tmTemp);

    // _tcsftime() doesn't handle %D, so do it here

    CString strPreParsed;
    LPCTSTR pstrSource = pFormat;
    int nTargetChar = 0;
    int nAccumulatedLength = lstrlen(pFormat);
    LPTSTR pstrTarget = strPreParsed.GetBuffer(nAccumulatedLength);

    while (*pstrSource) {
      if (*pstrSource == '%' && pstrSource[1] == 'D') {
        TCHAR szDay[12];
        _itot(GetDays(), szDay, 10);
        strPreParsed.ReleaseBuffer(nTargetChar);
        strPreParsed += szDay;
        int nTemp = lstrlen(szDay);
        nAccumulatedLength += nTemp;
        nTargetChar += nTemp;
        pstrTarget = strPreParsed.GetBuffer(nAccumulatedLength)
            + nTargetChar;
        pstrSource = _tcsinc(pstrSource);
        pstrSource = _tcsinc(pstrSource);
      }

      *pstrTarget = *pstrSource;
      nTargetChar++;
      pstrSource = _tcsinc(pstrSource);
      pstrTarget = _tcsinc(pstrTarget);
    }

    strPreParsed.ReleaseBuffer(nTargetChar);

    // Fill in the buffer, disregard return value as it's not necessary
    LPTSTR lpszTemp = strSpan.GetBufferSetLength(MAX_TIME_BUFFER_SIZE);
    _tcsftime(lpszTemp, strSpan.GetLength(), (LPCTSTR) strPreParsed, &tmTemp);
    strSpan.ReleaseBuffer();

    return strSpan;
  }

  CString COleDateTimeSpan::Format(UINT nFormatID) const {
    CString strFormat;
    VERIFY(strFormat.LoadString(nFormatID) != 0);
    return Format(strFormat);
  }

  void COleDateTimeSpan::CheckRange() {
    if (m_span < -MAX_DAYS_IN_SPAN || m_span > MAX_DAYS_IN_SPAN) {
      SetStatus(invalid);
    }
  }

};


COleDateTime operator-(const COleDateTime& d, const COleDateTimeSpan& dateSpan)
{
  COleDateTime dateResult;    // Initializes m_status to valid

  // If either operand NULL, result NULL
  if (d.GetStatus() == COleDateTime::null || dateSpan.GetStatus() == COleDateTime::null) {
    dateResult.SetStatus(COleDateTime::null);
    return dateResult;
  }

  // If either operand invalid, result invalid
  if (d.GetStatus() == COleDateTime::invalid || dateSpan.GetStatus() == COleDateTime::invalid) {
    dateResult.SetStatus(COleDateTime::invalid);
    return dateResult;
  }

  // Compute the actual date difference by subtracting underlying dates
  dateResult = _AfxDateFromDouble(_AfxDoubleFromDate(d.m_dt) - dateSpan.m_span);

  // Validate within range
  dateResult.CheckRange();

  return dateResult;
}

COleDateTimeSpan operator-(const COleDateTime& d, const COleDateTime& date)
{
  COleDateTimeSpan spanResult;

  // If either operand NULL, result NULL
  if (d.GetStatus() == COleDateTime::null || date.GetStatus() == COleDateTime::null) {
    spanResult.SetStatus(COleDateTimeSpan::null);
    return spanResult;
  }

  // If either operand invalid, result invalid
  if (d.GetStatus() == COleDateTime::invalid || date.GetStatus() == COleDateTime::invalid) {
    spanResult.SetStatus(COleDateTimeSpan::invalid);
    return spanResult;
  }

  // Return result (span can't be invalid, so don't check range)
  return _AfxDoubleFromDate(d.m_dt) - _AfxDoubleFromDate(date.m_dt);
}

COleDateTime operator+(const COleDateTime& d, const COleDateTimeSpan& dateSpan)
{
  COleDateTime dateResult;    // Initializes m_status to valid

  // If either operand NULL, result NULL
  if (d.GetStatus() == COleDateTime::null || dateSpan.GetStatus() == COleDateTime::null) {
    dateResult.SetStatus(COleDateTime::null);
    return dateResult;
  }

  // If either operand invalid, result invalid
  if (d.GetStatus() == COleDateTime::invalid || dateSpan.GetStatus() == COleDateTime::invalid) {
    dateResult.SetStatus(COleDateTime::invalid);
    return dateResult;
  }

  // Compute the actual date difference by adding underlying dates
  dateResult = _AfxDateFromDouble(_AfxDoubleFromDate(d.m_dt) + dateSpan.m_span);

  // Validate within range
  dateResult.CheckRange();

  return dateResult;
}
struct CPrintDialog {};
struct CPrintInfo { // Printing information structure
  CPrintInfo();
  ~CPrintInfo();

  CPrintDialog* m_pPD;     // pointer to print dialog

  BOOL m_bDocObject;       // TRUE if printing by IPrint interface
  BOOL m_bPreview;         // TRUE if in preview mode
  BOOL m_bDirect;          // TRUE if bypassing Print Dialog
  BOOL m_bContinuePrinting;// set to FALSE to prematurely end printing
  UINT m_nCurPage;         // Current page
  UINT m_nNumPreviewPages; // Desired number of preview pages
  CString m_strPageDesc;   // Format string for page number display
  LPVOID m_lpUserData;     // pointer to user created struct
  CRect m_rectDraw;        // rectangle defining current usable page area

  // these only valid if m_bDocObject
  UINT m_nOffsetPage;      // offset of first page in combined IPrint job
  DWORD m_dwFlags;         // flags passed to IPrint::Print

  void SetMinPage(UINT nMinPage);
  void SetMaxPage(UINT nMaxPage);
  UINT GetMinPage() const;
  UINT GetMaxPage() const;
  UINT GetFromPage() const;
  UINT GetToPage() const;
  UINT GetOffsetPage() const;
};