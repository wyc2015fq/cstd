

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

  virtual int forward(const std::vector<img_t>& bottom_blobs, std::vector<img_t>& top_blobs) const;

public:
  img_t slices;
};

} // namespace ncnn

#endif // LAYER_SLICE_H
