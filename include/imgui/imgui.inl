
#ifndef _IMGUI_INL_
#define _IMGUI_INL_
#include "draw/imdraw.inl"
//#include "img/imgopt.inl"

CC_INLINE int window_del(window_t* root, window_t* win)
{
  if (win) {
    window_t* child = win->child;
    LIST_DEL(root, win);
    for (; child; child = child->next) {
      window_del(root, child);
    }
  }
  return 0;
}
CC_INLINE int host_del(sysio_t* io, window_host_t* host)
{
  window_del(host->root, host->root);
  LIST_DEL(io->root, host);
  memset(host, 0, sizeof(*host));
  return 0;
}
CC_INLINE int host_create(sysio_t* io, window_host_t* host, const char* name) {
  int ret = 0;
  if (NULL == host->x) {
    IRECT rc = host->rect;
    int x = rc.l, y = rc.t, w = RCW(&rc), h = RCH(&rc);
    host->x = sys_create_window(0, name, x, y, w, h, host);
    {
      strncpy(host->name, name, 64);
      LIST_ADDFRONT(io->root, host);
      host->open = true;
      ret = 1;
    }
  }
  else {
    if (strcmp(host->name, name)) {
      sys_set_window_text(host->x, host->name);
      strncpy(host->name, name, 64);
    }
  }
  sys_get_window_rect(host->x, &host->rect, &host->root->rc);
  return ret;
}

CC_INLINE int null_SetTexture(render_drv_t* drv, texture_t* tex, PixFmt fmt, int height, int width, void* data, int step)
{
  int bpp = GetPixFmtBpp(fmt);
  bmp_setsize(tex, height, width, bpp);
  if (tex->data && data) {
    int y;
    uchar* pixels = (uchar*)data;
    uchar* pFontTexture = (uchar*)tex->data;
    int bytes_per_line = (width * bpp + 7) / 8;
    for (y = 0; y < height; y++) {
      memcpy(pFontTexture + y * tex->s, pixels + y * step, bytes_per_line);
    }
  }
  return 0;
}
CC_INLINE int null_Render(render_drv_t* drv, const gc_t* ctx)
{
  null_drv_t* g = (null_drv_t*)drv->x;
  img_t* im = g->im1;
  img_t* im1 = g->im;
  if (im1->h>0 && im1->w>0) {
  //IMRESIZE(im->h, im->w, im->tt.data, im->s, im->c, im1->h, im1->w, im1->tt.data, im1->s, im1->c, INTER_BILINEARITY);
    IMRESIZE(im->h, im->w, im->tt.data, im->s, im->c, im1->h, im1->w, im1->tt.data, im1->s, im1->c, INTER_NEARSAMPLE);
    sys_window_bitblt(g->x, 0, 0, g->im->w, g->im->h, g->hMemDC, 0, 0);
  }
  return 0;
}
CC_INLINE int null_SetSize(render_drv_t* drv, int w, int h)
{
  null_drv_t* g = (null_drv_t*)drv->x;
  sys_dc_set_size(&g->hMemDC, h, w, WINBMPCN);
  sys_dc_get_image(g->hMemDC, g->im);
  drv->rcDirty = iRECT(0, 0, w, h);
  return 0;
}
CC_INLINE int null_UnInit(render_drv_t* drv)
{
  null_drv_t* g = (null_drv_t*)drv->x;
  sys_del_dc(g->hMemDC);
  myMemFree(g);
  memset(drv, 0, sizeof(*drv));
  return 0;
}
CC_INLINE int null_Init(window_host_t* host)
{
  render_drv_t* drv = (render_drv_t*)host->drv;
  null_drv_t* g = NULL;
  int w, h;
  IRECT rc = {0};
  myReAlloc(g, 1);
  drv->x = g;
  drv->UnInit = null_UnInit;
  drv->Render = null_Render;
  drv->SetSize = null_SetSize;
  drv->SetTexture = null_SetTexture;
  //drv->CopyToScreen = null_CopyToScreen;
  sys_get_window_rect(host->x, 0, &rc);
  w = rc.r - rc.l;
  h = rc.b - rc.t;
  sys_dc_set_size(&g->hMemDC, h, w, WINBMPCN);
  sys_dc_get_image(g->hMemDC, g->im);
  
  g->x = host->x;
  drv->rcDirty = iRECT(0, 0, w, h);
  return 1;
}
CC_INLINE int null_event(sysio_t* io) {
  window_host_t* host = io->msg_host;
  render_drv_t* drv = (render_drv_t*)host->drv;
  null_drv_t* g = (null_drv_t*)drv->x;
  if (g && MSG_MOUSEMOVE==io->msg) {
    char buf[64];
    img_t* im = g->im1; // 
    img_t* im1 = g->im;
    int x = io->mouse->pos->x;
    int y = io->mouse->pos->y;
    int x1, y1;
    uchar* pix;
    x1 = x * im->w/im1->w;
    y1 = y * im->h/im1->h;
    x1 = BOUND(x1, 0, im->w-1);
    y1 = BOUND(y1, 0, im->h-1);
    pix = im->tt.data + (im->h-1-y1)*im->s + x1*im->c;
    //printf("%d %d\n", x1, y1);
    _snprintf(buf, countof(buf), "X=%d Y=%d R=%d G=%d B=%d", x1, y1, pix[2], pix[1], pix[0]);
    sys_set_window_text(host->x, buf);
  }
  return 0;
}
CC_INLINE window_host_t* host_find_name(sysio_t* io, const char* name) {
  window_host_t* host = io->root;
  for (;host; host = host->next) {
    if (0==strcmp(name, host->name)) {
      return host;
    }
  }
  return NULL;
}

