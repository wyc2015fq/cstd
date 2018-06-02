

#ifndef LAYER_POWER_H
#define LAYER_POWER_H

#include "layer.h"

namespace ncnn
{

struct Power : public Layer
{
public:
  Power();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;

  virtual int forward_inplace(Blob* bottom_top_blob) const;

public:
  float power;
  float scale;
  float shift;
};

} // namespace ncnn

#endif // LAYER_POWER_H
