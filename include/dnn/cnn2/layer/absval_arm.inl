

#include "absval_arm.h"

#if __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

namespace ncnn
{

DEFINE_LAYER_CREATOR(AbsVal_arm)

int AbsVal_arm::forward(const Blob* bottom_blob, Blob* top_blob) const
{
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = Blob_channels(bottom_blob);
  int size = w * h;

  top_blob->create(w, h, channels);

  if (top_blob->empty()) {
    return -100;
  }

  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    const float* ptr = Blob_channel_p(bottom_blob, q);
    float* outptr = Blob_channel_p(top_blob, q);

#if __ARM_NEON
    int nn = size >> 2;
    int remain = size - (nn << 2);
#else
    int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON
#if __aarch64__

    for (; nn > 0; nn--) {
      float32x4_t _p = vld1q_f32(ptr);
      float32x4_t _outp = vabsq_f32(_p);
      vst1q_f32(outptr, _outp);

      ptr += 4;
      outptr += 4;
    }

#else

    if (nn > 0) {
      asm volatile(
          "0:                             \n"
          "vld1.f32   {d0-d1}, [%1]!      \n"
          "vabs.f32   q0, q0              \n"
          "subs       %0, #1              \n"
          "vst1.f32   {d0-d1}, [%2]!      \n"
          "bne        0b                  \n"
          : "=r"(nn),     // %0
          "=r"(ptr),    // %1
          "=r"(outptr)  // %2
          : "0"(nn),
          "1"(ptr),
          "2"(outptr)
          : "cc", "memory", "q0"
      );
    }

#endif // __aarch64__
#endif // __ARM_NEON

    for (; remain > 0; remain--) {
      *outptr = *ptr > 0 ? *ptr : -*ptr;

      ptr++;
      outptr++;
    }
  }

  return 0;
}

int AbsVal_arm::forward_inplace(Blob* bottom_top_blob) const
{
  int w = bottom_top_blob->w;
  int h = bottom_top_blob->h;
  int channels = Blob_channels(bottom_top_blob);
  int size = w * h;

  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    float* ptr = Blob_channel_p(bottom_top_blob, q);

#if __ARM_NEON
    int nn = size >> 2;
    int remain = size - (nn << 2);
#else
    int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON
#if __aarch64__

    for (; nn > 0; nn--) {
      float32x4_t _p = vld1q_f32(ptr);
      _p = vabsq_f32(_p);
      vst1q_f32(ptr, _p);

      ptr += 4;
    }

#else

    if (nn > 0) {
      asm volatile(
          "0:                             \n"
          "vld1.f32   {d0-d1}, [%1]       \n"
          "vabs.f32   q0, q0              \n"
          "subs       %0, #1              \n"
          "vst1.f32   {d0-d1}, [%1]!      \n"
          "bne        0b                  \n"
          : "=r"(nn),     // %0
          "=r"(ptr)     // %1
          : "0"(nn),
          "1"(ptr)
          : "cc", "memory", "q0"
      );
    }

#endif // __aarch64__
#endif // __ARM_NEON

    for (; remain > 0; remain--) {
      *ptr = *ptr > 0 ? *ptr : -*ptr;

      ptr++;
    }
  }

  return 0;
}

} // namespace ncnn
