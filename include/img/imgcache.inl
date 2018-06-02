
#include "cfile.h"
#include "thread.h"
#include "imgio.inl"

#define MAXTHUMB_H 200
#define MAXTHUMB_W MAXTHUMB_H

typedef struct filebuf_t {
  str_t name[1];
  img_t im[1];
  img_t imthumb[1];
  PaletteEntry pal[256];
  int is_im_cached;
  int is_imthumb_cached;
  int h, w, c, f;
} filebuf_t;

typedef int (*imgcache_eventproc_t)(void* s, int x);

typedef struct imgcache_t {
  filebuf_t* v;
  int n, size;
  int running;
  int breaking;
  int cur;
  HANDLE  evtStart;
  thread_t th[1];
  critical_section_t cs[1];
  imgcache_eventproc_t fun;
  void* user;
} imgcache_t;

// sizeMalloc
int test_msize()
{
  int i = 0;

  for (i = 0; i < 30; i++) {
    void* p = pmalloc(i);
    printf("pmalloc %d-%d bytes!\n", i, _msize(p));
    pfree(p);
  }

  return 0;
}

CC_INLINE int imgcache_setsize(imgcache_t* sv, int n)
{
  int i;
  int n_old = sv->n;

  if (sv->v) {
    for (i = n; i < sv->n; ++i) {
      str_free(sv->v[i].name);
      //bffree(sv->v[i].data);
      imfree(sv->v[i].im);
    }
  }

  sv->v = (filebuf_t*)prealloc(sv->v, n * sizeof(filebuf_t));
  sv->n = n;
  return 0;
}

CC_INLINE int imgcache_ins_str(imgcache_t* sv, int i, str_t s0, int size)
{
  int n = sv->n;
  i = (i < 0 || i > n) ? n : i;

  //i = BOUND(i, 0, n);
  if (i >= n) {
    imgcache_setsize(sv, sv->n + 1);
  }

  MEMMOVE(sv->v + i + 1, sv->v + i, n - i);
  str_setstr1(sv->v[i].name, s0);
  //sv->v[i].off = sv->size;
  //sv->v[i].size = size;
  sv->v[i].is_im_cached = 0;
  sv->v[i].is_imthumb_cached = 0;
  sv->size += size;

  return 0;
}

typedef int (*finddata_fun_t)(const char* szDir, struct finddata_t* fd);

static int __filelist3(const char* szDir, struct finddata_t* fd)
{
#if 0
  imgcache_t* sv = (imgcache_t*)fd->userdata;
  int n = strlen(szDir) + strlen(fd->info->name) + 4;
  char picname[1024];
  _snprintf(picname, 1024, "%s\\%s", szDir, fd->info->name);
  imgcache_ins_str(sv, -1, STR1(picname), fd->info->size);
#endif
  return 0;
}

#include "quantizer.inl"

