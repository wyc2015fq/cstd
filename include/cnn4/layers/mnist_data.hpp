#ifndef CAFFE_MNIST_DATA_LAYER_HPP_
#define CAFFE_MNIST_DATA_LAYER_HPP_

#include "std/types_c.h"
#include "std/algo.h"
#include "../data_transformer.hpp"
#include "../blobdata.h"

#define MnistDataLayer_DEF(DEF) \
DEF##Int(rand_skip, 0, 0) \
DEF##Struct(transform, 0, DataTransformerInfo) \
DEF##Int(batch_size, 64, 0) \

struct MnistDataLayer : public Layer
{
  //vector< std::pair<std::string, vector<int> > > lines_;
  int line_id_;
  vector<string> source_;
  vector<FILE*> fp_;
  vector<DataInfo> datainfo_;
  vector<size_t> begin_;
  size_t num_, pos_;
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
  }
  virtual void fromJson(cjson* param) {
    MnistDataLayer_DEF(Get);
    cjson_GetObjectStringArray(param, "source", source_);
    size_t i, n = source_.size();
    fp_.resize(n);
    begin_.resize(n);
    datainfo_.resize(n);
    num_ = MAX_INT;
    size_t size = 0;
    for (size_t i = 0; i < n; ++i) {
      FILE* pf = fopen(source_[i].c_str(), "rb");
      fp_[i] = pf;
      int ret = mnist_read_head(pf, &datainfo_[i]);
      DataShape shape = datainfo_[i].shape;
      DimType dimtype = datainfo_[i].dimtype_;
      LOG_IF(FATAL, !(dimtype == NCHW || dimtype==NHWC));
      CHECK_NE(ret, 0) << "mnist_read_head fail";
      num_ = MIN(num_, shape.n);
      size = MAX(size, shape.c*shape.h*shape.w);
      begin_[i] = ftell(pf);
    }
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
    for (size_t i = 0; i < fp_.size(); ++i) {
      fclose(fp_[i]);
    }
  }

  virtual void LayerSetUp(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
  }

  virtual void Reshape(const vector<Blob*> & bottom, const vector<Blob*> & top) {
    CHECK_EQ(top.size(), datainfo_.size());
    for (int i = 0; i < top.size(); ++i) {
      DataShape shape = datainfo_[i].shape;
      shape.n = batch_size_;
      top[i]->Reshape(shape);
    }
  }
  virtual void Forward_(const vector<Blob*> & bottom, const vector<Blob*> & top) {
    //rand skip
    CHECK_LE(top.size(), datainfo_.size());
    for (int item_id = 0; item_id < batch_size_; ++item_id) {
      for (int i = 0; i < top.size(); ++i) {
        Blob* blob = top[i];
        Dtype* data = blob->cpu_mdata() + blob->offset(item_id);
        DataShape shape = datainfo_[i].shape;
        DimType dimtype = datainfo_[i].dimtype_;
        TypeFlag type = datainfo_[i].type_;
        size_t size = shape.c*shape.h*shape.w;
        fread(buf_, size, 1, fp_[i]);
        DataTransformer(i == 0 ? &transform_ : NULL, dimtype, shape, buf_, type, data);
      }
      ++pos_;
      if (pos_ >= num_) {
        pos_ = 0;
        for (int i = 0; i < fp_.size(); ++i) {
          fseek(fp_[i], begin_[i], SEEK_SET);
        }
      }
    }
    return;
  }
  virtual void Backward_(const vector<Blob*> & top, const vector<Blob*> & bottom) {
  }

};

INSTANTIATE_CLASS(MnistData, MnistDataLayer);


#endif  // CAFFE_MNIST_DATA_LAYER_HPP_
