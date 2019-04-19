# Hessian实现远程调用 - 零度的博客专栏 - CSDN博客
2016年09月29日 10:03:26[零度anngle](https://me.csdn.net/zmx729618)阅读数：4680
       Hessian是一个轻量级的remoting on http工具,采用的是Binary RPC协议，所以它很适合于发送二进制数据,同时又具有防火墙穿透能力。Hessian一般是通过Web应用来提供服务，因此非常类似于平时我们用的WebService。只是它不使用SOAP协议,但相比webservice而言更简单、快捷。
Hessian官网：http://hessian.caucho.com/
Hessian可通过Servlet提供远程服务，需要将匹配某个模式的请求映射到Hessian服务。也可Spring框架整合，通过它的DispatcherServlet可以完成该功能，DispatcherServlet可将匹配模式的请求转发到Hessian服务。Hessian的server端提供一个servlet基类, 用来处理发送的请求，而Hessian的这个远程过程调用，完全使用动态代理来实现的,，建议采用面向接口编程，Hessian服务通过接口暴露。
Hessian处理过程示意图：
客户端——>序列化写到输出流——>远程方法(服务器端)——>序列化写到输出流 ——>客户端读取输入流——>输出结果
下面详细介绍最常用的两种方式实现Hessian提供webservice：
纯Hessian实现
配合Spring框架实现
在开始之前当然需要到官网上下载相关的lib包，放入项目的/WEB-INF/lib/下
【一】、纯Hessian实现步骤：
1.基本代码
首先编写一个服务的接口类：HelloHessian.java
Java代码
- package michael.hessian;     
- import java.util.List;     
- import java.util.Map;     
- 
- /**   
-  * @author michael   
-  *    
-  */
- publicinterface HelloHessian {     
- 
-     String sayHello();     
- 
-     MyCar getMyCar();     
- 
-     List myLoveFruit();     
- 
-     Map myBabays();     
- 
- }   
一个java bean文件MyCar.java:
Java代码
- package michael.hessian;     
- 
- import java.io.Serializable;     
- 
- /**   
-  * @author michael   
-  *    
-  */
- publicclass MyCar implements Serializable {     
- 
- /**   
-      *    
-      */
- privatestaticfinallong serialVersionUID = 4736905401908455439L;     
- 
- private String carName;     
- 
- private String carModel;     
- 
- /**   
-      * @return the carName   
-      */
- public String getCarName() {     
- return carName;     
-     }     
- 
- /**   
-      * @return the carModel   
-      */
- public String getCarModel() {     
- return carModel;     
-     }     
- 
- /**   
-      * @param pCarName the carName to set   
-      */
- publicvoid setCarName(String pCarName) {     
-         carName = pCarName;     
-     }     
- 
- /**   
-      * @param pCarModel the carModel to set   
-      */
- publicvoid setCarModel(String pCarModel) {     
-         carModel = pCarModel;     
-     }     
- 
- /**   
-      * @see java.lang.Object#toString()   
-      * @return   
-      */
- @Override
- public String toString() {     
- return"my car name:[" + this.carName + "] model:[" + this.carModel     
-                 + "].";     
-     }     
- 
- }   
服务端接口的实现类：HelloHessianImpl.java
Java代码
- package michael.hessian.impl;     
- 
- import java.util.ArrayList;     
- import java.util.HashMap;     
- import java.util.List;     
- import java.util.Map;     
- 
- import michael.hessian.HelloHessian;     
- import michael.hessian.MyCar;     
- 
- /**   
-  * @author michael   
-  *    
-  */
- publicclass HelloHessianImpl extends HessianServlet implements HelloHessian {     
- 
- public MyCar getMyCar() {     
-         MyCar car = new MyCar();     
-         car.setCarName("阿斯顿·马丁");     
-         car.setCarModel("One-77");     
- return car;     
-     }     
- 
- public Map myBabays() {     
-         Map map = new HashMap();     
-         map.put("son", "孙吴空");     
-         map.put("daughter", "孙小美");     
- return map;     
-     }     
- 
- public List myLoveFruit() {     
-         List list = new ArrayList();     
-         list.add("apple");     
-         list.add("kiwi");     
-         list.add("orange");     
- return list;     
-     }     
- 
- public String sayHello() {     
- return"welcom to Hessian";     
-     }     
- 
- }   
2.配置文件web.xml修改
在web.xml配置文件里增加如下信息：
Xml代码
- <servlet>
- <servlet-name>HelloHessian</servlet-name>
- <servlet-class>
-             com.caucho.hessian.server.HessianServlet     
-         </servlet-class>
- <init-param>
- <param-name>home-class<param-name>
- <param-value>michael.hessian.impl.HelloHessianImpl<param-value>
-        < /init-param>
- <init-param>
- <param-name>home-api<param-name>
- <param-value>michael.hessian.HelloHessian<param-value>
-         </init-param>
- <load-on-startup>1</load-on-startup>
-     </servlet>
- <servlet-mapping>
- <servlet-name>HelloHessian</servlet-name>
- <url-pattern>/HessianService</url-pattern>
- </servlet-mapping>
3.java客户端验证
Java代码
- package michael.hessian.client;     
- 
- import java.net.MalformedURLException;     
- import java.util.Map;     
- 
- import michael.hessian.HelloHessian;     
- import michael.hessian.MyCar;     
- 
- import com.caucho.hessian.client.HessianProxyFactory;     
- 
- /**   
-  * @author michael   
-  *   
-  */
- publicclass HessianClientTest {     
- 
- /**   
-      * @param args   
-      */
- publicstaticvoid main(String[] args) {     
-         String url = "http://localhost:8082/J2EE_sjsky/HessianService";     
-         HessianProxyFactory factory = new HessianProxyFactory();     
- try {     
-             HelloHessian hello = (HelloHessian) factory.create(     
-                     HelloHessian.class, url);     
-             System.out.println(hello.sayHello());     
- 
-             MyCar car = hello.getMyCar();     
-             System.out.println(car.toString());     
- 
- for (Map.Entry entry : hello.myBabays().entrySet()) {     
-                 System.out.println(entry.getKey() + "   " + entry.getValue());     
-             }     
- 
- for (String str : hello.myLoveFruit()) {     
-                 System.out.println(str);     
-             }     
- 
-         } catch (MalformedURLException e) {     
-             e.printStackTrace();     
-         }     
- 
-     }     
- 
- }    
运行结果如下：
Java代码
- welcom to Hessian     
- my car name:[阿斯顿·马丁] model:[One-77].     
- daughter   孙小美     
- son   孙吴空     
- apple     
- kiwi     
- orange   
【二】、spring+hessian 实现服务端：
1.基本代码见上面的demo
2.修改配置文件
在web.xml，增加内容如下：
- <context-param> 
-         <param-name>contextConfigLocation</param-name> 
-         <param-value> 
-                        /WEB-INF/hessian-servlet.xml 
-          </param-value> 
- </context-param> 
- <servlet> 
-              <servlet-name>hessian</servlet-name> 
-              <servlet-class>
-                          org.springframework.web.servlet.DispatcherServlet
-              </servlet-class> 
-             <load-on-startup>4</load-on-startup> 
- </servlet> 
-  <servlet-mapping> 
-             <servlet-name>hessian</servlet-name> 
-             <url-pattern>﻿﻿/hessian/*</url-pattern> 
- </servlet-mapping>
   hessian-servlet.xml如下：
- <beans> 
-          <bean id="defaultHandlerMapping" class="org.springframework.web.servlet.handler.BeanNameUrlHandlerMapping"/> 
-          <bean id="helloService" class="lavasoft.suths.service.HelloService"/> 
-          <bean name="/helloHessianService" class="org.springframework.remoting.caucho.HessianServiceExporter"> 
-               <property name="service" ref="helloHessianImpl"/> 
-               <property name="serviceInterface" value="michael.hessian.HelloHessian"/> 
-         </bean> 
- </beans>  
3.java客户端验证
和上面的例子相似，只需要把访问的url替换成新的即可如下：
Java代码:
- String url=http://localhost:8082/J2EE_sjsky/springhessian/helloHessianService   
运行结果和上面例子一样。
4.spring配置客户端
增加一个spring的bean配置文件hessian-client.xml
Xml代码
- <xmlversion="1.0"encoding="UTF-8"?>
- <beans>
- <beanid="helloHessianClient"
- class="org.springframework.remoting.caucho.HessianProxyFactoryBean">
- <propertyname="serviceUrl">
- <value>
- [http://localhost:8082/J2EE_sjsky/hessian/helloHessianService](http://localhost:8082/J2EE_sjsky/hessian/helloHessianService)
- </value>
-        </ property>
- <propertyname="serviceInterface"
- value="michael.hessian.HelloHessian"/>
-      </bean>
- </beans>
测试代码HessianSpringClient.java：
Java代码
- package michael.hessian.client;     
- 
- import java.util.Map;     
- 
- import michael.hessian.HelloHessian;     
- import michael.hessian.MyCar;     
- 
- import org.springframework.context.ApplicationContext;     
- import org.springframework.context.support.ClassPathXmlApplicationContext;     
- 
- /**   
-  * @author michael   
-  *    
-  */
- publicclass HessianSpringClient {     
- 
- /**   
-      * @param args   
-      */
- publicstaticvoid main(String[] args) {     
- try {     
-             ApplicationContext context = new ClassPathXmlApplicationContext(     
- "hessian-client.xml");     
- 
-             HelloHessian hello = (HelloHessian) context     
-                     .getBean("helloHessianClient");     
- 
-             System.out.println(hello.sayHello());     
- 
-             MyCar car = hello.getMyCar();     
-             System.out.println(car.toString());     
- 
- for (Map.Entry entry : hello.myBabays().entrySet()) {     
-                 System.out.println(entry.getKey() + "   " + entry.getValue());     
-             }     
- 
- for (String str : hello.myLoveFruit()) {     
-                 System.out.println(str);     
-             }     
-         } catch (Exception e) {     
-             e.printStackTrace();     
-         }     
- 
-     }     
- 
- }   
运行结果和上面完全一致。
5.com.caucho.hessian.io.HessianProtocolException: expected string at 0x6d异常处理
我在spring+hessian整合测试过程中，客户端调用时，发生了异常，而服务端错误内容如下：
Java代码
- 2011-4-2516:14:44 org.apache.catalina.core.StandardWrapperValve invoke     
- 严重: Servlet.service() for servlet remoting threw exception     
- com.caucho.hessian.io.HessianProtocolException: expected string at 0x6d
-     at com.caucho.hessian.io.Hessian2Input.error(Hessian2Input.java:2882)     
-     at com.caucho.hessian.io.Hessian2Input.expect(Hessian2Input.java:2830)     
-     at com.caucho.hessian.io.Hessian2Input.readString(Hessian2Input.java:1362)     
-     at com.caucho.hessian.io.Hessian2Input.readMethod(Hessian2Input.java:272)     
-     at com.caucho.hessian.server.HessianSkeleton.invoke(HessianSkeleton.java:249)     
-     at com.caucho.hessian.server.HessianSkeleton.invoke(HessianSkeleton.java:221)     
-     at org.springframework.remoting.caucho.Hessian2SkeletonInvoker.invoke(Hessian2SkeletonInvoker.java:67)     
-     at org.springframework.remoting.caucho.HessianServiceExporter.handleRequest(HessianServiceExporter.java:147)     
-     at org.springframework.web.servlet.mvc.HttpRequestHandlerAdapter.handle(HttpRequestHandlerAdapter.java:49)     
-     at org.springframework.web.servlet.DispatcherServlet.doDispatch(DispatcherServlet.java:819)     
-     at org.springframework.web.servlet.DispatcherServlet.doService(DispatcherServlet.java:754)     
-     at org.springframework.web.servlet.FrameworkServlet.processRequest(FrameworkServlet.java:399)     
-     at org.springframework.web.servlet.FrameworkServlet.doPost(FrameworkServlet.java:364)     
-     at javax.servlet.http.HttpServlet.service(HttpServlet.java:710)     
-     at javax.servlet.http.HttpServlet.service(HttpServlet.java:803)     
-     at org.apache.catalina.core.ApplicationFilterChain.internalDoFilter(ApplicationFilterChain.java:290)     
-     at org.apache.catalina.core.ApplicationFilterChain.doFilter(ApplicationFilterChain.java:206)     
-     at org.apache.catalina.core.StandardWrapperValve.invoke(StandardWrapperValve.java:230)     
-     at org.apache.catalina.core.StandardContextValve.invoke(StandardContextValve.java:175)     
-     at org.apache.catalina.core.StandardHostValve.invoke(StandardHostValve.java:128)     
-     at org.apache.catalina.valves.ErrorReportValve.invoke(ErrorReportValve.java:104)     
-     at org.apache.catalina.core.StandardEngineValve.invoke(StandardEngineValve.java:109)     
-     at org.apache.catalina.connector.CoyoteAdapter.service(CoyoteAdapter.java:261)     
-     at org.apache.coyote.http11.Http11Processor.process(Http11Processor.java:844)     
-     at org.apache.coyote.http11.Http11Protocol$Http11ConnectionHandler.process(Http11Protocol.java:581)     
-     at org.apache.tomcat.util.net.JIoEndpoint$Worker.run(JIoEndpoint.java:447)     
-     at java.lang.Thread.run(Thread.java:619)   
经查资料发现原始是hessian和spring的版本不兼容引起的，重新下载hessian-3.1.6.jar导入项目，一起运行正常，测试完全通过。
