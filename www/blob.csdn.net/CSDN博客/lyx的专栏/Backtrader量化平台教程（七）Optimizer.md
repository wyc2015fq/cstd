# Backtrader量化平台教程（七）Optimizer - lyx的专栏 - CSDN博客





2017年05月05日 16:46:53[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：2130
所属专栏：[量化回测平台Backtrader教程](https://blog.csdn.net/column/details/15582.html)









AD：（本人录制的backtrader视频课程，大家多多支持哦~ [https://edu.csdn.net/course/detail/9040](https://edu.csdn.net/course/detail/9040)）       

还是那我们以前的SMA这个简单到不能叫策略的策略来说事吧。我们在使用SMA这个策略的时候，我们需要传入一个参数，也就是MA的平滑窗口。



```python
self.sma = bt.indicators.SimpleMovingAverage(self.datas[0], period=self.params.maperiod)
```

        这里，第二个参数就是MA的窗口。

        既然是参数，顾名思义，其实我们就可以调整这个参数。同时， 既然是赚钱，当然是越多越好。所以，我们的优化过程就是，不断地去修改这里的这个参数，然后看看每个参数对应的赚钱的效应，最后做个比较。

        如果我们人类来完成这样的过程，显得很麻烦，还好，backtrader里面自带了optimizer。

        其实过程很简单，之前我们是addStrategy来给berebro加入一个量化策略。那么优化一个策略的参数，我们用到的是



```python
cerebro.optstrategy(
        MyStrategy,
        maperiod=range(10, 31))
```



optstrategy这个方法。当然啦，可以想一想，除了把我们的strategy类传入进去时候，肯定还得指定一个优化范围。range(10,31)就是我们的优化范围了，而在sma策略中，参数名是maperiod，所以我们指定方法的参数传入名，就可以实现效果了。

完整的代码如下：

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

        # Add a MovingAverageSimple indicator
        # self.ssa = ssa_index_ind(
        #     self.datas[0], ssa_window=self.params.ssa_window)
        self.sma = bt.indicators.SimpleMovingAverage(
            self.datas[0], period=self.params.maperiod)


    def next(self):
        if self.order:
            return
        if not self.position:
            if self.dataclose[0] > self.sma[0]:
                self.order = self.buy()

        else:

            if self.dataclose[0] < self.sma[0]:
                # Keep track of the created order to avoid a 2nd order
                self.order = self.sell()

    def stop(self):
        self.log('(MA Period %2d) Ending Value %.2f' %
                 (self.params.maperiod, self.broker.getvalue()))


if __name__ == '__main__':
    cerebro = bt.Cerebro()
    cerebro.optstrategy(
        MyStrategy,
        maperiod=range(10, 31))
    dataframe = pd.read_csv('./price_data/000001.SH.csv', index_col=0, parse_dates=True)
    dataframe['openinterest'] = 0
    data = bt.feeds.PandasData(dataname=dataframe,
                            fromdate = datetime.datetime(2015, 1, 1),
                            todate = datetime.datetime(2016, 12, 31)
                            )
    cerebro.adddata(data)
    cerebro.broker.setcash(10000.0)
    cerebro.addsizer(bt.sizers.FixedSize, stake=1)
    cerebro.broker.setcommission(commission=0.0)
    print('Starting Portfolio Value: %.2f' % cerebro.broker.getvalue())
    cerebro.run()
    print('Final Portfolio Value: %.2f' % cerebro.broker.getvalue())
    # cerebro.plot()
```

我们看一下输出结果：

![](https://img-blog.csdn.net/20170505164436823?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

可以看到，我们一下子就获得了不同MA窗口下的最后策略的钱。显然，这个策略是亏钱的。

















