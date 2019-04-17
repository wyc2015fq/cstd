# JavaNIO：非阻塞NIO通信及相关对象讲解 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年09月13日 19:42:40[boonya](https://me.csdn.net/boonya)阅读数：1000








### 1.ServerSocketChannel



ServerSocketChannel是一个基于通道的socket监听器。它同我们所熟悉的java.net.ServerSocket执行相同的基本任务，不过它增加了通道语义，因此能够在非阻塞模式下运行。它可用静态的open( )工厂方法创建一个新的ServerSocketChannel对象，将会返回同一个未绑定的java.net.ServerSocket关联的通道。该对等ServerSocket可以通过在返回的ServerSocketChannel上调用socket( )方法来获取。


在ServerSocket上调用accept( )方法，那么它会同任何其他的ServerSocket表现一样的行为：总是阻塞并返回一个java.net.Socket对象(阻塞的！！！！)

在ServerSocketChannel上调用accept( )方法则会返回SocketChannel类型的对象，返回的对象能够在非阻塞模式下运行。





如果以非阻塞模式被调用，当没有传入连接在等待时，ServerSocketChannel.accept( )会立即返回null (因为他是非阻塞的所以要有返回，)。正是这种检查连接而不阻塞的能力实现了可伸缩性并降低了复杂性。可选择性也因此得到实现。我们可以使用一个选择器实例来注册一个ServerSocketChannel对象以实现新连接到达时自动通知的功能。

**初始化ServerSocketChannel**



```java
/**
	 * 初始化ServerSocketChannel
	 * @throws Exception
	 */
	public void initChannel() throws Exception{
		channel = ServerSocketChannel.open();
		channel.socket().bind(new InetSocketAddress(port));
		channel.configureBlocking(false);//非阻塞模式
		
		selector = Selector.open();
		channel.register(selector, SelectionKey.OP_ACCEPT);
		System.out.println("NIO TCP Push Listener nio provider: "+selector.provider().getClass().getCanonicalName());
	}
```



### **2.SocketChannel vs. ServerSocketChannel**
- 父类：SelectableChannel。Channel表现了一个可以进行IO操作的通道（比如，通过FileChannel，我们可以对文件进行读写操作）
- ServerSocketChannel主要用在Server中，用于接收客户端的链接请求 ；

SocketChannel则用于真正的读写数据，同时还可以用于客户端发送链接请求。 
- 真正实现读写数据操作的就是这些SocketChannel，上面的ServerSocketChannel只是负责接收连接请求。
- 以下均简称为channel

#### **2.1channel vs. Selector**
- channel需要注册到selector上。channel可以注册到一个或多个Selector上以进行异步IO操作。



       channel.register(selector, SelectionKey.OP_ACCEPT);

channel.register(selector, xxx, object); //attachment被存放在返回的SelectionKey中

channel.keyFor(selector); //返回该channe在Selector上的注册关系所对应的SelectionKey。若无注册关系，返回null。
- Selector可以同时监控多个SelectableChannel的IO状况，是异步IO的核心。

Selector.open(); //静态方法，创建一个selector实例

       selector.select(); //selector通过调用select()，将注册的channel中有事件发生的取出来进行处理。监控所有注册的channel，当其中有注册的IO操作可以进行时，该函数返回，并将对应的SelectionKey加入selected-key set。

       selector.keys(); //所有注册在这个Selector上的channel

       selector.selectedKeys(); //所有通过select()方法监测到可以进行IO操作的channel

#### **2.2.SelectionKey**
- 代表了Selector和SelectableChannel的注册关系

       key.attachment(); //返回SelectionKey的attachment，attachment可以在注册channel的时候指定。

       key.channel(); // 返回该SelectionKey对应的channel。

       key.selector(); // 返回该SelectionKey对应的Selector。

key.interestOps(); //返回代表需要Selector监控的IO操作的bit mask

       key.readyOps(); //返回一个bit mask，代表在相应channel上可以进行的IO操作。

#### 2.3Channel,Selector &SelectionKey示例代码



