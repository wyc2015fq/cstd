

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

  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;

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
