
# Gauss error function - Zhang's Wikipedia - CSDN博客


2018年05月20日 17:47:36[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：310



## 0. error function

$$
\text{erf}(x)=\frac{1}{\sqrt\pi}\int_{-x}^{x}e^{-t^2}dt
$$
python 下的 math 标准库定义着 erf 的实现。
## 1. 从 error function 到标准正态分布 cdf 的实现
标准正态分布的累积分布函数无法用基本的解析形式表示，但却可通过的简单形式变换而计算得到：

$$
\Phi (x) = \frac{1}{2}+ \frac{1}{2} \operatorname{erf} \left(x/ \sqrt{2}\right) = \frac{1}{2} \operatorname{erfc} \left(-x/ \sqrt{2}\right)
$$

## 2. python 实现
```python
def
```
```python
phi
```
```python
(x, mu=
```
```python
0
```
```python
, sigma=
```
```python
1
```
```python
)
```
```python
:
```
```python
return
```
```python
(
```
```python
1
```
```python
+ math.erf((x-mu) / math.sqrt(
```
```python
2
```
```python
) / math.sqrt(sigma)))/
```
```python
2
```

