#ifndef CAFFE_INTERP_LAYER_HPP_
#define CAFFE_INTERP_LAYER_HPP_







namespace
{
  /**
   * @brief Changes the spatial resolution by bi-linear interpolation.
   *        The target size is specified in terms of pixels.
   *        The start and end pixels of the input are mapped to the start
   *        and end pixels of the output.
   */
  template <typename Dtype>
  class InterpLayer : public Layer
  {
  public:
    explicit InterpLayer()
      : Layer() {}
    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top);
    virtual void Reshape(const vector<Blob*> & bottom,
                         const vector<Blob*> & top);

    virtual inline const char* type() const { return "Interp"; }
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

    int num_, channels_;
    int height_in_, width_in_;
    int height_out_, width_out_;
    int pad_beg_, pad_end_;
    int height_in_eff_, width_in_eff_;
  };

}  // namespace

#endif  // CAFFE_CONV_LAYER_HPP_
