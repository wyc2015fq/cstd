#include <Windows.h>
#include <tchar.h>
#include <time.h>
#include "Engine3D.inl"
#include "game.inl"
#define MAIN_WINDOW_CLASS_NAME _T("3DEngineMainWindowClass")
#define MAIN_WINDOW_WIDTH 800
#define MAIN_WINDOW_HEIGHT 600
#define FRAMES_PER_SECOND 100
#define NUMBER_LIGHTS 2
#define NUMBER_TEST_OBJECTS 500
HINSTANCE g_hInstance = NULL;
HWND g_hMainWindow = NULL;
BOOL g_bPaused = FALSE;
game2D g_engine3D;
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  int lResult = 0;
  // Process main window messages
  switch (uMsg) {
  case WM_SIZE: {
    int nWidth = LOWORD(lParam); // width of client area
    int nHeight = HIWORD(lParam); // height of client area
    g_engine3D.SetSize(nWidth, nHeight);
  }
  break;
  case WM_DESTROY: {
    // Exit application
    PostQuitMessage(0);
  }
  break;
  case WM_LBUTTONDOWN: {
    int x, y;
    x = LOWORD(lParam);
    y = HIWORD(lParam);
    for (int i = 0; i < g_engine3D.rs; i++) {
      if (g_engine3D.man[i].jisu == 0)  {
        g_engine3D.man[i].x0 = x;
        g_engine3D.man[i].y0 = y;
        g_engine3D.man[i].p = g_engine3D.man[i].m1 - 1;
        break;
      }
    }
  }
  break;
  case WM_KEYDOWN: {
    if (wParam == VK_ESCAPE) {
      // Destroy main window
      DestroyWindow(hWnd);
    }
    else if ((_TCHAR)wParam == '0') {
    }
    else if ((_TCHAR)wParam == '1') {
    }
    else if ((_TCHAR)wParam == '2') {
    }
    else if ((_TCHAR)wParam == '3') {
    }
    else if ((_TCHAR)wParam == '4') {
    }
    else if ((_TCHAR)wParam == '5') {
    }
    else if ((_TCHAR)wParam == '6') {
    }
    else if ((_TCHAR)wParam == 'B') {
    }
    else if ((_TCHAR)wParam == 'P') {
    }
    else if ((_TCHAR)wParam == VK_LEFT) {
    }
    else if ((_TCHAR)wParam == VK_RIGHT) {
    }
    else if ((_TCHAR)wParam == VK_UP) {
    }
    else if ((_TCHAR)wParam == VK_DOWN) {
    }
    else if ((_TCHAR)wParam == 'R') {
    }
  }
  break;
  case WM_MOUSEMOVE: {
  }
  break;
  default: {
    // Call main window default procedure
    lResult = DefWindowProc(hWnd, uMsg, wParam, lParam);
  }
  }
  return lResult;
}
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib ")
int Main(HWND hWnd)
{
  int fps = 30;
  int det = 1000 / 30;
  int t0, t1;
  MSG msg;
  g_engine3D.Init();
  g_engine3D.loadmap("./地图/game0.dat");
  char buf[256];
  int p = 0;
  _snprintf(buf, 256, "./图片/地面.BMP");
  Image32* img = loadbmpfile(buf, 32, 1);
  //cc=dir+"地面.BMP";
  _snprintf(buf, 256, "./图片/人/c%05d.bmp", p);
  Image32* ren = loadbmpfile(buf, 32, 1);
  //savebmpfile("asdf.bmp", im, 32, 24, 0);
  // Run main window message loop
  while (true) {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      // Test for quit message
      if (msg.message == WM_QUIT) {
        break;
      }
      // Translate any accelerator keys
      TranslateMessage(&msg);
      // Send message to window procedure
      DispatchMessage(&msg);
    }
    // Start frame
    // End frame
    HDC hDC = GetDC(hWnd);
    //g_engine3D.Draw_Image(0, 0, img->width, img->height, (_PIXEL*)(img+1), img->byte_width, -1);
    //g_engine3D.Draw_Image(0, 0, ren->width, ren->height, (_PIXEL*)(ren+1), ren->byte_width, -1);
    char szText[1024];
    sprintf(szText, "我我我我我");
    g_engine3D.Draw_Text(10, 10, szText, _RGB(255, 255, 0), 100, _T("宋体"), 32, 1, FALSE);
    g_engine3D.Draw_Frame(hDC);
    ReleaseDC(hWnd, hDC);
    // Draw frame
    while (1) {
      t1 = timeGetTime();
      if (t1 - t0 > det) {
        t0 = t1;
        break;
      }
      Sleep(10);
    }
  }
  g_engine3D.Shutdown();
  free(ren);
  free(img);
  return 0;
}
#include <direct.h>
int test_Engine3D()
{
  _chdir("D:\\code\\c\\游戏引擎\\code\\game(GDI)\\运行程序");
  HINSTANCE hInstance = 0;
  int iReturnValue = 0;
  // Format main window class
  WNDCLASSEX wce;
  memset(&wce, 0, sizeof(WNDCLASSEX));
  wce.cbSize = sizeof(WNDCLASSEX);
  wce.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
  wce.lpfnWndProc = (WNDPROC)MainWindowProc;
  wce.hInstance = hInstance;
  wce.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  wce.hCursor = LoadCursor(NULL, IDC_ARROW);
  wce.lpszClassName = MAIN_WINDOW_CLASS_NAME;
  // Calculate main window position
  int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
  int iScreenHeight = GetSystemMetrics(SM_CYSCREEN);
  int x = (iScreenWidth - MAIN_WINDOW_WIDTH) / 2;
  int y = (iScreenHeight - MAIN_WINDOW_HEIGHT) / 2;
  srand((unsigned)time(NULL));
  // Register main window class
  if (RegisterClassEx(&wce) != 0) {
    // Save application instance
    g_hInstance = hInstance;
    // Create main window
    g_hMainWindow = CreateWindowEx(0, MAIN_WINDOW_CLASS_NAME, NULL, WS_OVERLAPPEDWINDOW | WS_VISIBLE, x, y,
        MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT + 24, NULL, NULL, g_hInstance, NULL);
    if (g_hMainWindow != NULL) {
      // Main
      Main(g_hMainWindow);
    }
    else {
      // Error creating main window
      MessageBox(NULL, _T("The main window can not be created!"), _T("Error..."), MB_OK | MB_ICONERROR);
      iReturnValue = -1;
    }
    // Unregister main window class
    UnregisterClass(MAIN_WINDOW_CLASS_NAME, hInstance);
  }
  else {
    // Error registering main window class
    MessageBox(NULL, _T("The main window class can not be registered!"), _T("Error..."), MB_OK | MB_ICONERROR);
    iReturnValue = -1;
  }
  return iReturnValue;
}

