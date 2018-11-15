



#include "caffe/util/interp.hpp"
#include "caffe/layers/interp_layer.hpp"

namespace
{

  template <typename Dtype>
  void InterpLayer::LayerSetUp(const vector<Blob*> & bottom,
                                      const vector<Blob*> & top)
  {
    InterpParameter interp_param = this->param_->interp_param();
    pad_beg_ = interp_param.pad_beg();
    pad_end_ = interp_param.pad_end();
    CHECK_LE(pad_beg_, 0) << "Only supports non-pos padding (cropping) for now";
    CHECK_LE(pad_end_, 0) << "Only supports non-pos padding (cropping) for now";
  }

  template <typename Dtype>
  void InterpLayer::Reshape(const vector<Blob*> & bottom,
                                   const vector<Blob*> & top)
  {
    num_ = bottom[0]->num();
    channels_ = bottom[0]->channels();
    height_in_ = bottom[0]->height();
    width_in_ = bottom[0]->width();
    height_in_eff_ = height_in_ + pad_beg_ + pad_end_;
    width_in_eff_ = width_in_ + pad_beg_ + pad_end_;
    InterpParameter interp_param = this->param_->interp_param();
    if (interp_param.has_shrink_factor() &&
        !interp_param.has_zoom_factor()) {
      const int shrink_factor = interp_param.shrink_factor();
      CHECK_GE(shrink_factor, 1) << "Shrink factor must be positive";
      height_out_ = (height_in_eff_ - 1) / shrink_factor + 1;
      width_out_ = (width_in_eff_ - 1) / shrink_factor + 1;
    } else if (interp_param.has_zoom_factor() &&
               !interp_param.has_shrink_factor()) {
      const int zoom_factor = interp_param.zoom_factor();
      CHECK_GE(zoom_factor, 1) << "Zoom factor must be positive";
      height_out_ = height_in_eff_ + (height_in_eff_ - 1) * (zoom_factor - 1);
      width_out_ = width_in_eff_ + (width_in_eff_ - 1) * (zoom_factor - 1);
    } else if (interp_param.has_height() && interp_param.has_width()) {
      height_out_  = interp_param.height();
      width_out_  = interp_param.width();
    } else if (interp_param.has_shrink_factor() &&
               interp_param.has_zoom_factor()) {
      const int shrink_factor = interp_param.shrink_factor();
      const int zoom_factor = interp_param.zoom_factor();
      CHECK_GE(shrink_factor, 1) << "Shrink factor must be positive";
      CHECK_GE(zoom_factor, 1) << "Zoom factor must be positive";
      height_out_ = (height_in_eff_ - 1) / shrink_factor + 1;
      width_out_ = (width_in_eff_ - 1) / shrink_factor + 1;
      height_out_ = height_out_ + (height_out_ - 1) * (zoom_factor - 1);
      width_out_ = width_out_ + (width_out_ - 1) * (zoom_factor - 1);
    } else {
      LOG(FATAL);
    }
    CHECK_GT(height_in_eff_, 0) << "height should be positive";
    CHECK_GT(width_in_eff_, 0) << "width should be positive";
    CHECK_GT(height_out_, 0) << "height should be positive";
    CHECK_GT(width_out_, 0) << "width should be positive";
    top[0]->Reshape(num_, channels_, height_out_, width_out_);
  }

  template <typename Dtype>
  void InterpLayer::Forward_(CPUContext* context, const vector<Blob*> & bottom,
                                       const vector<Blob*> & top)
  {
    caffe_interp2<Dtype, false>(num_ * channels_,
                                    bottom[0]->data(), - pad_beg_, - pad_beg_, height_in_eff_, width_in_eff_, height_in_, width_in_,
                                    top[0]->mdata(), 0, 0, height_out_, width_out_, height_out_, width_out_);
  }

  template <typename Dtype>
  void InterpLayer::Backward_(CPUContext* context, const vector<Blob*> & top,
                                        const vector<Blob*> & bottom)
  {
    if (!bottom[0]->propagate_down_) { return; }
    caffe_set(bottom[0]->count(), Dtype(0), bottom[0]->mdiff());
    caffe_interp2_backward<Dtype, false>(num_ * channels_,
        bottom[0]->mdiff(), - pad_beg_, - pad_beg_, height_in_eff_, width_in_eff_, height_in_, width_in_,
        top[0]->diff(), 0, 0, height_out_, width_out_, height_out_, width_out_);
  }

#ifndef CPU_ONLY
  template <typename Dtype>
  void InterpLayer::Forward_(GPUContext* context, const vector<Blob*> & bottom,
                                       const vector<Blob*> & top)
  {
    caffe_gpu_interp2<Dtype, false>(num_ * channels_,
                                    bottom[0]->data(), - pad_beg_, - pad_beg_, height_in_eff_, width_in_eff_, height_in_, width_in_,
                                    top[0]->mdata(), 0, 0, height_out_, width_out_, height_out_, width_out_);
  }

  template <typename Dtype>
  void InterpLayer::Backward_(GPUContext* context, const vector<Blob*> & top,
                                        const vector<Blob*> & bottom)
  {
    if (!bottom[0]->propagate_down_) { return; }
    caffe_gpu_set(bottom[0]->count(), Dtype(0), bottom[0]->gpu_mdiff());
    caffe_gpu_interp2_backward<Dtype, false>(num_ * channels_,
        bottom[0]->gpu_mdiff(), - pad_beg_, - pad_beg_, height_in_eff_, width_in_eff_, height_in_, width_in_,
        top[0]->gpu_diff(), 0, 0, height_out_, width_out_, height_out_, width_out_);
  }
#endif

#ifdef CPU_ONLY
  STUB_GPU(InterpLayer);
#endif

  INSTANTIATE_CLASS(InterpLayer);
  REGISTER_LAYER_CLASS(Interp);

}  // namespace
