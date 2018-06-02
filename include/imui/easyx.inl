#include "imui.inl"
// 颜色
#define BLACK 0
#define BLUE 0xAA0000
#define GREEN 0x00AA00
#define CYAN 0xAAAA00
#define RED 0x0000AA
#define MAGENTA 0xAA00AA
#define BROWN 0x0055AA
#define LIGHTGRAY 0xAAAAAA
#define DARKGRAY 0x555555
#define LIGHTBLUE 0xFF5555
#define LIGHTGREEN 0x55FF55
#define LIGHTCYAN 0xFFFF55
#define LIGHTRED 0x5555FF
#define LIGHTMAGENTA 0xFF55FF
#define YELLOW 0x55FFFF
#define WHITE 0xFFFFFF
// 定义颜色转换宏
#define EASYX_COLOR(color) ( (((color) & 0xFF) << 16) | ((color) & 0xFF00FF00) | (((color) & 0xFF0000) >> 16) | (0xFF000000) )
//#define EASYX_COLOR(color) (color)
typedef struct {
  ImGuiHost host[1];
  COLOR m_BkColor; // 当前背景颜色
  gc_t ctx[1];
} easyx_t;
easyx_t g_easyx[1] = {0};
int initgraph(int width, int height, int flag DEFAULT(0))
{
  ImGuiDriverInit_t drv_init;
  drv_init = gl2_Init;
  drv_init = d3d9_Init;
  drv_init = gdip_Init;
  drv_init = gdi_Init;
  drv_init = soft_Init;
  ImGuiHost_Init(g_easyx->host, drv_init, "easyx", 100, 100, width, height);
  return 0;
}
int _getch()
{
  for (; imuiLoop(50);) {
    GETGUIIO2();
    if (host_begin(g_easyx->host, g_easyx->m_BkColor)) {
      gc_t* ctx = g_easyx->host->rootWindow->ctx;
      char* ptr = (char*)myAllocPush(ctx->buf, ctx->len, g_easyx->ctx->len);
      memcpy(ptr, g_easyx->ctx->buf, g_easyx->ctx->len);
      //g_easyx->ctx->len = 0;
      if (MSG_KEYDOWN == io->lastMsg) {
        break;
      }
      host_end();
    }
  }
  return 0;
}
int closegraph()
{
  imuiUnInit();
  return 0;
}
// 绘图环境设置
// 清屏
int cleardevice()
{
  return 0;
}
int setcliprect(int x, int y, int w, int h); // 设置当前绘图设备的裁剪区
int clearclip(); // 清除裁剪区的屏幕内容
int setlinestyle(int line_style, double wline) // 设置当前画线样式
{
  gcLineStyle(g_easyx->ctx, line_style | ALIGN_CENTER);
  gcLineWidth(g_easyx->ctx, wline);
  return 0;
}
// 设置当前填充样式
//int setfillstyle(const FILLSTYLE* pstyle);
// 设置坐标原点
int setorigin(double x, double y)
{
  gcTranslate(g_easyx->ctx, x, y);
  return 0;
}
int getaspectratio(float* pxasp, float* pyasp); // 获取当前缩放因子
int setaspectratio(float xasp, float yasp); // 设置当前缩放因子
int getrop2(); // 获取前景的二元光栅操作模式
int setrop2(int mode); // 设置前景的二元光栅操作模式
int getpolyfillmode(); // 获取多边形填充模式
int setpolyfillmode(int mode); // 设置多边形填充模式
int graphdefaults(); // 重置所有绘图设置为默认值
COLOR getlinecolor(); // 获取当前线条颜色
// 设置当前线条颜色
int setlinecolor(COLOR color)
{
  gcStrokeColor(g_easyx->ctx, EASYX_COLOR(color));
  return 0;
}
COLOR gettextcolor(); // 获取当前文字颜色
int settextcolor(COLOR color); // 设置当前文字颜色
COLOR getfillcolor(); // 获取当前填充颜色
// 设置当前填充颜色
int setfillcolor(COLOR color)
{
  gcFillColor(g_easyx->ctx, EASYX_COLOR(color));
  return 0;
}
int setcolor(COLOR color)
{
  gcFillColor(g_easyx->ctx, EASYX_COLOR(color));
  gcStrokeColor(g_easyx->ctx, EASYX_COLOR(color));
  return 0;
}
COLOR getbkcolor(); // 获取当前绘图背景色
// 设置当前绘图背景色
int setbkcolor(COLOR color)
{
  g_easyx->m_BkColor = EASYX_COLOR(color);
  return 0;
}
int getbkmode(); // 获取背景混合模式
int setbkmode(int mode);
// 设置背景混合模式
// 颜色模型转换函数
COLOR RGBtoGRAY(COLOR rgb);
int RGBtoHSLf(COLOR rgb, float* H, float* S, float* L);
int RGBtoHSVf(COLOR rgb, float* H, float* S, float* V);
COLOR HSLtoRGB(float H, float S, float L);
COLOR HSVtoRGBf(float H, float S, float V);
// 绘图函数
COLOR getpixel(int x, int y); // 获取点的颜色
int putpixel(int x, int y, COLOR color); // 画点
int moveto(double x, double y); // 移动当前点(绝对坐标)
int moverel(double dx, double dy); // 移动当前点(相对坐标)
int line(double x1, double y1, double x2, double y2) // 画线
{
  gc_t* g = g_easyx->ctx;
  gcBeginPath(g);
  gcMoveTo(g, x1, y1);
  gcLineTo(g, x2, y2);
  gcStroke(g);
  return 0;
}
int linerel(double dx, double dy); // 画线(至相对坐标)
int lineto(double x, double y); // 画线(至绝对坐标)
// 画圆角矩形
int roundrect(double left, double top, double right, double bottom, double r);
// 画填充圆角矩形(有边框)
int fillroundrect_impl(double left, double top, double right, double bottom, double r, int dotype)
{
  gc_t* g = g_easyx->ctx;
  double t;
  r *= 0.5;
  gcBeginPath(g);
  if (left > right) {
    CV_SWAP(left, right, t);
  }
  if (top > bottom) {
    CV_SWAP(top, bottom, t);
  }
  gcRoundRect(g, left, top, right - left, bottom - top, r, r, r, r);
  gcClosePath(g);
  if (dotype & 1) {
    gcFill(g);
  }
  if (dotype & 2) {
    gcStroke(g);
  }
  return 0;
}
int rectangle_impl(double left, double top, double right, double bottom, int dotype)
{
  gc_t* g = g_easyx->ctx;
  gcBeginPath(g);
  gcRect(g, left, top, right - left, bottom - top);
  gcClosePath(g);
  if (dotype & 1) {
    gcFill(g);
  }
  if (dotype & 2) {
    gcStroke(g);
  }
  return 0;
}
// 画矩形
int rectangle(double left, double top, double right, double bottom)
{
  return rectangle_impl(left, top, right, bottom, 2);
}
// 画填充矩形(有边框)
int fillrectangle(double left, double top, double right, double bottom)
{
  return rectangle_impl(left, top, right, bottom, 3);
}
// 画填充矩形(无边框)
int solidrectangle(double left, double top, double right, double bottom)
{
  return rectangle_impl(left, top, right, bottom, 2);
}
int clearrectangle(double left, double top, double right, double bottom); // 清空矩形区域
int fillroundrect(double left, double top, double right, double bottom, double r)
{
  return fillroundrect_impl(left, top, right, bottom, r, 3);
}
// 画填充圆角矩形(无边框)
int solidroundrect(double left, double top, double right, double bottom, double r)
{
  return fillroundrect_impl(left, top, right, bottom, r, 1);
}
int arc_impl(double left, double top, double right, double bottom, double stangle, double endangle, int dotype, int close)
{
  gc_t* g = g_easyx->ctx;
  if (stangle > endangle) {
    //double t;
    //CV_SWAP(stangle, endangle, t);
  }
  gcBeginPath(g);
  gcArc(g, (right + left) * 0.5, (bottom + top) * 0.5, fabs(right - left) * 0.5, fabs(bottom - top) * 0.5, -endangle * 180 / PI, -stangle * 180 / PI);
  if (close) {
    gcClosePath(g);
  }
  if (dotype & 1) {
    gcFill(g);
  }
  if (dotype & 2) {
    gcStroke(g);
  }
  return 0;
}
// 画椭圆扇形(起始角度和终止角度为弧度制)
int pie(double left, double top, double right, double bottom, double stangle, double endangle)
{
  return arc_impl(left, top, right, bottom, stangle, endangle, 1, 1);
}
// 画椭圆弧(起始角度和终止角度为弧度制)
int arc(double left, double top, double right, double bottom, double stangle, double endangle)
{
  return arc_impl(left, top, right, bottom, stangle, endangle, 2, 0);
}
// 画填充椭圆扇形(有边框)
int fillpie(double left, double top, double right, double bottom, double stangle, double endangle)
{
  return arc_impl(left, top, right, bottom, stangle, endangle, 3, 1);
}
int solidpie(double left, double top, double right, double bottom, double stangle, double endangle); // 画填充椭圆扇形(无边框)
int clearpie(double left, double top, double right, double bottom, double stangle, double endangle); // 清空椭圆扇形区域
int ellipse(double left, double top, double right, double bottom); // 画椭圆
// 画填充椭圆(有边框)
int fillellipse(double left, double top, double right, double bottom)
{
  return arc_impl(left, top, right, bottom, 0, 2 * PI, 3, 1);
}
// 画填充椭圆(无边框)
int solidellipse(double left, double top, double right, double bottom)
{
  return arc_impl(left, top, right, bottom, 0, 2 * PI, 1, 1);
}
int clearellipse(double left, double top, double right, double bottom); // 清空椭圆形区域
// 画圆
int circle(double x, double y, double radius)
{
  return arc_impl(x - radius, y - radius, x + radius, y + radius, 0, 2 * PI, 2, 1);
}
// 画填充圆(有边框)
int fillcircle(double x, double y, double radius)
{
  return arc_impl(x - radius, y - radius, x + radius, y + radius, 0, 2 * PI, 3, 1);
}
// 画填充圆(无边框)
int solidcircle(double x, double y, double radius)
{
  return arc_impl(x - radius, y - radius, x + radius, y + radius, 0, 2 * PI, 1, 1);
}
// 清空圆形区域
int clearcircle(double x, double y, double radius);
int polyline(const IPOINT* points, int num); // 画多条连续的线
int polygon(const IPOINT* points, int num); // 画多边形
int fillpolygon(const IPOINT* points, int num); // 画填充的多边形(有边框)
int solidpolygon(const IPOINT* points, int num); // 画填充的多边形(无边框)
int clearpolygon(const IPOINT* points, int num); // 清空多边形区域
//int floodfill(int x, int y, int border); // 填充区域
// 文字相关函数
int outtext(const char* str); // 在当前位置输出字符串
int outchar(int c); // 在当前位置输出字符
int outtextxy(int x, int y, const char* str); // 在指定位置输出字符串
int outcharxy(int x, int y, int c); // 在指定位置输出字符
int textwidth(const char* str); // 获取字符串占用的像素宽
int charwidth(int c); // 获取字符占用的像素宽
int textheight(const char* str); // 获取字符串占用的像素高
int charheight(int c); // 获取字符占用的像素高
int drawtext(const char* str, IRECT* pRect, UINT uFormat); // 在指定区域内以指定格式输出字符串
int drawchar(int c, IRECT* pRect, UINT uFormat); // 在指定区域内以指定格式输出字符
// 设置当前字体样式(详见帮助)
// nHeight: 字符的平均高度；
// nWidth: 字符的平均宽度(0 表示自适应)；
// lpszFace: 字体名称；
// nEscapement: 字符串的书写角度(单位 0.1 度)；
// nOrientation: 每个字符的书写角度(单位 0.1 度)；
// nWeight: 字符的笔画粗细(0 表示默认粗细)；
// bItalic: 是否斜体；
// bUnderline: 是否下划线；
// bStrikeOut: 是否删除线；
// fbCharSet: 指定字符集；
// fbOutPrecision: 指定文字的输出精度；
// fbClipPrecision: 指定文字的剪辑精度；
// fbQuality: 指定文字的输出质量；
// fbPitchAndFamily: 指定以常规方式描述字体的字体系列。
int settextstyle(int nHeight, int nWidth, const char* lpszFace);
int settextstyle2(int nHeight, int nWidth, const char* lpszFace, int nEscapement, int nOrientation, int nWeight, BOOL bItalic, BOOL bUnderline, BOOL bStrikeOut);
int settextstyle3(int nHeight, int nWidth, const char* lpszFace, int nEscapement, int nOrientation, int nWeight, BOOL bItalic, BOOL bUnderline, BOOL bStrikeOut, BYTE fbCharSet, BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality, BYTE fbPitchAndFamily);
//int settextfont(const char* font); // 设置当前字体样式
//int gettextfont1(LOGFONT* font); // 获取当前字体样式
#include "softgc.inl"
#include "imui_win32.inl"
#include "imui_gdi.inl"
#include "imui_gdip.inl"
#include "imui_d3d9.inl"
#include "imui_gl2.inl"
#include "imui_soft.inl"

