

#ifndef LAYER_PERMUTE_H
#define LAYER_PERMUTE_H

#include "layer.h"

namespace ncnn
{

struct Permute : public Layer
{
public:
  Permute();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;

public:
  int order_type;
};

} // namespace ncnn

#endif // LAYER_PERMUTE_H
