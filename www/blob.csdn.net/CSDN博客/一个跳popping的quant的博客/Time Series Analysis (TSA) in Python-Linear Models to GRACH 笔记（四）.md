# Time Series Analysis (TSA) in Python-Linear Models to GRACH 笔记（四） - 一个跳popping的quant的博客 - CSDN博客





2019年01月24日 13:15:48[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：680
所属专栏：[时间序列分析](https://blog.csdn.net/column/details/34308.html)









以下内容主要来自：

[Time Series Analysis (TSA) in Python - Linear Models to GARCH](http://www.blackarbs.com/blog/time-series-analysis-in-python-linear-models-to-garch/11/1/2016#WNandRW=)

**目录**

[Autoregressive Moving Average Models - ARMA(p, q)](#yui_3_17_2_1_1548233153632_1863)

[Autoregressive Integrated Moving Average Models - ARIMA(p, d, q)](#Autoregressive%20Integrated%20Moving%20Average%20Models%20-%20ARIMA(p%2C%20d%2C%20q))

# Autoregressive Moving Average Models - ARMA(p, q)

顾名思义，ARMA（自回归移动平均模型）只是 AR(p) 和 MA(q) 模型的合并。让我们从量化金融的角度来看一下，这些模型对我们的意义：
- AR(p) 模型试图去捕捉或者解释金融交易市场中的动量（momentum）和均值回复效应（mean reversion）。
- MA(q) 模型试图去捕捉或者解释白噪声项中观测到的冲击效应（shock effects）。这些冲击效应被认为是一些意料之外的事件，它们会影响观测过程。例如恐怖袭击。

ARMA模型的缺点在于**它忽略了大多数金融时间序列中存在的波动聚集效应**。

波动聚集性（volatility clustering）是指在某个时间段上波动率比较高，而其他的时间段上波动率较小。

ARMA模型的公式：



![](https://img-blog.csdnimg.cn/20190123173228146.png)

 让我们模拟生成一个 ARMA(2, 2) 的过程，然后去拟合 ARMA(2, 2)模型，看是否能正确估计出这些参数。设置 a = 0.5和-0.25，b = 0.5 和 -0.3。

```python
# Simulate an ARMA(2, 2) model with alphas=[0.5,-0.25] and betas=[0.5,-0.3]
max_lag = 30

n = int(5000) # lots of samples to help estimates
burn = int(n/10) # number of samples to discard before fit

alphas = np.array([0.5, -0.25])
betas = np.array([0.5, -0.3])
ar = np.r_[1, -alphas]
ma = np.r_[1, betas]

arma22 = smt.arma_generate_sample(ar=ar, ma=ma, nsample=n, burnin=burn)
_ = tsplot(arma22, lags=max_lag)

mdl = smt.ARMA(arma22, order=(2, 2)).fit(
    maxlag=max_lag, method='mle', trend='nc', burnin=burn)
p(mdl.summary())
```



![](https://img-blog.csdnimg.cn/20190123173718992.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)





![](https://img-blog.csdnimg.cn/20190123173803425.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)

 这个模型正确回复了我们的参数，而且正确的参数值都在95%的置信区间里面。

下面我们模拟生成一段 ARMA(3, 2)的样本数据，然后用 ARMA 模型去拟合样本数据，p, q 值的选择则通过循环遍历来得到最好的组合，我们最好的 p, q 值组合的选择是根据哪个模型生成最小的 AIC (Akaike Information Criterion)来决定的。

AIC 中文名为赤池信息量准则，是评估统计模型的复杂度和衡量模型拟合优良性的一种标准，赤池信息量是建立在信息熵的基础上的。赤池信息量准则的方法是**寻找最好解释数据并且包含自由参数最少**的模型。[维基百科](https://zh.wikipedia.org/wiki/%E8%B5%A4%E6%B1%A0%E4%BF%A1%E6%81%AF%E9%87%8F%E5%87%86%E5%88%99)

```python
# Simulate an ARMA(3, 2) model with alphas=[0.5,-0.25,0.4] and betas=[0.5,-0.3]

max_lag = 30

n = int(5000)
burn = 2000

alphas = np.array([0.5, -0.25, 0.4])
betas = np.array([0.5, -0.3])

ar = np.r_[1, -alphas]
ma = np.r_[1, betas]

arma32 = smt.arma_generate_sample(ar=ar, ma=ma, nsample=n, burnin=burn)
_ = tsplot(arma32, lags=max_lag)

# pick best order by aic 
# smallest aic value wins
best_aic = np.inf 
best_order = None
best_mdl = None

rng = range(5)
for i in rng:
    for j in rng:
        try:
            tmp_mdl = smt.ARMA(arma32, order=(i, j)).fit(method='mle', trend='nc')
            tmp_aic = tmp_mdl.aic
            if tmp_aic < best_aic:
                best_aic = tmp_aic
                best_order = (i, j)
                best_mdl = tmp_mdl
        except: continue


p('aic: {:6.5f} | order: {}'.format(best_aic, best_order))

# aic: 14108.27213 | order: (3, 2)
```

正确的阶数恢复了。下面是我们的没有经过模型拟合的时间序列。



![](https://img-blog.csdnimg.cn/201901241148439.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)



![](https://img-blog.csdnimg.cn/20190124114904868.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)

 我们注意到正确的阶数被选择出了，模型正确估计了我们的参数。然而注意到，MA.L1.y 的系数0.5几乎超出了95%的置信区间。

下面我们观察一下模型的残差，很明显这是一个白噪声过程，因此最合适的模型已经解释了这个数据。

![](https://img-blog.csdnimg.cn/20190124115559262.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)

下面我们用 ARMA模型去拟合 SPY 的收益。下面是拟合之前的数据：

![](https://img-blog.csdnimg.cn/2019012411581660.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)

通过循环遍历来选择最合适的 p，q 组合。

```python
# Fit ARMA model to SPY returns

best_aic = np.inf 
best_order = None
best_mdl = None

rng = range(5) # [0,1,2,3,4,5]
for i in rng:
    for j in rng:
        try:
            tmp_mdl = smt.ARMA(lrets['SPY'], order=(i, j)).fit(
                method='mle', trend='nc'
            )
            tmp_aic = tmp_mdl.aic
            if tmp_aic < best_aic:
                best_aic = tmp_aic
                best_order = (i, j)
                best_mdl = tmp_mdl
        except: continue


p('aic: {:6.5f} | order: {}'.format(best_aic, best_order))

# aic: -11518.22902 | order: (4, 4)
```

 我们画出最合适的模型 ARMA(4,4) 的残差：

![](https://img-blog.csdnimg.cn/20190124120319877.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)

ACF和PACF并没有显示出明显的相关性。QQ和概率图显示近似的正态分布并伴随着 heavy tails。然而，这个模型的残差图并不像白噪声。注意高亮区域具有明显的 conditional heteroskedasticity (条件异方差，conditional volatility)，但是模型并没有捕捉到。

# Autoregressive Integrated Moving Average Models - ARIMA(p, d, q)

ARIMA（差分自回归移动平均模型）是一个对 ARMA 模型的延伸。正如我们之前提到的，很多时间序列都是不稳定的，但是可以通过差分来使它们变为稳定的。例如，我们将高斯随机漫步时间序列进行一阶差分时，得到的结果证实为白噪声。换句话说，我们将不稳定的随机漫步通过一阶差分转换为了稳定的白噪声。

不需要对 ARIMA(p, d, q) 等式做太多深究，只要知道 **d 指的是对时间序列差分的次数**就可以了。注意一点：在 python 中，如果我们要进行多次差分，那么必须得用 **np.diff() **来实现，pandas 中的 pd.diff() 只能实现一次差分，没有实现时间序列分析中的递归差分。

下面的例子，我们还是通过循环遍历来选择合适的 p, d, q 的最佳组合来拟合SPY的收益数据。同样还是用 AIC 指标来衡量每个模型，最低的 AIC 的模型将被选中。

```python
# Fit ARIMA(p, d, q) model to SPY Returns
# pick best order and final model based on aic

best_aic = np.inf 
best_order = None
best_mdl = None

pq_rng = range(5) # [0,1,2,3,4]
d_rng = range(2) # [0,1]
for i in pq_rng:
    for d in d_rng:
        for j in pq_rng:
            try:
                tmp_mdl = smt.ARIMA(lrets.SPY, order=(i,d,j)).fit(method='mle', trend='nc')
                tmp_aic = tmp_mdl.aic
                if tmp_aic < best_aic:
                    best_aic = tmp_aic
                    best_order = (i, d, j)
                    best_mdl = tmp_mdl
            except: continue


p('aic: {:6.5f} | order: {}'.format(best_aic, best_order))
# aic: -11518.22902 | order: (4, 0, 4)

# ARIMA model resid plot
_ = tsplot(best_mdl.resid, lags=30)
```

 并不惊讶最好的模型进行的差分次数是 0 ，回忆一下，我们已经对股票收益的对数价格进行过一次一阶差分了。下面，我们画出了模型的残差。这个结果本质上和 ARMA(4, 4) 的效果是一样的。明显 ARIMA 模型也没有解释序列中的条件波动。



![](https://img-blog.csdnimg.cn/20190124125219756.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)

现在我们已经累积了足够的知识来做一个简单的预测了。现在我们用模型的 **forecast()** 方法，它有两个参数，一个是要预测的时间步长，另一个是一个参数表示置信区间，默认是 95 % 的置信区间，如果是 99%，那就设置为 0.01。

```python
# Create a 21 day forecast of SPY returns with 95%, 99% CI
n_steps = 21

f, err95, ci95 = best_mdl.forecast(steps=n_steps) # 95% CI
_, err99, ci99 = best_mdl.forecast(steps=n_steps, alpha=0.01) # 99% CI

idx = pd.date_range(data.index[-1], periods=n_steps, freq='D')
fc_95 = pd.DataFrame(np.column_stack([f, ci95]), 
                     index=idx, columns=['forecast', 'lower_ci_95', 'upper_ci_95'])
fc_99 = pd.DataFrame(np.column_stack([ci99]), 
                     index=idx, columns=['lower_ci_99', 'upper_ci_99'])
fc_all = fc_95.combine_first(fc_99)
fc_all.head()
```



![](https://img-blog.csdnimg.cn/20190124131359441.png)

```python
# Plot 21 day forecast for SPY returns

plt.style.use('bmh')
fig = plt.figure(figsize=(9,7))
ax = plt.gca()

ts = lrets.SPY.iloc[-500:].copy()
ts.plot(ax=ax, label='Spy Returns')
# in sample prediction
pred = best_mdl.predict(ts.index[0], ts.index[-1])
pred.plot(ax=ax, style='r-', label='In-sample prediction')

styles = ['b-', '0.2', '0.75', '0.2', '0.75']
fc_all.plot(ax=ax, style=styles)
plt.fill_between(fc_all.index, fc_all.lower_ci_95, fc_all.upper_ci_95, color='gray', alpha=0.7)
plt.fill_between(fc_all.index, fc_all.lower_ci_99, fc_all.upper_ci_99, color='gray', alpha=0.2)
plt.title('{} Day SPY Return Forecast\nARIMA{}'.format(n_steps, best_order))
plt.legend(loc='best', fontsize=10)
```



![](https://img-blog.csdnimg.cn/20190124131516451.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)



[Time Series Analysis (TSA) in Python-Linear Models to GRACH 笔记（五）](https://blog.csdn.net/FrankieHello/article/details/86633118)





