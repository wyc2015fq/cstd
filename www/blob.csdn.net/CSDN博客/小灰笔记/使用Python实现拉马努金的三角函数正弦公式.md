# 使用Python实现拉马努金的三角函数正弦公式 - 小灰笔记 - CSDN博客





2017年05月06日 12:05:04[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1695








最近其实是在看《数学女孩》的书籍，看着看着觉得中间的故事情节确实是累赘了。切换一下思维，找来了之前看过的拉马努金的故事。看到了其中的公式之后，简单实用程序实现了一下，做一下简单的验证。

在我看到的文章中，有这么一个公式：

![](https://img-blog.csdn.net/20170506120424504?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


简单使用Python实现如下：

  1 #!/usr/bin/python 

  2 

  3 pi =
3.1415926

  4 

  5 deffac(n):

  6     val = 1

  7     i = 1

  8 while(i<=n):

  9         val *= i

 10         i += 1

 11 return val

 12 

 13 defsin(x):

 14     r_val = 0.0

 15 for n
inrange(20):

 16         r_val += (((-1) ** n) * (x ** (2 * n +
1)))/fac(2 * n +
1)

 17 return r_val

 18 

 19 

 20 print("sin(pi/2) = %f" % sin(pi/2))

 21 print("sin(pi/3) = %f" % sin(pi/3))

 22 print("sin(pi/4) = %f" % sin(pi/4))

 23 print("sin(pi/6) = %f" % sin(pi/6))

 24 




程序的实现并不是很准确，仅仅进行了20步的迭代。但是，通过后面四个比较熟悉的计算看结果发现，仅仅10步的迭代已经取得了比较准确的结果。程序的运行结果如下：

GreydeMac-mini:python_exp11 greyzhang$ python lmnj_sin.py 

sin(pi/2) = 1.000000

sin(pi/3) = 0.866025

sin(pi/4) = 0.707107

sin(pi/6) = 0.500000




数学，的确是一个复杂深奥并且充满奥妙的科目。表达方式反复，但是确实最我们理解的世界最本真的描述。



