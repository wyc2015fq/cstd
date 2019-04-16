# pyfolio教程1--第一个pyfolio demo - lyx的专栏 - CSDN博客





2017年07月01日 22:54:12[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：4195








        相比于alphalens，pyfolio使用起来要简单很多。安装与alphalens一样，直接pip就可以了。数据也很简单，基本在国内使用的话，用于benchmark不可能让pyfolio自己去获取，所以，最简单的demo中，只需要我们的portfolio的daily return与benchmark的daily return就可以了。

        直接上一段代码吧，避免以后要用pyfolio的时候不知道如何开始。



```python
import pyfolio as pf
import pandas as pd
import matplotlib.pyplot as plt
benchmark_rets = pd.read_csv('return.csv', index_col=0, parse_dates=True)
stock_rets = pd.read_csv('benchmark.csv', index_col=0, parse_dates=True)
returns = stock_rets['return'].tz_localize('UTC')
benchmark_rets = benchmark_rets['return'].tz_localize('UTC')
live_start_date = '2016-5-1'
pf.create_returns_tear_sheet(returns, benchmark_rets=benchmark_rets, live_start_date=live_start_date)
plt.show()
```

代码着重讲几点：

1.returns的daily return必须是pandas的series，所以不是只是读进csv就可以了，因为read_csv返回的是一个dataframe对象;

2.存在时区的问题，如果不像上面这么调的话，运行会出问题。

3.这里是一下子绘制了所有的图片，在notebook上可能还可以，但如果是平pycham这样的ide可能显示有问题，所以        我们更加希望能够实现单个图片的绘制，其实实现起来很简单，查看一下

```python
create_returns_tear_sheet
```
        调用哪些函数就可以了，通常，这些函数都在pyfolio.plotting里面。然后需要什么参数穿什么参数就可以了，还是很easy的。

        其实，使用pyfolio的核心是理解他的结果，并会做出投资决策。正真理解结果，怕就是很难了吧。接下来学习的目标也就是这个了，











