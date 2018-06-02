
struct ELU : public Layer
{
public:
  ELU();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const img_t* bottom_blob, img_t* top_blob) const;

  virtual int forward_inplace(img_t* bottom_top_blob) const;

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

int ELU::forward(const img_t* bottom_blob, img_t* top_blob) const
{
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = pBox_channels(bottom_blob);
  int size = w * h;

  top_blob->create(w, h, channels);

  if (top_blob->empty()) {
    return -100;
  }

  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    const float* ptr = pBox_channel_p(bottom_blob, q);
    float* outptr = pBox_channel_p(top_blob, q);

    for (int i = 0; i < size; i++) {
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

int ELU::forward_inplace(img_t* bottom_top_blob) const
{
  int w = bottom_top_blob->w;
  int h = bottom_top_blob->h;
  int channels = pBox_channels(bottom_top_blob);
  int size = w * h;

  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    float* ptr = pBox_channel_p(bottom_top_blob, q);

    for (int i = 0; i < size; i++) {
      if (ptr[i] < 0.f) {
        ptr[i] = alpha * (exp(ptr[i]) - 1.f);
      }
    }
  }

  return 0;
}

