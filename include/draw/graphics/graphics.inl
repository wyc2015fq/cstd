
#ifndef _GRAPHICS_INL_
#define _GRAPHICS_INL_

#include "ui/uiwin.inl"

#ifndef WINVER
#define WINVER 0x0400      // Specifies that the minimum required platform is Windows 95 and Windows NT 4.0.
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500    // Specifies that the minimum required platform is Windows 2000.
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410  // Specifies that the minimum required platform is Windows 98.
#endif

#include <windows.h>
#include <tchar.h>

// 绘图窗口初始化参数
#define SHOWCONSOLE    1    // 创建图形窗口时，保留控制台的显示
#define NOCLOSE        2    // 没有关闭功能
#define NOMINIMIZE     4    // 没有最小化功能

// 颜色
#define  BLACK         0
#define  BLUE          0xAA0000
#define  GREEN         0x00AA00
#define  CYAN          0xAAAA00
#define  RED           0x0000AA
#define  MAGENTA       0xAA00AA
#define  BROWN         0x0055AA
#define  LIGHTGRAY     0xAAAAAA
#define  DARKGRAY      0x555555
#define  LIGHTBLUE     0xFF5555
#define  LIGHTGREEN    0x55FF55
#define  LIGHTCYAN     0xFFFF55
#define  LIGHTRED      0x5555FF
#define  LIGHTMAGENTA  0xFF55FF
#define  YELLOW        0x55FFFF
#define  WHITE         0xFFFFFF

// 定义颜色转换宏
#define BGR(color)  ( (((color) & 0xFF) << 16) | ((color) & 0xFF00FF00) | (((color) & 0xFF0000) >> 16) )


// 定义线的样式
typedef struct LINESTYLE {
  DWORD  style;
  DWORD  thickness;
  DWORD* puserstyle;
  DWORD  userstylecount;
} LINESTYLE;

typedef struct FILLSTYLE {
  int    style;      // 填充形式
  long    hatch;      // 填充图案样式
  //IMAGE*  ppattern;   // 填充图像
} FILLSTYLE;

// 定义图像对象
typedef struct IMAGE {
  int width, height;    // 对象的宽高
  HBITMAP m_hBmp;
  HWND hwnd;
  HDC hdc;
  int m_MemCurX;      // 当前点X坐标
  int m_MemCurY;      // 当前点Y坐标
  float m_data[6];
  COLORREF  m_LineColor;    // 当前线条颜色
  COLORREF  m_FillColor;    // 当前填充颜色
  COLORREF  m_TextColor;    // 当前文字颜色
  COLORREF  m_BkColor;      // 当前背景颜色
  DWORD*    m_pBuffer;      // 绘图区的内存

  LINESTYLE  m_LineStyle;    // 画线样式
  FILLSTYLE  m_FillStyle;    // 填充样式
} IMAGE;
IMAGE g_win[1] = {0};

int IMAGE_SetDefault(IMAGE* im);            // 设置为默认状态

int IMAGE_resize(IMAGE* im, int _width, int _height);  // 调整尺寸
#define DEFAULT(x)

