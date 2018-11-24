#ifndef CAFFE_MVN_LAYER_HPP_
#define CAFFE_MVN_LAYER_HPP_







namespace
{

  /**
   * @brief Normalizes the input to have 0-mean and/or unit (1) variance.
   *
   * TODO(dox): thorough documentation for Forward_, Backward_, and proto params.
   */
  template <typename Dtype>
  class MVNLayer : public Layer
  {
  public:
    explicit MVNLayer()
      : Layer() {}
    virtual void Reshape(const vector<Blob*> & bottom,
                         const vector<Blob*> & top);

    virtual inline const char* type() const { return "MVN"; }
    virtual inline int ExactNumBottomBlobs() const { return 1; }
    virtual inline int ExactNumTopBlobs() const { return 1; }

  public:
    virtual void Forward_(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Forward_(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Backward_(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
    virtual void Backward_(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);

    Blob mean_, variance_, temp_;

    /// sum_multiplier is used to carry out sum using BLAS
    Blob sum_multiplier_;
    Dtype eps_;
  };

}  // namespace

#endif  // CAFFE_MVN_LAYER_HPP_
