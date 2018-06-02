

#ifndef LAYER_SOFTMAX_ARM_H
#define LAYER_SOFTMAX_ARM_H

#include "softmax.h"

namespace ncnn
{

struct Softmax_arm : public Softmax
{
public:
  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;

  virtual int forward_inplace(Blob* bottom_top_blob) const;
};

} // namespace ncnn

#endif // LAYER_SOFTMAX_ARM_H
