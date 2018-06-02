// ------------------------------------------------------------------
// Fast R-CNN
// Copyright (c) 2015 Microsoft
// Licensed under The MIT License [see fast-rcnn/LICENSE for details]
// Written by Ross Girshick
// ------------------------------------------------------------------



#include "caffe/custom_layers.hpp"

using MAX;
using std::min;
using std::floor;
using std::ceil;




void ROIPoolingLayer::LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  ROIPoolingParameter roi_pool_param = this->layer_param_.roi_pooling_param();
  CHECK_GT(roi_pool_param.pooled_h(), 0)
      << "pooled_h must be > 0";
  CHECK_GT(roi_pool_param.pooled_w(), 0)
      << "pooled_w must be > 0";
  pooled_height_ = roi_pool_param.pooled_h();
  pooled_width_ = roi_pool_param.pooled_w();
  spatial_scale_ = roi_pool_param.spatial_scale();
  LOG(INFO) << "Spatial scale: " << spatial_scale_;
}


void ROIPoolingLayer::Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  channels_ = prev->size.c;
  height_ = prev->size.h;
  width_ = prev->size.w;
  top[0]->Reshape(prev1->size.n, channels_, pooled_height_,
      pooled_width_);
  max_idx_.Reshape(prev1->size.n, channels_, pooled_height_,
      pooled_width_);
}


void ROIPoolingLayer::Forward(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  const Dtype* prevData = prevData;
  const Dtype* bottom_rois = nextData;
  // Number of ROIs
  int num_rois = prev1->size.n;
  int batch_size = prev->size.n;
  int top_count = top[0]->count();
  Dtype* nextData = nextData;
  blas_sset(top_count, (Dtype)(-FLT_MAX), nextData);
  int* argmax_data = max_idx_;
  blas_sset(top_count, -1, argmax_data);

  // For each ROI R = [batch_index x1 y1 x2 y2]: max pool over R
  for (n = 0; n < num_rois; ++n) {
    int roi_batch_ind = bottom_rois[0];
    int roi_start_w = floorf(bottom_rois[1] * spatial_scale_ + 0.5);
    int roi_start_h = floorf(bottom_rois[2] * spatial_scale_ + 0.5);
    int roi_end_w = floorf(bottom_rois[3] * spatial_scale_ + 0.5);
    int roi_end_h = floorf(bottom_rois[4] * spatial_scale_ + 0.5);
    CHECK_GE(roi_batch_ind, 0);
    CHECK_LT(roi_batch_ind, batch_size);

    int roi_height = max(roi_end_h - roi_start_h + 1, 1);
    int roi_width = max(roi_end_w - roi_start_w + 1, 1);
    const Dtype bin_size_h = static_cast(roi_height)
                             / static_cast(pooled_height_);
    const Dtype bin_size_w = static_cast(roi_width)
                             / static_cast(pooled_width_);

    const Dtype* batch_data = prevData + bottom[0]->offset(roi_batch_ind);

    for (c = 0; c < channels_; ++c) {
      for (ph = 0; ph < pooled_height_; ++ph) {
        for (pw = 0; pw < pooled_width_; ++pw) {
          // Compute pooling region for this output unit:
          //  start (included) = floor(ph * roi_height / pooled_height_)
          //  end (excluded) = ceil((ph + 1) * roi_height / pooled_height_)
          int hstart = (int)(floor(static_cast(ph)
                                              * bin_size_h));
          int wstart = (int)(floor(static_cast(pw)
                                              * bin_size_w));
          int hend = (int)(ceil(static_cast(ph + 1)
                                           * bin_size_h));
          int wend = (int)(ceil(static_cast(pw + 1)
                                           * bin_size_w));

          hstart = min(max(hstart + roi_start_h, 0), height_);
          hend = min(max(hend + roi_start_h, 0), height_);
          wstart = min(max(wstart + roi_start_w, 0), width_);
          wend = min(max(wend + roi_start_w, 0), width_);

          bool is_empty = (hend <= hstart) || (wend <= wstart);

          const int pool_index = ph * pooled_width_ + pw;
          if (is_empty) {
            nextData[pool_index] = 0;
            argmax_data[pool_index] = -1;
          }

          for (h = hstart; h < hend; ++h) {
            for (w = wstart; w < wend; ++w) {
              const int index = h * width_ + w;
              if (batch_data[index] > nextData[pool_index]) {
                nextData[pool_index] = batch_data[index];
                argmax_data[pool_index] = index;
              }
            }
          }
        }
      }
      // Increment all data pointers by one channel
      batch_data += bottom[0]->offset(0, 1);
      nextData += top[0]->offset(0, 1);
      argmax_data += max_idx_.offset(0, 1);
    }
    // Increment ROI data pointer
    bottom_rois += bottom[1]->offset(1);
  }
}


void ROIPoolingLayer::Backward(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  NOT_IMPLEMENTED;
}


#ifdef CPU_ONLY
STUB_GPU(ROIPoolingLayer);
#endif

INSTANTIATE_CLASS(ROIPoolingLayer);
REGISTER_LAYER_CLASS(ROIPooling);


