# R语言︱集合运算——小而美法则 - 素质云笔记/Recorder... - CSDN博客





2016年04月28日 21:08:09[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：8172
所属专栏：[R的数据操作与清洗](https://blog.csdn.net/column/details/13587.html)













**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)


———————————————————————————


集合运算的一般规则如下：
    union(x,y)    #求并集
    intersect(x,y)    #求交集
    setdiff(x,y)    #求属于x而不属于y的所有元素
    setequal(x,y)    #判断x与y是否相等
    a %in% y    #判断a是否为y中的元素
    choose(n, k)    #n个里面取k个的组合数
    combn(x,n)    #x中的元素每次取n个的所有组合
    combn(x,n,f)     #将这些组合用于指定函数f







```
> x=c(1,4,5)
> y=c(2,4,8)
> union(x,y)    #求并集
[1] 1 4 5 2 8
> intersect(x,y)    #求交集
[1] 4
> setdiff(x,y)    #求属于x而不属于y的所有元素
[1] 1 5
> setequal(x,y)    #判断x与y是否相等
[1] FALSE
> x %in% y    #判断a是否为y中的元素
[1] FALSE  TRUE FALSE
```

union(x,y)可以生成XY的并集，有一定的去重功能；


intersect代表交集，可以找出共有的，在文本挖掘中，词和词之间的对应关系；

setdiff代表去掉x中xy共有的地方；

x %in%y，这个管道函数很有意思，x中xy共有的为TURE，生成了一个逻辑向量，从而可以进行一些文档匹配的操作。




类似的用法sqldf包中的union 也可以实现： 

# [R语言︱ 数据库SQL-R连接与SQL语句执行（RODBC、sqldf包）](http://blog.csdn.net/sinat_26917383/article/details/51601539)






——————————————————————————————————————




**1、情感分析中setdiff和x %in%y的用处**

    功能：停用词的清理&词库之间相互匹配。stopword是停用词库，testterm是原序列。



```
stopword <- read.csv("F:/R语言/R语言与文本挖掘/情感分析/数据/dict/stopword.csv", header = T, sep = ",", stringsAsFactors = F)
#结果是一个和stopword等长的波尔值向量，“非”函数将布尔值反向
testterm <- testterm[!testterm$term %in% stopword,]#去除停用词
```


```
stopword <- read.csv("F:/R语言/R语言与文本挖掘/情感分析/数据/dict/stopword.csv", header = T, sep = ",", stringsAsFactors = F)
stopword <- setdiff(stopword$term,posneg$term)
testterm<- setdiff(testterm$term,stopword)
```
上面两端代码能实现同样的效果。
详情可见： 
# [R语言︱情感分析文本操作技巧汇总（打标签、词典与数据匹配等）](http://blog.csdn.net/sinat_26917383/article/details/51265179)






——————————————————————————————————————





2、集合运算应用在数据匹配之上




      集合运算可以较好地应用在数据之间的匹配。而匹配中，%in%的效率最高。是一个很好的匹配的媒介。一些情况要由于merge




——————————————————————————————




3、相等运算





`identical(rownames(dtm_train), train$id)`
返回的是逻辑值，如果相等则TRUE。









**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)


———————————————————————————




