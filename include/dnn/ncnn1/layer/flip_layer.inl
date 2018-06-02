






#include "caffe/layer.hpp"
#include "flip_layer.hpp"
#include "caffe/net.hpp"





void FlipLayer::LayerSetUp(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  flip_width_ = this->layer_param_.flip_param().flip_width();
  flip_height_ = this->layer_param_.flip_param().flip_height();
}


void FlipLayer::Reshape(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  top[0]->ReshapeLike(*bottom[0]);
}


void FlipLayer::Forward_cpu(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* bottom_data = pBox_datap(bottom_blob->data);
  Dtype* top_data = top[0]->mutable_cpu_data();
  int num = bottom[0]->num();
  int channels = bottom[0]->channels();
  int width = bottom[0]->width();
  int height = bottom[0]->height();

  for (int n = 0; n < num; n++) {
    for (int c = 0; c < channels; c++) {
      for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
          top_data[(((n * channels + c) * height + h) * width) + w] = 
            bottom_data[(((n * channels + c) * height + (flip_height_ ? (height - 1 - h) : h)) * width) + (flip_width_ ? (width - 1 - w) : w)];
        }
      }
    }
  }
}


void FlipLayer::Backward_cpu(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  const Dtype* top_diff = top[0]->cpu_diff();
  Dtype* bottom_diff = bottom[0]->mutable_cpu_diff();
  int num = bottom[0]->num();
  int channels = bottom[0]->channels();
  int width = bottom[0]->width();
  int height = bottom[0]->height();

  if (propagate_down[0]) {
    for (int n = 0; n < num; n++) {
      for (int c = 0; c < channels; c++) {
        for (int h = 0; h < height; h++) {
          for (int w = 0; w < width; w++) {
            bottom_diff[(((n * channels + c) * height + h) * width) + w] = 
              top_diff[(((n * channels + c) * height + (flip_height_ ? (height - 1 - h) : h)) * width) + (flip_width_ ? (width - 1 - w) : w)];
          }
        }
      }
    }
  }
}

#ifdef CPU_ONLY
STUB_GPU(FlipLayer);
#endif

INSTANTIATE_CLASS(FlipLayer);
REGISTER_LAYER_CLASS(Flip);


