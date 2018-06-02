
struct Eltwise : public Layer
{
public:
  Eltwise();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const std::vector<Blob>& bottom_blobs, std::vector<Blob>& top_blobs) const;

  enum { Operation_PROD = 0, Operation_SUM = 1, Operation_MAX = 2 };

public:
  // param
  int op_type;
  Blob coeffs;
};

Eltwise::Eltwise()
{
}

int Eltwise::load_param(const ParamDict& pd)
{
  op_type = pd.get(0, 0);
  coeffs = pd.get(1, Blob());

  return 0;
}

int Eltwise::forward(const std::vector<Blob>& bottom_blobs, std::vector<Blob>& top_blobs) const
{
  const Blob* prev = bottom_blobs[0];
  int w = prevSize.w;
  int h = prevSize.h;
  int channels = prevSize.c;
  int size = w * h;

  Blob* next = top_blobs[0];
  next->create(w, h, channels);

  if (next->empty()) {
    return -100;
  }

  if (op_type == Operation_PROD) {
    // first blob
    const Blob* bottom_blob1 = bottom_blobs[1];
    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      const float* ptr = prevData + q * Data_2DSize(prevSize);
      const float* ptr1 = bottom_blob1.channel(q);
      float* outptr = nextData + q * Data_2DSize(nextSize);

      for (i = 0; i < size; i++) {
        outptr[i] = ptr[i] * ptr1[i];
      }
    }

    for (size_t b = 2; b < bottom_blobs.size(); b++) {
      const Blob* bottom_blob1 = bottom_blobs[b];
      #pragma omp parallel for

      for (q = 0; q < channels; q++) {
        const float* ptr = bottom_blob1.channel(q);
        float* outptr = nextData + q * Data_2DSize(nextSize);

        for (i = 0; i < size; i++) {
          outptr[i] *= ptr[i];
        }
      }
    }
  }
  else if (op_type == Operation_SUM) {
    if (coeffs.w == 0) {
      // first blob
      const Blob* bottom_blob1 = bottom_blobs[1];
      #pragma omp parallel for

      for (q = 0; q < channels; q++) {
        const float* ptr = prevData + q * Data_2DSize(prevSize);
        const float* ptr1 = bottom_blob1.channel(q);
        float* outptr = nextData + q * Data_2DSize(nextSize);

        for (i = 0; i < size; i++) {
          outptr[i] = ptr[i] + ptr1[i];
        }
      }

      for (size_t b = 2; b < bottom_blobs.size(); b++) {
        const Blob* bottom_blob1 = bottom_blobs[b];
        #pragma omp parallel for

        for (q = 0; q < channels; q++) {
          const float* ptr = bottom_blob1.channel(q);
          float* outptr = nextData + q * Data_2DSize(nextSize);

          for (i = 0; i < size; i++) {
            outptr[i] += ptr[i];
          }
        }
      }
    }
    else {
      const float* coeffs_ptr = coeffs;

      // first blob
      const Blob* bottom_blob1 = bottom_blobs[1];
      float coeff0 = coeffs_ptr[0];
      float coeff1 = coeffs_ptr[1];
      #pragma omp parallel for

      for (q = 0; q < channels; q++) {
        const float* ptr = prevData + q * Data_2DSize(prevSize);
        const float* ptr1 = bottom_blob1.channel(q);
        float* outptr = nextData + q * Data_2DSize(nextSize);

        for (i = 0; i < size; i++) {
          outptr[i] = ptr[i] * coeff0 + ptr1[i] * coeff1;
        }
      }

      for (size_t b = 2; b < bottom_blobs.size(); b++) {
        const Blob* bottom_blob1 = bottom_blobs[b];
        float coeff = coeffs_ptr[b];
        #pragma omp parallel for

        for (q = 0; q < channels; q++) {
          const float* ptr = bottom_blob1.channel(q);
          float* outptr = nextData + q * Data_2DSize(nextSize);

          for (i = 0; i < size; i++) {
            outptr[i] += ptr[i] * coeff;
          }
        }
      }
    }
  }
  else if (op_type == Operation_MAX) {
    // first blob
    const Blob* bottom_blob1 = bottom_blobs[1];
    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      const float* ptr = prevData + q * Data_2DSize(prevSize);
      const float* ptr1 = bottom_blob1.channel(q);
      float* outptr = nextData + q * Data_2DSize(nextSize);

      for (i = 0; i < size; i++) {
        outptr[i] = MAX(ptr[i], ptr1[i]);
      }
    }

    for (size_t b = 2; b < bottom_blobs.size(); b++) {
      const Blob* bottom_blob1 = bottom_blobs[b];
      #pragma omp parallel for

      for (q = 0; q < channels; q++) {
        const float* ptr = bottom_blob1.channel(q);
        float* outptr = nextData + q * Data_2DSize(nextSize);

        for (i = 0; i < size; i++) {
          outptr[i] = MAX(outptr[i], ptr[i]);
        }
      }
    }
  }

  return 0;
}
