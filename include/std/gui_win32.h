
#ifndef _GUI_WIN32_H_
#define _GUI_WIN32_H_

///////////////////////////////////////////////////////////////////////////////////////////
#include <winuser.h>
#include <commctrl.h>
#include <CommDlg.h>
#ifdef _MSC_VER
#pragma warning(disable : 4305)
#pragma warning(disable : 4996)
#pragma warning(disable : 4819)
#pragma warning(disable : 4018)

// kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#endif // _MSC_VER
///////////////////////////////////////////////////////////////////////////////////////////
#ifndef AC_SRC_ALPHA
#define AC_SRC_ALPHA 0x01
#endif
#ifndef GET_WHEEL_DELTA_WPARAM
#define GET_WHEEL_DELTA_WPARAM(wParam) ((short)HIWORD(wParam))
#endif
#if _MSC_VER >= 1200
#pragma warning( disable: 4710 )
#endif

typedef int(*CvWin32WindowCB)(HWND, UINT, WPARAM, LPARAM, int*);
static const char* trackbar_text = " ";
static const char* icvWindowPosRootKey = "Software\\OpenCV\\HighGUI\\Windows\\";

//#define GetWindowLongPtr GetWindowLongA
#if 0
#define GetWindowLongPtrA GetWindowLongPtrA
#define SetWindowLongPtrA( hwnd, id, ptr ) SetWindowLongA( hwnd, id, (size_t)ptr )
//#define SetWindowLongPtr( hwnd, id, ptr ) SetWindowLongA( hwnd, id, (size_t)ptr )
#define icvGetClassLongPtr GetClassLong
#define GWL_WNDPROC (-4)
#define GWL_HINSTANCE (-6)
#define GWL_HWNDPARENT (-8)
#define GWL_STYLE (-16)
#define GWL_EXSTYLE (-20)
#define GWL_USERDATA (-21)
#define GWL_ID (-12)
#endif
//#define GWLP_USERDATA GWLP_USERDATA
//#define GWLP_WNDPROC GWLP_WNDPROC
#define CC_HCURSOR GCL_HCURSOR
#define CC_HBRBACKGROUND GCL_HBRBACKGROUND
struct CvWindow;
typedef int(*notify2_t)(int, int*);;
typedef struct CvTrackbar {
  int signature;
  HWND hwnd;
  char* name;
  struct CvTrackbar* next;
  struct CvWindow* parent;
  HWND buddy;
  int* data;
  int pos;
  int maxval;
  int(*notify)(int);
  notify2_t notify2;
  int* userdata;
  int id;
}
CvTrackbar;
//typedef int ( *MouseCB )(int event, int x, int y, int flags, int* param);
typedef struct CvWindow {
  int signature;
  HWND hwnd;
  char* name;
  struct CvWindow* prev;
  struct CvWindow* next;
  HWND frame;
  HWND hList;
  HDC dc;
  HGDIOBJ image;
  int last_key;
  int flags;
  int status; //0 normal, 1 fullscreen (YV)
  MouseCB on_mouse;
  int* on_mouse_param;
  struct {
    HWND toolbar;
    int pos;
    int rows;
    WNDPROC toolBarProc;
    CvTrackbar* first;
  }
  toolbar;
}
CvWindow;
#define HG_BUDDY_WIDTH 130
#ifndef TBIF_SIZE
#define TBIF_SIZE 0x40
#endif
#ifndef TB_SETBUTTONINFO
#define TB_SETBUTTONINFO (WM_USER + 66)
#endif
#ifndef TBM_GETTOOLTIPS
#define TBM_GETTOOLTIPS (WM_USER + 30)
#endif
static LRESULT CALLBACK HighGUIProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static int icvUpdateWindowPos(CvWindow* window);
static CvWindow* hg_windows = 0;
static CvWindow* focus_window = 0;
static CvWin32WindowCB hg_on_preprocess = 0, hg_on_postprocess = 0;
static HINSTANCE hg_hinstance = 0;
static const char* highGUIclassName = "HighGUI struct";
static const char* mainHighGUIclassName = "Main HighGUI struct";
// DestroyAllWindows
// 销毁所有HighGUI窗口
// int DestroyAllWindows(int);
// 函数DestroyAllWindows销毁所有已经打开的HighGUI窗口。
static int DestroyAllWindows()
{
  CvWindow* window = hg_windows;
  while (window) {
    HWND mainhWnd = window->frame;
    HWND hwnd = window->hwnd;
    window = window->next;
    SendMessageA(hwnd, WM_CLOSE, 0, 0);
    SendMessageA(mainhWnd, WM_CLOSE, 0, 0);
  }
  return 0;
}
static void __cdecl icvCleanupHighgui(void)
{
  DestroyAllWindows();
  UnregisterClassA(highGUIclassName, hg_hinstance);
  UnregisterClassA(mainHighGUIclassName, hg_hinstance);
}
// cvInitSystem 初始化HighGUI
// int InitSystem( int argc, TCHAR** argv );
// argc 命令行参数个数。
// argv 命令行参数数组。
// 函数cvInitSystem初始化HighGUI。如果在第一个窗口被创建前这个函数不能被用户显式地调用，
// 这个函数将以参数argc=0，argv=NULL隐式地被调用。在Win32下，没有必要显式调用这个函数。
// 在X Window下，参数可以被用来自定义HighGUI窗口和控件的外观。
static int InitSystem(int a, char** b)
{
  static int wasInitialized = 0;
  // check initialization status
  if (!wasInitialized) {
    WNDCLASSA wndc;
    // Initialize the stogare
    hg_windows = 0;
    // Register the struct
    wndc.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
    wndc.lpfnWndProc = WindowProc;
    wndc.cbClsExtra = 0;
    wndc.cbWndExtra = 0;
    wndc.hInstance = hg_hinstance;
    wndc.lpszClassName = highGUIclassName;
    wndc.lpszMenuName = highGUIclassName;
    wndc.hIcon = LoadIcon(0, IDI_APPLICATION);
    //wndc.hCursor = ( HCURSOR ) LoadCursorA( 0, ( LPSTR ) ( size_t ) IDC_CROSS );
    wndc.hCursor = (HCURSOR)LoadCursorA(0, (char*)(size_t)IDC_ARROW);
    wndc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    RegisterClassA(&wndc);
    wndc.lpszClassName = mainHighGUIclassName;
    wndc.lpszMenuName = mainHighGUIclassName;
    wndc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    wndc.lpfnWndProc = MainWindowProc;
    RegisterClassA(&wndc);
    atexit(icvCleanupHighgui);
    wasInitialized = 1;
  }
  return 0;
}
static int StartWindowThread()
{
  return 0;
}
static CvWindow* icvFindWindowByName(const char* name)
{
  CvWindow* window = hg_windows;
  for (; window != 0 && strcmp(name, window->name) != 0; window = window->next)
    ;
  return window;
}
#if 0
BOOL CreateCaret1(HBITMAP hBitmap, int nWidth, int nHeight)
{
  return focus_window ? CreateCaret(focus_window->hwnd, hBitmap, nWidth, nHeight) : 0;
}
POINT GetCaretPos1()
{
  POINT pt = { 0, 0 };
  GetCaretPos(&pt);
  return pt;
}
BOOL CreateSolidCaret1(int nWidth, int nHeight)
{
  return focus_window ? CreateCaret(focus_window->hwnd, (HBITMAP)0, nWidth, nHeight) : 0;
}
BOOL ShowCaret1()
{
  return focus_window ? ShowCaret(focus_window->hwnd) : 0;
}
BOOL HideCaret1()
{
  return focus_window ? HideCaret(focus_window->hwnd) : 0;
}
BOOL SetCaretPos1(POINT pt)
{
  return SetCaretPos(pt.x, pt.y);
}
#endif
static CvWindow* icvWindowByHWND(HWND hwnd)
{
  CvWindow* window = (CvWindow*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);
  return window != 0 && hg_windows != 0 &&
         window->signature == CC_WINDOW_MAGIC_VAL ? window : 0;
}
static CvTrackbar* icvTrackbarByHWND(HWND hwnd)
{
  CvTrackbar* trackbar = (CvTrackbar*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);
  return trackbar != 0 && trackbar->signature == CC_TRACKBAR_MAGIC_VAL &&
         trackbar->hwnd == hwnd ? trackbar : 0;
}
// Window positions saving/loading added by Philip Gruebele.
//<a href="mailto:pgruebele@cox.net">pgruebele@cox.net</a>
// Restores the window position from the registry saved position.
static int icvLoadWindowPos(const char* name, IRECT* prect)
{
  HKEY hkey;
  char szKey[1024];
  IRECT rect = { 100, 100, 500, 500 };
  int def = 640;
  strcpy(szKey, icvWindowPosRootKey);
  strcat(szKey, name);
  if (RegOpenKeyExA(HKEY_CURRENT_USER, szKey, 0, KEY_QUERY_VALUE, &hkey) == ERROR_SUCCESS) {
    // Yes we are installed.
    DWORD dwType = 0;
    DWORD dwSize = sizeof(int);
    RegQueryValueExA(hkey, "Left", NULL, &dwType, (BYTE*)& rect.l, &dwSize);
    RegQueryValueExA(hkey, "Top", NULL, &dwType, (BYTE*)& rect.t, &dwSize);
    RegQueryValueExA(hkey, "Right", NULL, &dwType, (BYTE*)& rect.r, &dwSize);
    RegQueryValueExA(hkey, "Bottom", NULL, &dwType, (BYTE*)& rect.b, &dwSize);
    if (rect.l != (int)CW_USEDEFAULT && (rect.l < -200 || rect.l > 3000)) {
      rect.l = 100;
    }
    if (rect.r != (int)CW_USEDEFAULT && (rect.r < -200 || rect.r > 3000)) {
      rect.r = 100 + def;
    }
    if (rect.t != (int)CW_USEDEFAULT && (rect.t < -200 || rect.t > 3000)) {
      rect.t = 100;
    }
    if (rect.b != (int)CW_USEDEFAULT && (rect.b < -200 || rect.b > 3000)) {
      rect.b = 100 + def;
    }
    RegCloseKey(hkey);
  }
  *prect = rect;
  return 0;
}
// Window positions saving/loading added by Philip Gruebele.
//<a href="mailto:pgruebele@cox.net">pgruebele@cox.net</a>
// philipg. Saves the window position in the registry
static int icvSaveWindowPos(const char* name, IRECT rect)
{
  static const DWORD MAX_RECORD_COUNT = 100;
  HKEY hkey;
  char szKey[1024];
  char rootKey[1024];
  strcpy(szKey, icvWindowPosRootKey);
  strcat(szKey, name);
  if (RegOpenKeyExA(HKEY_CURRENT_USER, szKey, 0, KEY_READ, &hkey) != ERROR_SUCCESS) {
    HKEY hroot;
    DWORD count = 0;
    FILETIME oldestTime = { UINT_MAX, UINT_MAX };
    char oldestKey[1024];
    char currentKey[1024];
    strcpy(rootKey, icvWindowPosRootKey);
    rootKey[strlen(rootKey) - 1] = '\0';
    if (RegCreateKeyExA(HKEY_CURRENT_USER, rootKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ + KEY_WRITE, 0, &hroot, NULL) != ERROR_SUCCESS)
      //RegOpenKeyEx( HKEY_CURRENT_USER,rootKey,0,KEY_READ,&hroot) != ERROR_SUCCESS )
    {
      return 0;
    }
    for (;;) {
      DWORD csize = sizeof(currentKey);
      FILETIME accesstime = { 0, 0 };
      LONG code = RegEnumKeyExA(hroot, count, currentKey, &csize, NULL, NULL, NULL, &accesstime);
      if (code != ERROR_SUCCESS && code != ERROR_MORE_DATA) {
        break;
      }
      count++;
      if (oldestTime.dwHighDateTime > accesstime.dwHighDateTime ||
          (oldestTime.dwHighDateTime == accesstime.dwHighDateTime &&
           oldestTime.dwLowDateTime > accesstime.dwLowDateTime)) {
        oldestTime = accesstime;
        strcpy(oldestKey, currentKey);
      }
    }
    if (count >= MAX_RECORD_COUNT) {
      RegDeleteKeyA(hroot, oldestKey);
    }
    RegCloseKey(hroot);
    if (RegCreateKeyExA(HKEY_CURRENT_USER, szKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &hkey, NULL) != ERROR_SUCCESS) {
      return 0;
    }
  } else {
    RegCloseKey(hkey);
    if (RegOpenKeyExA(HKEY_CURRENT_USER, szKey, 0, KEY_WRITE, &hkey) != ERROR_SUCCESS) {
      return 0;
    }
  }
  RegSetValueExA(hkey, "Left", 0, REG_DWORD, (BYTE*)& rect.l, sizeof(rect.l));
  RegSetValueExA(hkey, "Top", 0, REG_DWORD, (BYTE*)& rect.t, sizeof(rect.t));
  RegSetValueExA(hkey, "Right", 0, REG_DWORD, (BYTE*)& rect.r, sizeof(rect.r));
  RegSetValueExA(hkey, "Bottom", 0, REG_DWORD, (BYTE*)& rect.b, sizeof(rect.b));
  RegCloseKey(hkey);
  return 0;
}
static double cvGetMode_W32(const char* name) //YV
{
  double result = -1;
  CvWindow* window;
  CC_FUNCNAME("cvGetMode_W32");
  __BEGIN__;
  if (!name) {
    CC_ERROR(CC_StsNullPtr, "NULL name string");
  }
  window = icvFindWindowByName(name);
  if (!window) {
    CC_ERROR(CC_StsNullPtr, "NULL window");
  }
  result = window->status;
  __END__;
  return result;
}
#ifdef MONITOR_DEFAULTTONEAREST
static int ChangeMode_W32(const char* name, double prop_value) //Yannick Verdie
{
  CvWindow* window;
  CC_FUNCNAME("cvChangeMode_W32");
  __BEGIN__;
  if (!name) {
    CC_ERROR(CC_StsNullPtr, "NULL name string");
  }
  window = icvFindWindowByName(name);
  if (!window) {
    CC_ERROR(CC_StsNullPtr, "NULL window");
  }
  if (window->flags & CC_WINDOW_AUTOSIZE) { //if the flag CC_WINDOW_AUTOSIZE is set
    EXIT;
  }
  {
    DWORD dwStyle = GetWindowLongPtr(window->frame, GWL_STYLE);
    IRECT position;
    if (window->status == CC_WINDOW_FULLSCREEN && prop_value == CC_WINDOW_NORMAL) {
      icvLoadWindowPos(window->name, &position);
      SetWindowLongPtr(window->frame, GWL_STYLE, dwStyle | WS_CAPTION);
      SetWindowPos(window->frame, HWND_TOP, position.l, position.t, RCW(&position), RCH(&position), SWP_NOZORDER | SWP_FRAMECHANGED);
      window->status = CC_WINDOW_NORMAL;
      EXIT;
    }
    if (window->status == CC_WINDOW_NORMAL && prop_value == CC_WINDOW_FULLSCREEN) {
      //save dimension
      IRECT rect;
      IRECT RectCV;
      HMONITOR hMonitor;
      MONITORINFO mi;
      GetWindowRect(window->frame, (RECT*)&rect);
      RectCV = iRECT(rect.l, rect.t, rect.r, rect.b);
      icvSaveWindowPos(window->name, RectCV);
      //Look at coordinate for fullscreen
      hMonitor = MonitorFromRect((RECT*)&rect, MONITOR_DEFAULTTONEAREST);
      mi.cbSize = sizeof(mi);
      GetMonitorInfo(hMonitor, &mi);
      //fullscreen
      position = *(IRECT*)&mi.rcMonitor;
      SetWindowLongPtr(window->frame, GWL_STYLE, dwStyle & ~WS_CAPTION);
      SetWindowPos(window->frame, HWND_TOP, position.l, position.t, RCW(&position), RCH(&position), SWP_NOZORDER | SWP_FRAMECHANGED);
      window->status = CC_WINDOW_FULLSCREEN;
      EXIT;
    }
  }
  __END__;
  return 0;
}
#else
int ChangeMode_W32(const char* c, double d)
{
  return 0;
}
#endif
// 创建窗口
// int NamedWindow( const TCHAR* name, int flags=CC_WINDOW_AUTOSIZE );
// name 窗口的名字，它被用来区分不同的窗口，并被显示为窗口标题。
// flags 窗口属性标志。目前唯一支持的标志是CC_WINDOW_AUTOSIZE。当这个标志被设置后，
// 用户不能手动改变窗口大小，窗口大小会自动调整以适合被显示图像（参考cvShowImage）。
// 函数NamedWindow创建一个可以放置图像和trackbar的窗口。被创建的窗口可以通过它们的名字被引用。
// 如果已经存在这个名字的窗口，这个函数将不做任何事情。
int NamedWindow(const char* name, int flags)
{
  int result = 0;
  HWND hwnd, mainhWnd;
  CvWindow* window = NULL;
  DWORD defStyle;
  int len;
  IRECT rect;
  CC_FUNCNAME("NamedWindow");
  __BEGIN__;
  defStyle = WS_VISIBLE | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;
  InitSystem(0, 0);
  if (!name) {
    CC_ERROR(CC_StsNullPtr, "NULL name string");
  }
  // Check the name in the storage
  if (icvFindWindowByName(name) != 0) {
    result = 1;
    EXIT;
  }
  if (!(flags & CC_WINDOW_AUTOSIZE)) { //YV add border in order to resize the window
    defStyle |= WS_SIZEBOX;
  }
  icvLoadWindowPos(name, &rect);
  mainhWnd = CreateWindowA("Main HighGUI struct", name, defStyle | WS_OVERLAPPED,
                           rect.l, rect.t, RCW(&rect), RCH(&rect), 0, 0, hg_hinstance, 0);
  if (!mainhWnd) {
    CC_ERROR(CC_StsError, "Frame window can not be created");
  }
  ShowWindow(mainhWnd, SW_SHOW);
  //YV- remove one border by changing the style
  hwnd = CreateWindowA("HighGUI struct", "", (defStyle & ~WS_SIZEBOX) | WS_CHILD, CW_USEDEFAULT, 0, RCW(&rect), RCW(&rect), mainhWnd, 0, hg_hinstance, 0);
  if (!hwnd) {
    CC_ERROR(CC_StsError, "Frame window can not be created");
  }
  ShowWindow(hwnd, SW_SHOW);
  len = (int)strlen(name);
  CC_CALL(window = (CvWindow*)malloc(sizeof(CvWindow) + len + 1));
  memset(window, 0, sizeof(CvWindow));
  window->signature = CC_WINDOW_MAGIC_VAL;
  window->hwnd = hwnd;
  window->frame = mainhWnd;
  window->name = (char*)(window + 1);
  memcpy(window->name, name, len + 1);
  window->flags = flags;
  window->image = 0;
  window->dc = CreateCompatibleDC(0);
  window->last_key = 0;
  window->status = CC_WINDOW_NORMAL; //YV
  window->on_mouse = 0;
  window->on_mouse = mouse_proc_default;
  window->on_mouse_param = 0;
  memset(&window->toolbar, 0, sizeof(window->toolbar));
  window->next = hg_windows;
  window->prev = 0;
  if (hg_windows) {
    hg_windows->prev = window;
  }
  hg_windows = window;
  SetWindowLongPtrA(hwnd, GWLP_USERDATA, (LONG_PTR)window);
  SetWindowLongPtrA(mainhWnd, GWLP_USERDATA, (LONG_PTR)window);
  // Recalculate window position
  icvUpdateWindowPos(window);
  result = 1;
  __END__;
  return result;
}
static int icvRemoveWindow(CvWindow* window)
{
  CvTrackbar* trackbar = NULL;
  IRECT wrect = { 0, 0, 0, 0 };
  if (window->frame) {
    GetWindowRect(window->frame, (RECT*)&wrect);
  }
  if (window->name) {
    icvSaveWindowPos(window->name, wrect);
  }
  if (window->hwnd) {
    SetWindowLongPtrA(window->hwnd, GWLP_USERDATA, 0);
  }
  if (window->frame) {
    SetWindowLongPtrA(window->frame, GWLP_USERDATA, 0);
  }
  if (window->toolbar.toolbar) {
    SetWindowLongPtrA(window->toolbar.toolbar, GWLP_USERDATA, 0);
  }
  if (window->prev) {
    window->prev->next = window->next;
  } else {
    hg_windows = window->next;
  }
  if (window->next) {
    window->next->prev = window->prev;
  }
  window->prev = window->next = 0;
  if (window->dc && window->image) {
    DeleteObject(SelectObject(window->dc, window->image));
  }
  if (window->dc) {
    DeleteDC(window->dc);
  }
  for (trackbar = window->toolbar.first; trackbar != 0;) {
    CvTrackbar* next = trackbar->next;
    if (trackbar->hwnd) {
      SetWindowLongPtrA(trackbar->hwnd, GWLP_USERDATA, 0);
      free(trackbar);
      trackbar = NULL;
    }
    trackbar = next;
  }
  FREE(window);
  return 0;
}
// cvDestroyWindow1
// 销毁一个窗口
// int DestroyWindow1( const TCHAR* name );
// name 要被销毁的窗口的名字。
// 函数cvDestroyWindow1销毁指定名字的窗口。
static int DestroyWindow1(const char* name)
{
  CvWindow* window;
  HWND mainhWnd;
  CC_FUNCNAME("cvDestroyWindow1");
  __BEGIN__;
  if (!name) {
    CC_ERROR(CC_StsNullPtr, "NULL name string");
  }
  window = icvFindWindowByName(name);
  if (!window) {
    EXIT;
  }
  mainhWnd = window->frame;
  SendMessageA(window->hwnd, WM_CLOSE, 0, 0);
  SendMessageA(mainhWnd, WM_CLOSE, 0, 0);
  // Do NOT call _remove_window -- CvWindow list will be updated automatically ...
  __END__;
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
/* Calculatess the window coordinates relative to the upper l corner of the mainhWnd window */
static IRECT icvCalcWindowRect(CvWindow* window)
{
  const int gutter = 1;
  IRECT crect, trect, rect;
  assert(window);
  GetClientRect(window->frame, (RECT*)&crect);
  if (window->toolbar.toolbar) {
    GetWindowRect(window->toolbar.toolbar, (RECT*)&trect);
    icvScreenToClient(window->frame, &trect);
    SubtractRect((RECT*)&rect, (RECT*)&crect, (RECT*)&trect);
  } else {
    rect = crect;
  }
  rect.t += gutter;
  rect.l += gutter;
  rect.b -= gutter;
  rect.r -= gutter;
  return rect;
}


#define  BMPMARK(A)  (((A)+3)&~3)

// returns TRUE if there is a problem such as ERROR_IO_PENDING.
static BOOL GetBitmapData(HDC dc, int* pwidth, int* pheight, int* channels, int** data)
{
  BITMAP bmp;
  HGDIOBJ h;
  GdiFlush();
  h = GetCurrentObject(dc, OBJ_BITMAP);
  if (pwidth) {
    *pwidth = 0;
  }
  if (pheight) {
    *pheight = 0;
  }
  if (data) {
    *data = 0;
  }
  if (h == NULL) {
    return TRUE;
  }
  if (GetObject(h, sizeof(bmp), &bmp) == 0) {
    return TRUE;
  }
  if (pwidth) {
    *pwidth = abs(bmp.bmWidth);
  }
  if (pheight) {
    *pheight = abs(bmp.bmHeight);
  }
  if (channels) {
    *channels = bmp.bmBitsPixel / 8;
  }
  if (data) {
    *data = (int*)bmp.bmBits;
  }
  return FALSE;
}
static void FillSolidRect(HDC m_hDC, void* lpRect, COLORREF clr)
{
  SetBkColor(m_hDC, clr);
  ExtTextOutA(m_hDC, 0, 0, ETO_OPAQUE, (RECT*)lpRect, NULL, 0, NULL);
}
static int FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin, const RGBQUAD* inpal)
{
  BITMAPINFOHEADER* bmih;
  assert(bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));
  bmih = &(bmi->bmiHeader);
  memset(bmih, 0, sizeof(*bmih));
  bmih->biSize = sizeof(BITMAPINFOHEADER);
  bmih->biWidth = width;
  bmih->biHeight = origin ? abs(height) : -abs(height);
  bmih->biPlanes = 1;
  bmih->biBitCount = (unsigned short)bpp;
  bmih->biCompression = BI_RGB;
  if (bpp <= 8) {
    RGBQUAD* palette = bmi->bmiColors;
    if (inpal) {
      memcpy(palette, inpal, 256 * sizeof(RGBQUAD));
    } else {
      int i;
      for (i = 0; i < 256; i++) {
        palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
        palette[i].rgbReserved = 0;
      }
    }
  }
  return 0;
}



