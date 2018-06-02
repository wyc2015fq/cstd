#define WM_MOUSEWHEEL 0x020A
#ifndef _WINDOW_W32_H_
#define _WINDOW_W32_H_
#include <windows.h>
#include <commctrl.h>
#include <winuser.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
//#include "cstd.h"
#include "macro.h"
//#include "dibapi.inl"
#ifndef GET_WHEEL_DELTA_WPARAM
#define GET_WHEEL_DELTA_WPARAM(wParam) ((short)HIWORD(wParam))
#endif
#if defined WIN32 || defined WIN64 || defined _WIN64
#if _MSC_VER >= 1200
#pragma warning( disable: 4710 )
#endif
#include <commctrl.h>
#include <winuser.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"advapi32.lib")
typedef int (* CvWin32WindowCallback)(HWND, UINT, WPARAM, LPARAM, int*);
static const char* trackbar_text =
    " ";
#if defined WIN64 || defined _WIN64
#define icvGetWindowLongPtr GetWindowLongPtr
#define icvSetWindowLongPtr( hwnd, id, ptr ) SetWindowLongPtr( hwnd, id, (LONG_PTR)(ptr) )
#define icvGetClassLongPtr GetClassLongPtr
#define CV_USERDATA GWLP_USERDATA
#define CV_WNDPROC GWLP_WNDPROC
#define CV_HCURSOR GCLP_HCURSOR
#define CV_HBRBACKGROUND GCLP_HBRBACKGROUND
#else
#define icvGetWindowLongPtr GetWindowLong
#define icvSetWindowLongPtr( hwnd, id, ptr ) SetWindowLong( hwnd, id, (size_t)ptr )
#define icvGetClassLongPtr GetClassLong
#define CV_USERDATA GWL_USERDATA
#define CV_WNDPROC GWL_WNDPROC
#define CV_HCURSOR GCL_HCURSOR
#define CV_HBRBACKGROUND GCL_HBRBACKGROUND
#endif
int FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin, const RGBQUAD* inpal)
{
  BITMAPINFOHEADER* bmih;
  assert(bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));
  bmih = &(bmi->bmiHeader);
  memset(bmih, 0, sizeof(*bmih));
  bmih->biSize = sizeof(BITMAPINFOHEADER);
  bmih->biWidth = width;
  bmih->biHeight = origin ? abs(height) : -abs(height);
  bmih->biPlanes = 1;
  bmih->biBitCount = (unsigned short) bpp;
  bmih->biCompression = BI_RGB;

  if (bpp <= 8) {
    RGBQUAD* palette = bmi->bmiColors;

    if (inpal) {
      memcpy(palette, inpal, 256 * sizeof(RGBQUAD));
    }
    else {
      int i;

      for (i = 0; i < 256; i++) {
        palette[ i ].rgbBlue = palette[ i ].rgbGreen = palette[ i ].rgbRed = (BYTE) i;
        palette[ i ].rgbReserved = 0;
      }
    }
  }

  return 0;
}
struct CvWindow;
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
  void (*notify)(int);
  void (*notify2)(int, void*);
  void* userdata;
  int id;
}
CvTrackbar;
#include <setjmp.h>
#define MSGBEG(w, msg) setjmp((w)->sigbuf)
#define MSGSIG(w, x) if (!setjmp((w)->retbuf)) {longjmp((w)->sigbuf,x);}
#define MSGRET(w) longjmp((w)->retbuf,1)
typedef int (*MouseCallback)(int event, int x, int y, int flags, void* param);
typedef struct uibtn {
  RECT rc;
  UINT stage;
} uibtn;
typedef struct CvWindow {
  int signature;
  char* name;
  struct CvWindow* prev;
  struct CvWindow* next;
  HWND frame;
  jmp_buf sigbuf;
  jmp_buf retbuf;
  HDC dc;
  HGDIOBJ image;
  int last_key;
  int flags;
  int status; //0 normal, 1 fullscreen (YV)
  MouseCallback on_mouse;
  void* on_mouse_param;
  struct {
    HWND toolbar;
    int pos;
    int rows;
    WNDPROC toolBarProc;
    CvTrackbar* first;
  }
  toolbar;
  uibtn btn[3];
  int hit;
  RECT rcCap;
  RECT rcClient;
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
static void icvUpdateWindowPos(CvWindow* window);
static CvWindow* hg_windows = 0;
static CvWindow* focus_window = 0;
static CvWin32WindowCallback hg_on_preprocess = 0, hg_on_postprocess = 0;
static HINSTANCE hg_hinstance = 0;
static const char* highGUIclassName = "HighGUI class";
static const char* mainHighGUIclassName = "Main HighGUI class";
void
cvDestroyAllWindows(void)
{
  CvWindow* window = hg_windows;

  while (window) {
    HWND mainhWnd = window->frame;
    window = window->next;
    SendMessage(mainhWnd, WM_CLOSE, 0, 0);
  }
}
static void icvCleanupHighgui(void)
{
  cvDestroyAllWindows();
  UnregisterClass(highGUIclassName, hg_hinstance);
  UnregisterClass(mainHighGUIclassName, hg_hinstance);
}
int cvInitSystem(int a, char** b)
{
  static int wasInitialized = 0;

  // check initialization status
  if (!wasInitialized) {
    WNDCLASS wndc;
    // Initialize the stogare
    hg_windows = 0;
    // Register the class
    wndc.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
    wndc.lpfnWndProc = WindowProc;
    wndc.cbClsExtra = 0;
    wndc.cbWndExtra = 0;
    wndc.hInstance = hg_hinstance;
    wndc.lpszClassName = highGUIclassName;
    wndc.lpszMenuName = highGUIclassName;
    wndc.hIcon = LoadIcon(0, IDI_APPLICATION);
    //wndc.hCursor = ( HCURSOR ) LoadCursor( 0, ( LPSTR ) ( size_t ) IDC_CROSS );
    wndc.hCursor = (HCURSOR) LoadCursor(0, (LPSTR)(size_t) IDC_ARROW);
    wndc.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH);
    RegisterClass(&wndc);
    wndc.lpszClassName = mainHighGUIclassName;
    wndc.lpszMenuName = mainHighGUIclassName;
    wndc.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH);
    wndc.lpfnWndProc = MainWindowProc;
    RegisterClass(&wndc);
    atexit(icvCleanupHighgui);
    wasInitialized = 1;
  }

  return 0;
}
int cvStartWindowThread()
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
  POINT pt = {0, 0};
  GetCaretPos(&pt);
  return pt;
}
BOOL CreateSolidCaret1(int nWidth, int nHeight)
{
  return focus_window ? CreateCaret(focus_window->hwnd, (HBITMAP) 0, nWidth, nHeight) : 0;
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
  CvWindow* window = (CvWindow*) icvGetWindowLongPtr(hwnd, CV_USERDATA);
  return window != 0 && hg_windows != 0 &&
      window->signature == CV_WINDOW_MAGIC_VAL ? window : 0;
}
static CvTrackbar* icvTrackbarByHWND(HWND hwnd)
{
  CvTrackbar* trackbar = (CvTrackbar*) icvGetWindowLongPtr(hwnd, CV_USERDATA);
  return trackbar != 0 && trackbar->signature == CV_TRACKBAR_MAGIC_VAL &&
      trackbar->hwnd == hwnd ? trackbar : 0;
}
// Window positions saving/loading added by Philip Gruebele.
//<a href="mailto:pgruebele@cox.net">pgruebele@cox.net</a>
// Restores the window position from the registry saved position.
static void
icvLoadWindowPos(const char* name, CvRect* prect)
{
  HKEY hkey;
  char szKey[ 1024 ];
  CvRect rect;
  strcpy(szKey, icvWindowPosRootKey);
  strcat(szKey, name);
  rect.x = rect.y = CW_USEDEFAULT;
  rect.width = rect.height = 320;

  if (RegOpenKeyEx(HKEY_CURRENT_USER, szKey, 0, KEY_QUERY_VALUE, &hkey) == ERROR_SUCCESS) {
    // Yes we are installed.
    DWORD dwType = 0;
    DWORD dwSize = sizeof(int);
    RegQueryValueEx(hkey, "Left", NULL, &dwType, (BYTE*) & rect.x, &dwSize);
    RegQueryValueEx(hkey, "Top", NULL, &dwType, (BYTE*) & rect.y, &dwSize);
    RegQueryValueEx(hkey, "Width", NULL, &dwType, (BYTE*) & rect.width, &dwSize);
    RegQueryValueEx(hkey, "Height", NULL, &dwType, (BYTE*) & rect.height, &dwSize);

    if (rect.x != (int) CW_USEDEFAULT && (rect.x < -200 || rect.x > 3000)) {
      rect.x = 100;
    }

    if (rect.y != (int) CW_USEDEFAULT && (rect.y < -200 || rect.y > 3000)) {
      rect.y = 100;
    }

    if (rect.width != (int) CW_USEDEFAULT && (rect.width < 0 || rect.width > 3000)) {
      rect.width = 100;
    }

    if (rect.height != (int) CW_USEDEFAULT && (rect.height < 0 || rect.height > 3000)) {
      rect.height = 100;
    }

    RegCloseKey(hkey);
  }

  *prect = rect;
}
// Window positions saving/loading added by Philip Gruebele.
//<a href="mailto:pgruebele@cox.net">pgruebele@cox.net</a>
// philipg. Saves the window position in the registry
static void
icvSaveWindowPos(const char* name, CvRect rect)
{
  static const DWORD MAX_RECORD_COUNT = 100;
  HKEY hkey;
  char szKey[ 1024 ];
  char rootKey[ 1024 ];
  strcpy(szKey, icvWindowPosRootKey);
  strcat(szKey, name);

  if (RegOpenKeyEx(HKEY_CURRENT_USER, szKey, 0, KEY_READ, &hkey) != ERROR_SUCCESS) {
    HKEY hroot;
    DWORD count = 0;
    FILETIME oldestTime = { UINT_MAX, UINT_MAX };
    char oldestKey[ 1024 ];
    char currentKey[ 1024 ];
    strcpy(rootKey, icvWindowPosRootKey);
    rootKey[ strlen(rootKey) - 1 ] = '\0';

    if (RegCreateKeyEx(HKEY_CURRENT_USER, rootKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ + KEY_WRITE, 0, &hroot, NULL) != ERROR_SUCCESS)
      //RegOpenKeyEx( HKEY_CURRENT_USER,rootKey,0,KEY_READ,&hroot) != ERROR_SUCCESS )
    {
      return ;
    }

    for (;;) {
      DWORD csize = sizeof(currentKey);
      FILETIME accesstime = { 0, 0 };
      LONG code = RegEnumKeyEx(hroot, count, currentKey, &csize, NULL, NULL, NULL, &accesstime);

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
      RegDeleteKey(hroot, oldestKey);
    }

    RegCloseKey(hroot);

    if (RegCreateKeyEx(HKEY_CURRENT_USER, szKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &hkey, NULL) != ERROR_SUCCESS) {
      return ;
    }
  }
  else {
    RegCloseKey(hkey);

    if (RegOpenKeyEx(HKEY_CURRENT_USER, szKey, 0, KEY_WRITE, &hkey) != ERROR_SUCCESS) {
      return ;
    }
  }

  RegSetValueEx(hkey, "Left", 0, REG_DWORD, (BYTE*) & rect.x, sizeof(rect.x));
  RegSetValueEx(hkey, "Top", 0, REG_DWORD, (BYTE*) & rect.y, sizeof(rect.y));
  RegSetValueEx(hkey, "Width", 0, REG_DWORD, (BYTE*) & rect.width, sizeof(rect.width));
  RegSetValueEx(hkey, "Height", 0, REG_DWORD, (BYTE*) & rect.height, sizeof(rect.height));
  RegCloseKey(hkey);
}
double cvGetMode_W32(const char* name) //YV
{
  double result = -1;
  CvWindow* window;
  CV_FUNCNAME("cvGetMode_W32");
  __BEGIN__;

  if (!name) {
    CV_ERROR(CV_StsNullPtr, "NULL name string");
  }

  window = icvFindWindowByName(name);

  if (!window) {
    CV_ERROR(CV_StsNullPtr, "NULL window");
  }

  result = window->status;
  __END__;
  return result;
}
#ifdef MONITOR_DEFAULTTONEAREST
void cvChangeMode_W32(const char* name, double prop_value) //Yannick Verdie
{
  CvWindow* window;
  CV_FUNCNAME("cvChangeMode_W32");
  __BEGIN__;

  if (!name) {
    CV_ERROR(CV_StsNullPtr, "NULL name string");
  }

  window = icvFindWindowByName(name);

  if (!window) {
    CV_ERROR(CV_StsNullPtr, "NULL window");
  }

  if (window->flags & CV_WINDOW_AUTOSIZE) { //if the flag CV_WINDOW_AUTOSIZE is set
    EXIT;
  }

  {
    DWORD dwStyle = GetWindowLongPtr(window->frame, GWL_STYLE);
    CvRect position;

    if (window->status == CV_WINDOW_FULLSCREEN && prop_value == CV_WINDOW_NORMAL) {
      icvLoadWindowPos(window->name, &position);
      SetWindowLongPtr(window->frame, GWL_STYLE, dwStyle | WS_CAPTION);
      SetWindowPos(window->frame, HWND_TOP, position.x, position.y , position.width, position.height, SWP_NOZORDER | SWP_FRAMECHANGED);
      window->status = CV_WINDOW_NORMAL;
      EXIT;
    }

    if (window->status == CV_WINDOW_NORMAL && prop_value == CV_WINDOW_FULLSCREEN) {
      //save dimension
      RECT rect;
      CvRect RectCV;
      HMONITOR hMonitor;
      MONITORINFO mi;
      GetWindowRect(window->frame, &rect);
      RectCV = cvRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
      icvSaveWindowPos(window->name, RectCV);
      //Look at coordinate for fullscreen
      hMonitor = MonitorFromRect(&rect, MONITOR_DEFAULTTONEAREST);
      mi.cbSize = sizeof(mi);
      GetMonitorInfo(hMonitor, &mi);
      //fullscreen
      position.x = mi.rcMonitor.left;
      position.y = mi.rcMonitor.top;
      position.width = mi.rcMonitor.right - mi.rcMonitor.left;
      position.height = mi.rcMonitor.bottom - mi.rcMonitor.top;
      SetWindowLongPtr(window->frame, GWL_STYLE, dwStyle & ~WS_CAPTION);
      SetWindowPos(window->frame, HWND_TOP, position.x, position.y , position.width, position.height, SWP_NOZORDER | SWP_FRAMECHANGED);
      window->status = CV_WINDOW_FULLSCREEN;
      EXIT;
    }
  }

  __END__;
}
#else
void cvChangeMode_W32(const char* c, double d) {}
#endif
int cvNamedWindow(const char* name, int flags)
{
  int result = 0;
  HWND mainhWnd;
  CvWindow* window;
  int len;
  CvRect rect;
  CV_FUNCNAME("cvNamedWindow");
  __BEGIN__;
  cvInitSystem(0, 0);

  if (!name) {
    CV_ERROR(CV_StsNullPtr, "NULL name string");
  }

  // Check the name in the storage
  if (icvFindWindowByName(name) != 0) {
    result = 1;
    EXIT;
  }

  icvLoadWindowPos(name, &rect);
  mainhWnd = CreateWindowEx(0x80000, "Main HighGUI class", name, WS_POPUP,
      rect.x, rect.y, rect.width, rect.height, 0, 0, hg_hinstance, 0);

  if (!mainhWnd) {
    CV_ERROR(CV_StsError, "Frame window can not be created");
  }

  ShowWindow(mainhWnd, SW_SHOW);
  len = (int) strlen(name);
  CV_CALL(window = (CvWindow*) cvAlloc(sizeof(CvWindow) + len + 1));
  window->signature = CV_WINDOW_MAGIC_VAL;
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
  icvSetWindowLongPtr(mainhWnd, CV_USERDATA, window);
  // Recalculate window position
  icvUpdateWindowPos(window);
  result = 1;
  __END__;
  return result;
}
static void icvRemoveWindow(CvWindow* window)
{
  CvTrackbar* trackbar = NULL;
  RECT wrect = {0, 0, 0, 0};

  if (window->frame) {
    GetWindowRect(window->frame, &wrect);
  }

  if (window->name)
    icvSaveWindowPos(window->name, cvRect(wrect.left, wrect.top,
        wrect.right - wrect.left, wrect.bottom - wrect.top));

  if (window->frame) {
    icvSetWindowLongPtr(window->frame, CV_USERDATA, 0);
  }

  if (window->toolbar.toolbar) {
    icvSetWindowLongPtr(window->toolbar.toolbar, CV_USERDATA, 0);
  }

  if (window->prev) {
    window->prev->next = window->next;
  }
  else {
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
      icvSetWindowLongPtr(trackbar->hwnd, CV_USERDATA, 0);
      cvFree(&trackbar);
    }

    trackbar = next;
  }

  cvFree(&window);
}
void cvDestroyWindow(const char* name)
{
  CvWindow* window;
  HWND mainhWnd;
  CV_FUNCNAME("cvDestroyWindow");
  __BEGIN__;

  if (!name) {
    CV_ERROR(CV_StsNullPtr, "NULL name string");
  }

  window = icvFindWindowByName(name);

  if (!window) {
    EXIT;
  }

  mainhWnd = window->frame;
  SendMessage(mainhWnd, WM_CLOSE, 0, 0);
  // Do NOT call _remove_window -- CvWindow list will be updated automatically ...
  __END__;
}
static void icvScreenToClient(HWND hwnd, RECT* rect)
{
  POINT p;
  p.x = rect->left;
  p.y = rect->top;
  ScreenToClient(hwnd, &p);
  OffsetRect(rect, p.x - rect->left, p.y - rect->top);
}
/* Calculatess the window coordinates relative to the upper left corner of the mainhWnd window */
static RECT icvCalcWindowRect(CvWindow* window)
{
  const int gutter = 1;
  RECT crect, trect, rect;
  assert(window);
  GetClientRect(window->frame, &crect);

  if (window->toolbar.toolbar) {
    GetWindowRect(window->toolbar.toolbar, &trect);
    icvScreenToClient(window->frame, &trect);
    SubtractRect(&rect, &crect, &trect);
  }
  else {
    rect = crect;
  }

  rect.top += gutter;
  rect.left += gutter;
  rect.bottom -= gutter;
  rect.right -= gutter;
  return rect;
}
// returns TRUE if there is a problem such as ERROR_IO_PENDING.
static BOOL icvGetBitmapData(CvWindow* window, SIZE* size, int* cn, void** data)
{
  BITMAP bmp;
  HGDIOBJ h;
  GdiFlush();
  h = GetCurrentObject(window->dc, OBJ_BITMAP);

  if (size) {
    size->w = size->h = 0;
  }

  if (data) {
    * data = 0;
  }

  if (h == NULL) {
    return TRUE;
  }

  if (GetObject(h, sizeof(bmp), &bmp) == 0) {
    return TRUE;
  }

  if (size) {
    size->w = abs(bmp.bmWidth);
    size->h = abs(bmp.bmHeight);
  }

  if (cn) {
    * cn = bmp.bmBitsPixel / 8;
  }

  if (data) {
    * data = bmp.bmBits;
  }

  return FALSE;
}
static void icvUpdateWindowPos(CvWindow* window)
{
  RECT rect;
  assert(window);

  if ((window->flags & CV_WINDOW_AUTOSIZE) && window->image) {
    int i;
    SIZE size = {0, 0};
    icvGetBitmapData(window, &size, 0, 0);

    // Repeat two times because after the first resizing of the mainhWnd window
    // toolbar may resize too
    for (i = 0; i < (window->toolbar.toolbar ? 2 : 1); i++) {
      RECT rmw, rw = icvCalcWindowRect(window);
      GetWindowRect(window->frame, &rmw);
      // Resize the mainhWnd window in order to make the bitmap fit into the child window
      MoveWindow(window->frame, rmw.left, rmw.top,
          rmw.right - rmw.left + size.w - rw.right + rw.left,
          rmw.bottom - rmw.top + size.h - rw.bottom + rw.top, TRUE);
    }
  }

  rect = icvCalcWindowRect(window);
}
int NameWindowImage(const char* name, int height, int width)
{
  CvWindow* window;
  SIZE size = { 0, 0 };
  int cn = 0;
  void* dst_ptr = 0;
  const int channels0 = 3;
  int origin = 0;
  BOOL changed_size = FALSE; // philipg
  CV_FUNCNAME("NameWindowImage");
  __BEGIN__;

  if (!name) {
    CV_ERROR(CV_StsNullPtr, "NULL name");
  }

  window = icvFindWindowByName(name);

  if (!window) {
    cvNamedWindow(name, 1);
    window = icvFindWindowByName(name);
  }

  if (!window) {
    EXIT; // keep silence here.
  }

  if (window->image)

    // if there is something wrong with these system calls, we cannot display image...
    if (icvGetBitmapData(window, &size, &cn, &dst_ptr)) {
      return 0;
    }

  if (height > 0 && width > 0) {
    if (size.w != width || size.h != height || cn != channels0) {
      uchar buffer[ sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD) ];
      BITMAPINFO* binfo = (BITMAPINFO*) buffer;
      changed_size = TRUE;
      DeleteObject(SelectObject(window->dc, window->image));
      window->image = 0;
      size.w = width;
      size.h = height;
      cn = channels0;
      FillBitmapInfo(binfo, size.w, size.h, cn * 8, 1, 0);
      window->image = SelectObject(window->dc, CreateDIBSection(window->dc, binfo,
          DIB_RGB_COLORS, &dst_ptr, 0, 0));
    }
  }
  else {
    width = size.w;
    height = size.h;
  }

  if (changed_size) {
    icvUpdateWindowPos(window);
  }

  InvalidateRect(window->frame, 0, 0);
  __END__;
  return 0;
}
int GetWindowImage(const char* name, int* pheight, int* pwidth, unsigned char** arr, int* pstep, int* pcn)
{
  CvWindow* window;
  SIZE size = { 0, 0 };
  int cn = 0, step = 0;
  unsigned char* dst_ptr = 0;
  const int channels0 = 3;
  int origin = 0;
  BOOL changed_size = FALSE; // philipg
  CV_FUNCNAME("NameWindowImage");
  __BEGIN__;

  if (!name) {
    CV_ERROR(CV_StsNullPtr, "NULL name");
  }

  window = icvFindWindowByName(name);

  if (!window) {
    cvNamedWindow(name, 1);
    NameWindowImage(name, 300, 300);
    window = icvFindWindowByName(name);
  }

  if (!window || !arr) {
    EXIT; // keep silence here.
  }

  if (icvGetBitmapData(window, &size, &cn, (void**)&dst_ptr)) {
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

  InvalidateRect(window->frame, 0, 0);
  __END__;
  return NULL != dst_ptr;
}
int InvalidateWindow(const char* name)
{
  CvWindow* window;
  CV_FUNCNAME("InvalidateWindow");
  __BEGIN__;

  if (!name) {
    CV_ERROR(CV_StsNullPtr, "NULL name");
  }

  window = icvFindWindowByName(name);

  if (!window) {
    EXIT; // keep silence here.
  }

  InvalidateRect(window->frame, 0, 0);
  __END__;
  return 0;
}
void cvShowImagePal(const char* name, int height, int width, const unsigned char* arr, int step, int cn, const RGBQUAD* inpal)
{
  CvWindow* window;
  SIZE size = { 0, 0 };
  int cn = 0;
  void* dst_ptr = 0;
  int origin = 0;
  //CvMat stub, dst;
  BOOL changed_size = FALSE; // philipg
  CV_FUNCNAME("cvShowImage");
  __BEGIN__;

  if (!name) {
    CV_ERROR(CV_StsNullPtr, "NULL name");
  }

  window = icvFindWindowByName(name);

  if (!window) {
    // run_once 使这段代码只运行一次
    cvNamedWindow(name, 0);
    window = icvFindWindowByName(name);
  }

  if (!window || !arr) {
    EXIT; // keep silence here.
  }

  //CV_CALL( image = cvGetMat( arr, &stub ));
  if (window->image)

    // if there is something wrong with these system calls, we cannot display image...
    if (icvGetBitmapData(window, &size, &cn, &dst_ptr)) {
      return ;
    }

  if (size.w != width || size.h != height || cn != cn) {
    uchar buffer[ sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD) ];
    BITMAPINFO* binfo = (BITMAPINFO*) buffer;

    if (inpal) {
      LPLOGPALETTE lpPal = (LPLOGPALETTE)buffer; // 指向逻辑调色板的指针
      HPALETTE hPal = NULL; // 调色板的句柄
      int i;
      lpPal->palVersion = 0x300; // 设置版本号
      lpPal->palNumEntries = (WORD) 256; // 设置颜色数目

      for (i = 0; i < (int) 256; i++) {
        lpPal->palPalEntry[ i ].peRed = inpal[ i ].rgbRed;
        lpPal->palPalEntry[ i ].peGreen = inpal[ i ].rgbGreen;
        lpPal->palPalEntry[ i ].peBlue = inpal[ i ].rgbBlue;
        lpPal->palPalEntry[ i ].peFlags = 0;
      }

      hPal = CreatePalette(lpPal);
      DeleteObject(SelectPalette(window->dc, hPal, TRUE));
    }

    changed_size = TRUE;
    DeleteObject(SelectObject(window->dc, window->image));
    window->image = 0;
    size.w = width;
    size.h = height;
    FillBitmapInfo(binfo, size.w, size.h, cn * 8, 1, inpal);
    window->image = SelectObject(window->dc, CreateDIBSection(window->dc, binfo, DIB_RGB_COLORS, &dst_ptr, 0, 0));
  }

  //cvInitMatHeader( &dst, size.h, size.w, CV_8UC3,
  // dst_ptr, (size.w * cn + 3) & -4 );
  //cvConvertImage( image, &dst, origin == 0 ? CV_CVTIMG_FLIP : 0 );
  {
    int i, dststep = (size.w * cn + 3) & -4;
    unsigned char* dst = (unsigned char*)dst_ptr;
    dst += (size.h - 1) * dststep;

    //IM2IM( size.h, size.w, arr, step, cn, dst, -dststep, cn );
    for (i = 0; i < size.h; ++i) {
      memcpy(dst + i * (-dststep), arr + i * step, size.w * cn);
    }

    if (1 == cn) {
      int s = 0;
      SKIP_MAX_ELEMENT(height, width, arr, step, cn, s);
      dst = (unsigned char*)dst_ptr;

      if (1 == arr[s]) {
        int i;

        for (i = 0; i < size.h * dststep; ++i) {
          dst[i] = dst[i] ? 255 : 0;
        }
      }
    }
  }

  // ony resize window if needed
  if (changed_size) {
    icvUpdateWindowPos(window);
  }

  InvalidateRect(window->frame, 0, 0);
  // philipg: this is not needed and just slows things down
  // UpdateWindow(window->hwnd);
  __END__;
}
void
cvSetWindowText(const char* name, const char* text)
{
  CvWindow* window;
  SIZE size = { 0, 0 };
  int cn = 0;
  void* dst_ptr = 0;
  const int channels0 = 3;
  int origin = 0;
  //CvMat stub, dst;
  BOOL changed_size = FALSE; // philipg
  CV_FUNCNAME("cvSetWindowText");
  __BEGIN__;

  if (!name) {
    CV_ERROR(CV_StsNullPtr, "NULL name");
  }

  window = icvFindWindowByName(name);

  if (!window) {
    cvNamedWindow(name, 1);
    window = icvFindWindowByName(name);
  }

  if (!window) {
    EXIT; // keep silence here.
  }

  SetWindowText(window->frame, text);
  __END__;
}
void cvResizeWindow(const char* name, int width, int height)
{
  int i;
  CvWindow* window;
  RECT rmw, rw, rect;
  CV_FUNCNAME("cvResizeWindow");
  __BEGIN__;

  if (!name) {
    CV_ERROR(CV_StsNullPtr, "NULL name");
  }

  window = icvFindWindowByName(name);

  if (!window) {
    EXIT;
  }

  // Repeat two times because after the first resizing of the mainhWnd window
  // toolbar may resize too
  for (i = 0; i < (window->toolbar.toolbar ? 2 : 1); i++) {
    rw = icvCalcWindowRect(window);
    GetWindowRect(window->frame, &rmw);
    // Resize the mainhWnd window in order to make the bitmap fit into the child window
    MoveWindow(window->frame, rmw.left, rmw.top,
        rmw.right - rmw.left + width - rw.right + rw.left,
        rmw.bottom - rmw.top + height - rw.bottom + rw.top, TRUE);
  }

  __END__;
}
void cvMoveWindow(const char* name, int x, int y)
{
  CvWindow* window;
  RECT rect;
  CV_FUNCNAME("cvMoveWindow");
  __BEGIN__;

  if (!name) {
    CV_ERROR(CV_StsNullPtr, "NULL name");
  }

  window = icvFindWindowByName(name);

  if (!window) {
    EXIT;
  }

  GetWindowRect(window->frame, &rect);
  MoveWindow(window->frame, x, y, rect.right - rect.left, rect.bottom - rect.top, TRUE);
  __END__;
}
void FillSolidRect(HDC m_hDC, const RECT* rect, COLORREF clr)
{
  ASSERT(m_hDC != NULL);
  SetBkColor(m_hDC, clr);
  ExtTextOut(m_hDC, 0, 0, ETO_OPAQUE, rect, NULL, 0, NULL);
}
static LRESULT CALLBACK
MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
      RECT rect;
      SIZE size = {0, 0};
      int flags = (wParam & MK_LBUTTON ? CV_EVENT_FLAG_LBUTTON : 0) |
          (wParam & MK_RBUTTON ? CV_EVENT_FLAG_RBUTTON : 0) |
          (wParam & MK_MBUTTON ? CV_EVENT_FLAG_MBUTTON : 0) |
          (wParam & MK_CONTROL ? CV_EVENT_FLAG_CTRLKEY : 0) |
          (wParam & MK_SHIFT ? CV_EVENT_FLAG_SHIFTKEY : 0) |
          (GetKeyState(VK_MENU) < 0 ? CV_EVENT_FLAG_ALTKEY : 0);
      int event = GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? CV_EVENT_MOUSEWHEELUP : CV_EVENT_MOUSEWHEELDOWN;
      pt.x = LOWORD(lParam);
      pt.y = HIWORD(lParam);
      icvGetBitmapData(window, &size, 0, 0);
      window->on_mouse(event, pt.x * size.w / MAX(rect.right - rect.left, 1),
          pt.y * size.h / MAX(rect.bottom - rect.top, 1), flags,
          window->on_mouse_param);
    }

    break;
