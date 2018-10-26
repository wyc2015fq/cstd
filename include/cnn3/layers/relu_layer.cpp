#include <algorithm>
#include <vector>

#include "layers/relu_layer.hpp"

namespace caffe
{

  template <typename Dtype>
  void ReLULayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*> & bottom,
                                     const vector<Blob<Dtype>*> & top)
  {
    const Dtype* bottom_data = bottom[0]->cpu_data();
    Dtype* top_data = top[0]->mutable_cpu_data();
    const int count = bottom[0]->count();
    Dtype negative_slope = this->layer_param_.relu_param().negative_slope();
    for (int i = 0; i < count; ++i) {
      top_data[i] = std::max(bottom_data[i], Dtype(0))
                    + negative_slope * std::min(bottom_data[i], Dtype(0));
    }
  }

  template <typename Dtype>
  void ReLULayer<Dtype>::Backward_cpu(const vector<Blob<Dtype>*> & top,
                                      const vector<bool> & propagate_down,
                                      const vector<Blob<Dtype>*> & bottom)
  {
    if (propagate_down[0]) {
      const Dtype* bottom_data = bottom[0]->cpu_data();
      const Dtype* top_diff = top[0]->cpu_diff();
      Dtype* bottom_diff = bottom[0]->mutable_cpu_diff();
      const int count = bottom[0]->count();
      Dtype negative_slope = this->layer_param_.relu_param().negative_slope();
      for (int i = 0; i < count; ++i) {
        bottom_diff[i] = top_diff[i] * ((bottom_data[i] > 0)
                                        + negative_slope * (bottom_data[i] <= 0));
      }
    }
  }


#ifdef CPU_ONLY
  STUB_GPU(ReLULayer);
#endif

  INSTANTIATE_CLASS(ReLULayer);

  // Get relu layer according to engine.
  template <typename Dtype>
  SHARED_PTR(Layer<Dtype>) GetReLULayer(CJSON* param)
  {
    ReLUParameter_Engine engine = param.relu_param().engine();
    if (engine == ReLUParameter_Engine_DEFAULT) {
      engine = ReLUParameter_Engine_CAFFE;
#ifdef USE_CUDNN
      engine = ReLUParameter_Engine_CUDNN;
#endif
    }
    if (engine == ReLUParameter_Engine_CAFFE) {
      return SHARED_PTR(Layer<Dtype>)(new ReLULayer<Dtype>(param));
#ifdef USE_CUDNN
    }
    else if (engine == ReLUParameter_Engine_CUDNN) {
      return SHARED_PTR(Layer<Dtype>)(new CuDNNReLULayer<Dtype>(param));
#endif
    }
    else {
      LOG(FATAL) << "Layer " << param.name() << " has unknown engine.";
    }
    return NULL;
  }

  REGISTER_LAYER_CREATOR(ReLU, GetReLULayer);

}  // namespace caffe
