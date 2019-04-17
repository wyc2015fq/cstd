# VLAD - Vector of Locally Aggregated Descriptors - 局部聚合描述子向量 - 原理简述 - curryche的博客 - CSDN博客





2018年06月14日 15:48:41[curryche](https://me.csdn.net/whwan11)阅读数：402








局部聚合描述子向量（Vector of Locally Aggregated Descriptors，VLAD）是一种编码方法，被广泛应用到计算机视觉的许多课题中，例如图像检索和场景识别。 

假设是$\{x_t\in R^{d}\}_{t=1}^{m}$输入的一幅图像的局部描述子向量的集合，$\{d_i\in R^{d}\}_{i=1}^{K}$是已经学习得到的码本，则该图像的VLAD编码为： 


$V=[v_1^T,v_2^T,...v_K^T]^T$
其中$v_i=\sum\limits_{x_t\in d_i}d_i-x_t$，$x_t\in d_i$表示$d_i$是距离$x_t$最近的码字。 

最终得到的VLAD编码是$Kd\times1$的向量。




