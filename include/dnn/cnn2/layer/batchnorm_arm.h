

#ifndef LAYER_BATCHNORM_ARM_H
#define LAYER_BATCHNORM_ARM_H

#include "batchnorm.h"

namespace ncnn
{

struct BatchNorm_arm : public BatchNorm
{
public:
  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;

  virtual int forward_inplace(Blob* bottom_top_blob) const;
};

} // namespace ncnn

#endif // LAYER_BATCHNORM_ARM_H
