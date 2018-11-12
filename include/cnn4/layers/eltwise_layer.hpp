#ifndef CAFFE_ELTWISE_LAYER_HPP_
#define CAFFE_ELTWISE_LAYER_HPP_







namespace
{

  /**
   * @brief Compute elementwise operations, such as product and sum,
   *        along multiple input Blobs.
   *
   * TODO(dox): thorough documentation for Forward, Backward, and proto params.
   */
  template <typename Dtype>
  class EltwiseLayer : public Layer
  {
  public:
    explicit EltwiseLayer()
      : Layer() {}
    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top);
    virtual void Reshape(const vector<Blob*> & bottom,
                         const vector<Blob*> & top);

    virtual inline const char* type() const { return "Eltwise"; }
    virtual inline int MinBottomBlobs() const { return 2; }
    virtual inline int ExactNumTopBlobs() const { return 1; }

  public:
    virtual void Forward(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Forward(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Backward(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
    virtual void Backward(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);

    EltwiseParameter_EltwiseOp op_;
    vector<Dtype> coeffs_;
    Blob<int> max_idx_;

    bool stable_prod_grad_;
  };

}  // namespace

#endif  // CAFFE_ELTWISE_LAYER_HPP_
