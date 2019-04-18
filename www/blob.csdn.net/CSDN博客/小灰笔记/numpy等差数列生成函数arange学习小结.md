# numpy等差数列生成函数arange学习小结 - 小灰笔记 - CSDN博客





2017年02月08日 23:05:33[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：24794
个人分类：[Python](https://blog.csdn.net/grey_csdn/article/category/6639459)









简单接触了numpy的等差数列生成函数arange，在使用上又一点像range函数。在使用上，需要有三个参数，除此之外似乎还有隐含参数。关于隐含参数就不去计较了，暂时先不去学习这么深入的功能。而常用的三个参数中，前两个参数是等差数列的数据范围，第一个参数是等差数列第一个元素。第3个参数则是等差数列的公差。




  1 #!/usr/bin/python

  2 

  3 import numpy
as np

  4 

  5 a = np.arange(0,100,10)

  6 print(a)




运行结果如下：

 $ python arange.py 

[ 0 10 20 30 40 50 60 70 80 90]







如果填入的参数为浮点数，那么生成的等差数列数据类型则为浮点数。




  1 #!/usr/bin/python

  2 

  3 import numpy
as np

  4 

  5 a = np.arange(0.0,100.0,10.0)

  6 print(a)




运行结果如下：

GreydeMac-mini:numpy greyzhang$ python arange.py 

[  0.  10.  20.  30.  40.  50.  60.  70.  80.  90.]



