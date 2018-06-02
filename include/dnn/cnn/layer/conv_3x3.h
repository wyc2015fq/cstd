
CC_INLINE void sfill(int n, float* data, float x)
{
  int i = 0;
  if (0==x) {
    memset(data, 0, n*sizeof(*data));
    return ;
  }

  for (; i < n-3; i += 4) {
    data[i] = x;
    data[i + 1] = x;
    data[i + 2] = x;
    data[i + 3] = x;
  }

  for (; i < n; i++) {
    data[i] = x;
  }
}

static void sconv3x3s1(int h, int w, const float* src, int src_step, int srccn, float* dst, int dst_step, int dstcn, const float* kernel, const float* bias)
{
  int p, q;
  int outh = h - 2;
  int outw = w - 2;

#pragma omp parallel for
  for (p = 0; p < dstcn; p++) {
    float* outptr = dst + p * outh*dst_step;
    const float* kernel0 = kernel + p * srccn * 9;
    const float bias0 = bias ? bias[p] : 0.f;
    //sfill(outh*outw, outptr, bias0);
    blas_sset(outh*outw, bias0, outptr);

    for (q = 0; q < srccn; q++) {
      const float* img0 = src + q*h*src_step;
      const float* k0 = kernel0;
      const float* k1 = kernel0 + 3;
      const float* k2 = kernel0 + 6;

#if __ARM_NEON
      float32x4_t _k0123 = vld1q_f32(kernel0);
      float32x4_t _k3456 = vld1q_f32(kernel0 + 3);
      float32x4_t _k6789 = vld1q_f32(kernel0 + 6);
#endif // __ARM_NEON

#if CC_SSE2
      __m128 _k00 = _mm_set_ps1(k0[0]);
      __m128 _k01 = _mm_set_ps1(k0[1]);
      __m128 _k02 = _mm_set_ps1(k0[2]);
      __m128 _k10 = _mm_set_ps1(k1[0]);
      __m128 _k11 = _mm_set_ps1(k1[1]);
      __m128 _k12 = _mm_set_ps1(k1[2]);
      __m128 _k20 = _mm_set_ps1(k2[0]);
      __m128 _k21 = _mm_set_ps1(k2[1]);
      __m128 _k22 = _mm_set_ps1(k2[2]);
#endif // CC_SSE2
      int i = 0;
      int src_w = w;

#if 0
      for (; i < outh-3; i += 4) {
        int remain = outw;
        const float* r0 = img0 + i * src_w;
        const float* r1 = r0 + src_w;
        const float* r2 = r0 + src_w * 2;
        const float* r3 = r0 + src_w * 3;
        const float* r4 = r0 + src_w * 4;
        const float* r5 = r0 + src_w * 5;
        float* outptr0 = outptr + i * outw;
        float* outptr1 = outptr0 + outw;
        float* outptr2 = outptr0 + outw * 2;
        float* outptr3 = outptr0 + outw * 3;
#if __ARM_NEON
        int nn = remain >> 2;
        remain = remain & 3;
        
        if (nn > 0) {
          float* dst = outptr0;
          const float* src = r0;
          int outw1 = outw * sizeof(float);
          int src_w1 = src_w * sizeof(float);
        }

        if (nn > 0) {
          asm volatile(
              "ld1  {v0.4s}, [%[k0]]              \n"
              "ld1  {v1.4s}, [%[k1]]              \n"
              "ld1  {v2.4s}, [%[k2]]              \n"

              "0:                                 \n"
              "PRFM PLDL1KEEP, [%1, #128]         \n"
              "ld1  {v3.4s}, [%[outptr0]]         \n"

              "PRFM PLDL1KEEP, [%3, #196]         \n"
              "ld1  {v6.4s}, [%[r0]]              \n"
              "add  %[r0], %[r0], #16             \n"
              "ld1  {v9.4s}, [%[r0]]              \n"
              "ext  v7.16b, v6.16b, v9.16b, #4    \n"
              "ext  v8.16b, v6.16b, v9.16b, #8    \n"

              "fmla v3.4s, v6.4s, v0.4s[0]        \n"
              "fmla v3.4s, v7.4s, v0.4s[1]        \n"
              "fmla v3.4s, v8.4s, v0.4s[2]        \n"

              "PRFM PLDL1KEEP, [%[outptr1], #128] \n"
              "ld1  {v4.4s}, [%[outptr1]]         \n"

              "PRFM PLDL1KEEP, [%4, #196]         \n"
              "ld1  {v6.4s}, [%[r1]]              \n"
              "add  %[r1], %[r1], #16             \n"
              "ld1  {v9.4s}, [%[r1]]              \n"
              "ext  v7.16b, v6.16b, v9.16b, #4    \n"
              "ext  v8.16b, v6.16b, v9.16b, #8    \n"
#if 0
              "fmla v3.4s, v6.4s, v1.4s[0]        \n"
              "fmla v3.4s, v7.4s, v1.4s[1]        \n"
              "fmla v3.4s, v8.4s, v1.4s[2]        \n"

              "fmla v4.4s, v6.4s, v0.4s[0]        \n"
              "fmla v4.4s, v7.4s, v0.4s[1]        \n"
              "fmla v4.4s, v8.4s, v0.4s[2]        \n"
#else
              "fmla v3.4s, v6.4s, v1.4s[0]        \n"
              "fmla v4.4s, v6.4s, v0.4s[0]        \n"

              "fmla v3.4s, v7.4s, v1.4s[1]        \n"
              "fmla v4.4s, v7.4s, v0.4s[1]        \n"

              "fmla v3.4s, v8.4s, v1.4s[2]        \n"
              "fmla v4.4s, v8.4s, v0.4s[2]        \n"
#endif

              "ld1  {v5.4s}, [%[outptr2]]         \n"

              "PRFM PLDL1KEEP, [%[r2], #196]      \n"
              "ld1  {v6.4s}, [%[r2]]              \n"
              "add  %[r2], %[r2], #16             \n"
              "ld1  {v9.4s}, [%[r2]]              \n"
              "ext  v7.16b, v6.16b, v9.16b, #4    \n"
              "ext  v8.16b, v6.16b, v9.16b, #8    \n"
#if 0
              "fmla v3.4s, v6.4s, v2.4s[0]        \n"
              "fmla v3.4s, v7.4s, v2.4s[1]        \n"
              "fmla v3.4s, v8.4s, v2.4s[2]        \n"

              "st1  {v3.4s}, [%[outptr0]]         \n"

              "fmla v4.4s, v6.4s, v1.4s[0]        \n"
              "fmla v4.4s, v7.4s, v1.4s[1]        \n"
              "fmla v4.4s, v8.4s, v1.4s[2]        \n"

              "fmla v5.4s, v6.4s, v0.4s[0]        \n"
              "fmla v5.4s, v7.4s, v0.4s[1]        \n"
              "fmla v5.4s, v8.4s, v0.4s[2]        \n"
#else
              "fmla v3.4s, v6.4s, v2.4s[0]        \n"
              "fmla v4.4s, v6.4s, v1.4s[0]        \n"
              "fmla v5.4s, v6.4s, v0.4s[0]        \n"

              "fmla v3.4s, v7.4s, v2.4s[1]        \n"
              "fmla v4.4s, v7.4s, v1.4s[1]        \n"
              "fmla v5.4s, v7.4s, v0.4s[1]        \n"

              "fmla v3.4s, v8.4s, v2.4s[2]        \n"
              "fmla v4.4s, v8.4s, v1.4s[2]        \n"
              "fmla v5.4s, v8.4s, v0.4s[2]        \n"
              "st1  {v3.4s}, [%[outptr0]]         \n"
#endif
              "ld1  {v3.4s}, [%[outptr3]]         \n"

              "PRFM PLDL1KEEP, [%[r3], #196]      \n"
              "ld1  {v6.4s}, [%[r3]]              \n"
              "add  %[r3], %[r3], #16             \n"
              "ld1  {v9.4s}, [%[r3]]              \n"
              "ext  v7.16b, v6.16b, v9.16b, #4    \n"
              "ext  v8.16b, v6.16b, v9.16b, #8    \n"
#if 0
              "fmla v4.4s, v6.4s, v2.4s[0]        \n"
              "fmla v4.4s, v7.4s, v2.4s[1]        \n"
              "fmla v4.4s, v8.4s, v2.4s[2]        \n"

              "st1  {v4.4s}, [%[outptr1]]         \n"

              "fmla v5.4s, v6.4s, v1.4s[0]        \n"
              "fmla v5.4s, v7.4s, v1.4s[1]        \n"
              "fmla v5.4s, v8.4s, v1.4s[2]        \n"
              
              "fmla v3.4s, v6.4s, v0.4s[0]        \n"
              "fmla v3.4s, v7.4s, v0.4s[1]        \n"
              "fmla v3.4s, v8.4s, v0.4s[2]        \n"
#else
              "fmla v4.4s, v6.4s, v2.4s[0]        \n"
              "fmla v5.4s, v6.4s, v1.4s[0]        \n"
              "fmla v3.4s, v6.4s, v0.4s[0]        \n"

              "fmla v4.4s, v7.4s, v2.4s[1]        \n"
              "fmla v5.4s, v7.4s, v1.4s[1]        \n"
              "fmla v3.4s, v7.4s, v0.4s[1]        \n"

              "fmla v4.4s, v8.4s, v2.4s[2]        \n"
              "fmla v5.4s, v8.4s, v1.4s[2]        \n"
              "fmla v3.4s, v8.4s, v0.4s[2]        \n"
              "st1  {v4.4s}, [%[outptr1]]         \n"
#endif
              "PRFM PLDL1KEEP, [%[r3], #196]      \n"
              "ld1  {v6.4s}, [%[r4]]              \n"
              "add  %[r4], %[r4], #16             \n"
              "ld1  {v9.4s}, [%[r4]]              \n"
              "ext  v7.16b, v6.16b, v9.16b, #4    \n"
              "ext  v8.16b, v6.16b, v9.16b, #8    \n"
#if 0
              "fmla v5.4s, v6.4s, v2.4s[0]        \n"
              "fmla v5.4s, v7.4s, v2.4s[1]        \n"
              "fmla v5.4s, v8.4s, v2.4s[2]        \n"

              "st1  {v5.4s}, [%[outptr2]]         \n"

              "fmla v3.4s, v6.4s, v1.4s[0]        \n"
              "fmla v3.4s, v7.4s, v1.4s[1]        \n"
              "fmla v3.4s, v8.4s, v1.4s[2]        \n"
#else
              "fmla v5.4s, v6.4s, v2.4s[0]        \n"
              "fmla v3.4s, v6.4s, v1.4s[0]        \n"

              "fmla v5.4s, v7.4s, v2.4s[1]        \n"
              "fmla v3.4s, v7.4s, v1.4s[1]        \n"

              "fmla v5.4s, v8.4s, v2.4s[2]        \n"
              "fmla v3.4s, v8.4s, v1.4s[2]        \n"
              "st1  {v5.4s}, [%[outptr2]]         \n"
#endif
              "PRFM PLDL1KEEP, [%[r3], #196]      \n"
              "ld1  {v6.4s}, [%[r5]]              \n"
              "add  %[r5], %[r5], #16             \n"
              "ld1  {v9.4s}, [%[r5]]              \n"
              "ext  v7.16b, v6.16b, v9.16b, #4    \n"
              "ext  v8.16b, v6.16b, v9.16b, #8    \n"
#if 1
              "fmla v3.4s, v6.4s, v2.4s[0]        \n"
              "fmla v3.4s, v7.4s, v2.4s[1]        \n"
              "fmla v3.4s, v8.4s, v2.4s[2]        \n"

              "st1  {v3.4s}, [%[outptr3]]         \n"

              "add  %[outptr0], %[outptr0], #16   \n"
              "add  %[outptr1], %[outptr1], #16   \n"
              "add  %[outptr2], %[outptr2], #16   \n"
              "add  %[outptr3], %[outptr3], #16   \n"
#else
              "fmla v3.4s, v6.4s, v2.4s[0]        \n"
              "fmla v3.4s, v7.4s, v2.4s[1]        \n"
              "fmla v3.4s, v8.4s, v2.4s[2]        \n"
              "st1  {v3.4s}, [%[outptr3]]         \n"

              "add  %[outptr0], %[outptr0], #16   \n"
              "add  %[outptr1], %[outptr1], #16   \n"
              "add  %[outptr2], %[outptr2], #16   \n"
              "add  %[outptr3], %[outptr3], #16   \n"

#endif
              "subs %[nn], %[nn], #1              \n"
              "bne  0b                            \n"
              : [nn] "+r"(nn),
              [outptr0]"+r"(outptr0),
              [outptr1]"+r"(outptr1),
              [outptr2]"+r"(outptr2),
              [outptr3]"+r"(outptr3),
              [r0]"+r"(r0),
              [r1]"+r"(r1),
              [r2]"+r"(r2),
              [r3]"+r"(r3),
              [r4]"+r"(r4),
              [r5]"+r"(r5)
              : [k0] "r" (k0), [k1] "r" (k1), [k2] "r" (k2)
            : "cc", "memory", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", "v10", "v11", "v12", "v13"
            );
        }

#endif
#if CC_SSE2

        for (; remain > 3; remain -= 4) {
          __m128 _sum0 = _mm_loadu_ps(outptr0);
          __m128 _sum1 = _mm_loadu_ps(outptr1);
          __m128 _sum2 = _mm_loadu_ps(outptr2);
          __m128 _sum3 = _mm_loadu_ps(outptr3);

          __m128 _r0, _r1, _r2;
          _r0 = _mm_loadu_ps(r0);
          _r1 = _mm_loadu_ps(r0 + 1);
          _r2 = _mm_loadu_ps(r0 + 2);

          _mm_muladd_ps(_sum0, _r0, _k00);
          _mm_muladd_ps(_sum0, _r1, _k01);
          _mm_muladd_ps(_sum0, _r2, _k02);

          _r0 = _mm_loadu_ps(r1);
          _r1 = _mm_loadu_ps(r1 + 1);
          _r2 = _mm_loadu_ps(r1 + 2);

          _mm_muladd_ps(_sum0, _r0, _k10);
          _mm_muladd_ps(_sum0, _r1, _k11);
          _mm_muladd_ps(_sum0, _r2, _k12);

          _mm_muladd_ps(_sum1, _r0, _k00);
          _mm_muladd_ps(_sum1, _r1, _k01);
          _mm_muladd_ps(_sum1, _r2, _k02);

          _r0 = _mm_loadu_ps(r2);
          _r1 = _mm_loadu_ps(r2 + 1);
          _r2 = _mm_loadu_ps(r2 + 2);

          _mm_muladd_ps(_sum0, _r0, _k20);
          _mm_muladd_ps(_sum0, _r1, _k21);
          _mm_muladd_ps(_sum0, _r2, _k22);

          _mm_muladd_ps(_sum1, _r0, _k10);
          _mm_muladd_ps(_sum1, _r1, _k11);
          _mm_muladd_ps(_sum1, _r2, _k12);

          _mm_muladd_ps(_sum2, _r0, _k00);
          _mm_muladd_ps(_sum2, _r1, _k01);
          _mm_muladd_ps(_sum2, _r2, _k02);

          _r0 = _mm_loadu_ps(r3);
          _r1 = _mm_loadu_ps(r3 + 1);
          _r2 = _mm_loadu_ps(r3 + 2);

          _mm_muladd_ps(_sum1, _r0, _k20);
          _mm_muladd_ps(_sum1, _r1, _k21);
          _mm_muladd_ps(_sum1, _r2, _k22);

          _mm_muladd_ps(_sum2, _r0, _k10);
          _mm_muladd_ps(_sum2, _r1, _k11);
          _mm_muladd_ps(_sum2, _r2, _k12);

          _mm_muladd_ps(_sum3, _r0, _k00);
          _mm_muladd_ps(_sum3, _r1, _k01);
          _mm_muladd_ps(_sum3, _r2, _k02);

          _r0 = _mm_loadu_ps(r4);
          _r1 = _mm_loadu_ps(r4 + 1);
          _r2 = _mm_loadu_ps(r4 + 2);

          _mm_muladd_ps(_sum2, _r0, _k20);
          _mm_muladd_ps(_sum2, _r1, _k21);
          _mm_muladd_ps(_sum2, _r2, _k22);

          _mm_muladd_ps(_sum3, _r0, _k10);
          _mm_muladd_ps(_sum3, _r1, _k11);
          _mm_muladd_ps(_sum3, _r2, _k12);

          _r0 = _mm_loadu_ps(r5);
          _r1 = _mm_loadu_ps(r5 + 1);
          _r2 = _mm_loadu_ps(r5 + 2);

          _mm_muladd_ps(_sum3, _r0, _k20);
          _mm_muladd_ps(_sum3, _r1, _k21);
          _mm_muladd_ps(_sum3, _r2, _k22);

          _mm_storeu_ps(outptr0, _sum0);
          _mm_storeu_ps(outptr1, _sum1);
          _mm_storeu_ps(outptr2, _sum2);
          _mm_storeu_ps(outptr3, _sum3);

          r0 += 4;
          r1 += 4;
          r2 += 4;
          r3 += 4;
          r4 += 4;
          r5 += 4;
          outptr0 += 4;
          outptr1 += 4;
          outptr2 += 4;
          outptr3 += 4;
        }

#endif
        for (; remain > 0; remain--) {
          *outptr0++ += r0[0] * k0[0] + r0[1] * k0[1] + r0[2] * k0[2] + r1[0] * k1[0] + r1[1] * k1[1] + r1[2] * k1[2] + r2[0] * k2[0] + r2[1] * k2[1] + r2[2] * k2[2];
          *outptr1++ += r1[0] * k0[0] + r1[1] * k0[1] + r1[2] * k0[2] + r2[0] * k1[0] + r2[1] * k1[1] + r2[2] * k1[2] + r3[0] * k2[0] + r3[1] * k2[1] + r3[2] * k2[2];
          *outptr2++ += r2[0] * k0[0] + r2[1] * k0[1] + r2[2] * k0[2] + r3[0] * k1[0] + r3[1] * k1[1] + r3[2] * k1[2] + r4[0] * k2[0] + r4[1] * k2[1] + r4[2] * k2[2];
          *outptr3++ += r3[0] * k0[0] + r3[1] * k0[1] + r3[2] * k0[2] + r4[0] * k1[0] + r4[1] * k1[1] + r4[2] * k1[2] + r5[0] * k2[0] + r5[1] * k2[1] + r5[2] * k2[2];
          r0++;
          r1++;
          r2++;
          r3++;
          r4++;
          r5++;
        }
      }
#endif
#if 1
      for (; i + 1 < outh; i += 2) {
        int remain = outw;
        const float* r0 = img0 + i * src_w;
        const float* r1 = r0 + src_w * 1;
        const float* r2 = r0 + src_w * 2;
        const float* r3 = r0 + src_w * 3;
        float* outptr0 = outptr + i * outw;
        float* outptr1 = outptr0 + outw;

#if CC_SSE2

        for (; remain > 3; remain -= 4) {
          __m128 _sum1 = _mm_loadu_ps(outptr0);
          __m128 _sum2 = _mm_setzero_ps();
          __m128 _sum3 = _mm_loadu_ps(outptr1);
          __m128 _sum4 = _mm_setzero_ps();

          __m128 _r00 = _mm_loadu_ps(r0);
          __m128 _r01 = _mm_loadu_ps(r0 + 1);
          __m128 _r02 = _mm_loadu_ps(r0 + 2);

          __m128 _r10 = _mm_loadu_ps(r1);
          __m128 _r11 = _mm_loadu_ps(r1 + 1);
          __m128 _r12 = _mm_loadu_ps(r1 + 2);

          __m128 _r20 = _mm_loadu_ps(r2);
          __m128 _r21 = _mm_loadu_ps(r2 + 1);
          __m128 _r22 = _mm_loadu_ps(r2 + 2);

          __m128 _r30 = _mm_loadu_ps(r3);
          __m128 _r31 = _mm_loadu_ps(r3 + 1);
          __m128 _r32 = _mm_loadu_ps(r3 + 2);

          _mm_muladd_ps(_sum1, _r00, _k00);
          _mm_muladd_ps(_sum2, _r01, _k01);
          _mm_muladd_ps(_sum3, _r10, _k00);
          _mm_muladd_ps(_sum4, _r11, _k01);

          _mm_muladd_ps(_sum1, _r02, _k02);
          _mm_muladd_ps(_sum2, _r10, _k10);
          _mm_muladd_ps(_sum3, _r12, _k02);
          _mm_muladd_ps(_sum4, _r20, _k10);

          _mm_muladd_ps(_sum1, _r11, _k11);
          _mm_muladd_ps(_sum2, _r12, _k12);
          _mm_muladd_ps(_sum3, _r21, _k11);
          _mm_muladd_ps(_sum4, _r22, _k12);

          _mm_muladd_ps(_sum1, _r20, _k20);
          _mm_muladd_ps(_sum2, _r21, _k21);
          _mm_muladd_ps(_sum3, _r30, _k20);
          _mm_muladd_ps(_sum4, _r31, _k21);

          _mm_muladd_ps(_sum1, _r22, _k22);
          _mm_muladd_ps(_sum3, _r32, _k22);

          _sum1 = _mm_add_ps(_sum1, _sum2);
          _sum3 = _mm_add_ps(_sum3, _sum4);

          _mm_storeu_ps(outptr0, _sum1);
          _mm_storeu_ps(outptr1, _sum3);

          r0 += 4;
          r1 += 4;
          r2 += 4;
          r3 += 4;
          outptr0 += 4;
          outptr1 += 4;
        }

#endif
#if __ARM_NEON
        int nn = remain >> 2;
        remain = remain & 3;
#if __aarch64__
#if 1
#if 0
        if (nn > 0) {
          asm volatile(
              "ld1  {v0.4s}, [%[k0]]              \n"
              "ld1  {v1.4s}, [%[k1]]              \n"
              "ld1  {v2.4s}, [%[k2]]              \n"

              "0:                                 \n"
              "PRFM PLDL1KEEP, [%[outptr0], #128] \n"
              "ld1  {v3.4s}, [%[outptr0]]         \n"

              "PRFM PLDL1KEEP, [%[r0], #196]      \n"
              "ld1  {v6.4s}, [%[r0]]              \n"
              "add  %[r0], %[r0], #16             \n"
              "ld1  {v9.4s}, [%[r0]]              \n"
              "ext  v7.16b, v6.16b, v9.16b, #4    \n"
              "ext  v8.16b, v6.16b, v9.16b, #8    \n"

              "fmla v3.4s, v6.4s, v0.4s[0]        \n"
              "fmla v3.4s, v7.4s, v0.4s[1]        \n"
              "fmla v3.4s, v8.4s, v0.4s[2]        \n"

              "PRFM PLDL1KEEP, [%[outptr1], #128] \n"
              "ld1  {v4.4s}, [%[outptr1]]         \n"

              "PRFM PLDL1KEEP, [%[r1], #196]      \n"
              "ld1  {v6.4s}, [%[r1]]              \n"
              "add  %[r1], %[r1], #16             \n"
              "ld1  {v9.4s}, [%[r1]]              \n"
              "ext  v7.16b, v6.16b, v9.16b, #4    \n"
              "ext  v8.16b, v6.16b, v9.16b, #8    \n"

              "fmla v3.4s, v6.4s, v1.4s[0]        \n"
              "fmla v3.4s, v7.4s, v1.4s[1]        \n"
              "fmla v3.4s, v8.4s, v1.4s[2]        \n"

              "fmla v4.4s, v6.4s, v0.4s[0]        \n"
              "fmla v4.4s, v7.4s, v0.4s[1]        \n"
              "fmla v4.4s, v8.4s, v0.4s[2]        \n"

              "PRFM PLDL1KEEP, [%[r2], #196]      \n"
              "ld1  {v6.4s}, [%[r2]]              \n"
              "add  %[r2], %[r2], #16             \n"
              "ld1  {v9.4s}, [%[r2]]              \n"
              "ext  v7.16b, v6.16b, v9.16b, #4    \n"
              "ext  v8.16b, v6.16b, v9.16b, #8    \n"

              "fmla v3.4s, v6.4s, v2.4s[0]        \n"
              "fmla v3.4s, v7.4s, v2.4s[1]        \n"
              "fmla v3.4s, v8.4s, v2.4s[2]        \n"

              "fmla v4.4s, v6.4s, v1.4s[0]        \n"
              "fmla v4.4s, v7.4s, v1.4s[1]        \n"
              "fmla v4.4s, v8.4s, v1.4s[2]        \n"

              "PRFM PLDL1KEEP, [%[r3], #196]      \n"
              "ld1  {v6.4s}, [%[r3]]              \n"
              "add  %[r3], %[r3], #16             \n"
              "ld1  {v9.4s}, [%[r3]]              \n"
              "ext  v7.16b, v6.16b, v9.16b, #4    \n"
              "ext  v8.16b, v6.16b, v9.16b, #8    \n"

              "fmla v4.4s, v6.4s, v2.4s[0]        \n"
              "fmla v4.4s, v7.4s, v2.4s[1]        \n"
              "fmla v4.4s, v8.4s, v2.4s[2]        \n"

              "st1  {v3.4s}, [%[outptr0]]         \n"
              "st1  {v4.4s}, [%[outptr1]]         \n"

              "add  %[outptr0], %[outptr0], #16   \n"
              "add  %[outptr1], %[outptr1], #16   \n"

              "subs %[nn], %[nn], #1              \n"
              "bne  0b                            \n"
              : [nn] "+r"(nn),
              [outptr0]"+r"(outptr0),
              [outptr1]"+r"(outptr1),
              [r0]"+r"(r0),
              [r1]"+r"(r1),
              [r2]"+r"(r2),
              [r3]"+r"(r3)
              : [k0] "r" (k0), [k1] "r" (k1), [k2] "r" (k2)
            : "cc", "memory", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9"
            );
        }
#endif

        if (nn > 0) {
          asm volatile(
              "ld1  {v5.4s}, [%[k0]]               \n"
              "ld1  {v6.4s}, [%[k1]]               \n"
              "ld1  {v7.4s}, [%[k2]]               \n"

              "0:                                  \n"
              "PRFM PLDL1KEEP, [%[outptr0], #128]  \n"
              "ld1  {v1.4s}, [%[outptr0]]          \n"
              "eor  v2.16b, v2.16b, v2.16b         \n"
              "eor  v12.16b, v12.16b, v12.16b      \n"

              "PRFM PLDL1KEEP, [%[r0], #196]       \n"
              "ld1  {v8.4s}, [%[r0]]               \n"
              "fmla v1.4s, v8.4s, v5.4s[0]         \n"
              "add  %[r0], %[r0], #16              \n"
              "ld1  {v9.4s}, [%[r0]]               \n"
              "ext  v10.16b, v8.16b, v9.16b, #4    \n"
              "fmla v2.4s, v10.4s, v5.4s[1]        \n"
              "ext  v11.16b, v8.16b, v9.16b, #8    \n"

              "fmla v12.4s, v11.4s, v5.4s[2]       \n"

              "PRFM PLDL1KEEP, [%[outptr1], #128]  \n"
              "ld1  {v3.4s}, [%[outptr1]]          \n"
              "eor  v4.16b, v4.16b, v4.16b         \n"
              "eor  v13.16b, v13.16b, v13.16b      \n"

              "PRFM PLDL1KEEP, [%[r1], #196]       \n"
              "ld1  {v8.4s}, [%[r1]]               \n"
              "fmla v1.4s, v8.4s, v6.4s[0]         \n"
              "fmla v3.4s, v8.4s, v5.4s[0]         \n"
              "add  %[r1], %[r1], #16              \n"
              "ld1  {v9.4s}, [%[r1]]               \n"
              "ext  v10.16b, v8.16b, v9.16b, #4    \n"
              "fmla v2.4s, v10.4s, v6.4s[1]        \n"
              "fmla v4.4s, v10.4s, v5.4s[1]        \n"
              "ext  v11.16b, v8.16b, v9.16b, #8    \n"

              "fmla v12.4s, v11.4s, v6.4s[2]       \n"

              "fmla v13.4s, v11.4s, v5.4s[2]       \n"

              "PRFM PLDL1KEEP, [%[r2], #196]       \n"
              "ld1  {v8.4s}, [%[r2]]               \n"
              "fmla v1.4s, v8.4s, v7.4s[0]         \n"
              "fmla v3.4s, v8.4s, v6.4s[0]         \n"
              "add  %[r2], %[r2], #16              \n"
              "ld1  {v9.4s}, [%[r2]]               \n"
              "ext  v10.16b, v8.16b, v9.16b, #4    \n"
              "fmla v2.4s, v10.4s, v7.4s[1]        \n"
              "fmla v4.4s, v10.4s, v6.4s[1]        \n"
              "ext  v11.16b, v8.16b, v9.16b, #8    \n"

              "fmla v12.4s, v11.4s, v7.4s[2]       \n"
              "fmla v13.4s, v11.4s, v6.4s[2]       \n"

              "PRFM PLDL1KEEP, [%[r3], #196]       \n"
              "ld1  {v8.4s}, [%[r3]]               \n"
              "fmla v3.4s, v8.4s, v7.4s[0]         \n"
              "add  %[r3], %[r3], #16              \n"
              "ld1  {v9.4s}, [%[r3]]               \n"
              "ext  v10.16b, v8.16b, v9.16b, #4    \n"
              "fmla v4.4s, v10.4s, v7.4s[1]        \n"
              "ext  v11.16b, v8.16b, v9.16b, #8    \n"

              "fmla v13.4s, v11.4s, v7.4s[2]       \n"

              "fadd v1.4s, v1.4s, v2.4s            \n"
              "fadd v1.4s, v1.4s, v12.4s           \n"
              "fadd v3.4s, v3.4s, v4.4s            \n"
              "fadd v3.4s, v3.4s, v13.4s           \n"

              "st1  {v1.4s}, [%[outptr0]]          \n"
              "st1  {v3.4s}, [%[outptr1]]          \n"

              "add  %[outptr0], %[outptr0], #16    \n"
              "add  %[outptr1], %[outptr1], #16    \n"

              "subs %[nn], %[nn], #1               \n"
              "bne  0b                             \n"
              : [nn] "+r"(nn),
              [outptr0]"+r"(outptr0),
              [outptr1]"+r"(outptr1),
              [r0]"+r"(r0),
              [r1]"+r"(r1),
              [r2]"+r"(r2),
              [r3]"+r"(r3)
              : [k0] "r" (k0), [k1] "r" (k1), [k2] "r" (k2)
            : "cc", "memory", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", "v10", "v11", "v12", "v13"
            );
        }

#else

        for (; nn > 0; nn--) {
          float32x4_t _sum1 = vld1q_f32(outptr0);
          float32x4_t _sum2 = vdupq_n_f32(0.f);
          float32x4_t _sum3 = vld1q_f32(outptr1);
          float32x4_t _sum4 = vdupq_n_f32(0.f);

          float32x4_t _r00 = vld1q_f32(r0);
          float32x4_t _r00n = vld1q_f32(r0 + 4);
          float32x4_t _r01 = vextq_f32(_r00, _r00n, 1);
          float32x4_t _r02 = vextq_f32(_r00, _r00n, 2);

          float32x4_t _r10 = vld1q_f32(r1);
          float32x4_t _r10n = vld1q_f32(r1 + 4);
          float32x4_t _r11 = vextq_f32(_r10, _r10n, 1);
          float32x4_t _r12 = vextq_f32(_r10, _r10n, 2);

          float32x4_t _r20 = vld1q_f32(r2);
          float32x4_t _r20n = vld1q_f32(r2 + 4);
          float32x4_t _r21 = vextq_f32(_r20, _r20n, 1);
          float32x4_t _r22 = vextq_f32(_r20, _r20n, 2);

          float32x4_t _r30 = vld1q_f32(r3);
          float32x4_t _r30n = vld1q_f32(r3 + 4);
          float32x4_t _r31 = vextq_f32(_r30, _r30n, 1);
          float32x4_t _r32 = vextq_f32(_r30, _r30n, 2);

#if 0
          _sum1 = vfmaq_laneq_f32(_sum1, _r00, _k0123, 0);
          _sum2 = vfmaq_laneq_f32(_sum2, _r01, _k0123, 1);
          _sum1 = vfmaq_laneq_f32(_sum1, _r02, _k0123, 2);
          _sum2 = vfmaq_laneq_f32(_sum2, _r10, _k3456, 0);
          _sum1 = vfmaq_laneq_f32(_sum1, _r11, _k3456, 1);
          _sum2 = vfmaq_laneq_f32(_sum2, _r12, _k3456, 2);
          _sum1 = vfmaq_laneq_f32(_sum1, _r20, _k6789, 0);
          _sum2 = vfmaq_laneq_f32(_sum2, _r21, _k6789, 1);
          _sum1 = vfmaq_laneq_f32(_sum1, _r22, _k6789, 2);

          _sum3 = vfmaq_laneq_f32(_sum3, _r10, _k0123, 0);
          _sum4 = vfmaq_laneq_f32(_sum4, _r11, _k0123, 1);
          _sum3 = vfmaq_laneq_f32(_sum3, _r12, _k0123, 2);
          _sum4 = vfmaq_laneq_f32(_sum4, _r20, _k3456, 0);
          _sum3 = vfmaq_laneq_f32(_sum3, _r21, _k3456, 1);
          _sum4 = vfmaq_laneq_f32(_sum4, _r22, _k3456, 2);
          _sum3 = vfmaq_laneq_f32(_sum3, _r30, _k6789, 0);
          _sum4 = vfmaq_laneq_f32(_sum4, _r31, _k6789, 1);
          _sum3 = vfmaq_laneq_f32(_sum3, _r32, _k6789, 2);
#else
          _sum1 = vfmaq_laneq_f32(_sum1, _r00, _k0123, 0);
          _sum2 = vfmaq_laneq_f32(_sum2, _r01, _k0123, 1);
          _sum3 = vfmaq_laneq_f32(_sum3, _r10, _k0123, 0);
          _sum4 = vfmaq_laneq_f32(_sum4, _r11, _k0123, 1);

          _sum1 = vfmaq_laneq_f32(_sum1, _r02, _k0123, 2);
          _sum2 = vfmaq_laneq_f32(_sum2, _r10, _k3456, 0);
          _sum3 = vfmaq_laneq_f32(_sum3, _r12, _k0123, 2);
          _sum4 = vfmaq_laneq_f32(_sum4, _r20, _k3456, 0);

          _sum1 = vfmaq_laneq_f32(_sum1, _r11, _k3456, 1);
          _sum2 = vfmaq_laneq_f32(_sum2, _r12, _k3456, 2);
          _sum3 = vfmaq_laneq_f32(_sum3, _r21, _k3456, 1);
          _sum4 = vfmaq_laneq_f32(_sum4, _r22, _k3456, 2);

          _sum1 = vfmaq_laneq_f32(_sum1, _r20, _k6789, 0);
          _sum2 = vfmaq_laneq_f32(_sum2, _r21, _k6789, 1);
          _sum3 = vfmaq_laneq_f32(_sum3, _r30, _k6789, 0);
          _sum4 = vfmaq_laneq_f32(_sum4, _r31, _k6789, 1);

          _sum1 = vfmaq_laneq_f32(_sum1, _r22, _k6789, 2);
          _sum3 = vfmaq_laneq_f32(_sum3, _r32, _k6789, 2);
#endif

          _sum1 = vaddq_f32(_sum1, _sum2);
          _sum3 = vaddq_f32(_sum3, _sum4);

          vst1q_f32(outptr0, _sum1);
          vst1q_f32(outptr1, _sum3);

          r0 += 4;
          r1 += 4;
          r2 += 4;
          r3 += 4;
          outptr0 += 4;
          outptr1 += 4;
        }

#endif
#else

        if (nn > 0) {
          asm volatile(
              "veor       q6, q6              \n"
              "veor       q15, q15            \n"

              "pld        [%3, #192]          \n"
              "vld1.f32   {d18-d20}, [%3 :64] \n"
              "add  %3, #16                   \n"

              "veor       q13, q13            \n"
              "veor       q14, q14            \n"

              "vext.32    q11, q9, q10, #1    \n"
              "vext.32    q12, q9, q10, #2    \n"

              "0:                             \n"

              "pld        [%1, #128]          \n"
              "vld1.f32   {d14-d15}, [%1 :64] \n"

              "vmla.f32   q7, q9, %e14[0]     \n"
              "vmla.f32   q6, q11, %e14[1]    \n"
              "vmla.f32   q13, q12, %f14[0]   \n"

              "pld        [%4, #192]          \n"
              "vld1.f32   {d18-d20}, [%4]     \n"
              "add  %4, #16             \n"

              "vmla.f32   q7, q9, %e15[0]     \n"

              "vext.32    q11, q9, q10, #1    \n"
              "vext.32    q12, q9, q10, #2    \n"

              "vmla.f32   q6, q11, %e15[1]    \n"
              "vmla.f32   q13, q12, %f15[0]   \n"

              "pld        [%2, #128]          \n"
              "vld1.f32   {d16-d17}, [%2]     \n"

              "vmla.f32   q8, q9, %e14[0]     \n"
              "vmla.f32   q14, q11, %e14[1]   \n"
              "vmla.f32   q15, q12, %f14[0]   \n"

              "pld        [%5, #192]          \n"
              "vld1.f32   {d18-d20}, [%5 :64] \n"
              "add  %5, #16                   \n"

              "vmla.f32   q7, q9, %e16[0]     \n"

              "vext.32    q11, q9, q10, #1    \n"
              "vext.32    q12, q9, q10, #2    \n"

              "vmla.f32   q6, q11, %e16[1]    \n"
              "vmla.f32   q13, q12, %f16[0]   \n"

              "vmla.f32   q8, q9, %e15[0]     \n"
              "vmla.f32   q14, q11, %e15[1]   \n"
              "vmla.f32   q15, q12, %f15[0]   \n"

              "pld        [%6, #192]          \n"
              "vld1.f32   {d18-d20}, [%6]     \n"
              "add  %6, #16                   \n"

              "vmla.f32   q8, q9, %e16[0]     \n"

              "vext.32    q11, q9, q10, #1    \n"
              "vext.32    q12, q9, q10, #2    \n"

              "vmla.f32   q14, q11, %e16[1]   \n"
              "vmla.f32   q15, q12, %f16[0]   \n"

              "vadd.f32   q7, q7, q6          \n"
              "veor       q6, q6              \n"

              "pld        [%3, #192]          \n"
              "vld1.f32   {d18-d20}, [%3 :64] \n"

              "vadd.f32   q8, q8, q14         \n"
              "veor       q14, q14            \n"
              "vadd.f32   q7, q7, q13         \n"
              "veor       q13, q13            \n"
              "vadd.f32   q8, q8, q15         \n"
              "veor       q15, q15            \n"

              "vext.32    q11, q9, q10, #1    \n"
              "vext.32    q12, q9, q10, #2    \n"

              "add  %3, #16                   \n"

              "vst1.f32   {d14-d15}, [%1]!    \n"
              "vst1.f32   {d16-d17}, [%2]!    \n"

              "subs       %0, #1              \n"
              "bne        0b                  \n"

              "sub        %3, #16             \n"
              : "=r"(nn),         // %0
              "=r"(outptr0),     // %1
              "=r"(outptr1),    // %2
              "=r"(r0),         // %3
              "=r"(r1),         // %4
              "=r"(r2),         // %5
              "=r"(r3)          // %6
              : "0"(nn),
              "1"(outptr0),
              "2"(outptr1),
              "3"(r0),
              "4"(r1),
              "5"(r2),
              "6"(r3),
              "w"(_k0123),      // %14
              "w"(_k3456),      // %15
              "w"(_k6789)       // %16
              : "cc", "memory", "q6", "q7", "q8", "q9", "q10", "q11", "q12", "q13", "q14", "q15"
          );
        }

#endif // __aarch64__
#endif // __ARM_NEON

        for (; remain > 0; remain--) {
#if __ARM_NEON
          float32x4_t _r00 = vld1q_f32(r0);
          float32x4_t _r10 = vld1q_f32(r1);
          float32x4_t _r20 = vld1q_f32(r2);
          float32x4_t _r30 = vld1q_f32(r3);

          float32x4_t _sum = vmulq_f32(_r00, _k0123);
          _sum = vmlaq_f32(_sum, _r10, _k3456);
          _sum = vmlaq_f32(_sum, _r20, _k6789);

          float32x4_t _sum2 = vmulq_f32(_r10, _k0123);
          _sum2 = vmlaq_f32(_sum2, _r20, _k3456);
          _sum2 = vmlaq_f32(_sum2, _r30, _k6789);

          _sum = vsetq_lane_f32(*outptr0, _sum, 3);
          _sum2 = vsetq_lane_f32(*outptr1, _sum2, 3);

#if __aarch64__
          *outptr0++ = vaddvq_f32(_sum);
          *outptr1++ = vaddvq_f32(_sum2);
#else
          float32x2_t _ss = vadd_f32(vget_low_f32(_sum), vget_high_f32(_sum));
          float32x2_t _ss2 = vadd_f32(vget_low_f32(_sum2), vget_high_f32(_sum2));

          float32x2_t _sss2 = vpadd_f32(_ss, _ss2);

          *outptr0++ = vget_lane_f32(_sss2, 0);
          *outptr1++ = vget_lane_f32(_sss2, 1);
#endif // __aarch64__
#else
          *outptr0++ += r0[0] * k0[0] + r0[1] * k0[1] + r0[2] * k0[2] + r1[0] * k1[0] + r1[1] * k1[1] + r1[2] * k1[2] + r2[0] * k2[0] + r2[1] * k2[1] + r2[2] * k2[2];
          *outptr1++ += r1[0] * k0[0] + r1[1] * k0[1] + r1[2] * k0[2] + r2[0] * k1[0] + r2[1] * k1[1] + r2[2] * k1[2] + r3[0] * k2[0] + r3[1] * k2[1] + r3[2] * k2[2];

#endif
          r0++;
          r1++;
          r2++;
          r3++;
        }
      }

#endif

      for (; i < outh; i++) {
        int remain = outw;
        const float* r0 = img0 + i * src_w;
        const float* r1 = r0 + src_w * 1;
        const float* r2 = r0 + src_w * 2;
        float* outptr0 = outptr + i * outw;

#if __ARM_NEON
        int nn = outw >> 2;
        remain = outw & 3;
#if __aarch64__
        for (; nn > 0; nn--) {
          float32x4_t _sum1 = vld1q_f32(outptr0);
          float32x4_t _sum2 = vdupq_n_f32(0.f);

          float32x4_t _r00 = vld1q_f32(r0);
          float32x4_t _r00n = vld1q_f32(r0 + 4);
          float32x4_t _r01 = vextq_f32(_r00, _r00n, 1);
          float32x4_t _r02 = vextq_f32(_r00, _r00n, 2);

          float32x4_t _r10 = vld1q_f32(r1);
          float32x4_t _r10n = vld1q_f32(r1 + 4);
          float32x4_t _r11 = vextq_f32(_r10, _r10n, 1);
          float32x4_t _r12 = vextq_f32(_r10, _r10n, 2);

          float32x4_t _r20 = vld1q_f32(r2);
          float32x4_t _r20n = vld1q_f32(r2 + 4);
          float32x4_t _r21 = vextq_f32(_r20, _r20n, 1);
          float32x4_t _r22 = vextq_f32(_r20, _r20n, 2);

          _sum1 = vfmaq_laneq_f32(_sum1, _r00, _k0123, 0);
          _sum2 = vfmaq_laneq_f32(_sum2, _r01, _k0123, 1);
          _sum1 = vfmaq_laneq_f32(_sum1, _r02, _k0123, 2);
          _sum2 = vfmaq_laneq_f32(_sum2, _r10, _k3456, 0);
          _sum1 = vfmaq_laneq_f32(_sum1, _r11, _k3456, 1);
          _sum2 = vfmaq_laneq_f32(_sum2, _r12, _k3456, 2);
          _sum1 = vfmaq_laneq_f32(_sum1, _r20, _k6789, 0);
          _sum2 = vfmaq_laneq_f32(_sum2, _r21, _k6789, 1);
          _sum1 = vfmaq_laneq_f32(_sum1, _r22, _k6789, 2);

          _sum1 = vaddq_f32(_sum1, _sum2);

          vst1q_f32(outptr0, _sum1);

          r0 += 4;
          r1 += 4;
          r2 += 4;
          outptr0 += 4;
        }

#else

        if (nn > 0) {
          asm volatile(
              "pld        [%2, #192]          \n"
              "vld1.f32   {d16-d18}, [%2]     \n"// r0
              "add  %2, #16             \n"

              "veor       q13, q13            \n"
              "veor       q14, q14            \n"

              "vext.32    q10, q8, q9, #1     \n"
              "vext.32    q11, q8, q9, #2     \n"

              "0:                             \n"

              "pld        [%1, #128]          \n"
              "vld1.f32   {d14-d15}, [%1]     \n"// _sum

              "vmla.f32   q7, q8, %e10[0]     \n"
              "vmla.f32   q13, q10, %e10[1]   \n"
              "vmla.f32   q14, q11, %f10[0]   \n"

              "pld        [%3, #192]          \n"
              "vld1.f32   {d16-d18}, [%3]     \n"// r1
              "add  %3, #16             \n"

              "vmla.f32   q7, q8, %e11[0]     \n"

              "vext.32    q10, q8, q9, #1     \n"
              "vext.32    q11, q8, q9, #2     \n"

              "vmla.f32   q13, q10, %e11[1]   \n"
              "vmla.f32   q14, q11, %f11[0]   \n"

              "pld        [%4, #192]          \n"
              "vld1.f32   {d16-d18}, [%4]     \n"// r2
              "add  %4, #16             \n"

              "vmla.f32   q7, q8, %e12[0]     \n"

              "vext.32    q10, q8, q9, #1     \n"
              "vext.32    q11, q8, q9, #2     \n"

              "vmla.f32   q13, q10, %e12[1]   \n"
              "vmla.f32   q14, q11, %f12[0]   \n"

              "pld        [%2, #192]          \n"
              "vld1.f32   {d16-d18}, [%2]     \n"// r0
              "add  %2, #16             \n"

              "vadd.f32   q7, q7, q13         \n"
              "veor       q13, q13            \n"
              "vadd.f32   q7, q7, q14         \n"
              "veor       q14, q14            \n"

              "vext.32    q10, q8, q9, #1     \n"
              "vext.32    q11, q8, q9, #2     \n"

              "vst1.f32   {d14-d15}, [%1]!    \n"

              "subs       %0, #1              \n"
              "bne        0b                  \n"

              "sub        %2, #16             \n"
              : "=r"(nn),         // %0
              "=r"(outptr0),     // %1
              "=r"(r0),         // %2
              "=r"(r1),         // %3
              "=r"(r2)          // %4
              : "0"(nn),
              "1"(outptr0),
              "2"(r0),
              "3"(r1),
              "4"(r2),
              "w"(_k0123),      // %10
              "w"(_k3456),      // %11
              "w"(_k6789)       // %12
              : "cc", "memory", "q7", "q8", "q9", "q10", "q11", "q12", "q13", "q14", "q15"
          );
        }

#endif // __aarch64__
#endif // __ARM_NEON

        for (; remain > 0; remain--) {
#if __ARM_NEON
          float32x4_t _r00 = vld1q_f32(r0);
          float32x4_t _r10 = vld1q_f32(r1);
          float32x4_t _r20 = vld1q_f32(r2);

          float32x4_t _sum = vmulq_f32(_r00, _k0123);
          _sum = vmlaq_f32(_sum, _r10, _k3456);
          _sum = vmlaq_f32(_sum, _r20, _k6789);

          _sum = vsetq_lane_f32(*outptr0, _sum, 3);

#if __aarch64__
          *outptr0++ = vaddvq_f32(_sum);
#else
          float32x2_t _ss = vadd_f32(vget_low_f32(_sum), vget_high_f32(_sum));
          _ss = vpadd_f32(_ss, _ss);

          *outptr0++ = vget_lane_f32(_ss, 0);
#endif // __aarch64__
#else
          *outptr0++ += r0[0] * k0[0] + r0[1] * k0[1] + r0[2] * k0[2] + r1[0] * k1[0] + r1[1] * k1[1] + r1[2] * k1[2] + r2[0] * k2[0] + r2[1] * k2[1] + r2[2] * k2[2];

#endif
          r0++;
          r1++;
          r2++;
        }
      }

      kernel0 += 9;
    }
  }
}

