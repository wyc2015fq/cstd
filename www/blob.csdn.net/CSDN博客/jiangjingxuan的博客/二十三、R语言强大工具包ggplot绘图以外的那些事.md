# 二十三、R语言强大工具包ggplot绘图以外的那些事 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 10:31:14[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：2028












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_391_default_big.png)



ggplot是R语言最为强大的作图软件包，除了绘图本身的功能之外，还有很多绘图周边的细节需要掌握才能绘出一张完美的图像，本节我们围绕绘图周边来详细讲解

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 画布定位

先看这张图：

```
> x <- c(1,2,3)
> y <- c(1,3,4)
> data <- data.frame(x,y)
> ggplot(data, aes(x = x, y = y)) + geom_point()
```

![](http://www.shareditor.com/uploads/media/my-context/0001/01/51b4054ed36de323cf5f742ff832b826059e88f7.png)

如果我们希望让画布再大一些，让这三个点集中一些怎么办？我们可以调整画布的坐标范围，以下两种方法效果是一样的：

```
> ggplot(data, aes(x = x, y = y)) + geom_point() + expand_limits(x = c(0, 4), y = c(0, 5))
> ggplot(data, aes(x = x, y = y)) + geom_point() + xlim(0, 4) + ylim(0, 5)
```

![](http://www.shareditor.com/uploads/media/my-context/0001/01/dac5f1cd914c47f498785789acc6cc414a7dcba2.png)



## 修改点的形状

我们可以画出多种点的形状
`> ggplot(data,  aes(x, z)) + geom_point(aes(shape = y))`
![](http://www.shareditor.com/uploads/media/my-context/0001/01/44637f74e883a9f5f3d07aba20ca5d88aa77b920.png)

我们也可以把形状画成空心状的：
`> ggplot(data,  aes(x, z)) + geom_point(aes(shape = y))+ scale_shape(solid = FALSE)`
![](http://www.shareditor.com/uploads/media/my-context/0001/01/066f3c343103ad9034c237181de347267fad02b6.png)

当然我们还可以调整点的大小：
`> ggplot(data,  aes(x, z, size=z)) + geom_point(aes(shape = y))+ scale_shape(solid = FALSE)`
![](http://www.shareditor.com/uploads/media/my-context/0001/01/4a92fbee3ab41a30240896fef880a2d0842727a7.png)



## 各种标注方法

可以通过如下两种方式来添加title、x轴标签、y轴标签，效果是一样的，如下：

```cpp
> ggplot(data,  aes(x,y)) + geom_point() + labs(title = "my title") +labs(x = "New x label") +labs(y = "New y label")
> ggplot(data,  aes(x,y)) + geom_point() + ggtitle("my title") + xlab("New x label") + ylab("New y label")
```

![](http://www.shareditor.com/uploads/media/my-context/0001/01/b61bfe2aa162476703da1a5d64c40fb6849f44f2.png)

我们还可以在某一个坐标位置写一句话

```bash
> ggplot(data,  aes(x,y)) + geom_point() + ggtitle("my title") + xlab("New x label") + ylab("New y label") + annotate("text", x = 2, y = 25, label = "Some text")
```

![](http://www.shareditor.com/uploads/media/my-context/0001/01/54b995af03ca142d82e152b6ce12867b36a946b7.png)

我们还可以在某一个范围画一个矩形来重点标注

```bash
> ggplot(data,  aes(x,y)) + geom_point() + ggtitle("my title") + xlab("New x label") + ylab("New y label") + annotate("rect", xmin = 1.75, xmax = 2.25, ymin = 18, ymax = 22, alpha = .2)
```

![](http://www.shareditor.com/uploads/media/my-context/0001/01/9a6e8d00635be79127ab7e27534494c40bcfd9e0.png)

也可以在某一个范围画一条线段

```bash
> ggplot(data,  aes(x,y)) + geom_point() + ggtitle("my title") + xlab("New x label") + ylab("New y label") + annotate("segment", x = 1.75, xend = 2.25, y = 18, yend = 22, colour = "blue")
```

![](http://www.shareditor.com/uploads/media/my-context/0001/01/1b0414211faf998f61d5577b667d923a21cc7ddd.png)



## 坐标系统

我们看下面这个例子：

```
> x <- c(1,2,3)
> y <- c(10,20,30)
> data <- data.frame(x, y)
>  ggplot(data,  aes(x,y)) + geom_point()
```

![](http://www.shareditor.com/uploads/media/my-context/0001/01/b16ea9b3a8bea73d3b0cdf971d1c0fe9584ae60e.png)

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

我们发现坐标上x和y是不等比例的，x的宽度1相当于y宽度10，怎么样可以让其等比例显示呢？
`>  ggplot(data,  aes(x,y)) + geom_point() + coord_fixed(ratio = 1)`
![](http://www.shareditor.com/uploads/media/my-context/0001/01/29e6bbe80f8f56f4c4600a0168840e80aa451cfb.png)

如果我们希望横过来显示，那么也可以这样让坐标轴对调：
`>  ggplot(data,  aes(x,y)) + geom_point() + coord_flip()`
![](http://www.shareditor.com/uploads/media/my-context/0001/01/ed1ff9d4ad6fd1d3406771ecfe310001b3309dc5.png)

有时我们希望把坐标变换成极坐标，如下：

```bash
>  ggplot(data,  aes(x,y)) + geom_point() +coord_polar(theta = "y")
```

![](http://www.shareditor.com/uploads/media/my-context/0001/01/3baa73d6b7dec134dec9f6468f489ffe7eb2d76b.png)



## 分网格展示

分网格显示便于把不同组数据分离观察

按x的值分成多行
`>  ggplot(data,  aes(x,y)) + geom_point() + facet_grid(x ~ .)`
![](http://www.shareditor.com/uploads/media/my-context/0001/01/70b4705ee30c10f178a833974846c5356ec31867.png)

按y的值分成多列
`>  ggplot(data,  aes(x,y)) + geom_point() + facet_grid(. ~ y)`
![](http://www.shareditor.com/uploads/media/my-context/0001/01/d68bcb01feb1ec22ac65cd6c47b70fef439240e8.png)

按x和y分成网格
`>  ggplot(data,  aes(x,y)) + geom_point() + facet_grid(x ~ y)`
![](http://www.shareditor.com/uploads/media/my-context/0001/01/36544f837c492ab698e39b085c235381eaab1c66.png)

按照某一个类别分成多个网格

```
> x
[1] 1 2 3
> y
[1] 10 20 30
> z <- c("type1", "type2", "type1")
> data <- data.frame(x, y, z)
>  ggplot(data,  aes(x,y)) + geom_point() + facet_wrap(~z)
```

![](http://www.shareditor.com/uploads/media/my-context/0001/01/503d5eb8b5d84c3a83212cf74a3660d11f592ff9.png)

如果我们希望分两行展示，那么可以：
`>  ggplot(data,  aes(x,y)) + geom_point() + facet_wrap(~z, nrow=2)`
![](http://www.shareditor.com/uploads/media/my-context/0001/01/82b1a11f77bd2c60d5a0264a72798e1e0effa9cc.png)



## 主题风格

我们可以选择不同的主题风格，像如下几种，当然还有很多：

```
> ggplot(data,  aes(x,y)) + geom_point() +  theme_light()
> ggplot(data,  aes(x,y)) + geom_point() +  theme_dark()
> ggplot(data,  aes(x,y)) + geom_point() +  theme_gray()
```

![](http://www.shareditor.com/uploads/media/my-context/0001/01/0e696fc501ab0931b5daccebfa01183405bb933b.png)

## 快速画图

如果我们需求比较简单，不想写那么长的命令，可以使用qplot来简单画图，它实际上也是自动转化成ggplot来画图的

比如我们要画一个y = x^2的曲线，那么可以这样：

```
> a <- -100:100
> b <- a ^ 2
> qplot(a, b)
```

![](http://www.shareditor.com/uploads/media/my-context/0001/01/fc34b8a97a44f83628ca130a2737c9c049c66b92.png)




