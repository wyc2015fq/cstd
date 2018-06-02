


#include "correlation_loss_layer.hpp"




void CorrelationLossLayer::LayerSetUp(const vector<Blob*>& bottom,
                                               const vector<Blob*>& top) {
  LossLayer::LayerSetUp(bottom, top);
  CHECK_GE(bottom.size(), 2);
}


void CorrelationLossLayer::Reshape(const vector<Blob*>& bottom,
                                                 const vector<Blob*>& top) {
  LossLayer::Reshape(bottom, top);
  CHECK_EQ(bottom[0]->channels(), 1);
  CHECK_EQ(bottom[1]->channels(), 1);
}


void CorrelationLossLayer::Forward_cpu(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* bottom_data = pBox_datap(bottom_blob->data);
  const Dtype* label = pBox_datap(top_blob->data);

  Dtype* loss = top[0]->mutable_cpu_data();
  loss[0] = caffe_cpu_dot(bottom[0]->count(), bottom_data, label);
  loss[0] /= bottom[0]->count();
}


void CorrelationLossLayer::Backward_cpu(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  if (propagate_down[0]) {
    caffe_cpu_scale(bottom[0]->count(), top[0]->cpu_diff()[0] / bottom[0]->count(), pBox_datap(top_blob->data), bottom[0]->mutable_cpu_data());
  }
  if (propagate_down[1]) {
    caffe_cpu_scale(bottom[1]->count(), top[0]->cpu_diff()[0] / bottom[0]->count(), pBox_datap(bottom_blob->data), bottom[1]->mutable_cpu_data());
  }
}

#ifdef CPU_ONLY
STUB_GPU(CorrelationLossLayer);
#endif

INSTANTIATE_CLASS(CorrelationLossLayer);
REGISTER_LAYER_CLASS(CorrelationLoss);


