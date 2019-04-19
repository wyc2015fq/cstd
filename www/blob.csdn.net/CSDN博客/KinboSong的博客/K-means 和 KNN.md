# K-means 和 KNN - KinboSong的博客 - CSDN博客
2017年03月14日 20:58:03[songjinbo3](https://me.csdn.net/KinboSong)阅读数：381
具体可以参考博客：[http://blog.csdn.net/loadstar_kun/article/details/39450615](http://blog.csdn.net/loadstar_kun/article/details/39450615)
K-means的时间复杂度：O(tKmn)，其中，t为迭代次数，K为簇的数目，m为记录数，n为维数
K-means的空间复杂度：O((m+K)n)，其中，K为簇的数目，m为记录数，n为维数
K-means算法是**局部**收敛的
证明：若K-means是全局收敛的，那么选取不同的初始聚类中心一定会收敛到同一个结果；as we all know，选取不同的初始聚类中心，收敛的结果不一样；通过反证法，所以K-means不是全局收敛的。又因为K-means是收敛的，所以K-means是局部收敛的。
![](https://img-blog.csdn.net/20170314205748678?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2luYm9Tb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
