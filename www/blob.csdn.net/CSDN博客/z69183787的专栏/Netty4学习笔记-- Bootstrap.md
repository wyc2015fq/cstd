# Netty4学习笔记-- Bootstrap - z69183787的专栏 - CSDN博客
2016年09月22日 15:33:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1805
Netty4的代码比我想象的要复杂的多，不过Netty4很好的将这种复杂性隐藏了起来，暴露出来的，是一个相对容易使用的接口。Bootstrap就是Netty试图隐藏这种复杂性的一个例子。
## bootstrap包
bootstrap包是Netty4代码里最简单的一个包，总共只有4个类：
![](https://img-blog.csdn.net/20131219163029921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## Bootstrap继承结构
AbstractBootstrap是抽象类，有两个具体的实现，Bootstrap和ServerBootstrap：
![](https://img-blog.csdn.net/20131219164500031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## Bootstrap例子
Netty4自带的例子里有一个EchoClient，看看它是如何使用Bootstrap启动一个客户端程序的：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17419229#)[copy](http://blog.csdn.net/zxhoo/article/details/17419229#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/119661/fork)
- publicclass EchoClient {  
- 
- privatefinal String host;  
- privatefinalint port;  
- privatefinalint firstMessageSize;  
- 
- public EchoClient(String host, int port, int firstMessageSize) {  
- this.host = host;  
- this.port = port;  
- this.firstMessageSize = firstMessageSize;  
-     }  
- 
- publicvoid run() throws Exception {  
- // Configure the client.
-         EventLoopGroup group = new NioEventLoopGroup();  
- try {  
-             Bootstrap b = new Bootstrap();  
-             b.group(group)  
-              .channel(NioSocketChannel.class)  
-              .option(ChannelOption.TCP_NODELAY, true)  
-              .handler(new ChannelInitializer<SocketChannel>() {  
- @Override
- publicvoid initChannel(SocketChannel ch) throws Exception {  
-                      ch.pipeline().addLast(  
- //new LoggingHandler(LogLevel.INFO),
- new EchoClientHandler(firstMessageSize));  
-                  }  
-              });  
- 
- // Start the client.
-             ChannelFuture f = b.connect(host, port).sync();  
- 
- // Wait until the connection is closed.
-             f.channel().closeFuture().sync();  
-         } finally {  
- // Shut down the event loop to terminate all threads.
-             group.shutdownGracefully();  
-         }  
-     }  
- 
- publicstaticvoid main(String[] args) throws Exception {  
- // ...
-     }  
- }  
## Builder模式？
看上面的例子，Bootstrap的使用很像[Builder模式](http://en.wikipedia.org/wiki/Builder_pattern)，Bootstrap就是Builder，EventLoopGroup、Channel和Handler等是各种Part。稍有不同的是，准备好各种Part后，并不是直接build出一个Product来，而是直接通过connect()方法使用这个Product。
## AbstractBootstrap
为了弄清楚Bootstrap如何工作，我们先从AbstractBootstrap入手：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17419229#)[copy](http://blog.csdn.net/zxhoo/article/details/17419229#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/119661/fork)
- publicabstractclass AbstractBootstrap<B extends AbstractBootstrap<B, C>, C extends Channel> implements Cloneable {  
- 
- privatevolatile EventLoopGroup group;  
- privatevolatile ChannelFactory<? extends C> channelFactory;  
- privatevolatile SocketAddress localAddress;  
- privatefinal Map<ChannelOption<?>, Object> options = new LinkedHashMap<ChannelOption<?>, Object>();  
- privatefinal Map<AttributeKey<?>, Object> attrs = new LinkedHashMap<AttributeKey<?>, Object>();  
- privatevolatile ChannelHandler handler;  
- // ...
- }  
可以看到，AbstractBootstrap这个抽象Builder一共需要有6个Part，如下图所示：
![](https://img-blog.csdn.net/20131219173610421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 设置各个Part
AbstractBootstrap有一组方法用来设置各个Part，例如下面这些：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17419229#)[copy](http://blog.csdn.net/zxhoo/article/details/17419229#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/119661/fork)
- public B group(EventLoopGroup group)  
- public B channel(Class<? extends C> channelClass)  
- public B channelFactory(ChannelFactory<? extends C> channelFactory)  
- public B localAddress(SocketAddress localAddress)  
- public <T> B option(ChannelOption<T> option, T value)  
- public <T> B attr(AttributeKey<T> key, T value)  
- public B handler(ChannelHandler handler)  
还有一组对应方法获得各个Part，如下：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17419229#)[copy](http://blog.csdn.net/zxhoo/article/details/17419229#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/119661/fork)
- final SocketAddress localAddress()  
- final ChannelFactory<? extends C> channelFactory()  
- final ChannelHandler handler()  
- publicfinal EventLoopGroup group()  
- final Map<ChannelOption<?>, Object> options()  
- final Map<AttributeKey<?>, Object> attrs()  
## ChannelFactory
AbstractBootstrap通过ChannelFactory创建Channel实例，channel(channelClass)方法看起来好像是设置了一个Channel，但实际上只是设置了默认的ChannelFactory实现：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17419229#)[copy](http://blog.csdn.net/zxhoo/article/details/17419229#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/119661/fork)
- public B channel(Class<? extends C> channelClass) {  
- if (channelClass == null) {  
- thrownew NullPointerException("channelClass");  
-     }  
- return channelFactory(new BootstrapChannelFactory<C>(channelClass));  
- }  
默认的ChannelFactory实现使用反射创建Channel实例：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17419229#)[copy](http://blog.csdn.net/zxhoo/article/details/17419229#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/119661/fork)
- privatestaticfinalclass BootstrapChannelFactory<T extends Channel> implements ChannelFactory<T> {  
- privatefinal Class<? extends T> clazz;  
- 
-     BootstrapChannelFactory(Class<? extends T> clazz) {  
- this.clazz = clazz;  
-     }  
- 
- @Override
- public T newChannel() {  
- try {  
- return clazz.newInstance();  
-         } catch (Throwable t) {  
- thrownew ChannelException("Unable to create Channel from class " + clazz, t);  
-         }  
-     }  
- }  
## Bootstrap.connect()
再来看Bootstrap类的connect()方法：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17419229#)[copy](http://blog.csdn.net/zxhoo/article/details/17419229#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/119661/fork)
- public ChannelFuture connect(SocketAddress remoteAddress, SocketAddress localAddress) {  
- if (remoteAddress == null) {  
- thrownew NullPointerException("remoteAddress");  
-     }  
-     validate();  
- return doConnect(remoteAddress, localAddress);  
- }  
connect()方法调用validate()方法看各个Part是否准备就绪，然后调用doConnect()方法：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17419229#)[copy](http://blog.csdn.net/zxhoo/article/details/17419229#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/119661/fork)
- private ChannelFuture doConnect(final SocketAddress remoteAddress, final SocketAddress localAddress) {  
- final ChannelFuture regFuture = initAndRegister();  
- final Channel channel = regFuture.channel();  
- if (regFuture.cause() != null) {  
- return regFuture;  
-     }  
- 
- final ChannelPromise promise = channel.newPromise();  
- if (regFuture.isDone()) {  
-         doConnect0(regFuture, channel, remoteAddress, localAddress, promise);  
-     } else {  
-         regFuture.addListener(new ChannelFutureListener() {  
- @Override
- publicvoid operationComplete(ChannelFuture future) throws Exception {  
-                 doConnect0(regFuture, channel, remoteAddress, localAddress, promise);  
-             }  
-         });  
-     }  
- 
- return promise;  
- }  
doConnect()方法首先调用了initAndRegister()方法，然后又调用了doConnect0()方法，方法调用示意图如下：
![](https://img-blog.csdn.net/20131219184137781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## AbstractBootstrap.initAndRegister()
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17419229#)[copy](http://blog.csdn.net/zxhoo/article/details/17419229#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/119661/fork)
- final ChannelFuture initAndRegister() {  
- final Channel channel = channelFactory().newChannel();  
- try {  
-         init(channel);  
-     } catch (Throwable t) {  
-         channel.unsafe().closeForcibly();  
- return channel.newFailedFuture(t);  
-     }  
- 
-     ChannelPromise regPromise = channel.newPromise();  
-     group().register(channel, regPromise);  
- // ...
- }  
initAndRegister()方法用ChannelFactory创建了一个Channel的实例，然后调用init()方法初始化Channel，最后将Channel注册到EventLoopGroup上：
![](https://img-blog.csdn.net/20131219184919484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
而Channel在实例化的时候已经自动关联了Pipeline，这点从AbstractChannel的构造函数可以看出：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17419229#)[copy](http://blog.csdn.net/zxhoo/article/details/17419229#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/119661/fork)
- protected AbstractChannel(Channel parent) {  
- this.parent = parent;  
-     unsafe = newUnsafe();  
-     pipeline = new DefaultChannelPipeline(this);  
- }  
![](https://img-blog.csdn.net/20131219185819078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## Bootstrap.init()方法
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17419229#)[copy](http://blog.csdn.net/zxhoo/article/details/17419229#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/119661/fork)
- void init(Channel channel) throws Exception {  
-     ChannelPipeline p = channel.pipeline();  
-     p.addLast(handler());  
- // ...
- }  
Bootstrap.init()方法把Handler添加到了Pipeline的末尾，到这里，Channel就准备就绪了：
![](https://img-blog.csdn.net/20131219190204406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 继续Bootstrap.doConnect()
initAndRegister()方法结束之后，doConnect()方法紧接着调用了doConnect0()方法，doConnect0()方法继而调用了Channel.connect()方法，这样Channel就接通服务器，可以收发消息了！
![](https://img-blog.csdn.net/20131219191910046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这篇文章接着[上一篇](http://blog.csdn.net/zxhoo/article/details/17419229)，分析一下Netty4的ServerBootstrp是如何工作的。
## EchoServer
先看看Netty自带的EchoServer例子：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17532857#)[copy](http://blog.csdn.net/zxhoo/article/details/17532857#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/126560/fork)
- /**
-  * Echoes back any received data from a client.
-  */
- publicclass EchoServer {  
- 
- privatefinalint port;  
- 
- public EchoServer(int port) {  
- this.port = port;  
-     }  
- 
- publicvoid run() throws Exception {  
- // Configure the server.
-         EventLoopGroup bossGroup = new NioEventLoopGroup();  
-         EventLoopGroup workerGroup = new NioEventLoopGroup();  
- try {  
-             ServerBootstrap b = new ServerBootstrap();  
-             b.group(bossGroup, workerGroup)  
-              .channel(NioServerSocketChannel.class)  
-              .option(ChannelOption.SO_BACKLOG, 100)  
-              .handler(new LoggingHandler(LogLevel.INFO))  
-              .childHandler(new ChannelInitializer<SocketChannel>() {  
- @Override
- publicvoid initChannel(SocketChannel ch) throws Exception {  
-                      ch.pipeline().addLast(  
- //new LoggingHandler(LogLevel.INFO),
- new EchoServerHandler());  
-                  }  
-              });  
- 
- // Start the server.
-             ChannelFuture f = b.bind(port).sync();  
- 
- // Wait until the server socket is closed.
-             f.channel().closeFuture().sync();  
-         } finally {  
- // Shut down all event loops to terminate all threads.
-             bossGroup.shutdownGracefully();  
-             workerGroup.shutdownGracefully();  
-         }  
-     }  
- 
- publicstaticvoid main(String[] args) throws Exception {  
- int port;  
- if (args.length > 0) {  
-             port = Integer.parseInt(args[0]);  
-         } else {  
-             port = 8080;  
-         }  
- new EchoServer(port).run();  
-     }  
- }  
可以看出，用法和Bootstrap差不多。
## 作为Builder的ServerBootstrap
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17532857#)[copy](http://blog.csdn.net/zxhoo/article/details/17532857#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/126560/fork)
- publicfinalclass ServerBootstrap extends AbstractBootstrap<ServerBootstrap, ServerChannel> {  
- 
- privatefinal Map<ChannelOption<?>, Object> childOptions = new LinkedHashMap<ChannelOption<?>, Object>();  
- privatefinal Map<AttributeKey<?>, Object> childAttrs = new LinkedHashMap<AttributeKey<?>, Object>();  
- privatevolatile EventLoopGroup childGroup;  
- privatevolatile ChannelHandler childHandler;  
- // ...
- }  
看代码可以知道，ServerBootstrap比它的超类多了四个Part，如下图所示：
![](https://img-blog.csdn.net/20131224171645406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 从bind()方法入手
bind()方法实际上是在AbstractBootstrap里定义的，bind()先调用validate()方法检查各个Part是否准备就绪，然后把工作交给了doBind()方法。doBind()方法首先调用initAndRegister()方法，然后把工作交给doBind0()。initAndRegister()会调用[模板方法](http://en.wikipedia.org/wiki/Template_method_pattern)init()来初始化Channel，initAndRegister()方法的细节[上篇文章](http://blog.csdn.net/zxhoo/article/details/17419229)分析过了，这里不再复述。下面是整个方法调用过程的示意图：
![](https://img-blog.csdn.net/20131224174210250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## init()方法
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17532857#)[copy](http://blog.csdn.net/zxhoo/article/details/17532857#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/126560/fork)
- @Override
- void init(Channel channel) throws Exception {  
- final Map<ChannelOption<?>, Object> options = options();  
- synchronized (options) {  
-         channel.config().setOptions(options);  
-     }  
- 
- final Map<AttributeKey<?>, Object> attrs = attrs();  
- synchronized (attrs) {  
- for (Entry<AttributeKey<?>, Object> e: attrs.entrySet()) {  
- @SuppressWarnings("unchecked")  
-             AttributeKey<Object> key = (AttributeKey<Object>) e.getKey();  
-             channel.attr(key).set(e.getValue());  
-         }  
-     }  
- 
-     ChannelPipeline p = channel.pipeline();  
- if (handler() != null) {  
-         p.addLast(handler());  
-     }  
- 
- final EventLoopGroup currentChildGroup = childGroup;  
- final ChannelHandler currentChildHandler = childHandler;  
- final Entry<ChannelOption<?>, Object>[] currentChildOptions;  
- final Entry<AttributeKey<?>, Object>[] currentChildAttrs;  
- synchronized (childOptions) {  
-         currentChildOptions = childOptions.entrySet().toArray(newOptionArray(childOptions.size()));  
-     }  
- synchronized (childAttrs) {  
-         currentChildAttrs = childAttrs.entrySet().toArray(newAttrArray(childAttrs.size()));  
-     }  
- 
-     p.addLast(new ChannelInitializer<Channel>() {  
- @Override
- publicvoid initChannel(Channel ch) throws Exception {  
-             ch.pipeline().addLast(new ServerBootstrapAcceptor(  
-                     currentChildGroup, currentChildHandler, currentChildOptions, currentChildAttrs));  
-         }  
-     });  
- }  
ServerBootstrap类的init()方法虽然很长，但是却不难理解。首先是给Channel设置options和attrs，然后把User提供的针对NioServerSocketChannel的Handler添加到Pipeline的末尾。接下来复制childOptions和childAttrs，最后实例化一个ChannelInitializer，添加到Pipeline的末尾。init()方法执行完毕之后，AbstractBootstrap的initAndRegister()方法会将NioServerSocketChannel注册到group里。到此为止，NioServerSocketChannel的状态如下图所示：
![](https://img-blog.csdn.net/20131224180353312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
ChannelInitializer在channelRegistered事件触发后会调用initChannel()方法，随后把自己从Pipeline里删除：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17532857#)[copy](http://blog.csdn.net/zxhoo/article/details/17532857#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/126560/fork)
- @Sharable
- publicabstractclass ChannelInitializer<C extends Channel> extends ChannelInboundHandlerAdapter {  
- 
- privatestaticfinal InternalLogger logger = InternalLoggerFactory.getInstance(ChannelInitializer.class);  
- 
- /**
-      * This method will be called once the {@link Channel} was registered. After the method returns this instance
-      * will be removed from the {@link ChannelPipeline} of the {@link Channel}.
-      *
-      * @param ch            the {@link Channel} which was registered.
-      * @throws Exception    is thrown if an error occurs. In that case the {@link Channel} will be closed.
-      */
- protectedabstractvoid initChannel(C ch) throws Exception;  
- 
- @SuppressWarnings("unchecked")  
- @Override
- publicfinalvoid channelRegistered(ChannelHandlerContext ctx)  
- throws Exception {  
- boolean removed = false;  
- boolean success = false;  
- try {  
-             initChannel((C) ctx.channel());  
-             ctx.pipeline().remove(this);  
-             removed = true;  
-             ctx.fireChannelRegistered();  
-             success = true;  
-         } catch (Throwable t) {  
-             logger.warn("Failed to initialize a channel. Closing: " + ctx.channel(), t);  
-         } finally {  
- if (!removed) {  
-                 ctx.pipeline().remove(this);  
-             }  
- if (!success) {  
-                 ctx.close();  
-             }  
-         }  
-     }  
- }  
所以注册之后的NioServerSocketChannel状态如下图所示：
![](https://img-blog.csdn.net/20131224180952781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## ServerBootstrapAcceptor
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17532857#)[copy](http://blog.csdn.net/zxhoo/article/details/17532857#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/126560/fork)
- privatestaticclass ServerBootstrapAcceptor extends ChannelInboundHandlerAdapter {  
- 
- privatefinal EventLoopGroup childGroup;  
- privatefinal ChannelHandler childHandler;  
- privatefinal Entry<ChannelOption<?>, Object>[] childOptions;  
- privatefinal Entry<AttributeKey<?>, Object>[] childAttrs;  
- 
- @SuppressWarnings("unchecked")  
-     ServerBootstrapAcceptor(  
-             EventLoopGroup childGroup, ChannelHandler childHandler,  
-             Entry<ChannelOption<?>, Object>[] childOptions, Entry<AttributeKey<?>, Object>[] childAttrs) {  
- this.childGroup = childGroup;  
- this.childHandler = childHandler;  
- this.childOptions = childOptions;  
- this.childAttrs = childAttrs;  
-     }  
- 
- @Override
- @SuppressWarnings("unchecked")  
- publicvoid channelRead(ChannelHandlerContext ctx, Object msg) {  
-         Channel child = (Channel) msg;  
- 
-         child.pipeline().addLast(childHandler);  
- 
- for (Entry<ChannelOption<?>, Object> e: childOptions) {  
- try {  
- if (!child.config().setOption((ChannelOption<Object>) e.getKey(), e.getValue())) {  
-                     logger.warn("Unknown channel option: " + e);  
-                 }  
-             } catch (Throwable t) {  
-                 logger.warn("Failed to set a channel option: " + child, t);  
-             }  
-         }  
- 
- for (Entry<AttributeKey<?>, Object> e: childAttrs) {  
-             child.attr((AttributeKey<Object>) e.getKey()).set(e.getValue());  
-         }  
- 
- try {  
-             childGroup.register(child);  
-         } catch (Throwable t) {  
-             child.unsafe().closeForcibly();  
-             logger.warn("Failed to register an accepted channel: " + child, t);  
-         }  
-     }  
- // ...
- }  
ServerBootstrapAcceptor在channelRead事件触发的时候（也就有客户端连接的时候），把childHandler加到childChannel Pipeline的末尾，设置childHandler的options和attrs，最后把childHandler注册进childGroup，如下图所示：
![](https://img-blog.csdn.net/20131224181801250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



