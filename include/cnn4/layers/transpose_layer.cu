
__global__ void FUN(transpose_gpu)(const int nthreads, const Dtype* from_data, Dtype* to_data,
	const DataShape from_counts, const DataShape to_counts, const DataShape map, const int num_axes) {
  int from_inds[kMaxBlobAxes] = { 0 };
  CUDA_KERNEL_LOOP(index, nthreads) {
  	int from_index = index, to_index = 0;
  	for(int i = 0; i < num_axes; i++) {
		from_inds[i] = from_index / from_counts.dim[i];
		from_index = from_index % from_counts.dim[i];
	}
	for(int i = 0; i < num_axes; i++) {
		to_index += from_inds[map.dim[i]] * to_counts.dim[i];
	}

	*(to_data + to_index) = *(from_data + index);
  }
}

void FUN(transpose)(const int nthreads, const Dtype* from_data, Dtype* to_data, const DataShape from_counts, const DataShape to_counts, const DataShape map, const int num_axes) {
  FUN(transpose_gpu) << <CAFFE_GET_BLOCKS(nthreads), CAFFE_CUDA_NUM_THREADS >> >
    (nthreads, from_data, to_data, from_counts, to_counts, map, num_axes);
}
