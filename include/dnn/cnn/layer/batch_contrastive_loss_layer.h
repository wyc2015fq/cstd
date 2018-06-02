

#include "loss_layer.hpp"




class BatchContrastiveLossLayer : public LossLayer {
 public:
  explicit BatchContrastiveLossLayer(const LayerParameter& param)
      : LossLayer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
                          const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
                       const vector<Blob*>& top);
  virtual inline const char* type() const { return "BatchContrastiveLoss"; }
  virtual inline int ExactNumBottomBlobs() const { return -1; }
  virtual inline int ExactNumTopBlobs() const { return -1; }
  virtual inline int MinTopBlobs() const { return 1; }
  virtual inline int MaxTopBlobs() const { return 2; }

 protected:
  virtual void Forward(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual void Backward(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  Dtype positive_margin_;
  Dtype negative_margin_;
  Dtype positive_weight_;
  Dtype negative_weight_;
  bool max_only_;
  int max_positive_1_, max_positive_2_;
  int min_negative_1_, min_negative_2_;
};






void BatchContrastiveLossLayer::LayerSetUp(const vector<Blob*>& bottom,
                                               const vector<Blob*>& top) {
  LossLayer::LayerSetUp(bottom, top);
  CHECK_GE(layer->i_size, 2);
  positive_margin_ = this->layer_param_.batch_contrastive_loss_param().positive_margin();
  negative_margin_ = this->layer_param_.batch_contrastive_loss_param().negative_margin();
  positive_weight_ = this->layer_param_.batch_contrastive_loss_param().positive_weight();
  negative_weight_ = this->layer_param_.batch_contrastive_loss_param().negative_weight();
  max_only_ = this->layer_param_.batch_contrastive_loss_param().max_only();
}


void BatchContrastiveLossLayer::Reshape(const vector<Blob*>& bottom,
                                                 const vector<Blob*>& top) {
  LossLayer::Reshape(bottom, top);
  CHECK_EQ(prev->size.n, prev->size.c);
  if (layer->o_size >= 2) {
    // positive distance, negative distance.
    top[1]->Reshape({ 2 });
  }
}


void BatchContrastiveLossLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* prevData = prevData;
  Dtype* prevDiff = bottom[0]->mutable_diff();
  const Dtype* label = nextData;
  int num = prev->size.n;
  Dtype positive_distance = (Dtype)(0);
  int positive_count = 0;
  Dtype negative_distance = (Dtype)(0);
  int negative_count = 0;
  max_positive_1_ = -1; max_positive_2_ = -1;
  min_negative_1_ = -1; min_negative_2_ = -1;
  Dtype max_positive_value = -FLT_MAX;
  Dtype min_negative_value = FLT_MAX;
  Dtype* loss = nextData;

  for (i = 0; i < num; ++i) {
    for (j = i + 1; j < num; ++j) {
      if (label[i] == label[j]) {
        positive_distance += prevData[i*num + j];
        positive_count++;
        if (prevData[i*num + j] > positive_margin_) {
          if (max_only_ && prevData[i*num + j] > max_positive_value) {
            max_positive_value = prevData[i*num + j];
            max_positive_1_ = i;
            max_positive_2_ = j;
          }
          loss[0] += positive_weight_ * (prevData[i*num + j] - positive_margin_);
        }
      }
      else {
        negative_distance += prevData[i*num + j];
        negative_count++;
        if (prevData[i*num + j] < negative_margin_) {
          if (max_only_ && prevData[i*num + j] < min_negative_value) {
            min_negative_value = prevData[i*num + j];
            min_negative_1_ = i;
            min_negative_2_ = j;
          }
          loss[0] += negative_weight_ * (negative_margin_ - prevData[i*num + j]);
        }
      }
    }
  }
  if (max_only_) {
    loss[0] = (Dtype)(0);
    if (max_positive_1_ >= 0 && max_positive_2_ >= 0) {
      loss[0] += positive_weight_ * (prevData[max_positive_1_ * num + max_positive_2_] - positive_margin_);
    }
    if (min_negative_1_ >= 0 && min_negative_2_ >= 0) {
      loss[0] += negative_weight_ * (negative_margin_ - prevData[min_negative_1_ * num + min_negative_2_]);
    }
  }
  else {
    loss[0] /= num * (num - 1) / 2;
  }
  if (layer->o_size >= 2) {
    Dtype* distances = top[1]->mutable_data();
    distances[0] = positive_distance / positive_count;
    distances[1] = negative_distance / negative_count;
  }
}


void BatchContrastiveLossLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  if (propagate_down[1]) {
    LOG(FATAL) << this->type()
               << " Layer cannot backpropagate to label inputs.";
  }
  if (propagate_down[0]) {
    const Dtype* prevData = prevData;
    Dtype* prevDiff = bottom[0]->mutable_diff();
    const Dtype* label = nextData;
    int num = prev->size.n;

    blas_sset(num*num, (Dtype)(0), prevDiff);
    if (max_only_) {
      if (max_positive_1_ >= 0 && max_positive_2_ >= 0) {
        prevDiff[max_positive_1_ * num + max_positive_2_] = positive_weight_;
      }
      if (min_negative_1_ >= 0 && min_negative_2_ >= 0) {
        prevDiff[min_negative_1_ * num + min_negative_2_] = -negative_weight_;
      }
    }
    else {
      for (i = 0; i < num; ++i) {
        for (j = i + 1; j < num; ++j) {
          if (label[i] == label[j]) {
            if (prevData[i*num + j] > positive_margin_) {
              prevDiff[i*num + j] = positive_weight_;
            }
          }
          else {
            if (prevData[i*num + j] < negative_margin_) {
              prevDiff[i*num + j] = -negative_weight_;
            }
          }
        }
      }
    }

    const Dtype loss_weight = nextDiff[0];
    if (max_only_) {
      blas_sscal(Data_count(prev->size), loss_weight / 2, prevDiff);
    }
    else {
      blas_sscal(Data_count(prev->size), loss_weight / num, prevDiff);
    }
  }
}

INSTANTIATE_CLASS(BatchContrastiveLossLayer);
REGISTER_LAYER_CLASS(BatchContrastiveLoss);


