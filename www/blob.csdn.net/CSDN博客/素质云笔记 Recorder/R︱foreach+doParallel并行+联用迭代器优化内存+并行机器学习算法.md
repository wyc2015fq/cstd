# R︱foreach+doParallel并行+联用迭代器优化内存+并行机器学习算法 - 素质云笔记/Recorder... - CSDN博客





2016年11月26日 15:11:08[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：3918标签：[foreach																[R																[doParallel																[并行																[内存](https://so.csdn.net/so/search/s.do?q=内存&t=blog)
个人分类：[大数据、并行计算&R](https://blog.csdn.net/sinat_26917383/article/category/6093549)







**要学的东西太多，无笔记不能学~~ 欢迎关注公众号，一起分享学习笔记，记录每一颗“贝壳”~**![](https://img-blog.csdn.net/20161213101203247)


———————————————————————————


      接着之前写的并行算法parallel包，parallel相比foreach来说，相当于是foreach的进阶版，好多东西封装了。而foreach包更为基础，而且可自定义的内容很多，而且实用性比较强，可以简单的用，也可以用得很复杂。笔者将自己的学习笔记记录一下。

[](http://blog.csdn.net/sinat_26917383/article/details/52719232)[R︱并行计算以及提高运算效率的方式(parallel包、clusterExport函数、SupR包简介)](http://blog.csdn.net/sinat_26917383/article/details/52719232)






——————————————————————————————————————








# 一、foreach包简介与主要函数解读




      foreach包是revolutionanalytics公司贡献给R开源社区的一个包，它能使R中的并行计算更为方便。大多数并行计算都主要完成三件事情：将问题分割小块、对小块问题进行并行计算、合并计算结果。foreach包中，迭代器完成分割工作，”%dopar%“函数实现对小块的并行计算，”.combine”函数完成合并工作。





```
foreach(..., .combine, .init, .final=NULL, .inorder=TRUE,
       .multicombine=FALSE,
       .maxcombine=if (.multicombine) 100 else 2,
       .errorhandling=c('stop', 'remove', 'pass'),
       .packages=NULL, .export=NULL, .noexport=NULL,
       .verbose=FALSE)
when(cond)
e1 %:% e2
obj %do% ex
obj %dopar% ex
times(n)
```


参数解读：

（1）%do%严格按照顺序执行任务（所以，也就非并行计算），%dopar%并行执行任务，%do%时候就像sapply或lapply，%dopar%就是并行启动器
（2）.combine：运算之后结果的显示方式，default是list，“c”返回vector， cbind和rbind返回矩阵，"+"和"*"可以返回rbind之后的“+”或者“*”，帮你把数据整合起来，太良心了！！
（3）.init：.combine函数的第一个变量
（4）.final：返回最后结果
（5）.inorder：TRUE则返回和原始输入相同顺序的结果（对结果的顺序要求严格的时候），FALSE返回没有顺序的结果（可以提高运算效率）。这个参数适合于设定对结果顺序没有需求的情况。
（6）.muticombine：设定.combine函数的传递参数，default是FALSE表示其参数是2，TRUE可以设定多个参数
（7）.maxcombine：设定.combine的最大参数
（8）.errorhandling：如果循环中出现错误，对错误的处理方法
（9）.packages：指定在%dopar%运算过程中依赖的package（%do%会忽略这个选项），用于并行一些机器学习算法。
（10）.export:在编译函数的时候需要预先加载一些内容进去，类似parallel的clusterExport



如果你不知道自己的机器有没有启动并行，你可以通过以下的函数来进行查看，帮助你理解自己电脑的核心数：



```
getDoParWorkers( )    #查看注册了多少个核，配合doMC package中的registerDoMC( )使用
getDoParRegistered( ) # 查看doPar是否注册；如果没有注册返回FALSE
getDoParName( )       #查看已经注册的doPar的名字
getDoParVersion( )    #查看已经注册的doPar的version
```

本节内容主要参考：[R语言处理大数据](http://blog.sina.com.cn/s/blog_61f013b80100xxir.html)







——————————————————————————————————————








# 二、新手教程：foreach应用




## 1、最简单模式——堪比lapply






```
foreach(a=1:3, b=rep(10, 3)) %do% {
  a + b
}
## [[1]]
## [1] 11
## 
## [[2]]
## [1] 12
## 
## [[3]]
## [1] 13
```
      这个并不是并行，只是有着类似lapply的功能。
      foreach返回的是list格式值，list格式是默认的数据格式。来看看上面的内容怎么用lapply实现：



`lapply(cbind(1:3,rep(10,3),function(x,y) x+y ))`




但是有个小细节就是，%do%之后的{}可以随意写中间赋值过程，譬如c<-a+b，这个用lapply不是特别好写。所以这个我超级喜欢！

这里需要注意的一点是：a, b叫循环变量，循环次数取两者长度中最小的。譬如a=1,2,3 b=1,2，也就只能循环两次。




## 2、参数：.combine——定义输出结果的整合




      默认是foreach之后返回的是list，你可以指定自己想要的格式。.combine选项连接了“c”函数，该函数的功能是连接所有返回值组成向量。此外，我们可以使用“cbind”将生成的多个向量组合成矩阵,例如生成四组随机数向量，进而按列合并成矩阵：






```
foreach(i=1:4, .combine="cbind") %do% rnorm(4)
##      result.1 result.2 result.3 result.4
## [1,]  0.26634 -0.73193 -0.25927   0.8632
## [2,]  0.54132  0.08586  1.46398  -0.6995
## [3,] -0.15619  0.85427 -0.47997   0.2160
## [4,]  0.02697 -1.40507 -0.06972   0.2252
```

      运算之后结果的显示方式，default是list，“c”返回vector， cbind和rbind返回矩阵，"+"和"*"可以返回rbind之后的“+”或者“*”，帮你把数据整合起来。



      .combine还可以接上自己编译的函数，这点很人性化，譬如：






```
cfun <- function(a, b)  a+b
foreach(i=1:4, .combine="cfun") %do% rnorm(4)
```




.combine帮你把输出结果，再要调整的问题一次性解决了，并且将数据整合也进行并行加速，棒！




一些关于.combine的c,rbind,cbind,*,+其他案例：






```
x <- foreach(a=1:3, b=rep(10, 3), .combine="c") %do%
 {
    x1<-(a + b);
     x2<-a*b;
     c(x1,x2);  
 }
> x
[1] 11 10 12 20 13 30
> x <- foreach(a=1:3, b=rep(10, 3), .combine="rbind") %do%
 {
    x1<-(a + b);
     x2<-a*b;
     c(x1,x2);  
 }
> x
         [,1] [,2]
result.1   11   10
result.2   12   20
result.3   13   30
> x <- foreach(a=1:3, b=rep(10, 3), .combine="cbind") %do%
 {
     x1<-(a + b);
     x2<-a*b;
     c(x1,x2);  
 }
> x
     result.1 result.2 result.3
[1,]       11       12       13
[2,]       10       20       30
> x <- foreach(a=1:3, b=rep(10, 3), .combine="+") %do%
 {
     x1<-(a + b);
     x2<-a*b;
     c(x1,x2);  
 }
> x
[1] 36 60
> x <- foreach(a=1:3, b=rep(10, 3), .combine="*") %do%
 {
    x1<-(a + b);
     x2<-a*b;
     c(x1,x2);  
 }
> x
[1] 1716 6000
```





## 3、参数.inorder——定义输出结果的顺序




      .inorder：TRUE则返回和原始输入相同顺序的结果（对结果的顺序要求严格的时候），FALSE返回没有顺序的结果（可以提高运算效率）。这个参数适合于设定对结果顺序没有需求的情况。





      顺序这东西，写过稍微复杂的函数都知道，特别在数据匹配时尤为重要，因为你需要定义一些rownames的名称，这时候输出的顺序万一不匹配，可能后面还要花时间匹配过来。






```
foreach(i=4:1, .combine='c', .inorder=FALSE) %dopar% {
  Sys.sleep(3 * i)
  i
}
## [1] 4 3 2 1
```







——————————————————————————————————————








# 三、中级教程：利用doParallel并行+联用迭代器优化内存

## 1、利用doParallel并行——%dopar%

      foreach包创作是为了解决一些并行计算问题，将”%do%“更改为“%dopar%”前面例子就可以实现并行计算。在并行之前，需要`register`注册集群:




```
library(foreach)
library(doParallel)

cl<-makeCluster(no_cores)
registerDoParallel(cl)
```
      要记得最后要结束集群（不是用stopCluster()）：stopImplicitCluster()






## 2、参数when——按条件运算


`foreach(a=irnorm(1, count=10), .combine='c') %:% when(a >= 0) %do% sqrt(a)`
其中when是通过%:%来导出，而且%:%之后，还可以接%do%




```
qsort <- function(x) {
  n <- length(x)
  if (n == 0) {
    x
    } else {
      p <- sample(n, 1)
      smaller <- foreach(y=x[-p], .combine=c) %:% when(y <= x[p]) %do% y
      larger <- foreach(y=x[-p], .combine=c) %:% when(y > x[p]) %do% y
      c(qsort(smaller), x[p], qsort(larger))
      }
}

qsort(runif(12))
##  [1] 0.1481 0.2000 0.2769 0.4729 0.4747 0.5730 0.6394 0.6524 0.8315 0.8325
## [11] 0.8413 0.8724
```





## 3、联用iterators——优化、控制内存






iterators是为了给foreach提供循环变量，每次定义一个iterator，它都内定了“循环次数”和“每次循环返回的值”，因此非常适合结合foreach的使用。

iter(obj, ...)：可以接受iter, vector, matrix, data.frame, function。

nextElem(obj, ...)：接受iter对象，显示对象数值。




以matrix为例，




`iter(obj, by=c('column', 'cell', 'row'), chunksize=1L, checkFunc=function(...) TRUE, recycle=FALSE, ...)`
      参数解读：

      by：按照什么顺序循环；

      matrix和data.frame都默认是“row”，“cell”是按列依次输出（所以对于“cell”，chunksize只能指定为默认值，即1）

      chunksize：每次执行函数nextElem后，按照by的设定返回结果的长度。如果返回结构不够，将取剩余的全部。

      checkFunc=function(...) TRUE：执行函数checkFun，如果返回TRUE，则返回；否则，跳过。

      recycle：设定在nextElem循环到底（“错误: StopIteration”）是否要循环处理，即从头再来一遍。




### （1）iter+function迭代输出



      来看一个iter案例，帮你把函数分块给你，不用一次性导入计算，耗费内存：




```
> a
     [,1] [,2] [,3] [,4] [,5]
[1,]    1    5    9   13   17
[2,]    2    6   10   14   18
[3,]    3    7   11   15   19
[4,]    4    8   12   16   20
> i2 <- iter(a, by = "column", checkFunc=function(x) sum(x) > 50)
> nextElem(i2)
     [,1]
[1,]   13
[2,]   14
[3,]   15
[4,]   16
> nextElem(i2)
     [,1]
[1,]   17
[2,]   18
[3,]   19
[4,]   20
> nextElem(i2)
错误: StopIteration
```
不过，如果没有next了，就会出现报错，这时候就需要稍微注意一下。




iter(function()rnorm(1))，使用nextElem可以无限重复；但是iter(rnorm(1))，只能来一下。

更有意思的是对象如果是iter，即test1 <- iter(obj); test2 <- iter(test1)，那么这两个对象是连在一起的，同时变化。




###       （2）生成随机数






      irnorm(..., count)；irunif(..., count)；irbinom(..., count)；irnbinom(..., count)；irpois(..., count)是内部生成iterator的工具，分别表示从normal，uniform，binomial，negativity binomial和Poisson分布中随机选取N个元素，进行count次。




      其中，negative binomial分布：其概率积累函数(probability mass function)为掷骰子，每次骰子为3点的概率为p，在第r+k次恰好出现r次的概率。




      icount(count)可以生成1:conunt的iterator；如果count不指定，将从无休止生成1:Inf

      icountn(vn)比较好玩，vn是指一个数值向量（如果是小数，则向后一个数取整，比如2.3 --> 3）。循环次数为prod(vn)，每次返回的向量中每个元素都从1开始，不超过设定 vn，变化速率从左向右依次递增。




      idiv(n, ..., chunks, chunkSize)返回截取从1:n的片段长度，“chunks”和“chunkSize”不能同时指定，“chunks”为分多少片段（长度从大到小），“chunkSize”为分段的最大长度（长度由大到小）

      iapply(X, MARGIN)：与apply很像，MARGIN中1是row，2是column

      isplit(x, f, drop=FALSE, ...)：按照指定的f划分矩阵




```
> i2 <- icountn(c(3.4, 1.2))
> nextElem(i2)
[1] 1 1
> nextElem(i2)
[1] 2 1
> nextElem(i2)
[1] 3 1
> nextElem(i2)
[1] 4 1
> nextElem(i2)
[1] 1 2
> nextElem(i2)
[1] 2 2
> nextElem(i2)
[1] 3 2
> nextElem(i2)
[1] 4 2
> nextElem(i2)
错误: StopIteration
```





——————————————————————————————————————




# 四、高级教程：并行机器学习算法




并行计算一些小任务会比按顺序运算它们花费更多的时间，所以当普通运算足够快的时候，并没有必要使用并行计算模式改进其运算效率。


同时，最适合并行莫过于随机森林算法了。



```
#生成矩阵x作为输入值，y作为目标因子
x <- matrix(runif(500), 100)
y <- gl(2, 50)
#导入randomForest包
require(randomForest)
```






## 1、独立循环运行随机森林算法



如果我们要创建一个包含1200棵树的随机森林模型，在6核CPU电脑上，我们可以将其分割为六块执行randomForest函数六次，同时将ntree参赛设为200,最后再将结果合并。



```
rf <- foreach(ntree=rep(200, 6), .combine=combine) %do%
  randomForest(x, y, ntree=ntree)
rf
## 
## Call:
##  randomForest(x = x, y = y, ntree = ntree) 
##                Type of random forest: classification
##                      Number of trees: 1200
## No. of variables tried at each split: 2
```



分开来运行6个200树的随机森林算法。


## 2、参数.packages——并行运行随机森林算法




将%do%改为“%dopar%”,同时使用.packages调用randomForest：




```
rf <- foreach(ntree=rep(200,6), .combine=combine, .packages="randomForest") %dopar% 
  randomForest(x, y, ntree=ntree)
rf
```
通过.packages来将函数包导入其中，类似parallel中的clusterEvalQ，但是foreach在一个函数里面包含了函数、包的导入过程。


当然还可以使用一些其他包，使用.packages参数来加载包,比如说：.packages = c("rms", "mice")





## 3、参数.export——将doParallel并行写入函数




写入函数有个问题就是，运行函数的时候，运用不了R外面内存环境的变量。而且会报错：


```
test <- function (exponent) {
  foreach(exponent = 2:4, 
          .combine = c)  %dopar%  
    base^exponent
}
test()

 Error in base^exponent : task 1 failed - "object 'base' not found"
```




      所以需要在写函数的时候，将一些外面的内存函数，写到函数之中，通过.export，而不需要使用clusterExport。注意的是，他可以加载最终版本的变量，在函数运行前，变量都是可以改变的：



```
base <- 2
cl<-makeCluster(2)
registerDoParallel(cl)
 
base <- 4
test <- function (exponent) {
  foreach(exponent = 2:4, 
          .combine = c,
          .export = "base")  %dopar%  
    base^exponent
}
test()
 
stopCluster(cl)

 [1]  4  8 16
```





——————————————————————————————————————


# 应用一：并行时.dopar，如何输入多个常规值，.export输入多个值？




**本节想解决的问题：**

1、能不能输出每次迭代的时间？（参考：[R语言︱函数使用技巧（循环、if族/for、switch、repeat、ifelse、stopifnot）](http://blog.csdn.net/sinat_26917383/article/details/51191916)）

2、并行的时候，如何导入多个数值型变量？




**注意：**

.export需要输入方程中没有的值，而且必须是一个文本型，可以用list的方式。






```
library(foreach)
registerDoParallel(makeCluster(2))  

a=c(1,2,3)
b=c(3,4,5,5,5,5)
d=c(2,4,35,5,5,2)
i=0
t1 = Sys.time()

foreach(x=a, .combine="rbind",.export="list(b,d,i,t1)") %dopar% {
  cc=b+d
  a+cc
  i=i+1
  print(paste("迭代了",i,"次",sep=""))
  print(difftime(Sys.time(), t1, units = 'sec'))  
}
```

注意代码中，设置了2个核心。
.combine可以将数据合并起来，之后步骤在所有结果出来之后再进行相应的合并。所以结果出来，会有result.1等项目：



```
[1] "迭代了1次"
Time difference of 0.03000283 secs
[1] "迭代了2次"
Time difference of 0.031003 secs
[1] "迭代了3次"
Time difference of 0.031003 secs
               [,1]
result.1 0.03000283
result.2 0.03100300
result.3 0.03100300
```









——————————————————————————————————————




# 参考文献：




1、[R语言中的并行计算：foreach,iterators, doParallel包](http://blog.csdn.net/wa2003/article/details/48145147)


2、f[oreach包简介](http://blog.csdn.net/sadfasdgaaaasdfa/article/details/45155349)     /    FROM:《Using The foreach Package》

3、[R语言︱大数据集下运行内存管理](http://blog.csdn.net/sinat_26917383/article/details/51114265)



4、[R︱并行计算以及提高运算效率的方式(parallel包、clusterExport函数、SupR包简介)](http://blog.csdn.net/sinat_26917383/article/details/52719232)](https://so.csdn.net/so/search/s.do?q=并行&t=blog)](https://so.csdn.net/so/search/s.do?q=doParallel&t=blog)](https://so.csdn.net/so/search/s.do?q=R&t=blog)](https://so.csdn.net/so/search/s.do?q=foreach&t=blog)




