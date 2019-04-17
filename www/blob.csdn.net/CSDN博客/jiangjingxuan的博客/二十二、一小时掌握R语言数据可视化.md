# 二十二、一小时掌握R语言数据可视化 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 10:30:33[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：2165












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_372_default_big.png)



ggplot2是R语言最为强大的作图软件包，有着自成一派的可视化理念，数据可视化是数据分析的重要一步，让我们通过由浅入深的掌握数据可视化的精髓。

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 展开一张画布

ggplot2和其他作图工具不同，它是以图层覆盖图层的方式画出一个完美图像的，就像是photoshop里的图层，那么首先我们得有一张画布（如果没有安装R语言和ggplot2请见《[十八-R语言特征工程实战](http://www.shareditor.com/blogshow/?blogId=106)》）

```
[root@centos $] R
> library(ggplot2)
> ggplot()
```

![](http://www.shareditor.com/uploads/media/my-context/0001/01/04f127da8c76a5582c46f04e25faa8f9e0b13e22.png)



## 使用geom_abline、geom_hline、geom_vline画直线

下面我们来在这张画布上画一条横线：
`> ggplot() + geom_hline(yintercept = 5)`
![](http://www.shareditor.com/uploads/media/my-context/0001/01/fcacbace139391d0dead72ccb2d94075fb0d2edc.png)

我们也可以画一条竖线
`> ggplot() + geom_vline(xintercept = 5)`
![](http://www.shareditor.com/uploads/media/my-context/0001/01/712810550b5009078caa646c0e6ae7a809803f97.png)

当然我们也可以画斜线，
`> geom_abline(intercept = 2.5, slope=1)`
本应该画一条斜率为1，截距为2.5的斜线，但是因为画布不会自动移动到这条直线所在的位置，所以我们要实现几个点来定位一下画布，那么怎么画点呢，我们先来研究一下



## 使用geom_point画点

下面我们来一张空画布上画一个点，画点和画线不同在于：线可以指定一个x或y的截距就可以了，可以作为一个简单的参数传给geom_hline或geom_vline，但是画点涉及到的是一些x、y的数据值，ggplot是把数据和作图撇清的，也就是数据是数据，成像是成像

我们先来构造点：

```
> x <- c(1,2,3)
> y <- c(1,3,4)
> data <- data.frame(x,y)
> str(data)
'data.frame':      3 obs. of  2 variables:
 $ x: num  1 2 3
 $ y: num  1 3 4
```

我们其实构建了一个frame，里面包含了三个点：(1,1), (2,3), (3,4)

那么如果要画出这些点的话应该这样：
`> ggplot(data, aes(x = x, y = y)) + geom_point()`
![](http://www.shareditor.com/uploads/media/my-context/0001/01/20c8311885d29f7ba3cb24b25988a865b2e75df6.png)

前面是声明数据部分，后面是声明怎么成像

下面我们开始调整geom_point的参数，比如展示不同的颜色(左)，和展示不同的形状(右)

```
> ggplot(data, aes(x, y)) + geom_point(aes(colour = factor(y)))
> ggplot(data, aes(x, y)) + geom_point(aes(shape = factor(y)))
```

![](http://www.shareditor.com/uploads/media/my-context/0001/01/1a1e45a6bb7ee363086c1f5b3a1478b3e09db436.png)

如果颜色不是按factor区分，而是按连续值来区分，那么就是渐变形式，即
`> ggplot(data, aes(x, y)) + geom_point(aes(colour = y))`
![](http://www.shareditor.com/uploads/media/my-context/0001/01/9b85ad845c9911b2f54997b9f03b95a511d85c06.png)

还可以展示不同的大小，可以固定大小(左)，也可以根据数据确定大小(右)

```
> ggplot(data, aes(x, y)) + geom_point(aes(size = 3))
> ggplot(data, aes(x, y)) + geom_point(aes(size = y))
```

![](http://www.shareditor.com/uploads/media/my-context/0001/01/a010e5465b841e28046bf21b22b054ec49acc289.png)

这里我们要说明一下aes的作用，看下面两个用法(如图左、右)：左边的含义就是画红色点，右边是按照指定的一个维度展示不同的颜色

```bash
> ggplot(data, aes(x, y)) + geom_point(colour="red")
> ggplot(data, aes(x, y)) + geom_point(aes(colour="red"))
```

![](http://www.shareditor.com/uploads/media/my-context/0001/01/0769205f32e4fa83af240e002c15b26ef893a119.png)

接着上面划线一节，我们在已经画了点的画布上再画一条斜线：一条斜率为1，截距为1的直线，也就是y=x+1，那么一定是经过(2,3),(3,4)两个点的
`> ggplot(data, aes(x, y)) + geom_point(aes(colour = y)) + geom_abline(slope = 1, intercept = 1)`
![](http://www.shareditor.com/uploads/media/my-context/0001/01/5598075562a923f9199a48d870b52969c50dd9f0.png)

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址



## 使用geom_bar来画直方图

直观上看，直方图是表达一种累积量，因此默认的直方图的高度是counts或sum，也就是像下面这样子：因为我们的x只有1、2、3单独的三个值，所以直接geom_bar()高度相同，但如果判断x<2，那么有一个满足，两个不满足，所以高度分别是1和2

```
> ggplot(data, aes(x)) + geom_bar()
> ggplot(data, aes(x<2)) + geom_bar()
```

![](http://www.shareditor.com/uploads/media/my-context/0001/01/125052165be2291de5c1bd7da924093de5c6ff72.png)

当然我们可以自己指定直方图的高度的计算方法，以下两种方法效果相同

```
> ggplot(data, aes(x)) + geom_bar(aes(weight=y))
> ggplot(data, aes(x,y)) + geom_bar(stat = "identity")
```

![](http://www.shareditor.com/uploads/media/my-context/0001/01/9c7bd0cb64f8a0921b38580e12d99b541641ead0.png)

如果我们想要把多种取值的统计数目累加显示在柱状图上，可以这样：这里面对同一个x，不同y出现总数不一样，累加起来就像下图展示，其中如果y是数字，那么想把他当成类别，需要转成factor

```
> x <- rep(c(1,2), c(2,3))
> y <- rep(c(3,2), c(1,4))
> data <- data.frame(x,y)
> ggplot(data, aes(x)) + geom_bar(aes(fill=factor(y)))
```

![](http://www.shareditor.com/uploads/media/my-context/0001/01/a7b87927553bceceee186d9e54aa791b5dc70596.png)

当然我们也可以不简单堆叠起来，比如扁平放置(左)，或拉伸至顶部(右)

```bash
> ggplot(data, aes(x)) + geom_bar(aes(fill=factor(y)), position="dodge")
> ggplot(data, aes(x)) + geom_bar(aes(fill=factor(y)), position="fill")
```

![](http://www.shareditor.com/uploads/media/my-context/0001/01/8fbeac89074f2ab4a6d08a7029ea1e1de83955d1.png)



## 利用geom_density画概率密度曲线

概率密度就是某些值出现的频次多少的一个曲线，并做平滑，如下：

```
> x <- rep(c(1,3,7,11,23,50,60),c(1,30,400,60,4,55,11))
> y <- rep(c(1,3,7,11,23,50,60),c(1,30,400,60,4,55,11))
> data <- data.frame(x,y)
> ggplot(data, aes(x)) + geom_density()
```

![](http://www.shareditor.com/uploads/media/my-context/0001/01/f6dabf09178c4f5a4e12e31c5d730987e9f1e687.png)

我们可以调整平滑的宽度：
`> ggplot(data, aes(x)) + geom_density(adjust = 1/5)`
![](http://www.shareditor.com/uploads/media/my-context/0001/01/4f8b74a3716bb07371296806a5f5c7c2f2f2e38d.png)

如果我们想按照不同的y值来分开画密度图，并且用不同颜色来表示不同的y值，那么我们可以用描边的方式(左)，也可以用填充的方式(中)，当然也可以两者结合

```
> ggplot(data, aes(x, colour = factor(y))) + geom_density(adjust = 1/5)
> ggplot(data, aes(x, fill = factor(y))) + geom_density(adjust = 1/5)
> ggplot(data, aes(x, colour = factor(y), fill = factor(y))) + geom_density(adjust = 1/5, alpha = 0.1)
```

![](http://www.shareditor.com/uploads/media/my-context/0001/01/86b8dca8e1031fa31a25b86e985779e11e682218.png)

和柱状图一样，我们也可以通过geom_density的position参数来显示累计情况：

```bash
> ggplot(data, aes(x, fill = factor(y))) + geom_density(adjust = 1/5, position='fill')
> ggplot(data, aes(x, fill = factor(y))) + geom_density(adjust = 1/5, position='stack')
```

![](http://www.shareditor.com/uploads/media/my-context/0001/01/5baced5718ba42f39338a8ee7d9d98bdf5cf50c9.png)



## 用geom_text和geom_label写标注文本

为了让图像更清晰，我们需要把关键数据打上标签展示出来，我们可以这样做：

```
> ggplot(data, aes(x, y, label=rownames(data))) + geom_point(aes(colour = y)) + geom_abline(slope = 1, intercept = 1) + geom_text(check_overlap = TRUE)
> ggplot(data, aes(x, y, label=rownames(data))) + geom_point(aes(colour = y)) + geom_abline(slope = 1, intercept = 1) + geom_label()
```

![](http://www.shareditor.com/uploads/media/my-context/0001/01/6b14e409a7b4b22adefe1affd39385a15cafd137.png)



## 总结

本节介绍了ggplot作图原理以及基本的几种作图方式，基于这些知识相信你很容易能做出精美的图像了




