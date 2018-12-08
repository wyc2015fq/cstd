
#include "wstd/filesystem.hpp"
//#include <iostream>
#include "test_sub_match.hpp"
#include "face/face.inl"
#include "face/cascadearr.h"
//#include "face/face_recog.inl"
#include "face/face.inl"
#include "face/cascadearr.h"
//#include "face/DSP_feat.h"
using namespace std;

int isdebug = 0;
int iline = 0;

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

#include "opencvex/opencvex.hpp"

RotatedRect face_rect_to_card_rect(const RotatedRect& face, double y_off_scale, double scalex, double scaley) {
  double w = face.size.width;
  double len = -w * 2;
  double a = (face.angle) * CV_PI / 180;
  Point2f center = ptMove(face.center, face.angle - 90, y_off_scale*len);
  Point2f end(center.x + len*cos(a), center.y + len*sin(a));
  return RotatedRect(end, Size2f(w * 6* scalex, w * 4 * scaley), face.angle);
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
  Vec4i botton_line;
  Mat color_edge;
  vector<Vec4i> lines;
  vector<RotatedRect> rects;
  int run(Mat src) {
    cv::Ptr<cv::MSER> mesr1 = cv::MSER::create(2, 10, 200, 0.2, 0.3);
    Mat gray;
    std::vector<cv::Rect> bboxes1;
    std::vector<std::vector<cv::Point> > regContours;
    cv::cvtColor(src, gray, CV_BGR2GRAY);

    mesr1->detectRegions(gray, regContours, bboxes1);
    cv::Mat mserMapMat = cv::Mat::zeros(src.size(), CV_8UC1);
    vector<int> cnt;
    vector<int> cnt1;
    if (1) {
      cnt.assign(src.cols, 0);
      cnt1.assign(src.rows, 0);
      for (int i = (int)regContours.size() - 1; i >= 0; i--) {
        Rect rc = bboxes1[i];
        if (rc.x < 50 || rc.x>250 || rc.y < 30 || rc.y>170) {
          cnt[rc.x]++;
          cnt1[rc.y]++;
        }
      }
      cnt = sum_part(cnt, 20);
      cnt1 = sum_part(cnt1, 20);
    }

    for (int i = (int)regContours.size() - 1; i >= 0; i--)
    {
      // 根据检测区域点生成mser+结果
      const std::vector<cv::Point>& p = regContours[i];
      Rect rc = bboxes1[i];
      //if (r.width > 30 || r.height > 30 || r.height<2 || r.width<2)        continue;
      RotatedRect r(Point2f(rc.x + rc.width / 2, rc.y + rc.height / 2), Size2f(rc.width, rc.height), 0);
      r = minAreaRect(p);
      //int k = 10;
      //if (rr.size.height < k || rr.size.width < k) { continue; }
      double aa = r.size.width*1. / r.size.height;
      double d = ptdist(r.center, Point(src.cols*0.8, src.rows*0.4))*1. / src.cols;
      int angle = mymodi(r.angle - 90, 90);
      int c = 0;
      //c = cnt[labels[i]];
      c = cnt[rc.x]+ cnt1[rc.y];
      if ((c<200) && d>0.1 && (aa<2 || (aa<4 && fabs(angle)<10)) && r.size.area() < 200 && r.size.area() > 10)
      {
        //RotatedRect rectPoint = minAreaRect(p);
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        //drawRotatedRect(color_edge, rectPoint, color, 1);
        rects.push_back(r);
        for (int j = 0; j < (int)p.size(); j++)
        {
          cv::Point pt = p[j];
          //if (ptInRect(pt.x, pt.y, r))
          mserMapMat.at<unsigned char>(pt) = 255;
        }
      }
    }

    if (0) {
      Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
      morphologyEx(mserMapMat, mserMapMat, MORPH_ERODE, element);
    }
    cv::cvtColor(mserMapMat, color_edge, CV_GRAY2BGR);
    drawRotatedRects(color_edge, rects, 1);
    imshow("color_edge", color_edge);// waitKey(0);
    if (0) {
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
    botton_line = Vec4i(0);
    Vec4i top_line = Vec4i(0, 1000, 200, 1000);
    double max_angle = 20; //最大允许倾斜角度
    if (1) {
      // 检测直线，最小投票为90，线条不短于50，间隙不小于10
      HoughLinesP(mserMapMat, lines, 1, CV_PI / 180, 50, 30, 30);
      //drawDetectLines(color_edge, lines, 2);
      for (int i = 0; i < lines.size(); ++i) {
        Vec4i l = lines[i];
        double len = line_len(l);
        int angle = line_angle(l);
        if (fabs(angle) < 30) {
          anglecnt[(angle + 180) % 180] += 2 * len;
          anglecnt[(angle + 181) % 180] += len;
          anglecnt[(angle + 179) % 180] += len;
          //printf("%d\n", angle);
          if (l[1] > botton_line[1]) {
            botton_line = l;
          }
          if (l[1] < top_line[1]) {
            top_line = l;
          }
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
    if (iangle > 90) {
      iangle -= 180;
    }
    if (fabs(iangle) > max_angle) {
      ret = 0;
    }
    if ((botton_line[1] - top_line[1]) < 50) {
      ret = 0;
    }
    if (ret) {
      vector<Vec4i> lines1;
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
          double d = line_len(l);
          if (d > max_dis) {
            max_dis = d;
          }
        }
      }
      if (lines.size()>0) {
        mylines_part_t lp;
        lp.run(lines);
        lines.resize(0);
        double prelen = 0;
        double precnt = 0;
        for (int j = 0; j < lp.lines1.size(); ++j) {
          Vec4i l = lp.lines1[j];
          double len = line_len(l);
          int angle = line_angle(l);
          int cnt = rect_near_count(l, rects, 10);
          if (fabs(angle)<30 && l[1] >= 70 && len>prelen && cnt>precnt) {
            prelen = len;
            precnt = cnt;
            lines.push_back(lp.lines1[j]);
          }
        }
      }
      centor_dis = pt2Line(Point(src.cols*0.5, src.rows*0.5), botton_line);
      cv::putText(color_edge, wstd::format("%d %d", (int)max_dis, (int)centor_dis), Point(0, 50), FONT_HERSHEY_COMPLEX, 2, Scalar(0, 255, 255), 2, 8, 0);
      drawDetectLines(color_edge, lines, 2);
      //imshow("color_edge", color_edge);
    }
    return ret;
  }
};

struct get_rect_t {
  vector<RotatedRect> lines_rect;
  vector<RotatedRect> lines_ok;
  vector<Vec4i> lines;
  vector<Vec4i> linesxx;
  Mat color_edge;
  int run(const Mat& src) {
    int ret = 1;
    cv::Ptr<cv::MSER> mesr1 = cv::MSER::create(2, 10, 200, 0.2, 0.3);
    Mat gray;
    std::vector<cv::Rect> bboxes1;
    std::vector<cv::RotatedRect> vrr;
    std::vector<std::vector<cv::Point> > regContours;
    cv::cvtColor(src, gray, CV_BGR2GRAY);
    Rect aRect(0,0,src.cols, src.rows);
    mesr1->detectRegions(gray, regContours, bboxes1);
    cv::Mat mserMapMat = cv::Mat::zeros(src.size(), CV_8UC1);
    Mat bw;

    vector<int> cnt;
    vector<int> cnt1;
    if (1) {
      cnt.assign(src.cols, 0);
      cnt1.assign(src.rows, 0);
      for (int i = (int)regContours.size() - 1; i >= 0; i--) {
        Rect rc = bboxes1[i];
        if (rc.x < 50 || rc.x>250 || rc.y < 20 || rc.y>190) {
          cnt[rc.x]++;
          cnt1[rc.y]++;
        }
      }
      cnt = sum_part(cnt, 30);
      cnt1 = sum_part(cnt1, 20);
    }

    for (int i = (int)regContours.size() - 1; i >= 0; i--)
    {
      // 根据检测区域点生成mser+结果
      const std::vector<cv::Point>& p = regContours[i];
      Rect rc = bboxes1[i];
      //if (r.width > 30 || r.height > 30 || r.height<2 || r.width<2)        continue;
      RotatedRect r(Point2f(rc.x + rc.width / 2, rc.y + rc.height / 2), Size2f(rc.width, rc.height), 0);
      r = minAreaRect(p);
      //int k = 10;
      //if (rr.size.height < k || rr.size.width < k) { continue; }
      double aa= r.size.width*1. / r.size.height;
      double d = ptdist(r.center, Point(src.cols*0.8, src.rows*0.4))*1. / src.cols;
      int angle = mymodi(r.angle - 90, 90);
      int c = 0;
      c = cnt[rc.x]+cnt1[rc.y];
      if ((c<150) && d>0.1 && (aa<2 || (aa<4 && fabs(angle)<10)) && r.size.area() < 200 && r.size.area() > 10)
      {
        RotatedRect rectPoint = minAreaRect(p);
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        //drawRotatedRect(color_edge, rectPoint, color, 1);
        vrr.push_back(r);
        for (int j = 0; j < (int)p.size(); j++)
        {
          cv::Point pt = p[j];
          //if (ptInRect(pt.x, pt.y, r))
          mserMapMat.at<unsigned char>(pt) = 255;
        }
      }
      else {
        int asdf = 0;
      }
    }

    if (0) {
      Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
      morphologyEx(mserMapMat, mserMapMat, MORPH_ERODE, element);
    }
    if (0) {
      Mat element = getStructuringElement(MORPH_RECT, Size(1, 3));
      morphologyEx(mserMapMat, mserMapMat, MORPH_DILATE, element);
    }

    cv::cvtColor(mserMapMat, color_edge, CV_GRAY2BGR);
    rectangle(color_edge, aRect, Scalar(0,0,255), 1);
    if (0) {
      Mat ttt;
      //bw = gray;
      ttt = getHistImage(bw, 0, &color_edge);
      ttt = getHistImage(bw, 1, &color_edge);
    }
    vector<RotatedRect> vec_rrect;
    vector<RotatedRect> bot_rrect;
    if (1) {
      vector<vector<Point> > contours;
      vector<Vec4i> hierarchy;
      findContours(mserMapMat.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
      for (int i = 0; i < (int)contours.size(); i++) {
        cv::Rect r0 = cv::boundingRect(contours[i]);
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        //cv::rectangle(color_edge, r0, color, 1);
        cv::RotatedRect r = minAreaRect(contours[i]);
        r = curr(r);
        double aa = r.size.width*1. / r.size.height;
        if ((aa<2 || (aa<3 && fabs(mymodi(r.angle, 90))<10)) && r.size.area() < 300 && r.size.area() > 10 && (r.center.x - 150)<r.center.y)
        {
          vec_rrect.push_back(r);
        }
      }
      drawRotatedRects(color_edge, vec_rrect, 1);
    }
    if (1) {
      // 检测直线，最小投票为90，线条不短于50，间隙不小于10
      HoughLinesP(mserMapMat, lines, 1, CV_PI / 180, 50, 30, 30);
      int n = 0;
      for (int i = 0; i < lines.size(); ++i) {
        Vec4i l = lines[i];
        double x = (l[2] - l[0]);
        double y = (l[3] - l[1]);
        int angle = (int)(atan2(y, x) * 180 / CV_PI);
        double len = line_len(l);
        angle = mymodi(angle, 180);
        if (fabs(angle) < 5)
        {
          lines[n++] = l;
        }
      }
      lines.resize(n);
      if (n>0) {
        vector<int> labels;
        double thd = 10;
        vector<vector<Vec4i>> lines_class;
        vector<Vec4i> lines1;
        int nclasses = cv::partition(lines, labels, SimilarLine2(thd, thd * 3));
        lines_class.resize(nclasses);
        vector<int> cnt(lines.size(), 0);
        lines1.resize(nclasses);
        for (int i = 0; i < labels.size(); ++i) {
          int j = labels[i];
          ++cnt[j];
        }
        //nclasses = min(nclasses, 1);
        std::vector<int> index = argsort(cnt);
        for (int i = 0; i < nclasses; ++i) {
          int idx = index[i];
          //vector<Vec4i> line_tmp;
          Vec4i s(0);
          double all_len = 0;
          for (int j = 0; j < lines.size(); ++j) {
            if (labels[j] == idx) {
              //line_tmp.push_back(lines[j]);
              double len = line_len(lines[j]);
              all_len += len;
              s += lines[j] * len;
            }
          }
          lines1[i] = s *(1./ all_len);
        }
        lines = lines1;
        lines.resize(0);
        for (int j = 0; j < lines1.size(); ++j) {
          double len = line_len(lines1[j]);
          if(lines1[j][1]>=120) {
            lines.push_back(lines1[j]);
          }
        }
      }
      if (lines.size() > 0) {
        vector<int> cnt(lines.size(), 0);
        for (int i = 0; i < vec_rrect.size(); ++i) {
          int j = near_line(lines, vec_rrect[i].center, vec_rrect[i].size.height);
          if (j>=0) {
            cnt[j]++;
          }
        }
        int idx = argmax(cnt);
        if (cnt[idx] > 9 && lines[idx][1]>130) {
          linesxx.push_back(lines[idx]);
          lines.assign(1, lines[idx]);
          drawDetectLines(color_edge, lines, 2);
          ret = 1;
        }
        else {
          ret = 0;
          lines.clear();
        }
      }
    }
    vector<vector<RotatedRect>> rrects2;
    vector<int> labels;
    if (1) {
    vector<int> cnt;
      vector<Point > pts;
      Vec4i top_line = Vec4i(0, 1000, 200, 1000);
      double max_angle = 20; //最大允许倾斜角度
      vector<RotatedRect> vec_rrect2 = vec_rrect;
      vec_rrect.clear();
      if (ret && linesxx.size() > 0) {
        const Vec4i& l = linesxx[0];
        double maxy = (l[1] - 20);
        double miny = (200 - l[1] - 30);
        for (int i = 0; i < vec_rrect2.size(); ++i) {
          const RotatedRect& r = vec_rrect2[i];
          if (miny < r.center.y && r.center.y < maxy) {
            vec_rrect.push_back(r);
          }
          double d;
          //d = fabs(r.center.y - l[1]);
          d = pt2Line(r.center, l);
          if (d < 10) {
            bot_rrect.push_back(r);
          }
        }
      }
    }
    double meanh = 0;
    if (bot_rrect.size() > 5) {
      double s = 0;
      for (int i = 0; i < bot_rrect.size(); ++i) {
        s += bot_rrect[i].size.height;
      }
      meanh = s / bot_rrect.size();
      if (0) {
        vector<RotatedRect> bot_rrect2 = bot_rrect;
        bot_rrect.clear();
        RotatedRect r = minAreaRect(bot_rrect2);
        Vec4f l = linesxx[0];
        for (int i = 0; i < bot_rrect2.size(); ++i) {
          double d = fabs(bot_rrect2[i].center.y - r.center.y);
          d = pt2Line(bot_rrect2[i].center, l);
          if (d < (meanh)) {
            bot_rrect.push_back(bot_rrect2[i]);
          }
        }
      }
    }

    if (ret && linesxx.size() > 0) {
      //rrects.resize();
      int nclasses = cv::partition(vec_rrect, labels, [&meanh](const RotatedRect& r1, const RotatedRect& r2) {
        double minh = meanh*0.5;
        double maxh = meanh*2;
        if (r1.size.height < minh || r1.size.height>maxh || r2.size.height < minh || r2.size.height>maxh) {
          return false;
        }
        double hd = 10;
        hd = MIN(r1.size.height, r2.size.height);
        return fabs(r1.center.y-r2.center.y)<hd*0.5 && fabs(r1.size.height- r2.size.height)<hd;
      });
      cnt.assign(nclasses, 0);
      for (int i = 0; i < labels.size(); ++i) {
        int j = labels[i];
        ++cnt[j];
      }
      std::vector<int> index = argsort(cnt);
      //nclasses = min(nclasses, 15);
      vector<int> cnt2;
      rrects2.push_back(bot_rrect);
      cnt2.push_back(bot_rrect.size());
      for (int i = 0; i < nclasses; ++i) {
        int idx = index[i];
        int c = cnt[idx];
        if (c > 1) {
          vector<RotatedRect> rrects;
          for (int j = 0; j < vec_rrect.size(); ++j) {
            if (labels[j] == idx) {
              rrects.push_back(vec_rrect[j]);
            }
          }
          rrects2.push_back(rrects);
          cnt2.push_back(c);
        }
      }
      nclasses = rrects2.size();
      for (int i = 0; i < nclasses; ++i) {
        RotatedRect r = myMinAreaRect(rrects2[i], meanh);
        r = curr(r);
        const vector<RotatedRect>& rr = rrects2[i];
        double sumw = 0;
        for (int j = 0; j < rr.size(); ++j) { sumw += rr[j].size.width; }
        sumw /= r.size.width;
        if ((sumw>0.5) && (r.size.height>meanh) && (r.size.width>150 || (r.center.x - r.size.width*0.5) < 150)) {
          int k = -1;
          for (int j = 0; j < lines_rect.size(); ++j) {
            if (fabs(r.center.y - lines_rect[j].center.y) < 10) {
              k = j;
            }
          }
          if (k < 0) {
            lines_rect.push_back(r);
          }
          else if (r.size.width > lines_rect[k].size.width) {
            lines_rect[k] = r;
          }
        }
      }
      sort(lines_rect.begin(), lines_rect.end(), [](const RotatedRect& a, const RotatedRect& b) {
        return a.center.y > b.center.y;
      });
      nclasses = lines_rect.size();
      drawRotatedRects(color_edge, lines_rect, 2);
      if (lines_rect.size()>3) {
        double gapy = (lines_rect[0].center.y - lines_rect[lines_rect.size() - 1].center.y)*0.2;
        vector<RotatedRect> lines_tmp;
        if (1) {
          double tty = lines_rect[lines_rect.size() - 1].center.y-gapy;
          selectRotatedRect(vec_rrect, tty, meanh*1.5, lines_tmp);
          if (lines_tmp.size() > 1) {
            RotatedRect r = minAreaRect(lines_tmp);
            r = curr(r);
            if (r.center.x<150 && r.size.height >(meanh)) {
              lines_rect.push_back(r);
            }
          }
        }
        if (1) {
          int tmies = 0;
          for (; tmies < 4; ++tmies) {
            int has = 0;
            double tty = 0;
            for (int i = lines_rect.size() - 1; i>0; --i) {
              double tt = fabs(lines_rect[i].center.y - lines_rect[i - 1].center.y);
              double t1 = i==(lines_rect.size()-3) ? 1.3 : 1.5;
              if (tt>t1*gapy) {
                has = i;
                double gap = gapy;
                if (i == 1 && tt<2.5*gapy) {
                  gap = gapy*0.7;
                }
                tty = lines_rect[i].center.y + gap;
                break;
              }
            }
            if (has) {
              selectRotatedRect(vec_rrect, tty, meanh, lines_tmp);
              if (lines_tmp.size() > 0) {
                RotatedRect r;
                //r = minAreaRect(lines_tmp);
                r = myMinAreaRect(lines_tmp, 10);
                r = curr(r);
                if ((r.center.x- r.size.width*0.5)<150 && r.size.height >(meanh*0.8)) {
                  lines_rect.insert(lines_rect.begin() + has, r);
                }
              }
            }
          }
        }
        nclasses = lines_rect.size();
        lines_ok.resize(7);
        lines_ok[0] = lines_rect[0];
        int n = min(nclasses, 7);
        for (int i = 1; i < n; ++i) {
          lines_ok[7-i] = lines_rect[i];
        }
      }
      else {
        ret = 0;
      }
      drawRotatedRects(color_edge, rrects2, 1);
      drawRotatedRects(color_edge, lines_ok, 2);
      //imshow("color_edge", color_edge);
    }

    return ret;
  }
};
