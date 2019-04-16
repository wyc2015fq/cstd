# Stata中的单位根检验 - 磐石若水的博客 - CSDN博客





2018年11月07日 00:06:56[磐石若水](https://me.csdn.net/weixin_38421869)阅读数：5157








今天帮连玉君老师的stata连享会翻译了一篇文章，[原文链接](https://blog.stata.com/2016/06/21/unit-root-tests-in-stata/)。第一次尝试用markdown编辑，发现CSDN竟然支持markdown，所以顺手po上来试试效果。以下为正文：

检验序列的平稳性是时间序列分析的关键步骤。时间序列中很多估计量的统计特性都依赖于数据是否平稳。一般意义上，一个(弱)平稳过程的期望、方差和自相关系数应不随时间变化。

然而在大多可观测的时间序列中，趋势项的存在总会使得序列不具有平稳性。趋势项包括确定趋势项和随机趋势项，趋势项的类型决定了我们需要使用什么方法将时间序列转换成平稳序列。比如，含有随机趋势项的单位根过程可以通过差分变得平稳。然而，对实际上含有确定趋势项的序列进行差分则会得到含单位根的移动平均过程。因此，在做转换之前，识别出序列的非平稳性到底是源于确定趋势项还是随机趋势项是非常重要的。

在这篇文章中，我会介绍检验单位根的三个命令。
# 随机趋势

随机趋势的一个简单例子是随机游走过程。

## 随机游走

考虑以下AR(1)过程：
$$(1)y_t=y_{t-1}+\varepsilon_t  \tag{1}​$$

其中$y_t$是被解释变量。误差项$\varepsilon_t$期望为0，方差为$\sigma^2$且独立同分布。如果这个过程的初值为$y_0=0$，则$y_t$可以写成:
$$y_t=\sum_{i=1}^{t}  \varepsilon_t$$

其中等式右端为随机趋势项$\sum_{i=1}^{t}  \varepsilon_t$， $y_t$的期望为0，方差为$\sigma^2$。显然期望是常数，而方差随时间变化。
## 带漂移项的随机游走

向随机游走过程加入一个常数项即可得到带漂移项的随机游走：
$$(2)y_t=\alpha+y_{t-1}+\varepsilon_t \tag{2}$$

其中$\alpha$是常数项。如果这个过程的初值为0，则$y_t$可以写成：
$$y_t=\alpha t+\sum_{i=1}^{t}  \varepsilon_t$$

现在等式右端是一个确定性趋势项$\alpha t​$和一个随机趋势项$\sum_{i=1}^{t}  \varepsilon_t​$的和。$y_t​$的期望为$\alpha t​$，方差为$\sigma^2​$。期望和方差都随着时间$t​$的增加而增加。注意如果漂移项$\alpha​$的值接近于0，则带漂移项的随机游走将非常接近普通的随机游走。

# 确定趋势

考虑如下带有线性确定趋势的随机过程：
$$y_t=\alpha+\delta t+\phi y_{t-1}+\varepsilon_t$$

其中$\delta$是时间$t$的系数，$\phi$是AR过程的参数且$\left| \phi\right|<1​$。注意到带漂移项的随机游走过程也与有线性确定趋势的过程相似，只是前者在确定趋势之外还包含一个随机趋势。
# 非平稳过程绘图

首先，我分别根据随机游走模型和漂移项为0.1的随机游走模型模拟生成数据并画图，代码在附录中提供。
![](https://ws1.sinaimg.cn/large/d47b5479gy1fwyn4wdzd1j20zg0ps42h.jpg)

从上图来看，两个时间序列都没有明显的趋势，并且红线相当于蓝线总是有正向的调整。如果将这两个序列分别画到两张图上，几乎不可能区分出哪个才是带漂移项的随机游走过程。然而，由于两个序列都包含随机趋势，我们仍然可以通过将两个序列差分来获得平稳的时间序列。

类似地，我分别根据漂移项为1的随机游走模型和带有确定性趋势的模型分别模拟数据并画图。
![](https://ws1.sinaimg.cn/large/d47b5479gy1fwyng7nrmnj20zg0psdiv.jpg)

如上图所示，两个序列看起来非常相像。蓝线在一个确定的趋势线上下浮动。红线的随机趋势则在样本开始时缓慢增长，而在样本后期迅速增长。在这种情况下想要转换为平稳的时间序列，就像前面提到的，应用正确的转换方式是非常重要的。
# 单位根检验

单位根检验的原假设是真正的随机过程为随机游走过程$(1)$或者是一个带漂移项的随机游走过程$(2)$。考虑如下的AR(1)模型：
$$y_t=\phi y_{t-1}+\varepsilon_t$$

其中$\varepsilon_t$服从独立同分布的$N(0,\sigma^2)$分布。原假设意味着$\phi=1$，而备择假设意味着$\left| \phi\right| < 1$。

如果$\phi​$确实是1，则随着样本量的扩大，OLS估计值$\hat{\phi}​$相对于平稳序列以一个更快的速度收敛于其真实值1。然而，由于$\hat{\phi}​$的渐近分布不是标准分布，因此$t​$检验在这里并不适用。

此外，在回归中是否包含常数或时间趋势项会导致检验统计量有不同的渐近分布，这也意味着在做这类检验时清楚设定原假设和备择假设是非常重要的。
# ADF检验

在单位根检验的原假设下，真实的过程要么是随机游走，要么是带漂移项的随机游走。对于模型$(3)$，原假设成立意味着$\phi=1$。使用OLS估计模型$(3)$将忽略残差的序列相关性。
$$(3)y_t=\alpha+\delta t+\phi y_{t-1}+\varepsilon_t \tag{3}$$

为了解决这个问题，ADF检验通过将模型$(3)$变形为模型$(4)$的差分形式并检验是否$\beta=0$。
$$(4)\Delta y_t=\alpha+\delta t+\beta \phi y_{t-1}+\sum_{I=1}^{k}\gamma_i \Delta y_{t-i}+\varepsilon_t \tag{4}$$

注意$(4)$可以约束$\alpha$和$\delta$其中一个等于0或同时为0，但是不同的约束会对应不同的检验统计量。Hamilton(1994)[1](#fn1)给出了四种可能情况下检验统计量的分布。

我分别模拟了两列时间序列，其中**yrwd2**是漂移项为1的随机游走过程，**yt**是有线性趋势的模型。我用**dfuller**命令来做ADF检验。我感兴趣的原假设I是**yrwd2**为有漂移项的随机游走过程，其备择假设为**yrwd2**是一个有线性趋势的平稳过程。因此为了检验$(4)$，我会加入**dfuller**命令的选项**trend**。
```
. dfuller yrwd2, trend

Dickey-Fuller test for unit root        Number of obs   =   149

               ---------- Interpolated Dickey-Fuller ---------
                  Test         1% CV      5% CV    10%CV
               Statistic                                   
-------------------------------------------------------------------
 Z(t)           -2.664        -4.024     -3.443   -3.143
-------------------------------------------------------------------
MacKinnon approximate p-value for Z(t) = 0.2511
```

和预期一样，**yrwd2**的检验结果显示不能拒绝该过程为带漂移的随机游走过程的原假设。接下来，我们对**yt**序列也做类似的检验。

```
. dfuller yt, trend

Dickey-Fuller test for unit root       Number of obs   =  149

                       ------- Interpolated Dickey-Fuller ------
                  Test         1% CV      5% CV     10% CV
               Statistic                           
----------------------------------------------------------------
 Z(t)           -5.328        -4.024     -3.443     -3.143
----------------------------------------------------------------
MacKinnon approximate p-value for Z(t) = 0.0000
```

和预期一样，**yt**序列的检验结果拒绝了原假设。

# Phillips–Perron检验

Phillips(1987)[2](#fn2)和Phillips and Perron(1988)[3](#fn3)开发出这个检验主要是为了解决残差项中潜在的序列相关和异方差问题，其检验统计量的渐进分布和临界值与ADF检验相同。

在`stata`中，PP检验的命令**pperron**与**dfuller**的用法类似。使用**pperron**对**yrwd2**和**yt**可以得到与ADF检验类似的结果。

# GLS去势的ADF检验

Elliott et al. (1996)[4](#fn4)提出的GLS-ADF检验与ADF检验类似，只是在对模型$(4)$进行估计之前，需要先对时间序列进行GLS回归。Elliott et al. (1996)[4](#fn4)证明了这个检验比ADF检验表现更好。

为了检验**yrwd2**是否是一个带漂移项的随机游走过程，我使用设定最大4阶滞后的**dfgls**命令对模型$(4)$进行估计：

```
. dfgls yrwd2, maxlag(4)

DF-GLS for yrwd2                                      Number of obs =   145

               DF-GLS tau      1% Critical     5% Critical     10% Critical
  [lags]     Test Statistic        Value           Value            Value
---------------------------------------------------------------------------
    4            -1.404           -3.520          -2.930           -2.643
    3            -1.420           -3.520          -2.942           -2.654
    2            -1.638           -3.520          -2.953           -2.664
    1            -1.644           -3.520          -2.963           -2.673

Opt Lag (Ng-Perron seq t) = 0 [use maxlag(0)]
Min SC   =   3.31175 at lag  1 with RMSE  5.060941
Min MAIC =  3.295598 at lag  1 with RMSE  5.060941
```

注意，与**dfuller**和**pperon**命令不同的是，**dfgls**默认控制了线性时间趋势项。结果仍然显示不能拒绝**yrwd2**是带漂移项的随机游走过程的原假设。

最后，我对**yt**序列做了同样的检验：

```
. dfgls yt, maxlag(4)

DF-GLS for yt                                         Number of obs =   145

               DF-GLS tau      1% Critical     5% Critical     10% Critical
  [lags]     Test Statistic        Value           Value            Value
---------------------------------------------------------------------------
    4            -4.013           -3.520          -2.930           -2.643
    3            -4.154           -3.520          -2.942           -2.654
    2            -4.848           -3.520          -2.953           -2.664
    1            -4.844           -3.520          -2.963           -2.673

Opt Lag (Ng-Perron seq t) = 0 [use maxlag(0)]
Min SC   =  3.302146 at lag  1 with RMSE  5.036697
Min MAIC =  3.638026 at lag  1 with RMSE  5.036697
```

与期望一致，我们拒绝了**yt**序列为带漂移项的随机游走过程的原假设。

# 结论

在这篇文章中，我讨论了时间序列非平稳主要是由于存在随机趋势项或确定趋势项或二者都有，同时还使用介绍了如何使用**dfuller**、**pperron**和**dfgls**命令检验是否存在单位根。

# 附录

这是生成随机游走过程、带漂移项的随机游走过程以及带线性趋势过程的代码：

```
clear all
set seed 2016
local T = 200
set obs `T'
gen time = _n
label var time "Time"
tsset time
gen eps = rnormal(0,5)
 
/*Random walk*/
gen yrw = eps in 1
replace yrw = l.yrw + eps in 2/l
 
/*Random walk with drift*/
gen yrwd1 = 0.1 + eps in 1
replace yrwd1 = 0.1 + l.yrwd1 + eps in 2/l
 
/*Random walk with drift*/
gen yrwd2 = 1 + eps in 1
replace yrwd2 = 1 + l.yrwd2 + eps in 2/l
 
/*Stationary around a time trend model*/
gen yt = 0.5 + 0.1*time + eps in 1
replace yt = 0.5 + 0.1*time +0.8*l.yt+ eps in 2/l
drop in 1/50
 
tsline yrw yrwd1, title("Stochastic trend")          ///
        legend(label(1 "Random walk")                ///
        label(2 "Random walk with drift"))
tsline yt yrwd2,                                     ///
        legend(label(1 "Deterministic time trend")   ///
        label(2 "Random walk with drift"))           ///
        title("Stochastic and deterministic trend")
```
- 
Hamilton, J. D. 1994. Time Series Analysis. Princeton: Princeton University Press. [??](#fnref1)

- 
Phillips, P. C. B. 1987. Time series regression with a unit root. Econometrica 55: 277–301. [??](#fnref2)

- 
Phillips, P. C. B., and P. Perron. 1988. Testing for a unit root in time series regression. Biometrika 75: 335–346. [??](#fnref3)

- 
Elliott, G. R., T. J. Rothenberg, and J. H. Stock. 1996. Efficient tests for an autoregressive unit root. Econometrica 64: 813–836. [??](#fnref4)[??](#fnref4:1)











