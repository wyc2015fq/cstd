# JAX-WS开发的webservice如何在Tomcat下发布 - z69183787的专栏 - CSDN博客
2015年07月14日 21:48:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2484
目录：
- 
构建项目
- 
服务端编码
- 
创建：sun-jaxws.xml
- 
配置：web.xml
- 
添加依赖库
- 
发布并验证
[一]、构建项目


```
```
|1|mvn archetype:create-DgroupId=com.micmiu.jaxws.web-DartifactId=jaxws-web-demo-DarchetypeArtifactId=maven-archetype-webapp-DinteractiveMode=false|
转为 Eclipse web 项目：mvn eclipse:eclipse -Dwtpversion=1.0 导入Eclipse项目
创建配置相应的源目录、编译目录，目录结构如下：

``
|12345678910111213141516171819202122232425262728293031323334353637383940|D:\workspace_sun\jaxws-web-demo&gt;tree/F卷work的文件夹PATH列表卷序列号为2AF7-9BD9D:.│.classpath│.project│pom.xml│├─.settings│.component│org.eclipse.jdt.core.prefs│org.eclipse.wst.common.project.facet.core.xml│org.maven.ide.eclipse.prefs│├─src│├─main││├─java│││└─com│││└─micmiu│││└─jaxws│││└─demo2│││└─impl│││││├─resources││└─webapp│││index.jsp│││││└─WEB-INF││web.xml│││└─test│└─java└─target├─classes│└─com│└─micmiu│└─jaxws│└─demo2│└─test-classes|
[二]、服务端编码
编码服务接口：HelloService.java

```
```
|12345678910111213141516171819|packagecom.micmiu.jaxws.demo2;import javax.jws.WebMethod;import javax.jws.WebParam;import javax.jws.WebService;import javax.jws.soap.SOAPBinding;import javax.jws.soap.SOAPBinding.Style;import javax.jws.soap.SOAPBinding.Use;/** * * @author <a href="http://www.micmiu.com">Michael</a> */@WebService(serviceName="HelloWorldService")@SOAPBinding(style=Style.DOCUMENT,use=Use.LITERAL)publicinterfaceHelloService{@WebMethodStringsayHello(@WebParam(name="userName")StringuserName);}|
服务接口的实现类：HelloServiceImpl.java

``
|1234567891011121314151617181920|packagecom.micmiu.jaxws.demo2.impl;importjavax.jws.WebService;importcom.micmiu.jaxws.demo2.HelloService;/*** blog http://www.micmiu.com** @author Michael*/@WebService(endpointInterface="com.micmiu.jaxws.demo2.HelloService")publicclassHelloServiceImplimplementsHelloService{publicStringsayHello(StringuserName){return"Hi,"+userName+" welcome to JAX-WS see more http://www.micmiu.com ";}}|
[三]、创建：sun-jaxws.xml
在webapp/WEB-INF/ 下创建文件：sun-jaxws.xml

XHTML
```
```
|123456|<?xml version="1.0"encoding="UTF-8"?><endpoints xmlns="http://java.sun.com/xml/ns/jax-ws/ri/runtime"version="2.0"><endpoint name="HelloWorldService"implementation="com.micmiu.jaxws.demo2.impl.HelloServiceImpl"url-pattern="/helloworld" /></endpoints>|
[四]、 配置：web.xml
修改web.xml 文件如下：

XHTML
``
|123456789101112131415161718192021222324|<!DOCTYPE web-app PUBLIC"-//Sun Microsystems, Inc.//DTD Web Application 2.3//EN""http://java.sun.com/dtd/web-app_2_3.dtd" ><web-app><display-name>JAX-WS+Web@ www.micmiu.com</display-name><listener><listener-class>com.sun.xml.ws.transport.http.servlet.WSServletContextListener</listener-class></listener><servlet><servlet-name>helloworld</servlet-name><servlet-class>com.sun.xml.ws.transport.http.servlet.WSServlet</servlet-class><load-on-startup>1</load-on-startup></servlet><servlet-mapping><servlet-name>helloworld</servlet-name><url-pattern>/helloworld</url-pattern></servlet-mapping></web-app>|
[五]、添加依赖库
如果是maven构建，只需要在 POM.xml 中添加如下内容：

XHTML
```
```
|1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556|<dependency><groupId>javax.xml.ws</groupId><artifactId>jaxws-api</artifactId><version>2.2.8</version><type>jar</type><scope>compile</scope></dependency><dependency><groupId>com.sun.xml.ws</groupId><artifactId>jaxws-rt</artifactId><version>2.2.6-2</version><type>jar</type><scope>compile</scope></dependency><dependency><groupId>org.glassfish.gmbal</groupId><artifactId>gmbal-api-only</artifactId><version>3.2.0-b003</version><type>jar</type><scope>compile</scope></dependency><dependency><groupId>org.glassfish.external</groupId><artifactId>management-api</artifactId><version>3.1.0-b001</version><type>jar</type><scope>compile</scope></dependency><dependency><groupId>org.jvnet.staxex</groupId><artifactId>stax-ex</artifactId><version>1.7</version><type>jar</type><scope>compile</scope></dependency><dependency><groupId>com.sun.xml.stream.buffer</groupId><artifactId>streambuffer</artifactId><version>1.5</version><type>jar</type><scope>compile</scope></dependency><dependency><groupId>com.sun.xml.ws</groupId><artifactId>policy</artifactId><version>2.3.1</version><type>jar</type><scope>compile</scope></dependency><dependency><groupId>com.sun.xml.bind</groupId><artifactId>jaxb-impl</artifactId><version>2.2.5</version><type>jar</type><scope>compile</scope></dependency>|
或者去官网：[http://jax-ws.java.net/](http://jax-ws.java.net/)  下载相关的lib包。
[六]、验证服务发布
Eclipse中可以直接选择在 tomcat server（或Jetty）下启动项目：

``
|123456|....2012-8-623:39:25com.sun.xml.ws.transport.http.servlet.WSServletContextListenercontextInitialized信息:WSSERVLET12:JAX-WS上下文监听程序正在初始化2012-8-723:39:26com.sun.xml.ws.transport.http.servlet.WSServletDelegate&lt;init&gt;信息:WSSERVLET14:JAX-WSservlet正在初始化....|
在浏览器中输入：http://localhost:8082/jaxws-web-demo/helloworld 
![](http://www.micmiu.com/wp-content/uploads/2012/08/jax-ws-web-01.jpg)
看到类似上述截图的内容，表示服务已经发布成功了。
