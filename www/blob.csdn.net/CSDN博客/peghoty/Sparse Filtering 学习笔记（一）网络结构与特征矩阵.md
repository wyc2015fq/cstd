
# Sparse Filtering 学习笔记（一）网络结构与特征矩阵 - peghoty - CSDN博客


2014年03月25日 17:18:32[皮果提](https://me.csdn.net/peghoty)阅读数：4051



Sparse Filtering 是一个用于提取特征的无监督学习算法，与通常特征学习算法试图建模训练数据的分布的做法不同，Sparse Filtering 直接对训练数据的特征分布进行分析，在所谓“好特征”的指导下构建目标函数来进行优化，其中只涉及一个可调参数。本文将主要讨论两个问题：（1）什么样的特征是好的特征；（2）如何利用好特征的条件来构造 Sparse Filtering 的目标函数。

**目录链接**

（一）网络结构与特征矩阵
（二）好特征的刻画
（三）目标函数的建立和求解

![](https://img-blog.csdn.net/20140325165309109)![](https://img-blog.csdn.net/20140421101122062)![](https://img-blog.csdn.net/20140325162639484)

**参考文献**

1.[Ngiam, J., Koh, P. W., Chen, Z., Bhaskar, S. A., Ng, A. Y. (2011). Sparse Filtering.
 In NIPS (Vol. 11, pp. 1125-1133).](http://ai.stanford.edu/~ang/papers/nips11-SparseFiltering.pdf)
2.牛顿法与拟牛顿法学习笔记（五）[L-BFGS 算法](http://blog.csdn.net/itplus/article/details/21897715)
3.[Deep learning made easy](http://fastml.com/deep-learning-made-easy/)
4.[Deep
 Learning论文笔记之（二）Sparse Filtering稀疏滤波](http://blog.csdn.net/zouxy09/article/details/9982859)

作者: peghoty
出处:[http://blog.csdn.net/itplus/article/details/22071035](http://blog.csdn.net/itplus/article/details/22071035)
欢迎转载/分享, 但请务必声明文章出处.

