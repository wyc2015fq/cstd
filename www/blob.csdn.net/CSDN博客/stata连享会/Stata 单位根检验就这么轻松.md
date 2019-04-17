# Stata: 单位根检验就这么轻松 - stata连享会 - CSDN博客





2018年12月05日 14:54:15[arlionn](https://me.csdn.net/arlionn)阅读数：940










> 
作者：许梦洁 (编译)  ([知乎](https://zhuanlan.zhihu.com/arlion) | [简书](http://www.jianshu.com/u/69a30474ef33) | [码云](https://gitee.com/arlionn))


### Stata连享会 [「Stata 现场班报名中……」](https://gitee.com/arlionn/stata_training/blob/master/README.md)


Source: [Ashish Rajbhandari](https://blog.stata.com/author/arajbhandari/) → [Unit-root tests in Stata](https://blog.stata.com/2016/06/21/unit-root-tests-in-stata/)




### 文章目录
- [Stata连享会 [「Stata 现场班报名中……」](https://gitee.com/arlionn/stata_training/blob/master/README.md)](#Stata_Stata_httpsgiteecomarlionnstata_trainingblobmasterREADMEmd_6)
- [](#emsp_13)
- [1. 随机趋势](#1__24)
- [1.1 随机游走](#11__27)
- [1.2 带漂移项的随机游走](#12__38)
- [2. 确定趋势](#2__50)
- [3. 非平稳过程绘图](#3__60)
- [4. 单位根检验](#4__74)
- [4.1 ADF 检验](#41_ADF__88)
- [4.2 Phillips–Perron 检验](#42_PhillipsPerron__136)
- [4.3 GLS 去势的 ADF 检验](#43_GLS__ADF__142)
- [结论](#_187)
- [附录](#_192)
- [参考文献](#_231)
- [关于我们](#_243)
- [联系我们](#_250)
- [往期精彩推文](#_257)




检验序列的平稳性是时间序列分析的关键步骤。时间序列中很多估计量的统计特性都依赖于数据是否平稳。一般意义上，一个 (弱) 平稳过程的期望、方差和自相关系数应不随时间变化。

然而，在大多可观测的时间序列中，趋势项的存在总会使得序列不具有平稳性。

趋势项包括确定趋势项和随机趋势项，趋势项的类型决定了我们需要使用什么方法将时间序列转换成平稳序列。比如，含有随机趋势项的单位根过程可以通过差分变得平稳。然而，对实际上含有确定趋势项的序列进行差分则会得到含单位根的移动平均过程。因此，在做转换之前，识别出序列的非平稳性到底是源于确定趋势项还是随机趋势项是非常重要的。

在这篇文章中，我会介绍检验单位根的三个命令。

## 1. 随机趋势

随机趋势的一个简单例子是随机游走过程。

### 1.1 随机游走

考虑以下 AR(1) 过程：

$$(1)y_t=y_{t-1}+\varepsilon_t  \tag{1}$$

其中，$y_t$ 是被解释变量。误差项 $\varepsilon_t$ 期望为 0，方差为 $\sigma^2$ 且独立同分布。如果这个过程的初值为 $y_0=0$，则 $y_t$ 可以写成:

$$y_t=\sum_{i=1}^{t}  \varepsilon_t$$

其中，等式右端为随机趋势项 $\sum_{i=1}^{t}  \varepsilon_t$，$ y_t$ 的期望为 0，方差为 $\sigma^2$。显然，期望是常数，而方差随时间变化。

### 1.2 带漂移项的随机游走

向随机游走过程加入一个常数项即可得到带漂移项的随机游走：
$$(2)y_t=\alpha+y_{t-1}+\varepsilon_t    \tag{2}$$

其中，$\alpha$ 是常数项。如果这个过程的初值为 0，则 $y_t$ 可以写成：
$$y_t=\alpha t+\sum_{i=1}^{t}  \varepsilon_t$$

现在等式右端是一个确定性趋势项 $\alpha t$ 和一个随机趋势项 $\sum_{i=1}^{t}  \varepsilon_t$ 的和。$y_t$ 的期望为 $\alpha t$，方差为 $\sigma^2$。期望和方差都随着时间 $t$ 的增加而增加。注意，如果漂移项 $\alpha$ 的值接近于 0，则带漂移项的随机游走将非常接近普通的随机游走。

## 2. 确定趋势

考虑如下带有线性确定趋势的随机过程：

$$y_t=\alpha+\delta t+\phi y_{t-1}+\varepsilon_t$$

其中，$\delta$ 是时间 $t$ 的系数，$\phi$ 是 AR 过程的参数且 $\left| \phi\right|<1$。注意，带漂移项的随机游走过程也与有线性确定趋势的过程相似，只是前者在确定趋势之外还包含一个随机趋势。

## 3. 非平稳过程绘图

首先，我分别根据随机游走模型和漂移项为 0.1 的随机游走模型模拟生成数据并画图，代码在附录中提供。

![随机游走过程-Stata连享会 (公众号：StataChina)](http://upload-images.jianshu.io/upload_images/7692714-8479fa3771ff3a12.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

从上图来看，两个时间序列都没有明显的趋势，并且红线相当于蓝线总是有正向的调整。如果将这两个序列分别画到两张图上，几乎不可能区分出哪个才是带漂移项的随机游走过程。然而，由于两个序列都包含随机趋势，我们仍然可以通过将两个序列差分来获得平稳的时间序列。

类似地，我分别根据漂移项为 1 的随机游走模型和带有确定性趋势的模型分别模拟数据并画图。

![带漂移项和趋势项的随机游走过程-Stata连享会 (公众号：StataChina)](http://upload-images.jianshu.io/upload_images/7692714-0ae144de70be1da4.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

如上图所示，两个序列看起来非常相像。蓝线在一个确定的趋势线上下浮动。红线的随机趋势则在样本开始时缓慢增长，而在样本后期迅速增长。在这种情况下想要转换为平稳的时间序列，就像前面提到的，应用正确的转换方式是非常重要的。

## 4. 单位根检验

单位根检验的原假设是真正的随机过程为随机游走过程 $(1)$ 或者是一个带漂移项的随机游走过程 $(2)$。考虑如下的 AR(1) 模型：

$$y_t=\phi y_{t-1}+\varepsilon_t$$

其中，$\varepsilon_t$ 服从独立同分布的 $N(0,\sigma^2)$ 分布。原假设意味着$\phi=1$，而备择假设意味着 $\left| \phi\right|<1$。

如果 $\phi$ 确实是 1，则随着样本量的扩大，OLS 估计值 $\hat{\phi}$ 会相对于平稳序列以一个更快的速度收敛于其真实值 1。然而，由于 $\hat{\phi}$ 的渐近分布不是标准分布，传统的 *t* 检验在这里并不适用。

此外，在回归中是否包含常数或时间趋势项会导致检验统计量有不同的渐近分布，这也意味着在做这类检验时清楚设定原假设和备择假设是非常重要的。

## 4.1 ADF 检验

在单位根检验的原假设下，真实的过程要么是随机游走，要么是带漂移项的随机游走。对于模型 $(3)$，原假设成立意味着 $\phi=1$。使用 OLS 估计模型 $(3)$ 将忽略残差的序列相关性。

$$(3)y_t=\alpha+\delta t+\phi y_{t-1}+\varepsilon_t   \tag{3}$$

为了解决这个问题，ADF检验通过将模型 $(3)$ 变形为模型 $(4)$ 的差分形式并检验是否 $\beta=0$。

$$(4)\Delta y_t=\alpha+\delta t+\beta \phi y_{t-1}+\sum_{I=1}^{k}\gamma_i \Delta y_{t-i}+\varepsilon_t   \tag{4}$$

注意，$(4)$ 可以约束 $\alpha$ 和 $\delta$ 其中一个等于 0 或同时为 0，但是不同的约束会对应不同的检验统计量。Hamilton(1994)[1](#fn1) 给出了四种可能情况下检验统计量的分布。

我分别模拟了两列时间序列，其中 **yrwd2** 是漂移项为 1 的随机游走过程，**yt** 是有线性趋势的模型。这里用 **dfuller** 命令来做 ADF 检验。我们感兴趣的原**假设 1** 是：**yrwd2**为有漂移项的随机游走过程。其备择假设为：**yrwd2** 是一个有线性趋势的平稳过程。

因此，为了检验 $(4)$，我们会在 `dfuller` 命令中加入选项 `trend`。

```
. dfuller yrwd2, trend

Dickey-Fuller test for unit root       Number of obs = 149

            --------- Interpolated Dickey-Fuller ---------
          Test     1% Critical  5% Critical   10% Critical
       Statistic       Value        Value          Value
----------------------------------------------------------
 Z(t)     -2.664        -4.024       -3.443         -3.143
----------------------------------------------------------
MacKinnon approximate p-value for Z(t) = 0.2511
```

与预期一致，**yrwd2** 的检验结果显示：不能拒绝该过程为带漂移的随机游走过程的原假设。接下来，我们对 **yt** 序列也做类似的检验。

```
. dfuller yt, trend

Dickey-Fuller test for unit root        Number of obs = 149

            ---------- Interpolated Dickey-Fuller ---------
          Test      1% Critical   5% Critical  10% Critical
       Statistic        Value          Value          Value
-----------------------------------------------------------
 Z(t)     -5.328         -4.024        -3.443        -3.143
-----------------------------------------------------------
MacKinnon approximate p-value for Z(t) = 0.0000
```

此时，**yt** 序列的检验结果拒绝了原假设。

### 4.2 Phillips–Perron 检验

Phillips(1987)[2](#fn2) 和 Phillips and Perron(1988)[3](#fn3) 开发出这个检验主要是为了解决残差项中潜在的序列相关和异方差问题，其检验统计量的渐进分布和临界值与 ADF 检验相同。

在 Stata 中，PP 检验的命令 `pperron` 与 `dfuller` 的用法相似。使用 `pperron` 对**yrwd2** 和 **yt** 执行检验可以得到与 ADF 检验类似的结果。

### 4.3 GLS 去势的 ADF 检验

Elliott et al. (1996)[4](#fn4) 提出的 GLS-ADF 检验与 ADF 检验类似，只是在对模型 $(4)$ 进行估计之前，需要先对时间序列进行GLS回归。Elliott et al. (1996)[4](#fn4) 证明了这个检验比 ADF 检验表现更好。

为了检验 **yrwd2** 是否是一个带漂移项的随机游走过程，我使用设定最大 4 阶滞后的 `dfgls` 命令对模型 $(4)$ 进行估计：

```
. dfgls yrwd2, maxlag(4)

DF-GLS for yrwd2                            Number of obs = 145

            DF-GLS tau   1% Critical  5% Critical  10% Critical
  [lags]  Test Statistic     Value        Value         Value
---------------------------------------------------------------
    4         -1.404        -3.520       -2.930        -2.643
    3         -1.420        -3.520       -2.942        -2.654
    2         -1.638        -3.520       -2.953        -2.664
    1         -1.644        -3.520       -2.963        -2.673

Opt Lag (Ng-Perron seq t) = 0 [use maxlag(0)]
Min SC   =   3.31175 at lag  1 with RMSE  5.060941
Min MAIC =  3.295598 at lag  1 with RMSE  5.060941
```

注意，与 `dfuller` 和 `pperon` 命令不同的是，`dfgls` 默认控制了线性时间趋势项。结果仍然显示不能拒绝 **yrwd2** 是带漂移项的随机游走过程的原假设。

最后，我对 **yt** 序列做了同样的检验：

```
. dfgls yt, maxlag(4)

DF-GLS for yt                               Number of obs = 145

           DF-GLS tau    1% Critical  5% Critical  10% Critical
  [lags] Test Statistic      Value        Value         Value
---------------------------------------------------------------
    4        -4.013         -3.520       -2.930        -2.643
    3        -4.154         -3.520       -2.942        -2.654
    2        -4.848         -3.520       -2.953        -2.664
    1        -4.844         -3.520       -2.963        -2.673

Opt Lag (Ng-Perron seq t) = 0 [use maxlag(0)]
Min SC   =  3.302146 at lag  1 with RMSE  5.036697
Min MAIC =  3.638026 at lag  1 with RMSE  5.036697
```

与期望一致，我们拒绝了 **yt** 序列为带漂移项的随机游走过程的原假设。

## 结论

在这篇文章中，我讨论了时间序列非平稳主要是由于存在随机趋势项或确定趋势项或二者都有，同时还使用介绍了如何使用 `dfuller`、`pperron` 和`dfgls` 命令检验是否存在单位根。

## 附录

这是生成随机游走过程、带漂移项的随机游走过程以及带线性趋势过程的 Stata 代码：

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

## 参考文献

> 
#### 关于我们

- 【**Stata 连享会(公众号：StataChina)**】由中山大学连玉君老师团队创办，旨在定期与大家分享 Stata 应用的各种经验和技巧。
- 公众号推文同步发布于 [CSDN-Stata连享会](https://blog.csdn.net/arlionn) 、[简书-Stata连享会](http://www.jianshu.com/u/69a30474ef33) 和 [知乎-连玉君Stata专栏](https://www.zhihu.com/people/arlionn)。可以在上述网站中搜索关键词`Stata`或`Stata连享会`后关注我们。
- 点击推文底部【阅读原文】可以查看推文中的链接并下载相关资料。
- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)

> 
#### 联系我们

- **欢迎赐稿：** 欢迎将您的文章或笔记投稿至`Stata连享会(公众号: StataChina)`，我们会保留您的署名；录用稿件达`五篇`以上，即可**免费**获得 Stata 现场培训 (初级或高级选其一) 资格。
- **意见和资料：** 欢迎您的宝贵意见，您也可以来信索取推文中提及的程序和数据。
- **招募英才：** 欢迎加入我们的团队，一起学习 Stata。合作编辑或撰写稿件五篇以上，即可**免费**获得 Stata 现场培训 (初级或高级选其一) 资格。
- **联系邮件：**[StataChina@163.com](mailto:StataChina@163.com)

> 
#### 往期精彩推文

- [Stata连享会推文列表1](https://www.jianshu.com/p/de82fdc2c18a)
- [Stata连享会推文列表2](https://github.com/arlionn/Stata_Blogs/blob/master/README.md)
- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)
![](https://upload-images.jianshu.io/upload_images/7692714-8b1fb0b5068487af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![欢迎加入Stata连享会(公众号: StataChina)](http://upload-images.jianshu.io/upload_images/7692714-44ec99a3b12388e8.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 
Hamilton, J. D. 1994. Time Series Analysis. Princeton: Princeton University Press. [↩︎](#fnref1)

- 
Phillips, P. C. B. 1987. Time series regression with a unit root. Econometrica 55: 277–301. [↩︎](#fnref2)

- 
Phillips, P. C. B., and P. Perron. 1988. Testing for a unit root in time series regression. Biometrika 75: 335–346. [↩︎](#fnref3)

- 
Elliott, G. R., T. J. Rothenberg, and J. H. Stock. 1996. Efficient tests for an autoregressive unit root. Econometrica 64: 813–836. [↩︎](#fnref4)[↩︎](#fnref4:1)






