

#ifndef LAYER_CONVOLUTIONDEPTHWISE_ARM_H
#define LAYER_CONVOLUTIONDEPTHWISE_ARM_H

#include "convolutiondepthwise.h"

namespace ncnn
{

struct ConvolutionDepthWise_arm : public ConvolutionDepthWise
{
public:
  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;
};

} // namespace ncnn

#endif // LAYER_CONVOLUTIONDEPTHWISE_ARM_H
