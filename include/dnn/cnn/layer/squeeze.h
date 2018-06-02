

struct Squeeze : public Layer
{
public:
  Squeeze();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const Blob* prev, Blob* next) const;

public:
  int squeeze_w;
  int squeeze_h;
  int squeeze_c;
};


Squeeze::Squeeze()
{
  one_blob_only = true;
  support_inplace = false;
}

int Squeeze::load_param(const ParamDict& pd)
{
  squeeze_w = pd.get(0, 0);
  squeeze_h = pd.get(1, 0);
  squeeze_c = pd.get(2, 0);

  return 0;
}

int Squeeze::forward(const Blob* prev, Blob* next) const
{
  int w = prevSize.w;
  int h = prevSize.h;
  int channels = prevSize.c;
  int dims = prev->dims;

  next = prev;

  if (squeeze_c && dims == 3 && channels == 1) {
    if (squeeze_h && h == 1) {
      next = prev->reshape(w);
    }
    else {
      next = prev->reshape(w, h);
    }
  }
  else if (squeeze_h && dims >= 2 && h == 1) {
    if (squeeze_w && w == 1) {
      next = prev->reshape(channels);
    }
    else {
      next = prev->reshape(w, channels);
    }
  }
  else if (squeeze_w && dims >= 1 && w == 1) {
    if (squeeze_h && h == 1) {
      next = prev->reshape(channels);
    }
    else {
      next = prev->reshape(h, channels);
    }
  }

  if (next->empty()) {
    return -100;
  }

  return 0;
}

} // namespace ncnn
