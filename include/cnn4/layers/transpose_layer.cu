#include "caffe/layers/transpose_layer.hpp"

namespace {

template <typename Dtype>
__global__ void transpose_gpu(const int nthreads, const Dtype* from_data, Dtype* to_data, 
	const int* from_counts, const int* to_counts, const int* map, const int num_axes, int* buf) {
  CUDA_KERNEL_LOOP(index, nthreads) {
  	int* from_inds=buf + index * num_axes;

  	int from_index = index, to_index = 0;
  	for(int i = 0; i < num_axes; i++) {
		from_inds[i] = from_index / from_counts[i];
		from_index = from_index % from_counts[i];
	}
	for(int i = 0; i < num_axes; i++) {
		to_index += from_inds[map[i]] * to_counts[i];
	}

	*(to_data + to_index) = *(from_data + index);
  }
}

template <typename Dtype>
void TransposeLayer<Dtype>::Forward(GPUContext* context, const vector<Blob<Dtype>*>& bottom, 
		const vector<Blob<Dtype>*>& top) {
	const int nthreads = bottom[0]->count();

	transpose_gpu<Dtype>  // NOLINT_NEXT_LINE(whitespace/operators)
        <<<CAFFE_GET_BLOCKS(nthreads), CAFFE_CUDA_NUM_THREADS>>>(
        nthreads, bottom[0]->data<Context>(), top[0]->mutable_data<Context>(), 
        bottom_counts_.data<Context>(), top_counts_.data<Context>(), forward_map_.data<Context>(), 
          (int)bottom[0]->shape().size(), buf_.mutable_data<Context>());
}

template <typename Dtype>
void TransposeLayer<Dtype>::Backward(GPUContext* context, const vector<Blob<Dtype>*>& top,
    	const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom) {
	if (!top[0]->propagate_down_) {
		return;
	}
	const int nthreads = bottom[0]->count();

	transpose_gpu<Dtype>  // NOLINT_NEXT_LINE(whitespace/operators)
        <<<CAFFE_GET_BLOCKS(nthreads), CAFFE_CUDA_NUM_THREADS>>>(
        nthreads, top[0]->gpu_diff(), bottom[0]->mutable_gpu_diff(), 
        top_counts_.data<Context>(), bottom_counts_.data<Context>(), backward_map_.data<Context>(), 
        (int)bottom[0]->shape().size(), buf_.mutable_data<Context>());
}

INSTANTIATE_LAYER_GPU_FUNCS(TransposeLayer);

}  // namespace
