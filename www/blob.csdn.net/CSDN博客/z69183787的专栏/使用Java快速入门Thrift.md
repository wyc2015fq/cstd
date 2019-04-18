# 使用Java快速入门Thrift - z69183787的专栏 - CSDN博客
2016年05月06日 17:13:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1020
[Apache Thrift](http://http//www.oschina.net/p/thrift)是一个facebook建立的RPC框架，现在是一个Apache的顶级项目。Thrift允许通过一个跨语言的定义文件的方式定义数据类型和服务接口，这个文件作为RPC客户端和服务器通信的标准，你也可以去看看Thrift的白皮书了解更多信息。
根据Apache Thrift的官方站点的描述，Thrift是一个：
> 
software framework, for scalable cross-language services development, combines a software stack with a code generation engine to build services that work efficiently and seamlessly between C++, Java, Python, PHP, Ruby, Erlang, Perl, Haskell, C#, Cocoa, JavaScript,
 Node.js, Smalltalk, OCaml and Delphi and other languages.
![](http://static.oschina.net/uploads/space/2012/0708/195707_W5if_82993.png)
安装Thrift比较的烦，但是在Windows下官方编译了一个thrift.exe，下载安装就行了。 
### 写 Thrift定义文件（.thrift file）
如果你之前有接触过这个东西的话，写定义文件非常的简单。但这里可以参考官方的教程快速开始。
示例定义文件（add.thrift）
```java
namespace java com.eviac.blog.samples.thrift.server  // defines the namespace   
   
typedef i32 int //typedefs to get convenient names for your types  
   
service AdditionService {  // defines the service to add two numbers  
        intadd(1:intn1, 2:intn2), //defines a method  
}
```
### 编译Thrift定义文件
下面的命令编译.thrift文件
thrift --gen <language> <Thrift filename>
对于我的例子来讲，命令是：
thrift --gen java add.thrift
在执行完代码后，在gen-java目录下你会发现构建RPC服务器和客户端有用的源代码。在我的例子中我将创建一个叫做AddtionService.java的java文件。
### 写一个 service handler
Service handler 类必须实现 AdditionService.Iface接口。
示例Service handler（AdditionServiceHandler.java）
```java
package com.eviac.blog.samples.thrift.server;  
import org.apache.thrift.TException;  
   
public class AdditionServiceHandler implements AdditionService.Iface {  
 @Override 
 publicint add(intn1, intn2) throws TException {  
  returnn1 + n2;  
 } 
   
}
```
### 写一个简单的服务器
下面的示例代码是一个简单的Thrift服务器。可以看到下面的代码中有一段是注释了的，可以去掉注释来启用多线程服务器。
示例服务器（MyServer.java）
```java
package com.eviac.blog.samples.thrift.server;  
   
import org.apache.thrift.transport.TServerSocket;  
import org.apache.thrift.transport.TServerTransport;  
import org.apache.thrift.server.TServer;  
import org.apache.thrift.server.TServer.Args;  
import org.apache.thrift.server.TSimpleServer;  
   
public class MyServer {  
   
 public static void StartsimpleServer(AdditionService.Processor<AdditionServiceHandler> processor) { 
  try{  
   TServerTransport serverTransport = new TServerSocket(9090); 
   TServer server = new TSimpleServer(  
     newArgs(serverTransport).processor(processor));  
   
   // Use this for a multithreaded server  
   // TServer server = new TThreadPoolServer(new  
   // TThreadPoolServer.Args(serverTransport).processor(processor));  
   
   System.out.println("Starting the simple server..."); 
   server.serve(); 
  }catch(Exception e) {  
   e.printStackTrace(); 
  } 
 } 
    
 public static void main(String[] args) {  
  StartsimpleServer(new AdditionService.Processor<AdditionServiceHandler>(new AdditionServiceHandler()));  
 } 
   
}
```
### 写一个客户端
下面的例子是一个使用Java写的客户端短使用AdditionService的服务。
```java
package com.eviac.blog.samples.thrift.client;  
   
import org.apache.thrift.TException;  
import org.apache.thrift.protocol.TBinaryProtocol;  
import org.apache.thrift.protocol.TProtocol;  
import org.apache.thrift.transport.TSocket;  
import org.apache.thrift.transport.TTransport;  
import org.apache.thrift.transport.TTransportException;  
   
public class AdditionClient {  
   
 public static void main(String[] args) {  
   
  try{  
   TTransport transport;  
   
   transport = new TSocket("localhost",9090); 
   transport.open(); 
   
   TProtocol protocol = new TBinaryProtocol(transport);  
   AdditionService.Client client = new AdditionService.Client(protocol);  
   
   System.out.println(client.add(100,200)); 
   
   transport.close(); 
  }catch(TTransportException e) {  
   e.printStackTrace(); 
  }catch(TException x) {  
   x.printStackTrace(); 
  } 
 } 
   
}
```
运行服务端代码（MyServer.java）将会看到下面的输出。
Starting the simple server...
然后运行客户端代码（AdditionClient.java），将会看到如下输出。
300
