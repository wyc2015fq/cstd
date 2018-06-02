

static int EuclideanLossLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  Blob* prev = IBLOB(0);
  Blob* bottom_blob1 = LBLOB(1);
  Blob* next = OBLOB(0);
  int count = Blob_count(prevData);
  Dtype* diff_ = NULL;

  switch (op) {
  case LO_Init:
    break;
  case Op_Forward:
    {
      Blob_create(next->im, count, 1);
      diff_ = Blob_datap(next->im);
      blas_sub(count,prevData,Blob_datap(bottom_blob1->data), diff_);//diff_ = bottom[0] - bottom[1]
      Dtype dot = blas_dot(count, diff_, diff_);  // dot = ||diff_||^2
      Dtype loss = dot / (prevData->f * (Dtype)(2));
      nextData[0] = loss;
    }
    break;
  case Op_Backward:
    {
      Dtype* nextDiff = Blob_datap(next->diff);
      diff_ = Blob_datap(next->im);
      for (i = 0; i < 2; ++i) {
        const Dtype sign = (i == 0) ? 1 : -1;
        const Dtype alpha = sign * nextDiff[0] / prevData->f;
        blas_axpby(count, alpha, diff_, (Dtype)(0), Blob_datap(LBLOB(i)->diff));  // b
      }
    }
    break;
  }
  return 0;
}

