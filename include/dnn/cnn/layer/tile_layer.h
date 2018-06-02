



/**
 * @brief Copy a Blob along specified dimensions.
 */

class TileLayer : public Layer {
 public:
  explicit TileLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "Tile"; }
  virtual inline int ExactNumBottomBlobs() const { return 1; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

 protected:
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  unsigned int axis_, tiles_, outer_dim_, inner_dim_;
};




void TileLayer::Reshape(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
  const TileParameter& tile_param = this->layer_param_.tile_param();
  axis_ = bottom[0]->CanonicalAxisIndex(tile_param.axis());
  CHECK(tile_param.has_tiles()) << "Number of tiles must be specified";
  tiles_ = tile_param.tiles();
  CHECK_GT(tiles_, 0) << "Number of tiles must be positive.";
  vector<int> top_shape = bottom[0]->shape();
  top_shape[axis_] = bottom[0]->shape(axis_) * tiles_;
  top[0]->Reshape(top_shape);
  outer_dim_ = bottom[0]->count(0, axis_);
  inner_dim_ = bottom[0]->count(axis_);
}


void TileLayer::Forward(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
  const Dtype* prevData = prevData;
  Dtype* nextData = nextData;
  for (i = 0; i < outer_dim_; ++i) {
    for (t = 0; t < tiles_; ++t) {
      blas_scopy(inner_dim_, prevData, nextData);
      nextData += inner_dim_;
    }
    prevData += inner_dim_;
  }
}


void TileLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  if (!propagate_down[0]) { return; }
  const Dtype* nextDiff = nextDiff;
  Dtype* prevDiff = bottom[0]->mutable_diff();
  for (i = 0; i < outer_dim_; ++i) {
    blas_scopy(inner_dim_, nextDiff, prevDiff);
    nextDiff += inner_dim_;
    for (t = 1; t < tiles_; ++t) {
      blas_saxpy(inner_dim_, (Dtype)(1), nextDiff, prevDiff);
      nextDiff += inner_dim_;
    }
    prevDiff += inner_dim_;
  }
}

#ifdef CPU_ONLY
STUB_GPU(TileLayer);
#endif

INSTANTIATE_CLASS(TileLayer);
REGISTER_LAYER_CLASS(Tile);


