# 当我们在谈论K-means：其他聚类算法

[![余文毅](https://pic3.zhimg.com/a3e2422ec_xs.jpg)](https://www.zhihu.com/people/wyyualan)

[余文毅](https://www.zhihu.com/people/wyyualan)

不自由职业者

2 人赞同了该文章

本系列意在长期连载分享，内容上可能也会有所增删改减； 

**因此如果转载，请务必保留源地址，非常感谢！**

**知乎专栏：当我们在谈论数据挖掘**

**博客园：当我们在谈论数据挖掘（暂时公式显示有问题）**

## Fuzzy C-Means (FCM)

Fuzzy C-Means 是一种模糊聚类算法。K-means中每一个元素只能属于一个类别，而Fuzzy C-Means中一个元素以不同的概率属于每一个类别。

Fuzzy C-Means最早出自"**J. C. Dunn. "A Fuzzy Relative of the ISODATA Process and Its Use in Detecting Compact Well-Separated Clusters." 1973**"，而后在"**Bezdek, James C. Pattern recognition with fuzzy objective function algorithms, 1981**"书中被改进。由于后者是一本书，我就没有去翻看原著了。以下给出的是现在常用的版本

- \1. 类似于K-means，FCM的损失函数如下 



![img](https://pic4.zhimg.com/80/v2-55c4549df235b04a4c490a32f994b513_hd.png)

其中，



满足以下限制



![img](https://pic4.zhimg.com/80/v2-23e831b4c8e88a25c8658decad72abf7_hd.png)

- \2. 求取FCM损失函数极小值，依然是迭代进行，步骤如下。其中参数更新的公式，可以通过对1中公式用拉格朗日法求得。

![img](https://pic4.zhimg.com/80/v2-63310a6a1c6f67275b0f329417b8a76f_hd.png)

## Hierarchical Clustering

Hierarchical Clustering即层次聚类。它有两种类型：

1. 自下而上：即最初将每个样本点看做一个类别，然后将最相似的两个样本聚合，迭代这个步骤，直到只有一个类别
2. 自上而下：即最初将所有样本看做一个类别，然后依据一定规则进行分裂，直到每个样本单独一个类别；这个方法在实际中应用很少

Hierarchical Clustering最早出自"**Johnson, Stephen C. "Hierarchical clustering schemes." 1967**"，步骤如下：

1. Start by assigning each item to a cluster, so that if you have N items, you now have N clusters, each containing just one item. Let the distances (similarities) between the clusters the same as the distances (similarities) between the items they contain.
2. Find the closest (most similar) pair of clusters and merge them into a single cluster, so that now you have one cluster less.
3. Compute distances (similarities) between the new cluster and each of the old clusters.
4. Repeat steps 2 and 3 until all items are clustered into a single cluster of size N.

其中在步骤3中计算距离，有几种不同的计算方式：

- single-linkage: 当计算两个cluster之间的距离时，计算这两个cluster中距离最短的两个点的距离
- complete-linkage: 当计算两个cluster之间的距离时，计算这两个cluster中距离最长的两个点的距离
- average-linkage: 当计算两个cluster之间的距离时，计算这两个cluster中两两样本点距离的均值
- median-linkage: 当计算两个cluster之间的距离时，计算这两个cluster中两两样本点距离的中值

## DBSCAN

DBSCAN即Density-Based Spatial Clustering of Applications with Noise，是一种基于密度的聚类算法，最早出自"**Ester, Martin, et al. "A Density-Based Algorithm for Discovering Clusters in Large Spatial Databases with Noise." 1996**"。作者认为聚类算法应该尽量满足以下三点要求：

- 需要的参数尽量的少。因为在实际分析场景中，一般对数据的先验信息所知甚少
- 能有效针对不同形状的数据集。如K-means只能适用于凸的数据集
- 对大规模数据的性能要好

针对以上三点，作者提出了DBSCAN算法。这种算法本质是认为同一个类别的数据点集在空间中的密度是比较大的，而不同类别的数据集间的点在空间中密度很小。如下图，应该被分割成四个类别 



![img](https://pic2.zhimg.com/80/v2-4861b4071b920bcedcdae2d578c1bc0d_hd.png)

DBSCAN算法首先需要定义一些概念：



- Ε邻域：设定半径为E，则以某对象为中心，半径为Ε内的区域称为该对象的Ε邻域
- 核心对象：如果给定对象的Ε领域内的样本点数大于等于MinPts，则称该对象为核心对象
- 直接密度可达：对于样本p、q，如果样本点q在p的Ε领域内，并且p为核心对象，那么称对象q从对象p直接密度可达
- 密度可达：给定一系列样本点p1,p2….pn，p= p1,q= pn,假如对象pi从pi-1直接密度可达，那么对象q从对象p密度可达。
- 密度相连：存在样本集合D中的一点o，如果对象o到对象p和对象q都是密度可达的，那么p和q密度相联

DBSCAN算法的具体步骤如下（引用的[DBSCAN](http://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/DBSCAN%23Algorithm)伪代码，比原文中的易懂）

```text
DBSCAN(D, eps, MinPts) {
    # 找到核心对象P
   C = 0
   for each point P in dataset D {
      if P is visited
         continue next point
      mark P as visited
      NeighborPts = regionQuery(P, eps)
      if sizeof(NeighborPts) < MinPts
         mark P as NOISE
      else {
         C = next cluster
         expandCluster(P, NeighborPts, C, eps, MinPts)
      }
   }
}
 
expandCluster(P, NeighborPts, C, eps, MinPts) {
    # 找到所有从核心对象P密度可达的点
   add P to cluster C
   for each point P'' in NeighborPts {
      if P'' is not visited {
         mark P'' as visited
         NeighborPts'' = regionQuery(P'', eps)
         if sizeof(NeighborPts'') >= MinPts
            NeighborPts = NeighborPts joined with NeighborPts''
      }
      if P'' is not yet member of any cluster
         add P'' to cluster C
   }
}
 
regionQuery(P, eps)
# 查询点P的E邻域的点集
   return all points within P''s eps-neighborhood (including P)
```

原文中还有如何探索式的选择合适的参数，这里不再赘述。

## Mixture of Gaussians

在"**Dempster, A. P., N. M. Laird, and D. B. Rubin. "Maximum Likelihood from Incomplete Data via the EM Algorithm." 1977**"，作者提出了著名的EM算法（Expectation-Maximization）。在EM的应用部分，作者介绍了利用EM算法来求解混合高斯模型（GMM，Gaussian Mixture Model）的参数。

在GMM中，GMM 的概率密度函数由 ![K](https://www.zhihu.com/equation?tex=K) 个 Gaussian 分布线性组成，如下。因此，利用ＧＭＭ来聚类，本质就是对数据集用ＧＭＭ拟合，并估计参数。求出的![K](https://www.zhihu.com/equation?tex=K)个Gaussian就对应着![K](https://www.zhihu.com/equation?tex=K)个类别。



![img](https://pic3.zhimg.com/80/v2-9bfff5d4c8716821594dd9b5780fe02a_hd.png)

以下简要介绍利用ＥＭ算法估计GMM参数的过程：



- \1. 对于![N](https://www.zhihu.com/equation?tex=N)个数据点，![K](https://www.zhihu.com/equation?tex=K)个Gaussian分布，其联合概率密度可以表示为 

![img](https://pic3.zhimg.com/80/v2-74993d9151eaf0092903ac1888ed1efe_hd.png)

- \2. 对于数据![x_{i}](https://www.zhihu.com/equation?tex=x_%7Bi%7D)，它属于第![k](https://www.zhihu.com/equation?tex=k)个类别的概率为

![img](https://pic2.zhimg.com/80/v2-790f78cd89f33f1f3a0bc0b667958445_hd.png)

![img](https://pic3.zhimg.com/80/v2-c770c5e9b2d3b0f31b779c964fa20e86_hd.png)

- \3. 利用最大似然估计，可以得到每个Gaussian的参数为。其中![N_{k}=\sum_{i=1}^{N}{\gamma (i,k)} ](https://www.zhihu.com/equation?tex=N_%7Bk%7D%3D%5Csum_%7Bi%3D1%7D%5E%7BN%7D%7B%5Cgamma+%28i%2Ck%29%7D+)，并且![\pi _{k}=N_{k}/N](https://www.zhihu.com/equation?tex=%5Cpi+_%7Bk%7D%3DN_%7Bk%7D%2FN)

![img](https://pic1.zhimg.com/80/v2-1ff4a54a60739c6ccb28150441481ad0_hd.png)

- \4. 重复步骤2~3，直到似然函数的值收敛为止
- \5. 更多的细节，可以翻阅原文，同时可以参考[这个博客](http://link.zhihu.com/?target=http%3A//www.cnblogs.com/jerrylead/archive/2011/04/06/2006936.html)

## Density Peaks

此聚类算法是2014年发表在Science的一篇文章，"**Clustering by fast search and find of density peaks**"，此算法是基于密度的一种聚类算法。传统的DBSCAN算法需要提前设定大量参数，且结果对阈值较敏感；而此算法对阈值更加鲁棒。

- \1. DP算法本质上是通过两个指标来确定聚类中心。一个是局部密度，即它认为聚类中心的局部密度应该是比较大的，其公式如下 

![img](https://pic1.zhimg.com/80/v2-8524fb8f527596a8fd55b9c6e78318a4_hd.png)

- \2. 另一指标是个离密度较大点的距离，即它认为不同聚类中心之间的距离是比较大的，其公式如下 

![img](https://pic4.zhimg.com/80/v2-96a98dfebc2d38b6f537e0dda585290f_hd.png)

![img](https://pic2.zhimg.com/80/v2-ab6d3b5896c4778164a697c9d962e329_hd.png)

- \3. 在对每个点计算指标后，设定阈值选择![\rho _{i}](https://www.zhihu.com/equation?tex=%5Crho+_%7Bi%7D)和![\delta _{i}](https://www.zhihu.com/equation?tex=%5Cdelta+_%7Bi%7D)都比较大的点，作为聚类中心。如下 

![img](https://pic2.zhimg.com/80/v2-8e1dbded3706e35723fc70dd6e89e959_hd.png)

- \4. 当确定了聚类中心后，某个点的类别被设定为与“离它最近，且密度比它大”的点相同的类别。可以看出，与DBSCAN类似，此聚类算法在选择所属类别时也有“传递”的概念在，因此理想情况下可以对任意形状的数据进行聚类。效果如下 

![img](https://pic3.zhimg.com/80/v2-dadb7376310f80e86537cf5ce27a66c2_hd.png)

- \5. 对含有噪声的数据，该算法并不是直接选择全局阈值来确定噪声点。不过本质上，DP算法依然是将局部密度较小的点当成是噪声。详细的步骤，以及作者推荐的参数选择办法如下 

![img](https://pic3.zhimg.com/80/v2-6de9b5ed03af7b7daedfff4bf55d545e_hd.png)

![img](https://pic2.zhimg.com/80/v2-fee40f521929c0dd4a0d640552f2eedd_hd.png)

## **本系列其他文章：**

- [当我们在谈论K-means：数据概述](https://zhuanlan.zhihu.com/p/24911284?refer=data-miner)
- [当我们在谈论K-means：论文概述（1）](https://zhuanlan.zhihu.com/p/25024565?refer=data-miner)
- [当我们在谈论K-means：论文概述（2）](https://zhuanlan.zhihu.com/p/25032775?refer=data-miner)
- 当我们在谈论K-means：其他聚类算法
- [当我们在谈论K-means：总结](https://zhuanlan.zhihu.com/p/25032944?refer=data-miner)