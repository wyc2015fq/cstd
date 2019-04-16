# pyalgotrade教程4--broker设置：交易费用，滑点模型 - lyx的专栏 - CSDN博客





2017年06月25日 17:45:39[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：4437








前面，我们完全没有考虑交易的手续费、交易的滑点等等。而实际在回测的时候，这些都是要实实在在考虑的因素。pyalgotrade里面通过实现一个broker类，来完成这一系列的设置。

# 1.broker设置的步骤

**step1.实例一个手续费策略类**

        根据不同的手续费策略实现不同的手续费策略类。



a.没有手续费

broker_commission = pyalgotrade.broker.backtesting.NoCommission()



b.每笔交易手续费是固定的，无论你的交易量如何
broker_commission = pyalgotrade.broker.backtesting.FixedPerTrade(amount)

amount：每笔交易的手续费



c.手续费是每笔交易金额的百分比

broker_commission = broker.backtesting.TradePercentage(percentage )

percentage (float.) – 0到1之间的数字，0.01表示1%


**step2.实例化一个broker**

        我们需要实例化一个broker类传给策略，broker类的定义如下：

pyalgotrade.broker.backtesting.Broker(cash, barFeed, commission=None)

        类的初始化参数如下：

cash (int/float.) – 回测策略的初始金额.

barFeed (pyalgotrade.barfeed.BarFeed) – 我们之前获得的bar数据.

commission (Commission) –第一步中生成的手续费策略类.

        这个broker类，还可以设置成交策略模型，通过：

setFillStrategy(strategy)来实现。

strategy是一个 pyalgotrade.broker.fillstrategy.FillStrategy类


**step3.pyalgotrade.broker.fillstrategy.FillStrategy类**

        FillStrategy是一个成交策略的基类，通常我们通过实现：

pyalgotrade.broker.fillstrategy.DefaultStrategy(volumeLimit=0.25)

        这个类来实现一个成交策略。

volumeLimit (float) – 一个bar中，能够成交的数目占成交量的比值，大于0，小于等于1.譬如，我们在某时刻下单100手，由于是回测数据，我们知道总成交量，假设为200，则当这个参数设置为0.25的时候，不能完全成交，大于0.5的时候 ，可以完全成交。

这个类有两个set方法，分别是

setSlippageModel(slippageModel)与setVolumeLimit(volumeLimit)

后者其实就是修改前面的那个最大可成交比例，而前面这个就是设置滑点模型，其传入的参数是一个pyalgotrade.broker.slippage.SlippageModel类，而这个类是一个基类，我们通过实现下面这些类子类来获得滑点模型。


**step4.滑点模型**

        滑点是回测的时候，记得下单影响实际价格，从而影响你的执行价的数目。当你进行一笔交易的时候，你的买单会使得价格上升，你的卖单会是的价格下降，这就是所谓的你的交易影响了市场价格。对价格的影响程度取决于你的交易单量与当前市场的比较。


a.没有滑点

pyalgotrade.broker.slippage.NoSlippage()



b.VolumeShareSlippage滑点模型。

pyalgotrade.broker.slippage.VolumeShareSlippage(priceImpact=0.1)

priceImpact (float.) – 你的订单对交易价格的影响程度有多大。

这一模型与 Zipline的 VolumeShareSlippage model一直.滑点计算大致为： priceImpact 乘以下单的量除以总量的比例的平方。

在VolumeShareSlippage模型中，你成交的价格是你的交易量相对总成交量的比的函数。首先，你提供一个成交量的限制，譬如总量的2.5%，这个参数限制的是每根bar上你最大能成交的数量。

举例来说，如果回测的数据是分钟数据，你下单60手，而后面的每分钟成交量是1000手，成交量的限制（volume_limit ）是2.5%，那么你的交易单会被分为25手、25手、10手，分三分钟完成。

另外一个参数是价格影响常量（price impact constant），定义为你对回测价格影响的程度。滑点计算为：价格影响常量 *（下单量/成交量） ^2。





# 2.具体例子代码



```python
# coding=utf-8
from pyalgotrade import plotter
from pyalgotrade.stratanalyzer import returns, sharpe, drawdown, trades
from pyalgotrade import strategy
from pyalgotrade.bar import Frequency
from pyalgotrade.barfeed.csvfeed import GenericBarFeed
from pyalgotrade.technical import ma
from pyalgotrade import broker
# 1.构建一个策略
class MyStrategy(strategy.BacktestingStrategy):
    def __init__(self, feed, instrument, brk):
        super(MyStrategy, self).__init__(feed, brk)
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
                self.__position = self.enterLong(self.__instrument, 10, True)
        # 平掉多头头寸.
        elif bar.getPrice() < self.__sma[-1] and not self.__position.exitActive():
            self.__position.exitMarket()

# 2.获得回测数据，官网来源于yahoo，由于墙的关系，我们用本地数据
feed = GenericBarFeed(Frequency.DAY, None, None)
feed.addBarsFromCSV("fd", "fd.csv")
# 3.broker setting
# 3.1 commission类设置
broker_commission = broker.backtesting.TradePercentage(0.0001)
# 3.2 fill strategy设置
fill_stra = broker.fillstrategy.DefaultStrategy(volumeLimit=0.1)
sli_stra = broker.slippage.NoSlippage()
fill_stra.setSlippageModel(sli_stra)
# 3.3完善broker类
brk = broker.backtesting.Broker(1000000, feed, broker_commission)
brk.setFillStrategy(fill_stra)
# 4.把策略跑起来

myStrategy = MyStrategy(feed, "fd", brk)

# Attach a returns analyzers to the strategy.
trade_situation = trades.Trades()
myStrategy.attachAnalyzer(trade_situation)
# Attach the plotter to the strategy.
plt = plotter.StrategyPlotter(myStrategy)

# Run the strategy.
myStrategy.run()
myStrategy.info("Final portfolio value: $%.2f" % myStrategy.getResult())
print "total number of trades", trade_situation.getCount()
print "commissions for each trade",trade_situation.getCommissionsForAllTrades()
# Plot the strategy.
plt.plot()
```
最后我们能够看到运行的结果，其中，把每次交易的费用列了出来：



![](https://img-blog.csdn.net/20170625174324941?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



# 3.再来理一下这些类之间的关系

![](https://img-blog.csdn.net/20170625174505030?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
















