# 在caffe 中实现Generative Adversarial Nets（二）

2016年11月09日 15:23:18 [seven_first](https://me.csdn.net/seven_first) 阅读数：8019



# 目录



- 目录
  - - 一添加Loss Function
      - [gan_loss_layerhpp](https://blog.csdn.net/seven_first/article/details/53100325#1-ganlosslayerhpp)
      - [gan_loss_layercpp](https://blog.csdn.net/seven_first/article/details/53100325#1-ganlosslayercpp)
    - 二添加gan_gate_layer
      - [gan_gate_layerhpp](https://blog.csdn.net/seven_first/article/details/53100325#gangatelayerhpp)
      - [gan_gate_layercpp](https://blog.csdn.net/seven_first/article/details/53100325#gangatelayercpp)
    - 三添加rand_vec_layer
      - [rand_vec_layerhpp](https://blog.csdn.net/seven_first/article/details/53100325#randveclayerhpp)
      - [rand_vec_layercpp](https://blog.csdn.net/seven_first/article/details/53100325#randveclayercpp)
    - [四修改Solver中Step函数](https://blog.csdn.net/seven_first/article/details/53100325#%E5%9B%9B%E4%BF%AE%E6%94%B9solver%E4%B8%ADstep%E5%87%BD%E6%95%B0)



### 一、添加Loss Function

#### 1. gan_loss_layer.hpp

```
//  Create on: 2016/10/19 ShanghaiTech
//  Author:    Yingying Zhang

#ifndef CAFFE_GAN_LOSS_LAYER_HPP_
#define CAFFE_GAN_LOSS_LAYER_HPP_

#include <vector>

#include "caffe/blob.hpp"
#include "caffe/layer.hpp"
#include "caffe/proto/caffe.pb.h"

#include "caffe/layers/loss_layer.hpp"

namespace caffe {

/*This loss layer computes loss for GAN.
 *  1. loss for discriminator
 *     bottom[0] : D(x)
 *     bottom[1] : D(G(z))
 *  2. loss for generator
 *     bottom[0] : D(G(z))
 */


template <typename Dtype>
class GANLossLayer : public LossLayer<Dtype> {
 public:
  explicit GANLossLayer(const LayerParameter& param)
      : LossLayer<Dtype>(param)  {}
  virtual void LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Reshape(
      const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
      vector<int> loss_shape(0);  // Loss layers output a scalar; 0 axes.
      top[0]->Reshape(loss_shape);
  }
  virtual inline int ExactNumBottomBlobs() const { return -1; }
  virtual inline int MinBottomBlobs() const { return 1; }
  virtual inline int MaxBottomBlobs() const { return 2; }
  virtual inline const char* type() const { return "GANLoss"; }
  virtual inline bool AllowForceBackward(const int bottom_index) const {
    return true;
  }

 protected:
  virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Backward_cpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);

  int dis_iter_;
  int gen_iter_;
  int iter_idx_;
};

template <typename Dtype>
class GANDGLossLayer : public LossLayer<Dtype> {
 public:
  explicit GANDGLossLayer(const LayerParameter& param)
      : LossLayer<Dtype>(param)  {}
  virtual void LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Reshape(
      const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
      vector<int> loss_shape(0);  // Loss layers output a scalar; 0 axes.
      top[0]->Reshape(loss_shape);
  }
  virtual inline int ExactNumBottomBlobs() const { return 1; }
  virtual inline const char* type() const { return "GANDGLoss"; }
  virtual inline bool AllowForceBackward(const int bottom_index) const {
    return true;
  }

 protected:
  virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Backward_cpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);

  int dis_iter_;
  int gen_iter_;
  int gan_mode_;
  int diter_idx_;
  int giter_idx_;
};


}  // namespace caffe

#endif  // CAFFE_GAN_LOSS_LAYER_HPP_
1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556575859606162636465666768697071727374757677787980818283848586878889909192
```

#### 1. gan_loss_layer.cpp

```
//  Create on: 2016/10/19 ShanghaiTech
//  Author:    Yingying Zhang

#include <algorithm>
#include <vector>

#include "caffe/layers/gan_loss_layer.hpp"


namespace caffe {

template <typename Dtype>
void GANLossLayer<Dtype>::LayerSetUp(
  const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
      LossLayer<Dtype>::LayerSetUp(bottom, top);
      iter_idx_ = 0;
      dis_iter_ = this->layer_param_.gan_loss_param().dis_iter();
      gen_iter_ = this->layer_param_.gan_loss_param().gen_iter();
      //discriminative mode
      if (bottom.size() == 2) {
        CHECK_EQ(bottom[0]->shape(0), bottom[1]->shape(0));
        CHECK_EQ(bottom[0]->shape(1), 1);
        CHECK_EQ(bottom[1]->shape(1), 1);
      }
      //generative mode
      if (bottom.size() == 1) {
        CHECK_EQ(bottom[0]->shape(1), 1);
      }
}

template <typename Dtype>
void GANLossLayer<Dtype>::Forward_cpu(
    const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top) {
      int batch_size = bottom[0]->count();
      Dtype loss(0.0);
      //1. discriminative mode
      if (bottom.size() == 2) {
        const Dtype* score1 = bottom[0]->cpu_data();
        const Dtype* score2 = bottom[1]->cpu_data();
        for(int i = 0; i<batch_size; ++i) {
          loss -= std::log(score1[i]) + std::log(1 - score2[i]);
        }
      }
      //2. generative mode
      if (bottom.size() == 1) {
        const Dtype* score = bottom[0]->cpu_data();
        for(int i = 0; i<batch_size; ++i) {
          loss -= std::log(score[i]);
        }
      }
      loss /= static_cast<Dtype>(batch_size);
      top[0]->mutable_cpu_data()[0] = loss;
      iter_idx_++;
}

template <typename Dtype>
void GANLossLayer<Dtype>::Backward_cpu(const vector<Blob<Dtype>*>& top,
    const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom) {
      int batch_size = bottom[0]->count();
      //1. discriminative mode
      if (bottom.size() == 2) {
        if (iter_idx_ % dis_iter_ == 0) {
          for (int i = 0; i<batch_size; ++i) {
            bottom[0]->mutable_cpu_diff()[i] = Dtype(-1) /
                    bottom[0]->cpu_data()[i] / static_cast<Dtype>(batch_size);
            bottom[1]->mutable_cpu_diff()[i] = Dtype(-1) /
                    (bottom[1]->cpu_data()[i] - Dtype(1))  / static_cast<Dtype>(batch_size);
          }
        } else {
          for (int i = 0; i<batch_size; ++i) {
            bottom[0]->mutable_cpu_diff()[i] = Dtype(0);
            bottom[1]->mutable_cpu_diff()[i] = Dtype(0);
          }
        }
      }
      //2. generative mode
      if (bottom.size() == 1) {
        if (iter_idx_ % gen_iter_ == 0) {
          for (int i = 0; i<batch_size; ++i) {
            bottom[0]->mutable_cpu_diff()[i] = Dtype(-1) /
                    bottom[0]->cpu_data()[i] / static_cast<Dtype>(batch_size);
          }
        } else {
          for (int i = 0; i<batch_size; ++i) {
            bottom[0]->mutable_cpu_diff()[i] = Dtype(0);
          }
        }
      }
}

INSTANTIATE_CLASS(GANLossLayer);
REGISTER_LAYER_CLASS(GANLoss);


template <typename Dtype>
void GANDGLossLayer<Dtype>::LayerSetUp(
  const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
      LossLayer<Dtype>::LayerSetUp(bottom, top);
      diter_idx_ = 0;
      giter_idx_ = 0;
      dis_iter_ = this->layer_param_.gan_loss_param().dis_iter();
      gen_iter_ = this->layer_param_.gan_loss_param().gen_iter();
      gan_mode_ = 1;
}

template <typename Dtype>
void GANDGLossLayer<Dtype>::Forward_cpu(
  const vector<Blob<Dtype>*>& bottom,
  const vector<Blob<Dtype>*>& top) {
  int batch_size = bottom[0]->count();
  const Dtype* score = bottom[0]->cpu_data();
  Dtype loss(0.0);
  //when gan_mode_ = 1, the input of loss is D(x)
  //loss is discriminative loss: -log(D(x))
  if (gan_mode_ == 1) {
    diter_idx_++;
    for(int i = 0; i<batch_size; ++i) {
      loss -= std::log(score[i]);
    }
  }
  //when gan_mode_ = 2, the input of loss is D(G(z))
  //loss is discriminative loss: -log(1-D(G(z)))
  if (gan_mode_ == 2){
    for(int i = 0; i<batch_size; ++i) {
      loss -= std::log(1 - score[i]);
    }
  }
  //when gan_mode_ = 3, the input of loss is D(G(z))
  //loss is generative loss: -log(D(G(z)))
  if (gan_mode_ == 3){
    giter_idx_++;
    for(int i = 0; i<batch_size; ++i) {
      loss -= std::log(score[i]);
    }
  }
  loss /= static_cast<Dtype>(batch_size);
  top[0]->mutable_cpu_data()[0] = loss;
}

template <typename Dtype>
void GANDGLossLayer<Dtype>::Backward_cpu(const vector<Blob<Dtype>*>& top,
  const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom) {
  int batch_size = bottom[0]->count();
  //when gan_mode_ = 1, the input of loss is D(x)
  //backward for discriminative loss
  if (gan_mode_ == 1) {
    if (diter_idx_ % dis_iter_ == 0 ) {
      for(int i = 0; i<batch_size; ++i) {
        bottom[0]->mutable_cpu_diff()[i] = Dtype(-1) /
          bottom[0]->cpu_data()[i] / static_cast<Dtype>(batch_size);
      }
    } else {
      for (int i = 0; i<batch_size; ++i) {
        bottom[0]->mutable_cpu_diff()[i] = Dtype(0);
      }
    }
  }
  //when gan_mode_ = 2, the input of loss is D(G(z))
  //backward for discriminative loss
  if (gan_mode_ == 2){
    if (diter_idx_ % dis_iter_ == 0 ) {
      for(int i = 0; i<batch_size; ++i) {
        bottom[0]->mutable_cpu_diff()[i] = Dtype(-1) /
          (bottom[0]->cpu_data()[i] - Dtype(1))  / static_cast<Dtype>(batch_size);
      }
    } else {
      for (int i = 0; i<batch_size; ++i) {
        bottom[0]->mutable_cpu_diff()[i] = Dtype(0);
      }
    }
  }
  //when gan_mode_ = 3, the input of loss is D(G(z))
  //backward for generative loss
  if (gan_mode_ == 3){
    if (giter_idx_ % gen_iter_ == 0 ) {
      for(int i = 0; i<batch_size; ++i) {
        bottom[0]->mutable_cpu_diff()[i] = Dtype(-1) /
          bottom[0]->cpu_data()[i] / static_cast<Dtype>(batch_size);
      }
    } else {
      for (int i = 0; i<batch_size; ++i) {
        bottom[0]->mutable_cpu_diff()[i] = Dtype(0);
      }
    }
  }
  // update gan_mode_
  gan_mode_ = gan_mode_ == 3 ? 1 : gan_mode_ + 1;

}



INSTANTIATE_CLASS(GANDGLossLayer);
REGISTER_LAYER_CLASS(GANDGLoss);

}  // namespace caffe
123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899100101102103104105106107108109110111112113114115116117118119120121122123124125126127128129130131132133134135136137138139140141142143144145146147148149150151152153154155156157158159160161162163164165166167168169170171172173174175176177178179180181182183184185186187188189190191192193194195196197198
```

### 二、添加gan_gate_layer

#### gan_gate_layer.hpp

```
//  Create on: 2016/10/19 ShanghaiTech
//  Author:    Yingying Zhang


#ifndef CAFFE_GAN_GATE_LAYER_HPP_
#define CAFFE_GAN_GATE_LAYER_HPP_

#include <vector>
#include "caffe/blob.hpp"
#include "caffe/layer.hpp"

namespace caffe {

template <typename Dtype>
class GANGateLayer : public Layer<Dtype> {
 public:
  explicit GANGateLayer(const LayerParameter& param)
    : Layer<Dtype>(param) {}
  virtual void LayerSetUp(const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top);
  virtual void Reshape(const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top) {}

  virtual inline const char* type() const { return "GANGate"; }
  virtual inline int ExactNumBottomBlobs() const { return 2; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

 protected:
  virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top);
  virtual void Backward_cpu(const vector<Blob<Dtype>*>& top,
    const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom) {}
  int gan_mode_;
};

}  // namespace caffe

#endif  // CAFFE_GAN_GATE_LAYER_HPP_
123456789101112131415161718192021222324252627282930313233343536373839
```

#### gan_gate_layer.cpp

```
//  Create on: 2016/10/21 ShanghaiTech
//  Author:    Yingying Zhang


#include <vector>
#include "caffe/layers/gan_gate_layer.hpp"


namespace caffe {

  template <typename Dtype>
  void GANGateLayer<Dtype>::LayerSetUp(
    const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
    gan_mode_ = 1;
    top[0]->ReshapeLike(*bottom[0]);
  }

  template <typename Dtype>
  void GANGateLayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top) {
    CHECK_EQ(bottom[0]->num(), bottom[1]->num());
    CHECK_EQ(bottom[0]->channels(), bottom[1]->channels());
    CHECK_EQ(bottom[0]->height(), bottom[1]->height());
    CHECK_EQ(bottom[0]->width(), bottom[1]->width());
    int index = gan_mode_ == 1 ? 0 : 1;
    top[0]->ReshapeLike(*bottom[index]);
    top[0]->ShareData(*bottom[index]);
    top[0]->ShareDiff(*bottom[index]);
    gan_mode_ = gan_mode_ == 3 ? 1 : gan_mode_ + 1;
  }

  INSTANTIATE_CLASS(GANGateLayer);
  REGISTER_LAYER_CLASS(GANGate);

}  // namespace caffe1234567891011121314151617181920212223242526272829303132333435
```

### 三、添加rand_vec_layer

#### rand_vec_layer.hpp

```
//  Create on: 2016/10/19 ShanghaiTech
//  Author:    Yingying Zhang


#ifndef CAFFE_RAND_VEC_LAYER_HPP_
#define CAFFE_RAND_VEC_LAYER_HPP_

#include <vector>

#include "caffe/blob.hpp"
#include "caffe/layer.hpp"
#include "caffe/proto/caffe.pb.h"
#include "caffe/util/rng.hpp"
namespace caffe {

template <typename Dtype>
class RandVecLayer : public Layer<Dtype> {
 public:
  explicit RandVecLayer(const LayerParameter& param)
    : Layer<Dtype>(param) {}
  virtual void LayerSetUp(const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top);
  virtual void Reshape(const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top) {}

  virtual inline const char* type() const { return "RandVec"; }
  virtual inline int ExactNumBottomBlobs() const { return 0; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

 protected:
  virtual Dtype GetRandom(const Dtype lower, const Dtype upper);
  virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top);
  virtual void Backward_cpu(const vector<Blob<Dtype>*>& top,
    const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom) {}


  shared_ptr<Caffe::RNG> data_rng_;
  int batch_size_;
  int dim_;
  Dtype lower_;
  Dtype upper_;
  int iter_idx_;
  int repeat_;
  int height_;
  int width_;
};

}  // namespace caffe

#endif  // CAFFE_RAND_VEC_LAYER_HPP_
12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152
```

#### rand_vec_layer.cpp

```
//  Create on: 2016/10/19 ShanghaiTech
//  Author:    Yingying Zhang


#include <vector>

#include "caffe/layer.hpp"
#include "caffe/layers/rand_vec_layer.hpp"

#define MAX_RANDOM 10000

namespace caffe {

  template <typename Dtype>
  Dtype RandVecLayer<Dtype>::GetRandom(const Dtype lower, const Dtype upper) {
    CHECK(data_rng_);
    CHECK_LT(lower, upper) << "Upper bound must be greater than lower bound!";
    caffe::rng_t* data_rng =
        static_cast<caffe::rng_t*>(data_rng_->generator());
    return static_cast<Dtype>(((*data_rng)()) % static_cast<unsigned int>(
        (upper - lower) * MAX_RANDOM)) / static_cast<Dtype>(MAX_RANDOM)+lower;
  }

  template <typename Dtype>
  void RandVecLayer<Dtype>::LayerSetUp(
    const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
    const RandVecParameter& rand_vec_param = this->layer_param_.rand_vec_param();
    batch_size_ = rand_vec_param.batch_size();
    dim_ = rand_vec_param.dim();
    height_ = rand_vec_param.height();
    width_ = rand_vec_param.width();
    lower_ = rand_vec_param.lower();
    upper_ = rand_vec_param.upper();
    repeat_ = rand_vec_param.repeat();
    iter_idx_ = 1;
    vector<int> top_shape(2);
    top_shape[0] = batch_size_;
    top_shape[1] = dim_;
    if (height_ >0 && width_>0) {
      top_shape.resize(4);
      top_shape[0] = batch_size_;
      top_shape[1] = dim_;
      top_shape[2] = height_;
      top_shape[3] = width_;
    }
    top[0]->Reshape(top_shape);
  }

  template <typename Dtype>
  void RandVecLayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top) {
    if (iter_idx_ == 1) {
      const unsigned int data_rng_seed = caffe_rng_rand();
      data_rng_.reset(new Caffe::RNG(data_rng_seed));
      int count = top[0]->count();
      for (int i = 0; i<count; ++i) {
        top[0]->mutable_cpu_data()[i] = GetRandom(lower_, upper_);
      }
    }
    //update iter_idx_
    iter_idx_ = iter_idx_ == repeat_ ? 1 : iter_idx_ + 1;
  }

  INSTANTIATE_CLASS(RandVecLayer);
  REGISTER_LAYER_CLASS(RandVec);

}  // namespace caffe
1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556575859606162636465666768
```

### 四、修改Solver中Step函数

```
template <typename Dtype>
void Solver<Dtype>::Step(int iters) {
  const int start_iter = iter_;
  const int stop_iter = iter_ + iters;
  int average_loss = this->param_.average_loss();
  bool gan_solver = this->param_.gan_solver();
  losses_.clear();
  smoothed_loss_ = 0;
  Dtype loss_buff = 0;
  int gan_iter_times = 2;

  while (iter_ < stop_iter) {
    // zero-init the params
    net_->ClearParamDiffs();
    if (param_.test_interval() && iter_ % param_.test_interval() == 0
        && (iter_ > 0 || param_.test_initialization())
        && Caffe::root_solver()) {
      TestAll();
      if (requested_early_exit_) {
        // Break out of the while loop because stop was requested while testing.
        break;
      }
    }

    for (int i = 0; i < callbacks_.size(); ++i) {
      callbacks_[i]->on_start();
    }
    const bool display = param_.display() && iter_ % param_.display() == 0;
    net_->set_debug_info(display && param_.debug_info());
    // accumulate the loss and gradient
    Dtype loss = 0;
    if (gan_solver) {
      for (int i = 0; i < gan_iter_times; ++i) {
        loss += net_->ForwardBackward();
      }
      gan_iter_times = gan_iter_times == 2 ? 1 : 2;
    } else {
      for (int i = 0; i < param_.iter_size(); ++i) {
        loss += net_->ForwardBackward();
      }
    }
    loss /= param_.iter_size();

    // average the loss across iterations for smoothed reporting
    UpdateSmoothedLoss(loss, start_iter, average_loss);
    if (display) {
      LOG_IF(INFO, Caffe::root_solver()) << "Iteration " << iter_
          << ", loss = " << smoothed_loss_;
      if (gan_solver) {
        if (gan_iter_times == 1 && iter_ != 0) {
          LOG_IF(INFO, Caffe::root_solver()) << "    dis_loss =  "  << loss;
          LOG_IF(INFO, Caffe::root_solver()) << "    gen_loss =  "  << loss_buff;
        }
      }
      const vector<Blob<Dtype>*>& result = net_->output_blobs();
      int score_index = 0;
      for (int j = 0; j < result.size(); ++j) {
        const Dtype* result_vec = result[j]->cpu_data();
        const string& output_name =
            net_->blob_names()[net_->output_blob_indices()[j]];
        const Dtype loss_weight =
            net_->blob_loss_weights()[net_->output_blob_indices()[j]];
        for (int k = 0; k < result[j]->count(); ++k) {
          ostringstream loss_msg_stream;
          if (loss_weight) {
            loss_msg_stream << " (* " << loss_weight
                            << " = " << loss_weight * result_vec[k] << " loss)";
          }
          LOG_IF(INFO, Caffe::root_solver()) << "    Train net output #"
              << score_index++ << ": " << output_name << " = "
              << result_vec[k] << loss_msg_stream.str();
        }
      }
    }
    loss_buff = loss;
    for (int i = 0; i < callbacks_.size(); ++i) {
      callbacks_[i]->on_gradients_ready();
    }
    ApplyUpdate();

    // Increment the internal iter_ counter -- its value should always indicate
    // the number of times the weights have been updated.
    ++iter_;

    SolverAction::Enum request = GetRequestedAction();

    // Save a snapshot if needed.
    if ((param_.snapshot()
         && iter_ % param_.snapshot() == 0
         && Caffe::root_solver()) ||
         (request == SolverAction::SNAPSHOT)) {
      Snapshot();
    }
    if (SolverAction::STOP == request) {
      requested_early_exit_ = true;
      // Break out of training loop.
      break;
    }
  }
}123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899100
```

PS：看到很多同学索要全部修改过的源码，现在给出下载链接：[GAN相关源码](http://pan.baidu.com/s/1mh5s6py)