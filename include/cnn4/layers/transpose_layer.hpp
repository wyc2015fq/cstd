#ifndef CAFFE_TRANSPOSE_LAYER_HPP_
#define CAFFE_TRANSPOSE_LAYER_HPP_







namespace
{

  template <typename Dtype>
  class TransposeLayer : public Layer
  {
  public:
    explicit TransposeLayer()
      : Layer() {}
    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top);
    virtual void Reshape(const vector<Blob*> & bottom,
                         const vector<Blob*> & top);

    virtual inline const char* type() const { return "Transpose"; }
    virtual inline int ExactNumBottomBlobs() const { return 1; }
    virtual inline int ExactNumTopBlobs() const { return 1; }

  public:
    virtual void Forward(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Backward(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
    virtual void Forward(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Backward(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
  private:
    TransposeParameter transpose_param_;
    vector<int> permute(const vector<int> & vec);
    Blob<int> bottom_counts_;
    Blob<int> top_counts_;
    Blob<int> forward_map_;
    Blob<int> backward_map_;
    Blob<int> buf_;
  };

}  // namespace

#endif  // CAFFE_TRANSPOSE_LAYER_HPP_
