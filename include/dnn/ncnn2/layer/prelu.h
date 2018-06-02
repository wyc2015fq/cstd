

#ifndef LAYER_PRELU_H
#define LAYER_PRELU_H

#include "layer.h"

namespace ncnn
{

struct PReLU : public Layer
{
public:
  PReLU();

  virtual int load_param(const ParamDict& pd);

#if NCNN_STDIO
  virtual int load_model(FILE* binfp);
#endif // NCNN_STDIO
  virtual int load_model(const unsigned char*& mem);

  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;

  virtual int forward_inplace(img_t* bottom_top_blob) const;

public:
  int num_slope;
  img_t slope_data;
};

} // namespace ncnn

#endif // LAYER_PRELU_H
