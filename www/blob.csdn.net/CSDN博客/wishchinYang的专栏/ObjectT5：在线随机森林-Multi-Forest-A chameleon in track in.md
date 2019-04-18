# ObjectT5：在线随机森林-Multi-Forest-A chameleon in track in - wishchinYang的专栏 - CSDN博客
2014年07月16日 17:52:40[wishchin](https://me.csdn.net/wishchin)阅读数：809
个人分类：[目标追踪																[ReinforceLearning](https://blog.csdn.net/wishchin/article/category/5783969)](https://blog.csdn.net/wishchin/article/category/1508415)
原文：：**Multi-Forest：A chameleon in tracking**，CVPR2014  下的蛋...原文
使用随机森林的优势，在于可以使用GPU把每棵树分到一个流处理器里运行，容易并行化实现。
OpenCV中的使用：[OpenCV：使用 随机森林](http://blog.csdn.net/wishchin/article/details/46519275)与GBDT
随机森林的优点：[http://www.cnblogs.com/wentingtu/archive/2011/12/13/2286212.html](http://www.cnblogs.com/wentingtu/archive/2011/12/13/2286212.html)
- 在数据集上表现良好
- 在当前的很多数据集上，相对其他算法有着很大的优势
- 它能够处理很高维度（feature很多）的数据，并且不用做特征选择
- 在训练完后，它能够给出哪些feature比较重要
- 在创建随机森林的时候，对generlization error使用的是无偏估计
- 训练速度快
- 在训练过程中，能够检测到feature间的互相影响
- 容易做成并行化方法
- 实现比较简单
（1）:在线Boost
使用在线Boosting项目主页：[http://www.vision.ee.ethz.ch/boostingTrackers/index.htm](http://www.vision.ee.ethz.ch/boostingTrackers/index.htm)
使用在线boosting学习的Code：[http://www.vision.ee.ethz.ch/boostingTrackers/download.htm](http://www.vision.ee.ethz.ch/boostingTrackers/download.htm)
使用Haar特征树作为弱分类器，取得不错的效果。但是速度稍慢。
（2）:在线随机森林：
09年，ICCV 上的文章，效果和离线的[random
 forest](http://www.cvchina.info/tag/online-random-forest/)差不多，特别的牛。可以做分类，也可以做预测....
程序下载：[http://www.everbox.com/f/7igWhZFJBp7xxqmbaM6UyewatN](http://www.everbox.com/f/7igWhZFJBp7xxqmbaM6UyewatN)
这里介绍的主要是在线随机决策树，其思想主要是：每棵树可以在线分裂。每个叶子分裂的条件是预测的数量要达到一定的值和每个叶子节点信息。
每个树的生长主要通过预测的样本（在线接受的样本），每棵树的叶子节点分裂主要根据该节点的熵或Gini
![](http://pic002.cnblogs.com/images/2012/358029/2012051018382520.png)  or![](http://pic002.cnblogs.com/images/2012/358029/2012051018372176.png)
学过决策树和信息论的，对这个概念都有了解。其中j表示第j棵树，i表示第i个分类结果。K表示总的分类数。
对有一个给定的结合S（在线预测中给定），每棵树上叶子节点Pj的的概率可以表示为：
![](http://pic002.cnblogs.com/images/2012/358029/2012051018481182.png)
如果要在Pj叶子节点分类，那么，得到二个叶子节点的概率可以用下式表示：
![](http://pic002.cnblogs.com/images/2012/358029/2012051018513330.png) 
            and             ![](http://pic002.cnblogs.com/images/2012/358029/2012051018512124.png)
解释一下 Pjls，l为left，s为测试集合。所以Pjls表示为在集合S中Pj叶子节点的分列的左节点。同理，Pjrs表示为在集合S中Pj叶子节点的分列的右节点。
那么，每棵树上叶子节点Pj分裂必须符合以下二个条件：
        1. 落在叶子节点Pj的个数必须大于一个常数（可以人工设定）
        2. 叶子节点的Gini必须大于一个常数（可以人工设定），Gini计算公式如下：
![](http://pic002.cnblogs.com/images/2012/358029/2012051019023117.png)
        以上步骤就完成整个树的更新。
步骤3. 用个possion分布确定从采样的次数，其原理见online boosting： [http://www.cnblogs.com/liqizhou/archive/2012/05/10/2494145.html](http://www.cnblogs.com/liqizhou/archive/2012/05/10/2494145.html)
步骤6. u代表分类的类别。
步骤7. j代表第t棵树上叶子节点。
步骤8. 统计第j个叶子节点的数目和计算Gini
步骤9. 判断条件是否分裂的二个条件。
步骤10. 在符合条件的叶子节点中，选择一个Gini最大的叶子节点作为分类节点。
以上就是online Random forests 的主要思想
下面给出了在线随机森林算法的流程：
![](http://pic002.cnblogs.com/images/2012/358029/2012051017221779.png)

待写....
