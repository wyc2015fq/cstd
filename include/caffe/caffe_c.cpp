

#include "caffe.h"
#include "caffe.hpp"
//#include "opencv2/opencv2.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


struct Net;
void set_debug_info(int level) {
}
void net_del(Net* net) {
  delete (caffe::Net<float>*)net;
  return;
}
Net* net_loadjson(const char* fn) {
#define ROOTPATH "E:\\OCR_Line\\"
  string model_path = ROOTPATH"model\\densenet-no-blstm\\";
  const string trained_file = model_path + "/model.caffemodel";
  const string model_file = model_path + "/deploy.prototxt";
  caffe::Net<float>* net_ = new caffe::Net<float>(model_file.c_str(), caffe::TEST);
  net_->CopyTrainedLayersFrom(trained_file);
  return (Net*)(net_);
}


int net_train(Net* net) { return 0; }
double net_forward(Net* net) { return 0; }
const float* net_output(Net* net, int idx, int* pcount) { return 0; }
int net_set_input_u8(Net* net, const unsigned char* img_data, int w, const double* mean_values) { return 0; }
