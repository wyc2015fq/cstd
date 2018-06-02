

/**
 * @brief Computes @f$ y = log_{\gamma}(\alpha x + \beta) @f$,
 *        as specified by the scale @f$ \alpha @f$, shift @f$ \beta @f$,
 *        and base @f$ \gamma @f$.
 */

class LogLayer {
 public:
  /**
   * @param param provides LogParameter log_param,
   *     with LogLayer options:
   *   - scale (\b optional, default 1) the scale @f$ \alpha @f$
   *   - shift (\b optional, default 0) the shift @f$ \beta @f$
   *   - base (\b optional, default -1 for a value of @f$ e \approx 2.718 @f$)
   *         the base @f$ \gamma @f$
   */
  explicit LogLayer(const LayerParameter& param)
      : NeuronLayer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "Log"; }

 protected:
  /**
   * @param bottom input Blob vector (length 1)
   *   -# @f$ (N \times C \times H \times W) @f$
   *      the inputs @f$ x @f$
   * @param top output Blob vector (length 1)
   *   -# @f$ (N \times C \times H \times W) @f$
   *      the computed outputs @f$
   *        y = log_{\gamma}(\alpha x + \beta)
   *      @f$
   */
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  /**
   * @brief Computes the error gradient w.r.t. the exp inputs.
   *
   * @param top output Blob vector (length 1), providing the error gradient with
   *      respect to the outputs
   *   -# @f$ (N \times C \times H \times W) @f$
   *      containing error gradients @f$ \frac{\partial E}{\partial y} @f$
   *      with respect to computed outputs @f$ y @f$
   * @param propagate_down see Layer::Backward.
   * @param bottom input Blob vector (length 1)
   *   -# @f$ (N \times C \times H \times W) @f$
   *      the inputs @f$ x @f$; Backward fills their diff with
   *      gradients @f$
   *        \frac{\partial E}{\partial x} =
   *            \frac{\partial E}{\partial y} y \alpha \log_e(gamma)
   *      @f$ if propagate_down[0]
   */
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  Dtype base_scale_;
  Dtype input_scale_, input_shift_;
  Dtype backward_num_scale_;
};





void LogLayer::LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  NeuronLayer::LayerSetUp(bottom, top);
  const Dtype base = this->layer_param_.log_param().base();
  if (base != (Dtype)(-1)) {
    CHECK_GT(base, 0) << "base must be strictly positive.";
  }
  // If base == -1, interpret the base as e and set log_base = 1 exactly.
  // Otherwise, calculate its log explicitly.
  const Dtype log_base = (base == (Dtype)(-1)) ? (Dtype)(1) : log(base);
  CHECK(!isnan(log_base))
      << "NaN result: log(base) = log(" << base << ") = " << log_base;
  CHECK(!isinf(log_base))
      << "Inf result: log(base) = log(" << base << ") = " << log_base;
  base_scale_ = (Dtype)(1) / log_base;
  CHECK(!isnan(base_scale_))
      << "NaN result: 1/log(base) = 1/log(" << base << ") = " << base_scale_;
  CHECK(!isinf(base_scale_))
      << "Inf result: 1/log(base) = 1/log(" << base << ") = " << base_scale_;
  input_scale_ = this->layer_param_.log_param().scale();
  input_shift_ = this->layer_param_.log_param().shift();
  backward_num_scale_ = input_scale_ / log_base;
}


void LogLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const int count = Data_count(prev->size);
  const Dtype* prevData = prevData;
  Dtype* nextData = nextData;
  if (input_scale_ == (Dtype)(1) && input_shift_ == (Dtype)(0)) {
    blas_slog(count, prevData, nextData);
  } else {
    blas_scopy(count, prevData, nextData);
    if (input_scale_ != (Dtype)(1)) {
      blas_sscal(count, input_scale_, nextData);
    }
    if (input_shift_ != (Dtype)(0)) {
      blas_sadd_scalar(count, input_shift_, nextData);
    }
    blas_slog(count, nextData, nextData);
  }
  if (base_scale_ != (Dtype)(1)) {
    blas_sscal(count, base_scale_, nextData);
  }
}


void LogLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  if (!propagate_down[0]) { return; }
  const int count = Data_count(prev->size);
  const Dtype* prevData = prevData;
  const Dtype* nextDiff = nextDiff;
  Dtype* prevDiff = bottom[0]->mutable_diff();
  blas_scopy(count, prevData, prevDiff);
  if (input_scale_ != (Dtype)(1)) {
    blas_sscal(count, input_scale_, prevDiff);
  }
  if (input_shift_ != (Dtype)(0)) {
    blas_sadd_scalar(count, input_shift_, prevDiff);
  }
  blas_spowx(count, prevDiff, (Dtype)(-1), prevDiff);
  if (backward_num_scale_ != (Dtype)(1)) {
    blas_sscal(count, backward_num_scale_, prevDiff);
  }
  blas_smul(count, nextDiff, prevDiff, prevDiff);
}

#ifdef CPU_ONLY
STUB_GPU(LogLayer);
#endif

INSTANTIATE_CLASS(LogLayer);
REGISTER_LAYER_CLASS(Log);


