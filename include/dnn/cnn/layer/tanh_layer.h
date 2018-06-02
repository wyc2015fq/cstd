
#define net_tanh(net, input) net_null_1_1(net, input, TanhLayer, "tanh")
static Dtype TanhLayer(enum LayerOp op, struct Net* net, struct Layer* layer)
{
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  switch (op) {
  case Op_Forward: 
    next->size = prev->size;
    {
      const float* prevData = getData(prev);
      float* nextData = setDataZero(next);
      if (1) {
        DISPATCH_WORKER_BEGIN(prev->size.n) {
          const int offset = start * Data_3DSize(prev->size);
          const int total_size = (stop - start) * Data_3DSize(prev->size);
          blas_stanh(prevData + offset, nextData + offset, total_size);
        };
        DISPATCH_WORKER_END;
      }
      if (0) {
        int i, count = Data_count(prev->size);
        for (i = 0; i < count; ++i) {
          nextData[i] = tanh(prevData[i]);
        }
      }
    }
  break;
  case Op_Backward: {
    const float* nextData = getData(next);
    float* prevDiff = setDiffZero(prev);
    const float* nextDiff = setDiffZero(next);
    easyAssert(Data_4DEqu(prev->size, next->size), "size must be equal!");
    easyAssert(Data_4DEqu(prev->size, prev->size), "size of prevDiff and size of prev must be equals");
    //update prevDiff
    if (1) {
    DISPATCH_WORKER_BEGIN(prev->size.n) {
      const int offset = start * Data_3DSize(prev->size);
      const int total_size = (stop - start) * Data_3DSize(prev->size);
      //calculate current inner diff
      df_tanh(nextData + offset, prevDiff + offset, total_size);
      //multiply next diff
      mul_inplace(prevDiff + offset, nextDiff + offset, total_size);
    };
    DISPATCH_WORKER_END;
    }
    if (0) {
      int i, count = Data_count(prev->size);
      for (i = 0; i < count; ++i) {
        Dtype tanhx = nextData[i];
        prevDiff[i] = nextDiff[i] * (1 - tanhx * tanhx);
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
