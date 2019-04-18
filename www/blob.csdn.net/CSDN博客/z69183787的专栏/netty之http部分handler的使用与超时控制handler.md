# netty之http部分handler的使用与超时控制handler - z69183787的专栏 - CSDN博客
2016年09月26日 16:49:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4280
个人分类：[RPC-Netty](https://blog.csdn.net/z69183787/article/category/6412734)
这部分讲讲使用的最多的一种handler的使用情况，http部分的handler，另外再来讲讲超时控制handler的实现
```java
package fjs;
import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelOption;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioServerSocketChannel;
import io.netty.handler.codec.http.HttpObjectAggregator;
import io.netty.handler.codec.http.HttpRequestDecoder;
import io.netty.handler.codec.http.HttpResponseEncoder;
import io.netty.handler.stream.ChunkedWriteHandler;
import io.netty.handler.timeout.ReadTimeoutHandler;
import io.netty.handler.timeout.WriteTimeoutHandler;
public class Fjs {
	public void run() throws Exception {
		EventLoopGroup bossGroup = new NioEventLoopGroup();   //这个是用于serversocketchannel的eventloop
		EventLoopGroup workerGroup = new NioEventLoopGroup();    //这个是用于处理accept到的channel
		try {
			ServerBootstrap b = new ServerBootstrap();    //构建serverbootstrap对象
			b.group(bossGroup, workerGroup);   //设置时间循环对象，前者用来处理accept事件，后者用于处理已经建立的连接的io
			b.channel(NioServerSocketChannel.class);   //用它来建立新accept的连接，用于构造serversocketchannel的工厂类
			
			
			b.childHandler(new ChannelInitializer<SocketChannel>(){      //为accept channel的pipeline预添加的inboundhandler
				@Override     //当新连接accept的时候，这个方法会调用
				protected void initChannel(SocketChannel ch) throws Exception {
					
					ch.pipeline().addLast(new ReadTimeoutHandler(10));
					ch.pipeline().addLast(new WriteTimeoutHandler(1));
					
					ch.pipeline().addLast(new HttpRequestDecoder());   //用于解析http报文的handler
					ch.pipeline().addLast(new HttpObjectAggregator(65536));   //用于将解析出来的数据封装成http对象，httprequest什么的
					ch.pipeline().addLast(new HttpResponseEncoder());   //用于将response编码成httpresponse报文发送
				    //ch.pipeline().addLast("chunkedWriter", new ChunkedWriteHandler());
					
					ch.pipeline().addLast(new HttpHanlder());
				}
				
			});
			//bind方法会创建一个serverchannel，并且会将当前的channel注册到eventloop上面，
			//会为其绑定本地端口，并对其进行初始化，为其的pipeline加一些默认的handler
			ChannelFuture f = b.bind(80).sync();    
			f.channel().closeFuture().sync();  //相当于在这里阻塞，直到serverchannel关闭
		} finally {
			bossGroup.shutdownGracefully();
			workerGroup.shutdownGracefully();
		}
	}
	
	public static void main(String args[]) throws Exception {
		new Fjs().run();
	}
}
```
这部分主要是要看pipeline上面handler的分布情况
（1）ReadTimeoutHandler，用于控制读取数据的时候的超时，10表示如果10秒钟都没有数据读取了，那么就引发超时，然后关闭当前的channel
（2）WriteTimeoutHandler，用于控制数据输出的时候的超时，构造参数1表示如果持续1秒钟都没有数据写了，那么就超时。
（3）HttpRequestrianDecoder，这个handler用于从读取的数据中将http报文信息解析出来，无非就是什么requestline，header，body什么的。。。
（4）然后HttpObjectAggregator则是用于将上卖解析出来的http报文的数据组装成为封装好的httprequest对象。。
（5）HttpresponseEncoder，用于将用户返回的httpresponse编码成为http报文格式的数据
（6）HttpHandler，哈，不要被这个名字给唬住了，其实这个是自己定义的handler，用于处理接收到的http请求。
那么接下来来看看我自己定义的这个handler吧，主要就看一下channelRead方法就好了，当数据传递到这个handler的时候会调用这个方法来处理进来的数据：
```java
@Override
	public void channelRead(ChannelHandlerContext ctx, Object req)
			throws Exception {
		// TODO Auto-generated method stub
		FullHttpRequest request = (FullHttpRequest)req;  //将其强制转化为httprequest
		
		boolean keepAlive = isKeepAlive(request);   //判断当前的连接时否是keepalive的
		
		ByteBuf b = ctx.alloc().buffer();
		
		
		b.writeBytes("<html><head><title>haha</title></head><body>aaa<body></html>".getBytes());
		
		FullHttpResponse response = new DefaultFullHttpResponse(
               HTTP_1_1, BAD_REQUEST, b);
       response.headers().set(CONTENT_TYPE, "text/html; charset=UTF-8");
       response.headers().set(CONTENT_LENGTH, b.readableBytes());
       if (!keepAlive) {
           ctx.writeAndFlush(response).addListener(ChannelFutureListener.CLOSE);
       } else {
    	   response.headers().set(CONNECTION, Values.KEEP_ALIVE);
           ctx.writeAndFlush(response);
       }
	}
```
这部分的代码就更加的简单了，说白了就是从request对象中取得一些信息，然后再将要发送的数据封装成httpresponse对象写出去就好了。。。
好了，那么在上面就基本说明了在netty中http部分handler的使用，那么接下来来看看在netty超时handler的实现原理吧：
```java
public ReadTimeoutHandler(long timeout, TimeUnit unit) {
        if (unit == null) {
            throw new NullPointerException("unit");
        }
        //这里是用于设置超时的时间
        if (timeout <= 0) {
            timeoutMillis = 0;
        } else {
            timeoutMillis = Math.max(unit.toMillis(timeout), 1);
        }
    }
    @Override
    public void handlerAdded(ChannelHandlerContext ctx) throws Exception {
        if (ctx.channel().isActive() && ctx.channel().isRegistered()) {
            // channelActvie() event has been fired already, which means this.channelActive() will
            // not be invoked. We have to initialize here instead.
            initialize(ctx);
        } else {
            // channelActive() event has not been fired yet.  this.channelActive() will be invoked
            // and initialization will occur there.
        }
    }
```
这里构造函数很简单，无非就是设置一下超时的时间而已，然后这里会有一个比较重要的函数，initialize函数，这个用于初始化当前的超时handler，
那么我们来看看这个初始化方法的定义吧：
```java
<span style="font-size:12px;">//初始化当前的handler，用于设置task，当超时时间到了以后会被调用，然后再判断是否有数据读取了
    private void initialize(ChannelHandlerContext ctx) {
        // Avoid the case where destroy() is called before scheduling timeouts.
        // See: https://github.com/netty/netty/issues/143
        switch (state) {
        case 1:
        case 2:
            return;
        }
        state = 1;
        lastReadTime = System.currentTimeMillis();  //lastReadTime表示上一次有数据读取的时间点
        if (timeoutMillis > 0) {
            timeout = ctx.executor().schedule(  //构造并调度超时任务
                    new ReadTimeoutTask(ctx),
                    timeoutMillis, TimeUnit.MILLISECONDS);
        }
    }</span>
```
其实看到这里基本上就已经很清楚在netty中io的超时handler是怎么处理的了，很简单，无非就是在当前的eventLoop中再设置一个task，当然这个task是按时间调度的，当超后就被调用，然后再判断当前的io是否已经超时。。。‘
那么解下来来看看这个超时task是怎么定义的吧：
```java
<span style="font-size:18px;"> </span><span style="font-size:12px;">private final class ReadTimeoutTask implements Runnable {
        private final ChannelHandlerContext ctx;
        ReadTimeoutTask(ChannelHandlerContext ctx) {
            this.ctx = ctx;
        }
        @Override
        public void run() {
            if (!ctx.channel().isOpen()) {  //如果channel已经关闭了，那么就不用理会了
                return;
            }
            long currentTime = System.currentTimeMillis();  //当前的系统时间
            long nextDelay = timeoutMillis - (currentTime - lastReadTime);  //nextDelay表示当前有这么多时间没有数据读取了
            if (nextDelay <= 0) {  //表示超时已经发生了
                // Read timed out - set a new timeout and notify the callback.
                timeout = ctx.executor().schedule(this, timeoutMillis, TimeUnit.MILLISECONDS);  //这里相当于再设置一下timeout，确保channel关闭
                try {
                    readTimedOut(ctx);
                } catch (Throwable t) {
                    ctx.fireExceptionCaught(t);
                }
            } else {
                // Read occurred before the timeout - set a new timeout with shorter delay.
                timeout = ctx.executor().schedule(this, nextDelay, TimeUnit.MILLISECONDS);  //继续下次超时任务
            }
        }
    }</span>
```
啊。这个应该很简单吧，当被调用的时候，那么就判断一下超时是否已经发生，如果之前发生了io，那么就重新设置一下超时任务，如果没有的话，那么就调用timeout函数就好了。。。。
另外timeout函数其实很简单，就是抛出超时异常，然后关闭当前的channel。。
上面的是读取的超时，其实write的超时原理都一样的。
好了，那么到这里http部分的handler的基本使用，以及超时handler的原理就差不太多了。。。
