# Python 性能分析工具简介 - weixin_33985507的博客 - CSDN博客
2016年11月18日 00:30:52[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
# 性能分析和调优工具简介
总会遇到一个时候你会想提高程序执行效率，想看看哪部分耗时长成为瓶颈，想知道程序运行时内存和CPU使用情况。这时候你会需要一些方法对程序进行性能分析和调优。
## By Context Manager
可以上下文管理器自己实现一个计时器, 参见之前的介绍 `timeit` 文章里做的那样，通过定义类的 `__enter__` 和 `__exit__` 方法来实现对管理的函数计时, 类似如:
```
# timer.py
import time
class Timer(object):
    def __init__(self, verbose=False):
        self.verbose = verbose
    def __enter__(self):
        self.start = time.time()
        return self
    def __exit__(self, *args):
        self.end = time.time()
        self.secs = self.end - self.start
        self.msecs = self.secs * 1000            # 毫秒
        if self.verbose:
            print 'elapsed time: %f ms' % self.msecs
```
使用方式如下:
```
from timer import Timer
with Timer() as t:
    foo()
print "=> foo() spends %s s" % t.secs
```
## By Decorator
然而我认为装饰器的方式更加优雅
```
import time
from functools import wraps
def timer(function):
    @wraps(function)
    def function_timer(*args, **kwargs):
        t0 = time.time()
        result = function(*args, **kwargs)
        t1 = time.time()
        print ("Total time running %s: %s seconds" %
                (function.func_name, str(t1-t0))
                )
        return result
    return function_timer
```
使用就很简单了:
```
@timer
def my_sum(n):
    return sum([i for i in range(n)])
if __name__ == "__main__":
    my_sum(10000000)
```
运行结果:
```
➜  python profile.py
Total time running my_sum: 0.817697048187 seconds
```
## 系统自带的time命令
使用示例如下:
```
➜ time python profile.py
Total time running my_sum: 0.854454040527 seconds
python profile.py  0.79s user 0.18s system 98% cpu 0.977 total
```
上面的结果说明: 执行脚本消耗0.79sCPU时间， 0.18秒执行内核函数消耗的时间，总共0.977s时间。
其中， `total时间 - (user时间 + system时间) = 消耗在输入输出和系统执行其它任务消耗的时间`
## python timeit 模块
可以用来做benchmark, 可以方便的重复一个程序执行的次数，来查看程序可以运行多块。具体参考之前写的[文章](http://www.jianshu.com/p/20b6fb168824)。
## cProfile
直接看带注释的使用示例吧。
```
#coding=utf8
def sum_num(max_num):
    total = 0
    for i in range(max_num):
        total += i
    return total
def test():
    total = 0
    for i in range(40000):
        total += i
    t1 = sum_num(100000)
    t2 = sum_num(200000)
    t3 = sum_num(300000)
    t4 = sum_num(400000)
    t5 = sum_num(500000)
    test2()
    return total
def test2():
    total = 0
    for i in range(40000):
        total += i
    t6 = sum_num(600000)
    t7 = sum_num(700000)
    return total
if __name__ == "__main__":
    import cProfile
    # # 直接把分析结果打印到控制台
    # cProfile.run("test()")
    # # 把分析结果保存到文件中
    # cProfile.run("test()", filename="result.out")
    # 增加排序方式
    cProfile.run("test()", filename="result.out", sort="cumulative")
```
cProfile将分析的结果保存到result.out文件中，但是以二进制形式存储的，想直接查看的话用提供的 `pstats` 来查看。
```
import pstats
# 创建Stats对象
p = pstats.Stats("result.out")
# strip_dirs(): 去掉无关的路径信息
# sort_stats(): 排序，支持的方式和上述的一致
# print_stats(): 打印分析结果，可以指定打印前几行
# 和直接运行cProfile.run("test()")的结果是一样的
p.strip_dirs().sort_stats(-1).print_stats()
# 按照函数名排序，只打印前3行函数的信息, 参数还可为小数,表示前百分之几的函数信息
p.strip_dirs().sort_stats("name").print_stats(3)
# 按照运行时间和函数名进行排序
p.strip_dirs().sort_stats("cumulative", "name").print_stats(0.5)
# 如果想知道有哪些函数调用了sum_num
p.print_callers(0.5, "sum_num")
# 查看test()函数中调用了哪些函数
p.print_callees("test")
```
截取一个查看test()调用了哪些函数的输出示例:
```
➜  python python profile.py
   Random listing order was used
   List reduced from 6 to 2 due to restriction <'test'>
Function              called...
                          ncalls  tottime  cumtime
profile.py:24(test2)  ->       2    0.061    0.077  profile.py:3(sum_num)
                               1    0.000    0.000  {range}
profile.py:10(test)   ->       5    0.073    0.094  profile.py:3(sum_num)
                               1    0.002    0.079  profile.py:24(test2)
                               1    0.001    0.001  {range}
```
### profile.Profile
cProfile还提供了可以自定义的类，可以更精细的分析, 具体看[文档](https://docs.python.org/2/library/profile.html#module-cProfile)。
格式如： `class profile.Profile(timer=None, timeunit=0.0, subcalls=True, builtins=True)`
下面这个例子来自官方文档:
```
import cProfile, pstats, StringIO
pr = cProfile.Profile()
pr.enable()
# ... do something ...
pr.disable()
s = StringIO.StringIO()
sortby = 'cumulative'
ps = pstats.Stats(pr, stream=s).sort_stats(sortby)
ps.print_stats()
print s.getvalue()
```
## lineprofiler
lineprofiler是一个对函数进行逐行性能分析的工具，可以参见github项目说明，地址: [https://github.com/rkern/line...](https://github.com/rkern/line_profiler)
### 示例
```
#coding=utf8
def sum_num(max_num):
    total = 0
    for i in range(max_num):
        total += i
    return total
@profile                     # 添加@profile 来标注分析哪个函数
def test():
    total = 0
    for i in range(40000):
        total += i
    t1 = sum_num(10000000)
    t2 = sum_num(200000)
    t3 = sum_num(300000)
    t4 = sum_num(400000)
    t5 = sum_num(500000)
    test2()
    return total
def test2():
    total = 0
    for i in range(40000):
        total += i
    t6 = sum_num(600000)
    t7 = sum_num(700000)
    return total
test()
```
通过 `kernprof` 命令来注入分析，运行结果如下：
```
➜ kernprof -l -v profile.py
Wrote profile results to profile.py.lprof
Timer unit: 1e-06 s
Total time: 3.80125 s
File: profile.py
Function: test at line 10
Line #      Hits         Time  Per Hit   % Time  Line Contents
==============================================================
    10                                           @profile
    11                                           def test():
    12         1            5      5.0      0.0      total = 0
    13     40001        19511      0.5      0.5      for i in range(40000):
    14     40000        19066      0.5      0.5          total += i
    15
    16         1      2974373 2974373.0     78.2      t1 = sum_num(10000000)
    17         1        58702  58702.0      1.5      t2 = sum_num(200000)
    18         1        81170  81170.0      2.1      t3 = sum_num(300000)
    19         1       114901 114901.0      3.0      t4 = sum_num(400000)
    20         1       155261 155261.0      4.1      t5 = sum_num(500000)
    21         1       378257 378257.0     10.0      test2()
    22
    23         1            2      2.0      0.0      return total
```
hits（执行次数） 和 time（耗时） 值高的地方是有比较大优化空间的地方。
## memoryprofiler
类似于"lineprofiler"对基于行分析程序内存使用情况的模块。github 地址：[https://github.com/fabianp/me...](https://github.com/fabianp/memory_profiler) 。ps:安装 `psutil`, 会分析的更快。
同样是上面"lineprofiler"中的代码，运行 `python -m memory_profiler profile.py` 命令生成结果如下:
```
➜ python -m memory_profiler profile.py
Filename: profile.py
Line #    Mem usage    Increment   Line Contents
================================================
    10   24.473 MiB    0.000 MiB   @profile
    11                             def test():
    12   24.473 MiB    0.000 MiB       total = 0
    13   25.719 MiB    1.246 MiB       for i in range(40000):
    14   25.719 MiB    0.000 MiB           total += i
    15
    16  335.594 MiB  309.875 MiB       t1 = sum_num(10000000)
    17  337.121 MiB    1.527 MiB       t2 = sum_num(200000)
    18  339.410 MiB    2.289 MiB       t3 = sum_num(300000)
    19  342.465 MiB    3.055 MiB       t4 = sum_num(400000)
    20  346.281 MiB    3.816 MiB       t5 = sum_num(500000)
    21  356.203 MiB    9.922 MiB       test2()
    22
    23  356.203 MiB    0.000 MiB       return total
```
## TODO objgraph
# 参考资料:
- 
[https://docs.python.org/2/lib...](https://docs.python.org/2/library/profile.html#module-cProfile)
- 
[http://xianglong.me/article/a...](http://xianglong.me/article/analysis-python-application-performance-using-cProfile/)
- 
[http://www.cnblogs.com/btchen...](http://www.cnblogs.com/btchenguang/archive/2012/02/03/2337112.html)
- 
[https://www.huyng.com/posts/p...](https://www.huyng.com/posts/python-performance-analysis)
- 
[http://www.marinamele.com/7-t...](http://www.marinamele.com/7-tips-to-time-python-scripts-and-control-memory-and-cpu-usage)
# NEXT 代码的调优tips
