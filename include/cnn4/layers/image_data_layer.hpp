#ifndef CAFFE_IMAGE_DATA_LAYER_HPP_
#define CAFFE_IMAGE_DATA_LAYER_HPP_

#include <string>
#include <utility>



#include "caffe/data_transformer.hpp"
#include "caffe/internal_thread.hpp"

#include "caffe/layers/base_data_layer.hpp"


namespace
{

  /**
   * @brief Provides data to the Net from image files.
   *
   * TODO(dox): thorough documentation for Forward and proto params.
   */
  template <typename Dtype>
  class ImageDataLayer : public BasePrefetchingDataLayer<Dtype>
  {
  public:
    explicit ImageDataLayer()
      : BasePrefetchingDataLayer<Dtype>() {}
    virtual ~ImageDataLayer();
    virtual void DataLayerSetUp(const vector<Blob<Dtype>*> & bottom,
                                const vector<Blob<Dtype>*> & top);

    virtual inline const char* type() const { return "ImageData"; }
    virtual inline int ExactNumBottomBlobs() const { return 0; }
    virtual inline int ExactNumTopBlobs() const { return 2; }

  public:
    virtual void ShuffleImages();
    virtual void load_batch(Batch<Dtype>* batch);

    vector<std::pair<std::string, std::vector<int> > > lines_;
    vector<std::pair<std::string, std::vector<float> > > regression_lines_;
    int lines_id_;
  };


}  // namespace

#endif  // CAFFE_IMAGE_DATA_LAYER_HPP_
