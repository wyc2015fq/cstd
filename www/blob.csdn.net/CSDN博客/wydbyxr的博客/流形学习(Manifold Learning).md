# 流形学习(Manifold Learning) - wydbyxr的博客 - CSDN博客
2018年10月20日 11:45:42[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：74
所属专栏：[经典机器学习算法](https://blog.csdn.net/column/details/28812.html)
# 流形学习方法(Manifold Learning)
  简称流形学习 (manifold learning) 。
  流形学习被认为属于非线性降维的一个分支，现在正大行其道，并且与数据可视化有着密不可分的关系。
  自2000年在著名的科学杂志《Science》被首次提出以来，已成为信息科学领域的研究热点。
## 基本概念
  流形学习涉及微分流行和黎曼几何等数学知识。
  假设数据是均匀采样于一个高维欧氏空间中的低维流形，流形学习就是从高维采样数据中恢复低维流形结构，即找到高维空间中的低维流形，并求出相应的嵌入映射，以实现维数约简或者数据可视化。
  如何理解“嵌入在高维空间中的低维流形？高维的数据是难以想象的，所以最直观的例子通常都会是**嵌入在三维空间中的二维或者一维流行**。
## 例子
### 1）例子1
  比如说一块布，可以把它看成一个二维平面，这是一个二维的欧氏空间，现在我们（在三维）中把它扭一扭，它就变成了一个流形（当然，不扭的时候，它也是一个流形，欧氏空间是流形的一种特殊情况）。
### 2）例子2
  简单来说，地球表面就是一个典型的流形，在流形上计算距离与欧式空间有所区别。例如，计算南极与北极点之间的距离不是从地心穿一个洞计算直线距离，而是沿着地球表面寻找一条最短路径，这样的一条路径称为测地线。如下面所示的三幅图	
![在这里插入图片描述](https://img-blog.csdn.net/20181020114206912?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
  其中第一张图为原始数据点分布，红色虚线是欧式距离，蓝色实线是沿着流形的真实测地线距离。
  第二张图是在原始数据点的基础上基于欧式距离构造的kNN图（灰色线条，下面还会具体介绍kNN图），红色实线表示kNN图中两点之间的最短路径距离。
  第三张图是将流形展开后的效果，可以看到，kNN图中的最短路径距离（红色实线）要略长于真实测地线距离（蓝色实线）。
  在实际应用中，真实测地距离较难获得，一般可以通过构造kNN图，在kNN图中寻找最短路径距离作为真实测地线距离的近似。
## 常用的Manifold Embedding算法
- Isomap
- Locally Linear Embedding
- Laplacian Eigenmaps
- Hessian Eigenmaps
- Local Tangent Space Alignment
- Semidefinite Embedding (Maximum Variance Unfolding)
- t-SNE表现最优，但它的计算复杂度也是最高的
## 应用
  在real world 的数据中，究竟哪些是分别在流形上的呢？这个却是很难说。
  不过，除了典型的 face 和 hand written digit 之外，大家也有把基于流形的算法直接用在诸如 text 看起来好像也流形没有什么关系的数据上，效果似乎也还不错。
## 总结
  所谓 Machine Learning 里的 Learning ，就是在建立一个模型之后，通过给定数据来求解模型参数。而 Manifold Learning 就是在模型里包含了对数据的流形假设。
## 不同流行学习算法和可视化算法的比较
[http://scikit-learn.org/stable/auto_examples/manifold/plot_lle_digits.html](http://scikit-learn.org/stable/auto_examples/manifold/plot_lle_digits.html)
