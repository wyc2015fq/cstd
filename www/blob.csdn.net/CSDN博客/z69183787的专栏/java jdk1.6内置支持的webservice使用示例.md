# java jdk1.6内置支持的webservice使用示例 - z69183787的专栏 - CSDN博客
2016年10月27日 15:58:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1208
webService是一种跨语言的系统间交互标准。在[Java](http://lib.csdn.net/base/javaee)中使用webservice根据服务器端的服务根据描述生成WSDL文件，并将应用与此WSDL文件一起放入HTTP服务器中，借助服务工具根据WSDL文件生成客户端STUB代码。此代码的作用是将产生的对象请求信息封装成标准的SOAP格式数据，并发送到服务器端，服务器端根据接收到的SOAP格式数据进行转换，并最终通过反射调用响应类的响应方法。
jdk 6中集成了WEB service的支持，通过WebService 的annotation来暴露服务的实现，并通过Endpoint.publish将服务发布到指定的地址，客户端通过wsimport来访问响应地址的wsdl文件，生成调用服务器端服务的stub类信息，客户端即可通过生成的类来调用服务器的服务了。
具体示例如下：
1.对外暴露的接口
**[java]**[view
 plain](http://blog.csdn.net/zhongweijian/article/details/8008471#)[copy](http://blog.csdn.net/zhongweijian/article/details/8008471#)
- publicinterface TestWebService {  
- 
- public String echo();  
- }  
2.服务器端的实现类，并通过@WebService来指定对外提供服务的服务名称，客户端生成的类目和包名
**[java]**[view
 plain](http://blog.csdn.net/zhongweijian/article/details/8008471#)[copy](http://blog.csdn.net/zhongweijian/article/details/8008471#)
- import javax.jws.WebService;  
- import javax.xml.ws.Endpoint;  
- 
- 
- @WebService(name="MyTestWS",serviceName="MyWebService",targetNamespace="http://localhost/client")  
- publicclass WebServiceImpl implements TestWebService{  
- 
- @Override
- public String echo() {  
- return"webservice return msg";  
-     }  
- 
- publicstaticvoid main(String[] args) {  
-         Endpoint.publish("http://localhost:8080/MyWebService", new WebServiceImpl());  
-     }  
- }  
3.然后运行服务器的WebServiceImpl的main函数，暴露服务并将服务注册到一个http服务地址上，客户端通过jdk的bin下面的wsimport命令来获取服务器的wsdl文件并生成客户端的stub类信息
**[java]**[view
 plain](http://blog.csdn.net/zhongweijian/article/details/8008471#)[copy](http://blog.csdn.net/zhongweijian/article/details/8008471#)
- wsimport -keep http://localhost:8080/MyWebService?wsdl
4.然后在你的路径上就会生成下面几个类
![](http://dl.iteye.com/upload/attachment/0074/2000/4bbcc4c9-af31-335e-ab69-317929b1c80a.jpg)
5.然后我们编写客户端的调用代码
**[java]**[view
 plain](http://blog.csdn.net/zhongweijian/article/details/8008471#)[copy](http://blog.csdn.net/zhongweijian/article/details/8008471#)
- import localhost.client.MyWebService;  
- 
- 
- 
- publicclass WebServiceClient {  
- 
- publicstaticvoid main(String[] args) {  
-         MyWebService myWebService = new MyWebService();  
-         System.out.println(myWebService.getMyTestWSPort().echo());  
-     }  
- }  
6.执行客户端的调用代码，输出如下：
![](http://dl.iteye.com/upload/attachment/0074/2004/faef8a10-0680-3871-9b48-ad228a455e97.jpg)
7.我们看下最终生成客户端服务调用的类内容
**[java]**[view
 plain](http://blog.csdn.net/zhongweijian/article/details/8008471#)[copy](http://blog.csdn.net/zhongweijian/article/details/8008471#)
- package localhost.client;  
- 
- import java.net.MalformedURLException;  
- import java.net.URL;  
- import javax.xml.namespace.QName;  
- import javax.xml.ws.Service;  
- import javax.xml.ws.WebEndpoint;  
- import javax.xml.ws.WebServiceClient;  
- 
- 
- /**
-  * This class was generated by the JAXWS SI.
-  * JAX-WS RI 2.0_02-b08-fcs
-  * Generated source version: 2.0
-  * 
-  */
- @WebServiceClient(name = "MyWebService", targetNamespace = "http://localhost/client", wsdlLocation = "http://localhost:8080/MyWebService?wsdl")  
- publicclass MyWebService  
- extends Service  
- {  
- 
- privatefinalstatic URL MYWEBSERVICE_WSDL_LOCATION;  
- 
- static {  
-         URL url = null;  
- try {  
-             url = new URL("http://localhost:8080/MyWebService?wsdl");  
-         } catch (MalformedURLException e) {  
-             e.printStackTrace();  
-         }  
-         MYWEBSERVICE_WSDL_LOCATION = url;  
-     }  
- 
- public MyWebService(URL wsdlLocation, QName serviceName) {  
- super(wsdlLocation, serviceName);  
-     }  
- 
- public MyWebService() {  
- super(MYWEBSERVICE_WSDL_LOCATION, new QName("http://localhost/client", "MyWebService"));  
-     }  
- 
- /**
-      * 
-      * @return
-      *     returns MyTestWS
-      */
- @WebEndpoint(name = "MyTestWSPort")  
- public MyTestWS getMyTestWSPort() {  
- return (MyTestWS)super.getPort(new QName("http://localhost/client", "MyTestWSPort"), MyTestWS.class);  
-     }  
- 
- }  
- 
