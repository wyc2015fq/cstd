
#include "cstd.h"
#include "cfile.h"

#include "imgio.inl"

static int test_allfmt()
{
  int i;
  vstr_t sv[1] = {0};
  img_t im[1] = {0};
  buf_t bf[1] = {0};
  sys_chdir("E:/www/pic/www.juemei.cc/jpg");
  sys_chdir("E:/www/gif/addictedtoeveythingwomen.tumblr.com/down");
  vstr_load("list.txt", sv);
  
  for (i = 0; i < sv->n; ++i) {
    imfree(im);
    buf_load(sv->v[i].s, bf);
    {
      utime_start(_start_time);
      imload_mem(bf->data, bf->len, 3, im);
      printf("%8.5f %4d %4d %3d %s\n", utime_elapsed(_start_time), im->w, im->h, im->f, GetFileNameExt(sv->v[i].s));
    }
    ASSERT(im->h > 0);
    //imshow(im);
    //cvWaitKey(100);
  }
  
  imfree(im);
  bffree(bf);
  return 0;
}

#include "draw/imdraw.inl"
#include "cv/cv.inl"
#include "face/face.inl"
#include "face/cascadearr.h"

static int test_imgio()
{
  img_t im[1] = {0};
  int i, j;
  if (1) {
    vstr_t sv[1] = {0};
    char buf[256];
    sys_chdir("E:/pub/pic");
    sys_chdir("E:/pub/pic/aaa");
    //test_allfmt();
    //buf_load("baboon.jpg", bf);
    dirvstr(sv, "D:/code/CLM-framework-master/Release/videos", ".jpg", FF_FULLNAME);
    for (i = 0; i < sv->n; ++i) {
      if (imread(sv->v[i].s, 3, 1, im)) {
        ReplaceExt(sv->v[i].s, "txt", buf, countof(buf));
        str_t s[1] = {0};
        str_load(buf, s);
        DRECT dr;
        sscanf(s->s, "%lf %lf %lf %lf", &dr.l, &dr.t, &dr.r, &dr.b);
        str_free(s);
        XRECT outr[100];
        int ret = cvFaceDetect(vis_nesting_face20110713, im, NULL, 200./im->h, NULL, outr, countof(outr));
        imdrawaa_rect(im, false, NULL, dr, 0, ColorRed, 2);
        if (ret>0) {
          for (j=0; j<ret; ++j) {
            dr = dRECT2(outr[j].x, outr[j].y, outr[j].w, outr[j].h);
            imdrawaa_rect(im, false, NULL, dr, 0, ColorGreen, 2);
          }
        }
        imshow_(im);
        WaitKey(-1);
      }
    }
    
    vstr_free(sv);
  }
#if 0
  if (1) {
    img_t im[1] = {0};
    uchar pal[1024] = {0};
    //xpm_load();
    sys_chdir("E:/pub/pic/Êó±êÑùÊ½");
    ico_load_filename("01414.ico", 4, im, 0, 0);
    //ani_load_filename("0001.ani", 4, im, 0, 0);
    //xpm_load_filename("open.xpm", 4, im, 0, 0);
    imshowpal("im", im, 0);
    WaitKey(-1);
  }
  if (0) {
    img_t im[1] = {0};
    uchar pal[1024] = {0};
    xpm_load_from_str(open_xpm, 4, im, 0, pal);
    //xpm_load();
    sys_chdir("E:/code/cstd/include/ui/buttons");
    //xpm_load_filename("open.xpm", 4, im, 0, 0);
    if (1) {
      memset(pal, 0, 1024);
      imquantizer(64, im, im, (PaletteEntry*)pal);
      xpm_save_filename("open1.xpm", im, pal);
    }
    imshowpal("im", im, (RGBQUAD*)pal);
    WaitKey(-1);
  }
#endif
  return 0;
}
