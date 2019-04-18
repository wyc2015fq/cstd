# Java NIO使用及原理分析 (四) - z69183787的专栏 - CSDN博客
2014年04月14日 10:35:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1021
转载自：[李会军•宁静致远](http://terrylee.me/blog/post/2010/10/01/java-nio-internal-part1.aspx)
在上一篇文章中介绍了关于缓冲区的一些细节内容，现在终于可以进入NIO中最有意思的部分非阻塞I/O。通常在进行同步I/O操作时，如果读取数据，代码会阻塞直至有 可供读取的数据。同样，写入调用将会阻塞直至数据能够写入。传统的Server/Client模式会基于TPR（Thread per Request）,服务器会为每个客户端请求建立一个线程，由该线程单独负责处理一个客户请求。这种模式带来的一个问题就是线程数量的剧增，大量的线程会增大服务器的开销。大多数的实现为了避免这个问题，都采用了线程池模型，并设置线程池线程的最大数量，这由带来了新的问题，如果线程池中有200个线程，而有200个用户都在进行大文件下载，会导致第201个用户的请求无法及时处理，即便第201个用户只想请求一个几KB大小的页面。传统的
 Server/Client模式如下图所示：
![](http://hi.csdn.net/attachment/201107/17/0_131088978763g8.gif)
NIO中非阻塞I/O采用了基于Reactor模式的工作方式，I/O调用不会被阻塞，相反是注册感兴趣的特定I/O事件，如可读数据到达，新的套接字连接等等，在发生特定事件时，系统再通知我们。NIO中实现非阻塞I/O的核心对象就是Selector，Selector就是注册各种I/O事件地 方，而且当那些事件发生时，就是这个对象告诉我们所发生的事件，如下图所示：
![](http://hi.csdn.net/attachment/201107/17/0_1310889791hy91.gif)
从图中可以看出，当有读或写等任何注册的事件发生时，可以从Selector中获得相应的SelectionKey，同时从 SelectionKey中可以找到发生的事件和该事件所发生的具体的SelectableChannel，以获得客户端发送过来的数据。关于 SelectableChannel的可以参考[Java
 NIO使用及原理分析（一）](http://blog.csdn.net/wuxianglong/article/details/6604817)
使用NIO中非阻塞I/O编写服务器处理程序，大体上可以分为下面三个步骤：
1. 向Selector对象注册感兴趣的事件 
2. 从Selector中获取感兴趣的事件 
3. 根据不同的事件进行相应的处理
接下来我们用一个简单的示例来说明整个过程。首先是向Selector对象注册感兴趣的事件：
**[java]**[view
 plain](http://blog.csdn.net/wuxianglong/article/details/6612282#)[copy](http://blog.csdn.net/wuxianglong/article/details/6612282#)[print](http://blog.csdn.net/wuxianglong/article/details/6612282#)[?](http://blog.csdn.net/wuxianglong/article/details/6612282#)
- /*
-  * 注册事件
-  * */
- protected Selector getSelector() throws IOException {  
- // 创建Selector对象
-     Selector sel = Selector.open();  
- 
- // 创建可选择通道，并配置为非阻塞模式
-     ServerSocketChannel server = ServerSocketChannel.open();  
-     server.configureBlocking(false);  
- 
- // 绑定通道到指定端口
-     ServerSocket socket = server.socket();  
-     InetSocketAddress address = new InetSocketAddress(port);  
-     socket.bind(address);  
- 
- // 向Selector中注册感兴趣的事件
-     server.register(sel, SelectionKey.OP_ACCEPT);   
- return sel;  
- }  
创建了ServerSocketChannel对象，并调用configureBlocking()方法，配置为非阻塞模式，接下来的三行代码把该通道绑定到指定端口，最后向Selector中注册事件，此处指定的是参数是OP_ACCEPT，即指定我们想要监听accept事件，也就是新的连接发 生时所产生的事件，对于ServerSocketChannel通道来说，我们唯一可以指定的参数就是OP_ACCEPT。
从Selector中获取感兴趣的事件，即开始监听，进入内部循环：
**[java]**[view
 plain](http://blog.csdn.net/wuxianglong/article/details/6612282#)[copy](http://blog.csdn.net/wuxianglong/article/details/6612282#)[print](http://blog.csdn.net/wuxianglong/article/details/6612282#)[?](http://blog.csdn.net/wuxianglong/article/details/6612282#)
- /*
-  * 开始监听
-  * */
- publicvoid listen() {   
-     System.out.println("listen on " + port);  
- try {   
- while(true) {   
- // 该调用会阻塞，直到至少有一个事件发生
-             selector.select();   
-             Set<SelectionKey> keys = selector.selectedKeys();  
-             Iterator<SelectionKey> iter = keys.iterator();  
- while (iter.hasNext()) {   
-                 SelectionKey key = (SelectionKey) iter.next();   
-                 iter.remove();   
-                 process(key);   
-             }   
-         }   
-     } catch (IOException e) {   
-         e.printStackTrace();  
-     }   
- }  
在非阻塞I/O中，内部循环模式基本都是遵循这种方式。首先调用select()方法，该方法会阻塞，直到至少有一个事件发生，然后再使用selectedKeys()方法获取发生事件的SelectionKey，再使用迭代器进行循环。
最后一步就是根据不同的事件，编写相应的处理代码：
**[java]**[view
 plain](http://blog.csdn.net/wuxianglong/article/details/6612282#)[copy](http://blog.csdn.net/wuxianglong/article/details/6612282#)[print](http://blog.csdn.net/wuxianglong/article/details/6612282#)[?](http://blog.csdn.net/wuxianglong/article/details/6612282#)
- /*
-  * 根据不同的事件做处理
-  * */
- protectedvoid process(SelectionKey key) throws IOException{  
- // 接收请求
- if (key.isAcceptable()) {  
-         ServerSocketChannel server = (ServerSocketChannel) key.channel();  
-         SocketChannel channel = server.accept();  
-         channel.configureBlocking(false);  
-         channel.register(selector, SelectionKey.OP_READ);  
-     }  
- // 读信息
- elseif (key.isReadable()) {  
-         SocketChannel channel = (SocketChannel) key.channel();   
- int count = channel.read(buffer);   
- if (count > 0) {   
-             buffer.flip();   
-             CharBuffer charBuffer = decoder.decode(buffer);   
-             name = charBuffer.toString();   
-             SelectionKey sKey = channel.register(selector, SelectionKey.OP_WRITE);   
-             sKey.attach(name);   
-         } else {   
-             channel.close();   
-         }   
-         buffer.clear();   
-     }  
- // 写事件
- elseif (key.isWritable()) {  
-         SocketChannel channel = (SocketChannel) key.channel();   
-         String name = (String) key.attachment();   
- 
-         ByteBuffer block = encoder.encode(CharBuffer.wrap("Hello " + name));   
- if(block != null)  
-         {  
-             channel.write(block);  
-         }  
- else
-         {  
-             channel.close();  
-         }  
- 
-      }  
- }  
此处分别判断是接受请求、读数据还是写事件，分别作不同的处理。
到这里关于Java NIO使用及原理分析的四篇文章就全部完成了。Java NIO提供了通道、缓冲区、选择器这样一组抽象概念，极大的简化了我们编写高性能并发型服务器程序，后面有机会我会继续谈谈使用Java NIO的一些想法。
**(完)**
