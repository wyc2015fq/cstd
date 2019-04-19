# Global average Pooling - 数据之美的博客 - CSDN博客
2017年09月11日 10:15:33[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：901
最近在看关于cifar10 的分类的识别的文章
在看all convolution network 中看到中用到一个global average pooling 
下面就介绍一下global average pooling 
这个概念出自于 network in network 
主要是用来解决全连接的问题，其主要是是将最后一层的特征图进行整张图的一个均值池化，形成一个特征点，将这些特征点组成最后的特征向量
进行softmax中进行计算。
举个例子
假如，最后的一层的数据是10个6*6的特征图，global average pooling是将每一张特征图计算所有像素点的均值，输出一个数据值，
这样10 个特征图就会输出10个数据点，将这些数据点组成一个1*10的向量的话，就成为一个特征向量，就可以送入到softmax的分类中计算了
![](https://img-blog.csdn.net/20160528001522947)
上图是从PPT中截取的对比全连接与全局均值池化的差异
原文中介绍这样做主要是进行全连接的替换，减少参数的数量，这样计算的话，global average pooling层是没有数据参数的
这也与network in network 有关，其文章中提出了一种非线性的 类似卷积核的mlpconv的感知器的方法，计算图像的分块的值
可以得到空间的效果，这样就取代了pooling的作用，但是会引入一些参数，但是为了平衡，作者提出了使用global average pooling
下面是network in network 中的摘取
![](https://img-blog.csdn.net/20160528002057996)
下图是是一个基于MLP的局部计算，最后使用global average pooling 的network in network 的结构图
![](https://img-blog.csdn.net/20160528002103157)
