

#include "caffe/layers/deconv_layer.hpp"

namespace
{

#ifdef CPU_ONLY
  STUB_GPU(DeconvolutionLayer);
#endif

  INSTANTIATE_CLASS(DeconvolutionLayer);
  REGISTER_LAYER_CLASS(Deconvolution);

}  // namespace
