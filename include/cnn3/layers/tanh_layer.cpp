// TanH neuron activation function layer.
// Adapted from ReLU layer code written by Yangqing Jia

#include <vector>

#include "layers/tanh_layer.hpp"

namespace caffe
{

  template <typename Dtype>
  void TanHLayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*> & bottom,
                                     const vector<Blob<Dtype>*> & top)
  {
    const Dtype* bottom_data = bottom[0]->cpu_data();
    Dtype* top_data = top[0]->mutable_cpu_data();
    const int count = bottom[0]->count();
    for (int i = 0; i < count; ++i) {
      top_data[i] = tanh(bottom_data[i]);
    }
  }

  template <typename Dtype>
  void TanHLayer<Dtype>::Backward_cpu(const vector<Blob<Dtype>*> & top,
                                      const vector<bool> & propagate_down,
                                      const vector<Blob<Dtype>*> & bottom)
  {
    if (propagate_down[0]) {
      const Dtype* top_data = top[0]->cpu_data();
      const Dtype* top_diff = top[0]->cpu_diff();
      Dtype* bottom_diff = bottom[0]->mutable_cpu_diff();
      const int count = bottom[0]->count();
      Dtype tanhx;
      for (int i = 0; i < count; ++i) {
        tanhx = top_data[i];
        bottom_diff[i] = top_diff[i] * (1 - tanhx * tanhx);
      }
    }
  }

#ifdef CPU_ONLY
  STUB_GPU(TanHLayer);
#endif

  INSTANTIATE_CLASS(TanHLayer);

  // Get tanh layer according to engine.
  template <typename Dtype>
  SHARED_PTR(Layer<Dtype>) GetTanHLayer(CJSON* param)
  {
    TanHParameter_Engine engine = param.tanh_param().engine();
    if (engine == TanHParameter_Engine_DEFAULT) {
      engine = TanHParameter_Engine_CAFFE;
#ifdef USE_CUDNN
      engine = TanHParameter_Engine_CUDNN;
#endif
    }
    if (engine == TanHParameter_Engine_CAFFE) {
      return SHARED_PTR(Layer<Dtype>)(new TanHLayer<Dtype>(param));
#ifdef USE_CUDNN
    }
    else if (engine == TanHParameter_Engine_CUDNN) {
      return SHARED_PTR(Layer<Dtype>)(new CuDNNTanHLayer<Dtype>(param));
#endif
    }
    else {
      LOG(FATAL) << "Layer " << param.name() << " has unknown engine.";
    }
    return NULL;
  }

  REGISTER_LAYER_CREATOR(TanH, GetTanHLayer);

}  // namespace caffe
