

#include "caffe/layers/threshold_layer.hpp"

namespace
{

  template <typename Dtype>
  void ThresholdLayer::LayerSetUp(const vector<Blob*> & bottom,
                                         const vector<Blob*> & top)
  {
    NeuronLayer::LayerSetUp(bottom, top);
    threshold_ = this->param_->threshold_param().threshold();
  }

  template <typename Dtype>
  void ThresholdLayer::Forward(CPUContext* context, const vector<Blob*> & bottom,
                                          const vector<Blob*> & top)
  {
    const Dtype* bottom_data = bottom[0]->data();
    Dtype* top_data = top[0]->mdata();
    const int count = bottom[0]->count();
    for (int i = 0; i < count; ++i) {
      top_data[i] = (bottom_data[i] > threshold_) ? Dtype(1) : Dtype(0);
    }
  }

#ifdef CPU_ONLY
  STUB_GPU_FORWARD(ThresholdLayer, Forward);
#endif

  INSTANTIATE_CLASS(ThresholdLayer);
  REGISTER_LAYER_CLASS(Threshold);

}  // namespace
