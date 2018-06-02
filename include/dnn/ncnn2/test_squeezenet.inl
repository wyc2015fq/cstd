
#include "net.inl"

#if 0

static int detect_squeezenet(const img_t* bgr, float* cls_scores)
{
  ncnn::Net squeezenet;
  squeezenet.load_param("squeezenet_v1.1.param");
  squeezenet.load_model("squeezenet_v1.1.bin");

  ncnn::Mat in = ncnn::Mat::from_pixels_resize(bgr.data, ncnn::Mat::PIXEL_BGR, bgr.cols, bgr.rows, 227, 227);

  const float mean_vals[3] = {104.f, 117.f, 123.f};
  in.substract_mean_normalize(mean_vals, 0);

  ncnn::Extractor ex = squeezenet.create_extractor();
  ex.set_light_mode(true);

  ex.input("data", in);
  ncnn::Mat out;
  ex.extract("prob", out);

  for (int j = 0; j < out.c; j++) {
    const float* prob = out.data + out.cstep * j;
    cls_scores[j] = prob[0];
  }

  return out.c;
}

static int print_topk(const std::vector<float>& cls_scores, int topk)
{
  // partial sort topk with index
  int size = cls_scores.size();
  std::vector< std::pair<float, int> > vec;
  vec.resize(size);

  for (int i = 0; i < size; i++) {
    vec[i] = std::make_pair(cls_scores[i], i);
  }

  std::partial_sort(vec.begin(), vec.begin() + topk, vec.end(),
      std::greater< std::pair<float, int> >());

  // print topk and score
  for (int i = 0; i < topk; i++) {
    float score = vec[i].first;
    int index = vec[i].second;
    fprintf(stderr, "%d = %f\n", index, score);
  }

  return 0;
}

#endif

#include "cfile.h"

int test_squeezenet(int argc, char** argv)
{
  //test_partial_sort();
  const char* imagepath = argv[1];
  sys_chdir("E:/code/cstd/include/facelib/ncnn/squeezenet");
  imagepath = "cat.jpg";
  Net net[1] = {0};
  net_load_param(net, fopen("squeezenet_v1.1.param", "rb"));
#if 0
  img_t im[1] = {0};
  if (imread(imagepath, 3, 1, im)) {
    std::vector<float> cls_scores;
    detect_squeezenet(m, cls_scores);
    print_topk(cls_scores, 3);
  }
#endif
  return 0;
}
