
#pragma warning(disable: 4267)
#pragma warning(disable: 4244)

#include "wstd/utime.hpp"
#include "wstd/string.hpp"
#include <direct.h>
//#include "cnn4.hpp"
//#include "test/test_all.hpp"

struct Net;
Net* net_new();
void net_del(Net* net);
int net_loadjson(Net* net, const char* fn);
int net_train(Net* net);

int test_cnn4() {
  const char* fn = NULL;
  //test_caffe2json();
  char buf[256];
  itoa_c(1234, buf, 10);
  if (1) {
    fn = "C:/caffe_train/mnist/lenet.json";
    _chdir("C:/caffe_train/mnist/");
  }
  if (0) {
    fn = "E:/OCR_Line/model/densenet-no-blstm/densenet-no-blstm.json";
    _chdir("E:/OCR_Line/model/densenet-no-blstm/");
  }

  Net* net = net_new();
  if (net_loadjson(net, fn)) {
    net_train(net);
  }
  net_del(net);
  return 0;
}
