#include <windows.h>
#include "cstd.h"
typedef struct window_t {
  struct window_t* prev, * next;
  void* hwnd;
  char* name;
  int id;
} window_t;
static window_t* window_opt(window_t* p0_add, window_t* p0_remove)
{
  static window_t* hg_windows = NULL;
  static unsigned int add_id = 0;
  window_t* p0 = NULL;
  if (p0_add) {
    p0_add->id = add_id++;
    p0 = p0_add;
    if (hg_windows) {
      hg_windows->prev = p0;
      p0->next = hg_windows;
    }
    else {
      p0->next = NULL;
    }
    p0->prev = NULL;
    hg_windows = p0;
  }
  if (p0_remove) {
    p0 = p0_remove;
    if (p0->prev) {
      p0->prev->next = p0->next;
    }
    else {
      hg_windows = p0->next;
    }
    if (p0->next) {
      p0->next->prev = p0->prev;
    }
  }
  return hg_windows;
}
static const char* highGUIclassName = "HighGUI struct";
static HINSTANCE hg_hinstance = 0;
static int cvDestroyAllWindows()
{
  window_t* window = window_opt(NULL, NULL);
  while (window) {
    HWND hwnd = (HWND)window->hwnd;
    window = window->next;
    SendMessageA(hwnd, WM_CLOSE, 0, 0);
  }
  return 0;
}
static void icvCleanupHighgui(void)
{
  cvDestroyAllWindows();
  UnregisterClassA(highGUIclassName, hg_hinstance);
}
typedef window_t CvWindow;
//#define GetWindowLongPtr GetWindowLongA
#define icvGetWindowLongPtr GetWindowLongA
#define icvSetWindowLongPtr( hwnd, id, ptr ) SetWindowLongA( hwnd, id, (size_t)ptr )
//#define SetWindowLongPtr( hwnd, id, ptr ) SetWindowLongA( hwnd, id, (size_t)ptr )
#define icvGetClassLongPtr GetClassLong
#define GWL_WNDPROC (-4)
#define GWL_HINSTANCE (-6)
#define GWL_HWNDPARENT (-8)
#define GWL_STYLE (-16)
#define GWL_EXSTYLE (-20)
#define GWL_USERDATA (-21)
#define GWL_ID (-12)
#define CV_USERDATA GWL_USERDATA
#define CV_WNDPROC GWL_WNDPROC
#define CV_HCURSOR GCL_HCURSOR
#define CV_HBRBACKGROUND GCL_HBRBACKGROUND
static CvWindow* icvWindowByHWND(HWND hwnd)
{
  CvWindow* window = (CvWindow*) icvGetWindowLongPtr(hwnd, CV_USERDATA);
  return window;
}
static int icvRemoveWindow(CvWindow* window)
{
  if (window->hwnd) {
    icvSetWindowLongPtr(window->hwnd, CV_USERDATA, 0);
  }
  window_opt(NULL, window);
  FREE(window);
  return 0;
}
static int icvScreenToClient(HWND hwnd, IRECT* rect)
{
  POINT p;
  p.x = rect->l;
  p.y = rect->t;
  ScreenToClient(hwnd, &p);
  OffsetRect((RECT*)rect, p.x - rect->l, p.y - rect->t);
  return 0;
}
static LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  CvWindow* window = icvWindowByHWND(hwnd);
  if (!window) {
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
  switch (uMsg) {
    //#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
#ifdef WM_MOUSEWHEEL
  case WM_MOUSEWHEEL:
    if (window->on_mouse) {
      POINT pt;
      IRECT rect;
      ISIZE size = {0, 0};
      int flags = (wParam & MK_LBUTTON ? CV_EVENT_FLAG_LBUTTON : 0) |
          (wParam & MK_RBUTTON ? CV_EVENT_FLAG_RBUTTON : 0) |
          (wParam & MK_MBUTTON ? CV_EVENT_FLAG_MBUTTON : 0) |
          (wParam & MK_CONTROL ? CV_EVENT_FLAG_CTRLKEY : 0) |
          (wParam & MK_SHIFT ? CV_EVENT_FLAG_SHIFTKEY : 0) |
          (GetKeyState(VK_MENU) < 0 ? CV_EVENT_FLAG_ALTKEY : 0);
      int event = GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? CV_EVENT_MOUSEWHEELUP : CV_EVENT_MOUSEWHEELDOWN;
      pt.x = LOWORD(lParam);
      pt.y = HIWORD(lParam);
      GetClientRect(window->hwnd, (RECT*)&rect);
      GetBitmapData(window->dc, &size.w, &size.h, 0, 0);
      window->on_mouse(event, pt.x * size.w / MAX(rect.r - rect.l, 1),
          pt.y * size.h / MAX(rect.b - rect.t, 1), flags,
          window->on_mouse_param);
    }
    break;
#endif
  case WM_PAINT:
    if (0) {
      PAINTSTRUCT paint;
      HDC hDC;
      hDC = BeginPaint(hwnd, &paint);
      SetStretchBltMode(hDC, COLORONCOLOR);
      EndPaint(hwnd, &paint);
    }
    else {
      return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    break;
  case WM_DESTROY:
    icvRemoveWindow(window);
    // Do nothing!!!
    //PostQuitMessage(0);
    break;
  case WM_GETMINMAXINFO:
    if (1) {
      MINMAXINFO* minmax = (MINMAXINFO*) lParam;
      LRESULT retval = DefWindowProc(hwnd, uMsg, wParam, lParam);
      minmax->ptMinTrackSize.y = 100;
      minmax->ptMinTrackSize.x = 100;
      return retval;
    }
    break;
  case WM_WINDOWPOSCHANGED: {
    WINDOWPOS* pos = (WINDOWPOS*) lParam;
    // Update the toolbar position/size
    break;
  }
  case WM_ACTIVATE:
    if (LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE) {
      SetFocus(window->hwnd);
    }
    break;
  case WM_ERASEBKGND: {
    IRECT cr = {0}, tr = {0}, wrc = {0};
    HRGN rgn, rgn1, rgn2;
    int ret;
    HDC hDC = (HDC) wParam;
    GetWindowRect(window->hwnd, (RECT*)&cr);
    icvScreenToClient(window->hwnd, &cr);
    GetClientRect(window->hwnd, (RECT*)&wrc);
    rgn = CreateRectRgn(0, 0, wrc.r, wrc.b);
    rgn1 = CreateRectRgn(cr.l, cr.t, cr.r, cr.b);
    rgn2 = CreateRectRgn(tr.l, tr.t, tr.r, tr.b);
    ret = CombineRgn(rgn, rgn, rgn1, RGN_DIFF);
    ret = CombineRgn(rgn, rgn, rgn2, RGN_DIFF);
    if (ret != NULLREGION && ret != ERROR) {
      FillRgn(hDC, rgn, (HBRUSH) icvGetClassLongPtr(hwnd, CV_HBRBACKGROUND));
    }
    DeleteObject(rgn);
    DeleteObject(rgn1);
    DeleteObject(rgn2);
  }
  return 1;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
static int cvInitSystem()
{
  static int wasInitialized = 0;
  // check initialization status
  if (!wasInitialized) {
    WNDCLASS wndc;
    // Initialize the stogare
    // Register the struct
    wndc.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
    wndc.cbClsExtra = 0;
    wndc.cbWndExtra = 0;
    wndc.hInstance = hg_hinstance;
    wndc.lpszClassName = highGUIclassName;
    wndc.lpszMenuName = highGUIclassName;
    wndc.hIcon = LoadIconA(0, IDI_APPLICATION);
    //wndc.hCursor = ( HCURSOR ) LoadCursorA( 0, ( LPSTR ) ( size_t ) IDC_CROSS );
    wndc.hCursor = (HCURSOR) LoadCursorA(0, (char*)(size_t) IDC_ARROW);
    wndc.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH);
    wndc.lpfnWndProc = MainWindowProc;
    RegisterClassA(&wndc);
    atexit(icvCleanupHighgui);
    wasInitialized = 1;
  }
  return 0;
}
static int cvNamedWindow(const char* name, int flags)
{
  int result = 0;
  HWND hwnd, mainhWnd;
  CvWindow* window;
  DWORD defStyle;
  int len;
  IRECT rect;
  CV_FUNCNAME("cvNamedWindow");
  __BEGIN__;
  defStyle = WS_VISIBLE | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;
  cvInitSystem(0, 0);
  if (!name) {
    CV_ERROR(CV_StsNullPtr, "NULL name string");
  }
  // Check the name in the storage
  if (icvFindWindowByName(name) != 0) {
    result = 1;
    EXIT;
  }
  if (!(flags & CV_WINDOW_AUTOSIZE)) { //YV add border in order to resize the window
    defStyle |= WS_SIZEBOX;
  }
  icvLoadWindowPos(name, &rect);
  mainhWnd = CreateWindow("Main HighGUI struct", name, defStyle | WS_OVERLAPPED,
      rect.l, rect.t, RCW(&rect), RCH(&rect), 0, 0, hg_hinstance, 0);
  if (!mainhWnd) {
    CV_ERROR(CV_StsError, "Frame window can not be created");
  }
  ShowWindow(mainhWnd, SW_SHOW);
  //YV- remove one border by changing the style
  hwnd = CreateWindow("HighGUI struct", "", (defStyle & ~WS_SIZEBOX) | WS_CHILD, CW_USEDEFAULT, 0, RCW(&rect), RCW(&rect), mainhWnd, 0, hg_hinstance, 0);
  if (!hwnd) {
    CV_ERROR(CV_StsError, "Frame window can not be created");
  }
  ShowWindow(hwnd, SW_SHOW);
  len = (int) strlen(name);
  CV_CALL(window = (CvWindow*) cvAlloc(sizeof(CvWindow) + len + 1));
  memset(window, 0, sizeof(CvWindow));
  window->signature = CV_WINDOW_MAGIC_VAL;
  window->hwnd = hwnd;
  window->frame = mainhWnd;
  window->name = (char*)(window + 1);
  memcpy(window->name, name, len + 1);
  window->flags = flags;
  window->image = 0;
  window->dc = CreateCompatibleDC(0);
  window->last_key = 0;
  window->status = CV_WINDOW_NORMAL; //YV
  window->on_mouse = 0;
  window->on_mouse_param = 0;
  memset(&window->toolbar, 0, sizeof(window->toolbar));
  window->next = hg_windows;
  window->prev = 0;
  if (hg_windows) {
    hg_windows->prev = window;
  }
  hg_windows = window;
  icvSetWindowLongPtr(hwnd, CV_USERDATA, window);
  icvSetWindowLongPtr(mainhWnd, CV_USERDATA, window);
  // Recalculate window position
  icvUpdateWindowPos(window);
  result = 1;
  __END__;
  return result;
}

