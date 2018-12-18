
#include "ocr_ctc.inl"
#ifdef _WIN32
#define MODEL_PATH "E:/OCR_Line/model/densenet-no-blstm/"
#else
#define MODEL_PATH "/data/local/tmp/"
#endif
//#include "cnn4/cnn4.hpp"

int test_ocr_static1() {
  const char* model_file = MODEL_PATH  "model.json";
  double mean_values[] = { 152,152,152 };
  printf("test_ocr_static\n");
  Net* net = NULL;
  // Load the network. 
  net = net_loadjson(model_file);
  if (net) {
    //CHECK_EQ(net->num_inputs(), 1) << "Network should have exactly one input.";
    //CHECK_EQ(net->num_outputs(), 1) << "Network should have exactly one output.";
    wchar_t wstr[256] = { 0 };
    char str[256] = { 0 };
#ifdef _DEBUG
#endif
    //set_debug_info(2);
    int n = ocr_run_w(wstr, 256, net, mean_values, img_data, 280, labels_w);
    setlocale(LC_ALL, "zh_CN.UTF-8");
    wprintf(L":%s\n", wstr); //输出 wts （字符串）
  }
  net_del(net);
  printf("end\n");
  return 0;
}

int test_cnn_ctc2() {
  const char* model_file = MODEL_PATH  "model.json";
  ocr_net ocr;
  printf("test_ocr_static\n");
  // Load the network. 
  if (ocr.loadjson(model_file)) {
    //CHECK_EQ(net->num_inputs(), 1) << "Network should have exactly one input.";
    //CHECK_EQ(net->num_outputs(), 1) << "Network should have exactly one output.";
    wchar_t wstr[256] = { 0 };
    char str[256] = { 0 };
    //set_debug_info(2);
    int n = ocr.run(wstr, 256, img_data, 280);
    setlocale(LC_ALL, "zh_CN.UTF-8");
    wprintf(L":%s\n", wstr); //输出 wts （字符串）
  }
  printf("end\n");
  return 0;
}
