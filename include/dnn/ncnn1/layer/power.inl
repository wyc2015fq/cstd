

#include "power.h"


namespace ncnn
{

DEFINE_LAYER_CREATOR(Power)

Power::Power()
{
  one_blob_only = true;
  support_inplace = true;
}

int Power::load_param(const ParamDict& pd)
{
  power = pd.get(0, 1.f);
  scale = pd.get(1, 1.f);
  shift = pd.get(2, 0.f);

  return 0;
}

int Power::forward(const img_t* bottom_blob, img_t* top_blob) const
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
      outptr[i] = pow((shift + ptr[i] * scale), power);
    }
  }

  return 0;
}

int Power::forward_inplace(img_t* bottom_top_blob) const
{
  int w = bottom_top_blob->w;
  int h = bottom_top_blob->h;
  int channels = pBox_channels(bottom_top_blob);
  int size = w * h;

  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    float* ptr = pBox_channel_p(bottom_top_blob, q);

    for (int i = 0; i < size; i++) {
      ptr[i] = pow((shift + ptr[i] * scale), power);
    }
  }

  return 0;
}

} // namespace ncnn
