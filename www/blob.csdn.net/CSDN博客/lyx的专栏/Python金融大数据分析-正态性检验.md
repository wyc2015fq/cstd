# Python金融大数据分析-正态性检验 - lyx的专栏 - CSDN博客





2016年12月14日 21:00:12[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：7809










## 1.话题引入



我们在线性回归做假设检验，在时间序列分析做自回归检验，那么我们如何检验一个分布是否是正态分布的呢？

首先，我们定义一个用来生成价格路径的函数。当然啦，在这之前我们先导入我们今天要用的库。



```python
import numpy as np
np.random.seed(1000)
import scipy.stats as scs
import statsmodels.api as sm
import matplotlib as mpl
import matplotlib.pyplot as plt
```
然后，我们写一下第一个函数，并做一下测试。




```python
def gen_path(S0,r,sigma,T,M,I):
    dt = float(T)/M
    paths = np.zeros((M+1,I),np.float64)
    paths[0] = S0
    for t in range(1,M+1):
        rand = np.random.standard_normal(I)
        rand = (rand - rand.mean())/rand.std()
        paths[t] = paths[t-1]*np.exp((r - 0.5*sigma**2)*dt + sigma * np.sqrt(dt) * rand)
    return paths
```

```python
S0 = 100
r = 0.05
sigma = 0.2
T = 1.0
M = 50
I = 250000
paths = gen_path(S0,r,sigma,T,M,I)
plt.plot(paths[:,:10])
plt.show()
```
然后，大家就可以看到和昨天和像的一幕了。



![](https://img-blog.csdn.net/20161214174924532?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



当然这是我们仿真出来的路径，那么如果我们真的获取了这样的价格数据，我们要知道他是不是服从正态分布我们该怎么办呢？比较在金融理论里面，正态分布有着很大的优越性。

## 2.正态性检验

我们知道，其实价格服从的是lognormal分布，而每天的收益率是服从正态分布呢，所以，首先我们根据上面的仿真数据来获得每天的收益率数据。



```python
log_returns = np.log(paths[1:]/paths[0:-1])#from second to the end dividend by the first to the last second one to get the return rate
```
看着很玄乎，其实就是每一条路径的今天的价格除以昨天的价格然后取对数。



我们现在写一个函数来研究一下这个每日收益率是个什么东西，至少对他的分布大概有个印象。



```python
def print_statistics(array):
    sta = scs.describe(array)
    #print sta[0]
    print "%14s %15.5f" % ('size',sta[0])
    print "%14s %15.5f" % ('min',sta[1][0])
    print "%14s %15.5f" % ('max',sta[1][1])
    print "%14s %15.5f" % ('mean',sta[2])
    print "%14s %15.5f" % ('std',np.sqrt(sta[3]))
    print "%14s %15.5f" % ('skew',sta[4])
    print "%14s %15.5f" % ('kurtosis',sta[5])
```

```python
print_statistics(log_returns.flatten())
```



这里，我们把log_return做个一个扯平的操作，说白了，就是把多维数组强行拉成一维。

我们能看到下面这样的效果。

![](https://img-blog.csdn.net/20161214175440946?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

只知道这点，其实我们不知道这是不是正态分布，所以我们就把他画出来和真正的正态分布比较呗。



```python
plt.hist(log_returns.flatten(),bins = 70,normed=True)
x = np.linspace(plt.axis()[0],plt.axis()[1])
plt.plot(x, scs.norm.pdf(x,loc = r/M,scale = sigma/np.sqrt(M)),'r',lw = 2)
plt.show()
sm.qqplot(log_returns.flatten()[::500],line='s')
plt.show()
```
最基本的当然就是在直方图的基础上画一个真正的正态分布的图，但其实业界一般用QQ图来评估两个分布是否一样。


我们都画出来了，大概呢是这样的两个图：

![](https://img-blog.csdn.net/20161214175856378?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20161214175902566?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

当然啦，这都是有眼睛看看的，我们当然跟喜欢量化的。

所以，写个函数来分析：



```python
def normality_test(arr):
    print "Skew of dataset %14.3f" % scs.skew(arr)
    print "Skew test p-value %14.3f" % scs.skewtest(arr)[1]
    print "Kurt of dataset %14.3f" % scs.kurtosis(arr)
    print "Kurt test p-value %14.3f" % scs.kurtosistest(arr)[1]
    print "Norm test p-value %14.3f" % scs.normaltest(arr)[1]
```

```python
normality_test(log_returns.flatten())
```
如此之后，大概能看到这样的结果：


![](https://img-blog.csdn.net/20161214180045770?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这个正态分布的假设检验的零假设当然就是分布是正态分布的。结果我们发现，p-value很大，所以我们不能拒绝原假设。当然，这一块逻辑是存在一定缺陷的，也就是说，我们不能拒绝原假设，但是统计意义是，其实我们并不能直接接受这就是正态分布这一零假设了，所以通常我们都需要配上QQ图来说明。













