

#include "pooling_arm.h"

namespace ncnn
{

#include "pooling_2x2.h"
#include "pooling_3x3.h"

DEFINE_LAYER_CREATOR(Pooling_arm)

int Pooling_arm::forward(const img_t* bottom_blob, img_t* top_blob) const
{
  // max value in NxN window
  // avg value in NxN window

  if (pooling_type != PoolMethod_MAX || stride != 2 || global_pooling == 1) {
    return Pooling::forward(bottom_blob, top_blob);
  }

  if (kernel_size != 2 && kernel_size != 3) {
    return Pooling::forward(bottom_blob, top_blob);
  }

  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = pBox_channels(bottom_blob);

  img_t bottom_blob_bordered = bottom_blob;

  if (pad > 0) {
    copy_make_border(bottom_blob, bottom_blob_bordered, pad, pad, pad, pad, BORDER_CONSTANT, 0.f);

    if (bottom_blob_bordered.empty()) {
      return -100;
    }

    w = bottom_blob_bordered.w;
    h = bottom_blob_bordered.h;
  }
  else if (pad == -233) {
    int wpad = kernel_size + (w - 1) / stride * stride - w;
    int hpad = kernel_size + (h - 1) / stride * stride - h;

    if (wpad > 0 || hpad > 0) {
      copy_make_border(bottom_blob, bottom_blob_bordered, hpad / 2, hpad - hpad / 2, wpad / 2, wpad - wpad / 2, BORDER_CONSTANT, 0.f);

      if (bottom_blob_bordered.empty()) {
        return -100;
      }
    }

    w = bottom_blob_bordered.w;
    h = bottom_blob_bordered.h;
  }

  int outw = (w - kernel_size) / stride + 1;
  int outh = (h - kernel_size) / stride + 1;

  int wtail = (w - kernel_size) % stride;
  int htail = (h - kernel_size) % stride;

  if (pad == -233 || pad == -2333) {
    wtail = 0;
    htail = 0;
  }

  if (wtail != 0 || htail != 0) {
    int wtailpad = 0;
    int htailpad = 0;

    if (wtail != 0) {
      wtailpad = kernel_size - wtail;
    }

    if (htail != 0) {
      htailpad = kernel_size - htail;
    }

    img_t bottom_blob_bordered2;
    copy_make_border(bottom_blob_bordered, bottom_blob_bordered2, 0, htailpad, 0, wtailpad, BORDER_REPLICATE, 0.f);

    if (bottom_blob_bordered2.empty()) {
      return -100;
    }

    bottom_blob_bordered = bottom_blob_bordered2;

    w = bottom_blob_bordered.w;
    h = bottom_blob_bordered.h;

    if (wtail != 0) {
      outw += 1;
    }

    if (htail != 0) {
      outh += 1;
    }
  }

  top_blob->create(outw, outh, channels);

  if (top_blob->empty()) {
    return -100;
  }

  if (kernel_size == 2) {
    pooling2x2s2_max_neon(bottom_blob_bordered, top_blob);
  }

  if (kernel_size == 3) {
    pooling3x3s2_max_neon(bottom_blob_bordered, top_blob);
  }

  return 0;
}

} // namespace ncnn
