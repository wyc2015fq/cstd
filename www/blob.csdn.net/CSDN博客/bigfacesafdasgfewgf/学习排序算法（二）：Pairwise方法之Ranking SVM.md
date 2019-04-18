# 学习排序算法（二）：Pairwise方法之Ranking  SVM - bigfacesafdasgfewgf - CSDN博客





2014年12月24日 15:08:15[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：10198








**学习排序算法（二）：Pairwise方法之Ranking  SVM**



# 1. Pairwise方法的基本思想




    Pairwise考虑了文档顺序的关系。它将同一个query的相关文档其中起来，把任意两个文档组成一个pair。我们研究就是以这个pair文档对来研究的。




# 2. Ranking SVM




![](https://img-blog.csdn.net/20141224151116524?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








    在测试的时候，我们就把一个query的所有结果先构建所有可能的pair，然后输入到学习到的模型中，得到每个pair的相对顺序。根据推算就可以得到所有搜索结果的排序了。



    典型的Pairwise方法就有ranking SVM。微软大牛刘铁岩的一张slide中讲到了ranking SVM，如下图所示：



![](https://img-blog.csdn.net/20141224151222356?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








     除了Ranking SVM方法之外，Pairwise的方法还有RankNet, LambdaRank等，他们将在下面的博客中介绍。









