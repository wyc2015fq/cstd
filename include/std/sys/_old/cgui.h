

////////////////////////////////////////////////////////////////////////////////

#define WINBMPCN 4


typedef enum {
    //KEY_ENTER = '\n', // FOR TEXT EDIT
    //KEY_TAB, // FOR TABBING THROUGH FIELDS
    KEY_LEFT, // FOR TEXT EDIT
    KEY_RIGHT,// FOR TEXT EDIT
    KEY_UP, // FOR TEXT EDIT
    KEY_DOWN, // FOR TEXT EDIT
    KEY_PAGEUP,
    KEY_PAGEDOWN,
    KEY_HOME, // FOR TEXT EDIT
    KEY_END, // FOR TEXT EDIT
    KEY_DELETE, // FOR TEXT EDIT
    KEY_BACK, // FOR TEXT EDIT
    KEY_ESCAPE, // FOR TEXT EDIT
    //KEY_CTRL,
    //KEY_SHIFT,
    //KEY_ALT,
    //KEY_SUPER,
    KEY_COUNT
} KeyCode;
typedef enum {
  MOUSE_LBUTTON,
  MOUSE_MBUTTON,
  MOUSE_RBUTTON,
} MouseCode;
enum {
  MSG_FLAG_LBUTTON = 1,
    MSG_FLAG_RBUTTON = 2,
    MSG_FLAG_MBUTTON = 4,
    MSG_FLAG_CTRLKEY = 8,
    MSG_FLAG_SHIFTKEY = 16,
    MSG_FLAG_ALTKEY = 32
};
typedef enum WindowMsg_ {
  MSG_NOP,
    MSG_TIMER,
#if 1
    MSG_LBUTTONDOWN,
    MSG_LBUTTONUP,
    MSG_RBUTTONDOWN,
    MSG_RBUTTONUP,
    MSG_MBUTTONDOWN,
    MSG_MBUTTONUP,
    MSG_LBUTTONDBLCLK,
    MSG_RBUTTONDBLCLK,
    MSG_MBUTTONDBLCLK,
#else
    MSG_MOUSEDOWN,
    MSG_MOUSEUP,
#endif
    MSG_MOUSEWHEEL,
    MSG_MOUSEWHEELUP,
    MSG_MOUSEWHEELDOWN,
    MSG_MOUSEMOVE,
    MSG_KEYDOWN,
    MSG_SIZE,
    MSG_KEYUP,
    MSG_CHAR,
    MSG_CLOSE,
} WindowMsg;

typedef struct {
  IRECT rc;
  ISIZE sz;
} ctrl_t;

typedef struct {
  IRECT margin, border, padding, content;
} box_t;

typedef struct {
  IRECT rect;
  IRECT rcLT;
  IRECT rcRB;
  IPOINT Size;
  IPOINT Pos;
  BOOL horz;
  int irc;
  int nrcs;
  int gap;
  int ilsz;
  int layoutflags;
} layerinfo_t;

typedef struct {
  int deep;
  layerinfo_t layinfo[16];
  IRECT rcs[100];
  int nrcs;
  int gap;
  int layoutflags;
} layer_t;


typedef enum {
  ImDrawCmd_NOP,
#define GCFUNDEF(RET, NAME, ARG)   ImDrawCmd_##NAME,
    GCFUNDEF_DEF(GCFUNDEF)
#undef GCFUNDEF
} DrawCmd;

