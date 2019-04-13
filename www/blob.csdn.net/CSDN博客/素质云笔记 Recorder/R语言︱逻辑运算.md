
# R语言︱逻辑运算 - 素质云笔记-Recorder... - CSDN博客

2016年04月28日 21:39:10[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：11939


R软件包含两个逻辑值，TRUE和FALSE。在其他[编程](http://www.2cto.com/kf)语言中也称为布尔值（Boolean values）。布尔向量就是充满着逻辑值的逻辑向量。那么有如何的应用呢？

**1、比较运算可以产生逻辑值**
>、<、>=、<=、==、!=

`b=c(2,3,3,3,5,8,9,3,4,1)
a=c(3,4,9)
a==b#面对长度不相等的恒等比较，相当于补齐a=c(3,4,9,3,4,9,3,4,9,3)然后一一对比`恒等比较的过程中，是将a补齐，然后与b进行一一恒等比较。

**2、&、|、!、&&、||、xor**
注意：运算符“逻辑与”和“逻辑或”存在两种形式，“&”和“|”作用在对象中的每一个元素上并且返回和比较次数相等长度的逻辑值；“&&”和“||”只作用在对象的第一个元素上。
xor为异或，两值不等为真，两值相等为假。例：xor(0, 1)

&  和 &&
这两者都是逻辑与，操作都是x&y和x&&y
如：
`> x <- c(T,T,F)
> y <- c(F,T,F)
> x&&y
[1] FALSE
> x&y
[1] FALSE  TRUE FALSE`
|  和  ||
这两者的使用与前者类似：

`> x <- c(T,T,F)
> y <- c(F,T,F)
> x|y
[1]  TRUE  TRUE FALSE
> x||y
[1] TRUE`


**3、all()和any()**
判断数据中是否存在TRUE值，
其中all()是在全部为TURE时返回T，any是在存在任何一个TRUE时返回TRUE。
他们都还有另外一个参数，即是否删除NA值，即not available值。na.rm
如： > all(x, na.rm=T)

**4、其他**
ifelse()：二者择一 match，%in%：查找unique：找出互不相同的元素 which：找到真值下标集合duplicated：找到重复元素。

**参考链接：**
# [R语言系列：数据的基本运算](http://my.oschina.net/explore/blog/84940)
# [R中的逻辑运算符&, &&, |, ||](http://blog.qiubio.com:8080/archives/2921)
[R 语言常用函数整理](http://blog.sina.com.cn/s/blog_618985870101fub1.html)
[2.8 向量比较](http://book.2cto.com/201305/21950.html)


