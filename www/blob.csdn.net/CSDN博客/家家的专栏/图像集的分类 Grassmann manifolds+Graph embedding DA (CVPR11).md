# 图像集的分类~Grassmann manifolds+Graph embedding DA  (CVPR11) - 家家的专栏 - CSDN博客





2012年06月04日 21:54:33[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：2460








解读文献：Graph embedding discriminant analysis on Grassmannnian manifolds for improved image set matching



把图像集作为Grssmanian manidold上的点，来进行处理~

但是传统的DA (Discriminant analysis，判别式分析)没有考虑数据的局部结构，所以提出一种**Graph embedding**判别式分析的方法。

**类内和类间的相似性的图，使得类内紧凑、类间分离~**



**1、Grassmannian Analysis （不具体分析~）**

正交阵及其扩展，是Grassmannian manifold上的一个点~ 



**2、Graph embedding discriminant analysis**

LDA(线性判别式分析)是寻找一种线性的透影，使得类间不相似性最大，类内不相似性最小~ 

但是不能获取数据的局部结构~例如：多模态数据（一个类有几个分离的Cluster组成）或者奇异点~  处理所有的数据点，都是采用一种相同的方式~

所以基于图嵌入+LDA进行改进之~

图（**V, W**）V代表节点；W代表连接节点的边。W是一个对称矩阵。

图的对角阵D和Laplacian矩阵定义为：L=D-W;     ![](https://img-my.csdn.net/uploads/201206/04/1338796726_2095.png)



引入在向量空间中的图，嵌入图DA的目的是，最大化判别能力。把潜在的数据映射到另外一个向量空间，同事保存节点之间的相似性。

假设Grassmannian流形上的N个标记点，通过建立类内相似性图和雷剑相似性图，来建模流形的局部几何结构。

使得流形上的点，投影到一个新的流行上，使得类内点之间尽可能的靠近，类间点之间尽可能的远离~



