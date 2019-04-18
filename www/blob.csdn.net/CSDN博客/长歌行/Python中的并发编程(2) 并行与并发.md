# Python中的并发编程(2): 并行与并发 - 长歌行 - CSDN博客





2018年03月19日 15:15:10[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：532
所属专栏：[Python与GIS](https://blog.csdn.net/column/details/pygis.html)









## 并行与并发

很多人都会有一个问题，并发(Concurrency)和并行(Parallelism)是一个概念吗？它们之间有什么区别呢？只有在充分了解概念的情况下，才能在接下来的学习中，不被文献中充斥的各种概念弄混淆；在实践中，也能更好的选择实现手段。

一般说来，并发，指的是多个任务能够同时存在，是否同时执行并不重要，而并行，主要侧重于多个任务之间能够同时进行。所以，我们也可以理解为并行是并发的一个子集。

![img](https://or45huni1.bkt.clouddn.com/18-3-19/76127716.jpg)

如上图所示，大家分为两队去接饮料，这就是并发。大家分两队，同时分别去两个饮料机接饮料，这就是并行。

### 并发系统的一些特性
- 包含多个操作
- 拥有共享的资源：内存，硬盘或者其他资源
- 遵从一些规则：只有所有的操作都严格的遵循一定的规则，整个系统才能按照期待的方向发展

## 性能瓶颈

并发编程一般都是用于解决一些性能问题，主要的性能问题有两种，I/O性能瓶颈，CPU性能瓶颈。

### I/O瓶颈

这种情况下，程序主要阻塞在输入输出等部分。例如网络爬虫。

### CPU瓶颈

这种情况下，程序主要阻塞在计算部分。主要由时钟频率和CPU的核心个数影响。

使用单核优点：
- 不需要很复杂的通信协定
- 消耗电能更小，适合于物联网。

使用单核缺点：
- 处理速度有限

反过来就是多核的优缺点了。就不一一说明。

## 任务调度

在这儿只是说明任务调度的复杂性和重要性，至于如何安排任务，在以后会进行详细说明。 并发程序的多个线程之间的运作方式有很大的不确定性，如果不加以控制的话，程序运行的结果也会有很强的不确定性。这里以一个经典的生产者，消费者模型/使用场景为例进行说明。

```
#!/usr/bin/env python
# -*- coding: utf-8 -*-


import threading
import time
import random

counter = 1


def workerA():
    global counter
    while counter < 1000:
        counter += 1
        print("Worker A is incrementing counter to {}".format(counter))
        sleepTime = random.randint(0,1)
        time.sleep(sleepTime)


def workerB():
    global counter
    while counter > -1000:
        counter -= 1
        print("Worker B is decrementing counter to {}".format(counter))
        sleepTime = random.randint(0,1)
        time.sleep(sleepTime)


def main():
    t0 = time.time()
    thread1 = threading.Thread(target=workerA)
    thread2 = threading.Thread(target=workerB)

    thread1.start()
    thread2.start()

    thread1.join()
    thread2.join()

    t1 = time.time()

    print("Execution Time {}".format(t1-t0))

if __name__ == '__main__':
    main()
```

## 系统架构

在正式编程之前，还是要介绍下目前的系统架构，主要分为 SISD， SIMD， MISD， MIMD四种。直接从wiki上搬运些资料吧。原地址：[费林分类法](https://zh.wikipedia.org/wiki/%25E8%25B2%25BB%25E6%259E%2597%25E5%2588%2586%25E9%25A1%259E%25E6%25B3%2595)

### SISD

单指令流单数据流（英文：Single instruction, single data，缩写：SISD），每个指令部件每次仅译码一条指令，而且在执行时仅为操作部件提供一份数据。符合冯·诺伊曼结构。

单指令流单数据流是费林分类法中4种计算机处理架构类别的一种。在这个分类系统中，分类根据是指令流和资料流的数量，以此根据来划分计算机处理架构的类别。根据米高·J·费林的观点，当指令、资料处理流水化/管线化时，单指令流单数据流也可以拥有并行计算的特点。管线化的指令读取执行在当代的单指令流单数据流处理机种上很常见。

![img](https://upload.wikimedia.org/wikipedia/commons/thumb/a/ae/SISD.svg/250px-SISD.svg.png)

注明： PU 指的是程序单元(processing unit)

### SIMD

单指令流多数据流（英语：Single Instruction Multiple Data，缩写：SIMD）是一种采用一个控制器来控制多个处理器，同时对一组数据（又称“数据向量”）中的每一个分别执行相同的操作从而实现空间上的并行性的技术。

在微处理器中，单指令流多数据流技术则是一个控制器控制多个平行的处理微元，例如Intel的MMX或SSE，以及AMD的3D Now!指令集。

图形处理器（GPU）拥有强大的并发处理能力和可编程流水线，面对单指令流多数据流时，运算能力远超传统CPU。OpenCL和CUDA分别是目前最广泛使用的开源和专利通用图形处理器（GPGPU）运算语言。

![img](https://upload.wikimedia.org/wikipedia/commons/thumb/2/21/SIMD.svg/225px-SIMD.svg.png)

### MISD

多指令流单数据流（Multiple Instruction stream Single Datastream, MISD）是并行计算机的一种结构。MISD具有n个处理单元，按n条不同指令的要求对同一数据流及其中间结果进行不同的处理。一个处理单元的输出又作为另一个处理单元的输入。

严格意义上，采用这种结构的计算机至今没有出现，但是已经有了近似的例子。在流水线结构中，一条指令的执行过程被分为多个步骤，并且交给不同的硬件处理单元，以加快指令的执行速度。

![img](https://upload.wikimedia.org/wikipedia/commons/thumb/9/97/MISD.svg/250px-MISD.svg.png)

### MIMD

多指令流多数据流（Multiple Instruction Stream Multiple Data Stream，缩写：MIMD），是使用多个控制器来异步地控制多个处理器，从而实现空间上的并行性的技术。

![img](https://upload.wikimedia.org/wikipedia/commons/thumb/c/c6/MIMD.svg/225px-MIMD.svg.png)

## 计算机的内存模式

在正式开始程序设计之前，计算机的内存模式也是需要做了解的概念。

### 均匀访存模型(UMA)

均匀访存模型（Uniform Memory Access）通常简称UMA，指所有的物理存储器被均匀共享，即处理器访问它们的时间是一样的。UMA亦称作统一寻址技术或统一内存存取。

这种系统因为高度的资源共享也被称为紧耦合系统（Tightly Coupled System）。

![img](https://or45huni1.bkt.clouddn.com/18-3-19/30034127.jpg)

这种模式的好处是，缓存都是一致的，硬件也容易设计，但是不利于扩展。

### 非统一内存访问架构(NUMA)

非统一内存访问架构（英语：Non-uniform memory access，简称NUMA）是一种为多处理器的电脑设计的内存，内存访问时间取决于内存相对于处理器的位置。在NUMA下，处理器访问它自己的本地内存的速度比非本地内存（内存位于另一个处理器，或者是处理器之间共享的内存）快一些。

非统一内存访问架构的特点是：被共享的内存物理上是分布式的，所有这些内存的集合就是全局地址空间。所以处理器访问这些内存的时间是不一样的，显然访问本地内存的速度要比访问全局共享内存或远程访问外地内存要快些。另外，NUMA中内存可能是分层的：本地内存，群内共享内存，全局共享内存。

![img](https://or45huni1.bkt.clouddn.com/18-3-19/74820258.jpg)

这种模式利于扩展，但是处理器对于不同位置的内存的访问速度不一致，并且多个处理器之间需要识别并维护内存内容的一致性。

## 总结

本节也只是对一般的概念做介绍，即使不需要使用python，但是对并发感兴趣的同学也可以了解下相关的概念。



