


#define sign(x) (Dtype(0) < (x)) - ((x) < (Dtype)(0))


  /**
  * @brief MovingNormalizes input.
  */
  
  class MovingNormalizeLayer : public Layer {
  public:
    explicit MovingNormalizeLayer(const LayerParameter& param)
      : Layer(param) {}
    virtual void LayerSetUp(const vector<Blob*>& bottom,
                            const vector<Blob*>& top);
    virtual void Reshape(const vector<Blob*>& bottom,
                         const vector<Blob*>& top);

    virtual inline const char* type() const { return "MovingNormalize"; }
    virtual inline int ExactNumBottomBlobs() const { return 1; }
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

    Blob sum_multiplier_, squared_, norm_;
    int iter_;
  };


void MovingNormalizeLayer::LayerSetUp(
  const vector<Blob*>& bottom, const vector<Blob*>& top) {
  if (this->blobs_.size() > 0) {
    LOG(INFO) << "Skipping parameter initialization";
  }
  else {
    this->blobs_.resize(1);
    this->blobs_[0].reset(new Blob({ 1 }));
    this->blobs_[0]->mutable_data()[0] = -1;
  }
  if (this->layer_param_.param_size() == 0) {
    ParamSpec* fixed_param_spec = this->layer_param_.add_param();
    fixed_param_spec->set_lr_mult(0.f);
    fixed_param_spec->set_decay_mult(0.f);
  }
  else {
    CHECK_EQ(this->layer_param_.param(0).lr_mult(), 0.f)
      << "Cannot configure statistics as layer parameters.";
  }
}


void MovingNormalizeLayer::Reshape(const vector<Blob*>& bottom,
                                          const vector<Blob*>& top) {
  top[0]->ReshapeLike(*bottom[0]);
  squared_.ReshapeLike(*bottom[0]);
  if (layer->o_size == 2) {
    top[1]->Reshape({ 1 });
  }
  norm_.Reshape(prev->size.n, 1,
                prev->size.h, prev->size.w);
  sum_multiplier_.Reshape(prev->size.n, 1,
                          prev->size.h, prev->size.w);
  blas_sset(sum_multiplier_.count(), (Dtype)(1), sum_multiplier_);
}


void MovingNormalizeLayer::Forward(const vector<Blob*>& bottom,
                                              const vector<Blob*>& top) {
  NOT_IMPLEMENTED;
}


void MovingNormalizeLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
      NOT_IMPLEMENTED;
}


#ifdef CPU_ONLY
STUB_GPU(MovingNormalizeLayer);
#endif

INSTANTIATE_CLASS(MovingNormalizeLayer);
REGISTER_LAYER_CLASS(MovingNormalize);

