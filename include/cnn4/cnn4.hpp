


#ifndef _CNN4_HPP_
#define _CNN4_HPP_

//#include "cnn4.h"

#include "std/types_c.h"
#include "utime.h"
#include "proto.h"
#include "insert_splits.hpp"

CnnNet* cnnnet_new() {
  CnnNet* net = new CnnNet;
  net->init();
  return net;
}
void cnnnet_del(CnnNet* net) {
  delete net;
}
int cnnnet_loadjson(CnnNet* net, const char* fn) {
  cjson* root = cjson_OpenFile(fn);
  int ret = 0;
  if (root) {
    ret = net->FromJson(root);
    cjson_Delete(root);
  }
  return ret;
}
int cnnnet_savejson(CnnNet* net, const char* fn) {
  cjson* root = net->ToJson();
  if (root) {
    cjson_SaveFile(fn, root);
    cjson_Delete(root);
  }
  return 1;
}
int cnnnet_train(CnnNet* net) {
  Solver* solver = new SGDSolver();
  solver->init(net);
  solver->Solve();
  delete solver;
  return 0;
}
double cnnnet_forward(CnnNet* net) {
  return net->Forward(TEST);
}
const float* cnnnet_output(CnnNet* net, int idx, int* pcount) {
  Blob* output_layer = net->output_blobs(0);
  const float* pred = output_layer->cpu_data();
  const int count = output_layer->count();
  *pcount = count;
  return pred;
}
//#include "std/color_c.h"
#include "std/imgopt.h"

int cnnnet_set_input_u8(CnnNet* net, const unsigned char* img_data, int h, int w, int c, double scale, const double* mean_values, int mean_values_size) {
  Blob* input_layer = net->input_blobs(0);
  CHECK(c == 3 || c == 1)
    << "Input layer should have 1 or 3 channels.";
  if (input_layer) {
    int dc = input_layer->channels();
    int dw = input_layer->width();
    int dh = input_layer->height();
    int ds = dw*dc;
    int ss = w*c;
    unsigned char* img_temp = 0;
    CHECK(dc == 3 || dc == 1)
      << "Input layer should have 1 or 3 channels.";
    if (c != dc) {
      CHECK(dc == c)
        << "Input layer should have 1 or 3 channels.";
      return 0;
    }
    if (dh != h || dw != w) {
      img_temp = (unsigned char*)malloc(dh*dw*dc);
      IMRESIZE(h, w, img_data, ss, c, dh, dw, img_temp, ds, c, INTER_BILINEARITY);
      img_data = img_temp;
      h = dh, w = dw;
    }
    //input_geometry_ = cv::Size(input_layer->width(), input_layer->height());

    DataShape shape = input_layer->shape_;
    shape.w = w;
    input_layer->Reshape(shape);

    DataTransformerInfo di = { 0 };
    di.init();
    di.mean_values_size = mean_values_size;
    for (int i = 0; i < mean_values_size; ++i) {
      di.mean_values_[i] = mean_values[i];
    }
    di.scale = scale;
    float* data = input_layer->cpu_mdata();
    blob_data_transform_T(&di, NHWC, input_layer->shape_, data, img_data, 0, 0);
    if (img_temp) free(img_temp);
    return 1;
  }
  return 0;
}

#endif // _CNN4_HPP_
