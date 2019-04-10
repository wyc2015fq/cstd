# Caffe源码（十）：eltwise_layer 分析

2015年08月18日 14:08:58 [seven_first](https://me.csdn.net/seven_first) 阅读数：17491



# 目录



- [目录](https://blog.csdn.net/seven_first/article/details/47751149#%E7%9B%AE%E5%BD%95)
- [简单介绍](https://blog.csdn.net/seven_first/article/details/47751149#%E7%AE%80%E5%8D%95%E4%BB%8B%E7%BB%8D)
- 主要函数
  - - [LayerSetUp 函数](https://blog.csdn.net/seven_first/article/details/47751149#1layersetup-%E5%87%BD%E6%95%B0)
    - [Reshape 函数](https://blog.csdn.net/seven_first/article/details/47751149#2reshape-%E5%87%BD%E6%95%B0)
    - [Forward_cpu 函数](https://blog.csdn.net/seven_first/article/details/47751149#3forwardcpu-%E5%87%BD%E6%95%B0)
    - [Backward_cpu 函数](https://blog.csdn.net/seven_first/article/details/47751149#4backwardcpu-%E5%87%BD%E6%95%B0)



# 简单介绍

eltwise_layer 实现多个blobs element-wise 的相加，相乘或者取最大值。

# 主要函数

### 1.LayerSetUp 函数：

```
template <typename Dtype>
void EltwiseLayer<Dtype>::LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  CHECK(this->layer_param().eltwise_param().coeff_size() == 0
      || this->layer_param().eltwise_param().coeff_size() == bottom.size()) <<
      "Eltwise Layer takes one coefficient per bottom blob.";
   // 如果存在系数，则每个blob 对应一个系数
  CHECK(!(this->layer_param().eltwise_param().operation()
      == EltwiseParameter_EltwiseOp_PROD
      && this->layer_param().eltwise_param().coeff_size())) <<
      "Eltwise layer only takes coefficients for summation.";
  op_ = this->layer_param_.eltwise_param().operation();//默认为SUM
  // Blob-wise coefficients for the elementwise operation.
  coeffs_ = vector<Dtype>(bottom.size(), 1);//将系数初始化为1
  if (this->layer_param().eltwise_param().coeff_size()) {
    for (int i = 0; i < bottom.size(); ++i) {
      coeffs_[i] = this->layer_param().eltwise_param().coeff(i);
    }//coeffs_ 存放从protobuf 传入的每个系数的值
  }
  stable_prod_grad_ = this->layer_param_.eltwise_param().stable_prod_grad();//默认true
}123456789101112131415161718192021
```

### 2.Reshape 函数：

```
template <typename Dtype>
void EltwiseLayer<Dtype>::Reshape(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  for (int i = 1; i < bottom.size(); ++i) {
    CHECK(bottom[i]->shape() == bottom[0]->shape());
  }//输入的每个bottom要有相同的shape
  top[0]->ReshapeLike(*bottom[0]);  // 输入和输出的shape相同
  // If max operation, we will initialize the vector index part.
  if (this->layer_param_.eltwise_param().operation() ==
      EltwiseParameter_EltwiseOp_MAX && top.size() == 1) {
    max_idx_.Reshape(bottom[0]->shape());//存放取最大值时的index
  }
}12345678910111213
```

### 3.Forward_cpu 函数：

```
template <typename Dtype>
void EltwiseLayer<Dtype>::Forward_cpu(
    const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
  int* mask = NULL;
  const Dtype* bottom_data_a = NULL;
  const Dtype* bottom_data_b = NULL;
  const int count = top[0]->count();
  Dtype* top_data = top[0]->mutable_cpu_data();
  switch (op_) {
  case EltwiseParameter_EltwiseOp_PROD:    
    caffe_mul(count, bottom[0]->cpu_data(), bottom[1]->cpu_data(), top_data);
    //输出top为输入的两个bottom的element-wise乘积 
    for (int i = 2; i < bottom.size(); ++i) {
      caffe_mul(count, top_data, bottom[i]->cpu_data(), top_data);
    } //将所有输入的bottom做element-wise乘积
    break;
  case EltwiseParameter_EltwiseOp_SUM:
    caffe_set(count, Dtype(0), top_data);
    // TODO(shelhamer) does BLAS optimize to sum for coeff = 1?
    for (int i = 0; i < bottom.size(); ++i) {
      caffe_axpy(count, coeffs_[i], bottom[i]->cpu_data(), top_data);
    }// 输出top 为 所有输入bottom乘以对应系数之和
    break;
  case EltwiseParameter_EltwiseOp_MAX:
    // Initialize
    mask = max_idx_.mutable_cpu_data();
    caffe_set(count, -1, mask);//初始化mask所有elements 为-1
    caffe_set(count, Dtype(-FLT_MAX), top_data);
    // 初始化top所有element 为负无穷
    // bottom 0 & 1
    bottom_data_a = bottom[0]->cpu_data();
    bottom_data_b = bottom[1]->cpu_data();
    for (int idx = 0; idx < count; ++idx) {
      if (bottom_data_a[idx] > bottom_data_b[idx]) {
        top_data[idx] = bottom_data_a[idx];  // maxval
        mask[idx] = 0;  // maxid 
      } else {
        top_data[idx] = bottom_data_b[idx];  // maxval
        mask[idx] = 1;  // maxid
      }
    }//bottom 0 和bottom 1 做比较
    // bottom 2++
    for (int blob_idx = 2; blob_idx < bottom.size(); ++blob_idx) {
      bottom_data_b = bottom[blob_idx]->cpu_data();
      for (int idx = 0; idx < count; ++idx) {
        if (bottom_data_b[idx] > top_data[idx]) {
          top_data[idx] = bottom_data_b[idx];  // maxval
          mask[idx] = blob_idx;  // maxid 更新为当前最大值的bottom id
        }//
      }
    }
    break;
  default:
    LOG(FATAL) << "Unknown elementwise operation.";
  }
}1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556
```

### 4.Backward_cpu 函数：

```
template <typename Dtype>
void EltwiseLayer<Dtype>::Backward_cpu(const vector<Blob<Dtype>*>& top,
    const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom) {
  const int* mask = NULL;
  const int count = top[0]->count();
  const Dtype* top_data = top[0]->cpu_data();
  const Dtype* top_diff = top[0]->cpu_diff();
  for (int i = 0; i < bottom.size(); ++i) {
    if (propagate_down[i]) {
      const Dtype* bottom_data = bottom[i]->cpu_data();
      Dtype* bottom_diff = bottom[i]->mutable_cpu_diff();
      switch (op_) {
      case EltwiseParameter_EltwiseOp_PROD:
        if (stable_prod_grad_) {
          bool initialized = false;
          for (int j = 0; j < bottom.size(); ++j) {
            if (i == j) { continue; }
            if (!initialized) {
              caffe_copy(count, bottom[j]->cpu_data(), bottom_diff);
              initialized = true;
            } else {
              caffe_mul(count, bottom[j]->cpu_data(), bottom_diff,
                        bottom_diff);
            }//对于bottom[i]导数为除了自己外输入bottom的element-wise 乘积
          }
        } else {5
          caffe_div(count, top_data, bottom_data, bottom_diff);
        }
        caffe_mul(count, bottom_diff, top_diff, bottom_diff);
        //乘以上一层传下来的导数top_diff
        break;
      case EltwiseParameter_EltwiseOp_SUM:
        if (coeffs_[i] == Dtype(1)) {
          caffe_copy(count, top_diff, bottom_diff);
        } else {
          caffe_cpu_scale(count, coeffs_[i], top_diff, bottom_diff);
        }
        break;
      case EltwiseParameter_EltwiseOp_MAX:
        mask = max_idx_.cpu_data();
        for (int index = 0; index < count; ++index) {
          Dtype gradient = 0;
          if (mask[index] == i) {
            gradient += top_diff[index];
          }
          bottom_diff[index] = gradient;
        }//当该bottom的index值取为最大时，继承top_diff在该index的值，其他位置为0；
        break;
      default:
        LOG(FATAL) << "Unknown elementwise operation.";
      }
    }
  }
}
```