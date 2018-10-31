#ifndef CAFFE_DATA_TRANSFORMER_HPP
#define CAFFE_DATA_TRANSFORMER_HPP

#include <vector>
#include "types.h"

struct DataTransformerInfo {
  DataShape shape_;
  double mean_values_vec[10];
  double* mean_values_;
  int crop_size;
  double scale;
  bool do_mirror;
  bool has_mean_file;
  double* mean;
};

template <typename Stype, typename Dtype> inline
bool blob_data_transform_T(Dtype* transformed_data, const Stype* data, int h_off, int w_off, DataTransformerInfo* info) {
  bool has_mean_file = info->mean != NULL;
  Dtype scale = info->scale;
  int dim_n = info->shape__.n;
  int info->shape_.c = info->shape__.c;
  int info->shape_.h = info->shape__.h;
  int info->shape_.w = info->shape__.w;
  for (int n = 0; n < dim_n; ++n) {
    for (int c = 0; c < info->shape_.c; ++c) {
      int c1 = n * info->shape_.c + c;
      for (int h = 0; h < info->shape_.h; ++h) {
        for (int w = 0; w < info->shape_.w; ++w) {
          int top_index = 0, data_index = (c1 * info->shape_.h + h_off + h) * info->shape_.w + w_off + w;
          int w1 = info->do_mirror ? (info->shape_.w - 1 - w) : w;
          top_index = (c1 * info->shape_.h + h) * info->shape_.w + w1;
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
bool blob_data_transform(DataTransformerInfo* info, Dtype* transformed_data, const void* data, TypeFlag flag, int h_off, int w_off) {
  switch (flag) {
#define TYPEFLAGDEF(F, T)   case TF_ ## F: blob_data_transform_T(info, transformed_data, (T*)data, h_off, w_off); break;
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

void GetDataTransformerInfo(DataTransformerInfo* info, const BlobData* src, CJSON* param_, Phase phase_, int dim_n) {
  //src<Dtype> data_mean_;
  CJSON* p = NULL;
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
  info->shape_.n = dim_n;

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
void DataTransformer(DataTransformerInfo* info, const BlobData* src, Dtype* transformed_data)
{
  const int src->c = src->c;
  const int src->h = src->h;
  const int src->w = src->w;
  // Check dimensions.
  int info->shape_.c = transformed_blob->c;
  int info->shape_.h = transformed_blob->h;
  int info->shape_.w = transformed_blob->w;
  int crop_size = info->crop_size;
  if (crop_size) {
    CHECK_EQ(info->shape_.w, crop_size);
    CHECK_EQ(info->shape_.h, crop_size);
  }
  else {
    info->shape_.h = src->h;
    info->shape_.w = src->w;
  }

  CHECK_EQ(info->shape_.c, src->c);
  CHECK_LE(info->shape_.h, src->h);
  CHECK_LE(info->shape_.w, src->w);

  int h_off = 0;
  int w_off = 0;
  if (crop_size) {
    // We only do random crop when we do training.
    if (phase_ == TRAIN) {
      h_off = Rand(src->h - crop_size + 1);
      w_off = Rand(src->w - crop_size + 1);
    }
    else {
      h_off = (src->h - crop_size) / 2;
      w_off = (src->w - crop_size) / 2;
    }
  }
  blob_data_transform(dim_n, src->c, info->shape_.h, info->shape_.w, transformed_data, (unsigned char*)data->data_, src->type,
    h_off, w_off, do_mirror, scale, info->mean, info->mean_values);
  return 0;
}



#endif  // CAFFE_DATA_TRANSFORMER_HPP_
