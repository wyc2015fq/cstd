

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

  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;

  virtual int forward_inplace(img_t* bottom_top_blob) const;

public:
  float power;
  float scale;
  float shift;
};

} // namespace ncnn

#endif // LAYER_POWER_H
