# python的numba加速 - lyx的专栏 - CSDN博客





2017年11月20日 20:41:20[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：7864
所属专栏：[和python在一起的日子](https://blog.csdn.net/column/details/16842.html)









        之前笔者写过一个pypy的加速方法，可以参阅笔者之前的文章：http://blog.csdn.net/qtlyx/article/details/78078636

        但是这一方法中，我们有一个很不现实的要求，就是所有的python代码都要求是python build-in的库来写。今天，我们使用另外一种jit加速的方法，虽然本质上是一样的，但是其实更加好用，因为支持使用别的库，只要我们把计算瓶颈部分改成使用python的build-in函数来进行计算，毕竟，二八原则，百分之二十的代码支配着百分之八十的速度。

        不说了，我们来看一下：



```python
#-*-coding:utf-8-*-
import time
import pandas as pd
def time_com(i):
    cum = 0
    for test in range(i):
        for ind in range(i):
            cum += (test * ind) % 3

if __name__ == '__main__':
    t1 = time.clock()
    df = pd.DataFrame()
    for i in range(500):
        time_com(i)
    t2 = time.clock()
    print "run time:%f s" % (t2 - t1)
```

控制台上输出的结果是：

run time:7.714948 s

接下来，我们只加入两行代码，分是是引入numba包，一个是用装饰器修饰我们的计算函数：



```python
#-*-coding:utf-8-*-
import time
import pandas as pd
from numba import jit

@jit
def time_com(i):
    cum = 0
    for test in range(i):
        for ind in range(i):
            cum += (test * ind) % 3

if __name__ == '__main__':
    t1 = time.clock()
    df = pd.DataFrame()
    for i in range(500):
        time_com(i)
    t2 = time.clock()
    print "run time:%f s" % (t2 - t1)
```
我们来看一下结果;
run time:0.190903 s

        不说了，勇敢的去加速吧，笔者还特地是用来pandas，然而还是可以加速。但是，要注意的是，在jit装饰器装饰的函数中，不可以有第三方的package哦。










