#ifndef CAFFE_MNIST_DATA_LAYER_HPP_
#define CAFFE_MNIST_DATA_LAYER_HPP_

#include "std/types_c.h"
#include "std/algo.h"
#include "../data_transformer.hpp"
#include "std/mnistdata_c.h"
#include "std/gui_c.h"

#define MnistDataLayer_DEF(DEF) \
DEF##Int(rand_skip, 0, 0) \
DEF##Struct(transform, 0, DataTransformerInfo) \
DEF##String(source, "", 0) \
DEF##Int(batch_size, 64, 0) \

struct MnistDataLayer : public Layer
{
  //vector< std::pair<std::string, vector<int> > > lines_;
  int line_id_;
  FILE* fp_;
  DataInfo info_[1];
  int64_t num_, pos_, begin_;
  uchar* buf_;
  MnistDataLayer_DEF(Def);

  // MnistDataLayer uses DataReader instead for sharing for parallelism
  virtual inline bool ShareInParallel() const { return false; }
  virtual inline const char* type() const { return "MnistData"; }
  virtual inline int ExactNumBottomBlobs() const { return 0; }
  virtual inline int MinTopBlobs() const { return 1; }
  virtual inline int MaxTopBlobs() const { return 10; }

  virtual void init() {
    MnistDataLayer_DEF(Init);
    num_ = 0;
    pos_ = 0;
    buf_ = 0;
    begin_ = 0;
  }
  virtual void fromJson(cjson* param) {
    MnistDataLayer_DEF(Get);
    fp_ = fopen(source_.c_str(), "rb");
    num_ = MAX_INT;
    size_t size = 0;
    int ret = mnist_read_head(fp_, info_);
    DataShape shape;
    shape.n = info_->n;
    shape.c = info_->c;
    shape.h = info_->h;
    shape.w = info_->w;
    DimType dimtype = info_->dimtype_;
    LOG_IF(FATAL, !(dimtype == NCHW || dimtype == NHWC));
    CHECK_NE(ret, 0) << "mnist_read_head fail";
    num_ = MIN(num_, shape.n);
    size = MAX(size, shape.c*shape.h*shape.w);
    begin_ = _ftelli64(fp_);
    buf_ = (uchar*)realloc(buf_, size);
  }
  virtual void toJson(cjson* param) {
    MnistDataLayer_DEF(Set);
  }

  MnistDataLayer() {
    init();
  }
  ~MnistDataLayer()
  {
    fclose(fp_);
  }
  virtual void LayerSetUp(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
  }
  virtual void Reshape(const vector<Blob*> & bottom, const vector<Blob*> & top) {
    CHECK_EQ(top.size(), info_->label_num+1);
    for (int i = 0; i < top.size(); ++i) {
      DataShape shape = i==0 ? dataShape(info_->n, info_->c, info_->h, info_->w) : dataShape(batch_size_, info_->label_dim[i-1], 1, 1);
      shape.n = batch_size_;
      top[i]->Reshape(shape);
    }
  }
  virtual void Forward_(const vector<Blob*> & bottom, const vector<Blob*> & top) {
    //rand skip
    for (int item_id = 0; item_id < batch_size_; ++item_id) {
      for (int i = 0; i < top.size(); ++i) {
        Blob* blob = top[i];
        Dtype* data = blob->cpu_mdata() + blob->offset(item_id);
        DataShape shape = i == 0 ? dataShape(info_->n, info_->c, info_->h, info_->w) : dataShape(batch_size_, info_->label_dim[i - 1], 1, 1);
        if (i == 0) {
          size_t size = shape.c*shape.h*shape.w;
          fread(buf_, size, 1, fp_);
          //ShowImagePal("asdf", shape.h, shape.w, buf_, shape.w*shape.c, shape.c, PF_8bppGray, NULL);          waitkey(-1);
          DataTransformer(i == 0 ? &transform_ : NULL, info_->dimtype_, shape, buf_, info_->type_, data);
        }
        else {
          fread(buf_, sizeof(float), info_->label_dim[i - 1], fp_);
          DataTransformer(NULL, NCHW, shape, buf_, TF_F32, data);
        }
      }
      ++pos_;
      if (pos_ >= num_) {
        pos_ = 0;
        _fseeki64(fp_, begin_, SEEK_SET);
      }
    }
    return;
  }
  virtual void Backward_(const vector<Blob*> & top, const vector<Blob*> & bottom) {
  }
};

INSTANTIATE_CLASS(MnistData, MnistDataLayer);


#endif  // CAFFE_MNIST_DATA_LAYER_HPP_
