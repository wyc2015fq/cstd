

#ifndef LAYER_INTERP_H
#define LAYER_INTERP_H

#include "layer.h"

namespace ncnn
{

struct Interp : public Layer
{
public:
  Interp();
  virtual ~Interp();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const Blob& bottom_blob, Blob& top_blob) const;

public:
  // param
  float width_scale;
  float height_scale;
  int output_width;
  int output_height;
  int resize_type;//1:near 2: bilinear
};

} // namespace ncnn

#endif // LAYER_INTERP_H
