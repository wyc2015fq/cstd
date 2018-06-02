

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

  virtual int forward(const std::vector<img_t>& bottom_blobs, std::vector<img_t>& top_blobs) const;

  enum { Operation_PROD = 0, Operation_SUM = 1, Operation_MAX = 2 };

public:
  // param
  int op_type;
  img_t coeffs;
};

} // namespace ncnn

#endif // LAYER_ELTWISE_H
