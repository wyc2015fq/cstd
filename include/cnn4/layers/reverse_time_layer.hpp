#ifndef REVERSE_TIME_LAYER_HPP
#define REVERSE_TIME_LAYER_HPP







namespace
{

  /*
   * @brief Reverses the data of the input Blob into the output blob.
   *
   * Note: This is a useful layer if you want to reverse the time of
   * a recurrent layer.
   */

  template <typename Dtype>
  class ReverseTimeLayer : public Layer
  {
  public:
    explicit ReverseTimeLayer();

    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top);
    virtual void Reshape(const vector<Blob*> & bottom,
                         const vector<Blob*> & top);

    virtual inline const char* type() const { return "ReverseTime"; }

    virtual inline int ExactNumBottomBlobs() const { return 2; }
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

    bool copy_remaining_;
  };

}  // namespace

#endif  // REVERSE_TIME_LAYER_HPP
