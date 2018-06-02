
#ifndef _LUOBO_INL_
#define _LUOBO_INL_

#include <stdlib.h>
//#define mxFree wrFree
#include "cstd.h"
#include "mex.h"
#include "str.h"
#include "luobo.h"

#ifdef _WINDOW_INL_
int mxShowArray(const char* name, const mxArray* arr) {
  char fmt[32]="%";
  int sz = 0, i;
  switch(arr->classID) {
#define CLASSSIZEDEF(ID, TYPE, FMT)  case ID: sz=sizeof(TYPE); strcat(fmt, FMT); break;
#include "mexclass.txt"
#undef CLASSSIZEDEF
  };
  if (arr->number_of_dims>2 && arr->dims[2]>1) {
    char buf[256];
	const char* p = (const char*)arr->realdata;
	int n = MIN(3, arr->dims[2]);
	for (i=0; i<n; ++i) {
	  _snprintf(buf, 256, "%s:%d", name, i);
      cvShowMat(buf, fmt, arr->dims[0], arr->dims[1], p+i*sz*arr->dims[0]*arr->dims[1], sz, arr->dims[0]*sz, -1);
	}
  } else {
    cvShowMat(name, fmt, arr->dims[0], arr->dims[1], arr->realdata, sz, arr->dims[0]*sz, -1);
  }
  return 0;
}
#endif // _WINDOW_INL_




#define MATLAB_MEX_FILE
#include "gradientMex.inl"
#include "rgbConvertMex.inl"
#include "imResampleMex.inl"

//template <typename TA, typename TB> inline
#define mat_transport(ah, aw, cn, A, al, am, an, B, bl, bm, bn, mulx, addx, isbgr, TYP) {\
  int i, j, k;\
  if (isbgr) {\
    for (i=0; i<ah; ++i) {\
      for (j=0; j<aw; ++j) {\
        for (k=0; k<cn; ++k) {\
          B[i*bl + j*bm + k*bn] = (TYP)(A[i*al + j*am + (cn-1-k)*an]*mulx + addx);\
        }\
      }\
    }\
  } else {\
    for (i=0; i<ah; ++i) {\
      for (j=0; j<aw; ++j) {\
        for (k=0; k<cn; ++k) {\
          B[i*bl + j*bm + k*bn] = (TYP)(A[i*al + j*am + k*an]*mulx + addx);\
        }\
      }\
    }\
  }\
}

int im2Array(const img_t* im, mxClassID clss, double mulx, double addx, int isbgr, mxArray* arr) {
  int dims[] = {im->h, im->w, im->c};
  mxSetNumericArray(arr, im->c>1?3:2, dims, clss, mxREAL);
  switch(clss) {
#define CLASSSIZEDEF(ID, TYPE, FMT)  case ID: mat_transport(im->h, im->w, im->c, im->tt.data, im->s, im->c, 1, ((TYPE*)arr->realdata), 1, im->h, im->h*im->w, mulx, addx, isbgr, TYPE); break;
#include "mexclass.txt"
#undef CLASSSIZEDEF
  };
  return 0;
}

int mxArray2im(const mxArray* arr, img_t* im) {
  int h = arr->dims[0];
  int w = arr->dims[1];
  int c = arr->dims[2];
  int sz = mxGetClassSize(arr->classID);
  imsetsize(im, h, w, c*sz, 1);
  switch(arr->classID) {
#define CLASSSIZEDEF(ID, TYPE, FMT)  case ID: mat_transport(h, w, c, ((TYPE*)arr->realdata), 1, h, h*w, ((TYPE*)im->tt.data), w*c, c, 1, 1, 0, 0, TYPE); break;
#include "mexclass.txt"
#undef CLASSSIZEDEF
  };
  return 0;
}

int mxDestroyArrays(int n, mxArray* pr) {
  int i;
  for (i=0; i<n; ++i) {
    mxDestroyArray(pr+i);
  }
  return 0;
}


