# Time Series Analysis (TSA) in Python-Linear Models to GRACH 笔记（三） - 一个跳popping的quant的博客 - CSDN博客





2019年01月23日 14:44:47[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：692
所属专栏：[时间序列分析](https://blog.csdn.net/column/details/34308.html)









以下内容主要来自：

[Time Series Analysis (TSA) in Python - Linear Models to GARCH](http://www.blackarbs.com/blog/time-series-analysis-in-python-linear-models-to-garch/11/1/2016#WNandRW=)

**目录**

[Autoregressive Models - AR(p)](#Autoregressive%20Models%20-%20AR(p))

[Moving Average Models - MA(q)](#Moving%20Average%20Models%20-%20MA(q))

# Autoregressive Models - AR(p)

**独立的因变量可以由它的一个或者多个滞后项（lag）表示**时，这样的模型称为**自回归**模型。它的式子为：

![](https://img-blog.csdnimg.cn/20190122152814152.png)

 当我们需要描述模型的阶数时，**AR模型中阶数 p 代表了滞后项的数量**，举个例子，一个二阶的AR模型中，就有两个滞后项：

![](https://img-blog.csdnimg.cn/20190122153333534.png)

这里，a 代表了系数，w 是白噪声项。a 在 AR 模型中不能为零。注意这一点：一个 AR(1) 伴随着一个 a ，当 a 为1时就是一个随机漫步模型，因此是不稳定的：

![](https://img-blog.csdnimg.cn/20190122153708894.png)

下面我们来模拟一下AR(1)并且将a设置为0.6：

```python
# Simulate an AR(1) process with alpha = 0.6

np.random.seed(1)
n_samples = int(1000)
a = 0.6
x = w = np.random.normal(size=n_samples)

for t in range(n_samples):
    x[t] = a*x[t-1] + w[t]
    
_ = tsplot(x, lags=lags)
```



![](https://img-blog.csdnimg.cn/20190122153911775.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)

 正如我们所预期的，AR(1) 模型是正态分布的。滞后值之间存在明显的序列相关性，尤其是滞后为1时，从PACF图中就可以证明这一点。

**补充一点ACF和PACF的意义与理解：**

ACF是X从 t-k 项到 t 项的影响程度，其中包含了中间的数据的影响，PACF是指剔除了X从 t-k 项到 t 项中间的 k - 1 项之后，t-k项到 t 项的影响程度。所以也可以看出当 k = 1时，ACF和PACF的值应该是一样的，k 不等于 1 时，它们二者的值应该不一样。

现在，我们通过 python 中的 **statsmodels **来模拟 AR{p} 模型。首先我们用 AR 模型去拟合我们我的数据，然后返回估计的 a 系数。然后我们用 statsmodels 中的函数 **select_order() **来看看是选择了正确的阶数。如果 AR 模型是正确的，那么它的 a 系数就会接近 0.6 阶数就会是 1。

```python
# Fit an AR(p) model to simulated AR(1) model with alpha = 0.6

mdl = smt.AR(x).fit(maxlag=30, ic='aic', trend='nc')
%time est_order = smt.AR(x).select_order(
    maxlag=30, ic='aic', trend='nc')

true_order = 1
p('\nalpha estimate: {:3.5f} | best lag order = {}'
  .format(mdl.params[0], est_order))
p('\ntrue alpha = {} | true order = {}'
  .format(a, true_order))
```



![](https://img-blog.csdnimg.cn/20190122155536191.png)

 看来我们能够恢复我们模拟数据的参数。让我们用 a1 = 0.666 和 a2 = -0.333 模拟一个 AR(2) 过程。为了实现这个，我们可以利用 statsmodels 中的** arma_generate_samples() **函数。这个函数可以让我们模拟一个指定阶数的 AR 模型。注意在使用这个函数之前，有一些额外步骤需要执行，这是根据 python 版本的不同特性决定的。

```python
# Simulate an AR(2) process

n = int(1000)
alphas = np.array([.666, -.333])
betas = np.array([0.])

# Python requires us to specify the zero-lag value which is 1
# Also note that the alphas for the AR model must be negated
# We also set the betas for the MA equal to 0 for an AR(p) model
# For more information see the examples at statsmodels.org
ar = np.r_[1, -alphas]
ma = np.r_[1, betas]

ar2 = smt.arma_generate_sample(ar=ar, ma=ma, nsample=n) 
_ = tsplot(ar2, lags=lags)
```



![](https://img-blog.csdnimg.cn/20190122160313500.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)

 让我们看看是否回复了正确的参数：

```python
# Fit an AR(p) model to simulated AR(2) process

max_lag = 10
mdl = smt.AR(ar2).fit(maxlag=max_lag, ic='aic', trend='nc')
est_order = smt.AR(ar2).select_order(
    maxlag=max_lag, ic='aic', trend='nc')

true_order = 2
p('\ncoef estimate: {:3.4f} {:3.4f} | best lag order = {}'
  .format(mdl.params[0],mdl.params[1], est_order))
p('\ntrue coefs = {} | true order = {}'
  .format([.666,-.333], true_order))
  
# coef estimate: 0.6291 -0.3196 | best lag order = 2
# true coefs = [0.666, -0.333] | true order = 2
```

并不差，现在让我们看看 AR(p) 模型是否可以拟合 MSFT 公司股价的对数收益。

![](https://img-blog.csdnimg.cn/2019012216123111.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)

```
# Select best lag order for MSFT returns
# lrets = np.log(data/data.shift(1)).dropna()

max_lag = 30
mdl = smt.AR(lrets.MSFT).fit(maxlag=max_lag, ic='aic', trend='nc')
est_order = smt.AR(lrets.MSFT).select_order(
    maxlag=max_lag, ic='aic', trend='nc')

p('best estimated lag order = {}'.format(est_order))

# best estimated lag order = 23
```

 最好的阶数是 23，也就是说有 23 个参数。任何模型有这么多参数在实际当中都不可能有用。显然会有比这个模型更复杂的模型可以来解释。

# Moving Average Models - MA(q)

MA(q) 模型和 AR(p) 模型十分类似，不同的地方在于 MA(q) 模型是一个对过去白噪声误差项的线性组合，而 AR(p) 模型则是对过去观测值的线性组合。MA 模型的目的在于我们可以通过拟合一个模型中的误差项，来直接观察到误差过程中的 shocks ，而在 AR(p) 模型中，这些 shocks 是通过对过去观测值用 ACF 来间接得到的。MA(q) 的公式如下：

![](https://img-blog.csdnimg.cn/20190122164104302.png)

 其中 w 是白噪声，且它的期望 E(w) = 0，让我们用 b = 0.6 来模拟 MA 的一阶模型。

```python
# Simulate an MA(1) process

n = int(1000)

# set the AR(p) alphas equal to 0
alphas = np.array([0.])
betas = np.array([0.6])

# add zero-lag and negate alphas
ar = np.r_[1, -alphas]
ma = np.r_[1, betas]

ma1 = smt.arma_generate_sample(ar=ar, ma=ma, nsample=n) 
_ = tsplot(ma1, lags=30)
```



![](https://img-blog.csdnimg.cn/20190122164502900.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)



从ACF中可以看出，滞后1是很明显的，所以说MA(1)模型可能会适合我们的模拟数据。

不论如何，我们可以尝试用一个 MA(1) 的模型来拟合我们的数据。我们同样可以用 statsmodels 中的ARMA()函数来确定阶数。我们调用 fit() 方法来返回模型的输出。

```
# Fit the MA(1) model to our simulated time series
# Specify ARMA model with order (p, q)

max_lag = 30
mdl = smt.ARMA(ma1, order=(0, 1)).fit(
    maxlag=max_lag, method='mle', trend='nc')
p(mdl.summary())
```



![](https://img-blog.csdnimg.cn/20190122173923886.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)

 模型可以估计出滞后项系数为0.58，这很接近0.6。另外，我们95%的置信区间中包含了真值。

下面我们来模拟一个MA(3)的序列，然后用我们ARMA函数来拟合一个三阶的MA模型，然后看是否可以回复正确的滞后系数（0.6，0.4，0.2)。

```python
# Simulate MA(3) process with betas 0.6, 0.4, 0.2

n = int(1000)
alphas = np.array([0.])
betas = np.array([0.6, 0.4, 0.2])
ar = np.r_[1, -alphas]
ma = np.r_[1, betas]

ma3 = smt.arma_generate_sample(ar=ar, ma=ma, nsample=n)
_ = tsplot(ma3, lags=30)
```



![](https://img-blog.csdnimg.cn/20190122174732524.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)



```python
# Fit MA(3) model to simulated time series

max_lag = 30
mdl = smt.ARMA(ma3, order=(0, 3)).fit(
    maxlag=max_lag, method='mle', trend='nc')
p(mdl.summary())
```



![](https://img-blog.csdnimg.cn/20190122174930489.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)

同样在95%的置信区间中包含了这三个真值。现在让我们用一个 MA(3) 来拟合SPY的对数收益，记住我们不知道真正的参数值。

```python
# Fit MA(3) to SPY returns

max_lag = 30
Y = lrets.SPY
mdl = smt.ARMA(Y, order=(0, 3)).fit(
    maxlag=max_lag, method='mle', trend='nc')
p(mdl.summary())
_ = tsplot(mdl.resid, lags=max_lag)
```



![](https://img-blog.csdnimg.cn/2019012217542321.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)

让我们一起看一下模型的残差：

![](https://img-blog.csdnimg.cn/20190122175630130.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)

 效果还不错，但是ACF中的一些滞后，尤其是在5，16和18处特别突出。它们可能是抽样误差，但是由于 heavy tail 让我认为这并不是预测SPY收益的最佳模型。

----------------------------------------------------------------------------------------------------------------------------------------

补充一点关于根据ACF和PACF来选择合适AR或者MA模型来拟合数据的方法：

首先明确两个概念：截尾和拖尾。

**截尾**就是在图中的某阶之后，系数都变为 0 。**拖尾**就是每一阶都在衰减，但是不都为 0 。

如果自相关图拖尾，偏相关截尾，则用 AR 算法

如果自相关截尾，偏相关拖尾，则用 MA 算法

如果自相关和偏相关都是拖尾，则用 ARMA 算法



[Time Series Analysis (TSA) in Python-Linear Models to GRACH 笔记（四）](https://blog.csdn.net/FrankieHello/article/details/86613337)



