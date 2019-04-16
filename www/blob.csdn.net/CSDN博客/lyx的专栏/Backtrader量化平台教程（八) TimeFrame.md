# Backtrader量化平台教程（八) TimeFrame - lyx的专栏 - CSDN博客





2017年05月24日 22:36:36[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：2084
所属专栏：[量化回测平台Backtrader教程](https://blog.csdn.net/column/details/15582.html)









AD：（本人录制的backtrader视频课程，大家多多支持哦~ [https://edu.csdn.net/course/detail/9040](https://edu.csdn.net/course/detail/9040)）       

有时候我们原有的数据和我们想要的数据不是同一个时间框架下的。譬如，我们手上只有分钟级别的数据，而我们想要的是日线级别的数据，或者说手上是日线级别的数据，希望变成周线级别的数据。在backtrader中，有很好的的方法解决这样的问题。总而言之，就是timeframe转换的问题

# 1.resampling

        这个方法，字面意思看起来是“采样”，准确的来说，是上采样，从小的时间点变成大的时间点。



方法很简单，就是在添加数据的时候，不在使用 cerebro.adddata(data)，而是使用cerebro.resampledata(data, **kwargs)。

        后面的参数主要有两个，一个是timeframe，也就是你希望变成的timeframe是多少，day还是week；另外一个是compression，就是对bar进行压缩。



# 2.代码

        所有的代码是这样的:



```python
from __future__ import (absolute_import, division, print_function,
                        unicode_literals)

import datetime  # For datetime objects
import backtrader as bt
import backtrader.feeds as btfeeds
import backtrader.indicators as btind
import pandas as pd
import numpy as np


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


    def start(self):
        print("the world call me!")

    def prenext(self):
        print("not mature")

    def notify_order(self, order):
        if order.status in [order.Submitted, order.Accepted]:
            # Buy/Sell order submitted/accepted to/by broker - Nothing to do
            return

        # Check if an order has been completed
        # Attention: broker could reject order if not enougth cash
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
        # Simply log the closing price of the series from the reference
        self.log('Close, %.2f' % self.dataclose[0])

        # Check if an order is pending ... if yes, we cannot send a 2nd one
        if self.order:
            return

        # Check if we are in the market
        if not self.position:

            # Not yet ... we MIGHT BUY if ...
            if self.dataclose[0] > self.sma[0]:

                # BUY, BUY, BUY!!! (with all possible default parameters)
                self.log('BUY CREATE, %.2f' % self.dataclose[0])

                # Keep track of the created order to avoid a 2nd order
                self.order = self.buy()

        else:

            if self.dataclose[0] < self.sma[0]:
                # SELL, SELL, SELL!!! (with all possible default parameters)
                self.log('SELL CREATE, %.2f' % self.dataclose[0])

                # Keep track of the created order to avoid a 2nd order
                self.order = self.sell()
    def stop(self):
        print("death")




if __name__ == '__main__':

    # Create a cerebro entity
    cerebro = bt.Cerebro(stdstats=False)


    # Add a strategy
    cerebro.addstrategy(MyStrategy)

    dataframe = pd.read_csv('RB.SHF.csv', index_col=0, parse_dates=True)
    data0 = bt.feeds.PandasData(dataname=dataframe,
                                fromdate=datetime.datetime(2014, 5, 13),
                                todate=datetime.datetime(2014, 6, 20),
                                timeframe=bt.TimeFrame.Minutes
                                )
    # Add the Data Feed to Cerebro
    cerebro.adddata(data0)

    data2 = cerebro.resampledata(data0, timeframe=bt.TimeFrame.Days)
    cerebro.run()
    cerebro.plot(style='bar')
```



老样子，我们来看一下比较核心的代码。



读取数据，数据为分钟级别的数据，如下：


`2014-05-13 08:59:00.005004,3198.0,3198.0,3198.0,3198.0,2148.0,0 2014-05-13 09:00:00.005000,3198.0,3202.0,3195.0,3195.0,37426.0,0 2014-05-13 09:01:00.004997,3195.0,3199.0,3194.0,3198.0,19704.0,0 2014-05-13 09:02:00.005003,3198.0,3199.0,3193.0,3193.0,22682.0,0 2014-05-13 09:03:00.005000,3193.0,3195.0,3192.0,3193.0,23064.0,0 2014-05-13 09:04:00.004996,3193.0,3194.0,3190.0,3190.0,29058.0,0 2014-05-13 09:05:00.005002,3191.0,3191.0,3186.0,3188.0,25044.0,0 2014-05-13 09:06:00.004999,3188.0,3189.0,3186.0,3189.0,16020.0,0 2014-05-13 09:07:00.004995,3189.0,3189.0,3187.0,3188.0,12336.0,0 2014-05-13 09:08:00.005002,3188.0,3188.0,3185.0,3186.0,20484.0,0 2014-05-13 09:09:00.004998,3186.0,3187.0,3184.0,3186.0,19234.0,0`




```python
dataframe = pd.read_csv('RB.SHF.csv', index_col=0, parse_dates=True)
```
dataframe = pd.read_csv('RB.SHF.csv', index_col=0, parse_dates=True)
```python
data0 = bt.feeds.PandasData(dataname=dataframe,
                                fromdate=datetime.datetime(2014, 5, 13),
                                todate=datetime.datetime(2014, 6, 20),
                                timeframe=bt.TimeFrame.Minutes
                                )
```

```python
#timeframe=bt.TimeFrame.Minutes用来指明datafeed的timeframe，默认是days

    # Add the Data Feed to Cerebro，就像平常一样
    cerebro.adddata(data0)

    data2 = cerebro.resampledata(data0, timeframe=bt.TimeFrame.Days)#加入另外一个新的timeframe的datafeed的时候，就不能是adddata了，而是之前说的resampling
    cerebro.run()
    cerebro.plot(style='bar')
```
timeframe=bt.TimeFrame.Minutes用来指明datafeed的timeframe，默认是days

    # Add the Data Feed to Cerebro，就像平常一样
    cerebro.adddata(data0)

    data2 = cerebro.resampledata(data0, timeframe=bt.TimeFrame.Days)#加入另外一个新的timeframe的datafeed的时候，就不能是adddata了，而是之前说的resampling
    cerebro.run()
    cerebro.plot(style='bar')

最后，画出来的plot是这样的：



![](https://img-blog.csdn.net/20170524223525240?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