CC_INLINE int imsetbitmap_cn4(img_t* im, int h, int w, const void* data, int step, int cn, PixFmt fmt, bool flip_y, bool scaling) {
  imsetsize(im, h, w, 4, 1);
  img_setbitmap_cn4(h, w, data, step, cn, fmt, flip_y, scaling, im->tt.data, im->s, NULL);
  return 0;
}
#define show_im(name, im)  show_bitmap(name, im->h, im->w, im->tt.data, im->s, im->c, cn2PixFmt(im->c), 1, 0)
CC_INLINE int show_bitmap(const char* name, int h, int w, const void* bit, int step, int cn, PixFmt fmt, bool flip_y, bool scaling) {
  sysio_t* io = sys_getio();
  window_host_t* host = host_find_name(io, name);
  static int pos_x = 20, pos_y = 20;
  COLOR clrbg = 0;
  if (NULL==host) {
    MYREALLOC(host, 1);
    MEMSET(host, 0, 1);
    host->rect = iRECT2(pos_x, pos_y, w, h);
    host->event_fun = null_event;
    if (host_create(io, host, name)) {
      null_Init(host);
      pos_x += 20, pos_y += 20;
    }
  } else {
    //sys_set_window_rect();
  }
  
  if (host && host->drv) {
    render_drv_t* drv = host->drv;
    null_drv_t* g = (null_drv_t*)drv->x;
    imsetbitmap_cn4(g->im1, h, w, bit, step, cn, fmt, flip_y, scaling);
    drv->Render(drv, NULL);
  }
  return 0;
}

CC_INLINE int waitkey(int delay) {
  sysio_t* io = sys_getio();
  int time0 = sys_tick_count();
  while (1) {
    int tt = sys_tick_count() - time0;
    if ((delay > 0 && abs(tt) >= delay) || io->root == 0) {
      return -1;
    }
    sys_waitio(MAX(delay, 10));
    if (MSG_CHAR==io->msg) {
      return io->last_key;
    }
  }
  return -1;
}

