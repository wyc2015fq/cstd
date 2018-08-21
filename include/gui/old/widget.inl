#define GET_WORD(ptr) (*(const WORD *)(ptr))
#define GET_DWORD(ptr) (*(const DWORD *)(ptr))
#define WM_SYSTIMER 0x0118
#define WM_POPUPSYSTEMMENU 0x0313
#define HWID_TOP ((HWID)0)
#define HWID_BOTTOM ((HWID)1)
#define HWID_TOPMOST ((HWID)-1)
#define HWID_NOTOPMOST ((HWID)-2)
#define MDI_FIRST_CHILD_ID 2004
/* undocumented SWP flags - from SDK 3.1 */
#define SWP_NOCLIENTSIZE 0x0800
#define SWP_NOCLIENTMOVE 0x1000
#define SWP_STATECHANGED 0x8000
#define SW_NORMALNA 0xCC /* undoc. flag in MinMaximize */
#ifndef WM_KEYF1
#define WM_KEYF1 0x004d
#endif
#define WS_EX_DRAGDETECT 0x0040
#ifndef WM_SYSTIMER
#define WM_SYSTIMER 0x0118
#endif
#define WND_PARENT_ID 1
#define WND_POPUP_ID 2
#define WND_CHILD_ID 3
#ifndef WM_LBTRACKPOINT
#define WM_LBTRACKPOINT 0x0131
#endif
int GetWidgetText(HWID hWid, char* lpString, int nMaxCount)
{
  int len = 0;
  if (lpString && nMaxCount > 0) {
    *lpString = 0;
  }
  if (hWid && hWid->text) {
    len = strlen(hWid->text);
    if (lpString) {
      strncpy(lpString, hWid->text, nMaxCount);
    }
  }
  return len;
}
BOOL IsWidget(HWID hWid)
{
  return TRUE;
}
#define WIN_ReleasePtr(x) (x)
BOOL InvalidateRect(HWID hWid, CONST RECT* lpRect, BOOL bErase)
{
  RECT rc = hWid->rect;
  if (lpRect) {
    IntersectRect(&rc, &rc, lpRect);
  }
  while (hWid->parent && NULL == hWid->handle) {
    hWid = hWid->parent;
  }
  return hWid->handle ? InvalidateRect((HWND)(hWid->handle->hwnd), &rc, bErase) : FALSE;
}
BOOL GetScrollInfo(HWID hWid, INT nBar, LPSCROLLINFO info);
INT SetScrollPos(HWID hWid, INT nBar, INT nPos, BOOL bRedraw);
BOOL EnableScrollBar(HWID hWid, UINT nBar, UINT flags);
BOOL GetWidgetRect(HWID hWid, LPRECT lpRect)
{
  if (hWid) {
    *lpRect = hWid->rect;
    int nHeight = GetSystemMetrics(SM_CYVSCROLL);
    SCROLLINFO si = {sizeof(SCROLLINFO)};
    si.fMask = SIF_ALL;
    if (hWid->dwStyle & WS_VSCROLL) {
      GetScrollInfo(hWid, SB_VERT, &si);
      if (si.nPage < si.nMax) {
        int nWidht = GetSystemMetrics(SM_CXVSCROLL);
        lpRect->right -= nWidht;
      }
    }
    if (hWid->dwStyle & WS_HSCROLL) {
      GetScrollInfo(hWid, SB_HORZ, &si);
      if (si.nPage < si.nMax) {
        int nHeight = GetSystemMetrics(SM_CYVSCROLL);
        lpRect->bottom -= nHeight;
      }
    }
  }
  return !!(hWid);
}
HFONT GetWidgetFont(HWID hWid)
{
  return NULL;
}
#define WidgetSendMessageW SendMessage
#define SendMessageW SendMessage
LRESULT SendMessage(HWID hWid, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  if (hWid && hWid->wc && hWid->wc->lpfnWndProc) {
    return //ScrollBarWndProc_common(hWid, Msg, wParam, lParam) ||
        hWid->wc->lpfnWndProc(hWid, Msg, wParam, lParam);
  }
  return NULL;
}
HDC GetDC(HWID hWid)
{
  HDC hdc = GetDC((hWid && hWid->handle) ? hWid->handle->hwnd : NULL);
  return hdc;
}
int ReleaseDC(HWID hWid, HDC hdc)
{
  return ReleaseDC((hWid && hWid->handle) ? hWid->handle->hwnd : NULL, hdc);
}
HDC GetDCEx(HWID hWid, HRGN hrgnClip, DWORD flags)
{
  HWND hwnd = (hWid && hWid->handle) ? hWid->handle->hwnd : NULL;
  //return GetDCEx(hwnd, hrgnClip, flags);
  return GetDC(hwnd);
}
BOOL HideCaret(HWID hWid)
{
  HWND hwnd = (hWid && hWid->handle) ? hWid->handle->hwnd : NULL;
  return HideCaret(hwnd);
}
UINT_PTR SetSystemTimer(HWID hWid, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc)
{
  HWND hwnd = (hWid && hWid->handle) ? hWid->handle->hwnd : NULL;
  return SetTimer(hwnd, nIDEvent, uElapse, lpTimerFunc);
}
UINT_PTR KillSystemTimer(HWID hWid, UINT_PTR nIDEvent)
{
  HWND hwnd = (hWid && hWid->handle) ? hWid->handle->hwnd : NULL;
  return KillTimer(hwnd, nIDEvent);
}
BOOL CreateCaret(HWID hWid, HBITMAP hBitmap, int nWidth, int nHeight)
{
  HWND hwnd = (hWid && hWid->handle) ? hWid->handle->hwnd : NULL;
  return CreateCaret(hwnd, hBitmap, nWidth, nHeight);
}
BOOL ShowCaret(HWID hWid)
{
  HWND hwnd = (hWid && hWid->handle) ? hWid->handle->hwnd : NULL;
  return ShowCaret(hwnd);
}
LONG GetClassLong(HWID hWid, int nIndex)
{
  return 0;
}
#define GetWidgetLongA GetWidgetLong
#define GetWidgetLongW GetWidgetLong
#define GetWidgetLongPtrA GetWidgetLong
#define SetWidgetLongPtrA SetWidgetLong
#define GetWidgetLongPtrW GetWidgetLong
#define SetWidgetLongPtrW SetWidgetLong
#define SetWidgetLongA SetWidgetLong
#define SetWidgetLongW SetWidgetLong
LONG GetWidgetLong(HWID hWid, int nIndex)
{
  if (NULL == hWid) {
    return 0;
  }
  if (nIndex < 0) {
    switch (nIndex) {
    case GWL_WNDPROC:
      return (LONG)(hWid->wc->lpfnWndProc);
    case GWL_STYLE:
      return (LONG)(hWid->dwStyle);
    case GWL_EXSTYLE:
      return (LONG)(hWid->dwExStyle);
    case GWL_USERDATA:
      return (LONG)(hWid->userdata);
    case GWLP_ID:
      return (LONG)(hWid->id);
    }
  }
  else {
    LONG* extbuf = (LONG*)(hWid + 1);
    nIndex /= sizeof(LONG);
    if (nIndex < hWid->wc->cbWndExtra) {
      return extbuf[nIndex];
    }
  }
  return 0;
}
LONG SetWidgetLong(HWID hWid, int nIndex, LONG dwNewLong)
{
  if (NULL == hWid) {
    return 0;
  }
  if (nIndex < 0) {
    switch (nIndex) {
    case GWL_WNDPROC:
      return (LONG)(hWid->wc->lpfnWndProc = (WIDPROC)dwNewLong);
    case GWL_STYLE:
      return (LONG)(hWid->dwStyle = (DWORD)dwNewLong);
    case GWL_EXSTYLE:
      return (LONG)(hWid->dwExStyle = (DWORD)dwNewLong);
    case GWL_USERDATA:
      return (LONG)(hWid->userdata = (void*)dwNewLong);
    case GWLP_ID:
      return (LONG)(hWid->id = (int)dwNewLong);
    }
  }
  else {
    LONG* extbuf = (LONG*)(hWid + 1);
    nIndex /= sizeof(LONG);
    if (nIndex < hWid->wc->cbWndExtra) {
      extbuf[nIndex] = dwNewLong;
    }
  }
  return 0;
}
#define WIN_SetStyle SetStyle
ULONG SetStyle(HWID hWid, ULONG set_bits, ULONG clear_bits)
{
  return hWid->dwStyle = (hWid->dwStyle | set_bits) & ~clear_bits;
}
typedef PAINTSTRUCT PAINT_T;
typedef CREATESTRUCT CREATE_T;
#define IsWidgetVisible IsVisible
BOOL IsVisible(HWID hWid)
{
  return !!(hWid->dwStyle & WS_VISIBLE);
}
HWID GetParent(HWID hWid)
{
  return hWid->parent;
}
HWID SetParent(HWID hWid, HWID hWidParent)
{
  if (hWidParent && hWid->parent == hWidParent) {
    return hWidParent;
  }
  if (hWid->parent) {
    hWid->parent->nchild--;
    if (hWid->parent->child_first == hWid) {
      hWid->parent->child_first = hWid->next;
    }
    if (hWid->parent->child_last == hWid) {
      hWid->parent->child_last = hWid->prev;
    }
  }
  if (hWid->next) {
    hWid->next->prev = hWid->prev;
  }
  if (hWid->prev) {
    hWid->prev->next = hWid->next;
  }
  hWid->parent = hWidParent;
  if (hWidParent) {
    hWid->next = hWidParent->child_first;
    if (hWidParent->child_first) {
      hWidParent->child_first->prev = hWid;
    }
    hWidParent->child_first = hWid;
  }
  else {
    if (NULL == hWid->handle) {
      IRECT r = *(IRECT*)(&hWid->rect);
      int style = hWid->dwStyle;
      int style_ex = hWid->dwExStyle;
      int app = 0; // appearance_default
      if (style & WS_MINIMIZEBOX) {
        app |= appearance_minimize;
      }
      if (style & WS_MAXIMIZEBOX) {
        app |= appearance_maximize;
      }
      if (style & WS_THICKFRAME) {
        app |= appearance_sizable;
      }
      if (style & (WS_OVERLAPPED | WS_CAPTION)) {
        app |= appearance_decoration;
        app |= appearance_taskbar;
      }
      if (style_ex & WS_EX_APPWINDOW) {
        app |= appearance_taskbar;
      }
      //style |= (nested ? WS_CHILD : WS_POPUP);
      hWid->handle = create_window(NULL, 0, r, app);
      set_window_caption(hWid->handle->hwnd, hWid->text);
      hWid->handle->form = hWid;
    }
  }
  {
    CREATESTRUCTA ca[1] = {0};
    IRECT r = *(IRECT*)(&hWid->rect);
    ca->x = r.l, ca->y = r.t;
    ca->cx = RCW(&r), ca->cy = RCH(&r);
    ca->style = hWid->dwStyle;
    ca->dwExStyle = hWid->dwExStyle;
    SendMessage(hWid, WM_CREATE, 0, (LPARAM)ca);
    SendMessage(hWid, WM_SIZE, 0, (LPARAM)MAKELONG(ca->cx, ca->cy));
  }
  return hWid->parent;
}
/*****************************************************************
 * GetAncestor (USER32.@)
 */
