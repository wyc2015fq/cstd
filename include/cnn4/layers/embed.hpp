#ifndef CAFFE_EMBED_LAYER_HPP_
#define CAFFE_EMBED_LAYER_HPP_







namespace
{

  /**
   * @brief A layer for learning "embeddings" of one-hot vector input.
   *        Equivalent to an InnerProductLayer with one-hot vectors as input, but
   *        for efficiency the input is the "hot" index of each column itself.
   *
   * TODO(dox): thorough documentation for Forward_, Backward_, and proto params.
   */
  template <typename Dtype>
  class EmbedLayer : public Layer
  {
  public:
    explicit EmbedLayer()
      : Layer() {}
    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top);
    virtual void Reshape(const vector<Blob*> & bottom,
                         const vector<Blob*> & top);

    virtual inline const char* type() const { return "Embed"; }
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

    int M_;
    int K_;
    int N_;
    bool bias_term_;
    Blob bias_multiplier_;
  };

}  // namespace

#endif  // CAFFE_EMBED_LAYER_HPP_
