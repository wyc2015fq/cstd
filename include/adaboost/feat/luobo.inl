
#include <stdlib.h>
//#define mxFree wrFree
#include "cstd.h"
#include "mem.h"
#include "mex.h"
#define MATLAB_MEX_FILE
#include "gradientMex.cpp"
#include "rgbConvertMex.cpp"

template <typename T>
mxArray* im2ArrayT(const img_t* im, mxClassID clss, T) {
  int dims[] = {im->h, im->w, im->c};
  mxArray* arr = mxCreateNumericArray(3, dims, clss, mxREAL);
  int i, j, k;
  T* p = (T*)arr->realdata;
  for (i=0; i<im->h; ++i) {
    for (j=0; j<im->w; ++j) {
      for (k=0; k<im->c; ++k) {
        p[k*im->h*im->w + j*im->h + i] = im->tt.data[i*im->s + j*im->c + k];
      }
    }
  }
  return 0;
}

mxArray* im2Array(const img_t* im, mxClassID clss) {
  switch(clss) {
#define CLASSSIZEDEF(a, b)  case a: return im2ArrayT(im, a, b());
#include "mexclass.txt"
#undef CLASSSIZEDEF
  };
  return 0;
}

int gradientMag(const img_t* I, double* args, img_t* M, img_t* O) {
  int i, channel = 0;
  double normRad = 0, normConst = .005, full = 0;
  mxArray* argin[10] = {0};
  if (NULL==O) {
  }
  argin[0] = mxCreateString("gradientMag");
  argin[0] = im2Array(I, mxDOUBLE_CLASS);
  if (0==normRad) {
  } else {
    //mGradMag
    //gradientMex();
    //S = convTri( M, normRad );
    //gradientMex('gradientMagNorm',M,S,normConst); % operates on M
  }
  for (i=0; i<10; ++i) {
    mxDestroyArray(argin[i]);
  }
  return 0;
}

int getfeature_10(const img_t* img, int bin_size, int nbin, int sample_factor) {
  //gradientMag();
  return 0;
}
