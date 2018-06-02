
static int AbsVal_forward_fast(const float* prevData, float* nextData, DataSize size)
{
  int q, nn, w = size.w;
  int h = size.h;
  int channels = size.c;
  int count = w * h;

  for (nn=0; nn<size.n; ++nn) {
#pragma omp parallel for
    for (q = 0; q < channels; q++) {
      const float* ptr = prevData + Data_4DIndex2(size, nn, q);
      float* outptr = nextData + Data_4DIndex2(size, nn, q);
      
#if __ARM_NEON
      int nn = count >> 2;
      int remain = count - (nn << 2);
#else
      int remain = count;
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
  }
  return 0;
}

static int AbsVal_forward_inplace_fast(float* nextData, DataSize size) {
  int q, nn, w = size.w;
  int h = size.h;
  int channels = size.c;
  int count = w * h;

  for (nn=0; nn < size.n; ++nn) {
#pragma omp parallel for
    for (q = 0; q < channels; q++) {
      float* ptr = nextData + Data_4DIndex2(size, nn, q);
      
#if __ARM_NEON
      int nn = count >> 2;
      int remain = count - (nn << 2);
#else
      int remain = count;
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
  }
  return 0;
}
static int AbsVal_forward(const float* prevData, float* nextData, DataSize size)
{
  int nn, w = size.w, h = size.h;
  int q, i, channels = size.c;
  int count = w * h;
  
  for (nn; nn<size.n; ++nn) {
    if (prevData==nextData) {
#pragma omp parallel for
      for (q = 0; q < channels; q++) {
        float* ptr = nextData + Data_4DIndex2(size, nn, q);
        
        for (i = 0; i < count; i++) {
          if (ptr[i] < 0) {
            ptr[i] = -ptr[i];
          }
        }
      }
    } else {
#pragma omp parallel for
      for (q = 0; q < channels; q++) {
        const float* ptr = prevData + Data_4DIndex2(size, nn, q);
        float* outptr = nextData + Data_4DIndex2(size, nn, q);
        
        for (i = 0; i < count; i++) {
          if (ptr[i] < 0) {
            outptr[i] = -ptr[i];
          }
          else {
            outptr[i] = ptr[i];
          }
        }
      }
    }
  }  
  return 0;
}

// @brief Computes @f$ y = |x| @f$
//
// @param bottom input Blob vector (length 1)
//   -# @f$ (N \times C \times H \times W) @f$
//      the inputs @f$ x @f$
// @param top output Blob vector (length 1)
//   -# @f$ (N \times C \times H \times W) @f$
//      the computed outputs @f$ y = |x| @f$
static Dtype AbsValLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  int count = Data_count(prev->size);
  switch (op) {
  case Op_Forward:
    next->size = prev->size;
    {
      const float* prevData = getData(prev);
      float* nextData = setDataZero(next);
      blas_sabs(count, prevData, nextData);
    }
    break;
  case Op_Backward:
    {
      const float* prevData = getData(prev);
      const float* nextDiff = getDiff(next);
      float* prevDiff = setDiff(prev);
      blas_ssign(count, prevData, prevDiff);
      blas_smul(count, prevDiff, nextDiff, prevDiff);
    }
    break;
  default:
    break;
  }
  return 0;
}
