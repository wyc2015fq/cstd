# R语言︱大数据集下运行内存管理 - 素质云笔记/Recorder... - CSDN博客





2016年04月10日 18:48:51[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：6642
个人分类：[大数据、并行计算&R](https://blog.csdn.net/sinat_26917383/article/category/6093549)















**在实操时出现以下的问题：**


`Error: cannot allocate vector of size 2.9GB`




大神指导（http://bbs.pinggu.org/thread-3682816-1-1.html）

cannot allocate vector就是典型的数据太大读不了
方法有三
一、升级硬件
二、改进算法
三、修改操作系统分配给R的内存上限, memory.size(T)查看已分配内存 

memory.size(F)查看已使用内存  

memory.limit()查看内存上限 

object.size()看每个变量占多大内存。
memory.size()查看现在的work space的内存使用
memory.limit()查看系统规定的内存使用上限。如果现在的内存上限不够用，可以通过memory.limit(newLimit)更改到一个新的上限。注意，在32位的R中，封顶上限为4G，无法在一个程序上使用超过4G （数位上限）。这种时候，可以考虑使用64位的版本。


——————————————————————————————

下面来看一个处理大数据，内存的管理办法。




**《R语言处理大数据》**





参考：http://blog.sina.com.cn/s/blog_61f013b80100xxir.html




R最大的缺点就是不能进行并行计算和内存限制。







# 一、内存限制的突破



[“参考网址1”](http://jliblog.com/archives/276)中提到如果只是对整数运算（运算过程和结果都只使用整数），没有必要使用“double”(8 byte)，而应该用更小的“integer”(4 byte)。使用storage.mode(x)查看对象存数的模式，storage.mode(x)
 <- 进行赋值；使用format(object.size(a), units = 'auto')查看对象占用的内存空间（此处有疑问，即在R中每个integer到底占用了多大的空间？）。

需要解释gc()函数，可以查看内存使用情况。同样，在清除了大的对象之后，使用gc()以释放内存使用空间。




李航在[”参考网址2“](http://cos.name/wp-content/uploads/2011/05/01-Li-Jian-HPC.pdf)中提到，对于大矩阵的操作，尽量避免使用cbind和rbind之类，因为这会让内存不停地分配空间。“对于长度增加的矩阵，尽量先定义一个大矩阵，然后逐步增加”和“注意清除中间对象”。




使用bigmemory家族：bigmemory, biganalytics, synchronicity, bigtabulate and bigalgebra， 同时还有biglm。







bigmemory package的使用：




## 1. 建立big.memory对象




bigmemory采用C++的数据格式来“模仿”R中的matrix。




编写大数据格式文件时候，可以先建立filebacked.big.matrix




big.matrix(nrow, ncol, type = options()$bigmemory.default.type, init = NULL, dimnames = NULL, separated = FALSE, backingfile = NULL, backingpath
 = NULL, descriptorfile = NULL, shared = TRUE)




filebacked.big.matrix(nrow, ncol, type = options()$bigmemory.default.type, init = NULL, dimnames = NULL, separated = FALSE, backingfile = NULL,
 backingpath = NULL, descriptorfile = NULL)




as.big.matrix(x, type = NULL, separated = FALSE, backingfile = NULL, backingpath = NULL, descriptorfile = NULL, shared=TRUE)




使用注意：




big.matrix采用两种方式储存数据：一种是big.matrix默认的方式，如果内存空间比较大，可以尝试使用；另外一种是filebacked.big.matrix，这种储存方法可能会备份文件（file-backings），而且需要descriptor file；

“init”指矩阵的初始化数值，如果设定，会事先将设定的数值填充到矩阵中；如果不设置，将处理为NA

"type"是指在big.matrix中atomic element的储存格式，默认是“double”(8 byte)，可以改为“integer”(4 byte), "short"(2 byte) or "char"(1 byte)。注意：这个包不支持字符串的储存，type
 = "char"是指ASCII码字母。




在big.matrix非常大的时候，避免使用rownames和colnames(并且bigmemory禁止用名称访问元素)，因为这种做法非常占用内存。如果一定要改变，使用options(bigmemory.allow.dimnames=TRUE)，之后colnames, rownames设置。




直接在命令提示符后输入x（x是一个big matrix），将返回x的描述，不会出现所有x中所有内容。因此，注意x[ , ](打印出矩阵全部内容)；

如果big.matrix有很多列，那么应该将其转置后储存；（不推荐）或者将参数“separated”设置为TRUE，这样就将每一列分开储存。否则，将用R的传统方式（column major的方式）储存数据。




如果建立一个filebacked.big.matrix，那么需要指定backingfile的名称和路径+descriptorfile。可能多个big.matrix对象对应唯一一个descriptorfile，即如果descriptorfile改变，所以对应的big.matrix随之改变；同样，decriptorfile随着big.matrix的改变而改变；如果想维持一种改变，需要重新建立一个filebacked.big.matrix。attach.big.matrix(descriptorfile
 or describe(big.matrix))函数用于将一个descriptorfile赋值给一个big.matrix。这个函数很好用，因为每次在创建一个filebacked.big.matrix后，保存R并退出后，先前创建的矩阵会消失，需要再attach.big.matrix以下







## 2. 对big.matrix的列的特定元素进行条件筛选




对内存没有限制；而且比传统的which更加灵活（赞！）




mwhich(x, cols, vals, comps, op = 'AND')




x既可以是big.matrix，也可以是传统的R对象；

cols：行数

vals：cutoff，可以设定两个比如c(1, 2)

comps：'eq'(==), 'neq'(!=), 'le'(<), 'lt'(<=), 'ge'(>) and 'gt'(>=)

op：“AND”或者是“OR”

可以直接比较NA，Inf和-Inf





## 3.bigmemory中其他函数




nrow, ncol, dim, dimnames, tail, head, typeof继承base包




big.matrix, is.big.matrix, as.big.matrix, attach.big.matrix, describe, read.big.matrix, write.big.matrix, sub.big.matrix, is.sub.big.matrix为特有的big.matrix文件操作；filebacked.big.matrix, is.filebacked（判断big.matrix是否硬盘备份）
 , flush(将filebacked的文件刷新到硬盘备份上)是filebacked的big.matrix的操作。




mwhich增强base包中的which， morder增强order，mpermute（对matrix中的一列按照特定序列操作，但是会改变原来对象，这是为了避免内存溢出）





big.matrix对象的copy使用deepcopy(x, cols = NULL, rows = NULL, y = NULL, type = NULL, separated = NULL, backingfile = NULL, backingpath = NULL, descriptorfile
 = NULL, shared=TRUE)




biganalytics package的使用




biganalytics主要是一些base基本函数的扩展，主要有max, min, prod, sum, range, colmin, colmax, colsum, colprod, colmean, colsd, colvar, summary, apply（只能用于行或者列，不能用行列同时用）等




比较有特色的是bigkmeans的聚类

剩下的biglm.big.matrix和bigglm.big.matrix可以参考Lumley's biglm package。

bigtabulate package的使用










# 二、并行计算限制的突破：




使用doMC家族：doMC, doSNOW, doMPI, doRedis, doSMP和foreach packages.




## foreach package的使用




foreach(..., .combine, .init, .final=NULL, .inorder=TRUE, .multicombine=FALSE, .maxcombine=if (.multicombine) 100 else 2, .errorhandling=c('stop',
 'remove', 'pass'), .packages=NULL, .export=NULL, .noexport=NULL, .verbose=FALSE)

foreach的特点是可以进行并行运算，如在NetWorkSpace和snow？

%do%严格按照顺序执行任务（所以，也就非并行计算），%dopar%并行执行任务

...：指定循环的次数；

.combine：运算之后结果的显示方式，default是list，“c”返回vector， cbind和rbind返回矩阵，"+"和"*"可以返回rbind之后的“+”或者“*”

.init：.combine函数的第一个变量

.final：返回最后结果

.inorder：TRUE则返回和原始输入相同顺序的结果（对结果的顺序要求严格的时候），FALSE返回没有顺序的结果（可以提高运算效率）。这个参数适合于设定对结果顺序没有需求的情况。

.muticombine：设定.combine函数的传递参数，default是FALSE表示其参数是2，TRUE可以设定多个参数

.maxcombine：设定.combine的最大参数

.errorhandling：如果循环中出现错误，对错误的处理方法

.packages：指定在%dopar%运算过程中依赖的package（%do%会忽略这个选项）。




getDoParWorkers( ) ：查看注册了多少个核，配合doMC package中的registerDoMC( )使用

getDoParRegistered( ) ：查看doPar是否注册；如果没有注册返回FALSE

getDoParName( ) ：查看已经注册的doPar的名字

getDoParVersion( )：查看已经注册的doPar的version




===================================================





```
# foreach的循环次数可以指定多个变量，但是只用其中最少？的
> foreach(a = 1:10, b = rep(10, 3)) %do% (a*b)
[[1]]
[1] 10

[[2]]
[1] 20

[[3]]
[1] 30

# foreach中.combine的“+”或者“*”是cbind之后的操作；这也就是说"expression"返回一个向量，会对向量+或者*
> foreach(i = 1:4, .combine = "+") %do% 2
[1] 8
> foreach(i = 1:4, .combine = "rbind") %do% rep(2, 5)
         [,1] [,2] [,3] [,4] [,5]
result.1    2    2    2    2    2
result.2    2    2    2    2    2
result.3    2    2    2    2    2
result.4    2    2    2    2    2
> foreach(i = 1:4, .combine = "+") %do% rep(2, 5)
[1] 8 8 8 8 8
> foreach(i = 1:4, .combine = "*") %do% rep(2, 5)
[1] 16 16 16 16 16
```







=============================================




## iterators package的使用




iterators是为了给foreach提供循环变量，每次定义一个iterator，它都内定了“循环次数”和“每次循环返回的值”，因此非常适合结合foreach的使用。




iter(obj, ...)：可以接受iter, vector, matrix, data.frame, function。

nextElem(obj, ...)：接受iter对象，显示对象数值。




以matrix为例，




iter(obj, by=c('column', 'cell', 'row'), chunksize=1L, checkFunc=function(...) TRUE, recycle=FALSE, ...)

by：按照什么顺序循环；matrix和data.frame都默认是“row”，“cell”是按列依次输出（所以对于“cell”，chunksize只能指定为默认值，即1）




chunksize：每次执行函数nextElem后，按照by的设定返回结果的长度。如果返回结构不够，将取剩余的全部。

checkFunc=function(...) TRUE：执行函数checkFun，如果返回TRUE，则返回；否则，跳过。

recycle：设定在nextElem循环到底（“错误: StopIteration”）是否要循环处理，即从头再来一遍。




以function为例




iter(function()rnorm(1))，使用nextElem可以无限重复；但是iter(rnorm(1))，只能来一下。

更有意思的是对象如果是iter，即test1 <- iter(obj); test2 <- iter(test1)，那么这两个对象是连在一起的，同时变化。





==============================================









```
> a
     [,1] [,2] [,3] [,4] [,5]
[1,]    1    5    9   13   17
[2,]    2    6   10   14   18
[3,]    3    7   11   15   19
[4,]    4    8   12   16   20
> i2 <- iter(a, by = "row", chunksize=3)
> nextElem(i2)
     [,1] [,2] [,3] [,4] [,5]
[1,]    1    5    9   13   17
[2,]    2    6   10   14   18
[3,]    3    7   11   15   19
> nextElem(i2)  #第二次iterate之后，只剩下1行，全部返回
     [,1] [,2] [,3] [,4] [,5]
[1,]    4    8   12   16   20
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
> colSums(a)
[1] 10 26 42 58 74
> testFun <- function(x){return(x+2)}
> i2 <- iter(function()testFun(1))
> nextElem(i2)
[1] 3
> nextElem(i2)
[1] 3
> nextElem(i2)
[1] 3
> i2 <- iter(testFun(1))
> nextElem(i2)
[1] 3
> nextElem(i2)
错误: StopIteration
> i2 <- iter(testFun(1))
> i3 <- iter(i2)
> nextElem(i3)
[1] 3
> nextElem(i2)
错误: StopIteration
```










============================================



## iterators package中包括




irnorm(..., count)；irunif(..., count)；irbinom(...,
 count)；irnbinom(..., count)；irpois(..., count)中内部生成iterator的工具，分别表示从normal，uniform，binomial，negativity binomial和Poisson分布中随机选取N个元素，进行count次。其中，negative
 binomial分布：其概率积累函数(probability mass function)为掷骰子，每次骰子为3点的概率为p，在第r+k次恰好出现r次的概率。





icount(count)可以生成1:conunt的iterator；如果count不指定，将从无休止生成1:Inf

icountn(vn)比较好玩，vn是指一个数值向量（如果是小数，则向后一个数取整，比如2.3 --> 3）。循环次数为prod(vn)，每次返回的向量中每个元素都从1开始，不超过设定
 vn，变化速率从左向右依次递增。




idiv(n, ..., chunks, chunkSize)返回截取从1:n的片段长度，“chunks”和“chunkSize”不能同时指定，“chunks”为分多少片段（长度从大到小），“chunkSize”为分段的最大长度（长度由大到小）

iapply(X, MARGIN)：与apply很像，MARGIN中1是row，2是column

isplit(x, f, drop=FALSE, ...)：按照指定的f划分矩阵




=============================================


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




=============================================








参考文献：

1 [http://jliblog.com/archives/276](http://jliblog.com/archives/276)

2 [http://cos.name/wp-content/uploads/2011/05/01-Li-Jian-HPC.pdf](http://cos.name/wp-content/uploads/2011/05/01-Li-Jian-HPC.pdf)

3 R 高性能计算和并行计算 [http://cran.r-project.org/web/views/HighPerformanceComputing.html](http://cran.r-project.org/web/views/HighPerformanceComputing.html)




更新记录：2014年6月25日

























