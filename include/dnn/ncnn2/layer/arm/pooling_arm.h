

#ifndef LAYER_POOLING_ARM_H
#define LAYER_POOLING_ARM_H

#include "pooling.h"

namespace ncnn
{

struct Pooling_arm : public Pooling
{
public:
  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;
};

} // namespace ncnn

#endif // LAYER_POOLING_ARM_H
