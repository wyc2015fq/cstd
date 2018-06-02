
static int AbsVal_load_param(Layer* layer, const char* param) {
  return 0;
}

static int AbsVal_forward(buf_t* bf, const Layer* layer, const img_t* bottom_blob, img_t* top_blob)
{
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int q, i, channels = pBox_channels(bottom_blob);
  int size = w * h;

  bf_pBoxsetsize(bf, top_blob, h, w, channels, 1);

  #pragma omp parallel for
  for (q = 0; q < channels; q++) {
    const float* ptr = pBox_channel_p(bottom_blob, q);
    float* outptr = pBox_channel_p(top_blob, q);

    for (i = 0; i < size; i++) {
      if (ptr[i] < 0) {
        outptr[i] = -ptr[i];
      }
      else {
        outptr[i] = ptr[i];
      }
    }
  }

  return 0;
}

static int AbsVal_forward_inplace(buf_t* bf, const Layer* layer, img_t* bottom_top_blob)
{
  int w = bottom_top_blob->w;
  int h = bottom_top_blob->h;
  int q, i, channels = pBox_channels(bottom_top_blob);
  int size = w * h;

  #pragma omp parallel for
  for (q = 0; q < channels; q++) {
    float* ptr = pBox_channel_p(bottom_top_blob, q);

    for (i = 0; i < size; i++) {
      if (ptr[i] < 0) {
        ptr[i] = -ptr[i];
      }
    }
  }

  return 0;
}

