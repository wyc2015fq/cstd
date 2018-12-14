

void FUN(BatchNormalizationForwardInference)(int num, int channels, int inner_num_, const Dtype *x, Dtype *y, const Dtype *bnScale, const Dtype *bnBias, const Dtype *estimatedMean, const Dtype *estimatedVariance, Stype epsilon) {
  int count = num * channels * inner_num_;
  CPU_KERNEL_LOOP(i, count) {
    if (i == 143300) {
      int adsf = 0;
    }
    int k = (i / inner_num_) % channels;
    y[i] = (Dtype)((bnScale[k] * (x[i] - estimatedMean[k]) / sqrt(epsilon + estimatedVariance[k])) + bnBias[k]);
  }
}
void FUN(getMean)(int N, int C, int M, const Dtype* X, Dtype* mean, Stype factor) {
  int NM = N*M;
  Stype ss = NM*(1 - factor)/ factor;
  CPU_KERNEL_LOOP(c, C) {
    mean[c] *= ss;
  }
  CPU_KERNEL_LOOP(n, N) {
    CPU_KERNEL_LOOP(c, C) {
      CPU_KERNEL_LOOP(m, M) {
        Dtype x = *X++;
        mean[c] += x;
      }
    }
  }
  ss = factor / NM;
  CPU_KERNEL_LOOP(c, C) {
    mean[c] = mean[c] * ss;
  }
}
//  E(X^2)-E(X)^2
void FUN(getVar)(int N, int C, int M, const Dtype* X, const Dtype* mean, Dtype* var) {
  int NM = N*M;
  Stype ss = (NM / (NM - 1.0));
  CPU_KERNEL_LOOP(c, C) {
    var[c] = 0;
  }
  CPU_KERNEL_LOOP(n, N) {
    CPU_KERNEL_LOOP(c, C) {
      CPU_KERNEL_LOOP(m, M) {
        Dtype x = *X++;
        var[c] += x*x;
      }
    }
  }
  CPU_KERNEL_LOOP(c, C) {
    Dtype y = mean[c];
    var[c] = (var[c] / NM - y*y)*ss;
  }
}

void FUN(getMeanKernel)(int N, int C, int M, const Dtype* X, Dtype* mean, Stype factor) {
  CPU_KERNEL_LOOP(c, C) {
    double s = 0;
    Dtype meanc = mean[c];
    for (int n = 0; n < N; ++n) {
      int off = (n*C + c)*M;
      for (int m = 0; m < M; ++m) {
        s += X[off + m];
      }
    }
    s /= (N*M);
    mean[c] = s*factor + (1.0 - factor)*meanc;
  }
}

void FUN(getVarKernel)(int N, int C, int M, const Dtype* X, const Dtype* mean, Dtype* var) {
  CPU_KERNEL_LOOP(c, C) {
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

DEF(void, BatchNormalizationForwardTraining, (int N, int C, int M, Dtype* X, Dtype* Y, const Dtype* scaler, const Dtype* bias, Stype factor, Dtype* runningMean, Dtype* runningVar, Stype epsilon, Dtype* batchMean, Dtype* batchInvVar))
{
  int NM = N * M;
  FUN(getMeanKernel)(N, C, M, X, runningMean, factor);
  FUN(getVarKernel)(N, C, M, X, runningMean, runningVar);
  for (int c = 0; c < C; ++c) {
    batchMean[c] = runningMean[c];
    batchInvVar[c] = 1.0 / sqrt(runningVar[c] + epsilon);
  }
  //process data
  for (int n = 0; n < N; ++n) {
    for (int c = 0; c < C; ++c) {
      for (int m = 0; m < M; ++m) {
        Dtype x = *X++;
        *Y++ = scaler[c] * (x - runningMean[c]) * batchInvVar[c] + bias[c];
      }
    }
  }
}
