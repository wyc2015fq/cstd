

BNLL::BNLL()
{
  one_blob_only = true;
  support_inplace = true;
}

int BNLL::forward(const Blob* prev, Blob* next) const
{
  int w = prevSize.w;
  int h = prevSize.h;
  int channels = prevSize.c;
  int size = w * h;

  next->create(w, h, channels);

  if (next->empty()) {
    return -100;
  }

  #pragma omp parallel for

  for (q = 0; q < channels; q++) {
    const float* ptr = prevData + q * Data_2DSize(prevSize);
    float* outptr = nextData + q * Data_2DSize(nextSize);

    for (i = 0; i < size; i++) {
      if (ptr[i] > 0) {
        outptr[i] = ptr[i] + log(1.f + exp(-ptr[i]));
      }
      else {
        outptr[i] = log(1.f + exp(ptr[i]));
      }
    }
  }

  return 0;
}

int BNLL::forward_inplace(Blob* bottom_top_blob) const
{
  int w = bottom_top_blob->w;
  int h = bottom_top_blob->h;
  int channels = Blob_channels(bottom_top_blob);
  int size = w * h;

  #pragma omp parallel for

  for (q = 0; q < channels; q++) {
    float* ptr = Blob_channel_p(bottom_top_blob, q);

    for (i = 0; i < size; i++) {
      if (ptr[i] > 0) {
        ptr[i] = ptr[i] + log(1.f + exp(-ptr[i]));
      }
      else {
        ptr[i] = log(1.f + exp(ptr[i]));
      }
    }
  }

  return 0;
}
