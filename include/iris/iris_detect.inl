
#ifndef _IRIS_DETECT_INL_
#define _IRIS_DETECT_INL_
//#include "debug.h"
//

#include <math.h>
#include "./cstd.h"
#include "./iris.h"
#include "cmath.h"
//#include "img/imgio.inl"
#include "../img/imgopt.inl"
#include "../img/morph.inl"
#include "../img/inpaint2.inl"
#include "blurdetect.inl"
//#include "omkernel.inl"
#include "omkernel_ok.inl"
#include "omkernel.inl"
//#include "../face/face.inl"
#if 0
#include "../face/init.inl"
#include "../face/faceDetect.inl"

static unsigned int nir_iris20140725[] = {
  //#include "cas/nir_iris20140725.inl"
#include "../face/cas/nir_iris20140725.inl"
  //#include "D:\pub\bin\iris\adaboost/nir_iris20140725_f.inl"
};
#endif

#define FINDFIRSTNOZERO(_N, _F, _FI, _POS)  {int _i=0; for (;_i<_N;++_i, _POS+=_FI) {if (*(_F+_POS)) {break;}}}

#define value_yue_jie(x1, x2, y1, y2, pic_xsize, pic_ysize) \
  if (x1 < 1) {x1 = 1;}if (x2 < 2) {x2 = 2;}if (y1 < 1) {y1 = 1;}if (y2 < 2) {y2 = 2;}\
  if (x1 >= pic_xsize) {x1 = pic_xsize - 1;}if (x2 >= pic_xsize) {x2 = pic_xsize - 1;}\
  if (y1 >= pic_ysize) {y1 = pic_ysize - 1;}if (y2 >= pic_ysize) {y2 = pic_ysize - 1;}

#define imsize(im) cvSize(im->w, im->h)

#define POS
#define SORT_INSERT(_N, _A, _X, _OP) {int _I=_N; for (; _I-->0 && !_OP(_A[_I], _X); ) { _A[_I+1]=_A[_I]; } _A[_I+1]=_X; }

#include "myerode.inl"
#include "my_connect_domain.inl"
#include "my_fill.inl"
#include "my_areaopen.inl"
#include "rough_center_7_20.inl"
#include "pupil_to_polar_coordinate.inl"
//#include "Imfilter.inl"
//#include "canny.inl"
#include "canny_fix.inl"
//#include "nonmaxsup.inl"
#include "edge_detection.inl"
#include "Hooke_law_total.inl"
#include "horizontal_filter.inl"
#include "my_two_polyfit.inl"
#include "TopEyelidLocation.inl"
#include "iris_to_polar_coordinate.inl"
#include "edge_detection_waiyuan.inl"
#include "BottomEyelidLocation.inl"
#include "Daugmannormalized.inl"
#include "iris_detect_show.inl"
#include "glassRemoval.inl"
#include "pp.inl"
#include "wai_yuan_detection.inl"
#include "../img/color.inl"
#include "iris_std.inl"
#include "blurdetect.inl"

int copy_2d(int h, int w, const uchar* A, int al, int ai, uchar* B, int bl, int bi)
{
  int i, j;

  for (i = 0; i < h; ++i, A += al, B += bl) {
    for (j = 0; j < w; ++j) {
      B[j * bi] = A[j * ai];
    }
  }

  return 0;
}

#if 0
#define getfeat(a, b) bf_makeom_i1(bf, a, b, 0.8, 3)
#else
#define getfeat(a, b) makeom_bit_block_select(bf, a, b)
#endif

