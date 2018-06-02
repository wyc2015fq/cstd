
static int Dropout(enum LayerOp op, const struct Layer* layer, struct Net* net)
{
  const img_t* bottom_data = LBLOB(0)->data;
  img_t* top_blob = net->blobs[layer->blobs[1]].data;
  float scale = FARG(scale);

  if (scale == 1.f) {
    if (top_blob!=bottom_data) {
      imclone2(bottom_data, top_blob);
    }
    return 0;
  }

  int w = bottom_data->w;
  int h = bottom_data->h;
  int q, i, channels = pBox_channels(bottom_data);
  int size = w * h;

  if (top_blob!=bottom_data) {
    pBox_create(top_blob, w, h, channels);

    if (imempty(top_blob)) {
      return -100;
    }
  }

  #pragma omp parallel for
  for (q = 0; q < channels; q++) {
    const float* ptr = pBox_channel_p(bottom_data, q);
    float* outptr = pBox_channel_p(top_blob, q);

    for (i = 0; i < size; i++) {
      outptr[i] = ptr[i] * scale;
    }
  }

  return 0;
}


