
# A Sensitivity Analysis of Convolutional Neural Networks for Sentence Classification - lpty的博客 - CSDN博客

2018年11月19日 23:52:09[lpty](https://me.csdn.net/sinat_33741547)阅读数：1314



# 引言
Ye Zhang在2016年挂在arXiv上的论文，从名字大概可以看出来，这是一篇CNN调参指南。
# 概述
模型方面用的是单层CNN，主要是CNN用做文本分类方面的研究，模型结构如下所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181119212054805.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
上述模型来自Convolutional Neural Networks for Sentence Classification，详细的介绍可以参考：[博文](https://blog.csdn.net/sinat_33741547/article/details/84106589)
[
](https://img-blog.csdnimg.cn/20181119212054805.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
# 内容
[
](https://img-blog.csdnimg.cn/20181119212054805.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
### Baseline Models
[
](https://img-blog.csdnimg.cn/20181119212054805.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)为了跟CNN模型进行对比，这里用SVM做了几个基线模型，如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181119215233392.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
其中，bowSVM是使用uni和bi-gram特征和liner kernel，
wvSVM使用句子词向量的平均值特征和RBF-kernel，
bowwvSVM使用上述n-gram特征的词向量平均值特征和RBF-kermel。
### Base Configuration
以下用作研究验证的模型，如果没有特别说明，都用这些参数作为默认设置，
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181119221444483.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
### Word Vectors
使用上述基础配置，更改词向量进行训练，这里使用了word2vec和glove这两种不同的分布式变量，均为300维，结果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111922222173.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
上述词向量模型中，对于OOV的词均使用随机初始化的方式赋值。从结果来看，简单将两种不同词向量拼接并没有起到效果，而两种不同的初始化方式并没有很明显的差距，建议对不同的任务尝试不同的初始化方式进行选择。
另外一个值得说的是，论文中尝试了使用one-hot变量的方式进行测试，结果表现很差，从数据集的角度分析，one-hot初始化的方式比较适合大文本，因为在短文本one-hot相对而言会形成稀疏矩阵，影响模型整体表现。
### Filter Size
##### Single filter
对于不同的filter，这里只谈论filter的高度h，即另一个维度与词向量维度一致，这里是100，对比不同的数据集如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181119223641996.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
上图以filter size为3时作为基准，只使用一个filter，其中横轴为filter size的大小，y轴为不同大小的filter相比size=3时的准确率百分比差值。
从中可以看到，对于每一个数据集都存在一个“最优的”filter大小，一般在1~10之间可以取到。
值得注意的是，对于文本长度比较长的数据集，如CR，最长的文本长度有105个字符，它的最优大小就在36~56之间，所以需要根据实际项目情况进行选区，对于长度越长的文本相对更大的filter表现会更好。
##### Multiple filter
这里指的是组合使用不同大小的filter，对于每一个找到单个最优大小filter的数据集，可以考虑用多个大小相邻的filter或者大小差距很大的filter进行对比，
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181119224812448.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
从上图中看到，使用多个大小相邻的filter表现比单个filter要好，而使用大小差距较大的filter变现反而变差。
[
](https://img-blog.csdnimg.cn/20181119224812448.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
### Filter Number
[
](https://img-blog.csdnimg.cn/20181119224812448.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)对于filter的数量这里也做了分析，取维度区间{10, 50, 100, 200, 400, 600, 1000, 2000}，如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181119231603483.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
以size=100为基准，其他大小相对基准的准确率百分比差为纵轴，由上图可以看到，相对比较合理的维度大小范围在100~600，但是需要注意的是，如果取得最优值的大小在边界处，就得考虑继续进行扩大范围搜索。
随着维度大小的增加，相应的计算耗时也越来越长，所以需要进行权衡考虑。
### Activation Function
激活函数这里测试了7种不同类型，
> ReLU (as per the baseline configuration), hyperbolic tangent (tanh), Sigmoid function (Maas et al., 2013), SoftPlus function (Dugas et al., 2001), Cube function (Chen and Manning, 2014), and tanh cube function (Pei et al., 2015)
还有一种比较有趣的是，不适用任何激活函数（Iden），最终结果表示 Iden, ReLU 和tanh是其中表现较好的三种，值得注意的是，iden在多层CNN中可能表现效果不会很好。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181119233031698.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
### Pooling Strategy
pooling这里使用了k-max pooling，其中k取值 {1, 5, 10, 15, 20}和average pooling，取值{3, 10, 20, 30}，结果显示1-max pooling表现相对较好。
### Regularization
正则化部分使用了Dropout和l2 norm，如下所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181119234433796.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181119234447883.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
从上述图片可以看到，dropout部分当取值在0.1~0.5的时候，对整个系统能有一定的提升，虽然这个提升并不是特别明显；而l2 norm则在整个过程并没有实质性的帮助，反而出现了效果下降的现象。
考虑单层CNN的模型复杂度不高所以整体模型并没有过拟合导致提升不明显，或者分布式的词向量已经达到了一定防止过拟合的效果。
所以在选择正则化部分是，根据模型的复杂度进行选取，选择在0.1~0.5范围内的dropout，而l2 norm则选择一个相应较大的值。
# 其他
1、更多详细的数据可以参考原论文
2、[A Sensitivity Analysis of Convolutional Neural Networks for Sentence Classification](https://arxiv.org/pdf/1510.03820.pdf)

[
  ](https://img-blog.csdnimg.cn/20181119234447883.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)