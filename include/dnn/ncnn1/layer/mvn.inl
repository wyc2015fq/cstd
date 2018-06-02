

#include "mvn.h"


namespace ncnn
{

DEFINE_LAYER_CREATOR(MVN)

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

int MVN::forward(const img_t* bottom_blob, img_t* top_blob) const
{
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = pBox_channels(bottom_blob);
  int size = w * h;

  top_blob->create(w, h, channels);

  if (top_blob->empty()) {
    return -100;
  }

  // prepare sum per channel
  img_t sum(channels);

  if (sum.empty()) {
    return -100;
  }

  float* sum_ptr = sum;

  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    const float* ptr = pBox_channel_p(bottom_blob, q);

    float sum = 0.f;

    for (int i = 0; i < size; i++) {
      sum += ptr[i];
    }

    sum_ptr[q] = sum;
  }

  if (across_channels) {
    // compute mean across channels
    float mean = 0.f;

    for (int q = 0; q < channels; q++) {
      mean += sum_ptr[q];
    }

    mean = mean / (channels * size);

    // subtract mean
    #pragma omp parallel for

    for (int q = 0; q < channels; q++) {
      const float* ptr = pBox_channel_p(bottom_blob, q);
      float* outptr = pBox_channel_p(top_blob, q);

      for (int i = 0; i < size; i++) {
        outptr[i] = ptr[i] - mean;
      }
    }
  }
  else {
    // subtract mean
    #pragma omp parallel for
    for (int q = 0; q < channels; q++) {
      const float* ptr = pBox_channel_p(bottom_blob, q);
      float* outptr = pBox_channel_p(top_blob, q);
      float mean = sum_ptr[q] / size;

      for (int i = 0; i < size; i++) {
        outptr[i] = ptr[i] - mean;
      }
    }
  }

  if (normalize_variance) {
    // prepare squared sum per channel
    img_t sqsum(channels);

    if (sqsum.empty()) {
      return -100;
    }

    float* sqsum_ptr = sqsum;

    #pragma omp parallel for

    for (int q = 0; q < channels; q++) {
      const float* ptr = pBox_channel_p(top_blob, q);

      float sum = 0.f;

      for (int i = 0; i < size; i++) {
        sum += ptr[i] * ptr[i];
      }

      sqsum_ptr[q] = sum;
    }

    if (across_channels) {
      // compute squared mean across channels
      float sqmean = 0.f;

      for (int q = 0; q < channels; q++) {
        sqmean += sqsum_ptr[q];
      }

      sqmean = sqmean / (channels * size);

      // normalize variance
      float norm_var = sqrt(sqmean) + eps;
      float norm_var_inv = 1.f / norm_var;

      // apply normalize_variance
      #pragma omp parallel for

      for (int q = 0; q < channels; q++) {
        float* outptr = pBox_channel_p(top_blob, q);

        for (int i = 0; i < size; i++) {
          outptr[i] = outptr[i] * norm_var_inv;
        }
      }
    }
    else {
      // apply normalize_variance
      #pragma omp parallel for
      for (int q = 0; q < channels; q++) {
        float* outptr = pBox_channel_p(top_blob, q);
        float sqmean = sqsum_ptr[q] / size;
        float norm_var = sqrt(sqmean) + eps;
        float norm_var_inv = 1.f / norm_var;

        for (int i = 0; i < size; i++) {
          outptr[i] = outptr[i] * norm_var_inv;
        }
      }
    }

  }

  return 0;
}

} // namespace ncnn
