#ifdef USE_OPENCV
#include <opencv2/core/core.hpp>
#endif  // USE_OPENCV
#include <stdint.h>



#include "caffe/data_transformer.hpp"
#include "caffe/layers/data_layer.hpp"
#include "caffe/util/benchmark.hpp"

namespace
{

  INSTANTIATE_CLASS(DataLayer);
  REGISTER_LAYER_CLASS(Data);

}  // namespace
