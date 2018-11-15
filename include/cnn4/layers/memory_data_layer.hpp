#ifndef CAFFE_MEMORY_DATA_LAYER_HPP_
#define CAFFE_MEMORY_DATA_LAYER_HPP_







#include "caffe/layers/base_data_layer.hpp"

namespace
{

  /**
   * @brief Provides data to the Net from memory.
   *
   * TODO(dox): thorough documentation for Forward_ and proto params.
   */
  template <typename Dtype>
  class MemoryDataLayer : public BaseDataLayer
  {
  public:
    explicit MemoryDataLayer()
      : BaseDataLayer(param), has_new_data_(false) {}
    virtual void DataLayerSetUp(const vector<Blob*> & bottom,
                                const vector<Blob*> & top);

    virtual inline const char* type() const { return "MemoryData"; }
    virtual inline int ExactNumBottomBlobs() const { return 0; }
    virtual inline int ExactNumTopBlobs() const { return 2; }

    virtual void AddDatumVector(const vector<Datum> & datum_vector);
#ifdef USE_OPENCV
    virtual void AddMatVector(const vector<cv::Mat> & mat_vector,
                              const vector<int> & labels);
#endif  // USE_OPENCV

    // Reset should accept const pointers, but can't, because the memory
    //  will be given to Blob, which is mutable
    void Reset(Dtype* data, Dtype* label, int n);
    void set_batch_size(int new_size);

    int batch_size() { return batch_size_; }
    int channels() { return channels_; }
    int height() { return height_; }
    int width() { return width_; }

  public:
    virtual void Forward_(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);

    int batch_size_, channels_, height_, width_, size_, label_size_;
    Dtype* data_;
    Dtype* labels_;
    int n_;
    size_t pos_;
    Blob added_data_;
    Blob added_label_;
    bool has_new_data_;
  };

}  // namespace

#endif  // CAFFE_MEMORY_DATA_LAYER_HPP_
