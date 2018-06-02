

struct Dropout
{
  float scale;
};

static int Dropout_load_param(Layer* layer, const char* param) {
  struct Dropout* s = (struct Dropout*)layer->x;
  char* p = (char*)param;
  str_getfloat(p, &p, &s->scale, 1.f);
  return 1;
}

static int Dropout_forward(buf_t* bf, const Layer* layer, const img_t* bottom_blob, img_t* top_blob) {
  struct Dropout* s = (struct Dropout*)layer->x;
  if (s->scale == 1.f) {
    if (top_blob!=bottom_blob) {
      imclone2(bottom_blob, top_blob);
    }
    return 0;
  }

  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int q, i, channels = pBox_channels(bottom_blob);
  int size = w * h;

  if (top_blob!=bottom_blob) {
    pBox_create(top_blob, w, h, channels);

    if (imempty(top_blob)) {
      return -100;
    }
  }

  #pragma omp parallel for
  for (q = 0; q < channels; q++) {
    const float* ptr = pBox_channel_p(bottom_blob, q);
    float* outptr = pBox_channel_p(top_blob, q);

    for (i = 0; i < size; i++) {
      outptr[i] = ptr[i] * s->scale;
    }
  }

  return 0;
}

static int Dropout_forward_inplace(buf_t* bf, const Layer* layer, img_t* bottom_top_blob)
{
  return Dropout_forward(bf, layer, bottom_top_blob, bottom_top_blob);
}

