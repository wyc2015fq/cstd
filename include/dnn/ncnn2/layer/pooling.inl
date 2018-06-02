

typedef enum PoolMethod { PoolMethod_MAX = 0, PoolMethod_AVE = 1 } PoolMethod;

static void pooling2x2s2_max_neon(const img_t* bottom_blob, img_t* top_blob)
{
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int q, i, inch = pBox_channels(bottom_blob);

  int outw = top_blob->w;
  int outh = top_blob->h;
  int outch = pBox_channels(top_blob);

  #pragma omp parallel for

  for (q = 0; q < inch; q++) {
    const float* img0 = pBox_channel_p(bottom_blob, q);
    float* outptr = pBox_channel_p(top_blob, q);

    const float* r0 = img0;
    const float* r1 = img0 + w;

    for (i = 0; i < outh; i++) {
      int remain = outw;

#if CC_NEON
      int nn = outw >> 2;
      remain = outw - (nn << 2);
#if __aarch64__

      for (; nn > 0; nn--) {
        float32x4_t _r00 = vld1q_f32(r0);
        float32x4_t _r10 = vld1q_f32(r1);
        float32x4_t _r01 = vld1q_f32(r0 + 4);
        float32x4_t _r11 = vld1q_f32(r1 + 4);

        float32x4_t _max0 = vmaxq_f32(_r00, _r10);
        float32x4_t _max1 = vmaxq_f32(_r01, _r11);

        float32x4_t _max = vpmaxq_f32(_max0, _max1);

        vst1q_f32(outptr, _max);

        r0 += 8;
        r1 += 8;
        outptr += 4;
      }

#else

      if (nn > 0) {
        asm volatile(
            "0:                             \n"
            "pld        [%1, #256]          \n"
            "pld        [%2, #256]          \n"
            "vld1.f32   {d0-d3}, [%1]!      \n"
            "vld1.f32   {d4-d7}, [%2]!      \n"
            "vmax.f32   q0, q0, q2          \n"
            "vmax.f32   q1, q1, q3          \n"
            "vpmax.f32  d4, d0, d1          \n"
            "vpmax.f32  d5, d2, d3          \n"
            "subs       %0, #1              \n"
            "vst1.f32   {d4-d5}, [%3]!      \n"
            "bne        0b                  \n"
            : "=r"(nn),     // %0
            "=r"(r0),     // %1
            "=r"(r1),     // %2
            "=r"(outptr)  // %3
            : "0"(nn),
            "1"(r0),
            "2"(r1),
            "3"(outptr)
            : "cc", "memory", "q0", "q1", "q2", "q3"
        );
      }

#endif // __aarch64__
#endif // CC_NEON

      for (; remain > 0; remain--) {
        float max0 = MAX(r0[0], r0[1]);
        float max1 = MAX(r1[0], r1[1]);

        *outptr = MAX(max0, max1);

        r0 += 2;
        r1 += 2;
        outptr++;
      }

      r0 += w;
      r1 += w;
    }
  }
}

