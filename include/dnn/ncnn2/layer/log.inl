

#include "log.h"
#include <math.h>

namespace ncnn
{

DEFINE_LAYER_CREATOR(Log)

Log::Log()
{
  one_blob_only = true;
  support_inplace = true;
}

int Log::load_param(const ParamDict& pd)
{
  base = pd.get(0, -1.f);
  scale = pd.get(1, 1.f);
  shift = pd.get(2, 0.f);

  return 0;
}

int Log::forward(const img_t* bottom_blob, img_t* top_blob) const
{
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = pBox_channels(bottom_blob);
  int size = w * h;

  top_blob->create(w, h, channels);

  if (top_blob->empty()) {
    return -100;
  }

  if (base == -1.f) {
    #pragma omp parallel for

    for (int q = 0; q < channels; q++) {
      const float* ptr = pBox_channel_p(bottom_blob, q);
      float* outptr = pBox_channel_p(top_blob, q);

      for (int i = 0; i < size; i++) {
        outptr[i] = log(shift + ptr[i] * scale);
      }
    }
  }
  else {
    float log_base_inv = 1.f / log(base);

    #pragma omp parallel for

    for (int q = 0; q < channels; q++) {
      const float* ptr = pBox_channel_p(bottom_blob, q);
      float* outptr = pBox_channel_p(top_blob, q);

      for (int i = 0; i < size; i++) {
        outptr[i] = log(shift + ptr[i] * scale) * log_base_inv;
      }
    }
  }

  return 0;
}

int Log::forward_inplace(img_t* bottom_top_blob) const
{
  int w = bottom_top_blob->w;
  int h = bottom_top_blob->h;
  int channels = pBox_channels(bottom_top_blob);
  int size = w * h;

  if (base == -1.f) {
    #pragma omp parallel for

    for (int q = 0; q < channels; q++) {
      float* ptr = pBox_channel_p(bottom_top_blob, q);

      for (int i = 0; i < size; i++) {
        ptr[i] = log(shift + ptr[i] * scale);
      }
    }
  }
  else {
    float log_base_inv = 1.f / log(base);

    #pragma omp parallel for

    for (int q = 0; q < channels; q++) {
      float* ptr = pBox_channel_p(bottom_top_blob, q);

      for (int i = 0; i < size; i++) {
        ptr[i] = log(shift + ptr[i] * scale) * log_base_inv;
      }
    }
  }

  return 0;
}

} // namespace ncnn
