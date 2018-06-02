

struct Reshape : public Layer
{
public:
  Reshape();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const Blob* prev, Blob* next) const;

private:
  int w;
  int h;
  int c;
  int permute;
  int ndim;
};


Reshape::Reshape()
{
  one_blob_only = true;
  support_inplace = false;
}

int Reshape::load_param(const ParamDict& pd)
{
  w = pd.get(0, -233);
  h = pd.get(1, -233);
  c = pd.get(2, -233);
  permute = pd.get(3, 0);

  ndim = 3;

  if (c == -233) {
    ndim = 2;
  }

  if (h == -233) {
    ndim = 1;
  }

  if (w == -233) {
    ndim = 0;
  }

  return 0;
}

int Reshape::forward(const Blob* prev, Blob* next) const
{
  int total = prevSize.w * prevSize.h * prevSize.c;

  if (ndim == 1) {
    int _w = w;

    if (_w == 0) {
      _w = prevSize.w;
    }

    if (_w == -1) {
      _w = total;
    }

    if (permute == 1) {
      next->create(_w);

      if (next->empty()) {
        return -100;
      }

      // c-h-w to h-w-c
      float* ptr = next;

      for (i = 0; i < prevSize.h; i++) {
        for (j = 0; j < prevSize.w; j++) {
          for (p = 0; p < prevSize.c; p++) {
            const float* bptr = Blob_channel_p(prev, p);
            *ptr++ = bptr[i * prevSize.w + j];
          }
        }
      }
    }
    else {
      next = prev->reshape(_w);
    }
  }
  else if (ndim == 2) {
    int _w = w;
    int _h = h;

    if (_w == 0) {
      _w = prevSize.w;
    }

    if (_h == 0) {
      _h = prevSize.h;
    }

    if (_w == -1) {
      _w = total / _h;
    }

    if (_h == -1) {
      _h = total / _w;
    }

    next = prev->reshape(_w, _h);
  }
  else if (ndim == 3) {
    int _w = w;
    int _h = h;
    int _c = c;

    if (_w == 0) {
      _w = prevSize.w;
    }

    if (_h == 0) {
      _h = prevSize.h;
    }

    if (_c == 0) {
      _c = prevSize.c;
    }

    if (_w == -1) {
      _w = total / _c / _h;
    }

    if (_h == -1) {
      _h = total / _c / _w;
    }

    if (_c == -1) {
      _c = total / _h / _w;
    }

    next = prev->reshape(_w, _h, _c);
  }

  if (next->empty()) {
    return -100;
  }

  return 0;
}
