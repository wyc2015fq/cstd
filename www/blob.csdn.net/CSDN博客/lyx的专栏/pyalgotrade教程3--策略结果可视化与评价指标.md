# pyalgotrade教程3--策略结果可视化与评价指标 - lyx的专栏 - CSDN博客





2017年06月24日 14:51:32[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：4805








        我们写好策略，最好回测后，其实是很有必要看一下我们策略的效果，一般采用可视化的折线图与一些指标相结合的方式来评价一个策略。

# 1.策略可视化

        在pyalgotrade中，已经为我们提供了很好用的可视化模块了，plotter。

        所以，一开始我们先导入模块。



```python
from pyalgotrade import plotter
```

        把策略的净值图、买卖开仓情况绘制出来其实很简单，把我们的策略实例化之后，传给plotter就可以了。



```python
myStrategy = MyStrategy(feed, "fd")# 策略实例化
plt = plotter.StrategyPlotter(myStrategy) # 传入策略
myStrategy.run()# 运行策略
myStrategy.info("Final portfolio value: $%.2f" % myStrategy.getResult())
plt.plot()# 绘制
```
效果如如下：

可以看出，只有交易的时点标志和portfolio的净值曲线。

![](https://img-blog.csdn.net/20170624142719408?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


# 2.评价指标



        对于一个策略而言，评价指标当然有很多。

        这一类，在pyalgotrade里面都封装在了pyalgotrade.stratanalyzer里面，里面大概有如下几类评价指标：

a)  class pyalgotrade.stratanalyzer.returns.Returns(maxLen=None)

Returns：包含两种，一个是累计收益率，通过getCumulativeReturns()获得；一种是每个bar的收益率，通过getReturns()获得，返回值都是一个DataSeries

b) class pyalgotrade.stratanalyzer.sharpe.SharpeRatio(useDailyReturns=True)

参数为真，表示使用每日的收益率计算sharpratio，否则的话，按照bar算。通过getSharpeRatio(riskFreeRate, annualized=True)来获得夏普率，其中，第一个参数是无风险利率，后面一个是是否年化的设置。

c) class pyalgotrade.stratanalyzer.drawdown.DrawDown()

回测的指标，有两个：

getLongestDrawDownDuration()，返回的是datetime.timedelta类型，表示最长的回撤时间长度。


getMaxDrawDown()，返回的是最大回撤。


d) class pyalgotrade.stratanalyzer.trades.Trades

这个指标的内容很多。

getCount()：返回总的交易次数

getProfitableCount()：返回盈利的交易次数

getUnprofitableCount()：返回亏损的交易次数

getEvenCount()：返回不赚不亏的交易次数

getAll()：返回一个numpy.array的数据，内容是每次交易的盈亏

getProfits()：返回一个numpy.array的数据，内容是，每次盈利交易的盈利

getLosses()：返回一个numpy.array的数据，内容是每次亏损交易的亏损额

getAllReturns()：返回一个numpy.array的数据，内容是每次交易的盈利，是百分比的形式

getPositiveReturns()：返回一个numpy.array的数据，内容是，每次盈利交易的收益

getNegativeReturns()：返回一个numpy.array的数据，内容是每次亏损交易的损失
下面是四种情况下的手续费

getCommissionsForAllTrades()

getCommissionsForProfitableTrades()

getCommissionsForUnprofitableTrades()

getCommissionsForEvenTrades()

使用这些指标的步骤如下：

（1）import相关指标



```python
from pyalgotrade.stratanalyzer import returns, sharpe, drawdown, trades
```
（2）构实例化一个指标的类



```python
sharpe_ratio = sharpe.SharpeRatio()
```
（3）向策略中添加这个指标类



```python
myStrategy.attachAnalyzer(sharpe_ratio)
```
（4）运行策略



```python
myStrategy.run()
```
（5）获取结果



```python
print "sharpe_ratio", sharpe_ratio.getSharpeRatio(0)
```
这里，0是无风险利率。

# 3.代码



```python
# coding=utf-8
from pyalgotrade import plotter
from pyalgotrade.stratanalyzer import returns, sharpe, drawdown, trades
from pyalgotrade import strategy
from pyalgotrade.bar import Frequency
from pyalgotrade.barfeed.csvfeed import GenericBarFeed
from pyalgotrade.technical import ma
# 1.构建一个策略
class MyStrategy(strategy.BacktestingStrategy):
    def __init__(self, feed, instrument):
        super(MyStrategy, self).__init__(feed)
        self.__position = None
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

    def getSMA(self):
        return self.__sma

    def onBars(self, bars):# 每一个数据都会抵达这里，就像becktest中的next

        # Wait for enough bars to be available to calculate a SMA.
        if self.__sma[-1] is None:
            return
        #bar.getTyoicalPrice = (bar.getHigh() + bar.getLow() + bar.getClose())/ 3.0

        bar = bars[self.__instrument]
        # If a position was not opened, check if we should enter a long position.
        if self.__position is None:
            if bar.getPrice() > self.__sma[-1]:
                # 开多头.
                self.__position = self.enterLong(self.__instrument, 100, True)
        # 平掉多头头寸.
        elif bar.getPrice() < self.__sma[-1] and not self.__position.exitActive():
            self.__position.exitMarket()

# 2.获得回测数据，官网来源于yahoo，由于墙的关系，我们用本地数据
feed = GenericBarFeed(Frequency.DAY, None, None)
feed.addBarsFromCSV("fd", "fd.csv")

# 3.实例化策略
myStrategy = MyStrategy(feed, "fd")
# 4.设置指标和绘图
sharpe_ratio = sharpe.SharpeRatio()
myStrategy.attachAnalyzer(sharpe_ratio)
plt = plotter.StrategyPlotter(myStrategy)

# 5.运行策略
myStrategy.run()
myStrategy.info("Final portfolio value: $%.2f" % myStrategy.getResult())

# 6.输出夏普率、绘图
print "sharpe_ratio", sharpe_ratio.getSharpeRatio(0)
plt.plot()
```













