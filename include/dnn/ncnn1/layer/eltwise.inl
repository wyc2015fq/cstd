

#include "eltwise.h"
#include <algorithm>

namespace ncnn
{

DEFINE_LAYER_CREATOR(Eltwise)

Eltwise::Eltwise()
{
}

int Eltwise::load_param(const ParamDict& pd)
{
  op_type = pd.get(0, 0);
  coeffs = pd.get(1, img_t());

  return 0;
}

int Eltwise::forward(const std::vector<img_t>& bottom_blobs, std::vector<img_t>& top_blobs) const
{
  const img_t* bottom_blob = bottom_blobs[0];
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = pBox_channels(bottom_blob);
  int size = w * h;

  img_t* top_blob = top_blobs[0];
  top_blob->create(w, h, channels);

  if (top_blob->empty()) {
    return -100;
  }

  if (op_type == Operation_PROD) {
    // first blob
    const img_t* bottom_blob1 = bottom_blobs[1];
    #pragma omp parallel for

    for (int q = 0; q < channels; q++) {
      const float* ptr = pBox_channel_p(bottom_blob, q);
      const float* ptr1 = bottom_blob1.channel(q);
      float* outptr = pBox_channel_p(top_blob, q);

      for (int i = 0; i < size; i++) {
        outptr[i] = ptr[i] * ptr1[i];
      }
    }

    for (size_t b = 2; b < bottom_blobs.size(); b++) {
      const img_t* bottom_blob1 = bottom_blobs[b];
      #pragma omp parallel for

      for (int q = 0; q < channels; q++) {
        const float* ptr = bottom_blob1.channel(q);
        float* outptr = pBox_channel_p(top_blob, q);

        for (int i = 0; i < size; i++) {
          outptr[i] *= ptr[i];
        }
      }
    }
  }
  else if (op_type == Operation_SUM) {
    if (coeffs.w == 0) {
      // first blob
      const img_t* bottom_blob1 = bottom_blobs[1];
      #pragma omp parallel for

      for (int q = 0; q < channels; q++) {
        const float* ptr = pBox_channel_p(bottom_blob, q);
        const float* ptr1 = bottom_blob1.channel(q);
        float* outptr = pBox_channel_p(top_blob, q);

        for (int i = 0; i < size; i++) {
          outptr[i] = ptr[i] + ptr1[i];
        }
      }

      for (size_t b = 2; b < bottom_blobs.size(); b++) {
        const img_t* bottom_blob1 = bottom_blobs[b];
        #pragma omp parallel for

        for (int q = 0; q < channels; q++) {
          const float* ptr = bottom_blob1.channel(q);
          float* outptr = pBox_channel_p(top_blob, q);

          for (int i = 0; i < size; i++) {
            outptr[i] += ptr[i];
          }
        }
      }
    }
    else {
      const float* coeffs_ptr = coeffs;

      // first blob
      const img_t* bottom_blob1 = bottom_blobs[1];
      float coeff0 = coeffs_ptr[0];
      float coeff1 = coeffs_ptr[1];
      #pragma omp parallel for

      for (int q = 0; q < channels; q++) {
        const float* ptr = pBox_channel_p(bottom_blob, q);
        const float* ptr1 = bottom_blob1.channel(q);
        float* outptr = pBox_channel_p(top_blob, q);

        for (int i = 0; i < size; i++) {
          outptr[i] = ptr[i] * coeff0 + ptr1[i] * coeff1;
        }
      }

      for (size_t b = 2; b < bottom_blobs.size(); b++) {
        const img_t* bottom_blob1 = bottom_blobs[b];
        float coeff = coeffs_ptr[b];
        #pragma omp parallel for

        for (int q = 0; q < channels; q++) {
          const float* ptr = bottom_blob1.channel(q);
          float* outptr = pBox_channel_p(top_blob, q);

          for (int i = 0; i < size; i++) {
            outptr[i] += ptr[i] * coeff;
          }
        }
      }
    }
  }
  else if (op_type == Operation_MAX) {
    // first blob
    const img_t* bottom_blob1 = bottom_blobs[1];
    #pragma omp parallel for

    for (int q = 0; q < channels; q++) {
      const float* ptr = pBox_channel_p(bottom_blob, q);
      const float* ptr1 = bottom_blob1.channel(q);
      float* outptr = pBox_channel_p(top_blob, q);

      for (int i = 0; i < size; i++) {
        outptr[i] = MAX(ptr[i], ptr1[i]);
      }
    }

    for (size_t b = 2; b < bottom_blobs.size(); b++) {
      const img_t* bottom_blob1 = bottom_blobs[b];
      #pragma omp parallel for

      for (int q = 0; q < channels; q++) {
        const float* ptr = bottom_blob1.channel(q);
        float* outptr = pBox_channel_p(top_blob, q);

        for (int i = 0; i < size; i++) {
          outptr[i] = MAX(outptr[i], ptr[i]);
        }
      }
    }
  }

  return 0;
}

} // namespace ncnn
