# CXF+Spring+Tomcat简明示例 - z69183787的专栏 - CSDN博客
2014年05月05日 10:06:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：646
个人分类：[WebService](https://blog.csdn.net/z69183787/article/category/2175701)
多系统（异构系统）进行交互时，一种良好的方式便是调用Web Service，本示例基于Apache组织的CXF，为了方便起见特将服务端和客户端写在同一个工程下，实际项目中是不可能的，但是客户端却依赖于服务端的Web Service接口，那么可以通过导出jar的方式。
环境：
MyEclipse10
JDK6
Tomcat7
CXF2.5
Spring3
示例项目结构图：
![](http://www.blogjava.net/images/blogjava_net/icewee/cxf/2012-07-06_164811.png)
如上图所示，全部依赖的第三方库都在lib中,**另需要asm jar包**，下面贴出全部代码。
IHelloService.java
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)package bing.server;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)import javax.jws.WebService;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)/**
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif) * <p>
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif) * WebService接口
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif) * </p>
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif) * 
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif) * @author IceWee
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif) * @date 2012-7-6
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif) * @version 1.0
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)*/
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)@WebService
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)publicinterface IHelloService {
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)public String sayHello(String username);
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
HelloServiceImpl.java
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)package bing.server;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)import javax.jws.WebService;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)/**
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif) * <p>
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif) * WebService实现类
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif) * </p>
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif) * 
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif) * @author IceWee
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif) * @date 2012-7-6
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif) * @version 1.0
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)*/
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)@WebService(endpointInterface = "bing.server.IHelloService", serviceName = "HelloService")
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)publicclass HelloServiceImpl implements IHelloService {
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)    @Override
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)public String sayHello(String username) {
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)return "hello, " + username;
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
HelloServiceClient.java
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)package bing.client;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)import org.springframework.context.ApplicationContext;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)import org.springframework.context.support.ClassPathXmlApplicationContext;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)import bing.server.IHelloService;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)/**
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif) * <p>
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif) * WebService调用方-客户端
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif) * </p>
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif) * 
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif) * @author IceWee
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif) * @date 2012-7-6
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif) * @version 1.0
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)*/
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)publicclass HelloServiceClient {
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)publicstaticvoid main(String[]
 args) {
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)        ApplicationContext context = new ClassPathXmlApplicationContext("applicationContext-client.xml");
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)        IHelloService helloService = (IHelloService) context.getBean("client");
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)        String response = helloService.sayHello("Peter");
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)        System.out.println(response);
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
applicationContext-server.xml
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<?xml version="1.0" encoding="UTF-8"?>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<beans xmlns="http://www.springframework.org/schema/beans"
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)    xmlns:jaxws="http://cxf.apache.org/jaxws"
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)    xsi:schemaLocation="http://www.springframework.org/schema/beans
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)                       http://www.springframework.org/schema/beans/spring-beans.xsd
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)                       http://cxf.apache.org/jaxws http://cxf.apache.org/schemas/jaxws.xsd">
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<!--
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)        ***注意*** 
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)        手动添加的内容：
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)        xmlns:jaxws="http://cxf.apache.org/jaxws"
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)        http://cxf.apache.org/jaxws http://cxf.apache.org/schemas/jaxws.xsd"
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)-->
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<import resource="classpath:META-INF/cxf/cxf.xml"/>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<import resource="classpath:META-INF/cxf/cxf-extension-soap.xml"/>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<import resource="classpath:META-INF/cxf/cxf-servlet.xml"/>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<jaxws:endpoint id="helloService" implementor="bing.server.HelloServiceImpl" address="/helloService"/>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)</beans>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
applicationContext-client.xml
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<?xml version="1.0" encoding="UTF-8"?>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<beans xmlns="http://www.springframework.org/schema/beans"
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)    xmlns:jaxws="http://cxf.apache.org/jaxws"
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)    xsi:schemaLocation="http://www.springframework.org/schema/beans
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)                       http://www.springframework.org/schema/beans/spring-beans.xsd
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)                       http://cxf.apache.org/jaxws http://cxf.apache.org/schemas/jaxws.xsd">
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<!--
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)        ***注意*** 
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)        手动添加的内容：
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)        xmlns:jaxws="http://cxf.apache.org/jaxws"
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)        http://cxf.apache.org/jaxws http://cxf.apache.org/schemas/jaxws.xsd"
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)-->
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<import resource="classpath:META-INF/cxf/cxf.xml"/>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<import resource="classpath:META-INF/cxf/cxf-extension-soap.xml"/>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<import resource="classpath:META-INF/cxf/cxf-servlet.xml"/>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<bean id="client" class="bing.server.IHelloService" factory-bean="clientFactory" factory-method="create"/>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<bean id="clientFactory" class="org.apache.cxf.jaxws.JaxWsProxyFactoryBean">
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<property name="serviceClass" value="bing.server.IHelloService"/>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<property name="address" value="http://localhost:8080/CXFDemo/ws/helloService"/>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)</bean>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)</beans>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
web.xml
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<?xml version="1.0" encoding="UTF-8"?>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<web-app version="3.0"
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)    xmlns="http://java.sun.com/xml/ns/javaee"
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)    xsi:schemaLocation="http://java.sun.com/xml/ns/javaee 
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)    http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd">
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<display-name>CXFDemo</display-name>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<context-param>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<param-name>contextConfigLocation</param-name>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<param-value>classpath:applicationContext-server.xml</param-value>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)</context-param>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<listener>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)</listener>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<servlet>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<servlet-name>CXFServlet</servlet-name>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<display-name>CXFServlet</display-name>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<servlet-class>org.apache.cxf.transport.servlet.CXFServlet</servlet-class>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<load-on-startup>1</load-on-startup>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)</servlet>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<servlet-mapping>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<servlet-name>CXFServlet</servlet-name>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<url-pattern>/ws/*</url-pattern>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)</servlet-mapping>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<welcome-file-list>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)<welcome-file>index.jsp</welcome-file>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)</welcome-file-list>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)</web-app>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
所有项目都已配置完成，可以发布到Tomcat了，在浏览器中输入：[http://localhost:8080/CXFDemo/ws](http://localhost:8080/CXFDemo/ws)，返回如图：
![](http://www.blogjava.net/images/blogjava_net/icewee/cxf/2012-07-06_170337.png)
从上图中可以看到我们对外发布的WebService接口，点击蓝色超链接，返回如图：
![](http://www.blogjava.net/images/blogjava_net/icewee/cxf/2012-07-06_170411.png)
到此，证明我们的Web Service已经发布成功，可以进行调用测试了。运行HelloServiceClient，返回如图：
![](http://www.blogjava.net/images/blogjava_net/icewee/cxf/2012-07-06_170754.png)
全文完！

