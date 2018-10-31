#ifndef CAFFE_MVN_LAYER_HPP_
#define CAFFE_MVN_LAYER_HPP_







namespace
{

  /**
   * @brief Normalizes the input to have 0-mean and/or unit (1) variance.
   *
   * TODO(dox): thorough documentation for Forward, Backward, and proto params.
   */
  template <typename Dtype>
  class MVNLayer : public Layer<Dtype>
  {
  public:
    explicit MVNLayer()
      : Layer<Dtype>() {}
    virtual void Reshape(const vector<Blob<Dtype>*> & bottom,
                         const vector<Blob<Dtype>*> & top);

    virtual inline const char* type() const { return "MVN"; }
    virtual inline int ExactNumBottomBlobs() const { return 1; }
    virtual inline int ExactNumTopBlobs() const { return 1; }

  public:
    virtual void Forward(CPUContext* context, const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top);
    virtual void Forward(GPUContext* context, const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top);
    virtual void Backward(CPUContext* context, const vector<Blob<Dtype>*> & top,
                              const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom);
    virtual void Backward(GPUContext* context, const vector<Blob<Dtype>*> & top,
                              const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom);

    Blob<Dtype> mean_, variance_, temp_;

    /// sum_multiplier is used to carry out sum using BLAS
    Blob<Dtype> sum_multiplier_;
    Dtype eps_;
  };

}  // namespace

#endif  // CAFFE_MVN_LAYER_HPP_
