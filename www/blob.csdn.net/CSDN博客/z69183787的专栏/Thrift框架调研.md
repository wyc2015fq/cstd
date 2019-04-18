# Thrift框架调研 - z69183787的专栏 - CSDN博客
2016年05月09日 20:50:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1038
u **简介**
Thrift是一种开源的跨语言的RPC服务框架。Thrift最初由facebook公司开发的，在2007年facebook将其提交apache基金会开源了。对于当时的facebook来说创造thrift是为了解决facebook系统中各系统间大数据量的传输通信以及系统之间语言环境不同需要跨平台的特性。所以thrift可以支持多种程序语言，支持的语言如下：
![](http://www.blogjava.net/images/blogjava_net/ldwblog/thrift%E6%94%AF%E6%8C%81%E7%9A%84%E8%AF%AD%E8%A8%80.png)
在多种不同的语言之间通信thrift可以作为二进制的高性能的通讯中间件，支持数据(对象)序列化和多种类型的RPC服务。Thrift是IDL(interface definition language)描述性语言的一个具体实现，Thrift适用于程序对程序静态的数据交换，需要先确定好他的数据结构，他是完全静态化的，当数据结构发生变化时，必须重新编辑IDL文件，代码生成，再编译载入的流程，跟其他IDL工具相比较可以视为是Thrift的弱项，Thrift适用于搭建大型数据交换及存储的通用工具，对于大型系统中的子系统间数据传输相对于JSON和xml无论在性能、传输大小上有明显的优势。
![](http://www.blogjava.net/images/blogjava_net/ldwblog/thrift%E6%9E%B6%E6%9E%84%E5%9B%BE.png)
u **基础架构**
如下图所示是thrift的协议栈整体的架构，thrift是一个客户端和服务器端的架构体系（c/s），在最上层是用户自行实现的业务逻辑代码。    第二层是由thrift编译器自动生成的代码，主要用于结构化数据的解析，发送和接收。TServer主要任务是高效的接受客户端请求，并将请求转发给Processor处理。Processor负责对客户端的请求做出响应，包括RPC请求转发，调用参数解析和用户逻辑调用，返回值写回等处理。从TProtocol以下部分是thirft的传输协议和底层I/O通信。TProtocol是用于数据类型解析的，将结构化数据转化为字节流给TTransport进行传输。TTransport是与底层数据传输密切相关的传输层，负责以字节流方式接收和发送消息体，不关注是什么数据类型。底层IO负责实际的数据传输，包括socket、文件和压缩数据流等。
协议层TProtocol：
在传输协议上总体上划分为文本(text)和二进制(binary)传输协议, 为节约带宽，提供传输效率，一般情况下使用二进制类型的传输协议为多数。
   1>TBinaryProtocol – 二进制编码格式进行数据传输。 
   2>TCompactProtocol – 高效的编码方式，使用类似于protobuffer的Variable-Length Quantity (VLQ) 编码(可以节省传输空间，使数据的传输效率更高)对数据进行压缩。
 关于VLQ了解更多（[http://en.wikipedia.org/wiki/Variable-length_quantity](http://en.wikipedia.org/wiki/Variable-length_quantity)）
   3>TJSONProtocol – 使用JSON的数据编码协议进行数据传输。 
   4>TSimpleJSONProtocol – 这种节约只提供JSON只写的协议，适用于通过脚本语言解析    
   5>TDebugProtocol – 在开发的过程中帮助开发人员调试用的，以文本的形式展现方便阅读。
传输层TTransport
    1>TSocket- 使用阻塞式I/O进行传输，也是最常见的模式。 
    2>TFramedTransport- 使用非阻塞方式，按块的大小，进行传输，类似于Java中的NIO。               
    3>TFileTransport- 顾名思义按照文件的方式进程传输，虽然这种方式不提供Java的实现，但是实现起来非常简单。 
    4>TMemoryTransport- 使用内存I/O，就好比Java中的ByteArrayOutputStream实现。 
    5>TZlibTransport- 使用执行zlib压缩，不提供Java的实现。
    6>TNonblockingTransport-使用非阻塞方式，用于构建异步客户端。
服务端类型 ：
    1>TSimpleServer -  单线程服务器端使用标准的阻塞式I/O。     
    2>TThreadPoolServer -  多线程服务器端使用标准的阻塞式I/O。 
    3>TNonblockingServer – 多线程服务器端使用非阻塞式I/O，并且实现了Java中的NIO通道。
u **数据类型**
Thrift 脚本可定义的数据类型包括以下几种类型：
- 基本类型：- bool：布尔值，true 或 false，对应 Java 的 boolean
- byte：8 位有符号整数，对应 Java 的 byte
- i16：16 位有符号整数，对应 Java 的 short
- i32：32 位有符号整数，对应 Java 的 int
- i64：64 位有符号整数，对应 Java 的 long
- double：64 位浮点数，对应 Java 的 double
- string：未知编码文本或二进制字符串，对应 Java 的 String
- 结构体类型：- struct：定义公共的对象，类似于 C 语言中的结构体定义，在 Java 中是一个 JavaBean
- 容器类型：- list：对应 Java 的 ArrayList
- set：对应 Java 的 HashSet
- map：对应 Java 的 HashMap
- 异常类型：- exception：对应 Java 的 Exception
- 服务类型：service：对应服务的类
u **安装使用**
在Windows下安装前需要很多依赖包的安装，比较麻烦。下面介绍了在Linux下的源码安装，直接wget或手动下载最tar.gz安装包编译安装:
tar -xvf thrift-0.9.1.tar.gz
cd thrift-0.9.1
./configure
make 
make install
使用命令thrift -version，显示Thrift version 0.9.1 则表示安装成功。
我们使用到了java，所以通过ant方式构建thrift相关lib包，进入$THRIFT_HOME/lib/java目录下，在ant编译libthrift出现了如下问题：
Buildfile: /letv/apps_install/thrift-0.9.1/lib/java/build.xml 
setup.init: 
mvn.ant.tasks.check: 
proxy: 
mvn.ant.tasks.download: 
[get] Getting: [http://repo1.maven.org/maven2/org/apache/maven/maven-ant-tasks/2.1.3/maven-ant-tasks-2.1.3.jar](http://repo1.maven.org/maven2/org/apache/maven/maven-ant-tasks/2.1.3/maven-ant-tasks-2.1.3.jar)
[get] To: /letv/apps_install/thrift-0.9.1/lib/java/build/tools/maven-ant-tasks-2.1.3.jar 
[get] Not modified - so not downloaded 
mvn.init: 
Unable to obtain resource from /letv/apps_install/thrift-0.9.1/lib/java/build/tools/maven-ant-tasks-2.1.3.jar: java.util.zip.ZipException: error in opening zip file
[typedef] Unable to obtain resource from /letv/apps_install/thrift-0.9.1/lib/java/build/tools/maven-ant-tasks-2.1.3.jar: 
[typedef] java.util.zip.ZipException: error in opening zip file 
[typedef] at java.util.zip.ZipFile.open(Native Method) 
[typedef] at java.util.zip.ZipFile.<init>(ZipFile.java:214) 
[typedef] at java.util.zip.ZipFile.<init>(ZipFile.java:144) 
[typedef] at java.util.jar.JarFile.<init>(JarFile.java:153) 
[typedef] at java.util.jar.JarFile.<init>(JarFile.java:117) 
[typedef] at org.apache.tools.ant.AntClassLoader.getResourceURL(AntClassLoader.java:1014) 
[typedef] at org.apache.tools.ant.AntClassLoader$ResourceEnumeration.findNextResource(AntClassLoader.java:150) 
[typedef] at org.apache.tools.ant.AntClassLoader$ResourceEnumeration.<init>(AntClassLoader.java:111) 
[typedef] at org.apache.tools.ant.AntClassLoader.findResources(AntClassLoader.java:954) 
[typedef] at org.apache.tools.ant.AntClassLoader.getNamedResources(AntClassLoader.java:923) 
[typedef] at org.apache.tools.ant.loader.AntClassLoader5.getResources(AntClassLoader5.java:58) 
[typedef] at org.apache.tools.ant.taskdefs.Definer.resourceToURLs(Definer.java:360) 
[typedef] at org.apache.tools.ant.taskdefs.Definer.execute(Definer.java:246) 
[typedef] at org.apache.tools.ant.UnknownElement.execute(UnknownElement.java:292) 
[typedef] at sun.reflect.GeneratedMethodAccessor4.invoke(Unknown Source) 
[typedef] at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43) 
[typedef] at java.lang.reflect.Method.invoke(Method.java:601) 
[typedef] at org.apache.tools.ant.dispatch.DispatchUtils.execute(DispatchUtils.java:106) 
[typedef] at org.apache.tools.ant.Task.perform(Task.java:348) 
[typedef] at org.apache.tools.ant.Target.execute(Target.java:435) 
[typedef] at org.apache.tools.ant.Target.performTasks(Target.java:456) 
[typedef] at org.apache.tools.ant.Project.executeSortedTargets(Project.java:1393) 
[typedef] at org.apache.tools.ant.Project.executeTarget(Project.java:1364) 
[typedef] at org.apache.tools.ant.helper.DefaultExecutor.executeTargets(DefaultExecutor.java:41) 
[typedef] at org.apache.tools.ant.Project.executeTargets(Project.java:1248) 
[typedef] at org.apache.tools.ant.Main.runBuild(Main.java:851) 
[typedef] at org.apache.tools.ant.Main.startAnt(Main.java:235) 
[typedef] at org.apache.tools.ant.launch.Launcher.run(Launcher.java:280) 
[typedef] at org.apache.tools.ant.launch.Launcher.main(Launcher.java:109) 
[typedef] Could not load definitions from resource org/apache/maven/artifact/ant/antlib.xml. It could not be found. 
BUILD FAILED 
/letv/apps_install/thrift-0.9.1/lib/java/build.xml:279: Problem: failed to create task or type antlib:org.apache.maven.artifact.ant:remoteRepository 
Cause: The name is undefined. 
Action: Check the spelling. 
Action: Check that any custom tasks/types have been declared. 
Action: Check that any <presetdef>/<macrodef> declarations have taken place. 
No types or tasks have been defined in this namespace yet 
This appears to be an antlib declaration. 
Action: Check that the implementing library exists in one of: 
-/letv/apps_install/apache-ant-1.9.4/lib 
-/root/.ant/lib 
-a directory added on the command line with the -lib argument 
Total time: 2 seconds
解决方法：ant通过yum方式安装，默认版本1.7.1，自行安装最新版本1.9.4，成功编译后在当前目录生成了build文件夹，包括libthrift-0.9.1.jar以及依赖的lib：
commons-codec-1.6.jar
commons-lang3-3.1.jar
commons-logging-1.1.1.jar
httpclient-4.2.5.jar
httpcore-4.2.4.jar
junit-4.4.jar
log4j-1.2.14.jar
servlet-api-2.5.jar
slf4j-api-1.5.8.jar
slf4j-log4j12-1.5.8.jar
u **开发流程及代码示例![](http://www.blogjava.net/images/blogjava_net/ldwblog/thrift%E5%BC%80%E5%8F%91.png)**
1，编写IDL文件helloService.thrift，如下：
/**
 * Hello world Testing
 *
 */
namespace java com.le.mms
service HelloService {
    i32 sayInt(1:i32 param)
    string sayString(1:string param)
    bool sayBoolean(1:bool param)
    void sayVoid()
}
2. 使用thrift编译器生成所需语言的代码
thrift --gen java helloService.thrift
在当前目录下生成gen-java目录，里面生成了HelloService.java文件。
Thrift --gen py helloService.thrift 可以生成python相关代码。
3. 新建Java工程或maven工程，java工程引入ant生成的lib，maven工程pom.xml中引入依赖。
   <dependency>
      <groupId>org.apache.thrift</groupId>
      <artifactId>libthrift</artifactId>
      <version>0.9.1</version>
    </dependency>
自动会将依赖的lib下载下来。
4. 编写HelloServiceImpl.java业务实现类. 需要实现HelloService.Iface接口，由thrift自动生成的代码。
package com.le.mms.thrift; 
import org.apache.thrift.TException;
public class HelloServiceImpl implements HelloService.Iface{
    @Override
    public int sayInt(int param) throws TException {
        System.out.println("say int :" + param);
        return param;
    }
    @Override
    public String sayString(String param) throws TException {
        System.out.println("say string :" + param);
        return param;
    }
    @Override
    public boolean sayBoolean(boolean param) throws TException {
        System.out.println("say boolean :" + param);
        return param;
    }
    @Override
    public void sayVoid() throws TException {
        System.out.println("say void ...");
    }
}
5. 编写thrift-java服务器端，监听端口9090
package com.le.mms.thrift.server;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TBinaryProtocol.Factory;
import org.apache.thrift.server.TServer;
import org.apache.thrift.server.TSimpleServer;
import org.apache.thrift.transport.TServerSocket;
import org.apache.thrift.transport.TTransportException;
import com.le.mms.thrift.HelloService;
import com.le.mms.thrift.HelloServiceImpl;
import com.le.mms.thrift.HelloService.Processor;
/**
 * 启动thrift的java服务器端
 *
 * @author david
 *
 */
public class HelloServiceServer {
    public static void main(String[] args) {
        try {
            // 设置服务器端口
            TServerSocket serverTransport = new TServerSocket(9090);
            // 设置二进制协议工厂
            Factory protocolFactory = new TBinaryProtocol.Factory();
            //处理器关联业务实现
            Processor<HelloService.Iface> processor = new HelloService.Processor<HelloService.Iface>(new HelloServiceImpl());
            // 1. 使用单线程标准阻塞I/O模型
            TServer.Args simpleArgs = new TServer.Args(serverTransport);
            simpleArgs.processor(processor);
            simpleArgs.protocolFactory(protocolFactory);
            TServer server = new TSimpleServer(simpleArgs);
            // 2. 使用线程池服务模型
//            TThreadPoolServer.Args poolArgs = new TThreadPoolServer.Args(serverTransport);
//            poolArgs.processor(processor);
//            poolArgs.protocolFactory(protocolFactory);
//            TServer poolServer = new TThreadPoolServer(poolArgs);
//            poolServer.serve();
            System.out.println("开启thrift服务器，监听端口：9090");
            server.serve();
        } catch (TTransportException e) {
            e.printStackTrace();
        }
    }
}
6.编写thrift-java客户端，发出请求。
package com.le.mms.thrift.client;
import org.apache.thrift.TException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TTransport;
import org.apache.thrift.transport.TTransportException;
import com.le.mms.thrift.HelloService; 
/**
 * 调用thrift的java客户端
 *
 * @author david
 *
 */
public class HelloServiceClient {
    public static void main(String[] args) {
        try {
            // 设置调用的服务地址-端口
            TTransport transport = new TSocket("localhost", 9090);
            //  使用二进制协议
            TProtocol protocol = new TBinaryProtocol(transport);
            // 使用的接口
            HelloService.Client client = new HelloService.Client(protocol);
            //打开socket
            transport.open();
            client.sayBoolean(true);
            client.sayString("Hello world");
            client.sayInt(20141111);
            client.sayVoid();
            transport.close();
        } catch (TTransportException e) {
            e.printStackTrace();
        } catch (TException te) {
            te.printStackTrace();
        }
    }
}
以上编写完了server和client端代码，运行server开启监听。然后运行client就能够与server端进行rpc调用方式的通讯了。
