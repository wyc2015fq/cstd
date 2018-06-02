#include "cstd.h"
#include "inifile.h"
#include "fileio.h"
//#include "ui/window.inl"
#include "cvui/wincv.inl"
int test_wincv_onbtncap1(struct _Widget* wid, int xx)
{
  return 0;
}
int test_wincv_onbtnstop1(struct _Widget* wid, int xx)
{
  return 0;
}
int test_wincv_onbtnzero1(struct _Widget* wid, int xx)
{
  return 0;
}
int test_Kinect_on_mouse(int event, int x, int y, int flags, void* param)
{
}
int test_cvui()
{
  char buf[256];
  const char* winname = "test_cvui";
  char key = 0;
  int i, j, h, w, h2, w2;
  static int n = 0;
  int fram = 0;
  cvNamedWindow(winname, 1);
  cvSetWidgetsXml(winname, "test_cvui01.xml");
  cvSetOnClick(winname, "btncap", test_wincv_onbtncap1);
  cvSetOnClick(winname, "btnstop", test_wincv_onbtnstop1);
  cvSetOnClick(winname, "zero", test_wincv_onbtnzero1);
  cvSetMouseCallback(winname, test_Kinect_on_mouse, 0);
  for (; key != 27; ++fram) {
    //cvSetImage( winname, buf, h, w, cvBGRImg, w*3, 3, 1 );
    _snprintf(buf, 256, "cap %d frames", n);
    cvSetWindowText(winname, "s_time", buf);
    key = cvWaitKey(50);
    if (-2 == key) {
      break;
    }
  }
  return 0;
}

