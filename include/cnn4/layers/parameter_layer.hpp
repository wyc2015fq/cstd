#ifndef CAFFE_PARAMETER_LAYER_HPP_
#define CAFFE_PARAMETER_LAYER_HPP_





namespace
{

  template <typename Dtype>
  class ParameterLayer : public Layer<Dtype>
  {
  public:
    explicit ParameterLayer()
      : Layer<Dtype>() {}
    virtual void LayerSetUp(const vector<Blob<Dtype>*> & bottom,
                            const vector<Blob<Dtype>*> & top) {
      if (this->blobs_.size() > 0) {
        LOG(INFO) << "Skipping parameter initialization";
      } else {
        this->blobs_.resize(1);
        this->blobs_[0].reset(new Blob<Dtype>());
        this->blobs_[0]->Reshape(this->param_->parameter_param().shape());
      }
      top[0]->Reshape(this->param_->parameter_param().shape());
    }
    virtual void Reshape(const vector<Blob<Dtype>*> & bottom,
                         const vector<Blob<Dtype>*> & top) { }
    virtual inline const char* type() const { return "Parameter"; }
    virtual inline int ExactNumBottomBlobs() const { return 0; }
    virtual inline int ExactNumTopBlobs() const { return 1; }

  public:
    virtual void Forward(CPUContext* context, const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top) {
      top[0]->ShareData(*(this->blobs_[0]));
      top[0]->ShareDiff(*(this->blobs_[0]));
    }
    virtual void Backward(CPUContext* context, const vector<Blob<Dtype>*> & top,
                              const vector<Blob<Dtype>*> & bottom)
    { }
  };

}  // namespace

#endif
