

#include "expanddims.h"

namespace ncnn
{

DEFINE_LAYER_CREATOR(ExpandDims)

ExpandDims::ExpandDims()
{
  one_blob_only = true;
  support_inplace = false;
}

int ExpandDims::load_param(const ParamDict& pd)
{
  expand_w = pd.get(0, 0);
  expand_h = pd.get(1, 0);
  expand_c = pd.get(2, 0);

  return 0;
}

int ExpandDims::forward(const Blob* bottom_blob, Blob* top_blob) const
{
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int dims = bottom_blob->dims;

  top_blob = bottom_blob;

  if (dims == 1) {
    if (expand_w) {
      if (expand_h) {
        top_blob = bottom_blob->reshape(1, 1, w);
      }
      else if (expand_c) {
        top_blob = bottom_blob->reshape(1, w, 1);
      }
      else {
        top_blob = bottom_blob->reshape(1, w);
      }
    }
    else if (expand_h) {
      if (expand_c) {
        top_blob = bottom_blob->reshape(w, 1, 1);
      }
      else {
        top_blob = bottom_blob->reshape(w, 1);
      }
    }
  }
  else if (dims == 2) {
    if (expand_w) {
      top_blob = bottom_blob->reshape(1, w, h);
    }
    else if (expand_h) {
      top_blob = bottom_blob->reshape(w, 1, h);
    }
    else if (expand_c) {
      top_blob = bottom_blob->reshape(w, h, 1);
    }
  }

  if (top_blob->empty()) {
    return -100;
  }

  return 0;
}

} // namespace ncnn
