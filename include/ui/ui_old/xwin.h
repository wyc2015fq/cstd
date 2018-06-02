
#ifndef _XWIN_H_
#define _XWIN_H_

#define XWINMAX  256
#define XDRAWST  256

enum { WM_INIT = WM_USER + 10, WM_XCOMMAND };

//#define xSendEvent(EVENT)  SendMessage(msg->win->hwnd, WM_XCOMMAND, EVENT, (LPARAM)ci)
//#define xSendCmd(EVENT)  PostMessage(msg->win->hwnd, WM_XCOMMAND, EVENT, (LPARAM)cid)
#define xSendCmd(EVENT)  win->oncommand(win, WM_XCOMMAND, s)

#define xShowCaret() ShowCaret(win->hwnd)
#define xCreateCaret(x, y, w, h)  CreateCaret(win->hwnd, 0, w, h), xShowCaret(), SetCaretPos(x, y)

// WM_MDIMAXIMIZE SC_MAXIMIZE
enum {
  XCMD_CLOSE,
  //XCMD_MAXIMIZE,
  //XCMD_MINIMIZE,
  XCMD_RBOX_CHANGE,
  XCMD_CLICK,
};

struct xdisp_t;

typedef int (*draw_t)(img_t* im, const void* s);

typedef struct xpos_t {
  IRECT rc;
  int z;
} xpos_t;

typedef struct xwin_t xwin_t;
typedef struct uistyle_t uistyle_t;


typedef int (*ondraw_t)(xwin_t* win, void* s, img_t* im, const uistyle_t* drawer);
typedef int (*onmouse_t)(xwin_t* win, void* s, int msg, int x, int y, int flag);
typedef int (*oncommand_t)(xwin_t* win, int cmd, void* cid);

// 控件信息
typedef struct xctrlinfo_t {
  //IRECT rc;  // 显示矩形
  ISIZE minsz; // 最小大小
  IPOINT off;  // 坐标偏移
  int t;       // 类型id
  int id;      // id
  int redraw;  // 重绘
  void* s;     // 数据
  ondraw_t ondraw;
  onmouse_t onmouse;
} xctrlinfo_t;

enum {
  UIS_DEFAULT,
  UIS_WIN2K,
};

typedef struct uistyle_t {
  int style;
  font_t* font;
} uistyle_t;

typedef struct xwin_t xwin_t;

typedef struct xwin_t {
  HWND hwnd;
  //int winid;
  int ishide;
  struct xdisp_t* disp;
  img_t im[1];
  void* sub;
  void* userdata;
  ondraw_t ondraw;
  onmouse_t onmouse;
  oncommand_t oncommand;
} xwin_t;

typedef int (*imdrawui_ctrl_f)(const void* drawer, const img_t* im, int t, void* s);

typedef struct xdisp_t {
  xwin_t* win[XWINMAX];
  xctrlinfo_t ci[XDRAWST];
  int ndi;
  int nwin;
  xwin_t* foc_win;
  int ldown;
  uistyle_t drawer[1];
} xdisp_t;

enum {
#define UICTRLINFODEF(a)  dm_##a,
#include "uictrl.txt"
#undef UICTRLINFODEF
  dm_ctrlnum
};

enum {
#define XWINMSGDEF(a)  MSG_##a,
#include "xwinmsg.txt"
#undef XWINMSGDEF
};

enum {
#define XWINMSGDEF(a)  MSG_FLAG_##a,
#include "xwinmsgflag.txt"
#undef XWINMSGDEF
};
//enum {MSG_MASK_KEY = 1, MSG_MASK_MOUSE = 2};

//#define force_redraw(e)   if (e->win) {InvalidateRect(e->win->hwnd, 0, 0);}
//#define force_redraw(e)   ci[cid].redraw=e->redraw=1
#define force_redraw(win)        InvalidateRect(win->hwnd, 0, 1)
#define update_ctrl(e, cid)      (disp->ci[cid].redraw=1, InvalidateRect(e->win->hwnd, 0, 0))

//#define XMSGINIT(XMSG, M, K)  ((XMSG)->msg=M, (XMSG)->key=K)

// 默认 盘旋 焦点 按下 禁用
enum {CS_NOM, CS_HOT, CS_FOC, CS_HIT, CS_DIS};

#if 0
typedef struct xmsg_t {
  xwin_t* win;
  int msg;
  int x, y;
  int key;
  int flags;
  int wp, lp;
  int redraw;
  int wheel_delta;
  void* s;
} xmsg_t;

int xctrl_msg_proc(const xmsg_t* msg, xctrlinfo_t* ci, int cid, img_t* im, const uistyle_t* drawer);
int xctrl_size_proc(xctrlinfo_t* ci, int cid, img_t* im, const uistyle_t* drawer);
#endif

#endif // _XWIN_H_
