# Backtrader量化平台教程（四）SSA策略实际案例 - lyx的专栏 - CSDN博客





2017年04月24日 20:32:30[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：3719
所属专栏：[量化回测平台Backtrader教程](https://blog.csdn.net/column/details/15582.html)









AD：（本人录制的backtrader视频课程，大家多多支持哦~ [https://edu.csdn.net/course/detail/9040](https://edu.csdn.net/course/detail/9040)）       

上次我们讲述了strategy的完整生命周期，并引入了一个indicator。今天，我们先讲述几个零碎的知识点，然后正式编写一个SSA策略，包括indicator的编写和策略的回测。

# 1.关于data

## 1.1datafeed

        在一开始我们就讨论过datafeed，也就是cerebro的本源，数据。我们看一下上次我们的策略是怎么来的。



```python
dataframe = pd.read_csv('dfqc.csv', index_col=0, parse_dates=True)
    dataframe['openinterest'] = 0
    data = bt.feeds.PandasData(dataname=dataframe,
                            fromdate = datetime.datetime(2015, 1, 1),
                            todate = datetime.datetime(2016, 12, 31)
                            )
    # Add the Data Feed to Cerebro
    cerebro.adddata(data)
```

        我们的数据是从一个dfqc的csv文件里面来的，这里，可能大家没有这个数据，之前上传的csdn，不只是还没通过还是怎么的。今天就把他存到百度云，大家有需要的可以自行下载。这是东风汽车的股票数据，是从wind上获取的。大家没有wind也可以用tushare之类的免费api获得。



链接：http://pan.baidu.com/s/1slnULbv 密码：wi3k

        文件里面的数据大概是这样的：



```
2014-03-13 00:00:00.005,1.425,1.434,1.449,1.418,457767208.0
2014-03-14 00:00:00.005,1.429,1.422,1.436,1.416,196209439.0
2014-03-17 00:00:00.005,1.433,1.434,1.437,1.422,250946201.0
2014-03-18 00:00:00.005,1.434,1.425,1.437,1.424,245516577.0
2014-03-19 00:00:00.005,1.423,1.419,1.423,1.406,331866195.0
2014-03-20 00:00:00.005,1.412,1.408,1.434,1.407,379443759.0
2014-03-21 00:00:00.005,1.406,1.463,1.468,1.403,825467935.0
```

```python
dataframe = pd.read_csv('dfqc.csv', index_col=0, parse_dates=True)
```

         大家注意我们把csv读入pandas的参数，index_col=0表示第一列时间数据是作为pandas 的index的，parse_dates=Ture是自动把数据中的符合日期的格式变成datetime类型。为什么要这样呢？其实读入后的pandas长怎么样都是由backtrader规定的。



![](https://img-blog.csdn.net/20170424195002137?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 1.2 strategy中的data



        大家不知道有没有注意，strategy的初始化函数（init）中，



```python
self.dataclose = self.datas[0].close
```

        有这样一句。在backtrader中，self.datas[0] 与self.data是等价的，换句话说，同一个东西不同的叫法罢了。为什么会有前面这种，笔者认为，backtrader应当可以支持多个datafeed，目前还没有使用到，先按下不表。



        看看前面的pandas的要求的结构，我们就知道，不仅仅有self.datas[0].close,还会有self.datas[0].open。也确实如此。只是我们通常拿close作为一个价格基准罢了。这里，

```python
self.datas[0].close
```

        返回的是一个lines。lines是backtrader一个很重要的概念，我们可以理解为时间序列流，这类数据，后面可以跟index，也就是说，可以有

```python
self.datas[0].close[0]
```

```python
self.datas[0].close[1]
```

```python
self.datas[0].close[-1]
```



等等，但是这里的index是有意义的，0代表当前时刻，-1代表前一时刻，1代表后一时刻，一次类推。



所以在next中使用self.dataclose[0],self.dataclose[-1]就很好理解了。



# 2.SSA



        接下来，开始进入完整的一个策略的研究，同时学习编写一个indicator。

        SSA就是奇异谱分析。

        奇异谱分析是近年来兴起的一种研究非线性时间序列数据的强大的方法。它根据所观测到的时间序列构造出轨迹矩阵，并对轨迹矩阵进行分解、重构，从而提取出代表原时间序列不同成分的信号，如长期趋势信号、周期信号、噪声信号等，从而对时间序列的结构进行分析，并可进一步预测。

        说的通俗点，我们平时用均线，但是有一个问题，均线虽然算法本质上有平滑噪音的含义，但是有时候，也把重要的信息给平滑掉了。所以，我们是不是除了平滑这样的方法之外，要开拓新的方法来分离噪音和有用信息呢。

图像的压缩给我们一个很重要的启示，就是谱分解，如果不知道什么是谱分解的话，只要知道，这是一种先分解，然后获取重要信息的一种方法即可。

        奇异谱分解的基本思想很简单，分成如下几个步骤：

## 2.1将时间序列转化为轨迹矩阵

假设有一个时间序列，当然，对于我们而言，就是股价：Y(T)=(y(1),⋯,y(T))

X=(y1，y2，y3，⋯ym；

      y2，y3，y4，⋯ym+1；

     ..................................

     ..................................

      yn,yn+1,yn+2,⋯,yT)

就变成了这样一个n*m的矩阵。

其中，m为选取的窗口长度，可以当做MA中的窗口长度一样理解。n=T-m+1。

## 2.2SVD分解与矩阵重构

计算X.T*X并对其进行奇异值分解（SVD），从而得到其m个特征值

λ1≥λ2≥⋯≥λm≥0

然后按照从大到小的原则进行矩阵重构。笔者建议大家看一下SVD分解的相关内容，可以更好的理解这一部分。

假设重构后的矩阵为

X=(x1,1 x1,2 x1,3 ⋯ x1,m；

 x2,1 x2,2 x2,3  ⋯ x2,m+1 

 ..........................

 ..........................

 xn,1 xn,2 xn,3 ⋯  xn,n+m−1)

## 2.3序列重构

   可是我们要的是像MA一样的线呀，现在这个显然不是，所以我们要再把矩阵变成时间序列。方法还是有很多的，比如最常用的有对角相加：

y1=x1,1 y2=(x1,2 + x2,1)/2  y3=(x1,3 + x2,2 + x3,1)/3

......

## 2.4 接下来，我们就用代码实现上面的这些步骤。



```python
def get_window_matrix(self, input_array, t, m):
        # 将时间序列变成矩阵
        temp = []
        n = t - m + 1
        for i in range(n):
            temp.append(input_array[i:i + m])
        window_matrix = np.array(temp)
        return window_matrix

    def svd_reduce(self, window_matrix):
        # svd分解
        u, s, v = np.linalg.svd(window_matrix)
        m1, n1 = u.shape
        m2, n2 = v.shape
        index = s.argmax()  # get the biggest index
        u1 = u[:, index]
        v1 = v[index]
        u1 = u1.reshape((m1, 1))
        v1 = v1.reshape((1, n2))
        value = s.max()
        new_matrix = value * (np.dot(u1, v1))
        return new_matrix

    def recreate_array(self, new_matrix, t, m):
        # 时间序列重构
        ret = []
        n = t - m + 1
        for p in range(1, t + 1):
            if p < m:
                alpha = p
            elif p > t - m + 1:
                alpha = t - p + 1
            else:
                alpha = m
            sigma = 0
            for j in range(1, m + 1):
                i = p - j + 1
                if i > 0 and i < n + 1:
                    sigma += new_matrix[i - 1][j - 1]
            ret.append(sigma / alpha)
        return ret

    def SSA(self, input_array, t, m):
        window_matrix = self.get_window_matrix(input_array, t, m)
        new_matrix = self.svd_reduce(window_matrix)
        new_array = self.recreate_array(new_matrix, t, m)
        return new_array
```

   这里，我们调用SSA函数就可以获得重构后的时间序列。



那么按照逻辑，重构后的时间序列我们选取最后一个，就是我们要的当天的SSA了，然后就可以像趋势线一样使用。

# 3.indicator编写

   到这里，我们已经有了生成一个像SMA均线一样的函数了，接下来就可以开始写一个自己的indicator。到这里，我们先来看一下上次我们生成的indicator最后plot出来的图：

![](https://img-blog.csdn.net/20170424200502314?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


  我们可以从图上看出很多直观的信息。比如什么时候买，什么时候卖，之前的indicator线长什么样。

  不说SMA，我们写一下我们自己的indicator吧。

  首先，我们看一下我们调用SMA的时候是怎么调用的吧。



```python
self.sma = bt.indicators.SimpleMovingAverage(
            self.datas[0], period=self.params.maperiod)
```

  其实们这里self.datas[0]不最为参数传入也是可以的。



  即如果你写成：



```python
self.sma = bt.indicators.SimpleMovingAverage(period=self.params.maperiod)
```

效果是一毛一样的。



  我们首先可以看到，这是一个和bt.indicator类有很大关系的一个东西，然后可以在调用的时候传入一个参数，在SMA中是平滑移动的窗口大小。

  实际上，任何一个indicator都是bt.indicator的子类，也就是说，我们自己写indicator的时候需要继承bt.indicator。既然是继承，那么接下来就是override几个函数，然后就可以实现功能了。而实际情况就是这样。

我们再来观察一下我们在使用indicator的时候是怎么使用的。



```python
if self.dataclose[0] > self.sma[0]:
```

前面说过，

```python
self.dataclose[0]
```

是一个lines，不难猜测，self.sma也是一个lines。所以说，我们自己写的indica最后返回的就应该是一个lines。



好了，我们开始吧。



```python
class ssa_index_ind(bt.Indicator):
    lines = ('ssa',)
    def __init__(self, ssa_window):
        self.params.ssa_window = ssa_window
        # 这个很有用，会有 not maturity生成
        self.addminperiod(self.params.ssa_window * 2)

    def get_window_matrix(self, input_array, t, m):
        # 将时间序列变成矩阵
        temp = []
        n = t - m + 1
        for i in range(n):
            temp.append(input_array[i:i + m])
        window_matrix = np.array(temp)
        return window_matrix

    def svd_reduce(self, window_matrix):
        # svd分解
        u, s, v = np.linalg.svd(window_matrix)
        m1, n1 = u.shape
        m2, n2 = v.shape
        index = s.argmax()  # get the biggest index
        u1 = u[:, index]
        v1 = v[index]
        u1 = u1.reshape((m1, 1))
        v1 = v1.reshape((1, n2))
        value = s.max()
        new_matrix = value * (np.dot(u1, v1))
        return new_matrix

    def recreate_array(self, new_matrix, t, m):
        # 时间序列重构
        ret = []
        n = t - m + 1
        for p in range(1, t + 1):
            if p < m:
                alpha = p
            elif p > t - m + 1:
                alpha = t - p + 1
            else:
                alpha = m
            sigma = 0
            for j in range(1, m + 1):
                i = p - j + 1
                if i > 0 and i < n + 1:
                    sigma += new_matrix[i - 1][j - 1]
            ret.append(sigma / alpha)
        return ret

    def SSA(self, input_array, t, m):
        window_matrix = self.get_window_matrix(input_array, t, m)
        new_matrix = self.svd_reduce(window_matrix)
        new_array = self.recreate_array(new_matrix, t, m)
        return new_array

    def next(self):
        data_serial = self.data.get(size=self.params.ssa_window * 2)
        self.lines.ssa[0] = self.SSA(data_serial, len(data_serial), int(len(data_serial) / 2))[-1]
```



  大部分是ssa计算大代码，我们看一下之前没见过的。





```python
lines = ('ssa',)
    def __init__(self, ssa_window):
        self.params.ssa_window = ssa_window
        # 这个很有用，会有 not maturity生成
        self.addminperiod(self.params.ssa_window * 2)
```

  这里，lines是必须的，一个indicator至少要有一个lines，里面的ssa就是这个lines的命名，后续会出现。



既然是继承了一个类，那么init函数是必须的了。传入的参数是ssa是窗口大小，那么不言而已，我们收下就是了。

```python
self.addminperiod(self.params.ssa_window * 2)
```

  这个是什么呢？我们在上一次将strategy的生命周期的时候讲到过，比如一个5day的均线，那么开始五天是没有indicator的，这个时候，策略会调用prenext方法。而在indicator中，这个函数就是告诉strategy，我需要几天才能成熟。根据ssa的计算方法，我们需要的是

```python
self.params.ssa_window * 2
```



  然后是next，和strategy一样



```python
def next(self):
        data_serial = self.data.get(size=self.params.ssa_window * 2)
        self.lines.ssa[0] = self.SSA(data_serial, len(data_serial), int(len(data_serial) / 2))[-1]
```

  每一个时刻这个方法都会被调用一次。





```python
self.data.get(size=self.params.ssa_window * 2)
```

  用于获得size数量的数据，当然是往历史方向延伸。



到这里，我们的indicator就完全写好了。

# 4.调用indicator



```python
self.ssa = ssa_index_ind(ssa_window=self.params.ssa_window, subplot=False)
```

  一模一样的调用方法。这里我们看到比SMA多了一个参数，subplot = False。这个最后再解释。



  把所有代码贴出来，然后运行一下吧。



```python
# -*- coding: utf-8 -*-

from __future__ import (absolute_import, division, print_function,
                        unicode_literals)

import datetime  # For datetime objects
import pandas as pd
import backtrader as bt
import numpy as np

class ssa_index_ind(bt.Indicator):
    lines = ('ssa',)
    def __init__(self, ssa_window):
        self.params.ssa_window = ssa_window
        # 这个很有用，会有 not maturity生成
        self.addminperiod(self.params.ssa_window * 2)

    def get_window_matrix(self, input_array, t, m):
        # 将时间序列变成矩阵
        temp = []
        n = t - m + 1
        for i in range(n):
            temp.append(input_array[i:i + m])
        window_matrix = np.array(temp)
        return window_matrix

    def svd_reduce(self, window_matrix):
        # svd分解
        u, s, v = np.linalg.svd(window_matrix)
        m1, n1 = u.shape
        m2, n2 = v.shape
        index = s.argmax()  # get the biggest index
        u1 = u[:, index]
        v1 = v[index]
        u1 = u1.reshape((m1, 1))
        v1 = v1.reshape((1, n2))
        value = s.max()
        new_matrix = value * (np.dot(u1, v1))
        return new_matrix

    def recreate_array(self, new_matrix, t, m):
        # 时间序列重构
        ret = []
        n = t - m + 1
        for p in range(1, t + 1):
            if p < m:
                alpha = p
            elif p > t - m + 1:
                alpha = t - p + 1
            else:
                alpha = m
            sigma = 0
            for j in range(1, m + 1):
                i = p - j + 1
                if i > 0 and i < n + 1:
                    sigma += new_matrix[i - 1][j - 1]
            ret.append(sigma / alpha)
        return ret

    def SSA(self, input_array, t, m):
        window_matrix = self.get_window_matrix(input_array, t, m)
        new_matrix = self.svd_reduce(window_matrix)
        new_array = self.recreate_array(new_matrix, t, m)
        return new_array

    def next(self):
        data_serial = self.data.get(size=self.params.ssa_window * 2)
        self.lines.ssa[0] = self.SSA(data_serial, len(data_serial), int(len(data_serial) / 2))[-1]

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
        self.ssa = ssa_index_ind(ssa_window=self.params.ssa_window, subplot=False)
        # bt.indicator.LinePlotterIndicator(self.ssa, name='ssa')
        self.sma = bt.indicators.SimpleMovingAverage(period=self.params.maperiod)
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
            if self.dataclose[0] > self.ssa[0]:

                # BUY, BUY, BUY!!! (with all possible default parameters)
                self.log('BUY CREATE, %.2f' % self.dataclose[0])

                # Keep track of the created order to avoid a 2nd order
                self.order = self.buy()

        else:

            if self.dataclose[0] < self.ssa[0]:
                # SELL, SELL, SELL!!! (with all possible default parameters)
                self.log('SELL CREATE, %.2f' % self.dataclose[0])

                # Keep track of the created order to avoid a 2nd order
                self.order = self.sell()
    def stop(self):
        print("death")

if __name__ == '__main__':
    # Create a cerebro entity
    cerebro = bt.Cerebro()
    # Add a strategy
    cerebro.addstrategy(MyStrategy)
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
    cerebro.broker.setcash(100.0)
    # 设置每笔交易交易的股票数量
    cerebro.addsizer(bt.sizers.FixedSize, stake=10)
    # Set the commission
    cerebro.broker.setcommission(commission=0.0)
    # Print out the starting conditions
    print('Starting Portfolio Value: %.2f' % cerebro.broker.getvalue())
    # Run over everything
    cerebro.run()
    # Print out the final result
    print('Final Portfolio Value: %.2f' % cerebro.broker.getvalue())
    cerebro.plot()
```

结果是这样的：



![](https://img-blog.csdn.net/20170424202750157?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


  如果我们把

```python
subplot=False
```

删去，大家可以自己运行一下看看plot出来的是怎么样的呢。














