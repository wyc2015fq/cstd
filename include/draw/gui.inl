
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "cstd.h"

#include "xtypes.h"
#include "macro.h"
#include "cstd.h"
#include "imresize.h"
#include "gui.h"
#include "tchar_x.h"

typedef CvScalar CvScalar;
typedef IPOINT IPOINT;
char wndname[] = "Drawing Demo";

int width = 1000, height = 700, cn = 3;
#define IMGSIZE (width*height*3)
#define STEP      (width*cn)
#define CN        (cn)
#define SHOWIMG() cvShowImage(wndname, height, width, image, STEP, CN)

unsigned char* image = NULL;
gwin* g_win = NULL;
gwin* c_win = NULL;

int loadimg(const char* picname, int h, int w,
    unsigned char* img, int al, int cn)
{
  struct fmtimgreader* reader = NULL;

  if (NULL == picname) {
    return 0;
  }

  reader = new_reader(picname);

  if (reader) {
    int height = reader->m_height;
    int width = reader->m_width;
    int cn = cn;
    int step = width * cn;
    unsigned char* data;
    data = MALLOC(unsigned char, height * step);
    imread(reader, data, step, cn);
    IMRESIZE(height, width, data, step, cn, h, w, img, al, cn, INTER_BILINEARITY);
    free(data);
    free_reader(reader);
  }

  return NULL != reader;
}

#define LOADIMG(picname, _height, _width, img)  loadimg(picname, _height, _width, img, _width*3, 3)

int push_win(gwin** pwin, gwin* win)
{
  if (NULL == *pwin) {
    *pwin = win;
  }
  else {
    gwin* w = *pwin;

    for (; w->next; w = w->next)
      ;

    w->next = win;
  }

  return 0;
}

gwin* gui_find_win(gwin* w, char* title, gwin** prev)
{
  //gwin* w = g_win;
  gwin* prev_win = NULL;

  if (NULL == title) {
    return 0;
  }

  for (; w; prev_win = w, w = w->next) {
    if (0 == stricmp(w->title, title)) {
      if (prev) {
        *prev = prev_win;
      }

      return w;
    }
  }

  return 0;
}

gwin* gui_find_win_withkey(gwin* w, int qkey)
{
  //gwin* w = g_win;
  gwin* prev_win = NULL;

  if (0 == qkey) {
    return 0;
  }

  for (w = w->btn; w; prev_win = w, w = w->next) {
    if (qkey == w->quick_key) {
      return w;
    }
  }

  return 0;
}

int gui_set_active_key(gwin* w, int qkey)
{
  w->active = gui_find_win_withkey(w, 'D');
  return w->active != NULL;
}
//#define find_win(title, prev) gui_find_win(g_win, title, prev)

gwin* find_win(char* title, gwin** prev)
{
  return gui_find_win(g_win, title, prev);
}

int gui_set_style(void* win0, int add0, int del0)
{
  gwin* win = (gwin*) win0;

  if (win) {
    win->style |= add0;
    win->style &= ~del0;
  }

  return NULL != win;
}

int gui_set_on_click(void* win0, fun_t on_click, void* click_param)
{
  gwin* win = (gwin*) win0;

  if (win) {
    win->on_click = on_click;
    win->click_param = click_param;

    if (win->h == 60) {
      int aaa = 0;
    }
  }

  return NULL != win;
}

int gui_set_active(char* title)
{
  int ret = 0;
  gwin* win = NULL;
  win = find_win(title, 0);

  if (win) {
    c_win = win;
  }

  return NULL != win;
}

int gui_set_title(void* win0, const char* title)
{
  gwin* win = (gwin*) win0;
  assert(strlen(title) < MAXTITLELEN);
  strcpy(win->title, title);
  return 0;
}

int gui_set_on_key(void* win0, fun_t on_key)
{
  gwin* win = (gwin*) win0;
  win->on_key = on_key;
  return 0;
}

int gui_set_on_show(void* win0, fun_t on_show)
{
  gwin* win = (gwin*) win0;
  win->on_show = on_show;
  return 0;
}

void draw_digi(void* win0)
{
  gwin* btn = (gwin*) win0;
  xDrawDigi(height, width, image, STEP, CN,
      btn->title, CC_RGB(0, 255, 0),
      btn->h, btn->h * 6 / 10, btn->y, btn->x, 1, 0.1);
}

