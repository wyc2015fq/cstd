

#ifndef LAYER_ELU_H
#define LAYER_ELU_H

#include "layer.h"

namespace ncnn
{

struct ELU : public Layer
{
public:
  ELU();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;

  virtual int forward_inplace(img_t* bottom_top_blob) const;

public:
  float alpha;
};

} // namespace ncnn

#endif // LAYER_ELU_H
