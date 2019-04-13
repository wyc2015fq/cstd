
# 异常值检测 —— MAD（median absolute deviation） - Zhang's Wikipedia - CSDN博客


2018年05月20日 13:00:25[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：3108


MAD 定义为，一元序列$X_i$同其中位数偏差的绝对值的中位数（deviation，偏差本身有正有负）；

$$
\text{MAD=median}\left(|X_i-\text{median(X)}|\right)
$$

## 1. MAD 用于异常点的检测
假定数据服从正态分布，我们让异常点（outliers）落在两侧的 50% 的面积里，让正常值落在中间的 50% 的区域里：

$$
P(|X-\mu|\leq MAD)=P(\frac{|X-\mu|}{\sigma}\leq \frac{MAD}{\sigma})=P(Z\leq \frac{MAD}{\sigma})=1/2
$$
其中$P(Z\leq \frac{MAD}{\sigma})=\Phi(\frac{MAD}{\sigma})-\Phi(-\frac{MAD}{\sigma})=1/2$，又由$\Phi(-a)=1-\Phi(a)$，可$\Phi(MAD/\sigma)=3/4$⇒$MAD/\sigma=\Phi^{-1}(3/4)$，查表可知，$MAD/\sigma$=0.6749。
```python
from
```
```python
scipy.stats
```
```python
import
```
```python
norm
```
```python
def
```
```python
mad_based_outlier
```
```python
(points, thresh=
```
```python
3.5
```
```python
)
```
```python
:
```
```python
if
```
```python
type(points)
```
```python
is
```
```python
list:
        points = np.asarray(points)
```
```python
if
```
```python
len(points.shape) ==
```
```python
1
```
```python
:
        points = points[:,
```
```python
None
```
```python
]
    med = np.median(points, axis=
```
```python
0
```
```python
)
    abs_dev = np.absolute(points - med)
    med_abs_dev = np.median(abs_dev)
    mod_z_score = norm.ppf(
```
```python
0.75
```
```python
) * abs_dev / med_abs_dev
```
```python
return
```
```python
mod_z_score > thresh
```
## 2. MAD 与基于分位数方法的对比
MAD 的方法相对于分位数方法的一大优势即在于 MAD 方法对样本大小是不敏感也即是稳定的鲁棒的一种评价指标。
```python
def
```
```python
percentile_based_outlier
```
```python
(data, threshold=
```
```python
95
```
```python
)
```
```python
:
```
```python
diff = (
```
```python
100
```
```python
- threshold) /
```
```python
2.0
```
```python
minval, maxval = np.percentile(data, [diff,
```
```python
100
```
```python
- diff])
```
```python
return
```
```python
(data < minval) | (data > maxval)
```
---
[Pythonic way of detecting outliers in one dimensional observation data](https://stackoverflow.com/questions/22354094/pythonic-way-of-detecting-outliers-in-one-dimensional-observation-data?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa)

