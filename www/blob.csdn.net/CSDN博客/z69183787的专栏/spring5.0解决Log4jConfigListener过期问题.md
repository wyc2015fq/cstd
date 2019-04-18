# spring5.0解决Log4jConfigListener过期问题 - z69183787的专栏 - CSDN博客
2018年03月24日 15:51:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：628
[https://blog.csdn.net/zhiyuzhe/article/details/78850238](https://blog.csdn.net/zhiyuzhe/article/details/78850238)
升级到spring5.0，发现之前web.xml 里配置的log监听Log4jConfigListener被spring废弃了，百度了一下，没有发现答案。 
这个是web.xml之前的配置：
```
<context-param> 
        <param-name>log4jConfigLocation</param-name>
        <param-value>classpath:log4j.properties</param-value>
    </context-param>
    <context-param>  
        <param-name>webAppRootKey</param-name>
        <param-value>webApp.root</param-value>
    </context-param>
    <context-param>
        <param-name>log4jRefreshInterval</param-name>
        <param-value>6000</param-value>
    </context-param>
    <listener>
        <listener-class>org.springframework.web.util.Log4jConfigListener</listener-class>
    </listener>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
上官网瞅了下，版本5的api里已经没有这个方法了，切换到版本4.3.13，发现原文是这么说的。spring建议用log4j2 来替换这个类。以下为官方原文
```
Deprecated. 
as of Spring 4.2.1, in favor of Apache Log4j 2 (following Apache's EOL declaration for log4j 1.x)
@Deprecated
public class Log4jConfigListener
extends java.lang.Object
implements ServletContextListener
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
于是看了一下log4j2，实现过程记录一下： 
首先引入全新的log4j2 依赖jar包
```xml
<dependency>
        <groupId>org.apache.logging.log4j</groupId>
        <artifactId>log4j-api</artifactId>
        <version>2.10.0</version>
    </dependency>
    <dependency>
        <groupId>org.apache.logging.log4j</groupId>
        <artifactId>log4j-core</artifactId>
        <version>2.10.0</version>
    </dependency>
    <dependency>
      <groupId>org.apache.logging.log4j</groupId>
      <artifactId>log4j-web</artifactId>
      <version>2.9.1</version>
    </dependency>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
web.xml 的配置如下：
```xml
<!-- log4j2-begin -->
     <listener>
         <listener-class>org.apache.logging.log4j.web.Log4jServletContextListener</listener-class>
    </listener>
     <filter>
         <filter-name>log4jServletFilter</filter-name>
         <filter-class>org.apache.logging.log4j.web.Log4jServletFilter</filter-class>
     </filter>
     <filter-mapping>
         <filter-name>log4jServletFilter</filter-name>
         <url-pattern>/*</url-pattern>
         <dispatcher>REQUEST</dispatcher>
         <dispatcher>FORWARD</dispatcher>
         <dispatcher>INCLUDE</dispatcher>
         <dispatcher>ERROR</dispatcher>
     </filter-mapping>
     <!-- log4j2-end -->
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
src/main/resources`目录下新增log4j2.xml 文件，这个文件名是官方默认的
```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="OFF" monitorInterval="1800">
    <properties>
        <property name="LOG_HOME">/WEB-INF/logs</property>
        <property name="FILE_NAME">finance-pay</property>
    </properties>
    <Appenders>
        <Console name="Console" target="SYSTEM_OUT">
            <PatternLayout pattern="%d{HH:mm:ss.SSS} [%t] %-5level %logger{36} - %msg%n" />
        </Console>
        <RollingFile name="running-log" fileName="${LOG_HOME}/${FILE_NAME}.log"
            filePattern="${LOG_HOME}/$${date:yyyy-MM}/${FILE_NAME}-%d{yyyy-MM-dd}-%i.log.gz"
            immediateFlush="true">
            <PatternLayout
                pattern="%date{yyyy-MM-dd HH:mm:ss.SSS} %level [%thread][%file:%line] - %msg%n" />
            <Policies>
                <TimeBasedTriggeringPolicy />
                <SizeBasedTriggeringPolicy size="10 MB" />
            </Policies>
            <DefaultRolloverStrategy max="20" />
        </RollingFile>
    </Appenders>
    <Loggers>
        <!-- <Logger name="com.cssweb.test.app" level="trace" additivity="true"> 
            <AppenderRef ref="running-log" /> </Logger> -->
        <Root level="info">
               <!-- 这里是输入到文件，很重要-->
            <AppenderRef ref="running-log" />
            <!-- 这里是输入到控制台-->
            <AppenderRef ref="Console" />
        </Root>
    </Loggers>
</Configuration>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
编写一个测试类还试试吧：
```
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
public class TestLog4j2 {
     static Logger logger = LogManager.getLogger(LogManager.ROOT_LOGGER_NAME);
 public static void main(String[] args) {
         logger.trace("trace message");
         logger.debug("debug message");
         logger.info("info message");
         logger.warn("warn message");
         logger.error("error message");
         logger.fatal("fatal message");
         System.out.println("Hello World!");
     }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
发现已经写进去了，大功告成。
