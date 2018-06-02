

#ifndef LAYER_CONVOLUTION_ARM_H
#define LAYER_CONVOLUTION_ARM_H

#include "convolution.h"

namespace ncnn
{

struct Convolution_arm : public Convolution
{
public:
  virtual int load_param(const ParamDict& pd);

#if NCNN_STDIO
  virtual int load_model(FILE* binfp);
#endif // NCNN_STDIO
  virtual int load_model(const unsigned char*& mem);

  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;

public:
  bool use_winograd3x3;
  img_t weight_3x3_winograd64_data;
};

} // namespace ncnn

#endif // LAYER_CONVOLUTION_ARM_H
