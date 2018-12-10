
#include "ocr_ctc.inl"
//#include "cnn4/cnn4.hpp"

int test_ocr_static() {
#ifdef _WIN32
#define MODEL_PATH "E:/OCR_Line/model/densenet-no-blstm/"
#else
#define MODEL_PATH "/data/local/tmp/"
#endif
  const char* model_file = MODEL_PATH  "model.json";
  double mean_values[] = { 152,152,152 };
  printf("test_ocr_static\n");
  Net* net = NULL;
  /* Load the network. */
  net = net_new();
  if (net_loadjson(net, model_file)) {
    //CHECK_EQ(net->num_inputs(), 1) << "Network should have exactly one input.";
    //CHECK_EQ(net->num_outputs(), 1) << "Network should have exactly one output.";
    wchar_t str[256] = { 0 };
    int n = ocr_run_w(str, 256, net, mean_values, img_data, labels_w);
    //wprintf(L"%s\n", str); //Êä³ö wts £¨×Ö·û´®£©
  }
#if 0
#endif
  printf("end\n");
  return 0;
}
