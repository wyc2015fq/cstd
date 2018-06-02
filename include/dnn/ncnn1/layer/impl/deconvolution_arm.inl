

#include "deconvolution_arm.h"

namespace ncnn
{

#include "deconvolution_3x3.h"

DEFINE_LAYER_CREATOR(Deconvolution_arm)

int Deconvolution_arm::forward(const img_t* bottom_blob, img_t* top_blob) const
{
  if (kernel_size != 3 || stride != 1 || dilation != 1) {
    return Deconvolution::forward(bottom_blob, top_blob);
  }

  typedef void (*deconv_func)(const img_t*, img_t*, const img_t*, const img_t*);

  deconv_func deconv = deconv3x3s1_neon;

  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = pBox_channels(bottom_blob);

  int outw = (w - 1) * stride + kernel_size;
  int outh = (h - 1) * stride + kernel_size;

  img_t top_blob_bordered;
  top_blob_bordered.create(outw, outh, num_output);

  if (top_blob_bordered.empty()) {
    return -100;
  }

  deconv(bottom_blob, top_blob_bordered, weight_data, bias_data);

  top_blob = top_blob_bordered;

  if (pad > 0) {
    copy_cut_border(top_blob_bordered, top_blob, pad, pad, pad, pad);

    if (top_blob->empty()) {
      return -100;
    }

    outw = top_blob->w;
    outh = top_blob->h;
  }

  return 0;
}

} // namespace ncnn
