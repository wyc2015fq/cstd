# Thrift入门教程 - 零度的博客专栏 - CSDN博客
2017年09月25日 10:45:11[零度anngle](https://me.csdn.net/zmx729618)阅读数：238
#### 1、概述
Thrift是一个软件框架，用来进行可扩展且跨语言的服务的开发。它结合了功能强大的软件堆栈和代码生成引擎，以构建在 C++, Java, Python, PHP, Ruby, Erlang, Perl, Haskell, C#, Cocoa, JavaScript, Node.js, Smalltalk, and OCaml 等等编程语言间无缝结合的、高效的服务。
Thrift最初由facebook开发，07年四月开放源码，08年5月进入apache孵化器。thrift允许你定义一个简单的定义文件中的数据类型和服务接口。以作为输入文件，编译器生成代码用来方便地生成RPC客户端和服务器通信的无缝跨编程语言。其传输数据采用二进制格式，相对于XML和JSON等序列化方式体积更小，对于高并发、大数据量和多语言的环境更有优势。
Thrift它含有三个主要的组件：protocol，transport和server，其中，protocol定义了消息是怎样序列化的，transport定义了消息是怎样在客户端和服务器端之间通信的，server用于从transport接收序列化的消息，根据protocol反序列化之，调用用户定义的消息处理器，并序列化消息处理器的响应，然后再将它们写回transport。
**本文代码可参考：[https://github.com/hawkingfoo/thrift-demo](https://github.com/hawkingfoo/thrift-demo)**
#### 2、下载配置
使用的版本为0.9.3。如果是Maven构建项目的，直接在pom.xml 中添加如下内容：
```
<!-- https://mvnrepository.com/artifact/org.apache.thrift/libthrift -->
<dependency>
    <groupId>org.apache.thrift</groupId>
    <artifactId>libthrift</artifactId>
    <version>0.9.3</version>
</dependency>
```
#### 3、基本概念
##### 3.1 数据类型
- 
基本类型：
- bool：布尔值，true 或 false，对应 Java 的 boolean
- byte：8 位有符号整数，对应 Java 的 byte
- i16：16 位有符号整数，对应 Java 的 short
- i32：32 位有符号整数，对应 Java 的 int
- i64：64 位有符号整数，对应 Java 的 long
- double：64 位浮点数，对应 Java 的 double
- string：未知编码文本或二进制字符串，对应 Java 的 String
- 
结构体类型：
- struct：定义公共的对象，类似于 C 语言中的结构体定义，在 Java 中是一个 JavaBean
- 
集合类型：
- list：对应 Java 的 ArrayList
- set：对应 Java 的 HashSet
- map：对应 Java 的 HashMap
- 
异常类型：
- exception：对应 Java 的 Exception
- 
服务类型：
- service：对应服务的类
##### 3.2 数据传输层Transport
- **TSocket** —— 使用阻塞式 I/O 进行传输，是最常见的模式
- **TFramedTransport** —— 使用非阻塞方式，按块的大小进行传输，类似于 Java 中的 NIO，若使用 TFramedTransport 传输层，其服务器必须修改为非阻塞的服务类型
- **TNonblockingTransport** —— 使用非阻塞方式，用于构建异步客户端
##### 3.3 数据传输协议Protocol
Thrift 可以让用户选择客户端与服务端之间传输通信协议的类别，在传输协议上总体划分为文本 (text) 和二进制 (binary) 传输协议，为节约带宽，提高传输效率，一般情况下使用二进制类型的传输协议为多数，有时还会使用基于文本类型的协议，这需要根据项目 / 产品中的实际需求。常用协议有以下几种：
- **TBinaryProtocol** —— 二进制编码格式进行数据传输
`TProtocol protocol = new TBinaryProtocol(transport);`
- **TCompactProtocol** —— 高效率的、密集的二进制编码格式进行数据传输
`TCompactProtocol protocol = new TCompactProtocol(transport);`
- **TJSONProtocol** —— 使用 JSON 的数据编码协议进行数据传输
`TJSONProtocol protocol = new TJSONProtocol(transport);`
- **TSimpleJSONProtocol** —— 只提供 JSON 只写的协议，适用于通过脚本语言解析**
`TProtocol protocol = new TSimpleJSONProtocol(transport);`
##### 3.4 服务器类型Server
- **TSimpleServer** —— 单线程服务器端使用标准的阻塞式 I/O，一般用于测试。
- **TThreadPoolServer** —— 多线程服务器端使用标准的阻塞式 I/O，预先创建一组线程处理请求。
- **TNonblockingServer** —— 多线程服务器端使用非阻塞式 I/O，服务端和客户端需要指定 TFramedTransport 数据传输的方式。
- **THsHaServer** —— 半同步半异步的服务端模型，需要指定为： TFramedTransport 数据传输的方式。它使用一个单独的线程来处理网络I/O，一个独立的worker线程池来处理消息。这样，只要有空闲的worker线程，消息就会被立即处理，因此多条消息能被并行处理。
- **TThreadedSelectorServer** —— TThreadedSelectorServer允许你用多个线程来处理网络I/O。它维护了两个线程池，一个用来处理网络I/O，另一个用来进行请求的处理。当网络I/O是瓶颈的时候，TThreadedSelectorServer比THsHaServer的表现要好。
#### 4、实例
##### 4.1 准备
新建Maven项目，并且添加thrift依赖
```
<dependencies>
        <dependency>
            <groupId>org.apache.thrift</groupId>
            <artifactId>libthrift</artifactId>
            <version>0.9.3</version>
        </dependency>
        <dependency>
            <groupId>org.apache.logging.log4j</groupId>
            <artifactId>log4j-api</artifactId>
            <version>2.7</version>
        </dependency>
        <dependency>
            <groupId>org.apache.logging.log4j</groupId>
            <artifactId>log4j-core</artifactId>
            <version>2.7</version>
        </dependency>
    </dependencies>
    <build>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <version>3.3</version>
                <configuration>
                    <source>1.7</source>
                    <target>1.7</target>
                    <encoding>UTF-8</encoding>
                </configuration>
            </plugin>
        </plugins>
    </build>
```
##### 4.2 编写IDL接口并生成接口文件
```
namespace java thrifttest.service
// 计算类型 - 仅限整数四则运算
enum ComputeType {
    ADD = 0;
    SUB = 1;
    MUL = 2;
    DIV = 3;
}
// 服务请求
struct ComputeRequest {
    1:required i64 x;
    2:required i64 y;
    3:required ComputeType computeType;
}
// 服务响应
struct ComputeResponse {
    1:required i32 errorNo;
    2:optional string errorMsg;
    3:required i64 computeRet;
}
service ComputeServer {
    ComputeResponse getComputeResult(1:ComputeRequest request);
}
```
执行编译命令：
`thrift-0.9.3.exe --gen java computeServer.thrift`
##### 4.3 服务端接口实现以及服务启动
```
public class ThriftTestImpl implements ComputeServer.Iface {
    private static final Logger logger = LogManager.getLogger(ThriftTestImpl.class);
    public ComputeResponse getComputeResult(ComputeRequest request) {
        ComputeType computeType = request.getComputeType();
        long x = request.getX();
        long y = request.getY();
        logger.info("get compute result begin. [x:{}] [y:{}] [type:{}]", x, y, computeType.toString());
        long begin = System.currentTimeMillis();
        ComputeResponse response = new ComputeResponse();
        response.setErrorNo(0);
        try {
            long ret;
            if (computeType == ComputeType.ADD) {
                ret = add(x, y);
                response.setComputeRet(ret);
            } else if (computeType == ComputeType.SUB) {
                ret = sub(x, y);
                response.setComputeRet(ret);
            } else if (computeType == ComputeType.MUL) {
                ret = mul(x, y);
                response.setComputeRet(ret);
            } else {
                ret = div(x, y);
                response.setComputeRet(ret);
            }
        } catch (Exception e) {
            response.setErrorNo(1001);
            response.setErrorMsg(e.getMessage());
            logger.error("exception:", e);
        }
        long end = System.currentTimeMillis();
        logger.info("get compute result end. [errno:{}] cost:[{}ms]",
                response.getErrorNo(), (end - begin));
        return response;
    }
    public long add(long x, long y) {
        return x + y;
    }
    public long sub(long x, long y) {
        return x - y;
    }
    public long mul(long x, long y) {
        return x * y;
    }
    public long div(long x, long y) {
        return x / y;
    }
}
```
```
public class ServerMain {
    private static final Logger logger = LogManager.getLogger(ServerMain.class);
    public static void main(String[] args) {
        try {
            ThriftTestImpl workImpl = new ThriftTestImpl();
            TProcessor tProcessor = new ComputeServer.Processor<ComputeServer.Iface>(workImpl);
            final TNonblockingServerTransport transport = new TNonblockingServerSocket(9000);
            TThreadedSelectorServer.Args ttpsArgs = new TThreadedSelectorServer.Args(transport);
            ttpsArgs.transportFactory(new TFramedTransport.Factory());
            ttpsArgs.protocolFactory(new TBinaryProtocol.Factory());
            ttpsArgs.processor(tProcessor);
            ttpsArgs.selectorThreads(16);
            ttpsArgs.workerThreads(32);
            logger.info("compute service server on port :" + 9000);
            TServer server = new TThreadedSelectorServer(ttpsArgs);
            server.serve();
        } catch (Exception e) {
            logger.error(e);
        }
    }
}
```
##### 4.4 客户端访问
```
public class ComputeClient {
    private ComputeRequest request;
    public ComputeClient() {
        request = new ComputeRequest();
        request.setX(1);
        request.setY(2);
        request.setComputeType(ComputeType.ADD);
    }
    public static void main(String[] args) {
        TTransport transport = null;
        try {
            System.out.println("***********");
            long begin = System.currentTimeMillis();
            // localhost
            transport = new TFramedTransport(new TSocket("127.0.0.1", 9000));
            transport.open();
            TProtocol protocol = new TBinaryProtocol(transport);
            ComputeServer.Client client = new ComputeServer.Client(protocol);
            //调用client的getComputeResult方法
            ComputeResponse response = client.getComputeResult(new ComputeClient().request);
            System.out.println("cost:[" + (System.currentTimeMillis() - begin) + "ms]");
            System.out.println("***********");
            if (response != null) {
                System.out.println(response.toString());
            }
            transport.close();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (transport != null)
                transport.close();
        }
    }
}
```
##### 4.5 整体代码结构
![](http://upload-images.jianshu.io/upload_images/4324380-6ecb24c92c47e464.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
##### 4.5 测试
服务端：
![](http://upload-images.jianshu.io/upload_images/4324380-26469ae1a4077087.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
客户端：
![](http://upload-images.jianshu.io/upload_images/4324380-709344a8c3e4119d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

