

#ifndef LAYER_THRESHOLD_H
#define LAYER_THRESHOLD_H

#include "layer.h"

namespace ncnn
{

struct Threshold : public Layer
{
public:
  Threshold();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;

  virtual int forward_inplace(Blob* bottom_top_blob) const;

public:
  float threshold;
};

} // namespace ncnn

#endif // LAYER_THRESHOLD_H
