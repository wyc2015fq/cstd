
#include <assert.h>


// 全局变量:
HINSTANCE hInst = 0;              // 当前实例

HDC m_hDcBackground = NULL;
HBITMAP m_hbmpBackground = NULL;
HDC m_hDcPaint = NULL;

// 此代码模块中包含的函数的前向声明:
ATOM        MyRegisterClass(HINSTANCE hInstance);
BOOL        InitInstance(HINSTANCE, int);
LRESULT CALLBACK  WndProc(HWND, UINT, WPARAM, LPARAM);

BOOL RegisterWindowClass(WNDPROC fWndProc, HINSTANCE hInstance, LPCTSTR szClassName)
{
  WNDCLASSEX wce = {0};
  wce.cbSize = sizeof(wce);
  wce.style = CS_HREDRAW | CS_VREDRAW;
  wce.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
  wce.lpfnWndProc = fWndProc;
  wce.cbClsExtra = 0;
  wce.cbWndExtra = 0;
  wce.hInstance = hInstance;
  wce.hIcon = NULL;
  wce.hCursor = LoadCursor(NULL, IDC_ARROW);
  wce.hbrBackground = (HBRUSH)(6); //(HBRUSH)(COLOR_WINDOW+1);
  wce.lpszMenuName = NULL;
  wce.lpszClassName = szClassName;
  wce.hIconSm = NULL;
  ATOM nAtom = RegisterClassEx(&wce);

  if (nAtom == 0) {
    return FALSE;
  }

  return TRUE;

}

int test_win2()
{
  MSG msg;
  HWND hWnd;

  if (!RegisterWindowClass(WndProc, hInst, "resize")) {
    assert(L"注册窗口失败");
  }

  hWnd = CreateWindowEx(WS_EX_STATICEDGE | WS_EX_APPWINDOW, "resize",
      _T(""), WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
      600, 300, NULL, NULL, (HINSTANCE)GetModuleHandle(NULL), NULL);

  if (!hWnd) {
    return FALSE;
  }

  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  // 主消息循环:
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return (int) msg.wParam;
}

