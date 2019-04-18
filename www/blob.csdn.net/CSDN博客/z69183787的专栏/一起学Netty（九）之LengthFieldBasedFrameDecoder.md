# 一起学Netty（九）之LengthFieldBasedFrameDecoder - z69183787的专栏 - CSDN博客
2016年10月31日 11:00:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1788
之前介绍了Netty天然的几种解析器，也稍微介绍了一下ByteToMessageDecoder类，我们对Netty的解码器还是有了一定的了解~
今天要介绍的是Netty中一个很重要的解码器，因为相比于其他的普通的解码器，这个解码器用的场景更多，并不是说其他解码器不重要，只是因为我们业务场景所致
在当今比较流行的水平拆分的[架构](http://lib.csdn.net/base/architecture)之下，RPC协议很是流行，这样可以使各个项目解耦，使得更加灵活，每个项目之间通过远程调用交互，相互之间定义一个通讯私有协议，然后解析，这样就可以进行数据接口交互
例如我们定义一个这样的协议类：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51371595#)[copy](http://blog.csdn.net/linuu/article/details/51371595#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1679598/fork)
- package com.lyncc.netty.codec.lengthFieldBasedFrame;  
- 
- publicclass CustomMsg {  
- 
- //类型  系统编号 0xAB 表示A系统，0xBC 表示B系统
- privatebyte type;  
- 
- //信息标志  0xAB 表示心跳包    0xBC 表示超时包  0xCD 业务信息包
- privatebyte flag;  
- 
- //主题信息的长度
- privateint length;  
- 
- //主题信息
- private String body;  
- 
- public CustomMsg() {  
- 
-     }  
- 
- public CustomMsg(byte type, byte flag, int length, String body) {  
- this.type = type;  
- this.flag = flag;  
- this.length = length;  
- this.body = body;  
-     }  
- 
- publicbyte getType() {  
- return type;  
-     }  
- 
- publicvoid setType(byte type) {  
- this.type = type;  
-     }  
- 
- publicbyte getFlag() {  
- return flag;  
-     }  
- 
- publicvoid setFlag(byte flag) {  
- this.flag = flag;  
-     }  
- 
- publicint getLength() {  
- return length;  
-     }  
- 
- publicvoid setLength(int length) {  
- this.length = length;  
-     }  
- 
- public String getBody() {  
- return body;  
-     }  
- 
- publicvoid setBody(String body) {  
- this.body = body;  
-     }  
- 
- }  
我们规定两个系统通过Netty去发送这样的一个格式的信息，CustomMsg中包含这样的几类信息：
1）type表示发送端的系统类型
2）flag表示发送信息的类型，是业务数据，还是心跳包数据
3）length表示主题body的长度
4）body表示主题信息
有了这样的相互规定，发送端与接收端按照这种格式去编码和解码数据，这样就很容易的进行数据交互，当然如果netty不提供任何的类，我们也能进行编码解码，但是Netty还是提供了一个现有的类，这样可以避免我们重复造车，并且即使我们愿意重复造车，我们造的车也不一定比Netty好，所以我们还是直接使用吧
Netty提供的类叫做LengthFieldBasedFrameDecoder，与其他的解码器不一致的地方是它需要几个参数作为它的构造函数参数：
这几个参数的详细解析可以见如下文档：
http://blog.163.com/linfenliang@126/blog/static/127857195201210821145721/
我们也仔细说明一下这些参数，加入我们需要解析，加入我们需要解析我们刚才定义的CustomMsg，我们需要自定义一个decoder，这个类继承Netty提供的LengthFieldBasedFrameDecoder：
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51371595#)[copy](http://blog.csdn.net/linuu/article/details/51371595#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1679598/fork)
- package com.lyncc.netty.codec.lengthFieldBasedFrame;  
- 
- import io.netty.buffer.ByteBuf;  
- import io.netty.channel.ChannelHandlerContext;  
- import io.netty.handler.codec.LengthFieldBasedFrameDecoder;  
- 
- publicclass CustomDecoder extends LengthFieldBasedFrameDecoder {  
- 
- //判断传送客户端传送过来的数据是否按照协议传输，头部信息的大小应该是 byte+byte+int = 1+1+4 = 6
- privatestaticfinalint HEADER_SIZE = 6;  
- 
- privatebyte type;  
- 
- privatebyte flag;  
- 
- privateint length;  
- 
- private String body;  
- 
- /**
-      * 
-      * @param maxFrameLength 解码时，处理每个帧数据的最大长度
-      * @param lengthFieldOffset 该帧数据中，存放该帧数据的长度的数据的起始位置
-      * @param lengthFieldLength 记录该帧数据长度的字段本身的长度
-      * @param lengthAdjustment 修改帧数据长度字段中定义的值，可以为负数
-      * @param initialBytesToStrip 解析的时候需要跳过的字节数
-      * @param failFast 为true，当frame长度超过maxFrameLength时立即报TooLongFrameException异常，为false，读取完整个帧再报异常
-      */
- public CustomDecoder(int maxFrameLength, int lengthFieldOffset, int lengthFieldLength,  
- int lengthAdjustment, int initialBytesToStrip, boolean failFast) {  
- super(maxFrameLength, lengthFieldOffset, lengthFieldLength,  
-                 lengthAdjustment, initialBytesToStrip, failFast);  
-     }  
- 
- @Override
- protected Object decode(ChannelHandlerContext ctx, ByteBuf in) throws Exception {  
- if (in == null) {  
- returnnull;  
-         }  
- if (in.readableBytes() < HEADER_SIZE) {  
- thrownew Exception("可读信息段比头部信息都小，你在逗我？");  
-         }  
- 
- //注意在读的过程中，readIndex的指针也在移动
-         type = in.readByte();  
- 
-         flag = in.readByte();  
- 
-         length = in.readInt();  
- 
- if (in.readableBytes() < length) {  
- thrownew Exception("body字段你告诉我长度是"+length+",但是真实情况是没有这么多，你又逗我？");  
-         }  
-         ByteBuf buf = in.readBytes(length);  
- byte[] req = newbyte[buf.readableBytes()];  
-         buf.readBytes(req);  
-         body = new String(req, "UTF-8");  
- 
-         CustomMsg customMsg = new CustomMsg(type,flag,length,body);  
- return customMsg;  
-     }  
- 
- }  
头部信息的大小我们这边写的是6，原因在代码里面也解释了，byte是一个字节，int是四个字节，那么头部大小就是6个字节，接下来就是要定义构造函数了，构造函数的入参的解释代码里已经标注了，我们真实的入参是：
![](https://img-blog.csdn.net/20160511094150588?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
稍微解释一下：
1）LENGTH_FIELD_LENGTH指的就是我们这边CustomMsg中length这个属性的大小，我们这边是int型，所以是4
2）LENGTH_FIELD_OFFSET指的就是我们这边length字段的起始位置，因为前面有type和flag两个属性，且这两个属性都是byte，两个就是2字节，所以偏移量是2
3）LENGTH_ADJUSTMENT指的是length这个属性的值，假如我们的body长度是40，有时候，有些人喜欢将length写成44，因为length本身还占有4个字节，这样就需要调整一下，那么就需要-4，我们这边没有这样做，所以写0就可以了
好了，以下给出完整的代码：
CustomServer.[Java](http://lib.csdn.net/base/javaee)
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51371595#)[copy](http://blog.csdn.net/linuu/article/details/51371595#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1679598/fork)
- package com.lyncc.netty.codec.lengthFieldBasedFrame;  
- 
- import io.netty.bootstrap.ServerBootstrap;  
- import io.netty.channel.ChannelFuture;  
- import io.netty.channel.ChannelInitializer;  
- import io.netty.channel.ChannelOption;  
- import io.netty.channel.EventLoopGroup;  
- import io.netty.channel.nio.NioEventLoopGroup;  
- import io.netty.channel.socket.SocketChannel;  
- import io.netty.channel.socket.nio.NioServerSocketChannel;  
- 
- import java.net.InetSocketAddress;  
- 
- publicclass CustomServer {  
- 
- privatestaticfinalint MAX_FRAME_LENGTH = 1024 * 1024;  
- privatestaticfinalint LENGTH_FIELD_LENGTH = 4;  
- privatestaticfinalint LENGTH_FIELD_OFFSET = 2;  
- privatestaticfinalint LENGTH_ADJUSTMENT = 0;  
- privatestaticfinalint INITIAL_BYTES_TO_STRIP = 0;  
- 
- privateint port;  
- 
- public CustomServer(int port) {  
- this.port = port;  
-     }  
- 
- publicvoid start(){  
-         EventLoopGroup bossGroup = new NioEventLoopGroup(1);  
-         EventLoopGroup workerGroup = new NioEventLoopGroup();  
- try {  
-             ServerBootstrap sbs = new ServerBootstrap().group(bossGroup,workerGroup).channel(NioServerSocketChannel.class).localAddress(new InetSocketAddress(port))  
-                     .childHandler(new ChannelInitializer<SocketChannel>() {  
- 
- protectedvoid initChannel(SocketChannel ch) throws Exception {  
-                              ch.pipeline().addLast(new CustomDecoder(MAX_FRAME_LENGTH,LENGTH_FIELD_LENGTH,LENGTH_FIELD_OFFSET,LENGTH_ADJUSTMENT,INITIAL_BYTES_TO_STRIP,false));  
-                              ch.pipeline().addLast(new CustomServerHandler());  
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
- 
- publicstaticvoid main(String[] args) throws Exception {  
- int port;  
- if (args.length > 0) {  
-             port = Integer.parseInt(args[0]);  
-         } else {  
-             port = 8080;  
-         }  
- new CustomServer(port).start();  
-     }  
- }  
CustomServerHandler.java
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51371595#)[copy](http://blog.csdn.net/linuu/article/details/51371595#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1679598/fork)
- package com.lyncc.netty.codec.lengthFieldBasedFrame;  
- 
- import io.netty.channel.ChannelHandlerContext;  
- import io.netty.channel.SimpleChannelInboundHandler;  
- 
- publicclass CustomServerHandler extends SimpleChannelInboundHandler<Object> {  
- 
- @Override
- protectedvoid channelRead0(ChannelHandlerContext ctx, Object msg) throws Exception {  
- if(msg instanceof CustomMsg) {  
-             CustomMsg customMsg = (CustomMsg)msg;  
-             System.out.println("Client->Server:"+ctx.channel().remoteAddress()+" send "+customMsg.getBody());  
-         }  
- 
-     }  
- 
- }  
CustomClient.java
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51371595#)[copy](http://blog.csdn.net/linuu/article/details/51371595#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1679598/fork)
- package com.lyncc.netty.codec.lengthFieldBasedFrame;  
- 
- import io.netty.bootstrap.Bootstrap;  
- import io.netty.channel.ChannelFuture;  
- import io.netty.channel.ChannelInitializer;  
- import io.netty.channel.ChannelOption;  
- import io.netty.channel.EventLoopGroup;  
- import io.netty.channel.nio.NioEventLoopGroup;  
- import io.netty.channel.socket.SocketChannel;  
- import io.netty.channel.socket.nio.NioSocketChannel;  
- 
- publicclass CustomClient {  
- 
- staticfinal String HOST = System.getProperty("host", "127.0.0.1");  
- staticfinalint PORT = Integer.parseInt(System.getProperty("port", "8080"));  
- staticfinalint SIZE = Integer.parseInt(System.getProperty("size", "256"));  
- 
- publicstaticvoid main(String[] args) throws Exception {  
- 
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
-                      ch.pipeline().addLast(new CustomEncoder());  
-                      ch.pipeline().addLast(new CustomClientHandler());  
-                  }  
-              });  
- 
-             ChannelFuture future = b.connect(HOST, PORT).sync();  
-             future.channel().writeAndFlush("Hello Netty Server ,I am a common client");  
-             future.channel().closeFuture().sync();  
-         } finally {  
-             group.shutdownGracefully();  
-         }  
-     }  
- 
- }  
CustomClientHandler.java
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51371595#)[copy](http://blog.csdn.net/linuu/article/details/51371595#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1679598/fork)
- package com.lyncc.netty.codec.lengthFieldBasedFrame;  
- 
- import io.netty.channel.ChannelHandlerContext;  
- import io.netty.channel.ChannelInboundHandlerAdapter;  
- 
- publicclass CustomClientHandler extends ChannelInboundHandlerAdapter {  
- 
- @Override
- publicvoid channelActive(ChannelHandlerContext ctx) throws Exception {  
-         CustomMsg customMsg = new CustomMsg((byte)0xAB, (byte)0xCD, "Hello,Netty".length(), "Hello,Netty");  
-         ctx.writeAndFlush(customMsg);  
-     }  
- 
- }  
最最重要的就是两个译码器：
CustomDecoder.java
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51371595#)[copy](http://blog.csdn.net/linuu/article/details/51371595#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1679598/fork)
- package com.lyncc.netty.codec.lengthFieldBasedFrame;  
- 
- import io.netty.buffer.ByteBuf;  
- import io.netty.channel.ChannelHandlerContext;  
- import io.netty.handler.codec.LengthFieldBasedFrameDecoder;  
- 
- publicclass CustomDecoder extends LengthFieldBasedFrameDecoder {  
- 
- //判断传送客户端传送过来的数据是否按照协议传输，头部信息的大小应该是 byte+byte+int = 1+1+4 = 6
- privatestaticfinalint HEADER_SIZE = 6;  
- 
- privatebyte type;  
- 
- privatebyte flag;  
- 
- privateint length;  
- 
- private String body;  
- 
- /**
-      * 
-      * @param maxFrameLength 解码时，处理每个帧数据的最大长度
-      * @param lengthFieldOffset 该帧数据中，存放该帧数据的长度的数据的起始位置
-      * @param lengthFieldLength 记录该帧数据长度的字段本身的长度
-      * @param lengthAdjustment 修改帧数据长度字段中定义的值，可以为负数
-      * @param initialBytesToStrip 解析的时候需要跳过的字节数
-      * @param failFast 为true，当frame长度超过maxFrameLength时立即报TooLongFrameException异常，为false，读取完整个帧再报异常
-      */
- public CustomDecoder(int maxFrameLength, int lengthFieldOffset, int lengthFieldLength,  
- int lengthAdjustment, int initialBytesToStrip, boolean failFast) {  
- super(maxFrameLength, lengthFieldOffset, lengthFieldLength,  
-                 lengthAdjustment, initialBytesToStrip, failFast);  
-     }  
- 
- @Override
- protected Object decode(ChannelHandlerContext ctx, ByteBuf in) throws Exception {  
- if (in == null) {  
- returnnull;  
-         }  
- if (in.readableBytes() < HEADER_SIZE) {  
- thrownew Exception("可读信息段比头部信息都小，你在逗我？");  
-         }  
- 
- //注意在读的过程中，readIndex的指针也在移动
-         type = in.readByte();  
- 
-         flag = in.readByte();  
- 
-         length = in.readInt();  
- 
- if (in.readableBytes() < length) {  
- thrownew Exception("body字段你告诉我长度是"+length+",但是真实情况是没有这么多，你又逗我？");  
-         }  
-         ByteBuf buf = in.readBytes(length);  
- byte[] req = newbyte[buf.readableBytes()];  
-         buf.readBytes(req);  
-         body = new String(req, "UTF-8");  
- 
-         CustomMsg customMsg = new CustomMsg(type,flag,length,body);  
- return customMsg;  
-     }  
- 
- }  
CustomEncoder.java
**[java]**[view
 plain](http://blog.csdn.net/linuu/article/details/51371595#)[copy](http://blog.csdn.net/linuu/article/details/51371595#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1679598/fork)
- package com.lyncc.netty.codec.lengthFieldBasedFrame;  
- 
- import java.nio.charset.Charset;  
- 
- import io.netty.buffer.ByteBuf;  
- import io.netty.channel.ChannelHandlerContext;  
- import io.netty.handler.codec.MessageToByteEncoder;  
- 
- publicclass CustomEncoder extends MessageToByteEncoder<CustomMsg> {  
- 
- @Override
- protectedvoid encode(ChannelHandlerContext ctx, CustomMsg msg, ByteBuf out) throws Exception {  
- if(null == msg){  
- thrownew Exception("msg is null");  
-         }  
- 
-         String body = msg.getBody();  
- byte[] bodyBytes = body.getBytes(Charset.forName("utf-8"));  
-         out.writeByte(msg.getType());  
-         out.writeByte(msg.getFlag());  
-         out.writeInt(bodyBytes.length);  
-         out.writeBytes(bodyBytes);  
- 
-     }  
- 
- }  
好了，到此为止代码就全部写完了，运行测试一下：
启动服务器端：
![](https://img-blog.csdn.net/20160511101732683?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
运行客户端后，再回到服务器端的控制台：
![](https://img-blog.csdn.net/20160511101801355?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

