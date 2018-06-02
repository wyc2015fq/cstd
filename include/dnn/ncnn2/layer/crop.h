

#ifndef LAYER_CROP_H
#define LAYER_CROP_H

#include "layer.h"

namespace ncnn
{

struct Crop : public Layer
{
public:
  Crop();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const std::vector<img_t>& bottom_blobs, std::vector<img_t>& top_blobs) const;

public:
  int woffset;
  int hoffset;
};

} // namespace ncnn

#endif // LAYER_CROP_H
