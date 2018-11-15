#include <algorithm>


#include "caffe/layers/contrastive_loss_layer.hpp"


namespace {

template <typename Dtype>
void ContrastiveLossLayer::Forward_gpu(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
  const int count = bottom[0]->count();
  caffe_gpu_sub(
      count,
      bottom[0]->data(),  // a
      bottom[1]->data(),  // b
      diff_.mdata());  // a_i-b_i
  caffe_gpu_powx(
      count,
      diff_.mdata(),  // a_i-b_i
      Dtype(2),
      diff_sq_.mdata());  // (a_i-b_i)^2
  caffe_gpu_gemv(
      CblasNoTrans,
      bottom[0]->num(),
      bottom[0]->channels(),
      Dtype(1.0),
      diff_sq_.data(),  // (a_i-b_i)^2
      summer_vec_.data(),
      Dtype(0.0),
      dist_sq_.mdata());  // \Sum (a_i-b_i)^2
  Dtype margin = this->param_->contrastive_loss_param().margin();
  bool legacy_version =
      this->param_->contrastive_loss_param().legacy_version();
  Dtype loss(0.0);
  for (int i = 0; i < bottom[0]->num(); ++i) {
    if (static_cast<int>(bottom[2]->data()[i])) {  // similar pairs
      loss += dist_sq_.data()[i];
    } else {  // dissimilar pairs
      if (legacy_version) {
        loss += std::max(margin - dist_sq_.data()[i], Dtype(0.0));
      } else {
        Dtype dist = std::max(margin - sqrt(dist_sq_.data()[i]),
                              Dtype(0.0));
        loss += dist*dist;
      }
    }
  }
  loss = loss / static_cast<Dtype>(bottom[0]->num()) / Dtype(2);
  top[0]->mdata()[0] = loss;
}

template <typename Dtype>
__global__ void CLLBackward(const int count, const int channels,
    const Dtype margin, const bool legacy_version, const Dtype alpha,
    const Dtype* y, const Dtype* diff, const Dtype* dist_sq,
    Dtype *bottom_diff) {
  CUDA_KERNEL_LOOP(i, count) {
    int n = i / channels;  // the num index, to access y and dist_sq
    if (static_cast<int>(y[n])) {  // similar pairs
      bottom_diff[i] = alpha * diff[i];
    } else {  // dissimilar pairs
      Dtype mdist(0.0);
      Dtype beta(0.0);
      if (legacy_version) {
        mdist = (margin - dist_sq[n]);
        beta = -alpha;
      } else {
        Dtype dist = sqrt(dist_sq[n]);
        mdist = (margin - dist);
        beta = -alpha * mdist / (dist + Dtype(1e-4)) * diff[i];
      }
      if (mdist > 0.0) {
        bottom_diff[i] = beta;
      } else {
        bottom_diff[i] = 0;
      }
    }
  }
}

template <typename Dtype>
void ContrastiveLossLayer::Backward_(GPUContext* context, const vector<Blob*>& top,
    const vector<Blob*>& bottom) {
  for (int i = 0; i < 2; ++i) {
    if (bottom[i]->propagate_down_) {
      const int count = bottom[0]->count();
      const int channels = bottom[0]->channels();
      Dtype margin = this->param_->contrastive_loss_param().margin();
      const bool legacy_version =
          this->param_->contrastive_loss_param().legacy_version();
      const Dtype sign = Dtype((i == 0) ? 1 : -1);
      const Dtype alpha = sign * top[0]->diff()[0] /
          static_cast<Dtype>(bottom[0]->num());
      // NOLINT_NEXT_LINE(whitespace/operators)
      CLLBackward<Dtype><<<CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS>>>(
          count, channels, margin, legacy_version, alpha,
          bottom[2]->data(),  // pair similarity 0 or 1
          diff_.data(),  // the cached eltwise difference between a and b
          dist_sq_.data(),  // the cached square distance between a and b
          bottom[i]->gpu_mdiff());
      CUDA_POST_KERNEL_CHECK;
    }
  }
}

INSTANTIATE_LAYER_GPU_FUNCS(ContrastiveLossLayer);

}  // namespace
