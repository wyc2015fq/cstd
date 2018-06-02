


/**
 * @brief Normalizes the input to have 0-mean and/or unit (1) variance.
 *
 * TODO(dox): thorough documentation for Forward, Backward, and proto params.
 */

class MVNLayer : public Layer {
 public:
  explicit MVNLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "MVN"; }
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

  Blob mean_, variance_, temp_;

  /// sum_multiplier is used to carry out sum using BLAS
  Blob sum_multiplier_;
  Dtype eps_;
};



void MVNLayer::Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  top[0]->Reshape(prev->size.n, prev->size.c,
      prev->size.h, prev->size.w);
  mean_.Reshape(prev->size.n, prev->size.c,
      1, 1);
  variance_.Reshape(prev->size.n, prev->size.c,
      1, 1);
  temp_.Reshape(prev->size.n, prev->size.c,
      prev->size.h, prev->size.w);
  if ( this->layer_param_.mvn_param().across_channels() ) {
    sum_multiplier_.Reshape(1, prev->size.c, prev->size.h,
                            prev->size.w);
  } else {
    sum_multiplier_.Reshape(1, 1, prev->size.h, prev->size.w);
  }
  Dtype* multiplier_data = sum_multiplier_;
  blas_sset(sum_multiplier_.count(), (Dtype)(1), multiplier_data);
  eps_ = this->layer_param_.mvn_param().eps();
}


void MVNLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* prevData = prevData;
  Dtype* nextData = nextData;
  int num;
  if (this->layer_param_.mvn_param().across_channels())
    num = prev->size.n;
  else
    num = prev->size.n * prev->size.c;

  int dim = Data_count(prev->size) / num;

  // subtract mean
  blas_sgemv(CblasNoTrans, num, dim, 1. / dim, prevData,
      sum_multiplier_.cpu_data(), 0., mean_);  // EX
  blas_sgemm(CblasNoTrans, CblasNoTrans, num, dim, 1, -1.,
      mean_.cpu_data(), sum_multiplier_.cpu_data(), 0.,
      temp_);
  blas_sadd(temp_.count(), prevData, temp_.cpu_data(), nextData);  // X-EX

  if (this->layer_param_.mvn_param().normalize_variance()) {
    // compute variance using var(X) = E((X-EX)^2)
    blas_spowx(Data_count(prev->size), nextData, (Dtype)(2),
        temp_);  // (X-EX)^2
    blas_sgemv(CblasNoTrans, num, dim, 1. / dim, temp_.cpu_data(),
        sum_multiplier_.cpu_data(), 0.,
        variance_);  // E((X-EX)^2)

    // normalize variance
    blas_spowx(variance_.count(), variance_.cpu_data(), (Dtype)(0.5),
          variance_);

    blas_sadd_scalar(variance_.count(), eps_, variance_);

    blas_sgemm(CblasNoTrans, CblasNoTrans, num, dim, 1, 1.,
          variance_.cpu_data(), sum_multiplier_.cpu_data(), 0.,
          temp_);

    blas_sdiv(temp_.count(), nextData, temp_.cpu_data(), nextData);
  }
}


void MVNLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
  const Dtype* nextDiff = nextDiff;
  const Dtype* nextData = top[0]->cpu_data();
  const Dtype* prevData = prevData;
  Dtype* prevDiff = bottom[0]->mutable_diff();

  int num;
  if (this->layer_param_.mvn_param().across_channels())
    num = prev->size.n;
  else
    num = prev->size.n * prev->size.c;

  int dim = Data_count(prev->size) / num;

  if (this->layer_param_.mvn_param().normalize_variance()) {
    blas_smul(temp_.count(), nextData, nextDiff, prevDiff);
    blas_sgemv(CblasNoTrans, num, dim, 1., prevDiff,
          sum_multiplier_.cpu_data(), 0., mean_);
    blas_sgemm(CblasNoTrans, CblasNoTrans, num, dim, 1, 1.,
          mean_.cpu_data(), sum_multiplier_.cpu_data(), 0.,
          prevDiff);
    blas_smul(temp_.count(), nextData, prevDiff, prevDiff);

    blas_sgemv(CblasNoTrans, num, dim, 1., nextDiff,
            sum_multiplier_.cpu_data(), 0., mean_);
    blas_sgemm(CblasNoTrans, CblasNoTrans, num, dim, 1, 1.,
            mean_.cpu_data(), sum_multiplier_.cpu_data(), 1.,
            prevDiff);

    blas_saxpby(temp_.count(), (Dtype)(1), nextDiff, (Dtype)(-1. / dim),
        prevDiff);

    // put the squares of bottom into temp_
    blas_spowx(temp_.count(), prevData, (Dtype)(2),
        temp_);
    blas_sgemm(CblasNoTrans, CblasNoTrans, num, dim, 1, 1.,
        variance_.cpu_data(), sum_multiplier_.cpu_data(), 0.,
        temp_);

    blas_sdiv(temp_.count(), prevDiff, temp_.cpu_data(), prevDiff);
  } else {
    blas_sgemv(CblasNoTrans, num, dim, 1. / dim, nextDiff,
      sum_multiplier_.cpu_data(), 0., mean_);
    blas_sgemm(CblasNoTrans, CblasNoTrans, num, dim, 1, -1.,
      mean_.cpu_data(), sum_multiplier_.cpu_data(), 0.,
      temp_);
    blas_sadd(temp_.count(), nextDiff, temp_.cpu_data(), prevDiff);
  }
}


#ifdef CPU_ONLY
STUB_GPU(MVNLayer);
#endif

INSTANTIATE_CLASS(MVNLayer);
REGISTER_LAYER_CLASS(MVN);


