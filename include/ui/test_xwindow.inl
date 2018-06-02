//gcc -o x-hello x-hello.c -lX11
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int test_xwindow0()
{
  Display* d;
  Window w;
  XEvent e;
  char* msg = "Hello, World!";
  int s;
  d = XOpenDisplay(NULL);
  if (d == NULL) {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  }
  s = DefaultScreen(d);
  w = XCreateSimpleWindow(d, RootWindow(d, s), 100, 100, 500, 500, 1,
      777215, 111111);
  printf("BlackPixel(d, s) is %d\n", (int)BlackPixel(d, s));
  printf("WhitePixel(d, s) is %d\n", (int)WhitePixel(d, s));
  XSelectInput(d, w, ExposureMask | KeyPressMask);
  XMapWindow(d, w);
  while (1) {
    XNextEvent(d, &e);
    if (e.type == Expose) {
      XFillRectangle(d, w, DefaultGC(d, s), 20, 20, 10, 10);
      XDrawString(d, w, DefaultGC(d, s), 10, 50, msg, strlen(msg));
    }
    if (e.type == KeyPress) {
      break;
    }
  }
  XCloseDisplay(d);
  return 0;
}
//例子一：
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <stdio.h>
void draw(Display* display, Window win, GC gc,
    XFontStruct* fs, XFontStruct* fs16)
{
  char* str1 = "Hello World!";
  char* str2 = "你好，大家??！";
  //参考线
  XDrawLine(display, win, gc, 20, 100, 300, 100);
  //画字符串
  XSetFont(display, gc, fs->fid);
  XDrawString(display, win, gc, 20, 100, str1, strlen(str1));
  //计算宽度
  printf("str1 width:%d\n", XTextWidth(fs, str1, strlen(str1)));
  //画中文字符串
  XSetFont(display, gc, fs16->fid);
  XDrawString16(display, win, gc, 200, 100, (XChar2b*)str2,
      strlen(str2) / 2);
  //计算宽度
  printf("str2 width:%d\n", XTextWidth16(fs16,
      (XChar2b*)str2, strlen(str2)));
}
int test_xwindow1()
{
  int i;
  Display* display;
  int screen_num;
  Window win; //窗口ID
  unsigned int width, height; //窗口尺寸
  unsigned int border_width = 4; //边界空白
  unsigned int display_width, display_height;//屏幕尺寸
  int count;
  XEvent report;
  GC gc;
  unsigned long valuemask = 0;
  XGCValues values;
  char* display_name = NULL;
  char** fonts;
  int font_count;
  XFontStruct* fs, *fs16;
  // 和X 服务器连接
  if ((display = XOpenDisplay(display_name)) == NULL) {
    printf("Cannot connect to X server %s\n",
        XDisplayName(display_name));
    exit(-1);
  }
  //获得缺省的 screen_num
  screen_num = DefaultScreen(display);
  //获得屏幕的宽度和高度
  display_width = DisplayWidth(display, screen_num);
  display_height = DisplayHeight(display, screen_num);
  //指定所建立窗口?宽度和高度
  //width = display_width/2;
  //height = display_height/2;
  width = 600;
  height = 400;
  //列出所有GB2312
  printf("All gb2312 fonts on this server:\n");
  fonts = XListFonts(display, "*gb2312*", 10000, &font_count);
  for (i = 0; i < font_count; i++) {
    printf("%s\n", fonts[i]);
  }
  XFreeFontNames(fonts);
  //载入8x16字体
  if ((fs = XLoadQueryFont(display, "9x15")) == NULL) {
    printf("Cannot load font 9x15\n");
    exit(1);
  }
  //载入中文16点阵字体
  if ((fs16 = XLoadQueryFont(display, "hanzigb16st")) == NULL) {
    printf("Cannot load Chinese font\n");
    exit(1);
  }
  //建立窗口
  win = XCreateSimpleWindow(display, //display
      RootWindow(display, screen_num), //父窗口
      0, 0, width, height, //位置和大小
      border_width, //边界宽度
      BlackPixel(display, screen_num), //前景色
      WhitePixel(display, screen_num)); //背景色
  //选择窗口感兴趣的事件掩码
  XSelectInput(display, win,
      ExposureMask | KeyPressMask |
      ButtonPressMask | StructureNotifyMask);
  //建立GC
  gc = XCreateGC(display, win, valuemask, &values);
  //显示窗口
  XMapWindow(display, win);
  //进入事件循环
  while (1) {
    //取得队列中的事件
    XNextEvent(display, &report);
    switch (report.type) {
      //曝光事件, 窗口应重绘
    case Expose:
      //取得最后一个曝光事件
      if (report.xexpose.count != 0) {
        break;
      }
      draw(display, win, gc, fs, fs16);
      break;
      //窗口尺寸改变, 重新取得窗口的宽度和高度
    case ConfigureNotify:
      width = report.xconfigure.width;
      height = report.xconfigure.height;
      break;
      //鼠标点击或有按键, 释放资源?退出
    case ButtonPress:
      XFreeGC(display, gc);
      XCloseDisplay(display);
      exit(1);
    case KeyPress:
      printf("report: %c\n", report.xkey.keycode);
      break;
    default:
      break;
    }
  }
}
//例子二：
#include <X11/Xlib.h>
#include <stdio.h>
int test_xwindow2()
{
  Display* d;
  Window w, wb;
  XEvent event;
  Font f;
  GC gc;
  d = XOpenDisplay(NULL);
  w = XCreateSimpleWindow(d, RootWindow(d, 0),
      180, 50,
      260, 160,
      1, WhitePixel(d, 0), BlackPixel(d, 0));
  XSelectInput(d, w, ExposureMask | ButtonPressMask);
  wb = XCreateSimpleWindow(d, w,
      20, 20,
      100, 20,
      1, WhitePixel(d, 0), BlackPixel(d, 0));
  XSelectInput(d, wb, ExposureMask | ButtonPressMask);
  gc = XCreateGC(d, w, 0, 0);
  f = XLoadFont(d, "fixed");
  XSetFont(d, gc, f);
  XMapWindow(d, w);
  XMapWindow(d, wb);
  XFlush(d);
  while (1) {
    XNextEvent(d, &event);
    switch (event.type) {
    case Expose:
      if (event.xany.window == wb) {
        XSetForeground(d, gc, BlackPixel(d, 0));
        XFillRectangle(d, w, gc, 0, 0, 260, 160);
        XSetForeground(d, gc, WhitePixel(d, 0));
        XDrawString(d, wb, gc, 10, 10, "push", 4); //把push换成中文例如”确 //定”？？？？？为什么编译完后，执行，不能出现中文
      }
      break;
    case ButtonPress:
      if (event.xany.window == w) {
        printf("w ");
        fflush(stdout);
      }
      if (event.xany.window == wb) {
        printf("wb ");
        fflush(stdout);
      }
      break;
    }
  }
  return 0;
}
//例子三：
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
/*
#include
*/
#include <X11/Xlocale.h>
void DrawCenteredMbString(Display*, Window, XFontSet, GC, char*, int, int,
    int, int, int);
