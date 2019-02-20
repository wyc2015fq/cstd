

#include "opencvex\opencvex.hpp"

int test_imArticulation() {
  const char* path_a = "E:/OCR_Line/bin/pic/20190213142643.jpg";
  Mat im_a = imread(path_a, cv::IMREAD_COLOR);
  double tt_a = imArticulation2(im_a);
  const char* path_b = "E:/OCR_Line/bin/pic/20190213152904.jpg";
  Mat im_b = imread(path_b, cv::IMREAD_COLOR);
  double tt_b = imArticulation2(im_b);
  const char* path_c = "E:/OCR_Line/bin/pic/20190213152923.jpg";
  Mat im_c = imread(path_c, cv::IMREAD_COLOR);
  double tt_c = imArticulation2(im_c);
  return 0;
}