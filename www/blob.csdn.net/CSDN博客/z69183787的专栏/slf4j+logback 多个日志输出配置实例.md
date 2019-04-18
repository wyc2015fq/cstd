# slf4j+logback 多个日志输出配置实例 - z69183787的专栏 - CSDN博客
2014年06月11日 16:53:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6520
一, Maven依赖包 pom.xml
 1<dependency>
 2<groupId>org.slf4j</groupId>
 3<artifactId>slf4j-api</artifactId>
 4<version>1.6.0</version>
 5<type>jar</type>
 6<scope>compile</scope>
 7</dependency>
 8<dependency>
 9<groupId>ch.qos.logback</groupId>
10<artifactId>logback-core</artifactId>
11<version>0.9.28</version>
12<type>jar</type>
13</dependency>
14<dependency>
15<groupId>ch.qos.logback</groupId>
16<artifactId>logback-classic</artifactId>
17<version>0.9.28</version>
18<type>jar</type>
19</dependency>
二, 代码使用
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
/** logger */
privatestatic Logger logger = LoggerFactory.getLogger(ExampleClass.class);
logger.debug("output logs");
三, 日志配置文件
配置多个logger
比如
第一个记载日志,
第二个输出告警
logback.xml
 1<?xml version="1.0"?>
 2<configuration>
 3<!--property file="/home/testProject/config/app.properties"/-->
 4<property file="src\main\resources\app.properties"/>
 5<appender name="FILE"  class="ch.qos.logback.core.rolling.RollingFileAppender">
 6<Encoding>UTF-8</Encoding>
 7<File>${path.log}/testProject.log</File>
 8<Prudent>true</Prudent>
 9<rollingPolicy class="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
10<FileNamePattern>${path.log}/testProject-%d{yyyy-MM-dd_HH}-%i.log</FileNamePattern>
11<MaxHistory>10</MaxHistory>
12<TimeBasedFileNamingAndTriggeringPolicy class="ch.qos.logback.core.rolling.SizeAndTimeBasedFNATP">
13<MaxFileSize>5MB</MaxFileSize>
14</TimeBasedFileNamingAndTriggeringPolicy>
15</rollingPolicy>
16<layout class="ch.qos.logback.classic.PatternLayout">
17<Pattern>%d{yyyy-MM-dd HH:mm:ss.SSS} [%thread] %-5level %logger{36} - %msg%n</Pattern>
18</layout>
19</appender>
20<appender name="warningLoggerAppender"  class="ch.qos.logback.core.rolling.RollingFileAppender">
21<Encoding>UTF-8</Encoding>
22<File>${path.alarm}/testProject-warning.txt</File>
23<Prudent>true</Prudent>
24<rollingPolicy class="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
25<FileNamePattern>${path.alarm}/testProject-warning-%d{yyyyMMddHHmmss}.txt</FileNamePattern>
26<MaxHistory>3</MaxHistory>
27<TimeBasedFileNamingAndTriggeringPolicy class="ch.qos.logback.core.rolling.SizeAndTimeBasedFNATP">
28<MaxFileSize>5MB</MaxFileSize>
29</TimeBasedFileNamingAndTriggeringPolicy>
30</rollingPolicy>
31<layout class="ch.qos.logback.classic.PatternLayout">
32<!-- Pattern>%d{yyyy-MM-dd HH:mm:ss.SSS} [%thread] %-5level %logger{36} - %msg%n</Pattern-->
33<Pattern>%msg%n</Pattern>
34</layout>
35</appender>
36
37<logger name="com.test">
38<level value="DEBUG"/><!-- "TRACE", "DEBUG" -->
39<appender-ref ref="FILE"/>
40</logger>
41<logger name="warningLogger">
42<level value="TRACE"/><!-- "TRACE", "DEBUG" -->
43<appender-ref ref="warningLoggerAppender"/>
44</logger>
45</configuration>
