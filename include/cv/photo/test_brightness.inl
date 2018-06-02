
#include "brightness.inl"
#include "balanceWhite.inl"
#include "Vignetting.inl"
#include "lic.inl"
#include "img/imgio.inl"

int test_brightness() {
  img_t im[10] = {0};
  int i=0;
  img_t* src = im+i++;
  img_t* dst = im+i++;
  int brightnessval = 50;
  sys_chdir("F:/pub/bin/photo/pics");
  imread("22c28182d36aeabc83302986d35f281c.jpg", 3, 1, src);
  imshow2("src", src);  
  // 调色
  //ColorTransfer(src, tar, dst);
  imshow2("src", src);
  cvNamedWindow("dst", 1);
  cvCreateTrackbar("brightness", "dst", &brightnessval, 100, NULL);
  for(;1;) {
    imclone2(src, dst);
    brightness(src->h, src->w, src->tt.data, src->s, dst->tt.data, dst->s, dst->c, brightnessval);
    imshow2("dst", dst);
    cvWaitKey(20);
  }
  imfrees(im, 10);
  return 0;  
}  



int test_balanceWhite() {
  img_t im[10] = {0};
  int i=0;
  img_t* src = im+i++;
  img_t* dst = im+i++;
  int strength = 50;
  sys_chdir("F:/pub/bin/photo/pics");
  imread("dog.png", 3, 1, src);
  imread("20170403143853277.jpg", 3, 1, src);
  imshow2("src", src);  
  // 调色
  //ColorTransfer(src, tar, dst);
  imshow2("src", src);
  cvNamedWindow("dst", 1);
  cvCreateTrackbar("brightness", "dst", &strength, 100, NULL);
  for(;1;) {
    imclone2(src, dst);
    //balanceWhite(src->h, src->w, src->tt.data, src->s, dst->tt.data, dst->s, dst->c, 0, 255, 0, 255);
    Vignetting(src->h, src->w, src->tt.data, src->s, dst->tt.data, dst->s, dst->c, (strength-50)/50.);
    imshow2("dst", dst);
    cvWaitKey(20);
  }
  imfrees(im, 10);
  return 0;  
}  


