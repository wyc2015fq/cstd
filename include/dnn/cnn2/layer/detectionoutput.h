

#ifndef LAYER_DETECTIONOUTPUT_H
#define LAYER_DETECTIONOUTPUT_H

#include "layer.h"

namespace ncnn
{

struct DetectionOutput : public Layer
{
public:
  DetectionOutput();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const std::vector<Blob>& bottom_blobs, std::vector<Blob>& top_blobs) const;

public:
  int num_class;
  float nms_threshold;
  int nms_top_k;
  int keep_top_k;
  float confidence_threshold;
};

} // namespace ncnn

#endif // LAYER_DETECTIONOUTPUT_H
