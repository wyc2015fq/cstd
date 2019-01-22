#ifdef USE_OPENCV
#include <opencv2/core/core.hpp>
#endif  // USE_OPENCV
#include <stdint.h>

#include <vector>

#include "caffe/data_transformer.hpp"
#include "caffe/layers/mnist_data_layer.hpp"
#include "caffe/util/benchmark.hpp"

namespace caffe {

template <typename Dtype>
MnistDataLayer<Dtype>::MnistDataLayer(const LayerParameter& param)
  : BasePrefetchingDataLayer<Dtype>(param),
    offset_() {
  count_ = 0;
  offset_ = 0;
  all_num_ = 0;
  source_ = param.data_param().source();
  f_ = fopen(source_.c_str(), "rb");
  rand_skip_num_ = param.data_param().rand_skip();
}

template <typename Dtype>
MnistDataLayer<Dtype>::~MnistDataLayer() {
  this->StopInternalThread();
}

static int64_t fsize64(FILE* stream)
{
  int64_t pos, size;
  pos = _ftelli64(stream);
  _fseeki64(stream, 0, SEEK_END);
  size = _ftelli64(stream);
  _fseeki64(stream, pos, SEEK_SET);
  return size;
}
template <typename Dtype>
void MnistDataLayer<Dtype>::DataLayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  const int batch_size = this->layer_param_.data_param().batch_size();
  // Read a data point, and use it to initialize the top blob.
  mnist_read_head(f_, info);
  offset_ = _ftelli64(f_);
  int64_t file_size = fsize64(f_);
  int h = info->shape.h;
  int w = info->shape.w;
  int cn = info->shape.c;
  int img_size = h*w*cn;
  int size1 = img_size;
  size_t i;
  for (i = 0; i < info->label_num; ++i) {
    size1 += info->label_dim[i] * sizeof(float);
  }
  all_num_ = (file_size - offset_)/size1;
  _fseeki64(f_, offset_, SEEK_SET);
  // Reshape top[0] and prefetch_data according to the batch_size.
  for (int j = 0; j < top.size(); ++j) {
    vector<int> top_shape;
    if (j == 0) {
      top_shape.assign(info->shape.dim, info->shape.dim + 4);
      this->transformed_data_[j].Reshape(top_shape);
      // Reshape top[0] and prefetch_data according to the batch_size.
      top_shape[0] = batch_size;
    }
    else {
      top_shape.push_back(batch_size);
      top_shape.push_back(info->label_dim[j - 1]);
      top_shape.push_back(1);
      top_shape.push_back(1);
    }
    top[j]->Reshape(top_shape);
    for (int i = 0; i < this->prefetch_.size(); ++i) {
      this->prefetch_[i]->data_[j].Reshape(top_shape);
    }
  }
  LOG_IF(INFO, Caffe::root_solver())
      << "output data size: " << top[0]->num() << ","
      << top[0]->channels() << "," << top[0]->height() << ","
      << top[0]->width();
  // label
}

template <typename Dtype>
bool MnistDataLayer<Dtype>::Skip() {
  return false;
}

template<typename Dtype>
void MnistDataLayer<Dtype>::Next() {
}

// This function is called on prefetch thread
template<typename Dtype>
void MnistDataLayer<Dtype>::load_batch(Batch<Dtype>* batch) {
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
  size_t img_size = info->shape.w*info->shape.h*info->shape.c;
  size_t lab_size = 0;
  size_t i;
  for (i = 0; i < info->label_num; ++i) {
    lab_size += info->label_dim[i];
  }
  unsigned char* pixels = (unsigned char*)malloc(img_size);
  float* labels = (float*)malloc(lab_size * sizeof(float));

  for (i = 0; i<info->label_num+1; ++i) {
    datum.add_blob();
  }
  for (int item_id = 0; item_id < batch_size; ++item_id) {
    timer.Start();
    while (Skip()) {
      Next();
    }
    {
      if (count_ >= all_num_) {
        count_ = 0;
        _fseeki64(f_, offset_, SEEK_SET);
        LOG_IF(INFO, Caffe::root_solver())
          << "Restarting data prefetching from start.";
      }
      //datum.ParseFromString(cursor_->value());
      fread(pixels, 1, img_size, f_);
      fread(labels, sizeof(float), lab_size, f_);
      ++count_;
      int labels_off = 0;
      DCHECK_GE(*labels, 0);
      //datum.
      for (i = 0; i<info->label_num+1; ++i) {
        BlobData* blob = datum.mutable_blob(i);
        if (0 == i) {
          Blob_NCHW(blob, false, pixels, info->shape.w, info->shape.h, info->shape.c, 1);
        }
        else {
          Blob_NCHW(blob, false, labels+ labels_off, 1, 1, info->label_dim[i-1], 1);
          labels_off += info->label_dim[i - 1];
        }
      }
    }
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
      if (0 == j) {
        this->data_transformer_->Transform(datum.blob(j), this->transformed_data_ + j);
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
  free(pixels);
  free(labels);
  //DLOG(INFO) << "Prefetch batch: " << batch_timer.MilliSeconds() << " ms.";
  //DLOG(INFO) << "     Read time: " << read_time / 1000 << " ms.";
  //DLOG(INFO) << "Transform time: " << trans_time / 1000 << " ms.";
}

INSTANTIATE_CLASS(MnistDataLayer);
REGISTER_LAYER_CLASS(MnistData);

}  // namespace caffe