void Paint(HWND m_hWnd)
{
  DWORD dwExStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);

  if ((dwExStyle & WS_EX_LAYERED) != 0x80000) {
    SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle ^ WS_EX_LAYERED);
  }

  RECT rcClient = { 0 };
  ::GetClientRect(m_hWnd, &rcClient);

  PAINTSTRUCT ps = { 0 };
  ::BeginPaint(m_hWnd, &ps);

  if (m_hDcBackground != NULL) {
    ::DeleteDC(m_hDcBackground);
  }

  if (m_hbmpBackground != NULL) {
    ::DeleteObject(m_hbmpBackground);
  }

  m_hDcBackground = NULL;
  m_hbmpBackground = NULL;
  PVOID pvBits = NULL;
  SIZE sizeWindow;

  if (m_hbmpBackground == NULL) {
    m_hDcBackground = ::CreateCompatibleDC(m_hDcPaint);

    sizeWindow.w = rcClient.right - rcClient.left;
    sizeWindow.h = rcClient.bottom - rcClient.top;

    BITMAPINFOHEADER stBmpInfoHeader = { 0 };
    int nBytesPerLine = ((sizeWindow.w * 32 + 31) & (~31)) >> 3;
    stBmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    stBmpInfoHeader.biWidth = sizeWindow.w;
    stBmpInfoHeader.biHeight = sizeWindow.h;
    stBmpInfoHeader.biPlanes = 1;
    stBmpInfoHeader.biBitCount = 32;
    stBmpInfoHeader.biCompression = BI_RGB;
    stBmpInfoHeader.biClrUsed = 0;
    stBmpInfoHeader.biSizeImage = nBytesPerLine * sizeWindow.h;
    m_hbmpBackground = ::CreateDIBSection(NULL, (PBITMAPINFO)&stBmpInfoHeader, DIB_RGB_COLORS, &pvBits, NULL, 0);

    assert(m_hDcBackground);
    assert(m_hbmpBackground);
  }

  ::SelectObject(m_hDcBackground, m_hbmpBackground);

  RECT rcWnd = {0};
  ::GetWindowRect(m_hWnd, &rcWnd);
  POINT pt = {rcWnd.left, rcWnd.top};

  FillSolidRect(m_hDcBackground, &rcWnd, RGB(255, 255, 255));
  SIZE szWindow = {rcClient.right - rcClient.left, rcClient.bottom - rcClient.top};
  POINT ptSrc = {0, 0};
  BLENDFUNCTION blendPixelFunction = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
  memset(pvBits, 255, sizeWindow.w * sizeWindow.h * 4);
  ::UpdateLayeredWindow(m_hWnd, NULL, &pt, &szWindow, m_hDcBackground, &ptSrc, 0, &blendPixelFunction, ULW_ALPHA);
  ::EndPaint(m_hWnd, &ps);
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  int wmId, wmEvent;
  PAINTSTRUCT ps;
  HDC hdc;

  switch (message) {
  case WM_CREATE: {
    LONG styleValue = ::GetWindowLong(hWnd, GWL_STYLE);
    styleValue &= ~WS_CAPTION;
    ::SetWindowLong(hWnd, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);


    m_hDcPaint = GetDC(hWnd);
    SendMessage(hWnd, WM_PAINT, NULL, NULL);
    //在CreateWindowEx的第一个参数使用WS_EX_APPWINDOW时，系统会自动发送WM_PAINT消息，此时会出现白色背景闪烁问题
    //手动发送WM_PAINT消息，能解决这个问题
    return 0;
  }
  break;

  case WM_PAINT:
    Paint(hWnd);
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    DeleteDC(m_hDcPaint);
    break;

  case WM_NCLBUTTONDBLCLK:
  case WM_LBUTTONDBLCLK:
    if (1) {
      int asdf = 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
    break;

  case WM_NCHITTEST:
    ////这个消息的响应方式非常奇怪，只能由自己响应，即便是在子类中有响应函数，也不能让子类来响应，如果由子类响应会出现移动时跟鼠标移动位置不符合的问题，
    //不知为何，这也就是为什么在DUILIB中，必须在用户产生的窗体类中响应WM_NCHITTEST；而不能由PaintManagerUI来响应；
    POINT pt;
    pt.x = GET_X_LPARAM(lParam);
    pt.y = GET_Y_LPARAM(lParam);
    ::ScreenToClient(hWnd, &pt);

    RECT rcClient;
    ::GetClientRect(hWnd, &rcClient);

    if (pt.x < rcClient.left + 20 && pt.y < rcClient.top + 20) { //左上角,判断是不是在左上角，就是看当前坐标是不是即在左边拖动的范围内，又在上边拖动的范围内，其它角判断方法类似
      return HTTOPLEFT;
    }
    else if (pt.x > rcClient.right - 20 && pt.y < rcClient.top + 20) { //右上角
      return HTTOPRIGHT;
    }
    else if (pt.x < rcClient.left + 20 && pt.y > rcClient.bottom - 20) { //左下角
      return HTBOTTOMLEFT;
    }
    else if (pt.x > rcClient.right - 20 && pt.y > rcClient.bottom - 20) { //右下角
      return HTBOTTOMRIGHT;
    }
    else if (pt.x < rcClient.left + 20) {
      return HTLEFT;
    }
    else if (pt.x > rcClient.right - 20) {
      return HTRIGHT;
    }
    else if (pt.y < rcClient.top + 20) {
      return HTTOP;
    }

    if (pt.y > rcClient.bottom - 20) {
      return HTBOTTOM;          //以上这四个是上、下、左、右四个边
    }
    else {
      return HTCAPTION;
    }

    break;

  case WM_SIZE: { //要让窗体能够随着缩放改变，要响应WM-SIZE消息
    RECT rcClient = { 0 };
    ::GetClientRect(hWnd, &rcClient);
    InvalidateRect(hWnd, &rcClient, FALSE);
  }
  break;

  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }

  return 0;
}
