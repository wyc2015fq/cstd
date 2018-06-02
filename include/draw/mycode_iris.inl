#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "cstd.h"
//#include "font/font.inl"
//#include "iris/iris.inl"
#include "iris/iris_recog.inl"
int mode = 0;
int curid = 0;
int cur_pic_count = 0;
int on_btn_reg(int x)
{
  mode = 1;
  cur_pic_count = 0;
  return 0;
}
typedef struct myfeat_t {
  char feat[5400];
  char userid[100];
} myfeat_t;
myfeat_t cutfeat;
#define MAXPIC 20
myfeat_t featlib[10 * MAXPIC];
uchar stdiris[100 * 100];
int memnum = 0;
double get_sys_time()
{
  struct timeval tv = {0};
  double total = 0;
  if (gettimeofday(&tv , 0) == -1) {
    //return 0;
  }
  total = (int64)tv.tv_sec * 1000000. + tv.tv_usec;
  return total;
}
int myframe(int h, int w, void* img, int step, int cn, int t, int out_h, int out_w, void* out_buf, int out_step, int out_cn)
{
  int iris_recog(int h, int w, const void * data, int step, int cn, const char * s_pixtype, const char * s_mode, int is_trans, double scale,
      int * xywh, void * stdiris, void * feature, const void * featlib, int featstep, int featnum, int * pid, int * pdis);
  //aaa(12);
  char buf[256] = {0};
  int xywh[12] = {0}; // ÈËÁ³ ×óÑÛ ÓÒÑÛ ¾ØÐÎ¿òÎ»ÖÃ [x, y, w, h]
  double get_sys_time();
  double t1, t2;
  double sc = 0.5;
  img_t im[1] = {0};
  int ret = 0;
  int featstep = sizeof(myfeat_t);
  int pid = -1;
  int pdis = -1;
  int featnum = memnum * MAXPIC;
  static const char* smode[] = {"DET", "REG", "REC"};
  IMINIT(im, out_h, out_w, (uchar*)out_buf, out_step, out_cn, 1);
  t1 = get_sys_time() * 1000;
  //iris(0, 0, 0, 0);
  ret = iris_recog(h, w, img, w, 1, "GRAY", smode[mode], t, sc, xywh, stdiris, &cutfeat, featlib, featstep, featnum, &pid, &pdis);
  t2 = get_sys_time() * 1000;
  if (1 == mode) {
    if (ret) {
      snprintf(cutfeat.userid, 64, "%d", curid);
      featlib[featnum + cur_pic_count] = cutfeat;
      ++cur_pic_count;
    }
    if (cur_pic_count >= MAXPIC) {
      cur_pic_count = 0;
      mode = 2;
      ++memnum;
      ++curid;
      if (memnum >= 10) {
        memnum = 0;
      }
    }
  }
  {
    int x, y;
    snprintf(buf, 256, "%d %d [%d %d %d %d](%d %s %d) %d %.3f", ret, cur_pic_count, xywh[0], xywh[1], xywh[2], xywh[3], pid, featlib[pid].userid, pdis, memnum, t2 - t1);
    imput_rect(im, x = xywh[0], y = xywh[1], xywh[2], xywh[3], 0, _rgb(0, 0, 1), 2);
    imput_str(im, 10, 50, buf, -1, 0xffffffff);
    imput_circle(im, x + (xywh[4] >> 8), y + (xywh[5] >> 8), xywh[6] >> 8, 0xffffffff);
    imput_circle(im, x + (xywh[8] >> 8), y + (xywh[9] >> 8), xywh[10] >> 8, 0xffffffff);
  }
  return 0;
}

