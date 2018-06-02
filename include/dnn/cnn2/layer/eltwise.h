

#ifndef LAYER_ELTWISE_H
#define LAYER_ELTWISE_H

#include "layer.h"

namespace ncnn
{

struct Eltwise : public Layer
{
public:
  Eltwise();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const std::vector<Blob>& bottom_blobs, std::vector<Blob>& top_blobs) const;

  enum { Operation_PROD = 0, Operation_SUM = 1, Operation_MAX = 2 };

public:
  // param
  int op_type;
  Blob coeffs;
};

} // namespace ncnn

#endif // LAYER_ELTWISE_H
