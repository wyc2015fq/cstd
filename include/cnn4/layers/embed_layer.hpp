#ifndef CAFFE_EMBED_LAYER_HPP_
#define CAFFE_EMBED_LAYER_HPP_







namespace
{

  /**
   * @brief A layer for learning "embeddings" of one-hot vector input.
   *        Equivalent to an InnerProductLayer with one-hot vectors as input, but
   *        for efficiency the input is the "hot" index of each column itself.
   *
   * TODO(dox): thorough documentation for Forward, Backward, and proto params.
   */
  template <typename Dtype>
  class EmbedLayer : public Layer<Dtype>
  {
  public:
    explicit EmbedLayer()
      : Layer<Dtype>() {}
    virtual void LayerSetUp(const vector<Blob<Dtype>*> & bottom,
                            const vector<Blob<Dtype>*> & top);
    virtual void Reshape(const vector<Blob<Dtype>*> & bottom,
                         const vector<Blob<Dtype>*> & top);

    virtual inline const char* type() const { return "Embed"; }
    virtual inline int ExactNumBottomBlobs() const { return 1; }
    virtual inline int ExactNumTopBlobs() const { return 1; }

  public:
    virtual void Forward(CPUContext* context, const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top);
    virtual void Forward(GPUContext* context, const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top);
    virtual void Backward(CPUContext* context, const vector<Blob<Dtype>*> & top,
                              const vector<Blob<Dtype>*> & bottom);
    virtual void Backward(GPUContext* context, const vector<Blob<Dtype>*> & top,
                              const vector<Blob<Dtype>*> & bottom);

    int M_;
    int K_;
    int N_;
    bool bias_term_;
    Blob<Dtype> bias_multiplier_;
  };

}  // namespace

#endif  // CAFFE_EMBED_LAYER_HPP_
