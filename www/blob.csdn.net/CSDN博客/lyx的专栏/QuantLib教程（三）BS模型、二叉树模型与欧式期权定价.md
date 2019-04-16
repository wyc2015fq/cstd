# QuantLib教程（三）BS模型、二叉树模型与欧式期权定价 - lyx的专栏 - CSDN博客





2017年05月11日 17:44:26[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：7186
所属专栏：[QuantLib的python使用教程](https://blog.csdn.net/column/details/15583.html)









# 1.风险中性（risk-netural）与无套利假设

风险中性与无套利假设是期权定价公式的基础理论，或者说基石。我们来简单说说这两个是怎么回事吧。

现在有一个股票，价格为S0，那么t时间之后的价格是多少呢？或者说，期望价格是多少呢？这两个理论告诉我们是S0 * exp（r * t），其中，r是无风险利率。在一个理想环境中，我们可以以无风险利率借钱，也可以以无风险利率借给别人钱。

所以说，如果t时间之后，股票的价格不是S0 * exp（r * t），那么我们可以选择在一开始借钱买股票或者卖空股票然后把钱借给别人获得无风险利率。当然了，这个理论看起来显然是扯扯的，毕竟我们都需要风险溢价。但是假设就是假设嘛，没有这个就不好定价，大家还怎么玩。

# 2.BS公式

好了，现在我们假设我们手上的不是一个股票，而是一个call option，也就是看涨期权。那么，看涨期权的在t时刻的期望价格是多少呢？E[max(V−K,0)]，这个大家应该都是知道的。如果期权最后的payoff是怎么计算都不知道的话，读者还是先去了解一下期权吧。

这个公式呢，可以这么写：

![](https://img-blog.csdn.net/20170511165826346?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


后面的g(V)是在t时候，价格为V的概率。同时，我们假设股票的价格服从lognormal分布，这样的话每天的收益率就服从正态分布了。

这个是我们这个期权在t时刻的期望价值，那么我们折现回来，就是

![](https://img-blog.csdn.net/20170511165925238?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这样，我们就获得了现在这个call option值多少钱了。

然后，我们把上面两个式子结合起来，经过复杂的数学运算之后，包括伊藤引理，几何布朗运动 balabala，我们得到了下面这个式子：

![](https://img-blog.csdn.net/20170511170330728?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


其中：

![](https://img-blog.csdn.net/20170511170340530?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


N是正太分布的累积概率函数。

那么如果我们的股票还要分红，那怎么办呢？我们假设以q的分红利率分红，那么就有：

![](https://img-blog.csdn.net/20170511170347733?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


到这里，我们就能自己写个公式，然后计算一个期权的价格了。我们看一下，如果要计算一个期权价格需要哪些参数的输入：

1.S0 股票当前价格  2.K期权行权价格 3.无风险利率 4.股票波动率 5.期权到期时间 6.分红率 7.期权类型，看涨看跌

# 3.二叉树模型

其实对期权定价的模型有很多，上的BSM只是其中的一种而已。还有一种比较多见的就是二叉树。

![](https://img-blog.csdn.net/20170511171020485?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


现在是t0时刻，我们的股票价格是S0，在t1时刻，股票价格可能上升也可能下降，所以，存在一个上升的概率p，同时，上涨幅度是u-1；下降的概率就是（1-p），幅度是1-d，然后，在后面每个时刻都这么做。

根据一开始讲的无套利和风险中性的基石，我们有：

![](https://img-blog.csdn.net/20170511171330691?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


然后，我们考虑一下股票的方差，就可以得到下面这个等式：

![](https://img-blog.csdn.net/20170511171805279?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


如果，我们假设 u * d = 1，那么：

![](https://img-blog.csdn.net/20170511171830022?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


最后，有了上面这些值，就可以知道二叉树模型下，在t0时刻股票的价格了：

![](https://img-blog.csdn.net/20170511171832545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


当然，这只是一个step。我们不断地做二叉树，坐的阶数越高，最后的价格越精确。

# 4.QuantLib计算欧式看涨期权

先放代码：



```python
#coding=utf8
import QuantLib as ql
import matplotlib.pyplot as plt

# 1.设置期权的五要素以及分红率和期权类型
# 1.1五要素
maturity_date = ql.Date(11, 8, 2017)
spot_price = 9.37
strike_price = 10.00
volatility = 0.20 # the historical vols for a year
risk_free_rate = 0.001
# 1.2分红率
dividend_rate = 0.01
# 1.3期权类型
option_type = ql.Option.Call

# 1.4设置日期计算方式与使用地区
day_count = ql.Actual365Fixed()
calendar = ql.UnitedStates()
# 1.5计算期权价格的日期，也就是估值日，我们设为今天
calculation_date = ql.Date(11, 5, 2017)
ql.Settings.instance().evaluationDate = calculation_date

# 2.利用上的设置配置一个欧式期权
payoff = ql.PlainVanillaPayoff(option_type, strike_price)
exercise = ql.EuropeanExercise(maturity_date)
# 2.1根据payoff与exercise完成欧式期权的构建
european_option = ql.VanillaOption(payoff, exercise)

# 3.构造我们的BSM定价引擎
# 3.1 处理股票当前价格
spot_handle = ql.QuoteHandle(
    ql.SimpleQuote(spot_price)
)
# 3.2 根据之前的无风险利率和日期计算方式，构建利率期限结构
flat_ts = ql.YieldTermStructureHandle(
    ql.FlatForward(calculation_date, risk_free_rate, day_count)
)
# 3.3 设置分红率期限结构
dividend_yield = ql.YieldTermStructureHandle(
    ql.FlatForward(calculation_date, dividend_rate, day_count)
)
# 3.4 设置波动率结构
flat_vol_ts = ql.BlackVolTermStructureHandle(
    ql.BlackConstantVol(calculation_date, calendar, volatility, day_count)
)
# 3.5 构造BSM定价引擎
bsm_process = ql.BlackScholesMertonProcess(spot_handle,
                                           dividend_yield,
                                           flat_ts,
                                           flat_vol_ts)

# 4使用BSM定价引擎计算
european_option.setPricingEngine(ql.AnalyticEuropeanEngine(bsm_process))
bs_price = european_option.NPV()
print "The theoretical price is ", bs_price
```

看起来计算一个期权很复杂，步骤很多，其实只要弄清楚步骤和了解这样设计的原因就很好理解了。


一共四个步骤。其实第一步就是参数的初始化，方面后面调用这些变量罢了。

这是第一步：



```python
# 1.设置期权的五要素以及分红率和期权类型
# 1.1五要素
maturity_date = ql.Date(11, 8, 2017)
spot_price = 9.37
strike_price = 10.00
volatility = 0.20 # the historical vols for a year
risk_free_rate = 0.001
# 1.2分红率
dividend_rate = 0.01
# 1.3期权类型
option_type = ql.Option.Call

# 1.4设置日期计算方式与使用地区
day_count = ql.Actual365Fixed()
calendar = ql.UnitedStates()
# 1.5计算期权价格的日期，也就是估值日，我们设为今天
calculation_date = ql.Date(11, 5, 2017)
ql.Settings.instance().evaluationDate = calculation_date
```



这些参数其实可以分成两种，一个是和期权直接相关的，且在期权存续区间不会变化的，也就是执行价格和到期如，剩下的，譬如波动率、估值日期、股票的现价，都会随着时间的变化而变化。所以在第二步中，我们把不变的那些变量放到我们的期权中去。



```python
# 2.利用上的设置配置一个欧式期权
payoff = ql.PlainVanillaPayoff(option_type, strike_price)
exercise = ql.EuropeanExercise(maturity_date)
# 2.1根据payoff与exercise完成欧式期权的构建
european_option = ql.VanillaOption(payoff, exercise)
```
然后，我们设置那些经常要变的变量：




```python
# 3.构造我们的BSM定价引擎
# 3.1 处理股票当前价格
spot_handle = ql.QuoteHandle(
    ql.SimpleQuote(spot_price)
)
# 3.2 根据之前的无风险利率和日期计算方式，构建利率期限结构
flat_ts = ql.YieldTermStructureHandle(
    ql.FlatForward(calculation_date, risk_free_rate, day_count)
)
# 3.3 设置分红率期限结构
dividend_yield = ql.YieldTermStructureHandle(
    ql.FlatForward(calculation_date, dividend_rate, day_count)
)
# 3.4 设置波动率结构
flat_vol_ts = ql.BlackVolTermStructureHandle(
    ql.BlackConstantVol(calculation_date, calendar, volatility, day_count)
)
# 3.5 构造BSM定价引擎
bsm_process = ql.BlackScholesMertonProcess(spot_handle,
                                           dividend_yield,
                                           flat_ts,
                                           flat_vol_ts)
```
最后，把要变的和不要变的结合在一起，就可以计算期权价格了。我们这个测试案例中，股票当前价格是9.37，执行价格是10元，三个月的看涨期权，今天的价格是0.145601234225。


利用QuantLib计算BSM模型下的期权价格就是这样。



