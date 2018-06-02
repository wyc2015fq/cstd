

#include "softmax_arm.h"
#include <float.h>
#include <math.h>

#if __ARM_NEON
#include <arm_neon.h>
#include "neon_mathfun.h"
#endif // __ARM_NEON

namespace ncnn
{

DEFINE_LAYER_CREATOR(Softmax_arm)

int Softmax_arm::forward(const img_t* bottom_blob, img_t* top_blob) const
{
  if (axis != 0) {
    return Softmax::forward(bottom_blob, top_blob);
  }

  // value = exp( value - global max value )
  // sum all value
  // value = value / sum

  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = pBox_channels(bottom_blob);
  int size = w * h;

  top_blob->create(w, h, channels);

  if (top_blob->empty()) {
    return -100;
  }

  img_t max;
  max.create(w, h);

  if (max.empty()) {
    return -100;
  }

  max.fill(-FLT_MAX);

  for (int q = 0; q < channels; q++) {
    const float* ptr = pBox_channel_p(bottom_blob, q);
    float* maxptr = max;

    for (int i = 0; i < size; i++) {
      maxptr[i] = MAX(maxptr[i], ptr[i]);
    }
  }

  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    const float* ptr = pBox_channel_p(bottom_blob, q);
    float* outptr = pBox_channel_p(top_blob, q);
    float* maxptr = max;

#if __ARM_NEON
    int nn = size >> 2;
    int remain = size - (nn << 2);
#else
    int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON

    for (; nn > 0; nn--) {
      float32x4_t _p = vld1q_f32(ptr);
      float32x4_t _max = vld1q_f32(maxptr);

      _p = exp_ps(vsubq_f32(_p, _max));

      vst1q_f32(outptr, _p);

      ptr += 4;
      maxptr += 4;
      outptr += 4;
    }

#endif // __ARM_NEON

    for (; remain > 0; remain--) {
      *outptr = exp(*ptr - *maxptr);

      ptr++;
      maxptr++;
      outptr++;
    }
  }

  img_t sum;
  sum.create(w, h);

  if (sum.empty()) {
    return -100;
  }

  sum.fill(0.f);

  for (int q = 0; q < channels; q++) {
    const float* outptr = pBox_channel_p(top_blob, q);
    float* sumptr = sum;

    for (int i = 0; i < size; i++) {
      sumptr[i] += outptr[i];
    }
  }

  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    float* outptr = pBox_channel_p(top_blob, q);
    float* sumptr = sum;

#if __ARM_NEON
    int nn = size >> 2;
    int remain = size - (nn << 2);
#else
    int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON

    for (; nn > 0; nn--) {
      float32x4_t _p = vld1q_f32(outptr);
      float32x4_t _sum = vld1q_f32(sumptr);
#if __aarch64__
      _p = vdivq_f32(_p, _sum);
#else
      _p = div_ps(_p, _sum);
#endif // __aarch64__
      vst1q_f32(outptr, _p);

      outptr += 4;
      sumptr += 4;
    }

#endif // __ARM_NEON

    for (; remain > 0; remain--) {
      *outptr /= *sumptr;

      outptr++;
      sumptr++;
    }
  }

  return 0;
}

int Softmax_arm::forward_inplace(img_t* bottom_top_blob) const
{
  if (axis != 0) {
    return Softmax::forward_inplace(bottom_top_blob);
  }

  // value = exp( value - global max value )
  // sum all value
  // value = value / sum

  int w = bottom_top_blob->w;
  int h = bottom_top_blob->h;
  int channels = pBox_channels(bottom_top_blob);
  int size = w * h;

  img_t max;
  max.create(w, h);

  if (max.empty()) {
    return -100;
  }

  max.fill(-FLT_MAX);

  for (int q = 0; q < channels; q++) {
    float* ptr = pBox_channel_p(bottom_top_blob, q);
    float* maxptr = max;

    for (int i = 0; i < size; i++) {
      maxptr[i] = MAX(maxptr[i], ptr[i]);
    }
  }

  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    float* ptr = pBox_channel_p(bottom_top_blob, q);
    float* maxptr = max;

#if __ARM_NEON
    int nn = size >> 2;
    int remain = size - (nn << 2);
#else
    int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON

    for (; nn > 0; nn--) {
      float32x4_t _p = vld1q_f32(ptr);
      float32x4_t _max = vld1q_f32(maxptr);

      _p = exp_ps(vsubq_f32(_p, _max));

      vst1q_f32(ptr, _p);

      ptr += 4;
      maxptr += 4;
    }

#endif // __ARM_NEON

    for (; remain > 0; remain--) {
      *ptr = exp(*ptr - *maxptr);

      ptr++;
      maxptr++;
    }
  }

  img_t sum;
  sum.create(w, h);

  if (sum.empty()) {
    return -100;
  }

  sum.fill(0.f);

  for (int q = 0; q < channels; q++) {
    float* ptr = pBox_channel_p(bottom_top_blob, q);
    float* sumptr = sum;

#if __ARM_NEON
    int nn = size >> 2;
    int remain = size - (nn << 2);
#else
    int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON

    for (; nn > 0; nn--) {
      float32x4_t _p = vld1q_f32(ptr);
      float32x4_t _sum = vld1q_f32(sumptr);
      _sum = vaddq_f32(_sum, _p);
      vst1q_f32(sumptr, _sum);

      ptr += 4;
      sumptr += 4;
    }

#endif // __ARM_NEON

    for (; remain > 0; remain--) {
      *sumptr += *ptr;

      ptr++;
      sumptr++;
    }
  }

  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    float* ptr = pBox_channel_p(bottom_top_blob, q);
    float* sumptr = sum;

#if __ARM_NEON
    int nn = size >> 2;
    int remain = size - (nn << 2);
#else
    int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON

    for (; nn > 0; nn--) {
      float32x4_t _p = vld1q_f32(ptr);
      float32x4_t _sum = vld1q_f32(sumptr);
#if __aarch64__
      _p = vdivq_f32(_p, _sum);
#else
      _p = div_ps(_p, _sum);
#endif // __aarch64__
      vst1q_f32(ptr, _p);

      ptr += 4;
      sumptr += 4;
    }

#endif // __ARM_NEON

    for (; remain > 0; remain--) {
      *ptr /= *sumptr;

      ptr++;
      sumptr++;
    }
  }

  return 0;
}

} // namespace ncnn
