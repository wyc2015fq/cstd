# WebService CXF学习（进阶篇3）：CXF整合Spring框架 - z69183787的专栏 - CSDN博客
2014年06月30日 11:25:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1459
 通过前面两节的讲解，相信你对CXF框架开始有一些认识了。在当今项目开发中，Spring框架基上都用到过，那么它怎么与CXF结合呢，这就是我们这一间要讲的内容。好了，闲话少说。 
    首先，在前面基础上再导入几个spring要用到的几个.jar包：
               1、spring-asm.jar
               2、spring-beans.jar
               3、spring-context.jar
               4、spring-core.jar
               5、spring-expression.jar
               6、spring-aop.jar
               7、spring-web.jar
第一步：新建一个服务端web project,导入要用到的cxf和spring的.jar包，修改web.xml。配置如下
**[html]**[view
 plain](http://blog.csdn.net/vickychen89/article/details/6606571#)[copy](http://blog.csdn.net/vickychen89/article/details/6606571#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <web-appversion="2.4"
- xmlns="http://java.sun.com/xml/ns/j2ee"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://java.sun.com/xml/ns/j2ee   
-     http://java.sun.com/xml/ns/j2ee/web-app_2_4.xsd">
- <!-- Spring 容器加载的配置文件 设置 -->
- <context-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>
-             classpath:/applicationContext-server.xml  
- </param-value>
- </context-param>
- 
- <!-- Spring 配置 -->
- <listener>
- <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
- </listener>
- <listener>
- <listener-class>org.springframework.web.util.IntrospectorCleanupListener</listener-class>
- </listener>
- 
- <!-- WebServices设置 -->
- <servlet>
- <servlet-name>CXFServices</servlet-name>
- <servlet-class>org.apache.cxf.transport.servlet.CXFServlet</servlet-class>
- <load-on-startup>0</load-on-startup>
- </servlet>
- <servlet-mapping>
- <servlet-name>CXFServices</servlet-name>
- <url-pattern>/services/*</url-pattern>
- </servlet-mapping>
- 
- <welcome-file-list>
- <welcome-file>index.jsp</welcome-file>
- </welcome-file-list>
- </web-app>
第二步：新建一个接口类和接口实现类
**[java]**[view
 plain](http://blog.csdn.net/vickychen89/article/details/6606571#)[copy](http://blog.csdn.net/vickychen89/article/details/6606571#)
- package com.ms.services;  
- 
- import java.util.List;  
- import javax.jws.WebService;  
- import com.ms.model.UserInfo;  
- 
- @WebService
- publicinterface IHelloServices {  
- public String sayHello(String name);  
- public String sayHelloToAll(List<UserInfo> users);  
- }  
**[java]**[view
 plain](http://blog.csdn.net/vickychen89/article/details/6606571#)[copy](http://blog.csdn.net/vickychen89/article/details/6606571#)
- package com.ms.services.impl;  
- 
- import java.util.List;  
- import javax.jws.WebService;  
- import com.ms.model.UserInfo;  
- import com.ms.services.IHelloServices;  
- 
- @WebService(endpointInterface="com.ms.services.IHelloServices")  
- publicclass HelloServicesImpl implements IHelloServices {  
- 
- public String sayHello(String name) {  
- return"Hello "+name+" .";  
-     }  
- public String sayHelloToAll(List<UserInfo> users) {  
-         String hello = "hello ";  
- for(UserInfo user:users){  
-             hello += user.getUserName()+" ,";  
-         }  
-         hello += " ,everybody.";  
- return hello;  
-     }  
- }  
第三步：新建一个spring Bean的xml文件，配置CXF webservices的服务
**[html]**[view
 plain](http://blog.csdn.net/vickychen89/article/details/6606571#)[copy](http://blog.csdn.net/vickychen89/article/details/6606571#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xmlns:jaxws="http://cxf.apache.org/jaxws"
- xsi:schemaLocation="    
-             http://www.springframework.org/schema/beans     
-             http://www.springframework.org/schema/beans/spring-beans-3.0.xsd    
-             http://cxf.apache.org/jaxws    
-             http://cxf.apache.org/schemas/jaxws.xsd">
- <!-- Import apache CXF bean definition 固定-->
- <importresource="classpath:META-INF/cxf/cxf.xml"/>
- <importresource="classpath:META-INF/cxf/cxf-extension-soap.xml"/>
- <importresource="classpath:META-INF/cxf/cxf-servlet.xml"/>
- 
- <!-- services接口配置 -->
- <beanid="helloServicesBean"class="com.ms.services.impl.HelloServicesImpl"/>
- <!-- CXF 配置WebServices的服务名及访问地址 -->
- <jaxws:serverid="helloServices"address="/HelloServices"
- serviceClass="com.ms.services.IHelloServices">
- <jaxws:serviceBean>
- <refbean="helloServicesBean"/>
- </jaxws:serviceBean>
- </jaxws:server>
- </beans>
第四步：将工程部署到Tomcat中运行，在IE中输入"[**http://localhost:8090/CxfServer_Spring/services**](http://localhost:8090/CxfServer_Spring/services)",测试服务是否发布成功
第五步：新建一个客户端web project,导入要用到的cxf和spring的.jar包
第六步：将服务端的接口类及JavaBean对象类copy到客户端工程中，且路径要与服务端路径一致
第七步：新建一个spring Bean的xml配置文件，配置CXF webservices的客户端
**[html]**[view
 plain](http://blog.csdn.net/vickychen89/article/details/6606571#)[copy](http://blog.csdn.net/vickychen89/article/details/6606571#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xmlns:jaxws="http://cxf.apache.org/jaxws"
- xsi:schemaLocation="    
-             http://www.springframework.org/schema/beans     
-             http://www.springframework.org/schema/beans/spring-beans-3.0.xsd    
-             http://cxf.apache.org/jaxws    
-             http://cxf.apache.org/schemas/jaxws.xsd">
- <!-- Import apache CXF bean definition -->
- <importresource="classpath:META-INF/cxf/cxf.xml"/>
- <importresource="classpath:META-INF/cxf/cxf-extension-soap.xml"/>
- <importresource="classpath:META-INF/cxf/cxf-servlet.xml"/>
- 
- <!-- CXF webservices 客户端配置 -->
- <jaxws:clientid="helloClient"serviceClass="com.ms.services.IHelloServices"
- address="http://localhost:8090/CxfServer_Spring/services/HelloServices">
- </jaxws:client>
- </beans>
第八步：新建一个测试类进行测试，代码如下
**[java]**[view
 plain](http://blog.csdn.net/vickychen89/article/details/6606571#)[copy](http://blog.csdn.net/vickychen89/article/details/6606571#)
- package com.test;  
- 
- import java.util.ArrayList;  
- import java.util.List;  
- import org.apache.cxf.jaxws.JaxWsProxyFactoryBean;  
- import org.springframework.context.ApplicationContext;  
- import org.springframework.context.support.ClassPathXmlApplicationContext;  
- import com.ms.model.UserInfo;  
- import com.ms.services.IHelloServices;  
- 
- publicclass Client {  
- publicstaticvoid main(String[] args) {  
-         invokeBySpring();  
-     }  
- 
- /**
-      * 通过Spring测试webservices
-      */
- publicstaticvoid invokeBySpring(){  
-         ApplicationContext context = new ClassPathXmlApplicationContext("applicationContext-client.xml");  
-         IHelloServices helloServices = context.getBean("helloClient",IHelloServices.class);  
- 
-         List<UserInfo> users = new ArrayList<UserInfo>();  
-         users.add(new UserInfo("vicky",23));  
-         users.add(new UserInfo("caty",23));  
-         users.add(new UserInfo("ivy",23));  
-         users.add(new UserInfo("kelly",23));  
-         String helloAll = helloServices.sayHelloToAll(users);  
- 
-         System.out.println(helloAll);  
-     }  
- 
- publicstaticvoid invoke(){  
- //创建WebService客户端代理工厂   
-         JaxWsProxyFactoryBean factory = new JaxWsProxyFactoryBean();     
- //注册WebService接口   
-         factory.setServiceClass(IHelloServices.class);     
- //设置WebService地址   
-         factory.setAddress("http://localhost:8090/CxfServer_Spring/services/HelloServices");          
-         IHelloServices helloServices = (IHelloServices)factory.create();     
-         System.out.println("invoke helloServices webservice...");  
-         String hello = helloServices.sayHello("vicky");  
- 
-         System.out.println(hello);  
-     }  
- }  
- 
