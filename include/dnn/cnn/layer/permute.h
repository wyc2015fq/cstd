

#ifndef LAYER_PERMUTE_H
#define LAYER_PERMUTE_H

#include "layer.h"

namespace ncnn
{

struct Permute : public Layer
{
public:
  Permute();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const Blob* prev, Blob* next) const;

public:
  int order_type;
};

} // namespace ncnn

#endif // LAYER_PERMUTE_H

namespace ncnn
{

DEFINE_LAYER_CREATOR(Permute)

Permute::Permute()
{
  one_blob_only = true;
  support_inplace = false;
}

int Permute::load_param(const ParamDict& pd)
{
  order_type = pd.get(0, 0);

  return 0;
}

int Permute::forward(const Blob* prev, Blob* next) const
{
  int w = prevSize.w;
  int h = prevSize.h;
  int channels = prevSize.c;
  int size = w * h;

  // order_type
  // 0 = w h c
  // 1 = h w c
  // 2 = w c h
  // 3 = c w h
  // 4 = h c w
  // 5 = c h w

  if (order_type == 0) {
    next = prev;
  }
  else if (order_type == 1) {
    next->create(h, w, channels);

    if (next->empty()) {
      return -100;
    }

    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      const float* ptr = prevData + q * Data_2DSize(prevSize);
      float* outptr = nextData + q * Data_2DSize(nextSize);

      for (i = 0; i < w; i++) {
        for (j = 0; j < h; j++) {
          outptr[i * h + j] = ptr[j * w + i];
        }
      }
    }
  }
  else if (order_type == 2) {
    next->create(w, channels, h);

    if (next->empty()) {
      return -100;
    }

    #pragma omp parallel for

    for (q = 0; q < h; q++) {
      float* outptr = nextData + q * Data_2DSize(nextSize);

      for (i = 0; i < channels; i++) {
        const float* ptr = Blob_channel_p(prev, i).row(q);

        for (j = 0; j < w; j++) {
          outptr[i * w + j] = ptr[j];
        }
      }
    }
  }
  else if (order_type == 3) {
    next->create(channels, w, h);

    if (next->empty()) {
      return -100;
    }

    #pragma omp parallel for

    for (q = 0; q < h; q++) {
      float* outptr = nextData + q * Data_2DSize(nextSize);

      for (i = 0; i < w; i++) {
        for (j = 0; j < channels; j++) {
          const float* ptr = Blob_channel_p(prev, j).row(q);

          outptr[i * channels + j] = ptr[i];
        }
      }
    }
  }
  else if (order_type == 4) {
    next->create(h, channels, w);

    if (next->empty()) {
      return -100;
    }

    #pragma omp parallel for

    for (q = 0; q < w; q++) {
      float* outptr = nextData + q * Data_2DSize(nextSize);

      for (i = 0; i < channels; i++) {
        const float* ptr = Blob_channel_p(prev, i);

        for (j = 0; j < h; j++) {
          outptr[i * channels + j] = ptr[j * w + q];
        }
      }
    }
  }
  else if (order_type == 5) {
    next->create(channels, h, w);

    if (next->empty()) {
      return -100;
    }

    #pragma omp parallel for

    for (q = 0; q < w; q++) {
      float* outptr = nextData + q * Data_2DSize(nextSize);

      for (i = 0; i < h; i++) {
        for (j = 0; j < channels; j++) {
          const float* ptr = Blob_channel_p(prev, j);

          outptr[i * channels + j] = ptr[i * w + q];
        }
      }
    }
  }

  return 0;
}

} // namespace ncnn
