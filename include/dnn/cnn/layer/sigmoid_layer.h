
static int sigmoid_forward(const float* prevData, float* nextData, DataSize prevSize)
{
  int w = prevSize.w;
  int h = prevSize.h;
  int channels = prevSize.c;
  int i, q, size = w * h;
  DataSize nextSize = prevSize;
  
  if (1) {
    if (prevData==nextData) {
#pragma omp parallel for
      for (q = 0; q < channels; q++) {
        float* ptr = nextData + q * Data_2DSize(nextSize);
        
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
    } else {
#pragma omp parallel for
      for (q = 0; q < channels; q++) {
        const float* ptr = prevData + q * Data_2DSize(prevSize);
        float* outptr = nextData + q * Data_2DSize(nextSize);
        
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
    }
    
    
  }
  if (0) {
    if (prevData==nextData) {
#pragma omp parallel for
      for (q = 0; q < channels; q++) {
        float* ptr = nextData + q * Data_2DSize(nextSize);
        
        for (i = 0; i < size; i++) {
          ptr[i] = 1.f / (1.f + exp(-ptr[i]));
        }
      }
    } else {
#pragma omp parallel for
      for (q = 0; q < channels; q++) {
        const float* ptr = prevData + q * Data_2DSize(prevSize);
        float* outptr = nextData + q * Data_2DSize(nextSize);
        
        for (i = 0; i < size; i++) {
          outptr[i] = 1.f / (1.f + exp(-ptr[i]));
        }
      }
    }
  }
  return 0;
}

static void blas_ssigmoid2(const float* X, float* y, int len)
{
  int i;
  for (i = 0; i < len; i++) {
    float x = X[i];
    y[i] = 0.5 * tanh(0.5 * x) + 0.5;
  }
}

#define net_sigmoid(net, input) net_null_1_1(net, input, SigmoidLayer, "sigmoid")
static Dtype SigmoidLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  switch (op) {
  case Op_Forward:
    next->size = prev->size;
    {
      const float* prevData = getData(prev);
      float* nextData = setDataZero(next);
      DISPATCH_WORKER_BEGIN(prev->size.n) {
        const int offset = start * Data_3DSize(prev->size);
        const int total_size = (stop - start) * Data_3DSize(prev->size);
        blas_ssigmoid(prevData + offset, nextData + offset, total_size);
        //blas_ssigmoid2(prevData + offset, nextData + offset, total_size);
      };
      DISPATCH_WORKER_END;
    }
    break;
  case Op_Backward: {
    const float* nextData = getData(next);
    float* prevDiff = setDiffZero(prev);
    const float* nextDiff = getDiff(next);
    easyAssert(Data_4DEqu(prev->size, next->size), "size must be equal!");
    //update prevDiff
    if (1) {
      DISPATCH_WORKER_BEGIN(prev->size.n) {
        const int offset = start * Data_3DSize(prev->size);
        const int total_size = (stop - start) * Data_3DSize(prev->size);
        //calculate current inner diff
        df_sigmoid(nextData + offset, prevDiff + offset, total_size);
        //multiply next diff
        mul_inplace(prevDiff + offset, nextDiff + offset, total_size);
      };
      DISPATCH_WORKER_END;
    }
    if (0) {
      int i, count = Data_count(prev->size);
      for (i = 0; i < count; ++i) {
        const Dtype sigmoid_x = nextData[i];
        prevDiff[i] = nextDiff[i] * sigmoid_x * (1. - sigmoid_x);
      }
    }
    //update this layer's param
    //Tanh layer : nop
                    }
    break;
  default:
    break;
  }
  return 0;
}
