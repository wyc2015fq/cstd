#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/Glut.h>
// #include <GL/glaux.h>
// #include <GL/glext.h>
// #include <GL/wglext.h>
//#pragma comment(lib,"glu32.lib")
//pragma comment(lib,"glaux.lib")
//pragma comment(lib,"opengl32.lib")
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //窗口函数说明
int test_win32()
{
  HINSTANCE hInstance = 0; //WinMain函数说明
  HINSTANCE hPrevInstance = 0;
  LPSTR lpCmdLine = 0;
  int nCmdShow = 0;
  //建议采用Pascal的变量定义风格，即在程序（函数）开始处定义所有变量
  //虽然C++的变量定义比较灵活，本程序为了使程序易于理解,未采用这种方法
  char lpszClassName[] = "窗口"; //窗口类名
  char lpszTitle[] = "Win32窗口示例"; //窗口标题名
  WNDCLASS wndclass;
  wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //窗口类型为缺省类型
  wndclass.lpfnWndProc = WndProc; //窗口处理函数为WndProc
  wndclass.cbClsExtra = 0; //窗口类无扩展
  wndclass.cbWndExtra = 0; //窗口实例无扩展
  wndclass.hInstance = hInstance; //当前实例句柄
  wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //使用缺省图标
  wndclass.hCursor = LoadCursor(NULL, IDC_ARROW); //窗口采用箭头光标
  wndclass.hbrBackground = NULL; //窗口背景为白色
  wndclass.lpszMenuName = NULL; //窗口中无菜单
  wndclass.lpszClassName = lpszClassName; //窗口类名为'窗口实例'
  if (!RegisterClass(&wndclass)) { //注册窗口，若失败，则发出声音
    MessageBeep(0);
    return FALSE;
  }
  //创建窗口操作
  HWND hwnd; //窗口结构
  hwnd = CreateWindow(lpszClassName, //创建窗口，窗口类名
      lpszTitle, //窗口实例的标题名
      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, //窗口的风格
      CW_USEDEFAULT,
      CW_USEDEFAULT, //窗口左上角坐标为缺省值
      CW_USEDEFAULT,
      CW_USEDEFAULT, //窗口的高度和宽度为缺省值
      NULL, //此窗口无父窗口
      NULL, //此窗口无主菜单
      hInstance, //应用程序当前句柄
      NULL); //不使用该值
  ShowWindow(hwnd, nCmdShow); //显示窗口
  UpdateWindow(hwnd); //绘制用户区
  // int InitGL(GLvoid);
  MSG msg; //消息结构
  while (GetMessage(&msg, NULL, 0, 0)) { //消息循环
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam; //程序终止时，将信息返回操作系统
}
void SetupPixelFormat(HDC hDC) //为设备环境设置像素格式
{
  int nPixelFormat; //像素格式变量
  static PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR), //数据结构大小
    1, //版本号，总设为1
    PFD_DRAW_TO_WINDOW | //支持窗口
    PFD_SUPPORT_OPENGL | //支持OpenGL
    PFD_DOUBLEBUFFER, //支持双缓存
    PFD_TYPE_RGBA, //RGBA颜色模式
    32, //32位颜色模式
    0, 0, 0, 0, 0, 0, //忽略颜色为，不使用
    0, //无alpha缓存
    0, //忽略偏移位
    0, //无累积缓存
    0, 0, 0, 0, //忽略累积位
    16, //16位z-buffer（z缓存）大小
    0, //无模板缓存
    0, //无辅助缓存
    PFD_MAIN_PLANE, //主绘制平面
    0, //保留的数据项
    0, 0, 0
  };
  //忽略层面掩模
  //选择最匹配的像素格式，返回索引值
  nPixelFormat = ChoosePixelFormat(hDC, &pfd);
  //设置环境设备的像素格式
  SetPixelFormat(hDC, nPixelFormat, &pfd);
}
int InitGL(GLvoid) // 此处开始对OpenGL进行所有设置
{
  glShadeModel(GL_SMOOTH); // 启用阴影平滑
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // 黑色背景
  glClearDepth(1.0f); // 设置深度缓存
  glEnable(GL_DEPTH_TEST); // 启用深度测试
  glDepthFunc(GL_LEQUAL); // 所作深度测试的类型
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);// 告诉系统对透视进行修正
  return TRUE; // 初始化 OK
}
void ChangeSize(int width, int height)
{
  glViewport(0, 0, width, height); // 重置当前的视口glMatrixMode(GL_PROJECTION);
  glMatrixMode(GL_PROJECTION); // 选择投影矩阵
  glLoadIdentity(); // 重置投影矩阵
  // 设置投影模式为透视投影
  gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清屏和清除深度缓冲区
  glLoadIdentity(); // 重置当前的模型观察矩阵
  glTranslatef(-1.5f, 0.0f, -26.0f); // 左移 1.5 单位，并移入屏幕 6.0
  glBegin(GL_TRIANGLES); // 绘制三角形
  glVertex3f(0.0f, 1.0f, 0.0f); // 上顶点
  glVertex3f(-1.0f, -1.0f, 0.0f); // 左下
  glVertex3f(1.0f, -1.0f, 0.0f); // 右下
  glEnd(); // 三角形绘制结束
  glTranslatef(3.0f, 0.0f, 0.0f); // 右移3单位
  glRotatef(45, 1, 1, 0);
  glutWireCube(2);
  //此处添加绘图命令
  //glutSwapBuffers();
  // glFlush();
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  static HGLRC hRC; //绘制环境
  static HDC hDC; //设备环境
  switch (message) {
  case WM_CREATE: {
    hDC = GetDC(hwnd); //得到当前窗口的设备环境
    SetupPixelFormat(hDC); //调用像素格式设置函数
    hRC = wglCreateContext(hDC); //创建OpenGL绘图环境并创建一个指向OpenGL绘制环境的句柄
    wglMakeCurrent(hDC, hRC); //将传递过来的绘制环境设置为OpenGL将要进行绘制的当前绘制环境
    // SetTimer(hwnd, 33, 1, NULL);
    InitGL();
    return 0;
  }
  break;
  case WM_DESTROY: {
    // KillTimer(hwnd, 101);
    wglMakeCurrent(hDC, NULL);
    wglDeleteContext(hRC);
    PostQuitMessage(0); //调用该函数发出WM_QUIT消息
  }
  break;
  case WM_SIZE: {
    //ChangeSize(LOWORD(lParam), HIWORD(lParam));
    ChangeSize(LOWORD(lParam), HIWORD(lParam));
  }
  break;
  // case WM_TIMER:
  // {
  // //IdleFunction();
  // InvalidateRect(hwnd, NULL, FALSE);
  // }
  // break;
  case WM_PAINT: {
    RenderScene();
    SwapBuffers(hDC);
    ValidateRect(hwnd, NULL);
  }
  break;
  // case
  default: //缺省消息处理函数
    return DefWindowProc(hwnd, message, wParam, lParam);
  }
  return 0;
}

