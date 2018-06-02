#ifndef CAFFE_WINDOW_DATA_LAYER_HPP_
#define CAFFE_WINDOW_DATA_LAYER_HPP_





#include "caffe/blob.hpp"
#include "caffe/data_transformer.hpp"
#include "caffe/internal_thread.hpp"
#include "caffe/layer.hpp"
#include "base_data_layer.hpp"




/**
 * @brief Provides data to the Net from windows of images files, specified
 *        by a window data file. This layer is *DEPRECATED* and only kept for
 *        archival purposes for use by the original R-CNN.
 *
 * TODO(dox): thorough documentation for Forward and proto params.
 */

class WindowDataLayer : public BasePrefetchingDataLayer {
 public:
  explicit WindowDataLayer(const LayerParameter& param)
      : BasePrefetchingDataLayer(param) {}
  virtual ~WindowDataLayer();
  virtual void DataLayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "WindowData"; }
  virtual inline int ExactNumBottomBlobs() const { return 0; }
  virtual inline int ExactNumTopBlobs() const { return 2; }

 protected:
  virtual unsigned int PrefetchRand();
  virtual void load_batch(Batch* batch);

  shared_ptr<Caffe::RNG> prefetch_rng_;
  vector<std::pair<std::string, vector<int> > > image_database_;
  enum WindowField { IMAGE_INDEX, LABEL, OVERLAP, X1, Y1, X2, Y2, NUM };
  vector<vector<float> > fg_windows_;
  vector<vector<float> > bg_windows_;
  Blob data_mean_;
  vector mean_values_;
  bool has_mean_file_;
  bool has_mean_values_;
  bool cache_images_;
  vector<std::pair<std::string, Datum > > image_database_cache_;
};



#endif  // CAFFE_WINDOW_DATA_LAYER_HPP_
