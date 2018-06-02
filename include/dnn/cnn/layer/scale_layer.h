

static int scale_forward_fast(const float* prevData, const float* scaleData, const float* biasData, float* nextData, DataSize size)
{
  int nn, q, w = size.w, h = size.h;
  int channels = size.c;
  int count = w * h;

  for (nn; nn<size.n; ++nn) {
    if (biasData) {
      const float* scale_ptr = scaleData;
      const float* bias_ptr = biasData;
#pragma omp parallel for
      
      for (q = 0; q < channels; q++) {
        const float* ptr = prevData + Data_4DIndex2(size, nn, q);
        float* outptr = nextData + Data_4DIndex2(size, nn, q);
        
        float s = scale_ptr[q];
        float bias = bias_ptr[q];
        
#if __ARM_NEON
        int nn = count >> 2;
        int remain = count - (nn << 2);
#else
        int remain = count;
#endif // __ARM_NEON
        
#if __ARM_NEON
        float32x4_t _s = vdupq_n_f32(s);
        float32x4_t _bias = vdupq_n_f32(bias);
        
        for (; nn > 0; nn--) {
          float32x4_t _p = vld1q_f32(ptr);
          _p = vmlaq_f32(_bias, _p, _s);
          vst1q_f32(outptr, _p);
          
          ptr += 4;
          outptr += 4;
        }
        
#endif // __ARM_NEON
        
        for (; remain > 0; remain--) {
          *outptr = *ptr * s + bias;
          
          ptr++;
          outptr++;
        }
      }
    }
    else {
      const float* scale_ptr = scaleData;
#pragma omp parallel for
      
      for (q = 0; q < channels; q++) {
        const float* ptr = prevData + Data_4DIndex2(size, nn, q);
        float* outptr = nextData + Data_4DIndex2(size, nn, q);
        
        float s = scale_ptr[q];
        
#if __ARM_NEON
        int nn = count >> 2;
        int remain = count - (nn << 2);
#else
        int remain = count;
#endif // __ARM_NEON
        
#if __ARM_NEON
        float32x4_t _s = vdupq_n_f32(s);
        
        for (; nn > 0; nn--) {
          float32x4_t _p = vld1q_f32(ptr);
          _p = vmulq_f32(_p, _s);
          vst1q_f32(outptr, _p);
          
          ptr += 4;
          outptr += 4;
        }
        
#endif // __ARM_NEON
        
        for (; remain > 0; remain--) {
          *outptr = *ptr * s;
          
          ptr++;
          outptr++;
        }
      }
    }
  }

  return 0;
}
static int scale_forward_inplace(const float* scaleData, const float* biasData, float* nextData, DataSize size)
{
  int q, nn, w = size.w, h = size.h;
  int channels = size.c;
  int count = w * h;

  for (nn; nn<size.n; ++nn) {
    if (biasData) {
      const float* scale_ptr = scaleData;
      const float* bias_ptr = biasData;
#pragma omp parallel for
      
      for (q = 0; q < channels; q++) {
        float* ptr = nextData + Data_4DIndex2(size, nn, q);
        
        float s = scale_ptr[q];
        float bias = bias_ptr[q];
        
#if __ARM_NEON
        int nn = count >> 2;
        int remain = count - (nn << 2);
#else
        int remain = count;
#endif // __ARM_NEON
        
#if __ARM_NEON
        float32x4_t _s = vdupq_n_f32(s);
        float32x4_t _bias = vdupq_n_f32(bias);
        
        for (; nn > 0; nn--) {
          float32x4_t _p = vld1q_f32(ptr);
          _p = vmlaq_f32(_bias, _p, _s);
          vst1q_f32(ptr, _p);
          
          ptr += 4;
        }
        
#endif // __ARM_NEON
        
        for (; remain > 0; remain--) {
          *ptr = *ptr * s + bias;
          
          ptr++;
        }
      }
    }
    else {
      const float* scale_ptr = scaleData;
#pragma omp parallel for
      
      for (q = 0; q < channels; q++) {
        float* ptr = nextData + Data_4DIndex2(size, nn, q);
        
        float s = scale_ptr[q];
        
#if __ARM_NEON
        int nn = count >> 2;
        int remain = count - (nn << 2);
#else
        int remain = count;
#endif // __ARM_NEON
        
#if __ARM_NEON
        float32x4_t _s = vdupq_n_f32(s);
        
        for (; nn > 0; nn--) {
          float32x4_t _p = vld1q_f32(ptr);
          _p = vmulq_f32(_p, _s);
          vst1q_f32(ptr, _p);
          
          ptr += 4;
        }
        
#endif // __ARM_NEON
        
        for (; remain > 0; remain--) {
          *ptr *= s;
          
          ptr++;
        }
      }
    }
  }
  return 0;
}
static int scale_forward(const float* prevData, const float* scaleData, const float* biasData, float* nextData, DataSize size)
{
  int i, n, d, nn, q;
  int outer_dim = size.n;
  int bias_dim = size.c;
  int scale_dim = bias_dim;
  int inner_dim = size.w * size.h;
  if (1) {
    for (n = 0; n < outer_dim; ++n) {
      for (d = 0; d < scale_dim; ++d) {
        const float factor = scaleData[d];
        int off = (n*scale_dim+d)*inner_dim;
        blas_sscale(inner_dim, factor, prevData+off, nextData+off);
      }
    }
    if (biasData) {
      bias_forward(prevData, biasData, nextData, size);
    }
  }
  if (0) {
    if (prevData==nextData) {
      if (biasData) {
        for (nn=0; nn<outer_dim; ++nn) {
          const float* scale_ptr = scaleData;
          const float* bias_ptr = biasData;
#pragma omp parallel for
          
          for (q = 0; q < scale_dim; q++) {
            float* ptr = nextData + (nn * scale_dim+q)*inner_dim;
            float s = scale_ptr[q];
            float bias = bias_ptr[q];
            for (i = 0; i < inner_dim; i++) {
              ptr[i] = ptr[i] * s + bias;
            }
          }
        }
      }
      else {
        for (nn=0; nn<outer_dim; ++nn) {
          const float* scale_ptr = scaleData;
#pragma omp parallel for
          
          for (q = 0; q < scale_dim; q++) {
            float* ptr = nextData + (nn * scale_dim+q)*inner_dim;
            float s = scale_ptr[q];
            
            for (i = 0; i < inner_dim; i++) {
              ptr[i] *= s;
            }
          }
        }
      }
    } else {
      if (biasData) {
        for (nn=0; nn<outer_dim; ++nn) {
          const float* scale_ptr = scaleData;
          const float* bias_ptr = biasData;
#pragma omp parallel for
          
          for (q = 0; q < scale_dim; q++) {
            const float* ptr = prevData + (nn * scale_dim+q)*inner_dim;
            float* outptr = nextData + (nn * scale_dim+q)*inner_dim;
            float s = scale_ptr[q];
            float bias = bias_ptr[q];
            
            for (i = 0; i < inner_dim; i++) {
              outptr[i] = ptr[i] * s + bias;
            }
          }
        }
      }
      else {
        for (nn=0; nn<outer_dim; ++nn) {
          const float* scale_ptr = scaleData;
#pragma omp parallel for
          
          for (q = 0; q < scale_dim; q++) {
            const float* ptr = prevData + (nn * scale_dim+q)*inner_dim;
            float* outptr = nextData + (nn * scale_dim+q)*inner_dim;
            
            float s = scale_ptr[q];
            
            for (i = 0; i < inner_dim; i++) {
              outptr[i] = ptr[i] * s;
            }
          }
        }
      }
    }
  }
  return 0;
}
static int scale_backward(const float* prevData, const float* nextDiff, const float* scaleData, float* prevDiff, float* scaleDiff, float* biasDiff, DataSize size, bool accum, bool scale_param)
{
  int n, d;
  int outer_dim = size.n;
  int bias_dim = size.c;
  int scale_dim = bias_dim;
  int inner_dim = size.w * size.h;
  int sum_result_count = outer_dim * scale_dim;
  float* sum_multiplier = NULL;
  float* sum_result_buf = NULL;
  const int sum_mult_size = MAX(outer_dim, inner_dim);
  resetArr(&sum_multiplier, sum_mult_size);
  blas_sset(sum_mult_size, (1), sum_multiplier);

  if (biasDiff) {
    bias_backward(nextDiff, prevDiff, biasDiff, size, accum);
  }
  if (1) {
    float* product = prevDiff;
    blas_smul(Data_count(size), nextDiff, prevData, product);
    if (1) {
      float* sum_result = NULL;
      if (inner_dim == 1) {
        sum_result = product;
      } else if (sum_result_count == 1) {
        const float* sum_mult = sum_multiplier;
        if (scale_param) {
          float result = blas_sdot(inner_dim, product, sum_mult);
          *scaleDiff += result;
        } else {
          *scaleDiff = blas_sdot(inner_dim, product, sum_mult);
        }
      } else {
        const float* sum_mult = sum_multiplier;
        sum_result = (outer_dim == 1) ? scaleDiff : resetArr(&sum_result_buf, sum_result_count);
        blas_sgemv(CblasNoTrans, sum_result_count, inner_dim, (1), product, sum_mult, (0), sum_result);
      }
      if (outer_dim != 1) {
        const float* sum_mult = sum_multiplier;
        if (scale_dim == 1) {
          if (scale_param) {
            float result = blas_sdot(outer_dim, sum_mult, sum_result);
            *scaleDiff += result;
          } else {
            *scaleDiff = blas_sdot(outer_dim, sum_mult, sum_result);
          }
        } else {
          blas_sgemv(CblasTrans, outer_dim, scale_dim, (1), sum_result, sum_mult, (scale_param), scaleDiff);
        }
      }
    }
  }
  if (1) {
    for (n = 0; n < outer_dim; ++n) {
      for (d = 0; d < scale_dim; ++d) {
        const float factor = scaleData[d];
        blas_sscale(inner_dim, factor, nextDiff, prevDiff);
        prevDiff += inner_dim;
        nextDiff += inner_dim;
      }
    }
  }
  FREE(sum_result_buf);
  FREE(sum_multiplier);
  return 0;
}
static Dtype ScaleLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  struct Blob* scale = (layer->i_size > 1) ? IBLOB(1) : WBLOB(0);
  struct Blob* bias = (layer->i_size > 2) ? IBLOB(2) : (layer->w_size > 1) ? WBLOB(1) : NULL;
  ASSERT(layer->w_size>0);

  switch (op) {
  case Op_Forward:
    next->size = prev->size;
    {
      const float* prevData = getData(prev);
      const float* scaleData = getData(scale);
      const float* biasData = bias ? getData(bias) : NULL;
      float* nextData = setData(next);
#if 0
      if (this->layer_param_.scale_param().has_min_value()) {
        for (d = 0; d < scale_dim; d++) {
          scaleData[d] = MAX(scaleData[d], this->layer_param_.scale_param().min_value());
        }
      }
      if (this->layer_param_.scale_param().has_max_value()) {
        for (d = 0; d < scale_dim; d++) {
          scaleData[d] = min(scaleData[d], this->layer_param_.scale_param().max_value());
        }
      }
#endif
      scale_forward(prevData, scaleData, biasData, nextData, prev->size);
    }
    break;
  case Op_Backward:
    {
      const bool scale_param = (layer->i_size == 1);
      const float* nextDiff = getDiff(next);
      const float* prevData = getData(prev);
      const float* scaleData = getData(scale);
      float* scaleDiff = setDiffZero(scale);
      float* prevDiff = setDiffZero(prev);
      float* biasDiff = bias ? setDiffZero(bias) : NULL;
      scale_backward(prevData, nextDiff, scaleData, prevDiff, scaleDiff, biasDiff, prev->size, false, scale_param);
    }
    break;
  default:
    break;
  }
  return 0;
}
static int net_scale(struct Net* net, int input)
{
  const char* type = "scale";
  struct Layer* layer = net_addLayer(net, 1, 1, 2, ScaleLayer, type);
  DataSize prevSize = IBLOB(0)->size;
  int s, b;
  LIBLOB(0) = input;
  s = LWBLOB(0) = net_addBlob(net, dataSize(1, prevSize.c, 1, 1), type, "s");
  b = LWBLOB(1) = net_addBlob(net, dataSize(1, prevSize.c, 1, 1), type, "b");
  Blob_init_const(BLOB(s), 1.f);
  Blob_init_const(BLOB(b), 0.f);
  return LOBLOB(0) = net_addBlob(net, IBLOB(0)->size, type, "o");
}
