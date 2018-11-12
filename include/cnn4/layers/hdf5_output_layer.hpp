#ifndef CAFFE_HDF5_OUTPUT_LAYER_HPP_
#define CAFFE_HDF5_OUTPUT_LAYER_HPP_

#include "hdf5.h"

#include <string>






namespace
{

#define HDF5_DATA_DATASET_NAME "data"
#define HDF5_DATA_LABEL_NAME "label"

  /**
   * @brief Write blobs to disk as HDF5 files.
   *
   * TODO(dox): thorough documentation for Forward and proto params.
   */
  template <typename Dtype>
  class HDF5OutputLayer : public Layer
  {
  public:
    explicit HDF5OutputLayer()
      : Layer(param), file_opened_(false) {}
    virtual ~HDF5OutputLayer();
    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top);
    // Data layers should be shared by multiple solvers in parallel
    virtual inline bool ShareInParallel() const { return true; }
    // Data layers have no bottoms, so reshaping is trivial.
    virtual void Reshape(const vector<Blob*> & bottom,
                         const vector<Blob*> & top) {}

    virtual inline const char* type() const { return "HDF5Output"; }
    // TODO: no limit on the number of blobs
    virtual inline int ExactNumBottomBlobs() const { return 2; }
    virtual inline int ExactNumTopBlobs() const { return 0; }

    inline std::string file_name() const { return file_name_; }

  public:
    virtual void Forward(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Forward(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Backward(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
    virtual void Backward(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
    virtual void SaveBlobs();

    bool file_opened_;
    std::string file_name_;
    hid_t file_id_;
    Blob data_blob_;
    Blob label_blob_;
  };

}  // namespace

#endif  // CAFFE_HDF5_OUTPUT_LAYER_HPP_
