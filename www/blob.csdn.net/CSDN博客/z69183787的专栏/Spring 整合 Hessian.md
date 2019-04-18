# Spring 整合 Hessian - z69183787的专栏 - CSDN博客
2015年05月22日 15:07:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：670
Spring让Hessian变得不但强大，而且易用，但是易用背后，却有不少陷阱！
这个例子很简单，但实际上的确花费了我超过一小时的时间，排除了种种问题，最后问题终于水落石出。
整合以上篇Hello Hessian为基础，加入Spring框架，进行改进。
一、环境
jdk1.5
[http://labs.xiaonei.com/apache-mirror/tomcat/tomcat-6/v6.0.20/bin/apache-tomcat-6.0.20.zip](http://labs.xiaonei.com/apache-mirror/tomcat/tomcat-6/v6.0.20/bin/apache-tomcat-6.0.20.zip)
[http://s3.amazonaws.com/dist.springframework.org/release/SPR/spring-framework-2.5.6.SEC01-with-dependencies.zip](http://s3.amazonaws.com/dist.springframework.org/release/SPR/spring-framework-2.5.6.SEC01-with-dependencies.zip)
顺便说下，如果不说环境版本，很难保证你的程序在别的版本下能运行。
二、整合
1、写Spring的发布Hessian服务的配置文件
hessian-servlet.xml
<!DOCTYPE beans PUBLIC "-//SPRING//DTD BEAN//EN" "http://www.springframework.org/dtd/spring-beans.dtd"> 
<beans>
<beanid="defaultHandlerMapping"class="org.springframework.web.servlet.handler.BeanNameUrlHandlerMapping"/>
<beanid="helloService"class="lavasoft.suths.service.HelloService"/>
<beanname="/hello"class="org.springframework.remoting.caucho.HessianServiceExporter">
<propertyname="service"ref="helloService"/>
<propertyname="serviceInterface"value="lavasoft.suths.service.Hello"/>
</bean>
</beans>
2、配置web.xml
<?xmlversion="1.0"encoding="UTF-8"?>
<web-appxmlns="http://java.sun.com/xml/ns/javaee"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                 xsi:schemaLocation="http://java.sun.com/xml/ns/javaee 
        http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd" 
version="2.5">
<context-param>
<param-name>contextConfigLocation</param-name>
<param-value>
                        /WEB-INF/hessian-servlet.xml 
</param-value>
</context-param>
<servlet>
<servlet-name>hessian</servlet-name>
<servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
<load-on-startup>4</load-on-startup>
</servlet>
<servlet-mapping>
<servlet-name>hessian</servlet-name>
<url-pattern>/hessian/*</url-pattern>
</servlet-mapping>
</web-app>
陷阱：
a）hessian-servlet.xml的文件名必须以<servlet-name>hessian</servlet-name>名字开头，并且加上-servlet.xml一段，组成完整的文件名。
b）hessian-servlet.xml的文件名格式必须是[servlet-name]-servlet.xml格式，否则出错。
三、部署应用
因为涉及到类加载顺序问题，好用IDEA启动Tomcat测试老失败，不知道为啥！这次不用IDEA嵌入式启动Tomcat了，直接自己部署测试。
部署后，启动界面如下：
![](http://img1.51cto.com/attachment/200908/200908141250240593453.png)
三、写测试
这次测试，可以在上个例子的基础上进行修改来测试，根据上面的配置，那么请求HelloService的URL应该是：[http://localhost:8080/hessianapp/hessian/hello](http://localhost:8080/hessianapp/hessian/hello)
package lavasoft.suths.service.client; 
import com.caucho.hessian.client.HessianProxyFactory; 
import lavasoft.suths.service.Hello; 
import java.net.MalformedURLException; 
/** 
* 客户端调用（会依赖服务接口） 
* 
* @author leizhimin 2009-8-14 12:29:33 
*/
publicclass Client { 
publicstaticvoid main(String[] args) throws MalformedURLException
 { 
                String url = "http://localhost:8080/hessianapp/hessian/hello"; 
                HessianProxyFactory factory = new HessianProxyFactory(); 
                Hello hello = (Hello) factory.create(Hello.class, url); 
                System.out.println(hello.sayHello("Hessian")); 
        } 
}
运行结果：
Hello Hessian! 
Process finished with exit code 0
还有一种测试方法，就是在客户端也使用Spring，需要做个配置remoting-client.xml：
<!DOCTYPE beans PUBLIC "-//SPRING//DTD BEAN//EN" "http://www.springframework.org/dtd/spring-beans.dtd"> 
<beans> 
        <bean id="helloServiceClient"class="org.springframework.remoting.caucho.HessianProxyFactoryBean"> 
                <property name="serviceUrl" value="http://localhost:8080/hessianapp/hessian/hello"/> 
                <property name="serviceInterface" value="lavasoft.suths.service.Hello"/> 
        </bean> 
</beans>
然后写个测试类：
package lavasoft.suths.service.client; 
import lavasoft.suths.service.Hello; 
import org.springframework.context.ApplicationContext; 
import org.springframework.context.support.ClassPathXmlApplicationContext; 
/** 
* Spring整合Hessian，客户端测试 
* 
* @author leizhimin 2009-8-14 15:32:46 
*/
publicclass TestClient { 
publicstaticvoid main(String[] args) { 
try { 
                        ApplicationContext context = new ClassPathXmlApplicationContext("/remoting-client.xml"); 
                        Hello hello = (Hello) context.getBean("helloServiceClient"); 
                        System.out.println(hello.sayHello("Spring Hession")); 
                } 
catch (Exception e) { 
                        e.printStackTrace(); 
                } 
        } 
}
运行结果：
Hello Spring Hession! 
Process finished with exit code 0
陷阱：
实际上，看着代码好好，程序有时候还是不能跑，原因是Hessian的版本问题，这里推荐使用Spring自带的版本，就不会有问题了。
整个工程所以依赖的包：
log4j-1.2.15.jar 
spring-aop.jar 
spring-beans.jar 
spring-context.jar 
spring-context-support.jar 
spring-core.jar 
spring-jdbc.jar 
spring-jms.jar 
spring-orm.jar 
spring-test.jar 
spring-tx.jar 
spring-web.jar 
spring-webmvc.jar 
spring-webmvc-portlet.jar 
spring-webmvc-struts.jar 
hessian-3.1.3.jar 
aopalliance.jar 
commons-logging.jar
