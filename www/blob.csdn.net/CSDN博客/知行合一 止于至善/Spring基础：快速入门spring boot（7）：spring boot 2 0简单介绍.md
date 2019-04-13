
# Spring基础：快速入门spring boot（7）：spring boot 2.0简单介绍 - 知行合一 止于至善 - CSDN博客

2018年10月30日 22:16:58[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：2503所属专栏：[spring快速入门](https://blog.csdn.net/column/details/13907.html)



![在这里插入图片描述](https://img-blog.csdn.net/20161203082310637)
从这篇文章开始以spring boot2为主要版本进行使用介绍。
[
](https://img-blog.csdn.net/20161203082310637)
# Spring boot 2特性
[
](https://img-blog.csdn.net/20161203082310637)spring boot2在如下的部分有所变化和增强，相关特性在后续逐步展开。
[
](https://img-blog.csdn.net/20161203082310637)
## 特性增强
[
](https://img-blog.csdn.net/20161203082310637)基础组件升级：
JDK1.8+
tomcat 8+
Thymeleaf 3
Hibernate 5.2
spring framework 5
Reactive Spring
Functional API
Kotlin支持
Metrics
Security
使用变化
配置属性变化
Gradle插件
Actuator endpoints
依赖条件
以当前GA版本的spring boot 2.0.6为例，其依赖条件如下：
Maven： 3.2+
Gradle：4.x
Servlet容器
支持的servlet容器信息如下：
Tomcat 8.5: servlet3.1
Jetty 9.4:     servlet 3.1
Undertow 1.4: servlet 3.1
pom.xml
按照如下方式设定pom文件
坐标项设定值|
|---|---|
groupIdcom.liumiaocn|
artifactIdspringbootdemo|
version0.0.1-SNAPSHOT|
packagingjar|
pom.xml文件详细如下：
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
	<modelVersion>4.0.0</modelVersion>
	<groupId>com.liumiaocn</groupId>
	<artifactId>springbootdemo</artifactId>
	<version>0.0.1-SNAPSHOT</version>
	<packaging>jar</packaging>
	<name>springbootdemo</name>
	<description>spring boot demo project</description>
	<parent>
		<groupId>org.springframework.boot</groupId>
		<artifactId>spring-boot-starter-parent</artifactId>
		<version>2.0.6.RELEASE</version>
		<relativePath/> <!-- lookup parent from repository -->
	</parent>
	<properties>
		<project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
		<project.reporting.outputEncoding>UTF-8</project.reporting.outputEncoding>
		<java.version>1.8</java.version>
	</properties>
	<dependencies>
		<dependency>
			<groupId>org.springframework.boot</groupId>
			<artifactId>spring-boot-starter-web</artifactId>
		</dependency>
		<dependency>
			<groupId>org.springframework.boot</groupId>
			<artifactId>spring-boot-starter-test</artifactId>
			<scope>test</scope>
		</dependency>
	</dependencies>
	<build>
		<plugins>
			<plugin>
				<groupId>org.springframework.boot</groupId>
				<artifactId>spring-boot-maven-plugin</artifactId>
			</plugin>
		</plugins>
	</build>

</project>SpringbootdemoApplication
liumiaocn:springbootdemo liumiao$ cat src/main/java/com/liumiaocn/springbootdemo/SpringbootdemoApplication.java
package com.liumiaocn.springbootdemo;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.bind.annotation.RequestMapping;
@RestController
@SpringBootApplication
public class SpringbootdemoApplication {
        @RequestMapping("/")
        String home() {
          return "Hello, Spring Boot 2";
        }
	public static void main(String[] args) {
		SpringApplication.run(SpringbootdemoApplication.class, args);
	}
}
liumiaocn:springbootdemo liumiao$RestController和RequestMapping注解都是Spring MVC的注解，用于快速设定路由跳转信息
SpringBootApplication注解用于入口类，也是保证junit测试能够进行的条件之一。
SPRING INITIALIZR
也可以使用SPRING INITIALIZR快速创建spring boot项目，因为以前的文章中已经解释过，此处不再赘述。
访问地址：[https://start.spring.io/](https://start.spring.io/)
编译&构建&运行
编译&构建
编译命令：mvn install
运行
运行命令：java -jar target/springbootdemo-0.0.1-SNAPSHOT.jar
或者使用mvn命令：mvn spring-boot:run
结果确认
liumiaocn:springbootdemo liumiao$ curl http://localhost:8080
Hello, Spring Boot 2liumiaocn:springbootdemo liumiao$

