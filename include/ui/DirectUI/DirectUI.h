
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#endif
#include <windows.h>
#include <CommCtrl.h>
#include <WindowsX.h>
#include <assert.h>
#define UI_WNDSTYLE_CONTAINER  (0)
#define UI_WNDSTYLE_FRAME      (WS_VISIBLE | WS_OVERLAPPEDWINDOW)
#define UI_WNDSTYLE_CHILD      (WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)
#define UI_WNDSTYLE_DIALOG     (WS_VISIBLE | WS_POPUPWINDOW | WS_CAPTION | WS_DLGFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)
#define UI_WNDSTYLE_EX_FRAME   (WS_EX_WINDOWEDGE)
#define UI_WNDSTYLE_EX_DIALOG  (WS_EX_TOOLWINDOW | WS_EX_DLGMODALFRAME)
#define UI_CLASSSTYLE_CONTAINER  (0)
#define UI_CLASSSTYLE_FRAME      (CS_VREDRAW | CS_HREDRAW)
#define UI_CLASSSTYLE_CHILD      (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)
#define UI_CLASSSTYLE_DIALOG     (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS) 

#define NEW(T, N)  (T*)memset((T*)malloc(sizeof(T)*(N)), 0, sizeof(T)*(N))
#define DEL(P)     (free((void*)(P)), (P)=0)
#define Invalidate(hWnd) InvalidateRect(hWnd, 0, 1)
#define Invalidateww() Invalidate(ww->m_hWnd)
#define ASSERT assert
#define true 1
#define false 0

#ifndef MAX
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define strdup(s)   memcpy(malloc(strlen(s)+1), s, strlen(s)+1)
#define free_s(_p)   ((_p!=NULL)?(free((void*)(_p)), _p=0):(0))
#define setstr(_d, _s)   (free_s(_d), _d=strdup(_s))
#define IsEmpty(str)  (str==NULL || 0==strlen(str))

typedef int bool;

//
typedef enum EVENTTYPE_UI
{
  UIEVENT__FIRST = 0,
    UIEVENT__KEYBEGIN,
    UIEVENT_INIT,
    UIEVENT_UNINIT,
    UIEVENT_KEYDOWN,
    UIEVENT_KEYUP,
    UIEVENT_CHAR,
    UIEVENT_SYSKEY,
    UIEVENT__KEYEND,
    UIEVENT__MOUSEBEGIN,
    UIEVENT_MOUSEMOVE,
    UIEVENT_MOUSELEAVE,
    UIEVENT_MOUSEENTER,
    UIEVENT_MOUSEHOVER,
    UIEVENT_BUTTONDOWN,
    UIEVENT_BUTTONUP,
    UIEVENT_DBLCLICK,
    UIEVENT_CONTEXTMENU,
    UIEVENT_SCROLLWHEEL,
    UIEVENT__MOUSEEND,
    UIEVENT_KILLFOCUS,
    UIEVENT_SETFOCUS,
    UIEVENT_WINDOWSIZE,
    UIEVENT_SETCURSOR,
    UIEVENT_MEASUREITEM,
    UIEVENT_DRAWITEM,
    UIEVENT_TIMER,
    UIEVENT_NOTIFY,
    UIEVENT_COMMAND,
    UIEVENT_FIND_CONTROL_BY_POINT,
    UIEVENT__LAST
};
//
// Flags for CControlUI::GetControlFlags()
#define UIFLAG_TABSTOP       0x00000001
#define UIFLAG_SETCURSOR     0x00000002
#define UIFLAG_WANTRETURN    0x00000004 
// Flags for FindControl()
#define UIFIND_ALL           0x00000000
#define UIFIND_VISIBLE       0x00000001
#define UIFIND_ENABLED       0x00000002
#define UIFIND_HITTEST       0x00000004
#define UIFIND_TOP_FIRST     0x00000008
#define UIFIND_ME_FIRST      0x80000000 
// Flags for the CDialogLayout stretching
#define UISTRETCH_NEWGROUP   0x00000001
#define UISTRETCH_NEWLINE    0x00000002
#define UISTRETCH_MOVE_X     0x00000004
#define UISTRETCH_MOVE_Y     0x00000008
#define UISTRETCH_SIZE_X     0x00000010
#define UISTRETCH_SIZE_Y     0x00000020 
// Flags used for controlling the paint
#define UISTATE_FOCUSED      0x00000001
#define UISTATE_SELECTED     0x00000002
#define UISTATE_DISABLED     0x00000004
#define UISTATE_HOT          0x00000008
#define UISTATE_PUSHED       0x00000010
#define UISTATE_READONLY     0x00000020
#define UISTATE_CAPTURED     0x00000040 


typedef struct _CControlUI CControlUI;
typedef struct _CWindowWnd CWindowWnd;
typedef struct _UIResource UIResource;

