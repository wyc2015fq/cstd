
#include "wstd/filesystem.hpp"
//下面就给出应用HoughLinesP函数检测直线段的应用程序：
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencvex/opencvex.hpp"
 
#include <iostream>
using namespace cv;
using namespace std;


int calc_mod(int i, int n) {
  return i = ((i%n) + n) % n;
}

int setarr(float* arr, int n, int i, double t) {
  int k = 1;
  i = calc_mod(i, n);
  arr[i] += t;
  for (; t > 1;++k) {
    t *= 0.3;
    arr[calc_mod(i - k, n)] += t;
    arr[calc_mod(i + k, n)] += t;
  }
  return 0;
}

int debug = 0;

int getImageAngle(const Mat& src) {
  Mat edge;
  vector<Vec4i> lines;
  Canny(src, edge, 50, 200, 3);
  HoughLinesP(edge, lines, 1, CV_PI / 180, 20, 20, 5);
  float cnt[180] = { 0 };
  for (size_t i = 0; i < lines.size(); i++) {
    Vec4i l = lines[i];
    double y = (l[3] - l[1]), x = (l[2] - l[0]);
    double d = sqrt(x*x + y*y);
    double a = atan2(-y, x)* 360 /(2* CV_PI);
    setarr(cnt, 180, a, d);
  }
  int j = 0;
  for (int i = 1; i < 180; ++i) {
    if (cnt[j] < cnt[i]) {
      j = i;
    }
  }

  if (debug) {
    Mat color_edge;
    cvtColor(edge, color_edge, CV_GRAY2BGR);
    for (size_t i = 0; i < lines.size(); i++) {
      Vec4i l = lines[i];
      line(color_edge, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 1);
    }
    //namedWindow("lines", CV_WINDOW_NORMAL);
    imshow("lines", mergeImg(10, 500, 0, src, edge, color_edge));
  }
  return j%180;
}

int test_hough(int argc, char** argv)
{
  Mat src;
  string root = "E:/data/ew_id/mtcnn/reg/";
  string txtfile = root + "images/train.txt";
  vector<string> lines;
  wstd::readlines(txtfile.c_str(), lines);
  int cnt = 0;
  int okcnt = 0;
  for (int i = 32; i < lines.size(); ++i) {
    vector<string> strs;
    wstd::split(strs, lines[i], ";");
    string fn = root + strs[0];
    double angle = atof(strs[1].c_str());
    src = imread(fn.c_str());
    if (!src.data) {
      //return -1;
      continue;
    }
    int a = getImageAngle(src);
    int b = int(angle * 360) % 180;
    ++cnt;
    if (fabs(a - b) < 5) {
      ++okcnt;
      printf("%3d %3d %3d ", i, a, b);
      printf("%5.4lf", okcnt*1. / cnt);
    }
    if (debug) {
      waitKey(0);
    }
    printf("\n");
  }
  return 0;
}

int test_hough1(int argc, char** argv)
{
  Mat src, edge, color_edge;
  char buf[256];
  for (int i = 100; i < 1000; ++i) {
    snprintf(buf, 256, "E:/data/ew_id/mtcnn/48a-gpu/images/positive/%d.jpg", i);
    src = imread(buf);
    if (!src.data) {
      //return -1;
      continue;
    }

    Canny(src, edge, 50, 200, 3);
    cvtColor(edge, color_edge, CV_GRAY2BGR);
    vector<Vec4i> lines;
    HoughLinesP(edge, lines, 1, CV_PI / 180, 20, 10, 5);
    for (size_t i = 0; i < lines.size(); i++)
    {
      Vec4i l = lines[i];
      line(color_edge, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 1);
    }
    printf("%d\n", i);
    namedWindow("lines", CV_WINDOW_NORMAL);
    imshow("lines", color_edge);
    waitKey(0);
  }
  return 0;
}

