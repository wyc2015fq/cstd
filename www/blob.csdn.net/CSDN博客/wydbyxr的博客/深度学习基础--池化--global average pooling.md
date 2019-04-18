# 深度学习基础--池化--global average pooling - wydbyxr的博客 - CSDN博客
2018年11月15日 10:05:00[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：111
# global average pooling
  这个概念出自于 network in network。
  主要是用来解决全连接的问题（代替FC），其主要是是将最后一层的特征图进行整张图的一个均值池化，形成一个特征点，将这些特征点组成最后的特征向量进行softmax中进行计算。
  global average pooling代替FC层(GoogLenet )，但global average pooling的泛化性不好，只用在imagenet；我们需要在net的顶部添加一个线性层来激励。
## 例子
  假如，最后的一层的数据是10个6*6的特征图，global average pooling是将每一张特征图计算所有像素点的均值，输出一个数据值，这样10 个特征图就会输出10个数据点，将这些数据点组成一个1*10的向量的话，就成为一个特征向量，就可以送入到softmax的分类中计算了。
