
#include "wstd/filesystem.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "opencvex/opencvex.hpp"
#include "test_sub_match.cpp"
using namespace std;
using namespace cv;

bool isdebug;

int test_mser_opencv2()
{
  //return test_hsv_bin();
	//return test_myfindContours();
	int k;
  RNG rng(12345);
	isdebug = true;
  /// 加载源图像
  string im_dir = "E:/data/ew_id/we-loan.oss-cn-shanghai.aliyuncs.com/";
  string fn = im_dir + "upload/backend/20160927235243069-2951.jpg";
  vector<string> list;
  wstd::readlines("E:/data/ew_id/list1.txt", list, 2000);
  int iline;
  iline = 1182;
  iline = 1192;
  iline = 1222;
  iline = 1229;
  iline = 1265;
  for (; iline < list.size(); ) {
    vector<string> strs;
    wstd::split(strs, list[iline], " ");
    string fn = im_dir + strs[0];
    cv::Mat src, srcImage;
    src = cv::imread(fn, 1);
    printf("%d %s %d %d\n", iline, wstd::path_split_filename(fn.c_str()).c_str(), src.rows, src.cols);
		if (src.empty()) {
			continue;
		}
    int maxrows = 600;

      double t = maxrows*1. / src.rows;
      cv::resize(src, srcImage, cv::Size(), t, t, cv::INTER_LANCZOS4);

    //int k = 20;    cv::copyMakeBorder(src, src, k, k, k, k, cv::BORDER_CONSTANT, 0);

		//cv::imshow("src Image", srcImage);
		std::vector<cv::Rect> candidates;
		std::vector<std::vector<cv::Rect>> blocks;
		std::string out;
    Mat gray;

    if (0) {
      Rect maxrect(0, 0, 0, 0);
      Mat stdImage;
      vector<Mat> rotarr;

      for (int i = 0; i < 4; ++i) {
        Mat dst;
        rotate90(srcImage, dst, i);
        if (1) {
          Mat gray;
          XRECT out[100];
          cv::cvtColor(dst, gray, CV_BGR2GRAY);
          int n = detect_idcard(gray.rows, gray.cols, gray.data, gray.step1(), out, countof(out));
          //printf("%d\n", n);
          Rect maxrect1(0, 0, 0, 0);
          for (int j = 0; j < n; ++j) {
            Rect r(out[j].x, out[j].y, out[j].w, out[j].h);
            Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
            //rectangle(dst, r, color, 1, 8, 0);//用矩形画矩形窗
            if (r.width > maxrect1.width) {
              maxrect1 = r;
            }
          }
          if (maxrect1.width > maxrect.width) {
            maxrect = maxrect1;
            stdImage = dst;
          }
        }
        rotarr.push_back(dst);
      }
      //imshow("rotarr", mergeImgs(22, 500, 0, rotarr));
      //waitKey(0);
      if (maxrect.width < 100) {
        Scalar color = Scalar(0, 0, 255);
        rectangle(srcImage, maxrect, color, 1, 8, 0);//用矩形画矩形窗
        imshow("srcImage", srcImage);
        waitKey(0);
        continue;
      }
      //Rect rect(maxrect.x / t, maxrect.y / t, maxrect.width / t, maxrect.height / t);
      srcImage = stdImage(maxrect);
    }

    cv::Mat mserMapMat = cv::Mat::zeros(srcImage.size(), CV_8UC1);
    cv::Mat color_edge = cv::Mat::zeros(srcImage.size(), CV_8UC3);
    cv::Ptr<cv::MSER> mesr1 = cv::MSER::create(2, 10, 200, 0.2, 0.3);
    std::vector<cv::Rect> bboxes1;
    std::vector<Vec3f> bgr1;
    std::vector<std::vector<cv::Point> > regContours;
    cv::cvtColor(srcImage, gray, CV_BGR2GRAY);
    Mat graySmooth;
    double cc = clarityTenengrad(gray);
    //进行高斯滤波
    if (cc>4) {
      GaussianBlur(gray, graySmooth, Size(5, 5), 1, 1);
    } else if (cc>1.5) {
      GaussianBlur(gray, graySmooth, Size(3, 3), 1, 1);
    }
    else {
      graySmooth = gray;
    }
    double ccs = clarityTenengrad(graySmooth);
    //printf("%lf %lf\n", cc, ccs);
    mesr1->detectRegions(graySmooth, regContours, bboxes1);
    bgr1.resize(regContours.size());
    for (int i = (int)regContours.size() - 1; i >= 0; i--)
    {
      // 根据检测区域点生成mser+结果
      const std::vector<cv::Point>& p = regContours[i];
      Rect r = bboxes1[i];
      //if (r.width > 30 || r.height > 30 || r.height<2 || r.width<2)        continue;
      RotatedRect rr(Point2f(r.x + r.width / 2, r.y + r.height / 2), Size2f(r.width, r.height), 0);
      //int k = 10;
      //if (rr.size.height < k || rr.size.width < k) { continue; }
      Vec3f &bgr2 = bgr1[i];
      rr = curr(rr);
      double aa = rr.size.height*1. / rr.size.width;
      if ((aa)>0.5 && rr.size.area() < 4000 && rr.size.area() > 100)
      {
        RotatedRect rectPoint = minAreaRect(p);
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        //drawRotatedRect(color_edge, rectPoint, color, 1);
        for (int j = 0; j < (int)p.size(); j++)
        {
          cv::Point pt = p[j];
          //if (ptInRect(pt.x, pt.y, r))
          mserMapMat.at<unsigned char>(pt) = 255;
          Vec3i bgr = srcImage.at<Vec3b>(pt);
          bgr2 += bgr;
        }
        if (p.size()>0) {
          bgr2 *= 1. / p.size();
          bgr2 *= 1.;
        }
        for (int j = 0; j < (int)p.size(); j++)
        {
          cv::Point pt = p[j];
          color_edge.at<Vec3b>(pt) = bgr2;
        }
      }
    }
    cv::cvtColor(mserMapMat, color_edge, CV_GRAY2BGR);
    if (0) {
      //Mat element = getStructuringElement(MORPH_RECT, Size(15, 3));  //闭形态学的结构元素
      //morphologyEx(mserMapMat, mserMapMat, CV_MOP_ERODE, element);
      //erode(mserMapMat, mserMapMat, element);
      vector<vector<Point> > contours;
      vector<Vec4i> hierarchy;
      findContours(mserMapMat.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
      for (int i = 0; i < (int)contours.size(); i++) {
        cv::Rect r0 = cv::boundingRect(contours[i]);
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        cv::rectangle(color_edge, r0, color, 1);
      }
    }
    if (0) {
      //Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));  //闭形态学的结构元素
      //morphologyEx(mserMapMat, mserMapMat, CV_MOP_ERODE, element);
      vector<vector<Point> > contours;
      vector<Vec4i> hierarchy;
      findContours(mserMapMat.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
      vector<vector<Point> >poly(contours.size());
      for (int i = 0; i < contours.size(); i++)
      {
        approxPolyDP(Mat(contours[i]), poly[i], 5, true);
      }
      for (int i = 0; i < (int)contours.size(); i++) {
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        //if (rectPoint.size.area() > 1000)
        if (1) {
          RotatedRect rectPoint = minAreaRect(contours[i]);
          drawRotatedRect(color_edge, rectPoint, color, 1);
        }
      }
    }
    if (1) {
      vector<Point > pts;
      vector<Vec4i> lines;
      vector<RotatedRect> vec_rrect;
      vector<Vec3f> bgr;
      int nline;
      if (1) {
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        findContours(mserMapMat.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
        for (int i = (int)contours.size() - 1; i >= 0; i--) {
          RotatedRect rr = minAreaRect(contours[i]);
          rr = curr(rr);
          double aa = rr.size.height*1. / rr.size.width;
          if (aa>0.3 && rr.size.area() < 1000 && rr.size.area()>100) {
            pts.push_back(Point(rr.center.x, rr.center.y));
            vec_rrect.push_back(rr);
          }
        }
      }
      else {
        for (int i = (int)bboxes1.size() - 1; i >= 0; i--) {
          const Rect& r = bboxes1[i];
          RotatedRect rr(Point2f(r.x + r.width / 2, r.y + r.height / 2), Size2f(r.width, r.height), 0);
          double xk = rr.center.x*1. / srcImage.cols;
          double yk = rr.center.y*1. / srcImage.rows;
          double mink = 0.05;
          double maxk = 1- mink;
          if (rr.size.area() < 400 && rr.size.area() > 40 && xk>mink && xk<maxk && yk>mink && yk<maxk) {
            pts.push_back(Point(rr.center.x, rr.center.y));
            vec_rrect.push_back(rr);
            bgr.push_back(bgr1[i]);
          }
        }
      }
      if (1) {
        drawRotatedRects(color_edge, vec_rrect, 1);
      }
      nline = icvHoughLinesProbabilistic(pts.data(), pts.size(), srcImage.rows, srcImage.cols, 1, CV_PI / 180, 8, 30, 10, lines, 20);
      lines_part_t lp;
      lp.run(lines, vec_rrect, 2);
      int nvline = lp.lines_class.size();
      //nvline = MIN(lines_vec.size(), 1);
      if (0) {
        for (int j = 0; j < nvline; ++j) {
          Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
          vector<Vec4i>& lines = lp.lines_class[j];
          nline = lines.size();
          for (int i = 0; i < nline; ++i) {
            double lw = (nline - i) * 1;
            lw = max(lw, 1.);
            lw = 1;
            Vec4i l = lines[i];
            line(color_edge, Point(l[0], l[1]), Point(l[2], l[3]), color, lw, 8);
          }
        }
      }
      if (nvline > 0) {
        double k = 0;
        vector<Vec4i>& lines = lp.lines_class[0];
        for (int i = 0; i < lines.size(); ++i) {
          Vec4i l = lines[i];
          int x = (l[2] - l[0]);
          int y = (l[3] - l[1]);
          k += y / (x + 0.00001);
        }
        k /= lines.size();
        int angle = (int)(atan(k) * 180 / CV_PI);
        if (0) {
          imRotation(color_edge.clone(), color_edge, angle);
          imRotation(srcImage.clone(), srcImage, angle);
          imRotation(mserMapMat.clone(), mserMapMat, angle);
        }
        rrects_part rp;
        rp.run(vec_rrect, angle, 0.1, 10);
        rrects_w_part wp;
        wp.run(rp.lines_bound, angle, 0.1);
        if (1) {
          //drawRotatedRects(color_edge, rp.lines_class, 1);
          drawRotatedRects(color_edge, rp.lines_bound, 2);
          //drawRotatedRects(color_edge, wp.lines_bound, 2);
        }
        const vector<RotatedRect>& lines_bound = rp.lines_bound;
        if (0) {
          //double di[] = { 47,26,26,44,44,44 };
          double dia[] = { 0, 47, 73, 99, 143, 187, 231 };
          //double dia[] = { 0, 99, 143, 187, 231 };
          double a = (angle)*CV_PI / 180;
          double s = -sin(a);
          double c = cos(a);
          std::vector<double> y_vec(lines_bound.size(), 0);
          std::vector<int> index(lines_bound.size(), 0);
          for (int i = 0; i < y_vec.size(); ++i) {
            const RotatedRect& r1 = lines_bound[i];
            double y1 = r1.center.x*s + r1.center.y*c;
            y_vec[i] = y1;
            //printf(" %5.2lf\n", y1);
          }
          index = argsort_d(y_vec);
          y_vec = select_index_all(y_vec, index);
          int mk = 0;
          vector<RotatedRect> lines = select_index_all(lines_bound, index);
          for (int i = 0; i < y_vec.size(); ++i) {
            y_vec[i] = y_vec[i];
            //printf("%d %lf\n", i+1, pow(i+1, 0.01));
          }
          //for (int i = 0; i < countof(di); ++i) {            dia[i+1] = dia[i] + di[i];          }
          std::sort(y_vec.begin(), y_vec.end());
          int ma = 0, mb = 0;
          double* arr = &y_vec[0];
          double dis = sub_match(arr, y_vec.size(), dia, countof(dia), 3, 100, -1, 1, ma, mb);
          //print_mask_arr(arr, y_vec.size(), ma);
          //print_mask_arr(dia, countof(dia), mb);
          //printf("%lf\n", dis);
          double sign = 1;
          if (1) {
            int n = y_vec.size();
            std::vector<double> y_vec_r(lines_bound.size(), 0);
            for (int i = 0; i < n; ++i) {
              y_vec_r[i] = - y_vec[n-1-i];
            }
            double* arr = &y_vec_r[0];
            int mar = 0, mbr = 0;
            double dis_r = sub_match(arr, n, dia, countof(dia), 3, 100, -1, 1, mar, mbr);
            //print_mask_arr(arr, y_vec.size(), mar);
            //print_mask_arr(dia, countof(dia), mbr);
            //printf("%lf\n", dis_r);
            if (dis_r > dis) {
              sign = -1;
              ma = mar;
              mb = mbr;
              y_vec = y_vec_r;
              for (int i = 0; i < n/2; ++i) {
                std::swap(lines[i], lines[n-1-i]);
              }
            }
          }
          vector<double> y_tmp;
          vector<RotatedRect> lines_tmp;
          vector<double> y_ok;
          vector<RotatedRect> lines_ok(countof(dia));
          Point2f pt(0, 0);
          Size2f sz(0, 0);
          for (int j = 0; j < y_vec.size(); ++j) {
            if (ma & (1 << j)) {
              lines_tmp.push_back(lines[j]);
              pt.x += lines[j].center.x;
              pt.y += lines[j].center.y;
              sz.height += lines[j].size.height;
              sz.width += lines[j].size.width;
              y_tmp.push_back(y_vec[j]);
            }
          }
          pt.x /= y_tmp.size();
          pt.y /= y_tmp.size();
          sz.height /= y_tmp.size();
          sz.width /= y_tmp.size();
          int k;
          k  = 0;
          for (int j = 0; j < countof(dia); ++j) {
            if (mb & (1 << j)) {
              y_ok.push_back(dia[j]);
            }
          }
          int n = y_ok.size();
          double suma = y_tmp[n - 1] - y_tmp[0];
          double sumb = y_ok[n - 1] - y_ok[0];

          k = 0;
          for (int j = 0; j < countof(dia); ++j) {
            if (mb & (1 << j)) {
              lines_ok[j] = (lines_tmp[k++]);
            }
            else {
              double y1 = sign*(((dia[j] - y_ok[0])*suma/ sumb) + y_tmp[0]);
              //double y1 = r1.center.x*s + r1.center.y*c;
              Point2f pt1 = pt;
              if (fabs(s) > fabs(c)) {
                pt1.x = (y1 - 0*c) / s;
              } else {
                pt1.y = (y1 - 0*s) / c;
              }
              Vec4f l = getLine(pt1, angle, 100);
              double u;
              Point2f pt2 = ptFootOfPerpendicular(pt, l, u);
              lines_ok[j] = RotatedRect(pt2, sz, angle);
            }
          }
          if (1) {
            //drawRotatedRects(color_edge, rp.lines_class, 1);
            drawRotatedRects(color_edge, lines_ok, 2);
            //drawRotatedRects(color_edge, wp.lines_bound, 2);
          }
        }
      }
    }
    if (0) {
      vector<Point > pts;
      for (int i = (int)bboxes1.size() - 1; i >= 0; i--) {
        const Rect& r = bboxes1[i];
        if (r.height<20 && r.width<20) {
          pts.push_back(Point(r.x + r.width / 2, r.y + r.height / 2));
        }
      }
      vector<Vec4i> lines;
      int nline = icvHoughLinesProbabilistic(pts.data(), pts.size(), srcImage.rows, srcImage.cols, 1, CV_PI / 180, 8, 30, 10, lines, 20);
      for (int i = 0; i <nline; ++i) {
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        double lw = (nline - i) * 1;
        lw = max(lw, 1.);
        lw = 1;
        Vec4i l = lines[i];
        line(color_edge, Point(l[0], l[1]), Point(l[2], l[3]), color, lw, 8);
      }
    }

    if (0) {
      vector<Point > pts;
      for (int i = (int)bboxes1.size() - 1; i >= 0; i--) {
        const Rect& r = bboxes1[i];
        if (r.height<20 && r.width<20) {
          pts.push_back(Point(r.x + r.width / 2, r.y + r.height / 2));
        }
      }
      Vec2f lines[20];
      int nline = HoughLinesByPoints(pts.data(), pts.size(), srcImage.rows, srcImage.cols, 1, CV_PI/180, 2, lines, countof(lines));
      for (int i = 0; i <nline; ++i) {
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        double lw = (nline - i)*1;
        lw = max(lw, 1.);
        lw = 1;
        drawLine(color_edge, lines[i], color, lw);
      }
    }

    if (0) {
      vector<Point > pts;
      vector<vector<Rect>> lines_class;
      int nclasses = rects_part(bboxes1, lines_class, 1);
      if (1) {
      }
    }
    if (0) {
      vector<Point > pts;
      vector<Vec4i> lines;
      for (int i = (int)bboxes1.size() - 1; i >= 0; i--) {
        const Rect& r = bboxes1[i];
        pts.push_back(Point(r.x + r.width / 2, r.y + r.height/ 2));
      }
      fitLines(bboxes1, lines, 20);
      vector<vector<Vec4i>> lines_class;
      if (0) {
        for (size_t i = 0; i < lines.size(); i++)
        {
          Vec4i l = lines[i];
          line(color_edge, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 1);
        }
      }
      int nclasses = lines_part(lines, lines_class, 5);
      if (1) {
        for (size_t j = 0; j < lines_class.size(); j++) {
          Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
          for (size_t i = 0; i < lines_class[j].size(); i++) {
            Vec4i l = lines_class[j][i];
            line(color_edge, Point(l[0], l[1]), Point(l[2], l[3]), color, 1);
          }
          if (0) {
            vector<vector<Vec4i>> lines_class2;
            int nclls = lines_part(lines_class[j], lines_class2, 3);
            printf("asdfasdf\n");
          }
        }
      }
    }
    if (0) {
      cv::cvtColor(mserMapMat, mserMapMat, CV_GRAY2BGR);
      for (int i = (int)bboxes1.size() - 1; i >= 0; i--) {
        const Rect& r = bboxes1[i];
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        rectangle(color_edge, r, color, 1, 8, 0);//用矩形画矩形窗  
      }
    }
    if (0) {
      vector<Vec4i> lines;
      HoughLinesP(mserMapMat, lines, 1, CV_PI / 180, 20, 10, 5);
      for (size_t i = 0; i < lines.size(); i++)
      {
        Vec4i l = lines[i];
        line(color_edge, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 1);
      }
    }

		if (1) {
			cv::imshow("srcImage", mergeImgs(10, 0, 600, srcImage, color_edge));
			int c = cv::waitKey(0);
      if (c == 'p'|| c == 'P') {
        iline = iline+ list.size() -1;
        iline %= list.size();
      }
      else {
        ++iline;
      }
		}
	}
	return 0;
}