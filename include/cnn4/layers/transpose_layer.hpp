#ifndef CAFFE_TRANSPOSE_LAYER_HPP_
#define CAFFE_TRANSPOSE_LAYER_HPP_







namespace
{

  template <typename Dtype>
  class TransposeLayer : public Layer<Dtype>
  {
  public:
    explicit TransposeLayer()
      : Layer<Dtype>() {}
    virtual void LayerSetUp(const vector<Blob<Dtype>*> & bottom,
                            const vector<Blob<Dtype>*> & top);
    virtual void Reshape(const vector<Blob<Dtype>*> & bottom,
                         const vector<Blob<Dtype>*> & top);

    virtual inline const char* type() const { return "Transpose"; }
    virtual inline int ExactNumBottomBlobs() const { return 1; }
    virtual inline int ExactNumTopBlobs() const { return 1; }

  public:
    virtual void Forward(CPUContext* context, const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top);
    virtual void Backward(CPUContext* context, const vector<Blob<Dtype>*> & top,
                              const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom);
    virtual void Forward(GPUContext* context, const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top);
    virtual void Backward(GPUContext* context, const vector<Blob<Dtype>*> & top,
                              const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom);
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
