

struct Normalize : public Layer
{
public:
  Normalize();

  virtual int load_param(const ParamDict& pd);

#if NCNN_STDIO
  virtual int load_model(FILE* binfp);
#endif // NCNN_STDIO
  virtual int load_model(const unsigned char*& mem);

  virtual int forward(const Blob* prev, Blob* next) const;

public:
  // param
  int across_spatial;
  int channel_shared;
  float eps;
  int scale_data_size;

  Blob scale_data;
};


namespace ncnn
{

DEFINE_LAYER_CREATOR(Normalize)

Normalize::Normalize()
{
  one_blob_only = true;
  support_inplace = false;
}

int Normalize::load_param(const ParamDict& pd)
{
  across_spatial = pd.get(0, 0);
  channel_shared = pd.get(1, 0);
  eps = pd.get(2, 0.0001f);
  scale_data_size = pd.get(3, 0);

  return 0;
}

#if NCNN_STDIO
int Normalize::load_model(FILE* binfp)
{
  int nread;

  scale_data.create(1, scale_data_size);
  nread = fread(scale_data, scale_data_size * sizeof(float), 1, binfp);

  if (nread != 1) {
    fprintf(stderr, "Normalize read scale_data failed %d\n", nread);
    return -1;
  }

  return 0;
}
#endif // NCNN_STDIO

int Normalize::load_model(const unsigned char*& mem)
{
  scale_data = Blob(1, scale_data_size, (float*)mem);
  mem += scale_data_size * sizeof(float);

  return 0;
}

int Normalize::forward(const Blob* prev, Blob* next) const
{
  int w = prevSize.w;
  int h = prevSize.h;
  int channels = prevSize.c;
  int size = w * h;

  next->create(w, h, channels);

  if (next->empty()) {
    return -100;
  }

  if (across_spatial) {
    // square
    Blob square_sum_blob;
    square_sum_blob.create(channels);

    if (square_sum_blob.empty()) {
      return -100;
    }

    float* square_sum_ptr = square_sum_blob;
    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      const float* ptr = prevData + q * Data_2DSize(prevSize);

      float ssum = 0.f;

      for (i = 0; i < size; i++) {
        ssum += ptr[i] * ptr[i];
      }

      square_sum_ptr[q] = ssum;
    }

    // sum + eps
    float ssum = eps;

    for (q = 0; q < channels; q++) {
      ssum += square_sum_ptr[q];
    }

    // 1 / sqrt(ssum)
    float a = 1.f / sqrt(ssum);

    if (channel_shared) {
      float scale = a * scale_data.data[0];

      #pragma omp parallel for

      for (q = 0; q < channels; q++) {
        const float* ptr = prevData + q * Data_2DSize(prevSize);
        float* outptr = nextData + q * Data_2DSize(nextSize);

        for (i = 0; i < size; i++) {
          outptr[i] = ptr[i] * scale;
        }
      }
    }
    else {
      #pragma omp parallel for

      for (q = 0; q < channels; q++) {
        const float* ptr = prevData + q * Data_2DSize(prevSize);
        float* outptr = nextData + q * Data_2DSize(nextSize);
        float scale = a * scale_data.data[q];

        for (i = 0; i < size; i++) {
          outptr[i] = ptr[i] * scale;
        }
      }
    }
  }
  else {
    // square sum, 1 / sqrt(ssum)
    Blob square_sum_blob;
    square_sum_blob.create(w, h);

    if (square_sum_blob.empty()) {
      return -100;
    }

    float* ssptr = square_sum_blob;

    if (channel_shared) {
      float scale = scale_data.data[0];

      #pragma omp parallel for

      for (i = 0; i < size; i++) {
        float ssum = eps;

        for (q = 0; q < channels; q++) {
          const float* ptr = prevData + q * Data_2DSize(prevSize);
          ssum += ptr[i] * ptr[i];
        }

        ssptr[i] = 1.f / sqrt(ssum) * scale;
      }

      #pragma omp parallel for

      for (q = 0; q < channels; q++) {
        const float* ptr = prevData + q * Data_2DSize(prevSize);
        float* outptr = nextData + q * Data_2DSize(nextSize);

        for (i = 0; i < size; i++) {
          outptr[i] = ptr[i] * ssptr[i];
        }
      }
    }
    else {
      #pragma omp parallel for

      for (i = 0; i < size; i++) {
        float ssum = eps;

        for (q = 0; q < channels; q++) {
          const float* ptr = prevData + q * Data_2DSize(prevSize);
          ssum += ptr[i] * ptr[i];
        }

        ssptr[i] = 1.f / sqrt(ssum);
      }

      #pragma omp parallel for

      for (q = 0; q < channels; q++) {
        const float* ptr = prevData + q * Data_2DSize(prevSize);
        float* outptr = nextData + q * Data_2DSize(nextSize);
        float scale = scale_data.data[q];

        for (i = 0; i < size; i++) {
          outptr[i] = ptr[i] * ssptr[i] * scale;
        }
      }
    }
  }

  return 0;
}

} // namespace ncnn
