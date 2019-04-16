# R语言并行运算(snow包) - littlely_ll的博客 - CSDN博客





2017年02月08日 18:01:06[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：2694标签：[r语言																[并行运算																[snow包](https://so.csdn.net/so/search/s.do?q=snow包&t=blog)
个人分类：[并行运算](https://blog.csdn.net/littlely_ll/article/category/6706924)





# snow包

snow包主要运行于传统的集群计算。它主要用于蒙特卡洛模拟（Monte Carlo simulations），boostrapping，交叉验证（cross validation），集成机器学习算法（ensemble machine learning algorithms）和K均值聚类（K-Means clustering）等，并且也很好支持并行随机数的生成。 

snow包的安装一般应用`install.packages('snow')`. 

要使用snow包进行并行运算，首先要创建一个集群对象，主要用于与内核的交互，一般传递与snow包中函数的第一个参数。 

基本的集群创建函数为*makeCluster()*，它能创建任何类型的集群，例如，在本地机器上创建一个4核的集群：`cl <- makeCluster(4, type="SOCK")`
第一个参数为集群设定，即设定使用的CPU数；第二个参数是集群类型。 
**注意：**在结束集群时使用`stopCluster()`。 

下面使用snow包做一个并行运算。
# 并行运算K均值聚类

数据是MASS包自带的Boston数据集。 

K均值函数使用stats包中的*kmeans()*。

```
library(MASS)
result <- kmeans(Boston, 4, nstart = 100)
```

在使用并行计算前，首先看看使用*lapply*函数的运行情况

```
results <- lapply(rep(25,4), function(nstart) kmeans(Boston, 4, nstart = nstart)
i <- sapply(results, function(result) result$tot.withiness)
result <- results[[which.min(i)]]
```

## 使用clusterApply并行计算

```
library(snow)
cl <- makeCluster(4, type="SOCK")
ignore <- clusterEvalQ(cl, {library(MASS); NULL}) #用clusterEvalQ函数初始化内核，在每一个内核中载入包
results <- clusterApply(cl, rep(25, 4), function(nstart) kmeans(Boston, 4, nstart=nstart)) #parallel computing
i <- sapply(results, function(result) result$tot.withinss)
result <- results[[which.min(i)]]
```

相比较`clusterEvalQ`而言，`clusterCall`更好用，它的第一个参数为集群对象，第二个参数为一个函数，并且其他的参数可以传递给这个函数。最终返回一个列表。

```php
clusterCall(cl, function() { library(MASS); NULL })
```

*clusterCall*函数可以调用多个包，

```
worker.init <- function(packages) {
for (p in packages) {
library(p, character.only=TRUE)
}
NULL
}
clusterCall(cl, worker.init, c('MASS', 'boot'))
```

`character.only=TRUE`是使*library*把参数解释为字符向量，否则*library*会重复载入名称为p的包。

## 使用clusterApplyLB并行运算

*clusterApplyLB*与*clusterApply*函数相似，但*clusterApplyLB*的运行效率更高。在使用*clusterApply*并行运算时，如果每一个内核花费不同的时间进行运算，那么在那个运行时间长的内核结束之前，运行时间短的内核不能进行下一次运算，而*clusterApplyLB*不同，它是在运行时间短的内核结束之后接着就运行下一次的运算，这样就减少了时间的浪费，因此提高了效率。 

为了说明*clusterApplyLB*的效率，我们使用控制任务时间长度的函数*Sys.sleep*。用*snow.time*收集整个执行过程的时间信息。

```
set.seed(7777442)
sleeptime <- abs(rnorm(10, 10, 10))
tm <- snow.time(clusterApplyLB(cl, sleeptime, Sys.sleep))
plot(tm)
```

![clusterApplyLB](https://img-blog.csdn.net/20170208153308760?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

作为对照，我们用*clusterApply*函数来观察运行时间
```
tm1 <- snow.time(clusterApply(cl, sleeptime, Sys.sleep))
plot(tm1)
```

![clusterApply](https://img-blog.csdn.net/20170208153626028?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

由此看出*clusterApplyLB*效率更高。
## 使用parLapply并行运算

*parLapply*是snow包中的一个高级函数，它比*clusterApply*函数更高效，如果参数x的长度与内核的数量相等，*parLapply*的优势不明显，如果参数x的长度远大于内核的数量，*parLapply*相比于*clusterApply*是一个更好的选择。 

应用一个并行休眠函数的用法，首先使用*clusterApply*

```php
bigsleep <- function(sleeptime, mat) Sys.sleep(sleeptime)
bigmatrix <- matrix(0, 2000, 2000)
sleeptime <- rep(1, 100)
tm2 <- snow.time(clusterApply(cl, sleeptime, bigsleep, bigmatrix))
plot(tm2)
```

![clusterApply](https://img-blog.csdn.net/20170208174059886?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

由图看出并不高效，因为有大量的输入输出时间，下面再试一试*clusterApplyLB*：
```
tm3 <- snow.time(clusterApplyLB(cl, sleeptime, bigsleep, bigmatrix))
plot(tm3)
```

![clusterApplyLB](https://img-blog.csdn.net/20170208175537641?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

由上图看出，运算集中在第1、2和3个内核上，第4个内核上运算的时间非常少，及运算效率差，但总体还是比*clusterApply*函数要好一些。 

看看最终的函数*parLapply*：
```
tm4 <- snow.time(parLapply(cl, sleeptime, bigsleep, bigmatrix))
plot(tm4)
```

![parLapply](https://img-blog.csdn.net/20170208174918433?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

由此看出，传输交换的时间大大缩短了，因此提高了效率。
**$参考文献$**

  1. [Luke Tierney, A. J. Rossini, Na Li, H. Sevcikova, *package snow*.](https://cran.r-project.org/web/packages/snow/snow.pdf)

  2. [Q. Ethan McCallum and Stephen Weston, *Parallel R*.](http://shop.oreilly.com/product/0636920021421.do)](https://so.csdn.net/so/search/s.do?q=并行运算&t=blog)](https://so.csdn.net/so/search/s.do?q=r语言&t=blog)




