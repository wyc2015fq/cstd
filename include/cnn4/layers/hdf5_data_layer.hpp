#ifndef CAFFE_HDF5_DATA_LAYER_HPP_
#define CAFFE_HDF5_DATA_LAYER_HPP_

#include "hdf5.h"

#include <string>






#include "caffe/layers/base_data_layer.hpp"

namespace
{

  /**
   * @brief Provides data to the Net from HDF5 files.
   *
   * TODO(dox): thorough documentation for Forward and proto params.
   */
  template <typename Dtype>
  class HDF5DataLayer : public Layer
  {
  public:
    explicit HDF5DataLayer()
      : Layer() {}
    virtual ~HDF5DataLayer() {}
    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top);
    // Data layers should be shared by multiple solvers in parallel
    virtual inline bool ShareInParallel() const { return true; }
    // Data layers have no bottoms, so reshaping is trivial.
    virtual void Reshape(const vector<Blob*> & bottom,
                         const vector<Blob*> & top) {}

    virtual inline const char* type() const { return "HDF5Data"; }
    virtual inline int ExactNumBottomBlobs() const { return 0; }
    virtual inline int MinTopBlobs() const { return 1; }

  public:
    virtual void Forward(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Forward(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Backward(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom) {}
    virtual void Backward(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom) {}
    virtual void LoadHDF5FileData(const char* filename);

    std::vector<std::string> hdf_filenames_;
    unsigned int num_files_;
    unsigned int current_file_;
    hsize_t current_row_;
    std::vector<SHARED_PTR<Blob > > hdf_blobs_;
    std::vector<unsigned int> data_permutation_;
    std::vector<unsigned int> file_permutation_;
  };

}  // namespace

#endif  // CAFFE_HDF5_DATA_LAYER_HPP_
