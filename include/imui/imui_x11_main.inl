#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <X11/Xlocale.h>
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#define NK_XLIB_IMPLEMENTATION
//#include "../../nuklear.h"
//#include "nuklear_xlib.h"
#define DTIME 20
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define UNUSED(a) (void)a
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define LEN(a) (sizeof(a)/sizeof(a)[0])
typedef struct XWindow XWindow;
struct XWindow {
  Display* dpy;
  Window root;
  Visual* vis;
  Colormap cmap;
  XWindowAttributes attr;
  XSetWindowAttributes swa;
  Window win;
  int screen;
  //XFont *font;
  unsigned int width;
  unsigned int height;
};
typedef struct {
  GC gc;
  Drawable drawable;
  unsigned int w, h;
} ImGuiX11;
static void
die(const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  fputs("\n", stderr);
  exit(EXIT_FAILURE);
}
static void*
xcalloc(size_t siz, size_t n)
{
  void* ptr = calloc(siz, n);
  if (!ptr) {
    die("Out of memory\n");
  }
  return ptr;
}
static long timestamp(void)
{
  struct timeval tv;
  if (gettimeofday(&tv, NULL) < 0) {
    return 0;
  }
  return (long)((long)tv.tv_sec * 1000 + (long)tv.tv_usec / 1000);
}
static void sleep_for(long t)
{
  struct timespec req;
  const time_t sec = (int)(t / 1000);
  const long ms = t - (sec * 1000);
  req.tv_sec = sec;
  req.tv_nsec = ms * 1000000L;
  while (-1 == nanosleep(&req, &req));
}
int main(void)
{
  long dt;
  long started;
  int running = 1;
  XWindow xw;
  ImGuiX11* surf = NULL;
  XWindow* s = &xw;
  /* X11 */
  memset(&xw, 0, sizeof xw);
  xw.dpy = XOpenDisplay(NULL);
  if (!xw.dpy) {
    die("Could not open a display; perhaps $DISPLAY is not set?");
  }
  xw.root = DefaultRootWindow(xw.dpy);
  xw.screen = XDefaultScreen(xw.dpy);
  xw.vis = XDefaultVisual(xw.dpy, xw.screen);
  xw.cmap = XCreateColormap(xw.dpy, xw.root, xw.vis, AllocNone);
  xw.swa.colormap = xw.cmap;
  xw.swa.event_mask =
      ExposureMask | KeyPressMask | KeyReleaseMask |
      ButtonPress | ButtonReleaseMask | ButtonMotionMask |
      Button1MotionMask | Button3MotionMask | Button4MotionMask | Button5MotionMask |
      PointerMotionMask | KeymapStateMask;
  xw.win = XCreateWindow(xw.dpy, xw.root, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0,
      XDefaultDepth(xw.dpy, xw.screen), InputOutput,
      xw.vis, CWEventMask | CWColormap, &xw.swa);
  XStoreName(xw.dpy, xw.win, "X11");
  XMapWindow(xw.dpy, xw.win);
  XGetWindowAttributes(xw.dpy, xw.win, &xw.attr);
  xw.width = (unsigned int)xw.attr.width;
  xw.height = (unsigned int)xw.attr.height;
  {
    surf = (ImGuiX11*)calloc(1, sizeof(ImGuiX11));
    int w = s->width, h = s->height;
    if (!setlocale(LC_ALL, "")) {
      return 0;
    }
    if (!XSupportsLocale()) {
      return 0;
    }
    if (!XSetLocaleModifiers("@im=none")) {
      return 0;
    }
    /* create invisible cursor */
    surf->w = w;
    surf->h = h;
    surf->gc = XCreateGC(s->dpy, s->root, 0, NULL);
    XSetLineAttributes(s->dpy, surf->gc, 1, LineSolid, CapButt, JoinMiter);
    surf->drawable = XCreatePixmap(s->dpy, s->root, w, h, (unsigned int)DefaultDepth(s->dpy, s->screen));
  }
  while (running) {
    /* Input */
    XEvent evt;
    started = timestamp();
    while (XCheckWindowEvent(xw.dpy, xw.win, xw.swa.event_mask, &evt)) {
      if (XFilterEvent(&evt, xw.win)) {
        continue;
      }
      //nk_xlib_handle_event(xw.dpy, xw.screen, xw.win, &evt);
    }
    /* Draw */
    XClearWindow(xw.dpy, xw.win);
    XSetForeground(s->dpy, surf->gc, 0xff112233);
    XFillRectangle(s->dpy, surf->drawable, surf->gc, 0, 0, surf->w, surf->h);
    XSetForeground(s->dpy, surf->gc, 0xffff0000);
    XFillRectangle(s->dpy, surf->drawable, surf->gc, 100, 100, 200, 200);
    XCopyArea(s->dpy, surf->drawable, s->win, surf->gc, 0, 0, surf->w, surf->h, 0, 0);
    XFlush(xw.dpy);
    /* Timing */
    dt = timestamp() - started;
    if (dt < DTIME) {
      sleep_for(DTIME - dt);
    }
  }
  XFreePixmap(s->dpy, surf->drawable);
  XFreeGC(s->dpy, surf->gc);
  free(surf);
  //XFreeCursor(s->dpy, s->cursor);
  //XUnmapWindow(xw.dpy, xw.win);
  XFreeColormap(xw.dpy, xw.cmap);
  XDestroyWindow(xw.dpy, xw.win);
  XCloseDisplay(xw.dpy);
  return 0;
}

