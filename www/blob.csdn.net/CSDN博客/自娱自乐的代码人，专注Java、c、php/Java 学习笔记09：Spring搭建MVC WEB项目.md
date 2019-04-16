# Java 学习笔记09：Spring搭建MVC WEB项目 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年11月21日 15:59:53[initphp](https://me.csdn.net/initphp)阅读数：7902
个人分类：[Java学习笔记系列](https://blog.csdn.net/initphp/article/category/1230072)

所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)








- 创建一个web项目

![](https://img-my.csdn.net/uploads/201211/21/1353483708_8869.jpg)


- 假设，我们已经安装完毕Spring所需要的依赖包，以及一些其它的扩展包，以及Jetty容器，ps：Jetty容器安装看上一节文章。

![](https://img-my.csdn.net/uploads/201211/21/1353484000_3200.jpg)

![](https://img-my.csdn.net/uploads/201211/21/1353484011_7238.jpg)


- 运行web项目，必须有web.xml配置文件，web.xml放置在WebContent/WEB-INF/目录下面。


```
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  
    xmlns="http://java.sun.com/xml/ns/javaee" xmlns:web="http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd"  
    xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd"  
    id="WebApp_ID" version="2.5">  
    
     <!-- 配置文件位置，默认为/WEB-INF/applicationContext.xml -->  
	<context-param>  
        <param-name>contextConfigLocation</param-name>  
        <param-value>/WEB-INF/applicationContext.xml</param-value>  
    </context-param> 
    
    <!-- 上下文Spring监听器 --> 
	<listener>  
        <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>  
    </listener>  
	
	<!-- servlet控制跳转,这边默认回去走spring-servlet.xml的xml文件 --> 
	<servlet>
		<servlet-name>spring</servlet-name>
		<servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
		<init-param>  
            <param-name>contextConfigLocation</param-name>  
            <param-value>/WEB-INF/spring-servlet.xml</param-value>  
        </init-param> 
	</servlet>
	<!-- url-pattern 是Spring监听路由过来的方式，然后去寻找匹配的Controller
	例如：
	<url-pattern>/</url-pattern>  一般是  /hello/say/ 这样的URL方式
	<url-pattern>*.htm</url-pattern> 一般是 /hello/say.htm 这样的URL方式
	 --> 
	<servlet-mapping>  
        <servlet-name>spring</servlet-name>  
        <url-pattern>/</url-pattern>  
    </servlet-mapping> 
   
</web-app>
```


- 运行Spring需要有applicationContext.xml这个配置文件，我们也将applicationContext.xml放置在WEB-INF/目录下。


```
<?xml version="1.0" encoding="UTF-8"?>
<beans  
    xmlns="http://www.springframework.org/schema/beans"  
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  
    xmlns:context="http://www.springframework.org/schema/context"
    xmlns:aop="http://www.springframework.org/schema/aop"
    xsi:schemaLocation="http://www.springframework.org/schema/beans 
    http://www.springframework.org/schema/beans/spring-beans-2.0.xsd
    http://www.springframework.org/schema/context
    http://www.springframework.org/schema/context/spring-context-2.5.xsd
    http://www.springframework.org/schema/aop
    http://www.springframework.org/schema/aop/spring-aop-2.5.xsd
    ">  
    <context:annotation-config/>
    <aop:aspectj-autoproxy/> 
</beans>
```


- spring-servlet.xml 配置了具体的Spring需要访问的Controller文件夹目录以及模板的目录和模板的后缀名称。


```
<beans xmlns="http://www.springframework.org/schema/beans"
 xmlns:context="http://www.springframework.org/schema/context"
 xmlns:p="http://www.springframework.org/schema/p"
 xmlns:mvc="http://www.springframework.org/schema/mvc"
 xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
 xsi:schemaLocation="http://www.springframework.org/schema/beans
      http://www.springframework.org/schema/beans/spring-beans-3.0.xsd
      http://www.springframework.org/schema/context
      http://www.springframework.org/schema/context/spring-context.xsd
      http://www.springframework.org/schema/mvc
      http://www.springframework.org/schema/mvc/spring-mvc-3.0.xsd">
     
     <!-- 访问com.mvc.rest包下有@Controller注解的Controller文件 -->  
     <context:component-scan base-package="com.mvc.rest" />  
     <!-- 通过注解，把URL映射到Controller上，该标签默认注册DefaultAnnotationHandlerMapping和AnnotationMethodHandlerAdapter -->  
     <mvc:annotation-driven />
     <!-- 视图配置，配置试图的目录，后缀名等 --> 
	<bean id="viewResolver"  
        class="org.springframework.web.servlet.view.InternalResourceViewResolver">  
        <property name="viewClass"  
            value="org.springframework.web.servlet.view.JstlView" />  
        <property name="prefix" value="/WEB-INF/views/" />  
        <property name="suffix" value=".jsp"></property>  
    </bean>  
</beans>
```


- 我们需要创建一个com.mvc.rest包，在包下面创建一个名为：HelloController.java的文件。这个文件就是MVC的控制器。那么，我们有两个方法，分别为say和yes。url中访问分别是：/hello/say/和/hello/yes/


```java
package com.mvc.rest;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;

//@Controller 是一个标识这个是控制器类的注解标签，如果是控制器类 都需要有这个注解。
@Controller
//@RequestMapping(value="/hello") 会映射到url /hello/则访问HelloController中的Action
@RequestMapping(value="/hello")
public class HelloController {
	
	//@RequestMapping(value="/say") 会映射到url /hello/say则访问HelloController中的Action
	@RequestMapping(value="/say")
	public void say() {
		System.out.print("this is HelloController And say Action \r\n");
		
	}
	
	@RequestMapping(value="/yes")
	public void yes() {}

}
```


- 我们需要在/WEB-INF/目录下创建一个views的目录，然后再创建一个/views/hello/的目录，里面分别是say.jsp和yes.jsp，是模板文件。

![](https://img-my.csdn.net/uploads/201211/21/1353484848_4102.jpg)


- 然后运行配置，通过Jetty容器，运行web程序。

![](https://img-my.csdn.net/uploads/201211/21/1353484922_8997.jpg)


- 运行完毕后，控制台会出现以下信息：

![](https://img-my.csdn.net/uploads/201211/21/1353484980_3921.jpg)


- 最后在URL中分别访问：http://127.0.0.1:8090/hello/say/ 和 http://127.0.0.1:8090/hello/yes/ 成功！

![](https://img-my.csdn.net/uploads/201211/21/1353485078_7903.jpg)
- **详细Spring Controller部分的注解，请阅读：http://zachary-guo.iteye.com/blog/1318597**




