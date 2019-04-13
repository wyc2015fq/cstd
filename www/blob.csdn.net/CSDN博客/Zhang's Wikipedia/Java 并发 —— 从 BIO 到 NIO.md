
# Java 并发 —— 从 BIO 到 NIO - Zhang's Wikipedia - CSDN博客


2018年09月01日 22:17:29[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：199个人分类：[并发](https://blog.csdn.net/lanchunhui/article/category/7071346)



BIO，同步阻塞；NIO：同步非阻塞；
在 NIO 技术之前，服务器侧使用多线程是 BIO 的一种伪阻塞的解决方法；
BIO 的多线程机制存在的问题：
每一个 Socket 连接服务器，服务器侧都立刻开启（创建）一个线程处理，
Socket socket =server.accept();while(true) {newThread(newServerHandler(socket));
}每个 IO 操作完成后，线程就会销毁（IO 线程可以理解为一个数据传输的通道，channel），而线程的创建是比较耗费资源的，奈何取之尽锱铢，而用之如泥沙；
NIO 针对这 BIO 的这两个问题，进行如下的改进：
新的 Socket 到来，并不立即开启线程，等到 IO 操作再开启；
这便是 java.nio 的 Selector 选择子（以线程的形式存在），定义三种状态：ACCEPT，Readable，Writable
publicstaticfinalintOP_READ =1<<0;publicstaticfinalintOP_WRITE =1<<2;publicstaticfinalintOP_CONNECT =1<<3;publicstaticfinalintOP_ACCEPT =1<<4;每个IO操作完成，也不立即销毁线程；
这便是 java.nio 的 Channel；
BIO 的流式读写，不能同时读写。为了支持同时读写，变阻塞为非阻塞，java.nio 提出 Buffer 的概念；
1. buffer 机制
[Java NIO Buffer](http://actimem.com/java/nio/buffer/)
三大概念：
capacity容量
position位置
limit限制
在写模式，limit的含义是我们所能写入的最大数据量。它等同于buffer的容量。
一旦切换到读模式，limit则代表我们所能读取的最大数据量，他的值等同于写模式下position的位置。
数据读取的上限时buffer中已有的数据，也就是limit的位置（原position所指的位置）。

java nio 下的 Buffer Types：
ByteBuffer
MappedByteBuffer
CharBuffer
DoubleBuffer
FloatBuffer
IntBuffer
LongBuffer
ShortBuffer


