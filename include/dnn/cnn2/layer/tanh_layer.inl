
static int TanHLayer(enum LayerOp op, struct NetWork* net, struct Layer* layer) {
  Blob* bottom_blob = LBLOB(0);
  Blob* top_blob = LBLOB(1);
  Dtype negative_slope = FARG(slope);
  int i;
  switch (op) {
  case LO_Init:
    break;
  case LO_Forward:
    {
      Blob_ReshapeLike(top_blob->data, bottom_blob->data);
      const Dtype* bottom_data = Blob_datap(bottom_blob->data);
      Dtype* top_data = Blob_datap(top_blob->data);
      const int count = Blob_count(bottom_blob->data);
      for (i = 0; i < count; ++i) {
        top_data[i] = tanh(bottom_data[i]);
      }
    }
    break;
  case LO_Backward:
    {
      Blob_ReshapeLike(bottom_blob->diff, bottom_blob->data);
      const Dtype* bottom_data = Blob_datap(bottom_blob->data);
      const Dtype* top_data = Blob_datap(top_blob->data);
      const Dtype* top_diff = Blob_datap(top_blob->diff);
      Dtype* bottom_diff = Blob_datap(bottom_blob->diff);
      const int count = Blob_count(bottom_blob->data);
      for (i = 0; i < count; ++i) {
        Dtype tanhx = top_data[i];
        bottom_diff[i] = top_diff[i] * (1 - tanhx * tanhx);
      }
    }
    break;
  }
  return 0;
}
