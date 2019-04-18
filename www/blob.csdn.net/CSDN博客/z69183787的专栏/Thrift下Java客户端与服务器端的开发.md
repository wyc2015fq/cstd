# Thrift下Java客户端与服务器端的开发 - z69183787的专栏 - CSDN博客
2016年05月06日 17:12:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1815
**1****创建Thrift文件**
Thrift文件与编程语言无关，用于定义数据类型和服务接口，然后生成用来构建RPC客户和服务器所需的全部代码。
**1.1****编写testJava.thrift**
**[html]**[view
 plain](http://blog.csdn.net/andy_yf/article/details/7396373#)[copy](http://blog.csdn.net/andy_yf/article/details/7396373#)
[print](http://blog.csdn.net/andy_yf/article/details/7396373#)[?](http://blog.csdn.net/andy_yf/article/details/7396373#)
- #!/usr/local/bin/thrift --gen java  
- 
- namespace java Test  
- 
- service Something {  
-    i32 ping()  
- }  
**1.2****运行thrift编译器，生成项目源文件**
在linux命令行下输入：Thrift –gen javatestJava.thrift
生成的源文件在./gen-java/目录下。
**2****创建Java服务器**
进入./gen-java目录，确保系统的CLASSPATH设置正确。libthrift.jar,，slf4j-api和slf4j-simple这几个包都需要包含在CLASSPATH里。
**2.1****编写SomethingImpl.java**
**[html]**[view
 plain](http://blog.csdn.net/andy_yf/article/details/7396373#)[copy](http://blog.csdn.net/andy_yf/article/details/7396373#)
[print](http://blog.csdn.net/andy_yf/article/details/7396373#)[?](http://blog.csdn.net/andy_yf/article/details/7396373#)
- package Test;  
- 
- import org.apache.thrift.TException;  
- 
- class SomethingImpl implements Something.Iface {  
-  public SomethingImpl() {}  
-  public int ping() throws TException {  
-     System.out.println( "Recieve ping from client..." );  
-     return 0;  
- }  
- }  
**2.2****创建Server.java**
**[html]**[view
 plain](http://blog.csdn.net/andy_yf/article/details/7396373#)[copy](http://blog.csdn.net/andy_yf/article/details/7396373#)
[print](http://blog.csdn.net/andy_yf/article/details/7396373#)[?](http://blog.csdn.net/andy_yf/article/details/7396373#)
- package Test;  
- 
- import java.io.IOException;  
- import org.apache.thrift.protocol.TBinaryProtocol;  
- import org.apache.thrift.protocol.TBinaryProtocol.Factory;  
- import org.apache.thrift.server.TServer;  
- import org.apache.thrift.server.TThreadPoolServer;  
- import org.apache.thrift.transport.TServerSocket;  
- import org.apache.thrift.transport.TTransportException;  
- 
- public class Server {  
- private void start() {  
- try {  
-    TServerSocket serverTransport = new TServerSocket(7911);  
-    Something.Processor processor = new Something.Processor(newSomethingImpl());  
-    Factory protFactory = new TBinaryProtocol.Factory(true, true);  
-    TServer server = new TThreadPoolServer(processor, serverTransport,protFactory);  
-    System.out.println("Starting server on port 7911 ...");  
-    server.serve();  
-   } catch (TTransportException e) {  
-    e.printStackTrace();  
-   } catch (Exception e) {  
-    e.printStackTrace();  
- }  
- }  
- 
- public static void main(String args[]){  
-    Server srv = new Server();  
-    srv.start();  
- }    
- }  
**3****创建Java客户端**
**创建Client.java**
**[html]**[view
 plain](http://blog.csdn.net/andy_yf/article/details/7396373#)[copy](http://blog.csdn.net/andy_yf/article/details/7396373#)
[print](http://blog.csdn.net/andy_yf/article/details/7396373#)[?](http://blog.csdn.net/andy_yf/article/details/7396373#)
- package Test;  
- 
- import java.io.IOException;  
- import org.apache.thrift.*;  
- import org.apache.thrift.protocol.*;  
- import org.apache.thrift.transport.*;  
- 
- public class Client {  
-      public static void main(String [] args) {  
-           try {  
-                     TTransport transport = new
 TSocket("localhost", 7911);  
-                     TProtocol protocol = new
 TBinaryProtocol(transport);  
-                     Something.Client client =new Something.Client(protocol);  
-                     transport.open();  
-                     System.out.println("Client calls ping()");  
-                     client.ping();  
-                     transport.close();  
-                } catch (TException x) {  
-                     x.printStackTrace();  
-                }    
-        }    
- }   
**4****编译及运行**
**4.1****编译**
在linux命令行下输入：javac *.java，生成客户端与服务器端的class文件。
**4.2****运行**
首先启动服务器。退到gen-java目录，输入java Test/Server，屏幕显示如下：
Starting server on port 7911 ...
然后启动客户端。在同一目录下输入java Test/Client，屏幕显示如下：
Client calls ping()
这时服务器端的输出多了一行：
Recieve ping from client...
