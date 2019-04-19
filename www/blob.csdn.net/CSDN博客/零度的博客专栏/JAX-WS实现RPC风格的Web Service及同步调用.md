# JAX-WS实现RPC风格的Web Service及同步调用 - 零度的博客专栏 - CSDN博客
2016年05月09日 10:02:29[零度anngle](https://me.csdn.net/zmx729618)阅读数：777标签：[web service](https://so.csdn.net/so/search/s.do?q=web service&t=blog)
个人分类：[WebService](https://blog.csdn.net/zmx729618/article/category/6220066)
Web Service技术，无疑会越来越被广泛应用的，可以非常好的实现异构应用程序之间的相互通信。JAX-WS是SUN推出的一种实现Web Service的工具。下面是我学习用JAX-WS实现的一个很简单Web Service例子。
**服务端：**
新建Java Project
1、 创建服务接口Endpoint
**[java]**[view
 plain](http://blog.csdn.net/leon709/article/details/7102834#)[copy](http://blog.csdn.net/leon709/article/details/7102834#)
- package leon.rpcws.iface;  
- 
- import javax.jws.WebMethod;  
- import javax.jws.WebService;  
- import javax.jws.soap.SOAPBinding;  
- import javax.jws.soap.SOAPBinding.Style;  
- 
- @WebService
- @SOAPBinding(style = Style.RPC)  
- publicinterface IHello{  
- 
- @WebMethod
-     String sayHello(String name);  
- }  
2、 接口的实现类
**[java]**[view
 plain](http://blog.csdn.net/leon709/article/details/7102834#)[copy](http://blog.csdn.net/leon709/article/details/7102834#)
- package leon.rpcws.impl;  
- 
- import javax.jws.WebService;  
- import leon.rpcws.iface.IHello;  
- 
- @WebService(endpointInterface = "leon.rpcws.iface.IHello")  
- publicclass HelloImpl implements IHello {  
- 
- @Override
- public String sayHello(String name) {  
- return"Hello JAX-WS. Welcome, "+name;  
-     }  
- }  
3、发布
**[java]**[view
 plain](http://blog.csdn.net/leon709/article/details/7102834#)[copy](http://blog.csdn.net/leon709/article/details/7102834#)
- package leon.rpcws;  
- 
- import javax.xml.ws.Endpoint;  
- 
- import leon.rpcws.impl.HelloImpl;  
- 
- publicclass HelloPublisher {  
- publicstaticvoid main(String[] args) {  
-         Endpoint.publish("http://10.225.112.86:9999/ws/hello", new HelloImpl());  
-         System.out.println("published.");  
-     }  
- }  
以上3个类，就可以完成一个很简单的Web Service服务端的创建和发布。运行HelloPublisher，console打印出published.同时红色方块亮起，表示发布成功。
打开浏览器，访问：[http://10.225.112.86:9999/ws/hello?wsdl](http://10.225.112.86:9999/ws/hello?wsdl)
可以查看到发布的wsdl文件：
![](http://hi.csdn.net/attachment/201112/26/0_1324878079boEn.gif)
OK，服务端发布成功。
**客户端：**
再新建一个Java Project作为客户端
1、 将服务端的**IHello**接口打成jar包，提供给客户端工程，本实验中，我为了方便，在客户端创建了包和接口名完全一样的IHello接口。
2、 创建客户端调用类
**[java]**[view
 plain](http://blog.csdn.net/leon709/article/details/7102834#)[copy](http://blog.csdn.net/leon709/article/details/7102834#)
- package rpctest;  
- 
- import java.net.URL;  
- 
- import javax.xml.namespace.QName;  
- import javax.xml.ws.Service;  
- import leon.rpcws.iface.IHello;  
- 
- publicclass TestRPC {  
- publicstaticvoid main(String[] args) throws Exception {  
- //1: argument service URI, refer to wsdl document above
- //2: argument is service name, refer to wsdl document above
- 
-         URL url = new URL("http://10.225.112.86:9999/ws/hello?wsdl");  
-         QName qname = new QName("http://impl.rpcws.leon/", "HelloImplService");  
-         Service service = Service.create(url, qname);  
-         IHello hello = service.getPort(IHello.class);  
- 
-         System.out.println(hello.sayHello("Leon Su"));  
-     }  
- }  
控制台正确的打印出：Hello JAX-WS. Welcome, Leon Su
表示成功了。
另外在JDK6中集成了两个工具wsgen和import，在JDK根目录\bin下可以查看到的，这是两个很有用的工具，可以大大提高开发效率的。下面我们使用wsimport在客户端生成辅助类来调用服务端的Web Service。
1、新建一个Java Project以后，是一个“空的”的工程，什么都没有，我们也不需要服务端给我们提供SEI（Service Endpoint Interface），服务端只需要知道怎么能访问到wsdl文件就可以啦。事实上，WS从理论上也是的，客户端只需要得到服务端的wsdl文件，就足够调用服务端的WS了。
  客户端访问服务端的wsdl地址：[http://10.225.112.86:9999/ws/hello?wsdl](http://10.225.112.86:9999/ws/hello?wsdl)
2、打开Windows的DOS命令行，进入客户端的根目录，然后运行一下命令：
   wsimport -keep http://10.225.112.86:9999/ws/hello?wsdl 
    成功执行后，发现他给我们在客户端工程根目录下创建了一个**leon\rpcws\impl**目录，而且生成了两个类：**HelloImplService.java**和**IHello.java，**将这两个类源文件**剪切**到客户端工程的源文件目录下（默认src），注意要带包剪切，也就是说两个类的包为：**packageleon.rpcws.impl;**
这两个类就是我们访问服务端WS的辅助类了，现在可以如下访问了：
**[java]**[view
 plain](http://blog.csdn.net/leon709/article/details/7102834#)[copy](http://blog.csdn.net/leon709/article/details/7102834#)
- package leon.rpcws.test;  
- 
- import leon.rpcws.impl.HelloImplService;  
- import leon.rpcws.impl.IHello;  
- 
- publicclass HelloClient{  
- publicstaticvoid main(String[] args) {  
-         HelloImplService helloService = new HelloImplService();  
-         IHello hello = helloService.getHelloImplPort();  
-         System.out.println(hello.sayHello("Leon Su"));  
-     }  
- }  
服务端的WS发布启动，运行这个HelloClient，同样能成功的调用服务端的WS。
发现，这样访问也是很好看的哈！
好了，这就是一个简单的Web Service，这里只是同步调用，而且都是Java 类，Java Project。没有依托Web 容器。
如果要集成到Web 工程，那么就要添加配置文件，并且有种做法是把Web Service绑定成为Servlet。
