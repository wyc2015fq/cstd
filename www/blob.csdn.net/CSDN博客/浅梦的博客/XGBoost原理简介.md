# XGBoost原理简介 - 浅梦的博客 - CSDN博客





2017年09月28日 13:50:11[浅梦s](https://me.csdn.net/u012151283)阅读数：1282








# 模型函数形式

![这里写图片描述](https://img-blog.csdn.net/20170928124715088?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170928124806699?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 正则化的目标函数

![这里写图片描述](https://img-blog.csdn.net/20170928124927562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 梯度提升树

令$\hat{y}_i^{(t)}$表示第t次迭代时对第i个样本的预测值。现在需要添加$f_t$来使得下面的目标函数最小化 
$L^{(t)}\approx \sum\limits_{i=1}^nl(y_i,\hat{y_i}^{(t-1)}+f_t(x_i))+\Omega(f_t)$

二阶近似算法可以用来快速优化目标函数。 
![这里写图片描述](https://img-blog.csdn.net/20170928130121087?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170928130317232?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 样本累加统一为叶结点累加形式

可以看到当前的损失函数为 


$\begin{align*}\hat{L}^{(t)}=&\sum\limits_{i=1}^{n}[g_if_t(x_i)+\frac{1}{2}f_t^2(x_i)]+\Omega(f_t)\\=&\sum\limits_{i=1}^{n}[g_iw_{q(x_i)}+\frac{1}{2}w_{q(x_i)}^2]+\gamma T+\lambda \frac{1}{2}\sum\limits_{j=1}^Tw_j^2\end{align*}$
![这里写图片描述](https://img-blog.csdn.net/20170928131459160?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170928131616747?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

上式可以用用来衡量结构为q的树的质量。这类似于决策树中的不纯度得分，只不过可以用在更广泛的目标函数中。
## 回归树的学习策略

![这里写图片描述](https://img-blog.csdn.net/20170928131940184?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170928132049235?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 缩减和列采样

除了正则化的目标函数外，两个额外的技术也被用来防止过拟合。
- 缩减在树提升的每一轮次以因子$\eta$缩减新添加的树的权重。减少未来每一棵独立的树和叶子结点的影响。
- 列（特征）抽样，该技术在随机森林中有应用。即每次选择特征的一个子集进行最优分裂特征选取。该技术同时加快了并行算法的计算。

# 树结点分裂方法Split Finding

## Basic Exact Greedy Algorithm

![这里写图片描述](https://img-blog.csdn.net/20170928133238462?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

对于连续特征，枚举所有可能的分割点的计算开销是非常大的。为了更高效地实现，算法要预先对特征取值进行排序，然后以排序顺序访问样本来累积梯度统计量。
## Approximate Algorithm

![这里写图片描述](https://img-blog.csdn.net/20170928133559066?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

当数据不能完全放入内存时，精确贪婪算法很难高效地实现。在分布式设置中也有同样的问题。 

近似算法首先根据特征分布的分为点提出一些候选划分点。 

然后将连续特征映射到这些候选点划分成的区间（桶）内，聚合统计量。根据聚合统计量寻找最佳划分。 

根据提议给出的时间，算法有两种变型。 

全局变型在树构造的初始阶段提出所有的候选划分，之后在所有阶段使用相同的划分。 

局部变型在每次划分时重新提出划分。 

全局划分需要更少的提议步骤。但是，通常更多比局部划分需要更多的候选点，因为候选点在每次划分后没有被更新。 

通过实验，局部提议事实上需要更少的候选点，而全局提议当给出足够多的候选点的时准确率也可以接近局部提议。 

分位数策略得益于分布式和重计算。 
![这里写图片描述](https://img-blog.csdn.net/20170928135254033?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## Weighted Quantile Sketch

![这里写图片描述](https://img-blog.csdn.net/20170928135323475?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**还需要细看**
## Sparsity-aware Split Finding

导致稀疏性的原因 

1. 缺失值 

2. 频繁的0值统计量 

3. 人工特征工程，如onehot 
![这里写图片描述](https://img-blog.csdn.net/20170928135649533?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

该算法的关键提示在于只访问非缺失元素。枚举将特征缺失样本划分到右边和左边，比较哪种情况下得分更高。
# 系统设计

![这里写图片描述](https://img-blog.csdn.net/20170928141527704?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## Column Block for Parallel Learning

树学习最耗时的部分是将数据转换成排序的顺序。为了降低排序的代价，提出了将数据放进内存单元，称之为**block**。每个块中以压缩列(CSC)形式排序。这种布局只需要在训练前计算一次，之后的迭代中都可以使用。 
![这里写图片描述](https://img-blog.csdn.net/20170928141325976?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## Cache-aware Acces

block结构帮助优化了特征划分的的计算时间复杂度。新的算法通过行索引间接的获取梯度统计量，因为这些值以特征的顺序被访问。这是一种非连续的内存访问。 
**还需细看**

块大小为$2^{16}$样本平衡了闪存性能和并行化。
## Bolcks for Out-of-core Computation

将数据划分成多个block然后在硬盘上存储block。在计算时，使用独立线程预取block到贮存缓冲区非常重要，这样计算就可以和硬盘读并行。 

通过两项技术提升核外计算
- 块压缩Block Compression 
- 使用一个通用的压缩算法来压缩特征值**细看**
- 对于行索引，减去块开头的索引，使用16位整数存储每一项偏置。这需要每一块存储$2^{16}$样本，已经被证实了是一个较好的设置。
- 在大部分测试数据集中，达到了26%-29%的压缩率。

- 块分片 Block Sharding 

第二项技术是将数据以一种交替的方式分片到多个硬盘。每个硬盘上分配一个预取线程来将数据读入主存缓冲区。训练线程交替从每一个缓冲区中读取数据。 

当多块硬盘可用时，这提升了硬盘读取的吞吐量。
参考资料

> 
XGBoost: A Scalable Tree Boosting System Tianqi Chen 

  GBDT算法原理与系统设计简介 weapon 
[集成学习总结](https://xijunlee.github.io/2017/06/03/%E9%9B%86%E6%88%90%E5%AD%A6%E4%B9%A0%E6%80%BB%E7%BB%93/)















