# 深度学习基础--池化&pooling - wydbyxr的博客 - CSDN博客
2018年11月15日 10:00:54[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：123
# 池化&pooling
  这也就意味着在一个图像区域有用的特征极有可能在另一个区域同样适用。因此，为了描述大的图像，一个很自然的想法就是对不同位置的特征进行聚合统计，例如，人们可以计算图像一个区域上的某个特定特征的平均值 (或最大值)。
  这些概要统计特征不仅具有低得多的维度 (相比使用所有提取得到的特征)，同时还会改善结果(不容易过拟合)。这种聚合的操作就叫做池化。
  一般地，每个卷积层l都会接一个下采样层l+1。而下采样层=pooling（池化），下采样中的一种方法就是池化（mean-pooling和max-pooling为例）。另外，上采样是扩大。
  pooling的结果是使得特征减少，参数减少，但pooling的目的并不仅在于此。pooling目的是为了保持某种不变性（旋转、平移、伸缩等），常用的有mean-pooling，max-pooling和Stochastic-pooling三种。
## 常见的pooling类型
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181115095902120.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
## Stochastic pooling（随机池化）
### 计算过程
  1）先将方格中的元素同时除以它们的和sum，得到概率矩阵；
  2）按照概率随机选中方格；
  3）pooling得到的值就是方格位置的值。
  使用stochastic pooling时(即test过程)，其推理过程也很简单，对矩阵区域求加权平均即可。
  在反向传播求导时，只需保留前向传播已经记录被选中节点的位置的值，其它值都为0,这和max-pooling的反向传播非常类似。
### 求值过程的例子
  0*0+1.1*0.11+2.5*0.25+0.9*0.09+2.0*0.2+1.0*0.1+0*0+1.5*0.15+1.0*0.1=1.625
  说明此时对小矩形pooling后的结果为1.625。
