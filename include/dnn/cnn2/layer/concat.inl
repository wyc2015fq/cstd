
static int Concat(enum LayerOp op, struct NetWork* net, struct Layer* layer)
{
  const Blob* bottom_blob = net->blobs[LARG(bottom0)].im;
  Blob* top_blob = net->blobs[LARG(top0)].im;
  int i, j, q, b, dims = Blob_dims(bottom_blob);
  int axis = LARG(axis);

  if (dims == 1) { // axis == 0
    // concat vector
    // total length
    int top_w = 0;

    for (b = 0; b < layer->bottoms_size; b++) {
      const Blob* bottom_blob = net->blob_mats+layer->blobs[b];
      top_w += bottom_blob->w;
    }

    Blob_create(top_blob, top_w, 1);

    if (Blob_empty(top_blob)) {
      return -100;
    }

    float* outptr = Blob_datap(top_blob);

    for (b = 0; b < layer->bottoms_size; b++) {
      const Blob* bottom_blob = net->blob_mats+layer->blobs[b];

      int w = bottom_blob->w;

      const float* ptr = Blob_datap(bottom_blob);

      for (i = 0; i < w; i++) {
        outptr[i] = ptr[i];
      }

      outptr += w;
    }

    return 0;
  }

  if (dims == 2 && axis == 0) {
    // concat image
    int w = bottom_blob->w;

    // total height
    int top_h = 0;

    for (b = 0; b < layer->bottoms_size; b++) {
      const Blob* bottom_blob = net->blob_mats+layer->blobs[b];
      top_h += bottom_blob->h;
    }

    Blob_create(top_blob, w, top_h);

    if (Blob_empty(top_blob)) {
      return -100;
    }

    float* outptr = Blob_datap(top_blob);

    for (b = 0; b < layer->bottoms_size; b++) {
      const Blob* bottom_blob = net->blob_mats+layer->blobs[b];

      int size = w * bottom_blob->h;

      const float* ptr = Blob_datap(bottom_blob);

      for (i = 0; i < size; i++) {
        outptr[i] = ptr[i];
      }

      outptr += size;
    }

    return 0;
  }

  if (dims == 2 && axis == 1) {
    // interleave image row
    int h = bottom_blob->h;

    // total width
    int top_w = 0;

    for (b = 0; b < layer->bottoms_size; b++) {
      const Blob* bottom_blob = net->blob_mats+layer->blobs[b];
      top_w += bottom_blob->w;
    }

    Blob_create(top_blob, top_w, h);

    if (Blob_empty(top_blob)) {
      return -100;
    }

    #pragma omp parallel for

    for (i = 0; i < h; i++) {
      float* outptr = Blob_row0(top_blob, i);

      for (b = 0; b < layer->bottoms_size; b++) {
        const Blob* bottom_blob = net->blob_mats+layer->blobs[b];
        const float* ptr = Blob_row0(bottom_blob, i);

        for (j = 0; j < bottom_blob->w; j++) {
          outptr[j] = ptr[j];
        }

        outptr += bottom_blob->w;
      }
    }

    return 0;
  }

  if (dims == 3 && axis == 0) {
    // concat dim
    int w = bottom_blob->w;
    int h = bottom_blob->h;

    // total channels
    int top_channels = 0;

    for (b = 0; b < layer->bottoms_size; b++) {
      const Blob* bottom_blob = net->blob_mats+layer->blobs[b];
      top_channels += Blob_channels(bottom_blob);
    }

    Blob_create(top_blob, w, h, top_channels);

    if (Blob_empty(top_blob)) {
      return -100;
    }

    int q = 0;

    for (b = 0; b < layer->bottoms_size; b++) {
      const Blob* bottom_blob = net->blob_mats+layer->blobs[b];

      int channels = Blob_channels(bottom_blob);
      int size = Blob_total(bottom_blob);

      const float* ptr = Blob_datap(bottom_blob);
      float* outptr = Blob_channel_p(top_blob, q);

      for (i = 0; i < size; i++) {
        outptr[i] = ptr[i];
      }

      q += channels;
    }

    return 0;
  }

  if (dims == 3 && axis == 1) {
    // interleave dim height
    int w = bottom_blob->w;
    int channels = bottom_blob->c;

    // total height
    int top_h = 0;

    for (b = 0; b < layer->bottoms_size; b++) {
      const Blob* bottom_blob = net->blob_mats+layer->blobs[b];
      top_h += bottom_blob->h;
    }

    Blob_create(top_blob, w, top_h, channels);

    if (Blob_empty(top_blob)) {
      return -100;
    }

    #pragma omp parallel for
    for (q = 0; q < channels; q++) {
      float* outptr = Blob_channel_p(top_blob, q);

      for (b = 0; b < layer->bottoms_size; b++) {
        const Blob* bottom_blob = net->blob_mats+layer->blobs[b];
        int size = bottom_blob->w * bottom_blob->h;
        const float* ptr = Blob_channel_p(bottom_blob, q);

        for (i = 0; i < size; i++) {
          outptr[i] = ptr[i];
        }
      }
    }

    return 0;
  }

  if (dims == 3 && axis == 2) {
    // interleave dim width
    int h = bottom_blob->h;
    int channels = bottom_blob->c;

    // total height
    int top_w = 0;

    for (b = 0; b < layer->bottoms_size; b++) {
      const Blob* bottom_blob = net->blob_mats+layer->blobs[b];
      top_w += bottom_blob->w;
    }

    Blob_create(top_blob, top_w, h, channels);

    if (Blob_empty(top_blob)) {
      return -100;
    }

    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      float* outptr = Blob_channel_p(top_blob, q);

      for (i = 0; i < h; i++) {
        for (b = 0; b < layer->bottoms_size; b++) {
          const Blob* bottom_blob = net->blob_mats+layer->blobs[b];
          const float* ptr = Blob_crow(bottom_blob, q, i);

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
