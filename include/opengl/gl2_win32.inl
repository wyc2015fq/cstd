#include <windows.h>
#include <gl/glew.h>
#pragma comment(lib,"glew32.lib")
#include <gl/gl.h>
#include <gl/glu.h>
#pragma comment(lib,"opengl32.lib")
struct gl_sys_win32_t {
  sys_event_t event[1];
  short KeyMap[512];
};
struct gl2_window_win32_t {
  HWND hwnd;
  HDC hDC;
  HGLRC hGLRC;
  const char* name;
  struct gl2_window_win32_t* next;
  struct gl2_window_win32_t* prev;
  int inited;
};
struct gl_sys_win32_t g_sys[1] = {0};
struct gl2_window_win32_t g_glwin[1] = {0};
void BeginRender()
{
  //glEnableClientState(GL_VERTEX_ARRAY);
  //glEnable(GL_COLOR_ARRAY);
  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void EndRender()
{
  SwapBuffers(g_glwin->hDC);
}
sys_event_t* sys_waitevent(int delay)
{
  sys_event_t* event = g_sys->event;
  static int time_start = 0;
  int time0 = GetTickCount();
  if (0 == time_start) {
    time_start = time0;
  }
  EndRender();
  //io->time = sys_utime_counter();
  // memset(event, 0, sizeof(*event));
  if (g_glwin->inited) {
    static MSG msg[1] = {0};
    event->msg = MSG_NOP;
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
  }
  else {
    g_glwin->inited = 1;
  }
  //TraceMsg(msg->hwnd, msg->message, msg->lParam, msg->wParam);
  //if (!g->g_FontTexture) { imuiImplDX9_CreateDeviceObjects(); }
  {
    // Setup display size (every frame to accommodate for window resizing)
    static int g_TicksPerSecond = 0;
    static int pre_time = 0;
    // Setup time step
    // Read keyboard modifiers inputs
    event->keyctrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
    event->keyshift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
    event->keyalt = (GetKeyState(VK_MENU) & 0x8000) != 0;
    event->tick = time0 - time_start;
    //io->delta_time = (double)(current_time - g_Time) / g_TicksPerSecond;
    //event->tick = (event->time>0 && (current_time - io->time) > delay);
    if (event->msg == MSG_NOP && event->tick - pre_time > delay) {
      event->msg = MSG_TIMER;
      pre_time = event->tick;
    }
  }
  {
#ifdef __glew_h__
    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return NULL;
    }
#endif
  }
  BeginRender();
  return event;
}
//用来检查OpenGL版本，需要GLSL 2.0支持
void getGlVersion(int* major, int* minor)
{
  const char* verstr = (const char*)glGetString(GL_VERSION);
  if ((verstr == NULL) || (sscanf(verstr, "%d.%d", major, minor) != 2)) {
    *major = *minor = 0;
    fprintf(stderr, "Invalid GL_VERSION format!!!/n");
  }
}
int gl2_initio()
{
  int i;
  static int inited = 0;
  if (!inited) {
    SYSIO;
    short* KeyMap = g_sys->KeyMap;
    inited = 1;
    //mouse->dbclick_time = 0.3;
    (void)g;
    (void)x;
    (void)y;
    //SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0);
    //strcpy(io->def_font_name, lf.lfFaceName);
    //io->def_font_size = lf.lfHeight;
    for (i = 0; i < countof(g_sys->KeyMap); ++i) {
      KeyMap[i] = i;
    }
    //KeyMap[VK_TAB] = KEY_TAB;
    KeyMap[VK_LEFT] = KEY_LEFT;
    KeyMap[VK_RIGHT] = KEY_RIGHT;
    KeyMap[VK_UP] = KEY_UP;
    KeyMap[VK_DOWN] = KEY_DOWN;
    KeyMap[VK_PRIOR] = KEY_PAGEUP;
    KeyMap[VK_NEXT] = KEY_PAGEDOWN;
    KeyMap[VK_HOME] = KEY_HOME;
    KeyMap[VK_END] = KEY_END;
    KeyMap[VK_DELETE] = KEY_DELETE;
    KeyMap[VK_BACK] = KEY_BACK;
    //KeyMap[VK_RETURN] = KEY_ENTER;
    KeyMap[VK_ESCAPE] = KEY_ESCAPE;
    //KeyMap[VK_CONTROL] = KEY_CTRL;
    //KeyMap[VK_SHIFT] = KEY_SHIFT;
    //KeyMap[VK_MENU] = KEY_ALT;
    //KeyMap[VK_MENU] = KEY_ALT;
  }
  return 0;
}
LRESULT CALLBACK gl2WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  sysio_t* io = sys_getio();
  sysmouse_t* mouse = io->mouse;
  struct gl2_window_win32_t* s = g_glwin;
  struct sys_event_t* event = g_sys->event;
  gl2_initio();
  switch (uMsg) {
  case WM_CREATE: {
    CREATESTRUCT* creation = (CREATESTRUCT*)(lParam);
    event->w = creation->cx;
  }
  break;
  case WM_CLOSE:
    PostQuitMessage(0);
    break;
  case WM_DESTROY:
    return 0;
  case WM_SIZE:
    event->msg = MSG_SIZE;
    event->w = (int)LOWORD(lParam);
    event->h = (int)HIWORD(lParam);
    return 1;
#if 0
  case WM_PALETTECHANGED:
    //如果它不是当前窗口，则实现调色板与hDC的对应
    if (s && s->hGLRC && hPalette && (HWND)wParam != hwnd) {
      UnrealizeObject(hPalette);
      SelectPalette(hDC, hPalette, FALSE);
      break;
    }
    break;
#endif
    //=========================
  case WM_LBUTTONDOWN:
    event->msg = MSG_LBUTTONDOWN;
    return 1;
  case WM_LBUTTONUP:
    event->msg = MSG_LBUTTONUP;
    return 1;
  case WM_RBUTTONDOWN:
    event->msg = MSG_RBUTTONDOWN;
    return 1;
  case WM_RBUTTONUP:
    event->msg = MSG_RBUTTONUP;
    return 1;
  case WM_MBUTTONDOWN:
    event->msg = MSG_MBUTTONDOWN;
    return 1;
  case WM_MBUTTONUP:
    event->msg = MSG_MBUTTONUP;
    return 1;
  case WM_MOUSEWHEEL:
    event->msg = MSG_MOUSEWHEEL;
    event->wheelDelta = ((short)HIWORD(wParam)) > 0 ? +1.0f : -1.0f;
    return 1;
  case WM_MOUSEMOVE:
    event->msg = MSG_MOUSEMOVE;
    event->x = (signed short)(lParam);
    event->y = (signed short)(lParam >> 16);
    return 1;
  case WM_KEYDOWN:
    event->msg = MSG_KEYDOWN;
    event->keydown[event->last_key = g_sys->KeyMap[wParam & 511]] = 1;
    break;
  default:
    break;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
#define MYGLWINCLASSNAME "gl2_win"
void DisableOpenGL(struct gl2_window_win32_t* s)
{
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(s->hGLRC);
  ReleaseDC(s->hwnd, s->hDC);
  s->hGLRC = NULL;
  s->hwnd = NULL;
  s->hDC = NULL;
}
void EnableOpenGL(struct gl2_window_win32_t* s)
{
  PIXELFORMATDESCRIPTOR pfd;
  int iFormat;
  /* get the device context (DC) */
  s->hDC = GetDC(s->hwnd);
  /* set the pixel format for the DC */
  ZeroMemory(&pfd, sizeof(pfd));
  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 24;
  pfd.cDepthBits = 16;
  pfd.iLayerType = PFD_MAIN_PLANE;
  iFormat = ChoosePixelFormat(s->hDC, &pfd);
  SetPixelFormat(s->hDC, iFormat, &pfd);
  /* create and enable the render context (RC) */
  s->hGLRC = wglCreateContext(s->hDC);
  wglMakeCurrent(s->hDC, s->hGLRC);
}
void rotatef(float angle, float x, float y, float z)
{
  glRotatef(angle, x, y, z);
}
int mode2glmode(GcMode mode)
{
  switch (mode) {
#define MODE2GLMODE(a, b) case a: return b
    MODE2GLMODE(GC_POINTS, GL_POINTS);
    MODE2GLMODE(GC_LINES, GL_LINES);
    MODE2GLMODE(GC_LINE_STRIP, GL_LINE_STRIP);
    MODE2GLMODE(GC_TRIANGLES, GL_TRIANGLES);
    MODE2GLMODE(GC_TRIANGLE_STRIP, GL_TRIANGLE_STRIP);
    MODE2GLMODE(GC_TRIANGLE_FAN, GL_TRIANGLE_FAN);
    MODE2GLMODE(GC_QUADS, GL_QUADS);
    MODE2GLMODE(GC_QUAD_STRIP, GL_QUAD_STRIP);
    MODE2GLMODE(GC_POLYGON, GL_POLYGON);
#undef MODE2GLMODE
  }
  return 0;
}
int type2glmode(GCTYPE type)
{
  switch (type) {
#define MODE2GLMODE(a, b) case a: return b
    MODE2GLMODE(GC_INT8, GL_BYTE);
    MODE2GLMODE(GC_BYTE, GL_UNSIGNED_BYTE);
    MODE2GLMODE(GC_INT16, GL_SHORT);
    MODE2GLMODE(GC_UINT16, GL_UNSIGNED_SHORT);
    MODE2GLMODE(GC_INT32, GL_INT);
    MODE2GLMODE(GC_UINT32, GL_UNSIGNED_INT);
    MODE2GLMODE(GC_FLOAT, GL_FLOAT);
    MODE2GLMODE(GC_DOUBLE, GL_DOUBLE);
#undef MODE2GLMODE
  }
  return 0;
}
void gcVertexPointer(int size, GCTYPE type, int stride, const void* pointer)
{
  int gltype = type2glmode(type);
  glVertexPointer(size, gltype, stride, pointer);
}
void gcDrawArrays(GcMode mode, int first, int count)
{
  int glmode = mode2glmode(mode);
  glDrawArrays(glmode, first, count);
}
void gcDrawElements(GcMode mode, int count, GCTYPE type, const void* indices)
{
  int glmode = mode2glmode(mode);
  int gltype = type2glmode(type);
  glDrawElements(glmode, count, gltype, indices);
}
int new_win(const char* windowName, int winX, int winY, int winWidth, int winHeight)
{
  HINSTANCE hInstance = 0;
  HINSTANCE hPrevInstance = 0;
  HWND hwnd;
  //MSG msg;
  BOOL bQuit = FALSE;
  float theta = 0.0f;
  /* register window class */
  static WNDCLASSEXA wcex = {0};
  if (0 == wcex.cbSize) {
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW; //CS_OWNDC;
    wcex.lpfnWndProc = gl2WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = MYGLWINCLASSNAME;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;
    if (!RegisterClassExA(&wcex)) {
      wcex.cbSize = 0;
      return 0;
    }
  }
  /* create main window */
  hwnd = CreateWindowExA(0,
      MYGLWINCLASSNAME,
      windowName,//"OpenGL Sample",
      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
      winX,//CW_USEDEFAULT,
      winY,//CW_USEDEFAULT,
      winWidth,//256,
      winHeight,//256,
      NULL,
      NULL,
      hInstance,
      NULL);
  if (hwnd) {
    struct gl2_window_win32_t* s = g_glwin;
    memset(s, 0, sizeof(struct gl2_window_win32_t));
    s->name = _strdup(windowName);
    //LIST_ADDFRONT(g_glwin, s);
    s->hwnd = hwnd;
    ShowWindow(hwnd, 1);
    EnableOpenGL(s);
  }
  return 1;
}
static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
  GLuint ShaderObj = glCreateShader(ShaderType);
  const GLchar* p[1];
  GLint Lengths[1];
  GLint success;
  if (ShaderObj == 0) {
    fprintf(stderr, "Error creating shader type %d\n", ShaderType);
    exit(0);
  }
  p[0] = pShaderText;
  Lengths[0] = strlen(pShaderText);
  glShaderSource(ShaderObj, 1, p, Lengths);
  glCompileShader(ShaderObj);
  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
    dbg_printf("Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
    ASSERT(0);
  }
  glAttachShader(ShaderProgram, ShaderObj);
}
static GLuint CompileShaders(const char* vs, const char* fs)
{
  GLuint ShaderProgram = glCreateProgram();
  GLint Success = 0;
  GLchar ErrorLog[1024] = { 0 };
  if (ShaderProgram == 0) {
    fprintf(stderr, "Error creating shader program\n");
    exit(1);
  }
  AddShader(ShaderProgram, vs, GL_VERTEX_SHADER);
  AddShader(ShaderProgram, fs, GL_FRAGMENT_SHADER);
  glLinkProgram(ShaderProgram);
  glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
  if (Success == 0) {
    glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
    fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
    exit(1);
  }
  glValidateProgram(ShaderProgram);
  glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
  if (!Success) {
    glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
    fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
    exit(1);
  }
  glUseProgram(ShaderProgram);
  return ShaderProgram;
}

