# Java NIO原理图文分析及代码实现 - 零度的博客专栏 - CSDN博客
2016年07月08日 09:35:01[零度anngle](https://me.csdn.net/zmx729618)阅读数：8756标签：[nio																[通信](https://so.csdn.net/so/search/s.do?q=通信&t=blog)](https://so.csdn.net/so/search/s.do?q=nio&t=blog)
个人分类：[JavaIO BIO/NIO/AIO																[Java](https://blog.csdn.net/zmx729618/article/category/3108443)](https://blog.csdn.net/zmx729618/article/category/6297072)
###        一、java NIO 和阻塞I/O的区别
#### 1. 阻塞I/O通信模型
假如现在你对阻塞I/O已有了一定了解，我们知道阻塞I/O在调用InputStream.read()方法时是阻塞的，它会一直等到数据到来时（或超时）才会返回；同样，在调用ServerSocket.accept()方法时，也会一直阻塞到有客户端连接才会返回，每个客户端连接过来后，服务端都会启动一个线程去处理该客户端的请求。阻塞I/O的通信模型示意图如下：
![](http://images.51cto.com/files/uploadimg/20120411/1553280.jpg)
如果你细细分析，一定会发现阻塞I/O存在一些缺点。根据阻塞I/O通信模型，我总结了它的两点缺点：
- 当客户端多时，会创建大量的处理线程。且每个线程都要占用栈空间和一些CPU时间
- 阻塞可能带来频繁的上下文切换，且大部分上下文切换可能是无意义的。
在这种情况下非阻塞式I/O就有了它的应用前景。
#### 2. java NIO原理及通信模型
Java NIO是在jdk1.4开始使用的，它既可以说成“新I/O”，也可以说成非阻塞式I/O。下面是java NIO的工作原理：
- 由一个专门的线程来处理所有的 IO 事件，并负责分发。
- 事件驱动机制：事件到的时候触发，而不是同步的去监视事件。
- 线程通讯：线程之间通过 wait,notify 等方式通讯。保证每次上下文切换都是有意义的。减少无谓的线程切换。
阅读过一些资料之后，下面贴出我理解的java NIO的工作原理图：
![](http://images.51cto.com/files/uploadimg/20120411/1553281.jpg)
（注：每个线程的处理流程大概都是读取数据、解码、计算处理、编码、发送响应。）
Java NIO的服务端只需启动一个专门的线程来处理所有的 IO 事件，这种通信模型是怎么实现的呢？呵呵，我们一起来探究它的奥秘吧。java NIO采用了双向通道（channel）进行数据传输，而不是单向的流（stream），在通道上可以注册我们感兴趣的事件。一共有以下四种事件：
|事件名|对应值|
|----|----|
|服务端接收客户端连接事件|SelectionKey.OP_ACCEPT(16)|
|客户端连接服务端事件|SelectionKey.OP_CONNECT(8)|
|读事件|SelectionKey.OP_READ(1)|
|写事件|SelectionKey.OP_WRITE(4)|
服务端和客户端各自维护一个管理通道的对象，我们称之为selector，该对象能检测一个或多个通道 (channel) 上的事件。我们以服务端为例，如果服务端的selector上注册了读事件，某时刻客户端给服务端发送了一些数据，阻塞I/O这时会调用read()方法阻塞地读取数据，而NIO的服务端会在selector中添加一个读事件。服务端的处理线程会轮询地访问selector，如果访问selector时发现有感兴趣的事件到达，则处理这些事件，如果没有感兴趣的事件到达，则处理线程会一直阻塞直到感兴趣的事件到达为止。下面是我理解的java
 NIO的通信模型示意图：
![](http://images.51cto.com/files/uploadimg/20120411/1553282.jpg)
### 二、java NIO服务端和客户端代码实现
为了更好地理解java NIO,下面贴出服务端和客户端的简单代码实现。
服务端：
```
package zmx.nio.test;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
/**
 * NIO服务端
 */
public class NIOServer {
	// 通道管理器
	private Selector selector;
	/**
	 * 获得一个ServerSocket通道，并对该通道做一些初始化的工作
	 * @param port 绑定的端口号
	 * @throws IOException
	 */
	public void initServer(int port) throws IOException {
		// 获得一个ServerSocketChannel通道
		ServerSocketChannel serverSocketChannel = ServerSocketChannel.open();
		// 设置通道为非阻塞
		serverSocketChannel.configureBlocking(false);
		// 将该通道对应的ServerSocket绑定到port端口		
		serverSocketChannel.bind(new InetSocketAddress(port));
		// 获得一个通道管理器
		this.selector = Selector.open();
		// 将通道管理器和该通道绑定，并为该通道注册SelectionKey.OP_ACCEPT事件,注册该事件后，
		// 当该事件到达时，selector.select()会返回，如果该事件没到达selector.select()会一直阻塞。
		serverSocketChannel.register(selector, SelectionKey.OP_ACCEPT);
	}
	/**
	 * 采用轮询的方式监听selector上是否有需要处理的事件，如果有，则进行处理
	 * @throws IOException
	 */
	public void listen() throws IOException {
		System.out.println("服务端启动成功！");
		// 轮询访问selector
		while (true) {
			// 当注册的事件到达时，方法返回；否则,该方法会一直阻塞
			selector.select();
			// 获得selector中选中的项的迭代器，选中的项为注册的事件
			Iterator<SelectionKey> ite = this.selector.selectedKeys().iterator();
			while (ite.hasNext()) {
				SelectionKey key = (SelectionKey) ite.next();
				// 删除已选的key,以防重复处理
				ite.remove();
				
				if (key.isAcceptable()) {// 客户端请求连接事件
					ServerSocketChannel server = (ServerSocketChannel) key.channel();
					// 获得和客户端连接的通道
					SocketChannel channel = server.accept();
					// 设置成非阻塞
					channel.configureBlocking(false);
					// 在这里可以给客户端发送信息哦
					channel.write(ByteBuffer.wrap(new String("向客户端发送了一条信息")
							.getBytes("utf-8")));
					// 在和客户端连接成功之后，为了可以接收到客户端的信息，需要给通道设置读的权限。
					channel.register(this.selector, SelectionKey.OP_READ);
					
				} else if (key.isReadable()) {// 获得了可读的事件
					read(key);
				}
			}
		}
	}
	/**
	 * 处理读取客户端发来的信息 的事件
	 * 
	 * @param key
	 * @throws IOException
	 */
	public void read(SelectionKey key) throws IOException {
		// 服务器可读取消息:得到事件发生的Socket通道
		SocketChannel channel = (SocketChannel) key.channel();
		// 创建读取的缓冲区
		ByteBuffer buffer = ByteBuffer.allocate(512); 
		channel.read(buffer);
		byte[] data = buffer.array();
		String msg = new String(data).trim();
		System.out.println("服务端收到信息：" + msg);
		ByteBuffer outBuffer = ByteBuffer.wrap(msg.getBytes("utf-8"));
		channel.write(outBuffer);// 将消息回送给客户端
	}
	/**
	 * 启动服务端测试
	 * 
	 * @throws IOException
	 */
	public static void main(String[] args) throws IOException {
		NIOServer server = new NIOServer();
		server.initServer(8000);
		server.listen();
	}
}
```
客户端：
```
package zmx.nio.test;
import java.io.IOException;  
import java.net.InetSocketAddress;  
import java.nio.ByteBuffer;  
import java.nio.channels.SelectionKey;  
import java.nio.channels.Selector;  
import java.nio.channels.SocketChannel;  
import java.util.Iterator;  
 
/**  
 * NIO客户端  
 */ 
public class NIOClient {  
    //通道管理器  
    private Selector selector;  
 
    /**  
     * 获得一个Socket通道，并对该通道做一些初始化的工作  
     * @param ip 连接的服务器的ip  
     * @param port  连接的服务器的端口号           
     * @throws IOException  
     */ 
    public void initClient(String ip,int port) throws IOException {  
        // 获得一个Socket通道  
        SocketChannel channel = SocketChannel.open();  
        // 设置通道为非阻塞  
        channel.configureBlocking(false);  
        // 获得一个通道管理器  
        this.selector = Selector.open();  
          
        // 客户端连接服务器,其实方法执行并没有实现连接，需要在listen（）方法中调  
        //用channel.finishConnect();才能完成连接  
        channel.connect(new InetSocketAddress(ip,port));  
        //将通道管理器和该通道绑定，并为该通道注册SelectionKey.OP_CONNECT事件。  
        channel.register(selector, SelectionKey.OP_CONNECT);  
    }  
 
    /**  
     * 采用轮询的方式监听selector上是否有需要处理的事件，如果有，则进行处理  
     * @throws IOException  
     */  
    public void connect() throws IOException {  
        // 轮询访问selector  
        while (true) {  
            // 选择一组可以进行I/O操作的事件，放在selector中,客户端的该方法不会阻塞，  
            //这里和服务端的方法不一样，查看api注释可以知道，当至少一个通道被选中时，  
            //selector的wakeup方法被调用，方法返回，而对于客户端来说，通道一直是被选中的  
            selector.select();  
            // 获得selector中选中的项的迭代器  
            Iterator<SelectionKey> ite = this.selector.selectedKeys().iterator();  
            while (ite.hasNext()) {  
                SelectionKey key = (SelectionKey) ite.next();  
                // 删除已选的key,以防重复处理  
                ite.remove();  
                // 连接事件发生  
                if (key.isConnectable()) {  
                    SocketChannel channel = (SocketChannel) key.channel();  
                    // 如果正在连接，则完成连接  
                    if(channel.isConnectionPending()){  
                        channel.finishConnect();                            
                    }  
                    // 设置成非阻塞  
                    channel.configureBlocking(false);  
                    //在这里可以给服务端发送信息哦  
                    channel.write(ByteBuffer.wrap(new String("向服务端发送了一条信息").getBytes("utf-8")));  
                    //在和服务端连接成功之后，为了可以接收到服务端的信息，需要给通道设置读的权限。  
                    channel.register(this.selector, SelectionKey.OP_READ);                                            // 获得了可读的事件  
                } else if (key.isReadable()) {  
                        read(key);  
                }  
            }   
        }  
    }  
    /**  
     * 处理读取服务端发来的信息 的事件  
     * @param key  
     * @throws IOException   
     */ 
    public void read(SelectionKey key) throws IOException{  
        //和服务端的read方法一样  
		// 服务器可读取消息:得到事件发生的Socket通道
		SocketChannel channel = (SocketChannel) key.channel();
		// 创建读取的缓冲区
		ByteBuffer buffer = ByteBuffer.allocate(512);
		channel.read(buffer);
		byte[] data = buffer.array();
		String msg = new String(data).trim();
		System.out.println("客户端收到信息：" + msg);
		ByteBuffer outBuffer = ByteBuffer.wrap(msg.getBytes("utf-8"));
		channel.write(outBuffer);// 将消息回送给客户端
    }  
      
      
    /**  
     * 启动客户端测试  
     * @throws IOException   
     */ 
    public static void main(String[] args) throws IOException {  
        NIOClient client = new NIOClient();  
        client.initClient("localhost",8000);  
        client.connect();  
    }  
 
}
```
### 三、NIO实现文件读写 
```
package zmx.nio.test;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
public class NIOFileCopy {
	
	public static void fileCopyByNIO(String resource,String destination) throws Exception{
		
		FileInputStream fileInputStream = new FileInputStream(resource);
		FileOutputStream fileOutputStream = new FileOutputStream(destination);
		
		FileChannel fileReadChannel = fileInputStream.getChannel();		
		FileChannel fileWriteChannel = 	fileOutputStream.getChannel();
		
		ByteBuffer buffer = ByteBuffer.allocate(1024);
		buffer.clear();
		int len = 0;
		while((len=fileReadChannel.read(buffer))!=-1){
			buffer.flip();
			fileWriteChannel.write(buffer);
			buffer.clear();
		}
		
		fileReadChannel.close();
		fileWriteChannel.close();		
		
	}
	
	
	public static void operFileMapped(String file) throws Exception{
		
		RandomAccessFile raf = new RandomAccessFile(file, "rw");
		
		FileChannel fc = raf.getChannel();
		 
		MappedByteBuffer mbb = fc.map(FileChannel.MapMode.READ_WRITE, 0, raf.length());
		 		
		/*
		while(mbb.hasRemaining()){
			System.out.println((char)mbb.get());
		}
		*/
		byte index0 =  mbb.get(0);
		System.out.println(index0);
		
		mbb.put(0,(byte)-119);
		
		raf.close();
		
	}
	
	
	public static void main(String[] args) {
		
		/*
		try {
			NIOFileCopy.fileCopyByNIO("D:\\projects\\testWeb\\src\\zmx\\nio\\test\\test.txt", "D:\\projects\\testWeb\\src\\zmx\\nio\\test\\copy.txt");
		} catch (Exception e) {			
			e.printStackTrace();
		}
		*/
				
		try {
			NIOFileCopy.operFileMapped("D:\\projects\\testWeb\\src\\zmx\\nio\\test\\yumi.png");
		} catch (Exception e) {
			e.printStackTrace();
		}
		
	}
}
```
