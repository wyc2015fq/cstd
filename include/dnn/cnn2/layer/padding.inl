

#ifndef LAYER_PADDING_H
#define LAYER_PADDING_H

#include "layer.h"

namespace ncnn
{

struct Padding : public Layer
{
public:
  Padding();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;

public:
  int top;
  int bottom;
  int left;
  int right;
  int type;
  float value;
};

} // namespace ncnn

#endif // LAYER_PADDING_H



Padding::Padding()
{
  one_blob_only = true;
  support_inplace = false;
}

int Padding::load_param(const ParamDict& pd)
{
  top = pd.get(0, 0);
  bottom = pd.get(1, 0);
  left = pd.get(2, 0);
  right = pd.get(3, 0);
  type = pd.get(4, 0);
  value = pd.get(5, 0.f);

  return 0;
}

int Padding::forward(const Blob* bottom_blob, Blob* top_blob) const
{
  copy_make_border(bottom_blob, top_blob, top, bottom, left, right, type, value);

  if (top_blob->empty()) {
    return -100;
  }

  return 0;
}


