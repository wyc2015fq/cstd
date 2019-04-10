# Caffe源码（七）：ReLU，Sigmoid and Tanh

2015年08月16日 16:47:43 [seven_first](https://me.csdn.net/seven_first) 阅读数：6754





# 简单介绍

### ReLU 激活函数：

ReLu使得网络可以自行引入稀疏性，在没做预训练情况下，以ReLu为激活的网络性能优于其它激活函数。 
数学表达式： $y = \max(0, x)$

### Sigmoid 激活函数：

sigmoid 激活函数在神经网络学习方面，可以将重点特征推向中央区，将非重点特征推向两侧区。 
数学表达式： $y = (1 + \exp(-x))^{-1}$

### Tanh 激活函数：

Tanh 激活函数使得输出与输入的关系能保持非线性单调上升和下降关系，比sigmoid 函数延迟了饱和期，对神经网路的容错性好。 
数学表达式： $y = \frac{\exp(x) -\exp(-x) }{\exp(x) +\exp(-x)}$

# ReLU 主要函数

### Forward_cpu 函数：

```
template <typename Dtype>
void ReLULayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top) {
  const Dtype* bottom_data = bottom[0]->cpu_data();
  Dtype* top_data = top[0]->mutable_cpu_data();
  const int count = bottom[0]->count();
  Dtype negative_slope = this->layer_param_.relu_param().negative_slope(); //输入小于0时的斜率，默认为0；
  for (int i = 0; i < count; ++i) {
    top_data[i] = std::max(bottom_data[i], Dtype(0))
        + negative_slope * std::min(bottom_data[i], Dtype(0));
  }//输入大于零斜率为1，小于0斜率为negative_slope。
}123456789101112
```

### Backward_cpu 函数：

```
template <typename Dtype>
void ReLULayer<Dtype>::Backward_cpu(const vector<Blob<Dtype>*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob<Dtype>*>& bottom) {
  if (propagate_down[0]) {
    const Dtype* bottom_data = bottom[0]->cpu_data();
    const Dtype* top_diff = top[0]->cpu_diff();
    Dtype* bottom_diff = bottom[0]->mutable_cpu_diff();
    const int count = bottom[0]->count();
    Dtype negative_slope = this->layer_param_.relu_param().negative_slope();
    for (int i = 0; i < count; ++i) {
      bottom_diff[i] = top_diff[i] * ((bottom_data[i] > 0)
          + negative_slope * (bottom_data[i] <= 0));
    }
  }
}
1234567891011121314151617
```

# Sigmoid主要函数

### Forward_cpu 函数：

```
template <typename Dtype>
void SigmoidLayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top) {
  const Dtype* bottom_data = bottom[0]->cpu_data();
  Dtype* top_data = top[0]->mutable_cpu_data();
  const int count = bottom[0]->count();
  for (int i = 0; i < count; ++i) {
    top_data[i] = sigmoid(bottom_data[i]);
  }
}12345678910
```

sigmoid 函数定义如下：

```
template <typename Dtype>
inline Dtype sigmoid(Dtype x) {
  return 1. / (1. + exp(-x));
}1234
```

### Backward_cpu 函数：

求导： 
$$
\frac{dy}{dx} = \frac{-1}{\big(1+\exp(-x)\big)^2} \times\big (-\exp(-x)\big) =\frac{1}{1 + \exp(-x)}\times\big(1-\frac{1}{1 + \exp(-x)}\big)
$$

```
template <typename Dtype>
void SigmoidLayer<Dtype>::Backward_cpu(const vector<Blob<Dtype>*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob<Dtype>*>& bottom) {
  if (propagate_down[0]) {
    const Dtype* top_data = top[0]->cpu_data();
    const Dtype* top_diff = top[0]->cpu_diff();
    Dtype* bottom_diff = bottom[0]->mutable_cpu_diff();
    const int count = bottom[0]->count();
    for (int i = 0; i < count; ++i) {
      const Dtype sigmoid_x = top_data[i];
      bottom_diff[i] = top_diff[i] * sigmoid_x * (1. - sigmoid_x);
    }
  }
}123456789101112131415
```

# Tanh主要函数

### Forward_cpu 函数：

```
template <typename Dtype>
void TanHLayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top) {
  const Dtype* bottom_data = bottom[0]->cpu_data();
  Dtype* top_data = top[0]->mutable_cpu_data();
  const int count = bottom[0]->count();
  for (int i = 0; i < count; ++i) {
    top_data[i] = tanh(bottom_data[i]);
  }
}12345678910
```

### Backward_cpu 函数：

求导： 
$$
\frac{dy}{dx} = \frac{\big(\exp(x)+\exp(-x)\big)^2-\big(\exp(x)-\exp(-x)\big)^2}{\big(\exp(x)+\exp(-x)\big)^2} = 1-\Big(\frac{\exp(x) -\exp(-x) }{\exp(x) +\exp(-x)}\Big)^2
$$


```
template <typename Dtype>
void TanHLayer<Dtype>::Backward_cpu(const vector<Blob<Dtype>*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob<Dtype>*>& bottom) {
  if (propagate_down[0]) {
    const Dtype* top_data = top[0]->cpu_data();
    const Dtype* top_diff = top[0]->cpu_diff();
    Dtype* bottom_diff = bottom[0]->mutable_cpu_diff();
    const int count = bottom[0]->count();
    Dtype tanhx;
    for (int i = 0; i < count; ++i) {
      tanhx = top_data[i];
      bottom_diff[i] = top_diff[i] * (1 - tanhx * tanhx);
    }
  }
}
```