typedef struct render_drv_t_ render_drv_t;
struct render_drv_t_ {
  void* x;
  IRECT rcDirty;
  int (*UnInit)(render_drv_t* drv);
  int (*Render)(render_drv_t* drv, const gc_t* ctx);
  int (*SetSize)(render_drv_t* drv, int w, int h);
  int (*SetTexture)(render_drv_t* drv, texture_t* tex, PixFmt fmt, int height, int width, void* data, int step);
};
typedef struct window_t_ window_t; //
struct window_t_ {
  IRECT rc;
  char text[16];
  bool show;
  ctrl_t client[1];
  window_t* prev;
  window_t* next;
  window_t* parent;
  window_t* child;
  gc_t g[1];
};
typedef struct window_native_t window_native_t;
typedef struct window_dc_t window_dc_t;
typedef struct window_host_t_ window_host_t;
typedef struct sysio_t_ sysio_t;
struct window_host_t_ {
  window_native_t* x;
  void* user;
  IRECT rect;
  bool open;
  char name[64];
  bool flip_y;
  render_drv_t drv[1];
  window_t root[1];
  window_t tooltip[1];
  window_t* hot_win;
  int (*event_fun)(sysio_t* io);
  window_host_t* prev, * next;
};
typedef struct sysmouse_t_ {
  IPOINT pos[10];
  int64 clicked_time[5];
  bool double_clicked[5];
  bool down[5];
  //void* downid[5];
  double dbclick_time;
  double wheelDelta;
  int code;
} sysmouse_t;
//#define keyshift()  io->keydown[KEY_SHIFT]
//#define keyctrl()   io->keydown[KEY_CTRL]
struct sysio_t_ {
  WindowMsg msg;
  int last_key;
  int last_char;
  bool keyshift;
  bool keyctrl;
  bool keyalt;
  uchar keydown[256];
  bool tick;
  int time;
  sysmouse_t mouse[1];
  window_host_t* root;
  window_host_t* cur_host;
  window_host_t* msg_host;
  window_t* cur_win;
  window_t* foc_win;
  //texfont_t font[1];
  font_t* font;
  //gc_t g[1];
  //layer_t la[1];
  void* hotid;
  void* hitid;
  int hitvalue;
  void* focid;
  const char** res;
  const char* cursor;
  gc_t* g;
};
#define ISHOT(c) (io->hotid==(c))
#define SETHOT(c) (io->hotid=(c))
#define SETHIT(c) (io->hitid = (c))
#define SETHOT2(c, hot) (io->hotid=(hot) ? (c) : NULL)
#define ISHIT(c) (io->hitid==(c))
#define SETHIT1(c) (io->hitid=(c))
#define SETHITV(v) (io->hitvalue = v)
#define GETHITV() (io->hitvalue)
#define ISFOCUS(c) (io->focid==(c))
#define SETFOCUS(c) (io->focid=(c))
#define SETFOCUSWIN(c) (io->foc_win=(c))
#define ISFOCUSWIN(c) (io->foc_win==(c))
#define FORCE_REDRAW(rc) (gcSetDirty(g, rc, false))
//#define ISHOVEREDWIN(c) (io->cur_host->hot_win==(c))
//#define ISHOVEREDWIN2() (io->cur_host->hot_win==io->cur_win)
#define ISHOVEREDWIN2() (1)

//#define SYSIO  sysio_t*io = sys_getio(); window_host_t* host = io->cur_host; gc_t* g = io->g; layer_t* la = io->la; sysmouse_t* mouse = io->mouse; int x = mouse->pos->x, y = mouse->pos->y
#define SYSIO  sysio_t*io = sys_getio(); gc_t* g = io->g; sysmouse_t* mouse = io->mouse; int x = mouse->pos->x, y = mouse->pos->y

CC_INLINE sysio_t* sys_getio() {
  static sysio_t io[1];
  return io;
}
static int sys_waitio(int delay);
static window_native_t* sys_create_window(int flag, const char* name, int x, int y, int w, int h, window_host_t* host);
static int sys_get_window_rect(window_native_t* w, IRECT* pwindow, IRECT* pclient);
static int sys_set_window_rect(window_native_t* w, IRECT* pwindow, IRECT* pclient);
static int sys_set_window_text(window_native_t* w, const char* text);
static int sys_del_dc(window_dc_t* dc);
static int sys_window_bitblt(window_native_t* dst_win, int x, int y, int w, int h, window_dc_t* src_dc, int src_x, int src_y);
static int sys_dc_set_size(window_dc_t** pdc, int h, int w, int cn);
static int sys_dc_get_image(window_dc_t* pdc, img_t* im);
////////////////////////////////////////////////////////////////////////////////
typedef struct {
  window_native_t* x;
  window_dc_t* hMemDC;
  img_t im[1];
  img_t im1[1];
} null_drv_t;


