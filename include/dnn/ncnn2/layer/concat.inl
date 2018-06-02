
struct Concat {
  int axis;
};

static int Concat_load_param(Layer* layer, const char* param) {
  struct Concat* s = (struct Concat*)layer->x;
  char* p = (char*)param;
  str_getint(p, &p, &s->axis, 0);
  return 1;
}

static int Concat_forward(buf_t* bf, const Layer* layer, const img_t* bottom_blobs, img_t* top_blobs) {
  struct Concat* s = (struct Concat*)layer->x;
  int i, j, q, b, dims = pBox_dims(bottom_blobs);

  if (dims == 1) { // axis == 0
    // concat vector
    // total length
    int top_w = 0;

    for (b = 0; b < layer->bottoms_size; b++) {
      const img_t* bottom_blob = bottom_blobs+b;
      top_w += bottom_blob->w;
    }

    img_t* top_blob = top_blobs;
    pBox_create(top_blob, top_w, 1);

    if (pBox_empty(top_blob)) {
      return -100;
    }

    float* outptr = pBox_datap(top_blob);

    for (b = 0; b < layer->bottoms_size; b++) {
      const img_t* bottom_blob = bottom_blobs+b;

      int w = bottom_blob->w;

      const float* ptr = pBox_datap(bottom_blob);

      for (i = 0; i < w; i++) {
        outptr[i] = ptr[i];
      }

      outptr += w;
    }

    return 0;
  }

  if (dims == 2 && s->axis == 0) {
    // concat image
    int w = bottom_blobs[0].w;

    // total height
    int top_h = 0;

    for (b = 0; b < layer->bottoms_size; b++) {
      const img_t* bottom_blob = bottom_blobs+b;
      top_h += bottom_blob->h;
    }

    img_t* top_blob = top_blobs;
    pBox_create(top_blob, w, top_h);

    if (pBox_empty(top_blob)) {
      return -100;
    }

    float* outptr = pBox_datap(top_blob);

    for (b = 0; b < layer->bottoms_size; b++) {
      const img_t* bottom_blob = bottom_blobs+b;

      int size = w * bottom_blob->h;

      const float* ptr = pBox_datap(bottom_blob);

      for (i = 0; i < size; i++) {
        outptr[i] = ptr[i];
      }

      outptr += size;
    }

    return 0;
  }

  if (dims == 2 && s->axis == 1) {
    // interleave image row
    int h = bottom_blobs[0].h;

    // total width
    int top_w = 0;

    for (b = 0; b < layer->bottoms_size; b++) {
      const img_t* bottom_blob = bottom_blobs+b;
      top_w += bottom_blob->w;
    }

    img_t* top_blob = top_blobs;
    pBox_create(top_blob, top_w, h);

    if (pBox_empty(top_blob)) {
      return -100;
    }

    #pragma omp parallel for

    for (i = 0; i < h; i++) {
      float* outptr = pBox_row0(top_blob, i);

      for (b = 0; b < layer->bottoms_size; b++) {
        const img_t* bottom_blob = bottom_blobs+b;
        const float* ptr = pBox_row0(bottom_blob, i);

        for (j = 0; j < bottom_blob->w; j++) {
          outptr[j] = ptr[j];
        }

        outptr += bottom_blob->w;
      }
    }

    return 0;
  }

  if (dims == 3 && s->axis == 0) {
    // concat dim
    int w = bottom_blobs[0].w;
    int h = bottom_blobs[0].h;

    // total channels
    int top_channels = 0;

    for (b = 0; b < layer->bottoms_size; b++) {
      const img_t* bottom_blob = bottom_blobs+b;
      top_channels += pBox_channels(bottom_blob);
    }

    img_t* top_blob = top_blobs;
    pBox_create(top_blob, w, h, top_channels);

    if (pBox_empty(top_blob)) {
      return -100;
    }

    int q = 0;

    for (b = 0; b < layer->bottoms_size; b++) {
      const img_t* bottom_blob = bottom_blobs+b;

      int channels = pBox_channels(bottom_blob);
      int size = pBox_total(bottom_blob);

      const float* ptr = pBox_datap(bottom_blob);
      float* outptr = pBox_channel_p(top_blob, q);

      for (i = 0; i < size; i++) {
        outptr[i] = ptr[i];
      }

      q += channels;
    }

    return 0;
  }

  if (dims == 3 && s->axis == 1) {
    // interleave dim height
    int w = bottom_blobs[0].w;
    int channels = bottom_blobs[0].c;

    // total height
    int top_h = 0;

    for (b = 0; b < layer->bottoms_size; b++) {
      const img_t* bottom_blob = bottom_blobs+b;
      top_h += bottom_blob->h;
    }

    img_t* top_blob = top_blobs;
    pBox_create(top_blob, w, top_h, channels);

    if (pBox_empty(top_blob)) {
      return -100;
    }

    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      float* outptr = pBox_channel_p(top_blob, q);

      for (b = 0; b < layer->bottoms_size; b++) {
        const img_t* bottom_blob = bottom_blobs+b;
        int size = bottom_blob->w * bottom_blob->h;
        const float* ptr = pBox_channel_p(bottom_blob, q);

        for (i = 0; i < size; i++) {
          outptr[i] = ptr[i];
        }
      }
    }

    return 0;
  }

  if (dims == 3 && s->axis == 2) {
    // interleave dim width
    int h = bottom_blobs[0].h;
    int channels = bottom_blobs[0].c;

    // total height
    int top_w = 0;

    for (b = 0; b < layer->bottoms_size; b++) {
      const img_t* bottom_blob = bottom_blobs+b;
      top_w += bottom_blob->w;
    }

    img_t* top_blob = top_blobs;
    pBox_create(top_blob, top_w, h, channels);

    if (pBox_empty(top_blob)) {
      return -100;
    }

    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      float* outptr = pBox_channel_p(top_blob, q);

      for (i = 0; i < h; i++) {
        for (b = 0; b < layer->bottoms_size; b++) {
          const img_t* bottom_blob = bottom_blobs+b;
          const float* ptr = pBox_crow(bottom_blob, q, i);

          for (j = 0; j < bottom_blob->w; j++) {
            outptr[j] = ptr[j];
          }

          outptr += bottom_blob->w;
        }
      }
    }

    return 0;
  }

  return 0;
}
