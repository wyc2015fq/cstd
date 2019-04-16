# data.table简介 - littlely_ll的博客 - CSDN博客





2017年04月11日 14:01:29[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：244标签：[R																[data-table																[数据处理](https://so.csdn.net/so/search/s.do?q=数据处理&t=blog)
个人分类：[R语言基础																[R数据预处理](https://blog.csdn.net/littlely_ll/article/category/6700496)](https://blog.csdn.net/littlely_ll/article/category/6569419)





`data.table`是一个十分有效的数据处理包，它是`data.frame`的一个扩展，能够快速的对数据进行分片，分组，聚合等操作，比`data.frame`快很多。在数据处理方面，据实验结果表明，对于2G的数据，`tapply`耗时16秒，而`data.table`耗时1.6秒，快了10倍，官方还介绍说，对于100G的数据，它还能运行很好。 

下面介绍几个主要用到的函数：

# `fread`

和`read.table`相似，但是更快更方便

```
fread(input, sep="auto", sep2="auto", nrows=-1L, header="auto", na.strings="NA", file,
stringsAsFactors=FALSE, verbose=getOption("datatable.verbose"), autostart=1L,
skip=0L, select=NULL, drop=NULL, colClasses=NULL,
integer64=getOption("datatable.integer64"), # default: "integer64"
dec=if (sep!=".") "." else ",", col.names,
check.names=FALSE, encoding="unknown", quote="\"",
strip.white=TRUE, fill=FALSE, blank.lines.skip=FALSE, key=NULL,
showProgress=getOption("datatable.showProgress"), # default: TRUE
data.table=getOption("datatable.fread.datatable") # default: TRUE
)
```

**主要参数：**
`input`：输入的文件，也可以是一个URL 
`sep`：列的分隔符，默认的是符号集[,\t |;:]中的第一个 
`nrows`：要读取的行数，默认为-1为全部 
`header`：第一行是否为列名 
`stringAsFactors`：是否把字符串的列转化为因子向量 
`skip`：`skip>0`意味着开始于第`skip+1`行 
`select`：选择要留下的列向量，丢弃剩下的 
`drop`：和`seelct`相反 
`colClasses`：设定列的类型，一个向量或一个列表 
`key`：传递到`setkey`的列名的字符向量，像`key="x,y,z"`或`key=c("x","y","z")`，只有当`data.table=TRUE`时才有效 
`data.table`：TRUE时返回一个`data.table`，FALSE时返回一个`data.frame`
```
n=1e7
DT = data.table( a=sample(1:1000,n,replace=TRUE),
b=sample(1:1000,n,replace=TRUE),
c=rnorm(n),
d=sample(c("foo","bar","baz","qux","quux"),n,replace=TRUE),
e=rnorm(n),
f=sample(1:1000,n,replace=TRUE) )

 fwrite(DT,"DT.csv",row.names = F)
```

写入的数据507M。下面试一试`fread`，`read.csv`和`read_csv`的读入时间。

```
system.time(DT <- fread("DT.csv",header=F))

Read 10000001 rows and 6 (of 6) columns from 0.495 GB file in 00:02:01
  用户   系统   流逝 
118.44   1.83 121.09 

system.time(dt <- read.csv("DT.csv",header=F))
   用户    系统    流逝 
1436.48    6.53 1454.64

library(readr)
system.time(dt <- read_csv("DT.csv"))
 用户  系统  流逝 
19.67  0.58 22.26
```

这儿有些奇怪，用`readr`包里的`read_csv`竟然比`fread`要快，我又试了几次，`fread`几次读取时间差别都很大，在读取到80%之前，几乎每秒读取20%，但是在80%上卡了，最后平均用时40多秒。

# `fsort`
`fsort(x, decreasing = FALSE, na.last = FALSE, internal=FALSE, verbose=FALSE, ...)`
**参数：**
`x`：向量 
`decreasing`：是否是降序排列 
`na.last`：对NA的控制，如果TRUE，则数据中的缺失值放到最后，如果FALSE，放在最前，如果是NA，则去除。
```
x = runif(1e7)
system.time(ans1 <- sort(x,method="quick"))
用户 系统 流逝 
2.53 0.10 2.62 

system.time(ans2 <- fsort(x))
用户 系统 流逝 
0.78 0.19 0.35
```

# `duplicated`

```
duplicated(x, incomparables=FALSE, fromLast=FALSE, by=seq_along(x), ...)

unique(x, incomparables=FALSE, fromLast=FALSE, by=seq_along(x), ...)
```

**参数：**
`x`：一个data.table 
`fromLast`：保留所有重复的最后一个 
`by`：字符或整数向量，指示在`x`中和哪些列的组合为唯一值 
`na.rm`：默认FALSE，缺失值是否要移除
```
DT <- data.table(A = rep(1:3, each=4), B = rep(1:4, each=3),
C = rep(1:2, 6), key = "A,B")
duplicated(DT)
[1] FALSE FALSE  TRUE FALSE FALSE FALSE FALSE FALSE FALSE FALSE FALSE  TRUE

unique(DT)
    A B C
 1: 1 1 1
 2: 1 1 2
 3: 1 2 2
 4: 2 2 1
 5: 2 2 2
 6: 2 3 1
 7: 2 3 2
 8: 3 3 1
 9: 3 4 2
10: 3 4 1

duplicated(DT,by=c("A","C"))
 [1] FALSE FALSE  TRUE  TRUE FALSE FALSE  TRUE  TRUE FALSE FALSE  TRUE  TRUE

unique(DT, by=c("A", "C"))
   A B C
1: 1 1 1
2: 1 1 2
3: 2 2 1
4: 2 2 2
5: 3 3 1
6: 3 4 2
```

# `first`和`last`

```
first(x, ...)
last(x, ...)
```

返回第一个值 
**参数：**
`x`：一个向量，列表，data.frame或data.table
```
x = data.table(x=1:5, y=6:10)
first(x)
   x y
1: 1 6

last(x)
   x  y
1: 5 10
```

和x[1]一样，但是这儿注意，此时的x[1]和用data.frame生成的x[1]是不一样的，用data.table生成的x，x[1]取的是行，而data.rame生成的x，x[1]取的是列。

# `like`

```
like(vector,pattern)
vector %like% pattern
```

调用正则表达式 
**参数：**
`vector`：字符向量或因子，因子更快 
`pattern`：传递到`grepl`
```
DT = data.table(Name=c("Mary","George","Martha"), Salary=c(2,3,4))
 DT
     Name Salary
1:   Mary      2
2: George      3
3: Martha      4

DT[Name %like% "^Mar"]
     Name Salary
1:   Mary      2
2: Martha      4

DT[like(Name,"^Mar")]
     Name Salary
1:   Mary      2
2: Martha      4
```

# `melt`

```
melt(data, id.vars, measure.vars,
variable.name = "variable", value.name = "value",
..., na.rm = FALSE, variable.factor = TRUE,
value.factor = FALSE,
verbose = getOption("datatable.verbose"))
```

**参数：**
`data`：一个要融合的data.table 
`id.vars`：id变量向量 
`measure.vars`：度量变量向量 
`variable.name`：度量变量的名称 
`value.name`:融合后数据值的名称
```
DT <- data.table(
i_1 = c(1:5, NA),
i_2 = c(NA,6,7,8,9,10),
f_1 = factor(sample(c(letters[1:3], NA), 6, TRUE)),
f_2 = factor(c("z", "a", "x", "c", "x", "x"), ordered=TRUE),
c_1 = sample(c(letters[1:3], NA), 6, TRUE),
d_1 = as.Date(c(1:3,NA,4:5), origin="2013-09-01"),
d_2 = as.Date(6:1, origin="2012-01-01"))

 DT
   i_1 i_2 f_1 f_2 c_1        d_1        d_2
1:   1  NA   a   z   a 2013-09-02 2012-01-07
2:   2   6   b   a   a 2013-09-03 2012-01-06
3:   3   7   b   x   a 2013-09-04 2012-01-05
4:   4   8   b   c   b       <NA> 2012-01-04
5:   5   9  NA   x   a 2013-09-05 2012-01-03
6:  NA  10   a   x   b 2013-09-06 2012-01-02

melt(DT,id=1:2,measure="f_1")
   i_1 i_2 variable value
1:   1  NA      f_1     a
2:   2   6      f_1     b
3:   3   7      f_1     b
4:   4   8      f_1     b
5:   5   9      f_1    NA
6:  NA  10      f_1     a

melt(DT,id=c("i_1","i_2"),measure=3)##same as above
   i_1 i_2 variable value
1:   1  NA      f_1     a
2:   2   6      f_1     b
3:   3   7      f_1     b
4:   4   8      f_1     b
5:   5   9      f_1    NA
6:  NA  10      f_1     a

melt(DT, id=1:2, measure=patterns("^f_", "^d_"), value.factor=TRUE)
    i_1 i_2 variable value1     value2
 1:   1  NA        1      a 2013-09-02
 2:   2   6        1      b 2013-09-03
 3:   3   7        1      b 2013-09-04
 4:   4   8        1      b       <NA>
 5:   5   9        1     NA 2013-09-05
 6:  NA  10        1      a 2013-09-06
 7:   1  NA        2      z 2012-01-07
 8:   2   6        2      a 2012-01-06
 9:   3   7        2      x 2012-01-05
10:   4   8        2      c 2012-01-04
11:   5   9        2      x 2012-01-03
12:  NA  10        2      x 2012-01-02
```

# `merge`

快速融合，默认的，它试图融合

> - 首先依据共有的键，如果没有，那么
- 依据第一个参数`x`的键，如果没有，那么
- 依据两个data.table中相同的列


```
merge(x, y, by = NULL, by.x = NULL, by.y = NULL,
all = FALSE, all.x = all, all.y = all, sort = TRUE, suffixes = c(".x", ".y"),
allow.cartesian=getOption("datatable.allow.cartesian"), # default FALSE
...)
```

`x,y`：为data.table 
`by`：在`x,y`中要融合的共享的列名向量 
`by.x, by.y`：`x,y`中要融合的列名向量 
`all`：`all.x=TRUE,all.y=TRUE`的简写 
`all.x`：逻辑值，如果TRUE，额外的行将会加入到输出列，默认为FALSE，即只有在`y,x`中都有的数据才被输出 
`all.y`：上同 
`sort`：逻辑值，如果为TRUE，融合后的数据会根据键排序，如果FALSE，不排序。
```
(dt1 <- data.table(A = letters[1:10], X = 1:10, key = "A"))
    A  X
 1: a  1
 2: b  2
 3: c  3
 4: d  4
 5: e  5
 6: f  6
 7: g  7
 8: h  8
 9: i  9
10: j 10

(dt2 <- data.table(A = letters[5:14], Y = 1:10, key = "A"))
    A  Y
 1: e  1
 2: f  2
 3: g  3
 4: h  4
 5: i  5
 6: j  6
 7: k  7
 8: l  8
 9: m  9
10: n 10

merge(dt1, dt2)
   A  X Y
1: e  5 1
2: f  6 2
3: g  7 3
4: h  8 4
5: i  9 5
6: j 10 6

merge(dt1, dt2, all = TRUE)
    A  X  Y
 1: a  1 NA
 2: b  2 NA
 3: c  3 NA
 4: d  4 NA
 5: e  5  1
 6: f  6  2
 7: g  7  3
 8: h  8  4
 9: i  9  5
10: j 10  6
11: k NA  7
12: l NA  8
13: m NA  9
14: n NA 10

set.seed(1L)
d1 <- data.table(a=sample(rep(1:3,each=2)), z=1:6)
d2 <- data.table(a=2:0, z=10:12)

d1
   a z
1: 1 1
2: 3 2
3: 2 3
4: 2 4
5: 1 5
6: 3 6
d2
   a  z
1: 2 10
2: 1 11
3: 0 12

merge(d1,d2,by="a")
   a z.x z.y
1: 1   1  11
2: 1   5  11
3: 2   3  10
4: 2   4  10

merge(d1,d2,by="a",all=T)
   a z.x z.y
1: 0  NA  12
2: 1   1  11
3: 1   5  11
4: 2   3  10
5: 2   4  10
6: 3   2  NA
7: 3   6  NA

setnames(d2, "a", "b")

d2
   b  z
1: 2 10
2: 1 11
3: 0 12

merge(d1, d2, by.x="a", by.y="b")
   a z.x z.y
1: 1   1  11
2: 1   5  11
3: 2   3  10
4: 2   4  10

merge(d1, d2, by.x="a", by.y="b", all=TRUE)
   a z.x z.y
1: 0  NA  12
2: 1   1  11
3: 1   5  11
4: 2   3  10
5: 2   4  10
6: 3   2  NA
7: 3   6  NA

merge(d2, d1, by.x="b", by.y="a")
   b z.x z.y
1: 1  11   1
2: 1  11   5
3: 2  10   3
4: 2  10   4
```

# 一些介绍data.table的文章
- 
对数据的一些简单的处理，请看[Introduction to data.table](https://cran.r-project.org/web/packages/data.table/vignettes/datatable-intro.html)

- 
常见的一些问题，请看[Frequently Asked Questions about data.table](https://cran.r-project.org/web/packages/data.table/vignettes/datatable-faq.html)

- 
数据的重塑，请看[Efficient reshaping using data.tables](https://cran.r-project.org/web/packages/data.table/vignettes/datatable-reshape.html)

- 
设置键而二值搜索，请看[Keys and fast binary search based subset](https://cran.r-project.org/web/packages/data.table/vignettes/datatable-keys-fast-subset.html)

- 参考语义，请看[Reference semantics](https://cran.r-project.org/web/packages/data.table/vignettes/datatable-reference-semantics.html)](https://so.csdn.net/so/search/s.do?q=data-table&t=blog)](https://so.csdn.net/so/search/s.do?q=R&t=blog)




