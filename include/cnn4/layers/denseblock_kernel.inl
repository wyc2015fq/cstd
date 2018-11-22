

void FUN(BatchNormalizationForwardInference)(int count, int channels, int inner_num_, const Dtype *x, Dtype *y, const Dtype *bnScale, const Dtype *bnBias, const Dtype *estimatedMean, const Dtype *estimatedVariance, Stype epsilon) {
  CPU_KERNEL_LOOP(i, count) {
    if (i == 143300) {
      int adsf = 0;
    }
    int k = (i / inner_num_) % channels;
    y[i] = (bnScale[k] * (x[i] - estimatedMean[k]) / sqrt(epsilon + estimatedVariance[k])) + bnBias[k];
  }
}
