# WebService学习总结(三)——使用JDK开发WebService - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [WebService学习总结(三)——使用JDK开发WebService](https://www.cnblogs.com/xdp-gacl/p/4259481.html)



## 一、WebService的开发手段

　　使用Java开发WebService时可以使用以下两种开发手段

　　　　1、 使用JDK开发(1.6及以上版本)

　　　　2、使用CXF框架开发(工作中)

## 二、使用JDK开发WebService

### 2.1、开发WebService服务器端

　　1、定义一个interface，使用@WebService注解标注接口，使用@WebMethod注解标注接口中定义的所有方法，如下所示：

```
1 package me.gacl.ws;
 2 
 3 import javax.jws.WebMethod;
 4 import javax.jws.WebService;
 5 
 6 /**
 7  * @author gacl
 8  * 定义SEI(WebService EndPoint Interface(终端))
 9  */
10 //使用@WebService注解标注WebServiceI接口
11 @WebService
12 public interface WebServiceI {
13 
14     //使用@WebMethod注解标注WebServiceI接口中的方法
15     @WebMethod
16     String sayHello(String name);
17     
18     @WebMethod
19     String save(String name,String pwd);
20 }
```

　　2、编写interface的实现类，使用**@WebService**注解标注实现类，实现接口中定义的所有方法，如下所示：

```
1 package me.gacl.ws;
 2 
 3 import javax.jws.WebService;
 4 
 5 /**
 6  * @author gacl
 7  * SEI的具体实现
 8  */
 9 //使用@WebService注解标注WebServiceI接口的实现类WebServiceImpl
10 @WebService
11 public class WebServiceImpl implements WebServiceI {
12 
13     @Override
14     public String sayHello(String name) {
15         System.out.println("WebService sayHello "+name);
16         return "sayHello "+name;
17     }
18 
19     @Override
20     public String save(String name, String pwd) {
21         System.out.println("WebService save "+name+"， "+pwd);
22         return "save Success";
23     }
24 }
```

　　3、使用Endpoint(终端)类发布webservice，代码如下：

```
1 package me.gacl.ws.test;
 2 
 3 import javax.xml.ws.Endpoint;
 4 
 5 import me.gacl.ws.WebServiceImpl;
 6 
 7 /**
 8  * @author gacl
 9  *
10  * 发布Web Service
11  */
12 public class WebServicePublish {
13 
14     public static void main(String[] args) {
15         //定义WebService的发布地址，这个地址就是提供给外界访问Webervice的URL地址，URL地址格式为：http://ip:端口号/xxxx
16         //String address = "http://192.168.1.100:8989/";这个WebService发布地址的写法是合法的
17         //String address = "http://192.168.1.100:8989/Webservice";这个WebService发布地址的是合法的
18         String address = "http://192.168.1.100:8989/WS_Server/Webservice";
19         //使用Endpoint类提供的publish方法发布WebService，发布时要保证使用的端口号没有被其他应用程序占用
20         Endpoint.publish(address , new WebServiceImpl());
21         System.out.println("发布webservice成功!");
22     }
23 }
```

　　运行WebServicePublish类，就可以将编写好的WebService发布好了，WebService的访问URL是：[http://192.168.1.100:8989/WS_Server/Webservice](http://192.168.1.100:8989/WS_Server/Webservice) ，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/291553270817392.png)

　　这里我们编写了一个WebServicePublish类来发布WebService，如果是Web项目，那么我们可以使用监听器或者Servlet来发布WebService，如下：

　　1、使用ServletContextListener监听器发布WebService

```
1 package me.gacl.listener;
 2 
 3 import javax.servlet.ServletContextEvent;
 4 import javax.servlet.ServletContextListener;
 5 import javax.servlet.annotation.WebListener;
 6 import javax.xml.ws.Endpoint;
 7 import me.gacl.ws.WebServiceImpl;
 8 
 9 /**
10  * @author gacl
11  * 用于发布WebService的监听器
12  */
13 //使用Servlet3.0提供的@WebListener注解将实现了ServletContextListener接口的WebServicePublishListener类标注为一个Listener
14 @WebListener
15 public class WebServicePublishListener implements ServletContextListener {
16 
17     @Override
18     public void contextDestroyed(ServletContextEvent sce) {
19 
20     }
21 
22     @Override
23     public void contextInitialized(ServletContextEvent sce) {
24         //WebService的发布地址
25         String address = "http://192.168.1.100:8080/WS_Server/WebService";
26         //发布WebService，WebServiceImpl类是WebServie接口的具体实现类
27         Endpoint.publish(address , new WebServiceImpl());
28         System.out.println("使用WebServicePublishListener发布webservice成功!");
29     }
30 }
```

　　将Web应用部署到服务器运行时，在初始化Web应用上下文时，就会发布WebService了。

　　接着我们可以使用发布的URL地址访问WebService，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/291648418945385.png)

