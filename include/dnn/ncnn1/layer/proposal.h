

#ifndef LAYER_PROPOSAL_H
#define LAYER_PROPOSAL_H

#include "layer.h"

namespace ncnn
{

struct Proposal : public Layer
{
public:
  Proposal();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const std::vector<img_t>& bottom_blobs, std::vector<img_t>& top_blobs) const;

public:
  // param
  int feat_stride;
  int base_size;
  int pre_nms_topN;
  int after_nms_topN;
  float nms_thresh;
  int min_size;

  img_t ratios;
  img_t scales;

  img_t anchors;
};

} // namespace ncnn

#endif // LAYER_PROPOSAL_H
