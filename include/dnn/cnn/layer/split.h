
static int Split_load_param(Layer* layer, const char* param) {
  return 0;
}
static int Split(enum LayerOp op, struct Net* net, struct Layer* layer)
{
  const Blob* prev = net->blobs[LARG(bottom0)].im;
  Blob* next = net->blobs[LARG(top0)].im;
  int i;
  for (i = 0; i < layer->tops_size; i++) {
    imclone2(bf, prev, next+i);
  }
  return 0;
}

