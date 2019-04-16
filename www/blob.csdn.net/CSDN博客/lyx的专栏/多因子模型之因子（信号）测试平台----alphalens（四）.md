# 多因子模型之因子（信号）测试平台----alphalens（四） - lyx的专栏 - CSDN博客





2017年09月05日 21:37:01[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：2132








广告：本人的单因子测试视频教程 [https://study.163.com/course/introduction/1005568012.htm](https://study.163.com/course/introduction/1005568012.htm)



接下来，还剩下最后一部分的单因子分析，就是换手率的分析。当然，我们要知道，这仅仅是三个主要部分，后续我们可能会增加几个观察的指标。

#         1.换手率概括表

![](https://img-blog.csdn.net/20170905213258208?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        上图是turnover的分析表。由于实际交易中，我们是有手续费的，所以，对于因子而已，我们不希望因子打分很不稳定。换句话说，如果一个因子，今天给一个股票打了很高的分，明天就打很低的分。这样造成的结果就是，我们对这个股票，一会儿卖，一会儿买，从而造成很高的手续费。所以，因子的换手率分析也是很重要的一个部分。

        还有一个角度可以查看因子造成的换手率高低，就是因子的自相关性。如果因子的自相关性低，那么就一会儿高，一会儿低，往往会造成很大的换手率。而因子的自相关性高，就不会有这样的问题。

# 2.Top，Buttom换手率时间序列

![](https://img-blog.csdn.net/20170905213517137?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        然后是对换手率这一指标的展开，上图就是top和bottom层的换手率的时间序列。实际中，我们其实是对top和bottom两个层比较关心，所以有时候仅仅看这两个层也是足够了的。

# 3.rank 的自回归时间序列

![](https://img-blog.csdn.net/20170905213614111?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



        然后是自相关性的时间序列。







