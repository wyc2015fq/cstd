#ifndef CAFFE_TILE_LAYER_HPP_
#define CAFFE_TILE_LAYER_HPP_







namespace
{

  /**
   * @brief Copy a Blob along specified dimensions.
   */
  template <typename Dtype>
  class TileLayer : public Layer<Dtype>
  {
  public:
    explicit TileLayer()
      : Layer<Dtype>() {}
    virtual void Reshape(const vector<Blob<Dtype>*> & bottom,
                         const vector<Blob<Dtype>*> & top);

    virtual inline const char* type() const { return "Tile"; }
    virtual inline int ExactNumBottomBlobs() const { return 1; }
    virtual inline int ExactNumTopBlobs() const { return 1; }

  public:
    virtual void Forward(CPUContext* context, const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top);
    virtual void Forward(GPUContext* context, const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top);

    virtual void Backward(CPUContext* context, const vector<Blob<Dtype>*> & top,
                              const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom);
    virtual void Backward(GPUContext* context, const vector<Blob<Dtype>*> & top,
                              const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom);

    unsigned int axis_, tiles_, outer_dim_, inner_dim_;
  };

}  // namespace

#endif  // CAFFE_TILE_LAYER_HPP_
