# 一起学Netty（三）之 SimpleChannelInboundHandler - z69183787的专栏 - CSDN博客
2016年10月31日 10:34:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：10414
 其实Netty的知识点还是很零碎的，比如这个SimpleChannelInboundHandler这个类，在《Netty in Action》该书中的原版的Hello world的demo的客户端就是使用的SimpleChannelInboundHandler来作为处理器的，我本来也是使用这个类作为我处理类的，但是做一个新手，这个类还是让我走了一点弯路，我们可以看到SimpleChannelInboundHandler中有一个channelRead0的方法需要我们实现：
![](https://img-blog.csdn.net/20160503192618125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
尼玛，我记得当时就是使用的channelRead0这个方法，结果服务器端就是不打印，服务器返回的结果，当时客户端是这样写的
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51307060#)[copy](http://blog.csdn.net/linuu/article/details/51307060#)
- package com.lyncc.netty.component.simplehandler;  
- 
- import io.netty.buffer.ByteBuf;  
- import io.netty.channel.ChannelHandlerContext;  
- import io.netty.channel.SimpleChannelInboundHandler;  
- 
- publicclass BaseClientHandler extends SimpleChannelInboundHandler<ByteBuf>{  
- 
- @Override
- protectedvoid channelRead0(ChannelHandlerContext ctx, ByteBuf msg) throws Exception {  
-         System.out.println("Client channelRead0 received:" + msg);  
-     }  
- 
- //    @Override
- //    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
- //        System.out.println("Client channelRead received:" + msg);
- //        
- //    }
- 
- @Override
- publicvoid exceptionCaught(ChannelHandlerContext ctx, Throwable cause) {  
-          cause.printStackTrace();  
-          ctx.close();  
-      }  
- 
- 
- 
- }  
结果channelRead0方法就是不运行执行，让我郁闷了有一会，后来，我打开了SimpleChannelInboundHandler的源码，其实这是我很不想看见的，尼玛，一个hello world就已经开始看源代码了，还让不让人活了啊，结果源码中显示SimpleChannelInboundHandler是继承于ChannelInboundHandlerAdapter，重写了channelRead方法：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51307060#)[copy](http://blog.csdn.net/linuu/article/details/51307060#)
- @Override
- publicvoid channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {  
- boolean release = true;  
- try {  
- if (acceptInboundMessage(msg)) {  
- @SuppressWarnings("unchecked")  
-                 I imsg = (I) msg;  
-                 channelRead0(ctx, imsg);  
-             } else {  
-                 release = false;  
-                 ctx.fireChannelRead(msg);  
-             }  
-         } finally {  
- if (autoRelease && release) {  
-                 ReferenceCountUtil.release(msg);  
-             }  
-         }  
-     }  
原来我们SimpleChannelInboundHandler后面指定了处理类型，也就是源码中的"I",acceptInboundMessage方法判断msg是不是SimpleChannelInboundHandler中指定的类型，我们这边指定的是ByteBuf，感觉没啥问题啊，但是我们忽略了一个问题，我们客户端中有3个处理器，两个inbound类型的处理器，其中一个就是HelloWorldClientHandler，还有一个就是StringDecoder，此时我内心是崩溃的，尼玛，上一个处理器已经把服务器端的信息转化成String，我还用ByteBuf来接收，能处理才有鬼，修改一下我们的代码就可以了，重新指定一下处理的类型
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51307060#)[copy](http://blog.csdn.net/linuu/article/details/51307060#)
- package com.lyncc.netty.component.simplehandler;  
- 
- import io.netty.channel.ChannelHandlerContext;  
- import io.netty.channel.SimpleChannelInboundHandler;  
- 
- publicclass BaseClientHandler extends SimpleChannelInboundHandler<String>{  
- 
- @Override
- protectedvoid channelRead0(ChannelHandlerContext ctx, String msg) throws Exception {  
-         System.out.println("Client channelRead0 received:" + msg);  
-     }  
- 
- //    @Override
- //    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
- //        System.out.println("Client channelRead received:" + msg);
- //        
- //    }
- 
- @Override
- publicvoid exceptionCaught(ChannelHandlerContext ctx, Throwable cause) {  
-          cause.printStackTrace();  
-          ctx.close();  
-      }  
- 
- 
- }  
这样就能使channelRead0方法运行了，也怪我眼瞎，书中写的很清楚：
![](https://img-blog.csdn.net/20160503193814763?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
SimpleChannelInboundHandler的channelRead0还有一个好处就是你不用关心释放资源，因为源码中已经帮你释放了，所以如果你保存获取的信息的引用，是无效的~
这个是学习Netty走的一段弯路，在这里跟大家share一下~
