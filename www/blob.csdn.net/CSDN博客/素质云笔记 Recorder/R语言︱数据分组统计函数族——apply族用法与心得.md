
# R语言︱数据分组统计函数族——apply族用法与心得 - 素质云笔记-Recorder... - CSDN博客

2016年04月07日 15:55:40[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：18251所属专栏：[R的数据操作与清洗](https://blog.csdn.net/column/details/13587.html)




**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————

笔者寄语：apply族功能强大，实用，可以代替很多循环语句，R语言中不要轻易使用循环语句。

|函数名|功能|特点|
|apply|按行、列运算均值、求和、众数等|简单运算|
|tapply=table apply|在apply之上加入table功能，可以分组汇总|table结合，可以分组汇总|
|lapply=list apply|都需要数据框格式，可以与list合用，返回仍是list|list用法|
|sapply=simplify apply|=unlist(lapply)|都需要数据框格式，可以与list合用，返回是矩阵|与lapply一样，但是可以输出矩阵格式|


|apply
|Apply Functions Over Array Margins
|对阵列行或者列使用函数
|apply(X, MARGIN, FUN, ...)
|
|lapply
|Apply a Function over a List or Vector
|对列表或者向量使用函数
|lapply(X, FUN, ...)
|
|sapply
|Apply a Function over a List or Vector
|对列表或者向量使用函数
|sapply(X, FUN, ..., simplify = TRUE, USE.NAMES = TRUE)
|
|vapply
|Apply a Function over a List or Vector
|对列表或者向量使用函数
|vapply(X, FUN, FUN.VALUE, ..., USE.NAMES = TRUE)
|
|tapply
|Apply a Function Over a Ragged Array
|对不规则阵列使用函数
|tapply(X, INDEX, FUN = NULL, ..., simplify = TRUE)
|
|eapply
|Apply a Function Over Values in an Environment
|对环境中的值使用函数
|eapply(env, FUN, ..., all.names = FALSE, USE.NAMES = TRUE)
|
|mapply
|Apply a Function to Multiple List or Vector Arguments
|对多个列表或者向量参数使用函数
|mapply(FUN, ..., MoreArgs = NULL, SIMPLIFY = TRUE, USE.NAMES = TRUE)
|
|rapply
|Recursively Apply a Function to a List
|运用函数递归产生列表
|rapply(object, f, classes = "ANY", deflt = NULL,how = c("unlist", "replace", "list"), ...)
|


# 1、apply函数
对一个数组按行或者按列进行计算，矩阵纵、横运算（sum,average等）
其中apply中，1等于行，2等于列
`> ma <- matrix(c(1:4, 1, 6:8), nrow = 2)
> ma
     [,1] [,2] [,3] [,4]
[1,]    1    3    1    7
[2,]    2    4    6    8
> apply(ma, c(1,2), sum)
     [,1] [,2] [,3] [,4]
[1,]    1    3    1    7
[2,]    2    4    6    8
> apply(ma, 1, sum)
[1] 12 20
> apply(ma, 2, sum)
[1]  3  7  7 15`
`> tapply(1:17, fac, sum, simplify = FALSE)
$`1`
[1] 51
$`2`
[1] 57
$`3`
[1] 45
$`4`
NULL
$`5`
NULL
> tapply(1:17, fac, range)
$`1`
[1]  1 16
$`2`
[1]  2 17
$`3`
[1]  3 15
$`4`
NULL
$`5`
NULL`

# 2、tapply
（进行分组统计）
`tapply(X, INDEX, FUN = NULL, ..., simplify = TRUE)
#把x在index分类下进行fun
#例：把x在因子分类下，进行汇总操作
fac <- factor(rep(1:3, length = 4), levels = 1:5)
fac
[1] 1 2 3 1
Levels: 1 2 3 4 5
tapply(1:4, fac, sum)
1  2  3  4  5 
5  2  3   NA NA`

\#当index不是因子时，可以用as.factor()把参数强制转换成因子
额外案例，实现excel中数据透视表的功能
`#利用tapply实现类似于excel里的数据透视表的功能：
> da
   year province sale
1  2007        A    1
2  2007        B    2
3  2007        C    3
4  2007        D    4
5  2008        A    5
6  2008        C    6
7  2008        D    7
8  2009        B    8
9  2009        C    9
10 2009        D   10
> attach(da)
> tapply(sale,list(year,province))   #以sale为基，按照year,province的顺序，排列
 [1]  1  4  7 10  2  8 11  6  9 12
> tapply(sale,list(year,province),mean)
      A  B C  D
2007  1  2 3  4
2008  5 NA 6  7
2009 NA  8 9 10`

# 3、函数table（求因子出现的频数）
`使用格式为：
table(..., exclude = if (useNA == "no") c(NA, NaN), useNA = c("no",
    "ifany", "always"), dnn = list.names(...), deparse.level = 1)
其中参数exclude表示哪些因子不计算。
示例代码：
> d <- factor(rep(c("A","B","C"), 10), levels=c("A","B","C","D","E"))
> d
 [1] A B C A B C A B C A B C A B C A B C A B C A B C A B C A B C
Levels: A B C D E
> table(d, exclude="B")
d
 A  C  D  E
10 10  0  0`


# 4、函数lapply与函数sapply
每一列数据采用同一种函数形式，比如求X变量得分位数，比如求X变量的循环函数。
lapply的使用格式为：
lapply(X, FUN, ...)
lapply的返回值是和一个和X有相同的长度的list对象，
这个list对象中的每个元素是将函数FUN应用到X的每一个元素。
其中X为List对象（该list的每个元素都是一个向量），
其他类型的对象会被R通过函数as.list()自动转换为list类型。


函数sapply是函数lapply的一个特殊情形，对一些参数的值进行了一些限定，其使用格式为：
sapply(X, FUN,..., simplify = TRUE, USE.NAMES = TRUE)
sapply(*, simplify = FALSE, USE.NAMES = FALSE) 和lapply(*)的返回值是相同的。
如果参数simplify=TRUE，则函数sapply的返回值不是一个list，而是一个矩阵；
若simplify=FALSE，则函数sapply的返回值仍然是一个list。
`x <- list(a = 1:10, beta = exp(-3:3), logic = c(TRUE,FALSE,FALSE,TRUE))
> lapply(x, quantile)
$a
   0%   25%   50%   75%  100%
 1.00  3.25  5.50  7.75 10.00
$beta
         0%         25%         50%         75%        100%
 0.04978707  0.25160736  1.00000000  5.05366896 20.08553692
$logic
  0%  25%  50%  75% 100%
 0.0  0.0  0.5  1.0  1.0
> sapply(x, quantile,simplify=FALSE,use.names=FALSE)
$a
   0%   25%   50%   75%  100%
 1.00  3.25  5.50  7.75 10.00
$beta
         0%         25%         50%         75%        100%
 0.04978707  0.25160736  1.00000000  5.05366896 20.08553692
$logic
  0%  25%  50%  75% 100%
 0.0  0.0  0.5  1.0  1.0``#参数simplify=TRUE的情况
> sapply(x, quantile)
         a        beta logic
0%    1.00  0.04978707   0.0
25%   3.25  0.25160736   0.0
50%   5.50  1.00000000   0.5
75%   7.75  5.05366896   1.0
100% 10.00 20.08553692   1.0`
# 5、函数mapply
函数mapply是函数sapply的变形版，mapply 将函数 FUN 依次应用每一个参数的第一个元素、第二个元素、第三个元素上。函数mapply的使用格式如下：
mapply(FUN, ..., MoreArgs = NULL, SIMPLIFY = TRUE,USE.NAMES = TRUE)

其中参数MoreArgs表示函数FUN的参数列表。


`> mapply(rep, times=1:4, x=4:1)
[[1]]
[1] 4
[[2]]
[1] 3 3
[[3]]
[1] 2 2 2
[[4]]
[1] 1 1 1 1
#直接使用函数rep的结果：
> rep(1:4,1:4)
 [1] 1 2 2 3 3 3 4 4 4 4`

# 6、vapply {base}——按变量进行函数操作

vapply类似于sapply函数，但是它的返回值有预定义类型，所以它使用起来会更加安全，有的时候会更快。
在vapply函数中总是会进行简化，vapply会检测FUN的所有值是否与FUN.VALUE兼容，
以使他们具有相同的长度和类型。类型顺序：逻辑、整型、实数、复数

vapply(X, FUN, FUN.VALUE, ..., USE.NAMES = TRUE)

X表示一个向量或者表达式对象，其余对象将被通过as.list强制转换为list
simplify 逻辑值或者字符串，如果可以，结果应该被简化为向量、矩阵或者高维数组。
必须是命名的，不能是简写。默认值是TRUE，若合适将会返回一个向量或者矩阵。如果simplify=”array”，结果将返回一个阵列。
USE.NAMES  逻辑值，如果为TRUE，且x没有被命名，则对x进行命名。
FUN.VALUE   一个通用型向量，FUN函数返回值得模板。

`> x<-data.frame(a=rnorm(4,4,4),b=rnorm(4,5,3),c=rnorm(4,5,3))
> vapply(x,mean,c(c=0))
 a       b       c
 1.8329043  6.0442858 -0.1437202``> k<-function(x)
+ {
+ list(mean(x),sd(x))
+ }
> vapply(x,k,c(c=0))
错误于vapply(x, k, c(c = 0)) : 值的长度必需为1，
 但FUN(X[[1]])结果的长度却是2``> vapply(x,k,c(c=0,b=0))
错误于vapply(x, k, c(c = 0, b = 0)) : 值的种类必需是'double'，
 但FUN(X[[1]])结果的种类却是'list'``> vapply(x,k,c(list(c=0,b=0)))
 a     b     c        
c 1.832904 6.044286 -0.1437202
b 1.257834 1.940433 3.649194`
sapply与vapply函数之间的区别:

`> i39 <- sapply(3:9, seq)
> i39
[[1]]
[1] 1 2 3
[[2]]
[1] 1 2 3 4
[[3]]
[1] 1 2 3 4 5
[[4]]
[1] 1 2 3 4 5 6
[[5]]
[1] 1 2 3 4 5 6 7
[[6]]
[1] 1 2 3 4 5 6 7 8
[[7]]
[1] 1 2 3 4 5 6 7 8 9
> sapply(i39, fivenum)
     [,1] [,2] [,3] [,4] [,5] [,6] [,7]
[1,]  1.0  1.0    1  1.0  1.0  1.0    1
[2,]  1.5  1.5    2  2.0  2.5  2.5    3
[3,]  2.0  2.5    3  3.5  4.0  4.5    5
[4,]  2.5  3.5    4  5.0  5.5  6.5    7
[5,]  3.0  4.0    5  6.0  7.0  8.0    9
> vapply(i39, fivenum,
+        c(Min. = 0, "1st Qu." = 0, Median = 0, "3rd Qu." = 0, Max. = 0))
        [,1] [,2] [,3] [,4] [,5] [,6] [,7]
Min.     1.0  1.0    1  1.0  1.0  1.0    1
1st Qu.  1.5  1.5    2  2.0  2.5  2.5    3
Median   2.0  2.5    3  3.5  4.0  4.5    5
3rd Qu.  2.5  3.5    4  5.0  5.5  6.5    7
Max.     3.0  4.0    5  6.0  7.0  8.0    9`




# 7、eapply {base}

eapply函数通过对environment中命名值进行FUN计算后返回一个列表值，用户可以请求所有使用过的命名对象。
eapply(env, FUN, ..., all.names = FALSE, USE.NAMES = TRUE)
env  将被使用的环境
all.names  逻辑值，指示是否对所有值使用该函数
USE.NAMES  逻辑值，指示返回的列表结果是否包含命名

`> require(stats)
>
> env <- new.env(hash = FALSE) # so the order is fixed
> env$a <- 1:10
> env$beta <- exp(-3:3)
> env$logic <- c(TRUE, FALSE, FALSE, TRUE)
> # what have we there?
> utils::ls.str(env)
a :  int [1:10] 1 2 3 4 5 6 7 8 9 10
beta :  num [1:7] 0.0498 0.1353 0.3679 1 2.7183 ...
logic :  logi [1:4] TRUE FALSE FALSE TRUE
>
> # compute the mean for each list element
>        eapply(env, mean)
$logic
[1] 0.5
 
$beta
[1] 4.535125
 
$a
[1] 5.5
 
> unlist(eapply(env, mean, USE.NAMES = FALSE))
[1] 0.500000 4.535125 5.500000
>
> # median and quartiles for each element (making use of "..." passing):
> eapply(env, quantile, probs = 1:3/4)
$logic
25% 50% 75%
0.0 0.5 1.0
 
$beta
      25%       50%       75%
0.2516074 1.0000000 5.0536690
 
$a
 25%  50%  75%
3.25 5.50 7.75
 
> eapply(env, quantile)
$logic
  0%  25%  50%  75% 100%
 0.0  0.0  0.5  1.0  1.0
 
$beta
         0%         25%         50%         75%        100%
 0.04978707  0.25160736  1.00000000  5.05366896 20.08553692
 
$a
   0%   25%   50%   75%  100%
 1.00  3.25  5.50  7.75 10.00`

# 8、rapply {base}
rapply是lapply的递归版本
rapply(X, FUN, classes = "ANY", deflt = NULL, how = c("unlist", "replace", "list"), ...)
X  一个列表
classes  关于类名的字符向量，或者为any时则匹配任何类
deflt  默认结果，如果使用了how=”replace”，则不能使用
how  字符串匹配三种可能结果




# 参考文献：
# [菜鸟的成长的博客](http://blog.sina.com.cn/u/1823385791)
# ：http://blog.sina.com.cn/s/blog_6caea8bf0100xkpg.html



————————————————————————————————————————————————————————————
# 拓展一：lapply的用法
因为一直想不明白，所以最开始的时候都不太会使用这个函数来进行并行处理， 最近想明白，用了一下发现验证了我的想法。
lapply中所要使用的函数，一定需要是输入为单一变量，输出为单一变量可以存至list中。
同时，lapply（x,fun），这个x的格式很重要，如果灌入的是list，在使用的时候，其实是先把x[[1]]，之后然后放入fun之中。也就是说，先拨开的list，然后再灌入。

例如：
`a=function(x)[
x=names(x)
x[x=="a"]
}
lapply(y,a)`
从这段代码大致可以了解到，lapply精髓在输入与输出。
输入应该单列，或者List格式的，整个多字节的数据框是不对的；
输出也只能是某一种形式，可以保存到list中的，比如此时的字符形式。
**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————


