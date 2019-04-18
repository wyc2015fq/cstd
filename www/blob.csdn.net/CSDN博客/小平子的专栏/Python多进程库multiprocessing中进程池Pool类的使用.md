# Python多进程库multiprocessing中进程池Pool类的使用 - 小平子的专栏 - CSDN博客





2016年09月04日 18:27:21[阿拉丁吃米粉](https://me.csdn.net/jinping_shi)阅读数：32783标签：[python																[并行																[多进程](https://so.csdn.net/so/search/s.do?q=多进程&t=blog)




# 问题起因

最近要将一个文本分割成好几个topic，每个topic设计一个regressor，各regressor是相互独立的，最后汇总所有topic的regressor得到总得预测结果。没错！类似bagging ensemble！只是我没有抽样。文本不大，大概3000行，topic个数为8，于是我写了一个串行的程序，一个topic算完之后再算另一个topic。可是我在每个topic中用了`GridSearchCV`来调参，又要选特征又要调整regressor的参数，导致参数组合一共有1782种。我真是低估了调参的时间，程序跑了一天一夜最后因为忘记import一个库导致最终的预测精度没有算出来。后来想到，既然每个topic的预测都是独立的，那是不是可以并行呢？

# Python中的多线程与多进程

但是听闻Python的多线程实际上并不能真正利用多核，所以如果使用多线程实际上还是在一个核上做并发处理。不过，如果使用多进程就可以真正利用多核，因为各进程之间是相互独立的，不共享资源，可以在不同的核上执行不同的进程，达到并行的效果。同时在我的问题中，各topic相互独立，不涉及进程间的通信，只需最后汇总结果，因此使用多进程是个不错的选择。

# multiprocessing

## 一个子进程

multiprocessing模块提供process类实现新建进程。下述代码是新建一个子进程。

```python
from multiprocessing import Process

def f(name):
    print 'hello', name

if __name__ == '__main__':
    p = Process(target=f, args=('bob',)) # 新建一个子进程p，目标函数是f，args是函数f的参数列表
    p.start() # 开始执行进程
    p.join() # 等待子进程结束
```

上述代码中`p.join()`的意思是等待子进程结束后才执行后续的操作，一般用于进程间通信。例如有一个读进程pw和一个写进程pr，在调用pw之前需要先写`pr.join()`，表示等待写进程结束之后才开始执行读进程。

# 多个子进程

如果要同时创建多个子进程可以使用`multiprocessing.Pool`类。该类可以创建一个进程池，然后在多个核上执行这些进程。

```python
import multiprocessing
import time

def func(msg):
    print multiprocessing.current_process().name + '-' + msg

if __name__ == "__main__":
    pool = multiprocessing.Pool(processes=4) # 创建4个进程
    for i in xrange(10):
        msg = "hello %d" %(i)
        pool.apply_async(func, (msg, ))
    pool.close() # 关闭进程池，表示不能在往进程池中添加进程
    pool.join() # 等待进程池中的所有进程执行完毕，必须在close()之后调用
    print "Sub-process(es) done."
```

输出结果如下：

```
Sub-process(es) done.
PoolWorker-34-hello 1
PoolWorker-33-hello 0
PoolWorker-35-hello 2
PoolWorker-36-hello 3
PoolWorker-34-hello 7
PoolWorker-33-hello 4
PoolWorker-35-hello 5
PoolWorker-36-hello 6
PoolWorker-33-hello 8
PoolWorker-36-hello 9
```

上述代码中的`pool.apply_async()`是`apply()`函数的变体，`apply_async()`是`apply()`的并行版本，`apply()`是`apply_async()`的阻塞版本，使用`apply()`主进程会被阻塞直到函数执行结束，所以说是阻塞版本。`apply()`既是`Pool`的方法，也是Python内置的函数，两者等价。可以看到输出结果并不是按照代码for循环中的顺序输出的。

# 多个子进程并返回值

`apply_async()`本身就可以返回被进程调用的函数的返回值。上一个创建多个子进程的代码中，如果在函数`func`中返回一个值，那么`pool.apply_async(func, (msg, ))`的结果就是返回pool中所有进程的**值的对象（注意是对象，不是值本身）**。

```python
import multiprocessing
import time

def func(msg):
    return multiprocessing.current_process().name + '-' + msg

if __name__ == "__main__":
    pool = multiprocessing.Pool(processes=4) # 创建4个进程
    results = []
    for i in xrange(10):
        msg = "hello %d" %(i)
        results.append(pool.apply_async(func, (msg, )))
    pool.close() # 关闭进程池，表示不能再往进程池中添加进程，需要在join之前调用
    pool.join() # 等待进程池中的所有进程执行完毕
    print ("Sub-process(es) done.")

    for res in results:
        print (res.get())
```

上述代码输出结果如下：

```
Sub-process(es) done.
PoolWorker-37-hello 0
PoolWorker-38-hello 1
PoolWorker-39-hello 2
PoolWorker-40-hello 3
PoolWorker-37-hello 4
PoolWorker-38-hello 5
PoolWorker-39-hello 6
PoolWorker-37-hello 7
PoolWorker-40-hello 8
PoolWorker-38-hello 9
```

与之前的输出不同，这次的输出是有序的。

如果电脑是八核，建立8个进程，在Ubuntu下输入top命令再按下大键盘的1，可以看到每个CPU的使用率是比较平均的，如下图：

![八核CPU使用情況](https://img-blog.csdn.net/20160904182454185)

在system monitor中也可以清楚看到执行多进程前后CPU使用率曲线的差异。 
![CPU使用情況](https://img-blog.csdn.net/20160904182523979)](https://so.csdn.net/so/search/s.do?q=并行&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)




