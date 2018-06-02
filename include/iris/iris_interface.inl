
#if defined _WIN32
#include "iris_interface_arm.inl"
#else
#include "iris_interface_dsp.inl"
#endif
#include "../cstd.h"
#include "./iris.h"
//#include "../draw/imdraw.inl"
//#include "draw/imdraw_text_raster.inl"
//#include "draw/imdraw_text_vector.inl"

//#include "imdraw_text_raster.inl"
//#include "../img/imgopt.inl"

int DrawIris(img_t* im, const irisrecog_t* ir, unsigned long color0, char* s, int isflip, int wline)
{
  int i, x1 = ir->rc.x, y1 = ir->rc.y;
  int nrr[3] = {0};
  int wrr[3] = {0};
  double urr[3] = {0};
  double drr[3] = {0};
  double shift1 = 1 << IRIS_SHIFT1;
  double shift2 = 1 << IRIS_SHIFT2;

  for (i = 0; i < 3; ++i) {
    nrr[i] = (int)((ir->in[i]) / shift1);
    wrr[i] = (int)((ir->iw[i]) / shift1);
    urr[i] = ((ir->iu[i]) / shift2);
    drr[i] = ((ir->id[i]) / shift2);
  }

  if (isflip) {
    img_t im2[1] = {0};
    uchar* data = im->tt.data + (im->h-1)*im->s;
    IMINIT(im2, im->h, im->w, data, -im->s, im->c, 1);
    im = im2;
  }
#ifdef _IMDRAW_INL_
  //DrawRectangle2(im, ir->rc, color0, wline);
  imdraw_ploy(im, 0, 3, urr, x1, y1, color0, wline);
  imdraw_ploy(im, 0, 3, drr, x1, y1, color0, wline);
  imdraw_circle1(im, 0, x1 + wrr[0], y1 + wrr[1], wrr[2], color0, wline);
  imdraw_circle1(im, 0, x1 + nrr[0], y1 + nrr[1], nrr[2], color0, wline);
  if (s) {
    CvFont font1[1]={0};
    cvInitFont(font1, CV_FONT_HERSHEY_SIMPLEX, 10, 10, 0, 3);
    //imdraw_text(im, iRECT3(ir->rc, ir->rc), s, strlen(s), zimo_songti_12, CV_RGB(255, 255, 255), 0, TT_LEFT);
    //imdraw_text_v(im, 0, s, ir->rc.x, ir->rc.y, font1, CV_RGB(255, 255, 255));
  }
#endif
  return 0;
}


int iris_draw(int h, int w, unsigned char* img, int step, int cn, const irisrecog_t* ir, unsigned long color0, char* s, int isflip, int wline) {
  img_t im[1]={0};
  IMINIT(im, h, w, im, step, cn, 1);
  DrawIris(im, ir, color0, s, isflip, wline);
  return 0;
}
