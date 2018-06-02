

int BatchNorm_forward(const Blob* prev, Blob* next) const
{
  // a = bias - slope * mean / sqrt(var)
  // b = slope / sqrt(var)
  // value = b * value + a

  int w = prevSize.w;
  int h = prevSize.h;
  int size = w * h;

  next->create(w, h, channels);

  if (next->empty()) {
    return -100;
  }

  const float* a_data_ptr = a_data;
  const float* b_data_ptr = b_data;
  #pragma omp parallel for

  for (q = 0; q < channels; q++) {
    const float* ptr = prevData + q * Data_2DSize(prevSize);
    float* outptr = nextData + q * Data_2DSize(nextSize);

    float a = a_data_ptr[q];
    float b = b_data_ptr[q];

#if __ARM_NEON
    int nn = size >> 2;
    int remain = size - (nn << 2);
#else
    int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON
#if __aarch64__
    float32x4_t _a = vdupq_n_f32(a);
    float32x4_t _b = vdupq_n_f32(b);

    for (; nn > 0; nn--) {
      float32x4_t _p = vld1q_f32(ptr);
      float32x4_t _outp = _a;
      _outp = vfmaq_f32(_outp, _p, _b);
      vst1q_f32(outptr, _outp);

      ptr += 4;
      outptr += 4;
    }

#else

    if (nn > 0) {
      asm volatile(
          "vdup.f32   q1, %6              \n"
          "vdup.f32   q2, %7              \n"
          "0:                             \n"
          "pld        [%1, #128]          \n"
          "vld1.f32   {d0-d1}, [%1 :128]! \n"
          "vorr.32    q3, q1, q1          \n"
          "vmla.f32   q3, q0, q2          \n"
          "subs       %0, #1              \n"
          "vst1.f32   {d6-d7}, [%2 :128]! \n"
          "bne        0b                  \n"
          : "=r"(nn),     // %0
          "=r"(ptr),    // %1
          "=r"(outptr)  // %2
          : "0"(nn),
          "1"(ptr),
          "2"(outptr),
          "r"(a),       // %6
          "r"(b)        // %7
          : "cc", "memory", "q0", "q1", "q2", "q3"
      );
    }

#endif // __aarch64__
#endif // __ARM_NEON

    for (; remain > 0; remain--) {
      *outptr = b * *ptr + a;

      ptr++;
      outptr++;
    }
  }

  return 0;
}

int BatchNorm_forward_inplace(Blob* bottom_top_blob) const
{
  // a = bias - slope * mean / sqrt(var)
  // b = slope / sqrt(var)
  // value = b * value + a

  int w = bottom_top_blob->w;
  int h = bottom_top_blob->h;
  int size = w * h;

  const float* a_data_ptr = a_data;
  const float* b_data_ptr = b_data;
  #pragma omp parallel for

  for (q = 0; q < channels; q++) {
    float* ptr = Blob_channel_p(bottom_top_blob, q);

    float a = a_data_ptr[q];
    float b = b_data_ptr[q];

#if __ARM_NEON
    int nn = size >> 2;
    int remain = size - (nn << 2);
#else
    int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON
#if __aarch64__
    float32x4_t _a = vdupq_n_f32(a);
    float32x4_t _b = vdupq_n_f32(b);

    for (; nn > 0; nn--) {
      float32x4_t _p = vld1q_f32(ptr);
      float32x4_t _outp = _a;
      _outp = vfmaq_f32(_outp, _p, _b);
      vst1q_f32(ptr, _outp);

      ptr += 4;
    }

#else

    if (nn > 0) {
      asm volatile(
          "vdup.f32   q1, %4              \n"
          "vdup.f32   q2, %5              \n"
          "0:                             \n"
          "pld        [%1, #128]          \n"
          "vld1.f32   {d0-d1}, [%1 :128]  \n"
          "vorr.32    q3, q1, q1          \n"
          "vmla.f32   q3, q0, q2          \n"
          "subs       %0, #1              \n"
          "vst1.f32   {d6-d7}, [%1 :128]! \n"
          "bne        0b                  \n"
          : "=r"(nn),     // %0
          "=r"(ptr)     // %1
          : "0"(nn),
          "1"(ptr),
          "r"(a),       // %4
          "r"(b)        // %5
          : "cc", "memory", "q0", "q1", "q2", "q3"
      );
    }

#endif // __aarch64__
#endif // __ARM_NEON

    for (; remain > 0; remain--) {
      *ptr = b * *ptr + a;

      ptr++;
    }
  }

  return 0;
}

