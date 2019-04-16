# pyalgotrade教程2--第一笔交易 - lyx的专栏 - CSDN博客





2017年06月24日 14:11:42[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：32228








        最快的速度扫描了一遍pyalgotrade的文档，从可理解性角度来讲，确实比backtrader好很多，但是功能方面似乎就有缺失了。功能缺失也有好处，就是能够更加灵活，不用再受到文档描述不清楚，不了解功能怎么用的痛苦了吧。

# 1.pyalgotrade的交易

        这里，还是老样子，用简单的SMA策略来学习一下pyalgotrade的基本交易方法。当现在价格上穿SMA时，开多单；当现在的价格，下穿SMA时，平掉先前的多头头寸。



```python
def onBars(self, bars):# 每一个数据都会抵达这里，就像becktest中的next
        # SMA的计算存在窗口，所以前面的几个bar下是没有SMA的数据的.
        if self.__sma[-1] is None:
            return
        #bar.getTyoicalPrice = (bar.getHigh() + bar.getLow() + bar.getClose())/ 3.0

        bar = bars[self.__instrument]
        # If a position was not opened, check if we should enter a long position.
        if self.__position is None:# 如果手上没有头寸，那么
            if bar.getPrice() > self.__sma[-1]:
                # 开多，如果现价大于移动均线，且当前没有头寸.
                self.__position = self.enterLong(self.__instrument, 100, True)
        # 当前有多头头寸，平掉多头头寸.
        elif bar.getPrice() < self.__sma[-1] and not self.__position.exitActive():
            self.__position.exitMarket()
```
        这一部分基本就是交易逻辑。





```python
enterLong(self.__instrument, 100, True)
```
        这就是开多单的命令，第一个就是开多单的标的，100是下单数目，最后一个是allOrNone参数，也就是说，是不是要么全部成交，要么一个不成，默认是false。


与backtrader一样，也存在各种各样的订单监听函数，



```python
def onEnterOk(self, position):
        execInfo = position.getEntryOrder().getExecutionInfo()
        self.info("BUY at %.2f" % (execInfo.getPrice()))

    def onEnterCanceled(self, position):
        self.__position = None

    def onExitOk(self, position):
        execInfo = position.getExitOrder().getExecutionInfo()
        self.info("SELL at $%.2f" % (execInfo.getPrice()))
        self.__position = None

    def onExitCanceled(self, position):
        # If the exit was canceled, re-submit it.
        self.__position.exitMarket()
```


        基本都是大同小异，功能上没有什么实质性区别。

# 2.SMA策略示例完整代码



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
        self.__position  = None
        self.__sma = ma.SMA(feed[instrument].getCloseDataSeries(), 150)
        self.__instrument = instrument
        self.getBroker()
    def onEnterOk(self, position):
        execInfo = position.getEntryOrder().getExecutionInfo()
        self.info("BUY at %.2f" % (execInfo.getPrice()))

    def onEnterCanceled(self, position):
        self.__position = None

    def onExitOk(self, position):
        execInfo = position.getExitOrder().getExecutionInfo()
        self.info("SELL at $%.2f" % (execInfo.getPrice()))
        self.__position = None

    def onExitCanceled(self, position):
        # If the exit was canceled, re-submit it.
        self.__position.exitMarket()


    def onBars(self, bars):# 每一个数据都会抵达这里，就像becktest中的next
        # SMA的计算存在窗口，所以前面的几个bar下是没有SMA的数据的.
        if self.__sma[-1] is None:
            return
        #bar.getTyoicalPrice = (bar.getHigh() + bar.getLow() + bar.getClose())/ 3.0

        bar = bars[self.__instrument]
        # If a position was not opened, check if we should enter a long position.
        if self.__position is None:# 如果手上没有头寸，那么
            if bar.getPrice() > self.__sma[-1]:
                # 开多，如果现价大于移动均线，且当前没有头寸.
                self.__position = self.enterLong(self.__instrument, 100, True)
        # 当前有多头头寸，平掉多头头寸.
        elif bar.getPrice() < self.__sma[-1] and not self.__position.exitActive():
            self.__position.exitMarket()

# 2.获得回测数据，官网来源于yahoo，由于墙的关系，我们用本地数据
feed = GenericBarFeed(Frequency.DAY, None, None)
feed.addBarsFromCSV("fd", "fd.csv")

# 3.把策略跑起来

myStrategy = MyStrategy(feed, "fd")
myStrategy.run()
myStrategy.info("Final portfolio value: $%.2f" % myStrategy.getResult())
```
        最后，我们看到把myStrategy.getResult()输出，就可以看到最后我们的资产净值。

2015-08-14 00:00:00 strategy [INFO] BUY at 3976.41

2015-08-19 00:00:00 strategy [INFO] SELL at $3646.80

2016-07-13 00:00:00 strategy [INFO] BUY at 3049.51

2016-08-02 00:00:00 strategy [INFO] SELL at $2950.08

2016-08-03 00:00:00 strategy [INFO] BUY at 2963.22

2016-12-30 00:00:00 strategy [INFO] Final portfolio value: $1394503.20












