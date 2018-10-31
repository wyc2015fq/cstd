#ifndef CAFFE_SLICE_LAYER_HPP_
#define CAFFE_SLICE_LAYER_HPP_







namespace
{

  /**
   * @brief Takes a Blob and slices it along either the num or channel dimension,
   *        outputting multiple sliced Blob results.
   *
   * TODO(dox): thorough documentation for Forward, Backward, and proto params.
   */
  template <typename Dtype>
  class SliceLayer : public Layer<Dtype>
  {
  public:
    explicit SliceLayer()
      : Layer<Dtype>() {}
    virtual void LayerSetUp(const vector<Blob<Dtype>*> & bottom,
                            const vector<Blob<Dtype>*> & top);
    virtual void Reshape(const vector<Blob<Dtype>*> & bottom,
                         const vector<Blob<Dtype>*> & top);

    virtual inline const char* type() const { return "Slice"; }
    virtual inline int ExactNumBottomBlobs() const { return 1; }
    virtual inline int MinTopBlobs() const { return 1; }

  public:
    virtual void Forward(CPUContext* context, const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top);
    virtual void Forward(GPUContext* context, const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top);
    virtual void Backward(CPUContext* context, const vector<Blob<Dtype>*> & top,
                              const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom);
    virtual void Backward(GPUContext* context, const vector<Blob<Dtype>*> & top,
                              const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom);

    int count_;
    int num_slices_;
    int slice_size_;
    int slice_axis_;
    vector<int> slice_point_;
  };

}  // namespace

#endif  // CAFFE_SLICE_LAYER_HPP_
