#ifndef CAFFE_IMAGE_DATA_LAYER_HPP_
#define CAFFE_IMAGE_DATA_LAYER_HPP_





#include "caffe/blob.hpp"
#include "caffe/data_transformer.hpp"
#include "caffe/internal_thread.hpp"
#include "caffe/layer.hpp"
#include "base_data_layer.hpp"




/**
 * @brief Provides data to the Net from image files.
 *
 * TODO(dox): thorough documentation for Forward and proto params.
 */

class ImageDataLayer : public BasePrefetchingDataLayer {
 public:
  explicit ImageDataLayer(const LayerParameter& param)
      : BasePrefetchingDataLayer(param) {}
  virtual ~ImageDataLayer();
  virtual void DataLayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "ImageData"; }
  virtual inline int ExactNumBottomBlobs() const { return 0; }
  virtual inline int ExactNumTopBlobs() const { return -1; }
  virtual inline int MaxTopBlobs() const { return 3; }

 protected:
  shared_ptr<Caffe::RNG> prefetch_rng_;
  virtual void ShuffleImages();
  virtual void load_batch(Batch* batch);

  vector<std::pair<std::string, int> > lines_;
  int lines_id_;
  vector<int> num_samples_;
  vector class_weights_;
  bool balance_;
  vector<vector<std::pair<std::string, int> > > filename_by_class_;
  int class_id_;
};




#endif  // CAFFE_IMAGE_DATA_LAYER_HPP_
