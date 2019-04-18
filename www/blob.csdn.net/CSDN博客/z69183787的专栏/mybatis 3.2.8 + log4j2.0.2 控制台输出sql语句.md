# mybatis 3.2.8 + log4j2.0.2 控制台输出sql语句 - z69183787的专栏 - CSDN博客
2016年07月12日 20:53:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1899
mybatis3.2.7有一个bug,使用log4j2 (2.0.2)版本时,会找不到类 ,导致启动失败,详见
[https://github.com/mybatis/mybatis-3/issues/235](https://github.com/mybatis/mybatis-3/issues/235)
但没过多久 , 3.2.8就已经修复了这个bug , 最新的mybatis3.2.8下载地址为:
[https://github.com/mybatis/mybatis-3/releases](https://github.com/mybatis/mybatis-3/releases)
mybatis 3.2.8 整合 log4j2.0.2并不复杂 , 如果用spring-mvc做为web框架 , 以下是使用步骤:
1. pom.xml添加依赖项
```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 <dependency>
 2             <groupId>org.slf4j</groupId>
 3             <artifactId>slf4j-api</artifactId>
 4             <version>1.7.7</version>
 5         </dependency>
 6         <dependency>
 7             <groupId>org.apache.logging.log4j</groupId>
 8             <artifactId>log4j-api</artifactId>
 9             <version>${log4j2.version}</version>
10         </dependency>
11         <dependency>
12             <groupId>org.apache.logging.log4j</groupId>
13             <artifactId>log4j-core</artifactId>
14             <version>${log4j2.version}</version>
15         </dependency>
16         <dependency>
17             <groupId>org.apache.logging.log4j</groupId>
18             <artifactId>log4j-web</artifactId>
19             <version>${log4j2.version}</version>
20         </dependency>
21         <dependency>
22             <groupId>org.apache.logging.log4j</groupId>
23             <artifactId>log4j-slf4j-impl</artifactId>
24             <version>2.0.2</version>
25         </dependency>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
slf4j的二项好象不加也行 , 大家可以自行试试
2. web.xml中增加listener
```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1     <listener>
 2         <listener-class>org.apache.logging.log4j.web.Log4jServletContextListener</listener-class>
 3     </listener>
 4     <filter>
 5         <filter-name>log4jServletFilter</filter-name>
 6         <filter-class>org.apache.logging.log4j.web.Log4jServletFilter</filter-class>
 7     </filter>
 8     <filter-mapping>
 9         <filter-name>log4jServletFilter</filter-name>
10         <url-pattern>/*</url-pattern>
11         <dispatcher>REQUEST</dispatcher>
12         <dispatcher>FORWARD</dispatcher>
13         <dispatcher>INCLUDE</dispatcher>
14         <dispatcher>ERROR</dispatcher>
15     </filter-mapping>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
上面这一段,加在web.xml最开头（注：这一步好象也可以去掉）
3. maven项目的resouces目录下 , 放置log4j2.xml
```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 <?xml version="1.0" encoding="UTF-8"?>
 2 <Configuration status="off" monitorInterval="1800">    
 3 
 4     <Appenders>
 5         <Console name="Console" target="SYSTEM_OUT">
 6             <PatternLayout pattern="%d{HH:mm:ss.SSS} [%t] %-5level %logger{36} - %msg%n" />
 7         </Console>        
 8     </Appenders>
 9 
10     <Loggers>            
11         <Root level="debug">
12             <AppenderRef ref="Console" />
13         </Root>
14     </Loggers>
15 </Configuration>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
只要把root logger的级别调成debug级别即可
4.如果采用mybatis-spring项目来集成mybatis, spring配置文件参考下面这样:
```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1     <bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
2         <property name="dataSource" ref="dataSource" />
3         <property name="configLocation" value="classpath:mybatis-config.xml"></property>
4         <property name="typeAliasesPackage" value="com.cnblogs.yjmyzz.entity"></property>
5         <property name="mapperLocations" value="classpath:mybatis/**/*.xml"></property>
6     </bean>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
<property name="configLocation" value="classpath:mybatis-config.xml"></property> 这一行,指定了mybatis的主配置文件
5.mybatis-config配置文件
```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1 <?xml version="1.0" encoding="UTF-8" ?>
2 <!DOCTYPE configuration PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
3 "http://mybatis.org/dtd/mybatis-3-config.dtd">
4 <configuration>
5     <settings>
6         <setting name="logImpl" value="LOG4J2" />
7     </settings>
8 </configuration>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
指定mybatis使用log4j2来记录日志
如果在jboss eap 6.x上部署 , 启动时会有一个error , 但并不影响项目正常运行 , 这是log4j2的一个bug , 估计在后续版本中会修复,详情见:[http://mail-archives.apache.org/mod_mbox/logging-log4j-dev/201403.mbox/%3CJIRA.12696787.1393155172406.128647.1393632623629@arcas%3E](http://mail-archives.apache.org/mod_mbox/logging-log4j-dev/201403.mbox/%3CJIRA.12696787.1393155172406.128647.1393632623629@arcas%3E)
