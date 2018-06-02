


/**
 * @brief Compute "reductions" -- operations that return a scalar output Blob
 *        for an input Blob of arbitrary size, such as the sum, absolute sum,
 *        and sum of squares.
 *
 * TODO(dox): thorough documentation for Forward, Backward, and proto params.
 */

class ReductionLayer : public Layer {
 public:
  explicit ReductionLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "Reduction"; }
  virtual inline int ExactNumBottomBlobs() const { return 1; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

 protected:
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  /// @brief the reduction operation performed by the layer
  ReductionParameter_ReductionOp op_;
  /// @brief a scalar coefficient applied to all outputs
  Dtype coeff_;
  /// @brief the index of the first input axis to reduce
  int axis_;
  /// @brief the number of reductions performed
  int num_;
  /// @brief the input size of each reduction
  int dim_;
  /// @brief a helper Blob used for summation (op_ == SUM)
  Blob sum_multiplier_;
};



void ReductionLayer::LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  op_ = this->layer_param_.reduction_param().operation();
}


void ReductionLayer::Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  axis_ = bottom[0]->CanonicalAxisIndex(
      this->layer_param_.reduction_param().axis());
  // In the output, we'll keep all axes up to the reduction axis, but
  // throw away any after that.
  // Note: currently reducing along non-tail axes is not supported; otherwise,
  // we'd need to also copy any axes following an "end_axis".
  vector<int> top_shape(bottom[0]->shape().begin(),
                        bottom[0]->shape().begin() + axis_);
  top[0]->Reshape(top_shape);
  num_ = bottom[0]->count(0, axis_);
  dim_ = bottom[0]->count(axis_);
  CHECK_EQ(num_, top[0]->count());
  if (op_ == ReductionParameter_ReductionOp_SUM ||
      op_ == ReductionParameter_ReductionOp_MEAN) {
    vector<int> sum_mult_shape(1, dim_);
    sum_multiplier_.Reshape(sum_mult_shape);
    blas_sset(dim_, (Dtype)(1), sum_multiplier_);
  }
  coeff_ = this->layer_param().reduction_param().coeff();
}


void ReductionLayer::Forward(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
  const Dtype* prevData = prevData;
  const Dtype* mult_data = NULL;
  if (sum_multiplier_.count() > 0) {
    mult_data = sum_multiplier_.cpu_data();
  }
  Dtype* nextData = nextData;
  for (i = 0; i < num_; ++i) {
    switch (op_) {
    case ReductionParameter_ReductionOp_SUM:
    case ReductionParameter_ReductionOp_MEAN:
      *nextData = blas_sdot(dim_, mult_data, prevData);
      break;
    case ReductionParameter_ReductionOp_ASUM:
      *nextData = blas_sasum(dim_, prevData);
      break;
    case ReductionParameter_ReductionOp_SUMSQ:
      *nextData = blas_sdot(dim_, prevData, prevData);
      break;
    default:
      LOG(FATAL) << "Unknown reduction op: "
          << ReductionParameter_ReductionOp_Name(op_);
    }
    prevData += dim_;
    ++nextData;
  }
  if (op_ == ReductionParameter_ReductionOp_MEAN) {
    // Reset the nextData pointer.
    nextData = nextData;
    blas_sscal(num_, (Dtype)(1) / dim_, nextData);
  }
  if (coeff_ != (Dtype)(1)) {
    // Reset the nextData pointer.
    nextData = nextData;
    blas_sscal(num_, coeff_, nextData);
  }
}


void ReductionLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  if (!propagate_down[0]) { return; }
  // Get prevData, if needed.
  const Dtype* prevData = NULL;
  switch (op_) {
  // Operations that don't need prevData
  case ReductionParameter_ReductionOp_SUM:
  case ReductionParameter_ReductionOp_MEAN:
    break;
  // Operations that need prevData
  case ReductionParameter_ReductionOp_ASUM:
  case ReductionParameter_ReductionOp_SUMSQ:
    prevData = prevData;
    break;
  default:
    LOG(FATAL) << "Unknown reduction op: "
        << ReductionParameter_ReductionOp_Name(op_);
  }
  const Dtype* nextDiff = nextDiff;
  Dtype* prevDiff = bottom[0]->mutable_diff();
  for (i = 0; i < num_; ++i) {
    Dtype bottom_coeff = (*nextDiff) * coeff_;
    if (op_ == ReductionParameter_ReductionOp_MEAN) {
      bottom_coeff /= dim_;
    }
    switch (op_) {
    case ReductionParameter_ReductionOp_SUM:
    case ReductionParameter_ReductionOp_MEAN:
      blas_sset(dim_, bottom_coeff, prevDiff);
      break;
    case ReductionParameter_ReductionOp_ASUM:
      blas_ssign(dim_, prevData, prevDiff);
      blas_sscal(dim_, bottom_coeff, prevDiff);
      break;
    case ReductionParameter_ReductionOp_SUMSQ:
      blas_sscale(dim_, 2 * bottom_coeff, prevData, prevDiff);
      break;
    default:
      LOG(FATAL) << "Unknown reduction op: "
          << ReductionParameter_ReductionOp_Name(op_);
    }
    prevData += dim_;
    prevDiff += dim_;
    ++nextDiff;
  }
}

#ifdef CPU_ONLY
STUB_GPU(ReductionLayer);
#endif

INSTANTIATE_CLASS(ReductionLayer);
REGISTER_LAYER_CLASS(Reduction);


