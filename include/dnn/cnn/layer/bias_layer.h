

static int bias_forward(const float* prevData, const float* biasData, float* nextData, DataSize size)
{
  int i, n, nn, nc;
  int outer_dim = size.n;
  int bias_dim = size.c;
  int inner_dim = size.w * size.h;
  if (1) {
    int dim = bias_dim * inner_dim;
    float* bias_multiplier = NULL;
    resetArr(&bias_multiplier, inner_dim);
    blas_sset(inner_dim, (1), bias_multiplier);

    if (prevData != nextData) {
      blas_scopy(Data_count(size), prevData, nextData);
    }
    for (n = 0; n < outer_dim; ++n) {
      blas_sgemm(CblasNoTrans, CblasNoTrans, bias_dim, inner_dim, 1, (1), biasData, bias_multiplier, (1), nextData);
      nextData += dim;
    }
    FREE(bias_multiplier);
  }

  if (0) {
    if (prevData==nextData) {
#pragma omp parallel for
      for (nn = 0; nn < outer_dim; nn++) {
        for (nc = 0; nc < bias_dim; nc++) {
          float* ptr = nextData;
          float bias = biasData[nc];
          
          for (i = 0; i < inner_dim; i++) {
            ptr[i] += bias;
          }
          nextData += inner_dim;
        }
      }
    } else {
#pragma omp parallel for
      for (nn = 0; nn < outer_dim; nn++) {
        for (nc = 0; nc < bias_dim; nc++) {
          const float* ptr = prevData;
          float* outptr = nextData;
          float bias = biasData[nc];
          
          for (i = 0; i < inner_dim; i++) {
            outptr[i] = ptr[i] + bias;
          }
          prevData += inner_dim;
          nextData += inner_dim;
        }
      }
    }
  }
  return 0;
}

static int bias_backward(const float* nextDiff, float* prevDiff, float* biasDiff, DataSize size, bool accum)
{
  int n;
  int outer_dim = size.n;
  int bias_dim = size.c;
  int inner_dim = size.w * size.h;
  if (1) {
    int dim = bias_dim * inner_dim;
    float* bias_multiplier = NULL;
    resetArr(&bias_multiplier, inner_dim);
    blas_sset(inner_dim, (1), bias_multiplier);
    if (prevDiff != nextDiff) {
      blas_scopy(Data_count(size), nextDiff, prevDiff);
    }
    // in-place, we don't need to do anything with the data diff
    for (n = 0; n < outer_dim; ++n) {
      blas_sgemv(CblasNoTrans, bias_dim, inner_dim, (float)(1), nextDiff, bias_multiplier, (float)(accum), biasDiff);
      nextDiff += dim;
      accum = true;
    }
    FREE(bias_multiplier);
  }
  return 0;
}

static Dtype BiasLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  struct Blob* bias = (layer->i_size > 1) ? IBLOB(1) : WBLOB(0);
  switch (op) {
  case Op_Forward:
    next->size = prev->size;
    {
      const float* prevData = getData(prev);
      const float* biasData = getData(bias);
      float* nextData = setDataZero(next);
      bias_forward(prevData, biasData, nextData, prev->size);
    }
    break;
  case Op_Backward:
    {
      const float* nextDiff = getDiff(next);
      float* prevDiff = setDiff(prev);
      float* biasDiff = setDiff(bias);
      bool accum = (layer->i_size == 1);
      bias_backward(nextDiff, prevDiff, biasDiff, prev->size, accum);
    }
    break;
  default:
    break;
  }
  return 0;
}
