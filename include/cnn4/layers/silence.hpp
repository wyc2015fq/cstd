#ifndef CAFFE_SILENCE_LAYER_HPP_
#define CAFFE_SILENCE_LAYER_HPP_







namespace
{

  /**
   * @brief Ignores bottom blobs while producing no top blobs. (This is useful
   *        to suppress outputs during testing.)
   */
  template <typename Dtype>
  class SilenceLayer : public Layer
  {
  public:
    explicit SilenceLayer()
      : Layer() {}
    virtual void Reshape(const vector<Blob*> & bottom,
                         const vector<Blob*> & top) {}

    virtual inline const char* type() const { return "Silence"; }
    virtual inline int MinBottomBlobs() const { return 1; }
    virtual inline int ExactNumTopBlobs() const { return 0; }

  public:
    virtual void Forward_(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top) {}
    // We can't define Forward_gpu here, since STUB_GPU will provide
    // its own definition for CPU_ONLY mode.
    virtual void Forward_(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Backward_(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
    virtual void Backward_(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
  };

}  // namespace

#endif  // CAFFE_SILENCE_LAYER_HPP_
