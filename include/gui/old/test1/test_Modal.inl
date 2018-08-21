#include <windows.h>
const char* AppName = "AppName";
const char* AppName2 = "AppName2";
void Modal(HWND last)
{
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  EnableWindow(last, TRUE);
  SetForegroundWindow(last);
}
LRESULT CALLBACK test_Modal_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg) {
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  case WM_LBUTTONDOWN: {
    HWND hwnd2 = NULL;
    int id;
    int ent;
    ent = HIWORD(wParam);//通知吗
    id = LOWORD(wParam);//子窗口ID
    //判断按钮的单机事件
    EnableWindow(hwnd, FALSE);
    hwnd2 = CreateWindowEx(NULL, AppName2, TEXT("窗口标题"), WS_OVERLAPPEDWINDOW, 150, 150, 500, 500, hwnd, NULL, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), 0);
    if (hwnd2 == NULL) {
      MessageBox(NULL, TEXT("创建窗口失败!"), TEXT("错误"), MB_ICONERROR);
      return 0;
    }
    //SetWindowLong(hwnd2, GWL_WNDPROC, (LONG)test_Modal_WndProc2);
    ShowWindow(hwnd2, 5);
    UpdateWindow(hwnd2);
    Modal(hwnd);
  }
  break;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
LRESULT CALLBACK test_Modal_WndProc2(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg) {
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
int test_Modal()
{
  HINSTANCE hInstance = GetModuleHandle(0);
  WNDCLASSA wc;
  //InitCommonControls();
  //这里是在构建窗口类结构
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = test_Modal_WndProc;//窗口回调函数指针
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;//实例句柄
  wc.hIcon = LoadIcon(hInstance, TEXT("ICON_1"));
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);//默认指针
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);//默认背景颜色
  wc.lpszMenuName = NULL;
  wc.lpszClassName = AppName;//窗口类名
  //注册窗口类
  if (!RegisterClass(&wc)) {
    MessageBox(NULL, TEXT("注册窗口类失败!"), TEXT("错误"), MB_ICONERROR);
    return 0;
  }
  //注册第二个窗口的窗口类
  wc.lpfnWndProc = test_Modal_WndProc2;
  wc.lpszClassName = AppName2;
  if (!RegisterClass(&wc)) {
    MessageBox(NULL, TEXT("注册窗口类失败!"), TEXT("错误"), MB_ICONERROR);
    return 0;
  }
  //建立窗口
  HWND hwnd;
  hwnd = CreateWindowEx(0, AppName, //窗口类名称
      TEXT("The Hello Program"), //窗口标题
      WS_OVERLAPPEDWINDOW | WS_VSCROLL, //窗口风格
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      NULL,
      NULL,
      hInstance, //实例句柄
      NULL);
  ShowWindow(hwnd, 1) ;
  UpdateWindow(hwnd) ;
  //消息循环
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) { //从消息队列中取消息
    TranslateMessage(&msg) ; //转换消息
    DispatchMessage(&msg) ; //派发消息
  }
  return 0;
}

