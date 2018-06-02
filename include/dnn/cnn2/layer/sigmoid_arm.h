

#ifndef LAYER_SIGMOID_ARM_H
#define LAYER_SIGMOID_ARM_H

#include "sigmoid.h"

namespace ncnn
{

struct Sigmoid_arm : public Sigmoid
{
public:
  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;

  virtual int forward_inplace(Blob* bottom_top_blob) const;
};

} // namespace ncnn

#endif // LAYER_SIGMOID_ARM_H
