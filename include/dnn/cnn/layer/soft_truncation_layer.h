


class SoftTruncationLayer {
 public:
  explicit SoftTruncationLayer(const LayerParameter& param)
      : NeuronLayer(param) {}

  virtual inline const char* type() const { return "SoftTruncation"; }

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



void SoftTruncationLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* prevData = prevData;
  Dtype* nextData = nextData;
  const int count = Data_count(prev->size);
  Dtype c = this->layer_param_.soft_truncation_param().c();
  for (i = 0; i < count; ++i) {
    nextData[i] = 1 - exp(prevData[i] / (-c));
  }
}


void SoftTruncationLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
  if (propagate_down[0]) {
    const Dtype* nextData = top[0]->cpu_data();
    const Dtype* nextDiff = nextDiff;
    Dtype* prevDiff = bottom[0]->mutable_diff();
    const int count = Data_count(prev->size);
    Dtype c = this->layer_param_.soft_truncation_param().c();
    
    for (i = 0; i < count; ++i) {
      prevDiff[i] = nextDiff[i] * (1 - nextData[i]) / c;
    }
  }
}


#ifdef CPU_ONLY
STUB_GPU(SoftTruncationLayer);
#endif

INSTANTIATE_CLASS(SoftTruncationLayer);
REGISTER_LAYER_CLASS(SoftTruncation);


