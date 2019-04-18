# 使用MATLAB和Python迭代求解黄金分割 - 小灰笔记 - CSDN博客





2017年01月02日 15:05:01[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：785








虽说可能使用Python写几段小代码测试一下算法，但是我现在学习的确实是MATLAB的教程。是Mooler老爷子的书籍，MATLAB编程体验。

第一个话题是迭代，这在学习Python的时候也是一个比较重要的话题。书中的例子是用MATLAB来实现黄金分割的求解。用Python实现了一下如下（看一下，稍后附加MATLAB程序）：

  1 #!/usr/bin/python

  2 

  3 from mathimport *

  4 

  5 x = 3.0

  6 num = 0

  7 while x != sqrt(1 + x):

  8         num += 1

  9         x = sqrt(1 + x)

 10 print("%d times,x = %f" %(num,x))

 11 print("value of x is:%f" % x)

 12 




部分输出结果如下：

27 times,x = 1.618034

28 times,x = 1.618034

29 times,x = 1.618034

30 times,x = 1.618034

31 times,x = 1.618034

value of x is:1.618034




老爷子在书里提到了，其实类似上面这个程序在描述上是有问题的。因为，理论计算上，第7行中的判断条件是永远也不会为True的。之所以能够经过30几步迭代后达到了一个稳定值，是因为MATLAB采用了近似求解的方法。看起来，Python也是采用了类似的求解方法。比较合理的算法，在此处的判断应该修改为x与期望值的偏差在浮点数在相应数值附近是否在最小表示偏差范围内。查了一下，在Pytho的math模块库中并没有提供类似的功能，网络查询倒是发现numpy可以提供相似的功能。

借用numpy的功能，修改代码如下：

  1 #!/usr/bin/python

  2 

  3 from mathimport *

  4 import numpy

  5 

  6 x = 3.0

  7 num = 0

  8 whileabs(x - sqrt(1 + x)) > numpy.spacing(x - sqrt(1
 + x)):

  9         num += 1

 10         x = sqrt(1 + x)

 11 print("%d times,x = %f" %(num,x))

 12 print("value of x is:%f" % x)

 13 




再次运行程序，部分结果显示如下：




27 times,x = 1.618034

28 times,x = 1.618034

29 times,x = 1.618034

30 times,x = 1.618034

31 times,x = 1.618034

value of x is:1.618034




现在的程序计算结果问题已经不大了，但是还是有值得优化的地方，那就是程序的while循环中存在两次相同的平方根运算，这样的效率相对会低一些。进一步优化程序如下：

 1 #!/usr/bin/python

  2 

  3 from mathimport *

  4 import numpy

  5 

  6 x = 3.0

  7 y = 0.0

  8 num = 0

  9 

 10 whileabs(x - y)> numpy.spacing(x):

 11         num += 1

 12         y = x

 13         x = sqrt(1 + x)

 14 print("%d times,x = %f" %(num,x))

 15 print("value of x is:%f" % x)

 16 




再次执行程序，部分县市结果如下：

27 times,x = 1.618034

28 times,x = 1.618034

29 times,x = 1.618034

30 times,x = 1.618034

31 times,x = 1.618034

32 times,x = 1.618034

value of x is:1.618034




附加MATLAB的程序以及部分运算结果如下：

 1 x = 3,y =0;

  2 whileabs(x- y)
> eps(x)

  3     y = x;x = sqrt(1+
 x)

  4 end

  5 




结果：




  8 1.618033988749897

  9 

 10 

 11 x =

 12 

 13 1.618033988749895

 14 

 15 

 16 x =

 17 

 18 1.618033988749895

 19 

 20 

 21 x =

 22 

 23 1.618033988749895



