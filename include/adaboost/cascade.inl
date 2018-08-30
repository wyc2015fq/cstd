
#ifndef _CASCADE_INL_
#define _CASCADE_INL_
#include "adaboost.h"
#include "img/imgopt.inl"

#include "seqpartition.h"

int cascade_pass_img(CASCADE* ca, const uchar* im1_data, int al, double* pscore) {
  double* inter = 0;
  const int cx = ca->w, cy = ca->h;
  const int w = ca->w, h = ca->h;
  const int INTEGRAL_CN = ca->featfun->INTEGRAL_CN;
  int stp = (w + 1);
  int siz = (h + 1) * stp * INTEGRAL_CN;
  inter = MALLOC(double, siz);
  ca->featfun->integral(h, w, im1_data, al, inter, stp, INTEGRAL_CN);
  double score, invvar, var=0;
  int ispass = 0;
  double* inter1 = inter+(h+1)*(w+1);
  double invarea = 1.f / (cx * cy);
  char* chptr;
  chptr = (char*)(ca->featptr);
  SKIP_INTEGRAL_VAR(inter, stp * cy, cx, inter1, stp * cy, cx, invarea, var);
  invvar = (var <= 0.f) ? 1.f : 1.f / sqrt(var);
  if (ca->featfun->setoffset) {
    int i;
    for (i = 0; i < ca->weaklen; ++i, chptr += ca->featfun->elemsize) {
      ca->featfun->setoffset(chptr, stp, 1, 1.f);
    }
  }
  cascade_pass(ca, inter, stp, invvar, &ispass, pscore);
  FREE(inter);
  return ispass;
}
// HAAR 检测
// [h, w, mag, al] 梯度积分图 通道数=10
int cascade_detect(buf_t* bf, CASCADE* ca, const img_t* im0, double ssmin, double ssmax, double ss, int stepxy, XRECT* B, int B_len)
{
  int i, len = 0;
  const int cx = ca->w, cy = ca->h;
  const int INTEGRAL_CN = ca->featfun->INTEGRAL_CN;
  int w = FLOOR(im0->w / ssmin), h = FLOOR(im0->h / ssmin);
  int stp = (w + 1);
  int siz = (h + 1) * stp * INTEGRAL_CN;
  char* chptr;
  img_t im[1] = {0};
  double* inter = 0;
  uchar* im1_data = 0;
  int im1sz = h * w * im0->c;
  assert(INTEGRAL_CN != 0);
  bf_imresize(bf, im0, h, w, im);
  BFMALLOC(bf, im1_data, im1sz);
  BFMALLOC(bf, inter, siz);
  
  for (; len < B_len && h >= cy && w >= cx; ssmin *= ss, w = FLOOR(im0->w / ssmin), h = FLOOR(im0->h / ssmin)) {
    int ranx = w - cx + 1, rany = h - cy + 1;
    int x, y;
    int stepx = stepxy, stepy = stepxy;
    double invarea = 1.f / (cx * cy);
    double* inter1 = inter+(h+1)*(w+1);
    stp = (w + 1);
    
    bf_imresize_impl(im->h, im->w, im->tt.data, im->s, im->c, h, w, im1_data, w * im->c, im->c, CC_INTER_AREA);
    //ShowImage("", h, w, im1_data, w * im->c, im->c);waitkey(-1);
    
    ca->featfun->integral(h, w, im1_data, w * im->c, inter, stp, INTEGRAL_CN);
    chptr = (char*)(ca->featptr);
    
    if (ca->featfun->setoffset) {
      for (i = 0; i < ca->weaklen; ++i, chptr += ca->featfun->elemsize) {
        ca->featfun->setoffset(chptr, stp, 1, 1.f);
      }
    }
    
    for (y = 0; len < B_len && y < rany; y += stepy) {
      for (x = 0; len < B_len && x < ranx; x += stepx) {
        double* tmpSamp = inter + y * stp + x;
        double* tmpSamp1 = inter1 + y * stp + x;
        double score, invvar, var=0;
        int ispass = 0;
        SKIP_INTEGRAL_VAR(tmpSamp, stp * cy, cx, tmpSamp1, stp * cy, cx, invarea, var);
        //cvShowMat("asdf1", "%I64f", (cy+1)*2, (cx+1), tmpSamp, sizeof(*tmpSamp)*stp, sizeof(*tmpSamp), -1);//cvWaitKey(-1);
        invvar = (var <= 0.f) ? 1.f : 1.f / sqrt(var);
        cascade_pass(ca, tmpSamp, stp, invvar, &ispass, &score);
        
        if (ispass) {
          B[ len ].x = FLOOR(x * ssmin);
          B[ len ].y = FLOOR(y * ssmin);
          B[ len ].w = FLOOR(cx * ssmin);
          B[ len ].h = FLOOR(cy * ssmin);
          B[ len ].count = 1;
          B[ len ].score = (int)score;
          ++len;
        }
      }
    }
  }
  
  BFFREE(bf, inter, siz);
  BFFREE(bf, im1_data, im1sz);
  bf_imfree(bf, im);
  
  if (1 && len > 0) {
    char* pabuf = 0;
    int pabuflen = SEQPARTITIONSIZE(sizeof(XRECT), len), plen;
    BFMALLOC(bf, pabuf, pabuflen);
    len = rect_partition(B, B + len, 0.5, 1, 1, pabuf, &plen);
    BFFREE(bf, pabuf, pabuflen);
  }
  
  return len;
}

#endif // _CASCADE_INL_
