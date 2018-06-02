

struct Flatten : public Layer
{
};


Flatten::Flatten()
{
  one_blob_only = true;
  support_inplace = false;
}

int Flatten::forward(const Blob* prev, Blob* next) const
{
  int w = prevSize.w;
  int h = prevSize.h;
  int channels = prevSize.c;
  int size = w * h;

  next->create(size * channels);

  if (next->empty()) {
    return -100;
  }

  #pragma omp parallel for

  for (q = 0; q < channels; q++) {
    const float* ptr = prevData + q * Data_2DSize(prevSize);
    float* outptr = nextData + size * q;

    for (i = 0; i < size; i++) {
      outptr[i] = ptr[i];
    }
  }

  return 0;
}

} // namespace ncnn