HWID GetAncestor(HWID hwnd, UINT type)
{
  HWID ret = 0;
  switch (type) {
  case GA_PARENT:
    ret = hwnd->parent;
    break;
  case GA_ROOT:
    ret = hwnd->parent;
    while (ret->parent) {
      ret = ret->parent;
    }
    break;
  case GA_ROOTOWNER:
    ret = hwnd->owner;
    while (ret->owner) {
      ret = ret->owner;
    }
    break;
  }
  return ret;
}
/***********************************************************************
 * IsWidgetEnabled (USER32.@)
 */
BOOL WINAPI IsWidgetEnabled(HWID hWid)
{
  return !(GetWidgetLongW(hWid, GWL_STYLE) & WS_DISABLED);
}
BOOL IsChild(HWID parent, HWID child)
{
  return (child && parent) ? (child->parent == parent) : NULL;
}
/*******************************************************************
 * EnableWidget (USER32.@)
 */
BOOL WINAPI EnableWidget(HWID hwnd, BOOL enable)
{
  BOOL retvalue;
  TRACE("( %p, %d )\n", hwnd, enable);
  retvalue = !IsWidgetEnabled(hwnd);
  if (enable && retvalue) {
    SetStyle(hwnd, 0, WS_DISABLED);
    SendMessageW(hwnd, WM_ENABLE, TRUE, 0);
  }
  else if (!enable && !retvalue) {
    HWID capture_wnd;
    SendMessageW(hwnd, WM_CANCELMODE, 0, 0);
    SetStyle(hwnd, WS_DISABLED, 0);
    if (hwnd == WidgetGetFocus()) {
      WidgetSetFocus(0); /* A disabled window can't have the focus */
    }
    capture_wnd = WidgetGetCapture();
    if (hwnd == capture_wnd || IsChild(hwnd, capture_wnd)) {
      ReleaseCapture(); /* A disabled window can't capture the mouse */
    }
    SendMessageW(hwnd, WM_ENABLE, FALSE, 0);
  }
  return retvalue;
}
LONG WINAPI GdiGetCharDimensions(HDC hdc, LPTEXTMETRICW lptm, LONG* height)
{
  SIZE sz;
  static const WCHAR alphabet[] = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
    'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 0
  };
  if (lptm && !GetTextMetricsW(hdc, lptm)) {
    return 0;
  }
  if (!GetTextExtentPointW(hdc, alphabet, 52, &sz)) {
    return 0;
  }
  if (height) {
    *height = sz.cy;
  }
  return (sz.cx / 26 + 1) / 2;
}
int RegisterAllWidgetClass();
HWID CreateWidget(DWORD dwExStyle, const char* lpClassName, const char* lpWidgetName, DWORD dwStyle,
    int X, int Y, int nWidth, int nHeight, HWID hWndParent, LPVOID lpParam)
{
#define IS_DEFAULT(x) ((x) == CW_USEDEFAULT || (x) == (SHORT)0x8000)
  WIDCLASS* wc = GetWidgetClass(lpClassName);
  HWID hWid = NULL;
  RegisterAllWidgetClass();
  if (IS_DEFAULT(X)) {
    X = 0;
    Y = 0;
  }
  if (IS_DEFAULT(nWidth)) {
    ISIZE screen = primary_monitor_size();
    nWidth = screen.w * 3 / 4;
    nHeight = screen.h * 3 / 4;
    X = screen.w / 8;
    Y = screen.h / 8;
  }
  if (wc) {
    int sz = sizeof(widget_t) + wc->cbWndExtra;
    char* buf = NULL;
    MYREALLOC(buf, sz);
    memset(buf, 0, sz);
    hWid = (HWID)buf;
    hWid->wc = wc;
    hWid->dwStyle = dwStyle;
    hWid->dwExStyle = dwExStyle;
    if (lpWidgetName) {
      int len = strlen(lpWidgetName) + 1;
      MYREALLOC(hWid->text, len);
      memcpy(hWid->text, lpWidgetName, len);
    }
    if (hWndParent) {
      SetRect(&hWid->rect, X, Y, X + nWidth, Y + nHeight);
    }
    else {
      SetRect(&hWid->rect, 0, 0, nWidth, nHeight);
    }
    SetParent((HWID)hWid, hWndParent);
  }
  return hWid;
}
LRESULT CallOriginalClass(HWID hWid, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  return NULL;
}
BOOL RedrawWidget(HWID hWid, CONST RECT* lprcUpdate, HRGN hrgnUpdate, UINT flags)
{
  InvalidateRect(hWid, lprcUpdate, TRUE);
  return TRUE;
}
BOOL UpdateWidget(HWID hWid)
{
  InvalidateRect(hWid, NULL, TRUE);
  return TRUE;
}
typedef struct WIDGETTRACKMOUSEEVENT WIDGETTRACKMOUSEEVENT;
struct WIDGETTRACKMOUSEEVENT {
  DWORD cbSize;
  DWORD dwFlags;
  HWID hwndTrack;
  DWORD dwHoverTime;
};
BOOL TrackMouseEvent(WIDGETTRACKMOUSEEVENT* lpEventTrack)
{
  return TRUE;
}
BOOL MoveWidget(HWID hWid, int X, int Y, int nWidth, int nHeight, BOOL bRepaint)
{
  return TRUE;
}
BOOL SetWidgetPos(HWID hWid, HWID hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags)
{
  return TRUE;
}
HWID GetWidget(HWID hWid, UINT uCmd)
{
  HWID parent = hWid->parent;
  switch (uCmd) {
  case GW_HWNDFIRST:
    return parent ? parent->child_first : NULL;
  case GW_HWNDLAST:
    return parent ? parent->child_last : NULL;
  case GW_HWNDNEXT:
    return hWid->next;
  case GW_HWNDPREV:
    return hWid->prev;
  case GW_CHILD:
    return hWid->child_first;
  case GW_OWNER:
    return hWid->owner;
  }
  return NULL;
}
// ShowWindow
BOOL ShowWidget(HWID hWid, int nCmdShow)
{
  SetStyle(hWid, WS_VISIBLE, 0);
  return 0;
}
HDC BeginPaint(HWID hWid, PAINTSTRUCT* lpPaint)
{
  while (hWid->parent) {
    hWid = hWid->parent;
  }
  if (lpPaint) {
    memset(lpPaint, 0, sizeof(PAINTSTRUCT));
    lpPaint->hdc = hWid->handle->hdc;
    lpPaint->rcPaint = hWid->rect;
    if (hWid->handle->hdc) {
      lpPaint->fRestore = SaveDC(lpPaint->hdc);
    }
  }
  return hWid->handle->hdc;
}
BOOL EndPaint(HWID hWid, CONST PAINTSTRUCT* lpPaint)
{
  if (lpPaint->hdc) {
    RestoreDC(lpPaint->hdc, lpPaint->fRestore);
  }
  return 0;
}
#define WIN_GetFullHandle(x) (x)
/***********************************************************************
 * GetNextDlgGroupItem (USER32.@)
 *
 * Corrections to MSDN documentation
 *
 * (Under Windows 2000 at least, where hwndDlg is not actually a dialog)
 * 1. hwndCtrl can be hwndDlg in which case it behaves as for NULL
 * 2. Prev of NULL or hwndDlg fails
 */
