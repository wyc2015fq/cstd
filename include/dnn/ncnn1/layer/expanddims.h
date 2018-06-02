

#ifndef LAYER_EXPANDDIMS_H
#define LAYER_EXPANDDIMS_H

#include "layer.h"

namespace ncnn
{

struct ExpandDims : public Layer
{
public:
  ExpandDims();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;

public:
  int expand_w;
  int expand_h;
  int expand_c;
};

} // namespace ncnn

#endif // LAYER_EXPANDDIMS_H
