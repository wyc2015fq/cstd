# SpringMVC的四种HandlerMapping - z69183787的专栏 - CSDN博客
2016年10月10日 09:54:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2807
各视图解析器分析：[http://blog.csdn.net/z69183787/article/details/40425919](http://blog.csdn.net/z69183787/article/details/40425919)
1:基础配置
　　(1)pom.xml
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.zhao</groupId>
    <artifactId>SpringMVC</artifactId>
    <packaging>war</packaging>
    <version>0.0.1-SNAPSHOT</version>
    <name>SpringMVC Maven Webapp</name>
    <url>http://maven.apache.org</url>
    <dependencies>
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <version>4.12</version>
            <scope>test</scope>
        </dependency>
        <!-- Spring -->
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context</artifactId>
            <version>4.1.7.RELEASE</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-test</artifactId>
            <version>4.1.7.RELEASE</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-core</artifactId>
            <version>4.1.7.RELEASE</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-beans</artifactId>
            <version>4.1.7.RELEASE</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-web</artifactId>
            <version>4.1.7.RELEASE</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-webmvc</artifactId>
            <version>4.1.7.RELEASE</version>
        </dependency>
        <!-- Servlet -->
        <dependency>
            <groupId>javax.servlet</groupId>
            <artifactId>javax.servlet-api</artifactId>
            <version>3.1.0</version>
        </dependency>
    </dependencies>
    <build>
        <finalName>SpringMVC</finalName>
    </build>
</project>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　(2)web.xml
　　注意一下spring dispatcherServlet的参数contextConfigLocation的值，我创建了四个xml文件，每次运行的时候都会指定其中的一个，这样能把spring的几种handlerMapping分的清晰一点。所以需要指定spring-*的值。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns="http://xmlns.jcp.org/xml/ns/javaee" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://xmlns.jcp.org/xml/ns/javaee
                      http://xmlns.jcp.org/xml/ns/javaee/web-app_3_1.xsd"
    version="3.1" metadata-complete="true">
    <!-- DispatcherServlet:SpringMVC核心 -->
    <servlet>
        <servlet-name>spring-dispatcher</servlet-name>
        <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
        <init-param>
            <param-name>contextConfigLocation</param-name>
            <param-value>classpath:spring-XXX.xml</param-value>
        </init-param>
        <load-on-startup>1</load-on-startup>
    </servlet>
    <servlet-mapping>
        <servlet-name>spring-dispatcher</servlet-name>
        <url-pattern>/</url-pattern>
    </servlet-mapping>
    
    <welcome-file-list>
        <welcome-file>index.jsp</welcome-file>
    </welcome-file-list>
</web-app>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　(3)index.jsp
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Index Page</title>
</head>
<body>
    <a href="annotationController">Annotation Controller</a>
    <br/>
    <a href="simpleUrlController">Simple Url Controller</a>
    <br/>
    <a href="beanNameUrlController">BeanName Url Controller</a>
    <br/>
    <a href="controllerclassname">ControllerClassName Controller</a>
</body>
</html>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　(4)home.jsp
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Home Page</title>
</head>
<body>
    Home Page: 测试成功
</body>
</html>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
2:DefaultAnnotationHandlerMapping--》基于注解的方式配置Controller
　　(1)显示的指定web.xml中ContextConfigLocation的值
```
<init-param>
            <param-name>contextConfigLocation</param-name>
            <param-value>classpath:spring-mvc.xml</param-value>
        </init-param>
```
　　(2)spring-mvc.xml
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
    xmlns:context="http://www.springframework.org/schema/context"
    xmlns:mvc="http://www.springframework.org/schema/mvc"
    xsi:schemaLocation="http://www.springframework.org/schema/beans
        http://www.springframework.org/schema/beans/spring-beans.xsd
        http://www.springframework.org/schema/context
        http://www.springframework.org/schema/context/spring-context.xsd
        http://www.springframework.org/schema/mvc
        http://www.springframework.org/schema/mvc/spring-mvc.xsd
        ">
    <context:component-scan base-package="com.zhao.web"/>
    <!-- 1:HandlerMapping -->
    <bean class="org.springframework.web.servlet.mvc.annotation.DefaultAnnotationHandlerMapping"/>
    <!-- 2:HandlerAdapter -->
    <bean class="org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter"/>
    <!-- 3:ViewResolver,视图解析器。Struts2是显示的在struts.xml中指定逻辑视图和物理视图的关联,springMVC采用视图解析器统一配置 -->
    <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">
        <property name="prefix" value="/WEB-INF/view/"/>
        <property name="suffix" value=".jsp"/>
    </bean>
</beans>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
HandlerMapping:处理映射器，为用户发送的请求找到合适的Handler Adapter。
HandlerAdapter:可以简单的理解为调用Controller中的方法。
　　HandlerMapping+HnadlerAdapter：找到url所对应的Controller Method。
ViewResolver:视图解析器,Controller会返回一个view，这个view是String类型的逻辑视图名。我们需要根据这个逻辑视图名找到物理视图。ViewResolver就定义了怎么找物理视图。ViewResolver指定了物理视图的前缀和后缀，这样根据逻辑视图名就能拼出物理视图。比如说Controller返回"home"。那么物理视图就是 /WEB-INF/view/home.jsp。
　　(3)Controller
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package com.zhao.web;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;
import org.springframework.web.servlet.mvc.AbstractController;
@Controller
public class AnnotationController extends AbstractController{
    @Override
    @RequestMapping(value="annotationController")
    protected ModelAndView handleRequestInternal(HttpServletRequest request, HttpServletResponse response)
            throws Exception {
        System.out.println("AnnotationController Working.");
        return new ModelAndView("home");
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
 返回了一个ModelAndView对象，model是数据，类似于map。view就是逻辑视图名，String类型。
注意，使用注解的前提是在spring-mvc.xml开启了注解，比如说包扫描。
<context:component-scan base-package="com.zhao.web"/>
3：SimpleUrlHandlerMapping--》显示的指定url对应关系
　　(1)修改web.xml，把spring-mvc.xml改成spring-url.xml
　　(2)srping-url.xml
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    <!-- 1:SimpleUrlHandlerMapping -->
    <bean class="org.springframework.web.servlet.handler.SimpleUrlHandlerMapping">
        <property name="mappings">
            <props>
                <prop key="simpleUrlController">simleUrl</prop>
            </props>
        </property>
    </bean>
    <!-- 2:ViewResolver,视图解析器。-->
    <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">
        <property name="prefix" value="/WEB-INF/view/"/>
        <property name="suffix" value=".jsp"/>
    </bean>
    <!-- 3:Controller -->
    <bean id="simleUrl" class="com.zhao.web.SimpleUrlController" />
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
ViewResolver是必须有的，无论用哪种方式的HandlerMapping，都需要使用ViewResolver。
不同于注解，SimpleUrlHandlerMapping需要显示的载入Controller。
SimpleUrlHandlerMapping需要配置请求和响应:
<props>
<prop key="simpleUrlController">simleUrl</prop>
</props>
prop的key值就说请求，后面指定处理这个请求的Controller。
　　(3)SimpleUrlController,不再展示，主体内容和注解的Controller类似
4：BeanNameUrlHandlerMapping--》基于BeanName找到Controller
　　(1)修改web.xml  -》 spring-name.xml
　　(2)spring-name.xml
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
   <!-- 1:BeanNameUrlHandlerMapping -->
    <bean class="org.springframework.web.servlet.handler.BeanNameUrlHandlerMapping"/>
    
    <!-- 2:ViewResolver,视图解析器。-->
    <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">
        <property name="prefix" value="/WEB-INF/view/"/>
        <property name="suffix" value=".jsp"/>
    </bean>
    
    <!-- 3:Controller -->
    <bean name="/beanNameUrlController" class="com.zhao.web.BeanNameUrlController"/>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　配置了BeanNameUrlHandlerMapping的bean，用户的请求通过bean的name找到Controller。
注意　 bean的name是/beanNameUrlController。要加 '/'　
　(3)BeanNameUrlController也是一样的。
5:ControllerClassNameHandlerMapping--》基于Controller的Class Name来找到Controller
　　(1)修改web.xml->spring-class.xml
　　(2)spring-class.xml
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    <!-- 1:ControllerClassNameHandlerMapping -->
    <bean class="org.springframework.web.servlet.mvc.support.ControllerClassNameHandlerMapping"/>
    
    <!-- 2:ViewResolver,视图解析器。-->
    <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">
        <property name="prefix" value="/WEB-INF/view/"/>
        <property name="suffix" value=".jsp"/>
    </bean>
    
    <!-- 3:Controller -->
    <bean class="com.zhao.web.ControllerClassNameController"/>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　(3)ControllerClassNameController也是一样的。
6：总结
　　(1)SpringMVC导入jar包,配置web.xml 这些都是基础的操作。
　　(2)四种HandlerMapping:DefaultAnnotationHandlerMapping;SimpleUrlHandlerMapping;BeanNameUrlHandlerMapping;ControllerClassNameHandlerMapping。
　　(3)无论哪种HandlerMapping都需要配置ViewResovler。因为我们都需要根据逻辑视图名找到物理视图，Struts2是显示的配置，在action中一个一个进行配置。而SpringMVC是统一配置。
　　(4)用哪种HandlerMapping就需要导入哪种HandlerMapping的bean。
　　(5)HandlerMapping的本质就是找到Controller。
　　　　DefaultAnnotationHandlerMapping是扫描具体的包下使用了@Controller注解的类。这样能实现请求和Controller的关联。
　　　　剩下的三个，通过url、beanname或者classname都需要显示的配置Controller的bean。
　　　　　　SimpleUrlHandlerMapping:是通过配置mappings的参数，显示的指定url和Controller关联。
　　　　　　BeanNameUrlHandlerMapping：是URL就是Controller这个bean的name。需要规范url.
　　　　　　ControllerClassNameHandlerMapping:是URL就说Controller这个bean的类名去掉Controller后的String。也需要规范url.
