


#include "caffe/layer.hpp"

#include "caffe/custom_layers.hpp"



  
  void GramLayer::Reshape(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
    top[0]->Reshape(prev->size.n, prev->size.c, prev->size.c, 1);
  }

  
  void GramLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
    const Dtype* prevData = prevData;
    Dtype* nextData = nextData;
    int num = prev->size.n;
    int channel = prev->size.c;
    int spatial_dim = prev->size.h * prev->size.w;
    Blob temp;
    temp.ReshapeLike(*bottom[0]);
    blas_scopy(Data_count(prev->size), prevData, temp);

    for (n = 0; n < num; n++) {
      blas_sgemm(CblasNoTrans, CblasTrans, channel, channel, spatial_dim,
        1 / (Dtype)spatial_dim / (Dtype)channel, prevData + n * spatial_dim * channel, temp.cpu_data() + n * spatial_dim * channel, (Dtype)(0), nextData + n * channel * channel);
    }
  }

  
  void GramLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
    Dtype* nextDiff = top[0]->mutable_diff();
    const Dtype* prevData = prevData;
    Dtype* prevDiff = bottom[0]->mutable_diff();
    int num = prevSize.n;
    int channel = prevSize.c;
    int spatial_dim = bottom[0]->shape(2) * bottom[0]->shape(3);

    for (n = 0; n < num; n++) {
      blas_sgemm(CblasNoTrans, CblasNoTrans, channel, spatial_dim, channel,
                            1 / (Dtype)spatial_dim / (Dtype)channel, nextDiff + n * channel * channel, prevData + n * spatial_dim * channel,
                            (Dtype)(0), prevDiff + n * spatial_dim * channel);
      blas_sgemm(CblasTrans, CblasNoTrans, channel, spatial_dim, channel,
                            1 / (Dtype)spatial_dim / (Dtype)channel, nextDiff + n * channel * channel, prevData + n * spatial_dim * channel,
                            (Dtype)(1), prevDiff + n * spatial_dim * channel);
    }
  }


#ifdef CPU_ONLY
  STUB_GPU(GramLayer);
#endif

  INSTANTIATE_CLASS(GramLayer);
  REGISTER_LAYER_CLASS(Gram);


