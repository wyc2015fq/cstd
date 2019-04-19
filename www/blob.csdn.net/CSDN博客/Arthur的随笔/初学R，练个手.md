# 初学R，练个手 - Arthur的随笔 - CSDN博客
2012年01月12日 00:07:36[largetalk](https://me.csdn.net/largetalk)阅读数：735标签：[plot																[function																[table](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=plot&t=blog)
个人分类：[R&datamine](https://blog.csdn.net/largetalk/article/category/1063234)
随机扔2个色子，求相加之和的数出现概率
```
> data <- outer(1:6, 1:6, "+")
> data
     [,1] [,2] [,3] [,4] [,5] [,6]
[1,]    2    3    4    5    6    7
[2,]    3    4    5    6    7    8
[3,]    4    5    6    7    8    9
[4,]    5    6    7    8    9   10
[5,]    6    7    8    9   10   11
[6,]    7    8    9   10   11   12
> gailv <- function(x) length(x)/length(data)
> fc <- factor(data)
> tapply(data, fc, gailv)
         2          3          4          5          6          7          8 
0.02777778 0.05555556 0.08333333 0.11111111 0.13888889 0.16666667 0.13888889 
         9         10         11         12 
0.11111111 0.08333333 0.05555556 0.02777778
```
画个图
```
> fr <- table(data)
> fr
data
 2  3  4  5  6  7  8  9 10 11 12 
 1  2  3  4  5  6  5  4  3  2  1 
> plot(as.numeric(names(fr)), fr, type="h", xlab="Determinant", ylab="Frequency")
```
![](http://hi.csdn.net/attachment/201201/11/0_13262981981f4Q.gif)
