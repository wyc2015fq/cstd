

#include "hdf5.h"
#include "hdf5_hl.h"

#include "caffe/layers/hdf5_output_layer.hpp"

namespace {

template <typename Dtype>
void HDF5OutputLayer::Forward(GPUContext* context, const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  CHECK_GE(bottom.size(), 2);
  CHECK_EQ(bottom[0]->num(), bottom[1]->num());
  data_blob_.Reshape(bottom[0]->num(), bottom[0]->channels(),
                     bottom[0]->height(), bottom[0]->width());
  label_blob_.Reshape(bottom[1]->num(), bottom[1]->channels(),
                     bottom[1]->height(), bottom[1]->width());
  const int data_datum_dim = bottom[0]->count() / bottom[0]->num();
  const int label_datum_dim = bottom[1]->count() / bottom[1]->num();

  for (int i = 0; i < bottom[0]->num(); ++i) {
    caffe_copy(data_datum_dim, &bottom[0]->data()[i * data_datum_dim],
        &data_blob_.mdata()[i * data_datum_dim]);
    caffe_copy(label_datum_dim, &bottom[1]->data()[i * label_datum_dim],
        &label_blob_.mdata()[i * label_datum_dim]);
  }
  SaveBlobs();
}

template <typename Dtype>
void HDF5OutputLayer::Backward(GPUContext* context, const vector<Blob*>& top,
      const vector<Blob*>& bottom) {
  return;
}

INSTANTIATE_LAYER_GPU_FUNCS(HDF5OutputLayer);

}  // namespace
