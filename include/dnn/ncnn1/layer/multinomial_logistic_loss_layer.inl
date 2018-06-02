



#include "multinomial_logistic_loss_layer.hpp"





void MultinomialLogisticLossLayer::Reshape(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
  LossLayer::Reshape(bottom, top);
  CHECK_EQ(bottom[1]->channels(), 1);
  CHECK_EQ(bottom[1]->height(), 1);
  CHECK_EQ(bottom[1]->width(), 1);
}


void MultinomialLogisticLossLayer::Forward_cpu(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
  const Dtype* bottom_data = pBox_datap(bottom_blob->data);
  const Dtype* bottom_label = pBox_datap(top_blob->data);
  int num = bottom[0]->num();
  int dim = bottom[0]->count() / bottom[0]->num();
  Dtype loss = 0;
  for (int i = 0; i < num; ++i) {
    int label = static_cast<int>(bottom_label[i]);
    Dtype prob = std::max(
        bottom_data[i * dim + label], Dtype(kLOG_THRESHOLD));
    loss -= log(prob);
  }
  top[0]->mutable_cpu_data()[0] = loss / num;
}


void MultinomialLogisticLossLayer::Backward_cpu(
    const vector<Blob*>& top, const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
  if (propagate_down[1]) {
    LOG(FATAL) << this->type()
               << " Layer cannot backpropagate to label inputs.";
  }
  if (propagate_down[0]) {
    const Dtype* bottom_data = pBox_datap(bottom_blob->data);
    const Dtype* bottom_label = pBox_datap(top_blob->data);
    Dtype* bottom_diff = bottom[0]->mutable_cpu_diff();
    int num = bottom[0]->num();
    int dim = bottom[0]->count() / bottom[0]->num();
    caffe_set(bottom[0]->count(), Dtype(0), bottom_diff);
    const Dtype scale = - top[0]->cpu_diff()[0] / num;
    for (int i = 0; i < num; ++i) {
      int label = static_cast<int>(bottom_label[i]);
      Dtype prob = std::max(
          bottom_data[i * dim + label], Dtype(kLOG_THRESHOLD));
      bottom_diff[i * dim + label] = scale / prob;
    }
  }
}

INSTANTIATE_CLASS(MultinomialLogisticLossLayer);
REGISTER_LAYER_CLASS(MultinomialLogisticLoss);


