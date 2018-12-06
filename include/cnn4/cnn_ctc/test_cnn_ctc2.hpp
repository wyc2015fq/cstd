

#include "cnn4/cnn4.hpp"

//typedef unsigned short ushort;

#ifdef _WIN32
void GetPredictString2(char* str, IRANGE* r, const char* const* labels, const float* fm, int size, int idxBlank) {
  for (int i = 0; i < size; i++)
  {
    int label = (int)(fm[i] + 0.5f);
    if (label >= 0 && label != idxBlank) {
      const char* s1 = labels[label];
      size_t l1 = strlen(s1);
      strcat_c(str, r, s1, l1);
    }
  }
}

int ocr_run(char* str, int maxstrlen, Net* net_, const double* mean_values, const uchar* img_data, const char*const * labels) {
  Blob* input_layer = net_->input_blobs(0);
  int num_channels_ = input_layer->channels();
  CHECK(num_channels_ == 3 || num_channels_ == 1)
    << "Input layer should have 1 or 3 channels.";
  //input_geometry_ = cv::Size(input_layer->width(), input_layer->height());

  DataTransformerInfo di = { 0 };
  di.init();
  di.mean_values_ = mean_values;
  di.shape_type_ = NHWC;
  float* data = input_layer->cpu_mdata();
  blob_data_transform_T(&di, input_layer->shape_, data, img_data, 0, 0);

  //uutime a;
  net_->Forward(TEST);
  //LOG(INFO) << a.elapsed();
  Blob* output_layer = net_->output_blobs(0);
  const float* pred = output_layer->cpu_data();
  const int count = output_layer->count();
  //outshape = output_layer->shape_vec();
  int idxBlank = 0;
  IRANGE r = iRANGE(0, maxstrlen);
  GetPredictString2(str, &r, labels, pred, count, idxBlank);
  str[r.s] = 0;
  return r.s;
}
char* labels[] = {
  //" "
#include "label1.txt"
};

#endif

static uchar img_data[] = {
#include "test_cnn_ctc.hpp_img.txt"
};

void GetPredictString2_w(wchar_t* str, IRANGE* r, const wchar_t* labels, const float* fm, int size, int idxBlank) {
  for (int i = 0; i < size; i++)
  {
    int label = (int)(fm[i] + 0.5f);
    if (label >= 0 && label != idxBlank) {
      str[r->s++] = labels[label];
      printf("%d\n", labels[label]);
    }
  }
}

int ocr_run_w(wchar_t* str, int maxstrlen, Net* net_, const double* mean_values, const uchar* img_data, const wchar_t* labels_w) {
  Blob* input_layer = net_->input_blobs(0);
  int num_channels_ = input_layer->channels();
  CHECK(num_channels_ == 3 || num_channels_ == 1)
    << "Input layer should have 1 or 3 channels.";
  //input_geometry_ = cv::Size(input_layer->width(), input_layer->height());

  DataTransformerInfo di = { 0 };
  di.init();
  di.mean_values_ = mean_values;
  di.shape_type_ = NHWC;
  float* data = input_layer->cpu_mdata();
  blob_data_transform_T(&di, input_layer->shape_, data, img_data, 0, 0);

  //uutime a;
  utime_start(a);
  net_->Forward(TEST);
  double t = utime_elapsed(a);  printf("time : %lf", t);
  //LOG(INFO) << a.elapsed();
  Blob* output_layer = net_->output_blobs(0);
  const float* pred = output_layer->cpu_data();
  const int count = output_layer->count();
  //outshape = output_layer->shape_vec();
  int idxBlank = 0;
  IRANGE r = iRANGE(0, maxstrlen);
  GetPredictString2_w(str, &r, labels_w, pred, count, idxBlank);
  str[r.s] = 0;
  return r.s;
}

wchar_t labels_w[] = {
#include "test_cnn_ctc.hpp_labels.txt"
};

int test_ocr_static() {
#ifdef _WIN32
#define MODEL_PATH "E:/OCR_Line/model/densenet-no-blstm/"
#else
#define MODEL_PATH "/data/local/tmp/"
#endif
  const char* model_file = MODEL_PATH  "model.json";
  double mean_values[] = { 152,152,152 };
  printf("test_ocr_static\n");
  Net* net_ = NULL;
  /* Load the network. */
  net_ = new Net;
  net_->FromJsonFile(model_file);
  CHECK_EQ(net_->num_inputs(), 1) << "Network should have exactly one input.";
  CHECK_EQ(net_->num_outputs(), 1) << "Network should have exactly one output.";
  wchar_t str[256] = { 0 };
  int n = ocr_run_w(str, 256, net_, mean_values, img_data, labels_w);
  //wprintf(L"%s\n", str); //Êä³ö wts £¨×Ö·û´®£©
#if 0
#endif
  printf("end\n");
  return 0;
}