////////////////////////////////////////////////////////////////////////////////
CC_INLINE int img_setbitmap_cn4(int h, int w, const void* data, int step, int cn, PixFmt fmt, bool flip_y, bool scaling, uchar* outdata, int outstep, const void* pal) {
  int i, j;
  const uchar* srcarr = (const uchar*)data;
  double smin[4] = {DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX};
  double smax[4] = {DBL_MIN, DBL_MIN, DBL_MIN, DBL_MIN};
  double su[4] = {1, 1, 1, 1};
  if (h<=0 || w<=0) {
    return 0;
  }
  switch(fmt) {
  case PF_64bppGrayDouble:
    {
      typedef double arrtype;
      ASSERT(cn>=(int)sizeof(arrtype));
      if (scaling) {
        for (i = 0; i < h; ++i) {
          const uchar* src1 = (const uchar*)(srcarr + i * step);
          for (j = 0; j < w; ++j, src1+=cn) {
            arrtype s = *(arrtype*)src1;
            if (s<smin[0]) { smin[0] = s; } else if (s>smax[0]) { smax[0] = s; }
          }
        }
        su[0] = 255. / (smax[0] - smin[0]);
      }
      for (i = 0; i < h; ++i) {
        uchar* dst1 = outdata + (flip_y ? (h-1-i) : i) * outstep;
        const uchar* src1 = (const uchar*)(srcarr + i * step);
        for (j = 0; j < w; ++j, dst1 += 4, src1+=cn) {
          arrtype s = *(arrtype*)src1;
          int t = (int)((s - smin[0]) * su[0]);
          int gry = BOUND(t, 0, 255);
          dst1[0] = gry;
          dst1[1] = gry;
          dst1[2] = gry;
          dst1[3] = 255;
        }
      }
    }
    break;
  case PF_32bppGrayFloat:
    {
      typedef float arrtype;
      ASSERT(cn>=(int)sizeof(arrtype));
      if (scaling) {
        for (i = 0; i < h; ++i) {
          const uchar* src1 = (const uchar*)(srcarr + i * step);
          for (j = 0; j < w; ++j, src1+=cn) {
            arrtype s = *(arrtype*)src1;
            if (s<smin[0]) { smin[0] = s; } else if (s>smax[0]) { smax[0] = s; }
          }
        }
        su[0] = 255. / (smax[0] - smin[0]);
      }
      for (i = 0; i < h; ++i) {
        uchar* dst1 = outdata + (flip_y ? (h-1-i) : i) * outstep;
        const uchar* src1 = (const uchar*)(srcarr + i * step);
        for (j = 0; j < w; ++j, dst1 += 4, src1+=cn) {
          arrtype s = *(arrtype*)src1;
          int t = (int)((s - smin[0]) * su[0]);
          int gry = BOUND(t, 0, 255);
          dst1[0] = gry;
          dst1[1] = gry;
          dst1[2] = gry;
          dst1[3] = 255;
        }
      }
    }
    break;
  case PF_16bppGray:
    {
      typedef short arrtype;
      ASSERT(cn>=(int)sizeof(arrtype));
      if (scaling) {
        for (i = 0; i < h; ++i) {
          const uchar* src1 = (const uchar*)(srcarr + i * step);
          for (j = 0; j < w; ++j, src1+=cn) {
            arrtype s = *(arrtype*)src1;
            if (s<smin[0]) { smin[0] = s; } else if (s>smax[0]) { smax[0] = s; }
          }
        }
        su[0] = 256. / (smax[0] - smin[0]);
      }
      for (i = 0; i < h; ++i) {
        uchar* dst1 = outdata + (flip_y ? (h-1-i) : i) * outstep;
        const uchar* src1 = (const uchar*)(srcarr + i * step);
        for (j = 0; j < w; ++j, dst1 += 4, src1+=cn) {
          arrtype s = *(arrtype*)src1;
          int t = (int)((s - smin[0]) * su[0]);
          int gry = BOUND(t, 0, 255);
          dst1[0] = gry;
          dst1[1] = gry;
          dst1[2] = gry;
          dst1[3] = 255;
        }
      }
    }
    break;
  case PF_8bppGray:
    {
      typedef uchar arrtype;
      ASSERT(cn>=(int)sizeof(arrtype));
      if (scaling) {
        for (i = 0; i < h; ++i) {
          const uchar* src1 = (const uchar*)(srcarr + i * step);
          for (j = 0; j < w; ++j, src1+=cn) {
            arrtype s = *(arrtype*)src1;
            if (s<smin[0]) { smin[0] = s; } else if (s>smax[0]) { smax[0] = s; }
          }
        }
        su[0] = 256. / (smax[0] - smin[0]);
      }
      for (i = 0; i < h; ++i) {
        uchar* dst1 = outdata + (flip_y ? (h-1-i) : i) * outstep;
        const uchar* src1 = (const uchar*)(srcarr + i * step);
        for (j = 0; j < w; ++j, dst1 += 4, src1+=cn) {
          arrtype s = *(arrtype*)src1;
          int t = (int)((s - smin[0]) * su[0]);
          int gry = BOUND(t, 0, 255);
          dst1[0] = gry;
          dst1[1] = gry;
          dst1[2] = gry;
          dst1[3] = 255;
        }
      }
    }
    break;
  case PF_1bppIndexed:
    for (i = 0; i < h; ++i) {
      uchar* dst1 = outdata + (flip_y ? (h-1-i) : i) * outstep;
      const uchar* src1 = (const uchar*)(srcarr + i * step);
      for (j = 0; j < w; ++j, dst1 += 4) {
        //int t = (src1[j>>3]>>(j&7))&1;
        int t = (src1[j>>3]>>(7-(j&7)))&1;
        int gry = t ? 255 : 0;
        dst1[0] = gry;
        dst1[1] = gry;
        dst1[2] = gry;
        dst1[3] = 255;
      }
    }
    break;
  case PF_24bppRGB:
    for (i = 0; i < h; ++i) {
      uchar* dst1 = outdata + (flip_y ? (h-1-i) : i) * outstep;
      const uchar* src1 = (const uchar*)(srcarr + i * step);
      for (j = 0; j < w; ++j, dst1 += 4, src1+=cn) {
        dst1[0] = src1[0];
        dst1[1] = src1[1];
        dst1[2] = src1[2];
        dst1[3] = 255;
      }
    }
    break;
  case PF_32bppPARGB:
    for (i = 0; i < h; ++i) {
      uchar* dst1 = outdata + (flip_y ? (h-1-i) : i) * outstep;
      const uchar* src1 = (const uchar*)(srcarr + i * step);
      for (j = 0; j < w; ++j, dst1 += 4, src1+=cn) {
        uchar a = src1[3];
        dst1[0] = src1[0]*a/255;
        dst1[1] = src1[1]*a/255;
        dst1[2] = src1[2]*a/255;
        dst1[3] = 255;
      }
    }
  break;
  case PF_32bppARGB:
  case PF_32bppRGB:
    for (i = 0; i < h; ++i) {
      uint32* dst1 = (uint32*)(outdata + (flip_y ? (h-1-i) : i) * outstep);
      const uint32* src1 = (const uint32*)(srcarr + i * step);
      for (j = 0; j < w; ++j) {
        *dst1++ = *src1++;
      }
    }
    break;
  case PF_128bppARGBFloat:
    for (i = 0; i < h; ++i) {
      uchar* dst1 = (uchar*)(outdata + (flip_y ? (h-1-i) : i) * outstep);
      const float* src1 = (const float*)(srcarr + i * step);
      for (j = 0; j < w; ++j) {
        dst1[0] = (uchar)(BOUND(src1[0], 0, 1)*255);
        dst1[1] = (uchar)(BOUND(src1[1], 0, 1)*255);
        dst1[2] = (uchar)(BOUND(src1[2], 0, 1)*255);
        dst1[3] = (uchar)(BOUND(src1[3], 0, 1)*255);
        dst1 += 4;
        src1 += 4;
      }
    }
    break;
  case PF_128bppABGRFloat:
    for (i = 0; i < h; ++i) {
      uchar* dst1 = (uchar*)(outdata + (flip_y ? (h-1-i) : i) * outstep);
      const float* src1 = (const float*)(srcarr + i * step);
      for (j = 0; j < w; ++j) {
        dst1[0] = (uchar)(BOUND(src1[2], 0, 1)*255);
        dst1[1] = (uchar)(BOUND(src1[1], 0, 1)*255);
        dst1[2] = (uchar)(BOUND(src1[0], 0, 1)*255);
        dst1[3] = (uchar)(BOUND(src1[3], 0, 1)*255);
        dst1 += 4;
        src1 += 4;
      }
    }
    break;
  case PF_96bppBGRFloat:
    if (scaling) {
      for (i = 0; i < h; ++i) {
        const float* src1 = (const float*)(srcarr + i * step);
        for (j = 0; j < w; ++j, src1+=3) {
          MINMAX_UPDATE(src1[0], smin[0], smax[0]);
          MINMAX_UPDATE(src1[1], smin[1], smax[1]);
          MINMAX_UPDATE(src1[2], smin[2], smax[2]);
        }
      }
      su[0] = 256. / (smax[0] - smin[0]);
      su[1] = 256. / (smax[1] - smin[1]);
      su[2] = 256. / (smax[2] - smin[2]);
    }
    for (i = 0; i < h; ++i) {
      uchar* dst1 = (uchar*)(outdata + (flip_y ? (h-1-i) : i) * outstep);
      const float* src1 = (const float*)(srcarr + i * step);
      for (j = 0; j < w; ++j) {
        double t0 = ((src1[0] - smin[0]) * su[0]);
        double t1 = ((src1[1] - smin[1]) * su[1]);
        double t2 = ((src1[2] - smin[2]) * su[2]);
        dst1[0] = (uchar)(BOUND(t0, 0, 255));
        dst1[1] = (uchar)(BOUND(t1, 0, 255));
        dst1[2] = (uchar)(BOUND(t2, 0, 255));
        dst1[3] = 255;
        dst1 += 4;
        src1 += 3;
      }
    }
    break;
  case PF_8bppIndexed:
    if (pal) {
      const uint32* upal = (const uint32*)pal;
      for (i = 0; i < h; ++i) {
        uint32* dst1 = (uint32*)(outdata + (flip_y ? (h-1-i) : i) * outstep);
        const uchar* src1 = (const uchar*)(srcarr + i * step);
        for (j = 0; j < w; ++j) {
          uchar t = src1[j];
          dst1[j] = upal[t];
        }
      }
    } else {
      for (i = 0; i < h; ++i) {
        uchar* dst1 = (uchar*)(outdata + (flip_y ? (h-1-i) : i) * outstep);
        const uchar* src1 = (const uchar*)(srcarr + i * step);
        for (j = 0; j < w; ++j) {
          uchar t = src1[j];
          dst1[0] = t;
          dst1[1] = t;
          dst1[2] = t;
          dst1[3] = 255;
          dst1 += 4;
        }
      }
    }
    break;
  default:
    ASSERT(0);
    break;
  }
  return 0;
}