```java
package com.boonya.nio;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.channels.spi.SelectorProvider;
import java.util.Iterator;
import java.util.Set;

public class JustPartCode {

	String host = "192.168.1.137";

	int port = 9901;

	SocketChannel channel;

	Selector selector = null;
	
	/**
	 * 为Selector注册channel
	 * @throws IOException
	 */
	public void initSelector() throws IOException{
		selector=Selector.open();
		channel.register(selector, SelectionKey.OP_ACCEPT);
	}

	/**
	 * Server端处理Selector注册的事件
	 * 
	 * @throws IOException
	 */
	public void handEvent() throws IOException {
		// 得到selector所捕获的事件数量
		int n = selector.select();
		// 当真正捕获到事件时，才执行相应操作
		if (n > 0) {
			// 获取捕获到的事件集合
			Set selectedKeys = selector.selectedKeys();
			Iterator i = selectedKeys.iterator();
			while (i.hasNext()) {
				// 对事件一一处理
				SelectionKey s = (SelectionKey) i.next();
				// 一个key被处理完成后，就都被从就绪关键字（ready keys）列表中除去
				i.remove();
				if (s.isAcceptable()) // 表示该事件为OP_ACCEPT事件
				{
					// 从channel()中取得我们刚刚注册的ServerSocketChannel
					// 为请求获取新的SocketChannel
					SocketChannel sc = ((ServerSocketChannel) s.channel())
							.accept().socket().getChannel();
					// 设置SocketChannel为非阻塞方式
					sc.configureBlocking(false);
					// 将新的SocketChannel注册到selector中，注册事件为OP_READ和OP_WRITE
					sc.register(selector, SelectionKey.OP_READ
							| SelectionKey.OP_WRITE);

				} else {
					// 执行其他操作
				}
			}
		}
	}

	/**
	 * Server端读取通道数据
	 * 
	 * @throws IOException
	 */
	public void readDataFromChannel() throws IOException {
		// 获取捕获到的事件集合
		Set selectedKeys = selector.selectedKeys();
		Iterator i = selectedKeys.iterator();
		while (i.hasNext()) {
			// 对事件一一处理
			SelectionKey key = (SelectionKey) i.next();
			// 一个key被处理完成后，就都被从就绪关键字（ready keys）列表中除去
			i.remove();
			ByteBuffer clientBuffer = ByteBuffer.allocate(4096);
			// 读信息
			if (key.isReadable()) {
				// 获取相应的SocketChannel
				SocketChannel channel = (SocketChannel) key.channel();
				// 将数据读入clientBuffer
				int count = channel.read(clientBuffer);
				// 当有数据读入时
				if (count > 0) {
					// 反转此缓冲区
					clientBuffer.flip();
					// 如果需要，对缓冲区中的字符进行解码
					// CharBuffer charBuffer = decoder.decode(clientBuffer);
				}
			}
		}
	}

	/**
	 * 客户端向服务端发送数据
	 * 
	 * @throws IOException
	 */
	public void clientSendData() throws IOException {
		InetSocketAddress addr = new InetSocketAddress(host, port);
		// 生成一个socketchannel
		channel = SocketChannel.open();
		// 连接到server
		channel.connect(addr);
		// 当连接成功时，执行相应操作
		if (channel.finishConnect()) {
			// 准备数据
			ByteBuffer buffer = ByteBuffer.allocate(1024);
			buffer.put("Hello stupid server".getBytes());
			buffer.flip(); // 反转此缓冲区
			while (buffer.hasRemaining())
				// 发送数据到server
				channel.write(buffer);
		}
	}
}
```



### 3.Selector详解

Selector（选择器）是Java NIO中能够检测一到多个NIO通道，并能够知晓通道是否为诸如读写事件做好准备的组件。这样，一个单独的线程可以管理多个channel，从而管理多个网络连接。



#### 3.1为什么使用Selector?



