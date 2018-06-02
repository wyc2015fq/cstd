

#ifndef LAYER_PRELU_ARM_H
#define LAYER_PRELU_ARM_H

#include "prelu.h"

namespace ncnn
{

struct PReLU_arm : public PReLU
{
public:
  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;

  virtual int forward_inplace(Blob* bottom_top_blob) const;
};

} // namespace ncnn

#endif // LAYER_PRELU_ARM_H