#define flex_direction_mask                  3// 主轴，从而定义了伸缩项目放置在伸缩容器的方向。
#define flex_row                             0// (默认值)                                从左向右排列
#define flex_row_reverse                     1// 从右向左排列
#define flex_column                          2// 从上到下排列
#define flex_column_reverse                  3// 从下到上排列
#define flex_wrap_mask                       (3<<2)// 如何换行
#define flex_nowrap                          0// (默认值)                                不换行
#define flex_wrap                            (1<<2)// 换行，第一行在上方
#define flex_wrap_reverse                    (2<<2)// 换行，第一行在下方
#define flex_justify_content_mask            (7<<4)// 主轴的对齐方式
#define flex_start                           0//(默认值)：向一行的起始位置靠齐
#define flex_end                             (1<<4)// 向一行的结束位置靠齐
#define flex_center                          (2<<4)//向一行的中间位置靠齐
#define flex_space_between                   (3<<4)//会平均地分布在行里。第一个在一行中的最开始位置，最后一个在一行中最终点位置
#define flex_space_around                    (4<<4)//会平均地分布在行里，两端保留一半的空间
#define flex_stretch                         (5<<4)//会平均地分布在行里，两端保留一半的空间
#define flex_align_items_mask                (7<<7)// 侧轴上对齐方式
#define flex_istretch                        0// （默认值）：元素拉伸填充整个伸缩容器
#define flex_istart                          (1<<7)// 元素在侧轴起点边的外边距紧靠住该行在侧轴起始的边
#define flex_iend                            (2<<7)// 在侧轴终点边的外边距靠住该行在侧轴终点的边
#define flex_icenter                         (3<<7)// 外边距盒在该行的侧轴上居中放置
//#define flex_baseline                      (4<<7)// 根据他们的基线对齐
#define flex_align_content_mask              (7<<10)// 多根轴线的对齐方式。如果项目只有一根轴线，该属性不起作用
#define flex_cstretch                        0//（默认值）各行将会伸展以占用剩余的空间。
#define flex_cstart                          (1<<10)//各行向伸缩容器的起点位置堆叠
#define flex_cend                            (2<<10)//各行向伸缩容器的结束位置堆叠
#define flex_ccenter                         (3<<10)//各行向伸缩容器的中间位置堆叠
#define flex_cspace_between                  (4<<10)//各行在伸缩容器中平均分布
#define flex_cspace_around                   (5<<10)//各行在伸缩容器中平均分布，在两边各有一半的空间

