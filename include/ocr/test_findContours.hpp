
#include "wstd/filesystem.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "opencvex/opencvex.hpp"
#include "opencvex/test_minrect.cpp"
//#include "opencvex/bernsen.hpp"
//#include "opencvex/niblack.hpp"
//#include "opencvex/niblack2.hpp"
//#include "opencvex/sauvola.hpp"

using namespace cv;
using namespace std;

// 画箭头( draw an arrow)

int test_drawArrow()
{
  Mat M(400, 400, CV_8UC3, Scalar(0, 0, 0));
  Point pStart(380, 100), pEnd(100, 250);
  Scalar lineColor(0, 255, 255);
  drawArrow(M, pStart, pEnd, 10, 45, lineColor);
  pStart = Point(100, 100);
  pEnd = Point(320, 190);
  lineColor = Scalar(0, 0, 255);
  drawArrow(M, pStart, pEnd, 25, 30, lineColor, 2, CV_AA);
  pStart = Point(200, 420);
  pEnd = Point(370, 170);
  lineColor = Scalar(255, 0, 255);
  drawArrow(M, pStart, pEnd, 17, 15, lineColor, 1, 4);
  imshow("draw arrow", M);
  waitKey();
  return 0;
}


/** @function main */
int test_findContours(int argc, char** argv)
{

  Mat src; Mat src_gray;
  int thresh = 100;
  int max_thresh = 255;
  RNG rng(12345);

  Mat threshold_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  Mat drawing;

  /// 加载源图像
  string im_dir = "E:/data/ew_id/we-loan.oss-cn-shanghai.aliyuncs.com/";
  string fn = im_dir + "upload/backend/20160927235243069-2951.jpg";
  vector<string> list;
  wstd::readlines("E:/data/ew_id/list1.txt", list);
  for (int j = 0; j < list.size(); ++j) {
    vector<string> strs;
    wstd::split(strs, list[j], " ");
    string fn = im_dir + strs[0];
    Mat src1 = imread(fn.c_str(), 1);
    if (src1.rows *src1.cols < 10) { continue; }
    double k = 200. / src1.rows;
    resize(src1, src, Size(), k, k);
    /// 转成灰度图并进行模糊降噪
    cvtColor(src, src_gray, CV_BGR2GRAY);
    blur(src_gray, src_gray, Size(5, 5));

    /// 创建窗体
   // char* source_window = "Source";
    //namedWindow(source_window, CV_WINDOW_NORMAL);
    //imshow(source_window, src);

    //createTrackbar(" Threshold:", "Source", &thresh, max_thresh, thresh_callback);


      /// 对图像进行二值化
      //threshold(src_gray, threshold_output, thresh, 255, THRESH_BINARY);
    threshold(src_gray, threshold_output, thresh, 255, THRESH_OTSU);
    //imshow("threshold", mergeImgs(10, 400, 0, src_gray, threshold_output));
    //Niblack(src_gray, threshold_output, cv::Size(101, 101));
    //threshold_output = src_gray.clone();
    /// 寻找轮廓
    findContours(threshold_output.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    /*Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);*/

    /* 对每个轮廓计算其凸包*/
    vector<vector<Point> >poly(contours.size());
    for (int i = 0; i < contours.size(); i++)
    {
      approxPolyDP(Mat(contours[i]), poly[i], 5, true);
    }

    /* 绘出轮廓及其凸包*/
    drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
    std::vector<Mat> matarr;
    matarr.push_back(src);
    matarr.push_back(src_gray);
    matarr.push_back(threshold_output);
    matarr.push_back(drawing);
    for (int i = 0; i < contours.size(); i++)
    {
      Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
      if (0) {
        //drawContours(drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
        drawContours(drawing, poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
      }

      RotatedRect rectPoint = minAreaRect(poly[i]);
      if (rectPoint.size.area() > 1000) {
        drawRotatedRect(drawing, rectPoint, color, 1);
        Mat dst = getSubImage(src, rectPoint, 0, 0);
        matarr.push_back(dst);
      }
    }
    /// 把结果显示在窗体
    //namedWindow("Hull demo", CV_WINDOW_NORMAL);
    imshow("Hull", mergeImgs(0, 200, 0, matarr));
    //imshow("Hull", mergeImgs(0, 200, 0, src, src_gray, threshold_output, drawing));
    //imshow("src1", src1);
    waitKey(0);
  }
  //imshow("matarr", mergeImgss(10, 200, 0, matarr));
  /*vector<Point> poly;
  approxPolyDP(Mat(contours[3]), poly, 5, false);
  vector<Point>::const_iterator itp = poly.begin();
  while (itp != (poly.end() - 2))
  {
  line(drawing, *itp, *(itp + 1), Scalar(255), 2);
  ++itp;
  }
  line(drawing, *itp, *(poly.begin()), Scalar(255), 2);*/

  return(0);
}

