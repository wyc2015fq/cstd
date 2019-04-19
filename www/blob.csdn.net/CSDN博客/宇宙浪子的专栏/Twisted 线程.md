# Twisted 线程 - 宇宙浪子的专栏 - CSDN博客
2016年01月12日 09:31:23[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：1805
转自：[http://www.cnblogs.com/zhengyun_ustc/archive/2010/05/18/1738357.html](http://www.cnblogs.com/zhengyun_ustc/archive/2010/05/18/1738357.html)
[Twisted 的 callInThread 和 callFromThread 区别](http://my.oschina.net/shniu/blog/210256?fromerr=AvlNCz6G#OSC_h2_1)[threads.deferToThread和reactor.callInThread的区别](http://my.oschina.net/shniu/blog/210256?fromerr=AvlNCz6G#OSC_h2_2)[twisted 的 defer.succeed](http://my.oschina.net/shniu/blog/210256?fromerr=AvlNCz6G#OSC_h2_3)[twisted. internet.defer.succeed(result)](http://my.oschina.net/shniu/blog/210256?fromerr=AvlNCz6G#OSC_h3_4)
一般让爬虫在一个进程内多线程并发，有几种方法：
[Stackless](http://www.stackless.com/) ：Stackless Python是Python的一个增强版本。Stackless Python修改了Python的代码，提供了对微线程的支持。微线程是轻量级的线程，与前边所讲的线程相比，微线程在多个线程间切换所需的时间更多，占用资源也更少。
[Twisted](http://twistedmatrix.com/) ：主要利用 Twisted 中的异步编程能力。如 addCallback ， callLater ， defer.succeed ，deferToThread ， callFromThread 和 callInThread 等等。
threading 和 Queue ：这都是 Python 原生库。从这个库可以衍生出很多线程池的第三方实现。如[2003年的一个实现](http://code.activestate.com/recipes/203871-a-generic-programming-thread-pool/)。比如 [Christopher
 Arndt 的一个版本](http://chrisarndt.de/projects/threadpool/)。比如[2010年的一个实现](http://code.activestate.com/recipes/577187-python-thread-pool/)。
greenlet 和 [eventlet](http://eventlet.net/) ：greenlet 不是一种真正的并发机制，而是在同一线程内，在不同函数的执行代码块之间切换，实施“你运行一会、我运行一会”，并且在进行切换时必须指定何时切换以及切换到哪。粗糙来讲，greenlet是“阻塞了我就先干点儿别的，但是程序员得明确告诉greenlet能先干点儿啥以及什么时候回来”。
（注：关于 Python Threading 代码片段，参考：[http://code.activestate.com/recipes/langs/python/tags/meta:requires=threading/](http://code.activestate.com/recipes/langs/python/tags/meta:requires=threading/) ，有很多例子）
## Twisted 的 callInThread 和 callFromThread 区别
这两个函数的定义在  [IReactorThreads
 的文档](http://twistedmatrix.com/documents/10.0.0/api/twisted.internet.interfaces.IReactorThreads.html)里。
Method [callInThread](http://twistedmatrix.com/documents/10.0.0/api/twisted.internet.interfaces.IReactorThreads.html#callInThread) ：
Run the callable object in a separate thread.
Method [callFromThread](http://twistedmatrix.com/documents/10.0.0/api/twisted.internet.interfaces.IReactorThreads.html#callFromThread) ：
Cause a function to be executed by the reactor thread.
Use this method when you want to run a function in the reactor's thread from another thread. Calling [`callFromThread`](http://twistedmatrix.com/documents/10.0.0/api/twisted.internet.interfaces.IReactorThreads.html#callFromThread) should
 wake up the main thread (where `reactor.run()` is executing) and run the given callable in that thread.
If you're writing a multi-threaded application the `callable` may need to be thread safe, but this method doesn't
 require it as such. If you want to call a function in the next mainloop iteration, but you're in the same thread, use [`callLater`](http://twistedmatrix.com/documents/10.0.0/api/twisted.internet.interfaces.IReactorTime.html#callLater) with
 a delay of 0.
也就是说，reactor.callFromThread 是在由 reactor.run() 激发的主消息循环（main event loop）中执行，所以也就能被 reactor.stop() 终止执行。甚至可以通过：
reactor.callFromThread(reactor.stop)
来主动要求主消息循环关闭 reactor 的主线程运行。
callFromThread 有时候比较危险，如果压的任务太多，会阻塞主消息循环，造成其他事件无法得到及时的处理。
参考 callInThread 的代码，可以看出它是在 reactor 的一个私有线程池里工作的：
def callInThread(self, _callable, *args, **kwargs):
    if self.threadpool is None:
        self._initThreadPool()
    self.threadpool.callInThread(_callable, *args, **kwargs)
所以，我们可以通过
from twisted.internet import reactor
reactor.[suggestThreadPoolSize](http://twistedmatrix.com/documents/10.0.0/api/twisted.internet.interfaces.IReactorThreads.html#suggestThreadPoolSize)(15)
来设置该线程池的大小。默认最小是5个线程，最大10个（[the default reactor
 uses a minimum size of 5 and a maximum size of 10](http://twistedmatrix.com/documents/current/core/howto/threading.html#auto3)）。
这里有两个问题：
1、如何通知 callInThread 执行任务的线程退出呢，如何确保线程池内的工作线程安全退出呢？
2、如果让工作线程去某网站抓取页面，由于 TCP/IP 的不确定性，可能该工作线程挂起，长时间不返回。如果线程池内的每一个线程被这样耗尽，没有空闲线程，就相当于抓取全部停止了。某个线程或许会因请求超时而退出，但这也未必可靠。一般通过代码：
import timeoutsocket 
timeoutsocket.setDefaultSocketTimeout(120)
设置 socket 超时时间，但有时候就是会莫名其妙地挂住线程。
## threads.deferToThread和reactor.callInThread的区别
twisted.internet.threads.deferToThread 与 callInThread 一样，默认用 reactor.getThreadPool() 所开辟的线程池。它调用这个线程池的 threadpool.callInThreadWithCallback 方法，实际效果和 reactor.callInThread 一样。区别只是 deferToThread 可以返回一个deferred对象，从而允许你设定回调函数。
示范代码：
def finish_success(request): 
    pass 
threads.deferToThread(parseData, body).addCallback(lambda x: finish_success(request))
## twisted 的 defer.succeed
twisted还提供了一个简易办法
### [twisted](http://twistedmatrix.com/documents/current/api/twisted.html).[internet](http://twistedmatrix.com/documents/current/api/twisted.internet.html).[defer](http://twistedmatrix.com/documents/current/api/twisted.internet.defer.html).[succeed](http://twistedmatrix.com/documents/current/api/twisted.internet.defer.succeed.html)(result)
Return a Deferred that has already had '.callback(result)' called.
This is useful when you're writing synchronous code to an asynchronous interface: i.e., some code is calling you expecting a Deferred result, but you don't actually need to do anything asynchronous. Just return defer.succeed(theResult).
代码示范参考 howto 文档的 [Returning
 Deferreds from synchronous functions](http://twistedmatrix.com/documents/current/core/howto/gendefer.html#auto4)。
还可以参考 《[Twisted
 服务器开发技巧(1) - 将性能优化到底](http://wiki.woodpecker.org.cn/moin/Twisted%E6%9C%8D%E5%8A%A1%E5%99%A8%E5%BC%80%E5%8F%91%E6%8A%80%E5%B7%A7%281%29)》中的示范。
defer.succeed 说白了就是为了让某函数 A 返回一个 Deferred 对象，从而让 A.addCallback(…) 异步触发成为现实。
参考资源：
1、[http://www.network-theory.co.uk/docs/pytut/Multithreading.html](http://www.network-theory.co.uk/docs/pytut/Multithreading.html)
2、[http://www.tutorialspoint.com/python/python_multithreading.htm](http://www.tutorialspoint.com/python/python_multithreading.htm)
3、[[CPyUG:88421]
 Re: 关于twisted中reactor的callInThread](http://markmail.org/message/ezocrfv26nhuwvgw?q=Twisted+callinthread+callfromthread&page=1&refer=6yjg2arjw62im7us)
4、[Generating Deferreds](http://twistedmatrix.com/documents/current/core/howto/gendefer.html)
5、[Python几种并发实现方案的性能比较](http://www.elias.cn/Develop/PyConcurrency)