typedef struct PtrArray PtrArray;
typedef struct StringPtrMap StringPtrMap; 

typedef struct _FINDTABINFO {
  CControlUI* pFocus;
  CControlUI* pLast;
  bool bForward;
  bool bNextIsIt;
}
FINDTABINFO;
typedef struct _FINDSHORTCUT {
  TCHAR ch;
  bool bPickNext;
}
FINDSHORTCUT;
typedef struct _TIMERINFO {
  CControlUI* pSender;
  UINT nLocalID;
  HWND hWnd;
  UINT uWinTimer;
  bool bKilled;
  UINT m_uTimerID;
  CWindowWnd* ww;
}
TIMERINFO;
// Structure for notifications from the system
// to the control implementation.
typedef struct _TEventUI {
  int Type;
  CControlUI* pSender;
  DWORD dwTimestamp;
  POINT ptMouse;
  TCHAR chKey;
  WORD wKeyState;
  WPARAM wParam;
  LPARAM lParam;
}
TEventUI;
// Structure for notifications to the outside world
typedef struct _TNotifyUI {
  LPCTSTR sType;
  CControlUI* pSender;
  DWORD dwTimestamp;
  POINT ptMouse;
  WPARAM wParam;
  LPARAM lParam;
}
TNotifyUI;
#define RectJoin( out, rc )  if ( rc.left < out.left ) {out.left = rc.left;} if ( rc.top < out.top ) {out.top = rc.top;} \
if ( rc.right > out.right ) {out.right = rc.right;} if ( rc.bottom > out.bottom ) {out.bottom = rc.bottom;}

#define CtrInit(ctr)  (ctr)->vtable->f_CtrInit(ctr)
#define CtrFree(ctr)  (ctr)->vtable->f_CtrFree(ctr)
#define DoPaint(ctr, hDC, rcPaint)  (ctr)->vtable->f_DoPaint(ww, ctr, hDC, rcPaint)
#define Event(ctr, event) ((ctr&&(ctr)->vtable->f_Event) ? (ctr)->vtable->f_Event(ww, ctr, &(event)) : (0))
#define Event2(ctr, TYPE, SENDER, CHKEY, KEYSTATE) if (ctr&&(ctr)->vtable->f_Event) { \
TEventUI event = { 0 }; event.Type = TYPE, event.pSender = SENDER, event.dwTimestamp = GetTickCount(); \
event.ptMouse = ww->m_ptLastMousePos, event.chKey = CHKEY, event.wKeyState = KEYSTATE; \
(ctr)->vtable->f_Event(ww, ctr, &(event));}
#define SetPos(ctr, rc)   if (ctr) {if ((ctr)->vtable->f_SetPos) {(ctr)->vtable->f_SetPos(ww, ctr, rc);} else {(ctr)->m_rcItem=rc;}}
#define SetAttribute(res, ctr, pstrName, pstrValue)  (ctr)->vtable->f_SetAttribute(res, ctr, pstrName, pstrValue)
typedef struct _CControlUI_vtable {
  const char* Name;
  UINT Size;
  int (*f_CtrInit)( CControlUI* hCtr );
  int (*f_CtrFree)( CControlUI* hCtr );
  int (*f_DoPaint)( CWindowWnd* ww, CControlUI* hCtr, HDC hDC, RECT rcPaint );
  int (*f_Event)( CWindowWnd* ww, CControlUI* hCtr, TEventUI* event );
  int (*f_SetPos)( CWindowWnd* ww, CControlUI* hCtr, RECT rc );
  int (*f_SetAttribute)( UIResource* res, CControlUI* pCtr, LPCTSTR pstrName, LPCTSTR pstrValue );
} CControlUI_vtable;
#define lengthof(x) (sizeof(x)/sizeof(*x))
//#define NeedUpdate(ctr)      ((ctr) ? ((ctr)->m_bUpdateNeeded = true) : (0))
#define NeedUpdate(ww, ctr)      UINeedUpdate(ww, ctr)
#define GetPos(ctr)          ((ctr)->m_rcItem)
#define IsVisible(ctr)       ((ctr)->m_bVisible)
#define IsEnabled(ctr)       ((ctr)->m_bEnabled)
#define GetToolTip(ctr, pt)  ((ctr)->m_sToolTip)
typedef struct _CControlUI {
  CControlUI_vtable* vtable;
  //CWindowWnd* m_pManager;
  CControlUI* m_pParent;// 父控件
  PtrArray* m_items; // 子控件
  //CControlUI** m_items2; // 子控件
  LPCTSTR m_sName;
  LPCTSTR m_sText;
  LPCTSTR m_sToolTip;
  LPCTSTR m_sUserData;
  LPCTSTR m_sBkImage;
  RECT m_rcItem;
  RECT m_rcPaint;
  RECT m_rcPadding;//外边距，由上层窗口绘制
  SIZE m_cXY;
  SIZE m_cxyFixed;
  SIZE m_cxyMin;
  SIZE m_cxyMax;
  bool m_bUpdateNeeded;
  bool m_bVisible;
  bool m_bInternVisible;
  bool m_bEnabled;
  bool m_bMouseEnabled;
  bool m_bFocused;
  bool m_bFloat;
  bool m_bFloatSetPos; // 防止SetPos循环调用
  DWORD m_dwBackColor;
  DWORD m_dwBackColor2;
  DWORD m_dwBorderColor;
  UINT_PTR m_pTag;
  int m_nBorderSize;
  TCHAR m_chShortcut;
} CControlUI;
typedef struct _CWindowWnd {
  HWND m_hWnd;
  HDC m_hDcPaint;
  HDC m_hDcOffscreen;
  HBITMAP m_hbmpOffscreen;
  HWND m_hwndTooltip;
  TOOLINFO m_ToolTip;

  //HWND m_hWndPaint;
  WNDPROC m_OldWndProc;
  BOOL m_bSubclassed;
  CControlUI* m_pRoot;
  CControlUI* m_pFocus;
  CControlUI* m_pEventHover;
  CControlUI* m_pEventClick;
  CControlUI* m_pEventKey;

  POINT m_ptLastMousePos;
  RECT m_rcSizeBox;
  RECT m_rcCaption;
  SIZE m_szMinWindow;
  SIZE m_szInitWindowSize;
  SIZE m_szRoundCorner;
  UINT m_uTimerID;
  UINT m_uButtonState;
  bool m_bFirstLayout;
  bool m_bUpdateNeeded;
  bool m_bFocusNeeded;
  bool m_bOffscreenPaint;
  bool m_bMouseTracking;
  bool m_bMouseCapture;
  PtrArray* m_aTimers;
  PtrArray* m_aNotifiers;
  UIResource* m_pResource;
  int (*Final)( CWindowWnd* ww );
} CWindowWnd;
int m_aPreMessagesSize=0;
CWindowWnd* m_aPreMessages[100];
#define GetPaintWindow(xx) (xx)->m_hWnd

