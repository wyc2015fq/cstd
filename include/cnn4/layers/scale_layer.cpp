#include <algorithm>


#include "caffe/filler.hpp"
#include "caffe/layer_factory.hpp"
#include "caffe/layers/scale_layer.hpp"


namespace
{

  template <typename Dtype>
  void ScaleLayer::Forward(_CONTEXT,
    const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    const Dtype* bottom_data = bottom[0]->data();
    if (bottom[0] == top[0]) {
      // In-place computation; need to store bottom data before overwriting it.
      // Note that this is only necessary for Backward; we could skip this if not
      // doing Backward, but Caffe currently provides no way of knowing whether
      // we'll need to do Backward at the time of the Forward call.
      caffe_copy(bottom[0]->count(), bottom[0]->data(),
                 temp_.mdata());
    }
    const Dtype* scale_data =
      ((bottom.size() > 1) ? bottom[1] : this->blobs_[0].get())->data();
    //LOG(INFO) << this->param_->name() << " scale value=" << scale_data[0];
    Dtype* top_data = top[0]->mdata();
    for (int n = 0; n < outer_dim_; ++n) {
      for (int d = 0; d < scale_dim_; ++d) {
        const Dtype factor = scale_data[d];
        caffe_scale(inner_dim_, factor, bottom_data, top_data);
        bottom_data += inner_dim_;
        top_data += inner_dim_;
      }
    }
    if (bias_layer_) {
      bias_layer_->runForward(bias_bottom_vec_, top);
    }
  }

  template <typename Dtype>
  void ScaleLayer::Backward(CPUContext* context, const vector<Blob*> & top,
                                       const vector<Blob*> & bottom)
  {
    if (bias_layer_ &&
        this->param_propagate_down_[this->param_propagate_down_.size() - 1]) {
      bias_layer_->runBackward(top, bias_propagate_down_, bias_bottom_vec_);
    }
    const bool scale_param = (bottom.size() == 1);
    Blob* scale = scale_param ? this->blobs_[0].get() : bottom[1];
    if ((!scale_param && bottom[1]->propagate_down_) ||
        (scale_param && this->blobs_[0]->propagate_down_)) {
      const Dtype* top_diff = top[0]->diff();
      const bool in_place = (bottom[0] == top[0]);
      const Dtype* bottom_data = (in_place ? &temp_ : bottom[0])->data();
      // Hack: store big eltwise product in bottom[0] diff, except in the special
      // case where this layer itself does the eltwise product, in which case we
      // can store it directly in the scale diff, and we're done.
      // If we're computing in-place (and not doing eltwise computation), this
      // hack doesn't work and we store the product in temp_.
      const bool is_eltwise = (bottom[0]->count() == scale->count());
      Dtype* product = (is_eltwise ? scale->mdiff() :
                        (in_place ? temp_.mdata() : bottom[0]->mdiff()));
      caffe_mul(top[0]->count(), top_diff, bottom_data, product);
      if (!is_eltwise) {
        Dtype* sum_result = NULL;
        if (inner_dim_ == 1) {
          sum_result = product;
        } else if (sum_result_.count() == 1) {
          const Dtype* sum_mult = sum_multiplier_.data();
          Dtype* scale_diff = scale->mdiff();
          if (scale_param) {
            Dtype result = caffe_dot(inner_dim_, product, sum_mult);
            *scale_diff += result;
          } else {
            *scale_diff = caffe_dot(inner_dim_, product, sum_mult);
          }
        } else {
          const Dtype* sum_mult = sum_multiplier_.data();
          sum_result = (outer_dim_ == 1) ?
                       scale->mdiff() : sum_result_.mdata();
          caffe_gemv(CblasNoTrans, sum_result_.count(), inner_dim_,
                         Dtype(1), product, sum_mult, Dtype(0), sum_result);
        }
        if (outer_dim_ != 1) {
          const Dtype* sum_mult = sum_multiplier_.data();
          Dtype* scale_diff = scale->mdiff();
          if (scale_dim_ == 1) {
            if (scale_param) {
              Dtype result = caffe_dot(outer_dim_, sum_mult, sum_result);
              *scale_diff += result;
            } else {
              *scale_diff = caffe_dot(outer_dim_, sum_mult, sum_result);
            }
          } else {
            caffe_gemv(CblasTrans, outer_dim_, scale_dim_,
                           Dtype(1), sum_result, sum_mult, Dtype(scale_param),
                           scale_diff);
          }
        }
      }
    }
    if (bottom[0]->propagate_down_) {
      const Dtype* top_diff = top[0]->diff();
      const Dtype* scale_data = scale->data();
      Dtype* bottom_diff = bottom[0]->mdiff();
      for (int n = 0; n < outer_dim_; ++n) {
        for (int d = 0; d < scale_dim_; ++d) {
          const Dtype factor = scale_data[d];
          caffe_scale(inner_dim_, factor, top_diff, bottom_diff);
          bottom_diff += inner_dim_;
          top_diff += inner_dim_;
        }
      }
    }
  }

#ifdef CPU_ONLY
  STUB_GPU(ScaleLayer);
#endif

  INSTANTIATE_CLASS(ScaleLayer);
  REGISTER_LAYER_CLASS(Scale);

}  // namespace