int flexbox_items(int x, int w, int n, const void* sz, int sz_step, void* pt, int pt_step, void* os, int os_step, int fmt)
{
  const uchar* sz0 = (const uchar*)(sz);
  const uchar* pt0 = (const uchar*)(pt);
  const uchar* os0 = (const uchar*)(os);
  int i, e = x + w;
  switch (fmt) {
  case flex_istretch:
    for (i = 0; i < n; ++i) {
      int* sz1 = (int*)(sz0 + i * sz_step);
      int* pt1 = (int*)(pt0 + i * pt_step);
      int* os1 = (int*)(os0 + i * os_step);
      *pt1 = x;
      *os1 = w;
    }
    break;
  case flex_istart:
    for (i = 0; i < n; ++i) {
      int* sz1 = (int*)(sz0 + i * sz_step);
      int* pt1 = (int*)(pt0 + i * pt_step);
      int* os1 = (int*)(os0 + i * os_step);
      *pt1 = x;
      *os1 = *sz1;
    }
    break;
  case flex_iend:
    for (i = 0; i < n; ++i) {
      int* sz1 = (int*)(sz0 + i * sz_step);
      int* pt1 = (int*)(pt0 + i * pt_step);
      int* os1 = (int*)(os0 + i * os_step);
      *pt1 = e - *sz1;
      *os1 = *sz1;
    }
    break;
  case flex_icenter:
    for (i = 0; i < n; ++i) {
      int* sz1 = (int*)(sz0 + i * sz_step);
      int* pt1 = (int*)(pt0 + i * pt_step);
      int* os1 = (int*)(os0 + i * os_step);
      *pt1 = x + (w - *sz1) / 2;
      *os1 = *sz1;
    }
    break;
  }
  return 0;
}
int flexbox_content(int x, int w, int n, const void* sz, int sz_step, void* pt, int pt_step, void* os, int os_step, int gap, int fmt, int is_align)
{
  const uchar* sz0 = (const uchar*)(sz);
  const uchar* pt0 = (const uchar*)(pt);
  const uchar* os0 = (const uchar*)(os);
  int i, sumsz = 0, agap = (n - 1) * gap;
  double gapx = gap, offx = 0, szx = 0, s;
  for (i = 0; i < n; ++i) {
    int* sz1 = (int*)(sz0 + i * sz_step);
    sumsz += *sz1;
  }
  s = w - sumsz - agap;
  if (is_align) {
    switch (fmt) {
    case flex_cstretch:
      fmt = flex_stretch;
      break;
    case flex_cstart:
      fmt = flex_start;
      break;
    case flex_cend:
      fmt = flex_end;
      break;
    case flex_ccenter:
      fmt = flex_center;
      break;
    case flex_cspace_between:
      fmt = flex_space_between;
      break;
    case flex_cspace_around:
      fmt = flex_space_around;
      break;
    }
  }
  switch (fmt) {
  case flex_stretch:
    szx = s / n;
    break;
  case flex_start:
    break;
  case flex_end:
    offx = s;
    break;
  case flex_center:
    offx = s / 2;
    break;
  case flex_space_between:
    gapx += s / (n - 1);
    break;
  case flex_space_around:
    offx = s / (n * 2);
    gapx += s / n;
    break;
  }
  offx += x;
  for (i = 0; i < n; ++i) {
    int* sz1 = (int*)(sz0 + i * sz_step);
    int* pt1 = (int*)(pt0 + i * pt_step);
    int* os1 = (int*)(os0 + i * os_step);
    *pt1 = (int)offx;
    *os1 = (int)(*sz1 + szx);
    offx += *sz1 + szx + gapx;
  }
  return sumsz + agap;
}
int flexbox(IRECT r, int n, const void* sz, int sz_step, void* pt, int pt_step, void* os, int os_step, const int* gap, int fmt, int* osz)
{
  const uchar* sz0 = (const uchar*)(sz);
  const uchar* pt0 = (const uchar*)(pt);
  const uchar* os0 = (const uchar*)(os);
  int direction = fmt & flex_direction_mask;
  int wrap = fmt & flex_wrap_mask;
  int justify_content = fmt & flex_justify_content_mask;
  int align_items = fmt & flex_align_items_mask;
  int align_content = fmt & flex_align_content_mask;
  int reverse = flex_row_reverse == direction || flex_column_reverse == direction;
  int i, j, k, l, x, y;
  int rsz[2] = {RCW(&r), RCH(&r)};
  int rpt[2] = {r.l, r.t};
  int maxsz[2] = {0};
  x = flex_column == direction || flex_column_reverse == direction;
  y = !x;
  if (wrap) {
    enum {maxk = 64};
    int len[maxk] = {0};
    int rysz[maxk] = {0};
    int rypt[maxk] = {0};
    int ryos[maxk] = {0};
    for (i = 0, k = 0, l = 0; i < n; ++i) {
      int* sz1 = (int*)(sz0 + i * sz_step);
      maxsz[0] = MAX(maxsz[0], sz1[0]);
      maxsz[1] = MAX(maxsz[1], sz1[1]);
      l += sz1[x];
      if (l > rsz[x]) {
        ++k;
        l = sz1[x];
      }
      l += gap[x];
      len[k]++;
    }
    ++k;
    if (0) {
      for (i = 0; i < k; ++i) {
        rysz[i] = maxsz[y];
      }
    }
    else {
      for (l = 0, i = 0; i < k; ++i) {
        rysz[i] = 0;
        for (j = 0; j < len[i]; ++j) {
          int* sz1 = (int*)(sz0 + (l + j) * sz_step);
          rysz[i] = MAX(rysz[i], sz1[y]);
        }
        l += len[i];
      }
    }
    osz[x] = rsz[x];
    osz[y] = flexbox_content(rpt[y], rsz[y], k, rysz, sizeof(int), rypt, sizeof(int), ryos, sizeof(int), gap[y], align_content, 1);
    if (flex_wrap_reverse == wrap) {
      int re = rpt[y] + rpt[y] + rsz[y];
      for (i = 0; i < k; ++i) {
        rypt[i] = re - rypt[i] - ryos[i];
      }
    }
    for (l = 0, i = 0; i < k; ++i) {
      flexbox_content(rpt[x], rsz[x], len[i], (int*)(sz0 + l * sz_step) + x, sz_step, (int*)(pt0 + l * pt_step) + x, pt_step, (int*)(os0 + l * os_step) + x, os_step, gap[x], justify_content, 0);
      flexbox_items(rypt[i], ryos[i], len[i], (int*)(sz0 + l * sz_step) + y, sz_step, (int*)(pt0 + l * pt_step) + y, pt_step, (int*)(os0 + l * os_step) + y, os_step, align_items);
      l += len[i];
    }
  }
  else {
    int sumsz[2] = {0};
    for (i = 0, k = 0, l = 0; i < n; ++i) {
      int* sz1 = (int*)(sz0 + i * sz_step);
      maxsz[0] = MAX(maxsz[0], sz1[0]);
      maxsz[1] = MAX(maxsz[1], sz1[1]);
      sumsz[0] += sz1[0];
      sumsz[1] += sz1[1];
    }
    osz[x] = sumsz[x];
    osz[y] = maxsz[y];
    flexbox_content(rpt[x], rsz[x], n, (int*)sz + x, sz_step, (int*)pt + x, pt_step, (int*)os + x, os_step, gap[x], justify_content, 0);
    flexbox_items(rpt[y], rsz[y], n, (int*)sz + y, sz_step, (int*)pt + y, pt_step, (int*)os + y, os_step, align_items);
  }
  if (reverse) {
    int re = rpt[x] + rpt[x] + rsz[x];
    for (i = 0; i < n; ++i) {
      int* sz1 = (int*)(sz0 + i * sz_step);
      int* pt1 = (int*)(pt0 + i * pt_step);
      int* os1 = (int*)(os0 + i * os_step);
      pt1[x] = re - pt1[x] - os1[x];
    }
  }
  for (i = 0; i < n; ++i) {
    int* sz1 = (int*)(sz0 + i * sz_step);
    int* pt1 = (int*)(pt0 + i * pt_step);
    int* os1 = (int*)(os0 + i * os_step);
    os1[0] += pt1[0];
    os1[1] += pt1[1];
  }
  return 0;
}

