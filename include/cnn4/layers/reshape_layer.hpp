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
  class ReshapeLayer : public Layer<Dtype>
  {
  public:
    explicit ReshapeLayer()
      : Layer<Dtype>() {}
    virtual void LayerSetUp(const vector<Blob<Dtype>*> & bottom,
                            const vector<Blob<Dtype>*> & top);
    virtual void Reshape(const vector<Blob<Dtype>*> & bottom,
                         const vector<Blob<Dtype>*> & top);

    virtual inline const char* type() const { return "Reshape"; }
    virtual inline int ExactNumBottomBlobs() const { return 1; }
    virtual inline int ExactNumTopBlobs() const { return 1; }

  public:
    virtual void Forward(CPUContext* context, const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top) {}
    virtual void Backward(CPUContext* context, const vector<Blob<Dtype>*> & top,
                              const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom) {}
    virtual void Forward(GPUContext* context, const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top) {}
    virtual void Backward(GPUContext* context, const vector<Blob<Dtype>*> & top,
                              const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom) {}

    /// @brief vector of axes indices whose dimensions we'll copy from the bottom
    vector<int> copy_axes_;
    /// @brief the index of the axis whose dimension we infer, or -1 if none
    int inferred_axis_;
    /// @brief the product of the "constant" output dimensions
    int constant_count_;
  };

}  // namespace

#endif  // CAFFE_XXX_LAYER_HPP_
