// ------------------------------------------------------------------
// Fast R-CNN
// Copyright (c) 2015 Microsoft
// Licensed under The MIT License [see fast-rcnn/LICENSE for details]
// Written by Ross Girshick
// ------------------------------------------------------------------

#include "caffe/custom_layers.hpp"



  
  void SmoothL1LossLayer::LayerSetUp(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
    has_weights_ = (bottom.size() == 3);
    turn_point_ = this->layer_param_.smoothl1_param().turn_point();
  }

  
  void SmoothL1LossLayer::Reshape(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
    LossLayer::Reshape(bottom, top);
    CHECK_EQ(bottom[0]->channels(), bottom[1]->channels());
    CHECK_EQ(bottom[0]->height(), bottom[1]->height());
    CHECK_EQ(bottom[0]->width(), bottom[1]->width());
    if (has_weights_) {
      CHECK_EQ(bottom[0]->channels(), bottom[2]->channels());
      CHECK_EQ(bottom[0]->height(), bottom[2]->height());
      CHECK_EQ(bottom[0]->width(), bottom[2]->width());
    }
    diff_.Reshape(bottom[0]->num(), bottom[0]->channels(),
                  bottom[0]->height(), bottom[0]->width());
    errors_.Reshape(bottom[0]->num(), bottom[0]->channels(),
                    bottom[0]->height(), bottom[0]->width());
  }

  
  void SmoothL1LossLayer::Forward_cpu(const vector<Blob*>& bottom,
                                             const vector<Blob*>& top) {
    NOT_IMPLEMENTED;
  }

  
  void SmoothL1LossLayer::Backward_cpu(const vector<Blob*>& top,
                                              const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
    NOT_IMPLEMENTED;
  }

#ifdef CPU_ONLY
  STUB_GPU(SmoothL1LossLayer);
#endif

  INSTANTIATE_CLASS(SmoothL1LossLayer);
  REGISTER_LAYER_CLASS(SmoothL1Loss);

