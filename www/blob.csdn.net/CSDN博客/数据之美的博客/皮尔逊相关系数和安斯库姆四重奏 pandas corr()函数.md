# 皮尔逊相关系数和安斯库姆四重奏  pandas corr()函数 - 数据之美的博客 - CSDN博客
2017年12月18日 18:35:02[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：1140
[《智能Web算法》](http://www.hankcs.com/tag/%E3%80%8A%E6%99%BA%E8%83%BDweb%E7%AE%97%E6%B3%95%E3%80%8B/)在讲相关系数的时候用到了皮尔逊相关系数，我茫然不知为何物，看了定义才知道原来就是概率与数理统计课里面的相关系数r。正好概率与数理统计课的东西考完就还回去了，这边做个笔记。
## 皮尔逊相关系数
援引维基百科的定义：
> 
两个变量之间的皮尔逊相关系数定义为两个变量之间的[协方差](http://zh.wikipedia.org/wiki/%E5%8D%8F%E6%96%B9%E5%B7%AE)和[标准差](http://zh.wikipedia.org/wiki/%E6%A0%87%E5%87%86%E5%B7%AE)的商：
- 
![\rho _{{X,Y}}={{\mathrm  {cov}}(X,Y) \over \sigma _{X}\sigma _{Y}}={E[(X-\mu _{X})(Y-\mu _{Y})] \over \sigma _{X}\sigma _{Y}},](http://upload.wikimedia.org/math/1/7/7/17709e96782a6a8bcd39904c5f2383e6.png)
非常可耻的是，我连协方差是什么都忘掉了，看来水到高中水平了。
> 
协方差（Covariance）在[概率论](http://zh.wikipedia.org/wiki/%E6%A6%82%E7%8E%87%E8%AB%96)和[统计学](http://zh.wikipedia.org/wiki/%E7%B5%B1%E8%A8%88%E5%AD%B8)中用于衡量两个变量的总体[误差](http://zh.wikipedia.org/wiki/%E8%AF%AF%E5%B7%AE)。而[方差](http://zh.wikipedia.org/wiki/%E6%96%B9%E5%B7%AE)是协方差的一种特殊情况，即当两个变量是相同的情况。
[期望值](http://zh.wikipedia.org/wiki/%E6%9C%9F%E6%9C%9B%E5%80%BC)分别为![E(X)=\mu](http://upload.wikimedia.org/math/6/0/3/60390643adab2c7ca8f71dcc3f7f80a7.png)与![E(Y)=\nu](http://upload.wikimedia.org/math/b/f/6/bf680d71c501085ab7650d67e8a2950c.png)的两个[实数](http://zh.wikipedia.org/wiki/%E5%AE%9E%E6%95%B0)[随机变量](http://zh.wikipedia.org/wiki/%E9%9A%8F%E6%9C%BA%E5%8F%98%E9%87%8F)X 与Y 之间的协方差定义为：
- 
![\operatorname {cov}(X,Y)=\operatorname {E}((X-\mu )(Y-\nu ))](http://upload.wikimedia.org/math/5/f/b/5fbe23eb934ddb586d90484274f7125a.png)，
其中E是期望值。它也可以表示为：
- 
![\operatorname {cov}(X,Y)=\operatorname {E}(X\cdot Y)-\mu \nu](http://upload.wikimedia.org/math/b/7/0/b70ed66ad7847048828191255e707505.png)，
如果用Java来实现的话，分子应当是下面的xy/n：
```java;toolbar
```
- // 为每个向量计算平均值
- double avgX = getAverage(x);
- double avgY = getAverage(y);
- 
- // 为每个向量计算标准差
- double sX = getStdDev(avgX, x);
- double sY = getStdDev(avgY, y);
- 
- double xy =0;
- 
- for(int i =0; i < n; i++)
- {
- // 计算平均偏差后的点积，也就是协方差
-             xy +=(x[i]- avgX)*(y[i]- avgY);
- }
分母应当是上面的sX * sY。
## 安斯库姆四重奏
安斯库姆四重奏是安斯库姆对皮尔逊相关系数的嘲讽，他用四幅图展示了皮尔逊相关系数相同的四个例子，这四个例子是完全不同的分布：
> 
安斯库姆四重奏（Anscombe's quartet）是四组基本的统计特性一致的数据，但由它们绘制出的图表则截然不同。每一组数据都包括了11个(x,y)点。这四组数据由统计学家[弗朗西斯·安斯库姆](http://zh.wikipedia.org/w/index.php?title=%E5%BC%97%E6%9C%97%E8%A5%BF%E6%96%AF%C2%B7%E5%AE%89%E6%96%AF%E5%BA%93%E5%A7%86&action=edit&redlink=1)（Francis
 Anscombe）于1973年构造，他的目的是用来说明在分析数据前先绘制图表的重要性，以及[离群值](http://zh.wikipedia.org/w/index.php?title=%E7%A6%BB%E7%BE%A4%E5%80%BC&action=edit&redlink=1)对统计的影响之大。
这四组数据的共同统计特性如下：
|性质|数值|
|----|----|
|x的[平均数](http://zh.wikipedia.org/wiki/%E5%B9%B3%E5%9D%87%E6%95%B0)|9|
|x的[方差](http://zh.wikipedia.org/wiki/%E6%96%B9%E5%B7%AE)|11|
|y的平均数|7.50（精确到小数点后两位）|
|y的方差|4.122或4.127（精确到小数点后三位）|
|x与y之间的[相关系数](http://zh.wikipedia.org/wiki/%E7%9B%B8%E5%85%B3%E7%B3%BB%E6%95%B0)|0.816（精确到小数点后三位）|
|[线性回归](http://zh.wikipedia.org/wiki/%E7%B7%9A%E6%80%A7%E5%9B%9E%E6%AD%B8)线|![y=3.00+0.500x](http://upload.wikimedia.org/math/b/e/0/be060d1e6fc0f46d0bfb63979b765e7d.png)（分别精确到小数点后两位和三位）|
[](http://zh.wikipedia.org/wiki/File:Anscombe%27s_quartet_3.svg)
![](http://bits.wikimedia.org/static-1.23wmf12/skins/common/images/magnify-clip.png)
安斯库姆四重奏的四组数据图表
在四幅图中，由第一组数据绘制的图表（左上图）是看起来最“正常”的，可以看出两个随机变量之间的相关性。从第二组数据的图表（右上图）则可以明显地看出两个随机变量间的关系是非线性的。第三组中（左下图），虽然存在着线性关系，但由于一个离群值的存在，改变了线性回归线，也使得相关系数从1降至0.81。最后，在第四个例子中（右下图），尽管两个随机变量间没有线性关系，但仅仅由于一个离群值的存在就使得相关系数变得很高。
真没想到数学这么有用。
