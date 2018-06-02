
#define net_relu(net, input) net_null_1_1(net, input, ReluLayer, "relu")
static Dtype ReluLayer(enum LayerOp op, struct Net* net, struct Layer* layer)
{
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  float slope = FARG(slope);
  int i, count = Data_count(prev->size);
  switch (op) {
  case Op_Forward:
    next->size = prev->size;
    {
      const float* prevData = getData(prev);
      float* nextData = setDataZero(next);
      //f(x)=max(x,0)
      if (0) {
        if (0.f==slope) {
#pragma omp parallel for
          for (i = 0; i < count; i++) {
            const float x = prevData[i];
            nextData[i] = x > 0.f ? x : 0.f;
          }
        } else {
#pragma omp parallel for
          for (i = 0; i < count; i++) {
            const float x = prevData[i];
            nextData[i] = x > 0.f ? x : (x*slope);
          }
        }
      }
      if (1) {
#pragma omp parallel for
        for (i = 0; i < count; ++i) {
          nextData[i] = MAX(prevData[i], (0)) + slope * MIN(prevData[i], (0));
        }
      }
    }
    break;
  case Op_Backward:
    {
      const float* prevData = getData(prev);
      //const float* nextData = getData(next);
      const float* nextDiff = getDiff(next);
      float* prevDiff = setDiffZero(prev);
      easyAssert(Data_4DEqu(prev->size, next->size), "size must be equal!");
      //f'(x)=0(x<=0),1(x>0)
#pragma omp parallel for
      for (i = 0; i < count; ++i) {
        prevDiff[i] = nextDiff[i] * ((prevData[i] > 0) + slope * (prevData[i] <= 0));
      }
    }
    break;
  default:
    break;
  }
  return 0;
}