HWID WINAPI GetNextDlgGroupItem(HWID hwndDlg, HWID hwndCtrl, BOOL fPrevious)
{
  HWID hwnd, hwndNext, retvalue, hwndLastGroup = 0;
  BOOL fLooped = FALSE;
  BOOL fSkipping = FALSE;
  hwndDlg = WIN_GetFullHandle(hwndDlg);
  hwndCtrl = WIN_GetFullHandle(hwndCtrl);
  if (hwndDlg == hwndCtrl) {
    hwndCtrl = NULL;
  }
  if (!hwndCtrl && fPrevious) {
    return 0;
  }
  if (hwndCtrl) {
    if (!IsChild(hwndDlg, hwndCtrl)) {
      return 0;
    }
  }
  else {
    /* No ctrl specified -> start from the beginning */
    if (!(hwndCtrl = GetWidget(hwndDlg, GW_CHILD))) {
      return 0;
    }
    /* MSDN is wrong. fPrevious does not result in the last child */
    /* Maybe that first one is valid. If so then we don't want to skip it*/
    if ((GetWidgetLong(hwndCtrl, GWL_STYLE) & (WS_VISIBLE | WS_DISABLED)) == WS_VISIBLE) {
      return hwndCtrl;
    }
  }
  /* Always go forward around the group and list of controls; for the
  * previous control keep track; for the next break when you find one
  */
  retvalue = hwndCtrl;
  hwnd = hwndCtrl;
  while (hwndNext = GetWidget(hwnd, GW_HWNDNEXT),
      1) {
    while (!hwndNext) {
      /* Climb out until there is a next sibling of the ancestor or we
      * reach the top (in which case we loop back to the start)
      */
      if (hwndDlg == GetParent(hwnd)) {
        /* Wrap around to the beginning of the list, within the same
        * group. (Once only)
        */
        if (fLooped) {
          goto end;
        }
        fLooped = TRUE;
        hwndNext = GetWidget(hwndDlg, GW_CHILD);
      }
      else {
        hwnd = GetParent(hwnd);
        hwndNext = GetWidget(hwnd, GW_HWNDNEXT);
      }
    }
    hwnd = hwndNext;
    /* Wander down the leading edge of controlparents */
    while ((GetWidgetLongW(hwnd, GWL_EXSTYLE) & WS_EX_CONTROLPARENT) &&
        ((GetWidgetLongW(hwnd, GWL_STYLE) & (WS_VISIBLE | WS_DISABLED)) == WS_VISIBLE) &&
        (hwndNext = GetWidget(hwnd, GW_CHILD))) {
      hwnd = hwndNext;
    }
    /* Question. If the control is a control parent but either has no
    * children or is not visible/enabled then if it has a WS_GROUP does
    * it count? For that matter does it count anyway?
    * I believe it doesn't count.
    */
    if ((GetWidgetLongW(hwnd, GWL_STYLE) & WS_GROUP)) {
      hwndLastGroup = hwnd;
      if (!fSkipping) {
        /* Look for the beginning of the group */
        fSkipping = TRUE;
      }
    }
    if (hwnd == hwndCtrl) {
      if (!fSkipping) {
        break;
      }
      if (hwndLastGroup == hwnd) {
        break;
      }
      hwnd = hwndLastGroup;
      fSkipping = FALSE;
      fLooped = FALSE;
    }
    if (!fSkipping &&
        (GetWidgetLongW(hwnd, GWL_STYLE) & (WS_VISIBLE | WS_DISABLED)) ==
        WS_VISIBLE) {
      retvalue = hwnd;
      if (!fPrevious) {
        break;
      }
    }
  }
end:
  return retvalue;
}
/***********************************************************************
 * WIN_IsWindowDrawable
 *
 * hwnd is drawable when it is visible, all parents are not
 * minimized, and it is itself not minimized unless we are
 * trying to draw its default class icon.
 */
