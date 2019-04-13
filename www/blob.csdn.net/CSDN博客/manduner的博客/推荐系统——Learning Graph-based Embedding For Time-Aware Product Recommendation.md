
# 推荐系统——Learning Graph-based Embedding For Time-Aware Product Recommendation - manduner的博客 - CSDN博客


2018年07月17日 16:19:03[Manduner_TJU](https://me.csdn.net/manduner)阅读数：230


# 论文描述：
目前，各种推荐系统的方法被提出，最初是基于人口统计学，内容和协同过滤。尽管各种方法在一些领域具有高效性，但是它们通常在较小的数据集上表现良好，当数据的规模不断增加时，问题就变得很有挑战性了。针对大规模数据集，几种基于网络嵌入的推荐算法已经被提出，然而它们不能高效的表示用户的动态偏好。
本文提出了一种新的产品图嵌入模型(PGE)，利用网络表示学习技术来研究时间感知的产品推荐。本文的模型通过将历史购买记录转换为产品图来捕获产品的顺序影响。然后利用网络嵌入模型将产品转化为低维向量。一旦产品被映射到潜在空间中，本文提出了一种计算用户最新偏好的新方法，它将用户投射到与产品相同的潜在空间中。一旦获得产品嵌入，采用时间衰减函数来跟踪用户偏好的动态。最终的产品推荐是基于产品的嵌入以及在相同的潜在空间中的用户偏好。

# 参考文献：
*Li Y, Chen W, Yan H. Learning Graph-based Embedding For Time-Aware Product Recommendation[C]//Proceedings of the 2017 ACM on Conference on Information and Knowledge Management. ACM, 2017: 2163-2166.*


