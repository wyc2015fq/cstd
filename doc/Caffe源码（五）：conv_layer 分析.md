# Caffe源码（五）：conv_layer 分析

2015年08月14日 18:23:14 [seven_first](https://me.csdn.net/seven_first) 阅读数：7515



# 目录



- [目录](https://blog.csdn.net/seven_first/article/details/47665817#%E7%9B%AE%E5%BD%95)
- [简单介绍](https://blog.csdn.net/seven_first/article/details/47665817#%E7%AE%80%E5%8D%95%E4%BB%8B%E7%BB%8D)
- 主要函数
  - [compute_output_shape 函数](https://blog.csdn.net/seven_first/article/details/47665817#1-computeoutputshape-%E5%87%BD%E6%95%B0)
  - [Forward_cpu 函数](https://blog.csdn.net/seven_first/article/details/47665817#2forwardcpu-%E5%87%BD%E6%95%B0)
  - [Backward_cpu 函数](https://blog.csdn.net/seven_first/article/details/47665817#3backwardcpu-%E5%87%BD%E6%95%B0)



# 简单介绍

```
首先要明确的一点是：ConvolutionLayer 是 BaseConvolutionLayer的子类，BaseConvolutionLayer 是 Layer 的子类。ConvolutionLayer 除了继承了相应的成员变量和函数以外，自己的成员函数主要有：compute_output_shape，Forward_cpu，Backward_cpu 。
1
```

# 主要函数

## 1. compute_output_shape 函数：

计算输出feature map 的shape。

```
template <typename Dtype>
void ConvolutionLayer<Dtype>::compute_output_shape() {
  this->height_out_ = (this->height_ + 2 * this->pad_h_ - this->kernel_h_)
      / this->stride_h_ + 1;  //输出feature map 的 height
  this->width_out_ = (this->width_ + 2 * this->pad_w_ - this->kernel_w_)
      / this->stride_w_ + 1;  //输出 feature map 的 width 
}
12345678
```

## 2.Forward_cpu 函数：

该函数在Layer 中声明，实现前向传播功能。

```
template <typename Dtype>
void ConvolutionLayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  const Dtype* weight = this->blobs_[0]->cpu_data();
   //blobs_ 用来存储可学习的参数blobs_[0] 是weight，blobs_[1]是bias
  for (int i = 0; i < bottom.size(); ++i) { 
  //这里的i为输入bottom的个数，输入多少个bottom就产生相应个数的输出 top。
    const Dtype* bottom_data = bottom[i]->cpu_data();
    Dtype* top_data = top[i]->mutable_cpu_data();
    for (int n = 0; n < this->num_; ++n) {
      this->forward_cpu_gemm(bottom_data + bottom[i]->offset(n), weight,
          top_data + top[i]->offset(n));//计算卷积操作之后的输出
      if (this->bias_term_) {
        const Dtype* bias = this->blobs_[1]->cpu_data();
        this->forward_cpu_bias(top_data + top[i]->offset(n), bias);
      }//加上bias
    }
  }
}12345678910111213141516171819
```

Layer的构造函数

```
explicit Layer(const LayerParameter& param)
    : layer_param_(param) {
      // Set phase and copy blobs (if there are any).
      phase_ = param.phase();
      if (layer_param_.blobs_size() > 0) {
        blobs_.resize(layer_param_.blobs_size());
        for (int i = 0; i < layer_param_.blobs_size(); ++i) {
          blobs_[i].reset(new Blob<Dtype>());
          blobs_[i]->FromProto(layer_param_.blobs(i));
        }
      }
    }//用从protobuf 读入message LayerParameter 中的blobs 初始化 blobs_ 
     //blobs_定义：vector<shared_ptr<Blob<Dtype> > > blobs_
1234567891011121314
```

## 3.Backward_cpu 函数

实现反向传播，根据上一层传下来的导数计算相应的bottom data ， weight， bias 的导数

```
template <typename Dtype>
void ConvolutionLayer<Dtype>::Backward_cpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom) {
  const Dtype* weight = this->blobs_[0]->cpu_data();
  Dtype* weight_diff = this->blobs_[0]->mutable_cpu_diff();
  if (this->param_propagate_down_[0]) {
    caffe_set(this->blobs_[0]->count(), Dtype(0), weight_diff);
  }
  if (this->bias_term_ && this->param_propagate_down_[1]) {
    caffe_set(this->blobs_[1]->count(), Dtype(0),
        this->blobs_[1]->mutable_cpu_diff());
  }
  for (int i = 0; i < top.size(); ++i) {
    const Dtype* top_diff = top[i]->cpu_diff();//上一层传下来的导数
    const Dtype* bottom_data = bottom[i]->cpu_data();
    Dtype* bottom_diff = bottom[i]->mutable_cpu_diff();//传给下一层的导数
    // Bias gradient, if necessary.
    if (this->bias_term_ && this->param_propagate_down_[1]) {
      Dtype* bias_diff = this->blobs_[1]->mutable_cpu_diff();
      for (int n = 0; n < this->num_; ++n) {
        this->backward_cpu_bias(bias_diff, top_diff + top[i]->offset(n));
      }
    }
    if (this->param_propagate_down_[0] || propagate_down[i]) {
      for (int n = 0; n < this->num_; ++n) {
        // gradient w.r.t. weight. Note that we will accumulate diffs.
        if (this->param_propagate_down_[0]) {
          this->weight_cpu_gemm(bottom_data + bottom[i]->offset(n),
              top_diff + top[i]->offset(n), weight_diff);
        }//对weight 计算导数（用来更新weight）
        // gradient w.r.t. bottom data, if necessary.
        if (propagate_down[i]) {
          this->backward_cpu_gemm(top_diff + top[i]->offset(n), weight,
              bottom_diff + bottom[i]->offset(n));
        }//对bottom数据计算导数（传给下一层）
      }
    }
  }
}
```