　　2、使用Servlet发布WebService

```
1 package me.gacl.web.controller;
 2 
 3 import javax.servlet.ServletException;
 4 import javax.servlet.annotation.WebServlet;
 5 import javax.servlet.http.HttpServlet;
 6 import javax.xml.ws.Endpoint;
 7 import me.gacl.ws.WebServiceImpl;
 8 
 9 /**
10  * @author gacl
11  * 用于发布WebService的Servlet
12  */
13 //使用Servlet3.0提供的@WebServlet注解将继承HttpServlet类的普通Java类标注为一个Servlet
14 //将value属性设置为空字符串，这样WebServicePublishServlet就不提供对外访问的路径
15 //loadOnStartup属性设置WebServicePublishServlet的初始化时机
16 @WebServlet(value="",loadOnStartup=0)
17 public class WebServicePublishServlet extends HttpServlet {
18 
19     /* (non-Javadoc)
20      * @see javax.servlet.GenericServlet#init()
21      * 在WebServicePublishServlet初始化时发布WebService
22      */
23     public void init() throws ServletException {
24         //WebService的发布地址
25         String address = "http://192.168.1.100:8888/WebService";
26         //发布WebService，WebServiceImpl类是WebServie接口的具体实现类
27         Endpoint.publish(address , new WebServiceImpl());
28         System.out.println("使用WebServicePublishServlet发布webservice成功!");
29     }
30 }
```

　　将Web应用部署到服务器运行时，在初始化WebServicePublishServlet时，就会发布WebService了。如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/291639417221127.png)

　　接着我们可以使用发布的URL地址访问WebService，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/291644548476288.png)

　　关于发布WebService主要就是通过javax.xml.ws.Endpoint类提供的静态方法publish进行发布，如果是普通的java项目，那么可以专门写一个类用于发布WebService，如果是Web项目，那么可以使用ServletContextListener或者Servlet进行发布。

### 2.2、开发WebService客户端

　　1、借助jdk的wsimort.exe工具生成客户端代码，wsimort.exe工具位于Jdk的bin目录下，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/291705220198829.png)

**　　执行命令：wsimport -keep url(**url为wsdl文件的路径)生成客户端代码。

　　创建一个WebService客户端测试项目，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/291708542226858.png)

　　打开命令行窗口，切换到src目录，执行"wsimport -keep [http://192.168.1.100:8888/WebService?wsdl](http://192.168.1.100:8888/WebService?wsdl)"生成客户端代码，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/291714174096228.png)

　　执行命令的过程中没有出现任何错误，那么代码就生成成功了，刷新一下src目录，就可以看到生成的代码了，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/291716493008897.png)

　　2、 借助生成的代码编写调用WebService对外提供的方法

　　wsimport工具帮我们生成了好几个java类，但我们只需要关心WebServiceImplService类和WebServiceImpl接口的使用即可，如下所示：

```
1 package me.gacl.ws.client;
 2 
 3 import me.gacl.ws.WebServiceImpl;
 4 import me.gacl.ws.WebServiceImplService;
 5 
 6 /**
 7  * @author gacl
 8  * 调用WebService的客户端
 9  */
10 public class WSClient {
11 
12     public static void main(String[] args) {
13         //创建一个用于产生WebServiceImpl实例的工厂，WebServiceImplService类是wsimport工具生成的
14         WebServiceImplService factory = new WebServiceImplService();
15         //通过工厂生成一个WebServiceImpl实例，WebServiceImpl是wsimport工具生成的
16         WebServiceImpl wsImpl = factory.getWebServiceImplPort();
17         //调用WebService的sayHello方法
18         String resResult = wsImpl.sayHello("孤傲苍狼");
19         System.out.println("调用WebService的sayHello方法返回的结果是："+resResult);
20         System.out.println("---------------------------------------------------");
21         //调用WebService的save方法
22         resResult = wsImpl.save("孤傲苍狼","123");
23         System.out.println("调用WebService的save方法返回的结果是："+resResult);
24     }
25 }
```

　　客户端调用服务器端的WebService方法运行结果如下：

![](https://images0.cnblogs.com/blog/289233/201501/291732044728721.png)

　　从调用返回的结果显示，借助wsimport工具生成的客户端代码已经成功调用到了WebService中的方法。以上就是使用JDK开发WebService的相关内容。









