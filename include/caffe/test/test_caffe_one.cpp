
#include "caffe/caffe_config.h"
#include "wstd/logging.hpp"
#include "caffe/util/math_functions.cpp"
#include "caffe/common.cpp"
#include "caffe/proto/caffe.pb.cc"
#include "caffe/blob.hpp"
#include "caffe/blob.cpp"
#include "caffe/syncedmem.cpp"

using namespace caffe;

int test_caffe_one() {
  Blob<float> bb;

  bb.Reshape(1,2,3,4);
  return 0;
}
