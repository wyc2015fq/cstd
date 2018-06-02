
CC_INLINE Dtype sigmoid(Dtype x) {
  return 0.5 * tanh(0.5 * x) + 0.5;
}

static int SigmoidLayer(enum LayerOp op, const struct Layer* layer, struct Net* net) {
  Blob* bottom_blob = LBLOB(0);
  Blob* top_blob = LBLOB(1);
  Dtype negative_slope = FARG(slope);
  int i;
  switch (op) {
  case LO_Init:
    break;
  case LO_Forward:
    {
      pBox_ReshapeLike(top_blob->data, bottom_blob->data);
      const Dtype* bottom_data = pBox_datap(bottom_blob->data);
      Dtype* top_data = pBox_datap(top_blob->data);
      const int count = pBox_count(bottom_blob->data);
      for (i = 0; i < count; ++i) {
        top_data[i] = sigmoid(bottom_data[i]);
      }
    }
    break;
  case LO_Backward:
    {
      pBox_ReshapeLike(bottom_blob->diff, bottom_blob->data);
      const Dtype* bottom_data = pBox_datap(bottom_blob->data);
      const Dtype* top_data = pBox_datap(top_blob->data);
      const Dtype* top_diff = pBox_datap(top_blob->diff);
      Dtype* bottom_diff = pBox_datap(bottom_blob->diff);
      const int count = pBox_count(bottom_blob->data);
      for (i = 0; i < count; ++i) {
        const Dtype sigmoid_x = top_data[i];
        bottom_diff[i] = top_diff[i] * sigmoid_x * (1. - sigmoid_x);
      }
    }
    break;
  }
  return 0;
}
