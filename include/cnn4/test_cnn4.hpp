
#pragma warning(disable: 4267)
#pragma warning(disable: 4244)

#include "wstd/utime.hpp"
#include "wstd/string.hpp"
#include <direct.h>
#include "cnn4.h"
//#include "cnn4.hpp"

int test_cnn4() {
  const char* fn = NULL;
  const char* fn1 = NULL;
  //test_caffe2json();
  char buf[256];
  itoa_c(1234, buf, 10);
  if (0) {
    fn = "E:/caffe_train/mnist/lenet.json";
    fn1 = "E:/caffe_train/mnist/lenet_out.json";
    _chdir("E:/caffe_train/mnist/");
  }
  if (1) {
    fn = "E:/OCR_Line/chars/lenet.json";
    fn1 = "E:/OCR_Line/chars/lenet_out.json";
    _chdir("E:/OCR_Line/chars/");
  }
  if (0) {
    fn = "E:/OCR_Line/model/densenet-no-blstm/densenet-no-blstm.json";
    fn1 = "E:/OCR_Line/model/densenet-no-blstm/densenet-no-blstm_out.json";
    _chdir("E:/OCR_Line/model/densenet-no-blstm/");
  }

  Net* net = net_new();
  if (net_loadjson(net, fn)) {
    net_train(net);
  }
  net_savejson(net, fn1);
  net_del(net);
  return 0;
}
