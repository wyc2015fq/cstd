

#ifndef LAYER_SLICE_ARM_H
#define LAYER_SLICE_ARM_H

#include "slice.h"

namespace ncnn
{

struct Slice_arm : public Slice
{
public:
  virtual int forward(const std::vector<Blob>& bottom_blobs, std::vector<Blob>& top_blobs) const;
};

} // namespace ncnn

#endif // LAYER_SLICE_ARM_H
