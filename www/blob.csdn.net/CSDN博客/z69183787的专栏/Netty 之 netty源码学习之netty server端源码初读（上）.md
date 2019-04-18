# Netty 之 netty源码学习之netty server端源码初读（上） - z69183787的专栏 - CSDN博客
2016年09月23日 15:39:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：470
server端是使用了Reactor模式对nio进行了一些封装，Reactor模式网上有很多资料，不赘述，了解了这个模式开始看源码
netty的版本是4.0.21.Final
**[html]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- <dependency>
- <groupId>io.netty</groupId>
- <artifactId>netty-all</artifactId>
- <version>4.0.21.Final</version>
- </dependency>
netty端server端的比较经典的代码如下：
（代码一）
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- publicvoid start(){  
-         EventLoopGroup bossGroup = new NioEventLoopGroup(1);  
-         EventLoopGroup workerGroup = new NioEventLoopGroup();  
- try {  
-             ServerBootstrap sbs = new ServerBootstrap();  
-             sbs.group(bossGroup,workerGroup);  
-             sbs.channel(NioServerSocketChannel.class);  
-             sbs.localAddress(new InetSocketAddress(port));  
-             sbs.childHandler(new ChannelInitializer<SocketChannel>() {  
- 
- protectedvoid initChannel(SocketChannel ch) throws Exception {  
-                             ch.pipeline().addLast("decoder", new StringDecoder());  
-                             ch.pipeline().addLast("encoder", new StringEncoder());  
-                             ch.pipeline().addLast(new HelloWorldServerHandler());  
-                         };  
- 
-                     }).option(ChannelOption.SO_BACKLOG, 128)     
-                     .childOption(ChannelOption.SO_KEEPALIVE, true);  
- // 绑定端口，开始接收进来的连接
-              ChannelFuture future = sbs.bind(port).sync();    
- 
-              System.out.println("Server start listen at " + port );  
-              future.channel().closeFuture().sync();  
-         } catch (Exception e) {  
-             bossGroup.shutdownGracefully();  
-             workerGroup.shutdownGracefully();  
-         }  
-     }  
step1：
这是一段普通不能再普通的代码了，先看EventLoopGroup这个类，这个类是EventLoop的一个集合，我们先简单的看下EventLoopGroup的一个具体实现NioEventLoopGroup
**[html]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- public NioEventLoopGroup() {  
-         this(0);  
- }  
- 
- public NioEventLoopGroup(int nThreads) {  
-         this(nThreads, null);  
- }  
- 
- public NioEventLoopGroup(int nThreads, ThreadFactory threadFactory) {  
-         this(nThreads, threadFactory, SelectorProvider.provider());  
- }  
- 
- public NioEventLoopGroup(  
-             int nThreads, ThreadFactory threadFactory, final SelectorProvider selectorProvider) {  
-         super(nThreads, threadFactory, selectorProvider);  
- }  
NioEventLoopGroup构造函数最后调用了它父类MultithreadEventLoopGroup的构造函数：
（代码二）
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- protected MultithreadEventLoopGroup(int nThreads, ThreadFactory threadFactory, Object... args) {  
- super(nThreads == 0? DEFAULT_EVENT_LOOP_THREADS : nThreads, threadFactory, args);  
- }  
其中DEFAULT_EVENT_LOOP_THREADS设置了这个group中的个数，如果我们没有默认传递参数，系统默认会是：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- DEFAULT_EVENT_LOOP_THREADS = Math.max(1, SystemPropertyUtil.getInt(  
- "io.netty.eventLoopThreads", Runtime.getRuntime().availableProcessors() * 2));  
一般是你CPU *2的个数，在Reactor模式中，mainReactor角色一般只需要一个线程就搞定了，subReactor角色就是那个苦逼的worker了，一般boss（mainReactor）一个就够了，subReactor就是需要多个了，所以在【代码一】中：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- EventLoopGroup bossGroup = new NioEventLoopGroup(1);  
- EventLoopGroup workerGroup = new NioEventLoopGroup();  
看名字就知道哪个EventLoopGroup对应哪一个Reactor模型中的角色了
回到上面一个话题中，我们接着【代码二】看，它接着调用它的父类MultithreadEventExecutorGroup的构造函数
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- protected MultithreadEventExecutorGroup(int nThreads, ThreadFactory threadFactory, Object... args) {  
- if (nThreads <= 0) {  
- thrownew IllegalArgumentException(String.format("nThreads: %d (expected: > 0)", nThreads));  
-         }  
- 
- if (threadFactory == null) {  
-             threadFactory = newDefaultThreadFactory();  
-         }  
- 
-         children = new SingleThreadEventExecutor[nThreads];  
- if (isPowerOfTwo(children.length)) {  
-             chooser = new PowerOfTwoEventExecutorChooser();  
-         } else {  
-             chooser = new GenericEventExecutorChooser();  
-         }  
- 
- for (int i = 0; i < nThreads; i ++) {  
- boolean success = false;  
- try {  
-                 children[i] = newChild(threadFactory, args);  
-                 success = true;  
-             } catch (Exception e) {  
- // TODO: Think about if this is a good exception type
- thrownew IllegalStateException("failed to create a child event loop", e);  
-             } finally {  
- if (!success) {  
- for (int j = 0; j < i; j ++) {  
-                         children[j].shutdownGracefully();  
-                     }  
- 
- for (int j = 0; j < i; j ++) {  
-                         EventExecutor e = children[j];  
- try {  
- while (!e.isTerminated()) {  
-                                 e.awaitTermination(Integer.MAX_VALUE, TimeUnit.SECONDS);  
-                             }  
-                         } catch (InterruptedException interrupted) {  
-                             Thread.currentThread().interrupt();  
- break;  
-                         }  
-                     }  
-                 }  
-             }  
-         }  
这是本质的方法，所有的工作基本上都在这里做了，因为前面我们很懒惰，啥都没有做，因为我们构造NioEventLoopGroup的时候，没有传递任何参数，netty给了一些默认的实现，首先newDefaultThreadFactory用来穿件一个默认的ThreadFactory，然后初始化了一个叫做children的实例变量，该children的定义是这样的
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- privatefinal EventExecutor[] children;  
EventExecutor是一个Executor，也就是一个线程执行器，换而言之，children = new SingleThreadEventExecutor[nThreads]这行代码对children进行了初始化动作，nThreads表示个数，如果我们CPU是4核的话，4*2表示初始化8个SingleThreadEventExecutor线程执行器，SingleThreadEventExecutor其实就是Reactor模型中真正工作的worker了，我们先简单的看下SingleThreadEventExecutor源码，下面列出2段SingleThreadEventExecutor的代码片段
SingleThreadEventExecutor片段一：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- privatefinal EventExecutorGroup parent;  
- privatefinal Queue<Runnable> taskQueue;  
- final Queue<ScheduledFutureTask<?>> delayedTaskQueue = new PriorityQueue<ScheduledFutureTask<?>>();  
- 
- privatefinal Thread thread;  
- privatefinal Semaphore threadLock = new Semaphore(0);  
- privatefinal Set<Runnable> shutdownHooks = new LinkedHashSet<Runnable>();  
- privatefinalboolean addTaskWakesUp;  
首先先看taskQueue这是一个Runnable的任务队列，还有一个delayedTaskQueue延迟队列，最后每一个SingleThreadEventExecutor都绑定了唯一的Thread，用这个thread去执行这些taskQueue和delayedTaskQueue中的任务
SingleThreadEventExecutor片段二：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- thread = threadFactory.newThread(new Runnable() {  
- @Override
- publicvoid run() {  
- boolean success = false;  
-                 updateLastExecutionTime();  
- try {  
-                     SingleThreadEventExecutor.this.run();  
-                     success = true;  
-                 } catch (Throwable t) {  
-                     logger.warn("Unexpected exception from an event executor: ", t);  
-                 }   
这个thread最后回去调用SingleThreadEventExecutor的run方法，这个后面再一起分析
回到上文children的初始化的代码中来，初始化好之后，开始为一个child赋值：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- children[i] = newChild(threadFactory, args);  
看newChild方法：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- @Override
- protected EventExecutor newChild(  
-             ThreadFactory threadFactory, Object... args) throws Exception {  
- returnnew NioEventLoop(this, threadFactory, (SelectorProvider) args[0]);  
- }  
- 
- NioEventLoop(NioEventLoopGroup parent, ThreadFactory threadFactory, SelectorProvider selectorProvider) {  
- super(parent, threadFactory, false);  
- if (selectorProvider == null) {  
- thrownew NullPointerException("selectorProvider");  
-         }  
-         provider = selectorProvider;  
-         selector = openSelector();  
- }  
- 
- protected SingleThreadEventLoop(EventLoopGroup parent, ThreadFactory threadFactory, boolean addTaskWakesUp) {  
- super(parent, threadFactory, addTaskWakesUp);  
-  }  
可以看出最后还是调用的SingleThreadEventExecutor的构造函数，期间做了selector的处理，我们知道NIO是基于事件驱动的，所以netty也是离不开selector，所以每一个NioEventLoop（上文描述的SingleThreadEventLoop的子类）都需要绑定一个selector，这样在netty初始化channel的时候，只需要将channel绑定selector就可以了，关于openSelector这个方法，netty也进行了大量的优化，关于openselector这个方法，大家可以参考http://budairenqin.iteye.com/blog/2215896，我这个就不重复介绍了
好了，到此为止NioEventLoopGroup的源码就已经分析完了，此时NioEventLoopGroup中的每一个NioEventLoop中绑定的thread还没有start，我们接着看
step2：
回到代码一：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- ServerBootstrap sbs = new ServerBootstrap();  
- sbs.group(bossGroup,workerGroup);  
group方法是将我们刚刚初始化好的bossGroup，workerGroup分别进行赋值（当然需要进行赋值绑定啦，否则实例化他们干啥用，实例化就是为了用，多说两句，不忘初心，bossGroup和workerGroup这两个的使命是什么，重述一遍，bossGroup和workerGroup中维护的都是维护了两个队列，一个thread，boss的thread线程是用来接收链接的然后转发给worker处理，worker列队就是一个个任务，等待worker的thread做处理，但是这两个group中的thread都没有start呢，并且selector都没有注册，哈哈，不忘初心，做人也是一样）
废话不多说，workerGroup赋值给ServerBootstrap实例的childGroup，bossGroup赋值给AbstractBootstrap的group,具体代码如下：
ServerBootstrap.java
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- privatevolatile EventLoopGroup childGroup;  
- 
- public ServerBootstrap group(EventLoopGroup parentGroup, EventLoopGroup childGroup) {  
- super.group(parentGroup);  
- if (childGroup == null) {  
- thrownew NullPointerException("childGroup");  
-         }  
- if (this.childGroup != null) {  
- thrownew IllegalStateException("childGroup set already");  
-         }  
- this.childGroup = childGroup;  
- returnthis;  
- }  
AbstractBootstrap.java
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- privatevolatile EventLoopGroup group;  
- 
- AbstractBootstrap(AbstractBootstrap<B, C> bootstrap) {  
-         group = bootstrap.group;  
-         channelFactory = bootstrap.channelFactory;  
-         handler = bootstrap.handler;  
-         localAddress = bootstrap.localAddress;  
- synchronized (bootstrap.options) {  
-             options.putAll(bootstrap.options);  
-         }  
- synchronized (bootstrap.attrs) {  
-             attrs.putAll(bootstrap.attrs);  
-         }  
- }  
接着看代码一：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- sbs.channel(NioServerSocketChannel.class);  
这个channel方法则是规定了这个ServerBootstrap的channel是啥，这里面维护了一个叫做channelFactory的东西，顾名思义，这是一个channel的工厂，我们传递的是NioServerSocketChannel这个类，那么这个channelFactory工厂创建出来的channel就是NioServerSocketChannel
去掉一些handler类，我们看：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- ChannelFuture future = sbs.bind(port).sync();    
我们看bind方法
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- public ChannelFuture bind(int inetPort) {  
- return bind(new InetSocketAddress(inetPort));  
- }  
- 
- public ChannelFuture bind(SocketAddress localAddress) {  
-         validate();  
- if (localAddress == null) {  
- thrownew NullPointerException("localAddress");  
-         }  
- return doBind(localAddress);  
- }  
- 
- private ChannelFuture doBind(final SocketAddress localAddress) {  
- final ChannelFuture regFuture = initAndRegister();  
- final Channel channel = regFuture.channel();  
- if (regFuture.cause() != null) {  
- return regFuture;  
-         }  
- 
- final ChannelPromise promise;  
- if (regFuture.isDone()) {  
-             promise = channel.newPromise();  
-             doBind0(regFuture, channel, localAddress, promise);  
-         } else {  
- // Registration future is almost always fulfilled already, but just in case it's not.
-             promise = new PendingRegistrationPromise(channel);  
-             regFuture.addListener(new ChannelFutureListener() {  
- @Override
- publicvoid operationComplete(ChannelFuture future) throws Exception {  
-                     doBind0(regFuture, channel, localAddress, promise);  
-                 }  
-             });  
-         }  
- 
- return promise;  
- }  
最后调用了AbstractBootstrap（这个我们刚才做过处理，把bossGroup赋值了AbstractBootstrap的group，也就是说这个AbstractBootstrap获取到一个thread执行器了）
言归正传，我们看doBind方法：
（代码三）
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- final ChannelFuture regFuture = initAndRegister();  
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- final ChannelFuture initAndRegister() {  
- final Channel channel = channelFactory().newChannel();  
- try {  
-             init(channel);  
-         } catch (Throwable t) {  
-             channel.unsafe().closeForcibly();  
- return channel.newFailedFuture(t);  
-         }  
- 
-         ChannelFuture regFuture = group().register(channel);  
- if (regFuture.cause() != null) {  
- if (channel.isRegistered()) {  
-                 channel.close();  
-             } else {  
-                 channel.unsafe().closeForcibly();  
-             }  
-         }  
- 
- return regFuture;  
-     }  
好了，我们看到了channelFactory了，那么它new出来的channel当然就是NioServerSocketChannel
（代码四）
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- @Override
- public T newChannel() {  
- try {  
- return clazz.newInstance();  
-             } catch (Throwable t) {  
- thrownew ChannelException("Unable to create Channel from class " + clazz, t);  
-             }  
-         }  
这边要提及的是，我们创建NioServerSocketChannel的实例的时候，会触发它的父类AbstractChannel的构造函数：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- protected AbstractChannel(Channel parent) {  
- this.parent = parent;  
-         unsafe = newUnsafe();  
-         pipeline = new DefaultChannelPipeline(this);  
- }  
unsafe = newUnsafe()这个是我们需要关注的，unsafe看起来就很熟悉，不过此unsafe并不是juc包那个CAS的unsafe，我们看看它的抽象：
源码位于io.netty.channel.Channel的Unsafe
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- interface Unsafe {  
- /**
-          * Return the {@link SocketAddress} to which is bound local or
-          * {@code null} if none.
-          */
-         SocketAddress localAddress();  
- 
- /**
-          * Return the {@link SocketAddress} to which is bound remote or
-          * {@code null} if none is bound yet.
-          */
-         SocketAddress remoteAddress();  
- 
- /**
-          * Register the {@link Channel} of the {@link ChannelPromise} with the {@link EventLoop} and notify
-          * the {@link ChannelFuture} once the registration was complete.
-          */
- void register(EventLoop eventLoop, ChannelPromise promise);  
我列出部分代码的意思就是这个unsafe其实是束缚于Channel的，但也提供了channel的一些方法，比如register(EventLoop eventLoop, ChannelPromise promise)这个方法，将channel注册到eventLoop上去，这是很关键的，我们再回归到AbstractChannel的newUnsafe方法上来，追踪源代码最终发现：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- @Override
- protected AbstractNioUnsafe newUnsafe() {  
- returnnew NioMessageUnsafe();  
- }  
好了，最终NioServerSocketChannel这个channel对应的Unsafe是NioMessageUnsafe，关于AbstractChannel的newUnsafe这只是一个插曲（下文会用到），我们还是回到【代码三】
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- final Channel channel = channelFactory().newChannel();  
- 
- init(channel);  
init的方法，很重要，这里面规范了channel的一些属性，我们看看位于ServerBootstrap中的init方法
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- @Override
- void init(Channel channel) throws Exception {  
- final Map<ChannelOption<?>, Object> options = options();  
- synchronized (options) {  
-             channel.config().setOptions(options);  
-         }  
- 
- final Map<AttributeKey<?>, Object> attrs = attrs();  
- synchronized (attrs) {  
- for (Entry<AttributeKey<?>, Object> e: attrs.entrySet()) {  
- @SuppressWarnings("unchecked")  
-                 AttributeKey<Object> key = (AttributeKey<Object>) e.getKey();  
-                 channel.attr(key).set(e.getValue());  
-             }  
-         }  
- 
-         ChannelPipeline p = channel.pipeline();  
- if (handler() != null) {  
-             p.addLast(handler());  
-         }  
- 
- final EventLoopGroup currentChildGroup = childGroup;  
- final ChannelHandler currentChildHandler = childHandler;  
- final Entry<ChannelOption<?>, Object>[] currentChildOptions;  
- final Entry<AttributeKey<?>, Object>[] currentChildAttrs;  
- synchronized (childOptions) {  
-             currentChildOptions = childOptions.entrySet().toArray(newOptionArray(childOptions.size()));  
-         }  
- synchronized (childAttrs) {  
-             currentChildAttrs = childAttrs.entrySet().toArray(newAttrArray(childAttrs.size()));  
-         }  
- 
-         p.addLast(new ChannelInitializer<Channel>() {  
- @Override
- publicvoid initChannel(Channel ch) throws Exception {  
-                 ch.pipeline().addLast(new ServerBootstrapAcceptor(  
-                         currentChildGroup, currentChildHandler, currentChildOptions, currentChildAttrs));  
-             }  
-         });  
-     }  
因为channel这边有很多option，另外我们看到了childGroup（我们刚才赋值的workerGroup），childHandler也就是handlers，也就是channel配置了options，获取到线程执行器了，最后我们需要关注的就是channel中的ChannelPipeline中增加了一个ServerBootstrapAcceptor（后面再分析，这边就是说明一下，在channelPipeLine中有了ServerBootstrapAcceptor）
回到initAndRegister这个方法中来，init完了channel，最最关键的一步就是
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- ChannelFuture regFuture = group().register(channel);  
还是不忘初心，在NIO的编程中有一个很重要的步骤，我们可以将这个步骤写出来：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- SocketChannel channel = SocketChannel.open();  
- channel.configureBlocking(false);  
- InetSocketAddress s = new InetSocketAddress("localhost", 2000);  
- channel.connect(s);  
- 
- Selector selector = Selector.open();  
- channel.register(selector, SelectionKey.OP_CONNECT | SelectionKey.OP_READ);  
这是一段很经典的纯的Java的NIO的的client代码。
这边我们看channel.register这行代码，我们需要将channel注册到selector上，并且传递自己感兴趣的事件参数，这样当有selector上有事件发生的是，就会通知注册的channel触发对应的事件
我们在分析netty的时候，我们知道在分析NioEventLoopGroup源码的时候，我们知道每一个NioEventLoopGroup都维护了一个selector，再后文中我们也分析了一个channel的创建过程，但是还没有进行selector和selector进行register
这边的group().register(channel)最后调用的是：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- channel.unsafe().register(this, promise);  
回到一圈，我们有看到了unsafe，这个unsafe上文已经分析过了，是NioMessageUnsafe，register是它父类AbstractUnsafe完成的动作
我们也注意下register的入参，this指代的是SingleThreadEventLoop这个实例（其实我要的不是这个实例，而是这个实例中的selector（位于NioEventLoop.java中））
最后的注册动作是在AbstractNioChannel.java中完成的
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- @Override
- protectedvoid doRegister() throws Exception {  
- boolean selected = false;  
- for (;;) {  
- try {  
-                 selectionKey = javaChannel().register(eventLoop().selector, 0, this);  
- return;  
-             } catch (CancelledKeyException e) {  
- if (!selected) {  
- // Force the Selector to select now as the "canceled" SelectionKey may still be
- // cached and not removed because no Select.select(..) operation was called yet.
-                     eventLoop().selectNow();  
-                     selected = true;  
-                 } else {  
- // We forced a select operation on the selector before but the SelectionKey is still cached
- // for whatever reason. JDK bug ?
- throw e;  
-                 }  
-             }  
-         }  
-     }  
这边我们要注意evetLoop就是我们刚才说的this，也就是SingleThreadEventLoop抽象类的具体实现，这边获取到了selector，javaChannel()这个方法就是
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- protected SelectableChannel javaChannel() {  
- return ch;  
-     }  
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- privatefinal SelectableChannel ch;  
估计这边大家要问这个ch是什么时候赋值的，请看【代码四】
在newInstant的时候创建NioServerSocketChannel实例的时候，进行了赋值，这边ch其实就是NioServerSocketChannel
好了，到此为止，我们基本分析了Netty的server端的启动过程，你也许会问server的thread貌似还没有start吧，其实不然，你可以仔细观察一下register0这个方法，是这么调用的：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- try {  
-                     eventLoop.execute(new OneTimeTask() {  
- @Override
- publicvoid run() {  
-                             register0(promise);  
-                         }  
-                     });  
-                 } catch (Throwable t) {  
-                     logger.warn(  
- "Force-closing a channel whose registration task was not accepted by an event loop: {}",  
-                             AbstractChannel.this, t);  
-                     closeForcibly();  
-                     closeFuture.setClosed();  
-                     safeSetFailure(promise, t);  
- }  
这个eventLoop其实就是AbstractBootstrap的group，像注册这种动作也是一种事件，所以需要Bootstrap的自带的thread去做这个动作，最终会调用SingleThreadEventExecutor的execute的方法：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
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
最后会调用startThread方法，归于自然，归于本心，调用SingleThreadEventExecutor的
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- privatevoid startThread() {  
- if (STATE_UPDATER.get(this) == ST_NOT_STARTED) {  
- if (STATE_UPDATER.compareAndSet(this, ST_NOT_STARTED, ST_STARTED)) {  
-                 delayedTaskQueue.add(new ScheduledFutureTask<Void>(  
- this, delayedTaskQueue, Executors.<Void>callable(new PurgeTask(), null),  
-                         ScheduledFutureTask.deadlineNanos(SCHEDULE_PURGE_INTERVAL), -SCHEDULE_PURGE_INTERVAL));  
-                 thread.start();  
-             }  
-         }  
-     }  
thrad的start调用的方法就是我们刚才片段二种的
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/52022956#)[copy](http://blog.csdn.net/linuu/article/details/52022956#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1788350/fork)
- SingleThreadEventExecutor.this.run();  
好了，先分析这么多吧，我们进行总结一下，分析太多，我自己也记不过来
1）Netty的server端代码一开始初始化了两个EventLoopGroup，其实就是初始化EventLoop，每一个EventLoop的具体实现就是维护了一个任务队列，一个延迟任务队列，一个thread，并且每一个EventLoop都有一个属于自己的Executor执行器，这样做的好处就是每一个唯一的thread去不停的循环调用，去执行任务队列和延迟任务队列中的task，没有了上下文的切换们还要记得每一个EventLoop还初始化了一个selector，关于selector的创建，netty做了很大的优化，使其与CPU更加亲和
（中间还忘记分析了，CPU是2的倍数的时候，Netty的优化，大家可以自己看下）
2）关于serverBootstrap的初始化，主要就是做了channel的创建，channel的执行器的绑定，option属性的配置，绑定端口，这些配置好了之后就是channel和selector的绑定，绑定的时候，顺带启动一些AbstractBootstrap中的thread，让其进行无限循环中
3）关于细节
①serverBootstrap中在channelPipeline中偷偷地加了一个ServerBootstrapAcceptor
②serverBootstrap中的boss线程对应的unsafe对象是NioMessageUnsafe实例




