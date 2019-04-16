# pyfolio教程2——第一个returns_tear_sheet - lyx的专栏 - CSDN博客





2019年03月22日 22:12:42[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：59








        首先，说明一下我们的数据，为了一步一步的明确pyfolio的功能和一些结果，我们首先选取我们的策略是0.3的中证500指数、0.3的中证1000指数和0.4的上证50指数，也就是我们的组合的收益就是这三个指数的加权收益。换句话说，我们是把这三个指数当成了三个资产。benchmark是沪深300，后续有需要的时候，回依次加入其它的东西。

        我们先来看一下我的return ser文件：

```
date,zz500,zz1000,sz50,return
2016/2/22,0.020771931,0.020462588,0.024827323,0.022301285
2016/2/23,-0.005443023,-0.006005945,-0.011851228,-0.008175182
2016/2/24,0.006678676,0.000874356,0.005846597,0.004604548
2016/2/25,-0.07936297,-0.079150999,-0.053290504,-0.068870392
2016/2/26,0.004067755,-0.00464371,0.011794361,0.004544958
```

        在notebook中写入下面代码。

```
import pyfolio as pf
import pandas as pd
%matplotlib inline

return_ser = pd.read_csv('return_ser.csv')
return_ser['date'] = pd.to_datetime(return_ser['date'])
return_ser.set_index('date', inplace=True)
pf.create_returns_tear_sheet(return_ser['return'])
```

      这里，我们最简单的使用了一个creat_returns_tear_sheet。然后，我们来看一下结果：

![](https://img-blog.csdnimg.cn/20190321200920876.JPG)

      这个比较好理解，就是我们策略收益的开始和结束，以及一共跨度的时间。

![](https://img-blog.csdnimg.cn/20190321201015614.JPG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sdXlpeGlhby5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

      然后是整个return时间序列能够计算出来的指标。大部分比较常见，解释一下比较少见的。首先是stability，也就是所谓的稳定性。其实很简单，就是时间增量对累计净值的解释力度是多少，也就是回归的r平方。这样说有点抽象，我们简单解释一下。

      假设我们的daily return的净值曲线是这样的：

![](https://img-blog.csdnimg.cn/20190321201410345.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sdXlpeGlhby5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

      而这个黑线则是随着时间等量递增的。那么净值和这条直线回归的R方就是所谓的稳定性。

      Tail ratio比较有趣，对daily return的分布选取95分位和5分位，然后相除取绝对值。本质的含义就是赚取的return比亏钱的大多少倍。

      然后是回测的情况：

![](https://img-blog.csdnimg.cn/20190321201747535.JPG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sdXlpeGlhby5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

      后面的图相对而言就比较简单了，不用解释就可以看懂，直接放上来。

![](https://img-blog.csdnimg.cn/20190321202609415.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sdXlpeGlhby5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

接下来，我们好好研究一下这个return tear sheet的参数。

```python
def create_returns_tear_sheet(returns, positions=None,
                              transactions=None,
                              live_start_date=None,
                              cone_std=(1.0, 1.5, 2.0),
                              benchmark_rets=None,
                              bootstrap=False,
                              turnover_denom='AGB',
                              header_rows=None,
                              return_fig=False)
```

我们看到这个函数的签名是这样的。

## 1、live_start_date

      我们先来研究几个简单的吧，比如第二个，live_start_date，这个参数的意思就是，你的return啥时候是开始实盘的？比如我们上面这一串return，假设我们在2018年6月份之后是实盘，而前面是模拟盘或者是回测的结果，那么我们就可以这样设置：

```python
live_start_date=‘2018-06-01’
```

我们看一下结果：

![](https://img-blog.csdnimg.cn/20190321203124888.JPG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sdXlpeGlhby5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

我们发现多了几个out of sample的东西，也就是说，各种指标开始区分样本内外了，理论上我们可以根据这个来看一下我们的策略有没有过拟合。如果样本内外的差异很大，那么大概率这就是过拟合了。

![](https://img-blog.csdnimg.cn/20190321203308761.JPG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sdXlpeGlhby5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

图片中，样本外也会被标记出来，而且会给出不同的标准差的区间。那么，这个标准差是怎么决定的呢？在pyfolio里面有这样一段注释：

```
Determines the upper and lower bounds of an n standard deviation
cone of forecasted cumulative returns. Future cumulative mean and
standard devation are computed by repeatedly sampling from the
in-sample daily returns (i.e. bootstrap). This cone is non-parametric,
meaning it does not assume that returns are normally distributed.
```

      说白了就是用历史的dailyreturn通过bootstrap来进行计算的。所谓的bootstrap其实就是有放回的抽样。而标准差倍数则由cone_std=(1.0, 1.5, 2.0)这样一个tuple来决定。

## 2、benchmark

      然后，我们在加入benchmark试一试看看有什么变化：

```python
pf.create_returns_tear_sheet(return_ser['return'], live_start_date='2018-06-01', benchmark_rets=return_ser['bench'])
```

      我们看到，多了beta和alpha

![](https://img-blog.csdnimg.cn/20190321204232522.JPG)

     图上也是，多了一个benchmark：
![](https://img-blog.csdnimg.cn/20190321204320279.JPG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sdXlpeGlhby5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

      同时也多了一个滚动的beta

![](https://img-blog.csdnimg.cn/20190321204400309.JPG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sdXlpeGlhby5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)