static void pooling3x3s2_max_neon(const img_t* bottom_blob, img_t* top_blob)
{
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int q, i, inch = pBox_channels(bottom_blob);

  int outw = top_blob->w;
  int outh = top_blob->h;
  int outch = pBox_channels(top_blob);

  const int tailstep = w - 2 * outw + w;

  #pragma omp parallel for

  for (q = 0; q < inch; q++) {
    const float* img0 = pBox_channel_p(bottom_blob, q);
    float* outptr = pBox_channel_p(top_blob, q);

    const float* r0 = img0;
    const float* r1 = img0 + w;
    const float* r2 = img0 + w * 2;

    for (i = 0; i < outh; i++) {
      int remain = outw;

#if CC_SSE2
#define _mm_loadu_ps2(a)  _mm_set_ps(*(a+6), *(a+4), *(a+2), *(a))

      for (; remain > 3; remain -= 4) {
        __m128 _max0 = _mm_max_ps(_mm_loadu_ps2(r0), _mm_loadu_ps2(r0 + 1));
        __m128 _max1 = _mm_max_ps(_mm_loadu_ps2(r1), _mm_loadu_ps2(r1 + 1));
        __m128 _max, _max2 = _mm_max_ps(_mm_loadu_ps2(r2), _mm_loadu_ps2(r2 + 1));

        _max0 = _mm_max_ps(_max0, _mm_loadu_ps2(r0 + 2));
        _max1 = _mm_max_ps(_max1, _mm_loadu_ps2(r1 + 2));
        _max2 = _mm_max_ps(_max2, _mm_loadu_ps2(r2 + 2));

        _max = _mm_max_ps(_mm_max_ps(_max0, _max1), _max2);
        _mm_storeu_ps(outptr, _max);

        r0 += 8;
        r1 += 8;
        r2 += 8;
        outptr += 4;
      }

#endif

#if CC_NEON
      int nn = outw >> 2;
      remain = outw - (nn << 2);
#if __aarch64__
      float32x4x2_t _r0 = vld2q_f32(r0);
      float32x4x2_t _r1 = vld2q_f32(r1);
      float32x4x2_t _r2 = vld2q_f32(r2);

      for (; nn > 0; nn--) {
        float32x4x2_t _r0n = vld2q_f32(r0 + 8);
        float32x4x2_t _r1n = vld2q_f32(r1 + 8);
        float32x4x2_t _r2n = vld2q_f32(r2 + 8);

        float32x4_t _max0 = vmaxq_f32(_r0.val[0], _r0.val[1]);
        float32x4_t _max1 = vmaxq_f32(_r1.val[0], _r1.val[1]);
        float32x4_t _max2 = vmaxq_f32(_r2.val[0], _r2.val[1]);

        float32x4_t _r02 = vextq_f32(_r0.val[0], _r0n.val[0], 1);
        float32x4_t _r12 = vextq_f32(_r1.val[0], _r1n.val[0], 1);
        float32x4_t _r22 = vextq_f32(_r2.val[0], _r2n.val[0], 1);

        _max0 = vmaxq_f32(_max0, _r02);
        _max1 = vmaxq_f32(_max1, _r12);
        _max2 = vmaxq_f32(_max2, _r22);

        float32x4_t _max = vmaxq_f32(vmaxq_f32(_max0, _max1), _max2);

        vst1q_f32(outptr, _max);

        _r0 = _r0n;
        _r1 = _r1n;
        _r2 = _r2n;

        r0 += 8;
        r1 += 8;
        r2 += 8;
        outptr += 4;
      }

#else

      if (nn > 0) {
        asm volatile(
            "pld        [%1, #256]          \n"
            "vld2.f32   {d0-d3}, [%1]!      \n"// q0 = 0 2 4 6  q1 = 1 3 5 7
            "pld        [%2, #256]          \n"
            "vld2.f32   {d4-d7}, [%2]!      \n"
            "pld        [%3, #256]          \n"
            "vld2.f32   {d8-d11}, [%3]!     \n"
            "0:                             \n"
            "pld        [%1, #256]          \n"
            "vld2.f32   {d12-d15}, [%1]!    \n"// q6 = 8 10 12 14  q7 = 9 11 13 15

            "vmax.f32   q12, q0, q1         \n"
            "vmax.f32   q13, q2, q3         \n"

            "pld        [%2, #256]          \n"
            "vld2.f32   {d16-d19}, [%2]!    \n"

            "vmax.f32   q14, q4, q5         \n"
            "vext.32    q0, q0, q6, #1      \n"

            "pld        [%3, #256]          \n"
            "vld2.f32   {d20-d23}, [%3]!    \n"

            "vext.32    q2, q2, q8, #1      \n"

            "vmax.f32   q12, q12, q0        \n"
            "vext.32    q4, q4, q10, #1     \n"

            "vmax.f32   q13, q13, q2        \n"
            "vmax.f32   q14, q14, q4        \n"
            "vmax.f32   q12, q12, q13       \n"

            "vorr       q0, q6, q6          \n"
            "vorr       q1, q7, q7          \n"
            "vmax.f32   q12, q12, q14       \n"

            "vorr       q2, q8, q8          \n"
            "vorr       q3, q9, q9          \n"
            "vorr       q4, q10, q10        \n"
            "vorr       q5, q11, q11        \n"

            "subs       %0, #1              \n"
            "vst1.f32   {d24-d25}, [%4]!    \n"
            "bne        0b                  \n"
            "sub        %1, #32             \n"
            "sub        %2, #32             \n"
            "sub        %3, #32             \n"
            : "=r"(nn),     // %0
            "=r"(r0),     // %1
            "=r"(r1),     // %2
            "=r"(r2),     // %3
            "=r"(outptr)  // %4
            : "0"(nn),
            "1"(r0),
            "2"(r1),
            "3"(r2),
            "4"(outptr)
            : "cc", "memory", "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8", "q9", "q10", "q11", "q12", "q13", "q14"
        );
      }

#endif // __aarch64__
#endif // CC_NEON

      for (; remain > 0; remain--) {
        float max0 = MAX(MAX(r0[0], r0[1]), r0[2]);
        float max1 = MAX(MAX(r1[0], r1[1]), r1[2]);
        float max2 = MAX(MAX(r2[0], r2[1]), r2[2]);

        *outptr = MAX(MAX(max0, max1), max2);

        r0 += 2;
        r1 += 2;
        r2 += 2;
        outptr++;
      }

      r0 += tailstep;//1 + w;
      r1 += tailstep;//1 + w;
      r2 += tailstep;//1 + w;
    }
  }
}

