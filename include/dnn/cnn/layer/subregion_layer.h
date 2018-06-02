



#include "caffe/common.hpp"
#include "caffe/layer.hpp"
#include "caffe/syncedmem.hpp"

#include "caffe/custom_layers.hpp"




void SubRegionLayer::LayerSetUp(const vector<Blob*>& bottom,
                                  const vector<Blob*>& top) {
  SubRegionParameter sub_region_param = this->layer_param_.sub_region_param();
  height_ = sub_region_param.region_height();
  width_ = sub_region_param.region_width();
  data_height_ = sub_region_param.data_height();
  data_width_ = sub_region_param.data_width();
  as_dim_ = sub_region_param.as_dim();
}


void SubRegionLayer::Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  CHECK_EQ(4, bottom[0]->num_axes()) << "Input must have 4 axes, "
      << "corresponding to (num, channels, height, width)";
  CHECK_EQ(prev1->size.c % 2, 0) << "The coordinate blob's size must be able to divided by 2!";
  int num_point = prev1->size.c / 2;
  if (as_dim_ == 0) {
    top[0]->Reshape({ prev->size.n * num_point, prev->size.c, height_, width_ });
  }
  else {
    top[0]->Reshape({ prev->size.n, prev->size.c * num_point, height_, width_ });
  }
  if (layer->o_size == 3) {
    top[1]->ReshapeLike(*bottom[1]);
    top[2]->ReshapeLike(*bottom[1]);
  }
}


void SubRegionLayer::Forward(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  const Dtype* prevData = prevData;
  const Dtype* point_data = nextData;
  const Dtype* ground_truth_point_data = getData(prev2);
  Dtype* nextData = nextData;
  
  const int num_point = prevSize1.c / 2;
  const int num = prev->size.n;
  const int channels = prev->size.c;
  for (n = 0; n < num; n++) {
    for (i = 0; i < num_point; i++) {
      Dtype center_x = (point_data[n * prevSize1.c + i * 2] / data_width_ + 0.5)  * prev->size.w;
      Dtype center_y = (point_data[n * prevSize1.c + i * 2 + 1] / data_height_ + 0.5) * prev->size.h;
      int x0 = floor(center_x - width_ / 2);
      int y0 = floor(center_y - height_ / 2);
      if (layer->o_size == 3) {
        top[2]->mutable_data()[n * prevSize1.c + i * 2] = (Dtype)(x0 + (Dtype)width_ / 2 + 0.5) / (Dtype)prev->size.w * data_width_;
        top[2]->mutable_data()[n * prevSize1.c + i * 2 + 1] = (Dtype)(y0 + (Dtype)height_ / 2 + 0.5) / (Dtype)prev->size.h * data_height_;
        top[1]->mutable_data()[n * prevSize1.c + i * 2] = 
          (ground_truth_point_data[n * prevSize1.c + i * 2] + data_width_ / 2) - top[2]->cpu_data()[n * prevSize1.c + i * 2];
        top[1]->mutable_data()[n * prevSize1.c + i * 2 + 1] = 
          (ground_truth_point_data[n * prevSize1.c + i * 2 + 1] + data_height_ / 2) - top[2]->cpu_data()[n * prevSize1.c + i * 2 + 1];

      }

      for (c = 0; c < channels; c++) {
        for (h = 0; h < height_; h++) {
          for (w = 0; w < width_; w++) {
            if (y0 + h >= 0 && y0 + h <= prev->size.h - 1
                && x0 + w >= 0 && x0 + w <= prev->size.w - 1) {
              if (as_dim_ == 0) {
                nextData[top[0]->offset(num * i + n, c, h, w)] = prevData[bottom[0]->offset(n, c, y0 + h, x0 + w)];
              }
              else {
                nextData[top[0]->offset(n, channels * i + c, h, w)] = prevData[bottom[0]->offset(n, c, y0 + h, x0 + w)];
              }
            }
            else {
              if (as_dim_ == 0) {
                nextData[top[0]->offset(num * i + n, c, h, w)] = 0;
              }
              else {
                nextData[top[0]->offset(n, channels * i + c, h, w)] = 0;
              }
            }
          }
        }
      }
    }
  }
}


void SubRegionLayer::Backward(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  if (!propagate_down[0]) {
    return;
  }
  const Dtype* nextDiff = nextDiff;
  const Dtype* point_data = nextData;
  Dtype* prevDiff = top[0]->mutable_diff();
  blas_sset(Data_count(prev->size), (Dtype)(0), prevDiff);

  const int num_point = prevSize1.c / 2;
  const int num = prev->size.n;
  const int channels = prev->size.c;
  for (n = 0; n < num; n++) {
    for (i = 0; i < num_point; i++) {
      Dtype center_x = (point_data[n * prevSize1.c + i * 2] / data_width_ + 0.5)  * prev->size.w;
      Dtype center_y = (point_data[n * prevSize1.c + i * 2 + 1] / data_height_ + 0.5) * prev->size.h;
      int x0 = floor(center_x - width_ / 2);
      int y0 = floor(center_y - height_ / 2);

      for (c = 0; c < channels; c++) {
        for (h = 0; h < height_; h++) {
          for (w = 0; w < width_; w++) {
            if (y0 + h >= 0 && y0 + h <= prev->size.h - 1
                && x0 + w >= 0 && x0 + w <= prev->size.w - 1) {
              if (as_dim_ == 0) {
                prevDiff[bottom[0]->offset(n, c, y0 + h, x0 + w)] += nextDiff[top[0]->offset(num * i + n, c, h, w)];
              }
              else {
                prevDiff[bottom[0]->offset(n, c, y0 + h, x0 + w)] += nextDiff[top[0]->offset(n, channels * i + c, h, w)];
              }
            }
          }
        }
      }
    }
  }
}


#ifdef CPU_ONLY
STUB_GPU(SubRegionLayer);
#endif

INSTANTIATE_CLASS(SubRegionLayer);
REGISTER_LAYER_CLASS(SubRegion);


