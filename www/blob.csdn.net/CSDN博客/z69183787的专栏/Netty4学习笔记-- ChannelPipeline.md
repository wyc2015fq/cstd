# Netty4学习笔记-- ChannelPipeline - z69183787的专栏 - CSDN博客
2016年09月22日 15:30:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1876
## Netty4
[Netty](http://netty.io/)是一个和[MINA](http://mina.apache.org/)类似的Java NIO框架，目前的最新版本是[4.0.13](http://dl.bintray.com/netty/downloads/netty-4.0.13.Final.tar.bz2)，这两个框架的主要作者好像都是[同一个韩国人](http://stackoverflow.com/users/55808/trustin)。
## Channel
[Channel](http://netty.io/4.0/api/io/netty/channel/Channel.html)是Netty最核心的接口，一个Channel就是一个联络Socket的通道，通过Channel，你可以对Socket进行各种操作。
## ChannelHandler
用Netty编写网络程序的时候，你很少直接操纵Channel，而是通过[ChannelHandler](http://netty.io/4.0/api/io/netty/channel/ChannelHandler.html)来间接操纵Channel。
## ChannelPipeline
[ChannelPipeline](http://netty.io/4.0/api/io/netty/channel/ChannelPipeline.html)实际上应该叫做ChannelHandlerPipeline，可以把ChannelPipeline看成是一个ChandlerHandler的链表，当需要对Channel进行某种处理的时候，Pipeline负责依次调用每一个Handler进行处理。每个Channel都有一个属于自己的Pipeline，调用**Channel#pipeline()**方法可以获得Channel的Pipeline，调用**Pipeline#channel()**方法可以获得Pipeline的Channel。
![](https://img-blog.csdn.net/20131211170811796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
ChannelPipeline的方法有很多，其中一部分是用来管理ChannelHandler的，如下面这些：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17264263#)[copy](http://blog.csdn.net/zxhoo/article/details/17264263#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/109260/fork)
- ChannelPipeline addFirst(String name, ChannelHandler handler);  
- ChannelPipeline addLast(String name, ChannelHandler handler);  
- ChannelPipeline addBefore(String baseName, String name, ChannelHandler handler);  
- ChannelPipeline addAfter(String baseName, String name, ChannelHandler handler);  
- ChannelPipeline remove(ChannelHandler handler);  
- ChannelHandler remove(String name);  
- ChannelHandler removeFirst();  
- ChannelHandler removeLast();  
- ChannelPipeline replace(ChannelHandler oldHandler, String newName, ChannelHandler newHandler);  
- ChannelHandler replace(String oldName, String newName, ChannelHandler newHandler);  
- ChannelHandler first();  
- ChannelHandler last();  
- ChannelHandler get(String name);  
根据上面的方法，能够大概想象的到Pipeline按照什么样的方式组织Handler。
## ChannelHandlerContext
ChannelPipeline并不是直接管理ChannelHandler，而是通过[ChannelHandlerContext](http://netty.io/4.0/api/io/netty/channel/ChannelHandlerContext.html)来间接管理，这一点通过ChannelPipeline的默认实现DefaultChannelPipeline可以看出来。
![](https://img-blog.csdn.net/20131211194121609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
调用**ChannelHandlerContext#channel()**方法可以得到和Context绑定的Channel，调用**ChannelHandlerContext#handler()**方法可以得到和Context绑定的Handler。
ChannelPipeline和ChannelHandlerContext默认实现
DefaultChannelHandlerContext和DefaultChannelPipeline是ChannelHandlerContext和ChannelPipeline的默认实现，下面是它们的部分代码：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17264263#)[copy](http://blog.csdn.net/zxhoo/article/details/17264263#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/109260/fork)
- finalclass DefaultChannelHandlerContext extends DefaultAttributeMap implements ChannelHandlerContext {  
- 
- volatile DefaultChannelHandlerContext next;  
- volatile DefaultChannelHandlerContext prev;  
- 
- privatefinalboolean inbound;  
- privatefinalboolean outbound;  
- privatefinal AbstractChannel channel;  
- privatefinal DefaultChannelPipeline pipeline;  
- privatefinal String name;  
- privatefinal ChannelHandler handler;  
- privateboolean removed;  
- 
- // ...
- }  
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17264263#)[copy](http://blog.csdn.net/zxhoo/article/details/17264263#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/109260/fork)
- finalclass DefaultChannelPipeline implements ChannelPipeline {  
- // ...
- 
- final DefaultChannelHandlerContext head;  
- final DefaultChannelHandlerContext tail;  
- 
- // ...
- }  
从上面的代码可以看出，在DefaultPipeline内部，DefaultChannelHandlerContext组成了一个双向链表：
![](https://img-blog.csdn.net/20131212093055609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
再来看看DefaultChannelPipeline的构造函数：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17264263#)[copy](http://blog.csdn.net/zxhoo/article/details/17264263#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/109260/fork)
- public DefaultChannelPipeline(AbstractChannel channel) {  
- if (channel == null) {  
- thrownew NullPointerException("channel");  
-     }  
- this.channel = channel;  
- 
-     TailHandler tailHandler = new TailHandler();  
-     tail = new DefaultChannelHandlerContext(this, null, generateName(tailHandler), tailHandler);  
- 
-     HeadHandler headHandler = new HeadHandler(channel.unsafe());  
-     head = new DefaultChannelHandlerContext(this, null, generateName(headHandler), headHandler);  
- 
-     head.next = tail;  
-     tail.prev = head;  
- }  
可以看到，DefaultChinnelPipeline内部使用了两个特殊的Handler来表示Handler链的头和尾：
![](https://img-blog.csdn.net/20131212120733046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## ChannelHandler的种类
从上面DefaultChannelHandlerContext代码可以知道，Handler实际上分为两种，Inbound和Outbound，这一点也可以从ChannelHandler接口的子接口得到证明：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17264263#)[copy](http://blog.csdn.net/zxhoo/article/details/17264263#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/109260/fork)
- publicinterface ChannelInboundHandler extends ChannelHandler {  
- // ...
- }  
- 
- publicinterface ChannelOutboundHandler extends ChannelHandler {  
- // ...
- }  
## 事件的传播
为了搞清楚事件如何在Pipeline里传播，让我们从Channel的抽象子类AbstractChannel开始，下面是AbstractChannel#write()方法的实现：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17264263#)[copy](http://blog.csdn.net/zxhoo/article/details/17264263#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/109260/fork)
- publicabstractclass AbstractChannel extends DefaultAttributeMap implements Channel {  
- // ...
- @Override
- public Channel write(Object msg) {  
- return pipeline.write(msg);  
-     }  
- // ...
- }  
AbstractChannel直接调用了Pipeline的write()方法：
![](https://img-blog.csdn.net/20131212133508828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
再看DefaultChannelPipeline的write()方法实现：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17264263#)[copy](http://blog.csdn.net/zxhoo/article/details/17264263#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/109260/fork)
- finalclass DefaultChannelPipeline implements ChannelPipeline {  
- // ...
- @Override
- public ChannelFuture write(Object msg) {  
- return tail.write(msg);  
-     }  
- // ...
- }  
因为write是个outbound事件，所以DefaultChannelPipeline直接找到tail部分的context，调用其write()方法：
![](https://img-blog.csdn.net/20131212134216687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
接着看DefaultChannelHandlerContext的write()方法：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17264263#)[copy](http://blog.csdn.net/zxhoo/article/details/17264263#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/109260/fork)
- finalclass DefaultChannelHandlerContext extends DefaultAttributeMap implements ChannelHandlerContext {  
- // ...
- @Override
- public ChannelFuture write(Object msg) {  
- return write(msg, newPromise());  
-     }  
- 
- @Override
- public ChannelFuture write(final Object msg, final ChannelPromise promise) {  
- if (msg == null) {  
- thrownew NullPointerException("msg");  
-         }  
- 
-         validatePromise(promise, true);  
- 
-         write(msg, false, promise);  
- 
- return promise;  
-     }  
- 
- privatevoid write(Object msg, boolean flush, ChannelPromise promise) {  
-         DefaultChannelHandlerContext next = findContextOutbound();  
-         next.invokeWrite(msg, promise);  
- if (flush) {  
-             next.invokeFlush();  
-         }  
-     }  
- 
- private DefaultChannelHandlerContext findContextOutbound() {  
-         DefaultChannelHandlerContext ctx = this;  
- do {  
-             ctx = ctx.prev;  
-         } while (!ctx.outbound);  
- return ctx;  
-     }  
- 
- privatevoid invokeWrite(Object msg, ChannelPromise promise) {  
- try {  
-             ((ChannelOutboundHandler) handler).write(this, msg, promise);  
-         } catch (Throwable t) {  
-             notifyOutboundHandlerException(t, promise);  
-         }  
-     }  
- 
- // ...
- }  
context的write()方法沿着context链往前找，直至找到一个outbound类型的context为止，然后调用其invokeWrite()方法：
![](https://img-blog.csdn.net/20131212140450171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
invokeWrite()接着调用handler的write()方法：
![](https://img-blog.csdn.net/20131212140949687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
最后看看ChannelOutboundHandlerAdapter的write()方法实现：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/17264263#)[copy](http://blog.csdn.net/zxhoo/article/details/17264263#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/109260/fork)
- publicclass ChannelOutboundHandlerAdapter extends ChannelHandlerAdapter implements ChannelOutboundHandler {  
- // ...
- @Override
- publicvoid write(ChannelHandlerContext ctx, Object msg, ChannelPromise promise) throws Exception {  
-         ctx.write(msg, promise);  
-     }  
- // ...
- }  
默认的实现调用了context的write()方法而不做任何处理，这样write事件就沿着outbound链继续传播：
![](https://img-blog.csdn.net/20131212141601265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenhob28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
可见，Pipeline的事件传播，是靠Pipeline，Context和Handler共同协作完成的。
