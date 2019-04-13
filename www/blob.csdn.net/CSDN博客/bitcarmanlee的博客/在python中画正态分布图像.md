
# 在python中画正态分布图像 - bitcarmanlee的博客 - CSDN博客


2018年01月24日 18:06:08[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：16887



## 1.正态分布简介
正态分布(normal distribtution)又叫做高斯分布(Gaussian distribution)，是一个非常重要也非常常见的连续概率分布。正态分布大家也都非常熟悉，下面做一些简单的介绍。
假设随机变量$X$服从一个位置参数为$\mu$、尺度参数为$\sigma$的正态分布，则可以记为：
$$
{\displaystyle X\sim N(\mu ,\sigma ^{2})}
$$
而概率密度函数为
$$
{\displaystyle f(x)={1 \over \sigma {\sqrt {2\pi }}}\,e^{-{(x-\mu )^{2} \over 2\sigma ^{2}}}}
$$

## 2.在python中画正态分布直方图
先直接上代码
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
import
```
```python
matplotlib.mlab
```
```python
as
```
```python
mlab
```
```python
import
```
```python
matplotlib.pyplot
```
```python
as
```
```python
plt
```
```python
def
```
```python
demo1
```
```python
()
```
```python
:
```
```python
mu ,sigma =
```
```python
0
```
```python
,
```
```python
1
```
```python
sampleNo =
```
```python
1000
```
```python
np.random.seed(
```
```python
0
```
```python
)
    s = np.random.normal(mu, sigma, sampleNo)
    plt.hist(s, bins=
```
```python
100
```
```python
, normed=
```
```python
True
```
```python
)
    plt.show()
```
上面是一个标准正态分布的直方图。最后输出的图像为：
![这里写图片描述](https://img-blog.csdn.net/20180124174948060?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
很多同学心里会有疑惑：这个图像看上去虽然是有点奇怪，虽然形状有点像正态分布，但是差得还比较多嘛，不能算是严格意义上的正态分布。
为什么会有这种情况出现呢？其实原因很简单，代码中我们设定的smapleno = 1000。这个数量并不是很大，所以整个图像看起来分布并不是很规则，只是有大致的正态分布的趋势。如果我们将这个参数加大，相当于增加样本数量，那么整个图像就会更加接近正态分布的形状。跟抛硬币的原理一致，抛的次数越多，正面与反面的出现概率更接近50%。
如果我们将sampleno设置为1000000，分布图像如下。
![这里写图片描述](https://img-blog.csdn.net/20180124175839333?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
下面这个图像是不是看起来就漂亮多了！
## 3.画直方图与概率分布曲线
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
import
```
```python
matplotlib.mlab
```
```python
as
```
```python
mlab
```
```python
import
```
```python
matplotlib.pyplot
```
```python
as
```
```python
plt
```
```python
def
```
```python
demo2
```
```python
()
```
```python
:
```
```python
mu, sigma , num_bins =
```
```python
0
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
50
```
```python
x = mu + sigma * np.random.randn(
```
```python
1000000
```
```python
)
```
```python
# 正态分布的数据
```
```python
n, bins, patches = plt.hist(x, num_bins, normed=
```
```python
True
```
```python
, facecolor =
```
```python
'blue'
```
```python
, alpha =
```
```python
0.5
```
```python
)
```
```python
# 拟合曲线
```
```python
y = mlab.normpdf(bins, mu, sigma)
    plt.plot(bins, y,
```
```python
'r--'
```
```python
)
    plt.xlabel(
```
```python
'Expectation'
```
```python
)
    plt.ylabel(
```
```python
'Probability'
```
```python
)
    plt.title(
```
```python
'histogram of normal distribution: $\mu = 0$, $\sigma=1$'
```
```python
)
    plt.subplots_adjust(left =
```
```python
0.15
```
```python
)
    plt.show()
```
最后得到的图像为：
![这里写图片描述](https://img-blog.csdn.net/20180124180513089?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

