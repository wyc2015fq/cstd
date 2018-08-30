
#include "cstd.h"

//#include "ui/window.inl"

#include "face.inl"


#include "test_face_recog.inl"
#include "test_jieya_algo.inl"
//#include "test_ASM.inl"
#include "test_face_lena.inl"
//#include "test_face_recog.inl"
#include "test_facerecog.inl"
#include "test_facedetect.inl"
//#include "cv/copy.inl"
//#include "test_facerecog.inl" imtrans

#if 0
#include "img/imgopt.inl"
#include "test_lbf_face_alignment.inl"
#include "draw/imdraw.inl"
#include "font/font.inl"
#include "ui/uiwin.inl"
#endif

int test_face_detect_defaut1() {
  int xywh[12] = {0}, i;
  _chdir("E:/pub/bin/face/facedata");
  test_face_detect_defaut("adfasdf.bmp", 1, 1, xywh);
  printf("xywh=");
  for (i=0; i<12; ++i) {
    printf(" %d", xywh[i]);
  }
  return 0;
}

int test_pic_dat1() {
  int xywh[12] = {0};
  buf_t bf[1] = {0};
  img_t im[1] = {0};
  img_t im1[1] = {0};
  int h, w;
  _chdir("D:/");
  buf_load("aa1.dat", bf);
  h = 120, w = 320;
  IMINIT(im, h, w, bf->data+4, w*3, 3, 1);
  imflip_ud(im);
  imresize2(im, h*2, w, im1);
  //imswap_rb(im);
  //cvNamedWindow("im", 1);
  imshow(im1);
  cvWaitKey(-1);
  return 0;
}

int test_face() {
  //HMODULE xx = LoadLibrary("C:\\Platform.dll");
  //xx=xx;
  //test_huff1();
  //test_face_recog();
  //test_jieya_algo();
  //test_face_recog_rate_algo();
  //test_face_recog_pic();
  //test_facedetect();
  //test_facerecog();
  //test_lbf_face_alignment_lena();
  //test_face_lena();
  //test_lbf_face_alignment();
  return 0;
}
