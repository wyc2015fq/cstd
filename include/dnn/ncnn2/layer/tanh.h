

#ifndef LAYER_TANH_H
#define LAYER_TANH_H

#include "layer.h"

namespace ncnn
{

struct TanH : public Layer
{
public:
  TanH();

  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;

  virtual int forward_inplace(img_t* bottom_top_blob) const;

public:
};

} // namespace ncnn

#endif // LAYER_TANH_H
