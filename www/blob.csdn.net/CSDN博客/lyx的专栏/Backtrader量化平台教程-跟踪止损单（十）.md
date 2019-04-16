# Backtrader量化平台教程-跟踪止损单（十） - lyx的专栏 - CSDN博客





2018年07月25日 20:34:27[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1245








AD：（本人录制的backtrader视频课程，大家多多支持哦~ [https://edu.csdn.net/course/detail/9040](https://edu.csdn.net/course/detail/9040)）

CTA当中，我们经常会采用跟踪止损的方法来控制回测，backtrader当中其实给我们准备好了这一方法。至于什么叫做跟踪止损单，简单介绍一下。

譬如在15年牛市中，我在某球网上听到一种大道至简的逃顶方式，就是你的净值跟踪止损达到20%的时候，马上全部立场走人，一年内不要碰股票。事实证明，这确实是一个挺好的方法。言下之意，当你某笔交易回撤达到某个值就止损的方法叫做跟踪止损。

```python
class MyStrategy(bt.Strategy):

    def __init__(self):
        self.up_down = three_bars(self.data0)
        self.buy_signal = bt.indicators.CrossOver(self.data.close, self.up_down.up)
        self.sell_signal = bt.indicators.CrossDown(self.data.close, self.up_down.down)


    def next(self):
        if not self.position and self.buy_signal[0] == 1:
            self.order = self.buy(size=1)
            self.order = self.sell(size=1, exectype=bt.Order.StopTrail, trailamount=25)
```

我们看一下上面这个策略，先不管buy_signal是什么，满足交易条件的时候，我们先买了一首，然后同时下了一个卖出的止损单“

```python
self.order = self.buy(size=1)
self.order = self.sell(size=1, exectype=bt.Order.StopTrail, trailamount=25)
```

这里，我们的跟踪止损是这一笔交易亏损25元之后，就止损离场。如果你希望是一个百分比，那么就是，下面这样就是跟踪止损2%。

```python
self.order = self.buy(size=1)
self.order = self.sell(size=1, exectype=bt.Order.StopTrail, trailpercent=0.02)
```

上面的策略跑一下，就是下面这样的。 

![](https://img-blog.csdn.net/20180725203340526?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3F0bHl4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



