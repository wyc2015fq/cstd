

__global__ void FUN(BatchNormalizationForwardInferenceKernel)(int count, int C, int M, const Dtype *X, Dtype *Y, const Dtype *bnScale, const Dtype *bnBias, const Dtype *runningMean, const Dtype *runningVar, Dtype epsilon) {
  CUDA_KERNEL_LOOP(i, count) {
    int c = (i/M) % C;
    Y[i] = bnScale[c] * (X[i] - runningMean[c]) / sqrt(epsilon + runningVar[c]) + bnBias[c];
  }
}

void FUN(BatchNormalizationForwardInference)(int N, int C, int M, const Dtype *X, Dtype *Y, const Dtype *bnScale, const Dtype *bnBias, const Dtype *runningMean, const Dtype *runningVar, Stype epsilon) {
  int count = N * C * M;
  FUN(BatchNormalizationForwardInferenceKernel) <<<CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS >>> (count, C, M, X, Y, bnScale, bnBias, runningMean, runningVar, epsilon);
}

__global__ void FUN(getMeanKernel)(int N, int C, int M, const Dtype* X, Dtype* mean, Stype factor) {
  CUDA_KERNEL_LOOP(c, C) {
    double s = 0;
    Dtype meanc = mean[c];
    for (int n = 0; n < N; ++n) {
      int off = (n*C + c)*M;
      for (int m = 0; m < M; ++m) {
        s += X[off + m];
      }
    }
    s /= (N*M);
    mean[c] = s*factor + (1.0-factor)*meanc;
  }
}

void FUN(getMean)(int N, int C, int M, const Dtype* X, Dtype* mean, Stype factor) {
  FUN(getMeanKernel) <<<CAFFE_GET_BLOCKS(C), CAFFE_CUDA_NUM_THREADS >>> (N, C, M, X, mean, factor);
}

__global__ void FUN(getVarKernel)(int N, int C, int M, const Dtype* X, const Dtype* mean, Dtype* var) {
  CUDA_KERNEL_LOOP(c, C) {
    double s = 0;
    Dtype meanc = mean[c];
    for (int n = 0; n < N; ++n) {
      int off = (n*C + c)*M;
      for (int m = 0; m < M; ++m) {
        double t = X[off + m] - meanc;
        s += t*t;
      }
    }
    s /= (N*M - 1);
    var[c] = s;
  }
}

void FUN(getVar)(int N, int C, int M, const Dtype* X, const Dtype* mean, Dtype* var) {
  FUN(getVarKernel) << <CAFFE_GET_BLOCKS(C), CAFFE_CUDA_NUM_THREADS >> > (N, C, M, X, mean, var);
}

__global__ void FUN(BatchNormalizationForwardTrainingUpdate)(int C, const Dtype* runningMean, const Dtype* runningVar, Dtype* batchMean, Dtype* batchInvVar, Stype epsilon) {
  CUDA_KERNEL_LOOP(c, C) {
    batchMean[c] = runningMean[c];
    batchInvVar[c] = 1.0 / sqrt(runningVar[c] + epsilon);
  }
}
__global__ void FUN(BatchNormalizationForwardTrainingKernel)(int count, int C, int M, const Dtype *X, Dtype *Y, const Dtype *bnScale, const Dtype *bnBias, const Dtype *runningMean, const Dtype *invVar) {
  CUDA_KERNEL_LOOP(i, count) {
    int c = (i / M) % C;
    Y[i] = bnScale[c] * (X[i] - runningMean[c]) * invVar [c] + bnBias[c];
  }
}
DEF(void, BatchNormalizationForwardTraining, (int N, int C, int M, Dtype* X, Dtype* Y, const Dtype* bnScale, const Dtype* bnBias, Stype factor, Dtype* runningMean, Dtype* runningVar, Stype epsilon, Dtype* batchMean, Dtype* batchInvVar))
{
  //FUN(getMean)(N, C, M, X, runningMean, factor);
  //FUN(getVar)(N, C, M, X, runningMean, runningVar);
  FUN(getMeanKernel) << <CAFFE_GET_BLOCKS(C), CAFFE_CUDA_NUM_THREADS >> > (N, C, M, X, runningMean, factor);
  FUN(getVarKernel) << <CAFFE_GET_BLOCKS(C), CAFFE_CUDA_NUM_THREADS >> > (N, C, M, X, runningMean, runningVar);
  FUN(BatchNormalizationForwardTrainingUpdate) << <CAFFE_GET_BLOCKS(C), CAFFE_CUDA_NUM_THREADS >> > (C, runningMean, runningVar, batchMean, batchInvVar, epsilon);
  //process data
  int count = N * C * M;
  FUN(BatchNormalizationForwardTrainingKernel) <<<CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS >> > (count, C, M, X, Y, bnScale, bnBias, runningMean, batchInvVar);
}
