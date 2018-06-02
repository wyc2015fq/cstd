


#include "caffe/filler.hpp"
#include "caffe/layer.hpp"
#include "neuron_layer.hpp"
#include "cosine_layer.hpp"




void CosineLayer::Forward_cpu(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* bottom_data = pBox_datap(bottom_blob->data);
  Dtype* top_data = top[0]->mutable_cpu_data();
  const int count = bottom[0]->count();
  
  for (int i = 0; i < count; ++i) {
    top_data[i] = cos(bottom_data[i]);
  }
}


void CosineLayer::Backward_cpu(const vector<Blob*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
  const Dtype* bottom_data = pBox_datap(bottom_blob->data);
  const Dtype* top_diff = top[0]->cpu_diff();
  const int count = bottom[0]->count();

  // Propagate to bottom
  if (propagate_down[0]) {
    Dtype* bottom_diff = bottom[0]->mutable_cpu_diff();
    for (int i = 0; i < count; ++i) {
      bottom_diff[i] = -sin(bottom_data[i]);
    }
  }
}


#ifdef CPU_ONLY
STUB_GPU(CosineLayer);
#endif

INSTANTIATE_CLASS(CosineLayer);
REGISTER_LAYER_CLASS(Cosine);


