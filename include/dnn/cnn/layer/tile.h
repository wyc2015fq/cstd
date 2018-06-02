

struct Tile : public Layer
{
public:
  Tile();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const Blob* prev, Blob* next) const;

public:
  int dim;
  int tiles;
};


Tile::Tile()
{
  one_blob_only = true;
  support_inplace = false;
}

int Tile::load_param(const ParamDict& pd)
{
  dim = pd.get(0, 0);
  tiles = pd.get(1, 1);

  return 0;
}

int Tile::forward(const Blob* prev, Blob* next) const
{
  int w = prevSize.w;
  int h = prevSize.h;
  int channels = prevSize.c;

  if (dim == 0) {
    next->create(w, h, channels * tiles);

    if (next->empty()) {
      return -100;
    }

    const float* ptr = prev;
    int size = prev->cstep * channels;

    #pragma omp parallel for

    for (p = 0; p < tiles; p++) {
      float* outptr = Blob_channel_p(next, p * channels);

      for (i = 0; i < size; i++) {
        outptr[i] = ptr[i];
      }
    }
  }
  else if (dim == 1) {
    next->create(w, h * tiles, channels);

    if (next->empty()) {
      return -100;
    }

    int size = w * h;

    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      const float* ptr = prevData + q * Data_2DSize(prevSize);
      float* outptr = nextData + q * Data_2DSize(nextSize);

      for (p = 0; p < tiles; p++) {
        for (i = 0; i < size; i++) {
          outptr[i] = ptr[i];
        }

        outptr += size;
      }
    }
  }
  else if (dim == 2) {
    next->create(w * tiles, h, channels);

    if (next->empty()) {
      return -100;
    }

    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      const float* ptr = prevData + q * Data_2DSize(prevSize);
      float* outptr = nextData + q * Data_2DSize(nextSize);

      for (i = 0; i < h; i++) {
        for (p = 0; p < tiles; p++) {
          for (j = 0; j < w; j++) {
            outptr[j] = ptr[j];
          }

          outptr += w;
        }

        ptr += w;
      }
    }
  }

  return 0;
}

} // namespace ncnn
