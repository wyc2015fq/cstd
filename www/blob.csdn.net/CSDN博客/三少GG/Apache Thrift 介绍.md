# Apache Thrift 介绍 - 三少GG - CSDN博客
2013年10月12日 14:42:21[三少GG](https://me.csdn.net/scut1135)阅读数：1255
个人分类：[云计算/大数据](https://blog.csdn.net/scut1135/article/category/1449467)
[http://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/](http://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/)
# Apache Thrift - 可伸缩的跨语言服务开发框架
**简介：** Apache Thrift 是 Facebook 实现的一种高效的、支持多种编程语言的远程服务调用的框架。本文将从 Java 开发人员角度详细介绍 Apache Thrift 的架构、开发和部署，并且针对不同的传输协议和服务类型给出相应的 Java 实例，同时详细介绍 Thrift 异步客户端的实现，最后提出使用 Thrift 需要注意的事项。
**发布日期：** 2012 年 1 月 16 日 
**级别：** 中级 
**访问情况 ：** 29612 次浏览 
**评论：** ([查看](http://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/#icomments) | [添加评论](http://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/#) -
 登录)
![平均分 5 星 共 69 个评分](http://dw1.s81c.com/developerworks/i/stars120x20-5b.jpg) 平均分 (69个评分)
[为本文评分](http://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/#iratings)

前言：
目前流行的服务调用方式有很多种，例如基于 SOAP 消息格式的 Web Service，基于 JSON 消息格式的 RESTful 服务等。其中所用到的数据传输方式包括 XML，JSON 等，然而 XML 相对体积太大，传输效率低，JSON 体积较小，新颖，但还不够完善。本文将介绍由 Facebook 开发的远程服务调用框架 Apache Thrift，它采用接口描述语言定义并创建服务，支持可扩展的跨语言服务开发，所包含的代码生成引擎可以在多种语言中，如 C++, Java, Python, PHP, Ruby,
 Erlang, Perl, Haskell, C#, Cocoa, Smalltalk 等创建高效的、无缝的服务，其传输数据采用二进制格式，相对 XML 和 JSON 体积更小，对于高并发、大数据量和多语言的环境更有优势。本文将详细介绍 Thrift 的使用，并且提供丰富的实例代码加以解释说明，帮助使用者快速构建服务。
[回页首](http://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/#ibm-pcon)
一个简单的 Thrift 实例
本文首先介绍一个简单的 Thrift 实现实例，使读者能够快速直观地了解什么是 Thrift 以及如何使用 Thrift 构建服务。
创建一个简单的服务 Hello。首先根据 Thrift 的语法规范编写脚本文件 Hello.thrift，代码如下：
清单 1. Hello.thrift
				 
 namespace java service.demo 
 service Hello{ 
  string helloString(1:string para) 
  i32 helloInt(1:i32 para) 
  bool helloBoolean(1:bool para) 
  void helloVoid() 
  string helloNull() 
 } 
其中定义了服务 Hello 的五个方法，每个方法包含一个方法名，参数列表和返回类型。每个参数包括参数序号，参数类型以及参数名。 Thrift 是对 IDL(Interface Definition Language) 描述性语言的一种具体实现。因此，以上的服务描述文件使用 IDL 语法编写。使用 Thrift 工具编译 Hello.thrift，就会生成相应的 Hello.java 文件。该文件包含了在 Hello.thrift 文件中描述的服务 Hello 的接口定义，即 Hello.Iface 接口，以及服务调用的底层通信细节，包括客户端的调用逻辑
 Hello.Client 以及服务器端的处理逻辑 Hello.Processor，用于构建客户端和服务器端的功能。
创建 HelloServiceImpl.java 文件并实现 Hello.java 文件中的 Hello.Iface 接口，代码如下：
清单 2. HelloServiceImpl.java
				 
 package service.demo; 
 import org.apache.thrift.TException; 
 public class HelloServiceImpl implements Hello.Iface { 
    @Override 
    public boolean helloBoolean(boolean para) throws TException { 
        return para; 
    } 
    @Override 
    public int helloInt(int para) throws TException { 
        try { 
            Thread.sleep(20000); 
        } catch (InterruptedException e) { 
            e.printStackTrace(); 
        } 
        return para; 
    } 
    @Override 
    public String helloNull() throws TException { 
        return null; 
    } 
    @Override 
    public String helloString(String para) throws TException { 
        return para; 
    } 
    @Override 
    public void helloVoid() throws TException { 
        System.out.println("Hello World"); 
    } 
 } 
创建服务器端实现代码，将 HelloServiceImpl 作为具体的处理器传递给 Thrift 服务器，代码如下：
清单 3. HelloServiceServer.java
				 
 package service.server; 
 import org.apache.thrift.TProcessor; 
 import org.apache.thrift.protocol.TBinaryProtocol; 
 import org.apache.thrift.protocol.TBinaryProtocol.Factory; 
 import org.apache.thrift.server.TServer; 
 import org.apache.thrift.server.TThreadPoolServer; 
 import org.apache.thrift.transport.TServerSocket; 
 import org.apache.thrift.transport.TTransportException; 
 import service.demo.Hello; 
 import service.demo.HelloServiceImpl; 
 public class HelloServiceServer { 
    /** 
     * 启动 Thrift 服务器
     * @param args 
     */ 
    public static void main(String[] args) { 
        try { 
            // 设置服务端口为 7911 
            TServerSocket serverTransport = new TServerSocket(7911); 
            // 设置协议工厂为 TBinaryProtocol.Factory 
            Factory proFactory = new TBinaryProtocol.Factory(); 
            // 关联处理器与 Hello 服务的实现
            TProcessor processor = new Hello.Processor(new HelloServiceImpl()); 
            TServer server = new TThreadPoolServer(processor, serverTransport, 
                    proFactory); 
            System.out.println("Start server on port 7911..."); 
            server.serve(); 
        } catch (TTransportException e) { 
            e.printStackTrace(); 
        } 
    } 
 } 
创建客户端实现代码，调用 Hello.client 访问服务端的逻辑实现，代码如下：
清单 4. HelloServiceClient.java
				 
 package service.client; 
 import org.apache.thrift.TException; 
 import org.apache.thrift.protocol.TBinaryProtocol; 
 import org.apache.thrift.protocol.TProtocol; 
 import org.apache.thrift.transport.TSocket; 
 import org.apache.thrift.transport.TTransport; 
 import org.apache.thrift.transport.TTransportException; 
 import service.demo.Hello; 
 public class HelloServiceClient { 
 /** 
     * 调用 Hello 服务
     * @param args 
     */ 
    public static void main(String[] args) { 
        try { 
            // 设置调用的服务地址为本地，端口为 7911 
            TTransport transport = new TSocket("localhost", 7911); 
            transport.open(); 
            // 设置传输协议为 TBinaryProtocol 
            TProtocol protocol = new TBinaryProtocol(transport); 
            Hello.Client client = new Hello.Client(protocol); 
            // 调用服务的 helloVoid 方法
            client.helloVoid(); 
            transport.close(); 
        } catch (TTransportException e) { 
            e.printStackTrace(); 
        } catch (TException e) { 
            e.printStackTrace(); 
        } 
    } 
 } 
代码编写完后运行服务器，再启动客户端调用服务 Hello 的方法 helloVoid，在服务器端的控制台窗口输出“Hello World”(helloVoid 方法实现在控制台打印字符串，没有返回值，所以客户端调用方法后没有返回值输出，读者可以自己尝试其他有返回值方法的调用，其结果可以打印在客户端的控制台窗口 )。
[回页首](http://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/#ibm-pcon)
Thrift 架构
Thrift 包含一个完整的堆栈结构用于构建客户端和服务器端。下图描绘了 Thrift 的整体架构。
图 1. 架构图
![图 1. 架构图](http://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/image003.jpg)
如图所示，图中黄色部分是用户实现的业务逻辑，褐色部分是根据 Thrift 定义的服务接口描述文件生成的客户端和服务器端代码框架，红色部分是根据 Thrift 文件生成代码实现数据的读写操作。红色部分以下是 Thrift 的传输体系、协议以及底层 I/O 通信，使用 Thrift 可以很方便的定义一个服务并且选择不同的传输协议和传输层而不用重新生成代码。
Thrift 服务器包含用于绑定协议和传输层的基础架构，它提供阻塞、非阻塞、单线程和多线程的模式运行在服务器上，可以配合服务器 / 容器一起运行，可以和现有的 J2EE 服务器 /Web 容器无缝的结合。
服务端和客户端具体的调用流程如下：
图 2. Server 端启动、服务时序图（**[查看大图](http://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/image004.png)）**
![图 2. Server 端启动、服务时序图](http://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/image005.gif)
该图所示是 HelloServiceServer 启动的过程以及服务被客户端调用时，服务器的响应过程。从图中我们可以看到，程序调用了 TThreadPoolServer 的 serve 方法后，server 进入阻塞监听状态，其阻塞在 TServerSocket 的 accept 方法上。当接收到来自客户端的消息后，服务器发起一个新线程处理这个消息请求，原线程再次进入阻塞状态。在新线程中，服务器通过 TBinaryProtocol 协议读取消息内容，调用 HelloServiceImpl 的 helloVoid
 方法，并将结果写入 helloVoid_result 中传回客户端。
图 3. Client 端调用服务时序图（**[查看大图](http://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/image006.png)）**
![图 3. Client 端调用服务时序图](http://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/image007.gif)
该图所示是 HelloServiceClient 调用服务的过程以及接收到服务器端的返回值后处理结果的过程。从图中我们可以看到，程序调用了 Hello.Client 的 helloVoid 方法，在 helloVoid 方法中，通过 send_helloVoid 方法发送对服务的调用请求，通过 recv_helloVoid 方法接收服务处理请求后返回的结果。
[回页首](http://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/#ibm-pcon)
数据类型
Thrift 脚本可定义的数据类型包括以下几种类型：
- 
基本类型：
- - 
bool：布尔值，true 或 false，对应 Java 的 boolean
- 
byte：8 位有符号整数，对应 Java 的 byte
- 
i16：16 位有符号整数，对应 Java 的 short
- 
i32：32 位有符号整数，对应 Java 的 int
- 
i64：64 位有符号整数，对应 Java 的 long
- 
double：64 位浮点数，对应 Java 的 double
- 
string：未知编码文本或二进制字符串，对应 Java 的 String
- 
结构体类型：
- 
struct：定义公共的对象，类似于 C 语言中的结构体定义，在 Java 中是一个 JavaBean
- 
容器类型：
- 
list：对应 Java 的 ArrayList
- 
set：对应 Java 的 HashSet
- 
map：对应 Java 的 HashMap
- 
异常类型：
- 
exception：对应 Java 的 Exception
- 
服务类型：
- 
service：对应服务的类
[回页首](http://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/#ibm-pcon)
协议
Thrift 可以让用户选择客户端与服务端之间传输通信协议的类别，在传输协议上总体划分为文本 (text) 和二进制 (binary) 传输协议，为节约带宽，提高传输效率，一般情况下使用二进制类型的传输协议为多数，有时还会使用基于文本类型的协议，这需要根据项目 / 产品中的实际需求。常用协议有以下几种：
- 
TBinaryProtocol —— 二进制编码格式进行数据传输
使用方法如清单 3 和清单 4 所示。
- 
TCompactProtocol —— 高效率的、密集的二进制编码格式进行数据传输
构建 TCompactProtocol 协议的服务器和客户端只需替换清单 3 和清单 4 中 TBinaryProtocol 协议部分即可，替换成如下代码：
清单 5. 使用 TCompactProtocol 协议构建的 HelloServiceServer.java
				 
 TCompactProtocol.Factory proFactory = new TCompactProtocol.Factory(); 
清单 6. 使用 TCompactProtocol 协议的 HelloServiceClient.java
				 
 TCompactProtocol protocol = new TCompactProtocol(transport); 
- 
TJSONProtocol —— 使用 JSON 的数据编码协议进行数据传输
构建 TJSONProtocol 协议的服务器和客户端只需替换清单 3 和清单 4 中 TBinaryProtocol 协议部分即可，替换成如下代码：
清单 7. 使用 TJSONProtocol 协议构建的 HelloServiceServer.java
				 
 TJSONProtocol.Factory proFactory = new TJSONProtocol.Factory(); 
清单 8. 使用 TJSONProtocol 协议的 HelloServiceClient.java
				 
 TJSONProtocol protocol = new TJSONProtocol(transport); 
- 
TSimpleJSONProtocol —— 只提供 JSON 只写的协议，适用于通过脚本语言解析
[回页首](http://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/#ibm-pcon)
传输层
常用的传输层有以下几种：
- 
TSocket —— 使用阻塞式 I/O 进行传输，是最常见的模式
使用方法如清单 4 所示。
- 
TFramedTransport —— 使用非阻塞方式，按块的大小进行传输，类似于 Java 中的 NIO
若使用 TFramedTransport 传输层，其服务器必须修改为非阻塞的服务类型，客户端只需替换清单 4 中 TTransport 部分，代码如下，清单 9 中 TNonblockingServerTransport 类是构建非阻塞 socket 的抽象类，TNonblockingServerSocket 类继承 TNonblockingServerTransport
清单 9. 使用 TFramedTransport 传输层构建的 HelloServiceServer.java
				 
 TNonblockingServerTransport serverTransport; 
 serverTransport = new TNonblockingServerSocket(10005); 
 Hello.Processor processor = new Hello.Processor(new HelloServiceImpl()); 
 TServer server = new TNonblockingServer(processor, serverTransport); 
 System.out.println("Start server on port 10005 ..."); 
 server.serve(); 
清单 10. 使用 TFramedTransport 传输层的 HelloServiceClient.java
				 
 TTransport transport = new TFramedTransport(new TSocket("localhost", 10005)); 
- 
TNonblockingTransport —— 使用非阻塞方式，用于构建异步客户端
使用方法请参考 Thrift 异步客户端构建
[回页首](http://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/#ibm-pcon)
服务端类型
常见的服务端类型有以下几种：
- 
TSimpleServer —— 单线程服务器端使用标准的阻塞式 I/O
代码如下：
清单 11. 使用 TSimpleServer 服务端构建的 HelloServiceServer.java
				 
 TServerSocket serverTransport = new TServerSocket(7911); 
 TProcessor processor = new Hello.Processor(new HelloServiceImpl()); 
 TServer server = new TSimpleServer(processor, serverTransport); 
 System.out.println("Start server on port 7911..."); 
 server.serve(); 
客户端的构建方式可参考清单 4。
- 
TThreadPoolServer —— 多线程服务器端使用标准的阻塞式 I/O
使用方法如清单 3 所示。
- 
TNonblockingServer —— 多线程服务器端使用非阻塞式 I/O
使用方法请参考 Thrift 异步客户端构建
[回页首](http://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/#ibm-pcon)
Thrift 异步客户端构建
Thrift 提供非阻塞的调用方式，可构建异步客户端。在这种方式中，Thrift 提供了新的类 TAsyncClientManager 用于管理客户端的请求，在一个线程上追踪请求和响应，同时通过接口 AsyncClient 传递标准的参数和 callback 对象，服务调用完成后，callback 提供了处理调用结果和异常的方法。
首先我们看 callback 的实现：
清单 12.CallBack 的实现：MethodCallback.java
				 
 package service.callback; 
 import org.apache.thrift.async.AsyncMethodCallback; 
 public class MethodCallback implements AsyncMethodCallback { 
    Object response = null; 
    public Object getResult() { 
        // 返回结果值
        return this.response; 
    } 
    // 处理服务返回的结果值
    @Override 
    public void onComplete(Object response) { 
        this.response = response; 
    } 
    // 处理调用服务过程中出现的异常
    @Override 
    public void onError(Throwable throwable) { 
    } 
 } 
如代码所示，onComplete 方法接收服务处理后的结果，此处我们将结果 response 直接赋值给 callback 的私有属性 response。onError 方法接收服务处理过程中抛出的异常，此处未对异常进行处理。
创建非阻塞服务器端实现代码，将 HelloServiceImpl 作为具体的处理器传递给异步 Thrift 服务器，代码如下：
清单 13.HelloServiceAsyncServer.java
				 
 package service.server; 
 import org.apache.thrift.server.TNonblockingServer; 
 import org.apache.thrift.server.TServer; 
 import org.apache.thrift.transport.TNonblockingServerSocket; 
 import org.apache.thrift.transport.TNonblockingServerTransport; 
 import org.apache.thrift.transport.TTransportException; 
 import service.demo.Hello; 
 import service.demo.HelloServiceImpl; 
 public class HelloServiceAsyncServer { 
    /** 
     * 启动 Thrift 异步服务器
     * @param args 
     */ 
    public static void main(String[] args) { 
        TNonblockingServerTransport serverTransport; 
        try { 
            serverTransport = new TNonblockingServerSocket(10005); 
            Hello.Processor processor = new Hello.Processor( 
                    new HelloServiceImpl()); 
            TServer server = new TNonblockingServer(processor, serverTransport); 
            System.out.println("Start server on port 10005 ..."); 
            server.serve(); 
        } catch (TTransportException e) { 
            e.printStackTrace(); 
        } 
    } 
 } 
HelloServiceAsyncServer 通过 java.nio.channels.ServerSocketChannel 创建非阻塞的服务器端等待客户端的连接。
创建异步客户端实现代码，调用 Hello.AsyncClient 访问服务端的逻辑实现，将 MethodCallback 对象作为参数传入调用方法中，代码如下：
清单 14.HelloServiceAsyncClient.java
				 
 package service.client; 
 import java.io.IOException; 
 import org.apache.thrift.async.AsyncMethodCallback; 
 import org.apache.thrift.async.TAsyncClientManager; 
 import org.apache.thrift.protocol.TBinaryProtocol; 
 import org.apache.thrift.protocol.TProtocolFactory; 
 import org.apache.thrift.transport.TNonblockingSocket; 
 import org.apache.thrift.transport.TNonblockingTransport; 
 import service.callback.MethodCallback; 
 import service.demo.Hello; 
 public class HelloServiceAsyncClient { 
    /** 
     * 调用 Hello 服务
     * @param args 
     */ 
    public static void main(String[] args) throws Exception { 
        try { 
            TAsyncClientManager clientManager = new TAsyncClientManager(); 
            TNonblockingTransport transport = new TNonblockingSocket( 
                    "localhost", 10005); 
            TProtocolFactory protocol = new TBinaryProtocol.Factory(); 
            Hello.AsyncClient asyncClient = new Hello.AsyncClient(protocol, 
                    clientManager, transport); 
            System.out.println("Client calls ....."); 
            MethodCallback callBack = new MethodCallback(); 
            asyncClient.helloString("Hello World", callBack); 
            Object res = callBack.getResult(); 
            while (res == null) { 
                res = callBack.getResult(); 
            } 
            System.out.println(((Hello.AsyncClient.helloString_call) res) 
                    .getResult()); 
        } catch (IOException e) { 
            e.printStackTrace(); 
        } 
  } 
 } 
HelloServiceAsyncClient 通过 java.nio.channels.Socketchannel 创建异步客户端与服务器建立连接。在本文中异步客户端通过以下的循环代码实现了同步效果，读者可去除这部分代码后再运行对比。
清单 15. 异步客户端实现同步效果代码段
Object res = callBack.getResult();
// 等待服务调用后的返回结果
while (res == null) {
   res = callBack.getResult();
}
通过与清单 9 和清单 10 的代码比较，我们可以构建一个 TNonblockingServer 服务类型的服务端，在客户端构建一个 TFramedTransport 传输层的同步客户端和一个 TNonblockingTransport 传输层的异步客户端，那么一个服务就可以通过一个 socket 端口提供两种不同的调用方式。有兴趣的读者可以尝试一下。
[回页首](http://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/#ibm-pcon)
常见问题
NULL 问题
我们在对服务的某个方法调用时，有时会出现该方法返回 null 值的情况，在 Thrift 中，直接调用一个返回 null 值的方法会抛出 TApplicationException 异常。在清单 2 中，HelloServiceImpl 里实现了 helloNull 方法，返回 null 值，我们在 HelloServiceClient.java 中加入调用该方法的代码，出现如下图所示的异常：
图 4. TApplicationException 异常
![图 4. TApplicationException 异常](http://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/image009.jpg)
为了处理返回 null 值情况，我们要捕获该异常，并进行相应的处理，具体客户端代码实现如下：
清单 16. 处理服务返回值为 null 的代码
				 
 package service.client; 
 import org.apache.thrift.TApplicationException; 
 import org.apache.thrift.TException; 
 import org.apache.thrift.protocol.TBinaryProtocol; 
 import org.apache.thrift.protocol.TProtocol; 
 import org.apache.thrift.transport.TSocket; 
 import org.apache.thrift.transport.TTransport; 
 import org.apache.thrift.transport.TTransportException; 
 import service.demo.Hello; 
 public class HelloServiceClient { 
    /** 
     * 调用 Hello 服务，并处理 null 值问题
     * @param args 
     */ 
    public static void main(String[] args) { 
        try { 
            TTransport transport = new TSocket("localhost", 7911); 
            transport.open(); 
            TProtocol protocol = new TBinaryProtocol(transport); 
            Hello.Client client = new Hello.Client(protocol); 
            System.out.println(client.helloNull()); 
            transport.close(); 
        } catch (TTransportException e) { 
            e.printStackTrace(); 
        } catch (TException e) { 
            if (e instanceof TApplicationException 
                    && ((TApplicationException) e).getType() ==   
                                 TApplicationException.MISSING_RESULT) { 
                System.out.println("The result of helloNull function is NULL"); 
            } 
        } 
    } 
 } 
调用 helloNull 方法后，会抛出 TApplicationException 异常，并且异常种类为 MISSING_RESULT，本段代码显示，捕获该异常后，直接在控制台打印“The result of helloNull function is NULL”信息。
[回页首](http://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/#ibm-pcon)
安装部署
Apache Thrift 的官方网站为：[http://thrift.apache.org/tutorial/](http://thrift.apache.org/tutorial/%20)，具体安装步骤如下：
- 
下载 thrift 源文件（[http://svn.apache.org/repos/asf/thrift/tags/thrift-0.6.1/](http://svn.apache.org/repos/asf/thrift/tags/thrift-0.6.1/)）
- 
将 thrift 源文件导入 eclipse，进入 /lib/java 目录，使用 ant 编译 build.xml 获得 libthrift-0.6.1-snapshot.jar
- 
将 libthrift-0.6.1-snapshot.jar、slf4j-api-1.5.8.jar、slf4j-log4j12-1.5.8.jar 和 log4j-1.2.14.jar 导入 eclipse 开发环境
- 
下载 thrift 编译工具，该工具可将 thrift 脚本文件编译成 java 文件，下载地址：[http://apache.etoak.com//thrift/0.6.0/thrift-0.6.1.exe](http://apache.etoak.com/thrift/0.6.0/thrift-0.6.1.exe)
- 
创建 Hello.thrift 脚本文件，具体代码如上一章节所述，进入 thrift-0.6.1.exe 所在目录，执行命令"thrift-0.6.1.exe -gen java x:\Hello.thrift"，在当前运行盘符下，可看见 gen-java 目录，进入目录可看到生成的 Java 代码。更多 thrift 的命令内容，请参考 thrift 自带的 help 命令
- 
编写服务端和客户端代码，完成 thrift 的安装和部署
基于 Apache Thrift 框架生成的服务包括客户端和服务器端，具体的部署模式如下所示：
图 5. 部署图
![图 5. 部署图](http://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/image011.jpg)
从图中我们可以看到，客户端和服务器端部署时，需要用到公共的 jar 包和 java 文件，如图“Common file”区域，其中 Hello.java 由 Hello.thrift 编译而来。在服务器端，服务必须实现 Hello.Iface 接口，同时要包括服务器的启动代码 HelloServiceServer.java。在客户端，包括客户端调用服务的代码 HelloServiceClient.java。客户端和服务器通过 Hello.java 提供的 API 实现远程服务调用。
[回页首](http://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/#ibm-pcon)
总结
本文介绍了 Apache Thrift 的安装部署和架构，并通过大量实例介绍了在不同情况下如何使用 Apache Thrift 来构建服务，同时着重介绍了 Thrift 异步客户端的构建，希望能给读者带来一些帮助。
参考资料
**学习**
- [Apache Thrift 官网](http://thrift.apache.org/)：可下载 Thrift 工具和源码。
- [Thrift Features and Non-features](http://wiki.apache.org/thrift/ThriftFeatures)：Thrift 的功能特点和不足之处。
- [Apache Thrift 介绍](http://jnb.ociweb.com/jnb/jnbJun2009.html)：介绍 Thrift 架构、协议、传输层和服务端类型，并与其他构建服务的方法 ( 如：REST) 进行比较分析。
- [Thrift 的安装部署](http://www.javabloger.com/article/thrift-java-code-example.html)：Thrift 的安装部署说明
- [Thrift: Scalable Cross-Language Services Implementation](http://thrift.apache.org/static/thrift-20070401.pdf)：Thrift 官方文档，详细介绍 Thrift 的设计
- [Thrift API](http://people.apache.org/~jfarrell/thrift/0.6.1/javadoc/)：关于 Apache Thrift 0.6.1 构建服务端和客户端的 API 手册
- [Thrift 实例](http://wenku.baidu.com/view/f77bc1a6f524ccbff1218461.html)：Thrift 的简单应用实例
- [Fully async Thrift client in Java](http://blog.rapleaf.com/dev/2010/06/23/fully-async-thrift-client-in-java/)：关于 Thrift 异步客户端的介绍
- [developerWorks Java 技术专区](http://www.ibm.com/developerworks/cn/java/)：这里有数百篇关于 Java 编程各个方面的文章。 
--------------------------------------------------------------
Thrift  是什么？
  Thrift源于大名鼎鼎的facebook之手，在2007年facebook提交Apache基金会将Thrift作为一个开源项目，对于当时的facebook来说创造thrift是为了解决facebook系统中各系统间大数据量的传 输通信以及系统之间语言环境不同需要跨平台的特性。所以thrift可以支持多种程序语言，例如:  C++, C#, Cocoa, Erlang, Haskell, Java, Ocami, Perl, PHP, Python, Ruby, Smalltalk. 在多种不同的语言之间通信thrift可以作为二进制的高性能的通讯中间件，支持数据(对象)序列化和多种类型的RPC服务。Thrift适用于程序对程
 序静态的数据交换，需要先确定好他的数据结构，他是完全静态化的，当数据结构发生变化时，必须重新编辑IDL文件，代码生成，再编译载入的流程，跟其他IDL工具相比较可以视为是Thrift的弱项，Thrift适用于搭建大型数据交换及存储的通用工具，对于大型系统中的内部数据传输相对于JSON和xml无论在性能、传输大小上有明显的优势。
   Thrift是IDL(interface definition language)描述性语言的一个具体实现，关于IDL的话题我们可以追溯到CORBA盛行1999-2001年(Common Object Request Broker Architecture/公用对象请求代理体系结构)，在 IDL 中我们似乎不会忘记到这几个关键字：module、interface、string、long 和 int，我还记得IDL利用module来创建名称空间，并且准确地映射为 Java 的 package，这些特性几乎和现在thrift的特性完全相同，所以thrift的设计思想和理念绝不是什么从火星来的new
 idea，看看在那个CORBA盛行的年代人们提出的概念，如图所示CORBA 请求的各个部分，回头我们再与thrift进行对比一下：
![Common Object Request Broker Architecture](http://zcitrq.bay.livefilestore.com/y1pJronriAjq9AasxlfkRFGO9E5Ioljuht5vGIT_Kjq5LANU7uTtKElt0UWmO6igfln4YBqrYCZPqG4hee1NQZ7mQoVSQz1TGCR/jt-2001-8-20-simpex-1.gif?psid=1)
Thrift 基础架构
   Thrift是一个服务端和客户端的架构体系，从我个人的感官上来看Thrift是一个类似XML-RPC+Java-to- IDL+Serialization Tools=Thrift 的东东，Thrift 具有自己内部定义的传输协议规范(TProtocol)和传输数据标准(TTransports)，通过IDL脚本对传输数据的数据结构(struct) 和传输数据的业务逻辑(service)根据不同的运行环境快速的构建相应的代码，并且通过自己内部的序列化机制对传输的数据进行简化和压缩提高高并发、 大型系统中数据交互的成本，下图描绘了Thrift的整体架构，分为6个部分：1.你的业务逻辑实现(You
 Code) 2.客户端和服务端对应的Service 3.执行读写操作的计算结果4.TProtocol 5.TTransports  6.底层I/O通信
![thrift](http://public.bay.livefilestore.com/y1pCfZp75Z-beYM_ZfeOVUmIjN3FskFdNTU9qpECWiCnk95JOaw6w5o4ZIiurVmK8w54SJ-pVkfQbYtJPaNb1F3vw/Apache-Thrift-Architecture.png?psid=1)
图 中前面3个部分是1.你通过Thrift脚本文件生成的代码，2.图中的褐色框部分是你根据生成代码构建的客户端和处理器的代码，3.图中红色的部分是2 端产生的计算结果。从TProtocol下面3个部分是Thrift的传输体系和传输协议以及底层I/O通信，Thrift并且提供 堵塞、非阻塞，单线程、多线程的模式运行在服务器上，还可以配合服务器/容器一起运行，可以和现有JEE服务器/Web容器无缝的结合。
数据类型
     * Base Types：基本类型
     * Struct：结构体类型
     * Container：容器类型，即List、Set、Map
     * Exception：异常类型
     * Service： 定义对象的接口，和一系列方法
协议
  Thrift可以让你选择客户端与服务端之间传输通信协议的类别，在传输协议上总体上划分为文本(text)和二进制(binary)传输协议, 为节约带宽，提供传输效率，一般情况下使用二进制类型的传输协议为多数，但有时会还是会使用基于文本类型的协议，这需要根据项目/产品中的实际需求：
    * TBinaryProtocol – 二进制编码格式进行数据传输。
    * TCompactProtocol – 这种协议非常有效的，使用Variable-Length Quantity (VLQ) 编码对数据进行压缩。
    * TJSONProtocol – 使用JSON的数据编码协议进行数据传输。
    * TSimpleJSONProtocol – 这种节约只提供JSON只写的协议，适用于通过脚本语言解析
    * TDebugProtocol – 在开发的过程中帮助开发人员调试用的，以文本的形式展现方便阅读。
传输层
    * TSocket- 使用堵塞式I/O进行传输，也是最常见的模式。
    * TFramedTransport- 使用非阻塞方式，按块的大小，进行传输，类似于Java中的NIO。
    * TFileTransport- 顾名思义按照文件的方式进程传输，虽然这种方式不提供Java的实现，但是实现起来非常简单。
    * TMemoryTransport- 使用内存I/O，就好比Java中的ByteArrayOutputStream实现。
    * TZlibTransport- 使用执行zlib压缩，不提供Java的实现。
服务端类型
    * TSimpleServer -  单线程服务器端使用标准的堵塞式I/O。
    * TThreadPoolServer -  多线程服务器端使用标准的堵塞式I/O。
    * TNonblockingServer – 多线程服务器端使用非堵塞式I/O，并且实现了Java中的NIO通道。
谁在用thrift ？
![Quara](http://public.bay.livefilestore.com/y1pEaI-1Jvv7yWALGBZpwJE4baM80WXFYHaHBeIrPS7DiItCE7dhTWN8bblTPiOMyeo597BqCjgtyOyR-r5tZHqOQ/quora.gif?psid=1)
     Thrift用于Quara系统后端数据的通信，服务端是用C++来实现的，客户端则是python。
Quara背景:Quara是在线问答服务公司，类似新浪微博和百度知道的合体，消息灵通人士透露，去年Quara获得了1400万美元投资，目前他们只有9名员工。
原文：http://www.philwhln.com/quoras-technology-examined#thrift
![Evernote](http://public.bay.livefilestore.com/y1p0BU4Q_vPabzwkr0bUupVTpp2ly6B6E1L89I5s4azW4K96vETVgPMlPyVMVmrxmkAah7qY-CDdtIF5iC_9kO3sw/evernote-logo.gif?psid=1)
  Thrift用于在多种Evernote API平台开发的客户端与Evernote服务器之间的通信与数据传输，Evernote API定义了自己的Evernote Data Access and Management (EDAM) 协议规范，让客户端使用更小的网络带宽上传、下载文件和在线即时搜索服务。
Evernote 背景:EverNote是一款非常著名的免费软件，它最大的特点就是支持多平台，而且数据能通过网络互相同步。譬如说，你可以随时在手机上的Evernote新增笔记，回家后在电脑上也能看到它了！
原文：http://www.evernote.com/about/developer/api/evernote-api.htm
HBase 中的Thrift
    Thrift用于HBase中是为了提供跨平台的服务接口，在HBase 中可以使用[hbase-root]/bin/hbase thrift start 命令启动涵盖Thrift的HBase服务端，客户端通过thrift的命令生成不同版本的客户端代码，根据定义的数据格式，对远程HBase服务端进行 操作，是除了REST远程方法调用的另一种途径。
参见：http://wiki.apache.org/hadoop/Hbase/ThriftApi
更多资料请阅读：http://wiki.apache.org/thrift/PoweredBy
Thrift与其他传输方式的比较
    xml与JSON相比体积太大，但是xml传统，也不算复杂。
    json 体积较小，新颖，但不够完善。
    thrift 体积超小，使用起来比较麻烦，不如前两者轻便，但是对于1.高并发、2.数据传输量大、3.多语言环境， 满足其中2点使用 thrift还是值得的。
  假定需要传输相同的内容，但使用不同的方式从 1、传输内容所产生的大小 2、传输过程中服务端和客户端所产生的开销，这2个方便进行比较。使用Thrift和其他方式的所产生的内容大小比较结果如下：
![thrift-size](http://public.bay.livefilestore.com/y1pCfZp75Z-bebqxcDBqfvqi1Cf6svZCypC7wkXK1OzwpqGYdgztQx3t9XEU9lZPm41HOIRc-_1fm03YiNgHrWApg/thrift-size.png?psid=1)
    在上图中我们能明显看出，最臃肿的是RMI，其次是xml，使用Thrift的TCompactProtocol协议和Google 的 Protocol Buffers 相差的不算太多，相比而言还是Google 的 Protocol Buffers效果最佳。
使用Thrift 中的协议和其他方式的所产生的运行开销比较结果如下：
![thrift-load](http://public.bay.livefilestore.com/y1p7G5AiRmyV-zw46l5XQRY8t2pMv2kOwlHrgTPyhb28_3uP3a-CkBAVsk1Ohwsugh09MY9A37u5w_1B6AI-Iqp2Q/thrift-load.png?psid=1)
   在上图中我们能明显看出，最占资源是REST2中协议，使用Thrift的TCompactProtocol协议和Google 的 Protocol Buffers 相差的不算太多，相比而言Thrift的TCompactProtocol协议效果最佳。
接下来请继续阅读，[迈向Thrift的第一步，Thrift示例](http://www.javabloger.com/article/thrift-java-code-example.html)。
