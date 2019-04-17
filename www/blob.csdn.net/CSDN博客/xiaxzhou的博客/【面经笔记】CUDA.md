# 【面经笔记】CUDA - xiaxzhou的博客 - CSDN博客





2017年08月01日 17:25:21[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：1019








> 
[http://blog.csdn.net/junparadox/article/details/50540602](http://blog.csdn.net/junparadox/article/details/50540602)
[http://www.cnblogs.com/muchen/p/6306792.html](http://www.cnblogs.com/muchen/p/6306792.html)


### 与CUDA相关的几个概念：thread，block，grid，warp，sp，sm。
- 
sp: 小核（流处理器）最基本的处理单元，streaming processor  最后具体的指令和任务都是在sp上处理的。GPU进行并行计算，也就是很多个sp同时做处理

- 
sm: 大核（流多处理器）多个sp加上其他的一些资源组成一个sm,  streaming multiprocessor. 其他资源也就是存储资源，共享内存，寄储器等。每个SM包含的SP数量依据GPU架构而不同。

- 
Warp：（线程束）GPU执行程序时的调度单位，一起执行。目前cuda的warp的大小为32，同在一个warp的线程，以不同数据资源执行相同的指令。

- 
grid、block、thread：在利用cuda进行编程时，一个grid分为多个block，而一个block分为多个thread.其中任务划分到是否影响最后的执行效果。划分的依据是任务特性和GPU本身的硬件特性。


### 利用图形API 和CUDA进行GPU通用计算的性能区别？

利用图形API需要把问题转化为图形学的变化；而CUDA是C语言的扩展，比较适合做通用计算

### GPU架构的缺点？如果让你设计，你会怎么改进？

a.**精度问题**，b.编程模式不太灵活

### GPU有通过成千上万线程来隐藏访问延迟，CPU也有隐藏访问延迟的优化，最大的区别是什么？

CPU的线程切换开销是昂贵的，一般都是需要1000cycle而gpu的线程切换只有1个cycle

同一个warp中的thread可以以任意顺序执行，active warps被sm资源限制。当一个warp空闲时，SM就可以调度驻留在该SM中另一个可用warp。在并发的warp之间切换是没什么消耗的，因为硬件资源早就被分配到所有thread和block，所以该新调度的warp的状态已经存储在SM中了。

### 请你列举出一些常用的优化方法

**（线程束->线程块）**对于block和thread的分配问题，有这么一个技巧，每个block里面的thread个数最好是32的倍数，因为，这样可以让线程束一起执行，计算效率更高，促进memory coalescing

**（ 线程块 ）**一个block只会由一个sm调度，**一个sm同一时间只会执行一个block里的warp，当该block里warp执行完才会执行其他block里的warp**。进行划分时，最好保证每个block里的warp比较合理，那样可以一个sm可以交替执行里面的warp，从而提高效率，此外，**在分配block时，要根据GPU的sm个数，分配出合理的block数，让GPU的sm都利用起来，提利用率**。分配时，也要考虑到同一个线程block的资源问题，不要出现对应的资源不够。

一个SP可以执行一个thread，但是实际上并不是所有的thread能够在同一时刻执行。Nvidia把32个threads组成一个warp，warp是调度和运行的基本单元。warp中所有threads并行的执行相同的指令。**一个warp需要占用一个SM运行**，多个warps需要轮流进入SM。由SM的硬件warp scheduler负责调度。目前每个warp包含32个threads（Nvidia保留修改数量的权利）。所以，一个GPU上resident thread最多只有 SM*warp个。 

**（内存）**共享内存的使用量也是影响occupancy的一个重要因子，一块大核拥有一块共享内存。shared添加到变量声明中，这将使这个变量驻留在共享内存中。在声明共享内存变量后，线程块中的每个线程都共享这块内存，使得一个线程块中的多个线程能够在计算上进行通信和协作。

**（内存）**纹理缓存是只读的内存，专门为内存访问存在大量空间局部性的设计，核函数需要特殊的函数告诉GPU读取纹理内存而不是全局内存。使用纹理内存，如果同一个线程束内的thread的访问地址很近的话，那么性能更高。

### CUDA编程同步
- __syncthreads()：

线程调用此函数后，该线程所属**块中的所有线程均运行到这个调用点后才会继续往下运行**。
- cudaDeviceSynchronize()：

主机端CPU会阻塞当前程序的执行，直到所有任务都处理完毕（这里的任务其实就是指的是**所有的线程都已经执行完了kernel function**）。
- cudaStreamSynchronize():

和上面的两个函数类似，这个函数带有一个参数，cuda流ID，它只阻塞那些cuda流ID等于参数中指定ID的那些cuda例程，对于那些流ID不等的例程，还是异步执行的

### 流并行相关

cudaMemcpyAsync（）：

调用函数时，只是放置了一个请求，表示在流中执行一次内存复制操作，这个流式通过参数stream指定的，函数返回无法保证是否已经结束，但复制操作肯定会在执行下一个被放入流中的操作之前执行。

**使用多个流，可以在GPU执行核函数的同时，还能在主机和GPU之间执行复制操作。**

每个流上都装载了一个核函数，同时传递参数有些不同，也就是每个核函数作用的对象也不同。这样就实现了**任务级别的并行**，当我们有几个互不相关的任务时，可以写多个核函数，资源允许的情况下，我们将这些核函数装载到不同流上，然后执行，这样可以实现更粗粒度的并行。