int iris_detect(irisrecog_t* ir)
{
  int h = ir->height, w = ir->width;
  img_t Iris0[1] = {0};
  img_t Iris[1] = {0};
  int i, nr, nx, ny;
  int nrr[3] = {0};
  int wrr[3] = {0};
  double urr[3] = {0};
  double drr[3] = {0};
  img_t ROI_rgb[1] = {0};
  img_t I1[1] = {0};
  img_t I2[1] = {0};
  img_t I3[1] = {0};
  double boost_detect_scale = 30. / MIN(ir->height, ir->width);
  int ss = 1;
  int stdsz = 200;
  uchar* aa = (uchar*)ir;
  buf_t bf[1] = {0};
  int mode = ir->mode;
  //GMALLOC_BEGIN(((uchar*)ir)+datalen, buflen);
#ifdef _OMKERNEL_OK_INL_
  ir->featlen = getfeat(0, 0);
  ir->distthd = 3309, ir->distmax = 6420;
  ir->distthd = 13332, ir->distmax = 24766;
#endif

  if (0 == mode || 0 == ir->buflen) {
    return 0;
  }

  bfinit(bf, ir->buf, ir->buflen);

  if (mode) {
#ifdef _COLOR_INL_
    int cn = colorcn(ir->color_type);
    IMINIT(Iris0, ir->height, ir->width, ir->data, ir->width*cn, cn, 1);
    //bf_imsetsize(Iris, ir->height, ir->width, 1, 1);
    bf_imcolorcvt(bf, Iris0, Iris, ir->color_type, T_GRAY);
#else
    IMINIT(Iris, ir->height, ir->width, aa + ir->img_off, ir->width, 1, 1);
#endif
    //imshow(Iris); cvWaitKey(-1);
    boost_detect_scale = MAX(boost_detect_scale, 0.1);

    for (i = 0; i < 3; ++i) {
      nrr[i] = wrr[i] = 0;
      urr[i] = drr[i] = 0;
      ir->in[i] = ir->iw[i] = ir->iu[i] = ir->id[i] = 0;
    }
  }

  if (IR_BOOST_DETECT & mode) {
    img_t I11[1] = {0};
    int cnt = 0;
#ifdef _FACEDETECT_INL_
    XRECT* xrc = 0;
    //utime_start(_start_time);
    BFMALLOC(bf, xrc, 100);
    cnt = face_detect(bf, nir_iris20140725, Iris->h, Iris->w, Iris->tt.data, Iris->s,
        0, 0, boost_detect_scale, 1.f, 15.f, 1.1f, 0, 0.5f, 4, xrc, 100, 0);
    ir->rc = xrc[0];
    //printf(" %.4f", utime_elapsed(_start_time));
    BFFREE(bf, xrc, 100);

    //ASSERT(buflen==bf->len);
    if (0 == cnt) {
      mode = 0;
    }

#else
    ir->rc.x = 210, ir->rc.y = 101, ir->rc.w = 280, ir->rc.h = 280;
#endif

    imref(Iris, ir->rc.x, ir->rc.y, ir->rc.w, ir->rc.h, I11);
    bf_imsetsize(bf, I1, stdsz, stdsz, 1, 1);
    imresize_bili(I11, I1);
    ss = (I11->h << 8) / stdsz;
  }
  else {
    bf_imsetsize(bf, I1, stdsz, stdsz, 1, 1);
    imresize_bili(Iris, I1);
    ss = (Iris->h << 8) / stdsz;
  }
  
  if (mode) {
    int blurrate = blurdetect(I1);
    //imshow(I1); cvWaitKey(-1);
    //yy=xx;
    ir->blurrate = blurrate;
  }
  if (IR_N_DETECT & mode) {
    //utime_start(_start_time);
    glassRemoval(bf, I1, I2);
#if 1
    //imshow(I2);cvWaitKey(-1);
    bf_imsetsize(bf, I3, I2->h / 2, I2->w / 2, 1, 1);
    imresize_bili(I2, I3);
    pp(bf, I3, nrr);
    nrr[0] *= 2, nrr[1] *= 2, nrr[2] *= 2;
    nx = nrr[0] >> IRIS_SHIFT1, ny = nrr[1] >> IRIS_SHIFT1, nr = nrr[2] >> IRIS_SHIFT1;
#endif
    //printf(" n%.4f", utime_elapsed(_start_time));
    // 得到上眼睑的曲线，直角坐标下，z_top每列对应一个值为当前的上眼睑坐标
  }

  if ((IR_NU_DETECT & mode) == IR_NU_DETECT) {
    int* z_top = 0;
    //utime_start(_start_time);
    TopEyelidLocation(bf, I3, nr, nx, ny, ss, urr);
    //printf(" t%.4f", utime_elapsed(_start_time));

    bf_imsetsize(bf, ROI_rgb, I2->h, I2->w, I2->c, 1);
    //printf(" %.4f", utime_elapsed(_start_time));}
    // 将上眼睑和上眼睑以上的部分检测出来，用白色255表示,可以在外圆检测时去除一大部分误检点
#if 0
    imclone(I2, ROI_rgb);
    //imshow(I2);
    ROI_row = ROI_rgb->h, ROI_col = ROI_rgb->w;
    //ir->u[0] /= 2, ir->u[2] *= 2;
    BFMALLOC(bf, z_top, ROI_col);
    get_y(ROI_col, urr, z_top);

    for (ii = 0; ii < ROI_col; ++ii) {
      int n = z_top[ii];
      uchar* pos = ROI_rgb->tt.data + ii;

      if (n <= 0 || n >= ROI_row) {
        continue;
      }

      FILL1D(n, pos, ROI_rgb->s, 255);
    }

    imshow(ROI_rgb);
    cvWaitKey(-1);
    BFFREE(bf, z_top, ROI_col);
#endif
  }

  if ((IR_NUW_DETECT & mode) == IR_NUW_DETECT) {
    //imshow(ROI_rgb); cvWaitKey(-1);
    //utime_start(_start_time);
    wai_yuan_detection(bf, nx / 2, ny / 2, nr / 2, I3, wrr);
    //V3MULX(wrr, wrr, 2);
    wrr[2] *= 2, wrr[0] *= 2, wrr[1] *= 2;
    //printf(" w%.4f", utime_elapsed(_start_time));
  }

  //imshow(ROI_rgb); cvWaitKey(-1);
  // 得到上眼睑的曲线，直角坐标下，z_top每列对应一个值为当前的上眼睑坐标
  // 如果下眼睑在外圆半径外，则不进行检测，因为对后面的iris提取不起作用
  if ((IR_NUWD_DETECT & mode) == IR_NUWD_DETECT) {
    //utime_start(_start_time);
    //imshow(I2);imshow(I3);cvWaitKey(-1);
    BottomEyelidLocation(bf, I2, nr, (int)wrr[2], nx, ny, ss, drr);
    //BottomEyelidLocation(bf, I3, nr/2, (int)wrr[2]/2, nx/2, ny/2, drr);
    //printf(" b%.4f", utime_elapsed(_start_time));
    //ir->d[0] *= 2, ir->d[2] /= 2;
  }

  for (i = 0; i < 3; ++i) {
    ir->in[i] = ((nrr[i] * ss) >> 8);
    //ir->in[i] = nrr[i];
    ir->iw[i] = ((wrr[i] * ss) >> 8);
    ir->iu[i] = F2FIX(urr[i]);
    ir->id[i] = F2FIX(drr[i]);
  }

  if (IR_STDIMG & mode) {
    img_t gry[1] = {0};

    //unsigned char* stdimg = aa + ir->std_off;
    //utime_start(_start_time);
    if (IR_BOOST_DETECT & mode) {
      imrefrc(Iris, ir->rc, gry);
    }
    else {
      *gry = *Iris;
    }

    ir->cnt_zero = iris_stdimg(bf, ir, gry, ir->stdiris);
    //printf(" s%.4f", utime_elapsed(_start_time));
  }

#ifdef _OMKERNEL_OK_INL_

  if (IR_FEATURE & mode) {
    uchar* feat = ir->feature;
    ir->featlen = getfeat(ir->stdiris, feat);
  }

#undef getfeat

  if (IR_RECOG & mode) {
    int featlen = ir->featlen;
    int i;
    uchar* fa = ir->feature;
    uchar* fb = ir->featlib;
    ir->maxdist_id = -1;
    ir->maxdist = 10000000;

#if 0
    memcpy(fa, fb, ir->featlen);
    memcpy(fb, fb+ir->featstep, ir->featlen);
#endif
    for (i = 0; i < ir->featnum; ++i) {
      int dis = count_xor(featlen, fa, fb + i * ir->featstep);

      if (dis < ir->maxdist) {
        ir->maxdist = dis;
        ir->maxdist_id = i;
      }
    }
    
    //ir->maxdist_id = count_xor(featlen, fa, fb + 5 * ir->featstep);
    //ir->maxdist_id = calc_sum(featlen, fb + 1 * ir->featstep);
    {
      int asdf=0;
    }
  }

#endif

  bf_imfree(bf, ROI_rgb);
  bf_imfree(bf, I3);
  bf_imfree(bf, I2);
  bf_imfree(bf, I1);
#ifdef _COLOR_INL_
  bf_imfree(bf, Iris);
#endif

  //GMALLOC_END();
  ASSERT(ir->buflen == bf->len);
  return 1;
}

#endif // _IRIS_DETECT_INL_
