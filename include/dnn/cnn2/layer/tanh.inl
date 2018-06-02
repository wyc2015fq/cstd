

#include "tanh.h"


namespace ncnn
{

DEFINE_LAYER_CREATOR(TanH)

TanH::TanH()
{
  one_blob_only = true;
  support_inplace = true;
}

int TanH::forward(const Blob* bottom_blob, Blob* top_blob) const
{
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = Blob_channels(bottom_blob);
  int size = w * h;

  top_blob->create(w, h, channels);

  if (top_blob->empty()) {
    return -100;
  }

  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    const float* ptr = Blob_channel_p(bottom_blob, q);
    float* outptr = Blob_channel_p(top_blob, q);

    for (int i = 0; i < size; i++) {
      outptr[i] = tanh(ptr[i]);
    }
  }

  return 0;
}

int TanH::forward_inplace(Blob* bottom_top_blob) const
{
  int w = bottom_top_blob->w;
  int h = bottom_top_blob->h;
  int channels = Blob_channels(bottom_top_blob);
  int size = w * h;

  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    float* ptr = Blob_channel_p(bottom_top_blob, q);

    for (int i = 0; i < size; i++) {
      ptr[i] = tanh(ptr[i]);
    }
  }

  return 0;
}

} // namespace ncnn
