

#ifndef LAYER_SIGMOID_H
#define LAYER_SIGMOID_H

#include "layer.h"

namespace ncnn
{

struct Sigmoid : public Layer
{
public:
  Sigmoid();

  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;

  virtual int forward_inplace(img_t* bottom_top_blob) const;

public:
};

} // namespace ncnn

#endif // LAYER_SIGMOID_H
