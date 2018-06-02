
static int Split_load_param(Layer* layer, const char* param) {
  return 0;
}
static int Split(enum LayerOp op, const struct Layer* layer, struct Net* net)
{
  const img_t* bottom_blob = net->blobs[LARG(bottom0)].im;
  img_t* top_blob = net->blobs[LARG(top0)].im;
  int i;
  for (i = 0; i < layer->tops_size; i++) {
    imclone2(bf, bottom_blob, top_blob+i);
  }
  return 0;
}

