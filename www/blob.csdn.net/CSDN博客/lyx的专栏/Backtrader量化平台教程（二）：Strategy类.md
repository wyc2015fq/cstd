# Backtrader量化平台教程（二）：Strategy类 - lyx的专栏 - CSDN博客





2017年03月20日 23:46:16[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：3991
所属专栏：[量化回测平台Backtrader教程](https://blog.csdn.net/column/details/15582.html)









AD：（本人录制的backtrader视频课程，大家多多支持哦~ [https://edu.csdn.net/course/detail/9040](https://edu.csdn.net/course/detail/9040)）

上次我们分析了回测平台大的框架，这次着重介绍一下策略的编写。先来看一个策略的类，上次说了，一个策略其实被一个类完全描述了。



```python
# Create a Stratey
class TestStrategy(bt.Strategy):

    def log(self, txt, dt=None):
        ''' Logging function fot this strategy'''
        dt = dt or self.datas[0].datetime.date(0)
        print('%s, %s' % (dt.isoformat(), txt))

    def __init__(self):
        # Keep a reference to the "close" line in the data[0] dataseries
        self.dataclose = self.datas[0].close

    def next(self):
        # Simply log the closing price of the series from the reference
        self.log('Close, %.2f' % self.dataclose[0])

        if self.dataclose[0] < self.dataclose[-1]:
            # current close less than previous close

            if self.dataclose[-1] < self.dataclose[-2]:
                # previous close less than the previous close

                # BUY, BUY, BUY!!! (with all possible default parameters)
                self.log('BUY CREATE, %.2f' % self.dataclose[0])
                self.buy()
```

        既然是类，那么肯定要initiation，策略类当然也一样。这里，我们的初始化函数就是获取了一个数据而已。这个数据的来源就是我们喂给框架的DataFeed。





       这里初始化的数据是一个很重要的东西，是一个heartbeat。什么叫做heartbeat呢？就是一个时间基准，在strategy类中，当我们获取下标为0的数据的时候，表示的是当前的数据，而-1则是前一时刻，一次类推。既然有了时间线，那么怎么走呢？所以我们有一个next方法，这个方法写的其实就是整个策略的核心部分了，就是你要判断什么时候买股票，什么时候卖股票，卖多少，买多少。这个方法，顾名思义，就是每次一天的数据使用完，就会被cerbero调用，所以叫做next。

这样，整个基本的框架就搭建完毕了。









