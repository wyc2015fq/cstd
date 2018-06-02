

#include "scale.h"

namespace ncnn
{

DEFINE_LAYER_CREATOR(Scale)

Scale::Scale()
{
  one_blob_only = true;
  support_inplace = true;
}

Scale::~Scale()
{
}

int Scale::load_param(const ParamDict& pd)
{
  scale_data_size = pd.get(0, 0);
  bias_term = pd.get(1, 0);

  return 0;
}

#if NCNN_STDIO
int Scale::load_model(FILE* binfp)
{
  int nread;

  scale_data.create(scale_data_size);
  nread = fread(scale_data, scale_data_size * sizeof(float), 1, binfp);

  if (nread != 1) {
    fprintf(stderr, "Scale read scale_data failed %d\n", nread);
    return -1;
  }

  if (bias_term) {
    bias_data.create(scale_data_size);

    if (bias_data.empty()) {
      return -100;
    }

    nread = fread(bias_data, scale_data_size * sizeof(float), 1, binfp);

    if (nread != 1) {
      fprintf(stderr, "Scale read bias_data failed %d\n", nread);
      return -1;
    }
  }

  return 0;
}
#endif // NCNN_STDIO

int Scale::load_model(const unsigned char*& mem)
{
  scale_data = img_t(scale_data_size, (float*)mem);
  mem += scale_data_size * sizeof(float);

  if (bias_term) {
    bias_data = img_t(scale_data_size, (float*)mem);
    mem += scale_data_size * sizeof(float);
  }

  return 0;
}

int Scale::forward(const img_t* bottom_blob, img_t* top_blob) const
{
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = pBox_channels(bottom_blob);
  int size = w * h;

  top_blob->create(w, h, channels);

  if (top_blob->empty()) {
    return -100;
  }

  if (bias_term) {
    const float* scale_ptr = scale_data;
    const float* bias_ptr = bias_data;
    #pragma omp parallel for

    for (int q = 0; q < channels; q++) {
      const float* ptr = pBox_channel_p(bottom_blob, q);
      float* outptr = pBox_channel_p(top_blob, q);

      float s = scale_ptr[q];
      float bias = bias_ptr[q];

      for (int i = 0; i < size; i++) {
        outptr[i] = ptr[i] * s + bias;
      }
    }
  }
  else {
    const float* scale_ptr = scale_data;
    #pragma omp parallel for

    for (int q = 0; q < channels; q++) {
      const float* ptr = pBox_channel_p(bottom_blob, q);
      float* outptr = pBox_channel_p(top_blob, q);

      float s = scale_ptr[q];

      for (int i = 0; i < size; i++) {
        outptr[i] = ptr[i] * s;
      }
    }
  }

  return 0;
}

int Scale::forward_inplace(img_t* bottom_top_blob) const
{
  int w = bottom_top_blob->w;
  int h = bottom_top_blob->h;
  int channels = pBox_channels(bottom_top_blob);
  int size = w * h;

  if (bias_term) {
    const float* scale_ptr = scale_data;
    const float* bias_ptr = bias_data;
    #pragma omp parallel for

    for (int q = 0; q < channels; q++) {
      float* ptr = pBox_channel_p(bottom_top_blob, q);

      float s = scale_ptr[q];
      float bias = bias_ptr[q];

      for (int i = 0; i < size; i++) {
        ptr[i] = ptr[i] * s + bias;
      }
    }
  }
  else {
    const float* scale_ptr = scale_data;
    #pragma omp parallel for

    for (int q = 0; q < channels; q++) {
      float* ptr = pBox_channel_p(bottom_top_blob, q);

      float s = scale_ptr[q];

      for (int i = 0; i < size; i++) {
        ptr[i] *= s;
      }
    }
  }

  return 0;
}

} // namespace ncnn
