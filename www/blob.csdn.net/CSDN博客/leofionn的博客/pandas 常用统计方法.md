
# pandas 常用统计方法 - leofionn的博客 - CSDN博客


2018年06月04日 21:54:08[leofionn](https://me.csdn.net/qq_36142114)阅读数：257


### [https://blog.csdn.net/qq_16234613/article/details/64217337](https://blog.csdn.net/qq_16234613/article/details/64217337)
### 也是非常不错的教程
### 方法
pandas 对象有一些统计方法。它们大部分都属于约简和汇总统计，用于从 Series 中提取单个值，或从 DataFrame 的行或列中提取一个 Series。
比如`DataFrame.mean(axis=0,skipna=True)`方法，当数据集中存在 NA 值时，这些值会被简单跳过，除非整个切片（行或列）全是 NA，如果不想这样，则可以通过`skipna=False`来禁用此功能：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|>>> df
|one  two
|a|1.40|NaN
|b|7.10|-|4.5
|c   NaN  NaN
|d|0.75|-|1.3
|[|4|rows x|2|columns]
|>>> df.mean()
|one|3.083333
|two|-|2.900000
|dtype: float64
|>>> df.mean(axis|=|1|)
|a|1.400
|b|1.300
|c      NaN
|d|-|0.275
|dtype: float64
|>>> df.mean(axis|=|1|,skipna|=|False|)
|a      NaN
|b|1.300
|c      NaN
|d|-|0.275
|dtype: float64
|
其他常用的统计方法有：
|\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#|******************************************|
|count|非 NA 值的数量|
|describe|针对 Series 或 DF 的列计算汇总统计|
|min , max|最小值和最大值|
|argmin , argmax|最小值和最大值的索引位置（整数）|
|idxmin , idxmax|最小值和最大值的索引值|
|quantile|样本分位数（0 到 1）|
|sum|求和|
|mean|均值|
|median|中位数|
|mad|根据均值计算平均绝对离差|
|var|方差|
|std|标准差|
|skew|样本值的偏度（三阶矩）|
|kurt|样本值的峰度（四阶矩）|
|cumsum|样本值的累计和|
|cummin , cummax|样本值的累计最大值和累计最小值|
|cumprod|样本值的累计积|
|diff|计算一阶差分（对时间序列很有用）|
|pct_change|计算百分数变化|

