#ifndef CAFFE_IMAGE_DATA_LAYER_HPP_
#define CAFFE_IMAGE_DATA_LAYER_HPP_

#include "std/types_c.h"
#include "std/algo.h"
#include "wstd/string.hpp"
#include "imgio/imgio.h"
#include "../data_transformer.hpp"
using namespace wstd;

#define ImageDataLayer_DEF(DEF) \
DEF##String(source, "", 0) \
DEF##String(root_folder, "", 0) \
DEF##Int(rand_skip, 0, 0) \
DEF##Struct(transform, 0, DataTransformerInfo) \
DEF##Int(batch_size, 64, 0) \

struct ImageDataLayer : public Layer
{
  //vector< std::pair<std::string, vector<int> > > lines_;
  FILE* fp_;
  int iline_;
  vector<DataShape> shape_;
  ImageDataLayer_DEF(Def);

  // ImageDataLayer uses DataReader instead for sharing for parallelism
  virtual inline bool ShareInParallel() const { return false; }
  virtual inline const char* type() const { return "ImageData"; }
  virtual inline int ExactNumBottomBlobs() const { return 0; }
  virtual inline int MinTopBlobs() const { return 1; }
  virtual inline int MaxTopBlobs() const { return 10; }

  virtual void init() {
    ImageDataLayer_DEF(Init);
    fp_ = 0;
    iline_ = 0;
  }
  virtual void fromJson(cjson* param) {
    ImageDataLayer_DEF(Get);
    cjson* shapes_json = cjson_GetObjectItem(param, "shapes");
    int n = cjson_GetArraySize(shapes_json);
    shape_.resize(n);
    for (int i = 0; i < n; ++i) {
      cjson* shape_json = cjson_GetArrayItem(shapes_json, i);
      int m = cjson_GetArraySize(shape_json);
      int dim[4] = { 1,1,1,1 };
      m = MIN(m, 4);
      for (int j = 0; j < m; ++j) {
        dim[3-j] = cjson_GetArrayItem(shape_json, j)->valueint;
      }
      shape_[i].set(dim, 4);
    }
    fp_ = fopen(source_.c_str(), "rb");
    CHECK_NE(fp_, NULL) << "flie open fail : " << source_;
  }
  virtual void toJson(cjson* param) {
    ImageDataLayer_DEF(Set);
  }

  ImageDataLayer() {
    init();
    fp_ = NULL;
  }
  ~ImageDataLayer()
  {
    fclose(fp_);
  }

  virtual void LayerSetUp(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
  }

  virtual void Reshape(const vector<Blob*> & bottom, const vector<Blob*> & top) {
    CHECK_EQ(top.size(), shape_.size());
    for (int i = 0; i < top.size(); ++i) {
      DataShape shape = shape_[i];
      shape.n = batch_size_;
      top[i]->Reshape(shape);
    }
  }
  virtual void Forward_(const vector<Blob*> & bottom, const vector<Blob*> & top) {
    //rand skip
    img_t im[1] = { 0 };
    char buf[1024];
    CHECK_LE(top.size(), shape_.size());
    for (int item_id = 0; item_id < batch_size_; ) {
      if (feof(fp_)) {
        fseek(fp_, 0, SEEK_SET);
        iline_ = 0;
      }
      fgets(buf, 1024, fp_);
      vector<string> strs;
      split(strs, buf, ";");
      if (strs.size() ==1) {
        strs.resize(2);
        char* p = strchr(buf, ' ');
        if (p) {
          strs[0].assign(buf, p - buf);
          strs[1].assign(p + 1);
        }
        else {
          LOG(FATIL) << "line error:" << iline_;
          continue;
        }
      }
      CHECK_EQ(top.size(), strs.size());
      ++iline_;
      bool ok = 0;
      for (int i = 0; i < top.size(); ++i) {
        Blob* blob = top[i];
        Dtype* data = blob->cpu_mdata() + blob->offset(item_id);
        DataShape shape = shape_[i];
        string fn = strs[i];
        if (0 == i) {
          string fullfn = root_folder_ + fn;
          int w = shape.w;
          int h = shape.h;
          int cn = shape.c;
          if (imread(fullfn.c_str(), cn, 1, im)) {
            if (im->h != h || im->w != w) {
              imresize(im, h, w, im);
            }
            DataTransformer(i == 0 ? &transform_ : NULL, NHWC, shape, im->data, TF_U8, data);
            ok = true;
          }
        }
        else {
          vector<float> vec;
          atovf(fn.c_str(), vec);
          DataTransformer(NULL, NCHW, shape, vec.data(), TF_F32, data);
        }
      }
      if (ok) {
        ++item_id;
      }
    }
    return;
  }
  virtual void Backward_(const vector<Blob*> & top, const vector<Blob*> & bottom) {
  }

};

INSTANTIATE_CLASS(ImageData, ImageDataLayer);


#endif  // CAFFE_MNIST_DATA_LAYER_HPP_
