# 当我们在谈论K-means：论文概述（2）

[![余文毅](https://pic3.zhimg.com/a3e2422ec_xs.jpg)](https://www.zhihu.com/people/wyyualan)

[余文毅](https://www.zhihu.com/people/wyyualan)

不自由职业者

10 人赞同了该文章

本系列意在长期连载分享，内容上可能也会有所增删改减； 

**因此如果转载，请务必保留源地址，非常感谢！**

**知乎专栏：当我们在谈论数据挖掘**

**博客园：当我们在谈论数据挖掘（暂时公式显示有问题）**

## 2001年

在“**Estlick, Mike, et al. "Algorithmic transformations in the implementation of K- means clustering on reconfigurable hardware." 2001**”中，作者将K-means算法用在FPGA板子中。在传统K-means中，用到了浮点数运算与乘法运算，而这两种运算在FPGA中非常耗时。为了能在FPGA中高效使用K-means算法，作者提出了修改的K-means算法。

- \1. 先介绍一下明氏距离（Minkowski Distance），其定义如下

![img](https://pic2.zhimg.com/80/v2-00a83493ce49ce82b3960c4692f33c11_hd.png)

- - 1-1. 如果令![P=2](https://www.zhihu.com/equation?tex=P%3D2)，即得到常见的欧氏距离（Euclidean Distance）；从概率的角度看，欧氏距离即认为数据服从标准多维正态分布，其概率密度函数中，欧氏距离描述的就是空间中的点偏离中心的概率，相同的欧氏距离即对应着概率密度函数的等高线

![img](https://pic2.zhimg.com/80/v2-7732781797c4a7b84202681a14f4c601_hd.png)

- - 1-2. 如果令![P=0](https://www.zhihu.com/equation?tex=P%3D0)，即得到曼哈顿距离（Manhattan Distance），即每个维度的绝对值的和；当计算像素欧氏距离复杂度较高，有时候可以使用曼哈顿距离作为替代

![img](https://pic1.zhimg.com/80/v2-cb22df2bf0a4e66683b53da58f374ddc_hd.png)

- - 1-3. 令![P\rightarrow \infty ](https://www.zhihu.com/equation?tex=P%5Crightarrow+%5Cinfty+)，即切比雪夫距离（Chebyshev Distance），即取不同纬度间的最大值；不过我也不知道什么时候会用上它

![img](https://pic1.zhimg.com/80/v2-28da9411837bf0e79ee8d33c37969ea0_hd.png)

- - 1-4. 在此我们可以再总结一些常见的距离度量，如马氏距离(MahalanobisDistance)；从概率角度看，其作用就是用多维正态分布拟合数据，描述的同样是空间中的点偏离中心的概率，相同的马氏距离即对应着概率密度函数的等高线

![img](https://pic4.zhimg.com/80/v2-eb7a59121f710e52eb8290d54c7e70d3_hd.png)

- - 1-5. 余弦相似度（Cosine Similarity），描述的是两个向量的夹角大小

![img](https://pic2.zhimg.com/80/v2-adb3d7e4393c51adddbad6348cfa26d1_hd.png)

- - 1-6. Jaccard相似系数（Jaccard Coefficient），描述的是两个集合的相似性

![img](https://pic3.zhimg.com/80/v2-09d02762dba11a08bdcdc2dd707ef3da_hd.png)

- \2. 作者表示在FPGA中，欧氏距离的计算量太大，他希望用“曼哈顿距离”和“切比雪夫距离”替代。下图表示，空间中两个聚类中心，使用不同距离的分界面 

![img](https://pic3.zhimg.com/80/v2-fe405a8bea30e412418d32a276f8c3d6_hd.png)

- \3. 单独使用“曼哈顿距离”和“切比雪夫距离”都无法很好地替代“欧氏距离”，于是作者将两者融合，并说明效果的下降在允许范围内，而计算量大大降低。（想法很有趣） 

![img](https://pic1.zhimg.com/80/v2-6d4aeca0ab2309be26f5d366a4566a04_hd.png)

## 2002年

在"**Kanungo, Tapas, et al. "An Efficient k-Means Clustering Algorithm: Analysis and Implementation." 2002**"中，面对K-means运算量较大的问题，作者提出了“KD树”加速K-means算法的方法。

但是，其方法基本跟"**Pelleg, et al. "Accelerating exact k -means algorithms with geometric reasoning." 1999.**"没什么区别。此处不再赘述。

## 2004年

在"**Lee, Sangkeun, and M. H. Hayes. "Properties of the singular value decomposition for efficient data clustering." 2004**"中，作者对SVD的性质进行了讨论，并表示这些性能能加快K-means的过程。

- \1. 作者首先给出了对数据集A进行SVD的解释 

![img](https://pic1.zhimg.com/80/v2-882665ec82fcf1161f07b45bcdd51ef0_hd.png)

- \2. 然后给出了本文最主要的公式，即![A](https://www.zhihu.com/equation?tex=A)中每两个向量的欧氏距离，可以用对应的“右奇异向量”的加权和表示。（注：这里我们进一步分析，由于![A](https://www.zhihu.com/equation?tex=A)是一个![m*n](https://www.zhihu.com/equation?tex=m%2An)的矩阵，![V](https://www.zhihu.com/equation?tex=V)是一个![n*n](https://www.zhihu.com/equation?tex=n%2An)的矩阵，若要SVD分解后能加速K-means，至少要求![m>n](https://www.zhihu.com/equation?tex=m%3En)，即样本维数大于样本数量，然而这种情况比较少见。同时，SVD分解本身也是个非常耗时的操作。因此此方法更多的是提供一种思考方式。） 

![img](https://pic1.zhimg.com/80/v2-1af9454b7846212c177662f48fa3fd98_hd.png)

- \3. 本文还给出了一种设置聚类中心数量![K](https://www.zhihu.com/equation?tex=K)的方法。本质跟PCA类似，就是计算数据集![A](https://www.zhihu.com/equation?tex=A)的主要能量聚集在多少维度上。区别是PCA需要的是这几个维度对应的向量，而这里只需要维度的数量。 

![img](https://pic3.zhimg.com/80/v2-682a1156aa7e59360b1d9e4c16ca2d4a_hd.png)

- \4. 文中还有更多利用SVD加速K-means聚类的细节，不再赘述

## 2005年

在"**Huang, Joshua Zhexue, et al. "Automated Variable Weighting in k-Means Type Clustering." 2005**"中，作者针对K-means算法中，每一维特征在聚类结果中权重相同的情况，提出了修改的K-mwans。

- \1. 作者首先提出，在数据挖掘过程中，往往数据的维数都是成百上千，而其中对分析有意义的维数只是部分。以往根据经验给每一维数据赋权重，作者提出一种算法来自动求出权重。
- \2. 先给出原始K-means的损失函数，即最小均方误差 

![img](https://pic4.zhimg.com/80/v2-b3135f964795694de9c70a9b0132ba03_hd.png)

![img](https://pic2.zhimg.com/80/v2-fb9034584941dc109f4e59ca81896e25_hd.png)

- \3. 然后作者给出修改的K-means的损失函数。本质就是在损失函数里增加了权重，然后继续通过EM算法求解。在最小均方误差的约束下，类内距离小的那一维特征会被赋予较大的权重，类内距离较大的则会被赋予较小的权重。即作者所说的，自动求解权重 

![img](https://pic1.zhimg.com/80/v2-0a018ad8ea3f25714454bef184600790_hd.png)

![img](https://pic2.zhimg.com/80/v2-8ce50c38ab530f4e2893fb08213ba675_hd.png)

- \4. 关于详细的求解步骤，与收敛性的证明，可以参考原论文

## 2006年

在"**Kuncheva, L. I., and D. P. Vetrov. "Evaluation of Stability of k-Means Cluster Ensembles with Respect to Random Initialization." 2006**"中，作者研究了通过Ensembling来提升K-means等算法的稳定性

- \1. 作者先明确了研究的问题，即

- - Ensembling是否能提升聚类的稳定性？
  - 是否聚类的稳定性能与准确性正相关？
  - 是否能利用聚类稳定性指标来描述聚类的有效性？

- \2. 作者给出了Ensembling的方法，即把数据分成![L](https://www.zhihu.com/equation?tex=L)组，再分别对![L](https://www.zhihu.com/equation?tex=L)组的数据进行聚类，并将结果融合

![img](https://pic2.zhimg.com/80/v2-fffb3768f4c006db0088c180624d3dc1_hd.png)

- \3. 对于上述问题，作者都没有给出理论证明，都是实验上的说明：

- - **Ensembling是否能提升聚类的稳定性？**大部分情况下，Ensembling能提升聚类的稳定性。同时需要说明的是，Ensembling更稳定的情况基本发生在聚类中心较大的时候，即Ensembling会倾向于选择更多的聚类中心
  - **是否聚类的稳定性能与准确性正相关？** 跟设想的结果差不多，聚类的稳定性跟准确性并没有明确的正相关。不同的数据集上，有着完全不同的相关性。
  - **是否能利用聚类稳定性指标来描述聚类的有效性？**在这部分，作者主要阐述了利用聚类稳定性指标来选择聚类中心数量的想法。即，作者通过给出一个稳定性指标，表示在稳定性较大的时候的聚类中心数量会很接近真实的类别数量。

## 2007年

在"**Arthur, David, and S. Vassilvitskii. "k-means++: the advantages of careful seeding." 2007**"中，作者提出了K-means++算法，也是较为常用的K-means修改算法之一。这个算法主要提出了一种选择初始化聚类中心的方法，并从理论上证明了这个方案会使收敛更快，且效果更好

- \1. 这个初始化聚类中心的方法其实很简单：即以概率的形式逐个选择聚类中心，并在选择聚类中心时，给距离较远的点更高的权重，即更容易被选择为聚类中心

![img](https://pic3.zhimg.com/80/v2-24b8545f0ab2bbcc28c4c979774cff96_hd.png)

- \2. 这个想法其实并不是非常新奇，这种逐个选择聚类中心的思想，在1997年就有作者提出过（参考“**当我们在谈论kmeans：论文概述（1），1997**”）。但是作者在这个初始化聚类中心方法的基础上，接下来又证明了通过这种方法，平均均方误差大大降低，且收敛速度更快。证明过程好复杂，大家可以自己去研读。

## 2010年

在"**Chiang, Ming Tso, and B. Mirkin. "Intelligent Choice of the Number of Clusters in K-Means Clustering: An Experimental Study with Different Cluster Spreads." 2010**"中，针对K-means算法中聚类中心数量难以确定的问题，作者通过实验的方式，比较了多种估计K-means聚类中心数量的方法。并通过实验对比了这些方法在估计类别数量、中心、标记时的准确度。

- \1. 作者首先介绍了Mirkin提出的Intelligent K-means算法，本质是通过异常检测的思想，一步步确定每个类别。具体描述如下

![img](https://pic3.zhimg.com/80/v2-e38cd7ec6466b415ec7be7590e3df0a2_hd.png)

![img](https://pic3.zhimg.com/80/v2-7027a896fd630edbee4a6e5fd6e30b72_hd.png)

- \2. 为了选择对照算法，作者总结了其他估计聚类数量![K](https://www.zhihu.com/equation?tex=K)的算法。针对不同类型的方法，作者也给出了例子。有兴趣的同学可以参考原文。

- - 基于变化的算法：即定义一个函数，随着![K](https://www.zhihu.com/equation?tex=K)的改变，认为在正确的![K](https://www.zhihu.com/equation?tex=K)时会产生极值。如Gap Statistic（Estimating the number of clusters in a data set via the gap statistic, Tibshirani, Walther, and Hastie 2001），Jump Statistic （finding the number of clusters in a data set, Sugar and James 2003）
  - 基于结构的算法：即比较类内距离、类间距离以确定![K](https://www.zhihu.com/equation?tex=K)。这个也是最常用的办法，如使用平均轮廓系数，越趋近1聚类效果越好；如计算类内距离/类间距离，值越小越好；等。
  - 基于一致性矩阵的算法：即认为在正确的![K](https://www.zhihu.com/equation?tex=K)时，不同次聚类的结果会更加相似，以此确定![K](https://www.zhihu.com/equation?tex=K)。
  - 基于层次聚类：即基于合并或分裂的思想，在一定情况下停止获得![K](https://www.zhihu.com/equation?tex=K)。
  - 基于采样的算法：即对样本采样，分别做聚类；根据这些结果的相似性确定![K](https://www.zhihu.com/equation?tex=K)。如，将样本分为训练与测试样本；对训练样本训练分类器，用于预测测试样本类别，并与聚类的类别比较。

- \3. 最后通过对比实验，作者给出结论认为Intelligent K-means能较为有效的估计真实聚类中心、以及样本所属类别。同时，Intelligent K-means对类别数量的估计普遍较大。不过由于实验是在高斯分布的仿真实验下进行的，结论并非我所关注，不再赘述。

## **本系列其他文章：**