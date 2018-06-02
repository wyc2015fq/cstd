

#ifndef LAYER_SLICE_H
#define LAYER_SLICE_H

#include "layer.h"

namespace ncnn
{

struct Slice : public Layer
{
public:
  Slice();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const std::vector<Blob>& bottom_blobs, std::vector<Blob>& top_blobs) const;

public:
  Blob slices;
};

} // namespace ncnn

#endif // LAYER_SLICE_H
