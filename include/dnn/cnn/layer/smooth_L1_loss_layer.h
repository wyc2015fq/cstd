// ------------------------------------------------------------------
// Fast R-CNN
// Copyright (c) 2015 Microsoft
// Licensed under The MIT License [see fast-rcnn/LICENSE for details]
// Written by Ross Girshick
// ------------------------------------------------------------------

#include "caffe/custom_layers.hpp"



  
  void SmoothL1LossLayer::LayerSetUp(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
    has_weights_ = (layer->i_size == 3);
    turn_point_ = this->layer_param_.smoothl1_param().turn_point();
  }

  
  void SmoothL1LossLayer::Reshape(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
    LossLayer::Reshape(bottom, top);
    CHECK_EQ(prev->size.c, prev1->size.c);
    CHECK_EQ(prev->size.h, prev1->size.h);
    CHECK_EQ(prev->size.w, prev1->size.w);
    if (has_weights_) {
      CHECK_EQ(prev->size.c, prev2->size.c);
      CHECK_EQ(prev->size.h, prev2->size.h);
      CHECK_EQ(prev->size.w, prev2->size.w);
    }
    diff_.Reshape(prev->size.n, prev->size.c,
                  prev->size.h, prev->size.w);
    errors_.Reshape(prev->size.n, prev->size.c,
                    prev->size.h, prev->size.w);
  }

  
  void SmoothL1LossLayer::Forward(const vector<Blob*>& bottom,
                                             const vector<Blob*>& top) {
    NOT_IMPLEMENTED;
  }

  
  void SmoothL1LossLayer::Backward(const vector<Blob*>& top,
                                              const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
    NOT_IMPLEMENTED;
  }

#ifdef CPU_ONLY
  STUB_GPU(SmoothL1LossLayer);
#endif

  INSTANTIATE_CLASS(SmoothL1LossLayer);
  REGISTER_LAYER_CLASS(SmoothL1Loss);

