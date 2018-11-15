#ifndef CAFFE_SLICE_LAYER_HPP_
#define CAFFE_SLICE_LAYER_HPP_







namespace
{

  /**
   * @brief Takes a Blob and slices it along either the num or channel dimension,
   *        outputting multiple sliced Blob results.
   *
   * TODO(dox): thorough documentation for Forward_, Backward_, and proto params.
   */
  template <typename Dtype>
  class SliceLayer : public Layer
  {
  public:
    explicit SliceLayer()
      : Layer() {}
    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top);
    virtual void Reshape(const vector<Blob*> & bottom,
                         const vector<Blob*> & top);

    virtual inline const char* type() const { return "Slice"; }
    virtual inline int ExactNumBottomBlobs() const { return 1; }
    virtual inline int MinTopBlobs() const { return 1; }

  public:
    virtual void Forward_(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Forward_(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Backward_(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
    virtual void Backward_(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);

    int count_;
    int num_slices_;
    int slice_size_;
    int slice_axis_;
    vector<int> slice_point_;
  };

}  // namespace

#endif  // CAFFE_SLICE_LAYER_HPP_
