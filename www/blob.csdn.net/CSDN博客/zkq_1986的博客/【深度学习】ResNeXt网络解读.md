# 【深度学习】ResNeXt网络解读 - zkq_1986的博客 - CSDN博客





2018年11月19日 13:05:37[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：95








1. 网络结构

ResNeXt与resnet基本一致，唯一改动的地方在于resnet三层卷积的layer。它将layer从原来的多个filter卷积核拆分为多个个数少的filter卷积核，这些个数少的filter卷积核结构相同，目的是为了简化网络设计。在原文中，拆分后的结构相同的block数量称为cardinality（基数）。ResNeXt的核心网络结构 如下图所示。

![](https://img-blog.csdnimg.cn/20181119124850387.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181119125840871.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2,size_16,color_FFFFFF,t_70)

2. 优缺点分析

优点：在不增加参数量情况下，提高了准确率。

![](https://img-blog.csdnimg.cn/20181119125029966.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2,size_16,color_FFFFFF,t_70)

缺点：待补充

3. 实验结果

![](https://img-blog.csdnimg.cn/20181119130323884.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181119130512866.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/2018111913000291.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2,size_16,color_FFFFFF,t_70)

4. 结论

    核心创新点就在于提出了 aggregrated transformations，用一种平行堆叠相同拓扑结构的blocks代替原来 ResNet 的三层卷积的block，在不明显增加参数量级的情况下提升了模型的准确率，同时由于拓扑结构相同，超参数也减少了，便于模型移植。



