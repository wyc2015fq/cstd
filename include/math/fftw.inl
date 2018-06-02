
#include "fftw/fftw3.h"
#pragma comment(lib, "libfftw3-3.lib")
#pragma comment(lib, "libfftw3f-3.lib")
#pragma comment(lib, "libfftw3l-3.lib")

static void fftw_dft(const img_t* src, img_t* dst, int flag, int outcn, bool is64f) {
  int i, j, k, w = src->w, h = src->h, step = src->w, cn = CC_MAT_CN(src);
  ASSERT(cn==1 || cn==2);
  IM_BEGIN(im, 10);
  img_t* src1 = cvSetMat(im++, h, w, CC_64F, cn, NULL, CC_AUTO_STEP);
  img_t* dst1 = cvSetMat(im++, h, w, CC_64F, outcn, NULL, CC_AUTO_STEP);
  int inv = flag&CC_DXT_INVERSE;
  cvConvertScale(src, src1, 1, 0);
  fftw_complex *data_in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * w * h);
  fftw_complex *data_ou = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * w * h);
  fftw_plan plan_f = fftw_plan_dft_2d(h, w, data_in, data_ou, inv ? FFTW_BACKWARD : FFTW_FORWARD, FFTW_ESTIMATE);
  /* load img_src's data to fftw input */
  if (1==cn) {
    for (i = 0, k = 0; i < h; ++i) {
      double* row1 = img_row(double, src1, i);
      for (j = 0; j < w; ++j) {
        // method 1: 输入数据乘以(-1)^（i+j），即可中心化
        data_in[k][0] = row1[j];
        data_in[k][1] = 0.0;
        k++;
      }
    }
  } else {
    for (i = 0, k = 0; i < h; ++i) {
      double* row1 = img_row(double, src1, i);
      for (j = 0; j < w; ++j) {
        // method 1: 输入数据乘以(-1)^（i+j），即可中心化
        data_in[k][0] = row1[2*j+0];
        data_in[k][1] = row1[2*j+1];
        k++;
      }
    }
  }
  
  /* perform FFT */
  fftw_execute(plan_f);
  double scale = 1;
  if (flag & CC_DXT_SCALE) {
    scale = 1./(src->cols * src->rows);
  }
  if (1==outcn) {
    //ASSERT(outcn==1);
    for (i = 0, k = 0; i < h; ++i) {
      double* row1 = img_row(double, dst1, i);
      for (j = 0; j < w; ++j) {
        // method 1: 输入数据乘以(-1)^（i+j），即可中心化
        row1[j] = scale * sqrt(data_ou[k][0] * data_ou[k][0] + data_ou[k][1] * data_ou[k][1]);
        k++;
      }
    }
  } else {
    for (i = 0, k = 0; i < h; ++i) {
      double* row1 = img_row(double, dst1, i);
      for (j = 0; j < w; ++j) {
        // method 1: 输入数据乘以(-1)^（i+j），即可中心化
        row1[2*j+0] = scale * data_ou[k][0];
        row1[2*j+1] = scale * data_ou[k][1];
        k++;
      }
    }
  }
  cvSetMat(dst, h, w, src->tid, outcn, NULL, CC_AUTO_STEP);
  cvConvertScale(dst1, dst, 1, 0);
  fftw_destroy_plan(plan_f);
  fftw_free(data_in);
  fftw_free(data_ou);
  IM_END(im);
  return ;
}
