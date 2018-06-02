

struct Reduction : public Layer
{
public:
  Reduction();
  virtual ~Reduction();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const Blob* prev, Blob* next) const;

  enum {
    ReductionOp_SUM     = 0,
    ReductionOp_ASUM    = 1,
    ReductionOp_SUMSQ   = 2,
    ReductionOp_MEAN    = 3,
    ReductionOp_MAX     = 4,
    ReductionOp_MIN     = 5,
    ReductionOp_PROD    = 6
  };

public:
  // param
  int operation;
  int dim;
  float coeff;
};


Reduction::Reduction()
{
  one_blob_only = true;
  support_inplace = false;
}

Reduction::~Reduction()
{
}

int Reduction::load_param(const ParamDict& pd)
{
  operation = pd.get(0, 0);
  dim = pd.get(1, 0);
  coeff = pd.get(2, 1.f);

  return 0;
}

template<typename Op, typename Op2>
static int reduction_op(const Blob* a, Blob* b, float v0, int dim, float coeff)
{
  Op op;
  Op2 op2;

  int w = a.w;
  int h = a.h;
  int channels = a.c;
  int size = w * h;

  if (dim == 0) {
    // w h c -> X X X
    b.create(1);
  }
  else if (dim == 1) {
    // w h c -> X X c
    b.create(channels);
  }
  else if (dim == 2) {
    // w h c -> X h c
    b.create(h, channels);
  }
  else if (dim == -1) {
    // w h c -> w X X
    b.create(w);
  }
  else if (dim == -2) {
    // w h c -> w h X
    b.create(w, h);
  }

  if (b.empty()) {
    return -100;
  }

  if (dim == 0) {
    Blob sums(channels);

    if (sums.empty()) {
      return -100;
    }

    float* sums_ptr = sums;
    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      const float* ptr = a.channel(q);

      float sum = v0;

      for (i = 0; i < size; i++) {
        sum = op(sum, ptr[i]);
      }

      sums_ptr[q] = sum;
    }

    float* outptr = b;

    float sum = v0;

    for (i = 0; i < channels; i++) {
      sum = op2(sum, sums_ptr[i]);
    }

    outptr[0] = sum * coeff;
  }
  else if (dim == 1) {
    float* outptr = b;
    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      const float* ptr = a.channel(q);

      float sum = v0;

      for (i = 0; i < size; i++) {
        sum = op(sum, ptr[i]);
      }

      outptr[q] = sum * coeff;
    }
  }
  else if (dim == 2) {
    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      const float* ptr = a.channel(q);
      float* outptr = b.channel(q);

      for (i = 0; i < h; i++) {
        float sum = v0;

        for (j = 0; j < w; j++) {
          sum = op(sum, ptr[i]);
        }

        outptr[i] = sum * coeff;

        ptr += w;
      }
    }
  }
  else if (dim == -1) {
    Blob mins(w, 1, channels);

    if (mins.empty()) {
      return -100;
    }

    mins.fill(v0);

    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      const float* ptr = a.channel(q);
      float* mins_ptr = mins.channel(q);

      for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
          mins_ptr[j] = op(mins_ptr[j], ptr[i]);
        }

        ptr += w;
      }
    }

    b.fill(v0);

    float* outptr = b;

    for (q = 0; q < channels; q++) {
      const float* mins_ptr = mins.channel(q);

      for (j = 0; j < w; j++) {
        outptr[j] = op2(outptr[j], mins_ptr[j]);
      }
    }

    for (j = 0; j < w; j++) {
      outptr[j] *= coeff;
    }
  }
  else if (dim == -2) {
    b.fill(v0);

    for (q = 0; q < channels; q++) {
      const float* ptr = a.channel(q);
      float* outptr = b;

      for (i = 0; i < size; i++) {
        outptr[i] = op(outptr[i], ptr[i]);
      }
    }

    float* outptr = b;

    for (i = 0; i < size; i++) {
      outptr[i] *= coeff;
    }
  }

  return 0;
}

template<typename T>
struct reduction_op_asum : std::binary_function<T, T, T> {
  T operator()(const T& x, const T& y) const {
    return x + fabs(y);
  }
};

template<typename T>
struct reduction_op_sumsq : std::binary_function<T, T, T> {
  T operator()(const T& x, const T& y) const {
    return x + y * y;
  }
};

template<typename T>
struct reduction_op_max : std::binary_function<T, T, T> {
  T operator()(const T& x, const T& y) const {
    return MAX(x, y);
  }
};

template<typename T>
struct reduction_op_min : std::binary_function<T, T, T> {
  T operator()(const T& x, const T& y) const {
    return MIN(x, y);
  }
};

int Reduction::forward(const Blob* prev, Blob* next) const
{
  if (operation == ReductionOp_SUM) {
    return reduction_op< std::plus<float>, std::plus<float> >(prev, next, 0.f, dim, coeff);
  }

  if (operation == ReductionOp_ASUM) {
    return reduction_op< reduction_op_asum<float>, std::plus<float> >(prev, next, 0.f, dim, coeff);
  }

  if (operation == ReductionOp_SUMSQ) {
    return reduction_op< reduction_op_sumsq<float>, std::plus<float> >(prev, next, 0.f, dim, coeff);
  }

  if (operation == ReductionOp_MEAN) {
    int ret = reduction_op< std::plus<float>, std::plus<float> >(prev, next, 0.f, dim, coeff);

    if (ret != 0) {
      return -100;
    }

    int w = prevSize.w;
    int h = prevSize.h;
    int channels = prevSize.c;
    int size = w * h;

    if (dim == 0) {
      float* outptr = next;
      outptr[0] /= channels * size;
    }
    else if (dim == 1) {
      float* outptr = next;

      for (q = 0; q < channels; q++) {
        outptr[q] /= size;
      }
    }
    else if (dim == 2) {
      for (q = 0; q < channels; q++) {
        float* outptr = nextData + q * Data_2DSize(nextSize);

        for (i = 0; i < h; i++) {
          outptr[i] /= w;
        }
      }
    }
    else if (dim == -1) {
      float* outptr = next;

      for (j = 0; j < w; j++) {
        outptr[j] /= h * channels;
      }
    }
    else if (dim == -2) {
      float* outptr = next;

      for (i = 0; i < size; i++) {
        outptr[i] /= channels;
      }
    }
  }

  if (operation == ReductionOp_MAX) {
    return reduction_op< reduction_op_max<float>, reduction_op_max<float> >(prev, next, -FLT_MAX, dim, coeff);
  }

  if (operation == ReductionOp_MIN) {
    return reduction_op< reduction_op_min<float>, reduction_op_min<float> >(prev, next, FLT_MAX, dim, coeff);
  }

  if (operation == ReductionOp_PROD) {
    return reduction_op< std::multiplies<float>, std::multiplies<float> >(prev, next, 1.f, dim, coeff);
  }

  return 0;
}

} // namespace ncnn
