# Backtrader量化平台教程（六）Analyzer - lyx的专栏 - CSDN博客





2017年05月02日 23:43:02[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：2819
所属专栏：[量化回测平台Backtrader教程](https://blog.csdn.net/column/details/15582.html)









AD：（本人录制的backtrader视频课程，大家多多支持哦~ [https://edu.csdn.net/course/detail/9040](https://edu.csdn.net/course/detail/9040)）

# 1.什么是Anayzer        

        我们写了一个策略，跑了一下，看了一下图，觉得不错。但是，究竟我们的策略如何呢？譬如，Sharpe Ratio是多少，最大回撤是多少等等。这些指标我们如何获取呢？在backtrader中，给我们提供了获取这些指标的途径，就是Analyzer。顾名思义，就是分析师。

        这里我们要提一下，Analyzer与Lines这一类数据是不一样的。显然的，Analyzer是在你跑完策略之后给你一个结果，不会每时每刻都有一个数据产生。但是我们要说明的是在backtrader的内部，其实Analyzer也是一种Lines，这可能是出于代码后期兼容性和可扩展性的考虑吧，作为end user，其实不用考虑那么多。

# 2.如何使用Anayzer

        具体怎么使用Analyzer呢？和Strategy、Observe和Datas一样，我们把我们想要的Analyzer加入到cerebro中就可以了，具体使用的方法是：

        addanalyzer()



```python
# -*- coding: utf-8 -*-

from __future__ import (absolute_import, division, print_function,
                        unicode_literals)

import datetime  # For datetime objects
import pandas as pd
import backtrader as bt
import numpy as np

# Create a Stratey
class MyStrategy(bt.Strategy):
    params = (
        ('ssa_window', 15),
        ('maperiod', 15),
    )

    def log(self, txt, dt=None):
        ''' Logging function fot this strategy'''
        dt = dt or self.datas[0].datetime.date(0)
        print('%s, %s' % (dt.isoformat(), txt))

    def __init__(self):
        # Keep a reference to the "close" line in the data[0] dataseries
        self.dataclose = self.datas[0].close

        # To keep track of pending orders and buy price/commission
        self.order = None
        self.buyprice = None
        self.buycomm = None

        self.sma = bt.indicators.SimpleMovingAverage(
            self.datas[0], period=self.params.maperiod)
    def notify_order(self, order):
        if order.status in [order.Submitted, order.Accepted]:
            return

        if order.status in [order.Completed]:
            if order.isbuy():
                self.log(
                    'BUY EXECUTED, Price: %.2f, Cost: %.2f, Comm %.2f' %
                    (order.executed.price,
                     order.executed.value,
                     order.executed.comm))

                self.buyprice = order.executed.price
                self.buycomm = order.executed.comm
            else:  # Sell
                self.log('SELL EXECUTED, Price: %.2f, Cost: %.2f, Comm %.2f' %
                         (order.executed.price,
                          order.executed.value,
                          order.executed.comm))

            self.bar_executed = len(self)

        elif order.status in [order.Canceled, order.Margin, order.Rejected]:
            self.log('Order Canceled/Margin/Rejected')

        self.order = None

    def notify_trade(self, trade):
        if not trade.isclosed:
            return

        self.log('OPERATION PROFIT, GROSS %.2f, NET %.2f' %
                 (trade.pnl, trade.pnlcomm))

    def next(self):
        self.log('Close, %.2f' % self.dataclose[0])
        if self.order:
            return
        if not self.position:
            if self.dataclose[0] > self.sma[0]:
                self.log('BUY CREATE, %.2f' % self.dataclose[0])
                self.order = self.buy()
        else:
            if self.dataclose[0] < self.sma[0]:
                self.log('SELL CREATE, %.2f' % self.dataclose[0])
                self.order = self.sell()

if __name__ == '__main__':
    cerebro = bt.Cerebro()
    cerebro.addstrategy(MyStrategy)
    dataframe = pd.read_csv('dfqc.csv', index_col=0, parse_dates=True)
    dataframe['openinterest'] = 0
    data = bt.feeds.PandasData(dataname=dataframe,
                            fromdate = datetime.datetime(2015, 1, 1),
                            todate = datetime.datetime(2016, 12, 31)
                            )
    cerebro.adddata(data)
    cerebro.broker.setcash(100.0)
    cerebro.addsizer(bt.sizers.FixedSize, stake=10)
    cerebro.broker.setcommission(commission=0.0)
    print('Starting Portfolio Value: %.2f' % cerebro.broker.getvalue())
    cerebro.addanalyzer(bt.analyzers.SharpeRatio, _name = 'SharpeRatio')
    cerebro.addanalyzer(bt.analyzers.DrawDown, _name='DW')
    results = cerebro.run()
    strat = results[0]
    print('Final Portfolio Value: %.2f' % cerebro.broker.getvalue())
    print('SR:', strat.analyzers.SharpeRatio.get_analysis())
    print('DW:', strat.analyzers.DW.get_analysis())
    cerebro.plot()
```

         用的还是我们之前SMA的例子。我们着重看一下我们的analyzer是怎么添加和使用的。





```python
cerebro.addanalyzer(bt.analyzers.SharpeRatio, _name = 'SharpeRatio')
    cerebro.addanalyzer(bt.analyzers.DrawDown, _name='DW')
    results = cerebro.run()
    strat = results[0]
    print('Final Portfolio Value: %.2f' % cerebro.broker.getvalue())
    print('SR:', strat.analyzers.SharpeRatio.get_analysis())
    print('DW:', strat.analyzers.DW.get_analysis())
    cerebro.plot()
```

        我们看一下，在rcerebro run之前，我们先添加了两个analyzer：SharpeRatio和DrawDown，在添加的时候，我们分别对这两个anayzer命令名，也就是

```python
_name = 'SharpeRatio'与 _name='DW'
```
_name='DW'


        为了使用这一anayzer，我们需要获取cerebro run之后的返回值，然后获取策略相关信息，



```python
results = cerebro.run()
strat = results[0]
```

        这一部分在后续解析cerebro时再展开。





```python
print('SR:', strat.analyzers.SharpeRatio.get_analysis())
    print('DW:', strat.analyzers.DW.get_analysis())
```

        这里，我们就获取了这一策略的SharpeRatio与回撤的信息了。



![](https://img-blog.csdn.net/20170502233812332?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        我们可以看到，回撤信息布置最大回撤，还有好多。

# 3.有哪些指标

        那么，具体，有哪些Analyzer呢？

        笔者大概列了一下：

AnnualReturn

Calmar

DrawDown

TimeDrawDown

GrossLeverage

PositionsValue

PyFolio

LogReturnsRolling

PeriodStats

Returns

SharpeRatio

SharpeRatio_A

SQN

TimeReturn

TradeAnalyzer

Transactions

VWR

        大概有这么些，具体后续用到了再详细讲解。



