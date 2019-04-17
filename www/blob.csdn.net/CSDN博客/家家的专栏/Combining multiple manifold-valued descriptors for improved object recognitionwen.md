# Combining multiple manifold-valued descriptors for improved object recognitionwen - 家家的专栏 - CSDN博客





2014年01月03日 20:00:34[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：745
个人分类：[乐在科研中																[图像处理算法](https://blog.csdn.net/yihaizhiyan/article/category/715109)](https://blog.csdn.net/yihaizhiyan/article/category/742190)








分析paper：Combining multiple manifold-valued descriptors for improved object recognition

From Mehrtash harandi




Classification using multiple manifold-valued features. 

**Riemannnian Geometry**

**Combining the HOG + Region Covariance on two different manifolds  **

通常把欧式空间的算法应用到黎曼流形，是通过切空间近似，来获得一个欧式表示。

但是切空间投影度量等，是在一个临近区域内，当数据分布非常远时，投影到切空间边失去了意义。

**Mehrtash一直主张，通过正定kernel投影，把流形空间嵌入到高维的Reproducing Kernel Hilbert Space.**




联合**HOG特征和区域协方差特征**，他们分别处于两种特定的流形上，**单位球 和 黎曼流形**。

**1、单位球 （The unit n-sphere）**

**定义**：The n-dimensional sphere that has unit radius and is centered at the origin of the n+1 dimensional Euclidean space, denoted by S^n.

**度量**：![](https://img-blog.csdn.net/20140103170138531)

     其中HOG特征通过L2规则化。

**2、黎曼流形 （SPD矩阵）**

**度量**：![](https://img-blog.csdn.net/20140103171734328)













