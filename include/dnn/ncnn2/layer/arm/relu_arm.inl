

#include "relu_arm.h"

#if __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

namespace ncnn
{

DEFINE_LAYER_CREATOR(ReLU_arm)

int ReLU_arm::forward(const img_t* bottom_blob, img_t* top_blob) const
{
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = pBox_channels(bottom_blob);
  int size = w * h;

  top_blob->create(w, h, channels);

  if (top_blob->empty()) {
    return -100;
  }

  if (slope == 0.f) {
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
#if __aarch64__
      float32x4_t _zero = vdupq_n_f32(0.f);

      for (; nn > 0; nn--) {
        float32x4_t _p = vld1q_f32(ptr);
        float32x4_t _outp = vmaxq_f32(_p, _zero);
        vst1q_f32(outptr, _outp);

        ptr += 4;
        outptr += 4;
      }

#else

      if (nn > 0) {
        asm volatile(
            "veor       q1, q0, q0          \n"
            "0:                             \n"
            "pld        [%1, #128]          \n"
            "vld1.f32   {d0-d1}, [%1 :128]! \n"
            "vmax.f32   q0, q0, q1          \n"
            "subs       %0, #1              \n"
            "vst1.f32   {d0-d1}, [%2 :128]! \n"
            "bne        0b                  \n"
            : "=r"(nn),     // %0
            "=r"(ptr),    // %1
            "=r"(outptr)  // %2
            : "0"(nn),
            "1"(ptr),
            "2"(outptr)
            : "cc", "memory", "q0", "q1"
        );
      }

#endif // __aarch64__
#endif // __ARM_NEON

      for (; remain > 0; remain--) {
        *outptr = MAX(*ptr, 0.f);

        ptr++;
        outptr++;
      }
    }
  }
  else {
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
#if __aarch64__
      float32x4_t _zero = vdupq_n_f32(0.f);
      float32x4_t _slope = vdupq_n_f32(slope);

      for (; nn > 0; nn--) {
        float32x4_t _p = vld1q_f32(ptr);
        uint32x4_t _lemask = vcleq_f32(_p, _zero);
        float32x4_t _ps = vmulq_f32(_p, _slope);
        float32x4_t _outp = vbslq_f32(_lemask, _ps, _p);
        vst1q_f32(outptr, _outp);

        ptr += 4;
        outptr += 4;
      }

#else

      if (nn > 0) {
        asm volatile(
            "veor       q1, q0, q0          \n"
            "vdup.f32   q2, %6              \n"
            "0:                             \n"
            "pld        [%1, #128]          \n"
            "vld1.f32   {d0-d1}, [%1 :128]  \n"
            "vcle.f32   q3, q0, q1          \n"
            "vmul.f32   q4, q0, q2          \n"
            "vbit.32    q0, q4, q3          \n"
            "subs       %0, #1              \n"
            "vst1.f32   {d0-d1}, [%2 :128]! \n"
            "bne        0b                  \n"
            : "=r"(nn),     // %0
            "=r"(ptr),    // %1
            "=r"(outptr)  // %2
            : "0"(nn),
            "1"(ptr),
            "2"(outptr),
            "r"(slope)    // %6
            : "cc", "memory", "q0", "q1", "q2", "q3", "q4"
        );
      }

#endif // __aarch64__
#endif // __ARM_NEON

      for (; remain > 0; remain--) {
        if (*ptr < 0) {
          *outptr = *ptr * slope;
        }
        else {
          *outptr = *ptr;
        }

        ptr++;
        outptr++;
      }
    }
  }

  return 0;
}

int ReLU_arm::forward_inplace(img_t* bottom_top_blob) const
{
  int w = bottom_top_blob->w;
  int h = bottom_top_blob->h;
  int channels = pBox_channels(bottom_top_blob);
  int size = w * h;

  if (slope == 0.f) {
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
#if __aarch64__
      float32x4_t _zero = vdupq_n_f32(0.f);

      for (; nn > 0; nn--) {
        float32x4_t _p = vld1q_f32(ptr);
        _p = vmaxq_f32(_p, _zero);
        vst1q_f32(ptr, _p);

        ptr += 4;
      }

#else

      if (nn > 0) {
        asm volatile(
            "veor       q1, q0, q0          \n"
            "0:                             \n"
            "pld        [%1, #128]          \n"
            "vld1.f32   {d0-d1}, [%1 :128]  \n"
            "vmax.f32   q0, q0, q1          \n"
            "subs       %0, #1              \n"
            "vst1.f32   {d0-d1}, [%1 :128]! \n"
            "bne        0b                  \n"
            : "=r"(nn),     // %0
            "=r"(ptr)     // %1
            : "0"(nn),
            "1"(ptr)
            : "cc", "memory", "q0", "q1"
        );
      }

#endif // __aarch64__
#endif // __ARM_NEON

      for (; remain > 0; remain--) {
        *ptr = MAX(*ptr, 0.f);

        ptr++;
      }
    }
  }
  else {
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
#if __aarch64__
      float32x4_t _zero = vdupq_n_f32(0.f);
      float32x4_t _slope = vdupq_n_f32(slope);

      for (; nn > 0; nn--) {
        float32x4_t _p = vld1q_f32(ptr);
        uint32x4_t _lemask = vcleq_f32(_p, _zero);
        float32x4_t _ps = vmulq_f32(_p, _slope);
        _p = vbslq_f32(_lemask, _ps, _p);
        vst1q_f32(ptr, _p);

        ptr += 4;
      }

#else

      if (nn > 0) {
        asm volatile(
            "veor       q1, q0, q0          \n"
            "vdup.f32   q2, %4              \n"
            "0:                             \n"
            "pld        [%1, #128]          \n"
            "vld1.f32   {d0-d1}, [%1 :128]  \n"
            "vcle.f32   q3, q0, q1          \n"
            "vmul.f32   q4, q0, q2          \n"
            "vbit.32    q0, q4, q3          \n"
            "subs       %0, #1              \n"
            "vst1.f32   {d0-d1}, [%1 :128]! \n"
            "bne        0b                  \n"
            : "=r"(nn),     // %0
            "=r"(ptr)     // %1
            : "0"(nn),
            "1"(ptr),
            "r"(slope)    // %4
            : "cc", "memory", "q0", "q1", "q2", "q3", "q4"
        );
      }

#endif // __aarch64__
#endif // __ARM_NEON

      for (; remain > 0; remain--) {
        if (*ptr < 0) {
          *ptr *= slope;
        }

        ptr++;
      }
    }
  }

  return 0;
}

} // namespace ncnn
