

#include "sigmoid_arm.h"

#if __ARM_NEON
#include <arm_neon.h>
#include "neon_mathfun.h"
#endif // __ARM_NEON



namespace ncnn
{

DEFINE_LAYER_CREATOR(Sigmoid_arm)

int Sigmoid_arm::forward(const img_t* bottom_blob, img_t* top_blob) const
{
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = pBox_channels(bottom_blob);
  int size = w * h;

  top_blob->create(w, h, channels);

  if (top_blob->empty()) {
    return -100;
  }

  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    const float* ptr = pBox_channel_p(bottom_blob, q);
    float* outptr = pBox_channel_p(top_blob, q);

#if __ARM_NEON
    int nn = size >> 2;
    int remain = size - (nn << 2);
#else
    int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON
    float32x4_t _one = vdupq_n_f32(1.f);

    for (; nn > 0; nn--) {
      float32x4_t _p = vld1q_f32(ptr);
      _p = vnegq_f32(_p);
      _p = exp_ps(_p);
      _p = vaddq_f32(_p, _one);
      float32x4_t _outp = vrecpeq_f32(_p);
      _outp = vmulq_f32(vrecpsq_f32(_p, _outp), _outp);
      //             _outp = vmulq_f32(vrecpsq_f32(_p, _outp), _outp);
      vst1q_f32(outptr, _outp);

      ptr += 4;
      outptr += 4;
    }

#endif // __ARM_NEON

    for (; remain > 0; remain--) {
      *outptr = 1.f / (1.f + exp(-*ptr));

      ptr++;
      outptr++;
    }
  }

  return 0;
}

int Sigmoid_arm::forward_inplace(img_t* bottom_top_blob) const
{
  int w = bottom_top_blob->w;
  int h = bottom_top_blob->h;
  int channels = pBox_channels(bottom_top_blob);
  int size = w * h;

  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    float* ptr = pBox_channel_p(bottom_top_blob, q);

#if __ARM_NEON
    int nn = size >> 2;
    int remain = size - (nn << 2);
#else
    int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON
    float32x4_t _one = vdupq_n_f32(1.f);

    for (; nn > 0; nn--) {
      float32x4_t _p = vld1q_f32(ptr);
      _p = vnegq_f32(_p);
      _p = exp_ps(_p);
      _p = vaddq_f32(_p, _one);
      float32x4_t _outp = vrecpeq_f32(_p);
      _outp = vmulq_f32(vrecpsq_f32(_p, _outp), _outp);
      //             _outp = vmulq_f32(vrecpsq_f32(_p, _outp), _outp);
      vst1q_f32(ptr, _outp);

      ptr += 4;
    }

#endif // __ARM_NEON

    for (; remain > 0; remain--) {
      *ptr = 1.f / (1.f + exp(-*ptr));

      ptr++;
    }
  }

  return 0;
}

} // namespace ncnn
