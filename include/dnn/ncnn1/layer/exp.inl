

#include "exp.h"


namespace ncnn
{

DEFINE_LAYER_CREATOR(Exp)

Exp::Exp()
{
  one_blob_only = true;
  support_inplace = true;
}

int Exp::load_param(const ParamDict& pd)
{
  base = pd.get(0, -1.f);
  scale = pd.get(1, 1.f);
  shift = pd.get(2, 0.f);

  return 0;
}

int Exp::forward(const img_t* bottom_blob, img_t* top_blob) const
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
        outptr[i] = exp(shift + ptr[i] * scale);
      }
    }
  }
  else {
    #pragma omp parallel for

    for (int q = 0; q < channels; q++) {
      const float* ptr = pBox_channel_p(bottom_blob, q);
      float* outptr = pBox_channel_p(top_blob, q);

      for (int i = 0; i < size; i++) {
        outptr[i] = pow(base, (shift + ptr[i] * scale));
      }
    }
  }

  return 0;
}

int Exp::forward_inplace(img_t* bottom_top_blob) const
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
        ptr[i] = exp(shift + ptr[i] * scale);
      }
    }
  }
  else {
    #pragma omp parallel for

    for (int q = 0; q < channels; q++) {
      float* ptr = pBox_channel_p(bottom_top_blob, q);

      for (int i = 0; i < size; i++) {
        ptr[i] = pow(base, (shift + ptr[i] * scale));
      }
    }
  }

  return 0;
}

} // namespace ncnn
