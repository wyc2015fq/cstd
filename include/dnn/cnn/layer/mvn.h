

struct MVN : public Layer
{
public:
  MVN();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const Blob* prev, Blob* next) const;

public:
  int normalize_variance;
  int across_channels;
  float eps;
};


MVN::MVN()
{
  one_blob_only = true;
  support_inplace = false;
}

int MVN::load_param(const ParamDict& pd)
{
  normalize_variance = pd.get(0, 0);
  across_channels = pd.get(1, 0);
  eps = pd.get(2, 0.0001f);

  return 0;
}

int MVN::forward(const Blob* prev, Blob* next) const
{
  int w = prevSize.w;
  int h = prevSize.h;
  int channels = prevSize.c;
  int size = w * h;

  next->create(w, h, channels);

  if (next->empty()) {
    return -100;
  }

  // prepare sum per channel
  Blob sum(channels);

  if (sum.empty()) {
    return -100;
  }

  float* sum_ptr = sum;

  #pragma omp parallel for

  for (q = 0; q < channels; q++) {
    const float* ptr = prevData + q * Data_2DSize(prevSize);

    float sum = 0.f;

    for (i = 0; i < size; i++) {
      sum += ptr[i];
    }

    sum_ptr[q] = sum;
  }

  if (across_channels) {
    // compute mean across channels
    float mean = 0.f;

    for (q = 0; q < channels; q++) {
      mean += sum_ptr[q];
    }

    mean = mean / (channels * size);

    // subtract mean
    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      const float* ptr = prevData + q * Data_2DSize(prevSize);
      float* outptr = nextData + q * Data_2DSize(nextSize);

      for (i = 0; i < size; i++) {
        outptr[i] = ptr[i] - mean;
      }
    }
  }
  else {
    // subtract mean
    #pragma omp parallel for
    for (q = 0; q < channels; q++) {
      const float* ptr = prevData + q * Data_2DSize(prevSize);
      float* outptr = nextData + q * Data_2DSize(nextSize);
      float mean = sum_ptr[q] / size;

      for (i = 0; i < size; i++) {
        outptr[i] = ptr[i] - mean;
      }
    }
  }

  if (normalize_variance) {
    // prepare squared sum per channel
    Blob sqsum(channels);

    if (sqsum.empty()) {
      return -100;
    }

    float* sqsum_ptr = sqsum;

    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      const float* ptr = nextData + q * Data_2DSize(nextSize);

      float sum = 0.f;

      for (i = 0; i < size; i++) {
        sum += ptr[i] * ptr[i];
      }

      sqsum_ptr[q] = sum;
    }

    if (across_channels) {
      // compute squared mean across channels
      float sqmean = 0.f;

      for (q = 0; q < channels; q++) {
        sqmean += sqsum_ptr[q];
      }

      sqmean = sqmean / (channels * size);

      // normalize variance
      float norm_var = sqrt(sqmean) + eps;
      float norm_var_inv = 1.f / norm_var;

      // apply normalize_variance
      #pragma omp parallel for

      for (q = 0; q < channels; q++) {
        float* outptr = nextData + q * Data_2DSize(nextSize);

        for (i = 0; i < size; i++) {
          outptr[i] = outptr[i] * norm_var_inv;
        }
      }
    }
    else {
      // apply normalize_variance
      #pragma omp parallel for
      for (q = 0; q < channels; q++) {
        float* outptr = nextData + q * Data_2DSize(nextSize);
        float sqmean = sqsum_ptr[q] / size;
        float norm_var = sqrt(sqmean) + eps;
        float norm_var_inv = 1.f / norm_var;

        for (i = 0; i < size; i++) {
          outptr[i] = outptr[i] * norm_var_inv;
        }
      }
    }

  }

  return 0;
}

} // namespace ncnn
