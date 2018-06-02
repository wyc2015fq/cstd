

/**
 * @brief Computes the multinomial logistic loss for a one-of-many
 *        classification task, passing real-valued predictions through a
 *        softmax to get a probability distribution over classes.
 *
 * This layer should be preferred over separate
 * SoftmaxLayer + MultinomialLogisticLossLayer
 * as its gradient computation is more numerically stable.
 * At test time, this layer can be replaced simply by a SoftmaxLayer.
 *
 * @param bottom input Blob vector (length 2)
 *   -# @f$ (N \times C \times H \times W) @f$
 *      the predictions @f$ x @f$, a Blob with values in
 *      @f$ [-\infty, +\infty] @f$ indicating the predicted score for each of
 *      the @f$ K = CHW @f$ classes. This layer maps these scores to a
 *      probability distribution over classes using the softmax function
 *      @f$ \hat{p}_{nk} = \exp(x_{nk}) /
 *      \left[\sum_{k'} \exp(x_{nk'})\right] @f$ (see SoftmaxLayer).
 *   -# @f$ (N \times 1 \times 1 \times 1) @f$
 *      the labels @f$ l @f$, an integer-valued Blob with values
 *      @f$ l_n \in [0, 1, 2, ..., K - 1] @f$
 *      indicating the correct class label among the @f$ K @f$ classes
 * @param top output Blob vector (length 1)
 *   -# @f$ (1 \times 1 \times 1 \times 1) @f$
 *      the computed cross-entropy classification loss: @f$ E =
 *        \frac{-1}{N} \sum\limits_{n=1}^N \log(\hat{p}_{n,l_n})
 *      @f$, for softmax output class probabilites @f$ \hat{p} @f$
 */

class SoftmaxWithLossLayer : public LossLayer {
 public:
   /**
    * @param param provides LossParameter loss_param, with options:
    *  - ignore_label (optional)
    *    Specify a label value that should be ignored when computing the loss.
    *  - normalize (optional, default true)
    *    If true, the loss is normalized by the number of (nonignored) labels
    *    present; otherwise the loss is simply summed over spatial locations.
    */
  explicit SoftmaxWithLossLayer(const LayerParameter& param)
      : LossLayer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "SoftmaxWithLoss"; }
  virtual inline int ExactNumBottomBlobs() const { return -1; }
  virtual inline int MinBottomBlobs() const { return 2; }
  virtual inline int MaxBottomBlobs() const { return 3; }
  virtual inline int ExactNumTopBlobs() const { return -1; }
  virtual inline int MinTopBlobs() const { return 1; }
  virtual inline int MaxTopBlobs() const { return 2; }

 protected:
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  /**
   * @brief Computes the softmax loss error gradient w.r.t. the predictions.
   *
   * Gradients cannot be computed with respect to the label inputs (bottom[1]),
   * so this method ignores bottom[1] and requires !propagate_down[1], crashing
   * if propagate_down[1] is set.
   *
   * @param top output Blob vector (length 1), providing the error gradient with
   *      respect to the outputs
   *   -# @f$ (1 \times 1 \times 1 \times 1) @f$
   *      This Blob's diff will simply contain the loss_weight* @f$ \lambda @f$,
   *      as @f$ \lambda @f$ is the coefficient of this layer's output
   *      @f$\ell_i@f$ in the overall Net loss
   *      @f$ E = \lambda_i \ell_i + \mbox{other loss terms}@f$; hence
   *      @f$ \frac{\partial E}{\partial \ell_i} = \lambda_i @f$.
   *      (*Assuming that this top Blob is not used as a bottom (input) by any
   *      other layer of the Net.)
   * @param propagate_down see Layer::Backward.
   *      propagate_down[1] must be false as we can't compute gradients with
   *      respect to the labels.
   * @param bottom input Blob vector (length 2)
   *   -# @f$ (N \times C \times H \times W) @f$
   *      the predictions @f$ x @f$; Backward computes diff
   *      @f$ \frac{\partial E}{\partial x} @f$
   *   -# @f$ (N \times 1 \times 1 \times 1) @f$
   *      the labels -- ignored as we can't compute their error gradients
   */
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  /// Read the normalization mode parameter and compute the normalizer based
  /// on the blob size.  If normalization_mode is VALID, the count of valid
  /// outputs will be read from valid_count, unless it is -1 in which case
  /// all outputs are assumed to be valid.
  virtual Dtype get_normalizer(
      LossParameter_NormalizationMode normalization_mode, Dtype valid_count);

  /// The internal SoftmaxLayer used to map predictions to a distribution.
  shared_ptr<Layer > softmax_layer_;
  /// prob stores the output probability predictions from the SoftmaxLayer.
  Blob prob_;
  /// bottom vector holder used in call to the underlying SoftmaxLayer::Forward
  vector<Blob*> softmax_bottom_vec_;
  /// top vector holder used in call to the underlying SoftmaxLayer::Forward
  vector<Blob*> softmax_top_vec_;
  /// Whether to ignore instances with a certain label.
  bool has_ignore_label_;
  /// The label indicating that an instance should be ignored.
  int ignore_label_;
  bool has_hard_ratio_;
  float hard_ratio_;
  bool has_hard_mining_label_;
  int hard_mining_label_;
  bool has_class_weight_;
  Blob class_weight_;
  Blob counts_;
  Blob loss_;
  /// How to normalize the output loss.
  LossParameter_NormalizationMode normalization_;
  bool has_cutting_point_;
  Dtype cutting_point_;
  std::string normalize_type_;
  Dtype label_smooth_factor_;

  int softmax_axis_, outer_num_, inner_num_;
};






