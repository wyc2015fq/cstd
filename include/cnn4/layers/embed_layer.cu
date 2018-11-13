

#include "caffe/filler.hpp"
#include "caffe/layers/embed_layer.hpp"
#include "caffe/util/gpu_util.cuh"


namespace {

template <typename Dtype>
__global__ void EmbedForward(const int nthreads, const Dtype* bottom_data,
    const Dtype* weight, const int M, const int N, const int K,
    Dtype* top_data) {
  CUDA_KERNEL_LOOP(top_index, nthreads) {
    const int n = top_index / N;
    const int d = top_index % N;
    const int index = static_cast<int>(bottom_data[n]);
    const int weight_index = index * N + d;
    top_data[top_index] = weight[weight_index];
  }
}

template <typename Dtype>
__global__ void EmbedBackward(const int nthreads, const Dtype* bottom_data,
    const Dtype* top_diff, const int M, const int N, const int K,
    Dtype* weight_diff);

template <typename Dtype>
__global__ void EmbedBackward(const int nthreads, const Dtype* bottom_data,
    const Dtype* top_diff, const int M, const int N, const int K,
    Dtype* weight_diff) {
  CUDA_KERNEL_LOOP(top_index, nthreads) {
    const int n = top_index / N;
    const int d = top_index % N;
    const int index = static_cast<int>(bottom_data[n]);
    const int weight_index = index * N + d;
    caffe_gpu_atomic_add(top_diff[top_index], weight_diff + weight_index);
  }
}

template <typename Dtype>
void EmbedLayer::Forward(GPUContext* context, const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* bottom_data = bottom[0]->data();
  Dtype* top_data = top[0]->mdata();
  const Dtype* weight = this->blobs_[0]->data();
  const int count = top[0]->count();
  EmbedForward<Dtype>  // NOLINT_NEXT_LINE(whitespace/operators)
      <<<CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS>>>(
      count, bottom_data, weight, M_, N_, K_, top_data);
  if (bias_term_) {
    caffe_gpu_gemm<Dtype>(CblasNoTrans, CblasNoTrans, M_, N_, 1, Dtype(1),
        bias_multiplier_.data(),
        this->blobs_[1]->data(), Dtype(1), top_data);
  }
}

template <typename Dtype>
void EmbedLayer::Backward(GPUContext* context, const vector<Blob*>& top,
    const vector<Blob*>& bottom) {
  CHECK(!bottom[0]->propagate_down_) << "Can't backpropagate to EmbedLayer input.";
  if (this->blobs_[0]->propagate_down_) {
    const int top_count = top[0]->count();
    const Dtype* top_diff = top[0]->gpu_diff();
    const Dtype* bottom_data = bottom[0]->data();
    Dtype* weight_diff = this->blobs_[0]->gpu_mdiff();
    EmbedBackward<Dtype>  // NOLINT_NEXT_LINE(whitespace/operators)
        <<<CAFFE_GET_BLOCKS(top_count), CAFFE_CUDA_NUM_THREADS>>>(
        top_count, bottom_data, top_diff, M_, N_, K_, weight_diff);
  }
  if (bias_term_ && this->blobs_[1]->propagate_down_) {
    const Dtype* top_diff = top[0]->gpu_diff();
    Dtype* bias_diff = this->blobs_[1]->gpu_mdiff();
    caffe_gpu_gemv<Dtype>(CblasTrans, M_, N_, Dtype(1), top_diff,
        bias_multiplier_.data(), Dtype(1), bias_diff);
  }
}

INSTANTIATE_LAYER_GPU_FUNCS(EmbedLayer);

}  // namespace
