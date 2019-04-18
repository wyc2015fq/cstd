# 一个java生成与使用WebService的简单例子 - z69183787的专栏 - CSDN博客
2014年08月04日 10:14:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：10023
java用Metro生成WebService，在Tomcat中发布WebService和如何使用WebService
一个简单的例子：
**下载metro,并安装。**（本例使用1.5）
[http://java.net/projects/metro](http://java.net/projects/metro)
下载完metro-1.5.jar的包后，运行java -jar metro-1.5.jar
在metro.jar所在的目录下面会生成一个metro文件夹，里面包含了我们要使用的包等。
导包：如下5个包。
/metro1_5Core/webservices-api.jar    
/metro/webservices-extra.jar
/metro/webservices-extra-api.jar
/metro/webservices-rt.jar
/metro/webservices-tools.jar
（可以将该包复制到Tomcat的servers/lib下面，保证webservice的不出错）
**实体类：User**
**[java]**[view
 plain](http://blog.csdn.net/mylitboy/article/details/6346868#)[copy](http://blog.csdn.net/mylitboy/article/details/6346868#)
- package com.you.me.web.entity;  
- 
- publicclass User {  
- 
- private String userId="";  
- private String userName="";  
- private String nickName="";  
- private String email="";  
- private String password="";  
- private String registTime="";  
- private String statusId="";  
- }  
**写要发布的WebService。**如用户服务类UserService，可以根据用户ID得到用户对象
**[java]**[view
 plain](http://blog.csdn.net/mylitboy/article/details/6346868#)[copy](http://blog.csdn.net/mylitboy/article/details/6346868#)
- package com.you.me.ws;  
- 
- import javax.jws.WebMethod;  
- import javax.jws.WebService;  
- 
- import com.you.me.web.entity.User;  
- 
- /**
-  * @author you.me.2011-4-23
-  *
-  */
- @WebService( serviceName="UserServiceWS",  
-   portName="UserServiceInstance",  
-   name="UserService",       
-   targetNamespace="http://www.mylitboy.com/youme")  
- publicclass UserServiceImpl {  
- @WebMethod
- public User getUserById(String id){  
- returnnew User("1", "abc","abcd","abc@abc.com");  
-  }  
- }  
注： 
 serviceName不写，默认为类名+Service，如UserServiceImplService
 name不写，默认为类名，如UserServiceImpl
 portName不写，默认为类名+Port，如UserServiceImplPort
 targetNamespace不写默认为包名，如（[http://ws.me.you.com](http://ws.me.you.com/)）
**配置：web.xml**在web.xml中添加如下语句。
**[xhtml]**[view
 plain](http://blog.csdn.net/mylitboy/article/details/6346868#)[copy](http://blog.csdn.net/mylitboy/article/details/6346868#)
- <listener>
- <listener-class>com.sun.xml.ws.transport.http.servlet.WSServletContextListener</listener-class>
- </listener>
- 
- <servlet>
- <servlet-name>HelloService</servlet-name>
- <servlet-class>com.sun.xml.ws.transport.http.servlet.WSServlet</servlet-class>
- </servlet>
- <servlet-mapping>
- <servlet-name>HelloService</servlet-name>
- <url-pattern>/ws/*</url-pattern>
- </servlet-mapping>
注：
 url-pattern：为你需要listener监听的访问地址。
**配置sun-jaxws.xml**在WEB-INF中新建sun-jaxws.xml文件。
**[xhtml]**[view
 plain](http://blog.csdn.net/mylitboy/article/details/6346868#)[copy](http://blog.csdn.net/mylitboy/article/details/6346868#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <endpointsversion="2.0"
- xmlns="http://java.sun.com/xml/ns/jax-ws/ri/runtime"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://java.sun.com/xml/ns/jax-ws/ri/runtime http://java.sun.com/xml/ns/jax-ws/ri/runtime/sun-jaxws.xsd ">
- <endpointimplementation="com.you.me.ws.UserServiceImpl"
- url-pattern="/ws/a"name="UserService"></endpoint>
- <endpointimplementation="com.you.me.ws.UserServiceImpl2"
- url-pattern="/ws/ab"name="UserService2"></endpoint>
- </endpoints>
注：
 如果有多个WebService,写多个endpoint即可。
 implementation:WebService类
 url-pattern：访问的地址，要输入web.xml中url-pattern的范围内。
 name：随便写
**然后把工程部署到Tomcat中去。**访问如下地址可以查看相应的WebService信息：
[http://localhost:8116/youme/ws/a](http://localhost:8116/youme/ws/a)
[http://localhost:8116/youme/ws/a?wsdl](http://localhost:8116/youme/ws/a?wsdl)
**使用wsimport生成WebService代码**
然后在cmd中输入：wsimport命令生成webService相关代码。
C:/Users/you.me>wsimport -s d://temp [http://localhost:8116/youme/ws/a?wsdl](http://localhost:8116/youme/ws/a?wsdl)
出现如下语句表示成功。
parsing WSDL...
generating code...
compiling code...
此时在D:/temp下面可以看到一些java类：
D:/temp/com/mylitboy/youme
GetUserById.java
GetUserByIdResponse.java
ObjectFactory.java
User.java
UserService.java
UserServiceWS.java
如此服务器端WebService算是完成了。
下面开始使用生成的WebService。
**我们可以把这些java类打包成一个jar文件。也可以直接复制到其他工程中使用。**
使用方法：
将该文件夹复制到新建的工程中（即你要用WebService的工程）。
/youmews_test/com/mylitboy/youme/GetUserById.java
/youmews_test/com/mylitboy/youme/GetUserByIdResponse.java
......
**写测试代码：**Test.java
**[java]**[view
 plain](http://blog.csdn.net/mylitboy/article/details/6346868#)[copy](http://blog.csdn.net/mylitboy/article/details/6346868#)
- package ss;  
- 
- import com.mylitboy.youme.UserService;  
- import com.mylitboy.youme.UserServiceWS;  
- 
- /**
-  * @author you.me.2011-4-23
-  *
-  */
- publicclass Test {  
- publicstaticvoid main(String[] args) {  
-   UserService us = new UserServiceWS().getUserServiceInstance();  
-   System.out.println(us.getUserById("1"));  
-  }  
- }  
注：
 这里面的UserService 就是前面@WebService里面的name属性值。
 UserServiceWS就是@WebService里面的serviceName属性值。
如此整个过程就完成了。
