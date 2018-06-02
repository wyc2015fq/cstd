// TODO (sergeyk): effect should not be dependent on phase. wasted memcpy.


  /**
  * @brief Add noise.
  */
  
  class NoiseLayer {
  public:
    explicit NoiseLayer(const LayerParameter& param)
      : NeuronLayer(param) {}
    virtual void Reshape(const vector<Blob*>& bottom,
                         const vector<Blob*>& top);

    virtual inline const char* type() const { return "Noise"; }

  protected:
    virtual void Forward_cpu(const vector<Blob*>& bottom,
                             const vector<Blob*>& top);
    virtual void Forward_gpu(const vector<Blob*>& bottom,
                             const vector<Blob*>& top);
    virtual void Backward_cpu(const vector<Blob*>& top,
                              const vector<bool>& propagate_down, const vector<Blob*>& bottom);
    virtual void Backward_gpu(const vector<Blob*>& top,
                              const vector<bool>& propagate_down, const vector<Blob*>& bottom);

    Blob mask;
  };



  
  void NoiseLayer::Reshape(const vector<Blob*>& bottom,
                                      const vector<Blob*>& top) {
    mask.ReshapeLike(*bottom[0]);
  }


void NoiseLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* prevData = prevData;
  Dtype* nextData = nextData;
  const int count = Data_count(prev->size);
  if (this->phase_ == TRAIN) {
    Dtype data_magnitude = sqrt(bottom[0]->sumsq_data() / (Dtype)(Data_count(prev->size)));
    if (this->layer_param_.noise_param().has_gaussian_std()) {
      blas_srng_gaussian(count, this->layer_param_.noise_param().bias(),
                                data_magnitude * this->layer_param_.noise_param().gaussian_std(), mask);
    }
    else if (this->layer_param_.noise_param().has_uniform_range()) {
      blas_srng_uniform(count, this->layer_param_.noise_param().bias() - this->layer_param_.noise_param().uniform_range(),
                        this->layer_param_.noise_param().bias() + this->layer_param_.noise_param().uniform_range(), mask);
    }
    blas_sadd(count, prevData, mask.cpu_data(), nextData);
  } else {
    blas_scopy(count, prevData, nextData);
  }
}


void NoiseLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
  if (propagate_down[0]) {
    bottom[0]->ShareDiff(*top[0]);
  }
}


#ifdef CPU_ONLY
STUB_GPU(NoiseLayer);
#endif

INSTANTIATE_CLASS(NoiseLayer);
REGISTER_LAYER_CLASS(Noise);


