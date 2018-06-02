

#ifndef LAYER_RELU_ARM_H
#define LAYER_RELU_ARM_H

#include "relu.h"

namespace ncnn
{

struct ReLU_arm : public ReLU
{
public:
  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;

  virtual int forward_inplace(img_t* bottom_top_blob) const;
};

} // namespace ncnn

#endif // LAYER_RELU_ARM_H
