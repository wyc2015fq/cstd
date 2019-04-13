#ifndef _TEST_CAM_INL_
#define _TEST_CAM_INL_

#include "cap/cap.inl"
#include "img/imgio.inl"
#include "utime.h"
#include "std/gui_c.h"
//#include "cap/capdshow.inl"

#ifdef _WIN32
#define cap_open capdshow_open
#else
#define cap_open capv4l_open
#endif

#if 0
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp> 
int test_cam() {
	VideoCapture capture;
  if (capture.open(0)) {
    int frame = 0;
    Mat cap;
    for (; frame<10; ++frame) {
		  capture.read(cap);
      printf(" %d %d\n", cap.cols, cap.rows);
    }
  }
  return 0;
}
#endif

int test_cam() {
  capdev cap[1] = {0};
  int ret;
  //ret = cap_open(cap, 0, 720, 576);
  //ret = cap_open(cap, 0, 640, 480);
  ret = cap_open(cap, 0, 1280, 960);
  
  if (ret > 0) {
    int frame = 0;
    char ch = 0;
    img_t im[1] = {0};
    
    {utime_start(_start_time);
    //imsetsize(im, 100, 100, 3, 1);
    //imwrite("/storage/emulated/pic/pic_aa.jpg", im);
    for (; 'q' != ch && 'Q' != ch && frame<100000; ++frame) {
      cap_getframe(cap, im, CAP_RGB);
      if (1) {
        imshow_(im);
        ch = waitkey(10);
        char buf[256];
        _snprintf(buf, 256, "/storage/emulated/pic/pic_%d.jpg", frame);
        //imwrite(buf, im);
        //_snprintf(buf, 256, "/storage/emulated/pic/pic_%d.dat", frame);
        //savefile(buf, im->tt.data, im->h*im->s);
      }
      printf("pic %d w=%d h=%d\n", frame, im->w, im->h);
    }
    printf("%f\n", utime_elapsed(_start_time));}
    cap_close(cap);
    imfree(im);
  }
  return 0;
}
#ifndef _WIN32
#include "sys/sys_linux.inl"
#endif // _WIN32

#endif // _TEST_CAM_INL_
