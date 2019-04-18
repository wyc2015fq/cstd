# Netty4学习笔记-- NioEventLoopGroup NioEventLoop - z69183787的专栏 - CSDN博客
2016年09月22日 15:42:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3472
## NioEventLoopGroup继承层次结构图
Netty4的[NioEventLoopGroup](http://netty.io/4.0/api/io/netty/channel/nio/NioEventLoopGroup.html)继承结构很复杂，为了理解它如何工作，我画了一张类似**UML**的图。但是这张图也很巨大，所以我做了以下处理：
- 将位于不同**包**中的类或接口用不同的**颜色**加以区分
![](https://img-blog.csdn.net/20131228172201484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 接口名后面的括号里标注了大写字母**I（Interface）**，抽象类名的后面标注了大写字母**A（Abstract）**
![](https://img-blog.csdn.net/20131228172246187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 每个类或接口，只列出了我认为最重要的**一个**方法
![](https://img-blog.csdn.net/20131228172324578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## EventExecutorGroup
首先从[EventExecutorGroup](http://netty.io/4.0/api/io/netty/util/concurrent/EventExecutorGroup.html)开始，EventExecutorGroup是NioEventLoopGroup最上层的接口，再往上就是Java的地盘了：
![](https://img-blog.csdn.net/20131228175008171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
从上面的图可以看出3点：
- EventExecutorGroup接口继承了**java.util.concurrent.ScheduledExecutorService**接口，因此它可以调度执行task
- EventExecutorGroup内部管理了n个[EventExecutor](http://netty.io/4.0/api/io/netty/util/concurrent/EventExecutor.html)，**next()**方法返回其中的一个
- EventExecutor也是EventExecutorGroup（的子类）
想象一下，EventExecutorGroup就像一个BOSS，每当有活儿的时候，就派一个小弟（EventExecutor）去干：
![](https://img-blog.csdn.net/20131228180432781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## AbstractEventExecutorGroup
[AbstractEventExecutorGroup](http://netty.io/4.0/api/io/netty/util/concurrent/AbstractEventExecutorGroup.html)抽象类实现了ScheduledExecutorService接口，但大部分实现都只是调用**next()**拿到一个EventExecutor，然后调用EventExecutor的相应方法，例如submit()方法：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17634683#)[copy](http://blog.csdn.net/zxhoo/article/details/17634683#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/134623/fork)
- publicabstractclass AbstractEventExecutorGroup implements EventExecutorGroup {  
- 
- @Override
- public Future<?> submit(Runnable task) {  
- return next().submit(task);  
-     }  
-     ...  
- }  
![](https://img-blog.csdn.net/20131228181347156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## MultithreadEventExecutorGroup
从类名来看，[MultithreadEventExecutorGroup](http://netty.io/4.0/api/io/netty/util/concurrent/MultithreadEventExecutorGroup.html)有并行（多线程）分配小弟去干活的能力，换句话说，每个小弟都工作在自己的线程中。从代码中也能看出这一点：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17634683#)[copy](http://blog.csdn.net/zxhoo/article/details/17634683#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/134623/fork)
- publicabstractclass MultithreadEventExecutorGroup extends AbstractEventExecutorGroup {  
- 
- privatefinal EventExecutor[] children;  
- privatefinal AtomicInteger childIndex = new AtomicInteger();  
- privatefinal AtomicInteger terminatedChildren = new AtomicInteger();  
- privatefinal Promise<?> terminationFuture = new DefaultPromise(GlobalEventExecutor.INSTANCE);  
- 
- protected MultithreadEventExecutorGroup(int nThreads, ThreadFactory threadFactory, Object... args) {  
-         ...  
-         children = new SingleThreadEventExecutor[nThreads];  
-         ...  
-     }  
- 
-     ...  
- }  
可以看出，MultithreadEventExecutorGroup的每一个小弟都是一个SingleThreadEventExecutor，而且小弟的数量在构造的时候就确定了，如下图所示：
![](https://img-blog.csdn.net/20131228183050281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
从MultithreadEventExecutorGroup的**next()**方法可以看到，这个BOSS的小弟分配逻辑相当简单，无非就是轮流使唤：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17634683#)[copy](http://blog.csdn.net/zxhoo/article/details/17634683#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/134623/fork)
- @Override
- public EventExecutor next() {  
- return children[Math.abs(childIndex.getAndIncrement() % children.length)];  
- }  
注：这里调用了Math.abs()方法以防止childIndex溢出
## MultithreadEventLoopGroup
[MultithreadEventLoopGroup](http://netty.io/4.0/api/io/netty/channel/MultithreadEventLoopGroup.html)类实现了EventLoopGroup接口和register()方法：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17634683#)[copy](http://blog.csdn.net/zxhoo/article/details/17634683#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/134623/fork)
- publicabstractclass MultithreadEventLoopGroup extends MultithreadEventExecutorGroup implements EventLoopGroup {  
-     ...  
- 
- @Override
- public ChannelFuture register(Channel channel) {  
- return next().register(channel);  
-     }  
- 
- @Override
- public ChannelFuture register(Channel channel, ChannelPromise promise) {  
- return next().register(channel, promise);  
-     }  
- }  
## NioEventLoopGroup
最后是[NioEventLoopGroup](http://netty.io/4.0/api/io/netty/channel/nio/NioEventLoopGroup.html)，这个BOSS的每一个小弟都是一个NioEventLoop。下面是完整的继承层次结构图：
![](https://img-blog.csdn.net/20131230154256171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 结论
- NioEventLoopGroup实际上就是个**线程池**
- NioEventLoopGroup在后台启动了n个NioEventLoop来处理Channel事件
- 每一个NioEventLoop负责处理m个Channel
- NioEventLoopGroup从NioEventLoop数组里挨个取出NioEventLoop来处理Channel
![](https://img-blog.csdn.net/20131230154334203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
[前一篇文章](http://blog.csdn.net/zxhoo/article/details/17634683)画了一张巨大的类图，但是只重点分析了[NioEventLoopGroup](http://netty.io/4.0/api/io/netty/channel/nio/NioEventLoopGroup.html)的继承层次，这篇文章来看看[NioEventLoop](http://netty.io/4.0/api/io/netty/channel/nio/NioEventLoop.html)的继承层次。
## 从Executor接口说起
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17709765#)[copy](http://blog.csdn.net/zxhoo/article/details/17709765#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/136321/fork)
- publicinterface Executor {  
- void execute(Runnable command);  
- }  
Executor接口只有一个方法，用来执行一个命令。但是什么时候执行，在哪个线程里执行，得看具体的实现。
## ExecutorService接口
ExecutorService接口扩展了Executor接口，添加了两组方法，一组用来终止Executor的执行：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17709765#)[copy](http://blog.csdn.net/zxhoo/article/details/17709765#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/136321/fork)
- void shutdown();  
- List<Runnable> shutdownNow();  
- boolean isShutdown();  
- boolean isTerminated();  
- boolean awaitTermination(long timeout, TimeUnit unit) throws InterruptedException;  
另一组扩展Executor的执行能力，让它可以提交和执行Callable任务：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17709765#)[copy](http://blog.csdn.net/zxhoo/article/details/17709765#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/136321/fork)
- <T> Future<T> submit(Callable<T> task);  
- <T> Future<T> submit(Runnable task, T result);  
- Future<?> submit(Runnable task);  
- <T> List<Future<T>> invokeAll(Collection<? extends Callable<T>> tasks) throws InterruptedException;  
- <T> List<Future<T>> invokeAll(Collection<? extends Callable<T>> tasks, long timeout, TimeUnit unit) throws InterruptedException;  
- <T> T invokeAny(Collection<? extends Callable<T>> tasks) throws InterruptedException, ExecutionException;  
- <T> T invokeAny(Collection<? extends Callable<T>> tasks, long timeout, TimeUnit unit) throws InterruptedException, ExecutionException, TimeoutException;  
## ScheduledExecutorService接口
ScheduledExecutorService接口扩展ExecutorService接口，添加了调度能力：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17709765#)[copy](http://blog.csdn.net/zxhoo/article/details/17709765#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/136321/fork)
- public ScheduledFuture<?> schedule(Runnable command, long delay, TimeUnit unit);  
- public <V> ScheduledFuture<V> schedule(Callable<V> callable, long delay, TimeUnit unit);  
- public ScheduledFuture<?> scheduleAtFixedRate(Runnable command, long initialDelay, long period, TimeUnit unit);  
- public ScheduledFuture<?> scheduleWithFixedDelay(Runnable command, long initialDelay, long delay, TimeUnit unit);  
## AbstractExecutorService抽象类
AbstractExecutorService实现了ExecutorService接口，实现了ExecutorService接口里定义的任务执行方法。
## EventExecutorGroup接口
前面几个是java.util.concurrent包里头的类或接口，从[EventExecutorGroup](http://netty.io/4.0/api/io/netty/util/concurrent/EventExecutorGroup.html)开始，进入Netty的世界。
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17709765#)[copy](http://blog.csdn.net/zxhoo/article/details/17709765#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/136321/fork)
- publicinterface EventExecutorGroup extends ScheduledExecutorService, Iterable<EventExecutor> {  
- boolean isShuttingDown();  
-     Future<?> shutdownGracefully();  
-     Future<?> shutdownGracefully(long quietPeriod, long timeout, TimeUnit unit);  
-     Future<?> terminationFuture();  
- 
-     EventExecutor next();  
- }  
EventExecutorGroup把从ExecutorService接口里继承来的shutdown()和shutdownNow()方法给废弃（@Deprecated）了，然后提供了shutdownGracefully()方法。shutdownGracefully()方法我没有彻底理解，从Javadoc来看，shutdownGracefully()方法通知EventExecutorGroup准备结束，然后进入一个“安静期（the
 quiet period）”，如果这个期间内有任务提交进来，这个任务仍然会被接收，然后安静期重新开始。只有安静期顺利度过，EventExecutorGroup才会真正的关闭自己。
EventExecutorGroup还实现了**Iterable**接口，并其提供了**next()**方法返回其中的一个[EventExecutor](http://netty.io/4.0/api/io/netty/util/concurrent/EventExecutor.html)。这样来看，EventExecutorGroup这个类名起的真是太恰当了：它逻辑上实际上就是一组EventExecutor：
![](https://img-blog.csdn.net/20131231112516718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## EventExecutor接口
EventExecutor给EventExecutorGroup添加了下面这些方法：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17709765#)[copy](http://blog.csdn.net/zxhoo/article/details/17709765#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/136321/fork)
- publicinterface EventExecutor extends EventExecutorGroup {  
-     EventExecutorGroup parent();  
- 
- boolean inEventLoop();  
- boolean inEventLoop(Thread thread);  
- 
-     <V> Promise<V> newPromise();  
-     <V> ProgressivePromise<V> newProgressivePromise();  
-     <V> Future<V> newSucceededFuture(V result);  
-     <V> Future<V> newFailedFuture(Throwable cause);  
- }  
parent()方法比较好理解，返回自己所属的group。EventExecutor需要一个线程来执行任务，inEventLoop()方法用来判断某个线程是不是EventExecutor所使用的线程。但剩下的四个方法不太好理解，暂时先不管了。
## EventLoopGroup接口
[EventLoopGroup](http://netty.io/4.0/api/io/netty/channel/EventLoopGroup.html)接口比较简单，只定义了两个方法，用来注册Channel：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17709765#)[copy](http://blog.csdn.net/zxhoo/article/details/17709765#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/136321/fork)
- publicinterface EventLoopGroup extends EventExecutorGroup {  
-     ChannelFuture register(Channel channel);  
-     ChannelFuture register(Channel channel, ChannelPromise promise);  
- }  
## AbstractEventExecutor
[AbstractEventExecutor](http://netty.io/4.0/api/io/netty/util/concurrent/AbstractEventExecutor.html)为EventExecutor接口里的一些方法提供了默认实现，这些默认实现大概可以分为下面几组：
- 无参数版方法默认实现（调用有参数版）
- inEventLoop()
- shutdownGracefully()
- 转发给AbstractExecutorService
- submit(Runnable task)
- submit(Runnable task, T result)
- submit(Callable<T> task)
- 直接抛出UnsupportedOperationException
- schedule(Callable<V> callable, long delay, TimeUnit unit)
- scheduleAtFixedRate(Runnable command, long initialDelay, long period, TimeUnit unit)
- scheduleWithFixedDelay(Runnable command, long initialDelay, long delay, TimeUnit unit)
- 覆盖（override）AbstractExecutorService中的方法实现
- newTaskFor(Runnable runnable, T value)
- newTaskFor(Callable<T> callable)
最后**next()**方法只是返回自己：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17709765#)[copy](http://blog.csdn.net/zxhoo/article/details/17709765#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/136321/fork)
- @Override
- public EventExecutor next() {  
- returnthis;  
- }  
下面是类图：
![](https://img-blog.csdn.net/20131231130817781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## SingleThreadEventExecutor
再往下看是[SingleThreadEventExecutor](http://netty.io/4.0/api/io/netty/util/concurrent/SingleThreadEventExecutor.html)，从名字上看，SingleThreadEventExecutor肯定是在单线程里执行task。SingleThreadEventExecutor类的代码有800多行，但是只要大概看一下就可以了解它是如何工作的：
- 它内部有两个task队列，**taskQueue**和**delayedTaskQueue**，普通任务会进入taskQueue，调度任务进入delayedTaskQueue
- 默认的taskQueue是一个**LinkedBlockingQueue**
- 实现了很多**任务执行**相关的方法，但核心的**run()**方法留给子类来实现，子类需要在run()方法里取出并执行task
下面是类图：
![](https://img-blog.csdn.net/20131231145007812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## SingleThreadEventLoop
[SingleThreadEventLoop](http://netty.io/4.0/api/io/netty/channel/SingleThreadEventLoop.html)比较简单，下面是它的全部代码：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17709765#)[copy](http://blog.csdn.net/zxhoo/article/details/17709765#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/136321/fork)
- publicabstractclass SingleThreadEventLoop extends SingleThreadEventExecutor implements EventLoop {  
- 
- protected SingleThreadEventLoop(EventLoopGroup parent, ThreadFactory threadFactory, boolean addTaskWakesUp) {  
- super(parent, threadFactory, addTaskWakesUp);  
-     }  
- 
- @Override
- public EventLoopGroup parent() {  
- return (EventLoopGroup) super.parent();  
-     }  
- 
- @Override
- public EventLoop next() {  
- return (EventLoop) super.next();  
-     }  
- 
- @Override
- public ChannelFuture register(Channel channel) {  
- return register(channel, channel.newPromise());  
-     }  
- 
- @Override
- public ChannelFuture register(final Channel channel, final ChannelPromise promise) {  
- if (channel == null) {  
- thrownew NullPointerException("channel");  
-         }  
- if (promise == null) {  
- thrownew NullPointerException("promise");  
-         }  
- 
-         channel.unsafe().register(this, promise);  
- return promise;  
-     }  
- }  
## NioEventLoop
最后终于轮到NioEventLoop了，不过遗憾的是，NioEventLoop太复杂了，我还没有完全看明白。下面是完整的继承层次结构图：
![](https://img-blog.csdn.net/20131231150114656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## NioEventLoop的run()方法
最后我想简单分析一下NioEventLoop的run()方法，下面是run()方法的部分代码：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17709765#)[copy](http://blog.csdn.net/zxhoo/article/details/17709765#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/136321/fork)
- @Override
- protectedvoid run() {  
- for (;;) {  
- // process io events
-         ...  
- finallong ioTime = System.nanoTime() - ioStartTime;  
- finalint ioRatio = this.ioRatio;  
-         runAllTasks(ioTime * (100 - ioRatio) / ioRatio);  
-         ...  
-     }  
- }  
也就是说，NioEventLoop在单线程里同时处理IO事件和其他任务，NioEventLoop尽量（但不能保证）按照给定的比率（默认为50%）来分配花在这两种事情上的时间。换句话说，我们不应该在NioEventLoop里执行耗时的操作（比如数据库操作），这样会卡死NioEventLoop，降低程序的响应性。

