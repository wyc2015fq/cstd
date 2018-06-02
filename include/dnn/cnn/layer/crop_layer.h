



/**
 * @brief Takes a Blob and crop it, to the shape specified by the second input
 *  Blob, across all dimensions after the specified axis.
 *
 * TODO(dox): thorough documentation for Forward, Backward, and proto params.
 */


class CropLayer : public Layer {
 public:
  explicit CropLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "Crop"; }
  virtual inline int ExactNumBottomBlobs() const { return 2; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

 protected:
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  Blob<int> offsets;
  Blob<int> src_strides_;
  Blob<int> dest_strides_;

 private:
  // Recursive copy function.
  void crop_copy(const vector<Blob*>& bottom,
               const vector<Blob*>& top,
               const int* offsets,
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
  void crop_copy_gpu(const vector<Blob*>& bottom,
                const vector<Blob*>& top,
                const vector<int>& offsets,
                vector<int> indices,
                int cur_dim,
                const Dtype* src_data,
                Dtype* dest_data,
                bool is_forward);
};





void CropLayer::LayerSetUp(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  // LayerSetup() handles the number of dimensions; Reshape() handles the sizes.
  // bottom[0] supplies the data
  // bottom[1] supplies the size
  const CropParameter& param = this->layer_param_.crop_param();
  CHECK_EQ(layer->i_size, 2) << "Wrong number of bottom blobs.";
  int input_dim = bottom[0]->num_axes();
  const int start_axis = bottom[0]->CanonicalAxisIndex(param.axis());
  CHECK_LT(start_axis, input_dim) << "crop axis bigger than input dim";
  if (param.offset_size() > 1) {
    // the number of crop values specified must be equal to the number
    // of dimensions following axis
    CHECK_EQ(start_axis + param.offset_size(), input_dim)
      << "number of offset values specified must be equal to the number of "
      << "dimensions following axis.";
  }
}


void CropLayer::Reshape(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const CropParameter& param = this->layer_param_.crop_param();
  int input_dim = bottom[0]->num_axes();
  const int start_axis = bottom[0]->CanonicalAxisIndex(param.axis());

  // Initialize offsets to 0 and the new shape to the current shape of the data.
  vector<int> new_shape(bottom[0]->shape());
  vector<int> offsets_shape(1, input_dim);
  offsets.Reshape(offsets_shape);
  int* offset_data = offsets;

  // Determine crop offsets and the new shape post-crop.
  for (i = 0; i < input_dim; ++i) {
    int crop_offset = 0;
    int new_size = bottom[0]->shape(i);
    if (i >= start_axis) {
      new_size = bottom[1]->shape(i);
      if (param.offset_size() == 1) {
        // If only one offset is given, all crops have the same offset.
        crop_offset = param.offset(0);
      } else if (param.offset_size() > 1) {
        // For several offsets, the number of offsets must be equal to the
        // number of dimensions to crop, that is dimensions after the axis.
        crop_offset = param.offset(i - start_axis);
      }
      // Check that the crop and offset are within the dimension's bounds.
      CHECK_GE(bottom[0]->shape(i) - crop_offset, bottom[1]->shape(i))
          << "the crop for dimension " << i << " is out-of-bounds with "
          << "size " << bottom[1]->shape(i) << " and offset " << crop_offset;
    }
    new_shape[i] = new_size;
    offset_data[i] = crop_offset;
  }
  top[0]->Reshape(new_shape);
  // Compute strides
  src_strides_.Reshape(offsets_shape);
  dest_strides_.Reshape(offsets_shape);
  for (i = 0; i < input_dim; ++i) {
    src_strides_[i] = bottom[0]->count(i + 1, input_dim);
    dest_strides_[i] = top[0]->count(i + 1, input_dim);
  }
}


void CropLayer::crop_copy(const vector<Blob*>& bottom,
             const vector<Blob*>& top,
             const int* offsets,
             vector<int> indices,
             int cur_dim,
             const Dtype* src_data,
             Dtype* dest_data,
             bool is_forward) {
  if (cur_dim + 1 < top[0]->num_axes()) {
    // We are not yet at the final dimension, call copy recursively
    for (i = 0; i < top[0]->shape(cur_dim); ++i) {
      indices[cur_dim] = i;
      crop_copy(bottom, top, offsets, indices, cur_dim+1,
                src_data, dest_data, is_forward);
    }
  } else {
    // We are at the last dimensions, which is stored continuously in memory
    // prepare index vector reduced(red) and with offsets(off)
    std::vector<int> ind_red(cur_dim, 0);
    std::vector<int> ind_off(cur_dim+1, 0);
    for (j = 0; j < cur_dim; ++j) {
      ind_red[j] = indices[j];
      ind_off[j] = indices[j] + offsets[j];
    }
    ind_off[cur_dim] = offsets[cur_dim];
    // do the copy
    if (is_forward) {
      blas_scopy(top[0]->shape(cur_dim),
          src_data + bottom[0]->offset(ind_off),
          dest_data + top[0]->offset(ind_red));
    } else {
      // in the backwards pass the src_data is nextDiff
      // and the dest_data is prevDiff
      blas_scopy(top[0]->shape(cur_dim),
          src_data + top[0]->offset(ind_red),
          dest_data + bottom[0]->offset(ind_off));
    }
  }
}


void CropLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  std::vector<int> indices(top[0]->num_axes(), 0);
  const Dtype* prevData = prevData;
  Dtype* nextData = nextData;
  crop_copy(bottom, top, offsets.cpu_data(), indices, 0, prevData, nextData,
      true);
}


void CropLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  const Dtype* nextDiff = nextDiff;
  Dtype* prevDiff = bottom[0]->mutable_diff();

  if (propagate_down[0]) {
    blas_sset(Data_count(prev->size), static_cast(0), prevDiff);
    std::vector<int> indices(top[0]->num_axes(), 0);
    crop_copy(bottom, top, offsets.cpu_data(), indices, 0, nextDiff,
        prevDiff, false);
  }
}

#ifdef CPU_ONLY
STUB_GPU(CropLayer);
#endif

INSTANTIATE_CLASS(CropLayer);
REGISTER_LAYER_CLASS(Crop);


