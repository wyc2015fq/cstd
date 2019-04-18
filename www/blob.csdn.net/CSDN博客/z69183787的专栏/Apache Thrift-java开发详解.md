# Apache Thrift - java开发详解 - z69183787的专栏 - CSDN博客
2016年11月02日 10:46:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：914
**1、添加依赖 jar**
```
<dependency>
  <groupId>org.apache.thrift</groupId>
  <artifactId>libthrift</artifactId>
  <version>0.8.0</version>
</dependency>
<dependency>
  <groupId>org.slf4j</groupId>
  <artifactId>slf4j-log4j12</artifactId>
  <version>1.6.1</version>
</dependency>
```
**2、编写IDL文件 Hello.thrift**
namespace [Java](http://lib.csdn.net/base/java)service.demo
service Hello {
    string helloString(1:string para)
    i32 helloInt(1:i32 para)
    bool helloBoolean(1:bool para)
    void helloVoid()
    string helloNull()
}
**3、生成代码**
`thrift -o <output directory> -gen java Hello.thrift`生成代码缩略图：
![](https://img-my.csdn.net/uploads/201208/06/1344246293_5921.jpg)
**4、编写实现类、实现Hello.Iface：**
缩略图：
![](https://img-my.csdn.net/uploads/201208/06/1344246424_7730.jpg)
**5、编写服务端，发布（阻塞式IO + 多线程处理）服务。**
**[java]**[view
 plain](http://blog.csdn.net/m13321169565/article/details/7836006#)[copy](http://blog.csdn.net/m13321169565/article/details/7836006#)
- /**
-      * 阻塞式、多线程处理
-      * 
-      * @param args
-      */
- @SuppressWarnings({ "unchecked", "rawtypes" })  
- publicstaticvoid main(String[] args) {  
- try {  
- //设置传输通道，普通通道
-             TServerTransport serverTransport = new TServerSocket(7911);  
- 
- //使用高密度二进制协议
-             TProtocolFactory proFactory = new TCompactProtocol.Factory();  
- 
- //设置处理器HelloImpl
-             TProcessor processor = new Hello.Processor(new HelloImpl());  
- 
- //创建服务器
-             TServer server = new TThreadPoolServer(  
- new Args(serverTransport)  
-                     .protocolFactory(proFactory)  
-                     .processor(processor)  
-                 );  
- 
-             System.out.println("Start server on port 7911...");  
-             server.serve();  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
-     }  
**6、编写客户端，调用（阻塞式IO + 多线程处理）服务：**
**[java]**[view
 plain](http://blog.csdn.net/m13321169565/article/details/7836006#)[copy](http://blog.csdn.net/m13321169565/article/details/7836006#)
- publicstaticvoid main(String[] args) throws Exception {  
- // 设置传输通道 - 普通IO流通道
-         TTransport transport = new TSocket("localhost", 7911);  
-         transport.open();  
- 
- //使用高密度二进制协议
-         TProtocol protocol = new TCompactProtocol(transport);  
- 
- //创建Client
-         Hello.Client client = new Hello.Client(protocol);  
- 
- long start = System.currentTimeMillis();  
- for(int i=0; i<10000; i++){  
-             client.helloBoolean(false);  
-             client.helloInt(111);  
-             client.helloNull();  
-             client.helloString("dongjian");  
-             client.helloVoid();  
-         }  
-         System.out.println("耗时：" + (System.currentTimeMillis() - start));  
- 
- //关闭资源
-         transport.close();  
-     }  
现在已完成整个开发过程，超级无敌简单。
其中**服务端使用的协议需要与客户端保持一致**。
-------------------------------------------------------------------------------------------------------------------
上面展示了普通且常用的服务端和客户端，下面请看非阻塞IO，即[Java](http://lib.csdn.net/base/javaee)中的NIO：
**基于非阻塞IO（NIO）的服务端**：
**[java]**[view
 plain](http://blog.csdn.net/m13321169565/article/details/7836006#)[copy](http://blog.csdn.net/m13321169565/article/details/7836006#)
- publicstaticvoid main(String[] args) {  
- try {  
- //传输通道 - 非阻塞方式
-             TNonblockingServerTransport serverTransport = new TNonblockingServerSocket(7911);  
- 
- //异步IO，需要使用TFramedTransport，它将分块缓存读取。
-             TTransportFactory transportFactory = new TFramedTransport.Factory();  
- 
- //使用高密度二进制协议
-             TProtocolFactory proFactory = new TCompactProtocol.Factory();  
- 
- //设置处理器 HelloImpl
-             TProcessor processor = new Hello.Processor(new HelloImpl());  
- 
- //创建服务器
-             TServer server = new TThreadedSelectorServer(  
- new Args(serverTransport)  
-                     .protocolFactory(proFactory)  
-                     .transportFactory(transportFactory)  
-                     .processor(processor)  
-                 );  
- 
-             System.out.println("Start server on port 7911...");  
-             server.serve();  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
-     }  
**调用非阻塞IO（NIO）服务的客户端**：
**[java]**[view
 plain](http://blog.csdn.net/m13321169565/article/details/7836006#)[copy](http://blog.csdn.net/m13321169565/article/details/7836006#)
- publicstaticvoid main(String[] args) throws Exception {  
- //设置传输通道，对于非阻塞服务，需要使用TFramedTransport，它将数据分块发送
-         TTransport transport = new TFramedTransport(new TSocket("localhost", 7911));  
-         transport.open();  
- 
- //使用高密度二进制协议
-         TProtocol protocol = new TCompactProtocol(transport);  
- 
- //创建Client
-         Hello.Client client = new Hello.Client(protocol);  
- 
- long start = System.currentTimeMillis();  
- for(int i=0; i<10000; i++){  
-             client.helloBoolean(false);  
-             client.helloInt(111);  
-             client.helloNull();  
-             client.helloString("360buy");  
-             client.helloVoid();  
-         }  
-         System.out.println("耗时：" + (System.currentTimeMillis() - start));  
- 
- //关闭资源
-         transport.close();  
-     }  
-----------------------------------------------------------------------------------------------------------------------------------
**客户端异步调用**：
**[java]**[view
 plain](http://blog.csdn.net/m13321169565/article/details/7836006#)[copy](http://blog.csdn.net/m13321169565/article/details/7836006#)
- /** 调用[非阻塞IO]服务，异步 */
- publicstaticvoid main(String[] args) {  
- try {  
- //异步调用管理器
-             TAsyncClientManager clientManager = new TAsyncClientManager();  
- //设置传输通道，调用非阻塞IO。
- final TNonblockingTransport transport = new TNonblockingSocket("localhost", 7911);    
- //设置协议
-             TProtocolFactory protocol = new TCompactProtocol.Factory();    
- //创建Client
- final Hello.AsyncClient client = new Hello.AsyncClient(protocol, clientManager, transport);  
- // 调用服务 
-             System.out.println("开始：" + System.currentTimeMillis());  
-             client.helloBoolean(false, new AsyncMethodCallback<Hello.AsyncClient.helloBoolean_call>() {  
- publicvoid onError(Exception exception) {  
-                     System.out.println("错误1： " + System.currentTimeMillis());  
-                 }  
- publicvoid onComplete(helloBoolean_call response) {  
-                     System.out.println("完成1： " + System.currentTimeMillis());  
- try {  
-                         client.helloBoolean(false, new AsyncMethodCallback<Hello.AsyncClient.helloBoolean_call>() {  
- publicvoid onError(Exception exception) {  
-                                 System.out.println("错误2： " + System.currentTimeMillis());  
-                             }  
- 
- publicvoid onComplete(helloBoolean_call response) {  
-                                 System.out.println("完成2： " + System.currentTimeMillis());  
-                                 transport.close();  
-                             }  
-                         });  
-                     } catch (TException e) {  
-                         e.printStackTrace();  
-                     }  
-                 }  
-             });  
-             System.out.println("结束：" + System.currentTimeMillis());  
-             Thread.sleep(5000);  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
-     }  
-----------------------------------------------------------------------------------------------------------------------------------
**使用SSL的服务端：**
![](https://img-my.csdn.net/uploads/201208/06/1344247527_4406.jpg)
**调用基于SSL服务端的客户端：**
![](https://img-my.csdn.net/uploads/201208/06/1344247606_9192.jpg)
