# 计算机视觉中的——图像集分类（image set classification） - 家家的专栏 - CSDN博客





2012年05月17日 11:25:52[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：2965标签：[classification																[image																[orthogonal																[reference																[youtube																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[图像处理算法](https://blog.csdn.net/yihaizhiyan/article/category/715109)





针对集（set）的度量：Sparse approximated nearest point distance.

**基于视频的分类**（video-based classification）：

         例如：dynamic texture （IJCV03）

         关注于图像之间的时间关联性 （focus on exploiting the temporal relationship between the images）

**基于图像集的分类**（image set based classification）:

       关注于视角、光照变化、非刚体变形，而不是时间关联性。



图像集分类技术，基于两个标准： 一是图像集如何表示，二是度量。

**图像表示**：分为参数和非参数的表示方法。

                     基于参数模型的表示方法，是利用一些参数分布来表示图像集~  但是如果图像集中没有很好的统计关联性，那么估计的模型就不能很好的进行图像集的分类。

                                                                    两分布度量：如KLD.

                     基于非参数的表示方法，试图表示一个图像集为一个线性子空间（比如：Grassmannian manifold）,混合子空间，或者非线性流行。

                                                                 第一种度量是分析样本数据：样本均值差；测地线；

                                                                  第二种度量是分析模型结构，Canonical corelation analysis (CCA); Mutual subspave method (MSM); orthogonal subspace mehtod..



现试图提出一种新的度量，同时利用结构信息和样本信息。

第c个图像集的表示，有N个图像组成一个特征矩阵。

![](https://img-my.csdn.net/uploads/201205/17/1337223792_9788.png)

model一个图像集为an affine hull of the set data: （ 带权重的sum）

![](https://img-my.csdn.net/uploads/201205/17/1337223799_1258.png)

或者这种Affine hull 的另一种表示形式

![](https://img-my.csdn.net/uploads/201205/17/1337224069_9181.png)      其中![](https://img-my.csdn.net/uploads/201205/17/1337224012_6385.png)



由于现有的方法，直接搜索最近邻的点，而不加任何额外约束条件。噪声和奇异点的干扰~

先提出sparse approximated nearest points进行度量。





使用的数据集：Honda；Mobo and Youtube datasets。



Reference: sparse approximated nearest points for image set classification](https://so.csdn.net/so/search/s.do?q=youtube&t=blog)](https://so.csdn.net/so/search/s.do?q=reference&t=blog)](https://so.csdn.net/so/search/s.do?q=orthogonal&t=blog)](https://so.csdn.net/so/search/s.do?q=image&t=blog)](https://so.csdn.net/so/search/s.do?q=classification&t=blog)




