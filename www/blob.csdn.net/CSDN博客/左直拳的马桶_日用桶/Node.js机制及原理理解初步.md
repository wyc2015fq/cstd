# Node.js机制及原理理解初步 - 左直拳的马桶_日用桶 - CSDN博客
2014年12月16日 19:48:16[左直拳](https://me.csdn.net/leftfist)阅读数：38844
一、node.js优缺点
node.js是单线程。
好处就是
1）简单
2）高性能，避免了频繁的线程切换开销
3）占用资源小，因为是单线程，在大负荷情况下，对内存占用仍然很低
3）线程安全，没有加锁、解锁、死锁这些问题
php
![](https://img-blog.csdn.net/20141216195618405?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
node.js
![](https://img-blog.csdn.net/20141216195721445?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
坏处就是
如何解决高并发？
node使用异步IO和事件驱动（回调函数）来解决这个问题。
一般来说，高并发解决方案会提供多线程模型，为每个业务逻辑提供一个线程，通过系统线程切换来来弥补同步I/O调用的时间开销。像apache，是一个请求一个线程。
而node.js使用的是单线程模型，对所有I/O都采用异步的请求方式，避免频繁的上下文切换，在node.js执行的时候维护着一个事件队列；程序在执行时进入事件循环等待下一个事件到来，每个异步I/O请求完成后都会被推送到事件队列中的等待执行。
比如说：
对于一个简单的数据库访问操作，传统方式是这样实现的
```java
res = db.query('SELECT * from some_table');
 res.output();
```
代码执行到第一行的时候线程会阻塞，等待query返回结果，然后继续处理。由于数据库查询、磁盘读写、网络通信等原因（所谓的I/O）阻塞时间会非常大（相对于CPU始终频率）。对于高并发的访问，一方面线程长期阻塞等待，另一方面为了应付新情求而不断添加新线程，会浪费大量系统资源，同时线程的增加也会也会占用大量的CPU时间来处理内存上下文切换。看看node.js怎么处理
**[javascript]**[view plain](http://blog.csdn.net/leftfist/article/details/41896569#)[copy](http://blog.csdn.net/leftfist/article/details/41896569#)
- db.query('SELECT * from some_table', function(res) {   
-    res.output();  
- });  
- 
query的第二个参数是一个回调函数，进程执行到db.query的时候不会等待结果返回，而是直接继续执行下面的语句，直到进入事件循环。当数据库执行结果返回的时候会将事件发送到事件队列，等到线程进入事件循环后才会调用之前的回调函数。
node.js的异步机制是基于事件的，所有的I/O、网络通信、数据库查询都以非阻塞的方式执行，返回结果由事件循环来处理。node.js在同一时刻只会处理一个事件，完成后立即进入事件循环检查后面事件。这样CPU和内存在同一时间集中处理一件事，同时尽量让耗时的I/O等操作并行执行。
**事件循环机制**
所谓事件循环是指node.js会把所有的异步操作使用事件机制解决，有个线程在不断地循环检测事件队列。
node.js中所有的逻辑都是事件的回调函数，所以node.js始终在事件循环中，程序入口就是事件循环第一个事件的回调函数。事件的回调函数中可能会发出I/O请求或直接发射（ emit）事件，执行完毕后返回事件循环。事件循环会检查事件队列中有没有未处理的事件，直到程序结束。node.js的事件循环对开发者不可见，由libev库实现，libev不断检查是否有活动的、可供检测的事件监听器，直到检查不到时才退出事件循环，程序结束。
如图所示
![](https://img-blog.csdn.net/20141216194247656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
 libuv 是一个高性能事件驱动的程序库，封装了 Windows 和 Unix 平台一些底层特性，为开发者提供了统一的 API.
因此，node.js 是单线程，异步非阻塞。
但毕竟，如何弥补单线程缺陷？是不是有异步非阻塞，就可以高枕无忧了？
不是的。
1）CPU密集型任务存在短板
如上所述，nodejs的机制是单线程，这个线程里面，有一个事件循环机制，处理所有的请求。如图所示。在事件处理过程中，它会智能地将一些涉及到IO、网络通信等耗时比较长的操作，交由worker threads去执行，执行完了再回调，这就是所谓的异步IO非阻塞吧。但是，那些非IO操作，只用CPU计算的操作，它就自己扛了，比如算什么斐波那契数列之类。它是单线程，这些自己扛的任务要一个接着一个地完成，前面那个没完成，后面的只能干等。因此，对CPU要求比较高的CPU密集型任务多的话，就有可能会造成号称高性能，适合高并发的node.js服务器反应缓慢。
![](https://img-blog.csdn.net/20141216195736812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20141216200721188?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2）无法利用CPU的多核
最开始，线程只是用于分配单个处理器处理时间的一种机制。但假如操作系统本身支持多个CPU/内核，那么每个线程都可以得到一个不同自己的CPU/内核，实现真正的“并行运算”。在这种情况下，多线程程序可以提高资源使用效率。Node.js是单线程程序，它只有一个event loop，也只占用一个CPU/内核。现在大部分服务器都是多CPU或多核的，当Node.js程序的event loop被CPU密集型的任务占用，导致有其它任务被阻塞时，却还有CPU/内核处于闲置的状态，造成资源的浪费。
解决方案
利用原生模块或第三方模块，开辟进程或子进程，用于处理这些特殊的任务。
3）如果有异常抛出，因为是单线程，整个项目将不可用。但这归根到底是代码的问题，糟糕的代码，不管什么体系，都会有问题，即使不崩溃。解决办法是用pm2等工具来运行？
二、nodejs与javascript的关系
nodejs本身不是开发语言，它是一个工具或者平台，在服务器端解释、运行javascript；coffeescript属于nodejs体系，算是一种新的开发语言，但它的目的在于最后编译成javascript。
nodejs利用Google V8来解释运行javascript，但是系统真正执行的代码是用C++写的。javascript做的只是调用这些API而已。因此，并无执行效率的问题。
![](https://img-blog.csdn.net/20141216195846231?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
三、nodejs适用场景
1、RESTful API
这是适合 Node 的理想情况，因为您可以构建它来处理数万条连接。它仍然不需要大量逻辑；它本质上只是从某个数据库中查找一些值并将它们组成一个响应。由于响应是少量文本，入站请求也是少量的文本，因此流量不高，一台机器甚至也可以处理最繁忙的公司的 API 需求。
2、实时程序
比如聊天服务
聊天应用程序是最能体现 Node.js 优点的例子：轻量级、高流量并且能良好的应对跨平台设备上运行密集型数据（虽然计算能力低）。同时，聊天也是一个非常值得学习的用例，因为它很简单，并且涵盖了目前为止一个典型的 Node.js 会用到的大部分解决方案。
3、单页APP
ajax很多。现在单页的机制似乎很流行，比如phonegap做出来的APP，一个页面包打天下的例子比比皆是。
。。。
总而言之，NodeJS适合运用在高并发、I/O密集、少量业务逻辑的场景
参考文章
[关于node.js的误会](http://www.cnblogs.com/dolphinx/p/3475090.html)
[Node.js的线程和进程](https://github.com/DoubleSpout/threadAndPackage/blob/master/thread_and_process.md)
[http://www.slideshare.net/mysqlops/nodejs-9313477](http://www.slideshare.net/mysqlops/nodejs-9313477)
[Node.js软肋之CPU密集型任务](http://www.infoq.com/cn/articles/nodejs-weakness-cpu-intensive-tasks)
[http://www.ruanyifeng.com/blog/2014/10/event-loop.html](http://www.ruanyifeng.com/blog/2014/10/event-loop.html)
[http://segmentfault.com/a/1190000000375619](http://segmentfault.com/a/1190000000375619)
[http://www.cnblogs.com/sysuys/p/3460614.html](http://www.cnblogs.com/sysuys/p/3460614.html)
