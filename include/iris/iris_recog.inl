
#ifndef _FACE_RECOG_INL_
#define _FACE_RECOG_INL_

#include "iris_detect.inl"
#include "../cstd.h"
#include "./iris.h"
#include "../easydraw/easydraw.inl"
//#include "img/imgio.inl"

//#include "lib/Sentinel/libsentinel.inl"
#define sentinel_login() 1

#define FR_FACE_DET (FR_FACE_DETECT|FR_EYES_DETECT)
#define FR_FACE_REG (FR_FACE_DETECT|FR_EYES_DETECT|FR_FACE_STD|FR_FACE_FEATURE|FR_RECOG_TEST)
#define FR_FACE_REC (FR_FACE_DETECT|FR_EYES_DETECT|FR_FACE_STD|FR_FACE_FEATURE|FR_FACE_RECOG)

int irisrecog_set(irisrecog_t* ir)
{
  memset(ir, 0, sizeof(irisrecog_t));
  //iris_detect(ir); // 获取featlen一般不会超过 10000 ,现在用的特征==1350
  return 0;
}

static char iris_membuf[10*1024*1024];

int irisrecog_run(irisrecog_t* s, const img_t* im, int pixtype, int mode)
{
  //int buflen = countof(iris_membuf);
  s->height = im->h, s->width = im->s;
  s->data = im->tt.data;
  s->color_type = pixtype;
  s->mode = mode;
  s->buf = (uchar*)iris_membuf;
  s->buflen = countof(iris_membuf);
  iris_detect(s); // 获取信息
  return 0;
}
 
int iris_recog(int h, int w, const void* data, int step, int cn, const char* s_pixtype, const char* s_mode, int is_debug, double scale,
               int* xywh, void* stdiris, void* feature, const void* featlib, int featstep, int featnum, int* pid, int* pdis) {
  img_t im[1] = {0};
  irisrecog_t s[1] = {0};
  int pixtype, mode;
  static const char* pixtypes ="|GRAY|BGR|RGB|BGRA|BGR555|BGR565|YUYV|UYVY|";
  static const int pixtypearr[] = {T_GRAY, T_BGR, T_RGB, T_BGRA, T_BGR555, T_BGR565, T_YUYV, T_UYVY};
  static const char* modes ="|DET|REG|REC|";
  static const int modearr[] = {FR_IRIS_DET, FR_IRIS_REG, FR_IRIS_REC};
  int have_iris = 0;

  IMINIT(im, h, w, data, step, cn, 1);
  pixtype = cstr_splitfind(pixtypes, -1, s_pixtype, -1, 1, 0);
  mode = cstr_splitfind(modes, -1, s_mode, -1, 1, 0);
  irisrecog_set(s);
  if (data && pixtype>=0 && mode>=0 && mode<countof(modearr)) {
    int mode2 = modearr[mode];
    s->stdiris = (unsigned char*)stdiris;
    s->feature = (uchar*)feature;
    s->featlib = (uchar*)featlib;
    s->featstep = featstep;
    s->featnum = featnum;
    irisrecog_run(s, im, pixtypearr[pixtype], mode2);
    have_iris = (s->rc.w>0);
    if (xywh) {
      XRECT_int(&s->rc, xywh);
      memcpy(xywh + 4, s->in, 3);
      memcpy(xywh + 8, s->iw, 3);
    }
#ifdef _EASYDRAW_INL_
    if (is_debug) {
      int x = s->rc.x, y = s->rc.y;
      imput_circle(im, x+(s->in[0]>>8), y+(s->in[1]>>8), s->in[2]>>8, 0xffffffff);
      imput_circle(im, x+(s->iw[0]>>8), y+(s->iw[1]>>8), s->iw[2]>>8, 0xffffffff);
    }
#endif // _EASYDRAW_INL_
    if (pid) {
      *pid = s->maxdist_id;
    }
    if (pdis) {
      *pdis = s->maxdist*1/50;
    }
  }
  return have_iris;
}

//  int xywh[12] = {0};
//  h 图像高 w 图像宽 data图像数据 step图像每行字节数 cn图像通道数
// s_pixtype GRAY 灰度图 BGR|RGB|BGRA|BGR555|BGR565 彩色图 YUYV|UYVY 其他格式
// is_trans 是否翻转90度
// scale 图像预缩放
// xywh 输出 人脸[x, y, w, h] 左眼[x, y, w, h] 右眼[x, y, w, h]
//  iris_detect_defaut(im->h, im->w, im->tt.data, im->s, im->c, "gray", trans, 1, xywh);
//  printf("%d %d\n", xywh[0], xywh[1]);
int iris_detect_defaut(int h, int w, const void* data, int step, int cn, const char* s_pixtype, int is_trans, double scale, int* xywh) {
  return iris_recog(h, w, data, step, cn, s_pixtype, "DET", is_trans, scale, xywh, 0, 0, 0, 0, 0, 0, 0);
}


#endif // _FACE_RECOG_INL_

