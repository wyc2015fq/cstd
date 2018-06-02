
#include <stdio.h>
#include <cv.h>
#include <cxcore.h>
#include <cxcore.h>
#include <highgui.h>
#pragma comment(lib, "Comctl32.lib")
#include "utime.h"

int ddd=0;
IplImage*  cvCreateImage1( CvSize size, int depth, int channels ) {
  ++ddd;
  return cvCreateImage( size, depth, channels );
}
IplImage* cvLoadImage1( const char* filename, int iscolor) {
  ++ddd;
  return cvLoadImage( filename, iscolor );
}
#define cvCreateImage   cvCreateImage1
#define cvReleaseImage(image)  (--ddd, cvReleaseImage(image))
//#include "..\\OpenCV1\\OpenCV1.inl"

#include "test_fft.inl"
#include "test_monofilt.inl"