typedef struct _TFontInfo {
  HFONT hFont;
  TEXTMETRIC tm;
}
TFontInfo;
typedef struct _TImageInfo {
  HBITMAP hBitmap;
  int nX;
  int nY;
  bool alphaChannel;
}
TImageInfo;
typedef struct _CRenderEngine {
  int asdf;
}CRenderEngine;

typedef struct _UIResource {
  HDC m_hDcPaint;
  DWORD m_dwDefalutDisabledColor;
  HFONT m_hDefalutFont;
  DWORD m_dwDefalutFontColor;
  TEXTMETRIC m_hDefalutFontTextMetric;
  HFONT m_hDefalutBoldFont;
  DWORD m_dwDefalutBoldFontColor;
  TEXTMETRIC m_hDefaluttBoldFontTextMetric;
  HFONT m_hDefalutLinkFont;
  DWORD m_dwDefalutLinkFontColor;
  DWORD m_dwDefalutLinkFontHoverColor;
  TEXTMETRIC m_hDefalutLinkFontTextMetric;
  PtrArray* m_aCustomFonts;
  StringPtrMap* m_mImageHash;
  StringPtrMap* m_DefaultAttrHash;
}UIResource;

enum {
  DEFAULT_SCROLLBAR_SIZE = 16,
    DEFAULT_TIMERID = 10,
};

typedef struct _UIScrollbarBase {
  CControlUI base;
  bool m_bHorizontal;
  int m_nRange;
  int m_nScrollPos;
  int m_nLineSize;
  POINT ptLastMouse;
  int m_nLastScrollPos;
  int m_nLastScrollOffset;
  int m_nScrollRepeatDelay;
  RECT m_rcButton1;
  UINT m_uButton1State;
  RECT m_rcButton2;
  UINT m_uButton2State;
  RECT m_rcThumb;
  UINT m_uThumbState;
} UIScrollbarBase;

