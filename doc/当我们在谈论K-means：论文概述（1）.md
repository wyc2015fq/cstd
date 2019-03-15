# 当我们在谈论K-means：论文概述（1）

[![余文毅](https://pic3.zhimg.com/a3e2422ec_xs.jpg)](https://www.zhihu.com/people/wyyualan)

[余文毅](https://www.zhihu.com/people/wyyualan)

不自由职业者

4 人赞同了该文章

本系列意在长期连载分享，内容上可能也会有所增删改减； 

**因此如果转载，请务必保留源地址，非常感谢！**

**知乎专栏：当我们在谈论数据挖掘**

**博客园：当我们在谈论数据挖掘（暂时公式显示有问题）**

## 引言

上一篇文章，对从1969年以来，与K-means相关文章的数据进行了简单描述，并对其中某些数据趋势尝试分析。这属于对数据的整体情况的一个简要分析。

本篇文章，则希望能通过简单介绍K-means一路以来一些相关文章，进而能大概梳理下该算法的发展进程。

## 1967年

公认的K-means术语的最初使用是在"**J. MacQueen, Some Methods for classification and Analysis of Multivariate Observations, 1967**"。根据wiki的说法，K-means的算法还能追溯到更早的时候，然而这就不在本文的讨论范围内了。文章中给出了最初的k-means定义，即

- \1. An initial clustering is created by choosing k random centroids from the dataset.For each data point, calculate the distance from all centroids, and assign its membership to the nearest centroid.
- \2. Recalculate the new cluster centroids by the average of all data points that are assigned to the clusters.
- \3. Repeat step 2 and 3 until convergence.

## 1979年

在"**Hartigan, J. A.; Wong, M. A., "Algorithm AS 136: A K-Means Clustering Algorithm, 1979**"中，参考原始K-means算法，作者提出了一种新的聚类算法，同时也提出了一种初始化聚类中心的办法：

- \1. 该聚类算法本质是通过定义损失函数，迭代将每个点分配到更优的类别，促使损失函数持续减小；若损失函数不再减小，则停止迭代。思想上是梯度下降，因此与K-means一样只能达到局部最优。
- \2. 在作者也提出了一种初始化聚类中心的方法，即：先计算整体样本中心，然后根据样本点到中心的距离，由近至远均匀采样作为初试聚类中心。这样可以避免初次聚类后，某类为空的问题。

## 1984年 

在“**Selim, Shokri Z., and Mohamed A. Ismail. "K-means-type algorithms: a generalized convergence theorem and characterization of local optimality." 1984**”中，作者表示还没有文章对K-means的性质进行系统的理论分析。针对这一情况，作者主要对K-means的一些性质进行了理论上的推导与证明：

- \1. 作者首先根据最初K-means的定义给出损失函数表达式![P](https://www.zhihu.com/equation?tex=P)： 

![img](https://pic3.zhimg.com/80/v2-e01678f39002bc7ea2ee28b41df960fe_hd.png)

![img](https://pic3.zhimg.com/80/v2-e01678f39002bc7ea2ee28b41df960fe_hd.png)

- \2. 然后给出等价的损失函数![RP](https://www.zhihu.com/equation?tex=RP)，并证明其是非凸的：

![img](https://pic1.zhimg.com/80/v2-1cf19895f632f2e875196f51288f5500_hd.png)

![img](https://pic1.zhimg.com/80/v2-0b70ca49dbd7b6a872b81b7bc70c3f04_hd.png)

![img](https://pic2.zhimg.com/80/v2-cfd433de5dced8c5e56baa58ad837d5d_hd.png)

- \3. 接下来给出K-means类型的聚类算法的思想（本质就是现在所说的EM算法的一种应用）：

![img](https://pic3.zhimg.com/80/v2-88bf8e6de2be6655a3f2b87d0bfc67f2_hd.png)

![img](https://pic2.zhimg.com/80/v2-bf87eb95888299833cf13681ebf6b6f5_hd.png)

- \4. 当距离定义为minkowski距离时，K-means可能无法收敛到局部最小值，即

![img](https://pic1.zhimg.com/80/v2-2966e3a2cab97fbb3a2c0ba000909d18_hd.png)

- \5. 当距离定义为二次距离时，K-means可以收敛到局部最小值，即 

![img](https://pic1.zhimg.com/80/v2-e34292366c081364308c0414dc2aac74_hd.png)

## 1995年（1）

在“**Cheng, Yizong. "Mean shift, mode seeking, and clustering." 1995**”中，作者对前人提出的Mean Shift算法进行了扩展并研究了其性质，同时证明了K-means算法是Mean Shift算法的特殊形式。这篇文章也奠定了Mean Shift算法能被广泛使用的基础。

- \1. 作者首先给出原始Mean Shift算法的定义。算法本质就是迭代去寻找离某每个点最近的，且稳定的点；接下来把收敛到同个稳定点的所有点作为一类。这里稳定可以理解该点邻域质心与邻域中心一致。 

![img](https://pic1.zhimg.com/80/v2-b4b764aa7722da3b7e482139f8259108_hd.png)

- \2. 然后作者在此基础上进行了扩展，加入了核函数和权重。由于Mean Shift本质是迭代逼近密度局部最大的点，有了核函数，就可以自由定义这种密度的表达，因此适用性大大增强 

![img](https://pic1.zhimg.com/80/v2-6fd62b0a049b85e5e7810d40fb1fe1c8_hd.png)

- \3. 作者表示，一篇关于“Maximum Entropy”的文章里已经证明了K-means为“Maximum Entropy”算法的特殊形式；本文作者又证明了“Maximum Entropy”算法为扩展后Mean Shift的特殊形式，于是K-means即为Mean Shift的特殊形式
- \4. 作者尝试利用Mean Shift进行聚类，而现在Mean Shift也是一种常见的聚类算法之一
- \5. 由于Mean Shift可以逼近概率密度局部最大点，作者也尝试利用Mean Shift来求解最优化问题
- \6. 此外，作者还定义了很多相关概念，并进行了性质的说明与证明，此处不再赘述

## 1995年（2）

在“**Chinrungrueng, C., and C. H. Sequin. "Optimal adaptive k-means algorithm with dynamic adjustment of learning rate. " 1995**”中，作者对之前on-line的K-means算法进行了修改

- \1. 作者先定义K-means算法的损失函数，即最小均方误差

![img](https://pic1.zhimg.com/80/v2-2b2224b35a45867b48d97a56c6b98ca0_hd.png)

- \2. 接下来介绍以前的Adaptive K-means算法，这种算法的思想跟梯度下降法差不多。其所存在的问题也跟传统梯度下降法一样，如果步长![\mu ](https://www.zhihu.com/equation?tex=%5Cmu+)过小，则收敛时间慢；如果步长![\mu ](https://www.zhihu.com/equation?tex=%5Cmu+)过大，则可能在最优点附近震荡。

![img](https://pic4.zhimg.com/80/v2-a573041c330e364329234c6b6a65d37f_hd.png)

- \3. 接下来，作者将损失函数进行修改，即改成了加权的最小均方误差

![img](https://pic4.zhimg.com/80/v2-e13c7fbfb4a0a3c695c6d5a9e7ebefe7_hd.png)

![img](https://pic4.zhimg.com/80/v2-a302e32eeacba142b796b7b7f5db0757_hd.png)

- \4. 然后作者引用文献的证明，表示“当类别数![K](https://www.zhihu.com/equation?tex=K)很大，且样本的真实概率密度函数![P](https://www.zhihu.com/equation?tex=P)平滑的假设下，使用K-means算法进行分割，每一个区域的方差是相同的”。于是，在这个前提下，即“每个类别的方差相同”的前提下，修改后的K-means可以自适应计算learning rate ![\mu ](https://www.zhihu.com/equation?tex=%5Cmu+)。![\mu ](https://www.zhihu.com/equation?tex=%5Cmu+)会在初期近似于1，而在接近最优时会趋于0，加快了搜索速度，同时避免步长太大在最优点附近震荡的情况。限于篇幅，![\mu ](https://www.zhihu.com/equation?tex=%5Cmu+)的更新公式不再本文贴出，有兴趣的可以去文章中找。

## 1997年

在“**Chaudhuri, D., and B. B. Chaudhuri. "A novel multiseed nonhierarchical data clustering technique. " 1997**”中，针对传统K-means算法在非凸数据集下较差的表现，作者提出了修改的K-means。传统的K-means针对每一个类别选择一个聚类中心，但是对于非凸或长条状的类别，一个聚类中心并不是很好的选择（博主注：针对非凸与长条等数据，个人认为也可以在特征转换层面处理，但是这样就要求首先对数据有深入的分析，而且适用性比较差）。本文提出，一个类别可以包含多个聚类中心，使K-means在非凸情况下也能有较好的表现。

- \1. 为了判断聚类的形状，作者首先提出可以通过找出边界点，然后根据边界点“最大最小距离比”来判断。当比值接近1时，说明形状是接近超球体的；当比值远大于1时，说明是长条或非凸的。为了求边界点，先求取所有候选边界点，的方式如下。本质就是如果一个点很少被别的点包围，则处于边界。 

![img](https://pic2.zhimg.com/80/v2-cdcdb969157e783aa7c15a0e552d7e4d_hd.png)

![img](https://pic3.zhimg.com/80/v2-7f461fc73406b3c92a36e6d28e81298e_hd.png)

- \2. 然后开始求取边界点：给定边界点数目![m](https://www.zhihu.com/equation?tex=m)，边界点的求解公式如下，本质就是从所有候选边界点中找出![m](https://www.zhihu.com/equation?tex=m)个最突出的作为边界点

![img](https://pic1.zhimg.com/80/v2-40fa4cad9014ca99613ce3a8cc9828e0_hd.png)

- \3. 接下来初始化聚类中心。本质还是认为密度较大的，即聚类中心。通过找到密度最大点，然后将它周围的球体区域内的点都认为暂时属于这个类，并从整体数据中删除。重复这个过程，直到得到所有的聚类中心。

![img](https://pic1.zhimg.com/80/v2-b380167650dc4f9971005a2403d18e04_hd.png)

- \4. 初次聚类：根据最近邻，将每个点分配到相应的聚类中心。
- \5. 为了让一个类别能出现多个聚类中心，作者增加了一步，即类别合并（本质是从上往下的层次聚类）：即将所有聚类中心看做图的节点，边的权重即两个聚类中心的距离，然后对这幅图绘出最小生成树；在生成了树后，再重新计算每条边的权重，这时候权重被重新定义为一种”疏远度“；迭代剪断”疏远度“最大的边，直到剩下的类别数为K。整个流程被作者描述的相当复杂，如下 

![img](https://pic4.zhimg.com/80/v2-fad947527a95099cd99e208cc70eaadb_hd.png)

![img](https://pic1.zhimg.com/80/v2-2dadda3ee1d312495e15adf32152528c_hd.png)

![img](https://pic3.zhimg.com/80/v2-943942a6ee2bbd0124e1cf08774b404a_hd.png)

- \6. 更新聚类中心：此步骤包含两个子步骤：

- 6-1. 为数据重新分配类别。每个数据点的类别定义为，离它最近的聚类中心的类别。 

- 6-2. 分别更新类内聚类中心。对每个类别，若有多个聚类中心，则按照标准K-means的更新聚类中心方法重新计算聚类中心。

- \7. 重复5~6步，直到达到一定迭代次数或者聚类中心稳定。

- 除了提出算法外，作者文中还提到几种初始化聚类中心的方法：

- - \1. 初步将数据分成![K](https://www.zhihu.com/equation?tex=K)个区域，将每个区域中心作为初始聚类中心
  - \2. 计算出每个点的”密度“，认为”密度“较大的是聚类中心。先把”密度“最大的挑出作为第一个聚类中心，从剩下的点中找出密度最大，且离所有已有聚类中心大于一定距离的点作为下一个聚类中心，直到选择了![K](https://www.zhihu.com/equation?tex=K)个
  - \3. 计算整体均值，作为第一个聚类中心。从剩下的点中顺序寻找，当遇到离所有已有聚类中心大于一定距离的点，则作为下一个聚类中心，直到选择了![K](https://www.zhihu.com/equation?tex=K)个

## 1999年

在"**Pelleg, et al. "Accelerating exact k -means algorithms with geometric reasoning." 1999.**"中，针对传统K-means算法计算复杂度高因此费时的情况，作者提出通过“KD树”来加速K-means算法。作者通过实验说明，加速后的算法比原始K-means算法快25~175倍。

- \1. 首先，将数据用“KD树”这种结构存储。关于构造“KD树”的详细资料，可以参考本文的[KD-trees部分](http://link.zhihu.com/?target=http%3A//www.cs.cmu.edu/%7Edpelleg/kmeans.html)。这里仅进行一些额外的补充说明：

- - “KD树”其实就是一种二叉树
  - 树的每个节点代表一个矩形，存储着这个矩形中数据点的信息：![h_{min}, h_{max}](https://www.zhihu.com/equation?tex=h_%7Bmin%7D%2C+h_%7Bmax%7D)，这两个向量用于描述这个矩形的边界；还有矩形中数据点的数量
  - 树的每个非叶子节点还额外存储关于分裂成子树的信息：划分子树的值；此数值所在的维度
  - 树的根节点表示囊括所有数据的大矩形
  - 树的叶子节点存储具体的数据点信息，即该矩形中每个数据点的值
  - **树的深度是提前设定好的，并不一定要分裂到不可再分** 

![img](https://pic1.zhimg.com/80/v2-30554592e943a0cf6f2a9d5ab0a9ab38_hd.png)

- \2. 将数据用“KD树”存储后，利用树的性质，一般能比传统K-means更快找到每个点最近的聚类中心。详细的算法比较复杂，有兴趣的可以参考论文。这里给出几点说明帮助理解：

- - 比之传统K-means对每个样本点找最近邻聚类中心，“KD树”对其中的一个矩形找最近的聚类中心。对某个矩形（即树中某节点），若它平面中的每个点（区别于样本点）到某聚类中心的距离，比到其他聚类中心的距离都近，那么该矩形中所有样本点的最近邻聚类中心就能一次性确定下来了。
  - 由于并非树中所有矩形（即节点），都能找到满足以上条件的聚类中心。对于这些矩形，只能用传统办法求其中样本点的最近邻聚类中心
  - 因此，在最坏情况下，即树中每个矩形都不满足上述条件，“KD树”的K-means算法会比传统K-means更慢（因为还要构造树）。于是，“KD树”分裂的深度，是影响这个算法性能的一个重要参数

## 2000年

在"**Pelleg, et al. "X-means: Extending K-means with Efficient Estimation of the Number of Clusters. Intelligent Data Engineering and Automated Learning" 2000.**"中，作者提出了一种改进的K-means算法，即X-means

- \1. 作者先提出K-means聚类算法面临的三个主要问题，并表示X-means能解决前两个问题，并改善第三个问题

- - 计算量大
  - 聚类数量![K](https://www.zhihu.com/equation?tex=K)需要提前设定
  - 只能收敛到局部最优

- \2. 算法的整体思路如下，本质就是由上而下的层次聚类法： 

- - 2-1. 先设定![K](https://www.zhihu.com/equation?tex=K)的范围![[K_{min} ,K_{max} ]](https://www.zhihu.com/equation?tex=%5BK_%7Bmin%7D+%2CK_%7Bmax%7D+%5D)，令![K=K_{min}](https://www.zhihu.com/equation?tex=K%3DK_%7Bmin%7D)
  - 2-2. 迭代2-3~2-4， 并令![K=K*2](https://www.zhihu.com/equation?tex=K%3DK%2A2)，直到![K>K_{max}](https://www.zhihu.com/equation?tex=K%3EK_%7Bmax%7D)
  - 2-3. 执行传统K-means
  - 2-4. 将每一个聚类中心分裂成两个，并检验性能是否提升，是则保留，否则放弃分裂
  - 文章截图如下

![img](https://pic3.zhimg.com/80/v2-e4f8aec374ed625fbeb69a5438fc2c86_hd.png)

- \3. 聚类中心分裂算法： 

- - 3-1. 对于其中一个聚类中心，首先将它分裂成两个，并让这两个点沿着相反的方向移动。移动的距离与这个区域大小成正比，移动的方向随机
  - 在父聚类中心的区域中，对这两个分裂后的聚类中心执行传统K-means算法
  - 利用 Bayesian information criterion (BIC)/Schwarz criterion计算得分，挑选得分较优秀的模型作为最终模型。BIC本质就是计算似然，但是为了克服过拟合，又增加对参数数量的惩罚项。详情可以参考wiki的[BIC词条](http://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Bayesian_information_criterion)。文中的BIC定义如下

![img](https://pic4.zhimg.com/80/v2-579225b19a9146f30be7acc76fd19533_hd.png)

- \4. 本文作者与上文"**Pelleg, et al. "Accelerating exact k -means algorithms with geometric reasoning." 1999.**"作者是一样的，因此作者还提出能够用“KD树”来加速X-means算法。加速算法的细节不再赘述

## **本系列其他文章：**