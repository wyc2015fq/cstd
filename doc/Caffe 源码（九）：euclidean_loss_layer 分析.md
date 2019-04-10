# Caffe 源码（九）：euclidean_loss_layer 分析

2015年08月17日 16:21:30 [seven_first](https://me.csdn.net/seven_first) 阅读数：15444





# 简单介绍

Euclidean loss layer 计算两个输入的差的平方和：$\frac{1}{2N}\sum_{i = 1}^N||x_{1i}-x_{2i}||_2^2$

# 主要函数

## 1.Reshape 函数：

```
template <typename Dtype>
void EuclideanLossLayer<Dtype>::Reshape(
  const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
  LossLayer<Dtype>::Reshape(bottom, top);//在LossLayer 中定义
  CHECK_EQ(bottom[0]->count(1), bottom[1]->count(1))//保证输入维度相同
      << "Inputs must have the same dimension.";
  diff_.ReshapeLike(*bottom[0]);//Blob 类型的diff_用来存放两个bottom的差，和bottom具有相同的shape
}12345678
```

EuclideanLossLayer 是LossLayer的子类，因此我们必须明确用到的LossLayer 中的成员函数Reshape：

```
template <typename Dtype>
void LossLayer<Dtype>::Reshape(
    const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
  CHECK_EQ(bottom[0]->num(), bottom[1]->num())
      << "The data and label should have the same number.";
  vector<int> loss_shape(0);  // Loss layers output a scalar; 0 axes.
  top[0]->Reshape(loss_shape);
}//输出top是一个标量。12345678
```

## 2.Forward_cpu 函数：

```
template <typename Dtype>
void EuclideanLossLayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top) {
  int count = bottom[0]->count();
  caffe_sub(
      count,
      bottom[0]->cpu_data(),
      bottom[1]->cpu_data(),
      diff_.mutable_cpu_data());//diff_ = bottom[0] - bottom[1]
  Dtype dot = caffe_cpu_dot(count, diff_.cpu_data(), diff_.cpu_data());  // dot = ||diff_||^2
  Dtype loss = dot / bottom[0]->num() / Dtype(2);//输出的loss
  top[0]->mutable_cpu_data()[0] = loss;
}12345678910111213
```

## 3.Backward_cpu 函数：

```
template <typename Dtype>
void EuclideanLossLayer<Dtype>::Backward_cpu(const vector<Blob<Dtype>*>& top,
    const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom) {
  for (int i = 0; i < 2; ++i) {
    if (propagate_down[i]) {//对于输入的label bottom propagate_dowm 为0
      const Dtype sign = (i == 0) ? 1 : -1;//由于diff_ = bottom[0] - bottom[1]
      const Dtype alpha = sign * top[0]->cpu_diff()[0] / bottom[i]->num();
      caffe_cpu_axpby(
          bottom[i]->count(),              // count
          alpha,                              // alpha
          diff_.cpu_data(),                   // a
          Dtype(0),                           // beta
          bottom[i]->mutable_cpu_diff());  // b
    }//bottom[i]->mutable_cpu_diff()) = alpha*diff_.cpu_data()
  }
}
```