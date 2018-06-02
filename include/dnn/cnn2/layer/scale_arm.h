

#ifndef LAYER_SCALE_ARM_H
#define LAYER_SCALE_ARM_H

#include "scale.h"

namespace ncnn
{

struct Scale_arm : public Scale
{
public:
  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;

  virtual int forward_inplace(Blob* bottom_top_blob) const;
};

} // namespace ncnn

#endif // LAYER_SCALE_ARM_H
