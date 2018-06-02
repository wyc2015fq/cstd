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
typedef struct {
  Display* dpy;
  Window root;
  Visual* vis;
  Colormap cmap;
  XWindowAttributes attr;
  XSetWindowAttributes swa;
  Window win;
  Cursor cursor;
  int screen;
  int width;
  int height;
  int (*handle_event)(ImGui* io, ImGuiHost* host, XEvent* evt);
} XWindow;
typedef struct {
  int ascent;
  int descent;
  int height;
  XFontSet set;
  XFontStruct* xfont;
  void* handle;
} XFont;
typedef struct {
  GC gc;
  Drawable drawable;
  unsigned int w, h;
  COLOR pen;
  COLOR brush;
  float lineWidth;
  float miterLimit;
  int lineCap;
  int lineJoin;
  int textAlign;
  dpath_t path[1];
  IRECT clip[1];
} ImGuiX11;
#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#endif
static void die(const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  fputs("\n", stderr);
  exit(EXIT_FAILURE);
}
static void* xcalloc(size_t siz, size_t n)
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
int nk_xlib_handle_event(ImGui* io, ImGuiHost* host, XEvent* evt)
{
  XWindow* s = (XWindow*)host->Window;
  Display* dpy = s->dpy;
  int screen = s->screen;
  Window win = s->win;
  /* optional grabbing behavior */
#if 0
  if (ctx->input.mouse.grab) {
    XDefineCursor(xlib.dpy, xlib.root, xlib.cursor);
    ctx->input.mouse.grab = 0;
  }
  else if (ctx->input.mouse.ungrab) {
    XWarpPointer(xlib.dpy, None, xlib.root, 0, 0, 0, 0,
        (int)ctx->input.mouse.prev.x, (int)ctx->input.mouse.prev.y);
    XUndefineCursor(xlib.dpy, xlib.root);
    ctx->input.mouse.ungrab = 0;
  }
#endif
  switch (evt->type) {
#if 0
  case KeyPress:
  case KeyRelease: {
    /* Key handler */
    int ret, down = (evt->type == KeyPress);
    KeySym* code = XGetKeyboardMapping(xlib.surf->dpy, (KeyCode)evt->xkey.keycode, 1, &ret);
    if (*code == XK_Shift_L || *code == XK_Shift_R) {
      nk_input_key(ctx, NK_KEY_SHIFT, down);
    }
    else if (*code == XK_Delete) {
      nk_input_key(ctx, NK_KEY_DEL, down);
    }
    else if (*code == XK_Return) {
      nk_input_key(ctx, NK_KEY_ENTER, down);
    }
    else if (*code == XK_Tab) {
      nk_input_key(ctx, NK_KEY_TAB, down);
    }
    else if (*code == XK_Left) {
      nk_input_key(ctx, NK_KEY_LEFT, down);
    }
    else if (*code == XK_Right) {
      nk_input_key(ctx, NK_KEY_RIGHT, down);
    }
    else if (*code == XK_Up) {
      nk_input_key(ctx, NK_KEY_UP, down);
    }
    else if (*code == XK_Down) {
      nk_input_key(ctx, NK_KEY_DOWN, down);
    }
    else if (*code == XK_BackSpace) {
      nk_input_key(ctx, NK_KEY_BACKSPACE, down);
    }
    else if (*code == XK_Escape) {
      nk_input_key(ctx, NK_KEY_TEXT_RESET_MODE, down);
    }
    else if (*code == XK_Page_Up) {
      nk_input_key(ctx, NK_KEY_SCROLL_UP, down);
    }
    else if (*code == XK_Page_Down) {
      nk_input_key(ctx, NK_KEY_SCROLL_DOWN, down);
    }
    else if (*code == XK_Home) {
      nk_input_key(ctx, NK_KEY_TEXT_START, down);
      nk_input_key(ctx, NK_KEY_SCROLL_START, down);
    }
    else if (*code == XK_End) {
      nk_input_key(ctx, NK_KEY_TEXT_END, down);
      nk_input_key(ctx, NK_KEY_SCROLL_END, down);
    }
    else {
      if (*code == 'c' && (evt->xkey.state & ControlMask)) {
        nk_input_key(ctx, NK_KEY_COPY, down);
      }
      else if (*code == 'v' && (evt->xkey.state & ControlMask)) {
        nk_input_key(ctx, NK_KEY_PASTE, down);
      }
      else if (*code == 'x' && (evt->xkey.state & ControlMask)) {
        nk_input_key(ctx, NK_KEY_CUT, down);
      }
      else if (*code == 'z' && (evt->xkey.state & ControlMask)) {
        nk_input_key(ctx, NK_KEY_TEXT_UNDO, down);
      }
      else if (*code == 'r' && (evt->xkey.state & ControlMask)) {
        nk_input_key(ctx, NK_KEY_TEXT_REDO, down);
      }
      else if (*code == XK_Left && (evt->xkey.state & ControlMask)) {
        nk_input_key(ctx, NK_KEY_TEXT_WORD_LEFT, down);
      }
      else if (*code == XK_Right && (evt->xkey.state & ControlMask)) {
        nk_input_key(ctx, NK_KEY_TEXT_WORD_RIGHT, down);
      }
      else if (*code == 'b' && (evt->xkey.state & ControlMask)) {
        nk_input_key(ctx, NK_KEY_TEXT_LINE_START, down);
      }
      else if (*code == 'e' && (evt->xkey.state & ControlMask)) {
        nk_input_key(ctx, NK_KEY_TEXT_LINE_END, down);
      }
      else {
        if (*code == 'i') {
          nk_input_key(ctx, NK_KEY_TEXT_INSERT_MODE, down);
        }
        else if (*code == 'r') {
          nk_input_key(ctx, NK_KEY_TEXT_REPLACE_MODE, down);
        }
        if (down) {
          char buf[32];
          KeySym keysym = 0;
          if (XLookupString((XKeyEvent*)evt, buf, 32, &keysym, NULL) != NoSymbol) {
            nk_input_glyph(ctx, buf);
          }
        }
      }
    }
    XFree(code);
    return 1;
  }
  break;
#endif
  case ButtonPress:
  case ButtonRelease: {
    /* Button handler */
    int down = (evt->type == ButtonPress);
    const int x = evt->xbutton.x, y = evt->xbutton.y;
    io->MousePos.x = x;
    io->MousePos.y = y;
    switch (evt->xbutton.button) {
    case Button1:
      printf("x=%d y=%d %d\n", x, y, down);
      io->MouseDown[0] = down;
      io->lastMouseDownPos = io->MousePos;
      io->lastMsg = down ? MSG_LBUTTONDOWN : MSG_LBUTTONUP;
      break;
    case Button2:
      io->MouseDown[3] = down;
      io->lastMouseDownPos = io->MousePos;
      io->lastMsg = down ? MSG_MBUTTONDOWN : MSG_MBUTTONUP;
      break;
    case Button3:
      io->MouseDown[2] = down;
      io->lastMouseDownPos = io->MousePos;
      io->lastMsg = down ? MSG_RBUTTONDOWN : MSG_RBUTTONUP;
      break;
    case Button4:
      io->lastMsg = MSG_MOUSEWHEEL;
      io->MouseWheel += 1.0f;
      break;
    case Button5:
      io->lastMsg = MSG_MOUSEWHEEL;
      io->MouseWheel += -1.0f;
      break;
    default:
      return 0;
    }
    return 1;
  }
  break;
  case MotionNotify: {
    /* Mouse motion handler */
    const int x = evt->xmotion.x, y = evt->xmotion.y;
    io->MousePos.x = x;
    io->MousePos.y = y;
    io->lastMsg = MSG_MOUSEMOVE;
#if 0
    if (ctx->input.mouse.grabbed) {
      ctx->input.mouse.pos.x = ctx->input.mouse.prev.x;
      ctx->input.mouse.pos.y = ctx->input.mouse.prev.y;
      XWarpPointer(xlib.dpy, None, xlib.surf->root, 0, 0, 0, 0, (int)ctx->input.mouse.pos.x, (int)ctx->input.mouse.pos.y);
    }
#endif
    return 1;
  }
  break;
  case KeymapNotify: {
    XRefreshKeyboardMapping(&evt->xmapping);
    return 1;
  }
  break;
  }
  if (s && s->handle_event) {
    return s->handle_event(io, host, evt);
  }
  return 0;
}
int imuiLoop(int delay)
{
  long dt, started = timestamp();
  XEvent evt;
  GETGUIIO1();
  XWindow* s;
  int i;
  host = io->HeadHost;
  s = (XWindow*)host->Window;
  for (i = 0; i < 5; ++i) {
    if (!io->MouseDown[i]) {
      io->MouseDownID[i] = NULL;
    }
  }
  while (XCheckWindowEvent(s->dpy, s->win, s->swa.event_mask, &evt)) {
    if (XFilterEvent(&evt, s->win)) {
      continue;
    }
  }
  if (io->HeadHost) {
    host = io->HeadHost;
    nk_xlib_handle_event(io, host, &evt);
  }
  dt = timestamp() - started;
  if (dt < delay) {
    sleep_for(delay - dt);
  }
  return 1;
}
int imuiLoop1(int delay)
{
  XEvent evt;
  GETGUIIO1();
  long time0 = timestamp(), finded = 0;
  XWindow* s;
  host = io->HeadHost;
  s = (XWindow*)host->Window;
  io->HotHost = NULL;
  if (delay > 0) {
    while (!finded && abs((int)(timestamp() - time0)) < delay) {
      for (host = io->HeadHost; !finded && host; host = host->next) {
        XWindow* s = (XWindow*)host->Window;
        if (XCheckWindowEvent(s->dpy, s->win, s->swa.event_mask, &evt)) {
          if (XFilterEvent(&evt, s->win)) {
            continue;
          }
          finded = 1;
          io->HotHost = host;
          break;
        }
      }
    }
  }
  else {
    for (host = io->HeadHost; !finded && host; host = host->next) {
      XWindow* s = (XWindow*)host->Window;
      XNextEvent(s->dpy, &evt);
      if (!XFilterEvent(&evt, s->win)) {
        finded = 1;
        io->HotHost = host;
        break;
      }
      sleep_for(100);
    }
  }
  if (finded && io->HeadHost) {
    host = io->HeadHost;
    nk_xlib_handle_event(io, host, &evt);
  }
  return 1;
}
int imuiUnInit()
{
  return 0;
}
int ImGuiHost_Init(ImGuiHost* host, ImGuiDriverInit_t drv_init, const char* lpWindowName, int x, int y, int nWidth, int nHeight, COLOR clrbg)
{
  XWindow* s = NULL;
  imuiReAlloc(s, 1);
  /* X11 */
  host->clrbg = clrbg;
  s->dpy = XOpenDisplay(NULL);
  if (!s->dpy) {
    printf("Could not open a display; perhaps $DISPLAY is not set?");
  }
  strcpy(io->def_font_name, "fixed");
  io->def_font_size = 13;
  s->root = DefaultRootWindow(s->dpy);
  s->screen = XDefaultScreen(s->dpy);
  s->vis = XDefaultVisual(s->dpy, s->screen);
  s->cmap = XCreateColormap(s->dpy, s->root, s->vis, AllocNone);
  s->swa.colormap = s->cmap;
  s->swa.event_mask =
      ExposureMask | KeyPressMask | KeyReleaseMask |
      ButtonPress | ButtonRelease | ButtonReleaseMask | ButtonMotionMask |
      Button1MotionMask | Button3MotionMask | Button4MotionMask | Button5MotionMask |
      PointerMotionMask | KeymapStateMask;
  s->win = XCreateWindow(s->dpy, s->root, 0, 0, nWidth, nHeight, 0,
      XDefaultDepth(s->dpy, s->screen), InputOutput,
      s->vis, CWEventMask | CWColormap, &s->swa);
  XStoreName(s->dpy, s->win, lpWindowName);
  XMapWindow(s->dpy, s->win);
  XGetWindowAttributes(s->dpy, s->win, &s->attr);
  s->width = (int)s->attr.width;
  s->height = (int)s->attr.height;
  host->Window = s;
  if (!drv_init(host)) {
  }
  ImGuiHost_Add(host);
  return 0;
}

