# Maven配置Spring+Hibernate Shiro权限控制项目 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年07月23日 17:38:40[boonya](https://me.csdn.net/boonya)阅读数：9592








     前言：在Eclipse中安装好Maven插件，然后创建一个Sample项目。在Eclipse中检出Shiro的官方示例，地址[http://svn.apache.org/repos/asf/shiro/trunk/samples/](http://svn.apache.org/repos/asf/shiro/trunk/samples/)，参考spring-hibernate项目配置。由于之前对Maven的认识不足所以今天专门配置来了解其工作机制，下面是我按照SVN上配置的示例。

#### 1.创建Maven项目并复制配置文件

如图所示：

![](https://img-blog.csdn.net/20140723171417531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


注：文件的位置与官网示例一致。

#### 2.配置pom.xml文件



```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
	<!-- <parent> <groupId>org.apache.shiro.samples</groupId> <artifactId>shiro-samples</artifactId> 
		<version>1.2.3</version> <relativePath>../pom.xml</relativePath> </parent> -->

	<modelVersion>4.0.0</modelVersion>
	<groupId>com.boonya.shiro</groupId>
	<artifactId>boonyashiro-springhibernate</artifactId>
	<version>0.0.1-SNAPSHOT</version>
	<packaging>war</packaging>

	<!-- Java代码依赖环境包 -->
	<dependencies>
		<dependency>
			<groupId>log4j</groupId>
			<artifactId>log4j</artifactId>
			<version>1.2.17</version>
		</dependency>
		<dependency>
			<groupId>org.slf4j</groupId>
			<artifactId>slf4j-log4j12</artifactId>
			<version>1.6.0</version>
		</dependency>
		<dependency>
			<groupId>org.slf4j</groupId>
			<artifactId>jcl-over-slf4j</artifactId>
			<version>1.6.0</version>
		</dependency>
		<dependency>
			<groupId>jstl</groupId>
			<artifactId>jstl</artifactId>
			<version>1.2</version>
		</dependency>
		<dependency>
			<groupId>javax.transaction</groupId>
			<artifactId>jta</artifactId>
			<version>1.1</version>
		</dependency>

		<dependency>
			<groupId>javax.servlet</groupId>
			<artifactId>servlet-api</artifactId>
			<version>2.5</version>
		</dependency>
		<dependency>
			<groupId>net.sf.ehcache</groupId>
			<artifactId>ehcache-core</artifactId>
			<version>2.4.3</version>
		</dependency>

		<dependency>
			<groupId>org.hsqldb</groupId>
			<artifactId>hsqldb</artifactId>
			<version>2.3.2</version>
		</dependency>

		<dependency>
			<groupId>org.apache.shiro</groupId>
			<artifactId>shiro-all</artifactId>
			<version>${org.apache.shiro.version}</version>
		</dependency>
		<dependency>
			<groupId>org.hibernate</groupId>
			<artifactId>hibernate</artifactId>
			<version>${org.hibernate.version}</version>
		</dependency>
		<dependency>
			<groupId>org.hibernate</groupId>
			<artifactId>hibernate-core</artifactId>
			<version>${org.hibernate.version}</version>
		</dependency>
		<dependency>
			<groupId>org.hibernate</groupId>
			<artifactId>hibernate-ehcache</artifactId>
			<version>${org.hibernate.version}</version>
		</dependency>

		<dependency>
			<groupId>org.hibernate</groupId>
			<artifactId>hibernate-annotations</artifactId>
			<version>3.4.0.GA</version>
		</dependency>
		<dependency>
			<groupId>org.hibernate</groupId>
			<artifactId>hibernate-c3p0</artifactId>
			<version>${org.hibernate.version}</version>
		</dependency>

		<dependency>
			<groupId>org.hibernate</groupId>
			<artifactId>hibernate-entitymanager</artifactId>
			<version>${org.hibernate.version}</version>
		</dependency>
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-core</artifactId>
			<version>${org.springframework.version}</version>
		</dependency>

		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-expression</artifactId>
			<version>${org.springframework.version}</version>
		</dependency>

		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-beans</artifactId>
			<version>${org.springframework.version}</version>
		</dependency>

		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-aop</artifactId>
			<version>${org.springframework.version}</version>
		</dependency>

		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-context</artifactId>
			<version>${org.springframework.version}</version>
		</dependency>

		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-context-support</artifactId>
			<version>${org.springframework.version}</version>
		</dependency>

		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-tx</artifactId>
			<version>${org.springframework.version}</version>
		</dependency>

		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-jdbc</artifactId>
			<version>${org.springframework.version}</version>
		</dependency>

		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-orm</artifactId>
			<version>${org.springframework.version}</version>
		</dependency>

		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-oxm</artifactId>
			<version>${org.springframework.version}</version>
		</dependency>

		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-web</artifactId>
			<version>${org.springframework.version}</version>
		</dependency>

		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-webmvc</artifactId>
			<version>${org.springframework.version}</version>
		</dependency>

		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-webmvc-portlet</artifactId>
			<version>${org.springframework.version}</version>
		</dependency>

		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-test</artifactId>
			<version>${org.springframework.version}</version>
			<scope>test</scope>
		</dependency>

	</dependencies>

	<!-- 项目编译 -->

	<build>
		<!-- 项目资源路径引入 -->
		<resources>
			<resource>
				<directory>src/main/resources</directory>
				<filtering>true</filtering>
			</resource>
		</resources>

		<!-- 项目打包插件 -->
		<plugins>
			<plugin>
				<groupId>org.apache.maven.plugins</groupId>
				<artifactId>maven-compile-plugin</artifactId>
				<version>2.1</version>
				<configuration>
					<skip>true</skip>
				</configuration>
			</plugin>
			<plugin>
				<groupId>org.apache.maven.plugins</groupId>
				<artifactId>maven-surfire-plugin</artifactId>
				<version>2.5</version>
				<configuration>
					<skip>true</skip>
				</configuration>
			</plugin>
		</plugins>

	</build>

	<!-- 配置可变版本号 （Hibernate、spring、shiro） -->
	<properties>
		<javax.servlet.version>3.1.0</javax.servlet.version>
		<org.apache.shiro.version>1.2.3</org.apache.shiro.version>
		<org.hibernate.version>3.6.10.Final</org.hibernate.version>
		<org.springframework.version>3.0.2.RELEASE</org.springframework.version>
		<developer.organization><![CDATA[scengine]]></developer.organization>
	</properties>


</project>
```

注：${*.version}此处实现了Hibernate和Spring版本统一管理，以前看到别人用过，不知道在哪里实现版本控制，今天网上查阅了不少资料终于明白怎么做了。


#### 3.修改web.xml文件

由于示例中的*-servlet.xml,此处改为了boonyashiro-servlet.xml，此中内容如下：



```
<?xml version="1.0" encoding="UTF-8"?>
<!-- ~ Licensed to the Apache Software Foundation (ASF) under one ~ or more 
	contributor license agreements. See the NOTICE file ~ distributed with this 
	work for additional information ~ regarding copyright ownership. The ASF 
	licenses this file ~ to you under the Apache License, Version 2.0 (the ~ 
	"License"); you may not use this file except in compliance ~ with the License. 
	You may obtain a copy of the License at ~ ~ http://www.apache.org/licenses/LICENSE-2.0 
	~ ~ Unless required by applicable law or agreed to in writing, ~ software 
	distributed under the License is distributed on an ~ "AS IS" BASIS, WITHOUT 
	WARRANTIES OR CONDITIONS OF ANY ~ KIND, either express or implied. See the 
	License for the ~ specific language governing permissions and limitations 
	~ under the License. -->
<web-app xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xmlns="http://java.sun.com/xml/ns/javaee"
	xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd"
	metadata-complete="false" version="2.5">
	<context-param>
		<param-name>contextConfigLocation</param-name>
		<param-value>
            /WEB-INF/applicationContext.xml
        </param-value>
	</context-param>
	<context-param>
		<param-name>webAppRootKey</param-name>
		<param-value>boonyashiro-springhibernate.webapp.root</param-value>
	</context-param>
	<listener>
		<listener-class>org.springframework.web.util.Log4jConfigListener</listener-class>
	</listener>
	<listener>
		<listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
	</listener>
	<filter>
		<filter-name>openSessionInViewFilter</filter-name>
		<filter-class>org.springframework.orm.hibernate3.support.OpenSessionInViewFilter</filter-class>
	</filter>
	<filter>
		<filter-name>shiroFilter</filter-name>
		<filter-class>org.springframework.web.filter.DelegatingFilterProxy</filter-class>
	</filter>
	<filter-mapping>
		<filter-name>openSessionInViewFilter</filter-name>
		<url-pattern>/s/*</url-pattern>
	</filter-mapping>
	<filter-mapping>
		<filter-name>shiroFilter</filter-name>
		<url-pattern>/s/*</url-pattern>
	</filter-mapping>
	<span style="color:#ff0000;"><strong><servlet>
		<servlet-name>boonyashiro</servlet-name>
		<servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
		<load-on-startup>1</load-on-startup>
	</servlet>
	<servlet-mapping>
		<servlet-name>boonyashiro</servlet-name>
		<url-pattern>/s/*</url-pattern>
	</servlet-mapping></strong></span>
	<welcome-file-list>
		<welcome-file>/index.jsp</welcome-file>
	</welcome-file-list>
	<error-page>
		<error-code>401</error-code>
		<location>/unauthorized.jsp</location>
	</error-page>

</web-app>
```

注：此处关键处是org.springframework.web.servlet.DispatcherServlet的配置，它的servlet-name与*-servlet.xml是一致的，*代表servlet的名字。


#### 5.Maven依赖不成功处理方法

1、在google 中搜寻 XX Maven repository。

2、下载jar失败：删除.m2/repository/下对应目录的*.pom.lastUpdate（这种文件一旦存在则Maven将不会再更新下载jar），可以手动下载对应版本扔到改对应路径下。

注：删除的时候如果删除不掉需要关闭IDE删除后重新启动就可以了。

#### 6.Shiro配置成果展示

1、登录

![](https://img-blog.csdn.net/20140723173724166?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


2、主页

![](https://img-blog.csdn.net/20140723173747722?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


3、控制台输出

![](https://img-blog.csdn.net/20140723173850216?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


只要有足够的信心，要做成一件事是很容易的。

#### 7.安装打包

        好久没用Maven了，有个小小的疑惑，为什么我的项目右键Run as没有 mvn package命令了呢？我的Maven是org.apache.maven 3.2.2，可能是新版本将命令mvn package囊括在了mvn intall上了。我用mvn install打出了程序包，效果跟之前用的mvn package 一致。下图是我的打包项目结构截图：

![](https://img-blog.csdn.net/20140724104107140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


用过Maven的都知道target目录是项目编译打包后的项目文件存放的地方。









