

#include "squeeze.h"

namespace ncnn
{

DEFINE_LAYER_CREATOR(Squeeze)

Squeeze::Squeeze()
{
  one_blob_only = true;
  support_inplace = false;
}

int Squeeze::load_param(const ParamDict& pd)
{
  squeeze_w = pd.get(0, 0);
  squeeze_h = pd.get(1, 0);
  squeeze_c = pd.get(2, 0);

  return 0;
}

int Squeeze::forward(const Blob* bottom_blob, Blob* top_blob) const
{
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = Blob_channels(bottom_blob);
  int dims = bottom_blob->dims;

  top_blob = bottom_blob;

  if (squeeze_c && dims == 3 && channels == 1) {
    if (squeeze_h && h == 1) {
      top_blob = bottom_blob->reshape(w);
    }
    else {
      top_blob = bottom_blob->reshape(w, h);
    }
  }
  else if (squeeze_h && dims >= 2 && h == 1) {
    if (squeeze_w && w == 1) {
      top_blob = bottom_blob->reshape(channels);
    }
    else {
      top_blob = bottom_blob->reshape(w, channels);
    }
  }
  else if (squeeze_w && dims >= 1 && w == 1) {
    if (squeeze_h && h == 1) {
      top_blob = bottom_blob->reshape(channels);
    }
    else {
      top_blob = bottom_blob->reshape(h, channels);
    }
  }

  if (top_blob->empty()) {
    return -100;
  }

  return 0;
}

} // namespace ncnn
