# Mybatis3.2+版本 log4j2 Debug 模式打印sql语句 - z69183787的专栏 - CSDN博客
2016年07月12日 20:48:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7004
pom：
```
<slf4j.version>1.7.10</slf4j.version>
<log4j2.version>2.3</log4j2.version>
<mybatis-spring.version>1.2.2</mybatis-spring.version>
<mybatis.version>3.2.8</mybatis.version>
```
```
<!-- log4j2-->
<dependency>
    <groupId>org.slf4j</groupId>
    <artifactId>slf4j-api</artifactId>
    <version>${slf4j.version}</version>
</dependency>
<dependency>
    <groupId>org.slf4j</groupId>
    <artifactId>jcl-over-slf4j</artifactId>
    <version>${slf4j.version}</version>
</dependency>
<dependency>
    <groupId>org.slf4j</groupId>
    <artifactId>jul-to-slf4j</artifactId>
    <version>${slf4j.version}</version>
</dependency>
<dependency>
    <groupId>org.apache.logging.log4j</groupId>
    <artifactId>log4j-api</artifactId>
    <version>${log4j2.version}</version>
</dependency>
<dependency>
    <groupId>org.apache.logging.log4j</groupId>
    <artifactId>log4j-core</artifactId>
    <version>${log4j2.version}</version>
</dependency>
<dependency>
    <groupId>org.apache.logging.log4j</groupId>
    <artifactId>log4j-slf4j-impl</artifactId>
    <version>${log4j2.version}</version>
</dependency>
<dependency>
    <groupId>org.apache.logging.log4j</groupId>
    <artifactId>log4j-1.2-api</artifactId>
    <version>${log4j2.version}</version>
</dependency>
<!-- 异步日志 第三方-->
<dependency>
    <groupId>com.lmax</groupId>
    <artifactId>disruptor</artifactId>
    <version>${disruptor.version}</version>
</dependency>
<!-- log4j2-->
```
mybatis-configuration.xml：
```
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration PUBLIC "-//mybatis.org//DTD Config 3.0//EN" "http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
    <settings>
        <setting name="logImpl" value="LOG4J2" />
    </settings>
</configuration>
```
log4j2.xml：
```
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="WARN" >
    <properties>
        <property name="LOG_HOME">/data/logs/xxxx-service</property>
    </properties>
    <Appenders>
        <Console name="Console" target="SYSTEM_OUT">
            <PatternLayout pattern="[%date{yyyy-MM-dd HH:mm:ss.SSS}][%thread][%level][%class][%line]:%message%n" />
        </Console>
        <RollingRandomAccessFile name="infoLog" fileName="${LOG_HOME}/app.log"
                                 filePattern="${LOG_HOME}/app.%d{yyyy-MM-dd}-%i.log.gz" append="true">
            <PatternLayout pattern="[%date{yyyy-MM-dd HH:mm:ss.SSS}][%thread][%level][%class][%line]:%message%n"/>
            <Filters>
                <ThresholdFilter level="info" onMatch="ACCEPT" onMismatch="NEUTRAL"/>
            </Filters>
            <Policies>
                <TimeBasedTriggeringPolicy interval="1" modulate="true"/>  <!-- 对应 filePattern维度，此处为天数-->
            </Policies>
            <DefaultRolloverStrategy max="30"/> <!-- 日志自动删除，最大个数 -->
        </RollingRandomAccessFile>
        <RollingRandomAccessFile name="warnLog" fileName="${LOG_HOME}/warn.log"
                                 filePattern="${LOG_HOME}/warn.%d{yyyy-MM-dd}-%i.log.gz" append="true">
            <Filters>
                <ThresholdFilter level="error" onMatch="DENY" onMismatch="NEUTRAL"/>
                <ThresholdFilter level="warn" onMatch="ACCEPT" onMismatch="DENY"/>
            </Filters>
            <PatternLayout pattern="[%date{yyyy-MM-dd HH:mm:ss.SSS}][%thread][%level][%class][%line]:%message%n"/>
            <Policies>
                <TimeBasedTriggeringPolicy interval="1" modulate="true"/>
            </Policies>
            <DefaultRolloverStrategy max="30"/>
        </RollingRandomAccessFile>
        <RollingRandomAccessFile name="errorLog" fileName="${LOG_HOME}/error.log"
                                 filePattern="${LOG_HOME}/error.%d{yyyy-MM-dd}-%i.log.gz" append="true">
            <Filters>
                <ThresholdFilter level="ERROR" onMatch="ACCEPT" onMismatch="DENY"/>
            </Filters>
            <PatternLayout pattern="[%date{yyyy-MM-dd HH:mm:ss.SSS}][%thread][%level][%class][%line]:%message%n"/>
            <Policies>
                <TimeBasedTriggeringPolicy interval="1" modulate="true"/>
            </Policies>
            <DefaultRolloverStrategy max="30"/>
        </RollingRandomAccessFile>
        <RollingRandomAccessFile name="jobLog" fileName="${LOG_HOME}/job.log"
                                 filePattern="${LOG_HOME}/job.%d{yyyy-MM-dd}-%i.log.gz" append="true">
            <PatternLayout pattern="[emidas-lottery-job][%date{yyyy-MM-dd HH:mm:ss.SSS}][%thread][%level][%class][%line]:%message%n"/>
            <Filters>
                <ThresholdFilter level="info" onMatch="ACCEPT" onMismatch="NEUTRAL"/>
            </Filters>
            <Policies>
                <TimeBasedTriggeringPolicy interval="1" modulate="true"/>
            </Policies>
            <DefaultRolloverStrategy max="30"/>
        </RollingRandomAccessFile>
        <RollingRandomAccessFile name="lotteryLog" fileName="${LOG_HOME}/lottery.log"
                                 filePattern="${LOG_HOME}/lottery.%d{yyyy-MM-dd}-%i.log.gz" append="true">
            <PatternLayout pattern="[main][%date{yyyy-MM-dd HH:mm:ss.SSS}][%thread][%level][%class][%line]:%message%n"/>
            <Filters>
                <ThresholdFilter level="info" onMatch="ACCEPT" onMismatch="NEUTRAL"/>
            </Filters>
            <Policies>
                <TimeBasedTriggeringPolicy interval="1" modulate="true"/>
            </Policies>
            <DefaultRolloverStrategy max="30"/>
        </RollingRandomAccessFile>
        <RollingRandomAccessFile name="bonusLog" fileName="${LOG_HOME}/bonus.log"
                                 filePattern="${LOG_HOME}/bonus.%d{yyyy-MM-dd}-%i.log.gz" append="true">
            <PatternLayout pattern="[main][%date{yyyy-MM-dd HH:mm:ss.SSS}][%thread][%level][%class][%line]:%message%n"/>
            <Filters>
                <ThresholdFilter level="info" onMatch="ACCEPT" onMismatch="NEUTRAL"/>
            </Filters>
            <Policies>
                <TimeBasedTriggeringPolicy interval="1" modulate="true"/>
            </Policies>
            <DefaultRolloverStrategy max="30"/>
        </RollingRandomAccessFile>
        <RollingRandomAccessFile name="accessLog" fileName="${LOG_HOME}/access.log"
                                 filePattern="${LOG_HOME}/access.%d{yyyy-MM-dd}-%i.log.gz" append="true">
            <PatternLayout pattern="[main][%date{yyyy-MM-dd HH:mm:ss.SSS}][%thread][%level][%class][%line]:%message%n"/>
            <Filters>
                <ThresholdFilter level="info" onMatch="ACCEPT" onMismatch="NEUTRAL"/>
            </Filters>
            <Policies>
                <TimeBasedTriggeringPolicy interval="1" modulate="true"/>
            </Policies>
            <DefaultRolloverStrategy max="30"/>
        </RollingRandomAccessFile>
        <RollingRandomAccessFile name="alertLog" fileName="${LOG_HOME}/alert.log"
                                 filePattern="${LOG_HOME}/alert.%d{yyyy-MM-dd}-%i.log.gz" append="true">
            <PatternLayout pattern="[main][%date{yyyy-MM-dd HH:mm:ss.SSS}][%thread][%level][%class][%line]:%message%n"/>
            <Filters>
                <ThresholdFilter level="info" onMatch="ACCEPT" onMismatch="NEUTRAL"/>
            </Filters>
            <Policies>
                <TimeBasedTriggeringPolicy interval="1" modulate="true"/>
            </Policies>
            <DefaultRolloverStrategy max="30"/>
        </RollingRandomAccessFile>
        <Cat name="catAppender">
            <PatternLayout pattern="[%date{yyyy-MM-dd HH:mm:ss.SSS}][%thread][%level][%class][%line]:%message%n"/>
        </Cat>
    </Appenders>
    <Loggers>
        <!-- 第三方的软件日志级别 -->
        <AsyncLogger name="org.springframework" level="info" additivity="true">
            <AppenderRef ref="warnLog"/>
            <AppenderRef ref="errorLog"/>
        </AsyncLogger>
        <!-- 应用的日志级别 -->
        <AsyncLogger name="package.xxxx" level="info" additivity="true">
            <AppenderRef ref="infoLog"/>
            <AppenderRef ref="warnLog"/>
            <AppenderRef ref="errorLog"/>
        </AsyncLogger>
        <!--<logger name="java.sql.PreparedStatement" level="debug" additivity="true">-->
        <!--<AppenderRef ref="Console"/>-->
        <!--</logger>-->
        <!--<logger name="xxxx.dao.mapper" level="debug" additivity="false">-->
        <!--<AppenderRef ref="Console"/>-->
        <!--</logger>-->
        <AsyncLogger name="job" level="info" additivity="false">
            <AppenderRef ref="jobLog"/>
        </AsyncLogger>
        <AsyncLogger name="lottery" level="info" additivity="false">
            <AppenderRef ref="lotteryLog"/>
        </AsyncLogger>
        <AsyncLogger name="bonus" level="info" additivity="false">
            <AppenderRef ref="bonusLog"/>
        </AsyncLogger>
        <AsyncLogger name="access" level="info" additivity="false">
            <AppenderRef ref="accessLog"/>
        </AsyncLogger>
        <AsyncLogger name="alert" level="info" additivity="false">
            <AppenderRef ref="alertLog"/>
        </AsyncLogger>
        <!-- 剩余其他的日志级别 -->
        <asyncRoot  level="info" includeLocation="true">
            <AppenderRef ref="Console" />
            <AppenderRef ref="infoLog" />
            <AppenderRef ref="catAppender"/>
        </asyncRoot>
    </Loggers>
</Configuration>
```
