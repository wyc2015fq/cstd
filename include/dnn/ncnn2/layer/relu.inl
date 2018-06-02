
struct ReLU {
  float slope;
};

static int ReLU_load_param(Layer* layer, const char* param) {
  struct ReLU* s = (struct ReLU*)layer->x;
  char* p = (char*)param;
  str_getfloat(p, &p, &s->slope, 0);
  return 0;
}

static int ReLU_forward(buf_t* bf, const Layer* layer, const img_t* bottom_blob, img_t* top_blob) {
  struct ReLU* s = (struct ReLU*)layer->x;
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int q, i, channels = pBox_channels(bottom_blob);
  int size = w * h;

  pBox_create(top_blob, w, h, channels, 1);

  if (pBox_empty(top_blob)) {
    return -100;
  }

  if (s->slope == 0.f) {
    #pragma omp parallel for
    for (q = 0; q < channels; q++) {
      const float* ptr = pBox_channel_p(bottom_blob, q);
      float* outptr = pBox_channel_p(top_blob, q);

      for (i = 0; i < size; i++) {
        if (ptr[i] < 0) {
          outptr[i] = 0;
        }
        else {
          outptr[i] = ptr[i];
        }
      }
    }
  }
  else {
    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      const float* ptr = pBox_channel_p(bottom_blob, q);
      float* outptr = pBox_channel_p(top_blob, q);

      for (i = 0; i < size; i++) {
        if (ptr[i] < 0) {
          outptr[i] = ptr[i] * s->slope;
        }
        else {
          outptr[i] = ptr[i];
        }
      }
    }
  }

  return 0;
}

static int ReLU_forward_inplace(buf_t* bf, const Layer* layer, img_t* bottom_top_blob)
{
  struct ReLU* s = (struct ReLU*)layer->x;
  int w = bottom_top_blob->w;
  int h = bottom_top_blob->h;
  int q, i, channels = pBox_channels(bottom_top_blob);
  int size = w * h;

  if (s->slope == 0.f) {
    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      float* ptr = pBox_channel_p(bottom_top_blob, q);

      for (i = 0; i < size; i++) {
        if (ptr[i] < 0) {
          ptr[i] = 0;
        }
      }
    }
  }
  else {
    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      float* ptr = pBox_channel_p(bottom_top_blob, q);

      for (i = 0; i < size; i++) {
        if (ptr[i] < 0) {
          ptr[i] *= s->slope;
        }
      }
    }
  }

  return 0;
}


