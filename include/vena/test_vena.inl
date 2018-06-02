
#define _WINDOWS
//#include "debug.h"
#include "cstd.h"

#include "img/imgio.inl"
//#include "draw/imdraw.inl"
#include <string.h>

//#include "cap/libcap.cpp"
#include "sftime.h"
//#include "draw/test_draw_circle.inl"

#include "vena.inl"

#define PICNUM 10
#define VENA_THD  5000
#define VENA_STEP (VENA_FEATLEN+64)
#define VENA_LIBFILE "./venalib.dat"

//#include "test_venaui.inl"

int test_vena() {
  img_t im[10] = {0};
  uchar im3_data[VENA_STDH * VENA_STDW];
  uchar feat[VENA_FEATLEN];
  img_t im3[1] = {0};
  int i, k=0;
  buf_t bf[1] = {0};
  //return test_draw_circle(); vena_recog_bf
  bfsetsize(bf, 10*1000*1000);
  //test_lbp();
  IMINIT(im3, VENA_STDH, VENA_STDW, im3_data, VENA_STDW, 1, 1);
#ifdef _DEBUG
  sys_chdir("E:/code/cstd/src/vena/ROI提取");
#endif
  if (1) {
    int i, n, id=0, dis=0;
    vstr_t sv[1] = {0};
    sys_chdir("E:/pub/bin/vena");
    vstr_load("list.txt", sv);
    for (i=0; i<sv->n; ++i) {
      imread(sv->v[i].s, 1, 1, im);
      n = vena_recog_bf(bf->data, bf->len, im->h, im->w, im->tt.data, im->s, im->c, 0, feat, VENA_FEATLEN, im3_data, feat, &id, &dis);
      //n = vena_recog_bf(bf->data, bf->len, im->h, im->w, im->tt.data, im->s, im->c, 0, feat, VENA_FEATLEN, im3_data, feat, &id, &dis);
      //imshow(im3);cvWaitKey(0);
    }
    return 0;
  }
  if (0) {
    MEM_CHECK_BEGIN();
    imread("b.bmp", 1, 1, im);
    k = vena_detect(bf, im, im3_data);
    imfree(im3);
    imfrees(im, 10);
    MEM_CHECK_END();
    if (1) {
      //imshow(im3);
      //imshow(BW2);
      //imshow(shuru1);
      //imshow(im1);
      //imshow(im);
      //cvWaitKey(-1);
    }
  }
  if (0) {
    imread("b.bmp", 1, 1, im);
    //imread("cap/cc00018.bmp", 1, 1, im);
    k = vena_detect(bf, im, im3_data);
    if (1) {
      imshow(im3);
      imshow(im);
      //imshow(shuru1);
      //imshow(im1);
      //imshow(im);
      cvWaitKey(-1);
    }
  }
  if (0) {
    vstr_t sv[1] = {0};
    vstr_load("list.txt", sv);
    for (i=0; i<sv->n; ++i) {
      imread(sv->v[i].s, 1, 1, im);
      k = vena_detect(bf, im, im3_data);
      if (1) {
        if (k) {
          //k = vena_detect(im, im3);
          imshow(im3);
          printf("w=%d h=%d\n", im3->w, im3->h);
        }
        //imshow(BW2);
        //imshow(shuru1);
        //imshow(im1);
        imshow(im);
        cvWaitKey(20);
      }
    }
    strv_free(sv);
  }
  if (0) {
    int n = cap_num(CAP_DSHOW);
    cap_t cap[1] = {0};
    cap_open(cap, 0, CAP_DSHOW);
    for (i=0; 1; ++i) {
      cap_getframe(cap, im, T_GRAY);
      if (0) {
        char buf[256];
        _snprintf(buf, 256, "./cap/cc%05d.bmp", i);
        imwrite(buf, im);
      }
      if (1) {
        utime_start(_start_time);
        k = vena_detect(bf, im, im3_data);
        printf("%f\n", utime_elapsed(_start_time));
      }
      if (1) {
        imshow(im);
        if (k)  {
          imshow(im3);
        }
        //printf("k = %d\n", k);
        //imshow(BW2);
        //imshow(shuru1);
        //imshow(im1);
        //imshow(im);
        cvWaitKey(20);
      }
    }
  }
  if (1) {
    int n = cap_num(CAP_DSHOW);
    cap_t cap[1] = {0};
    uchar* featlib = NULL;
    enum {pic_per_man = 10};
    int nman = 0, npic = 0, ch=0;
    int isrecmode = 1;// 识别模式
    cap_open(cap, 0, CAP_DSHOW);
    MYREALLOC(featlib, 10*10*VENA_FEATLEN);
    for (i=0; 1; ++i) {
      int id=-1, dis=0;
      //cap_getframe(cap, im, T_GRAY);
      cap_getframe(cap, im, T_BGR);
      if (1) {
        utime_start(_start_time);
        k = vena_recog_bf(bf->data, bf->len, im->h, im->w, im->tt.data, im->s, im->c,
          pic_per_man*nman, isrecmode ? featlib : NULL, VENA_FEATLEN, im3_data, feat, &id, &dis);
        printf("%f\n", utime_elapsed(_start_time));
        if (!k)  {
          imdraw_fill(im3, _rgb(0, 0, 0));
        } else {
          if (isrecmode) {
            char buf[256];
            _snprintf(buf, 256, "id = %d dis = %d", id, dis);
            imdraw_text_v(im, 0, buf, 100, 100, NULL, dis>4000 ? _rgb(1, 0, 0) : _rgb(0, 1, 0));
          } else {
            if (npic<(nman+1)*pic_per_man) {
              mem_push_back(&featlib, &npic, feat, 1, VENA_FEATLEN);
            } else {
              isrecmode = 1; // 切换回识别模式
              ++nman;
            }
          }
        }
        {
          IRECT rc = iRECT(0, 0, im3->w, im3->h);
          imdraw_image_corner(im, &rc, rc, im3, 0, 0, 0);
          imdraw_rect(im, &rc, rc, 0, isrecmode ? _rgba(1,0,0,0.4) : _rgba(1,1,1,0.4), 10, BDM_ALL);
        }
      }
      if (1) {
        imshow(im);
        ch = cvWaitKey(20);
        if ('r'==ch || 'R'==ch) { // 切换到注册模式
          npic = nman*pic_per_man;
          isrecmode = 0;
        }
      }
    }
    FREE(featlib);
  }
  imfrees(im, 10);
  bffree(bf);
  return 0;
}
