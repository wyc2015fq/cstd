# 【关联规则】Apriori详解 - zkq_1986的博客 - CSDN博客





2018年01月03日 15:51:21[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：861








# 1.数据挖掘与关联分析


数据挖掘是一个比较庞大的领域，它包括数据预处理（清洗去噪）、数据仓库、分类聚类、关联分析等。关联分析可以算是数据挖掘最贴近我们生活的一部分了，打开卓越亚马逊，当挑选一本《Android4高级编程》时，它会不失时机的列出你可能还会感兴趣的书籍，比如Android游戏开发、Cocos2d-x引擎等，让你的购物车又丰富了些，而钱包又空了些。


关联分析，即从一个数据集中发现项之间的隐藏关系。本篇文章Apriori算法主要是基于频繁集的关联分析，所以本文中所出现的关联分析默认都是指基于频繁集的关联分析。


有了一个感性的认识，我们来一段理性的形式化描述：


令项集I={i1,i2,...in}


且有一个数据集合D，它其中的每一条记录T，都是I的子集


那么关联规则都是形如A->B的表达式，A、B均为I的子集，且A与B的交集为空


这条关联规则的支持度：support = P(A并B)


这条关联规则的置信度：confidence = support(A并B）/suport(A)


如果存在一条关联规则，它的支持度和置信度都大于预先定义好的最小支持度与置信度，我们就称它为强关联规则。强关联规则就可以用来了解项之间的隐藏关系。所以关联分析的主要目的就是为了寻找强关联规则，而Apriori算法则主要用来帮助寻找强关联规则。

注意：因为频率=事件出现次数/总事件次数，为了方便，我们在以下都用事件出现的频数而非频率来作为支持度。


# 2.Apriori算法描述


Apriori算法指导我们，如果要发现强关联规则，就必须先找到频繁集。所谓频繁集，即支持度大于最小支持度的项集。如何得到数据集合D中的所有频繁集呢？


有一个非常土的办法，就是对于数据集D，遍历它的每一条记录T，得到T的所有子集，然后计算每一个子集的支持度，最后的结果再与最小支持度比较。且不论这个数据集D中有多少条记录（十万？百万？），就说每一条记录T的子集个数（{1,2,3}的子集有{1}，{2}，{3}，{1,2}，{2,3}，{1,3}，{1,2,3}，即如果记录T中含有n项，那么它的子集个数是2^n-1）。计算量非常巨大，自然是不可取的。


所以Aprior算法提出了一个逐层搜索的方法，如何逐层搜索呢？包含两个步骤：


1.自连接获取候选集。第一轮的候选集就是数据集D中的项，而其他轮次的候选集则是由前一轮次频繁集自连接得到（频繁集由候选集剪枝得到）。


2.对于候选集进行剪枝。如何剪枝呢？候选集的每一条记录T，如果它的支持度小于最小支持度，那么就会被剪掉；此外，如果一条记录T，它的子集有不是频繁集的，也会被剪掉。


算法的终止条件是，如果自连接得到的已经不再是频繁集，那么取最后一次得到的频繁集作为结果。

需要值得注意的是：

Apriori算法为了进一步缩小需要计算支持度的候选集大小，减小计算量，所以在取得候选集时就进行了它的子集必须也是频繁集的判断。（参见《数据挖掘：概念与技术》一书）。





例：




考虑下面的频繁3-项集的集合：{1,2,3},{1,2,4},{1,2,5},{1,3,4},{1,3,5},{2,3,4},{2,3,5},{3,4,5}假定数据集中只有5个项,采用合并策略,由候选产生过程得到4-项集不包含( C )


A.1,2,3,4 B.1,2,3,5


C.1,2,4,5 D.1,3,4,5
根据数据挖掘Apriori算法的性质之一：判定是否可作为K项频繁集是通过K项集分裂为K个[K-1](https://www.baidu.com/s?wd=K-1&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y3nWPbnyNWPjbYnj6vuj030ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPj63njTznW63)项集，考察[K-1](https://www.baidu.com/s?wd=K-1&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y3nWPbnyNWPjbYnj6vuj030ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPj63njTznW63)项集是否为L[k-1](https://www.baidu.com/s?wd=k-1&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y3nWPbnyNWPjbYnj6vuj030ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPj63njTznW63)，要生成4-项集，｛1，2，4，5｝分裂后为｛1，2，4｝｛2，4，5｝｛1，2，5｝｛1，4，5｝其中，｛1，4，5｝不属于频繁3项集，所以｛1，2，4，5｝不能作为4项集，因为有性质为：任何非频繁的K-1项集都不可能是频繁项集K项集的子集。
A、B、D你分别可以试一下，分裂后的子集是否为频繁三项集。
例如：｛1.2.3.4｝分裂后：｛1，2，4｝｛2，3，4｝｛1，3，4｝｛1，2，3｝均满足频繁三项集里的子集。




# 3.Apriori算法例子推导


上面的描述是不是有点抽象，例子是最能帮助理解的良方。（假设最小支持度为2，最小置信度为0.6，最小支持度和置信度都是人定的，可以根据实验结果的优劣对这两个参数进行调整）


假设初始的数据集D如下：

![](https://img-blog.csdn.net/20141025224058200?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcm9uZ3lvbmdmZWlrYWky/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


  那么第一轮候选集和剪枝的结果为：

![](https://img-blog.csdn.net/20141025224255528?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcm9uZ3lvbmdmZWlrYWky/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


可以看到，第一轮时，其实就是用的数据集中的项。而因为最小支持度是2的缘故，所以没有被剪枝的，所以得到的频繁集就与候选集相同。


第二轮的候选集与剪枝结果为：

![](https://img-blog.csdn.net/20141028090227125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcm9uZ3lvbmdmZWlrYWky/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



可以看到，第二轮的候选集就是第一轮的频繁集自连接得到的（进行了去重），然后根据数据集D计算得到支持度，与最小支持度比较，过滤了一些记录。频繁集已经与候选集不同了。


第三轮候选集与频繁集结果为：

![](https://img-blog.csdn.net/20141028090322093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcm9uZ3lvbmdmZWlrYWky/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



可以看到，第三轮的候选集发生了明显的缩小，这是为什么呢？


请注意取候选集的两个条件：


1.两个K项集能够连接的两个条件是，它们有K-1项是相同的。所以，（I2，I4）和（I3，I5）这种是不能够进行连接的。缩小了候选集。


2.如果一个项集是频繁集，那么它不存在不是子集的频繁集。比如（I1，I2）和（I1，I4）得到（I1，I2，I4），而（I1，I2，I4）存在子集（I1，I4）不是频繁集。缩小了候选集。


第三轮得到的2个候选集，正好支持度等于最小支持度。所以，都算入频繁集。



这时再看第四轮的候选集与频繁集结果：

![](https://img-blog.csdn.net/20141028090822562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcm9uZ3lvbmdmZWlrYWky/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


可以看到，候选集和频繁集居然为空了！因为通过第三轮得到的频繁集自连接得到{I1，I2，I3，I5}，它拥有子集{I2,I3,I5}，而{I2,I3,I5}不是频繁集，不满足：频繁集的子集也是频繁集这一条件，所以被剪枝剪掉了。所以整个算法终止，取最后一次计算得到的频繁集作为最终的频繁集结果：


也就是：['I1,I2,I3', 'I1,I2,I5']


那么，如何得到强规则呢？


比如对于：I1，I2，I3这个频繁集，我们可以得到它的子集：{I1},{I2},{I3},{I1，I2},{I1，I3},{I2，I3}，那么可以得到的规则如下：


I1->I3^I2 0.333333333333

I2->I1^I3 0.285714285714

I3->I1^I2 0.333333333333

I1^I2->I3 0.5

I1^I3->I2 0.5

I2^I3->I1 0.5

左边是规则，右边是置信度。conf(I1->I3^I2) = support(I1,I2,I3)/support(I1)


与最小置信度相比较，我们就可以得到强规则了。


所以对于频繁集['I1,I2,I3', 'I1,I2,I5']


我们得到的规则为：


I1->I3^I2 0.333333333333

I2->I1^I3 0.285714285714

I3->I1^I2 0.333333333333

I1^I2->I3 0.5

I1^I3->I2 0.5

I2^I3->I1 0.5

I1->I2^I5 0.333333333333

I2->I1^I5 0.285714285714

I5->I1^I2 1.0

I1^I2->I5 0.5


I1^I5->I2 1.0

 I2^I5->I1 1.0


从而得到强规则为：


I5->I1^I2 : 1.0

I1^I5->I2 : 1.0

I2^I5->I1 : 1.0


意味着如果用户买了商品I5，则极有可能买商品I1，I2;买了I1和I5，则极有可能买I2，如果买了I2，I5，则极有可能买I1。所以，你就知道在页面上该如何推荐了





转载自：http://blog.csdn.net/rongyongfeikai2/article/details/40457827



