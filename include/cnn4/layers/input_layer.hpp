#ifndef CAFFE_INPUT_LAYER_HPP_
#define CAFFE_INPUT_LAYER_HPP_







namespace
{

  /**
   * @brief Provides data to the Net by assigning tops directly.
   *
   * This data layer is a container that merely holds the data assigned to it;
   * forward, backward, and reshape are all no-ops.
   */
  template <typename Dtype>
  class InputLayer : public Layer
  {
  public:
    explicit InputLayer()
      : Layer() {}
    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top);
    // Data layers should be shared by multiple solvers in parallel
    virtual inline bool ShareInParallel() const { return true; }
    // Data layers have no bottoms, so reshaping is trivial.
    virtual void Reshape(const vector<Blob*> & bottom,
                         const vector<Blob*> & top) {}

    virtual inline const char* type() const { return "Input"; }
    virtual inline int ExactNumBottomBlobs() const { return 0; }
    virtual inline int MinTopBlobs() const { return 1; }

  public:
    virtual void Forward(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top) {}
    virtual void Backward(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom) {}
  };

}  // namespace

#endif  // CAFFE_INPUT_LAYER_HPP_
