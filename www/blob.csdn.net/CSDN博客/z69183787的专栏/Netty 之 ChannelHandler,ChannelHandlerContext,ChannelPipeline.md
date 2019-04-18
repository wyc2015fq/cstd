# Netty 之 ChannelHandler,ChannelHandlerContext,ChannelPipeline - z69183787的专栏 - CSDN博客
2016年09月22日 15:45:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1234
本小节一起学习一下ChannelHandler,ChannelHandlerContext,ChannelPipeline这三个Netty常用的组件，不探究它们的底层源码，我们就简单的分析一下用法
首先先分析一下ChannelHandler，ChannelHandler是我们日常开发中使用最多的组件了，大概我们平时写的最多的组件就是Handler了，继承图如下
![](https://img-blog.csdn.net/20160504160757707?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们平时继承的最多的就是ChannelInboundHandlerAdapter和ChannelOutboundHandlerAdapter,这两个不是接口也不是抽象类，所以我们可以仅仅重写我们需要的方法，没有必须要实现的方法，当然我们也会使用SimpleChannelInboundHandler，这个类我们上个小节也稍微讲了它的优缺点，这里不赘述
ChannelHandler,ChannelHandlerContext,ChannelPipeline这三者的关系很特别，相辅相成，一个ChannelPipeline中可以有多个ChannelHandler实例，而每一个ChannelHandler实例与ChannelPipeline之间的桥梁就是ChannelHandlerContext实例，如图所示：
![](https://img-blog.csdn.net/20160504161903129?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
看图就知道，ChannelHandlerContext的重要性了，如果你获取到了ChannelHandlerContext的实例的话，你可以获取到你想要的一切，你可以根据ChannelHandlerContext执行ChannelHandler中的方法，我们举个例子来说，我们可以看下ChannelHandlerContext部分API:
![](https://img-blog.csdn.net/20160504165756603?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**这几个API都是使用比较频繁的，都是调用当前handler之后同一类型的channel中的某个方法，这里的同一类型指的是同一个方向，比如inbound调用inbound，outbound调用outbound类型的channel，一般来说，都是一个channel的ChannnelActive方法中调用fireChannelActive来触发调用下一个handler中的ChannelActive方法**
我们举例来说，我们修改Helloworld版中的部分代码，在客户端中的channel中修改一下代码
首先我们修改一下bootstrap的启动类代码：
**[html]**[view
 plain](http://blog.csdn.net/linuu/article/details/51315373#)[copy](http://blog.csdn.net/linuu/article/details/51315373#)
- try {  
-             Bootstrap b = new Bootstrap();  
-             b.group(group)  
-              .channel(NioSocketChannel.class)  
-              .option(ChannelOption.TCP_NODELAY, true)  
-              .handler(new ChannelInitializer<SocketChannel>() {  
-                  @Override  
-                  public void initChannel(SocketChannel ch) throws Exception {  
-                      ChannelPipeline p = ch.pipeline();  
-                      p.addLast("decoder", new StringDecoder());  
-                      p.addLast("encoder", new StringEncoder());  
-                      p.addLast(new BaseClient1Handler());  
-                      p.addLast(new BaseClient2Handler());  
-                  }  
-              });  
- 
-             ChannelFuture future = b.connect(HOST, PORT).sync();  
-             future.channel().writeAndFlush("Hello Netty Server ,I am a common client");  
-             future.channel().closeFuture().sync();  
-         } finally {  
-             group.shutdownGracefully();  
-         }  
我们在channelhandler链中加了两个自定义的BaseClient1Handler和BaseClient2Handler的处理器
BaseClient1Handler的方法也很简单：
BaseClient1Handler.java
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51315373#)[copy](http://blog.csdn.net/linuu/article/details/51315373#)
- package com.lyncc.netty.component.channelhandler;  
- 
- import io.netty.channel.ChannelHandlerContext;  
- import io.netty.channel.ChannelInboundHandlerAdapter;  
- 
- /**
-  * 
-  * @author bazingaLyncc
-  * 描述：客户端的第一个自定义的inbound处理器
-  * 时间  2016年5月3日
-  */
- publicclass BaseClient1Handler extends ChannelInboundHandlerAdapter{  
- 
- @Override
- publicvoid channelActive(ChannelHandlerContext ctx) throws Exception {  
-         System.out.println("BaseClient1Handler channelActive");  
- //        ctx.fireChannelActive();
-     }  
- 
- @Override
- publicvoid channelInactive(ChannelHandlerContext ctx) throws Exception {  
-         System.out.println("BaseClient1Handler channelInactive");  
-     }  
- 
- }  
BaseClient2Handler.java
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51315373#)[copy](http://blog.csdn.net/linuu/article/details/51315373#)
- package com.lyncc.netty.component.channelhandler;  
- 
- import io.netty.channel.ChannelHandlerContext;  
- import io.netty.channel.ChannelInboundHandlerAdapter;  
- 
- /**
-  * 
-  * @author bazingaLyncc
-  * 描述：客户端的第二个自定义的inbound处理器
-  * 时间  2016年5月3日
-  */
- publicclass BaseClient2Handler extends ChannelInboundHandlerAdapter{  
- 
- @Override
- publicvoid channelActive(ChannelHandlerContext ctx) throws Exception {  
-         System.out.println("BaseClient2Handler Active");  
-     }  
- 
- 
- 
- }  
其他的代码不修改的，我们先启动服务器端，然后启动客户端，你会发现控制台打印了：
![](https://img-blog.csdn.net/20160504173625107?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
不会打印BaseClient2Handler类中channelActive方法中的输出语句，如果你想要打印，你可以将BaseClient1Handler中的channelActive的ctx.fireChannelActive()注释去掉。重新运行：
![](https://img-blog.csdn.net/20160504173922899?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
也就是说如果一个channelPipeline中有多个channelHandler时，且这些channelHandler中有同样的方法时，例如这里的channelActive方法，只会调用处在第一个的channelHandler中的channelActive方法，如果你想要调用后续的channelHandler的同名的方法就需要调用以“fire”为开头的方法了，这样做很灵活
目前来说这样做的好处：
1）每一个handler只需要关注自己要处理的方法，如果你不关注channelActive方法时，你自定义的channelhandler就不需要重写channelActive方法
2）异常处理，如果 exceptionCaught方法每个handler都重写了，只需有一个类捕捉到然后做处理就可以了，不需要每个handler都处理一遍
3）灵活性。例如如下图所示：
![](https://img-blog.csdn.net/20160504175109763?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如图所示在业务逻辑处理中，也许左侧第一个ChannelHandler根本不需要管理某个业务逻辑，但是从第二个ChannelHandler就需要关注处理某个业务需求了，那么就可以很灵活地从第二个ChannelHandler开始处理业务，不需要从channel中的第一个ChannelHandler开始处理，这样会使代码显得让人看不懂~
初步看懂的ChannelHandler,ChannelHandlerContext,ChannelPipeline之间的关系就是如上总结的
以上三点是我自己总结的，没看源码，有些也可能不对，欢迎拍砖，一起学习的过程，不保证全部对~
