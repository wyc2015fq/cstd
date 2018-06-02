#undef WINVER
#undef _WIN32_WINNT
#undef _WIN32_IE
#define WINVER 0x0500
#define _WIN32_WINNT 0x0501
#define _WIN32_IE 0x0501
#include <windows.h>
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#define CLEARTYPE_QUALITY 5
COLORREF WINAPI SetDCBrushColor(IN HDC, IN COLORREF);
COLORREF WINAPI SetDCPenColor(IN HDC, IN COLORREF);
#define DC_BRUSH 18
#define DC_PEN 19
#endif // WM_MOUSEWHEEL
typedef struct {
  HWND g_hWnd;
  LRESULT(*DrvWndProc)(ImGuiHost* host, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
} imui_win32_t;
//#include "sys/TraceMsg.inl"
int imuiLoop(int delay)
{
  int i, time0 = GetTickCount();
  GETGUIIO1();
  static MSG msg[1] = {0};
  io->lastMsg = MSG_NOP;
  sys_utime(&io->Time);
  for (i = 0; i < 5; ++i) {
    if (!io->MouseDown[i]) {
      io->MouseDownID[i] = NULL;
    }
  }
  memset(msg, 0, sizeof(*msg));
  if (delay > 0) {
    while (abs((int)(GetTickCount() - time0)) < delay) {
      if (PeekMessage(msg, 0, 0, 0, PM_REMOVE) == FALSE) {
        Sleep(1);
      }
      else {
        TranslateMessage(msg);
        DispatchMessage(msg);
        break;
      }
    }
  }
  else {
    GetMessage(msg, 0, 0, 0);
    TranslateMessage(msg);
    DispatchMessage(msg);
  }
  //TraceMsg(msg->hwnd, msg->message, msg->lParam, msg->wParam);
  //if (!g->g_FontTexture) { imuiImplDX9_CreateDeviceObjects(); }
  {
    // Setup display size (every frame to accommodate for window resizing)
    static INT64 g_Time = 0, g_TicksPerSecond = 0;
    INT64 current_time;
    if (!g_TicksPerSecond) {
      // Setup ImGui binding
      if (!QueryPerformanceFrequency((LARGE_INTEGER*)&g_TicksPerSecond)) {
        return false;
      }
      if (!QueryPerformanceCounter((LARGE_INTEGER*)&g_Time)) {
        return false;
      }
    }
    // Setup time step
    QueryPerformanceCounter((LARGE_INTEGER*)&current_time);
    io->DeltaTime = (float)(current_time - g_Time) / g_TicksPerSecond;
    g_Time = current_time;
    for (host = io->HeadHost; host; host = host->next) {
      imui_win32_t* s = (imui_win32_t*)host->Window;
      GetClientRect(s->g_hWnd, (RECT*)&host->rootWindow->rect);
      //io->DisplaySize = fVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));
    }
    // Read keyboard modifiers inputs
    io->KeyCtrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
    io->KeyShift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
    io->KeyAlt = (GetKeyState(VK_MENU) & 0x8000) != 0;
    io->KeySuper = false;
    // io->KeysDown : filled by WM_KEYDOWN/WM_KEYUP events
    // io->MousePos : filled by WM_MOUSEMOVE events
    // io->MouseDown : filled by WM_*BUTTON* events
    // io->MouseWheel : filled by WM_MOUSEWHEEL events
    // Hide OS mouse cursor if ImGui is drawing it
    //SetCursor(io->MouseDrawCursor ? NULL : LoadCursor(NULL, IDC_ARROW));
    // Start the frame
    //imuiNewFrame();
    if (msg->hwnd) {
      io->HotHost = (ImGuiHost*)GetWindowLong(msg->hwnd, GWL_USERDATA);
    }
    else {
      io->HotHost = NULL;
    }
  }
  return msg->message != WM_QUIT;
}
IM_STATIC LRESULT WINAPI imuiWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  GETGUIIO2();
  host = (ImGuiHost*)GetWindowLong(hWnd, GWL_USERDATA);
  io->lastMsg = MSG_NOP;
  switch (msg) {
  case WM_LBUTTONDOWN:
    io->MouseDown[0] = true;
    io->lastMouseDownPos = io->MousePos;
    io->lastMsg = MSG_LBUTTONDOWN;
    io->MouseDoubleClicked[0] = sys_utime(&io->MouseClickedTime[0]) < io->MouseDoubleClickTime;
    if (io->MouseDoubleClicked[0]) {
      printf("io->MouseDoubleClicked[0]\n");
    }
    SetCapture(hWnd);
    return true;
  case WM_LBUTTONUP:
    io->MouseDoubleClicked[0] = false;
    io->MouseDown[0] = false;
    ReleaseCapture();
    io->lastMsg = MSG_LBUTTONUP;
    return true;
  case WM_RBUTTONDOWN:
    io->MouseDown[1] = true;
    io->lastMsg = MSG_RBUTTONDOWN;
    return true;
  case WM_RBUTTONUP:
    io->MouseDown[1] = false;
    io->lastMsg = MSG_RBUTTONUP;
    return true;
  case WM_MBUTTONDOWN:
    io->MouseDown[2] = true;
    io->lastMsg = MSG_MBUTTONDOWN;
    return true;
  case WM_MBUTTONUP:
    io->MouseDown[2] = false;
    io->lastMsg = MSG_MBUTTONUP;
    return true;
  case WM_MOUSEWHEEL:
    io->lastMsg = MSG_MOUSEWHEEL;
    io->MouseWheel = GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
    return true;
  case WM_MOUSEMOVE:
    io->lastMsg = MSG_MOUSEMOVE;
    io->MousePos.x = (signed short)(lParam);
    io->MousePos.y = (signed short)(lParam >> 16);
    return true;
  case WM_KEYDOWN:
    io->lastMsg = MSG_KEYDOWN;
    io->lastChar = io->KeyMap[wParam & 511];
    if (wParam < 256) {
      io->KeysDown[wParam] = 1;
    }
    return true;
  case WM_KEYUP:
    io->lastMsg = MSG_KEYUP;
    if (wParam < 256) {
      io->KeysDown[wParam] = 0;
    }
    return true;
  case WM_CHAR:
    //io->KeyDownTime[0] = sys_utime(&io->KeyDownTime[0]) > io->KeyRepeatDelay;
    io->lastMsg = MSG_CHAR;
    io->lastChar = io->KeyMap[wParam & 511];
    // You can also use ToAscii()+GetKeyboardState() to retrieve characters.
    if (wParam > 0 && wParam < 0x10000) {
      ImGuiIO_AddInputCharacter(io, io->lastChar);
    }
    return true;
  case WM_SYSCOMMAND:
    if ((wParam & 0xfff0) == SC_KEYMENU) { // Disable ALT application menu
      return 0;
    }
    break;
  case WM_DESTROY:
    if (host) {
      host->DrvUnInit(host);
    }
    ImGuiBase_FreeAllCtx(host->rootWindow);
    free(host->Window);
    ImGuiHost_Del(host);
    memset(host, 0, sizeof(*host));
    PostQuitMessage(0);
    return 0;
  }
  if (host) {
    imui_win32_t* s = (imui_win32_t*)(host->Window);
    if (s && s->DrvWndProc && s->DrvWndProc(host, hWnd, msg, wParam, lParam)) {
      return true;
    }
  }
  return DefWindowProc(hWnd, msg, wParam, lParam);
}
#define IMGUIWIN32_CLASSNAME "ImGui Example"
WNDCLASSEX* getWndClass()
{
  static WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, imuiWndProc, 0L, 0L, NULL, NULL, NULL, NULL, NULL, IMGUIWIN32_CLASSNAME, NULL };
  if (!wc.hInstance) {
    wc.hInstance = GetModuleHandle(NULL);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  }
  return &wc;
}
int imuiUnInit()
{
  UnregisterClass(IMGUIWIN32_CLASSNAME, getWndClass()->hInstance);
  return 0;
}
#define OUTERRMSG() printf("%s\n", GetErrorMsg(0))
int ImGuiHost_Init(ImGuiHost* host, ImGuiDriverInit_t drv_init, const char* lpWindowName, int x, int y, int nWidth, int nHeight)
{
  int i;
  imui_win32_t* s = NULL;
  // Create application window
  static int inited = 0;
  HWND hWnd;
  WNDCLASSEX* pwc = getWndClass();
  if (!inited) {
    GETGUIIO1();
    LOGFONT lf;
    inited = 1;
    if (!RegisterClassEx(pwc)) {
      return 0;
    }
    imuiInit();
    SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0);
    strcpy(io->def_font_name, lf.lfFaceName);
    io->def_font_size = lf.lfHeight;
    for (i = 0; i < countof(io->KeyMap); ++i) {
      io->KeyMap[i] = i;
    }
    io->KeyMap[VK_TAB] = KEY_TAB;
    io->KeyMap[VK_LEFT] = KEY_LEFT;
    io->KeyMap[VK_RIGHT] = KEY_RIGHT;
    io->KeyMap[VK_UP] = KEY_UP;
    io->KeyMap[VK_DOWN] = KEY_DOWN;
    io->KeyMap[VK_PRIOR] = KEY_PAGEUP;
    io->KeyMap[VK_NEXT] = KEY_PAGEDOWN;
    io->KeyMap[VK_HOME] = KEY_HOME;
    io->KeyMap[VK_END] = KEY_END;
    io->KeyMap[VK_DELETE] = KEY_DELETE;
    io->KeyMap[VK_BACK] = KEY_BACK;
    io->KeyMap[VK_RETURN] = KEY_ENTER;
    io->KeyMap[VK_ESCAPE] = KEY_ESCAPE;
  }
  hWnd = CreateWindowA(IMGUIWIN32_CLASSNAME, lpWindowName, WS_OVERLAPPEDWINDOW, x, y, nWidth, nHeight, NULL, NULL, pwc->hInstance, NULL);
  if (!hWnd) {
    const char* ss = sys_errmsg(0);
    printf("err: %s\n", ss);
    return 0;
  }
  SetWindowLong(hWnd, GWL_USERDATA, (long)host);
  ShowWindow(hWnd, SW_SHOWDEFAULT);
  UpdateWindow(hWnd);
  myReAlloc(s, 1);
  host->Window = s;
  s->g_hWnd = hWnd;
  if (!drv_init(host)) {
  }
  ImGuiHost_Add(host);
  return 0;
}

