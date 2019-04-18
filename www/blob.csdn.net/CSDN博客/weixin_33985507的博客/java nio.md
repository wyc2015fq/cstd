# java nio - weixin_33985507的博客 - CSDN博客
2016年10月18日 22:59:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
Java NIO提供了与标准IO不同的IO工作方式： 
- - **Channels and Buffers（通道和缓冲区）**：标准的IO基于字节流和字符流进行操作的，而NIO是基于通道（Channel）和缓冲区（Buffer）进行操作，数据总是从通道读取到缓冲区中，或者从缓冲区写入到通道中。
- **Asynchronous IO（异步IO）**：Java NIO可以让你异步的使用IO，例如：当线程从通道读取数据到缓冲区时，线程还是可以进行其他事情。当数据被写入到缓冲区时，线程可以继续处理它。从缓冲区写入通道也类似。
- **Selectors（选择器）**：Java NIO引入了选择器的概念，选择器用于监听多个通道的事件（比如：连接打开，数据到达）。因此，单个的线程可以监听多个数据通道。
- 
- 
- Java NIO 由以下几个核心部分组成： 
- Channels
- Buffers
- Selectors
Channel和Buffer有好几种类型。下面是JAVA NIO中的一些主要Channel的实现： 
- FileChannel
- DatagramChannel
- SocketChannel
- ServerSocketChannel
正如你所看到的，这些通道涵盖了UDP 和 TCP 网络IO，以及文件IO。 
- 以下是Java NIO里关键的Buffer实现： 
- ByteBuffer
- CharBuffer
- DoubleBuffer
- FloatBuffer
- IntBuffer
- LongBuffer
- ShortBuffer
这些Buffer覆盖了你能通过IO发送的基本数据类型：byte, short, int, long, float, double 和 char。 
- **Selector**
Selector允许单线程处理多个 Channel。如果你的应用打开了多个连接（通道），但每个连接的流量都很低，使用Selector就会很方便。例如，在一个聊天服务器中。 
这是在一个单线程中使用一个Selector处理3个Channel的图示： 
![](http://dl2.iteye.com/upload/attachment/0096/3972/79224e12-3615-3917-9e85-42e7edbd8b40.png)
要使用Selector，得向Selector注册Channel，然后调用它的select()方法。这个方法会一直阻塞到某个注册的通道有事件就绪。一旦这个方法返回，线程就可以处理这些事件，事件的例子有如新连接进来，数据接收等。 
- **Java NIO和IO的主要区别**
下表总结了Java NIO和IO之间的主要差别，我会更详细地描述表中每部分的差异。 
|IO|NIO|
|----|----|
|Stream oriented|Buffer oriented|
|Blocking IO|Non blocking IO|
||Selectors|
**面向流与面向缓冲**
Java NIO和IO之间第一个最大的区别是，IO是面向流的，NIO是面向缓冲区的。 Java IO面向流意味着每次从流中读一个或多个字节，直至读取所有字节，它们没有被缓存在任何地方。此外，它不能前后移动流中的数据。如果需要前后移动从流中读取的数据，需要先将它缓存到一个缓冲区。 Java NIO的缓冲导向方法略有不同。数据读取到一个它稍后处理的缓冲区，需要时可在缓冲区中前后移动。这就增加了处理过程中的灵活性。但是，还需要检查是否该缓冲区中包含所有您需要处理的数据。而且，需确保当更多的数据读入缓冲区时，不要覆盖缓冲区里尚未处理的数据。 
- **阻塞与非阻塞IO**
Java IO的各种流是阻塞的。这意味着，当一个线程调用read() 或 write()时，该线程被阻塞，直到有一些数据被读取，或数据完全写入。该线程在此期间不能再干任何事情了。 Java NIO的非阻塞模式，使一个线程从某通道发送请求读取数据，但是它仅能得到目前可用的数据，如果目前没有数据可用时，就什么都不会获取。而不是保持线程阻塞，所以直至数据变的可以读取之前，该线程可以继续做其他的事情。 非阻塞写也是如此。一个线程请求写入一些数据到某通道，但不需要等待它完全写入，这个线程同时可以去做别的事情。 线程通常将非阻塞IO的空闲时间用于在其它通道上执行IO操作，所以一个单独的线程现在可以管理多个输入和输出通道（channel）。 
- 
Java代码 
- InputStream input = … ; // get the InputStream from the client socket  
- BufferedReader reader = new BufferedReader(new InputStreamReader(input));  
- 
- String nameLine   = reader.readLine();  
- String ageLine    = reader.readLine();  
- String emailLine  = reader.readLine();  
- String phoneLine  = reader.readLine();  
请注意处理状态由程序执行多久决定。换句话说，一旦reader.readLine()方法返回，你就知道肯定文本行就已读完， readline()阻塞直到整行读完，这就是原因。你也知道此行包含名称；同样，第二个readline()调用返回的时候，你知道这行包含年龄等。 正如你可以看到，该处理程序仅在有新数据读入时运行，并知道每步的数据是什么。
