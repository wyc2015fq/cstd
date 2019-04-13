
# 统计学（检验、分布）的 python（numpy-pandas-scipy） 实现 - Zhang's Wikipedia - CSDN博客


2018年04月15日 21:26:02[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：1411


scipy 中统计相关的 api：[https://docs.scipy.org/doc/scipy/reference/stats.html](https://docs.scipy.org/doc/scipy/reference/stats.html)
[https://zhuanlan.zhihu.com/p/24635014](https://zhuanlan.zhihu.com/p/24635014)
[https://blog.csdn.net/lanchunhui/article/details/52328380](https://blog.csdn.net/lanchunhui/article/details/52328380)
## 1. t 检验：两个分布的差异
> 多维数据集的每一个属性列都可理解为一个特征的实例。两个分布的距离：每一个属性列代表的特征跟标签列之间的相关性。
t 检验用 t 分布理论来推论差异发生的概率，以比较两个分布的平均数之间的差异是否显著。主要用于样本含量小（$n < 30$），总体标准差$\sigma$未知的正态分布。
独立样本 t 检验统计量如下计算：

$$
t=\frac{\bar X_1-\bar X_2}{\sqrt{\frac{S_1^2}{n_1}+\frac{S_2^2}{n_2 	}}}, \quad S_x^2=\frac{\sum(x-\mu)^2}{n-1}
$$
尤其注意，这里的方差是无偏估计（**np.std(x, ddof=1)**）
```python
# 通过字典构造 DataFrame
```
```python
data = {
```
```python
'Category'
```
```python
: [
```
```python
'cat2'
```
```python
,
```
```python
'cat1'
```
```python
,
```
```python
'cat2'
```
```python
,
```
```python
'cat1'
```
```python
,
```
```python
'cat2'
```
```python
,
```
```python
'cat1'
```
```python
,
```
```python
'cat2'
```
```python
,
```
```python
'cat1'
```
```python
,
```
```python
'cat1'
```
```python
,
```
```python
'cat1'
```
```python
,
```
```python
'cat2'
```
```python
],
```
```python
'values'
```
```python
: [
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
,
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
,
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
,
```
```python
5
```
```python
,
```
```python
1
```
```python
]}
data_df = DataFrame(data)
```
```python
# 计算各自的均值
```
```python
>> data_df.groupby(
```
```python
'Category'
```
```python
).mean()
cat1
```
```python
2.666667
```
```python
cat2
```
```python
1.600000
```
```python
cat1 = data_df[data_df[
```
```python
'Category'
```
```python
] ==
```
```python
'cat1'
```
```python
]
    # data_df.Category ==
```
```python
'cat1'
```
```python
cat2 = data_df[data_df[
```
```python
'Category'
```
```python
] ==
```
```python
'cat2'
```
```python
]
from scipy.stats import ttest_ind
```
```python
# 计算二者的 t 检验统计量，及对应的 p-value
```
```python
>> ttest_ind(cat1[
```
```python
'values'
```
```python
], cat2[
```
```python
'values'
```
```python
])
Ttest_indResult(statistic=
```
```python
1.4927289925706944
```
```python
, pvalue=
```
```python
0.16970867501294376
```
```python
)
```
scipy 下的 t-test 计算方法
deft_test(x1, x2):n1, n2 = x1.size, x2.size
    mu1, mu2 = np.mean(x1), np.mean(x2)
    s1, s2 = np.std(x1, ddof=1), np.std(x2, ddof=1)
    num = np.abs(mu1 - mu2)
    denom = np.sqrt((((n1-1)*s1**2+ (n2-1)*s2**2)/(n1+n2-2))*(1/n1+1/n2))withnp.errstate(divide='ignore'):returnnum / denom

