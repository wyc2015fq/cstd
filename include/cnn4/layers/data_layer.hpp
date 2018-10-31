#ifndef CAFFE_DATA_LAYER_HPP_
#define CAFFE_DATA_LAYER_HPP_

#include "../data_transformer.hpp"
#include "../db.hpp"


template <typename Dtype>
struct DataLayer : public Layer<Dtype>
{
  vector< std::pair<std::string, vector<int> > > lines_;
  int line_id_;
  DB* db_;
  Cursor* cursor_;
  uint64_t offset_;

  unsigned int rand_skip_num_;
  // DataLayer uses DataReader instead for sharing for parallelism
  virtual inline bool ShareInParallel() const { return false; }
  virtual inline const char* type() const { return "Data"; }
  virtual inline int ExactNumBottomBlobs() const { return 0; }
  virtual inline int MinTopBlobs() const { return 1; }
  virtual inline int MaxTopBlobs() const { return 10; }

  void init() {
  }

  ~DataLayer()
  {
    delete cursor_;
    delete db_;
  }

  int batch_size_;

  void Next()
  {
    cursor_->Next();
    if (!cursor_->valid()) {
      LOG_IF(INFO, Caffe::root_solver())
        << "Restarting data prefetching from start.";
      cursor_->SeekToFirst();
    }
    offset_++;
  }

  bool Skip()
  {
    int size = Caffe::solver_count();
    int rank = Caffe::solver_rank();
    bool keep = (offset_ % size) == rank ||
      // In test mode, only rank 0 runs, so avoid skipping
      this->layer_param_.phase() == TEST;
    return !keep;
  }

  // This function is called on prefetch thread
  void load_batch(vector<Blob<Dtype>*> & top)
  {
    //rand skip
    if (rand_skip_num_ > 0) {
      unsigned int skip = rand() % rand_skip_num_;
      unsigned int k = 0;
      while (k < skip) {
        Next();
        k++;
      }
      rand_skip_num_ = 0;//skip once
    }
    Datum datum;
    for (int item_id = 0; item_id < batch_size_; ++item_id) {
      while (Skip()) {
        Next();
      }
      ParseFromString(cursor_->value().c_str(), datum);
      int top_size = MIN(top.size(), datum.size());
      for (int j = 0; j < top_size; ++j) {
        Blob<Dtype>* blob = top[j];
        Dtype* data = blob->mutable_data<CPUContext>() + blob->offset(item_id);
        DataTransformer(info, &datum[j], data);
      }
      Next();
    }
    timer.Stop();
    batch_timer.Stop();
    //DLOG(INFO) << "Prefetch batch: " << batch_timer.MilliSeconds() << " ms.";
    //DLOG(INFO) << "     Read time: " << read_time / 1000 << " ms.";
    //DLOG(INFO) << "Transform time: " << trans_time / 1000 << " ms.";
  }
  DataTransformerInfo info[1];
  void LayerSetUp(const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top)
  {
    db_ = GetDB(param_->GetObjectString("backend", "lmdb"));
    db_->Open(param_->GetObjectString("source", NULL), READ);
    cursor_ = db_->NewCursor();
    rand_skip_num_ = param_->GetObjectInt("rand_skip", 0);
    batch_size_ = this->param_->GetObjectInt("batch_size", 64);
  }

  virtual void Reshape(const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top) {
    Datum datum;
    ParseFromString(cursor_->value().c_str(), datum);
    for (int j = 0; j < top_size; ++j) {
      GetDataTransformerInfo(info, &datum[j], param_, phase_, batch_size_);
      top[j]->Reshape(info->shape_);
    }
  }
  virtual void Forward(Context* context, const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top) {
    load_batch(top);
  }
  virtual void Backward(Context* context, const vector<Blob<Dtype>*> & top, const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom) {
  }

};


#endif  // CAFFE_DATA_LAYER_HPP_
