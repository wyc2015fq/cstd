# Netty——1、网络编程基本概念 - 隔壁老王的专栏 - CSDN博客





2016年11月19日 16:25:59[隔壁老王来也](https://me.csdn.net/haoyuyang)阅读数：9363








**1.阻塞IO—Socket**

Socket又称“套接字”，应用程序通常通过“套接字”向网络发出请求或应答网络请求。

Socket和ServerSocket类库位于java.net包中。ServerSocket用于服务器端，Socket是建立网络连接时使用的。在连接成功时，应用程序两端都会产生一个Socket实例，操作这个实例，完成所需的会话。对于一个网络连接来说，套接字是平等的，不会因为在服务器端或在客户端而产生不同的级别。不管是ServerSocket还是Socket，它们的工作都是通过SocketImpl类及其子类完成的。

套接字的连接过程可以分为四个步骤：服务器监听、客户端请求服务器、服务器端连接确认、客户端连接确认并进行通信。

（1）服务器监听：服务器端套接字并不定位具体的客户端套接字，而是出于等待连接的状态，实时监控网络状态。

（2）客户端请求：客户端的套接字提出连接请求，要连接的目标是服务器端的套接字。为此，客户端的套接字必须首先描述要连接的服务器端的套接字，指出服务器端的套接字的地址和端口号，然后向服务器端套接字提出连接请求。

（3）服务器端连接确认：当服务器端的套接字监听到或者说接收到客户端套接字的连接请求，它就响应客户端套接字的请求，建立一个新的线程，把服务器端套接字的描述发送给客户端。

（4）客户端连接确认：一旦客户端确认了此描述，连接就建立好了，双方开始通信。而服务器端套接字继续处于监听状态，继续接收其他客户端套接字的连接请求。

借用一下网上的Socket通信模型图片：

![](https://img-blog.csdn.net/20161119184425747?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




Socket通信步骤：

①创建ServerSocket和Socket

②打开连接到Socket的输入/输出流

③按照协议对Socket进行读写操作

④关闭输入输出流、关闭Socket

服务器端：

①创建ServerSocket对象，绑定监听端口

②通过accept()方法监听客户端请求

③建立连接后，通过输入流读取客户端发送的请求信息

④通过输出流向客户端发送响应信息

⑤关闭相关资源

客户端：

①创建Socket对象，指明需要连接的服务器的地址和端口号

②连接建立后，通过输出流向服务器端发送请求信息

③通过输入流获取服务器响应信息

④关闭响应资源

下面看一个简单的小例子：

服务器端响应工具类：



```java
public class ServerHandler implements Runnable {

    private Socket socket;

    public ServerHandler(Socket socket) {
        this.socket = socket;
    }

    @Override
    public void run() {
        BufferedReader bufferedReader = null;
        PrintWriter printWriter = null;
        try {
            bufferedReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            printWriter = new PrintWriter(socket.getOutputStream(), true);

            while (true) {
                String info = bufferedReader.readLine();
                if(info == null)
                    break;
                System.out.println("客户端发送的消息：" + info);
                printWriter.println("服务器端响应了客户端请求....");
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if(bufferedReader != null){
                try {
                    bufferedReader.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if(printWriter != null){
                try {
                    printWriter.close();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
            if(socket != null){
                try {
                    socket.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            socket = null;
        }
    }
}
```
服务器端：





```java
public class Server {
    private static int PORT = 8379;
    public static void main(String[] args) {
        ServerSocket serverSocket = null;
        try {
            serverSocket = new ServerSocket(PORT);
            System.out.println("服务器端启动了....");
            //进行阻塞
            Socket socket = serverSocket.accept();
            //启动一个线程来处理客户端请求
            new Thread(new ServerHandler(socket)).start();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if(serverSocket != null){
                try {
                    serverSocket.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            serverSocket = null;
        }
    }
}
```
客户端：





```java
public class Client {
    private static int PORT = 8379;
    private static String IP = "127.0.0.1";

    public static void main(String[] args) {
        BufferedReader bufferedReader = null;
        PrintWriter printWriter = null;
        Socket socket = null;
        try {
            socket = new Socket(IP, PORT);
            printWriter = new PrintWriter(socket.getOutputStream(), true);
            bufferedReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));

            printWriter.println("客户端请求了服务器....");
            String response = bufferedReader.readLine();
            System.out.println("Client：" + response);
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if(bufferedReader != null){
                try {
                    bufferedReader.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if(printWriter != null){
                try {
                    printWriter.close();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
            if(socket != null){
                try {
                    socket.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            } else {
                socket = null;
            }
        }
    }
}
```
运行结果：



![](https://img-blog.csdn.net/20161119184542481?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20161119184602718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


以上的代码有个问题，就是每次有客户端请求服务器端都会创建一个线程，当线程过多时，服务器端可能会宕机。解决这个问题，可以使用JDK提供的线程池（伪异步）。其它地方都不变，将服务器端的代码修改成如下即可：



```java
public class Server {
    private static int PORT = 8379;
    public static void main(String[] args) {
        ServerSocket serverSocket = null;
        try {
            serverSocket = new ServerSocket(PORT);
            System.out.println("服务器端启动了....");
            //进行阻塞
            Socket socket = null;
            //启动一个线程来处理客户端请求
            //new Thread(new ServerHandler(socket)).start();
            HandlerExecutorPool pool = new HandlerExecutorPool(50, 1000);
            while (true) {
                socket = serverSocket.accept();
                pool.execute(new ServerHandler(socket));
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if(serverSocket != null){
                try {
                    serverSocket.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            serverSocket = null;
        }
    }
}
```
其中HandlerExecutorPool为自定义的线程池，代码如下：
```java
public class HandlerExecutorPool {

    private ExecutorService executor;

    public HandlerExecutorPool(int maxSize, int queueSize) {
        this.executor = new ThreadPoolExecutor(Runtime.getRuntime().availableProcessors(), maxSize, 120L, TimeUnit.SECONDS,
```

```java
new ArrayBlockingQueue<>(queueSize));
    }

    public void execute(Runnable task) {
        executor.execute(task);
    }
}
```

**2.IO（BIO）与NIO的区别**

其本质就是阻塞和非阻塞的区别。

阻塞概念：应用程序在获取网络数据的时候，如果网络传输数据很慢，那么程序就一直等着，直到传输完毕为止。

非阻塞概念：应用程序直接可以获取已经准备就绪的数据，无需等待。

IO为同步阻塞形式，NIO为同步非阻塞形式。NIO没有实现异步，在JDK1.7之后，升级了NIO库包，支持异步非阻塞通信模型，即NIO2.0(AIO)。

同步和异步：同步和异步一般是面向操作系统与应用程序对IO操作的层面上来区别的。①同步时，应用程序会直接参与IO读写操作，并且应用程序会直接阻塞到某一个方法上，直到数据准备就绪（BIO）；或者采用轮询的策略实时检查数据的就绪状态，如果就绪则获取数据（NIO）。②异步时，则所有的IO读写操作都交给操作系统处理，与应用程序没有直接关系，应用程序并不关心IO读写，当操作系统完成IO读写操作时，会向应用程序发出通知，应用程序直接获取数据即可。

同步说的是Server服务端的执行方式，阻塞说的是具体的技术，接收数据的方式、状态（io、nio）。

3.NIO编程介绍

学习NIO编程，首先需要了解几个概念：

（1）Buffer（缓冲区）

Buffer是一个对象，它包含一些需要写入或者读取的数据。在NIO类库中加入Buffer对象，体现了新类库与原IO的一个重要区别。在面向流的IO中，可以直接将数据写入或读取到Stream对象中。在NIO类库中，所有的数据都是用缓冲区处理的（读写）。 缓冲区实质上是一个数组，通常它是一个字节数组（ByteBuffer），也可以使用其他类型的数组。这个数组为缓冲区提供了访问数据的读写等操作属性，如位置、容量、上限等概念，具体的可以参考API文档。

Buffer类型：最常使用的是ByteBuffer，实际上每一种java基本类型都对应了一种缓存区（除了Boolean类型）。

①ByteBuffer②CharBuffer③ShortBuffer④IntBuffer⑤LongBuffer⑥FloatBuffer⑦DoubleBuffer

（2）Channel（管道、通道）

Channel就像自来水管道一样，网络数据通过Channel读取和写入，通道与流的不同之处在于通道是双向的，而流只能在一个方向上移动（一个流必须是InputStream或者OutputStream的子类），而通道可以用于读、写或者二者同时进行，最关键的是可以和多路复用器集合起来，有多种的状态位，方便多路复用器去识别。通道分为两大类：一类是用于网络读写的SelectableChannel，另一类是用于文件操作的FileChannel，我们使用的SocketChannel和ServerSocketChannel都是SelectableChannel的子类。



（3）Selector（选择器、多路复用器）

是NIO编程的基础，非常重要。多路复用器提供选择已经就绪的任务的能力。简单说，就是Selector会不断的轮询注册在其上的通道（Channel），如果某个通道发生了读写操作，这个通道就处于就绪状态，会被Selector轮询出来，然后通过SelectionKey可以取得就绪的Channel集合，从而进行后续的IO操作。一个多路复用器（Selector）可以负责成千上万的通道（Channel），没有上限。这也是JDK使用了epoll代替传统的select实现，获得连接句柄（客户端）没有限制。那也就意味着我们只要一个线程负责Selector的轮询，就可以接入成千上万个客户端，这是JDK
 NIO库的巨大进步。

Selector线程类似一个管理者（Master），管理了成千上万个管道，然后轮询哪个管道的数据已经准备好了，通知CPU执行IO的读取或写入操作。

Selector模式：当IO事件（管道）注册到选择器以后，Selector会分配给每个管道一个key值，相当于标签。Selector选择器是以轮询的方式进行查找注册的所有IO事件（管道），当IO事件（管道）准备就绪后，Selector就会识别，会通过key值来找到相应的管道，进行相关的数据处理操作（从管道中读取或写入数据，写到缓冲区中）。每个管道都会对选择器进行注册不同的事件状态，以便选择器查找。

事件状态：

SelectionKey.OP_CONNECT

SelectionKey.OP_ACCEPT

SelectionKey.OP_READ

SelectionKey.OP_WRITE

NIO通信模型图解：

![](https://img-blog.csdn.net/20161119210915533)

（虚线表示不直接相关联）

下面用代码来演示一下Buffer、Channel、Selector的使用。

以IntBuffer为例，讲解一下Buffer的常用API：



```java
public class IntBufferTest {
    public static void main(String[] args) {
        //1、基本操作
        //创建指定长度的缓冲区
        /*IntBuffer buffer = IntBuffer.allocate(10);
        buffer.put(11); //position位置：0->1
        buffer.put(5); //position位置：1->2
        buffer.put(32); //position位置：2->3
        System.out.println("未调用flip复位方法前的buffer：" + buffer);
        //把位置复位为0，也就是position位置由3->0
        buffer.flip();
        //比较未调用flip方法和调用之后buffer的limit可以发现，不进行复位操作的话，position的值为3，limit的值为10
        // 因为缓冲区中已有11、5、32三个元素，也就意味着put()方法会使position向后递增1
        System.out.println("调用flip复位方法后的buffer：" + buffer);
        System.out.println("buffer容量为：" + buffer.capacity());
        System.out.println("buffer限制为：" + buffer.limit());
        System.out.println("获取下标为1的元素：" + buffer.get(1));
        System.out.println("调用get(index)方法后的buffer：" + buffer); //调用get(index)方法，不会改变position的值
        buffer.put(1, 4); //将buffer位置为1的值替换为4，调用put(index，value)不会改变position的值
        System.out.println("调用put(index, value)方法后的buffer：" + buffer);

        for(int i=0; i<buffer.limit(); i++) {
            //调用get方法会使缓冲区的位置(position)向后递增一位
            System.out.print(buffer.get() + "\t");
        }
        System.out.println("\nbuffer对象遍历之后buffer为：" + buffer);*/


        //2、wrap方法的使用
        /*int[] arr = new int[]{1, 2, 3};
        IntBuffer buffer = IntBuffer.wrap(arr);
        System.out.println("wrap(arr)方法：" + buffer);
        //IntBuffer.wrap(array, postion, length)表示容量为array的长度，但是可操作的元素为位置postion到length的数组元素
        buffer = IntBuffer.wrap(arr, 0, 2);
        System.out.println("wrap(arr, 0, 2)：" + buffer);*/

        //3、其他方法
        IntBuffer buffer = IntBuffer.allocate(10);
        int[] arr = new int[]{1, 2, 3};
        buffer.put(arr);
        System.out.println("调用put(arr)方法后的buffer：" + buffer);
        //一种复制方法，buffer1的pos、lim、cap与buffer的一样
        IntBuffer buffer1 = buffer.duplicate();
        System.out.println("buffer1：" + buffer1);

        buffer.position(1); //将buffer的position设置为1，不建议使用。功能相当于flip()方法，但是从运行结果可以看出，lim依然等于10
        System.out.println("调用position()方法后的buffer：" + buffer);
        System.out.println("buffer的可读数据量：" + buffer.remaining()); //计算出从pos到lim的长度
        int[] arr1 = new int[buffer.remaining()];
        //将缓冲区的数据放入arr1中
        buffer.get(arr1);
        for(Integer i : arr1) {
            System.out.print(Integer.toString(i) + ",");
        }
        System.out.println();

        //比较flip()方法和position(index)方法的区别
        buffer1.flip();
        System.out.println("buffer1的可读数量：" + buffer1.remaining());
        arr1 = new int[buffer1.remaining()];
        buffer1.get(arr1);
        for(Integer i : arr1) {
            System.out.print(Integer.toString(i) + ",");
        }
    }
}
```
运行结果：



![](https://img-blog.csdn.net/20161119230855583)


![](https://img-blog.csdn.net/20161119230903318)

![](https://img-blog.csdn.net/20161119230911174)

接下来是Buffer、Channel、Selector的一个入门的小例子：

Server端：



```java
public class Server implements Runnable {

    private Selector selector;
    private ByteBuffer buffer = ByteBuffer.allocate(1024);

    public Server(int port) {
        try {
            //1 打开多复用器
            selector = Selector.open();
            //2 打开服务器通道
            ServerSocketChannel ssc = ServerSocketChannel.open();
            //3 设置服务器通道为非阻塞方式
            ssc.configureBlocking(false);
            //4 绑定地址
            ssc.bind(new InetSocketAddress(port));
            //5 把服务器通道注册到多路复用选择器上，并监听阻塞状态
            ssc.register(selector, SelectionKey.OP_ACCEPT);
            System.out.println("Server start, port：" + port);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void run() {
        while (true) {
            try {
                //1 必须让多路复用选择器开始监听
                selector.select();
                //2 返回所有已经注册到多路复用选择器上的通道的SelectionKey
                Iterator<SelectionKey> keys = selector.selectedKeys().iterator();
                //3 遍历keys
                while (keys.hasNext()) {
                    SelectionKey key = keys.next();
                    keys.remove();
                    if(key.isValid()) { //如果key的状态是有效的
                        if(key.isAcceptable()) { //如果key是阻塞状态，则调用accept()方法
                            accept(key);
                        }
                        if(key.isReadable()) { //如果key是可读状态，则调用read()方法
                            read(key);
                        }
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private void accept(SelectionKey key) {
        try {
            //1 获取服务器通道
            ServerSocketChannel ssc = (ServerSocketChannel) key.channel();
            //2 执行阻塞方法
            SocketChannel sc = ssc.accept();
            //3 设置阻塞模式为非阻塞
            sc.configureBlocking(false);
            //4 注册到多路复用选择器上，并设置读取标识
            sc.register(selector, SelectionKey.OP_READ);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void read(SelectionKey key) {
        try {
            //1 清空缓冲区中的旧数据
            buffer.clear();
            //2 获取之前注册的SocketChannel通道
            SocketChannel sc = (SocketChannel) key.channel();
            //3 将sc中的数据放入buffer中
            int count = sc.read(buffer);
            if(count == -1) { // == -1表示通道中没有数据
                key.channel().close();
                key.cancel();
                return;
            }
            //读取到了数据，将buffer的position复位到0
            buffer.flip();
            byte[] bytes = new byte[buffer.remaining()];
            //将buffer中的数据写入byte[]中
            buffer.get(bytes);
            String body = new String(bytes).trim();
            System.out.println("Server：" + body);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        new Thread(new Server(8379)).start();
    }
}
```
客户端：





```java
public class Client {
    public static void main(String[] args) {
        InetSocketAddress address = new InetSocketAddress("127.0.0.1", 8379);
        SocketChannel sc = null;
        ByteBuffer buffer = ByteBuffer.allocate(1024);
        try {
            //打开通道
            sc = SocketChannel.open();
            //建立连接
            sc.connect(address);
            while (true) {
                byte[] bytes = new byte[1024];
                System.in.read(bytes);
                //把输入的数据放入buffer缓冲区
                buffer.put(bytes);
                //复位操作
                buffer.flip();
                //将buffer的数据写入通道
                sc.write(buffer);
                //清空缓冲区中的数据
                buffer.clear();
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if(sc != null) {
                try {
                    sc.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
```
运行结果：



![](https://img-blog.csdn.net/20161120160204869)

![](https://img-blog.csdn.net/20161120160212885)

**3、AIO**

在NIO的基础上引入了异步通道的概念，并提供了异步文件和异步套接字通道的实现，从而在真正意义上实现了异步非阻塞，之前的NIO只是非阻塞而并非异步。AIO不需要通过对多路复用器对注册的通道进行轮询操作即可实现异步读写，从而简化NIO编程模型。

①AsynchronousServerSocketChannel

②AsynchronousSocketChannel

下面看代码：

Server端：



```java
public class Server {
    //线程池
    private ExecutorService executorService;
    //线程组
    private AsynchronousChannelGroup channelGroup;
    //服务器通道
    public AsynchronousServerSocketChannel channel;

    public Server(int port) {
        try {
            //创建线程池
            executorService  = Executors.newCachedThreadPool();
            //创建线程组
            channelGroup = AsynchronousChannelGroup.withCachedThreadPool(executorService, 1);
            //创建服务器通道
            channel = AsynchronousServerSocketChannel.open(channelGroup);
            //绑定地址
            channel.bind(new InetSocketAddress(port));
            System.out.println("server start, port：" + port);
            channel.accept(this, new ServerCompletionHandler());
            Thread.sleep(Integer.MAX_VALUE);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        Server server = new Server(8379);
    }
}
```
ServerCompletionHandler类：
```java
public class ServerCompletionHandler implements CompletionHandler<AsynchronousSocketChannel, Server> {
    @Override
    public void completed(AsynchronousSocketChannel channel, Server attachment) {
        //当有下一个客户端接入的时候，直接调用Server的accept方法，这样反复执行下去，保证多个客户端都可以阻塞
        attachment.channel.accept(attachment, this);
        read(channel);
    }

    private void read(AsynchronousSocketChannel channel) {
        //读取数据
        ByteBuffer buffer = ByteBuffer.allocate(1024);
        channel.read(buffer, buffer, new CompletionHandler<Integer, ByteBuffer>() {
            @Override
            public void completed(Integer resultSize, ByteBuffer attachment) {
                attachment.flip();
                System.out.println("Server->" + "收到客户端发送的数据长度为：" + resultSize);
                String data = new String(buffer.array()).trim();
                System.out.println("Server->" + "收到客户端发送的数据为：" + data);
                String response = "服务器端响应了客户端。。。。。。";
                write(channel, response);
            }

            @Override
            public void failed(Throwable exc, ByteBuffer attachment) {
                exc.printStackTrace();
            }
        });
    }

    private void write(AsynchronousSocketChannel channel, String response) {
        try {
            ByteBuffer buffer = ByteBuffer.allocate(1024);
            buffer.put(response.getBytes());
            buffer.flip();
            channel.write(buffer).get();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void failed(Throwable exc, Server attachment) {
        exc.printStackTrace();
    }
}
```
客户端：
```java
public class Client implements Runnable {

    private AsynchronousSocketChannel channel;

    public Client() throws IOException {
        channel = AsynchronousSocketChannel.open();
    }

    public void connect() {
        channel.connect(new InetSocketAddress("127.0.0.1", 8379));
    }

    public void write(String data) {
        try {
            channel.write(ByteBuffer.wrap(data.getBytes())).get();
            read();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void read() {
        ByteBuffer buffer = ByteBuffer.allocate(1024);
        try {
            channel.read(buffer).get();
            buffer.flip();
            byte[] bytes = new byte[buffer.remaining()];
            buffer.get(bytes);
            String data = new String(bytes, "UTF-8").trim();
            System.out.println(data);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void run() {
        while (true) {

        }
    }

    public static void main(String[] args) {
        try {
            Client c1 = new Client();
            Client c2 = new Client();
            Client c3 = new Client();

            c1.connect();
            c2.connect();
            c3.connect();

            new Thread(c1).start();
            new Thread(c2).start();
            new Thread(c3).start();

            Thread.sleep(1000);

            c1.write("c1 aaa");
            c2.write("c2 bbbb");
            c3.write("c3 ccccc");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
运行结果：



![](https://img-blog.csdn.net/20161120182732500)


![](https://img-blog.csdn.net/20161120182741400)