typedef struct _UIScrollbar {
  CControlUI base;
  bool m_bHorizontal;
  int m_nRange;
  int m_nScrollPos;
  int m_nLineSize;
  POINT ptLastMouse;
  int m_nLastScrollPos;
  int m_nLastScrollOffset;
  int m_nScrollRepeatDelay;
  RECT m_rcButton1;
  UINT m_uButton1State;
  RECT m_rcButton2;
  UINT m_uButton2State;
  RECT m_rcThumb;
  UINT m_uThumbState;
  
  LPCTSTR m_sBkNormalImage;
  LPCTSTR m_sBkHotImage;
  LPCTSTR m_sBkPushedImage;
  LPCTSTR m_sBkDisabledImage;

  LPCTSTR m_sButton1NormalImage;
  LPCTSTR m_sButton1HotImage;
  LPCTSTR m_sButton1PushedImage;
  LPCTSTR m_sButton1DisabledImage;

  LPCTSTR m_sButton2NormalImage;
  LPCTSTR m_sButton2HotImage;
  LPCTSTR m_sButton2PushedImage;
  LPCTSTR m_sButton2DisabledImage;

  LPCTSTR m_sThumbNormalImage;
  LPCTSTR m_sThumbHotImage;
  LPCTSTR m_sThumbPushedImage;
  LPCTSTR m_sThumbDisabledImage;
  LPCTSTR m_sRailNormalImage;
  LPCTSTR m_sRailHotImage;
  LPCTSTR m_sRailPushedImage;
  LPCTSTR m_sRailDisabledImage;
  LPCTSTR m_sImageModify;
}UIScrollbar;

typedef struct _UICam {
  CControlUI base;
  void* cam;
  int index;
  int rate;
  int isflip;
  UINT m_uStyle;
} UICam;

typedef struct _CContainerUI{
  CControlUI base;
  RECT m_rcInset;
  int m_iChildPadding;
  bool m_bAutoDestroy;
  bool m_bMouseChildEnabled;
  bool m_bScrollProcess; // 防止SetPos循环调用
  CControlUI* m_pVerticalScrollbar;
  CControlUI* m_pHorizontalScrollbar;
} CContainerUI;

typedef struct _UIHorizontalLayout {
  CContainerUI base;
  int m_iSepWidth;
  UINT m_uButtonState;
  POINT ptLastMouse;
  RECT m_rcNewPos;
  bool m_bImmMode;
} UIHorizontalLayout;

typedef struct _UICoolEdit{
  CControlUI base;
  BOOL m_bLBtDown;     // 鼠标左键按下。
  int m_nCharPos;      // 当前光标处于第几个字符的后面，从0开始计数。
  int m_nShowChar;     // 在编辑框中第一个显示的字符，从0开始计数。
  int m_nMaxShowChars; // 在编辑框中一次最多显示的字符数量 。
  int m_nSelectBegin;  // 选择的字符的起始位置。
  int m_nSelectEnd;    // 选择的字符的终止位置。
  int m_nShiftBegin;   // shift键第一次时光标所在的字符的位置。
  POINT m_ptCursor;    // 当前光标的位置。
  POINT m_ptTxtBegin;  // 文本的起始位置。
  POINT m_ptFirst;     // 鼠标左键按下时的第一点位置。
  POINT m_ptSecond;    // 鼠标当前的位置。
  TEXTMETRIC m_tm;     // 文本字体的信息。
  int m_iRoundCorner;
  BOOL m_bDrawShade;
  int m_iFont;
} UICoolEdit;

typedef struct _CListInfo{
  int nColumnCount;
  int nItemCount;
} CListInfo;

typedef struct _CListColumn{
  TCHAR m_strText[256];
  int m_nWidth;
  BOOL m_bFixed;
  UINT m_nFormat;
  UINT m_nFlags;
  int m_nImage;
  //int m_nIndex;
  //CListArray < CStdString > m_aComboList;
}CListColumn;

typedef struct _CSubItem{
  TCHAR m_strText[256];
  int m_nImage;
  UINT m_nFormat;
  UINT m_nFlags;
  //CListArray < CStdString > m_aComboList;
  HFONT m_hFont;
  COLORREF m_rgbBackground;
  COLORREF m_rgbText;
  int m_nItemImage;
  UINT m_nItemFormat;
  UINT m_nItemFlags;
} CSubItem;

typedef struct _UIListCtrlCallBack{
  int (*f_GetList)(void* pListData, int nItem, int nSubItem, CListInfo* pListInfo, CListColumn* pListColumn, CSubItem* pSubItem);
}UIListCtrlCallBack;

