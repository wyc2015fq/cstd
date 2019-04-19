# JAVA AIO 服务器与客户端实现示例(代码2) - 零度的博客专栏 - CSDN博客
2016年11月15日 14:27:57[零度anngle](https://me.csdn.net/zmx729618)阅读数：2018
                
AIO用于文件处理还是比较快乐的，但用AIO来写网络消息处理服务器端与客户端是比较麻烦的事情，当然这只是我个人意见，主要是有几点原因：
一是AIO需要操作系统支持，还好Windows与Linux(模拟)都支持；
二是AIO同时使用递归调用和异步调用容易把程序员搞晕，代码容易出错；
三是CompletionHandler会使用单独的线程跑，容易出现多线程问题，频繁线程上下文切换比较消耗资源；
四是异步写要创建队列来缓存需要写入的数据，否则肯定会遇到WritePendingException。
相对来说，NIO比较清楚直白，容易控制。
另外，笔者使用多线程模拟多个客户场景失败，代码在run方法中调用AsynchronousSocketChannel.connect()没返回，没连接上服务器，不知道为何，请大侠指教，最后只好使用多个进程模拟多个客户端，写个类似下面代码的bat文件，同时运行多个。
```
`java -classpath .\ com.stevex.app.aio.Client 1`
`java -classpath .\ com.stevex.app.aio.Client 1`
`pause`
```
服务器代码：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52953974#)[copy](http://blog.csdn.net/z69183787/article/details/52953974#)[print](http://blog.csdn.net/z69183787/article/details/52953974#)[?](http://blog.csdn.net/z69183787/article/details/52953974#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1953542/fork)
- package aio;  
- 
- /**
-  * Created by Administrator on 2016/10/28.
-  */
- import java.io.IOException;  
- import java.net.InetSocketAddress;  
- import java.net.StandardSocketOptions;  
- import java.nio.ByteBuffer;  
- import java.nio.CharBuffer;  
- import java.nio.channels.AsynchronousChannelGroup;  
- import java.nio.channels.AsynchronousServerSocketChannel;  
- import java.nio.channels.AsynchronousSocketChannel;  
- import java.nio.channels.CompletionHandler;  
- //import java.nio.channels.WritePendingException;
- import java.nio.charset.CharacterCodingException;  
- import java.nio.charset.Charset;  
- import java.util.LinkedList;  
- import java.util.Queue;  
- import java.util.concurrent.Executors;  
- 
- publicclass XiaoNa {  
- privatefinal AsynchronousServerSocketChannel server;  
- //写队列，因为当前一个异步写调用还没完成之前，调用异步写会抛WritePendingException
- //所以需要一个写队列来缓存要写入的数据，这是AIO比较坑的地方
- privatefinal Queue<ByteBuffer> queue = new LinkedList<ByteBuffer>();  
- privateboolean writing = false;  
- 
- publicstaticvoid main(String[] args) throws IOException{  
-         XiaoNa xiaona = new XiaoNa();  
-         xiaona.listen();  
-     }  
- 
- public XiaoNa() throws IOException{  
- //设置线程数为CPU核数
-         AsynchronousChannelGroup channelGroup = AsynchronousChannelGroup.withFixedThreadPool(Runtime.getRuntime().availableProcessors(), Executors.defaultThreadFactory());  
-         server = AsynchronousServerSocketChannel.open(channelGroup);  
- //重用端口
-         server.setOption(StandardSocketOptions.SO_REUSEADDR, true);  
- //绑定端口并设置连接请求队列长度
-         server.bind(new InetSocketAddress(8383), 80);  
-     }  
- 
- publicvoid listen() {  
-         System.out.println(Thread.currentThread().getName() + ": run in listen method" );  
- //开始接受第一个连接请求
-         server.accept(null, new CompletionHandler<AsynchronousSocketChannel, Object>(){  
- @Override
- publicvoid completed(AsynchronousSocketChannel channel,  
-                                   Object attachment) {  
-                 System.out.println(Thread.currentThread().getName() + ": run in accept completed method" );  
- 
- //先安排处理下一个连接请求，异步非阻塞调用，所以不用担心挂住了
- //这里传入this是个地雷，小心多线程
-                 server.accept(null, this);  
- //处理连接读写
-                 handle(channel);  
-             }  
- 
- privatevoid handle(final AsynchronousSocketChannel channel) {  
-                 System.out.println(Thread.currentThread().getName() + ": run in handle method" );  
- //每个AsynchronousSocketChannel，分配一个缓冲区
- final ByteBuffer readBuffer = ByteBuffer.allocateDirect(1024);  
-                 readBuffer.clear();  
-                 channel.read(readBuffer, null, new CompletionHandler<Integer, Object>(){  
- 
- @Override
- publicvoid completed(Integer count, Object attachment) {  
-                         System.out.println(Thread.currentThread().getName() + ": run in read completed method" );  
- 
- if(count > 0){  
- try{  
-                                 readBuffer.flip();  
- //CharBuffer charBuffer = CharsetHelper.decode(readBuffer);
-                                 CharBuffer charBuffer = Charset.forName("UTF-8").newDecoder().decode(readBuffer);  
-                                 String question = charBuffer.toString();  
-                                 String answer = Helper.getAnswer(question);  
- /*//写入也是异步调用，也可以使用传入CompletionHandler对象的方式来处理写入结果
-                                 //channel.write(CharsetHelper.encode(CharBuffer.wrap(answer)));
-                                 try{
-                                     channel.write(Charset.forName("UTF-8").newEncoder().encode(CharBuffer.wrap(answer)));
-                                 }
-                                 //Unchecked exception thrown when an attempt is made to write to an asynchronous socket channel and a previous write has not completed.
-                                 //看来操作系统也不可靠
-                                 catch(WritePendingException wpe){
-                                     //休息一秒再重试，如果失败就不管了
-                                     Helper.sleep(1);
-                                     channel.write(Charset.forName("UTF-8").newEncoder().encode(CharBuffer.wrap(answer)));
-                                 }*/
-                                 writeStringMessage(channel, answer);  
- 
-                                 readBuffer.clear();  
-                             }  
- catch(IOException e){  
-                                 e.printStackTrace();  
-                             }  
-                         }  
- else{  
- try {  
- //如果客户端关闭socket，那么服务器也需要关闭，否则浪费CPU
-                                 channel.close();  
-                             } catch (IOException e) {  
-                                 e.printStackTrace();  
-                             }  
-                         }  
- 
- //异步调用OS处理下个读取请求
- //这里传入this是个地雷，小心多线程
-                         channel.read(readBuffer, null, this);  
-                     }  
- 
- /**
-                      * 服务器读失败处理
-                      * @param exc
-                      * @param attachment
-                      */
- @Override
- publicvoid failed(Throwable exc, Object attachment) {  
-                         System.out.println("server read failed: " + exc);  
- if(channel != null){  
- try {  
-                                 channel.close();  
-                             } catch (IOException e) {  
-                                 e.printStackTrace();  
-                             }  
-                         }  
-                     }  
- 
-                 });  
-             }  
- 
- /**
-              * 服务器接受连接失败处理
-              * @param exc
-              * @param attachment
-              */
- @Override
- publicvoid failed(Throwable exc, Object attachment) {  
-                 System.out.println("server accept failed: " + exc);  
-             }  
- 
-         });  
-     }  
- 
- /**
-      * Enqueues a write of the buffer to the channel.
-      * The call is asynchronous so the buffer is not safe to modify after
-      * passing the buffer here.
-      *
-      * @param buffer the buffer to send to the channel
-      */
- privatevoid writeMessage(final AsynchronousSocketChannel channel, final ByteBuffer buffer) {  
- boolean threadShouldWrite = false;  
- 
- synchronized(queue) {  
-             queue.add(buffer);  
- // Currently no thread writing, make this thread dispatch a write
- if (!writing) {  
-                 writing = true;  
-                 threadShouldWrite = true;  
-             }  
-         }  
- 
- if (threadShouldWrite) {  
-             writeFromQueue(channel);  
-         }  
-     }  
- 
- privatevoid writeFromQueue(final AsynchronousSocketChannel channel) {  
-         ByteBuffer buffer;  
- 
- synchronized (queue) {  
-             buffer = queue.poll();  
- if (buffer == null) {  
-                 writing = false;  
-             }  
-         }  
- 
- // No new data in buffer to write
- if (writing) {  
-             writeBuffer(channel, buffer);  
-         }  
-     }  
- 
- privatevoid writeBuffer(final AsynchronousSocketChannel channel, ByteBuffer buffer) {  
-         channel.write(buffer, buffer, new CompletionHandler<Integer, ByteBuffer>() {  
- @Override
- publicvoid completed(Integer result, ByteBuffer buffer) {  
- if (buffer.hasRemaining()) {  
-                     channel.write(buffer, buffer, this);  
-                 } else {  
- // Go back and check if there is new data to write
-                     writeFromQueue(channel);  
-                 }  
-             }  
- 
- @Override
- publicvoid failed(Throwable exc, ByteBuffer attachment) {  
-                 System.out.println("server write failed: " + exc);  
-             }  
-         });  
-     }  
- 
- /**
-      * Sends a message
-      * @param msg
-      * @throws CharacterCodingException
-      */
- privatevoid writeStringMessage(final AsynchronousSocketChannel channel, String msg) throws CharacterCodingException {  
-         writeMessage(channel, Charset.forName("UTF-8").newEncoder().encode(CharBuffer.wrap(msg)));  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package aio;
/**
 * Created by Administrator on 2016/10/28.
 */
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.StandardSocketOptions;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.channels.AsynchronousChannelGroup;
import java.nio.channels.AsynchronousServerSocketChannel;
import java.nio.channels.AsynchronousSocketChannel;
import java.nio.channels.CompletionHandler;
//import java.nio.channels.WritePendingException;
import java.nio.charset.CharacterCodingException;
import java.nio.charset.Charset;
import java.util.LinkedList;
import java.util.Queue;
import java.util.concurrent.Executors;
public class XiaoNa {
    private final AsynchronousServerSocketChannel server;
    //写队列，因为当前一个异步写调用还没完成之前，调用异步写会抛WritePendingException
    //所以需要一个写队列来缓存要写入的数据，这是AIO比较坑的地方
    private final Queue<ByteBuffer> queue = new LinkedList<ByteBuffer>();
    private boolean writing = false;
    public static void main(String[] args) throws IOException{
        XiaoNa xiaona = new XiaoNa();
        xiaona.listen();
    }
    public XiaoNa() throws IOException{
        //设置线程数为CPU核数
        AsynchronousChannelGroup channelGroup = AsynchronousChannelGroup.withFixedThreadPool(Runtime.getRuntime().availableProcessors(), Executors.defaultThreadFactory());
        server = AsynchronousServerSocketChannel.open(channelGroup);
        //重用端口
        server.setOption(StandardSocketOptions.SO_REUSEADDR, true);
        //绑定端口并设置连接请求队列长度
        server.bind(new InetSocketAddress(8383), 80);
    }
    public void listen() {
        System.out.println(Thread.currentThread().getName() + ": run in listen method" );
        //开始接受第一个连接请求
        server.accept(null, new CompletionHandler<AsynchronousSocketChannel, Object>(){
            @Override
            public void completed(AsynchronousSocketChannel channel,
                                  Object attachment) {
                System.out.println(Thread.currentThread().getName() + ": run in accept completed method" );
                //先安排处理下一个连接请求，异步非阻塞调用，所以不用担心挂住了
                //这里传入this是个地雷，小心多线程
                server.accept(null, this);
                //处理连接读写
                handle(channel);
            }
            private void handle(final AsynchronousSocketChannel channel) {
                System.out.println(Thread.currentThread().getName() + ": run in handle method" );
                //每个AsynchronousSocketChannel，分配一个缓冲区
                final ByteBuffer readBuffer = ByteBuffer.allocateDirect(1024);
                readBuffer.clear();
                channel.read(readBuffer, null, new CompletionHandler<Integer, Object>(){
                    @Override
                    public void completed(Integer count, Object attachment) {
                        System.out.println(Thread.currentThread().getName() + ": run in read completed method" );
                        if(count > 0){
                            try{
                                readBuffer.flip();
                                //CharBuffer charBuffer = CharsetHelper.decode(readBuffer);
                                CharBuffer charBuffer = Charset.forName("UTF-8").newDecoder().decode(readBuffer);
                                String question = charBuffer.toString();
                                String answer = Helper.getAnswer(question);
                                /*//写入也是异步调用，也可以使用传入CompletionHandler对象的方式来处理写入结果
                                //channel.write(CharsetHelper.encode(CharBuffer.wrap(answer)));
                                try{
                                    channel.write(Charset.forName("UTF-8").newEncoder().encode(CharBuffer.wrap(answer)));
                                }
                                //Unchecked exception thrown when an attempt is made to write to an asynchronous socket channel and a previous write has not completed.
                                //看来操作系统也不可靠
                                catch(WritePendingException wpe){
                                    //休息一秒再重试，如果失败就不管了
                                    Helper.sleep(1);
                                    channel.write(Charset.forName("UTF-8").newEncoder().encode(CharBuffer.wrap(answer)));
                                }*/
                                writeStringMessage(channel, answer);
                                readBuffer.clear();
                            }
                            catch(IOException e){
                                e.printStackTrace();
                            }
                        }
                        else{
                            try {
                                //如果客户端关闭socket，那么服务器也需要关闭，否则浪费CPU
                                channel.close();
                            } catch (IOException e) {
                                e.printStackTrace();
                            }
                        }
                        //异步调用OS处理下个读取请求
                        //这里传入this是个地雷，小心多线程
                        channel.read(readBuffer, null, this);
                    }
                    /**
                     * 服务器读失败处理
                     * @param exc
                     * @param attachment
                     */
                    @Override
                    public void failed(Throwable exc, Object attachment) {
                        System.out.println("server read failed: " + exc);
                        if(channel != null){
                            try {
                                channel.close();
                            } catch (IOException e) {
                                e.printStackTrace();
                            }
                        }
                    }
                });
            }
            /**
             * 服务器接受连接失败处理
             * @param exc
             * @param attachment
             */
            @Override
            public void failed(Throwable exc, Object attachment) {
                System.out.println("server accept failed: " + exc);
            }
        });
    }
    /**
     * Enqueues a write of the buffer to the channel.
     * The call is asynchronous so the buffer is not safe to modify after
     * passing the buffer here.
     *
     * @param buffer the buffer to send to the channel
     */
    private void writeMessage(final AsynchronousSocketChannel channel, final ByteBuffer buffer) {
        boolean threadShouldWrite = false;
        synchronized(queue) {
            queue.add(buffer);
            // Currently no thread writing, make this thread dispatch a write
            if (!writing) {
                writing = true;
                threadShouldWrite = true;
            }
        }
        if (threadShouldWrite) {
            writeFromQueue(channel);
        }
    }
    private void writeFromQueue(final AsynchronousSocketChannel channel) {
        ByteBuffer buffer;
        synchronized (queue) {
            buffer = queue.poll();
            if (buffer == null) {
                writing = false;
            }
        }
        // No new data in buffer to write
        if (writing) {
            writeBuffer(channel, buffer);
        }
    }
    private void writeBuffer(final AsynchronousSocketChannel channel, ByteBuffer buffer) {
        channel.write(buffer, buffer, new CompletionHandler<Integer, ByteBuffer>() {
            @Override
            public void completed(Integer result, ByteBuffer buffer) {
                if (buffer.hasRemaining()) {
                    channel.write(buffer, buffer, this);
                } else {
                    // Go back and check if there is new data to write
                    writeFromQueue(channel);
                }
            }
            @Override
            public void failed(Throwable exc, ByteBuffer attachment) {
                System.out.println("server write failed: " + exc);
            }
        });
    }
    /**
     * Sends a message
     * @param msg
     * @throws CharacterCodingException
     */
    private void writeStringMessage(final AsynchronousSocketChannel channel, String msg) throws CharacterCodingException {
        writeMessage(channel, Charset.forName("UTF-8").newEncoder().encode(CharBuffer.wrap(msg)));
    }
}
```
客户端代码：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52953974#)[copy](http://blog.csdn.net/z69183787/article/details/52953974#)[print](http://blog.csdn.net/z69183787/article/details/52953974#)[?](http://blog.csdn.net/z69183787/article/details/52953974#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1953542/fork)
- package aio;  
- 
- /**
-  * Created by Administrator on 2016/10/28.
-  */
- import java.io.IOException;  
- import java.net.InetSocketAddress;  
- import java.net.StandardSocketOptions;  
- import java.nio.ByteBuffer;  
- import java.nio.CharBuffer;  
- import java.nio.channels.AsynchronousChannelGroup;  
- import java.nio.channels.AsynchronousSocketChannel;  
- import java.nio.channels.CompletionHandler;  
- import java.nio.charset.CharacterCodingException;  
- import java.nio.charset.Charset;  
- import java.util.LinkedList;  
- import java.util.Queue;  
- import java.util.concurrent.CountDownLatch;  
- import java.util.concurrent.Executors;  
- 
- publicclass Client implements Runnable{  
- private AsynchronousSocketChannel channel;  
- private Helper helper;  
- private CountDownLatch latch;  
- privatefinal Queue<ByteBuffer> queue = new LinkedList<ByteBuffer>();  
- privateboolean writing = false;  
- 
- public Client(AsynchronousChannelGroup channelGroup, CountDownLatch latch) throws IOException, InterruptedException{  
- this.latch = latch;  
-         helper = new Helper();  
-         initChannel(channelGroup);  
-     }  
- 
- privatevoid initChannel(AsynchronousChannelGroup channelGroup) throws IOException {  
- //在默认channel group下创建一个socket channel
-         channel = AsynchronousSocketChannel.open(channelGroup);  
- //设置Socket选项
-         channel.setOption(StandardSocketOptions.TCP_NODELAY, true);  
-         channel.setOption(StandardSocketOptions.SO_KEEPALIVE, true);  
-         channel.setOption(StandardSocketOptions.SO_REUSEADDR, true);  
-     }  
- 
- publicstaticvoid main(String[] args) throws IOException, InterruptedException {  
- int sleepTime = Integer.parseInt(args[0]);  
-         Helper.sleep(sleepTime);  
- 
-         AsynchronousChannelGroup channelGroup = AsynchronousChannelGroup.withFixedThreadPool(Runtime.getRuntime().availableProcessors(), Executors.defaultThreadFactory());  
- //只能跑一个线程，第二个线程connect会挂住，暂时不明原因
- finalint THREAD_NUM = 1;  
-         CountDownLatch latch = new CountDownLatch(THREAD_NUM);  
- 
- //创建个多线程模拟多个客户端，模拟失败，无效
- //只能通过命令行同时运行多个进程来模拟多个客户端
- for(int i=0; i<THREAD_NUM; i++){  
-             Client c = new Client(channelGroup, latch);  
-             Thread t = new Thread(c);  
-             System.out.println(t.getName() + "---start");  
-             t.start();  
- //让主线程等待子线程处理再退出, 这对于异步调用无效
- //t.join();
-         }  
- 
-         latch.await();  
- 
- if(channelGroup !=null){  
-             channelGroup.shutdown();  
-         }  
-     }  
- 
- @Override
- publicvoid run() {  
-         System.out.println(Thread.currentThread().getName() + "---run");  
- 
- //连接服务器
-         channel.connect(new InetSocketAddress("localhost", 8383), null, new CompletionHandler<Void, Void>(){  
- final ByteBuffer readBuffer = ByteBuffer.allocateDirect(1024);  
- 
- @Override
- publicvoid completed(Void result, Void attachment) {  
- //连接成功后, 异步调用OS向服务器写一条消息
- try {  
- //channel.write(CharsetHelper.encode(CharBuffer.wrap(helper.getWord())));
-                     writeStringMessage(helper.getWord());  
-                 } catch (CharacterCodingException e) {  
-                     e.printStackTrace();  
-                 }  
- 
- //helper.sleep();//等待写异步调用完成
-                 readBuffer.clear();  
- //异步调用OS读取服务器发送的消息
-                 channel.read(readBuffer, null, new CompletionHandler<Integer, Object>(){  
- 
- @Override
- publicvoid completed(Integer result, Object attachment) {  
- try{  
- //异步读取完成后处理
- if(result > 0){  
-                                 readBuffer.flip();  
-                                 CharBuffer charBuffer = CharsetHelper.decode(readBuffer);  
-                                 String answer = charBuffer.toString();  
-                                 System.out.println(Thread.currentThread().getName() + "---" + answer);  
-                                 readBuffer.clear();  
- 
-                                 String word = helper.getWord();  
- if(word != null){  
- //异步写
- //channel.write(CharsetHelper.encode(CharBuffer.wrap(word)));
-                                     writeStringMessage(word);  
- //helper.sleep();//等待异步操作
-                                     channel.read(readBuffer, null, this);  
-                                 }  
- else{  
- //不想发消息了，主动关闭channel
-                                     shutdown();  
-                                 }  
-                             }  
- else{  
- //对方已经关闭channel，自己被动关闭，避免空循环
-                                 shutdown();  
-                             }  
-                         }  
- catch(Exception e){  
-                             e.printStackTrace();  
-                         }  
-                     }  
- 
- /**
-                      * 读取失败处理
-                      * @param exc
-                      * @param attachment
-                      */
- @Override
- publicvoid failed(Throwable exc, Object attachment) {  
-                         System.out.println("client read failed: " + exc);  
- try {  
-                             shutdown();  
-                         } catch (IOException e) {  
-                             e.printStackTrace();  
-                         }  
-                     }  
- 
-                 });  
-             }  
- 
- /**
-              * 连接失败处理
-              * @param exc
-              * @param attachment
-              */
- @Override
- publicvoid failed(Throwable exc, Void attachment) {  
-                 System.out.println("client connect to server failed: " + exc);  
- 
- try {  
-                     shutdown();  
-                 } catch (IOException e) {  
-                     e.printStackTrace();  
-                 }  
-             }  
-         });  
-     }  
- 
- privatevoid shutdown() throws IOException {  
- if(channel != null){  
-             channel.close();  
-         }  
- 
-         latch.countDown();  
-     }  
- 
- /**
-      * Enqueues a write of the buffer to the channel.
-      * The call is asynchronous so the buffer is not safe to modify after
-      * passing the buffer here.
-      *
-      * @param buffer the buffer to send to the channel
-      */
- privatevoid writeMessage(final ByteBuffer buffer) {  
- boolean threadShouldWrite = false;  
- 
- synchronized(queue) {  
-             queue.add(buffer);  
- // Currently no thread writing, make this thread dispatch a write
- if (!writing) {  
-                 writing = true;  
-                 threadShouldWrite = true;  
-             }  
-         }  
- 
- if (threadShouldWrite) {  
-             writeFromQueue();  
-         }  
-     }  
- 
- privatevoid writeFromQueue() {  
-         ByteBuffer buffer;  
- 
- synchronized (queue) {  
-             buffer = queue.poll();  
- if (buffer == null) {  
-                 writing = false;  
-             }  
-         }  
- 
- // No new data in buffer to write
- if (writing) {  
-             writeBuffer(buffer);  
-         }  
-     }  
- 
- privatevoid writeBuffer(ByteBuffer buffer) {  
-         channel.write(buffer, buffer, new CompletionHandler<Integer, ByteBuffer>() {  
- @Override
- publicvoid completed(Integer result, ByteBuffer buffer) {  
- if (buffer.hasRemaining()) {  
-                     channel.write(buffer, buffer, this);  
-                 } else {  
- // Go back and check if there is new data to write
-                     writeFromQueue();  
-                 }  
-             }  
- 
- @Override
- publicvoid failed(Throwable exc, ByteBuffer attachment) {  
-             }  
-         });  
-     }  
- 
- /**
-      * Sends a message
-      * @param msg
-      * @throws CharacterCodingException
-      */
- publicvoid writeStringMessage(String msg) throws CharacterCodingException {  
-         writeMessage(Charset.forName("UTF-8").newEncoder().encode(CharBuffer.wrap(msg)));  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package aio;
/**
 * Created by Administrator on 2016/10/28.
 */
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.StandardSocketOptions;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.channels.AsynchronousChannelGroup;
import java.nio.channels.AsynchronousSocketChannel;
import java.nio.channels.CompletionHandler;
import java.nio.charset.CharacterCodingException;
import java.nio.charset.Charset;
import java.util.LinkedList;
import java.util.Queue;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.Executors;
public class Client implements Runnable{
    private AsynchronousSocketChannel channel;
    private Helper helper;
    private CountDownLatch latch;
    private final Queue<ByteBuffer> queue = new LinkedList<ByteBuffer>();
    private boolean writing = false;
    public Client(AsynchronousChannelGroup channelGroup, CountDownLatch latch) throws IOException, InterruptedException{
        this.latch = latch;
        helper = new Helper();
        initChannel(channelGroup);
    }
    private void initChannel(AsynchronousChannelGroup channelGroup) throws IOException {
        //在默认channel group下创建一个socket channel
        channel = AsynchronousSocketChannel.open(channelGroup);
        //设置Socket选项
        channel.setOption(StandardSocketOptions.TCP_NODELAY, true);
        channel.setOption(StandardSocketOptions.SO_KEEPALIVE, true);
        channel.setOption(StandardSocketOptions.SO_REUSEADDR, true);
    }
    public static void main(String[] args) throws IOException, InterruptedException {
        int sleepTime = Integer.parseInt(args[0]);
        Helper.sleep(sleepTime);
        AsynchronousChannelGroup channelGroup = AsynchronousChannelGroup.withFixedThreadPool(Runtime.getRuntime().availableProcessors(), Executors.defaultThreadFactory());
        //只能跑一个线程，第二个线程connect会挂住，暂时不明原因
        final int THREAD_NUM = 1;
        CountDownLatch latch = new CountDownLatch(THREAD_NUM);
        //创建个多线程模拟多个客户端，模拟失败，无效
        //只能通过命令行同时运行多个进程来模拟多个客户端
        for(int i=0; i<THREAD_NUM; i++){
            Client c = new Client(channelGroup, latch);
            Thread t = new Thread(c);
            System.out.println(t.getName() + "---start");
            t.start();
            //让主线程等待子线程处理再退出, 这对于异步调用无效
            //t.join();
        }
        latch.await();
        if(channelGroup !=null){
            channelGroup.shutdown();
        }
    }
    @Override
    public void run() {
        System.out.println(Thread.currentThread().getName() + "---run");
        //连接服务器
        channel.connect(new InetSocketAddress("localhost", 8383), null, new CompletionHandler<Void, Void>(){
            final ByteBuffer readBuffer = ByteBuffer.allocateDirect(1024);
            @Override
            public void completed(Void result, Void attachment) {
                //连接成功后, 异步调用OS向服务器写一条消息
                try {
                    //channel.write(CharsetHelper.encode(CharBuffer.wrap(helper.getWord())));
                    writeStringMessage(helper.getWord());
                } catch (CharacterCodingException e) {
                    e.printStackTrace();
                }
                //helper.sleep();//等待写异步调用完成
                readBuffer.clear();
                //异步调用OS读取服务器发送的消息
                channel.read(readBuffer, null, new CompletionHandler<Integer, Object>(){
                    @Override
                    public void completed(Integer result, Object attachment) {
                        try{
                            //异步读取完成后处理
                            if(result > 0){
                                readBuffer.flip();
                                CharBuffer charBuffer = CharsetHelper.decode(readBuffer);
                                String answer = charBuffer.toString();
                                System.out.println(Thread.currentThread().getName() + "---" + answer);
                                readBuffer.clear();
                                String word = helper.getWord();
                                if(word != null){
                                    //异步写
                                    //channel.write(CharsetHelper.encode(CharBuffer.wrap(word)));
                                    writeStringMessage(word);
                                    //helper.sleep();//等待异步操作
                                    channel.read(readBuffer, null, this);
                                }
                                else{
                                    //不想发消息了，主动关闭channel
                                    shutdown();
                                }
                            }
                            else{
                                //对方已经关闭channel，自己被动关闭，避免空循环
                                shutdown();
                            }
                        }
                        catch(Exception e){
                            e.printStackTrace();
                        }
                    }
                    /**
                     * 读取失败处理
                     * @param exc
                     * @param attachment
                     */
                    @Override
                    public void failed(Throwable exc, Object attachment) {
                        System.out.println("client read failed: " + exc);
                        try {
                            shutdown();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                });
            }
            /**
             * 连接失败处理
             * @param exc
             * @param attachment
             */
            @Override
            public void failed(Throwable exc, Void attachment) {
                System.out.println("client connect to server failed: " + exc);
                try {
                    shutdown();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
    }
    private void shutdown() throws IOException {
        if(channel != null){
            channel.close();
        }
        latch.countDown();
    }
    /**
     * Enqueues a write of the buffer to the channel.
     * The call is asynchronous so the buffer is not safe to modify after
     * passing the buffer here.
     *
     * @param buffer the buffer to send to the channel
     */
    private void writeMessage(final ByteBuffer buffer) {
        boolean threadShouldWrite = false;
        synchronized(queue) {
            queue.add(buffer);
            // Currently no thread writing, make this thread dispatch a write
            if (!writing) {
                writing = true;
                threadShouldWrite = true;
            }
        }
        if (threadShouldWrite) {
            writeFromQueue();
        }
    }
    private void writeFromQueue() {
        ByteBuffer buffer;
        synchronized (queue) {
            buffer = queue.poll();
            if (buffer == null) {
                writing = false;
            }
        }
        // No new data in buffer to write
        if (writing) {
            writeBuffer(buffer);
        }
    }
    private void writeBuffer(ByteBuffer buffer) {
        channel.write(buffer, buffer, new CompletionHandler<Integer, ByteBuffer>() {
            @Override
            public void completed(Integer result, ByteBuffer buffer) {
                if (buffer.hasRemaining()) {
                    channel.write(buffer, buffer, this);
                } else {
                    // Go back and check if there is new data to write
                    writeFromQueue();
                }
            }
            @Override
            public void failed(Throwable exc, ByteBuffer attachment) {
            }
        });
    }
    /**
     * Sends a message
     * @param msg
     * @throws CharacterCodingException
     */
    public void writeStringMessage(String msg) throws CharacterCodingException {
        writeMessage(Charset.forName("UTF-8").newEncoder().encode(CharBuffer.wrap(msg)));
    }
}
```

工具类：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52953974#)[copy](http://blog.csdn.net/z69183787/article/details/52953974#)[print](http://blog.csdn.net/z69183787/article/details/52953974#)[?](http://blog.csdn.net/z69183787/article/details/52953974#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1953542/fork)
- package aio;  
- 
- /**
-  * Created by Administrator on 2016/10/28.
-  */
- import java.util.Random;  
- import java.util.concurrent.ArrayBlockingQueue;  
- import java.util.concurrent.BlockingQueue;  
- import java.util.concurrent.TimeUnit;  
- 
- publicclass Helper {  
- privatestatic BlockingQueue<String> words;  
- privatestatic Random random;  
- 
- public Helper() throws InterruptedException{  
-         words = new ArrayBlockingQueue<String>(5);  
-         words.put("hi");  
-         words.put("who");  
-         words.put("what");  
-         words.put("where");  
-         words.put("bye");  
- 
-         random = new Random();  
-     }  
- 
- public String getWord(){  
- return words.poll();  
-     }  
- 
- publicvoid sleep() {  
- try {  
-             TimeUnit.SECONDS.sleep(random.nextInt(3));  
-         } catch (InterruptedException e) {  
-             e.printStackTrace();  
-         }  
-     }  
- 
- publicstaticvoid sleep(long l) {  
- try {  
-             TimeUnit.SECONDS.sleep(l);  
-         } catch (InterruptedException e) {  
-             e.printStackTrace();  
-         }  
-     }  
- 
- publicstatic String getAnswer(String question){  
-         String answer = null;  
- 
- switch(question){  
- case"who":  
-                 answer = "我是小娜\n";  
- break;  
- case"what":  
-                 answer = "我是来帮你解闷的\n";  
- break;  
- case"where":  
-                 answer = "我来自外太空\n";  
- break;  
- case"hi":  
-                 answer = "hello\n";  
- break;  
- case"bye":  
-                 answer = "88\n";  
- break;  
- default:  
-                 answer = "请输入 who， 或者what， 或者where";  
-         }  
- 
- return answer;  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package aio;
/**
 * Created by Administrator on 2016/10/28.
 */
import java.util.Random;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.TimeUnit;
public class Helper {
    private static BlockingQueue<String> words;
    private static Random random;
    public Helper() throws InterruptedException{
        words = new ArrayBlockingQueue<String>(5);
        words.put("hi");
        words.put("who");
        words.put("what");
        words.put("where");
        words.put("bye");
        random = new Random();
    }
    public String getWord(){
        return words.poll();
    }
    public void sleep() {
        try {
            TimeUnit.SECONDS.sleep(random.nextInt(3));
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
    public static void sleep(long l) {
        try {
            TimeUnit.SECONDS.sleep(l);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
    public static String getAnswer(String question){
        String answer = null;
        switch(question){
            case "who":
                answer = "我是小娜\n";
                break;
            case "what":
                answer = "我是来帮你解闷的\n";
                break;
            case "where":
                answer = "我来自外太空\n";
                break;
            case "hi":
                answer = "hello\n";
                break;
            case "bye":
                answer = "88\n";
                break;
            default:
                answer = "请输入 who， 或者what， 或者where";
        }
        return answer;
    }
}
```
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52953974#)[copy](http://blog.csdn.net/z69183787/article/details/52953974#)[print](http://blog.csdn.net/z69183787/article/details/52953974#)[?](http://blog.csdn.net/z69183787/article/details/52953974#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1953542/fork)
- package aio;  
- 
- /**
-  * Created by Administrator on 2016/10/28.
-  */
- 
- import java.nio.ByteBuffer;  
- import java.nio.CharBuffer;  
- import java.nio.charset.CharacterCodingException;  
- import java.nio.charset.Charset;  
- import java.nio.charset.CharsetDecoder;  
- import java.nio.charset.CharsetEncoder;  
- 
- publicclass CharsetHelper {  
- privatestaticfinal String UTF_8 = "UTF-8";  
- privatestatic CharsetEncoder encoder = Charset.forName(UTF_8).newEncoder();  
- privatestatic CharsetDecoder decoder = Charset.forName(UTF_8).newDecoder();  
- 
- publicstatic ByteBuffer encode(CharBuffer in) throws CharacterCodingException{  
- return encoder.encode(in);  
-     }  
- 
- publicstatic CharBuffer decode(ByteBuffer in) throws CharacterCodingException{  
- return decoder.decode(in);  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package aio;
/**
 * Created by Administrator on 2016/10/28.
 */
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.charset.CharacterCodingException;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import java.nio.charset.CharsetEncoder;
public class CharsetHelper {
    private static final String UTF_8 = "UTF-8";
    private static CharsetEncoder encoder = Charset.forName(UTF_8).newEncoder();
    private static CharsetDecoder decoder = Charset.forName(UTF_8).newDecoder();
    public static ByteBuffer encode(CharBuffer in) throws CharacterCodingException{
        return encoder.encode(in);
    }
    public static CharBuffer decode(ByteBuffer in) throws CharacterCodingException{
        return decoder.decode(in);
    }
}
```
