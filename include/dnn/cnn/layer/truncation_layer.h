


class TruncationLayer {
 public:
  explicit TruncationLayer(const LayerParameter& param)
      : NeuronLayer(param) {}

  virtual inline const char* type() const { return "Truncation"; }

 protected:
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
};



  
  void TruncationLayer::Forward(const vector<Blob*>& bottom,
                                           const vector<Blob*>& top) {
    const Dtype* prevData = prevData;
    Dtype* nextData = nextData;
    const int count = Data_count(prev->size);
    Dtype lower_bound = this->layer_param_.truncation_param().lower_bound();
    Dtype upper_bound = this->layer_param_.truncation_param().upper_bound();
    for (i = 0; i < count; ++i) {
      nextData[i] = std::min(MAX(prevData[i], lower_bound), upper_bound);
    }
  }

  
  void TruncationLayer::Backward(const vector<Blob*>& top,
                                            const vector<bool>& propagate_down,
                                            const vector<Blob*>& bottom) {
    if (propagate_down[0]) {
      const Dtype* prevData = prevData;
      const Dtype* nextDiff = nextDiff;
      Dtype* prevDiff = bottom[0]->mutable_diff();
      const int count = Data_count(prev->size);
      Dtype lower_bound = this->layer_param_.truncation_param().lower_bound();
      Dtype upper_bound = this->layer_param_.truncation_param().upper_bound();

      for (i = 0; i < count; ++i) {
        prevDiff[i] = (prevData[i] > lower_bound && prevData[i] < upper_bound) ? nextDiff[i] : 0;
      }
    }
  }


#ifdef CPU_ONLY
  STUB_GPU(TruncationLayer);
#endif

  INSTANTIATE_CLASS(TruncationLayer);
  REGISTER_LAYER_CLASS(Truncation);


