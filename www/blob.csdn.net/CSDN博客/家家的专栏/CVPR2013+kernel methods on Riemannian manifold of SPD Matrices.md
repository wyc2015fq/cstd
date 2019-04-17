# CVPR2013+kernel methods on Riemannian manifold of SPD Matrices - 家家的专栏 - CSDN博客





2013年06月18日 19:42:40[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1905








题目：Kernel methods on the Riemannian manifold of symmetric positive definite matrices

既然入选poster，想必有些亮点，下载细读之~



SPD （symetric positive definite）matrices，正定对称矩阵，经常用来编码图像信息。SPD矩阵的黎曼流形的几何结构，取得了一些成功。

现有的方法的缺陷：仅通过利用切平面，近似表示流形的局部结构。

本文，受到核方法的启发，提出把**SPD矩阵投影到高维Hilbert空间**。

为了在映射过程中，编码流形结构，本文在SPD矩阵黎曼流形空间中，引入**一系列的正定kernel。**

这些kernel函数，来自于高斯核，但是利用流形上的不同度量方法。

扩展欧式空间中的核方法(如SVM, 核PCA)，到SPD矩阵的黎曼流形空间中。

最后将此方法**应用**在在行人检测、对象分类、纹理分析、2D运动分割和图像分割中。



正定对称矩阵：区域协方差描述子、Diffusion tensors、Structure tensors.



SPD矩阵的黎曼流形结构。







