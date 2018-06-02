

#include "flatten.h"

namespace ncnn
{

DEFINE_LAYER_CREATOR(Flatten)

Flatten::Flatten()
{
  one_blob_only = true;
  support_inplace = false;
}

int Flatten::forward(const Blob* bottom_blob, Blob* top_blob) const
{
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = Blob_channels(bottom_blob);
  int size = w * h;

  top_blob->create(size * channels);

  if (top_blob->empty()) {
    return -100;
  }

  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    const float* ptr = Blob_channel_p(bottom_blob, q);
    float* outptr = top_blob->data + size * q;

    for (int i = 0; i < size; i++) {
      outptr[i] = ptr[i];
    }
  }

  return 0;
}

} // namespace ncnn
