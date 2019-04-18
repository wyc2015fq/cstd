# webservice获取客户端IP地址 - z69183787的专栏 - CSDN博客
2015年07月14日 21:49:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：15598
目录：
- 
基于JDK6 jax-ws开发的webservice获取客户端IP地址
- 
Endpoint.publish() 轻量级HTTP服务发布
- 
在web容器tomcat下发布
- 
基于XFire开发的webservice获取客户端IP地址
- 
基于Axis开发的webservice获取客户端IP地址
[一]、基于JDK6 jax-ws开发的webservice获取客户端IP地址
以：[http://www.micmiu.com/soa/webservice/jax-ws-demo/](http://www.micmiu.com/soa/webservice/jax-ws-demo/) 中的 [三]
 2 具体示例为基础：
1. 情况一：如果以 Endpoint.publish() 的方式发布：
服务端接口实现类：HelloServiceImpl.java 修改如下：

```
```
|123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051|packagecom.micmiu.jaxws.demo.impl;import java.net.InetSocketAddress;import javax.annotation.Resource;import javax.jws.WebMethod;import javax.jws.WebParam;import javax.jws.WebService;import javax.xml.ws.WebServiceContext;import javax.xml.ws.handler.MessageContext;import com.micmiu.jaxws.demo.HelloService;import com.sun.net.httpserver.HttpExchange;import com.sun.xml.internal.ws.developer.JAXWSProperties;/** * blog http://www.micmiu.com * * @author Michael */@WebService()publicclassHelloServiceImplimplementsHelloService{@ResourceprivateWebServiceContext wsContext;@WebMethodpublicStringsayHello(@WebParam(name="userName")StringuserName){System.out.println(" ----> 获取客户端信息开始 <---- ");getClientInfo();System.out.println(" ----> 获取客户端信息结束 <---- ");return"hi,"+userName+" welcom to www.micmiu.com";}privatevoidgetClientInfo(){try{MessageContext mc=wsContext.getMessageContext();HttpExchange exchange=(HttpExchange)mc.get(JAXWSProperties.HTTP_EXCHANGE);InetSocketAddress isa=exchange.getRemoteAddress();System.out.println("InetSocketAddress : "+isa);System.out.println("Hostname : "+isa.getAddress().getHostAddress()+" address: "+isa.getAddress().getHostName());}catch(Exceptione){e.printStackTrace();}}}|
客户端调用后，服务端运行日志：

``
|12345|publishwebservicesuccessful----&gt;获取客户端信息开始&lt;----InetSocketAddress:/127.0.0.1:61333Hostname:127.0.0.1address:demo.micmiu.com----&gt;获取客户端信息结束&lt;----|
从上面的日志信息中可看出：服务端完全可以获取到客户端的IP地址。
2. 情况二：如果以web容器的方式发布（jetty 或 tomcat为例）：
服务端接口实现 HelloServiceImpl.java 修改成如下：

```
```
|123456789101112131415161718192021222324252627282930313233343536373839404142434445|packagecom.micmiu.jaxws.demo2.impl;import javax.annotation.Resource;import javax.jws.WebService;import javax.servlet.http.HttpServletRequest;import javax.xml.ws.WebServiceContext;import javax.xml.ws.handler.MessageContext;import com.micmiu.jaxws.demo2.HelloService;/** * blog http://www.micmiu.com * * @author Michael */@WebService(endpointInterface="com.micmiu.jaxws.demo2.HelloService")publicclassHelloServiceImplimplementsHelloService{@ResourceprivateWebServiceContext wsContext;publicStringsayHello(StringuserName){System.out.println(" ----> 获取客户端信息开始 <---- ");StringclientIP=getClientInfo();System.out.println(" ----> 获取客户端信息结束 <---- ");return"Hi,"+userName+" welcome to JAX-WS with IP: "+clientIP+" . see more http://www.micmiu.com ";}privateStringgetClientInfo(){StringclientIP=null;try{MessageContext mc=wsContext.getMessageContext();HttpServletRequest request=(HttpServletRequest)(mc.get(MessageContext.SERVLET_REQUEST));clientIP=request.getRemoteAddr();System.out.println("client IP : "+clientIP);}catch(Exceptione){e.printStackTrace();}returnclientIP;}}|
客户端代码不用修改，运行如下：

``
|1234|startwebserviceclient...sendMichaeltoserverHi,MichaelwelcometoJAX-WSwithIP:127.0.0.1.seemorehttp://www.micmiu.comtestclientend.|
服务端运行日志如下：


```
```
|12345678|2012-8-619:15:24com.sun.xml.ws.transport.http.servlet.WSServletContextListener contextInitialized信息:WSSERVLET12:JAX-WS上下文监听程序正在初始化2012-8-619:15:25com.sun.xml.ws.transport.http.servlet.WSServletDelegate&lt;init&gt;信息:WSSERVLET14:JAX-WS servlet正在初始化2012-08-0619:15:25.645:INFO::Started SelectChannelConnector@0.0.0.0:8080----&gt;获取客户端信息开始&lt;----client IP:127.0.0.1----&gt;获取客户端信息结束&lt;----|
从上面的日志信息中可看出：服务端完全可以获取到客户端的IP地址。
[二]、基于XFire开发的webservice获取客户端IP地址
以：[http://www.micmiu.com/soa/webservice/xfire-ws-base-demo/](http://www.micmiu.com/soa/webservice/xfire-ws-base-demo/) 的示例为基础：
服务端接口实现类：HelloServiceImpl.java 修改成如下：

``
|1234567891011121314151617181920212223242526272829303132|packagecom.micmiu.xfire.demo.base;importjavax.servlet.http.HttpServletRequest;importorg.codehaus.xfire.transport.http.XFireServletController;/*** @blog http://www.micmiu.com* @author Michael*/publicclassHelloWorldServiceImplimplementsHelloWorldService{publicStringsayHello(Stringusername){System.out.println(" ----> 获取客户端信息  <---- ");StringclientIP=getClientInfo();System.out.println("客户端IP ："+clientIP);return"Hi,"+username+" welcome,see more http://www.micmiu.com";}publicStringgetClientInfo(){StringclientIP=null;try{HttpServletRequestrequest=XFireServletController.getRequest();System.out.println("Addr : "+request.getRemoteAddr()+" host: "+request.getRemoteHost());clientIP=request.getRemoteAddr();}catch(Exceptione){e.printStackTrace();}returnclientIP;}}|
客户端调用后，服务端的日志信息如下：

```
```
|123|----&gt;获取客户端信息&lt;----Addr:127.0.0.1host:127.0.0.1客户端IP：127.0.0.1|
从上面的日志信息中可看出：服务端完全可以获取到客户端的IP地址。
[三]、Axis开发的webservice获取客户端IP地址
以Axis最简单的部署方式为例：
服务端代码：HelloWorld.java

``
|1234567891011121314151617181920212223242526272829303132333435|importjavax.servlet.http.HttpServletRequest;importorg.apache.axis.MessageContext;importorg.apache.axis.transport.http.HTTPConstants;/**** @author <a href="http://www.micmiu.com">Michael Sun</a>*/publicclassHelloWorld{publicStringsayHello(Stringusername){System.out.println(" ----> Get client info  <---- ");StringclientIP=getClientInfo();System.out.println("client ip"+clientIP);return"Hi,"+username+" welcome to axis with IP: "+clientIP+" . see more http://www.micmiu.com ";}privateStringgetClientInfo(){MessageContextmc=null;HttpServletRequestrequest=null;StringclientIP=null;try{mc=MessageContext.getCurrentContext();request=(HttpServletRequest)mc.getProperty(HTTPConstants.MC_HTTP_SERVLETREQUEST);clientIP=request.getRemoteAddr();}catch(Exceptione){e.printStackTrace();}returnclientIP;}}|
客户端调用代码：Client4Hello.java

```
```
|123456789101112131415161718192021222324252627282930|import org.apache.axis.client.Call;import org.apache.axis.client.Service;/** * * @author <a href="http://www.micmiu.com">Michael Sun</a> */publicclassClient4Hello{/**	 * @param args	 */publicstaticvoidmain(String[]args){StringwsdlUrl="http://localhost:8088/axis/HelloWorld.jws";try{System.out.println(" ----> 客户端调用测试  <---- ");Services=newService();Call call=(Call)s.createCall();call.setOperationName("sayHello");// 这里是要调用的方法名call.setTargetEndpointAddress(wsdlUrl);// 设置调用的wsdlStringret=(String)call.invoke(newObject[]{"Michael"});System.out.println("发送 Michael ,返回的信息 :"+ret);}catch(Exceptione){e.printStackTrace();}}}|
运行客户端结果：

``
|1234|----&gt;客户端调用测试&lt;----2012-8-616:56:13org.apache.axis.utils.JavaUtilsisAttachmentSupported。。。。发送Michael,返回的信息:Hi,MichaelwelcometoaxiswithIP:127.0.0.1.seemorehttp://www.micmiu.com|
从tomcat的日志文件catalina.out 中看到服务端运行信息：

```
```
|12|----&gt;Getclientinfo&lt;----clientip127.0.0.1|
从上面的日志信息中可看出：服务端完全可以获取到客户端的IP地址。
到此演示了JAX-WS、XFire、Axis三种webservice的获取客户端IP的简单实现过程。
