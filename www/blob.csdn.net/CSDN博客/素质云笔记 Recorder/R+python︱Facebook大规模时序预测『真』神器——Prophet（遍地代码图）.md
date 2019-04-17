# R+python︱Facebook大规模时序预测『真』神器——Prophet（遍地代码图） - 素质云笔记/Recorder... - CSDN博客





置顶2017年02月26日 20:53:05[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：12170








> 
经统专业看到预测的packages都是很眼馋的。除了之前的forecast包，现在这个prophet功能也很强大。本packages是由机器之心报道之后，抽空在周末试玩几小时。一些基本介绍可见机器之心的[《业界 

  | 

  Facebook开源大规模预测工具Prophet：支持Python和R》

  并不喜欢理论分析，能直接上案例的，一般不码字，力求简单粗暴！！
官网网址：[https://facebookincubator.github.io/prophet/](https://facebookincubator.github.io/prophet/)

github网址：[https://github.com/facebookincubator/prophet](https://github.com/facebookincubator/prophet)

论文：《Forecasting at Scale   Sean J.Taylor and Benjamin Letham》 

案例数据下载：[http://download.csdn.net/detail/sinat_26917383/9764537](http://download.csdn.net/detail/sinat_26917383/9764537)
最后会补充一些facebook的理论。

**那么试玩下来觉得比较赞的功能点：**
- 1、**大规模、细粒度数据。**其实并不是大量数据，而是时间粒度可以很小，在学校玩的计量大多都是“年/月”粒度，而这个包可以适应“日/时”级别的，具体的见后面的案例就知道了。不过，预测速度嘛~ 

可以定义为：较慢！！！
- 2、**趋势预测+趋势分解**，最亮眼模块哟~~  

拟合的有两种趋势：线性趋势、logistic趋势；趋势分解有很多种：Trend趋势、星期、年度、季节、节假日，同时也可以看到节中、节后效应。
- 3、**突变点识别+调整**。多种对抗突变办法以及调节方式。
- 4、**异常值/离群值检测。**时间维度的异常值检测。突变点和异常点既相似、又不同。
- 5、**处理缺失值数据。**这里指的是你可能有一些时间片段数据的缺失，之前的做法是先插值，然后进行预测（一些模型不允许断点），这里可以兼顾缺失值，同时也达到预测的目的。可以处理缺失值数据，这点很棒。

> 
prophet应该就是我一直在找的，目前看到最好的营销活动分析的预测工具，是网站分析、广告活动分析的福音，如果您看到本篇文章内的方法，您在使用中发现什么心得，还请您尽量分享出来~


```
# install.packages('prophet')
library(prophet)
library(dplyr)
```

. 

.

# 一、趋势预测+趋势分解

## 1、案例一：线性趋势+趋势分解
- 数据生成+建模阶段

```
history <- data.frame(ds = seq(as.Date('2015-01-01'), as.Date('2016-01-01'), by = 'd'),
                      y = sin(1:366/200) + rnorm(366)/10)

m <- prophet(history,growth = "linear")
```

其中，生成数据的时候注意，最好用ds（时间项）、y（一定要numeric）这两个命名你的变量，本案例是单序列+时间项。数据长这样： 
![这里写图片描述](https://img-blog.csdn.net/20170226172926989?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

prophet是生成模型阶段，m中有很多参数，有待后来人慢慢研究。- 预测阶段

```
#时间函数
future <- make_future_dataframe(m, periods = 365)
tail(future)
#预测
forecast <- predict(m, future)
tail(forecast[c('ds', 'yhat', 'yhat_lower', 'yhat_upper')])
#直线预测
plot(m, forecast)
#趋势分解
prophet_plot_components(m, forecast)
```

make_future_dataframe：有趣的时间生成函数，之前的ds数据是2015-1-1到2016-1-1，现在生成了一个2015-1-1到2016-12-30序列，多增加了一年，以备预测。而且可以灵活的调控是预测天，还是周，freq参数。 

predict，预测那么ds是时间，yhat是预测值，lower和upper是置信区间。 

感受一下plot： 
![这里写图片描述](https://img-blog.csdn.net/20170226173508606?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

prophet_plot_components函数是趋势分解函数，将趋势分成了趋势项、星期、年份，这是默认配置。 
![这里写图片描述](https://img-blog.csdn.net/20170226173542148?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
.

## 2、案例二：logitics趋势+趋势分解

logitics是啥？  不懂烦请百度。

```
#数据生成阶段
history <- data.frame(ds = seq(as.Date('2015-01-01'), as.Date('2016-01-01'), by = 'd'),
                      y = sin(1:366/200) + rnorm(366)/10,
                      cap=sin(1:366/200) + rnorm(366)/10+rep(0.3,366))
#最大增长趋势，cap设置cap，就是这个规模的顶点,y当时顶点

#模型生成
m <- prophet(history,growth = "logistic")
future <- make_future_dataframe(m, periods = 1826)
future$cap <- sin(1:2192/200) + rnorm(2192)/10+rep(0.3,2192)

#预测阶段
fcst <- predict(m, future)
plot(m, fcst)
```

prophet这里如果是要拟合logitics趋势，**就需要一个cap变量，这个变量是y变量的上限（譬如最大市场规模），因为y如果服从logitics趋势不给范围的话，很容易一下预测就到顶点了，所以cap来让预测变得不那么“脆弱”…**

下面来看一个失败拟合logitics案例： 
![这里写图片描述](https://img-blog.csdn.net/20170226174713121?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
. 

.

# 二、节假日效应

可以考察节中、节后效应。来看看paper中如何解释节日效应的（[论文地址](https://facebookincubator.github.io/prophet/static/prophet_paper_20170113.pdf)）： 
![这里写图片描述](https://img-blog.csdn.net/20170911151356820?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
也就是说，节日效应能量函数h(t)由两部分组成，Z(t)是一个示性函数的集合（indicator function），而参数K服从（0,v）正态分布。可以说，将节日看成是一个正态分布，把活动期间当做波峰，lower_window 以及upper_window 的窗口作为扩散。

## 1、节中效应

```
#数据生成：常规数据
history <- data.frame(ds = seq(as.Date('2015-01-01'), as.Date('2016-01-01'), by = 'd'),
                      y = sin(1:366/200) + rnorm(366)/10,
                      cap=sin(1:366/200) + rnorm(366)/10+rep(0.3,366))
#数据生成：节假日数据
library(dplyr)
playoffs <- data_frame(
  holiday = 'playoff',
  ds = as.Date(c('2008-01-13', '2009-01-03', '2010-01-16',
                 '2010-01-24', '2010-02-07', '2011-01-08',
                 '2013-01-12', '2014-01-12', '2014-01-19',
                 '2014-02-02', '2015-01-11', '2016-01-17',
                 '2016-01-24', '2016-02-07')),
  lower_window = 0,
  upper_window = 1
)
superbowls <- data_frame(
  holiday = 'superbowl',
  ds = as.Date(c('2010-02-07', '2014-02-02', '2016-02-07')),
  lower_window = 0,
  upper_window = 1
)
holidays <- bind_rows(playoffs, superbowls)

#预测
m <- prophet(history, holidays = holidays)
forecast <- predict(m, future)

#影响效应
forecast %>% 
  select(ds, playoff, superbowl) %>% 
  filter(abs(playoff + superbowl) > 0) %>%
  tail(10)

#趋势组件
prophet_plot_components(m, forecast);
```

**数据生成环节有两个数据集要生成，一批数据是常规的数据（譬如流量），还有一个是节假日的时间数据**

其中lower_window，upper_window 可以理解为假日延长时限，国庆和元旦肯定休息时间不一致，设置地很人性化，譬如**圣诞节的平安夜+圣诞节两天，那么就要设置（lower_window = -1,  upper_window = 1）**。这个lower_window 的尺度为天，所以如果你的数据是星期/季度，需要设置-7/+7，比较合理。举一个python中的设置方式（时序是by week）：

```bash
c3_4 = pd.DataFrame({
  'holiday': 'c1',
  'ds': pd.to_datetime(['2017/2/26',
'2017/3/5'),
  'lower_window': -7,
  'upper_window': 7,
})
```

lower_window，upper_window 是节日效应的精髓，一般情况下，在-7 / +7 的时间跟活动期的数值不一样，刚好可以很多表示出节日的正态效应。 

数据长这样：

```xml
holiday         ds lower_window upper_window
       <chr>     <date>        <dbl>        <dbl>
1    playoff 2008-01-13            0            1
2    playoff 2009-01-03            0            1
3    playoff 2010-01-16            0            1
4    playoff 2010-01-24            0            1
5    playoff 2010-02-07            0            1
```

预测阶段，记得要开启prophet(history, holidays = holidays)中的holidays。现在可以来看看节假日效应：

```
ds      playoff superbowl
1 2015-01-11  0.012300004         0
2 2015-01-12 -0.008805914         0
3 2016-01-17  0.012300004         0
4 2016-01-18 -0.008805914         0
5 2016-01-24  0.012300004         0
6 2016-01-25 -0.008805914         0
7 2016-02-07  0.012300004         0
8 2016-02-08 -0.008805914         0
```

从数据来看，可以看到有一个日期是重叠的，超级碗+季后赛在同一天，那么这样就会出现**节日效应累加的情况。**

可以看到季后赛当日的影响比较明显，超级碗当日基本没啥影响，当然了，这些数据都是我瞎编的，要是有效应就见xxx。 

趋势分解这里，除了趋势项、星期、年份，多了一个节假日影响，看到了吗？
![这里写图片描述](https://img-blog.csdn.net/20170226175541682?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

.

## 2、调和节日效应（Prior scale for holidays and seasonality）

一些情况下节假日会发生过拟合，那么可以使用holidays.prior.scale参数来进行调节，使其平滑过渡。（不知道翻译地对不对，本来刚开始以为是节后效应…）

```
#节后效应 holidays.prior.scale
m <- prophet(history, holidays = holidays, holidays.prior.scale = 1)
forecast <- predict(m, future)
forecast %>% 
  select(ds, playoff, superbowl) %>% 
  filter(abs(playoff + superbowl) > 0) %>%
  tail(10)
```

主要通过holidays.prior.scale来实现，默认是10。由于笔者乱整数据，这里显示出效应，所以粘贴官网数据。官网的案例里面，通过调节，使得当晚超级碗的效应减弱，兼顾了节前的情况对当日的影响。 

同时除了节前，还有季节前的效应，通过参数seasonality_prior_scale 调整

```
DS  PLAYOFF SUPERBOWL
2190    2014-02-02  1.362312    0.693425
2191    2014-02-03  2.033471    0.542254
2532    2015-01-11  1.362312    0.000000
2533    2015-01-12  2.033471    0.000000
2901    2016-01-17  1.362312    0.000000
2902    2016-01-18  2.033471    0.000000
2908    2016-01-24  1.362312    0.000000
```

. 

.

# 三、突变点调节、间断点、异常点

> 
本节之后主要就是玩案例里面的数据，案例数据如果R包中没有，可以从[这里下载](http://download.csdn.net/detail/sinat_26917383/9764537)。


.

## 1、Prophet——自动突变点识别

时间序列里面的很可能存在突变点，譬如一些节假日的冲击。Prophet会自动检测这些突变点，并进行适当的调整，但是机器判断会出现：没有对突变点进行调整、突变点过度调整两种情况，如果真的突变点出现，也可以通过函数中的参数进行调节。

**Prophet自己会检测一些突变点**，以下的图就是Prophet自己检测出来的，虚纵向代表突变点。检测到了25个，那么Prophet的做法跟L1正则一样，“假装”/删掉看不见这些突变。 
![这里写图片描述](https://img-blog.csdn.net/20170226181835776?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其自己检验突变点的方式，类似观察ARIMA的自相关/偏相关系数截尾、拖尾： 
![这里写图片描述](https://img-blog.csdn.net/20170226182228610?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

.
## 2、人为干预突变点——弹性范围

通过changepoint_prior_scale进行人为干预。

```
df = pd.read_csv('../examples/example_wp_peyton_manning.csv')
m <- prophet(df, changepoint.prior.scale = 0.5)
forecast <- predict(m, future)
plot(m, forecast)
```

来感受一下changepoint.prior.scale=0.05和0.5的区别： 
![这里写图片描述](https://img-blog.csdn.net/20170226183055017?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170226183102090?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

可以把changepoint.prior.scale看成一个弹性尺度，值越大，受异常值影响越大，那么波动越大，如0.5这样的。 

.
## 3、人为干预突变点——某突变点

当你知道数据中，存在某一个确定的突变点，且知道时间。可以用changepoints 函数。不po图了。

```
df = pd.read_csv('../examples/example_wp_peyton_manning.csv')
m <- prophet(df, changepoints = c(as.Date('2014-01-01')))
forecast <- predict(m, future)
plot(m, forecast)
```

.

## 4、突变预测

标题取了这么一个名字，也是够吓人的，哈哈~   第三节的前3点都是如何消除突变点并进行预测。 

但是！ 现实是，突变点是真实存在，且有些是有意义的，譬如双11、双12这样的节日。不能去掉这些突变点，但是不去掉又会影响真实预测，这时候Prophet新奇的来了一招：序列生成模型中，多少受异常值些影响（类似前面的changepoint_prior_scale，但是这里是从生成模型阶段就给一个弹性值）。 

这里从生成模型中可以进行三个角度的调节： 

（1）调节趋势； 

（2）季节性调节- **（1）趋势突变适应**

```
df = pd.read_csv('../examples/example_wp_peyton_manning.csv')
m <- prophet(df, interval.width = 0.95)
forecast <- predict(m, future)
```

在prophet生成模型阶段，加入interval.width，就是代表生成模型时，整个序列趋势，还有5%受异常值影响。
- **（2）季节性突变适应**

对于生产厂家来说，季节性波动是肯定有的，那么又想保留季节性突变情况，又要预测。而且季节性适应又是一个比较麻烦的事情，prophet里面需要先进行全贝叶斯抽样，mcmc.samples参数，默认为0.

```
m <- prophet(df, mcmc.samples = 500)
forecast <- predict(m, future)
prophet_plot_components(m, forecast);
```

打开mcmc.samples按钮，会把MAP估计改变为MCMC采样，**训练时间很长，可能是之前的10倍**。最终结果，官网DAO图： 
![这里写图片描述](https://img-blog.csdn.net/20170226191113412?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
.

## 5、异常值/离群值

异常值与突变点是有区别的，离群值对预测影响尤其大。

```
df <- read.csv('../examples/example_wp_R_outliers1.csv')
df$y <- log(df$y)
m <- prophet(df)
future <- make_future_dataframe(m, periods = 1096)
forecast <- predict(m, future)
plot(m, forecast);
```

![这里写图片描述](https://img-blog.csdn.net/20170226192615558?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

对结果的影响很大，而且导致预测置信区间扩大多倍不止。prophet的优势体现出来了，**prophet是可以接受空缺值NA的**，所以这些异常点删掉或者NA掉，都是可以的。

```
#异常点变为NA+进行预测
outliers <- (as.Date(df$ds) > as.Date('2010-01-01')
             & as.Date(df$ds) < as.Date('2011-01-01'))
df$y[outliers] = NA
m <- prophet(df)
forecast <- predict(m, future)
plot(m, forecast);
```

当然啦，你也可以删掉整一段影响数据，特别是天灾人祸的影响是永久存在的，那么可以删掉这一整段。下图就是这样的情况，2015年6月份左右的一批数据，都是离群值。 
![这里写图片描述](https://img-blog.csdn.net/20170226193402327?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
. 

.

# 四、缺失值、空缺时间的处理+预测

前面第三章后面就提过，**prophet是可以处理缺失值**。那么这里就可以实现这么一个操作，如果你的数据不完整，且是间断的，譬如你有一个月20天的数据，那么你也可以根据prophet预测，同时给予你每天的数据结果。实现了以下的功能：
`prophet=缺失值预测+插值`
```
df <- read.csv('../examples/example_retail_sales.csv')
m <- prophet(df)
future <- make_future_dataframe(m, periods = 3652)
fcst <- predict(m, future)
plot(m, fcst);
```

![这里写图片描述](https://img-blog.csdn.net/20170226193952799?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

源数据长这样：

```
ds      y
1   1992-01-01 146376
2   1992-02-01 147079
3   1992-03-01 159336
4   1992-04-01 163669
5   1992-05-01 170068
```

也就是你只有一年的每个月的数据，上面是预测接下来每一天的数据，也能预测，但是后面每天预测的误差有点大。所以你可以设置make_future_dataframe中的freq，后面预测的是每个月的：

```
future <- make_future_dataframe(m, periods = 120, freq = 'm')
fcst <- predict(m, future)
plot(m, fcst)
```

![这里写图片描述](https://img-blog.csdn.net/20170226194322254?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

.
# 五、用python实现prophet时序预测

## 1、安装

笔者在linux实践的时候，安装就遇到了很多问题。
`pip install fbprophet`
[官网说](https://facebookincubator.github.io/prophet/docs/installation.html)：Make sure compilers (gcc, g++) and Python development tools (python-dev) are installed. If you are using a VM, be aware that you will need at least 2GB of memory to run PyStan. 

还需要预先加载pystan这个包。 

同时在调用的时候，`from fbprophet import Prophet` 报错，因为github最新版不是官方文档中的语句了。。。真是坑 

应该是：`from forecaster import Prophet`
.

## 2、实践案例

模拟一个最简单的节日效应的案例：

```
from forecaster import Prophet
m = Prophet(holidays=holidays, holidays_prior_scale=20)
m.fit(df)
future = m.make_future_dataframe(periods = 1 ,freq = 'w' )
forecast = m.predict(future)
forecast
```

forecast中包含所有的信息，是一个dataframe表。包含：预测的y，趋势项、季节项、活动项等 

其中freq 可以自己调节。其中plot_components是趋势分解。
`m.plot_components(forecast)`
![这里写图片描述](https://img-blog.csdn.net/20170911150645256?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 延伸一：Facebook 的数据预测工具 Prophet ——贝叶斯推理

[Facebook 的数据预测工具 Prophet 有何优势？用贝叶斯推理一探究竟](https://mp.weixin.qq.com/s?__biz=MjM5ODU3OTIyOA==&mid=2650664534&idx=3&sn=6e2716c318c15fc3aff6b61de5b2ff47&chksm=bec1d32589b65a330ed6b8e4da0777b5d3cf1422d82fd9016003f9d8a10d89d3cfda37d360a6&mpshare=1&scene=1&srcid=0409xAW5FCHFTGZar0JAWYYP#rd)

Prophet 在进行预测，其后端系统是一个概率程序语言 Stan，这代表 Prophet 能发挥出很多贝叶斯算法的优势，比如说：

使模型具有简单、易解释的周期性结构； 

预测结果包括才完全后验分布中导出的置信区间，即Prophet提供的是一个数据驱动的风险估计。 

在下面研究中，研究者让Prophet对两组数据进行预测，在后端使用概率程序语言，读者可以借此看到使用Stan的一些工作细节。
Prophet使用了一种通用时间序列模型，这种模型可适用于Facebook上的数据，并且具有分段走向（piecewise trends）、多周期及弹性假期（floating holiday）三种特性。

Prophet的把时间序列预测问题转变成了一个曲线拟合练习（exercise）。在这个曲线中，因变量是增长、周期和holiday的总体表现。
``
 - 增长（growth）

这一部分采用一个随时间变化的逻辑增长模型，属于非线性增长，所以，要用简单的分段常数函数来模拟线性增长。
用比率调整向量模拟分段点，每个分段点都对应一个具体的时间点。用拉普拉斯分布（Laplace distribution）模拟比率调整变量，位置参数（location parameter）设定为0。

 -Prophet 模型周期（periodic seasonality）

采用标准傅里叶级数。年、周的周期性（seasonality）近似值分别为20和6，周期性成分（seasonal component）在正常情况下是平滑状态。

 - 假期（Holiday）

用一个指标函数来模拟。
使用者可以调节扩散参数（spread parameter），以模拟未来会有多少历史季节性变化（historical seasonal variation）。

公众号“素质云笔记”定期更新博客内容：
![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)](http://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=2650723632&idx=2&sn=686c0a40ee081c18dda15ef82dbacd45&chksm=871b114eb06c985815df268fc8bb4bc0e0e3e58bb72ecc98f905ed95ee3389388854f51b6146&mpshare=1&scene=1&srcid=0224hRL2Manpk6aWFA2gDLTi#rd)




