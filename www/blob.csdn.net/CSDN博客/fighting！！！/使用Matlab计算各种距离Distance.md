# 使用Matlab计算各种距离Distance - fighting！！！ - CSDN博客
2018年11月02日 20:29:28[dujiahei](https://me.csdn.net/dujiahei)阅读数：1860
转自：[http://jacoxu.com/%E4%BD%BF%E7%94%A8matlab%E8%AE%A1%E7%AE%97%E5%90%84%E7%A7%8D%E8%B7%9D%E7%A6%BBdistance/](http://jacoxu.com/%E4%BD%BF%E7%94%A8matlab%E8%AE%A1%E7%AE%97%E5%90%84%E7%A7%8D%E8%B7%9D%E7%A6%BBdistance/)
计算距离的需求有两种：
　　一种是给定一个特征集合X，然后计算Pairwise距离矩阵，那么可使用D=pdist(X,distance)的方式；
　　另一种是给定两个对应的特征集合X和Y，然后计算X与Y对应的距离信息，使用D=pdist2(X,Y,distance)的方式；
需注意，2011版本以前的Matlab是没有pdist2.m文件的，而早期的pdist2.m文件中的距离计算方式也比较少，所以建议使用最新的Matlab版本，很重要。
其中，distance的定义有如下几种：
**欧几里德距离Euclidean distance(‘euclidean’)**
欧氏距离虽然很有用，但也有明显的缺点。
一：它将样品的不同属性（即各指标或各变量）之间的差别等同看待，这一点有时不能满足实际要求。
二：它没有考虑各变量的数量级(量纲)，容易犯大数吃小数的毛病。所以，可以先对原始数据进行规范化处理再进行距离计算。
**标准欧几里德距离Standardized Euclidean distance(‘seuclidean’)**
相比单纯的欧氏距离，标准欧氏距离能够有效的解决上述缺点。注意，这里的V在许多Matlab函数中是可以自己设定的，不一定非得取标准差，可以依据各变量的重要程度设置不同的值，如knnsearch函数中的Scale属性。
**马哈拉诺比斯距离Mahalanobis distance(‘mahalanobis’)**
where C is the covariance matrix.
马氏距离是由印度统计学家马哈拉诺比斯(P. C. Mahalanobis)提出的，表示数据的协方差距离。它是一种有效的计算两个未知样本集的相似度的方法。与欧式距离不同的是它考虑到各种特性之间的联系（例如：一条关于身高的信息会带来一条关于体重的信息，因为两者是有关联的）并且是尺度无关的(scale-invariant)，即独立于测量尺度。
如果协方差矩阵为单位矩阵,那么马氏距离就简化为欧式距离,如果协方差矩阵为对角阵,则其也可称为正规化的欧氏距离.
马氏优缺点：
　　1）马氏距离的计算是建立在总体样本的基础上的，因为C是由总样本计算而来，所以马氏距离的计算是不稳定的；
　　2）在计算马氏距离过程中，要求总体样本数大于样本的维数。
　　3）协方差矩阵的逆矩阵可能不存在。
**曼哈顿距离(城市区块距离)City block metric(‘cityblock’)**
Notice that the city block distance is a special case of the Minkowski metric, where p=1.
**闵可夫斯基距离Minkowski metric(‘minkowski’)**
Notice that for the special case of p = 1, the Minkowski metric gives the city block metric, for the special case of p = 2, the Minkowski metric gives the Euclidean distance, and for the special case of p = ∞, the Minkowski metric gives the Chebychev distance.
闵可夫斯基距离由于是欧氏距离的推广，所以其缺点与欧氏距离大致相同。
**切比雪夫距离Chebychev distance(‘chebychev’)**
Notice that the Chebychev distance is a special case of the Minkowski metric, where p = ∞.
**夹角余弦距离Cosine distance(‘cosine’)**
与Jaccard距离相比，Cosine距离不仅忽略0-0匹配，而且能够处理非二元向量，即考虑到变量值的大小。
**相关距离Correlation distance(‘correlation’)**
Correlation距离主要用来度量两个向量的线性相关程度。
**汉明距离Hamming distance(‘hamming’)**
两个向量之间的汉明距离的定义为两个向量不同的变量个数所占变量总数的百分比。
**杰卡德距离Jaccard distance(‘jaccard’)**
Jaccard距离常用来处理仅包含非对称的二元(0-1)属性的对象。很显然，Jaccard距离不关心0-0匹配，而Hamming距离关心0-0匹配。
Spearman distance(‘spearman’)
【参考博文】
1. http://cn.mathworks.com/help/stats/pdist2.html
2.http://blog.sciencenet.cn/blog-531885-589056.html
