
#include "wstd/filesystem.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "opencvex/opencvex.hpp"
#include "test_sub_match.cpp"
#include "face/face.inl"
#include "face/cascadearr.h"
#include "face/face_recog.inl"
using namespace std;
using namespace cv;

int isdebug = 0;
int iline = 0;
vector<Mat> vecdst;

int proc_key(int c, int npic, int& iline) {
  if (c == 'p' || c == 'P') {
    iline = iline + npic - 1;
    iline %= npic;
  }
  else {
    ++iline;
  }
  return c;
}

RotatedRect face_rect_to_card_rect(const RotatedRect& face, int angleoff, double scale) {
  double w = face.size.width;
  double len = -w * 2;
  double a = (face.angle- angleoff) * CV_PI / 180;
  Point2f end(face.center.x + len*cos(a), face.center.y + len*sin(a));
  return RotatedRect(end, Size2f(w*6*scale, w*4 * scale), face.angle);
}
struct FaceDetector {
  vector<RotatedRect> rects;
  int run(const Mat& src) {
    buf_t bf[1] = { 0 };
    bfsetsize(bf, 50 * _1M);
    XRECT out[100];
    rects.resize(0);
    Mat gray;
    //vector<Mat> vv;
    for (int i = 0; i < 4; ++i) {
      rotate90(src, gray, i);
      //vv.push_back(gray.clone());
      int n;
      double ss = 300. / gray.rows;
      n = face_detect(bf, vis_nesting_face20110713, gray.rows, gray.cols, gray.data, gray.step1(),
        0, 0, ss, 1, 1000, 1.1, 1, 0.8, 5, out, countof(out), 0);
      for (int j = 0; j < n; j++) {
        const XRECT& r = out[j];
        RotatedRect rr(Point2f(r.x + r.w *0.5, r.y + r.h *0.5), Size2f(r.w, r.h), 0);
        cv::Point2f center(gray.cols / 2., gray.rows / 2.);

        rr = rect_rotate90(rr, Size(gray.cols, gray.rows), -i);
        rects.push_back(rr);
      }
    }
    //cv::imshow("srcImage", mergeImgs(10, 0, 200, vv));
    //cv::waitKey(0);
    bffree(bf);
    return rects.size();
  }
};

RNG rng(12345);

