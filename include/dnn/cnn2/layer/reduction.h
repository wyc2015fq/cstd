

#ifndef LAYER_REDUCTION_H
#define LAYER_REDUCTION_H

#include "layer.h"

namespace ncnn
{

struct Reduction : public Layer
{
public:
  Reduction();
  virtual ~Reduction();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;

  enum {
    ReductionOp_SUM     = 0,
    ReductionOp_ASUM    = 1,
    ReductionOp_SUMSQ   = 2,
    ReductionOp_MEAN    = 3,
    ReductionOp_MAX     = 4,
    ReductionOp_MIN     = 5,
    ReductionOp_PROD    = 6
  };

public:
  // param
  int operation;
  int dim;
  float coeff;
};

} // namespace ncnn

#endif // LAYER_REDUCTION_H
