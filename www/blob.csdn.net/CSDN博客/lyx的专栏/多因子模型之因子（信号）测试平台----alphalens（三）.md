# 多因子模型之因子（信号）测试平台----alphalens（三） - lyx的专栏 - CSDN博客





2017年09月02日 14:09:16[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：2252








广告：本人的单因子测试视频教程 [https://study.163.com/course/introduction/1005568012.htm](https://study.163.com/course/introduction/1005568012.htm)



今天，我们讲alphalens下一个重要的因子测试的输出部分，Information Analysis，也就是，信息的分析。听起来有点抽象。那么，我们开始吧。

        首先，讲一个主动投资组合管理的第一个定理，叫做fundmental law，其形式就是：IR = IC * Breadth**0.5

我们知道，IR就是information ratio，是一个衡量基金经理对信息处理、分析、使用能力的指标。当然了，本质上，这也是一个风险调整后的评价指标。

        这个fundmental law把IR拆成了两个部分。

        西方的思维就是这样，想要理解一个东西，就把他拆开来，拆成相乘或者相加，然后各个分析。IR被拆成了IC和breadth的平方根。IC就是information coefficient，说白了，就是一个相关性，计算方法也是相关性。衡量的就是指标的预测能力。譬如我们的调仓周期是5天，那么，我们在今天计算一个因子，譬如就是之前的revs10，然后我们去做预测。怎么预测呢？很简单，首先明确，我们的是反向signal，那么如果我们的signal很有预测能力，那么今天打分低的五天之后的收益率就会比打分高的收益率高。那么市场上有那么多股票，每个股票都有一个分值。然后我们可以计算接下来五天每个股票五天的收益率。这样，我们就有两个向量了，一个是站在t时刻给每个股票打分的向量，一个是t+5天每个股票的收益率，也就是t到t+5天五天间的收益率。接下来，我们对这两个向量做一个相关性计算，这个计算出来的相关性，就是IC。就是这么简单，但是其后面完美的数学推导，其实还是有点复杂的，后续有机会给大家推导一下。

        还要提到一点是计算相关性的方法。我们知道，计算相关性其实有很多方法，最常用的就是pearson和spearman。Pearson就是我们一般学到的相关性的计算方法，也是线性相关性，spearman则是另外一种相关性计算方法，其特点是只考虑两个向量中排序是否一致，这样就能很好的避免异常值的影响。由于是考虑排序是否一样，所以用spearman计算出来的IC也叫作RankIC。

        还有一个成分，breadth，其实就是你对市场做出预测的次数。我们考虑一个很简单的场景，如果你有一个指标，可以预测一个股票的涨和跌，但是，准确率只有51%。假设没有手续费，那么我们要买很多次才能体现出这个指标的盈利性，因为这个指标的正确率很低。而这里的准确率就是预测能力。所以，如果你想获得高的IR，要么，找到好的signal，有很高的预测能力，也就是IC很高；或者，找一个不那么高IC的signal，但是不断的买卖。而我们往往会选择前者，因为fundmental law成立是有很多假设的，比如每次买卖都是独立的，而且，没有手续费。

        好了，说了这么多，其实就是说，你的signal打分一定要真的有用。

# 1.information table

![](https://img-blog.csdn.net/20170902140324535?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        首先是上面这个information coefficient分析的表。这里给出了每个调仓周期下，IC的均值、方差以及t统计量，还有偏度、峰度以及年化的IR。

        当然啦，我们固然希望ic的均值高，方差小，t统计量大，也就是p-value小。至于偏度和峰度主要考察和正态分布的偏离程度，从而分析小概率事件的影响。

# 2.IC的时间序列

![](https://img-blog.csdn.net/20170902140428313?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        接下来，我们看到的是三个调仓周期下，ic的时间序列，可想而知，好的信号我们希望他的ic比较平稳，而且不能只在零附近绕圈子，绝大部分大于零和小于零就是一个好的signal的表现。毕竟，如果小于零，我们作为负向指标就可以了。

# 3.IC的分布图与Q-Q-plot

![](https://img-blog.csdn.net/20170902140538653?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        这个就更加好理解了。虽然有些signal的ic可能均值挺大的，但是嫉妒肥尾巴或者均值挺大，但是均值大是由于极度的尖峰和右偏造成的。这样的情况，其实都不好，所以，我们需要具体来看一下ic的分布来决定一个signal的预测能力是不是可靠。

        右边的q-q plot就是用来和正态分布进行比较的。

# 4.IC的hot-map

![](https://img-blog.csdn.net/20170902140633864?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        然后是每个调仓周期下，ic值的hotmap，也就是热力图。这样我们就能一下子看出，那些时间段ic值比较好，那些时间段ic比较差，有没有什么突发的情况。然后可以对此进行情景分析。

        热力图最大的作用，就是方便我们进行因子的情景分析。

        Information Analysis中，alphalens提供给我们的信息大概就是这些，但是其实由于Information真的很重要，所以我们往往会自己加一些别的图来观察signal是否足够优秀，譬如ic cumulative plot等等。这是后话了。