static int icvUpdateWindowPos(CvWindow* window)
{
  IRECT rect;
  assert(window);
  if ((window->flags & CC_WINDOW_AUTOSIZE) && window->image) {
    int i;
    ISIZE size = { 0, 0 };
    GetBitmapData(window->dc, &size.w, &size.h, 0, 0);
    // Repeat two times because after the first resizing of the mainhWnd window
    // toolbar may resize too
    for (i = 0; i < (window->toolbar.toolbar ? 2 : 1); i++) {
      IRECT rmw, rw = icvCalcWindowRect(window);
      MoveWindow(window->hwnd, rw.l, rw.t,
                 rw.r - rw.l + 1, rw.b - rw.t + 1, FALSE);
      GetClientRect(window->hwnd, (RECT*)&rw);
      GetWindowRect(window->frame, (RECT*)&rmw);
      // Resize the mainhWnd window in order to make the bitmap fit into the child window
      MoveWindow(window->frame, rmw.l, rmw.t,
                 rmw.r - rmw.l + size.w - rw.r + rw.l,
                 rmw.b - rmw.t + size.h - rw.b + rw.t, TRUE);
    }
  }
  rect = icvCalcWindowRect(window);
  MoveWindow(window->hwnd, rect.l, rect.t, rect.r - rect.l + 1, rect.b - rect.t + 1, TRUE);
  return 0;
}
static int NameWindowImage(const char* name, int height, int width)
{
  CvWindow* window;
  ISIZE size = { 0, 0 };
  int channels = 0;
  int* dst_ptr = 0;
  const int channels0 = 3;
  int origin = 0;
  BOOL changed_size = FALSE; // philipg
  CC_FUNCNAME("NameWindowImage");
  __BEGIN__;
  if (!name) {
    CC_ERROR(CC_StsNullPtr, "NULL name");
  }
  window = icvFindWindowByName(name);
  if (!window) {
    NamedWindow(name, 1);
    window = icvFindWindowByName(name);
  }
  if (!window) {
    EXIT; // keep silence here.
  }
  if (window->image)
    // if there is something wrong with these system calls, we cannot display image...
    if (GetBitmapData(window->dc, &size.w, &size.h, &channels, &dst_ptr)) {
      return 0;
    }
  if (height > 0 && width > 0) {
    if (size.w != width || size.h != height || channels != channels0) {
      uchar buffer[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
      BITMAPINFO* binfo = (BITMAPINFO*)buffer;
      changed_size = TRUE;
      DeleteObject(SelectObject(window->dc, window->image));
      window->image = 0;
      size.w = width;
      size.h = height;
      channels = channels0;
      FillBitmapInfo(binfo, size.w, size.h, channels * 8, 1, 0);
      window->image = SelectObject(window->dc, CreateDIBSection(window->dc, binfo, DIB_RGB_COLORS, (void**)&dst_ptr, 0, 0));
    }
  } else {
    width = size.w;
    height = size.h;
  }
  if (changed_size) {
    icvUpdateWindowPos(window);
  }
  InvalidateRect(window->hwnd, 0, 0);
  __END__;
  return 0;
}
static int GetWindowImage(const char* name, int* pheight, int* pwidth, unsigned char** arr, int* pstep, int* pcn)
{
  CvWindow* window;
  ISIZE size = { 0, 0 };
  int channels = 0, step = 0;
  unsigned char* dst_ptr = 0;
  const int channels0 = 3;
  int origin = 0;
  BOOL changed_size = FALSE; // philipg
  CC_FUNCNAME("NameWindowImage");
  __BEGIN__;
  if (!name) {
    CC_ERROR(CC_StsNullPtr, "NULL name");
  }
  window = icvFindWindowByName(name);
  if (!window) {
    NamedWindow(name, 1);
    NameWindowImage(name, 300, 300);
    window = icvFindWindowByName(name);
  }
  if (!window || !arr) {
    EXIT; // keep silence here.
  }
  if (GetBitmapData(window->dc, &size.w, &size.h, &channels, (int**)&dst_ptr)) {
    return 0;
  }
  step = (size.w * 3 + 3) & -4;
  if (arr) {
    *arr = dst_ptr + (size.h - 1) * step;
  }
  if (pheight) {
    *pheight = size.h;
  }
  if (pwidth) {
    *pwidth = size.w;
  }
  if (pstep) {
    *pstep = -step;
  }
  if (pcn) {
    *pcn = 3;
  }
  if (changed_size) {
    icvUpdateWindowPos(window);
  }
  InvalidateRect(window->hwnd, 0, 0);
  __END__;
  return NULL != dst_ptr;
}
static int InvalidateWindow(const char* name)
{
  CvWindow* window;
  CC_FUNCNAME("InvalidateWindow");
  __BEGIN__;
  if (!name) {
    CC_ERROR(CC_StsNullPtr, "NULL name");
  }
  window = icvFindWindowByName(name);
  if (!window) {
    EXIT; // keep silence here.
  }
  InvalidateRect(window->hwnd, 0, 0);
  __END__;
  return 0;
}
static int ShowDC(const char* name, HDC hDC, IRECT rc)
{
  CvWindow* window;
  ISIZE size = { 0, 0 };
  int channels = 0;
  int* dst_ptr = 0;
  const int channels0 = 3;
  int origin = 0;
  int width = rc.r - rc.l;
  int height = rc.b - rc.t;
  //CvMat stub, dst;
  BOOL changed_size = FALSE; // philipg
  CC_FUNCNAME("cvShowImage");
  __BEGIN__;
  if (!name) {
    CC_ERROR(CC_StsNullPtr, "NULL name");
  }
  window = icvFindWindowByName(name);
  if (!window) {
    NamedWindow(name, 1);
    window = icvFindWindowByName(name);
  }
  if (!window || !hDC) {
    EXIT; // keep silence here.
  }
  //CC_CALL( image = cvGetMat( arr, &stub ));
  if (window->image) {
  }
  // if there is something wrong with these system calls, we cannot display image...
  if (GetBitmapData(window->dc, &size.w, &size.h, &channels, &dst_ptr)) {
    return 0;
  }
  if (size.w != width || size.h != height || channels != channels0) {
    uchar buffer[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
    BITMAPINFO* binfo = (BITMAPINFO*)buffer;
    changed_size = TRUE;
    DeleteObject(SelectObject(window->dc, window->image));
    window->image = 0;
    size.w = width;
    size.h = height;
    channels = channels0;
    FillBitmapInfo(binfo, size.w, size.h, channels * 8, 1, 0);
    window->image = SelectObject(window->dc, CreateDIBSection(window->dc, binfo, DIB_RGB_COLORS, (void**)&dst_ptr, 0, 0));
  }
  BitBlt(window->dc, 0, 0, width, height, hDC, rc.l, rc.t, SRCCOPY);
  // ony resize window if needed
  if (changed_size) {
    icvUpdateWindowPos(window);
  }
  InvalidateRect(window->hwnd, 0, 0);
  // philipg: this is not needed and just slows things down
  // UpdateWindow(window->hwnd);
  __END__;
  return 0;
}

static void iResizeWindow(CvWindow* window, int width, int height)
{
  // Repeat two times because after the first resizing of the mainhWnd window
  // toolbar may resize too
  int i;
  IRECT rmw, rw, rect;
  for (i = 0; i < (window->toolbar.toolbar ? 2 : 1); i++) {
    rw = icvCalcWindowRect(window);
    MoveWindow(window->hwnd, rw.l, rw.t,
               rw.r - rw.l + 1, rw.b - rw.t + 1, FALSE);
    GetClientRect(window->hwnd, (RECT*)&rw);
    GetWindowRect(window->frame, (RECT*)&rmw);
    // Resize the mainhWnd window in order to make the bitmap fit into the child window
    MoveWindow(window->frame, rmw.l, rmw.t,
               rmw.r - rmw.l + width - rw.r + rw.l,
               rmw.b - rmw.t + height - rw.b + rw.t, TRUE);
  }
  rect = icvCalcWindowRect(window);
  MoveWindow(window->hwnd, rect.l, rect.t,
             rect.r - rect.l + 1, rect.b - rect.t + 1, TRUE);
}

// 设定窗口大小
//
// int ResizeWindow( const TCHAR* name, int width, int height );
// name 将被设置窗口的名字。
// width 新的窗口宽度。
// height 新的窗口高度。
// 函数cvResizeWindow改变窗口的大小。
static int ResizeWindow(const char* name, int width, int height)
{
  CvWindow* window;
  CC_FUNCNAME("cvResizeWindow");
  __BEGIN__;
  if (!name) {
    CC_ERROR(CC_StsNullPtr, "NULL name");
  }
  window = icvFindWindowByName(name);
  if (window) {
    iResizeWindow(window, width, height);
  }
  __END__;
  return 0;
}
// cvMoveWindow
// 设定窗口的位置
//
// int MoveWindow( const TCHAR* name, int x, int y );
// name 将被设置的窗口的名字。
// x 窗口左上角的x坐标。
// y 窗口左上角的y坐标。
// 函数cvMoveWindow改变窗口的位置。
static int cMoveWindow(const char* name, int x, int y)
{
  CvWindow* window;
  IRECT rect;
  CC_FUNCNAME("cvMoveWindow");
  __BEGIN__;
  if (!name) {
    CC_ERROR(CC_StsNullPtr, "NULL name");
  }
  window = icvFindWindowByName(name);
  if (!window) {
    EXIT;
  }
  GetWindowRect(window->frame, (RECT*)&rect);
  MoveWindow(window->frame, x, y, rect.r - rect.l, rect.b - rect.t, TRUE);
  __END__;
  return 0;
}
/*
cvShowImage
指定窗口中显示图像
int ShowImage( const TCHAR* name, const CvArr* image );
name 窗口的名字。
image 被显示的图像。
数cvShowImage 在指定窗口中显示图像。如果窗口创建的时候被设定标志CC_WINDOW_AUTOSIZE，
么图像将以原始尺寸显示；否则，图像将被伸缩以适合窗口大小。
*/
int ShowImagePal(const char* name, int height, int width, const unsigned char* arr, int step, int cn, PixFmt fmt, const void* inpal0)
{
  const RGBQUAD* inpal = (const RGBQUAD*)inpal0;
  CvWindow* window;
  ISIZE size = { 0, 0 };
  int channels = 0;
  int* dst_ptr = 0;
  int origin = 0;
  //CvMat stub, dst;
  BOOL changed_size = FALSE; // philipg
  if (!name) {
    CC_ERROR(CC_StsNullPtr, "NULL name");
  }
  window = icvFindWindowByName(name);
  if (!window) {
    // run_once 使这段代码只运行一次
    NamedWindow(name, 0);
    window = icvFindWindowByName(name);
  }
  if (!window || !arr) {
    return 0; // keep silence here.
  }
  iResizeWindow(window, width, height);
  if (window->image) {
    // if there is something wrong with these system calls, we cannot display image...
    if (GetBitmapData(window->dc, &size.w, &size.h, &channels, &dst_ptr)) {
      return 0;
    }
  }
  if (size.w != width || size.h != height || channels != cn) {
    uchar buffer[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
    BITMAPINFO* binfo = (BITMAPINFO*)buffer;
    //int cnbit[] = {1, 1, 1, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    if (inpal) {
      LPLOGPALETTE lpPal = (LPLOGPALETTE)buffer; // 指向逻辑调色板的指针
      HPALETTE hPal = NULL; // 调色板的句柄
      int i;
      lpPal->palVersion = 0x300; // 设置版本号
      lpPal->palNumEntries = (WORD)256; // 设置颜色数目
      for (i = 0; i < (int)256; i++) {
        lpPal->palPalEntry[i].peRed = inpal[i].rgbRed;
        lpPal->palPalEntry[i].peGreen = inpal[i].rgbGreen;
        lpPal->palPalEntry[i].peBlue = inpal[i].rgbBlue;
        lpPal->palPalEntry[i].peFlags = 0;
      }
      hPal = CreatePalette(lpPal);
      DeleteObject(SelectPalette(window->dc, hPal, TRUE));
    }
    changed_size = TRUE;
    DeleteObject(SelectObject(window->dc, window->image));
    window->image = 0;
    size.w = width;
    size.h = height;
    FillBitmapInfo(binfo, width, height, 32, 1, inpal);
    window->image = SelectObject(window->dc, CreateDIBSection(window->dc, binfo, DIB_RGB_COLORS, (void**)&dst_ptr, 0, 0));
  }
  if ((fmt & 0xff) >= PixFmtMax) {
    fmt = fmt < PixFmtMax ? fmt : cn2PixFmt(cn);
    fmt = cn == 4 ? PF_32bppRGB : fmt;
  }
  img_setbitmap_cn4(height, width, arr, step, cn, fmt, 1, 1, (uchar*)dst_ptr, width * 4, inpal);
  //memset(dst_ptr, 255, height*width*4);
  // ony resize window if needed
  if (changed_size) {
    icvUpdateWindowPos(window);
  }
  InvalidateRect(window->hwnd, 0, 0);
  // philipg: this is not needed and just slows things down
  // UpdateWindow(window->hwnd);
  return 0;
}
static int cSetWindowText(const char* name, const char* text)
{
  CvWindow* window;
  ISIZE size = { 0, 0 };
  int channels = 0;
  int* dst_ptr = 0;
  const int channels0 = 3;
  int origin = 0;
  //CvMat stub, dst;
  BOOL changed_size = FALSE; // philipg
  CC_FUNCNAME("SetWindowText");
  __BEGIN__;
  if (!name) {
    CC_ERROR(CC_StsNullPtr, "NULL name");
  }
  window = icvFindWindowByName(name);
  if (!window) {
    NamedWindow(name, 1);
    window = icvFindWindowByName(name);
  }
  if (!window) {
    EXIT; // keep silence here.
  }
  SetWindowTextA(window->frame, text);
  __END__;
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
      ISIZE size = { 0, 0 };
      int flags = (wParam & MK_LBUTTON ? MSG_FLAG_LBUTTON : 0) |
                  (wParam & MK_RBUTTON ? MSG_FLAG_RBUTTON : 0) |
                  (wParam & MK_MBUTTON ? MSG_FLAG_MBUTTON : 0) |
                  (wParam & MK_CONTROL ? MSG_FLAG_CTRLKEY : 0) |
                  (wParam & MK_SHIFT ? MSG_FLAG_SHIFTKEY : 0) |
                  (GetKeyState(VK_MENU) < 0 ? MSG_FLAG_ALTKEY : 0);
      int event = GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? MSG_MOUSEWHEELUP : MSG_MOUSEWHEELDOWN;
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
#if 0
    if (window->image != 0 && 0) {
      int nchannels = 3;
      ISIZE size = { 0, 0 };
      PAINTSTRUCT paint;
      HDC hDC;
      // Determine the bitmap's dimensions
      GetBitmapData(window->dc, &size.w, &size.h, &nchannels, 0);
      hDC = BeginPaint(hwnd, &paint);
      SetStretchBltMode(hDC, COLORONCOLOR);
      if (nchannels == 1) {
        int i;
        RGBQUAD table[256];
        for (i = 0; i < 256; i++) {
          table[i].rgbBlue = (unsigned char)i;
          table[i].rgbGreen = (unsigned char)i;
          table[i].rgbRed = (unsigned char)i;
        }
        //SetDIBColorTable( window->dc, 0, 255, table );
      }
      if (1) {
        BLENDFUNCTION m_Blend;
        int m_BakWidth = size.w, m_BakHeight = size.h;
        IRECT rct;
        POINT ptSrc = { 0, 0 };
        POINT ptWinPos;
        ISIZE sizeWindow;
        DWORD dwExStyle;
        typedef BOOL(WINAPI * MYFUNC)(HWND, HDC, POINT*, ISIZE*, HDC, POINT*, COLORREF, BLENDFUNCTION*, DWORD);
        MYFUNC UpdateLayeredWindow = 0;
        HINSTANCE hFuncInst;
        // TODO: Add your specialized creation code here
        hFuncInst = LoadLibraryA("User32.DLL");
        if (hFuncInst) {
          UpdateLayeredWindow = (MYFUNC)GetProcAddress(hFuncInst, "UpdateLayeredWindow");
        } else {
          printf("User32.dll ERROR!");
          exit(0);
        }
        GetWindowRect(hwnd, (RECT*)&rct);
        ptWinPos.x = rct.l, ptWinPos.y = rct.t;
        sizeWindow.w = m_BakWidth, sizeWindow.h = m_BakHeight;
        dwExStyle = GetWindowLongA(hwnd, GWL_EXSTYLE);
        if ((dwExStyle & 0x80000) != 0x80000) {
          SetWindowLongA(hwnd, GWL_EXSTYLE, dwExStyle ^ 0x80000);
        }
        if (0) {
          LONG style = GetWindowLongA(hwnd, GWL_STYLE);
          if (style & WS_CAPTION) {
            style ^= WS_CAPTION;
          }
          if (style & WS_THICKFRAME) {
            style ^= WS_THICKFRAME;
          }
          if (style & WS_SYSMENU) {
            style ^= WS_SYSMENU;
          }
          SetWindowLongA(hwnd, GWL_STYLE, style);
          style = GetWindowLongA(hwnd, GWL_EXSTYLE);
          if (style & WS_EX_APPWINDOW) {
            style ^= WS_EX_APPWINDOW;
          }
          SetWindowLongA(hwnd, GWL_EXSTYLE, style);
        }
        m_Blend.BlendOp = AC_SRC_OVER; //theonlyBlendOpdefinedinWindows2000
        m_Blend.BlendFlags = 0; //nothingelseisspecial
        m_Blend.AlphaFormat = AC_SRC_ALPHA; //
        m_Blend.SourceConstantAlpha = 255; //AC_SRC_ALPHA 透明度
        printf("UpdateLayeredWindow\n");
        UpdateLayeredWindow(hwnd, hDC, &ptWinPos, &sizeWindow, window->dc, &ptSrc, 0, &m_Blend, 2);
      }
      EndPaint(hwnd, &paint);
    } else
#endif
    {
      return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    break;
  case WM_DESTROY:
    icvRemoveWindow(window);
    // Do nothing!!!
    //PostQuitMessage(0);
    break;
  case WM_GETMINMAXINFO:
    if (!(window->flags & CC_WINDOW_AUTOSIZE)) {
      MINMAXINFO* minmax = (MINMAXINFO*)lParam;
      IRECT rect;
      LRESULT retval = DefWindowProc(hwnd, uMsg, wParam, lParam);
      minmax->ptMinTrackSize.y = 100;
      minmax->ptMinTrackSize.x = 100;
      if (window->toolbar.first) {
        GetWindowRect(window->toolbar.first->hwnd, (RECT*)&rect);
        minmax->ptMinTrackSize.y += window->toolbar.rows * (rect.b - rect.t);
        minmax->ptMinTrackSize.x = MAX(rect.r - rect.l + HG_BUDDY_WIDTH, HG_BUDDY_WIDTH * 2);
      }
      return retval;
    }
    break;
  case WM_WINDOWPOSCHANGED: {
    WINDOWPOS* pos = (WINDOWPOS*)lParam;
    // Update the toolbar position/size
    if (window->toolbar.toolbar) {
      IRECT rect;
      GetWindowRect(window->toolbar.toolbar, (RECT*)&rect);
      MoveWindow(window->toolbar.toolbar, 0, 0, pos->cx, rect.b - rect.t, TRUE);
    }
    if (!(window->flags & CC_WINDOW_AUTOSIZE)) {
      icvUpdateWindowPos(window);
    }
    break;
  }
  case WM_ACTIVATE:
    if (LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE) {
      SetFocus(window->hwnd);
    }
    break;
  case WM_ERASEBKGND: {
    IRECT cr, tr, wrc;
    HRGN rgn, rgn1, rgn2;
    int ret;
    HDC hDC = (HDC)wParam;
    GetWindowRect(window->hwnd, (RECT*)&cr);
    icvScreenToClient(window->frame, &cr);
    if (window->toolbar.toolbar) {
      GetWindowRect(window->toolbar.toolbar, (RECT*)&tr);
      icvScreenToClient(window->frame, &tr);
    } else {
      tr.l = tr.t = tr.r = tr.b = 0;
    }
    GetClientRect(window->frame, (RECT*)&wrc);
    rgn = CreateRectRgn(0, 0, wrc.r, wrc.b);
    rgn1 = CreateRectRgn(cr.l, cr.t, cr.r, cr.b);
    rgn2 = CreateRectRgn(tr.l, tr.t, tr.r, tr.b);
    ret = CombineRgn(rgn, rgn, rgn1, RGN_DIFF);
    ret = CombineRgn(rgn, rgn, rgn2, RGN_DIFF);
    if (ret != NULLREGION && ret != ERROR) {
      //FillRgn(hDC, rgn, (HBRUSH) icvGetClassLongPtr(hwnd, CC_HBRBACKGROUND));
    }
    DeleteObject(rgn);
    DeleteObject(rgn1);
    DeleteObject(rgn2);
  }
  return 1;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
static int FillSolidRect1(HDC m_hDC, const IRECT* rect, COLORREF clr)
{
  ASSERT(m_hDC != NULL);
  SetBkColor(m_hDC, clr);
  ExtTextOut(m_hDC, 0, 0, ETO_OPAQUE, (RECT*)rect, NULL, 0, NULL);
  return 0;
}
static LRESULT CALLBACK HighGUIProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  CvWindow* window = icvWindowByHWND(hwnd);
  if (!window)
    // This window is not mentioned in HighGUI storage
    // Actually, this should be error except for the case of calls to CreateWindow
  {
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
  // Process the message
  {
    //LRESULT WINAPI imuiWndProc_impl(window_host_t* host, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //imuiWndProc_impl(NULL, hwnd, uMsg, wParam, lParam);
  }
  switch (uMsg) {
  case WM_WINDOWPOSCHANGING: {
    LPWINDOWPOS pos = (LPWINDOWPOS)lParam;
    IRECT rect = icvCalcWindowRect(window);
    pos->x = rect.l;
    pos->y = rect.t;
    pos->cx = rect.r - rect.l + 1;
    pos->cy = rect.b - rect.t + 1;
  }
  break;
  case WM_SETFOCUS: {
    focus_window = window;
  }
  break;
  case WM_LBUTTONDOWN:
  case WM_RBUTTONDOWN:
  case WM_MBUTTONDOWN:
  case WM_LBUTTONDBLCLK:
  case WM_RBUTTONDBLCLK:
  case WM_MBUTTONDBLCLK:
  case WM_LBUTTONUP:
  case WM_RBUTTONUP:
  case WM_MBUTTONUP:
  case WM_MOUSEMOVE:
    if (1) {
      POINT pt;
      IRECT rect;
      ISIZE size = { 0, 0 };
      int flags = (wParam & MK_LBUTTON ? MSG_FLAG_LBUTTON : 0) |
                  (wParam & MK_RBUTTON ? MSG_FLAG_RBUTTON : 0) |
                  (wParam & MK_MBUTTON ? MSG_FLAG_MBUTTON : 0) |
                  (wParam & MK_CONTROL ? MSG_FLAG_CTRLKEY : 0) |
                  (wParam & MK_SHIFT ? MSG_FLAG_SHIFTKEY : 0) |
                  (GetKeyState(VK_MENU) < 0 ? MSG_FLAG_ALTKEY : 0);
      int event = uMsg == WM_LBUTTONDOWN ? MSG_LBUTTONDOWN :
                  uMsg == WM_RBUTTONDOWN ? MSG_RBUTTONDOWN :
                  uMsg == WM_MBUTTONDOWN ? MSG_MBUTTONDOWN :
                  uMsg == WM_LBUTTONUP ? MSG_LBUTTONUP :
                  uMsg == WM_RBUTTONUP ? MSG_RBUTTONUP :
                  uMsg == WM_MBUTTONUP ? MSG_MBUTTONUP :
                  uMsg == WM_LBUTTONDBLCLK ? MSG_LBUTTONDBLCLK :
                  uMsg == WM_RBUTTONDBLCLK ? MSG_RBUTTONDBLCLK :
                  uMsg == WM_MBUTTONDBLCLK ? MSG_MBUTTONDBLCLK :
                  MSG_MOUSEMOVE;
      if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN) {
        SetCapture(hwnd);
      }
      if (uMsg == WM_LBUTTONUP || uMsg == WM_RBUTTONUP || uMsg == WM_MBUTTONUP) {
        ReleaseCapture();
      }
      pt.x = LOWORD(lParam);
      pt.y = HIWORD(lParam);
      GetClientRect(window->hwnd, (RECT*)&rect);
      GetBitmapData(window->dc, &size.w, &size.h, 0, 0);
      {
#if 0
        int x = 0, y = 0, dw, dh, tx, ty, x1, y1;
        dw = rect.r - rect.l, dh = rect.b - rect.t;
        tx = size.w * dh;
        ty = size.h * dw;
        if (tx > ty) {
          tx = dw, ty /= size.w, y = (dh - ty) / 2;
        } else {
          ty = dh, tx /= size.h, x = (dw - tx) / 2;
        }
        x1 = pt.x * size.w / tx;
        y1 = pt.y * size.h / ty;
        if (x1 >= 0 && x1 < size.w && y1 >= 0 && y1 < size.h) {
          window->on_mouse(event, x1, y1, flags, window->on_mouse_param);
        }
#else
        int x1, y1;
        x1 = pt.x * size.w / MAX(rect.r - rect.l, 1);
        y1 = pt.y * size.h / MAX(rect.b - rect.t, 1);
        if (uMsg == WM_MOUSEMOVE) {
          char buf[256];
          _snprintf(buf, 256, "%s[%d %d]", window->name, x1, y1);
          SetWindowTextA(window->frame, buf);
        }
        if (window->on_mouse) {
          window->on_mouse(event, x1, y1, flags, window->on_mouse_param);
        }
#endif
      }
    }
    break;
  case WM_SIZE:
    if (window && window->hList) {
      RECT rc;
      GetClientRect(window->hwnd, &rc);
      MoveWindow(window->hList, 0, 0, rc.right - rc.left, rc.bottom - rc.top, TRUE);
    }
    break;
  case WM_PAINT:
    if (window->image != 0) {
      int nchannels = 3;
      ISIZE size = { 0, 0 };
      PAINTSTRUCT paint;
      HDC hDC;
      // Determine the bitmap's dimensions
      GetBitmapData(window->dc, &size.w, &size.h, &nchannels, 0);
      hDC = BeginPaint(hwnd, &paint);
      SetStretchBltMode(hDC, COLORONCOLOR);
      if (nchannels == 1) {
        int i;
        RGBQUAD table[256];
        for (i = 0; i < 256; i++) {
          table[i].rgbBlue = (unsigned char)i;
          table[i].rgbGreen = (unsigned char)i;
          table[i].rgbRed = (unsigned char)i;
        }
        //SetDIBColorTable( window->dc, 0, 255, table );
      }
      if (window->flags & CC_WINDOW_AUTOSIZE) {
        BitBlt(hDC, 0, 0, size.w, size.h, window->dc, 0, 0, SRCCOPY);
      } else {
        IRECT rect;
        IRECT rc;
        int dw, dh;
        DWORD crBk;
        HDC hDCMem;
        HBITMAP hBmpMem, hPreBmp;
        GetClientRect(window->hwnd, (RECT*)&rect);
        /* 创建双缓冲区 */
        // 创建与当前DC兼容的内存DC
        hDCMem = CreateCompatibleDC(hDC);
        // 创建一块指定大小的位图
        hBmpMem = CreateCompatibleBitmap(hDC, rect.r, rect.b);
        // 将该位图选入到内存DC中，默认是全黑色的
        hPreBmp = (HBITMAP)SelectObject(hDCMem, hBmpMem);
        crBk = GetSysColor(COLOR_BTNFACE);
        dw = RCW(&rect), dh = RCH(&rect);
        iRectScaling(rect, size.w, size.h, &rc);
        FillSolidRect(hDCMem, (RECT*)&rect, crBk);
        if (RCW(&rc) < size.w) {
          //SetStretchBltMode(hDCMem, HALFTONE);
          SetStretchBltMode(hDCMem, COLORONCOLOR);
        }
        //FillRect( dcMemory, &rect, (HBRUSH)( GetStockObject( BLACK_BRUSH ) ) );
        StretchBlt(hDCMem, rc.l, rc.t, RCW(&rc), RCH(&rc), window->dc, 0, 0, size.w, size.h, SRCCOPY);
        /* 将双缓冲区图像复制到显示缓冲区 */
        BitBlt(hDC, 0, 0, rect.r, rect.b, hDCMem, 0, 0, SRCCOPY);
        /* 释放资源 */
        SelectObject(hDCMem, hPreBmp);
        DeleteObject(hBmpMem);
        DeleteDC(hDCMem);
      }
      //DeleteDC(hDC);
      EndPaint(hwnd, &paint);
    } else {
      return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
  case WM_ERASEBKGND:
    if (window->image) {
      return 0;
    }
    break;
  case WM_DESTROY:
    icvRemoveWindow(window);
    // Do nothing!!!
    //PostQuitMessage(0);
    break;
  case WM_SETCURSOR:
    //SetCursor((HCURSOR) icvGetClassLongPtr(hwnd, CC_HCURSOR));
    return 0;
  case WM_KEYDOWN:
    window->last_key = (int)wParam;
    return 0;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT ret;
  if (hg_on_preprocess) {
    int was_processed = 0;
    int ret = hg_on_preprocess(hwnd, uMsg, wParam, lParam, &was_processed);
    if (was_processed) {
      return ret;
    }
  }
  ret = HighGUIProc(hwnd, uMsg, wParam, lParam);
  if (hg_on_postprocess) {
    int was_processed = 0;
    int ret = hg_on_postprocess(hwnd, uMsg, wParam, lParam, &was_processed);
    if (was_processed) {
      return ret;
    }
  }
  return ret;
}
static int icvUpdateTrackbar(CvTrackbar* trackbar, int pos)
{
  const int max_name_len = 10;
  const char* suffix = "";
  char pos_text[32];
  int name_len;
  if (trackbar->data) {
    *trackbar->data = pos;
  }
  if (trackbar->pos != pos) {
    trackbar->pos = pos;
    if (trackbar->notify2) {
      trackbar->notify2(pos, trackbar->userdata);
    }
    if (trackbar->notify) {
      trackbar->notify(pos);
    }
    name_len = (int)strlen(trackbar->name);
    if (name_len > max_name_len) {
      int start_len = max_name_len * 2 / 3;
      int end_len = max_name_len - start_len - 2;
      memcpy(pos_text, trackbar->name, start_len);
      memcpy(pos_text + start_len, "...", 3);
      memcpy(pos_text + start_len + 3, trackbar->name + name_len - end_len, end_len + 1);
    } else {
      memcpy(pos_text, trackbar->name, name_len + 1);
    }
    sprintf(pos_text + strlen(pos_text), "%s: %d\n", suffix, pos);
    SetWindowTextA(trackbar->buddy, pos_text);
  }
  return 0;
}
static LRESULT CALLBACK HGToolbarProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  CvWindow* window = icvWindowByHWND(hwnd);
  if (!window) {
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
  // Control messages processing
  switch (uMsg) {
    // Slider processing
  case WM_HSCROLL: {
    HWND slider = (HWND)lParam;
    int pos = (int)SendMessageA(slider, TBM_GETPOS, 0, 0);
    CvTrackbar* trackbar = icvTrackbarByHWND(slider);
    if (trackbar) {
      if (trackbar->pos != pos) {
        icvUpdateTrackbar(trackbar, pos);
      }
    }
    SetFocus(window->hwnd);
    return 0;
  }
  case WM_NCCALCSIZE: {
    LRESULT ret = CallWindowProc(window->toolbar.toolBarProc, hwnd, uMsg, wParam, lParam);
    int rows = (int)SendMessageA(hwnd, TB_GETROWS, 0, 0);
    if (window->toolbar.rows != rows) {
      CvTrackbar* trackbar;
      SendMessageA(window->toolbar.toolbar, TB_BUTTONCOUNT, 0, 0);
      trackbar = window->toolbar.first;
      for (; trackbar != 0; trackbar = trackbar->next) {
        IRECT rect;
        SendMessageA(window->toolbar.toolbar, TB_GETITEMRECT,
                     (WPARAM)trackbar->id, (LPARAM)& rect);
        MoveWindow(trackbar->hwnd, rect.l + HG_BUDDY_WIDTH, rect.t,
                   rect.r - rect.l - HG_BUDDY_WIDTH,
                   rect.b - rect.t, FALSE);
        MoveWindow(trackbar->buddy, rect.l, rect.t,
                   HG_BUDDY_WIDTH, rect.b - rect.t, FALSE);
      }
      window->toolbar.rows = rows;
    }
    return ret;
  }
  }
  return CallWindowProc(window->toolbar.toolBarProc, hwnd, uMsg, wParam, lParam);
}
// cvWaitKey 等待按键事件
// int WaitKey( int delay=0 );
// delay 延迟的毫秒数。
// 数cvWaitKey无限制的等待按键事件（delay<=0时）；或者延迟"delay"毫秒。
// 回值为被按键的值，如果超过指定时间则返回-1。
// 释：这个函数是HighGUI中唯一能够获取和操作事件的函数，所以在一般的事件处理中，
// 需要周期地被调用，除非HighGUI被用在某些能够处理事件的环境中。
// 者注：比如在MFC环境下，这个函数不起作用。
int WaitMsg(int delay, bool bWaitMouseMsg)
{
  int time0 = GetTickCount();
  for (;;) {
    MSG message;
    CvWindow* win = NULL;
    int is_processed = 0;
    {
      //sysio_t* io = sys_getio();
      //if (!(io->mouse->down[0] || io->mouse->down[1] || io->mouse->down[2])) {        io->hitid = NULL;      }
    }
    if ((delay > 0 && abs((int)(GetTickCount() - time0)) >= delay) || hg_windows == 0) {
      return -1;
    }
    if (delay <= 0) {
      GetMessage(&message, 0, 0, 0);
    } else if (PeekMessage(&message, 0, 0, 0, PM_REMOVE) == FALSE) {
      Sleep(1);
      continue;
    }
    if (hg_windows) {
      CvWindow* window = NULL;
      for (window = hg_windows; window != 0 && is_processed == 0; window = window->next) {
        if (window->hwnd == message.hwnd || window->frame == message.hwnd) {
          is_processed = 1;
          win = window;
        }
      }
    }
    if (win) {
      switch (message.message) {
      case WM_DESTROY:
      case WM_CHAR:
        DispatchMessage(&message);
        return (int)message.wParam;
      case WM_SYSKEYDOWN:
        if (message.wParam == VK_F10) {
          is_processed = 1;
          return (int)(message.wParam << 16);
        }
        break;
      case WM_MBUTTONDOWN:
      case WM_MBUTTONUP:
      case WM_RBUTTONDOWN:
      case WM_RBUTTONUP:
      case WM_LBUTTONDOWN:
      case WM_LBUTTONUP:
        if (bWaitMouseMsg) {
          TranslateMessage(&message);
          DispatchMessage(&message);
          return (int)(message.wParam << 16);
        } else {
          DispatchMessage(&message);
          is_processed = 1;
        }
        break;
      case WM_KEYDOWN:
        TranslateMessage(&message);
        if ((message.wParam >= VK_F1 && message.wParam <= VK_F24) ||
            message.wParam == VK_HOME || message.wParam == VK_END ||
            message.wParam == VK_UP || message.wParam == VK_DOWN ||
            message.wParam == VK_LEFT || message.wParam == VK_RIGHT ||
            message.wParam == VK_INSERT || message.wParam == VK_DELETE ||
            message.wParam == VK_PRIOR || message.wParam == VK_NEXT) {
          DispatchMessage(&message);
          is_processed = 1;
          return (int)(message.wParam << 16);
        }
      default:
        DispatchMessage(&message);
        is_processed = 1;
        break;
      }
    }
    if (!is_processed) {
      TranslateMessage(&message);
      DispatchMessage(&message);
    }
  }
}
static CvTrackbar*
icvFindTrackbarByName(const CvWindow* window, const char* name)
{
  CvTrackbar* trackbar = window->toolbar.first;
  for (; trackbar != 0 && strcmp(trackbar->name, name) != 0; trackbar = trackbar->next)
    ;
  return trackbar;
}
typedef struct {
  UINT cbSize;
  DWORD dwMask;
  int idCommand;
  int iImage;
  BYTE fsState;
  BYTE fsStyle;
  WORD cx;
  DWORD lParam;
  LPSTR pszText;
  int cchText;
}
ButtonInfo;
static int icvCreateTrackbar(const char* trackbar_name, const char* window_name,
                             int* val, int count, TrackbarCB on_notify,
                             TrackbarCB2 on_notify2, int* userdata)
{
  char slider_name[32];
  CvWindow* window = 0;
  CvTrackbar* trackbar = 0;
  int pos = 0;
  int result = 0;
  CC_FUNCNAME("icvCreateTrackbar");
  __BEGIN__;
  if (!window_name || !trackbar_name) {
    CC_ERROR(CC_StsNullPtr, "NULL window or trackbar name");
  }
  if (count <= 0) {
    CC_ERROR(CC_StsOutOfRange, "Bad trackbar maximal value");
  }
  window = icvFindWindowByName(window_name);
  if (!window) {
    EXIT;
  }
  trackbar = icvFindTrackbarByName(window, trackbar_name);
  if (!trackbar) {
    TBBUTTON tbs;
    ButtonInfo tbis;
    IRECT rect;
    int bcount;
    int len = (int)strlen(trackbar_name);
    // create toolbar if it is not created yet
    if (!window->toolbar.toolbar) {
      const int default_height = 30;
      window->toolbar.toolbar = CreateToolbarEx(
                                  window->frame, WS_CHILD | CCS_TOP | TBSTYLE_WRAPABLE,
                                  1, 0, 0, 0, 0, 0, 16, 20, 16, 16, sizeof(TBBUTTON));
      GetClientRect(window->frame, (RECT*)&rect);
      MoveWindow(window->toolbar.toolbar, 0, 0,
                 rect.r - rect.l, default_height, TRUE);
      SendMessageA(window->toolbar.toolbar, TB_AUTOSIZE, 0, 0);
      ShowWindow(window->toolbar.toolbar, SW_SHOW);
      window->toolbar.first = 0;
      window->toolbar.pos = 0;
      window->toolbar.rows = 0;
      window->toolbar.toolBarProc =
        (WNDPROC)GetWindowLongPtrA(window->toolbar.toolbar, GWLP_WNDPROC);
      icvUpdateWindowPos(window);
      // Subclassing from toolbar
      SetWindowLongPtrA(window->toolbar.toolbar, GWLP_WNDPROC, (LONG_PTR)HGToolbarProc);
      SetWindowLongPtrA(window->toolbar.toolbar, GWLP_USERDATA, (LONG_PTR)window);
    }
    /* Retrieve current buttons count */
    bcount = (int)SendMessageA(window->toolbar.toolbar, TB_BUTTONCOUNT, 0, 0);
    if (bcount > 1) {
      /* If this is not the first button then we need to
      separate it from the previous one */
      tbs.iBitmap = 0;
      tbs.idCommand = bcount; // Set button id to it's number
      tbs.iString = 0;
      tbs.fsStyle = TBSTYLE_SEP;
      tbs.fsState = TBSTATE_ENABLED;
      SendMessageA(window->toolbar.toolbar, TB_ADDBUTTONS, 1, (LPARAM)& tbs);
      // Retrieve current buttons count
      bcount = (int)SendMessageA(window->toolbar.toolbar, TB_BUTTONCOUNT, 0, 0);
    }
    /* Add a button which we're going to cover with the slider */
    tbs.iBitmap = 0;
    tbs.idCommand = bcount; // Set button id to it's number
    tbs.fsState = TBSTATE_ENABLED;
#if 0/*!defined WIN64 && !defined EM64T*/
    tbs.fsStyle = 0;
    tbs.iString = 0;
#else
#ifndef TBSTYLE_AUTOSIZE
#define TBSTYLE_AUTOSIZE 0x0010
#endif
#ifndef TBSTYLE_GROUP
#define TBSTYLE_GROUP 0x0004
#endif
    //tbs.fsStyle = TBSTYLE_AUTOSIZE;
    tbs.fsStyle = TBSTYLE_GROUP;
    tbs.iString = (INT_PTR)trackbar_text;
#endif
    SendMessageA(window->toolbar.toolbar, TB_ADDBUTTONS, 1, (LPARAM)& tbs);
    /* Adjust button size to the slider */
    tbis.cbSize = sizeof(tbis);
    tbis.dwMask = TBIF_SIZE;
    GetClientRect(window->hwnd, (RECT*)&rect);
    tbis.cx = (unsigned short)(rect.r - rect.l);
    SendMessageA(window->toolbar.toolbar, TB_SETBUTTONINFO,
                 (WPARAM)tbs.idCommand, (LPARAM)& tbis);
    /* Get button position */
    SendMessageA(window->toolbar.toolbar, TB_GETITEMRECT,
                 (WPARAM)tbs.idCommand, (LPARAM)& rect);
    /* Create a slider */
    trackbar = (CvTrackbar*)malloc(sizeof(CvTrackbar) + len + 1);
    trackbar->signature = CC_TRACKBAR_MAGIC_VAL;
    trackbar->notify = 0;
    trackbar->notify2 = 0;
    trackbar->parent = window;
    trackbar->pos = 0;
    trackbar->data = 0;
    trackbar->id = bcount;
    trackbar->next = window->toolbar.first;
    trackbar->name = (char*)(trackbar + 1);
    memcpy(trackbar->name, trackbar_name, len + 1);
    window->toolbar.first = trackbar;
    sprintf(slider_name, "Trackbar%p", val);
    trackbar->hwnd = CreateWindowExA(0, TRACKBAR_CLASSA, slider_name,
                                     WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS |
                                     TBS_FIXEDLENGTH | TBS_HORZ | TBS_BOTTOM,
                                     rect.l + HG_BUDDY_WIDTH, rect.t,
                                     rect.r - rect.l - HG_BUDDY_WIDTH,
                                     rect.b - rect.t, window->toolbar.toolbar,
                                     (HMENU)(size_t)bcount, hg_hinstance, 0);
    sprintf(slider_name, "Buddy%p", val);
    trackbar->buddy = CreateWindowExA(0, "STATIC", slider_name,
                                      WS_CHILD | SS_RIGHT,
                                      rect.l, rect.t,
                                      HG_BUDDY_WIDTH, rect.b - rect.t,
                                      window->toolbar.toolbar, 0, hg_hinstance, 0);
    SetWindowLongPtrA(trackbar->hwnd, GWLP_USERDATA, (LONG_PTR)trackbar);
    /* Minimize the number of rows */
    SendMessageA(window->toolbar.toolbar, TB_SETROWS,
                 MAKEWPARAM(1, FALSE), (LPARAM)& rect);
  } else {
    trackbar->data = 0;
    trackbar->notify = 0;
    trackbar->notify2 = 0;
  }
  trackbar->maxval = count;
  /* Adjust slider parameters */
  SendMessageA(trackbar->hwnd, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, count));
  SendMessageA(trackbar->hwnd, TBM_SETTICFREQ, (WPARAM)1, (LPARAM)0);
  if (val) {
    pos = *val;
  }
  SendMessageA(trackbar->hwnd, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)pos);
  SendMessageA(window->toolbar.toolbar, TB_AUTOSIZE, 0, 0);
  trackbar->pos = -1;
  icvUpdateTrackbar(trackbar, pos);
  ShowWindow(trackbar->buddy, SW_SHOW);
  ShowWindow(trackbar->hwnd, SW_SHOW);
  trackbar->notify = on_notify;
  trackbar->notify2 = (notify2_t)on_notify2;
  trackbar->userdata = userdata;
  trackbar->data = val;
  /* Resize the window to reflect the toolbar resizing*/
  icvUpdateWindowPos(window);
  result = 1;
  __END__;
  return result;
}
// 创建trackbar并将它添加到指定的窗口。
// CC_EXTERN_C_FUNCPTR( int (*TrackbarCB)(int pos) );
// trackbar_name 被创建的trackbar名字。
// window_name窗口名字，这个窗口将为被创建trackbar的父对象。
// value 整数指针，它的值将反映滑块的位置。这个变量指定创建时的滑块位置。
// count 滑块位置的最大值。最小值一直是0。
// on_change
// 每次滑块位置被改变的时候，被调用函数的指针。这个函数应该被声明为int Foo(int);
// 如果没有回调函数，这个值可以设为NULL。
// 用指定的名字和范围来创建trackbar（滑块或者范围控制），
// 指定与trackbar位置同步的变量，并且指定当trackbar位置被改变的时候调用的回调函数。
// 被创建的trackbar显示在指定窗口的顶端。
int CreateTrackbar(const char* trackbar_name, const char* window_name, int* val, int count, TrackbarCB on_notify)
{
  return icvCreateTrackbar(trackbar_name, window_name, val, count, on_notify, 0, 0);
}
static int CreateTrackbar2(const char* trackbar_name, const char* window_name, int* val, int count, TrackbarCB2 on_notify2, int* userdata)
{
  return icvCreateTrackbar(trackbar_name, window_name, val, count, 0, on_notify2, userdata);
}
// assign callback for mouse events
// 设置鼠标事件的回调函数
// int SetMouseCB( const char* window_name, MouseCB on_mouse, int* param=NULL );
// window_name 窗口的名字。
// on_mouse 指定窗口里每次鼠标事件发生的时候，被调用的函数指针。这个函数的原型应该为
// int Foo(int event, int x, int y, int flags, int* param);
// 其中event是 MSG_*变量之一， x和y是鼠标指针在图像坐标系的坐标（不是窗口坐标系），
// flags是MSG_FLAG的组合， param是用户定义的传递到SetMouseCB函数调用的参数。
// param 用户定义的传递到回调函数的参数。
// 函数SetMouseCB设定指定窗口鼠标事件发生时的回调函数。
static int SetMouseCB(const char* window_name, MouseCB on_mouse, void* param)
{
  CvWindow* window = 0;
  CC_FUNCNAME("SetMouseCB");
  __BEGIN__;
  if (!window_name) {
    CC_ERROR(CC_StsNullPtr, "NULL window name");
  }
  window = icvFindWindowByName(window_name);
  if (!window) {
    EXIT;
  }
  window->on_mouse = on_mouse;
  window->on_mouse_param = (int*)param;
  __END__;
  return 0;
}
// cvGetTrackbarPos
// 获取trackbar的位置
// int GetTrackbarPos( const char* trackbar_name, const char* window_name );
// trackbar_name
// trackbar的名字。
// window_name
// trackbar父窗口的名字。
// 函数cvGetTrackbarPos返回指定trackbar的当前位置。
static int GetTrackbarPos(const char* trackbar_name, const char* window_name)
{
  CvWindow* window;
  CvTrackbar* trackbar = 0;
  int pos = -1;
  CC_FUNCNAME("cvGetTrackbarPos");
  __BEGIN__;
  if (trackbar_name == 0 || window_name == 0) {
    CC_ERROR(CC_StsNullPtr, "NULL trackbar or window name");
  }
  window = icvFindWindowByName(window_name);
  if (window) {
    trackbar = icvFindTrackbarByName(window, trackbar_name);
  }
  if (trackbar) {
    pos = trackbar->pos;
  }
  __END__;
  return pos;
}
// SetTrackbarPos
// 设置trackbar位置
// int SetTrackbarPos( const char* trackbar_name, const char* window_name, int pos );
// trackbar_name
// trackbar的名字。
// window_name
// trackbar父窗口的名字。
// pos
// 新的位置。
// 函数SetTrackbarPos设置指定trackbar的位置。
static int SetTrackbarPos(const char* trackbar_name, const char* window_name, int pos)
{
  CvWindow* window;
  CvTrackbar* trackbar = 0;
  CC_FUNCNAME("SetTrackbarPos");
  __BEGIN__;
  if (trackbar_name == 0 || window_name == 0) {
    CC_ERROR(CC_StsNullPtr, "NULL trackbar or window name");
  }
  window = icvFindWindowByName(window_name);
  if (window) {
    trackbar = icvFindTrackbarByName(window, trackbar_name);
  }
  if (trackbar) {
    if (pos < 0) {
      pos = 0;
    }
    if (pos > trackbar->maxval) {
      pos = trackbar->maxval;
    }
    SendMessageA(trackbar->hwnd, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)pos);
    icvUpdateTrackbar(trackbar, pos);
  }
  __END__;
  return 0;
}
// GetWindowHandle
// 通过名字获取窗口句柄
// int* GetWindowHandle( const TCHAR* name );
// name 窗口名字。
// 函数GetWindowHandle返回原始的窗口句柄（在Win32情况下返回HWND，GTK+情况下返回GtkWidget）
static int* GetWindowHandle(const char* window_name)
{
  CvWindow* window;
  int* hwnd = 0;
  CC_FUNCNAME("GetWindowHandle");
  __BEGIN__;
  if (window_name == 0) {
    CC_ERROR(CC_StsNullPtr, "NULL window name");
  }
  window = icvFindWindowByName(window_name);
  if (window) {
    hwnd = (int*)window->hwnd;
  }
  __END__;
  return hwnd;
}
// GetWindowName
// 过句柄获取窗口的名字
// const TCHAR* GetWindowName( int* window_handle );
// window_handle 口句柄。
// 定窗口的句柄（在Win32情况下是HWND，GTK+情况下是GtkWidget），返回窗口的名字。
static const char* GetWindowName(int* window_handle)
{
  CvWindow* window;
  const char* window_name = "";
  CC_FUNCNAME("GetWindowName");
  __BEGIN__;
  if (window_handle == 0) {
    CC_ERROR(CC_StsNullPtr, "NULL window");
  }
  window = icvWindowByHWND((HWND)window_handle);
  if (window) {
    window_name = window->name;
  }
  __END__;
  return window_name;
}
static int SetPreprocessFuncWin32(int(*on_preprocess)(HWND, UINT, WPARAM, LPARAM, int*))
{
  if (on_preprocess) {
    hg_on_preprocess = on_preprocess;
  } else {
    assert(on_preprocess);
  }
  return 0;
}
static int SetPostprocessFuncWin32(int(*on_postprocess)(HWND, UINT, WPARAM, LPARAM, int*))
{
  if (on_postprocess) {
    hg_on_postprocess = on_postprocess;
  } else {
    assert(on_postprocess);
  }
  return 0;
}
#if 0
static CvScalar hsv2rgb2(float hue)
{
  int rgb[3], p, sector;
  static const int sector_data[][3] =
  { {0, 2, 1}, {1, 2, 0}, {1, 0, 2}, {2, 0, 1}, {2, 1, 0}, {0, 1, 2} };
  hue *= 0.033333333333333333333333333333333f;
  sector = cvFloor(hue);
  p = cvRound(255 * (hue - sector));
  p ^= sector & 1 ? 255 : 0;
  rgb[sector_data[sector][0]] = 255;
  rgb[sector_data[sector][1]] = 0;
  rgb[sector_data[sector][2]] = p;
  return cvScalar(rgb[2], rgb[1], rgb[0], 0);
}
int imshowhist(const char* name, int h, int w, const unsigned char* a, int al, int ai, int l, int r)
{
  int hist[256] = { 0 };
  int back[256] = { 0 };
  enum { cc = 2, hh = 100 * cc, ww = 256 * cc, hdims = 256 };
  unsigned char bb[hh * ww * 3] = { 0 };
  int i, j, maxval, bin_w;
  if (l > r) {
    CC_SWAP(l, r, i);
  }
  l /= cc, r /= cc;
  l = BOUND(l, 0, 255);
  r = BOUND(r, 0, 255);
  SKIP_HIST(h, w, a, al, ai, hist);
  MINMAX_ELEM(256, hist, 1, i, j);
  maxval = hist[j];
  bin_w = ww / hdims;
  DrawRectangle(hh, ww, bb, ww * 3, 3, iPOINT(l * bin_w, hh),
                iPOINT(r * bin_w, 0), CC_RGB(255, 255, 255), -1, 8, 0);
  for (i = 0; i < hdims; i++) {
    int val = cvRound(hist[i] * hh * 0.9 / maxval);
    CvScalar color = hsv2rgb2(i * 180.f / hdims);
    DrawRectangle(hh, ww, bb, ww * 3, 3, iPOINT(i * bin_w, hh),
                  iPOINT((i + 1)*bin_w, hh - val), color, -1, 8, 0);
  }
  {
    double sc = 800. / MAX(h, w);
    int h2 = cvRound(h * sc), w2 = cvRound(w * sc);
    unsigned char* b = MALLOC(unsigned char, h * w * 3);
    unsigned char* b2 = MALLOC(unsigned char, h2 * w2 * 3);
    int s = 0;
    char str[16];
    IPOINT* hor = MALLOC(IPOINT, h);
    IPOINT* ver = MALLOC(IPOINT, w);
    MEMSET(hor, 0, h);
    MEMSET(ver, 0, w);
    for (i = l; i <= r; ++i) {
      back[i] = 255;
      s += hist[i];
    }
    for (i = 0; i < h; ++i) {
      for (j = 0; j < w; ++j) {
        int bw = back[a[i * al + j * ai]];
        int k = i * w + j;
        b[k * 3] = b[k * 3 + 1] = b[k * 3 + 2] = bw;
        hor[i].x += bw > 128, hor[i].y = i;
        ver[j].y += bw > 128, ver[j].x = j;
      }
    }
    for (i = 0; i < h; ++i) {
      hor[i].x = cvRound(hor[i].x * sc);
      hor[i].y = cvRound(hor[i].y * sc);
    }
    for (i = 0; i < w; ++i) {
      ver[i].x = cvRound(ver[i].x * sc);
      ver[i].y = cvRound(ver[i].y * sc);
    }
    _snprintf(str, 16, "%1.2f %3d %3d", s * 1. / (h * w), l, r);
    xDrawText(hh, ww, bb, ww * 3, 3, 0, 0, "黑体", 20, 0, 0, 0, str, CC_RGB(255, 0, 0));
    IMRESIZE(h, w, b, w * 3, 3, h2, w2, b2, w2 * 3, 3, INTER_NEARSAMPLE);
    PolyLineM(h2, w2, b2, w2 * 3, 3, &hor, &h, 1, 0, CC_RGB(255, 0, 0), 1, 8, 0);
    PolyLineM(h2, w2, b2, w2 * 3, 3, &ver, &w, 1, 0, CC_RGB(255, 0, 0), 1, 8, 0);
    NamedWindow("bin", 1);
    cvShowImage("bin", h2, w2, b2, w2 * 3, 3);
    SAFEFREE(b2);
    SAFEFREE(b);
    SAFEFREE(hor);
    SAFEFREE(ver);
  }
  //NamedWindow( name, 0 );
  cvShowImage(name, hh, ww, bb, ww * 3, 3);
  //cvWaitKey(-1);
  return 0;
}
typedef struct _hist_param {
  int h, w, al, ai;
  const unsigned char* a;
  int l, r;
  int select_object;
} _hist_param;
int _hist_on_mouse(int event, int x, int y, int flags, int* param)
{
  _hist_param* im = (_hist_param*)param;
  if (event == MSG_LBUTTONDOWN) {
    im->l = x;
    im->r = x;
    im->select_object = 1;
  }
  if (event == MSG_LBUTTONUP) {
    im->select_object = 0;
  }
  if (im->select_object) {
    im->r = x;
  }
  //imshowhist("hist", im->h, im->w, im->a, im->al, im->ai, im->l, im->r);
}
double cvGetThreshVal_Otsu(int height, int width, const unsigned char* src, int src_step, int srccn);
int imcalchist(int h, int w, const unsigned char* a, int al, int ai)
{
  char ch;
  _hist_param im;
  NamedWindow("hist", 1);
  im.h = h, im.w = w, im.a = a, im.al = al, im.ai = ai, im.l = 0, im.r = 0, im.select_object = 0;
  SetMouseCB("hist", _hist_on_mouse, &im);
  im.r = (int)0;//(2*cvGetThreshVal_Otsu(h, w, a, al, ai));
  for (; 1;) {
    ch = cvWaitKey(10);
    if ('q' == ch) {
      break;
    }
    imshowhist("hist", h, w, a, al, ai, im.l, im.r);
  }
  return 0;
}
static int ShowTaskBar(BOOL bShow)
{
  IRECT rectWorkArea = { 0, 0, 0, 0 };
  IRECT rectTaskBar = { 0, 0, 0, 0 };
  HWND pWnd = FindWindowA("Shell_TrayWnd", "");
  if (!bShow) { //隐藏
    SystemParametersInfo(SPI_GETWORKAREA, 0, (LPVOID)&rectWorkArea, 0);
    if (pWnd) {
      GetWindowRect(pWnd, (RECT*)&rectTaskBar);
      rectWorkArea.b += RCH(&rectTaskBar);
      SystemParametersInfo(SPI_SETWORKAREA, 0, (LPVOID)&rectWorkArea, 0);
      ShowWindow(pWnd, SW_HIDE);
    }
  } else { //显示
    SystemParametersInfo(SPI_GETWORKAREA, 0, (LPVOID)&rectWorkArea, 0);
    if (pWnd) {
      GetWindowRect(pWnd, (RECT*)&rectTaskBar);
      rectWorkArea.b -= RCH(&rectTaskBar);
      SystemParametersInfo(SPI_SETWORKAREA, 0, (LPVOID)&rectWorkArea, 0);
      ShowWindow(pWnd, SW_SHOW);
    }
  }
  return 0;
}
#endif
static int SetFullScreen(const char* name)
{
  CvWindow* window;
  HWND hwnd = 0;
  static IRECT rc_back = { 0, 0, 300, 300 };
  static int is_FullScreen = 0;
  IRECT rect;
  if (!name) {
    CC_ERROR(CC_StsNullPtr, "NULL name");
  }
  window = icvFindWindowByName(name);
  hwnd = window->frame;
  //ShowTaskBar(is_FullScreen);
  if (is_FullScreen) {
    rect = rc_back;
    SetWindowLongA(hwnd, GWL_STYLE, GetWindowLongA(hwnd, GWL_STYLE) | ((WS_CAPTION | WS_BORDER)));
    ShowWindow(hwnd, SW_HIDE);
    SetWindowPos(hwnd, 0, rect.l, rect.t, RCW(&rect), RCH(&rect), SWP_SHOWWINDOW);
  } else {
    int frameWidth = GetSystemMetrics(SM_CXFRAME);
    int frameHeight = GetSystemMetrics(SM_CYFRAME);
    int captionHeight = GetSystemMetrics(SM_CYCAPTION);
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    SetWindowLongA(hwnd, GWL_STYLE, GetWindowLongA(hwnd, GWL_STYLE) & (~(WS_CAPTION | WS_BORDER)));
    GetWindowRect(hwnd, (RECT*)&rc_back);
    GetClientRect(hwnd, (RECT*)&rect);
    rect.l = rect.l - frameWidth;
    rect.r = rect.l + screenWidth + 2 * frameWidth;
    rect.t = rect.t - frameHeight;
    rect.b = rect.t + screenHeight + 2 * frameHeight;
    ShowWindow(hwnd, SW_HIDE);
    SetForegroundWindow(hwnd);
    SetWindowPos(hwnd, HWND_TOPMOST, rect.l, rect.t, RCW(&rect), RCH(&rect), SWP_SHOWWINDOW | SWP_NOZORDER);
    //SetWindowPos(hwnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
  }
  is_FullScreen = !is_FullScreen;
  return 0;
}
static CvWindow* cvGetWindowByName(const char* name)
{
  CvWindow* window = 0;
  if (!name) {
    CC_ERROR(CC_StsNullPtr, "NULL name");
  }
  window = icvFindWindowByName(name);
  if (!window) {
    NamedWindow(name, 0);
    window = icvFindWindowByName(name);
  }
  ASSERT(NULL != window);
  return window;
}
static int SetListLable(const char* name, int n, const char** lable, int nColumnWidth)
{
  CvWindow* window = cvGetWindowByName(name);
  if (window->hList) {
    DestroyWindow(window->hList);
    window->hList = NULL;
  }
  if (NULL == window->hList) {
    RECT rc;
    DWORD dwStyle = WS_TABSTOP | LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS | WS_CHILD | WS_VISIBLE;
    DWORD dwExStyle =
#if 0
      LVS_EX_FULLROWSELECT | //设置可选整行
      LVS_EX_GRIDLINES | //listView的item的每个栏目之间添加分割线
      LVS_EX_HEADERDRAGDROP |//允许litview的目录通过拖放重排序
#endif
      LVS_EDITLABELS;
    //dwStyle |= WS_BORDER;
    GetClientRect(window->hwnd, &rc);
    window->hList = CreateWindowA(WC_LISTVIEWA, ("ListView"), dwStyle,
                                  0, 0, rc.right - rc.left, rc.bottom - rc.top, window->hwnd, 0, hg_hinstance, 0);
    //ListView_SetExtendedListViewStyle(window->hList, dwExStyle);
    //ListView_SetTextColor(hList,RGB(12,12,12));//字体颜色
  }
  if (nColumnWidth <= 0) {
    nColumnWidth = -1;
  }
  if (n > 0) {
    int k = 0, nCol;
    char buf2[256] = { 0 };
    LVCOLUMNA lvColumn = { 0 };
    HWND hList = window->hList;
    HWND hHeader = 0;//ListView_GetHeader(window->hList);
    HD_ITEM hdr[1] = { 0 };
    nCol = Header_GetItemCount(hHeader);
    lvColumn.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM;
    lvColumn.cx = nColumnWidth;
    lvColumn.fmt = LVCFMT_CENTER;
    for (k = 0; k < nCol; ++k) {
      //Header_DeleteItem(hHeader, nCol-1-k);
    }
#if 0
    for (k = nCol; k < n; ++k) {
      //Header_InsertItem(hHeader, k, (LPARAM)hdr);
    }
    if (lable) {
      for (k = 0; k < n; ++k) {
        lvColumn.pszText = (char*)lable[k];
        hdr->pszText = (char*)lable[k];
        Header_InsertItem(hHeader, k, (LPARAM)hdr);
      }
    } else {
      _snprintf(buf2, 256, "");
      lvColumn.pszText = buf2;
      hdr->pszText = buf2;
      for (k = 0; k <= n; ++k) {
        _snprintf(buf2, 256, "%d", k);
        lvColumn.pszText = buf2;
        hdr->pszText = buf2;
        Header_InsertItem(hHeader, k, (LPARAM)hdr);
        //ListView_InsertColumn(hList, k, (LPARAM)&lvColumn);
      }
    }
#else
    if (lable) {
      for (k = 0; k < n; ++k) {
        lvColumn.pszText = (char*)lable[k];
        ListView_InsertColumn(hList, k, (LPARAM)&lvColumn);
      }
    } else {
      _snprintf(buf2, 256, "");
      lvColumn.pszText = buf2;
      ListView_InsertColumn(hList, 0, (LPARAM)&lvColumn);
      for (k = 1; k <= n; ++k) {
        _snprintf(buf2, 256, "%d", k);
        lvColumn.pszText = buf2;
        ListView_InsertColumn(hList, k, (LPARAM)&lvColumn);
      }
    }
#endif
  }
  return 0;
}
static int ListInsertItem(const char* name)
{
  return 0;
}
#define ListView_SetItemTextA(hwndLV, i, iSubItem_, pszText_) \
{ LV_ITEMA _ms_lvi;\
  _ms_lvi.iSubItem = iSubItem_;\
  _ms_lvi.pszText = pszText_;\
  SNDMSG((hwndLV), LVM_SETITEMTEXT, (WPARAM)(i), (LPARAM)(LV_ITEM *)&_ms_lvi);\
}

static int SetListItemText(const char* name, int iRow, int iCol, const char* text)
{
  CvWindow* window = cvGetWindowByName(name);
  //HWND hHeader = ListView_GetHeader(window->hList);
  //int nCol = Header_GetItemCount(hHeader);
  int nRow = ListView_GetItemCount(window->hList);
  if (iRow >= nRow) {
    LV_ITEMA item = { 0 }; // 项
    item.mask = LVIF_TEXT; // 文字
    item.cchTextMax = MAX_PATH; // 文字长度
    item.iItem = 0;
    item.iSubItem = 0;
    for (nRow; nRow <= iRow; ++nRow) {
      item.iItem = nRow;
      item.pszText = NULL;
      ListView_InsertItem(window->hList, (LPARAM)&item);
    }
  }
  ListView_SetItemTextA(window->hList, iRow, iCol, (char*)text);
  ListView_SetColumnWidth(window->hList, iCol, -1);
  return 0;
}
#if 0
static int SetList_strv(const char* name, int iCol, const vstr_t* sv)
{
  CvWindow* window = cvGetWindowByName(name);
  int nRow = ListView_GetItemCount(window->hList);
  int iRow;
  for (iRow = 0; iRow < sv->n; ++iRow) {
    if (iRow == nRow) {
      LV_ITEMA item = { 0 }; // 项
      item.mask = LVIF_TEXT; // 文字
      item.cchTextMax = MAX_PATH; // 文字长度
      item.iItem = 0;
      item.iSubItem = 0;
      item.iItem = nRow;
      item.pszText = NULL;
      ListView_InsertItem(window->hList, (LPARAM)&item);
      ++nRow;
    }
    ListView_SetItemTextA(window->hList, iRow, iCol, (char*)(sv->v[iRow].s));
  }
  ListView_SetColumnWidth(window->hList, iCol, -1);
  return 0;
}
static int ShowMat(const char* name, const char* fmt, int h, int w, const void* arr, int al, int ai, int nColumnWidth /*= -1*/)
{
  CvWindow* window = 0;
  if (!name) {
    CC_ERROR(CC_StsNullPtr, "NULL name");
  }
  window = icvFindWindowByName(name);
  if (!window) {
    NamedWindow(name, 0);
    window = icvFindWindowByName(name);
  }
  {
    int j, n = 0, k = 0;
    char buf2[256] = { 0 };
    LV_ITEMA item = { 0 }; // 项
    for (j = 0; j < w; ++j) {
      ListView_DeleteColumn(window->hList, 0);
    }
    SetListLable(name, w, 0, nColumnWidth);
    item.mask = LVIF_TEXT; // 文字
    item.cchTextMax = MAX_PATH; // 文字长度
    item.iItem = 0;
    item.iSubItem = 0;
    ListView_DeleteAllItems(window->hList);
    for (j = 0; j < h; ++j) {
      int now = 0;
      for (k = 0; k <= w; ++k) {
        if (0 == k) {
          _snprintf(buf2, 256, "%d", j + 1);
          item.iItem = j;
          item.pszText = buf2;
          now = ListView_InsertItem(window->hList, (LPARAM)&item);
        } else {
          va_list arglist = (va_list)((uchar*)arr + j * al + (k - 1) * ai);
          arglist = cstr_vnprintf_arr(buf2, 256, fmt, arglist, NULL);
          ListView_SetItemTextA(window->hList, now, k, buf2);
        }
      }
    }
    if (nColumnWidth < 0) {
      for (k = 0; k <= w; ++k) {
        ListView_SetColumnWidth(window->hList, k, nColumnWidth);
      }
    }
  }
  return 0;
}
#endif

#endif // _GUI_WIN32_H_
