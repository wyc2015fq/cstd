

#ifndef LAYER_SOFTMAX_H
#define LAYER_SOFTMAX_H

#include "layer.h"

namespace ncnn
{

struct Softmax : public Layer
{
public:
  Softmax();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;

public:
  int axis;
};

} // namespace ncnn

#endif // LAYER_SOFTMAX_H