void SoftmaxWithLossLayer::LayerSetUp(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
  LossLayer::LayerSetUp(bottom, top);
  normalize_type_ =
    this->layer_param_.softmax_param().normalize_type();
  if (normalize_type_ == "Softmax") {
    LayerParameter softmax_param(this->layer_param_);
    softmax_param.set_type("Softmax");
    softmax_layer_ = LayerRegistry::CreateLayer(softmax_param);
    softmax_bottom_vec_.clear();
    softmax_bottom_vec_.push_back(bottom[0]);
    softmax_top_vec_.clear();
    softmax_top_vec_.push_back(&prob_);
    softmax_layer_->SetUp(softmax_bottom_vec_, softmax_top_vec_);
  }
  else if(normalize_type_ == "L2" || normalize_type_ == "L1") {
    LayerParameter normalize_param(this->layer_param_);
    normalize_param.set_type("Normalize");
    softmax_layer_ = LayerRegistry::CreateLayer(normalize_param);
    softmax_bottom_vec_.clear();
    softmax_bottom_vec_.push_back(bottom[0]);
    softmax_top_vec_.clear();
    softmax_top_vec_.push_back(&prob_);
    softmax_layer_->SetUp(softmax_bottom_vec_, softmax_top_vec_);
  }
  else {
    NOT_IMPLEMENTED;
  }

  label_smooth_factor_ = this->layer_param_.softmax_param().label_smooth_factor();
  has_ignore_label_ =
    this->layer_param_.loss_param().has_ignore_label();
  if (has_ignore_label_) {
    ignore_label_ = this->layer_param_.loss_param().ignore_label();
  }
  has_hard_ratio_ =
    this->layer_param_.softmax_param().has_hard_ratio();
  if (has_hard_ratio_) {
    hard_ratio_ = this->layer_param_.softmax_param().hard_ratio();
    CHECK_GE(hard_ratio_, 0);
    CHECK_LE(hard_ratio_, 1);
  }
  has_cutting_point_ =
    this->layer_param_.softmax_param().has_cutting_point();
  if (has_cutting_point_) {
    cutting_point_ = this->layer_param_.softmax_param().cutting_point();
    CHECK_GE(cutting_point_, 0);
    CHECK_LE(cutting_point_, 1);
  }
  has_hard_mining_label_ = this->layer_param_.softmax_param().has_hard_mining_label();
  if (has_hard_mining_label_) {
    hard_mining_label_ = this->layer_param_.softmax_param().hard_mining_label();
  }
  has_class_weight_ = (this->layer_param_.softmax_param().class_weight_size() != 0);
  softmax_axis_ =
    bottom[0]->CanonicalAxisIndex(this->layer_param_.softmax_param().axis());
  if (has_class_weight_) {
    class_weight_.Reshape({ bottom[0]->shape(softmax_axis_) });
    CHECK_EQ(this->layer_param_.softmax_param().class_weight().size(), bottom[0]->shape(softmax_axis_));
    for (int i = 0; i < bottom[0]->shape(softmax_axis_); i++) {
      class_weight_.mutable_cpu_data()[i] = (Dtype)this->layer_param_.softmax_param().class_weight(i);
    }
  }
  else {
    if (bottom.size() == 3) {
      class_weight_.Reshape({ bottom[0]->shape(softmax_axis_) });
      for (int i = 0; i < bottom[0]->shape(softmax_axis_); i++) {
        class_weight_.mutable_cpu_data()[i] = (Dtype)1.0;
      }
    }
  }
  if (!this->layer_param_.loss_param().has_normalization() &&
      this->layer_param_.loss_param().has_normalize()) {
    normalization_ = this->layer_param_.loss_param().normalize() ?
                     LossParameter_NormalizationMode_VALID :
                     LossParameter_NormalizationMode_BATCH_SIZE;
  } else {
    normalization_ = this->layer_param_.loss_param().normalization();
  }
}