#if 0
static void conv3x3s2_neon(const Blob* bottom_blob, Blob* top_blob, const float* _kernel, const float* _bias)
{
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int p, q, srccn = Blob_channels(bottom_blob);

  int outw = top_blob->w;
  int outh = top_blob->h;
  int dstcn = Blob_channels(top_blob);

  const int tailstep = w - 2 * outw + w;

  const float* kernel = _kernel;
  const float* bias = _bias;

  #pragma omp parallel for

  for (p = 0; p < dstcn; p++) {
    Blob out = Blob_channel(top_blob, p);

    const float bias0 = bias ? bias[p] : 0.f;
    const float* kernel0 = kernel + p * srccn * 9;

    Blob_fill(&out, bias0);


    for (q = 0; q < srccn; q++) {
      float* outptr0 = Blob_datap(&out);
      float* outptr1 = outptr0 + outw;

      const float* img0 = Blob_channel_p(bottom_blob, q);

      const float* r0 = img0;
      const float* r1 = img0 + w;
      const float* r2 = img0 + w * 2;

      const float* k0 = kernel0;
      const float* k1 = kernel0 + 3;
      const float* k2 = kernel0 + 6;

#if __ARM_NEON
      float32x4_t _k0123 = vld1q_f32(k0);
      float32x4_t _k3456 = vld1q_f32(k1);
      float32x4_t _k6789 = vld1q_f32(k2);
#endif // __ARM_NEON

      int i = 0;

      for (; i < outh; i++) {
#if __ARM_NEON
        int nn = outw >> 2;
        int remain = outw & 3;
#else
        int remain = outw;
#endif // __ARM_NEON

#if __ARM_NEON
#if __aarch64__

        for (; nn > 0; nn--) {
          float32x4_t _outp = vld1q_f32(outptr0);

          float32x4x2_t _r0 = vld2q_f32(r0);
          float32x4x2_t _r0n = vld2q_f32(r0 + 8);

          float32x4_t _r00 = _r0.val[0];// 0 2 4 6
          float32x4_t _r01 = _r0.val[1];// 1 3 5 7
          float32x4_t _r02 = vextq_f32(_r00, _r0n.val[0], 1);// 2 4 6 8

          _outp = vfmaq_laneq_f32(_outp, _r00, _k0123, 0);
          _outp = vfmaq_laneq_f32(_outp, _r01, _k0123, 1);
          _outp = vfmaq_laneq_f32(_outp, _r02, _k0123, 2);

          float32x4x2_t _r1 = vld2q_f32(r1);
          float32x4x2_t _r1n = vld2q_f32(r1 + 8);

          float32x4_t _r10 = _r1.val[0];
          float32x4_t _r11 = _r1.val[1];
          float32x4_t _r12 = vextq_f32(_r10, _r1n.val[0], 1);

          _outp = vfmaq_laneq_f32(_outp, _r10, _k3456, 0);
          _outp = vfmaq_laneq_f32(_outp, _r11, _k3456, 1);
          _outp = vfmaq_laneq_f32(_outp, _r12, _k3456, 2);

          float32x4x2_t _r2 = vld2q_f32(r2);
          float32x4x2_t _r2n = vld2q_f32(r2 + 8);

          float32x4_t _r20 = _r2.val[0];
          float32x4_t _r21 = _r2.val[1];
          float32x4_t _r22 = vextq_f32(_r20, _r2n.val[0], 1);

          _outp = vfmaq_laneq_f32(_outp, _r20, _k6789, 0);
          _outp = vfmaq_laneq_f32(_outp, _r21, _k6789, 1);
          _outp = vfmaq_laneq_f32(_outp, _r22, _k6789, 2);

          vst1q_f32(outptr0, _outp);

          r0 += 8;
          r1 += 8;
          r2 += 8;
          outptr0 += 4;
        }

#else

        if (nn > 0) {
          asm volatile(
              "pld        [%2, #256]          \n"
              "vld2.f32   {d4-d7}, [%2]!      \n"

              "veor       q10, q10            \n"
              "veor       q11, q11            \n"

              "0:                             \n"
              "pld        [%1, #128]          \n"
              "vld1.f32   {d0-d1}, [%1]       \n"

              "vmla.f32   q0, q2, %e10[0]     \n"
              "vmla.f32   q10, q3, %e10[1]    \n"

              "pld        [%2, #256]          \n"
              "vld2.f32   {d16-d19}, [%2]     \n"
              "vext.32    q1, q2, q8, #1      \n"

              "vmla.f32   q11, q1, %f10[0]    \n"

              "pld        [%3, #256]          \n"
              "vld2.f32   {d4-d7}, [%3]!      \n"

              "vmla.f32   q0, q2, %e11[0]     \n"
              "vmla.f32   q10, q3, %e11[1]    \n"

              "pld        [%3, #256]          \n"
              "vld2.f32   {d16-d19}, [%3]     \n"
              "vext.32    q1, q2, q8, #1      \n"

              "vmla.f32   q11, q1, %f11[0]    \n"

              "pld        [%4, #256]          \n"
              "vld2.f32   {d4-d7}, [%4]!      \n"

              "vmla.f32   q0, q2, %e12[0]     \n"
              "vmla.f32   q10, q3, %e12[1]    \n"

              "pld        [%4, #256]          \n"
              "vld2.f32   {d16-d19}, [%4]     \n"
              "vext.32    q1, q2, q8, #1      \n"

              "vmla.f32   q11, q1, %f12[0]    \n"

              "pld        [%2, #256]          \n"
              "vld2.f32   {d4-d7}, [%2]!      \n"

              "vadd.f32   q0, q0, q10         \n"
              "veor       q10, q10            \n"
              "vadd.f32   q0, q0, q11         \n"
              "veor       q11, q11            \n"

              "subs       %0, #1              \n"
              "vst1.f32   {d0-d1}, [%1]!      \n"
              "bne        0b                  \n"
              "sub        %2, #32             \n"
              : "=r"(nn),     // %0
              "=r"(outptr0), // %1
              "=r"(r0),     // %2
              "=r"(r1),
              "=r"(r2)
              : "0"(nn),
              "1"(outptr0),
              "2"(r0),
              "3"(r1),
              "4"(r2),
              "w"(_k0123),  // %10
              "w"(_k3456),  // %11
              "w"(_k6789)   // %12
              : "cc", "memory", "q0", "q1", "q2", "q3", "q8", "q9", "q10", "q11", "q12", "q13", "q14", "q15"
          );
        }

#endif // __aarch64__
#endif // __ARM_NEON

        for (; remain > 0; remain--) {
#if __ARM_NEON
          float32x4_t _r00 = vld1q_f32(r0);
          float32x4_t _r10 = vld1q_f32(r1);
          float32x4_t _r20 = vld1q_f32(r2);

          float32x4_t _sum = vmulq_f32(_r00, _k0123);
          _sum = vmlaq_f32(_sum, _r10, _k3456);
          _sum = vmlaq_f32(_sum, _r20, _k6789);

          _sum = vsetq_lane_f32(*outptr0, _sum, 3);

#if __aarch64__
          *outptr0 = vaddvq_f32(_sum);
#else
          float32x2_t _ss = vadd_f32(vget_low_f32(_sum), vget_high_f32(_sum));
          _ss = vpadd_f32(_ss, _ss);

          *outptr0 = vget_lane_f32(_ss, 0);
#endif // __aarch64__
#else
          float sum0 = 0;

          sum0 += r0[0] * k0[0];
          sum0 += r0[1] * k0[1];
          sum0 += r0[2] * k0[2];
          sum0 += r1[0] * k1[0];
          sum0 += r1[1] * k1[1];
          sum0 += r1[2] * k1[2];
          sum0 += r2[0] * k2[0];
          sum0 += r2[1] * k2[1];
          sum0 += r2[2] * k2[2];

          *outptr0 += sum0;
#endif // __ARM_NEON

          r0 += 2;
          r1 += 2;
          r2 += 2;
          outptr0++;
        }

        r0 += tailstep;
        r1 += tailstep;
        r2 += tailstep;
      }

      kernel0 += 9;
    }
  }
}
#endif