void draw_text(void* win0, int r, int g, int b)
{
  gwin* btn = (gwin*) win0;
  char* text = btn->title;
  char buf[ MAXTITLELEN ];
  CvScalar btnfontcol[ 4 ];
  int y2 = btn->h / 2, x2 = btn->w / 2;
  int btnstep = btn->w * CN;
  unsigned char* img = btn->img ? (btn->img + (y2) * btnstep + (x2) * CN) : (image + (btn->y + y2) * STEP + (btn->x + x2) * CN);
  int aa = 0;
  aa = (img[ 0 ] + img[ 1 ] + img[ 1 ] + img[ 2 ]) >> 2;

  if (aa < 128) {
    btnfontcol[ 0 ] = CC_RGB(255, 255, 0);
    btnfontcol[ 1 ] = CC_RGB(255, 255, 255);
    btnfontcol[ 2 ] = CC_RGB(0, 255, 0);
    btnfontcol[ 3 ] = CC_RGB(255, 0, 0);
  }
  else {
    btnfontcol[ 0 ] = CC_RGB(52, 111, 52);
    btnfontcol[ 1 ] = CC_RGB(255, 0, 0);
    btnfontcol[ 2 ] = CC_RGB(0, 255, 0);
    btnfontcol[ 3 ] = CC_RGB(255, 0, 0);
  }

  if (ST_PASSWORD & btn->style) {
    memset(buf, '*', strlen(btn->title));
    buf[ strlen(btn->title) ] = '\0';
    text = buf;
  }

  if (text[ 0 ] == '#') {
    ++text;
  }

  xDrawText(height, width, image, STEP, CN,
      btn->y + btn->h / 4,
      btn->x + (btn->w - btn->h * strlen(text) / 4) / 2 + (1 == btn->btn_state),
      "ºÚÌå", btn->h / 2, 0,
      0, 1, text,
      btnfontcol[ btn->btn_state ]);
}

int on_show_bmp(void* win0, void* b)
{
  gwin* btn = (gwin*) win0;

  if (btn->img) {
    unsigned char* img = image + btn->y * STEP + btn->x * CN;
    int w = (btn->w) * CN;
    unsigned char* btn_img = btn->img;
    int btnstep = btn->w * CN;
    SKIP_COPY(btn->h, w, btn_img, btnstep, 1, img, STEP, 1);
    return 1;
  }

  return 0;
}

int on_show_bmp4(void* win0, void* b)
{
  gwin* btn = (gwin*) win0;

  if (btn->img) {
    unsigned char* img = image + btn->y * STEP + btn->x * CN;
    int w = (btn->w) * CN;
    unsigned char* btn_img = btn->img + btn->btn_state * w;
    int btnstep = btn->w * BTN_PICS * CN;
    SKIP_COPY(btn->h, w, btn_img, btnstep, 1, img, STEP, 1);
    return 1;
  }

  return 0;
}

int on_show_bmpbtn(void* win0, void* b)
{
  gwin* btn = (gwin*) win0;
  on_show_bmp4(btn, b);

  if (1) {
    draw_text(btn, 0, 0, 0);
  }

  return 0;
}

int on_show_obj(void* win0, void* b)
{
  gwin* btn = (gwin*) win0;
  on_show_bmp(btn, b);
  return 0;
}

gwin* gui_create_win(gwin** pwin, char* title, int x, int y, int cx, int cy,
    int quick_key, fun_t on_click, void* click_param, void* param)
{
  gwin* win = NULL;
  char* picpath = (char*) param;
  int ret;
  win = MALLOC(gwin, 1);

  if (NULL == win) {
    return 0;
  }

  memset(win, 0, sizeof(gwin));
  win->img = MALLOC(unsigned char, cx * cy * cn);
  ret = LOADIMG(picpath, cy, cx, win->img);

  if (0 == ret) {
#if 1
    SAFEFREE(win->img);
#else

    memset(win->img, 0, cx * cy * cn);
#endif

  }

  if (title) {
    assert(strlen(title) < MAXTITLELEN);
    strcpy(win->title, title);
  }

  win->x = x;
  win->y = y;
  win->w = cx;
  win->h = cy;
  win->quick_key = quick_key;
  win->click_param = click_param;
  win->on_click = on_click;
  win->next = NULL;
  push_win(pwin, win);
  return win;
}

gwin* gui_add_win(char* title, void* param)
{
  return gui_create_win(&g_win, title, 0, 0, width, height, 0, 0, 0, param);
}

gwin* gui_add_btn(char* wintitle, char* title, int x, int y, int cx, int cy,
    int quick_key, fun_t fun, void* fun_param, void* param)
{
  gwin* win = find_win(wintitle, NULL);
  gwin* btn = NULL;

  if (win) {
    btn = gui_create_win(&(win->btn), title, x, y, cx * BTN_PICS, cy, quick_key, fun, fun_param, param);
    btn->w /= BTN_PICS;
    gui_set_on_show(btn, on_show_bmpbtn);
    btn->parent = win;
  }

  return btn;
}