void SoftmaxWithLossLayer::Reshape(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
  LossLayer::Reshape(bottom, top);
  softmax_layer_->Reshape(softmax_bottom_vec_, softmax_top_vec_);
  softmax_axis_ =
      bottom[0]->CanonicalAxisIndex(this->layer_param_.softmax_param().axis());
  outer_num_ = bottom[0]->count(0, softmax_axis_);
  inner_num_ = bottom[0]->count(softmax_axis_ + 1);
  counts_.Reshape({ outer_num_, inner_num_ });
  loss_.Reshape({ outer_num_, inner_num_ });
  CHECK_EQ(outer_num_ * inner_num_, bottom[1]->count())
      << "Number of labels must match number of predictions; "
      << "e.g., if softmax axis == 1 and prediction shape is (N, C, H, W), "
      << "label count (number of labels) must be N*H*W, "
      << "with integer values in {0, 1, ..., C-1}.";
  if (bottom.size() == 3) {
    CHECK_EQ(outer_num_ * inner_num_, bottom[2]->count())
      << "Number of loss weights must match number of label.";
  }
  if (top.size() >= 2) {
    // softmax output
    top[1]->ReshapeLike(*bottom[0]);
  }
  if (has_class_weight_) {
    CHECK_EQ(class_weight_.count(), bottom[0]->shape(1));
  }
}


Dtype SoftmaxWithLossLayer::get_normalizer(
    LossParameter_NormalizationMode normalization_mode, Dtype valid_count) {
  Dtype normalizer;
  switch (normalization_mode) {
    case LossParameter_NormalizationMode_FULL:
      normalizer = Dtype(outer_num_ * inner_num_);
      break;
    case LossParameter_NormalizationMode_VALID:
      if (valid_count == -1) {
        normalizer = Dtype(outer_num_ * inner_num_);
      } else {
        normalizer = valid_count;
      }
      break;
    case LossParameter_NormalizationMode_BATCH_SIZE:
      normalizer = Dtype(outer_num_);
      break;
    case LossParameter_NormalizationMode_NONE:
      normalizer = Dtype(1);
      break;
    default:
      LOG(FATAL) << "Unknown normalization mode: "
          << LossParameter_NormalizationMode_Name(normalization_mode);
  }
  // Some users will have no labels for some examples in order to 'turn off' a
  // particular loss in a multi-task setup. The max prevents NaNs in that case.
  return std::max(Dtype(1.0), normalizer);
}


