# 时间序列分析之AR、MA、ARMA和ARIMA模型 - 一个跳popping的quant的博客 - CSDN博客





2018年07月02日 15:50:29[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：7025
所属专栏：[一个用人工智能做量化的小白的进阶之路](https://blog.csdn.net/column/details/20644.html)[时间序列分析](https://blog.csdn.net/column/details/34308.html)









如果一个时间序列经过平稳性检验后得到是一个平稳非白噪声序列，那么该序列中就蕴含着相关性的信息。

在统计学中，通常是建立一个线性模型来拟合该时间序列的趋势。其中，AR、MA、ARMA以及ARIMA都是较为常见的模型。

### 1、AR(Auto Regressive Model)自回归模型

AR是线性时间序列分析模型中最简单的模型。通过自身前面部分的数据与后面部分的数据之间的相关关系（自相关）来建立回归方程，从而可以进行预测或者分析。下图中展示了一个时间如果可以表示成如下结构，那么就说明它服从p阶的自回归过程，表示为AR(p)。其中，ut表示白噪声，是时间序列中的数值的随机波动，但是这些波动会相互抵消，最终是0。theta表示自回归系数。

![](https://img-blog.csdn.net/20180702145930344?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


所以当只有一个时间记录点时，称为一阶自回归过程，即AR(1)。

![](https://img-blog.csdn.net/20180702150118629?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


### 2、MA(Moving Average Model)移动平均模型

通过将一段时间序列中白噪声序列进行加权和，可以得到移动平均方程。如下图所示为q阶移动平均过程，表示为MA(q)。theta表示移动回归系数。ut表示不同时间点的白噪声。


![](https://img-blog.csdn.net/20180702151637219?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





### 3、ARMA(Auto Regressive and Moving Average Model)自回归移动平均模型

自回归移动平均模型是与自回归和移动平均模型两部分组成。所以可以表示为ARMA(p, q)。p是自回归阶数，q是移动平均阶数。

![](https://img-blog.csdn.net/20180702152652146?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

从式子中就可以看出，自回归模型结合了两个模型的特点，其中，AR可以解决当前数据与后期数据之间的关系，MA则可以解决随机变动也就是噪声的问题。






### 4、ARIMA(Auto Regressive Integrate Moving Average Model)差分自回归移动平均模型

同前面的三种模型，ARIMA模型也是基于平稳的时间序列的或者差分化后是稳定的，另外前面的几种模型都可以看作ARIMA的某种特殊形式。表示为ARIMA(p, d, q)。p为自回归阶数，q为移动平均阶数，d为时间成为平稳时所做的差分次数，也就是Integrate单词的在这里的意思。

具体步骤如下：


- 获取被观测系统时间序列数据；
- 对数据绘图，观测是否为平稳时间序列；对于非平稳时间序列要先进行d阶差分运算，化为平稳时间序列；
- 经过第二步处理，已经得到平稳时间序列。要对平稳时间序列分别求得其自相关系数ACF 和偏自相关系数PACF，通过对自相关图和偏自相关图的分析，得到最佳的阶层 p 和阶数 q
- 由以上得到的d、q、p，得到ARIMA模型。然后开始对得到的模型进行模型检验。
ARIMA的详细可以参考：[https://www.cnblogs.com/bradleon/p/6827109.html](http://https//www.cnblogs.com/bradleon/p/6827109.html)









