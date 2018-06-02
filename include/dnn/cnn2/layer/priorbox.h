

#ifndef LAYER_PRIORBOX_H
#define LAYER_PRIORBOX_H

#include "layer.h"

namespace ncnn
{

struct PriorBox : public Layer
{
public:
  PriorBox();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const std::vector<Blob>& bottom_blobs, std::vector<Blob>& top_blobs) const;

public:
  Blob min_sizes;
  Blob max_sizes;
  Blob aspect_ratios;
  float variances[4];
  int flip;
  int clip;
  int image_width;
  int image_height;
  float step_width;
  float step_height;
  float offset;
};

} // namespace ncnn

#endif // LAYER_PRIORBOX_H
