
#include "ocr/classification/test_ocr_classification.cpp"
#include "ocr/test_ocr_detect.cpp"
#include "wstd/string.hpp"

Rect Rect_bound(Rect rect, int w, int h) {
  rect.x = BOUND(rect.x, 0, w);
  rect.y = BOUND(rect.y, 0, h);
  rect.width = BOUND(rect.width, 0, w - rect.x);
  rect.height= BOUND(rect.height, 0, h - rect.y);
  return rect;
}

Rect Rect_ex(Rect rect, int k) {
  rect.x -= k;
  rect.y -= k;
  rect.width += 2 * k;
  rect.height += 2 * k;
  return rect;
}

int on_pushButton_2_clicked1(const cv::Mat& imgSrc, ocr_classification* ocr, bool is_seq)
{
  Mat srcImage = resetsize(imgSrc);
  //Mat imgSrc_test = imread("E:/PracticeOfQt/Id_recognition/id_cardBai.jpg" ,1); //用于测试，实际应用只需将下面替换成imgSrc
  Mat imgRplane = getRplane(srcImage); //获得原始图像R分量

  vector <Rect>  rects1;
  vector<vector<Rect>>  rectvec;
  //Rect rc;
  //posDetect0(srcImage, imgRplane, rc);
  //return;
  //srcImage = srcImage(rc);
  imgRplane = getRplane(srcImage);
  posDetect(srcImage, imgRplane, rects1);  //获得身份证号码区域
  partitionLine(rects1, rectvec);
  for (int k = 0; k < rectvec.size(); ++k) {
    vector <Rect>&  rects = rectvec[k];
    for (int j = 0; j < rects.size(); ++j) {
      //normalPosArea(imgRplane, rects[j], outputMat); //获得身份证号码字符矩阵
      //if (k == 2 && j == 1)
      {
        Rect rect = rects[j];
        printf("%2d行%2d列(%3d %3d)", k, j, rect.y, rect.x);
        vector<Rect> char_mat;  //获得切割得的字符矩阵
        std::vector<float> pred;
        std::vector<int> shape;
        string s;
        if (is_seq) {
          rect = Rect_ex(rect, rect.height*0.2);
          rect = Rect_bound(rect, imgRplane.cols, imgRplane.rows);
          Mat outputMat = imgRplane(rect);
          s = ocr->ocr(outputMat, pred, shape, is_seq);
          cv::rectangle(srcImage, rect, cv::Scalar(255, 0, 0), 2);
        }
        else {
          Mat outputMat = imgRplane(rect);
          char_segment(outputMat, char_mat);
          for (int i = 0; i < char_mat.size(); ++i) {
            Rect r0 = char_mat[i];
            r0.x += rect.x;
            r0.y += rect.y - 1;
            r0.height += 2;
            int d = r0.height - r0.width;
            if (d > 0) {
              r0.x -= d / 2;
              r0.width = r0.height;
            }
            s += ocr->ocr(srcImage(r0), pred, shape, is_seq);
          }
          cv::rectangle(srcImage, rect, cv::Scalar(255, 0, 0), 2);
          for (int i = 0; i < char_mat.size(); ++i) {
            char buf[256];
            Rect r0 = char_mat[i];
            r0.x += rect.x;
            r0.y += rect.y;
            snprintf(buf, 256, "%d", i);
            cv::rectangle(srcImage, r0, cv::Scalar(0, 0, 255), 1);
            //printf("%d %d\n", char_mat[i].width(), char_mat[i].height());
            //imshow("char", srcImage(r0)); waitKey(-1);            cv::destroyWindow("char");
          }
        }
        printf("%s\n", s.c_str());
      }
    }
    //cv::waitKey(-1);
  }
  imshow("srcImage", srcImage);
  cv::waitKey(-1);
  //getAnnXML();  //该函数只需执行一次，获得训练矩阵和标签矩阵，保存于xml文件中
  return 0;
}
int test_ocr_caffe()
{
  string modelfolder = "E:/OCR_Line/model/sss/";
  double scale = 0.00390625;
  bool is_seq = false;
  bool usegpu = false;
  if (0) {
    cv::Mat imgSrc;
    std::vector<string> filenames;
    LoadTextFileList("D:/code/pudn/ocr/AstarLight/CPS-OCR-Engine/ocr/test.txt", filenames);
    ocr_classification* ocr = new ocr_classification(modelfolder, scale, usegpu);
    std::vector<float> pred;
    std::vector<int> shape;
    string s;
    int i;
    for (i = 0; i < filenames.size(); ++i) {
      imgSrc = cv::imread(filenames[i], 1);  //使用打开的文件对话框获得的路径，由imread读出
      s = ocr->ocr(imgSrc, pred, shape, is_seq);
      printf("%s\n", s.c_str());
    }
    return 0;
  }
  if (0) {
    cv::Mat imgSrc;
    const char* ch = "D:/code/pudn/ocr/Id_recognition/id_card9.jpg";
    ch = "E:/OCR_Line/demo_images/012.jpg";
    imgSrc = cv::imread(ch, 1);  //使用打开的文件对话框获得的路径，由imread读出
    on_pushButton_2_clicked(imgSrc);
    //cv::imshow("asdf", imgSrc);
    cv::waitKey(-1);
  }
  if (1) {
    modelfolder = "E:/OCR_Line/model/densenet-no-blstm/";
    scale = 0;
    is_seq = true;
    usegpu = true;
  }
//#define CPU_ONLY
#ifdef CPU_ONLY
  usegpu = false;
#endif
  if (0) {
    std::vector<string> filenames;
    LoadTextFileList("E:/OCR_Line/std/list.txt", filenames);
    int k;
    ocr_classification* ocr = new ocr_classification(modelfolder, scale, usegpu);
    for (k = 15; k < filenames.size(); ++k) {
      string fn = filenames[k];
      printf("%s\n", fn.c_str());
      cv::Mat srcImage = cv::imread(fn, IMREAD_COLOR);
      if (srcImage.empty()) {
        continue;
      }
      if (srcImage.cols > 600) {
        double t = 600.*1. / srcImage.cols;
        cv::resize(srcImage, srcImage, cv::Size(), t, t, cv::INTER_LANCZOS4);
      }
      on_pushButton_2_clicked1(srcImage, ocr, is_seq);
      //cv::imshow("asdf", imgSrc);
      //cv::waitKey(-1);
    }
  }
  if (1) {
    ocr_classification* ocr = new ocr_classification(modelfolder, scale, usegpu);
    FILE* pf = fopen("E:/data/ew_id/t1_0911.txt", "rb");
    char buf[256];
    string root_path = "E:/data/ew_id/we-loan.oss-cn-shanghai.aliyuncs.com/";
    for (; fgets(buf, 256, pf); ) {
      vector<string> strvec;
      string line = wstd::trim(buf, "\r\n");
      wstd::split(strvec, line, "\t");
      string fn = root_path + strvec[1];
      printf("%s\n", fn.c_str());
      cv::Mat srcImage = cv::imread(fn, IMREAD_COLOR);
      if (srcImage.cols < 210) {
        continue;
      }
      if (srcImage.empty()) {
        continue;
      }
      if (srcImage.cols > 600) {
        double t = 600.*1. / srcImage.cols;
        cv::resize(srcImage, srcImage, cv::Size(), t, t, cv::INTER_LANCZOS4);
      }
      on_pushButton_2_clicked1(srcImage, ocr, is_seq);
      //cv::imshow("asdf", imgSrc);
      //cv::waitKey(-1);
    }
  }
	return 0;
}
