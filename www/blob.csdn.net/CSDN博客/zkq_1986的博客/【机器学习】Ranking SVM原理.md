# 【机器学习】Ranking SVM原理 - zkq_1986的博客 - CSDN博客





2019年01月09日 20:03:00[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：74








假定有x1,x2,x3三个点，查询q，对应的查询排序结果为x1,x2,x3。那么可获得新的点，即x1-x2,x1-x3, x2-x3, x2-x1,x3-x1,x3-x2，它们的标签分别为1,1,1，-1,-1,-1。

有了数据点，以及标签，就领用传统的SVM，训练出超平面。

在测试的时候，我们就把一个query的所有结果先构建所有可能的pair，然后输入到学习到的模型中，得到每个pair的相对顺序。根据推算就可以得到所有搜索结果的排序了。

![](https://images2015.cnblogs.com/blog/995611/201704/995611-20170409102328332-158330266.png)![](https://images2015.cnblogs.com/blog/995611/201704/995611-20170409102533222-1585791487.png)



![](https://img-blog.csdn.net/20141224151222356?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

# 缺点

1. 没考虑查询与文档的关系



