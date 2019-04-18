# Netty之 netty源码学习之netty server端源码初读（下） - z69183787的专栏 - CSDN博客
2016年09月23日 15:54:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：568
上一篇简单的分析了一下NioEventLoop，ServerBootstrap等组件的创建过程做的一些操作
现在我们一起看下当SingleThreadEventExecutor.java中的thread启动后，netty做的一些最最重要的一些操作
我们接着昨天的代码看，昨天分析到register之后，调用这段代码：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52032409#)[copy](http://blog.csdn.net/linuu/article/details/52032409#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788352/fork)
- eventLoop.execute(new OneTimeTask() {  
- @Override
- publicvoid run() {  
-                             register0(promise);  
-                         }  
- });  
接着下去就是SingleThreadEventExecutor.java execute方法：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52032409#)[copy](http://blog.csdn.net/linuu/article/details/52032409#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788352/fork)
- @Override
- publicvoid execute(Runnable task) {  
- if (task == null) {  
- thrownew NullPointerException("task");  
-         }  
- 
- boolean inEventLoop = inEventLoop();  
- if (inEventLoop) {  
-             addTask(task);  
-         } else {  
-             startThread();  
-             addTask(task);  
- if (isShutdown() && removeTask(task)) {  
-                 reject();  
-             }  
-         }  
- 
- if (!addTaskWakesUp && wakesUpForTask(task)) {  
-             wakeup(inEventLoop);  
-         }  
-     }  
startThread这个方法就是调用thread.start的方法
最后就会调用我们上篇讲述的片段二的代码SingleThreadEventExecutor.this.run()：
最后追述到NioEventLoop.java中的run方法：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52032409#)[copy](http://blog.csdn.net/linuu/article/details/52032409#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788352/fork)
- @Override
- protectedvoid run() {  
- for (;;) {  
-             oldWakenUp = wakenUp.getAndSet(false);  
- try {  
- if (hasTasks()) {  
-                     selectNow();  
-                 } else {  
-                     select();  
- if (wakenUp.get()) {  
-                         selector.wakeup();  
-                     }  
-                 }  
- 
-                 cancelledKeys = 0;  
-                 needsToSelectAgain = false;  
- finalint ioRatio = this.ioRatio;  
- if (ioRatio == 100) {  
-                     processSelectedKeys();  
-                     runAllTasks();  
-                 } else {  
- finallong ioStartTime = System.nanoTime();  
- 
-                     processSelectedKeys();  
- 
- finallong ioTime = System.nanoTime() - ioStartTime;  
-                     runAllTasks(ioTime * (100 - ioRatio) / ioRatio);  
-                 }  
- 
- if (isShuttingDown()) {  
-                     closeAll();  
- if (confirmShutdown()) {  
- break;  
-                     }  
-                 }  
-             } catch (Throwable t) {  
-                 logger.warn("Unexpected exception in the selector loop.", t);  
- 
- // Prevent possible consecutive immediate failures that lead to
- // excessive CPU consumption.
- try {  
-                     Thread.sleep(1000);  
-                 } catch (InterruptedException e) {  
- // Ignore.
-                 }  
-             }  
-         }  
-     }  
首先这个方法是无限自循环，没有shutdown之前是不会停止的，讲述这个方法之前，先再理清一些概念，首先是这个run方法的执行线程是thread，也就是之前讲述nioEventLoopGroup时，说netty为每一个NioEventLoop绑定了一个自己的线程，它去执行队列和延迟队列中的task任务，除了执行这两种任务之外，还执行一些IO操作，何为IO操作，例如OP_ACCEPT、OP_CONNECT、OP_READ、OP_WRITE，相反那些队列中的task则为非IO操作，例如bind，register，channelRead，channelActive等等
好的，理清这个大前提之后，我们接着看这个方法
先进行判断，查看hasTask方法，这个方法主要查看队列里面是否有未执行的task，如果有任务，就调用selectNow方法，立即返回，不阻塞，这边很好理解啊，如果队列中有任务当然立即返回，否则一直阻塞在那边，你等别人请你吃午饭啊，如果hasTask方法返回false的时候，说明队列中没有任务，则调用select()方法，这边需要解释一下，如果阻塞调用的时候，就不怕在阻塞的期间队列中有新的task吗，这边下面的分析的时候会做解释，先放在这边，我们接着把这个run方法分析完整
接下来我们会根据我们配置的ioRatio的值，分配IO和非IO任务执行的时间比例，默认情况下，ioRatio为50，走else分支，processSelectedKeys这是是执行IO操作，runAllTasks是执行队列任务，整个run方法就是做这么多事情，去查看自己独自持有的selector上不停的select或者selectNow，去查看是否有IO事件发生，与此同时，如果有队列任务，则顺便执行了
就比如我们上一篇做的那个channel注册selector的那个register方法一样，就是调用的 runAllTasks(ioTime * (100 - ioRatio) / ioRatio)的方法，所以绑定channel就是一个非IO操作
到此为止，我们简单的一起分析了run方法的主要流程，我们还没有对processSelectedKeys()这个处理IO的方法进行分析，并且还有一个疑问就是我们刚才说明的那个问题，为什么当前时刻没有task就可以调用阻塞方法select方法而不当心阻塞的过程中有task加入到队列的问题了
接下来，我们将启动一个client来连接server端，通过连接，我们真实场景下进行debug，简单地分析一下Netty的启动过程，首先在debug之前，我们先说明一下，如果Client端连接Server并发送一个信息的话，我们需要注意的事情
1）因为Netty的Boss/worker的代码是复用的，我们在debug的时候，需要注意上下文,要区分此时的线程是boss的thread还是worker的thread
2）client端连接server端并发送信息，这会触发一些什么事件，我们也需要注意，上文讲述过，任务分两种IO任务和非IO任务(我们站在Server端的角度)
IO任务：OP_ACCEPT，OP_READ
非IO任务：channelActive和channelRead方法
当client启动后，我们进入在server端进入debug模式
进入debug模式后，我们聚焦NioEventLoop的run方法
![](https://img-blog.csdn.net/20160726151356673?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
processSelectedKeys方法是处理在selector上的IO事件，进入processSelectedKeys方法接着看
![](https://img-blog.csdn.net/20160726151600627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以看到在select方法操作之后，seletedKeys上的确有事件发生，如上图所示，接着debug：
![](https://img-blog.csdn.net/20160726151703041?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
因为我们目前debug的是boss线程绑定的NioEventLoop上的thread 运行的run方法，而boss线程的selector绑定的channel是NioServerSocketChannel，也就是我们上一篇文章介绍的那个register方法，因为绑定的selector上有事件发生（OP_ACCEPTOR事件）接着看processSelectedKeysOptimized这个方法中的processSelectedKey子方法：
![](https://img-blog.csdn.net/20160726152047764?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们知道NioServerSocketChannel对应的unsafe对象是NioMessageUnsafe对象，并且这边k.readyOps()的值是16也就是SelectionKey.OP_ACCEPT的值，所以我们需要处理客户端的连接操作，processSelectedKey的方法使用unsafe.read()的方法进行处理，也就是这边的NioMessageUnsafe来处理，我们进入NioMessageUnsafe的read方法
![](https://img-blog.csdn.net/20160726152450505?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在这个方法中，我们会获取到NioSeverSocketChannel上的管道，我们上文说过，netty在管道中偷偷的塞入了一个ServerBootstrapAcceptor，看这个类的名字就知道，是用来处理ServerBootstrap接收其他链接用的一个处理器，获取到ServerBootstrapAcceptor之后，我们接着看doReadMessages这个方法
![](https://img-blog.csdn.net/20160726153421902?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以看到此时才进行accpt操作（不知道我理解的对不对，BACKLOG的值就是建立好的连接和为建立好的连接之和，建立好的连接和未建立好的链接的区分点就是是否accept），言归正传，下面将建立好的Channel封装成NioSocketChannel，这边要注意的是每一个channel都有一个Unsafe对象，我们可以看新建的NioSocketChannel的Unsafe对象与NioServerSocketChannel的NioMessageUnsafe对象有何不同，我们进入new NioSocketChannel（this,ch）的方法内部，最后debug到：
![](https://img-blog.csdn.net/20160726153919789?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以看到NioSocketChannel对应的NioByteUnsafe，同时也没有在管道中偷偷塞入类似ServerBootstrapAcceptor这样的处理器
![](https://img-blog.csdn.net/20160726154111276?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以看到defaultChannelPipeline目前还是空的，接着回来看doReadMessages方法，因为这个方法是在无限for循环中的，当下次accept没有获取到新的channel的时候，就会退出循环，接着看
![](https://img-blog.csdn.net/20160726154359750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
pipeline就是我们刚才说的ServerBootstrapAcceptor这个对象，我们看看ServerBootstrapAcceptor对新的NioSocketChannel做了什么，其实想想我们应该知道做什么，我们已经获取到了新的channel，我们只要让其绑定8个worker中的任意worker，然后让channel绑定selector，再顺便启动一下worker的thread，boss线程要干的事情就算做完了，而且我们在分析ServerBootstrapAcceptor的时候，我们应该想起，ServerBootstrapAcceptor的构造函数里面就有workGroup，这样一想感觉顺理成章，我们接着看
![](https://img-blog.csdn.net/20160726154925429?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
debug下看的确最后调用的是ServerBootstrapAcceptor这个handler。接着看
![](https://img-blog.csdn.net/20160726155001618?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
对channel做了一些属性的配置之后，果然走到了最后一步register，其实上一节我们分析了register的方法的。我们这边再一起分析一下，温故而知新嘛
![](https://img-blog.csdn.net/20160726155116800?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
因为8个worker要均匀的分配，所以我们会看到最后调用的next方法是：
![](https://img-blog.csdn.net/20160726155200167?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
最后也就是调用了register方法，与上文没有差别，且顺便借用register之手，启动了其中一个worker的线程
好了，到此为止，boss线程的NioEventLoop分析就到此为止了，我们接着看worker线程的NioEventLoop的run方法
![](https://img-blog.csdn.net/20160726155414123?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以看到此时的AbstractNioChannel已经变成了我们刚才封装初始化绑定的NioSocketChannel，此时为啥会触发这个方法呢，因为客户端发送了一个信息到server端了，
这边的readyOps的值是1，也就是OP_READ,unsafe对象也是我们刚才说的NioByteUnsafe
![](https://img-blog.csdn.net/20160726155642908?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160726155702739?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
pipeline中的也是我们自定义的handler
![](https://img-blog.csdn.net/20160726155739068?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
好了，server端的启动流程基本分析结束了，boss的线程，worker的启动，都简单的说明了一些，关于如何处理client端的信息，我们下次在一起分析一下~
关于那个为什么队列中没有任务的时候，就可以阻塞操作select的原因始因为我觉得（不一定对）绝大多数的非IO操作也是因为IO事件产生的，也就是说如果当前时刻没有
IO事件的时候，Netty不会无脑会产生一些非IO的任务的，例如channelActive，bind等等的操作
我们总结一下，Netty的server端流程其实是很清晰的
1）先初始化好boss和worker的NioEventLoopGroup，并初始化好Group中的每一个NioEventLoop，为每一个NioEventLoop都穿件一个selector对象
2）Netty会在bind的动作上，去让boss的NioserverSocketChannel去绑定selector，并启动与boss捆绑在一起的thread，进入无尽的循环
3）在这个生命不息，循环不止的方法中，主要做了两件事情，1是去select，不管是selectNow()方法还是select()方法，其主要目的就是去查看boss关注的selector是否有事件发生
4）当有事件发生的时候，一般就是因为有client链接，如果有链接，boss线程就需要做的事情就是初始化封装好新来的SocketChannel
5）封装好的NioSocketChannel也会有自己的Unsafe对象，这个对象是用来做一些其他的操作的，例如读操作，这与boss的Unsafe对象不一样，boss的Unsafe对象是NioMessageUnsafe是用来进行绑定channel
6）NioSocketChannel用Boss线程管道中的ServerBootstrapAcceptor对象绑定确定属于自己的worker线程之后，绑定好worker线程的selector之后就开始调用自己的run方法
用来监听selector上的IO事件
7）要说明白的一点就是一个worker处理的channel在多链接的场景下，一个worker会处理不止一个SocketChannel