typedef struct _UIListCtrl{
  CContainerUI base;
  void* pListData;
  UIListCtrlCallBack* cb;
  BOOL m_bShowHeader;
  BOOL m_bShowThemed;
  BOOL m_bShowSort;
  BOOL m_bSortAscending;
  BOOL m_bButtonDown;
  BOOL m_bMouseOver;
  BOOL m_bColumnSizing;
  BOOL m_bBeginSelect;
  BOOL m_bSingleSelect;
  BOOL m_bFocusSubItem;
  BOOL m_bGroupSelect;
  BOOL m_bShowHorizScroll;
  BOOL m_bShowVertScroll;
  BOOL m_bEnableHorizScroll;
  BOOL m_bEnableVertScroll;
  BOOL m_bResizeTimer;
  BOOL m_bDragDrop;
  BOOL m_bSmoothScroll;
  BOOL m_bEditItem;
  BOOL m_bScrolling;
  BOOL m_bScrollDown;
  BOOL m_bTileBackground;
  int m_nItemHeight;
  int m_nHeaderHeight;
  int m_nFirstSelected;
  int m_nFocusItem;
  int m_nFocusSubItem;
  int m_nHotItem;
  int m_nHotSubItem;
  int m_nTitleTipItem;
  int m_nTitleTipSubItem;
  int m_nMouseWheelScroll;
  int m_nTotalWidth;
  int m_nSortColumn;
  int m_nDragColumn;
  int m_nHighlightColumn;
  int m_nHotColumn;
  int m_nHotDivider;
  int m_nColumnSizing;
  int m_nScrollOffset;
  int m_nScrollDelta;
  int m_nScrollUnit;
  int m_nStartScrollPos;
  int m_nStartSize;
  int m_nStartPos;
  COLORREF m_rgbBackground;
  COLORREF m_rgbHeaderBackground;
  COLORREF m_rgbHeaderBorder;
  COLORREF m_rgbHeaderShadow;
  COLORREF m_rgbHeaderText;
  COLORREF m_rgbHeaderHighlight;
  COLORREF m_rgbSelectedItem;
  COLORREF m_rgbSelectedText;
  COLORREF m_rgbItemText;
  COLORREF m_rgbSelectOuter;
  COLORREF m_rgbSelectInner;
  COLORREF m_rgbSelectTop;
  COLORREF m_rgbSelectBottom;
  COLORREF m_rgbNoFocusTop;
  COLORREF m_rgbNoFocusBottom;
  COLORREF m_rgbNoFocusOuter;
  COLORREF m_rgbNoFocusInner;
  COLORREF m_rgbFocusTop;
  COLORREF m_rgbFocusBottom;
  COLORREF m_rgbProgressTop;
  COLORREF m_rgbProgressBottom;
  COLORREF m_rgbItemFocus;
  COLORREF m_rgbHyperLink;
  DWORD m_dwSearchTick;
  DWORD m_dwScrollTick;
  POINT m_ptDownPoint;
  POINT m_ptSelectPoint;
  RECT m_rcGroupSelect;
  LPCTSTR m_strSearchString;
  HBITMAP m_bmpScrollList;
  HBITMAP m_bmpBackground;
  UINT m_nHeaderClipboardFormat;
  HCURSOR m_curDivider;
  HCURSOR m_curHyperLink;
  HFONT m_fntListFont;
  HFONT m_fntUnderlineFont;
  HIMAGELIST m_ilListItems;
  HIMAGELIST m_ilItemImages;
  //CDragDrop < CListImpl > m_oleDragDrop;
  //CToolTipCtrl* m_ttToolTip;
  //CTitleTip* m_wndTitleTip;
  //CDropArrows* m_wndDropArrows;
  //CListEdit* m_wndItemEdit;
  //CEditWnd* m_wndItemEdit;
  //CListCombo* m_wndItemCombo;
  //CListDate* m_wndItemDate;
  //CListArray < CListColumn > m_aColumns;
  //set< int > m_setSelectedItems;
  char* m_setSelectedItems;
  int m_setSelectedItemsSize;
} UIListCtrl;

#define IDC_INPLACE_CONTROL   8                  // ID of inplace edit controls

// Handy functions
#define IsSHIFTpressed() ( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0   )
#define IsCTRLpressed()  ( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 )

// This structure sent to Grid's parent in a WM_NOTIFY message
typedef struct tagNM_GRIDVIEW {
  NMHDR hdr;
  int   iRow;
  int   iColumn;
} NM_GRIDVIEW;

// This is sent to the Grid from child in-place edit controls
typedef struct tagGV_DISPINFO {
  NMHDR   hdr;
  //GV_ITEM item;
} GV_DISPINFO;

// This is sent to the Grid from child in-place edit controls
typedef struct tagGV_CACHEHINT {
  NMHDR      hdr;
  //CCellRange range;
} GV_CACHEHINT;

// For virtual mode callback
typedef BOOL (CALLBACK* GRIDCALLBACK)(GV_DISPINFO *, LPARAM);

