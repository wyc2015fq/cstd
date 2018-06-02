

#ifndef LAYER_SPP_H
#define LAYER_SPP_H

#include "layer.h"

namespace ncnn
{

struct SPP : public Layer
{
public:
  SPP();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;

  enum { PoolMethod_MAX = 0, PoolMethod_AVE = 1 };

public:
  // param
  int pooling_type;
  int pyramid_height;
};

} // namespace ncnn

#endif // LAYER_SPP_H
