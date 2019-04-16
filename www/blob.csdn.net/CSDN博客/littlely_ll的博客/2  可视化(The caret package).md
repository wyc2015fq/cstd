# 2. 可视化(The caret package) - littlely_ll的博客 - CSDN博客





2017年02月28日 14:58:47[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：1099
所属专栏：[R语言caret包应用](https://blog.csdn.net/column/details/29369.html)









[1. 简介(The caret package )](http://blog.csdn.net/littlely_ll/article/details/58592298)

# 2. 可视化

`featurPlot`是对lattice包作图的一个包装。例如：下列图表展示了用`featurePlot`函数画出的连续变量结果的图形。 

 对于分类的数据集可用`iris`数据来分析。 
`str(iris)`
```
## 'data.frame':    150 obs. of  5 variables:
##  $ Sepal.Length: num  5.1 4.9 4.7 4.6 5 5.4 4.6 5 4.4 4.9 ...
##  $ Sepal.Width : num  3.5 3 3.2 3.1 3.6 3.9 3.4 3.4 2.9 3.1 ...
##  $ Petal.Length: num  1.4 1.4 1.3 1.5 1.4 1.7 1.4 1.5 1.4 1.5 ...
##  $ Petal.Width : num  0.2 0.2 0.2 0.2 0.2 0.4 0.3 0.2 0.2 0.1 ...
##  $ Species     : Factor w/ 3 levels "setosa","versicolor",..: 1 1 1 1 1 1 1 1 1 1 ...
```

**散点图矩阵**

```
library(AppliedPredictiveModeling)
transparentTheme(trans = 0.4)
library(caret)
featurePlot(x = iris[,1:4],
             y = iris$Species,
             plot = "pairs",
             ## Add a key at the top
             auto.key = list(columns = 3))
```

![这里写图片描述](https://img-blog.csdn.net/20170228143240638?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**散点图矩阵加椭圆**
```php
featurePlot(x = iris[, 1:4], 
            y = iris$Species, 
            plot = "ellipse",
            ## Add a key at the top
            auto.key = list(columns = 3))
```

![这里写图片描述](https://img-blog.csdn.net/20170228143648801?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**覆盖密度图**
```
transparentTheme(trans = .9)
featurePlot(x = iris[, 1:4], 
            y = iris$Species,
            plot = "density", 
            ## Pass in options to xyplot() to 
            ## make it prettier
            scales = list(x = list(relation="free"), 
                          y = list(relation="free")), 
            adjust = 1.5, 
            pch = "|", 
            layout = c(4, 1), 
            auto.key = list(columns = 3))
```

![这里写图片描述](https://img-blog.csdn.net/20170228143815553?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**箱线图**
```
featurePlot(x = iris[, 1:4], 
            y = iris$Species, 
            plot = "box", 
            ## Pass in options to bwplot() 
            scales = list(y = list(relation="free"),
                          x = list(rot = 90)),  
            layout = c(4,1 ), 
            auto.key = list(columns = 2))
```

![这里写图片描述](https://img-blog.csdn.net/20170228144006148?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**散点图**

用Boston Housing数据进行回归：
```
library(mlbench)
data(BostonHousing)
regVar <- c("age", "lstat", "tax")
str(BostonHousing[, regVar])
```

```
## 'data.frame':    506 obs. of  3 variables:
##  $ age  : num  65.2 78.9 61.1 45.8 54.2 58.7 66.6 96.1 100 85.9 ...
##  $ lstat: num  4.98 9.14 4.03 2.94 5.33 ...
##  $ tax  : num  296 242 242 222 222 222 311 311 311 311 ...
```

当预测子是连续的时候，`featurePlot`可对每个预测子及结果变量创建散点图。例如：

```
theme1 <- trellis.par.get()
theme1$plot.symbol$col = rgb(.2, .2, .2, .4)
theme1$plot.symbol$pch = 16
theme1$plot.line$col = rgb(1, 0, 0, .7)
theme1$plot.line$lwd <- 2
trellis.par.set(theme1)
featurePlot(x = BostonHousing[, regVar], 
            y = BostonHousing$medv, 
            plot = "scatter", 
            layout = c(3, 1))
```

![这里写图片描述](https://img-blog.csdn.net/20170228144835511?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

注意到x坐标刻度的不同，函数自动的使用`scale = list(y = list(relation = "free"))`，所以你不用加上它。我们也可以向lattice函数`xyplot`中传递其他选项。例如，我们可以通过传递新选项加入散点图平滑项：
```
featurePlot(x = BostonHousing[, regVar], 
            y = BostonHousing$medv, 
            plot = "scatter",
            type = c("p", "smooth"),
            span = .5,
            layout = c(3, 1))
```

![这里写图片描述](https://img-blog.csdn.net/20170228145450124?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
`degree`和`span`选项控制平滑项的平滑度。









