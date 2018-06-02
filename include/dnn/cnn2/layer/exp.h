

#ifndef LAYER_EXP_H
#define LAYER_EXP_H

#include "layer.h"

namespace ncnn
{

struct Exp : public Layer
{
public:
  Exp();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;

  virtual int forward_inplace(Blob* bottom_top_blob) const;

public:
  float base;
  float scale;
  float shift;
};

} // namespace ncnn

#endif // LAYER_EXP_H
