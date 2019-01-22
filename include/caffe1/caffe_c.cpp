

#include "caffe.h"
#include "caffe.hpp"
//#include "opencv2/opencv2.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#if 0
struct caffeNet {};

void set_debug_info(int level) {
}
void net_del(caffeNet* net) {
  delete (caffe::Net<float>*)net;
  return;
}
caffeNet* net_loadjson(const char* fn) {
#define ROOTPATH "E:\\OCR_Line\\"
  string model_path = ROOTPATH"model\\densenet-no-blstm\\";
  const string trained_file = model_path + "/model.caffemodel";
  const string model_file = model_path + "/deploy.prototxt";
  caffe::Net<float>* net_ = new caffe::Net<float>(model_file.c_str(), caffe::TEST);
  net_->CopyTrainedLayersFrom(trained_file);
  return (caffeNet*)(net_);
}


int net_train(caffeNet* net) { return 0; }
double net_forward(caffeNet* net) { return 0; }
const float* net_output(caffeNet* net, int idx, int* pcount) { return 0; }
int net_set_input_u8(caffeNet* net, const unsigned char* img_data, int w, const double* mean_values) { return 0; }

#endif

