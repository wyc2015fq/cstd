


/**
 * @brief A layer for learning "embeddings" of one-hot vector input.
 *        Equivalent to an InnerProductLayer with one-hot vectors as input, but
 *        for efficiency the input is the "hot" index of each column itself.
 *
 * TODO(dox): thorough documentation for Forward, Backward, and proto params.
 */

class EmbedLayer : public Layer {
 public:
  explicit EmbedLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "Embed"; }
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

  int M_;
  int K_;
  int N_;
  bool bias_term_;
  Blob bias_multiplier_;
};





void EmbedLayer::LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  N_ = this->layer_param_.embed_param().num_output();
  CHECK_GT(N_, 0) << "EmbedLayer num_output must be positive.";
  K_ = this->layer_param_.embed_param().input_dim();
  CHECK_GT(K_, 0) << "EmbedLayer input_dim must be positive.";
  bias_term_ = this->layer_param_.embed_param().bias_term();
  // Check if we need to set up the weights
  if (this->blobs_.size() > 0) {
    LOG(INFO) << "Skipping parameter initialization";
  } else {
    if (bias_term_) {
      this->blobs_.resize(2);
    } else {
      this->blobs_.resize(1);
    }
    // Initialize the weights --
    // transposed from InnerProductLayer for spatial locality.
    vector<int> weight_shape(2);
    weight_shape[0] = K_;
    weight_shape[1] = N_;
    this->blobs_[0].reset(new Blob(weight_shape));
    // fill the weights
    shared_ptr<Filler > weight_filler(GetFiller(
        this->layer_param_.embed_param().weight_filler()));
    weight_filler->Fill(this->blobs_[0].get());
    // If necessary, initialize and fill the bias term
    if (bias_term_) {
      vector<int> bias_shape(1, N_);
      this->blobs_[1].reset(new Blob(bias_shape));
      shared_ptr<Filler > bias_filler(GetFiller(
          this->layer_param_.embed_param().bias_filler()));
      bias_filler->Fill(this->blobs_[1].get());
    }
  }  // parameter initialization
  this->param_propagate_down_.resize(this->blobs_.size(), true);
}


void EmbedLayer::Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  // Figure out the dimensions
  M_ = Data_count(prev->size);
  vector<int> top_shape = bottom[0]->shape();
  top_shape.push_back(N_);
  top[0]->Reshape(top_shape);
  // Set up the bias multiplier
  if (bias_term_) {
    vector<int> bias_shape(1, M_);
    bias_multiplier_.Reshape(bias_shape);
    blas_sset(M_, (Dtype)(1), bias_multiplier_);
  }
}


void EmbedLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* prevData = prevData;
  const Dtype* weight = this->blobs_[0]->cpu_data();
  Dtype* nextData = nextData;
  int index;
  for (n = 0; n < M_; ++n) {
    index = (int)(prevData[n]);
    DCHECK_GE(index, 0);
    DCHECK_LT(index, K_);
    DCHECK_EQ(static_cast(index), prevData[n]) << "non-integer input";
    blas_scopy(N_, weight + index * N_, nextData + n * N_);
  }
  if (bias_term_) {
    const Dtype* bias = this->blobs_[1]->cpu_data();
    blas_sgemm(CblasNoTrans, CblasNoTrans, M_, N_, 1, (Dtype)(1),
        bias_multiplier_.cpu_data(), bias, (Dtype)(1), nextData);
  }
}


void EmbedLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  CHECK(!propagate_down[0]) << "Can't backpropagate to EmbedLayer input.";
  if (this->param_propagate_down_[0]) {
    const Dtype* nextDiff = nextDiff;
    const Dtype* prevData = prevData;
    // Gradient with respect to weight
    Dtype* weight_diff = this->blobs_[0]->mutable_diff();
    int index;
    for (n = 0; n < M_; ++n) {
      index = (int)(prevData[n]);
      DCHECK_GE(index, 0);
      DCHECK_LT(index, K_);
      DCHECK_EQ(static_cast(index), prevData[n])
          << "non-integer input";
      blas_saxpy(N_, (Dtype)(1), nextDiff + n * N_, weight_diff + index * N_);
    }
  }
  if (bias_term_ && this->param_propagate_down_[1]) {
    const Dtype* nextDiff = nextDiff;
    Dtype* bias_diff = this->blobs_[1]->mutable_diff();
    blas_sgemv(CblasTrans, M_, N_, (Dtype)(1), nextDiff,
        bias_multiplier_.cpu_data(), (Dtype)(1), bias_diff);
  }
}

#ifdef CPU_ONLY
STUB_GPU(EmbedLayer);
#endif

INSTANTIATE_CLASS(EmbedLayer);
REGISTER_LAYER_CLASS(Embed);