static LRESULT CALLBACK WindowProc1(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT ret = 0;
  IMAGE* win = g_win;

  switch (uMsg) {
  case WM_GETMINMAXINFO: {
  }
  break;

  case WM_SIZE: {
    ret = hdc_resize(&g_win->hdc, HIWORD(lParam), LOWORD(lParam), 4);
  }
  break;

  case WM_PAINT:
    if (win) {
      PAINTSTRUCT ps = {0};
      HDC hDC = BeginPaint(hwnd, &ps);
      img_t im[1] = {0};
      hdc_get_image(win->hdc, im);
      imflip(im);
      //BitBlt(hMemDC, 0, 0, w, h, hDC, 0, 0, SRCCOPY);
      //ret = win->ctrl->ctrlproc(0, win->ctrl, hwnd, uMsg, (WPARAM)im, lParam);
      //imflip(im);
      BitBlt(hDC, 0, 0, im->w, im->h, win->hdc, 0, 0, SRCCOPY);
      imflip(im);
      EndPaint(hwnd, &ps);
    }

    break;

  case WM_NCLBUTTONDBLCLK: {
    //RECT rc1 = {300, 400, 600, 600};
    //GetWindowRect(hwnd, &rc1);
    //drawDragFrame(rc1);
    //printf("drawDragFrame\n");
    //return 1;
  }
  break;

  case WM_CLOSE:
    DestroyWindow(hwnd);
    break;

  case WM_DESTROY:
    break;

  default:
    //ret = win->ctrl->ctrlproc(0, win->ctrl, hwnd, uMsg, wParam, lParam);
    break;
  }

  if (ret) {
    return ret;
  }

  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// 关闭图形环境
void closegraph(void)
{
  if (g_win->hwnd) {
    ShowWindow(g_win->hwnd, SW_HIDE);
    SendMessage(g_win->hwnd, WM_CLOSE, 0, 0);
    g_win->hwnd = NULL;
  }

  if (g_win->hdc) {
    DeleteObject(SelectObject(g_win->hdc, 0));
    DeleteDC(g_win->hdc);
    g_win->hdc = NULL;
  }

  return ;
}

// 绘图模式相关函数
HWND initgraph(int width, int height, int flag DEFAULT(0))
{
  static int inited = 0;

  if (!inited) {
    g_hInstance = GetModuleHandle(0);
    // first fill in the window class stucture
    g_winclass.cbSize = sizeof(WNDCLASSEX);
    g_winclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    g_winclass.lpfnWndProc = WindowProc1;
    g_winclass.cbClsExtra = 0;
    g_winclass.cbWndExtra = 0;
    g_winclass.hInstance = g_hInstance;
    g_winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    g_winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    //g_winclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    g_winclass.hbrBackground = (HBRUSH)COLOR_WINDOW;
    g_winclass.lpszMenuName = NULL;
    g_winclass.lpszClassName = MYWINDOWCLASSNAME;
    g_winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    //register the window class
    if (!RegisterClassEx(&g_winclass)) {
      MessageBox(NULL, "Class Registration Failed!", "Error", 0);
      //exit the application
      return 0;
    }

    inited = 1;
    atexit(closegraph);
  }

  ASSERT(0 == g_win->hdc);
  ASSERT(0 == g_win->hwnd);
  {
    DWORD dwStyleEx = 0;
    DWORD dwStyle = MYWS_WINDOW;
    IRECT rc = iRECT(0, 0, width, height);
    hdc_resize(&g_win->hdc, height, width, 4);
    g_win->hwnd = CreateWindowEx(dwStyleEx, MYWINDOWCLASSNAME, "test", dwStyle,
        rc.l, rc.t, width, height, 0, 0, g_hInstance, 0); // creation parameters
    ShowWindow(g_win->hwnd, SW_SHOW);
  }
  return g_win->hwnd;
}  // 初始化图形环境

// 绘图环境设置

int cleardevice();          // 清屏
int setcliprgn(HRGN hrgn);      // 设置当前绘图设备的裁剪区
int clearcliprgn();        // 清除裁剪区的屏幕内容

int getlinestyle(LINESTYLE* pstyle);            // 获取当前画线样式
int setlinestyle(const LINESTYLE* pstyle) {      // 设置当前画线样式
  g_win->m_LineStyle = *pstyle;
  return 0;
}
int setlinestyle2(int style, int thickness DEFAULT(1), const DWORD* puserstyle DEFAULT(NULL), DWORD userstylecount DEFAULT(0)) {  // 设置当前画线样式
  LINESTYLE pstyle[1] = {0};
  pstyle->style = style;
  pstyle->thickness = thickness;
  pstyle->puserstyle = (DWORD*)puserstyle;
  pstyle->userstylecount = userstylecount;
  
  return 0;
}
int getfillstyle(FILLSTYLE* pstyle);            // 获取当前填充样式
int setfillstyle(const FILLSTYLE* pstyle);      // 设置当前填充样式
int setfillstyle2(int style, long hatch DEFAULT(NULL), IMAGE* ppattern DEFAULT(NULL));    // 设置当前填充样式
int setfillstyle3(BYTE* ppattern8x8);           // 设置当前填充样式

int setorigin(int x, int y);              // 设置坐标原点
int getaspectratio(float* pxasp, float* pyasp);  // 获取当前缩放因子
int setaspectratio(float xasp, float yasp);      // 设置当前缩放因子

int getrop2();                 // 获取前景的二元光栅操作模式
int setrop2(int mode);        // 设置前景的二元光栅操作模式
int getpolyfillmode();         // 获取多边形填充模式
int setpolyfillmode(int mode);    // 设置多边形填充模式

int graphdefaults();        // 重置所有绘图设置为默认值

COLORREF getlinecolor();      // 获取当前线条颜色
int setlinecolor(COLORREF color);  // 设置当前线条颜色
COLORREF gettextcolor();      // 获取当前文字颜色
int settextcolor(COLORREF color);  // 设置当前文字颜色
COLORREF getfillcolor();      // 获取当前填充颜色
int setfillcolor(COLORREF color)
{
  g_win->m_FillColor = SWAPRB(color);
  return 0;
}  // 设置当前填充颜色
COLORREF getbkcolor();        // 获取当前绘图背景色
int setbkcolor(COLORREF color);  // 设置当前绘图背景色
int getbkmode();          // 获取背景混合模式
int setbkmode(int mode);      // 设置背景混合模式

// 颜色模型转换函数
COLORREF RGBtoGRAY(COLORREF rgb);
int RGBtoHSLf(COLORREF rgb, float* H, float* S, float* L);
int RGBtoHSVf(COLORREF rgb, float* H, float* S, float* V);
COLORREF HSLtoRGB(float H, float S, float L);
COLORREF HSVtoRGBf(float H, float S, float V);


// 绘图函数

COLORREF getpixel(int x, int y);        // 获取点的颜色
int putpixel(int x, int y, COLORREF color);  // 画点

int moveto(double x, double y);            // 移动当前点(绝对坐标)
int moverel(double dx, double dy);          // 移动当前点(相对坐标)

int line(double x1, double y1, double x2, double y2) {    // 画线
  img_t im[1] = {0};
  hdc_get_image(g_win->hdc, im);
  imdrawaa_line(im, 0, x1, y1, x2, y2, g_win->m_LineColor, g_win->m_LineStyle.thickness);
  return 0;
}
int linerel(double dx, double dy);          // 画线(至相对坐标)
int lineto(double x, double y);            // 画线(至绝对坐标)

int rectangle(double left, double top, double right, double bottom);       // 画矩形
int fillrectangle(double left, double top, double right, double bottom);   // 画填充矩形(有边框)
int solidrectangle(double left, double top, double right, double bottom);  // 画填充矩形(无边框)
int clearrectangle(double left, double top, double right, double bottom);  // 清空矩形区域

int circle(double x, double y, double radius);        // 画圆
int fillcircle(double x, double y, double radius)
{
  img_t im[1] = {0};
  hdc_get_image(g_win->hdc, im);
  imdrawaa_ellipse(im, 0, x, y, radius, radius, g_win->m_FillColor, 0, 0);
  return 0;
}     // 画填充圆(有边框)
int solidcircle(double x, double y, double radius);    // 画填充圆(无边框)
int clearcircle(double x, double y, double radius);    // 清空圆形区域

int ellipse(double left, double top, double right, double bottom);     // 画椭圆
int fillellipse(double left, double top, double right, double bottom);   // 画填充椭圆(有边框)
int solidellipse(double left, double top, double right, double bottom);  // 画填充椭圆(无边框)
int clearellipse(double left, double top, double right, double bottom);  // 清空椭圆形区域

int roundrect(double left, double top, double right, double bottom, int ellipsewidth, int ellipseheight);         // 画圆角矩形
int fillroundrect(double left, double top, double right, double bottom, int ellipsewidth, int ellipseheight);     // 画填充圆角矩形(有边框)
int solidroundrect(double left, double top, double right, double bottom, int ellipsewidth, int ellipseheight);    // 画填充圆角矩形(无边框)
int clearroundrect(double left, double top, double right, double bottom, int ellipsewidth, int ellipseheight);    // 清空圆角矩形区域

int arc(double left, double top, double right, double bottom, double stangle, double endangle);     // 画椭圆弧(起始角度和终止角度为弧度制)
int pie(double left, double top, double right, double bottom, double stangle, double endangle);     // 画椭圆扇形(起始角度和终止角度为弧度制)
int fillpie(double left, double top, double right, double bottom, double stangle, double endangle);   // 画填充椭圆扇形(有边框)
int solidpie(double left, double top, double right, double bottom, double stangle, double endangle);  // 画填充椭圆扇形(无边框)
int clearpie(double left, double top, double right, double bottom, double stangle, double endangle);  // 清空椭圆扇形区域

int polyline(const POINT* points, int num);     // 画多条连续的线
int polygon(const POINT* points, int num);     // 画多边形
int fillpolygon(const POINT* points, int num);   // 画填充的多边形(有边框)
int solidpolygon(const POINT* points, int num);  // 画填充的多边形(无边框)
int clearpolygon(const POINT* points, int num);  // 清空多边形区域

int floodfill(int x, int y, int border);      // 填充区域

// 文字相关函数

int outtext(LPCTSTR str);          // 在当前位置输出字符串
int outchar(TCHAR c);            // 在当前位置输出字符
int outtextxy(int x, int y, LPCTSTR str);  // 在指定位置输出字符串
int outcharxy(int x, int y, TCHAR c);    // 在指定位置输出字符
int textwidth(LPCTSTR str);          // 获取字符串占用的像素宽
int charwidth(TCHAR c);            // 获取字符占用的像素宽
int textheight(LPCTSTR str);        // 获取字符串占用的像素高
int charheight(TCHAR c);          // 获取字符占用的像素高
int drawtext(LPCTSTR str, RECT* pRect, UINT uFormat);  // 在指定区域内以指定格式输出字符串
int drawchar(TCHAR c, RECT* pRect, UINT uFormat);    // 在指定区域内以指定格式输出字符

// 设置当前字体样式(详见帮助)
//    nHeight: 字符的平均高度；
//    nWidth: 字符的平均宽度(0 表示自适应)；
//    lpszFace: 字体名称；
//    nEscapement: 字符串的书写角度(单位 0.1 度)；
//    nOrientation: 每个字符的书写角度(单位 0.1 度)；
//    nWeight: 字符的笔画粗细(0 表示默认粗细)；
//    bItalic: 是否斜体；
//    bUnderline: 是否下划线；
//    bStrikeOut: 是否删除线；
//    fbCharSet: 指定字符集；
//    fbOutPrecision: 指定文字的输出精度；
//    fbClipPrecision: 指定文字的剪辑精度；
//    fbQuality: 指定文字的输出质量；
//    fbPitchAndFamily: 指定以常规方式描述字体的字体系列。
int settextstyle(int nHeight, int nWidth, LPCTSTR lpszFace);
int settextstyle2(int nHeight, int nWidth, LPCTSTR lpszFace, int nEscapement, int nOrientation, int nWeight, BOOL bItalic, BOOL bUnderline, BOOL bStrikeOut);
int settextstyle3(int nHeight, int nWidth, LPCTSTR lpszFace, int nEscapement, int nOrientation, int nWeight, BOOL bItalic, BOOL bUnderline, BOOL bStrikeOut, BYTE fbCharSet, BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality, BYTE fbPitchAndFamily);
int settextfont(const LOGFONT* font);  // 设置当前字体样式
int gettextfont1(LOGFONT* font);    // 获取当前字体样式



// 图像处理函数
//int loadimage(IMAGE *pDstImg, LPCTSTR pImgFile, int nWidth DEFAULT(0), int nHeight DEFAULT(0), BOOL bResize DEFAULT(FALSE));          // 从图片文件获取图像(bmp/jpg/gif/emf/wmf)
//int loadimage(IMAGE *pDstImg, LPCTSTR pResType, LPCTSTR pResName, int nWidth DEFAULT(0), int nHeight DEFAULT(0), BOOL bResize DEFAULT(FALSE));  // 从资源文件获取图像(bmp/jpg/gif/emf/wmf)
//int saveimage(LPCTSTR pImgFile, IMAGE* pImg DEFAULT(NULL));                                    // 保存图像
int getimage(IMAGE* pDstImg, int srcX, int srcY, int srcWidth, int srcHeight);                        // 从当前绘图设备获取图像
int putimage(int dstX, int dstY, const IMAGE* pSrcImg, DWORD dwRop DEFAULT(SRCCOPY));                        // 绘制图像到屏幕
int putimage2(int dstX, int dstY, int dstWidth, int dstHeight, const IMAGE* pSrcImg, int srcX, int srcY, DWORD dwRop DEFAULT(SRCCOPY));    // 绘制图像到屏幕(指定宽高)
int rotateimage(IMAGE* dstimg, IMAGE* srcimg, double radian, COLORREF bkcolor DEFAULT(BLACK), BOOL autosize DEFAULT(FALSE), BOOL highquality DEFAULT(TRUE));// 旋转图像
int Resize(IMAGE* pImg, int width, int height);  // 调整绘图设备的大小
DWORD* GetImageBuffer(img_t* im)
{
  hdc_get_image(g_win->hdc, im);
  ASSERT(4 == im->c);
  return (DWORD*)im->tt.data;
}      // 获取绘图设备的显存指针
IMAGE* GetWorkingImage();              // 获取当前绘图设备
int SetWorkingImage(IMAGE* pImg DEFAULT(NULL));      // 设置当前绘图设备
HDC GetImageHDC(IMAGE* pImg DEFAULT(NULL));        // 获取绘图设备句柄(HDC)


// 其它函数

int getwidth();      // 获取绘图区宽度
int getheight();    // 获取绘图区高度
int getx();        // 获取当前 x 坐标
int gety();        // 获取当前 y 坐标

int BeginBatchDraw()
{
  return 0; // 开始批量绘制
}
int FlushBatchDraw()
{
  InvalidateRect(g_win->hwnd, 0, 0);  // 执行未完成的绘制任务
  return 0;
}
int EndBatchDraw()
{
  return 0; // 结束批量绘制，并执行未完成的绘制任务
}

HWND GetHWnd();                // 获取绘图窗口句柄(HWND)
TCHAR* GetEasyXVer();            // 获取 EasyX 当前版本

// 获取用户输入
BOOL InputBox(LPTSTR pString, int nMaxCount, LPCTSTR pPrompt DEFAULT(NULL), LPCTSTR pTitle DEFAULT(NULL), LPCTSTR pDefault DEFAULT(NULL), int width DEFAULT(0), int height DEFAULT(0), BOOL bOnlyOK DEFAULT(TRUE));



// 鼠标消息
// 支持如下消息：
//    WM_MOUSEMOVE    鼠标移动
//    WM_MOUSEWHEEL    鼠标滚轮拨动
//    WM_LBUTTONDOWN    左键按下
//    WM_LBUTTONUP    左键弹起
//    WM_LBUTTONDBLCLK  左键双击
//    WM_MBUTTONDOWN    中键按下
//    WM_MBUTTONUP    中键弹起
//    WM_MBUTTONDBLCLK  中键双击
//    WM_RBUTTONDOWN    右键按下
//    WM_RBUTTONUP    右键弹起
//    WM_RBUTTONDBLCLK  右键双击
typedef struct MOUSEMSG {
  UINT uMsg;       // 当前鼠标消息
  BOOL mkCtrl;     // Ctrl 键是否按下
  BOOL mkShift;    // Shift 键是否按下
  BOOL mkLButton;  // 鼠标左键是否按下
  BOOL mkMButton;  // 鼠标中键是否按下
  BOOL mkRButton;  // 鼠标右键是否按下
  short x;         // 当前鼠标 x 坐标
  short y;         // 当前鼠标 y 坐标
  short wheel;     // 鼠标滚轮滚动值 (120 的倍数)
} MOUSEMSG;

BOOL GetMouseMsg(MOUSEMSG* msg, int delay)
{
  int time0 = GetTickCount();

  if (msg) {
    memset(msg, 0, sizeof(*msg));
  }

  for (;;) {
    MSG message;
    int is_processed = 0;

    if (0 == g_win->hwnd) {
      return -2;
    }

    if ((delay > 0 && abs((int)(GetTickCount() - time0)) >= delay)) {
      return -1;
    }

    if (delay <= 0) {
      GetMessage(&message, 0, 0, 0);
    }
    else if (PeekMessage(&message, 0, 0, 0, PM_REMOVE) == FALSE) {
      Sleep(1);
      continue;
    }

    if (is_processed == 0) {
      if (g_win->hwnd == message.hwnd) {
        is_processed = 1;

        switch (message.message) {
        case WM_DESTROY:
        case WM_CHAR:
          DispatchMessage(&message);
          return (int) message.wParam;

        case WM_LBUTTONUP:
        case WM_LBUTTONDOWN:
        case WM_MOUSEMOVE:
          if (msg) {
            msg->uMsg = message.message;
            msg->x = GET_X_LPARAM(message.lParam);
            msg->y = GET_Y_LPARAM(message.lParam);
            msg->mkCtrl = !!(MK_CONTROL & message.wParam);
            msg->mkShift = !!(MK_SHIFT & message.wParam);
            msg->mkLButton = !!(MK_LBUTTON & message.wParam);
            msg->mkRButton = !!(MK_RBUTTON & message.wParam);
            msg->mkMButton = !!(MK_MBUTTON & message.wParam);
            is_processed = 1;
          }

          break;

        case WM_SYSKEYDOWN:
          if (message.wParam == VK_F10) {
            is_processed = 1;
            return (int)(message.wParam << 16);
          }

          break;

        case WM_KEYDOWN:
          TranslateMessage(&message);

          if ((message.wParam >= VK_F1 && message.wParam <= VK_F24) ||
              message.wParam == VK_HOME || message.wParam == VK_END ||
              message.wParam == VK_UP || message.wParam == VK_DOWN ||
              message.wParam == VK_LEFT || message.wParam == VK_RIGHT ||
              message.wParam == VK_INSERT || message.wParam == VK_DELETE ||
              message.wParam == VK_PRIOR || message.wParam == VK_NEXT) {
            DispatchMessage(&message);
            is_processed = 1;
            return (int)(message.wParam << 16);
          }

          break;

        default:
          DispatchMessage(&message);
          is_processed = 1;
          break;
        }
      }
    }

    if (!is_processed) {
      TranslateMessage(&message);
      DispatchMessage(&message);
    }
  }

  return 0;
}    // 获取一个鼠标消息。如果没有，就等待
int FlushMouseMsgBuffer();  // 清空鼠标消息缓冲区


// 兼容 initgraph 绘图模式的宏定义（无实际意义）
#define DETECT  0
#define VGA    0
#define  VGALO  0
#define VGAMED  0
#define VGAHI  0
#define CGA    0
#define  CGAC0  0
#define  CGAC1  0
#define CGAC2  0
#define CGAC3  0
#define CGAHI  0
#define EGA    0
#define EGALO  0
#define  EGAHI  0

int bar(double left, double top, double right, double bottom);    // 画无边框填充矩形
int bar3d(double left, double top, double right, double bottom, int depth, BOOL topflag);  // 画有边框三维填充矩形

int drawpoly(int numpoints, const int* polypoints);  // 画多边形
int fillpoly(int numpoints, const int* polypoints);  // 画填充的多边形

int getmaxx();          // 获取最大的宽度值
int getmaxy();          // 获取最大的高度值

COLORREF getcolor();      // 获取当前绘图前景色
int setcolor(COLORREF color)
{
  g_win->m_LineColor = color;
  return 0;
}  // 设置当前绘图前景色

int setwritemode(int mode);  // 设置前景的二元光栅操作模式




///////////////////////////////////////////////////////////////////////////////////
// 以下函数仅为兼容早期 EasyX 的接口，不建议使用。请使用相关新函数替换。
//

// 设置当前字体样式(该函数已不再使用，请使用 settextstyle 代替)
//    nHeight: 字符的平均高度；
//    nWidth: 字符的平均宽度(0 表示自适应)；
//    lpszFace: 字体名称；
//    nEscapement: 字符串的书写角度(单位 0.1 度)；
//    nOrientation: 每个字符的书写角度(单位 0.1 度)；
//    nWeight: 字符的笔画粗细(0 表示默认粗细)；
//    bItalic: 是否斜体；
//    bUnderline: 是否下划线；
//    bStrikeOut: 是否删除线；
//    fbCharSet: 指定字符集；
//    fbOutPrecision: 指定文字的输出精度；
//    fbClipPrecision: 指定文字的剪辑精度；
//    fbQuality: 指定文字的输出质量；
//    fbPitchAndFamily: 指定以常规方式描述字体的字体系列。
int setfont(int nHeight, int nWidth, LPCTSTR lpszFace);
int setfont2(int nHeight, int nWidth, LPCTSTR lpszFace, int nEscapement, int nOrientation, int nWeight, BOOL bItalic, BOOL bUnderline, BOOL bStrikeOut);
int setfont3(int nHeight, int nWidth, LPCTSTR lpszFace, int nEscapement, int nOrientation, int nWeight, BOOL bItalic, BOOL bUnderline, BOOL bStrikeOut, BYTE fbCharSet, BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality, BYTE fbPitchAndFamily);
int setfont4(const LOGFONT* font);  // 设置当前字体样式
int getfont(LOGFONT* font);      // 获取当前字体样式

// 以下填充样式不再使用，新的填充样式请参见帮助文件
#define  NULL_FILL       BS_NULL
#define  EMPTY_FILL      BS_NULL
#define  SOLID_FILL      BS_SOLID
// 普通一组
#define BDIAGONAL_FILL    BS_HATCHED, HS_BDIAGONAL          // /// 填充 (对应 BGI 的 LTSLASH_FILL)
#define CROSS_FILL        BS_HATCHED, HS_CROSS            // +++ 填充
#define DIAGCROSS_FILL    BS_HATCHED, HS_DIAGCROSS          // xxx 填充 (heavy cross hatch fill) (对应 BGI 的 XHTACH_FILL)
#define DOT_FILL          (BYTE*)"\x80\x00\x08\x00\x80\x00\x08\x00"  // xxx 填充 (对应 BGI 的 WIDE_DOT_FILL)  
#define FDIAGONAL_FILL    BS_HATCHED, HS_FDIAGONAL          // \\\ 填充
#define HORIZONTAL_FILL   BS_HATCHED, HS_HORIZONTAL          // === 填充
#define VERTICAL_FILL     BS_HATCHED, HS_VERTICAL            // ||| 填充
// 密集一组
#define BDIAGONAL2_FILL   (BYTE*)"\x44\x88\x11\x22\x44\x88\x11\x22"
#define CROSS2_FILL       (BYTE*)"\xff\x11\x11\x11\xff\x11\x11\x11"  // (对应 BGI 的 fill HATCH_FILL)
#define DIAGCROSS2_FILL   (BYTE*)"\x55\x88\x55\x22\x55\x88\x55\x22"
#define DOT2_FILL         (BYTE*)"\x88\x00\x22\x00\x88\x00\x22\x00"  // (对应 BGI 的 CLOSE_DOT_FILL)
#define FDIAGONAL2_FILL   (BYTE*)"\x22\x11\x88\x44\x22\x11\x88\x44"
#define HORIZONTAL2_FILL  (BYTE*)"\x00\x00\xff\x00\x00\x00\xff\x00"
#define VERTICAL2_FILL    (BYTE*)"\x11\x11\x11\x11\x11\x11\x11\x11"
// 粗线一组
#define BDIAGONAL3_FILL   (BYTE*)"\xe0\xc1\x83\x07\x0e\x1c\x38\x70"  // (对应 BGI 的 SLASH_FILL)
#define CROSS3_FILL       (BYTE*)"\x30\x30\x30\x30\x30\x30\xff\xff"
#define DIAGCROSS3_FILL   (BYTE*)"\xc7\x83\xc7\xee\x7c\x38\x7c\xee"
#define DOT3_FILL         (BYTE*)"\xc0\xc0\x0c\x0c\xc0\xc0\x0c\x0c"
#define FDIAGONAL3_FILL   (BYTE*)"\x07\x83\xc1\xe0\x70\x38\x1c\x0e"
#define HORIZONTAL3_FILL  (BYTE*)"\xff\xff\x00\x00\xff\xff\x00\x00"  // (对应 BGI 的 LINE_FILL)  
#define VERTICAL3_FILL    (BYTE*)"\x33\x33\x33\x33\x33\x33\x33\x33"
// 其它
#define INTERLEAVE_FILL   (BYTE*)"\xcc\x33\xcc\x33\xcc\x33\xcc\x33"  // (对应 BGI 的 INTERLEAVE_FILL)

#endif // _GRAPHICS_INL_
