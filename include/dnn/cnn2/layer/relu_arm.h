

#ifndef LAYER_RELU_ARM_H
#define LAYER_RELU_ARM_H

#include "relu.h"

namespace ncnn
{

struct ReLU_arm : public ReLU
{
public:
  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;

  virtual int forward_inplace(Blob* bottom_top_blob) const;
};

} // namespace ncnn

#endif // LAYER_RELU_ARM_H
