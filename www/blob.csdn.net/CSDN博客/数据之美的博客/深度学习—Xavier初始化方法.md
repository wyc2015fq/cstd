# 深度学习—Xavier初始化方法 - 数据之美的博客 - CSDN博客
2017年09月13日 16:05:54[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：395
目录[(?)](http://blog.csdn.net/shuzfan/article/details/51338178#)[[-]](http://blog.csdn.net/shuzfan/article/details/51338178#)
- [为了使得网络中信息更好的流动每一层输出的方差应该尽量相等](http://blog.csdn.net/shuzfan/article/details/51338178#t0)- [基于这个目标现在我们就去推导一下每一层的权重应该满足哪种条件](http://blog.csdn.net/shuzfan/article/details/51338178#t1)
- [综上为了保证前向传播和反向传播时每一层的方差一致应满足](http://blog.csdn.net/shuzfan/article/details/51338178#t2)
- [因此Xavier初始化的实现就是下面的均匀分布](http://blog.csdn.net/shuzfan/article/details/51338178#t3)
“Xavier”初始化方法是一种很有效的神经网络初始化方法，方法来源于2010年的一篇论文[《Understanding the difficulty of training deep feedforward neural
 networks》](http://machinelearning.wustl.edu/mlpapers/paper_files/AISTATS2010_GlorotB10.pdf)，可惜直到近两年，这个方法才逐渐得到更多人的应用和认可。
## 为了使得网络中信息更好的流动，每一层输出的方差应该尽量相等。
### 基于这个目标，现在我们就去推导一下：每一层的权重应该满足哪种条件。
文章先假设的是线性激活函数，而且满足0点处导数为1，即 
![这里写图片描述](https://img-blog.csdn.net/20160507175722190)
现在我们先来分析一层卷积： 
![这里写图片描述](https://img-blog.csdn.net/20160507180252629)
其中ni表示输入个数。
根据概率统计知识我们有下面的方差公式： 
![这里写图片描述](https://img-blog.csdn.net/20160507180808647)
特别的，当我们假设输入和权重都是0均值时（目前有了BN之后，这一点也较容易满足），上式可以简化为： 
![这里写图片描述](https://img-blog.csdn.net/20160507181258399)
进一步假设输入x和权重w独立同分布，则有： 
![这里写图片描述](https://img-blog.csdn.net/20160507181653603)
于是，为了保证输入与输出方差一致，则应该有： 
![这里写图片描述](https://img-blog.csdn.net/20160507181828965)
对于一个多层的网络，某一层的方差可以用累积的形式表达： 
![这里写图片描述](https://img-blog.csdn.net/20160507182047578)
特别的，反向传播计算梯度时同样具有类似的形式： 
![这里写图片描述](https://img-blog.csdn.net/20160507182201262)
### 综上，为了保证前向传播和反向传播时每一层的方差一致，应满足：
![这里写图片描述](https://img-blog.csdn.net/20160507182402488)
但是，实际当中输入与输出的个数往往不相等，于是为了均衡考量，最终我们的权重方差应满足：
——————————————————————————————————————— 
![这里写图片描述](https://img-blog.csdn.net/20160507182552982)
———————————————————————————————————————
学过概率统计的都知道 [a,b] 间的均匀分布的方差为： 
![这里写图片描述](https://img-blog.csdn.net/20160507182843806)
### 因此，Xavier初始化的实现就是下面的均匀分布：
—————————————————————————————————————————— 
![这里写图片描述](https://img-blog.csdn.net/20160507183053496)
———————————————————————————————————————————
下面，我们来看一下caffe中具体是怎样实现的，代码位于include/caffe/filler.hpp文件中。
```
template <typename Dtype>
class XavierFiller : public Filler<Dtype> {
 public:
  explicit XavierFiller(const FillerParameter& param)
      : Filler<Dtype>(param) {}
  virtual void Fill(Blob<Dtype>* blob) {
    CHECK(blob->count());
    int fan_in = blob->count() / blob->num();
    int fan_out = blob->count() / blob->channels();
    Dtype n = fan_in;  // default to fan_in
    if (this->filler_param_.variance_norm() ==
        FillerParameter_VarianceNorm_AVERAGE) {
      n = (fan_in + fan_out) / Dtype(2);
    } else if (this->filler_param_.variance_norm() ==
        FillerParameter_VarianceNorm_FAN_OUT) {
      n = fan_out;
    }
    Dtype scale = sqrt(Dtype(3) / n);
    caffe_rng_uniform<Dtype>(blob->count(), -scale, scale,
        blob->mutable_cpu_data());
    CHECK_EQ(this->filler_param_.sparse(), -1)
         << "Sparsity not supported by this Filler.";
  }
};
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
由上面可以看出，caffe的Xavier实现有三种选择
（1） 默认情况，方差只考虑输入个数： 
![这里写图片描述](https://img-blog.csdn.net/20160507183438673)
（2） FillerParameter_VarianceNorm_FAN_OUT，方差只考虑输出个数： 
![这里写图片描述](https://img-blog.csdn.net/20160507183630098)
（3） FillerParameter_VarianceNorm_AVERAGE，方差同时考虑输入和输出个数： 
![这里写图片描述](https://img-blog.csdn.net/20160507183816880)
之所以默认只考虑输入，我个人觉得是因为前向信息的传播更重要一些
