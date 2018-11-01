#ifndef CAFFE_SOFTMAX_LAYER_HPP_
#define CAFFE_SOFTMAX_LAYER_HPP_







namespace
{

  /**
   * @brief Computes the softmax function.
   *
   * TODO(dox): thorough documentation for Forward, Backward, and proto params.
   */
  template <typename Dtype>
  class SoftmaxLayer : public Layer<Dtype>
  {
  public:
    explicit SoftmaxLayer()
      : Layer<Dtype>() {}
    virtual void Reshape(const vector<Blob<Dtype>*> & bottom,
                         const vector<Blob<Dtype>*> & top);

    virtual inline const char* type() const { return "Softmax"; }
    virtual inline int ExactNumBottomBlobs() const { return 1; }
    virtual inline int ExactNumTopBlobs() const { return 1; }

  public:
    virtual void Forward(CPUContext* context, const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top);
    virtual void Forward(GPUContext* context, const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top);
    virtual void Backward(CPUContext* context, const vector<Blob<Dtype>*> & top,
                              const vector<Blob<Dtype>*> & bottom);
    virtual void Backward(GPUContext* context, const vector<Blob<Dtype>*> & top,
                              const vector<Blob<Dtype>*> & bottom);

    int outer_num_;
    int inner_num_;
    int softmax_axis_;
    /// sum_multiplier is used to carry out sum using BLAS
    Blob<Dtype> sum_multiplier_;
    /// scale is an intermediate Blob to hold temporary results.
    Blob<Dtype> scale_;
  };

}  // namespace

#endif  // CAFFE_SOFTMAX_LAYER_HPP_
