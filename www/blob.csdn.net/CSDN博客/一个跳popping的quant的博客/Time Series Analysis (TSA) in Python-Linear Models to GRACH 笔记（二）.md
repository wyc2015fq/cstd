# Time Series Analysis (TSA) in Python-Linear Models to GRACH 笔记（二） - 一个跳popping的quant的博客 - CSDN博客





2019年01月21日 19:54:45[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：644
所属专栏：[时间序列分析](https://blog.csdn.net/column/details/34308.html)









以下内容主要来自：

[Time Series Analysis (TSA) in Python - Linear Models to GARCH](http://www.blackarbs.com/blog/time-series-analysis-in-python-linear-models-to-garch/11/1/2016#WNandRW=)

本篇目录：

[Linear Models](#Linear%20Models)

[Log-Linear Models](#Log-Linear%20Models)

# Linear Models

线性模型也称作趋势模型，它表示一个时间序列可以用一条直线来表示。它的基本等式：![y_{t} = b_{0} + b_{1}t +\varepsilon _{t}](https://private.codecogs.com/gif.latex?y_%7Bt%7D%20%3D%20b_%7B0%7D%20&plus;%20b_%7B1%7Dt%20&plus;%5Cvarepsilon%20_%7Bt%7D)

以一个公司的销售总额为例，一开始的初始是 5000，每隔一个时间步长增加 2500。

```python
# simulate linear trend
# example Firm ABC sales are -$50 by default and +$25 at every time step

w = np.random.randn(100)
y = np.empty_like(w)

b0 = -50.
b1 = 25.
for t in range(len(w)):
    y[t] = b0 + b1*t + w[t]

tsplot(y, lags=30)
```

![](https://img-blog.csdnimg.cn/20190120131140273.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)

 可以看出模型的残差是相关的，并且线性下降。这个分布大约是正态的。在用线性模型做预测时，我们应该考虑去除序列中明显的自相关的成分。PACF在滞后1的显著性表明这个自回归模型可能是合适的。

# Log-Linear Models

对数模型和线性模型很像，不同之处在于数据点形成的是一个指数函数，它代表了每个时间步长之间恒定的变化率。

下面以ABC公司的销售额为例，每个时间步长增加 X%。

```python
# Simulate ABC exponential growth

# fake dates
idx = pd.date_range('2007-01-01', '2012-01-01', freq='M')

# fake sales increasing at exponential rate
sales = [np.exp( x/12 ) for x in range(1, len(idx)+1)]

# create dataframe and plot
df = pd.DataFrame(sales, columns=['Sales'], index=idx)

with plt.style.context('bmh'):
    df.plot()
    plt.title('ABC Sales')
```



![](https://img-blog.csdnimg.cn/20190121193418122.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)

 对销售额取一个自然对数，这样得到的一个线性回归就可以更好地来地和数据。

```python
# ABC log sales 

with plt.style.context('bmh'):
    pd.Series(np.log(sales), index=idx).plot()
    plt.title('ABC Log Sales')
```



![](https://img-blog.csdnimg.cn/20190121193922113.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==,size_16,color_FFFFFF,t_70)

正如我们之前讨论的，这些模型有一个致命的弱点，那就是他们都假设序列不相关的误差，在线性模型中这样是不正确的。在现实中，时间序列往往都是违背平稳性假设的，这就促使我们去研究自回归模型。



[Time Series Analysis (TSA) in Python-Linear Models to GRACH 笔记（三）](https://blog.csdn.net/FrankieHello/article/details/86583374)