void SoftmaxWithLossLayer::Forward_cpu(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
  // The forward pass computes the softmax prob values.
  softmax_layer_->Forward(softmax_bottom_vec_, softmax_top_vec_);
  const Dtype* prob_data = prob_.cpu_data();
  const Dtype* label = Blob_datap(top_blob->data);
  int dim = prob_.count() / outer_num_;
  Dtype count = 0;
  Dtype loss = 0;
  if (bottom.size() == 2) {
    for (int i = 0; i < outer_num_; ++i) {
      for (int j = 0; j < inner_num_; j++) {
        const int label_value = static_cast<int>(label[i * inner_num_ + j]);
        if (has_ignore_label_ && label_value == ignore_label_) {
          continue;
        }
        DCHECK_GE(label_value, 0);
        DCHECK_LT(label_value, prob_.shape(softmax_axis_));
        loss -= log(std::max(prob_data[i * dim + label_value * inner_num_ + j],
          Dtype(FLT_MIN)));
        count += 1;
      }
    }
  }
  else if(bottom.size() == 3) {
    const Dtype* weights = bottom[2]->cpu_data();
    for (int i = 0; i < outer_num_; ++i) {
      for (int j = 0; j < inner_num_; j++) {
        const int label_value = static_cast<int>(label[i * inner_num_ + j]);
        const Dtype weight_value = weights[i * inner_num_ + j] * (has_class_weight_? class_weight_.cpu_data()[label_value] : 1.0);
        if (weight_value == 0) continue;
        if (has_ignore_label_ && label_value == ignore_label_) {
          continue;
        }
        DCHECK_GE(label_value, 0);
        DCHECK_LT(label_value, prob_.shape(softmax_axis_));
        loss -= weight_value * log(std::max(prob_data[i * dim + label_value * inner_num_ + j],
          Dtype(FLT_MIN)));
        count += weight_value;
      }
    }
  }
  top[0]->mutable_cpu_data()[0] = loss / get_normalizer(normalization_, count);
  if (top.size() == 2) {
    top[1]->ShareData(prob_);
  }
}


void SoftmaxWithLossLayer::Backward_cpu(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  if (propagate_down[1]) {
    LOG(FATAL) << this->type()
               << " Layer cannot backpropagate to label inputs.";
  }
  if (propagate_down[0]) {
    Dtype* bottom_diff = bottom[0]->mutable_cpu_diff();
    const Dtype* prob_data = prob_.cpu_data();
    caffe_copy(prob_.count(), prob_data, bottom_diff);
    const Dtype* label = Blob_datap(top_blob->data);
    int dim = prob_.count() / outer_num_;
    Dtype count = 0;
    if (bottom.size() == 2) {
      for (int i = 0; i < outer_num_; ++i) {
        for (int j = 0; j < inner_num_; ++j) {
          const int label_value = static_cast<int>(label[i * inner_num_ + j]);
          if (has_ignore_label_ && label_value == ignore_label_) {
            for (int c = 0; c < bottom[0]->shape(softmax_axis_); ++c) {
              bottom_diff[i * dim + c * inner_num_ + j] = 0;
            }
          }
          else {
            bottom_diff[i * dim + label_value * inner_num_ + j] -= 1;
            count += 1;
          }
        }
      }
    }
    else if (bottom.size() == 3) {
      const Dtype* weights = bottom[2]->cpu_data();
      for (int i = 0; i < outer_num_; ++i) {
        for (int j = 0; j < inner_num_; ++j) {
          const int label_value = static_cast<int>(label[i * inner_num_ + j]);
          const Dtype weight_value = weights[i * inner_num_ + j];
          if (has_ignore_label_ && label_value == ignore_label_) {
            for (int c = 0; c < bottom[0]->shape(softmax_axis_); ++c) {
              bottom_diff[i * dim + c * inner_num_ + j] = 0;
            }
          }
          else {
            bottom_diff[i * dim + label_value * inner_num_ + j] -= 1;
            for (int c = 0; c < bottom[0]->shape(softmax_axis_); ++c) {
              bottom_diff[i * dim + c * inner_num_ + j] *= weight_value * (has_class_weight_ ? class_weight_.cpu_data()[label_value] : 1.0);
            }
            if(weight_value != 0) count += weight_value;
          }
        }
      }
    }
    // Scale gradient
    Dtype loss_weight = top[0]->cpu_diff()[0] /
                        get_normalizer(normalization_, count);
    caffe_scal(prob_.count(), loss_weight, bottom_diff);
  }
}

#ifdef CPU_ONLY
STUB_GPU(SoftmaxWithLossLayer);
#endif

INSTANTIATE_CLASS(SoftmaxWithLossLayer);
REGISTER_LAYER_CLASS(SoftmaxWithLoss);