int gradientMag(int nl, mxArray* pl, const mxArray* I, double channel DEFAULT(0), double full DEFAULT(0)) {
  int i=0;
  mxArray pr[5] = {0};
  pr[i++] = *I;
  mxSetDoubleScalar(pr+i++, channel);
  mxSetDoubleScalar(pr+i++, full);
  mGradMag(nl, pl, i, pr);
  mxDestroyArrays(2, pr+1);
  return 0;
}
int gradientHist(int nl, mxArray* pl, const mxArray* M, const mxArray* O,
 double binSize DEFAULT(8),double nOrients DEFAULT(9),double softBin DEFAULT(1), double useHog DEFAULT(1), double clip DEFAULT(.2)) {
  int i=0;
  mxArray pr[10] = {0};
  pr[i++] = *M;
  pr[i++] = *O;
  mxSetDoubleScalar(pr+i++, binSize);
  mxSetDoubleScalar(pr+i++, nOrients);
  mxSetDoubleScalar(pr+i++, softBin);
  mxSetDoubleScalar(pr+i++, useHog);
  mxSetDoubleScalar(pr+i++, clip);
  mGradHist(nl, pl, i, pr);
  mxDestroyArrays(5, pr+2);
  return 0;
}

int rgbConvert(int nl, mxArray* pl, const mxArray* I, const char* colorSpace, int useSingle DEFAULT(1) ) {
  const char* support_space = "|gray|rgb|luv|hsv|orig|";
  int flag = STRsplitfind(support_space, -1, colorSpace, -1, 1, 0);
  int i=0, d = flag ? 3 : 1;
  int outClass = useSingle ? mxSINGLE_CLASS : mxDOUBLE_CLASS;
  mxArray pr[10] = {0};
  if (flag<0) {
    printf("unknown colorSpace: %s", colorSpace);
    ASSERT(0);
    return 0;
  }
  //pl[0] = mxSetNumericMatrix3(I->dims[0], I->dims[1], d, (mxClassID)outClass, mxREAL);
  pr[i++] = *I;
  mxSetDoubleScalar(pr+i++, flag);
  mxSetDoubleScalar(pr+i++, useSingle);
  rgbConvertMex(nl, pl, i, pr);
  mxDestroyArrays(2, pr+1);
  return 0;
}

int imResample(int nl, mxArray* pl, const mxArray* I, int m, int n, int norm DEFAULT(1) ) {
  int i=0;
  mxArray pr[10] = {0};
  pr[i++] = *I;
  mxSetDoubleScalar(pr+i++, m);
  mxSetDoubleScalar(pr+i++, n);
  mxSetDoubleScalar(pr+i++, norm);
  imResampleMex(nl, pl, i, pr);
  mxDestroyArrays(3, pr+1);
  return 0;
}

int imgetfeature_10(const img_t* im, float* feat) {
  mxArray pl[10] = {0};
  mxArray I[1] = {0};
  int sz;
  int bin_size = 4;
  int nbin=6;
  int sample_factor = 4;
  int h, w;
  ASSERT(3==im->c);
  im2Array(im, mxSINGLE_CLASS, 1/255., 0, 1, I);

  gradientMag(2, pl, I, 0, 0);
  gradientHist(1, pl+2, pl, pl+1, 4, 6, 1, 1, .2);
  rgbConvert(1, pl+3, I, "luv", 1);
  h = im->h/sample_factor;
  w = im->w/sample_factor;
  imResample(1, pl+4, pl, h, w, 1);
  imResample(1, pl+5, pl+3, h, w, 1);
#if 0
  mxShowArray("M", pl+0);
  mxShowArray("O", pl+1);
  mxShowArray("H", pl+2);
  mxShowArray("luv", pl+3);
  mxShowArray("grad_m", pl+4);
  mxShowArray("luv20", pl+5); cvWaitKey(-1);
#endif
  sz=h*w;
  memcpy(feat, pl[5].realdata, sizeof(float)*sz*3);
  memcpy(feat+sz*3, pl[4].realdata, sizeof(float)*sz);
  memcpy(feat+sz*4, pl[2].realdata, sizeof(float)*sz*nbin);

  mxDestroyArrays(1, I);
  mxDestroyArrays(10, pl);
  return 0;
}

int getfeature_10(const uchar* img, float* feat) {
  if (img && feat) {
    img_t im[1] = {0};
    IMINIT(im, 80, 80, img, 80*3, 3, 1);
    //imshow(im);cvWaitKey(-1);
    return imgetfeature_10(im, feat);
  }
  return 4000;
}

#endif // _LUOBO_INL_
