static void GetWindowRectParentClient(HWND wnd, IRECT* rc /*out*/) // get window rect in parent's client space.
{
  GetWindowRect(wnd, (RECT*)rc);
  MapWindowPoints(HWND_DESKTOP, GetParent(wnd), (POINT*) rc, 2);
}
//
static BOOL HasWindowPos(HWND wnd, IRECT const* rc) // return 'TRUE' if window has 'rc' position.
{
  IRECT rcNow;
  GetWindowRectParentClient(wnd, &rcNow);
  return RCEQ(&rcNow, rc) != 0;
}
//
static void MoveChangedWindow(HWND wnd, IRECT const* rc, BOOL redraw)
{
  if (HasWindowPos(wnd, rc) == FALSE) {
    MoveWindow(wnd, rc->l, rc->t, rc->r - rc->l, rc->b - rc->t, (redraw == TRUE ? TRUE : FALSE));
  }
}
//
static void MoveWindowDWP(HDWP dwp, HWND wnd, IRECT const* rc, BOOL redraw)
{
  if (HasWindowPos(wnd, rc) == FALSE)
    DeferWindowPos(dwp, wnd, 0,
        rc->l, rc->t, rc->r - rc->l, rc->b - rc->t,
        SWP_NOZORDER | SWP_NOACTIVATE | (redraw == TRUE ? 0 : SWP_NOREDRAW));
}
//
static void MoveShowWindowDWP(HDWP dwp, HWND wnd, IRECT const* rc, BOOL redraw)
{
  BOOL needShow = (IsWindowVisible(wnd) == 0);
  //
  if (HasWindowPos(wnd, rc) == FALSE || needShow == TRUE)
    DeferWindowPos(dwp, wnd, 0,
        rc->l, rc->t, rc->r - rc->l, rc->b - rc->t,
        SWP_NOZORDER | SWP_NOACTIVATE | (needShow == TRUE ? SWP_SHOWWINDOW : 0) |
        (redraw == TRUE ? 0 : SWP_NOREDRAW));
}
//
// get address of SetLayeredWindowAttributes function.
typedef BOOL(__stdcall* SetLayeredWindowAttributes_t)(HWND, COLORREF, BYTE, DWORD);
static SetLayeredWindowAttributes_t GetLayerFunctionPtr()
{
  HMODULE hModule = LoadLibrary(_T("User32.dll"));
  typedef BOOL (__stdcall * SetLayeredWindowAttributes_fun_t)(HWND, COLORREF, BYTE, DWORD);
  SetLayeredWindowAttributes_fun_t ptr;
  if (hModule == NULL) {
    return NULL;
  }
  ptr = (SetLayeredWindowAttributes_fun_t) GetProcAddress(hModule, "SetLayeredWindowAttributes");
  FreeLibrary(hModule);
  return ptr;
}
// transparence 0 ~ 100
static BOOL SetWindowTransparence(HWND hwnd, COLORREF clrMask, int transparence)
{
  static SetLayeredWindowAttributes_t fun = NULL;
  BYTE transp = (BYTE)(((100 - transparence) * 255) / 100);
  if (!fun) {
    fun = GetLayerFunctionPtr();
  }
  return fun(hwnd, clrMask, transp, 0x00000001 /*LWA_COLORKEY*/ | 0x00000002 /*LWA_ALPHA*/);
}
//
static void SetLayered(HWND wnd, BOOL layered)
{
  LONG styleEx = GetWindowLong(wnd, GWL_EXSTYLE);
  //
  if (layered == TRUE) {
    if ((styleEx & 0x00080000 /*WS_EX_LAYERED*/) == 0) {
      SetWindowLong(wnd, GWL_EXSTYLE, (styleEx | 0x00080000 /*WS_EX_LAYERED*/));
    }
  }
  else if ((styleEx & 0x00080000 /*WS_EX_LAYERED*/) != 0) {
    SetWindowLong(wnd, GWL_EXSTYLE, (styleEx & ~0x00080000 /*WS_EX_LAYERED*/));
    RedrawWindow(wnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);
  }
}
//
static BOOL IsDescendantWindow(HWND wndDst, HWND wndSrc)
{
  HWND h;
  for (h = wndSrc; h != NULL; h = GetParent(h))
    if (h == wndDst) {
      return TRUE;
    }
  return FALSE;
}
//
static BOOL IsPointInWindow(HWND wndDst, int x, int y)
{
  POINT pt = {x, y};
  return IsDescendantWindow(wndDst, WindowFromPoint(pt));
}
//
// return 0 - wnd1=wnd2.
// return >0 - wnd1 above wnd2.
// return <0 - wnd2 above wnd1.
static int CompareZOrderWindows(HWND wnd1, HWND wnd2)
{
  HWND h;
  ASSERT(GetParent(wnd1) == GetParent(wnd2));
  //
  if (wnd1 == wnd2) {
    return 0;
  }
  //
  for (h = GetNextWindow(wnd1, GW_HWNDNEXT); h != NULL; h = GetNextWindow(h, GW_HWNDNEXT))
    if (h == wnd2) {
      return 1;
    }
  return -1;
}
//
static BOOL HasWindowRegion(HWND wnd)
{
  BOOL res = FALSE;
  //
  HRGN rgn = CreateRectRgn(0, 0, 0, 0);
  if (rgn != NULL) {
    int regionType = GetWindowRgn(wnd, rgn);
    res = (regionType != ERROR);
    DeleteObject(rgn);
  }
  return res;
}
//
static int GetMonitorRect(HWND wnd, IRECT* prc)
{
  HMONITOR hMonitor = MonitorFromWindow(wnd, MONITOR_DEFAULTTONEAREST);
  MONITORINFO info;
  info.cbSize = sizeof(MONITORINFO);
  GetMonitorInfo(hMonitor, &info);
  *prc = *(IRECT*)&info.rcMonitor;
  return 0;
}
//
static int GetMonitorWorkAreaRect(HWND wnd, IRECT* prc)
{
  HMONITOR hMonitor = MonitorFromWindow(wnd, MONITOR_DEFAULTTONEAREST);
  MONITORINFO info;
  info.cbSize = sizeof(MONITORINFO);
  GetMonitorInfo(hMonitor, &info);
  *prc = *(IRECT*)&info.rcWork;
  return 0;
}
//
static BOOL CorrectWindowPos(BOOL smallCaption, IRECT* rc /*in,out*/)
{
  IRECT rect = (*rc);
  rect.t += GetSystemMetrics(SM_CXBORDER);
  rect.b = rect.t +
      (smallCaption == FALSE ? GetSystemMetrics(SM_CYCAPTION) : GetSystemMetrics(SM_CYSMCAPTION));
  //
  if (MonitorFromRect((RECT*)&rect, MONITOR_DEFAULTTONULL) == NULL) {
    HMONITOR hMonitor = MonitorFromRect((RECT*)&rect, MONITOR_DEFAULTTONEAREST);
    //
    MONITORINFO info;
    info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(hMonitor, &info);
    //
    rect = *rc;
    rc->t = max(info.rcWork.top + 3, (info.rcWork.top + info.rcWork.bottom - RCH(&rect)) / 2);
    rc->b = rc->t + RCH(&rect);
    rc->l = max(info.rcWork.left + 3, (info.rcWork.left + info.rcWork.right - RCW(&rect)) / 2);
    rc->r = rc->l + RCW(&rect);
    //
    return TRUE; // êîððåêöèÿ áûëà.
  }
  return FALSE;
}
//
static char* RegisterWndClass(TCHAR const* namePrefix, UINT classStyle, HCURSOR cursor, HBRUSH background, HICON icon, char* className, int maxstrlen)
{
  HINSTANCE hInst = GetModuleHandle(0);
  WNDCLASSEX wndcls;
  ASSERT(namePrefix != NULL);
  //
  _snprintf(className, maxstrlen, _T("%s:%x:%x:%x:%x:%x"), namePrefix, (UINT) hInst, classStyle, (UINT) cursor, (UINT) background, (UINT) icon);
  //
  if (GetClassInfoEx(hInst, className, &wndcls) == 0) {
    wndcls.style = classStyle;
    wndcls.lpfnWndProc = DefWindowProc;
    wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
    wndcls.hInstance = hInst;
    wndcls.hIcon = icon;
    wndcls.hCursor = cursor;
    wndcls.hbrBackground = background;
    wndcls.lpszMenuName = NULL;
    wndcls.lpszClassName = className;
    //
    if (RegisterClassEx(&wndcls) == 0) {
      return NULL;
    }
  }
  return className;
}
//
static const char* RegWndClass(const char* ClassName, UINT classStyle, HCURSOR cursor, HBRUSH background, HICON icon, WNDPROC pWndProc)
{
  HINSTANCE hInst = GetModuleHandle(0);
  WNDCLASSEX wndcls = {0};
  HWND hwnd = NULL;
  //
  if (GetClassInfoEx(hInst, ClassName, &wndcls) == 0) {
    wndcls.cbSize = sizeof(WNDCLASSEX);
    wndcls.style = classStyle;
    wndcls.lpfnWndProc = pWndProc;
    wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
    wndcls.hInstance = hInst;
    wndcls.hIcon = icon;
    wndcls.hCursor = cursor;
    wndcls.hbrBackground = background;
    wndcls.lpszMenuName = NULL;
    wndcls.lpszClassName = ClassName;
    //
    if (RegisterClassEx(&wndcls) == 0) {
      return NULL;
    }
  }
  return ClassName;
}
//
static char* RegisterWndClass1(TCHAR const* namePrefix, char* className, int maxstrlen)
{
  return RegisterWndClass(namePrefix, CS_DBLCLKS, LoadCursor(NULL, IDC_ARROW), NULL, NULL, className, maxstrlen);
}
//
static void SendMouseMessage(UINT msg, int x, int y, BOOL absolute)
{
  INPUT input;
  input.type = INPUT_MOUSE;
  input.mi.dx = x;
  input.mi.dy = y;
  input.mi.mouseData = 0;
  input.mi.dwFlags = (absolute == TRUE ? MOUSEEVENTF_ABSOLUTE : 0);
  input.mi.time = 0;
  input.mi.dwExtraInfo = 0;
  //
  switch (msg) {
  case WM_MOUSEMOVE:
    input.mi.dwFlags |= MOUSEEVENTF_MOVE;
    break;
  case WM_LBUTTONDOWN:
    input.mi.dwFlags |= MOUSEEVENTF_LEFTDOWN;
    break;
  case WM_LBUTTONUP:
    input.mi.dwFlags |= MOUSEEVENTF_LEFTUP;
    break;
  case WM_RBUTTONDOWN:
    input.mi.dwFlags |= MOUSEEVENTF_RIGHTDOWN;
    break;
  case WM_RBUTTONUP:
    input.mi.dwFlags |= MOUSEEVENTF_RIGHTUP;
    break;
  case WM_MBUTTONDOWN:
    input.mi.dwFlags |= MOUSEEVENTF_MIDDLEDOWN;
    break;
  case WM_MBUTTONUP:
    input.mi.dwFlags |= MOUSEEVENTF_MIDDLEUP;
    break;
  }
  SendInput(1, &input, sizeof(input));
}
//
static BOOL PtInTriangle(IPOINT const pt, IPOINT const v1, IPOINT const v2, IPOINT const v3)
{
  IPOINT pts[ 3 ];
  int ret;
  HRGN rgn;
  pts[ 0 ] = v1;
  pts[ 1 ] = v2;
  pts[ 2 ] = v3;
  rgn = CreatePolygonRgn((POINT*)pts, sizeof(pts) / sizeof(POINT), WINDING);
  ret = PtInRegion(rgn, pt.x, pt.y) != 0;
  DeleteObject(rgn);
  return ret;
}
//
static IRECT RotateRect90(IRECT rc)
{
  return iRECT(rc.t, rc.r, rc.b, rc.l);
}
//
static BOOL IsIntersectRect(IRECT const* rc1, IRECT const* rc2)
{
  return ((rc2->l < rc1->l && rc2->r > rc1->l) || // compare X.
      (rc2->l < rc1->r && rc2->r > rc1->r) ||
      (rc2->l >= rc1->l && rc2->r <= rc1->r)) &&
      ((rc2->t < rc1->t && rc2->b > rc1->t) || // compare Y.
          (rc2->t < rc1->b && rc2->b > rc1->b) ||
          (rc2->t >= rc1->t && rc2->b <= rc1->b));
}
//
// return TRUE - rcDst contains rcSrc rect.
static BOOL IsContainRect(IRECT const* rcDst, IRECT const* rcSrc)
{
  return (rcSrc->l >= rcDst->l && rcSrc->r <= rcDst->r) &&
      (rcSrc->t >= rcDst->t && rcSrc->b <= rcDst->b);
}
//
#pragma warning(push)
#pragma warning(disable : 4201) // nonstandard extension used. 
//
#include <Mmsystem.h>
#pragma comment (lib, "Winmm.lib") // for Multimedia Timers. 
//
typedef int (*OnTimer_t)(struct Timer* timer);
typedef struct Timer {
  int m_iTime;
  OnTimer_t OnTimer;
  void* user;
  UINT m_uTimerID;
  int id;
} Timer;
static void CALLBACK myTimeProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
  Timer* p = (Timer*) dwUser;
  if (p && p->OnTimer) {
    p->OnTimer(p);
  }
  return ;
}
static int mySetTime(Timer* s, OnTimer_t OnTimer, void* user, int id, int time)
{
  if (0 == s->m_uTimerID) {
    s->OnTimer = OnTimer;
    s->id = id;
    s->user = user;
    s->m_uTimerID = timeSetEvent(time, 0, myTimeProc, (DWORD) s, TIME_PERIODIC);
    if (s->m_uTimerID) {
      s->m_iTime = time;
      return 1;
    }
  }
  return 0;
}
static int myKillTimer(Timer* s)
{
  timeKillEvent(s->m_uTimerID);
  s->user = 0;
  s->m_uTimerID = 0;
  return 0;
}
typedef void (*WindowsMsg_t)(struct WindowsHook* s, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
typedef struct WindowsHook {
  WindowsMsg_t OnMouseMsg;
  WindowsMsg_t OnKeyMsg;
  WindowsMsg_t OnWndMsg;
  void* user;
  struct WindowsHook* prev;
  struct WindowsHook* next;
} WindowsHook;
typedef struct WindowsHookData {
  HHOOK h_mouse_hook;
  HHOOK h_key_hook;
  HHOOK h_wnd_hook;
  WindowsHook* hook_data;
  CRITICAL_SECTION critSect;
} WindowsHookData;
static WindowsHookData* GetHookDataPtr()
{
  static WindowsHookData hd[1] = {0};
  return hd;
}
static int WindowsHook_free()
{
  WindowsHookData* s = GetHookDataPtr();
  s->hook_data = NULL;
  if (s->h_mouse_hook != NULL) {
    UnhookWindowsHookEx(s->h_mouse_hook);
    s->h_mouse_hook = NULL;
  }
  if (s->h_key_hook != NULL) {
    UnhookWindowsHookEx(s->h_key_hook);
    s->h_key_hook = NULL;
  }
  DeleteCriticalSection(&s->critSect);
  return 0;
}
static LRESULT __stdcall myMouseHookProc(int code, WPARAM wParam, LPARAM lParam)
{
  WindowsHookData* s = GetHookDataPtr();
  WindowsHook* i = s->hook_data;
  POINT pt = ((MOUSEHOOKSTRUCT*) lParam) ->pt;
  HWND hwnd = ((MOUSEHOOKSTRUCT*) lParam) ->hwnd;
  int x = pt.x, y = pt.y;
  if (code == HC_ACTION) {
    //
    //
    switch (wParam) {
    case WM_NCMOUSEMOVE:
      wParam = WM_MOUSEMOVE;
      break;
    case WM_NCLBUTTONDOWN:
      wParam = WM_LBUTTONDOWN;
      break;
    case WM_NCLBUTTONUP:
      wParam = WM_LBUTTONUP;
      break;
    case WM_NCRBUTTONDOWN:
      wParam = WM_RBUTTONDOWN;
      break;
    case WM_NCMBUTTONDOWN:
      wParam = WM_MBUTTONDOWN;
      break;
    };
    EnterCriticalSection(&s->critSect);
    for (i = s->hook_data; i; i = i->next) {
      if (i->OnMouseMsg) {
        i->OnMouseMsg(i, hwnd, wParam, 0, MAKELONG(pt.x, pt.y));
      }
    }
    LeaveCriticalSection(&s->critSect);
  }
  return CallNextHookEx(s->h_mouse_hook, code, wParam, lParam);
}
static LRESULT __stdcall myKeyHookProc(int code, WPARAM wParam, LPARAM lParam)
{
  WindowsHookData* s = GetHookDataPtr();
  WindowsHook* i = s->hook_data;
  //
  if (code == HC_ACTION) { // key is down.
    EnterCriticalSection(&s->critSect);
    for (i = s->hook_data; i; i = i->next) {
      if (i->OnKeyMsg) {
        i->OnKeyMsg(i, 0, 0, wParam, lParam);
      }
    }
    LeaveCriticalSection(&s->critSect);
  }
  return CallNextHookEx(s->h_key_hook, code, wParam, lParam);
}
static LRESULT __stdcall myWndHookProc(int code, WPARAM wParam, LPARAM lParam)
{
  WindowsHookData* s = GetHookDataPtr();
  WindowsHook* i = s->hook_data;
  //
  if (code == HC_ACTION) { // key is down.
    EnterCriticalSection(&s->critSect);
    for (i = s->hook_data; i; i = i->next) {
      if (i->OnWndMsg) {
        i->OnWndMsg(i, 0, 0, wParam, lParam);
      }
    }
    LeaveCriticalSection(&s->critSect);
  }
  return CallNextHookEx(s->h_mouse_hook, code, wParam, lParam);
}
static int WindowsHook_del(WindowsHook* p)
{
  WindowsHookData* s = GetHookDataPtr();
  WindowsHook* prev;
  if (p && s->hook_data) {
    EnterCriticalSection(&s->critSect);
    prev = p->prev;
    if (prev) {
      prev->next = p->next;
    }
    else {
      s->hook_data = p->next;
    }
    LeaveCriticalSection(&s->critSect);
  }
  return 0;
}
static int WindowsHook_addHook(WindowsHook* p, WindowsMsg_t Mousefun, WindowsMsg_t Keyfun, WindowsMsg_t Wndfun, void* user)
{
  if (Mousefun || Keyfun || Wndfun) {
    WindowsHookData* s = GetHookDataPtr();
    InitializeCriticalSection(&s->critSect);
    if (Mousefun && NULL == s->h_mouse_hook) {
      s->h_mouse_hook = SetWindowsHookEx(WH_MOUSE, (HOOKPROC) myMouseHookProc, NULL, GetCurrentThreadId());
    }
    if (Keyfun && NULL == s->h_key_hook) {
      s->h_key_hook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC) myKeyHookProc, NULL, GetCurrentThreadId());
    }
    if (Wndfun && NULL == s->h_wnd_hook) {
      s->h_wnd_hook = SetWindowsHookEx(WH_CALLWNDPROC, (HOOKPROC) myWndHookProc, NULL, GetCurrentThreadId());
    }
    p->OnMouseMsg = Mousefun;
    p->OnKeyMsg = Keyfun;
    p->OnWndMsg = Wndfun;
    p->user = user;
    p->next = NULL;
    p->prev = NULL;
    EnterCriticalSection(&s->critSect);
    if (s->hook_data) {
      p->next = s->hook_data->next;
      if (s->hook_data->next) {
        s->hook_data->next->prev = p;
      }
    }
    s->hook_data = p;
    LeaveCriticalSection(&s->critSect);
  }
  return 0;
}
static HANDLE LoadImageT(const char* lpszName, UINT uType, int cxDesired, int cyDesired)
{
  HINSTANCE hinst = GetModuleHandle(0);
  return LoadImage(hinst, lpszName, uType, cxDesired, cyDesired, LR_LOADFROMFILE);
}
static HBITMAP LoadBitmapT(const char* lpszName, int nCxDesired, int nCyDesired)
{
  return (HBITMAP) LoadImageT(lpszName, IMAGE_BITMAP, nCxDesired, nCyDesired);
}
static HICON LoadIconT(const char* lpszName, int nCxDesired, int nCyDesired)
{
  return (HICON) LoadImageT(lpszName, IMAGE_ICON, nCxDesired, nCyDesired);
}
static HCURSOR LoadCursorT(const char* lpszName)
{
  return (HCURSOR) LoadImageT(lpszName, IMAGE_CURSOR, 0, 0);
}
// cx Ã¿·ùÍ¼Ïñ¿í¶È
// cGrow Í¼ÏñÊýÁ¿
// crMask Éú³ÉÑÚÂëµÄÑÕÉ«²ÎÊý
// uType Í¼ÏñÀàÐÍ
static HIMAGELIST LoadImageListT(const char* lpszName, int cx, int cGrow, COLORREF crMask, UINT uType, UINT uFlags)
{
  HINSTANCE hinst = GetModuleHandle(0);
  uFlags |= LR_LOADFROMFILE;
  return ImageList_LoadImage(hinst, lpszName, cx, cGrow, crMask, uType, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
}
static HIMAGELIST LoadImageList(const char* lpszName, int cols, int rows, COLORREF crMask, int* cx, int* cy)
{
  HINSTANCE hinst = GetModuleHandle(0);
  HBITMAP bmp = NULL;
  BITMAP bmpInfo = {0};
  HIMAGELIST m_Imagelist = NULL;
  cols = MAX(cols, 1);
  rows = MAX(rows, 1);
  if ((bmp = LoadBitmapT(lpszName, 0, 0)) && GetBitmapT(bmp, &bmpInfo) &&
      (m_Imagelist = ImageList_Create(bmpInfo.bmWidth / cols, bmpInfo.bmHeight / rows, bmpInfo.bmBitsPixel, 3, 0)) &&
      ImageList_AddMasked(m_Imagelist, bmp, crMask) != -1) {}
  if (bmp) {
    DeleteObject(bmp);
  }
  if (cx) {
    *cx = bmpInfo.bmWidth / cols;
  }
  if (cy) {
    *cy = bmpInfo.bmWidth / cols;
  }
  return m_Imagelist;
}

