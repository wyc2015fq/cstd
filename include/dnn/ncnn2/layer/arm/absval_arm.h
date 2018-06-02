

#ifndef LAYER_ABSVAL_ARM_H
#define LAYER_ABSVAL_ARM_H

#include "absval.h"

namespace ncnn
{

struct AbsVal_arm : public AbsVal
{
public:
  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;

  virtual int forward_inplace(img_t* bottom_top_blob) const;

public:
};

} // namespace ncnn

#endif // LAYER_ABSVAL_ARM_H
