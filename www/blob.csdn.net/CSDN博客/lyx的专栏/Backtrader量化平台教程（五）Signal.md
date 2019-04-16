# Backtrader量化平台教程（五）Signal - lyx的专栏 - CSDN博客





2017年04月29日 10:16:11[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：2475
所属专栏：[量化回测平台Backtrader教程](https://blog.csdn.net/column/details/15582.html)









AD：（本人录制的backtrader视频课程，大家多多支持哦~ [https://edu.csdn.net/course/detail/9040](https://edu.csdn.net/course/detail/9040)）       

其实在backtrader中，我们也可以不编写Strategy类来实现我们的简单策略。这就涉及到另外一种策略的形式，Signal。在之前的例子中，写一个Strategy，初始化Indicator，在next方法中编写buy和sell的逻辑。但是当我们采用Signal的方式之后，我们只需要给Cerebro类add Signal就可以了。

# 1.代码

        代码很简单，之前用Strategy实现的的SMA策略用Signal的方式来实现其实就是这样：



```python
# -*- coding: utf-8 -*-

from __future__ import (absolute_import, division, print_function,
                        unicode_literals)

import datetime  # For datetime objects
import pandas as pd
import backtrader as bt

class MySignal(bt.Indicator):
    lines = ('signal',)
    params = (('period', 30),)

    def __init__(self):
        self.lines.signal = self.data - bt.indicators.SMA(period=self.p.period)

if __name__ == '__main__':
    cerebro = bt.Cerebro()
    dataframe = pd.read_csv('dfqc.csv', index_col=0, parse_dates=True)
    dataframe['openinterest'] = 0
    data = bt.feeds.PandasData(dataname=dataframe,
                            fromdate = datetime.datetime(2015, 1, 1),
                            todate = datetime.datetime(2016, 12, 31)
                            )
    # Add the Data Feed to Cerebro
    cerebro.adddata(data)

    cerebro.add_signal(bt.SIGNAL_LONGSHORT, MySignal, subplot=False)
    # 这句话很有用，画图看效果
    # cerebro.signal_accumulate(True)
    cerebro.broker.setcash(10000.0)
    cerebro.addsizer(bt.sizers.FixedSize, stake=10)
    cerebro.broker.setcommission(commission=0.0)
    print('Starting Portfolio Value: %.2f' % cerebro.broker.getvalue())
    cerebro.run()
    print('Final Portfolio Value: %.2f' % cerebro.broker.getvalue())
    cerebro.plot()
```

      在MySignal中，我们产生的逻辑是





```python
self.lines.signal = self.data - bt.indicators.SMA(period=self.p.period)
```

       其实就是当前的收盘价减去当前的SMA，也就是说，我们这个Indicator子类只会产生三种值，>0,<0,=0.每一个时刻，这个值会被Cerebro接收，当然啦，我们要先把我们的Siganl（其实就是一个Indicator）加入到cerebro中：





```python
cerebro.add_signal(bt.SIGNAL_LONGSHORT, MySignal, subplot=False)
```

       这样，我们就实现了。



# 2.Signal的含义

        那么cerebro是怎么解读Signal的呢？

> 0 -> 买入

< 0 -> 卖出

== 0 -> 没有指令

        注意add_signal函数的第一个参数，这个参数限制了cerebro的买入卖出的行为。 

        这一参数共有五中类型，又可以分为两类，为MainGroup和ExitGroup。我们主要介绍MainGroup，Exit部分，笔者还在摸索。

Main Group:

LONGSHORT: 买入卖出信号都接受执行

LONG:买入信号执行，卖出信号仅仅将多头头寸平仓，而不反向卖出。

SHORT:卖出信号被执行，而买入信号仅仅将空头头寸平仓，而不方向买入。

# 3.效果

我们运行一下，可以看到下面的图：

LONGSHORT：

![](https://img-blog.csdn.net/20170429101008543?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



LONG：注意上方的红色cash和value与上面的区别，可以发现，LONG的情况下，并不会卖出证券标的。

![](https://img-blog.csdn.net/20170429101033635?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        最后，还有一个参数要提一下，

```python
cerebro.signal_accumulate(True)
```

        如果我们在add signal之后加入这一句，我们看一下图：



![](https://img-blog.csdn.net/20170429101219123?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        我们会发现，信号不断的在产生，这就是accumulate的效果。当然，更多的时候，我们要的并不是这样的效果，所以，默认情况下，signal并不是accumulate的。



