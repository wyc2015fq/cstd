# 流形学习之Laplacian Eigenmaps - 家家的专栏 - CSDN博客





2011年11月08日 09:37:08[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：5126








 要说哪一个方法被做的全面，那莫非LE莫属。如果只说LE这个方法本身，是不新的，许多年前在做mesh相关的领域就开始这莫用。但是放在黎曼几何的框架内，给出完整的几何分析的，应该是Belkin和Niyogi（LE作者）的功劳。

LE的基本思想就是用一个无向有权图来描述一个流形，然后通过用图的嵌入（graph embedding）来找低维表示。说白了，就是保持图的局部邻接关系的情况把这个图从高维空间中重新画在一个低维空间中（graph drawing）。

在至今为止的流行学习的典型方法中，LE是速度最快、效果相对来说不怎莫样的。但是LE有一个其他方法没有的特点，就是如果出现outlier情况下，它的鲁棒性（robustness）特别好。

就算法而言，就是那三步：构建图、选择权、特征映射。

**关于流形学习的几篇文章~**

局部线性嵌入(LLE).S. T. Roweis and L. K. Saul. Nonlinear dimensionality reduction by locally linear embedding. Science, vol. 290, pp. 2323--2326, 2000.

等距映射(Isomap).J.B. Tenenbaum, V. de Silva, and J. C. Langford. A global geometric framework for nonlinear dimensionality reduction. Science, vol. 290, pp. 2319--2323, 2000.

拉普拉斯特征映射(Laplacian Eigenmap).M. Belkin, P. Niyogi, Laplacian Eigenmaps for Dimensionality Reduction and Data Representation. Neural Computation, Vol. 15, Issue 6, pp. 1373 –1396, 2003 . 



