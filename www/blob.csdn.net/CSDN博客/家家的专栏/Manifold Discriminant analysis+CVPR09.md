# Manifold Discriminant analysis+CVPR09 - 家家的专栏 - CSDN博客





2013年05月30日 11:24:23[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1412
个人分类：[乐在科研中](https://blog.csdn.net/yihaizhiyan/article/category/742190)









**这篇paper主要说是？**

Manifold disscriminant analysis (MDA) 是一种判别式学习的方法（discriminative learning method）。

解决图像集分类问题。

把每一个图像集建模为一个流形，这样

图像集分类问题 === 分类导向的多流形学习的问题~

MDA 目标导向：最大化流形边缘（maximize manifold margin），学习一个嵌入的空间，使得

不同类别的更好地分离，（同一类别的）局部数据的紧致性提高。



经典的流形学习方法，如Isomap、LLE、Laplacian Eigenmap、Locality preserving projections (LPP)、Unsupervised discriminant projection (UDP)

 分类：Linear discriminant analysis (LDA)及其一些扩展方法（Nonparametric discriminant analysis NDA, subclass discriminant  analysis.....）



MDA关键问题：Local linear model, and discriminative learnning



