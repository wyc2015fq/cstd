
#if 0
static int innerProduct(const Blob* prevData, Blob* next, int num_output, const float* weightData, const float* biasData)
{
  int w = prevData->w;
  int h = prevData->h;
  int channels = Blob_channels(prevData);
  int p, q, size = w * h;
  const float* weight_data_ptr = weightData;

  pBoxsetsize(next, 1, 1, num_output, 1);
  // num_output

  #pragma omp parallel for
  for (p = 0; p < num_output; p++) {
    float* outptr = Blob_channel_p(next, p);
    float sum = 0.f;
    const float* w = weight_data_ptr + size * channels * p;
    const float* w2 = w + size;

    if (biasData) {
      sum = biasData[p];
    }

#if __ARM_NEON
    float32x4_t _sum = vdupq_n_f32(0.f);
    float32x4_t _sum2 = vdupq_n_f32(0.f);
#endif // __ARM_NEON

    // channels
    for (q = 0; q < channels; q++) {
      const float* m = Blob_channel_p(prevData, q);
      int remain = size;

#if __ARM_NEON
      int nn = size >> 3;
      remain = size & 7;
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

#if 0
  #pragma omp parallel for

  for (p = 0; p < num_output; p++) {
    float* outptr = Blob_channel_p(next, p);
    float sum = 0.f;

    if (biasData) {
      sum = biasData[p];
    }

    // channels
    for (q = 0; q < channels; q++) {
      const float* w = weight_data_ptr + size * channels * p + size * q;
      const float* m = Blob_channel_p(prevData, q);

      for (i = 0; i < size; i++) {
        sum += m[i] * w[i];
      }
    }

    outptr[0] = sum;
  }

#endif

  return 0;
}

static int InnerProduct(enum LayerOp op, struct NetWork* net, struct Layer* layer)
{
  const Blob* prevData = LBLOB(0)->data;
  Blob* next = LBLOB(1)->data;
  struct InnerProduct* s = (struct InnerProduct*)layer->x;
  float* weightData = Blob_datap(WBLOB(0)->data);
  float* biasData = Blob_datap(WBLOB(1)->data);
  int num_output = WBLOB(0)->data->h;
  innerProduct(prevData, next, num_output, weightData, biasData);
  return 0;
}


#if 0

#if NCNN_STDIO
int InnerProduct::load_model(FILE* binfp)
{
  int nread;

  union {
    struct {
      unsigned char f0;
      unsigned char f1;
      unsigned char f2;
      unsigned char f3;
    };
    unsigned int tag;
  } flag_struct;

  nread = fread(&flag_struct, sizeof(flag_struct), 1, binfp);

  if (nread != 1) {
    fprintf(stderr, "InnerProduct read flag_struct failed %d\n", nread);
    return -1;
  }

  unsigned int flag = flag_struct.f0 + flag_struct.f1 + flag_struct.f2 + flag_struct.f3;

  weightData.create(weight_data_size);

  if (weightData.empty()) {
    return -100;
  }

  if (flag_struct.tag == 0x01306B47) {
    // half-precision weight data
    int align_weight_data_size = alignSize(weight_data_size * sizeof(unsigned short), 4);
    std::vector<unsigned short> float16_weights;
    float16_weights.resize(align_weight_data_size);
    nread = fread(float16_weights.data(), align_weight_data_size, 1, binfp);

    if (nread != 1) {
      fprintf(stderr, "InnerProduct read float16_weights failed %d\n", nread);
      return -1;
    }

    weightData = Blob::from_float16(float16_weights.data(), weight_data_size);

    if (weightData.empty()) {
      return -100;
    }
  }
  else if (flag != 0) {
    // quantized weight data
    float quantization_value[256];
    nread = fread(quantization_value, 256 * sizeof(float), 1, binfp);

    if (nread != 1) {
      fprintf(stderr, "InnerProduct read quantization_value failed %d\n", nread);
      return -1;
    }

    int align_weight_data_size = alignSize(weight_data_size * sizeof(unsigned char), 4);
    std::vector<unsigned char> index_array;
    index_array.resize(align_weight_data_size);
    nread = fread(index_array.data(), align_weight_data_size, 1, binfp);

    if (nread != 1) {
      fprintf(stderr, "InnerProduct read index_array failed %d\n", nread);
      return -1;
    }

    float* weight_data_ptr = weightData;

    for (int i = 0; i < weight_data_size; i++) {
      weight_data_ptr[i] = quantization_value[ index_array[i] ];
    }
  }
  else if (flag_struct.f0 == 0) {
    // raw weight data
    nread = fread(weightData, weight_data_size * sizeof(float), 1, binfp);

    if (nread != 1) {
      fprintf(stderr, "InnerProduct read weightData failed %d\n", nread);
      return -1;
    }
  }

  if (bias_term) {
    biasData.create(num_output);

    if (biasData.empty()) {
      return -100;
    }

    nread = fread(biasData, num_output * sizeof(float), 1, binfp);

    if (nread != 1) {
      fprintf(stderr, "InnerProduct read biasData failed %d\n", nread);
      return -1;
    }
  }

  return 0;
}
#endif // NCNN_STDIO

int InnerProduct::load_model(const unsigned char*& mem)
{
  union {
    struct {
      unsigned char f0;
      unsigned char f1;
      unsigned char f2;
      unsigned char f3;
    };
    unsigned int tag;
  } flag_struct;

  memcpy(&flag_struct, mem, sizeof(flag_struct));
  mem += sizeof(flag_struct);

  unsigned int flag = flag_struct.f0 + flag_struct.f1 + flag_struct.f2 + flag_struct.f3;

  if (flag_struct.tag == 0x01306B47) {
    // half-precision weight data
    weightData = Blob::from_float16((unsigned short*)mem, weight_data_size);
    mem += alignSize(weight_data_size * sizeof(unsigned short), 4);

    if (weightData.empty()) {
      return -100;
    }
  }
  else if (flag != 0) {
    // quantized weight data
    const float* quantization_value = (const float*)mem;
    mem += 256 * sizeof(float);

    const unsigned char* index_array = (const unsigned char*)mem;
    mem += alignSize(weight_data_size * sizeof(unsigned char), 4);

    weightData.create(weight_data_size);

    if (weightData.empty()) {
      return -100;
    }

    float* weight_data_ptr = weightData;

    for (int i = 0; i < weight_data_size; i++) {
      weight_data_ptr[i] = quantization_value[ index_array[i] ];
    }
  }
  else if (flag_struct.f0 == 0) {
    // raw weight data
    weightData = Blob(weight_data_size, (float*)mem);
    mem += weight_data_size * sizeof(float);
  }

  if (bias_term) {
    biasData = Blob(num_output, (float*)mem);
    mem += num_output * sizeof(float);
  }

  return 0;
}
#endif

static int InnerProductLayer(enum LayerOp op, struct NetWork* net, struct Layer* layer) {
  Blob* prev = LBLOB(0);
  Blob* next = LBLOB(1);
  Blob* weight_blob = WBLOB(0);
  int M_ = prev->data->f;
  int K_ = LARG(transpose) ? weightSize.h : weightSize.w;
  int N_ = LARG(transpose) ? weightSize.w : weightSize.h;
  ASSERT(K_==Blob_length(prev->data));
  // LARG(transpose)=0;  ///< if true, assume transposed weights
  float* bias_multiplier = NULL;
  int num_output = LARG(num_output);

  // Set up the bias multiplier
  if (LARG(bias_term)) {
    MYREALLOC(bias_multiplier, M_);
    ASSERT(bias_multiplier);
    blas_sset(M_, (1), bias_multiplier);
  }

  switch (op) {
  case LO_Forward:
    {
      Blob_create(next->data, 1, 1, num_output, prev->data->f);
      const float* prevData = Blob_datap(prev->data);
      float* nextData = Blob_datap(next->data);
      const float* weightData = Blob_datap(LBLOB(2)->data);

      blas_sgemm(CblasNoTrans, LARG(transpose) ? CblasNoTrans : CblasTrans, M_, N_, K_, (float)1.,
        prevData, weightData, (float)0., nextData);
      if (LARG(bias_term)) {
        const float* biasData = Blob_datap(WBLOB(1)->data);
        blas_sgemm(CblasNoTrans, CblasNoTrans, M_, N_, 1, (float)1., bias_multiplier, biasData, (float)1., nextData);
      }
    }
    break;
  case LO_Backward:
    {
      Blob_ReshapeLike(prev->diff, prev->data);
      const float* weightData = Blob_datap(LBLOB(2)->data);
      float* weightDiff = Blob_datap(LBLOB(2)->diff);
      const float* nextDiff = Blob_datap(next->diff);
      const float* prevData = Blob_datap(prev->data);
      float* prevDiff = Blob_datap(prev->diff);
      if (LARG(transpose)) {
        blas_sgemm(CblasTrans, CblasNoTrans, K_, N_, M_, (float)1., prevData, nextDiff, (float)1., weightDiff);
      }
      else {
        blas_sgemm(CblasTrans, CblasNoTrans, N_, K_, M_, (float)1., nextDiff, prevData, (float)1., weightDiff);
      }
      
      if (LARG(bias_term)) {
        float* biasDiff = Blob_datap(LBLOB(3)->diff);
        // Gradient with respect to bias
        blas_sgemv(CblasTrans, M_, N_, (float)1., nextDiff, bias_multiplier, (float)1., biasDiff);
      }
      // Gradient with respect to bottom data
      if (LARG(transpose)) {
        blas_sgemm(CblasNoTrans, CblasTrans, M_, K_, N_, (float)1., nextDiff, weightData, (float)0., prevDiff);
      } else {
        blas_sgemm(CblasNoTrans, CblasNoTrans, M_, K_, N_, (float)1., nextDiff, weightData, (float)0., prevDiff);
      }
    }
    break;
  }

  FREE(bias_multiplier);
  return 0;
}

#endif
static float* fullconnect_backword(float* prevDiff, const float* weightData, const float* nextDiff, int n, DataSize weightSize) {
  if (1) {
    int transpose = 0;
    int M = n;
    int K = (transpose) ? weightSize.h : weightSize.w;
    int N = (transpose) ? weightSize.w : weightSize.h;
    if (transpose) {
      blas_sgemm(CblasNoTrans, CblasTrans, M, K, N, (float)1., nextDiff, weightData, (float)0., prevDiff);
    } else {
      blas_sgemm(CblasNoTrans, CblasNoTrans, M, K, N, (float)1., nextDiff, weightData, (float)0., prevDiff);
    }
  }
  if (0) {
    int pn, pidx, nc;
    DISPATCH_WORKER_BEGIN(n) {
      for (pn = start; pn < stop; pn++) {
        for (pidx = 0; pidx < weightSize.w; pidx++) {
          const int prevDiffIdx = pn * weightSize.w + pidx;
          for (nc = 0; nc < weightSize.h; nc++) {
            const int weightIdx = nc * weightSize.w + pidx;
            const int nextDiffIdx = pn * weightSize.h + nc;
            prevDiff[prevDiffIdx] += weightData[weightIdx] * nextDiff[nextDiffIdx];
          }
        }
      }
    };
    DISPATCH_WORKER_END;
  }
  return prevDiff;
}
static float* fullconnect_backword_weight(float* weightDiff, const float* prevData, const float* nextDiff, int n, DataSize weightSize) {
  if (1) {
    int transpose = 0;
    int M = n;
    int K = (transpose) ? weightSize.h : weightSize.w;
    int N = (transpose) ? weightSize.w : weightSize.h;
    if (transpose) {
      blas_sgemm(CblasTrans, CblasNoTrans, K, N, M, (float)1., prevData, nextDiff, (float)1., weightDiff);
    }
    else {
      blas_sgemm(CblasTrans, CblasNoTrans, N, K, M, (float)1., nextDiff, prevData, (float)1., weightDiff);
    }
  }
  if (0) {
    int pn, nc, prevData3DIdx;
    for (pn = 0; pn < n; pn++) {
      for (nc = 0; nc < weightSize.h; nc++) {
        const int nextDiffIdx = pn * weightSize.h + nc;
        for (prevData3DIdx = 0; prevData3DIdx < weightSize.w; prevData3DIdx++) {
          const int weightIdx = nc * weightSize.w + prevData3DIdx;
          const int prevDataIdx = pn * weightSize.w + prevData3DIdx;
          weightDiff[weightIdx] += prevData[prevDataIdx] * nextDiff[nextDiffIdx];
        }
      }
    }
  }
  return weightDiff;
}

static float* fullconnect_backword_bias(float* biasDiff, const float* nextDiff, int n, int bias3DSize) {
  if (1) {
    int M_ = n, N_ = bias3DSize;
    float* bias_multiplier = NULL;
    MYREALLOC(bias_multiplier, M_);
    ASSERT(bias_multiplier);
    blas_sset(M_, 1, bias_multiplier);
    blas_sgemv(CblasTrans, M_, N_, (float)1., nextDiff, bias_multiplier, (float)1., biasDiff);
    FREE(bias_multiplier);
  }
  if (0) {
    int nn, biasDiffIdx;
    for (nn = 0; nn < n; nn++) {
      for (biasDiffIdx = 0; biasDiffIdx < bias3DSize; biasDiffIdx++) {
        biasDiff[biasDiffIdx] += 1.0f * nextDiff[nn * bias3DSize + biasDiffIdx];
      }
    }
  }
  return biasDiff;
}

static float* fullconnect_forword(const float* prevData, const float* weightData, const float* biasData, float* nextData, int n, DataSize weightSize)
{
  if (1) {
    int transpose = 0;
    int M = n;
    int K = (transpose) ? weightSize.h : weightSize.w;
    int N = (transpose) ? weightSize.w : weightSize.h;
    blas_sgemm(CblasNoTrans, (transpose) ? CblasNoTrans : CblasTrans, M, N, K, (float)1., prevData, weightData, (float)0., nextData);
    if (biasData) {
      float* bias_multiplier = NULL;
      MYREALLOC(bias_multiplier, M);
      ASSERT(bias_multiplier);
      blas_sset(M, 1.f, bias_multiplier);
      blas_sgemm(CblasNoTrans, CblasNoTrans, M, N, 1, (float)1., bias_multiplier, biasData, (float)1., nextData);
      FREE(bias_multiplier);
    }
  }
  if (0) {
    int i, j, k;
    int is = weightSize.w, os = weightSize.h;
    if (biasData) {
      for (k = 0; k < n; k++) {
        const float* n_input = prevData + k * is;
        float* n_output = nextData + k * os;
        for (i = 0; i < os; i++) {
          float sum = 0.0f;
          for (j = 0; j < is; j++) {
            sum += n_input[j] * weightData[i * is + j];
          }
          sum += biasData[i];
          n_output[i] = sum;
        }
      }
    }
    else {
      for (k = 0; k < n; k++) {
        const float* n_input = prevData + k * is;
        float* n_output = nextData + k * os;
        for (i = 0; i < os; i++) {
          float sum = 0.0f;
          for (j = 0; j < is; j++) {
            sum += n_input[j] * weightData[i * is + j];
          }
          n_output[i] = sum;
        }
      }
    }
  }
  return nextData;
}

