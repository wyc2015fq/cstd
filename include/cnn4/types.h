
#ifndef _CNN4_TYPES_H_
#define _CNN4_TYPES_H_

typedef void void_type;
typedef void void_float;

struct int8x4 { unsigned char v[4]; };
//DEF(U8x4, int8x4, CUDNN_DATA_INT8x4)

#define TYPEFLAGDEF_DEF(DEF) \
DEF(S8, char, CUDNN_DATA_INT8) \
DEF(U8, unsigned char, CUDNN_DATA_UINT8) \
DEF(S16, short, CUDNN_DATA_HALF) \
DEF(U16, unsigned short, CUDNN_DATA_HALF) \
DEF(S32, int, CUDNN_DATA_INT32) \
DEF(U32, unsigned int, CUDNN_DATA_INT32) \
DEF(F32, float, CUDNN_DATA_FLOAT) \
DEF(F64, double, CUDNN_DATA_DOUBLE)


enum TypeFlag //枚举消息类型
{
#define TYPEFLAGDEF(a, b, c)  TF_ ## a,
  TYPEFLAGDEF_DEF(TYPEFLAGDEF)
#undef TYPEFLAGDEF
};

static const int TypeSize[] = {
#define TYPEFLAGDEF(a, b, c)  sizeof(b),
  TYPEFLAGDEF_DEF(TYPEFLAGDEF)
#undef TYPEFLAGDEF
};

#if 1
template <typename T>
struct TypeFlag_TF { enum { flag = 0 }; };

#define TYPEFLAGDEF(F, T, c)   template<> struct TypeFlag_TF<T> { enum { flag = TF_ ## F }; };
TYPEFLAGDEF_DEF(TYPEFLAGDEF)
#undef TYPEFLAGDEF
#endif

//shape[0] = num;
//shape[1] = channels;
//shape[2] = height;
//shape[3] = width;

enum DimType { NCHW, NHWC };
#define MAX_DIM 4

struct DataShape {
  //TypeFlag t;
  union {
    int dim[MAX_DIM];
    struct { int n, c, h, w; };
  };
  void set(int n, int c = 1, int h = 1, int w = 1) { dim[0] = n, dim[1] = c, dim[2] = h, dim[3] = w; }
  int count() const { return dim[0] * dim[1] * dim[2] * dim[3]; }
  //int size() const { return count()*TypeSize[t]; }
  int* begin() { return dim; }
  int* end() { return dim + MAX_DIM; }
  const int* begin() const { return dim; }
  const int* end() const { return dim + MAX_DIM; }
  void resize(int k) {
    for (; k < MAX_DIM; ++k) {
      dim[k] = 1;
    }
  }
  inline int num_axes() const { return (int)4; }
  inline int shape(int index) const {
    return dim[CanonicalAxisIndex(index)];
  }
  inline int CanonicalAxisIndex(int axis_index) const {
    if (axis_index < 0) {
      return axis_index + num_axes();
    }
    return axis_index;
  }
  inline int count(int start_axis, int end_axis) const {
    start_axis = min(start_axis, num_axes());
    end_axis = min(end_axis, num_axes());
    CHECK_LE(start_axis, end_axis);
    CHECK_GE(start_axis, 0);
    CHECK_GE(end_axis, 0);
    CHECK_LE(start_axis, num_axes());
    CHECK_LE(end_axis, num_axes());
    int count = 1;
    for (int i = start_axis; i < end_axis; ++i) {
      count *= shape(i);
    }
    return count;
  }
  inline int count(int start_axis) const {
    return count(start_axis, num_axes());
  }
  /// @brief Deprecated legacy shape accessor num: use shape(0) instead.
  inline int num() const { return LegacyShape(0); }
  /// @brief Deprecated legacy shape accessor channels: use shape(1) instead.
  inline int channels() const { return LegacyShape(1); }
  /// @brief Deprecated legacy shape accessor height: use shape(2) instead.
  inline int height() const { return LegacyShape(2); }
  /// @brief Deprecated legacy shape accessor width: use shape(3) instead.
  inline int width() const { return LegacyShape(3); }
  inline int LegacyShape(int index) const {
    CHECK_LE(num_axes(), 4)
      << "Cannot use legacy accessors on Blobs with > 4 axes.";
    CHECK_LT(index, 4);
    CHECK_GE(index, -4);
    if (index >= num_axes() || index < -num_axes()) {
      // Axis is out of range, but still in [0, 3] (or [-4, -1] for reverse
      // indexing) -- this special case simulates the one-padding used to fill
      // extraneous axes of legacy blobs.
      return 1;
    }
    return shape(index);
  }
};

