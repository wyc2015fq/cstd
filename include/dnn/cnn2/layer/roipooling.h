

#ifndef LAYER_ROIPOOLING_H
#define LAYER_ROIPOOLING_H

#include "layer.h"

namespace ncnn
{

struct ROIPooling : public Layer
{
public:
  ROIPooling();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const std::vector<Blob>& bottom_blobs, std::vector<Blob>& top_blobs) const;

public:
  int pooled_width;
  int pooled_height;
  float spatial_scale;
};

} // namespace ncnn

#endif // LAYER_ROIPOOLING_H
