
#ifndef _CAP_H_
#define _CAP_H_

#if 0

int test_cap() {
  capdev cap[1] = {0};
  if (capdshow_open(cap, 0, 640, 480) > 0) {
    int fram = 0;
    char ch = 0;
    img_t im[1] = {0};
    for (; 'q' != ch && 'Q' != ch; ++fram) {
      cap_getframe(cap, im, CAP_RGB);
      //printf("%d w=%d h=%d\n", fram, im->w, im->h);
      imshow_(im);
      ch = WaitKey(10);
    }
    cap_close(cap);
    imfree(im);
  }
  return 0;
}

#endif

int caperoad_open(cap_t* s, int idx, int width, int height);
int capvfw_open(cap_t* s, int idx, int width, int height);
int capopenni_open(cap_t* s, int idx, int width, int height);
int capdshow_open(cap_t* s, int idx, int width, int height);
int capv4l_open(cap_t* s, int idx, int width, int height);
int capvfwavi_open(capdev* s0, const char* filename);

#if 0
int caperoad_capnum();
int caperoad_open(capdev* s, int idx);
int caperoad_close(capdev* s);
int caperoad_getframe(capdev* s, img_t* im);
int caperoad_set_sensitometry(capdev* s0, int Saturation);
int test_CCameraDS();
int dshow_capnum();
int dshow_open(capdev* s, int indx);
int dshow_close(capdev* s);
int dshow_getframe(capdev* s, img_t* im);
int openni_capnum();
int openni_open(capdev* s, int indx);
int openni_close(capdev* s);
int openni_getframe(capdev* s, img_t* im);
int openni_getframe_depth(capdev* s, img_t* im);
int test_capopenni();
#endif



#endif // _CAP_H_


