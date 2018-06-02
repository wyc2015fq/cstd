
#include "mat_pixel.inl"

int net_run_2(struct Net* net, const char* input_name, const char* output_name, const img_t* im, const float* mean_vals, const float* norm_vals, int topk, int* id, float* cls_scores) {
  int input = find_or_insert_blob(net, input_name, 0);
  int output = find_or_insert_blob(net, output_name, 0);
  if (input>=0 && output>=0) {
    Blob* inputBlob = BLOB(input);
    Blob* outputBlob = BLOB(output);
    float* inputData = setData(inputBlob);
    Layer* layer = net->layers;
    int h = IARG(input_h);
    int w = IARG(input_w);
    int c = IARG(input_c);
    ASSERT(3==c || 1==c);
    pBoxsetsize(inputBlob, h, w, c, im->f);
    float* in = pBox_datap(inputBlob);
    from_pixels_resize(im->data, 3==c ? PIXEL_BGR : PIXEL_GRAY, im->cols, im->rows, w, h, in);
    ssubstract_mean_normalize(c, h, w, in, w*c, mean_vals, norm_vals);
    MNARG(phase) = M_Test;
    setLogRedirect(NULL);
    net_forward(net);
    const float* outputData = getData(outputBlob);
    int i, labelSize = Data_3DSize(outputBlob->size);
    for (i=0; i<outputBlob->size.n; ++i) {
      getTopK(outputData+i*labelSize, labelSize, topk, id, cls_scores);
      id += topk;
      cls_scores += topk;
    }
    return 1;
  }
  ASSERT(0);
  return 0;
}

static int detect_squeezenet(const img_t* im, int topk, int* id, float* cls_scores)
{
  static struct Net squeezenet[1] = {0};
  if (0==squeezenet->l_size) {
#if 0
    int input, output;
    output = build_age_net(age_net, &input);
    net_set_param(age_net, age_64_model, countof(age_64_model));
#else
    sys_chdir("E:/code/cstd/include1/dnn/ncnn/squeezenet");
    net_load(squeezenet, "squeezenet_v1.1.txt", "squeezenet_v1.1.dat");
#endif
  }

  //ncnn::Mat in = ncnn::Mat::from_pixels_resize(bgr.data, ncnn::Mat::PIXEL_BGR, bgr.cols, bgr.rows, 227, 227);

  const float mean_vals[3] = {104.f, 117.f, 123.f};
  int ret = net_run_2(squeezenet, "data", "prob", im, mean_vals, NULL, topk, id, cls_scores);
  //in.substract_mean_normalize(mean_vals, 0);

#if 0
  ncnn::Extractor ex = squeezenet.create_extractor();
  ex.set_light_mode(true);

  ex.input("data", in);
  ncnn::Mat out;
  ex.extract("prob", out);
  for (int j = 0; j < out.c; j++) {
    const float* prob = out.data + out.cstep * j;
    cls_scores[j] = prob[0];
  }

#endif
  return ret;
}

#include "cfile.h"

int test_squeezenet(int argc, char** argv)
{
  if (0) {
    sys_chdir("C:/code/cnn/gender");
    Net net[1] = {0};
    net_load(net, "gender_std_model.txt", "gender_std_param.dat_5.dat");
    net_print(net, stdout, 0);
    net_free(net);
  }
  //test_partial_sort();
  const char* imagepath = argv[1];
  sys_chdir("E:/code/cstd/include1/dnn/ncnn/squeezenet");
  imagepath = "cat.jpg";
  Net net[1] = {0};
  net_load(net, "squeezenet_v1.1.txt", "squeezenet_v1.1.dat");
  //net_print(net, stdout, 0);
  //net_save(net, "squeezenet_v1.1_.txt", NULL, -1);
  img_t im[1] = {0};
  if (imread(imagepath, 3, 1, im)) {
    float score[3];
    int id[3];
//{utime_start(_start_time);
    detect_squeezenet(im, 3, id, score);
//printf("%f\n", utime_elapsed(_start_time));}
    for (int i = 0; i < 3; i++) {
      printf("%d = %f\n", id[i], score[i]);
    }
  }
  net_free(net);
  return 0;
}
