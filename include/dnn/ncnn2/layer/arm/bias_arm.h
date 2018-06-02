

#ifndef LAYER_BIAS_ARM_H
#define LAYER_BIAS_ARM_H

#include "bias.h"

namespace ncnn
{

struct Bias_arm : public Bias
{
public:
  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;

  virtual int forward_inplace(img_t* bottom_top_blob) const;
};

} // namespace ncnn

#endif // LAYER_BIAS_ARM_H
