

#ifndef LAYER_SQUEEZE_H
#define LAYER_SQUEEZE_H

#include "layer.h"

namespace ncnn
{

struct Squeeze : public Layer
{
public:
  Squeeze();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;

public:
  int squeeze_w;
  int squeeze_h;
  int squeeze_c;
};

} // namespace ncnn

#endif // LAYER_SQUEEZE_H
