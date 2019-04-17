# 黎曼流形（Riemannian manifold） - curryche的博客 - CSDN博客





2018年06月04日 20:34:42[curryche](https://me.csdn.net/whwan11)阅读数：3281








**1.黎曼流形**

    定义：黎曼流形（Riemannian manifold）是一个微分流形，其中每点p的切空间都定义了点积，而且其数值随p平滑地改变。它容许我们定义弧线长度、角度、面积、体积、曲率、函数梯度及向量域的散度。

    上述定义来自于维基百科（https://zh.wikipedia.org/wiki/%E9%BB%8E%E6%9B%BC%E6%B5%81%E5%BD%A2），对一个流形M,如果给这个流形赋予合适的黎曼测度d,那么这个流形M就成为一个黎曼流形，而黎曼测度就是定义中提到的流形上切空间的点积。

    最经典的黎曼流形有SPD流形、格列斯曼流形(Grassmann manifold)。

**2.黎曼测度**

    回想欧式空间，我们要计算欧式空间中点与点之间的距离时，直接利用向量的模运算就可以，但是在流形不是向量空间，因此向量运算不适用于流形，想要衡量流形上点与点之间的距离一般利用黎曼测度。

    一个黎曼测度通常表示为d(x1,x2), x1和x2都是流形上的点，通俗来说，在流形上我们利用测度来衡量点与点之间的距离，从而得到流形大致的几何结构。黎曼测度通常经过log映射把流形上的点映射到切空间上，再在切空间上通过定义的点积来计算距离。

    常用的黎曼测度有Log_Euclidean测度（[相关链接](https://blog.csdn.net/whwan11/article/details/80569644)），AIRM（Affine Invariant Riemannian Metric）测度（[相关链接](https://blog.csdn.net/whwan11/article/details/80598068)），Bregman散度[链接](https://blog.csdn.net/whwan11/article/details/80697574)。



