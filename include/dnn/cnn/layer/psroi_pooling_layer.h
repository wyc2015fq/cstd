// ------------------------------------------------------------------
// R-FCN
// Written by Yi Li
// ------------------------------------------------------------------


#include "caffe/blob.hpp"
#include "caffe/common.hpp"
#include "caffe/layer.hpp"




/** 
 * @brief Perform position-sensitive max pooling on regions of interest specified by input, takes
 *        as input N position-sensitive score maps and a list of R regions of interest.
 *   ROIPoolingLayer takes 2 inputs and produces 1 output. bottom[0] is
 *   [N x (C x K^2) x H x W] position-sensitive score maps on which pooling is performed. bottom[1] is
 *   [R x 5] containing a list R ROI tuples with batch index and coordinates of
 *   regions of interest. Each row in bottom[1] is a ROI tuple in format
 *   [batch_index x1 y1 x2 y2], where batch_index corresponds to the index of
 *   instance in the first input and x1 y1 x2 y2 are 0-indexed coordinates
 *   of ROI rectangle (including its boundaries). The output top[0] is [R x C x K x K] score maps pooled
 *   within the ROI tuples.
 * @param param provides PSROIPoolingParameter psroi_pooling_param,
 *        with PSROIPoolingLayer options:
 *  - output_dim. The pooled output channel number.
 *  - group_size. The number of groups to encode position-sensitive score maps
 *  - spatial_scale. Multiplicative spatial scale factor to translate ROI
 *  coordinates from their input scale to the scale used when pooling.
 * R-FCN
 * Written by Yi Li
 */


class PSROIPoolingLayer : public Layer {
 public:
  explicit PSROIPoolingLayer(const LayerParameter& param)
    : Layer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
    const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
    const vector<Blob*>& top);

  virtual inline const char* type() const { return "PSROIPooling"; }

  virtual inline int MinBottomBlobs() const { return 2; }
  virtual inline int MaxBottomBlobs() const { return 2; }
  virtual inline int MinTopBlobs() const { return 1; }
  virtual inline int MaxTopBlobs() const { return 1; }

 protected:
  virtual void Forward_cpu(const vector<Blob*>& bottom,
    const vector<Blob*>& top);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
    const vector<Blob*>& top);
  virtual void Backward_cpu(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Backward_gpu(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  Dtype spatial_scale_;
  int output_dim_;
  int group_size_;

  int channels_;
  int height_;
  int width_;

  int pooled_height_;
  int pooled_width_;
  Blob<int> mapping_channel_;
};



using MAX;
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
    channels_ = prev->size.c;
    CHECK_EQ(channels_, output_dim_*group_size_*group_size_)
      << "input channel number does not match layer parameters";
    height_ = prev->size.h;
    width_ = prev->size.w;
    top[0]->Reshape(
      prev1->size.n, output_dim_, pooled_height_, pooled_width_);
    mapping_channel_.Reshape(
      prev1->size.n, output_dim_, pooled_height_, pooled_width_);
  }

  
  void PSROIPoolingLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
    NOT_IMPLEMENTED;
  }

  
  void PSROIPoolingLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
    NOT_IMPLEMENTED;
  }
#ifdef CPU_ONLY
  STUB_GPU(PSROIPoolingLayer);
#endif

  INSTANTIATE_CLASS(PSROIPoolingLayer);
  REGISTER_LAYER_CLASS(PSROIPooling);