// Grid line/scrollbar selection
#define GVL_NONE                0L      // Neither
#define GVL_HORZ                1L      // Horizontal line or scrollbar
#define GVL_VERT                2L      // Vertical line or scrollbar
#define GVL_BOTH                3L      // Both 
// Autosizing option
#define GVS_DEFAULT             0
#define GVS_HEADER              1       // Size using column fixed cells data only
#define GVS_DATA                2       // Size using column non-fixed cells data only
#define GVS_BOTH                3       // Size using column fixed and non-fixed 
// Cell Searching options
#define GVNI_FOCUSED            0x0001
#define GVNI_SELECTED           0x0002
#define GVNI_DROPHILITED        0x0004
#define GVNI_READONLY           0x0008
#define GVNI_FIXED              0x0010
#define GVNI_MODIFIED           0x0020
#define GVNI_ABOVE              LVNI_ABOVE
#define GVNI_BELOW              LVNI_BELOW
#define GVNI_TOLEFT             LVNI_TOLEFT
#define GVNI_TORIGHT            LVNI_TORIGHT
#define GVNI_ALL                (LVNI_BELOW|LVNI_TORIGHT|LVNI_TOLEFT)
#define GVNI_AREA               (LVNI_BELOW|LVNI_TORIGHT) 
// Hit test values (not yet implemented)
#define GVHT_DATA               0x0000
#define GVHT_TOPLEFT            0x0001
#define GVHT_COLHDR             0x0002
#define GVHT_ROWHDR             0x0004
#define GVHT_COLSIZER           0x0008
#define GVHT_ROWSIZER           0x0010
#define GVHT_LEFT               0x0020
#define GVHT_RIGHT              0x0040
#define GVHT_ABOVE              0x0080
#define GVHT_BELOW              0x0100 
// Messages sent to the grid's parent (More will be added in future)
#define GVN_BEGINDRAG           LVN_BEGINDRAG        // LVN_FIRST-9
#define GVN_BEGINLABELEDIT      LVN_BEGINLABELEDIT   // LVN_FIRST-5
#define GVN_BEGINRDRAG          LVN_BEGINRDRAG
#define GVN_COLUMNCLICK         LVN_COLUMNCLICK
#define GVN_DELETEITEM          LVN_DELETEITEM
#define GVN_ENDLABELEDIT        LVN_ENDLABELEDIT     // LVN_FIRST-6
#define GVN_SELCHANGING         LVN_ITEMCHANGING
#define GVN_SELCHANGED          LVN_ITEMCHANGED
#define GVN_GETDISPINFO         LVN_GETDISPINFO
#define GVN_ODCACHEHINT         LVN_ODCACHEHINT

#define GRIDCONTROL_NO_DRAGDROP
enum eMouseModes 
{ MOUSE_NOTHING, MOUSE_SELECT_ALL, MOUSE_SELECT_COL, MOUSE_SELECT_ROW,
MOUSE_SELECT_CELLS, MOUSE_SCROLLING_CELLS,
MOUSE_OVER_ROW_DIVIDE, MOUSE_SIZING_ROW,
MOUSE_OVER_COL_DIVIDE, MOUSE_SIZING_COL,
MOUSE_PREPARE_EDIT,
#ifndef GRIDCONTROL_NO_DRAGDROP
MOUSE_PREPARE_DRAG, MOUSE_DRAGGING
#endif
};

typedef struct _CGridCell {
  int row, col;
  DWORD m_nState;      // Cell state (selected/focus etc)
  TCHAR m_strText[256];    // Cell text (or binary data if you wish...)
  LPARAM m_lParam;     // 32-bit value to associate with item
  int m_nImage;        // Index of the list view item抯 icon
  DWORD m_nFormat;
  UINT m_mask;            // Mask for use in getting/setting cell data
  COLORREF m_crFgClr;
  COLORREF m_crBkClr;
  LOGFONT* m_plfFont;
  int m_nMargin;
  BOOL m_bEditing;     // Cell being edited?

  SIZE m_Size;        // Default Size
  HFONT m_Font;       // Cached font
  DWORD m_dwStyle;    // Cell Style - unused

  // for CGridCellCheck
  BOOL m_bChecked;
  RECT m_Rect;
} CGridCell;

typedef struct _CCellID {
  int row, col;
}CCellID;

typedef struct _CCellRange {
  int m_nMinRow;
  int m_nMinCol;
  int m_nMaxRow;
  int m_nMaxCol;
}CCellRange;

typedef struct _UIGridCtrlCallBack {
  int (*f_GetGrid)(void* pListData, CGridCell* pSubItem);
} UIGridCtrlCallBack;

