#ifndef CAFFE_IM2COL_LAYER_HPP_
#define CAFFE_IM2COL_LAYER_HPP_







namespace
{

  /**
   * @brief A helper for image operations that rearranges image regions into
   *        column vectors.  Used by ConvolutionLayer to perform convolution
   *        by matrix multiplication.
   *
   * TODO(dox): thorough documentation for Forward, Backward, and proto params.
   */
  template <typename Dtype>
  class Im2colLayer : public Layer
  {
  public:
    explicit Im2colLayer()
      : Layer() {}
    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top);
    virtual void Reshape(const vector<Blob*> & bottom,
                         const vector<Blob*> & top);

    virtual inline const char* type() const { return "Im2col"; }
    virtual inline int ExactNumBottomBlobs() const { return 1; }
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

    /// @brief The spatial dimensions of a filter kernel.
    Blob<int> kernel_shape_;
    /// @brief The spatial dimensions of the stride.
    Blob<int> stride_;
    /// @brief The spatial dimensions of the padding.
    Blob<int> pad_;
    /// @brief The spatial dimensions of the dilation.
    Blob<int> dilation_;

    int num_spatial_axes_;
    int bottom_dim_;
    int top_dim_;

    int channel_axis_;
    int num_;
    int channels_;

    bool force_nd_im2col_;
  };

}  // namespace

#endif  // CAFFE_IM2COL_LAYER_HPP_