gwin* gui_add_obj(char* wintitle, char* title, int x, int y, int cx, int cy,
    int quick_key, fun_t fun, void* fun_param, void* param)
{
  gwin* win = find_win(wintitle, NULL);
  gwin* obj = NULL;

  if (win) {
    obj = gui_create_win(&(win->btn), title, x, y, cx, cy, quick_key, fun, fun_param, param);
    gui_set_on_show(obj, on_show_obj);
    obj->parent = win;
  }

  return obj;
}

int free_all_win(gwin** pwin);

gwin* free_win(gwin* win)
{
  gwin* next = NULL;
  free_all_win(&(win->btn));
  SAFEFREE(win->img);
  SAFEFREE(win);
  return next;
}

int free_win_by_title(char* title)
{
  gwin* prev = NULL;
  gwin* win = find_win(title, &prev);

  if (win) {
    prev->next = free_win(win);

    if (win == g_win) {
      g_win = NULL;
    }
  }

  return NULL != win;
}

int free_all_win(gwin** pwin)
{
  gwin* w = *pwin;
  g_win = NULL;
  c_win = NULL;

  for (; w;) {
    gwin* next = w->next;
    free_win(w);
    w = next;
  }

  return 0;
}

int gui_free_allwin()
{
  return free_all_win(&g_win);
}

int gui_init(int h, int w)
{
  height = h;
  width = w;
  image = MALLOC(unsigned char, IMGSIZE);
  return 0;
}

int gui_uninit()
{
  SAFEFREE(image);
  cvDestroyWindow(wndname);
  return 0;
}

#define GEVENT
typedef struct GMSG {
  int type;
  union {
    struct {
      short x, y;
    };
    int key;
  };
}
GMSG;

int win_msg(gwin* win, GMSG* msg)
{
  return 0;
}

#define PT_IN_WINDOW(_X, _Y, _WIN)  ((_X)>=(_WIN)->x) && ((_X)<((_WIN)->x+(_WIN)->w)) && \
  ((_Y)>=(_WIN)->y) && ((_Y)<((_WIN)->y+(_WIN)->h))

void on_mouse(int event, int x, int y, int flags, void* param)
{
  gwin* win = *(gwin**) param;

  if (CC_EVENT_LBUTTONDOWN == event) {
    printf("%3d %3d %3d %3d\n", event, x, y, flags);
  }

  if (win) {
    gwin* btn = win->btn;
    gwin* on_click_btn = NULL;

    for (; btn; btn = btn->next) {
      btn->btn_state = 0;

      if (PT_IN_WINDOW(x, y, btn)) {
        if (CC_EVENT_MOUSEMOVE == event) {
          btn->btn_state = 1;
        }

        if (CC_EVENT_LBUTTONDOWN == event) {
          btn->btn_state = 2;
        }

        if (CC_EVENT_LBUTTONUP == event) {
          //win->active = btn;
          if (btn->on_click && !(btn->style & ST_HIDE)) {
            //btn->on_click(btn, btn->click_param);
            on_click_btn = btn;
            //break;
          }
        }
      }
    }

    if (on_click_btn) {
      on_click_btn->on_click(on_click_btn, on_click_btn->click_param);
    }
  }

  return ;
}

void on_key(gwin* win, int key)
{
  if (win) {
    gwin* btn = win->btn;

    if (win->active && win->active->on_key) {
      win->active->on_key(win->active, (void*) key);
    }

    for (; btn; btn = btn->next) {
      if (btn->on_click && key == btn->quick_key && !(btn->style & ST_HIDE)) {
        //btn->btn_state = 1;
        btn->on_click(btn, (void*)(btn->click_param));
      }
    }
  }
}

int on_show(gwin* win)
{
  if (win) {
    if (win->img) {
      memcpy(image, win->img, IMGSIZE);
    }

    if (win->btn) {
      gwin* btn = win->btn;

      for (; btn; btn = btn->next) {
        if (1) {
          if (btn->on_show && !(btn->style & ST_HIDE)) {
            btn->on_show(btn, 0);
          }
          else {
            int adadsf = 0;
          }
        }
      }
    }

    SHOWIMG();
  }

  return 0;
}

int gui_mainloop()
{
  int i;
  cvNamedWindow(wndname, 1);
  cvSetMouseCallback(wndname, on_mouse, &c_win);

  for (i = 0; 1; ++i) {
    int ch = cvWaitKey(10);

    if ('q' == ch || 'Q' == ch) {
      break;
    }

    on_key(c_win, ch);
    on_show(c_win);
  }

  gui_uninit();
  return 0;
}
