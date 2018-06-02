

#ifndef LAYER_MVN_H
#define LAYER_MVN_H

#include "layer.h"

namespace ncnn
{

struct MVN : public Layer
{
public:
  MVN();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;

public:
  int normalize_variance;
  int across_channels;
  float eps;
};

} // namespace ncnn

#endif // LAYER_MVN_H
