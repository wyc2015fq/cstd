#ifndef CAFFE_DATA_LAYER_HPP_
#define CAFFE_DATA_LAYER_HPP_

#include "../data_transformer.hpp"
#include "../db.hpp"


#define DataParameter_DEF(DEF) \
DEF##Enum(backend, DBMethod_LMDB, DBMethod) \
DEF##String(source, "", 0) \
DEF##Int(rand_skip, 0, 0) \
DEF##Struct(transform, 0, DataTransformerInfo) \
DEF##Int(batch_size, 64, 0) \

struct DataLayer : public Layer
{
  vector< std::pair<std::string, vector<int> > > lines_;
  int line_id_;
  DB* db_;
  Cursor* cursor_;
  uint64_t offset_;
  DataParameter_DEF(Def);

  // DataLayer uses DataReader instead for sharing for parallelism
  virtual inline bool ShareInParallel() const { return false; }
  virtual inline const char* type() const { return "Data"; }
  virtual inline int ExactNumBottomBlobs() const { return 0; }
  virtual inline int MinTopBlobs() const { return 1; }
  virtual inline int MaxTopBlobs() const { return 10; }

  DataLayer() {
    DataParameter_DEF(Set);
  }
  void init(CJSON* param) {
    DataParameter_DEF(Get);
    db_ = GetDB(backend_);
    db_->Open(source_.c_str(), READ);
  }
  ~DataLayer()
  {
    delete cursor_;
    delete db_;
  }

  void Next()
  {
    cursor_->Next();
    if (!cursor_->valid()) {
      LOG_IF(INFO, root_solver())
        << "Restarting data prefetching from start.";
      cursor_->SeekToFirst();
    }
    offset_++;
  }

  bool Skip()
  {
    int size = solver_count();
    int rank = solver_rank();
    bool keep = (offset_ % size) == rank ||
      // In test mode, only rank 0 runs, so avoid skipping
      this->phase_ == TEST;
    return !keep;
  }

  // This function is called on prefetch thread
  void load_batch(const vector<Blob*> & top)
  {
    //rand skip
    if (rand_skip_ > 0) {
      unsigned int skip = rand() % rand_skip_;
      unsigned int k = 0;
      while (k < skip) {
        Next();
        k++;
      }
      rand_skip_ = 0;//skip once
    }
    Datum datum;
    for (int item_id = 0; item_id < batch_size_; ++item_id) {
      while (Skip()) {
        Next();
      }
      ParseFromString(cursor_->value().c_str(), datum);
      CHECK_LE(top.size(), datum.size());
      for (int j = 0; j < top.size(); ++j) {
        Blob* blob = top[j];
        Dtype* data = blob->cpu_mdata() + blob->offset(item_id);
        DataTransformer(j == 0 ? &transform_ : NULL, &datum[j], data);
      }
      Next();
    }
    //DLOG(INFO) << "Prefetch batch: " << batch_timer.MilliSeconds() << " ms.";
    //DLOG(INFO) << "     Read time: " << read_time / 1000 << " ms.";
    //DLOG(INFO) << "Transform time: " << trans_time / 1000 << " ms.";
  }
  
  virtual void LayerSetUp(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    cursor_ = db_->NewCursor();
  }

  virtual void Reshape(const vector<Blob*> & bottom, const vector<Blob*> & top) {
    Datum datum;
    string value = cursor_->value();
    ParseFromString(value.c_str(), datum);
    CHECK_LE(top.size(), datum.size());
    for (int j = 0; j < top.size(); ++j) {
      DataShape shape_ = datum[j].shape;
      if (j == 0) {
        shape_ = GetDataTransformerInfo(&transform_, &datum[j], phase_);
      }
      shape_.n = batch_size_;
      top[j]->Reshape(shape_);
    }
  }
  virtual void Forward_(const vector<Blob*> & bottom, const vector<Blob*> & top) {
    load_batch(top);
  }
  virtual void Backward_(const vector<Blob*> & top, const vector<Blob*> & bottom) {
  }

};

INSTANTIATE_CLASS(Data);


#endif  // CAFFE_DATA_LAYER_HPP_
