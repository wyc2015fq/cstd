

#ifndef LAYER_FLATTEN_H
#define LAYER_FLATTEN_H

#include "layer.h"

namespace ncnn
{

struct Flatten : public Layer
{
public:
  Flatten();

  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;
};

} // namespace ncnn

#endif // LAYER_FLATTEN_H
