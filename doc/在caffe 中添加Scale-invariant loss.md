# 在caffe 中添加Scale-invariant loss

2016年10月24日 19:27:41 [seven_first](https://me.csdn.net/seven_first) 阅读数：1503







### 一、简介

Scale-invariant loss 的定义为：
$$
L(y,y^*) = \frac{1}{n}\sum_id_i^2 - \frac{\lambda}{n^2}(\sum_id_i)^2
$$
参考论文：

Depth Map Prediction from a Single Image using a Multi-Scale Deep Network



### 二、创建ScaleInvariantLoss Layer

#### 1. caffe.proto

- 在 LayerParameter 中添加：

```
optional ScaleInvariantParameter scale_invariant_param = 163;1
```

- 新建message ScaleInvariantParameter ：

```
message ScaleInvariantParameter {
  optional float lambda = 1 [default = 1];
}123
```

#### 2.scale_invariant_loss_layer.hpp

```
//  Create on: 2016/10/24 ShanghaiTech
//  Author:    Yingying Zhang

#ifndef CAFFE_SCALE_INVARIANT_LOSS_LAYER_HPP_
#define CAFFE_SCALE_INVARIANT_LOSS_LAYER_HPP_

#include <vector>

#include "caffe/blob.hpp"
#include "caffe/layer.hpp"
#include "caffe/proto/caffe.pb.h"

#include "caffe/layers/loss_layer.hpp"

namespace caffe {

template <typename Dtype>
class ScaleInvariantLossLayer : public LossLayer<Dtype> {
 public:
  explicit ScaleInvariantLossLayer(const LayerParameter& param)
      : LossLayer<Dtype>(param), diff_() {}
  virtual void LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Reshape(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual inline const char* type() const { return "ScaleInvariantLoss"; }
  virtual inline bool AllowForceBackward(const int bottom_index) const {
    return true;
  }

 protected:

  virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Backward_cpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);
  virtual void Backward_gpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);

  Blob<Dtype> diff_;
  Blob<Dtype> ones_;
  Dtype lambda_;
  Dtype sum_di_;

};

}  // namespace caffe

#endif  // CAFFE_SCALE_INVARIANT_LOSS_LAYER_HPP_
12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152
```

#### 3.scale_invariant_loss_layer.cpp

```
//  Create on: 2016/10/24 ShanghaiTech
//  Author:    Yingying Zhang

#include <vector>

#include "caffe/layers/scale_invariant_loss_layer.hpp"
#include "caffe/util/math_functions.hpp"

namespace caffe {

template <typename Dtype>
void ScaleInvariantLossLayer<Dtype>::LayerSetUp(
  const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
  LossLayer<Dtype>::LayerSetUp(bottom, top);
  lambda_ = this->layer_param_.scale_invariant_param().lambda();
}

template <typename Dtype>
void ScaleInvariantLossLayer<Dtype>::Reshape(
  const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
  LossLayer<Dtype>::Reshape(bottom, top);
  CHECK_EQ(bottom[0]->count(1), bottom[1]->count(1))
      << "Inputs must have the same dimension.";
  diff_.ReshapeLike(*bottom[0]);
  ones_.ReshapeLike(*bottom[0]);
}

template <typename Dtype>
void ScaleInvariantLossLayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top) {
  NOT_IMPLEMENTED;
}

template <typename Dtype>
void ScaleInvariantLossLayer<Dtype>::Backward_cpu(const vector<Blob<Dtype>*>& top,
    const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom) {
  NOT_IMPLEMENTED;
}

INSTANTIATE_CLASS(ScaleInvariantLossLayer);
REGISTER_LAYER_CLASS(ScaleInvariantLoss);

}  // namespace caffe

123456789101112131415161718192021222324252627282930313233343536373839404142434445
```

#### 4.scale_invariant_loss_layer.cu

```
//  Create on: 2016/10/24 ShanghaiTech
//  Author:    Yingying Zhang

#include <vector>

#include "caffe/layers/scale_invariant_loss_layer.hpp"
#include "caffe/util/math_functions.hpp"

namespace caffe {

template <typename Dtype>
void ScaleInvariantLossLayer<Dtype>::Forward_gpu(const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top) {
  // 1. sum(d_i ^ 2) / 2n
  int count = bottom[0]->count();
  caffe_gpu_sub(
      count,
      bottom[0]->gpu_data(),
      bottom[1]->gpu_data(),
      diff_.mutable_gpu_data());
  Dtype dot;
  caffe_gpu_dot(count, diff_.gpu_data(), diff_.gpu_data(), &dot);
  Dtype loss = dot / bottom[0]->num() / Dtype(2);
  // 2. lambda  * (sum(d_i) ^ 2 / 2n^2
  caffe_gpu_set(count,  Dtype(1), ones_.mutable_gpu_data());
  caffe_gpu_dot(count, diff_.gpu_data(), ones_.gpu_data(), &sum_di_);
  loss -= lambda_ * sum_di_ * sum_di_ / bottom[0]->num()
          / bottom[0]->num() / Dtype(2);
  top[0]->mutable_cpu_data()[0] = loss;
}

template <typename Dtype>
void ScaleInvariantLossLayer<Dtype>::Backward_gpu(const vector<Blob<Dtype>*>& top,
    const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom) {
  for (int i = 0; i < 2; ++i) {
    if (propagate_down[i]) {
      //1. gradient for sum(d_i ^ 2) / 2n
      const Dtype sign = (i == 0) ? 1 : -1;
      Dtype alpha = sign * top[0]->cpu_diff()[0] / bottom[i]->num();
      caffe_gpu_axpby(
          bottom[i]->count(),            
          alpha,                         
          diff_.gpu_data(),                   
          Dtype(0),                          
          bottom[i]->mutable_gpu_diff());  
      //2. graidient for lambda  * (sum(d_i) ^ 2 / 2n^2
      alpha = -sign * top[0]->cpu_diff()[0] * sum_di_ * lambda_
              / bottom[i]->num() / bottom[i]->num();
      caffe_gpu_axpby(
          bottom[i]->count(),            
          alpha,                              
          ones_.gpu_data(),                  
          Dtype(1),       // accumulate gradient
          bottom[i]->mutable_gpu_diff()); 
    }
  }
}

INSTANTIATE_LAYER_GPU_FUNCS(ScaleInvariantLossLayer);

}  // namespace caffe
```