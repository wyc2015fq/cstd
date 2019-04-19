# 【SpringBoot】SpringBoot简介 - Big Smile - CSDN博客
2018年01月14日 21:59:32[王啸tr1912](https://me.csdn.net/tr1912)阅读数：284
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
# **简介**
        Spring Boot是由Pivotal团队提供的全新框架，其设计目的是用来简化新Spring应用的初始搭建以及开发过程。该框架使用了特定的方式来进行配置，从而使开发人员不再需要定义样板化的配置。通过这种方式，Spring Boot致力于在蓬勃发展的快速应用开发领域(rapid application development)成为领导者。                                ——《百度百科》
        说白了就是在原来spring的配置基础上，基于最新的java特性，把配置项写在了java文件中，通过一些程序来动态的控制，实现了简化配置，简化插件引入流程，从而可以让我们构建一个spring项目变得更加的快速。
# 为什么要用
       不知道大佬们有没有用过visualStudio的，创建一个.NET的项目，简直不要很爽，基本都是一键配置了好多东西，有的时候你都不知道，还有加载依赖和一些组件的时候，基本都是自动在往*.conf里面写入一些默认配置项，然后我们可以去修改一些细节，从而实现灵活的配置。但是在我们之前接触的spring项目中，会产生各种配置文件，比如自己写的spring-mvc.properties，config.properties等等，还有在web.xml的各种配，所以我们在初搭建框架的时候就会出现各种各样因为配置文件带来的问题，从而降低了我们的开发效率。举个例子
### 优点：
平时如果我们需要搭建一个spring web项目的时候需要怎么做呢？
1）配置web.xml，加载spring和spring mvc
2）配置数据库连接、配置spring事务
3）配置加载配置文件的读取，开启注解
4）配置日志文件
...
之后需要加载到tomcat等web服务器上才能最后看到效果。
     在微服务当代的今天，我们想要搭建一个服务，实现一个简单的功能的话都需要经历上面的步骤，但是如果用springboot的话，只要访问spring的官网 [http://start.spring.io/](http://start.spring.io/) 里面有个springboot的工程生成工具，然后我们就可以用他来生成一个基本的项目了。然后它只有一个主要的配置文件，就是Application.java，在这里进行相关的框架的一些配置，并且我们打开springboot的包，可以看到里面基本的jar包都已经包含了，只要配置使用方法就好了。
# maven项目引入
依赖：
``<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
	<modelVersion>4.0.0</modelVersion>
	<groupId>cn.itcast.springboot</groupId>
	<artifactId>itcast-springboot</artifactId>
	<version>1.0.0-SNAPSHOT</version>
	<packaging>war</packaging>
	<parent>
		<groupId>org.springframework.boot</groupId>
		<artifactId>spring-boot-starter-parent</artifactId>
		<version>1.5.2.RELEASE</version>
	</parent>
	<dependencies>
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-webmvc</artifactId>
			<version>4.3.7.RELEASE</version>
		</dependency>
		<!-- 连接池 -->
		<dependency>
			<groupId>com.jolbox</groupId>
			<artifactId>bonecp-spring</artifactId>
			<version>0.8.0.RELEASE</version>
		</dependency>
		<dependency>
			<groupId>org.springframework.boot</groupId>
			<artifactId>spring-boot-starter-web</artifactId>
		</dependency>
	</dependencies>
	<build>
		<finalName>${project.artifactId}</finalName>
		<plugins>
			<!-- 资源文件拷贝插件 -->
			<plugin>
				<groupId>org.apache.maven.plugins</groupId>
				<artifactId>maven-resources-plugin</artifactId>
				<configuration>
					<encoding>UTF-8</encoding>
				</configuration>
			</plugin>
			<!-- java编译插件 -->
			<plugin>
				<groupId>org.apache.maven.plugins</groupId>
				<artifactId>maven-compiler-plugin</artifactId>
				<configuration>
					<source>1.7</source>
					<target>1.7</target>
					<encoding>UTF-8</encoding>
				</configuration>
			</plugin>
		</plugins>
		<pluginManagement>
			<plugins>
				<!-- 配置Tomcat插件 -->
				<plugin>
					<groupId>org.apache.tomcat.maven</groupId>
					<artifactId>tomcat7-maven-plugin</artifactId>
					<version>2.2</version>
				</plugin>
			</plugins>
		</pluginManagement>
	</build>
</project>
引入这些依赖一个基本的springboot项目就算是搭建好了（注意如果想用新的版本的springboot，请手动修改版本），关于配置，下篇博客接着讨论。
下篇博客将介绍java配置的新特性，在.java文件中用@configuration注解和@bean来配置一个简单的bean引入，以及springboot的配置方法。
