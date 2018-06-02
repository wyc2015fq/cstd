


class ParameterLayer : public Layer {
 public:
  explicit ParameterLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
                          const vector<Blob*>& top) {
    if (this->blobs_.size() > 0) {
      LOG(INFO) << "Skipping parameter initialization";
    }
    else {
      this->blobs_.resize(1);
      this->blobs_[0].reset(new Blob());
      this->blobs_[0]->Reshape(this->layer_param_.parameter_param().shape());
      shared_ptr<Filler > filler(GetFiller(this->layer_param_.parameter_param().blob_filler()));
      filler->Fill(this->blobs_[0].get());
    }
    top[0]->Reshape(this->layer_param_.parameter_param().shape());
  }
  virtual void Reshape(const vector<Blob*>& bottom,
                       const vector<Blob*>& top) {
  }
  virtual inline const char* type() const { return "Parameter"; }
  virtual inline int ExactNumBottomBlobs() const { return 0; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

 protected:
   virtual void Forward_cpu(const vector<Blob*>& bottom,
                            const vector<Blob*>& top) {
     top[0]->ShareData(*(this->blobs_[0]));
   }
  virtual void Backward_cpu(const vector<Blob*>& top,
                            const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
    if (this->param_propagate_down_[0]) {
      this->blobs_[0]->ShareDiff(*top[0]);
    }
  }

  virtual void Forward_gpu(const vector<Blob*>& bottom,
                           const vector<Blob*>& top) {
    top[0]->ShareData(*(this->blobs_[0]));
  }
  virtual void Backward_gpu(const vector<Blob*>& top,
                            const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
    if (this->param_propagate_down_[0]) {
      this->blobs_[0]->ShareDiff(*top[0]);
    }
  }
};



