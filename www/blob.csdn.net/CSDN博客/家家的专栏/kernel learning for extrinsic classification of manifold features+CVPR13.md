# kernel learning for extrinsic classification of manifold features+CVPR13 - 家家的专栏 - CSDN博客





2013年10月07日 21:23:26[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：995
个人分类：[乐在科研中](https://blog.csdn.net/yihaizhiyan/article/category/742190)









**Kernel selection**

**Feature **lie on **Riemannian manifold** with known geometry.

现在很多学习算法（如：判别式分析、partial least squares、和SVM)不直接应用这些特征，因为这些特征处在非欧式空间中。

所以就面对这kernel选择的问题~

这篇paper就是通过kernel learning的方法来选择合适的kernel~

利用最佳化的问题，来综合考虑kernel学习和分类器。

例如，SVM分类器，用凸优化的方法来学习kernel-classifier。




**实验：图像集和动作识别**



现利用流行特征分类的普遍做法是：首先把流形映射到欧式空间，然后再新的空间中学习分类器。但是对应的切空间只是流形的局部结构，所以只是局部最优化~

这篇paper是，把流形映射到一个新的kernel Kilbert 空间中。

![](https://img-blog.csdn.net/20131007191423609)





黎曼流行上的特征，为了学习一个好的kernel-classifier，有以下两个标准：

1）风险函数 （最小化）

2）mapping应该保留潜在的流行结构~

![](https://img-blog.csdn.net/20131007191830546)


















