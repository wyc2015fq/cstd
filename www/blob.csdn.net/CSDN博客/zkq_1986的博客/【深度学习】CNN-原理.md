# 【深度学习】CNN-原理 - zkq_1986的博客 - CSDN博客





2017年05月25日 11:15:03[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：397








CNN原理

1CNN步骤

1）卷积层

通过filter（模板）与input（输入）进行卷积，获得卷积层输出。

2）池化层

通过池化(pooling)，将卷积层的输出变成一个固定长度的向量。常用的池化为1-max，最大、最小、平均等。

3）利用固定长度的向量即可通过常规的分类、回归模型进行分类、回归。如softmax、logistic regression。



CNN的结构图如下。

![](https://img-blog.csdn.net/20170525111031164)




paper: [http://](http://arxiv.org/pdf/1510.03820v4.pdf)[arxiv.org/pdf/1510.03820v4.pdf](http://arxiv.org/pdf/1510.03820v4.pdf)

code: [https](https://github.com/dennybritz/cnn-text-classification-tf)[://](https://github.com/dennybritz/cnn-text-classification-tf)[github.com/dennybritz/cnn-text-classification-tf](https://github.com/dennybritz/cnn-text-classification-tf)




![](https://img-blog.csdn.net/20170525111131367)










2关于池化操作需要的几个参数

•      Pooling 的 窗口大小（长，宽） 

•      Pooling的步长 
![](https://img-blog.csdn.net/20170525111405124)












