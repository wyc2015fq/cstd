#ifdef USE_OPENCV
#include <opencv2/core/core.hpp>
#endif  // USE_OPENCV
#include <stdint.h>

#include <vector>

#include "caffe/data_transformer.hpp"
#include "caffe/layers/data_layer.hpp"
#include "caffe/util/benchmark.hpp"

namespace caffe
{

  template <typename Dtype>
  DataLayer<Dtype>::DataLayer(const LayerParameter & param)
    : BasePrefetchingDataLayer<Dtype>(param),
      offset_()
  {
    db_.reset(db::GetDB(param.data_param().backend()));
    db_->Open(param.data_param().source(), db::READ);
    cursor_.reset(db_->NewCursor());
    rand_skip_num_ = param.data_param().rand_skip();
  }

  template <typename Dtype>
  DataLayer<Dtype>::~DataLayer()
  {
    this->StopInternalThread();
  }

  template <typename Dtype>
  void DataLayer<Dtype>::DataLayerSetUp(const vector<Blob<Dtype>*> & bottom,
                                        const vector<Blob<Dtype>*> & top)
  {
    const int batch_size = this->layer_param_.data_param().batch_size();
    // Read a data point, and use it to initialize the top blob.
    Datum datum;
    datum.ParseFromString(cursor_->value());
    LOG_IF(INFO, top.size() > datum.blob_size()) << " top.size()> datum.blob_size()";
    // Use data_transformer to infer the expected blob shape from datum.
    for (int i = 0; i < this->prefetch_.size(); ++i) {
      //this->prefetch_[i]->data_.resize(top.size());
      for (int j = 0; j < top.size(); ++j) {
        vector<int> top_shape = this->data_transformer_->InferBlobShape(datum.blob(j));
        this->transformed_data_[j].Reshape(top_shape);
        // Reshape top[0] and prefetch_data according to the batch_size.
        top_shape[0] = batch_size;
        top[j]->Reshape(top_shape);
        this->prefetch_[i]->data_[j].Reshape(top_shape);
      }
    }
    
    LOG_IF(INFO, Caffe::root_solver())
        << "output data size: " << top[0]->num() << ","
        << top[0]->channels() << "," << top[0]->height() << ","
        << top[0]->width();
  }

  template <typename Dtype>
  bool DataLayer<Dtype>::Skip()
  {
    int size = Caffe::solver_count();
    int rank = Caffe::solver_rank();
    bool keep = (offset_ % size) == rank ||
                // In test mode, only rank 0 runs, so avoid skipping
                this->layer_param_.phase() == TEST;
    return !keep;
  }

  template<typename Dtype>
  void DataLayer<Dtype>::Next()
  {
    cursor_->Next();
    if (!cursor_->valid()) {
      LOG_IF(INFO, Caffe::root_solver())
          << "Restarting data prefetching from start.";
      cursor_->SeekToFirst();
    }
    offset_++;
  }

// This function is called on prefetch thread
  template<typename Dtype>
  void DataLayer<Dtype>::load_batch(Batch<Dtype>* batch)
  {
    CPUTimer batch_timer;
    batch_timer.Start();
    double read_time = 0;
    double trans_time = 0;
    CPUTimer timer;
    //for (int j=0; j<batch->data_.size(); ++j) {      CHECK(batch->data_[j].count());    }
    //CHECK(this->transformed_data_.count());
    const int batch_size = this->layer_param_.data_param().batch_size();
    //rand skip
    if (rand_skip_num_ > 0) {
      unsigned int skip = caffe_rng_rand() % rand_skip_num_;
      unsigned int k = 0;
      while (k < skip) {
        Next();
        k++;
      }
      LOG_IF(INFO, Caffe::root_solver())
          << "skip " << skip;
      rand_skip_num_ = 0;//skip once
    }
    Datum datum;
    batch->data_size = datum.blob_size();
    for (int item_id = 0; item_id < batch_size; ++item_id) {
      timer.Start();
      while (Skip()) {
        Next();
      }
      datum.ParseFromString(cursor_->value());
      read_time += timer.MicroSeconds();
      for (int j = 0; j < datum.blob_size(); ++j) {
        if (item_id == 0) {
          // Reshape according to the first datum of each batch
          // on single input batches allows for inputs of varying dimension.
          // Use data_transformer to infer the expected blob shape from datum.
          vector<int> top_shape = this->data_transformer_->InferBlobShape(datum.blob(j));
          this->transformed_data_[j].Reshape(top_shape);
          // Reshape batch according to the batch_size.
          top_shape[0] = batch_size;
          batch->data_[j].Reshape(top_shape);
        }
        // Apply data transformations (mirror, scale, crop...)
        timer.Start();
        int offset = batch->data_[j].offset(item_id);
        Dtype* top_data = batch->data_[j].mutable_cpu_data();
        this->transformed_data_[j].set_cpu_data(top_data + offset);
        if (0==j) {
          this->data_transformer_->Transform(datum.blob(j), this->transformed_data_+j);
        }
        else {
          Dtype* transformed_data = this->transformed_data_[j].mutable_cpu_data();
          blob_data_copy(transformed_data, datum.blob(j));
        }
        // Copy label.
      }
      trans_time += timer.MicroSeconds();
      Next();
    }
    timer.Stop();
    batch_timer.Stop();
    //DLOG(INFO) << "Prefetch batch: " << batch_timer.MilliSeconds() << " ms.";
    //DLOG(INFO) << "     Read time: " << read_time / 1000 << " ms.";
    //DLOG(INFO) << "Transform time: " << trans_time / 1000 << " ms.";
  }

  INSTANTIATE_CLASS(DataLayer);
  REGISTER_LAYER_CLASS(Data);

}  // namespace caffe
