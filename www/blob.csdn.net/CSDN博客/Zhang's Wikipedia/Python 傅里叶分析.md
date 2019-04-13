
# Python 傅里叶分析 - Zhang's Wikipedia - CSDN博客


2018年04月01日 21:35:26[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：618



## 0. 一维序列卷积
np.convolve，注意same/valid参数下（默认为full），序列卷积出的结果的长度：
>> np.convolve([1,2,3], [0,1,0.5])array([0. ,1. ,2.5,4. ,1.5])>>np.convolve([1,2,3],[0,1,0.5], 'same')array([1. ,2.5,4. ])>>np.convolve([1,2,3],[0,1,0.5], 'valid')array([2.5])
## 1. numpy 傅里叶变换
np.fft.fft：Y = np.fft.fft(y, 512)
能量谱（表明各个频率分量的能量大小）：Pyy = Y * np.conj(Y) / 512
## 2. 傅里叶频谱分析
[从MATLAB帮助文档里扒下来的例子之——傅里叶变换用于频谱分析](https://www.guokr.com/blog/440583/)
傅里叶变换的一大用途是从混杂的时域信号中找出其中各频率成分的分布。
以一个由 50Hz、120Hz 两个频率正弦信号和随机噪声叠加得到的信号为例（采样频率 1000Hz）：
```python
import numpy as np
import matplotlib
```
```python
.pyplot
```
```python
as plt
t = np
```
```python
.arange
```
```python
(
```
```python
0
```
```python
,
```
```python
.6
```
```python
,
```
```python
.001
```
```python
)
```
```python
# 0.001 => 采样频率
```
```python
x
```
```python
= np
```
```python
.sin
```
```python
(
```
```python
2
```
```python
*np
```
```python
.pi
```
```python
*
```
```python
50
```
```python
*t) + np
```
```python
.sin
```
```python
(
```
```python
2
```
```python
*np
```
```python
.pi
```
```python
*
```
```python
120
```
```python
*t)
```
```python
y
```
```python
=
```
```python
x
```
```python
+
```
```python
2
```
```python
*np
```
```python
.random
```
```python
.randn
```
```python
(t
```
```python
.size
```
```python
)
```
```python
# plt.plot(t[:50], y[:50])
```
```python
# plt.show()
```
```python
Y
```
```python
= np
```
```python
.fft
```
```python
.fft
```
```python
(
```
```python
y
```
```python
,
```
```python
512
```
```python
)
Pyy =
```
```python
Y
```
```python
* np
```
```python
.conj
```
```python
(
```
```python
Y
```
```python
) /
```
```python
512
```
```python
plt
```
```python
.plot
```
```python
(np
```
```python
.round
```
```python
(
```
```python
1000
```
```python
*np
```
```python
.arange
```
```python
(
```
```python
256
```
```python
)/
```
```python
512
```
```python
), Pyy[:
```
```python
256
```
```python
])
plt
```
```python
.show
```
```python
()
```

