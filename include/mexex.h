#ifndef _MEXEX_H_
#define _MEXEX_H_
#include <mex.h>
int mat2u8(int h, int w, const void* A, int al, int ai, uchar* B, int bl, int bi, int clss)
{
  switch (clss) {
#define CASTU8(x)   ((uchar)(x))
#define CLASSSIZEDEF(ID, TYPE, FMT)  case ID: {const TYPE* tA = (const TYPE*)A; SKIP_COPY_CAST(h, w, tA, al, ai, B, bl, bi, CASTU8); } break;
#include "mexclass.txt"
#undef CLASSSIZEDEF
#undef CASTU8
  };
  return 0;
}
int u82mat(int h, int w, const uchar* A, int al, int ai, void* B, int bl, int bi, int clss)
{
  switch (clss) {
#define CLASSSIZEDEF(ID, TYPE, FMT)  case ID: {TYPE* tB = (TYPE*)B; SKIP_COPY(h, w, A, al, ai, tB, bl, bi); } break;
#include "mexclass.txt"
#undef CLASSSIZEDEF
#undef CASTU8
  };
  return 0;
}
int mxArray2u8(const mxArray* arr, uchar* B, int bl, int bi)
{
  mwSize* dims = mxGetDimensions(arr);
  int h = dims[0];
  int w = dims[1];
  void* realdata = mxGetData(arr);
  int clss = mxGetClassID(arr);
  mat2u8(h, w, realdata, 1, h, B, bl, bi, clss);
  return 0;
}
int mxArray2im(const mxArray* arr, img_t* im)
{
  const mwSize* dims = mxGetDimensions(arr);
  int h = dims[0];
  int w = dims[1];
  imsetsize(im, h, w, 1, 1);
  mxArray2u8(arr, im->tt.data, im->s, im->c);
  return 0;
}
int mxSetu8(mxArray* arr, int h, int w, const uchar* A, int al, int ai)
{
  void* realdata = 0;
  mxSetNumericMatrix(arr, h, w, mxUINT8_CLASS, mxREAL);
  realdata = mxGetData(arr);
  mat2u8(h, w, A, al, ai, realdata, 1, h, mxUINT8_CLASS);
  return 0;
}
int mxSetim(mxArray* arr, const img_t* im)
{
  mxSetu8(arr, im->h, im->w, im->tt.data, im->s, im->c);
  return 0;
}
#endif // _MATEX_H_

