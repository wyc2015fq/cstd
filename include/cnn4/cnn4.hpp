


#ifndef _CNN4_HPP_
#define _CNN4_HPP_

#include "cnn4.h"

#include "std/types_c.h"
#include "utime.h"
#include "proto.h"
#include "insert_splits.hpp"

Net* net_new() {
  Net* net = new Net;
  return net;
}
void net_del(Net* net) {
  delete net;
}
int net_loadjson(Net* net, const char* fn) {
  cjson* root = cjson_OpenFile(fn);
  int ret = 0;
  if (root) {
    ret = net->FromJson(root);
    cjson_Delete(root);
  }
  return ret;
}
int net_savejson(Net* net, const char* fn) {
  cjson* root = net->ToJson();
  if (root) {
    cjson_SaveFile(fn, root);
    cjson_Delete(root);
  }
  return 1;
}
int net_train(Net* net) {
  Solver* solver = new SGDSolver();
  solver->init(net);
  solver->Solve();
  delete solver;
  return 0;
}
double net_forward(Net* net) {
  return net->Forward(TEST);
}
const float* net_output(Net* net, int idx, int* pcount) {
  Blob* output_layer = net->output_blobs(0);
  const float* pred = output_layer->cpu_data();
  const int count = output_layer->count();
  *pcount = count;
  return pred;
}
int net_set_input_u8(Net* net, const unsigned char* img_data, int w, const double* mean_values, int mean_values_size) {
  Blob* input_layer = net->input_blobs(0);
  if (input_layer) {
    int num_channels_ = input_layer->channels();
    CHECK(num_channels_ == 3 || num_channels_ == 1)
      << "Input layer should have 1 or 3 channels.";
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
    di.shape_type_ = NHWC;
    float* data = input_layer->cpu_mdata();
    blob_data_transform_T(&di, input_layer->shape_, data, img_data, 0, 0);
    return 1;
  }
  return 0;
}

#endif // _CNN4_HPP_