typedef BOOL ( *PVIRTUALCOMPARE ) ( int, int );
typedef struct _UIGridCtrl {
  int asdf;
}UIGridCtrl;
  int* m_arRowOrder;
  int* m_arColOrder;
  BOOL m_bDragRowMode;
  int m_CurRow;
  PVIRTUALCOMPARE m_pfnVirtualCompare;
  BOOL m_InDestructor;
  BOOL m_AllowReorderColumn;
  BOOL m_QuitFocusOnTab;
  BOOL m_AllowSelectRowInFixedCol;

  // General attributes
  COLORREF    m_crFixedTextColour, m_crFixedBkColour;
  COLORREF    m_crGridBkColour, m_crGridLineColour;
  COLORREF    m_crWindowText, m_crWindowColour, m_cr3DFace, m_crShadow;    // System colours
  COLORREF    m_crTTipBackClr, m_crTTipTextClr;                 // Titletip colours - FNA
  
  BOOL        m_bVirtualMode;
  LPARAM      m_lParam;                                           // lParam for callback
  GRIDCALLBACK m_pfnCallback;                                     // The callback function
  
  int         m_nGridLines;
  BOOL        m_bEditable;
  BOOL        m_bModified;
  BOOL        m_bAllowDragAndDrop;
  BOOL        m_bListMode;
  BOOL        m_bSingleRowSelection;
  BOOL        m_bSingleColSelection;
  BOOL        m_bAllowDraw;
  BOOL        m_bEnableSelection;
  BOOL        m_bFixedRowSelection, m_bFixedColumnSelection;
  BOOL        m_bSortOnClick;
  BOOL        m_bHandleTabKey;
  BOOL        m_bDoubleBuffer;
  BOOL        m_bTitleTips;
  int         m_nBarState;
  BOOL        m_bWysiwygPrinting;
  BOOL        m_bHiddenColUnhide, m_bHiddenRowUnhide;
  BOOL        m_bAllowColHide, m_bAllowRowHide;
  BOOL        m_bAutoSizeSkipColHdr;
  BOOL        m_bTrackFocusCell;
  BOOL        m_bFrameFocus;
  UINT        m_nAutoSizeColumnStyle;
  
  // Cell size details
  int         m_nRows, m_nFixedRows, m_nCols, m_nFixedCols;
  int*  m_arRowHeights;
  int* m_arColWidths;
  int         m_nVScrollMax, m_nHScrollMax;
  
  // Fonts and images
  CGridCell m_cellDefault;  // "default" cell. Contains default colours, font etc.
  CGridCell m_cellFixedColDef, m_cellFixedRowDef, m_cellFixedRowColDef;
  HFONT      m_PrinterFont;  // for the printer
  HIMAGELIST m_pImageList;
  
  // Cell data
  //CTypedPtrArray<CObArray, GRID_ROW*> m_RowData;
  
  // Mouse operations such as cell selection
  int         m_MouseMode;
  BOOL        m_bLMouseButtonDown, m_bRMouseButtonDown;
  POINT      m_LeftClickDownPoint, m_LastMousePoint;
  CCellID     m_LeftClickDownCell, m_SelectionStartCell;
  CCellID     m_idCurrentCell, m_idTopLeftCell;
  INT_PTR     m_nTimerID;
  int         m_nTimerInterval;
  int         m_nResizeCaptureRange;
  BOOL        m_bAllowRowResize, m_bAllowColumnResize;
  int         m_nRowsPerWheelNotch;
  //CMap<DWORD,DWORD, CCellID, CCellID&> m_SelectedCellMap, m_PrevSelectedCellMap;
#define GRIDCONTROL_NO_TITLETIPS
#ifndef GRIDCONTROL_NO_TITLETIPS
  CTitleTip   m_TitleTip;             // Title tips for cells
#endif
  
  // Drag and drop
  CCellID     m_LastDragOverCell;
  
  // Printing information
  int        m_nPageHeight;
  SIZE       m_CharSize;
  SIZE       m_LogicalPageSize;      // Page size in gridctrl units.
  SIZE       m_PaperSize;            // Page size in device units.
  // additional properties to support Wysiwyg printing
  int         m_nPageWidth;
  int         m_nPrintColumn;
  int         m_nCurrPrintRow;
  int         m_nNumPages;
  int         m_nPageMultiplier;
  
  // sorting
  int          m_bAscending;
  int          m_nSortColumn;
  PFNLVCOMPARE m_pfnCompare;
  
  // EFW - Added to support shaded/unshaded printout.  If true, colored
  // cells will print as-is.  If false, all text prints as black on white.
  BOOL        m_bShadedPrintOut;
  
  // EFW - Added support for user-definable margins.  Top and bottom are in 
  // lines.  Left, right, and gap are in characters (avg width is used).
  int         m_nHeaderHeight, m_nFooterHeight, m_nLeftMargin,
    m_nRightMargin, m_nTopMargin, m_nBottomMargin, m_nGap;
  
#define GetColumnCount() (0)
#define GetColumnWidth(a) (0)
#define GetRowCount() (0)
#define GetRowHeight(a) (0)
#define GetCell(a,b,c) (0)
#define SetCoords(a,b,c) (0)

  // Cell states
#define GVIS_FOCUSED            0x0001
#define GVIS_SELECTED           0x0002
#define GVIS_DROPHILITED        0x0004
#define GVIS_READONLY           0x0008
#define GVIS_FIXED              0x0010
#define GVIS_FIXEDROW           0x0020
#define GVIS_FIXEDCOL           0x0040
#define GVIS_MODIFIED           0x0080 
  // Cell data mask
