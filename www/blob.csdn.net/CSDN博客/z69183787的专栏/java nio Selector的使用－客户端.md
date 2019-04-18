# java nio Selector的使用－客户端 - z69183787的专栏 - CSDN博客
2014年04月14日 11:22:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1018
个人分类：[IO-NIO](https://blog.csdn.net/z69183787/article/category/2191483)
### [java nio Selector的使用－客户端](http://writeblog.csdn.net/blog/392373)
　　接上一篇，客户端的程序就相对于简单了，只需要负责连接，发送下载文件名，再读数据就行了。主要步骤就是注册->连接服务器->发送下载请求->读数据->断开连接。
　　第一步：注册，并注册connect事件。
Java代码 ![复制代码](http://flym.javaeye.com/images/icon_copy.gif)
- **if**(selector == **null**)
   
-     selector = Selector.open();   
- SocketChannel channel = SocketChannel.open();   
- channel.configureBlocking(**false**);
   
- channel.connect(**new** InetSocketAddress("localhost", 1234));
   
- channel.register(selector, SelectionKey.OP_CONNECT);  
**[java]**[view
 plain](http://blog.csdn.net/kangojian/article/details/5711358#)[copy](http://blog.csdn.net/kangojian/article/details/5711358#)
- if(selector == null)  
-     selector = Selector.open();  
- SocketChannel channel = SocketChannel.open();  
- channel.configureBlocking(false);  
- channel.connect(new InetSocketAddress("localhost", 1234));  
- channel.register(selector, SelectionKey.OP_CONNECT);  
　　以上步骤，在实际中发现configure和connet并不能随意交换位置，即不能将connect放在configureBlocking的前面，更不能在open中直接添加InetSocketAddress参数了。在官方doc中，open(InetAddress)的解释是“这种便捷方法的工作方式就像以下过程一样：调用 open() 方法、在得到的套接字通道上调用 connect 方法、向其传递 remote，然后返回该通道。”，但不知道为什么在connect之后，就不能配置block了，导致无法进行数据下载和通信。
　　第二步：处理connect事件
Java代码 ![复制代码](http://flym.javaeye.com/images/icon_copy.gif)
- //连接事件 
- **if**(key.isConnectable()) {
   
-     SocketChannel socketChannel = (SocketChannel) key.channel();   
- **if**(socketChannel.isConnectionPending())
   
-         socketChannel.finishConnect();   
-     socketChannel.write(ByteBuffer.wrap(serverFileName.getBytes()));//向服务器发信息,信息中即服务器上的文件名 
-     socketChannel.register(selector, SelectionKey.OP_READ);   
- }  
**[java]**[view
 plain](http://blog.csdn.net/kangojian/article/details/5711358#)[copy](http://blog.csdn.net/kangojian/article/details/5711358#)
- //连接事件
- if(key.isConnectable()) {  
-     SocketChannel socketChannel = (SocketChannel) key.channel();  
- if(socketChannel.isConnectionPending())  
-         socketChannel.finishConnect();  
-     socketChannel.write(ByteBuffer.wrap(serverFileName.getBytes()));//向服务器发信息,信息中即服务器上的文件名
-     socketChannel.register(selector, SelectionKey.OP_READ);  
- }  
　　在以上步骤中，要完成连接之后，向服务器端发送了下载的文件名的数据信息，并注册read事件。服务器端在接收到相应文件名之后，就开启write事件向客户端进行发送数据了，客户端此时就可以进行数据读取了。
　　第三步：处理read事件
Java代码 ![复制代码](http://flym.javaeye.com/images/icon_copy.gif)
- **if**(key.isReadable()) {
   
-     SocketChannel socketChannel = (SocketChannel) key.channel();   
-     byteBuffer.clear();   
- **if**(!socketChannel.isConnected())
   
- **return****null**;
   
- //向本机下载文件创建文件channel 
- **if**(fileChannel == **null**)
   
-         fileChannel = **new** RandomAccessFile(localFileName, "rw").getChannel();
   
- **int** r = socketChannel.read(byteBuffer);
   
- //如果文件下载完毕,则关掉channel,同时关掉socketChannel 
- **if**(r <= 0) {
   
- **if**(fileChannel != **null**)
   
-             fileChannel.close();   
-         channel.close();   
-         key.cancel();   
- **return****null**;
   
-     }   
-     byteBuffer.flip();   
- //写到下载文件中 
-     fileChannel.write(byteBuffer);   
- }  
**[java]**[view
 plain](http://blog.csdn.net/kangojian/article/details/5711358#)[copy](http://blog.csdn.net/kangojian/article/details/5711358#)
- if(key.isReadable()) {  
-     SocketChannel socketChannel = (SocketChannel) key.channel();  
-     byteBuffer.clear();  
- if(!socketChannel.isConnected())  
- returnnull;  
- //向本机下载文件创建文件channel
- if(fileChannel == null)  
-         fileChannel = new RandomAccessFile(localFileName, "rw").getChannel();  
- int r = socketChannel.read(byteBuffer);  
- //如果文件下载完毕,则关掉channel,同时关掉socketChannel
- if(r <= 0) {  
- if(fileChannel != null)  
-             fileChannel.close();  
-         channel.close();  
-         key.cancel();  
- returnnull;  
-     }  
-     byteBuffer.flip();  
- //写到下载文件中
-     fileChannel.write(byteBuffer);  
- }  
　　就是处理读信息，如果数据已经读取完毕，则完成相应下载保存文件的文件流，并退出程序。
　　这样，整个客户端就完成了，在运行时，我同时启用１０个线程来向服务器端读同一个文件，并保存为不同的文件备份，以达到模拟数据传输的功能。如下所示：
Java代码 ![复制代码](http://flym.javaeye.com/images/icon_copy.gif)
- ExecutorService executorService = Executors.newSingleThreadExecutor();   
- **for**(**int** i = 0; i < 10; i++) {
   
-     executorService.submit(**new** DownloadClient<Object>("d:/log4j.log", "d:/down" + i + ".log"));
   
- }   
- executorService.shutdown();  
**[java]**[view
 plain](http://blog.csdn.net/kangojian/article/details/5711358#)[copy](http://blog.csdn.net/kangojian/article/details/5711358#)
- ExecutorService executorService = Executors.newSingleThreadExecutor();  
- for(int i = 0; i < 10; i++) {  
-     executorService.submit(new DownloadClient<Object>("d:/log4j.log", "d:/down" + i + ".log"));  
- }  
- executorService.shutdown();  
　　整个selector仅是一个作为练习用的小例子，如果用在实际代码中，还需要处理不同的异常和相应的逻辑等。对于学习还是有一定的帮助的。希望对你有用。
