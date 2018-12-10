
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utime.h"
#include "wstd/string.hpp"

//typedef unsigned short ushort;

struct Net;
Net* net_new();
void net_del(Net* net);
int net_loadjson(Net* net, const char* fn);
int net_train(Net* net);
int net_forward(Net* net);
const float* net_output(Net* net, int idx, int* pcount);
int net_set_input_u8(Net* net, const unsigned char* img_data, const double* mean_values);

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

int ocr_run(char* str, int maxstrlen, Net* net, const double* mean_values, const uchar* img_data, const char*const * labels) {
  net_set_input_u8(net, img_data, mean_values);
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
      printf("%d\n", labels[label]);
    }
  }
}

int ocr_run_w(wchar_t* str, int maxstrlen, Net* net, const double* mean_values, const uchar* img_data, const wchar_t* labels_w) {
  net_set_input_u8(net, img_data, mean_values);
  //uutime a;
  utime_start(a);
  net_forward(net);
  double t = utime_elapsed(a);  printf("time : %lf", t);
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