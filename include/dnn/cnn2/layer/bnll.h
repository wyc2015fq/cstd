

#ifndef LAYER_BNLL_H
#define LAYER_BNLL_H

#include "layer.h"

namespace ncnn
{

struct BNLL : public Layer
{
public:
  BNLL();

  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;

  virtual int forward_inplace(Blob* bottom_top_blob) const;

public:
};

} // namespace ncnn

#endif // LAYER_BNLL_H
