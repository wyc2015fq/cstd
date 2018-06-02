

#ifndef LAYER_RESHAPE_H
#define LAYER_RESHAPE_H

#include "layer.h"

namespace ncnn
{

struct Reshape : public Layer
{
public:
  Reshape();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;

private:
  int w;
  int h;
  int c;
  int permute;
  int ndim;
};

} // namespace ncnn

#endif // LAYER_RESHAPE_H
