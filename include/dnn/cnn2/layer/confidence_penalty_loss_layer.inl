



#include "confidence_penalty_loss_layer.hpp"




  
  void ConfidencePenaltyLossLayer::LayerSetUp(const vector<Blob*>& bottom,
                                                const vector<Blob*>& top) {
    LossLayer::LayerSetUp(bottom, top);
    beta_ = this->layer_param_.confidence_penalty_loss_param().beta();
  }

  
  void ConfidencePenaltyLossLayer::Reshape(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
    LossLayer::Reshape(bottom, top);
    CHECK_EQ(bottom[1]->channels(), 1);
    CHECK_EQ(bottom[1]->height(), 1);
    CHECK_EQ(bottom[1]->width(), 1);
  }

  
  void ConfidencePenaltyLossLayer::Forward_cpu(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
    const Dtype* bottom_data = Blob_datap(bottom_blob->data);
    const Dtype* bottom_label = Blob_datap(top_blob->data);
    int num = bottom[0]->num();
    int dim = bottom[0]->count() / bottom[0]->num();
    Dtype loss = 0;
    for (int i = 0; i < num; ++i) {
      for (int j = 0; j < dim; ++j) {
        Dtype prob = std::max(
          bottom_data[i * dim + j], Dtype(kLOG_THRESHOLD));
        if (j == static_cast<int>(bottom_label[i])) {
          loss -= (Dtype(1.0) - beta_) * log(prob);
        }
        else {
          loss -= (beta_ / (num - Dtype(1.0))) * log(prob);
        }
      }
    }
    top[0]->mutable_cpu_data()[0] = loss / num;
  }

  
  void ConfidencePenaltyLossLayer::Backward_cpu(
    const vector<Blob*>& top, const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
    if (propagate_down[1]) {
      LOG(FATAL) << this->type()
        << " Layer cannot backpropagate to label inputs.";
    }
    if (propagate_down[0]) {
      const Dtype* bottom_data = Blob_datap(bottom_blob->data);
      const Dtype* bottom_label = Blob_datap(top_blob->data);
      Dtype* bottom_diff = bottom[0]->mutable_cpu_diff();
      int num = bottom[0]->num();
      int dim = bottom[0]->count() / bottom[0]->num();
      caffe_set(bottom[0]->count(), Dtype(0), bottom_diff);
      const Dtype scale = -top[0]->cpu_diff()[0] / num;
      for (int i = 0; i < num; ++i) {
        for (int j = 0; j < dim; ++j) {
          Dtype prob = std::max(bottom_data[i * dim + j], Dtype(kLOG_THRESHOLD));
          if (j == static_cast<int>(bottom_label[i])) {
            bottom_diff[i * dim + j] = scale * (Dtype(1.0) - beta_) / prob;
          }
          else {
            bottom_diff[i * dim + j] = scale * (beta_ / (num - Dtype(1.0))) / prob;
          }
        }
      }
    }
  }

  INSTANTIATE_CLASS(ConfidencePenaltyLossLayer);
  REGISTER_LAYER_CLASS(ConfidencePenaltyLoss);


