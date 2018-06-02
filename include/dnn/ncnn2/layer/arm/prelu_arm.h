

#ifndef LAYER_PRELU_ARM_H
#define LAYER_PRELU_ARM_H

#include "prelu.h"

namespace ncnn
{

struct PReLU_arm : public PReLU
{
public:
  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;

  virtual int forward_inplace(img_t* bottom_top_blob) const;
};

} // namespace ncnn

#endif // LAYER_PRELU_ARM_H