仅用单个线程来处理多个Channels的好处是，只需要更少的线程来处理通道。事实上，可以只用一个线程处理所有的通道。对于操作系统来说，线程之间上下文切换的开销很大，而且每个线程都要占用系统的一些资源（如内存）。因此，使用的线程越少越好。

但是，需要记住，现代的操作系统和CPU在多任务方面表现的越来越好，所以多线程的开销随着时间的推移，变得越来越小了。实际上，如果一个CPU有多个内核，不使用多任务可能是在浪费CPU能力。不管怎么说，关于那种设计的讨论应该放在另一篇不同的文章中。在这里，只要知道使用Selector能够处理多个通道就足够了。

下面是单线程使用一个Selector处理3个channel的示例图：

#### 3.2Selector的创建

通过调用Selector.open()方法创建一个Selector，如下：




```java
Selector selector = Selector.open();
```




#### 3.3向Selector注册通道

为了将Channel和Selector配合使用，必须将channel注册到selector上。通过SelectableChannel.register()方法来实现，如下：



```java
channel.configureBlocking(false);
SelectionKey key = channel.register(selector,Selectionkey.OP_READ);
```



与Selector一起使用时，Channel必须处于非阻塞模式下。这意味着不能将FileChannel与Selector一起使用，因为FileChannel不能切换到非阻塞模式。而套接字通道都可以。

注意register()方法的第二个参数。这是一个“interest集合”，意思是在通过Selector监听Channel时对什么事件感兴趣。可以监听四种不同类型的事件：
- Connect
- Accept
- Read
- Write

通道触发了一个事件意思是该事件已经就绪。所以，某个channel成功连接到另一个服务器称为“连接就绪”。一个server socket channel准备好接收新进入的连接称为“接收就绪”。一个有数据可读的通道可以说是“读就绪”。等待写数据的通道可以说是“写就绪”。

这四种事件用SelectionKey的四个常量来表示：
- SelectionKey.OP_CONNECT
- SelectionKey.OP_ACCEPT
- SelectionKey.OP_READ
- SelectionKey.OP_WRITE

如果你对不止一种事件感兴趣，那么可以用“位或”操作符将常量连接起来，如下：



|`1`|`int``interestSet = SelectionKey.OP_READ | SelectionKey.OP_WRITE;`|
|----|----|




在下面还会继续提到interest集合。

#### 3.4SelectionKey

在上一小节中，当向Selector注册Channel时，register()方法会返回一个SelectionKey对象。这个对象包含了一些你感兴趣的属性：
- interest集合
- ready集合
- Channel
- Selector
- 附加的对象（可选）

下面我会描述这些属性。

##### 3.4.1interest集合

