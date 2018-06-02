

#include "scale_arm.h"

#if __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

namespace ncnn
{

DEFINE_LAYER_CREATOR(Scale_arm)

int Scale_arm::forward(const Blob* bottom_blob, Blob* top_blob) const
{
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = Blob_channels(bottom_blob);
  int size = w * h;

  top_blob->create(w, h, channels);

  if (top_blob->empty()) {
    return -100;
  }

  if (bias_term) {
    const float* scale_ptr = scale_data;
    const float* bias_ptr = bias_data;
    #pragma omp parallel for

    for (int q = 0; q < channels; q++) {
      const float* ptr = Blob_channel_p(bottom_blob, q);
      float* outptr = Blob_channel_p(top_blob, q);

      float s = scale_ptr[q];
      float bias = bias_ptr[q];

#if __ARM_NEON
      int nn = size >> 2;
      int remain = size - (nn << 2);
#else
      int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON
      float32x4_t _s = vdupq_n_f32(s);
      float32x4_t _bias = vdupq_n_f32(bias);

      for (; nn > 0; nn--) {
        float32x4_t _p = vld1q_f32(ptr);
        _p = vmlaq_f32(_bias, _p, _s);
        vst1q_f32(outptr, _p);

        ptr += 4;
        outptr += 4;
      }

#endif // __ARM_NEON

      for (; remain > 0; remain--) {
        *outptr = *ptr * s + bias;

        ptr++;
        outptr++;
      }
    }
  }
  else {
    const float* scale_ptr = scale_data;
    #pragma omp parallel for

    for (int q = 0; q < channels; q++) {
      const float* ptr = Blob_channel_p(bottom_blob, q);
      float* outptr = Blob_channel_p(top_blob, q);

      float s = scale_ptr[q];

#if __ARM_NEON
      int nn = size >> 2;
      int remain = size - (nn << 2);
#else
      int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON
      float32x4_t _s = vdupq_n_f32(s);

      for (; nn > 0; nn--) {
        float32x4_t _p = vld1q_f32(ptr);
        _p = vmulq_f32(_p, _s);
        vst1q_f32(outptr, _p);

        ptr += 4;
        outptr += 4;
      }

#endif // __ARM_NEON

      for (; remain > 0; remain--) {
        *outptr = *ptr * s;

        ptr++;
        outptr++;
      }
    }
  }

  return 0;
}

int Scale_arm::forward_inplace(Blob* bottom_top_blob) const
{
  int w = bottom_top_blob->w;
  int h = bottom_top_blob->h;
  int channels = Blob_channels(bottom_top_blob);
  int size = w * h;

  if (bias_term) {
    const float* scale_ptr = scale_data;
    const float* bias_ptr = bias_data;
    #pragma omp parallel for

    for (int q = 0; q < channels; q++) {
      float* ptr = Blob_channel_p(bottom_top_blob, q);

      float s = scale_ptr[q];
      float bias = bias_ptr[q];

#if __ARM_NEON
      int nn = size >> 2;
      int remain = size - (nn << 2);
#else
      int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON
      float32x4_t _s = vdupq_n_f32(s);
      float32x4_t _bias = vdupq_n_f32(bias);

      for (; nn > 0; nn--) {
        float32x4_t _p = vld1q_f32(ptr);
        _p = vmlaq_f32(_bias, _p, _s);
        vst1q_f32(ptr, _p);

        ptr += 4;
      }

#endif // __ARM_NEON

      for (; remain > 0; remain--) {
        *ptr = *ptr * s + bias;

        ptr++;
      }
    }
  }
  else {
    const float* scale_ptr = scale_data;
    #pragma omp parallel for

    for (int q = 0; q < channels; q++) {
      float* ptr = Blob_channel_p(bottom_top_blob, q);

      float s = scale_ptr[q];

#if __ARM_NEON
      int nn = size >> 2;
      int remain = size - (nn << 2);
#else
      int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON
      float32x4_t _s = vdupq_n_f32(s);

      for (; nn > 0; nn--) {
        float32x4_t _p = vld1q_f32(ptr);
        _p = vmulq_f32(_p, _s);
        vst1q_f32(ptr, _p);

        ptr += 4;
      }

#endif // __ARM_NEON

      for (; remain > 0; remain--) {
        *ptr *= s;

        ptr++;
      }
    }
  }

  return 0;
}

} // namespace ncnn
