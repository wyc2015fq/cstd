#include <algorithm>
#include <cmath>


#include "caffe/layers/multinomial_logistic_loss_layer.hpp"


namespace
{

  template <typename Dtype>
  void MultinomialLogisticLossLayer::Reshape(
    const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    LossLayer::Reshape(bottom, top);
    CHECK_EQ(bottom[1]->channels(), 1);
    CHECK_EQ(bottom[1]->height(), 1);
    CHECK_EQ(bottom[1]->width(), 1);
  }

  template <typename Dtype>
  void MultinomialLogisticLossLayer::Forward_(_CONTEXT,
    const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    const Dtype* bottom_data = bottom[0]->data();
    const Dtype* bottom_label = bottom[1]->data();
    int num = bottom[0]->num();
    int dim = bottom[0]->count() / bottom[0]->num();
    Dtype loss = 0;
    for (int i = 0; i < num; ++i) {
      int label = static_cast<int>(bottom_label[i]);
      Dtype prob = std::max(
                     bottom_data[i * dim + label], Dtype(kLOG_THRESHOLD));
      loss -= log(prob);
    }
    top[0]->mdata()[0] = loss / num;
  }

  template <typename Dtype>
  void MultinomialLogisticLossLayer::Backward_cpu(
    const vector<Blob*> & top, int*
    const vector<Blob*> & bottom)
  {
    if (bottom[1]->propagate_down_) {
      LOG(FATAL) << this->type()
                 << " Layer cannot backpropagate to label inputs.";
    }
    if (bottom[0]->propagate_down_) {
      const Dtype* bottom_data = bottom[0]->data();
      const Dtype* bottom_label = bottom[1]->data();
      Dtype* bottom_diff = bottom[0]->mdiff();
      int num = bottom[0]->num();
      int dim = bottom[0]->count() / bottom[0]->num();
      caffe_set(bottom[0]->count(), Dtype(0), bottom_diff);
      const Dtype scale = - top[0]->diff()[0] / num;
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

}  // namespace
