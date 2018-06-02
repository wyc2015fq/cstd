


#include "caffe/filler.hpp"
#include "caffe/layer.hpp"
#include "neuron_layer.hpp"
#include "arccos_layer.hpp"




void ArccosLayer::Forward_cpu(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* bottom_data = pBox_datap(bottom_blob->data);
  Dtype* top_data = top[0]->mutable_cpu_data();
  const int count = bottom[0]->count();
  
  for (int i = 0; i < count; ++i) {
    top_data[i] = acosf(bottom_data[i]);
  }
}


void ArccosLayer::Backward_cpu(const vector<Blob*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
  const Dtype* bottom_data = pBox_datap(bottom_blob->data);
  const Dtype* top_diff = top[0]->cpu_diff();
  const int count = bottom[0]->count();

  // Propagate to bottom
  if (propagate_down[0]) {
    Dtype* bottom_diff = bottom[0]->mutable_cpu_diff();
    for (int i = 0; i < count; ++i) {
      Dtype fixed_in_data = std::min(bottom_data[i], Dtype(1.0) - Dtype(0.01));
      bottom_diff[i] = top_diff[i] * -1 / sqrtf(1.0f - fixed_in_data * fixed_in_data);
    }
  }
}


#ifdef CPU_ONLY
STUB_GPU(ArccosLayer);
#endif

INSTANTIATE_CLASS(ArccosLayer);
REGISTER_LAYER_CLASS(Arccos);


