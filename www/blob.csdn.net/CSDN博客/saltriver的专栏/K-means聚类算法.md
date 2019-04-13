
# K-means聚类算法 - saltriver的专栏 - CSDN博客


2017年07月24日 21:37:30[saltriver](https://me.csdn.net/saltriver)阅读数：1625标签：[非监督学习																](https://so.csdn.net/so/search/s.do?q=非监督学习&t=blog)[K-means算法																](https://so.csdn.net/so/search/s.do?q=K-means算法&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=非监督学习&t=blog)个人分类：[机器学习																](https://blog.csdn.net/saltriver/article/category/6399667)



K-means算法是最简单的非监督学习算法，主要用于解决已知数据的聚类问题。其基本过程如下：
（1）预先指定数值k；将数据集分成k个簇。
（2）随机确定k个中心位置点。
（3）计算各个数据点与k个中心点的距离。
（4）根据距离排序将该数据点分配给最邻近的中心，形成k个簇。
（5）计算每个簇的平均位置，作为新的中心点。
（6）重复步骤3-5直到簇不发生变化或达到最大迭代次数。
**一、算法过程**
下面结合图例给出其算法过程：
设平面上有如下的数据集（共有9个数据），需要将其进行聚类。
![这里写图片描述](https://img-blog.csdn.net/20170724212922301?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170724212922301?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（1）指定k=3，将数据集聚成3类。
（2）随机生成k=3个中心点。
![这里写图片描述](https://img-blog.csdn.net/20170724213009182?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170724213009182?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（3）计算这9个数据点分别到3个中心点的距离。
（4）对这些距离进行排序，将9个数据点按照距离最近原则将其聚类到相应的中心点。
![这里写图片描述](https://img-blog.csdn.net/20170724213047611?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170724213047611?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如图所示，本次聚类：1,2,3归为了一类；4,5归为了一类；6,7,8,9归为了一类。
（5）计算每一类中的平均位置。例如数据点4,5作为一类，其坐标分别为（20,20），（22,21），则平均位置为（21,20.5），将这个平均位置作为新的中心位置。
![这里写图片描述](https://img-blog.csdn.net/20170724213155386?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170724213155386?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（6）再次计算这9个数据点分别到3个新中心点的距离。
（7）对这些距离进行排序，将9个数据点按照距离最近原则将其聚类到相应的新中心点。
![这里写图片描述](https://img-blog.csdn.net/20170724213232639?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170724213232639?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如图所示，本次聚类：1,2,3归为了一类；4,5,6归为了一类；7,8,9归为了一类。
（8）再次计算每一类中的平均位置，将平均位置又作为新的中心点位置。
![这里写图片描述](https://img-blog.csdn.net/20170724213356601?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170724213356601?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
生成的新中心点如图所示。
（9）再次计算这9个数据点分别到3个新中心点的距离。
（10）对这些距离进行排序，将9个数据点按照距离最近原则将其聚类到相应的新中心点。
本次聚类：1,2,3归为了一类；4,5,6归为了一类；7,8,9归为了一类。与上次的聚类没有发生变化，算法过程结束退出。
**二、时间复杂度**
从算法过程中可以看到，需要求n个数据点和k个中心的距离，外面有多次迭代，迭代次数为m。则k-means算法的时间复杂度为：
O(mnkd)，m与数据集本身的分布情况和初始中心点位置有关。n为数据集中数据样本数量，k为聚类个数，d为数据的维数。
**三、优缺点**
优点：
算法速度快；
原理简单，
易于理解；
聚类效果好。
缺点：
- 需要先确定k的值。事实上我们一般不知道数据集的分布情况。
- 对非线性数据集效果不好或无能为力。如下图的数据集。
![这里写图片描述](https://img-blog.csdn.net/20170724213535783?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

