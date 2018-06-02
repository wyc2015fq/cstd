

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

  virtual int forward(const std::vector<img_t>& bottom_blobs, std::vector<img_t>& top_blobs) const;

public:
  img_t min_sizes;
  img_t max_sizes;
  img_t aspect_ratios;
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