#define GVIF_TEXT               LVIF_TEXT
#define GVIF_IMAGE              LVIF_IMAGE
#define GVIF_PARAM              LVIF_PARAM
#define GVIF_STATE              LVIF_STATE
#define GVIF_BKCLR              (GVIF_STATE<<1)
#define GVIF_FGCLR              (GVIF_STATE<<2)
#define GVIF_FORMAT             (GVIF_STATE<<3)
#define GVIF_FONT               (GVIF_STATE<<4)
#define GVIF_MARGIN             (GVIF_STATE<<5)
#define GVIF_ALL                (GVIF_TEXT|GVIF_IMAGE|GVIF_PARAM|GVIF_STATE|GVIF_BKCLR|GVIF_FGCLR|GVIF_FORMAT|GVIF_FONT|GVIF_MARGIN)
  
#define GetState(pCell)                ( pCell->m_nState )
#define CellIsFocused(pCell)           ( pCell->m_nState & GVIS_FOCUSED )
#define CellIsFixed(pCell)             ( pCell->m_nState & GVIS_FIXED )
#define CellIsFixedCol(pCell)          ( pCell->m_nState & GVIS_FIXEDCOL )
#define CellIsFixedRow(pCell)          ( pCell->m_nState & GVIS_FIXEDROW )
#define CellIsSelected(pCell)          ( pCell->m_nState & GVIS_SELECTED )
#define CellIsReadOnly(pCell)          ( pCell->m_nState & GVIS_READONLY )
#define CellIsModified(pCell)          ( pCell->m_nState & GVIS_MODIFIED )
#define CellIsDropHighlighted(pCell)   ( pCell->m_nState & GVIS_DROPHILITED )
#define CellGetDefaultCell(pCell)      GetDefaultCell( CellIsFixedRow(pCell), CellIsFixedCol(pCell) )
#define CellGetMargin(pCell)           (pCell->m_nMargin)
#define CellGetImage(pCell)            (pCell->m_nImage)
#define CellGetFont(pCell)             (pCell->m_plfFont)
  
#define GetSortAscending(pGrid)           ( m_bAscending )
#define GetSortColumn(pGrid)           ( m_nSortColumn )
#define GetImageList(pGrid)           ( m_pImageList )
#define GetGridLines(pGrid)           ( m_nGridLines )
#define GetFrameFocusCell(pGrid)      ( m_bFrameFocus )
#define GetFocusCell(pGrid)           ( m_idCurrentCell )
#define GetTrackFocusCell(pGrid)      ( m_bTrackFocusCell )
#define GridIsValid( pGrid, nRow, nCol )  ( nRow >= 0 && nRow < m_nRows && nCol >= 0 && nCol < m_nCols )

#define GetCurrentPen(m_hDC)    ( ( HPEN )GetCurrentObject( m_hDC, OBJ_PEN ) )

#define GetImageInfo( m_hImageList, nImage, pImageInfo )   ImageList_GetImageInfo( m_hImageList, nImage, pImageInfo )
  
  
#define TopLeft(rect)      (* ( ( POINT* ) (&(rect) )     ))
#define BottomRight(rect)  (* ( ( POINT* ) (&(rect) ) + 1 ))
  
#define CImageList_Draw( m_hImageList, hDC, nImage, pt,  nStyle )     ImageList_Draw( m_hImageList, nImage, hDC, pt.x, pt.y, nStyle )
  
SIZE GetTextExtent( HDC m_hDC, LPCTSTR lpszString, int nCount ) {
  SIZE Size;
  GetTextExtentPoint32( m_hDC, lpszString, nCount, &Size );
  return Size;
}
typedef UIGridCtrl CGridCtrl;

// CGridCellBase Sizing
BOOL CellGetTextRect( CControlUI* ctr, CGridCell* pCell, LPRECT pRect )         // i/o:  i=dims of cell rect; o=dims of text rect
{
  if ( CellGetImage(pCell) >= 0 ) {
    IMAGEINFO Info;
    CGridCtrl* pGrid = (CGridCtrl*)ctr;
    HIMAGELIST pImageList = GetImageList(pGrid);
    if ( pImageList && GetImageInfo( pImageList, CellGetImage(pCell), &Info ) ) {
      int nImageWidth = Info.rcImage.right - Info.rcImage.left + 1;
      pRect->left += nImageWidth + CellGetMargin(pCell);
    }
  }
  return TRUE;
}
#define CellGetFormat(pCell)  ( pCell->m_nFormat )
#define CellGetText(pCell)  (( pCell->m_strText ) ? _T( "" ) : ( pCell->m_strText ))

