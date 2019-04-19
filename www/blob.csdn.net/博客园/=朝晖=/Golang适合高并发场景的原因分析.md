# Golang适合高并发场景的原因分析 - =朝晖= - 博客园
# [Golang适合高并发场景的原因分析](https://www.cnblogs.com/dhcn/p/7124732.html)
http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/ghj1976/article/details/27996095
# 典型的两个现实案例：
我们先看两个用[Go](http://lib.csdn.net/base/go)做消息推送的案例实际处理能力。
## 360消息推送的数据：
16台机器，标配：24个硬件线程，64GB内存 
[Linux](http://lib.csdn.net/base/linux) Kernel 2.6.32 x86_64 
单机80万并发连接，load 0.2~0.4，CPU 总使用率 7%~10%，内存占用20GB (res) 
目前接入的产品约1280万在线用户 
2分钟一次GC，停顿2秒 (1.0.3 的 GC 不给力，直接升级到 tip，再次吃螃蟹) 
15亿个心跳包/天，占大多数。
## 京东云消息推送系统
(团队人数:4) 
单机并发tcp连接数峰值118w 
内存占用23G(Res) 
Load 0.7左右 
心跳包 4k/s 
gc时间2-3.x s
# C10K问题
为什么可以支撑这么高并发的请求呢？我们先从C10K问题说起：2001年左右的时候，有一个叫Dan Kegel的人在网上提出：现在的硬件应该能够让一台机器支持10000个并发的client。然后他讨论了用不同的方式实现大规模并发服务的技术。
[http://www.kegel.com/c10k.html](http://www.kegel.com/c10k.html) （英文版）
[http://www.oschina.net/translate/c10k](http://www.oschina.net/translate/c10k) （中文翻译版） 
[http://www.cnblogs.com/fll/archive/2008/05/17/1201540.html](http://www.cnblogs.com/fll/archive/2008/05/17/1201540.html)
当然， 现在C10K 已经不是问题了, 任何一个普通的程序员, 都能利用手边的语言和库, 轻松地写出 C10K 的服务器. 这既得益于软件的进步, 也得益于硬件性能的提高，现在应该扩展讨论的是应该是C10M问题了。
参考资料：
千万级并发实现的秘密：内核不是解决方案，而是问题所在！ 
[http://www.csdn.net/article/2013-05-16/2815317-The-Secret-to-10M-Concurrent-Connections](http://www.csdn.net/article/2013-05-16/2815317-The-Secret-to-10M-Concurrent-Connections)
# Coroutine模型 和 非阻塞/异步IO(callback)
不论线程还是进程，都不可能一个连接创建一个，相应的成本太大，多进程和多线程都有资源耗费比较大的问题，所以在高并发量的服务器端使用并不多。解决方案是一个线程或者进程处理多个连接，更具体的现在比较主流的是：Coroutine模型 和 非阻塞/异步IO(callback)，在分析这两个之前，我们先看看多进程和多线程的情况。
## 多进程
这种模型在[linux](http://lib.csdn.net/base/linux)下面的服务程序广泛采用，比如大名鼎鼎的apache。
下图说明了Apache的生命周期（prefork模式）。主进程负责监听和管理连接，而具体的业务处理都会交给子进程来处理。
![1234514831_ddvip_588](http://img.blog.csdn.net/20140601215959281)
这种[架构](http://lib.csdn.net/base/architecture)的最大的好处是隔离性，子进程万一crash并不会影响到父进程。缺点就是对系统的负担过重，想像一下如果有上万的连接，会需要多少进程来处理。所以这种模型比较合适那种不需要太多并发量的服务器程序。另外，进程间的通讯效率也是一个瓶颈之一，大部分会采用share memory等技术来减低通讯开销。
apache的处理能力，下面有几篇文章：
2008年时的数据：[http://www.blogjava.net/daniel-tu/archive/2008/12/29/248883.html](http://www.blogjava.net/daniel-tu/archive/2008/12/29/248883.html)
[http://wenku.baidu.com/view/c527582a453610661ed9f40f.html](http://wenku.baidu.com/view/c527582a453610661ed9f40f.html)
### Apache的问题
Apache的问题在于服务器的性能会随着连接数的增多而变差 
关键点：性能和可扩展性并不是一回事。当人们谈论规模时，他们往往是在谈论性能，但是规模和性能是不同的，比如Apache。 
持续几秒的短期连接，比如快速事务，如果每秒处理1000个事务，只有约1000个并发连接到服务器。 
事务延长到10秒，要维持每秒1000个事务，必须打开1万个并发连接。这种情况下：尽管你不顾DoS攻击，Apache也会性能陡降；同时大量的下载操作也会使Apache崩溃。 
如果每秒处理的连接从5千增加到1万，你会怎么做？比方说，你升级硬件并且提高处理器速度到原来的2倍。发生了什么？你得到两倍的性能，但你没有得到两倍的处理规模。每秒处理的连接可能只达到了6000。你继续提高速度，情况也没有改善。甚至16倍的性能时，仍然不能处理1万个并发连接。所以说性能和可扩展性是不一样的。 
问题在于Apache会创建一个CGI进程，然后关闭，这个步骤并没有扩展。 
为什么呢？内核使用的O(N^2)[算法](http://lib.csdn.net/base/datastructure)使服务器无法处理1万个并发连接。 
内核中的两个基本问题： 
连接数=线程数/进程数。当一个数据包进来，内核会遍历其所有进程以决定由哪个进程来处理这个数据包。 
连接数=选择数/轮询次数（单线程）。同样的可扩展性问题，每个包都要走一遭列表上所有的socket。 
解决方法：改进内核使其在常数时间内查找。 
使线程切换时间与线程数量无关。 
使用一个新的可扩展epoll()/IOCompletionPort常数时间去做socket查询。
参考:[http://www.csdn.net/article/2013-05-16/2815317-The-Secret-to-10M-Concurrent-Connections](http://www.csdn.net/article/2013-05-16/2815317-The-Secret-to-10M-Concurrent-Connections)
## 多线程
这种模型在windows下面比较常见。它使用一个线程来处理一个client。他的好处是编程简单，最重要的是你会有一个清晰连续顺序的work flow。简单意味着不容易出错。
这种模型的问题就是太多的线程会减低软件的运行效率。
## 线程和进程的成本
普通的线程，需要消耗1M的堆栈 
[http://www.cnblogs.com/PurpleTide/archive/2010/11/12/1875763.html](http://www.cnblogs.com/PurpleTide/archive/2010/11/12/1875763.html)
多进程和多线程的优缺点...  
[http://blog.163.com/ymguan@yeah/blog/static/140072872201147832740/](http://blog.163.com/ymguan@yeah/blog/static/140072872201147832740/)
我们知道，[操作系统](http://lib.csdn.net/base/operatingsystem)的最小调度单元是“线程”，要执行任何一段代码，都必须落实到“线程”上。可惜线程太重，资源占用太高，频繁创建销毁会带来比较严重的性能问题，于是又诞生出线程池之类的常见使用模式。也是类似的原因，“阻塞”一个线程往往不是一个好主意，因为线程虽然暂停了，但是它所占用的资源还在。线程的暂停和继续对于调度器都会带来压力，而且线程越多，调度时的开销便越大，这其中的平衡很难把握。
针对这个问题，有两类架构解决它：基于callback和coroutine的架构。
## Callback- 非阻塞/异步IO
这种架构的特点是使用非阻塞的IO，这样服务器就可以持续运转，而不需要等待，可以使用很少的线程，即使只有一个也可以。需要定期的任务可以采取定时器来触发。把这种架构发挥到极致的就是[Node.js](http://lib.csdn.net/base/nodejs),一个用[JavaScript](http://lib.csdn.net/base/javascript)来写服务器端程序的框架。在[node.js](http://lib.csdn.net/base/nodejs)中，所有的io都是non-block的，可以设置回调。
举个例子来说明一下。 
传统的写法:
 <span class="kwrd" style="color: rgb(0, 0, 255);">var</span> file = open(‘my.txt’);
 <span class="kwrd" style="color: rgb(0, 0, 255);">var</span> data = file.read(); <span class="rem" style="color: rgb(0, 128, 0);">//block</span>
 sleep(1);
 print(data); //block
node.[js](http://lib.csdn.net/base/javascript)的写法:
 fs.open(‘my.txt’,<span class="kwrd" style="color: rgb(0, 0, 255);">function</span>(err,data){
    setTimeout(1000,<span class="kwrd" style="color: rgb(0, 0, 255);">function</span>(){
       console.log(data);
    }
 }); //non-block
这种架构的好处是performance会比较好，缺点是编程复杂，把以前连续的流程切成了很多片段。另外也不能充分发挥多核的能力。
## Coroutine-协程
coroutine本质上是一种轻量级的thread，它的开销会比使用thread少很多。多个coroutine可以按照次序在一个thread里面执行，一个coroutine如果处于block状态，可以交出执行权，让其他的coroutine继续执行。
非阻塞I/O模型协程(Coroutines)使得开发者可以采用阻塞式的开发风格,却能够实现非阻塞I/O的效果隐式事件调度,
简单来说：协程十分**轻量**，可以在一个进程中执行有数以十万计的协程，依旧保持高性能。
**进程、线程、协程的关系和区别：**
- 进程拥有自己独立的堆和栈，既不共享堆，亦不共享栈，进程由操作系统调度。
- 线程拥有自己独立的栈和共享的堆，共享堆，不共享栈，线程亦由操作系统调度(标准线程是的)。
- 协程和线程一样共享堆，不共享栈，协程由程序员在协程的代码里显示调度。
堆和栈的区别请参看：[http://www.cnblogs.com/ghj1976/p/3623037.html](http://www.cnblogs.com/ghj1976/p/3623037.html)
协程和线程的区别是：协程避免了无意义的调度，由此可以提高性能，但也因此，程序员必须自己承担调度的责任。
执行协程只需要极少的栈内存（大概是4～5KB），默认情况下，线程栈的大小为1MB。
goroutine就是一段代码，一个函数入口，以及在堆上为其分配的一个堆栈。所以它非常廉价，我们可以很轻松的创建上万个goroutine，但它们并不是被操作系统所调度执行。
Google [go](http://lib.csdn.net/base/go)语言对coroutine使用了语言级别的支持，使用关键字go来启动一个coroutine(从这个关键字可以看出Go语言对coroutine的重视),结合chan(类似于message queue的概念)来实现coroutine的通讯，实现了Go的理念 ”Do not communicate by sharing memory; instead, share memory by communicating.”。
[http://my.oschina.net/Obahua/blog/144549](http://my.oschina.net/Obahua/blog/144549)
goroutine 的一个主要特性就是它们的消耗；创建它们的初始内存成本很低廉（与需要 1 至 8MB 内存的传统 POSIX 线程形成鲜明对比）以及根据需要动态增长和缩减占用的资源。这使得 goroutine 会从 4096 字节的初始栈内存占用开始按需增长或缩减内存占用，而无需担心资源的耗尽。
为了实现这个目标，链接器（5l、6l 和 8l）会在每个函数前插入一个序文，这个序文会在函数被调用之前检查判断当前的资源是否满足调用该函数的需求（备注 1）。如果不满足，则调用 runtime.morestack 来分配新的栈页面（备注 2），从函数的调用者那里拷贝函数的参数，然后将控制权返回给调用者。此时，已经可以安全地调用该函数了。当函数执行完毕，事情并没有就此结束，函数的返回参数又被拷贝至调用者的栈结构中，然后释放无用的栈空间。
通过这个过程，有效地实现了栈内存的无限使用。假设你并不是不断地在两个栈之间往返，通俗地讲叫栈分割，则代价是十分低廉的。
简单来说:Go语言通过系统的线程来多路派遣这些函数的执行，使得每个用go关键字执行的函数可以运行成为一个单位协程。当一个协程阻塞的时候，调度器就会自动把其他协程安排到另外的线程中去执行，从而实现了程序无等待并行化运行。而且调度的开销非常小，一颗CPU调度的规模不下于每秒百万次，这使得我们能够创建大量的goroutine，从而可以很轻松地编写高并发程序，达到我们想要的目的。
## Coroutine模型 和 非阻塞/异步IO(callback)性能对比
从性能角度来说，callback的典型node.js和golang的性能[测试](http://lib.csdn.net/base/softwaretest)结果，两者差不多，参考下面测试数据：
[http://www.cnblogs.com/QLeelulu/archive/2012/08/12/2635261.html](http://www.cnblogs.com/QLeelulu/archive/2012/08/12/2635261.html)
不过从代码可读性角度来说，callback确实有点不太好。
参考资料： 
风格之争：Coroutine模型 vs 非阻塞/异步IO(callback) 
[http://blog.csdn.net/kjfcpua/article/details/15809703](http://blog.csdn.net/kjfcpua/article/details/15809703)
Goroutine（协程）为何能处理大并发？ [http://www.cnblogs.com/ghj1976/p/3642513.html](http://www.cnblogs.com/ghj1976/p/3642513.html) python Eventlet [http://www.360doc.com/content/14/0522/00/8504707_379786818.shtml](http://www.360doc.com/content/14/0522/00/8504707_379786818.shtml) 为什么我认为goroutine和channel是把别的平台上类库的功能内置在语言里 [http://blog.zhaojie.me/2013/04/why-channel-and-goroutine-in-golang-are-buildin-libraries-for-other-platforms.html](http://blog.zhaojie.me/2013/04/why-channel-and-goroutine-in-golang-are-buildin-libraries-for-other-platforms.html) Go-简洁的并发 [http://www.yankay.com/go-clear-concurreny/](http://www.yankay.com/go-clear-concurreny/) GOROUTINE性能测试 [http://www.kankanews.com/ICkengine/archives/115285.shtml](http://www.kankanews.com/ICkengine/archives/115285.shtml) Golang特性介绍 [http://mryufeng.iteye.com/blog/576968/](http://mryufeng.iteye.com/blog/576968/) 并发编程 [http://book.2cto.com/201301/14436.html](http://book.2cto.com/201301/14436.html)

