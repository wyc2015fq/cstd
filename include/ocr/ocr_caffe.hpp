
#ifndef __OCR_CAFFE_HPP__
#define __OCR_CAFFE_HPP__

#include "caffe/classification/ICNNPredict.h"
//#include "classification/libClassification.hpp"
//#pragma comment (lib,"libClassification.lib")

string GetPredictString(const vector<float>& fm, int idxBlank, const vector<string>& labels)
{
  string str;
  for (size_t t = 0; t < fm.size(); t++)
  {
    int idx = t;
    int label = (int)fm[idx] + 0.5f;
    if (label >= 0 && label != idxBlank)
    {
      str += labels[label];
    }
  }
  return str;
}


struct ocr_caffe {
  ICNNPredict* pCNN;
  int wstd = 0, hstd = 0;
  vector<string> alphabets;
  int idxBlank = 0;
  ocr_caffe() {
    string imgfolder = "D:/OCR_Line/lines/han200w/imgtest/";
    const char* model_folder;
    model_folder = "D:/OCR_Line/lines/han200w/densenet-no-blstm_caffe4/";
    model_folder = "E:/OCR_Line/model/resnet-res-blstm/";
    model_folder = "E:/OCR_Line/model/inception-bn-res-blstm/";
    model_folder = "D:/OCR_Line/lines/han200w/densenet-sum-blstm-full-res-blstm/";
    pCNN = CreatePredictInstance(model_folder, true);
    pCNN->GetInputImageSize(wstd, hstd);
    alphabets  = pCNN->GetLabels();
    vector<string>::const_iterator it = find(alphabets.begin(), alphabets.end(), "blank");
    if (it != alphabets.end())
      idxBlank = (int)(it - alphabets.begin());
  }
  string run(const Mat& im) {
    Mat img = im;
    int w = img.cols, h = img.rows;
    int w1 = hstd*w / h;
    if (w1 != w && h != hstd)
      cv::resize(img, img, cv::Size(w1, hstd));

    cvtColor(img, img, CV_BGR2GRAY);
    //cvtColor(img, img, CV_GRAY2BGR);

    //imshow("asdfasdf", img); waitKey(-1);

    vector<int> shape;
    vector<float> pred = pCNN->GetOutputFeatureMap(img, shape);

    string strpredict0 = GetPredictString(pred, idxBlank, alphabets);
    return strpredict0;
  }
};

ocr_caffe ocr_caffe1;

#endif // __OCR_CAFFE_HPP__
