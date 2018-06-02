

#ifndef LAYER_SOFTMAX_ARM_H
#define LAYER_SOFTMAX_ARM_H

#include "softmax.h"

namespace ncnn
{

struct Softmax_arm : public Softmax
{
public:
  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;

  virtual int forward_inplace(img_t* bottom_top_blob) const;
};

} // namespace ncnn

#endif // LAYER_SOFTMAX_ARM_H
