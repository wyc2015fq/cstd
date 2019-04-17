# Spring MVC+Maven+Velocity的配置过程 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月13日 00:22:46[boonya](https://me.csdn.net/boonya)阅读数：736








# 1. 简介

本文通过一个简单的实例展示Spring MVC+Maven+Velocity的配置过程，该配置的重点在于以下几个方面：
-  创建Maven工程

- Maven依赖包配置
- 配置 web.xml文件
- Spring配置文件

- 编写程序验证配置


# 2.构建步骤

要构建一个如上图所示的工程，分以下几步实现：

## 2.1 创建Maven（webapp）工程

![](https://img-blog.csdn.net/20170212193635329)

最终我们的项目看起来是这样的：

![](https://img-blog.csdn.net/20170212205902014)


## 2.2 配置pom.xml依赖

可以在Maven官网：http://search.maven.org/ 进行搜索需要的依赖配置。下面是具体的依赖配置：

pom.xml



```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
	<modelVersion>4.0.0</modelVersion>
	<groupId>com.boonya.spring.mvc.velocity</groupId>
	<artifactId>SpringVelocity</artifactId>
	<version>0.0.1-SNAPSHOT</version>
	<packaging>war</packaging>
	<url>http://maven.apache.org</url>

	<dependencies>
		<dependency>
		    <groupId>javax.servlet</groupId>
		    <artifactId>javax.servlet-api</artifactId>
		    <version>3.1.0</version>
		</dependency>

		<!-- Spring Framework -->
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-web</artifactId>
			<version>3.2.5.RELEASE</version>
		</dependency>
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-webmvc</artifactId>
			<version>3.2.5.RELEASE</version>
		</dependency>
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-core</artifactId>
			<version>3.2.5.RELEASE</version>
		</dependency>

		<!-- velocity -->
		<dependency>
		    <groupId>org.apache.velocity</groupId>
		    <artifactId>velocity</artifactId>
		    <version>1.7</version>
		</dependency>
		<dependency>
			<groupId>com.googlecode.sli4j</groupId>
			<artifactId>sli4j-slf4j-log4j</artifactId>
			<version>2.0</version>
		</dependency>
		<dependency>
			<groupId>junit</groupId>
			<artifactId>junit</artifactId>
			<version>4.11</version>
			<scope>test</scope>
		</dependency>
		<dependency>
			<groupId>net.sf.ehcache</groupId>
			<artifactId>ehcache-core</artifactId>
			<version>2.6.5</version>
		</dependency>
		<dependency>
			<groupId>org.codehaus.jackson</groupId>
			<artifactId>jackson-mapper-asl</artifactId>
			<version>1.9.13</version>
		</dependency>
		<dependency>
			<groupId>net.sf.json-lib</groupId>
			<artifactId>json-lib</artifactId>
			<version>2.4</version>
			<classifier>jdk15</classifier>
		</dependency>
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-core</artifactId>
			<version>3.2.5.RELEASE</version>
		</dependency>
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-context-support</artifactId>
			<version>3.2.4.RELEASE</version>
			<type>jar</type>
			<scope>compile</scope>
		</dependency>
		<dependency>
			<groupId>org.mybatis</groupId>
			<artifactId>mybatis-spring</artifactId>
			<version>1.2.1</version>
		</dependency>
		<dependency>
			<groupId>org.mybatis</groupId>
			<artifactId>mybatis-ehcache</artifactId>
			<version>1.0.0</version>
		</dependency>
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-jdbc</artifactId>
			<version>3.2.5.RELEASE</version>
		</dependency>
        <!-- 阿里巴巴数据源包 -->
        <dependency>
            <groupId>com.alibaba</groupId>
            <artifactId>druid</artifactId>
            <version>1.0.2</version>
        </dependency>
		<dependency>
			<groupId>org.apache.poi</groupId>
			<artifactId>poi</artifactId>
			<version>3.8</version>
		</dependency>
		<!-- <dependency>
			<groupId>google</groupId>
			<artifactId>google-connections</artifactId>
			<version>1.0</version>
		</dependency> -->
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-webmvc</artifactId>
			<version>3.2.5.RELEASE</version>
		</dependency>
		<dependency>
			<groupId>commons-httpclient</groupId>
			<artifactId>commons-httpclient</artifactId>
			<version>3.1</version>
		</dependency>
		<!-- cxf.jar -->
		<dependency>
			<groupId>org.apache.cxf</groupId>
			<artifactId>cxf-rt-frontend-jaxws</artifactId>
			<version>2.7.6</version>
		</dependency>
	<!-- 	<dependency>
			<groupId>org.apache</groupId>
			<artifactId>neethi</artifactId>
			<version>3.0.2</version>
		</dependency> -->
		<dependency>
			<groupId>wsdl4j</groupId>
			<artifactId>wsdl4j</artifactId>
			<version>1.6.3</version>
			<type>jar</type>
			<scope>compile</scope>
		</dependency>
	<!-- 	<dependency>
			<groupId>org.apache.ws</groupId>
			<artifactId>commons.schema</artifactId>
			<version>2.0.3</version>
		</dependency>
		<dependency>
			<groupId>com.ctc</groupId>
			<artifactId>wstx</artifactId>
			<version>4.2.0</version>
		</dependency>
		<dependency>
			<groupId>org.codehaus</groupId>
			<artifactId>stax2</artifactId>
			<version>3.1.1</version>
		</dependency> -->
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-beans</artifactId>
			<version>3.2.5.RELEASE</version>
		</dependency>
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-test</artifactId>
			<version>3.2.5.RELEASE</version>
			<scope>test</scope>
		</dependency>
		<dependency>
			<groupId>org.aspectj</groupId>
			<artifactId>aspectjweaver</artifactId>
			<version>1.7.4</version>
		</dependency>
        <!-- mysql驱动包 -->
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <version>5.1.12</version>
        </dependency>
		<!-- <dependency>
			<groupId>com.oracle</groupId>
			<artifactId>ojdbc14</artifactId>
			<version>10.2.0.1.0</version>
		</dependency> -->
		<dependency>
			<groupId>org.mybatis</groupId>
			<artifactId>mybatis</artifactId>
			<version>3.1.1</version>
		</dependency>
		<dependency>
			<groupId>javax</groupId>
			<artifactId>javaee-api</artifactId>
			<version>6.0</version>
			<scope>provided</scope>
			<!--  <scope>compile</scope>-->
		</dependency>

		<dependency>
			<groupId>javax.mail</groupId>
			<artifactId>mail</artifactId>
			<version>1.4.7</version>
		</dependency>
		<dependency>
			<groupId>javax.servlet</groupId>
			<artifactId>jstl</artifactId>
			<version>1.2</version>
		</dependency>
		<dependency>
			<groupId>commons-fileupload</groupId>
			<artifactId>commons-fileupload</artifactId>
			<version>1.3</version>
		</dependency>
		<dependency>
			<groupId>org.apache.clerezza.ext</groupId>
			<artifactId>org.json.simple</artifactId>
			<version>0.3-incubating</version>
			<scope>compile</scope>
		</dependency>
		<dependency>
			<groupId>com.alibaba</groupId>
			<artifactId>fastjson</artifactId>
			<version>1.1.41</version>
		</dependency>
		<dependency>
			<groupId>dom4j</groupId>
			<artifactId>dom4j</artifactId>
			<version>1.6</version>
		</dependency>
		<!--二维码-->
		<dependency> 
			<groupId>com.google.zxing</groupId>
			<artifactId>core</artifactId>
			<version>3.0.1</version>
		</dependency>
		<!--缩略图-->
		<dependency>
			<groupId>net.coobird</groupId> 
			<artifactId>thumbnailator</artifactId>
			<version>0.4.8</version>
		</dependency>

		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-orm</artifactId>
			<version>3.2.5.RELEASE</version>
		</dependency>
		 <dependency>
			<groupId>org.unitils</groupId>
			<artifactId>unitils-core</artifactId>
			<version>3.1</version>
			<scope>test</scope>
		</dependency>
		<dependency>
			<groupId>org.unitils</groupId>
			<artifactId>unitils-database</artifactId>
			<version>3.1</version>
			<scope>test</scope>
		</dependency> 
		<dependency>
			<groupId>org.unitils</groupId>
			<artifactId>unitils-dbmaintainer</artifactId>
			<version>3.1</version>
			<scope>test</scope>
		</dependency>
		<dependency>
			<groupId>org.unitils</groupId>
			<artifactId>unitils-dbunit</artifactId>
			<version>3.1</version>
			<scope>test</scope>
		</dependency>
		<dependency>
			<groupId>org.unitils</groupId>
			<artifactId>unitils-spring</artifactId>
			<version>3.1</version>
			<scope>test</scope>
		</dependency>
		<dependency>
			<groupId>org.unitils</groupId>
			<artifactId>unitils-orm</artifactId>
			<version>3.1</version>
			<scope>test</scope>
		</dependency> 
		<dependency>
            <groupId>org.quartz-scheduler</groupId>
            <artifactId>quartz</artifactId>
            <version>1.8.5</version>
        </dependency>  
	</dependencies>
	<build>
		<pluginManagement>
			<plugins>

				<plugin>
					<groupId>org.apache.maven.plugins</groupId>
					<artifactId>maven-compiler-plugin</artifactId>
					<version>2.3.2</version>
					<configuration>
						<source>1.7</source>
						<target>1.7</target>
					</configuration>
				</plugin>

				<plugin>
					<groupId>org.mybatis.generator</groupId>
					<artifactId>mybatis-generator-maven-plugin</artifactId>
					<version>1.3.1</version>
					<configuration>
						<overwrite>false</overwrite>
					</configuration>
				</plugin>

				<plugin>
					<groupId>org.apache.tomcat.maven</groupId>
					<artifactId>tomcat7-maven-plugin</artifactId>
					<version>2.1</version>
					<configuration>
						<uriEncoding>utf-8</uriEncoding>
						<port>8888</port>
						<path>/</path>
						<contextReloadable>false</contextReloadable>
						<useTestClasspath>false</useTestClasspath>
					</configuration>
				</plugin>

				<plugin>
					<groupId>org.codehaus.mojo</groupId>
					<artifactId>tomcat-maven-plugin</artifactId>
					<version>1.1</version>
					<configuration>
						<url>http://192.168.200.121:8080/manager/text</url>
						<username>tomcat</username>
						<password>password</password>
						<path>/SpringVelocity</path>
					</configuration>
				</plugin>

			</plugins>
		</pluginManagement>
		<resources>
			<resource>
				<directory>src/main/resources</directory>
				<includes>
					<include>**/*.properties</include>
					<include>**/*.xml</include>
				</includes>
			</resource>
			<resource>
				<directory>src/main/java</directory>
				<excludes>
					<exclude>**/*.java</exclude>
					<exclude>**/.svn/*</exclude>
				</excludes>
			</resource>
		</resources>
	</build>
</project>
```

注：有些没用的依赖可以去掉。



## 2.3web.xml文件配置

web.xml




```
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns="http://java.sun.com/xml/ns/javaee"
    xmlns:web="http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd" xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd"
    id="WebApp_ID" version="3.0">
    <display-name>Archetype Created Web Application</display-name>

    <listener>
        <description>spring</description>
        <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
    </listener>

    <!-- spring mvc servlet -->
    <servlet>
        <servlet-name>springServlet</servlet-name>
        <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
        <init-param>
            <description>spring mvc</description>
            <param-name>contextConfigLocation</param-name>          
            <param-value>/WEB-INF/spring-servlet.xml</param-value> <!-- spring-servlet 配置路径 -->
        </init-param>
    </servlet>

    <servlet-mapping>
        <servlet-name>springServlet</servlet-name>
        <url-pattern>*.html</url-pattern> <!-- 以html结尾的url将会交给springServlet来处理  -->
    </servlet-mapping>
</web-app>
```




## 2.4Spring文件配置

**Spring通用配置：**applicationContext.xml




```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:aop="http://www.springframework.org/schema/aop"
	xmlns:context="http://www.springframework.org/schema/context"
	xmlns:mvc="http://www.springframework.org/schema/mvc" xmlns:tx="http://www.springframework.org/schema/tx"
	xmlns:jdbc="http://www.springframework.org/schema/jdbc"

	xsi:schemaLocation="
		http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.0.xsd
		http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-3.0.xsd
		http://www.springframework.org/schema/jdbc http://www.springframework.org/schema/jdbc/spring-jdbc-3.0.xsd
		http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-3.0.xsd
		http://www.springframework.org/schema/mvc http://www.springframework.org/schema/mvc/spring-mvc-3.0.xsd
		http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-3.0.xsd">

    
    
	</beans>
```


由于数据库什么的都没用到，所以去掉了多余的配置，如果要配置完整的可以参考src/resources/main/applicationContext.txt.



**SpringMVC的配置：**spring-servlet.xml




```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:p="http://www.springframework.org/schema/p"
	xmlns:aop="http://www.springframework.org/schema/aop"
	xmlns:context="http://www.springframework.org/schema/context"
	xmlns:mvc="http://www.springframework.org/schema/mvc"
	xsi:schemaLocation="http://www.springframework.org/schema/beans
		http://www.springframework.org/schema/beans/spring-beans-3.0.xsd
		http://www.springframework.org/schema/context 
		http://www.springframework.org/schema/context/spring-context-3.0.xsd
		http://www.springframework.org/schema/mvc 
		http://www.springframework.org/schema/mvc/spring-mvc-3.0.xsd
		http://www.springframework.org/schema/aop
		http://www.springframework.org/schema/aop/spring-aop.xsd">
		
    <!-- 激活@Controller模式 -->
    <mvc:annotation-driven />

    <!-- spring可以自动去扫描base-package下面或者子包下面的java文件-->
    <!--如果扫描到有@Component @Controller@Service等这些注解的类，则把这些类注册为bean -->
    <context:component-scan base-package="com.boonya.spring.mvc.velocity.controller" />

    <!-- for velocity view -->
    <bean id="velocityConfig" class="org.springframework.web.servlet.view.velocity.VelocityConfigurer">
        <property name="resourceLoaderPath" value="/WEB-INF/templates/" /> <!-- velocity前端渲染文件.vm路径设置-->
    </bean>
    <bean id="vmViewResolver" class="org.springframework.web.servlet.view.velocity.VelocityViewResolver">
        <property name="cache" value="true" />
        <property name="prefix" value="" />
        <property name="suffix" value=".vm" />
        <property name="order" value="1" /> <!-- 多ViewResovler配置 -->
    </bean>
</beans>
```



## 2.5视图控制层Controller代码

SpringVelocityController.java




```java
package com.boonya.spring.mvc.velocity.controller;

import java.util.ArrayList;
import java.util.List;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

@Controller
public class SpringVelocityController  {
  
  @RequestMapping("/velocity")
  public ModelAndView velocityView(HttpServletRequest req, HttpServletResponse resp) throws Exception {
      System.out.println("------------ enter SpringVelocityController");

      List<String> list = new ArrayList<String>();
      list.add("a");
      list.add("b");
      list.add("c");
      return new ModelAndView("velocity", "list", list);// velocity:即SpringVelocity/src/main/webapp/WEB-INF/templates/velocity.vm
  }

}
```




# 3.测试项目

## 3.1测试配置正确性

我们加一个空的JSP页面作为首页：index.jsp



```
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title>SpringVelocity首页</title>
</head>
<body>

<h3>欢迎来到，SpringVelocity!</h3>

</body>
</html>
```
启动tomcat访问：http://localhost:8080/SpringVelocity/ 看到以下输出说明配置可以正常启动。



![](https://img-blog.csdn.net/20170212211013035)

## 3.2测试Velocity模板输出

在浏览器输入：http://localhost:8080/SpringVelocity/velocity.html 如果看到如下输出则配置成功。

![](https://img-blog.csdn.net/20170212211201801)




还是那句话，实践出真知。文章有借鉴阿里实习小朋友的（文章上面有误）：[http://blog.csdn.net/sun_wangdong/article/details/51992194](http://blog.csdn.net/sun_wangdong/article/details/51992194)


## 3.3工程代码下载

[http://download.csdn.net/detail/boonya/9752942](http://download.csdn.net/detail/boonya/9752942)













