
# math、numpy、pandas NaN 判断 - Zhang's Wikipedia - CSDN博客


2018年05月21日 23:25:14[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：2699



```python
>> np
```
```python
.nan
```
```python
== np
```
```python
.nan
```
```python
False
>> np
```
```python
.nan
```
```python
is np
```
```python
.nan
```
```python
True
>> math
```
```python
.nan
```
```python
is np
```
```python
.nan
```
```python
False
>> np
```
```python
.isnan
```
```python
(math
```
```python
.nan
```
```python
)
True
```
## 1. 判断 ndarray 中是否存在 nan
```python
>> c = np.
```
```python
array
```
```python
([
```
```python
1.
```
```python
,
```
```python
2.
```
```python
, np.nan,
```
```python
3.
```
```python
,
```
```python
4.
```
```python
])
>> np.isnan(c)
```
```python
array
```
```python
([
```
```python
False
```
```python
,
```
```python
False
```
```python
,
```
```python
True
```
```python
,
```
```python
False
```
```python
,
```
```python
False
```
```python
])
```
```python
# 注意
```
```python
>> np.nan != np.nan
```
```python
True
```
```python
>> np.nan in c
```
```python
False
```
上述方案要么返回的是一个序列，要么给出的是错误的结果。判断 numpy 下的多维数组中是否存在 nan 的简单方式：
```python
>> np
```
```python
.isnan
```
```python
(np
```
```python
.min
```
```python
(c))
True
>> np
```
```python
.isnan
```
```python
(np
```
```python
.sum
```
```python
(c))
True
```
```python
# 因为
```
```python
>> np
```
```python
.min
```
```python
(c)
nan
>> np
```
```python
.sum
```
```python
(c)
nan
```
## 2. 将 nan 填充为均值
```python
>> c = np
```
```python
.array
```
```python
([
```
```python
1.
```
```python
,
```
```python
2.
```
```python
, np
```
```python
.nan
```
```python
,
```
```python
3.
```
```python
,
```
```python
4.
```
```python
])
>> c[np
```
```python
.isnan
```
```python
(c)] = np
```
```python
.mean
```
```python
(c[~np
```
```python
.nan
```
```python
(c)])
```

