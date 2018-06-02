#ifndef _UIWIN_INL_
#define _UIWIN_INL_
#undef WINVER
#undef _WIN32_WINNT
#define WINVER 0x0500 // this is needed for UpdateLayeredWindow
#define _WIN32_WINNT 0x0500 // this is needed for the WS_EX_LAYERED
#include "cstd.h"
#include <windows.h>
//#include <windowsx.h>
#include <commctrl.h>
#include <setjmp.h>
#include "draw/imdraw.inl"
//#define WM_SYSTIMER 0x0118 //(caret blink)
//#define DRV_EXITAPPLICATION 0x000C //驱动程序消息相关
//#define WM_HIBERNATE 0x03FF //WINCE独有的消息(内存资源限制相关)
#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp) (short)LOWORD(lp)
#define GET_Y_LPARAM(lp) (short)HIWORD(lp)
#endif
//窗口消息解码
static char* winmsg(int code)
{
#define IDSTR(a, b) case a: return b;
  //#include "winmsg.inl"
#undef IDSTR
  return 0;
}
//#pragma comment(lib,"Comctl32.lib")
#define MYWINDOWCLASSNAME "MyWindowClass"
#define MAXWINNUM 100
static WNDCLASSEX g_winclass = {0};
enum {
  FlowLayout,
  GridLayout, // 网格布局 new GridLayout(2, 3); 如果组件容纳不下标签文本，将使用省略号（...）表示省略的文本
  BorderLayout, // newBorderLayout(10, 20); // 参数10,20用于指定组件之间的水平，垂直距离 该布局有5个方位：上下左右中
  BoxLayout,
};
enum {
  WM_CTRLINIT = WM_USER + 1,
  WM_CTRLFREE,
  //WM_SET,
  //WM_GET,
  WM_TESTINIT,
};
enum {
  WT_WINDOWS,
  WT_CHILD,
  WT_DOCKER,
  WT_MENU,
  WT_TOOLTIP,
  WT_POPUP,
};
enum {
  XCMD_CLOSE,
  //XCMD_MAXIMIZE,
  //XCMD_MINIMIZE,
  XCMD_CHANGE,
  XCMD_CLICK,
};
typedef struct ctrl_t ctrl_t;
typedef LRESULT(*ctrl_proc_t)(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam);
typedef struct sys_t sys_t;
typedef struct uidraw_t uidraw_t;
#include "uistyle.inl"
#define SBW1 16
#define THUMB_MIN (SBW1/4)
enum {
  SBHT_LINE_0 = 0,
  SBHT_LINE_1,
  SBHT_PAGE_0,
  SBHT_PAGE_1,
  SBHT_THUMB,
};
#define HITRECTNUM 32
typedef struct xpos_t {
  IRECT rc;
  int z;
} xpos_t;
typedef struct scroll_t {
  int hit, hot;
  SCROLLINFO si[2]; // 0-H 1-V
  IPOINT pthit;
  int hit_pos;
} scroll_t;
typedef struct frame_t {
  int hit;
  int hot;
  int x, y;
  IRECT rc1;
  IRECT rc2;
} frame_t;
typedef struct ctrl_t {
  IRECT rc; // box
  IRECT rcclip;
  IRECT margin;
  IRECT border;
  IRECT pading;
  //IRECT content; = box - margin - border - pading
  scroll_t scroll[1];
  frame_t frame[1];
  int id;
  void* x;
  //img_t im[1];
  int redraw;
  int redraw_back;
  int recalcsize;
  ctrl_proc_t ctrlproc;
  int isdock;
  ctrl_t* listener;
  int isshow;
  //ctrl_t* dockctrl;
  int type;
  int style;
  font_t* fo; // font
  sys_t* sys;
  const char* text;
  struct ctrl_t* prev;
  struct ctrl_t* next;
  struct ctrl_t* sub;
  struct ctrl_t* father;
} ctrl_t;
typedef struct imgdata_t {
  char name[MAXNAMELEN];
  img_t im[1];
  struct imgdata_t* next;
} imgdata_t;
static imgdata_t* imgdata_find(imgdata_t* imd, const char* name)
{
  int l = strlen(name);
  for (; imd;) {
    if (0 == cstr_icmp(imd->name, -1, name, l, 1)) {
      return imd;
    }
    imd = imd->next;
  }
  return 0;
}
typedef struct sys_t {
  uidraw_t* drawer;
  ctrl_t* win;
  ctrl_t* focus;
  ctrl_t* focus_noactivate;
  font_t* fo;
  imgdata_t* imd;
  //ctrl_t win[MAXWINNUM];
  //int nwin;
  int ldown;
  HWND hwnd;
  HDC hdc;
  IRECT rcDirty;
  IRECT rcCursor;
  int cursor_speed;
  int cursor_show;
  //int style; //
  //font_t* font;
  char fontpaths[256];
  char imagepaths[256];
  //ctrl_t menu[MAXMENUDEEP];
  IPOINT ptMouse;
  IPOINT ptLDown;
  HINSTANCE hInstance; // GetModuleHandle();
} sys_t;
sys_t g_sys[1] = {0};
typedef struct imginfo_t {
  imgdata_t* imd;
  IRECT source;
  IRECT corner;
} imginfo_t;
#if 1
//#define force_redraw(c) ((c)->redraw=1, c->sys->hwnd ? InvalidateRect(c->sys->hwnd, 0, 0) : 0)
#else
//#include "debug.h"
//#define force_redraw(c) ((c)->redraw=1, InvalidateRect(hwnd, 0, 0)), printf("%s(%d)\n", __FILE__, __LINE__);
#endif
#define sendmsg(sender, to, uMsg, wParam, lParam) if (to) {(to)->ctrlproc(sender, to, uMsg, wParam, lParam);}
static int force_redraw(ctrl_t* c)
{
  (c)->redraw = 1;
  InvalidateRect(c->sys->hwnd, 0, 0);
  c->sys->rcDirty = iRectUnion(c->sys->rcDirty, c->rc);
  //printf("%s(%d)\n", __FILE__, __LINE__);
  return 0;
}
// 1、aaa.jpg
// 2、file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0'
static int imageinfo_prars(const char* imginfostr, char* name, imginfo_t* out)
{
  str_t s = STR1(imginfostr);
  int i = 0;
  str_t key[1] = {0};
  str_t value[1] = {0};
  IRECT rc;
  const char* ss = "|file|restype|dest|source|corner|";
  int l = strlen(ss);
  for (; (i = STRsplit_pair(s, i, '=', '\'', '\'', "\t ", key, value)) > 0;) {
    int k = cstr_splitfind(ss, l, key->s, key->l, 0, 0), j;
    switch (k) {
    case 0:
      j = MIN(MAXNAMELEN - 1, value->l);
      strncpy(name, value->s, j);
      name[j] = 0;
      break;
    case 1:
      break;
    case 3:
      j = 0;
      rc.l = str2i(value->s, value->l, j, " \t,", &j);
      rc.t = str2i(value->s, value->l, j, " \t,", &j);
      rc.r = str2i(value->s, value->l, j, " \t,", &j);
      rc.b = str2i(value->s, value->l, j, " \t,", &j);
      out->source = rc;
      break;
    case 4:
      j = 0;
      rc.l = str2i(value->s, value->l, j, " \t,", &j);
      rc.t = str2i(value->s, value->l, j, " \t,", &j);
      rc.r = str2i(value->s, value->l, j, " \t,", &j);
      rc.b = str2i(value->s, value->l, j, " \t,", &j);
      out->corner = rc;
      break;
    default:
      break;
    }
  }
  return 1;
}
#include "img/imgio.inl"
static imgdata_t* sys_find_imgdata(sys_t* sys, const char* name)
{
  imgdata_t* imd = imgdata_find(sys->imd, name);
  if (NULL == imd) {
    buf_t bf[1] = {0};
    if (buf_load_from_paths(sys->imagepaths, name, "|.bmp|.png|.jpg", bf)) {
      imd = MALLOC(imgdata_t, 1);
      memset(imd, 0, sizeof(imgdata_t));
      imload_mem(bf->data, bf->len, 0, imd->im);
      strcpy(imd->name, name);
      if (sys->imd) {
        imd->next = sys->imd->next;
      }
      sys->imd = imd;
    }
    bffree(bf);
  }
  return imd;
}
static img_t* sys_find_img(sys_t* sys, const char* name)
{
  imgdata_t* imd = sys_find_imgdata(sys, name);
  return imd ? imd->im : NULL;
}
static int imageinfo_load(sys_t* sys, const char* imginfostr, imginfo_t* imginfo)
{
  char name[256];
  if (imageinfo_prars(imginfostr, name, imginfo)) {
    imginfo->imd = sys_find_imgdata(sys, name);
    return 1;
  }
  return 0;
}
static int sys_add_font(sys_t* sys, font_t* fo)
{
  if (sys->fo) { // 第一个是默认字体不能替换
    fo->next = sys->fo->next, sys->fo->next = fo;
  }
  else {
    fo->next = sys->fo, sys->fo = fo;
  }
  return 0;
}
static int sys_font_load_memraster(sys_t* sys, const char* name, const raster_font_t* rfo)
{
  font_t* fo = NULL;
  fo = font_find(sys->fo, name, rfo->h, 0);
  if (!fo) {
    fo = (font_t*)malloc(sizeof(font_t));
    font_init_memraster(fo, name, rfo);
    sys_add_font(sys, fo);
  }
  return 0;
}
static int sys_font_ttffile_release(const struct font_t* fo)
{
  font_ttffile_t* fc = (font_ttffile_t*)fo->user;
  int i;
  for (i = 0; i < (1 << 16); ++i) {
    if (fc->bmp[i]) {
      free(fc->bmp[i]);
    }
  }
  bffree(fc->bf);
  free(fc);
  return 0;
}
static font_t* sys_font_load_ttffile(sys_t* sys, const char* fontinfo)
{
  int i = 0;
  int ret = 0, style = 0, size = 20;
  int maxn = MAXFONT;
  font_t* fo = NULL;
  char name[32] = {0};
  if (!font_prars(fontinfo, name, &size, &style)) {
    return NULL;
  }
  fo = font_find(sys->fo, name, size, style);
  if (!fo) {
    font_ttffile_t* fc = NULL;
    fc = (font_ttffile_t*)malloc(sizeof(font_ttffile_t));
    memset(fc, 0, sizeof(font_ttffile_t));
    if (buf_load_from_paths(sys->fontpaths, name, ".ttf", fc->bf)
        && stbtt_InitFont(fc->font, fc->bf->data, stbtt_GetFontOffsetForIndex(fc->bf->data, 0))) {
      float scale;
      int ascent = 0, descent, lineGap;
      fo = fc->fo;
      fo->h = size;
      fo->w = size / 2;
      fo->style = style;
      fo->user = fc;
      fo->getbit = sys_font_ttffile_getbit;
      fo->release = sys_font_ttffile_release;
      fo->bpp = 8; // 8bit
      strcpy(fo->name, name);
      scale = stbtt_ScaleForPixelHeight(fc->font, (float)fc->fo->h);
      stbtt_GetFontVMetrics(fc->font, &ascent, &descent, &lineGap);
      fo->bl = (int)(ascent * scale);
      sys_add_font(sys, fo);
    }
    else {
      bffree(fc->bf);
      free(fc);
      return NULL;
    }
  }
  return fo;
}
typedef struct font_win32_t {
  font_t fo[1];
  font_bit_t* bmp[1 << 16];
  HDC hMemDC;
  HFONT h;
} font_win32_t;
CC_INLINE int sys_font_win32_getbit(const font_t* fo, int asi_code, font_bit_t* bit0)
{
  static const MAT2 mat2 = {{0, 1}, {0, 0}, {0, 0}, {0, 1}};
  font_win32_t* fc = (font_win32_t*)(fo->user);
  font_bit_t* ret = NULL;
  if (!fc->bmp[asi_code]) {
    int nLen;
    GLYPHMETRICS gm[1] = {0};
    int uFormat = 8 == fo->bpp ? GGO_GRAY8_BITMAP : GGO_BITMAP;
    //uni_code = L'B';
    //SelectObject(fc->hMemDC, fc->h);
    nLen = GetGlyphOutline(fc->hMemDC, asi_code, uFormat, gm, 0, 0, &mat2);
    if (nLen > 0) {
      const uchar g_byAlphaLevel8[65] = { 0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220, 224, 228, 232, 236, 240, 244, 248, 252, 255 };
      int sz = sizeof(font_bit_t) + nLen;
      font_bit_t* bit = (font_bit_t*)malloc(sz);
      memset(bit, 0, sz);
      bit->bit = (uchar*)(bit + 1);
      nLen = GetGlyphOutline(fc->hMemDC, asi_code, uFormat, gm, nLen, bit->bit, &mat2);
      bit->bx = gm->gmBlackBoxX, bit->by = gm->gmBlackBoxY;
      bit->x = (int)gm->gmptGlyphOrigin.x, bit->y = (int)gm->gmptGlyphOrigin.y;
      bit->step = (gm->gmBlackBoxX + 3) & ~3;
      ret = fc->bmp[asi_code] = bit;
      for (; --nLen >= 0;) {
        bit->bit[nLen] = g_byAlphaLevel8[bit->bit[nLen]];
      }
    }
  }
  else {
    ret = fc->bmp[asi_code];
  }
  if (ret) {
    *bit0 = *ret;
  }
  return ret != NULL;
}
static int sys_font_win32_release(const struct font_t* fo)
{
  font_win32_t* fc = (font_win32_t*)fo->user;
  int i;
  for (i = 0; i < (1 << 16); ++i) {
    if (fc->bmp[i]) {
      free(fc->bmp[i]);
    }
  }
  DeleteDC(fc->hMemDC);
  DeleteObject((HGDIOBJ)(HFONT)(fc->h));
  free(fc);
  return 0;
}
static font_t* sys_font_load_win32_impl(sys_t* sys, const char* name, int size, int style)
{
  font_t* fo = NULL;
  int i = 0, ret = 0, maxn = MAXFONT;
  fo = font_find(sys->fo, name, size, style);
  if (!fo) {
    int fnWeight = style & FONT_BOLD ? FW_BOLD : FW_NORMAL;
    font_win32_t* fc = NULL;
    TEXTMETRIC tm[1] = {0};
    HDC hDC = GetDC(0);
    fc = (font_win32_t*)malloc(sizeof(font_win32_t));
    memset(fc, 0, sizeof(font_win32_t));
    fc->hMemDC = CreateCompatibleDC(hDC);
#if 1
    fc->h = CreateFont(-size, 0, 0, 0, style & FONT_BOLD ? FW_BOLD : FW_NORMAL, style & FONT_ITALIC ? 1 : 0,
        style & FONT_UNDERLINE ? 1 : 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE | DEFAULT_PITCH, name);
#else
    {
      LOGFONTA lf[1] = {0};
      int ly = GetDeviceCaps(fc->hMemDC, LOGPIXELSY);
      int sz = MulDiv(12, ly, 72);
      lf->lfHeight = -(abs(sz));
      lf->lfWeight = style & FONT_BOLD ? FW_BOLD : FW_NORMAL;
      lf->lfItalic = (BYTE)(style & FONT_ITALIC ? 1 : 0);
      lf->lfCharSet = DEFAULT_CHARSET;
      lf->lfQuality = DEFAULT_QUALITY;
      strncpy(lf->lfFaceName, name, sizeof(lf->lfFaceName));
      //hash = HashFont(faceName_, characterSet_, size_, bold_, italic_, extraFontFlag_);
      fc->h = CreateFontIndirectA(lf);
    }
#endif
    // 创建GDI相关设备
    ReleaseDC(0, hDC);
    if (fc->h) {
      SelectObject(fc->hMemDC, fc->h);
      GetTextMetrics(fc->hMemDC, tm);
      fo = fc->fo;
      fo->h = size;
      fo->w = size / 2;
      fo->style = style;
      fo->user = fc;
      fo->getbit = sys_font_win32_getbit;
      fo->release = sys_font_win32_release;
      fo->bpp = 8; // 8bit
      strcpy(fo->name, name);
      fo->bl = (uchar)tm->tmAscent;
      sys_add_font(sys, fo);
    }
  }
  return fo;
}
static font_t* sys_font_load_win32(sys_t* sys, const char* fontinfo)
{
  int i = 0;
  int ret = 0, style = 0, size = 20;
  int maxn = MAXFONT;
  char name[32] = {0};
  if (!font_prars(fontinfo, name, &size, &style)) {
    return NULL;
  }
  return sys_font_load_win32_impl(sys, name, size, style);
}
#define WINBMPCN 3
static int SetFullScreen(HWND hWnd, int is_FullScreen, IRECT* rc_back)
{
  IRECT rect;
  //ShowTaskBar(is_FullScreen);
  if (is_FullScreen) {
    rect = *rc_back;
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) | ((WS_CAPTION | WS_BORDER)));
    ShowWindow(hWnd, SW_HIDE);
    SetWindowPos(hWnd, 0, rect.l, rect.t, RCW(&rect), RCH(&rect), SWP_SHOWWINDOW);
  }
  else {
    int frame_w = GetSystemMetrics(SM_CXFRAME);
    int frame_h = GetSystemMetrics(SM_CYFRAME);
    int screen_w = GetSystemMetrics(SM_CXSCREEN);
    int screen_h = GetSystemMetrics(SM_CYSCREEN);
    //int caption_h = GetSystemMetrics(SM_CYCAPTION);
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & (~(WS_CAPTION | WS_BORDER)));
    GetWindowRect(hWnd, (RECT*)rc_back);
    GetClientRect(hWnd, (RECT*)&rect);
    rect.l = rect.l - frame_w;
    rect.r = rect.l + screen_w + 2 * frame_w;
    rect.t = rect.t - frame_h;
    rect.b = rect.t + screen_h + 2 * frame_h;
    ShowWindow(hWnd, SW_HIDE);
    SetForegroundWindow(hWnd);
    SetWindowPos(hWnd, HWND_TOPMOST, rect.l, rect.t, RCW(&rect), RCH(&rect), SWP_SHOWWINDOW | SWP_NOZORDER);
    //SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
  }
  is_FullScreen = !is_FullScreen;
  return is_FullScreen;
}
static int GetBitmapT(HBITMAP hbmp, BITMAP* bmp)
{
  if (GetObject(hbmp, sizeof(BITMAP), bmp) == 0) {
    return 0;
  }
  return 1;
}
#include "sys/win32.inl"
static LRESULT TraceMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  char* str = 0;
  switch (msg) {
  case WM_NCPAINT:
  case WM_NCHITTEST:
  case WM_SETCURSOR:
    return 0;
  }
  switch (msg) {
#define MSG2STR(a, b, c) case a: str=#a","c;break;
#include "msg2string.txt"
#undef MSG2STR
  default:
    str = 0;
    break;
  }
  if (str) {
    printf("w=%08x l=%08x %s\n", wparam, lparam, str);
    //printf("%s\n", str);
  }
  else {
    printf("m=%08x w=%08x l=%08x %s\n", msg, wparam, lparam, "未知");
    //printf("%d\n", msg.message);
    //printf("%x\n", 0x8000);
  }
  return 0;
}
CC_INLINE IPOINT MyClientToScreen(HWND hwnd, int x, int y)
{
  POINT pt;
  pt.x = x, pt.y = y;
  ClientToScreen(hwnd, &pt);
  return iPOINT(pt.x, pt.y);
}
CC_INLINE IPOINT MyScreenToClient(HWND hwnd, int x, int y)
{
  POINT pt;
  pt.x = x, pt.y = y;
  ScreenToClient(hwnd, &pt);
  return iPOINT(pt.x, pt.y);
}
static int myClientToScreen(HWND hwnd, int* x, int* y)
{
  POINT pt = {*x, *y};
  ClientToScreen(hwnd, &pt);
  *x = pt.x, *y = pt.y;
  return 0;
}
static int myScreenToClient(HWND hwnd, int* x, int* y)
{
  POINT pt = {*x, *y};
  ScreenToClient(hwnd, &pt);
  *x = pt.x, *y = pt.y;
  return 0;
}
CC_INLINE int MyScreenToClient2(HWND hwnd, void* p, int n)
{
  POINT* pt = (POINT*)p;
  for (; --n > 0;) {
    ScreenToClient(hwnd, pt + n);
  }
  return 1;
}
CC_INLINE int MyClientToScreen2(HWND hwnd, void* p, int n)
{
  POINT* pt = (POINT*)p;
  for (; --n >= 0;) {
    ClientToScreen(hwnd, pt + n);
  }
  return 1;
}
//#include "dock_impl.inl"
CC_INLINE int ctrl_isfocus(ctrl_t* c)
{
  if (c && c->sys) {
    ctrl_t* focus = c->sys->focus;
    for (; focus; focus = focus->father) {
      if (c == focus) {
        return 1;
      }
    }
  }
  return 0;
}
CC_INLINE int ctrl_setfocus(ctrl_t* c)
{
  if (c) {
    sys_t* sys = c->sys;
    if (c->isshow && sys) {
      if (c->isshow != SW_SHOWNOACTIVATE) {
        if (sys->focus != c) {
          if (sys->focus && sys->focus->ctrlproc) {
            sys->focus->ctrlproc(c, sys->focus, WM_KILLFOCUS, 0, (LPARAM)c);
            force_redraw(sys->focus);
          }
          sys->focus = c;
        }
        if (sys->focus_noactivate != c) {
          if (sys->focus_noactivate && sys->focus_noactivate->ctrlproc) {
            sys->focus_noactivate->ctrlproc(c, sys->focus_noactivate, WM_KILLFOCUS, 0, (LPARAM)c);
            force_redraw(sys->focus_noactivate);
          }
          sys->focus_noactivate = NULL;
        }
      }
      else {
        sys->focus_noactivate = c;
      }
      if (c) {
        force_redraw(c);
      }
      return 1;
    }
  }
  return 0;
}
CC_INLINE int ctrl_recalcsize(ctrl_t* c)
{
  if (c) {
    ctrl_t* ctrl;
    c->recalcsize = 1;
    c->redraw = 1;
    ctrl = c->sub;
    for (; ctrl; ctrl = ctrl->next) {
      ctrl_recalcsize(ctrl);
    }
  }
  return 0;
}
CC_INLINE int ctrl_setrect(ctrl_t* c, IRECT rc)
{
  if (c && !RCEQ(&c->rc, &rc)) {
    //imsetsize(c->im, RCH(&rc), RCW(&rc), 4, 1);
    c->sys->rcDirty = iRectUnion(c->sys->rcDirty, rc);
    c->sys->rcDirty = iRectUnion(c->sys->rcDirty, c->rc);
    c->rc = rc;
    c->rcclip = rc;
    c->redraw = 1;
    c->recalcsize = 1;
    ctrl_recalcsize(c);
    //c->ctrlproc(0, c, WM_SIZE, 0, 0);
  }
  return 0;
}
CC_INLINE int ctrl_setredraw(ctrl_t* c)
{
  ctrl_t* sub = c->sub;
  return 0;
  for (; NULL != sub; sub = sub->next) {
    if (c->redraw && !sub->redraw) {
      IRECT rc = iRectInter(c->rc, sub->rc);
      if (!iRectIsEmpty(&rc)) {
        sub->redraw = 1;
      }
    }
    ctrl_setredraw(sub);
  }
  return c->redraw;
}
CC_INLINE void* mymalloc(int sz)
{
  void* p = malloc(sz);
  memset(p, 0, sz);
  return p;
}
int ctrl_set_capture(ctrl_t* c)
{
  return 0;
}
int ctrl_show1(ctrl_t* c, BOOL bshow)
{
  c->isshow = bshow;
  return 0;
}
int ctrl_move(ctrl_t* c, int x, int y, int cx, int cy, BOOL bRepaint)
{
  IRECT rc = iRECT2(x, y, cx, cy);
  ctrl_setrect(c, rc);
  c->redraw = bRepaint;
  return 0;
}
static int sys_setcursor(sys_t* sys, const char* lpCursorName)
{
  SetCursor(LoadCursor(NULL, lpCursorName));
  return 0;
}
static int ctrl_setcursor(ctrl_t* c, int hit)
{
  if (hit == HTLEFT || hit == HTRIGHT) {
    sys_setcursor(c->sys, IDC_SIZEWE);
  }
  else if (hit == HTTOP || hit == HTBOTTOM) {
    sys_setcursor(c->sys, IDC_SIZENS);
  }
  else if (hit == HTTOPLEFT || hit == HTBOTTOMRIGHT) {
    sys_setcursor(c->sys, IDC_SIZENWSE);
  }
  else if (hit == HTTOPRIGHT || hit == HTBOTTOMLEFT) {
    sys_setcursor(c->sys, IDC_SIZENESW);
  }
  else if (HTCAPTION == hit) {
    //sys_setcursor(sys, IDC_SIZEALL);
  }
  return 0;
}
static int sys_init(sys_t* sys);
int ctrl_free(ctrl_t* win)
{
  return 0;
}
int sys_remove_allctrl(sys_t* sys, ctrl_t* win)
{
  return 0;
}
#if 0
CC_INLINE HWND get_hwnd(ctrl_t* c)
{
  for (; c;) {
    if (sys->hwnd) {
      return sys->hwnd;
    }
    c = c->listener;
  }
  return 0;
}
#endif
int sys_count(sys_t* sys)
{
  int i = 0;
  ctrl_t* c = sys->win;
  for (; c;) {
    i++;
    c = c->next;
  }
  return i;
}
int ctrl_remove(ctrl_t* c)
{
  sys_t* sys = c->sys;
  ctrl_t* fa = c->father;
  if (sys->win == c) {
    sys->win = c->next;
    if (c->next) {
      c->next->prev = NULL;
    }
  }
  else {
    if (c->prev) {
      c->prev->next = c->next;
    }
    if (c->next) {
      c->next->prev = c->prev;
    }
  }
  if (fa && fa->sub == c) {
    fa->sub = c->next;
  }
  c->prev = NULL;
  c->next = NULL;
  c->father = NULL;
  return 0;
}
int ctrl_add_head(ctrl_t** phead, ctrl_t* c)
{
  ctrl_remove(c);
  ASSERT(phead != NULL);
  c->next = *phead;
  c->prev = NULL;
  if (*phead) {
    (*phead)->prev = c;
  }
  *phead = c;
  return 0;
}
int ctrl_add_tail(ctrl_t** phead, ctrl_t* c)
{
  ASSERT(phead != NULL);
  ctrl_remove(c);
  if (*phead) {
    ctrl_t* head = *phead;
    for (; head->next; head = head->next) {}
    head->next = c;
    c->prev = head;
    c->next = NULL;
  }
  else {
    *phead = c;
    c->prev = *phead;
    c->next = NULL;
  }
  return 0;
}
CC_INLINE int ctrl_addsub_head(ctrl_t* c, ctrl_t* sub)
{
  if (NULL != c && NULL != sub) {
    ctrl_add_head(&c->sub, sub);
    sub->father = c;
  }
  return 0;
}
CC_INLINE int ctrl_addsub_tail(ctrl_t* c, ctrl_t* sub)
{
  if (NULL != c && NULL != sub) {
    ctrl_add_tail(&c->sub, sub);
    sub->father = c;
  }
  return 0;
}
int ctrl_close(ctrl_t* c)
{
  sys_t* sys = c->sys;
  if (sys && c->ctrlproc) {
    c->ctrlproc(0, c, WM_CTRLFREE, 0, 0);
    ctrl_remove(c);
    c->sys = NULL;
    c->text = NULL;
    c->rc = iRECT(0, 0, 0, 0);
    c->rcclip = iRECT(0, 0, 0, 0);
    //imfree(c->im);
  }
  return 0;
}
int getcxdlgframe()
{
  return GetSystemMetrics(SM_CXDLGFRAME);
}
int getcycaption()
{
  return GetSystemMetrics(SM_CYCAPTION);
}
int ctrl_set_style(ctrl_t* c, int style)
{
  int old_style = c->style;
  c->style = style;
  return old_style;
}
int ctrl_modify_style(ctrl_t* c, int add0, int del0, int swap0)
{
  if (add0) {
    c->style |= add0;
  }
  if (del0) {
    c->style &= ~del0;
  }
  if (swap0) {
    if (c->style & swap0) {
      c->style &= ~del0;
    }
    else {
      c->style |= add0;
    }
  }
  return 0;
}
int ctrl_get_rect(const ctrl_t* c, IRECT rc, IRECT* caption, IRECT* client, IRECT* content)
{
  if (c->style & WS_BORDER) {
    int b = getcxdlgframe();
    iRectDeflate2(&rc, b, b);
  }
  if (c->style & WS_DLGFRAME) {
    int CaptionHeight = getcycaption();
    if (caption) {
      *caption = iRECT(rc.l, rc.t, rc.r, rc.t + CaptionHeight);
    }
    rc.t += CaptionHeight;
  }
  if (client) {
    *client = rc;
  }
  if (content) {
    iRectDeflateR(&rc, &c->margin);
    iRectDeflateR(&rc, &c->border);
    iRectDeflateR(&rc, &c->pading);
    *content = rc;
  }
  return 0;
}
static ISIZE ctrl_calc_minsize(ctrl_t* c)
{
  ISIZE sz;
  MINMAXINFO mminfo[1] = {0};
  c->ctrlproc(0, c, WM_GETMINMAXINFO, 0, (LPARAM)mminfo);
  sz.w = mminfo->ptMinTrackSize.x + RCX(&c->margin) + RCX(&c->border) + RCX(&c->pading);
  sz.h = mminfo->ptMinTrackSize.y + RCY(&c->margin) + RCY(&c->border) + RCY(&c->pading);
  return sz;
}
static IRECT ctrl_calc_rect_impl(ctrl_t* c, IRECT rc)
{
  if (c->ctrlproc) {
    MINMAXINFO mminfo[1] = {0};
    int w = RCW(&rc), h = RCH(&rc);
    //GetWindowRect(win->hwnd, &rc);
    c->ctrlproc(0, c, WM_GETMINMAXINFO, 0, (LPARAM)mminfo);
    w = MAX(w, 80);
    h = MAX(h, 80);
    w = MAX(w, mminfo->ptMinTrackSize.x);
    h = MAX(h, mminfo->ptMinTrackSize.y);
    rc = iRECT2(rc.l, rc.t, w, h);
  }
  return rc;
}
int imdraw_cursor(img_t* im, const IRECT* pclip, IRECT rc, int speed)
{
  int tt = speed;
  if (0) {
    if ((clock() % tt) > (tt >> 1)) {
      imdraw_fillrect(im, pclip, rc, _rgb(0, 0, 0));
      imdraw_inv(im, pclip, rc, 1);
    }
    else {
      imdraw_fillrect(im, pclip, rc, _rgb(1, 1, 1));
    }
  }
  else {
    static int aa = 0;
    if (0 == aa % 10) {
      imdraw_inv(im, pclip, rc, 1);
    }
    ++aa;
  }
  return 0;
}
#include "scroll.inl"
#include "frame.inl"
CC_INLINE int call_ctrlproc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  int ret;
  if (c->style & (WS_HSCROLL | WS_VSCROLL)) {
    ret = scroll_proc(c->scroll, c, uMsg, wParam, lParam);
    if (ret) {
      return ret;
    }
  }
  if ((c->style & WS_CAPTION)) {
    ret = frame_proc(c->frame, c, uMsg, wParam, lParam);
    if (ret) {
      return ret;
    }
  }
  ret = c->ctrlproc(sender, c, uMsg, wParam, lParam);
  return ret;
}
CC_INLINE int ctrlproc_paint(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  int ret = 0;
  if (NULL != c) {
    IRECT rc;
    if (c->recalcsize) {
      c->redraw = c->isshow != 0;
    }
    if (c->redraw) {
      //ret = call_ctrlproc(sender, c, uMsg, (WPARAM)(c->im), lParam);
      // printf("paint\n");
    }
    if (iRectIntersect(&rc, c->sys->rcDirty, c->rc)) {
      if (1) {
        img_t im1[1] = {0};
        imsubref((img_t*)wParam, c->rc, im1);
        c->redraw = 1;
        ret = call_ctrlproc(sender, c, uMsg, (WPARAM)(im1), lParam);
      }
      else {
        //imdraw_image_corner((img_t*)wParam, &rc, c->rc, c->im, 0, 0, 0);
      }
      c->sys->rcDirty = iRectUnion(c->sys->rcDirty, c->rc);
      //imbitblt((img_t*)wParam, &rc, c->rc.l, c->rc.t, c->im, 0, 0);
      printf("paint1\n");
    }
    c->redraw = 0;
    c->recalcsize = 0;
  }
  (NULL != c->sub && (ret = ctrlproc_paint(c, c->sub, uMsg, wParam, lParam)));
  (NULL != c->next && (ret = ctrlproc_paint(sender, c->next, uMsg, wParam, lParam)));
  return 0;
}
CC_INLINE ctrl_t* ctrlproc_mouse(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  ctrl_t* ret = NULL;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  if (NULL != c->next && (ret = ctrlproc_mouse(sender, c->next, uMsg, wParam, lParam))) {
    return ret;
  }
  //printf("x=%d y=%d\n", x, y);
  if (NULL != c->sub && (ret = ctrlproc_mouse(c, c->sub, uMsg, wParam, lParam))) {
    return ret;
  }
  if (c->isshow && (ctrl_isfocus(c) || iPtInRect(&c->rc, x, y))) {
    if (call_ctrlproc(sender, c, uMsg, wParam, lParam)) {
      ret = c;
      return ret;
    }
  }
  return ret;
}
CC_INLINE int ctrlproc_default(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  int ret = 0;
  if (NULL != c->sub && (ret = ctrlproc_default(c, c->sub, uMsg, wParam, lParam))) {
    return ret;
  }
  if (c->isshow && c->ctrlproc && 0 != (ret = call_ctrlproc(sender, c, uMsg, wParam, lParam))) {
    return ret;
  }
  if (NULL != c->next && (ret = ctrlproc_default(sender, c->next, uMsg, wParam, lParam))) {
    return ret;
  }
  return ret;
}
static LRESULT CALLBACK sys_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT ret = 0;
  sys_t* sys = (sys_t*)GetWindowLong(hwnd, GWL_USERDATA);
  ctrl_t* win = sys ? sys->win : NULL;
  switch (uMsg) {
  case WM_CREATE: {
    //CREATESTRUCT* pc = (CREATESTRUCT*)lParam;
    //()pc->lpCreateParams;
  }
  break;
  case WM_CLOSE:
    DestroyWindow(hwnd);
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
    break;
  case WM_DESTROY:
    //win_remove(win);
    //CC_INLINE win_remove(ctrl_t* win)
    if (win) {
      if (sys->hdc) {
        DeleteObject(SelectObject(sys->hdc, 0));
        DeleteDC(sys->hdc);
        sys->hdc = NULL;
      }
      sys->hwnd = 0;
      ctrl_close(win);
      SetWindowLong(hwnd, GWL_USERDATA, 0);
      return 0;
      //return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    break;
  default:
    break;
  }
  if (!win || !win || !win->x) {
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
  switch (uMsg) {
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo;
    RECT rc = {0};
    MyAdjustWindowRect(hwnd, &rc);
    mminfo = (PMINMAXINFO)lParam;
#if 0
    mminfo->ptMinTrackSize.x = 200;
    mminfo->ptMinTrackSize.y = 200;
    mminfo->ptMaxSize.x = 300;
    mminfo->ptMaxSize.y = 300;
    mminfo->ptMaxPosition.x = 0;
    mminfo->ptMaxPosition.y = 0;
    mminfo->ptMaxTrackSize.x = 300;
    mminfo->ptMaxTrackSize.y = 300;
#endif
    ret = win->ctrlproc(0, win, uMsg, wParam, lParam);
    //printf("WM_GETMINMAXINFO %d %d\n", mminfo->ptMinTrackSize.x, mminfo->ptMinTrackSize.y);
    mminfo->ptMinTrackSize.x += rc.right - rc.left;
    mminfo->ptMinTrackSize.y += rc.bottom - rc.top;
  }
  break;
  case WM_MOUSEACTIVATE:
    if (win && win->type == WT_MENU) {
      return MA_NOACTIVATE;
      return MA_NOACTIVATEANDEAT; //不激活窗体，但删除鼠标消息。
    }
    break;
  case WM_SIZE: {
    //RECT rc = {0};
    //int fwSizeType = wParam; // resizing flag
    //ret = win_resize(win, LOWORD(lParam), HIWORD(lParam));
    //static int win_resize(ctrl_t* win, int w, int h)
    int w = LOWORD(lParam), h = HIWORD(lParam);
    IRECT rc = iRECT(0, 0, w, h);
    hdc_resize(&sys->hdc, h, w, WINBMPCN);
    ctrl_setrect(win, rc);
    //return ret = ctrlproc_default(0, win, uMsg, wParam, lParam);
    //printf("WM_SIZE\n");
  }
  break;
  case WM_TIMER:
    InvalidateRect(hwnd, 0, 0);
    ret = ctrlproc_default(0, win, uMsg, wParam, lParam);
    break;
  case WM_PAINT:
    if (win) {
      PAINTSTRUCT ps = {0};
      HDC hDC = BeginPaint(hwnd, &ps);
      img_t im[1] = {0};
      hdc_get_image(sys->hdc, im);
      ctrl_setredraw(win);
      //BitBlt(hMemDC, 0, 0, w, h, hDC, 0, 0, SRCCOPY);
      imflip(im);
      {
        utime_start(_start_time);
        ret = ctrlproc_paint(0, win, uMsg, (WPARAM)im, lParam);
        printf("%f\n", utime_elapsed(_start_time));
      }
      if (sys->cursor_speed > 0) {
        imdraw_cursor(im, 0, sys->rcCursor, sys->cursor_speed);
      }
      imflip(im);
      sys->rcDirty = iRECT(0, 0, 0, 0);
      win->redraw = 0;
      BitBlt(hDC, 0, 0, im->w, im->h, sys->hdc, 0, 0, SRCCOPY);
      EndPaint(hwnd, &ps);
    }
    break;
  case WM_ERASEBKGND:
    if (win) {
      //printf("WM_ERASEBKGND\n");
      return 0; // 解决闪烁
    }
    break;
#if 0
  case WM_NCHITTEST:
    return HTCAPTION;
    break;
  case WM_MOUSELEAVE:
  case WM_MOUSEHOVER:
    ret = ctrlproc_default(0, win, uMsg, wParam, lParam);
    break;
  case WM_MOUSEMOVE:
    if (!win->bTrack) {
      TRACKMOUSEEVENT csTME;
      csTME.cbSize = sizeof(csTME);
      csTME.dwFlags = TME_LEAVE | TME_HOVER | TME_NONCLIENT;
      csTME.hwndTrack = hwnd; // 指定要 追踪 的窗口
      csTME.dwHoverTime = 0; // 鼠标在按钮上停留超过 10ms ，才认为状态为 HOVER
      _TrackMouseEvent(&csTME); // 开启 Windows 的 WM_MOUSELEAVE ， WM_MOUSEHOVER 事件支持
      win->bTrack = 1;
    }
    ret = ctrlproc_default(0, win, uMsg, wParam, lParam);
    break;
#endif
  case WM_NCMOUSEMOVE: {
    sys_t* sys = win->sys;
    int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
    myScreenToClient(hwnd, &x, &y);
    //sys->ptMouse.x = x;
    //sys->ptMouse.y = y;
    //ret = win->ctrlproc(0, win, WM_MOUSEMOVE, wParam, MAKELPARAM(x, y));
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
  case WM_NCLBUTTONUP:
  case WM_NCLBUTTONDOWN:
    if (win) {
      UINT uMsg2;
      int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
      ctrl_t* retc = NULL;
      myScreenToClient(hwnd, &x, &y);
      if (WM_NCLBUTTONDOWN == uMsg) {
        win->sys->ldown = 1;
        //ctrl_set_capture(hwnd);
        uMsg2 = WM_LBUTTONDOWN;
      }
      else if (WM_NCLBUTTONUP == uMsg) {
        win->sys->ldown = 0;
        //ReleaseCapture();
        uMsg2 = WM_LBUTTONUP;
      }
      retc = ctrlproc_mouse(0, win, uMsg2, wParam, MAKELPARAM(x, y));
      if (WM_LBUTTONDOWN == uMsg2) {
        //ctrl_setfocus(retc);
      }
      sys->ptLDown.x = x;
      sys->ptLDown.y = y;
      sys->ptMouse.x = x;
      sys->ptMouse.y = y;
      return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    break;
  case WM_MOUSEMOVE:
    if (win) {
      int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
      sys->ldown = !!(wParam & MK_LBUTTON);
      ret = NULL != ctrlproc_mouse(0, win, uMsg, wParam, lParam);
      sys->ptMouse.x = x;
      sys->ptMouse.y = y;
    }
    break;
  case WM_LBUTTONUP:
  case WM_LBUTTONDOWN:
  case WM_LBUTTONDBLCLK:
    //case WM_NCLBUTTONDBLCLK:
    if (win) {
      int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
      ctrl_t* retc = NULL;
      if (WM_LBUTTONDOWN == uMsg) {
        win->sys->ldown = 1;
        SetCapture(hwnd);
      }
      else if (WM_LBUTTONUP == uMsg) {
        win->sys->ldown = 0;
        ReleaseCapture();
      }
      if (WM_LBUTTONDOWN == uMsg || WM_LBUTTONDBLCLK == uMsg) {
        sys->cursor_show = 0;
      }
      retc = ctrlproc_mouse(0, win, uMsg, wParam, lParam);
      ret = NULL != retc;
      if (WM_LBUTTONDOWN == uMsg && retc && win->type != WT_MENU && win->type != WT_TOOLTIP) {
        ctrl_setfocus(retc);
      }
      if (WM_LBUTTONDOWN == uMsg || WM_LBUTTONDBLCLK == uMsg) {
        sys->ptLDown.x = x;
        sys->ptLDown.y = y;
      }
      sys->ptMouse.x = x;
      sys->ptMouse.y = y;
    }
    break;
  case WM_SETFOCUS:
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
    break;
  case WM_MOUSEWHEEL: {
    POINT pt;
    pt.x = GET_X_LPARAM(lParam), pt.y = GET_Y_LPARAM(lParam);
    ScreenToClient(hwnd, &pt);
    lParam = MAKELPARAM(pt.x, pt.y);
  }
  default:
    ret = ctrlproc_default(0, win, uMsg, wParam, lParam);
    break;
  }
  if (ret) {
    return ret;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
CC_INLINE void sys_exit()
{
  sys_t* sys = g_sys;
  ctrl_t* win = sys->win;
  for (; win;) {
    ctrl_t* next = win->next;
    SendMessage(sys->hwnd, WM_CLOSE, 0, 0);
    win = next;
  }
  sys->win = 0;
  UnregisterClass(MYWINDOWCLASSNAME, sys->hInstance);
  {
    font_t* fo = sys->fo;
    for (; fo;) {
      font_t* next = fo->next;
      fo->release(fo);
      fo = next;
    }
  }
  {
    imgdata_t* imd = sys->imd;
    for (; imd;) {
      imgdata_t* next = imd->next;
      imfree(imd->im);
      free(imd);
      imd = next;
    }
  }
}
#include "font/font.inl"
static int sys_init(sys_t* sys)
{
  static int inited = 0;
  if (!inited) {
    sys->cursor_show = 0;
    sys->cursor_speed = 800;
    sys->hInstance = GetModuleHandle(0);
    // first fill in the window class stucture
    g_winclass.cbSize = sizeof(WNDCLASSEX);
    g_winclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    g_winclass.lpfnWndProc = sys_window_proc;
    g_winclass.cbClsExtra = 0;
    g_winclass.cbWndExtra = 0;
    g_winclass.hInstance = sys->hInstance;
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
    sys_font_load_memraster(sys, "songti12", zimo_songti_12);
    strcpy(sys->fontpaths, "c:/windows/fonts/");
    strcpy(sys->imagepaths, "./");
    //sys->font = zimo_songti_12;
    //sys_font_set(sys, "YaHei.Consolas.1.11b|24|");
    //sys_font_set(sys, "simfang|24|");
    inited = 1;
  }
  return inited;
}
CC_INLINE POINT MyGetCursorPos(HWND hwnd)
{
  POINT pt = {0};
  GetCursorPos(&pt);
  ScreenToClient(hwnd, &pt);
  return pt;
}
#define MYWS_POPUP (WS_POPUP | WS_CHILD)
#define MYWS_WINDOW (WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_THICKFRAME)
#define WT_DISABLED (1<<30)
#define WT_DOMODAL (1<<31)
CC_INLINE int ctrl_set(ctrl_proc_t ctrlproc, void* obj, ctrl_t* c, ctrl_t* listener, int type, const char* text)
{
  static const DWORD style[] = {MYWS_WINDOW, MYWS_POPUP, MYWS_POPUP, MYWS_POPUP, MYWS_POPUP, MYWS_POPUP};
  static const DWORD styleex[] = {0, WS_EX_TOOLWINDOW, WS_EX_TOOLWINDOW, WS_EX_TOOLWINDOW, WS_EX_TOOLWINDOW, WS_EX_TOOLWINDOW};
  DWORD dwStyle, dwStyleEx;
  IRECT rc = {0};
  int ret = 0, domodal = WT_DOMODAL & type;
  int len = 0;
  int x = 0, y = 0;
  int h = 60, w = 80;
  HWND hWndParent = NULL;
  sys_t* sys = g_sys;
  sys_init(g_sys);
  c->listener = listener;
  c->ctrlproc = ctrlproc;
  c->x = obj;
  c->sys = g_sys;
  c->text = text;
  c->fo = g_sys->fo;
  c->recalcsize = 1;
  c->redraw = 1;
  c->isshow = SW_SHOW;
  //ctrl_show(c, SW_SHOW);
  type &= 0xffff;
  //ctrl_set(sys, ctrlproc, obj, c, listener);
  ASSERT(type < countof(style));
  dwStyle = style[type];
  dwStyleEx = styleex[type];
  //win_new(c, name, 0, style[type], hWndParent);
  if (WT_MENU == type || WT_TOOLTIP == type) {
    //SetFocus(hWndParent);
    //dwStyleEx = WS_EX_NOACTIVATE;
    c->isshow = SW_SHOWNOACTIVATE;
  }
  if (WT_CHILD != type) {
    ctrl_add_tail(&g_sys->win, c);
  }
  c->style = dwStyle;
  c->type = type;
  //rc = ctrl_calc_rect(c, x, y, w, h, dwStyle);
  if (NULL == sys->hwnd) {
    sys->hwnd = CreateWindowEx(0, MYWINDOWCLASSNAME, text, MYWS_WINDOW,
        rc.l, rc.t, RCW(&rc) + 1, RCH(&rc) + 1, hWndParent, 0, c->sys->hInstance, 0); // creation parameters
    SetWindowLong(sys->hwnd, GWL_USERDATA, (LONG)(sys));
    if (ctrlproc) {
      MINMAXINFO mminfo[1] = {0};
      ctrlproc(0, c, WM_CTRLINIT, 0, 0);
      ctrlproc(0, c, WM_GETMINMAXINFO, 0, (LPARAM)mminfo);
      w = MAX(w, mminfo->ptMinTrackSize.x);
      h = MAX(h, mminfo->ptMinTrackSize.y);
      rc.r = rc.l + w;
      rc.b = rc.t + h;
      AdjustWindowRect((RECT*)&rc, dwStyle, 0);
      iRectOff2(&rc, x - rc.l, y - rc.t);
    }
    ctrl_setrect(c, rc);
    MoveWindow(sys->hwnd, rc.l, rc.t, RCW(&rc), RCH(&rc), TRUE);
    //make sure the window creation has gone OK
    if (!sys->hwnd) {
      MessageBox(NULL, "CreateWindowEx Failed!", "Error!", 0);
    }
    SetTimer(sys->hwnd, 15, 40, 0);
    //GetWindowRect(hwnd, &rc);
    //SetWindowPos(hwnd, 0, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
    if (WT_WINDOWS == type) {
      ShowWindow(sys->hwnd, SW_SHOW);
    }
    else {
      SetWindowPos(sys->hwnd, 0, rc.l, rc.t, RCW(&rc), RCH(&rc), SWP_NOACTIVATE | (WT_TOOLTIP == type ? SWP_HIDEWINDOW : SWP_SHOWWINDOW));
    }
    UpdateWindow(sys->hwnd);
  }
  else {
    c->ctrlproc(0, c, WM_CTRLINIT, 0, (LPARAM)obj);
  }
#if 0
  //InvalidateRect(hwnd, 0, 0);
  if (domodal && hWndParent) {
    //UINT ShowModal(HWND m_hWnd)
    HWND m_hWnd = sys->hwnd;
    MSG msg = { 0 };
    ASSERT(IsWindow(m_hWnd));
    //hWndParent = GetWindowOwner(m_hWnd);
    ShowWindow(m_hWnd, SW_SHOWNORMAL);
    EnableWindow(hWndParent, FALSE);
    while (IsWindow(m_hWnd) && GetMessage(&msg, NULL, 0, 0)) {
      if (msg.message == WM_CLOSE && msg.hwnd == m_hWnd) {
        ret = msg.wParam;
        EnableWindow(hWndParent, TRUE);
        SetFocus(hWndParent);
      }
      if (!TranslateMessage(&msg)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
      if (msg.message == WM_QUIT) {
        break;
      }
    }
    EnableWindow(hWndParent, TRUE);
    SetFocus(hWndParent);
    if (msg.message == WM_QUIT) {
      PostQuitMessage(msg.wParam);
    }
  }
#endif
  return ret;
}
// cvWaitKey 等待按键事件
// int cvWaitKey( int delay=0 );
// delay 延迟的毫秒数
// 数cvWaitKey无限制的等待按键事件（delay<=0时）；或者延迟"delay"毫秒。
// 回值为被按键的值，如果超过指定时间则返回-1
// 释：这个函数是HighGUI中唯一能够获取和操作事件的函数，所以在一般的事件处理中
// 需要周期地被调用，除非HighGUI被用在某些能够处理?录幕肪持?
// 者注：比如在MFC环境下，这个函数不起作用
CC_INLINE int waitkey(int delay)
{
  sys_t* sys = g_sys;
  int time0 = GetTickCount();
  for (;;) {
    MSG message;
    int is_processed = 0;
    ctrl_t* win = sys->win;
    if (NULL == win) {
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
    //for (; win && is_processed == 0; win = win->next)
    {
      if (sys->hwnd == message.hwnd) {
        is_processed = 1;
        switch (message.message) {
        case WM_DESTROY:
        case WM_CHAR:
          DispatchMessage(&message);
          return (int) message.wParam;
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
}
typedef struct null_t {
  ctrl_t c[1];
  int hit;
} null_t;
typedef struct pushbutton_s {
  ctrl_t c[1];
  int xx;
  const char* text;
} pushbutton_t;
typedef struct lable_t {
  ctrl_t c[1];
} lable_t;
typedef struct button_t {
  ctrl_t c[1];
  int hit, hot;
} button_t;
typedef struct groupbox_t {
  ctrl_t c[1];
  ctrl_t* sub;
} groupbox_t;
typedef struct curve_t {
  ctrl_t c[1];
  IPOINT points[100];
  //IPOINT mousePoint;
  IPOINT downPoint;
  int npt;
  int hit, hot;
} curve_t;
typedef struct slider_t {
  ctrl_t c[1];
  const char* text;
  double m_min, m_max, m_value;
  int m_num_steps;
  int hit;
  int hot;
  IRECT rcsub[2];
  DPOINT pt;
  DPOINT m_pdx;
  double rad;
} slider_t;
typedef struct scale_t {
  ctrl_t c[1];
  int hit;
  double m_value1;
  double m_value2;
  double m_min_d;
  double m_pdx[2];
  int m_move_what;
  unsigned m_idx;
  unsigned m_vertex;
} scale_t;
typedef struct layout_t {
  ctrl_t c[1];
  int rows, cols;
  ISIZE margin;
  ISIZE gap;
  const char* text;
  const char* id;
  int* co;
  int* ro;
  int hit;
  int hot;
  IPOINT hitbd;
  IPOINT lastdownpt;
} layout_t;
enum {_lay_x = 'x', _lay_h = '-', _lay_v = '+'};
typedef struct cut_layout_item_t {
  int ind;
  int cmd; // 命令 l, t, r, b
  int d;
  ctrl_t* c;
} cut_layout_item_t;
typedef struct cut_layout_t {
  ctrl_t c[1];
  cut_layout_item_t* li;
  int len;
  int gap;
  int mouserange; // 鼠标范围
  int hit;
  IPOINT pt;
} cut_layout_t;
#define STYLE_TOOLBAR 0x0001
#define STYLE_BITMAP 0x0002
typedef struct tooltip_t {
  ctrl_t c[1];
  DWORD time_start;
  ctrl_t* to;
  int delay;
  int show;
  int x, y;
} tooltip_t;
typedef struct tab_item {
  int id;
  int icon;
  const char* text;
} tab_item;
typedef struct tabctrl_t {
  ctrl_t c[1];
  int hit, hot;
  int cur;
  int cur_old;
  int nitem;
  tab_item* items;
  int headoff;
  //IRECT rcs[256];
  //img_t m_im[1];
  //IRECT header, client, clientin;
} tabctrl_t;
typedef struct scrollbar_t {
  ctrl_t c[1];
  int hit, hot;
  SCROLLINFO si[2]; // 0-H 1-V
  IPOINT pthit;
  IRECT rc[2][6];
  IRECT rcc; // 客户区
  int hit_pos;
  int styles;
} scrollbar_t;
typedef struct scrollview_t {
  int hit, hot;
  SCROLLINFO si[2]; // 0-H 1-V
  IPOINT pthit;
  int hit_pos;
} scrollview_t;
typedef struct imageview_t {
  ctrl_t c[1];
  int hot;
  img_t im[1];
  double scale;
  scrollview_t sc[1];
} imageview_t;
typedef struct gridctrl_t {
  ctrl_t c[1];
  //int hit, hot;
  int rows;
  int cols;
  int* rowh;
  int* colw;
  IPOINT hit;
  int hittype; // 0 no hit 1 hline 2 vline 4 cell
  IPOINT end;
  int down;
  IPOINT lt; // left top cell
  int adjusthittype;
  IPOINT adjusthit; // 调整宽度或高度
  IPOINT adjustpt;
  scrollview_t sc[1];
} gridctrl_t;
typedef struct dockinfo_t {
  ctrl_t ctrl[1];
  ctrl_t win[1];
} dockinfo_t;
typedef struct dockctrl_t {
  ctrl_t c[1];
  int hit, hot;
  dockinfo_t di[1];
} dockctrl_t;
#define GRIPPER_W 8
#define EDIT_SB_CHARW 10
#define EDIT_MARGIN 2
//typedef struct cursor_rpos
typedef IPOINT charpos_t;
typedef struct strdata_t {
  int (*get_line)(struct strdata_t* s, int irow, int icol, char_color_t* out, int maxoutlen); // 获取i行str不含 \n
  int (*ins_str)(struct strdata_t* s, const char* str, int len); // 用str 替换 [p1,p2]的内容
  int (*set_pos)(struct strdata_t* s, int x, int y);
  int (*move_pos)(struct strdata_t* s, int ichr, int iline);
  int (*get_sel)(struct strdata_t* s, str_t* str);
  charpos_t beg, pos;
  str_t s[1];
  vstr_t sv[1];
  buf_t bf[1];
} strdata_t;
typedef struct editctrl_t {
  ctrl_t c[1];
  int fmt;
  IPOINT org;
  strdata_t sd[1];
} editctrl_t;
typedef struct menuitem_t {
  int ind;
  int cmd;
  int idx; // index of imagelist,-1 means no image
  const char* text; // menu item text
} menuitem_t;
#define MENU_BDW 2
#define MENU_PENDING 2
#define MENU_MAXDEEP 4
typedef struct menuctrl_t {
  ctrl_t c[1];
  // contain all menu item of this menu
  menuitem_t* items;
  int nitem;
  int pos;
  img_t* m_img;
  int ishor; // 水平
  int hit;
  int hot;
  //int select;
  //int press;
  int open0;
  ctrl_t sub[1];
} menuctrl_t;
//measureitem_t
typedef struct drawitem_t {
  img_t* im;
  IRECT* cliprc;
  IRECT itemrc;
  int state;
} drawitem_t;
typedef struct listbox_t {
  ctrl_t c[1];
  int cursel;
  int hot;
  int ldown;
  int rdown;
  int nitem;
  scrollbar_t sc[1]; // 滚动条
} listbox_t;
typedef struct rbox_t {
  ctrl_t c[1];
  const char** m_items;
  int nitem;
  int hot;
  int hit;
} rbox_t;
typedef struct cbox_t {
  ctrl_t c[1];
  const char* text;
  int hit;
} cbox_t;
typedef struct combobox_t {
  ctrl_t c[1];
  listbox_t m_listbox[1];
  int hot;
  int hit;
  const char** m_items;
  int nitem;
} combobox_t;
typedef struct polygon_t {
  ctrl_t c[1];
  int hit;
  DPOLYGON m_polygon[1];
  IPOINT m_pt;
  BOOL m_close;
  int m_hitmark;
} polygon_t;
typedef struct spline_t {
  ctrl_t c[1];
  int hit;
  int m_num_pnt;
  double m_xp[32];
  double m_yp[32];
  double m_values[256];
  double m_xs1;
  double m_ys1;
  double m_xs2;
  double m_ys2;
  int m_vertex;
  int m_active_pnt;
  int m_move_pnt;
  double m_pdx;
  double m_pdy;
} spline_t;
typedef struct gamma_t {
  ctrl_t c[1];
  int hit;
  gamma_spline_t m_gamma_spline[1];
  double m_border_width;
  double m_border_extra;
  double m_curve_width;
  double m_grid_width;
  double m_text_thickness;
  double m_point_size;
  double m_text_height;
  DPOINT m_p1;
  DPOINT m_p2;
  int m_p1_active;
  unsigned m_mouse_point;
  double m_pdx;
  double m_pdy;
  double m_xs1;
  double m_ys1;
  double m_xs2;
  double m_ys2;
} gamma_t;
typedef struct framectrl_t {
  ctrl_t c[1];
  int hit;
  int hot;
  int x, y;
  IRECT rc1;
  IRECT rc2;
  int ismax;
  int has_cap;
  int has_bdr;
} framectrl_t;
#include "uictrl.inl"
//#include "uidraw.inl"
#endif // _UIWIN_INL_

