

#ifndef LAYER_TANH_H
#define LAYER_TANH_H

#include "layer.h"

namespace ncnn
{

struct TanH : public Layer
{
public:
  TanH();

  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;

  virtual int forward_inplace(Blob* bottom_top_blob) const;

public:
};

} // namespace ncnn

#endif // LAYER_TANH_H
