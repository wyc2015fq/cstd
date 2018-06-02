
#include "chalg.h"
#include "../color.inl"
#include "../imopt.inl"

#include "EroadVideo.inl"

#include "../window.inl"
#define  WINNAME  "asdfasdf"
void trackbar_set_brightness(int pos, void* user) {
  eroadcap* s = (eroadcap*)user;
  eroadcap_set_brightness(s, pos);
}
void trackbar_set_contrast(int pos, void* user) {
  eroadcap* s = (eroadcap*)user;
  eroadcap_set_contrast(s, pos);
}
void trackbar_set_sensitometry(int pos, void* user) {
  eroadcap* s = (eroadcap*)user;
  eroadcap_set_sensitometry(s, pos);
}
int MyCBFunc(void* user, img_t* im, int pix_fmt) {
  img_t im2[1]={0};
  img_t im3[1]={0};
  img_t im4[1]={0};
  //上位视频对比度
  int upcontrast = 255;
  //设置上位对比度阀值
  int ThresholdValue = 255;
  
  //colorcvt(VideoHeight, VideoWidth, pData_RAW_YUV, VideoWidth, 1, pDataRGB, VideoWidth * 3, 3, T_BayerGR, T_BGR);

  imcolorcvt(im, im2, pix_fmt, T_BGR);
  imcolorcvt(im2, im3, T_BGR, T_BGRA);
  imcolorcvt(im3, im4, T_BGRA, T_BGR);
  RGB_ContrastAdjust(im2->data, im2->data, upcontrast, ThresholdValue, im2->width*im2->height);
  imshow2(WINNAME, im4);
  imfree(im2);
  imfree(im3);
  imfree(im4);
  return 0;
}

int test_EroadVideo() {
  eroadcap s[1] = {0};
  cvNamedWindow(WINNAME, 1);
  cvCreateTrackbar2("亮度", WINNAME, 0, 256, trackbar_set_brightness, s);
  cvCreateTrackbar2("对比度", WINNAME, 0, 256, trackbar_set_contrast, s);
  cvCreateTrackbar2("曝光度", WINNAME, 0, 256, trackbar_set_sensitometry, s);
  eroadcap_start(s, MyCBFunc, 0);
  cvWaitKey(-1);
  eroadcap_exit(s);
  return 0;
}

int main() {
  test_EroadVideo();
  return 0;
}
