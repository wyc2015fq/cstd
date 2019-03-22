
#ifndef _OCR_CTC_INL_
#define _OCR_CTC_INL_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utime.h"
#include "wstd/string.hpp"
//typedef unsigned short ushort;
//#include "caffe/caffe.h"

//#include "caffe/libcaffe.cpp"
#include "caffe/caffe.hpp"
//#include "ocr/dnn/caffe/pycaffe/caffe/_caffe.cpp"

#include <map>
#include "public.h"
#undef min
#undef max
#include "ICNNPredict.h"


#include "bktree.inl"
#include "levenshtein.inl"
#include "classification.inl"

#include <time.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <set>
using namespace std;

#include "caffe/ctcpp.h"


static uchar img_data[] = {
#include "test_cnn_ctc.hpp_img.txt"
};

wchar_t labels_w[] = {
#include "test_cnn_ctc.hpp_labels.txt"
};

int GetPredictString1_w(wchar_t* str, const int* labels, int size, int idxBlank) {
  int s = 0;
  for (int i = 0; i < size; i++)
  {
    int label = labels[i];
    if (label >= 0 && label != 0) {
      str[s++] = labels_w[label];
    }
  }
  return s;
}

int GetPredictString2_w(wchar_t* str, const float* fm, int size, int idxBlank) {
  int s = 0;
  for (int i = 0; i < size; i++)
  {
    int label = (int)(fm[i] + 0.5f);
    if (label >= 0 && label != 0) {
      str[s++] = labels_w[label];
    }
  }
  return s;
}

int ocr_run_w(wchar_t* str, int maxstrlen, Classifier* net, const double* mean_values, const uchar* img_data, int w, const wchar_t* labels_w) {
  cv::Mat img(cv::Size(w, 32), CV_8UC3);
  memcpy(img.data, img_data, w*32*3);
  std::vector<int> outshape;
  std::vector<float> pred = net->GetOutputFeatureMap(img, outshape);
  int ret = GetPredictString2_w(str, pred.data(), pred.size(), -1);
  str[ret] = 0;
  return ret;
}

struct ocr_net {
  Classifier* net;
  ocr_net() {
    net = NULL;
    net = new Classifier();
  }
  ~ocr_net() {
    delete net;
  }
  int loadjson(const char* model_file) {
    //const char* model_file = MODEL_PATH  "model.json";
#ifdef _DEBUG
#define ROOTPATH "E:\\OCR_Line\\"
#else
#define ROOTPATH "./"
#endif
    string modelfolder = ROOTPATH"model/densenet-no-blstm/";
    net->Init(modelfolder, true);
    return net != NULL;
  }
  int run(wchar_t* wstr, int maxlen, const uchar* img_data, int w) {
    double mean_values[] = { 152,152,152 };
    int n = ocr_run_w(wstr, maxlen, net, mean_values, img_data, w, labels_w);
    return n;
  }
};

#endif // _OCR_CTC_INL_
