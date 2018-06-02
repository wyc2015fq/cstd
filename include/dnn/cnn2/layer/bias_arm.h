

#ifndef LAYER_BIAS_ARM_H
#define LAYER_BIAS_ARM_H

#include "bias.h"

namespace ncnn
{

struct Bias_arm : public Bias
{
public:
  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;

  virtual int forward_inplace(Blob* bottom_top_blob) const;
};

} // namespace ncnn

#endif // LAYER_BIAS_ARM_H