struct get_angle_t {
  int iangle = 0;
  double max_dis;;
  double centor_dis;
  int run(Mat src) {
    cv::Ptr<cv::MSER> mesr1 = cv::MSER::create(2, 10, 200, 0.2, 0.3);
    Mat gray;
    std::vector<cv::Rect> bboxes1;
    std::vector<std::vector<cv::Point> > regContours;
    cv::cvtColor(src, gray, CV_BGR2GRAY);

    mesr1->detectRegions(gray, regContours, bboxes1);
    cv::Mat mserMapMat = cv::Mat::zeros(src.size(), CV_8UC1);

    for (int i = (int)regContours.size() - 1; i >= 0; i--)
    {
      // 根据检测区域点生成mser+结果
      const std::vector<cv::Point>& p = regContours[i];
      Rect r = bboxes1[i];
      //if (r.width > 30 || r.height > 30 || r.height<2 || r.width<2)        continue;
      RotatedRect rr(Point2f(r.x + r.width / 2, r.y + r.height / 2), Size2f(r.width, r.height), 0);
      //int k = 10;
      //if (rr.size.height < k || rr.size.width < k) { continue; }
      rr = curr(rr);
      double aa = rr.size.height*1. / rr.size.width;
      double d = ptdist(rr.center, Point(src.cols*0.8, src.rows*0.4))*1. / src.cols;
      if (d>0.1 && (aa) > 0.5 && rr.size.area() < 1000 && rr.size.area() > 50)
      {
        RotatedRect rectPoint = minAreaRect(p);
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        //drawRotatedRect(color_edge, rectPoint, color, 1);
        for (int j = 0; j < (int)p.size(); j++)
        {
          cv::Point pt = p[j];
          //if (ptInRect(pt.x, pt.y, r))
          mserMapMat.at<unsigned char>(pt) = 255;
        }
      }
    }
    Mat color_edge;
    cv::cvtColor(mserMapMat, color_edge, CV_GRAY2BGR);
    if (1) {
      vector<vector<Point> > contours;
      vector<Vec4i> hierarchy;
      findContours(mserMapMat.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
      for (int i = 0; i < (int)contours.size(); i++) {
        cv::Rect r0 = cv::boundingRect(contours[i]);
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        cv::rectangle(color_edge, r0, color, 1);
      }
    }
    vector<Point > pts;
    vector<RotatedRect> vec_rrect;
    float anglecnt[180] = { 0 };
    vector<Vec4i> lines;
    Vec4i botton_line(0);
    if (1) {
      // 检测直线，最小投票为90，线条不短于50，间隙不小于10
      HoughLinesP(mserMapMat, lines, 1, CV_PI / 180, 50, 30, 30);
      drawDetectLines(color_edge, lines, Scalar(0, 255, 0));
      for (int i = 0; i < lines.size(); ++i) {
        Vec4i l = lines[i];
        double x = (l[2] - l[0]);
        double y = (l[3] - l[1]);
        int angle = (int)(atan2(y, x) * 180 / CV_PI);
        double len = line_len(l);
        angle = mymodi(angle, 180);
        anglecnt[(angle + 180) % 180] += 2 * len;
        anglecnt[(angle + 181) % 180] += len;
        anglecnt[(angle + 179) % 180] += len;
        //printf("%d\n", angle);
        if (l[1] > botton_line[1]) {
          botton_line = l;
        }
      }
    }

    double maxv = 0;
    for (int i = 0; i < 180; ++i) {
      if (maxv < anglecnt[i]) {
        maxv = anglecnt[i];
        iangle = i;
      }
    }
    int ret = maxv > 1000;
    max_dis = 0;
    if (ret) {
      for (int i = 0; i < lines.size(); ++i) {
        Vec4i l = lines[i];
        double x = (l[2] - l[0]);
        double y = (l[3] - l[1]);
        int angle = (int)(atan2(y, x) * 180 / CV_PI);
        angle = mymodi(angle, 180);
        int diff = angle - iangle;
        diff = mymodi(diff, 180);
        diff = abs(diff);
        if (diff < 5) {
          Point pt(l[0], l[1]);
          double d = pt2Line(pt, botton_line);
          if (d > max_dis) {
            max_dis = d;
          }
        }
      }
      centor_dis = pt2Line(Point(src.cols*0.5, src.rows*0.5), botton_line);
      cv::putText(color_edge, wstd::format("%d %d", (int)max_dis, (int)centor_dis), Point(0, 50), FONT_HERSHEY_COMPLEX, 2, Scalar(0, 255, 255), 2, 8, 0);

      vecdst.push_back(color_edge);
    }
    return ret;
  }
};

int test_facedetect_idcard()
{
  //return test_hsv_bin();
  //return test_myfindContours();
  int k;
  RNG rng(12345);
  /// 加载源图像
  string im_dir = "E:/data/ew_id/we-loan.oss-cn-shanghai.aliyuncs.com/";
  string fn = im_dir + "upload/backend/20160927235243069-2951.jpg";
  vector<string> list;
  wstd::readlines("E:/data/ew_id/list1.txt", list, 2000);
  iline = 1182;
  iline = 1192;
  iline = 1222;
  iline = 1229;
  iline = 0;
  FaceDetector face_etector;
  int c = 0;
  int n = list.size();
  n = min(iline+100, n);
  srand(11);
  for (; iline < n; ) {
    vector<string> strs;
    wstd::split(strs, list[iline], " ");
    string fn = im_dir + strs[0];
    cv::Mat src, srcImage, color_edge, gray;
    src = cv::imread(fn, 1);
    printf("%d %s %d %d\n", iline, wstd::path_split_filename(fn.c_str()).c_str(), src.rows, src.cols);
    if (src.empty()) {
      proc_key(c, list.size(), iline);
      continue;
    }
    int aa_angle = 10;
    imRotation(src, src, iline &1 ? aa_angle :-aa_angle);
    //cv::resize(src, src, cv::Size(), 1, 1.5, cv::INTER_LANCZOS4);
    srcImage = src;
    color_edge = src.clone();
    cv::cvtColor(srcImage, gray, CV_BGR2GRAY);
    face_etector.run(gray);
    vector<RotatedRect> cardrect;
    vecdst.clear();
    get_angle_t get_angle;
    for (int i = 0; i < face_etector.rects.size(); ++i) {
      RotatedRect& face_rect = face_etector.rects[i];
      RotatedRect r = face_rect_to_card_rect(face_rect, 15, 1);
      if (Rect(0, 0, gray.cols, gray.rows).contains(r.center)) {
        Mat dst = getSubImage(src, r, Size(300, 200));
        if (get_angle.run(dst)) {
          int iangle = get_angle.iangle;
          if (iangle > 90) {
            iangle -= 180;
          }
          if (fabs(iangle) < 20) {
            r.angle += iangle;
            face_rect.angle += iangle;
            double tt = 65;
            //double scale, double y_off
            if (get_angle.centor_dis < tt) {
              int a = (tt - get_angle.centor_dis)*0.2;
              a = MIN(10, a);
              r = face_rect_to_card_rect(face_rect, 10 - a, 1);
            }
            else {
              double b = (get_angle.centor_dis*1./tt);
              r = face_rect_to_card_rect(face_rect, 10, b);
            }
            Mat dst_ok = getSubImage(src, r, Size(300, 200));
            vecdst.push_back(dst_ok);
            cardrect.push_back(r);
          }
        }
      }
      if (0) {
        Point2f fourPoint2f[4];
        r.points(fourPoint2f);
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        circle(color_edge, fourPoint2f[0], 5, color, -1);
        circle(color_edge, fourPoint2f[1], 5, color, -1);
      }
    }
    drawRotatedRects(color_edge, face_etector.rects, 2);
    drawRotatedRects(color_edge, cardrect, 2);

    vecdst.push_back(color_edge);
    Mat dst = mergeImgs(10, 0, 300, vecdst);
#ifdef _DEBUG
    isdebug = 1;
#endif
    if (isdebug) {
      cv::imshow("vecdst", dst);
      c  = cv::waitKey(0);
      proc_key(c, list.size(), iline);
    }
    else {
      string fn = wstd::format("C:\\outpic\\out_%03d_z.jpg", iline);
      cv::imwrite(fn, dst);
      ++iline;
    }
  }
  return 0;
}