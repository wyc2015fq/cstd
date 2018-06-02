
#include "wbGW.h"
#include "wbPR.h"
int cvWBGW(IplImage* src, IplImage* dst) {
  int rows=src->height, cols=src->width;
  wbGW( rows, cols, (uchar*)src->imageData, src->widthStep, (uchar*)dst->imageData, dst->widthStep);
  //wbPR( rows, cols, (uchar*)src->imageData, src->widthStep, (uchar*)dst->imageData, dst->widthStep);
  return 0;
}

#include "hist.inl"
int cvHist(IplImage* src, IplImage* dst) {
  int rows=src->height, cols=src->width;
  skip_hist( rows, cols, (uchar*)src->imageData, src->widthStep, (uchar*)dst->imageData, dst->widthStep);
  return 0;
}
// 锐化
int cvSharp(IplImage* src, IplImage* dst) {
#if 0
  double aaa[] = {
    -1, -1, -1,
      -1, 9, -1,
      -1, -1, -1,
  };
  CvMat kernel = cvMat(3,3,CC_64F,aaa);
#else
  double aaa[] = {
      -1, -4, -7, -4, -1,
      -4,-16,-26,-16, -4,
      -7,-26,505,-26, -7,
      -4,-16,-26,-16, -4,
      -1, -4, -7, -4, -1,
  };
  int i;
  for (i=0; i<25; ++i) aaa[i]/=273;
  CvMat kernel = cvMat(5,5,CC_64F,aaa);
#endif
  // 分配像素置
  cvFilter2D(src, dst, &kernel);
  return 0;
}

#include "usmsharping.h"
int test_wb()
{
  //char* fname = "1.jpg";
  //char* fname = "aaa.bmp";
  //usmsharping();
  char* fname = "E:\\code\\cstd\\src\\TestVideo\\aaa.bmp";
  IplImage* src=cvLoadImage(fname,CC_LOAD_IMAGE_COLOR);
  IplImage* dst=cvCreateImage(cvGetSize(src),IMG_DEPTH_8U, 3);

  cvWBGW(src, dst);
  cvHist(dst, dst);
  cvSmooth( dst, dst, CC_GAUSSIAN, 5, 5, 0, 0 );
  cvSmooth( dst, dst, CC_MEDIAN, 5, 5, 0, 0 );
  cvSharp(dst, dst);

  cvNamedWindow("原始图像", 1);
  cvShowImage("原始图像",src);

  cvNamedWindow("wbGW", 1);
  cvShowImage("wbGW",dst);
  
  cvReleaseImage(&dst);
  cvReleaseImage(&src);
  cvWaitKey(0);
  return 0;
}
