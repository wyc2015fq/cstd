




#include "general_contrastive_loss_layer.hpp"




void GeneralContrastiveLossLayer::LayerSetUp(const vector<Blob*>& bottom,
                                               const vector<Blob*>& top) {
  LossLayer::LayerSetUp(bottom, top);
  CHECK_GE(layer->i_size, 2);
  positive_margin_ = this->layer_param_.general_contrastive_loss_param().positive_margin();
  negative_margin_ = this->layer_param_.general_contrastive_loss_param().negative_margin();
  positive_weight_ = this->layer_param_.general_contrastive_loss_param().positive_weight();
  negative_weight_ = this->layer_param_.general_contrastive_loss_param().negative_weight();
  need_normalize_negative_ = this->layer_param_.general_contrastive_loss_param().has_normalize_negative();
  negative_gradient_norm_ = this->layer_param_.general_contrastive_loss_param().normalize_negative();
  positive_outlier_thresh_ = this->layer_param_.general_contrastive_loss_param().positive_outlier_thresh();
  max_negative_only_ = this->layer_param_.general_contrastive_loss_param().max_negative_only();
  max_positive_only_ = this->layer_param_.general_contrastive_loss_param().max_positive_only();
  positive_first_ = this->layer_param_.general_contrastive_loss_param().positive_first();
  positive_upper_bound_ = this->layer_param_.general_contrastive_loss_param().positive_upper_bound();
  exp_negative_weight_ = this->layer_param_.general_contrastive_loss_param().exp_negative_weight();
  add_intra_mae_ = this->layer_param_.general_contrastive_loss_param().add_intra_mae();
  max_negative_margin_ = this->layer_param_.general_contrastive_loss_param().max_negative_margin();
  intra_mae_ = (Dtype)(0);
}


void GeneralContrastiveLossLayer::Reshape(const vector<Blob*>& bottom,
                                                 const vector<Blob*>& top) {
  LossLayer::Reshape(bottom, top);
  if (layer->o_size >= 2) {
    if (add_intra_mae_) {
      // positive distance, negative distance, intra_mae.
      top[1]->Reshape({ 3 });
    }
    else {
      // positive distance, negative distance.
      top[1]->Reshape({ 2 });
    }
  }
  if (max_negative_only_) {
    max_negative_index_.Reshape({ prev->size.n });
  }
}


void GeneralContrastiveLossLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* prevData = (layer->i_size == 3) ? getData(prev2) : prevData;
  Dtype* prevDiff = bottom[0]->mutable_diff();
  const Dtype* label = nextData;
  int* max_negative_index_data = NULL;
  if(max_negative_only_ ) max_negative_index_data = max_negative_index_;
  int num = prev->size.n;
  int count = Data_count(prev->size);
  int dim = count / num;
  Dtype weighted_count = num * (abs(positive_weight_) + (dim - 1)*abs(negative_weight_));
  Dtype positive_distance = (Dtype)(0);
  Dtype negative_distance = (Dtype)(0);
  max_positive_index_ = 0;
  if (add_intra_mae_) negative_margin_ = intra_mae_ + this->layer_param_.general_contrastive_loss_param().negative_margin();

  for (i = 0; i < num; ++i) {
    Dtype same_distance = prevData[i * dim + (int)(label[i])];
    if(max_negative_only_) max_negative_index_data[i] = 0;
    for (j = 0; j < dim; ++j) {
      if (j == (int)(label[i])) {
        Dtype truncated_value = std::min(positive_outlier_thresh_, MAX(Dtype(0), prevData[i * dim + j] - positive_margin_));
        prevDiff[i * dim + j] = truncated_value * positive_weight_;
        positive_distance += truncated_value;
        if (max_positive_only_ && prevDiff[i * dim + j] < prevDiff[max_positive_index_*dim + (int)(label[max_positive_index_])]) {
          max_positive_index_ = i;
        }
      }
      else {
        if (prevData[i * dim + j] < negative_margin_) {
          if (exp_negative_weight_) {
            prevDiff[i * dim + j] = exp(-prevData[i * dim + j]) * negative_weight_;
          }
          else {
            prevDiff[i * dim + j] = MAX(
              (Dtype)(0), negative_margin_ - prevData[i * dim + j]) * negative_weight_;
          }
          negative_distance += MAX(
            (Dtype)(0), negative_margin_ - prevData[i * dim + j]);
          if (max_negative_only_ && prevDiff[i * dim + j] > prevDiff[i*dim + max_negative_index_data[i]]) {
            max_negative_index_data[i] = j;
          }
        }
        else {
          prevDiff[i*dim + j] = 0;
        }
      }
    }
    if (positive_first_ && same_distance > positive_upper_bound_) {
      for (j = 0; j < dim; ++j) {
        if (j != (int)(label[i])) {
          prevDiff[i * dim + j] = (Dtype)(0);
        }
      }
    }
  }
  if (max_positive_only_) {
    for (i = 0; i < num; ++i) {
      for (j = 0; j < dim; ++j) {
        if (j == (int)(label[i]) && i != max_positive_index_) {
          prevDiff[i*dim + j] = 0;
        }
      }
    }
  }
  if (max_negative_only_) {
    for (i = 0; i < num; ++i) {
      for (j = 0; j < dim; ++j) {
        if (j != (int)(label[i]) && j != max_negative_index_data[i]) {
          prevDiff[i*dim + j] = 0;
        }
      }
    }
  }
  intra_mae_ = 0.99 * intra_mae_ + 0.01 * positive_distance / num;
  if (intra_mae_ > max_negative_margin_) intra_mae_ = max_negative_margin_;

  Dtype* loss = nextData;
  loss[0] = blas_sasum(count, prevDiff) / num;
  if (layer->o_size >= 2) {
    Dtype* distances = top[1]->mutable_data();
    distances[0] = positive_distance / num;
    distances[1] = negative_distance / num / (dim - 1);
    if (add_intra_mae_) {
      distances[2] = intra_mae_;
    }
  }
}


void GeneralContrastiveLossLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  if (propagate_down[1]) {
    LOG(FATAL) << this->type()
               << " Layer cannot backpropagate to label inputs.";
  }
  if (propagate_down[0]) {
    const Dtype* prevData = (layer->i_size == 3) ? getData(prev2) : prevData;
    Dtype* prevDiff = bottom[0]->mutable_diff();
    const Dtype* label = nextData;
    const int* max_negative_index_data = NULL;
    if (max_negative_only_) max_negative_index_data = max_negative_index_.cpu_data();
    int num = prev->size.n;
    int count = Data_count(prev->size);
    int dim = count / num;

    Dtype negative_sum = (Dtype)(0);

    for (i = 0; i < num; ++i) {
      Dtype same_distance = prevData[i * dim + (int)(label[i])];
      if (positive_first_ && same_distance > positive_upper_bound_) {
        prevDiff[i * dim + (int)(label[i])] = positive_weight_;
        for (j = 0; j < dim; ++j) {
          if (j != (int)(label[i])) {
            prevDiff[i * dim + j] = (Dtype)(0);
          }
        }
        continue;
      }
      for (j = 0; j < dim; ++j) {
        if (j == (int)(label[i])) {
          if (prevData[i * dim + j] > positive_margin_ && prevData[i * dim + j] < positive_outlier_thresh_
              && !(max_positive_only_ && i != max_positive_index_)) {
            prevDiff[i * dim + j] = positive_weight_;
          }
          else {
            prevDiff[i * dim + j] = 0;
          }
        }
        else {
          if (prevData[i * dim + j] < negative_margin_) {
            if (max_negative_only_ && j != max_negative_index_data[i]) {
              prevDiff[i * dim + j] = 0;
            }
            else {
              if (exp_negative_weight_) {
                prevDiff[i * dim + j] = -1 * exp(-prevData[i * dim + j]) * negative_weight_;
                negative_sum += exp(-prevData[i * dim + j]) * negative_weight_;
              }
              else {
                prevDiff[i * dim + j] = -1 * negative_weight_;
                negative_sum += negative_weight_;
              }
              
            }
          }
          else{
            prevDiff[i * dim + j] = 0;
          }
        }
      }
    }

    if (need_normalize_negative_) {
      negative_sum = abs(negative_sum) / negative_gradient_norm_ / num;

      if (negative_sum > positive_weight_) {
        for (i = 0; i < num; ++i) {
          for (j = 0; j < dim; ++j) {
            if (j != (int)(label[i])) {
              prevDiff[i * dim + j] /= negative_sum;
            }
          }
        }
      }
    } 

    const Dtype loss_weight = nextDiff[0];
    blas_sscal(count, loss_weight / num, prevDiff);
  }
}

INSTANTIATE_CLASS(GeneralContrastiveLossLayer);
REGISTER_LAYER_CLASS(GeneralContrastiveLoss);


