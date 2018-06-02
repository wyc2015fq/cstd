

#ifndef LAYER_DECONVOLUTION_ARM_H
#define LAYER_DECONVOLUTION_ARM_H

#include "deconvolution.h"

namespace ncnn
{

struct Deconvolution_arm : public Deconvolution
{
public:
  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;
};

} // namespace ncnn

#endif // LAYER_DECONVOLUTION_ARM_H
