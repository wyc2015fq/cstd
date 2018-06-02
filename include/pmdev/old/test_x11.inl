#include <X11/Xlib.h>

int test_x11()
{
  XSetWindowAttributes winattrs;
  int swidth;/*屏幕宽度*/
  int sheight;/*屏幕高度*/
  int winmask;
  Display* display;
  int screen;
  Window window;

  display = XOpenDisplay(getenv("DISPLAY"));
  screen = DefaultScreen(display);
  swidth = DisplayWidth(display, screen);/*获取屏幕宽度*/
  sheight = DisplayHeight(display, screen);/*获取屏幕高度*/
  window = XCreateSimpleWindow(display, XDefaultRootWindow(display), 0, 0, swidth, sheight, 0, 0, 0);
  winattrs.override_redirect = True;
  winmask = CWOverrideRedirect;/*指定只对override_redirect属性做改变*/
  XChangeWindowAttributes(display, window, winmask, &winattrs);

  XMapWindow(display, window);

  while (1) {
    /*event loop*/
  }

  return 0;
}
