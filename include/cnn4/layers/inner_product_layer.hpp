#ifndef CAFFE_INNER_PRODUCT_LAYER_HPP_
#define CAFFE_INNER_PRODUCT_LAYER_HPP_







namespace
{

  /**
   * @brief Also known as a "fully-connected" layer, computes an inner product
   *        with a set of learned weights, and (optionally) adds biases.
   *
   * TODO(dox): thorough documentation for Forward, Backward, and proto params.
   */
  template <typename Dtype>
  class InnerProductLayer : public Layer<Dtype>
  {
  public:
    explicit InnerProductLayer()
      : Layer<Dtype>() {}
    virtual void LayerSetUp(const vector<Blob<Dtype>*> & bottom,
                            const vector<Blob<Dtype>*> & top);
    virtual void Reshape(const vector<Blob<Dtype>*> & bottom,
                         const vector<Blob<Dtype>*> & top);

    virtual inline const char* type() const { return "InnerProduct"; }
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

    int M_;
    int K_;
    int N_;
    bool bias_term_;
    Blob<Dtype> bias_multiplier_;
    bool transpose_;  ///< if true, assume transposed weights
  };

}  // namespace

#endif  // CAFFE_INNER_PRODUCT_LAYER_HPP_