enum { CC_WINDOW_AUTOSIZE = 1 };
// YV
enum {
  CC_WND_PROP_FULLSCREEN = 0,
    CC_WND_PROP_AUTOSIZE = 1,
    CC_WINDOW_NORMAL = 0,
    CC_WINDOW_FULLSCREEN = 1
};

CC_INLINE int mouse_proc_default(int event, int x, int y, int flags, void* param) {
  sysio_t* io = sys_getio();
  io->msg = (WindowMsg)event;
  io->mouse->pos->x = x;
  io->mouse->pos->y = y;
  UNUSED(param);
  UNUSED(flags);
  switch (io->msg) {
  case MSG_LBUTTONDOWN:
    io->msg = (WindowMsg)event;
    break;
  case MSG_LBUTTONUP:
    io->msg = (WindowMsg)event;
    break;
  default:
    break;
  }
  return 0;
}

#define event_def_def(event_def) \
  event_def(abort,图像加载被中断) \
  event_def(blur,元素失去焦点) \
  event_def(change,用户改变域的内容) \
  event_def(click,鼠标点击某个对象) \
  event_def(dblclick,鼠标双击某个对象) \
  event_def(error,当加载文档或图像时发生某个错误) \
  event_def(focus,元素获得焦点) \
  event_def(keydown,某个键盘的键被按下) \
  event_def(keypress,某个键盘的键被按下或按住) \
  event_def(keyup,某个键盘的键被松开) \
  event_def(load,某个页面或图像被完成加载) \
  event_def(mousedown,某个鼠标按键被按下) \
  event_def(mousemove,鼠标被移动) \
  event_def(mouseout,鼠标从某元素移开) \
  event_def(mouseover,鼠标被移到某元素之上) \
  event_def(mouseup,某个鼠标按键被松开) \
  event_def(reset,重置按钮被点击) \
  event_def(resize,窗口或框架被调整尺寸) \
  event_def(select,文本被选定) \
  event_def(submit,提交按钮被点击) \
  event_def(unload,用户退出页面)
