# caffe源码理解之inner_product_layer

2016年03月29日 09:50:28 [鱼蛋蛋哥](https://me.csdn.net/u010327085) 阅读数：10332



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/u010327085/article/details/51003215

# 介绍

inner_product_layer也即全连接层，如下示意图，每个输出都连接到所有的输入。 
![图一](https://img-blog.csdn.net/20160328090427177)

# 正文

## 成员变量

首先介绍一下几个成员变量

```
protected:
  int M_;//样本数量
  int K_;//单个输入特征长度
  int N_;//输出神经元数量
  bool bias_term_;//是否添加偏置，上图中的（+1）。
  Blob<Dtype> bias_multiplier_;//偏置的乘子123456
```

## 成员函数

其中的构造等成员函数基本上继承父类的，不详述，主要介绍LayerSetUp,Forward_cpu,Backward_cpu,Reshape这四个成员函数。

### Reshape

```
template <typename Dtype>
void InnerProductLayer<Dtype>::Reshape(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  // Figure out the dimensions
  const int axis = bottom[0]->CanonicalAxisIndex(
      this->layer_param_.inner_product_param().axis());
  const int new_K = bottom[0]->count(axis);
  /**这里解释一下，blob的CanonicalAxisIndex是为了标准化维度索引的输入，将一些非法维度输入转化为合法输入。
  *blob的count(int)是统计从某个维度开始，到结尾的总个数。这里第一个维度表示的是样本个数，也即是M_,与全连接层是独立的，其后面的是表示输入特征的个数。
  */
  CHECK_EQ(K_, new_K)
      << "Input size incompatible with inner product parameters.";
  // The first "axis" dimensions are independent inner products; the total
  // number of these is M_, the product over these dimensions.
  M_ = bottom[0]->count(0, axis);
  // The top shape will be the bottom shape with the flattened axes dropped,
  // and replaced by a single axis with dimension num_output (N_).
  vector<int> top_shape = bottom[0]->shape();
  top_shape.resize(axis + 1);
  top_shape[axis] = N_;
  top[0]->Reshape(top_shape);
  /**根据输入对输出进行reshape。
    *输出的shape是根据样本数和输出神经元个数确定的*/
  // Set up the bias multiplier
  if (bias_term_) {
    vector<int> bias_shape(1, M_);
    bias_multiplier_.Reshape(bias_shape);
    caffe_set(M_, Dtype(1), bias_multiplier_.mutable_cpu_data());
  }
  /**caffe_set(const int N, const Dtype alpha, Dtype* Y) 是用alpha的值来填充重Y开始的N个单元。*/
}12345678910111213141516171819202122232425262728293031
```

### LayerSetUp

```
template <typename Dtype>
void InnerProductLayer<Dtype>::LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  const int num_output = this->layer_param_.inner_product_param().num_output();
  bias_term_ = this->layer_param_.inner_product_param().bias_term();
  N_ = num_output;
  const int axis = bottom[0]->CanonicalAxisIndex(
      this->layer_param_.inner_product_param().axis());
  K_ = bottom[0]->count(axis);
  /*初始化N_和K_,类似reshape()中的解释*/
  if (this->blobs_.size() > 0) {
    LOG(INFO) << "Skipping parameter initialization";
  } else {
    if (bias_term_) {
      this->blobs_.resize(2);
    } else {
      this->blobs_.resize(1);
    }
    /** 初始化权重W和偏置b，caffe中包含多种初始化方式，在Filler中实现将开篇在介绍。
    *在blobs_中，blobs_[0]存放的为W，blobs_[1]存放的是偏置b。*/
    vector<int> weight_shape(2);
    weight_shape[0] = N_;
    weight_shape[1] = K_;
    this->blobs_[0].reset(new Blob<Dtype>(weight_shape));
    // fill the weights
    shared_ptr<Filler<Dtype> > weight_filler(GetFiller<Dtype>(
        this->layer_param_.inner_product_param().weight_filler()));
    weight_filler->Fill(this->blobs_[0].get());
    // If necessary, intiialize and fill the bias term
    if (bias_term_) {
      vector<int> bias_shape(1, N_);
      this->blobs_[1].reset(new Blob<Dtype>(bias_shape));
      shared_ptr<Filler<Dtype> > bias_filler(GetFiller<Dtype>(
          this->layer_param_.inner_product_param().bias_filler()));
      bias_filler->Fill(this->blobs_[1].get());
    }
  }  // parameter initialization
  this->param_propagate_down_.resize(this->blobs_.size(), true);
}123456789101112131415161718192021222324252627282930313233343536373839
```

### Forward_cpu

```
template <typename Dtype>
void InnerProductLayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top) {
  const Dtype* bottom_data = bottom[0]->cpu_data();
  Dtype* top_data = top[0]->mutable_cpu_data();
  const Dtype* weight = this->blobs_[0]->cpu_data();
  caffe_cpu_gemm<Dtype>(CblasNoTrans, CblasTrans, M_, N_, K_, (Dtype)1.,
      bottom_data, weight, (Dtype)0., top_data);
  if (bias_term_) {
    caffe_cpu_gemm<Dtype>(CblasNoTrans, CblasNoTrans, M_, N_, 1, (Dtype)1.,
        bias_multiplier_.cpu_data(),
        this->blobs_[1]->cpu_data(), (Dtype)1., top_data);
  }
}1234567891011121314
```

先介绍一下主要使用的到math_functions.hpp中的函数 caffe_cpu_gemm，其声明为

```
template <typename Dtype>
void caffe_cpu_gemm(const CBLAS_TRANSPOSE TransA,
    const CBLAS_TRANSPOSE TransB, const int M, const int N, const int K,
    const Dtype alpha, const Dtype* A, const Dtype* B, const Dtype beta,
    Dtype* C);12345
```

它的功能其实很直观，即C←αA×B+βC，TransA和TransB表明是否对A或B进行转置操作。A为M*K维矩阵，B为K*N维矩阵，C为M*N维矩阵。 
在Forward_cpu中，先进行 y←wx，或者说是y←xw’，然后是y←y+b，总结起来是y=wx+b。

### Backward_cpu

反向传播主要是为了更新W和b,其中的关键就是计算偏导，因此在这个函数中主要就是做了这三件事。首先介绍三个公式。 
![图二](https://img-blog.csdn.net/20160329093742215) 
![图三](https://img-blog.csdn.net/20160329093751731) 
前面两个是根据残差更新W和b，后面一个是计算梯度的。在caffe的全连接层中没有激活函数，因此后面一个公式中的导数项为1。

```
template <typename Dtype>
void InnerProductLayer<Dtype>::Backward_cpu(const vector<Blob<Dtype>*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob<Dtype>*>& bottom) {
  if (this->param_propagate_down_[0]) {
    const Dtype* top_diff = top[0]->cpu_diff();
    const Dtype* bottom_data = bottom[0]->cpu_data();
    // Gradient with respect to weight
    caffe_cpu_gemm<Dtype>(CblasTrans, CblasNoTrans, N_, K_, M_, (Dtype)1.,
        top_diff, bottom_data, (Dtype)1., this->blobs_[0]->mutable_cpu_diff());
  }
/*这里为更新W，top_diff表示的是残差\delte,bottom_data表示的是上一层的激活值a*/
  if (bias_term_ && this->param_propagate_down_[1]) {
    const Dtype* top_diff = top[0]->cpu_diff();
    // Gradient with respect to bias
    caffe_cpu_gemv<Dtype>(CblasTrans, M_, N_, (Dtype)1., top_diff,
        bias_multiplier_.cpu_data(), (Dtype)1.,
        this->blobs_[1]->mutable_cpu_diff());
/*更新b，caffe_cpu_gemv类似于caffe_cpu_gemm,只是前者用于矩阵向量相乘*/
  }
  if (propagate_down[0]) {
    const Dtype* top_diff = top[0]->cpu_diff();
    // Gradient with respect to bottom data
    caffe_cpu_gemm<Dtype>(CblasNoTrans, CblasNoTrans, M_, K_, N_, (Dtype)1.,
        top_diff, this->blobs_[0]->cpu_data(), (Dtype)0.,
        bottom[0]->mutable_cpu_diff());
  }
/*计算梯度，参见公式即可明白各变量含义*/
}1234567891011121314151617181920212223242526272829
```

# 总结

主要在看Backward_cpu，以前学习bp的时候是带激活函数的，在caffe中，为了更方便组合，其全连接层与激活函数是分开的，就这里有一点点不一样。