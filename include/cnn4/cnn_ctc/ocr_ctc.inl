
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utime.h"
#include "wstd/string.hpp"

//typedef unsigned short ushort;

#include "cnn4/cnn4.h"

#ifdef _WIN32
void GetPredictString2(char* str, IRANGE* r, const char* const* labels, const float* fm, int size, int idxBlank) {
  for (int i = 0; i < size; i++)
  {
    int label = (int)(fm[i] + 0.5f);
    if (label >= 0 && label != idxBlank) {
      const char* s1 = labels[label];
      size_t l1 = strlen(s1);
      strcat_c(str, r, s1, l1);
    }
  }
}

int ocr_run(char* str, int maxstrlen, CnnNet* net, const double* mean_values, int mean_values_size,
  const uchar* img_data, int h, int w, int c, const char*const * labels) {
  cnnnet_set_input_u8(net, img_data, h, w, c, 1, mean_values, mean_values_size);
  //uutime a;
  cnnnet_forward(net);
  //LOG(INFO) << a.elapsed();
  int count = 0;
  const float* pred = cnnnet_output(net, 0, &count);
  //outshape = output_layer->shape_vec();
  int idxBlank = 0;
  IRANGE r = iRANGE(0, maxstrlen);
  GetPredictString2(str, &r, labels, pred, count, idxBlank);
  str[r.s] = 0;
  return r.s;
}
char* labels[] = {
  //" "
#include "label1.txt"
};

#endif

static uchar img_data[] = {
#include "test_cnn_ctc.hpp_img.txt"
};

void GetPredictString2_w(wchar_t* str, IRANGE* r, const wchar_t* labels, const float* fm, int size, int idxBlank) {
  for (int i = 0; i < size; i++)
  {
    int label = (int)(fm[i] + 0.5f);
    if (label >= 0 && label != idxBlank) {
      str[r->s++] = labels[label];
      //printf("%d\n", labels[label]);
    }
  }
}

int ocr_run_w(wchar_t* str, int maxstrlen, CnnNet* net, const double* mean_values, int mean_values_size,
  const uchar* img_data, int h, int w, int c, const wchar_t* labels_w) {
  cnnnet_set_input_u8(net, img_data, h, w, c, 1, mean_values, mean_values_size);
  //uutime a;
  utime_start(a);
  cnnnet_forward(net);
  printf("time : %lf\n", utime_elapsed(a));
  //LOG(INFO) << a.elapsed();
  int count = 0;
  const float* pred = cnnnet_output(net, 0, &count);
  //outshape = output_layer->shape_vec();
  int idxBlank = 0;
  IRANGE r = iRANGE(0, maxstrlen);
  GetPredictString2_w(str, &r, labels_w, pred, count, idxBlank);
  str[r.s] = 0;
  return r.s;
}

wchar_t labels_w[] = {
#include "test_cnn_ctc.hpp_labels.txt"
};

struct ocr_net {
  CnnNet* net;
  ocr_net() {
    net = NULL;
    net = cnnnet_new();
  }
  ~ocr_net() {
    cnnnet_del(net);
  }
  int loadjson(const char* model_file) {
    //const char* model_file = MODEL_PATH  "model.json";
    return cnnnet_loadjson(net, model_file);
  }
  int run(wchar_t* wstr, int maxlen, const uchar* img_data, int h, int w, int c) {
    double mean_values[] = { 152,152,152 };
    int n = ocr_run_w(wstr, maxlen, net, mean_values, 3, img_data, h, w, c, labels_w);
    return n;
  }
};