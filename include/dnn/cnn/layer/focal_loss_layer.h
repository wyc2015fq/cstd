/*
Based on https://github.com/zimenglan-sysu-512/Focal-Loss.
Paper https://arxiv.org/abs/1708.02002.
NOTICE: This layer is NOT the original focal loss layer.
I changed 1-p to 1+p for face verification. If you want the original version, 
replace the cpp and cu files from https://github.com/zimenglan-sysu-512/Focal-Loss.
*/



/**
* Please refer "Focal Loss for Dense Object Detection"
*/

class FocalLossLayer : public LossLayer {
public:
  explicit FocalLossLayer(const LayerParameter& param)
    : LossLayer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
                          const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
                       const vector<Blob*>& top);

  virtual inline const char* type() const { return "FocalLoss"; }
  virtual inline int ExactNumTopBlobs() const { return -1; }
  virtual inline int MinTopBlobs() const { return 1; }
  virtual inline int MaxTopBlobs() const { return 2; }

protected:
  virtual void Forward_cpu(const vector<Blob*>& bottom,
                           const vector<Blob*>& top);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
                           const vector<Blob*>& top);

  virtual void Backward_cpu(const vector<Blob*>& top,
                            const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Backward_gpu(const vector<Blob*>& top,
                            const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  virtual Dtype get_normalizer(
    LossParameter_NormalizationMode normalization_mode, int valid_count);

  void compute_intermediate_values_of_cpu();
  void compute_intermediate_values_of_gpu();

  /// The internal SoftmaxLayer used to map predictions to a distribution.
  shared_ptr<Layer > softmax_layer_;
  /// prob stores the output probability predictions from the SoftmaxLayer.
  Blob prob_;        // softmax output p_t
  Blob log_prob_;    // log(p_t)
  Blob power_prob_;  // alpha * (1 - p_t) ^ gamma
  Blob ones_;        // 1
                            /// bottom vector holder used in call to the underlying SoftmaxLayer::Forward
  vector<Blob*> softmax_bottom_vec_;
  /// top vector holder used in call to the underlying SoftmaxLayer::Forward
  vector<Blob*> softmax_top_vec_;
  /// Whether to ignore instances with a certain label.
  bool has_ignore_label_;
  /// The label indicating that an instance should be ignored.
  int ignore_label_;
  /// How to normalize the output loss.
  LossParameter_NormalizationMode normalization_;
  ///
  FocalLossParameter_Type type_;

  Dtype alpha_, beta_, gamma_;
  int softmax_axis_, outer_num_, inner_num_;
  bool one_add_p_;
};





void FocalLossLayer::LayerSetUp(
  const vector<Blob*>& bottom, const vector<Blob*>& top) {
  // softmax laye setup
  LossLayer::LayerSetUp(bottom, top);
  LayerParameter softmax_param(this->layer_param_);
  softmax_param.set_type("Softmax");
  softmax_layer_ = LayerRegistry::CreateLayer(softmax_param);
  softmax_bottom_vec_.clear();
  softmax_bottom_vec_.push_back(bottom[0]);
  softmax_top_vec_.clear();
  softmax_top_vec_.push_back(&prob_);
  softmax_layer_->SetUp(softmax_bottom_vec_, softmax_top_vec_);

  // ignore label
  has_ignore_label_ = this->layer_param_.loss_param().has_ignore_label();
  if (has_ignore_label_) {
    ignore_label_ = this->layer_param_.loss_param().ignore_label();
  }

  // normalization
  if (!this->layer_param_.loss_param().has_normalization() &&
      this->layer_param_.loss_param().has_normalize()) {
    normalization_ = this->layer_param_.loss_param().normalize() ?
      LossParameter_NormalizationMode_VALID :
      LossParameter_NormalizationMode_BATCH_SIZE;
  }
  else {
    normalization_ = this->layer_param_.loss_param().normalization();
  }

  // focal loss parameter
  FocalLossParameter focal_loss_param = this->layer_param_.focal_loss_param();
  alpha_ = focal_loss_param.alpha();
  beta_ = focal_loss_param.beta();
  gamma_ = focal_loss_param.gamma();
  type_ = focal_loss_param.type();
  LOG(INFO) << "alpha: " << alpha_;
  LOG(INFO) << "beta: " << beta_;
  LOG(INFO) << "gamma: " << gamma_;
  LOG(INFO) << "type: " << type_;
  CHECK_GE(gamma_, 0) << "gamma must be larger than or equal to zero";
  CHECK_GT(alpha_, 0) << "alpha must be larger than zero";
  // CHECK_LE(alpha_, 1) << "alpha must be smaller than or equal to one";
}


void FocalLossLayer::Reshape(
  const vector<Blob*>& bottom, const vector<Blob*>& top) {
  // softmax laye reshape
  LossLayer::Reshape(bottom, top);
  softmax_layer_->Reshape(softmax_bottom_vec_, softmax_top_vec_);

  // cross-channels
  softmax_axis_ = bottom[0]->CanonicalAxisIndex(this->layer_param_.softmax_param().axis());
  outer_num_ = bottom[0]->count(0, softmax_axis_);
  inner_num_ = bottom[0]->count(softmax_axis_ + 1);
  CHECK_EQ(outer_num_ * inner_num_, bottom[1]->count())
    << "Number of labels must match number of predictions; "
    << "e.g., if softmax axis == 1 and prediction shape is (N, C, H, W), "
    << "label count (number of labels) must be N*H*W, "
    << "with integer values in {0, 1, ..., C-1}.";

  // softmax output
  if (layer->o_size >= 2) {
    top[1]->ReshapeLike(*bottom[0]);
  }

  // log(p_t)
  log_prob_.ReshapeLike(*bottom[0]);
  CHECK_EQ(prob_.count(), log_prob_.count());
  // (1 + p_t) ^ gamma
  power_prob_.ReshapeLike(*bottom[0]);
  CHECK_EQ(prob_.count(), power_prob_.count());
  // 1
  ones_.ReshapeLike(*bottom[0]);
  CHECK_EQ(prob_.count(), ones_.count());
  blas_sset(prob_.count(), (Dtype)(alpha_), ones_);
}


Dtype FocalLossLayer::get_normalizer(
  LossParameter_NormalizationMode normalization_mode, int valid_count) {
  Dtype normalizer;
  switch (normalization_mode) {
  case LossParameter_NormalizationMode_FULL:
    normalizer = (Dtype)(outer_num_ * inner_num_);
    break;
  case LossParameter_NormalizationMode_VALID:
    if (valid_count == -1) {
      normalizer = (Dtype)(outer_num_ * inner_num_);
    }
    else {
      normalizer = (Dtype)(valid_count);
    }
    break;
  case LossParameter_NormalizationMode_BATCH_SIZE:
    normalizer = (Dtype)(outer_num_);
    break;
  case LossParameter_NormalizationMode_NONE:
    normalizer = (Dtype)(1);
    break;
  default:
    LOG(FATAL) << "Unknown normalization mode: "
      << LossParameter_NormalizationMode_Name(normalization_mode);
  }
  // Some users will have no labels for some examples in order to 'turn off' a
  // particular loss in a multi-task setup. The max prevents NaNs in that case.
  return MAX(Dtype(1.0), normalizer);
}


void FocalLossLayer::compute_intermediate_values_of() {
  // compute the corresponding variables
  const int count = prob_.count();
  const Dtype* prob_data = prob_.cpu_data();
  const Dtype* ones_data = ones_.cpu_data();
  Dtype* log_prob_data = log_prob_;
  Dtype* power_prob_data = power_prob_;

  /// log(p_t)
  const Dtype eps = (Dtype)(FLT_MIN); // where FLT_MIN = 1.17549e-38, here u can change it
                                    // more stable
  for (i = 0; i < prob_.count(); i++) {
    log_prob_data[i] = log(MAX(prob_data[i], eps));
  }
  /// blas_slog(count,  prob_data, log_prob_data);

  if (type_ == FocalLossParameter::ONEADDP) {
    /// (1 + p_t) ^ gamma
    blas_sadd(count, ones_data, prob_data, power_prob_data);
  }
  else {
    /// (1 - p_t) ^ gamma
    blas_ssub(count, ones_data, prob_data, power_prob_data);
  }
  blas_spowx(count, power_prob_.cpu_data(), gamma_, power_prob_data);
  //blas_sscal(count, alpha_, power_prob_data);
}


void FocalLossLayer::Forward(
  const vector<Blob*>& bottom, const vector<Blob*>& top) {
  // The forward pass computes the softmax prob values.
  softmax_layer_->Forward(softmax_bottom_vec_, softmax_top_vec_);

  // compute all needed values
  compute_intermediate_values_of();
  const Dtype* label = nextData;
  const Dtype* log_prob_data = log_prob_.cpu_data();
  const Dtype* power_prob_data = power_prob_.cpu_data();

  // compute loss
  int count = 0;
  int channels = prob_.shape(softmax_axis_);
  int dim = prob_.count() / outer_num_;

  Dtype loss = 0;
  for (i = 0; i < outer_num_; ++i) {
    for (j = 0; j < inner_num_; j++) {
      const int label_value = (int)(label[i * inner_num_ + j]);
      if (has_ignore_label_ && label_value == ignore_label_) {
        continue;
      }
      DCHECK_GE(label_value, 0);
      DCHECK_LT(label_value, channels);
      const int index = i * dim + label_value * inner_num_ + j;
      // FL(p_t) = -(1 + p_t) ^ gamma * log(p_t)
      // loss -= MAX(power_prob_data[index] * log_prob_data[index],
      //                      (Dtype)(log(Dtype(FLT_MIN))));
      loss -= power_prob_data[index] * log_prob_data[index];
      ++count;
    }
  }

  // prob
  nextData[0] = loss / get_normalizer(normalization_, count);
  if (layer->o_size == 2) {
    top[1]->ShareData(prob_);
  }
}


void FocalLossLayer::Backward(const vector<Blob*>& top,
                                         const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  if (propagate_down[1]) {
    LOG(FATAL) << this->type()
      << " Layer cannot backpropagate to label inputs.";
  }

  if (propagate_down[0]) {
    // data
    Dtype* prevDiff = bottom[0]->mutable_diff();
    const Dtype* prob_data = prob_.cpu_data();
    const Dtype* label = nextData;
    // intermidiate  
    const Dtype* log_prob_data = log_prob_.cpu_data();
    const Dtype* power_prob_data = power_prob_.cpu_data();

    int count = 0;
    int channels = bottom[0]->shape(softmax_axis_);
    int dim = prob_.count() / outer_num_;
    const Dtype eps = 1e-10;

    for (i = 0; i < outer_num_; ++i) {
      for (j = 0; j < inner_num_; ++j) {
        // label
        const int label_value = (int)(label[i * inner_num_ + j]);

        // ignore label
        if (has_ignore_label_ && label_value == ignore_label_) {
          for (c = 0; c < channels; ++c) {
            prevDiff[i * dim + c * inner_num_ + j] = 0;
          }
          continue;
        }

        // the gradient from FL w.r.t p_t, here ignore the `sign`
        int ind_i = i * dim + label_value * inner_num_ + j; // index of ground-truth label
        Dtype grad;
        if (type_ == FocalLossParameter::ONEADDP) {
          grad = gamma_ * (power_prob_data[ind_i] / MAX(1 + prob_data[ind_i], eps))
            * log_prob_data[ind_i] * prob_data[ind_i]
            + power_prob_data[ind_i];
        }
        else {
          grad = -gamma_ * (power_prob_data[ind_i] / MAX(1 - prob_data[ind_i], eps))
            * log_prob_data[ind_i] * prob_data[ind_i]
            + power_prob_data[ind_i];
        }
        
        // the gradient w.r.t input data x
        for (c = 0; c < channels; ++c) {
          int ind_j = i * dim + c * inner_num_ + j;
          if (c == label_value) {
            CHECK_EQ(ind_i, ind_j);
            // if i == j, (here i,j are refered for derivative of softmax)
            prevDiff[ind_j] = grad * (prob_data[ind_i] - 1);
          }
          else {
            // if i != j, (here i,j are refered for derivative of softmax)
            prevDiff[ind_j] = grad * prob_data[ind_j];
          }
        }
        // count                    
        ++count;
      }
    }
    // Scale gradient
    Dtype loss_weight = nextDiff[0] / get_normalizer(normalization_, count);
    blas_sscal(prob_.count(), loss_weight, prevDiff);
  }
}