ISIZE flexboxlay(IRECT r, int n, ctrl_t* c, int gapxy, int fmt)
{
  int gap[2] = {gapxy, gapxy};
  int osz[2] = {0, 0};
  //gap = gap ? gap : gap0;
  flexbox(r, n, &c->sz, sizeof(ctrl_t), &c->rc.l, sizeof(ctrl_t), &c->rc.l + 2, sizeof(ctrl_t), gap, fmt, osz);
  return iSIZE(osz[0], osz[1]);
}

static const uchar menuimg_9x324[] = {
#include "draw/menuimg_9x324.txt"
};

static const raster_font_t songti_12[] = {
#include "font/songti_12.txt"
};
//#include "ui/window.inl"
static int createTexFont(render_drv_t* drv, texfont_t* font)
{
  img_t im[1] = {0};
  font_t fo[1] = {0};
  //show_bitmap("asdf", 9, 324, menuimg_9x324, sizeof(menuimg_9x324)/9, PF_1bppIndexed, 1, 1);
  //waitkey(-1);
  font_init_memraster(fo, "songti", songti_12);
  packing_raster_font(fo, font, im);
  drv->SetTexture(drv, font->tex, cn2PixFmt(im->c), im->h, im->w, im->tt.data, im->s);
  imfree(im);
  return 0;
}
static ISIZE measureText1(const char* text, const char* text_end, texfont_t* font, float size, int formatFlags, IRECT* prc)
{
  IRECT rc = prc ? *prc : iRECT2(0, 0, 10000, 10000);
  float w = RCW(&rc);
  ImVec2 sz;
  ISIZE szout;
  sz = texfont_CalcTextSizeA(font, size, w, w, text, text_end, NULL);
  szout.w = (int)sz.x, szout.h = (int)sz.y;
  if (prc) {
    iRectAlign(rc, szout.w, szout.h, formatFlags, &rc);
    *prc = rc;
  }
  return szout;
}
static ISIZE measureText(const char* text, const char* text_end, font_t* font, float size, int formatFlags, IRECT* prc)
{
  IRECT rc = prc ? *prc : iRECT2(0, 0, 10000, 10000);
  float w = (float)RCW(&rc);
  ImVec2 sz;
  ISIZE szout;
  sz = font_CalcTextSizeA(font, size, w, w, text, text_end, NULL);
  szout.w = (int)sz.x, szout.h = (int)sz.y;
  if (prc) {
    iRectAlign(rc, szout.w, szout.h, formatFlags, &rc);
    *prc = rc;
  }
  return szout;
}

