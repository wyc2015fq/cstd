

#include "deconvolution_3x3.h"


int Deconvolution_forward(const Blob* prev, Blob* next) const
{
  if (kernel_size != 3 || stride != 1 || dilation != 1) {
    return Deconvolution::forward(prev, next);
  }

  typedef void (*deconv_func)(const Blob*, Blob*, const Blob*, const Blob*);

  deconv_func deconv = deconv3x3s1_neon;

  int w = prevSize.w;
  int h = prevSize.h;
  int channels = prevSize.c;

  int outw = (w - 1) * stride + kernel_size;
  int outh = (h - 1) * stride + kernel_size;

  Blob top_blob_bordered;
  top_blob_bordered.create(outw, outh, num_output);

  if (top_blob_bordered.empty()) {
    return -100;
  }

  deconv(prev, top_blob_bordered, weight_data, bias_data);

  next = top_blob_bordered;

  if (pad > 0) {
    copy_cut_border(top_blob_bordered, next, pad, pad, pad, pad);

    if (next->empty()) {
      return -100;
    }

    outw = next->w;
    outh = next->h;
  }

  return 0;
}
