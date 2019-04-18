# eclipse 中  spring security +springmvc入门 （自定义登录页面） - 苝花向暖丨楠枝向寒 - CSDN博客

2018年06月24日 18:56:39[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：712


上网上看了很多个教程，基本上都不太全面，而且经常是xml的头文件报错。自己研究好久，终于弄出来了，下面分享给大家。

如果你想看idea版的 spring security +springmvc入门 

请看我下一篇文章  [点击打开链接](https://blog.csdn.net/weixin_40247263/article/details/80793668)


首先做一下思路梳理，spring security的作用主要就是一个拦截器，它会拦截指定的请求，并判断你是否登录了账号，如没有则会自动跳转到登录页面，这个登录页面是spring security内置的，特别简陋，项目中肯定不会使用，但我们可以自己配置登录页面。同时我们需要在配置文件中 配置账号，并且给这个账号一定的权限，这样我们通过这个账号登录成功以后就会跳转到指定的页面。

原理就是 我们 请求 某个 requestMapping，然后它给我们拦截，做判断。有权限就放行，会继续请求requestMapping，没权限，就重定向到指定的页面。


项目结构如下

![](https://img-blog.csdn.net/20180624182417390?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

pom.xml

```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.neuedu</groupId>
  <artifactId>springsecurity</artifactId>
  <version>0.0.1-SNAPSHOT</version>
  <packaging>war</packaging>

	<properties>
		<jdk.version>1.6</jdk.version>
		<spring.version>4.1.6.RELEASE</spring.version>
		<spring.security.version>3.2.3.RELEASE</spring.security.version>
		<jstl.version>1.2</jstl.version>
	</properties>

	<dependencies>

		<!-- Spring dependencies -->
		<dependency>
		    <groupId>org.springframework</groupId>
		    <artifactId>spring-context</artifactId>
		    <version>${spring.version}</version>
		</dependency>
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-webmvc</artifactId>
			<version>${spring.version}</version>
		</dependency>

		<!-- Spring Security -->
		<dependency>
			<groupId>org.springframework.security</groupId>
			<artifactId>spring-security-web</artifactId>
			<version>${spring.security.version}</version>
		</dependency>

		<dependency>
			<groupId>org.springframework.security</groupId>
			<artifactId>spring-security-config</artifactId>
			<version>${spring.security.version}</version>
		</dependency>

		<!-- jstl for jsp page -->
		<dependency>
			<groupId>jstl</groupId>
			<artifactId>jstl</artifactId>
			<version>${jstl.version}</version>
		</dependency>

	</dependencies>

</project>
```

web.xml

```
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns="http://java.sun.com/xml/ns/javaee" xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd" id="WebApp_ID" version="3.0">
	<!-- Spring MVC -->
	<servlet>
		<servlet-name>mvc-dispatcher</servlet-name>
		<servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
		<init-param>
			<param-name>namespace</param-name>
			<param-value>springmvc</param-value>
		</init-param>
		<load-on-startup>1</load-on-startup>
	</servlet>
	<servlet-mapping>
		<servlet-name>mvc-dispatcher</servlet-name>
		<url-pattern>/</url-pattern>
	</servlet-mapping>

	<listener>
		<listener-class>org.springframework.web.context.ContextLoaderListener
		</listener-class>
	</listener>

        <!-- Loads Spring Security config file -->
	<context-param>
		<param-name>contextConfigLocation</param-name>
		<param-value>
			/WEB-INF/spring-security.xml
		</param-value>
	</context-param>

	<!-- Spring Security -->
	<filter>
		<filter-name>springSecurityFilterChain</filter-name>
		<filter-class>org.springframework.web.filter.DelegatingFilterProxy
		</filter-class>
	</filter>

	<filter-mapping>
		<filter-name>springSecurityFilterChain</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
</web-app>
```

springmvc.xml

```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
	xmlns:p="http://www.springframework.org/schema/p"
	xmlns:context="http://www.springframework.org/schema/context"
	xmlns:util="http://www.springframework.org/schema/util" 
	xmlns:mvc="http://www.springframework.org/schema/mvc"
	xmlns:tx="http://www.springframework.org/schema/tx"
	xmlns:aop="http://www.springframework.org/schema/aop"
	xsi:schemaLocation="http://www.springframework.org/schema/beans
       http://www.springframework.org/schema/beans/spring-beans-4.0.xsd
       http://www.springframework.org/schema/context 
       http://www.springframework.org/schema/context/spring-context-4.0.xsd
       http://www.springframework.org/schema/util 
       http://www.springframework.org/schema/util/spring-util-4.0.xsd 
       http://www.springframework.org/schema/mvc 
       http://www.springframework.org/schema/mvc/spring-mvc-4.0.xsd
       http://www.springframework.org/schema/tx
       http://www.springframework.org/schema/tx/spring-tx-4.0.xsd
        http://www.springframework.org/schema/aop
       http://www.springframework.org/schema/aop/spring-aop-4.0.xsd
">

	<context:component-scan base-package="controller"></context:component-scan>
	<bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">
		<property name="viewClass" value="org.springframework.web.servlet.view.JstlView"></property>
		<property name="prefix" value="/WEB-INF/pages/"></property>
		<property name="suffix" value=".jsp"></property>
	</bean>
</beans>
```
spring-security.xml

```
<?xml version="1.0" encoding="UTF-8"?>  
<beans xmlns="http://www.springframework.org/schema/beans"  
    xmlns:sec="http://www.springframework.org/schema/security"  
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  
    xsi:schemaLocation="http://www.springframework.org/schema/beans  
        http://www.springframework.org/schema/beans/spring-beans-4.0.xsd  
        http://www.springframework.org/schema/security  
        http://www.springframework.org/schema/security/spring-security-3.2.xsd">  

	<!-- 不拦截的页面，我下面这行在我这个案例中没意义，可以设置一些静态资源 或 下面配置拦截所有请求的时候，选择不拦截登录页面 -->
    <sec:http pattern="/login.jsp" security="none"></sec:http>  
    
	<sec:http auto-config="false">
		<!-- 对/admin** 的url进行拦截，只有拥有ROLE_USER权限的用户才能进入 -->
		<sec:intercept-url pattern="/admin**" access="ROLE_USER" />
		<!-- 
			login-page="/login.jsp" 拦截以后重定向到自定义的登录页面
			authentication-failure-url="/login.jsp"登录的用户没有 ROLE_USER权限跳转的页面
			default-target-url="/admin"  登录的用户有  ROLE_USER权限跳转的页面
		-->
		<sec:form-login login-page="/login.jsp" authentication-failure-url="/login.jsp"  
        default-target-url="/admin" />  
        <!-- 
        		点击注销  跳转的页面
       			与 <a href="<c:url value="/logout" />" > 注销</a> 配合使用
         -->
	    <sec:logout logout-url="/logout" logout-success-url="/login.jsp" invalidate-session="true"/>
	</sec:http>

	<sec:authentication-manager>
	  <sec:authentication-provider>
	    <sec:user-service>
	    <!-- 给root 用户 配置 "ROLE_USER" 权限 -->
		<sec:user name="root" password="123456" authorities="ROLE_USER" />
	    </sec:user-service>
	  </sec:authentication-provider>
	</sec:authentication-manager>

</beans>
```

admin.jsp

```java
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8" session="true"%>
<%@taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
</head>
<body>
	<h1>标题: ${title}</h1>
	<h1>消息 : ${message}</h1>

	<c:if test="${pageContext.request.userPrincipal.name != null}">
	   <h2>欢迎: ${pageContext.request.userPrincipal.name} 
           | <a href="<c:url value="/logout" />" > 注销</a></h2>  
	</c:if>
</body>
</html>
```

超链接 地址/logout   与 spring-security.xml 中的sec:layout标签对应

hello.jsp

```java
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8" session="false"%>

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
</head>
<body>
	<h1>标题: ${title}</h1>	
	<h1>消息 : ${message}</h1>	
</body>
</html>
```

index.jsp

```
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
</head>
<body>
<h1>index</h1>
</body>
</html>
```

login.jsp

```
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
</head>
<body>
       <form action="j_spring_security_check" method="post">

                <input type="text" name="j_username"
                       placeholder=用户名 maxlength="11"  style="border-radius: 4px; outline:none;
                           ;width: 300px;height: 35px;text-indent: 10px"/><br/>
                <input type="password" name="j_password"
                       placeholder=密码 maxlength="11"  style="border-radius: 4px; outline:none;
                           ;width: 300px;height: 35px;text-indent: 10px"/><br/>           

                <input type="submit" 
                       style="width: 305px;height: 45px;background-color: orange" value="登录"
                       onmousemove="this.style.backgroundColor='darkorange';"
                       onmouseout="this.style.backgroundColor='orange   ';"/>

        </form>
</body>
</html>
```

我们自定义的登录页面

以下内容 是固定的。

请求地址：j_spring_security_check

method ：post

用户名input框的 name属性 ：j_username

密码input框的 name属性 ：j_password

原因 ： 我们的表单请求是提交给 spring security 的。所以我们必须和它写的 pojo中的 变量名一直。以及 请求必须提交到指定的 requestMapping

HelloController.java

```java
package controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

@Controller
public class HelloController {

	@RequestMapping(value = { "/", "/welcome**" }, method = RequestMethod.GET)
	public ModelAndView welcomePage() {

		ModelAndView model = new ModelAndView();
		model.addObject("title", "Spring Security Hello World");
		model.addObject("message", "This is welcome page!");
		model.setViewName("hello");
		return model;

	}

	@RequestMapping(value = "/admin**", method = RequestMethod.GET)
	public ModelAndView adminPage() {

		ModelAndView model = new ModelAndView();
		model.addObject("title", "Spring Security Hello World");
		model.addObject("message", "This is protected page!");
		model.setViewName("admin");

		return model;

	}

}
```