typedef struct {
  window_native_t* x;
  window_dc_t* hMemDC;
  softgc sg[1];
} soft_drv_t;

static int soft_drv_SetTexture(render_drv_t* drv, texture_t* tex, PixFmt fmt, int height, int width, void* data, int step)
{
  int bpp = GetPixFmtBpp(fmt);
  bmp_setsize(tex, height, width, bpp);
  if (tex->data && data) {
    int y;
    uchar* pixels = (uchar*)data;
    uchar* pFontTexture = (uchar*)tex->data;
    int bytes_per_line = (width * bpp + 7) / 8;
    for (y = 0; y < height; y++) {
      memcpy(pFontTexture + y * tex->s, pixels + y * step, bytes_per_line);
    }
  }
  return 0;
}
int soft_drv_Render(render_drv_t* drv, const gc_t* ctx)
{
  soft_drv_t* sd = (soft_drv_t*)drv->x;
  const char* va = ctx->buf;
  const char* vb = ctx->buf + ctx->len;
  softgc_run(sd->sg, va, vb);
  sys_window_bitblt(sd->x, 0, 0, sd->sg->im->w, sd->sg->im->h, sd->hMemDC, 0, 0);
  drv->rcDirty = iRECT2(0, 0, 0, 0);
  return 0;
}
int soft_drv_SetSize(render_drv_t* drv, int w, int h)
{
  soft_drv_t* sd = (soft_drv_t*)drv->x;
  sys_dc_set_size(&sd->hMemDC, h, w, WINBMPCN);
  sys_dc_get_image(sd->hMemDC, sd->sg->im);
  drv->rcDirty = iRECT(0, 0, w, h);
  return 0;
}
int soft_drv_UnInit(render_drv_t* drv)
{
  soft_drv_t* sd = (soft_drv_t*)drv->x;
  sys_del_dc(sd->hMemDC);
  softgc_free(sd->sg);
  //myMemFree(sd);
  memset(drv, 0, sizeof(*drv));
  return 0;
}
int soft_drv_Init(window_host_t* host, int iscmdmode, int swap_rb)
{
  render_drv_t* drv = (render_drv_t*)host->drv;
  static soft_drv_t sd[1] = {0};
  int w, h;
  IRECT rc = {0};
  drv->x = sd;
  drv->UnInit = soft_drv_UnInit;
  drv->Render = soft_drv_Render;
  drv->SetSize = soft_drv_SetSize;
  drv->SetTexture = soft_drv_SetTexture;
  //drv->CopyToScreen = soft_drv_CopyToScreen;
  sys_get_window_rect(host->x, 0, &rc);
  w = RCW(&rc);
  h = RCH(&rc);
  sys_dc_set_size(&sd->hMemDC, h, w, WINBMPCN);
  sys_dc_get_image(sd->hMemDC, sd->sg->im);
  softgc_init(sd->sg, swap_rb);
  sd->x = host->x;
  sd->sg->flip_y = host->flip_y;
  drv->rcDirty = iRECT(0, 0, w, h);
  if (iscmdmode) {
    cmdgc_init(host->root->g);
  } else {
    gc_set_softgc(host->root->g, sd->sg);
  }
  //host->w = w;
  //host->h = h;
  //ASSERT(sd->sg->im->w == w && sd->sg->im->h == h);
  return 1;
}

