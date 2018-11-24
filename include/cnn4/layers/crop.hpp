#ifndef CAFFE_CROP_LAYER_HPP_
#define CAFFE_CROP_LAYER_HPP_

#include <utility>






namespace
{

  /**
   * @brief Takes a Blob and crop it, to the shape specified by the second input
   *  Blob, across all dimensions after the specified axis.
   *
   * TODO(dox): thorough documentation for Forward_, Backward_, and proto params.
   */

  template <typename Dtype>
  class CropLayer : public Layer
  {
  public:
    explicit CropLayer()
      : Layer() {}
    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top);
    virtual void Reshape(const vector<Blob*> & bottom,
                         const vector<Blob*> & top);

    virtual inline const char* type() const { return "Crop"; }
    virtual inline int ExactNumBottomBlobs() const { return 2; }
    virtual inline int ExactNumTopBlobs() const { return 1; }

  public:
    virtual void Forward_(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Backward_(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
    virtual void Forward_(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Backward_(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);

    vector<int> offsets;

  private:
    // Recursive copy function.
    void crop_copy(const vector<Blob*> & bottom,
                   const vector<Blob*> & top,
                   const vector<int> & offsets,
                   vector<int> indices,
                   int cur_dim,
                   const Dtype* src_data,
                   Dtype* dest_data,
                   bool is_forward);

    // Recursive copy function: this is similar to crop_copy() but loops over all
    // but the last two dimensions to allow for ND cropping while still relying on
    // a CUDA kernel for the innermost two dimensions for performance reasons.  An
    // alterantive implementation could rely on the kernel more by passing
    // offsets, but this is problematic because of its variable length.
    // Since in the standard (N,C,W,H) case N,C are usually not cropped a speedup
    // could be achieved by not looping the application of the copy_kernel around
    // these dimensions.
    void crop_copy_gpu(const vector<Blob*> & bottom,
                       const vector<Blob*> & top,
                       const vector<int> & offsets,
                       vector<int> indices,
                       int cur_dim,
                       const Dtype* src_data,
                       Dtype* dest_data,
                       bool is_forward);
  };
}  // namespace

#endif  // CAFFE_CROP_LAYER_HPP_
