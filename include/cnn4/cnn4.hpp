


#ifndef _CNN4_HPP_
#define _CNN4_HPP_


struct Net;
void set_debug_info(int level);
Net* net_new();
void net_del(Net* net);
int net_loadjson(Net* net, const char* fn);
int net_train(Net* net);
double net_forward(Net* net);
const float* net_output(Net* net, int idx, int* pcount);
int net_set_input_u8(Net* net, const unsigned char* img_data, const double* mean_values);

#include "stdc/types_c.h"
#include "utime.h"
#include "proto.h"
#include "solver.hpp"
#include "SGDSolver.hpp"
#include "insert_splits.hpp"

Net* net_new() {
  Net* net = new Net;
  return net;
}
void net_del(Net* net) {
  delete net;
}
int net_loadjson(Net* net, const char* fn) {
  CJSON* root = cJSON_OpenFile(fn);
  int ret = 0;
  if (root) {
    ret = net->FromProto(root);
    cJSON_Delete(root);
  }
  return ret;
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
int net_set_input_u8(Net* net, const unsigned char* img_data, const double* mean_values) {
  Blob* input_layer = net->input_blobs(0);
  if (input_layer) {
    int num_channels_ = input_layer->channels();
    CHECK(num_channels_ == 3 || num_channels_ == 1)
      << "Input layer should have 1 or 3 channels.";
    //input_geometry_ = cv::Size(input_layer->width(), input_layer->height());

    DataTransformerInfo di = { 0 };
    di.init();
    di.mean_values_ = mean_values;
    di.shape_type_ = NHWC;
    float* data = input_layer->cpu_mdata();
    blob_data_transform_T(&di, input_layer->shape_, data, img_data, 0, 0);
    return 1;
  }
  return 0;
}

#endif // _CNN4_HPP_
