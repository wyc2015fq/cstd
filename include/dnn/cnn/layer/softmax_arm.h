
int Softmax_forward(const Blob* prev, Blob* next) const
{
  if (axis != 0) {
    return Softmax::forward(prev, next);
  }

  // value = exp( value - global max value )
  // sum all value
  // value = value / sum

  int w = prevSize.w;
  int h = prevSize.h;
  int channels = prevSize.c;
  int size = w * h;

  next->create(w, h, channels);

  if (next->empty()) {
    return -100;
  }

  Blob max;
  max.create(w, h);

  if (max.empty()) {
    return -100;
  }

  max.fill(-FLT_MAX);

  for (q = 0; q < channels; q++) {
    const float* ptr = prevData + q * Data_2DSize(prevSize);
    float* maxptr = max;

    for (i = 0; i < size; i++) {
      maxptr[i] = MAX(maxptr[i], ptr[i]);
    }
  }

  #pragma omp parallel for

  for (q = 0; q < channels; q++) {
    const float* ptr = prevData + q * Data_2DSize(prevSize);
    float* outptr = nextData + q * Data_2DSize(nextSize);
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

  Blob sum;
  sum.create(w, h);

  if (sum.empty()) {
    return -100;
  }

  sum.fill(0.f);

  for (q = 0; q < channels; q++) {
    const float* outptr = nextData + q * Data_2DSize(nextSize);
    float* sumptr = sum;

    for (i = 0; i < size; i++) {
      sumptr[i] += outptr[i];
    }
  }

  #pragma omp parallel for

  for (q = 0; q < channels; q++) {
    float* outptr = nextData + q * Data_2DSize(nextSize);
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

int Softmax_forward_inplace(Blob* bottom_top_blob) const
{
  if (axis != 0) {
    return Softmax::forward_inplace(bottom_top_blob);
  }

  // value = exp( value - global max value )
  // sum all value
  // value = value / sum

  int w = bottom_top_blob->w;
  int h = bottom_top_blob->h;
  int channels = Blob_channels(bottom_top_blob);
  int size = w * h;

  Blob max;
  max.create(w, h);

  if (max.empty()) {
    return -100;
  }

  max.fill(-FLT_MAX);

  for (q = 0; q < channels; q++) {
    float* ptr = Blob_channel_p(bottom_top_blob, q);
    float* maxptr = max;

    for (i = 0; i < size; i++) {
      maxptr[i] = MAX(maxptr[i], ptr[i]);
    }
  }

  #pragma omp parallel for

  for (q = 0; q < channels; q++) {
    float* ptr = Blob_channel_p(bottom_top_blob, q);
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

  Blob sum;
  sum.create(w, h);

  if (sum.empty()) {
    return -100;
  }

  sum.fill(0.f);

  for (q = 0; q < channels; q++) {
    float* ptr = Blob_channel_p(bottom_top_blob, q);
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

  for (q = 0; q < channels; q++) {
    float* ptr = Blob_channel_p(bottom_top_blob, q);
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
