// ------------------------------------------------------------------
// R-FCN
// Written by Yi Li
// ------------------------------------------------------------------







#include "psroi_pooling_layer.hpp"

using std::max;
using std::min;
using std::floor;
using std::ceil;


  
  void PSROIPoolingLayer::LayerSetUp(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
    PSROIPoolingParameter psroi_pooling_param =
      this->layer_param_.psroi_pooling_param();
    spatial_scale_ = psroi_pooling_param.spatial_scale();
    LOG(INFO) << "Spatial scale: " << spatial_scale_;

    CHECK_GT(psroi_pooling_param.output_dim(), 0)
      << "output_dim must be > 0";
    CHECK_GT(psroi_pooling_param.group_size(), 0)
      << "group_size must be > 0";

    output_dim_ = psroi_pooling_param.output_dim();
    group_size_ = psroi_pooling_param.group_size();
    pooled_height_ = group_size_;
    pooled_width_ = group_size_;
  }

  
  void PSROIPoolingLayer::Reshape(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
    channels_ = bottom[0]->channels();
    CHECK_EQ(channels_, output_dim_*group_size_*group_size_)
      << "input channel number does not match layer parameters";
    height_ = bottom[0]->height();
    width_ = bottom[0]->width();
    top[0]->Reshape(
      bottom[1]->num(), output_dim_, pooled_height_, pooled_width_);
    mapping_channel_.Reshape(
      bottom[1]->num(), output_dim_, pooled_height_, pooled_width_);
  }

  
  void PSROIPoolingLayer::Forward_cpu(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
    NOT_IMPLEMENTED;
  }

  
  void PSROIPoolingLayer::Backward_cpu(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
    NOT_IMPLEMENTED;
  }
#ifdef CPU_ONLY
  STUB_GPU(PSROIPoolingLayer);
#endif

  INSTANTIATE_CLASS(PSROIPoolingLayer);
  REGISTER_LAYER_CLASS(PSROIPooling);