void DrawCenteredMbString(Display* dpy, Window w, XFontSet fontset, GC gc, char* str, int num_bytes, int x, int y, int width, int height)
{
  XRectangle boundingbox;
  XRectangle dummy;
  int originx, originy;
  (void) XmbTextExtents(fontset, str, num_bytes, &dummy, &boundingbox);
  originx = x + (width - boundingbox.width) / 2 - boundingbox.x;
  originy = y + (height - boundingbox.height) / 2 - boundingbox.y;
  XmbDrawString(dpy, w, fontset, gc, originx, originy, str, num_bytes);
}
int test_xwindow3()
{
  Display* dpy;
  Window w;
  XFontSet fontset;
  char** missing_charsets;
  int num_missing_charsets;
  char* default_string;
  int i;
  GC gc;
  XEvent xe;
  char* program_name = "";
  if (setlocale(LC_ALL, "") == NULL) {
    (void) fprintf(stderr, "%s: cannot set locale.n",
        program_name);
    exit(1);
  }
  if (!XSupportsLocale()) {
    (void) fprintf(stderr, "%s: X does not support locale %s.n",
        program_name, setlocale(LC_ALL, NULL));
    exit(1);
  }
  if (XSetLocaleModifiers("") == NULL) {
    (void) fprintf(stderr,
        "%s: Warning: cannot set locale modifiers.n",
        program_name);
  }
  dpy = XOpenDisplay(NULL);
  w = XCreateSimpleWindow(dpy, RootWindow(dpy, 0), 50, 50,
      100, 50, 5, BlackPixel(dpy, 0),
      WhitePixel(dpy, 0));
  gc = XCreateGC(dpy, w, 0L, (XGCValues*) NULL);
  fontset = XCreateFontSet(dpy, "-*-*-*-*-*-*-16-*-*-*-*-*-*-*",
      &missing_charsets, &num_missing_charsets,
      &default_string);
  if (num_missing_charsets > 0) {
    (void) fprintf(stderr,
        "%s: The following charsets are missing:n",
        program_name);
    for (i = 0; i < num_missing_charsets; i++)
      (void)fprintf(stderr, "%s: %sn", program_name,
          missing_charsets);
    (void) fprintf(stderr, "%s: The string is %s", program_name,
        default_string);
    (void) fprintf(stderr,
        "%s: of any characters from those sets.n",
        program_name);
    XFreeStringList(missing_charsets);
  }
  XSetLineAttributes(dpy, gc, 5, LineSolid, CapRound, JoinRound);
  XSelectInput(dpy, w, ExposureMask | ButtonPressMask
      | EnterWindowMask | LeaveWindowMask);
  XMapWindow(dpy, w);
  XFlush(dpy);
  while (True) {
    XNextEvent(dpy, &xe);
    switch (xe.type) {
    case Expose:
      printf("Expose 出现.n");
      XSetForeground(dpy, gc, BlackPixel(dpy, 0));
      DrawCenteredMbString(dpy, w, fontset, gc,
          "你好 Hello",
          10, 0, 0, 100, 50);
      break;
    case ButtonPress:
      printf("按下!n");
      exit(1);
      break;
    case EnterNotify:
      printf("进入.n");
      XSetForeground(dpy, gc, BlackPixel(dpy, 0));
      XDrawRectangle(dpy, w, gc, 0, 0, 100, 50);
      break;
    case LeaveNotify:
      printf("离开.n");
      XSetForeground(dpy, gc, WhitePixel(dpy, 0));
      XDrawRectangle(dpy, w, gc, 0, 0, 100, 50);
      break;
    default:
      printf("默认.n");
      break;
    }
  }
  return 0;
}

