#ifndef CAFFE_TILE_LAYER_HPP_
#define CAFFE_TILE_LAYER_HPP_







namespace
{

  /**
   * @brief Copy a Blob along specified dimensions.
   */
  template <typename Dtype>
  class TileLayer : public Layer
  {
  public:
    explicit TileLayer()
      : Layer() {}
    virtual void Reshape(const vector<Blob*> & bottom,
                         const vector<Blob*> & top);

    virtual inline const char* type() const { return "Tile"; }
    virtual inline int ExactNumBottomBlobs() const { return 1; }
    virtual inline int ExactNumTopBlobs() const { return 1; }

  public:
    virtual void Forward(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Forward(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);

    virtual void Backward(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
    virtual void Backward(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);

    unsigned int axis_, tiles_, outer_dim_, inner_dim_;
  };

}  // namespace

#endif  // CAFFE_TILE_LAYER_HPP_
