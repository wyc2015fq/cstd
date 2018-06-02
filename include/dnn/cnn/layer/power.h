

struct Power : public Layer
{
public:
  Power();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const Blob* prev, Blob* next) const;

  virtual int forward_inplace(Blob* bottom_top_blob) const;

public:
  float power;
  float scale;
  float shift;
};

Power::Power()
{
  one_blob_only = true;
  support_inplace = true;
}

int Power::load_param(const ParamDict& pd)
{
  power = pd.get(0, 1.f);
  scale = pd.get(1, 1.f);
  shift = pd.get(2, 0.f);

  return 0;
}

int Power::forward(const Blob* prev, Blob* next) const
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
      outptr[i] = pow((shift + ptr[i] * scale), power);
    }
  }

  return 0;
}

int Power::forward_inplace(Blob* bottom_top_blob) const
{
  int w = bottom_top_blob->w;
  int h = bottom_top_blob->h;
  int channels = Blob_channels(bottom_top_blob);
  int size = w * h;

  #pragma omp parallel for

  for (q = 0; q < channels; q++) {
    float* ptr = Blob_channel_p(bottom_top_blob, q);

    for (i = 0; i < size; i++) {
      ptr[i] = pow((shift + ptr[i] * scale), power);
    }
  }

  return 0;
}