BOOL WIN_IsWidgetDrawable(HWID hwnd, BOOL icon)
{
  HWID parent;
  LONG style = GetWidgetLong(hwnd, GWL_STYLE);
  if (!(style & WS_VISIBLE)) {
    return FALSE;
  }
  if ((style & WS_MINIMIZE) && icon && GetClassLong(hwnd, GCLP_HICON)) {
    return FALSE;
  }
  parent = hwnd;
  while (parent = GetParent(parent)) {
    if ((GetWidgetLongW(parent, GWL_STYLE) & (WS_VISIBLE | WS_MINIMIZE)) != WS_VISIBLE) {
      return FALSE;
    }
  }
  return TRUE;
}
const char* SPY_GetMsgName(UINT msg)
{
  switch (msg) {
#define MSG2STR(a, b, c) case a: return #a " " #b " " c ;
#include "ui/msg2string.txt"
#undef MSG2STR
  }
  return "";
}
/*************************************************************************
 * fix_caret
 *
 * Helper for ScrollWindowEx:
 * If the return value is 0, no special caret handling is necessary.
 * Otherwise the return value is the handle of the window that owns the
 * caret. Its caret needs to be hidden during the scroll operation and
 * moved to new_caret_pos if move_caret is TRUE.
 */
static HWND fix_caret(HWND hWnd, const RECT* scroll_rect, INT dx, INT dy,
    UINT flags, LPBOOL move_caret, LPPOINT new_caret_pos)
{
  GUITHREADINFO info;
  RECT rect, mapped_rcCaret;
  BOOL hide_caret = FALSE;
  info.cbSize = sizeof(info);
  if (!GetGUIThreadInfo(GetCurrentThreadId(), &info)) {
    return 0;
  }
  if (!info.hwndCaret) {
    return 0;
  }
  if (info.hwndCaret == hWnd) {
    /* Move the caret if it's (partially) in the source rectangle */
    if (IntersectRect(&rect, scroll_rect, &info.rcCaret)) {
      *move_caret = TRUE;
      hide_caret = TRUE;
      new_caret_pos->x = info.rcCaret.left + dx;
      new_caret_pos->y = info.rcCaret.top + dy;
    }
    else {
      *move_caret = FALSE;
      /* Hide the caret if it's in the destination rectangle */
      rect = *scroll_rect;
      OffsetRect(&rect, dx, dy);
      hide_caret = IntersectRect(&rect, &rect, &info.rcCaret);
    }
  }
  else {
    if ((flags & SW_SCROLLCHILDREN) && IsChild(hWnd, info.hwndCaret)) {
      *move_caret = FALSE;
      /* Hide the caret if it's in the source or in the destination
      rectangle */
      mapped_rcCaret = info.rcCaret;
      MapWindowPoints(info.hwndCaret, hWnd, (LPPOINT)&mapped_rcCaret, 2);
      if (IntersectRect(&rect, scroll_rect, &mapped_rcCaret)) {
        hide_caret = TRUE;
      }
      else {
        rect = *scroll_rect;
        OffsetRect(&rect, dx, dy);
        hide_caret = IntersectRect(&rect, &rect, &mapped_rcCaret);
      }
    }
    else {
      return 0;
    }
  }
  if (hide_caret) {
    HideCaret(info.hwndCaret);
    return info.hwndCaret;
  }
  else {
    return 0;
  }
}
// ScrollWindow
BOOL ScrollWidget(HWID hWid, int XAmount, int YAmount, CONST RECT* lpRect, CONST RECT* lpClipRect)
{
  //InvalidateRect(hWid, lpRect, TRUE);
  return 0;
}
// ScrollWindowEx
BOOL ScrollWidgetEx(HWID hWid, int XAmount, int YAmount, CONST RECT* lpRect, CONST RECT* lpClipRect, HRGN hrgnUpdate, LPRECT prcUpdate, UINT flags)
{
  //InvalidateRect(hWid, lpRect, TRUE);
  return 0;
}
BOOL ClientToScreen(HWID hWid, LPPOINT lpPoint)
{
  return 0;
}

