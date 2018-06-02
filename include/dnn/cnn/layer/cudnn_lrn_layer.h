#ifdef USE_CUDNN


class CuDNNLRNLayer : public LRNLayer {
 public:
  explicit CuDNNLRNLayer(const LayerParameter& param)
      : LRNLayer(param), handles_setup_(false) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual ~CuDNNLRNLayer();

 protected:
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  bool handles_setup_;
  cudnnHandle_t             handle_;
  cudnnLRNDescriptor_t norm_desc_;
  cudnnTensorDescriptor_t bottom_desc_, top_desc_;

  int size_;
  Dtype alpha_, beta_, k_;
};



void CuDNNLRNLayer::LayerSetUp(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  LRNLayer::LayerSetUp(bottom, top);

  CUDNN_CHECK(cudnnCreate(&handle_));
  CUDNN_CHECK(cudnnCreateLRNDescriptor(&norm_desc_));
  cudnn::createTensor4dDesc(&bottom_desc_);
  cudnn::createTensor4dDesc(&top_desc_);

  // create a LRN handle
  handles_setup_ = true;

  size_ = this->layer_param().lrn_param().local_size();
  alpha_ = this->layer_param().lrn_param().alpha();
  beta_ = this->layer_param().lrn_param().beta();
  k_ = this->layer_param().lrn_param().k();
}


void CuDNNLRNLayer::Reshape(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  LRNLayer::Reshape(bottom, top);
  cudnn::setTensor4dDesc(&bottom_desc_, prev->size.n,
      this->channels_, this->height_, this->width_);
  cudnn::setTensor4dDesc(&top_desc_, prev->size.n,
      this->channels_, this->height_, this->width_);
  CUDNN_CHECK(cudnnSetLRNDescriptor(norm_desc_, size_, alpha_, beta_, k_));
}


CuDNNLRNLayer::~CuDNNLRNLayer() {
  // Check that handles have been setup before destroying.
  if (!handles_setup_) { return; }

  cudnnDestroyTensorDescriptor(bottom_desc_);
  cudnnDestroyTensorDescriptor(top_desc_);

  // destroy LRN handle
  cudnnDestroy(handle_);
}

INSTANTIATE_CLASS(CuDNNLRNLayer);

}   // namespace caffe
#endif
