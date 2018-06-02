
// value = exp( value - global max value )
// sum all value
// value = value / sum
static int Softmax(enum LayerOp op, const struct Layer* layer, struct Net* net)
{
  const img_t* bottom_data = LBLOB(0)->data;
  img_t* top_blob = net->blobs[layer->blobs[1]].data;
  img_t* bottom_top_blob = top_blob;
  int axis = LARG(axis);
  img_t im[2] = {0};
  img_t *max = im;
  img_t *sum = im+1;
  int i, j, q;

  if (axis == 0) {
    int w = bottom_data->w;
    int h = bottom_data->h;
    int channels = pBox_channels(bottom_data);
    int size = w * h;

    pBox_create(top_blob, w, h, channels);

    if (imempty(top_blob)) {
      return -100;
    }

    pBox_create(max, w, h);

    if (imempty(max)) {
      return -100;
    }

    pBox_fill(max, -FLT_MAX);

    for (q = 0; q < channels; q++) {
      const float* ptr = pBox_channel_p(bottom_data, q);
      float* maxptr = pBox_datap(max);

      for (i = 0; i < size; i++) {
        maxptr[i] = MAX(maxptr[i], ptr[i]);
      }
    }

    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      const float* ptr = pBox_channel_p(bottom_data, q);
      float* outptr = pBox_channel_p(top_blob, q);
      float* maxptr = pBox_datap(max);

      for (i = 0; i < size; i++) {
        outptr[i] = exp(ptr[i] - maxptr[i]);
      }
    }

    pBox_create(sum, w, h);

    if (imempty(sum)) {
      return -100;
    }

    pBox_fill(sum, 0.f);

    for (q = 0; q < channels; q++) {
      const float* outptr = pBox_channel_p(top_blob, q);
      float* sumptr = pBox_datap(sum);

      for (i = 0; i < size; i++) {
        sumptr[i] += outptr[i];
      }
    }

    #pragma omp parallel for
    for (q = 0; q < channels; q++) {
      float* outptr = pBox_channel_p(top_blob, q);
      float* sumptr = pBox_datap(sum);

      for (i = 0; i < size; i++) {
        outptr[i] /= sumptr[i];
      }
    }
  }
  else if (axis == 1) {
    int w = bottom_data->w;
    int h = bottom_data->h;
    int channels = pBox_channels(bottom_data);
    int size = w * h;

    pBox_create(top_blob, w, h, channels);

    if (imempty(top_blob)) {
      return -100;
    }

    pBox_create(max, h, channels);

    if (imempty(max)) {
      return -100;
    }

    pBox_fill(max, -FLT_MAX);
    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      const float* ptr = pBox_channel_p(bottom_data, q);
      float* maxptr = pBox_row0(max, q);

      for (i = 0; i < h; i++) {
        float max = -FLT_MAX;

        for (j = 0; j < w; j++) {
          max = MAX(max, ptr[j]);
        }

        maxptr[i] = max;
        ptr += w;
      }
    }

    #pragma omp parallel for
    for (q = 0; q < channels; q++) {
      const float* ptr = pBox_channel_p(bottom_data, q);
      float* outptr = pBox_channel_p(top_blob, q);
      float* maxptr = pBox_row0(max, q);

      for (i = 0; i < h; i++) {
        float max = maxptr[i];

        for (j = 0; j < w; j++) {
          outptr[j] = exp(ptr[j] - max);
        }

        ptr += w;
        outptr += w;
      }
    }

    pBox_create(sum, h, channels);

    if (imempty(sum)) {
      return -100;
    }

    pBox_fill(sum, 0.f);
    #pragma omp parallel for
    for (q = 0; q < channels; q++) {
      const float* outptr = pBox_channel_p(top_blob, q);
      float* sumptr = pBox_row0(sum, q);

      for (i = 0; i < h; i++) {
        float sum = 0.f;

        for (j = 0; j < w; j++) {
          sum += outptr[j];
        }

        sumptr[i] = sum;
        outptr += w;
      }
    }

    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      float* outptr = pBox_channel_p(top_blob, q);
      float* sumptr = pBox_row0(sum, q);

      for (i = 0; i < h; i++) {
        float sum = sumptr[i];

        for (j = 0; j < w; j++) {
          outptr[j] /= sum;
        }

        outptr += w;
      }
    }

  }
  else if (axis == 2) {
    int w = bottom_data->w;
    int h = bottom_data->h;
    int channels = pBox_channels(bottom_data);
    int size = w * h;

    pBox_create(top_blob, w, h, channels);

    if (imempty(top_blob)) {
      return -100;
    }

    pBox_create(max, w, channels);

    if (imempty(max)) {
      return -100;
    }

    pBox_fill(max, -FLT_MAX);
    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      const float* ptr = pBox_channel_p(bottom_data, q);
      float* maxptr = pBox_row0(max, q);

      for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
          maxptr[j] = MAX(maxptr[j], ptr[j]);
        }

        ptr += w;
      }
    }

    #pragma omp parallel for
    for (q = 0; q < channels; q++) {
      const float* ptr = pBox_channel_p(bottom_data, q);
      float* outptr = pBox_channel_p(top_blob, q);
      float* maxptr = pBox_row0(max, q);

      for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
          outptr[j] = exp(ptr[j] - maxptr[j]);
        }

        ptr += w;
        outptr += w;
      }
    }

    pBox_create(sum, w, channels);

    if (imempty(sum)) {
      return -100;
    }

    pBox_fill(sum, 0.f);
    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      const float* outptr = pBox_channel_p(top_blob, q);
      float* sumptr = pBox_row0(sum, q);

      for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
          sumptr[j] += outptr[j];
        }

        outptr += w;
      }
    }

    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      float* outptr = pBox_channel_p(top_blob, q);
      float* sumptr = pBox_row0(sum, q);

      for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
          outptr[j] /= sumptr[j];
        }

        outptr += w;
      }
    }
  }
  imfrees2(im);
  return 0;
}