#endif

  case WM_PAINT:
    if (window->image != 0) {
      int nchannels = 3;
      SIZE size = {0, 0};
      PAINTSTRUCT paint;
      HDC hdc;
      // Determine the bitmap's dimensions
      icvGetBitmapData(window, &size, &nchannels, 0);
      hdc = BeginPaint(hwnd, &paint);
      SetStretchBltMode(hdc, COLORONCOLOR);

      if (nchannels == 1) {
        int i;
        RGBQUAD table[ 256 ];

        for (i = 0; i < 256; i++) {
          table[ i ].rgbBlue = (unsigned char) i;
          table[ i ].rgbGreen = (unsigned char) i;
          table[ i ].rgbRed = (unsigned char) i;
        }

        //SetDIBColorTable( window->dc, 0, 255, table );
      }

      if (1) {
        BLENDFUNCTION m_Blend;
        RECT rct;
        POINT ptSrc = {0, 0};
        POINT ptWinPos;
        GetWindowRect(hwnd, &rct);
        ptWinPos.x = rct.left, ptWinPos.y = rct.top;
        m_Blend.BlendOp = AC_SRC_OVER; //theonlyBlendOpdefinedinWindows2000
        m_Blend.BlendFlags = 0; //nothingelseisspecial
        m_Blend.AlphaFormat = AC_SRC_ALPHA; //
        m_Blend.SourceConstantAlpha = 255; //AC_SRC_ALPHA 透明度
        //printf("UpdateLayeredWindow\n");
        UpdateLayeredWindow(hwnd, hdc, &ptWinPos, &size, window->dc, &ptSrc, 0, &m_Blend, 2);
      }

      EndPaint(hwnd, &paint);
    }
    else {
      return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    break;

  case WM_GETMINMAXINFO:
    if (!(window->flags & CV_WINDOW_AUTOSIZE)) {
      MINMAXINFO* minmax = (MINMAXINFO*) lParam;
      RECT rect;
      LRESULT retval = DefWindowProc(hwnd, uMsg, wParam, lParam);
      minmax->ptMinTrackSize.y = 100;
      minmax->ptMinTrackSize.x = 100;

      if (window->toolbar.first) {
        GetWindowRect(window->toolbar.first->hwnd, &rect);
        minmax->ptMinTrackSize.y += window->toolbar.rows * (rect.bottom - rect.top);
        minmax->ptMinTrackSize.x = MAX(rect.right - rect.left + HG_BUDDY_WIDTH, HG_BUDDY_WIDTH * 2);
      }

      return retval;
    }

    break;

  case WM_WINDOWPOSCHANGED: {
    WINDOWPOS* pos = (WINDOWPOS*) lParam;

    // Update the toolbar position/size
    if (window->toolbar.toolbar) {
      RECT rect;
      GetWindowRect(window->toolbar.toolbar, &rect);
      MoveWindow(window->toolbar.toolbar, 0, 0, pos->w, rect.bottom - rect.top, TRUE);
    }

    //if ( !( window->flags & CV_WINDOW_AUTOSIZE ) )
    // icvUpdateWindowPos( window );
    break;
  }

  case WM_ACTIVATE:
    if (LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE) {
      SetFocus(window->frame);
    }

    break;

  case WM_ERASEBKGND: {
    RECT cr, tr, wrc;
    HRGN rgn, rgn1, rgn2;
    int ret;
    HDC hdc = (HDC) wParam;
    GetWindowRect(window->frame, &cr);
    icvScreenToClient(window->frame, &cr);

    if (window->toolbar.toolbar) {
      GetWindowRect(window->toolbar.toolbar, &tr);
      icvScreenToClient(window->frame, &tr);
    }
    else {
      tr.left = tr.top = tr.right = tr.bottom = 0;
    }

    GetClientRect(window->frame, &wrc);
    rgn = CreateRectRgn(0, 0, wrc.right, wrc.bottom);
    rgn1 = CreateRectRgn(cr.left, cr.top, cr.right, cr.bottom);
    rgn2 = CreateRectRgn(tr.left, tr.top, tr.right, tr.bottom);
    ret = CombineRgn(rgn, rgn, rgn1, RGN_DIFF);
    ret = CombineRgn(rgn, rgn, rgn2, RGN_DIFF);

    if (ret != NULLREGION && ret != ERROR) {
      FillRgn(hdc, rgn, (HBRUSH) icvGetClassLongPtr(hwnd, CV_HBRBACKGROUND));
    }

    DeleteObject(rgn);
    DeleteObject(rgn1);
    DeleteObject(rgn2);
  }

  return 1;

  case WM_SETFOCUS: {
    focus_window = window;
  }
  break;

  case WM_NCHITTEST:
  case WM_NCLBUTTONDOWN:
  case WM_NCLBUTTONUP:
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
    if (window->on_mouse) {
      int x1, y1;
      int hit;
      RECT rect = {0, 0};
      SIZE size = {0, 0};

      if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN) {
        SetCapture(hwnd);
      }

      if (uMsg == WM_LBUTTONUP || uMsg == WM_RBUTTONUP || uMsg == WM_MBUTTONUP) {
        ReleaseCapture();
      }

      x1 = LOWORD(lParam);
      y1 = HIWORD(lParam);

      if (WM_NCHITTEST == uMsg) {
        GetWindowRect(window->frame, &rect);
        //printf(" = %d, %d\n", rect.left, rect.top);
      }

      hit = window->on_mouse(uMsg, x1 - rect.left, y1 - rect.top, wParam, window->on_mouse_param);

      if (0 == hit) {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
      }

      return hit;
    }

    break;

  case WM_DESTROY:
    icvRemoveWindow(window);
    // Do nothing!!!
    //PostQuitMessage(0);
    break;

  case WM_SETCURSOR:
    SetCursor((HCURSOR) icvGetClassLongPtr(hwnd, CV_HCURSOR));
    return 0;

  case WM_KEYDOWN:
    window->last_key = (int) wParam;
    return 0;
  }

  return DefWindowProc(hwnd, uMsg, wParam, lParam);
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
  switch (uMsg) {
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
static void icvUpdateTrackbar(CvTrackbar* trackbar, int pos)
{
  const int max_name_len = 10;
  const char* suffix = "";
  char pos_text[ 32 ];
  int name_len;

  if (trackbar->data) {
    * trackbar->data = pos;
  }

  if (trackbar->pos != pos) {
    trackbar->pos = pos;

    if (trackbar->notify2) {
      trackbar->notify2(pos, trackbar->userdata);
    }

    if (trackbar->notify) {
      trackbar->notify(pos);
    }

    name_len = (int) strlen(trackbar->name);

    if (name_len > max_name_len) {
      int start_len = max_name_len * 2 / 3;
      int end_len = max_name_len - start_len - 2;
      memcpy(pos_text, trackbar->name, start_len);
      memcpy(pos_text + start_len, "...", 3);
      memcpy(pos_text + start_len + 3, trackbar->name + name_len - end_len, end_len + 1);
    }
    else {
      memcpy(pos_text, trackbar->name, name_len + 1);
    }

    sprintf(pos_text + strlen(pos_text), "%s: %d\n", suffix, pos);
    SetWindowText(trackbar->buddy, pos_text);
  }
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
    HWND slider = (HWND) lParam;
    int pos = (int) SendMessage(slider, TBM_GETPOS, 0, 0);
    CvTrackbar* trackbar = icvTrackbarByHWND(slider);

    if (trackbar) {
      if (trackbar->pos != pos) {
        icvUpdateTrackbar(trackbar, pos);
      }
    }

    SetFocus(window->frame);
    return 0;
  }

  case WM_NCCALCSIZE: {
    LRESULT ret = CallWindowProc(window->toolbar.toolBarProc, hwnd, uMsg, wParam, lParam);
    int rows = (int) SendMessage(hwnd, TB_GETROWS, 0, 0);

    if (window->toolbar.rows != rows) {
      CvTrackbar* trackbar;
      SendMessage(window->toolbar.toolbar, TB_BUTTONCOUNT, 0, 0);
      trackbar = window->toolbar.first;

      for (; trackbar != 0; trackbar = trackbar->next) {
        RECT rect;
        SendMessage(window->toolbar.toolbar, TB_GETITEMRECT,
            (WPARAM) trackbar->id, (LPARAM) & rect);
        MoveWindow(trackbar->hwnd, rect.left + HG_BUDDY_WIDTH, rect.top,
            rect.right - rect.left - HG_BUDDY_WIDTH,
            rect.bottom - rect.top, FALSE);
        MoveWindow(trackbar->buddy, rect.left, rect.top,
            HG_BUDDY_WIDTH, rect.bottom - rect.top, FALSE);
      }

      window->toolbar.rows = rows;
    }

    return ret;
  }
  }

  return CallWindowProc(window->toolbar.toolBarProc, hwnd, uMsg, wParam, lParam);
}
int
cvWaitKey(int delay)
{
  int time0 = GetTickCount();

  if (NULL == hg_windows) {
    return -2;
  }

  for (;;) {
    CvWindow* window;
    MSG message;
    int is_processed = 0;

    if ((delay > 0 && abs((int)(GetTickCount() - time0)) >= delay) || hg_windows == 0) {
      return -1;
    }

    if (delay <= 0) {
      GetMessage(&message, 0, 0, 0);
    }
    else if (PeekMessage(&message, 0, 0, 0, PM_REMOVE) == FALSE) {
      Sleep(1);
      continue;
    }

    for (window = hg_windows; window != 0 && is_processed == 0; window = window->next) {
      if (window->frame == message.hwnd) {
        is_processed = 1;

        switch (message.message) {
        case WM_DESTROY:
        case WM_CHAR:
          DispatchMessage(&message);
          return (int) message.wParam;

        case WM_SYSKEYDOWN:
          if (message.wParam == VK_F10) {
            is_processed = 1;
            return (int)(message.wParam << 16);
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
static int
icvCreateTrackbar(const char* trackbar_name, const char* window_name,
    int* val, int count, TrackbarCallback on_notify,
    CvTrackbarCallback2 on_notify2, void* userdata)
{
  char slider_name[ 32 ];
  CvWindow* window = 0;
  CvTrackbar* trackbar = 0;
  int pos = 0;
  int result = 0;
  CV_FUNCNAME("icvCreateTrackbar");
  __BEGIN__;

  if (!window_name || !trackbar_name) {
    CV_ERROR(CV_StsNullPtr, "NULL window or trackbar name");
  }

  if (count <= 0) {
    CV_ERROR(CV_StsOutOfRange, "Bad trackbar maximal value");
  }

  window = icvFindWindowByName(window_name);

  if (!window) {
    EXIT;
  }

  trackbar = icvFindTrackbarByName(window, trackbar_name);

  if (!trackbar) {
    TBBUTTON tbs;
    ButtonInfo tbis;
    RECT rect;
    int bcount;
    int len = (int) strlen(trackbar_name);

    // create toolbar if it is not created yet
    if (!window->toolbar.toolbar) {
      const int default_height = 30;
      window->toolbar.toolbar = CreateToolbarEx(
          window->frame, WS_CHILD | CCS_TOP | TBSTYLE_WRAPABLE,
          1, 0, 0, 0, 0, 0, 16, 20, 16, 16, sizeof(TBBUTTON));
      GetClientRect(window->frame, &rect);
      MoveWindow(window->toolbar.toolbar, 0, 0,
          rect.right - rect.left, default_height, TRUE);
      SendMessage(window->toolbar.toolbar, TB_AUTOSIZE, 0, 0);
      ShowWindow(window->toolbar.toolbar, SW_SHOW);
      window->toolbar.first = 0;
      window->toolbar.pos = 0;
      window->toolbar.rows = 0;
      window->toolbar.toolBarProc =
          (WNDPROC) icvGetWindowLongPtr(window->toolbar.toolbar, CV_WNDPROC);
      icvUpdateWindowPos(window);
      // Subclassing from toolbar
      icvSetWindowLongPtr(window->toolbar.toolbar, CV_WNDPROC, HGToolbarProc);
      icvSetWindowLongPtr(window->toolbar.toolbar, CV_USERDATA, window);
    }

    /* Retrieve current buttons count */
    bcount = (int) SendMessage(window->toolbar.toolbar, TB_BUTTONCOUNT, 0, 0);

    if (bcount > 1) {
      /* If this is not the first button then we need to
      separate it from the previous one */
      tbs.iBitmap = 0;
      tbs.idCommand = bcount; // Set button id to it's number
      tbs.iString = 0;
      tbs.fsStyle = TBSTYLE_SEP;
      tbs.fsState = TBSTATE_ENABLED;
      SendMessage(window->toolbar.toolbar, TB_ADDBUTTONS, 1, (LPARAM) & tbs);
      // Retrieve current buttons count
      bcount = (int) SendMessage(window->toolbar.toolbar, TB_BUTTONCOUNT, 0, 0);
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
    tbs.iString = (INT_PTR) trackbar_text;
#endif
    SendMessage(window->toolbar.toolbar, TB_ADDBUTTONS, 1, (LPARAM) & tbs);
    /* Adjust button size to the slider */
    tbis.cbSize = sizeof(tbis);
    tbis.dwMask = TBIF_SIZE;
    GetClientRect(window->frame, &rect);
    tbis.w = (unsigned short)(rect.right - rect.left);
    SendMessage(window->toolbar.toolbar, TB_SETBUTTONINFO,
        (WPARAM) tbs.idCommand, (LPARAM) & tbis);
    /* Get button position */
    SendMessage(window->toolbar.toolbar, TB_GETITEMRECT,
        (WPARAM) tbs.idCommand, (LPARAM) & rect);
    /* Create a slider */
    trackbar = (CvTrackbar*) cvAlloc(sizeof(CvTrackbar) + len + 1);
    trackbar->signature = CV_TRACKBAR_MAGIC_VAL;
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
    trackbar->hwnd = CreateWindowEx(0, TRACKBAR_CLASS, slider_name,
        WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS |
        TBS_FIXEDLENGTH | TBS_HORZ | TBS_BOTTOM,
        rect.left + HG_BUDDY_WIDTH, rect.top,
        rect.right - rect.left - HG_BUDDY_WIDTH,
        rect.bottom - rect.top, window->toolbar.toolbar,
        (HMENU)(size_t) bcount, hg_hinstance, 0);
    sprintf(slider_name, "Buddy%p", val);
    trackbar->buddy = CreateWindowEx(0, "STATIC", slider_name,
        WS_CHILD | SS_RIGHT,
        rect.left, rect.top,
        HG_BUDDY_WIDTH, rect.bottom - rect.top,
        window->toolbar.toolbar, 0, hg_hinstance, 0);
    icvSetWindowLongPtr(trackbar->hwnd, CV_USERDATA, trackbar);
    /* Minimize the number of rows */
    SendMessage(window->toolbar.toolbar, TB_SETROWS,
        MAKEWPARAM(1, FALSE), (LPARAM) & rect);
  }
  else {
    trackbar->data = 0;
    trackbar->notify = 0;
    trackbar->notify2 = 0;
  }

  trackbar->maxval = count;
  /* Adjust slider parameters */
  SendMessage(trackbar->hwnd, TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG(0, count));
  SendMessage(trackbar->hwnd, TBM_SETTICFREQ, (WPARAM) 1, (LPARAM) 0);

  if (val) {
    pos = *val;
  }

  SendMessage(trackbar->hwnd, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) pos);
  SendMessage(window->toolbar.toolbar, TB_AUTOSIZE, 0, 0);
  trackbar->pos = -1;
  icvUpdateTrackbar(trackbar, pos);
  ShowWindow(trackbar->buddy, SW_SHOW);
  ShowWindow(trackbar->hwnd, SW_SHOW);
  trackbar->notify = on_notify;
  trackbar->notify2 = on_notify2;
  trackbar->userdata = userdata;
  trackbar->data = val;
  /* Resize the window to reflect the toolbar resizing*/
  icvUpdateWindowPos(window);
  result = 1;
  __END__;
  return result;
}
int
cvCreateTrackbar(const char* trackbar_name, const char* window_name,
    int* val, int count, TrackbarCallback on_notify)
{
  return icvCreateTrackbar(trackbar_name, window_name, val, count,
      on_notify, 0, 0);
}
int
cvCreateTrackbar2(const char* trackbar_name, const char* window_name,
    int* val, int count, CvTrackbarCallback2 on_notify2,
    void* userdata)
{
  return icvCreateTrackbar(trackbar_name, window_name, val, count,
      0, on_notify2, userdata);
}
void
cvSetMouseCallback(const char* window_name, MouseCallback on_mouse, void* param)
{
  CvWindow* window = 0;
  CV_FUNCNAME("cvSetMouseCallback");
  __BEGIN__;

  if (!window_name) {
    CV_ERROR(CV_StsNullPtr, "NULL window name");
  }

  window = icvFindWindowByName(window_name);

  if (!window) {
    EXIT;
  }

  window->on_mouse = on_mouse;
  window->on_mouse_param = param;
  __END__;
}
int cvGetTrackbarPos(const char* trackbar_name, const char* window_name)
{
  CvWindow* window;
  CvTrackbar* trackbar = 0;
  int pos = -1;
  CV_FUNCNAME("cvGetTrackbarPos");
  __BEGIN__;

  if (trackbar_name == 0 || window_name == 0) {
    CV_ERROR(CV_StsNullPtr, "NULL trackbar or window name");
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
void cvSetTrackbarPos(const char* trackbar_name, const char* window_name, int pos)
{
  CvWindow* window;
  CvTrackbar* trackbar = 0;
  CV_FUNCNAME("cvSetTrackbarPos");
  __BEGIN__;

  if (trackbar_name == 0 || window_name == 0) {
    CV_ERROR(CV_StsNullPtr, "NULL trackbar or window name");
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

    SendMessage(trackbar->hwnd, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) pos);
    icvUpdateTrackbar(trackbar, pos);
  }

  __END__;
}
void* cvGetWindowHandle(const char* window_name)
{
  CvWindow* window;
  void* hwnd = 0;
  CV_FUNCNAME("cvGetWindowHandle");
  __BEGIN__;

  if (window_name == 0) {
    CV_ERROR(CV_StsNullPtr, "NULL window name");
  }

  window = icvFindWindowByName(window_name);

  if (window) {
    hwnd = (void*) window->frame;
  }

  __END__;
  return hwnd;
}
const char* cvGetWindowName(void* window_handle)
{
  CvWindow* window;
  const char* window_name = "";
  CV_FUNCNAME("cvGetWindowName");
  __BEGIN__;

  if (window_handle == 0) {
    CV_ERROR(CV_StsNullPtr, "NULL window");
  }

  window = icvWindowByHWND((HWND) window_handle);

  if (window) {
    window_name = window->name;
  }

  __END__;
  return window_name;
}
void
cvSetPreprocessFuncWin32(int (__cdecl* on_preprocess)(HWND, UINT, WPARAM, LPARAM, int*))
{
  if (on_preprocess) {
    hg_on_preprocess = on_preprocess;
  }
  else {
    assert(on_preprocess);
  }
}
void
cvSetPostprocessFuncWin32(int (__cdecl* on_postprocess)(HWND, UINT, WPARAM, LPARAM, int*))
{
  if (on_postprocess) {
    hg_on_postprocess = on_postprocess;
  }
  else {
    assert(on_postprocess);
  }
}
#if 0
const TCHAR* myGetOpenFileName(TCHAR* szFilter)
{
  OPENFILENAME ofn; // 公共对话框结构。
  static TCHAR szFile[ MAX_PATH ];
  ZeroMemory(szFile, MAX_PATH * sizeof(TCHAR));
  // 初始化选择文件对话框。
  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = NULL;
  ofn.lpstrFile = szFile;
  ofn.lpstrFile[ 0 ] = _T('\0');
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFilter = szFilter ? szFilter : _T("All\0*.*\0图片\0*.bmp\0");
  ofn.nFilterIndex = 1;
  ofn.lpstrFileTitle = NULL;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrInitialDir = NULL;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  // 显示打开选择文件对话框。
  if (GetOpenFileName(&ofn)) {
    //显示选择的文件。
    //OutputDebugString(szFile);
    //OutputDebugString(_T("\r\n"));
  }

  return szFile;
}
#endif
static CvScalar hsv2rgb2(float hue)
{
  int rgb[3], p, sector;
  static const int sector_data[][3] =
  {{0, 2, 1}, {1, 2, 0}, {1, 0, 2}, {2, 0, 1}, {2, 1, 0}, {0, 1, 2}};
  hue *= 0.033333333333333333333333333333333f;
  sector = cvFloor(hue);
  p = cvRound(255 * (hue - sector));
  p ^= sector & 1 ? 255 : 0;
  rgb[sector_data[sector][0]] = 255;
  rgb[sector_data[sector][1]] = 0;
  rgb[sector_data[sector][2]] = p;
  return cvScalar(rgb[2], rgb[1], rgb[0], 0);
}
#include "imresize.h"
#if 0
int imshowhist(const char* name, int h, int w, const unsigned char* a, int al, int ai, int l, int r)
{
  int hist[256] = {0};
  int back[256] = {0};
  enum {cc = 2, hh = 100 * cc, ww = 256 * cc, hdims = 256};
  unsigned char bb[hh * ww * 3] = {0};
  int i, j, maxval, bin_w;

  if (l > r) {
    CV_SWAP(l, r, i);
  }

  l /= cc, r /= cc;
  l = BOUND(l, 0, 255);
  r = BOUND(r, 0, 255);
  SKIP_HIST(h, w, a, al, ai, hist);
  MINMAX_ELEM(256, hist, 1, i, j);
  maxval = hist[j];
  bin_w = ww / hdims;
  DrawRectangle(hh, ww, bb, ww * 3, 3, iPOINT(l * bin_w, hh),
      iPOINT(r * bin_w, 0), CV_RGB(255, 255, 255), -1, 8, 0);

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
    xDrawText(hh, ww, bb, ww * 3, 3, 0, 0, "黑体", 20, 0, 0, 0, str, CV_RGB(255, 0, 0));
    IMRESIZE(h, w, b, w * 3, 3, h2, w2, b2, w2 * 3, 3, INTER_NEARSAMPLE);
    PolyLineM(h2, w2, b2, w2 * 3, 3, &hor, &h, 1, 0, CV_RGB(255, 0, 0), 1, 8, 0);
    PolyLineM(h2, w2, b2, w2 * 3, 3, &ver, &w, 1, 0, CV_RGB(255, 0, 0), 1, 8, 0);
    cvNamedWindow("bin", 1);
    cvShowImage("bin", h2, w2, b2, w2 * 3, 3);
    SAFEFREE(b2);
    SAFEFREE(b);
    SAFEFREE(hor);
    SAFEFREE(ver);
  }

  //cvNamedWindow( name, 0 );
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
void _hist_event(int event, int x, int y, int flags, void* param)
{
  _hist_param* im = (_hist_param*)param;

  if (event == CV_EVENT_LBUTTONDOWN) {
    im->l = x;
    im->r = x;
    im->select_object = 1;
  }

  if (event == CV_EVENT_LBUTTONUP) {
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
  cvNamedWindow("hist", 1);
  im.h = h, im.w = w, im.a = a, im.al = al, im.ai = ai, im.l = 0, im.r = 0, im.select_object = 0;
  cvSetMouseCallback("hist", _hist_event, &im);
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
#endif
int ShowTaskBar(BOOL bShow)
{
  RECT rectWorkArea = {0, 0, 0, 0};
  RECT rectTaskBar = {0, 0, 0, 0};
  HWND pWnd = FindWindow("Shell_TrayWnd", "");

  if (!bShow) { //隐藏
    SystemParametersInfo(SPI_GETWORKAREA, 0, (LPVOID)&rectWorkArea, 0);

    if (pWnd) {
      GetWindowRect(pWnd, &rectTaskBar);
      rectWorkArea.bottom += RCH(&rectTaskBar);
      SystemParametersInfo(SPI_SETWORKAREA, 0, (LPVOID)&rectWorkArea, 0);
      ShowWindow(pWnd, SW_HIDE);
    }
  }
  else { //显示
    SystemParametersInfo(SPI_GETWORKAREA, 0, (LPVOID)&rectWorkArea, 0);

    if (pWnd) {
      GetWindowRect(pWnd, &rectTaskBar);
      rectWorkArea.bottom -= RCH(&rectTaskBar);
      SystemParametersInfo(SPI_SETWORKAREA, 0, (LPVOID)&rectWorkArea, 0);
      ShowWindow(pWnd, SW_SHOW);
    }
  }

  return 0;
}
int cvSetFullScreen(const char* name)
{
  CvWindow* window;
  HWND hWnd = 0;
  static RECT rc_back = {0, 0, 300, 300};
  static int is_FullScreen = 0;
  RECT rect;

  if (!name) {
    CV_ERROR(CV_StsNullPtr, "NULL name");
  }

  window = icvFindWindowByName(name);
  hWnd = window->frame;

  //ShowTaskBar(is_FullScreen);
  if (is_FullScreen) {
    rect = rc_back;
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) | ((WS_CAPTION | WS_BORDER)));
    ShowWindow(hWnd, SW_HIDE);
    SetWindowPos(hWnd, 0, rect.left, rect.top, RCW(&rect), RCH(&rect), SWP_SHOWWINDOW);
  }
  else {
    int frameWidth = GetSystemMetrics(SM_CXFRAME);
    int frameHeight = GetSystemMetrics(SM_CYFRAME);
    int captionHeight = GetSystemMetrics(SM_CYCAPTION);
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & (~(WS_CAPTION | WS_BORDER)));
    GetWindowRect(hWnd, &rc_back);
    GetClientRect(hWnd, &rect);
    rect.left = rect.left - frameWidth;
    rect.right = rect.left + screenWidth + 2 * frameWidth;
    rect.top = rect.top - frameHeight;
    rect.bottom = rect.top + screenHeight + 2 * frameHeight;
    ShowWindow(hWnd, SW_HIDE);
    SetForegroundWindow(hWnd);
    SetWindowPos(hWnd, HWND_TOPMOST, rect.left, rect.top, RCW(&rect), RCH(&rect), SWP_SHOWWINDOW | SWP_NOZORDER);
    //SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
  }

  is_FullScreen = !is_FullScreen;
  return 0;
}
#endif //WIN32
#endif // _WINDOW_W32_H_

