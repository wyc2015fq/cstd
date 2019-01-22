
#pragma warning(disable: 4267)
#pragma warning(disable: 4244)

#include "wstd/utime.hpp"
#include "wstd/string.hpp"
#include <direct.h>
#include "cnn4.h"
//#include "cnn4.hpp"

int test_cnn4(int argn, char* argc[]) {
  const char* fn = "train.json";
  const char* fn1 = "train_out.json";
  //test_caffe2json();
#ifdef _DEBUG
  if (0) {
    fn = "E:/OCR_Line/model/densenet-no-blstm/densenet-no-blstm.json";
    fn1 = "E:/OCR_Line/model/densenet-no-blstm/densenet-no-blstm_out.json";
    _chdir("E:/OCR_Line/model/densenet-no-blstm/");
  }
  _chdir("E:/OCR_Line/hans/20_50_500");
  _chdir("E:/OCR_Line/chars/20_50_500");
  _chdir("E:/caffe_train/mnist/20_50_500bn");
  _chdir("E:/OCR_Line/lines/densenet-no-blstm");
  _chdir("E:/OCR_Line/mnist/20_50_500");
#endif

  CnnNet* net = cnnnet_new();
  if (cnnnet_loadjson(net, fn)) {
    cnnnet_train(net);
  }
  cnnnet_savejson(net, fn1);
  cnnnet_del(net);
  return 0;
}

#include "wstd/filesystem.hpp"
#include <vector>
#include <string>
#include "imgio/imgio.h"

using namespace std;
using namespace wstd;

static int argmax(int count, const float* arr) {
  if (count > 0) {
    int i, j = 0;
    for (i = 1; i < count; ++i) {
      if (arr[i] > arr[j]) {
        j = i;
      }
    }
    return j;
  }
  return -1;
}


#include "ocr/hanz_charset.h"
#include "std/iconv_c.h"
#define DEF(a, b, c, d, e, f)   L##a L##c L##e L##f
static const wchar_t hanz_charset[] = HANZ_CHARSET_DEF(DEF);
#undef DEF

int test_cnn4_recog(int argn, char* argc[]) {
  const char* modelfn = NULL;
  const char* testfn = NULL;
#ifdef _DEBUG
  if (1) {
    modelfn = "E:/OCR_Line/chars/char_28_28_1.json";
    testfn = "E:/OCR_Line/chars/test.txt";
    _chdir("E:/OCR_Line/chars/");
  }
  if (1) {
    modelfn = "E:/OCR_Line/hans/han_28_28_1.json";
    testfn = "E:/OCR_Line/hans/test.txt";
    _chdir("E:/OCR_Line/hans/");
  }
  if (1) {
    modelfn = "train-val.json";
    testfn = "test.txt";
    _chdir("E:/OCR_Line/lines");
  }
  if (1) {
    modelfn = "deploy.json";
    testfn = "../test.txt";
    _chdir("D:/OCR_Line/lines/han200w/densenet-no-blstm_caffe5");
}
#else
#endif // _DEBUG
  CnnNet* net = cnnnet_new();
  if (!cnnnet_loadjson(net, modelfn)) {
    cnnnet_del(net);
    return 0;
  }
  vector<string> files;
  readlines(testfn, files);
  img_t im[1] = { 0 };
  int err_cnt = 0;
  int all_cnt = 0;
  for (int i = 0; i < files.size(); ++i) {
    vector<string> strs;
    split(strs, files[i], ";");
    if (strs.size() != 2) {
      printf("error(strs.size() != 2) : %s\n", files[i].c_str());
      continue;
    }
    const char* imgfn = strs[0].c_str();
    int id = atoi_c(strs[1].c_str());
    if (imread(imgfn, 1, 1, im)) {
      cnnnet_set_input_u8(net, im->data, im->h, im->w, im->c, 0.00390625, NULL, 0);
      cnnnet_forward(net);
      int count=0;
      const float* out = cnnnet_output(net, 0, &count);
      int out_id = argmax(count, out);
      err_cnt += out_id != id;
      ++all_cnt;
      if (1) {
        wchar_t hans[10] = { 0 };
        hans[0] = hanz_charset[out_id];
        char buf[20];
        int len = iconv_c(ICONV_UCS2LE, ICONV_GB2312, (char*)hans, 2, buf, 20);
        buf[len] = 0;
        printf("%s ", buf);
      }

      printf("%05d %6.3lf %s\n", all_cnt, 100.*err_cnt / all_cnt, imgfn);
    }
  }
  cnnnet_del(net);
  return 0;
}
