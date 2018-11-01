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
  class ReverseTimeLayer : public Layer<Dtype>
  {
  public:
    explicit ReverseTimeLayer();

    virtual void LayerSetUp(const vector<Blob<Dtype>*> & bottom,
                            const vector<Blob<Dtype>*> & top);
    virtual void Reshape(const vector<Blob<Dtype>*> & bottom,
                         const vector<Blob<Dtype>*> & top);

    virtual inline const char* type() const { return "ReverseTime"; }

    virtual inline int ExactNumBottomBlobs() const { return 2; }
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

    bool copy_remaining_;
  };

}  // namespace

#endif  // REVERSE_TIME_LAYER_HPP
