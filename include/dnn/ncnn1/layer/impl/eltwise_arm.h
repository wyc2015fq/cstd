

#ifndef LAYER_ELTWISE_ARM_H
#define LAYER_ELTWISE_ARM_H

#include "eltwise.h"

namespace ncnn
{

struct Eltwise_arm : public Eltwise
{
public:
  virtual int forward(const std::vector<img_t>& bottom_blobs, std::vector<img_t>& top_blobs) const;
};

} // namespace ncnn

#endif // LAYER_ELTWISE_ARM_H
