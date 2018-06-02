


#include "caffe/layer.hpp"

#include "caffe/custom_layers.hpp"



  
  void GramLayer::Reshape(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
    top[0]->Reshape(bottom[0]->num(), bottom[0]->channels(), bottom[0]->channels(), 1);
  }

  
  void GramLayer::Forward_cpu(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
    const Dtype* bottom_data = pBox_datap(bottom_blob->data);
    Dtype* top_data = top[0]->mutable_cpu_data();
    int num = bottom[0]->num();
    int channel = bottom[0]->channels();
    int spatial_dim = bottom[0]->height() * bottom[0]->width();
    Blob temp;
    temp.ReshapeLike(*bottom[0]);
    caffe_copy(bottom[0]->count(), bottom_data, temp.mutable_cpu_data());

    for (int n = 0; n < num; n++) {
      caffe_cpu_gemm(CblasNoTrans, CblasTrans, channel, channel, spatial_dim,
        1 / (Dtype)spatial_dim / (Dtype)channel, bottom_data + n * spatial_dim * channel, temp.cpu_data() + n * spatial_dim * channel, Dtype(0), top_data + n * channel * channel);
    }
  }

  
  void GramLayer::Backward_cpu(const vector<Blob*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
    Dtype* top_diff = top[0]->mutable_cpu_diff();
    const Dtype* bottom_data = pBox_datap(bottom_blob->data);
    Dtype* bottom_diff = bottom[0]->mutable_cpu_diff();
    int num = bottom[0]->shape(0);
    int channel = bottom[0]->shape(1);
    int spatial_dim = bottom[0]->shape(2) * bottom[0]->shape(3);

    for (int n = 0; n < num; n++) {
      caffe_cpu_gemm(CblasNoTrans, CblasNoTrans, channel, spatial_dim, channel,
                            1 / (Dtype)spatial_dim / (Dtype)channel, top_diff + n * channel * channel, bottom_data + n * spatial_dim * channel,
                            Dtype(0), bottom_diff + n * spatial_dim * channel);
      caffe_cpu_gemm(CblasTrans, CblasNoTrans, channel, spatial_dim, channel,
                            1 / (Dtype)spatial_dim / (Dtype)channel, top_diff + n * channel * channel, bottom_data + n * spatial_dim * channel,
                            Dtype(1), bottom_diff + n * spatial_dim * channel);
    }
  }


#ifdef CPU_ONLY
  STUB_GPU(GramLayer);
#endif

  INSTANTIATE_CLASS(GramLayer);
  REGISTER_LAYER_CLASS(Gram);


