
struct ELU : public Layer
{
public:
  ELU();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const Blob* prev, Blob* next) const;

  virtual int forward_inplace(Blob* bottom_top_blob) const;

public:
  float alpha;
};


ELU::ELU()
{
  one_blob_only = true;
  support_inplace = true;
}

int ELU::load_param(const ParamDict& pd)
{
  alpha = pd.get(0, 0.1f);

  return 0;
}

int ELU::forward(const Blob* prev, Blob* next) const
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
      if (ptr[i] < 0.f) {
        outptr[i] = alpha * (exp(ptr[i]) - 1.f);
      }
      else {
        outptr[i] = ptr[i];
      }
    }
  }

  return 0;
}

int ELU::forward_inplace(Blob* bottom_top_blob) const
{
  int w = bottom_top_blob->w;
  int h = bottom_top_blob->h;
  int channels = Blob_channels(bottom_top_blob);
  int size = w * h;

  #pragma omp parallel for

  for (q = 0; q < channels; q++) {
    float* ptr = Blob_channel_p(bottom_top_blob, q);

    for (i = 0; i < size; i++) {
      if (ptr[i] < 0.f) {
        ptr[i] = alpha * (exp(ptr[i]) - 1.f);
      }
    }
  }

  return 0;
}

