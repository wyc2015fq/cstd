/*------------------------------------------------------------
 HELLOWIN.C -- Displays "Hello, Windows 98!" in client area
 (c) Charles Petzold, 1998
 ------------------------------------------------------------*/

#include <windows.h>
#include <stdio.h>

//#include "ui/window.inl"
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message) {
  case WM_CREATE:
    InvalidateRect(hwnd, 0, 0);
    break;

  default:
    return DefWindowProc(hwnd, message, wParam, lParam);
    break;
  }

  return DefWindowProc(hwnd, message, wParam, lParam);
}

int test_test_sdk()
{
  HINSTANCE hInstance = 0;
  HINSTANCE hPrevInstance = 0;
  PSTR szCmdLine = 0;
  int iCmdShow = 1;
  static TCHAR szAppName[] = TEXT("HelloWin");
  HWND hwnd;
  MSG msg;
  WNDCLASS wndclass;

  wndclass.style = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc = WndProc;
  wndclass.cbClsExtra = 0;
  wndclass.cbWndExtra = 0;
  wndclass.hInstance = hInstance;
  wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
  wndclass.lpszMenuName = NULL;
  wndclass.lpszClassName = szAppName;

  if (!RegisterClass(&wndclass)) {
    MessageBox(NULL, TEXT("This program requires Windows NT!"),
        szAppName, MB_ICONERROR);
    return 0;
  }

  hwnd = CreateWindow(szAppName, TEXT("The Hello Program"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

  ShowWindow(hwnd, iCmdShow);
  UpdateWindow(hwnd);

  while (GetMessage(&msg, NULL, 0, 0)) {
    UINT message;
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    WPARAM wParam;
    LPARAM lParam;

    TranslateMessage(&msg);
    message = msg.message;
    wParam = msg.wParam;
    lParam = msg.lParam;

    if (hwnd == msg.hwnd) {
      switch (message) {
      case WM_CREATE:
        //PlaySound (TEXT ("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC);
        break;

      case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rect);
        printf("asdf\n");
        DrawText(hdc, TEXT("Hello, Windows 98!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        EndPaint(hwnd, &ps);
        break;

      case WM_DESTROY:
        PostQuitMessage(0);
        break;

      default:
        DefWindowProc(hwnd, message, wParam, lParam);
        break;
      }
    }
    else {
      DispatchMessage(&msg);
    }
  }

  return msg.wParam;
}


