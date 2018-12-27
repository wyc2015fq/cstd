


#include "std/time_c.h"
#include "utime.h"
#include "detect_idcard.hpp"
#include "parser\cJSON.hpp"
#include "idcard.h"

struct idcard {
  char gender[32];
  char name[32];
  char number[32];
  char birthday[32];
  char race[32];
  char address[128];
  char side[32];
  int type;
};

#include "std/iconv_c.h"



struct ocr_segchar {
  cv::Ptr<cv::MSER> mesr1;
  Mat color_edge;
  ocr_segchar() {
    mesr1 = cv::MSER::create(2, 50, 200, 0.2, 0.3);
  }

  int run(const Mat& src) {
    Mat gray;
    Mat bin;
    std::vector<cv::Rect> bboxes1;
    std::vector<std::vector<cv::Point> > regContours;
    cv::cvtColor(src, gray, CV_BGR2GRAY);
    mesr1->detectRegions(gray, regContours, bboxes1);
    cv::Mat mserMapMat = cv::Mat::zeros(src.size(), CV_8UC1);
    color_edge = src.clone();

    for (int i = (int)bboxes1.size() - 1; i >= 0; i--) {
      // 根据检测区域点生成mser+结果
      const std::vector<cv::Point>& p = regContours[i];
      Rect rc = bboxes1[i];
      if (1) {
        //RotatedRect rectPoint = minAreaRect(p);
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        //cv::rectangle(color_edge, bboxes1[i], color, 1);
      }
    }

    std::vector<vector<cv::Rect> > rrects2;
    if (bboxes1.size() > 10) {
      vector<int> labels;
      vector<int> cnt;
      int nclasses = cv::partition(bboxes1, labels, [](const cv::Rect& r1, const cv::Rect& r2) {
        double hd = 0.7;
        double a1 = r1.x;
        double b1 = r1.x + r1.width;
        double a2 = r2.x;
        double b2 = r2.x + r2.width;
        double l = MAX(a1, a2);
        double r = MIN(b1, b2);
        double d = r - l;
        double u = r1.width + r2.width - d;
        double iou = d/u;
        return iou>hd;
      });
      cnt.assign(nclasses, 0);
      for (int i = 0; i < labels.size(); ++i) {
        int j = labels[i];
        ++cnt[j];
      }
      std::vector<int> index = argsort(cnt);
      //nclasses = min(nclasses, 15);
      for (int i = 0; i < nclasses; ++i) {
        int idx = index[i];
        int c = cnt[idx];
        if (c > 1) {
          vector<Rect> rrects;
          for (int j = 0; j < bboxes1.size(); ++j) {
            if (labels[j] == idx) {
              rrects.push_back(bboxes1[j]);
            }
          }
          rrects2.push_back(rrects);
        }
      }
      nclasses = rrects2.size();
      for (int i = 0; i < nclasses; ++i) {
        RotatedRect r = minAreaRect(rrects2[i]);
        r = curr(r);
        //lines_rect.push_back(r);
        //cv::rectangle(color_edge, bboxes1[i], color, 1);
        //Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        Scalar color = Scalar(255, 0, 0);
        drawRotatedRect(color_edge, r, color, 1);
      }
    }
    //threshold(gray, bin, 0, 255, CV_THRESH_OTSU);
    imshow("color_edge", color_edge); waitKey(-1);
    //imshow("color_edge", color_edge); 
    //imshow("bin", bin);
    //waitKey(-1);
    return 0;
  }

};

#if 0
struct oct_tess {
  tesseract::TessBaseAPI tess;
  oct_tess() {
    tess.Init(NULL, "eng", tesseract::OEM_DEFAULT);
    tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
  }
  int run(const Mat& im)
  {
    cv::Mat gray;
    cv::cvtColor(im, gray, CV_BGR2GRAY);
    // ...other image pre-processing here...

    // Pass it to Tesseract API
    tess.SetImage((uchar*)gray.data, gray.cols, gray.rows, 1, gray.cols);

    // Get the text
    char* out = tess.GetUTF8Text();
    printf("out = %s\n", out);
    return 0;
  }
};
#endif

#ifndef _OCR_CTC_INL_
struct ocr_net {
  int loadjson(const char* fn) { return 0; }
  int run(wchar_t* wstr, int len, const uchar* data, int w) { return 0; }
};
#endif

