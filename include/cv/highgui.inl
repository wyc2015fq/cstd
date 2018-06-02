
#include "cstd.h"

#include "img/imgio.inl"

#define CVAPI(rettype) extern "C"  rettype __cdecl

typedef void img_t;
struct img_t;
uchar* cvGetImg(const img_t* srcarr, int* ph, int* pw, int* pstep, int* pcn);
void* cvSetImg(int h, int w, uchar* data, int step, int cn);

CVAPI(img_t*) cvLoadImage(const char* filename, int iscolor CC_DEFAULT(CC_LOAD_IMAGE_COLOR)) {
  img_t im[1] = {0};
  int cn = 3;
  img_t* img = NULL;
  switch(iscolor) {
  case CC_LOAD_IMAGE_GRAYSCALE: cn = 1;break;
  case CC_LOAD_IMAGE_COLOR: cn = 3;break;
  default: cn = 3;break;
  }
  if (imread(filename, cn, 1, im)) {
    //imshow(im);WaitKey(0);
    img = (img_t*)cvSetImg(im->h, im->w, im->tt.data, im->s, im->c);
  }
  imfree(im);
  return img;
}

CVAPI(int) cvSaveImage(const char* filename, const img_t* image) {
  int h=0, w=0, al=0, ai=0;
  uchar* data = cvGetImg(image, &h, &w, &al, &ai);
  return imsave(filename, h, w, data, al, ai);
}

#undef WaitKey
#undef cvShowImage
CVAPI(int) WaitKey(int delay = 0) {
  return cvWaitMsg(delay, false);
}

CVAPI(void) cvShowImage(const char* name, const img_t* image) {
  int h=0, w=0, al=0, ai=0;
  uchar* data = cvGetImg(image, &h, &w, &al, &ai);
  cvShowImagePal(name, h, w, data, al, ai, PixFmtMax, 0);
}


CVAPI(int) cvNamedWindow(const char* name, int flags  = 1) {
  return NamedWindow(name, flags);
}
CVAPI(void) cvDestroyWindow(const char* name) {
  0;//DestroyWindow(name);
}

//typedef void (__cdecl *CvTrackbarCallback)(int pos);

CVAPI(int) cvCreateTrackbar(const char* trackbar_name, const char* window_name,
                            int* value, int count, CvTrackbarCallback on_change) {
  return CreateTrackbar(trackbar_name, window_name, value, count, on_change);
}

#include "sys/sys_win32.inl"
