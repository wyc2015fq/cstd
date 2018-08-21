/*----------------------------------------
 BEEPER2.C -- Timer Demo Program No. 2
 (c) Charles Petzold, 1998
 ----------------------------------------*/
#include <windows.h>
#define ID_TIMER 1
LRESULT CALLBACK test_Beeper2_WndProc(HWND, UINT, WPARAM, LPARAM) ;
VOID CALLBACK test_Beeper2_TimerProc(HWND, UINT, UINT, DWORD) ;
int test_Beeper2()
{
  HINSTANCE hInstance = 0;
  int iCmdShow = 1;
  static TCHAR szAppName[] = TEXT("Beeper2") ;
  HWND hwnd ;
  MSG msg ;
  WNDCLASS wndclass ;
  wndclass.style = CS_HREDRAW | CS_VREDRAW ;
  wndclass.lpfnWndProc = test_Beeper2_WndProc ;
  wndclass.cbClsExtra = 0 ;
  wndclass.cbWndExtra = 0 ;
  wndclass.hInstance = hInstance ;
  wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION) ;
  wndclass.hCursor = LoadCursor(NULL, IDC_ARROW) ;
  wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH) ;
  wndclass.lpszMenuName = NULL ;
  wndclass.lpszClassName = szAppName ;
  if (!RegisterClass(&wndclass)) {
    MessageBox(NULL, TEXT("Program requires Windows NT!"),
        szAppName, MB_ICONERROR) ;
    return 0 ;
  }
  hwnd = CreateWindow(szAppName, TEXT("Beeper2 Timer Demo"),
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT,
      NULL, NULL, hInstance, NULL) ;
  ShowWindow(hwnd, iCmdShow) ;
  UpdateWindow(hwnd) ;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg) ;
    DispatchMessage(&msg) ;
  }
  return msg.wParam ;
}
LRESULT CALLBACK test_Beeper2_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message) {
  case WM_CREATE:
    SetTimer(hwnd, ID_TIMER, 1000, test_Beeper2_TimerProc) ;
    return 0 ;
  case WM_DESTROY:
    KillTimer(hwnd, ID_TIMER) ;
    PostQuitMessage(0) ;
    return 0 ;
  }
  return DefWindowProc(hwnd, message, wParam, lParam) ;
}
VOID CALLBACK test_Beeper2_TimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
  static BOOL fFlipFlop = FALSE ;
  HBRUSH hBrush ;
  HDC hdc ;
  RECT rc ;
  MessageBeep(-1) ;
  fFlipFlop = !fFlipFlop ;
  GetClientRect(hwnd, &rc) ;
  hdc = GetDC(hwnd) ;
  hBrush = CreateSolidBrush(fFlipFlop ? RGB(255, 0, 0) : RGB(0, 0, 255)) ;
  FillRect(hdc, &rc, hBrush) ;
  ReleaseDC(hwnd, hdc) ;
  DeleteObject(hBrush) ;
}

