

#ifndef LAYER_BINARYOP_H
#define LAYER_BINARYOP_H

#include "layer.h"

namespace ncnn
{

struct BinaryOp : public Layer
{
public:
  BinaryOp();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const std::vector<Blob>& bottom_blobs, std::vector<Blob>& top_blobs) const;

  enum {
    Operation_ADD   = 0,
    Operation_SUB   = 1,
    Operation_MUL   = 2,
    Operation_DIV   = 3,
    Operation_MAX   = 4,
    Operation_MIN   = 5,
    Operation_POW   = 6
  };

public:
  // param
  int op_type;
};

} // namespace ncnn

#endif // LAYER_BINARYOP_H
