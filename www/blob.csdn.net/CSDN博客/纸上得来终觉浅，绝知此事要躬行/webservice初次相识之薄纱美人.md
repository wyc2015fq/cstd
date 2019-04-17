# webservice初次相识之薄纱美人 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2012年04月20日 00:37:30[boonya](https://me.csdn.net/boonya)阅读数：1493








前言：webservice初次相识，只看到万分之一皮毛，请高手指点！

1、什么是webservice

百度百科这样解释：

Web Services是由企业发布的完成其特定商务需求的在线应用服务,其他公司或应用软件能够通过Internet来访问并使用这项在线服务。




它是一种构建[应用程序](http://baike.baidu.com/view/330120.htm)的普遍模型,可以在任何支持网络通信的[操作系统](http://baike.baidu.com/view/880.htm)中实施运行;它是一种新的webwebservice应用程序分支，是自包含、自描述、模块化的应用，可以发布、定位、通过web调用。Web Service是一个应用组件,它逻辑性的为其他应用程序提供数据与服务.各应用程序通过网络协议和规定的一些标准数据格式（Http，XML，Soap)来访问Web Service,通过Web Service内部执行得到所需结果.Web Service可以执行从简单的请求到复杂商务处理的任何功能。一旦部署以后，其他Web Service应用程序可以发现并调用它部署的服务。



Web Services用简单点的话说，就是系统对外的接口！


2、SOAP(simple object access protocol)------ in XML（like WSDL）
  使用注解：在接口上添加@WebService---表明这是一个webservice服务接口
  实现并发布webservice接口，客户端访问：
  http://localhost:8080/sns?wsdl---此为传递的XML消息
  XML分析：
  types:用来定义访问的类型
  messages：SOAP（信息数量是服务的2倍甚至更多）
  protoType：指明服务器的接口，通过operation指定in和out的消息
  operation->input:  参数（输入消息）
  operation->output: 返回值（输出消息）
  binding:绑定消息传递（输入/输出）所使用的格式
  service:指定服务所发布的名称

  webservice的一个常用工具：
        TCPMon设置监听<request  response>messages
  设置监听端口模拟客户端访问服务器过程：  客户端   <------>TCPMon（转发/回传信息）<------>   服务器
3、发布、调用和编译打包webservice服务：
（1）、服务器端发布服务：<---------接口即服务

```java
public class TestServer {
    /**
* @param args
*/
public static void main(String[] args) {
String address="http://localhost:8080/sns";
                Endpoint.publish(address, new MyserviceImpl());//接口实现类
}
}
```
（2）、客户端展示结果：<----------调用服务

```java
public class TestClient {
/**
* @param args
* @throws MalformedURLException 
*/
public static void main(String[] args) throws MalformedURLException {
MyserviceImplService msis=new MyserviceImplService();
Imyservice ims=msis.getMyserviceImplPort();
System.out.println(ims.minus(40, 20));
System.out.println(ims.add(40, 20));
System.out.println("find a root dir printinto console: wsimport -d d:temp/webservice/ -keep -verbose  http://localhost:8080/sns?wsdl");
}
}
```


(3)、打包生成源文件及编译后的详细信息
   cmd:  首先找到根目录d:  输入 wsimport -d d:temp/webservice/ -keep -verbose  http://localhost:8080/sns?wsdl
4、代码优先还是契约优先？
（1）、代码优先（注重开发效率）------直接用代码实现应用
    JWS还支持Web服务开发采用代码优先，契约编译方法。JWS鼓励采用代码优先方法以便可以很容易地生成WSDL文档。
    一旦服务被发布，相应的WSDL文档也就产生了，客户端也就可以据此进行开发。在Java语言中，通过注解，程序员
    可以决定，在某些特殊需求下，如何产生WSDL文档，或者依次产生WSDL相关工件。
    代码优先实例：http://book.51cto.com/art/200911/163800.htm
（2）、契约优先（注重开发标准）------适合异构系统
  定义契约的层：
    UI层：包含界面、验证逻辑和用户控件之间的交互
    逻辑层：实现需求、业务规则、计算和报告生成
    数据库层：存储数据，并保证表之间引用的完整性
  （注：逻辑模块独立于数据库存储）
  定义契约的服务：
   从服务能够得到什么样的结果，服务有哪些方法，这些方法的参数需要哪些数据结构。
   契约优先原则：http://book.51cto.com/art/201111/301309.htm
   契约优先实例：http://blog.ziki.cn/349.html

 个人主观看法：契约优先是大家最愿意看到的结果但是实际中的开发是更注重效率和成本的，所以现实中代码优先的例子很多。

5、查看WSDL的接口和参数

可以打开eclipse的Java EE下的webservice explorer查看访问的WSDL的服务，包括接口及参数


url:http://localhost:8080/sns?wsdl



