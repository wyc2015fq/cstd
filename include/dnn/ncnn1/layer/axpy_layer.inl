

/**
 * @brief For reduce memory and time both on training and testing, we combine
 *        channel-wise scale operation and element-wise addition operation 
 *        into a single layer called "axpy".
 *       
 */

class AxpyLayer: public Layer {
 public:
  explicit AxpyLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {}
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "Axpy"; }
  virtual inline int ExactNumBottomBlobs() const { return 3; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

 protected:
/**
 * @param Formulation:
 *            F = a * X + Y
 *	  Shape info:
 *            a:  N x C          --> bottom[0]      
 *            X:  N x C x H x W  --> bottom[1]       
 *            Y:  N x C x H x W  --> bottom[2]     
 *            F:  N x C x H x W  --> top[0]
 */
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  Blob spatial_sum_multiplier_;
};





void AxpyLayer::Reshape(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  CHECK_EQ(bottom[0]->shape(0), bottom[1]->shape(0));
  CHECK_EQ(bottom[0]->shape(1), bottom[1]->shape(1));
  if (bottom[0]->num_axes() == 4) {
    CHECK_EQ(bottom[0]->shape(2), 1);
    CHECK_EQ(bottom[0]->shape(3), 1);
  }
  CHECK(bottom[1]->shape() == bottom[2]->shape());  
  top[0]->ReshapeLike(*bottom[1]);
  int spatial_dim = bottom[1]->count(2);
  if (spatial_sum_multiplier_.count() < spatial_dim) {
    spatial_sum_multiplier_.Reshape(vector<int>(1, spatial_dim));
    blas_set(spatial_dim, Dtype(1), 
        spatial_sum_multiplier_.mutable_cpu_data());
  }
}


void AxpyLayer::Forward_cpu(const vector<Blob*>& bottom,  
    const vector<Blob*>& top) { 
  int channel_dim = bottom[1]->channels();
  int spatial_dim = bottom[1]->count(2);
  const Dtype* scale_data = pBox_datap(bottom_blob->data);
  const Dtype* x_data = pBox_datap(top_blob->data);
  Dtype* top_data = top[0]->mutable_cpu_data();
  blas_copy(bottom[2]->count(), bottom[2]->cpu_data(), top_data);
  for (int n = 0; n < bottom[1]->num(); ++n) {
    for (int c = 0; c < channel_dim; ++c) {
      int scale_offset = n * channel_dim + c;
      blas_axpy(spatial_dim, scale_data[scale_offset], 
          x_data + scale_offset * spatial_dim, 
          top_data + scale_offset * spatial_dim);  
    }
  }
}


void AxpyLayer::Backward_cpu(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  const int count = top[0]->count();
  const Dtype* top_diff = top[0]->cpu_diff();
  if (propagate_down[0]) {
    int spatial_dim = bottom[1]->count(2);
    const Dtype* x_data = pBox_datap(top_blob->data);
    Dtype* x_diff = bottom[1]->mutable_cpu_diff(); 
    Dtype* scale_diff = bottom[0]->mutable_cpu_diff();
    blas_mul(count, top_diff, x_data, x_diff);
    blas_set(bottom[0]->count(), Dtype(0), scale_diff);
    blas_gemv(CblasNoTrans, bottom[0]->count(), spatial_dim, Dtype(1),
        x_diff, spatial_sum_multiplier_.cpu_data(), Dtype(1), scale_diff); 
    if (!propagate_down[1]) {
      blas_set(bottom[1]->count(), Dtype(0), x_diff);
    }
  }
  if (propagate_down[0]) {
    int channel_dim = bottom[1]->channels();
    int spatial_dim = bottom[1]->count(2);
    const Dtype* scale_data = pBox_datap(bottom_blob->data);
    Dtype* x_diff = bottom[1]->mutable_cpu_diff();
    for (int n = 0; n < bottom[1]->num(); ++n) {
      for (int c = 0; c < channel_dim; ++c) {
        int scale_offset = n * channel_dim + c;
        blas_scale(spatial_dim, scale_data[scale_offset], 
            top_diff + scale_offset * spatial_dim, 
            x_diff + scale_offset * spatial_dim);  
      }
    }
  }
  if (propagate_down[2]) {
    blas_copy(count, top_diff, bottom[2]->mutable_cpu_diff());
  }
}

#ifdef CPU_ONLY
STUB_GPU(AxpyLayer);
#endif

INSTANTIATE_CLASS(AxpyLayer);
REGISTER_LAYER_CLASS(Axpy);

} // namespace
