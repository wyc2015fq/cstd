
static int Split_load_param(Layer* layer, const char* param) {
  return 0;
}
static int Split(enum LayerOp op, struct NetWork* net, struct Layer* layer)
{
  const Blob* bottom_blob = net->blobs[LARG(bottom0)].im;
  Blob* top_blob = net->blobs[LARG(top0)].im;
  int i;
  for (i = 0; i < layer->tops_size; i++) {
    imclone2(bf, bottom_blob, top_blob+i);
  }
  return 0;
}

