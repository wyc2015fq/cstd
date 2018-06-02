

#ifndef LAYER_LRN_ARM_H
#define LAYER_LRN_ARM_H

#include "lrn.h"

namespace ncnn
{

struct LRN_arm : public LRN
{
public:
  virtual int forward_inplace(img_t* bottom_top_blob) const;
};

} // namespace ncnn

#endif // LAYER_LRN_ARM_H
