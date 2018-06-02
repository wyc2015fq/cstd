

#include "bias_arm.h"

#if __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

namespace ncnn
{

DEFINE_LAYER_CREATOR(Bias_arm)

int Bias_arm::forward(const img_t* bottom_blob, img_t* top_blob) const
{
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = pBox_channels(bottom_blob);
  int size = w * h;

  top_blob->create(w, h, channels);

  if (top_blob->empty()) {
    return -100;
  }

  const float* bias_ptr = bias_data;
  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    const float* ptr = pBox_channel_p(bottom_blob, q);
    float* outptr = pBox_channel_p(top_blob, q);

    float bias = bias_ptr[q];

#if __ARM_NEON
    int nn = size >> 2;
    int remain = size - (nn << 2);
#else
    int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON
    float32x4_t _bias = vdupq_n_f32(bias);

    for (; nn > 0; nn--) {
      float32x4_t _p = vld1q_f32(ptr);
      float32x4_t _outp = vaddq_f32(_p, _bias);
      vst1q_f32(outptr, _outp);

      ptr += 4;
      outptr += 4;
    }

#endif // __ARM_NEON

    for (; remain > 0; remain--) {
      *outptr = *ptr + bias;

      ptr++;
      outptr++;
    }
  }

  return 0;
}

int Bias_arm::forward_inplace(img_t* bottom_top_blob) const
{
  int w = bottom_top_blob->w;
  int h = bottom_top_blob->h;
  int channels = pBox_channels(bottom_top_blob);
  int size = w * h;

  const float* bias_ptr = bias_data;
  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    float* ptr = pBox_channel_p(bottom_top_blob, q);

    float bias = bias_ptr[q];

#if __ARM_NEON
    int nn = size >> 2;
    int remain = size - (nn << 2);
#else
    int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON
    float32x4_t _bias = vdupq_n_f32(bias);

    for (; nn > 0; nn--) {
      float32x4_t _p = vld1q_f32(ptr);
      float32x4_t _outp = vaddq_f32(_p, _bias);
      vst1q_f32(ptr, _outp);

      ptr += 4;
    }

#endif // __ARM_NEON

    for (; remain > 0; remain--) {
      *ptr = *ptr + bias;

      ptr++;
    }
  }

  return 0;
}

} // namespace ncnn
