

int InnerProduct_arm_forward(const img_t* bottom_blob, img_t* top_blob, int num_output, const float* weight_data, const float* bias_data)
{
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = pBox_channels(bottom_blob);
  int size = w * h;

  top_blob->create(1, 1, num_output);

  if (top_blob->empty()) {
    return -100;
  }

  // num_output
  const float* weight_data_ptr = weight_data;

  #pragma omp parallel for

  for (int p = 0; p < num_output; p++) {
    float* outptr = pBox_channel(top_blob, p);
    float sum = 0.f;

    if (bias_term) {
      sum = bias_data.data[p];
    }

    const float* w = weight_data_ptr + size * channels * p;
    const float* w2 = w + size;

#if __ARM_NEON
    float32x4_t _sum = vdupq_n_f32(0.f);
    float32x4_t _sum2 = vdupq_n_f32(0.f);
#endif // __ARM_NEON

    // channels
    for (int q = 0; q < channels; q++) {
      const float* m = pBox_channel_p(bottom_blob, q);

#if __ARM_NEON
      int nn = size >> 3;
      int remain = size & 7;
#if __aarch64__

      for (; nn > 0; nn--) {
        float32x4_t _m = vld1q_f32(m);
        float32x4_t _w = vld1q_f32(w);
        _sum = vfmaq_f32(_sum, _m, _w);

        _m = vld1q_f32(m + 4);
        _w = vld1q_f32(w + 4);
        _sum2 = vfmaq_f32(_sum2, _m, _w);

        m += 8;
        w += 8;
      }

#else

      if (nn > 0) {
        asm volatile(
            "0:                             \n"
            "pld        [%1, #256]          \n"
            "vld1.f32   {d0-d3}, [%1 :128]! \n"
            "pld        [%2, #256]          \n"
            "vld1.f32   {d4-d7}, [%2]!      \n"
            "vmla.f32   %q3, q0, q2         \n"
            "subs       %0, #1              \n"
            "vmla.f32   %q4, q1, q3         \n"
            "bne        0b                  \n"
            : "=r"(nn),     // %0
            "=r"(m),      // %1
            "=r"(w),      // %2
            "=w"(_sum),   // %3
            "=w"(_sum2)   // %4
            : "0"(nn),
            "1"(m),
            "2"(w),
            "3"(_sum),
            "4"(_sum2)
            : "cc", "memory", "q0", "q1", "q2", "q3"
        );
      }

#endif // __aarch64__
#endif // __ARM_NEON
      int remain = size;

      for (; remain > 0; remain--) {
        sum += *m * *w;

        m++;
        w++;
      }
    }

#if __ARM_NEON
    _sum = vaddq_f32(_sum, _sum2);
#if __aarch64__
    sum += vaddvq_f32(_sum);
#else
    float32x2_t _sumss = vadd_f32(vget_low_f32(_sum), vget_high_f32(_sum));
    _sumss = vpadd_f32(_sumss, _sumss);
    sum += vget_lane_f32(_sumss, 0);
#endif // __aarch64__
#endif // __ARM_NEON

    outptr[0] = sum;
  }

  return 0;
}

} // namespace ncnn
