# Toeplitz matrix 与 Circulant matrix - Machine Learning with Peppa - CSDN博客





2018年01月10日 19:21:18[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：206
所属专栏：[机器学习与数据挖掘](https://blog.csdn.net/column/details/18961.html)










之所以专门定义两个新的概念，在于它们特殊的形式，带来的特别的形式。

## 1. Toeplitz matrix
- 对角为常数；

 的矩阵  是
 Toepliz 矩阵当且仅当，对于  有：










.


 表示行号减去列号，对于  的
 Toeplize 矩阵共  个不同的值，即 。







## 2. Toeplize 矩阵与卷积和傅里叶变换到关系


长度为  的信号 ，与长度为  的卷积核 ，二者之间的卷积可通过矩阵乘法的方式计算：








同样地根据卷积的性质，也有：






- 

由左边的 Toeplize 矩阵可知，Toeplize 矩阵不必是方阵；下面来看该矩阵的维度信息，如下图所示：



![这里写图片描述](https://img-blog.csdn.net/20170515184220365?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGFuY2h1bmh1aQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




上面在 wikipedia 中复制过来的矩阵信息其实是当  时的情形，且 。

## 3. Circulant matrix


是一种特殊的 Toeplitz 矩阵。


如下为一个 Circulant matrix 的基本形式：








在 Toeplize 的基础上，Circulant 进一步的要求是每一个行向量，是前一个行向量的循环右移一个元素。




