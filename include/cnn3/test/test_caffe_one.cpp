
#include "caffe_config.h"
#include "wstd/logging.hpp"
#include "math_functions.cpp"
#include "common.cpp"
#include "proto/caffe_proto.cc"
#include "blob.hpp"
#include "blob.cpp"
#include "syncedmem.cpp"

using namespace caffe;

int test_caffe_one() {
  Blob<float> bb;

  bb.Reshape(1,2,3,4);
  return 0;
}
