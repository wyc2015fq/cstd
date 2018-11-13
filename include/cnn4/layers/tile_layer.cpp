

#include "caffe/layers/tile_layer.hpp"


namespace
{

  template <typename Dtype>
  void TileLayer::Reshape(
    const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    const TileParameter & tile_param = this->param_->tile_param();
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

  template <typename Dtype>
  void TileLayer::Forward(_CONTEXT,
    const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    const Dtype* bottom_data = bottom[0]->data();
    Dtype* top_data = top[0]->mdata();
    for (size_t i = 0; i < outer_dim_; ++i) {
      for (size_t t = 0; t < tiles_; ++t) {
        caffe_copy(inner_dim_, bottom_data, top_data);
        top_data += inner_dim_;
      }
      bottom_data += inner_dim_;
    }
  }

  template <typename Dtype>
  void TileLayer::Backward(CPUContext* context, const vector<Blob*> & top,
                                      const vector<Blob*> & bottom)
  {
    if (!bottom[0]->propagate_down_) { return; }
    const Dtype* top_diff = top[0]->diff();
    Dtype* bottom_diff = bottom[0]->mdiff();
    for (size_t i = 0; i < outer_dim_; ++i) {
      caffe_copy(inner_dim_, top_diff, bottom_diff);
      top_diff += inner_dim_;
      for (size_t t = 1; t < tiles_; ++t) {
        caffe_axpy(inner_dim_, Dtype(1), top_diff, bottom_diff);
        top_diff += inner_dim_;
      }
      bottom_diff += inner_dim_;
    }
  }

#ifdef CPU_ONLY
  STUB_GPU(TileLayer);
#endif

  INSTANTIATE_CLASS(TileLayer);
  REGISTER_LAYER_CLASS(Tile);

}  // namespace
