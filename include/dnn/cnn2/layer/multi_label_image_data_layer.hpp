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
  
  class MultiLabelImageDataLayer : public BasePrefetchingDataLayer {
  public:
    explicit MultiLabelImageDataLayer(const LayerParameter& param)
      : BasePrefetchingDataLayer(param) {}
    virtual ~MultiLabelImageDataLayer();
    virtual void DataLayerSetUp(const vector<Blob*>& bottom,
                                const vector<Blob*>& top);

    virtual inline const char* type() const { return "MultiLabelImageData"; }
    virtual inline int ExactNumBottomBlobs() const { return 0; }
    virtual inline int ExactNumTopBlobs() const { return 2; }

  protected:
    shared_ptr<Caffe::RNG> prefetch_rng_;
    virtual void ShuffleImages();
    virtual void load_batch(Batch* batch);

    vector<std::pair<std::string, shared_ptr<vector > > > lines_;
    int label_count;
    int lines_id_;
    bool balance_;
    int balance_by_;
    vector<int> num_samples_;
    vector class_weights_;
    vector<vector<std::pair<std::string, shared_ptr<vector > > > > filename_by_class_;
    int class_id_;
    int label_cut_start_;
    int label_cut_end_;
  };



#endif  // CAFFE_IMAGE_DATA_LAYER_HPP_
