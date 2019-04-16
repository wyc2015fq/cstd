# python性能优化(1) - lyx的专栏 - CSDN博客





2017年06月16日 20:48:28[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：553
所属专栏：[和python在一起的日子](https://blog.csdn.net/column/details/16842.html)









看了一系列的公开课，学习了一下python性能分析和性能优化的一些方法。

# 1.为什么要分析性能

目前为止代码的编写还不涉及性能的优化问题，可能是做的东西都不够复杂吧。而且有人会觉得，既然用了python，就何必去追求性能呢。其实我们身边有很多商业化的应用都是用python来实现的，包括youtube和亚马逊。

当然，python和底层语言相比，当然不能拼速度，但是这个速度是运行速度。很多场景，往往需求变化很快，而这就是底层语言的弱点了，即开发周期长。

性能分析的目的基本就是三点：

我们写的python代码运行速度 如何

性能瓶颈在哪里

能有什么改进方案

一个程序的复杂度分为时间复杂度和空间复杂度，前者指的是运行的时间，后者指的是运行所需要的内存存储空间。

这里，我们先学者分析一下时间复杂度。

# 2.时间复杂度分析

在linux中，有time命令，不清楚的可以自行百度。这个命令会返回程序运行的时间情况，其中real与user+sys的比值反映了程序是重IO还是重计算。如果两者很接近，比值几乎为1，那么就是重计算；反之则为重IO。

这里我们不讨论这种方法，而讨论更加一般性的方法。

## 2.1python自带的time模块



```python
import time
print time.time()
```

打印出来的就是当前的时间戳，其实际含义是1970年零点到当期的秒数。所以，很简单的一个方法就是在一段代码执行之前记录一下时间戳，执行完之后，再记录时间戳，然后相减，就是代码运行的时间了。

```python
import time

def loop():
    _sum = 0
    for i in range(100000):
        _sum += i
    return _sum

t0 = time.time()
loop()
t1 = time.time()
print t1 - t0
```

print出来的就是我们想知道这个求和函数的耗时。

## 2.2cProfile

接下来介绍一个更加神奇的东西，cProfile模块，使用方法如下：



```python
import cProfile
def loop():
    _sum = 0
    for i in range(100000):
        _sum += i
    return _sum

cProfile.run("loop()")
```

我们看一下运行结果：
         4 function calls in 0.007 seconds



   Ordered by: standard name



   ncalls  tottime  percall  cumtime  percall filename:lineno(function)

        1    0.000    0.000    0.007    0.007 <string>:1(<module>)

        1    0.006    0.006    0.007    0.007 performance_analysis.py:3(loop)

        1    0.000    0.000    0.000    0.000 {method 'disable' of '_lsprof.Profiler' objects}

        1    0.001    0.001    0.001    0.001 {range}



第一行告诉我们，运行loop()函数之后，一共调用了四个函数，耗时0.0007，接下来是后面的这个表格，最后一列是调用函数的名称，我们看到，一共是四个。其中，主要是消耗在第二个上，0.006和第四个上，0.001.

ncalls是表示这个函数被调用的次数，有时候可能会有3/1这样的次数表示方式。这样的表示的含义是：斜杠前的是总调用次数，斜杠后的是原生调用次数。原生调用就是我们常规认为的调用，与之相对应的就是递归调用，也就是自己调用自己。3/1表示的就是共三次调用，其中一次原生调用，两次递归调用。

tottime这一列是这个函数调用的时间，但是不包括这个函数调用别的函数的时间。

percall就是第二列除以第一列

cumtime这一列表示的是函数进入到函数退出的总时间，包括了我们在这个函数运行中调用的其他函数的运行时间。譬如第二个，loop函数调用了range函数，而loop自身花费了0.006秒，所以第二列是0.006，而他调用range，range花费了0.001秒，所以cumtime就是0.007秒。

后面一列就是cumtime除以第一列。

除了上面这些外，还有两个神奇的东西，line_profiler与memory_profiler，留着下次介绍。



