

#ifndef LAYER_FLATTEN_H
#define LAYER_FLATTEN_H

#include "layer.h"

namespace ncnn
{

struct Flatten : public Layer
{
public:
  Flatten();

  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;
};

} // namespace ncnn

#endif // LAYER_FLATTEN_H
