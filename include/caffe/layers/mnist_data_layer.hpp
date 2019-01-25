#ifndef CAFFE_MNIST_DATA_LAYER_HPP_
#define CAFFE_MNIST_DATA_LAYER_HPP_

#include <vector>

#include "caffe/blob.hpp"
#include "caffe/data_transformer.hpp"
#include "caffe/internal_thread.hpp"
#include "caffe/layer.hpp"
#include "caffe/layers/base_data_layer.hpp"
#include "caffe/proto/caffe.pb.h"
#include "std/mnistdata_c.h"

namespace caffe {

template <typename Dtype>
class MnistDataLayer : public BasePrefetchingDataLayer<Dtype> {
 public:
  explicit MnistDataLayer(const LayerParameter& param);
  virtual ~MnistDataLayer();
  virtual void DataLayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  // MnistDataLayer uses DataReader instead for sharing for parallelism
  virtual inline bool ShareInParallel() const { return false; }
  virtual inline const char* type() const { return "MnistData"; }
  virtual inline int ExactNumBottomBlobs() const { return 0; }
  virtual inline int MinTopBlobs() const { return 1; }
  virtual inline int MaxTopBlobs() const { return 2; }

  int line_id_;
  DataInfo info[1];
  string source_;

 protected:
  void Next();
  bool Skip();
  virtual void load_batch(Batch<Dtype>* batch);

  uint64_t offset_;
  uint64_t count_;
  uint64_t all_num_;
  FILE* f_;

  unsigned int rand_skip_num_;
};

}  // namespace caffe

#endif  // CAFFE_DATA_LAYER_HPP_
