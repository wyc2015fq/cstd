# pyalgotrade教程1--第一个demo - lyx的专栏 - CSDN博客





2017年06月23日 15:08:21[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：9875








        之前一直使用backtrader作为回测的平台，但是近来觉得，backtrader虽然在有些设计上很精妙，但是官方demo中都有很多细节性的错误，而且很多功能描述模糊，以至于，之前实现日内突破策略的时候，一直没能在代码上实现。前几天在论坛里听到真有人使用pyalgotrade，于是尝试了一下，发现似乎文档可读性高于backtrader的，网上查了一下，使用者虽然不能和zipline比，但是比backtrader还是要多的。反正技多不压身，而且这种平台往往是通的，所以果断研究一遍pyalgotrade。

# 1.pyalgotrade框架

        pyalgotrade官网上的教程虽然很入门，没有接触过的人也能使用，但是一开始可能就会因为无法获取yahoo数据而退却，毕竟我们是在一个大型局域网内。

安装很简单，就是pip。

        PyAlgoTrade主要包含六个部分:

策略：Strategies

回测数据：Feeds

交易经纪人：Brokers

时间序列数据：DataSeries

技术分析：Technicals

优化器：Optimizer

        学习过之前backtrader的同学应该不难理解这些概念，没有学过也没有关系，我们先入个门，看最重要的两个，策略和回测数据。

# 2.第一个hello world demo



```python
# coding=utf-8
from pyalgotrade import strategy
from pyalgotrade.bar import Frequency
from pyalgotrade.barfeed.csvfeed import GenericBarFeed

# 1.构建一个策略
class MyStrategy(strategy.BacktestingStrategy):
    def __init__(self, feed, instrument):
        super(MyStrategy, self).__init__(feed)
        self.__instrument = instrument

    def onBars(self, bars):# 每一个数据都会抵达这里，就像becktest中的next
        bar = bars[self.__instrument]
        self.info(bar.getClose())# 我们打印输出收盘价

# 2.获得回测数据，官网来源于yahoo，由于墙的关系，我们用本地数据
feed = GenericBarFeed(Frequency.DAY, None, None)
feed.addBarsFromCSV("fd", "fd.csv")

# 3.把策略跑起来
myStrategy = MyStrategy(feed, "fd")
myStrategy.run()
```
        这里，数据我们用的很简单，毕竟是第一个demo，目的就是查看开发环境是否可以使用以及基本的一个运行框架。




Date Time,Open,Close,High,Low,Volume,openinterest
2000-01-04 00:00:00,1368.693,1406.371,1407.518,1361.214,903402300.0,0
2000-01-05 00:00:00,1407.829,1409.682,1433.78,1398.323,1057998400.0,0
2000-01-06 00:00:00,1406.036,1463.942,1463.955,1400.253,1348051500.0,0
2000-01-07 00:00:00,1477.154,1516.604,1522.825,1477.154,3451569900.0,0
2000-01-10 00:00:00,1531.712,1545.112,1546.723,1506.404,3125353900.0,0

fd.csv的数据是这样的。



        不出意外的话，应该看到这样的结果：

![](https://img-blog.csdn.net/20170623145502436?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



        我们看到，收盘价被打印了出来。

# 3.解释一下feed

        策略还不是我们关心的范畴，我们先来看一下pyalgotrade是怎么形成feed，也就是回测数据的。与一般的回测平台一样，pyalgotrade有自己的回测数据的数据结构，一般的交易数据存储成csv格式，然后由pyalgotrade自己转化为feed。这个demo中，我们使用的是GenericBarFeed，从名字上可以看出来，就是最普通的feed，我们看一下这个feed的定义：

class pyalgotrade.barfeed.csvfeed.GenericBarFeed(frequency, timezone=None, maxLen=None)

        这个类传入的参数有三个，频率、时区、最大长度。当我们的资产都是在同一个时区的时候，timezone可以设置为None，后面一个没有具体限制，也可以设置为None，所以我们demo中实例化feed的时候，是：

feed = GenericBarFeed(Frequency.DAY, None, None)


GenericBarFeed类只有一个方法，

addBarsFromCSV(instrument, path, timezone=None)


参数一目了然是什么意思。这里我们要注意的是csv中的内容的格式，大概要求有如下几点：

csv文件的第一列要求是列名，而且要求为

Date Time,Open,High,Low,Close,Volume,Adj Close

2013-01-01 13:59:00,13.51001,13.56,13.51,13.56,273.88014126,13.51001


这样的列名，其中，价格的顺序可以不同，但是名称，包括大小写必须一致。

Adj Close可以没有或者空白
4.technical与DataSeries

接下来，我们实现简单移动均线的计算，也就是SMA。pyalgotrade中的technical提供了计算SMA的函数。

具体代码如下：



```python
# coding=utf-8
from pyalgotrade import strategy
from pyalgotrade.bar import Frequency
from pyalgotrade.barfeed.csvfeed import GenericBarFeed
from pyalgotrade.technical import ma
# 1.构建一个策略
class MyStrategy(strategy.BacktestingStrategy):
    def __init__(self, feed, instrument):
        super(MyStrategy, self).__init__(feed)
        self.__sma = ma.SMA(feed[instrument].getCloseDataSeries(), 2)
        self.__instrument = instrument

    def onBars(self, bars):# 每一个数据都会抵达这里，就像becktest中的next
        bar = bars[self.__instrument]
        self.info("%s %s" % (bar.getClose(), self.__sma[-1]))# 我们打印输出收盘价与两日均线值

# 2.获得回测数据，官网来源于yahoo，由于墙的关系，我们用本地数据
feed = GenericBarFeed(Frequency.DAY, None, None)
feed.addBarsFromCSV("fd", "fd.csv")

# 3.把策略跑起来
myStrategy = MyStrategy(feed, "fd")
myStrategy.run()
```
我看一下核心的一句：





```python
self.__sma = ma.SMA(feed[instrument].getCloseDataSeries(), 2)
```
这里，不是getClose，而是得到了一个close价格的DataSeries也就是，时间序列。



同时，我们要知道，technical产生的技术指标值也是一个DataSeries，也就是说，技术指标还可以被技术指标处理一次。例如，可以直接求RSI的SMA：



```python
self.__rsi = rsi.RSI(feed[instrument].getCloseDataSeries(), 14)
self.__sma = ma.SMA(self.__rsi, 15)
```











