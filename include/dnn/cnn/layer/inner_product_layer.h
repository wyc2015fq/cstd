
static int innerProduct(const float* prevData, const float* weightData, const float* biasData, float* nextData, DataSize weightSize)
{
  int num_output = weightSize.n;
  int channels = weightSize.c;
  int p, q, size = weightSize.w;
  const float* weight_data_ptr = weightData;
  int nextStep = weightSize.h;
  int prevStep = weightSize.w;
  //ASSERT(weight->size.h == Data_3DSize(next->size));
  //ASSERT(weight->size.w == Data_3DSize(prev->size));
  //int M = n;
  //int K = (transpose) ? weightSize.h : weightSize.w;
  //int N = (transpose) ? weightSize.w : weightSize.h;
  //blas_sgemm(CblasNoTrans, CblasNoTrans, M, K, N, (float)1., nextDiff, weightData, (float)0., prevDiff);

  // num_output

  if (0)  {
#pragma omp parallel for
    for (p = 0; p < num_output; p++) {
      float* outptr = nextData + p * nextStep;
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
        const float* m = prevData + q * prevStep;
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
  }

  if (1) {
    int i, dim = size * channels;
#pragma omp parallel for
    for (p = 0; p < num_output; p++) {
      const float* w = weight_data_ptr + dim * p;
      const float* m = prevData;
      float sum = 0.f;
      if (biasData) {
        sum = biasData[p];
      }
      // channels
      for (i = 0; i < dim; i++) {
        sum += m[i] * w[i];
      }
      nextData[p] = sum;
    }
  }

  return 0;
}

#if 0
static Dtype InnerProductLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  Blob* prev = IBLOB(0);
  Blob* next = OBLOB(0);
  Blob* weight = WBLOB(0);
  Blob* bias = layer->w_size>1 ? WBLOB(1) : NULL;
  DataSize weightSize = weight->size;
  int M = prev->size.n;
  int K = IARG(transpose) ? weightSize.h : weightSize.w;
  int N = IARG(transpose) ? weightSize.w : weightSize.h;
  ASSERT(K==Data_3DSize(prev->size));
  // IARG(transpose)=0;  ///< if true, assume transposed weights
  float* bias_multiplier = NULL;
  int num_output = IARG(num_output);

  // Set up the bias multiplier
  if (bias) {
    MYREALLOC(bias_multiplier, M);
    ASSERT(bias_multiplier);
    blas_sset(M, (1), bias_multiplier);
  }

  switch (op) {
  case Op_Forward:
    next->size = dataSize(prev->size.n, num_output, 1, 1);
    {
      const float* prevData = prevData;
      float* nextData = nextData;
      const float* weightData = Blob_datap(LBLOB(2)->data);

      blas_sgemm(CblasNoTrans, IARG(transpose) ? CblasNoTrans : CblasTrans, M, N, K, (float)1.,
        prevData, weightData, (float)0., nextData);
      if (IARG(bias_term)) {
        const float* biasData = Blob_datap(WBLOB(1)->data);
        blas_sgemm(CblasNoTrans, CblasNoTrans, M, N, 1, (float)1., bias_multiplier, biasData, (float)1., nextData);
      }
  //innerProduct(prevData, weightData, biasData, nextData, num_output, WBLOB(0)->size);
    }
    break;
  case Op_Backward:
    {
      Blob_ReshapeLike(prev->diff, prevData);
      const float* weightData = Blob_datap(LBLOB(2)->data);
      float* weightDiff = Blob_datap(LBLOB(2)->diff);
      const float* nextDiff = Blob_datap(next->diff);
      const float* prevData = prevData;
      float* prevDiff = Blob_datap(prev->diff);
      if (IARG(transpose)) {
        blas_sgemm(CblasTrans, CblasNoTrans, K, N, M, (float)1., prevData, nextDiff, (float)1., weightDiff);
      }
      else {
        blas_sgemm(CblasTrans, CblasNoTrans, N, K, M, (float)1., nextDiff, prevData, (float)1., weightDiff);
      }
      
      if (IARG(bias_term)) {
        float* biasDiff = Blob_datap(LBLOB(3)->diff);
        // Gradient with respect to bias
        blas_sgemv(CblasTrans, M, N, (float)1., nextDiff, bias_multiplier, (float)1., biasDiff);
      }
      // Gradient with respect to bottom data
      if (IARG(transpose)) {
        blas_sgemm(CblasNoTrans, CblasTrans, M, K, N, (float)1., nextDiff, weightData, (float)0., prevDiff);
      } else {
        blas_sgemm(CblasNoTrans, CblasNoTrans, M, K, N, (float)1., nextDiff, weightData, (float)0., prevDiff);
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
    int M = n, N = bias3DSize;
    float* bias_multiplier = NULL;
    MYREALLOC(bias_multiplier, M);
    ASSERT(bias_multiplier);
    blas_sset(M, 1, bias_multiplier);
    blas_sgemv(CblasTrans, M, N, (float)1., nextDiff, bias_multiplier, (float)1., biasDiff);
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

static Dtype FullconnectLayer(enum LayerOp op, struct Net* net, struct Layer* layer)
{
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  struct Blob* weight = WBLOB(0);
  struct Blob* bias = layer->w_size>1 ? WBLOB(1) : NULL;
  int num_output = IARG(num_output);
  switch (op) {
  case Op_Forward:
    next->size = dataSize(prev->size.n, num_output, 1, 1);
    {
      const float* prevData = getData(prev);
      const float* weightData = getData(weight);
      const float* biasData = bias ? getData(bias) : NULL;
      float* nextData = setDataZero(next);
      ASSERT(weight->size.h == Data_3DSize(next->size));
      ASSERT(weight->size.w == Data_3DSize(prev->size));
      DISPATCH_WORKER_BEGIN(prev->size.n) {
        fullconnect_forword(prevData + start * Data_3DSize(prev->size), weightData, biasData, nextData + start * Data_3DSize(next->size), stop - start, weight->size);
      };
      DISPATCH_WORKER_END;
    }
  break;
  case Op_Backward:
    {
      const float* prevData = getData(prev);
      const float* nextData = getData(next);
      const float* nextDiff = getDiff(next);
      const float* weightData = getData(weight);
      float* prevDiff = setDiffZero(prev);
      ASSERT(weight->size.h == Data_3DSize(next->size));
      ASSERT(weight->size.w == Data_3DSize(prev->size));
      easyAssert(next->size.w == 1 && next->size.h == 1, "use channel only!");
      easyAssert(Data_total(weight->size) == Data_3DSize(prev->size) * Data_3DSize(next->size), "weight size is invalidate!");
      //update prevDiff
      //calculate current inner diff && multiply next diff
      //update this layer's param
      fullconnect_backword(prevDiff, weightData, nextDiff, next->size.n, weight->size);
      //get weight gradient
      if (weight) {
        float* weightDiff = setDiffZero(weight);
        fullconnect_backword_weight(weightDiff, prevData, nextDiff, next->size.n, weight->size);
        //div by batch size
        div_inplace(weightDiff, (float)next->size.n, Data_total(weight->size));
        //double diff_dot = blas_sdot(Data_total(weight->size), weight->diff, weight->diff)/Data_total(weight->size);
        //printf("%10s diff_dot=%f\n", weight->name, Blob_diffDot(weight));
        //printf("%10s diff_dot=%f\n", next->name, Blob_diffDot(next));
        //minmax_norm_inplace(weightDiff, Data_total(weight->size));
      }
      //update bias
      if (bias) {
        float* biasDiff = setDiffZero(bias);
        easyAssert(Data_total(bias->size) == Data_3DSize(next->size), "bias size is invalidate!");
        fullconnect_backword_bias(biasDiff, nextDiff, next->size.n, Data_3DSize(bias->size));
        //div by batch size
        div_inplace(biasDiff, (float)next->size.n, Data_total(bias->size));
        //printf("%10s diff_dot=%f\n", bias->name, Blob_diffDot(bias));
      }
    }
    break;
  default:
    break;
  }
  return 0;
}
static int net_fc(struct Net* net, int input, int* w, int* b, int num_output)
{
  DataSize prevSize, weightSize;
  const char* type = "fc";
  int enabledBias = b != NULL;
  struct Layer* layer = net_addLayer(net, 1, 1, 1 + enabledBias, FullconnectLayer, type);
  IARG(num_output) = num_output;
  LIBLOB(0) = input;
  prevSize = IBLOB(0)->size;
  weightSize = dataSize(1, 1, num_output, Data_3DSize(prevSize));
  LWBLOB(0) = *w = net_addBlob(net, weightSize, type, "w");
  //Blob_init_normal(net, *w, 0.0f, 0.1);
  Blob_init_xavier(BLOB(*w));
  if (b) {
    LWBLOB(1) = *b = net_addBlob(net, dataSize(1, num_output, 1, 1), type, "b");
    Blob_init_const(BLOB(*b), 0.f);
  }
  return LOBLOB(0) = net_addBlob(net, dataSize(prevSize.n, num_output, 1, 1), type, "o");
}

static void matMulv(const float* A, const float* B, float* C, int astep, int ai, int M, int N)
{
  int i, j;

  for (i = 0; i < M; ++i) {
    float s = 0;

    for (j = 0; j < N; ++j) {
      s += A[i * astep + j * ai] * B[j];
    }

    C[i] = s;
  }
}

static pBox* fullconnect(const Weight* weight, const pBox* box, pBox* outpBox)
{
  const Dtype* pIn;
  Dtype* pOut;
  int inch = pBox_channels(box);
  int instep = box->h * box->w * inch;
  int lastChannel = box->h * box->w * inch;
  pBox tmp[1] = {0};
  if (box==outpBox) {
    box = pBox_clone(box, tmp);
  }
  pBoxsetsize(outpBox, 1, 1, weight->size[0].n, box->size.n);
  if (NULL==outpBox->data) {
    return 0;
  }
#ifdef HAVE_OPENCL1

  if (used_gpu) {
    tic;
    static const char* matMulv_progstr =
        "__kernel void matMulv(__global float* A, "
        "        __global float* B,  "
        "        __global float* C, int astep, int ai, int M, int N)  "
        "{"
        "    int i = get_global_id(0);"
        "    int j = 0;"
        "    float s = 0;"
        "    for (j=0; j<N; ++j) {"
        "      s += A[i*astep+j*ai]*B[j];"
        "    }"
        "    C[i] = s;"
        "}"
        ;
    umem_setu(box->data);
    umem_setu(weight->data);
    outpBox->data->modifyu = 1;
    int astep = weight->lastChannel, ai = 1, M = weight->size[0].n, N = weight->lastChannel;
    size_t _globalsize[] = {weight->size[0].n};
    buf_t args[] = {
      clarg(weight->data->u),
      clarg(box->data->u),
      clarg(outpBox->data->u),
      clarg(astep),
      clarg(ai),
      clarg(M),
      clarg(N),
    };
    static ProgramSource  matMulv_oclsrc(matMulv_progstr);
    matMulv_oclsrc.run("matMulv", countof(_globalsize), _globalsize, NULL, countof(args), args);
    toc;
  }

#endif

  if (1) {
    pIn = pBox_datap(box);
    pOut = pBox_datap(outpBox);
    
    if (0) {
      umem_getu(weight->data);
      umem_getu(box->data);
      SETMODIFYP(outpBox, 1);
      ASSERT(box->size.n == 1);
      matMulv(weight->data->p, pIn, pOut, lastChannel, 1, weight->size[0].n, lastChannel);
      return outpBox;
    }
    
    //#ifdef USE_OPENBLAS
#if 0
    umem_getu(weight->data);
    umem_getu(box->data);
    SETMODIFYP(outpBox, 1);
    ASSERT(box->size.n == 1);
    tic;
    //Y←αAX + βY    β must be 0(zero)
    //               row         no trans         A's row               A'col
    cblas_sgemv(CblasRowMajor, CblasNoTrans, weight->size[0].n, weight->size[0].c, 1, weight->data->p, weight->size[0].c, pIn, 1, 0, pOut, 1);
    addbias(outpBox, weight->data+1);
    toc;
#else
    {
      tic;
      innerProduct(box->data, weight->data[0].p, weight->data[1].p, outpBox->data, weight->size[0]);
      toc;
    }
#endif
  }
  pBox_free(tmp);
  return outpBox;
}
