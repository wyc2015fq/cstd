#include "Windows.h"
#include "tchar.h"
#define WM_SHELLNOTIFY WM_USER+5
#define IDI_TRAY 0
#define IDM_RESTORE 1000
#define IDM_EXIT 1010
static TCHAR ClassName[] = _T("TrayIconWinClass");
static TCHAR AppName[] = _T("TrayIcon Demo");
static TCHAR RestoreString[] = _T("&Restore");
static TCHAR ExitString[] = _T("E&xit Program");
static HINSTANCE g_hInstance1;
static NOTIFYICONDATA note;
static HMENU hPopupMenu;
static LRESULT CALLBACK TrayProcWinMain(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  POINT pt;
  switch (Msg) {
  case WM_CREATE: {
    hPopupMenu = CreatePopupMenu();
    AppendMenu(hPopupMenu, MF_STRING, IDM_RESTORE, RestoreString);
    AppendMenu(hPopupMenu, MF_STRING, IDM_EXIT, ExitString);
  }
  break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  case WM_SIZE:
    if (wParam == SIZE_MINIMIZED) {
      note.cbSize = sizeof(NOTIFYICONDATA);
      note.hWnd = hwnd;
      note.uID = IDI_TRAY;
      note.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
      note.uCallbackMessage = WM_SHELLNOTIFY;
      note.hIcon = LoadIcon(NULL, IDI_WINLOGO);
      lstrcpy(note.szTip, AppName);
      ShowWindow(hwnd, SW_HIDE);
      Shell_NotifyIcon(NIM_ADD, &note);
    }
    break;
  case WM_COMMAND:
    if (lParam == 0) {
      Shell_NotifyIcon(NIM_DELETE, &note);
      if (LOWORD(wParam) == IDM_RESTORE) {
        ShowWindow(hwnd, SW_RESTORE);
      }
      else {
        DestroyWindow(hwnd);
      }
    }
    break;
  case WM_SHELLNOTIFY:
    if (wParam == IDI_TRAY) {
      if (lParam == WM_RBUTTONDOWN) {
        GetCursorPos(&pt);
        TrackPopupMenu(hPopupMenu, TPM_RIGHTALIGN, pt.x, pt.y, 0, hwnd, NULL);
      }
      else if (lParam == WM_LBUTTONDBLCLK) {
        SendMessage(hwnd, WM_COMMAND, IDM_RESTORE, 0);
      }
    }
    break;
  default:
    return DefWindowProc(hwnd, Msg, wParam, lParam);
  }
  return 0;
}
static INT_PTR CALLBACK ProcWinMain1(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  switch (Msg) {
  case WM_CREATE: {
  }
  return DefWindowProc(hwnd, Msg, wParam, lParam);
  break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  case WM_SIZE:
    if (wParam == SIZE_MINIMIZED) {
    }
    return DefWindowProc(hwnd, Msg, wParam, lParam);
    break;
  case WM_COMMAND:
    if (lParam == 0) {
    }
    break;
  case WM_SHELLNOTIFY:
    if (wParam == IDI_TRAY) {
    }
    return DefWindowProc(hwnd, Msg, wParam, lParam);
    break;
  default:
    return DefWindowProc(hwnd, Msg, wParam, lParam);
  }
  return DefWindowProc(hwnd, Msg, wParam, lParam);
}
#include "DockingMarkers.inl"
static int ldown = 0;
typedef struct MarkerWnd {
  HIMAGELIST m_ImageList;
  int m_Marker;
  COLORREF m_clrMask;
  BOOL m_bDisable;
  BOOL m_bHighlight;
} MarkerWnd;
static LRESULT CALLBACK MarkerWndProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  MarkerWnd* s = (MarkerWnd*)GetWindowLong(hwnd, GWL_USERDATA);
  switch (Msg) {
  case WM_CREATE: {
    CREATESTRUCT* ps = (CREATESTRUCT*)lParam;
    SetWindowLong(hwnd, GWL_USERDATA, (LONG)ps->lpCreateParams);
    SetTimer(hwnd, 10, 30, 0);
  }
  break;
  case WM_TIMER: {
    static int aaa = 0;
    aaa = (aaa + 2) % 200;
    SetWindowTransparence(hwnd, RGB(255, 0, 255), aaa < 100 ? aaa : 200 - aaa);
    InvalidateRect(hwnd, 0, 0);
  }
  break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  case WM_LBUTTONDOWN:
    ldown = 1;
    break;
  case WM_MOUSEMOVE:
    if (wParam & MK_LBUTTON) {
      printf("WM_MOUSEMOVE\n");
    }
    break;
  case WM_SIZE:
    break;
  case WM_NCHITTEST:
    return HTCAPTION;
    break;
  case WM_COMMAND:
    if (lParam == 0) {
    }
    break;
  case WM_SHELLNOTIFY:
    if (wParam == IDI_TRAY) {
    }
    return DefWindowProc(hwnd, Msg, wParam, lParam);
    break;
  case WM_PAINT:
    if (s) {
      PAINTSTRUCT ps = {0};
      HDC hDC = BeginPaint(hwnd, &ps);
      printf("adfadsf\n");
      ImageList_Draw(s->m_ImageList, (s->m_bDisable == FALSE ? (s->m_bHighlight == FALSE ? 0 : 1) : 2), hDC, 0, 0, ILD_NORMAL);
      EndPaint(hwnd, &ps);
    }
    break;
  default:
    return DefWindowProc(hwnd, Msg, wParam, lParam);
  }
  return DefWindowProc(hwnd, Msg, wParam, lParam);
}
static int test_Tray()
{
  WNDCLASSEX wc = {0};
  MSG msg;
  HWND hwnd;
  HINSTANCE hInstance = NULL;
  int dwStyle;
  dwStyle = WS_OVERLAPPEDWINDOW;
  dwStyle = WS_POPUPWINDOW;
  hwnd = CreateWindowEx(0,
      RegWndClass(ClassName, CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
          LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_APPWORKSPACE),
          LoadIcon(NULL, IDI_APPLICATION), TrayProcWinMain), AppName, dwStyle,
      CW_USEDEFAULT, CW_USEDEFAULT, 350, 200, NULL, NULL, hInstance, NULL);
  ShowWindow(hwnd, SW_SHOWNORMAL | SW_MINIMIZE);
  ShowWindow(hwnd, SW_SHOWNORMAL);
  UpdateWindow(hwnd);
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}
static int test_sdk()
{
  WNDCLASSEX wc = {0};
  MSG msg;
  HWND hwnd;
  HINSTANCE hInstance = NULL;
  MarkerWnd mw[1] = {0};
  int dwStyle, cx = 0, cy = 0;
  int dwExStyle;
  dwExStyle = 0;
  dwStyle = WS_OVERLAPPEDWINDOW;
  dwExStyle = WS_EX_TOOLWINDOW | 0x00080000;
  dwStyle = WS_POPUP;
  //sys_init(g_sys);
  _chdir("E:/code/c/tab/MultiPaneCtrl_src/res/Markers A/");
  mw->m_ImageList = LoadImageList("TabsPane.bmp", 3, 1, RGB(0, 0, 0), &cx, &cy);
  hwnd = CreateWindowEx(0,
      RegWndClass(ClassName, CS_DBLCLKS, LoadCursor(NULL, IDC_ARROW), NULL, NULL, MarkerWndProc), "", dwStyle,
      CW_USEDEFAULT, CW_USEDEFAULT, cx, cy, NULL, NULL, NULL, mw);
  SetLayered(hwnd, 1);
  ShowWindow(hwnd, SW_SHOWNORMAL | SW_MINIMIZE);
  ShowWindow(hwnd, SW_SHOWNORMAL);
  UpdateWindow(hwnd);
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}

