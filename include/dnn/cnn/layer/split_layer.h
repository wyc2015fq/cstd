
/**
 * @brief Creates a "split" path in the network by copying the bottom Blob
 *        into multiple top Blob%s to be used by multiple consuming layers.
 *
 * TODO(dox): thorough documentation for Forward, Backward, and proto params.
 */
static Dtype SplitLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  int i, count = Data_count(prev->size);
  switch (op) {
  case Op_Forward:
    next->size = prev->size;
    {
      const float* prevData = getData(prev);
      for (i = 0; i < layer->o_size; ++i) {
        float* nextData = setDataZero(OBLOB(i));
        blas_scopy(count, prevData, nextData);
      }
    }
    break;
  case Op_Backward:
    {
      const float* prevData = getData(prev);
      const float* nextDiff = getDiff(next);
      float* prevDiff = setDiff(prev);
      if (layer->o_size == 1) {
        blas_scopy(count, nextDiff, prevDiff);
        return 0;
      }
      blas_sadd(count, nextDiff, OBLOB(1)->diff, prevDiff);
      // Add remaining top blob diffs.
      for (i = 2; i < layer->o_size; ++i) {
        nextDiff = OBLOB(i)->diff;
        blas_saxpy(count, (Dtype)(1.), nextDiff, prevDiff);
      }
    }
    break;
  default:
    break;
  }
  return 0;
}