就像[向Selector注册通道](http://ifeve.com/selectors/#Registering)一节中所描述的，interest集合是你所选择的感兴趣的事件集合。可以通过SelectionKey读写interest集合，像这样：



|`1`|`int``interestSet = selectionKey.interestOps();`|
|----|----|


|`2`||
|----|----|


|`3`|`boolean``isInterestedInAccept  = (interestSet & SelectionKey.OP_ACCEPT) == SelectionKey.OP_ACCEPT；`|
|----|----|


|`4`|`boolean``isInterestedInConnect = interestSet & SelectionKey.OP_CONNECT;`|
|----|----|


|`5`|`boolean``isInterestedInRead    = interestSet & SelectionKey.OP_READ;`|
|----|----|


|`6`|`boolean``isInterestedInWrite   = interestSet & SelectionKey.OP_WRITE;`|
|----|----|




可以看到，用“位与”操作interest 集合和给定的SelectionKey常量，可以确定某个确定的事件是否在interest 集合中。

##### 3.4.2ready集合

ready 集合是通道已经准备就绪的操作的集合。在一次选择(Selection)之后，你会首先访问这个ready set。Selection将在下一小节进行解释。可以这样访问ready集合：



|`1`|`int``readySet = selectionKey.readyOps();`|
|----|----|




可以用像检测interest集合那样的方法，来检测channel中什么事件或操作已经就绪。但是，也可以使用以下四个方法，它们都会返回一个布尔类型：



|`1`|`selectionKey.isAcceptable();`|
|----|----|


|`2`|`selectionKey.isConnectable();`|
|----|----|


|`3`|`selectionKey.isReadable();`|
|----|----|


|`4`|`selectionKey.isWritable();`|
|----|----|




##### 3.4.3Channel + Selector

从SelectionKey访问Channel和Selector很简单。如下：



|`1`|`Channel  channel  = selectionKey.channel();`|
|----|----|


|`2`|`Selector selector = selectionKey.selector();`|
|----|----|




##### 3.4.5附加的对象

可以将一个对象或者更多信息附着到SelectionKey上，这样就能方便的识别某个给定的通道。例如，可以附加 与通道一起使用的Buffer，或是包含聚集数据的某个对象。使用方法如下：



|`1`|`selectionKey.attach(theObject);`|
|----|----|


|`2`|`Object attachedObj = selectionKey.attachment();`|
|----|----|




还可以在用register()方法向Selector注册Channel的时候附加对象。如：



|`1`|`SelectionKey key = channel.register(selector, SelectionKey.OP_READ, theObject);`|
|----|----|




#### 3.5通过Selector选择通道

一旦向Selector注册了一或多个通道，就可以调用几个重载的select()方法。这些方法返回你所感兴趣的事件（如连接、接受、读或写）已经准备就绪的那些通道。换句话说，如果你对“读就绪”的通道感兴趣，select()方法会返回读事件已经就绪的那些通道。

下面是select()方法：
- int select()
- int select(long timeout)
- int selectNow()

`select()`阻塞到至少有一个通道在你注册的事件上就绪了。

`select(long timeout)`和select()一样，除了最长会阻塞timeout毫秒(参数)。

`selectNow()`不会阻塞，不管什么通道就绪都立刻返回（*译者注：此方法执行非阻塞的选择操作。如果自从前一次选择操作后，没有通道变成可选择的，则此方法直接返回零。*）。

select()方法返回的int值表示有多少通道已经就绪。亦即，自上次调用select()方法后有多少通道变成就绪状态。如果调用select()方法，因为有一个通道变成就绪状态，返回了1，若再次调用select()方法，如果另一个通道就绪了，它会再次返回1。如果对第一个就绪的channel没有做任何操作，现在就有两个就绪的通道，但在每次select()方法调用之间，只有一个通道就绪了。

#### selectedKeys()

一旦调用了select()方法，并且返回值表明有一个或更多个通道就绪了，然后可以通过调用selector的selectedKeys()方法，访问“已选择键集（selected key set）”中的就绪通道。如下所示：



|`1`|`Set selectedKeys = selector.selectedKeys();`|
|----|----|




当像Selector注册Channel时，Channel.register()方法会返回一个SelectionKey 对象。这个对象代表了注册到该Selector的通道。可以通过SelectionKey的selectedKeySet()方法访问这些对象。

可以遍历这个已选择的键集合来访问就绪的通道。如下：




```java
Set selectedKeys = selector.selectedKeys();
Iterator keyIterator = selectedKeys.iterator();
while(keyIterator.hasNext()) {
    SelectionKey key = keyIterator.next();
    if(key.isAcceptable()) {
        // a connection was accepted by a ServerSocketChannel.
    } else if (key.isConnectable()) {
        // a connection was established with a remote server.
    } else if (key.isReadable()) {
        // a channel is ready for reading
    } else if (key.isWritable()) {
        // a channel is ready for writing
    }
    keyIterator.remove();
}
```




这个循环遍历已选择键集中的每个键，并检测各个键所对应的通道的就绪事件。

注意每次迭代末尾的keyIterator.remove()调用。Selector不会自己从已选择键集中移除SelectionKey实例。必须在处理完通道时自己移除。下次该通道变成就绪时，Selector会再次将其放入已选择键集中。

SelectionKey.channel()方法返回的通道需要转型成你要处理的类型，如ServerSocketChannel或SocketChannel等。

#### 3.6wakeUp()

某个线程调用select()方法后阻塞了，即使没有通道已经就绪，也有办法让其从select()方法返回。只要让其它线程在第一个线程调用select()方法的那个对象上调用Selector.wakeup()方法即可。阻塞在select()方法上的线程会立马返回。

如果有其它线程调用了wakeup()方法，但当前没有线程阻塞在select()方法上，下个调用select()方法的线程会立即“醒来（wake up）”。

#### 3.7close()

用完Selector后调用其close()方法会关闭该Selector，且使注册到该Selector上的所有SelectionKey实例无效。通道本身并不会关闭。

#### 3.8完整的示例

这里有一个完整的示例，打开一个Selector，注册一个通道注册到这个Selector上(通道的初始化过程略去),然后持续监控这个Selector的四种事件（接受，连接，读，写）是否就绪。



```java
Selector selector = Selector.open();
channel.configureBlocking(false);
SelectionKey key = channel.register(selector, SelectionKey.OP_READ);
while(true) {
  int readyChannels = selector.select();
  if(readyChannels == 0) continue;
  Set selectedKeys = selector.selectedKeys();
  Iterator keyIterator = selectedKeys.iterator();
  while(keyIterator.hasNext()) {
    SelectionKey key = keyIterator.next();
    if(key.isAcceptable()) {
        // a connection was accepted by a ServerSocketChannel.
    } else if (key.isConnectable()) {
        // a connection was established with a remote server.
    } else if (key.isReadable()) {
        // a channel is ready for reading
    } else if (key.isWritable()) {
        // a channel is ready for writing
    }
    keyIterator.remove();
  }
}
```



### 4.NIO非阻塞任务处理实例



ServerSocketChannel在运行前设置成非堵塞模式 然后注册到实际进行任务处理的Dispather线程的Selector中。



```java
package com.boonya.nio;

import java.io.IOException;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Dispatcher implements Runnable {

	Selector selector;
	
	ExecutorService pool=Executors.newFixedThreadPool(20);

	@Override
	public void run() {
		while (true) {
			try {
				dispatch();
			} catch (IOException e) {
				System.out.println(e);
			}
		}
	}

	private void dispatch() throws IOException {
		selector.select();
		for (Iterator<SelectionKey> itor = selector.selectedKeys().iterator(); itor
				.hasNext();) {
			SelectionKey sk = itor.next();
			itor.remove();
			if (sk.isAcceptable()) {
				ServerSocketChannel serverSocketChannel = (ServerSocketChannel) sk
						.channel();
				SocketChannel sc = serverSocketChannel.accept();
				sc.configureBlocking(false);

				RequestHandler handler = new RequestHandler(sc);
				sc.register(selector, SelectionKey.OP_READ,handler);
			} else if (sk.isReadable()) {
				SocketChannel sc = (SocketChannel) sk.channel();
				try {
					int count = selector.select();
					// 小于零 意味着要么就是传过来的没有信息 要么就是有异常了
					if (count < 0) {
						sk.cancel();
						sc.close();
						continue;
					}
					// 等于零的情况 继续
					if (count <= 0) {
						sk.cancel();
						sc.close();
						continue;
					}
				} catch (Exception e) {
					System.out.println("NIODiapatcher dispatch()"+e.getMessage());
					sk.cancel();
					sc.close();
					continue;
				}
				RequestHandler handler=(RequestHandler) sk.attachment();
				pool.execute(handler);
			}
		}
	}

	public void register(ServerSocketChannel ssc, int ops) {
		try {
			ssc.register(selector, ops);
		} catch (ClosedChannelException e) {

		}
	}
	
	public class RequestHandler implements Runnable{
		
		SocketChannel channel;

		public  RequestHandler(SocketChannel channel){
			this.channel=channel;
		}
		
		@Override
		public void run() {
			
		}
		
	}
}
```





参考资料：http://ifeve.com/selectors/，http://flym.iteye.com/blog/392350，http://www.tuicool.com/articles/QfeEFz，http://warnerhit.iteye.com/blog/1417744












