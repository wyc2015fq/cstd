
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

int ocr_run(char* str, int maxstrlen, Net* net, const double* mean_values, const uchar* img_data, int w, const char*const * labels) {
  net_set_input_u8(net, img_data, w, mean_values);
  //uutime a;
  net_forward(net);
  //LOG(INFO) << a.elapsed();
  int count = 0;
  const float* pred = net_output(net, 0, &count);
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

int ocr_run_w(wchar_t* str, int maxstrlen, Net* net, const double* mean_values, const uchar* img_data, int w, const wchar_t* labels_w) {
  net_set_input_u8(net, img_data, w, mean_values);
  //uutime a;
  utime_start(a);
  net_forward(net);
  printf("time : %lf\n", utime_elapsed(a));
  //LOG(INFO) << a.elapsed();
  int count = 0;
  const float* pred = net_output(net, 0, &count);
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
  Net* net;
  ocr_net() {
    net = NULL;
    net = net_new();
  }
  ~ocr_net() {
    net_del(net);
  }
  int loadjson(const char* model_file) {
    //const char* model_file = MODEL_PATH  "model.json";
    return net_loadjson(net, model_file);
  }
  int run(wchar_t* wstr, int maxlen, const uchar* img_data, int w) {
    double mean_values[] = { 152,152,152 };
    int n = ocr_run_w(wstr, maxlen, net, mean_values, img_data, w, labels_w);
    return n;
  }
};