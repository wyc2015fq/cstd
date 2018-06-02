

#ifndef LAYER_SCALE_ARM_H
#define LAYER_SCALE_ARM_H

#include "scale.h"

namespace ncnn
{

struct Scale_arm : public Scale
{
public:
  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;

  virtual int forward_inplace(img_t* bottom_top_blob) const;
};

} // namespace ncnn

#endif // LAYER_SCALE_ARM_H
