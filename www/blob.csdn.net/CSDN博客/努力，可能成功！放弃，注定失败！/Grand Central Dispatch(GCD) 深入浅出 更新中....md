# Grand Central Dispatch(GCD) 深入浅出 更新中... - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年06月22日 13:39:00[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：349标签：[Grand Central Dispat																[iOS 资源大全																[gcd](https://so.csdn.net/so/search/s.do?q=gcd&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)](https://so.csdn.net/so/search/s.do?q=Grand Central Dispat&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
# [本文来自简书，原文地址:http://www.jianshu.com/p/8cb4f395d2c4](http://www.jianshu.com/p/8cb4f395d2c4)
# 1 GCD 术语
## 1.1 Serial vs. Concurrent 串行 vs. 并发
概念:该术语描述执行当前任务与其他任务之间的关系。串行执行意味着每次只有一个任务被执行；并发执行即同一时间可以有多个任务被执行。
计算机有单核和多核之分，单 CPU 计算机实际为宏观上并行，微观上串行。就冯诺依曼原理来说，微观上只有等待上一条指令执行完毕才会执行下一条指令，任意一个时刻只处理一条指令；而宏观上来讲，我们眼睛所看到的是多个程序在“同时执行”，这又是如何办到呢？很简单，2 个 或 2 个以上的程序交替间隔得到 CPU 处理时间（称之为时间片），给人造成 CPU 在同时处理多个程序的错觉。
注意：概念中并发执行的同一时间 4 个字可以从宏观上理解，当然并发和并行是有一定区别的，具体请看Concurrency vs Parallelism 并发与并行一节。
## 1.2 Synchronous vs. Asynchronous 同步 vs. 异步
概念：术语描述函数执行某个任务后（任务交由 GCD 执行），等待计划任务完成返回称之为同步方式；而异步方式在将任务交由 GCD 执行后立即返回，执行函数中余下的部分代码，若没有则直接退出当前函数。
注意：同步函数将会阻塞当前线程，直到任务执行完毕返回才进行接下来的操作，而异步函数则不会，当然它会很好地完成预定任务。
## 1.3 Critical Section 临界区
概念：通过对多线程的串行化来访问公共资源或一段代码，速度快，适合控制数据访问。在任意时刻只允许一个线程对共享资源进行访问，如果有多个线程试图访问公共资源，那么在有一个线程进入后，其他试图访问公共资源的线程将被挂起，并一直等到进入临界区的线程离开，临界区在被释放后，其他线程才可以抢占。
## 1.4 Race Condition 竞态条件
概念：从多进程间通信的角度来讲，是指两个或多个进程对共享的数据进行读或写的操作时，最终的结果取决于这些进程的执行顺序。
多描述基于特定序列或事件执行时机的软件系统以不受控制的方式运行的行为 ，例如程序的并发任务执行的确切顺序。竞态条件可导致无法预测的行为，而不能通过代码检查立即发现。
## 1.5 Deadlock 死锁
概念：两个或两个以上的进程在执行过程中，由于竞争资源或者由于彼此通信而造成的一种阻塞的现象，若无外力作用，它们都将无法推进下去。此时称系统处于死锁状态或系统产生了死锁，这些永远在互相等待的进程称为死锁进程。
简单理解：两个（有时更多）东西——在大多数情况下，是线程——所谓的死锁是指它们都卡住了，并等待对方完成或执行其它操作。第一个不能完成是因为它在等待第二个的完成。但第二个也不能完成，因为它在等待第一个的完成。
## 1.6 Thread Safe 线程安全
线程安全的代码能在多线程或并发任务中被安全的调用，而不会导致任何问题（数据损坏，崩溃，等）。线程不安全的代码在某个时刻只能在一个上下文中运行。一个线程安全代码的例子是 NSDictionary 。你可以在同一时间在多个线程中使用它而不会有问题。另一方面，NSMutableDictionary 就不是线程安全的，应该保证一次只能有一个线程访问它。
## 1.7 Context Switch 上下文切换
概念：一个上下文切换指当你在单个进程里切换执行不同的线程时存储与恢复执行状态的过程。这个过程在编写多任务应用时很普遍，但会带来一些额外的开销。
注意：前面提及单 CPU 计算机微观上是串行执行任务，同一时刻只允许处理单个任务，采用时间调度方式，从宏观上给人造成多个程序同时处理的假象。而程序A 切换到程序 B 之时必须做一些工作：存储当前程序 A 的作业环境（eg.执行到哪了？环境变量 etc.）；恢复程序 B 的作业环境开始工作，即所谓的上下文切换。
## 1.8 Concurrency vs Parallelism 并发与并行
概念：并发和并行从宏观角度来看都是同时处理多个任务。但并发和并行又有区别，如果你理解的同时是指同一个时刻发生，那么称之为两个或多个任务并行执行；若你理解的同时是指同一时间间隔（0.01秒内）发生，那么称之为多个任务并发执行。
并发代码的不同部分可以同时执行，当然，至于怎么发生或是否发生都取决于系统。多核设备会开辟多个线程同时执行代码的不同部分，称之为并行；然而，单核设备如上面所提及的，它只有一个“大脑”，同一时刻只能执行一项任务，想要实现一样的效果，首先必须运行一个线程，执行上下文切换，然后运行另外一个线程或进程，称之为并发。这通常发生地足够快以致给我们并发执行地错觉，如下图所示：
![](http://upload-images.jianshu.io/upload_images/861390-38770fca3d078bf6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
总结：用数学上的集合符号表示并行 ∈ 并发 ，GCD 中我们可以编写代码要求并发执行，但 GCD 会为我们决定哪些代码并行执行却是未知的。并行执行一定是并发执行，而并发执行不一定是并行执行，毕竟单核设备也能通过上下文切换，造成多个任务“同时执行”的假象。
更多：如果你想深入了解任务的并发执行，不妨看看 [this excellent talk by Rob Pike](http://vimeo.com/49718712)。
## 1.9 Queues 队列
GCD 提供 dispatch queues 管理代码块。这些队列通过 FIFO 方式执行你提供给 GCD 的所有任务。FIFO ： First Input First Output 的缩写，是一种传统的按序执行方式，这意味着第一个被添加到队列里的任务将会是队列中第一个开始的任务，而第二个被添加到队列的任务将会是第二个开始，同理队列中其他任务也是如此。但是第二个任务何时开始我们不得而知，唯一能确定的是它将在第一个任务开始之后执行。
所有的调度队列（dispatch queues）自身都是线程安全，你能同时在多个线程访问它们。 GCD 的优点显而易见的，前提是你必须了解调度队列如何为你自己代码的不同部分提供线程安全。选择正确类型的调度队列和调度函数来提交任务是至关重要的。
GCD 提供了两种调度队列：串行队列和并发队列。
### 1.9.1 Serial Queues 串行队列
串行队列中加入的任务一次有且仅有一个被执行，只有当前一个任务执行完毕，后一个任务才能开始，至于什么时候开始，这取决于 GCD，如下图所示：
![](http://upload-images.jianshu.io/upload_images/861390-29558d80ddc6350d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这些任务的执行时机受到 GCD 的控制；唯一能确保的事情是 GCD 一次只执行一个任务，并且按照我们添加到队列的顺序来执行。
由于在串行队列中不会有两个任务并发运行，因此不会出现同时访问临界区的风险；相对于这些任务来说，这就从竞态条件下保护了临界区。所以如果访问临界区的唯一方式是通过提交到调度队列的任务，那么你就不需要担心临界区的安全问题了。
### 1.9.2 Concurrent Queues 并发队列
并发队列中的任务能得到的保证是它们会按照被添加的顺序开始执行，但这就是全部的保证了。任务可能以任意顺序完成，你不会知道何时开始运行下一个任务，或者任意时刻有多少 Block 在运行。再说一遍，这完全取决于 GCD 。
下图展示了一个示例任务执行计划，GCD 管理着四个并发任务：
![](http://upload-images.jianshu.io/upload_images/861390-642796e5898f16b3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
注意到队列中 4 个任务执行顺序即为添加到队列的顺序，但是 Block1 并未在 Block0 开始后立即执行，而是等待一段时间后开始(图中是在 Block0 执行完毕后开始)，而 Block1、Block2 和 Block3 按照顺序立即执行。
何时开始一个 Block 完全取决于 GCD 。如果一个 Block 的执行时间与另一个重叠，也是由 GCD 来决定是否分配一个 CPU 核单独处理，否则就用上下文切换的方式来执行不同的 Block 。
GCD 提供了至少五个特定的队列，可根据队列类型选择使用。
### 1.9.3 Queue Types 队列类型
|队列|队列类型|说明|
|----|----|----|
|主队列（main queue）|串行|保证所有的任务都在主线程执行，而主线程是唯一用于 UI 更新的线程。此外还用于发送消息给视图或发送通知。|
|四个全局调度队列（high、default、low、background）|并发|Apple 的接口也会使用这些队列，所以你添加的任何任务都不会是这些队列中唯一的任务|
|自定义队列|串行 or 并发|1. 多个任务以串行方式执行，但又不想在主线程中；2. 多个任务以并行方式执行，但不希望队列中有其他系统的任务干扰。|
# 2 API 接口介绍
## 2.1 `dispatch_sync`
一般使用方式：
```
// 线程 A 调用 someMethod 方法
- (void)someMethod {   
    // 同步 
    dispatch_sync(<#dispatch_queue_t  _Nonnull queue#>, <#^(void)block#>)
    // 由于是同步，线程A会被阻塞 
    [self doOtherThing];
}
```
> 
概述：线程 A 执行 someMethod 方法，将任务 block 同步加入到队列 queue 中等待执行，由于是同步加入，表明阻塞线程 A（至于为什么阻塞，可以阅读源代码理解），只有 block 执行完毕，才能继续执行 doOtherThing 方法。
在继续之后的 API 讲解前，希望一定要理解：我们使用 GCD 接口仅涉及 Queue & Task，正确地把 Task 加入到 Queue，然后什么都不用管。而 GCD 所要做的工作正如它的名字：Grand Central Dispatch 任务派发，根据任务性质，所处环境以及机器配置来决定是否使用现有线程，哪个线程，或是创建一个新的线程，然后把任务派发出去。如果是串行队列，它包含多个任务，将任务按照 FIFO 原则派发到同一个线程中执行，至于哪个线程，前面说了视情况而定；如果是并发队列，依旧按照 FIFO
 原则派发都不同的线程中执行。
Q:并发队列是指将每个任务都放到不同线程去执行吗？
这里给出的是raywenderlich的gif讲解：
![](http://upload-images.jianshu.io/upload_images/861390-f7e39952f2ee6361.gif?imageMogr2/auto-orient/strip)
## 2.1 `dispatch_async`
一般使用方式：
```
// 线程 A 调用 someMethod 方法
- (void)someMethod {   
    // 异步 
    dispatch_async(<#dispatch_queue_t  _Nonnull queue#>, <#^(void)block#>)
    // 由于是异步，线程A不会被阻塞
    [self doOtherThing];
}
```
> 
概述：线程 A 执行 someMethod 方法，将任务 block 异步加入到队列 queue 中等待执行，由于是异步加入，线程A不会阻塞，会立即执行 doOtherThing 方法。至于加入到队列的 block，正排着队，等 GCD 分配呢！
同样给出的是raywenderlich的gif讲解：
![](https://koenig-media.raywenderlich.com/uploads/2014/01/dispatch_async_in_action.gif)
## 2.2 `dispatch_after`
一般使用方式：
```
// 线程 A 调用 someMethod 方法
- (void)someMethod {   
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(<#delayInSeconds#> * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        <#code to be executed after a specified delay#>
    });
}
```
> 
概述：`dispatch_after` 不管 someMethod 方法在哪个线程被调用，代码的意思为将任务
 block 加入到主队列中——此时没有把任务派发到主线程中去执行，而是等待delayInSeconds秒才去执行，另外也不会阻塞someMethod方法，这一点和`dispatch_async`一样，其实它更像是一个延迟的`dispatch_async`。
Q：将任务加入到主队列中，如何实现一定时间后在主线程中执行block任务?
## 2.3 `dispatch_once`
一般使用方式：
```
static dispatch_once_t onceToken;
dispatch_once(&onceToken, ^{
    <#code to be executed once#>
});
```
> 
概述：这里不局限于单例生成，能够保证多线程情况下，代码只执行一次！请回顾下 Critical Section 临界区小节内容，是的！没错，这份代码就是临界区内容啦，只允许一个线程访问！其他线程只能干看着，而执行过一次后，我们会设置静态变量onceToken来标识已经执行过了，就算轮到下一个线程访问也不会执行了。
## 2.4 `dispatch_barrier_sync` 和 `dispatch_barrier_async`
首先 sync 和 async 的区别在于是否会阻塞当前线程，因此这里我们更加关注 barrier 的作用：
```cpp
```cpp
// 线程 A 调用 someMethod 方法
- (void)someMethod {   
    // 同步 
    dispatch_barrier_sync(<#dispatch_queue_t  _Nonnull queue#>, <#^(void)block#>)
    // 由于是同步，线程A会被阻塞 
    [self doOtherThing];
}
```
```
可以看到调用方法和 `dispatch_sync` 以及 `dispatch_async` 其实是一致的，无非就是将任务block放入队列queue等待被执行，回顾下：`dispatch_sync`会阻塞当前线程，直到队列中的任务被分派到某个线程被执行完毕；而`dispatch_async` 则不用等待任务完成，不阻塞当前线程，直接执行当前之后的任务。可以看到由这两个方法加入队列的任务，只是遵循了FIFO顺序执行，而其他执行要求则没有了。
> 
概述：是时候说说 barrier 了，由它加入队列的任务（先称为A）也同样遵循FIFO顺序执行，但是重点来了，任务A会等待它之前的所有其他任务完成，才开始执行！而它之后的任务会暂停，等待任务A完成之后继续按照之后顺序来。其他barrier方式加入队列的任务都是如此。
摘自raywenderlich的解释图：
![](http://upload-images.jianshu.io/upload_images/861390-7476193e901d33de.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
刚才解释了Barrier的作用，试想下串行队列和并发队列：串行队列中的任务都是一个接一个的执行，那barrier貌似多此一举了！再想想并发队列，我们希望block0 block1 block2 block3 被分发到不同的线程并发执行，而根据FIFO的顺序轮到 BarrierBlock时，它会等待那四个家伙执行完毕，最晚的那个执行完毕就开始执行BarrierBlock，而队列中它后面的任务都会暂停（正常的会继续将任务分派到线程中）。以大菊官来说，有点像串行方式，(Block 0 Block1 block2 block3)组成一个整体，Barrier
 Block独立一个，（Block 5 Block 6）一个整体！三者之间执行的顺序是串行的！
## 2.5
# 3 Demo 讲解
# 4 源码分析
# 文献
- [grand-central-dispatch-in-depth-part-1](https://www.raywenderlich.com/60749/grand-central-dispatch-in-depth-part-1)
- [grand-central-dispatch-in-depth-part-2](https://www.raywenderlich.com/63338/grand-central-dispatch-in-depth-part-2)
- [GCD 深入理解：第一部分(nixzhu译)](https://github.com/nixzhu/dev-blog/blob/master/2014-04-19-grand-central-dispatch-in-depth-part-1.md)
