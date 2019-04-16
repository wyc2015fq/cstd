# 特征选择之最小冗余最大相关性(mRMR) - littlely_ll的博客 - CSDN博客





2017年05月12日 17:37:04[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：13679
所属专栏：[特征提取与选择](https://blog.csdn.net/column/details/15615.html)









最小冗余最大相关性(mRMR)是一种滤波式的特征选择方法，由Peng et.al提出。 

用途：图像识别，机器学习等 

一种常用的特征选择方法是最大化特征与分类变量之间的相关度，就是选择与分类变量拥有最高相关度的前k个变量。但是，在特征选择中，单个好的特征的组合并不能增加分类器的性能，因为有可能特征之间是高度相关的，这就导致了特征变量的冗余。这就是Peng et.al说的“the m best features are not the best m features”。因此最终有了mRMR， 

即最大化特征与分类变量之间的相关性，而最小化特征与特征之间的相关性。这就是mRMR的核心思想。
## 互信息

定义：给定两个随机变量x和y，他们的概率密度函数（对应于连续变量）为$p(x),p(y),p(x,y)$，则互信息为 


$I(x;y)=\int\int p(x,y)log\frac{p(x,y)}{p(x)p(y)}dxdy$

## mRMR算法

我们的目标就是找出含有$m\{x_i\}$个特征的特征子集$S$
**离散变量**

最大相关性: 


$maxD(S,c), D=\frac{1}{|S|}\Sigma_{x_i\in S}I(x_i;c)$
$x_i为第i个特征，c为类别变量，S为特征子集$

最小冗余度： 


$min R(S), R=\frac{1}{|S|^2}\Sigma_{x_i,x_j\in S}I(x_i;x_j)$
**连续变量**

最大相关性: 


$max D_F, D_F=\frac{1}{|S|}\Sigma_{x_i\in S}F(x_i;c)$
$F(x_i,c)为F统计量$

最小冗余度： 


$min R_c, R=\frac{1}{|S|^2}\Sigma_{x_i,x_j\in S}c(x_i;x_j)$
$c(x_i,x_j)为相关函数$

当然，对于这些目标函数，还可以换做其他的函数，像信息增益，基尼指数等。 

然后整合最大相关性和最小冗余度： 

加法整合： 


$max \Phi(D,R), \Phi=D-R$

乘法整合： 


$max  \Phi(D,R), \Phi=D/R $

在实践中，用增量搜索方法寻找近似最优的特征。假设我们已有特征集$S_{m-1}$，我们的任务就是从剩下的特征$X-S_{m-1}$中找到第m个特征，通过选择特征使得$\Phi(.)$最大。增量算法优化下面的条件： 


$max_{x_j\in X-S_{m-1}}[I(x_j;c)-\frac{1}{m-1}\Sigma_{x_i\in S_{m-1}}I(x_j;x_i)]$

其算法的复杂度为$O(|S|\cdot M)$
## 算法优点

> - 速度快
- 估计结果更鲁棒
- 是$I(.)$的一阶最优估计


**参考**

【Hanchuan Peng et.al】Feature Selection Based on Mutual Information: Criteria of Max-Dependency, Max-Relevance, and Min-Redundancy 

【Barry O’Sullivan, Cork】Feature Selection for High-Dimensional Data





