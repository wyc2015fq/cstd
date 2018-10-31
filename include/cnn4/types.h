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

template <typename T>
struct TypeFlag_TF { enum { flag = 0 }; };

#define TYPEFLAGDEF(F, T)   template<> struct TypeFlag_TF<T> { enum { flag = TF_ ## F }; };
TYPEFLAGDEF_DEF(TYPEFLAGDEF)
#undef TYPEFLAGDEF

//shape[0] = num;
//shape[1] = channels;
//shape[2] = height;
//shape[3] = width;

enum DimType { NCHW, NHWC };

template <typename T> static
int NHWC2NCHW_T(T* dst, const T* src, int dim_w, int dim_h = 1, int dim_c = 1, int dim_n = 1) {
  int img_index = 0;
  for (int i = 0; i < dim_n; ++i) {
    for (int h = 0; h < dim_h; ++h) {
      for (int w = 0; w < dim_w; ++w) {
        for (int c = 0; c < dim_c; ++c) {
          int datum_index = ((i*dim_c + c) * dim_h + h) * dim_w + w;
          dst[datum_index] = (src[img_index++]);
        }
      }
    }
  }
  return 0;
}
int NHWC2NCHW(TypeFlag flag, void* dst, const void* src, int dim_w, int dim_h = 1, int dim_c = 1, int dim_n = 1) {
  switch (flag) {
#define TYPEFLAGDEF(F, T)   case TF_ ## F: NHWC2NCHW_T<T>((T*)dst, (const T*)src, dim_w, dim_h, dim_c, dim_n); break;
    TYPEFLAGDEF_DEF(TYPEFLAGDEF)
#undef TYPEFLAGDEF
  default:
    assert(0);
    break;
  }
  return 0;
}

struct DataShape {
  union {
  int dim[4];
  struct { int n, c, h, w; };
  };
  void set(int n, int c = 1, int h = 1, int w = 1) { dim[0] = n, dim[1] = c, dim[2] = h, dim[3] = w; }
  int count() const { return dim[0] * dim[1] * dim[2] * dim[3]; }
  int* begin() { return dim; }
  int* end() { return dim + 4; }
  const int* begin() const { return dim; }
  const int* end() const { return dim + 4; }
};

struct BlobData {
  int nbytes_;
  union {
    DataShape shape;
    int dim_[4];
    struct {
      int n, c, h, w;
    };
  };
  int type_;
  char* data_;
  BlobData() { memset(this, 0, sizeof(*this)); }
  ~BlobData() { Free(); }
  void Free() {
    free(data_);
    data_  = NULL;
  }
  void set(DimType dimtype, TypeFlag flag, const void* data, const int* dim) {
    set(dimtype, flag, data, dim[3], dim[2], dim[1], dim[0]);
  }
  void set(DimType dimtype, TypeFlag flag, const void* data, int dim_w, int dim_h = 1, int dim_c = 1, int dim_n = 1) {
    int size = dim_w*dim_h*dim_c*dim_n;
    int elemsz = TypeSize[flag];
    dim_[0] = dim_n;
    dim_[1] = dim_c;
    dim_[2] = dim_h;
    dim_[3] = dim_w;
    type_ = flag;
    nbytes_ = size*elemsz;
    data_ = (char*)realloc(data_, nbytes_);
    if (data) {
      if (dimtype == NCHW) {
        memcpy(data_, data, nbytes_);
      }
      else {
        NHWC2NCHW(flag, data_, data, dim_w, dim_h, dim_c, dim_n);
      }
    }
  }
};

#define IFRET(ret, comp)  if (ret = comp) {return ret;}
#define IFCMPRET(ret, a, b)  if (ret = CMP(a, b)) {return ret;}
int BlobData_cmp(const BlobData& a, const BlobData& b) {
  int ret;
  IFCMPRET(ret, a.nbytes_, b.nbytes_);
  IFCMPRET(ret, a.type_, b.type_);
  for (int i=0; i<4; ++i) {
    IFCMPRET(ret, a.dim_[i], b.dim_[i]);
  }
  for (int i = 0; i < a.nbytes_; ++i) {
    IFCMPRET(ret, a.data_[i], b.data_[i]);
  }
  return ret;
}

void mem_append(void** s, void* data, int len) {
  if (len>0) {
   int sz = _msize(*s);
   *s = realloc(s, sz + len);
   memcpy(sz+(char*)(*s), data, len);
  }
}
#define MEM_APPEND(s, t)  mem_append(s, &t, sizeof(t))
void mem_append_blob(void** s, BlobData* blob) {
  MEM_APPEND(s, blob->nbytes_);
  MEM_APPEND(s, blob->dim_);
  MEM_APPEND(s, blob->type_);
  mem_append(s, blob->data_, blob->nbytes_);
}

#include <string>
using namespace std;

typedef vector<BlobData> Datum;
int Datum_cmp(const Datum& a, const Datum& b) {
  int ret = 0;
  IFCMPRET(ret, a.size(), b.size());
  for (int i = 0; i < a.size(); ++i) {
    IFRET(ret, BlobData_cmp(a[i], b[i]));
  }
  return ret;
}

#define STR_APPEND(s, t)  s->append((char*)&t, sizeof(t))
void str_append_blob(string* s, BlobData* blob) {
  //STR_APPEND(s, blob->nbytes_);
  STR_APPEND(s, blob->dim_);
  STR_APPEND(s, blob->type_);
  s->append(blob->data_, blob->nbytes_);
}
void str_append_datum(string* s, Datum& datum) {
  int n = datum.size();
  STR_APPEND(s, n);
  for (int i = 0; i < n; ++i) {
    str_append_blob(s, &datum[i]);
  }
}

#define STR_GETN(s, i, p, n)  memcpy((char*)p, (const char*)s + (i+=n) - n, n)
#define STR_GET(s, i, t)  STR_GETN(s, i, &t, sizeof(t))
void str_get_blob(const char* s, int& i, BlobData* blob) {
  int dim[4], type;
  STR_GET(s, i, dim);
  STR_GET(s, i, type);
  blob->set(NCHW, (TypeFlag)type, NULL, dim);
  STR_GETN(s, i, blob->data_, blob->nbytes_);
}
void str_get_datum(const char* s, int& i, Datum& datum) {
  int n;
  STR_GET(s, i, n);
  datum.resize(n);
  for (int j = 0; j < n; ++j) {
    str_get_blob(s, i, &datum[j]);
  }
}
void ParseFromString(const string& s, Datum& datum) {
  int i = 0;
  str_get_datum(s.c_str(), i, datum);
}

#undef STR_APPEND
#undef STR_APPEND
#endif // _CNN4_TYPES_H_
