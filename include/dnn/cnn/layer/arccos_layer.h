
static Dtype ArccosLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  int i, count = Data_count(prev->size);
  switch (op) {
  case Op_Forward:
    next->size = prev->size;
    {
      const float* prevData = getData(prev);
      float* nextData = setDataZero(next);
      for (i = 0; i < count; ++i) {
        nextData[i] = acosf(prevData[i]);
      }
    }
    break;
  case Op_Backward:
    {
      const float* prevData = getData(prev);
      const float* nextDiff = getDiff(next);
      float* prevDiff = setDiff(prev);
      for (i = 0; i < count; ++i) {
        Dtype fixed_in_data = MIN(prevData[i], (Dtype)(1.0) - (Dtype)(0.01));
        prevDiff[i] = nextDiff[i] * -1 / sqrtf(1.0f - fixed_in_data * fixed_in_data);
      }
    }
    break;
  default:
    break;
  }
}
