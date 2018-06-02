


/**
 * @brief Parameterized Rectified Linear Unit non-linearity @f$
 *        y_i = \max(0, x_i) + a_i \min(0, x_i)
 *        @f$. The differences from ReLULayer are 1) negative slopes are
 *        learnable though backprop and 2) negative slopes can vary across
 *        channels. The number of axes of input blob should be greater than or
 *        equal to 2. The 1st axis (0-based) is seen as channels.
 */

class PReLULayer {
 public:
  /**
   * @param param provides PReLUParameter prelu_param,
   *     with PReLULayer options:
   *   - filler (\b optional, FillerParameter,
   *     default {'type': constant 'value':0.25}).
   *   - channel_shared (\b optional, default false).
   *     negative slopes are shared across channels.
   */
  explicit PReLULayer(const LayerParameter& param)
      : NeuronLayer(param) {}

  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "PReLU"; }

 protected:
  /**
   * @param bottom input Blob vector (length 1)
   *   -# @f$ (N \times C \times ...) @f$
   *      the inputs @f$ x @f$
   * @param top output Blob vector (length 1)
   *   -# @f$ (N \times C \times ...) @f$
   *      the computed outputs for each channel @f$i@f$ @f$
   *        y_i = \max(0, x_i) + a_i \min(0, x_i)
   *      @f$.
   */
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  /**
   * @brief Computes the error gradient w.r.t. the PReLU inputs.
   *
   * @param top output Blob vector (length 1), providing the error gradient with
   *      respect to the outputs
   *   -# @f$ (N \times C \times ...) @f$
   *      containing error gradients @f$ \frac{\partial E}{\partial y} @f$
   *      with respect to computed outputs @f$ y @f$
   * @param propagate_down see Layer::Backward.
   * @param bottom input Blob vector (length 1)
   *   -# @f$ (N \times C \times ...) @f$
   *      the inputs @f$ x @f$; For each channel @f$i@f$, backward fills their
   *      diff with gradients @f$
   *        \frac{\partial E}{\partial x_i} = \left\{
   *        \begin{array}{lr}
   *            a_i \frac{\partial E}{\partial y_i} & \mathrm{if} \; x_i \le 0 \\
   *            \frac{\partial E}{\partial y_i} & \mathrm{if} \; x_i > 0
   *        \end{array} \right.
   *      @f$.
   *      If param_propagate_down_[0] is true, it fills the diff with gradients
   *      @f$
   *        \frac{\partial E}{\partial a_i} = \left\{
   *        \begin{array}{lr}
   *            \sum_{x_i} x_i \frac{\partial E}{\partial y_i} & \mathrm{if} \; x_i \le 0 \\
   *            0 & \mathrm{if} \; x_i > 0
   *        \end{array} \right.
   *      @f$.
   */
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  bool channel_shared_;
  Blob multiplier_;  // dot multiplier for backward computation of params
  Blob backward_buff_;  // temporary buffer for backward computation
  Blob bottom_memory_;  // memory for in-place computation
};




void PReLULayer::LayerSetUp(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  CHECK_GE(bottom[0]->num_axes(), 2)
      << "Number of axes of bottom blob must be >=2.";
  PReLUParameter prelu_param = this->layer_param().prelu_param();
  int channels = prev->size.c;
  channel_shared_ = prelu_param.channel_shared();
  if (this->blobs_.size() > 0) {
    LOG(INFO) << "Skipping parameter initialization";
  } else {
    this->blobs_.resize(1);
    if (channel_shared_) {
      this->blobs_[0].reset(new Blob(vector<int>(0)));
    } else {
      this->blobs_[0].reset(new Blob(vector<int>(1, channels)));
    }
    shared_ptr<Filler > filler;
    if (prelu_param.has_filler()) {
      filler.reset(GetFiller(prelu_param.filler()));
    } else {
      FillerParameter filler_param;
      filler_param.set_type("constant");
      filler_param.set_value(0.25);
      filler.reset(GetFiller(filler_param));
    }
    filler->Fill(this->blobs_[0].get());
  }
  if (channel_shared_) {
    CHECK_EQ(this->blobs_[0]->count(), 1)
        << "Negative slope size is inconsistent with prototxt config";
  } else {
    CHECK_EQ(this->blobs_[0]->count(), channels)
        << "Negative slope size is inconsistent with prototxt config";
  }

  // Propagate gradients to the parameters (as directed by backward pass).
  this->param_propagate_down_.resize(this->blobs_.size(), true);
  multiplier_.Reshape(vector<int>(1, bottom[0]->count(1)));
  backward_buff_.Reshape(vector<int>(1, bottom[0]->count(1)));
  blas_sset(multiplier_.count(), (Dtype)(1), multiplier_);
}


void PReLULayer::Reshape(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  CHECK_GE(bottom[0]->num_axes(), 2)
      << "Number of axes of bottom blob must be >=2.";
  top[0]->ReshapeLike(*bottom[0]);
  if (bottom[0] == top[0]) {
    // For in-place computation
    bottom_memory_.ReshapeLike(*bottom[0]);
  }
}


void PReLULayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* prevData = prevData;
  Dtype* nextData = nextData;
  const int count = Data_count(prev->size);
  const int dim = bottom[0]->count(2);
  const int channels = prev->size.c;
  const Dtype* slope_data = this->blobs_[0]->cpu_data();

  // For in-place computation
  if (bottom[0] == top[0]) {
    blas_scopy(count, prevData, bottom_memory_);
  }

  // if channel_shared, channel index in the following computation becomes
  // always zero.
  const int div_factor = channel_shared_ ? channels : 1;
  for (i = 0; i < count; ++i) {
    int c = (i / dim) % channels / div_factor;
    nextData[i] = MAX(prevData[i], (Dtype)(0))
        + slope_data[c] * std::min(prevData[i], (Dtype)(0));
  }
}


void PReLULayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
  const Dtype* prevData = prevData;
  const Dtype* slope_data = this->blobs_[0]->cpu_data();
  const Dtype* nextDiff = nextDiff;
  const int count = Data_count(prev->size);
  const int dim = bottom[0]->count(2);
  const int channels = prev->size.c;

  // For in-place computation
  if (top[0] == bottom[0]) {
    prevData = bottom_memory_.cpu_data();
  }

  // if channel_shared, channel index in the following computation becomes
  // always zero.
  const int div_factor = channel_shared_ ? channels : 1;

  // Propagte to param
  // Since to write bottom diff will affect top diff if top and bottom blobs
  // are identical (in-place computaion), we first compute param backward to
  // keep nextDiff unchanged.
  if (this->param_propagate_down_[0]) {
    Dtype* slope_diff = this->blobs_[0]->mutable_diff();
    for (i = 0; i < count; ++i) {
      int c = (i / dim) % channels / div_factor;
      slope_diff[c] += nextDiff[i] * prevData[i] * (prevData[i] <= 0);
    }
  }
  // Propagate to bottom
  if (propagate_down[0]) {
    Dtype* prevDiff = bottom[0]->mutable_diff();
    for (i = 0; i < count; ++i) {
      int c = (i / dim) % channels / div_factor;
      prevDiff[i] = nextDiff[i] * ((prevData[i] > 0)
          + slope_data[c] * (prevData[i] <= 0));
    }
  }
}


#ifdef CPU_ONLY
STUB_GPU(PReLULayer);
#endif

INSTANTIATE_CLASS(PReLULayer);
REGISTER_LAYER_CLASS(PReLU);


