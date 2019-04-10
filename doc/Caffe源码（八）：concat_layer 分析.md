# Caffe源码（八）：concat_layer 分析

2015年08月17日 15:08:39 [seven_first](https://me.csdn.net/seven_first) 阅读数：10149



# 简单介绍

concat_layer 用来实现两个或者多个blob的链接，支持在num 维度上的链接（concat_dim = 0 : $(n_1+n_2+...+n_k)*c*h*w$ 和channel维度上的链接（concat_dim = 1 : $n*(c_1+c_2+...+c_k)*h*w$。

# 主要函数

## 1.LayerSetUp 函数：

```
template <typename Dtype>
void ConcatLayer<Dtype>::LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  const ConcatParameter& concat_param = this->layer_param_.concat_param();
  CHECK(!(concat_param.has_axis() && concat_param.has_concat_dim()))
      << "Either axis or concat_dim should be specified; not both.";
}//axis ，dim ：0 为 num 维度链接，1 为 channel 维度链接1234567
```

## 2.Reshape 函数：

```
template <typename Dtype>
void ConcatLayer<Dtype>::Reshape(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  const int num_axes = bottom[0]->num_axes();
  const ConcatParameter& concat_param = this->layer_param_.concat_param();
  if (concat_param.has_concat_dim()) {
    concat_axis_ = static_cast<int>(concat_param.concat_dim());
    // Don't allow negative indexing for concat_dim, a uint32 -- almost
    // certainly unintended.
    CHECK_GE(concat_axis_, 0) << "casting concat_dim from uint32 to int32 "
        << "produced negative result; concat_dim must satisfy "
        << "0 <= concat_dim < " << kMaxBlobAxes;
    CHECK_LT(concat_axis_, num_axes) << "concat_dim out of range.";
  } else {
    concat_axis_ = bottom[0]->CanonicalAxisIndex(concat_param.axis());
  }
  // Initialize with the first blob.  blob: num x channel x height x width
  vector<int> top_shape = bottom[0]->shape();
  num_concats_ = bottom[0]->count(0, concat_axis_);
  //concat_axis_ = 0 : num_concats_=num;concat_axis_ = 1 : num_concats_=num x channel;
  concat_input_size_ = bottom[0]->count(concat_axis_ + 1);
  //concat_axis_ = 0 : concat_input_size_=channel x height x width;
  //concat_axis_ = 1 : num_concats_=height x width;
  int bottom_count_sum = bottom[0]->count();
  for (int i = 1; i < bottom.size(); ++i) {
    CHECK_EQ(num_axes, bottom[i]->num_axes())
        << "All inputs must have the same #axes.";
    for (int j = 0; j < num_axes; ++j) {
      if (j == concat_axis_) { continue; }
      CHECK_EQ(top_shape[j], bottom[i]->shape(j))
          << "All inputs must have the same shape, except at concat_axis.";
    }
    bottom_count_sum += bottom[i]->count();
    top_shape[concat_axis_] += bottom[i]->shape(concat_axis_);
  }
  top[0]->Reshape(top_shape);
  CHECK_EQ(bottom_count_sum, top[0]->count());
}1234567891011121314151617181920212223242526272829303132333435363738
```

## 3.Forward_cpu 函数：

```
template <typename Dtype>
void ConcatLayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  Dtype* top_data = top[0]->mutable_cpu_data();
  int offset_concat_axis = 0;
  const int top_concat_axis = top[0]->shape(concat_axis_);
  for (int i = 0; i < bottom.size(); ++i) {//遍历所有输入bottom
    const Dtype* bottom_data = bottom[i]->cpu_data();
    const int bottom_concat_axis = bottom[i]->shape(concat_axis_);
    for (int n = 0; n < num_concats_; ++n) {
      caffe_copy(bottom_concat_axis * concat_input_size_,
          //case 0：num x channel x h x w;case 1: channel x h x w
          bottom_data + n * bottom_concat_axis * concat_input_size_,
          //case 0：bottom_data + n x num x channel x h x w ;
          //case 1:bottom_data + n x channel x h x w ;
          top_data + (n * top_concat_axis + offset_concat_axis)
              * concat_input_size_);
    }//把 各个bottom data 拷贝到输出 top data 的对应位置
    offset_concat_axis += bottom_concat_axis;
  }
}123456789101112131415161718192021
```

## 4. Backward_cpu 函数：

```
template <typename Dtype>
void ConcatLayer<Dtype>::Backward_cpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom) {
  const Dtype* top_diff = top[0]->cpu_diff();
  int offset_concat_axis = 0;
  const int top_concat_axis = top[0]->shape(concat_axis_);
  for (int i = 0; i < bottom.size(); ++i) {
    if (!propagate_down[i]) { continue; }
    Dtype* bottom_diff = bottom[i]->mutable_cpu_diff();
    const int bottom_concat_axis = bottom[i]->shape(concat_axis_);
    for (int n = 0; n < num_concats_; ++n) {
      caffe_copy(bottom_concat_axis * concat_input_size_, top_diff +
          (n * top_concat_axis + offset_concat_axis) * concat_input_size_,
          bottom_diff + n * bottom_concat_axis * concat_input_size_);
    }
    offset_concat_axis += bottom_concat_axis;
  }//对 diff 做和data相同的链接
}
```