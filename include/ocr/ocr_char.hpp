
#ifndef _OCR_CHAR_HPP_
#define _OCR_CHAR_HPP_

#include "cnn4/cnn4.h"
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

struct ocr_char_recog {
  CnnNet* net;
  int loadok;
  ocr_char_recog() {
    net = cnnnet_new();
    const char* modelfn = NULL;
#ifdef _DEBUG
    modelfn = "E:/OCR_Line/chars/char_28_28_1.json";
#else
    modelfn = "./model/char_28_28_1.json";
#endif

    loadok = cnnnet_loadjson(net, modelfn);
  }
  ~ocr_char_recog() {
    cnnnet_del(net);
  }
  int run(const uchar* data, int h, int w, int c) {
    cnnnet_set_input_u8(net, data, h, w, c, 0.00390625, NULL, 0);
    cnnnet_forward(net);
    int count = 0;
    const float* out = cnnnet_output(net, 0, &count);
    int out_id = argmax(count, out);
    return out_id;
  }
};

struct ocr_hans_recog {
  CnnNet* net;
  int loadok;
  ocr_hans_recog() {
    net = cnnnet_new();
    const char* modelfn = NULL;
#ifdef _DEBUG
    modelfn = "E:/OCR_Line/hans/han_28_28_1.json";
#else
    modelfn = "./model/han_28_28_1.json";
#endif

    loadok = cnnnet_loadjson(net, modelfn);
  }
  ~ocr_hans_recog() {
    cnnnet_del(net);
  }
  int run(const uchar* data, int h, int w, int c) {
    cnnnet_set_input_u8(net, data, h, w, c, 0.00390625, NULL, 0);
    cnnnet_forward(net);
    int count = 0;
    const float* out = cnnnet_output(net, 0, &count);
    int out_id = argmax(count, out);
    return out_id;
  }
};



#endif // _OCR_CHAR_HPP_
