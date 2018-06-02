

static int EuclideanLossLayer(enum LayerOp op, const struct Layer* layer, struct Net* net) {
  Blob* bottom_blob = LBLOB(0);
  Blob* bottom_blob1 = LBLOB(1);
  Blob* top_blob = LBLOB(1);
  int count = pBox_count(bottom_blob->data);
  Dtype* diff_ = NULL;

  switch (op) {
  case LO_Init:
    break;
  case LO_Forward:
    {
      pBox_create(top_blob->im, count, 1);
      diff_ = pBox_datap(top_blob->im);
      blas_sub(count,pBox_datap(bottom_blob->data),pBox_datap(bottom_blob1->data), diff_);//diff_ = bottom[0] - bottom[1]
      Dtype dot = blas_dot(count, diff_, diff_);  // dot = ||diff_||^2
      Dtype loss = dot / (bottom_blob->data->f * (Dtype)(2));
      pBox_datap(top_blob->data)[0] = loss;
    }
    break;
  case LO_Backward:
    {
      Dtype* top_diff = pBox_datap(top_blob->diff);
      diff_ = pBox_datap(top_blob->im);
      for (int i = 0; i < 2; ++i) {
        const Dtype sign = (i == 0) ? 1 : -1;
        const Dtype alpha = sign * top_diff[0] / bottom_blob->data->f;
        blas_axpby(count, alpha, diff_, (Dtype)(0), pBox_datap(LBLOB(i)->diff));  // b
      }
    }
    break;
  }
  return 0;
}

