
# Facebook时序预测工具Prophet实战分析 - OraYang的博客 - CSDN博客

2018年01月31日 19:10:00[OraYang](https://me.csdn.net/u010665216)阅读数：4245所属专栏：[机器学习](https://blog.csdn.net/column/details/16605.html)



# 引言
去年Facebook开源了一套时序预测工具叫做[Prophet](https://facebook.github.io/prophet/)。Prophet是一个预测时间序列数
据的模型。 它基于一个自加性模型，用来拟合年、周、季节以及假期等非线性趋势。 它在至少有一年历史数据的日常周期性数据，效果最好。 Prophet对缺失值，趋势的转变和大量的异常值是有极强的鲁棒性。Prophet中文翻译是：“先知”。名字还是挺贴切的。在看完本篇文章后，你将会知道：
> Prophet有什么亮点

> Prophet是怎么工作的

> 如何去使用Prophet

# 为什么会发布这套工具
预测是一项数据科学任务，是组织内许多活动的核心。 例如，Facebook这样的大型组织必须参与容量规划，以有效分配稀缺资源和目标设置，以便衡量相对于基线的性能。
得到高质量的预测对于任何机器或大多数分析师来说都不是一个简单的问题。 在现实生活中，我做了不少商业预测，归纳总结发现，预测任务有两大核心问题：
> 完全自动化的预测技术往往都比较“脆弱”，不是太灵活，对一些问题缺乏有效的假设以及启发式的思路

> 能够得到高质量预测的数据科学家是非常罕见的，因为预测需要丰富的经验以及专业的数据科学技能
在现实生活中，对高质量预测的需求往往要远远超过分析人员可以生产的速度。因此Prophet的出现就是为了让专家和非专家能够更轻松地进行高质量的预测。
很多人在做预测的时候，总会提出这个一个潜在的考虑因素：“规模”。当我数据量很大的时候，计算与存储的开销能满足要求么？其实对于时序预测问题，计算问题可以通过并行执行解决，存储问题可以通过关系型数据库MySQL或数据仓库Hive解决。
Prophet解决的“规模”问题，其实本质上是在完成时序预测任务时面临的各种复杂情况，Prophet能够适用于多种业务场景下的预测。
# Prophet的亮点
当你做了很多时序预测任务时，你就会发现，并不是所有的任务都能用一个预测流程来解决。“一招鲜，吃遍天”在数据科学里是行不通的。通过分析平时我们做时序任务时遇到的问题，我们发现给定的时序数据有以下几个特点：
> 观察值是按每小时或每天或每周或每月给出的一段历史数据

> 多尺度的周期性：一周七天，一个月30天，12个月等等

> 提前已知的一些重要假期：各种法定节日或传统节日

> 数据中存在缺失值或异常值

> 历史趋势的变化

> 趋势是非线性变化，达到自然极限或趋于饱和
Prophet通过将全自动预测与在线学习相结合从而保证了该工具能够解决大多数商业业务问题，Prophet工作流程如下图所示：
![prophet](https://img-blog.csdn.net/20180131150910817?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
相比于目前开源的一些预测工具，Prophet主要有以下两点优势：
> Prophet能够让你更方便直接地创建一个预测任务，而其他的一些工具包（ARMA，指数平滑）等，这些工具每个有自己的优缺点及参数，即使是优秀的数据分析师想要从众多的模型中选择合适的模型及相应的参数也是够让他头皮发麻的。

> Prophet是为非专家们”量身定制”的。为什么这么说呢？你利用Prophet可以直接通过修改季节参数来拟合季节性，修改趋势参数来拟合趋势信息，指定假期来拟合假期信息等等。

# Prophet是如何工作的
本质上讲，Prophet是由四个组件组成的[自加性回归模型](https://en.wikipedia.org/wiki/Additive_model)：
> 分段线性或逻辑增长曲线趋势。 Prophet通过从数据中选择改变点，自动检测趋势的变化。

> 用傅里叶级数模拟每年的季节性分量。

> 用虚拟变量来模拟每周的周期性分量。

> 用户提供的重要节日列表
Prophet其中最重要的思想就是曲线拟合，这与传统的时序预测算法有很大的不同。
# 如何使用Prophet
## 安装
因为Prophet已经发布在PyPI上了，所以我们可以通过pip进行安装：
```python
$
```
```python
pip install fbprophet
```
我的是ubuntu16.10版本，软件大小为68.1MB
## 快速启动
Prophet遵循sklearn模型API。 我们创建一个prophet类的实例，然后调用它的fit和predict方法。
Prophet的输入必须包含两列数据：ds和y，其中ds是时间戳列，必须是时间信息；y列必须是数值，代表我们需要预测的信息。首先我们获取数据：这里我们采用官网给出的[Peyton Manning](https://en.wikipedia.org/wiki/Peyton_Manning)这个维基百科页面日常被访问的数据（[下载链接](https://github.com/facebook/prophet/blob/master/examples/example_wp_peyton_manning.csv)）。
然后我们导入相关库以及数据：
```python
# Python
```
```python
import
```
```python
pandas
```
```python
as
```
```python
pd
```
```python
import
```
```python
numpy
```
```python
as
```
```python
np
```
```python
from
```
```python
fbprophet
```
```python
import
```
```python
Prophet
```
```python
df = pd.read_csv(
```
```python
'example_wp_peyton_manning.csv'
```
```python
)
df[
```
```python
'y'
```
```python
] = np.log(df[
```
```python
'y'
```
```python
])
df.head()
```
|ds|y|
|---|---|---|
|0|2007-12-10|9.590761|
|1|2007-12-11|8.519590|
|2|2007-12-12|8.183677|
|3|2007-12-13|8.072467|
|4|2007-12-14|7.893572|

```python
m = Prophet()
m.fit(df);
```
```python
# Python
```
```python
future = m.make_future_dataframe(periods=
```
```python
365
```
```python
)
future.tail()
```
|ds|
|---|---|
|3265|2017-01-15|
|3266|2017-01-16|
|3267|2017-01-17|
|3268|2017-01-18|
|3269|2017-01-19|

```python
# Python
```
```python
forecast = m.predict(future)
forecast.tail()
```
|ds|trend|trend_lower|trend_upper|yhat_lower|yhat_upper|seasonal|seasonal_lower|seasonal_upper|seasonalities|seasonalities_lower|seasonalities_upper|weekly|weekly_lower|weekly_upper|yearly|yearly_lower|yearly_upper|yhat|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
|3265|2017-01-15|7.189085|6.836460|7.556897|7.529009|8.874259|1.017668|1.017668|1.017668|1.017668|1.017668|1.017668|0.048286|0.048286|0.048286|0.969382|0.969382|0.969382|8.206753|
|3266|2017-01-16|7.188059|6.834473|7.557350|7.783923|9.237696|1.343707|1.343707|1.343707|1.343707|1.343707|1.343707|0.352298|0.352298|0.352298|0.991409|0.991409|0.991409|8.531766|
|3267|2017-01-17|7.187033|6.832412|7.557802|7.555636|9.061272|1.132123|1.132123|1.132123|1.132123|1.132123|1.132123|0.119631|0.119631|0.119631|1.012493|1.012493|1.012493|8.319156|
|3268|2017-01-18|7.186006|6.830079|7.558255|7.456370|8.982307|0.965765|0.965765|0.965765|0.965765|0.965765|0.965765|-0.066664|-0.066664|-0.066664|1.032429|1.032429|1.032429|8.151772|
|3269|2017-01-19|7.184980|6.827757|7.558474|7.445508|8.889737|0.978710|0.978710|0.978710|0.978710|0.978710|0.978710|-0.072264|-0.072264|-0.072264|1.050973|1.050973|1.050973|8.163690|

```python
m.plot(forecast)
```
![这里写图片描述](https://img-blog.csdn.net/20180131191233473?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
m.plot_components(forecast)
```
![这里写图片描述](https://img-blog.csdn.net/20180131191240769?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
\# 不同业务场景下的预测效果
这一小节，我们基于不同业务场景，看看Prophet的预测效果好不好。



\#\# 饱和性预测
这里通过制定预测值的上限、下限，从而来进行饱和性预测
```python
#Forecasting Growth
```
```python
df = pd.read_csv(
```
```python
'example_wp_R.csv'
```
```python
)
df[
```
```python
'y'
```
```python
] = np.log(df[
```
```python
'y'
```
```python
])
```
```python
df.head()
```
|ds|y|
|---|---|---|
|0|2008-01-30|5.976351|
|1|2008-01-16|6.049733|
|2|2008-01-17|6.011267|
|3|2008-01-14|5.953243|
|4|2008-01-15|5.910797|

```python
# 指定上限df['cap'] = 8.5
```
```python
df[
```
```python
'cap'
```
```python
] =
```
```python
8.5
```
```python
m = Prophet(growth=
```
```python
'logistic'
```
```python
)
m.fit(df)
future = m.make_future_dataframe(periods=
```
```python
1826
```
```python
)
future[
```
```python
'cap'
```
```python
] =
```
```python
8.5
```
```python
fcst = m.predict(future)
m.plot(fcst)
```
![png](https://img-blog.csdn.net/20180131185745677?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 趋势变化点
Prophet能够实现历史趋势变化点的自动检测,并可以通过调整参数适当修正变化。Prophet通过changepoint_prior_scale来调整变化点的趋势程度，值越大，趋势就越灵活。同时Prophet通过参数changepoints来手动指定趋势变化点。
```python
m = Prophet(changepoint_prior_scale=
```
```python
0.5
```
```python
)
forecast = m.fit(df).predict(future)
m.plot(forecast)
```
![png](https://img-blog.csdn.net/20180131185837873?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
m = Prophet(changepoint_prior_scale=
```
```python
0.001
```
```python
)
forecast = m.fit(df).predict(future)
m.plot(forecast)
```
![png](https://img-blog.csdn.net/20180131185849935?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
m = Prophet(changepoints=[
```
```python
'2014-01-01'
```
```python
])
forecast = m.fit(df).predict(future)
m.plot(forecast)
```
![这里写图片描述](https://img-blog.csdn.net/20180131190323708?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Prophet在异常值、缺失值、周期性、季节性、节假日等方面效果也不错，这里就不赘述了。
# 总结
读了这篇文章，是不是感觉这个Prophet很好用，是不是迫不及待想去试试水？哈哈哈
demo代码：github :[Prophe_in_action](https://github.com/shawshany/Prophet_in_action)

