# 多因子模型之因子（信号）测试平台----alphalens（二） - lyx的专栏 - CSDN博客





2017年08月29日 22:05:21[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：3481








广告：本人的单因子测试视频教程 [https://study.163.com/course/introduction/1005568012.htm](https://study.163.com/course/introduction/1005568012.htm)



今天这一个部分，我们要用到另外一个python的大杀器，notebook，现在叫做jupter notebook。大家如果装了anaconda的话就会有这个的。

        大家在命令窗口中把目录切换到工作目录就可以了。

![](https://img-blog.csdn.net/20170829211033641?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        回车之后就可以启动notebook了。

        为什么我们今天要在notebook里面编程，而不是继续在pycharm上面呢？原因就是图片显示的问题。在pycharm里面，我们plot会有很多问题，比例失真，而且图片不能保留，而notebook可是一个可视化很有利的工具。

        当然，接下来在notebook里面的教程只是一个教学的目的，后期，我们会把alphalens分解开来，然后在pycharm里面运行，不显示图片，而是直接把图片保存起来，方便最后文件的形成。最后的文件，可以是网页，也可以是pdf。而现在是用notebook的目的就是让我们明白测试单个因子的时候的一些指标的观察方法。毕竟这一系列的教程，不仅仅是编程的教程，更重要的是学习因子的测试和因子表现的分析。

        一个因子的表现，通常可以从Return， information coefficient，turnover三个方面来考察，当然也有别的，但是这三个是最主要的。而alphalens也正是围绕着这个展开的。

        今天我们先来分析一下return这一部分，alphalens给我们提供了哪些视角，以及含义是什么。

        在notebook里面，我们只需要运行下面这一句，alphalens就会把所有的关于return的分析展示出来：

```python
alphalens.tears.create_returns_tear_sheet(factor_data)
```



# 1.收益率表

![](https://img-blog.csdn.net/20170829213139492?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)











        我们先来分析一下这张表。

        我们有一个因子，给每个股票在每一天都打了分，那么，你得按照这个分数来分层次。比如，我们分5个层次，那么得分前20%的是第一层，即Top Quantile，最后面20%的是Bottom Quantile。然后，每隔特定的时间点，我们对这些层里面的股票进行轮换。所以，这里就产生一个问题，每隔多少时间调仓？

        我们在这个例子当中使用的是1、5、10，也就是按照每天调仓、一周调仓、半个月调仓来进行因子的测试。当然，我们也可以选择更多的调仓周期。

        如此一来，表格就很清楚了，三列分别对应的是三个调仓周期的结果。第一行是按照这样的调仓周期，获得的年化alpha，beta（这里，alphalens中的beta是全市场的平均收益，而这里的全市场是每一个层加起来的平均收益）。后面是每个层的收益。

        具体怎么计算，后续如果分析源码的话，可以接触到，比较alphalens只是一个工具，首先要会使用，然后想着改进提高。毕竟学习的过程就是守、破、离。必须先准守，弄明白了才可以有自己的见解，对工具做提高。

        当然啦，好的因子，对于上面的表格，年化收益率应该比较大，而且top和bottom的收益率spread应该比较显著。

# 2.分层平均收益率柱状图

        接下来是这张图，者可以说是单因子测试最重要的一张图。由于我们的股票标的池比较少，所以作为一个教程，笔者只把我们的因子得分分层了5层，也就是和上面举得例子一样。

![](https://img-blog.csdn.net/20170829213852105?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        上图中，每一组柱状图代表的是一个层，从左往右依次从bottom到top。显然，我们这个因子是一个反向因子，也就是说，因子的值越高，表现越差，实际情况下，我们应该先纠正方向。

        通常，如果我们的标的池很多，那么可以分很多的层，譬如30、50，然后绘制这个图，观察图形的形态。我们希望的是，这些柱状图是单调生长的，这样就说明，因子的单调性很好，分值越高，预测股票的表现越好或者越差。   

        当然，实际上，这样的因子并不多，所以说，寻找alpha是很困难的路，而寻找alpha的前提，就是有一个很好用的单因子回测平台。

# 3.所有收益率分布图

![](https://img-blog.csdn.net/20170829214410172?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        接下来，是小提琴图。这个图其实是这么多收益的分布。我们前面看到的柱状图的高度其实是每个调仓周期内获得的收益的均值。我们知道，均值仅仅是分布的一阶矩，相对而言，只看均值会损失很多信息。所以，这里就把每一层的每一次调仓周期内获得的收益分布绘制出来。

# 4.单信号组合构建收益率图

        从上面我们可以看到，十日调仓的收益明显更加集中，方差更小，换句话说，收益率更加可信。

![](https://img-blog.csdn.net/20170829214749077?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        我们知道，我们的因子值目前来说，是一个正态分布，均值几乎为零。那么我们就可以用因子值作为权重来构建一个全市场的股票组合进行回测。由于我们的是负向指标，所以出现了上面这样向下的曲线，而实际情况下，是向上的。

        这是一种很朴素的构建组合的方法，也就是单因子组合的构建。对于好的信号，当然是收益率越大越好。注意的是，这里并不是一个zero-investment的组合（看alphalens的说明，是这样的）。而事实上，zero-investment的组合也是一个很重要的指标，后期精益alphalens的时候，我们可以自己增加。

![](https://img-blog.csdn.net/20170829215055485?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        上面这个图，就是把每一层分开了构建了组合，同时，每一层内，只有多头，而股票的权重是按照因子的大小来设定。（具体权重设置是否如此有待考察）对于这类图，评价标准就是累计收益率是否发散，越发散越好，说明因子的区分度大。

        上面两幅图都是以1天为调仓周期的，后面还有5天和10天为调仓周期的。

# 5.Top minus bottom



![](https://img-blog.csdn.net/20170829215333311?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        这是每次调仓的时候，计算一下top层和bottom层收益率的差。这也是一个很重要的指标。对于好的因子，这个差的绝对值应该越大越好，同时，方向保持稳定。上图中的橙黄色的线则是对每次调仓收益率的差做了一个月度平滑。

![](https://img-blog.csdn.net/20170829215523360?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        比如上面这里的十日调仓和月度，其实趋势很接近了。

        这类图，好的信号应当有下面这样的表现：

a.波动比较小

b.某一个方向的占比处于绝对地位，比如大部分时候的Top Minus Bottom都是正的