CC_INLINE int iRectLayoutEx(IRECT rc, int cx, int cy, UINT fmt, int gap, IRECT* out, IRECT* prc, layerinfo_t* li)
{
  int xf = fmt & 0x07;
  int yf = (fmt >> 4) & 0x7;
  int retx, rety, ret = 0;
  IRECT rc2[1] = {0};
  IRECT* prc1 = (LF_alighParentLeft == yf) ? &li->rcLT : &li->rcRB;
  IRECT* prc2 = (LF_alighParentLeft == xf) ? &li->rcLT : &li->rcRB;
  int cc = (LF_alighParentLeft == xf || LF_alighParentRight == xf) && (LF_alighParentLeft == yf || LF_alighParentRight == yf);
  int cc1 = (LF_alighParentLeft == yf || LF_alighParentRight == yf);
  *prc = *out = *rc2 = rc;
  if (1) {
    if (cc && cx <= RCW(prc1)) {
      retx = iRectLayout1D(prc1->l, prc1->r, cx, xf, gap, &out->l, &out->r, &prc1->l, &prc1->r);
      if (LF_alighParentLeft == yf) {
        prc->t = MAX(prc->t, prc1->t + cy + gap);
        out->t = prc1->t;
        out->b = prc->t - gap;
      }
      else {
        prc->b = MIN(prc->b, prc1->b - cy - gap);
        out->t = prc->b + gap;
        out->b = prc1->b;
      }
      ret = 1;
      {
        int h = RCH(&li->rect) - RCH(prc);
        li->Size.y = MAX(li->Size.y, h);
        li->Size.x = MAX(li->Size.x, cx);
      }
    }
    else {
      li->rcLT = li->rcRB = iRECT(0, 0, 0, 0);
      retx = iRectLayout1D(rc.l, rc.r, cx, xf, gap, &out->l, &out->r, &rc2->l, &rc2->r);
      rety = iRectLayout1D(rc.t, rc.b, cy, yf, gap, &out->t, &out->b, &rc2->t, &rc2->b);
      if (retx && rc2->l < rc2->r && cc) {
        if (LF_alighParentLeft == yf) {
          prc1->t = rc.t;
          prc1->b = rc.t + cy;
        }
        else {
          prc1->t = rc.b - cy;
          prc1->b = rc.b;
        }
        prc1->l = rc2->l;
        prc1->r = rc2->r;
      }
      if (LF_alighParentLeft == yf) {
        prc->t += cy + gap;
        li->Size.x = MAX(li->Size.x, cx);
        li->Size.y += cy + gap;
      }
      else if (LF_alighParentRight == yf) {
        prc->b -= cy + gap;
        li->Size.x = MAX(li->Size.x, cx);
        li->Size.y += cy + gap;
      }
      else {
        if (LF_alighParentLeft == xf) {
          prc->l += cx + gap;
          li->Size.x += cx + gap;
          li->Size.y = MAX(li->Size.y, cy);
        }
        else {
          prc->r -= cx + gap;
          li->Size.x += cx + gap;
          li->Size.y = MAX(li->Size.y, cy);
        }
      }
      ret = retx || rety;
    }
  }
  li->irc += !ret;
  return ret;
}
#if 0
int calc_rect(int cx, int cy, int fmt, IRECT* prc)
{
  SYSIO;
  layerinfo_t* li = la->layinfo + la->deep - 1;
  if (prc) {
    if (li->irc < la->nrcs) {
      int ret = iRectLayoutEx(la->rcs[li->irc], cx, cy, fmt, la->gap, prc, &la->rcs[li->irc], li);
      return 1;
    }
  }
  return 0;
}
void set_layout_flags(int layoutflags)
{
  SYSIO;
  la->layoutflags = layoutflags;
}
#endif
void host_PushAllCtx(gc_t* g, window_t* c)
{
  if (c) {
    if (c->g->buf && c->g->len > 0) {
      char* p = (char*)myAllocPush(g->buf, g->len, c->g->len);
      memcpy(p, c->g->buf, c->g->len);
      c->g->len = 0;
    }
    host_PushAllCtx(g, c->next);
    host_PushAllCtx(g, c->child);
  }
}
window_t* find_window_by_pt(window_t* c, int x, int y)
{
  window_t* ret = NULL;
  if (c) {
    if (ret = find_window_by_pt(c->next, x, y)) {
      return ret;
    }
    if (ret = find_window_by_pt(c->child, x, y)) {
      return ret;
    }
    if (iPtInRect(&c->rc, x, y)) {
      return c;
    }
  }
  return ret;
}
int host_begin(window_host_t* host)
{
  // Create application window
  sysio_t* io = sys_getio();
  io->cur_win = host->root;
  io->cur_host = host;
  io->g = host->root->g;
  gcSetDirty(io->g, host->drv->rcDirty, TRUE);
  gcClipRect(io->g, NULL);
  {
    IRECT rc = host->root->rc;
    int x = io->mouse->pos->x;
    int y = io->mouse->pos->y;
    int hot = iPtInRect(&rc, x, y);
    //host->drv->SetSize(host->drv, RCW(&rc), RCH(&rc));
    host->root->client->rc = rc;
    switch (io->msg) {
    case MSG_LBUTTONDOWN:
      break;
    case MSG_MOUSEMOVE:
      if (hot) {
        if (!io->mouse->down[0]) {
          host->hot_win = find_window_by_pt(host->root, x, y);
        }
      }
      break;
    case MSG_CLOSE:
      if (host) {
        if (host->drv->UnInit) {
          host->drv->UnInit(host->drv);
        }
        host_del(io, host);
        return 0;
      }
      break;
    }
  }
  return 1;
}
int host_end(window_host_t* host)
{
  sysio_t* io = sys_getio();
  render_drv_t* drv = host->drv;
  if (drv->Render) {
    gc_t g[1] = {0};
    host_PushAllCtx(g, host->root);
    host_PushAllCtx(g, host->tooltip);
    drv->Render(drv, g);
    myFree(g->buf, g->len);
  }
  return 0;
}
// #define host_open(host, x, y, w, h) host->open = true, host->rect = iRECT2(x, y, w, h)
ctrl_t* host_wait(int delay) {
  static window_host_t ghost[1] = {0};
  static font_t fo[1] = {0};
  static int inited = 0;
  if (!inited) {
    sysio_t* io = sys_getio();
    inited = 1;
    ghost->open = true;
    // host_open(ghost, 200, 200, 800, 600);
    ghost->rect = iRECT2(200, 200, 800, 600);
    host_create(io, ghost, "demo");
    soft_drv_Init(ghost, 0, 0);
    //font_init_memraster(fo, "songti", songti_12);
    HZK16_font_init(fo);
    io->font = fo;
  }
  host_end(ghost);
  if (sys_waitio(delay) && host_begin(ghost)) {
    SYSIO;
    FORCE_REDRAW(ghost->rect);
    return ghost->root->client;
  }
  return NULL;
}
int host_run(int (*ctrl)(ctrl_t* x), int delay) {
  ctrl_t* c = NULL;
  for (; c = host_wait(delay);) {
    ctrl(c);
  }
  return 0;
}

#if 0

int test_imgui_sample()
{
  window_host_t host[2] = {0};
  ctrl_t cc[10] = {0};
  double time1 = 0;
  int pos = 0, tpos = 0;
  sysio_t* io = sys_getio();
  font_t fo[1] = {0};
  io->font = fo;
  host_open(host, 200, 200, 800, 600);
  host_create(io, host, "asdf");
  soft_drv_Init(host, 0, 0);
  font_init_memraster(fo, "songti", songti_12);
  for (; sys_waitio(100) && host_begin(host);) {
    utime_start(_start_time);
    host_end(host, _rgb(1, 1, 1));
    time1 = utime_elapsed(_start_time);
  }
  return 0;
}
#endif

#include "ctrl.inl"
#include "edit.inl"

#endif // _IMGUI_INL_
