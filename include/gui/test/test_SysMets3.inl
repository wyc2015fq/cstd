/*----------------------------------------------------
 SYSMETS3.C -- System Metrics Display Program No. 3
 (c) Charles Petzold, 1998
 ----------------------------------------------------*/
#define WINVER 0x0500
#include <windows.h>
/*-----------------------------------------------
 SYSMETS.H -- System metrics display structure
 -----------------------------------------------*/
#define NUMLINES ((int) (sizeof sysmetrics / sizeof sysmetrics [0]))
struct {
  int iIndex;
  TCHAR* szLabel;
  TCHAR* szDesc;
}
sysmetrics [] = {
  SM_CXSCREEN, TEXT("SM_CXSCREEN"), TEXT("Screen width in pixels"),
  SM_CYSCREEN, TEXT("SM_CYSCREEN"), TEXT("Screen height in pixels"),
  SM_CXVSCROLL, TEXT("SM_CXVSCROLL"), TEXT("Vertical scroll width"),
  SM_CYHSCROLL, TEXT("SM_CYHSCROLL"), TEXT("Horizontal scroll height"),
  SM_CYCAPTION, TEXT("SM_CYCAPTION"), TEXT("Caption bar height"),
  SM_CXBORDER, TEXT("SM_CXBORDER"), TEXT("Window border width"),
  SM_CYBORDER, TEXT("SM_CYBORDER"), TEXT("Window border height"),
  SM_CXFIXEDFRAME, TEXT("SM_CXFIXEDFRAME"), TEXT("Dialog window frame width"),
  SM_CYFIXEDFRAME, TEXT("SM_CYFIXEDFRAME"), TEXT("Dialog window frame height"),
  SM_CYVTHUMB, TEXT("SM_CYVTHUMB"), TEXT("Vertical scroll thumb height"),
  SM_CXHTHUMB, TEXT("SM_CXHTHUMB"), TEXT("Horizontal scroll thumb width"),
  SM_CXICON, TEXT("SM_CXICON"), TEXT("Icon width"),
  SM_CYICON, TEXT("SM_CYICON"), TEXT("Icon height"),
  SM_CXCURSOR, TEXT("SM_CXCURSOR"), TEXT("Cursor width"),
  SM_CYCURSOR, TEXT("SM_CYCURSOR"), TEXT("Cursor height"),
  SM_CYMENU, TEXT("SM_CYMENU"), TEXT("Menu bar height"),
  SM_CXFULLSCREEN, TEXT("SM_CXFULLSCREEN"), TEXT("Full screen client area width"),
  SM_CYFULLSCREEN, TEXT("SM_CYFULLSCREEN"), TEXT("Full screen client area height"),
  SM_CYKANJIWINDOW, TEXT("SM_CYKANJIWINDOW"), TEXT("Kanji window height"),
  SM_MOUSEPRESENT, TEXT("SM_MOUSEPRESENT"), TEXT("Mouse present flag"),
  SM_CYVSCROLL, TEXT("SM_CYVSCROLL"), TEXT("Vertical scroll arrow height"),
  SM_CXHSCROLL, TEXT("SM_CXHSCROLL"), TEXT("Horizontal scroll arrow width"),
  SM_DEBUG, TEXT("SM_DEBUG"), TEXT("Debug version flag"),
  SM_SWAPBUTTON, TEXT("SM_SWAPBUTTON"), TEXT("Mouse buttons swapped flag"),
  SM_CXMIN, TEXT("SM_CXMIN"), TEXT("Minimum window width"),
  SM_CYMIN, TEXT("SM_CYMIN"), TEXT("Minimum window height"),
  SM_CXSIZE, TEXT("SM_CXSIZE"), TEXT("Min/Max/Close button width"),
  SM_CYSIZE, TEXT("SM_CYSIZE"), TEXT("Min/Max/Close button height"),
  SM_CXSIZEFRAME, TEXT("SM_CXSIZEFRAME"), TEXT("Window sizing frame width"),
  SM_CYSIZEFRAME, TEXT("SM_CYSIZEFRAME"), TEXT("Window sizing frame height"),
  SM_CXMINTRACK, TEXT("SM_CXMINTRACK"), TEXT("Minimum window tracking width"),
  SM_CYMINTRACK, TEXT("SM_CYMINTRACK"), TEXT("Minimum window tracking height"),
  SM_CXDOUBLECLK, TEXT("SM_CXDOUBLECLK"), TEXT("Double click x tolerance"),
  SM_CYDOUBLECLK, TEXT("SM_CYDOUBLECLK"), TEXT("Double click y tolerance"),
  SM_CXICONSPACING, TEXT("SM_CXICONSPACING"), TEXT("Horizontal icon spacing"),
  SM_CYICONSPACING, TEXT("SM_CYICONSPACING"), TEXT("Vertical icon spacing"),
  SM_MENUDROPALIGNMENT, TEXT("SM_MENUDROPALIGNMENT"), TEXT("Left or right menu drop"),
  SM_PENWINDOWS, TEXT("SM_PENWINDOWS"), TEXT("Pen extensions installed"),
  SM_DBCSENABLED, TEXT("SM_DBCSENABLED"), TEXT("Double-Byte Char Set enabled"),
  SM_CMOUSEBUTTONS, TEXT("SM_CMOUSEBUTTONS"), TEXT("Number of mouse buttons"),
  SM_SECURE, TEXT("SM_SECURE"), TEXT("Security present flag"),
  SM_CXEDGE, TEXT("SM_CXEDGE"), TEXT("3-D border width"),
  SM_CYEDGE, TEXT("SM_CYEDGE"), TEXT("3-D border height"),
  SM_CXMINSPACING, TEXT("SM_CXMINSPACING"), TEXT("Minimized window spacing width"),
  SM_CYMINSPACING, TEXT("SM_CYMINSPACING"), TEXT("Minimized window spacing height"),
  SM_CXSMICON, TEXT("SM_CXSMICON"), TEXT("Small icon width"),
  SM_CYSMICON, TEXT("SM_CYSMICON"), TEXT("Small icon height"),
  SM_CYSMCAPTION, TEXT("SM_CYSMCAPTION"), TEXT("Small caption height"),
  SM_CXSMSIZE, TEXT("SM_CXSMSIZE"), TEXT("Small caption button width"),
  SM_CYSMSIZE, TEXT("SM_CYSMSIZE"), TEXT("Small caption button height"),
  SM_CXMENUSIZE, TEXT("SM_CXMENUSIZE"), TEXT("Menu bar button width"),
  SM_CYMENUSIZE, TEXT("SM_CYMENUSIZE"), TEXT("Menu bar button height"),
  SM_ARRANGE, TEXT("SM_ARRANGE"), TEXT("How minimized windows arranged"),
  SM_CXMINIMIZED, TEXT("SM_CXMINIMIZED"), TEXT("Minimized window width"),
  SM_CYMINIMIZED, TEXT("SM_CYMINIMIZED"), TEXT("Minimized window height"),
  SM_CXMAXTRACK, TEXT("SM_CXMAXTRACK"), TEXT("Maximum dragable width"),
  SM_CYMAXTRACK, TEXT("SM_CYMAXTRACK"), TEXT("Maximum dragable height"),
  SM_CXMAXIMIZED, TEXT("SM_CXMAXIMIZED"), TEXT("Width of maximized window"),
  SM_CYMAXIMIZED, TEXT("SM_CYMAXIMIZED"), TEXT("Height of maximized window"),
  SM_NETWORK, TEXT("SM_NETWORK"), TEXT("Network present flag"),
  SM_CLEANBOOT, TEXT("SM_CLEANBOOT"), TEXT("How system was booted"),
  SM_CXDRAG, TEXT("SM_CXDRAG"), TEXT("Avoid drag x tolerance"),
  SM_CYDRAG, TEXT("SM_CYDRAG"), TEXT("Avoid drag y tolerance"),
  SM_SHOWSOUNDS, TEXT("SM_SHOWSOUNDS"), TEXT("Present sounds visually"),
  SM_CXMENUCHECK, TEXT("SM_CXMENUCHECK"), TEXT("Menu check-mark width"),
  SM_CYMENUCHECK, TEXT("SM_CYMENUCHECK"), TEXT("Menu check-mark height"),
  SM_SLOWMACHINE, TEXT("SM_SLOWMACHINE"), TEXT("Slow processor flag"),
  SM_MIDEASTENABLED, TEXT("SM_MIDEASTENABLED"), TEXT("Hebrew and Arabic enabled flag"),
  SM_MOUSEWHEELPRESENT, TEXT("SM_MOUSEWHEELPRESENT"), TEXT("Mouse wheel present flag"),
  SM_XVIRTUALSCREEN, TEXT("SM_XVIRTUALSCREEN"), TEXT("Virtual screen x origin"),
  SM_YVIRTUALSCREEN, TEXT("SM_YVIRTUALSCREEN"), TEXT("Virtual screen y origin"),
  SM_CXVIRTUALSCREEN, TEXT("SM_CXVIRTUALSCREEN"), TEXT("Virtual screen width"),
  SM_CYVIRTUALSCREEN, TEXT("SM_CYVIRTUALSCREEN"), TEXT("Virtual screen height"),
  SM_CMONITORS, TEXT("SM_CMONITORS"), TEXT("Number of monitors"),
  SM_SAMEDISPLAYFORMAT, TEXT("SM_SAMEDISPLAYFORMAT"), TEXT("Same color format flag")
};
LRESULT CALLBACK test_SysMets3_WndProc(HWND, UINT, WPARAM, LPARAM);
int test_SysMets3()
{
  HINSTANCE hInstance = 0;
  int iCmdShow = 1;
  static TCHAR szAppName[] = TEXT("SysMets3");
  HWND hwnd;
  MSG msg;
  WNDCLASS wndclass;
  wndclass.style = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc = test_SysMets3_WndProc;
  wndclass.cbClsExtra = 0;
  wndclass.cbWndExtra = 0;
  wndclass.hInstance = hInstance;
  wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
  wndclass.lpszMenuName = NULL;
  wndclass.lpszClassName = szAppName;
  if (!RegisterClass(&wndclass)) {
    MessageBox(NULL, TEXT("Program requires Windows NT!"),
        szAppName, MB_ICONERROR);
    return 0;
  }
  hwnd = CreateWindow(szAppName, TEXT("Get System Metrics No. 3"),
      WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
      CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT,
      NULL, NULL, hInstance, NULL);
  ShowWindow(hwnd, iCmdShow);
  UpdateWindow(hwnd);
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}
LRESULT CALLBACK test_SysMets3_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  static int cxChar, cxCaps, cyChar, cxClient, cyClient, iMaxWidth;
  static int iDeltaPerLine, iAccumDelta ; // for mouse wheel logic
  HDC hdc;
  int i, x, y, iVertPos, iHorzPos, iPaintBeg, iPaintEnd;
  PAINTSTRUCT ps;
  SCROLLINFO si;
  TCHAR szBuffer[10];
  TEXTMETRIC tm;
  ULONG ulScrollLines ; // for mouse wheel logic
  switch (message) {
  case WM_CREATE:
    hdc = GetDC(hwnd);
    GetTextMetrics(hdc, &tm);
    cxChar = tm.tmAveCharWidth;
    cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
    cyChar = tm.tmHeight + tm.tmExternalLeading;
    ReleaseDC(hwnd, hdc);
    // Save the width of the three columns
    iMaxWidth = 40 * cxChar + 22 * cxCaps;
  case WM_SETTINGCHANGE:
    SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &ulScrollLines, 0) ;
    // ulScrollLines usually equals 3 or 0 (for no scrolling)
    // WHEEL_DELTA equals 120, so iDeltaPerLine will be 40
    if (ulScrollLines) {
      iDeltaPerLine = WHEEL_DELTA / ulScrollLines ;
    }
    else {
      iDeltaPerLine = 0 ;
    }
    return 0 ;
  case WM_SIZE:
    cxClient = LOWORD(lParam);
    cyClient = HIWORD(lParam);
    // Set vertical scroll bar range and page size
    si.cbSize = sizeof(si);
    si.fMask = SIF_RANGE | SIF_PAGE;
    si.nMin = 0;
    si.nMax = NUMLINES - 1;
    si.nPage = cyClient / cyChar;
    SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
    // Set horizontal scroll bar range and page size
    si.cbSize = sizeof(si);
    si.fMask = SIF_RANGE | SIF_PAGE;
    si.nMin = 0;
    si.nMax = iMaxWidth / cxChar;
    si.nPage = cxClient / cxChar;
    SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
    return 0;
  case WM_VSCROLL:
    // Get all the vertial scroll bar information
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    GetScrollInfo(hwnd, SB_VERT, &si);
    // Save the position for comparison later on
    iVertPos = si.nPos;
    switch (LOWORD(wParam)) {
    case SB_TOP:
      si.nPos = si.nMin;
      break;
    case SB_BOTTOM:
      si.nPos = si.nMax;
      break;
    case SB_LINEUP:
      si.nPos -= 1;
      break;
    case SB_LINEDOWN:
      si.nPos += 1;
      break;
    case SB_PAGEUP:
      si.nPos -= si.nPage;
      break;
    case SB_PAGEDOWN:
      si.nPos += si.nPage;
      break;
    case SB_THUMBTRACK:
      si.nPos = si.nTrackPos;
      break;
    default:
      break;
    }
    // Set the position and then retrieve it. Due to adjustments
    // by Windows it may not be the same as the value set.
    si.fMask = SIF_POS;
    SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
    GetScrollInfo(hwnd, SB_VERT, &si);
    // If the position has changed, scroll the window and update it
    if (si.nPos != iVertPos) {
      ScrollWindow(hwnd, 0, cyChar * (iVertPos - si.nPos), NULL, NULL);
      UpdateWindow(hwnd);
    }
    return 0;
  case WM_HSCROLL:
    // Get all the vertial scroll bar information
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    // Save the position for comparison later on
    GetScrollInfo(hwnd, SB_HORZ, &si);
    iHorzPos = si.nPos;
    switch (LOWORD(wParam)) {
    case SB_LINELEFT:
      si.nPos -= 1;
      break;
    case SB_LINERIGHT:
      si.nPos += 1;
      break;
    case SB_PAGELEFT:
      si.nPos -= si.nPage;
      break;
    case SB_PAGERIGHT:
      si.nPos += si.nPage;
      break;
    case SB_THUMBPOSITION:
      si.nPos = si.nTrackPos;
    case SB_THUMBTRACK:
      si.nPos = si.nTrackPos;
      break;
    default :
      break;
    }
    // Set the position and then retrieve it. Due to adjustments
    // by Windows it may not be the same as the value set.
    si.fMask = SIF_POS;
    SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
    GetScrollInfo(hwnd, SB_HORZ, &si);
    // If the position has changed, scroll the window
    printf("si.nPos = %d iHorzPos = %d LOWORD(wParam) = %d\n", si.nPos, iHorzPos, LOWORD(wParam));
    if (si.nPos != iHorzPos) {
      ScrollWindow(hwnd, cxChar * (iHorzPos - si.nPos), 0, NULL, NULL);
      UpdateWindow(hwnd);
    }
    return 0;
  case WM_KEYDOWN:
    switch (wParam) {
    case VK_HOME:
      SendMessage(hwnd, WM_VSCROLL, SB_TOP, 0) ;
      break ;
    case VK_END:
      SendMessage(hwnd, WM_VSCROLL, SB_BOTTOM, 0) ;
      break ;
    case VK_PRIOR:
      SendMessage(hwnd, WM_VSCROLL, SB_PAGEUP, 0) ;
      break ;
    case VK_NEXT:
      SendMessage(hwnd, WM_VSCROLL, SB_PAGEDOWN, 0) ;
      break ;
    case VK_UP:
      SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0) ;
      break ;
    case VK_DOWN:
      SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0) ;
      break ;
    case VK_LEFT:
      SendMessage(hwnd, WM_HSCROLL, SB_PAGEUP, 0) ;
      break ;
    case VK_RIGHT:
      SendMessage(hwnd, WM_HSCROLL, SB_PAGEDOWN, 0) ;
      break ;
    }
    return 0 ;
  case WM_MOUSEWHEEL:
    if (iDeltaPerLine == 0) {
      break ;
    }
    iAccumDelta += (short) HIWORD(wParam) ;  // 120 or -120
    while (iAccumDelta >= iDeltaPerLine) {
      SendMessage(hwnd, wParam & MK_CONTROL ? WM_HSCROLL : WM_VSCROLL, SB_LINEUP, 0) ;
      iAccumDelta -= iDeltaPerLine ;
    }
    while (iAccumDelta <= -iDeltaPerLine) {
      SendMessage(hwnd, wParam & MK_CONTROL ? WM_HSCROLL : WM_VSCROLL, SB_LINEDOWN, 0) ;
      iAccumDelta += iDeltaPerLine ;
    }
    return 0 ;
  case WM_PAINT : {
    RECT rect = {0};
    RECT rect2 = {0};
    hdc = BeginPaint(hwnd, &ps);
    // Get vertical scroll bar position
    GetClientRect(hwnd, &rect);
    GetWindowRect(hwnd, &rect2);
    //printf("WM_PAINT %d %d\n", (rect2.right - rect2.left) - (rect.right - rect.left), (rect2.bottom - rect2.top) - (rect.bottom - rect.top));
    printf("WM_PAINT %d %d %d %d\n", ps.rcPaint.left, ps.rcPaint.right, ps.rcPaint.top, ps.rcPaint.bottom);
    si.cbSize = sizeof(si);
    si.fMask = SIF_POS;
    GetScrollInfo(hwnd, SB_VERT, &si);
    iVertPos = si.nPos;
    // Get horizontal scroll bar position
    GetScrollInfo(hwnd, SB_HORZ, &si);
    iHorzPos = si.nPos;
    // Find painting limits
    iPaintBeg = max(0, iVertPos + ps.rcPaint.top / cyChar);
    iPaintEnd = min(NUMLINES - 1,
        iVertPos + ps.rcPaint.bottom / cyChar);
    for (i = iPaintBeg; i <= iPaintEnd; i++) {
      x = cxChar * (1 - iHorzPos);
      y = cyChar * (i - iVertPos);
      TextOut(hdc, x, y, sysmetrics[i].szLabel, lstrlen(sysmetrics[i].szLabel));
      TextOut(hdc, x + 22 * cxCaps, y, sysmetrics[i].szDesc, lstrlen(sysmetrics[i].szDesc));
      SetTextAlign(hdc, TA_RIGHT | TA_TOP);
      TextOut(hdc, x + 22 * cxCaps + 40 * cxChar, y, szBuffer, wsprintf(szBuffer, TEXT("%5d"),
          GetSystemMetrics(sysmetrics[i].iIndex)));
      SetTextAlign(hdc, TA_LEFT | TA_TOP);
    }
    EndPaint(hwnd, &ps);
  }
  return 0;
  case WM_DESTROY :
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hwnd, message, wParam, lParam);
}

