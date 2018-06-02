

#include "threshold_layer.hpp"




void ThresholdLayer::LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  NeuronLayer::LayerSetUp(bottom, top);
  threshold_ = this->layer_param_.threshold_param().threshold();
}


void ThresholdLayer::Forward_cpu(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* bottom_data = pBox_datap(bottom_blob->data);
  Dtype* top_data = top[0]->mutable_cpu_data();
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


