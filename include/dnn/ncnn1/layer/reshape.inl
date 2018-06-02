

#include "reshape.h"

namespace ncnn
{

DEFINE_LAYER_CREATOR(Reshape)

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

int Reshape::forward(const img_t* bottom_blob, img_t* top_blob) const
{
  int total = bottom_blob->w * bottom_blob->h * pBox_channels(bottom_blob);

  if (ndim == 1) {
    int _w = w;

    if (_w == 0) {
      _w = bottom_blob->w;
    }

    if (_w == -1) {
      _w = total;
    }

    if (permute == 1) {
      top_blob->create(_w);

      if (top_blob->empty()) {
        return -100;
      }

      // c-h-w to h-w-c
      float* ptr = top_blob;

      for (int i = 0; i < bottom_blob->h; i++) {
        for (int j = 0; j < bottom_blob->w; j++) {
          for (int p = 0; p < pBox_channels(bottom_blob); p++) {
            const float* bptr = pBox_channel_p(bottom_blob, p);
            *ptr++ = bptr[i * bottom_blob->w + j];
          }
        }
      }
    }
    else {
      top_blob = bottom_blob->reshape(_w);
    }
  }
  else if (ndim == 2) {
    int _w = w;
    int _h = h;

    if (_w == 0) {
      _w = bottom_blob->w;
    }

    if (_h == 0) {
      _h = bottom_blob->h;
    }

    if (_w == -1) {
      _w = total / _h;
    }

    if (_h == -1) {
      _h = total / _w;
    }

    top_blob = bottom_blob->reshape(_w, _h);
  }
  else if (ndim == 3) {
    int _w = w;
    int _h = h;
    int _c = c;

    if (_w == 0) {
      _w = bottom_blob->w;
    }

    if (_h == 0) {
      _h = bottom_blob->h;
    }

    if (_c == 0) {
      _c = pBox_channels(bottom_blob);
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

    top_blob = bottom_blob->reshape(_w, _h, _c);
  }

  if (top_blob->empty()) {
    return -100;
  }

  return 0;
}

} // namespace ncnn
