# pyalgotrade教程5--多标的策略 - lyx的专栏 - CSDN博客





2017年06月26日 21:17:01[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：3255








        pyalgotrade相比于zipline而言，对于多个标的的投资，似乎是薄弱了一点，但也并不是不行呀。既然是多标的策略，那么肯定有多个csv的add，其实逻辑是很简单，就直接上demo吧，反正很好理解。



```python
# coding=utf-8
from pyalgotrade import strategy
from pyalgotrade.barfeed.csvfeed import GenericBarFeed
from pyalgotrade.bar import Frequency
from pyalgotrade.stratanalyzer import returns
from pyalgotrade.stratanalyzer import sharpe
from pyalgotrade.utils import stats


class MyStrategy(strategy.BacktestingStrategy):
    def __init__(self, feed):
        super(MyStrategy, self).__init__(feed, 10000000)
        self.__position = None

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
        self.__position.exitMarket()

    def onBars(self, bars):
        # 1.我们弄一个简单的策略来假装一下
        day = bars.getDateTime().date().day
        if day == 5:
            self.__position = self.enterLong('a', 1, True)
        elif day == 10:
            self.__position = self.enterLong('b', 1, True)
        elif day == 15:
            self.__position = self.enterLong('c', 1, True)
        elif day == 20:
            self.__position = self.enterLong('d', 1, True)

# 2.读取csv文件.
feed = GenericBarFeed(Frequency.DAY, None, None)
feed.addBarsFromCSV("a", "a.csv")
feed.addBarsFromCSV("b", "b.csv")
feed.addBarsFromCSV("c", "c.csv")
feed.addBarsFromCSV("d", "d.csv")

myStrategy = MyStrategy(feed)

# 3.加入分析器
retAnalyzer = returns.Returns()
myStrategy.attachAnalyzer(retAnalyzer)
sharpeRatioAnalyzer = sharpe.SharpeRatio()
myStrategy.attachAnalyzer(sharpeRatioAnalyzer)

# 4.运行策略
myStrategy.run()

# 5.输出结果
print "Final portfolio value: $%.2f" % myStrategy.getResult()
print "Anual return: %.2f %%" % (retAnalyzer.getCumulativeReturns()[-1] * 100)
print "Average daily return: %.2f %%" % (stats.mean(retAnalyzer.getReturns()) * 100)
print "Std. dev. daily return: %.4f" % (stats.stddev(retAnalyzer.getReturns()))
print "Sharpe ratio: %.2f" % (sharpeRatioAnalyzer.getSharpeRatio(0))
```









