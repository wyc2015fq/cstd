# 主成分分析PCA - 浅梦的博客 - CSDN博客





2017年10月12日 14:42:04[浅梦s](https://me.csdn.net/u012151283)阅读数：334
个人分类：[数据降维																[机器学习](https://blog.csdn.net/u012151283/article/category/6653295)](https://blog.csdn.net/u012151283/article/category/7220542)








# 向量空间

![这里写图片描述](https://img-blog.csdn.net/20171012124203011?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 基

我们所说的向量其实是向量坐标和向量空间的基构成的线性组合。 

要准确的描述向量，首先需要确定向量空间的一组基，然后 

在通常的二维坐标系中，我们选中基为$(1,0)^T$和$(0,1)^T$，那么对于向量$(3,2)$,其实它是$3*(1,0)^T+2*(0,1)^T$。 
**通常基为列向量**，在进行坐标变换时，通常将基作为行向量，与原空间的坐标（列向量）做点乘。 

一般地，我们设定基向量为单位向量，因为这样可以直接用向量点乘基向量获取基上的坐标表示，否则需要除以基向量的模长。
## 基变换的矩阵表示

$  \begin{bmatrix}     1 & 0 \\     0 & 1 \end{bmatrix}*  \begin{bmatrix}     3  \\     2  \end{bmatrix}=  \begin{bmatrix}     3  \\     2  \end{bmatrix}$

上式的意义是，将右边矩阵每一列的列向量，变换到左边矩阵每一行的行向量为基所表示的空间中去。

# PCA

PCA是一种常用的降维方法。其思想是希望用一个低维超平面对正交属性空间中的样本点进行恰当的表达。 

这样的超平面具有以下性质
- 最近重构性：样本点到这个超平面的距离都足够近
- 最大可分性：样本点在这个超平面上的投影能尽可能分开

## 最近重构性

假定样本进行了中心化处理，即$\sum_ix_i =0$，假定投影变换后得到的新坐标系为$[w_1,w_2,...,w_d]$，其中$w_i$是标准正交基向量，$||w_i||_2=1$，$w_i^Tw_j=0(i\neq j)$

设新坐标系的维度为$d'<d$，则样本点在低维坐标系中的投影是$z_i$，其中$z_{ij}=w_j^Tx_i$是$x_i$在低维坐标系下第j维的坐标。若基于$z_i$来重构$x_i$，则会得到$\hat x_i=\sum_{j=1}^{d'}z_{ij}w_j$。

现在考虑整个数据集上原样本点$x_i$与投影筹够的样本点$\hat x_i$之间的距离 


$\begin{align}loss&=\sum\limits_{i=1}^{m}||\sum\limits_{j=1}^{d'}z_{ij}w_j-x_i||_2^2\\&=\sum\limits_{i=1}^{m}[(\sum\limits_{j=1}^{d'}z_{ij}w_j-x_i)^T(\sum\limits_{k=1}^{d'}z_{ij}w_j-x_i)]\\&=\sum\limits_{i=1}^{m}[(\sum\limits_{j=1}^{d'}z_{ij}w_j)^T(\sum\limits_{k=1}^{d'}z_{ij}w_j)-(\sum\limits_{j=1}^{d'}z_{ij}w_j)^Tx_i-x_i^T(\sum\limits_{k=1}^{d'}z_{ij}w_j)+x_i^Tx_i]\\&=\sum\limits_{i=1}^{m}[(\sum\limits_{j=1}^{d'}w_j^Tz_{ij}^T z_{ij}w_j)-2x_i^T(\sum\limits_{k=1}^{d'}z_{ij}w_j)+x_i^Tx_i]\\&=\sum\limits_{i=1}^{m}[(\sum\limits_{j=1}^{d'}w_j^Tx_ix_i^Tw_j)-2(\sum\limits_{k=1}^{d'}x_i^Tw_j^Tx_iw_j)+x_i^Tx_i]\\&=-\sum\limits_{j=1}^{d'}{(w_j^T(\sum\limits_{i=1}^{m}x_ix_i^T)w_j)}+\sum\limits_{i=1}^mx_i^Tx_i\\&=-tr(W^TXX^TW)+const\end{align}$

其中$\sum_ix_ix_i^T$是协方差矩阵，为$XX^T$

所以，优化目标为 


$\begin{align}\min_W&{-tr(W^TXX^TW)}\\s.t. &W^TW=I\end{align}$
## 最大可分性

样本点$x_i$在新空间中超平面上的投影是$W^Tx_i$，若所有投影能尽可能分开，则应该使投影后样本点的方差最大化，投影后样本点的方差是$\sum_iW^Tx_ix_i^TW$

优化目标可写为 


$\begin{align}\max_W&{tr(W^TXX^TW)}\\s.t. &W^TW=I\end{align}$
## 拉格朗日乘子法

$\min_W W^TXX^TW+\lambda(W^TW-I)$

解得 
$XX^TW=\lambda W$

于是，只需要对协方差矩阵$XX^T$进行特征值分解(**实践中通常对X进行奇异值分解来代替协方差矩阵的特征值分解**)，将求得的特征值排序；取前$d'$个特征值对应得特征向量构成$W=[w_1,w_2,...,w_{d'}]$就求出了投影矩阵。
## 伪代码

![这里写图片描述](https://img-blog.csdn.net/20171012151601929?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 评价

![这里写图片描述](https://img-blog.csdn.net/20171012151703087?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# Kernel PCA

> 
PCA也存在一些限制，例如它可以很好的解除线性相关，但是对于高阶相关性就没有办法了，对于存在高阶相关性的数据，可以考虑Kernel PCA，通过Kernel函数将非线性相关转为线性相关。另外，PCA假设数据各主特征是分布在正交方向上，如果在非正交方向上存在几个方差较大的方向，PCA的效果就大打折扣了。—转自知乎


参考资料

> 
《机器学习》第10章 
[向量的表示及协方差矩阵](http://blog.csdn.net/songzitea/article/details/18219237)
[PCA数学原理](https://zhuanlan.zhihu.com/p/21580949)









