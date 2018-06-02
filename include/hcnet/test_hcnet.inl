
#include "cstd.h"
#if 0
#include "hcnet.inl"
#include "ui/window.inl"

#include <time.h>
#define USECOLOR 1

int iPicNum = 0; //Set channel NO.
//LONG nPort = -1;


//#include "fmtimg/fmtimg.inl"

int test_hcnet()
{
  //cvWaitKey(0);
  capdev cap[1] = {0};
  int ch = 0;
  img_t im[1] = {0};
  hcnet_open(cap, "192.168.1.46", 8000, "admin", "123456");

  for (; 1;) {
    imshow(im);
    {
      utime_start(_start_time);
      cap_getframe(cap, im, 0);
      printf("%f\n", utime_elapsed(_start_time));
    }
    ch = cvWaitKey(50);

    if ('q' == ch || 'Q' == ch) {
      break;
    }
  }

  cap_close(cap);
  imfree(im);
  return 0;
}

#include "ui/test_ui.inl"
#include "ui/test_face_recog.inl"

#define _WINDOWS

int test_face_recog() {
  //win_show("test_circles", test_circles_proc);
  win_show("»À¡≥ ”∆µº‡øÿ—› æv0.1", test_face_recog_proc);
  //win_show("test_uidemo", test_uidemo_proc);
  
  for (;;) {
    int ch = waitkey(-1);
    //printf("--%08x\n", &ch);
    if (ch==-2) {
      break;
    }
  }
  
  return 0;
}
#endif
