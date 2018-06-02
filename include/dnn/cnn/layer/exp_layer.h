

/**
 * @brief Computes @f$ y = \gamma ^ {\alpha x + \beta} @f$,
 *        as specified by the scale @f$ \alpha @f$, shift @f$ \beta @f$,
 *        and base @f$ \gamma @f$.
 */

class ExpLayer {
 public:
  /**
   * @param param provides ExpParameter exp_param,
   *     with ExpLayer options:
   *   - scale (\b optional, default 1) the scale @f$ \alpha @f$
   *   - shift (\b optional, default 0) the shift @f$ \beta @f$
   *   - base (\b optional, default -1 for a value of @f$ e \approx 2.718 @f$)
   *         the base @f$ \gamma @f$
   */
  explicit ExpLayer(const LayerParameter& param)
      : NeuronLayer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "Exp"; }

 protected:
  /**
   * @param bottom input Blob vector (length 1)
   *   -# @f$ (N \times C \times H \times W) @f$
   *      the inputs @f$ x @f$
   * @param top output Blob vector (length 1)
   *   -# @f$ (N \times C \times H \times W) @f$
   *      the computed outputs @f$
   *        y = \gamma ^ {\alpha x + \beta}
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

  Dtype inner_scale_, outer_scale_;
};






void ExpLayer::LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  NeuronLayer::LayerSetUp(bottom, top);
  const Dtype base = this->layer_param_.exp_param().base();
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
  const Dtype input_scale = this->layer_param_.exp_param().scale();
  const Dtype input_shift = this->layer_param_.exp_param().shift();
  inner_scale_ = log_base * input_scale;
  outer_scale_ = (input_shift == (Dtype)(0)) ? (Dtype)(1) :
     ( (base != (Dtype)(-1)) ? pow(base, input_shift) : exp(input_shift) );
}


void ExpLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const int count = Data_count(prev->size);
  const Dtype* prevData = prevData;
  Dtype* nextData = nextData;
  if (inner_scale_ == (Dtype)(1)) {
    blas_sexp(count, prevData, nextData);
  } else {
    blas_sscale(count, inner_scale_, prevData, nextData);
    blas_sexp(count, nextData, nextData);
  }
  if (outer_scale_ != (Dtype)(1)) {
    blas_sscal(count, outer_scale_, nextData);
  }
}


void ExpLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  if (!propagate_down[0]) { return; }
  const int count = Data_count(prev->size);
  const Dtype* nextData = top[0]->cpu_data();
  const Dtype* nextDiff = nextDiff;
  Dtype* prevDiff = bottom[0]->mutable_diff();
  blas_smul(count, nextData, nextDiff, prevDiff);
  if (inner_scale_ != (Dtype)(1)) {
    blas_sscal(count, inner_scale_, prevDiff);
  }
}



