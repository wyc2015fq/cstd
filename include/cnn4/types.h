
#ifndef _CNN4_TYPES_H_
#define _CNN4_TYPES_H_

#define TYPEFLAGDEF_DEF(DEF) \
DEF(S8, char) \
DEF(U8, unsigned char) \
DEF(S16, short) \
DEF(U16, unsigned short) \
DEF(S32, int) \
DEF(U32, unsigned int) \
DEF(F32, float) \
DEF(F64, double)

enum TypeFlag //枚举消息类型
{
#define TYPEFLAGDEF(a, b)  TF_ ## a,
  TYPEFLAGDEF_DEF(TYPEFLAGDEF)
#undef TYPEFLAGDEF
};

static const int TypeSize[] = {
#define TYPEFLAGDEF(a, b)  sizeof(b),
  TYPEFLAGDEF_DEF(TYPEFLAGDEF)
#undef TYPEFLAGDEF
};

#if 0
template <typename T>
struct TypeFlag_TF { enum { flag = 0 }; };

#define TYPEFLAGDEF(F, T)   template<> struct TypeFlag_TF<T> { enum { flag = TF_ ## F }; };
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
  union {
    int dim[MAX_DIM];
    struct { int n, c, h, w; };
  };
  void set(int n, int c = 1, int h = 1, int w = 1) { dim[0] = n, dim[1] = c, dim[2] = h, dim[3] = w; }
  int count() const { return dim[0] * dim[1] * dim[2] * dim[3]; }
  int* begin() { return dim; }
  int* end() { return dim + MAX_DIM; }
  const int* begin() const { return dim; }
  const int* end() const { return dim + MAX_DIM; }
  void resize(int k) {
    for (; k < MAX_DIM; ++k) {
      dim[k] = 1;
    }
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

///////////////////////////////////////////////////

#endif // _CNN4_TYPES_H_
