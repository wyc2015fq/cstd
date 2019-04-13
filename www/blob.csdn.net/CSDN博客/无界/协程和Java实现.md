
# 协程和Java实现 - 无界 - CSDN博客

2019年03月27日 16:56:37[21aspnet](https://me.csdn.net/21aspnet)阅读数：146个人分类：[Java																](https://blog.csdn.net/21aspnet/article/category/1877583)



多线程的性能问题：
1.同步锁。
2.线程阻塞状态和可运行状态之间的切换。
3.线程上下文的切换。
**协程，英文Coroutines，是一种比线程更加轻量级的存在。**正如一个进程可以拥有多个线程一样，一个线程也可以拥有多个协程。
协程，又称微线程，纤程。英文名Coroutine。
最大的优势就是协程极高的执行效率。因为子程序切换不是线程切换，而是由程序自身控制，因此，没有线程切换的开销，和多线程比，线程数量越多，协程的性能优势就越明显。
第二大优势就是不需要多线程的锁机制，因为只有一个线程，也不存在同时写变量冲突，在协程中控制共享资源不加锁，只需要判断状态就好了，所以执行效率比多线程高很多。
因为协程是一个线程执行，那怎么利用多核CPU呢？最简单的方法是多进程+协程，既充分利用多核，又充分发挥协程的高效率，可获得极高的性能。
**Lua语言**
Lua从5.0版本开始使用协程，通过扩展库coroutine来实现。
**Python语言**
正如刚才所写的代码示例，python可以通过 yield/send 的方式实现协程。在python 3.5以后，async/await 成为了更好的替代方案。
**Go语言**
Go语言对协程的实现非常强大而简洁，可以轻松创建成百上千个协程并发执行。
早期版本的Go编译器并不能很智能的发现和利用多核的优势，即使在我们的代码中创建了多个goroutine，但实际上所有这些goroutine都允许在同一个CPU上，在一个goroutine得到时间片执行的时候其它goroutine都会处于等待状态。
实现下面的代码可以显式指定编译器将goroutine调度到多个CPU上运行。
```python
import "runtime"
...
runtime.GOMAXPROCS(4)
```
**Java语言**
如上文所说，Java语言并没有对协程的原生支持，但是某些开源框架模拟出了协程的功能。

### [java协程框架----kilim实现机制解析](https://yueyemaitian.iteye.com/blog/1826322)
java语言处理多任务的模式是基于多线程，java语言级别原生并不支持协程，我们想要java语言支持协程，就需要在线程和协程之间架起一道桥梁。在某个事件点（我们成为挂起点）上，我们在应用级别备份当前任务在线程上的调用栈信息（包括局部变量和操作栈上的数据），释放线程，让它去执行下一个任务；等某些事件被触发的时候，重新执行刚才的任务，用之前备份的调用栈信息恢复线程的调用栈，从挂起点开始执行。

# [Java如何实现协程](https://www.w3cschool.cn/java/java-x3pi2oso.html)
协程（Coroutine）这个词其实有很多叫法，比如有的人喜欢称为纤程（Fiber），或者绿色线程（GreenThread）。其实究其本质，对于协程最直观的解释是线程的线程。虽然读上去有点拗口，但本质上就是这样。
协程的核心在于调度那块由他来负责解决，遇到阻塞操作，立刻放弃掉，并且记录当前栈上的数据，阻塞完后立刻再找一个线程恢复栈并把阻塞的结果放到这个线程上去跑，这样看上去好像跟写同步代码没有任何差别，这整个流程可以称为coroutine，而跑在由coroutine负责调度的线程称为Fiber。
**java协程的实现**
早期，在JVM上实现协程一般会使用kilim，不过这个工具已经很久不更新了，现在常用的工具是Quasar，而本文章会全部基于Quasar来介绍。
下面尝试通过Quasar来实现类似于go语言的coroutine以及channel。
**Quasar是怎么实现Fiber的**
其实Quasar实现的coroutine的方式与Go语言很像，只不过前者是使用框架来实现，而go语言则是语言内置的功能。
不过如果你熟悉了Go语言的调度机制的话，那么对于Quasar的调度机制就会好理解很多了，因为两者有很多相似之处。
Quasar里的Fiber其实是一个continuation，他可以被Quasar定义的scheduler调度，一个continuation记录着运行实例的状态，而且会被随时中断，并且也会随后在他被中断的地方恢复。
Quasar其实是通过修改bytecode来达到这个目的，所以运行Quasar程序的时候，你需要先通过java-agent在运行时修改你的代码，当然也可以在编译期间这么干。go语言的内置了自己的调度器，而Quasar则是默认使用ForkJoinPool这个具有work-stealing功能的线程池来当调度器。work-stealing非常重要，因为你不清楚哪个Fiber会先执行完，而work-stealing可以动态的从其他的等等队列偷一个context过来，这样可以最大化使用CPU资源。
那这里你会问了，Quasar怎么知道修改哪些字节码呢，其实也很简单，Quasar会通过java-agent在运行时扫描哪些方法是可以中断的，同时会在方法被调用前和调度后的方法内插入一些continuation逻辑，如果你在方法上定义了@Suspendable注解，那Quasar会对调用该注解的方法做类似下面的事情。
这里假设你在方法f上定义了@Suspendable，同时去调用了有同样注解的方法g，那么所有调用f的方法会插入一些字节码，这些字节码的逻辑就是记录当前Fiber栈上的状态，以便在未来可以动态的恢复。(Fiber类似线程也有自己的栈)。在suspendable方法链内Fiber的父类会调用Fiber.park，这样会抛出SuspendExecution异常，从而来停止线程的运行，好让Quasar的调度器执行调度。这里的SuspendExecution会被Fiber自己捕获，业务层面上不应该捕获到。如果Fiber被唤醒了(调度器层面会去调用Fiber.unpark)，那么f会在被中断的地方重新被调用(这里Fiber会知道自己在哪里被中断)，同时会把g的调用结果(g会return结果)插入到f的恢复点，这样看上去就好像g的return是f的local variables了，从而避免了callback嵌套。
上面说了一大堆，其实简单点来讲就是，想办法让运行中的线程栈停下来，然后让Quasar的调度器介入。
**JVM线程中断的条件有两个：**
1、抛异常
2、return。
而在Quasar中，一般就是通过抛异常的方式来达到的，所以你会看到上面的代码会抛出SuspendExecution。
[Coroutine in Java - Quasar Fiber实现](https://segmentfault.com/a/1190000006079389)
Quasar Fiber则是通过字节码修改技术在编译或载入时织入必要的上下文保存/恢复代码，通过抛异常来暂停，恢复的时候根据保存的上下文(Continuation),恢复jvm的方法调用栈和局部变量,Quasar Fiber提供相应的Java类库来实现,对应用有一定的侵入性(很小)
Quasar Fiber 主要有 Instrument + Continuation + Scheduler几个部分组成
Instrument做一些代码的植入，如park前后上下文的保存/恢复等
Continuation保存方法调用的信息，如局部变量，引用等,用户态的stack,这个也是跟akka等基于固定callback接口的异步框架最大的区别
Scheduler调度器，负责将fiber分配到具体的os thread执行
下面具体介绍下Quasar Fiber的实现细节,最好先阅读下[quasar官方文档](http://docs.paralleluniverse.co/quasar/)

