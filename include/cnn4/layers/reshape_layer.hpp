#ifndef CAFFE_XXX_LAYER_HPP_
#define CAFFE_XXX_LAYER_HPP_







namespace
{

  /*
   * @brief Reshapes the input Blob into an arbitrary-sized output Blob.
   *
   * Note: similarly to FlattenLayer, this layer does not change the input values
   * (see FlattenLayer, Blob::ShareData and Blob::ShareDiff).
   */
  template <typename Dtype>
  class ReshapeLayer : public Layer
  {
  public:
    explicit ReshapeLayer()
      : Layer() {}
    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top);
    virtual void Reshape(const vector<Blob*> & bottom,
                         const vector<Blob*> & top);

    virtual inline const char* type() const { return "Reshape"; }
    virtual inline int ExactNumBottomBlobs() const { return 1; }
    virtual inline int ExactNumTopBlobs() const { return 1; }

  public:
    virtual void Forward(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top) {}
    virtual void Backward(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom) {}
    virtual void Forward(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top) {}
    virtual void Backward(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom) {}

    /// @brief vector of axes indices whose dimensions we'll copy from the bottom
    vector<int> copy_axes_;
    /// @brief the index of the axis whose dimension we infer, or -1 if none
    int inferred_axis_;
    /// @brief the product of the "constant" output dimensions
    int constant_count_;
  };

}  // namespace

#endif  // CAFFE_XXX_LAYER_HPP_
