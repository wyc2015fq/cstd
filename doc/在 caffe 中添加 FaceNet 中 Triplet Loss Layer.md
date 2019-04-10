# 在 caffe 中添加 FaceNet 中 Triplet Loss Layer

2015年12月12日 01:12:17 [seven_first](https://me.csdn.net/seven_first) 阅读数：12630





### 一、Triplet Loss 定义

论文：[FaceNet: A Unified Embedding for Face Recognition and Clustering](http://arxiv.org/pdf/1503.03832v3.pdf) 中有关于Triplet loss 详细定义及意义.
$$
L = \sum\limits_i^N[||f(x_i^a)-f(x_i^p)||_2^2 - ||f(x_i^a)-f(x_i^n)||^2_2+\alpha ]_+
$$


back propogation 需要用到的导数： 
$$
\frac{\partial(L)}{\partial f(x_i^a)} = 2\cdot(f(x_i^n) - f(x_i^p)) \\
\frac{\partial(L)}{\partial f(x_i^p)} =-2\cdot(f(x_i^a) - f(x_i^p)) \\
\frac{\partial(L)}{\partial f(x_i^n)} =2\cdot(f(x_i^a) - f(x_i^n))
$$

### 二、创建Triplet Loss Layer

#### 1. 在caffe.proto添加对应Message

- 首先在LayerParameter中添加：

```
optional TripletLossParameter triplet_loss_param = 136;
//其中136是我自己装的caffe的当前参数 ID，记得每添加新的参数时，更新message首段注释中的最新ID，以方便下次添加12
```

- 由于我们需要设定的参数只有 margin:    α   α， 所以新建Message TripletLossParameter ：

```
 message TripletLossParameter {  
        optional float margin = 1 [default = 1.0];   
    }  123
```

#### 2.创建triplet_loss_layer.hpp

```
#ifndef CAFFE_TRIPLET_LOSS_LAYER_HPP_
#define CAFFE_TRIPLET_LOSS_LAYER_HPP_

#include <vector>

#include "caffe/blob.hpp"
#include "caffe/layer.hpp"
#include "caffe/proto/caffe.pb.h"

#include "caffe/layers/loss_layer.hpp"

namespace caffe {

template <typename Dtype>
class TripletLossLayer : public LossLayer<Dtype> {
 public:
  explicit TripletLossLayer (const LayerParameter& param)
      : LossLayer<Dtype>(param), diff_np_(), diff_ap_(), diff_an_() {}
  virtual void LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);

  virtual inline int ExactNumBottomBlobs() const { return 3; }
  virtual inline const char* type() const { return "TripletLoss"; }
  /**
   * Unlike most loss layers, in the TripletLossLayer we can backpropagate
   * to the first three inputs.
   */
  virtual inline bool AllowForceBackward(const int bottom_index) const {
    return bottom_index != 3;
  }

 protected:
  /// @copydoc ContrastiveLossLayer
  virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);

  virtual void Backward_cpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);
  virtual void Backward_gpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);

  Blob<Dtype> diff_np_;  // cached for backward pass: f(x_i^n) - f(x_i^p)
  Blob<Dtype> diff_ap_;  // cached for backward pass: f(x_i^a) - f(x_i^p)
  Blob<Dtype> diff_an_;  // cached for backward pass: f(x_i^a) - f(x_i^n)
  Blob<Dtype> dist_ap_sq_;  // cached for backward pass :||f(x_i^a)-f(x_i^p)||^2
  Blob<Dtype> dist_an_sq_;  // cached for backward pass :||f(x_i^a)-f(x_i^n)||^2
  Blob<Dtype> diff_ap_sq_;  // tmp storage for gpu forward pass: f(x_i^a)-f(x_i^p).^2 
  Blob<Dtype> diff_an_sq_;  // tmp storage for gpu forward pass: f(x_i^a)-f(x_i^n).^2 
  Blob<Dtype> summer_vec_;  // tmp storage for gpu forward pass: [ 1, 1, 1,...,1 ]
};

}  // namespace caffe

#endif  // CAFFE_TRIPLET_LOSS_LAYER_HPP_
123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657
```

#### 3.创建triplet_loss_layer.cpp

```
#include <algorithm>
#include <vector>

#include "caffe/layers/triplet_loss_layer.hpp"
#include "caffe/util/math_functions.hpp"

namespace caffe {

template <typename Dtype>
void TripletLossLayer<Dtype>::LayerSetUp(
  const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
  LossLayer<Dtype>::LayerSetUp(bottom, top);
  // bottom[0] : f(x_i^a); bottom[1] : f(x_i^p); bottom[2] : f(x_i^n)
  CHECK_EQ(bottom[0]->num(), bottom[1]->num());  
  CHECK_EQ(bottom[1]->num(), bottom[2]->num());  
  CHECK_EQ(bottom[0]->channels(), bottom[1]->channels());  
  CHECK_EQ(bottom[1]->channels(), bottom[2]->channels());  
  CHECK_EQ(bottom[0]->height(), 1);  
  CHECK_EQ(bottom[0]->width(), 1);  
  CHECK_EQ(bottom[1]->height(), 1);  
  CHECK_EQ(bottom[1]->width(), 1);  
  CHECK_EQ(bottom[2]->height(), 1);  
  CHECK_EQ(bottom[2]->width(), 1);  

  diff_np_.Reshape(bottom[0]->num(), bottom[0]->channels(), 1, 1);  
  diff_ap_.Reshape(bottom[0]->num(), bottom[0]->channels(), 1, 1);  
  diff_an_.Reshape(bottom[0]->num(), bottom[0]->channels(), 1, 1);  
  dist_ap_sq_.Reshape(bottom[0]->num(), bottom[0]->channels(), 1, 1);  
  diff_an_sq_.Reshape(bottom[0]->num(), bottom[0]->channels(), 1, 1);  
  dist_ap_sq_.Reshape(bottom[0]->num(), 1, 1, 1);  
  dist_an_sq_.Reshape(bottom[0]->num(), 1, 1, 1);  
  // vector of ones used to sum along channels
  summer_vec_.Reshape(bottom[0]->channels(), 1, 1, 1);
  for (int i = 0; i < bottom[0]->channels(); ++i)
    summer_vec_.mutable_cpu_data()[i] = Dtype(1);
}

template <typename Dtype>
void TripletLossLayer<Dtype>::Forward_cpu(
    const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top) {
  int count = bottom[0]->count();
  caffe_sub(
      count,
      bottom[2]->cpu_data(),  // f(x_i^n)
      bottom[1]->cpu_data(),  // f(x_i^p)
      diff_np_.mutable_cpu_data());  // f(x_i^n)-f(x_i^p)
  caffe_sub(
      count,
      bottom[0]->cpu_data(),  // f(x_i^a)
      bottom[1]->cpu_data(),  // f(x_i^p)
      diff_ap_.mutable_cpu_data());  // f(x_i^a)-f(x_i^p)
  caffe_sub(
      count,
      bottom[0]->cpu_data(),  // f(x_i^a)
      bottom[2]->cpu_data(),  // f(x_i^n)
      diff_an_.mutable_cpu_data());  // f(x_i^a)-f(x_i^n)     
  const int channels = bottom[0]->channels();
  Dtype margin = this->layer_param_.triplet_loss_param().margin();
  Dtype loss(0.0);
  for (int i = 0; i < bottom[0]->num(); ++i) {
    dist_ap_sq_.mutable_cpu_data()[i] = caffe_cpu_dot(channels,
        diff_ap_.cpu_data() + (i*channels), diff_ap_.cpu_data() + (i*channels));
    dist_an_sq_.mutable_cpu_data()[i] = caffe_cpu_dot(channels,
        diff_an_.cpu_data() + (i*channels), diff_an_.cpu_data() + (i*channels));
    Dtype trip_dist = std::max(margin + dist_ap_sq_.cpu_data()[i] - dist_an_sq_.cpu_data()[i], Dtype(0.0));  
    loss += trip_dist;  
    if(trip_dist == Dtype(0)){  
        //when ||f(x_i^a)-f(x_i^p)||^2 - ||f(x_i^a)-f(x_i^n)||^2 + margin < 0
        //this triplet has no contribution to loss,so the differential should be zero.
        caffe_set(channels, Dtype(0), diff_np_.mutable_cpu_data() + (i*channels));  
        caffe_set(channels, Dtype(0), diff_ap_.mutable_cpu_data() + (i*channels));  
        caffe_set(channels, Dtype(0), diff_an_.mutable_cpu_data() + (i*channels));     
    }    
  }
  top[0]->mutable_cpu_data()[0] = loss;
}

template <typename Dtype>
void TripletLossLayer<Dtype>::Backward_cpu(const vector<Blob<Dtype>*>& top,
    const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom) {
  for (int i = 0; i < 3; ++i) {
    if (propagate_down[i]) {
      const Dtype sign = (i == 1) ? -2 : 2;
      const Dtype alpha = sign * top[0]->cpu_diff()[0];
      int num = bottom[i]->num();
      int channels = bottom[i]->channels();
      for (int j = 0; j < num; ++j) {
        Dtype* bout = bottom[i]->mutable_cpu_diff();
        if (i==0) {  // \frac{\partial(L)}{\partial f(x_i^a)}
              caffe_cpu_axpby(  
                  channels,  
                  alpha,          // 2   
                  diff_np_.cpu_data() + (j*channels),  
                  Dtype(0.0),  
                  bout + (j*channels));  
        } else if (i==1) {  // \frac{\partial(L)}{\partial f(x_i^p)}
              caffe_cpu_axpby(  
                  channels,  
                  alpha,         // -2
                  diff_ap_.cpu_data() + (j*channels),  
                  Dtype(0.0),  
                  bout + (j*channels));  
        } else if (i==2) {  // \frac{\partial(L)}{\partial f(x_i^n)}
              caffe_cpu_axpby(  
                  channels,  
                  alpha,         // 2
                  diff_an_.cpu_data() + (j*channels),  
                  Dtype(0.0),  
                  bout + (j*channels));   
        }
      } 
    }
  }
}

#ifdef CPU_ONLY
STUB_GPU(TripletLossLayer);
#endif

INSTANTIATE_CLASS(TripletLossLayer);
REGISTER_LAYER_CLASS(TripletLoss);

}  // namespace caffe
123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899100101102103104105106107108109110111112113114115116117118119120121122123124125
```

#### 4.创建triplet_loss_layer.cu

```
#include <algorithm>
#include <vector>

#include "caffe/layers/triplet_loss_layer.hpp"
#include "caffe/util/math_functions.hpp"
#include "caffe/util/io.hpp" 

namespace caffe {

template <typename Dtype>
void TripletLossLayer<Dtype>::Forward_gpu(
    const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
  const int count = bottom[0]->count();
  caffe_gpu_sub(  
      count,  
      bottom[2]->gpu_data(),  // f(x_i^n)
      bottom[1]->gpu_data(),  // f(x_i^p)
      diff_np_.mutable_gpu_data());  // f(x_i^n)-f(x_i^p)
  caffe_gpu_sub(  
      count,  
      bottom[0]->gpu_data(),  // f(x_i^a)
      bottom[1]->gpu_data(),  // f(x_i^p)
      diff_ap_.mutable_gpu_data());  // f(x_i^a)-f(x_i^p)
  caffe_gpu_sub(  
      count,  
      bottom[0]->gpu_data(),  // f(x_i^a)
      bottom[2]->gpu_data(),  // f(x_i^n)
      diff_an_.mutable_gpu_data());  // f(x_i^a)-f(x_i^n)
  caffe_gpu_powx(  
      count,  
      diff_ap_.mutable_gpu_data(),  // f(x_i^a)-f(x_i^p)
      Dtype(2),  
      diff_ap_sq_.mutable_gpu_data());  // (f(x_i^a)-f(x_i^p)).^2 
  caffe_gpu_powx(  
      count,  
      diff_an_.mutable_gpu_data(),  // f(x_i^a)-f(x_i^n)
      Dtype(2),  
      diff_an_sq_.mutable_gpu_data());  // (f(x_i^a)-f(x_i^n)).^2 
  caffe_gpu_gemv(
      CblasNoTrans,
      bottom[0]->num(),
      bottom[0]->channels(),
      Dtype(1.0),               //alpha
      diff_ap_sq_.gpu_data(),   // A :  (f(x_i^a)-f(x_i^p)).^2 
      summer_vec_.gpu_data(),   // x :  [ 1, 1, 1,...,1 ]
      Dtype(0.0),               // beta
      dist_ap_sq_.mutable_gpu_data());  // y :||f(x_i^a)-f(x_i^p)||^2 
  caffe_gpu_gemv(
      CblasNoTrans,
      bottom[0]->num(),
      bottom[0]->channels(),
      Dtype(1.0),               //alpha 
      diff_an_sq_.gpu_data(),   // A :  (f(x_i^a)-f(x_i^n)).^2 
      summer_vec_.gpu_data(),   // x :  [ 1, 1, 1,...,1 ]
      Dtype(0.0),               // beta
      dist_an_sq_.mutable_gpu_data());  // y :||f(x_i^a)-f(x_i^n)||^2 
  Dtype margin = this->layer_param_.triplet_loss_param().margin();
  Dtype loss(0.0);
for (int i = 0; i < bottom[0]->num(); ++i) {  
     loss += std::max(margin +dist_ap_sq_.cpu_data()[i]- dist_an_sq_.cpu_data()[i], Dtype(0.0));  
  }  
  top[0]->mutable_cpu_data()[0] = loss;
}

template <typename Dtype>  
__global__ void CLLBackward(const int count, const int channels,  
    const Dtype margin, const Dtype alpha, const Dtype* diff,
    const Dtype* dist_ap_sq_, const Dtype* dist_an_sq_,  
    Dtype *bottom_diff) {  
  CUDA_KERNEL_LOOP(i, count) {  
    int n = i / channels;  // the num index, to access dist_ap_sq_ and dist_an_sq_  
    Dtype trip_dist(0.0);  
    trip_dist= margin + dist_sq_ap_[n] - dist_sq_an_[n];  
    if (trip_dist> 0.0) {  
        bottom_diff[i] = alpha * diff[i];  
    } else {  
        bottom_diff[i] = 0;  
    }  
  }  
}  

template <typename Dtype>
void TripletLossLayer<Dtype>::Backward_gpu(const vector<Blob<Dtype>*>& top,
    const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom) {
  Dtype margin = this->layer_param_.triplet_loss_param().margin(); 
  const int count = bottom[0]->count();  
  const int channels = bottom[0]->channels(); 

  for (int i = 0; i < 3; ++i) {
    if (propagate_down[i]) {
      const Dtype sign = (i == 1) ? -2 : 2;
      const Dtype alpha = sign * top[0]->cpu_diff()[0];
      // NOLINT_NEXT_LINE(whitespace/operators)
      if(i==0){     // \frac{\partial(L)}{\partial f(x_i^a)}
          // NOLINT_NEXT_LINE(whitespace/operators)  
          CLLBackward<Dtype><<<CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS>>>(  
              count, channels, margin, alpha,  
              diff_np_.gpu_data(),  //  f(x_i^n)-f(x_i^p)
              dist_ap_sq_.gpu_data(),  // ||f(x_i^a)-f(x_i^p)||^2   
              dist_an_sq_.gpu_data(),  // ||f(x_i^a)-f(x_i^n)||^2 
              bottom[i]->mutable_gpu_diff());  
          CUDA_POST_KERNEL_CHECK;  
      }else if(i==1){    // \frac{\partial(L)}{\partial f(x_i^p)}
          // NOLINT_NEXT_LINE(whitespace/operators)  
          CLLBackward<Dtype><<<CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS>>>(  
              count, channels, margin, alpha,   
              diff_ap_.gpu_data(),  //  f(x_i^a)-f(x_i^p)
              dist_ap_sq_.gpu_data(),  // ||f(x_i^a)-f(x_i^p)||^2   
              dist_an_sq_.gpu_data(),  // ||f(x_i^a)-f(x_i^n)||^2   
              bottom[i]->mutable_gpu_diff());  
          CUDA_POST_KERNEL_CHECK;  
      }else if(i==2){   // \frac{\partial(L)}{\partial f(x_i^n)}
          // NOLINT_NEXT_LINE(whitespace/operators)  
          CLLBackward<Dtype><<<CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS>>>(  
              count, channels, margin, alpha,  
              diff_an_.gpu_data(),  //  f(x_i^a)-f(x_i^n)
              dist_ap_sq_.gpu_data(),  // ||f(x_i^a)-f(x_i^p)||^2   
              dist_an_sq_.gpu_data(),  // ||f(x_i^a)-f(x_i^n)||^2 
              bottom[i]->mutable_gpu_diff());  
          CUDA_POST_KERNEL_CHECK;  
      }
    }
  }
}

INSTANTIATE_LAYER_GPU_FUNCS(TripletLossLayer);

}  // namespace caffe
123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899100101102103104105106107108109110111112113114115116117118119120121122123124125126127128129
```

#### 5.创建test_triplet_loss_layer.cpp

```
#include <algorithm>
#include <cmath>
#include <vector>

#include "gtest/gtest.h"

#include "caffe/blob.hpp"
#include "caffe/common.hpp"
#include "caffe/filler.hpp"
#include "caffe/layers/triplet_loss_layer.hpp"

#include "caffe/test/test_caffe_main.hpp"
#include "caffe/test/test_gradient_check_util.hpp"

namespace caffe {

template <typename TypeParam>
class TripletLossLayerTest : public MultiDeviceTest<TypeParam> {
  typedef typename TypeParam::Dtype Dtype;

 protected:
  TripletLossLayerTest ()
      : blob_bottom_data_i_(new Blob<Dtype>(512, 2, 1, 1)),
        blob_bottom_data_j_(new Blob<Dtype>(512, 2, 1, 1)),
        blob_bottom_data_k_(new Blob<Dtype>(512, 2, 1, 1)),
        blob_top_loss_(new Blob<Dtype>()) {
    // fill the values
    FillerParameter filler_param;
    filler_param.set_min(-1.0);
    filler_param.set_max(1.0);  // distances~=1.0 to test both sides of margin
    UniformFiller<Dtype> filler(filler_param);
    filler.Fill(this->blob_bottom_data_i_);
    blob_bottom_vec_.push_back(blob_bottom_data_i_);
    filler.Fill(this->blob_bottom_data_j_);
    blob_bottom_vec_.push_back(blob_bottom_data_j_);
    filler.Fill(this->blob_bottom_data_k_);  
    blob_bottom_vec_.push_back(blob_bottom_data_k_);  
    blob_top_vec_.push_back(blob_top_loss_);
  }
  virtual ~TripletLossLayerTest () {
    delete blob_bottom_data_i_;
    delete blob_bottom_data_j_;
    delete blob_bottom_data_k_;
    delete blob_top_loss_;
  }

  Blob<Dtype>* const blob_bottom_data_i_; // f(x_i^a)
  Blob<Dtype>* const blob_bottom_data_j_; // f(x_i^p)
  Blob<Dtype>* const blob_bottom_data_k_; // f(x_i^n)
  Blob<Dtype>* const blob_top_loss_;      // loss
  vector<Blob<Dtype>*> blob_bottom_vec_;  //bottom
  vector<Blob<Dtype>*> blob_top_vec_;     //top
};

TYPED_TEST_CASE(TripletLossLayerTest , TestDtypesAndDevices);

TYPED_TEST(TripletLossLayerTest , TestForward) {
  typedef typename TypeParam::Dtype Dtype;
  LayerParameter layer_param;
  TripletLossLayer<Dtype> layer(layer_param);
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  // manually compute to compare
  const Dtype margin = layer_param.triplet_loss_param().margin();
  const int num = this->blob_bottom_data_i_->num();
  const int channels = this->blob_bottom_data_i_->channels();
  Dtype loss(0);

  for (int i = 0; i < num; ++i) {
    Dtype dist_sq_ap(0);
    Dtype dist_sq_an(0);
    for (int j = 0; j < channels; ++j) {
      Dtype diff_ap = this->blob_bottom_data_i_->cpu_data()[i*channels+j] -
          this->blob_bottom_data_j_->cpu_data()[i*channels+j];
      dist_sq_ap += diff_ap * diff_ap;
      Dtype diff_an = this->blob_bottom_data_i_->cpu_data()[i*channels+j] -
          this->blob_bottom_data_k_->cpu_data()[i*channels+j];
      dist_sq_an += diff_an * diff_an;
    }
    loss += std::max(Dtype(0.0), margin + dist_sq_ap - dist_sq_an);
  }
  EXPECT_NEAR(this->blob_top_loss_->cpu_data()[0], loss, 1e-6);
}

TYPED_TEST(TripletLossLayerTest, TestGradient) {
  typedef typename TypeParam::Dtype Dtype;
  LayerParameter layer_param;
  TripletLossLayer<Dtype> layer(layer_param);
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  GradientChecker<Dtype> checker(1e-2, 1e-2, 1701);
  // check the gradient for the three bottom layers
  checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
      this->blob_top_vec_, 0);  // check gradient for f(x_i^a)
  checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
      this->blob_top_vec_, 1);  // check gradient for f(x_i^p)
  checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
      this->blob_top_vec_, 2);  // check gradient for f(x_i^n)
}
}  // namespace caffe
```