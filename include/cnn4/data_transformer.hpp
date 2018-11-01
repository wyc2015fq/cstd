#ifndef CAFFE_DATA_TRANSFORMER_HPP
#define CAFFE_DATA_TRANSFORMER_HPP

#include <vector>
#include "types.h"

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
    data_ = NULL;
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

int BlobData_cmp(const BlobData& a, const BlobData& b) {
  int ret;
  IFCMPRET(ret, a.nbytes_, b.nbytes_);
  IFCMPRET(ret, a.type_, b.type_);
  for (int i = 0; i<4; ++i) {
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
    memcpy(sz + (char*)(*s), data, len);
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

struct DataTransformerInfo {
  DataShape shape_;
  double mean_values_vec[10];
  double* mean_values_;
  int crop_size;
  double scale;
  bool do_mirror;
  bool has_mean_file;
  double* mean;
  Phase phase_;
};

template <typename Stype, typename Dtype> inline
bool blob_data_transform_T(DataTransformerInfo* info, int dn, Dtype* transformed_data, const Stype* data, int h_off, int w_off) {
  bool has_mean_file = info->mean != NULL;
  Dtype scale = info->scale;
  //int dn = info->shape_.n;
  int dc = info->shape_.c;
  int dh = info->shape_.h;
  int dw = info->shape_.w;
  for (int n = 0; n < dn; ++n) {
    for (int c = 0; c < dc; ++c) {
      int c1 = n * dc + c;
      for (int h = 0; h < dh; ++h) {
        for (int w = 0; w < dw; ++w) {
          int top_index = 0, data_index = (c1 * dh + h_off + h) * dw + w_off + w;
          int w1 = info->do_mirror ? (dw - 1 - w) : w;
          top_index = (c1 * dh + h) * dw + w1;
          Dtype datum_element = static_cast<Dtype>(data[data_index]);
          if (info->mean) {
            transformed_data[top_index] = (datum_element - info->mean[data_index]) * scale;
          }
          else {
            if (info->mean_values_) {
              transformed_data[top_index] = (datum_element - info->mean_values_[c]) * scale;
            }
            else {
              transformed_data[top_index] = datum_element * scale;
            }
          }
        }
      }
    }
  }
  return 0;
}

template <typename Dtype> inline
bool blob_data_transform(DataTransformerInfo* info, int dn, Dtype* transformed_data, const void* data, TypeFlag flag, int h_off, int w_off) {
  switch (flag) {
#define TYPEFLAGDEF(F, T)   case TF_ ## F: blob_data_transform_T(info, dn, transformed_data, (T*)data, h_off, w_off); break;
    TYPEFLAGDEF_DEF(TYPEFLAGDEF)
#undef TYPEFLAGDEF
  default:
    assert(0);
    break;
  }
  return 0;
}


int Rand(int n) {
  //CHECK(rng_);
  CHECK_GT(n, 0);
  return (rand() % n);
}

void GetDataTransformerInfo(DataTransformerInfo* info, const BlobData* src, CJSON* param_, Phase phase_, int batch_size_) {
  //src<Dtype> data_mean_;
  CJSON* p = NULL;
  info->phase_ = phase_;
  if (p = param_->get("mean_value")) {
    int n = p->GetArraySize();
    n = MIN(10, n);
    n = MIN(10, src->c);
    if (n > 0) {
      int i = 0;
      for (; i < n; ++i) {
        info->mean_values_vec[i] = p->get(i)->valuedouble;
      }
      double t = info->mean_values_vec[0];
      for (; i < src->c; ++i) {
        info->mean_values_vec[i] = (t);
      }
      info->mean_values_ = info->mean_values_vec;
    
    }
  }
  info->crop_size = param_->GetObjectInt("crop_size", 0);
  info->scale = param_->GetObjectNumber("scale", 1);
  info->do_mirror = param_->GetObjectBool("mirror", false) && Rand(2);
  info->has_mean_file = false;// param_->has("mean_file");
  info->mean = NULL;

  // Check dimensions.
  info->shape_.c = src->c;
  if (info->crop_size) {
    info->shape_.w = info->shape_.h = info->crop_size;
  }
  else {
    info->shape_.h = src->h;
    info->shape_.w = src->w;
  }
  info->shape_.n = batch_size_;

  CHECK_EQ(info->shape_.c, src->c);
  CHECK_LE(info->shape_.h, src->h);
  CHECK_LE(info->shape_.w, src->w);

#if 0
  Dtype* mean = NULL;
  if (has_mean_file) {
    CHECK_EQ(src->c, data_mean_.info->shape_.c());
    CHECK_EQ(src->h, data_mean_.info->shape_.h());
    CHECK_EQ(src->w, data_mean_.info->shape_.w());
    mean = data_mean_.mutable_cpu_data();
  }
#endif
}

template <typename Dtype>
int DataTransformer(DataTransformerInfo* info, const BlobData* src, Dtype* transformed_data)
{
  // Check dimensions.
  int crop_size = info->crop_size;
  if (crop_size) {
    CHECK_EQ(info->shape_.w, crop_size);
    CHECK_EQ(info->shape_.h, crop_size);
  }
  else {
    CHECK_EQ(info->shape_.h, src->h);
    CHECK_EQ(info->shape_.w, src->w);
  }

  CHECK_EQ(info->shape_.c, src->c);
  CHECK_LE(info->shape_.h, src->h);
  CHECK_LE(info->shape_.w, src->w);

  int h_off = 0;
  int w_off = 0;
  if (crop_size) {
    // We only do random crop when we do training.
    if (info->phase_ == TRAIN) {
      h_off = Rand(src->h - crop_size + 1);
      w_off = Rand(src->w - crop_size + 1);
    }
    else {
      h_off = (src->h - crop_size) / 2;
      w_off = (src->w - crop_size) / 2;
    }
  }
  blob_data_transform(info, 1, transformed_data, src->data_, (TypeFlag)src->type_, h_off, w_off);
  return 0;
}



#endif  // CAFFE_DATA_TRANSFORMER_HPP_
