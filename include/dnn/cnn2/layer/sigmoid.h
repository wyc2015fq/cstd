

#ifndef LAYER_SIGMOID_H
#define LAYER_SIGMOID_H

#include "layer.h"

namespace ncnn
{

struct Sigmoid : public Layer
{
public:
  Sigmoid();

  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;

  virtual int forward_inplace(Blob* bottom_top_blob) const;

public:
};

} // namespace ncnn

#endif // LAYER_SIGMOID_H
