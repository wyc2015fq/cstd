# 【Python】闭包Closure - zkq_1986的博客 - CSDN博客





2018年01月29日 09:52:11[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：64
个人分类：[程序设计语言](https://blog.csdn.net/zkq_1986/article/category/6534515)










当一个内嵌函数引用其外部作用域的变量,我们就会得到一个闭包. 总结一下,创建一个闭包必须满足以下几点:
- 必须有一个内嵌函数
- 内嵌函数必须引用外部函数中的变量
- 外部函数的返回值必须是内嵌函数

内嵌函数就称为闭包。之所以称为闭包，是因为一旦外部变量传入内嵌函数，内嵌函数就将其包在里面，不与外界再通讯。也就是说所传入的外部变量值就成为一个定值。






如：


```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)>>> def ExFunc(n):
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)     sum=n
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)     def InsFunc():
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)             return sum+1
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)     return InsFunc
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)>>> myFunc=ExFunc(10)
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)>>> myFunc()
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)11
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)>>> myAnotherFunc=ExFunc(20)
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)>>> myAnotherFunc()
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)21
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)>>> myFunc()
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)11
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)>>> myAnotherFunc()
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)21
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)>>>

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```


在这段程序中，函数InsFunc是函数ExFunc的内嵌函数，并且是ExFunc函数的返回值。我们注意到一个问题：内嵌函数InsFunc中 引用到外层函数中的局部变量sum，IronPython会这么处理这个问题呢？先让我们来看看这段代码的运行结果。当我们调用分别由不同的参数调用 ExFunc函数得到的函数时（myFunc()，myAnotherFunc()），得到的结果是隔离的，也就是说每次调用ExFunc函数后都将生成并保存一个新的局部变量sum。其实这里ExFunc函数返回的就是闭包。



转载自：http://python.jobbole.com/85231/#article-comment

https://www.cnblogs.com/JohnABC/p/4076855.html




