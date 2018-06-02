

#ifndef LAYER_BATCHNORM_ARM_H
#define LAYER_BATCHNORM_ARM_H

#include "batchnorm.h"

namespace ncnn
{

struct BatchNorm_arm : public BatchNorm
{
public:
  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;

  virtual int forward_inplace(img_t* bottom_top_blob) const;
};

} // namespace ncnn

#endif // LAYER_BATCHNORM_ARM_H