static int pooling_forward(buf_t* bf, const img_t* bottom_blob, img_t* top_blob,PoolMethod pooling_type,int kernel_size,int stride,int pad,int global_pooling)
{
  // max value in NxN window
  // avg value in NxN window

  img_t im[2] = {0};
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = pBox_channels(bottom_blob);
  int q, i, size = w * h;
  int outw, outh, wtail, htail;
  const img_t* bottom_blob_bordered = bottom_blob;

  //     fprintf(stderr, "Pooling     input %d x %d  pad = %d  ksize=%d  stride=%d\n", w, h, pad, kernel_size, stride);
  if (global_pooling) {
    bf_pBoxsetsize(bf, top_blob, 1, 1, channels, 1);
  if (NULL==top_blob->data) {
    return 0;
  }

    if (imempty(top_blob)) {
      return -100;
    }

    if (pooling_type == PoolMethod_MAX) {
      #pragma omp parallel for

      for (q = 0; q < channels; q++) {
        const float* ptr = pBox_channel_p(bottom_blob, q);
        float* outptr = pBox_channel_p(top_blob, q);

        float max = ptr[0];

        for (i = 0; i < size; i++) {
          max = MAX(max, ptr[i]);
        }

        outptr[0] = max;
      }
    }
    else if (pooling_type == PoolMethod_AVE) {
      #pragma omp parallel for

      for (q = 0; q < channels; q++) {
        const float* ptr = pBox_channel_p(bottom_blob, q);
        float* outptr = pBox_channel_p(top_blob, q);

        float sum = 0.f;

        for (i = 0; i < size; i++) {
          sum += ptr[i];
        }

        outptr[0] = sum / size;
      }
    }

    return 0;
  }


  if (pad > 0) {
    bottom_blob_bordered = copy_make_border(bf, bottom_blob, im, pad, pad, pad, pad, BORDER_CONSTANT, 0.f);
    w = bottom_blob_bordered->w;
    h = bottom_blob_bordered->h;
  }
  else if (pad == -233) {
    int wpad = kernel_size + (w - 1) / stride * stride - w;
    int hpad = kernel_size + (h - 1) / stride * stride - h;

    if (wpad > 0 || hpad > 0) {
      bottom_blob_bordered = copy_make_border(bf, bottom_blob, im, hpad / 2, hpad - hpad / 2, wpad / 2, wpad - wpad / 2, BORDER_CONSTANT, 0.f);
    }

    w = bottom_blob_bordered->w;
    h = bottom_blob_bordered->h;
  }

  outw = (w - kernel_size) / stride + 1;
  outh = (h - kernel_size) / stride + 1;

  wtail = (w - kernel_size) % stride;
  htail = (h - kernel_size) % stride;

  if (pad == -233 || pad == -2333) {
    wtail = 0;
    htail = 0;
  }

  if (wtail != 0 || htail != 0) {
    int wtailpad = 0;
    int htailpad = 0;
    img_t* bottom_blob_bordered2 = im + 1;

    if (wtail != 0) {
      wtailpad = kernel_size - wtail;
    }

    if (htail != 0) {
      htailpad = kernel_size - htail;
    }


    if (pooling_type == PoolMethod_MAX) {
      copy_make_border(bf, bottom_blob_bordered, bottom_blob_bordered2, 0, htailpad, 0, wtailpad, BORDER_REPLICATE, 0.f);
    }
    else if (pooling_type == PoolMethod_AVE) {
      copy_make_border(bf, bottom_blob_bordered, bottom_blob_bordered2, 0, htailpad, 0, wtailpad, BORDER_CONSTANT, 0.f);
    }

    bottom_blob_bordered = bottom_blob_bordered2;

    w = bottom_blob_bordered->w;
    h = bottom_blob_bordered->h;

    if (wtail != 0) {
      outw += 1;
    }

    if (htail != 0) {
      outh += 1;
    }
  }

  bf_pBoxsetsize(bf, top_blob, outh, outw, channels, 1);
  if (NULL==top_blob->data) {
    return 0;
  }

  if (kernel_size == 2 && pooling_type == PoolMethod_MAX && stride == 2) {
    pooling2x2s2_max_neon(bottom_blob_bordered, top_blob);
  }
  else if (kernel_size == 3 && pooling_type == PoolMethod_MAX && stride == 2) {
    pooling3x3s2_max_neon(bottom_blob_bordered, top_blob);
  }
  else {
    int i, j, k, q, mw;
  const int maxk = kernel_size * kernel_size;
    // kernel offsets
    int* space_ofs = NULL;
    BF_MALLOC(space_ofs, maxk);
    {
      int p1 = 0;
      int p2 = 0;
      int gap = w - kernel_size;

      for (i = 0; i < kernel_size; i++) {
        for (j = 0; j < kernel_size; j++) {
          space_ofs[p1] = p2;
          p1++;
          p2++;
        }

        p2 += gap;
      }
    }

    mw = bottom_blob_bordered->w;

    if (pooling_type == PoolMethod_MAX) {
      #pragma omp parallel for

      for (q = 0; q < channels; q++) {
        const float* mdata = pBox_channel_p(bottom_blob_bordered, q);
        float* outptr = pBox_channel_p(top_blob, q);

        for (i = 0; i < outh; i++) {
          for (j = 0; j < outw; j++) {
            const float* sptr = mdata + mw * i * stride + j * stride;

            float max = sptr[0];

            for (k = 0; k < maxk; k++) {
              float val = sptr[ space_ofs[k] ];
              max = MAX(max, val);
            }

            outptr[j] = max;
          }

          outptr += outw;
        }
      }
    }
    else if (pooling_type == PoolMethod_AVE) {
      #pragma omp parallel for

      for (q = 0; q < channels; q++) {
        const float* mdata = pBox_channel_p(bottom_blob_bordered, q);
        float* outptr = pBox_channel_p(top_blob, q);

        for (i = 0; i < outh; i++) {
          for (j = 0; j < outw; j++) {
            const float* sptr = mdata + mw * i * stride + j * stride;

            float sum = 0;

            for (k = 0; k < maxk; k++) {
              float val = sptr[ space_ofs[k] ];
              sum += val;
            }

            outptr[j] = sum / maxk;
          }

          outptr += outw;
        }

        // fix tail pad
        if (wtail != 0) {
          const float scale = (float)kernel_size / wtail;

          outptr = pBox_channel_p(top_blob, q) + outw - 1;

          for (i = 0; i < outh; i++) {
            *outptr *= scale;
            outptr += outw;
          }
        }

        if (htail != 0) {
          const float scale = (float)kernel_size / htail;

          outptr = pBox_crow(top_blob, q, outh - 1);

          for (i = 0; i < outw; i++) {
            outptr[i] *= scale;
          }
        }
      }
    }

    BF_FREE(space_ofs);
  }

  return 0;
}

struct Pooling
{
  int pooling_type;
  int kernel_size;
  int stride;
  int pad;
  int global_pooling;
};

static int Pooling_load_param(Layer* layer, const char* param) {
  struct Pooling* s = (struct Pooling*)layer->x;
  char* p = (char*)param;
  str_getint(p, &p, &s->pooling_type, 0);
  str_getint(p, &p, &s->kernel_size, 0);
  str_getint(p, &p, &s->stride, 1);
  str_getint(p, &p, &s->pad, 0);
  str_getint(p, &p, &s->global_pooling, 0);
  return 0;
}

static int Pooling_forward(buf_t* bf, const Layer* layer, const img_t* bottom_blob, img_t* top_blob) {
  struct Pooling* s = (struct Pooling*)layer->x;
  pooling_forward(bf, bottom_blob, top_blob, (PoolMethod)s->pooling_type,s->kernel_size,s->stride,s->pad,s->global_pooling);
  return 0;
}