int filebuf_loadim(filebuf_t* fb)
{
  int ret = 0;

  if (fb->is_im_cached) {
    if (1 == fb->is_im_cached) {
      img_t* im = fb->im;
      imread(fb->name->s, 3, 0, im);
      fb->h = im->h;
      fb->w = im->w;
      fb->c = im->c;
      fb->f = im->f;
      //printf("%d %d\n", im->h, im->w);
      fb->is_im_cached = 2;
      ret |= 1;
    }
  }
  else if (fb->im->tt.data) {
    imfree(fb->im);
  }

  if (fb->is_imthumb_cached) {
    if (1 == fb->is_imthumb_cached) {
      img_t* im = fb->imthumb;
      if (fb->im->tt.data) {
        im = fb->im;
      } else {
        imread(fb->name->s, 3, 0, im);
      }
      fb->h = im->h;
      fb->w = im->w;
      fb->c = im->c;
      fb->f = im->f;
      //printf("%d %d\n", im->h, im->w);
      fb->is_imthumb_cached = 2;

      if (im->h * im->w > 0 && (im->h > MAXTHUMB_H || im->w > MAXTHUMB_W)) {
        imresizen(im, MIN(MAXTHUMB_H * 1. / im->h, MAXTHUMB_W * 1. / im->w), fb->imthumb, im->f);
      }
      else {
        imclone2(im, fb->imthumb);
      }
      imquantizer(255, fb->imthumb, fb->imthumb, fb->pal);

      ret |= 2;
    }
  }
  else if (fb->imthumb->tt.data) {
    imfree(fb->imthumb);
  }

  return ret;
}
int imgcache_thd(imgcache_t* sv)
{
  while (sv->running) {
    int i;
    WaitForSingleObject(sv->evtStart, 50);
    critical_section_enter(sv->cs);

    //printf("imgcache_thd\n");
    //sv->cur
    for (i = 0; i < sv->n; ++i) {
      filebuf_t* fb = sv->v + i;

      if (!fb->is_im_cached && fb->im->tt.data) {
        imfree(fb->im);
      }

      if (!fb->is_imthumb_cached && fb->imthumb->tt.data) {
        imfree(fb->imthumb);
      }
    }

    for (i = 0; i < sv->n && !sv->breaking; ++i) {
      int idx = sv->cur + ((i + 1) / 2) * (i & 1 ? 1 : -1);
      idx = (idx + sv->n) % sv->n;

      if (sv->fun && filebuf_loadim(sv->v + idx)) {
        sv->fun(sv->user, 0);
      }
    }

    critical_section_leave(sv->cs);
    sv->breaking = 0;
  }

  return 0;
}
int imgcache_open(imgcache_t* sv)
{
  BOOL bManualReset = FALSE;
  sv->running = 1;
  sv->evtStart = CreateEvent(NULL, bManualReset, FALSE, NULL);
  thread_create(1, sv->th, imgcache_thd, sv, 0);
  critical_section_init(sv->cs);
  return 0;
}
int imgcache_stop(imgcache_t* sv)
{
  int i;

  for (i = 0; i < sv->n; ++i) {
    sv->v[i].is_im_cached = 0;
  }

  critical_section_enter(sv->cs);

  for (i = 0; i < sv->n; ++i) {
    str_free(sv->v[i].name);
    //bffree(sv->v[i].data);
    imfree(sv->v[i].im);
  }

  sv->n = 0;
  critical_section_leave(sv->cs);
  return 0;
}
int imgcache_set_path(imgcache_t* sv, const char* path, const char* exts)
{
#if 0
  imgcache_stop(sv);
  find_file(path, exts, __filelist3, 1, sv);
  PulseEvent(sv->evtStart);
#endif
  return 0;
}
int imgcache_getim(imgcache_t* sv, int i, img_t* im)
{
  i = ((i % sv->n) + sv->n) % sv->n;

  if (im) {
    const img_t* im0 = sv->v[i].im;
    if (2 == sv->v[i].is_im_cached && im0->h>0 && im0->w>0) {
      int f = GetTickCount() * 10 / 1000;
      imclone3(im0, f, im);
      return 1;
    }
  }

  return 0;
}
int imgcache_getimthumb(imgcache_t* sv, int i, img_t* imthumb)
{
  i = ((i % sv->n) + sv->n) % sv->n;

  if (imthumb) {
    filebuf_t* fb = sv->v+i;
    const img_t* im0 = fb->imthumb;
    if (2 == fb->is_imthumb_cached && im0->h>0 && im0->w>0) {
      int f = GetTickCount() * 10 / 1000;
      //imclone3(fb->imthumb, f, imthumb);
      index2im2(im0, fb->pal, f, imthumb, 3);
      return 1;
    }
  }

  return 0;
}
int imgcache_set_range(imgcache_t* sv, int cur, int beg, int end, int page)
{
  int i;
  //printf("imgcache_set_range %d %d\n", beg, end);
  int ipage = cur / page;
  int b = cur - beg, e = cur + end;
  int pb = (ipage - beg) * page, pe = (ipage + end) * page;
  sv->cur = cur;
  sv->breaking = 1;
  b = BOUND(b, 0, sv->n);
  e = BOUND(e, 0, sv->n);
  pb = BOUND(pb, 0, sv->n);
  pe = BOUND(pe, 0, sv->n);

  for (i = 0; i < sv->n; ++i) {
    if (i >= b && i < e) {
      if (0 == sv->v[i].is_im_cached) {
        sv->v[i].is_im_cached = 1;
      }
    }
    else {
      sv->v[i].is_im_cached = 0;
    }

    if (i >= pb && i < pe) {
      if (0 == sv->v[i].is_imthumb_cached) {
        sv->v[i].is_imthumb_cached = 1;
      }
    }
    else {
      sv->v[i].is_imthumb_cached = 0;
    }
  }

  PulseEvent(sv->evtStart);
  return 0;
}
int imgcache_close(imgcache_t* sv)
{
  int i;
  sv->running = 0;
  PulseEvent(sv->evtStart);
  thread_join(1, sv->th, 100000);
  CloseHandle(sv->evtStart);
  sv->evtStart = NULL;

  for (i = 0; i < sv->n; ++i) {
    str_free(sv->v[i].name);
    //bffree(sv->v[i].data);
    imfree(sv->v[i].im);
  }

  pfree(sv->v);
  critical_section_destroy(sv->cs);
  return 0;
}
int test_imgcache()
{
  imgcache_t sv[1] = {0};
  const char* path = NULL;
  path = "E:/www/gif/addictedtoeveythingwomen.tumblr.com";
  path = "E:\\www\\mv\\aadyw.cc\\jpg";
  imgcache_open(sv);
  Sleep(1000);
  imgcache_set_path(sv, path, ".jpg|.gif");
  imgcache_set_range(sv, 10, 10, 50, 40);
  Sleep(1000);
  imgcache_set_path(sv, path, ".jpg|.gif");
  imgcache_set_range(sv, 30, 10, 50, 40);
  Sleep(1000);
  imgcache_close(sv);
  return 0;
}

int get_grid_rect(IRECT rc, int i, int row, int col, IRECT* prc)
{
  int h = RCH(&rc), w = RCW(&rc);
  int sh = h / row, sw = w / col;
  int x = i % col, y = i / col;
  int l = rc.l + x * sw, t = rc.t + y * sh;
  int r = (x == (col - 1)) ? rc.r : (l + sw), b = (y == (row - 1)) ? rc.b : (t + sh);
  iRectSet(prc, l, t, r, b);
  return 0;
}
