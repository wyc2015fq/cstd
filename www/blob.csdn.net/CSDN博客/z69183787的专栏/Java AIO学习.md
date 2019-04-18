# Java AIO学习 - z69183787的专栏 - CSDN博客
2016年10月27日 15:56:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：437
转载Reactor and Proactor的内容
系统I/O 可分为阻塞型, 非阻塞同步型以及非阻塞异步型[[1](http://www.cppblog.com/pansunyou/archive/2011/01/26/io_design_patterns.html#resources), [2](http://www.cppblog.com/pansunyou/archive/2011/01/26/io_design_patterns.html#resources)].
 阻塞型I/O意味着控制权只到调用操作结束了才会回到调用者手里. 结果调用者被阻塞了, 这段时间了做不了任何其它事情. 更郁闷的是,在等待IO结果的时间里,调用者所在线程此时无法腾出手来去响应其它的请求，这真是太浪费资源了。拿read()操作来说吧, 调用此函数的代码会一直僵在此处直至它所读的socket缓存中有数据到来.
相比之下，非阻塞同步是会立即返回控制权给调用者的。调用者不需要等等，它从调用的函数获取两种结果：要么此次调用成功进行了;要么系统返回错误标识告诉调用者当前资源不可用，你再等等或者再试度看吧。比如read()操作, 如果当前socket无数据可读，则立即返回**EWOULBLOCK/EAGAIN**，告诉调用read()者"数据还没准备好，你稍后再试".
在非阻塞异步调用中，稍有不同。调用函数在立即返回时，还告诉调用者，这次请求已经开始了。系统会使用另外的资源或者线程来完成这次调用操作，并在完成的时候知会调用者（比如通过回调函数）。拿Windows的**ReadFile()**或者POSIX的**aio_read()**来说,调用它之后，函数立即返回，[操作系统](http://lib.csdn.net/base/operatingsystem)在后台同时开始读操作。
在以上三种IO形式中，非阻塞异步是性能最高、伸缩性最好的。
这篇文章探讨不同的I/O利用机制并提供一种跨平台的设计模式(解决方案). 希望此文可以给于TCP高性能服务器开发者一些帮助，选择最佳的设计方案。下面我们会比较 [Java](http://lib.csdn.net/base/javaee),
 c#, C++各自对探讨方案的实现以及性能. 我们在文章的后面就不再提及阻塞式的方案了，因为阻塞式I/O实在是缺少可伸缩性，性能也达不到高性能服务器的要求。
**两种IO多路复用方案:Reactor and Proactor**
一般情况下，I/O 复用机制需要**事件分享器**(event demultiplexor [[1](http://www.cppblog.com/pansunyou/archive/2011/01/26/io_design_patterns.html#resources), [3](http://www.cppblog.com/pansunyou/archive/2011/01/26/io_design_patterns.html#resources)]).
 事件分享器的作用，即将那些读写事件源分发给各读写事件的处理者，就像送快递的在楼下喊: 谁的什么东西送了, 快来拿吧。开发人员在开始的时候需要在分享器那里注册感兴趣的事件，并提供相应的处理者(event handlers)，或者是回调函数; 事件分享器在适当的时候会将请求的事件分发给这些handler或者回调函数.
涉及到事件分享器的两种模式称为：Reactor and Proactor [[1](http://www.cppblog.com/pansunyou/archive/2011/01/26/io_design_patterns.html#resources)]. Reactor模式是基于同步I/O的，而Proactor模式是和异步I/O相关的. 在Reactor模式中，事件分离者等待某个事件或者可应用或个操作的状态发生（比如文件描述符可读写，或者是socket可读写）,事件分离者就把这个事件传给事先注册的事件处理函数或者回调函数，由后者来做实际的读写操作。
而在Proactor模式中，事件处理者(或者代由事件分离者发起)直接发起一个异步读写操作(相当于请求)，而实际的工作是由操作系统来完成的。发起时，需要提供的参数包括用于存放读到数据的缓存区，读的数据大小，或者用于存放外发数据的缓存区，以及这个请求完后的回调函数等信息。事件分离者得知了这个请求，它默默等待这个请求的完成，然后转发完成事件给相应的事件处理者或者回调。举例来说，在Windows上事件处理者投递了一个异步IO操作(称有overlapped的技术)，事件分离者等IOCompletion事件完成[[1](http://www.cppblog.com/pansunyou/archive/2011/01/26/io_design_patterns.html#resources)].
 这种异步模式的典型实现是基于操作系统底层异步API的，所以我们可称之为“系统级别”的或者“真正意义上”的异步，因为具体的读写是由操作系统代劳的。
举另外个例子来更好地理解Reactor与Proactor两种模式的区别。这里我们只关注read操作，因为write操作也是差不多的。下面是Reactor的做法：
- 某个事件处理者宣称它对某个socket上的读事件很感兴趣;
- 事件分离者等着这个事件的发生;
- 当事件发生了，事件分离器被唤醒，这负责通知先前那个事件处理者;
- 事件处理者收到消息，于是去那个socket上读数据了. 如果需要，它再次宣称对这个socket上的读事件感兴趣，一直重复上面的步骤;
下面再来看看真正意义的异步模式Proactor是如何做的：
- 事件处理者直接投递发一个写操作(当然，操作系统必须支持这个异步操作). 这个时候，事件处理者根本不关心读事件，它只管发这么个请求，它魂牵梦萦的是这个**写操作的完成事件**。这个处理者很拽，发个命令就不管具体的事情了，只等着别人（系统）帮他搞定的时候给他回个话。
- 事件分离者等着这个读事件的完成(比较下与Reactor的不同);
- 当事件分离者默默等待完成事情到来的同时，操作系统已经在一边开始干活了，它从目标读取数据，放入用户提供的缓存区中，最后通知事件分离者，这个事情我搞完了;
- 事件分享者通知之前的事件处理者: 你吩咐的事情搞定了;
- 事件处理者这时会发现想要读的数据已经乖乖地放在他提供的缓存区中，想怎么处理都行了。如果有需要，事件处理者还像之前一样发起另外一个写操作，和上面的几个步骤一样。
jdk 7中已经内置了AIO的实现，可以参考[http://www.iteye.com/topic/472333](http://www.iteye.com/topic/472333) 这个对aio的分析
我们试试jdk7的AIO的示例
服务器端程序
**[java]**[view
 plain](http://blog.csdn.net/zhongweijian/article/details/8005444#)[copy](http://blog.csdn.net/zhongweijian/article/details/8005444#)
- import java.io.IOException;  
- import java.net.InetSocketAddress;  
- import java.nio.ByteBuffer;  
- import java.nio.channels.AsynchronousChannelGroup;  
- import java.nio.channels.AsynchronousServerSocketChannel;  
- import java.nio.channels.AsynchronousSocketChannel;  
- import java.nio.channels.CompletionHandler;  
- import java.util.concurrent.ExecutionException;  
- import java.util.concurrent.ExecutorService;  
- import java.util.concurrent.Executors;  
- 
- 
- publicclass EchoAioServer {  
- 
- privatefinalint port;  
- 
- publicstaticvoid main(String args[]) {  
- int port = 8000;  
- new EchoAioServer(port);  
-     }  
- 
- public EchoAioServer(int port) {  
- this.port = port;  
-         listen();  
-     }  
- 
- privatevoid listen() {  
- try {  
-             ExecutorService executorService = Executors.newCachedThreadPool();  
-             AsynchronousChannelGroup threadGroup = AsynchronousChannelGroup.withCachedThreadPool(executorService, 1);  
- try (AsynchronousServerSocketChannel server = AsynchronousServerSocketChannel.open(threadGroup)) {  
-                 server.bind(new InetSocketAddress(port));  
- 
-                 System.out.println("Echo listen on " + port);  
- 
-                 server.accept(null, new CompletionHandler<AsynchronousSocketChannel, Object>() {  
- final ByteBuffer echoBuffer = ByteBuffer.allocateDirect(1024);  
- publicvoid completed(AsynchronousSocketChannel result, Object attachment) {  
-                         System.out.println("waiting ....");  
- try {  
-                             echoBuffer.clear();  
-                             result.read(echoBuffer).get();  
-                             echoBuffer.flip();  
- // echo data
-                             result.write(echoBuffer);  
-                             echoBuffer.flip();  
- //                            System.out.println("Echoed '" + new String(echoBuffer.array()) + "' to " + result);
-                         } catch (InterruptedException | ExecutionException e) {  
-                             System.out.println(e.toString());  
-                         } finally {  
- try {  
-                                 result.close();  
-                                 server.accept(null, this);  
-                             } catch (Exception e) {  
-                                 System.out.println(e.toString());  
-                             }  
-                         }  
- 
-                         System.out.println("done...");  
-                     }  
- 
- @Override
- publicvoid failed(Throwable exc, Object attachment) {  
-                         System.out.println("server failed: " + exc);  
-                     }  
-                 });  
- 
- try {  
- // Wait for ever
-                     Thread.sleep(Integer.MAX_VALUE);  
-                 } catch (InterruptedException ex) {  
-                     System.out.println(ex);  
-                 }  
-             }  
-         } catch (IOException e) {  
-             System.out.println(e);  
-         }  
-     }  
- }  
客户端程序
**[java]**[view
 plain](http://blog.csdn.net/zhongweijian/article/details/8005444#)[copy](http://blog.csdn.net/zhongweijian/article/details/8005444#)
- import java.net.InetSocketAddress;  
- import java.nio.ByteBuffer;  
- import java.nio.channels.AsynchronousSocketChannel;  
- import java.nio.channels.CompletionHandler;  
- import java.util.concurrent.ExecutionException;  
- 
- publicclass EchoAioClient {  
- privatefinal AsynchronousSocketChannel client ;  
- 
- public EchoAioClient() throws Exception{  
-        client = AsynchronousSocketChannel.open();  
-     }  
- 
- publicvoid start()throws Exception{  
-         client.connect(new InetSocketAddress("127.0.0.1",8000),null,new CompletionHandler<Void,Void>() {  
- @Override
- publicvoid completed(Void result, Void attachment) {  
- try {  
-                     client.write(ByteBuffer.wrap("this is a test".getBytes())).get();  
-                     System.out.println("send data to server");  
-                 } catch (Exception ex) {  
-                     ex.printStackTrace();  
-                 }  
-             }  
- 
- @Override
- publicvoid failed(Throwable exc, Void attachment) {  
-                 exc.printStackTrace();  
-             }  
-         });  
- final ByteBuffer bb = ByteBuffer.allocate(1024);  
-         client.read(bb, null, new CompletionHandler<Integer,Object>(){  
- 
- @Override
- publicvoid completed(Integer result, Object attachment) {  
-                  System.out.println(result);  
-                  System.out.println(new String(bb.array()));  
-             }  
- 
- @Override
- publicvoid failed(Throwable exc, Object attachment) {  
-                     exc.printStackTrace();  
-                 }  
-             }  
-         );  
- 
- 
- 
- try {  
- // Wait for ever
-             Thread.sleep(Integer.MAX_VALUE);  
-         } catch (InterruptedException ex) {  
-             System.out.println(ex);  
-         }  
- 
-     }  
- 
- publicstaticvoid main(String args[])throws Exception{  
- new EchoAioClient().start();  
-     }  
- }  
执行server跟client后输出如下：
server输出：
**[html]**[view
 plain](http://blog.csdn.net/zhongweijian/article/details/8005444#)[copy](http://blog.csdn.net/zhongweijian/article/details/8005444#)
- Echo listen on 8000  
- waiting ....  
- done...  
client输出：
**[html]**[view
 plain](http://blog.csdn.net/zhongweijian/article/details/8005444#)[copy](http://blog.csdn.net/zhongweijian/article/details/8005444#)
- send data to server  
- 14  
- this is a test  
![](http://static.blog.csdn.net/images/save_snippets.png)
