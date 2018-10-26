#include <cmath>
#include <vector>

#include "layers/sigmoid_layer.hpp"

namespace caffe
{

  template <typename Dtype>
  inline Dtype sigmoid1(Dtype x)
  {
    return 1. / (1. + exp(-x));
  }

  template <typename Dtype>
  void SigmoidLayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*> & bottom,
                                        const vector<Blob<Dtype>*> & top)
  {
    const Dtype* bottom_data = bottom[0]->cpu_data();
    Dtype* top_data = top[0]->mutable_cpu_data();
    const int count = bottom[0]->count();
    for (int i = 0; i < count; ++i) {
      top_data[i] = sigmoid(bottom_data[i]);
    }
  }

  template <typename Dtype>
  void SigmoidLayer<Dtype>::Backward_cpu(const vector<Blob<Dtype>*> & top,
                                         const vector<bool> & propagate_down,
                                         const vector<Blob<Dtype>*> & bottom)
  {
    if (propagate_down[0]) {
      const Dtype* top_data = top[0]->cpu_data();
      const Dtype* top_diff = top[0]->cpu_diff();
      Dtype* bottom_diff = bottom[0]->mutable_cpu_diff();
      const int count = bottom[0]->count();
      for (int i = 0; i < count; ++i) {
        const Dtype sigmoid_x = top_data[i];
        bottom_diff[i] = top_diff[i] * sigmoid_x * (1. - sigmoid_x);
      }
    }
  }

#ifdef CPU_ONLY
  STUB_GPU(SigmoidLayer);
#endif

  INSTANTIATE_CLASS(SigmoidLayer);

  // Get sigmoid layer according to engine.
  template <typename Dtype>
  SHARED_PTR(Layer<Dtype>) GetSigmoidLayer(CJSON* param)
  {
    SigmoidParameter_Engine engine = param.sigmoid_param().engine();
    if (engine == SigmoidParameter_Engine_DEFAULT) {
      engine = SigmoidParameter_Engine_CAFFE;
#ifdef USE_CUDNN
      engine = SigmoidParameter_Engine_CUDNN;
#endif
    }
    if (engine == SigmoidParameter_Engine_CAFFE) {
      return SHARED_PTR(Layer<Dtype>)(new SigmoidLayer<Dtype>(param));
#ifdef USE_CUDNN
    }
    else if (engine == SigmoidParameter_Engine_CUDNN) {
      return SHARED_PTR(Layer<Dtype>)(new CuDNNSigmoidLayer<Dtype>(param));
#endif
    }
    else {
      LOG(FATAL) << "Layer " << param.name() << " has unknown engine.";
    }
    return NULL;
  }

  REGISTER_LAYER_CREATOR(Sigmoid, GetSigmoidLayer);


}  // namespace caffe