static int DataShape_cmp(const DataShape& a, const DataShape& b) {
  int ret = 0;
  IFCMPRET(ret, a.n, b.n);
  IFCMPRET(ret, a.c, b.c);
  IFCMPRET(ret, a.h, b.h);
  IFCMPRET(ret, a.w, b.w);
  return ret;
}
inline bool operator ==(const DataShape& a, const DataShape& b) {
  return 0 == DataShape_cmp(a, b);
}
inline bool operator !=(const DataShape& a, const DataShape& b) {
  return 0 != DataShape_cmp(a, b);
}
///////////////////////////////////////////////////
#define PHASE_DEF_DEF(DEF) \
DEF(TRAINorTEST) \
DEF(TRAIN) \
DEF(TEST)

enum Phase {
#define POOL_DEF(a) a,
  PHASE_DEF_DEF(POOL_DEF)
#undef POOL_DEF
};

static const char* Phase_Name[] = {
#define POOL_DEF(a) #a ,
  PHASE_DEF_DEF(POOL_DEF)
#undef POOL_DEF
};
///////////////////////////////////////////////////
#define FILLER_DEF_DEF(DEF) \
DEF(constant) \
DEF(gaussian) \
DEF(positive_unitball) \
DEF(uniform) \
DEF(xavier) \
DEF(msra) \
DEF(bilinear)

enum FillerMethod {
#define DEF(a) FillerMethod_ ## a,
  FILLER_DEF_DEF(DEF)
#undef DEF
};
static const char* FillerMethod_Name[] = {
#define DEF(a) #a ,
  FILLER_DEF_DEF(DEF)
#undef DEF
};

///////////////////////////////////////////////////
#define DBMETHOD_DEF_DEF(DEF) \
DEF(LEVELDB) \
DEF(LMDB)
enum DBMethod {
#define DEF(a) DBMethod_##a,
  DBMETHOD_DEF_DEF(DEF)
#undef DEF
};
static const char* DBMethod_Name[] {
#define DEF(a) #a ,
  DBMETHOD_DEF_DEF(DEF)
#undef DEF
};

///////////////////////////////////////////////////
// pooling
#define POOL_DEF_DEF(DEF) \
DEF(MAX) \
DEF(AVE) \
DEF(STOCHASTIC) \
DEF(DEF) \
DEF(DEF_ALL) \
DEF(DEF_ALL2) \
DEF(DEF_ALL3) \
DEF(DEF_ALL4) \
DEF(LOWRES)

enum PoolMethod {
#define POOL_DEF(a) PoolMethod_ ## a,
  POOL_DEF_DEF(POOL_DEF)
#undef POOL_DEF
};

static const char* PoolMethod_Name[] = {
#define POOL_DEF(a) #a ,
  POOL_DEF_DEF(POOL_DEF)
#undef POOL_DEF
};

///////////////////////////////////////////////////
// pooling
#define NORMALIZATIONMODE_DEF_DEF(DEF) \
DEF(FULL) \
DEF(VALID) \
DEF(BATCH_SIZE) \
DEF(NONE)

enum NormalizationMode {
#define DEF(a) NormalizationMode_ ## a,
  NORMALIZATIONMODE_DEF_DEF(DEF)
#undef DEF
};

static const char* NormalizationMode_Name[] = {
#define DEF(a) #a ,
  NORMALIZATIONMODE_DEF_DEF(DEF)
#undef DEF
};
#if 0
int get_normalizer(DataShape bottom_shape, int axis_, NormalizationMode normalization_mode, int valid_count)
{
  int normalizer;
  int softmax_axis_ = bottom_shape.CanonicalAxisIndex(axis_);
  int outer_num_ = bottom_shape.count(0, softmax_axis_);
  int inner_num_ = bottom_shape.count(softmax_axis_ + 1);

  switch (normalization_mode) {
  case NormalizationMode_FULL:
    normalizer = int(outer_num_ * inner_num_);
    break;
  case NormalizationMode_VALID:
    if (valid_count == -1) {
      normalizer = int(outer_num_ * inner_num_);
    }
    else {
      normalizer = int(valid_count);
    }
    break;
  case NormalizationMode_BATCH_SIZE:
    normalizer = int(outer_num_);
    break;
  case NormalizationMode_NONE:
    normalizer = int(1);
    break;
  default:
    LOG(FATAL) << "Unknown normalization mode: "
      << NormalizationMode_Name[normalization_mode];
  }
  // Some users will have no labels for some examples in order to 'turn off' a
  // particular loss in a multi-task setup. The max prevents NaNs in that case.
  return std::max(int(1), normalizer);
}
#endif

///////////////////////////////////////////////////
enum VarianceNorm {
  FAN_IN, FAN_OUT, AVERAGE,
};
static const char* VarianceNorm_Name[] = {
  "FAN_IN", "FAN_OUT", "AVERAGE",
};

///////////////////////////////////////////////////

#endif // _CNN4_TYPES_H_