struct ocr_idcard_reg {
  ocr_net ocr;
  ocr_segchar seg;
  //oct_tess tess;
  int init() {
    // Load the network. 
    if (ocr.loadjson(0)) {
      //CHECK_EQ(net->num_inputs(), 1) << "Network should have exactly one input.";
      //CHECK_EQ(net->num_outputs(), 1) << "Network should have exactly one output.";
      //set_debug_info(2);
      return 1;
    }
    return 0;
  }
  int run(const Mat& mat, const RotatedRect* r, int n, idcard* out) {
    int i, j=0;
    Mat im;
    Mat im2;
    double ss = 1.9;
    //imshow("mat", mat);
    memset(out, 0, sizeof(*out));
    for (i = 0; i < n; ++i) {
      RotatedRect r2 = r[i];
      r2.size.width += 20;
      r2.size.height *= ss;
      im  = getSubImage(mat, r2);
      int w = (im.cols)*32/im.rows;
      w += 3;
      w &= ~3;
      resize(im, im2, Size(w, 32));
      if (1) {
        seg.run(im2);
      }
      //imshow("adsfasdf", im); waitKey(-1);
      wchar_t wstr[256] = { 0 };
      char str[256] = { 0 };
      int n=0;
      //seg.run(im2);
      //imshow("im2", im2);  waitKey(-1);
      n = ocr.run(wstr, 256, im2.data, w);
      //n = ocr.run(wstr, 256, img_data, 280);
      //tess.run(im2);
      int len = iconv_c("UCS-2LE", "gb2312", (char*)wstr, n*2, str, 256);
      str[len] = 0;
      printf("%s\n", str);
      if (strlen(str) > 0) {
        out->type = 1;
        strncpy(out->side, "front", 32);
        switch (j)
        {
        case 0:
          strncpy(out->number, str, 32);
          break;
        case 1:
          strncpy(out->name, str, 16);
          break;
        case 2:
          strncpy(out->gender, str, 32);
          break;
        case 3:
          strncpy(out->birthday, str, 32);
          break;
        default:
          strcat(out->address, str);
          break;
        }
        ++j;
      }
    }
    //waitKey(-1);
    return 0;
  }
};

struct ocr_detect {
  FaceDetector face_etector;
  cv::Mat srcImage, color_edge, gray, dst;
  ocr_idcard_reg ocr;
  get_angle_t get_angle;
  vector<idcard> vid;
  int time_used;
  ocr_detect() {
    ocr.init();
  }
  std::string jsonstr;
  idcard getidcard(int index) {
    idcard id = vid[index];
    char* number = id.number;
    int i, j = 0;
    for (i = 0, j=0; id.number[i]; ++i) {
      char c = number[i];
      if (('0' <= c && c <= '9') || 'x' == c || 'X' == c) {
        number[j++] = c;
      }
    }
    number[j] = 0;
    char* name = id.name;
    char* p;
    if (p = strstr(name, "名")) {
      strcpy(name, p+2);
    }
    return id;
  }

  std::string getjson(const cv::Mat& src) {
    const char* nowstr = timenow();
    utime_start(a);
    run(src);
    time_used = 1000*utime_elapsed(a);
    cJSON * root = cJSON_CreateObject();
    cJSON * cards = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "cards", cards);
    for (int i = 0; i < vid.size(); ++i) {
      cJSON * card = cJSON_CreateObject();
      idcard id = getidcard(i);
      cJSON_AddItemToObject(card, "gender", cJSON_CreateString(id.gender));
      cJSON_AddItemToObject(card, "name", cJSON_CreateString(id.gender));
      cJSON_AddItemToObject(card, "id_card_number", cJSON_CreateString(id.number));
      cJSON_AddItemToObject(card, "birthday", cJSON_CreateString(id.birthday));
      cJSON_AddItemToObject(card, "race", cJSON_CreateString(id.race));
      cJSON_AddItemToObject(card, "address", cJSON_CreateString(id.address));
      cJSON_AddItemToObject(card, "type", cJSON_CreateNumber(id.type));
      cJSON_AddItemToObject(card, "side", cJSON_CreateString(id.side));
      cards->AddItemToArray(card);
    }
    cJSON_AddItemToObject(root, "time_used", cJSON_CreateNumber(time_used));
    cJSON_AddItemToObject(root, "request_id", cJSON_CreateString(nowstr));
    jsonstr = cJSON_Print(root);
    printf("%s\n", jsonstr.c_str());
    cJSON_Delete(root);
    return jsonstr;
  }
  int run(const cv::Mat& src) {
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
    vid.clear();
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
            //drawRotatedRects(dst_ok, gr.lines_ok, 2, &color);
            //ocr.run();
            if (0) {
              double s = 0;
              for (int i = 0; i < gr.lines_ok.size(); ++i) {
                s += gr.lines_ok[i].center.x;
                s += gr.lines_ok[i].center.y;
                s += gr.lines_ok[i].size.width;
                s += gr.lines_ok[i].size.height;
                s += gr.lines_ok[i].angle;
              }
              printf("lines_ok avg = %lf\n", s);
            }
            Mat dst_ok2 = getSubImage(src, r, Size(300*2, 200*2));
            vector<RotatedRect> rr_ok;
            for (int i = 0; i < gr.lines_ok.size(); ++i) {
              rr_ok.push_back(scale(gr.lines_ok[i], 2, 2));
            }
            idcard out;
            ocr.run(dst_ok2, rr_ok.data(), rr_ok.size(), &out);
            vid.push_back(out);
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
    return 0;
  }
};


int test_detect_idcard()
{
  //return test_hsv_bin();
  //return test_myfindContours();
  //int k;
  RNG rng(12345);
  //Mat::convertTo;
  Rect r;
  //r.center;
  /// 加载源图像
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
    //if (!strstr(fn.c_str(), "20161005101521422-4375")) { continue; }
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