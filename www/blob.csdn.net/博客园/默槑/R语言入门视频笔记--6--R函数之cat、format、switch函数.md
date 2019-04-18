# R语言入门视频笔记--6--R函数之cat、format、switch函数 - 默槑 - 博客园







# [R语言入门视频笔记--6--R函数之cat、format、switch函数](https://www.cnblogs.com/modaidai/p/6670926.html)





一、cat

猫  怎么就变成一个输出函数了呢？

cat  一个输出函数，功能和print有相同之处

我们一起比较看看



1、cat(“hellow world”)或cat('hellow world')输出的是：

hello world 1 



print("hellow world")或print('hellow world')输出的是:

[1] "hello world 1 "



2、cat(c("AB","C"),c("E","F"),"n",sep = "/")输出的是AB/C/E/F/n

 　print(c("AB","C"),c("E","F"),"n",sep = "/")是不可以的  只能这样：print(c("AB","C"))



3、cat(12,212,31223,file="1.txt")  可以把前面的数字什么的写入当前工作目录的1.txt文件中，可以用这个方法把脚本     输出到文件，加上append=TRUE参数，可以不覆盖原文件，在后面继续添加。

  print肯定不行 别试了



4、如同上文：cat与format函数连用

    cat(type,"is not recognized type\n")





二、format

format函数可以将时间格式，调节成指定时间样式，这是它的功能。

用法实例如下：

 today<-Sys.time()





 format(as.Date(today),format="%Y")  

[1] "2016"  



format(as.POSIXlt(today),format="%Y")  

[1] "2016"  



 format(as.POSIXct(today),format="%Y")  

[1] "2016"  



三、switch

先举个栗子

x<-3
 switch(x,2+2,mean(1:10),rnorm(4))   执行的是rnorm（4）

x<-2
 switch(x,2+2,mean(1:10),rnorm(4))  执行的是mean（1:10）





这你就明白了吧 switch的用法是switch（EXPR，....）

EXPR：可以为一个number或字符串

...：the list of alternatives

若EXPR为字符串，则该list必须具有名字，每个list项为name_i=expr_i，当EXPR与某个name_i严格匹配时，expr_i为结果。

若EXPR不为字符串，则强制转换为integer，后面的list按EXPR取值从1开始对应计算，并作为该函数的结果。



再举个栗子

（1）

for(i in c(-1:3,0)) print(switch(i,1,2,3,4))

结果为

NULL
NULL
[1] 1
[1] 2
[1] 3
NULL

（2）switch("cc",a=10,cc=9,cd=8,d=7)

结果为

9





今天就到这里吧 休息了














