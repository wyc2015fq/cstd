# 高并发Java：NIO和AIO - 菜鸟路上的小白 - CSDN博客





2016年11月07日 14:38:28[somnus_小凯](https://me.csdn.net/u012486840)阅读数：1784









IO感觉上和多线程并没有多大关系，但是NIO改变了线程在应用层面使用的方式，也解决了一些实际的困难。而AIO是异步IO和前面的系列也有点关系。在此，为了学习和记录，也写一篇文章来介绍NIO和AIO。

## 1. 什么是NIO


NIO是New I/O的简称，与旧式的基于流的I/O方法相对，从名字看，它表示新的一套Java I/O标 准。它是在Java 1.4中被纳入到JDK中的，并具有以下特性：
- NIO是基于块（Block）的，它以块为基本单位处理数据 （硬盘上存储的单位也是按Block来存储，这样性能上比基于流的方式要好一些）
- 为所有的原始类型提供（Buffer）缓存支持
- 增加通道（Channel）对象，作为新的原始 I/O 抽象
- 支持锁（我们在平时使用时经常能看到会出现一些.lock的文件，这说明有线程正在使用这把锁，当线程释放锁时，会把这个文件删除掉，这样其他线程才能继续拿到这把锁）和内存映射文件的文件访问接口
- 提供了基于Selector的异步网络I/O

![KKNftaoEMWYIdp7q.png](https://img-blog.csdn.net/20161107143534533?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


所有的从通道中的读写操作，都要经过Buffer，而通道就是io的抽象，通道的另一端就是操纵的文件。

## 2. Buffer

![P8idokeokUYegA5G.png](https://img-blog.csdn.net/20161107143553549?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


Java中Buffer的实现。基本的数据类型都有它对应的Buffer


Buffer的简单使用例子：
- `package test;`
- `import java.io.File;`
- `import java.io.FileInputStream;`
- `import java.nio.ByteBuffer;`
- `import java.nio.channels.FileChannel;`
- `public class Test {`
- `    public static void main(String[] args) throws Exception {`
- `       FileInputStream fin = new FileInputStream(new File(`
- `          "d:\\temp_buffer.tmp"));`
- `       FileChannel fc = fin.getChannel();`
- `       ByteBuffer byteBuffer = ByteBuffer.allocate(1024);`
- `       fc.read(byteBuffer);`
- `       fc.close();`
- `       byteBuffer.flip();//读写转换`
- `    }`
- `}`


总结下使用的步骤是：
- 
得到Channel

- 
申请Buffer

- 
建立Channel和Buffer的读/写关系

- 
关闭



下面的例子是使用NIO来复制文件：
- `public static void nioCopyFile(String resource, String destination)`
- `         throws IOException {`
- `       FileInputStream fis = new FileInputStream(resource);`
- `       FileOutputStream fos = new FileOutputStream(destination);`
- `       FileChannel readChannel = fis.getChannel(); // 读文件通道`
- `       FileChannel writeChannel = fos.getChannel(); // 写文件通道`
- `       ByteBuffer buffer = ByteBuffer.allocate(1024); // 读入数据缓存`
- `       while (true) {`
- `         buffer.clear();`
- `         int len = readChannel.read(buffer); // 读入数据`
- `         if (len == -1) {`
- `          break; // 读取完毕`
- `         }`
- `         buffer.flip();`
- `         writeChannel.write(buffer); // 写入文件`
- `       }`
- `       readChannel.close();`
- `       writeChannel.close();`
- `    }`


Buffer中有3个重要的参数：位置（position）、容量（capactiy）和上限（limit）

![7RtqFUlReL36fD6q.png](https://img-blog.csdn.net/20161107143610393?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这里要区别下容量和上限，比如一个Buffer有10KB，那么10KB就是容量，我将5KB的文件读到Buffer中，那么上限就是5KB。


下面举个例子来理解下这3个重要的参数：
- `public static void main(String[] args) throws Exception {`
- `       ByteBuffer b = ByteBuffer.allocate(15); // 15个字节大小的缓冲区`
- `       System.out.println("limit=" + b.limit() + " capacity=" + b.capacity()`
- `          + " position=" + b.position());`
- `       for (int i = 0; i < 10; i++) {`
- `         // 存入10个字节数据`
- `         b.put((byte) i);`
- `       }`
- `       System.out.println("limit=" + b.limit() + " capacity=" + b.capacity()`
- `          + " position=" + b.position());`
- `       b.flip(); // 重置position`
- `       System.out.println("limit=" + b.limit() + " capacity=" + b.capacity()`
- `          + " position=" + b.position());`
- `       for (int i = 0; i < 5; i++) {`
- `         System.out.print(b.get());`
- `       }`
- `       System.out.println();`
- `       System.out.println("limit=" + b.limit() + " capacity=" + b.capacity()`
- `          + " position=" + b.position());`
- `       b.flip();`
- `       System.out.println("limit=" + b.limit() + " capacity=" + b.capacity()`
- `          + " position=" + b.position());`
- `    }`


整个过程如图：

![BO41n1oZsBXr0Mc1.png](https://img-blog.csdn.net/20161107143625612?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


此时position从0到10，capactiy和limit不变。

![k1ZMgOkg25SBjDf2.png](https://img-blog.csdn.net/20161107143637487?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


该操作会重置position，通常，将buffer从写模式转换为读 模式时需要执行此方法 flip()操作不仅重置了当前的position为0，还将limit设置到当前position的位置 。


limit的意义在于，来确定哪些数据是有意义的，换句话说，从position到limit之间的数据才是有意义的数据，因为是上次操作的数据。所以flip操作往往是读写转换的意思。

![Pn8I7kg06velfNef.png](https://img-blog.csdn.net/20161107143654597?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


意义同上。


而Buffer中大多数的方法都是去改变这3个参数来达到某些功能的：
- `public final Buffer rewind()`


将position置零，并清除标志位（mark）
- `public final Buffer clear()`


将position置零，同时将limit设置为capacity的大小，并清除了标志mark
- `public final Buffer flip()`


先将limit设置到position所在位置，然后将position置零，并清除标志位mark，通常在读写转换时使用

### 文件映射到内存
- `public static void main(String[] args) throws Exception {`
- `       RandomAccessFile raf = new RandomAccessFile("C:\\mapfile.txt", "rw");`
- `       FileChannel fc = raf.getChannel();`
- `       // 将文件映射到内存中`
- `       MappedByteBuffer mbb = fc.map(FileChannel.MapMode.READ_WRITE, 0,`
- `          raf.length());`
- `       while (mbb.hasRemaining()) {`
- `         System.out.print((char) mbb.get());`
- `       }`
- `       mbb.put(0, (byte) 98); // 修改文件`
- `       raf.close();`
- `    }`


对MappedByteBuffer的修改就相当于修改文件本身，这样操作的速度是很快的。

## 3. Channel


多线程网络服务器的一般结构：

![fgBZ90gheHS2yrKz.png](https://img-blog.csdn.net/20161107143708472?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


简单的多线程服务器：
- `public static void main(String[] args) throws Exception {`
- `       ServerSocket echoServer = null;`
- `       Socket clientSocket = null;`
- `       try {`
- `         echoServer = new ServerSocket(8000);`
- `       } catch (IOException e) {`
- `         System.out.println(e);`
- `       }`
- `       while (true) {`
- `         try {`
- `          clientSocket = echoServer.accept();`
- `          System.out.println(clientSocket.getRemoteSocketAddress()`
- `                 + " connect!");`
- `          tp.execute(new HandleMsg(clientSocket));`
- `         } catch (IOException e) {`
- `          System.out.println(e);`
- `         }`
- `       }`
- `    }`


功能就是服务器端读到什么数据，就向客户端回写什么数据。


这里的tp是一个线程池，HandleMsg是处理消息的类。
- `static class HandleMsg implements Runnable{  `
- `        省略部分信息                 `
- `        public void run(){         `
- `          try {         `
- `           is = new BufferedReader(new InputStreamReader(clientSocket.getInputStream())); `
- `           os = new PrintWriter(clientSocket.getOutputStream(), true); `
- `           // 从InputStream当中读取客户端所发送的数据              `
- `           String inputLine = null;                 `
- `           long b=System. currentTimeMillis ();                 `
- `           while ((inputLine = is.readLine()) != null)`
- `           {           `
- `               os.println(inputLine);                 `
- `           }                 `
- `           long e=System. currentTimeMillis ();                 `
- `           System. out.println ("spend:"+(e - b)+" ms ");             `
- `         } catch (IOException e) {                 `
- `          e.printStackTrace();             `
- `         }finally`
- `         {  `
- `          关闭资源 `
- `         }     `
- `       } `
- `     }`


客户端：
- `public static void main(String[] args) throws Exception {`
- `       Socket client = null;`
- `       PrintWriter writer = null;`
- `       BufferedReader reader = null;`
- `       try {`
- `         client = new Socket();`
- `         client.connect(new InetSocketAddress("localhost", 8000));`
- `         writer = new PrintWriter(client.getOutputStream(), true);`
- `         writer.println("Hello!");`
- `         writer.flush();`
- `         reader = new BufferedReader(new InputStreamReader(`
- `              client.getInputStream()));`
- `         System.out.println("from server: " + reader.readLine());`
- `       } catch (Exception e) {`
- `       } finally {`
- `         // 省略资源关闭`
- `       }`
- `    }`


以上的网络编程是很基本的，使用这种方式，会有一些问题：


为每一个客户端使用一个线程，如果客户端出现延时等异常，线程可能会被占用很长时间。因为数据的准备和读取都在这个线程中。此时，如果客户端数量众多，可能会消耗大量的系统资源。


解决方案:


使用非阻塞的NIO （读取数据不等待，数据准备好了再工作）


为了体现NIO使用的高效。


这里先模拟一个低效的客户端来模拟因网络而延时的情况：
- `private static ExecutorService tp= Executors.newCachedThreadPool();  `
- `       private static final int sleep_time=1000*1000*1000;  `
- `       public static class EchoClient implements Runnable{   `
- `         public void run(){          `
- `          try {              `
- `              client = new Socket();              `
- `              client.connect(new InetSocketAddress("localhost", 8000)); `
- `              writer = new PrintWriter(client.getOutputStream(), true); `
- `              writer.print("H");              `
- `              LockSupport.parkNanos(sleep_time);       `
- `              writer.print("e");           `
- `              LockSupport.parkNanos(sleep_time);      `
- `              writer.print("l");       `
- `              LockSupport.parkNanos(sleep_time);  `
- `              writer.print("l");       `
- `              LockSupport.parkNanos(sleep_time);  `
- `              writer.print("o");     `
- `              LockSupport.parkNanos(sleep_time);  `
- `              writer.print("!");         `
- `              LockSupport.parkNanos(sleep_time);    `
- `              writer.println();      `
- `              writer.flush(); `
- `          }catch(Exception e)`
- `          {`
- `          }`
- `         }`
- `       }`


服务器端输出：
- `spend:6000ms `
- `spend:6000ms `
- `spend:6000ms `
- `spend:6001ms `
- `spend:6002ms `
- `spend:6002ms `
- `spend:6002ms `
- `spend:6002ms `
- `spend:6003ms `
- `spend:6003ms`


因为
- `while ((inputLine = is.readLine()) != null)`


是阻塞的，所以时间都花在等待中。


如果用NIO来处理这个问题会怎么做呢？


NIO有一个很大的特点就是：把数据准备好了再通知我


而Channel有点类似于流，一个Channel可以和文件或者网络Socket对应 。

![uhKZhUiggOVI1lRa.png](https://img-blog.csdn.net/20161107143736941?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


selector是一个选择器，它可以选择某一个Channel，然后做些事情。


一个线程可以对应一个selector，而一个selector可以轮询多个Channel，而每个Channel对应了一个Socket。


与上面一个线程对应一个Socket相比，使用NIO后，一个线程可以轮询多个Socket。


当selector调用select()时，会查看是否有客户端准备好了数据。当没有数据被准备好时，select()会阻塞。平时都说NIO是非阻塞的，但是如果没有数据被准备好还是会有阻塞现象。


当有数据被准备好时，调用完select()后，会返回一个SelectionKey，SelectionKey表示在某个selector上的某个Channel的数据已经被准备好了。


只有在数据准备好时，这个Channel才会被选择。


这样NIO实现了一个线程来监控多个客户端。


而刚刚模拟的网络延迟的客户端将不会影响NIO下的线程，因为某个Socket网络延迟时，数据还未被准备好，selector是不会选择它的，而会选择其他准备好的客户端。


selectNow()与select()的区别在于，selectNow()是不阻塞的，当没有客户端准备好数据时，selectNow()不会阻塞，将返回0，有客户端准备好数据时，selectNow()返回准备好的客户端的个数。


主要代码：
- `package test;`
- `import java.net.InetAddress;`
- `import java.net.InetSocketAddress;`
- `import java.net.Socket;`
- `import java.nio.ByteBuffer;`
- `import java.nio.channels.SelectionKey;`
- `import java.nio.channels.Selector;`
- `import java.nio.channels.ServerSocketChannel;`
- `import java.nio.channels.SocketChannel;`
- `import java.nio.channels.spi.AbstractSelector;`
- `import java.nio.channels.spi.SelectorProvider;`
- `import java.util.HashMap;`
- `import java.util.Iterator;`
- `import java.util.LinkedList;`
- `import java.util.Map;`
- `import java.util.Set;`
- `import java.util.concurrent.ExecutorService;`
- `import java.util.concurrent.Executors;`
- `public class MultiThreadNIOEchoServer {`
- `    public static Map<Socket, Long> geym_time_stat = new HashMap<Socket, Long>();`
- `    class EchoClient {`
- `       private LinkedList<ByteBuffer> outq;`
- `       EchoClient() {`
- `         outq = new LinkedList<ByteBuffer>();`
- `       }`
- `       public LinkedList<ByteBuffer> getOutputQueue() {`
- `         return outq;`
- `       }`
- `       public void enqueue(ByteBuffer bb) {`
- `         outq.addFirst(bb);`
- `       }`
- `    }`
- `    class HandleMsg implements Runnable {`
- `       SelectionKey sk;`
- `       ByteBuffer bb;`
- `       public HandleMsg(SelectionKey sk, ByteBuffer bb) {`
- `         super();`
- `         this.sk = sk;`
- `         this.bb = bb;`
- `       }`
- `       @Override`
- `       public void run() {`
- `         // TODO Auto-generated method stub`
- `         EchoClient echoClient = (EchoClient) sk.attachment();`
- `         echoClient.enqueue(bb);`
- `         sk.interestOps(SelectionKey.OP_READ | SelectionKey.OP_WRITE);`
- `         selector.wakeup();`
- `       }`
- `    }`
- `    private Selector selector;`
- `    private ExecutorService tp = Executors.newCachedThreadPool();`
- `    private void startServer() throws Exception {`
- `       selector = SelectorProvider.provider().openSelector();`
- `       ServerSocketChannel ssc = ServerSocketChannel.open();`
- `       ssc.configureBlocking(false);`
- `       InetSocketAddress isa = new InetSocketAddress(8000);`
- `       ssc.socket().bind(isa);`
- `       // 注册感兴趣的事件，此处对accpet事件感兴趣`
- `       SelectionKey acceptKey = ssc.register(selector, SelectionKey.OP_ACCEPT);`
- `       for (;;) {`
- `         selector.select();`
- `         Set readyKeys = selector.selectedKeys();`
- `         Iterator i = readyKeys.iterator();`
- `         long e = 0;`
- `         while (i.hasNext()) {`
- `          SelectionKey sk = (SelectionKey) i.next();`
- `          i.remove();`
- `          if (sk.isAcceptable()) {`
- `              doAccept(sk);`
- `          } else if (sk.isValid() && sk.isReadable()) {`
- `              if (!geym_time_stat.containsKey(((SocketChannel) sk`
- `                   .channel()).socket())) {`
- `                 geym_time_stat.put(`
- `                    ((SocketChannel) sk.channel()).socket(),`
- `                    System.currentTimeMillis());`
- `              }`
- `              doRead(sk);`
- `          } else if (sk.isValid() && sk.isWritable()) {`
- `              doWrite(sk);`
- `              e = System.currentTimeMillis();`
- `              long b = geym_time_stat.remove(((SocketChannel) sk`
- `                   .channel()).socket());`
- `              System.out.println("spend:" + (e - b) + "ms");`
- `          }`
- `         }`
- `       }`
- `    }`
- `    private void doWrite(SelectionKey sk) {`
- `       // TODO Auto-generated method stub`
- `       SocketChannel channel = (SocketChannel) sk.channel();`
- `       EchoClient echoClient = (EchoClient) sk.attachment();`
- `       LinkedList<ByteBuffer> outq = echoClient.getOutputQueue();`
- `       ByteBuffer bb = outq.getLast();`
- `       try {`
- `         int len = channel.write(bb);`
- `         if (len == -1) {`
- `          disconnect(sk);`
- `          return;`
- `         }`
- `         if (bb.remaining() == 0) {`
- `          outq.removeLast();`
- `         }`
- `       } catch (Exception e) {`
- `         // TODO: handle exception`
- `         disconnect(sk);`
- `       }`
- `       if (outq.size() == 0) {`
- `         sk.interestOps(SelectionKey.OP_READ);`
- `       }`
- `    }`
- `    private void doRead(SelectionKey sk) {`
- `       // TODO Auto-generated method stub`
- `       SocketChannel channel = (SocketChannel) sk.channel();`
- `       ByteBuffer bb = ByteBuffer.allocate(8192);`
- `       int len;`
- `       try {`
- `         len = channel.read(bb);`
- `         if (len < 0) {`
- `          disconnect(sk);`
- `          return;`
- `         }`
- `       } catch (Exception e) {`
- `         // TODO: handle exception`
- `         disconnect(sk);`
- `         return;`
- `       }`
- `       bb.flip();`
- `       tp.execute(new HandleMsg(sk, bb));`
- `    }`
- `    private void disconnect(SelectionKey sk) {`
- `       // TODO Auto-generated method stub`
- `       //省略略干关闭操作`
- `    }`
- `    private void doAccept(SelectionKey sk) {`
- `       // TODO Auto-generated method stub`
- `       ServerSocketChannel server = (ServerSocketChannel) sk.channel();`
- `       SocketChannel clientChannel;`
- `       try {`
- `         clientChannel = server.accept();`
- `         clientChannel.configureBlocking(false);`
- `         SelectionKey clientKey = clientChannel.register(selector,`
- `              SelectionKey.OP_READ);`
- `         EchoClient echoClinet = new EchoClient();`
- `         clientKey.attach(echoClinet);`
- `         InetAddress clientAddress = clientChannel.socket().getInetAddress();`
- `         System.out.println("Accepted connection from "`
- `              + clientAddress.getHostAddress());`
- `       } catch (Exception e) {`
- `         // TODO: handle exception`
- `       }`
- `    }`
- `    public static void main(String[] args) {`
- `       // TODO Auto-generated method stub`
- `       MultiThreadNIOEchoServer echoServer = new MultiThreadNIOEchoServer();`
- `       try {`
- `         echoServer.startServer();`
- `       } catch (Exception e) {`
- `         // TODO: handle exception`
- `       }`
- `    }`
- `}`


代码仅作参考，主要的特点是，对不同事件的感兴趣来做不同的事。


当用之前模拟的那个延迟的客户端时，这次的时间消耗就在2ms到11ms之间了。性能提升是很明显的。


总结：


1. NIO会将数据准备好后，再交由应用进行处理，数据的读取/写入过程依然在应用线程中完成，只是将等待的时间剥离到单独的线程中去。
- 节省数据准备时间（因为Selector可以复用）

## 5. AIO


AIO的特点：


1. 读完了再通知我


2. 不会加快IO，只是在读完后进行通知


3. 使用回调函数，进行业务处理


AIO的相关代码：


AsynchronousServerSocketChannel
- `server = AsynchronousServerSocketChannel.open().bind( new InetSocketAddress (PORT));`


使用server上的accept方法
- `public abstract <A> void accept(A attachment,CompletionHandler<AsynchronousSocketChannel,? super A> handler);`


CompletionHandler为回调接口，当有客户端accept之后，就做handler中的事情。


示例代码：
- `server.accept(null,`
- `          new CompletionHandler<AsynchronousSocketChannel, Object>() {`
- `              final ByteBuffer buffer = ByteBuffer.allocate(1024);`
- `              public void completed(AsynchronousSocketChannel result,`
- `                   Object attachment) {`
- `                 System.out.println(Thread.currentThread().getName());`
- `                 Future<Integer> writeResult = null;`
- `                 try {`
- `                   buffer.clear();`
- `                   result.read(buffer).get(100, TimeUnit.SECONDS);`
- `                   buffer.flip();`
- `                   writeResult = result.write(buffer);`
- `                 } catch (InterruptedException | ExecutionException e) {`
- `                   e.printStackTrace();`
- `                 } catch (TimeoutException e) {`
- `                   e.printStackTrace();`
- `                 } finally {`
- `                   try {`
- `                    server.accept(null, this);`
- `                    writeResult.get();`
- `                    result.close();`
- `                   } catch (Exception e) {`
- `                    System.out.println(e.toString());`
- `                   }`
- `                 }`
- `              }`
- `              @Override`
- `              public void failed(Throwable exc, Object attachment) {`
- `                 System.out.println("failed: " + exc);`
- `              }`
- `          });`


这里使用了Future来实现即时返回，关于Future请参考[上一篇][Link 1]


在理解了NIO的基础上，看AIO，区别在于AIO是等读写过程完成后再去调用回调函数。


NIO是同步非阻塞的


AIO是异步非阻塞的


由于NIO的读写过程依然在应用线程里完成，所以对于那些读写过程时间长的，NIO就不太适合。


而AIO的读写过程完成后才被通知，所以AIO能够胜任那些重量级，读写过程长的任务。



