

class CorrelationLossLayer : public LossLayer {
 public:
  explicit CorrelationLossLayer(const LayerParameter& param)
      : LossLayer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
                          const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
                       const vector<Blob*>& top);
  virtual inline const char* type() const { return "CorrelationLoss"; }
  virtual inline int ExactNumBottomBlobs() const { return -1; }
  virtual inline int ExactNumTopBlobs() const { return -1; }
  virtual inline int ExactTopBlobs() const { return 1; }

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





void CorrelationLossLayer::LayerSetUp(const vector<Blob*>& bottom,
                                               const vector<Blob*>& top) {
  LossLayer::LayerSetUp(bottom, top);
  CHECK_GE(layer->i_size, 2);
}


void CorrelationLossLayer::Reshape(const vector<Blob*>& bottom,
                                                 const vector<Blob*>& top) {
  LossLayer::Reshape(bottom, top);
  CHECK_EQ(prev->size.c, 1);
  CHECK_EQ(prev1->size.c, 1);
}


void CorrelationLossLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* prevData = prevData;
  const Dtype* label = nextData;

  Dtype* loss = nextData;
  loss[0] = blas_sdot(Data_count(prev->size), prevData, label);
  loss[0] /= Data_count(prev->size);
}


void CorrelationLossLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  if (propagate_down[0]) {
    blas_sscale(Data_count(prev->size), nextDiff[0] / Data_count(prev->size), nextData, bottom[0]->mutable_data());
  }
  if (propagate_down[1]) {
    blas_sscale(bottom[1]->count(), nextDiff[0] / Data_count(prev->size), prevData, bottom[1]->mutable_data());
  }
}

#ifdef CPU_ONLY
STUB_GPU(CorrelationLossLayer);
#endif

INSTANTIATE_CLASS(CorrelationLossLayer);
REGISTER_LAYER_CLASS(CorrelationLoss);


