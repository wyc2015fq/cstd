

#include "threshold.h"

namespace ncnn
{

DEFINE_LAYER_CREATOR(Threshold)

Threshold::Threshold()
{
  one_blob_only = true;
  support_inplace = true;
}

int Threshold::load_param(const ParamDict& pd)
{
  threshold = pd.get(0, 0.f);

  return 0;
}

int Threshold::forward(const img_t* bottom_blob, img_t* top_blob) const
{
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = pBox_channels(bottom_blob);
  int size = w * h;

  top_blob->create(w, h, channels);

  if (top_blob->empty()) {
    return -100;
  }

  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    const float* ptr = pBox_channel_p(bottom_blob, q);
    float* outptr = pBox_channel_p(top_blob, q);

    for (int i = 0; i < size; i++) {
      outptr[i] = ptr[i] > threshold ? 1.f : 0.f;
    }
  }

  return 0;
}

int Threshold::forward_inplace(img_t* bottom_top_blob) const
{
  int w = bottom_top_blob->w;
  int h = bottom_top_blob->h;
  int channels = pBox_channels(bottom_top_blob);
  int size = w * h;

  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    float* ptr = pBox_channel_p(bottom_top_blob, q);

    for (int i = 0; i < size; i++) {
      ptr[i] = ptr[i] > threshold ? 1.f : 0.f;
    }
  }

  return 0;
}

} // namespace ncnn
