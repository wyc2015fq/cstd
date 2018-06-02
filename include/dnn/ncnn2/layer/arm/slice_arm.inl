

#include "slice_arm.h"
#if __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

namespace ncnn
{

DEFINE_LAYER_CREATOR(Slice_arm)

int Slice_arm::forward(const std::vector<img_t>& bottom_blobs, std::vector<img_t>& top_blobs) const
{
  const img_t* bottom_blob = bottom_blobs[0];
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = pBox_channels(bottom_blob);

  int q = 0;
  const int* slices_ptr = (const int*)slices.data;

  for (size_t i = 0; i < top_blobs.size(); i++) {
    int slice = slices_ptr[i];

    if (slice == -233) {
      slice = (channels - q) / (top_blobs.size() - i);
    }

    img_t* top_blob = top_blobs[i];
    top_blob->create(w, h, slice);

    if (top_blob->empty()) {
      return -100;
    }

    int size = bottom_blob->cstep * slice;

    const float* ptr = pBox_channel_p(bottom_blob, q);
    float* outptr = top_blob->data;

#if __ARM_NEON
    int nn = size >> 3;
    int remain = size - (nn << 3);
#else
    int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON
#if __aarch64__

    for (; nn > 0; nn--) {
      float32x4_t _p = vld1q_f32(ptr);
      float32x4_t _p2 = vld1q_f32(ptr + 4);
      vst1q_f32(outptr, _p);
      vst1q_f32(outptr + 4, _p2);

      ptr += 8;
      outptr += 8;
    }

#else

    if (nn > 0) {
      asm volatile(
          "0:                             \n"
          "pld        [%1, #256]          \n"
          "vld1.f32   {d0-d3}, [%1 :128]! \n"
          "subs       %0, #1              \n"
          "vst1.f32   {d0-d3}, [%2 :128]! \n"
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
      *outptr++ = *ptr++;
    }

    q += slice;
  }

  return 0;
}

} // namespace ncnn
