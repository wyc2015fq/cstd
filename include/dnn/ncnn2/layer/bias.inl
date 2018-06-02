

#include "bias.h"

namespace ncnn
{

DEFINE_LAYER_CREATOR(Bias)

Bias::Bias()
{
  one_blob_only = true;
  support_inplace = true;
}

Bias::~Bias()
{
}

int Bias::load_param(const ParamDict& pd)
{
  bias_data_size = pd.get(0, 0);

  return 0;
}

#if NCNN_STDIO
int Bias::load_model(FILE* binfp)
{
  int nread;

  bias_data.create(bias_data_size);

  if (bias_data.empty()) {
    return -100;
  }

  nread = fread(bias_data, bias_data_size * sizeof(float), 1, binfp);

  if (nread != 1) {
    fprintf(stderr, "Bias read bias_data failed %d\n", nread);
    return -1;
  }

  return 0;
}
#endif // NCNN_STDIO

int Bias::load_model(const unsigned char*& mem)
{
  bias_data = img_t(bias_data_size, (float*)mem);
  mem += bias_data_size * sizeof(float);

  return 0;
}

int Bias::forward(const img_t* bottom_blob, img_t* top_blob) const
{
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = pBox_channels(bottom_blob);
  int size = w * h;

  top_blob->create(w, h, channels);

  if (top_blob->empty()) {
    return -100;
  }

  const float* bias_ptr = bias_data;
  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    const float* ptr = pBox_channel_p(bottom_blob, q);
    float* outptr = pBox_channel_p(top_blob, q);

    float bias = bias_ptr[q];

    for (int i = 0; i < size; i++) {
      outptr[i] = ptr[i] + bias;
    }
  }

  return 0;
}

int Bias::forward_inplace(img_t* bottom_top_blob) const
{
  int w = bottom_top_blob->w;
  int h = bottom_top_blob->h;
  int channels = pBox_channels(bottom_top_blob);
  int size = w * h;

  const float* bias_ptr = bias_data;
  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    float* ptr = pBox_channel_p(bottom_top_blob, q);

    float bias = bias_ptr[q];

    for (int i = 0; i < size; i++) {
      ptr[i] += bias;
    }
  }

  return 0;
}

} // namespace ncnn
