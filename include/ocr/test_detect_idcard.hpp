

#include "detect_idcard.hpp"

struct ocr_detect {
  FaceDetector face_etector;
  cv::Mat srcImage, color_edge, gray, dst;

  void run(const cv::Mat& src) {
    vector<Mat> vecdst;
    int aa_angle = 10;
    //imRotation(src, src, iline &1 ? aa_angle :-aa_angle);
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
      RotatedRect r = face_rect_to_card_rect(face_rect, 0.1, 1.2, 1.2);
      if (Rect(0, 0, gray.cols, gray.rows).contains(r.center)) {
        Mat dst = getSubImage(src, r, Size(300, 200));
        if (get_angle.run(dst)) {
          int iangle = get_angle.iangle;
          r.angle += iangle;
          face_rect.angle += iangle;
          double tt = 70;
          double a = 0.1;
          double b = 1.1;
          //double scale, double y_off
          if (get_angle.centor_dis < tt) {
            a *= pow(tt / get_angle.centor_dis, 1);
          }
          else {
            //b *= pow(get_angle.centor_dis*1. / tt, 0.5);
            double mm = 200;
            if (get_angle.max_dis < mm) {
              //b *= 1.2;
            }
          }
          double dd = 250;
          if (get_angle.max_dis > 200) {
            b *= pow(get_angle.max_dis / dd, 0.5);
          }
          if (get_angle.centor_dis > 90) {
            b *= 1.2;
          }
          if (get_angle.max_dis < 150 && get_angle.centor_dis<60) {
            //b *= 60./ get_angle.centor_dis;
          }
          double bot_maxx = MAX(get_angle.botton_line[2], get_angle.botton_line[0]);
          double bot_minx = MIN(get_angle.botton_line[2], get_angle.botton_line[0]);
          if (bot_maxx < 200 && get_angle.max_dis<200 && get_angle.botton_line[0]) {
            //b *= 1.5;
          }
          r = face_rect_to_card_rect(face_rect, a, b, b);
          Mat dst_ok = getSubImage(src, r, Size(300, 200));
          get_rect_t gr;
          int ret = gr.run(dst_ok);
          if (!ret && 0) {
            b *= 1.2;
            r = face_rect_to_card_rect(face_rect, a, b, b);
            dst_ok = getSubImage(src, r, Size(300, 200));
            //imshow("dst_ok", dst_ok); waitKey(0);
            ret = gr.run(dst_ok);
          }
          //if (ret)
          {
            vecdst.push_back(get_angle.color_edge.clone());
            //imshow("color_edge1", get_angle.color_edge);
            vecdst.push_back(gr.color_edge);
            vecdst.push_back(dst_ok);
            if (0) {
              string fn = wstd::format("C:\\outpic\\dst_ok_%03d_%d.jpg", iline, i);
              cv::imwrite(fn, dst_ok);
            }
            Scalar color = Scalar(0, 0, 255);
            drawRotatedRects(dst_ok, gr.lines_ok, 2, &color);
            double s = 0;
            for (int i = 0; i < gr.lines_ok.size(); ++i) {
              s += gr.lines_ok[i].center.x;
              s += gr.lines_ok[i].center.y;
              s += gr.lines_ok[i].size.width;
              s += gr.lines_ok[i].size.height;
              s += gr.lines_ok[i].angle;
            }
            printf("lines_ok avg = %lf\n", s);
            //drawDetectLines(dst_ok, gr.lines, 4);
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
    dst  = mergeImgs(03, 0, 600, vecdst);
  }
};

int test_detect_idcard()
{
  GaussianBlur;
  //return test_hsv_bin();
  //return test_myfindContours();
  //int k;
  RNG rng(12345);
  Mat::convertTo;
  Rect r;
  r.center;
  /// ¼ÓÔØÔ´Í¼Ïñ
  string im_dir = "E:/data/ew_id/we-loan.oss-cn-shanghai.aliyuncs.com/";
  string fn = im_dir + "upload/backend/20160927235243069-2951.jpg";
  vector<string> list;
  wstd::readlines("E:/data/ew_id/list1.txt", list, 2000);
#ifdef _DEBUG
  iline = 1182;
  iline = 1192;
  iline = 1222;
  iline = 1229;
  iline = 113;
  iline = 113;
#endif
  int c = 0;
  int n = list.size();
  n = min(iline+300, n);
  ocr_detect od;
  srand(11);
  for (; iline < n; ) {
    vector<string> strs;
    wstd::split(strs, list[iline], " ");
    string fn = im_dir + strs[0];
    cv::Mat src;
    src = cv::imread(fn, 1);
    printf("%d %s %d %d\n", iline, wstd::path_split_filename(fn.c_str()).c_str(), src.rows, src.cols);
    if (src.empty()) {
      proc_key(c, list.size(), iline);
      continue;
    }

    od.run(src);
#ifdef _DEBUG
    isdebug = 1;
#endif
    if (1) {
      string fn = wstd::format("C:\\outpic\\dst_ok\\src_%03d_z.jpg", iline);
      cv::imwrite(fn, src);
    }
    if (isdebug) {
      cv::imshow("vecdst", od.dst);
      c  = cv::waitKey(0);
      proc_key(c, list.size(), iline);
    }
    else {
      string fn = wstd::format("C:\\outpic\\out_%03d_z.jpg", iline);
      cv::imwrite(fn, od.dst);
      ++iline;
    }
  }
  return 0;
}