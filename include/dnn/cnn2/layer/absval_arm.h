

#ifndef LAYER_ABSVAL_ARM_H
#define LAYER_ABSVAL_ARM_H

#include "absval.h"

namespace ncnn
{

struct AbsVal_arm : public AbsVal
{
public:
  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;

  virtual int forward_inplace(Blob* bottom_top_blob) const;

public:
};

} // namespace ncnn

#endif // LAYER_ABSVAL_ARM_H
