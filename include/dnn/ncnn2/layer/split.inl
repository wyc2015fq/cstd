
static int Split_load_param(Layer* layer, const char* param) {
  return 0;
}
static int Split_forward(buf_t* bf, const Layer* layer, const img_t* bottom_blob, img_t* top_blob) {
  int i;
  for (i = 0; i < layer->tops_size; i++) {
    bf_imclone2(bf, bottom_blob, top_blob+i);
  }
  return 0;
}

