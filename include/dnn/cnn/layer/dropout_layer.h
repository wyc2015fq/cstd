
#if 0
static int dropout_forward(const float* prevData, float* nextData, DataSize size, float scale)
{
  if (0) {
    if (scale == 1.f) {
      if (nextData!=prevData) {
        blas_scopy(Data_count(size), prevData, nextData);
      }
      return 0;
    }
    
    int w = size.w;
    int h = size.h;
    int q, i, channels = size.c;
    int count = w * h;
    
#pragma omp parallel for
    for (q = 0; q < channels; q++) {
      const float* ptr = prevData + q * count;
      float* outptr = nextData + q * count;
      
      for (i = 0; i < count; i++) {
        outptr[i] = ptr[i] * scale;
      }
    }
  }
  return 0;
}

static void DropoutLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  float scale = FARG(scale);
  float threshold = FARG(threshold);
  threshold = BOUND(threshold, 0., 1);
  scale = 1. / (1. - threshold);
  
  switch (op) {
  case Op_Forward:
    next->size = prev->size;
    {
      const float* prevData = getData(prev);
      float* nextData = setDataZero(next);
      unsigned int* mask = (MNARG(phase) == M_Train) ? setBuff(next, 0) : NULL;
      const int count = Data_count(prev->size);
      if (MNARG(phase) == M_Train) {
        // Create random numbers
        blas_srng_bernoulli(count, 1. - threshold, mask);
        for (i = 0; i < count; ++i) {
          nextData[i] = prevData[i] * mask[i] * scale;
        }
      } else {
        blas_scopy(Data_count(prevSize), prevData, nextData);
      }
    }
    break;
  case Op_Backward:
    {
      const Dtype* nextDiff = getDiff(next);
      Dtype* prevDiff = setDiffZero(prev);
      if (MNARG(phase) == M_Train) {
        const unsigned int* mask = rand_vec.cpuData();
        const int count = Data_count(prev->size);
        for (i = 0; i < count; ++i) {
          prevDiff[i] = nextDiff[i] * mask[i] * scale;
        }
      } else {
        blas_scopy(next[0]->count(), nextDiff, prevDiff);
      }
    }
    break;
  default:
    break;
  }
}
#endif

static Dtype DropoutLayer(enum LayerOp op, struct Net* net, struct Layer* layer)
{
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  float rate = FARG(rate);
  float scale = 1./rate;
  int i, j;
  DataSize maskSize = prev->size;
  maskSize.n = 1;
  switch (op) {
  case Op_Forward:
    next->size = prev->size;
    {
      const float* prevData = getData(prev);
      float* nextData = setData(next);
      easyAssert(Data_4DEqu(prev->size, next->size), "size must be equal!");
      //init rand seed
      srand((unsigned int)time(NULL));
      if (MNARG(phase) == M_Train) {
        //fill mask
        float* maskData = reset(&next->bufs[0], maskSize);
        //const_distribution_init(maskData, maskSize Data_total(), 1.0f);
        for (i = 0; i < Data_total(maskSize); i++) {
          maskData[i] = (float)(bernoulli(rate));
        }
        for (i = 0; i < next->size.n; i++) {
          for (j = 0; j < Data_3DSize(next->size); j++) {
            const int dataIdx = i * Data_3DSize(next->size) + j;
            nextData[dataIdx] = prevData[dataIdx] * maskData[j] * scale;
          }
        }
      }
      else {
        for (i = 0; i < Data_total(next->size); i++) {
          nextData[i] = prevData[i];
        }
      }
    }
    break;
  case Op_Backward:
    {
      const float* nextDiff = getDiff(next);
      float* prevDiff = setDiffZero(prev);
      easyAssert(Data_4DEqu(prev->size, next->size), "size must be equal!");
      //update prevDiff
      //calculate current inner diff && multiply next diff
      if (MNARG(phase) == M_Train) {
        const float* maskData = getBuff(next, 0);
        for (i = 0; i < next->size.n; i++) {
          for (j = 0; j < Data_3DSize(next->size); j++) {
            const int dataIdx = i * Data_3DSize(next->size) + j;
            prevDiff[dataIdx] = nextDiff[dataIdx] * maskData[j] * scale;
          }
        }
      } else {
        blas_scopy(Data_total(next->size), nextDiff, prevDiff);
      }
    }
    break;
  default:
    break;
  }
  return 0;
}
static int net_dropout(struct Net* net, int input, float rate)
{
  const char* type = "dropout";
  struct Layer* layer = net_addLayer(net, 1, 1, 0, DropoutLayer, type);
  FARG(rate) = rate;
  LIBLOB(0) = input;
  return LOBLOB(0) = net_addBlob(net, IBLOB(0)->size, type, "o");
}
