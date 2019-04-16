# Backtrader量化平台教程（一）:backtrader的整体框架 - lyx的专栏 - CSDN博客





2017年03月13日 21:15:03[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：11805
所属专栏：[量化回测平台Backtrader教程](https://blog.csdn.net/column/details/15582.html)









AD：（本人录制的backtrader视频课程，大家多多支持哦~ [https://edu.csdn.net/course/detail/9040](https://edu.csdn.net/course/detail/9040)） 

backtrader是一个量化策略的回测分析平台，功能还是很强大的。

## 1.安装backtrader

安装很简单，和别的lib安装一模一样，pip install backtrader。

## 2.从代码开始



```python
from __future__ import (absolute_import, division, print_function,
                        unicode_literals)

import datetime  # For datetime objects
import os.path  # To manage paths
import sys  # To find out the script name (in argv[0])
import pandas as pd
from WindPy import w
# Import the backtrader platform
import backtrader as bt


# Create a Stratey
class TestStrategy(bt.Strategy):

    def log(self, txt, dt=None):
        ''' Logging function fot this strategy'''
        dt = dt or self.datas[0].datetime.date(0)
        print('%s, %s' % (dt.isoformat(), txt))

    def __init__(self):
        # Keep a reference to the "close" line in the data[0] dataseries
        self.dataclose = self.datas[0].close
        # To keep track of pending orders
        self.order = None

    def notify(self, order):
        if order.status in [order.Submitted, order.Accepted]:
            # Buy/Sell order submitted/accepted to/by broker - Nothing to do
            return

        # Check if an order has been completed
        # Attention: broker could reject order if not enougth cash
        if order.status in [order.Completed, order.Canceled, order.Margin]:
            if order.isbuy():
                self.log('BUY EXECUTED, %.2f' % order.executed.price)
            elif order.issell():
                self.log('SELL EXECUTED, %.2f' % order.executed.price)

            self.bar_executed = len(self)

        # Write down: no pending order
        self.order = None

    def next(self):
        # Simply log the closing price of the series from the reference
        self.log('Close, %.2f' % self.dataclose[0])

        # Check if an order is pending ... if yes, we cannot send a 2nd one
        if self.order:
            return

        # Check if we are in the market
        if not self.position:

            # Not yet ... we MIGHT BUY if ...
            if self.dataclose[0] < self.dataclose[-1]:
                # current close less than previous close

                if self.dataclose[-1] < self.dataclose[-2]:
                    # previous close less than the previous close

                    # BUY, BUY, BUY!!! (with default parameters)
                    self.log('BUY CREATE, %.2f' % self.dataclose[0])

                    # Keep track of the created order to avoid a 2nd order
                    self.order = self.buy()

        else:

            # Already in the market ... we might sell
            if len(self) >= (self.bar_executed + 5):
                # SELL, SELL, SELL!!! (with all possible default parameters)
                self.log('SELL CREATE, %.2f' % self.dataclose[0])

                # Keep track of the created order to avoid a 2nd order
                self.order = self.sell()


if __name__ == '__main__':
    # Create a cerebro entity
    cerebro = bt.Cerebro()

    # Add a strategy
    cerebro.addstrategy(TestStrategy)

    # Create a Data Feed
    # 本地数据，笔者用Wind获取的东风汽车数据以csv形式存储在本地。
    # parase_dates = True是为了读取csv为dataframe的时候能够自动识别datetime格式的字符串，big作为index
    # 注意，这里最后的pandas要符合backtrader的要求的格式
    dataframe = pd.read_csv('dfqc.csv', index_col=0, parse_dates=True)
    dataframe['openinterest'] = 0
    data = bt.feeds.PandasData(dataname=dataframe,
                        fromdate = datetime.datetime(2015, 1, 1),
                        todate = datetime.datetime(2016, 12, 31)
                        )
    # Add the Data Feed to Cerebro
    cerebro.adddata(data)

    # Set our desired cash start
    cerebro.broker.setcash(100000.0)

    # Print out the starting conditions
    print('Starting Portfolio Value: %.2f' % cerebro.broker.getvalue())

    # Run over everything
    cerebro.run()

    # Print out the final result
    print('Final Portfolio Value: %.2f' % cerebro.broker.getvalue())
    # Plot the result
    cerebro.plot()
```

上面这段代码整个框架来自于backtrader的官网（[https://www.backtrader.com](https://www.backtrader.com)），笔者自己修改了获取数据的方式。大家安装完backtrader之后可以复制过去运行一样，一般来说，运行通过就说明环境没有问题（数据文件下文有提到）。接下来我们就分析一下这个不赚钱的策略。



我们想想，量化回测，首先得有数据。

在backtrader里面，DataFeed就是这样一个数据的概念。

```python
dataframe = pd.read_csv('dfqc.csv', index_col=0, parse_dates=True)
dataframe['openinterest'] = 0
data = bt.feeds.PandasData(dataname=dataframe,
                        fromdate = datetime.datetime(2015, 1, 1),
                        todate = datetime.datetime(2016, 12, 31)
                        )
```

这一段代码从本地csv文件获得某一个股票的交易数据，包括open，close，high，low，volume。大家注意到，这里我们的存储形式的pandas。backtrader是支持pandas这样的一种数据格式的，无形中方便了很多

关于该数据文件，大家可以去笔者的百度云下载。

链接：https://pan.baidu.com/s/1slnULbv 密码：wi3k

后面的很多教程案例，都会用到这一数据。



backtrader对每种数据的来源都有一定的标准，要不然咱们相信，不订立标准，框架怎么会知道每一列数据都是干嘛的呢。backtrader对于pandas的标准就是这些列的名字得是open，close，high，low，volume，openinterest。我们这里没有用到openinterest，所以把它设置为零就可以了，这里提示一下，backtrader要求pandas下的DataFeed，pandas的DataFrame的index是时间。

数据的问题解决了，那么数据要交给回测平台。这里整个回测功能的实现都是依赖于cercbro这个类实现的。这个单词的意思，在西班牙语里就是大脑。

我们创造一个大脑，



```python
# Create a cerebro entity
    cerebro = bt.Cerebro()
```

然后把之前的数据喂给大脑：





```python
# Add the Data Feed to Cerebro
    cerebro.adddata(data)
```

基础建设基本就完成了。



接下来就是技术性的较量，策略啦。



```python
class TestStrategy(bt.Strategy):
```

每一个策略都是一个类，是一个继承bt.Strategy类的父类。具体里面的细节，下次再说。



假设我们写好了一个策略，换句话说，就是构造好了一个Strategy类，那么我们就应该把它喂给大脑。



```python
# Add a strategy
    cerebro.addstrategy(TestStrategy)
```

好了，都齐全了。但是别忘了，我们还应该制定初始资金。





```python
# Set our desired cash start
    cerebro.broker.setcash(100000.0)
```



cerebro里面的broker成员就是管这事的。我们给了他100000元。

到这里，该有的都有了，我们run就可以了。cerebro.run()方法就是把上述东西跑起来。

跑完之后，我们想看一下效果，人嘛，可视化的最好，所以：

```python
# Plot the result
    cerebro.plot()
```

就能把结果跑出来。

## 3.我的结果

![](https://img-blog.csdn.net/20170313211002914?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

很显然，亏钱了，但是不要紧，程序对了，万里长征第一步对了。

俗话说的好，万事开头难，中间也难，结尾更难。

## 4.总结

最后总结一下，一个策略，你需要的是数据，而且要让回测平台认识，所以是满足一定规则整理的数据。然后就是回测的“脑子”。这些基础设施有了，写好你的策略。策略其实就是一个类，重写类里的方法就实现了一个个策略。接下来，run就可以了。so easy！















