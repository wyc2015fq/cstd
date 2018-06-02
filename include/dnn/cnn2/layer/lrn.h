

#ifndef LAYER_LRN_H
#define LAYER_LRN_H

#include "layer.h"

namespace ncnn
{

struct LRN : public Layer
{
public:
  LRN();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;

  virtual int forward_inplace(Blob* bottom_top_blob) const;

  enum { NormRegion_ACROSS_CHANNELS = 0, NormRegion_WITHIN_CHANNEL = 1 };

public:
  // param
  int region_type;
  int local_size;
  float alpha;
  float beta;
};

} // namespace ncnn

#endif // LAYER_LRN_H
