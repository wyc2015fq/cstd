

__global__ void FUN(BatchNormalizationForwardInferenceKernel)(int count, int channels, int inner_num_, const Dtype *x, Dtype *y, const Dtype *bnScale, const Dtype *bnBias, const Dtype *estimatedMean, const Dtype *estimatedVariance, Dtype epsilon) {
  CUDA_KERNEL_LOOP(i, count) {
    int k = (i/inner_num_) % channels;
    y[i] = bnScale[k] * (x[i] - estimatedMean[k]) / sqrt(epsilon + estimatedVariance[k]) + bnBias[k];
  }
}

void FUN(BatchNormalizationForwardInference)(int count, int channels, int inner_num_, const Dtype *x, Dtype *y, const Dtype *bnScale, const Dtype *bnBias, const Dtype *estimatedMean, const Dtype *estimatedVariance, Stype epsilon) {
  FUN(BatchNormalizationForwardInferenceKernel) <<<CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS >>> (count, channels, inner_num_, x, y, bnScale, bnBias, estimatedMean, estimatedVariance, epsilon);
}


#if 0
__global__ void FUN(kernel_spatial_sum)(const int num, const int channels,
  const int spatial_dim, const Dtype* data, Dtype* channel_sum) {
  CUDA_KERNEL_LOOP(index, channels) {
    int n = index / spatial_dim;
    int s = index % spatial_dim;
    Dtype sum = 0;
    for (int c = 0; c < channels; ++c) {
      sum += data[(n * channels + c) * spatial_dim + s];
    }
    for (int n = 0; n<num; ++n)
    channel_sum[index] = sum;
  }
}
#endif

