# Combing multiple manifold-valued descriptors for improved object recognition - 家家的专栏 - CSDN博客





2013年11月11日 14:34:24[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：911








Paper: Combing multiple manifold-valued descriptors for improved object recognition (Mehrtasg Harandi)

A** learning met**hod for **classification **using** multiple manifold-valued features**

1、**Feature combination**： Combing ***HOG ***and
***Region Convariance***descriptors that reside on two
***different manifolds***.

2、A kernel on the*** n-dimensional unit sphere***




一些非向量空间的描述子有：

Normalized histogram vectors、Convariance descriptors （object recognition）

Diffusion tensors (biomedical image analysis)

3D rotation matrices (geometrical computer vision)

Linear subspaces of the n-dimensional Euclidean space (video vision)




分析这种非向量空间描述子的方法是利用黎曼几何~

把欧式算法推广到黎曼流形的方法：

一是，obtain a Euclidean represenration of the manifold-valued data by approximating the manifold by the tangent space.
**(log and exp maps)**

 二是，embed the manifold in the high dimensional** Reproducing Kernel Hilbert space (RKHS) using a positive definite kerne**l.




Combining multiple manifold-valued descriptor via RKHS embedding.



