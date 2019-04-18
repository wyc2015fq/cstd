# Python中的并发编程(1)：起步 - 长歌行 - CSDN博客





2018年03月13日 12:57:17[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1363
所属专栏：[Python与GIS](https://blog.csdn.net/column/details/pygis.html)









## 一些概念

并发编程的概念从1960s兴起，到现在已经是计算机领域一个不可或缺的部分。相信之前接触过编程的朋友对这个都不陌生。 在使用其他编程语言的过程中，或多或少的都接触过并发编程的概念。例如，Task, job, async, event等关键词。但是我在这儿还是要再简单谈谈相关概念。

### 线程

> 
线程（英语：thread）是操作系统能够进行运算调度的最小单位。它被包含在进程之中，是进程中的实际运作单位。一条线程指的是进程中一个单一顺序的控制流，一个进程中可以并发多个线程，每条线程并行执行不同的任务。


### 多线程

> 
多线程（英语：multithreading），是指从软件或者硬件上实现多个线程并发执行的技术。具有多线程能力的计算机因有硬件支持而能够在同一时间执行多于一个线程，进而提升整体处理性能。


### 进程

> 
进程（英语：process），是计算机中已运行程序的实体。进程为曾经是分时系统的基本运作单位。


### 多元处理

> 
多元处理（英语：Multiprocessing），也译为多处理器处理、 多重处理，指在一个单一电脑系统中，使用二个或二个以上的中央处理器，以及能够将计算工作分配给这些处理器。拥有这个能力的电脑系统，也被称为是多元处理器系统（Multiprocessing system）。有时候，运行并发性程序，也会被称为是多元处理。


### 事件驱动程序设计

> 
事件驱动程序设计（英语：Event-driven programming）是一种电脑程序设计模型。这种模型的程序运行流程是由用户的动作（如鼠标的按键，键盘的按键动作）或者是由其他程序的消息来决定的。


### 响应式编程

> 
响应式编程或反应式编程（英语：Reactive programming）是一种面向数据流和变化传播的编程范式。这意味着可以在编程语言中很方便地表达静态或动态的数据流，而相关的计算模型会自动将变化的值通过数据流进行传播。


### 优劣
- 
多线程的优点
- 提高I/O密集型程序的效率
- 相对于进程而言，更加轻量级，使用内存小
- 线程之间资源是共享的，利于交换

- 
多线程的缺点
- CPython的多线程受限于GIL(global interperter lock)，所以效率会有影响
- 需要编写线程竞争的代码和逻辑
- 相对于单线程，在多个线程之间切换上下文的消耗大

- 
多进程的优点
- 能更好的使用多核处理器
- CPU密集型的任务效果比多线程好
- 不会受到GIL的限制
- 单一进程崩溃不会引起程序的崩溃

- 
多进程的缺点
- 进程之间不会共享资源，需要做进程间通讯
- 需要更多的内存


## 一个例子

这里使用一个下载图片的例子，由于I/O以及网络连接的限制，往往这种任务在单线程中效率很低，但是使用多线程，就能成倍的提升效率。

[点击获取源码](https://github.com/bentan2013/PythonAndGIS/blob/master/concurrency/hello_concurrency.py)

```
#!/usr/bin/env python
# -*- coding: utf-8 -*-

# REF
# https://github.com/elliotforbes/Concurrency-With-Python/blob/master/Chapter%2001/concurrentImage.py


import urllib.request
import time
import threading

test_image_url = "http://dimg04.c-ctrip.com/images/fd/hotel/g3/M04/FA/7A/CggYG1adnlmARycYAAF2ezz36-c207_W_1600_1200_Q70.jpg"

def download_image(image_path, file_name):
    urllib.request.urlretrieve(test_image_url, file_name)
    #print(file_name)


def sequential_image():
    for i in range(10):
        image_name = "temp/image-" + str(i) + ".jpg"
        download_image("http://lorempixel.com/400/200/sports", image_name)


def execute_thread(i):
    image_name = "temp/image-" + str(i) + ".jpg"
    download_image("http://lorempixel.com/400/200/sports", image_name)


def concurrent_image():
    threads = []
    for i in range(10):
        thread = threading.Thread(target=execute_thread, args=(i,))
        threads.append(thread)
        thread.start()
    for i in threads:
        i.join()


def calc_time(func):
    start = time.time()
    func()
    end = time.time()
    total_cost_time = end - start
    print("Total execution time of {0}: {1}".format(func.__name__, total_cost_time))

if __name__ == '__main__':
    calc_time(sequential_image)
    calc_time(concurrent_image)
```

直接运行程序，可以得到输出：

```
Total execution time of sequential_image: 9.648064136505127
Total execution time of concurrent_image: 1.1800084114074707
```

证明多线程，确实可以提高效率。

如果屏蔽

```
urllib.request.urlretrieve(test_image_url, file_name)
```

输出结果如下：

```
Total execution time of sequential_image: 0.0005002021789550781
Total execution time of concurrent_image: 0.013999223709106445
```

表明，多线程的上下文切换确实会消耗一部分时间。

## 小结

本文简单谈了下并发编程的相关概念，然后使用一个简单例子，说明了并发编程的适用情况。

敬请期待更多内容。



